#ifndef EASYPR_PLATE_HPP
#define EASYPR_PLATE_HPP

namespace easypr {

namespace demo {

using namespace cv;
using namespace std;

//车牌定位，输入一幅图像，定位其中可能是车牌的区域
int test_plate_locate() 
{
  cout << "test_plate_locate" << endl;

  const string file = "../resources/image/plate_locate.jpg";
  //将图像读入内存
  cv::Mat src = imread(file);

  vector<cv::Mat> resultVec;
  CPlateLocate plate;
  //plate.setDebug(0);
  //plate.setLifemode(true);
  //将图像中可能是车牌的多个区域都检测出来，保存到vector变量resultVec中
  int result = plate.plateLocate(src, resultVec);
  
  //将检测到的可能是车牌的矩形区域都显示出来
  if (result == 0) 
  {
    size_t num = resultVec.size();
    for (size_t j = 0; j < num; j++) 
	{
      cv::Mat resultMat = resultVec[j];
      imshow("plate_locate", resultMat);
      waitKey(0);
    }
    destroyWindow("plate_locate");
  }

  return result;
}

//首先是使用车牌定位函数检测出图像中可能为车牌的多个区域，将这些区域输入到svm中判断是否为车牌
//首先显示车牌定位的结果图像，然后显示经svm判断为车牌的图像
int test_plate_judge() {
  cout << "test_plate_judge" << endl;

  cv::Mat src = imread("resources/image/plate_judge.jpg");

  vector<cv::Mat> matVec;

  vector<cv::Mat> resultVec;

  CPlateLocate lo;
  lo.setDebug(1);
  lo.setLifemode(true);

  int resultLo = lo.plateLocate(src, matVec);

  if (0 != resultLo) return -1;

  cout << "plate_locate_img" << endl;
  size_t num = matVec.size();
  for (size_t j = 0; j < num; j++) {
    Mat resultMat = matVec[j];
    imshow("plate_judge", resultMat);
    waitKey(0);
  }
  destroyWindow("plate_judge");

  //输入由车牌定位得到的多个矩形区域，由svm进行判断是否为车牌，将确定为车牌的区域输出到resultVec中
  int resultJu = PlateJudge::instance()->plateJudge(matVec, resultVec);

  if (0 != resultJu) 
	  return -1;

  cout << "plate_judge_img" << endl;
  num = resultVec.size();
  for (size_t j = 0; j < num; j++) {
    Mat resultMat = resultVec[j];
    imshow("plate_judge", resultMat);
    waitKey(0);
  }
  destroyWindow("plate_judge");

  return resultJu;
}

//
int test_plate_detect() {
  cout << "test_plate_detect" << endl;

  cv::Mat src = imread("resources/image/plate_detect.jpg");

  vector<CPlate> resultVec;
  CPlateDetect pd;
  pd.setPDLifemode(true);

  int result = pd.plateDetect(src, resultVec);
  if (result == 0) 
  {
    size_t num = resultVec.size();
    for (size_t j = 0; j < num; j++)
	{
      CPlate resultMat = resultVec[j];
      imshow("plate_detect", resultMat.getPlateMat());
      waitKey(0);
    }
    destroyWindow("plate_detect");
  }

  return result;
}

int test_plate_recognize() {
  cout << "test_plate_recognize" << endl;
  Mat src = imread("resources/image/test.jpg");
  CPlateRecognize pr;
  pr.setLifemode(true);//生活模式
  pr.setDebug(false);//调试模式
  pr.setMaxPlates(4);//可检测最大车牌数量
  //pr.setDetectType(PR_DETECT_COLOR | PR_DETECT_SOBEL);
  pr.setDetectType(easypr::PR_DETECT_CMSER);//设置为字符检测模式

  //vector<string> plateVec;
  vector<CPlate> plateVec;

  //从图片中识别出哪些区域可能为车牌区域？
  int result = pr.plateRecognize(src, plateVec);
  //int result = pr.plateRecognizeAsText(src, plateVec);
  if (result == 0) {
    size_t num = plateVec.size();
    for (size_t j = 0; j < num; j++) {
      cout << "plateRecognize: " << plateVec[j].getPlateStr() << endl;
    }
  }

  if (result != 0) cout << "result:" << result << endl;

  return result;
}
}
}

#endif  // EASYPR_PLATE_HPP
