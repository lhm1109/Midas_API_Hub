#include "stdafx.h"
#include "Db_DllManager_Base.h"
#include "AttrCtrl.h"
#include "PostCtrl.h"
#include "SectUtil.h"
#include "MembCtrl.h"
#include "DBLib.h"
#include "wg_db_MatlDB.h"
#include "wg_db_SectDB.h"

CDgnDLLPathSaver::CDgnDLLPathSaver()
{
	m_csDgnDLLPath = CDBLib::GetDgnPluginPath();
	if ( !m_csDgnDLLPath.IsEmpty() )
	{
		SetDllDirectory(m_csDgnDLLPath);
	}
}

CDgnDLLPathSaver::~CDgnDLLPathSaver()
{
	if ( !m_csDgnDLLPath.IsEmpty() )
	{
		SetDllDirectory(NULL);
	}
}

CDb_DllManager_Base::CDb_DllManager_Base()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_hBaseDll    = NULL;
	m_hCalcDll    = NULL;
	m_hReportDll  = NULL;
	m_hDrawDll    = NULL;
	m_hUmdDataDll = NULL;
	m_hGsdDataDll = NULL;
	m_hPBeamLMeshDll = NULL;
	m_iNationType = 0;
}

CDb_DllManager_Base::~CDb_DllManager_Base()
{
	FreeDllLibrary();
}

void CDb_DllManager_Base::FreeDllLibrary()
{
	if ( m_hBaseDll != NULL )
	{
		FreeLibrary(m_hBaseDll);
		m_hBaseDll = NULL;
	}
	if ( m_hCalcDll != NULL )
	{
		FreeLibrary(m_hCalcDll);
		m_hCalcDll = NULL;
	}
	if ( m_hReportDll != NULL )
	{
		FreeLibrary(m_hReportDll);
		m_hReportDll = NULL;
	}
	if ( m_hDrawDll != NULL )
	{
		FreeLibrary(m_hDrawDll);
		m_hDrawDll = NULL;
	}
	if ( m_hUmdDataDll != NULL )
	{
		FreeLibrary(m_hUmdDataDll);
		m_hUmdDataDll = NULL;
	}
	if ( m_hGsdDataDll != NULL )
	{
		FreeLibrary(m_hGsdDataDll);
		m_hGsdDataDll = NULL;
	}
	if ( m_hPBeamLMeshDll != NULL )
	{
		FreeLibrary(m_hPBeamLMeshDll);
		m_hPBeamLMeshDll = NULL;
	}
	m_iNationType = 0;
}

CString CDb_DllManager_Base::Get_CalcDllFileName(int iNationType)
{
	BOOL bDebug = FALSE;
	BOOL bCivil = TRUE;
	CString strDllName = _T("");

#if defined(_DEBUG)
	bDebug = TRUE;
#endif

#if defined(_MGEN)
	bCivil = FALSE;
#endif

	if ( bCivil )
	{
		if ( iNationType == DGNENGINE_KR_DLL ) strDllName = bDebug ? _T("CVL_DgnCalc_KRD.dll") : _T("CVL_DgnCalc_KR.dll");
		else if ( iNationType == DGNENGINE_US_DLL ) strDllName = bDebug ? _T("CVL_DgnCalc_USD.dll") : _T("CVL_DgnCalc_US.dll");
		else if ( iNationType == DGNENGINE_CH_DLL ) strDllName = bDebug ? _T("CVL_DgnCalc_CHD.dgne") : _T("CVL_DgnCalc_CH.dgne");
		//else if(iNationType == DGNENGINE_JP_DLL) strDllName = bDebug ? _T("CVL_DgnCalc_JPD.dll") : _T("CVL_DgnCalc_JP.dll");
		else if ( iNationType == DGNENGINE_RS_DLL ) strDllName = bDebug ? _T("DGNe_RUSD.dgne") : _T("DGNe_RUS.dgne");
		else ASSERT(0);
	}
	else
	{
		if ( iNationType == DGNENGINE_KR_DLL ) strDllName = bDebug ? _T("GEN_DgnCalc_KRD.dll") : _T("GEN_DgnCalc_KR.dll");
		else if ( iNationType == DGNENGINE_US_DLL ) strDllName = bDebug ? _T("GEN_DgnCalc_USD.dll") : _T("GEN_DgnCalc_US.dll");
		else if ( iNationType == DGNENGINE_CH_DLL ) strDllName = bDebug ? _T("GEN_DgnCalc_CHD.dgne") : _T("GEN_DgnCalc_CH.dgne");
		//else if(iNationType == DGNENGINE_JP_DLL) strDllName = bDebug ? _T("GEN_DgnCalc_JPD.dll") : _T("GEN_DgnCalc_JP.dll");
		else ASSERT(0);
	}

	return strDllName;
}

CString CDb_DllManager_Base::Get_ReportDllFileName(int iNationType)
{
	BOOL bDebug = FALSE;
	BOOL bCivil = TRUE;
	CString strDllName = _T("");

#if defined(_DEBUG)
	bDebug = TRUE;
#endif

#if defined(_MGEN)
	bCivil = FALSE;
#endif

	if ( bCivil )
	{
		if ( iNationType == DGNENGINE_KR_DLL ) strDllName = bDebug ? _T("CVL_DgnReport_KRD.dgne") : _T("CVL_DgnReport_KR.dgne");
		else if ( iNationType == DGNENGINE_US_DLL ) strDllName = bDebug ? _T("CVL_DgnReport_USD.dgne") : _T("CVL_DgnReport_US.dgne");
		else if ( iNationType == DGNENGINE_CH_DLL ) strDllName = bDebug ? _T("CVL_DgnReport_CHD.dgne") : _T("CVL_DgnReport_CH.dgne");
		//else if(iNationType == DGNENGINE_JP_DLL) strDllName = bDebug ? _T("CVL_DgnReport_JPD.dgne") : _T("CVL_DgnReport_JP.dgne");
		else if ( iNationType == DGNENGINE_RS_DLL ) strDllName = bDebug ? _T("DGNe_RUSD.dgne") : _T("DGNe_RUS.dgne");
		else ASSERT(0);
	}
	else
	{
		if ( iNationType == DGNENGINE_KR_DLL ) strDllName = bDebug ? _T("GEN_DgnReport_KRD.dgne") : _T("GEN_DgnReport_KR.dgne");
		else if ( iNationType == DGNENGINE_US_DLL ) strDllName = bDebug ? _T("GEN_DgnReport_USD.dgne") : _T("GEN_DgnReport_US.dgne");
		else if ( iNationType == DGNENGINE_CH_DLL ) strDllName = bDebug ? _T("GEN_DgnReport_CHD.dgne") : _T("GEN_DgnReport_CH.dgne");
		//else if(iNationType == DGNENGINE_JP_DLL) strDllName = bDebug ? _T("GEN_DgnReport_JPD.dgne") : _T("GEN_DgnReport_JP.dgne");
		else ASSERT(0);
	}

	return strDllName;
}
CString CDb_DllManager_Base::Get_BaseManagerDllFileName()
{
	BOOL bDebug = FALSE;

#if defined(_DEBUG)
	bDebug = TRUE;
#endif

	return bDebug ? _T("DgnBaseManagerD.dgne") : _T("DgnBaseManager.dgne");
}
CString CDb_DllManager_Base::Get_DrawDllFileName()
{
	BOOL bDebug = FALSE;
	BOOL bCivil = TRUE;
	CString strDllName = _T("");

#if defined(_DEBUG)
	bDebug = TRUE;
#endif

#if defined(_MGEN)
	bCivil = FALSE;
#endif

	if ( bCivil ) strDllName = bDebug ? _T("CVL_DgnDraw_ExD.dgne") : _T("CVL_DgnDraw_Ex.dgne");
	else       strDllName = bDebug ? _T("GEN_DgnDraw_ExD.dgne") : _T("GEN_DgnDraw_Ex.dgne");

	return strDllName;
}
CString CDb_DllManager_Base::Get_UmdDataDllFileName()
{
	BOOL bDebug = FALSE;
	BOOL bCivil = TRUE;
	CString strDllName = _T("");

#if defined(_DEBUG)
	bDebug = TRUE;
#endif

#if defined(_MGEN)
	bCivil = FALSE;
#endif

	if ( bCivil ) strDllName = bDebug ? _T("CVL_UmdDataBaseD.dll") : _T("CVL_UmdDataBase.dll");
	else       strDllName = bDebug ? _T("GEN_UmdDataBaseD.dll") : _T("GEN_UmdDataBase.dll");

	return strDllName;
}

CString CDb_DllManager_Base::Get_GSDDataDllFileName()
{
	BOOL bDebug = FALSE;
	CString strDllName = _T("");

#if defined(_DEBUG)
	bDebug = TRUE;
#endif

	// by Hsshim. 2015-03-24.
	// GSD 마이그레이션 검증이 끝나기 전에는 기존 MBCS 버전 사용 (2015년 상반기)
	// DgnEngine이 Unicode 버전으로 바뀜에 따라, Unicode 버전 DgnEngine을 UMD, Design+ 에서 사용할 수 있도록하기 위해
	// 파일이름을 바꿔서 기존 MBCS 버전을 사용하도록 처리

	strDllName = bDebug ? _T("GSD_DataBaseD.dll") : _T("GSD_DataBase.dll");

	return strDllName;
}

CString CDb_DllManager_Base::Get_PBeamLMeshDllFileName()
{
	BOOL bDebug = FALSE;
	CString strDllName = _T("PBeamLMesh.dll");

	// #if defined(_DEBUG)
	//   bDebug = TRUE;
	// #endif
	//
	// 	strDllName = bDebug ? _T("PBeamLMesh_de.dll") : _T("PBeamLMesh.dll");

	return strDllName;
}

HINSTANCE CDb_DllManager_Base::GetOrLoad_CalcDllHandle(int iNationType)
{
	if ( m_hCalcDll != NULL && m_iNationType == iNationType ) return m_hCalcDll;

	if ( m_hCalcDll != NULL )
	{
		FreeLibrary(m_hCalcDll);    m_hCalcDll = NULL;
	}
	if ( m_hReportDll != NULL && m_iNationType != iNationType )
	{
		FreeLibrary(m_hReportDll);  m_hReportDll = NULL;
	}
	m_iNationType = iNationType;
	CString strDllName = Get_CalcDllFileName(iNationType);

	{
		// LoadLibrary()에서 리소스 체인을 망가뜨리는 경우가 있어서
		// 이를 복원하기 위해 AfxGetModuleState() 함수를 사용함.
		AFX_MANAGE_STATE(AfxGetModuleState());

		if ( m_hCalcDll == NULL )
		{
			if ( ( m_hCalcDll = LoadLibrary(strDllName) ) == NULL )
			{
				CString strMsg = "Can't Find "+strDllName;
				AfxMessageBox(strMsg);
			}
		}
	}

	return m_hCalcDll;
}

HINSTANCE CDb_DllManager_Base::GetOrLoad_ReportDllHandle(int iNationType)
{
	if ( m_hReportDll != NULL && m_iNationType == iNationType ) return m_hReportDll;

	if ( m_hReportDll != NULL )
	{
		FreeLibrary(m_hReportDll);  m_hReportDll = NULL;
	}
	if ( m_hCalcDll != NULL && m_iNationType != iNationType )
	{
		FreeLibrary(m_hCalcDll);    m_hCalcDll = NULL;
	}
	m_iNationType = iNationType;
	CString strDllName = Get_ReportDllFileName(iNationType);

	{
		// LoadLibrary()에서 리소스 체인을 망가뜨리는 경우가 있어서
		// 이를 복원하기 위해 AfxGetModuleState() 함수를 사용함.
		AFX_MANAGE_STATE(AfxGetModuleState());

		if ( m_hReportDll == NULL )
		{
			if ( ( m_hReportDll =  LoadLibrary(strDllName) ) == NULL )  // LoadLibrary  GetModuleHandle
			{
				CString strMsg = "Can't Find "+strDllName;
				AfxMessageBox(strMsg);
			}
		}
	}

	return m_hReportDll;
}

HINSTANCE CDb_DllManager_Base::GetOrLoad_BaseManagerDllHandle()
{
	if ( m_hBaseDll != NULL ) return m_hBaseDll;

	m_hBaseDll = NULL;
	CString strDllName = Get_BaseManagerDllFileName();

	{
		// LoadLibrary()에서 리소스 체인을 망가뜨리는 경우가 있어서
		// 이를 복원하기 위해 AfxGetModuleState() 함수를 사용함.
		AFX_MANAGE_STATE(AfxGetModuleState());

		if ( m_hBaseDll == NULL )
		{
			if ( ( m_hBaseDll = LoadLibrary(strDllName) ) == NULL )
			{
				CString strMsg = "Can't Find "+strDllName;
				AfxMessageBox(strMsg);
			}
		}
	}

	return m_hBaseDll;
}

HINSTANCE CDb_DllManager_Base::GetOrLoad_DrawDllHandle()
{
	if ( m_hDrawDll != NULL ) return m_hDrawDll;

	m_hDrawDll = NULL;
	CString strDllName = Get_DrawDllFileName();

	{
		// LoadLibrary()에서 리소스 체인을 망가뜨리는 경우가 있어서
		// 이를 복원하기 위해 AfxGetModuleState() 함수를 사용함.
		AFX_MANAGE_STATE(AfxGetModuleState());

		if ( m_hDrawDll == NULL )
		{
			if ( ( m_hDrawDll = LoadLibrary(strDllName) ) == NULL )
			{
				CString strMsg = "Can't Find "+strDllName;
				AfxMessageBox(strMsg);
			}
		}
	}

	return m_hDrawDll;
}
HINSTANCE CDb_DllManager_Base::GetOrLoad_UmdDataDllHandle()
{
	if ( m_hUmdDataDll != NULL ) return m_hUmdDataDll;

	m_hUmdDataDll = NULL;
	CString strDllName = Get_UmdDataDllFileName();

	{
		// LoadLibrary()에서 리소스 체인을 망가뜨리는 경우가 있어서
		// 이를 복원하기 위해 AfxGetModuleState() 함수를 사용함.
		AFX_MANAGE_STATE(AfxGetModuleState());

		if ( m_hUmdDataDll == NULL )
		{
			if ( ( m_hUmdDataDll = LoadLibrary(strDllName) ) == NULL )
			{
				CString strMsg = "Can't Find "+strDllName;
				AfxMessageBox(strMsg);
			}
		}
	}

	return m_hUmdDataDll;
}

HINSTANCE CDb_DllManager_Base::GetOrLoad_GsdDataDllHandle()
{
	if ( m_hGsdDataDll != NULL ) return m_hGsdDataDll;

	m_hGsdDataDll = NULL;
	CString strDllName = Get_GSDDataDllFileName();

	{
		// LoadLibrary()에서 리소스 체인을 망가뜨리는 경우가 있어서
		// 이를 복원하기 위해 AfxGetModuleState() 함수를 사용함.
		AFX_MANAGE_STATE(AfxGetModuleState());

		if ( m_hGsdDataDll == NULL )
		{
			if ( ( m_hGsdDataDll = LoadLibrary(strDllName) ) == NULL )
			{
				DWORD dwError = GetLastError();
				CString strMsg = "Can't Find "+strDllName;
#if defined _DEBUG
				TRACE(strMsg);
#else
				AfxMessageBox(strMsg);
#endif
			}
		}
	}

	return m_hGsdDataDll;
}

HINSTANCE CDb_DllManager_Base::GetOrLoad_PBeamLMeshDllHandle()
{
	if ( m_hPBeamLMeshDll != NULL ) return m_hPBeamLMeshDll;

	m_hPBeamLMeshDll = NULL;
	CString strDllName = Get_PBeamLMeshDllFileName();

	{
		// LoadLibrary()에서 리소스 체인을 망가뜨리는 경우가 있어서
		// 이를 복원하기 위해 AfxGetModuleState() 함수를 사용함.
		AFX_MANAGE_STATE(AfxGetModuleState());

		if ( m_hPBeamLMeshDll == NULL )
		{
#ifdef _MEC
#if defined(_X64)
			CString strPath = m_pDoc->GetDocPoint()->GetProgramPath() + _T("64bit Solver\\") + strDllName;
#else
			CString strPath = m_pDoc->GetDocPoint()->GetProgramPath() + _T("32bit Solver\\") + strDllName;
#endif
#else
			CString strPath = m_pDoc->GetDocPoint()->GetProgramPath() + strDllName;
#endif
			if ( ( m_hPBeamLMeshDll = LoadLibrary(strPath) ) == NULL )
			{
				CString strMsg = "Can't Find "+strDllName;
				AfxMessageBox(strMsg);
			}
		}
	}

	return m_hPBeamLMeshDll;
}
//////////////////////////////////////////////////////////////////////////

T_SECT_SECTBASE_D CDb_DllManager_Base::GetSectBaseD(T_SECT_D& SectD, int nIMJ)
{
	int nSType;
	return GetSectBaseD(SectD, nSType, nIMJ, TRUE);
}
T_SECT_SECTBASE_D CDb_DllManager_Base::GetSectBaseD(T_SECT_D& SectD, int& nSType, int nIMJ, BOOL bAfter)
{
	if ( nIMJ == 0 ) return GetSectBaseD_Pos(SectD, nSType, 0, bAfter); // I단
	else if ( nIMJ == 2 ) return GetSectBaseD_Pos(SectD, nSType, 4, bAfter); // J단
	else if ( nIMJ == 1 ) return GetSectBaseD_Pos(SectD, nSType, 2, bAfter); // M단

	//ASSERT(0);
	return GetSectBaseD_Pos(SectD, nSType, 0, bAfter);
}

T_SECT_SECTBASE_D CDb_DllManager_Base::GetSectBaseD_Pos(T_SECT_D& SectD, int& nSType, int nPos, BOOL bAfter)
{
	double dPos = 0.0;
	if ( nPos == 0 ) dPos = 0.0;
	else if ( nPos == 1 ) dPos = 1.0/4.0;
	else if ( nPos == 2 ) dPos = 2.0/4.0;
	else if ( nPos == 3 ) dPos = 3.0/4.0;
	else if ( nPos == 4 ) dPos = 1.0;
	else ASSERT(0);

	return GetSectBaseD_Pos(SectD, nSType, dPos, bAfter);
}

T_SECT_SECTBASE_D CDb_DllManager_Base::GetSectBaseD_Pos(T_SECT_D& SectD, int& nSType, double dPos, BOOL bAfter)
{
	nSType = ( SectD.nStype == D_SECT_TYPE_TAPERED ) ? ( ( bAfter && SectD.SectAfter.nStype>0 ) ? SectD.SectAfter.nStype : SectD.SectBefore.nStype ) : SectD.nStype;
	if ( SectD.nStype==D_SECT_TYPE_CONSTRUCTION )
	{
		if ( SectD.SectBefore.nStype==D_SECT_TYPE_TAP_REG || SectD.SectBefore.nStype==D_SECT_TYPE_TAP_USER )
			nSType = D_SECT_TYPE_REGULAR;
		else
			nSType = ( ( bAfter && SectD.SectAfter.nStype>0 ) ? SectD.SectAfter.nStype : SectD.SectBefore.nStype );
	}
	if ( SectD.nStype==D_SECT_TYPE_USER )
		nSType = D_SECT_TYPE_REGULAR;

	BOOL bDumb = m_pDoc->m_pSectDB->IsSectDumbAndStub(SectD);

	T_SECT_SECTBASE_D SectBaseD, SectBaseI, SectBaseJ;
	SectBaseD.Initialize();  SectBaseI.Initialize();  SectBaseJ.Initialize();
	T_SECT_STIFFNESS StiffnessD;   StiffnessD.Initialize();
	T_SECT_DESIGN    DesignD;      DesignD.Initialize();
	CString          strShape;     strShape = "";

	SectBaseI  = SectD.SectBefore.SectI;
	SectBaseJ  = SectD.SectBefore.SectJ;
	StiffnessD = SectD.SectBefore.Stiffness;
	DesignD    = SectD.SectBefore.Design;
	strShape   = SectD.SectBefore.Shape;
	if ( SectD.nStype==D_SECT_TYPE_CONSTRUCTION )
	{
		if ( SectD.SectBefore.nStype!=D_SECT_TYPE_TAP_REG && SectD.SectBefore.nStype!=D_SECT_TYPE_TAP_USER )
			dPos = 0.0;
		SectBaseI  = ( bAfter ) ? SectD.SectAfter.SectI : SectD.SectBefore.SectI;
		SectBaseJ  = ( bAfter ) ? SectD.SectAfter.SectJ : SectD.SectBefore.SectJ;
		StiffnessD = ( bAfter ) ? SectD.SectAfter.Stiffness : SectD.SectBefore.Stiffness;
		DesignD    = ( bAfter ) ? SectD.SectAfter.Design : SectD.SectBefore.Design;
		strShape   = ( bAfter && SectD.SectAfter.Shape != "" ) ? SectD.SectAfter.Shape : SectD.SectBefore.Shape;
	}
	else if ( SectD.nStype == D_SECT_TYPE_COMPO_B      || SectD.nStype == D_SECT_TYPE_COMPO_I      || SectD.nStype == D_SECT_TYPE_COMPO_TUB      ||
		SectD.nStype == D_SECT_TYPE_COMPO_STLG_B || SectD.nStype == D_SECT_TYPE_COMPO_STLG_I || SectD.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
		SectD.nStype == D_SECT_TYPE_COMPO_CI     || SectD.nStype == D_SECT_TYPE_COMPO_CT     || SectD.nStype == D_SECT_TYPE_COMPO_PC )
	{
		dPos = 0.0;
		if ( bAfter )
		{
			SectBaseI.Design    = SectD.SectAfter.SectI.Design;
			SectBaseI.Stiffness = SectD.SectAfter.SectI.Stiffness;
			SectBaseJ.Design    = SectD.SectAfter.SectJ.Design;
			SectBaseJ.Stiffness = SectD.SectAfter.SectJ.Stiffness;
			StiffnessD = SectD.SectAfter.Stiffness;
			DesignD    = SectD.SectAfter.Design;
			if ( SectD.SectAfter.Shape != "" ) strShape = SectD.SectAfter.Shape;
		}
	}
	else if (bDumb)
	{
		dPos = 0.0;
		if (bAfter)
		{
			SectBaseI.Design    = SectD.SectAfter.SectI.Design;
			SectBaseI.Stiffness = SectD.SectAfter.SectI.Stiffness;
			SectBaseJ.Design    = SectD.SectAfter.SectJ.Design;
			SectBaseJ.Stiffness = SectD.SectAfter.SectJ.Stiffness;
			StiffnessD          = SectD.SectAfter.Stiffness;
			DesignD             = SectD.SectAfter.Design;
			if ( SectD.SectAfter.Shape != "" ) strShape = SectD.SectAfter.Shape;
		}
	}
	else if ( SectD.nStype == D_SECT_TYPE_COMPO_G )
	{
		dPos = 0.0;
		//SectBaseI  = (bAfter) ? SectD.SectAfter.SectI : SectD.SectBefore.SectI;
		//SectBaseJ  = (bAfter) ? SectD.SectAfter.SectJ : SectD.SectBefore.SectJ;
		if ( SectBaseI.nBeforePart==0 ) SectBaseI.nBeforePart=SectD.SectBefore.SectI.nBeforePart;
		if ( SectBaseJ.nBeforePart==0 ) SectBaseJ.nBeforePart=SectD.SectBefore.SectI.nBeforePart;
		int nPartSize, nBeforePart;
		T_GSEC_PART GPartD;
		nPartSize = SectBaseI.aGeneralPart.GetSize();
		nBeforePart = min(SectBaseI.nBeforePart, nPartSize);
		if ( nPartSize > 0 && nBeforePart > 0 )
		{
			GPartD = bAfter ? SectBaseI.aGeneralPart[nPartSize-1] : SectBaseI.aGeneralPart[nBeforePart-1];
			SectBaseI.Stiffness   = GPartD.StiffnessSum;
			SectBaseI.Design.YBar = GPartD.YBarSum;
			SectBaseI.Design.ZBar = GPartD.ZBarSum;
			// 			SectBaseI.Design.YBar = GPartD.StiffnessSum.Cym; // 도심점은 시공된 단면의 도심점을 사용
			// 			SectBaseI.Design.ZBar = GPartD.StiffnessSum.Czm;
		}

		nPartSize = SectBaseJ.aGeneralPart.GetSize();
		nBeforePart = min(SectBaseJ.nBeforePart, nPartSize);
		if ( nPartSize > 0 && nBeforePart > 0 )
		{
			GPartD = bAfter ? SectBaseJ.aGeneralPart[nPartSize-1] : SectBaseJ.aGeneralPart[nBeforePart-1];
			SectBaseJ.Stiffness   = GPartD.StiffnessSum;
			SectBaseJ.Design.YBar = GPartD.YBarSum; // 도심점은 시공된 단면의 도심점을 사용
			SectBaseJ.Design.ZBar = GPartD.ZBarSum;
			// 			SectBaseJ.Design.YBar = GPartD.StiffnessSum.Cym; // 도심점은 시공된 단면의 도심점을 사용
			// 			SectBaseJ.Design.ZBar = GPartD.StiffnessSum.Czm;
		}
	}
	else if ( SectD.nStype==D_SECT_TYPE_TAPERED )
	{
		if ( SectD.SectBefore.nStype==D_SECT_TYPE_CONSTRUCTION )
		{
			SectBaseI  = ( bAfter ) ? SectD.SectAfter.SectI : SectD.SectBefore.SectI;
			SectBaseJ  = ( bAfter ) ? SectD.SectAfter.SectJ : SectD.SectBefore.SectJ;
			StiffnessD = ( bAfter ) ? SectD.SectAfter.Stiffness : SectD.SectBefore.Stiffness;
			DesignD    = ( bAfter ) ? SectD.SectAfter.Design : SectD.SectBefore.Design;
			strShape   = ( bAfter && SectD.SectAfter.Shape != "" ) ? SectD.SectAfter.Shape : SectD.SectBefore.Shape;
		}
		else if ( SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_B      || SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_I      || SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB      ||
			SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B || SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
			SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_CI     || SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_CT     || SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_PC )
		{
			if ( bAfter )
			{
				SectBaseI.Design    = SectD.SectAfter.SectI.Design;
				SectBaseI.Stiffness = SectD.SectAfter.SectI.Stiffness;
				SectBaseJ.Design    = SectD.SectAfter.SectJ.Design;
				SectBaseJ.Stiffness = SectD.SectAfter.SectJ.Stiffness;
				StiffnessD = SectD.SectAfter.Stiffness;
				DesignD    = SectD.SectAfter.Design;
				if ( SectD.SectAfter.Shape != "" ) strShape = SectD.SectAfter.Shape;
			}

			for ( int i=0; i<D_SECT_SIZE_NUM_MAX; i++ ) SectBaseJ.Size[i] = SectD.CmpTapJ.Size[i];
		}
		else if ( SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_G )
		{
			//SectBaseI  = (bAfter) ? SectD.SectAfter.SectI : SectD.SectBefore.SectI;
			//SectBaseJ  = (bAfter) ? SectD.SectAfter.SectJ : SectD.SectBefore.SectJ;
			if ( SectBaseI.nBeforePart==0 ) SectBaseI.nBeforePart=SectD.SectBefore.SectI.nBeforePart;
			if ( SectBaseJ.nBeforePart==0 ) SectBaseJ.nBeforePart=SectD.SectBefore.SectI.nBeforePart;
			int nPartSize, nBeforePart;
			T_GSEC_PART GPartD;
			nPartSize = SectBaseI.aGeneralPart.GetSize();
			nBeforePart = min(SectBaseI.nBeforePart, nPartSize);
			if ( nPartSize > 0 && nBeforePart > 0 )
			{
				GPartD = bAfter ? SectBaseI.aGeneralPart[nPartSize-1] : SectBaseI.aGeneralPart[nBeforePart-1];
				SectBaseI.Stiffness   = GPartD.StiffnessSum;
				SectBaseI.Design.YBar = GPartD.YBarSum;
				SectBaseI.Design.ZBar = GPartD.ZBarSum;
				// 				SectBaseI.Design.YBar = GPartD.StiffnessSum.Cym; // 도심점은 시공된 단면의 도심점을 사용
				// 				SectBaseI.Design.ZBar = GPartD.StiffnessSum.Czm;
			}

			nPartSize = SectBaseJ.aGeneralPart.GetSize();
			nBeforePart = min(SectBaseJ.nBeforePart, nPartSize);
			if ( nPartSize > 0 && nBeforePart > 0 )
			{
				GPartD = bAfter ? SectBaseJ.aGeneralPart[nPartSize-1] : SectBaseJ.aGeneralPart[nBeforePart-1];
				SectBaseJ.Stiffness   = GPartD.StiffnessSum;
				SectBaseJ.Design.YBar = GPartD.YBarSum;
				SectBaseJ.Design.ZBar = GPartD.ZBarSum;
				// 				SectBaseJ.Design.YBar = GPartD.StiffnessSum.Cym; // 도심점은 시공된 단면의 도심점을 사용
				// 				SectBaseJ.Design.ZBar = GPartD.StiffnessSum.Czm;
			}
		}
		else if ( SectD.SectBefore.nStype == D_SECT_TYPE_STLG_B ||
			SectD.SectBefore.nStype == D_SECT_TYPE_STLG_I ||
			SectD.SectBefore.nStype == D_SECT_TYPE_STLG_MCELL )
		{
			SectBaseI  = SectD.SectBefore.SectI;
			SectBaseJ  = SectD.SectBefore.SectJ;
			StiffnessD = SectD.SectBefore.SectI.Stiffness;
			DesignD    = SectD.SectBefore.SectI.Design;
			strShape   = SectD.SectBefore.Shape;
		}
	}
	else if ( SectD.nStype == D_SECT_TYPE_STLG_B ||
		SectD.nStype == D_SECT_TYPE_STLG_I ||
		SectD.nStype == D_SECT_TYPE_STLG_MCELL )
	{
		SectBaseI  = SectD.SectBefore.SectI;
		SectBaseJ  = SectD.SectBefore.SectI;
		StiffnessD = SectD.SectBefore.SectI.Stiffness;
		DesignD    = SectD.SectBefore.SectI.Design;
		strShape   = SectD.SectBefore.Shape;
	}
	else
	{
#if defined(_CIVIL_JP)
		if (SectD.nStype == D_SECT_TYPE_TAPERED)
		{
			dPos = dPos;
		}
		else dPos = 0.0;
#else
		dPos = 0.0;
#endif
	}

	SectBaseD = GetSectBaseD_Pos(SectBaseI, SectBaseJ, dPos);

	if ( SectBaseD.Shape != strShape && strShape.GetLength() != 0 )
	{
		// 단면의 형상은 TAPERED의 유무와 상관없이 Shape를 따라 간다.
		SectBaseD.Shape = strShape;
	}

	if ( nSType == D_SECT_TYPE_TAPERED || ( nSType == D_SECT_TYPE_USER && SectBaseD.Shape != D_SECT_SHAPE_REG_GEN ) )
	{
		int nTempStype = D_SECT_TYPE_REGULAR;
		if ( Find_nStype(SectBaseD.Shape, nTempStype) )
		{
			nSType = nTempStype;
		}
	}
	else if ( nSType == DGN_SECT_TYPE_TAP_REG )
	{
		int nTempStype = D_SECT_TYPE_REGULAR;
		if ( Find_nStype(SectBaseD.Shape, nTempStype) )
		{
			ASSERT(nSType!=D_SECT_TYPE_REGULAR);
			nSType = nTempStype;
		}
	}
	else if ( nSType == DGN_SECT_TYPE_TAP_USER && SectBaseD.Shape != D_SECT_SHAPE_REG_GEN )
	{
		int nTempStype = D_SECT_TYPE_REGULAR;
		if ( Find_nStype(SectBaseD.Shape, nTempStype) )
		{
			nSType = nTempStype;
		}
	}

	//if(nSType == D_SECT_TYPE_COMPO_G && (dPos != 0.0 && dPos != 1.0)) ASSERT(0); // D_SECT_TYPE_COMPO_G은 중앙부 지원하지 않음  이상없는지 Test 확인요망(2010.08.10)

	if ( SectD.nStype!=D_SECT_TYPE_TAPERED )
	{
		if ( SectBaseD.Stiffness.Area==0.0 || StiffnessD.Area!=0.0 ) SectBaseD.Stiffness = StiffnessD;
		if ( ( SectBaseD.Design.YBar==0.0 && SectBaseD.Design.ZBar==0.0 ) || ( DesignD.YBar!=0.0 || DesignD.ZBar!=0.0 ) ) SectBaseD.Design = DesignD;
	}

	if ( SectBaseD.Stiffness.Area!=0.0 && ( SectBaseD.Design.YBar==0.0 && SectBaseD.Design.ZBar==0.0 ) )
	{
		T_SECT_D TempSectD = SectD;
		TempSectD.nStype = nSType;
		TempSectD.SectBefore.Shape = SectBaseD.Shape;
		TempSectD.SectBefore.SectI = SectBaseD;
		if ( CSectUtil::CalcSectData(TempSectD, FALSE) )
		{
			if ( TempSectD.SectBefore.SectI.Design.YBar!=0.0 || TempSectD.SectBefore.SectI.Design.ZBar!=0.0 )
				SectBaseD.Design = TempSectD.SectBefore.SectI.Design;
		}
	}

	return SectBaseD;
}

T_SECT_SECTBASE_D CDb_DllManager_Base::GetSectBaseD(T_SECT_SECTBASE_D& SectI, T_SECT_SECTBASE_D& SectJ, int nIMJ)
{
	if ( nIMJ == 0 ) return GetSectBaseD_Pos(SectI, SectJ, 0.0); // I단
	else if ( nIMJ == 1 ) return GetSectBaseD_Pos(SectI, SectJ, 0.5); // M단
	else if ( nIMJ == 2 ) return GetSectBaseD_Pos(SectI, SectJ, 1.0); // J단

	ASSERT(0);
	return SectI;
}

T_SECT_SECTBASE_D CDb_DllManager_Base::GetSectBaseD_Pos(T_SECT_SECTBASE_D& SectI, T_SECT_SECTBASE_D& SectJ, int nPos)
{
	double dPos = 0.0;
	if ( nPos == 0 ) dPos = 0.0;
	else if ( nPos == 1 ) dPos = 1.0/4.0;
	else if ( nPos == 2 ) dPos = 2.0/4.0;
	else if ( nPos == 3 ) dPos = 3.0/4.0;
	else if ( nPos == 4 ) dPos = 1.0;
	else ASSERT(0);

	return GetSectBaseD_Pos(SectI, SectJ, dPos);
}

T_SECT_SECTBASE_D CDb_DllManager_Base::GetSectBaseD_Pos(T_SECT_SECTBASE_D& SectI, T_SECT_SECTBASE_D& SectJ, double dPos)
{
	T_SECT_SECTBASE_D SectBaseD;  SectBaseD.Initialize();

	if ( dPos <= 0.0 ) // I단
		SectBaseD = SectI;
	else if ( dPos >= 1.0 ) // J단
		SectBaseD = SectJ;
	else if ( dPos > 0.0 && dPos<1.0 ) // M단
	{
		SectBaseD = SectI;
		double dR_I = 1.0-dPos, dR_J = dPos;

		// Size 평균
		for ( int i=0; i<D_SECT_SIZE_NUM_MAX; i++ )
			SectBaseD.Size[i] = dR_I*SectI.Size[i] + dR_J*SectJ.Size[i];

		// Stiffness 평균
		SectBaseD.Stiffness = GetSectStiffness_Pos(SectI.Stiffness, SectJ.Stiffness, dPos);
		// Desgin 평균
		SectBaseD.Design = GetSectDesign_Pos(SectI.Design, SectJ.Design, dPos);

		// 외곽 Polygon 평균
		if ( SectI.aOuterPolygon.GetSize() != SectJ.aOuterPolygon.GetSize() )
			return SectBaseD;
		int nOutPolygon = SectBaseD.aOuterPolygon.GetSize();

		int nPoint, k, n;
		for ( k=0; k<nOutPolygon; k++ )
		{
			if ( SectI.aOuterPolygon[k].aVertex.GetSize() != SectJ.aOuterPolygon[k].aVertex.GetSize() )
				return SectBaseD;

			nPoint = SectBaseD.aOuterPolygon[k].aVertex.GetSize();
			for ( n=0; n<nPoint; n++ )
			{
				SectBaseD.aOuterPolygon[k].aVertex[n].dX = ( dR_I*SectI.aOuterPolygon[k].aVertex[n].dX + dR_J*SectJ.aOuterPolygon[k].aVertex[n].dX );
				SectBaseD.aOuterPolygon[k].aVertex[n].dY = ( dR_I*SectI.aOuterPolygon[k].aVertex[n].dY + dR_J*SectJ.aOuterPolygon[k].aVertex[n].dY );
			}
		}
		// 내부 Polygon 평균
		if ( SectI.aInnerPolygon.GetSize() != SectJ.aInnerPolygon.GetSize() )
			return SectBaseD;
		int nInPolygon = SectBaseD.aInnerPolygon.GetSize();

		for ( k=0; k<nInPolygon; k++ )
		{
			if ( SectI.aInnerPolygon[k].aVertex.GetSize() != SectJ.aInnerPolygon[k].aVertex.GetSize() )
				return SectBaseD;

			nPoint = SectBaseD.aInnerPolygon[k].aVertex.GetSize();
			for ( n=0; n<nPoint; n++ )
			{
				SectBaseD.aInnerPolygon[k].aVertex[n].dX = ( dR_I*SectI.aInnerPolygon[k].aVertex[n].dX + dR_J*SectJ.aInnerPolygon[k].aVertex[n].dX );
				SectBaseD.aInnerPolygon[k].aVertex[n].dY = ( dR_I*SectI.aInnerPolygon[k].aVertex[n].dY + dR_J*SectJ.aInnerPolygon[k].aVertex[n].dY );
			}
		}
		// Line
		if ( SectI.aLine.GetSize() != SectJ.aLine.GetSize() )
			return SectBaseD;
		int nLine = SectBaseD.aLine.GetSize();
		for ( k=0; k<nLine; k++ )
		{
			SectBaseD.aLine[k].dA    = ( dR_I*SectI.aLine[k].dA    + dR_J*SectJ.aLine[k].dA );
			SectBaseD.aLine[k].dThik = ( dR_I*SectI.aLine[k].dThik + dR_J*SectJ.aLine[k].dThik );
			SectBaseD.aLine[k].dVF   = ( dR_I*SectI.aLine[k].dVF   + dR_J*SectJ.aLine[k].dVF );
		}

		// GeneralPart
		if ( SectI.aGeneralPart.GetSize() != SectJ.aGeneralPart.GetSize() )
			return SectBaseD;
		int nGeneralPart = SectBaseD.aGeneralPart.GetSize();
		for ( k=0; k<nGeneralPart; k++ )
		{
			SectBaseD.aGeneralPart[k].Stiffness    = GetSectStiffness_Pos(SectI.aGeneralPart[k].Stiffness, SectJ.aGeneralPart[k].Stiffness, dPos);
			SectBaseD.aGeneralPart[k].StiffnessSum = GetSectStiffness_Pos(SectI.aGeneralPart[k].StiffnessSum, SectJ.aGeneralPart[k].StiffnessSum, dPos);

			SectBaseD.aGeneralPart[k].PeriIn        = ( dR_I*SectI.aGeneralPart[k].PeriIn        + dR_J*SectJ.aGeneralPart[k].PeriIn );
			SectBaseD.aGeneralPart[k].PeriOut       = ( dR_I*SectI.aGeneralPart[k].PeriOut       + dR_J*SectJ.aGeneralPart[k].PeriOut );
			SectBaseD.aGeneralPart[k].PeriInSum     = ( dR_I*SectI.aGeneralPart[k].PeriInSum     + dR_J*SectJ.aGeneralPart[k].PeriInSum );
			SectBaseD.aGeneralPart[k].PeriOutSum    = ( dR_I*SectI.aGeneralPart[k].PeriOutSum    + dR_J*SectJ.aGeneralPart[k].PeriOutSum );
			SectBaseD.aGeneralPart[k].YBar          = ( dR_I*SectI.aGeneralPart[k].YBar          + dR_J*SectJ.aGeneralPart[k].YBar );
			SectBaseD.aGeneralPart[k].ZBar          = ( dR_I*SectI.aGeneralPart[k].ZBar          + dR_J*SectJ.aGeneralPart[k].ZBar );
			SectBaseD.aGeneralPart[k].YBarSum       = ( dR_I*SectI.aGeneralPart[k].YBarSum       + dR_J*SectJ.aGeneralPart[k].YBarSum );
			SectBaseD.aGeneralPart[k].ZBarSum       = ( dR_I*SectI.aGeneralPart[k].ZBarSum       + dR_J*SectJ.aGeneralPart[k].ZBarSum );
			SectBaseD.aGeneralPart[k].dDencityRatio = ( dR_I*SectI.aGeneralPart[k].dDencityRatio + dR_J*SectJ.aGeneralPart[k].dDencityRatio );
			SectBaseD.aGeneralPart[k].dElastic      = ( dR_I*SectI.aGeneralPart[k].dElastic      + dR_J*SectJ.aGeneralPart[k].dElastic );
			SectBaseD.aGeneralPart[k].dPoisson      = ( dR_I*SectI.aGeneralPart[k].dPoisson      + dR_J*SectJ.aGeneralPart[k].dPoisson );
		}
	}
	return SectBaseD;
}

T_SECT_SECTBASE_D CDb_DllManager_Base::GetSectBaseD_For_ElemK(T_ELEM_K ElemK, int nIMJ, BOOL bDesign, int MatlType)
{
	T_SECT_D SectD;
	SectD.Initialize();

	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD.Initialize();

	if ( !GetSectD_For_ElemK(ElemK, nIMJ, SectD, bDesign, MatlType) ) return SectBaseD;

	return GetSectBaseD(SectD, nIMJ);
}

T_SECT_SECTBASE_D CDb_DllManager_Base::GetSectBaseD_For_SectK(T_SECT_K SectK, int nIMJ, BOOL bDesign)
{
	T_SECT_D SectD;
	SectD.Initialize();
	T_SECT_SECTBASE_D SectBaseD;
	SectBaseD.Initialize();

	if ( bDesign ) { if ( !m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD) )	return SectBaseD; }
	else { if ( !m_pDoc->m_pAttrCtrl->GetSect(SectK, SectD) )	return SectBaseD; }

	return GetSectBaseD(SectD, nIMJ);
}

BOOL CDb_DllManager_Base::GetSectD_For_ElemK(T_ELEM_K ElemK, int nIMJ, T_SECT_D& SectD, BOOL bDesign, int MatlType)
{
	SectD.Initialize();

	T_ELEM_D ElemD;  ElemD.Initialize();
	if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) )	return FALSE;
	if ( MatlType == 0 )
	{
		T_MATL_D MatlD;
		if ( !m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD) )	return FALSE;

		if ( MatlD.Type == "C" ) MatlType = 1;
		else if ( MatlD.Type == "S" ) MatlType = 2;
		else return FALSE;
	}

	if ( MatlType == 2 )
	{
		if ( bDesign ) { if ( !Get_DgnStlSect(ElemK, SectD) ) return FALSE; }
		else { if ( !m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD) )	return FALSE; }
	}
	else if ( MatlType == 1 )
	{
		if ( m_pDoc->m_pAttrCtrl->GetElemTsgrKey(ElemK) != 0 )
		{
			if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD) ) return FALSE;
			if ( !m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro, SectD) ) return FALSE;
		}
		else
		{
			T_SECT_K SectK = ElemD.elpro;
			if ( bDesign ) { if ( !m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD) )	return FALSE; }
			else { if ( !m_pDoc->m_pAttrCtrl->GetSect(SectK, SectD) )	return FALSE; }
		}
	}
	else { ASSERT(0);  return FALSE; }

	return TRUE;
}

T_SECT_STIFFNESS  CDb_DllManager_Base::GetSectStiffness_Pos(T_SECT_STIFFNESS& StiffI, T_SECT_STIFFNESS& StiffJ, double dPos)
{
	T_SECT_STIFFNESS StiffD;  StiffD.Initialize();

	if ( dPos <= 0.0 ) // I단
		StiffD = StiffI;
	else if ( dPos >= 1.0 ) // J단
		StiffD = StiffJ;
	else if ( dPos > 0.0 && dPos<1.0 ) // M단
	{
		StiffD = StiffI;
		double dR_I = 1.0-dPos, dR_J = dPos;

		// Stiffness 평균
		StiffD.Area =( dR_I*StiffI.Area +dR_J*StiffJ.Area );
		StiffD.Asy  =( dR_I*StiffI.Asy  +dR_J*StiffJ.Asy );
		StiffD.Asz  =( dR_I*StiffI.Asz  +dR_J*StiffJ.Asz );
		StiffD.Rxx  =( dR_I*StiffI.Rxx  +dR_J*StiffJ.Rxx );
		StiffD.Ryy  =( dR_I*StiffI.Ryy  +dR_J*StiffJ.Ryy );
		StiffD.Rzz  =( dR_I*StiffI.Rzz  +dR_J*StiffJ.Rzz );
		StiffD.Cyp  =( dR_I*StiffI.Cyp  +dR_J*StiffJ.Cyp );
		StiffD.Cym  =( dR_I*StiffI.Cym  +dR_J*StiffJ.Cym );
		StiffD.Czp  =( dR_I*StiffI.Czp  +dR_J*StiffJ.Czp );
		StiffD.Czm  =( dR_I*StiffI.Czm  +dR_J*StiffJ.Czm );
		StiffD.Qyb  =( dR_I*StiffI.Qyb  +dR_J*StiffJ.Qyb );
		StiffD.Qzb  =( dR_I*StiffI.Qzb  +dR_J*StiffJ.Qzb );
		StiffD.WArea=( dR_I*StiffI.WArea+dR_J*StiffJ.WArea );
		StiffD.dx1  =( dR_I*StiffI.dx1  +dR_J*StiffJ.dx1 );
		StiffD.dx2  =( dR_I*StiffI.dx2  +dR_J*StiffJ.dx2 );
		StiffD.dx3  =( dR_I*StiffI.dx3  +dR_J*StiffJ.dx3 );
		StiffD.dx4  =( dR_I*StiffI.dx4  +dR_J*StiffJ.dx4 );
		StiffD.dy1  =( dR_I*StiffI.dy1  +dR_J*StiffJ.dy1 );
		StiffD.dy2  =( dR_I*StiffI.dy2  +dR_J*StiffJ.dy2 );
		StiffD.dy3  =( dR_I*StiffI.dy3  +dR_J*StiffJ.dy3 );
		StiffD.dy4  =( dR_I*StiffI.dy4  +dR_J*StiffJ.dy4 );
	}
	return StiffD;
}
T_SECT_DESIGN     CDb_DllManager_Base::GetSectDesign_Pos(T_SECT_DESIGN& DgnI, T_SECT_DESIGN& DgnJ, double dPos)
{
	T_SECT_DESIGN DgnD;  DgnD.Initialize();

	if ( dPos <= 0.0 ) // I단
		DgnD = DgnI;
	else if ( dPos >= 1.0 ) // J단
		DgnD = DgnJ;
	else if ( dPos > 0.0 && dPos<1.0 ) // M단
	{
		DgnD = DgnI;
		double dR_I = 1.0-dPos, dR_J = dPos;

		// Desgin 평균
		DgnD.YBar    =( dR_I*DgnI.YBar    +dR_J*DgnJ.YBar );
		DgnD.ZBar    =( dR_I*DgnI.ZBar    +dR_J*DgnJ.ZBar );
		DgnD.Syy     =( dR_I*DgnI.Syy     +dR_J*DgnJ.Syy );
		DgnD.Szz     =( dR_I*DgnI.Szz     +dR_J*DgnJ.Szz );
		DgnD.Zyy     =( dR_I*DgnI.Zyy     +dR_J*DgnJ.Zyy );
		DgnD.Zzz     =( dR_I*DgnI.Zzz     +dR_J*DgnJ.Zzz );
		DgnD.Roy     =( dR_I*DgnI.Roy     +dR_J*DgnJ.Roy );
		DgnD.Roz     =( dR_I*DgnI.Roz     +dR_J*DgnJ.Roz );
		DgnD.Cwp     =( dR_I*DgnI.Cwp     +dR_J*DgnJ.Cwp );
		DgnD.TanA    =( dR_I*DgnI.TanA    +dR_J*DgnJ.TanA );
		DgnD.Rop     =( dR_I*DgnI.Rop     +dR_J*DgnJ.Rop );
		DgnD.XO      =( dR_I*DgnI.XO      +dR_J*DgnJ.XO );
		DgnD.RO      =( dR_I*DgnI.RO      +dR_J*DgnJ.RO );
		DgnD.Beta    =( dR_I*DgnI.Beta    +dR_J*DgnJ.Beta );
	}
	return DgnD;
}

BOOL CDb_DllManager_Base::Find_nStype(CString strShape, int &nStype)
{
	nStype = D_SECT_TYPE_REGULAR;
	if ( CSectUtil::GetShapeIndexFromNameReg(strShape) >= 0 ) nStype = D_SECT_TYPE_REGULAR;
	else if ( CSectUtil::GetShapeIndexFromNameSrc(strShape) >= 0 ) nStype = D_SECT_TYPE_SRC;
	else if ( CSectUtil::GetShapeIndexFromNamePsc(strShape) >= 0 ) nStype = D_SECT_TYPE_PSC;
	else if ( CSectUtil::GetShapeIndexFromNameCom(strShape) >= 0 ) nStype = D_SECT_TYPE_COMBINED;
	else if ( CSectUtil::GetShapeIndexFromNameCfm(strShape) >= 0 ) nStype = D_SECT_TYPE_COLDFORMED;
	else if ( CSectUtil::GetShapeIndexFromNameCfm(strShape) >= 0 ) nStype = D_SECT_TYPE_COLDFORMED;
	else if ( strShape == D_SECT_SHAPE_COMPO_B ) nStype = D_SECT_TYPE_COMPO_B;
	else if ( strShape == D_SECT_SHAPE_COMPO_I ) nStype = D_SECT_TYPE_COMPO_I;
	else if ( strShape == D_SECT_SHAPE_COMPO_TUB ) nStype = D_SECT_TYPE_COMPO_TUB;
	else if ( strShape == D_SECT_SHAPE_COMPO_STLG_B ) nStype = D_SECT_TYPE_COMPO_STLG_B;
	else if ( strShape == D_SECT_SHAPE_COMPO_STLG_I ) nStype = D_SECT_TYPE_COMPO_STLG_I;
	else if ( strShape == D_SECT_SHAPE_COMPO_STLG_TUB ) nStype = D_SECT_TYPE_COMPO_STLG_TUB;
	else if ( strShape == D_SECT_SHAPE_COMPO_CI ) nStype = D_SECT_TYPE_COMPO_CI;
	else if ( strShape == D_SECT_SHAPE_COMPO_CT ) nStype = D_SECT_TYPE_COMPO_CT;
	else if ( strShape == D_SECT_SHAPE_COMPO_PC ) nStype = D_SECT_TYPE_COMPO_PC;
	else if ( strShape == D_SECT_SHAPE_COMPO_G ) nStype = D_SECT_TYPE_COMPO_G;
	else return FALSE;

	return TRUE;
}
// Data Convert ////////////////////////////////////////////////////////////////////////

BOOL CDb_DllManager_Base::ConvertToDLL_Stiffness(T_SECT_D& SectD, int nIMJ, DGN_SECT_STIFFNESS& OutData, BOOL bDesign, BOOL bAfter)
{
	// bAfter 사용여부 검토
	if ( !IsAfterSection(SectD) ) bAfter = FALSE;

	int nSType;
	T_SECT_SECTBASE_D SectBaseD = GetSectBaseD(SectD, nSType, nIMJ, bAfter);
	return ConvertToDLL_Stiffness(SectBaseD.Stiffness, SectBaseD.Design, OutData);
}

BOOL CDb_DllManager_Base::ConvertToDLL_Stiffness(T_SECT_STIFFNESS& StiffnessD, T_SECT_DESIGN& DesignD, DGN_SECT_STIFFNESS& OutData)
{
	OutData.Initialize();

	// Stiffness의 Cym, Czm값은 SRC의 경우 Steel을 기준으로 Composite의 경우 Girder를 기준으로 값을 가지므로 단면 전체의 도심을 가르키지 않음
	OutData.dYBar = DesignD.YBar;
	OutData.dZBar = DesignD.ZBar;

	OutData.dRoy  = DesignD.Roy;
	OutData.dRoz  = DesignD.Roz;

	OutData.dArea = StiffnessD.Area;
	OutData.dIyy  = StiffnessD.Ryy;
	OutData.dIzz  = StiffnessD.Rzz;
	OutData.dAsy  = StiffnessD.Asy;
	OutData.dAsz  = StiffnessD.Asz;
	OutData.dIxx  = StiffnessD.Rxx;
	OutData.dCyp  = StiffnessD.Cyp;
	OutData.dCym  = StiffnessD.Cym;
	OutData.dCzp  = StiffnessD.Czp;
	OutData.dCzm  = StiffnessD.Czm;
	OutData.dQyb  = StiffnessD.Qyb;
	OutData.dQzb  = StiffnessD.Qzb;
	OutData.dx1   = StiffnessD.dx1;
	OutData.dx2   = StiffnessD.dx2;
	OutData.dx3   = StiffnessD.dx3;
	OutData.dx4   = StiffnessD.dx4;
	OutData.dy1   = StiffnessD.dy1;
	OutData.dy2   = StiffnessD.dy2;
	OutData.dy3   = StiffnessD.dy3;
	OutData.dy4   = StiffnessD.dy4;

	return TRUE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_SectData(T_SECT_D& SectD, int nIMJ, DGN_GSEC_SHAPE_D& OutData, BOOL bDesign, BOOL bAfter)
{
	if ( nIMJ == 0 ) return ConvertToDLL_SectData_Pos(SectD, 0, OutData, bDesign, bAfter); // I단
	else if ( nIMJ == 2 ) return ConvertToDLL_SectData_Pos(SectD, 4, OutData, bDesign, bAfter); // J단
	else if ( nIMJ == 1 ) return ConvertToDLL_SectData_Pos(SectD, 2, OutData, bDesign, bAfter); // M단

	return FALSE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_SectData_CenterPos(T_SECT_D& SectD, double dPos, double& dYBar, double& dZBar)
{
	// bAfter 사용여부 검토
	BOOL bAfter=TRUE;
	if ( !IsAfterSection(SectD) ) bAfter = FALSE;

	int nSType;
	T_SECT_SECTBASE_D SectBaseD = GetSectBaseD_Pos(SectD, nSType, dPos, bAfter);

	// Stiffness의 Cym, Czm값은 SRC의 경우 Steel을 기준으로 Composite의 경우 Girder를 기준으로 값을 가지므로 단면 전체의 도심을 가르키지 않음
	dYBar = SectBaseD.Design.YBar;
	dZBar = SectBaseD.Design.ZBar;

	return TRUE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_SectData_Pos(T_SECT_D& SectD, int nPos, DGN_GSEC_SHAPE_D& OutData, BOOL bDesign, BOOL bAfter)
{
	double dPos = 0.0;
	if ( nPos == 0 ) dPos = 0.0;
	else if ( nPos == 1 ) dPos = 1.0/4.0;
	else if ( nPos == 2 ) dPos = 2.0/4.0;
	else if ( nPos == 3 ) dPos = 3.0/4.0;
	else if ( nPos == 4 ) dPos = 1.0;
	else return FALSE;

	return ConvertToDLL_SectData_Pos(SectD, dPos, OutData, bDesign, bAfter);
}
BOOL CDb_DllManager_Base::ConvertToDLL_SectData_Pos(T_SECT_D& SectD, double dPos, DGN_GSEC_SHAPE_D& OutData, BOOL bDesign, BOOL bAfter)
{
	OutData.Initialize();

	// bAfter 사용여부 검토
	if ( !IsAfterSection(SectD) ) bAfter = FALSE;

	BOOL bDumb = m_pDoc->m_pSectDB->IsSectDumbAndStub(SectD);
	if (bDumb)
	{
		bAfter = TRUE;

		// Polygon 데이터 세팅
		if (SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_DUMB)
			m_pDoc->m_pAttrCtrl->GetSrcDumbbellPolygonData(SectD, dPos, 2);  // Out :4 In:2
		else if (SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_STUB)
			m_pDoc->m_pAttrCtrl->GetSrcSingletubePolygonData(SectD, dPos);   // Out :1 In:1
		else
			ASSERT(0);
	}

	int nSType;
	T_SECT_SECTBASE_D SectBaseD = GetSectBaseD_Pos(SectD, nSType, dPos, bAfter);
	OutData.nType = ConvertToDLL_Stype(nSType);
	if ( SectBaseD.Shape == D_SECT_SHAPE_REG_GEN ) OutData.nType = DGN_SECT_TYPE_USER;

	int i, nSize;
	OutData.Shape = CT2W(ConvertToDLL_ShapeName(SectBaseD.Shape));          // 단면형상
	nSize = min(DGN_SECT_SIZE_NUM_MAX, D_SECT_SIZE_NUM_MAX);
	for ( i=0; i<nSize; i++ )
		OutData.Size[i] = SectBaseD.Size[i];   // 단면치수 ([0]:H, [1]:B)

	// Stiffness의 Cym, Czm값은 SRC의 경우 Steel을 기준으로 Composite의 경우 Girder를 기준으로 값을 가지므로 단면 전체의 도심을 가르키지 않음
	OutData.dYBar = SectBaseD.Design.YBar;
	OutData.dZBar = SectBaseD.Design.ZBar;

	for ( i=0; i<4; i++ ) OutData.nStiffNum[i] = bAfter ? SectD.SectAfter.nStiffNum[i] : SectD.SectBefore.nStiffNum[i];

	DGN_GSEC_VERTEX ChkPoint;
	OutData.arChkPoint.SetSize(4);
	ChkPoint.dX = SectBaseD.Stiffness.dx1;  ChkPoint.dY = SectBaseD.Stiffness.dy1;  OutData.arChkPoint.SetAt(0, ChkPoint);
	ChkPoint.dX = SectBaseD.Stiffness.dx2;  ChkPoint.dY = SectBaseD.Stiffness.dy2;  OutData.arChkPoint.SetAt(1, ChkPoint);
	ChkPoint.dX = SectBaseD.Stiffness.dx3;  ChkPoint.dY = SectBaseD.Stiffness.dy3;  OutData.arChkPoint.SetAt(2, ChkPoint);
	ChkPoint.dX = SectBaseD.Stiffness.dx4;  ChkPoint.dY = SectBaseD.Stiffness.dy4;  OutData.arChkPoint.SetAt(3, ChkPoint);

	//Gen형식
	if ( SectBaseD.Shape == D_SECT_SHAPE_REG_GEN )
	{
		OutData.Size[0] = fabs(SectBaseD.Stiffness.Czp) + fabs(SectBaseD.Stiffness.Czm);   // 단면치수 ([0]:H, [1]:B)
		OutData.Size[1] = fabs(SectBaseD.Stiffness.Cyp) + fabs(SectBaseD.Stiffness.Cym);
	}
	DGN_GSEC_POLYGON DrawPolygon;
	DGN_GSEC_LINE    DrawLine;
	DGN_GSEC_PART    DrawPart;
	nSize = SectBaseD.aOuterPolygon.GetSize();
	OutData.arOutPolyData.SetSize(nSize);
	for ( i=0; i<nSize; i++ )
	{
		if ( !ConvertToDLL_Polygon(SectBaseD.aOuterPolygon[i], DrawPolygon) ) DrawPolygon.Initialize();
		OutData.arOutPolyData.SetAt(i, DrawPolygon);
	}
	nSize = SectBaseD.aInnerPolygon.GetSize();
	OutData.arInPolyData.SetSize(nSize);
	for ( i=0; i<nSize; i++ )
	{
		if ( !ConvertToDLL_Polygon(SectBaseD.aInnerPolygon[i], DrawPolygon) ) DrawPolygon.Initialize();
		OutData.arInPolyData.SetAt(i, DrawPolygon);
	}
	nSize = SectBaseD.aLine.GetSize();
	OutData.arLineData.SetSize(nSize);
	for ( i=0; i<nSize; i++ )
	{
		if ( !ConvertToDLL_Line(SectBaseD.aLine[i], DrawLine) ) DrawLine.Initialize();
		OutData.arLineData.SetAt(i, DrawLine);
	}

	// PSC
	OutData.BuiltUpFlag = SectBaseD.BuiltUpFlag;
	OutData.nJoint      = bAfter ? SectD.SectAfter.nJoint : SectD.SectBefore.nJoint;
	OutData.nCellType   = bAfter ? SectD.SectAfter.nCellType : SectD.SectBefore.nCellType;
	OutData.nCellShape  = bAfter ? SectD.SectAfter.nCellShape : SectD.SectBefore.nCellShape;
	OutData.bSymmetric  = bAfter ? SectD.SectAfter.bSymmetric : SectD.SectBefore.bSymmetric;
	OutData.bSmallHole  = bAfter ? SectD.SectAfter.bSmallHole : SectD.SectBefore.bSmallHole;

	if ( SectBaseD.Shape == D_SECT_SHAPE_COMPO_CI || SectBaseD.Shape == D_SECT_SHAPE_COMPO_CT )
	{
		OutData.nJoint = SectD.SectBefore.nJoint;
	}

	if ( SectBaseD.Shape == D_SECT_SHAPE_REG_CC )
	{
		CString strShape = SectD.SectBefore.SectI.Shape;

		if ( strShape.IsEmpty() )
		{
			OutData.nCellType = 1;
		}
		else
		{
			OutData.nCellType = _ttoi(strShape);

			CString strI = _T("I");
			CString strB = _T("B");
			if ( strShape.Find(strI) >= 0 ) OutData.nCellShape = 1;
			else if ( strShape.Find(strB) >= 0 ) OutData.nCellShape = 2;
		}
	}

	// SRC
	OutData.Matl_Elast     = bAfter ? SectD.SectAfter.Matl_Elast : SectD.SectBefore.Matl_Elast;
	OutData.Matl_Poisson_S = bAfter ? SectD.SectAfter.Matl_Poisson_S : SectD.SectBefore.Matl_Poisson_S;
	OutData.Matl_Poisson_C = bAfter ? SectD.SectAfter.Matl_Poisson_C : SectD.SectBefore.Matl_Poisson_C;
	// Combined
	if ( OutData.nType == DGN_SECT_TYPE_COMBINED )
	{
		for ( i=0; i<16; i++ ) OutData.SubSize[i] = bAfter ? SectD.SectAfter.SectJ.Size[i] : SectD.SectBefore.SectJ.Size[i];
	}
	else if ( OutData.nType == DGN_SECT_TYPE_SRC )
	{
		for ( i=0; i<16; i++ ) OutData.SubSize[i] = bAfter ? SectD.SectAfter.SectJ.Size[i] : SectD.SectBefore.SectJ.Size[i];
	}
	else if ( OutData.nType == DGN_SECT_TYPE_COMPO_TUB || OutData.nType == DGN_SECT_TYPE_COMPO_B || OutData.nType == DGN_SECT_TYPE_COMPO_I || OutData.nType == DGN_SECT_TYPE_COMPO_CI || OutData.nType == DGN_SECT_TYPE_COMPO_CT || OutData.nType == DGN_SECT_TYPE_COMPO_PC )
	{
		OutData.SubSize[0] = SectD.SectAfter.SectJ.Size[0];
		OutData.SubSize[1] = SectD.SectAfter.SectJ.Size[1];
		OutData.SubSize[2] = SectD.SectAfter.SectJ.Size[2];
		OutData.SubSize[3] = SectD.SectAfter.SectJ.Size[3];
	}
	else if ( OutData.nType == DGN_SECT_TYPE_COMPO_STLG_I || OutData.nType == DGN_SECT_TYPE_COMPO_STLG_B || OutData.nType == DGN_SECT_TYPE_COMPO_STLG_TUB )
	{
		OutData.SubSize[0] = SectD.SectAfter.SectJ.Size[0];
		OutData.SubSize[1] = SectD.SectAfter.SectJ.Size[1];
		OutData.SubSize[2] = SectD.SectAfter.SectJ.Size[2];
		OutData.SubSize[3] = SectD.SectAfter.SectI.Size[3];
		OutData.SubSize[4] = SectD.SectAfter.SectI.Size[4];
		OutData.SubSize[5] = SectD.SectAfter.SectI.Size[5];
		ConvertToDLL_SectData_SODStiffener(SectD.SectBefore.SectI.Stiffness, SectBaseD.SODStiffener, OutData.SODStiffener);
	}
	else if ( OutData.nType == DGN_SECT_TYPE_STL_GIRDER_B || OutData.nType == DGN_SECT_TYPE_STL_GIRDER_I || OutData.nType == DGN_SECT_TYPE_STLG_MCELL )
	{
		if ( SectD.nStype==D_SECT_TYPE_TAPERED )
		{
			T_SECT_SECTBASE_D SectBaseDA, SectBaseIA, SectBaseJA;
			SectBaseIA  = SectD.SectAfter.SectI;
			SectBaseJA  = SectD.SectAfter.SectJ;
			SectBaseDA = GetSectBaseD_Pos(SectBaseIA, SectBaseJA, dPos);
			OutData.SubSize[0] = SectBaseDA.Size[0];
			OutData.SubSize[1] = SectBaseDA.Size[1];
		}
		else
		{
			OutData.SubSize[0] = SectD.SectAfter.SectI.Size[0];
			OutData.SubSize[1] = SectD.SectAfter.SectI.Size[1];
		}
		ConvertToDLL_SectData_SODStiffener(SectD.SectBefore.SectI.Stiffness, SectBaseD.SODStiffener, OutData.SODStiffener);
		ConvertToDLL_SectData_Rivet       (SectD.SectBefore.SectI.Stiffness, SectBaseD.RivetStiff  , OutData.RivetStiff);
	}
	OutData.bAfter = bAfter;
	OutData.nBeforePart = SectBaseD.nBeforePart;
	nSize = SectBaseD.aGeneralPart.GetSize();
	OutData.aGeneralPart.SetSize(nSize);
	for ( i=0; i<nSize; i++ )
	{
		if ( !ConvertToDLL_Part(SectBaseD.aGeneralPart[i], DrawPart) ) DrawPart.Initialize();
		OutData.aGeneralPart.SetAt(i, DrawPart);
	}

	// ※주의 : nStiffNum은 GENw v730이후에 만들어진 변수로 이전에는 REGULAR의 경우 DGN_SECT_SHAPE_REG_BSTF와 DGN_SECT_SHAPE_REG_PSTF와
	//          SRC의 경우 DGN_SECT_SHAPE_SRC_BSTF와 DGN_SECT_SHAPE_SRC_PSTF의 경우  nCellShape, nCellType항목을 Stiffner갯수로 사용하고 있었습니다.
	//          따라서 이 부분을 nStiffNum에 값을 채워줘서 DgnEngine으로 넘겨줍니다.(by SHIN 10.02.19)
	if ( OutData.nType == DGN_SECT_TYPE_REGULAR || OutData.nType == DGN_SECT_TYPE_SRC )
	{
		OutData.nStiffNum[0] = OutData.nCellShape;
		OutData.nStiffNum[1] = OutData.nCellType;
	}

	if ( OutData.nType == DGN_SECT_TYPE_STLG_MCELL )
	{
		OutData.bSymmetric = SectD.SectBefore.bSymmetric;
		OutData.nCellType = SectD.SectBefore.nCellType;
		OutData.nCellNum = SectD.SectBefore.nCellNum;
		OutData.nFlangeShape = SectD.SectBefore.nFlangeShape;
		OutData.nJoint = SectD.SectBefore.nJoint;
	}

	OutData.bAfter = bAfter;
	return TRUE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_SectData_SODStiffener(T_SECT_STIFFNESS& Stiffness, T_SECT_SOD_STIFF_D& SODStiffenerIn, ST_SECT_SOD_STIFF_D& SODStiffenerOut)
{
	SODStiffenerOut.dCyp = Stiffness.Cyp;
	SODStiffenerOut.dCym = Stiffness.Cym;
	SODStiffenerOut.dCzp = Stiffness.Czp;
	SODStiffenerOut.dCzm = Stiffness.Czm;

	//
	int nStiffShapeNum =  SODStiffenerIn.aStiffShape.GetSize();
	SODStiffenerOut.aStiffShape.SetSize(nStiffShapeNum);

	ST_STIFF_SHAPE_D StiffShapeOut;
	for ( int i=0; i< nStiffShapeNum; ++i )
	{
		StiffShapeOut.Initialize();
		T_STIFF_SHAPE_D& StiffShapeIn = SODStiffenerIn.aStiffShape.GetAt(i);

		StiffShapeOut.strName = CT2W(StiffShapeIn.strName);
		StiffShapeOut.nType  = StiffShapeIn.nType;    // 0:I, 1:T, 2:U-Rib
		StiffShapeOut.dSize[8];
		memcpy(StiffShapeOut.dSize, StiffShapeIn.dSize, sizeof(StiffShapeOut.dSize));

		SODStiffenerOut.aStiffShape.SetAt(i, StiffShapeOut);
	}

	//
	int nSODStiffSubNum =  SODStiffenerIn.aSODStiffSub.GetSize();
	SODStiffenerOut.aSODStiffSub.SetSize(nSODStiffSubNum);

	ST_SECT_SOD_STIFF_SUB_D SODStiffSubOut;
	for ( int i = 0; i< nSODStiffSubNum; ++i )
	{
		T_SECT_SOD_STIFF_SUB_D& SODStiffSubIn = SODStiffenerIn.aSODStiffSub.GetAt(i);

		SODStiffSubOut.Initialize();
		SODStiffSubOut.nDeckPos    = SODStiffSubIn.nDeckPos;
		SODStiffSubOut.nDeckPart   = SODStiffSubIn.nDeckPart;
		SODStiffSubOut.strDeckName = SODStiffSubIn.strDeckName;
		SODStiffSubOut.nRefPos     = SODStiffSubIn.nRefPos;
		SODStiffSubOut.nNum        = SODStiffSubIn.nNum;

		ST_SECT_SOD_STIFF_DATA_D StiffDataOut;
		int nStiffNum = SODStiffSubIn.aStiffData.GetSize();
		SODStiffSubOut.aStiffData.SetSize(nStiffNum);
		for ( int j=0; j<nStiffNum; ++j )
		{
			T_SECT_SOD_STIFF_DATA_D& StiffDataIn =  SODStiffSubIn.aStiffData.GetAt(j);

			StiffDataOut.Initialize();
			StiffDataOut.bCalc        = StiffDataIn.bCalc;
			StiffDataOut.dSpacing     = StiffDataIn.dSpacing;
			StiffDataOut.strShapeName  = CT2W(StiffDataIn.strShapeName); // T_SECT_SOD_STIFF_D 구조체 aStiffShape 의 Index
			StiffDataOut.nStiffPos    = StiffDataIn.nStiffPos;   // 0:Left/Top 1:Right/Bottom 2:Both
			StiffDataOut.strStiffName = StiffDataIn.strStiffName;
			StiffDataOut.nStiffDir    = StiffDataIn.nStiffDir;
			SODStiffSubOut.aStiffData.SetAt(j, StiffDataOut);
		}

		SODStiffenerOut.aSODStiffSub.SetAt(i, SODStiffSubOut);
	}

	return TRUE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_SectData_Rivet(T_SECT_STIFFNESS& Stiffness, T_SECT_RIVET_STIFF_D& RivetIn, ST_SECT_RIVET_STIFF_D& RivetOut)
{
	RivetOut.dCyp = Stiffness.Cyp;
	RivetOut.dCym = Stiffness.Cym;
	RivetOut.dCzp = Stiffness.Czp;
	RivetOut.dCzm = Stiffness.Czm;

	RivetOut.bSymmetric = RivetIn.bSymmetric;
	RivetOut.bRivet     = RivetIn.bRivet;

	//
	int nStiffShapeNum = RivetIn.aStiffShape.GetSize();
	RivetOut.aStiffShape.SetSize(nStiffShapeNum);

	ST_STIFF_SHAPE_D StiffShapeOut;
	for (int i = 0; i < nStiffShapeNum; ++i)
	{
		StiffShapeOut.Initialize();
		T_STIFF_SHAPE_D& StiffShapeIn = RivetIn.aStiffShape.GetAt(i);

		StiffShapeOut.strName = CT2W(StiffShapeIn.strName);
		StiffShapeOut.nType = StiffShapeIn.nType;
		StiffShapeOut.dSize[8];
		memcpy(StiffShapeOut.dSize, StiffShapeIn.dSize, sizeof(StiffShapeOut.dSize));

		RivetOut.aStiffShape.SetAt(i, StiffShapeOut);
	}

	//
	int nPosNum = RivetIn.aStiffPosD.GetSize();
	RivetOut.aStiffPosD.SetSize(nPosNum);

	ST_SECT_STIFF_POS_D StiffPosOut;
	for (int i = 0; i < nPosNum; ++i)
	{
		StiffPosOut.Initialize();
		T_SECT_STIFF_POS_D& StiffPosIn = RivetIn.aStiffPosD.GetAt(i);

		StiffPosOut.bUseStiff    = StiffPosIn.bUseStiff;
		StiffPosOut.strShapeName = CT2W(StiffPosIn.strShapeName);
		
		RivetOut.aStiffPosD.SetAt(i, StiffPosOut);
	}

	//
	int nRivetNum = RivetIn.aRivetPosD.GetSize();
	RivetOut.aRivetPosD.SetSize(nRivetNum);

	ST_SECT_RIVET_POS_D RivetPosOut;
	for (int i = 0; i < nRivetNum; ++i)
	{
		RivetPosOut.Initialize();
		T_SECT_RIVET_POS_D& RivetPosIn = RivetIn.aRivetPosD.GetAt(i);

		RivetPosOut.nComponent = RivetPosIn.nComponent;
		RivetPosOut.nAngle     = RivetPosIn.nAngle    ; 
		RivetPosOut.nRefPos    = RivetPosIn.nRefPos   ; 
		RivetPosOut.dDist      = RivetPosIn.dDist     ;
		RivetPosOut.dDia       = RivetPosIn.dDia      ;

		RivetOut.aRivetPosD.SetAt(i, RivetPosOut);
	}
	return TRUE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_RebarData(T_SECT_D& SectD, int nIMJ, T_RCHK_COLM& RebarD, DGN_GSEC_REBAR_COLUMN& OutData)
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분
	// 일단 0 으로 고정해두었다~

	OutData.Initialize();

	DGN_GSEC_MBAR_COLUMN MbarD; MbarD.Initialize();
	for ( int i=0; i<5; i++ )
	{
		if ( RebarD.dDc[0][i] <= 0.0 ) break;
		MbarD.Initialize();
		MbarD.dDc = RebarD.dDc[0][i];
		MbarD.iNum1 = RebarD.iBarNum[0][0][i];
		MbarD.iNum2 = RebarD.iBarNum[0][1][i];
		MbarD.iNum3 = RebarD.iBarNum[0][2][i];

		MbarD.dDia1[0] = ( RebarD.strBarNa1[0][0][i] == "" ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.strBarNa1[0][0][i]) );
		MbarD.dDia2[0] = ( RebarD.strBarNa1[0][1][i] == "" ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.strBarNa1[0][1][i]) );
		MbarD.dDia3[0] = ( RebarD.strBarNa1[0][2][i] == "" ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.strBarNa1[0][2][i]) );
		MbarD.dDia1[1] = ( RebarD.strBarNa2[0][0][i] == "" ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.strBarNa2[0][0][i]) );
		MbarD.dDia2[1] = ( RebarD.strBarNa2[0][1][i] == "" ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.strBarNa2[0][1][i]) );
		MbarD.dDia3[1] = ( RebarD.strBarNa2[0][2][i] == "" ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.strBarNa2[0][2][i]) );

		MbarD.dArea1[0] = ( RebarD.strBarNa1[0][0][i] == "" ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.strBarNa1[0][0][i]) );
		MbarD.dArea2[0] = ( RebarD.strBarNa1[0][1][i] == "" ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.strBarNa1[0][1][i]) );
		MbarD.dArea3[0] = ( RebarD.strBarNa1[0][2][i] == "" ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.strBarNa1[0][2][i]) );
		MbarD.dArea1[1] = ( RebarD.strBarNa2[0][0][i] == "" ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.strBarNa2[0][0][i]) );
		MbarD.dArea2[1] = ( RebarD.strBarNa2[0][1][i] == "" ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.strBarNa2[0][1][i]) );
		MbarD.dArea3[1] = ( RebarD.strBarNa2[0][2][i] == "" ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.strBarNa2[0][2][i]) );

		OutData.arMainBar.Add(MbarD);
	}

	OutData.StirrupBar.nVNum[0] = RebarD.dSubBarNum_y[0];
	OutData.StirrupBar.nVNum[1] = RebarD.dSubBarNum_z[0];
	OutData.StirrupBar.dSpace   = RebarD.dSubBarDist[0];

	OutData.StirrupBar.dDia  = m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.strSubBarNa[0]);
	OutData.StirrupBar.dArea = m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.strSubBarNa[0]);

	return TRUE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_RebarData(T_SECT_D& SectD, int nIMJ, T_REBT_D& RebarD, DGN_GSEC_REBAR_COLUMN& OutData)
{
	OutData.Initialize();

	T_SECT_SECTBASE_D SectBaseD = GetSectBaseD(SectD, nIMJ);

	double dYBar = SectBaseD.Design.YBar;
	double dZBar = SectBaseD.Design.ZBar;

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분
	int iPosi;
	if ( nIMJ == 0 || RebarD.bIJBothMain )
		iPosi = 0;
	else if ( nIMJ == 2 )
		iPosi = 2;
	else
	{
		ASSERT(0); return FALSE;
	}

	DGN_GSEC_MBAR_GEN MBar;
	T_REBT_MABR       MBarDB;
	int nSize = RebarD.arMainRebar[iPosi].GetSize();
	OutData.arMainBar_Gen.SetSize(nSize);
	for ( int i=0; i<nSize; i++ )
	{
		MBarDB = RebarD.arMainRebar[iPosi].GetAt(i);
		MBar.Initialize();
		MBar.dX    = MBarDB.dCenterY;
		MBar.dY    = MBarDB.dCenterZ;
		MBar.dDia  = m_pDoc->m_pMatlDB->Get_RebarDia(MBarDB.strRebarName);
		MBar.dArea = m_pDoc->m_pMatlDB->Get_RebarArea(MBarDB.strRebarName);

		OutData.arMainBar_Gen.SetAt(i, MBar);
	}

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분
	if ( nIMJ == 0 || RebarD.bIJBothShear )
		iPosi = 0;
	else if ( nIMJ == 2 )
		iPosi = 2;
	else
	{
		ASSERT(0); return FALSE;
	}

	OutData.StirrupBar.nVNum[0] = RebarD.ShearRebar[iPosi].dSubRebarNum[0];
	OutData.StirrupBar.nVNum[1] = RebarD.ShearRebar[iPosi].dSubRebarNum[1];
	OutData.StirrupBar.dSpace   = RebarD.ShearRebar[iPosi].dSubRebarSpace;

	OutData.StirrupBar.dDia  = m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.ShearRebar[iPosi].strSubRebarName);
	OutData.StirrupBar.dArea = m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.ShearRebar[iPosi].strSubRebarName);

	return TRUE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_RebarData_Rebb(int nIMJ, const T_REBB_D &RebbD, DGN_GSEC_REBAR_BEAM& OutData)
{
	OutData.Initialize();

	T_REBB_BASE TopR;
	T_REBB_BASE BotR;
	switch ( nIMJ )
	{
	case 0:
		TopR = RebbD.Top_I;
		BotR = RebbD.Bot_I;
		break;
	case 1:
		TopR = RebbD.Top_C;
		BotR = RebbD.Bot_C;
		break;
	case 2:
		TopR = RebbD.Top_J;
		BotR = RebbD.Bot_J;
		break;
	default: ASSERT(0);	break;
	}

	DGN_GSEC_MBAR_BEAM MbarD;
	OutData.iArrangeType = 0;
	// Top
	int nTopArr = TopR.GetArrayType();
	for ( int i=0; i<nTopArr; ++i )
	{
		MbarD.Initialize();
		MbarD.dNum = ( i==0 ) ? static_cast< double >( TopR.nRebar1 ) : static_cast< double >( TopR.nRebar2 );
		MbarD.dDc  = RebbD.dT;
		MbarD.dDia[0]  = ( TopR.RebarName.IsEmpty() ) ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarDia(TopR.RebarName);
		MbarD.dDia[1]  = ( TopR.RebarName2nd.IsEmpty() ) ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarDia(TopR.RebarName2nd);
		MbarD.dArea[0] = ( TopR.RebarName.IsEmpty() ) ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarArea(TopR.RebarName);
		MbarD.dArea[1] = ( TopR.RebarName2nd.IsEmpty() ) ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarArea(TopR.RebarName2nd);
		OutData.arTopBar.Add(MbarD);
	}

	// Bottom
	int nBotArr = BotR.GetArrayType();
	for ( int i=0; i<nBotArr; ++i )
	{
		MbarD.Initialize();
		MbarD.dNum = ( i==0 ) ? static_cast< double >( BotR.nRebar1 ) : static_cast< double >( BotR.nRebar2 );
		MbarD.dDc  = RebbD.dT;
		MbarD.dDia[0]  = ( BotR.RebarName.IsEmpty() ) ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarDia(BotR.RebarName);
		MbarD.dDia[1]  = ( BotR.RebarName2nd.IsEmpty() ) ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarDia(BotR.RebarName2nd);
		MbarD.dArea[0] = ( BotR.RebarName.IsEmpty() ) ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarArea(BotR.RebarName);
		MbarD.dArea[1] = ( BotR.RebarName2nd.IsEmpty() ) ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarArea(BotR.RebarName2nd);
		OutData.arBotBar.Add(MbarD);
	}

	// Stirrup
	OutData.StirrupBar.dVNum  = static_cast< double >( TopR.iSubRebarNum );
	OutData.StirrupBar.dSpace = TopR.dSubRebarSpace;
	OutData.StirrupBar.dDia   = ( RebbD.SubRebarName.IsEmpty() ) ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarDia(RebbD.SubRebarName);
	OutData.StirrupBar.dArea  = ( RebbD.SubRebarName.IsEmpty() ) ? 0.0 : m_pDoc->m_pMatlDB->Get_RebarArea(RebbD.SubRebarName);

	return TRUE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_RebarData_Rebc(const T_SECT_D& SectD, const T_REBC_D& RebcD, OUT DGN_GSEC_REBAR_COLUMN& rRbar)
{
	rRbar.Initialize();

	int nRbar = RebcD.nQrb;
	rRbar.arMainBar_Gen.SetSize(nRbar);

	/*
	//DB단면
	CArray<DGN_GSEC_MBAR_COLUMN, DGN_GSEC_MBAR_COLUMN> arMainBar; //주철근정보
	//Gen형식
	int iCenterType;//주철근의 좌표중심 (0:도심, 1:좌하단) (default = 0)
	CArray<DGN_GSEC_MBAR_GEN, DGN_GSEC_MBAR_GEN&> arMainBar_Gen;
	// 전단철근
	DGN_GSEC_STBAR_COLUMN StirrupBar;
	*/
	return TRUE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_RebarData_Rebw(double dLw, double dTw, const T_REBW_D& RebwD, OUT DGN_GSEC_REBAR_COLUMN& rRbar)
{
	return TRUE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_Line(T_GSEC_LINE& InData, DGN_GSEC_LINE& OutData)
{
	OutData.Initialize();
	OutData.v1 = InData.v1;
	OutData.v2 = InData.v2;
	OutData.dThik = InData.dThik;
	OutData.nAlign = InData.nAlign;
	return TRUE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_Polygon(T_GSEC_POLYGON& InData, DGN_GSEC_POLYGON& OutData)
{
	OutData.Initialize();
	int nSize = InData.aVertex.GetSize();
	OutData.aVertex.SetSize(nSize);
	DGN_GSEC_VERTEX    VertexUnit;
	for ( int i=0; i<nSize; i++ )
	{
		VertexUnit.Set(InData.aVertex[i].dX, InData.aVertex[i].dY);
		OutData.aVertex.SetAt(i, VertexUnit);
	}
	return TRUE;
}

BOOL CDb_DllManager_Base::ConvertToDLL_Part(T_GSEC_PART& InData, DGN_GSEC_PART& OutData)
{
	OutData.Initialize();

	T_SECT_DESIGN DesignD;  DesignD.Initialize();
	DesignD.YBar = OutData.YBar;     DesignD.ZBar = OutData.ZBar;
	if ( !ConvertToDLL_Stiffness(InData.Stiffness, DesignD, OutData.Stiffness) ) return FALSE;
	DesignD.YBar = OutData.YBarSum;  DesignD.ZBar = OutData.ZBarSum;
	if ( !ConvertToDLL_Stiffness(InData.StiffnessSum, DesignD, OutData.StiffnessSum) ) return FALSE;
	OutData.YBar    = InData.YBar;
	OutData.ZBar    = InData.ZBar;
	OutData.YBarSum = InData.YBarSum;
	OutData.ZBarSum = InData.ZBarSum;
	for ( int i=0; i<5; i++ )
	{
		OutData.IdxStart[i] = InData.IdxStart[i];
		OutData.IdxEnd[i]   = InData.IdxEnd[i];
	}
	return TRUE;
}

CString  CDb_DllManager_Base::ConvertToDLL_ShapeName(CString Shape)
{
	// DLL에서 사용하는 String값으로 변환 (현재08.01.10 wGen과 동일)
	return Shape;
}
int  CDb_DllManager_Base::ConvertToDLL_LengthUnitID(int UnitID)
{
	// DLL에서 사용하는 ID값으로 변환 (현재08.01.10 wGen과 동일)
	return UnitID;
}
int CDb_DllManager_Base::ConvertToDLL_Stype(int nStype)
{
	// DLL에서 사용하는 ID값으로 변환 (현재09.09.11 wGen과 동일)
	return nStype;
}

BOOL CDb_DllManager_Base::Get_DgnStlSect(int key, T_SECT_D& rData)
{
	rData.Initialize();

	CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
	if ( pMembCtrl == NULL ) return FALSE;

	T_ELEM_K Showkey;
	if ( !pMembCtrl->GetShowElemByIncludeElem(key, Showkey) )	return FALSE;

	T_ELEM_D eData;
	eData.Initialize();
	if ( !m_pDoc->m_pAttrCtrl->GetElem(Showkey, eData) )	return FALSE;
	// Skip if Element Type is Wall.
	if ( m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp) )
	{
		// Change by ZINU.('01.10.6). For Group of Tapered Section.
		T_TSGR_K TsgrKey = m_pDoc->m_pAttrCtrl->GetElemTsgrKey(Showkey);
		BOOL bCheck=FALSE;
		if ( TsgrKey > 0 )	bCheck = m_pDoc->calcElemSect(Showkey, rData, 1, TRUE);
		else						bCheck = m_pDoc->m_pAttrCtrl->GetSectDesign(eData.elpro, rData);
		ASSERT(bCheck);
	}
	return TRUE;
}

BOOL CDb_DllManager_Base::IsAfterSection(T_SECT_D& SectD)
{
	// bAfter 사용여부 검토
	int nType = 1;
	T_RSCT_STIFF_D StiffInfo, AfterStiffInfo;
	if ( !m_pDoc->m_pAttrCtrl->GetReportSectStiffInfo(SectD, StiffInfo, &AfterStiffInfo) ) return FALSE;
	nType = StiffInfo.nType;
	if ( nType<1 || nType>7 )
	{
		ASSERT(0);  return FALSE;
	}
	return ( nType == 5 || nType == 7 ); // 5:construction & composite  7:tapered composite
}

int CDb_DllManager_Base::GetMovingTypeToDLL()
{
	int nType = CProduct::GetMovingType();
	if ( nType == D_PRODUCT_MOVING_ORG ) return DGNLANG_KR;
	if ( nType == D_PRODUCT_MOVING_JP ) return DGNLANG_JP;
	if ( nType == D_PRODUCT_MOVING_CH ) return DGNLANG_CH;
	if ( nType == D_PRODUCT_MOVING_US ) return DGNLANG_EN;
	if ( nType == D_PRODUCT_MOVING_RUS ) return DGNLANG_EN;
	if ( nType == D_PRODUCT_MOVING_ALL ) return DGNLANG_KR;
	ASSERT(0);
	return DGNLANG_KR;
}