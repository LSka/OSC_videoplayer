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

    
    int screenWidth;
    int screenHeight;
    int videoId;
    
    ofxXmlSettings settings;
        
    ofxOscReceiver receiver;
    ofxOscSender sender;
    ofxOscMessage outMessage;
    ofxOscMessage receivedMessage;
    
    ofImage background;
    
    vector<string> paths;
    ofVideoPlayer player;
    
    bool done;
    
    ofDirectory dir;
		
};
