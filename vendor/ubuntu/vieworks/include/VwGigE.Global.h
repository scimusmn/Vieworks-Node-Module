
#pragma once

#ifndef VWGIGE_GLOBAL_H
#define VWGIGE_GLOBAL_H
#include "VwGigE.Error.h"
#include "sys/time.h"
#include "ctype.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <vector>

using namespace std;

#if defined(VWGIGE_LIBRARY)
#  define CLASS_EXPORT     __attribute__((visibility("default")))
#  define CLASS_LOCAL     __attribute__((visibility("hidden")))
#  define FUNCTION_EXPORT     __attribute__((visibility("default")))
#  define FUNCTION_LOCAL     __attribute__((visibility("hidden")))
#else if
#  define CLASS_EXPORT
#  define CLASS_LOCAL
#  define FUNCTION_EXPORT
#  define FUNCTION_LOCAL
#endif


typedef int                 BOOL;
typedef unsigned char		BYTE;
typedef unsigned short int	WORD;
typedef unsigned int		DWORD;
typedef unsigned int		UINT;
typedef UINT                uint32_t;
typedef BYTE*               PBYTE;
typedef char                CHAR;
typedef void*               HANDLE;
typedef long long           __int64;
typedef unsigned long long  ULONGLONG;
typedef ULONGLONG           __uint64;
typedef void*               LPVOID;
typedef void*               PVOID;


#if defined(UNICODE)
    typedef short      TCHAR;
#else if
    typedef char       TCHAR;
#endif

#define ZeroMemory(Destination, Length) memset((Destination), 0, (Length))

#define INFINITE 0xFFFFFFFF
#define FALSE   0
#define TRUE    1
#define NULL    0

#define IN
#define OUT
#define INOUT


#define _GENERIC_CHECK(b,desc)      \
    {								\
        if(!(b))					\
            throw string(desc);		\
    }


#define _TL_CHECK(f,desc)									\
    {														\
        GC_ERROR gce = (f);									\
        if(gce!=GC_ERR_SUCCESS)								\
        {                                                   \
            string str;                                     \
            char ch[1024] = {0,};                           \
            sprintf(ch, "%s: %d ", (desc), (gce) );         \
            str = ch;                                       \
            throw str;                                      \
        }													\
    }

#define _TL_PROLOG try{

#define _TL_EPILOG                              \
    }catch(string desc){						\
        printf(desc.c_str());                   \
    }catch(GenericException gex){				\
        printf("%s", gex.GetDescription() );    \
    }




#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	if(p){ delete (p); (p) = NULL;}
#endif //SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	if(p){ delete[] (p); (p) = NULL;}
#endif //SAFE_DELETE_ARRAY

//#	if defined(_DEBUG)
//#		define DEBUG_LOG(log)\
//            {_DebugOutput("%s \t\t\t [%s] (%s:%d), error %d (%s)\n", log, __FUNCTION__, __FILE__, __LINE__, errno,\
//            strerror(errno));}
//#	else
//#		define DEBUG_LOG(log)
//#	endif

//inline void _DebugOutput(const char* fmt, ...)
//{

//    va_list val;
//    va_start(val, fmt);
//    char sz[2048];
//#	if defined(_WIN32)
//    _vsnprintf(sz, sizeof(sz), fmt, val);
//    OutputDebugString(sz);
//#	else
//    vprintf(fmt, val);
//#	endif
//    va_end(val);
//}



enum PIXEL_FORMAT
{
	PIXEL_FORMAT_MONO8					= 0x01080001,
	PIXEL_FORMAT_MONO8_SIGNED			= 0x01080002,
	PIXEL_FORMAT_MONO10					= 0x01100003 ,
	PIXEL_FORMAT_MONO10_PACKED			= 0x010C0004,
	PIXEL_FORMAT_MONO12					= 0x01100005,
	PIXEL_FORMAT_MONO12_PACKED			= 0x010C0006,
	PIXEL_FORMAT_MONO16					= 0x01100007,
	PIXEL_FORMAT_BAYGR8					= 0x01080008,
	PIXEL_FORMAT_BAYRG8					= 0x01080009,
	PIXEL_FORMAT_BAYGB8					= 0x0108000A,
	PIXEL_FORMAT_BAYBG8					= 0x0108000B,
	PIXEL_FORMAT_BAYGR10 				= 0x0110000C,
	PIXEL_FORMAT_BAYRG10 				= 0x0110000D,
	PIXEL_FORMAT_BAYGB10 				= 0x0110000E,
	PIXEL_FORMAT_BAYBG10 				= 0x0110000F,
	PIXEL_FORMAT_BAYGR10_PACKED			= 0x010C0026,
	PIXEL_FORMAT_BAYGR12_PACKED			= 0x010C002A,
	PIXEL_FORMAT_BAYGR12 				= 0x01100010,
	PIXEL_FORMAT_BAYRG12 				= 0x01100011,
	PIXEL_FORMAT_BAYGB12 				= 0x01100012,
	PIXEL_FORMAT_BAYBG12 				= 0x01100013,
	PIXEL_FORMAT_BAYRG10_PACKED			= 0x010C0027,
	PIXEL_FORMAT_BAYRG12_PACKED			= 0x010C002B,

	PIXEL_FORMAT_RGB8_PACKED			= 0x02180014,
	PIXEL_FORMAT_BGR8_PACKED			= 0x02180015,
	PIXEL_FORMAT_YUV422_UYVY			= 0x0210001F,
	PIXEL_FORMAT_YUV422_YUYV			= 0x02100032,
	PIXEL_FORMAT_YUV422_10_PACKED		= 0x80180001,
	PIXEL_FORMAT_YUV422_12_PACKED		= 0x80180002,
	PIXEL_FORMAT_YUV411					= 0x020C001E,
	PIXEL_FORMAT_YUV411_10_PACKED		= 0x80120004,
	PIXEL_FORMAT_YUV411_12_PACKED		= 0x80120005,
	PIXEL_FORMAT_BGR10V1_PACKED			= 0x0220001C,
	PIXEL_FORMAT_BGR10V2_PACKED			= 0x0220001D,
	PIXEL_FORMAT_RGB12_PACKED			= 0x0230001A,
	PIXEL_FORMAT_BGR12_PACKED			= 0x0230001B,
	PIXEL_FORMAT_YUV444					= 0x02180020,
	PIXEL_FORMAT_PAL_INTERLACED			= 0x02100001,
	PIXEL_FORMAT_NTSC_INTERLACED		= 0x02100002
};

enum TESTIMAGE
{
	TESTIMAGE_OFF, 
	TESTIMAGE_BLACK, 
	TESTIMAGE_WHITE, 
	TESTIMAGE_GREYHORIZONTALRAMP, 
	TESTIMAGE_GREYVERTICALRAMP, 
	TESTIMAGE_GREYHORIZONTALRAMPMOVING, 
	TESTIMAGE_GREYVERTICALRAMPMOVING,
	TESTIMAGE_GREYCROSSRAMP,
	TESTIMAGE_GREYCROSSRAMPMOVING
};

enum STROBE_POLARITY
{
	STROBE_POLARITY_ACTIVEHIGH, 
	STROBE_POLARITY_ACTIVELOW
};


enum BLACKLEVEL_SEL
{
	BLACKLEVEL_SEL_TAP1, 
	BLACKLEVEL_SEL_TAP2, 
	BLACKLEVEL_SEL_TAP3, 
	BLACKLEVEL_SEL_TAP4
};

enum EXPOSURE_MODE
{
	EXPOSURE_MODE_TIMED, 
	EXPOSURE_MODE_TRIGGERWIDTH
};

enum READOUT
{
	READOUT_NORMAL,
	READOUT_AOI, 
	READOUT_BINNING, 
	READOUT_HORIZONTALSTART, 
	READOUT_HORIZONTALEND, 
	READOUT_VERTICALSTART, 
	READOUT_VERTICALEND, 
	READOUT_BINNINGFATOR
};

enum GAIN_SEL
{
	GAIN_ANALOG_ALL, 
	GAIN_ANALOG_TAP1, 
	GAIN_ANALOG_TAP2, 
	GAIN_ANALOG_TAP3, 
	GAIN_ANALOG_TAP4
};

enum TRIGGER_SOURCE
{
	TRIGGER_SOURCE_SW, 
	TRIGGER_SOURCE_EXT
};

enum TRIGGER_ACTIVATION
{
	TRIGGER_ACTIVATION_RISINGEDGE,
	TRIGGER_ACTIVATION_FALLINGEDGE
};

enum GAIN_COLOR
{
	GAIN_COLOR_RED,
	GAIN_COLOR_GREEN,
	GAIN_COLOR_BLUE
};

enum STREAM_INFO
{
	STREAM_INFO_NUM_OF_FRAMES_LOST,
	STREAM_INFO_NUM_PACKETS_MISSING
};

enum FILTER_TYPE
{
    FILTER_TYPE_DIRECT = 1,
    FILTER_TYPE_WPCAB
};

enum MTU_SETTING
{
    MTU_SETTING_OPTIMIZE	= 0x00000003,
    MTU_SETTING_TIMEOUT		= 0x00000004
};

#if 1
enum GET_CUSTOM_COMMAND
{
	GET_CUSTOM_COMMAND_VALUE	= 0xF0,	// Value
	GET_CUSTOM_COMMAND_NUM,				// Entry Num
	GET_CUSTOM_COMMAND_MIN,				// Minimum
	GET_CUSTOM_COMMAND_MAX,				// Maximum
	GET_CUSTOM_COMMAND_INC,				// Increment
	GET_CUSTOM_COMMAND_INDEX
};
#endif


#ifdef __cplusplus
extern "C"
{
#endif

	struct OBJECT_INFO
	{
		void*		pUserPointer;
		void*		pVwCamera;
		OBJECT_INFO()
		{
			pUserPointer	= NULL;
			pVwCamera		= NULL;
		}
	};

	struct IMAGE_INFO
	{
		RESULT			callbackResult;
		unsigned int	bufferIndex;
		PIXEL_FORMAT	pixelFormat;
		unsigned int	width;
		unsigned int	height;
		void*			pImage;
	};

	struct CAMERA_INFO_STRUCT
	{
		BOOL				error;
		RESULT				errorResult;
		unsigned int		index;
		char				name[256];
		char				vendor[128];
		char				model[128];
		char				ip[32];
		char				mac[32];

	};

	struct INTERFACE_INFO_STRUCT
	{
		BOOL				error;
		RESULT				errorCause;
		unsigned int		index;
		char				name[256];
	};

	struct PIXEL_FORMAT_INFO
	{
		char				szName[256];
		PIXEL_FORMAT		pixelFormat;
		unsigned int		nSize;
	};

	struct DISCONNECT_INFO
	{
		int					nCurrHeartBeatTimeOut;	//	Current heartbeat timeout millisecond. 
		int					nTimeOutTryCount;		//	Heartbeat timeout try count.
	};

    typedef enum
    {
        UNKNOWN			= 0,
        ATTR_UINT		= 1,
        ATTR_FLOAT		= 2,
        ATTR_ENUM		= 3,
        ATTR_BOOLEAN	= 4,
        ATTR_STRING		= 5,
        ATTR_COMMAND	= 6,
        ATTR_CATEGORY	= 7
    } PROPERTY_TYPE;

    typedef enum
    {
        NOT_IMPLEMENT	= 0,					// Not implemented
        NOT_AVAILABLE	= 1,					// Not available
        WRITE_ONLY		= 2,					// Write Only
        READ_ONLY		= 3,					// Read Only
        READ_WRITE		= 4						// Read and Write
    } PROPERTY_ACCESS_MODE;

    typedef enum
    {
        BEGINNER = 0,       // Always visible
        EXPERT = 1,			// Visible for experts or Gurus
        GURU = 2,			// Visible for Gurus
        INVISIBLE = 3,		// Not Visible
        UNDEFINE = 4
    } PROPERTY_VISIBILITY;

    typedef struct
    {
    public:
        PROPERTY_TYPE			ePropType;
        PROPERTY_ACCESS_MODE	eAccessMode;
        PROPERTY_VISIBILITY		eVisibility;
        UINT					unPollingTime;

        char 					caName[256];
        char 					caDisplay[256];
        char					caDescription[512];

    } PROPERTY;

    typedef std::vector<PROPERTY> PROPERTY_LIST;

#ifdef __cplusplus
}
#endif

typedef void (ImageCallbackFn)(OBJECT_INFO* pObjectInfo, IMAGE_INFO* pImageInfo);

typedef void (DisconnectCallbackFn)( OBJECT_INFO* pObjectInfo, DISCONNECT_INFO tDisconnectInfo );


inline char* strlower(char *s)
{
    char* p = s;

    while(*p = tolower(*p))
        p++;

    return s;
}

inline unsigned int GetTickCount()
{
    struct timeval gettick;
    unsigned int tick;
    int ret;

    gettimeofday(&gettick, NULL);
    tick = gettick.tv_sec * 1000 + gettick.tv_usec / 1000;

    return tick;
}


#endif
