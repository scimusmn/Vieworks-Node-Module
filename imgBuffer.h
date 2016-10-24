#pragma once
#include <iostream>
#include <VwGigE.Global.h>
#include "VwGigE.API.h"
#include "VwCamera.h"
#include "VwImageProcess.h"
#include <QtCore/QThread>
#include "FreeImage.h"

class imgBuffer : public QThread {
	Q_OBJECT

protected:
	PBYTE* buffers;
	UINT cBuf;
	UINT nBuf;
	UINT nSz;
	UINT nStored;
	PBYTE convertBuffer;
	string saveDir;
public:
	imgBuffer(QObject *parent = 0);
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
signals:
	void doneSaving(int);
public slots:
	void save(string d);
protected:
	void run();
};
