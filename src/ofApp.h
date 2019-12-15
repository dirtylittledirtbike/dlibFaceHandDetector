#pragma once

#include "ofMain.h"
#include "ofxDlib.h"
#include <dlib/svm_threaded.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>

#include <iostream>
#include <fstream>

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    
    //define the architecture for the detectors
    typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6> > image_scanner_type;
    
    //create dlib objects for multiple detectors
    dlib::object_detector<image_scanner_type> detector1;
    dlib::object_detector<image_scanner_type> detector2;
    dlib::object_detector<image_scanner_type> detector3;
    dlib::object_detector<image_scanner_type> detector4;
    
    //create a vector to store multiple detectors
    std::vector<dlib::object_detector<image_scanner_type> > my_detectors;
    // or you could write: std::vector<dlib::object_detector<dlib::scan_fhog_pyramid<dlib::pyramid_down<6>>>> my_detectors;
    //create vector to store detectons from detectors
    std::vector<dlib::rect_detection> allDets;
    
    //image video and texture stuff for loading and displaying
    ofVideoGrabber video;
    ofPixels pixels;
    ofTexture videoTex;
    ofPixels fhogImage1;
    ofPixels fhogImage2;
    ofPixels fhogImage3;
    ofPixels fhogImage4;
    ofTexture fhogTex1;
    ofTexture fhogTex2;
    ofTexture fhogTex3;
    ofTexture fhogTex4;
    
    ofPixels fhogVidFrame;
    ofTexture fhogVidTex;
    
    
};
