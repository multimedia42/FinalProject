#include "Image.h"

using namespace std;
using namespace cv;

Image::Image(String srcPath)
{
    if (isFolder(srcPath))
    {
        vector<String> srcName;

        glob(srcPath, srcName, false);
        for (auto src : srcName)
        {
            Mat mat = imread(src);
            srcMats.push_back(mat);
        }
    }
    else
        srcMat = imread(srcPath);
}

Image::~Image()
{
    //srcMats.release();
    srcMat.release();
}

// true = isFolder
// false = isFile
bool isFolder(String path)
{
    size_t sizePath = path.length();
    wchar_t *bufferPath = new wchar_t[sizePath + 1];

    MultiByteToWideChar(CP_ACP, 0, path.c_str(), sizePath, bufferPath, sizePath * sizeof(wchar_t));
    bufferPath[sizePath] = 0;

    WIN32_FIND_DATA FindFileData;
    FindFirstFile(bufferPath, &FindFileData);
    if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    else
        return false;
}

int Image::Panorama(Mat &dstMat, Stitcher::Mode mode = Stitcher::PANORAMA)
{
    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    Stitcher::Status status = stitcher->stitch(srcMats, dstMat);

    if (status == Stitcher::OK)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}

Mat Image::getLightened(Mat srcimg, int trackBarValue)
{
	// Check for failure
	if (!srcimg.data) {
		cout << "no image" << endl;
		return -1;
	}
		Mat increase = Mat::zeros(srcimg.size(), srcimg.type());
		srcimg.Mat::convertTo(increase, CV_8U, 1, trackBarValue);
		return increase;
}

Mat Image::getDarkened(Mat srcimg, int trackBarValue)
{
	// Check for failure
	if (!srcimg.data) {
		cout << "no image" << endl;
		return -1;
	}
		Mat decrease = Mat::zeros(srcimg.size(), srcimg.type());
		srcimg.Mat::convertTo(decrease, CV_8U, 1, -trackBarValue);
		return decrease;
}

Mat Image::getResized(Mat srcimg, int trackBarValue)
{

	if (!srcimg.data) {
		cerr << "cannot load image" << endl;
		return -1;
	}
	Mat dstimg;

	// Scaling the image	
	double size = (double)trackBarValue / 100;
	resize(srcimg, dstimg, Size(0, 0), size, size, INTER_LINEAR);

	return dstimg;
}
