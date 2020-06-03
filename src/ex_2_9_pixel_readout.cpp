#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

int main( int argc, char** argv ){
    // check for two command line arguments
    if( argc != 2){
        cout << "Expected two command line arguments.\n";
        cout << "Received " << argc << " argument(s).\n";
        cout << "Usage: " << argv[0] << " <filename>\n";
        cout << "Exiting ... \n";
        return -1;
    }

    // Create Mat objects to hold the images
    cv::Mat img_rgb, img_gry, img_pyr1, img_pyr2, img_cny;

    // Define windows to show
    // 1) The input image
    string window_in = "Example 2-8: Input RGB";
    cv::namedWindow( window_in, cv::WINDOW_AUTOSIZE );
    // 2) The grey-scale converted image
    string window_gry = "Example 2-8: GRAY Scale";
    cv::namedWindow( window_gry, cv::WINDOW_AUTOSIZE );
    // 3) The second pyrdown image
    string window_pyr2 = "Example 2-8: Second Pyramid Down Transform";
    cv::namedWindow( window_pyr2, cv::WINDOW_AUTOSIZE );
    // 4) The Canny-transformed image
    string window_cny = "Example 2-8: Canny Transform Output";
    cv::namedWindow( window_cny, cv::WINDOW_AUTOSIZE );

    // Read in the input image
    img_rgb = cv::imread( argv[1] );
    // Get the width of the input image
    int img_width = img_rgb.size().width;

    // Convert img_rgb to grayscale
    cv::cvtColor( img_rgb, img_gry, cv::COLOR_BGR2GRAY );

    // run two passes of pyrDown() transform
    cv::pyrDown( img_gry, img_pyr1 );
    cv::pyrDown( img_pyr1, img_pyr2 );

    // Apply Canny edge detector transform
    cv::Canny( img_pyr2, img_cny, 10, 100, 3, true );

    // set the initial window position
    int win_x = 500;
    int win_y = 300;
    cv::moveWindow( window_in, win_x, win_y );
    // Place the gray-scale converted window
    cv::moveWindow( window_gry, win_x + img_width + 20, win_y );
    // place window for pyr2
    cv::moveWindow( window_pyr2, (win_x + 2*(img_width + 20)), win_y );
    // Get new image width of scaled image
    int pyr2_width = img_pyr2.size().width;
    // place Canny window
    cv::moveWindow( window_cny, (win_x + 2*(img_width + 20) + (pyr2_width + 20)), win_y );

    // Get pixel read-outs for specified X,Y
    int x = 16, y = 32;
    cv::Vec3b intensity = img_rgb.at< cv::Vec3b >(y, x);
    uchar blue = intensity[0];
    uchar green = intensity[1];
    uchar red = intensity[2];

    cout << "At (x,y) = (" << x << ", " << y << "): (blue,green,red) = (" << (unsigned int)blue << ", " << (unsigned int)green << ", " << (unsigned int)red << ")\n";
    cout << "Grey pixel there is : " << (unsigned int)img_gry.at<uchar>(y, x) << "\n";
    x /= 4;
    y /= 4;
    cout << "Pyramid2 pixel there is: " << (unsigned int)img_pyr2.at<uchar>(y, x) << "\n";

    // Change the pixel value in the Canny transformed image
    img_cny.at<uchar>(y, x) = 128;
    // Change the pixel value in the original RGB image to denote it's location
    x = 16;
    y = 32;
    img_rgb.at<uchar>(y, x) = 128;
    // Display the referenced pixel in the gray scale image
    img_gry.at<uchar>(y, x) = 128;
    // Display the referenced pixel in the PyramidDown 2 image
    img_pyr2.at<uchar>(y/=4, x/=4) = 128;

    // show the windows
    cv::imshow( window_in, img_rgb );
    cv::imshow( window_gry, img_gry );
    cv::imshow( window_pyr2, img_pyr2 );
    cv::imshow( window_cny, img_cny );

    // Wait for user input to continue
    cout << "Press any key to continue ... \n";
    cv::waitKey(0);

    return 0;
}