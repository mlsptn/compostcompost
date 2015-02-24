#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    w = ofGetScreenWidth(); h = ofGetScreenHeight();
    ofSetWindowShape(w, h);
    ofSetFullscreen(true);
    
    compostW = 800; compostH = 600;
    compostPos = ofVec2f(w/2 - compostW/2, h/2 - compostH/2);
    
    m1.loadImage("1.png"); m2.loadImage("2.png");
    m3.loadImage("3.png"); m4.loadImage("4.png");
    
    int mScale = 3;
    
    m1.resize(m1.getWidth()*mScale, m1.getHeight()*mScale);
    m2.resize(m2.getWidth()*mScale, m2.getHeight()*mScale);
    m3.resize(m3.getWidth()*mScale, m3.getHeight()*mScale);
    m4.resize(m4.getWidth()*mScale, m4.getHeight()*mScale);
    
    camW = 640; camH = 480;
    cam.setDeviceID(1);
    cam.initGrabber(camW, camH);
    
    resultImage.allocate(camW, camH);
    
    artk.setup(camW, camH);
    threshold = 85;
    artk.setThreshold(threshold);
    
    ofSetLogLevel(OF_LOG_NOTICE);
    
    checkForNewImages();
}

//--------------------------------------------------------------
void ofApp::checkForNewImages() {
    dir.listDir("images");
    dir.sort();
    
    for(int i = 0; i < (int)dir.size(); i++) {
        string filename = dir.getPath(i);
        ofImage thisImg; thisImg.loadImage(filename);
        ofVec2f randPos = ofVec2f(ofRandom(compostW), ofRandom(compostH));
        
        while(randPos.x + thisImg.getWidth() > compostW ||
           randPos.y + thisImg.getHeight() > compostH) {
            thisImg.resize(thisImg.getWidth()/2, thisImg.getHeight()/2);
        }
        imagesPos.push_back(randPos);
        images.push_back(thisImg);
        
        char rm_command [strlen(filename.c_str()) + 10];
        std::sprintf(rm_command, "rm ../../../data/%s", filename.c_str());
        //std::system(rm_command);
    }
    if(images.size() > 0) framesLeftToShowImages = SHOW_IMAGES_FOR;
}

//--------------------------------------------------------------
void ofApp::update(){
    //if(ofGetFrameNum() % CHECK_INTERVAL == 0) checkForNewImages();
        
    cam.update();
    if(cam.isFrameNew()) {
        colorImage.setFromPixels(cam.getPixels(), camW, camH);
        grayImage = colorImage;
        
        //grayImage.mirror(false, true); //do this if using mirror
        grayImage.threshold(mouseX);
        artk.update(grayImage.getPixels());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //make sure to pop images after u draw them, then make sure to save compost periodically
    //don't update every frame
    
    ofBackground(255);
    
    //===========================================================
    // ARTK + COMPOST
    //===========================================================
    int numMarkers = artk.getNumDetectedMarkers();
    
    ofSetColor(255);
    grayImage.draw(0, 0, camW/4, camH/4);
    ofSetHexColor(0x666666);
    ofDrawBitmapString(ofToString(numMarkers) + " marker(s) found", 10, 20);
    artk.draw(0, 0, camW/4, camH/4);
    
    if(numMarkers == 4) {
        vector<cv::Point2f> centers;
        vector<cv::Point2f> orderedCenters;
        for(int i = 0; i < numMarkers; i++) {
            ofPoint pt = artk.getDetectedMarkerCenter(i);
            centers.push_back(cv::Point2f(pt.x,pt.y));
        }
        
        orderedCenters.push_back(centers[0]);
        orderedCenters.push_back(centers[1]);
        orderedCenters.push_back(centers[3]);
        orderedCenters.push_back(centers[2]);
        
        ofImage r; r.allocate(camW, camH, OF_IMAGE_COLOR);
        ofxCv::unwarpPerspective(colorImage, r, orderedCenters);
        resultImage.setFromPixels(r.getPixelsRef());
        resultImage.draw(w/2 - compostW/2,h/2 - compostH/2,compostW, compostH);
    //===========================================================
    // NEW IMAGES
    //===========================================================
        if(images.size() > 0) {
            ofPushMatrix();
            ofTranslate(compostPos.x, compostPos.y);
            for(int i = 0; i < images.size(); i++) {
                ofVec2f pos = imagesPos[i];
                images[i].draw(pos.x, pos.y);
            }
            ofPopMatrix();
            
            framesLeftToShowImages--;
            if(framesLeftToShowImages <= 0) {
                images.clear(); //might have to show images for longer before clearing
                imagesPos.clear();
            }
        }
    }
    
    //===========================================================
    // MARKERS
    //===========================================================
    ofSetColor(255);
    ofSetRectMode(OF_RECTMODE_CENTER);
    float scale = 1.3;
    ofFill();
    ofRect(compostPos.x, compostPos.y,m1.getWidth()*scale, m1.getHeight()*scale);
    ofRect(compostPos.x+compostW, compostPos.y,m1.getWidth()*scale, m1.getHeight()*scale);
    ofRect(compostPos.x, compostPos.y+compostH,m1.getWidth()*scale, m1.getHeight()*scale);
    ofRect(compostPos.x+compostW, compostPos.y+compostH,m1.getWidth()*scale, m1.getHeight()*scale);
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    m1.draw(compostPos.x - m1.getWidth()/2, compostPos.y - m1.getHeight()/2);
    m2.draw(compostPos.x + compostW - m1.getWidth()/2, compostPos.y - m1.getHeight()/2);
    m3.draw(compostPos.x - m3.getWidth()/2, compostPos.y + compostH - m1.getHeight()/2);
    m4.draw(compostPos.x + compostW - m1.getWidth()/2, compostPos.y + compostH - m1.getHeight()/2);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
