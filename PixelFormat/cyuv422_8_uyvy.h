#ifndef CYUV422_8_UYVY_H
#define CYUV422_8_UYVY_H

#include "iyuv.h"

namespace PixelFormat
{
    namespace Yuv
    {

        class CYuv422_8_UYVY : public IYuv
        {
        public:
            CYuv422_8_UYVY(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CYuv422_8_UYVY();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t* ToRGB8(uint8_t* pRawImage);
            bool SaveImage();

            void ConvertYUV422_UYVYtoBGR8(const BYTE *pbSrc, BYTE *pbDst, DWORD nWidth, DWORD nHeight);
            void ConvertYUV422_UYVYtoRGB8(const BYTE *pbSrc, BYTE *pbDst, DWORD nWidth, DWORD nHeight);
        };

    }
}
#endif // CYUV422_8_UYVY_H
