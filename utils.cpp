//
// Created by francis on 17-4-10.
//




#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"


using namespace cv;
using namespace std;


void showBasicInformation(VideoCapture &capture) {

    //显示视频尺寸信息
    double dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Frame Size = " << dWidth << "x" << dHeight << endl;

    double rate = capture.get(CV_CAP_PROP_FPS);

    cout << "视频的帧率为 " << rate << "fps" << endl;
}