#include "Window.h"
#include "Image.h"


using namespace std;
using namespace cv;

Window::Window(){
	namedWindow(windowName, WINDOW_AUTOSIZE);
	Image srcImg = new Image();
}

   Window::~Window() {
	   destroyWindow(windowName);
}

void Window::Show(Mat matname){
	imshow(windowName, matname);

}

void Window::createTeackBar(String trackBarName, int initialValue, int maxValue, cv::TrackbarCallback Callback) {
	int tb = createTrackbar(trackBarName, windowName, &initialValue, maxValue, Callback, 0);
}

cv::TrackbarCallback Window::resizeCallback(int initialValue, void* userdata) {
	srcimg.getResized(initialValue);
}

cv::TrackbarCallback Window::lightenCallback(int initialValue, void* userdata) {
	srcimg.getLightened(initialValue);
}
cv::TrackbarCallback Window::resizeCallback(int initialValue, void* userdata) {
	srcimg.getDarkened(initialValue);
}
