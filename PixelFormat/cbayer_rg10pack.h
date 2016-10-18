#ifndef CBAYER_RG10PACK_H
#define CBAYER_RG10PACK_H

#include "ibayer.h"

namespace PixelFormat
{
    namespace Bayer
    {
        class CBayer_RG10Pack : public IBayer
        {
        public:
            CBayer_RG10Pack(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CBayer_RG10Pack();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t *ToRGB8(uint8_t *pRawImage);
            bool SaveImage();


        };
    }
}

#endif // CBAYER_RG10PACK_H
