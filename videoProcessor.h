//
// Created by francis on 17-4-10.
//

#ifndef LINEDETECT_VIDEOPROCESSOR_H
#define LINEDETECT_VIDEOPROCESSOR_H

#include "opencv2/highgui/highgui.hpp"

using namespace cv;

class videoProcessor {

private:

    VideoCapture caputure; //The OpenCV Video capture object

    string WindowNameInput;    //Input display windnow name
    string WindowNameOutput; //Output display window name

    // processing of each frame
    bool callIt;
    int fps;

    int delay;
    long fnumber; // number of processed frames

    long frameToStop; // stop at this frame number
    bool stop;


public:

    videoProcessor();

    double getFPS(VideoCapture &capture);


};


#endif //LINEDETECT_VIDEOPROCESSOR_H
