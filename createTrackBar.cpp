#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void createTrackBar(String windowName, String trackBarName, int initialValue, int maxValue) {
	int tb = createTrackbar(trackBarName, windowName, &initialValue, maxValue, 0, 0);
}