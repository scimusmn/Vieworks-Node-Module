#include "cmono_10pack.h"

namespace PixelFormat
{
    namespace Mono
    {

        CMono_10Pack::CMono_10Pack(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
            : IMono(unWidth, unHeight, unBpp)
        {
            m_pCovertedImage = new uint8_t[m_unWidth * m_unHeight * BGR_PIXEL];
        }

        CMono_10Pack::~CMono_10Pack()
        {
            DEBUG_LOG("CMono_10Pack::~CMono_10Pack()")
            SAFE_DELETE_ARRAY(m_pCovertedImage)
        }


        uint8_t* CMono_10Pack::ToBGR8(uint8_t *pRawImage)
        {
            uint32_t unBuffer_size = m_unWidth * m_unHeight * m_unBpp / 8;

            uint8_t* p = m_pCovertedImage;

            for(int i = 0; i < unBuffer_size; i+=3)
            {
                *p++ = pRawImage[i];
                *p++ = pRawImage[i];
                *p++ = pRawImage[i];

                *p++ = pRawImage[i+2];
                *p++ = pRawImage[i+2];
                *p++ = pRawImage[i+2];
            }
            return m_pCovertedImage;

        }

        uint8_t* CMono_10Pack::ToRGB8(uint8_t *pRawImage)
        {
            return ToBGR8(pRawImage);
        }

        bool CMono_10Pack::SaveImage()
        {
            DEBUG_LOG("Does not Implement.")
        }

    }
}
