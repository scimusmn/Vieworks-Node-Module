#include "cmono_12.h"


namespace PixelFormat
{
    namespace Mono
    {
        CMono_12::CMono_12(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
            : IMono(unWidth, unHeight, unBpp)
        {
            m_pCovertedImage = new uint8_t[m_unWidth * m_unHeight * BGR_PIXEL];
        }

        CMono_12::~CMono_12()
        {
            DEBUG_LOG("CMono_12::~CMono12()")
            SAFE_DELETE_ARRAY(m_pCovertedImage)
        }


        uint8_t* CMono_12::ToBGR8(uint8_t *pRawImage)
        {
            uint32_t unBuffer_size = m_unWidth * m_unHeight * m_unBpp / 8;

            uint8_t* p = m_pCovertedImage;

            for(int i = 0; i < unBuffer_size; i+=4)
            {
                uint8_t v = (pRawImage[i+1]<<4)|(pRawImage[i]>>4);
                *p++=v;
                *p++=v;
                *p++=v;

                v = (pRawImage[i+3]<<4)|(pRawImage[i+2]>>4);
                *p++=v;
                *p++=v;
                *p++=v;
            }
            return m_pCovertedImage;

        }

        uint8_t* CMono_12::ToRGB8(uint8_t *pRawImage)
        {
            return ToBGR8(pRawImage);
        }

        bool CMono_12::SaveImage()
        {
            DEBUG_LOG("Does not Implement.")
        }

    }
}
