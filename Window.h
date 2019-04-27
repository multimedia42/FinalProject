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
	static Image srcImage;

public:
	enum function { resize, lighten };
	Window(Image image, string name);
	Window();
	~Window();

	void create();

	void show();
	void lightenCallback(int intensity, void *data);
	static void resizeCallback(int size, void *data);

	int setTrackbar(function func);
};