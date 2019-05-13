// multi.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include"Image.h"
#include"Window.h"
#include <iostream>
#include <opencv2/opencv.hpp>
enum callBack { resized = 2, lighten };

using namespace std;
using namespace cv;
enum ff{wwdd,ww};
int main()
{
	//Window::function test = Window::function::resize;
	Image* image = new Image("lena.png");
	/*if (image->panorama() == EXIT_SUCCESS)
	{
		namedWindow("Pano", WINDOW_AUTOSIZE);
		imshow("Pano", image->getDstMat());
	}*/
	Window window(*image, "test1");
	//imshow("test1", image->getDstMat());
	window.create();
	window.settrackbar();
	waitKey();

	destroyAllWindows();
}
