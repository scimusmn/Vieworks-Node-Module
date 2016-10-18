//#pragma once

#ifndef VWCAMERA_H
#define VWCAMERA_H

//#include <Client.h>

#include "GenICam.h"
#include "TLI/GenTL_v1_3.h"

#include "VwGigE.Global.h"
#include "VwInterface.h"
#include "VwSerial.h"
#include <string.h>

#include <semaphore.h>

#pragma warning (disable:4251)



using namespace std;
using namespace GenICam;
using namespace GenICam::Client;
using namespace GenApi;



class CLASS_LOCAL MyPort : public IPort
{
public:
    MyPort(void)  { m_dev_handle = NULL; }

    void SetDevHandle(DEV_HANDLE hDev)
    {
        m_dev_handle = hDev;
        DevGetPort(m_dev_handle, &m_port_handle);
    };

    virtual ~MyPort()
    {
        DevClose(m_dev_handle);
    };

    virtual void Read(void *pBuffer, int64_t Address, int64_t Length)
    {
        if( GC_ERR_SUCCESS != GCReadPort( m_port_handle, Address, pBuffer, (size_t*)&Length ) )
            throw ACCESS_EXCEPTION( "Failure reading device data" );
    };

    virtual void Write(const void *pBuffer, int64_t Address, int64_t Length)
    {
        if( GC_ERR_SUCCESS != GCWritePort( m_port_handle, Address, pBuffer, (size_t*)&Length ) )
            throw ACCESS_EXCEPTION( "Failure writing device data" );
    };

    virtual EAccessMode GetAccessMode() const
    {
        return RW;
    };

private:
    DEV_HANDLE		m_dev_handle;
    PORT_HANDLE     m_port_handle;
};



class VwInterface;

//+++++++++++++++++++++++++++++++++++++++++++++++
// class VwCamera
//+++++++++++++++++++++++++++++++++++++++++++++++
class CLASS_EXPORT VwCamera
{
protected:
	VwInterface*		pInterface;
	DEV_HANDLE			hCamera;
	DS_HANDLE			hStream;
	string				xml;						// XML red from the camera
	CNodeMapRef*		pNodeMap;					// Node map from XML to R/W camera parameters
	MyPort				port;

	VwSerial			m_Serial;

	BOOL				m_bGrabThdRunning;			// Flag : Grab thread On/Off

    sem_t               m_Sem[2];
    sem_t               m_SemGrabThread;

	EVENT_HANDLE		hEventNewBuffer;
	UINT				m_nCurBufIdx;				// Current Buffer Index	// m_curbufferindex;
	void**				m_ppGigaBuffer;
	ImageCallbackFn*	m_pImageCallbackFn;
	DisconnectCallbackFn*	m_pDisconnectCallbackFn;	// When the connection has been lost, this function will be called.
	void*				m_pUserPointer;				// for Callback Function

    //HANDLE				m_pThrGrab;
    pthread_t           m_ThrGrab;
    pthread_mutex_t     m_MutexGrab;


	list<BUFFER_HANDLE> m_lstBuffers;
	UINT				m_nBufferNum;
	UINT				m_nPaketsMissing;
	
	BOOL				m_bIsVXDevice;				// Flag : Vx Device 
	UINT				m_nEvtGetDataTimeOut;		// EventGetData() Timeout Value
	UINT				m_unFilterType;

    //HANDLE				m_pThrDisconnect;			// Check to connect condition.
    pthread_t*          m_pThrDisconnect;			// Check to connect condition.

	BOOL				m_bExit;
	GenICam::Client::EVENT_HANDLE m_hEventError;
	__int64				m_nFrameCount;				// Frame count

    PROPERTY_LIST		m_PropList;


protected:
	VwCamera(void);

	// protected method
protected:
	static RESULT CreateBuffer( IN const UINT nImageWidth, IN const UINT nImageHeight, IN const UINT nBitsPerPixel, IN const UINT nBufferNumber, 
		IN list<BUFFER_HANDLE>& lstBuffers, IN const DS_HANDLE hStream, OUT void*** pppGigaBuffer );
	RESULT DeleteBuffer();

	static RESULT RearrangeQueue( IN list<BUFFER_HANDLE>& lstBuffers, IN const DS_HANDLE hStream );

	static RESULT StartAcquisition( IN const DS_HANDLE hStream );

	static RESULT SetRegisterEvent( IN const DS_HANDLE hStream, IN HANDLE* m_hEvtGigaLicx, OUT EVENT_HANDLE m_hEventNewBuffer );

	RESULT GetPixelFormatFromString( std::string strPixelFormat, PIXEL_FORMAT* pPixelFormat );
	RESULT GetPixelFormatName( IN PIXEL_FORMAT pixelFormat, std::string* strPixelFormat );

	RESULT IsValidPixelSize( IN UINT nPixelSize, OUT BOOL* pbValid );

	BOOL GetVxDevice();
	void SetVxDevice( BOOL bVal );

    void InitGrabThread();
    void UninitGrabThread();
    BOOL IsGrabRunning();

    void LoadPropertyList(  );
    void RetrieveNode(INode* pNode);


protected:
#ifdef USE_TL_13
#else
	BOOL LockCheckHeartBeatError();
	BOOL UnlockCheckHeartBeatError();
#endif

#ifdef USE_TL_13
	UINT ThrGrabContinuosOpt();
    UINT ThrGrabContinuosNotUseEvent();
#endif
	UINT ThrGrabContinuos();
	UINT ErrorCheckThread();

    static LPVOID myGrabContinuosThread (LPVOID me);
    static LPVOID thCheckConnection (LPVOID me);

public:
	RESULT GetPixelFormatEntry( OUT list<std::string>* plstPixelFormat );
	RESULT ChangeBufferFormat( IN UINT nBufferNum, IN UINT nWidth, IN UINT nHeight, IN PIXEL_FORMAT pixelFormat );

public:
	VwCamera(VwInterface* _pInterface, UINT _unFilterType);
	~VwCamera(void);

	RESULT GetVwInterface( OUT VwInterface** pVwInterface );

	RESULT GetDeviceHandle( OUT DEV_HANDLE* pDevHandle );

    RESULT OpenCamera(char* name,
                      UINT nNumBuffer,
                      UINT nWidth,
                      UINT nHeight,
                      UINT nPacketSize,
                      void* pUserPointer,
                      ImageCallbackFn* pImageCallbackFn,
                      DisconnectCallbackFn* pDisconnectCallbackFn = NULL);

	RESULT CloseCamera();

    string GetXML( void* hPort, BOOL bIsDevice);

	RESULT Snap(UINT nFrameCount);
	RESULT Grab();
	RESULT Abort();
    RESULT GetGrabCondition( BOOL& bIsGrabbing )		{	bIsGrabbing = m_bGrabThdRunning; return RESULT_SUCCESS; }

	RESULT GetWidth(UINT* pWidth);
	RESULT GetHeight(UINT* pHeight);
	RESULT SetWidth(UINT width);
	RESULT SetHeight(UINT height);

	RESULT SetPixelFormat(PIXEL_FORMAT format);
	RESULT GetPixelFormat( PIXEL_FORMAT* pPixelFormat );
	RESULT GetPixelFormatLineup( IN UINT nIndex, PIXEL_FORMAT* pPixelFormat );
	RESULT GetPixelFormatLineupNum( OUT UINT* pnIndex );

	RESULT SetPixelSize( UINT pnPixelSize );
	RESULT GetPixelSize( UINT* pnPixelSize );
	RESULT GetPixelSizeType( IN UINT nIndex, OUT UINT* pnPixelSize );
	RESULT GetPixelSizeTypeNum( OUT UINT* pnNum );

	RESULT SetTriggerMode( BOOL bTriggerMode );
	RESULT GetTriggerMode( BOOL* pbTriggerMode );
	RESULT GetTriggerModeLineup( IN UINT nIndex, OUT UINT* unValue );
	RESULT GetTriggerModeLineupNum( OUT UINT* pnIndex );

	RESULT GetTriggerSource( TRIGGER_SOURCE* pTriggerSoruce );
	RESULT GetTriggerSourceLineup( IN UINT nIndex, OUT TRIGGER_SOURCE* pTriggerSoruce );
	RESULT GetTriggerSourceLineupNum( OUT UINT* pnIndex );

	RESULT SetBinningHorizontal( UINT pnBinningHorizontal );
	RESULT GetBinningHorizontal( UINT* pnBinningHorizontal );
	RESULT GetBinningHorizontalLineup( IN UINT nIndex, OUT UINT* unValue );
	RESULT GetBinningHorizontalLineupNum( OUT UINT* pnIndex );

	RESULT SetBinningVertical( UINT pnBinningVertical );
	RESULT GetBinningVertical( UINT* pnBinningVertical );
	RESULT GetBinningVerticalLineup( IN UINT nIndex, OUT UINT* unValue );
	RESULT GetBinningVerticalLineupNum( OUT UINT* pnIndex );

	RESULT GetBinningFactor( UINT* pnFactor );
	RESULT SetBinningFactor( UINT nFactor );

	RESULT SetTriggerSoftware();
	RESULT GetFrameCapacity( UINT* pnFrameCapacity );
	
	RESULT SetStreamHold( UINT pnStreamHold );
	RESULT GetStreamHold( UINT* pnStreamHold );

	RESULT SetSCPSPacketSize( UINT pnSCPSPacketSize );
	RESULT GetSCPSPacketSize( UINT* pnSCPSPacketSize );

	RESULT SetTestImage( TESTIMAGE aTestImage );
	RESULT GetTestImage( TESTIMAGE* pTestImage );
	RESULT GetTestImageLineup( IN UINT nIndex, OUT TESTIMAGE* pTestImage );
	RESULT GetTestImageLineupNum( OUT UINT* pnIndex );

	RESULT SetAcquisitionTimeOut( IN UINT nTimeOut );
	RESULT GetAcquisitionTimeOut( OUT UINT* pnTimeOut );

	static RESULT GetDeviceModelName( IN DEV_HANDLE hCamera, OUT  char* pName, OUT size_t* pNameSize );
	RESULT GetDeviceModelName( IN int nIndex, OUT  char* pName, OUT size_t* pNameSize );
	RESULT GetDeviceVersion( IN int nIndex, char* pVersion, size_t* pVersionSize );
	static RESULT GetDeviceVendorName( IN DEV_HANDLE hCamera, OUT char* pInfo, OUT size_t* pInfoSize );
	RESULT GetDeviceVendorName( IN int nIndex, OUT char* pInfo, OUT size_t* pInfoSize );
	RESULT GetDeviceManufacturerInfo( char* pInfo, size_t* pInfoSize );
	RESULT GetDeviceID( IN int nIndex, char* pInfo, size_t* pInfoSize );
	static RESULT GetDeviceIP( IN DEV_HANDLE hCamera, OUT char* pInfo, OUT size_t* pInfoSize );
	RESULT GetDeviceIP( IN int nIndex, OUT char* pInfo, OUT size_t* pInfoSize );
	static RESULT GetDeviceMAC( IN DEV_HANDLE hCamera, OUT char* pInfo, OUT size_t* pInfoSize );
	RESULT GetDeviceMAC( IN int nIndex, OUT char* pInfo, OUT size_t* pInfoSize );

	RESULT GetBufferInfo( OUT UINT& nBufferNum, OUT UINT& nWidth, OUT UINT& nHeight, OUT PIXEL_FORMAT& pixelFormat );

	RESULT GetStreamInfo( IN STREAM_INFO streamInfo, OUT UINT& nInfo );

	RESULT SetReadoutMode( READOUT aReadout );
	RESULT GetReadoutMode( READOUT* paReadout );
	RESULT SetHorizontalStart(UINT uStart);
	RESULT GetHorizontalStart(UINT* puStart);
	RESULT SetHorizontalEnd(UINT uEnd);
	RESULT GetHorizontalEnd(UINT* puEnd);
	RESULT SetVerticalStart(UINT uStart);
	RESULT GetVerticalStart(UINT* puStart);
	RESULT SetVerticalEnd(UINT uEnd);
	RESULT GetVerticalEnd(UINT* puEnd);

	RESULT SetTriggerSource(TRIGGER_SOURCE aSource);
	RESULT SetTriggerActivation(TRIGGER_ACTIVATION aActivation);
	RESULT GetExposureMode(EXPOSURE_MODE* paExpmode);
	RESULT SetExposureMode(EXPOSURE_MODE aExpmode);
	RESULT GetExposureTime(UINT* aExptime_microsec);
	RESULT SetExposureTime(UINT aExptime_microsec);
	RESULT GetGain(GAIN_SEL* aGainSel, float* fGainVal);
	RESULT SetGain(GAIN_SEL aGainSel, float fGainVal);
	RESULT SetBlackLevel(BLACKLEVEL_SEL blackLevelSel, UINT aBlacklevelVal);
	RESULT SetReverseX(BOOL aBSet);
	RESULT SetStrobeOffset(UINT aOffsetVal);
	RESULT SetStrobePolarity(STROBE_POLARITY aStrobePolarity);	

	RESULT GigEGetCurrentIpConfigurationDHCP( bool* pbSet );
	RESULT GigEGetCurrentIpConfigurationPersistentIP( bool* pbSet );
	RESULT GigEGetPersistentSubnetMask( UINT* pnSubnetMask );
	RESULT GigEGetCurrentMACAddress( char* pszMACAddress, size_t* pNameSize );

	RESULT GigESetCurrentIpConfigurationDHCP( bool bSet );
	RESULT GigESetCurrentIpConfigurationPersistentIP( bool bSet );

	RESULT GigEGetColorRGBGain( int nRGBType, double* dpGainValue );
	RESULT GigESetColorRGBGain( int nRGBType, double dGainValue );

	RESULT GigEGetTemperature( double* dpTemperature );
	RESULT GigEGetSerialNumber( char* capSerialNumber, size_t* cbpSerialNumberSize );
	RESULT GigEGetSerialNumber( char capSerialNumber[], size_t& cbpSerialNumberSize );
	RESULT ReadData( void *pBuffer, int64_t Address, int64_t* Length );
	RESULT WriteData( void *pBuffer, int64_t Address, size_t* Length );

	void SetFilterType( UINT unFilterType );
	BOOL GetFilterType();

	RESULT SetCustomCommand( IN char* pCommand, IN char* pArg );

	RESULT GetCustomCommand( IN char* cpCommand, OUT char* cpArg, INOUT size_t* cbpArgSize, IN int nCmdType = GET_CUSTOM_COMMAND_VALUE );


	RESULT SetUARTCustomCommand( IN char* pCommand );
	RESULT GetUARTCustomCommand( IN char* pCommand, OUT char* pArg, IN size_t* pArgSize );

    RESULT GetPropertyCount(OUT int* pCount);
    RESULT GetPropertyInfo(IN int nIndex, OUT PROPERTY* pPropInfo);
    RESULT GetPropertyInfo(IN const char* cpCommand, OUT PROPERTY* pPropInfo);

    RESULT GetPollingTime( INode* pNode, __int64& nPollingTime );

//	RESULT GetHeartBeatTimeoutTryCount( OUT int& nCount );
//	RESULT SetHeartBeatTimeoutTryCount( IN int nCount );

	void ftoa( double dInput, char* chRet, int digit );

    char* strlower(char* s);
    unsigned int GetTickCount();
    void decompressionXML(char *pSource, int nSize, string &xmlDest);
};

#endif
