#include "imgBuffer.h"
#include <cstring> // memcpy
#include <cstdlib> //realloc



imgBuffer::imgBuffer(QObject * parent) : QThread(parent){
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

	delete convertBuffer;
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

void imgBuffer::run() {
	int saved = 0;
	cout << "saving..." << endl;
	for (int i = 0; i < nStored-1; i++){
		int bpp=24;
		//PBYTE buff = new BYTE[obj->bufferSize];;
		ConvertPixelFormat( PIXEL_FORMAT_BAYGR8, convertBuffer, buffers[i],  640,480 );
		FIBITMAP * bmp	= FreeImage_ConvertFromRawBits(convertBuffer, 640,480, 640*bpp/8, bpp, 0,0,0, true);
		FIBITMAP * rBmp = FreeImage_Rotate(bmp,270);
		char name[256];
		sprintf(name,"%s/%03i.jpg",saveDir.data(),i);
		FREE_IMAGE_FORMAT fif = FIF_JPEG;
		FreeImage_Save(fif, rBmp, name, 0);
		if(i==storageNumber()/2){
			FIBITMAP * thumb = FreeImage_Rescale(rBmp,480/4,640/4);
			sprintf(name,"%s/thumb.jpg",saveDir.data());
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
	}
	cout << "Done saving." << endl;
	emit doneSaving(saved);
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
	convertBuffer = new BYTE[nSz];

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

void imgBuffer::save(string d){
	saveDir = d;
	this->start();
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
