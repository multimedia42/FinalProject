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

  public:
	Image(String srcPath);
	int canny(int threshold);
	int panorama(Stitcher::Mode mode = Stitcher::PANORAMA);
	Mat getDstMat();
};
