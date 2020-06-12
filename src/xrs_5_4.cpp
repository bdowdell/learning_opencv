/*
Practice using region of interest (ROI).
1. Create a 210x210 single-channel byte image and zero it.
2. Within the image, build a pyramid of increasing values using ROI and cv::Mat::setTo().
That is: the outer border should be 0, the next inner border should be 20, the next inner border should be 40, 
and so on until the final innermost square is set to value 200;
3. All borders should be 10 pixels wide.
4. Display the image.
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

int main(){
    cv::Mat mat = cv::Mat::zeros(210, 210, CV_8UC1); // create a 210x210 single channel byte image of zeros
    string win_name = "ROI Pyramid";
    cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(win_name, 700, 300);
    cout << "Press <esc> key to exit ... \n";
    while(true){
        cv::imshow(win_name, mat);
        char c = (char) cv::waitKey(10);
        if (c == 27) break;
    }
}