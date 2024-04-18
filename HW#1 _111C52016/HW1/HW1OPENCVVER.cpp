#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <opencv2\opencv.hpp>
#include <windows.h>


using namespace std;
using namespace cv;

int img_width = 512;
int img_height = 512;
int size = img_width * img_height;
uchar* rotate4 = new uchar[img_width * img_height];
RGBTRIPLE* rotate = new RGBTRIPLE[img_width * img_height];



int HW1OPENCV()
{
	int choose;
	cout << "choose which process want to do " << endl << "1.Write(.bmp)" << endl << "2.Generate a color negative image." << endl << "3.Split and reassemble into new image." << endl << "4.Zoom Lena 512*512 to 1024*1024. " << endl << "5.Shrink Lena 512*512 to 128*128. " << endl << "6.Zoom color negative image 512*512 to 1024*1024. " << endl << "7.Shrink split and reassemble image from 512*512 to 256*256. " << endl << "8.Exit" << endl;
	cin >> choose;
	Mat BitImage = imread("./input_img/InputImage1.bmp");
	Mat negative, rotate1, rotate2;
	Mat rotate(BitImage.rows, BitImage.cols, CV_8UC3);
	


	switch (choose) {



	case 1:
		imshow("HW1opencv_1", BitImage);
		waitKey(0);
		imwrite("HW1opencv_1.bmp", BitImage);
		cout << "file has been saved as HW1opencv_1.bmp successful" << endl;
		break;
		
	case 2: {
		bitwise_not(BitImage, negative);
		imshow("HW1opencv_2", negative);
		waitKey(0);
		destroyWindow("HW1opencv");
		imwrite("HW1opencv_2.bmp", negative);
		cout << "file has been saved as HW1opencv_2.bmp successful" << endl;
		break;
	}
	case 3: {
		cv::rotate(BitImage, rotate1, cv::ROTATE_90_CLOCKWISE);
		cv::rotate(BitImage, rotate2, cv::ROTATE_90_COUNTERCLOCKWISE);

		int row = BitImage.rows;
		int col = BitImage.cols;


		for (int i = 0; i < col; i++)
		{
			for (int j = 0; j < row; j++)
			{

				if (i < j && i>511 - j || i > j && i < 511 - j)
					rotate.at<Vec3b>(i, j) = rotate1.at<Vec3b>(i, j);

				else
					rotate.at<Vec3b>(i, j) = rotate2.at<Vec3b>(i, j);

			}
		}

		imshow("HW1opencv1", rotate);
		imwrite("HW1opencv_3.bmp", rotate);
		waitKey(0);
		cout << "file has been saved as HW1opencv_3.bmp successful" << endl;
		break; }
	case 4: {
		resize(BitImage, rotate, cv::Size(BitImage.cols * 2, BitImage.rows * 2), 0, 0, cv::INTER_LINEAR);
		imshow("HW1opencv_bonus1", rotate);
		waitKey(0);
		cout << "file has been saved as HW1opencv_bonus1.bmp successful" << endl;
		imwrite("HW1opencv_bonus1.bmp", rotate);
		break;
	}
	case 5: {
		resize(BitImage, rotate, cv::Size(BitImage.cols * 0.5, BitImage.rows * 0.5), 0, 0, cv::INTER_LINEAR);
		imshow("HW1opencv_bonus2", rotate);
		waitKey(0);
		imwrite("HW1opencv_bonus2.bmp", rotate);
		cout << "file has been saved as HW1opencv_bonus2.bmp successful" << endl;
		break;
	}
	case 6: {
		bitwise_not(BitImage, negative);
		resize(negative, rotate, cv::Size(BitImage.cols * 2, BitImage.rows * 2), 0, 0, cv::INTER_LINEAR);
		imshow("HW1opencv_bonus3", rotate);
		waitKey(0);
		cout << "file has been saved as HW1opencv_bonus3.bmp successful" << endl;
		imwrite("HW1opencv_bonus3.bmp", rotate);
		break;
	}
	case 7: {
		cv::rotate(BitImage, rotate1, cv::ROTATE_90_CLOCKWISE);
		cv::rotate(BitImage, rotate2, cv::ROTATE_90_COUNTERCLOCKWISE);

		int row = BitImage.rows;
		int col = BitImage.cols;


		for (int i = 0; i < col; i++)
		{
			for (int j = 0; j < row; j++)
			{

				if (i < j && i>511 - j || i > j && i < 511 - j)
					rotate.at<Vec3b>(i, j) = rotate1.at<Vec3b>(i, j);

				else
					rotate.at<Vec3b>(i, j) = rotate2.at<Vec3b>(i, j);

			}
		}
		resize(rotate, rotate, cv::Size(BitImage.cols * 0.5, BitImage.rows * 0.5), 0, 0, cv::INTER_LINEAR);
		imshow("HW1opencv_bonus4", rotate);
		waitKey(0);
		imwrite("HW1opencv_bonus4.bmp", rotate);
		cout << "file has been saved as HW1opencv_bonus4.bmp successful" << endl;
		break;
	}
		}
	return 0;
}