
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Image.h>

	   using namespace std;
	   using namespace cv;

	   class Window {
	   private:
		   String windowName;
	   public:
		   Window();
		   ~Window();

		   void Show(Mat matname);
		   void createTeackBar( String trackBarName, int initialValue, int maxValue);
		   void resizeCallback(int initialValue, void* userdata);
		   void darkenCallback(int initialValue, void* userdata);
		   void lightenCallback(int initialValue, void* userdata);
	   };