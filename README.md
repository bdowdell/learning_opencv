# Learning OpenCV

This repository is where I experiment with OpenCV, initially in C++

OpenCV build: 4.3.0

Makefile compiles each .cpp to its own individual .o and then links to its own individual binary.

Using **Learning OpenCV 3**, *Adrian Kaehler and Gary Bradski* as a reference.

Source files beginning with "ex_" are examples from the text, where the first number following is the chapter number.

Source files beginning with "xrs_" are exercises from the text, where the first number following is the chapter number.

[src/obj_detect_yolov3.cpp](src/obj_detect_yolov3.cpp) uses OpenCV's DNN backend for YOLOv3 object detection.

Here is a still frame from a video I took of my wife juggling a soccer ball in our backyard.

The first image is as recorded from the video:

![Raw_Image](doc/kim_soccer_converted_still.jpg "Raw Image")

And the second image is after running it through the pretrained [YOLOv3](https://pjreddie.com/darknet/yolo/)

![Processed_Image](doc/kim_soccer_yolo_out_cpp_still.jpg "Processed Image")
