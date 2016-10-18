
#include "VwGigE.Global.h"
#include "TLI/GenTL_v1_3.h"

#include <list>
#include <map>
#include <string>

#include <string.h>
#include <stdlib.h>

using namespace std;
using namespace GenICam;
using namespace GenICam::Client;



class VwInterface;
class VwCamera;

class CLASS_LOCAL VWCAMERA_INFO
{
public:
	BOOL				error;
	RESULT				errorResult;
	UINT				index;
	// CAMERA_HANDLE	hCam;
	char				name[256];
	char				vendor[128];
	char				model[128];
	char				ip[32];
	char				mac[32];
	
public:
	VWCAMERA_INFO()
	{
		error = FALSE;
		index	= 0;
		// hCam = NULL;
		memset(name,0,sizeof(name));
		memset(vendor,0,sizeof(vendor));
		memset(model,0,sizeof(model));
	}
};

class CLASS_EXPORT VWINTERFACE_INFO
{
public:
	BOOL				error;
	RESULT				errorCause;
	UINT				index;
	char				name[256];
	list<VWCAMERA_INFO*>	cameraInfoList;
public:
	VWINTERFACE_INFO()
	{
		error			= FALSE;
		index			= 0;
		memset(name,0,sizeof(name));
		cameraInfoList.clear();
	}
	~VWINTERFACE_INFO()
	{
 		for(list<VWCAMERA_INFO*>::iterator iterCam = cameraInfoList.begin(); iterCam != cameraInfoList.end(); )
 		{
 			if ( (*iterCam) != NULL )
 			{
 				delete (*iterCam);
				(*iterCam) = NULL;
                //iterCam = cameraInfoList.erase( iterCam );
                cameraInfoList.erase( iterCam++ );
 			}
 			else
 			{
 				iterCam ++;
 			}
 		}
	}

	void AddCameraInfo(VWCAMERA_INFO* pCameraInfo)
	{
		cameraInfoList.push_back(pCameraInfo);
	}
};


class CLASS_LOCAL DISCOVERY_INFO
{
public:
	DISCOVERY_INFO()
	{

	}
	~DISCOVERY_INFO()
	{
		int nTest = 0;
		nTest ++;
	}
public:
	list<VWINTERFACE_INFO*>	interfaceInfoList;
public:
	VWINTERFACE_INFO*	GetInterfaceInfo(UINT nIndex)
	{
		UINT ifCount = 0;
		list<VWINTERFACE_INFO*>::iterator ifInfoIter;
		for(ifInfoIter = interfaceInfoList.begin(); ifInfoIter != interfaceInfoList.end(); ifInfoIter++)
		{
			if(nIndex == ifCount)
			{
				VWINTERFACE_INFO** pInterfaceInfo = (&(*ifInfoIter));
				return *pInterfaceInfo;
			}
			ifCount ++;
		}
		return NULL;
	}
	UINT GetNumInterfaces()
	{
		return (UINT)interfaceInfoList.size();
	}

	void Clear()
	{
		for(list<VWINTERFACE_INFO*>::iterator iterDis = interfaceInfoList.begin(); iterDis != interfaceInfoList.end();)
		{
			if ( (*iterDis) != NULL )
			{
				VWINTERFACE_INFO* pInterfaceInfo = (VWINTERFACE_INFO*)(*iterDis);
				if(pInterfaceInfo)
				{
					delete pInterfaceInfo;
                    //iterDis = interfaceInfoList.erase( iterDis );
                    interfaceInfoList.erase( iterDis++ );
				}
			}
			else
			{
				iterDis ++;
			}
		}
	}
};


//+++++++++++++++++++++++++++++++++++++++++++++++
// class VwGigE
//+++++++++++++++++++++++++++++++++++++++++++++++
class CLASS_EXPORT VwGigE
{
	private:
		TL_HANDLE		hTL;					// System handle for GigaLinx
		DISCOVERY_INFO	discoveryInfo;			// After calling Discovery() 
		BOOL			isDiscovered;			// Discover completed?
		std::map<string, VwInterface*>	vwInterfaceList;
		
		UINT			m_unFilterType;

	protected:
		BOOL IsValidIP( string strInterfaceSubnet, string strInterfaceIP, string strDeviceIP );

	public:
		VwGigE();
		~VwGigE();
	
		// FUNCTION Open() --------------------------------------
		// 1. Description 
		// (1) Open hTL for GenTL TL_HANDLE
		// (2) Class relationship is
		// VwGigE (Wrapping TL_HANDLE) -> VwInterface (Wrapping IF_HANDLE) -> VwCamera (Wrapping DEV_HANDLE)
		// (3) It must be called to use VwGigE
		// 2. Parameters
		// None
		// 3. Return 
		// RESULT
		// 4. Activity
		// (1) Open TL_HANDLE
		// (2) If discovery didn't called, it calls discovery function
		RESULT Open();

		// FUNCTION Open() --------------------------------------
		// 1. Description 
		// (1) Close hTL for GenTL TL_HANDLE
		// (2) Close all of the interfaces and delete VwInterface objects
		// (3) Close all of the cameras and delete VwCamera objects
		// 2. Parameters
		// None
		// 3. Return 
		// RESULT
		// 4. Activity
		// (1) Delete discovery info.
		RESULT Close();
	
		// Get the number of cameras connected
		RESULT Discovery();
		RESULT GetDiscoveryInfo( OUT DISCOVERY_INFO& pDiscoveryInfo );
		RESULT GetNumCameras(UINT* pNumCameras);
		RESULT GetNumInterfaces(UINT* pNumInterfaces);
		RESULT OpenInterface(UINT nIndex,VwInterface** pVwInterface);
		RESULT OpenInterface( char* name, VwInterface** ppVwInterface );
		RESULT CloseInterface( VwInterface* pVwInterface );
		RESULT CloseCamera( VwCamera* pVwCamera );

        RESULT OpenCamera(UINT nCameraIndex,
                          OUT VwCamera** ppCamera,
                          UINT nNumBuffer,
                          UINT nWidth,
                          UINT nHeight,
                          UINT nPacketSize,
                          void* pUserPointer,
                          ImageCallbackFn* pImageCallbackFn,
                          DisconnectCallbackFn* pDisconnectCallbackFn = NULL);

        RESULT OpenCamera(char* pCameraName,
                          OUT VwCamera** ppCamera,
                          UINT nNumBuffer,
                          UINT nWidth,
                          UINT nHeight,
                          UINT nPacketSize,
                          void* pUserPointer,
                          ImageCallbackFn* pImageCallbackFn,
                          DisconnectCallbackFn* pDisconnectCallbackFn = NULL);

        RESULT OpenCamera(OUT VwCamera** ppCamera,
                          UINT width,
                          UINT height,
                          UINT nPacketSize,
                          UINT nNumBuffer,
                          ImageCallbackFn* pImageCallbackFn,
                          DisconnectCallbackFn* pDisconnectCallbackFn);

        RESULT OpenCamera( char* pInterfaceName,
                           UINT nCameraIndex,
                           OUT VwCamera** ppCamera,
                           UINT nNumBuffer,
                           UINT nWidth,
                           UINT nHeight,
                           UINT nPacketSize,
                           void* pUserPointer,
                           ImageCallbackFn* pImageCallbackFn,
                           DisconnectCallbackFn* pDisconnectCallbackFn = NULL);

		RESULT GetCameraInfo( IN UINT nIndex, OUT VWCAMERA_INFO* pCameraInfo );
		RESULT GetCameraInfo( IN char* pCameraName, OUT VWCAMERA_INFO* pCameraInfo );
		RESULT GetInterfaceInfo( IN UINT nIndex, OUT VWINTERFACE_INFO* pInterfaceInfo );
		RESULT GetInterfaceInfo( IN char* pInterfaceName, OUT VWINTERFACE_INFO* pInterfaceInfo );
		RESULT SetForceIP( IN PBYTE mac, IN DWORD nIP, IN DWORD nSubnet, IN DWORD nGateway );

		RESULT GetFilterType( UINT* unpFilterType );
		RESULT SetFilterType( UINT unFilterType );

        RESULT UseMTUOptimize( IN bool bUse );
        RESULT SetMTUTimeout(UINT uTimeout);
};
