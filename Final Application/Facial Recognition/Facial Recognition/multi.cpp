// multi.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define CVUI_IMPLEMENTATION
#include"Image.h"
#include"cvui.h"
#include"Window.h"
#include "face_recognition.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

#define WINDOW_NAME "Simple Lightroom"
#define FACE_WINDOW "Face Recognition"

using namespace std;
using namespace cv;
using namespace face;

int workspace_row = 700;
int workspace_col = 1000;

void SetRect(Rect& rect,Mat image)
{
	rect.height = image.rows;
	rect.width = image.cols;
	rect.x = (workspace_col - rect.width) / 2;
	rect.y = (workspace_row - rect.height) / 2;
}

double Min(double x, double y)
{
	return((x < y) ? x : y);
}


int main()
{
	cv::namedWindow(WINDOW_NAME);
	cvui::init(WINDOW_NAME);

	cv::Mat combined;
	cv::Mat pic = cv::Mat(workspace_row, workspace_col, CV_8UC3, Scalar(127, 127, 127));
	cv::Mat extra = Mat(workspace_row, 300, CV_8UC3, Scalar(0, 0, 0));
	hconcat(pic, extra, combined);

	combined.copyTo(pic);
	cv::Mat frame;
	pic.copyTo(frame);
	Image image;
	Image images;
	int low_threshold = 50, high_threshold = 150;
	int darkness = 100;
	int size = 100;
	int dilate_value = 1;
	int erode_value = 1;
	bool use_canny = false;
	bool use_face_recognition = false;
	int image_row = workspace_row;
	int image_col= workspace_col;
	Rect rect;
	Mat src;

	//declaration for face recognition
	Ptr<FaceRecognizer> model;
	string cascadeName;
	vector<Rect> faces;
	CascadeClassifier cascade;
	cascadeName = "data/haarcascades/haarcascade_frontalface_alt.xml";
	if (!cascade.load(samples::findFile(cascadeName)))
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return -1;
	}
	double scale = 1;

	while (cv::waitKey(30) != 27) {
		Mat dst;
		src.copyTo(dst);
		Mat clear;
		combined.copyTo(clear);

		if (!dst.empty())
		{
			if (use_canny) {
				cv::cvtColor(pic, frame, cv::COLOR_BGR2GRAY);
				cv::Canny(frame, frame, low_threshold, high_threshold, 3);
			}
			else {
				resize(dst, dst, Size(0, 0), size  / 100.0, size  / 100.0, INTER_LINEAR);
				dst.convertTo(dst, CV_8U, 1, (double)darkness - 100);

				Mat dilate_mask = getStructuringElement(MORPH_RECT, Size(dilate_value, dilate_value));
				dilate(dst, dst, dilate_mask);
				Mat erode_mask = getStructuringElement(MORPH_RECT, Size(erode_value, erode_value));
				erode(dst, dst, erode_mask);

				SetRect(rect, dst);
				dst.copyTo(clear(rect));
				clear.copyTo(pic);
				pic.copyTo(frame);

				if (use_face_recognition) { //if face recognition mode is active
					cvui::printf(frame, 5, 5, 0.5, 0xff0000, "Click on the face you would like to save.");
					cvui::printf(frame, 5, 22, 0.5, 0xff0000, "Input its name on the terminal.");

					if (!faces.empty()) { //if there are faces in the image
						save_and_draw_face(model, faces, frame, scale);
					}
				}
				
			}
		}
		cvui::window(frame, workspace_col,0,300, workspace_row,"workspace");

		cvui::text(frame, workspace_col + 30, workspace_row - 630, "dilate");
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 600, 165, &dilate_value, 1, 25);

		cvui::text(frame, workspace_col + 30, workspace_row - 530, "erode");
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 500, 165, &erode_value, 1, 25);

		cvui::text(frame, workspace_col + 30, workspace_row - 430, "resize");
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 400, 165, &size, 1, (int)(Min((double)workspace_row / image_row, (double)workspace_col / image_col) * 100));
		cvui::text(frame, workspace_col + 30, workspace_row - 330,"ligthen/darken");
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 300, 165, &darkness, 0, 200);

		cvui::checkbox(frame, workspace_col + 30, workspace_row - 230, "Use Canny Edge", &use_canny);
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 200, 165, &low_threshold, 5, 150);
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 150, 165, &high_threshold, 80, 300);

		if (cvui::button(frame, workspace_col + 120, workspace_row - 90, 80, 30, "multi import"))
		{
			images = Image("pano");
			images.panorama();
			image_row = images.getDstMat().rows;
			image_col = images.getDstMat().cols;
			images.getDstMat().copyTo(src);
			SetRect(rect, src);
			src.copyTo(pic(rect));
			low_threshold = 50;
			high_threshold = 150;
			darkness = 100;
			size = 100;
		}
		if (cvui::button(frame, workspace_col+30,workspace_row-90,80,30, "import")) {
			image = Image("lena.png");
			image_row = image.getDstMat().rows;
			image_col = image.getDstMat().cols;
			image.getDstMat().copyTo(src);
			SetRect(rect, src);
			src.copyTo(pic(rect));
			low_threshold = 50;
			high_threshold = 150;
			darkness = 100;
			size = 100;
		}
		if (cvui::button(frame, workspace_col+30, workspace_row-50,80, 30, "export")) {
			
		}
		if (cvui::button(frame, workspace_col + 120, workspace_row - 50, 80, 30, "faces")) {//face button interaction
			if (use_face_recognition) { //if face mode already activated and clicked again
				use_face_recognition = false;
				model->save("face.xml"); //end face recognition mode and save all faces in face.xml
			}
			else { //else if face mode is not active and face button is clicked
				use_face_recognition = true; //start face recognition mode
				init_face(model);
				faces = vector<Rect>(recognize_face(frame, cascade, scale)); //get all the faces from the image
			}
		}

		cvui::update();

		cvui::imshow(WINDOW_NAME, frame);
	}
	model->save("face.xml");
	return 0;
//	window.create();
//	window.settrackbar(2);
//	waitKey();
//	destroyAllWindows();
}
