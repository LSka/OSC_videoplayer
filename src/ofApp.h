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
        void loadImages();
        void onVideoEnd(ofxOMXPlayer* player);
        void onVideoLoop(ofxOMXPlayer* player);
    
    int screenWidth;
    int screenHeight;
    int videoId;
    int imgId;
    int videoAlpha;
    int imgAlpha;
    
    ofxXmlSettings settings;
        
    ofxOscReceiver receiver;
    ofxOscSender sender;
    ofxOscMessage outMessage;
    ofxOscMessage receivedMessage;
    
    ofImage img;
    
    vector<string> paths;
    vector<string> imgPaths;
    ofxOMXPlayerSettings playerSettings;
    ofxOMXPlayer player;
    
    bool showInfo;
    bool clearOnEnd;
    bool imgLoaded;
    
    ofDirectory dir;
    ofDirectory imgDir;

		
};
