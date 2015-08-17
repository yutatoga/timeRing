#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void shootRingImage();

    ofVideoGrabber videoGrabber;
    ofShader shader;
    ofImage image, videoGrabberImage;
    vector<ofImage> ringImages;
    bool showDebug;
    int lastShootingTime;
    int shootingTimes;
    float waitingTime;
};
