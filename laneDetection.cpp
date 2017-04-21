//
// Created by francis on 17-4-6.
//

/*
 * 车道线检测核心程序
 *
 *
 *
 */

#include <iostream>
#include "laneDetection.h"

using namespace cv;

laneDetection::laneDetection() {



//    _LMWidth = 10;
    _LMWidth = 20;
    _thres = 40;
    _rho = 1;
    _theta = CV_PI / 180.0;
    _houghThres = 100;
    _ransacThres = 0.01;
}

laneDetection::~laneDetection() {

}

//////////
// Filter to detect lane Markings
// This is just one approach. Other approaches can be Edge detection, Connected Components, etc.
// The advantage of this approach is that it will only detect the edges in the vertical direction.
//这种方式与此前的Canny不同，仅显示垂直方向的边缘，因为车道线主要在垂直方向
Mat laneDetection::LMFiltering(Mat src) {
    Mat img;

    ///// Generating the mask to mask the top half of the image
//    Mat mask = Mat(src.size(), CV_8UC1, Scalar(1));
//    for (int i = 0; i < mask.rows / 2; i++) {
//        for (int j = 0; j < mask.cols; j++) {
//            mask.at<uchar>(Point(j, i)) = 0;
//        }
//    }
//    src.copyTo(img, mask);
    src.copyTo(img);
    /////

    Mat _detectedEdges = Mat(img.size(), CV_8UC1); // detectedEdges
    _detectedEdges.setTo(0);

    int val = 0;
    // iterating through each row
    for (int j = img.rows / 2; j < img.rows; j++) {
        unsigned char *imgptr = img.ptr<uchar>(j);
        unsigned char *detEdgesptr = _detectedEdges.ptr<uchar>(j);

        // iterating through each column seeing the difference among columns which are "width" apart
        for (int i = _LMWidth; i < img.cols - _LMWidth; ++i) {
            if (imgptr[i] != 0) {
                //二阶差分
                val = 2 * imgptr[i];
                val += -imgptr[i - _LMWidth];
                val += -imgptr[i + _LMWidth];
                val += -abs((int) (imgptr[i - _LMWidth] - imgptr[i + _LMWidth]));

                val = (val < 0) ? 0 : val;
                val = (val > 255) ? 255 : val;

                detEdgesptr[i] = (unsigned char) val;
            }
        }
    }

    // Thresholding
//    threshold(_detectedEdges, _detectedEdges, _thres, 255, THRESH_OTSU);
//    threshold(_detectedEdges, _detectedEdges, _thres, 255, THRESH_BINARY);
    threshold(_detectedEdges, _detectedEdges, _thres, 255, 0);


    return _detectedEdges;

}
//////////

// Performing Hough Transform--常规霍夫检测
//我自己的那个常规霍夫检查弃用，这里用它的常规霍夫看看效果
vector<Vec2f> laneDetection::houghTransform(Mat contours) {


    HoughLines(contours, _lines, _rho, _theta, _houghThres); // Finding the hough lines
    vector<Vec2f> retVar;

    if (_lines.size() > 1) {
        Mat labels, centers;
        Mat samples = Mat(_lines.size(), 2, CV_32F);

        for (int i = 0; i < _lines.size(); i++) {
            samples.at<float>(i, 0) = _lines[i][0];
            samples.at<float>(i, 1) = _lines[i][1];
        }
        // K means clustering to get two lines
        kmeans(samples, 2, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 1000, 0.001), 5, KMEANS_PP_CENTERS,
               centers);

        ////////////////// Using RANSAC to get rid of outliers
        _lines.clear();

        vector<Point2f> left;
        vector<Point2f> right;
        for (int i = 0; i < labels.rows; i++) {
            if (labels.at<int>(i) == 0) left.push_back(Point2f(samples.at<float>(i, 0), samples.at<float>(i, 1)));
            else right.push_back(Point2f(samples.at<float>(i, 0), samples.at<float>(i, 1)));
        }
        // Performing Ransac
        vector<Point2f> leftR = ransac(left);
        vector<Point2f> rightR = ransac(right);
        //////////////////

        if (leftR.size() < 1 || rightR.size() < 1 ||
            (float) (cos((leftR[0].y + leftR[1].y) / 2) * cos((rightR[0].y + rightR[1].y) / 2)) >= 0)
            return retVar;

        // pushing the end points of the line to _lines
        _lines.push_back(Vec2f((leftR[0].x + leftR[1].x) / 2, (leftR[0].y + leftR[1].y) / 2));
        _lines.push_back(Vec2f((rightR[0].x + rightR[1].x) / 2, (rightR[0].y + rightR[1].y) / 2));

    }


    return _lines;
}



//我自己的概率霍夫检测

vector<Vec4i> houghPTransform() {

}


// Implementing RANSAC to remove outlier lines
// Picking the best estimate having maximum number of inliers
// TO DO: Better implementation
//用RANSAC方法来滤掉异常的直线，RANSAC方法不太了解，但看到在相关论文上提到过
vector<Point2f> laneDetection::ransac(vector<Point2f> data) {

    vector<Point2f> res;
    int maxInliers = 0;

    // Picking up the first sample
    for (int i = 0; i < data.size(); i++) {
        Point2f p1 = data[i];

        // Picking up the second sample
        for (int j = i + 1; j < data.size(); j++) {
            Point2f p2 = data[j];
            int n = 0;

            // Finding the total number of inliers
            for (int k = 0; k < data.size(); k++) {
                Point2f p3 = data[k];
                float normalLength = norm(p2 - p1);
                float distance = abs(
                        (float) ((p3.x - p1.x) * (p2.y - p1.y) - (p3.y - p1.y) * (p2.x - p1.x)) / normalLength);
                if (distance < _ransacThres) n++;
            }

            // if the current selection has more inliers, update the result and maxInliers
            if (n > maxInliers) {
                res.clear();
                maxInliers = n;
                res.push_back(p1);
                res.push_back(p2);
            }

        }

    }

    return res;
}


// Draw Lines on the image
Mat laneDetection::drawLines(Mat img, vector<Vec2f> lines) {

    Mat imgRGB;

    vector<Point> endPoints;

    // Here, I convert the polar coordinates to Cartesian coordinates.
    // Then, I extend the line to meet the boundary of the image.
    for (int i = 0; i < lines.size(); i++) {
        float r = lines[i][0];
        float t = lines[i][1];

        float x = r * cos(t);
        float y = r * sin(t);

        Point p1(cvRound(x - 1.0 * sin(t) * 1000), cvRound(y + cos(t) * 1000));
        Point p2(cvRound(x + 1.0 * sin(t) * 1000), cvRound(y - cos(t) * 1000));

        clipLine(img.size(), p1, p2);
        if (p1.y > p2.y) {
            endPoints.push_back(p1);
            endPoints.push_back(p2);
        } else {
            endPoints.push_back(p2);
            endPoints.push_back(p1);
        }

    }

    ///// Finding the intersection point of two lines to plot only lane markings till the intersection
    Point pint;
    bool check = findIntersection(endPoints, pint);

    if (check) {
        line(imgRGB, endPoints[0], pint, Scalar(0, 255, 255), 5);
        line(imgRGB, endPoints[2], pint, Scalar(0, 255, 255), 5);
    }
    /////


    //打印出与车道线有关的信息
    // Saving to intercepts.csv

//    float xIntercept = min(endPoints[0].x,endPoints[2].x);
//    myfile << name << "," << xIntercept * 2 << "," << pint.x * 2 << endl;

    visualize(imgRGB); // Visualize the final result

    return imgRGB;
}


// Finding the Vanishing Point--找到消失点，确定水平线
bool laneDetection::findIntersection(vector<Point> endP, Point &pi) {

    Point x = endP[2] - endP[0];
    Point d1 = endP[1] - endP[0];
    Point d2 = endP[3] - endP[2];

    float cross = d1.x * d2.y - d1.y * d2.x;
    if (abs(cross) < 1e-8) // No intersection
        return false;

    double t1 = (x.x * d2.y - x.y * d2.x) / cross;
    pi = endP[0] + d1 * t1;
    return true;


}

// Visualize---可视化操作，用于展示
void laneDetection::visualize(Mat imgRGB) {

    namedWindow("LaneMarkings");
    imshow("LaneMarkings", imgRGB);

}


//C++中的单元测试操作吗？？？？
#ifdef LaneTest
int main()
{
    laneDetection detect; // object of laneDetection class

    string ippath = "./images/";
    string oppath = "./output/";
    string imname;
    ifstream imageNames ("imNames.txt");
    getline(imageNames,imname);

    ippath += imname;
    Mat img1 = imread(ippath,0); // Read the image
    resize(img1,img1,Size(detect._width,detect._height));

    detect.LMFiltering(img1); // Filtering to detect Lane Markings
    vector<Vec2f> lines = detect.houghTransform(); // Hough Transform
    Mat imgFinal = detect.drawLines(img1, lines, imname); // draw final Lane Markings on the original image for display

    oppath += imname;
    imwrite(oppath,imgFinal);

    while ( getline (imageNames,imname) ){
        ippath = "./images/";
        oppath = "./output/";
        ippath += imname;

        Mat img2 = imread(ippath,0); // Read the image
        resize(img2,img2,Size(detect._width,detect._height));

        detect.LMFiltering(img2); // Filtering to detect Lane Markings
        vector<Vec2f> lines2 = detect.houghTransform(); // Hough Transform


        // if lanes are not detected, then use the Kalman Filter prediction
        if (lines2.size() < 2) {
            imgFinal = detect.drawLines(img2,lines, imname); // draw final Lane Markings on the original image for display
            oppath += imname;
            imwrite(oppath,imgFinal);
            continue;
        }

        ///// Kalman Filter to predict the next state
        CKalmanFilter KF2(lines);
        vector<Vec2f> pp = KF2.predict();

        vector<Vec2f> lines2Final = KF2.update(lines2);
        lines = lines2Final;
        imgFinal = detect.drawLines(img2,lines2, imname); // draw final Lane Markings on the original image for display
        /////

        oppath += imname;
        imwrite(oppath,imgFinal);
    }


}
#endif
