#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetCircleResolution(64);
    ofSetFullscreen(true);
    
    // video grabber
    videoGrabber.setDeviceID(0);
    videoGrabber.setDesiredFrameRate(60);
    videoGrabber.initGrabber(ofGetWidth(), ofGetHeight());
    lastShootingTime = ofGetElapsedTimef();
    shootingTimes = 100;
    waitingTime = 1.0;
    
    // shader
#ifdef TARGET_OPENGLES
    shader.load("shadersES2/shader");
#else
    if(ofIsGLProgrammableRenderer()){
        ofLogNotice("using shadersGL3");
        shader.load("shadersGL3/shader");
    }else{
        ofLogNotice("using shadersGL2");
        shader.load("shadersGL2/shader");
    }
#endif
    
    // test image
    image.loadImage("img.jpg");
    
    // save videoGrabber frame
    videoGrabberImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    videoGrabberImage.clear();
    
    //debug
    showDebug = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    videoGrabber.update();
    
    if (ofGetElapsedTimef()-lastShootingTime > waitingTime) {
        if(ringImages.size() < shootingTimes){
            shootRingImage();
        } else{
            ringImages.clear();
        }
        lastShootingTime = ofGetElapsedTimef();        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){    
    // test drawing
    for (int i=0; i<ringImages.size(); i++) {
        ringImages[i].draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    // Debug
    if (showDebug) {
        videoGrabber.draw(ofGetWidth(), 0, -240, 180);
        // Info
        ofDrawBitmapString("press space key to add ringImage", ofPoint(5, 15));
        ofDrawBitmapString("press mouse to change live/debug mode", ofPoint(5, 30));
        ofDrawBitmapString("press 'c' to clear ringImages", ofPoint(5, 45));
    }
}

void ofApp::shootRingImage(){
    // mask by fbo
    ofFbo fboMask;
    
    // fbo to mask
    fboMask.allocate(ofGetWidth(), ofGetHeight());
    fboMask.begin();
    ofClear(0, 0, 0, 0);
    
    // draw mask
    ofSetColor(ofColor::red);// using R channel to mask
    float ringWidth = 10.0f;
    ofCircle(fboMask.getWidth()/2.0, fboMask.getHeight()/2.0f, (ringImages.size()+1)*ringWidth);
    ofSetColor(0, 255, 255, 255);// using R channel to mask
    ofCircle(fboMask.getWidth()/2.0, fboMask.getHeight()/2.0f, ringImages.size()*ringWidth);
    
    ofSetColor(ofColor::white);
    fboMask.end();
    
    // time ring image
    ofFbo fboRingImageCanvas;
    fboRingImageCanvas.allocate(ofGetWidth(), ofGetHeight());
    fboRingImageCanvas.begin();
    ofClear(0, 0, 0, 0);
    shader.begin();
    shader.setUniformTexture("imageMask", fboMask.getTextureReference(), 1);
    
    // draw image to be masked
    // videoGrabber.draw(videoGrabber.width, 0, -videoGrabber.width, videoGrabber.height); // prob: changing matrix??
    videoGrabberImage.setFromPixels(videoGrabber.getPixelsRef());
    videoGrabberImage.mirror(false, true);
    videoGrabberImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    shader.end();
    // save to ofImage
    ofPixels p;
    fboRingImageCanvas.readToPixels(p);
    
    // ringImage
    ofImage ringImage;
    ringImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR_ALPHA);
    ringImage.clear();
    ringImage.setFromPixels(p);
    fboRingImageCanvas.end();
    
    // save ringImage to vector
    ringImages.push_back(ringImage);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':{
            shootRingImage();
            break;
        }
        case 'c':
            ringImages.clear();
            break;
        default:
            break;
    }
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
    showDebug = !showDebug;
    if (showDebug) {
        ofShowCursor();
    }else{
        ofHideCursor();
    }
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
