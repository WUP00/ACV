#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <opencv2/highgui.hpp>
#include <opencv2\opencv.hpp>
#include <conio.h>
#include <math.h>
#include<ctime>
#include "opencv2/imgproc/types_c.h"
#include "HW2.h"
#include <stack>
#include<conio.h>

using namespace std;
using namespace cv;



void savebmp(char filename[], RGBTRIPLE* img, BITMAPFILEHEADER FileHeader, BITMAPINFOHEADER InfoHeader, int ImageX, int ImageY) {
    FILE* fpw;
    RGBTRIPLE rgb;
    fpw = fopen(filename, "wb");
    if (!fpw) {
        cout << "file save fail!" << endl;
    }
    fwrite(&FileHeader, sizeof(BITMAPFILEHEADER), 1, fpw);
    fwrite(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, fpw);

    for (int i = 0; i < ImageY; i++)
    {
        for (int j = 0; j < ImageX; j++)
        {
            rgb = img[i * ImageX + j];
            fwrite(&rgb, sizeof(RGBTRIPLE), 1, fpw);
        }
    }
    fclose(fpw);

}
void dilateTest(uchar* imageBuffer, uchar* outBuffer, int imageWidth, int imageHeight, int x)
{
    uchar* dilateBuffer = (uchar*)malloc((imageWidth + 1) * (1 + imageHeight));
    memset(dilateBuffer, 0, (imageHeight + 1) * (imageWidth + 1));

    for (int i = 0; i < imageHeight; i++)
    {
        for (int j = 0; j < imageWidth; j++)
        {
            dilateBuffer[i * (imageWidth + 1) + j + 1] = imageBuffer[i * imageWidth + j];
        }
    }

    uchar* srcImage = dilateBuffer;

    for (int i = 0; i < imageWidth; i++)
    {
        for (int j = 0; j < imageHeight; j++)
        {
            uchar tempNum = 0;

            srcImage = (dilateBuffer + (i * (imageWidth + 1) + j));
            for (int m = 0; m < x; m++)
            {
                for (int n = 0; n < x; n++)
                {


                    if (tempNum < srcImage[n])
                    {
                        tempNum = srcImage[n];
                    }
                }
                srcImage = (srcImage + m * (imageWidth + 1));
            }

            outBuffer[i * imageWidth + j] = tempNum;
        }
    }
}
double centroidY(unsigned char* newimg, int h, int w, int label, int area) {
    int sumY = 0;
    int sum = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (newimg[i * w + j] == label) {
                sumY = sumY + i;
                sum = sum + 1;
            }
        }
    }
    return (double)sumY / sum;
}
double centroidX(unsigned char* newimg, int h, int w, int label, int area) {
    int sumX = 0, sum = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (newimg[i * w + j] == label) {
                sumX = sumX + j;
                sum++;
            }
        }
    }
    return (double)sumX / sum;
}
int area(unsigned char* newimg, int h, int w, int label) {
    int sum = 0;
    for (int i = 0; i < h * w; i++) {
        if (newimg[i] == label)
            sum++;
    }
    return sum;
}
int detect(unsigned char* img, unsigned char* newimg, int h, int w) {
 
    int labelValue = 1;
    int seed, neighbor;
    stack<int> intStack;    
    int area = 0;
    if (!intStack.empty()) intStack.pop();    
    memcpy(newimg, img, sizeof(uchar) * h * w);
    Mat blurshrinklenaMat = Mat(w, h, CV_8UC1, newimg);
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (newimg[i * w + j] == 0)
            {
                area = 0;
                labelValue++;
                int k = i;
                int l = j;
                seed = i * w + j;     
                newimg[i * w + j] = labelValue;
                intStack.push(i);
                intStack.push(j);
                while (!intStack.empty())
                {
                    neighbor = (i + 1) * w + j;
                    if ((((i + 1) < h) && j < w) && (newimg[neighbor] == 0))
                    {
                        newimg[neighbor] = labelValue;
                        intStack.push(i + 1);
                        intStack.push(j);
                        area++;
                    }
                    neighbor = (i - 1) * w + j;
                    if (((i != 0) && j < w) && (newimg[neighbor] == 0))
                    {
                        newimg[neighbor] = labelValue;
                        intStack.push(i - 1);
                        intStack.push(j);
                        area++;
                    }
                    neighbor = i * w + j + 1;
                    if (((i < h) && (j + 1 < w)) && (newimg[neighbor] == 0))
                    {
                        newimg[neighbor] = labelValue;
                        intStack.push(i);
                        intStack.push(j + 1);
                        area++;
                    }
                    neighbor = i * w + j - 1;
                    if ((((i < h) && (j != 0)) && newimg[neighbor] == 0))
                    {
                        newimg[neighbor] = labelValue;
                        intStack.push(i);
                        intStack.push(j - 1);
                        area++;
                    }
                    j = intStack.top();
                    intStack.pop();
                    i = intStack.top();
                    intStack.pop();
                }
             /*    cout << "label " << labelValue << " : area = " << area << endl;*/
            }
        
        }

    }
    return labelValue;
}


void block(RGBTRIPLE* img, unsigned char* newimg, int h, int w, int label) {

    int COLOR[3][3] = { { 0, 255, 0 }, {255, 0, 0}, {0, 0, 255} };

    int maxX = 0, maxY = 0, minX = 800, minY = 800;
    for (int a = 1; a < label + 1; a++) {
        maxX = 0;
        maxY = 0;
        minX = 800;
        minY = 800;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (newimg[i * w + j] == a) {
                    if (j > maxX)
                        maxX = j;
                    else if (j < minX)
                        minX = j;
                    if (i > maxY)
                        maxY = i;
                    else if (i < minY)
                        minY = i;
                }
            }
        }
        for (int i = minX; i < maxX; i++) {
            img[i + minY * w].rgbtBlue = COLOR[a%3][0];
            img[i + minY * w].rgbtGreen = COLOR[a%3][1];
            img[i + minY * w].rgbtRed = COLOR[a%3][2];
        }
        for (int i = minX; i < maxX + 1; i++) {
            img[i + maxY * w].rgbtBlue = COLOR[a%3][0];
            img[i + maxY * w].rgbtGreen = COLOR[a%3][1];
            img[i + maxY * w].rgbtRed = COLOR[a%3][2];
        }
        for (int i = minY; i < maxY; i++) {
            img[minX + i * w].rgbtBlue = COLOR[a%3][0];
            img[minX + i * w].rgbtGreen = COLOR[a%3][1];
            img[minX + i * w].rgbtRed = COLOR[a%3][2];
        }
        for (int i = minY; i < maxY; i++) {
            img[maxX + i * w].rgbtBlue = COLOR[a%3][0];
            img[maxX + i * w].rgbtGreen = COLOR[a%3][1];
            img[maxX + i * w].rgbtRed = COLOR[a%3][2];
        }




      /*   cout << a << " " << minX << " " << minY << " " << maxX << " " << maxY << endl;*/
         vector<std::pair<int, int>> vec = { {minX, minY},{minX + maxX, minY + maxY}, };

         cout << "Distance between points (" << vec[0].first << ", "
             << vec[0].second << ") and (" << vec[1].first << ", "
             << vec[1].second << ") is " << calculateDistance(vec[0], vec[1]) << endl;



    }
}


int HW2C_PLUS()
{
    clock_t  Begin1, End1, Begin2, End2, Begin3, End3, Begin4, End4, Begin5, End5;;
    BITMAPFILEHEADER FileHeader;
    BITMAPINFOHEADER InfoHeader;
    FILE* fp,* fpw,* fpdi;
    int size, ImageX, ImageY;

    fp = fopen("./input_img/orchid_island.bmp", "rb");


    if (!fp) {
        cout << "file open fail!" << endl;
        return 0;
    }

    fread(&FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    ImageX = InfoHeader.biWidth;
    ImageY = InfoHeader.biHeight;
    size = ImageX * ImageY;

    RGBTRIPLE* color = new RGBTRIPLE[size];
    RGBTRIPLE* rotate = new RGBTRIPLE[size];
    RGBTRIPLE rgb, rgbdi;

    for (int i = 0; i < ImageY; i++)
    {
        for (int j = 0; j < ImageX; j++)
        {
            fread(&rgb, sizeof(RGBTRIPLE), 1, fp);
            color[i * ImageX + j] = rgb;
        }
    }
  fclose(fp);

     Begin1 = clock();
     for (int i = 0; i < ImageY; i++) {
      for (int j = 0; j < ImageX; j++) {
          rotate[i * ImageX + j].rgbtBlue = 255 - color[i * ImageX + j].rgbtBlue;
          rotate[i * ImageX + j].rgbtGreen = 255 - color[i * ImageX + j].rgbtGreen;
          rotate[i * ImageX + j].rgbtRed = 255 - color[i * ImageX + j].rgbtRed;
      }
    }
     unsigned char* bical = new unsigned char[size];
     unsigned char* newimg = new unsigned char[size];
     for (int i = 0; i < size; i++) {
        if (((rotate[i].rgbtBlue + rotate[i].rgbtGreen + rotate[i].rgbtRed) / 3) < 137) {
            bical[i] = 255;
            rotate[i].rgbtBlue = 255;
            rotate[i].rgbtGreen = 255;
            rotate[i].rgbtRed = 255;
        }
        else {
            bical[i] = 0;
            rotate[i].rgbtBlue = 0;
            rotate[i].rgbtGreen = 0;
            rotate[i].rgbtRed = 0;
        }
    }
    End1 = clock();
    cout << "binarizing: " << End1 - Begin1 << endl;
    Mat bimat = Mat(ImageX, ImageY, CV_8UC3, rotate);

 /*   imshow("binary", bimat);*/
    moveWindow("binary", 0, 0);
    waitKey(0);

    char filename[] = "HW2_C++_1.bmp";
    fpw = fopen(filename, "wb");
    if (!fpw) {
        cout << "file save fail!" << endl;
    }
    fwrite(&FileHeader, sizeof(BITMAPFILEHEADER), 1, fpw);
    fwrite(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, fpw);

    for (int i = 0; i < ImageY; i++)
    {
        for (int j = 0; j < ImageX; j++)
        {
            rgb = rotate[i * ImageX + j];
            fwrite(&rgb, sizeof(RGBTRIPLE), 1, fpw);
        }
    }
    cout << "file has been saved as HW2_C++_1.bmp successful" << endl;
    fclose(fpw);
    Begin2 = clock();
    unsigned char* dia = new unsigned char[size];
    dilateTest(bical, dia, ImageX, ImageY, 5);
    Mat diamat = Mat(ImageX, ImageY, CV_8UC1, dia);
    //imshow("dilate", diamat);
    moveWindow("dilate", 800, 0);
    imwrite("dilate.bmp", diamat);
    waitKey(0);
    End2 = clock();
    cout <<"morphology: " << End2 - Begin2 << endl;
    RGBTRIPLE* dirgb = new RGBTRIPLE[size];

    for (int i = 0; i < ImageY; i++) {
        for (int j = 0; j < ImageX; j++) {
            dirgb[i * ImageX + j].rgbtBlue = rotate[i * ImageX + j].rgbtBlue;
            dirgb[i * ImageX + j].rgbtGreen = rotate[i * ImageX + j].rgbtGreen;
            dirgb[i * ImageX + j].rgbtRed = rotate[i * ImageX + j].rgbtRed;
        }
    }
    for (int i = 0; i < size; i++) {
        if (((dirgb[i].rgbtBlue + dirgb[i].rgbtGreen + dirgb[i].rgbtRed) / 3) < 85) {
            dia[i] = 0;
            dirgb[i].rgbtBlue = 0;
            dirgb[i].rgbtGreen = 0;
            dirgb[i].rgbtRed = 0;
        }
        else {
            dia[i] = 255;
            dirgb[i].rgbtBlue = 255;
            dirgb[i].rgbtGreen = 255;
            dirgb[i].rgbtRed = 255;
        }
    }

    char  input_img[] = "./input_img/HW2opencv_2.bmp"; 
    FILE* input_file = fopen(input_img, "rb");
  
    unsigned char* raw_img = new unsigned char[size];
    unsigned char* img_thishead = new unsigned char[1078];
    fread(img_thishead, 1, 1078, input_file);
    unsigned char* bmp_img = new unsigned char[size]; // array for image data
    fread(bmp_img, 1, size, input_file);

    Mat bmp_mat = Mat(800, 800, CV_8UC1, bmp_img);
    Mat raw_mat = Mat(800, 800, CV_8UC1, raw_img);

    for (int i = 0; i < ImageX; i++)
    {
        for (int j = 0; j < ImageY; j++)
        {
            raw_img[(i * ImageY + j)] = bmp_img[(i * ImageY + j)];

        }
    }



 /*   imshow("open", bmp_mat);*/
    moveWindow("open", 0, 0);
    waitKey(0);
 /*   imshow("this", raw_mat);*/
    moveWindow("this", 0, 0);
    waitKey(0);

    Mat readdia = Mat(ImageX, ImageY, CV_8UC3, dirgb);
  /*  imshow("readdia", readdia);*/
    moveWindow("readdia", 800, 0);
    waitKey(0);
    char filename2[] = "HW2_C++_2.bmp";
    savebmp(filename2, dirgb, FileHeader, InfoHeader, ImageX, ImageY);
    cout << "file has been saved as HW2_C++_2.bmp successful" << endl;

    Begin5 = clock();
    int labelvalue = detect(raw_img, newimg, ImageX, ImageY);
    End5 = clock();
    cout << "connected component: " << End5 - Begin5 << endl;
    int* labelarea = new int[labelvalue];
    double* labelcentroidX = new double[labelvalue];
    double* labelcentroidY = new double[labelvalue];
    Begin3 = clock();

    block(color, newimg, ImageY, ImageX, labelvalue);
    Mat newblock = Mat(ImageX, ImageY, CV_8UC3, color);
    int POINT[3][3] = { { 0, 255, 0 }, {255, 0, 0}, {0, 0, 255} };
    clock_t  Begin6, End6;
    Begin6 = clock();
    for (int i = 2; i < labelvalue + 1; i++) {
        labelarea[i - 1] = area(newimg, ImageY, ImageX, i);
        labelcentroidX[i - 1] = centroidX(newimg, ImageY, ImageX, i, labelarea[i - 1]);
        labelcentroidY[i - 1] = centroidY(newimg, ImageY, ImageX, i, labelarea[i - 1]);
        cout << "label route" << i-1 << " : area = " << labelarea[i - 1] << " centroid = ( " << (int)labelcentroidX[i -1 ] << ", " << (int)labelcentroidY[i-1 ] << ")" << endl;
        color[(int)labelcentroidX[i - 1] + ImageX * (int)labelcentroidY[i - 1]].rgbtBlue = POINT[i % 3][0];
        color[(int)labelcentroidX[i - 1] + ImageX * (int)labelcentroidY[i - 1]].rgbtGreen = POINT[i % 3][1];
        color[(int)labelcentroidX[i - 1] + ImageX * (int)labelcentroidY[i - 1]].rgbtRed = POINT[i % 3][2];

    }
    End6 = clock();
    cout << "property analysis: " << End6 - Begin6 << endl;


   
    End3 = clock();
    cout <<"drawing: " << End3 - Begin3 << endl;
    
 /*   imshow("box", newblock);*/

    imwrite("block.bmp", newblock);
    moveWindow("box", 800, 0);
    waitKey(0);
    char filename3[] = "HW2_C++_3.bmp";
    savebmp(filename3, color, FileHeader, InfoHeader, ImageX, ImageY);
    cout << "file has been saved as HW2_C++_3.bmp successful" << endl;




     destroyAllWindows();



    return 0;
}
