#include "Image.h"

using namespace std;
using namespace cv;

vector<Mat> srcMats;
Mat srcMat;

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
    srcMats.release();
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