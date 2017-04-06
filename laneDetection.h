//
// Created by francis on 17-4-6.
//

/*
 * 车道检测部分的核心程序
 * 用来替换封装不好的头文件linefinder.h
 *
 *
 */


#ifndef LINEDETECT_LANEDETECTION_H
#define LINEDETECT_LANEDETECTION_H

#include <iostream>
#include <vector>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

using namespace cv;
using namespace std;

class laneDetection {

public:
    laneDetection();

    ~laneDetection();         //析构函数？？？
    Mat LMFiltering(Mat);

    vector<Vec4i> houghPTransform(Mat contours);                            //我的概率霍夫检测
    vector<Vec2f> houghTransform(Mat contours);                             //别人的常规霍夫检测
    Mat drawLines(Mat, vector<Vec2f>);

    int _width, _height;

protected:
    bool findIntersection(vector<Point>, Point &);

    vector<Point2f> ransac(vector<Point2f>);

    void visualize(Mat);

    vector<Vec2f> _lines;
    Mat _detectedEdges;
    int _LMWidth; // Lane Mark Width
    int _thres;
    float _rho, _theta, _ransacThres, _houghThres;

};


#endif //LINEDETECT_LANEDETECTION_H
