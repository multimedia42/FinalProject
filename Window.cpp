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

int Window::setTrackbar(String nameTrackbar, int initValue, int maxValue, TrackbarCallback callback)
{
	return createTrackbar(nameTrackbar, nameWindow, &initValue, maxValue, callback, 0);
}