#ifndef CMONO_16_H
#define CMONO_16_H

#include "imono.h"

namespace PixelFormat
{
    namespace Mono
    {
        class CMono_16 : public IMono
        {
        public:
            CMono_16(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CMono_16();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t* ToRGB8(uint8_t *pRawImage);
            bool SaveImage();
        };
    }
}


#endif // CMONO_16_H
