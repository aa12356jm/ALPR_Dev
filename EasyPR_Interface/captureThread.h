#pragma once

#include <QThread>
#include <QImage>
#include <atomic>
#include "opencv2/opencv.hpp"
#include "easypr.h"
#include "easypr/util/switch.hpp"
#include "alpr.h"

class captureThread : public QThread
{
	Q_OBJECT

public:
	captureThread(QObject *parent);
	~captureThread();

public:
	static QImage cvMat2QImage(const cv::Mat& mat);
	void run();
	void stream();
	void stop();
	void pause();
	//void mmv_process(cv::Mat &f1, cv::Mat &f2);
	bool isQuit() {
		return quit_status==1;
	}
	bool plateRecognize_easyPR(cv::Mat &img, vector<QString> &plateStr_easyPR);
	static bool plateRecognize_openALPR(cv::Mat &img,vector<QString> &plateStr_openALPR);

signals:
	void captured(int id, QImage img);
	void capturedStr(int id,vector<QString> plateStr);
	
	void captured(cv::Mat img);
private:
	bool pause_status;
	QVector<QRgb> grayColorTable;
	QVector<QRgb> ColorTable;
	cv::Mat error_image;
	std::atomic_int quit_status;  //0--running,1--quiting,2--quited
	cv::VideoCapture capture;//打开摄像头或者视频
	vector<QString> plateStr_easyPR;
	vector<QString> plateStr_openALPR;
};
