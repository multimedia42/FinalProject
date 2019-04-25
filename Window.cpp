#include "pch.h"
#include "Window.h"
#include "Image.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Window::Window(Image image,string name){
	windowName = name;
	dstimg = image;
}

   Window::~Window() {
	   destroyWindow(windowName);
}

void Window::Show(){
	imshow(windowName, dstimg.getDstMat());

}

void Window::createTeackBar(String trackBarName, int initialValue, int maxValue, cv::TrackbarCallback Callback) {
	int tb = createTrackbar(trackBarName, windowName, &initialValue, maxValue, Callback, 0);
}

void Window::resizeCallback(int initialValue, void* userdata) {
	dstimg.getResized(initialValue);
}

void Window::lightenCallback(int initialValue, void* userdata) {
	dstimg.getLightened(initialValue);
}

/*cv::TrackbarCallback Window::cannyCallback(int initialValue, void* userdata) {
	dstimg.getCannied(initialValue);
}*/
