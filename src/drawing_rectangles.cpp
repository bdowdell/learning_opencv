/*
Practice drawing boxes with the mouse
*/
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>

using namespace std;

// GLOBAL VARIABLES
cv::Point g_pt1 = cv::Point(0,0);
cv::Point g_pt2 = cv::Point(0,0);
cv::Mat g_mat = cv::Mat(500, 500, CV_8UC3, 0.0);
bool g_clicked = false;
string g_window_name = "Paint";

// mouse callback function
void onMouse( int event, int x, int y, int flags, void* params );

int main(){

    // Create a window to display the image
    cv::namedWindow(g_window_name, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(g_window_name, 500, 300);

    // Register mouse callback
    cv::setMouseCallback(g_window_name, onMouse, 0);
    
    // Display the window
    cv::imshow(g_window_name, g_mat);

    // check for user escape
    cout << "Press any key to exit.\n";
    cv::waitKey(0);

    return 0;
}

void onMouse( int event, int x, int y, int flags, void* params ){
    cv::Mat im = g_mat.clone();
    switch (event) {
        case cv::EVENT_LBUTTONDOWN: // Left mouse button is clicked down
            g_clicked = true;
            // set starting and ending points at position of LMB click
            g_pt1.x = x;
            g_pt1.y = y;
            g_pt2.x = x;
            g_pt2.y = y;
            cout << "LMB clicked down at: " << g_pt1 << endl;
            break;
        case cv::EVENT_LBUTTONUP: // LMB released
            g_clicked = false;
            // set ending point at position of LMB release
            g_pt2.x = x;
            g_pt2.y = y;
            g_mat = im; // save boxes to g_mat
            cout << "LMB released at: " << g_pt2 << endl;
            break;
        case cv::EVENT_MOUSEMOVE: // moving mouse around after LMB down
            if( g_clicked ){
                // LMB is clicked down, update point 2 as the mouse moves
                g_pt2.x = x;
                g_pt2.y = y;
            }
            break;
        default : break;
    }
    // Create the rectangle from user input & display it to current window
    cv::rectangle(im, g_pt1, g_pt2, cv::Scalar(0,0,255), 1, 8, 0);
    cv::imshow(g_window_name, im);
}