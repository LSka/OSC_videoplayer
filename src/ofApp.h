#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "ofxOMXPlayer.h"

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
    ofxOscMEssage receivedMessage;
    
    ofImage background;
    
    vector<string> paths;
    ofxOMXPlayerSettings playerSettings;
    ofxOMXPlayer player;
    
    bool showInfo;
    
    ofDirectory dir;
		
};
