#ifndef VWCAM_H
#define VWCAM_H

#include <nan.h>

#include <iostream>

#include "imgBuffer.h"

// for VwGigE
#include "VwGigE.h"
#include "VwGigE.API.h"
#include "VwCamera.h"
#include "VwImageProcess.h"
#include "PixelFormat/ipixelformat.h"
#include <QtCore/QMutexLocker>
#include <QtCore/QThread>
#include <QtCore/QCoreApplication>

//#include "VwGigE.API.h"

using namespace v8;
using namespace PixelFormat;

#include "FreeImage.h"

class vwCam : public Nan::ObjectWrap {

 public:
  static void Init(v8::Local<v8::Object> exports);
  void setDefaults();
  void allocate();
  void store(IMAGE_INFO* pImageInfo);
  void open();

 private:
   imgBuffer *buffer;
   VWGIGE_HANDLE GigE;
   HCAMERA camera;
   HCAMERA * pCam;
   BITMAPINFO* BmpInfo1;
   IPixelFormat* piPixelFormat;
	 UINT imageBufferNumber;
	 OBJECT_INFO* objectInfo;
   PBYTE liveBuffer;
   PBYTE liveConv;
   PBYTE convertBuffer;
   UINT formatMultiplier;
   PIXEL_FORMAT pixelFormat;
   UINT width,height,bufferSize,numStored;
   bool bReady,bCapturing;
   std::string outputString;

   Nan::Callback* saveCB;
   Nan::Callback* openCB;
   Nan::Callback* endCapCB;

   uv_mutex_t liveImageMutex;

  explicit vwCam();
  virtual ~vwCam();

  static void allocateBuffer(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void GetValue(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void PlusOne(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Multiply(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void output(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void setFrameRate(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void setImageGain(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void start(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void stop(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void save(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void capture(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void isCapturing(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void isReady(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void stopCapture(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getImage(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getWidth(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getHeight(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void idle(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
  double value_;

  static void EIO_Open(uv_work_t* req);
  static void EIO_AfterOpen(uv_work_t* req, int);
  static void storeImage(OBJECT_INFO*, IMAGE_INFO*);

public:
  void handleSaveFinish(int);

protected:
  void run();
};

BOOL ConvertPixelFormat( PIXEL_FORMAT ePixelFormat, BYTE* pDest, BYTE* pSource, int nWidth, int nHeight );

#endif
