#include "cmono_10.h"


namespace PixelFormat
{
    namespace Mono
    {

        CMono_10::CMono_10(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
            : IMono(unWidth, unHeight, unBpp)
        {
            m_pCovertedImage = new uint8_t[m_unWidth * m_unHeight * BGR_PIXEL];
        }

        CMono_10::~CMono_10()
        {
            DEBUG_LOG("CMono_10::~CMono10()")
            SAFE_DELETE_ARRAY(m_pCovertedImage)
        }


        uint8_t* CMono_10::ToBGR8(uint8_t *pRawImage)
        {
            uint32_t unBuffer_size = m_unWidth * m_unHeight * m_unBpp / 8;

            uint8_t* p = m_pCovertedImage;

            for(int i = 0; i < unBuffer_size; i+=4)
            {
                uint8_t v = (pRawImage[i+1]<<6)|(pRawImage[i]>>2);
                *p++=v;
                *p++=v;
                *p++=v;

                v = (pRawImage[i+3]<<6)|(pRawImage[i+2]>>2);
                *p++=v;
                *p++=v;
                *p++=v;
            }
            return m_pCovertedImage;

        }

        uint8_t* CMono_10::ToRGB8(uint8_t *pRawImage)
        {
            return ToBGR8(pRawImage);
        }

        bool CMono_10::SaveImage()
        {
            DEBUG_LOG("Does not Implement.")
        }
    }
}
