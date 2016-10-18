#ifndef IYUV_H
#define IYUV_H

#include "ipixelformat.h"

namespace PixelFormat
{
    namespace Yuv
    {
        class IYuv : public IPixelFormat
        {
        public:
            IYuv(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
                :IPixelFormat(unWidth, unHeight, unBpp){};

            virtual ~IYuv(){DEBUG_LOG("IYuv::~IYuv()")};

        };

    }
}

#endif // IYUV_H
