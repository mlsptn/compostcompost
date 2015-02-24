#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxARToolkitPlus.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void checkForNewImages();
   
        int SHOW_IMAGES_FOR = 5;
        int CHECK_INTERVAL = 80;
        int COMPOST_UPDATE_INTERVAL = 20;
        int camW; int camH;
        int w, h;
        int framesLeftToShowImages;
        int compostW, compostH;
        int threshold;
    
        ofDirectory dir;
        vector<ofImage> images;
        vector<ofVec2f> imagesPos;
    
        ofVec2f compostPos;
    
        ofVideoGrabber cam;
        ofxARToolkitPlus artk;
        
        ofImage m1, m2, m3, m4;
        
        ofxCvColorImage resultImage;
        ofxCvColorImage colorImage;
        ofxCvGrayscaleImage grayImage;
};
