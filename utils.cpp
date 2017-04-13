//
// Created by francis on 17-4-10.
//




#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;

void visulize(Mat &image, String name) {
    imshow(name, image);

}

Mat converttoGray(Mat &image) {
    Mat gray;
    cvtColor(image, gray, CV_RGB2GRAY);
    return gray;
}

Mat setROI(Mat &metric, Mat &src) {

    //设置感兴趣区域大小，滤除天空等干扰
    Rect roi(0, metric.cols / 3, metric.cols - 1, metric.rows - metric.cols / 3);// set the ROI for the src
    Mat ROI = src(roi);
    return ROI;
}
