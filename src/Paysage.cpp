//
//  Paysage.cpp
//  promenade_mobile_tvs
//
//  Created by Alain Barthelemy on 27/10/13.
//
//

#include "Paysage.h"

Paysage::Paysage(float _x, float _y, float _w, float _h)
{
	x = _x;
	y = _y;
	w = _w;
    h = _h;
	
    imgHIndex = 0;
    imgBIndex = 0;
    
    loadingHIndex=0;
    loadingBIndex=0;
    FIRST_LOAD_H=true;
    FIRST_LOAD_B=true;
    
    
    alpha = 255;
    timestamp = ofGetUnixTime();
    ON_TRANSITION = false;
    

    
    loadH();
    loadB();
    
    

    
}


void Paysage::update(){
	//w = ofGetWidth();
    //h = ofGetHeight();
    

    
    if(ofGetUnixTime() <= (timestamp + STATIC_LENGHT) && !ON_TRANSITION){
    }else if(ofGetUnixTime() > (timestamp + STATIC_LENGHT) && !ON_TRANSITION){
        timestamp = ofGetUnixTime();//reset the counter
        trans_ts = ofGetElapsedTimeMillis();
        ON_TRANSITION = true;
    }else if (ofGetUnixTime() <= (timestamp + TRANS_LENGHT) && ON_TRANSITION){
    }else if(ofGetUnixTime() > (timestamp + TRANS_LENGHT) && ON_TRANSITION){
        timestamp = ofGetUnixTime();//reset the counter
        imgHIndex++;
        if(imgHIndex >= IMG_STACK_LENGHT)
            imgHIndex = 0;
        imgBIndex++;
        if(imgBIndex >= IMG_STACK_LENGHT)
            imgBIndex = 0;
        
        loadH();
        loadB();
        
        ON_TRANSITION = false;
        
    }
	
}



void Paysage::draw(){
	
    if(ON_TRANSITION)
        drawTransition();
    else
        drawStatic();
    
}

void Paysage::drawStatic(){
    
	if(imgH[imgHIndex].bAllocated()){
        
        if(hasJustArrived(imgH[imgHIndex])) prepareLoadedImageH(&imgH[imgHIndex]);
        
		ofSetColor(255);
        imgH[imgHIndex].draw(x, y);
    }
	if(imgB[imgBIndex].bAllocated()){
        
        if(hasJustArrived(imgB[imgBIndex])) prepareLoadedImageB(&imgB[imgBIndex]);
		ofSetColor(255);
        imgB[imgBIndex].draw(x, y+h/2);
    }
    
}
void Paysage::drawTransition(){
    
    alpha = (int)floor(255*((float)(ofGetElapsedTimeMillis() - trans_ts)/(TRANS_LENGHT*1000)));
    int nextHIndex = (imgHIndex >= IMG_STACK_LENGHT-1) ? 0 : imgHIndex+1;
    int nextBIndex = (imgBIndex >= IMG_STACK_LENGHT-1) ? 0 : imgBIndex+1;
    
    ofEnableAlphaBlending();
    
	if(imgH[imgHIndex].bAllocated() && imgH[nextHIndex].bAllocated()){
        
        if(hasJustArrived(imgH[imgHIndex])) prepareLoadedImageH(&imgH[imgHIndex]);
        if(hasJustArrived(imgH[nextHIndex])) prepareLoadedImageH(&imgH[nextHIndex]);
		      
        ofSetColor(255,255,255,255-alpha);
        imgH[imgHIndex].getTextureReference().draw(x,y);
        ofSetColor(255,255,255,alpha);
        imgH[nextHIndex].getTextureReference().draw(x,y);
    }
	if(imgB[imgBIndex].bAllocated() && imgB[nextBIndex].bAllocated()){
        
        if(hasJustArrived(imgB[imgBIndex])) prepareLoadedImageB(&imgB[imgBIndex]);
        if(hasJustArrived(imgB[nextBIndex])) prepareLoadedImageB(&imgB[nextBIndex]);
		
        ofSetColor(255,255,255,255-alpha);
        imgB[imgBIndex].getTextureReference().draw(x,y+h/2);
        ofSetColor(255,255,255,alpha);
        imgB[nextBIndex].getTextureReference().draw(x,y+h/2);
    }
    ofDisableAlphaBlending();
    
}

void Paysage::prepareLoadedImageH(ofImage* image){
    image->resize(w, (int)floor(image->getHeight()*w/image->getWidth()));
    image->crop(0, 0, w, h/2);
}
void Paysage::prepareLoadedImageB(ofImage* image){
    image->resize(w, (int)floor(image->getHeight()*w/image->getWidth()));
    image->crop(0, image->getHeight() - h/2, w, h/2);
}
bool Paysage::hasJustArrived(ofImage image){
    if(image.isAllocated() && image.getWidth() != w && image.getHeight() != h/2){/// HACK !!!
        return true;
    }else{
        return false;
    }
}

/*void Paysage::urlResponse(ofHttpResponse & response){
    
	if(response.status==200 && response.request.name == "imgHPaysage"){
		imgH[loadingHIndex].loadImage(response.data);
        
        //fit width
        imgH[loadingHIndex].resize(w, (int)floor(imgH[loadingHIndex].getHeight()*w/imgH[loadingHIndex].getWidth()));
        imgH[loadingHIndex].crop(0, 0, w, h/2);
        
        
		//loadingH=false;
        
        loadingHIndex++;
        if(loadingHIndex >= PAYSAGE_IMG_STACK_LENGHT)
            loadingHIndex = 0;

        if(FIRST_LOAD_H){ // in the beginning we load twice
            loadH();
            FIRST_LOAD_H = false;
        }
        
        
	}else if(response.status==200 && response.request.name == "imgBPaysage"){
		imgB[loadingBIndex].loadImage(response.data);
        

        //fit width
        imgB[loadingBIndex].resize(w, (int)floor(imgB[loadingBIndex].getHeight()*w/imgB[loadingBIndex].getWidth()));
        imgB[loadingBIndex].crop(0, imgB[loadingBIndex].getHeight() - h/2, w, h/2);
        

        loadingBIndex++;
        if(loadingBIndex >= PAYSAGE_IMG_STACK_LENGHT)
            loadingBIndex = 0;

        if(FIRST_LOAD_B){ // in the beginning we load twice
            loadB();
            FIRST_LOAD_B = false;
        }
        
	}else {
		//cout << response.status << " " << response.error << endl;
		//if(response.status!=-1) loading=false;
	}
}*/

void Paysage::loadH(){
    
    
    if(FIRST_LOAD_H){ // in the beginning we load twice
        
        imgH[loadingHIndex].clear();
        imgH[loadingHIndex+1].clear();
        
        int imgNbr = (int)floor(1+ofRandom(1)*6.99);
        loader.loadFromURL(imgH[loadingHIndex], "http://pm.alainbarthelemy.com/paysage/img" + ofToString(imgNbr) + ".jpg");
        imgNbr = (int)floor(1+ofRandom(1)*6.99);
        loader.loadFromURL(imgH[loadingHIndex+1], "http://pm.alainbarthelemy.com/paysage/img" + ofToString(imgNbr) + ".jpg");
        
        
        
        loadingHIndex+=2;
        if(loadingHIndex >= IMG_STACK_LENGHT)
            loadingHIndex = 0;
        
        FIRST_LOAD_H = false;
    }else{
        
        imgH[loadingHIndex].clear();
        
        int imgNbr = (int)floor(1+ofRandom(1)*6.99);
        loader.loadFromURL(imgH[loadingHIndex], "http://pm.alainbarthelemy.com/paysage/img" + ofToString(imgNbr) + ".jpg");
        
        
        
        loadingHIndex++;
        if(loadingHIndex >= IMG_STACK_LENGHT)
            loadingHIndex = 0;
        
    }
}
void Paysage::loadB(){
    
    if(FIRST_LOAD_B){ // in the beginning we load twice
        
        imgB[loadingBIndex].clear();
        imgB[loadingBIndex+1].clear();
        
        int imgNbr = (int)floor(1+ofRandom(1)*6.99);
        loader.loadFromURL(imgB[loadingBIndex], "http://pm.alainbarthelemy.com/paysage/img" + ofToString(imgNbr) + ".jpg");
        imgNbr = (int)floor(1+ofRandom(1)*6.99);
        loader.loadFromURL(imgB[loadingBIndex+1], "http://pm.alainbarthelemy.com/paysage/img" + ofToString(imgNbr) + ".jpg");
        
        loadingBIndex+=2;
        if(loadingBIndex >= IMG_STACK_LENGHT)
            loadingBIndex = 0;
        
        FIRST_LOAD_B = false;
    }else{
        
        imgB[loadingBIndex].clear();
        
        int imgNbr = (int)floor(1+ofRandom(1)*6.99);
        loader.loadFromURL(imgB[loadingBIndex], "http://pm.alainbarthelemy.com/paysage/img" + ofToString(imgNbr) + ".jpg");
        
        //[loadingBIndex].resize(10,10);
        
        loadingBIndex++;
        if(loadingBIndex >= IMG_STACK_LENGHT)
            loadingBIndex = 0;
        
    }
    
}

void Paysage::exit() {
    loader.stopThread();
}