//
// Created by francis on 17-4-6.
//

#ifndef LINEDETECT_IMG_TRANSFORM_H
#define LINEDETECT_IMG_TRANSFORM_H


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;



Mat cannyExtract(Mat &imgROI, bool showCanny);

/*
 * 高斯滤波，让图像更加平滑，消除掉噪声对边缘检测器的干扰。
 *
 *
 *
 */

class GaussianFilter {
public:
    void Init(int _sigma_x, int _sigma_y);

    void Filter(Mat src, Mat &dst);

private:
    Mat gaussian_kernel_x_;
    Mat gaussian_kernel_y_;
    int GAUSSIANSIZE = 5;
    int sigma_x_;
    int sigma_y_;

    void onGaussianChange();
};



#endif //LINEDETECT_IMG_TRANSFORM_H
