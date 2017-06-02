#include "easypr/core/plate_judge.h"
#include "easypr/config.h"
#include "easypr/core/core_func.h"
#include "easypr/core/params.h"

namespace easypr {

  PlateJudge* PlateJudge::instance_ = nullptr;

  PlateJudge* PlateJudge::instance() {
    if (!instance_) {
      instance_ = new PlateJudge;
    }
    return instance_;
  }

  PlateJudge::PlateJudge() { 
    svm_ = ml::SVM::load(kDefaultSvmPath); 
    //svm_ = ml::SVM::load<ml::SVM>(kLBPSvmPath);   
    extractFeature = getLBPFeatures;
  }
  void PlateJudge::LoadModel(std::string path) {
    if (path != std::string(kDefaultSvmPath)) {

      if (!svm_->empty())
        svm_->clear();
      svm_ = ml::SVM::load(path);
    }
  }//输入一副图像，判断是否为车牌
  int PlateJudge::plateJudge(const Mat &inMat, int &result) {
    Mat features;
	//提取特征
    extractFeature(inMat, features);
	//svm判断是否为车牌
    float response = svm_->predict(features);
    /*std::cout << "response:" << response << std::endl;
    float score = svm_->predict(features, noArray(), cv::ml::StatModel::Flags::RAW_OUTPUT);
    std::cout << "score:" << score << std::endl;*/
    result = (int)response;
    return 0;
  }
  //输入多个矩形区域，根据svm判断是否为车牌，输出确定为车牌的区域
  int PlateJudge::plateJudge(const std::vector<Mat> &inVec,
    std::vector<Mat> &resultVec) {
    int num = inVec.size();
    for (int j = 0; j < num; j++) {
      Mat inMat = inVec[j];
      int response = -1;
      plateJudge(inMat, response);
      if (response == 1) resultVec.push_back(inMat);
    }
    return 0;
  }

  // set the score of plate
  // 0 is plate, -1 is not.
  //返回0则是车牌，-1则不是
  int PlateJudge::plateSetScore(CPlate& plate) {
    Mat features;
	//提取车牌特征
    extractFeature(plate.getPlateMat(), features);
	//svm判断是否为车牌
    float score = svm_->predict(features, noArray(), cv::ml::StatModel::Flags::RAW_OUTPUT);
    //std::cout << "score:" << score << std::endl;
    // score is the distance of margin，below zero is plate, up is not
    // when score is below zero, the samll the value, the more possibliy to be a plate.
	//此车牌的分数为：。。。
	plate.setPlateScore(score);
    if (score < 0)
      return 0;
    else
      return -1;
  }
  // non-maximum suppression
  void NMS(std::vector<CPlate> &inVec, std::vector<CPlate> &resultVec, double overlap) 
  {
	  //对车牌排序
    std::sort(inVec.begin(), inVec.end());
	//遍历车牌图像
    std::vector<CPlate>::iterator it = inVec.begin();
    for (; it != inVec.end(); ++it) 
	{
      CPlate plateSrc = *it;
      //std::cout << "plateScore:" << plateSrc.getPlateScore() << std::endl;
	  //得到车牌的旋转矩形的外界矩形
      Rect rectSrc = plateSrc.getPlatePos().boundingRect();
	  //下一个车牌
      std::vector<CPlate>::iterator itc = it + 1;
      for (; itc != inVec.end();) 
	  {
        CPlate plateComp = *itc;
        Rect rectComp = plateComp.getPlatePos().boundingRect();
        //Rect rectInter = rectSrc & rectComp;
        //Rect rectUnion = rectSrc | rectComp;
        //double r = double(rectInter.area()) / double(rectUnion.area());
		//依次将每个车牌的旋转外界矩形和下一个车牌的旋转外界矩形比较计算
		float iou = computeIOU(rectSrc, rectComp);

		//如果内部矩形和外部矩形的比值大于overlap
        if (iou > overlap) 
		{
          itc = inVec.erase(itc);
        }
        else 
		{
          ++itc;
        }
      }
    }
    resultVec = inVec;
  }

  //使用nms判断输入的多个图像是否为车牌，输入为inVec，输出到resultVec中
  int PlateJudge::plateJudgeUsingNMS(const std::vector<CPlate> &inVec, std::vector<CPlate> &resultVec, int maxPlates) {
    std::vector<CPlate> plateVec;
    int num = inVec.size();
    bool outputResult = false;
    bool useCascadeJudge = true;
    bool useShirkMat = true;
    for (int j = 0; j < num; j++) 
	{
      CPlate plate = inVec[j];
	  //车牌对象转换为mat
      Mat inMat = plate.getPlateMat();
	  //对车牌打分，返回结果为0则表示是车牌
      int result = plateSetScore(plate);
      if (result == 0) 
	  {
        if (0) 
		{
          imshow("inMat", inMat);
          waitKey(0);
          destroyWindow("inMat");
        }
		//如果车牌使用的定位类型是CMSER
        if (plate.getPlateLocateType() == CMSER) 
		{
          int w = inMat.cols;
          int h = inMat.rows;
		  //截取一部分区域
          Mat tmpmat = inMat(Rect_<double>(w * 0.05, h * 0.1, w * 0.9, h * 0.8));
          //复制一份图像
		  Mat tmpDes = inMat.clone();
		  //将截取的图像放大为原来的大小
          resize(tmpmat, tmpDes, Size(inMat.size()));
		  //设置调整之后图像为车牌矩形
          plate.setPlateMat(tmpDes);
		  
		  //如果使用这种方式判断。。。。
          if (useCascadeJudge) 
		  {
            int resultCascade = plateSetScore(plate);
            if (plate.getPlateLocateType() != CMSER)
              plate.setPlateMat(inMat);
            if (resultCascade == 0) 
			{
              if (0) 
			  {
                imshow("tmpDes", tmpDes);
                waitKey(0);
                destroyWindow("tmpDes");
              }
              plateVec.push_back(plate);
            }
          }
          else 
		  {//没有使用useCascadeJudge，则保存输出结果
            plateVec.push_back(plate);
          }
        }
        else 
		{//如果车牌使用的定位类型不是CMSER，则直接保存输出
          plateVec.push_back(plate);
        }                   
      }
    }

    std::vector<CPlate> reDupPlateVec;

    double overlap = 0.7;
    //double overlap = CParams::instance()->getParam1f();
    NMS(plateVec, reDupPlateVec, overlap);
  
    std::vector<CPlate>::iterator it = reDupPlateVec.begin();
    int count = 0;
    for (; it != reDupPlateVec.end(); ++it) {
      resultVec.push_back(*it);
      if (0) {
        imshow("plateMat", it->getPlateMat());
        waitKey(0);
        destroyWindow("plateMat");
      }
      count++;
      if (count >= maxPlates)
        break;
    }
    return 0;
  }
  //输入一列车牌图像，svm判断是否为车牌，将所有的判断为车牌的图像输出
  //如果第一次判断不是车牌，则考虑到可能车牌截取区域过大，将其缩小，继续判断
  int PlateJudge::plateJudge(const std::vector<CPlate> &inVec,
    std::vector<CPlate> &resultVec) {
    int num = inVec.size();
    for (int j = 0; j < num; j++) {
      CPlate inPlate = inVec[j];
	  //将车牌转换为mat
      Mat inMat = inPlate.getPlateMat();
      int response = -1;
	  //根据输入的图像svm判断是否为车牌
      plateJudge(inMat, response);
	  //为1表示为车牌，保存并输出
      if (response == 1)
        resultVec.push_back(inPlate);
      else {
        int w = inMat.cols;
        int h = inMat.rows;
		//如果判断不是车牌，则截取图像一部分，取出来
        Mat tmpmat = inMat(Rect_<double>(w * 0.05, h * 0.1, w * 0.9, h * 0.8));
        Mat tmpDes = inMat.clone();
		//将截取的图像放大到和原来的一样大小
        resize(tmpmat, tmpDes, Size(inMat.size()));
		//将截取后的图像再次判断是否为车牌
        plateJudge(tmpDes, response);
        if (response == 1) resultVec.push_back(inPlate);
      }
    }
    return 0;
  }
}
