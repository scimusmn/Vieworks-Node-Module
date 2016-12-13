#pragma once

#define LOG4CPP_FIX_ERROR_COLLISION 1

#include <iostream>
#include <nan.h>
#include <VwGigE.Global.h>
#include "VwGigE.API.h"
#include "VwCamera.h"
#include "VwImageProcess.h"
#include "FreeImage.h"

class imgBuffer {

protected:
	PBYTE* buffers;
	UINT cBuf;
	UINT nBuf;
	UINT nSz;
	UINT nStored;
	PBYTE convertBuffer;
	string saveDir;
	Nan::Callback* saveCB;
public:
	imgBuffer();
	~imgBuffer();
	void allocate(int num, int size);
	bool store(PBYTE pbuf);
	void resetStore();
	bool nextFrame();
	bool prevFrame();
	bool gotoFrame(int num);
	int storageNumber(){ return nStored; }
	int maxFrames(){ return nBuf; }
	int currentFrameNumber(){ return cBuf;}
	void gotoPercent(float perc);
	float getPercentDone();
	void setSaveDirectory(string d){ saveDir = d.c_str();}
	PBYTE operator[](int i);
	PBYTE currentFrame();

	static void EIO_Save(uv_work_t* req);
  static void EIO_AfterSave(uv_work_t* req, int);
public:
	void save(string d, Nan::Callback*);
protected:
	void run();
};
