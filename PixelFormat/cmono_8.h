#ifndef CMONO_8_H
#define CMONO_8_H

#include "imono.h"

namespace PixelFormat
{
    namespace Mono
    {
        class CMono_8 : public IMono
        {
        public:
            CMono_8(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CMono_8();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t* ToRGB8(uint8_t* pRawImage);
            bool SaveImage();
        };

    }
}

#endif // CMONO_8_H
