#pragma once

#define		IDLE_COLOR		0xFFFFFF
#define		OVER_COLOR		0x00FF00
#define		DOWN_COLOR		0xFF0000

#include "ofxVectorMath.h"

typedef enum{
	OBJECT_TRANSLATE,
	OBJECT_SCALE,
	OBJECT_ROTATE,
}objectState;

class MaskObject : public ofRectangle {
public:
	void setup() {
		oldMouseX = 0;
		oldMouseY = 0;
		rotation = 0;
		state = OBJECT_TRANSLATE;
		centreX = 0.f;
		centreY = 0.f;
		
		_mouseX = 0;
		_mouseY = 0;
		_mouseButton = 0;
		_mouseOver = false;
		_mouseDown = false;
		scaleFactor = 1.f;
	}

	void exit() {
	}
	
	void update(float _scaleFactor) {
		scaleFactor = _scaleFactor;		
	}
	
	void draw() {
		
		float newX = x*scaleFactor;
		float newY = y*scaleFactor;
		float newWidth = width*scaleFactor;
		float newHeight = height*scaleFactor; //taking into account scalling now
		
		ofPushStyle();
		
		ofEnableAlphaBlending();
		
		if(_mouseDown) ofSetColor(DOWN_COLOR);
		else if(_mouseOver) ofSetColor(OVER_COLOR);
		else ofSetColor(IDLE_COLOR);		
		
		glPushMatrix();
			glTranslatef(centreX+newX, centreY+newY, 0);
			glRotatef(180.0f*rotation/PI, 0, 0, 1);
			graphic.draw(-newWidth/2, -newHeight/2, newWidth, newHeight);
		glPopMatrix();		
		
		ofDisableAlphaBlending(); //safety
		
		ofPopStyle();
	}
	
		//events from the testApp
	void mouseMoved(int _x, int _y){
		_mouseX = _x;
		_mouseY = _y;
		
		if(hitTest(_x, _y)) {						// if mouse is over the object
			if(!_mouseOver) {						// if wasn't over previous frame
				onRollOver(_x, _y);						// call onRollOver
				_mouseOver = true;						// update flag
			}
			onMouseMove(_x, _y);						// and trigger onMouseMove
		} else if(_mouseOver) {					// if mouse is not over the object, but the flag is true (From previous frame)
			onRollOut();							// call onRollOut
			_mouseOver = false;						// update flag
		}		
	}
	
	void mouseDragged(int _x, int _y, int _button){		
		_mouseX = _x;
		_mouseY = _y;
		_mouseButton = _button;
		
		if(hitTest(_x, _y)) {						// if mouse is over the object
			if(!_mouseOver) {						// if wasn't over previous frame
													//				onPress(x, y);							// call onPress - maybe not
				_mouseOver = true;						// update flag
			}
			onDragOver(_x, _y, _button);				// and trigger onDragOver
		} else {
			if(_mouseOver) {					// if mouse is not over the object, but the flag is true (From previous frame)
				onRollOut();							// call onRollOut
				_mouseOver = false;						// update flag
			}
			if(_mouseDown) {
				onDragOutside(_x, _y, _button);
			}
		}		
	}
	
	void mousePressed(int _x, int _y, int _button){
		
		_mouseX = _x;
		_mouseY = _y;
		_mouseButton = _button;
		
		if(hitTest(_x, _y)) {						// if mouse is over
			if(!_mouseDown) {						 // if wasn't down previous frame
				onPress(_x, _y, _button);					// call onPress
				_mouseDown = true;						// update flag
			}
		} else {								// if mouse is not over
			onPressOutside(_x, _y, _button);
		}
		
	}
	
	void mouseReleased(int _x, int _y, int _button){
		_mouseX = _x;
		_mouseY = _y;
		_mouseButton = _button;
		
		if(hitTest(_x, _y)) {
			onRelease(_x, _y, _button);
		} else {
			if(_mouseDown) onReleaseOutside(_x, _y, _button);
		}
		_mouseDown = false;		
	}	
	
	
		//events
	
	void onRollOver(int _x, int _y) {
			//everything is measured from the centre...
			
		oldMouseX = _x-centreX;
		oldMouseY = _y-centreY;		
	}
	
	void onRollOut() {
			//printf("MaskObject::onRollOut()\n");
	}
	
	void onMouseMove(int _x, int _y){
			//cout << "Mouse move" << endl;
	}
	
	void onDragOver(int _x, int _y, int _button) {
			//printf("MaskObject::onDragOver(x: %i, y: %i, button: %i)\n", x, y, button);
			//this is where the drag to translate things happen, centre relative...
		int mouseXChange = _x-centreX-oldMouseX;
		int mouseYChange = _y-centreY-oldMouseY;
		
		if(state == OBJECT_TRANSLATE){
			x += mouseXChange;
			y += mouseYChange;				
		}else if(state == OBJECT_SCALE){
			width += mouseXChange;
			height += mouseYChange;				
		}else if(state == OBJECT_ROTATE){
			rotation += ofDegToRad(mouseXChange);
		}
		
		oldMouseX = _x-centreX;
		oldMouseY = _y-centreY;
	}
	
	void onDragOutside(int x, int y, int button) {
			//printf("MaskObject::onDragOutside(x: %i, y: %i, button: %i)\n", x, y, button);
	}
	
	void onPress(int x, int y, int button) {
			//printf("MaskObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
		
			//everything is measured from the centre...
		
		oldMouseX = x-centreX;
		oldMouseY = y-centreY;		
	}
	
	void onPressOutside(int x, int y, int button) {
			//printf("MaskObject::onPressOutside(x: %i, y: %i, button: %i)\n", x, y, button);	
	}	
	
	void onRelease(int x, int y, int button) {
			//printf("MaskObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);		
	}
	
	void onReleaseOutside(int x, int y, int button) {
			//printf("MaskObject::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
	}
	
	void keyPressed(int key) {
			//printf("MaskObject::keyPressed(key: %i)\n", key);
		
		switch (key){
			case 'q':{
				state = OBJECT_TRANSLATE;
				break;				
			}
			case 'w':{
				state = OBJECT_SCALE;	
				break;
			}	
			case 'e':{
				state = OBJECT_ROTATE;
				break;
			}				
			default:{
				break;
			}
		}
	}
	
	void keyReleased(int key) {
			//printf("MaskObject::keyReleased(key: %i)\n", key);
	}
	
	bool hitTest(int tx, int ty){
		
		
		tx -= centreX;
		ty -= centreY;
		
		int finalXPos = tx;
		int finalYPos = ty;
		
		if (rotation != 0) {
			ofxVec2f p = ofxVec2f(tx, ty);
			p.x -= x;
			p.y -= y;
			p.rotateRad(-rotation);
			p.x += x;
			p.y += y;
			
			finalXPos = p.x + width/2;
			finalYPos = p.y + height/2;
		} else { 
			finalXPos = tx + width/2;
			finalYPos = ty + height/2;
		}
		
//		cout << "Hitest, is " << finalXPos << "," << finalYPos << "inside " << x << "," << y << "|" << width << "," << height << endl;	
//		cout << "Centre XY is " << centreX << "," << centreY << endl;
		
		return ((finalXPos > x) && (finalXPos < x + width) && (finalYPos > y) && (finalYPos < y + height));
	}
	
	void setImagePath(string path){
		graphic.loadImage(path);
		width = graphic.getWidth();
		height = graphic.getHeight();
	}
	
	void setCentre(float _centreX, float _centreY){
		centreX = _centreX;
		centreY = _centreY;
		
		x = 0;
		y = 0;
	}
	
	void setPos(float _x, float _y) {
		x = _x;
		y = _y;
	}
	
	void setSize(float _w, float _h) {
		width = _w;
		height = _h;
	}
	
	void setPosAndSize(float _x, float _y, float _w, float _h) {
		setPos(_x, _y);
		setSize(_w, _h);
	}	
	
public:
	int oldMouseX;
	int oldMouseY;
	float rotation;
	
	objectState state;
	ofImage graphic;
	
	float centreX;
	float centreY;
	ofRectangle oldRect;
	
	int			_mouseX, _mouseY, _mouseButton;
	bool		_mouseOver;
	bool		_mouseDown;
	
	float scaleFactor;
};