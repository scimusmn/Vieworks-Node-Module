#ifndef CYUV422_8_YUYV_H
#define CYUV422_8_YUYV_H

#include "iyuv.h"

namespace PixelFormat
{
    namespace Yuv
    {
        class CYuv422_8_YUYV : public IYuv
        {
        public:
            CYuv422_8_YUYV(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CYuv422_8_YUYV();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t* ToRGB8(uint8_t* pRawImage);
            bool SaveImage();

            void ConvertYUV422_YUYVtoBGR8(const BYTE *pbSrc, BYTE *pbDst, DWORD nWidth, DWORD nHeight);
            void ConvertYUV422_YUYVtoRGB8(const BYTE *pbSrc, BYTE *pbDst, DWORD nWidth, DWORD nHeight);
        };
    }
}

#endif // CYUV422_8_YUYV_H
