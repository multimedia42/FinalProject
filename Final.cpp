#include "Image.h"

using namespace std;
using namespace cv;

int main()
{
	Image* image = new Image("../Data/pano");

	if (image->panorama() == EXIT_SUCCESS)
	{
		namedWindow("Pano", WINDOW_AUTOSIZE);
		imshow("Pano", image->getDstMat());
	}

	waitKey();
	destroyAllWindows();

	return 0;
}