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
\*------------------------------------------------------------------------------------------*/

#if !defined LINEF
#define LINEF


#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#define PI 3.1415926


using namespace std;
using namespace cv;

class LineFinder {

private:

    // original image
    cv::Mat img;

    // vector containing the end points
    // of the detected lines
//    std::vector<cv::Vec4i> lines;

    // accumulator resolution parameters
    double deltaRho;
    double deltaTheta;

    // minimum number of votes that a line
    // must receive before being considered
    int minVote;

    // min length for a line
    double minLength;

    // max allowed gap along the line
    double maxGap;

    // distance to shift the drawn lines down when using a ROI
    int shift;

public:

    // Default accumulator resolution is 1 pixel by 1 degree
    // no gap, no mimimum length
    LineFinder() : deltaRho(1), deltaTheta(PI / 180), minVote(10), minLength(0.), maxGap(0.) {}

    // Set the resolution of the accumulator
    void setAccResolution(double dRho, double dTheta) {

        deltaRho = dRho;
        deltaTheta = dTheta;
    }

    // Set the minimum number of votes
    void setMinVote(int minv) {

        minVote = minv;
    }

    // Set line length and gap
    void setLineLengthAndGap(double length, double gap) {

        minLength = length;
        maxGap = gap;
    }

    // set image shift
    void setShift(int imgShift) {

        shift = imgShift;
    }

    // Apply probabilistic Hough Transform
    std::vector<cv::Vec4i> findLines(cv::Mat &binary) {

        std::vector<cv::Vec4i> lines;
//        lines.clear();
        cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);

        return lines;
    }

    // Draw the detected lines on an image
    void drawDetectedLines(cv::Mat &image, vector<Vec4i> lines) {
        vector<cv::Point> leftEndPoint;
        vector<cv::Point> leftStartPoint;


        vector<cv::Point> rightStartPoint;
        vector<cv::Point> rightEndPoint;


        int houghPlinecount = 0;


        double k = std::numeric_limits<double>::infinity();
        // Draw the lines
        std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();

        cout << "概率Hough检测直线的个数为" << lines.size() << endl;

        while (it2 != lines.end()) {

            cv::Point pt1((*it2)[0], (*it2)[1] + shift);
            cv::Point pt2((*it2)[2], (*it2)[3] + shift);

            if ((*it2)[0] - (*it2)[1] != 0) {
                k = (double) ((*it2)[3] - (*it2)[1]) / (double) ((*it2)[2] - (*it2)[0]);
            }

//            cout << "k = " << k << endl;
            //根据斜率来筛选可能为直线的点，有点tricky
            if (k > 0.4 || k < -0.15) {

                //判断是否在图像的左边界还是右边界
                if ((*it2)[0] < 0.5 * image.cols) {
                    leftEndPoint.push_back(pt1);
                    leftStartPoint.push_back(pt2);
                } else {
                    rightEndPoint.push_back(pt1);
                    rightStartPoint.push_back(pt2);
                }


                cv::line(image, pt1, pt2, Scalar(255), 3);

//                  cv::line(image, leftEndPoint[0], leftStartPoint[0], color, 3);
//                  cv::line(image, rightEndPoint[0], rightStartPoint[0], color, 3);

                std::cout << " HoughP line: (" << pt1 << "," << pt2 << ")\n";
                houghPlinecount++;
            }

//            cv::line(image, pt1, pt2, color, 6);
//            std::cout << " HoughP line: (" << pt1 << "," << pt2 << ")\n";
            ++it2;
        }


//        float *fitline = new float[4];





//        cvFitLine(leftline, CV_DIST_L2, 0, 0.01, 0.01,fitline);







        cout << "概率Hough画出的直线数为" << houghPlinecount << endl;



    }

    // Eliminates lines that do not have an orientation equals to
    // the ones specified in the input matrix of orientations
    // At least the given percentage of pixels on the line must
    // be within plus or minus delta of the corresponding orientation
//    std::vector<cv::Vec4i> removeLinesOfInconsistentOrientations(
//            const cv::Mat &orientations, double percentage, double delta) {
//
//        std::vector<cv::Vec4i>::iterator it = lines.begin();
//
//        // check all lines
//        while (it != lines.end()) {
//
//            // end points
//            int x1 = (*it)[0];
//            int y1 = (*it)[1];
//            int x2 = (*it)[2];
//            int y2 = (*it)[3];
//
//            // line orientation + 90o to get the parallel line
//
//            double ori1;
//            if (ori1 > PI) ori1 = ori1 - 2 * PI;
//
//            double ori2 = atan2(static_cast<double>(y2 - y1), static_cast<double>(x2 - x1)) + PI / 2;
//            if (ori2 > PI) ori2 = ori2 - 2 * PI;
//
//            // for all points on the line
//            cv::LineIterator lit(orientations, cv::Point(x1, y1), cv::Point(x2, y2));
//            int i, count = 0;
//            for (i = 0, count = 0; i < lit.count; i++, ++lit) {
//
//                float ori = *(reinterpret_cast<float *>(*lit));
//
//                // is line orientation similar to gradient orientation ?
//                if (std::min(fabs(ori - ori1), fabs(ori - ori2)) < delta)
//                    count++;
//
//            }
//
//            double consistency = count / static_cast<double>(i);
//
//            // set to zero lines of inconsistent orientation
//            if (consistency < percentage) {
//
//                (*it)[0] = (*it)[1] = (*it)[2] = (*it)[3] = 0;
//
//            }
//
//            ++it;
//        }
//
//        return lines;
//    }
};


#endif