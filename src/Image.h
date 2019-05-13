#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>

using namespace std;
using namespace cv;

class Image
{
private:
	vector<Mat> srcMats;
	Mat srcMat;
	Mat dstMat;
	//VideoCapture vidSrc;

public:
	Image(String srcPath);
	//Image(String srcPath, String vidSrcPath);
	Image();
	~Image();

	int canny(int threshold);
	int lighten(int intensity);
	int panorama(Stitcher::Mode mode = Stitcher::PANORAMA);
	int resize(int size);
	int dilation(int value);
	int erosion(int value);
	Mat getDstMat();
};
