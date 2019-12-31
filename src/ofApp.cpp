#include "ofApp.h"

using namespace std;
using namespace dlib;

//--------------------------------------------------------------
void ofApp::setup(){
    
    video.setDesiredFrameRate(30);
    video.setup(640, 480);
    
    //load trained detectors from data/bin directory
    //you can also use the left_hand.svm/right_hand.svm's if you want to differentiate
    //between left and right hands.
    deserialize(ofToDataPath("face_detector.svm", true)) >> detector1;
    deserialize(ofToDataPath("face_detector2.svm", true)) >> detector2;
    deserialize(ofToDataPath("hand_detector.svm", true)) >> detector3;
    deserialize(ofToDataPath("hand_detector2.svm", true)) >> detector4;
    
    //push trained detectors into vector
    my_detectors.push_back(detector1);
    my_detectors.push_back(detector2);
    my_detectors.push_back(detector3);
    my_detectors.push_back(detector4);
    
    //convert fhog gradient images from detectors into ofPixels
    fhogImage1 = ofxDlib::toOf(draw_fhog(detector1));
    fhogImage2 = ofxDlib::toOf(draw_fhog(detector2));
    fhogImage3 = ofxDlib::toOf(draw_fhog(detector3));
    fhogImage4 = ofxDlib::toOf(draw_fhog(detector4));
    
    //load gradient images into textures
    fhogTex1.loadData(fhogImage1);
    fhogTex2.loadData(fhogImage2);
    fhogTex3.loadData(fhogImage3);
    fhogTex4.loadData(fhogImage4);
    
    //bool for displaying gradient video
    gradientVid = false;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //setup video
    video.update();
    if (video.isFrameNew()){
        //run all detectors on video input && store detections
        // i.e. (rectangle coordinates) in allDets vector.
        evaluate_detectors(my_detectors, video.getPixels(), allDets);
       
        //load video pixels into texture for displaying later
        videoTex.loadData(video.getPixels());

        //if key pressed get fhog gradient image from current video frame and store in fhogVidFrame
        if (ofGetKeyPressed(' ')){
            array2d<matrix<float,31,1> > hog;
            extract_fhog_features(video.getPixels(), hog);
            fhogVidFrame = ofxDlib::toOf(draw_fhog(hog));
            //load fhog gradient image of video into texture
            fhogVidTex.loadData(fhogVidFrame);
            //set bool to true for drawing gradient texture
            gradientVid = true;
        }
        
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    ofNoFill();
    ofSetColor(ofColor::white);
    
    //flip video so it is mirrored
    videoTex.draw(video.getWidth(), 0, -video.getWidth(), video.getHeight());
    
    ofPushMatrix();
    
    //iterate through all the detections from each detector.
    for (auto& allDets: allDets){
        
        //find which detector each rectangle came from by checking the weight_index.
        if (allDets.weight_index == 0){
            //color & draw truth rects for detector1
            ofSetColor(ofColor::yellow);
            ofRectangle rect = ofxDlib::toOf(allDets.rect);
            //adjust x position and rectangle width to match inverted video frame/draw rectangle
            ofDrawRectangle(video.getWidth() - rect.x, rect.y, -rect.width, rect.height);
            
        } else if (allDets.weight_index == 1){
            //color & draw truth rects for detector2
            ofSetColor(ofColor::red);
            ofRectangle rect = ofxDlib::toOf(allDets.rect);
            ofDrawRectangle(video.getWidth() - rect.x, rect.y, -rect.width, rect.height);
            // print confidence score at adjusted x y coordinates for truth rect
            ofDrawBitmapString("Face Score: " + ofToString(allDets.detection_confidence, 2), video.getWidth() - rect.x - rect.width, rect.y - 3);
            
        } else if (allDets.weight_index == 2){
            //color & draw truth rects for detector3
            ofSetColor(ofColor::blue);
            ofRectangle rect = ofxDlib::toOf(allDets.rect);
            ofDrawRectangle(video.getWidth() - rect.x, rect.y, -rect.width, rect.height);
            //print confidence scores
            ofDrawBitmapString("Hand Score: " + ofToString(allDets.detection_confidence, 2), video.getWidth() - rect.x -rect.width, rect.y - 3);
            
        } else if (allDets.weight_index == 3){
            //color & draw truth rects for detector4
            ofSetColor(ofColor::green);
            ofRectangle rect = ofxDlib::toOf(allDets.rect);
            ofDrawRectangle(video.getWidth() - rect.x, rect.y, -rect.width, rect.height);
            
        }
        
    }
    
    ofPopMatrix();
    
    //draw, display, and color gradient textures from hand && face detectors
    ofPushMatrix();
    ofSetColor(ofColor::white);
    for (auto& allDets: allDets){
        if (allDets.weight_index == 0){
            ofSetColor(ofColor::yellow);
        }
    }
    fhogTex1.draw(video.getWidth(), 0, video.getHeight()/4, video.getHeight()/4);
    ofPopMatrix();
    
    ofPushMatrix();
    ofSetColor(ofColor::white);
    for (auto& allDets: allDets){
        if (allDets.weight_index == 1){
            ofSetColor(ofColor::red);
        }
    }
    fhogTex2.draw(video.getWidth(), video.getHeight()/4, video.getHeight()/4, video.getHeight()/4);
    ofPopMatrix();
    
    ofPushMatrix();
    ofSetColor(ofColor::white);
    for (auto& allDets: allDets){
        if (allDets.weight_index == 2){
            ofSetColor(ofColor::blue);
        }
    }
    fhogTex3.draw(video.getWidth(), video.getHeight()/2 , video.getHeight()/4, video.getHeight()/4);
    ofPopMatrix();
    
    ofPushMatrix();
    ofSetColor(ofColor::white);
    for (auto& allDets: allDets){
        if (allDets.weight_index == 3){
            ofSetColor(ofColor::green);
        }
    }
    fhogTex4.draw(video.getWidth(), video.getHeight() - video.getHeight()/4, video.getHeight()/4, video.getHeight()/4);
    ofPopMatrix();
    
    //display fhog gradient from video feed
    if (gradientVid){
        
    ofSetColor(ofColor::white);
    fhogVidTex.draw(video.getWidth() + video.getHeight()/4 + video.getWidth()/1.3f, 0, -video.getWidth()/1.3f, video.getHeight()/1.3f);
        
    }
    
    //alternate display of gradient from video feed
    //    ofSetColor(ofColor::white);
    //    fhogVidTex.draw(video.getWidth() - (video.getWidth() - video.getWidth()/1.5f), video.getHeight(), -video.getWidth()/1.5f, video.getHeight()/1.5f);
 
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //clear gradient video texture
    if (key == 'c'){
        gradientVid = !gradientVid;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
