#ifndef CMONO_10_H
#define CMONO_10_H

#include "imono.h"

namespace PixelFormat
{
    namespace Mono
    {

        class CMono_10 : public IMono
        {
        public:
            CMono_10(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp);
            virtual ~CMono_10();

            uint8_t* ToBGR8(uint8_t* pRawImage);
            uint8_t* ToRGB8(uint8_t *pRawImage);
            bool SaveImage();
        };

    }

}

#endif // CMONO_10_H
