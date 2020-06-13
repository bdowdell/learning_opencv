/*
Create a mask using cv::compare().  Load a real image.  Use cv::split() to split the image into red, green, and blue images.
1. Find and display the green image.
2. Clone this green plane image twice (call these clone1 and clone2).
3. Find the green plane's minimum and maximum value.
4. Set clone1's values to thresh = (unsigned char) ((maximum - minimum)/2.0)
5. Set clone2 to 0 and use cv::compare(green_image, clone1, clone2, cv::CMP_GE).  Now clone2 will have a mask of where the values exceeds thresh in the green image.
6. Finally, use cv::subtract(green_image, thresh/2, green_image, clone2) and display the image.
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// look-up function to return string format of integer channel depth
string lookupDepth(int depth){
    switch(depth){
        case 0:
            return "CV_8U";
        case 1:
            return "CV_8S";
        case 2:
            return "CV_16U";
        case 3:
            return "CV_16S";
        case 4:
            return "CV_32S";
        case 5:
            return "CV_32F";
        case 6:
            return "CV_64F";
        default:
            return "";
    }
}

// recursivePyrDown() recursively downsamples a frame where scale is the number of recursive passes of pyrDown()
cv::Mat recursivePyrDown(cv::Mat inframe, int scale) {
    if ( scale == 1 ) {
        cv::Mat outframe;
        cv::pyrDown( inframe, outframe );
        return outframe;
    } else {
        cv::Mat processed_frame;
        cv::pyrDown( recursivePyrDown( inframe, --scale ), processed_frame );
        return processed_frame;
    }
}

int main( int argc, char** argv ){
    // check the correct number of args are passed.  expect two.
    if( argc != 2 ){
        cout << "Error.  Expected two command line arguments, but received: " << argc << "\n";
        cout << "Usage: " << argv[0] << " <filename>\n";
        cout << "Exiting ...\n";
        return -1;
    }

    // read in an image and split it into three channels (bgr)
    cv::Mat img = cv::imread(argv[1]);

    // if img width is bigger than 512 use pyrDown to resize
    if( img.size().width > 512 ) {
        cout << "Input image size: " << img.size() << "\n";
        int num_passes = sqrt( img.size().width / 512 ) + 1;
        img = recursivePyrDown(img, num_passes);
        cout << "Rescaled image with " << num_passes << " passes of pyramid down\n";
    }

    cout << "IMG:\n"
         << "\tdepth: " << lookupDepth(img.depth()) << "\n"
         << "\tchannels: " << img.channels() << "\n"
         << "\tsize: " << img.size() << "\n"; 
    cv::Mat channels[3];
    cv::split(img, channels);

    // create windows to hold the input image and the green channel of the split image
    string in_window = "Input";
    string green_window = "Green channel";
    string green_thresh_window = "Green Threshold Mask";
    cv::namedWindow(in_window, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(in_window, 500, 150);
    cv::namedWindow(green_window, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(green_window, 500+img.size().width, 150);
    cv::namedWindow(green_thresh_window, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(green_thresh_window, 500+2*img.size().width,150);

    // display the input image and the green channel
    cv::imshow(in_window, img);
    cv::imshow(green_window, channels[1]);

    // make two clones of the green channel for further manipulation
    cv::Mat clone1 = channels[1].clone();
    cout << "clone1:\n"
         << "\tdepth: " << lookupDepth(clone1.depth()) << "\n"
         << "\tchannels: " << clone1.channels() << "\n"
         << "\tsize: " << clone1.size() << "\n"; 
    cv::Mat clone2 = channels[1].clone();
    cout << "clone2:\n"
         << "\tdepth: " << lookupDepth(clone2.depth()) << "\n"
         << "\tchannels: " << clone2.channels() << "\n"
         << "\tsize: " << clone2.size() << "\n";

    // get minimum and maximum values for green channel
    double min, max;
    cv::Point minLoc, maxLoc; // we don't really need to get the (X,Y)'s but we will do it for practice
    cv::minMaxLoc(channels[1], &min, &max, &minLoc, &maxLoc);
    cout << "Green channel minimum value: " << min << " @ " << minLoc << "\n";
    cout << "Green channel maximum value: " << max << " @ " << maxLoc << "\n";

    // set clone1's threshold
    int thresh = (unsigned char) ((max - min) / 2.0);
    cout << "Threshold value = " << thresh << "\n";
    clone1.setTo(thresh);

    // convert clone2 to a mask for where green channel amps are above thresh
    clone2.setTo(0);
    cv::compare(channels[1], clone1, clone2, cv::CMP_GE); // cv::CMP_GE src1 >= src2

    // apply the threshold mask to the green channel
    cv::subtract(channels[1], thresh/2, channels[1], clone2);
    cv::imshow(green_thresh_window, channels[1]);

    cout << "press any key to exit ...\n";
    cv::waitKey(0);

    return 0;
}