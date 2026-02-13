// wg_dgn.cpp : Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include "wg_dgn.h"
#include <afxdllx.h>

#include "..\wg_base\wg_base_ModelessDlgCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "DgnGenLrdfDlg.h"
#include "RCDesignParamDlg.h"
#include "SteelKRDesignParamDlg.h"
#include "RCDesignPlateParamDlg.h"
#include "DgnConBeamNewDlg.h"
#include "DgnConColmNewDlg.h"
#include "DgnConBraceNewDlg.h"
#include "DgnConWallNewDlg.h"
#include "DgnLateralCapaCheckDlg.h"
#include "DgnConMeshedSlabWallDlg.h"
#include "DgnConBentUpBarForBeamSectDlg.h"
#include "DgnSlabSeisLoadCombTypeDlg.h"

#include "..\wg_base\XMultiLang.h"

#include "..\wg_base\LoadingStatus.h"
CLoadingStatus _g_LS(_T("wg_dgn"));


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE wg_dgnDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE(_T("WG_DGN.DLL Initializing!\n"));
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(wg_dgnDLL, hInstance))
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

		new CDynLinkLibrary(wg_dgnDLL);
		XMultiLang* pLangTranslate = new XMultiLang;
		pLangTranslate->LoadResourceHeaderByDLL(IDR_WGDGN_RESHEADER,_T("Binary"), hInstance);
		INIResourceManager::AppendMap(_T("wg_dgn"), pLangTranslate);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE(_T("WG_DGN.DLL Terminating!\n"));
		// Terminate the library before destructors are called
		AfxTermExtensionModule(wg_dgnDLL);
	}
	return 1;   // ok
}

//wg_dgn.dll의 명시적인 초기화가 필요할때 Dll  Client 의
// Application class의  InitInstance Member 함수에서 호출한다. 
// 물론 DllMain의 new CDynLinkLibrary(wg_dgnDLL);
// 구문은 Comment 로 바꾸어야 한다. 
extern "C" void WINAPI Init_wg_dgndll()
{
	//new CDynLinkLibrary(wg_dgnDLL);  
}

// 다른 DLL에서 wg_d에 있는 다이얼로그를 띄울 수 있게 하는
// 창구역할을 하는 함수이다. 새로운 다이얼로그가 추가되면 
// switch문에서 추가해 주어야 한다.
extern __declspec(dllexport) void* DgnCreateOrActivateDlg(CDBDoc* pDoc,UINT nIDTemplate,CWnd* pParent)
{
	CDgnDlgBase* pDlg = (CDgnDlgBase*)(pDoc->m_pDlgCtrl->GetDlg(nIDTemplate));

	DWORD dwLastErr = NO_ERROR;
	if (pDlg == 0) // Dialog가 없으면 만든다.
	{
		// 확장 DLL에서는 AFX_MANAGE_STATE 매크로를 호출하면 안된다.
		//AFX_MANAGE_STATE(AfxGetStaticModuleState());
		switch(nIDTemplate)
		{
		case CDgnGenLrdfDlg::IDD:
			pDlg = new CDgnGenLrdfDlg();
			break;
		case CRCDesignParamDlg::IDD:
			pDlg = new CRCDesignParamDlg();
			break;
		case CSteelKRDesignParamDlg::IDD:
			pDlg = new CSteelKRDesignParamDlg();
			break;
		case CRCDesignPlateParamDlg::IDD:
			pDlg = new CRCDesignPlateParamDlg();
			break;
		case CDgnConBeamNewDlg::IDD:
			pDlg = new CDgnConBeamNewDlg();
			break;
		case CDgnConColmNewDlg::IDD:
			pDlg = new CDgnConColmNewDlg();
			break;
		case CDgnConBraceNewDlg::IDD:
			pDlg = new CDgnConBraceNewDlg();
			break;
		case CDgnConWallNewDlg::IDD:
			pDlg = new CDgnConWallNewDlg();
			break;
		case CDgnLateralCapaCheckDlg::IDD:
			pDlg = new CDgnLateralCapaCheckDlg();
			break;
		case CDgnConMeshedSlabWallDlg::IDD:
			pDlg = new CDgnConMeshedSlabWallDlg();
			break;
		case CDgnConBentUpBarForBeamSectDlg::IDD:
			pDlg = new CDgnConBentUpBarForBeamSectDlg();
			break;
		case CDgnSlabSeisLoadCombTypeDlg::IDD:
			pDlg = new CDgnSlabSeisLoadCombTypeDlg();
			break;
		default:
			GSaveHistoryFormatNF(_T("Not registered Dialog"));
			return 0;
		}
		pDlg->m_pDoc = pDoc;
		pDlg->Create(nIDTemplate, pParent);
		dwLastErr = ::GetLastError();
	}
	if (::GetLastError() == NO_ERROR)
		::SetLastError(dwLastErr);

	pDlg->ShowWindow(SW_SHOW);
	return pDlg;
}


