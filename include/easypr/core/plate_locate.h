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
  
  //sobel��һ��������������
  int sobelFrtSearch(const Mat& src, std::vector<Rect_<float>>& outRects);
  //sobel�ڶ���������������
  int sobelSecSearch(Mat& bound, Point2f refpoint,
                     std::vector<RotatedRect>& outRects);
  int sobelSecSearchPart(Mat& bound, Point2f refpoint,
                         std::vector<RotatedRect>& outRects);
  //��Ťб����
  int deskew(const Mat& src, const Mat& src_b,
             std::vector<RotatedRect>& inRects, std::vector<CPlate>& outPlates,
             bool useDeteleArea = true, Color color = UNKNOWN);

  //�Ƿ�ƫб�������ֵ��ͼ���жϰ�ɫ�����Ƿ�Ϊƽ���ı���
  bool isdeflection(const Mat& in, const double angle, double& slope);
  
  //sobel��Ե�������������⵽�ĳ�������
  int sobelOper(const Mat& in, Mat& out, int blurSize, int morphW, int morphH);

  //��ת����
  bool rotation(Mat& in, Mat& out, const Size rect_size, const Point2f center,
                const double angle);
  //����任,���������ͼ����תΪ���ӽ�ͼ��
  void affine(const Mat& in, Mat& out, const double slope);
  
  //���ݳ�����ɫ����⳵������
  int plateColorLocate(Mat src, std::vector<CPlate>& candPlates, int index = 0);
  //ʹ��sobel��Ե��ⷽʽ��λ����
  int plateSobelLocate(Mat src, std::vector<CPlate>& candPlates, int index = 0);
  
  int sobelOperT(const Mat& in, Mat& out, int blurSize, int morphW, int morphH);
  
  //ʹ��mser��ʽ��λ����
  int plateMserLocate(Mat src, std::vector<CPlate>& candPlates, int index = 0);


  int colorSearch(const Mat& src, const Color r, Mat& out,
                  std::vector<RotatedRect>& outRects);

  int mserSearch(const Mat &src, vector<Mat>& out,
    vector<vector<CPlate>>& out_plateVec, bool usePlateMser, vector<vector<RotatedRect>>& out_plateRRect,
    int img_index = 0, bool showDebug = false);

  //�������mat���г��ƶ�λ
  int plateLocate(Mat, std::vector<Mat>&, int = 0);
  //�������CPlate���г��ƶ�λ
  int plateLocate(Mat, std::vector<CPlate>&, int = 0);

  //��minAreaRect��õ���С��Ӿ��Σ����ݺ�Ⱥ�����Ƚ����ж�
  bool verifySizes(RotatedRect mr);
  
  //���ó��ƶ�λģʽ�Ƿ�Ϊ����ģʽ
  void setLifemode(bool param);
  
  //���ø�˹ģ���뾶��С
  inline void setGaussianBlurSize(int param) { m_GaussianBlurSize = param; }
  inline int getGaussianBlurSize() const { return m_GaussianBlurSize; }
  
  //������̬ѧ����ģ��Ŀ��
  inline void setMorphSizeWidth(int param) { m_MorphSizeWidth = param; }
  inline int getMorphSizeWidth() const { return m_MorphSizeWidth; }
  
  //������̬ѧ����ģ��ĸ߶�
  inline void setMorphSizeHeight(int param) { m_MorphSizeHeight = param; }
  inline int getMorphSizeHeight() const { return m_MorphSizeHeight; }

  //���ÿ�߱ȵ�ƫ���ʴ�С
  inline void setVerifyError(float param) { m_error = param; }
  inline float getVerifyError() const { return m_error; }
  
  //������ʵ���ƵĿ�߱�,Ĭ��ֵ3.75������ʹ����������޸�
  inline void setVerifyAspect(float param) { m_aspect = param; }
  inline float getVerifyAspect() const { return m_aspect; }
  
  //����ʵ���Ƶ������ֵ����СֵĬ��Ϊ1��ʹ�ô˺����޸�
  inline void setVerifyMin(int param) { m_verifyMin = param; }

  //����ʵ���Ƶ������ֵ�����ֵĬ��Ϊ24��ʹ�ô˺����޸�
  inline void setVerifyMax(int param) { m_verifyMax = param; }
  
  //���ó��Ƶ������ת�Ƕ�
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
  //��˹ģ���뾶��С
  int m_GaussianBlurSize;
  //��̬ѧ������ģ����
  int m_MorphSizeWidth;
  //��̬ѧ������ģ��߶�
  int m_MorphSizeHeight;

  //����һ��ƫ����m_error���������ƫ���ʼ���������С�Ŀ�߱�rmax��rmin���жϾ��ε�r�Ƿ�������rmax��rmin֮�䡣
  //��ΪĬ��ֵ0.9
  float m_error; 
  //��ʵ���ƵĿ�߱�,����Ĭ��ֵ3.75
  float m_aspect;
  int m_verifyMin;//����ʵ���Ƶ������ֵ����СֵΪ1
  int m_verifyMax;//����ʵ���Ƶ������ֵ�����ֵΪ24

  int m_angle;//���Ƶ������ת�Ƕ�60��


  bool m_debug;//�Ƿ����
};

} /*! \namespace easypr*/

#endif  // EASYPR_CORE_PLATELOCATE_H_