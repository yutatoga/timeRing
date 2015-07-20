#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // video grabber
    videoGrabber.setDeviceID(0);
    videoGrabber.setDesiredFrameRate(60);
    videoGrabber.initGrabber(ofGetWidth(), ofGetHeight());
    
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
}

//--------------------------------------------------------------
void ofApp::update(){
    videoGrabber.update();
}

//--------------------------------------------------------------
void ofApp::draw(){    
    // test drawing
    for (int i=0; i<ringImages.size(); i++) {
        ringImages[i].draw(10+10*i, 10+10*i, ofGetWidth(), ofGetHeight());
    }
    
    // Debug
    videoGrabber.draw(ofGetWidth(), 0, -240, 180);
    
    // Info
    ofDrawBitmapString("press any keys to add ringImage", ofPoint(5, 15));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // mask by fbo
    ofFbo fboMask;
    
    // fbo to mask
    fboMask.allocate(ofGetWidth(), ofGetHeight());
    fboMask.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(ofColor::red);// using R channel to mask
    ofCircle(100, 100, 100);
    ofRect(500, 500, 300, 300);
    ofSetColor(0, 255, 255, 255);// using R channel to mask
    ofCircle(100, 100, 50);
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
