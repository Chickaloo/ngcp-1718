//HelloWorld.cpp
//Basic file to get OpenCV up and compiling properly
//by Kevin Worsley, following tutorial from Shermal Hermando
//for NGCP UUV 2017/18

#include "stdafx.h"
#include <opencv2/opencv.hpp> //necessary library for opencv

#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	Mat image = imread("C:/Users/Kevin Worsley/Desktop/CPP/UUV/test.jpg"); //Check a look at the image file stored locally on the PC

	//if statement checks and halts if image screws up
	if (image.empty()) {
		cout << "Image failed to open. Could not find or image unopenable" << endl;
		system("pause"); //wait for keypress
		return -1;
	}

	String windowName = "Test Image Window"; //Names the window

	namedWindow(windowName); //open a window

	imshow(windowName, image); //show our image in the created window

	moveWindow(windowName, 20, 20);

	waitKey(0); //wait for any keypress in the window

	destroyWindow(windowName); //kills the window on press

	return 0;
}

