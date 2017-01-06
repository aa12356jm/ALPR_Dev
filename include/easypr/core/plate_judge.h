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

  //输入多幅CPlate图像，输出多个是否为车牌的结果
  int plateJudge(const std::vector<CPlate> &, std::vector<CPlate> &);

  //使用nms判断输入的多个图像是否为车牌
  int plateJudgeUsingNMS(const std::vector<CPlate> &, std::vector<CPlate> &, int maxPlates = 5);

  //输入多幅mat图像，输出多个是否为车牌的结果
  int plateJudge(const std::vector<Mat> &, std::vector<Mat> &);

  //输入一个图像，输出一个是否为车牌的结果
  int plateJudge(const Mat &inMat, int &result);
  int plateSetScore(CPlate& plate);

 private:
  PlateJudge();

  static PlateJudge* instance_;//静态指针

  svmCallback extractFeature;

  cv::Ptr<ml::SVM> svm_;

};
}

#endif  // EASYPR_CORE_PLATEJUDGE_H_