#ifndef CBAYER_RG12_H
#define CBAYER_RG12_H

#include "ibayer.h"

namespace PixelFormat
{
    namespace Bayer
    {
        class CBayer_RG12 : public IBayer
        {
        public:
            CBayer_RG12(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CBayer_RG12();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t* ToRGB8(uint8_t* pRawImage);
            bool SaveImage();

            static void ConvertBAYRG12ToBGR8(const WORD *pbSrc, BYTE *pbDst, DWORD width, DWORD height);
            static void ConvertBAYRG12ToRGB8(const WORD *pbSrc, BYTE *pbDst, DWORD width, DWORD height);
        };
    }
}
#endif // CBAYER_RG12_H
