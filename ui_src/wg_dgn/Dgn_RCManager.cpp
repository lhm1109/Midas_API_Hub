// Dgn_RCManager.cpp: implementation of the CDgn_RCManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_RCManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_RCManager::CDgn_RCManager()
{

}

CDgn_RCManager::~CDgn_RCManager()
{

}

BOOL CDgn_RCManager::Calc_RC_Column_Execute_Chk(_CVL_CH_RC_COLUMN_JTG_W& InData, _CVL_CH_RC_COL_RES_W& OutData)
{		
#if defined(_MGEN)
	return FALSE; 
#endif
	
	HINSTANCE hDllCalc = GetOrLoad_CalcDllHandle(DGNENGINE_CH_DLL);
	if(hDllCalc == NULL) return FALSE;

	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function Beam Design").
	typedef BOOL (PSECTCALC)(void*, void*, size_t, size_t, BOOL&);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Calc_RC_Column_Execute_Chk");
	//==================================================================================================
	size_t Tsize_In  = sizeof(_CVL_CH_RC_COLUMN_JTG_W);
	size_t TSize_Out = sizeof(_CVL_CH_RC_COL_RES_W);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(&InData, &OutData, Tsize_In, TSize_Out, bVersionOK);

	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}