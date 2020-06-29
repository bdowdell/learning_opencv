#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>

using namespace std;

int main( int argc, char** argv ){
    // Check that 2 args are passed
    if( argc != 2 ){
        cout << "Error.  Expected 2 command line arguments, but received " << argc << endl;
        cout << "Usage: " << argv[0] << " <filename>\n";
        cout << "Exiting ...\n";
        return -1;
    }

    cv::VideoCapture cap;
    cv::Mat frame;
    cv::VideoWriter writer;

    cap.open(argv[1]);

    string outfile_name = argv[1];
    int pos = outfile_name.find_first_of(".");
    outfile_name = outfile_name.substr(0,pos);
    outfile_name.append("_converted.avi");

    cout << "FPS: " << cap.get(cv::CAP_PROP_FPS) << endl;
    cout << "Width: " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << endl;
    cout << "Height: " << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << endl;

    writer.open(outfile_name, cv::VideoWriter::fourcc('M','J','P','G'), cap.get(cv::CAP_PROP_FPS),
                cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

    for(;;){
        cap >> frame;
        // iPhone video needs to be flipped along both axis
        cv::flip(frame, frame, -1);
        if( frame.empty() ){
            cout << "Conversion complete.  Outputfile located: " << outfile_name << endl;
            break;
        }
        writer.write(frame);
    }
    cap.release();
    return 0;
}