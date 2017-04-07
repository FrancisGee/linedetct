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


void GaussianFilter::Init(int _sigma_x, int _sigma_y) {
    sigma_x_ = _sigma_x;
    sigma_y_ = _sigma_y;
    onGaussianChange();
}

void GaussianFilter::onGaussianChange() {
    int i;
    float x;
    float xs[GAUSSIANSIZE] = {0};
    float ys[GAUSSIANSIZE] = {0};
    float sumx = 0, sumy = 0;

    //fprintf(stderr, "\\sigam_{x}: %d, \\sigma_{y}: %d, Gaussian Size: %d\n", sigma_x_, sigma_y_, GAUSSIANSIZE);
    for (i = 0; i < GAUSSIANSIZE; i++) {
        x = 1.0 * i + 0.5 - GAUSSIANSIZE * 0.5;
        xs[i] = (1.0 / sigma_x_ / sigma_x_) * exp(-1.0 * x * x / 2 / sigma_x_ / sigma_x_) *
                (1 - x * x / sigma_x_ / sigma_x_);
        ys[i] = exp(-1.0 * x * x / 2 / sigma_y_ / sigma_y_);

        sumx += xs[i];
        sumy += ys[i];
    }
    for (i = 0; i < GAUSSIANSIZE; i++) {
        xs[i] /= sumx;
        ys[i] /= sumy;
    }


    gaussian_kernel_x_ = Mat(1, GAUSSIANSIZE, CV_32F, xs).clone();
    gaussian_kernel_y_ = Mat(1, GAUSSIANSIZE, CV_32F, ys).clone();
}

void GaussianFilter::Filter(Mat src, Mat &dst) {
    sepFilter2D(src, dst, src.depth(), gaussian_kernel_x_, gaussian_kernel_y_);
}

