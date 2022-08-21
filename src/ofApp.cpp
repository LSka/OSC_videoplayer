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
//     videoId = 0;
//     string videoPath = ofToDataPath(dir.getPath(videoId),true);
//     player.load(videoPath);
//     player.setLoopState(OF_LOOP_NONE);
//     player.play();

     
     //initialize OSC
     int oscInPort = settings.getValue("OSC:RECEIVER:PORT",6000);
     receiver.setup(oscInPort);
     ofxOscSenderSettings senderSettings;
     senderSettings.host = settings.getValue("OSC:SENDER:HOST","192.168.1.1");
     senderSettings.port = settings.getValue("OSC:SENDER:PORT",5000);
     senderSettings.broadcast = true;
	 sender.setup(senderSettings);
     
     receiver.start();
     
     outMessage.setAddress("/player/init");
     outMessage.addStringArg("ready");
     sender.sendMessage(outMessage);
     

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
            player.load(videoPath);
            player.setLoopState(OF_LOOP_NONE);
            if (player.isLoaded()) player.play();
            outMessage.setAddress("/videoplayer/playing");
            outMessage.addIntArg(videoId);
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/stop") == 0){
            ofLog()<<"stopping video";
            if (player.isPlaying()) player.stop();
            ofxOscMessage m;
            outMessage.setAddress("/videoplayer");
            outMessage.addStringArg("stopped");
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/pause") == 0){
            ofLog()<<"pausing video";
            player.setPaused(true);
            ofxOscMessage m;
            outMessage.setAddress("/videoplayer/paused");
            outMessage.addIntArg(1);
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/resume") == 0){
            ofLog()<<"resuming video";
            player.setPaused(false);
            ofxOscMessage m;
            outMessage.setAddress("/videoplayer/paused");
            outMessage.addIntArg(0);
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/reload") == 0){
            ofLog()<<"reloading videos";
            loadVideos();
            ofxOscMessage m;
            outMessage.setAddress("/videoplayer");
            outMessage.addStringArg("loading");
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/loop") == 0){
            int l = receivedMessage.getArgAsInt(0);
            switch(l){
                case 0:{
                    player.setLoopState(OF_LOOP_NONE);
                    ofLog()<<"looping disabled";
                    break;
                }
                case 1:{
                    player.setLoopState(OF_LOOP_NORMAL);
                    ofLog()<<"looping enabled";
                    break;
                }
                case 2:{
                    player.setLoopState(OF_LOOP_PALINDROME);
                    ofLog()<<"looping enabled";
                    break;
                    }
                default:{}
            }
            ofxOscMessage m;
            outMessage.setAddress("/videoplayer/looping");
            outMessage.addIntArg(l);
            sender.sendMessage(outMessage);
        }

    }
    

    if (!player.getIsMovieDone()){
        player.update();
        done = false;
    }
    else if(player.getIsMovieDone() && done == false){
        outMessage.setAddress("/videoplayer/");
        outMessage.addStringArg("done");
        sender.sendMessage(outMessage);
        done = true;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    player.draw(0, 0, screenWidth, screenHeight);
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
    ofLog()<<dir.size()<<" files loaded."<<endl;
}
