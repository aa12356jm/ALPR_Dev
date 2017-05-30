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
	//将图像读入内存
	cv::Mat src = imread(fileName.toStdString());
	if (src.empty())
	{
		QMessageBox::critical(this, "警告","未加载到图片");
		return 1;
	}

	vector<cv::Mat> resultVec;
	easypr::CPlateLocate plate;
	//plate.setDebug(0);
	//plate.setLifemode(true);
	//将图像中可能是车牌的多个区域都检测出来，保存到vector变量resultVec中
	int result = plate.plateLocate(src, resultVec);

	//将检测到的可能是车牌的矩形区域都显示出来
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

	//输入由车牌定位得到的多个矩形区域，由svm进行判断是否为车牌，将确定为车牌的区域输出到resultVec中
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

	//将车牌中的每个字符分割出来并显示
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
	//分割字符
	int result = cs.charsSegment(plate, matChars);
	if (result == 0)
	{
		for (size_t block_i = 0; block_i < matChars.size(); block_i++)
		{
			auto block = matChars[block_i];
			if (block_i == 0)
			{ //第一个字符标识为省的简称  	  
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

	std::string plateLicense = "苏E771H6";
#ifdef OS_WINDOWS
	plateLicense = easypr::utils::utf8_to_gbk(plateLicense.c_str());
#endif

	//显示出识别的字符和正确字符
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
	pr.setLifemode(true);//生活模式
	pr.setDebug(false);//调试模式
	pr.setMaxPlates(4);//可检测最大车牌数量
					   //pr.setDetectType(PR_DETECT_COLOR | PR_DETECT_SOBEL);
	pr.setDetectType(easypr::PR_DETECT_CMSER);//设置为字符检测模式

											  //vector<string> plateVec;
	vector<easypr::CPlate> plateVec;

	//从图片中识别出哪些区域可能为车牌区域？
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
		QMessageBox::critical(NULL, "警告", "请打开图片", QMessageBox::Yes);
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