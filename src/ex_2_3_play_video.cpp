#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main( int argc, char** argv ){
    // Check the number of inputs args are correct
    // Expect two args: program name & video file
    if ( argc != 2 ){
        cout << "Expected two (2) command line arguments.\n";
        cout << "Proper usage: " << argv[0] << " " << "<filename>\n";
        cout << "Exiting ... \n";
        return -1;
    }

    namedWindow( "Example 2-3", WINDOW_AUTOSIZE );
    VideoCapture cap;
    cap.open( string(argv[1]) );

    Mat frame;
    for(;;) {
        cap >> frame;
        if( frame.empty() ) break;
        imshow( "Example 2-3", frame );
        if( (char) waitKey(33) >= 0 ) break;
    }

    return 0;
}