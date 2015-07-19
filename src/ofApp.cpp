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
    
    // Info
    ofDrawBitmapString("press any keys to add ringImage", ofPoint(5, 15));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // mask by fbo
    ofFbo fboMask;
    
    // fbo
    fboMask.allocate(ofGetWidth(), ofGetHeight());
    fboMask.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(ofColor::white);
    ofRect(10, 10, 30, 100);
    ofCircle(100, 100, 30);
    fboMask.end();
    
    // time ring image
    ofFbo fboRingImageCanvas;
    fboRingImageCanvas.allocate(ofGetWidth(), ofGetHeight());
    fboRingImageCanvas.begin();
    ofClear(0, 0, 0, 0);
    shader.begin();
    shader.setUniformTexture("imageMask", fboMask.getTextureReference(), 1);
    image.draw(0, 0, ofGetWidth(), ofGetHeight());
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
