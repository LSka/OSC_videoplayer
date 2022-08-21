#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofHideCursor();
    // ofSetVerticalSync(true);
     ofBackground(0);
     
     screenWidth = ofGetWidth();
     screenHeight = ofGetHeight();
     
     ofLog() << "loading settings.xml" <<endl;
     
     //we load our settings file
     if( settings.loadFile("settings.xml") ){
         ofLog() << "settings.xml loaded!" << endl;
     }else{
         ofLog()<< "unable to load settings.xml check data/ folder" << endl;
     }
     
     

    loadVideos();
    
    playerSettings.enableTexture = true;
    playerSettings.enableLooping = false;
    playerSettings.enableAudio = false;
    playerSettings.autoStart = false;
    
    player.setup(playerSettings);
     
     //initialize OSC
     int oscInPort = settings.getValue("OSC:RECEIVER:PORT",6000);
     receiver.setup(oscInPort);
     ofxOscSenderSettings senderSettings;
     senderSettings.host = settings.getValue("OSC:SENDER:HOST","192.168.1.255");
     senderSettings.port = settings.getValue("OSC:SENDER:PORT",5000);
     senderSettings.broadcast = true;
	 sender.setup(senderSettings);
     
     receiver.start();
     
     outMessage.setAddress("/player/init");
     outMessage.addStringArg("ready");
     sender.sendMessage(outMessage);
     
    showInfo = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    if (receiver.hasWaitingMessages()){
        receiver.getNextMessage(receivedMessage);
        string addr = receivedMessage.getAddress();
        
        if (addr.compare("/videoplayer/play") == 0){ //check the OSC address
            videoId = receivedMessage.getArgAsInt(0);
            string videoPath = ofToDataPath(paths[videoId],true);
            ofLog()<<"loading video #"<<videoId<<": "<<videoPath<<endl;
            player.loadMovie(videoPath);
            player.start();
            outMessage.setAddress("/videoplayer/playing");
            outMessage.addIntArg(videoId);
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/stop") == 0){
            ofLog()<<"stopping video";
            if(player.isOpen()) player.close();
            outMessage.setAddress("/videoplayer");
            outMessage.addStringArg("stopped");
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/pause") == 0){
            ofLog()<<"pausing video";
            player.setPaused(true);
            outMessage.setAddress("/videoplayer/paused");
            outMessage.addIntArg(1);
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/resume") == 0){
            ofLog()<<"resuming video";
            player.setPaused(false);
            outMessage.setAddress("/videoplayer/paused");
            outMessage.addIntArg(0);
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/reload") == 0){
            ofLog()<<"reloading videos";
            loadVideos();
            outMessage.setAddress("/videoplayer");
            outMessage.addStringArg("loading");
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/loop") == 0){
            int l = receivedMessage.getArgAsInt(0);
            switch(l){
                case 0:{
                    player.disableLooping();
                    ofLog()<<"looping disabled";
                    break;
                }
                case 1:{
                    player.enableLooping();
                    ofLog()<<"looping enabled";
                    break;
                }
                default:{}
            }
            outMessage.setAddress("/videoplayer/looping");
            outMessage.addIntArg(l);
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/info") == 0){
            showInfo = receivedMessage.getArgAsBool(0);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    player.draw(0, 0, screenWidth, screenHeight);
    
    if (showInfo){
    ofDrawBitmapStringHighlight(player.getInfo(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
    }
}

void ofApp::loadVideos(){
    string path = settings.getValue("VIDEO:PATH","movies");
    dir.open(path);
    dir.allowExt("mp4");
    dir.listDir();
    dir.sort();
    
    
    for (int i = 0; i<dir.size(); i++){
        paths.push_back(dir.getPath(i));
    }
    ofLog()<<dir.size()<<endl;
}

