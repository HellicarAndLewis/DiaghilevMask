/*
 *  imgLoader.cpp
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 19/06/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "imgLoader.h"

//--------------------------------------------------------------
imgLoader::imgLoader() {
    imgLoaded = false;
    dirList.setVerbose(false);
	
	//now lets load in the XML if it exists
	cout << "loading location.xml"<< endl;;
	
	//we load our settings file
	if( XML.loadFile("location.xml") ){
		cout << "location.xml loaded!"<< endl;;
	}else{
		cerr << "unable to load location.xml check data/ folder"<< endl;;
	}
	
	int numLoadTags = XML.getNumTags("LOADLOCATION");
	cout << "Number of LoadLocation tags: " << numLoadTags << endl;
	loadLocation = "NONE";
	
	if(numLoadTags == 1)
	{
		std::ostringstream startOss;
		startOss << XML.getValue("LOADLOCATION", "NONE");
		loadLocation = startOss.str();
		cout << "Load location" << loadLocation << endl;
	}else{
		cerr << "No load location found!" << endl;
	}	
	
	imgLoaded = false;
    fetchTextureNames();
    start();	
}

//--------------------------------------------------------------
imgLoader::~imgLoader() {   
    delete [] textures;
    textures = NULL;
}

//--------------------------------------------------------------
void imgLoader::fetchTextureNames() {
	if(loadLocation != "NONE")
	{		
		ofDisableDataPath();
		// fetch all the texture file names
		dirList.reset();
		dirList.allowExt("jpg");
		numTextures = dirList.listDir(loadLocation);
		cout << "Number of images found: " << numTextures << endl;
		textures = new string[numTextures];
		for (int i=0; i < numTextures; i++) {
			textures[i] = loadLocation+dirList.getName(i);
		}
		
		// load the first texture
		textureIndex = -1;
		loadNextTexture();
		ofEnableDataPath();
	}else{
		cerr << "IMGLOADER: Invalid path, or no route to path." << endl;
	}
	
	currTexture=NULL;
    nextTexture=NULL;
}

//--------------------------------------------------------------
void imgLoader::loadNextTexture() {
	ofDisableDataPath();	
	//textureIndex = (textureIndex+1)%numTextures;
	//now we want textureIndex to be a random number between 0 and numTextures
	textureIndex = (int)(ofRandom(0, (float)numTextures));
	nextTexture = new ofImage();
	nextTexture->setUseTexture(false);
	nextTexture->loadImage(textures[textureIndex]);
	printf("Loading image %d: %s\n", textureIndex,  textures[textureIndex].c_str());
	imgLoaded=true;
	ofEnableDataPath();	
}

//--------------------------------------------------------------
ofImage * imgLoader::getNextTexture() {
    if(imgLoaded){
        if(currTexture)
            delete currTexture;
        currTexture=nextTexture;
        imgLoaded=false;
        return currTexture;
    }else{
        return NULL;
    }
}

//--------------------------------------------------------------
void imgLoader::goForIt() {
    go = true;
}

//--------------------------------------------------------------
void imgLoader::start() {
    startThread(true, false);  // blocking, verbose
}

//--------------------------------------------------------------
void imgLoader::stop() {
    stopThread();
}

//--------------------------------------------------------------
void imgLoader::threadedFunction() {
    while (isThreadRunning() != 0) {
		if (!imgLoaded) {
			loadNextTexture();
		}
    }
}
