#ifndef EASYPR_CORE_PLATEJUDGE_H_
#define EASYPR_CORE_PLATEJUDGE_H_

#include  <opencv2/ml.hpp>
#include "easypr/core/plate.hpp"
#include "easypr/core/feature.h"

namespace easypr {

class PlateJudge {
 public:
  static PlateJudge* instance();

  void LoadModel(std::string path);

  //������CPlateͼ���������Ƿ�Ϊ���ƵĽ��
  int plateJudge(const std::vector<CPlate> &, std::vector<CPlate> &);

  //ʹ��nms�ж�����Ķ��ͼ���Ƿ�Ϊ����
  int plateJudgeUsingNMS(const std::vector<CPlate> &, std::vector<CPlate> &, int maxPlates = 5);

  //������matͼ���������Ƿ�Ϊ���ƵĽ��
  int plateJudge(const std::vector<Mat> &, std::vector<Mat> &);

  //����һ��ͼ�����һ���Ƿ�Ϊ���ƵĽ��
  int plateJudge(const Mat &inMat, int &result);
  int plateSetScore(CPlate& plate);

 private:
  PlateJudge();

  static PlateJudge* instance_;//��ָ̬��

  svmCallback extractFeature;

  cv::Ptr<ml::SVM> svm_;

};
}

#endif  // EASYPR_CORE_PLATEJUDGE_H_