#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <vector>
#include <fstream>

#define IMAGE_HEIGHT 512
#define IMAGE_WIDTH 512

using namespace std;


RGBTRIPLE* nearest_neighbor(RGBTRIPLE* src, int height, int width, int new_height, int new_width)
{
    RGBTRIPLE* dst = new RGBTRIPLE[new_height * new_width];
    double rate = (double)new_height / height;
    //cv::Mat mat_result(new_height, new_width, CV_8UC3, dst);
    for (int row = 0; row < new_width; row++)
    {
        for (int col = 0; col < new_height; col++) {
            int zoomingRow = (int)(row / rate);
            int zoomingCol = (int)(col / rate);
            dst[row * new_width + col] = src[zoomingRow * width + zoomingCol];
        }
    }
    return dst;
}
#ifndef BMP_H
#define BMP_H

class BitMap {

private:
    unsigned char m_bmpFileHeader[14];
    unsigned int m_pixelArrayOffset;
    unsigned char m_bmpInfoHeader[40];

    int m_height;
    int m_width;
    int m_bitsPerPixel;

    int m_rowSize;
    int m_pixelArraySize;

    unsigned char* m_pixelData;

    char* m_copyname;
    const char* m_filename;
public:
    BitMap(const char* filename);
    ~BitMap();

    std::vector<unsigned int> getPixel(int i, int j);

    void makeCopy(char* filename);
    void writePixel(int i, int j, int R, int G, int B);

    void swapPixel(int i, int j, int i2, int j2);

    void dispPixelData();

    int width() { return m_width; }
    int height() { return m_height; }

    int vd(int i, int j);
    int hd(int i, int j);

    bool isSorted();
};

BitMap::BitMap(const char* filename) {

    using namespace std;

    m_filename = filename;

    ifstream inf(filename);
    if (!inf) {
        cerr << "Unable to open file: " << filename << "\n";
    }



    //unsigned char m_bmpFileHeader[14];
    unsigned char a;
    for (int i = 0; i < 14; i++) {
        inf >> hex >> a;
        m_bmpFileHeader[i] = a;
    }
    if (m_bmpFileHeader[0] != 'B' || m_bmpFileHeader[1] != 'M') {
        cerr << "Your info header might be different!\nIt should start with 'BM'.\n";
    }

    /*
        THE FOLLOWING LINE ONLY WORKS IF THE OFFSET IS 1 BYTE!!!!! (it can be 4 bytes max)
        That should be fixed now.
        old line was
        m_pixelArrayOffset = m_bmpFileHeader[10];
    */
    unsigned int* array_offset_ptr = (unsigned int*)(m_bmpFileHeader + 10);
    m_pixelArrayOffset = *array_offset_ptr;


    if (m_bmpFileHeader[11] != 0 || m_bmpFileHeader[12] != 0 || m_bmpFileHeader[13] != 0) {
        std::cerr << "You probably need to fix something. bmp.h(" << __LINE__ << ")\n";
    }



    //unsigned char m_bmpInfoHeader[40];
    for (int i = 0; i < 40; i++) {
        inf >> hex >> a;
        m_bmpInfoHeader[i] = a;
    }

    int* width_ptr = (int*)(m_bmpInfoHeader + 4);
    int* height_ptr = (int*)(m_bmpInfoHeader + 8);

    m_width = *width_ptr;
    m_height = *height_ptr;

    printf("W: %i, H: %i", m_width, m_height);

    m_bitsPerPixel = m_bmpInfoHeader[14];
    if (m_bitsPerPixel != 24) {
        cerr << "This program is for 24bpp files. Your bmp is not that\n";
    }
    int compressionMethod = m_bmpInfoHeader[16];
    if (compressionMethod != 0) {
        cerr << "There's some compression stuff going on that we might not be able to deal with.\n";
        cerr << "Comment out offending lines to continue anyways. bpm.h line: " << __LINE__ << "\n";
    }


    m_rowSize = int(floor((m_bitsPerPixel * m_width + 31.) / 32)) * 4;
    m_pixelArraySize = m_rowSize * abs(m_height);

    m_pixelData = new unsigned char[m_pixelArraySize];

    inf.seekg(m_pixelArrayOffset, ios::beg);
    for (int i = 0; i < m_pixelArraySize; i++) {
        inf >> hex >> a;
        m_pixelData[i] = a;
    }



}

BitMap::~BitMap() {
    delete[] m_pixelData;
}

void BitMap::dispPixelData() {
    for (int i = 0; i < m_pixelArraySize; i++) {
        std::cout << (unsigned int)m_pixelData[i] << " ";
    }
    std::cout << "\n";
}

// output is in rgb order.
std::vector<unsigned int> BitMap::getPixel(int x, int y) {
    if (x < m_width && y < m_height) {
        std::vector<unsigned int> v;
        v.push_back(0);
        v.push_back(0);
        v.push_back(0);

        y = m_height - 1 - y; //to flip things
        //std::cout<<"y: "<<y<<" x: "<<x<<"\n";
        v[0] = (unsigned int)(m_pixelData[m_rowSize * y + 3 * x + 2]); //red
        v[1] = (unsigned int)(m_pixelData[m_rowSize * y + 3 * x + 1]); //greed
        v[2] = (unsigned int)(m_pixelData[m_rowSize * y + 3 * x + 0]); //blue


        return v;
    }
    else { std::cerr << "BAD INDEX\n"; std::cerr << "X: " << x << " Y: " << y << "\n"; }
}

void BitMap::makeCopy(char* filename) {
    std::ofstream copyfile(filename);
    std::ifstream infile(m_filename);
    m_copyname = filename;

    unsigned char c;
    while (infile) {
        infile >> c;
        copyfile << c;
    }
}

// changes the file
void BitMap::writePixel(int x, int y, int R, int G, int B) {
    std::fstream file(m_filename);
    y = m_height - 1 - y; // to flip things.
    int blueOffset = m_pixelArrayOffset + m_rowSize * y + 3 * x + 0;

    // writes to the file
    file.seekg(blueOffset, std::ios::beg);
    file << (unsigned char)B;
    file.seekg(blueOffset + 1, std::ios::beg);
    file << (unsigned char)G;
    file.seekg(blueOffset + 2, std::ios::beg);
    file << (unsigned char)R;

    // edits data in pixelData array 
    m_pixelData[m_rowSize * y + 3 * x + 2] = (unsigned char)R;
    m_pixelData[m_rowSize * y + 3 * x + 1] = (unsigned char)G;
    m_pixelData[m_rowSize * y + 3 * x + 0] = (unsigned char)B;
}

// changes the file
void BitMap::swapPixel(int i, int j, int i2, int j2) {
    std::vector<unsigned int> p1 = (*this).getPixel(i, j);

    std::vector<unsigned int> p2 = (*this).getPixel(i2, j2);

    (*this).writePixel(i, j, p2[0], p2[1], p2[2]);
    (*this).writePixel(i2, j2, p1[0], p1[1], p1[2]);

}
#endif

int HW1C_PLUS()
{
    BITMAPFILEHEADER FileHeader;
    BITMAPINFOHEADER InfoHeader;
    char /*filename[50],*/ filename2[50];
    FILE* fp;
    FILE* fpw, *fpw2, * fpw3;
    int size, ImageX, ImageY, choose;

    fp = fopen("./input_img/InputImage1.bmp", "rb");
 

    if (!fp) {
        cout << "file open fail!" << endl;
        return 0;
    }

    fread(&FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    ImageX = InfoHeader.biWidth;
    ImageY = InfoHeader.biHeight;

    int height_in, width_in, height_out, width_out, height_out1024, width_out1024;
    height_in = 512;
    width_in = 512;
    height_out = 256;
    width_out = 256;
    height_out1024 = 1024;
    width_out1024 = 1024;



    size = ImageX * ImageY ;
    RGBTRIPLE rgb,rgb2,rgb3;
    RGBTRIPLE* color = new RGBTRIPLE[size];
    RGBTRIPLE* rotate = new RGBTRIPLE[size];
    RGBTRIPLE* scale1 = new RGBTRIPLE[size];
    RGBTRIPLE* rotate1 = new RGBTRIPLE[size];
    RGBTRIPLE* rotate2 = new RGBTRIPLE[size];
    RGBTRIPLE* inter = new RGBTRIPLE[size];

    RGBTRIPLE* img_nearest_neighbor_in = new RGBTRIPLE[height_out1024 * width_out1024];
    RGBTRIPLE* img_nearest_neighbor_out = new RGBTRIPLE[height_out1024 * width_out1024];
    unsigned char bmppad[3] = { 0,0,0 };

    for (int i = 0; i < ImageY; i++)
    {
        for (int j = 0; j < ImageX; j++)
        {
            fread(&rgb, sizeof(RGBTRIPLE), 1, fp);
            color[i * ImageX + j] = rgb;
        }
    }
    fclose(fp);

    //process
    cout << "choose which process want to do " << endl << "1.Write(.bmp)" << endl << "2.Generate a color negative image." << endl << "3.Split and reassemble into new image." << endl <<"4.Zoom Lena 512*512 to 1024*1024. " <<endl<< "5.Shrink Lena 512*512 to 128*128. "<<endl <<"6.Zoom color negative image 512*512 to 1024*1024. "<<endl<<"7.Shrink split and reassemble image from 512*512 to 256*256. "<< endl << "8.Exit" << endl;
    cin >> choose;
    switch (choose) {
    case 1:
        for (int i = 0; i < ImageY; i++) {
            for (int j = 0; j < ImageX; j++) {
                rotate[i * ImageX + j] = color[i * ImageX + j];
            }
        }


        break;


    case 2:


        for (int i = 0; i < ImageY; i++) {
            for (int j = 0; j < ImageX; j++) {
                rotate[i * ImageX + j].rgbtBlue = 255 - color[i * ImageX + j].rgbtBlue;
                rotate[i * ImageX + j].rgbtGreen = 255 - color[i * ImageX + j].rgbtGreen;
                rotate[i * ImageX + j].rgbtRed = 255 - color[i * ImageX + j].rgbtRed;
            }
        }
        break;

    case 3:
        for (int i = 0; i < ImageY; i++)
        {
            for (int j = 0; j < ImageX; j++)
            {
                rotate1[size - (i * ImageX + j)] = color[j * ImageX + (511 - i)];

            }
        }
        for (int i = 0; i < ImageY; i++)
        {
            for (int j = 0; j < ImageX; j++)
            {
                rotate2[size - (i * ImageX + j)] = color[(511 - j) * ImageX + i];

            }
        }
        for (int i = 0; i < ImageY; i++)
        {
            for (int j = 0; j < ImageX; j++)
            {

                if (i < j && i>511 - j || i > j && i < 511 - j)
                {
                    rotate[i * ImageX + j] = rotate2[i * ImageX + j];

                }
                else
                {
                    rotate[i * ImageX + j] = rotate1[i * ImageX + j];

                }

            }
        }

        //imshow("lena256_out", mat_lena);
        //cv::waitKey(0);
        break;

    case 4: {

        img_nearest_neighbor_in= nearest_neighbor(color, height_in, width_in, height_out1024, width_out1024);

        
 

        for (int i =0; i < height_out1024; i++)
        {
            for (int j = 0; j < width_out1024; j++)
            {
                img_nearest_neighbor_out[i * width_out1024 + j] = img_nearest_neighbor_in[(1024-i-1) * width_out1024 + j];
            }
        }


        break;
    }
    case 5: {
         img_nearest_neighbor_in = nearest_neighbor(color, height_in, width_in, height_out, width_out);

          for (int i = 0; i < height_out; i++)
          {
              for (int j = 0; j < width_out; j++)
              {
                  img_nearest_neighbor_out[i * width_out + j] = img_nearest_neighbor_in[(256 - i - 1) * width_out + j];
              }
          }

          break;
    }

    case 6:


        for (int i = 0; i < ImageY; i++) {
            for (int j = 0; j < ImageX; j++) {
                rotate[i * ImageX + j].rgbtBlue = 255 - color[i * ImageX + j].rgbtBlue;
                rotate[i * ImageX + j].rgbtGreen = 255 - color[i * ImageX + j].rgbtGreen;
                rotate[i * ImageX + j].rgbtRed = 255 - color[i * ImageX + j].rgbtRed;
            }
        }
        img_nearest_neighbor_in = nearest_neighbor(rotate, height_in, width_in, height_out1024, width_out1024);
        for (int i = 0; i < height_out1024; i++)
        {
            for (int j = 0; j < width_out1024; j++)
            {
                img_nearest_neighbor_out[i * width_out1024 + j] = img_nearest_neighbor_in[(1024 - i - 1) * width_out1024 + j];
            }
        }

        break;
    case 7:
        for (int i = 0; i < ImageY; i++)
        {
            for (int j = 0; j < ImageX; j++)
            {
                rotate1[size - (i * ImageX + j)] = color[j * ImageX + (511 - i)];

            }
        }
        for (int i = 0; i < ImageY; i++)
        {
            for (int j = 0; j < ImageX; j++)
            {
                rotate2[size - (i * ImageX + j)] = color[(511 - j) * ImageX + i];

            }
        }
        for (int i = 0; i < ImageY; i++)
        {
            for (int j = 0; j < ImageX; j++)
            {

                if (i < j && i>511 - j || i > j && i < 511 - j)
                {
                    rotate[i * ImageX + j] = rotate2[i * ImageX + j];

                }
                else
                {
                    rotate[i * ImageX + j] = rotate1[i * ImageX + j];

                }

            }
        }
        img_nearest_neighbor_in = nearest_neighbor(rotate, height_in, width_in, height_out, width_out);

        for (int i = 0; i < height_out; i++)
        {
            for (int j = 0; j < width_out; j++)
            {
                img_nearest_neighbor_out[i * width_out + j] = img_nearest_neighbor_in[(256 - i - 1) * width_out + j];
            }
        }
        //imshow("lena256_out", mat_lena);
        //cv::waitKey(0);
        break;



    case 8:
        return 0;
        default:

        break;
    }
   

    //output

    //cv::Mat r = cv::Mat(height_out, width_out, CV_8UC3, rotate);
    //imshow("HW1bonus", r);
    //waitKey(0);
    //destroyWindow("HW1bonus");
    //imwrite("HW1bonus.png", r);

    ////cout << "please enter the filename to save (must be .bmp) : ";
    ////cin >> filename ;
    fpw2 = fopen("./input_img/lena1.bmp", "rb");
    fpw3 = fopen("./input_img/lena2.bmp", "rb");

    if (choose == 1) {
        char filename[] = "HW1_C++_1.bmp";
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
        cout << "file has been saved as HW1_C++_1.bmp successful" << endl;
        fclose(fpw);
    }
    if (choose == 2) {
        char filename[] = "HW1_C++_2.bmp";
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
        cout << "file has been saved as HW1_C++_2.bmp successful" << endl;
        fclose(fpw);
    }
    if (choose == 3) {
        char filename[] = "HW1_C++_3.bmp";
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
        cout << "file has been saved as HW1_C++_2.bmp successful" << endl;

        fclose(fpw);
    }
    if (choose == 4) {

        BITMAPFILEHEADER FileHeader1;
        BITMAPINFOHEADER InfoHeader2;
  

        char filename[] = "HW1_C++_bonus1.bmp";
        fpw = fopen(filename, "wb");
        if (!fpw) {
            cout << "file save fail!" << endl;
        }

        fread(&FileHeader1, sizeof(BITMAPFILEHEADER), 1, fpw2);
        fread(&InfoHeader2, sizeof(BITMAPINFOHEADER), 1, fpw2);

        fwrite(&FileHeader1, sizeof(BITMAPFILEHEADER), 1, fpw);
        fwrite(&InfoHeader2, sizeof(BITMAPINFOHEADER), 1, fpw);



        for (int i = 0; i < 1024; i++)
        {
            for (int j = 0; j < 1024; j++)
            {
                rgb2 = img_nearest_neighbor_out[(1024 - i - 1) * 1024 + j];
                fwrite(&rgb2, sizeof(RGBTRIPLE), 1, fpw);
            }
        }
        cout << "file has been saved as HW1_C++_bonus1.bmp successful" << endl;
        fclose(fpw);
    }
    if (choose == 5) {

        BITMAPFILEHEADER FileHeader3;
        BITMAPINFOHEADER InfoHeader4;




        char filename[] = "HW1_C++_bonus2.bmp";
        fpw = fopen(filename, "wb");
        if (!fpw) {
            cout << "file save fail!" << endl;
        }

        fread(&FileHeader3, sizeof(BITMAPFILEHEADER), 1, fpw3);
        fread(&InfoHeader4, sizeof(BITMAPINFOHEADER), 1, fpw3);

        fwrite(&FileHeader3, sizeof(BITMAPFILEHEADER), 1, fpw);
        fwrite(&InfoHeader4, sizeof(BITMAPINFOHEADER), 1, fpw);



        for (int i = 0; i < 256; i++)
        {
            for (int j = 0; j < 256; j++)
            {
                rgb3 = img_nearest_neighbor_out[(256 - i - 1) * 256 + j];
                fwrite(&rgb3, sizeof(RGBTRIPLE), 1, fpw);
            }
        }
        cout << "file has been saved as HW1_C++_bonus2.bmp successful" << endl;
        fclose(fpw);
    }
    if (choose == 6) {

        BITMAPFILEHEADER FileHeader1;
        BITMAPINFOHEADER InfoHeader2;


        char filename[] = "HW1_C++_bonus3.bmp";
        fpw = fopen(filename, "wb");
        if (!fpw) {
            cout << "file save fail!" << endl;
        }

        fread(&FileHeader1, sizeof(BITMAPFILEHEADER), 1, fpw2);
        fread(&InfoHeader2, sizeof(BITMAPINFOHEADER), 1, fpw2);

        fwrite(&FileHeader1, sizeof(BITMAPFILEHEADER), 1, fpw);
        fwrite(&InfoHeader2, sizeof(BITMAPINFOHEADER), 1, fpw);



        for (int i = 0; i < 1024; i++)
        {
            for (int j = 0; j < 1024; j++)
            {
                rgb2 = img_nearest_neighbor_out[(1024 - i - 1) * 1024 + j];
                fwrite(&rgb2, sizeof(RGBTRIPLE), 1, fpw);
            }
        }
        cout << "file has been saved as HW1_C++_bonus3.bmp successful" << endl;
        fclose(fpw);
    }
    if (choose == 7) {

        BITMAPFILEHEADER FileHeader3;
        BITMAPINFOHEADER InfoHeader4;




        char filename[] = "HW1_C++_bonus4.bmp";
        fpw = fopen(filename, "wb");
        if (!fpw) {
            cout << "file save fail!" << endl;
        }

        fread(&FileHeader3, sizeof(BITMAPFILEHEADER), 1, fpw3);
        fread(&InfoHeader4, sizeof(BITMAPINFOHEADER), 1, fpw3);

        fwrite(&FileHeader3, sizeof(BITMAPFILEHEADER), 1, fpw);
        fwrite(&InfoHeader4, sizeof(BITMAPINFOHEADER), 1, fpw);



        for (int i = 0; i < 256; i++)
        {
            for (int j = 0; j < 256; j++)
            {
                rgb3 = img_nearest_neighbor_out[(256 - i - 1) * 256 + j];
                fwrite(&rgb3, sizeof(RGBTRIPLE), 1, fpw);
            }
        }
        cout << "file has been saved as HW1_C++_bonus4.bmp successful" << endl;
        fclose(fpw);
    }
    return 0();
}