#include "vieworks.h"

Nan::Persistent<v8::Function> vwCam::constructor;

template <class T>
char* to_charStar(T var){
  ostringstream ss;
  ss << var;
  return (char*)(ss.str().c_str());
}

vwCam::vwCam(QObject *parent) : QObject(parent) {
}

vwCam::~vwCam() {
  if(camera){
    CameraClose(camera);
    delete camera;
  }
  if(GigE){
    CloseVwGigE(GigE);
    delete GigE;
  }
  if(liveBuffer){
    delete liveBuffer;
  }
  if(liveConv){
    delete liveConv;
  }

  if(convertBuffer){
    delete convertBuffer;
  }

  thread->quit();
  //buffer->quit();
}

void vwCam::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("camera").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "output", output);
  Nan::SetPrototypeMethod(tpl, "allocateBuffer", allocateBuffer);
  Nan::SetPrototypeMethod(tpl, "begin", begin);
  Nan::SetPrototypeMethod(tpl, "setImageGain", setImageGain);
  Nan::SetPrototypeMethod(tpl, "setFrameRate", setFrameRate);
  Nan::SetPrototypeMethod(tpl, "getWidth", getWidth);
  Nan::SetPrototypeMethod(tpl, "getHeight", getHeight);
  Nan::SetPrototypeMethod(tpl, "start", start);
  Nan::SetPrototypeMethod(tpl, "stop", stop);
  Nan::SetPrototypeMethod(tpl, "save", save);
  Nan::SetPrototypeMethod(tpl, "getImage", getImage);
  Nan::SetPrototypeMethod(tpl, "capture", capture);
  Nan::SetPrototypeMethod(tpl, "stopCapture", stopCapture);
  Nan::SetPrototypeMethod(tpl, "isCapturing", isCapturing);
  Nan::SetPrototypeMethod(tpl, "idle", idle);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("camera").ToLocalChecked(), tpl->GetFunction());
}

void vwCam::onQAppStart(){
  if(QCoreApplication::instance() == NULL){
    int argc = 1;
    char * argv[] = {"vwCam",NULL};
    //app = new QCoreApplication(argc,argv);
    //app->exec();
    //while(1) app->processEvents();
  }
}

void vwCam::setDefaults(){
  objectInfo = new OBJECT_INFO;
  imageBufferNumber = 12;
  bReady = false;
  bCapturing = false;
  numStored = 0;
  //buffer = new imgBuffer();

  /*if(thread == NULL){
    thread = new QThread();
    connect(thread, SIGNAL(started()),this,SLOT(onQAppStart()),Qt::DirectConnection);
    thread->start();
  }*/


  /*if(connect(buffer,SIGNAL(doneSaving(int)),this,SLOT(handleSaveFinish(int)))){
    cout << "Successfully connected" << endl;
  }

  connect(this,SIGNAL(saveSignal(string)),buffer,SLOT(save(string)));*/

  //QObject::connect(buffer,&imgBuffer::finished,buffer, &QObject::deleteLater);
  //cb = NULL;
  int argc = 1;
  char * argv[] = {"vwCam",NULL};
  //app = new QCoreApplication(argc,argv);
}

void vwCam::handleSaveFinish(int saved){
  cout << "Done saving" << endl;
  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { Nan::New((int)saved) };
  //Nan::MakeCallback(Nan::GetCurrentContext()->Global(), saveCB, argc, argv);
  saveCB->Call(argc,argv);
}

void vwCam::idle(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  //obj->app->processEvents();
  info.GetReturnValue().Set(Nan::New((int)0));
}

void vwCam::getWidth(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  info.GetReturnValue().Set(Nan::New((int)obj->width));
}

void vwCam::getHeight(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  info.GetReturnValue().Set(Nan::New((int)obj->height));
}

void vwCam::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new vwCam(...)`
    vwCam* obj = new vwCam();
    obj->Wrap(info.This());
    obj->setDefaults();
    obj->open();
    info.GetReturnValue().Set(info.This());
  } else {
    // Invoked as plain function `vwCam(...)`, turn into construct call.
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { info[0] };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void vwCam::begin(const Nan::FunctionCallbackInfo<v8::Value>& info){
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  obj->GigE	=	new VwGigE;
  if ( NULL == obj->GigE )
	{
		cout << "Error creating GigE connection" << endl;
		//abort();
	}

  RESULT result = RESULT_ERROR;

	if ( RESULT_SUCCESS != (result=OpenVwGigE(&(obj->GigE)) ))
	{
		Nan::ThrowError("Error opening the GigE connection");
	} else {
    cout << "GigE connection opened successfully" << endl;
  }


}

void storeImage( OBJECT_INFO* pObjectInfo, IMAGE_INFO* pImageInfo ){
	vwCam* cam = (vwCam*)pObjectInfo->pUserPointer;
  cam->store(pImageInfo);
}

void vwCam::store(IMAGE_INFO* pImageInfo){
  UINT unWidth		= pImageInfo->width;
	UINT unHeight		= pImageInfo->height;
	PIXEL_FORMAT ePixelFormat = pImageInfo->pixelFormat;
  if (bCapturing){
    /*if (!buffer->store((PBYTE)pImageInfo->pImage)){
      bCapturing=false;
      cout << "Captured " << buffer->storageNumber() << " frames" << endl;
      //const unsigned argc = 1;
      //v8::Local<v8::Value> argv[argc] = { Nan::New((int)1) };
      //Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
    }*/
  } else{
    QMutexLocker locker( &(liveImageMutex) );
    memcpy(liveBuffer,(PBYTE)pImageInfo->pImage,unWidth*unHeight);
  }
  //numStored++;
}

void vwCam::getImage(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());

  QMutexLocker locker( &(obj->liveImageMutex) );
  ConvertPixelFormat( PIXEL_FORMAT_BAYGR8, obj->liveConv, obj->liveBuffer,  obj->width,obj->height );

  Nan::MaybeLocal<Object> ret = Nan::CopyBuffer((char*)obj->liveConv,(size_t)(obj->bufferSize));
  info.GetReturnValue().Set(ret.ToLocalChecked());
}

void vwCam::allocate(){
  pixelFormat = PIXEL_FORMAT_MONO8;

	CameraGetWidth(camera, &width );
	//camera->GetWidth( &width );
	CameraGetHeight(camera, &height );
	//camera->GetHeight( &height );
	CameraGetPixelFormat(camera, &pixelFormat );
	// camera->GetPixelFormat( &pixelFormat );

	switch( pixelFormat )
	{
		case PIXEL_FORMAT_MONO10_PACKED:
		case PIXEL_FORMAT_MONO12_PACKED:
		case PIXEL_FORMAT_BAYGR10_PACKED:
			formatMultiplier=6;
			break;
		case PIXEL_FORMAT_BAYGR8:
		case PIXEL_FORMAT_BAYRG8:
		case PIXEL_FORMAT_MONO16:
			formatMultiplier=3;
			break;
		default:
			formatMultiplier=3;
			break;
	}

	bufferSize = width*height*formatMultiplier;

  liveBuffer = new BYTE[bufferSize];
  liveConv = new BYTE[bufferSize];
  convertBuffer = new BYTE[bufferSize];
}

void vwCam::open(){
  //vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());

  RESULT result = RESULT_ERROR;

  ////////////////////////////////////////////////////
  // open GigE connection

  GigE	=	new VwGigE();
  if ( NULL == GigE )
	{
		cout << "Error creating GigE connection" << endl;
		//abort();
	}

	if ( RESULT_SUCCESS != (result=OpenVwGigE(&(GigE))) )
	{
		Nan::ThrowError("Error opening the GigE connection");
	} else {
    cout << "GigE connection opened successfully" << endl;
  }

  ////////////////////////////////////////////////////////
  // Open camera connection

  cout << "Attempting to open camera..."<< endl;
  objectInfo->pUserPointer = this;
  RESULT result2 =VwOpenCameraByIndex(GigE,0,&camera,imageBufferNumber,0,0,0,objectInfo,storeImage,NULL);
  //RESULT result2 = GigE->OpenCamera((UINT)0, &(camera), (imageBufferNumber), 0, 0, 0, (objectInfo), storeImage, NULL);//(*processFunction)

	if(result2 != RESULT_SUCCESS){
		cout << "Camera failed to open with code " << result << endl;
	} else {
    cout << "Camera opened successfully" << endl;
  }

  ////////////////////////////////////////////////
  // Success; set defaults on camera

	objectInfo->pVwCamera = camera;

	//camera->SetTriggerMode(false);
	CameraSetTriggerMode(camera,false);
	CameraSetGain(camera,GAIN_ANALOG_ALL,6);

	//camera->SetExposureTime(1000000/125);
	CameraSetCustomCommand(camera,"ExposureTime","5000");
	//camera->SetExposureMode(EXPOSURE_MODE_TIMED);
	CameraSetExposureMode(camera,EXPOSURE_MODE_TIMED);

	//camera->SetAcquisitionTimeOut(100);
	//CameraSetAcquisitionTimeOut(camera,100);

	int nCnt =3;

	//camera->GetHeartBeatTimeoutTryCount(nCnt);

	//getImageSize();
	allocate();

	bReady=true;
}



void vwCam::output(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->outputString).ToLocalChecked());
}

void vwCam::capture(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  RESULT result = RESULT_ERROR;
  if(obj->bReady){
    //obj->cb = v8::Local<v8::Function> cb = info[0].As<v8::Function>();//v8::Local<v8::Function>::New(info[0]);
    cout << "trying to capture" << endl;
    //obj->buffer->resetStore();
    obj->bCapturing = true;
    //info.GetReturnValue().Set(Nan::New((int)1));
    cout << "Began capture" << endl;
  } else {
    info.GetReturnValue().Set(Nan::New((int)RESULT_ERROR));
    cout << "Must open camera first" << endl;
  }
}

void vwCam::stopCapture(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  if(obj->bCapturing){
    obj->bCapturing = false;

    /*if(obj->cb.IsCallable()){
      const unsigned argc = 1;
      v8::Local<v8::Value> argv[argc] = { Nan::New((int)obj->buffer.storageNumber()) };
      Nan::MakeCallback(Nan::GetCurrentContext()->Global(), obj->cb, argc, argv);
    }*/
    //info.GetReturnValue().Set(Nan::New((int)obj->buffer->storageNumber()));
  } else {
    info.GetReturnValue().Set(Nan::New((int)0));
    cout << "Camera not capturing." << endl;
  }
}

void vwCam::setFrameRate(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  double rate = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();
  RESULT result = RESULT_ERROR;
  if(obj->bReady){
    cout << "Setting frame rate" << endl;
    //CameraSetExposureMode(obj->camera,EXPOSURE_MODE_TIMED);
    /*std:ostringstream stm;
    stm << (1000000/rate);
    char* s = (char*)stm.str().c_str();*/
    //obj->camera->SetExposureMode(EXPOSURE_MODE_TIMED);
    result = CameraSetCustomCommand(obj->camera,"ExposureTime",to_charStar<int>((int)1000000/rate));
    //result = CameraSetExposureTime(obj->camera,1000000/rate);
    //result = obj->camera->SetExposureTime(1000000/rate);
    info.GetReturnValue().Set(Nan::New((int)result));
    if(!result) cout << "Frame rate set to " << rate << endl;
  } else {
    info.GetReturnValue().Set(Nan::New((int)RESULT_ERROR));
    cout << "Must open camera first" << endl;
  }

}

void vwCam::isCapturing(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  info.GetReturnValue().Set(Nan::New((int)obj->bCapturing));
}

void vwCam::setImageGain(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  double gain = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();
  RESULT result = RESULT_ERROR;
  if(obj->bReady){
    CameraSetGain(obj->camera,GAIN_ANALOG_ALL,gain);
    if(!result) cout << "AnalogGainAll set to " << gain << endl;
    info.GetReturnValue().Set(Nan::New((int)result));
  } else {
    info.GetReturnValue().Set(Nan::New((int)RESULT_ERROR));
    cout << "Must open camera first" << endl;
  }
}

void vwCam::start(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  RESULT result = RESULT_ERROR;
  v8::Local<v8::Function> callb = info[0].As<v8::Function>();
  if(obj->bReady){
    result = CameraGrab(obj->camera);
    //result = obj->camera->Grab();
    info.GetReturnValue().Set(Nan::New((int)result));
	  if(!result) {
      cout << "Started camera. "<<endl;
      const unsigned argc = 1;
      v8::Local<v8::Value> argv[argc] = { Nan::New((int)1) };
      Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callb, argc, argv);
    } else cout << "Error while starting: "<< result << endl;
  } else {
    info.GetReturnValue().Set(Nan::New((int)RESULT_ERROR));
    cout << "Must open camera first" << endl;
  }
}

void vwCam::allocateBuffer(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  double numFrames = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();
  RESULT result = RESULT_ERROR;
  if(obj->bReady){
    //obj->buffer->allocate(numFrames,obj->bufferSize);
    //info.GetReturnValue().Set(Nan::New(obj->buffer->maxFrames()));
	  //if(obj->buffer->maxFrames() == numFrames) cout << "Allocated " << numFrames << " frames. "<<endl;
	  //else cout << "Error while allocating: "<< result << endl;
  } else {
    info.GetReturnValue().Set(Nan::New((int)RESULT_ERROR));
    cout << "Must open camera first" << endl;
  }
}

void vwCam::stop(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  RESULT result = RESULT_ERROR;
  if(obj->bReady){
    result = CameraAbort(obj->camera);
    info.GetReturnValue().Set(Nan::New((int)result));
	  if(!result) cout << "Grabbed " << obj->numStored << " Frames. "<<endl;
	  else cout << "Error while starting: "<< result << endl;
  } else {
    info.GetReturnValue().Set(Nan::New((int)RESULT_ERROR));
    cout << "Must open camera first" << endl;
  }
}

using namespace std;
using namespace v8;

void vwCam::save(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  String::Utf8Value cmd(info[0]);
  string dir = (string(*cmd).length() ? string(*cmd) : "temp");
  //dir = string(*cmd);
  cout << dir << " equals " << string(*cmd) << endl;

  //v8::Local<v8::Function> callb = info[1].As<v8::Function>();
  obj->saveCB = new Nan::Callback(info[1].As<v8::Function>());

  RESULT result = RESULT_ERROR;
  if(obj->buffer->storageNumber()){
    int saved = 0;
    cout << "Saving...";
    emit obj->saveSignal(dir);
    //obj->buffer->save(dir);
    /*for (int i = 0; i < obj->buffer.storageNumber(); i++){
      int bpp=24;
      //PBYTE buff = new BYTE[obj->bufferSize];;
      ConvertPixelFormat( PIXEL_FORMAT_BAYGR8, obj->convertBuffer, obj->buffer[i],  obj->width,obj->height );
			FIBITMAP * bmp	= FreeImage_ConvertFromRawBits(obj->convertBuffer, obj->width,obj->height, obj->width*bpp/8, bpp, 0,0,0, true);
      FIBITMAP * rBmp = FreeImage_Rotate(bmp,270);
      char name[256];
			sprintf(name,"%s/%03i.jpg",dir.c_str(),i);
			FREE_IMAGE_FORMAT fif = FIF_JPEG;
			FreeImage_Save(fif, rBmp, name, 0);
      if(i==obj->buffer.storageNumber()/2){
        FIBITMAP * thumb = FreeImage_Rescale(rBmp,obj->height/4,obj->width/4);
        sprintf(name,"%s/thumb.jpg",dir.c_str());
        FreeImage_Save(fif, thumb, name, 0);
        if (thumb != NULL) FreeImage_Unload(thumb);
      }
      saved++;
      if (rBmp != NULL){
				FreeImage_Unload(rBmp);
			}
			if (bmp != NULL){
				FreeImage_Unload(bmp);
			}
      //if(buff) delete buff;
    }*/
    /*cout << "Done" << endl;
	  if(saved ==  obj->buffer->storageNumber()){
       cout << "Saved " << obj->buffer->storageNumber() << " frames. "<<endl;

    }
	  else cout << "Error while saving: "<< result << endl;*/
  } else {
    info.GetReturnValue().Set(Nan::New((int)RESULT_ERROR));
    cout << "Record first" << endl;
  }
}
