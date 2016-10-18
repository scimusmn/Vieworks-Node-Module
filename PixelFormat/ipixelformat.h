#ifndef IPIXELFORMAT_H
#define IPIXELFORMAT_H

//#ifdef _LINUX
typedef unsigned char           BYTE;
typedef unsigned short int      WORD;
typedef unsigned int			DWORD;
typedef unsigned int			UINT;
//#endif

#include <stdint.h>
#include <cstddef>

namespace PixelFormat
{

    class IPixelFormat
    {
    public:
        IPixelFormat(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
            :BGR_PIXEL(3),
              m_unWidth(unWidth),
              m_unHeight(unHeight),
              m_unBpp(unBpp),
              m_pCovertedImage(NULL),
              m_pTmpBuffer(NULL)
        {};

        virtual ~IPixelFormat(){
            //DEBUG_LOG("IPixelFormat::~IPixelFormat()")
        };

        virtual uint8_t* ToBGR8(uint8_t* pRawImage)=0;
        virtual uint8_t* ToRGB8(uint8_t* pRawImage)=0;
        virtual bool SaveImage()=0;

        uint32_t    m_unBpp;
        uint32_t    m_unWidth;
        uint32_t    m_unHeight;

        uint8_t*    m_pCovertedImage;
        uint8_t*    m_pTmpBuffer;

        const int   BGR_PIXEL;
    };


}//namespace PixelFormat

#endif // IPIXELFORMAT_H
