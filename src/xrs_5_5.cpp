/*
Use multiple headers for one image.
1. Load an image that is at least 100x100.
2. Create two additional headers that are ROIs where width = 20 and height = 10.
   Their origins should be at (5,10) and (50,60), respectively.
3. Pass these new image subheaders to cv::bitwise_not().
4. Display the loaded image, which should have two inverted rectangles within the larger image.
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

int main( int argc, char** argv ){
    // check that the correct number of CL args have been passed.  Expect two.
    if( argc != 2 ){
        cout << "Error has occured.  Expected two command line arguments, but received: " << argc << "\n";
        cout << "Usage: " << argv[0] << " <filename>\n";
        cout << "Exiting ...\n";
        return -1;
    }
    cv::Mat img = cv::imread(argv[1]);
    // check the dimensions of the image are at least 100x100
    if( img.size().width < 100 || img.size().height < 100){
        cout << "Error has occured.  Require an image of at least 100x100.  Exiting ...\n";
        return -1;
    }
    // create two ROI's and pass each to cv::bitwise_not()
    cv::Mat roi1 = cv::Mat(img, cv::Rect(5, 10, 20, 10));
    cv::bitwise_not(roi1, roi1);
    cv::Mat roi2 = cv::Mat(img, cv::Rect(50, 60, 20, 10));
    cv::bitwise_not(roi2, roi2);
    // create a window to hold the image
    string win_name = "Exercise 5.5";
    cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(win_name, 600, 300);
    cout << "Press any key to exit ... \n";
    cv::imshow(win_name, img);
    cv::waitKey(0);
    return 0;
}