// experimenting with object detection using YOLOv3
// based on https://github.com/spmallick/learnopencv/blob/master/ObjectDetection-YOLO/object_detection_yolo.cpp
// This code is written at BigVision LLC. It is based on the OpenCV project. It is subject to the license terms in the LICENSE file found in this distribution and at http://opencv.org/license.html
// modifications by Ben Dowdell
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
"{iphone p       |<none>| device iphone }"
;

float g_confThreshold = 0.5; // Confidence threshold
float g_nmsThreshold = 0.4; // Non-maximum supression threshold
int g_inpWidth = 416; // Width of network's input image
int g_inpHeight = 416; // Height of network's input image
vector<string> g_classes; // Classes from coco.names
// END GLOBAL VARIABLES

// FUNCTION PROTOTYPES
// Remove the bounding boxes with low confidence using NMS
void postprocess(cv::Mat& frame, const vector<cv::Mat>& outs);

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
    string win_in = "Object Detection using OpenCV & YOLOv3";
    cv::namedWindow(win_in, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(win_in, 300, 300);

    int camera_mode = 0;

    // Try loading the image / video / camera using the command line parser
    try {
        if(parser.has("image")){
            // Open the image file
            str = parser.get<cv::String>("image");
            cap.open(str);
            if(!cap.isOpened()) throw("error");
            str.replace(str.end()-4, str.end(), "_yolo_out_cpp.jpg");
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

    // load YOLOv3 configuration and weights
    string model_config = "/home/bendowdell/github/external_repositories/darknet/cfg/yolov3.cfg";
    string model_wts = "/home/bendowdell/github/external_repositories/darknet/yolov3.weights";
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(model_config, model_wts);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // Initialize the video writer if the input format is not an image
    if(!parser.has("image")){
        writer.open(outputFile, cv::VideoWriter::fourcc('M','J','P','G'), cap.get(cv::CAP_PROP_FPS), 
        cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));
    }

    if( camera_mode == 1) cout << "To exit, press <esc> key.\n";

    // initialize variables to get average time to process a frame
    double t_total = 0;
    double t_ave = 0;
    int frames = (int) cap.get(cv::CAP_PROP_FRAME_COUNT);
    // process frames
    for(;;) {
        // get the current frame
        cap >> frame;

        if( parser.has("iphone")){
            cv::flip(frame, frame, 0); // flip video around x-axis for iPhone input
            cv::flip(frame, frame, 1); // flip video around y-axis for iPhone input
        }

        // stop processing if end of stream
        if( frame.empty() ){
            cout << "\nEnd of file\n";
            cout << "Output file is stored as " << outputFile << "\n";
            cout << "Press any key to exit.\n";
            cv::waitKey(0);
            break;
        }
        
        // Create a 4D blob from the frame
        cv::dnn::blobFromImage(frame, blob, 1/255.0, cv::Size(g_inpWidth, g_inpHeight), 
                               cv::Scalar(0,0,0), true, false);
        
        // Pass the current frame to the network
        net.setInput(blob);

        // Runs the forward pass to get output of the output layers
        vector<cv::Mat> outs;
        net.forward(outs, getOutputNames(net));

        // Remove the bounding boxes with low confidence
        postprocess(frame, outs);

        // Include efficiency information
        vector<double> layersTimes;
        double freq = cv::getTickFrequency() / 1000;
        double t = net.getPerfProfile(layersTimes) / freq;
        string label = cv::format("Inference time for a frame: %.2f ms", t);
        cv::putText(frame, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));
        t_total += t;

        // Write the frame with the detection boxes
        cv::Mat detected_frame;
        frame.convertTo(detected_frame, CV_8U);
        if ( parser.has("image") ) cv::imwrite(outputFile, detected_frame);
        else writer.write(detected_frame);

        cv::imshow(win_in, frame);
        char c = (char) cv::waitKey(10);
        if( c == 27 ) break;
    }

    // clean up
    cap.release();
    if(!parser.has("image")) writer.release();

    t_ave = t_total/frames;
    string ave_time = cv::format("%.2f ms", t_ave);
    cout << "Average time to process a frame: " << ave_time << endl;


    return 0;
}

// Remove the bounding boxes with low confidence using NMS
void postprocess(cv::Mat& frame, const vector<cv::Mat>& outs){
    vector<int> class_ids;
    vector<float> confidences;
    vector<cv::Rect> boxes;

    for(size_t i=0; i<outs.size(); ++i){
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores.  Assign the box's class label as the class
        // with the highest score for the box.
        float* data = (float*) outs[i].data;
        for (int j=0; j<outs[i].rows; ++j, data += outs[i].cols){
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point class_id_point;
            double confidence;
            // Get the value and location of the maximum score
            cv::minMaxLoc(scores, 0, &confidence, 0, &class_id_point);
            if ( confidence > g_confThreshold){
                int center_x = (int) (data[0] * frame.cols);
                int center_y = (int) (data[1] * frame.rows);
                int width = (int) (data[2] * frame.cols);
                int height = (int) (data[3] * frame.rows);
                int left = center_x - width / 2;
                int top = center_y - height / 2;
                class_ids.push_back(class_id_point.x);
                confidences.push_back((float)confidence);
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
    }

    // Pefrom non maximum suppression to eliminate redundant overlapping boxes with lower confidence
    vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, g_confThreshold, g_nmsThreshold, indices);
    for( size_t i=0; i<indices.size(); ++i){
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        drawPred(class_ids[idx], confidences[idx], box.x, box.y, box.x+box.width, box.y+box.height, frame);
    }

}

// Draw the predicted bounding box
void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame){
    // Draw a rectangle displaying the bounding box
    cv::rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(255, 178, 50), 2);

    // Get the label for the class name and its confidence
    string label = cv::format("%.2f", conf);
    if(!g_classes.empty()){
        CV_Assert(classId < (int) g_classes.size());
        label = g_classes[classId] + ":" + label;
    }

    // Display the label at the top of the bounding box
    int baseline;
    cv::Size label_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);
    top = cv::max(top, label_size.height);
    cv::rectangle(frame, cv::Point(left, top - round(1.5*label_size.height)), cv::Point(left+ round(1.5*label_size.width), top+baseline), cv::Scalar(255,255,255), cv::FILLED);
    cv::putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,0), 1);
    
}

// Get the names of the output layers
vector<string> getOutputNames(const cv::dnn::Net& net){
    static vector<string> names;
    if (names.empty()){
        // Get the indices of the output layers, i.e. the layers with unconnected outputs
        vector<int> out_layers = net.getUnconnectedOutLayers();

        // get the names of all the layers in the network
        vector<string> layers_names = net.getLayerNames();

        // Get the names of the output layers in names
        names.resize(out_layers.size());
        for(size_t i=0; i<out_layers.size(); ++i){
            names[i] = layers_names[out_layers[i] - 1];
        }
    }
    return names;
}