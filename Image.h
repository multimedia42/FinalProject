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

  public:
	Image(String srcPath);

	int Panorama(String srcPath, Mat &dstMat, Stitcher::Mode mode = Stitcher::PANORAMA);
	Mat getLightened(Mat srcimg, int trackBarValue);
	Mat getDarkened(Mat srcimg, int trackBarValue);
	Mat getResized(Mat srcimg, int trackBarValue);
};
