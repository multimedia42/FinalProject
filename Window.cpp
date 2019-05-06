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
	namedWindow(nameWindow);
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
	p->srcImage.resize(size);
	//srcImage.resize(size);
}

void Window::settrackbar()
{
	int pos = 50;
	createTrackbar(nameWindow + "1", nameWindow, &pos, 100, lightenCallback,this);
	while (true)
	{
		imshow(nameWindow, srcImage.getDstMat());
		waitKey(10);
	}
}



