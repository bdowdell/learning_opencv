#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

int main( int argc, char** argv ){
    // Check for two command line arguments
    // program name & file name
    if( argc != 2){
        cout << "Expected two (2) command line arguments.\n";
        cout << "Received " << argc << " command line args.\n";
        cout << "Usage: " << argv[0] << " <filename>\n";
        cout << "Exiting ... \n";
        return -1; // terminate program
    }

    // Load an image from the command line
    Mat image = imread(argv[1], -1);

    // Create a window for input & output
    string in_window = "Example 2-5 IN";
    string out_window = "Example 2-5 OUT";
    namedWindow( in_window, WINDOW_AUTOSIZE );
    moveWindow( in_window, 900, 100 ); // adjust window position
    namedWindow( out_window, WINDOW_AUTOSIZE );
    moveWindow( out_window, 900, 500 ); // adjust window position

    // Show our input image
    imshow( in_window, image );

    // Create a matrix image to hold the smoothed output
    Mat out;

    // Smooth the image using a GaussianBlur();
    // Other options include: Blur(), medianBlur(), & bilateralFilter()
    GaussianBlur( image, out, Size(5,5), 3, 3 );
    GaussianBlur( out, out, Size(5,5), 3, 3 );

    // Show the smoothed image
    imshow( out_window, out );

    // Wait for the user to press a key
    waitKey(0);
}