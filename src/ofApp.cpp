#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    depth = 50; //テキストの奥行き
    ro = 0; //回転率
    noiseVal = ofRandom(10); //ノイズ
    noiseVal2 = ofRandom(10);
    bRotation = true; //テキストの回転
    renderingMode = true;
   font.loadFont("mona.ttf", 36, true, true, 0.3f, 0, true); //フォント名、フォントサイズ
    font.setLineHeight(font.getFontSize()*1.4); //行の高さ
    ofBuffer buffer = ofBufferFromFile("aa.txt"); //テキストファイルの読み込み
    typeStr = buffer.getText();
    letterPaths = font.getStringAsPoints(typeStr);
    bbox = font.getStringBoundingBox(typeStr, 0, 0);
    
    for (int i = 0; i < letterPaths.size(); i++) {
        ofVboMesh front = letterPaths[i].getTessellation();
        ofVboMesh back = front;
        auto v = back.getVerticesPointer();
        for (int j = 0; j < (int)back.getNumVertices(); j++) {
            v[j].z += depth; //テキストの奥行き
            
            front.addNormal(ofVec3f(0, 0, -1));
            back.addNormal(ofVec3f(0, 0, 1));
        }
        textMesh.push_back(front);
        textMesh.push_back(back);
    }
    
    for (int i = 0; i < (int)letterPaths.size(); i++) {
        lines = letterPaths[i].getOutline();
        for (int j = 0; j < (int)lines.size(); j++) {
            if( j!= 0)ofNextContour(true) ;
            vector<glm::vec3> points = lines[j].getVertices();
            int k = 0;
            for (k = 0; k < (int)points.size()-1; k++) {
                ofPoint p1 = points[k+0];
                ofPoint p2 = points[k+1];
                
                side.addVertex(p1);
                side.addVertex(p2);
                side.addVertex(ofPoint(p1.x, p1.y, p1.z+depth));
                
                side.addVertex(ofPoint(p1.x, p1.y, p1.z+depth));
                side.addVertex(p2);
                side.addVertex(ofPoint(p2.x, p2.y, p2.z+depth));
                 
            }
            // Connect the last to the first
            ofPoint p1 = points[k];
            ofPoint p2 = points[0];
            
            side.addVertex(p1);
            side.addVertex(p2);
            side.addVertex(ofPoint(p1.x, p1.y, p1.z+depth));
            
            side.addVertex(ofPoint(p1.x, p1.y, p1.z+depth));
            side.addVertex(p2);
            side.addVertex(ofPoint(p2.x, p2.y, p2.z+depth));
            
            
        }
    }
    side.setupIndicesAuto();
    //setNormals(side); //法線を加える
    textMesh.push_back(side);
    
    //色の設定
    for (int i = 0; i < textMesh.size(); i++) {
        colors.push_back( ofColor( ofRandom(40,55), ofRandom(100, 160), ofRandom(130, 220), ofRandom(200, 250)));
    }
    
    //ofxCameraAnaglyph cam;
    cam.setPosition( 0, 0, 1000 );
    cam.lookAt( ofVec3f(0,0,0));
    cam.enableStereo();
}

//--------------------------------------------------------------
void ofApp::update(){
    noiseVal += 0.02;
    noiseVariance = customNoise(noiseVal, 12);
    noiseVal2 += ofRandom(0.03, 0.05);
    noiseVariance2 = customNoise(noiseVal2, 20);
    
    cam.eyeSeparation   = ofMap( noiseVariance, -1, 1, -1, 2., true ); //横のずれ
    cam.focalLength     = ofMap( noiseVariance2, -1, 1, 0, 100, true ); //縦のずれ
    
    //cam.eyeSeparation   = ofMap( mouseX, 0, ofGetWidth(), 0, 1., true );
    //cam.focalLength     = ofMap( mouseY, 0, ofGetHeight(), 0, 100, true );
    
    if(bRotation) ro = ofGetFrameNum();
    
    cam.orbitDeg(ro, 0.2f, 900 );
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(241, 240, 238);
    
    ofEnableDepthTest();
    
    
    if( cam.isStereo() ) {
        cam.beginLeft();
    } else {
        cam.begin();
    }
    
    ofPushMatrix();
    ofRotateDeg(180, 1, 0, 0);
    ofTranslate(-bbox.width/2+tx, -bbox.height/2+ty);
    
    if(renderingMode){
        for (int i = 0; i < textMesh.size(); i++) {
            ofSetColor(colors[i]);
            textMesh[i].draw();
        }
    } else {
        for (int i = 0; i < textMesh.size(); i++) {
            ofSetColor(colors[i]);
            textMesh[i].drawWireframe(); //ワイアーフレーム表示
        }
    }
    ofPopMatrix();
    
    // draw again into the right eye //
       if( cam.isStereo() ) {
           cam.beginRight();
           
           ofPushMatrix();
           ofRotateDeg(180, 1, 0, 0);
    ofTranslate(-bbox.width/2+tx, -bbox.height/2+ty);

           if(renderingMode){
               //ofEnableLighting();
               for (int i = 0; i < textMesh.size(); i++) {
                   ofSetColor(colors[i]);
                   textMesh[i].draw();
               }
           } else {
               for (int i = 0; i < textMesh.size(); i++) {
                   ofSetColor(colors[i]);
                   textMesh[i].drawWireframe(); //ワイアーフレーム表示
               }
           }
           
    ofPopMatrix();
       }
     
    
    if( cam.isStereo() ) {
        cam.endStereo();
    } else {
        cam.end();
    }
     
    ofDisableDepthTest();


}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'r': //回転
        case 'R':
            bRotation =!bRotation;
            break;
        case 'e':
        case 'E':
            ro = 0; //回転リセット
            break;
        case 'w': //レンダリング←→ワイアーフレーム
        case 'W':
            renderingMode =!renderingMode;
            break;
        case 'c': //色の変更
        case 'C':
            colors.clear();
            for (int i = 0; i < textMesh.size(); i++) {
                colors.push_back( ofColor( ofRandom(40,55), ofRandom(100, 160), ofRandom(130, 220), ofRandom(200, 250)));
            }
            break;
        case OF_KEY_DOWN: //テキストのy値 下に+10
            ty += 10;
            break;
        case OF_KEY_UP:  //テキストのy値 上に-10
            ty -= 10;
            break;
        case OF_KEY_RIGHT: //テキストのx値 右に+10
            tx += 10;
            break;
        case OF_KEY_LEFT: //テキストのx値 左に-10
            tx -= 10;
            break;
        case 's': //スクリーンショット
        case 'S':
            myImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            myImage.save(ofGetTimestampString("%Y%m%d%H%M%S")+"##.png");
            break;
        case ' ':
                if( cam.isStereo() ) cam.disableStereo();
                else cam.enableStereo();
    }

}
//--------------------------------------------------------------
void ofApp::setNormals(ofMesh &mesh){
    //The number of the vertices
    int nV = (int)mesh.getNumVertices();
    
    //The number of the triangles
    int nT = (int)mesh.getNumIndices()/3;
    
    vector<glm::vec3> norm(nV); //Array for the normals
    
    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (int t = 0; t < nT; t++) {
        //Get indices of the triangle t
        int i1 = mesh.getIndex(3*t);
        int i2 = mesh.getIndex(3*t+1);
        int i3 = mesh.getIndex(3*t+2);
        
        //Get vertices of the triangle
        const glm::vec3 &v1 = mesh.getVertex(i1);
        const glm::vec3 &v2 = mesh.getVertex(i2);
        const glm::vec3 &v3 = mesh.getVertex(i3);
        
        //Compute the triangle's normal
        glm::vec3 dir = glm::normalize(glm::cross(v2-v1, v3-v1));
        
        //Accumulate it to norm array for i1, i2, i3
        norm[i1] += dir;
        norm[i2] += dir;
        norm[i3] += dir;
    }
    
    //Normalixe the normal's length
    for (int i = 0; i < nV; i++) {
        norm[i] = glm::normalize(norm[i]);
    }
    
    //Set the normals to mesh
    mesh.clearNormals();
    mesh.addNormals(norm);
}
//--------------------------------------------------------------
float ofApp::customNoise(float value, int value2){ //戻り値 -1~+1
    int count = (int)value%value2; //value÷value2をしたときの余り
    float retValue = pow(sin(value), count);
    return retValue;
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
