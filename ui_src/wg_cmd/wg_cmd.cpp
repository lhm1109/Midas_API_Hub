// wg_cmd.cpp : Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include "wg_cmd.h"
#include <afxdllx.h>

#include "..\wg_base\LoadingStatus.h"
#include "..\wg_base\XMultiLang.h"

CLoadingStatus _g_LS(_T("wg_cmd"));

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE wg_cmdDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("WG_CMD.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(wg_cmdDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(wg_cmdDLL);
		XMultiLang* pLangTranslate = new XMultiLang;
		pLangTranslate->LoadResourceHeaderByDLL(IDR_WGCMD_RESHEADER,_T("Binary"), hInstance);
		INIResourceManager::AppendMap(_T("wg_cmd"), pLangTranslate);
		
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{


			TRACE0("WG_CMD.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(wg_cmdDLL);
	}
	return 1;   // ok
}

//wg_cmd.dll의 명시적인 초기화가 필요할때 Dll  Client 의
// Application class의  InitInstance Member 함수에서 호출한다. 
// 물론 DllMain의 new CDynLinkLibrary(wg_cmdDLL);
// 구문은 Comment 로 바꾸어야 한다. 
extern "C" void WINAPI Init_wg_cmddll()
{
	//new CDynLinkLibrary(wg_cmdDLL);  
}
