#ifndef CBAYER_RG8_H
#define CBAYER_RG8_H

#include "ibayer.h"

namespace PixelFormat
{
    namespace Bayer
    {
        class CBayer_RG8 : public IBayer
        {
        public:
            CBayer_RG8(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CBayer_RG8();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t* ToRGB8(uint8_t* pRawImage);

            bool SaveImage();

            void ConvertBAYRG8ToBGR8(const BYTE *pbSrc, BYTE *pbDst, DWORD width, DWORD height);
            void ConvertBAYRG8ToRGB8(const BYTE *pbSrc, BYTE *pbDst, DWORD width, DWORD height);
        };
    }
}
#endif // CBAYER_RG8_H
