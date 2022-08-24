#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void loadVideos();
        void loadImages();

    
    int screenWidth;
    int screenHeight;
    int videoId;
    int imgId;
    
    ofxXmlSettings settings;
        
    ofxOscReceiver receiver;
    ofxOscSender sender;
    ofxOscMessage outMessage;
    ofxOscMessage receivedMessage;
    
    ofImage img;
    
    vector<string> paths;
    vector<string> imgPaths;
    ofVideoPlayer player;
    
    bool done;
    
    ofDirectory dir;
    ofDirectory imgDir;
    
    ofFbo videoFbo;
    ofFbo imgFbo;
    
    int videoAlpha;
    int imgAlpha;
		
};
