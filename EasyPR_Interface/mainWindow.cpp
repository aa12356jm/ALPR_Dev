#include "mainWindow.h"

#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QTimer>
#include <QPixmap>
#include <QMetaType>//注册新类型使用

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


	/*****************************开始：相机显示界面*****************************/
	m_scene_easyPR = new QGraphicsScene(this);
	ui.graphicsView_easyPR->setScene(m_scene_easyPR);

	m_scene_openALPR= new QGraphicsScene(this);
	ui.graphicsView_openALPR->setScene(m_scene_openALPR);
	/*****************************结束：相机显示界面*****************************/


	/*****************************开始：开启线程启动相机*****************************/
	m_thread_capture = new captureThread(this);
	qRegisterMetaType<vector<QString>>("vector<QString>");//Qt多线程间信号槽传递非QObject类型对象的参数时，需要注册此类型，或者connect中使用 Qt::DirectConnection来传递信号槽
	connect(m_thread_capture, SIGNAL(captured(int,QImage)), this, SLOT(showImage(int,QImage)));
	connect(m_thread_capture, SIGNAL(capturedStr(int,vector<QString>)), this, SLOT(showPlateStr(int,vector<QString>)));
	m_thread_capture->start();
	m_thread_capture->stream();
	/*****************************结束：开启线程启动相机*****************************/


	/*****************************开始：处理结果显示*****************************/
	QStringList headers_result;
	headers_result.append("EasyPR");
	headers_result.append("车牌号");
	headers_result.append("OpenALPR");
	headers_result.append("车牌号");

	model_result = new QStandardItemModel();
	model_result->setHorizontalHeaderLabels(headers_result);//设置表头	
	ui.tableView_results->setModel(model_result);//使用setmodel方法将数据模型和tabelview绑定
	ui.tableView_results->verticalHeader()->hide();//默认显示行头，可以将其隐藏
	ui.tableView_results->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中时为整行选中	
	ui.tableView_results->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格的单元为只读属性，不能编辑
	ui.tableView_results->setContextMenuPolicy(Qt::CustomContextMenu);//如果你在使用QTableview中使用右键菜单，需要启用该属性
	/*****************************结束：处理结果显示*****************************/


	/*****************************开始：状态栏显示信息*****************************/
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(showCameraStatus()));
	m_timer->start(1000);
	m_label_status = new QLabel(this);
	m_label_status->setAlignment(Qt::AlignHCenter);
	ui.statusBar->addWidget(m_label_status);
	/*****************************结束：状态栏显示信息*****************************/


	/*****************************开始：添加菜单按钮等操作*****************************/
	QAction *openAction = new QAction(QIcon("icon/open.png"), tr("&%1").arg(QString::fromLocal8Bit("打开")), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an image file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(pushButton_open_clicked()));

	QMenu *file = menuBar()->addMenu(tr("&%1").arg(QString::fromLocal8Bit("文件")));
	file->addAction(openAction);
	//QToolBar *toolBar = addToolBar(tr("&File"));
	ui.mainToolBar->addAction(openAction);

	QAction *startCapture = new QAction(QIcon("icon/start.png"), tr("&%1").arg(QString::fromLocal8Bit("开始采集")), this);
	startCapture->setStatusTip(tr("Open an camera"));
	connect(startCapture, SIGNAL(triggered()), this, SLOT(pushButton_startCapture_clicked()));

	file = menuBar()->addMenu(tr("&%1").arg(QString::fromLocal8Bit("开始采集")));
	file->addAction(startCapture);
	//QToolBar *toolBar = addToolBar(tr("&File"));
	ui.mainToolBar->addAction(startCapture);
	/*****************************结束：添加菜单按钮等操作*****************************/

}

void mainWindow::pushButton_plate_recognize_clicked()
{
// 	if (!fileName.isEmpty())
// 	{
// 		ui.textBrowser->clear();
// 		Mat src = imread(fileName.toStdString());
// 		easypr::CPlateRecognize pr;
// 		//pr.setResultShow(true);//中间结果显示
// 		pr.setLifemode(true);//生活模式
// 		pr.setDebug(false);//调试模式
// 		pr.setMaxPlates(4);//可检测最大车牌数量
// 						   //pr.setDetectType(PR_DETECT_COLOR | PR_DETECT_SOBEL);
// 		pr.setDetectType(easypr::PR_DETECT_CMSER | easypr::PR_DETECT_COLOR | easypr::PR_DETECT_SOBEL);//设置为字符，颜色，sobel检测模式
// 
// 		vector<easypr::CPlate> plateVec;		
// 
// 		//从图片中识别出哪些区域可能为车牌区域？
// 		int result = pr.plateRecognize(src, plateVec);
// 		//int result = pr.plateRecognizeAsText(src, plateVec);
// 		if (result == 0) {
// 			size_t num = plateVec.size();
// 			for (size_t j = 0; j < num; j++) {
// 				ui.textBrowser->append(QString::fromStdString(plateVec[j].getPlateStr()));
// 				vPlateMat.push_back(plateVec[j].getPlateMat());
// 			}
// 			if (0!=vPlateMat.size())
// 			{
// 				pixItem_dst = new PixItem(&QPixmap::fromImage(cvMat2QImage(vPlateMat[0])));
// 				//将该图元对象添加到场景中，并设置此图元在场景中的位置为中心（0，0）
// 				m_graphicsScene_dst->addItem(pixItem_dst);
// 				pixItem_dst->setPos(0, 0);
// 			}
// 		}
// 		else
// 		{
// 			QMessageBox::critical(NULL, "警告", "未识别到车牌", QMessageBox::Yes);
// 		}
// 
// 	}
// 	else
// 	{
// 		QMessageBox::critical(NULL, "警告", "请先打开图片", QMessageBox::Yes);
// 	}
}

void mainWindow::pushButton_open_clicked()
{
// 	m_graphicsScene_src->clear();
// 	fileName = QFileDialog::getOpenFileName(this,
// 		tr("打开图像"),//对话框名称
// 		".",//默认打开文件位置“.”文件目录"/"根目录
// 		tr("image files(*.jpg *.png *.bmp)"));//筛选器
// 	
// 	Cv:Mat srcImg = imread(fileName.toStdString());
// 	
// 	pixItem_src = new PixItem(&QPixmap::fromImage(cvMat2QImage(srcImg)));
// 	//将该图元对象添加到场景中，并设置此图元在场景中的位置为中心（0，0）
// 	m_graphicsScene_src->addItem(pixItem_src);
// 	pixItem_src->setPos(0, 0);


	//ui.label_src->setPixmap(QPixmap::fromImage(cvMat2QImage(srcImg)));

}


void mainWindow::pushButton_startCapture_clicked()
{
// 	VideoCapture capture(0);
// 	//这里需要开启线程显示，否则会卡死
// 	while (true)
// 	{
// 		Mat frame;
// 		capture >> frame;
// 		//cv::imshow("读取视频", frame);
// 		//ui.label->setPixmap(QPixmap::fromImage(cvMat2QImage(frame)));
// 		waitKey(30);
// 	}
}

void mainWindow::showImage(int id,QImage img)
{
	if (m_thread_capture->isQuit())
	{
		return;
	}
	if (id==0)
	{
		auto size = ui.graphicsView_easyPR->geometry().size();
		m_scene_easyPR->addPixmap(QPixmap::fromImage(img).scaled(size));
		m_scene_easyPR->setSceneRect(0, 0, size.width(), size.height());
	}
	
	if (id == 1)
	{
		auto size = ui.graphicsView_openALPR->geometry().size();
		m_scene_openALPR->addPixmap(QPixmap::fromImage(img).scaled(size));
		m_scene_openALPR->setSceneRect(0, 0, size.width(), size.height());
	}
}

void mainWindow::showImage(cv::Mat img)
{
	if (m_thread_capture->isQuit())
	{
		return;
	}
	auto size = ui.graphicsView_easyPR->geometry().size();

	m_scene_easyPR->addPixmap(QPixmap::fromImage(m_thread_capture->cvMat2QImage(img)).scaled(size));
	m_scene_easyPR->setSceneRect(0, 0, size.width(), size.height());
}

void mainWindow::showPlateStr(int id,vector<QString> plateStr)
{
	if (0==plateStr.size())
	{
		return;
	}
	if (0==id)
	{
		ui.textBrowser_easyPR->clear();
		for (int i = 0; i < plateStr.size(); i++)
		{
			ui.textBrowser_easyPR->append(plateStr[i]);
		}
	}
	if (1 == id)
	{
		ui.textBrowser_openALPR->clear();
		for (int i = 0; i < plateStr.size(); i++)
		{
			ui.textBrowser_openALPR->append(plateStr[i]);
		}
	}

}

void mainWindow::showResultImage(QImage img)
{
// 	if (m_thread_capture->isQuit())
// 	{
// 		return;
// 	}
// 	if (m_image_result)
// 	{
// 		m_scene_camera
// 	}
// 	
}

void mainWindow::showCameraStatus()
{
	if (m_thread_capture->isQuit())
	{
		return;
	}

	m_label_status->setText(
		QString("Camera1 capture:%1,fps:%2; Camera2 Capture:%3,fps:%4").\
		arg(QString::number(1, 'f', 2)).\
		arg(QString::number(2, 'f', 2)).\
		arg(QString::number(3, 'f', 2)).\
		arg(QString::number(4, 'f', 2))
	);

}

void mainWindow::closeEvent(QCloseEvent *e)
{
	m_thread_capture->stop();
	m_timer->stop();

	while (!m_thread_capture->isQuit()||m_timer->isActive())
	{
		QCoreApplication::processEvents();
	}
	QMainWindow::closeEvent(e);
}

void mainWindow::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type())
	{
	case QEvent::LanguageChange:
		ui.retranslateUi(this);
		break;
	default:
		break;
	}
}