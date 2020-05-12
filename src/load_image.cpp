#include <opencv2/highgui.hpp>

using namespace cv;

int main( int argc, char** argv ) {
    //Read in an image from the command line
    Mat img = imread( argv[1] );
    return 0;
}