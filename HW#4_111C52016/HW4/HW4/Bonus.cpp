#include "HW4.h"

int Bonus()
{
	Mat ref = imread(".\\input_img\\findwally2.jpg");
    Mat ref_1 = imread(".\\input_img\\findwally1.jpg");
	Mat tpl_wally = imread(".\\input_img\\wally.jpg");

    int choose;
    cout << "choose which picture want to do " << endl << "1.wally1 \n" << "2.wally2" << endl;
    cin >> choose;
    switch (choose) {
    case 1: {
        Mat tpl = tpl_wally(Rect(38, 11, 8, 4));
        Mat templat_eye = tpl_wally(Rect(39, 23, 5, 5));
        Mat templat_shirt = tpl_wally(Rect(39, 48, 10, 8));
        Mat ref2 = ref_1.clone();
        Mat ref3 = ref_1.clone();
        Mat fin = ref_1.clone();

        Mat gref, gtpl;
        cvtColor(ref_1, gref, COLOR_BGR2GRAY);
        cvtColor(tpl, gtpl, COLOR_BGR2GRAY);

        Mat res(ref_1.rows - tpl.rows + 1, ref_1.cols - tpl.cols + 1, CV_32FC1);
        matchTemplate(gref, gtpl, res, TM_CCOEFF_NORMED);
        threshold(res, res, 0.8, 1., THRESH_TOZERO);

        vector<Point> temp1, temp2, temp3, temp_remain, temp_remain1, temp_remain2;

        while (true)
        {
            double minval, maxval, threshold = 0.9;
            Point minloc, maxloc;
            minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);
            /*cout << maxloc << endl;*/
            if (maxval >= threshold)
            {
                rectangle(ref_1, maxloc, Point(maxloc.x + tpl.cols, maxloc.y + tpl.rows), CV_RGB(0, 255, 0), 2);
                floodFill(res, maxloc, Scalar(0), 0, Scalar(.1), Scalar(1.));
                temp1.push_back(maxloc);
            }
            else
                break;
        }

     /*   imshow("reference", ref_1);
        waitKey(0);
        destroyWindow("reference");*/

        Mat gref2, gtpl2;
        cvtColor(ref2, gref2, COLOR_BGR2GRAY);
        cvtColor(templat_eye, gtpl2, COLOR_BGR2GRAY);
        Mat res2(ref2.rows - gtpl2.rows + 1, ref2.cols - gtpl2.cols + 1, CV_32FC1);
        matchTemplate(gref2, gtpl2, res2, TM_CCOEFF_NORMED);
        threshold(res2, res2, 0.8, 1., THRESH_TOZERO);

        while (true)
        {
            double minval_2, maxval_2, threshold_2 = 0.82;
            Point minloc_2, maxloc_2;
            minMaxLoc(res2, &minval_2, &maxval_2, &minloc_2, &maxloc_2);
            /*cout << maxloc_2 << endl;*/
            if (maxval_2 >= threshold_2)
            {
                rectangle(ref2, maxloc_2, Point(maxloc_2.x + tpl.cols, maxloc_2.y + tpl.rows), CV_RGB(0, 255, 0), 2);
                floodFill(res2, maxloc_2, Scalar(0), 0, Scalar(.1), Scalar(1.));
                temp2.push_back(maxloc_2);
            }

            else
                break;
        }

    /*    imshow("reference_2", ref2);
        waitKey(0);
        destroyWindow("reference_2");*/

        Mat gref3, gtpl3;
        cvtColor(ref3, gref3, COLOR_BGR2GRAY);
        cvtColor(templat_shirt, gtpl3, COLOR_BGR2GRAY);
        Mat res3(ref3.rows - gtpl3.rows + 1, ref3.cols - gtpl3.cols + 1, CV_32FC1);
        matchTemplate(gref3, gtpl3, res3, TM_CCOEFF_NORMED);
        threshold(res3, res3, 0.8, 1., THRESH_TOZERO);

        while (true)
        {
            double minval_3, maxval_3, threshold_3 = 0.9;
            Point minloc_3, maxloc_3;
            minMaxLoc(res3, &minval_3, &maxval_3, &minloc_3, &maxloc_3);
            /*cout<< maxloc_3 << endl;*/
            if (maxval_3 >= threshold_3)
            {
                rectangle(ref3, maxloc_3, Point(maxloc_3.x + gtpl3.cols, maxloc_3.y + gtpl3.rows), CV_RGB(0, 255, 0), 2);
                floodFill(res3, maxloc_3, Scalar(0), 0, Scalar(.1), Scalar(1.));
                temp3.push_back(maxloc_3);
            }

            else
                break;
        }

   /*     imshow("reference_3", ref3);
        waitKey(0);
        destroyWindow("reference_3");*/
        cout << temp1.size() << "\n" << temp2.size() << "\n" << temp3.size() << "\n" << endl;

        for (int i = 0; i < temp3.size(); i++) {
            for (int j = 0; j < temp2.size(); j++) {

                float temp_distance = calculateDistance(temp3[i], temp2[j]);
                cout << (int)temp_distance << "\n" << endl;
                if ((int)temp_distance < 20) {
                    temp_remain.push_back(temp3[i]);
                    temp_remain1.push_back(temp2[j]);
                }

            }
        }
        cout << temp_remain1 << endl;

        for (int i = 0; i < temp_remain1.size(); i++) {
            for (int j = 0; j < temp1.size(); j++) {
                float temp_distance = calculateDistance(temp_remain1[i], temp1[j]);
                cout << (int)temp_distance << "\n" << endl;
                if ((int)temp_distance < 20) {
                    temp_remain2.push_back(temp1[j]);
                }
            }
        }
        cout << temp_remain << endl;
        cout << temp_remain2 << endl;
        int bounding_x = tpl.rows + templat_eye.rows + templat_shirt.rows + 20;
        int bounding_y = tpl.cols + templat_eye.cols + 20 + templat_shirt.cols + 30;
        Rect box(temp_remain2.front().x, temp_remain2.front().y, bounding_x, bounding_y);
        rectangle(fin, box, CV_RGB(0, 255, 0), 2, 8, 0);
        imshow("final", fin);
        imwrite("Bonus_1.png", fin);
        waitKey(0);
        destroyWindow("final");
        break;
    }
    case 2: {
        Mat tpl = tpl_wally(Rect(33, 10, 7, 3));
        Mat templat_eye = tpl_wally(Rect(40, 18, 6, 5));
        Mat templat_shirt = tpl_wally(Rect(39, 48, 10, 8));
        Mat ref2 = ref.clone();
        Mat ref3 = ref.clone();
        Mat fin = ref.clone();

        Mat gref, gtpl;
        cvtColor(ref, gref, COLOR_BGR2GRAY);
        cvtColor(tpl, gtpl, COLOR_BGR2GRAY);

        Mat res(ref.rows - tpl.rows + 1, ref.cols - tpl.cols + 1, CV_32FC1);
        matchTemplate(gref, gtpl, res, TM_CCOEFF_NORMED);
        threshold(res, res, 0.8, 1., THRESH_TOZERO);

        vector<Point> temp1, temp2, temp3, temp_remain, temp_remain1, temp_remain2;

        while (true)
        {
            double minval, maxval, threshold = 0.9;
            Point minloc, maxloc;
            minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);
            /*cout << maxloc << endl;*/
            if (maxval >= threshold)
            {
                rectangle(ref, maxloc, Point(maxloc.x + tpl.cols, maxloc.y + tpl.rows), CV_RGB(0, 255, 0), 2);
                floodFill(res, maxloc, Scalar(0), 0, Scalar(.1), Scalar(1.));
                temp1.push_back(maxloc);
            }
            else
                break;
        }

        /*imshow("reference", ref);
        waitKey(0);
        destroyWindow("reference");*/

        Mat gref2, gtpl2;
        cvtColor(ref2, gref2, COLOR_BGR2GRAY);
        cvtColor(templat_eye, gtpl2, COLOR_BGR2GRAY);
        Mat res2(ref2.rows - gtpl2.rows + 1, ref2.cols - gtpl2.cols + 1, CV_32FC1);
        matchTemplate(gref2, gtpl2, res2, TM_CCOEFF_NORMED);
        threshold(res2, res2, 0.8, 1., THRESH_TOZERO);

        while (true)
        {
            double minval_2, maxval_2, threshold_2 = 0.5;
            Point minloc_2, maxloc_2;
            minMaxLoc(res2, &minval_2, &maxval_2, &minloc_2, &maxloc_2);
            /*cout << maxloc_2 << endl;*/
            if (maxval_2 >= threshold_2)
            {
                rectangle(ref2, maxloc_2, Point(maxloc_2.x + tpl.cols, maxloc_2.y + tpl.rows), CV_RGB(0, 255, 0), 2);
                floodFill(res2, maxloc_2, Scalar(0), 0, Scalar(.1), Scalar(1.));
                temp2.push_back(maxloc_2);
            }

            else
                break;
        }

        /* imshow("reference_2", ref2);
         waitKey(0);
         destroyWindow("reference_2");*/

        Mat gref3, gtpl3;
        cvtColor(ref3, gref3, COLOR_BGR2GRAY);
        cvtColor(templat_shirt, gtpl3, COLOR_BGR2GRAY);
        Mat res3(ref3.rows - gtpl3.rows + 1, ref3.cols - gtpl3.cols + 1, CV_32FC1);
        matchTemplate(gref3, gtpl3, res3, TM_CCOEFF_NORMED);
        threshold(res3, res3, 0.8, 1., THRESH_TOZERO);

        while (true)
        {
            double minval_3, maxval_3, threshold_3 = 0.9;
            Point minloc_3, maxloc_3;
            minMaxLoc(res3, &minval_3, &maxval_3, &minloc_3, &maxloc_3);
            /*cout<< maxloc_3 << endl;*/
            if (maxval_3 >= threshold_3)
            {
                rectangle(ref3, maxloc_3, Point(maxloc_3.x + gtpl3.cols, maxloc_3.y + gtpl3.rows), CV_RGB(0, 255, 0), 2);
                floodFill(res3, maxloc_3, Scalar(0), 0, Scalar(.1), Scalar(1.));
                temp3.push_back(maxloc_3);
            }

            else
                break;
        }

        /*  imshow("reference_3", ref3);
          waitKey(0);
          destroyWindow("reference_3");
          cout << temp1.size() << "\n" << temp2.size() << "\n" << temp3.size() << "\n" << endl;*/

        for (int i = 0; i < temp1.size(); i++) {
            for (int j = 0; j < temp2.size(); j++) {

                float temp_distance = calculateDistance(temp1[i], temp2[j]);
                cout << (int)temp_distance << "\n" << endl;
                if ((int)temp_distance < 20) {
                    temp_remain.push_back(temp1[i]);
                    temp_remain1.push_back(temp2[j]);
                }

            }
        }
        cout << temp_remain1 << endl;

        for (int i = 0; i < temp_remain1.size(); i++) {
            for (int j = 0; j < temp3.size(); j++) {
                float temp_distance = calculateDistance(temp_remain1[i], temp3[j]);
                cout << (int)temp_distance << "\n" << endl;
                if ((int)temp_distance < 30) {
                    temp_remain2.push_back(temp3[j]);
                }
            }
        }
        cout << temp_remain2 << endl;
        int bounding_x = tpl.rows + templat_eye.rows + templat_shirt.rows + 20;
        int bounding_y = tpl.cols + templat_eye.cols + 20 + templat_shirt.cols + 30;
        Rect box(temp_remain.back().x, temp_remain.back().y, bounding_x, bounding_y);
        rectangle(fin, box, CV_RGB(0, 255, 0), 2, 8, 0);
        imshow("final", fin);
        imwrite("Bonus_2.png", fin);
        waitKey(0);
        destroyWindow("final");
        break;
    }
        return 0;
    }
}