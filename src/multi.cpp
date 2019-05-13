// multi.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include"Image.h"
#include"Window.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
enum ff{wwdd,ww};
int main()
{/*
	//Window::function test = Window::function::resize;
	Image* image = new Image("lena.png");
	/*if (image->panorama() == EXIT_SUCCESS)
	{
		namedWindow("Pano", WINDOW_AUTOSIZE);
		imshow("Pano", image->getDstMat());
	}
	Window window(*image, "test1");
	//imshow("test1", image->getDstMat());
	window.create();
	window.settrackbar();
	waitKey();

	destroyAllWindows();
	*/
	
	Image* image = new Image("lena.png");
	Window window(*image, "test1");
	int loop = 1;
	int menu;
	int ret;

	//console menu
	while (loop) {
		cout << "1. canny()" << endl;
		cout << "2. lighten()" << endl;
		cout << "3. panorama()" << endl;
		cout << "4. resize()" << endl;
		cout << "5. dilate()" << endl;
		cout << "6. erode()" << endl;
		cout << "Default. exit" << endl;

		cin >> menu;

		switch (menu) {
		case 1:
			ret = 1; //canny
			break;

		case 2:
			window.settrackbar(lightenTrackbar);
			break;

		case 3:
			ret = 3; //panorama
			break;

		case 4:
			window.settrackbar(resizeTrackbar);
			break;

		case 5:
			window.settrackbar(dilateTrackbar);
			break;

		case 6:
			window.settrackbar(erodeTrackbar);
			break;

		default:
			loop = 0;
		}
		cout << endl;
	}
	return 0;
}
