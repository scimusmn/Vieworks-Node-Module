#ifndef IBAYER_H
#define IBAYER_H

#include "ipixelformat.h"

namespace PixelFormat
{
    namespace Bayer
    {

        class IBayer : public IPixelFormat
        {
        public:
            IBayer(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
                :IPixelFormat(unWidth, unHeight, unBpp){};

            virtual ~IBayer(){/*DEBUG_LOG("IBayer::~IBayer()")*/};

            void ConvertMono10PackedToMono16bit( const BYTE* pbSrc, UINT widht, UINT height, BYTE* pbDst )
            {
                BYTE data = 0;
                int count = 1;

                //Upper 6bit -> Zero.
                for(int i=0;i < widht*height*1.5 ;i=i+3)
                {
                    pbDst[count + i]	 = (pbSrc[i  ] & 0xC0) >> 6;
                    data				 =  pbSrc[i+1] & 0x03;
                    pbDst[count + i - 1] =  data;
                    data				 = (pbSrc[i  ] & 0x3F) << 2;
                    pbDst[count + i - 1] =  pbDst[count+i-1   ] | data;

                    pbDst[count + i + 2] = (pbSrc[i+2] & 0xC0) >> 6;
                    data				 = (pbSrc[i+1] & 0x30) >> 4;
                    pbDst[count + i + 1] =  data;
                    data				 = (pbSrc[i+2] & 0x3F) << 2;
                    pbDst[count + i + 1] =  pbDst[count+i+1   ] | data;

                    count++;
                }
            }

            void ConvertMono12PackedToMono16bit( const BYTE* pbSrc, UINT widht, UINT height, BYTE* pbDst )
            {
                BYTE data = 0;
                int count = 1;

                //Upper 4bit -> Zero.
                for(int i=0;i < widht*height*1.5 ;i=i+3)
                {
                    pbDst[count + i]	  = (pbSrc[i  ] & 0xF0) >> 4;
                    data				  =  pbSrc[i+1] & 0x0F;
                    pbDst[count + i - 1]  =  data;
                    data				  = (pbSrc[i  ] & 0x0F) << 4;
                    pbDst[count + i - 1]  =  pbDst[count+i-1   ] | data;

                    pbDst[count + i + 2]  = (pbSrc[i+2] & 0xF0) >> 4;
                    data				  = (pbSrc[i+1] & 0xF0) >> 4;
                    pbDst[count + i + 1]  =  data;
                    data				  = (pbSrc[i+2] & 0x0F) << 4;
                    pbDst[count + i + 1]  =  pbDst[count+i+1   ] | data;

                    count++;
                }
            }
        };

    }
}
#endif // IBAYER_H
