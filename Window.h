
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Image.h"

	   using namespace std;
	   using namespace cv;

	   class Window {
	   private:
		   String windowName;
		   Image srcimg();
	   public:
		   Window();
		   ~Window();

		   void Show(Mat matname);
		   void createTeackBar( String trackBarName, int initialValue, int maxValue, cv::TrackbarCallback Callback);
		   cv::TrackbarCallback resizeCallback(int initialValue, void* userdata);
		   cv::TrackbarCallback darkenCallback(int initialValue, void* userdata);
		   cv::TrackbarCallback lightenCallback(int initialValue, void* userdata);
	   };