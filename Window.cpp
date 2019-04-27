#include "Window.h"

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
	srcImage.lighten(intensity);
}

void Window::resizeCallback(int size, void *data)
{
	srcImage.resize(size);
}


int Window::setTrackbar(function func)
{
	int pos = 1;
	switch (func)
	{
	case(resize):createTrackbar(nameWindow + "_resize", nameWindow, &pos, 100,resizeCallback); break;
	}
}