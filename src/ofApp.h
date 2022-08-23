#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "ofxOMXPlayer.h"

class ofApp : public ofBaseApp, public ofxOMXPlayerListener{

	public:
		void setup();
		void update();
		void draw();
        void loadVideos();
        void onVideoEnd(ofxOMXPlayer* player);
        void onVideoLoop(ofxOMXPlayer* player);
    
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
    ofxOMXPlayerSettings playerSettings;
    ofxOMXPlayer player;
    
    bool showInfo;
    bool hasEnded;
    
    ofDirectory dir;
		
};
