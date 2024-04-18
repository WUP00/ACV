#include "HW3.h"
#define MAXPIX 384*260

#define origin_img_360 370
#define width 510
#define style_origin_img_360 750
#define style_width 563
#define hist_size 256

void Histogram(unsigned char* image, int* hist, int h, int w) {
	for (int i = 0; i < 256; i++) {
		hist[i] = 0;
	}
	for (int i = 0; i < h * w; i++) {
		for (int k = 0; k < 256; k++) {
			if (image[i] == k)
				hist[k]++;
		}
	}
}
void hist_match(unsigned char* image1, unsigned char* image2, int* hist1, int* hist2, int* hist_match, unsigned char* eq, int h1, int w1, int h2, int w2) {


	Histogram(image2, hist2, h2, w2);
	double* cdf = new double[256];
	for (int i = 0; i < 256; i++) {
		cdf[i] = (double)hist2[i] / (h2 * w2);
		if (i != 0)
			cdf[i] = cdf[i - 1] + cdf[i];
		hist_match[i] = cdf[i] * 255;
	}
	for (int i = 0; i < h1 * w1; i++) {
		int a = 0;
		a = image1[i];
		eq[i] = hist_match[a];
	}
	for (int i = 0; i < 256; i++) {
		hist_match[i] = 0;
	}
	for (int i = 0; i < h1 * w1; i++) {
		for (int k = 0; k < 256; k++) {
			if (eq[i] == k)
				hist_match[k]++;
		}
	}

	for (int i = 255; i > 0; i--) {
		hist_match[i] = hist_match[i] - hist_match[i - 1];
	}
}
double* cumulative_distribution(double*& prob)
{
	//double* cdf = (double*)malloc(sizeof(int) * hist_size);
	double* cdf = new double[hist_size];
	cdf[0] = prob[0];
	for (int i = 1; i < hist_size; i++)
	{
		cdf[i] = cdf[i - 1] + prob[i];
	}
	return cdf;
}

int* calculate_histogram(uchar* src, int ori_img_360, int width1)
{
	int* hist = (int*)malloc(sizeof(int) * hist_size);
	memset(hist, 0, sizeof(int) * hist_size);          
	for (int row = 0; row < ori_img_360; row++)
	{
		for (int col = 0; col < width1; col++)
		{
			hist[*(src + row * width1 + col)]++;
		}
	}
	return hist;
}

void histogram_matching(uchar* src, uchar* temp, uchar* dst, int src_ori_img_360, int src_width, int temp_ori_img_360, int temp_width)
{
	int* histogram_src = calculate_histogram(src, src_ori_img_360, src_width);
	int* histogram_temp = calculate_histogram(temp, temp_ori_img_360, temp_width);

	double size_src = src_ori_img_360 * src_width;
	double size_temp = temp_ori_img_360 * temp_width;
	double* prob_src = new double[256];
	double* prob_temp = new double[256];
	double* mapping_table = new double[256];

	for (int i = 0; i < 256; i++)
	{
		prob_src[i] = histogram_src[i] / size_src;
		prob_temp[i] = histogram_temp[i] / size_temp;
		mapping_table[i] = 0;
	}

	double* cdf_src = cumulative_distribution(prob_src);
	double* cdf_temp = cumulative_distribution(prob_temp);

	cdf_src[255] = cdf_temp[255] = 1;

	int current_id = 0;
	for (int i = 0; i < 256; i++)
	{
		bool mapped = false;
		while (mapped == false)
		{
			if (cdf_src[i] <= cdf_temp[current_id])
			{
				mapping_table[i] = current_id;
				mapped = true;
			}
			else
				current_id++;
		}
	}

	for (int i = 0; i < src_ori_img_360; i++)
	{
		for (int j = 0; j < src_width; j++)
		{
			uchar gray_value = src[i * src_width + j];
			int val = mapping_table[gray_value];
			dst[i * src_width + j] = val;
		}
	}
}
int Bonus()
{
	Mat moutainmat = imread(".\\HW3_2.bmp");
	Mat stylemat = imread(".\\input_img\\style.bmp");

	imshow("originpic", moutainmat);
	waitKey(0);
	imshow("originstyle", stylemat);
	waitKey(0);

	char left_input_path[] = ".\\input_img\\right.bmp";
	char right_input_path[] = ".\\input_img\\left.bmp";
	char style_input_path[] = ".\\input_img\\style.bmp";

	Mat mat_left = imread(left_input_path); 
	Mat mat_right = imread(right_input_path); 
	Mat mat_style = imread(style_input_path);

	int output_ori_img_360 = 480;
	int output_width = 900;
	uchar* output_img = new uchar[output_width * output_ori_img_360 * 3];

	for (int x = 0; x < 480; x++) {
		for (int y = 0; y < 900; y++) {
			output_img[(x + y * 480) * 3] = 0;
			output_img[(x + y * 480) * 3 + 1] = 0;
			output_img[(x + y * 480) * 3 + 2] = 0;
		}
	}

	uchar* img_left = (uchar*)mat_left.data;
	uchar* img_right = (uchar*)mat_right.data;
	uchar* img_style = (uchar*)mat_style.data;

	uchar* img_left_r = new uchar[origin_img_360 * width]{};
	uchar* img_left_g = new uchar[origin_img_360 * width]{};
	uchar* img_left_b = new uchar[origin_img_360 * width]{};

	uchar* img_right_r = new uchar[origin_img_360 * width]{};
	uchar* img_right_g = new uchar[origin_img_360 * width]{};
	uchar* img_right_b = new uchar[origin_img_360 * width]{};

	uchar* img_style_r = new uchar[style_origin_img_360 * style_width]{};
	uchar* img_style_g = new uchar[style_origin_img_360 * style_width]{};
	uchar* img_style_b = new uchar[style_origin_img_360 * style_width]{};

	uchar* img_left_matching_r = new uchar[origin_img_360 * width]{};
	uchar* img_left_matching_g = new uchar[origin_img_360 * width]{};
	uchar* img_left_matching_b = new uchar[origin_img_360 * width]{};

	uchar* img_right_matching_r = new uchar[origin_img_360 * width]{};
	uchar* img_right_matching_g = new uchar[origin_img_360 * width]{};
	uchar* img_right_matching_b = new uchar[origin_img_360 * width]{};

	uchar* left_matching = new uchar[origin_img_360 * width * 3]{};
	uchar* right_matching = new uchar[origin_img_360 * width * 3]{};


	for (int i = 0; i < origin_img_360; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int pixel = (i * width + j);
			img_left_b[pixel] = img_left[pixel * 3];
			img_left_g[pixel] = img_left[pixel * 3 + 1];
			img_left_r[pixel] = img_left[pixel * 3 + 2];
		}
	}

	for (int i = 0; i < origin_img_360; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int pixel = (i * width + j);
			img_right_b[pixel] = img_right[pixel * 3];
			img_right_g[pixel] = img_right[pixel * 3 + 1];
			img_right_r[pixel] = img_right[pixel * 3 + 2];
		}
	}

	for (int i = 0; i < style_origin_img_360; i++)
	{
		for (int j = 0; j < style_width; j++)
		{
			int pixel = (i * style_width + j);
			img_style_b[pixel] = img_style[pixel * 3];
			img_style_g[pixel] = img_style[pixel * 3 + 1];
			img_style_r[pixel] = img_style[pixel * 3 + 2];
		}
	}

	histogram_matching(img_left_b, img_style_b, img_left_matching_b, origin_img_360, width, style_origin_img_360, style_width);
	histogram_matching(img_left_g, img_style_g, img_left_matching_g, origin_img_360, width, style_origin_img_360, style_width);
	histogram_matching(img_left_r, img_style_r, img_left_matching_r, origin_img_360, width, style_origin_img_360, style_width);

	histogram_matching(img_right_b, img_style_b, img_right_matching_b, origin_img_360, width, style_origin_img_360, style_width);
	histogram_matching(img_right_g, img_style_g, img_right_matching_g, origin_img_360, width, style_origin_img_360, style_width);
	histogram_matching(img_right_r, img_style_r, img_right_matching_r, origin_img_360, width, style_origin_img_360, style_width);

	for (int i = 0; i < origin_img_360; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int pixel = (i * width + j);
			left_matching[pixel * 3] = img_left_matching_b[pixel];
			left_matching[pixel * 3 + 1] = img_left_matching_g[pixel];
			left_matching[pixel * 3 + 2] = img_left_matching_r[pixel];
		}
	}

	for (int i = 0; i < origin_img_360; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int pixel = (i * width + j);
			right_matching[pixel * 3] = img_right_matching_b[pixel];
			right_matching[pixel * 3 + 1] = img_right_matching_g[pixel];
			right_matching[pixel * 3 + 2] = img_right_matching_r[pixel];
		}
	}


	int x0 = 130, y0 = 250, x1 = 470, y1 = 310, x2 = 475, y2 = 900, x3 = 0, y3 = 770;

	int dx1 = x1 - x2, dx2 = x3 - x2, all_x = x0 - x1 + x2 - x3;
	int dy1 = y1 - y2, dy2 = y3 - y2, all_y = y0 - y1 + y2 - y3;


	double a = 0.5913980757;
	double b = 0.1151161892;
	double c = 250;

	double d = -0.2549019608;
	double e = 0.8475911534;
	double f = 130;

	double g = -0.0005561166;
	double h = -0.0001517612;
	double i = 1;

	double a11 = e * i - h * f;
	double a12 = d * i - g * f;
	double a13 = d * h - g * e;
	double a21 = b * i - h * c;
	double a22 = a * i - g * c;
	double a23 = a * h - g * b;
	double a31 = b * f - e * c;
	double a32 = a * f - d * c;
	double a33 = a * e - d * b;


	double A = e * i - f * h;
	double B = c * h - b * i;
	double C = b * f - c * e;
	double D = f * g - d * i;
	double E = a * i - c * g;
	double F = c * d - a * f;
	double G = d * h - e * g;
	double H = b * g - a * h;
	double I = a * e - b * d;

	double left_coor_x = 0.0;
	double left_coor_y = 0.0;
	double left_coor = 0.0;

	Mat result(output_ori_img_360, output_width, CV_8UC3);


	float* result1 = (float*)result.data;

	Mat mat_left_matching(origin_img_360, width, CV_8UC3, left_matching);
	Mat mat_right_matching(origin_img_360, width, CV_8UC3, right_matching);

	float* float_left = (float*)mat_left_matching.data;
	//float* float_right = (float*)mat_right_matching.data;

	for (int i = 0; i < 370; i++)
	{
		for (int j = 0; j < 510; j++)
		{
			output_img[(((i + 80) * 900 + j) * 3)] = left_matching[(i * 510 + j) * 3];
			output_img[(((i + 80) * 900 + j) * 3) + 1] = left_matching[((i * 510 + j) * 3) + 1];
			output_img[(((i + 80) * 900 + j) * 3) + 2] = left_matching[((i * 510 + j) * 3) + 2];
		}
	}


	for (int i = 0; i < output_ori_img_360; i++)
	{
		for (int j = 0; j < output_width; j++)
		{

			left_coor_x = A * j + B * i + C;
			left_coor_y = D * j + E * i + F;
			left_coor = G * j + H * i + I;

			left_coor_x = left_coor_x / left_coor;
			left_coor_y = left_coor_y / left_coor;

			if (left_coor_y < 370 && left_coor_y > 0 && left_coor_x < 510 && left_coor_x>0) {
				output_img[(i * output_width + j) * 3] = right_matching[(((int)left_coor_x + 510 * (int)left_coor_y) * 3)];
				output_img[((i * output_width + j) * 3) + 1] = right_matching[(((int)left_coor_x + 510 * (int)left_coor_y) * 3) + 1];
				output_img[((i * output_width + j) * 3) + 2] = right_matching[(((int)left_coor_x + 510 * (int)left_coor_y) * 3) + 2];
			}

		}
	}
	Mat result_mat(output_ori_img_360, output_width, CV_8UC3, output_img);





	imshow( "HW3_bouns", result_mat);
	moveWindow("HW3_bouns", 0, 0);
	waitKey(0);
	destroyAllWindows();

	imwrite("HW3_Bonus.bmp", result_mat);
	cout << "file has been saved as HW3_Bonus.bmp successful" << endl;
	return 0;
}
