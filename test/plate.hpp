#ifndef EASYPR_PLATE_HPP
#define EASYPR_PLATE_HPP

namespace easypr {

namespace demo {

using namespace cv;
using namespace std;

//���ƶ�λ������һ��ͼ�񣬶�λ���п����ǳ��Ƶ�����
int test_plate_locate() 
{
  cout << "test_plate_locate" << endl;

  const string file = "../resources/image/plate_locate.jpg";
  //��ͼ������ڴ�
  cv::Mat src = imread(file);

  vector<cv::Mat> resultVec;
  CPlateLocate plate;
  //plate.setDebug(0);
  //plate.setLifemode(true);
  //��ͼ���п����ǳ��ƵĶ�����򶼼����������浽vector����resultVec��
  int result = plate.plateLocate(src, resultVec);
  
  //����⵽�Ŀ����ǳ��Ƶľ���������ʾ����
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

//������ʹ�ó��ƶ�λ��������ͼ���п���Ϊ���ƵĶ�����򣬽���Щ�������뵽svm���ж��Ƿ�Ϊ����
//������ʾ���ƶ�λ�Ľ��ͼ��Ȼ����ʾ��svm�ж�Ϊ���Ƶ�ͼ��
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

  //�����ɳ��ƶ�λ�õ��Ķ������������svm�����ж��Ƿ�Ϊ���ƣ���ȷ��Ϊ���Ƶ����������resultVec��
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
  pr.setLifemode(true);//����ģʽ
  pr.setDebug(false);//����ģʽ
  pr.setMaxPlates(4);//�ɼ�����������
  //pr.setDetectType(PR_DETECT_COLOR | PR_DETECT_SOBEL);
  pr.setDetectType(easypr::PR_DETECT_CMSER);//����Ϊ�ַ����ģʽ

  //vector<string> plateVec;
  vector<CPlate> plateVec;

  //��ͼƬ��ʶ�����Щ�������Ϊ��������
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
