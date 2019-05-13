#include "Window.h"
#include"Image.h"
using namespace std;
using namespace cv;

Window::Window(Image image, string name)
{
	srcImage = image;
	nameWindow = name;
}

Window::Window() {}

Window::~Window()
{
	destroyWindow(nameWindow);
}

void Window::create()
{
	namedWindow(nameWindow, WINDOW_AUTOSIZE);
}

void Window::show()
{
	imshow(nameWindow, srcImage.getDstMat());
}

void Window::lightenCallback(int intensity, void *data)
{
	Window* p = (Window*)data;
	p->srcImage.lighten(intensity);
	//srcImage.lighten(intensity);
}

void Window::resizeCallback(int size, void* data)
{
	Window* p = (Window*)data;
	if (size < 1) {
		size = 1;
	}
	p->srcImage.resize(size);
	//srcImage.resize(size);
}

void Window::dilateCallback(int value, void *data) {
	
	Window* p = (Window*)data;
	if (value <= 0) {
		value = 1;
	}
	p->srcImage.dilation(value);
}

void Window::erodeCallback(int value, void *data) {

	Window* p = (Window*)data;
	if (value <= 0) {
		value = 1;
	}
	p->srcImage.erosion(value);
}

void Window::settrackbar(trackbarMode mode)
{
	int pos;
	String trackbarName("trackbar");
	String trackbarWindow("trackbar window");
	namedWindow(trackbarWindow);
	
	switch (mode) {
	case lightenTrackbar:
		pos = 100;
		createTrackbar(trackbarName, trackbarWindow, &pos, 200, lightenCallback, this);
		break;
	case resizeTrackbar:
		pos = 150;
		createTrackbar(trackbarName, trackbarWindow, &pos, 300, resizeCallback, this);
		break;
	case dilateTrackbar:
		pos = 1;
		createTrackbar(trackbarName, trackbarWindow, &pos, 50, dilateCallback, this);
		break;
	case erodeTrackbar:
		pos = 1;
		createTrackbar(trackbarName, trackbarWindow, &pos, 50, erodeCallback, this);
		break;
	}	
	while (true)
	{
		this->show();
		if (waitKey(10) == 27) {
			destroyAllWindows();
			break;
		}
	}
}



