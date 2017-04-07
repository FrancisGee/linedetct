//
// Created by francis on 17-4-6.
//

#ifndef LINEDETECT_BASIC_H
#define LINEDETECT_BASIC_H


#include <opencv2/core/core.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * 规定:
 * y轴递减方向为正方向
 */

using namespace cv;


#define RADIAN_1                0.017453292519943295
#define RADIAN_90               1.5707963267948966
#define RADIAN_180              3.141592653589793
#define ADIAN_NEGATIVE_90      -1.5707963267948966

#include "equation.h"
#include "img_transform.h"




typedef struct LaneLineNode {
    LaneLineNode() {}

    Point2i p1;
    Point2i p2;
    float k, b; //线性方程中y=kx+b 中的k,b
    float theta;
    float score;

    void Clear() {}
} LaneLineNode;


enum LaneLineType {
    DOTTED_LINE,//虚线
    FULL_LINE   //实线
};


#endif //LINEDETECT_BASIC_H
