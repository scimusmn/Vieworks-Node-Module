#include "imgBuffer.h"
#include <cstring> // memcpy
#include <cstdlib> //realloc



imgBuffer::imgBuffer(){
	nStored=0;
	buffers=0;
	nBuf=0;
	nSz=0;
	cBuf=0;
}

imgBuffer::~imgBuffer(){
	if ( buffers )
	{
		for(unsigned int i=0; i<nBuf; i++){
			delete [] buffers[i];
			buffers[i] = NULL;
		}
		delete [] buffers;
		buffers = NULL;
	}
}

void imgBuffer::allocate(int num, int size){
	if ( buffers )
	{
		for(unsigned int i=0; i<nBuf; i++){
			delete [] buffers[i];
			buffers[i] = NULL;
		}
		delete [] buffers;
		buffers = NULL;
	}

	nBuf=num;
	nSz = size;
	buffers = new PBYTE[num];

	for(unsigned int i=0; i<nBuf; i++){
		buffers[i] = new BYTE[nSz];
	}
}

bool imgBuffer::store(PBYTE pbuf){
	if(buffers){
		bool ret=true;
		if(nStored<nBuf&&nBuf){

			if(buffers[nStored]) memcpy(buffers[nStored++],pbuf,nSz/3);
		}
		else ret = false;
		return ret;
	}
	return 0;
}

void imgBuffer::resetStore(){
	nStored=0;
}

bool imgBuffer::nextFrame(){
	bool ret=false;
	if(ret=(cBuf<nStored-1)){
		cBuf++;
	}
	return ret;
}

bool imgBuffer::prevFrame(){
	bool ret=false;
	if(ret=(cBuf>0)){
		cBuf--;
	}
	return ret;
}

bool imgBuffer::gotoFrame(int num){
	bool ret=false;
	if(ret=(num>=0&&num<nStored)){
		cBuf=num;
	}
	return ret;
}

void imgBuffer::gotoPercent(float perc){
	int temp = perc*nStored;
	if(temp>=0&&temp<nStored){
		cBuf=temp;
	}
}

float imgBuffer::getPercentDone(){
	return float(cBuf)/float(nStored);
}

PBYTE imgBuffer::operator[](int i){
	return buffers[i];
}

PBYTE imgBuffer::currentFrame(){
	PBYTE ret=0;
	if(buffers) ret = buffers[cBuf];
	return ret;
}
