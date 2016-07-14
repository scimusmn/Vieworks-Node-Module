#pragma once
#include <windows.h>
#include <iostream>

class imgBuffer {
protected:
	PBYTE* buffers;
	UINT cBuf;
	UINT nBuf;
	UINT nSz;
	UINT nStored;
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
	PBYTE operator[](int i);
	PBYTE currentFrame();
};
