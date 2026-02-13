// Dgn_DrawManager.cpp: implementation of the CDgn_DrawManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_DrawManager.h"
#include "Dgn_DrawManager_Text.h"
#include "..\wg_db\AttrCtrl.h"

#define   DGN_RBAR_SIZE   4
#define   DGN_RBAR_LAYER  5
#define   DGN_RBAR_LAYER_BEAM  10// Seungjun-20140519 : Russia rebar layer 5->10

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_DrawManager::CDgn_DrawManager()
{

}

CDgn_DrawManager::~CDgn_DrawManager()
{

}

BOOL CDgn_DrawManager::GGet_SectionBoundary(DGN_GSEC_SHAPE_D& SectData, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
{
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;

	//==================================================================================================
	// Explicit Linkage for DLL 
	typedef BOOL (PSECTCALC)(void*, double&, double&, double&, double&, size_t, BOOL&);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "GGet_SectionBoundary");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGN_GSEC_SHAPE_D);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(&SectData, dXmax, dXmin, dYmax, dYmin, Tsize_Sect, bVersionOK);

	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::GDraw_Section(CDC* pDC, DGNDRAW_SECT& SectData, DGNDRAW_SECT_OPTION& pSectOption)
{
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;

	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function Beam Design").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, size_t, size_t, BOOL&);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "GDraw_Section");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_SECT);
	size_t TSize_Option = sizeof(DGNDRAW_SECT_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &SectData, &pSectOption, Tsize_Sect, TSize_Option, bVersionOK);

	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::GDraw_Section(CDC* pDC, DGN_GSEC_SHAPE_D& SectData, DGNDRAW_SECT_OPTION& pSectOption, BOOL& bVersionOK)
{
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;

	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function Beam Design").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, size_t, size_t, BOOL&);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "GDraw_Section");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGN_GSEC_SHAPE_D);
	size_t TSize_Option = sizeof(DGNDRAW_SECT_OPTION);
	
	bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &SectData, &pSectOption, Tsize_Sect, TSize_Option, bVersionOK);

	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_Section(CDC* pDC, DGNDRAW_SECT& SectData, DGNDRAW_EX_SECT_OPTION& pSectOption)
{
#if defined(_MGEN)
	return FALSE;
#endif
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;

	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function Beam Design").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, size_t, size_t, BOOL&);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_Section");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_SECT);
	size_t TSize_Option = sizeof(DGNDRAW_EX_SECT_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &SectData, &pSectOption, Tsize_Sect, TSize_Option, bVersionOK);

	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_Section_Beam(CDC* pDC, DGNDRAW_SECT_BEAM& SectData, DGNDRAW_EX_RCSECT_OPTION& pSectOption)
{
#if defined(_MGEN)
	return FALSE;
#endif
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;

	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function Beam Design").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, size_t, size_t, BOOL&);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_Section_Beam");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_SECT_BEAM);
	size_t TSize_Option = sizeof(DGNDRAW_EX_RCSECT_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &SectData, &pSectOption, Tsize_Sect, TSize_Option, bVersionOK);

	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_Section_Column(CDC* pDC, DGNDRAW_SECT_COLUMN& SectData, DGNDRAW_EX_RCSECT_OPTION& pSectOption)
{
#if defined(_MGEN)
	return FALSE;
#endif
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;

	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function Beam Design").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, size_t, size_t, BOOL&);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_Section_Column");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_SECT_COLUMN);
	size_t TSize_Option = sizeof(DGNDRAW_EX_RCSECT_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &SectData, &pSectOption, Tsize_Sect, TSize_Option, bVersionOK);

	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_Section_Wall(CDC* pDC, DGNDRAW_SECT_WALL& SectData, DGNDRAW_EX_RCSECT_OPTION& pSectOption)
{
#if defined(_MGEN)
	return FALSE;
#endif
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;

	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function Beam Design").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, size_t, size_t, BOOL&);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_Section_Wall");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_SECT_WALL);
	size_t TSize_Option = sizeof(DGNDRAW_EX_RCSECT_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &SectData, &pSectOption, Tsize_Sect, TSize_Option, bVersionOK);

	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_Section_Plate(CDC* pDC, DGNDRAW_SECT_PLATE& SectData, DGNDRAW_EX_RCSECT_OPTION& pSectOption)
{
#if defined(_MGEN)
	return FALSE;
#endif
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;

	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function Beam Design").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, size_t, size_t, BOOL&);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_Section_Plate");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_SECT_PLATE);
	size_t TSize_Option = sizeof(DGNDRAW_EX_RCSECT_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &SectData, &pSectOption, Tsize_Sect, TSize_Option, bVersionOK);

	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_Chart_MPhi(CDC* pDC, DGNDRAW_MPHI_CHART_DATA& MPhiData, DGNDRAW_MPHI_PDELTA_CHART_OPTION& MPhiOption)
{
#if defined(_MGEN)
	return FALSE;
#endif
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;

	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function M-φ Chart").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, size_t, size_t, BOOL&);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_Chart_MPhi");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_MPHI_CHART_DATA);
	size_t TSize_Option = sizeof(DGNDRAW_MPHI_PDELTA_CHART_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &MPhiOption, &MPhiData, Tsize_Sect, TSize_Option, bVersionOK);

	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}
BOOL CDgn_DrawManager::Draw_Chart_PDelta(CDC* pDC, DGNDRAW_PDELTA_CHART_DATA& PDeltaData, DGNDRAW_MPHI_PDELTA_CHART_OPTION& PDeltaOption)
{
#if defined(_MGEN)
	return FALSE;
#endif
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;

	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function P-δ Chart").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, size_t, size_t, BOOL&);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_Chart_PDelta");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_PDELTA_CHART_DATA);
	size_t TSize_Option = sizeof(DGNDRAW_MPHI_PDELTA_CHART_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &PDeltaOption, &PDeltaData, Tsize_Sect, TSize_Option, bVersionOK);

	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_Chart_MPhi_Multi(CDC* pDC, DGNDRAW_MPHI_MULTI_CHART_DATA& MPhiData, DGNDRAW_MPHI_MULTI_CHART_OPTION& MPhiOption)
{
#if defined(_MGEN)
	return FALSE;
#endif
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;

	CArray<T_DGN_CHART_FORMAT,T_DGN_CHART_FORMAT> aDrawFormat;
	CArray<T_DGN_CHART_GROUP,T_DGN_CHART_GROUP> aDgnChartGroup;

#ifdef _RUS
	//Set Language
	typedef BOOL (PSETLANG)(UINT);
	PSETLANG* pSetLang;
	pSetLang = (PSETLANG*)GetProcAddress(hDllCalc, "SetLanguage");
	BOOL bLang = (*pSetLang)(7);
#endif

	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function P-δ Chart").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, size_t, size_t, BOOL&, BOOL, void*, void*);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_Chart_MPhi_Multi");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_MPHI_MULTI_CHART_DATA);
	size_t TSize_Option = sizeof(DGNDRAW_MPHI_MULTI_CHART_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &MPhiOption, &MPhiData, Tsize_Sect, TSize_Option, bVersionOK, FALSE, &aDrawFormat, &aDgnChartGroup);

	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}

	// MQC 10770 : Gen/Civil(멀티바이트 문자열 집합 사용)에서 생성된 CDC에 DgnEngine(유니코드)가 글씨를 쓰면, 문자열이 2칸당 1글자씩 출력되는 문제
	// Moment-Curvature 관련 데이터를 Gen/Civil로 넘겨줘서 직접 출력하도록 수정
	CDgn_DrawManager_Text DrawTxtMgr;
	DrawTxtMgr.SetIsEMF(MPhiOption.CanvasType==1);
	DrawTxtMgr.Draw_DgnText(pDC, &aDrawFormat, &aDgnChartGroup);
		
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_SoilColumn(CDC* pDC, DGNDRAW_GRND_SECT& SoilData, DGNDRAW_FOUN_SECT& FounData, DGNDRAW_SECT_OPTION& SoilOption, BOOL bDrawPlate, DGNDRAW_GRND_STR_MSG* pStrRes)
{
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;
	
	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function SoilColumn").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, void*, size_t, size_t, size_t, BOOL&, BOOL, void*);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_SoilColumn");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_GRND_SECT);
	size_t Tsize_Foun = sizeof(DGNDRAW_FOUN_SECT);
	size_t TSize_Option = sizeof(DGNDRAW_SECT_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &SoilData, &FounData, &SoilOption, Tsize_Sect, Tsize_Foun, TSize_Option, bVersionOK, bDrawPlate, pStrRes);
	
	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_SoilColumn_Pile(CDC* pDC, DGNDRAW_GRND_SECT& GrndData, DGNDRAW_PILE_SECT& PileData, DGNDRAW_FOUN_SECT& FounData, DGNDRAW_SECT_OPTION& SoilOption, DGNDRAW_GRND_STR_MSG* pStrRes)
{
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;
	
	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function SoilColumn").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, void*, void*, size_t, size_t, size_t, size_t, BOOL&, void*);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_SoilColumn_Pile");
	//==================================================================================================
	size_t Tsize_Grnd= sizeof(DGNDRAW_GRND_SECT);
	size_t Tsize_Pile= sizeof(DGNDRAW_PILE_SECT);
	size_t Tsize_Foun= sizeof(DGNDRAW_FOUN_SECT);
	size_t TSize_Option = sizeof(DGNDRAW_SECT_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &GrndData, &PileData, &FounData, &SoilOption, Tsize_Grnd, Tsize_Pile, Tsize_Foun, TSize_Option, bVersionOK, pStrRes);
	
	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_PileArrange(CDC* pDC, DGNDRAW_PILE_ARRANGE& PileArrData, DGNDRAW_SECT_OPTION& PileOption)
{
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;
	
	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function SoilColumn").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, size_t, size_t, BOOL&);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_PileArrange");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_PILE_ARRANGE);
	size_t TSize_Option = sizeof(DGNDRAW_SECT_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &PileArrData, &PileOption, Tsize_Sect, TSize_Option, bVersionOK);
	
	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_PileArrange_Emf(CDC* pDC, DGNDRAW_PILE_ARRANGE& PileArrData, DGNDRAW_SECT_OPTION& PileOption, CString* pStrPilePath)
{
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;
	
	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function SoilColumn").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, size_t, size_t, BOOL&, void*);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_PileArrange_Emf");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_PILE_ARRANGE);
	size_t TSize_Option = sizeof(DGNDRAW_SECT_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &PileArrData, &PileOption, Tsize_Sect, TSize_Option, bVersionOK, pStrPilePath);
	
	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_SoilColumn_Emf(CDC* pDC, DGNDRAW_GRND_SECT& SoilData, DGNDRAW_FOUN_SECT& FounData, DGNDRAW_SECT_OPTION& SoilOption,  BOOL bDrawPlate, DGNDRAW_GRND_STR_MSG* pStrRes, CString* pStrPilePath)
{
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;
	
	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function SoilColumn").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, void*, size_t, size_t, size_t, BOOL&, BOOL, void*, void*);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_SoilColumn_Emf");
	//==================================================================================================
	size_t Tsize_Sect= sizeof(DGNDRAW_GRND_SECT);
	size_t Tsize_Foun = sizeof(DGNDRAW_FOUN_SECT);
	size_t TSize_Option = sizeof(DGNDRAW_SECT_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &SoilData, &FounData, &SoilOption, Tsize_Sect, Tsize_Foun, TSize_Option, bVersionOK, bDrawPlate,  pStrRes, pStrPilePath);
	
	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

BOOL CDgn_DrawManager::Draw_SoilColumn_Pile_Emf(CDC* pDC, DGNDRAW_GRND_SECT& GrndData, DGNDRAW_PILE_SECT& PileData, DGNDRAW_FOUN_SECT& FounData, DGNDRAW_SECT_OPTION& SoilOption, DGNDRAW_GRND_STR_MSG* pStrRes, CString* pStrPilePath)
{
	HINSTANCE hDllCalc = GetOrLoad_DrawDllHandle();
	if(hDllCalc == NULL) return FALSE;
	
	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function SoilColumn").
	typedef BOOL (PSECTCALC)(CDC*, void*, void*, void*, void*, size_t, size_t, size_t, size_t, BOOL&, void*, void*);
	PSECTCALC *pSectCalc;
	pSectCalc = (PSECTCALC*)GetProcAddress(hDllCalc, "Draw_SoilColumn_Pile_Emf");
	//==================================================================================================
	size_t Tsize_Grnd= sizeof(DGNDRAW_GRND_SECT);
	size_t Tsize_Pile= sizeof(DGNDRAW_PILE_SECT);
	size_t Tsize_Foun= sizeof(DGNDRAW_FOUN_SECT);
	size_t TSize_Option = sizeof(DGNDRAW_SECT_OPTION);
	
	BOOL bVersionOK = FALSE;
	BOOL bChk = (*pSectCalc)(pDC, &GrndData, &PileData, &FounData, &SoilOption, Tsize_Grnd, Tsize_Pile, Tsize_Foun, TSize_Option, bVersionOK, pStrRes, pStrPilePath);
	
	if(!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	
	return bChk;	
}

// Data Convert ////////////////////////////////////////////////////////////////////////
BOOL CDgn_DrawManager::ConvertToDLL_SectDrawData(T_SECT_SECTBASE_D& InData, DGNDRAW_SECT& OutData, BOOL bDesign)
{
	OutData.Initialize();
	int i, nSize;
	CString strShape = ConvertToDLL_ShapeName(InData.Shape);          // 단면형상 
	CStringW strWShape(strShape);
	OutData.Shape = strWShape;
	for(i=0 ; i<8 ; i++)
		OutData.dSize[i] = InData.Size[i];   // 단면치수 ([0]:H, [1]:B)
	
	OutData.dYBar = InData.Design.YBar;
	OutData.dZBar = InData.Design.ZBar;
		
	DGNDRAW_GSEC_VERTEX ChkPoint;
	OutData.arChkPoint.SetSize(4);
	ChkPoint.dX = InData.Stiffness.dx1;  ChkPoint.dY = InData.Stiffness.dy1;  OutData.arChkPoint.SetAt(0, ChkPoint);
	ChkPoint.dX = InData.Stiffness.dx2;  ChkPoint.dY = InData.Stiffness.dy2;  OutData.arChkPoint.SetAt(1, ChkPoint);
	ChkPoint.dX = InData.Stiffness.dx3;  ChkPoint.dY = InData.Stiffness.dy3;  OutData.arChkPoint.SetAt(2, ChkPoint);
	ChkPoint.dX = InData.Stiffness.dx4;  ChkPoint.dY = InData.Stiffness.dy4;  OutData.arChkPoint.SetAt(3, ChkPoint);

	//Gen형식
	OutData.dSize_Gen[0] = fabs(InData.Stiffness.Czp) + fabs(InData.Stiffness.Czm);   // 단면치수 ([0]:H, [1]:B)	
	OutData.dSize_Gen[1] = fabs(InData.Stiffness.Cyp) + fabs(InData.Stiffness.Cym);   
	DGNDRAW_GSEC_POLYGON DrawPolygon;
	DGNDRAW_GSEC_LINE    DrawLine;
	nSize = InData.aOuterPolygon.GetSize();
	OutData.aOuterPolygon.SetSize(nSize);	
	for(i=0 ; i<nSize ; i++)
	{
		if(!ConvertToDLL_DrawPolygon(InData.aOuterPolygon[i], DrawPolygon)) DrawPolygon.Initialize();
		OutData.aOuterPolygon.SetAt(i, DrawPolygon);		
	}
	nSize = InData.aInnerPolygon.GetSize();
	OutData.aInnerPolygon.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		if(!ConvertToDLL_DrawPolygon(InData.aInnerPolygon[i], DrawPolygon)) DrawPolygon.Initialize();
		OutData.aInnerPolygon.SetAt(i, DrawPolygon);		
	}
	nSize = InData.aLine.GetSize();
	OutData.aLine.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		if(!ConvertToDLL_DrawLine(InData.aLine[i], DrawLine)) DrawLine.Initialize();
		OutData.aLine.SetAt(i, DrawLine);
	}

	return TRUE;
}

BOOL CDgn_DrawManager::ConvertToDLL_DrawLine(T_GSEC_LINE& InData, DGNDRAW_GSEC_LINE& OutData)
{
	OutData.Initialize();
	OutData.v1 = InData.v1;
	OutData.v2 = InData.v2;
	OutData.dThik = InData.dThik;
	OutData.nAlign = InData.nAlign;
	return TRUE;
}

BOOL CDgn_DrawManager::ConvertToDLL_DrawPolygon(T_GSEC_POLYGON& InData, DGNDRAW_GSEC_POLYGON& OutData)
{
	OutData.Initialize();
	int nSize = InData.aVertex.GetSize();
	OutData.aVertex.SetSize(nSize);
	DGNDRAW_GSEC_VERTEX    VertexUnit;
	for(int i=0 ; i<nSize ; i++)
	{
		VertexUnit.Set(InData.aVertex[i].dX, InData.aVertex[i].dY);
		OutData.aVertex.SetAt(i, VertexUnit);
	}
	return TRUE;
}

BOOL CDgn_DrawManager::ConvertToDLL_RcBeamData  (T_SECT_SECTBASE_D& SectData, int nIMJ, T_RCHK_D& RbarData, DGNDRAW_SECT_BEAM&   OutData)
{
	// Section
	OutData.Initialize();
	int i;

	if(SectData.Shape == D_SECT_TYPE_PSC || SectData.Shape == D_SECT_SHAPE_PSC_MID)
	{
		double dHL1 = SectData.Size[ 1];
		double dHR1 = SectData.Size[16];
		double dBL2 = SectData.Size[11];
		double dBR2 = SectData.Size[26];
		CString strShape = ConvertToDLL_ShapeName(D_SECT_SHAPE_REG_SB);          // 단면형상 
		CStringW strWShape(strShape);
		OutData.Shape = strWShape;
		OutData.dSize[0] = (dHL1 + dHR1) / 2.;
		OutData.dSize[1] = dBL2 + dBR2;
	}
	else
	{
		CString strShape = ConvertToDLL_ShapeName(SectData.Shape);          // 단면형상 
		CStringW strWShape(strShape);
		OutData.Shape = strWShape;
		for(i=0 ; i<8 ; i++)
			OutData.dSize[i] = SectData.Size[i];   // 단면치수 ([0]:H, [1]:B)		
	}
	OutData.dYBar = SectData.Design.YBar;
	OutData.dZBar = SectData.Design.ZBar;

	// Rebar
	OutData.iArrangeType = 0; //Num형식

	DGNDRAW_MBAR_BEAM MRbar;

	for(i=0; i<DGN_RBAR_LAYER_BEAM; i++)
	{
		double dDT = RbarData.BEAM.dDt[nIMJ][i];  
		double dDB = RbarData.BEAM.dDb[nIMJ][i]; 		

		if(dDT > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc    = dDT;
			MRbar.dSpace = 0.0;
			MRbar.dNum   = (double)(RbarData.BEAM.iTopBarNum[nIMJ][i]);
			OutData.arTopBar.Add(MRbar);
		}
		if(dDB > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc    = dDB;
			MRbar.dSpace = 0.0;
			MRbar.dNum   = (double)(RbarData.BEAM.iBotBarNum[nIMJ][i]);
			OutData.arBotBar.Add(MRbar);
		}
	}

	return TRUE;
}

BOOL CDgn_DrawManager::ConvertToDLL_RcBeamData  (T_SECT_SECTBASE_D& SectData, int nIMJ, T_CPBE_D& RbarData, DGNDRAW_SECT_BEAM&   OutData)
{
	// Section
	OutData.Initialize();
	int i;

	if(SectData.Shape == D_SECT_TYPE_PSC || SectData.Shape == D_SECT_SHAPE_PSC_MID)
	{
		double dHL1 = SectData.Size[ 1];
		double dHR1 = SectData.Size[16];
		double dBL2 = SectData.Size[11];
		double dBR2 = SectData.Size[26];
		CString strShape = ConvertToDLL_ShapeName(D_SECT_SHAPE_REG_SB);          // 단면형상 
		CStringW strWShape(strShape);
		OutData.Shape = strWShape;
		OutData.dSize[0] = (dHL1 + dHR1) / 2.;
		OutData.dSize[1] = dBL2 + dBR2;
	}
	else
	{
		CString strShape = ConvertToDLL_ShapeName(SectData.Shape);          // 단면형상 
		CStringW strWShape(strShape);
		OutData.Shape = strWShape;
		for(i=0 ; i<8 ; i++)
			OutData.dSize[i] = SectData.Size[i];   // 단면치수 ([0]:H, [1]:B)		
	}
	OutData.dYBar = SectData.Design.YBar;
	OutData.dZBar = SectData.Design.ZBar;

	// Rebar
	if(RbarData.bNum) OutData.iArrangeType = 0;
	else              OutData.iArrangeType = 1;

	DGNDRAW_MBAR_BEAM MRbar;

	for(i=0; i<DGN_RBAR_LAYER_BEAM; i++)
	{
		double dDT = RbarData.dDt[nIMJ][i];  
		double dDB = RbarData.dDb[nIMJ][i]; 		

		if(dDT > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc    = dDT;
			MRbar.dSpace = RbarData.dTopBarCTC[nIMJ][i];
			MRbar.dNum   = RbarData.dTopBarNum[nIMJ][i];
			OutData.arTopBar.Add(MRbar);
		}
		if(dDB > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc    = dDB;
			MRbar.dSpace = RbarData.dBotBarCTC[nIMJ][i];
			MRbar.dNum   = RbarData.dBotBarNum[nIMJ][i];
			OutData.arBotBar.Add(MRbar);
		}
	}

	return TRUE;
}
BOOL CDgn_DrawManager::ConvertToDLL_RcColumnData(T_SECT_SECTBASE_D& SectData, T_RCHK_D& RbarData, DGNDRAW_SECT_COLUMN& OutData)
{
	// Section
	OutData.Initialize();
	int i;
	CString strShape = ConvertToDLL_ShapeName(SectData.Shape);          // 단면형상 
	CStringW strWShape(strShape);
	OutData.Shape = strWShape;
	for(i=0 ; i<8 ; i++)
		OutData.dSize[i] = SectData.Size[i];   // 단면치수 ([0]:H, [1]:B)
	OutData.dYBar = SectData.Design.YBar;
	OutData.dZBar = SectData.Design.ZBar;
	
	DGNDRAW_GSEC_VERTEX ChkPoint;
	OutData.arChkPoint.SetSize(4);
	ChkPoint.dX = SectData.Stiffness.dx1;  ChkPoint.dY = SectData.Stiffness.dy1;  OutData.arChkPoint.SetAt(0, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx2;  ChkPoint.dY = SectData.Stiffness.dy2;  OutData.arChkPoint.SetAt(1, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx3;  ChkPoint.dY = SectData.Stiffness.dy3;  OutData.arChkPoint.SetAt(2, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx4;  ChkPoint.dY = SectData.Stiffness.dy4;  OutData.arChkPoint.SetAt(3, ChkPoint);

	// Rebar
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	DGNDRAW_MBAR_COLUMN MBar;
	for(i=0; i<DGN_RBAR_LAYER; i++)
	{
		if(RbarData.COLM.dDc[0][i] > 0.0)
		{
			MBar.Initialize();
			MBar.dDc = RbarData.COLM.dDc[0][i];
			MBar.iNum1 = RbarData.COLM.iBarNum[0][0][i];
			MBar.iNum2 = RbarData.COLM.iBarNum[0][1][i];
			MBar.iNum3 = RbarData.COLM.iBarNum[0][2][i];
			OutData.arMainBar.Add(MBar);
		}
	}

	return TRUE;
}
BOOL CDgn_DrawManager::ConvertToDLL_RcColumnData(T_SECT_SECTBASE_D& SectData, int nIMJ, T_CPCO_D& RbarData, DGNDRAW_SECT_COLUMN& OutData)
{
	// Section
	OutData.Initialize();
	int i;
	CString strShape = ConvertToDLL_ShapeName(SectData.Shape);          // 단면형상 
	CStringW strWShape(strShape);
	OutData.Shape = strWShape;
	for(i=0 ; i<8 ; i++)
		OutData.dSize[i] = SectData.Size[i];   // 단면치수 ([0]:H, [1]:B)
	OutData.dYBar = SectData.Design.YBar;
	OutData.dZBar = SectData.Design.ZBar;
	
	DGNDRAW_GSEC_VERTEX ChkPoint;
	OutData.arChkPoint.SetSize(4);
	ChkPoint.dX = SectData.Stiffness.dx1;  ChkPoint.dY = SectData.Stiffness.dy1;  OutData.arChkPoint.SetAt(0, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx2;  ChkPoint.dY = SectData.Stiffness.dy2;  OutData.arChkPoint.SetAt(1, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx3;  ChkPoint.dY = SectData.Stiffness.dy3;  OutData.arChkPoint.SetAt(2, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx4;  ChkPoint.dY = SectData.Stiffness.dy4;  OutData.arChkPoint.SetAt(3, ChkPoint);

	// Rebar
	DGNDRAW_MBAR_COLUMN MBar;
	for(i=0; i<DGN_RBAR_LAYER; i++)
	{
		if(RbarData.dDc[nIMJ][i] > 0.0)
		{
			MBar.Initialize();
			MBar.dDc = RbarData.dDc[nIMJ][i];
			MBar.iNum1 = RbarData.iBarNum[nIMJ][0][i];
			MBar.iNum2 = RbarData.iBarNum[nIMJ][1][i];
			MBar.iNum3 = RbarData.iBarNum[nIMJ][2][i];
			OutData.arMainBar.Add(MBar);
		}
	}

	return TRUE;
}
BOOL CDgn_DrawManager::ConvertToDLL_RcColumnData_Gen(T_SECT_SECTBASE_D& SectData, int nIMJ, T_RBTE_D& RbarData, DGNDRAW_SECT_COLUMN& OutData)
{
	// Section
	OutData.Initialize();
	int i, nSize;
	CString strShape = ConvertToDLL_ShapeName(SectData.Shape);          // 단면형상 
	CStringW strWShape(strShape);
	OutData.Shape = strWShape;
	for(i=0 ; i<8 ; i++)
		OutData.dSize[i] = SectData.Size[i];   // 단면치수 ([0]:H, [1]:B)
	OutData.dYBar = SectData.Design.YBar;
	OutData.dZBar = SectData.Design.ZBar;
	
	DGNDRAW_GSEC_VERTEX ChkPoint;
	OutData.arChkPoint.SetSize(4);
	ChkPoint.dX = SectData.Stiffness.dx1;  ChkPoint.dY = SectData.Stiffness.dy1;  OutData.arChkPoint.SetAt(0, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx2;  ChkPoint.dY = SectData.Stiffness.dy2;  OutData.arChkPoint.SetAt(1, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx3;  ChkPoint.dY = SectData.Stiffness.dy3;  OutData.arChkPoint.SetAt(2, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx4;  ChkPoint.dY = SectData.Stiffness.dy4;  OutData.arChkPoint.SetAt(3, ChkPoint);

	OutData.dSize_Gen[0] = fabs(SectData.Stiffness.Czp) + fabs(SectData.Stiffness.Czm);   // 단면치수 ([0]:H, [1]:B)	
	OutData.dSize_Gen[1] = fabs(SectData.Stiffness.Cyp) + fabs(SectData.Stiffness.Cym);   
	DGNDRAW_GSEC_POLYGON DrawPolygon;
	nSize = SectData.aOuterPolygon.GetSize();
	OutData.aOuterPolygon.SetSize(nSize);	
	for(i=0 ; i<nSize ; i++)
	{
		if(!ConvertToDLL_DrawPolygon(SectData.aOuterPolygon[i], DrawPolygon)) DrawPolygon.Initialize();
		OutData.aOuterPolygon.SetAt(i, DrawPolygon);		
	}
	nSize = SectData.aInnerPolygon.GetSize();
	OutData.aInnerPolygon.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		if(!ConvertToDLL_DrawPolygon(SectData.aInnerPolygon[i], DrawPolygon)) DrawPolygon.Initialize();
		OutData.aInnerPolygon.SetAt(i, DrawPolygon);		
	}

	// Rebar
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	int iPosi;
	if(nIMJ == 0 || RbarData.bIJBothMain)
		iPosi = 0;
	else if(nIMJ == 2)
		iPosi = 2;
	else if(nIMJ == 1 && RbarData.bIJBothMain)
		iPosi = 1;
	else 
	{ASSERT(0); return FALSE;}

	DGNDRAW_GSEC_VERTEX MBar;
	T_RBTE_MABR         MBarDB;
	nSize = RbarData.arMainRebar[iPosi].GetSize();
	OutData.arMainBar_Gen.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		MBarDB = RbarData.arMainRebar[iPosi].GetAt(i);
		MBar.Initialize();
		MBar.dX = MBarDB.dCenterY;
		MBar.dY = MBarDB.dCenterZ;
		OutData.arMainBar_Gen.SetAt(i, MBar);
	}

	return TRUE;
}
BOOL CDgn_DrawManager::ConvertToDLL_RcColumnData_Gen(T_SECT_SECTBASE_D& SectData, int nIMJ, T_REBT_D& RbarData, DGNDRAW_SECT_COLUMN& OutData)
{
	// Section
	OutData.Initialize();
	int i, nSize;
	CString strShape = ConvertToDLL_ShapeName(SectData.Shape);          // 단면형상 
	CStringW strWShape(strShape);
	OutData.Shape = strWShape;
	for(i=0 ; i<8 ; i++)
		OutData.dSize[i] = SectData.Size[i];   // 단면치수 ([0]:H, [1]:B)
	OutData.dYBar = SectData.Design.YBar;
	OutData.dZBar = SectData.Design.ZBar;
	
	DGNDRAW_GSEC_VERTEX ChkPoint;
	OutData.arChkPoint.SetSize(4);
	ChkPoint.dX = SectData.Stiffness.dx1;  ChkPoint.dY = SectData.Stiffness.dy1;  OutData.arChkPoint.SetAt(0, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx2;  ChkPoint.dY = SectData.Stiffness.dy2;  OutData.arChkPoint.SetAt(1, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx3;  ChkPoint.dY = SectData.Stiffness.dy3;  OutData.arChkPoint.SetAt(2, ChkPoint);
	ChkPoint.dX = SectData.Stiffness.dx4;  ChkPoint.dY = SectData.Stiffness.dy4;  OutData.arChkPoint.SetAt(3, ChkPoint);

	OutData.dSize_Gen[0] = fabs(SectData.Stiffness.Czp) + fabs(SectData.Stiffness.Czm);   // 단면치수 ([0]:H, [1]:B)	
	OutData.dSize_Gen[1] = fabs(SectData.Stiffness.Cyp) + fabs(SectData.Stiffness.Cym);   
	DGNDRAW_GSEC_POLYGON DrawPolygon;
	nSize = SectData.aOuterPolygon.GetSize();
	OutData.aOuterPolygon.SetSize(nSize);	
	for(i=0 ; i<nSize ; i++)
	{
		if(!ConvertToDLL_DrawPolygon(SectData.aOuterPolygon[i], DrawPolygon)) DrawPolygon.Initialize();
		OutData.aOuterPolygon.SetAt(i, DrawPolygon);		
	}
	nSize = SectData.aInnerPolygon.GetSize();
	OutData.aInnerPolygon.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		if(!ConvertToDLL_DrawPolygon(SectData.aInnerPolygon[i], DrawPolygon)) DrawPolygon.Initialize();
		OutData.aInnerPolygon.SetAt(i, DrawPolygon);		
	}

	// Rebar
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	int iPosi;
	if(nIMJ == 0 || RbarData.bIJBothMain)
		iPosi = 0;
	else if(nIMJ == 2)
		iPosi = 2;
	else
	{ASSERT(0); return FALSE;}

	DGNDRAW_GSEC_VERTEX MBar;
	T_REBT_MABR         MBarDB;
	nSize = RbarData.arMainRebar[iPosi].GetSize();
	OutData.arMainBar_Gen.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		MBarDB = RbarData.arMainRebar[iPosi].GetAt(i);
		MBar.Initialize();
		MBar.dX = MBarDB.dCenterY;
		MBar.dY = MBarDB.dCenterZ;
		OutData.arMainBar_Gen.SetAt(i, MBar);
	}

	return TRUE;
}
BOOL CDgn_DrawManager::ConvertToDLL_RcWallData  (T_SECT_SECTBASE_D& SectData, int nIMJ, T_RCWA_D& WallData, T_CPWA_D& RbarData, DGNDRAW_SECT_WALL&   OutData)
{
	// Section
	OutData.Initialize();
	int i;
	CString strShape = ConvertToDLL_ShapeName(SectData.Shape);          // 단면형상 
	CStringW strWShape(strShape);
	OutData.Shape = strWShape;
	for(i=0 ; i<8 ; i++)
		OutData.dSize[i] = SectData.Size[i];   // 단면치수 ([0]:H, [1]:B)

	OutData.dHaunch = 0.0;	
	if(nIMJ==0)      OutData.dHaunch = WallData.dHaunch_hi/3.0;
	else if(nIMJ==2) OutData.dHaunch = WallData.dHaunch_hj/3.0;

	OutData.bUnitWidth = RbarData.bUnitWidth;

	// Rebar
	if(RbarData.bNum) OutData.iArrangeType = 0;
	else              OutData.iArrangeType = 1;

	DGNDRAW_MBAR_BEAM MRbar;

	for(i=0; i<DGN_RBAR_LAYER; i++)
	{
		double dDT = RbarData.dDt[nIMJ][i];  
		double dDB = RbarData.dDb[nIMJ][i]; 		

		if(dDT > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc    = dDT;
			MRbar.dSpace = RbarData.dTopBarCTC[nIMJ][i];
			MRbar.dNum   = RbarData.dTopBarNum[nIMJ][i];
			OutData.arTopBar.Add(MRbar);
		}
		if(dDB > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc    = dDB;
			MRbar.dSpace = RbarData.dBotBarCTC[nIMJ][i];
			MRbar.dNum   = RbarData.dBotBarNum[nIMJ][i];
			OutData.arBotBar.Add(MRbar);
		}
	}

	return TRUE;
}
BOOL CDgn_DrawManager::ConvertToDLL_RcPlateData (T_THIK_D& ThikData, _DGNC_CPPL_D& RbarData, DGNDRAW_SECT_PLATE&  OutData)
{
	// Section
	CString strShape = ConvertToDLL_ShapeName(D_SECT_SHAPE_REG_SB);
	CStringW strWShape(strShape);
	OutData.Shape = strWShape;

	m_pDoc->m_pAttrCtrl->GetThikForDesign(ThikData, OutData.dThik);

	// Rebar
	if(RbarData.bNum) OutData.iArrangeType = 0;
	else              OutData.iArrangeType = 1;

	DGNDRAW_MBAR_BEAM MRbar;

	for(int i=0; i<DGN_RBAR_LAYER; i++)
	{
		double dDT = RbarData.dDt[i];  
		double dDB = RbarData.dDb[i]; 		

		if(dDT > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc    = dDT;
			MRbar.dSpace = RbarData.dTopBarCTC[i];
			MRbar.dNum   = RbarData.dTopBarNum[i];
			OutData.arTopBar.Add(MRbar);
		}
		if(dDB > 0.0)
		{
			MRbar.Initialize();
			MRbar.dDc    = dDB;
			MRbar.dSpace = RbarData.dBotBarCTC[i];
			MRbar.dNum   = RbarData.dBotBarNum[i];
			OutData.arBotBar.Add(MRbar);
		}
	}

	return TRUE;
}

BOOL CDgn_DrawManager::ConvertToDrawDLL_DrawPolygon(DGN_GSEC_POLYGON_LIST& InData, CArray<DGNDRAW_GSEC_POLYGON, DGNDRAW_GSEC_POLYGON&>& OutData)
{
	int i, nSize;
	nSize = InData.GetSize();
	OutData.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		if(!ConvertToDrawDLL_DrawPolygon(InData[i], OutData[i])) return FALSE;
	}
	return TRUE;
}

BOOL CDgn_DrawManager::ConvertToDrawDLL_DrawPolygon(DGN_GSEC_POLYGON& InData, DGNDRAW_GSEC_POLYGON& OutData)
{
	int i, nSize;
	nSize = InData.aVertex.GetSize();
	OutData.aVertex.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		OutData.aVertex[i].Initialize();
		OutData.aVertex[i].dX = InData.aVertex[i].dX;
		OutData.aVertex[i].dY = InData.aVertex[i].dY;
	}
	return TRUE;
}