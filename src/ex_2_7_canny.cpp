#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

int main( int argc, char** argv ){
    // check for two command line arguments
    if( argc != 2) {
        cout << "Expected two command line arguments.\n";
        cout << "Received " << argc << " argument(s).\n";
        cout << "Usage: " << argv[0] << " <filename>\n";
        cout << "Exiting ... \n";
        return -1;
    }

    // Create Mat objects to hold images
    cv::Mat img_rgb, img_gry, img_cny;

    // Define the named windows to hold the gray-scaled converted image
    // and the Canny edge-detector image
    string window_gry = "Example Gray";
    cv::namedWindow( window_gry, cv::WINDOW_AUTOSIZE );
    string window_cny = "Example Canny";
    cv::namedWindow( window_cny, cv::WINDOW_AUTOSIZE );

    // Read in the input image
    img_rgb = cv::imread( argv[1] );
    int img_width = img_rgb.size().width;

    // move the windows using the image dimensions
    int window_x = 700;
    int window_y = 300;
    cv::moveWindow( window_gry, window_x, window_y );
    cv::moveWindow( window_cny, (window_x + img_width) + 20, window_y );

    // Convert the RGB image to Gray Scale
    cv::cvtColor( img_rgb, img_gry, cv::COLOR_BGR2GRAY );
    // Show the gray-scale converted image
    cv::imshow( window_gry, img_gry );

    // Call Canny edge detector
    cv::Canny( img_gry, img_cny, 10, 100, 3, true );
    // Show the Canny edge detector image
    cv::imshow( window_cny, img_cny );

    // Wait for user input to terminate
    cout << "Press any key to continue ...\n";
    cv::waitKey(0);

    return 0;
}