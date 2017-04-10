//
// Created by francis on 17-4-10.
//


/******
 * 这个常规Hough检测应该不会用到了，这个头文件和源文件仅仅是用来保存原来的调参结果．
 *
 *
 */

#ifndef LINEDETECT_REGULAR_HOUGH_H
#define LINEDETECT_REGULAR_HOUGH_H

#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;

void drawHoughDectedLines(vector<Vec2f> &lines, Mat &result, bool showHough);

vector<Vec2f> houghDected(int initialHoughVote, Mat &image);


#endif //LINEDETECT_REGULAR_HOUGH_H
