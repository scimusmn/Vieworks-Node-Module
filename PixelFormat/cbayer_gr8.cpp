#include "cbayer_gr8.h"

#define SELF		BYTE(*pbSrc)
#define RIGHT		BYTE(*(pbSrc+1))
#define LEFT		BYTE(*(pbSrc-1))
#define ABOVE		BYTE(*(pbSrc-width))
#define BELOW		BYTE(*(pbSrc+width))
#define ABOVELEFT	BYTE(*(pbSrc-width-1))
#define ABOVERIGHT	BYTE(*(pbSrc-width+1))
#define BELOWLEFT	BYTE(*(pbSrc+width-1))
#define BELOWRIGHT	BYTE(*(pbSrc+width+1))

#define AVG2(a,b)		((BYTE)((DWORD(a)+DWORD(b))/2))
#define AVG3(a,b,c)		((BYTE)((DWORD(a)+DWORD(b)+DWORD(c))/3))
#define AVG4(a,b,c,d)	((BYTE)((DWORD(a)+DWORD(b)+DWORD(c)+DWORD(d))/4))

namespace PixelFormat
{
    namespace Bayer
    {

        CBayer_GR8::CBayer_GR8(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
            : IBayer(unWidth, unHeight, unBpp)
        {
            m_pCovertedImage = new uint8_t[m_unWidth * m_unHeight * BGR_PIXEL];
        }

        CBayer_GR8::~CBayer_GR8()
        {
            //DEBUG_LOG("CBayer_GR8::~CBayer_GR8()")
            //SAFE_DELETE_ARRAY(m_pCovertedImage)
        }


        uint8_t* CBayer_GR8::ToBGR8(uint8_t *pRawImage)
        {
            ConvertBAYGR8ToBGR8(pRawImage, m_pCovertedImage, m_unWidth, m_unHeight);

            return m_pCovertedImage;
        }

        uint8_t* CBayer_GR8::ToRGB8(uint8_t *pRawImage)
        {
            ConvertBAYGR8ToRGB8(pRawImage, m_pCovertedImage, m_unWidth, m_unHeight);

            return m_pCovertedImage;
        }

        bool CBayer_GR8::SaveImage()
        {
            //DEBUG_LOG("Does not Implement.")
        }


        void CBayer_GR8::ConvertBAYGR8ToBGR8(const BYTE* pbSrc, BYTE* pbDst, DWORD width, DWORD height)
        {
            //BYTE* pbtempDst = pbDst + (height - 1) * width * 3;
            BYTE* pbtempDst = pbDst;

            *pbtempDst++ = BELOW;
            *pbtempDst++ = SELF;
            *pbtempDst++ = RIGHT;
            pbSrc++;

            for(DWORD x = 1; x < width-1; x += 2)
            {
                *pbtempDst++ = AVG2(BELOWLEFT,BELOWRIGHT);
                *pbtempDst++ = AVG3(LEFT,RIGHT,BELOW);
                *pbtempDst++ = SELF;
                pbSrc++;
                *pbtempDst++ = BELOW;
                *pbtempDst++ = SELF;
                *pbtempDst++ = AVG2(LEFT,RIGHT);
                pbSrc++;
            }

            *pbtempDst++ = BELOWLEFT;
            *pbtempDst++ = AVG2(LEFT,BELOW);;
            *pbtempDst++ = SELF;
            pbSrc++;

            for(DWORD y = 1; y < height-1; y += 2)
            {
                //pbtempDst = pbDst + (height - y - 1) * width * 3;
                pbtempDst = pbDst + y  * width * 3;

                *pbtempDst++ = SELF;
                *pbtempDst++ = AVG3(ABOVE,RIGHT,BELOW);
                *pbtempDst++ = AVG2(ABOVERIGHT,BELOWRIGHT);
                pbSrc++;

                for(DWORD x = 1; x < width-1; x += 2)
                {
                    *pbtempDst++ = AVG2(LEFT,RIGHT);
                    *pbtempDst++ = SELF;
                    *pbtempDst++ = AVG2(ABOVE,BELOW);
                    pbSrc++;
                    *pbtempDst++ = SELF;
                    *pbtempDst++ = AVG4(ABOVE,BELOW,LEFT,RIGHT);
                    *pbtempDst++ = AVG4(ABOVELEFT,ABOVERIGHT,BELOWLEFT,BELOWRIGHT);
                    pbSrc++;
                }

                *pbtempDst++ = LEFT;
                *pbtempDst++ = SELF;
                *pbtempDst++ = AVG2(ABOVE,BELOW);
                pbSrc++;

                //pbtempDst = pbDst + (height - y - 2) * width * 3;
                pbtempDst = pbDst +  ( y + 1 ) * width * 3;

                *pbtempDst++ = AVG2(ABOVE,BELOW);
                *pbtempDst++ = SELF;
                *pbtempDst++ = RIGHT;
                pbSrc++;

                for(DWORD x = 1; x < width-1; x += 2)
                {
                    *pbtempDst++ = AVG4(ABOVELEFT,ABOVERIGHT,BELOWLEFT,BELOWRIGHT);
                    *pbtempDst++ = AVG4(ABOVE,BELOW,LEFT,RIGHT);
                    *pbtempDst++ = SELF;
                    pbSrc++;
                    *pbtempDst++ = AVG2(ABOVE,BELOW);
                    *pbtempDst++ = SELF;
                    *pbtempDst++ = AVG2(LEFT,RIGHT);
                    pbSrc++;
                }

                *pbtempDst++ = AVG2(BELOWLEFT,ABOVELEFT);
                //*pbtempDst++ = AVG2(ABOVE,BELOW,LEFT);
                *pbtempDst++ = AVG2(ABOVE,BELOW); // Ethan 2011/01/27 - not for sure.
                *pbtempDst++ = SELF;
                pbSrc++;
            }

        /*
        GRGRGRGR
        BGBGBGBG
        GRGRGRGR
        BGBGBGBG
        GRGRGRGR
        BGBGBGBG
        */

            //pbtempDst = pbDst;

            *pbtempDst++ = SELF;
            *pbtempDst++ = AVG2(ABOVE,RIGHT);;
            *pbtempDst++ = ABOVERIGHT;
            pbSrc++;

            for(DWORD x = 1; x < width-1; x += 2)
            {
                *pbtempDst++ = AVG2(LEFT,RIGHT);
                *pbtempDst++ = SELF;
                *pbtempDst++ = ABOVE;
                pbSrc++;
                *pbtempDst++ = SELF;
                *pbtempDst++ = AVG3(ABOVE,LEFT,RIGHT);
                *pbtempDst++ = AVG2(ABOVELEFT,ABOVERIGHT);
                pbSrc++;
            }

            *pbtempDst++ = LEFT;
            *pbtempDst++ = SELF;
            *pbtempDst++ = ABOVE;
            pbSrc++;
        }

        void CBayer_GR8::ConvertBAYGR8ToRGB8(const BYTE* pbSrc, BYTE* pbDst, DWORD width, DWORD height)
        {
            //BYTE* pbtempDst = pbDst + (height - 1) * width * 3;
            BYTE* pbtempDst = pbDst;

            *pbtempDst++ = RIGHT;
            *pbtempDst++ = SELF;
            *pbtempDst++ = BELOW;
            pbSrc++;

            for(DWORD x = 1; x < width-1; x += 2)
            {
                *pbtempDst++ = SELF;
                *pbtempDst++ = AVG3(LEFT,RIGHT,BELOW);
                *pbtempDst++ = AVG2(BELOWLEFT,BELOWRIGHT);
                pbSrc++;
                *pbtempDst++ = AVG2(LEFT,RIGHT);
                *pbtempDst++ = SELF;
                *pbtempDst++ = BELOW;
                pbSrc++;
            }

            *pbtempDst++ = SELF;
            *pbtempDst++ = AVG2(LEFT,BELOW);
            *pbtempDst++ = BELOWLEFT;
            pbSrc++;

            for(DWORD y = 1; y < height-1; y += 2)
            {
                //pbtempDst = pbDst + (height - y - 1) * width * 3;
                pbtempDst = pbDst + y  * width * 3;

                *pbtempDst++ = AVG2(ABOVERIGHT,BELOWRIGHT);
                *pbtempDst++ = AVG3(ABOVE,RIGHT,BELOW);
                *pbtempDst++ = SELF;
                pbSrc++;

                for(DWORD x = 1; x < width-1; x += 2)
                {
                    *pbtempDst++ = AVG2(ABOVE,BELOW);
                    *pbtempDst++ = SELF;
                    *pbtempDst++ = AVG2(LEFT,RIGHT);
                    pbSrc++;
                    *pbtempDst++ = AVG4(ABOVELEFT,ABOVERIGHT,BELOWLEFT,BELOWRIGHT);
                    *pbtempDst++ = AVG4(ABOVE,BELOW,LEFT,RIGHT);
                    *pbtempDst++ = SELF;
                    pbSrc++;
                }

                *pbtempDst++ = AVG2(ABOVE,BELOW);
                *pbtempDst++ = SELF;
                *pbtempDst++ = LEFT;
                pbSrc++;

                //pbtempDst = pbDst + (height - y - 2) * width * 3;
                pbtempDst = pbDst +  ( y + 1 ) * width * 3;

                *pbtempDst++ = RIGHT;
                *pbtempDst++ = SELF;
                *pbtempDst++ = AVG2(ABOVE,BELOW);
                pbSrc++;

                for(DWORD x = 1; x < width-1; x += 2)
                {
                    *pbtempDst++ = SELF;
                    *pbtempDst++ = AVG4(ABOVE,BELOW,LEFT,RIGHT);
                    *pbtempDst++ = AVG4(ABOVELEFT,ABOVERIGHT,BELOWLEFT,BELOWRIGHT);
                    pbSrc++;
                    *pbtempDst++ = AVG2(LEFT,RIGHT);
                    *pbtempDst++ = SELF;
                    *pbtempDst++ = AVG2(ABOVE,BELOW);
                    pbSrc++;
                }

                //*pbtempDst++ = AVG2(ABOVE,BELOW,LEFT);
                *pbtempDst++ = SELF;
                *pbtempDst++ = AVG2(ABOVE,BELOW); // Ethan 2011/01/27 - not for sure.
                *pbtempDst++ = AVG2(BELOWLEFT,ABOVELEFT);
                pbSrc++;
            }

        /*
        GRGRGRGR
        BGBGBGBG
        GRGRGRGR
        BGBGBGBG
        GRGRGRGR
        BGBGBGBG
        */

            //pbtempDst = pbDst;

            *pbtempDst++ = ABOVERIGHT;
            *pbtempDst++ = AVG2(ABOVE,RIGHT);;
            *pbtempDst++ = SELF;
            pbSrc++;

            for(DWORD x = 1; x < width-1; x += 2)
            {
                *pbtempDst++ = ABOVE;
                *pbtempDst++ = SELF;
                *pbtempDst++ = AVG2(LEFT,RIGHT);
                pbSrc++;
                *pbtempDst++ = AVG2(ABOVELEFT,ABOVERIGHT);
                *pbtempDst++ = AVG3(ABOVE,LEFT,RIGHT);
                *pbtempDst++ = SELF;
                pbSrc++;
            }

            *pbtempDst++ = ABOVE;
            *pbtempDst++ = SELF;
            *pbtempDst++ = LEFT;
            pbSrc++;
        }


    }//namespace Bayer
}//namespace PixelFormat
