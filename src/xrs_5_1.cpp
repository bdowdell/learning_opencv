/*
This exercise will accustom you to the idea of many functions taking matrix types.
Create a two-dimensional matrix with three channels of type byte with data size 100x100.
Set all the values to 0.
a) Draw a cirlce in the matrix
b) Display the image
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

int main() {
    // create a 100x100 3-channel matrix with all values set to 0
    cv::Mat mat = cv::Mat(100, 100, CV_8UC3, 0.0);
    cv::circle(mat, cv::Point2f(mat.cols/2, mat.rows/2), mat.cols/2, cv::Scalar(0,0,255), 1, 8, 0); // draws a red circle
    string window_circle = "Circle";
    cv::namedWindow(window_circle, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(window_circle, 500, 300);
    cv::imshow(window_circle, mat);
    cout << "Press <esc> key to exit ... \n";
    cv::waitKey(0);
    return 0;
}