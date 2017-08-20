#pragma once

#include "captureThread.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"


TEST(openalpr, FunctionTest)
{
	std::ostringstream os;
	//os << SAMPLE_PATH << "./us-1.jpg";
	Mat src = imread(os.str());
	ASSERT_TRUE(src.data != nullptr);
	vector<QString> plateStr_openALPR;

	auto result = captureThread::plateRecognize_openALPR(src, plateStr_openALPR);

}