#include "cbayer_gr10pack.h"
#include "cbayer_gr10.h"

namespace PixelFormat
{
    namespace Bayer
    {

        CBayer_GR10Pack::CBayer_GR10Pack(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
            : IBayer(unWidth, unHeight, unBpp)
        {
            m_pCovertedImage = new uint8_t[m_unWidth * m_unHeight * BGR_PIXEL];
            m_pTmpBuffer = new uint8_t[m_unWidth * m_unHeight * 2];
        }

        CBayer_GR10Pack::~CBayer_GR10Pack()
        {
            DEBUG_LOG("CBayer_GR10Pack::~CBayer_GR10Pack()")
            SAFE_DELETE_ARRAY(m_pCovertedImage)
            SAFE_DELETE_ARRAY(m_pTmpBuffer)
        }


        uint8_t* CBayer_GR10Pack::ToBGR8(uint8_t *pRawImage)
        {
            ConvertMono10PackedToMono16bit(pRawImage, m_unWidth, m_unHeight, m_pTmpBuffer);
            CBayer_GR10::ConvertBAYGR10ToBGR8((uint16_t*)m_pTmpBuffer, m_pCovertedImage, m_unWidth, m_unHeight);

            return m_pCovertedImage;
        }

        uint8_t* CBayer_GR10Pack::ToRGB8(uint8_t *pRawImage)
        {
            ConvertMono10PackedToMono16bit(pRawImage, m_unWidth, m_unHeight, m_pTmpBuffer);
            CBayer_GR10::ConvertBAYGR10ToRGB8((uint16_t*)m_pTmpBuffer, m_pCovertedImage, m_unWidth, m_unHeight);

            return m_pCovertedImage;
        }

        bool CBayer_GR10Pack::SaveImage()
        {
            DEBUG_LOG("Does not Implement.")
        }

    }
}
