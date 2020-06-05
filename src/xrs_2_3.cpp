/* This program combines the Pyramid Down transform in ex_2_6_pyrdown_transform.cpp
With the capture and store video code used in ex_2_11_video_writer.cpp
The program should read video and write out a downsampled version*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

int main( int argc, char** argv ){
    // Declare cv::VideoCapture object to hold the input file
    cv::VideoCapture cap;

    // initialize a variable to indicate whether input is from camera or file
    int video_flag = 0;

    // Read in to cap
    if( argc == 2 ) {
        // reading from camera, only two args
        cap.open(0, cv::CAP_V4L);
        cout << "Reading from camera.  Recording until <esc> key is pressed ...\n";
    } else {
        // reading from file
        cap.open( argv[1] );
        cout << "Reading from file: " << argv[1] << "\n";
        video_flag = 1;
    }

    // check that the file is open
    if( !cap.isOpened() ) {
        cerr << "Error!  Could not open the file.  Exiting ...\n";
        return -1;
    }

    // get frames per second and dimensions
    double fps = cap.get(cv::CAP_PROP_FPS);
    cv::Size size(
        (int) cap.get(cv::CAP_PROP_FRAME_WIDTH),
        (int) cap.get(cv::CAP_PROP_FRAME_HEIGHT)
    );

    // Declare writer
    cv::VideoWriter writer;
    int codec = cv::VideoWriter::fourcc('M','J','P','G');
    
    // Open the output file
    // size needs to be divided by four as we are applying two passes of pyrDown
    if( !video_flag ) {
        writer.open( argv[1], codec, fps, size/=4); // reading from camera
    } else {
        writer.open( argv[2], codec, fps, size/=4); // reading from file
    }

    // Declare Mat objects to hold input frame & pyrDown frame
    cv::Mat bgr_frame, pyr1_frame, pyr2_frame;

    // Create a window for when we read from a camera
    string rec_window = "Recording";
    cv::namedWindow( rec_window, cv::WINDOW_AUTOSIZE );
    cv::moveWindow( rec_window, 700, 300 );

    // Loop over the input
    for(;;) {
        cap >> bgr_frame;
        if( bgr_frame.empty() ) break; // end of input
        if( !video_flag ) {
            cv::imshow( rec_window, bgr_frame );
        }
        // apply two passes of pyrDown
        cv::pyrDown( bgr_frame, pyr1_frame );
        cv::pyrDown( pyr1_frame, pyr2_frame );
        writer << pyr2_frame;

        if( !video_flag ) {
            char c = (char) cv::waitKey(10); // we are reading from the camera and need to tell it we are done capturing
            if( c == 27 ) break;
        }
    }

    cap.release();
    return 0;

}