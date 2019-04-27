#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Image.h"

using namespace std;
using namespace cv;

class Window
{
private:
	String nameWindow;
	Image srcImage;

public:
	Window(Image image, string name);
	Window();
	~Window();

	void show();
	void lightenCallback(int intensity, void *data);
	void resizeCallback(int size, void *data);

	int setTrackbar(String nameTrackbar, int initValue, int maxValue, TrackbarCallback callback);
};