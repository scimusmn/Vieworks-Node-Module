#include "cbayer_rg12.h"


#define SELF		BYTE(*pbSrc>>4)
#define RIGHT		BYTE(*(pbSrc+1)>>4)
#define LEFT		BYTE(*(pbSrc-1)>>4)
#define ABOVE		BYTE(*(pbSrc-width)>>4)
#define BELOW		BYTE(*(pbSrc+width)>>4)
#define ABOVELEFT	BYTE(*(pbSrc-width-1)>>4)
#define ABOVERIGHT	BYTE(*(pbSrc-width+1)>>4)
#define BELOWLEFT	BYTE(*(pbSrc+width-1)>>4)
#define BELOWRIGHT	BYTE(*(pbSrc+width+1)>>4)

#define AVG2(a,b)		((BYTE)((DWORD(a)+DWORD(b))/2))
#define AVG3(a,b,c)		((BYTE)((DWORD(a)+DWORD(b)+DWORD(c))/3))
#define AVG4(a,b,c,d)	((BYTE)((DWORD(a)+DWORD(b)+DWORD(c)+DWORD(d))/4))




namespace PixelFormat
{
    namespace Bayer
    {
        CBayer_RG12::CBayer_RG12(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
            : IBayer(unWidth, unHeight, unBpp)
        {
            m_pCovertedImage = new uint8_t[m_unWidth * m_unHeight * BGR_PIXEL];
        }

        CBayer_RG12::~CBayer_RG12()
        {
            DEBUG_LOG("CBayer_GR12::~CBayer_GR12()")
            SAFE_DELETE_ARRAY(m_pCovertedImage)
        }


        uint8_t* CBayer_RG12::ToBGR8(uint8_t *pRawImage)
        {
            ConvertBAYRG12ToBGR8((WORD*)pRawImage, m_pCovertedImage, m_unWidth, m_unHeight);

            return m_pCovertedImage;
        }

        uint8_t *CBayer_RG12::ToRGB8(uint8_t *pRawImage)
        {
            ConvertBAYRG12ToRGB8((WORD*)pRawImage, m_pCovertedImage, m_unWidth, m_unHeight);

            return m_pCovertedImage;
        }

        bool CBayer_RG12::SaveImage()
        {
            DEBUG_LOG("Does not Implement.")
        }

        void CBayer_RG12::ConvertBAYRG12ToBGR8(const WORD* pbSrc, BYTE* pbDst, DWORD width, DWORD height)
        {

            *pbDst++ = BELOWRIGHT;
            *pbDst++ = AVG2(RIGHT,BELOW);
            *pbDst++ = SELF;
            pbSrc++;

            for(DWORD x = 1; x < width-1; x += 2)
            {
                *pbDst++ = BELOW;
                *pbDst++ = SELF;
                *pbDst++ = AVG2(LEFT,RIGHT);
                pbSrc++;
                *pbDst++ = AVG2(BELOWLEFT,BELOWRIGHT);
                *pbDst++ = AVG3(LEFT,RIGHT,BELOW);
                *pbDst++ = SELF;
                pbSrc++;
            }

            *pbDst++ = BELOW;
            *pbDst++ = SELF;
            *pbDst++ = LEFT;
            pbSrc++;

            for(DWORD y = 1; y < height-1; y += 2)
            {
                *pbDst++ = RIGHT;
                *pbDst++ = SELF;
                *pbDst++ = AVG2(ABOVE,BELOW);
                pbSrc++;

                for(DWORD x = 1; x < width-1; x += 2)
                {
                    *pbDst++ = SELF;
                    *pbDst++ = AVG4(ABOVE,BELOW,LEFT,RIGHT);
                    *pbDst++ = AVG4(ABOVELEFT,ABOVERIGHT,BELOWLEFT,BELOWRIGHT);
                    pbSrc++;
                    *pbDst++ = AVG2(LEFT,RIGHT);
                    *pbDst++ = SELF;
                    *pbDst++ = AVG2(ABOVE,BELOW);
                    pbSrc++;
                }

                *pbDst++ = SELF;
                *pbDst++ = AVG2(ABOVE,BELOW);
                *pbDst++ = AVG2(ABOVELEFT,BELOWLEFT);
                pbSrc++;

                *pbDst++ = AVG2(ABOVERIGHT,BELOWRIGHT);
                *pbDst++ = AVG3(ABOVE,RIGHT,BELOW);
                *pbDst++ = SELF;
                pbSrc++;

                for(DWORD x = 1; x < width-1; x += 2)
                {
                    *pbDst++ = AVG2(ABOVE,BELOW);
                    *pbDst++ = SELF;
                    *pbDst++ = AVG2(LEFT,RIGHT);
                    pbSrc++;
                    *pbDst++ = AVG4(ABOVELEFT,ABOVERIGHT,BELOWLEFT,BELOWRIGHT);
                    *pbDst++ = AVG4(ABOVE,BELOW,LEFT,RIGHT);
                    *pbDst++ = SELF;
                    pbSrc++;
                }

                *pbDst++ = AVG2(ABOVE,BELOW);
                *pbDst++ = SELF;
                *pbDst++ = LEFT;
                pbSrc++;
            }

            /*
            RGRGRGRG
            GBGBGBGB
            RGRGRGRG
            GBGBGBGB
            RGRGRGRG
            GBGBGBGB
            */

            *pbDst++ = RIGHT;
            *pbDst++ = SELF;
            *pbDst++ = ABOVE;
            pbSrc++;

            for(DWORD x = 1; x < width-1; x += 2)
            {
                *pbDst++ = SELF;
                *pbDst++ = AVG3(ABOVE,LEFT,RIGHT);
                *pbDst++ = AVG2(ABOVELEFT,ABOVERIGHT);
                pbSrc++;
                *pbDst++ = AVG2(LEFT,RIGHT);
                *pbDst++ = SELF;
                *pbDst++ = ABOVE;
                pbSrc++;
            }

            *pbDst++ = SELF;
            *pbDst++ = AVG2(ABOVE,LEFT);
            *pbDst++ = ABOVELEFT;
            pbSrc++;


        }


        void CBayer_RG12::ConvertBAYRG12ToRGB8(const WORD* pbSrc, BYTE* pbDst, DWORD width, DWORD height)
        {

            *pbDst++ = SELF;
            *pbDst++ = AVG2(RIGHT,BELOW);
            *pbDst++ = BELOWRIGHT;
            pbSrc++;

            for(DWORD x = 1; x < width-1; x += 2)
            {
                *pbDst++ = AVG2(LEFT,RIGHT);
                *pbDst++ = SELF;
                *pbDst++ = BELOW;
                pbSrc++;
                *pbDst++ = SELF;
                *pbDst++ = AVG3(LEFT,RIGHT,BELOW);
                *pbDst++ = AVG2(BELOWLEFT,BELOWRIGHT);
                pbSrc++;
            }

            *pbDst++ = LEFT;
            *pbDst++ = SELF;
            *pbDst++ = BELOW;
            pbSrc++;

            for(DWORD y = 1; y < height-1; y += 2)
            {
                *pbDst++ = AVG2(ABOVE,BELOW);
                *pbDst++ = SELF;
                *pbDst++ = RIGHT;
                pbSrc++;

                for(DWORD x = 1; x < width-1; x += 2)
                {
                    *pbDst++ = AVG4(ABOVELEFT,ABOVERIGHT,BELOWLEFT,BELOWRIGHT);
                    *pbDst++ = AVG4(ABOVE,BELOW,LEFT,RIGHT);
                    *pbDst++ = SELF;
                    pbSrc++;
                    *pbDst++ = AVG2(ABOVE,BELOW);
                    *pbDst++ = SELF;
                    *pbDst++ = AVG2(LEFT,RIGHT);
                    pbSrc++;
                }

                *pbDst++ = AVG2(ABOVELEFT,BELOWLEFT);
                *pbDst++ = AVG2(ABOVE,BELOW);
                *pbDst++ = SELF;
                pbSrc++;

                *pbDst++ = SELF;
                *pbDst++ = AVG3(ABOVE,RIGHT,BELOW);
                *pbDst++ = AVG2(ABOVERIGHT,BELOWRIGHT);
                pbSrc++;

                for(DWORD x = 1; x < width-1; x += 2)
                {
                    *pbDst++ = AVG2(LEFT,RIGHT);
                    *pbDst++ = SELF;
                    *pbDst++ = AVG2(ABOVE,BELOW);
                    pbSrc++;
                    *pbDst++ = SELF;
                    *pbDst++ = AVG4(ABOVE,BELOW,LEFT,RIGHT);
                    *pbDst++ = AVG4(ABOVELEFT,ABOVERIGHT,BELOWLEFT,BELOWRIGHT);
                    pbSrc++;
                }

                *pbDst++ = LEFT;
                *pbDst++ = SELF;
                *pbDst++ = AVG2(ABOVE,BELOW);
                pbSrc++;
            }

            /*
            RGRGRGRG
            GBGBGBGB
            RGRGRGRG
            GBGBGBGB
            RGRGRGRG
            GBGBGBGB
            */

            *pbDst++ = ABOVE;
            *pbDst++ = SELF;
            *pbDst++ = RIGHT;
            pbSrc++;

            for(DWORD x = 1; x < width-1; x += 2)
            {
                *pbDst++ = AVG2(ABOVELEFT,ABOVERIGHT);
                *pbDst++ = AVG3(ABOVE,LEFT,RIGHT);
                *pbDst++ = SELF;
                pbSrc++;

                *pbDst++ = ABOVE;
                *pbDst++ = SELF;
                *pbDst++ = AVG2(LEFT,RIGHT);
                pbSrc++;
            }

            *pbDst++ = ABOVELEFT;
            *pbDst++ = AVG2(ABOVE,LEFT);
            *pbDst++ = SELF;
            pbSrc++;


        }

    }//namespace PixelFormat
}//namespace Bayer
