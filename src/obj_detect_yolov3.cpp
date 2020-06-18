// experimenting with object detection using YOLOv3
// based on https://github.com/spmallick/learnopencv/blob/master/ObjectDetection-YOLO/object_detection_yolo.cpp
// Usage examples:  ./obj_detect_yolov3 --video=run.mp4
//                  ./obj_detect_yolov3 --image=bird.jpg
// std headers
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
// opencv headers
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

// GLOBAL VARIABLES
const char* g_keys =
"{help h | | Usage examples: \n\t\t./obj_detect_yolov3 --image=dog.jpg \n\t\t./obj_detect_yolov3 --video=run_sm.mp4}"
"{image i        |<none>| input image   }"
"{video v        |<none>| input video   }"
;

float g_confThreshold = 0.5; // Confidence threshold
float g_nmsThreshold = 0.4; // Non-maximum supression threshold
int g_inpWidth = 416; // Width of network's input image
int g_inpHeight = 416; // Height of network's input image
vector<string> g_classes; // Classes from coco.names
// END GLOBAL VARIABLES

// FUNCTION PROTOTYPES
// Remove the bounding boxes with low confidence using NMS
void postprocess(cv::Mat& frame, const vector<cv::Mat>& out);

// Draw the predicted bounding box
void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame);

// Get the names of the output layers
vector<string> getOutputNames(const cv::dnn::Net& net);

// MAIN
int main( int argc, char** argv ){

    // Command Line Parser for input arguments
    cv::CommandLineParser parser(argc, argv, g_keys);
    parser.about("Use this script to run object detections using YOLOv3 in OpenCV.\n");
    if( parser.has("help")){
        parser.printMessage();
        return 0;
    }

    // Open a video file or an image file or a camera stream
    string str, outputFile;
    cv::VideoCapture cap;
    cv::VideoWriter writer;
    cv::Mat frame, blob;
    string win_in = "Input image";
    cv::namedWindow(win_in, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(win_in, 500, 300);

    int camera_mode = 0;

    // Try loading the image / video / camera using the command line parser
    try {
        if(parser.has("image")){
            // Open the image file
            str = parser.get<cv::String>("image");
            cap.open(str);
            if(!cap.isOpened()) throw("error");
            str.replace(str.end()-4, str.end(), "_yolo_out_cpp.avi");
            outputFile = str;
        } else if(parser.has("video")){
            // Open the video file
            str = parser.get<cv::String>("video");
            cap.open(str);
            if(!cap.isOpened()) throw("error");
            str.replace(str.end()-4, str.end(), "_yolo_out_cpp.avi");
            outputFile = str;
        } else {
            // Open the webcam, assume device = 0
            cap.open(0, cv::CAP_V4L);
            if(!cap.isOpened()) throw("error");
            outputFile = "cam_yolo_output.avi";
            camera_mode = 1;
        }
    } catch(...){
        cout << "Could not open the input image/video stream\n";
        return 0;
    }

    // load names of classes
    string classesFile = "/home/bendowdell/github/external_repositories/darknet/data/coco.names";
    ifstream ifs(classesFile.c_str());
    string line;
    int num_classes = 0;
    cout << "Loading class labels from: " << classesFile << endl;
    while (getline(ifs, line)){
        g_classes.push_back(line);
        num_classes++;
    }
    cout << "Total number of class labels: " << num_classes << "\n";

    if( camera_mode == 1) cout << "To exit, press <esc> key.\n";
    for(;;) {
        // get the current frame
        cap >> frame;
        // stop processing if end of stream
        if( frame.empty() ){
            cout << "\nEnd of file\n";
            cout << "Output file is stored as " << outputFile << "\n";
            cout << "Press any key to exit.\n";
            cv::waitKey(0);
            break;
        }
        cv::imshow(win_in, frame);
        char c = (char) cv::waitKey(10);
        if( c == 27 ) break;
    }
    return 0;
}