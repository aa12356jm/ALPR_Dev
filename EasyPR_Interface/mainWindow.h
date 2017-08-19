#pragma once

#include <QtWidgets/QMainWindow>
#include <QStandardItemModel>
#include <QFileDialog>


#include "ui_mainWindow.h"

#include "captureThread.h"

#include "pixitem.h"
//#include "easypr/core/plate_locate.h"

//#include "accuracy.hpp"
//#include "chars.hpp"
//#include "plate.hpp"

#pragma execution_character_set("utf-8")

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow(QWidget *parent = Q_NULLPTR);
	

	void closeEvent(QCloseEvent *);

	void changeEvent(QEvent *e);

private slots:
	void pushButton_plate_recognize_clicked();

	void pushButton_open_clicked();
	void pushButton_startCapture_clicked();
	
	void showImage(QImage img);
	void showImage(cv::Mat img);
	void showPlateStr(vector<QString> plateStr);
	void showResultImage(QImage img);
	void showCameraStatus();

signals:
	void start_Singal();

private:
	Ui::mainWindowClass ui;
	QString fileName;
	//vector<Mat> vPlateMat;
	QTimer *m_timer;  //��ʱ��

	PixItem *pixItem_src;       //�Զ����ͼԪ��
	QGraphicsScene *m_scene_camera;  //����
	captureThread *m_thread_capture;  //����߳�
	QStandardItemModel *model_result; 

	QLabel *m_label_status;  //״̬����ʾ��Ϣ

};
