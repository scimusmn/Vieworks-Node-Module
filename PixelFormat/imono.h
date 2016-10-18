#ifndef IMONO_H
#define IMONO_H

#include "ipixelformat.h"

namespace PixelFormat
{
    namespace Mono
    {
        class IMono : public IPixelFormat
        {
        public:
            IMono(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
                :IPixelFormat(unWidth, unHeight, unBpp){};

            virtual ~IMono(){DEBUG_LOG("IMono::~IMono()")};
        };

    }//namespace Mono
}//namespace PixelFormat

#endif // IMONO_H
