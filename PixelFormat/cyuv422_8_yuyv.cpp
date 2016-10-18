#include "cyuv422_8_yuyv.h"


namespace PixelFormat
{
    namespace Yuv
    {

        CYuv422_8_YUYV::CYuv422_8_YUYV(uint32_t unWidth, uint32_t unHeight, uint32_t unBpp)
            : IYuv(unWidth, unHeight, unBpp)
        {
            m_pCovertedImage = new uint8_t[m_unWidth * m_unHeight * BGR_PIXEL];
        }

        CYuv422_8_YUYV::~CYuv422_8_YUYV()
        {
            DEBUG_LOG("CYuv422_8_YUYV::~CYuv422_8_YUYV()")
            SAFE_DELETE_ARRAY(m_pCovertedImage)
        }

        uint8_t *CYuv422_8_YUYV::ToBGR8(uint8_t *pRawImage)
        {
            ConvertYUV422_YUYVtoBGR8(pRawImage, m_pCovertedImage, m_unWidth, m_unHeight );

            return m_pCovertedImage;
        }

        uint8_t *CYuv422_8_YUYV::ToRGB8(uint8_t *pRawImage)
        {
            ConvertYUV422_YUYVtoRGB8(pRawImage, m_pCovertedImage, m_unWidth, m_unHeight );

            return m_pCovertedImage;
        }

        bool CYuv422_8_YUYV::SaveImage()
        {
            DEBUG_LOG("Does not Implement.")
        }

        void CYuv422_8_YUYV::ConvertYUV422_YUYVtoBGR8(const BYTE *pbSrc, BYTE *pbDst, DWORD nWidth, DWORD nHeight)
        {
            unsigned int i;
            int Y1,Y2,U,V;

            for(i=0;i<nWidth * nHeight;i=i+2)
            {
                Y1=pbSrc[i*2];
                U=pbSrc[i*2+1];
                Y2=pbSrc[i*2+2];
                V=pbSrc[i*2+3];

                float fCalc;

                fCalc = (float)Y1+(1.402f*(float)(V-128));
                pbDst[i*3+2]= (unsigned char)( fCalc < 0 ? 0 : (fCalc>0xFF ? 0xFF : fCalc));

                fCalc = (float)Y1-(0.344f*(float)(U-128)+(0.714f*(float)(V-128)));
                pbDst[i*3+1]	=	(unsigned char)( fCalc < 0 ? 0 : (fCalc>0xFF ? 0xFF : fCalc));

                fCalc = (float)Y1+(1.772f*(float)(U-128));
                pbDst[i*3]		=	(unsigned char)( fCalc < 0 ? 0 : (fCalc>0xFF ? 0xFF : fCalc));

                fCalc = (float)Y2+(1.402f*(float)(V-128));
                pbDst[i*3+5]	=	(unsigned char)( fCalc < 0 ? 0 : (fCalc>0xFF ? 0xFF : fCalc));

                fCalc = (float)Y2-(0.344f*(float)(U-128)+(0.714f*(float)(V-128)));
                pbDst[i*3+4]	=	(unsigned char)( fCalc < 0 ? 0 : (fCalc>0xFF ? 0xFF : fCalc));

                fCalc = (float)Y2+(1.772f*(float)(U-128));
                pbDst[i*3+3]	=	(unsigned char)( fCalc < 0 ? 0 : (fCalc>0xFF ? 0xFF : fCalc));

            }
        }

        void CYuv422_8_YUYV::ConvertYUV422_YUYVtoRGB8(const BYTE *pbSrc, BYTE *pbDst, DWORD nWidth, DWORD nHeight)
        {
            unsigned int i;
            int Y1,Y2,U,V;

            for(i=0;i<nWidth * nHeight;i=i+2)
            {
                Y1=pbSrc[i*2];
                U=pbSrc[i*2+1];
                Y2=pbSrc[i*2+2];
                V=pbSrc[i*2+3];

                float fCalc;

                fCalc = (float)Y1+(1.402f*(float)(V-128));
                pbDst[i*3+0]= (unsigned char)( fCalc < 0 ? 0 : (fCalc>0xFF ? 0xFF : fCalc));

                fCalc = (float)Y1-(0.344f*(float)(U-128)+(0.714f*(float)(V-128)));
                pbDst[i*3+1]	=	(unsigned char)( fCalc < 0 ? 0 : (fCalc>0xFF ? 0xFF : fCalc));

                fCalc = (float)Y1+(1.772f*(float)(U-128));
                pbDst[i*3+2]		=	(unsigned char)( fCalc < 0 ? 0 : (fCalc>0xFF ? 0xFF : fCalc));

                fCalc = (float)Y2+(1.402f*(float)(V-128));
                pbDst[i*3+3]	=	(unsigned char)( fCalc < 0 ? 0 : (fCalc>0xFF ? 0xFF : fCalc));

                fCalc = (float)Y2-(0.344f*(float)(U-128)+(0.714f*(float)(V-128)));
                pbDst[i*3+4]	=	(unsigned char)( fCalc < 0 ? 0 : (fCalc>0xFF ? 0xFF : fCalc));

                fCalc = (float)Y2+(1.772f*(float)(U-128));
                pbDst[i*3+5]	=	(unsigned char)( fCalc < 0 ? 0 : (fCalc>0xFF ? 0xFF : fCalc));

            }
        }

    }//                 namespace Yuv

}//        namespace PixelFormat

