#include "HW3.h"
#define M_PI 3.14159265358979323846
int HW3_1()
{
	
    Mat ori_img_360 = imread(".\\input_img\\360degree.bmp");

    unsigned char* omnidirectional = (unsigned char*)ori_img_360.data;

    unsigned char* new_original_img_360 = new unsigned char[720 * 384 * 3];

    double R = (2 * M_PI) / 720; 
    float the;
    int x_c = 512, y_c = 384;

    for (int x = 0; x < 720; x++) {
        for (int y = 0; y < 384; y++) {

            the = (double)x * R;  
            float x_1 = (float)y * cos(the) + x_c;
            float y_1 = (float)y * sin(the) + y_c;
            new_original_img_360[(((384 - 1 - y) * 720 + (720 - 1 - x)) * 3) + 0] = omnidirectional[(((int)x_1 + (int)y_1 * 1024) * 3) + 0];
            new_original_img_360[(((384 - 1 - y) * 720 + (720 - 1 - x)) * 3) + 1] = omnidirectional[(((int)x_1 + (int)y_1 * 1024) * 3) + 1];
            new_original_img_360[(((384 - 1 - y) * 720 + (720 - 1 - x)) * 3) + 2] = omnidirectional[(((int)x_1 + (int)y_1 * 1024) * 3) + 2];

        }
    }

    Mat newmat = Mat(384, 720, CV_8UC3, new_original_img_360);
    imshow("HW3_1", newmat);
    imwrite("HW3_1.bmp", newmat);
    cout << "file has been saved as HW3_1.bmp successful" << endl;
    waitKey(0);
    destroyAllWindows();
	return 0;

}