#include "vieworks.h"

Nan::Persistent<v8::Function> vwCam::constructor;

template <class T>
char* to_charStar(T var){
  ostringstream ss;
  ss << var;
  return (char*)(ss.str().c_str());
}

vwCam::vwCam() {
  camera = NULL;
  GigE = NULL;
  liveBuffer = NULL;
  liveConv = NULL;
  convertBuffer = NULL;
  objectInfo = NULL;
}

vwCam::~vwCam() {
  if(camera){
    //CameraClose(camera);
    //delete camera;
  }
  if(GigE){
    //CloseVwGigE(GigE);
    //delete GigE;
  }
  if(liveBuffer){
    //delete liveBuffer;
  }
  if(liveConv){
    //delete liveConv;
  }

  if(convertBuffer){
    //delete convertBuffer;
  }
  uv_mutex_destroy(&liveImageMutex);

  //thread->quit();
  //buffer->quit();
}

void vwCam::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("camera").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  //Prototype
  Nan::SetPrototypeMethod(tpl, "output", output);
  Nan::SetPrototypeMethod(tpl, "allocateBuffer", allocateBuffer);
  Nan::SetPrototypeMethod(tpl, "setImageGain", setImageGain);
  Nan::SetPrototypeMethod(tpl, "setFrameRate", setFrameRate);
  Nan::SetPrototypeMethod(tpl, "getWidth", getWidth);
  Nan::SetPrototypeMethod(tpl, "getHeight", getHeight);
  Nan::SetPrototypeMethod(tpl, "startCapture", start);
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

void vwCam::setDefaults(){
  objectInfo = new OBJECT_INFO;
  imageBufferNumber = 12;
  bReady = false;
  bCapturing = false;
  numStored = 0;
  buffer = new imgBuffer();
  uv_mutex_init(&liveImageMutex);
  uv_async_init(uv_default_loop(),&liveImageAsync,vwCam::LiveImageCB);
  bLiveCap = false;
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

    obj->openCB = new Nan::Callback(info[0].As<v8::Function>());
    obj->GigE	=	new VwGigE();
    obj->pCam = new HCAMERA;
    uv_work_t* req = new uv_work_t();
    req->data = (void*)obj;


    //obj->open();
    uv_queue_work(uv_default_loop(),req,vwCam::EIO_Open,(uv_after_work_cb)vwCam::EIO_AfterOpen);
    info.GetReturnValue().Set(info.This());
  } else {
    // Invoked as plain function `vwCam(...)`, turn into construct call.
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { info[0] };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void vwCam::EIO_Open(uv_work_t* req) {
  vwCam* obj = (vwCam*)(req->data);
  obj->open();
}

void vwCam::EIO_AfterOpen(uv_work_t* req, int status) {
  vwCam* obj = (vwCam*)(req->data);
  cout << "After open" << endl;
  obj->camera = *(obj->pCam);
  obj->allocate();
  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { Nan::New((int)1) };
  obj->openCB->Call(argc,argv);
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
  RESULT result2 =VwOpenCameraByIndex(GigE,0,pCam,12,0,0,0,objectInfo,vwCam::storeImage,NULL);

	if(result2 != RESULT_SUCCESS){
		cout << "Camera failed to open with code " << result << endl;
	} else {
    cout << "Camera opened successfully" << endl;
  }

  ////////////////////////////////////////////////
  // Success; set defaults on camera

	objectInfo->pVwCamera = *pCam;//camera;

	CameraSetTriggerMode(*pCam,false);
	CameraSetGain(*pCam,GAIN_ANALOG_ALL,6);

	CameraSetCustomCommand(*pCam,"ExposureTime","5000");
	CameraSetExposureMode(*pCam,EXPOSURE_MODE_TIMED);

	int nCnt =3;

  cout << "done opening" << endl;

	bReady=true;
}



void vwCam::output(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->outputString).ToLocalChecked());
}

livePack imgPack;

void vwCam::storeImage( OBJECT_INFO* pObjectInfo, IMAGE_INFO* pImageInfo ){
	vwCam* cam = (vwCam*)pObjectInfo->pUserPointer;
  if(cam->bCapturing) cam->store(pImageInfo);
  else if(cam->bLiveCap){
    imgPack.objInfo = pObjectInfo;
    imgPack.imgInfo = pImageInfo;
    cam->liveImageAsync.data = (void*)(&imgPack);
    uv_async_send(&(cam->liveImageAsync));
  }
}

void vwCam::LiveImageCB(uv_async_t* req){
  OBJECT_INFO* objInfo = ((livePack*)req->data)->objInfo;
  IMAGE_INFO* imgInfo = ((livePack*)req->data)->imgInfo;
  vwCam* cam = (vwCam*)(objInfo->pUserPointer);
  PBYTE img = (PBYTE)(imgInfo->pImage);
  UINT unWidth		= imgInfo->width;
	UINT unHeight		= imgInfo->height;
	PIXEL_FORMAT ePixelFormat = imgInfo->pixelFormat;
  const unsigned argc = 1;
  ConvertPixelFormat( ePixelFormat, cam->liveConv, img,  unWidth,unHeight ); //
  Nan::MaybeLocal<Object> ret = Nan::CopyBuffer((char*)cam->liveConv,(size_t)(cam->bufferSize));
  v8::Local<v8::Value> argv[argc] = {ret.ToLocalChecked() };
  cam->liveCapCB.Call(argc,argv);
  cam->bLiveCap = false;
}

//Function called to actually store the image to the image buffers. Will stop
//capturing if the image buffer fills up.

//TODO: for some reason, if the else statement is left active, it screws up
// image acquisition over time; investigate.
void vwCam::store(IMAGE_INFO* pImageInfo){
  UINT unWidth		= pImageInfo->width;
	UINT unHeight		= pImageInfo->height;
	PIXEL_FORMAT ePixelFormat = pImageInfo->pixelFormat;
  if (bCapturing){
    if (!buffer->store((PBYTE)pImageInfo->pImage)){
      bCapturing=false;
      cout << "Captured " << buffer->storageNumber() << " frames" << endl;
      //const unsigned argc = 1;
      //v8::Local<v8::Value> argv[argc] = { Nan::New((int)1) };
      //Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
    }
  } else{
    //memcpy(liveBuffer,(PBYTE)pImageInfo->pImage,unWidth*unHeight);
    //imgPack
    /*if(bLiveCap){
      const unsigned argc = 1;
      ConvertPixelFormat( ePixelFormat, liveConv, (PBYTE)pImageInfo->pImage,  unWidth,unHeight ); //
      Nan::MaybeLocal<Object> ret = Nan::CopyBuffer((char*)liveConv,(size_t)(bufferSize));
      v8::Local<v8::Value> argv[argc] = {ret.ToLocalChecked() };
      liveCapCB.Call(argc,argv);
      bLiveCap = false;
      //liveCapCB.Reset();
    }*/
  }
  //numStored++;
}

//This would pass the current, live image from the camera as a javascript array,
// if it could actually reliably pass the data.
void vwCam::getImage(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());

  if(!obj->bLiveCap){
    obj->liveCapCB.SetFunction(info[0].As<v8::Function>());
    obj->bLiveCap = true;
  }

  /*ConvertPixelFormat( PIXEL_FORMAT_BAYGR8, obj->liveConv, obj->liveBuffer,  obj->width,obj->height );

  Nan::MaybeLocal<Object> ret = Nan::CopyBuffer((char*)obj->liveConv,(size_t)(obj->bufferSize));
  info.GetReturnValue().Set(ret.ToLocalChecked());*/
}

void vwCam::capture(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  RESULT result = RESULT_ERROR;
  if(obj->bReady){
    //obj->endCapCB = new Nan::Callback(info[0].As<v8::Function>());
    cout << "trying to capture" << endl;
    obj->buffer->resetStore();
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

    /*if(obj->endCapCB.IsCallable()){
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

void vwCam::isCapturing(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  info.GetReturnValue().Set(Nan::New((int)obj->bCapturing));
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
    obj->buffer->allocate(numFrames,obj->bufferSize);
    info.GetReturnValue().Set(Nan::New(obj->buffer->maxFrames()));
	  if(obj->buffer->maxFrames() == numFrames) cout << "Allocated " << numFrames << " frames. "<<endl;
	  else cout << "Error while allocating: "<< result << endl;
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
    cout << "Saving..." << endl;
    obj->buffer->save(dir,obj->saveCB);
  } else {
    info.GetReturnValue().Set(Nan::New((int)RESULT_ERROR));
    cout << "Record first" << endl;
  }
}
