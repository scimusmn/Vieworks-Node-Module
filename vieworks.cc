#include "vieworks.h"

Nan::Persistent<v8::Function> vwCam::constructor;

vwCam::vwCam(double value) : value_(value) {
}

vwCam::~vwCam() {
  if(camera){
    camera->CloseCamera();
    delete camera;
  }
  if(GigE){
    GigE->Close();
    delete GigE;
  }
  if(liveBuffer){
    delete liveBuffer;
  }
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
  Nan::SetPrototypeMethod(tpl, "start", start);
  Nan::SetPrototypeMethod(tpl, "stop", stop);
  Nan::SetPrototypeMethod(tpl, "save", save);
  Nan::SetPrototypeMethod(tpl, "capture", capture);
  Nan::SetPrototypeMethod(tpl, "stopCapture", stopCapture);
  Nan::SetPrototypeMethod(tpl, "isCapturing", isCapturing);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("camera").ToLocalChecked(), tpl->GetFunction());
}

void vwCam::setDefaults(){
  objectInfo = new OBJECT_INFO;
  imageBufferNumber = 1;
  bReady = false;
  bCapturing = false;
  numStored = 0;
  //cb = NULL;
}

void vwCam::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new vwCam(...)`
    double value = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
    vwCam* obj = new vwCam(value);
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

	if ( RESULT_SUCCESS != (result=obj->GigE->Open()) )
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
    if (!buffer.store((PBYTE)pImageInfo->pImage)){
      bCapturing=false;
      cout << "Captured " << buffer.storageNumber() << " frames" << endl;
      //const unsigned argc = 1;
      //v8::Local<v8::Value> argv[argc] = { Nan::New((int)1) };
      //Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
    }
  } else memcpy(liveBuffer,(PBYTE)pImageInfo->pImage,unWidth*unHeight);
  numStored++;
}

void vwCam::allocate(){
  pixelFormat = PIXEL_FORMAT_MONO8;

	camera->GetWidth( &width );
	camera->GetHeight( &height );
	camera->GetPixelFormat( &pixelFormat );

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
}

void vwCam::open(){
  //vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());

  RESULT result = RESULT_ERROR;

  ////////////////////////////////////////////////////
  // open GigE connection

  GigE	=	new VwGigE;
  if ( NULL == GigE )
	{
		cout << "Error creating GigE connection" << endl;
		//abort();
	}

	if ( RESULT_SUCCESS != (result=GigE->Open()) )
	{
		Nan::ThrowError("Error opening the GigE connection");
	} else {
    cout << "GigE connection opened successfully" << endl;
  }

  ////////////////////////////////////////////////////////
  // Open camera connection

  cout << "Attempting to open camera..."<< endl;
  objectInfo->pUserPointer = this;
  RESULT result2 = GigE->OpenCamera((UINT)0, &(camera), (imageBufferNumber), 0, 0, 0, (objectInfo), storeImage, NULL);//(*processFunction)

	if(result2 != RESULT_SUCCESS){
		cout << "Camera failed to open with code " << result << endl;
	} else {
    cout << "Camera opened successfully" << endl;
  }

  ////////////////////////////////////////////////
  // Success; set defaults on camera

	objectInfo->pVwCamera = camera;

	camera->SetTriggerMode(false);
	camera->SetGain(GAIN_ANALOG_ALL,6);

	camera->SetExposureTime(1000000/125);
	camera->SetExposureMode(EXPOSURE_MODE_TIMED);

	camera->SetAcquisitionTimeOut(100);

	int nCnt =3;

	camera->GetHeartBeatTimeoutTryCount(nCnt);

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
    obj->buffer.resetStore();
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
    info.GetReturnValue().Set(Nan::New((int)1));
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
    obj->camera->SetExposureMode(EXPOSURE_MODE_TIMED);
    result = obj->camera->SetExposureTime(1000000/rate);
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
    obj->camera->SetGain(GAIN_ANALOG_ALL,gain);
    info.GetReturnValue().Set(Nan::New((int)result));
  } else {
    info.GetReturnValue().Set(Nan::New((int)RESULT_ERROR));
    cout << "Must open camera first" << endl;
  }
}

void vwCam::start(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  RESULT result = RESULT_ERROR;
  if(obj->bReady){
    result = obj->camera->Grab();
    info.GetReturnValue().Set(Nan::New((int)result));
	  if(!result) cout << "Started camera. "<<endl;
	  else cout << "Error while starting: "<< result << endl;
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
    obj->buffer.allocate(numFrames,obj->bufferSize);
    info.GetReturnValue().Set(Nan::New(obj->buffer.maxFrames()));
	  if(obj->buffer.maxFrames() == numFrames) cout << "Allocated " << numFrames << " frames. "<<endl;
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
    result = obj->camera->AcquisitionStop();
    info.GetReturnValue().Set(Nan::New((int)result));
	  if(!result) cout << "Grabbed " << obj->numStored << " Frames. "<<endl;
	  else cout << "Error while starting: "<< result << endl;
  } else {
    info.GetReturnValue().Set(Nan::New((int)RESULT_ERROR));
    cout << "Must open camera first" << endl;
  }
}

BOOL ConvertPixelFormat( PIXEL_FORMAT ePixelFormat, BYTE* pDest, BYTE* pSource, int nWidth, int nHeight )
{
	if ( NULL == pDest ||
		 NULL == pSource )
	{
		return FALSE;
	}

	if ( 0 == nWidth || 0 == nHeight )
	{
		return FALSE;
	}

	BOOL bRet = TRUE;

	//memcpy( pDest, pSource, nWidth * nHeight );
	BYTE* bpConvertPixelFormat =	new BYTE[ nWidth * nHeight * 2 ];

	switch( ePixelFormat )
	{
		//-----------------------------------------------------------------
		// about MONO Pixel Format Series ---------------------------------
		//-----------------------------------------------------------------
	case PIXEL_FORMAT_MONO8:
		memcpy( pDest, pSource, nWidth * nHeight );
		break;
	case PIXEL_FORMAT_MONO10:
		VwImageProcess::ConvertMono10ToBGR8(PBYTE(pSource), nWidth*nHeight*2, pDest);
		break;
	case PIXEL_FORMAT_MONO12:
		VwImageProcess::ConvertMono12ToBGR8(PBYTE(pSource), nWidth*nHeight*2, pDest);
		break;
	case PIXEL_FORMAT_MONO10_PACKED:
	case PIXEL_FORMAT_MONO12_PACKED:
		VwImageProcess::ConvertMonoPackedToBGR8( pSource,
												UINT(1.5*nWidth*nHeight),
												pDest );
		break;
	case PIXEL_FORMAT_MONO16:
		VwImageProcess::ConvertMono16PackedToBGR8( pSource,
												UINT(2*nWidth*nHeight),
												pDest );
		break;
		//-----------------------------------------------------------------
		// about BAYER Pixel Format Series --------------------------------
		//-----------------------------------------------------------------
	case PIXEL_FORMAT_BAYGR8:
		VwImageProcess::ConvertBAYGR8ToBGR8( pSource,
											pDest,
											nWidth,
											nHeight );
		break;
	case PIXEL_FORMAT_BAYRG8:
		VwImageProcess::ConvertBAYRG8ToBGR8( pSource,
											pDest,
											nWidth,
											nHeight );
		break;
	case PIXEL_FORMAT_BAYGR10:
		VwImageProcess::ConvertBAYGR10ToBGR8( (WORD*)(pSource),
											pDest,
											nWidth,
											nHeight );
		break;
	case PIXEL_FORMAT_BAYRG10:
		VwImageProcess::ConvertBAYRG10ToBGR8( (WORD*)(pSource),
											pDest,
											nWidth,
											nHeight );
		break;
	case PIXEL_FORMAT_BAYGR12:
		VwImageProcess::ConvertBAYGR12ToBGR8( (WORD*)(pSource),
											pDest,
											nWidth,
											nHeight );
		break;
	case PIXEL_FORMAT_BAYRG12:
		VwImageProcess::ConvertBAYRG12ToBGR8( (WORD*)(pSource),
											pDest,
											nWidth,
											nHeight );
		break;
	case PIXEL_FORMAT_BAYGR10_PACKED:
		VwImageProcess::ConvertMono10PackedToMono16bit( (PBYTE)pSource,
														nWidth,
														nHeight,
														bpConvertPixelFormat );
		VwImageProcess::ConvertBAYGR10ToBGR8( (WORD*)bpConvertPixelFormat,
												pDest,
												nWidth,
												nHeight );
		break;
	case PIXEL_FORMAT_BAYRG10_PACKED:
		VwImageProcess::ConvertMono10PackedToMono16bit( (PBYTE)pSource,
														nWidth,
														nHeight,
														bpConvertPixelFormat );
		VwImageProcess::ConvertBAYRG10ToBGR8( (WORD*)bpConvertPixelFormat,
												pDest,
												nWidth,
												nHeight );
		break;
	case PIXEL_FORMAT_BAYGR12_PACKED:
		VwImageProcess::ConvertMono12PackedToMono16bit( (PBYTE)pSource,
														nWidth,
														nHeight,
														bpConvertPixelFormat );
		VwImageProcess::ConvertBAYGR12ToBGR8( (WORD*)bpConvertPixelFormat,
												pDest,
												nWidth,
												nHeight );
		break;
	case PIXEL_FORMAT_BAYRG12_PACKED:
		VwImageProcess::ConvertMono12PackedToMono16bit( (PBYTE)pSource,
														nWidth,
														nHeight,
														bpConvertPixelFormat );
		VwImageProcess::ConvertBAYRG12ToBGR8( (WORD*)bpConvertPixelFormat,
													pDest,
													nWidth,
													nHeight );
		break;
	case PIXEL_FORMAT_RGB8_PACKED:
		VwImageProcess::ConvertRGB8toBGR8( (PBYTE)pSource,
											UINT(3*nWidth*nHeight),
											pDest );
		break;
	case PIXEL_FORMAT_BGR8_PACKED:
		bRet = FALSE;
		break;
	case PIXEL_FORMAT_RGB12_PACKED:
		VwImageProcess::ConvertRGB12PackedtoBGR8( (PBYTE)pSource,
												UINT(6*nWidth*nHeight),
												pDest );
		break;
	case PIXEL_FORMAT_BGR12_PACKED:
		bRet = FALSE;
		break;
	case PIXEL_FORMAT_YUV411:
		VwImageProcess::ConvertYUV411toBGR8( (PBYTE)pSource,
											UINT(1.5*nWidth*nHeight),
											pDest );
		break;
	case PIXEL_FORMAT_YUV422_UYVY:
		VwImageProcess::ConvertYUV422_UYVYtoBGR8( (PBYTE)pSource,
												nWidth,
												nHeight,
												pDest );
		break;
	case PIXEL_FORMAT_YUV422_YUYV:
		VwImageProcess::ConvertYUV422_YUYVtoBGR8( (PBYTE)pSource,
												nWidth,
												nHeight,
												pDest );
		break;
	case PIXEL_FORMAT_YUV444:
		VwImageProcess::ConvertYUV444toBGR8( (PBYTE)pSource,
											UINT(1.5*nWidth*nHeight),
											pDest );
		break;
	case PIXEL_FORMAT_BGR10V1_PACKED:
		bRet = FALSE;
		break;
	case PIXEL_FORMAT_YUV411_10_PACKED:
	case PIXEL_FORMAT_YUV411_12_PACKED:
		VwImageProcess::ConvertYUV411PackedtoBGR8( (PBYTE)pSource,
													UINT(2.25*nWidth*nHeight),
													pDest );
		break;
	case PIXEL_FORMAT_YUV422_10_PACKED:
	case PIXEL_FORMAT_YUV422_12_PACKED:
		VwImageProcess::ConvertYUV422PackedtoBGR8( (PBYTE)pSource,
													UINT(3*nWidth*nHeight),
													pDest );
		break;
	case PIXEL_FORMAT_PAL_INTERLACED:
	case PIXEL_FORMAT_NTSC_INTERLACED:
		break;
	default:
		{
			bRet = FALSE;
		}
	}

	if ( NULL != bpConvertPixelFormat )
	{
		delete [] bpConvertPixelFormat;
	}

	return bRet;
}

using namespace std;
using namespace v8;

void vwCam::save(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  vwCam* obj = ObjectWrap::Unwrap<vwCam>(info.Holder());
  String::Utf8Value cmd(info[0]);
  string dir = (string(*cmd).length() ? string(*cmd) : "temp");
  //dir = string(*cmd);
  cout << dir << " equals " << string(*cmd) << endl;

  v8::Local<v8::Function> callb = info[1].As<v8::Function>();

  RESULT result = RESULT_ERROR;
  if(obj->buffer.storageNumber()){
    int saved = 0;
    for (int i = 0; i < obj->buffer.storageNumber(); i++){
      int bpp=24;
      ConvertPixelFormat( PIXEL_FORMAT_BAYGR8, obj->liveBuffer, obj->buffer[i],  obj->width,obj->height );
			FIBITMAP * bmp	= FreeImage_ConvertFromRawBits(obj->liveBuffer, obj->width,obj->height, obj->width*bpp/8, bpp, 0,0,0, true);
      bmp = FreeImage_Rotate(bmp,270);
      char name[256];
			sprintf(name,"%s\\%03i.jpg",dir.c_str(),i);
      cout << name << endl;
			FREE_IMAGE_FORMAT fif = FIF_JPEG;
			FreeImage_Save(fif, bmp, name, 0);
      if(i==obj->buffer.storageNumber()/2){
        FIBITMAP * thumb = FreeImage_Rescale(bmp,obj->height/4,obj->width/4);
        sprintf(name,"%s\\thumb.jpg",dir.c_str());
        FreeImage_Save(fif, thumb, name, 0);
        if (thumb != NULL) FreeImage_Unload(thumb);
      }
      saved++;
			if (bmp != NULL){
				FreeImage_Unload(bmp);
			}
    }
    info.GetReturnValue().Set(Nan::New((int)1));
	  if(saved ==  obj->buffer.storageNumber()){
       cout << "Saved " << obj->buffer.storageNumber() << " frames. "<<endl;
       const unsigned argc = 1;
       v8::Local<v8::Value> argv[argc] = { Nan::New((int)saved) };
       Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callb, argc, argv);
    }
	  else cout << "Error while saving: "<< result << endl;
  } else {
    info.GetReturnValue().Set(Nan::New((int)RESULT_ERROR));
    cout << "Record first" << endl;
  }
}
