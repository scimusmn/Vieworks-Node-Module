#include "cmono_8.h"


namespace PixelFormat
{
    namespace Mono
    {

        CMono_8::CMono_8(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
            : IMono(unWidth, unHeight, unBpp)
        {
            m_pCovertedImage = new uint8_t[m_unWidth * m_unHeight * BGR_PIXEL];
        }

        CMono_8::~CMono_8()
        {
            DEBUG_LOG("CMono_8::~CMono8()")
            SAFE_DELETE_ARRAY(m_pCovertedImage)
        }


        uint8_t* CMono_8::ToBGR8(uint8_t *pRawImage)
        {
            uint32_t unBuffer_size = m_unWidth * m_unHeight * m_unBpp / 8;

            uint8_t* p = m_pCovertedImage;

            for(int i = 0; i < unBuffer_size; i++)
            {
                *p++=pRawImage[i];
                *p++=pRawImage[i];
                *p++=pRawImage[i];
            }

            return m_pCovertedImage;

        }

        uint8_t* CMono_8::ToRGB8(uint8_t *pRawImage)
        {
            return ToBGR8(pRawImage);
        }

        bool CMono_8::SaveImage()
        {
            DEBUG_LOG("Does not Implement.")
        }

    }
}
