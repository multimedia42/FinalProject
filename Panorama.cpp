#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int Panorama(String srcPath, Mat &dstMat, Stitcher::Mode mode = Stitcher::PANORAMA)
{
    vector<Mat> srcMat;
    vector<String> srcName;

    glob(srcPath, srcName, false);
    for (auto src : srcName)
    {
        Mat mat = imread(src);
        srcMat.push_back(mat);
    }

    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    Stitcher::Status status = stitcher->stitch(srcMat, dstMat);

    if (status == Stitcher::OK)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
    Mat dstMat;
    //getPanorama("../Data/pano", dstMat);

    namedWindow("Pano", WINDOW_AUTOSIZE);
    imshow("Pano", dstMat);

    waitKey();
    destroyAllWindows();

    return 0;
}