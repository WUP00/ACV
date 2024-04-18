#include "HW3.h"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>


#define MAXPIX 384*260

typedef struct
{
	Point2f left_top;
	Point2f left_bottom;
	Point2f right_top;
	Point2f right_bottom;
}four_corners_t;

four_corners_t corners;

void CalcCorners(const Mat& H, const Mat& src)
{
	double v2[] = { 0, 0, 1 };
	double v1[3];
	Mat V2 = Mat(3, 1, CV_64FC1, v2);
	Mat V1 = Mat(3, 1, CV_64FC1, v1);

	V1 = H * V2;
	cout << "V2: " << V2 << endl;
	cout << "V1: " << V1 << endl;
	corners.left_top.x = v1[0] / v1[2];
	corners.left_top.y = v1[1] / v1[2];

	v2[0] = 0;
	v2[1] = src.rows;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);
	V1 = Mat(3, 1, CV_64FC1, v1);
	V1 = H * V2;
	corners.left_bottom.x = v1[0] / v1[2];
	corners.left_bottom.y = v1[1] / v1[2];

	v2[0] = src.cols;
	v2[1] = 0;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);
	V1 = Mat(3, 1, CV_64FC1, v1);
	V1 = H * V2;
	corners.right_top.x = v1[0] / v1[2];
	corners.right_top.y = v1[1] / v1[2];

	v2[0] = src.cols;
	v2[1] = src.rows;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);
	V1 = Mat(3, 1, CV_64FC1, v1);
	V1 = H * V2;
	corners.right_bottom.x = v1[0] / v1[2];
	corners.right_bottom.y = v1[1] / v1[2];

}

int HW3_3()
{
	char left_input_path[] = ".\\input_img\\left.bmp";
	char right_input_path[] = ".\\input_img\\right.bmp";

	Mat mat_left = imread(left_input_path); 
	Mat mat_right = imread(right_input_path); 

	int output_height = 480;
	int output_width = 900;
	uchar* output_img = new uchar[output_width * output_height * 3]{};

	uchar* img_left = (uchar*)mat_left.data;
	uchar* img_right = (uchar*)mat_right.data;

	vector<Mat>images;
	images.push_back(mat_left);
	images.push_back(mat_right);

	Ptr<Stitcher>stitcher = Stitcher::create();


	Mat result;
	Stitcher::Status status = stitcher->stitch(images, result);
	imshow("HW3_3", result);
	imwrite("HW3_3.bmp", result);
	cout << "file has been saved as HW3_3.bmp successful" << endl;

	cv::Ptr<cv::Feature2D> feature = cv::SIFT::create(500);
	
	Mat feature_pic1, feature_pic2;
	vector<KeyPoint> keyPoint1, keyPoint2;
	feature->detect(mat_left, keyPoint1);
	feature->detect(mat_right, keyPoint2);

	Mat descor1, descor2;
	feature->compute(mat_right, keyPoint2, descor1);
	feature->compute(mat_left, keyPoint1, descor2);

	drawKeypoints(mat_left, keyPoint1, feature_pic1, cv::Scalar(255, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(mat_right, keyPoint2, feature_pic2, cv::Scalar(255, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imwrite("right_feature.bmp", feature_pic1);
	imwrite("left_feature.bmp", feature_pic2);

	cout << "file has been saved as right_feature.bmp successful" << endl;
	cout << "file has been saved as left_feature.bmp successful" << endl;

	FlannBasedMatcher matcher;
	vector<DMatch> matches;

	matcher.match(descor1, descor2, matches);

	Mat match_mat;
	drawMatches(feature_pic2, keyPoint2, feature_pic1, keyPoint1, matches, match_mat, Scalar(255, 255, 0), Scalar::all(-1));
	imwrite("HW3_3_pair.bmp", match_mat);
	cout << "file has been saved as HW3_3_pair.bmp successful" << endl;
	imshow("HW3_3_pair", match_mat);
	moveWindow("HW3_3_pair", 0, 0);
	waitKey(0);
	destroyAllWindows();

	vector<KeyPoint> R_keypoint01, R_keypoint02;
	for (int i = 0; i < matches.size(); i++) {
		R_keypoint01.push_back(keyPoint2[matches[i].queryIdx]);
		R_keypoint02.push_back(keyPoint1[matches[i].trainIdx]);
	}

	vector<Point2f> p01, p02;
	for (int i = 0; i < matches.size(); i++) {
		p01.push_back(R_keypoint01[i].pt);
		p02.push_back(R_keypoint02[i].pt);
	}
	vector<uchar> RansacStatus;
	Mat homo_mat = findHomography(p02, p01, RansacStatus, RANSAC);
	Mat warp_mat;
	warpPerspective(mat_left, warp_mat, homo_mat, Size(output_width, output_height));
	imwrite("HW3_3_turn.bmp", warp_mat);
	cout << "file has been saved as HW3_3_turn.bmp successful" << endl;
	imshow("HW3_3turn", warp_mat);
	moveWindow("HW3_3turn", 0, 0);
	waitKey(0);
	destroyAllWindows();

	Mat stitching_mat = warp_mat.clone();
	mat_right.copyTo(stitching_mat(Rect(0, 0, mat_right.cols, mat_right.rows)));

	imshow("HW3_3_last", stitching_mat);
	cout << "file has been saved as HW3_3_last.bmp successful" << endl;
	imwrite("HW3_3_last.bmp", stitching_mat);
	moveWindow("HW3_3_last", 0, 0);
	waitKey(0);
	destroyAllWindows();
	


	

	return 0;
}