#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

int main( int argc, char** argv ){
    
    string window_name = "Example 2-10";
    cv::namedWindow( window_name, cv::WINDOW_AUTOSIZE );
    cv::moveWindow( window_name, 600, 100 );

    cv::VideoCapture cap;

    int video_flag = 0;

    // If only one argument is supplied, try opening the video camera
    // If a second command is supplied, open a video file instead
    if ( argc == 1 ) {
        cap.open( 0, cv::CAP_V4L );  // Open the first camera
        cout << "Streaming from camera. Press <esc> key to exit.\n";
    } else {
        cap.open( argv[1] );
        cout << "Reading video file.  Press <esc> key to exit.\n";
        video_flag = 1;
    }
    if ( !cap.isOpened() ) {
        cerr << "Couldn't open the capture.\n";
        return -1;
    }

    int frame_width = (int) cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = (int) cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    cout << "Video Resolution is: (" << frame_width << ", " << frame_height << ")\n";
    if ( video_flag == 1) {
        int frames = (int) cap.get(cv::CAP_PROP_FRAME_COUNT);
        cout << "Number of frames in video is: " << frames << "\n";
    }

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if( frame.empty() ) break;
        cv::imshow( window_name, frame );
        char c = (char) cv::waitKey(10);
        if( c == 27 ){
            cout << "Esc key entered.  Exiting ... \n";
            break;
        }
    }
    cap.release();
    return 0;
}