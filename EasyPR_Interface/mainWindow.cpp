#include "mainWindow.h"

#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QAction *openAction = new QAction(QIcon("icon/open.png"), tr("&��"), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an image file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(pushButton_open_clicked()));

	QMenu *file = menuBar()->addMenu(tr("&�ļ�"));
	file->addAction(openAction);
	//QToolBar *toolBar = addToolBar(tr("&File"));
	ui.mainToolBar->addAction(openAction);

	setMinimumSize(740, 520);
	m_graphicsView_src = new QGraphicsView();
	ui.horizontalLayout->addWidget(m_graphicsView_src);     //���Զ���������ӵ�������
	m_graphicsScene_src = new QGraphicsScene;  //new һ���µĳ�������
	m_graphicsScene_src->setSceneRect(-370, -250, 740, 500);     //�޶������������ʾ����
	m_graphicsView_src->setScene(m_graphicsScene_src);          //����ͼ�����ڳ�������

	m_graphicsView_dst = new QGraphicsView();
	ui.horizontalLayout->addWidget(m_graphicsView_dst);     //���Զ���������ӵ�������
	m_graphicsScene_dst = new QGraphicsScene;  //new һ���µĳ�������
	m_graphicsScene_dst->setSceneRect(-370, -250, 740, 500);     //�޶������������ʾ����
	m_graphicsView_dst->setScene(m_graphicsScene_dst);          //����ͼ�����ڳ�������

	connect(ui.pushButton_plate_recognize, SIGNAL(clicked()), this, SLOT(pushButton_plate_recognize_clicked()));
}

void mainWindow::pushButton_plate_recognize_clicked()
{
	if (!fileName.isEmpty())
	{
		ui.textBrowser->clear();
		Mat src = imread(fileName.toStdString());
		easypr::CPlateRecognize pr;
		//pr.setResultShow(true);//�м�����ʾ
		pr.setLifemode(true);//����ģʽ
		pr.setDebug(false);//����ģʽ
		pr.setMaxPlates(4);//�ɼ�����������
						   //pr.setDetectType(PR_DETECT_COLOR | PR_DETECT_SOBEL);
		pr.setDetectType(easypr::PR_DETECT_CMSER | easypr::PR_DETECT_COLOR | easypr::PR_DETECT_SOBEL);//����Ϊ�ַ�����ɫ��sobel���ģʽ

		vector<easypr::CPlate> plateVec;		

		//��ͼƬ��ʶ�����Щ�������Ϊ��������
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
				//����ͼԪ������ӵ������У������ô�ͼԪ�ڳ����е�λ��Ϊ���ģ�0��0��
				m_graphicsScene_dst->addItem(pixItem_dst);
				pixItem_dst->setPos(0, 0);
			}
		}
		else
		{
			QMessageBox::critical(NULL, "����", "δʶ�𵽳���", QMessageBox::Yes);
		}

	}
	else
	{
		QMessageBox::critical(NULL, "����", "���ȴ�ͼƬ", QMessageBox::Yes);
	}
}

void mainWindow::pushButton_open_clicked()
{
	m_graphicsScene_src->clear();
	fileName = QFileDialog::getOpenFileName(this,
		tr("��ͼ��"),//�Ի�������
		".",//Ĭ�ϴ��ļ�λ�á�.���ļ�Ŀ¼"/"��Ŀ¼
		tr("image files(*.jpg *.png *.bmp)"));//ɸѡ��
	
	Cv:Mat srcImg = imread(fileName.toStdString());
	
	pixItem_src = new PixItem(&QPixmap::fromImage(cvMat2QImage(srcImg)));
	//����ͼԪ������ӵ������У������ô�ͼԪ�ڳ����е�λ��Ϊ���ģ�0��0��
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