#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"

	//from kyle example

#include "ofxCvHaarFinder.h"

	//from maharajas app

#include "ofxXmlSettings.h"
#include "ofxVectorMath.h"

	//from drawing app

#define NUM_ELEMENTS 10

	//from previous client app
#define CAM_WIDTH 320
#define CAM_HEIGHT 240
#define ORIG_FACE_SIDE 525

#include "MaskObject.h"

#include "ofxDirList.h"

	//audio analysis

#include "aubioAnalyzer.h"

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

struct Mask{ // a face has several strokes in it, 10 usually
	string XMLLocation;
	float scale;	
	vector <Element> elements;
};

struct OldTrackBlob { // for remembering the last frame with a face in it's information
	ofRectangle rect;
	ofPoint centroid;
};

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
private:
	void drawCross(float centreX, float centreY, float crossWidth, float crossHeight);
	void saveBackground(); //saves the whole screen to a specified location, saveLocation with date stamp
	void pickRandomMask(); 
	void drawFlash(); // flash with alpha so people know they have taken a shot
	void drawMasks();	//draw the mask(s), using live data or, the old info
	void audioReceived(float * input, int bufferSize, int nChannels);	
private:
	ofVideoGrabber 		vidGrabber;
	
	ofxCvColorImage		colorImg;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	ofxCvContourFinder 	contourFinder;
	ofxCvHaarFinder		haarFinder;	
	
	int 				threshold;
	bool				bLearnBackground;	
	
	ofxXmlSettings XML;
	ofTrueTypeFont TTF;
	
	string xmlStructure;
	string message;
	
	vector <Mask> masks; //the masks
	int currentMask;
	int numberOfMaskObjects;
	
	ofPoint drawCentre;
	
	string saveLocation;
	string computerID;
	
	float timeFirstFace; //moment in milliseconds from the start of the app that a face was detected continuously
	float faceTimeElapsed; //since a face was found
	bool faceFound; //was a face found this frame?
	float gracePeriod; //how long you can go without loosing a face in seconds
	float graceActivated; //time since grace activation
	float howMuchGrace; //how much grace has gone
	float currentTime; //the current time	
	
	bool pleaseSaveBackground; //save the background this frame
	bool runFlash; //should flash be decreasing now?
	float flashPeriod;//duration of a flash
	float flashTimeRemaining; //flash time remaining
	float lastFrameTime;
	float deltaFromLastFrame;
	
	MaskObject * maskObjects[NUM_ELEMENTS];
	
		//from aubio demo
	
	float audioLevel;
	
	float * left;
	float * right;
	
	aubioAnalyzer AA;	
	
		//overlays
	ofxDirList  overlayDirList;
	vector <ofImage> overlays;
	int currentOverlayIndex;
};

#endif
