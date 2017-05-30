#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"

#include "easypr.h"
#include "easypr/util/switch.hpp"
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
	QImage cvMat2QImage(const cv::Mat& mat);

	int test_plate_locate();
	int test_plate_judge();
	int test_plate_detect();
	
	int test_chars_segment();
	int test_chars_identify();
	int test_chars_recognise();
	int test_plate_recognize();

private slots:
	void pushButton_plate_locate_clicked();
	void pushButton_plate_judge_clicked();
	void pushButton_plate_detect_clicked();
	void pushButton_chars_segment_clicked();
	void pushButton_chars_identify_clicked();
	void pushButton_chars_recognise_clicked();
	void pushButton_plate_recognize_clicked();
	void pushButton_test_all_clicked();

	void pushButton_open_clicked();
	
private:
	Ui::mainWindowClass ui;
	QString fileName;

	PixItem *pixItem_src;       //自定义的图元类
	QGraphicsScene *m_graphicsScene_src;  //场景
	QGraphicsView *m_graphicsView_src;

	PixItem *pixItem_dst;       //自定义的图元类
	QGraphicsScene *m_graphicsScene_dst;  //场景
	QGraphicsView *m_graphicsView_dst;
};
