#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream> //required for filestream buffer io
#include <string>

using namespace std;
using namespace cv;

// define global variables
int g_slider_position = 0; //Initialize trackbar slider at 0
int g_run = 1, g_dontset = 0; //Start in single-step mode
VideoCapture g_cap;

//Callback function for updating trackbar position
void onTrackbarSlide( int pos, void * ){
    g_cap.set(CAP_PROP_POS_FRAMES, pos);
    if ( !g_dontset ) g_run = 1;
    g_dontset = 0;
}

int main( int argc, char** argv ) {
    //Check that the number of command line arguments is correct
    //Expect two (2) args
    if ( argc != 2){
        cout << "Expected two arguments.\n";
        cout << "Usage: " << argv[0] << " <filename\n";
        cout << "Exiting ...\n";
        return -1;
    }
    
    // Create the window for displaying the video
    string window_name = "Example 2-4";
    namedWindow( window_name, WINDOW_AUTOSIZE );
    moveWindow(window_name, 900, 200);

    g_cap.open( string(argv[1]) );
    // get the frame count
    int frames = (int) g_cap.get(CAP_PROP_FRAME_COUNT);
    // get the frame width & height dimensions
    int tmpw = (int) g_cap.get(CAP_PROP_FRAME_WIDTH);
    int tmph = (int) g_cap.get(CAP_PROP_FRAME_HEIGHT);
    // output basic details of the video
    cout << "Video has " << frames << " frames of dimensions(" << tmpw << ", " << tmph << ").\n";
    
    // Create the trackback
    createTrackbar("Position", window_name, &g_slider_position, frames, onTrackbarSlide);

    // Create the matrix to hold each frame
    Mat frame;

    // Loop through the frames of the capture
    for(;;) {

        if( g_run != 0 ) {
            g_cap >> frame; if(frame.empty()) break;
            int current_pos = (int) g_cap.get(CAP_PROP_POS_FRAMES);
            g_dontset = 1;

            setTrackbarPos("Position", window_name, current_pos);
            imshow( window_name, frame );
            //cout << "Current Frame Position: " << current_pos + 1 << " of " << frames << ".\n";

            g_run -= 1;
        }

        // get user input to adjust run mode or exit
        char c = (char) waitKey(80);
        if( c == 's' ) {
            // single step mode
            g_run = 1;
            cout << "Single step, run = " << g_run << endl;
        }
        if( c == 'r' ) {
             // run mode
            g_run = -1; 
            cout << "Run mode, run = " << g_run << endl;
        }
        if( c == 27 )
            break; // escape key hit
    }
    return(0);
}