/*
This program reads in from a camera (or file if supplied) and allows the user
to interactively apply pyramid downscaling using a trackbar.

The initial position is 1 which represents the default 2x downsample.

The maximum position is 4 which represents 16x downsample.

The user can slide back and forth.  If the user selects 0, the trackbar
automatically resets to 1 as 0 is not a valid option.
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// GLOBAL VARIABLES

int g_slider_position = 0;
const int g_slider_max_pos = 4;
cv::Mat g_bgr_frame;
cv::Size g_pyr_size;

// END GLOBAL VARIABLES

// recursivePyrDown() recursively downsamples a frame where scale is the number of recursive passes of pyrDown()
cv::Mat recursivePyrDown(cv::Mat inframe, int scale) {
    if ( scale == 1 ) {
        cv::Mat outframe;
        cv::pyrDown( inframe, outframe );
        return outframe;
    } else {
        cv::Mat processed_frame;
        cv::pyrDown( recursivePyrDown( inframe, --scale ), processed_frame );
        return processed_frame;
    }
}

// callback function for trackbar position change
void onTrackbarSlide(int pos, void *){
    g_pyr_size.width = g_bgr_frame.size().width / pow(2, pos);
    g_pyr_size.height = g_bgr_frame.size().height / pow(2, pos);
    if ( pos != 0 ) {
        int scale = pow(2, pos);
        cout << "Applying " << scale << "x pyramid downsampling.\n";
        cout << "\tNew dimensions: " << g_pyr_size << "\n\n";
    } else {
        cout << "Not applying any pyramid downsampling.\n";
        cout << "\tBack to original dimensions: " << g_pyr_size << "\n\n";
    }
}

int main( int argc, char** argv ){

    cv::VideoCapture cap;

    // Read in to cap
    if( argc == 1 ) {
        // reading from camera, only two args
        cap.open(0, cv::CAP_V4L);
        cout << "\nReading from camera.  Reading until <esc> key is pressed ...\n";
    } else {
        // Wrong number of arguments
        cout << "Received the wrong number of arguments: " << argc << "\n";
        cout << "Exiting ... \n";
    }

    // check that the file is open
    if( !cap.isOpened() ) {
        cerr << "Error!  Could not open the file.  Exiting ...\n";
        return -1;
    }

    cv::Size size( (int) cap.get(cv::CAP_PROP_FRAME_WIDTH), (int) cap.get(cv::CAP_PROP_FRAME_HEIGHT) );
    cout << "\tVideo dimensions are: " << size << "\n\n";

    // Create a window for when we read from a camera as well as for the processed frame
    string rec_window = "Input Recording";
    string proc_window = "Processed Frame";
    int window_x = 600;
    int window_y = 150;
    int delta_x = (int) size.width;
    cv::namedWindow( rec_window, cv::WINDOW_AUTOSIZE );
    cv::moveWindow( rec_window, window_x, window_y );
    cv::namedWindow( proc_window, cv::WINDOW_AUTOSIZE );
    cv::moveWindow( proc_window, (window_x + delta_x) + 10, window_y );

    // Create trackbar for selecting pyramid downsample factor
    string trackbar_name = "Pyramid Downsampling Reduction Factor";
    cv::createTrackbar( trackbar_name, rec_window, &g_slider_position, g_slider_max_pos, onTrackbarSlide );

    cout << "Initially applying no pyramid downsampling.\n\n";

    // Loop over the input
    for(;;) {
        cap >> g_bgr_frame;
        int current_pos = cv::getTrackbarPos( trackbar_name, rec_window );
        if( g_bgr_frame.empty() ) break; // end of input

        cv::imshow( rec_window, g_bgr_frame );

        // only allow values greater than 0 for downsampling factor
        if ( current_pos != 0 ) {
            cv::imshow( proc_window, recursivePyrDown( g_bgr_frame, current_pos ) );
        } else {
            //cv::setTrackbarPos( trackbar_name, rec_window, 1 ); // reset the slider to 1
            cv::imshow( proc_window, g_bgr_frame ); // show the full-resolution frame
        }

        char c = (char) cv::waitKey(10); // we are reading from the camera and need to tell it we are done capturing
        if( c == 27 ) break;
    }

    cap.release();
    return 0;

}