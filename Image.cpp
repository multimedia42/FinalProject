#include "pch.h"
#include "Image.h"
#include<opencv2/opencv.hpp>
#include<Windows.h>
using namespace std;
using namespace cv;

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

// true = isFolder
// false = isFile

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
Image::Image() {};

Image::~Image()
{
    for (auto src : srcMats)
        src.release();
    srcMat.release();
}



int Image::canny(int threshold)
{
    Mat GrayImage;
    Mat CannyImage;

    GrayImage.create(srcMat.size(), srcMat.type());
    cvtColor(srcMat, GrayImage, COLOR_BGR2GRAY);
    CannyImage.create(srcMat.size(), srcMat.type());
    Canny(GrayImage, CannyImage, threshold, threshold * 3, 3);
    dstMat = CannyImage;
	if(!CannyImage.empty())
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

int Image::panorama(Stitcher::Mode mode)
{
    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    Stitcher::Status status = stitcher->stitch(srcMats, dstMat);

    if (status == Stitcher::OK)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}

Mat Image::getDstMat()
{
    return dstMat;
}

Mat Image::getLightened(int trackBarValue)
{

	Mat increase = Mat::zeros(srcMat.size(), srcMat.type());
	srcMat.Mat::convertTo(increase, CV_8U, 1, trackBarValue - 50);
	return increase;
}



Mat Image::getResized(int trackBarValue)
{
	Mat dstimg;

	// Scaling the image	
	double size = (double)trackBarValue / 100;
	resize(srcMat, dstimg, Size(0, 0), size, size, INTER_LINEAR);

	return dstimg;
}
