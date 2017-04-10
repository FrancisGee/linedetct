//
// Created by francis on 17-4-10.
//

#include <iostream>
#include "CKalmanFilter.h"

using namespace cv;
using namespace std;

// Constructor
CKalmanFilter::CKalmanFilter(vector<Vec4i> p) {

    kalman = new KalmanFilter(8, 8, 0); // ８ measurement and state parameters
    kalman->transitionMatrix = Mat::eye(8, 8, CV_64F);

    // Initialization
    prevResult = p;
    kalman->statePre.at<int>(0) = p[0][0]; //line1_x1
    kalman->statePre.at<int>(1) = p[0][1]; //line1_y1
    kalman->statePre.at<int>(2) = p[0][2]; //line1_x2
    kalman->statePre.at<int>(3) = p[0][3]; //line1_y2
    kalman->statePre.at<int>(4) = p[1][0]; //line2_x1
    kalman->statePre.at<int>(5) = p[1][1]; //line2_y1
    kalman->statePre.at<int>(6) = p[1][2]; //line2_x2
    kalman->statePre.at<int>(7) = p[1][3]; //line2_y2

    kalman->statePost.at<int>(0) = p[0][0]; //line1_x1
    kalman->statePost.at<int>(1) = p[0][1]; //line1_y1
    kalman->statePost.at<int>(2) = p[0][2]; //line1_x2
    kalman->statePost.at<int>(3) = p[0][3]; //line1_y2
    kalman->statePost.at<int>(4) = p[1][0]; //line2_x1
    kalman->statePost.at<int>(5) = p[1][1]; //line2_y1
    kalman->statePost.at<int>(6) = p[1][2]; //line2_x2
    kalman->statePost.at<int>(7) = p[1][3]; //line2_y2



    setIdentity(kalman->measurementMatrix);
    setIdentity(kalman->processNoiseCov, Scalar::all(1e-4));
    setIdentity(kalman->measurementNoiseCov, Scalar::all(1e-1));
    setIdentity(kalman->errorCovPost, Scalar::all(5));
}

// Destructor
CKalmanFilter::~CKalmanFilter() {
    delete kalman;
}

// Prediction
vector<Vec4i> CKalmanFilter::predict() {
    Mat prediction = kalman->predict(); // predict the state of the next frame
    prevResult[0][0] = prediction.at<int>(0);
    prevResult[0][1] = prediction.at<int>(1);
    prevResult[0][2] = prediction.at<int>(2);
    prevResult[0][3] = prediction.at<int>(3);

    prevResult[1][0] = prediction.at<int>(4);
    prevResult[1][1] = prediction.at<int>(5);
    prevResult[1][2] = prediction.at<int>(6);
    prevResult[1][3] = prediction.at<int>(7);

    return prevResult;

}

// Correct the prediction based on the measurement
vector<Vec4i> CKalmanFilter::update(vector<Vec4i> measure) {


    Mat_<int> measurement(8, 1);
    measurement.setTo(Scalar(0));

    measurement.at<int>(0) = measure[0][0];
    measurement.at<int>(1) = measure[0][1];
    measurement.at<int>(2) = measure[0][2];
    measurement.at<int>(3) = measure[0][3];

    measurement.at<int>(4) = measure[1][0];
    measurement.at<int>(5) = measure[1][1];
    measurement.at<int>(6) = measure[1][2];
    measurement.at<int>(7) = measure[1][3];

    Mat estimated = kalman->correct(
            measurement); // Correct the state of the next frame after obtaining the measurements


    //这个地方有点疑问,将大的值调整到后面，我是这么理解的，这里为什么只用比较一个r1就可以得到一个pair的大值？？
    // Update the measurement
    if (estimated.at<int>(0) < estimated.at<int>(4)) {
        measure[0][0] = estimated.at<int>(0);
        measure[0][1] = estimated.at<int>(1);
        measure[0][2] = estimated.at<int>(2);
        measure[0][3] = estimated.at<int>(3);
        measure[1][0] = estimated.at<int>(4);
        measure[1][1] = estimated.at<int>(5);
        measure[1][2] = estimated.at<int>(6);
        measure[1][3] = estimated.at<int>(7);
    } else {
        measure[0][0] = estimated.at<int>(4);
        measure[0][1] = estimated.at<int>(5);
        measure[0][2] = estimated.at<int>(6);
        measure[0][3] = estimated.at<int>(7);
        measure[1][0] = estimated.at<int>(0);
        measure[1][1] = estimated.at<int>(1);
        measure[1][2] = estimated.at<int>(2);
        measure[1][3] = estimated.at<int>(3);
    }

    waitKey(1);

    return measure; // return the measurement

}