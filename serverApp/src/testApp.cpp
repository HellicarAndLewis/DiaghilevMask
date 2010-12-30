#include "testApp.h"
#include "stdio.h"

//--------------------------------------------------------------
testApp::testApp(){

}

//--------------------------------------------------------------
void testApp::setup(){
	loader.start();
    ofSetVerticalSync(true);
	
    currX = 0;
    dirX = 1;
    speedX = 5;
    currImg=NULL;
    loadNextImg=true;
	
	temp_texture.allocate(1024,768,GL_RGB); // CHANGE FOR THE SIZE OF THE IMAGES ...
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(0,0,0);   // black because threads are EVIL ;)
	
    if (dirX == 1) {
        currX += (ofGetWidth()-currX)/speedX;
    } else {
        currX -= (currX/speedX);
    }
	
    if (currX+5 > ofGetWidth())
        dirX = -1;
    if (currX < 5)
        dirX = 1;
	
	if (ofGetFrameNum()%300 == 0){
		loadNextImg=true;
	}
	
	if (ofGetFrameNum()%3000 == 0){ //
		loader.stop();
		cout << "Resetting, looking for new files" << endl;
		loader.fetchTextureNames();
		loader.start();
	}	

	if(loadNextImg){
        ofImage * nextImg = loader.getNextTexture();
        if(nextImg!=NULL){
			currImg=nextImg;
			loadNextImg=false;
			temp_texture.loadData(currImg->getPixels(),currImg->width,currImg->height,GL_RGB);
        }
	}
}


//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xFFFFFF);
    if(currImg){
		temp_texture.draw(0,0,currImg->width,currImg->height);
		//cout << "Drawing the image!, its size is: " << currImg->width << " by " << currImg->height << endl;
	}
	
    //ofCircle(currX, ofGetHeight()/2, 50);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	switch (key){
		case 'f':{
			ofToggleFullscreen();
			break;				
		}
		default:{
			break;
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::resized(int w, int h){

}

