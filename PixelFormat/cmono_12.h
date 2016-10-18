#ifndef CMONO_12_H
#define CMONO_12_H

#include "imono.h"

namespace PixelFormat
{
    namespace Mono
    {

        class CMono_12 : public IMono
        {
        public:
            CMono_12(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CMono_12();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t* ToRGB8(uint8_t *pRawImage);
            bool SaveImage();
        };

    }
}

#endif // CMONO_12_H
