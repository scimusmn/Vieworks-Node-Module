#include "cbayer_gr12.h"

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


        CBayer_GR12::CBayer_GR12(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
            : IBayer(unWidth, unHeight, unBpp)
        {
            m_pCovertedImage = new uint8_t[m_unWidth * m_unHeight * BGR_PIXEL];
        }

        CBayer_GR12::~CBayer_GR12()
        {
            DEBUG_LOG("CBayer_GR12::~CBayer_GR12()")
            SAFE_DELETE_ARRAY(m_pCovertedImage)
        }


        uint8_t* CBayer_GR12::ToBGR8(uint8_t *pRawImage)
        {
            ConvertBAYGR12ToBGR8((WORD*)pRawImage, m_pCovertedImage, m_unWidth, m_unHeight);

            return m_pCovertedImage;
        }

        uint8_t* CBayer_GR12::ToRGB8(uint8_t *pRawImage)
        {
            ConvertBAYGR12ToRGB8((WORD*)pRawImage, m_pCovertedImage, m_unWidth, m_unHeight);

            return m_pCovertedImage;
        }

        bool CBayer_GR12::SaveImage()
        {
            DEBUG_LOG("Does not Implement.")
        }


        void CBayer_GR12::ConvertBAYGR12ToBGR8(const WORD* pbSrc, BYTE* pbDst, DWORD width, DWORD height)
        {
            //BYTE* pbDst = pbDst + (height - 1) * width * 3;
            *pbDst++ = BELOW;
            *pbDst++ = SELF;
            *pbDst++ = RIGHT;
            pbSrc++;
            for(DWORD x = 1; x < width-1; x += 2)
            {
                *pbDst++ = AVG2(BELOWLEFT,BELOWRIGHT);
                *pbDst++ = AVG3(LEFT,RIGHT,BELOW);
                *pbDst++ = SELF;
                pbSrc++;
                *pbDst++ = BELOW;
                *pbDst++ = SELF;
                *pbDst++ = AVG2(LEFT,RIGHT);
                pbSrc++;
            }
            *pbDst++ = BELOWLEFT;
            *pbDst++ = AVG2(LEFT,BELOW);;
            *pbDst++ = SELF;
            pbSrc++;
            for(DWORD y = 1; y < height-1; y += 2)
            {
                //pbDst = pbDst + (height - y - 1) * width * 3;
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
                //pbDst = pbDst + (height - y - 2) * width * 3;
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
                *pbDst++ = AVG2(BELOWLEFT,ABOVELEFT);
                *pbDst++ = AVG3(ABOVE,BELOW,LEFT);
                *pbDst++ = SELF;
                pbSrc++;
            }
            pbDst = pbDst;
            *pbDst++ = SELF;
            *pbDst++ = AVG2(ABOVE,RIGHT);;
            *pbDst++ = ABOVERIGHT;
            pbSrc++;
            for( DWORD x = 1; x < width-1; x += 2)
            {
                *pbDst++ = AVG2(LEFT,RIGHT);
                *pbDst++ = SELF;
                *pbDst++ = ABOVE;
                pbSrc++;
                *pbDst++ = SELF;
                *pbDst++ = AVG3(ABOVE,LEFT,RIGHT);
                *pbDst++ = AVG2(ABOVELEFT,ABOVERIGHT);
                pbSrc++;
            }
            *pbDst++ = LEFT;
            *pbDst++ = SELF;
            *pbDst++ = ABOVE;
            pbSrc++;
        }

        void CBayer_GR12::ConvertBAYGR12ToRGB8(const WORD* pbSrc, BYTE* pbDst, DWORD width, DWORD height)
        {
            //BYTE* pbDst = pbDst + (height - 1) * width * 3;
            *pbDst++ = RIGHT;
            *pbDst++ = SELF;
            *pbDst++ = BELOW;
            pbSrc++;
            for(DWORD x = 1; x < width-1; x += 2)
            {
                *pbDst++ = SELF;
                *pbDst++ = AVG3(LEFT,RIGHT,BELOW);
                *pbDst++ = AVG2(BELOWLEFT,BELOWRIGHT);
                pbSrc++;
                *pbDst++ = AVG2(LEFT,RIGHT);
                *pbDst++ = SELF;
                *pbDst++ = BELOW;
                pbSrc++;
            }
            *pbDst++ = SELF;
            *pbDst++ = AVG2(LEFT,BELOW);
            *pbDst++ = BELOWLEFT;
            pbSrc++;
            for(DWORD y = 1; y < height-1; y += 2)
            {
                //pbDst = pbDst + (height - y - 1) * width * 3;
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
                //pbDst = pbDst + (height - y - 2) * width * 3;
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
                *pbDst++ = AVG3(ABOVE,BELOW,LEFT);
                *pbDst++ = AVG2(BELOWLEFT,ABOVELEFT);
                pbSrc++;
            }
            pbDst = pbDst;
            *pbDst++ = ABOVERIGHT;
            *pbDst++ = AVG2(ABOVE,RIGHT);;
            *pbDst++ = SELF;
            pbSrc++;
            for( DWORD x = 1; x < width-1; x += 2)
            {
                *pbDst++ = ABOVE;
                *pbDst++ = SELF;
                *pbDst++ = AVG2(LEFT,RIGHT);
                pbSrc++;
                *pbDst++ = AVG2(ABOVELEFT,ABOVERIGHT);
                *pbDst++ = AVG3(ABOVE,LEFT,RIGHT);
                *pbDst++ = SELF;
                pbSrc++;
            }
            *pbDst++ = ABOVE;
            *pbDst++ = SELF;
            *pbDst++ = LEFT;
            pbSrc++;
        }

    }//            namespace Bayer

}//        namespace PixelFormat

