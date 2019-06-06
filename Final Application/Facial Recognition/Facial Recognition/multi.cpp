// multi.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define CVUI_IMPLEMENTATION
#include"Image.h"
#include"cvui.h"
#include"Window.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

#define WINDOW_NAME "Simple Lightroom"
#define FACE_WINDOW "Face Recognition"

using namespace std;
using namespace cv;
using namespace face;

string cascadeName;

Ptr<FaceRecognizer> model;

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

vector<Rect> recognize_face(Mat& img, CascadeClassifier& cascade,
	double scale)
{
	vector<Rect> faces, faces2;
	vector<Mat> images;
	vector<int> labels;
	const static Scalar colors[] =
	{
		Scalar(255,0,0),
		Scalar(255,128,0),
		Scalar(255,255,0),
		Scalar(0,255,0),
		Scalar(0,128,255),
		Scalar(0,255,255),
		Scalar(0,0,255),
		Scalar(255,0,255)
	};
	Mat gray, smallImg;

	//cvtColor(img, gray, COLOR_BGR2GRAY);
	gray = img.clone(); //alternative by not converting to grey
	double fx = 1 / scale;
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT);
	//equalizeHist(smallImg, smallImg);

	cascade.detectMultiScale(smallImg, faces,
		1.1, 2, 0
		| CASCADE_SCALE_IMAGE,
		Size(30, 30));

	return faces;
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

	cv::Point anchor;
	cv::Rect roi;

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
						for (size_t i = 0; i < faces.size(); i++) //for all faces
						{
							Rect r = faces[i];
							Point center;
							Scalar face_color(255, 0, 0); //blue
							int radius;
							bool save_face = false;
							double aspect_ratio = (double)r.width / r.height;

							//get the rectangle of the face
							Rect found_face(Point(cvRound(r.x*scale), cvRound(r.y*scale)), Point(cvRound((r.x + r.width - 1)*scale), cvRound((r.y + r.height - 1)*scale)));

							//convert to gray to train the FaceRecognizer
							Mat gray;
							cvtColor(Mat(frame, found_face), gray, COLOR_BGR2GRAY);

							//get the circle of the face
							center.x = cvRound((r.x + r.width*0.5)*scale);
							center.y = cvRound((r.y + r.height*0.5)*scale);
							radius = cvRound((r.width + r.height)*0.25*scale);

							//if the user clicks on a face
							if (cvui::mouse(cvui::LEFT_BUTTON, cvui::DOWN)) {
								Point mouse_point(cvui::mouse().x, cvui::mouse().y);								
								if (0.75 < aspect_ratio && aspect_ratio < 1.3) {
									Point diff = mouse_point - center;
									int distance = (int)sqrt(diff.x*diff.x + diff.y*diff.y);
									if (distance < radius) {
										save_face = true;
									}
								}
								else if (mouse_point.inside(r))
								{
									save_face = true;
								}
								if(save_face){
									//save the face
									string face_name;
									cout << "Input name of face:" << endl;
									getline(cin, face_name);

									vector<Mat> images;
									vector<int> labels;
									int predicted;
									if (!model->empty()) {
										predicted = model->predict(gray);

										if (predicted == -1) {
											vector<int> match = model->getLabelsByString(face_name);
											if (match.size() > 1) {
												for (size_t j = 0; j < match.size(); j++) {
													int match_label = match[j];
													String match_name = model->getLabelInfo(match_label);
													if (match_name == face_name) {
														predicted = match_label;
													}
												}
											}
											else if (match.size() == 1) {
												predicted = match.front();
											}
											else {
												for (i = 0; !model->getLabelInfo(i).empty(); i++);
												predicted = i;
											}
										}
									}
									else {
										predicted = 0;
									}
									labels.push_back(predicted);
									images.push_back(gray);
									model->update(images, labels);
									model->setLabelInfo(predicted, face_name);
								
								}
							}

							int predicted;
							if (!model->empty()) {
								predicted = model->predict(gray);
								if (predicted != -1) {
									String label_info = model->getLabelInfo(predicted);
									cvui::printf(frame, (int)center.x, (int)center.y + radius + 10, 0.7, 0x0000ff, "%s", label_info.c_str());
								}
							}

							if (0.75 < aspect_ratio && aspect_ratio < 1.3)
							{
								circle(frame, center, radius, face_color, 3, 8, 0);
							}
							else {
								rectangle(frame, found_face, face_color, 3, 8, 0);
							}
						}

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
				boolean fileExist = false;
				ifstream ifs("face.xml");
				if (ifs.is_open()) {
					fileExist = true;
				}
				ifs.close();

				if (fileExist) {//if there is already a face.xml file that contains remembered faces
					model = Algorithm::load<LBPHFaceRecognizer>("face.xml"); //load that file
				}
				else { //if there aren't any previous face.xml files
					model = LBPHFaceRecognizer::create();//create one
				}
				model->setThreshold(50.0);
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
