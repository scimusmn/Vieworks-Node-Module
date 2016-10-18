#ifndef CBAYER_RG10_H
#define CBAYER_RG10_H

#include "ibayer.h"

namespace PixelFormat
{
    namespace Bayer
    {

        class CBayer_RG10 : public IBayer
        {
        public:
            CBayer_RG10(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CBayer_RG10();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t* ToRGB8(uint8_t* pRawImage);
            bool SaveImage();

            static void ConvertBAYRG10ToBGR8(const WORD *pbSrc, BYTE *pbDst, DWORD width, DWORD height);
            static void ConvertBAYRG10ToRGB8(const WORD *pbSrc, BYTE *pbDst, DWORD width, DWORD height);
        };

    }
}

#endif // CBAYER_RG10_H
