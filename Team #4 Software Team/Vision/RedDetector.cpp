//RedDetector.cpp
//A basic program that attempts to detect a red object from a webcame video stream
//by Kevin Worsley
//for NGCP UUV 2017/18

#include "stdafx.h""
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv) {
	VideoCapture cap(1); //open webcam source

	if (cap.isOpened() == false) {
		cout << "Webcam failed to initialize" << endl;
		return -1;
	}

	string window_name = "UUV Feed";

	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	//program works by searching for specific hue, saturation, and value
	//these values are configured for red, will need to be redefined for color changes
	//TODO: create some sort of algorithm to figure out color shifts
	int iLowH = 170;
	int iHighH = 179;

	int iLowS = 150;
	int iHighS = 255;

	int iLowV = 60;
	int iHighV = 255;

	int iLastX = -1;
	int iLastY = -1;

	//temporary image from the camera stolen to do image work
	Mat imgTmp;
	cap.read(imgTmp);
	//rotate(imgTmp, imgTmp, ROTATE_180);

	Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);

	while (true) {
		Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); //grab a new frame from video

		if (bSuccess == false) {
			cout << "Capture source cannot be read from" << endl;
			break;
		}

		Mat imgHSV;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

		Mat imgThresholded;

		//thresholds the image
		//thresholding is essentially comparing values in the first image to the limits of the detected image, and only highlighting what we want
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);

		//create a morphological opening
		//thresholding isn't perfect, so a morphological opening / closing clean up lots of noise
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//now the morphological close
		//complete the morphological process
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//calculates the moments of the cleaned up image
		//moments are a measure of the density of certain pixels, and can lead us to being able to determine points
		//tbh i'm not super sure how this works but it does so lol
		Moments oMoments = moments(imgThresholded);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		if (dArea > 10000) {
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;

			//this if case determines if the center of the image is onscreen
			if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0) {
				line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0, 0, 255), 2);
				//display circle at center point
				//circle(imgLines, Point(posX, posY), 4, Scalar(255, 0, 0), -1, 8);
				//circle(imgLines, Point(posX, posY), 4, Scalar(0, 0, 0), -1, 8);
			}

			iLastX = posX;
			iLastY = posY;
		}

		imshow("Thresholded image", imgThresholded);

		imgOriginal = imgOriginal + imgLines;
		imshow("original", imgOriginal);

		if (waitKey(30) == 27) {
			cout << "Escape key pressed. Exiting" << endl;
			break;
		}
	}

	return 0;
}