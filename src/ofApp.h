#pragma once

#include "ofMain.h"
#include "ofxTrueTypeFontUL2.h"
#include "ofxCameraAnaglyph.h"

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void setNormals(ofMesh &mesh);
    float customNoise(float value, int value2);
    
    ofxTrueTypeFontUL2 font;
    ofImage myImage;
    vector <ofVboMesh> textMesh;
    ofVboMesh side;
    vector <ofPath> letterPaths;
    vector <ofPolyline> lines;
    vector <ofColor> colors;
    string typeStr;
    ofRectangle bbox;
    int depth; //テキストの奥行き
    bool bRotation;
    bool renderingMode;
    int tx, ty;
    float ro, noiseVal, noiseVariance, noiseVal2, noiseVariance2;
    
    ofxCameraAnaglyph cam;
};
