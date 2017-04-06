//
// Created by francis on 17-4-6.
//

/*
 * 图像预处理部分可能会用到的图像处理技术
 * 比如IPM仿射变换，高斯滤波，高帽卷积等
 *
 */


#include <opencv2/core/core.hpp>
#include "img_transform.h"


using namespace cv;


Mat cannyExtract(Mat &imgROI, bool showCanny) {

    // Canny algorithm
    Mat contours;
    double thresh = 200;
    Canny(imgROI, contours, 0.5 * thresh, thresh);
    Mat contoursInv;
    threshold(contours, contoursInv, 128, 255, THRESH_BINARY_INV);

    return contours;
}

