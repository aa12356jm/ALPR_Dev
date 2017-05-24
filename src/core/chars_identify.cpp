#include "easypr/core/chars_identify.h"
#include "easypr/core/character.hpp"
#include "easypr/config.h"
#include "easypr/core/core_func.h"
#include "easypr/core/feature.h"
#include "easypr/core/params.h"

namespace easypr {

  CharsIdentify* CharsIdentify::instance_ = nullptr;

  CharsIdentify* CharsIdentify::instance() {
    if (!instance_) {
      instance_ = new CharsIdentify;
    }
    return instance_;
  }

  CharsIdentify::CharsIdentify() {
    ann_ = ml::ANN_MLP::load<ml::ANN_MLP>(kDefaultAnnPath);
    annChinese_ = ml::ANN_MLP::load<ml::ANN_MLP>(kChineseAnnPath);
    kv_ = std::shared_ptr<Kv>(new Kv);
    kv_->load("../../etc/province_mapping");
  }

  void CharsIdentify::LoadModel(std::string path) {
    if (path != std::string(kDefaultAnnPath)) {

      if (!ann_->empty())
        ann_->clear();

      ann_ = ml::ANN_MLP::load<ml::ANN_MLP>(path);
    }
  }

  void CharsIdentify::LoadChineseModel(std::string path) {
    if (path != std::string(kChineseAnnPath)) {

      if (!annChinese_->empty())
        annChinese_->clear();

      annChinese_ = ml::ANN_MLP::load<ml::ANN_MLP>(path);
    }
  }


  void CharsIdentify::classify(cv::Mat featureRows, std::vector<int>& out_maxIndexs, 
    std::vector<float>& out_maxVals, std::vector<bool> isChineseVec){
    int rowNum = featureRows.rows;

    cv::Mat output(rowNum, kCharsTotalNumber, CV_32FC1);
    ann_->predict(featureRows, output);

    for (int output_index = 0; output_index < rowNum; output_index++) {
      Mat output_row = output.row(output_index);
      int result = -1;
      float maxVal = -2.f;
      bool isChinses = isChineseVec[output_index];
      if (!isChinses) {
        result = 0;
        for (int j = 0; j < kCharactersNumber; j++) {
          float val = output_row.at<float>(j);
          // std::cout << "j:" << j << "val:" << val << std::endl;
          if (val > maxVal) {
            maxVal = val;
            result = j;
          }
        }
      } 
      else {
        result = kCharactersNumber;
        for (int j = kCharactersNumber; j < kCharsTotalNumber; j++) {
          float val = output_row.at<float>(j);
          //std::cout << "j:" << j << "val:" << val << std::endl;
          if (val > maxVal) {
            maxVal = val;
            result = j;
          }
        }
      }
      out_maxIndexs[output_index] = result;
      out_maxVals[output_index] = maxVal;
    }
  }

  //对输入的文字进行分类，给每个字符预测其具体字符和对应的概率
  void CharsIdentify::classify(std::vector<CCharacter>& charVec){
    size_t charVecSize = charVec.size();

    if (charVecSize == 0)
      return;

    Mat featureRows;
    for (size_t index = 0; index < charVecSize; index++) {
      Mat charInput = charVec[index].getCharacterMat();//字符的矩形区域
      Mat feature = charFeatures(charInput, kPredictSize);//得到图像中文字区域的特征行向量
      featureRows.push_back(feature);//featureRows矩阵中每一行存储一个字符区域的特征向量
    }

    //cv::Mat output(charVecSize, kCharsTotalNumber, CV_32FC1);
	cv::Mat output(featureRows.size(), CV_32FC1);
	//将
    ann_->predict(featureRows, output);

	//将所有预测的结果打标签
    for (size_t output_index = 0; output_index < charVecSize; output_index++) {
      CCharacter& character = charVec[output_index];
      Mat output_row = output.row(output_index);

      int result = -1;
      float maxVal = -2.f;
      std::string label = "";
	  //依次判断每个字符是否为中文
      bool isChinses = character.getIsChinese();
	  //不是中文，则将对应的字符打标签
      if (!isChinses) {
        result = 0;
        for (int j = 0; j < kCharactersNumber; j++) {
          float val = output_row.at<float>(j);
          //std::cout << "j:" << j << "val:" << val << std::endl;
          if (val > maxVal) {
            maxVal = val;
            result = j;
          }
        }
        label = std::make_pair(kChars[result], kChars[result]).second;
      }
      else {//是中文的话，则将这个字与对应的省份打标签
        result = kCharactersNumber;
        for (int j = kCharactersNumber; j < kCharsTotalNumber; j++) {
          float val = output_row.at<float>(j);
          //std::cout << "j:" << j << "val:" << val << std::endl;
          if (val > maxVal) {
            maxVal = val;
            result = j;
          }
        }
        const char* key = kChars[result];
        std::string s = key;
        std::string province = kv_->get(s);
        label = std::make_pair(s, province).second;
      }
      /*std::cout << "result:" << result << std::endl;
      std::cout << "maxVal:" << maxVal << std::endl;*/
      character.setCharacterScore(maxVal);//字符的可能性
      character.setCharacterStr(label);//预测得到的字符
    }
  }


  void CharsIdentify::classifyChinese(std::vector<CCharacter>& charVec){
    size_t charVecSize = charVec.size();

    if (charVecSize == 0)
      return;

    Mat featureRows;
    for (size_t index = 0; index < charVecSize; index++) {
      Mat charInput = charVec[index].getCharacterMat();
      Mat feature = charFeatures(charInput, kChineseSize);
      featureRows.push_back(feature);
    }

    cv::Mat output(charVecSize, kChineseNumber, CV_32FC1);
    annChinese_->predict(featureRows, output);

    for (size_t output_index = 0; output_index < charVecSize; output_index++) {
      CCharacter& character = charVec[output_index];
      Mat output_row = output.row(output_index);
      bool isChinese = true;

      float maxVal = -2;
      int result = -1;

      for (int j = 0; j < kChineseNumber; j++) {
        float val = output_row.at<float>(j);
        //std::cout << "j:" << j << "val:" << val << std::endl;
        if (val > maxVal) {
          maxVal = val;
          result = j;
        }
      }

      // no match
      if (-1 == result) {
        result = 0;
        maxVal = 0;
        isChinese = false;
      }

      auto index = result + kCharsTotalNumber - kChineseNumber;
      const char* key = kChars[index];
      std::string s = key;
      std::string province = kv_->get(s);

      /*std::cout << "result:" << result << std::endl;
      std::cout << "maxVal:" << maxVal << std::endl;*/

      character.setCharacterScore(maxVal);
      character.setCharacterStr(province);
      character.setIsChinese(isChinese);
    }
  }
  //输入一个字符矩形图像，并告诉是否是中文，如果发现有字符大于maxVal，则返回其位置索引
  int CharsIdentify::classify(cv::Mat f, float& maxVal, bool isChinses){
    int result = -1;

    cv::Mat output(1, kCharsTotalNumber, CV_32FC1);
    //ann判别字符，将所有的字符输出到output，只有一行，其每一列是一个字符
	ann_->predict(f, output);

    maxVal = -2.f;
	//如果是中文的话，
    if (!isChinses) {
      result = 0;
      for (int j = 0; j < kCharactersNumber; j++) {
        float val = output.at<float>(j);
        // std::cout << "j:" << j << "val:" << val << std::endl;
        if (val > maxVal) {
          maxVal = val;
          result = j;
        }
      }
    }
    else {
      result = kCharactersNumber;
      for (int j = kCharactersNumber; j < kCharsTotalNumber; j++) {
        float val = output.at<float>(j);
        //std::cout << "j:" << j << "val:" << val << std::endl;
        if (val > maxVal) {
          maxVal = val;
          result = j;
        }
      }
    }
    //std::cout << "maxVal:" << maxVal << std::endl;
    return result;
  }

  bool CharsIdentify::isCharacter(cv::Mat input, std::string& label, float& maxVal, bool isChinese) {
    cv::Mat feature = charFeatures(input, kPredictSize);
    auto index = static_cast<int>(classify(feature, maxVal, isChinese));

    if (isChinese) {
      //std::cout << "maxVal:" << maxVal << std::endl;
    }


    float chineseMaxThresh = 0.2f;
    //float chineseMaxThresh = CParams::instance()->getParam2f();

    if (maxVal >= 0.9 || (isChinese && maxVal >= chineseMaxThresh)) {
      if (index < kCharactersNumber) {
        label = std::make_pair(kChars[index], kChars[index]).second;
      }
      else {
        const char* key = kChars[index];
        std::string s = key;
        std::string province = kv_->get(s);
        label = std::make_pair(s, province).second;
      }
      return true;
    }
    else
      return false;
  }

  /*bool CharsIdentify::charsJudge(std::vector<CCharacter>& charVec) {
    cv::Mat feature = charFeatures(input, kPredictSize);
    auto index = static_cast<int>(classify(feature, maxVal, isChinese));

    if (isChinese)
      std::cout << "maxVal:" << maxVal << std::endl;

    if (maxVal >= 0.9) {
      if (index < kCharactersNumber) {
        label = std::make_pair(kChars[index], kChars[index]).second;
      }
      else {
        const char* key = kChars[index];
        std::string s = key;
        std::string province = kv_->get(s);
        label = std::make_pair(s, province).second;
      }
      return true;
    }
    else
      return false;
  }*/

  std::pair<std::string, std::string> CharsIdentify::identifyChinese(cv::Mat input, float& out, bool& isChinese) {
    cv::Mat feature = charFeatures(input, kChineseSize);
    float maxVal = -2;

    int result = -1;

    cv::Mat output(1, kChineseNumber, CV_32FC1);
    annChinese_->predict(feature, output);

    for (int j = 0; j < kChineseNumber; j++) {
      float val = output.at<float>(j);
      //std::cout << "j:" << j << "val:" << val << std::endl;
      if (val > maxVal) {
        maxVal = val;
        result = j;
      }
    }

    // no match
    if (-1 == result) {
      result = 0;
      maxVal = 0;
      isChinese = false;
    }
    else if (maxVal > 0.9){
      isChinese = true;
    }

    auto index = result + kCharsTotalNumber - kChineseNumber;
    const char* key = kChars[index];
    std::string s = key;
    std::string province = kv_->get(s);

    out = maxVal;

    return std::make_pair(s, province);
  }


  std::pair<std::string, std::string> CharsIdentify::identify(cv::Mat input, bool isChinese) {
    cv::Mat feature = charFeatures(input, kPredictSize);
    float maxVal = -2;
    auto index = static_cast<int>(classify(feature, maxVal, isChinese));
    if (index < kCharactersNumber) {
      return std::make_pair(kChars[index], kChars[index]);
    }
    else {
      const char* key = kChars[index];
      std::string s = key;
      std::string province = kv_->get(s);
      return std::make_pair(s, province);
    }
  }

  int CharsIdentify::identify(std::vector<cv::Mat> inputs, std::vector<std::pair<std::string, std::string>>& outputs,
    std::vector<bool> isChineseVec) {
    Mat featureRows;
    size_t input_size = inputs.size();
    for (size_t i = 0; i < input_size; i++) {
      Mat input = inputs[i];
      cv::Mat feature = charFeatures(input, kPredictSize);
      featureRows.push_back(feature);
    }

    std::vector<int> maxIndexs;
    std::vector<float> maxVals;
    classify(featureRows, maxIndexs, maxVals, isChineseVec);

    for (size_t row_index = 0; row_index < input_size; row_index++) {
      int index = maxIndexs[row_index];
      if (index < kCharactersNumber) {
        outputs[row_index] = std::make_pair(kChars[index], kChars[index]);
      }
      else {
        const char* key = kChars[index];
        std::string s = key;
        std::string province = kv_->get(s);
        outputs[row_index] = std::make_pair(s, province);
      }
    }
    return 0;
  }
}
