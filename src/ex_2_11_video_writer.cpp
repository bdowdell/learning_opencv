#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

int main( int argc, char** argv ){
    // create windows to hold input & output
    string window_in = "Example 2-11";
    string window_out = "Log_Polar";
    cv::namedWindow( window_in, cv::WINDOW_AUTOSIZE );
    cv::namedWindow( window_out, cv::WINDOW_AUTOSIZE );

    // Declare cv::VideoCapture object
    cv::VideoCapture cap;
    int video_flag = 0; // if reading from camera or from video file

    if( argc == 2){
        cap.open(0, cv::CAP_V4L ); // read from video camera
        cout << "Reading from video camera.\n";
        cout << "Press <esc> key to exit.\n";
    } else {
        cap.open( argv[1] );
        cout << "Reading from video file: " << argv[1] << "\n";
        cout << "Press <esc> key to exit.\n";
        video_flag = 1;
    }

    if( !cap.isOpened() ){
        cerr << "Error.  Could not open file. Exiting ...\n";
        return -1;
    }

    // get frames per second & size
    double fps = cap.get( cv::CAP_PROP_FPS );
    cv::Size size( (int) cap.get(cv::CAP_PROP_FRAME_WIDTH), (int) cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    // declare cv::VideoWriter object
    cv::VideoWriter writer;
    int codec = cv::VideoWriter::fourcc('M','J','P','G');
    if( !video_flag ){
        writer.open( argv[1], codec, fps, size ); // reading from camera, so argv[1] is output filename
    } else {
        writer.open( argv[2], codec, fps, size ); // reading from video file, so argv[2] is output filename
    }

    // Declar Mat objects for the input and the output frames
    cv::Mat logpolar_frame, bgr_frame;

    // Positions windows
    int x = 500;
    int y = 300;
    cv::moveWindow( window_in, x, y);
    cv::moveWindow(window_out, x + size.width + 10, y);

    for(;;){
        cap >> bgr_frame;
        if( bgr_frame.empty() ) break;
        cv::imshow( window_in, bgr_frame );

        // apply log-polar transform
        cv::logPolar(
                    bgr_frame,
                    logpolar_frame,
                    cv::Point2f( bgr_frame.cols/2, bgr_frame.rows/2 ),
                    40, 
                    cv::WARP_FILL_OUTLIERS
                    );

        cv::imshow( window_out, logpolar_frame );
        writer << logpolar_frame;

        char c = (char) cv::waitKey(10);
        if( c == 27 ) break;
    }

    cap.release();
    return 0;

}