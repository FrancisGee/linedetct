//
// Created by francis on 17-4-10.
//


#include "videoProcessor.h"
#include "laneDetection.h"




videoProcessor::videoProcessor() {

    frame_width = 800;
    frame_height = 600;
}

double videoProcessor::getFPS(VideoCapture &capture) {
    double rate = capture.get(CV_CAP_PROP_FPS);
    cout << "视频的帧率为 " << rate << "fps" << endl;
    return rate;
}


void videoProcessor::showBasicInformation(VideoCapture &capture) {

    //显示视频尺寸信息
    double dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Frame Size = " << dWidth << "x" << dHeight << endl;

    double rate = capture.get(CV_CAP_PROP_FPS);

    cout << "视频的帧率为 " << rate << "fps" << endl;
}

void videoProcessor::setSize(Mat &input) {

    //固定视频帧的大小，便于检测---width = 800 , height = 600
    Size size(frame_width, frame_height);
    resize(input, input, size);
//    cout << "width = " << image.cols << "  height = " << image.rows << endl;

}
