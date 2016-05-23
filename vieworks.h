#ifndef VWCAM_H
#define VWCAM_H

#include <nan.h>

#include <iostream>

#include "imgBuffer.h"

// for VwGigE
#include "VwGigE.h"
#include "VwCamera.h"
#include "VwImageProcess.h"
//#include "VwGigE.API.h"

using namespace VWSDK;
using namespace v8;

#include "FreeImage.h"

class vwCam : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  void setDefaults();
  void allocate();
  void store(IMAGE_INFO* pImageInfo);
  void open();

 private:
   imgBuffer buffer;
   VwGigE* GigE;
   VwCamera* camera;
   BITMAPINFO* BmpInfo1;
	 UINT imageBufferNumber;
	 OBJECT_INFO* objectInfo;
   PBYTE liveBuffer;
   UINT formatMultiplier;
   PIXEL_FORMAT pixelFormat;
   UINT width,height,bufferSize,numStored;
   bool bReady,bCapturing;
   std::string outputString;

   Nan::Persistent<v8::Function> cb;

  explicit vwCam(double value = 0);
  ~vwCam();

  static void begin(const Nan::FunctionCallbackInfo<v8::Value>& info);
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
  static void stopCapture(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
  double value_;
};

BOOL ConvertPixelFormat( PIXEL_FORMAT ePixelFormat, BYTE* pDest, BYTE* pSource, int nWidth, int nHeight );

#endif
