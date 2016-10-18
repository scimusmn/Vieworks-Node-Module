//#pragma once

#ifndef VWINTERFACE_H
#define VWINTERFACE_H

//#include <Client.h>


#include "VwGigE.Global.h"
#include "VwCamera.h"
#include <list>

using namespace GenICam::Client;
using namespace std;

// forward declaration
class VwCamera;
class VwGigE;


//+++++++++++++++++++++++++++++++++++++++++++++++
// class VwInterface
//+++++++++++++++++++++++++++++++++++++++++++++++
class CLASS_EXPORT VwInterface
{
	protected:
		TL_HANDLE	hTL;
		IF_HANDLE	hInterface;
		std::map<string, VwCamera*>	vwCameraList;
	
		char		caName[128];
		VwGigE*		m_pVwGigE;

		UINT		m_unFilterType;

	protected:
		VwInterface(void);

	public:
		VwInterface( TL_HANDLE _hTL, UINT unFilterType );
		~VwInterface(void);

	public:
		IF_HANDLE GetInterfaceHandle()	{	return hInterface;	}
		std::map<string, VwCamera*>* GetCameraList()	{	return &vwCameraList; }
		RESULT GetVwGigE( VwGigE* pVwGigE );
		RESULT OpenInterface(char* name, IN VwGigE* pVwGigE );
		RESULT CloseInterface();
		RESULT RemoveCameraList();
		RESULT GetNumCameras(UINT* pNumDevices);
		RESULT GetCameraName(UINT unDevIndex, char* pcaName,size_t* pcbNameSize);
		RESULT GetCameraID(UINT unDevIndex, char* pcaID,size_t* pcbNameSize);
		RESULT GetSubnet( char* pszSubnet, size_t* paSubnet);
		RESULT GetIPAddress( char* pszIPAddress, size_t* paIPAddress );

        RESULT OpenCamera(char* name,
                          VwCamera** ppVwCamera,
                          UINT nNumBuffer,
                          UINT nWidth,
                          UINT nHeight,
                          UINT nPacketSize,
                          void* pUserPointer,
                          ImageCallbackFn* pImageCallbackFn,
                          DisconnectCallbackFn* pDisconnectCallbackFn = NULL );


        RESULT OpenCamera(UINT nIndex,
                          VwCamera** ppVwCamera,
                          UINT nNumBuffer,
                          UINT nWidth,
                          UINT nHeight,
                          UINT nPacketSize,
                          void* pUserPointer,
                          ImageCallbackFn* pImageCallbackFn,
                          DisconnectCallbackFn* pDisconnectCallbackFn = NULL );


		RESULT GetInterfaceInfo( const char* sIfaceID, INTERFACE_INFO_CMD_LIST iInfoCmd, INFO_DATATYPE* piType, void* pBuffer, size_t* piSize );


		RESULT GetFilterType( UINT* unpFilterType );
		RESULT SetFilterType( UINT unFilterType );
};

#endif
