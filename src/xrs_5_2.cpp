/*
Create a two-dimensional matrix with three channels of type byte
with data size 100x100, and set all the values to 0.  Use the cv::Mat
element access functions to modify the pixels.  Draw a green rectangle
between (20,5) and (40,20)
*/
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

int main(){
    cv::Mat mat = cv::Mat(100, 100, CV_8UC3, 0.0);
    // This makes a solid-filled green rectangle
    /*for(int i = 20; i < 41; i++){
        for(int j = 5; j < 21; j++){
            mat.at<cv::Vec3b>(i,j)[0] = 0;
            mat.at<cv::Vec3b>(i,j)[1] = 255;
            mat.at<cv::Vec3b>(i,j)[2] = 0;
        }
    }*/
    // top & bottom sides
    for(int j = 5; j < 21; j++){
        mat.at<cv::Vec3b>(20,j)[0] = 0;
        mat.at<cv::Vec3b>(20,j)[1] = 255;
        mat.at<cv::Vec3b>(20,j)[2] = 0;
        mat.at<cv::Vec3b>(40,j)[0] = 0;
        mat.at<cv::Vec3b>(40,j)[1] = 255;
        mat.at<cv::Vec3b>(40,j)[2] = 0;
    }
    // left and right sides
    for(int i = 20; i < 41; i++){
        mat.at<cv::Vec3b>(i,5)[0] = 0;
        mat.at<cv::Vec3b>(i,20)[0] = 0;
        mat.at<cv::Vec3b>(i,5)[1] = 255;
        mat.at<cv::Vec3b>(i,20)[1] = 255;
        mat.at<cv::Vec3b>(i,5)[2] = 0;
        mat.at<cv::Vec3b>(i,20)[2] = 0;
    }
    string window_square = "Square";
    cv::namedWindow(window_square, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(window_square, 500, 300);
    cv::imshow(window_square, mat);
    cout << "Press <esc> key to exit ...\n";
    cv::waitKey(0);
    return 0;
}