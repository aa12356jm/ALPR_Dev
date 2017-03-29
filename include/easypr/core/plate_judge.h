#ifndef EASYPR_CORE_PLATEJUDGE_H_
#define EASYPR_CORE_PLATEJUDGE_H_

#include "easypr/core/plate.hpp"
#include "easypr/core/feature.h"

namespace easypr {

class PlateJudge {
 public:
  static PlateJudge* instance();

  void LoadModel(std::string path);

  //$)AJdHk6`7yCPlateM<Oq#,Jd3v6`8vJG7qN*35EF5D=a9{
  int plateJudge(const std::vector<CPlate> &, std::vector<CPlate> &);

  //$)AJ9SCnmsEP6OJdHk5D6`8vM<OqJG7qN*35EF
  int plateJudgeUsingNMS(const std::vector<CPlate> &, std::vector<CPlate> &, int maxPlates = 5);

  //$)AJdHk6`7ymatM<Oq#,Jd3v6`8vJG7qN*35EF5D=a9{
  int plateJudge(const std::vector<Mat> &, std::vector<Mat> &);

  //$)AJdHkR;8vM<Oq#,Jd3vR;8vJG7qN*35EF5D=a9{
  int plateJudge(const Mat &inMat, int &result);
  int plateSetScore(CPlate& plate);

 private:
  PlateJudge();

  static PlateJudge* instance_;//$)A>2L,V8Uk

  svmCallback extractFeature;

  cv::Ptr<ml::SVM> svm_;

};
}

#endif  // EASYPR_CORE_PLATEJUDGE_H_