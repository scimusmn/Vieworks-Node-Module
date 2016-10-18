
//#pragma once
#ifndef VWGIGE_API_H
#define VWGIGE_API_H

#define IN
#define OUT
#define INOUT

#include "VwGigE.Error.h"
#include "VwGigE.Global.h"
#include <stddef.h>

 
#ifdef __cplusplus
extern "C"
{
#endif

	typedef void* VWGIGE_HANDLE;
	typedef void* HINTERFACE;
	typedef void* HCAMERA;

    FUNCTION_EXPORT RESULT OpenVwGigE(VWGIGE_HANDLE* hVwGigE);
    FUNCTION_EXPORT RESULT CloseVwGigE(VWGIGE_HANDLE hVwGigE);
    FUNCTION_EXPORT RESULT VwGetNumInterfaces(VWGIGE_HANDLE hVwGigE, UINT* pNumInterfaces);
    FUNCTION_EXPORT RESULT VwGetNumCameras(VWGIGE_HANDLE hVwGigE, UINT* aPNumCamera);

    FUNCTION_EXPORT RESULT VwOpenCameraByIndex(VWGIGE_HANDLE hVwGigE,
                                               UINT nCameraIndex,
                                               HCAMERA* phCamera,
                                               UINT nNumBuffer,
                                               UINT nWidth,
                                               UINT nHeight,
                                               UINT nPacketSize,
                                               void* pUserPointer,
                                               ImageCallbackFn* pImageCallbackFn,
                                               DisconnectCallbackFn* pDisconnectCallbackFn = NULL);

    FUNCTION_EXPORT RESULT VwOpenCameraByName(VWGIGE_HANDLE hVwGigE,
                                              char* pCameraName,
                                              HCAMERA* phCamera,
                                              UINT nNumBuffer,
                                              UINT nWidth,
                                              UINT nHeight,
                                              UINT nPacketSize,
                                              void* pUserPointer,
                                              ImageCallbackFn* pImageCallbackFn,
                                              DisconnectCallbackFn* pDisconnectCallbackFn = NULL);


    FUNCTION_EXPORT RESULT VwOpenInterfaceByIndex(VWGIGE_HANDLE hVwGigE, UINT aNIndex, HINTERFACE* phInterface);
    FUNCTION_EXPORT RESULT VwOpenInterfaceByName(VWGIGE_HANDLE hVwGigE, char* pInterfaceName, HINTERFACE* phInterface);
    FUNCTION_EXPORT RESULT VwDiscovery(VWGIGE_HANDLE hVwGigE);

    FUNCTION_EXPORT RESULT VwDiscoveryInterfaceInfo(VWGIGE_HANDLE hVwGigE,UINT nIndex,INTERFACE_INFO_STRUCT* pInterfaceInfoStruct);
    FUNCTION_EXPORT RESULT VwDiscoveryCameraInfo(VWGIGE_HANDLE hVwGigE,UINT nIndex,CAMERA_INFO_STRUCT* pCameraInfoStruct);

    //Does not implement.
    //FUNCTION_EXPORT RESULT VwForceIP(VWGIGE_HANDLE hVwGigE, PBYTE pMAC, DWORD nIP, DWORD nSubnet, DWORD nGateway);

    //FUNCTION_EXPORT RESULT VwUseMTUOptimize( IN VWGIGE_HANDLE hVwGigE, IN bool bUse );
    //FUNCTION_EXPORT RESULT VwSetMTUTimeout( IN VWGIGE_HANDLE hVwGigE, IN UINT uTimeout );
    //FUNCTION_EXPORT RESULT VwGetMTUTimeout( IN VWGIGE_HANDLE hVwGigE, OUT UINT& uTimeout );
    //Does not implement.
	
    FUNCTION_EXPORT RESULT InterfaceOpenCameraByIndex(IN HINTERFACE hInstance,IN void* pCallbackParent,IN UINT nDevIndex,HCAMERA* phCamera, UINT nNumBuffer,
		UINT nWidth, UINT nHeight, PIXEL_FORMAT pixelFormat, ImageCallbackFn* pImageCallbackFn, DisconnectCallbackFn* pDisconnectCallbackFn = NULL);
    FUNCTION_EXPORT RESULT InterfaceOpenCameraByName(HINTERFACE hInterface, void* pParent, char* pName,HCAMERA* phCamera, UINT nNumBuffer,
		UINT nWidth, UINT nHeight, PIXEL_FORMAT pixelFormat, ImageCallbackFn* pImageCallbackFn, DisconnectCallbackFn* pDisconnectCallbackFn = NULL);
    FUNCTION_EXPORT RESULT InterfaceGetNumCameras(HINTERFACE hInterface, UINT* aPNumDevices);
    FUNCTION_EXPORT RESULT InterfaceGetCameraName(HINTERFACE hInterface, UINT aNDevIndex, char* aName, size_t* aPNameSize);
    FUNCTION_EXPORT RESULT InterfaceGetIPAddress(HINTERFACE hInterface, char* pszIPAddress, size_t* paIPAddress);
    FUNCTION_EXPORT RESULT InterfaceGetSubnet(HINTERFACE hInterface, char* pszSubnet, size_t* paSubnet);
    FUNCTION_EXPORT RESULT InterfaceCloseInterface(HINTERFACE hInterface);

    FUNCTION_EXPORT RESULT InterfaceGetVwGigEHandle(IN HINTERFACE hInterface,OUT VWGIGE_HANDLE* phVwGigEHandle);

    FUNCTION_EXPORT RESULT CameraClose(HCAMERA hCamera);
    FUNCTION_EXPORT RESULT CameraGrab(HCAMERA hCamera);
    FUNCTION_EXPORT RESULT CameraSnap(HCAMERA hCamera, UINT unFrame);
    FUNCTION_EXPORT RESULT CameraAbort(HCAMERA hCamera);
    FUNCTION_EXPORT RESULT CameraGetGrabCondition(HCAMERA hCamera, BOOL& bIsGrabbing);
    FUNCTION_EXPORT RESULT CameraGetWidth(HCAMERA hCamera, UINT* punWidth);
    FUNCTION_EXPORT RESULT CameraGetHeight(HCAMERA hCamera, UINT* punHeight);
    FUNCTION_EXPORT RESULT CameraSetWidth(HCAMERA hCamera, UINT pnWidth);
    FUNCTION_EXPORT RESULT CameraSetHeight(HCAMERA hCamera, UINT pnHeight);
    FUNCTION_EXPORT RESULT CameraSetPixelFormat(HCAMERA hCamera, PIXEL_FORMAT pixelFormat);
    FUNCTION_EXPORT RESULT CameraGetPixelFormat(HCAMERA hCamera, PIXEL_FORMAT* pPixelFormat);
    FUNCTION_EXPORT RESULT CameraGetPixelFormatLineup(HCAMERA hCamera, UINT nIndex, PIXEL_FORMAT* pPixelFormat);
    FUNCTION_EXPORT RESULT CameraGetPixelFormatLineupNum(HCAMERA hCamera, UINT* npNum);

    FUNCTION_EXPORT RESULT CameraSetPixelSize(HCAMERA hCamera, UINT nPixelSize);
    FUNCTION_EXPORT RESULT CameraGetPixelSize(HCAMERA hCamera, UINT* pnPixelSize);
    FUNCTION_EXPORT RESULT CameraGetPixelSizeLineup(HCAMERA hCamera, UINT nIndex, UINT* pnPixelSize);
    FUNCTION_EXPORT RESULT CameraGetPixelSizeLineupNum(HCAMERA hCamera, UINT* npNum);

    FUNCTION_EXPORT RESULT CameraSetTriggerMode(IN HCAMERA hCamera, BOOL bSet);
    FUNCTION_EXPORT RESULT CameraGetTriggerMode(HCAMERA hCamera, BOOL* pbSet);
    FUNCTION_EXPORT RESULT CameraGetTriggerModeLineup(HCAMERA hCamera, UINT nIndex, UINT* nTriggerMode);
    FUNCTION_EXPORT RESULT CameraGetTriggerModeLineupNum(HCAMERA hCamera, UINT* npNum);

    FUNCTION_EXPORT RESULT CameraSetTriggerSource(IN HCAMERA hCamera, IN TRIGGER_SOURCE triggerSource);
    FUNCTION_EXPORT RESULT CameraGetTriggerSource(IN HCAMERA hCamera, OUT TRIGGER_SOURCE* pTriggerSource);
    FUNCTION_EXPORT RESULT CameraGetTriggerSourceLineup(HCAMERA hCamera, UINT nIndex, TRIGGER_SOURCE* pTriggerSoruce);
    FUNCTION_EXPORT RESULT CameraGetTriggerSourceLineupNum(HCAMERA hCamera, UINT* npNum);

    FUNCTION_EXPORT RESULT CameraSetBinningHorizontal(HCAMERA hCamera, UINT nBinningHorizontal);
    FUNCTION_EXPORT RESULT CameraGetBinningHorizontal(HCAMERA hCamera, UINT* pnBinningHorizontal);
    FUNCTION_EXPORT RESULT CameraGetBinningHorizontalLineup(HCAMERA hCamera, UINT nIndex, UINT* pnBinningHorizontal);
    FUNCTION_EXPORT RESULT CameraGetBinningHorizontalLineupNum(HCAMERA hCamera, UINT* npNum);

    FUNCTION_EXPORT RESULT CameraSetBinningVertical(HCAMERA hCamera, UINT nBinningVertical);
    FUNCTION_EXPORT RESULT CameraGetBinningVertical(HCAMERA hCamera, UINT* nBinningVertical);
    FUNCTION_EXPORT RESULT CameraGetBinningVerticalLineup(HCAMERA hCamera, UINT nIndex, UINT* nBinningVertical);
    FUNCTION_EXPORT RESULT CameraGetBinningVerticalLineupNum(HCAMERA hCamera, UINT* npNum);

    FUNCTION_EXPORT RESULT CameraGetBinningFactor( HCAMERA hCamera, UINT* pnFactor );
    FUNCTION_EXPORT RESULT CameraSetBinningFactor( HCAMERA hCamera, UINT nFactor );

    FUNCTION_EXPORT RESULT CameraSetHorizontalStart(HCAMERA hCamera, UINT uStart);
    FUNCTION_EXPORT RESULT CameraGetHorizontalStart(HCAMERA hCamera, UINT* puStart);
    FUNCTION_EXPORT RESULT CameraSetHorizontalEnd(HCAMERA hCamera, UINT uEnd);
    FUNCTION_EXPORT RESULT CameraGetHorizontalEnd(HCAMERA hCamera, UINT* puEnd);
    FUNCTION_EXPORT RESULT CameraSetVerticalStart(HCAMERA hCamera, UINT uStart);
    FUNCTION_EXPORT RESULT CameraGetVerticalStart(HCAMERA hCamera, UINT* puStart);
    FUNCTION_EXPORT RESULT CameraSetVerticalEnd(HCAMERA hCamera, UINT uEnd);
    FUNCTION_EXPORT RESULT CameraGetVerticalEnd(HCAMERA hCamera, UINT* puEnd);

    FUNCTION_EXPORT RESULT CameraGetFrameCapacity(HCAMERA hCamera, UINT* nFrameCapacity);

    FUNCTION_EXPORT RESULT CameraSetStreamHold(HCAMERA hCamera, UINT nStreamHold);
    FUNCTION_EXPORT RESULT CameraGetStreamHold(HCAMERA hCamera, UINT* nStreamHold);

    FUNCTION_EXPORT RESULT CameraSetSCPSPacketSize(HCAMERA hCamera, UINT nSCPSPacketSize);
    FUNCTION_EXPORT RESULT CameraGetSCPSPacketSize(HCAMERA hCamera, UINT* nSCPSPacketSize);

    FUNCTION_EXPORT RESULT CameraSetTestImage(HCAMERA hCamera, TESTIMAGE aTestImage);
    FUNCTION_EXPORT RESULT CameraGetTestImage(HCAMERA hCamera, TESTIMAGE* pTestImage);
    FUNCTION_EXPORT RESULT CameraGetTestImageLineup(HCAMERA hCamera, UINT nIndex, TESTIMAGE* pTestImage);
    FUNCTION_EXPORT RESULT CameraGetTestImageLineupNum(HCAMERA hCamera, UINT* npNum);

    FUNCTION_EXPORT RESULT CameraSetAcquisitionTimeOut(HCAMERA hCamera, UINT nTimeOut);
    FUNCTION_EXPORT RESULT CameraGetAcquisitionTimeOut(HCAMERA hCamera, UINT* pnTimeOut);

    FUNCTION_EXPORT RESULT CameraSetReadoutMode(HCAMERA hCamera, READOUT aReadout);
    FUNCTION_EXPORT RESULT CameraGetDeviceModelName(IN HCAMERA hCamera, IN int nIndex, OUT char* pInfo, INOUT size_t* pInfoSize);
    FUNCTION_EXPORT RESULT CameraGetDeviceVersion(IN HCAMERA hCamera, IN int nIndex, OUT char* pInfo, INOUT size_t* pInfoSize);
    FUNCTION_EXPORT RESULT CameraGetDeviceVendorName(IN HCAMERA hCamera, IN int nIndex, OUT char* pInfo, INOUT size_t* pInfoSize);
    FUNCTION_EXPORT RESULT CameraGetDeviceManufacturerInfo(IN HCAMERA hCamera, OUT char* pInfo, INOUT size_t* pInfoSize);
    FUNCTION_EXPORT RESULT CameraGetDeviceID(IN HCAMERA hCamera, IN int nIndex, OUT char* pInfo, INOUT size_t* pInfoSize);
    FUNCTION_EXPORT RESULT CameraSetTriggerMode(IN HCAMERA hCamera, BOOL bSet);
    FUNCTION_EXPORT RESULT CameraSetTriggerSource(IN HCAMERA hCamera, IN TRIGGER_SOURCE triggerSource);
    FUNCTION_EXPORT RESULT CameraSetTriggerActivation(IN HCAMERA hCamera, IN TRIGGER_ACTIVATION triggerActivation);
    FUNCTION_EXPORT RESULT CameraSetTriggerSoftware(IN HCAMERA hCamera);
    FUNCTION_EXPORT RESULT CameraGetExposureMode(IN HCAMERA hCamera, OUT EXPOSURE_MODE* pexpMode);
    FUNCTION_EXPORT RESULT CameraSetExposureMode(IN HCAMERA hCamera, EXPOSURE_MODE aExpmode);
    FUNCTION_EXPORT RESULT CameraGetExposureTime(IN HCAMERA hCamera, UINT* paExptime_microsec);
    FUNCTION_EXPORT RESULT CameraSetExposureTime(IN HCAMERA hCamera, UINT aExptime_microsec);
    FUNCTION_EXPORT RESULT CameraGetGain(IN HCAMERA hCamera, GAIN_SEL* pgainSel, float* pfGainValue);
    FUNCTION_EXPORT RESULT CameraSetGain(IN HCAMERA hCamera, GAIN_SEL pgainSel, float fGainValue);
    FUNCTION_EXPORT RESULT CameraSetBlackLevel(IN HCAMERA hCamera, BLACKLEVEL_SEL blackLevelSel, UINT aBlacklevelVal);
    FUNCTION_EXPORT RESULT CameraSetReverseX(IN HCAMERA hCamera, BOOL aBSet);
    FUNCTION_EXPORT RESULT CameraSetStrobeOffset(IN HCAMERA hCamera, UINT nOffset);
    FUNCTION_EXPORT RESULT CameraSetStrobePolarity(IN HCAMERA hCamera, STROBE_POLARITY aStrobePolarity);

    //Does not implement.
    //FUNCTION_EXPORT RESULT CameraSetForceIP( IN HCAMERA hCamera, IN DWORD dwIP, IN DWORD dwSubnet, IN DWORD dwGateway );
    //Does not implement.

    FUNCTION_EXPORT RESULT CameraChangeBufferFormat( HCAMERA hCamera, UINT nBufferNum, UINT nWidth, UINT nHeight, PIXEL_FORMAT pixelFormat );
    FUNCTION_EXPORT RESULT CameraGetBufferInfo( IN HCAMERA hCamera, OUT UINT& nBufferNum, OUT UINT& nWidth, OUT UINT& nHeight, OUT PIXEL_FORMAT& pixelFormat );
    FUNCTION_EXPORT RESULT CameraGetInterfaceHandle(IN HCAMERA hCamera,OUT HINTERFACE** phInterface);

    FUNCTION_EXPORT RESULT CameraGigESetCurrentIpConfigurationDHCP(IN HCAMERA hCamera, bool bSet );
    FUNCTION_EXPORT RESULT CameraGigESetCurrentIpConfigurationPersistentIP( IN HCAMERA hCamera, bool bSet );

    FUNCTION_EXPORT RESULT CameraGigEGetCurrentIpConfigurationDHCP( IN HCAMERA hCamera, bool* pbSet );
    FUNCTION_EXPORT RESULT CameraGigEGetCurrentIpConfigurationPersistentIP( IN HCAMERA hCamera, bool* pbSet );

    FUNCTION_EXPORT RESULT CameraGigEGetPersistentSubnetMask( IN HCAMERA hCamera, OUT UINT* pnSubnetMask );
    FUNCTION_EXPORT RESULT CameraGigEGetCurrentMACAddress( IN HCAMERA hCamera, IN size_t* pNameSize, OUT char* pszMACAddress );

    FUNCTION_EXPORT RESULT CameraGigEGetColorRGBGain( IN HCAMERA hCamera, IN int nRGBType, OUT double* dpRGBGainValue );
    FUNCTION_EXPORT RESULT CameraGigESetColorRGBGain( IN HCAMERA hCamera, IN int nRGBType, IN double dRGBGainValue );

    FUNCTION_EXPORT RESULT CameraGetStreamInfo( IN HCAMERA hCamera, IN STREAM_INFO streamInfo, OUT UINT& nInfo );

    FUNCTION_EXPORT RESULT CameraGigEGetTemperature( IN HCAMERA hCamera, OUT double* dpTemperature );
    FUNCTION_EXPORT RESULT CameraGigEGetSerialNumber( IN HCAMERA hCamera, char capSerialNumber[], size_t& cbpSerialNumberSize, int nTemp = NULL );

    FUNCTION_EXPORT RESULT CameraSetCustomCommand( IN HCAMERA hCamera, IN char* pCommand, IN char* pArg );
    FUNCTION_EXPORT RESULT CameraGetCustomCommand( IN HCAMERA hCamera, IN char* pCommand, OUT char* pArg, INOUT size_t* pArgSize, IN int nCmdType = GET_CUSTOM_COMMAND_VALUE );



    FUNCTION_EXPORT RESULT CameraGetPropertyCount( IN HCAMERA hCamera, OUT int* pnCount );
    FUNCTION_EXPORT RESULT CameraGetPropertyInfo( IN HCAMERA hCamera, IN const char* cpCommand, OUT PROPERTY* ptPropInfo );
    FUNCTION_EXPORT RESULT CameraGetPropertyInfoUsingIndex( IN HCAMERA hCamera, IN int nIndex, OUT PROPERTY* ptPropInfo );

    FUNCTION_EXPORT RESULT CameraSetUARTCustomCommand( IN HCAMERA hCamera, IN char* pCommand );
    FUNCTION_EXPORT RESULT CameraGetUARTCustomCommand( IN HCAMERA hCamera, IN char* pCommand, OUT char* pArg, IN size_t* pArgSize );

    //Does not implement.
//    FUNCTION_EXPORT RESULT CameraReadData( IN HCAMERA hCamera, void *pBuffer, __int64 Address, __int64* Length );
//    FUNCTION_EXPORT RESULT CameraWriteData( IN HCAMERA hCamera, void *pBuffer, __int64 Address, size_t* Length );

//    FUNCTION_EXPORT RESULT CameraSetHeartBeatTimeoutTryCount( IN HCAMERA hCamera, IN int nCount );
//    FUNCTION_EXPORT RESULT CameraGetHeartBeatTimeoutTryCount( IN HCAMERA hCamera, OUT int& nCount );
//    FUNCTION_EXPORT RESULT CameraSetEventControl( IN HCAMERA hCamera, IN const char* pcaEventName, IN size_t szEventName, EventControlCallbackFn* pEventCallback );
    //Does not implement.

	void Convert8a_4b_4a_8bTo16_16_MMX_I(BYTE* Src,WORD* Dest,int LenSrcBytes);
    FUNCTION_EXPORT void ConvertYUV422_UYVYtoBGR8( const BYTE* pbSrc,  const unsigned int nWidth, const unsigned int nHeight, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertYUV422_YUYVtoBGR8( const BYTE* pbSrc, const unsigned int nWidth, const unsigned int nHeight, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertYUV422toBGR8( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertYUV422toBGR8Interlaced( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst, bool bOdd, DWORD width, bool blend, bool _signed );
    FUNCTION_EXPORT void ConvertYUV422PackedtoBGR8( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertYUV411toBGR8( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertYUV411PackedtoBGR8( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertRGB12PackedtoBGR8( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertRGB8toBGR8( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertBGR10V2PackedtoBGR8( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertYUV444toBGR8( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertMonoPackedToBGR8( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertMono16PackedToBGR8( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertBAYGB8ToBGR8(const BYTE* pbSrc, BYTE* pbDst, DWORD width, DWORD height);
    FUNCTION_EXPORT void ConvertBAYRG8ToBGR8(const BYTE* pbSrc, BYTE* pbDst, DWORD width, DWORD height);
    FUNCTION_EXPORT void ConvertBAYGR8ToBGR8(const BYTE* pbSrc, BYTE* pbDst, DWORD width, DWORD height);
    FUNCTION_EXPORT void ConvertBAYGB10ToBGR8(const WORD* pbSrc, BYTE* pbDst, DWORD width, DWORD height);
    FUNCTION_EXPORT void ConvertBAYRG10ToBGR8(const WORD* pbSrc, BYTE* pbDst, DWORD width, DWORD height);
    FUNCTION_EXPORT void ConvertBAYGR10ToBGR8(const WORD* pbSrc, BYTE* pbDst, DWORD width, DWORD height);
    FUNCTION_EXPORT void ConvertBAYGB12ToBGR8(const WORD* pbSrc, BYTE* pbDst, DWORD width, DWORD height);
    FUNCTION_EXPORT void ConvertBAYRG12ToBGR8(const WORD* pbSrc, BYTE* pbDst, DWORD width, DWORD height);
    FUNCTION_EXPORT void ConvertBAYGR12ToBGR8(const WORD* pbSrc, BYTE* pbDst, DWORD width, DWORD height);
    FUNCTION_EXPORT void ConvertMono10PackedToMono16bit( const BYTE* pbSrc, UINT widht, UINT height, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertMono12PackedToMono16bit( const BYTE* pbSrc, UINT widht, UINT height, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertMono10ToBGR8( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst );
    FUNCTION_EXPORT void ConvertMono12ToBGR8( const BYTE* pbSrc, DWORD cbSrc, BYTE* pbDst );


#ifdef __cplusplus
}
#endif

#endif
