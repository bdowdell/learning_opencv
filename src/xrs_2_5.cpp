/*
This program reads in from a camera (or file if supplied) and allows the user
to interactively apply pyramid downscaling using a trackbar.

The initial position is 1 which represents the default 2x downsample.

The maximum position is 4 which represents 8x downsample.

The user can slide back and forth.  If the user selects 0, the trackbar
automatically resets to 1 as 0 is not a valid option.
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

// GLOBAL VARIABLES

int g_slider_position = 1;
const int g_slider_max_pos = 4;

// END GLOBAL VARIABLES

// pydDownsample() recursively downsamples a frame where scale is the number of recursive passes of pyrDown()
cv::Mat pydDownsample( cv::Mat inframe, int scale ) {
    if ( scale == 1 ) {
        cv::Mat outframe;
        cv::pyrDown( inframe, outframe );
        return outframe;
    } else {
        cv::Mat proc;
        cv::pyrDown( pydDownsample( inframe, --scale ), proc );
        return proc;
    }
}

void onTrackbarSlide( int pos, void * ){
    if ( pos != 0 ) {
        cout << "Applying " << 2*pos << "x pyramid downsampling.\n";
    }
}

int main( int argc, char** argv ){

    cv::VideoCapture cap;

    // Read in to cap
    if( argc == 1 ) {
        // reading from camera, only two args
        cap.open(0, cv::CAP_V4L);
        cout << "Reading from camera.  Reading until <esc> key is pressed ...\n";
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
    cout << "Video dimensions are: " << size << "\n";

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

    // Declare a Mat to hold the current frame from the camera stream
    cv::Mat bgr_frame;

    cout << "Initially applying " << 2*g_slider_position << "x pyramid downsampling.\n";

    // Loop over the input
    for(;;) {
        cap >> bgr_frame;
        int current_pos = cv::getTrackbarPos( trackbar_name, rec_window );
        if( bgr_frame.empty() ) break; // end of input

        cv::imshow( rec_window, bgr_frame );

        // only allow values greater than 0 for downsampling factor
        if ( current_pos != 0 ) {
            cv::imshow( proc_window, pydDownsample( bgr_frame, current_pos ) );
        } else {
            cv::setTrackbarPos( trackbar_name, rec_window, 1 ); // reset the slider to 1
        }

        char c = (char) cv::waitKey(10); // we are reading from the camera and need to tell it we are done capturing
        if( c == 27 ) break;
    }

    cap.release();
    return 0;

}