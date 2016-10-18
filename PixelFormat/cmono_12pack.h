#ifndef CMONO_12PACK_H
#define CMONO_12PACK_H

#include "imono.h"

namespace PixelFormat
{
    namespace Mono
    {
        class CMono_12Pack : public IMono
        {
        public:
            CMono_12Pack(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CMono_12Pack();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t* ToRGB8(uint8_t *pRawImage);
            bool SaveImage();
        };
    }
}
#endif // CMONO_12PACK_H
