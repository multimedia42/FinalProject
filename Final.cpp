#include "Image.h"

using namespace std;
using namespace cv;

int main()
{
	Image* image = new Image("test.jpg");

	if (image->panorama() == EXIT_SUCCESS)
	{
		namedWindow("Pano", WINDOW_AUTOSIZE);
		imshow("Pano", image->getDstMat());
	}

	waitKey();
	destroyAllWindows();

	return 0;
}