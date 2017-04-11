//
// Created by francis on 17-4-6.
//

//一些常用的辅助函数

#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;


#ifndef LINEDETECT_UTILS_H
#define LINEDETECT_UTILS_H

void visulize(Mat &image, String name);

Mat converttoGray(Mat &image);

Mat setROI(Mat &image, Mat &src);



#endif //LINEDETECT_UTILS_H
