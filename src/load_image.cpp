#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// OpenCV Tutorial Load & Display Image
// https://www.opencv-srf.com/2017/11/load-and-display-image.html
int main( int argc, char** argv ) {
    // check that the number of command line args is correct
    // expect 2 (program name & file to load)
    if ( argc != 2 ) {
        cout << "Expected two command line arguments.\n";
        cout << "Proper usage: " << argv[0] << " " << "<filename>\n";
        cout << "Exiting ...\n";
        return -1;
    }

    //Read in an image from the command line
    Mat img = imread( argv[1] );

    //Check for failure
    if (img.empty())
    {
        cout << "Could not open or find the image" << endl;
        cin.get();
        return -1;
    }

    String windowName = "Viewer";
    namedWindow(windowName);
    imshow(windowName, img);
    waitKey(0);
    destroyWindow(windowName);

    return 0;
}