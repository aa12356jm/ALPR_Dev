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



	connect(ui.pushButton_plate_locate, SIGNAL(clicked()), this, SLOT(pushButton_plate_locate_clicked()));
	connect(ui.pushButton_plate_judge, SIGNAL(clicked()), this, SLOT(pushButton_plate_judge_clicked()));
	connect(ui.pushButton_plate_detect, SIGNAL(clicked()), this, SLOT(pushButton_plate_detect_clicked()));
	connect(ui.pushButton_chars_segment, SIGNAL(clicked()), this, SLOT(pushButton_chars_segment_clicked()));
	connect(ui.pushButton_chars_identify, SIGNAL(clicked()), this, SLOT(pushButton_chars_identify_clicked()));
	connect(ui.pushButton_chars_recognise, SIGNAL(clicked()), this, SLOT(pushButton_chars_recognise_clicked()));
	connect(ui.pushButton_plate_recognize, SIGNAL(clicked()), this, SLOT(pushButton_plate_recognize_clicked()));
}

int mainWindow::test_plate_locate()
{
	//cout << "test_plate_locate" << endl;

	//const string file = "../resources/image/plate_locate.jpg";
	//��ͼ������ڴ�
	cv::Mat src = imread(fileName.toStdString());
	if (src.empty())
	{
		QMessageBox::critical(this, "����","δ���ص�ͼƬ");
		return 1;
	}

	vector<cv::Mat> resultVec;
	easypr::CPlateLocate plate;
	//plate.setDebug(0);
	//plate.setLifemode(true);
	//��ͼ���п����ǳ��ƵĶ�����򶼼����������浽vector����resultVec��
	int result = plate.plateLocate(src, resultVec);

	//����⵽�Ŀ����ǳ��Ƶľ���������ʾ����
	if (result == 0)
	{
		size_t num = resultVec.size();
		for (size_t j = 0; j < num; j++)
		{
			cv::Mat resultMat = resultVec[j];
			imshow("plate_locate", resultMat);
			waitKey(0);
		}
		destroyWindow("plate_locate");
	}
	return result;
}

int mainWindow::test_plate_judge()
{
	cout << "test_plate_judge" << endl;

	cv::Mat src = imread("resources/image/plate_judge.jpg");

	vector<cv::Mat> matVec;

	vector<cv::Mat> resultVec;

	easypr::CPlateLocate lo;
	lo.setDebug(1);
	lo.setLifemode(true);

	int resultLo = lo.plateLocate(src, matVec);

	if (0 != resultLo) return -1;

	cout << "plate_locate_img" << endl;
	size_t num = matVec.size();
	for (size_t j = 0; j < num; j++) {
		Mat resultMat = matVec[j];
		imshow("plate_judge", resultMat);
		waitKey(0);
	}
	destroyWindow("plate_judge");

	//�����ɳ��ƶ�λ�õ��Ķ������������svm�����ж��Ƿ�Ϊ���ƣ���ȷ��Ϊ���Ƶ����������resultVec��
	int resultJu = easypr::PlateJudge::instance()->plateJudge(matVec, resultVec);

	if (0 != resultJu)
		return -1;

	cout << "plate_judge_img" << endl;
	num = resultVec.size();
	for (size_t j = 0; j < num; j++) {
		Mat resultMat = resultVec[j];
		imshow("plate_judge", resultMat);
		waitKey(0);
	}
	destroyWindow("plate_judge");

	return resultJu;

}

int mainWindow::test_plate_detect()
{
	cout << "test_plate_detect" << endl;

	cv::Mat src = imread("resources/image/plate_detect.jpg");

	vector<easypr::CPlate> resultVec;
	easypr::CPlateDetect pd;
	pd.setPDLifemode(true);

	int result = pd.plateDetect(src, resultVec);
	if (result == 0)
	{
		size_t num = resultVec.size();
		for (size_t j = 0; j < num; j++)
		{
			easypr::CPlate resultMat = resultVec[j];
			imshow("plate_detect", resultMat.getPlateMat());
			waitKey(0);
		}
		destroyWindow("plate_detect");
	}

	return result;

}

int mainWindow::test_chars_segment()
{
	std::cout << "test_chars_segment" << std::endl;

	cv::Mat src = cv::imread("resources/image/chars_segment.jpg");

	std::vector<cv::Mat> resultVec;
	easypr::CCharsSegment plate;

	//�������е�ÿ���ַ��ָ��������ʾ
	int result = plate.charsSegment(src, resultVec);
	if (result == 0) {
		size_t num = resultVec.size();
		for (size_t j = 0; j < num; j++) {
			cv::Mat resultMat = resultVec[j];
			cv::imshow("chars_segment", resultMat);
			cv::waitKey(0);
		}
		cv::destroyWindow("chars_segment");
	}

	return result;

}

int mainWindow::test_chars_identify()
{
	std::cout << "test_chars_identify" << std::endl;
	cv::Mat plate = cv::imread("resources/image/chars_identify.jpg");

	std::vector<Mat> matChars;
	std::string license;

	easypr::CCharsSegment cs;
	//�ָ��ַ�
	int result = cs.charsSegment(plate, matChars);
	if (result == 0)
	{
		for (size_t block_i = 0; block_i < matChars.size(); block_i++)
		{
			auto block = matChars[block_i];
			if (block_i == 0)
			{ //��һ���ַ���ʶΪʡ�ļ��  	  
				auto character = easypr::CharsIdentify::instance()->identify(block, true);
				license.append(character.second);
			}
			else
			{
				auto character = easypr::CharsIdentify::instance()->identify(block, false);
				license.append(character.second);
			}
		}
		//for (auto block : matChars) {
		//  auto character = CharsIdentify::instance()->identify(block);
		//  license.append(character.second);
		//}
	}

	std::string plateLicense = "��E771H6";
#ifdef OS_WINDOWS
	plateLicense = easypr::utils::utf8_to_gbk(plateLicense.c_str());
#endif

	//��ʾ��ʶ����ַ�����ȷ�ַ�
	std::cout << "plateLicense: " << plateLicense << std::endl;
	std::cout << "plateIdentify: " << license << std::endl;

	if (plateLicense != license) {
		std::cout << "Identify Not Correct!" << std::endl;
		return -1;
	}
	std::cout << "Identify Correct!" << std::endl;

	return result;

}

int mainWindow::test_chars_recognise()
{
	std::cout << "test_chars_recognise" << std::endl;

	cv::Mat src = cv::imread("../resources/image/chars_recognise.jpg");
	easypr::CCharsRecognise cr;
	//return 0;
	std::string plateLicense = "";
	int result = cr.charsRecognise(src, plateLicense);
	if (result == 0)
		std::cout << "charsRecognise: " << plateLicense << std::endl;
	return 0;

}

int mainWindow::test_plate_recognize()
{
	cout << "test_plate_recognize" << endl;
	Mat src = imread("resources/image/test.jpg");
	easypr::CPlateRecognize pr;
	pr.setLifemode(true);//����ģʽ
	pr.setDebug(false);//����ģʽ
	pr.setMaxPlates(4);//�ɼ�����������
					   //pr.setDetectType(PR_DETECT_COLOR | PR_DETECT_SOBEL);
	pr.setDetectType(easypr::PR_DETECT_CMSER);//����Ϊ�ַ����ģʽ

											  //vector<string> plateVec;
	vector<easypr::CPlate> plateVec;

	//��ͼƬ��ʶ�����Щ�������Ϊ��������
	int result = pr.plateRecognize(src, plateVec);
	//int result = pr.plateRecognizeAsText(src, plateVec);
	if (result == 0) {
		size_t num = plateVec.size();
		for (size_t j = 0; j < num; j++) {
			cout << "plateRecognize: " << plateVec[j].getPlateStr() << endl;
		}
	}

	if (result != 0) cout << "result:" << result << endl;

	return result;
}

void mainWindow::pushButton_plate_locate_clicked()
{
	if (!fileName.isEmpty())
	{
		test_plate_locate();
	}
	else
	{
		QMessageBox::critical(NULL, "����", "���ͼƬ", QMessageBox::Yes);
		return;
	}
	

}

void mainWindow::pushButton_plate_judge_clicked()
{
	test_plate_judge();
}

void mainWindow::pushButton_plate_detect_clicked()
{
	test_plate_detect();
}

void mainWindow::pushButton_chars_segment_clicked()
{
	test_chars_segment();
}

void mainWindow::pushButton_chars_identify_clicked()
{
	test_chars_identify();
}

void mainWindow::pushButton_chars_recognise_clicked()
{
	test_chars_recognise();
}

void mainWindow::pushButton_plate_recognize_clicked()
{
	test_plate_recognize();
}

void mainWindow::pushButton_test_all_clicked()
{

}

void mainWindow::pushButton_open_clicked()
{
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