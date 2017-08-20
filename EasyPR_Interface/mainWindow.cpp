#include "mainWindow.h"

#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QTimer>
#include <QPixmap>
#include <QMetaType>//ע��������ʹ��

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


	/*****************************��ʼ�������ʾ����*****************************/
	m_scene_easyPR = new QGraphicsScene(this);
	ui.graphicsView_easyPR->setScene(m_scene_easyPR);

	m_scene_openALPR= new QGraphicsScene(this);
	ui.graphicsView_openALPR->setScene(m_scene_openALPR);
	/*****************************�����������ʾ����*****************************/


	/*****************************��ʼ�������߳��������*****************************/
	m_thread_capture = new captureThread(this);
	qRegisterMetaType<vector<QString>>("vector<QString>");//Qt���̼߳��źŲ۴��ݷ�QObject���Ͷ���Ĳ���ʱ����Ҫע������ͣ�����connect��ʹ�� Qt::DirectConnection�������źŲ�
	connect(m_thread_capture, SIGNAL(captured(int,QImage)), this, SLOT(showImage(int,QImage)));
	connect(m_thread_capture, SIGNAL(capturedStr(int,vector<QString>)), this, SLOT(showPlateStr(int,vector<QString>)));
	m_thread_capture->start();
	m_thread_capture->stream();
	/*****************************�����������߳��������*****************************/


	/*****************************��ʼ����������ʾ*****************************/
	QStringList headers_result;
	headers_result.append("EasyPR");
	headers_result.append("���ƺ�");
	headers_result.append("OpenALPR");
	headers_result.append("���ƺ�");

	model_result = new QStandardItemModel();
	model_result->setHorizontalHeaderLabels(headers_result);//���ñ�ͷ	
	ui.tableView_results->setModel(model_result);//ʹ��setmodel����������ģ�ͺ�tabelview��
	ui.tableView_results->verticalHeader()->hide();//Ĭ����ʾ��ͷ�����Խ�������
	ui.tableView_results->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��ʱΪ����ѡ��	
	ui.tableView_results->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ñ��ĵ�ԪΪֻ�����ԣ����ܱ༭
	ui.tableView_results->setContextMenuPolicy(Qt::CustomContextMenu);//�������ʹ��QTableview��ʹ���Ҽ��˵�����Ҫ���ø�����
	/*****************************��������������ʾ*****************************/


	/*****************************��ʼ��״̬����ʾ��Ϣ*****************************/
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(showCameraStatus()));
	m_timer->start(1000);
	m_label_status = new QLabel(this);
	m_label_status->setAlignment(Qt::AlignHCenter);
	ui.statusBar->addWidget(m_label_status);
	/*****************************������״̬����ʾ��Ϣ*****************************/


	/*****************************��ʼ����Ӳ˵���ť�Ȳ���*****************************/
	QAction *openAction = new QAction(QIcon("icon/open.png"), tr("&%1").arg(QString::fromLocal8Bit("��")), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an image file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(pushButton_open_clicked()));

	QMenu *file = menuBar()->addMenu(tr("&%1").arg(QString::fromLocal8Bit("�ļ�")));
	file->addAction(openAction);
	//QToolBar *toolBar = addToolBar(tr("&File"));
	ui.mainToolBar->addAction(openAction);

	QAction *startCapture = new QAction(QIcon("icon/start.png"), tr("&%1").arg(QString::fromLocal8Bit("��ʼ�ɼ�")), this);
	startCapture->setStatusTip(tr("Open an camera"));
	connect(startCapture, SIGNAL(triggered()), this, SLOT(pushButton_startCapture_clicked()));

	file = menuBar()->addMenu(tr("&%1").arg(QString::fromLocal8Bit("��ʼ�ɼ�")));
	file->addAction(startCapture);
	//QToolBar *toolBar = addToolBar(tr("&File"));
	ui.mainToolBar->addAction(startCapture);
	/*****************************��������Ӳ˵���ť�Ȳ���*****************************/

}

void mainWindow::pushButton_plate_recognize_clicked()
{
// 	if (!fileName.isEmpty())
// 	{
// 		ui.textBrowser->clear();
// 		Mat src = imread(fileName.toStdString());
// 		easypr::CPlateRecognize pr;
// 		//pr.setResultShow(true);//�м�����ʾ
// 		pr.setLifemode(true);//����ģʽ
// 		pr.setDebug(false);//����ģʽ
// 		pr.setMaxPlates(4);//�ɼ�����������
// 						   //pr.setDetectType(PR_DETECT_COLOR | PR_DETECT_SOBEL);
// 		pr.setDetectType(easypr::PR_DETECT_CMSER | easypr::PR_DETECT_COLOR | easypr::PR_DETECT_SOBEL);//����Ϊ�ַ�����ɫ��sobel���ģʽ
// 
// 		vector<easypr::CPlate> plateVec;		
// 
// 		//��ͼƬ��ʶ�����Щ�������Ϊ��������
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
// 				//����ͼԪ������ӵ������У������ô�ͼԪ�ڳ����е�λ��Ϊ���ģ�0��0��
// 				m_graphicsScene_dst->addItem(pixItem_dst);
// 				pixItem_dst->setPos(0, 0);
// 			}
// 		}
// 		else
// 		{
// 			QMessageBox::critical(NULL, "����", "δʶ�𵽳���", QMessageBox::Yes);
// 		}
// 
// 	}
// 	else
// 	{
// 		QMessageBox::critical(NULL, "����", "���ȴ�ͼƬ", QMessageBox::Yes);
// 	}
}

void mainWindow::pushButton_open_clicked()
{
// 	m_graphicsScene_src->clear();
// 	fileName = QFileDialog::getOpenFileName(this,
// 		tr("��ͼ��"),//�Ի�������
// 		".",//Ĭ�ϴ��ļ�λ�á�.���ļ�Ŀ¼"/"��Ŀ¼
// 		tr("image files(*.jpg *.png *.bmp)"));//ɸѡ��
// 	
// 	Cv:Mat srcImg = imread(fileName.toStdString());
// 	
// 	pixItem_src = new PixItem(&QPixmap::fromImage(cvMat2QImage(srcImg)));
// 	//����ͼԪ������ӵ������У������ô�ͼԪ�ڳ����е�λ��Ϊ���ģ�0��0��
// 	m_graphicsScene_src->addItem(pixItem_src);
// 	pixItem_src->setPos(0, 0);


	//ui.label_src->setPixmap(QPixmap::fromImage(cvMat2QImage(srcImg)));

}


void mainWindow::pushButton_startCapture_clicked()
{
// 	VideoCapture capture(0);
// 	//������Ҫ�����߳���ʾ������Ῠ��
// 	while (true)
// 	{
// 		Mat frame;
// 		capture >> frame;
// 		//cv::imshow("��ȡ��Ƶ", frame);
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