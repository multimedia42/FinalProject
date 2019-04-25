#include "pch.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Image.h"

	   using namespace std;
	   using namespace cv;

	   class Window {
	   private:
		   String windowName;
		   Image dstimg;
	   public:
		   Window(Image image, string name);
		   ~Window();

		   void Show();
		   void createTeackBar( String trackBarName, int initialValue, int maxValue, cv::TrackbarCallback Callback);
		   void resizeCallback(int initialValue, void* userdata);
		   void lightenCallback(int initialValue, void* userdata);
		   //cv::TrackbarCallback cannyCallback(int initialValue, void * userdata);
	   };