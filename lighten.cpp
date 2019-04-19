#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int getLightened(Mat srcimg,int trackBarValue)
{
	// Check for failure
	if (!srcimg.data) {
		cout << "no image" << endl;
		return -1;
	}
	while (true) {
		Mat increase = Mat::zeros(srcimg.size(), srcimg.type());
		srcimg.Mat::convertTo(increase, CV_8U, 1, trackBarValue);
	}
}
