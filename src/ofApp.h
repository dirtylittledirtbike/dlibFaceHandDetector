#pragma once

#include "ofMain.h"
#include "ofxDlib.h"

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
    dlib::object_detector<image_scanner_type> detector1, detector2, detector3, detector4;
    
    //create a vector to store multiple detectors
    std::vector<dlib::object_detector<image_scanner_type> > my_detectors;
    // or you could write: std::vector<dlib::object_detector<dlib::scan_fhog_pyramid<dlib::pyramid_down<6>>>> my_detectors;
    //create vector to store detectons from detectors
    std::vector<dlib::rect_detection> allDets;
    
    //image video and texture stuff for loading and displaying
    ofVideoGrabber video;
    ofPixels pixels, fhogVidFrame;
    ofImage fhogImage1, fhogImage2, fhogImage3, fhogImage4;
    ofTexture fhogTex1, fhogTex2, fhogTex3, fhogTex4, videoTex, fhogVidTex;
    
    bool gradientVid;
    
};
