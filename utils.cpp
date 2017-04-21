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


vector<Vec4i> validateLane(vector<Vec4i> lines) {
    std::vector<cv::Vec4i> lanes;
    //用斜率來过滤不是车道线的直线
    double k = std::numeric_limits<double>::infinity();

    std::vector<cv::Vec4i>::const_iterator it = lines.begin();

    while (it != lines.end()) {
        cv::Point pt1((*it)[0], (*it)[1]);
        cv::Point pt2((*it)[2], (*it)[3]);

        if ((*it)[0] - (*it)[1] != 0) {
            k = (double) ((*it)[3] - (*it)[1]) / (double) ((*it)[2] - (*it)[0]);
        }

        if (k > 0.4 || k < -0.15) {
            lanes.push_back(*it);
        }

        ++it;
    }
    return lanes;

}


vector<Vec4i> findleftLane(vector<Vec4i> lanes, Mat &image) {
    std::vector<cv::Vec4i> leftLanes;


    std::vector<cv::Vec4i>::const_iterator it = lanes.begin();

    while (it != lanes.end()) {
        if ((*it)[0] < 0.5 * image.cols) {
            leftLanes.push_back(*it);
        }
        it++;
    }
    return leftLanes;


}

vector<Vec4i> findrightLane(vector<Vec4i> lanes, Mat &image) {
    std::vector<cv::Vec4i> rightLanes;

    std::vector<cv::Vec4i>::const_iterator it = lanes.begin();

    while (it != lanes.end()) {
        if ((*it)[0] >= 0.5 * image.cols) {
            rightLanes.push_back(*it);
        }
        it++;
    }
    return rightLanes;
}





