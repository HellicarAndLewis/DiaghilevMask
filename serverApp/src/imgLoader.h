/*
 *  imgLoader.h
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 19/06/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TEXTURELOADER
#define _TEXTURELOADER

#include "ofMain.h"
#include "ofxDirList.h"
#include "ofxThread.h"
#include "ofxXmlSettings.h"

class imgLoader : public ofxThread {   
public:
	imgLoader();
	~imgLoader();
	
	void    start();
	void    stop();
	void    threadedFunction();
	
	void    fetchTextureNames();
	void    loadNextTexture();
	ofImage* getNextTexture();
	void    goForIt();
	
private:
	ofxDirList  dirList;
	string*     textures;
	int         numTextures;
	int         textureIndex;
	bool        go;
	ofxThread thread;
	ofxXmlSettings XML;
	
	ofImage * currTexture;
	ofImage * nextTexture;
	bool imgLoaded;	
	
	string loadLocation;
	
};

#endif