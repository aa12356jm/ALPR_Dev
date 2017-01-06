//////////////////////////////////////////////////////////////////////////
// Name:	    plate_locate Header
// Version:		1.2
// Date:	    2014-09-19
// MDate:		2014-09-29
// MDate:	    2015-03-13
// Author:	    liuruoze
// Copyright:   liuruoze
// Reference:	Mastering OpenCV with Practical Computer Vision Projects
// Reference:	CSDN Bloger taotao1233
// Desciption:
// Defines CPlateLocate
//////////////////////////////////////////////////////////////////////////
#ifndef EASYPR_CORE_PLATELOCATE_H_
#define EASYPR_CORE_PLATELOCATE_H_

#include "easypr/core/plate.hpp"

/*! \namespace easypr
    Namespace where all the C++ EasyPR functionality resides
*/

using namespace std;

namespace easypr {

class CPlateLocate {
 public:
  CPlateLocate();
  
  //sobel第一次搜索矩形轮廓
  int sobelFrtSearch(const Mat& src, std::vector<Rect_<float>>& outRects);
  //sobel第二次搜索矩形轮廓
  int sobelSecSearch(Mat& bound, Point2f refpoint,
                     std::vector<RotatedRect>& outRects);
  int sobelSecSearchPart(Mat& bound, Point2f refpoint,
                         std::vector<RotatedRect>& outRects);
  //抗扭斜处理
  int deskew(const Mat& src, const Mat& src_b,
             std::vector<RotatedRect>& inRects, std::vector<CPlate>& outPlates,
             bool useDeteleArea = true, Color color = UNKNOWN);

  //是否偏斜，输入二值化图像，判断白色区域是否为平行四边形
  bool isdeflection(const Mat& in, const double angle, double& slope);
  
  //sobel边缘检测操作，输出检测到的车牌区域
  int sobelOper(const Mat& in, Mat& out, int blurSize, int morphW, int morphH);

  //旋转操作
  bool rotation(Mat& in, Mat& out, const Size rect_size, const Point2f center,
                const double angle);
  //仿射变换,将输入变形图像旋转为正视角图像
  void affine(const Mat& in, Mat& out, const double slope);
  
  //根据车牌颜色来检测车牌区域
  int plateColorLocate(Mat src, std::vector<CPlate>& candPlates, int index = 0);
  //使用sobel边缘检测方式定位车牌
  int plateSobelLocate(Mat src, std::vector<CPlate>& candPlates, int index = 0);
  
  int sobelOperT(const Mat& in, Mat& out, int blurSize, int morphW, int morphH);
  
  //使用mser方式定位车牌
  int plateMserLocate(Mat src, std::vector<CPlate>& candPlates, int index = 0);


  int colorSearch(const Mat& src, const Color r, Mat& out,
                  std::vector<RotatedRect>& outRects);

  int mserSearch(const Mat &src, vector<Mat>& out,
    vector<vector<CPlate>>& out_plateVec, bool usePlateMser, vector<vector<RotatedRect>>& out_plateRRect,
    int img_index = 0, bool showDebug = false);

  //对输入的mat进行车牌定位
  int plateLocate(Mat, std::vector<Mat>&, int = 0);
  //对输入的CPlate进行车牌定位
  int plateLocate(Mat, std::vector<CPlate>&, int = 0);

  //对minAreaRect获得的最小外接矩形，用纵横比和面积比进行判断
  bool verifySizes(RotatedRect mr);
  
  //设置车牌定位模式是否为生活模式
  void setLifemode(bool param);
  
  //设置高斯模糊半径大小
  inline void setGaussianBlurSize(int param) { m_GaussianBlurSize = param; }
  inline int getGaussianBlurSize() const { return m_GaussianBlurSize; }
  
  //设置形态学操作模板的宽度
  inline void setMorphSizeWidth(int param) { m_MorphSizeWidth = param; }
  inline int getMorphSizeWidth() const { return m_MorphSizeWidth; }
  
  //设置形态学操作模板的高度
  inline void setMorphSizeHeight(int param) { m_MorphSizeHeight = param; }
  inline int getMorphSizeHeight() const { return m_MorphSizeHeight; }

  //设置宽高比的偏差率大小
  inline void setVerifyError(float param) { m_error = param; }
  inline float getVerifyError() const { return m_error; }
  
  //设置真实车牌的宽高比,默认值3.75，可以使用这个函数修改
  inline void setVerifyAspect(float param) { m_aspect = param; }
  inline float getVerifyAspect() const { return m_aspect; }
  
  //和真实车牌的面积比值，最小值默认为1，使用此函数修改
  inline void setVerifyMin(int param) { m_verifyMin = param; }

  //和真实车牌的面积比值，最大值默认为24，使用此函数修改
  inline void setVerifyMax(int param) { m_verifyMax = param; }
  
  //设置车牌的最大旋转角度
  inline void setJudgeAngle(int param) { m_angle = param; }

  inline void setDebug(bool param) { m_debug = param; }


  inline bool getDebug() { return m_debug; }


  static const int DEFAULT_GAUSSIANBLUR_SIZE = 5;
  static const int SOBEL_SCALE = 1;
  static const int SOBEL_DELTA = 0;
  static const int SOBEL_DDEPTH = CV_16S;
  static const int SOBEL_X_WEIGHT = 1;
  static const int SOBEL_Y_WEIGHT = 0;
  static const int DEFAULT_MORPH_SIZE_WIDTH = 17;  // 17
  static const int DEFAULT_MORPH_SIZE_HEIGHT = 3;  // 3


  static const int WIDTH = 136;
  static const int HEIGHT = 36;
  static const int TYPE = CV_8UC3;


  static const int DEFAULT_VERIFY_MIN = 1;   // 3
  static const int DEFAULT_VERIFY_MAX = 24;  // 20

  static const int DEFAULT_ANGLE = 60;  // 30

  static const int DEFAULT_DEBUG = 1;

 protected:
  //高斯模糊半径大小
  int m_GaussianBlurSize;
  //形态学操作的模板宽度
  int m_MorphSizeWidth;
  //形态学操作的模板高度
  int m_MorphSizeHeight;

  //设立一个偏差率m_error，根据这个偏差率计算最大和最小的宽高比rmax、rmin。判断矩形的r是否满足在rmax、rmin之间。
  //设为默认值0.9
  float m_error; 
  //真实车牌的宽高比,设置默认值3.75
  float m_aspect;
  int m_verifyMin;//和真实车牌的面积比值，最小值为1
  int m_verifyMax;//和真实车牌的面积比值，最大值为24

  int m_angle;//车牌的最大旋转角度60度


  bool m_debug;//是否调试
};

} /*! \namespace easypr*/

#endif  // EASYPR_CORE_PLATELOCATE_H_