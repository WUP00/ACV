#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <vector>
#include <opencv2/highgui.hpp>
#include <cmath>
#include <opencv2\opencv.hpp>
#include<ctime>
#include "opencv2/imgproc/types_c.h"
#include "HW2.h"

using namespace cv;
using namespace std;
using std::cout; using std::endl;
using std::vector; using std::copy;

double calculateDistance(std::pair<int, int>& x, std::pair<int, int>& y)
{
	return sqrt(pow(x.first - y.first, 2) +
		pow(x.second - y.second, 2));
}


RNG rng(12345);

void ccl_demo(Mat& src, Mat& out) {
	clock_t Begin3, End3, Begin4, End4, Begin5, End5;
	Mat labels = Mat::zeros(src.size(), CV_8UC3);
	Mat len_in, centroid;
	Begin3 = clock();
	int number_label = connectedComponentsWithStats(src, labels, len_in, centroid, 8, CV_32S, CCL_DEFAULT);
	End3 = clock();
	cout << "connected component: " << End3 - Begin3 << endl;

	vector<Vec3b> Color(number_label);
	vector<Vec3b> colors = { Vec3b(0, 255, 0), Vec3b(255, 0, 0) , Vec3b(0, 0, 255) };
	for (int i = 1; i < number_label; i++) {
		Color[i] = Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
	}

	Mat canvas = Mat::zeros(src.size(), CV_8UC3);

	out.copyTo(canvas);
	for (int i = 1; i < number_label; i++) {
		int cx = centroid.at<double>(i, 0);
		int cy = centroid.at<double>(i, 1);

		int lx = len_in.at<int>(i, CC_STAT_LEFT);
		int ly = len_in.at<int>(i, CC_STAT_TOP);
		int width = len_in.at<int>(i, CC_STAT_WIDTH);
		int height = len_in.at<int>(i, CC_STAT_HEIGHT);
		int area = len_in.at<int>(i, CC_STAT_AREA);
		Begin4 = clock();
		circle(canvas, Point(cx, cy), 5, colors[i-1], -1);
		Rect box(lx, ly, width, height);
		rectangle(canvas, box, colors[i-1], 3, 8, 0);
	
		Begin5 = clock();
		//cout <<"CC_STAT_LEFT:" << lx << endl; //左上角像素x位置
		//cout << "CC_STAT_TOP:" << ly << endl; //左上角像素y位置
		//cout << "CC_STAT_WIDTH:" << width << endl; //外接矩形寬度
		//cout << "CC_STAT_HEIGHT:" << height << endl; //外接矩形高度
		cout << "CC_STAT_AREA:" << area << endl; //連通組件面積
		//cout << number_label << endl;
		
		vector<std::pair<int, int>> vec ={ {lx, ly},{lx+ width, ly+ height}, };

		cout << "Distance between points (" << vec[0].first << ", "
			<< vec[0].second << ") and (" << vec[1].first << ", "
			<< vec[1].second << ") is " << calculateDistance(vec[0], vec[1]) << endl;

		
	}
	End5 = clock();
	cout << "property analysis: " << End5 - Begin5 << endl;
	End4 = clock();
	cout << "drawing: " << End4 - Begin4 << endl;


	imshow("HW2opencv_3", canvas);
	imwrite("HW2opencv_3.bmp", canvas);
	waitKey(0);

}
int openCV()
{
	clock_t  Begin1, End1, Begin2, End2;
	Mat BitImage = imread("./input_img/orchid_island.bmp");
	Mat blur1,gray,threshold1, threshold2, out, out1, last, last1, last2, last3;
	imshow("origin", BitImage);
	moveWindow("origin", 0, 0);
	
	cvtColor(BitImage, gray, CV_BGR2GRAY);
	blur(gray, blur1, Size(3, 3), Point(-1, -1));
	Begin1 = clock();
	threshold(gray, threshold1, 0, 255, THRESH_OTSU);
	End1 = clock();
	cout << "binarizing: " << End1 - Begin1 << endl;
	imshow("HW2opencv_1", threshold1);
	moveWindow("HW2opencv_1", 250, 0);
	waitKey(0);
	imwrite("HW2opencv_1.bmp", threshold1);

	Begin2 = clock();
	morphologyEx(threshold1, out, MORPH_OPEN, getStructuringElement(0, Size(3, 3)), Point(-1, -1), 1);
	morphologyEx(out, out1, MORPH_DILATE, getStructuringElement(0, Size(7, 7)), Point(-1, -1), 2);
	End2 = clock();
	cout << "morphology: " << End2 - Begin2 << endl;
	imshow("HW2opencv_2", out1);
	waitKey(0);
	
	imwrite("HW2opencv_2.bmp", out1);


	bitwise_not(out1, last3);
	ccl_demo(last3, BitImage);
	
	destroyAllWindows();

	return 0;
}