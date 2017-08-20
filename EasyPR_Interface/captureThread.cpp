#include "captureThread.h"
#include <QMessageBox>

#pragma execution_character_set("utf-8")

captureThread::captureThread(QObject *parent)
	: QThread(parent)
{
	pause_status = true;
	quit_status = 0;

	for (int i=0;i<256;i++)
	{
		grayColorTable.append(qRgb(i, i, i));
	}
	error_image = cv::Mat(800, 300, CV_8UC3, cv::Scalar(255, 0, 0));

}

captureThread::~captureThread()
{
	if (capture.isOpened())
	{
		capture.release();
	}
}


void captureThread::run()
{
	if (capture.isOpened())
	{
		capture.release();
	}

	capture.open(0);
	//这里需要开启线程显示，否则会卡死
	if (capture.isOpened())
	{
		while (quit_status == 0)
		{
			if (pause_status)
			{
				usleep(1000);
				continue;
			}

			cv::Mat frame_easyPR;
			cv::Mat frame_openALPR;
			capture >> frame_easyPR;
			capture >> frame_openALPR;

			//assert(frame_easyPR.data != nullptr);
			//assert(frame_openALPR.data != nullptr);

			if (frame_openALPR.data == nullptr)
			{
				//QMessageBox("没有捕捉到图像");
				continue;
			}
			//plateStr.clear();

			plateRecognize_easyPR(frame_easyPR, plateStr_easyPR);

			plateRecognize_openALPR(frame_openALPR, plateStr_openALPR);

			if (0!= plateStr_easyPR.size())
			{
				emit capturedStr(0,plateStr_easyPR);
			}
			if (0!=plateStr_openALPR.size())
			{
				emit capturedStr(1,plateStr_openALPR);
			}
			emit captured(0,cvMat2QImage(frame_easyPR));

			emit captured(1,cvMat2QImage(frame_openALPR));
			cv::waitKey(30);
		}
	}
}

void captureThread::stream()
{
	pause_status = false;
}

void captureThread::stop()
{
	pause_status = false;
	quit_status = 1;
}

void captureThread::pause()
{
	pause_status = true;
}

bool captureThread::plateRecognize_easyPR(cv::Mat &img, vector<QString> &plateStr_easyPR)
{
		plateStr_easyPR.clear();
		vector<cv::Mat> vPlateMat;
		vector<cv::RotatedRect> vPlateRotatedRect;

		easypr::CPlateRecognize pr;
		pr.setResultShow(false);//中间结果显示
		pr.setLifemode(true);//生活模式
		pr.setDebug(false);//调试模式
		pr.setMaxPlates(4);//可检测最大车牌数量
						   //pr.setDetectType(PR_DETECT_COLOR | PR_DETECT_SOBEL);
		pr.setDetectType(easypr::PR_DETECT_CMSER | easypr::PR_DETECT_COLOR | easypr::PR_DETECT_SOBEL);//设置为字符，颜色，sobel检测模式

		vector<easypr::CPlate> plateVec;

		
		//从图片中识别出哪些区域可能为车牌区域？
		int result = pr.plateRecognize(img, plateVec);
		//int result = pr.plateRecognizeAsText(src, plateVec);
		if (result == 0) 
		{
			size_t num = plateVec.size();
			for (size_t j = 0; j < num; j++) {		
				//vPlateMat.push_back(plateVec[j].getPlateMat());
				vPlateRotatedRect.push_back(plateVec[j].getPlatePos());
				cv::rectangle(img, vPlateRotatedRect[j].boundingRect(),Scalar(0,0,255),5,8,0);
				plateStr_easyPR.push_back(QString::fromStdString(plateVec[j].getPlateStr()));
			}
			//if (0 != vPlateMat.size())
			//{
				
// 				pixItem_dst = new PixItem(&QPixmap::fromImage(cvMat2QImage(vPlateMat[0])));
// 				//将该图元对象添加到场景中，并设置此图元在场景中的位置为中心（0，0）
// 				m_graphicsScene_dst->addItem(pixItem_dst);
// 				pixItem_dst->setPos(0, 0);
			//}
		}
		return true;
}

bool captureThread::plateRecognize_openALPR(cv::Mat &srcImg, vector<QString> &plateStr_openALPR)
{
	plateStr_openALPR.clear();

	alpr::Alpr openAlpr("us", "openalpr.conf");
	/*m_openAlpr = new alpr::Alpr("us", "openalpr.conf");*/
	openAlpr.setTopN(5);
	openAlpr.setDefaultRegion("md");

	if (openAlpr.isLoaded() == false)
	{
		return false;
	}

	//Mat srcImg = imread("ch-1.jpg");

	alpr::AlprResults results = openAlpr.recognize(srcImg);
	

	for (int i = 0; i < results.plates.size(); i++)
	{
		alpr::AlprPlateResult plate = results.plates[i];
		cv::Rect rect(plate.plate_points[0].x, plate.plate_points[0].y, plate.plate_points[1].x - plate.plate_points[0].x, plate.plate_points[3].y - plate.plate_points[0].y);

		cv::rectangle(srcImg, rect, Scalar(0, 0, 255),5,8);


		//ui.textBrowser->append(QString("一共有%1个结果").arg(plate.topNPlates.size()));
		for (int k = 0; k < plate.topNPlates.size(); k++)
		{
			alpr::AlprPlate candidate = plate.topNPlates[k];

			plateStr_openALPR.push_back(QString("-%1 \t confidence:%2").arg(QString::fromStdString(candidate.characters)).arg(candidate.overall_confidence));
			plateStr_openALPR.push_back(QString("\t pattern_match:%1").arg(candidate.matches_template));

			//ui.textBrowser->append(QString("-%1 \t confidence:%2").arg(QString::fromStdString(candidate.characters)).arg(candidate.overall_confidence));
			//ui.textBrowser->append(QString("\t pattern_match:%1").arg(candidate.matches_template));
			//std::cout << "    - " << candidate.characters << "\t confidence: " << candidate.overall_confidence;
			//std::cout << "\t pattern_match: " << candidate.matches_template << std::endl;
		}
	}
	return true;
}

QImage captureThread::cvMat2QImage(const cv::Mat& mat)
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
