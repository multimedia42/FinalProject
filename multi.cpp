// multi.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include"Image.h"
#include"Window.h"
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main()
{
	Image* image = new Image("C:\\Users\\Administrator\\source\\repos\\multi\\x64\\Debug\\sample.jpg");
	Window window(*image, "test1");
	window.create();
	window.settrackbar(2);
	waitKey();
	destroyAllWindows();
}
