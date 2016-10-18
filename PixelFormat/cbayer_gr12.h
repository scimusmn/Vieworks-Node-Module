#ifndef CBAYER_GR12_H
#define CBAYER_GR12_H

#include "ibayer.h"

namespace PixelFormat
{
    namespace Bayer
    {
        class CBayer_GR12 : public IBayer
        {
        public:
            CBayer_GR12(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CBayer_GR12();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t *ToRGB8(uint8_t *pRawImage);
            bool SaveImage();

            static void ConvertBAYGR12ToBGR8(const WORD *pbSrc, BYTE *pbDst, DWORD width, DWORD height);
            static void ConvertBAYGR12ToRGB8(const WORD *pbSrc, BYTE *pbDst, DWORD width, DWORD height);
        };
    }
}
#endif // CBAYER_GR12_H
