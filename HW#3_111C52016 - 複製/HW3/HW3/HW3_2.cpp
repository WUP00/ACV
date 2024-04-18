#include "HW3.h"

int HW3_2()
{
	BITMAPFILEHEADER fileheader3;
	BITMAPINFOHEADER infoheader3;
	char filename_left[30] = ".\\input_img\\right.bmp";
	char filename_right[30] = ".\\input_img\\left.bmp";

	FILE* fp3;
	fp3 = fopen(filename_left, "rb");

	fread(&fileheader3, sizeof(BITMAPFILEHEADER), 1, fp3);
	fread(&infoheader3, sizeof(BITMAPINFOHEADER), 1, fp3);
	int ImageX = infoheader3.biWidth;
	int ImageY = infoheader3.biHeight;

	int size = ImageX * ImageY;
	Mat left = imread(".\\input_img\\right.bmp");
	fclose(fp3);

	FILE* fp4;
	fp4 = fopen(filename_right, "rb");

	fread(&fileheader3, sizeof(BITMAPFILEHEADER), 1, fp4);
	fread(&infoheader3, sizeof(BITMAPINFOHEADER), 1, fp4);

	Mat right = imread(".\\input_img\\left.bmp");
	fclose(fp4);

	int width_out = 900;
	int  height_out = 480;
	int size_out = width_out * height_out;
	RGBTRIPLE* output = new RGBTRIPLE[size_out]();

	Mat outmat = Mat(height_out, width_out, CV_8UC3, output);

	for (int x = 0; x < ImageX; x++) {
		for (int y = 0; y < ImageY; y++) {
			output[x + (y + 80) * width_out].rgbtBlue = left.at<Vec3b>(y, x)[0];
			output[x + (y + 80) * width_out].rgbtGreen = left.at<Vec3b>(y, x)[1];
			output[x + (y + 80) * width_out].rgbtRed = left.at<Vec3b>(y, x)[2];
		}
	}
	int x0 = 130, y0 = 250, x1 = 470, y1 = 310, x2 = 475, y2 = 900, x3 = 0, y3 = 770;
	int dx1 = x1 - x2, dx2 = x3 - x2, all_x = x0 - x1 + x2 - x3;
	int dy1 = y1 - y2, dy2 = y3 - y2, all_y = y0 - y1 + y2 - y3;
	double g = (double)(((all_x * dy2 * 1.0) - (dx2 * all_y * 1.0)) / ((dx1 * dy2 * 1.0) - (dx2 * dy1 * 1.0)));
	double h = (double)(((dx1 * all_y * 1.0) - (all_x * dy1 * 1.0)) / ((dx1 * dy2 * 1.0) - (dx2 * dy1 * 1.0)));
	double a = (double)(x1 - x0 + g * x1 * 1.0);
	double b = (double)(x3 - x0 + h * x3 * 1.0);
	double c = (double)x0;
	double d = (double)(y1 - y0 + g * y1 * 1.0);
	double e = (double)(y3 - y0 + h * y3 * 1.0);
	double f = (double)y0;
	double i = (double)1.0;
	double A = e * i - f * h;
	double B = c * h - b * i;
	double C = b * f - c * e;
	double D = f * g - d * i;
	double E = a * i - c * g;
	double F = c * d - a * f;
	double G = d * h - e * g;
	double H = b * g - a * h;
	double I = a * e - b * d;
	for (int x = 0; x < height_out; x++)
	{
		for (int y = 0; y < width_out; y++)
		{
			int u = (A * x + B * y + C) * ImageY / (G * x + H * y + I);
			int v = (D * x + E * y + F) * ImageX/ (G * x + H * y + I);
			if (u >= 0 && u < right.rows && v >= 0 && v < right.cols)
			{
				output[x * 900 + y].rgbtBlue = right.at<Vec3b>(u, v)[0];
				output[x * 900 + y].rgbtGreen = right.at<Vec3b>(u, v)[1];
				output[x * 900 + y].rgbtRed = right.at<Vec3b>(u, v)[2];
			}
		}
	}
	
	imwrite("HW3_2.bmp", outmat);
	imshow("HW3_2", outmat);
	cout << "file has been saved as HW3_2.bmp successful" << endl;
	waitKey(0);
	destroyAllWindows();


	return 0;
}

