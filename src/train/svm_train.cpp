#include "easypr/train/svm_train.h"
#include "easypr/core/feature.h"
#include "easypr/util/util.h"

#ifdef OS_WINDOWS
#include <ctime>
#endif

using namespace cv::ml;

namespace easypr {

SvmTrain::SvmTrain(const char* plates_folder, const char* xml)
    : plates_folder_(plates_folder), svm_xml_(xml) {
  assert(plates_folder);
  assert(xml);
}

void SvmTrain::train() {
  svm_ = cv::ml::SVM::create();
  svm_->setType(cv::ml::SVM::C_SVC);
  svm_->setKernel(cv::ml::SVM::RBF);
  svm_->setDegree(0.1);
  // 1.4 bug fix: old 1.4 ver gamma is 1
  svm_->setGamma(0.1); 
  svm_->setCoef0(0.1);
  svm_->setC(1);
  svm_->setNu(0.1);
  svm_->setP(0.1);
  svm_->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 20000, 0.0001));//设置迭代次数和步进大小

  //得到训练数据
  auto train_data = tdata();

  fprintf(stdout, ">> Training SVM model, please wait...\n");
  long start = utils::getTimestamp();//开始时间
  //svm_->trainAuto(train_data, 10, SVM::getDefaultGrid(SVM::C),
  //                SVM::getDefaultGrid(SVM::GAMMA), SVM::getDefaultGrid(SVM::P),
  //                SVM::getDefaultGrid(SVM::NU), SVM::getDefaultGrid(SVM::COEF),
  //                SVM::getDefaultGrid(SVM::DEGREE), true);
  svm_->train(train_data);//开始训练

  long end = utils::getTimestamp();//结束时间
  fprintf(stdout, ">> Training done. Time elapse: %ldms\n", end - start);
  fprintf(stdout, ">> Saving model file...\n");
  svm_->save(svm_xml_);//将生成的xml文件保存到本地

  fprintf(stdout, ">> Your SVM Model was saved to %s\n", svm_xml_);
  fprintf(stdout, ">> Testing...\n");
  this->test();//开始测试

}

void SvmTrain::test() {
  // 1.4 bug fix: old 1.4 ver there is no null judge
  //if (NULL == svm_)
    svm_ = cv::ml::SVM::load(/*svm_xml_*/"D:/WorkSpace/EasyPR/EasyPR-master/tmp/svm.xml");//加载训练好的模型文件

  if (test_file_list_.empty()) {
    this->prepare();
  }

  double count_all = test_file_list_.size();
  double ptrue_rtrue = 0;
  double ptrue_rfalse = 0;
  double pfalse_rtrue = 0;
  double pfalse_rfalse = 0;

  for (auto item : test_file_list_) {
    auto image = cv::imread(item.file);
    if (!image.data) {
      
      std::cout << "no" << std::endl;
      continue;
    }
    cv::Mat feature;
    getLBPFeatures(image, feature);
	//使用模型进行判断
    auto predict = int(svm_->predict(feature));
    std::cout << "predict: " << predict << std::endl;

    auto real = item.label;
    if (predict == kForward && real == kForward) ptrue_rtrue++;
    if (predict == kForward && real == kInverse) ptrue_rfalse++;
    if (predict == kInverse && real == kForward) pfalse_rtrue++;
    if (predict == kInverse && real == kInverse) pfalse_rfalse++;
  }

  std::cout << "count_all: " << count_all << std::endl;
  std::cout << "ptrue_rtrue: " << ptrue_rtrue << std::endl;
  std::cout << "ptrue_rfalse: " << ptrue_rfalse << std::endl;
  std::cout << "pfalse_rtrue: " << pfalse_rtrue << std::endl;
  std::cout << "pfalse_rfalse: " << pfalse_rfalse << std::endl;

  //评价指标：查准率
  double precise = 0;
  if (ptrue_rtrue + ptrue_rfalse != 0) {
    precise = ptrue_rtrue / (ptrue_rtrue + ptrue_rfalse);
    std::cout << "precise: " << precise << std::endl;
  } else {
    std::cout << "precise: "
              << "NA" << std::endl;
  }

  //评价指标：查全率
  double recall = 0;
  if (ptrue_rtrue + pfalse_rtrue != 0) {
    recall = ptrue_rtrue / (ptrue_rtrue + pfalse_rtrue);
    std::cout << "recall: " << recall << std::endl;
  } else {
    std::cout << "recall: "
              << "NA" << std::endl;
  }
  //综合评价指标，当查准率和查全率都高时，才会高
  double Fsocre = 0;
  if (precise + recall != 0) {
    Fsocre = 2 * (precise * recall) / (precise + recall);
    std::cout << "Fsocre: " << Fsocre << std::endl;
  } else {
    std::cout << "Fsocre: "
              << "NA" << std::endl;
  }
}
//准备训练数据，读取数据
void SvmTrain::prepare() {
  srand(unsigned(time(NULL)));

  char buffer[260] = {0};

  //buffer指向文件夹
  sprintf(buffer, "%s/has/train", plates_folder_);
  //读取文件夹中的文件名保存到列表中
  auto has_file_train_list = utils::getFiles(buffer);
  //随机排序列表中的文件名
  std::random_shuffle(has_file_train_list.begin(), has_file_train_list.end());

  sprintf(buffer, "%s/has/test", plates_folder_);
  auto has_file_test_list = utils::getFiles(buffer);
  std::random_shuffle(has_file_test_list.begin(), has_file_test_list.end());

  sprintf(buffer, "%s/no/train", plates_folder_);
  auto no_file_train_list = utils::getFiles(buffer);
  std::random_shuffle(no_file_train_list.begin(), no_file_train_list.end());

  sprintf(buffer, "%s/no/test", plates_folder_);
  auto no_file_test_list = utils::getFiles(buffer);
  std::random_shuffle(no_file_test_list.begin(), no_file_test_list.end());

  fprintf(stdout, ">> Collecting train data...\n");

  ////收集训练数据
  //将有车牌的每个数据标记为1，保存到列表中
  for (auto file : has_file_train_list)
    train_file_list_.push_back({ file, kForward });
  
  //将没有车牌的每个数据标记为0，保存到列表中
  for (auto file : no_file_train_list)
    train_file_list_.push_back({ file, kInverse });

  fprintf(stdout, ">> Collecting test data...\n");

  ////收集测试数据
  for (auto file : has_file_test_list)
    test_file_list_.push_back({ file, kForward });

  for (auto file : no_file_test_list)
    test_file_list_.push_back({ file, kInverse });
}

cv::Ptr<cv::ml::TrainData> SvmTrain::tdata() {
  this->prepare();//将所有车牌数据打上标签，保存到列表中

  cv::Mat samples;
  std::vector<int> responses;

  for (auto f : train_file_list_) {
    auto image = cv::imread(f.file);
    if (!image.data) {
      fprintf(stdout, ">> Invalid image: %s  ignore.\n", f.file.c_str());
      continue;
    }
    cv::Mat feature;
    getLBPFeatures(image, feature);//提取图像的LBP特征
    feature = feature.reshape(1, 1);//将矩阵拉伸为1维向量

	//保存每个图像的特征数据和对应的标签
    samples.push_back(feature);
    responses.push_back(int(f.label));
  }

  cv::Mat samples_, responses_;
  samples.convertTo(samples_, CV_32FC1);//转换为32位浮点数单通道
  cv::Mat(responses).copyTo(responses_);

  return cv::ml::TrainData::create(samples_, cv::ml::SampleTypes::ROW_SAMPLE,
                                   responses_);
}

}  // namespace easypr
