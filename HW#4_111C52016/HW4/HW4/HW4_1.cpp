#include "HW4.h"

int HW4_1()
{
	Mat findMANGOGO01 = imread(".\\input_img\\findmango1.jpg");
	Mat MANGOGO01 = imread(".\\input_img\\mangogo1.jpg");
	Mat findMANGOGO02 = imread(".\\input_img\\findmango2.jpg");
	Mat MANGOGO02 = imread(".\\input_img\\mangogo2.jpg");

	int choose;
	cout << "choose which picture want to do " << endl << "1.findmango1 \n" << "2.findmango2" << endl;
	cin >> choose;
	switch (choose) {
	case 1: {
		int width = findMANGOGO01.cols - MANGOGO01.cols + 1;
		int height = findMANGOGO01.rows - MANGOGO01.rows + 1;
		Mat result(height, width, CV_32FC1);
		Mat dst = findMANGOGO01.clone();
		matchTemplate(findMANGOGO01, MANGOGO01, result, cv::TM_CCOEFF_NORMED);
		imshow("result", result);
		normalize(result, result, 0, 1, NORM_MINMAX, -1);
		double minValue, maxValue;
		Point minLoc, maxLoc;
		minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);
		cout << "minValue=" << minValue << endl;
		cout << "maxValue=" << maxValue << endl;
		rectangle(dst, maxLoc, Point(maxLoc.x + MANGOGO01.cols, maxLoc.y + MANGOGO01.rows), Scalar(0, 255, 0), 2, 8);
		imshow("dst", dst);
		imwrite("HW4_1_1.png", dst);
		moveWindow("dst",0,0);
		waitKey(0);
		destroyAllWindows();
		break;
	}
	case 2: {
		int width = findMANGOGO02.cols - MANGOGO02.cols + 1;
		int height = findMANGOGO02.rows - MANGOGO02.rows + 1;
		Mat result(height, width, CV_32FC1);
		Mat dst = findMANGOGO02.clone();
		matchTemplate(findMANGOGO02, MANGOGO02, result, cv::TM_CCOEFF_NORMED);
		imshow("result", result);
		normalize(result, result, 0, 1, NORM_MINMAX, -1);
		double minValue, maxValue;
		Point minLoc, maxLoc;
		minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);
		cout << "minValue=" << minValue << endl;
		cout << "maxValue=" << maxValue << endl;
		rectangle(dst, maxLoc, Point(maxLoc.x + MANGOGO02.cols, maxLoc.y + MANGOGO02.rows), Scalar(0, 255, 0), 2, 8);
		imshow("dst", dst);
		imwrite("HW4_1_2.png", dst);
		waitKey(0);
		destroyAllWindows();
		break;
	}
		  return 0;
	}
}