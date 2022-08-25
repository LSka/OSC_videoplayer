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
     
    videoAlpha = 255;
    imgAlpha = 255;

    loadVideos();
    loadImages();
    
    imgLoaded = false;
    
    playerSettings.enableTexture = true;
    playerSettings.enableLooping = false;
    playerSettings.enableAudio = false;
    playerSettings.listener = this;
    
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
    
    int cOe = settings.getValue("VIDEO:CLEARONEND",0);
    if (cOe == 1) {
        clearOnEnd = true;
        
    }
    else {
        clearOnEnd = false;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    if (receiver.hasWaitingMessages()){
        receiver.getNextMessage(receivedMessage);
        string addr = receivedMessage.getAddress();
        
        if (addr.compare("/videoplayer/video/play") == 0){ //check the OSC address
            videoId = receivedMessage.getArgAsInt(0);
            videoId = videoId -1;
            if (videoId >= dir.size()){
                ofLog()<<"Invalid cue number"<<endl;
            }
            else{
                videoId = ofClamp(videoId, 0, dir.size());
                string videoPath = ofToDataPath(paths[videoId],true);
                ofLog()<<"loading video #"<<videoId<<": "<<videoPath<<endl;
                player.loadMovie(videoPath);
                //player.start();
                outMessage.setAddress("/videoplayer/video/playing");
                outMessage.addIntArg(videoId);
                sender.sendMessage(outMessage);
            }

        }
        if (addr.compare("/videoplayer/video/stop") == 0){
            ofLog()<<"stopping video";
            if(player.isOpen()) player.close();
            outMessage.setAddress("/videoplayer/video");
            outMessage.addStringArg("stopped");
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/video/pause") == 0){
            ofLog()<<"pausing video";
            player.setPaused(true);
            outMessage.setAddress("/videoplayer/video/paused");
            outMessage.addIntArg(1);
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/video/resume") == 0){
            ofLog()<<"resuming video";
            player.setPaused(false);
            outMessage.setAddress("/videoplayer/video/paused");
            outMessage.addIntArg(0);
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/video/reload") == 0){
            ofLog()<<"reloading videos";
            loadVideos();
            outMessage.setAddress("/videoplayer/video");
            outMessage.addStringArg("loading");
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/image/reload") == 0){
            ofLog()<<"reloading images"<<endl;
            loadImages();
            outMessage.setAddress("/videoplayer/image");
            outMessage.addStringArg("loading");
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/video/loop") == 0){
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
            outMessage.setAddress("/videoplayer/video/looping");
            outMessage.addIntArg(l);
            sender.sendMessage(outMessage);
        }
        if (addr.compare("/videoplayer/info") == 0){
            showInfo = receivedMessage.getArgAsBool(0);
        }
        if (addr.compare("/videoplayer/image/load") == 0){
            imgId = receivedMessage.getArgAsInt(0);
            imgId = imgId -1;
            if (imgId >= imgDir.size()){
                ofLog()<<"Invalid image number"<<endl;
            }
            else {
                imgId = ofClamp(imgId,0,imgDir.size());
                string imgPath = ofToDataPath(imgPaths[imgId],true);
                ofLog()<<"loading image #"<<imgId+1<<": "<<imgPath<<endl;
                img.load(imgPath);
                imgLoaded = true;
                outMessage.setAddress("/videoplayer/image/loaded");
                outMessage.addIntArg(imgId+1);
                sender.sendMessage(outMessage);
            }
        }
        if (addr.compare("/videoplayer/image/clear") == 0){
            if (img.isAllocated()){
                img.clear();
                imgLoaded = false;
            }
        }
        if (addr.compare("/videoplayer/image/alpha") == 0){
            float iA = receivedMessage.getArgAsFloat(0);
            iA = ofClamp(iA,0.,1.);
            imgAlpha = int(iA*255.);
        }
        if (addr.compare("/videoplayer/video/alpha") == 0){
            float vA = receivedMessage.getArgAsFloat(0);
            vA = ofClamp(vA,0.,1.);
            videoAlpha = int(vA*255.);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (player.isPlaying()){
        ofSetColor(255.255.255,videoAlpha);
        player.draw(0, 0, screenWidth, screenHeight);
    }
    if(imgLoaded){
        ofSetColor(255,255,255,imgAlpha);
        img.draw(0,0,screenWidth,screenHeight);
    }
    
    
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
void ofApp::loadImages(){
    string imgPath = settings.getValue("IMAGES:PATH","images");
    imgDir.open(imgPath);
    imgDir.allowExt("jpg");
    imgDir.allowExt("png");
    imgDir.listDir();
    imgDir.sort();
    
    
    for (int i = 0; i<imgDir.size(); i++){
        imgPaths.push_back(imgDir.getPath(i));
        ofLog()<<imgDir.getPath(i)<<endl;
    }
    ofLog()<<imgDir.size()<<" files loaded."<<endl;
}


void ofApp::onVideoEnd(ofxOMXPlayer* player){
    ofLog()<<"video "<<videoId<<" ended"<<endl;
    if (clearOnEnd){
        player->close();
    }

}

void ofApp::onVideoLoop(ofxOMXPlayer* player){
    
}
