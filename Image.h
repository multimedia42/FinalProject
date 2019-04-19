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

	int Panorama();
};
