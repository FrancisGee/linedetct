//
// Created by francis on 17-4-10.
//




#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"


using namespace cv;
using namespace std;

void visulize(Mat &image, String name) {
    imshow("name", image);

}

Mat converttoGray(Mat &image) {

}

Mat setROI(Mat &image, Mat &src) {

    //设置感兴趣区域大小，滤除天空等干扰
    Rect roi(0, image.cols / 3, image.cols - 1, image.rows - image.cols / 3);// set the ROI for the image
    Mat grayROI = src(roi);
    return grayROI;
}
