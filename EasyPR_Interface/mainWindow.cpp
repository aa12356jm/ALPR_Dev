#include "mainWindow.h"

#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QAction *openAction = new QAction(QIcon("icon/open.png"), tr("&打开"), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an image file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(pushButton_open_clicked()));

	QMenu *file = menuBar()->addMenu(tr("&文件"));
	file->addAction(openAction);
	//QToolBar *toolBar = addToolBar(tr("&File"));
	ui.mainToolBar->addAction(openAction);

	setMinimumSize(740, 520);
	m_graphicsView_src = new QGraphicsView();
	ui.horizontalLayout->addWidget(m_graphicsView_src);     //将自定义的组件添加到布局中
	m_graphicsScene_src = new QGraphicsScene;  //new 一个新的场景对象
	m_graphicsScene_src->setSceneRect(-370, -250, 740, 500);     //限定场景对象的显示区域
	m_graphicsView_src->setScene(m_graphicsScene_src);          //将视图对象于场景相连

	m_graphicsView_dst = new QGraphicsView();
	ui.horizontalLayout->addWidget(m_graphicsView_dst);     //将自定义的组件添加到布局中
	m_graphicsScene_dst = new QGraphicsScene;  //new 一个新的场景对象
	m_graphicsScene_dst->setSceneRect(-370, -250, 740, 500);     //限定场景对象的显示区域
	m_graphicsView_dst->setScene(m_graphicsScene_dst);          //将视图对象于场景相连

	connect(ui.pushButton_plate_recognize, SIGNAL(clicked()), this, SLOT(pushButton_plate_recognize_clicked()));
}

void mainWindow::pushButton_plate_recognize_clicked()
{
	if (!fileName.isEmpty())
	{
		ui.textBrowser->clear();
		Mat src = imread(fileName.toStdString());
		easypr::CPlateRecognize pr;
		//pr.setResultShow(true);//中间结果显示
		pr.setLifemode(true);//生活模式
		pr.setDebug(false);//调试模式
		pr.setMaxPlates(4);//可检测最大车牌数量
						   //pr.setDetectType(PR_DETECT_COLOR | PR_DETECT_SOBEL);
		pr.setDetectType(easypr::PR_DETECT_CMSER | easypr::PR_DETECT_COLOR | easypr::PR_DETECT_SOBEL);//设置为字符，颜色，sobel检测模式

		vector<easypr::CPlate> plateVec;		

		//从图片中识别出哪些区域可能为车牌区域？
		int result = pr.plateRecognize(src, plateVec);
		//int result = pr.plateRecognizeAsText(src, plateVec);
		if (result == 0) {
			size_t num = plateVec.size();
			for (size_t j = 0; j < num; j++) {
				ui.textBrowser->append(QString::fromStdString(plateVec[j].getPlateStr()));
				vPlateMat.push_back(plateVec[j].getPlateMat());
			}
			if (0!=vPlateMat.size())
			{
				pixItem_dst = new PixItem(&QPixmap::fromImage(cvMat2QImage(vPlateMat[0])));
				//将该图元对象添加到场景中，并设置此图元在场景中的位置为中心（0，0）
				m_graphicsScene_dst->addItem(pixItem_dst);
				pixItem_dst->setPos(0, 0);
			}
		}
		else
		{
			QMessageBox::critical(NULL, "警告", "未识别到车牌", QMessageBox::Yes);
		}

	}
	else
	{
		QMessageBox::critical(NULL, "警告", "请先打开图片", QMessageBox::Yes);
	}
}

void mainWindow::pushButton_open_clicked()
{
	m_graphicsScene_src->clear();
	fileName = QFileDialog::getOpenFileName(this,
		tr("打开图像"),//对话框名称
		".",//默认打开文件位置“.”文件目录"/"根目录
		tr("image files(*.jpg *.png *.bmp)"));//筛选器
	
	Cv:Mat srcImg = imread(fileName.toStdString());
	
	pixItem_src = new PixItem(&QPixmap::fromImage(cvMat2QImage(srcImg)));
	//将该图元对象添加到场景中，并设置此图元在场景中的位置为中心（0，0）
	m_graphicsScene_src->addItem(pixItem_src);
	pixItem_src->setPos(0, 0);


	//ui.label_src->setPixmap(QPixmap::fromImage(cvMat2QImage(srcImg)));

}


QImage mainWindow::cvMat2QImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1  
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)  
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat  
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3  
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		//qDebug() << "CV_8UC4";
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		//qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}