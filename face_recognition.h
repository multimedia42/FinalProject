#ifndef _FACE_RECOGNITION_
#define _FACE_RECOGNITION_

#include <iostream>

#include "opencv2/opencv.hpp"
#include "opencv2/face.hpp"

using namespace std;
using namespace cv;
using namespace face;

vector<Rect> recognize_face(Mat& img, CascadeClassifier& cascade, double scale);
void init_face(Ptr<FaceRecognizer>& model);
void save_and_draw_face(Ptr<FaceRecognizer>& model, vector<Rect> faces, Mat& frame, double scale);

#endif