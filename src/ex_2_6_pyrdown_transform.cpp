#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main( int argc, char** argv ){
    // First check that two command line args are passed
    if ( argc != 2 ){
        cout << "Error! Expected two (2) command line arguments.\n";
        cout << "Received " << argc << " arguments.\n";
        cout << "Usage: " << argv[0] << " <filename>.\n";
        cout << "Exiting ...\n";
        return -1;
    }

    // Initialize two Mat objects for the input & output images
    Mat img1, img2;

    // Initialize two namedWindows
    string window_in = "Example 2-6 In";
    namedWindow( window_in, WINDOW_AUTOSIZE );
    moveWindow( window_in, 900, 200 ) ;
    string window_out = "Example 2-6 Out";
    namedWindow( window_out, WINDOW_AUTOSIZE );
    moveWindow( window_out, 900, 500 );

    // read the input image
    img1 = imread( argv[1] );
    // check the image was read in
    if( img1.empty() ){
        cout << "No image to display.  Exiting ... \n";
        return -1;
    }
    // display the input image
    imshow( window_in, img1 );

    // scale down the image using pyrDown()
    pyrDown( img1, img2 );
    imshow( window_out, img2 );

    // wait for user response
    cout << "Press any key to continue ... \n"; // prompt user to press a key to continue (end)
    waitKey(0);

    return 0;
}