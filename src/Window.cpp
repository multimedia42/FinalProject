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

void Window::dilationCallback(int value, void *data) {
	
	Window* p = (Window*)data;
	if (value <= 0) {
		value = 1;
	}
	p->srcImage.dilation(value);
}

void Window::erosionCallback(int value, void *data) {

	Window* p = (Window*)data;
	if (value <= 0) {
		value = 1;
	}
	p->srcImage.erosion(value);
}


void Window::settrackbar()
{
	namedWindow(nameWindow + "1");
	int pos = 50;
	createTrackbar(nameWindow + "1", nameWindow+"1", &pos, 200, erosionCallback, this);
	while (true)
	{
		this->show();
		if (waitKey(10) == 27) {
			destroyAllWindows();
			break;
		}
	}
}



