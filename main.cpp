/*------------------------------------------------------------------------------------------*\
   Lane Detection

   General idea and some code modified from:
   chapter 7 of Computer Vision Programming using the OpenCV Library.
   by Robert Laganiere, Packt Publishing, 2011.

   This program is free software; permission is hereby granted to use, copy, modify,
   and distribute this source code, or portions thereof, for any purpose, without fee,
   subject to the restriction that the copyright notice may not be removed
   or altered from any source or altered source distribution.
   The software is released on an as-is basis and without any warranties of any kind.
   In particular, the software is not guaranteed to be fault-tolerant or free from failure.
   The author disclaims all warranties with regard to this software, any use,
   and any consequent failure, is purely the responsibility of the user.

   Copyright (C) 2013 Jason Dorweiler, www.transistor.io


Notes:

	Add up number on lines that are found within a threshold of a given rho,theta and
	use that to determine a score.  Only lines with a good enough score are kept.

	Calculation for the distance of the car from the center.  This should also determine
	if the road in turning.  We might not want to be in the center of the road for a turn.

	Several other parameters can be played with: min vote on houghp, line distance and gap.  Some
	type of feed back loop might be good to self tune these parameters.

	We are still finding the Road, i.e. both left and right lanes.  we Need to set it up to find the
	yellow divider line in the middle.

	Added filter on theta angle to reduce horizontal and vertical lines.

	Added image ROI to reduce false lines from things like trees/powerlines
\*------------------------------------------------------------------------------------------*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include "linefinder.h"
#include "laneDetection.h"
#include "img_transform.h"
#include "utils.h"
#include "videoProcessor.h"

#define PI 3.1415926

using namespace cv;
using namespace std;



int main(int argc, char *argv[]) {


    //初始化视频处理类
    videoProcessor processor;



    //初始化Lanedetection实例
    laneDetection detect; // object of laneDetection class


    //初始化LineFinder实例
    LineFinder ld;

    //初始化概率霍夫参数
    ld.setLineLengthAndGap(20, 10);    //min accepted length and gap
    ld.setMinVote(100);
    //根据驾驶员的角度设置偏移量
    ld.setShift(0);


    // 初始化GaussianFilter class
    GaussianFilter _gaussianfilter;
    _gaussianfilter.Init(4, 25);





    string arg = argv[1];


    //Set up windows
    //for Debug
    bool showOriginal = false;
    bool showCanny = true;
    bool showHoughP = false;

    string window_name = "Processed Video";
    namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window;

    VideoCapture capture(arg);

    // Check video is open
    if (!capture.isOpened()) {
        cout << "Could not read video file" << endl;
        return -1;
    }


    //显示视频尺寸，帧率等信息
    int rate = processor.getFPS(capture);
    processor.showBasicInformation(capture);


    //Process Frame



    // Read first frame.
    Mat frame1;
    capture.read(frame1);



    Mat input;



    // Till we consider all the frame
    while (capture.read(input)) {
        // capture on intervals to make vid smoother



        if (input.empty())
            break;



        //固定视频帧的大小，便于检测
        processor.setSize(input);

        Mat gray = converttoGray(input);




        /**
         * 高斯平滑操作，消除图像噪点对边缘检检测器的影响(可选)
         *
         *
         *
         */

        Mat gauss_gray;
        _gaussianfilter.Filter(gray, gauss_gray);


        Mat gray_gauss_ROI = setROI(input, gauss_gray);



        //拿到输入固定大小帧的裁剪图像(未经过灰度变换，高斯模糊，边缘检测等操作)
        Mat imgROI = setROI(input, input);


        visulize(gray_gauss_ROI, "经过裁剪过的高斯模糊图像");


        //用canny算子对ROI进行特征提取
//        Mat contours = cannyExtract(grayROI, true);

        //垂直边缘检测
        Mat contours = detect.LMFiltering(gray_gauss_ROI);

        visulize(contours, "边缘检测后的图像");




        //  Detect lines
        std::vector<Vec4i> li = ld.findLines(contours);


        Mat houghP(imgROI.size(), CV_8U, Scalar(0));


        ld.drawDetectedLines(imgROI, li);
//
//        imshow("houghP", imgROI);

        visulize(imgROI, "概率霍夫检测车道线叠加到原图像");


        imshow(window_name, input);






        //交互式操作，按任意键就可以暂停，再按任意键就恢复播放
        int delay = 30;

        if (delay >= 0 && waitKey(delay) >= 0)
            waitKey(0);


    }
}



