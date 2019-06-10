﻿// multi.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define CVUI_IMPLEMENTATION
#include"Image.h"
#include "face_recognition.h"
#include"cvui.h"
#include"Window.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include "Video.h"

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
	Video video;
	VideoCapture camera;
	int low_threshold = 50, high_threshold = 150;
	int darkness = 100;
	int contrast = 100;
	int size = 100;
	int angle = 0;
	Mat M;
	bool use_rotate = false;
	int blur = 1;
	bool blur3 = false;
	bool blur5 = false;
	int gaussian = 1;
	bool gaussian3 = false;
	bool gaussian5 = false;
	int dilate_value = 1;
	int erode_value = 1;
	bool use_canny = false;
	bool use_face_recognition = false;
	bool isvideo = false;
	bool record = false;
	bool outputDone = false;

	int image_row = workspace_row;
	int image_col= workspace_col;

	double rate;
	Size videoSize;
	Mat key_face;

	Rect rect;
	Mat src;

	cv::Point anchor;
	cv::Rect roi;

	string cascadeName;
	Ptr<FaceRecognizer> model;
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
		Mat clear;
		if (use_face_recognition == true)
		{
			if (record == false)
			{
				camera >> src;
				src.copyTo(dst);
				combined.copyTo(clear);

				SetRect(rect, dst);
				dst.copyTo(clear(rect));
				clear.copyTo(pic);
				pic.copyTo(frame);
			}

			else
			{
				key_face.copyTo(src);
				src.copyTo(dst);
				combined.copyTo(clear);

				SetRect(rect, dst);
				dst.copyTo(clear(rect));
				clear.copyTo(pic);
				pic.copyTo(frame);

				faces = vector<Rect>(recognize_face(frame, cascade, scale));

				if (use_face_recognition) { //if face recognition mode is active
					cvui::printf(frame, 5, 5, 0.5, 0xff0000, "Click on the face you would like to save.");
					cvui::printf(frame, 5, 22, 0.5, 0xff0000, "Input its name on the terminal.");

					if (!faces.empty())
					{
						save_and_draw_face(model, faces, frame, scale);
					}
				}
			}
			

		}
		else
		{
			if (isvideo == false)
			{
				src.copyTo(dst);
				combined.copyTo(clear);
			}
			else
			{
				video.getCap() >> src;
				src.copyTo(dst);
				combined.copyTo(clear);
			}
			if (!dst.empty())
			{

				resize(dst, dst, Size(0, 0), size / 100.0, size / 100.0, INTER_LINEAR);
				dst.convertTo(dst, CV_8U, 1, (double)darkness - 100);
				dst.convertTo(dst, CV_8U, (double)contrast / 100, 0);

				Mat dilate_mask = getStructuringElement(MORPH_RECT, Size(dilate_value, dilate_value));
				dilate(dst, dst, dilate_mask);
				Mat erode_mask = getStructuringElement(MORPH_RECT, Size(erode_value, erode_value));
				erode(dst, dst, erode_mask);

				Point2f center(dst.cols / 2, dst.rows / 2);
				M = getRotationMatrix2D(center, angle, 1);
				warpAffine(dst, dst, M, dst.size());
				blur = 1;
				if (blur3) {
					blur = 3;
					cvui::checkbox(frame, workspace_col + 100, workspace_row - 240, "5*5", &blur5);
					if (blur5 == true)
					{
						cvui::checkbox(frame, workspace_col + 100, workspace_row - 240, "5*5", &blur5);
						blur3 = false;
						blur = 5;
					}
				}

				else if (blur5) {
					blur = 5;
					cvui::checkbox(frame, workspace_col + 30, workspace_row - 240, "3*3", &blur3);
					if (blur3 == true)
					{
						cvui::checkbox(frame, workspace_col + 30, workspace_row - 240, "3*3", &blur3);
						blur5 = false;
						blur = 3;
					}
				}

				if (!blur3 && !blur5)
				{
					blur = 1;
				}
				cv::blur(dst, dst, Size(blur, blur), Point(-1, -1), BORDER_DEFAULT);

				gaussian = 1;
				if (gaussian3) {
					gaussian = 3;
				}
				else if (gaussian5) {
					gaussian = 5;
				}
				else {
					gaussian = 1;
				}
				GaussianBlur(dst, dst, Size(gaussian, gaussian), 0, 0, BORDER_DEFAULT);

				if (use_canny) {
					cv::cvtColor(dst, dst, cv::COLOR_BGR2GRAY);
					cv::Canny(dst, dst, low_threshold, high_threshold, 3);
				}

				if (dst.channels() == 1)
					cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);

				SetRect(rect, dst);
				dst.copyTo(clear(rect));
				clear.copyTo(pic);
				pic.copyTo(frame);

				if (use_face_recognition) { //if face recognition mode is active
					cvui::printf(frame, 5, 5, 0.5, 0xff0000, "Click on the face you would like to save.");
					cvui::printf(frame, 5, 22, 0.5, 0xff0000, "Input its name on the terminal.");

					if (!faces.empty())
					{
						save_and_draw_face(model, faces, frame, scale);
					}
				}


			}
		}

		
		cvui::window(frame, workspace_col, 0, 300, workspace_row, "workspace");

		cvui::text(frame, workspace_col + 30, workspace_row - 670, "dilate");
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 655, 165, &dilate_value, 1, 25);

		cvui::text(frame, workspace_col + 30, workspace_row - 605, "erode");
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 590, 165, &erode_value, 1, 25);

		cvui::text(frame, workspace_col + 30, workspace_row - 540, "resize");
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 525, 165, &size, 1, (int)(Min((double)workspace_row / image_row, (double)workspace_col / image_col) * 100));
		
		cvui::text(frame, workspace_col + 30, workspace_row - 475, "ligthen/darken");
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 460, 165, &darkness, 0, 200);
		
		cvui::text(frame, workspace_col + 30, workspace_row - 410, "contrast");
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 395, 165, &contrast, 1, 400);
		
		cvui::text(frame, workspace_col + 30, workspace_row - 345, "rotate");
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 330, 165, &angle, 0, 360);

		cvui::text(frame, workspace_col + 30, workspace_row - 270, "blur");
		cvui::checkbox(frame, workspace_col + 30, workspace_row - 250, "3*3", &blur3);
		cvui::checkbox(frame, workspace_col + 100, workspace_row - 250, "5*5", &blur5);

		cvui::text(frame, workspace_col + 170, workspace_row - 270, "gaussian");
		cvui::checkbox(frame, workspace_col + 170, workspace_row - 250, "3*3", &gaussian3);
		cvui::checkbox(frame, workspace_col + 240, workspace_row - 250, "5*5", &gaussian5);

		cvui::checkbox(frame, workspace_col + 30, workspace_row - 215, "Use Canny Edge", &use_canny);
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 195, 165, &low_threshold, 5, 150);
		cvui::trackbar(frame, workspace_col + 30, workspace_row - 155, 165, &high_threshold, 80, 300);


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
			contrast = 100;
			angle = 0;
			dilate_value = 1;
			erode_value = 1;
			blur3 = false;
			blur5 = false;
			gaussian3 = false;
			gaussian5 = false;

			if (use_face_recognition)
			{
				use_face_recognition = false;
				camera.release();
				model->save("face.xml");
			}
		}
		if (cvui::button(frame, workspace_col + 30, workspace_row-90,80,30, "import")) {
			isvideo = false;
			image = Image("sample.jpg");
			image_row = image.getDstMat().rows;
			image_col = image.getDstMat().cols;
			image.getDstMat().copyTo(src);
			SetRect(rect, src);
			src.copyTo(pic(rect));
			low_threshold = 50;
			high_threshold = 150;
			darkness = 100;
			size = 100;
			contrast = 100;
			angle = 0;
			dilate_value = 1;
			erode_value = 1;
			blur3 = false;
			blur5 = false;
			gaussian3 = false;
			gaussian5 = false;

			if (use_face_recognition)
			{
				use_face_recognition = false;
				camera.release();
				model->save("face.xml");
			}
		}

		if (cvui::button(frame, workspace_col + 210, workspace_row - 90, 80, 30, "import video")) {
			isvideo = true;
			video = Video("chaplin.mp4");
			video.getCap() >> src;
			image_row = src.rows;
			image_col = src.cols;
			SetRect(rect, src);
			src.copyTo(pic(rect));
			low_threshold = 50;
			high_threshold = 150;
			darkness = 100;
			size = 100;
			contrast = 100;
			angle = 0;
			dilate_value = 1;
			erode_value = 1;
			blur3 = false;
			blur5 = false;
			gaussian3 = false;
			gaussian5 = false;

			if (use_face_recognition)
			{
				use_face_recognition = false;
				camera.release();
				model->save("face.xml");
			}

		}

		if (cvui::button(frame, workspace_col + 30, workspace_row - 50, 80, 30, "export")) {

			if (!isvideo) {

				imwrite("outputImage.jpg", dst);
			}
			else if (isvideo) {
				Video outputVideo = Video("chaplin.mp4");

				int width = outputVideo.getCap().get(CAP_PROP_FRAME_WIDTH);
				int height = outputVideo.getCap().get(CAP_PROP_FRAME_HEIGHT);
				int fourcc = VideoWriter::fourcc('M', 'J', 'P', 'G');
				double fps = 25.0;
				string filename = "outcpp.avi";
				VideoWriter vw = VideoWriter(filename, fourcc, fps, Size(width, height));
				while (1)
				{

					Mat result;
					outputVideo.getCap() >> result;
					if (result.empty())
					{
						outputDone = true;
						cout << "Found the end of the video" << endl;
						break;
					}
					resize(result, result, Size(0, 0), size / 100.0, size / 100.0, INTER_LINEAR);
					result.convertTo(result, CV_8U, 1, (double)darkness - 100);
					result.Mat::convertTo(result, -1, contrast / 100, 0);

					Mat dilate_mask = getStructuringElement(MORPH_RECT, Size(dilate_value, dilate_value));
					dilate(result, result, dilate_mask);
					Mat erode_mask = getStructuringElement(MORPH_RECT, Size(erode_value, erode_value));
					erode(result, result, erode_mask);

					Point2f center(result.cols / 2, result.rows / 2);
					M = getRotationMatrix2D(center, angle, 1);
					warpAffine(result, result, M, result.size());

					if (blur3) {
						blur = 3;
					}
					else if (blur5) {
						blur = 5;
					}
					else {
						blur = 1;
					}
					cv::blur(result, result, Size(blur, blur), Point(-1, -1), BORDER_DEFAULT);
					if (gaussian3) {
						gaussian = 3;
					}
					else if (gaussian5) {
						gaussian = 5;
					}
					else {
						gaussian = 1;
					}
					GaussianBlur(result, result, Size(gaussian, gaussian), 0, 0, BORDER_DEFAULT);
					vw.write(result);

				}

			}
		}
		if (outputDone) {
			cvui::text(frame, workspace_col + 30, workspace_row - 10, "done");
		}


		if (cvui::button(frame, workspace_col + 120, workspace_row - 50, 80, 30, "faces")) {//face button interaction
			if (use_face_recognition) { //if face mode already activated and clicked again
				use_face_recognition = false;
				camera.release();
				model->save("face.xml"); //end face recognition mode and save all faces in face.xml
			}
			else { //else if face mode is not active and face button is clicked
				use_face_recognition = true; //start face recognition mode
				camera.open(0);
				rate = 25.0;  //frame rate of video
				videoSize = Size((int)camera.get(CAP_PROP_FRAME_WIDTH), (int)camera.get(CAP_PROP_FRAME_HEIGHT));
				init_face(model);
				//faces = vector<Rect>(recognize_face(frame, cascade, scale));
			}
		}

		if (cvui::button(frame, workspace_col + 210, workspace_row - 50, 80, 30, "set face"))
		{
			if (record)
				record = false;
			else
			{
				camera >> key_face;
				record = true;

			}
		}

		cvui::update();

		cvui::imshow(WINDOW_NAME, frame);
	}
	if(use_face_recognition)
	model->save("face.xml");
	return 0;
}
