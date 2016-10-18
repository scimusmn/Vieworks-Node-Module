#include "cbayer_gr12pack.h"
#include "cbayer_gr12.h"


namespace PixelFormat
{
    namespace Bayer
    {
        CBayer_GR12Pack::CBayer_GR12Pack(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
            : IBayer(unWidth, unHeight, unBpp)
        {
            m_pCovertedImage = new uint8_t[m_unWidth * m_unHeight * BGR_PIXEL];
            m_pTmpBuffer = new uint8_t[m_unWidth * m_unHeight * 2];
        }

        CBayer_GR12Pack::~CBayer_GR12Pack()
        {
            DEBUG_LOG("CBayer_GR10Pack::~CBayer_GR10Pack()")
            SAFE_DELETE_ARRAY(m_pCovertedImage)
            SAFE_DELETE_ARRAY(m_pTmpBuffer)
        }


        uint8_t* CBayer_GR12Pack::ToBGR8(uint8_t *pRawImage)
        {
            ConvertMono12PackedToMono16bit(pRawImage, m_unWidth, m_unHeight, m_pTmpBuffer);
            CBayer_GR12::ConvertBAYGR12ToBGR8((uint16_t*)m_pTmpBuffer, m_pCovertedImage, m_unWidth, m_unHeight);

            return m_pCovertedImage;
        }

        uint8_t* CBayer_GR12Pack::ToRGB8(uint8_t *pRawImage)
        {
            ConvertMono12PackedToMono16bit(pRawImage, m_unWidth, m_unHeight, m_pTmpBuffer);
            CBayer_GR12::ConvertBAYGR12ToRGB8((uint16_t*)m_pTmpBuffer, m_pCovertedImage, m_unWidth, m_unHeight);

            return m_pCovertedImage;
        }

        bool CBayer_GR12Pack::SaveImage()
        {
            DEBUG_LOG("Does not Implement.")
        }


    }
}
