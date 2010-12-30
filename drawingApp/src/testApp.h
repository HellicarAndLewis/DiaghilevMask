#ifndef _TEST_APP
#define _TEST_APP

#include "ofxXmlSettings.h"
#include "ofxVectorMath.h"

#define NUM_ELEMENTS 10

#include "ofMain.h"

#include "MaskObject.h"

#include "ofxDirList.h"

struct Moment{
	int x;
	int y;
	int width;
	int height;
	float rotation;
};

struct Element{
	string imageLocation;
	Moment quiet;
	Moment loud;
};

class testApp : public ofBaseApp{
	
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
	void resized(int w, int h);
	
private:
	void saveMaskXML();				// output the currently drawn collection of strokes to XML
	void selectMaskObject(int newMaskIndex);			// select a certain stroke to draw, change the draw order to reflect this
	void drawCross(float centreX, float centreY, float crossWidth, float crossHeight);	
	void loadMaskAndSelectHighestObject(string pathToLoadFrom);
	void toggleQuietLoud();
	void copyQuietToLoud();
private:
	
	ofxXmlSettings loadXML;
	ofxXmlSettings saveXML;
	ofTrueTypeFont TTF;
	string xmlStructure;
	string message;
	
	vector <Element>	maskElements;
	MaskObject * maskObjects[NUM_ELEMENTS];	
	
	ofImage faceTemplate;
	float faceTemplateOrigWidth;
	float faceTemplateOrigHeight;
	float scaleFactor;
	ofPoint drawCentre;
	
	ofxDirList lister;
	
	int currentlySelectedMaskObject;
	int numberOfMaskObjects;
	bool quietNotLoud;
};

#endif

