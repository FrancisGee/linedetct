//
// Created by francis on 17-4-10.
//


#include "videoProcessor.h"


videoProcessor::videoProcessor() {

}

double videoProcessor::getFPS(VideoCapture &capture) {
    double rate = capture.get(CV_CAP_PROP_FPS);
    return rate;
}