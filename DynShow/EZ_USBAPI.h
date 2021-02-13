
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EZ_USBAPI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EZ_USBAPI_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef EZ_USBAPI_EXPORTS
#define EZ_USBAPI_API __declspec(dllexport)
#else
#define EZ_USBAPI_API __declspec(dllimport)
#endif

// This class is exported from the EZ_USBAPI.dll
class EZ_USBAPI_API CEZ_USBAPI {
public:
	CEZ_USBAPI(void);
	// TODO: add your methods here.
};

extern EZ_USBAPI_API int nEZ_USBAPI;

EZ_USBAPI_API int fnEZ_USBAPI(void);

//////////////////////////////////////////////////////////////


EZ_USBAPI_API HANDLE OpenDevice();
EZ_USBAPI_API BOOL CloseDevice(HANDLE hDev);
EZ_USBAPI_API int ReadData(HANDLE hDev, void* pBuf, int nBufLen);
EZ_USBAPI_API int WriteData(HANDLE hDev, void* pBuf, int nBufLen);
EZ_USBAPI_API int ControlEndPoint( HANDLE hDev,void* inoutdata,int dataLen);

////////////////////////////////////////
