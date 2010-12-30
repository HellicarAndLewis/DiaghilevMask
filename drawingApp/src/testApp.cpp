#include "testApp.h"
#include "stdio.h"


	//--------------------------------------------------------------
void testApp::setup(){	
	
		//load in the path for loading from...
		//-----------
		//the string is printed at the top of the app
		//to give the user some feedback
	message = "loading settings.xml";
	
		//we load our settings file
		//if it doesn't exist we can still make one
		//by hitting the 's' key
	if( loadXML.loadFile("settings.xml") ){
		message = "Press s to save to mask.xml. Remember to rename the file after you do it. Settings.xml was successfully loaded!";
	}else{
		message = "unable to load settings.xml check data/ folder";
	}
	
		//read the colors from XML
		//if the settings file doesn't exist we assigns default values (170, 190, 240)
	string pathFromXML = loadXML.getValue("LOADLOCATION", "none");	
	
	
		//loading in the face template....
	
	faceTemplate.loadImage("faceTemplate.jpg");
	faceTemplateOrigWidth = faceTemplate.getWidth();
	faceTemplateOrigHeight = faceTemplate.getHeight();
	scaleFactor = 0.60000f;
	faceTemplate.resize(faceTemplateOrigWidth*scaleFactor, faceTemplateOrigHeight*scaleFactor);
	faceTemplate.setAnchorPercent(0.5f, 0.5f);	
	
	drawCentre.x = ofGetWidth()/2.f;
	drawCentre.y = ofGetHeight()/2.f;
	
		//loading in the face elements
	lister.allowExt("png");
	
	for(int i=0; i < NUM_ELEMENTS;i++){
		maskObjects[i] = new MaskObject();
		maskObjects[i]->setup();
	}
	
	currentlySelectedMaskObject = 0;
	numberOfMaskObjects = 0;	
	
	if(pathFromXML != "none"){
		loadMaskAndSelectHighestObject(pathFromXML);		
	}
	
	quietNotLoud = true;
}

	//--------------------------------------------------------------
void testApp::update(){
	ofBackground(255, 255, 255);
	
	for(int i=0; i < numberOfMaskObjects; i++){
		maskObjects[i]->update(scaleFactor);
	}	
}

	//--------------------------------------------------------------
void testApp::draw(){		
	ofSetRectMode(OF_RECTMODE_CORNER);
	
	ofSetColor(0xFFFFFF);
	
	faceTemplate.draw(drawCentre.x, drawCentre.y);
	
	for(int i=0; i < numberOfMaskObjects; i++){
		maskObjects[i]->draw();
	}
	
		//draw the crosshairs so you know where you are
	
	drawCross(drawCentre.x, drawCentre.y, ofGetWidth(), ofGetHeight());		
	
		//draw if its quiet or loud mode
	
	ofSetColor(255, 0, 0);
	
	if(quietNotLoud){
		ofDrawBitmapString("QUIET", 20, ofGetHeight()-30);
	}else{
		ofDrawBitmapString("LOUD", 20, ofGetHeight()-30);
	}
	
		//draw the control instructions
	
	ofSetColor(0,0,255);
	
	string instructions;
	
	instructions = "1-0 to select, q translate, w scale, e rotate, space to toggle quiet/loud, p to copy quiet to loud (when quiet)";
	
	ofDrawBitmapString(instructions, 20, ofGetHeight()-10);
	
		//draw the file name of every file
	
	ofSetColor(255,255,255);
	
	string fileName;
	
	float startY = ofGetHeight() - 300;
	
	for(int i=0; i < numberOfMaskObjects; i++){
		fileName = maskElements[i].imageLocation;
		if(i == currentlySelectedMaskObject){
			ofSetColor(255, 0, 0);
		}else{
			ofSetColor(0, 255, 255);
		}
		
		ofDrawBitmapString(fileName, 20, startY + (i*20));
	}
	
	ofSetColor(255, 0, 0);
	ofDrawBitmapString(message, 20, 20);
}

	//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	maskObjects[currentlySelectedMaskObject]->keyPressed(key);
	
		//change this to mess with the array!
	
	switch (key){
		case '0':
		{
			selectMaskObject(0);
			break;
		}
		case '1':
		{
			selectMaskObject(1);
			break;
		}
		case '2':
		{
			selectMaskObject(2);
			break;
		}
		case '3':
		{
			selectMaskObject(3);
			break;
		}
		case '4':
		{
			selectMaskObject(4);
			break;
		}
		case '5':
		{
			selectMaskObject(5);
			break;
		}
		case '6':
		{
			selectMaskObject(6);
			break;
		}
		case '7':
		{
			selectMaskObject(7);
			break;
		}
		case '8':
		{
			selectMaskObject(8);
			break;
		}
		case '9':
		{
			selectMaskObject(9);
			break;
		}
		case 's':
		{
			saveMaskXML();
			break;
		}
		case ' ':
		{
			toggleQuietLoud();
			break;
		}
		case 'p':
		{
			copyQuietToLoud();
			break;
		}
		case '+':
		{
			scaleFactor += 0.01f;
				//faceTemplate.resize(faceTemplateOrigWidth*scaleFactor, faceTemplateOrigHeight*scaleFactor);
			break;
		}
		case '-':
		{
			if(scaleFactor > 0.02f){
				scaleFactor -= 0.01f;
					//faceTemplate.resize(faceTemplateOrigWidth*scaleFactor, faceTemplateOrigHeight*scaleFactor);
			}
			break;
		}
		default:
		{
			break;
		}
	}	
}

	//--------------------------------------------------------------
void testApp::keyReleased(int key){
	maskObjects[currentlySelectedMaskObject]->keyReleased(key);	
}

	//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	maskObjects[currentlySelectedMaskObject]->mouseMoved(x,y);		
}

	//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	maskObjects[currentlySelectedMaskObject]->mouseDragged(x,y,button);		
}

	//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	maskObjects[currentlySelectedMaskObject]->mousePressed(x,y,button);	
}


	//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	maskObjects[currentlySelectedMaskObject]->mouseReleased(x,y,button);	
}

	//--------------------------------------------------------------
void testApp::resized(int w, int h){
	
}

void testApp::saveMaskXML(){
	saveXML.clear(); //safety
	
	int scaleTagNumber = saveXML.addTag("SCALE");
	saveXML.setValue("SCALE", scaleFactor, scaleTagNumber);
	
	for(int i=0; i< numberOfMaskObjects; i++)
	{		
		int lastTagNumber = saveXML.addTag("MASKOBJECT");
		
		if( saveXML.pushTag("MASKOBJECT", lastTagNumber) ){
				//first add the image location
			int tagNum = saveXML.addTag("IMAGELOCATION");
			saveXML.setValue("IMAGELOCATION", maskElements[i].imageLocation, tagNum);
				//then then quiet details
			tagNum = saveXML.addTag("QUIET_X");
			saveXML.setValue("QUIET_X", maskElements[i].quiet.x, tagNum);
			tagNum = saveXML.addTag("QUIET_Y");
			saveXML.setValue("QUIET_Y", maskElements[i].quiet.y, tagNum);	
			tagNum = saveXML.addTag("QUIET_WIDTH");
			saveXML.setValue("QUIET_WIDTH", maskElements[i].quiet.width, tagNum);
			tagNum = saveXML.addTag("QUIET_HEIGHT");
			saveXML.setValue("QUIET_HEIGHT", maskElements[i].quiet.height, tagNum);
			tagNum = saveXML.addTag("QUIET_ROTATION");
			saveXML.setValue("QUIET_ROTATION", maskElements[i].quiet.rotation, tagNum);	
				//then then loud details
			tagNum = saveXML.addTag("LOUD_X");
			saveXML.setValue("LOUD_X", maskElements[i].loud.x, tagNum);
			tagNum = saveXML.addTag("LOUD_Y");
			saveXML.setValue("LOUD_Y", maskElements[i].loud.y, tagNum);	
			tagNum = saveXML.addTag("LOUD_WIDTH");
			saveXML.setValue("LOUD_WIDTH", maskElements[i].loud.width, tagNum);
			tagNum = saveXML.addTag("LOUD_HEIGHT");
			saveXML.setValue("LOUD_HEIGHT", maskElements[i].loud.height, tagNum);
			tagNum = saveXML.addTag("LOUD_ROTATION");
			saveXML.setValue("LOUD_ROTATION", maskElements[i].loud.rotation, tagNum);					
			
				//pop out to start on the next.....
			saveXML.popTag();
		}		
	}
	
	saveXML.saveFile("mask.xml");
	cout << "Mask saved to mask.xml!" << endl;
	
	message = "Mask saved to mask.xml! Press s to save to mask.xml. Remember to rename the file after you do it.";

}

void testApp::selectMaskObject(int newMaskIndex){
	
	currentlySelectedMaskObject = newMaskIndex;
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

void testApp::loadMaskAndSelectHighestObject(string pathToLoadFrom){
	numberOfMaskObjects = lister.listDir(pathToLoadFrom);
	
	if(numberOfMaskObjects < 1){
		return; //safety
	}
	
	for(int i=0; i < numberOfMaskObjects; i++){
		maskObjects[i]->setImagePath(lister.getPath(i));
		maskObjects[i]->setCentre(drawCentre.x, drawCentre.y);	
		maskObjects[i]->setPos(0, 0);
		
		Element	newMaskElement;
		newMaskElement.imageLocation	= lister.getPath(i);
		newMaskElement.quiet.x			= maskObjects[i]->x;
		newMaskElement.quiet.y			= maskObjects[i]->y;
		newMaskElement.quiet.width		= maskObjects[i]->width;
		newMaskElement.quiet.height		= maskObjects[i]->height;
		newMaskElement.quiet.rotation	= maskObjects[i]->rotation;
		newMaskElement.loud.x			= maskObjects[i]->x;
		newMaskElement.loud.y			= maskObjects[i]->y;
		newMaskElement.loud.width		= maskObjects[i]->width;
		newMaskElement.loud.height		= maskObjects[i]->height;
		newMaskElement.loud.rotation	= maskObjects[i]->rotation;	
		
		maskElements.push_back(newMaskElement);
	}
	
		//select the highest one
	selectMaskObject(numberOfMaskObjects-1);
}

void testApp::toggleQuietLoud(){
	if(quietNotLoud){
			//then its quiet, so lets switch to loud, rembering where we are first...
			//rembering
		for(int i=0; i < numberOfMaskObjects; i++){
			maskElements[i].quiet.x			= maskObjects[i]->x;
			maskElements[i].quiet.y			= maskObjects[i]->y;
			maskElements[i].quiet.width		= maskObjects[i]->width;
			maskElements[i].quiet.height	= maskObjects[i]->height;
			maskElements[i].quiet.rotation	= maskObjects[i]->rotation;			
		}			
		
			//changing
		for(int i=0; i < numberOfMaskObjects; i++){
			
			maskObjects[i]->setPosAndSize(maskElements[i].loud.x, maskElements[i].loud.y, 
										  maskElements[i].loud.width, maskElements[i].loud.height);
			maskObjects[i]->rotation = maskElements[i].loud.rotation;
		}	
		
			//selectMaskObject(numberOfMaskObjects-1);
		
	}else{
			//its loud, so lets be quiet, rmeembering where we are first
			//member
		for(int i=0; i < numberOfMaskObjects; i++){
			maskElements[i].loud.x			= maskObjects[i]->x;
			maskElements[i].loud.y			= maskObjects[i]->y;
			maskElements[i].loud.width		= maskObjects[i]->width;
			maskElements[i].loud.height		= maskObjects[i]->height;
			maskElements[i].loud.rotation	= maskObjects[i]->rotation;			
		}			
		
			//change
		for(int i=0; i < numberOfMaskObjects; i++){
			
			maskObjects[i]->setPosAndSize(maskElements[i].quiet.x, maskElements[i].quiet.y, 
										  maskElements[i].quiet.width, maskElements[i].quiet.height);
			maskObjects[i]->rotation = maskElements[i].quiet.rotation;
		}	
		
			//selectMaskObject(numberOfMaskObjects-1);		
	}
	
	quietNotLoud = !quietNotLoud; //toggle it
}

void testApp::copyQuietToLoud(){
	for(int i=0; i < numberOfMaskObjects; i++){
		maskElements[i].loud.x			= maskElements[i].quiet.x;		
		maskElements[i].loud.y			= maskElements[i].quiet.y;		
		maskElements[i].loud.width		= maskElements[i].quiet.width;	
		maskElements[i].loud.height		= maskElements[i].quiet.height;
		maskElements[i].loud.rotation	= maskElements[i].quiet.rotation;	
	}	
}
