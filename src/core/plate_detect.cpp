#include "easypr/core/plate_detect.h"
#include "easypr/util/util.h"
#include "easypr/config.h"

namespace easypr {

  CPlateDetect::CPlateDetect() {
    m_plateLocate = new CPlateLocate();

    m_maxPlates = 3;
    m_type = 0;

    m_showDetect = false;
  }

  CPlateDetect::~CPlateDetect() { SAFE_RELEASE(m_plateLocate); }

  //车牌检测
  int CPlateDetect::plateDetect(Mat src, std::vector<CPlate> &resultVec, int type,bool showDetectArea, int img_index) 
  {
    std::vector<CPlate> sobel_Plates;
    sobel_Plates.reserve(16);
    std::vector<CPlate> color_Plates;
    color_Plates.reserve(16);
    std::vector<CPlate> mser_Plates;
    mser_Plates.reserve(16);

    std::vector<CPlate> all_result_Plates;
    all_result_Plates.reserve(64);

	//并行使用三种定位车牌的方式，sobel边缘检测，颜色定位，mser定位
	//每个section都是并行执行
#pragma omp parallel sections
    {
#pragma omp section
      {
        if (!type || type & PR_DETECT_SOBEL) {
          m_plateLocate->plateSobelLocate(src, sobel_Plates, img_index);
        }
      }
#pragma omp section
      {
        if (!type || type & PR_DETECT_COLOR) {
          m_plateLocate->plateColorLocate(src, color_Plates, img_index);
        }
      }
#pragma omp section
      {
        if (!type || type & PR_DETECT_CMSER) {
          m_plateLocate->plateMserLocate(src, mser_Plates, img_index);
        }
      }
    }
	//使用sobel定位出的车牌结果push到all_result_Plates中
    for (auto plate : sobel_Plates)
	{
      plate.setPlateLocateType(SOBEL);
      all_result_Plates.push_back(plate);
    }
	//使用颜色定位的车牌结果push到all_result_Plates中
    for (auto plate : color_Plates) 
	{
      plate.setPlateLocateType(COLOR);
      all_result_Plates.push_back(plate);
    }
	//使用cmser定位的车牌结果push到all_result_Plates中
    for (auto plate : mser_Plates)
	{
      plate.setPlateLocateType(CMSER);
      all_result_Plates.push_back(plate);
    }

    // use nms to judge plate
	//将三种定位的结果使用nms判断是否为车牌，并将结果输出到resultVec中
    PlateJudge::instance()->plateJudgeUsingNMS(all_result_Plates, resultVec, m_maxPlates);
	
	//如果设定需要显示
    if (showDetectArea || getDetectShow()) 
	{
      int index = 0;
      Mat result;
      src.copyTo(result);

      for (size_t j = 0; j < resultVec.size(); j++) 
	  {
        CPlate item = resultVec[j];
		//将车牌类型CPlate转换为mat类型
        Mat plate = item.getPlateMat();

        int height = 36;
        int width = 136;
        if (height * index + height < result.rows) 
		{
          Mat imageRoi = result(Rect(0, 0 + height * index, width, height));
          addWeighted(imageRoi, 0, plate, 1, 0, imageRoi);
        }
        index++;
		//车牌的最小外接旋转矩形
        RotatedRect minRect = item.getPlatePos();
        Point2f rect_points[4];
		//将矩形的4个点坐标保存到rect_points中
        minRect.points(rect_points);

		//白色
        Scalar lineColor = Scalar(255, 255, 255);
		
        if (item.getPlateLocateType() == SOBEL) lineColor = Scalar(255, 0, 0);//sobel，使用红色标记
        if (item.getPlateLocateType() == COLOR) lineColor = Scalar(0, 255, 0);//颜色定位车牌，用绿色标记
        if (item.getPlateLocateType() == CMSER) lineColor = Scalar(0, 0, 255);//cmser定位车牌，用蓝色定位

        for (int j = 0; j < 4; j++)
          line(result, rect_points[j], rect_points[(j + 1) % 4], lineColor, 2, 8);//在四个点上划线表示
      }
      showResult(result, img_index);
    }
    return 0;
  }

  //车牌检测函数
  int CPlateDetect::plateDetect(Mat src, std::vector<CPlate> &resultVec, int img_index) 
  {
	//再调用另外一个函数，多一个bool参数，可以选择是否显示：使用不同定位方法定位出的车牌
    int result = plateDetect(src, resultVec, m_type, true, img_index);
    return result;
  }

  void CPlateDetect::LoadSVM(std::string path) {
    PlateJudge::instance()->LoadModel(path);
  }


  Mat CPlateDetect::showResult(const Mat &result, int img_index) {
    namedWindow("EasyPR", CV_WINDOW_AUTOSIZE);

    const int RESULTWIDTH = kShowWindowWidth;   // 640 930
    const int RESULTHEIGHT = kShowWindowHeight;  // 540 710

    Mat img_window;
    img_window.create(RESULTHEIGHT, RESULTWIDTH, CV_8UC3);

    int nRows = result.rows;
    int nCols = result.cols;

    Mat result_resize;
    if (nCols <= img_window.cols && nRows <= img_window.rows) {
      result_resize = result;

    }
    else if (nCols > img_window.cols && nRows <= img_window.rows) {
      float scale = float(img_window.cols) / float(nCols);
      resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);

    }
    else if (nCols <= img_window.cols && nRows > img_window.rows) {
      float scale = float(img_window.rows) / float(nRows);
      resize(result, result_resize, Size(), scale, scale, CV_INTER_AREA);

    }
    else if (nCols > img_window.cols && nRows > img_window.rows) {
      Mat result_middle;
      float scale = float(img_window.cols) / float(nCols);
      resize(result, result_middle, Size(), scale, scale, CV_INTER_AREA);

      if (result_middle.rows > img_window.rows) {
        float scale = float(img_window.rows) / float(result_middle.rows);
        resize(result_middle, result_resize, Size(), scale, scale, CV_INTER_AREA);
      }
      else {
        result_resize = result_middle;
      }
    }
    else {
      result_resize = result;
    }

    Mat imageRoi = img_window(Rect((RESULTWIDTH - result_resize.cols) / 2,
      (RESULTHEIGHT - result_resize.rows) / 2,
      result_resize.cols, result_resize.rows));
    addWeighted(imageRoi, 0, result_resize, 1, 0, imageRoi);

    if (1) {
      imshow("EasyPR", img_window);
      waitKey(0);
      destroyWindow("EasyPR");
    }

    if (1) {
      std::stringstream ss(std::stringstream::in | std::stringstream::out);
      ss << "resources/image/tmp/Result/plate_" << img_index << ".jpg";
      imwrite(ss.str(), img_window);
    }

    return img_window;
  }
}