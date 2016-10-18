#ifndef CMONO_10PACK_H
#define CMONO_10PACK_H

#include "imono.h"

namespace PixelFormat
{
    namespace Mono
    {

        class CMono_10Pack : public IMono
        {
        public:
            CMono_10Pack(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CMono_10Pack();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t* ToRGB8(uint8_t *pRawImage);
            bool SaveImage();
        };

    }
}

#endif // CMONO_10PACK_H
