#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Image.h"

using namespace std;
using namespace cv;

enum trackbarMode{cannyTrackbar, lightenTrackbar, resizeTrackbar, dilateTrackbar, erodeTrackbar};

class Window
{
private:
	String nameWindow;
	Image srcImage;

public:
	Window(Image image, string name);
	Window();
	~Window();

	void create();

	void show();
	static void lightenCallback(int intensity, void *data);
	static void resizeCallback(int size, void *data);
	static void dilateCallback(int value, void *data);
	static void erodeCallback(int value, void *data);
	void settrackbar(trackbarMode mode);
};