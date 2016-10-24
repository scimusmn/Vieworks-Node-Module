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

class vwCam : public QObject, public Nan::ObjectWrap {
  Q_OBJECT

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
   BITMAPINFO* BmpInfo1;
   IPixelFormat* piPixelFormat;
   QCoreApplication * app = NULL;
   QThread * thread = NULL;

   QMutex              liveImageMutex;

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

  explicit vwCam(QObject *parent = 0);
  virtual ~vwCam();

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
  static void isReady(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void stopCapture(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getImage(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getWidth(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getHeight(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void idle(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
  double value_;

signals:
  void saveSignal(string);
public slots:
  void handleSaveFinish(int);
  void onQAppStart();

protected:
  void run();
};

BOOL ConvertPixelFormat( PIXEL_FORMAT ePixelFormat, BYTE* pDest, BYTE* pSource, int nWidth, int nHeight );

#endif
