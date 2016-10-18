#ifndef CBAYER_GR8_H
#define CBAYER_GR8_H

#include "ibayer.h"

namespace PixelFormat
{
    namespace Bayer
    {
        class CBayer_GR8 : public IBayer
        {
        public:
            CBayer_GR8(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CBayer_GR8();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t *ToRGB8(uint8_t *pRawImage);
            bool SaveImage();

            void ConvertBAYGR8ToBGR8(const BYTE *pbSrc, BYTE *pbDst, DWORD width, DWORD height);
            void ConvertBAYGR8ToRGB8(const BYTE *pbSrc, BYTE *pbDst, DWORD width, DWORD height);

        };
    }
}
#endif // CBAYER_GR8_H
