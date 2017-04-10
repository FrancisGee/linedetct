//
// Created by francis on 17-4-10.
//

#ifndef LINEDETECT_CKALMANFILTER_H
#define LINEDETECT_CKALMANFILTER_H

#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

using namespace cv;
using namespace std;

class CKalmanFilter {
public:
    CKalmanFilter(vector<Vec4i>);

    ~CKalmanFilter();

    vector<Vec4i> predict();

    vector<Vec4i> update(vector<Vec4i>);

    KalmanFilter *kalman;
    vector<Vec4i> prevResult;

};


#endif //LINEDETECT_CKALMANFILTER_H
