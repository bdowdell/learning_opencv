/*Create a three-channel RGB image of size 100x100.
Clear it.  Use pointer arithmetic to draw a green square
between (20,5) and (40,20)
Use mat.ptr<cv::Vec3b>(i) to get pointer to row
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

int main(){
    cv::Mat mat = cv::Mat::zeros(100, 100, CV_8UC3);

    cv::Vec3b* ptr_r20 = mat.ptr<cv::Vec3b>(20); // pointer to row 20 (top)
    cv::Vec3b* ptr_r40 = mat.ptr<cv::Vec3b>(40); // pointer to row 40 (bottom)

    cout << "Pointer address to row 20: " << &ptr_r20 << "\n";
    cout << "Pointer address to row 40: " << &ptr_r40 << "\n";

    for(int c=5; c<21; c++){
        ptr_r20[c] = cv::Vec3b(0, 255, 0); // set each column between 5->20 in row 20 to green
        ptr_r40[c] = cv::Vec3b(0, 255, 0); // set each column between 5->20 in row 40 to green
    }

    for(int r=20; r<41; r++){
        cv::Vec3b* ptr = mat.ptr<cv::Vec3b>(r);
        ptr[5] = cv::Vec3b(0, 255, 0); // set each row between 20->40 in row 5 to green
        ptr[20] = cv::Vec3b(0, 255, 0); // set each row between 20->40 in row 20 to green
    }

    /* I'm not sure this counts as pointer "arithmetic", but
       it uses pointers and draws a green rectangle, so
       I'll count it as a win.
    */

    string window_name = "fun with pointers";
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(window_name, 500, 300);
    cout << "Press <esc> key to exit ...\n";
    while(true){
        cv::imshow(window_name, mat);
        char c = (char) cv::waitKey(10);
        if( c == 27 ) break;
    }
    return 0;
}