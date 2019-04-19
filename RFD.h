#ifndef _RFD_
#define _RFD_
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int  getLightened(Mat srcimg);
int getDarkened(Mat srcimg);
int getResized(Mat srcimg);

#endif // _RFD_

