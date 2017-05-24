#include "easypr/core/plate_locate.h"
#include "easypr/core/core_func.h"
#include "easypr/util/util.h"
#include "easypr/core/params.h"

using namespace std;

namespace easypr {

const float DEFAULT_ERROR = 0.9f;    // 0.6
const float DEFAULT_ASPECT = 3.75f;  // 3.75

CPlateLocate::CPlateLocate() {
  m_GaussianBlurSize = DEFAULT_GAUSSIANBLUR_SIZE;
  m_MorphSizeWidth = DEFAULT_MORPH_SIZE_WIDTH;
  m_MorphSizeHeight = DEFAULT_MORPH_SIZE_HEIGHT;

  m_error = DEFAULT_ERROR;
  m_aspect = DEFAULT_ASPECT;
  m_verifyMin = DEFAULT_VERIFY_MIN;
  m_verifyMax = DEFAULT_VERIFY_MAX;

  m_angle = DEFAULT_ANGLE;

  m_debug = DEFAULT_DEBUG;
}
//识别日常生活中拍摄到的车牌，生活模式
void CPlateLocate::setLifemode(bool param) 
{
  if (param) {
	  //在生活模式下，各个参数和工业采集到的图像的参数设置不同
    setGaussianBlurSize(5);//高斯模糊半径为5
    setMorphSizeWidth(10);//形态学操作模板宽度为10
    setMorphSizeHeight(3);//形态学操作模板高度为3
    setVerifyError(0.75);//
    setVerifyAspect(4.0);
    setVerifyMin(1);
    setVerifyMax(200);
  } else {
    setGaussianBlurSize(DEFAULT_GAUSSIANBLUR_SIZE);
    setMorphSizeWidth(DEFAULT_MORPH_SIZE_WIDTH);
    setMorphSizeHeight(DEFAULT_MORPH_SIZE_HEIGHT);
    setVerifyError(DEFAULT_ERROR);
    setVerifyAspect(DEFAULT_ASPECT);
    setVerifyMin(DEFAULT_VERIFY_MIN);
    setVerifyMax(DEFAULT_VERIFY_MAX);
  }
}
//对minAreaRect获得的最小外接矩形，用纵横比和面积比进行判断
bool CPlateLocate::verifySizes(RotatedRect mr) 
{
  float error = m_error;
  // Spain car plate size: 52x11 aspect 4,7272
  // China car plate size: 440mm*140mm，aspect 3.142857

  // Real car plate size: 136 * 32, aspect 4
  //宽高比
  float aspect = m_aspect;
  
  //设定一个面积最大值max与面积最小值min。判断矩形的面积area是否满足在max与min之间
  // Set a min and max area. All other patchs are discarded
  // int min= 1*aspect*1; // minimum area
  // int max= 2000*aspect*2000; // maximum area
  int min = 34 * 8 * m_verifyMin;  // minimum area
  int max = 34 * 8 * m_verifyMax;  // maximum area

  // Get only patchs that match to a respect ratio.
  //设立一个偏差率error，根据这个偏差率计算最大和最小的宽高比rmax、rmin。判断矩形的r是否满足在rmax、rmin之间
  float rmin = aspect - aspect * error;//车牌区域的最小宽高比
  float rmax = aspect + aspect * error;//车牌区域的最大宽高比

  float area = mr.size.height * mr.size.width;//输入的矩形的面积
  float r = (float) mr.size.width / (float) mr.size.height;//输入矩形的宽高比
  //如果宽高比小于1，则高度大于宽度，竖着的车牌区域？
  if (r < 1) 
	  r = (float) mr.size.height / (float) mr.size.width;
  // cout << "area:" << area << endl;
  // cout << "r:" << r << endl;
  //如果输入的矩形的面积不在这个范围内，或者矩形的宽高比也不在这个范围内
  if ((area < min || area > max) || (r < rmin || r > rmax))
    return false;
  else
    return true;
}

//! mser search method
int CPlateLocate::mserSearch(const Mat &src,  vector<Mat> &out,
  vector<vector<CPlate>>& out_plateVec, bool usePlateMser, vector<vector<RotatedRect>>& out_plateRRect,
  int img_index, bool showDebug) 
{
  vector<Mat> match_grey;

  vector<CPlate> plateVec_blue;//存储蓝色车牌区域
  plateVec_blue.reserve(16);
  vector<RotatedRect> plateRRect_blue;//存储蓝色车牌矩形区域
  plateRRect_blue.reserve(16);

  vector<CPlate> plateVec_yellow;//存储黄色车牌区域
  plateVec_yellow.reserve(16);

  vector<RotatedRect> plateRRect_yellow;//存储黄色车牌矩形区域
  plateRRect_yellow.reserve(16);

  mserCharMatch(src, match_grey, plateVec_blue, plateVec_yellow, usePlateMser, plateRRect_blue, plateRRect_yellow, img_index, showDebug);

  out_plateVec.push_back(plateVec_blue);
  out_plateVec.push_back(plateVec_yellow);

  out_plateRRect.push_back(plateRRect_blue);
  out_plateRRect.push_back(plateRRect_yellow);

  out = match_grey;

  return 0;
}


int CPlateLocate::colorSearch(const Mat &src, const Color r, Mat &out,
                              vector<RotatedRect> &outRects) {
  Mat match_grey;

  // width is important to the final results;

  const int color_morph_width = 10;
  const int color_morph_height = 2;

  //使用颜色定位车牌的匹配区域
  //输出二值图（整幅图像中，像素点为255代表匹配区域，为0代表不匹配区域）
  colorMatch(src, match_grey, r, false);

  //if (m_debug) {
  //  utils::imwrite("resources/image/tmp/match_grey.jpg", match_grey);
  //}
  //在上一步得到的图像已经是二值图像，还有必要二值化操作吗？经调试发现不必使用二值化操作
  Mat src_threshold;
  threshold(match_grey, src_threshold, 0, 255,
            CV_THRESH_OTSU + CV_THRESH_BINARY);
  
  //得到形态学模板
  Mat element = getStructuringElement(
      MORPH_RECT, Size(color_morph_width, color_morph_height));
  //使用闭运算，消除缝隙，连接区域
  morphologyEx(src_threshold, src_threshold, MORPH_CLOSE, element);

  //if (m_debug) {
  //  utils::imwrite("resources/image/tmp/color.jpg", src_threshold);
  //}

  src_threshold.copyTo(out);

  vector<vector<Point>> contours;
  //对二值化后的图像查找轮廓
  findContours(src_threshold,
               contours,               // a vector of contours
               CV_RETR_EXTERNAL,
               CV_CHAIN_APPROX_NONE);  // all pixels of each contours

  vector<vector<Point>>::iterator itc = contours.begin();
  while (itc != contours.end()) {
    RotatedRect mr = minAreaRect(Mat(*itc));

    if (!verifySizes(mr))
      itc = contours.erase(itc);
    else {
      ++itc;
      outRects.push_back(mr);
    }
  }

  return 0;
}

//sobel第一次搜索矩形轮廓，寻找
int CPlateLocate::sobelFrtSearch(const Mat &src,vector<Rect_<float>> &outRects) 
{
  Mat src_threshold;
  //sobel边缘检测操作(高斯模糊，sobel边缘检测，二值化，闭运算)，输出二值化矩形区域
  sobelOper(src, src_threshold, m_GaussianBlurSize, m_MorphSizeWidth,
            m_MorphSizeHeight);

  //在二值化图像中查找封闭轮廓点
  vector<vector<Point>> contours;
  findContours(src_threshold,
               contours,               // a vector of contours
               CV_RETR_EXTERNAL,
               CV_CHAIN_APPROX_NONE);  // all pixels of each contours

  vector<vector<Point>>::iterator itc = contours.begin();

  vector<RotatedRect> first_rects;
  //将所有疑似矩形区域找出来
  while (itc != contours.end()) 
  {
	//寻找最小包围矩形
    RotatedRect mr = minAreaRect(Mat(*itc));
	//判断矩形是否符合要求
    if (verifySizes(mr)) 
	{
      first_rects.push_back(mr);

      float area = mr.size.height * mr.size.width;
      float r = (float) mr.size.width / (float) mr.size.height;
      if (r < 1) 
		  r = (float) mr.size.height / (float) mr.size.width;
    }
    ++itc;
  }
  //再次筛选矩形区域
  for (size_t i = 0; i < first_rects.size(); i++) 
  {
    RotatedRect roi_rect = first_rects[i];

    Rect_<float> safeBoundRect;
	//计算封闭轮廓点的最小外接旋转矩形是否超过原始图像，超过的话进行处理，处理后的矩形放在safeBoundRect中
    if (!calcSafeRect(roi_rect, src, safeBoundRect)) 
		continue;
	//保存输出
    outRects.push_back(safeBoundRect);
  }
  return 0;
}

//对在源图像截取出的矩形区域再次进行检测
int CPlateLocate::sobelSecSearchPart(Mat &bound, Point2f refpoint,
                                     vector<RotatedRect> &outRects) {
  Mat bound_threshold;
  //高斯模糊，sobel在x方向上的边缘检测，二值化，闭操作等，得到二值化图像输出到bound_threshold中
  sobelOperT(bound, bound_threshold, 3, 6, 2);

  Mat tempBoundThread = bound_threshold.clone();
  
  //二值化图像中去除柳钉
  clearLiuDingOnly(tempBoundThread);

  int posLeft = 0, posRight = 0;
  //寻找矩形区域的左边界和右边界位置，进行修复
  if (bFindLeftRightBound(tempBoundThread, posLeft, posRight)) {
    // 找到左边界和右边界进行修复
    if (posRight != 0 && posLeft != 0 && posLeft < posRight) {
      int posY = int(bound_threshold.rows * 0.5);
      for (int i = posLeft + (int) (bound_threshold.rows * 0.1);
           i < posRight - 4; i++) {
        bound_threshold.data[posY * bound_threshold.cols + i] = 255;
      }
    }

    utils::imwrite("resources/image/tmp/repaireimg1.jpg", bound_threshold);

    // remove the left and right boundaries
	//移除左右边界
    for (int i = 0; i < bound_threshold.rows; i++) {
      bound_threshold.data[i * bound_threshold.cols + posLeft] = 0;
      bound_threshold.data[i * bound_threshold.cols + posRight] = 0;
    }
    utils::imwrite("resources/image/tmp/repaireimg2.jpg", bound_threshold);
  }
  //对修复后的矩形二值化区域寻找封闭轮廓点
  vector<vector<Point>> contours;
  findContours(bound_threshold,
               contours,               // a vector of contours
               CV_RETR_EXTERNAL,
               CV_CHAIN_APPROX_NONE);  // all pixels of each contours

  vector<vector<Point>>::iterator itc = contours.begin();

  vector<RotatedRect> second_rects;
  while (itc != contours.end()) {
	  //最小外接矩形
    RotatedRect mr = minAreaRect(Mat(*itc));
    second_rects.push_back(mr);
    ++itc;
  }

  for (size_t i = 0; i < second_rects.size(); i++) {
    RotatedRect roi = second_rects[i];
    if (verifySizes(roi)) {
      Point2f refcenter = roi.center + refpoint;
      Size2f size = roi.size;
      float angle = roi.angle;

      RotatedRect refroi(refcenter, size, angle);
	  //按照旋转矩形的中心点，大小，旋转角度创建矩形，并输出
      outRects.push_back(refroi);
    }
  }

  return 0;
}

//sobel二次搜索
int CPlateLocate::sobelSecSearch(Mat &bound, Point2f refpoint,vector<RotatedRect> &outRects) 
{
  Mat bound_threshold;

  //对其进行sobel X方向的边缘检测，输出检测到的可能车牌区域
  sobelOper(bound, bound_threshold, 3, 10, 3);

  utils::imwrite("resources/image/tmp/sobelSecSearch.jpg", bound_threshold);

  vector<vector<Point>> contours;
  //提取轮廓函数
  //它的输入图像是一幅二值图像，输出的是每一个连通区域的轮廓点的集合
  findContours(bound_threshold,contours, // 容器来存储轮廓
               CV_RETR_EXTERNAL, // 提取外部轮廓
               CV_CHAIN_APPROX_NONE);  // 每一个轮廓的所有点


  //对所有轮廓进行存储
  vector<vector<Point>>::iterator itc = contours.begin();
  vector<RotatedRect> second_rects;
  while (itc != contours.end()) 
  {
	//得到每个轮廓的最小外接旋转矩形
    RotatedRect mr = minAreaRect(Mat(*itc));
    second_rects.push_back(mr);
    ++itc;
  }

  for (size_t i = 0; i < second_rects.size(); i++) 
  {
    RotatedRect roi = second_rects[i];

	//这个旋转矩形的纵横比是否满足要求（车牌的纵横比），满足则保存
	if (verifySizes(roi)) 
	{
      Point2f refcenter = roi.center + refpoint;
      Size2f size = roi.size;
      float angle = roi.angle;

      RotatedRect refroi(refcenter, size, angle);
      outRects.push_back(refroi);
    }
  }

  return 0;
}

//sobel边缘检测操作，输出检测到的车牌区域
//sobel操作，输入：图像，高斯模糊半径大小，形态学模板的宽度和高度
int CPlateLocate::sobelOper(const Mat &in, Mat &out, int blurSize, int morphW,int morphH) 
{
  Mat mat_blur;
  mat_blur = in.clone();
  //高斯模糊，blurSize为模糊半径大小
  GaussianBlur(in, mat_blur, Size(blurSize, blurSize), 0, 0, BORDER_DEFAULT);

  Mat mat_gray;
  //三通道图像，转换为单通道灰度图
  if (mat_blur.channels() == 3)
    cvtColor(mat_blur, mat_gray, CV_RGB2GRAY);
  else
    mat_gray = mat_blur;

  int scale = SOBEL_SCALE;
  int delta = SOBEL_DELTA;
  int ddepth = SOBEL_DDEPTH;

  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  //对单通道灰度图进行x方向的边缘检测，可以检测到竖线
  Sobel(mat_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
  //使用线性变换转换输入数组元素成8位无符号整型
  convertScaleAbs(grad_x, abs_grad_x);

  Mat grad;
  //将2幅图像线性叠加，输出到grad中
  addWeighted(abs_grad_x, SOBEL_X_WEIGHT, 0, 0, 0, grad);

  Mat mat_threshold;

  //对图像进行OTSU二值化处理
  //使用大律法OTSU得到的全局自适应阈值
  //CV_THRESH_OTSU代表自适应阈值，CV_THRESH_BINARY代表正二值化
  //正二值化意味着像素的值越接近0，越可能被赋值为0，反之则为1。
  //而另外一种二值化方法表示反二值化，其含义是像素的值越接近0，越可能被赋值1
  double otsu_thresh_val =threshold(grad, mat_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

  //得到矩形结构元素,作为模板对图像进行腐蚀或者膨胀操作
  Mat element = getStructuringElement(MORPH_RECT, Size(morphW, morphH));
  
  //使用element结构元素对图像进行闭操作（先膨胀，后腐蚀），弥补缝隙，连接图像区域
  morphologyEx(mat_threshold, mat_threshold, MORPH_CLOSE, element);
  //输出处理后的图像
  out = mat_threshold;

  return 0;
}

void deleteNotArea(Mat &inmat, Color color = UNKNOWN) 
{
  Mat input_grey;
  cvtColor(inmat, input_grey, CV_BGR2GRAY);

  int w = inmat.cols;
  int h = inmat.rows;

  Mat tmpMat = inmat(Rect_<double>(w * 0.15, h * 0.1, w * 0.7, h * 0.7));

  Color plateType;
  if (UNKNOWN == color) {
    plateType = getPlateType(tmpMat, true);
  }
  else {
    plateType = color;
  }

  Mat img_threshold;

  if (BLUE == plateType) 
  {
    img_threshold = input_grey.clone();
	//截取区域
    Mat tmp = input_grey(Rect_<double>(w * 0.15, h * 0.15, w * 0.7, h * 0.7));
	//得到二值化阈值点
	int threadHoldV = ThresholdOtsu(tmp);
	//使用上面得到的阈值对图像进行二值化
    threshold(input_grey, img_threshold, threadHoldV, 255, CV_THRESH_BINARY);
    // threshold(input_grey, img_threshold, 5, 255, CV_THRESH_OTSU +
    // CV_THRESH_BINARY);

    utils::imwrite("resources/image/tmp/inputgray2.jpg", img_threshold);

  } 
  else if (YELLOW == plateType) 
  {
    img_threshold = input_grey.clone();
    Mat tmp = input_grey(Rect_<double>(w * 0.1, h * 0.1, w * 0.8, h * 0.8));
    //得到图像的二值化阈值
	int threadHoldV = ThresholdOtsu(tmp);
	//使用上面得到的阈值对图像进行二值化
    threshold(input_grey, img_threshold, threadHoldV, 255,
              CV_THRESH_BINARY_INV);

    utils::imwrite("resources/image/tmp/inputgray2.jpg", img_threshold);

    // threshold(input_grey, img_threshold, 10, 255, CV_THRESH_OTSU +
    // CV_THRESH_BINARY_INV);
  } 
  else
    threshold(input_grey, img_threshold, 10, 255,
              CV_THRESH_OTSU + CV_THRESH_BINARY);

  //img_threshold = input_grey.clone();
  //spatial_ostu(img_threshold, 8, 2, plateType);

  int posLeft = 0;
  int posRight = 0;

  int top = 0;
  int bottom = img_threshold.rows - 1;
  //去掉图像中的柳钉
  clearLiuDing(img_threshold, top, bottom);

  if (0) 
  {
    imshow("inmat", inmat);
    waitKey(0);
    destroyWindow("inmat");
  }

  if (bFindLeftRightBound1(img_threshold, posLeft, posRight)) 
  {
    inmat = inmat(Rect(posLeft, top, w - posLeft, bottom - top));
    if (0) 
	{
      imshow("inmat", inmat);
      waitKey(0);
      destroyWindow("inmat");
    }
  }
}

//抗扭斜处理
//在src源图像中画矩形区域的线
int CPlateLocate::deskew(const Mat &src, const Mat &src_b,
                         vector<RotatedRect> &inRects,
                         vector<CPlate> &outPlates, bool useDeteleArea, Color color) {
  Mat mat_debug;
  src.copyTo(mat_debug);

  for (size_t i = 0; i < inRects.size(); i++) {
    RotatedRect roi_rect = inRects[i];

    float r = (float) roi_rect.size.width / (float) roi_rect.size.height;
    float roi_angle = roi_rect.angle;

    Size roi_rect_size = roi_rect.size;
    if (r < 1) {
      roi_angle = 90 + roi_angle;
      swap(roi_rect_size.width, roi_rect_size.height);
    }
	//在源图像上画出矩形区域的4条线
    if (m_debug) {
      Point2f rect_points[4];
      roi_rect.points(rect_points);
      for (int j = 0; j < 4; j++)
        line(mat_debug, rect_points[j], rect_points[(j + 1) % 4],
             Scalar(0, 255, 255), 1, 8);
    }

    // changed
    // rotation = 90 - abs(roi_angle);
    // rotation < m_angel;

    // m_angle=60,矩形区域在正负60度之间，则进行旋转。。。
    if (roi_angle - m_angle < 0 && roi_angle + m_angle > 0) 
	{
      Rect_<float> safeBoundRect;
	  //计算安全矩形,防止旋转后的图像超过原始图像大小
      bool isFormRect = calcSafeRect(roi_rect, src, safeBoundRect);
      if (!isFormRect) continue;

      Mat bound_mat = src(safeBoundRect);
      Mat bound_mat_b = src_b(safeBoundRect);

      if (0) {
        imshow("bound_mat_b", bound_mat_b);
        waitKey(0);
        destroyWindow("bound_mat_b");
      }

      //std::cout << "roi_angle:" << roi_angle << std::endl;

      Point2f roi_ref_center = roi_rect.center - safeBoundRect.tl();

      Mat deskew_mat;

	  //如果矩形区域旋转角度在正负5度之间，或者为90度，或者-90度，不再旋转，直接输出
      if ((roi_angle - 5 < 0 && roi_angle + 5 > 0) || 90.0 == roi_angle ||-90.0 == roi_angle) 
	  {
        deskew_mat = bound_mat;
      } 
	  else 
	  {
		  // 角度在5到60度之间的，首先需要旋转 rotation
        Mat rotated_mat;
        Mat rotated_mat_b;

        if (!rotation(bound_mat, rotated_mat, roi_rect_size, roi_ref_center,roi_angle))
          continue;

        if (!rotation(bound_mat_b, rotated_mat_b, roi_rect_size, roi_ref_center,roi_angle))
          continue;

        // we need affine for rotatioed image
        double roi_slope = 0;
        // imshow("1roated_mat",rotated_mat);
        // imshow("rotated_mat_b",rotated_mat_b);
		//是否偏斜，输入二值化图像，判断白色区域是否为平行四边形，为平行四边形，则旋转
        if (isdeflection(rotated_mat_b, roi_angle, roi_slope)) 
		{
			//仿射变换,将输入变形车牌图像旋转为正视角图像
          affine(rotated_mat, deskew_mat, roi_slope);
        } 
		else
          deskew_mat = rotated_mat;
      }


      Mat plate_mat;
      plate_mat.create(HEIGHT, WIDTH, TYPE);

      // haitungaga add，affect 25% to full recognition.
      if (useDeteleArea)
        deleteNotArea(deskew_mat, color);

	  //判断，抗扭斜处理后图像的宽高比在2.3--6之间
      if (deskew_mat.cols * 1.0 / deskew_mat.rows > 2.3 &&
          deskew_mat.cols * 1.0 / deskew_mat.rows < 6) {
		  
		  //对图像缩放到同一尺寸
        if (deskew_mat.cols >= WIDTH || deskew_mat.rows >= HEIGHT)
          resize(deskew_mat, plate_mat, plate_mat.size(), 0, 0, INTER_AREA);
        else
          resize(deskew_mat, plate_mat, plate_mat.size(), 0, 0, INTER_CUBIC);

        CPlate plate;
        plate.setPlatePos(roi_rect);
        plate.setPlateMat(plate_mat);
        if (color != UNKNOWN) plate.setPlateColor(color);

        outPlates.push_back(plate);
      }
    }
  }

  return 0;
}

//旋转操作
//将原始图像与目标图像都进行了扩大化。首先新建一个尺寸为原始图像1.5倍的新图像，
//接着把原始图像映射到新图像上，于是我们得到了一个显示区域(视框)扩大化后的原始图像。
//显示区域扩大以后，那些在原图像中没有值的像素被置了一个初值。
//接着调用warpAffine函数，使用新图像的大小作为目标图像的大小。warpAffine函数会将新图像旋转，
//并用目标图像尺寸的视框去显示它。于是我们得到了一个所有感兴趣区域都被完整显示的旋转后图像。
bool CPlateLocate::rotation(Mat &in, Mat &out, const Size rect_size,
                            const Point2f center, const double angle) {
  if (0) {
    imshow("in", in);
    waitKey(0);
    destroyWindow("in");
  }
  //新建一个尺寸为原始图像1.5倍的新图像
  Mat in_large;
  in_large.create(int(in.rows * 1.5), int(in.cols * 1.5), in.type());

  float x = in_large.cols / 2 - center.x > 0 ? in_large.cols / 2 - center.x : 0;
  float y = in_large.rows / 2 - center.y > 0 ? in_large.rows / 2 - center.y : 0;

  float width = x + in.cols < in_large.cols ? in.cols : in_large.cols - x;
  float height = y + in.rows < in_large.rows ? in.rows : in_large.rows - y;

  /*assert(width == in.cols);
  assert(height == in.rows);*/

  if (width != in.cols || height != in.rows) 
	  return false;

  //截取ROI区域，是放大之前的图像面积大小
  Mat imageRoi = in_large(Rect_<float>(x, y, width, height));
  //将原图像线性叠加到ROI区域上
  addWeighted(imageRoi, 0, in, 1, 0, imageRoi);

  Point2f center_diff(in.cols / 2.f, in.rows / 2.f);
  Point2f new_center(in_large.cols / 2.f, in_large.rows / 2.f);

  //得到以新图像中心点为轴心的旋转矩形
  Mat rot_mat = getRotationMatrix2D(new_center, angle, 1);

  /*imshow("in_copy", in_large);
  waitKey(0);*/

  Mat mat_rotated;
  //对图像进行旋转操作
  warpAffine(in_large, mat_rotated, rot_mat, Size(in_large.cols, in_large.rows),
             CV_INTER_CUBIC);

  /*imshow("mat_rotated", mat_rotated);
  waitKey(0);*/

  Mat img_crop;
  //从旋转好的图像中截取一个子像素精度的矩形，保存在img_crop中
  getRectSubPix(mat_rotated, Size(rect_size.width, rect_size.height),
                new_center, img_crop);
  out = img_crop;

  if (0) {
    imshow("out", out);
    waitKey(0);
    destroyWindow("out");
  }

  /*imshow("img_crop", img_crop);
  waitKey(0);*/

  return true;
}

//! 是否偏斜
//! 输入二值化图像，判断白色区域是否为平行四边形
bool CPlateLocate::isdeflection(const Mat &in, const double angle,
                                double &slope) { /*imshow("in",in);
                                                waitKey(0);*/
  if (0) {
    imshow("in", in);
    waitKey(0);
    destroyWindow("in");
  }
  
  int nRows = in.rows;
  int nCols = in.cols;

  assert(in.channels() == 1);

  int comp_index[3];
  int len[3];
  //选取图像中的3行像素
  comp_index[0] = nRows / 4;
  comp_index[1] = nRows / 4 * 2;
  comp_index[2] = nRows / 4 * 3;

  const uchar* p;
  //遍历图像的三行像素的每个像素点，记录下每行第一个不是黑点的列数
  for (int i = 0; i < 3; i++) 
  {
    int index = comp_index[i];
	//指向这一行的行首
    p = in.ptr<uchar>(index);

    int j = 0;
    int value = 0;
	//哪一列像素值开始不为0，则保存列值
    while (0 == value && j < nCols) 
		value = int(p[j++]);

    len[i] = j;
  }

  // cout << "len[0]:" << len[0] << endl;
  // cout << "len[1]:" << len[1] << endl;
  // cout << "len[2]:" << len[2] << endl;

  // len[0]/len[1]/len[2] are used to calc the slope

  double maxlen = max(len[2], len[0]);
  double minlen = min(len[2], len[0]);
  double difflen = abs(len[2] - len[0]);

  double PI = 3.14159265;

  double g = tan(angle * PI / 180.0);

  if (maxlen - len[1] > nCols / 32 || len[1] - minlen > nCols / 32) {
	// 如果斜率为正，则底部在下，反之在上
    double slope_can_1 = double(len[2] - len[0]) / double(comp_index[1]);
    double slope_can_2 = double(len[1] - len[0]) / double(comp_index[0]);
    double slope_can_3 = double(len[2] - len[1]) / double(comp_index[0]);
    // cout<<"angle:"<<angle<<endl;
    // cout<<"g:"<<g<<endl;
    // cout << "slope_can_1:" << slope_can_1 << endl;
    // cout << "slope_can_2:" << slope_can_2 << endl;
    // cout << "slope_can_3:" << slope_can_3 << endl;
    // if(g>=0)
	//为了实现一个鲁棒性更好的计算方法，可以用(len2-len1)/Height*4与(len3-len1)/Height*2两者之间
	//更靠近tan(angle)的值作为solpe的值（在这里，angle代表的是原来RotataedRect的角度）
    slope = abs(slope_can_1 - g) <= abs(slope_can_2 - g) ? slope_can_1: slope_can_2;
    // cout << "slope:" << slope << endl;
    return true;
  } else {
    slope = 0;
  }

  return false;
}

//仿射变换,将输入变形图像旋转为正视角图像
void CPlateLocate::affine(const Mat &in, Mat &out, const double slope) {
  // imshow("in", in);
  // waitKey(0);
	//opencv中的放射变换函数warpAffine需要源图像的三个点（左上点，右上点，左下点），
	//和输出图像的左上点，右上点，左下点，也就是需要三个点对的坐标来建立映射关系，
	//通过这个映射关系将原始图像的所有点映射到目标图像上。
  Point2f dstTri[3];//目标图像中的三个点
  Point2f plTri[3];//源图像中的三个点

  float height = (float) in.rows;
  float width = (float) in.cols;
  float xiff = (float) abs(slope) * height;//平行四边形旋转为矩形的偏移量
  //平行四边形向左
  if (slope > 0) {

    // right, new position is xiff/2
	  //源图像的三个点
    plTri[0] = Point2f(0, 0);
    plTri[1] = Point2f(width - xiff - 1, 0);
    plTri[2] = Point2f(0 + xiff, height - 1);
	//目标图像的三个点
    dstTri[0] = Point2f(xiff / 2, 0);
    dstTri[1] = Point2f(width - 1 - xiff / 2, 0);
    dstTri[2] = Point2f(xiff / 2, height - 1);
  } else {
	  //平行四边形向右
    // left, new position is -xiff/2
    plTri[0] = Point2f(0 + xiff, 0);
    plTri[1] = Point2f(width - 1, 0);
    plTri[2] = Point2f(0, height - 1);

    dstTri[0] = Point2f(xiff / 2, 0);
    dstTri[1] = Point2f(width - 1 - xiff + xiff / 2, 0);
    dstTri[2] = Point2f(xiff / 2, height - 1);
  }
  //映射矩阵
  Mat warp_mat = getAffineTransform(plTri, dstTri);

  Mat affine_mat;
  affine_mat.create((int) height, (int) width, TYPE);

  if (in.rows > HEIGHT || in.cols > WIDTH)
    warpAffine(in, affine_mat, warp_mat, affine_mat.size(),CV_INTER_AREA);
  else
    warpAffine(in, affine_mat, warp_mat, affine_mat.size(), CV_INTER_CUBIC);

  out = affine_mat;
}
//根据车牌颜色来检测车牌区域
int CPlateLocate::plateColorLocate(Mat src, vector<CPlate> &candPlates,
                                   int index) {
  vector<RotatedRect> rects_color_blue;//存储蓝色矩形区域
  rects_color_blue.reserve(64);
  vector<RotatedRect> rects_color_yellow;//存储黄色矩形区域
  rects_color_yellow.reserve(64);

  vector<CPlate> plates_blue;//存储蓝色车牌
  plates_blue.reserve(64);
  vector<CPlate> plates_yellow;//存储黄色车牌
  plates_yellow.reserve(64);

  Mat src_clone = src.clone();

  Mat src_b_blue;
  Mat src_b_yellow;
  
  //声明该并行区域分为若干个section, section之间的运行顺序为并行的关系
  //并行搜索蓝色矩形区域和黄色矩形区域
#pragma omp parallel sections
  {
#pragma omp section
    {
		//寻找蓝色车牌区域，将所有
      colorSearch(src, BLUE, src_b_blue, rects_color_blue);
      deskew(src, src_b_blue, rects_color_blue, plates_blue, true, BLUE);
    }
#pragma omp section
    {
      colorSearch(src_clone, YELLOW, src_b_yellow, rects_color_yellow);
      deskew(src_clone, src_b_yellow, rects_color_yellow, plates_yellow, true, YELLOW);
    }
  }

  candPlates.insert(candPlates.end(), plates_blue.begin(), plates_blue.end());
  candPlates.insert(candPlates.end(), plates_yellow.begin(), plates_yellow.end());

  return 0;
}


//! MSER plate locate，使用mser方式定位车牌,(MSER)最大稳定极值区域
int CPlateLocate::plateMserLocate(Mat src, vector<CPlate> &candPlates, int img_index) 
{
  std::vector<Mat> channelImages;//存放不同通道的图像
  std::vector<Color> flags;//存放颜色类型（存放蓝色，黄色）
  flags.push_back(BLUE);
  flags.push_back(YELLOW);

  bool usePlateMser = false;
  int scale_size = 1000;
  //int scale_size = CParams::instance()->getParam1i();
  double scale_ratio = 1;

  // only conside blue plate
  if (1) 
  {
    Mat grayImage;
    cvtColor(src, grayImage, COLOR_BGR2GRAY);
    channelImages.push_back(grayImage);

    //Mat singleChannelImage;
    //extractChannel(src, singleChannelImage, 2);
    //channelImages.push_back(singleChannelImage);
    //flags.push_back(BLUE);

    //channelImages.push_back(255 - grayImage);
    //flags.push_back(YELLOW);
  }

  for (size_t i = 0; i < channelImages.size(); ++i) 
  {
    vector<vector<RotatedRect>> plateRRectsVec;
    vector<vector<CPlate>> platesVec;
    vector<Mat> src_b_vec;

    Mat channelImage = channelImages.at(i);
	//如果图像的宽度或者高度大于指定值，则同比例缩小图像，将缩小比例输出
    Mat image = scaleImage(channelImage, Size(scale_size, scale_size), scale_ratio);

    // vector<RotatedRect> rects;
	//使用mser方法对图像进行处理
    mserSearch(image, src_b_vec, platesVec, usePlateMser, plateRRectsVec, img_index, false);

    for (size_t j = 0; j < flags.size(); j++) {
      vector<CPlate>& plates = platesVec.at(j);
      Mat& src_b = src_b_vec.at(j);
      Color color = flags.at(j);

      vector<RotatedRect> rects_mser;
      rects_mser.reserve(64);
      std::vector<CPlate> deskewPlate;
      deskewPlate.reserve(64);
      std::vector<CPlate> mserPlate;
      mserPlate.reserve(64);

      // deskew for rotation and slope image
      for (auto plate : plates) {
        RotatedRect rrect = plate.getPlatePos();
        RotatedRect scaleRect = scaleBackRRect(rrect, (float)scale_ratio);
        plate.setPlatePos(scaleRect);
        plate.setPlateColor(color);

        rects_mser.push_back(scaleRect);
        mserPlate.push_back(plate);
        //all_plates.push_back(plate);
      }

      Mat resize_src_b;
      resize(src_b, resize_src_b, Size(channelImage.cols, channelImage.rows));

      //src_b_vec.push_back(resize_src_b);

      deskew(src, resize_src_b, rects_mser, deskewPlate, false, color);

      for (auto dplate : deskewPlate) {
        RotatedRect drect = dplate.getPlatePos();
        Mat dmat = dplate.getPlateMat();

        for (auto splate : mserPlate) {
          RotatedRect srect = splate.getPlatePos();
          float iou = 0.f;
          bool isSimilar = computeIOU(drect, srect, src.cols, src.rows, 0.95f, iou);
          if (isSimilar) {
            splate.setPlateMat(dmat);
            candPlates.push_back(splate);
            break;
          }
        }
      }
    }

  }

  //if (usePlateMser) {
  //  std::vector<RotatedRect> plateRRect_B;
  //  std::vector<RotatedRect> plateRRect_Y;

  //  for (auto rrect : all_plateRRect) {
  //    RotatedRect theRect = scaleBackRRect(rrect, (float)scale_ratio);
  //    //rotatedRectangle(src, theRect, Scalar(255, 0, 0));
  //    for (auto plate : all_plates) {
  //      RotatedRect plateRect = plate.getPlatePos();
  //      //rotatedRectangle(src, plateRect, Scalar(0, 255, 0));
  //      bool isSimilar = computeIOU(theRect, plateRect, src, 0.8f);
  //      if (isSimilar) {
  //        //rotatedRectangle(src, theRect, Scalar(0, 0, 255));
  //        Color color = plate.getPlateColor();
  //        if (color == BLUE) plateRRect_B.push_back(theRect);
  //        if (color == YELLOW) plateRRect_Y.push_back(theRect);
  //      }
  //    }
  //  }

  //  for (size_t i = 0; i < channelImages.size(); ++i) {
  //    Color color = flags.at(i);
  //    Mat resize_src_b = src_b_vec.at(i);

  //    std::vector<CPlate> deskewMserPlate;
  //    if (color == BLUE)
  //      deskew(src, resize_src_b, plateRRect_B, deskewMserPlate, false, color);
  //    if (color == YELLOW)
  //      deskew(src, resize_src_b, plateRRect_Y, deskewMserPlate, false, color);

  //    for (auto plate : deskewMserPlate) {
  //      candPlates.push_back(plate);
  //    }
  //  }
  //}

  
  if (0) {
    imshow("src", src);
    waitKey(0);
    destroyWindow("src");
  }

  return 0;
}

//sobel操作，输入：图像，高斯模糊半径大小，形态学模板的宽度和高度（高斯模糊，sobel在x方向上的边缘检测，二值化，闭操作）
int CPlateLocate::sobelOperT(const Mat &in, Mat &out, int blurSize, int morphW,int morphH) 
{
  Mat mat_blur;
  mat_blur = in.clone();
  GaussianBlur(in, mat_blur, Size(blurSize, blurSize), 0, 0, BORDER_DEFAULT);

  Mat mat_gray;
  if (mat_blur.channels() == 3)
    cvtColor(mat_blur, mat_gray, CV_BGR2GRAY);
  else
    mat_gray = mat_blur;

  utils::imwrite("resources/image/tmp/grayblure.jpg", mat_gray);

  // equalizeHist(mat_gray, mat_gray);

  int scale = SOBEL_SCALE;
  int delta = SOBEL_DELTA;
  int ddepth = SOBEL_DDEPTH;

  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  Sobel(mat_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
  convertScaleAbs(grad_x, abs_grad_x);

  Mat grad;
  addWeighted(abs_grad_x, 1, 0, 0, 0, grad);

  utils::imwrite("resources/image/tmp/graygrad.jpg", grad);

  Mat mat_threshold;
  double otsu_thresh_val =
      threshold(grad, mat_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

  utils::imwrite("resources/image/tmp/grayBINARY.jpg", mat_threshold);

  Mat element = getStructuringElement(MORPH_RECT, Size(morphW, morphH));
  morphologyEx(mat_threshold, mat_threshold, MORPH_CLOSE, element);

  utils::imwrite("resources/image/tmp/phologyEx.jpg", mat_threshold);

  out = mat_threshold;

  return 0;
}
//使用sobel边缘检测方式定位车牌
int CPlateLocate::plateSobelLocate(Mat src, vector<CPlate> &candPlates,int index) 
{
  vector<RotatedRect> rects_sobel_all;
  //给verctor变量预分配256个元素的存储空间
  rects_sobel_all.reserve(256);

  vector<CPlate> plates;
  plates.reserve(32);

  vector<Rect_<float>> bound_rects;
  bound_rects.reserve(256);
  //sobel第一次处理（对输入图像进行高斯模糊，sobel的x方向边缘检测，二值化，闭操作，查找封闭轮廓，所有轮廓的最小外接矩形，剔除不合格矩形区域）
  //将处理后的图像输出到bound_rects中
  sobelFrtSearch(src, bound_rects);

  vector<Rect_<float>> bound_rects_part;
  bound_rects_part.reserve(256);

  // enlarge area 将每个旋转矩形的区域放大
  //这部分作用就是将每个旋转矩形的区域进行放大处理
  //矩形区域左边界x向左延伸，上边界y向上延伸，对高度和宽度进行一定程度的放大处理
  for (size_t i = 0; i < bound_rects.size(); i++) 
  {
    float fRatio = bound_rects[i].width * 1.0f / bound_rects[i].height;
	//矩形区域的宽高比在1.0---3.0，并且矩形高度<120
    if (fRatio < 3.0 && fRatio > 1.0 && bound_rects[i].height < 120) 
	{
      Rect_<float> itemRect = bound_rects[i];

	  //矩形区域的x坐标向左延伸，变小
      itemRect.x = itemRect.x - itemRect.height * (4 - fRatio);
	  //但是矩形区域的x坐标不能超过整幅图像的左边界，为负
      if (itemRect.x < 0) 
	  {
        itemRect.x = 0;
      }
      itemRect.width = itemRect.width + itemRect.height * 2 * (4 - fRatio);//矩形区域宽度变大
	  
	  //每个旋转矩形的区域都不能超过整幅图像大小
      if (itemRect.width + itemRect.x >= src.cols) 
	  {
        itemRect.width = src.cols - itemRect.x;
      }
	  //矩形区域的y坐标向上延伸，变小
      itemRect.y = itemRect.y - itemRect.height * 0.08f;
      itemRect.height = itemRect.height * 1.16f;//矩形区域高度变大
	  //保存放大后的矩形区域
      bound_rects_part.push_back(itemRect);
    }
  }
  //利用检测到的矩形区域，将这部分矩形区域从原始图像中取出，再次进行处理
  // second processing to split one
#pragma omp parallel for
  for (int i = 0; i < (int)bound_rects_part.size(); i++) 
  {
    Rect_<float> bound_rect = bound_rects_part[i];
    Point2f refpoint(bound_rect.x, bound_rect.y);

    float x = bound_rect.x > 0 ? bound_rect.x : 0;
    float y = bound_rect.y > 0 ? bound_rect.y : 0;

    float width =
        x + bound_rect.width < src.cols ? bound_rect.width : src.cols - x;
    float height =
        y + bound_rect.height < src.rows ? bound_rect.height : src.rows - y;

    Rect_<float> safe_bound_rect(x, y, width, height);//得到旋转矩形在整幅图像中的不超过边界的安全矩形
    Mat bound_mat = src(safe_bound_rect);//在源图像中截取这个矩形区域（彩色图像，未经任何处理）

	//对源图像中截取的彩色图像再次进行处理（高斯模糊，sobel在x方向边缘，二值化，闭操作等，去除柳钉，修复边界等）
	//将输出图像放在rects_sobel中
    vector<RotatedRect> rects_sobel;
    rects_sobel.reserve(128);
    sobelSecSearchPart(bound_mat, refpoint, rects_sobel);

#pragma omp critical
    {
		//将处理后的图像保存到rects_sobel_all中
      rects_sobel_all.insert(rects_sobel_all.end(), rects_sobel.begin(), rects_sobel.end());
    }
  }
  //并行计算
#pragma omp parallel for
  for (int i = 0; i < (int)bound_rects.size(); i++) 
  {
    Rect_<float> bound_rect = bound_rects[i];
    Point2f refpoint(bound_rect.x, bound_rect.y);

    float x = bound_rect.x > 0 ? bound_rect.x : 0;
    float y = bound_rect.y > 0 ? bound_rect.y : 0;

    float width =
        x + bound_rect.width < src.cols ? bound_rect.width : src.cols - x;
    float height =
        y + bound_rect.height < src.rows ? bound_rect.height : src.rows - y;

    Rect_<float> safe_bound_rect(x, y, width, height);
    Mat bound_mat = src(safe_bound_rect);

    vector<RotatedRect> rects_sobel;
    rects_sobel.reserve(128);
	//对图像进行二次sobel搜索，输出所有矩形可能为车牌轮廓的旋转矩形到rects_sobel
    sobelSecSearch(bound_mat, refpoint, rects_sobel);

#pragma omp critical
    {
      rects_sobel_all.insert(rects_sobel_all.end(), rects_sobel.begin(), rects_sobel.end());
    }
  }

  Mat src_b;
  sobelOper(src, src_b, 3, 10, 3);
  //抗扭斜处理，rects_sobel_all中存储的所有矩形区域经过处理和筛选，保存到候选车牌plates中
  deskew(src, src_b, rects_sobel_all, plates);

  //for (size_t i = 0; i < plates.size(); i++) 
  //  candPlates.push_back(plates[i]);
  //候选车牌
  candPlates.insert(candPlates.end(), plates.begin(), plates.end());

  return 0;
}

//使用三种方式对车牌进行定位，并都输出到resultVec中
int CPlateLocate::plateLocate(Mat src, vector<Mat> &resultVec, int index) {
  vector<CPlate> all_result_Plates;

  plateColorLocate(src, all_result_Plates, index);//颜色定位
  plateSobelLocate(src, all_result_Plates, index);//sobel边缘定位
  plateMserLocate(src, all_result_Plates, index);//Mser(最大稳定极值区域)
  //使用基于定位文字的方法来定位车牌，将定位到的文字组合起来就成为了车牌区域

  for (size_t i = 0; i < all_result_Plates.size(); i++) 
  {
    CPlate plate = all_result_Plates[i];
    resultVec.push_back(plate.getPlateMat());
  }

  return 0;
}

int CPlateLocate::plateLocate(Mat src, vector<CPlate> &resultVec, int index) {
  vector<CPlate> all_result_Plates;

  plateColorLocate(src, all_result_Plates, index);
  plateSobelLocate(src, all_result_Plates, index);
  plateMserLocate(src, all_result_Plates, index);

  for (size_t i = 0; i < all_result_Plates.size(); i++) {
    resultVec.push_back(all_result_Plates[i]);
  }

  return 0;
}

}
