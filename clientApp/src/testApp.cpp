#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	audioLevel = 0.f;
	
		// 0 output channels, 
		// 2 input channels
		// 44100 samples per second
		// 256 samples per buffer
		// 4 num buffers (latency)
	
		//setup of sound input
	ofSoundStreamSetup(2, 2, this, 44100, 256, 4);	
	left = new float[256];	
	right = new float[256];
	
	AA.setup();
	
	currentMask = 0;
	numberOfMaskObjects = 0;
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(CAM_WIDTH,CAM_HEIGHT);
	
    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);
	
	bLearnBackground = true;
	threshold = 80;
	
		//lets load in our face xml file
	haarFinder.setup("haarcascade_frontalface_default.xml");
	
		//now lets load in the XML if it exists
		//	cout << "loading faces.xml"<< endl;
	
		//we load our settings file
	if( XML.loadFile("masks.xml") ){
			//		cout << "faces.xml loaded!"<< endl;
	}else{
		cerr << "unable to load masks.xml check data/ folder"<< endl;
	}
	
	int numIDTags = XML.getNumTags("COMPUTERID");
		//	cout << "Number of ID tags: " << numIDTags << endl;
	
	if(numIDTags == 1)
	{
		std::ostringstream startOss;
		startOss << XML.getValue("COMPUTERID", "NONE");
		computerID = startOss.str();
			//		cout << "Computer ID " << computerID << endl;
	}else{
		cerr << "No ID  found!" << endl;
	}	
	
	int numSaveTags = XML.getNumTags("SAVELOCATION");
	cout << "Number of save tags: " << numSaveTags << endl;
	
	if(numSaveTags == 1)
	{
		std::ostringstream startOss;
		startOss << XML.getValue("SAVELOCATION", "NONE");
		saveLocation = startOss.str();
			//		cout << "Save location " << saveLocation << endl;
	}else{
		cerr << "No save location found!" << endl;
	}
	
		//lets see how many <MASKLOCATION> </MASKLOCATION> tags there are in the xml file
	int numMaskTags = XML.getNumTags("MASKLOCATION");
	
	//resize the vector to the number of masks we have from the XML file
	
	masks.resize(numMaskTags);
	
	for(int i = 0; i < numMaskTags; i++){
		std::ostringstream maskXMLLocationOSS;
		maskXMLLocationOSS << XML.getValue("MASKLOCATION", "maskData/catMask.xml", i);
		string maskXMLLocation = maskXMLLocationOSS.str();
		masks[i].XMLLocation = maskXMLLocation;
	}
	
	for(int i = 0; i < numMaskTags; i++)
	{
		XML.clear();
		
			//now for all the maks, load in all their elements
			//now lets load in the XML if it exists
		cout << "loading: " << masks[i].XMLLocation << endl;
		
			//we load our settings file
		if( XML.loadFile(masks[i].XMLLocation) ){
		}else{
			cerr << "unable to load " << masks[i].XMLLocation << "check data/ folder"<< endl;
		}
		
			//lets get the scale
		float maskScale = XML.getValue("SCALE", 0.7f, 0); //0th...
		
		cout << "Scale of the mask" << masks[i].XMLLocation << ", is: " << maskScale << " (0.7f) default" << endl;
		
		masks[i].scale = maskScale;
		
			//lets see how many <MASKOBJECT> </MASKOBJECT> tags there are in the xml file
		int numMaskObjectTags = XML.getNumTags("MASKOBJECT");
		
			//		cout << "Number of strokes: " << numStrokeTags << ", in " << faces[i].XMLLocation << endl;
		
			//resize the vector to the number of strokes we have from the XML file
		
		masks[i].elements.resize(numMaskObjectTags);
		
			//if there is at least one <MASKOBJECT> tag we can read the list of points
			//and then try and draw it as a line on the screen
		for(int j = 0; j < numMaskObjectTags; j++){
				//we push into the jth MASKOBJECT tag
				//this temporarirly treats the tag as
				//the document root.
			XML.pushTag("MASKOBJECT", j);
			
			std::ostringstream imageOSS;
			imageOSS << XML.getValue("IMAGELOCATION", "maskImages/cat/000_Teeth.png");
			string imageLocation = imageOSS.str();
			
			int quietX = XML.getValue("QUIET_X", 0);
			int quietY = XML.getValue("QUIET_Y", 0); 
			int quietWidth = XML.getValue("QUIET_WIDTH", 100);
			int quietHeight = XML.getValue("QUIET_HEIGHT", 10);
			float quietRotation = XML.getValue("QUIET_ROTATION", 0.f);
			
			int loudX = XML.getValue("LOUD_X", 10);
			int loudY = XML.getValue("LOUD_Y", 10);
			int loudWidth = XML.getValue("LOUD_WIDTH", 10);
			int loudHeight = XML.getValue("LOUD_HEIGHT", 100);
			float loudRotation = XML.getValue("LOUD_ROTATION", 1.f);			
			
			masks[i].elements[j].imageLocation = imageLocation;
			
			masks[i].elements[j].quiet.x = quietX;
			masks[i].elements[j].quiet.y = quietY;
			masks[i].elements[j].quiet.width = quietWidth;
			masks[i].elements[j].quiet.height = quietHeight;
			masks[i].elements[j].quiet.rotation = quietRotation;
			
			masks[i].elements[j].loud.x = loudX;
			masks[i].elements[j].loud.y = loudY;
			masks[i].elements[j].loud.width = loudWidth;
			masks[i].elements[j].loud.height = loudHeight;
			masks[i].elements[j].loud.rotation = loudRotation;			
			
				//this pops us out of the MASKOBJECT tag
				//sets the root back to the xml document
			XML.popTag();
		}			
	}
	
		//initialise the Mask objects that will hold the images on screen
	
	for(int i=0; i < NUM_ELEMENTS;i++){
		maskObjects[i] = new MaskObject();
		maskObjects[i]->setup();
	}
	
	currentMask = 0;
	numberOfMaskObjects = 0;	
	
		// do an initial random selection of a certain mask here...
	
	pickRandomMask();
	
	drawCentre.x = ofGetWidth()/2.f;
	drawCentre.y = ofGetHeight()/2.f;
	
	faceFound = false;
	
	faceTimeElapsed = 0.f;
	timeFirstFace = 0.f;
	gracePeriod = 0.1f; // one tenth of a second is the graceperiod
	graceActivated = 0.f;
	howMuchGrace = 0.f;
	currentTime = 0.f;
	
	pleaseSaveBackground = false; //save the background this frame
	runFlash = false; //should flash be decreasing now?
	flashPeriod = 0.2f;//duration of a flash
	flashTimeRemaining = 0.f; //flash time remaining	
	
	deltaFromLastFrame = 0.f;
	lastFrameTime = ofGetElapsedTimef();
	
	ofHideCursor();
	
	overlayDirList.reset();
	overlayDirList.allowExt("png");
	int numOverlays = overlayDirList.listDir("overlays");	
	
	overlays.resize(numOverlays);
	
	if(numOverlays > 0){
		for(int i=0; i< overlays.size(); i++){
			overlays[i].loadImage(overlayDirList.getPath(i));
		}
	}else{
		cerr << "No overlay files found! Problem" << endl;
	}
	
	currentOverlayIndex = 0;
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);
	
//	audioLevel = (float)mouseX/(float)ofGetWidth();
	
	audioLevel = AA.amplitude;
	
	if(audioLevel < 0.f){
		audioLevel = 0.f;
	}
	
	if(audioLevel > 1.f){
		audioLevel = 1.f;
	} //safety...
	
	currentTime  = ofGetElapsedTimef();
	howMuchGrace = currentTime-graceActivated;
	deltaFromLastFrame = currentTime - lastFrameTime;	
    
    bool bNewFrame = false;
	
	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
	
	if (bNewFrame){
		
		colorImg.setFromPixels(vidGrabber.getPixels(), CAM_WIDTH,CAM_HEIGHT);
		
        grayImage = colorImg;
		if (bLearnBackground == true){
			grayBg = grayImage;		
				// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBackground = false;
		}
		
		haarFinder.findHaarObjects(grayImage, 30.f, 30.f); //new way of finding it with everthing
														   //haarFinder.findHaarObjects(grayImage, 10, 99999999, 10);
		
			// take the abs value of the difference between background and incoming and then threshold:
			//grayDiff.absDiff(grayBg, grayImage);
			//grayDiff.threshold(threshold);
		
			// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
			// also, find holes is set to true so we will get interior contours as well....
			//contourFinder.findContours(grayDiff, 20, (CAM_WIDTH*CAM_HEIGHT)/3, 10, true);	// find holes
	}
	
	int numFace = haarFinder.blobs.size();
	
	if(numFace > 0)
	{
		graceActivated = 0.f; // if we have faces graceactivated should be 0.f, so we can work out
							  // the transition to graceperiod next time
		if(faceFound) //already found a face on the previous frame, or in graceperiod
		{
				//just let the timer keep going
			faceTimeElapsed = currentTime - timeFirstFace;
		}else{
				//start the timer;
			timeFirstFace = currentTime;
			faceTimeElapsed = 0.f;
			faceFound = true; //then we have some faces, so set faceFound to true
		}
	}else
	{
		if(faceFound){ //so we had a face last frame, but not this one, or we are in grace
					   //just let the timer keep going
			faceTimeElapsed = currentTime - timeFirstFace;
			
			if(howMuchGrace == currentTime){
					//then we need to activate grace on this update, as we know graceactivated is 0 from 
					//a pre
				graceActivated = currentTime;
					//facefound stays true			
			}else{
				if(howMuchGrace > gracePeriod){ // run out of grace
					faceFound = false;
					faceTimeElapsed = 0.f; // zero this immediately, not on next frame
				}else{
						//stay in the graceperiod, do nothing
				}
			}
		}else{
			faceTimeElapsed = 0.f;
			graceActivated = 0.f;
		}
	}
	
	if(runFlash){
		flashTimeRemaining -= deltaFromLastFrame;
		
		if(flashTimeRemaining < 0.f)
		{
			runFlash = false;
		}
		
	}
	
	lastFrameTime = ofGetElapsedTimef();
	
	if ((ofGetFrameNum()%300 == 0) && (overlays.size() > 1)){
		currentOverlayIndex = (int)ofRandom(0, overlays.size()-1);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	ofNoFill();
	ofSetRectMode(OF_RECTMODE_CORNER); //make sure!	
	
	colorImg.draw(0, 0, ofGetWidth(), ofGetHeight());
	
	int numFace = haarFinder.blobs.size(); // as we know it's sorted in order of biggest, only draw 1
	
	if(numFace > 0){
		drawMasks(); //draw faces with live data
	}else{
			//pick another mask....
		pickRandomMask();
	}
	
	// finally, a report:

	//	ofSetColor(0xffffff);
	//	char reportStr[1024];
	//	sprintf(reportStr, "SPACE to save image, press 'b' to capture bg\nthreshold %i (press: +/-)", threshold);
	//	ofDrawBitmapString(reportStr, 20, 600);
	//	
	//	ofFill();
	//	ofSetColor(0x000000);
	//	ofRect(60, 630, 400,100);

	//some text to explain whats what

	//	std::ostringstream localOSS;
	//	string strokeDrawOrderInfo;
	//	int currentStrokeToDraw = strokeDrawList.back();
	//	
	//	localOSS << "Selected:" << currentStrokeToDraw 
	//	<< ", Start " << strokes[currentStrokeToDraw].startImageLocation
	//	<< "\n Middle " << strokes[currentStrokeToDraw].middleImageLocation
	//	<< "\n End " << strokes[currentStrokeToDraw].endImageLocation;
	//	
	//	strokeDrawOrderInfo = localOSS.str();
	//	
	//	ofDrawBitmapString(strokeDrawOrderInfo, 75, 670);

	//	string faceTimeElapsedStr = "FaceTime:" + ofToString(faceTimeElapsed)+ "HowMuchGraceTime:" + ofToString(howMuchGrace);
	//	ofSetColor(0xff0000);
	//	ofDrawBitmapString(faceTimeElapsedStr, 75, 760);
	
		//finally, overlay
	ofSetColor(255,255,255);
	
	ofEnableAlphaBlending();	
	
	overlays[currentOverlayIndex].draw(0,0,1024,768);
	
	ofDisableAlphaBlending();
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	switch (key){
		case ' ':
		{
			pleaseSaveBackground = true;
			break;				
		}
			
		case 'b':{
			bLearnBackground = true;
			break;			
		}
		case '+':{
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;			
		}
		case '-':{
			threshold --;
			if (threshold < 0) threshold = 0;
			break;			
		}
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
void testApp::windowResized(int w, int h){

}

void testApp::drawCross(float centreX, float centreY, float crossWidth, float crossHeight){
	ofSetColor(0xFF0000);
	
	ofPoint left, right, top, bottom;
	
	left.x = centreX - crossWidth/2.f;
	left.y = centreY;
	
	right.x = centreX + crossWidth/2.f;
	right.y = centreY;
	
	top.x = centreX;
	top.y = centreY - crossHeight/2.f;
	
	bottom.x = centreX;
	bottom.y = centreY + crossHeight/2.f;
	
	ofLine(left.x, left.y, right.x, right.y);
	ofLine(top.x, top.y, bottom.x, bottom.y);	
}

void testApp::saveBackground()
{
	if(saveLocation != "NONE" && computerID != "NONE"){
		ofImage screen;
		screen.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
		screen.grabScreen(0,0,ofGetWidth(), ofGetHeight());
		
		std::ostringstream stampOSS;
		stampOSS << ofGetYear() << "_" << ofGetMonth() << "_" << ofGetDay() << 	"_" 
		<< ofGetHours() << "_" << ofGetMinutes() << "_" << ofGetSeconds() << "_" << ofGetFrameNum();
		
		std::ostringstream finalOSS;
		finalOSS << saveLocation << computerID << stampOSS.str() << ".jpg";
		string completeSaveLocation = finalOSS.str();
		
			//cout << "Trying to save a grab to: " << completeSaveLocation << endl;
		
		ofDisableDataPath();
		screen.saveImage(completeSaveLocation);
		ofEnableDataPath();	
		
		pleaseSaveBackground = false; //save the background this frame
		runFlash = true; //should flash be decreasing now?
		flashTimeRemaining = flashPeriod; //flash time remaining		
	}else{
		cerr << "Invalid save path or path not loaded or invalid computer ID" << endl;
	}		
}

void testApp::pickRandomMask()
{
	int randomMaskIndex = ofRandom(0, masks.size()-1);
	
	currentMask = randomMaskIndex;
	
		//currentMask = 1;
	
	numberOfMaskObjects = masks[currentMask].elements.size();
	
	if(numberOfMaskObjects < 1){
		return; //safety
	}
	
	for(int i=0; i < numberOfMaskObjects; i++){
		maskObjects[i]->setImagePath(masks[currentMask].elements[i].imageLocation);
		maskObjects[i]->setCentre(drawCentre.x, drawCentre.y); //temporary for now, need to set this for every face, and the rest below too
	
		maskObjects[i]->x = masks[currentMask].elements[i].quiet.x;
		maskObjects[i]->y = masks[currentMask].elements[i].quiet.y;
		maskObjects[i]->width = masks[currentMask].elements[i].quiet.width;
		maskObjects[i]->height = masks[currentMask].elements[i].quiet.height;
		maskObjects[i]->rotation = masks[currentMask].elements[i].quiet.rotation;
	}
}

void testApp::drawFlash(){
		//draw the flash
	
	float alphaValue = (flashTimeRemaining/flashPeriod)*255.f;
	ofEnableAlphaBlending();
	
		//cout << "Alpha value:"<< alphaValue << endl;
	
	ofSetColor(255,255,255, (int) alphaValue);
	ofFill();
	ofRect(0,0,ofGetWidth(), ofGetHeight());
	ofNoFill();
	
	ofDisableAlphaBlending();
	
}

void testApp::drawMasks(){
	
	ofSetColor(255,255,255);
	
	ofEnableAlphaBlending();	
	
	int numFace = haarFinder.blobs.size(); // as we know it's sorted in order of biggest, only draw 1

	if(numFace > 0){ //if we have some faces...
		for(int i = 0; i < numFace; i++){
			float x = haarFinder.blobs[i].boundingRect.x;
			float y = haarFinder.blobs[i].boundingRect.y;
			float w = haarFinder.blobs[i].boundingRect.width;
			float h = haarFinder.blobs[i].boundingRect.height;
			float cx = haarFinder.blobs[i].centroid.x;
			float cy = haarFinder.blobs[i].centroid.y;
			
			float scaleXRatio = (w/CAM_WIDTH)/ ORIG_FACE_SIDE;
			scaleXRatio *= ofGetWidth();
				
			float scaleYRatio = (h/CAM_HEIGHT)/ ORIG_FACE_SIDE;
			scaleYRatio *= ofGetHeight();
			
			float blobX = x/CAM_WIDTH;
			blobX *= ofGetWidth();
			
			float blobY = y/CAM_HEIGHT;
			blobY *= ofGetHeight();
				
			float blobW = w/CAM_WIDTH;
			blobW *= ofGetWidth();
				
			float blobH = h/CAM_HEIGHT;
			blobH *= ofGetHeight();
				
			float blobCX = cx/CAM_WIDTH;
			blobCX *= ofGetWidth();
			
			float blobCY = cy/CAM_HEIGHT;
			blobCY *= ofGetHeight();
			
			if(numberOfMaskObjects < 1){
				return; //safety
			}
				
			for(int j=0; j < numberOfMaskObjects; j++){
				maskObjects[j]->setCentre(blobCX, blobCY);			
				
				maskObjects[j]->x = ofLerp(masks[currentMask].elements[j].quiet.x, masks[currentMask].elements[j].loud.x, audioLevel);
				maskObjects[j]->y = ofLerp(masks[currentMask].elements[j].quiet.y, masks[currentMask].elements[j].loud.y, audioLevel);
				maskObjects[j]->width = ofLerp(masks[currentMask].elements[j].quiet.width, masks[currentMask].elements[j].loud.width, audioLevel);
				maskObjects[j]->height = ofLerp(masks[currentMask].elements[j].quiet.height, masks[currentMask].elements[j].loud.height, audioLevel);
				maskObjects[j]->rotation = ofLerp(masks[currentMask].elements[j].quiet.rotation, masks[currentMask].elements[j].loud.rotation, audioLevel);				
				
				//maskObjects[j]->update(masks[currentMask].elements[j].scale);
				maskObjects[j]->update(scaleXRatio);
					
				maskObjects[j]->draw();
			}
		}
	}
	
	ofDisableAlphaBlending();	
	
	if(pleaseSaveBackground)
		saveBackground();
	
	if(runFlash)
		drawFlash();
}

	//--------------------------------------------------------------
void testApp::audioReceived (float * input, int bufferSize, int nChannels){	
	
	
		// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];		
		right[i] = input[i*2+1];
	}
	
	AA.processAudio(left, bufferSize);
}