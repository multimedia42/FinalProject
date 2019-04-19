#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat getResized(Mat srcimg, int trackBarValue)
{
	
	if (!srcimg.data) {
		cerr << "cannot load image" << endl;
		return -1;
	}
	Mat dstimg;
	
	// Scaling the image	
		double size = (double)trackBarValue / 100;
		resize(srcimg, dstimg, Size(0, 0), size, size , INTER_LINEAR);

		return dstimg;
}