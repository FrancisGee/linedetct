//
// Created by francis on 17-4-10.
//

/****
 * 这个常规Hough检测应该不会用到了，这个头文件和源文件仅仅是用来保存原来的调参结果．
 *
 *
 *
 *
 */



#include <iostream>
#include "regular_hough.h"


using namespace cv;
using namespace std;


#define PI 3.1415926


void drawHoughDectedLines(vector<Vec2f> &lines, Mat &result, bool showHough) {

    std::vector<Vec2f>::const_iterator it = lines.begin();
    cout << "常规Hough检测直线的个数为: " << lines.size() << endl;

    int count = 0;
//    while (it != lines.end()){
//        float the = (*it)[1]; // second element is angle theta
//        if (the > 0.09 && the < 1.48 ||
//            the < 3.14 && the > 1.88){
//            count++;
//        }
//    }
//


    while (it != lines.end()) {

        float rho = (*it)[0];   // first element is distance rho
        float theta = (*it)[1]; // second element is angle theta


        //根据检测出直线的角度信息, 过滤掉不是车道的直线
        //角度的设置可能tricky
        if (theta > 0.09 && theta < 1.48 ||
            theta < 3.14 && theta > 1.88) { // filter to remove vertical and horizontal lines

            count++;

//            Point pt1, pt2;
//            double a = cos(theta), b = sin(theta);
//            double x0 = a * rho, y0 = b * rho;
//            pt1.x = cvRound(x0 + 800 * (-b));
//            pt1.y = cvRound(y0 + 800*(a));
//            pt2.x = cvRound(x0 - 800*(-b));
//            pt2.y = cvRound(y0 - 800*(a));


            // point of intersection of the line with first row
            Point pt1(rho / cos(theta), 10);
            // point of intersection of the line with last row
            Point pt2((rho - result.rows * sin(theta)) / cos(theta), result.rows);
            // draw a  line : Color = Scalar(R, G, B), thickness
            line(result, pt1, pt2, Scalar(255, 0, 0), 3);
        }

        cout << "line: (" << rho << "," << theta << ")\n";
        ++it;
    }

    cout << "count = " << count << endl;

    // Display the detected line image
    if (showHough) {
        imshow("Hough", result);
    }
}


vector<Vec2f> houghDected(int initialHoughVote, Mat &contours) {
    //霍夫线段检测
    /*
       Hough tranform for line detection with feedback
       Increase by 25 for the next frame if we found some lines.
       This is so we don't miss other lines that may crop up in the next frame
       but at the same time we don't want to start the feed back loop from scratch.
   */

    //将houghVote初始化
    int houghVote = initialHoughVote;
    cout << houghVote << "\n";
    std::vector<Vec2f> lines;
    if (houghVote < 1 or lines.size() > 2) { // we lost all lines. reset
        houghVote = 300;
    } else { houghVote += 25; }
    while (lines.size() < 4 && houghVote > 0) {
        HoughLines(contours, lines, 1, PI / 180, houghVote);
        houghVote -= 5;
    }
    std::cout << houghVote << "\n";

    return lines;

}
