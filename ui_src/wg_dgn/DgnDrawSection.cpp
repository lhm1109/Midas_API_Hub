// DgnDrawSection.cpp: implementation of the CDgnDrawSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnDrawSection.h"

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\MathFunc.h"

#include "DgnDataCtrl.h"
#include "DgnSectionTool.h"

#define   DGNDRAW_UFAC    1000   
#define   ZOOM_FACTOR     0.65
#define   DGN_RBAR_SIZE   4
#define   DGN_RBAR_LAYER  5
#define   DGN_RBAR_LAYER_BEAM  10// Seungjun-20140519 : Russia rebar layer 5->10

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnDrawSection::CDgnDrawSection()
{
	m_pDoc = CDBDoc::GetDocPoint();   
}

CDgnDrawSection::~CDgnDrawSection()
{

}

//////////////////////////////////////////////////////////////////////////
int CDgnDrawSection::Make_SectionFig_For_SectK (CString& strFileName, T_SECT_K SectK, int nIMJ,_DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign, int MatlType, BOOL bAfter)
{
	optionD.CanvasType = 1;
	TCHAR szDesc[] = _T("MFS Enhanced MetaFile Application");
	CRect rcSect = optionD.Canvas;
	CMetaFileDC metaDC;

	metaDC.CreateEnhanced(NULL, strFileName, rcSect, szDesc);
	metaDC.SetMapMode(MM_HIMETRIC);	
	int nChk = Draw_Section_For_SectK(&metaDC, SectK, nIMJ, optionD, bDesign, MatlType, bAfter);
	HENHMETAFILE hMetaF = metaDC.CloseEnhanced();
	::DeleteEnhMetaFile(hMetaF);		

	return nChk;
}

int CDgnDrawSection::Make_SectionFig_For_ElemK(CString& strFileName, T_ELEM_K ElemK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign, BOOL bAfter)
{
	optionD.CanvasType = 1;
	TCHAR szDesc[] = _T("MFS Enhanced MetaFile Application");
	CRect rcSect = optionD.Canvas;
	CMetaFileDC metaDC;

	metaDC.CreateEnhanced(NULL, strFileName, rcSect, szDesc);
	metaDC.SetMapMode(MM_HIMETRIC);	
	int nChk = Draw_Section_For_ElemK(&metaDC, ElemK, nIMJ, optionD, bDesign, bAfter);
	HENHMETAFILE hMetaF = metaDC.CloseEnhanced();
	::DeleteEnhMetaFile(hMetaF);		

	return nChk;
}

int CDgnDrawSection::Draw_Section_For_SectK(CDC* pDC, T_SECT_K SectK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign, int MatlType, BOOL bAfter)
{
	T_SECT_D SectD; 
	SectD.Initialize();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(bDesign)
	{ if(!pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD))	return FALSE; }
	else 
	{ if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))	return FALSE; }
	
	DGN_GSEC_SHAPE_D    SectData;
	DGNDRAW_SECT_OPTION SectOption; SectOption.Initialize();

	BOOL bChk = ConvertToDLL_SectData(SectD, nIMJ, SectData, bDesign, bAfter);
	SectData.nMatlType = MatlType;	

	T_RSCT_STIFF_D StiffInfo, AfterStiffInfo;
	BOOL bFixCanvasRange = FALSE;
	double dFixBSize, dFixHSize;  dFixBSize=dFixHSize=0.0;	
	if(pDoc->m_pAttrCtrl->GetReportSectStiffInfo(SectD, StiffInfo, &AfterStiffInfo))
	{	
		int nType = StiffInfo.nType;	
		if(nType==5)
		{
			DGN_GSEC_SHAPE_D    SectData_B, SectData_A;
			double dXmax_B, dXmin_B, dYmax_B, dYmin_B;
			double dXmax_A, dXmin_A, dYmax_A, dYmin_A;
			if(!ConvertToDLL_SectData(SectD, 0, SectData_B, bDesign, FALSE)) bChk = FALSE;
			if(!ConvertToDLL_SectData(SectD, 0, SectData_A, bDesign, TRUE))  bChk = FALSE;
			if(!m_DrawManager.GGet_SectionBoundary(SectData_B, dXmax_B, dXmin_B, dYmax_B, dYmin_B)) bChk = FALSE;
			if(!m_DrawManager.GGet_SectionBoundary(SectData_A, dXmax_A, dXmin_A, dYmax_A, dYmin_A)) bChk = FALSE;
			dFixBSize = max(dXmax_B-dXmin_B, dXmax_A-dXmin_A);
			dFixHSize = max(dYmax_B-dYmin_B, dYmax_A-dYmin_A);
			bFixCanvasRange = TRUE;
		}
		else if(nType==6)
		{			
			DGN_GSEC_SHAPE_D    SectData_I, SectData_J;
			double dXmax_I, dXmin_I, dYmax_I, dYmin_I;
			double dXmax_J, dXmin_J, dYmax_J, dYmin_J;
			if(!ConvertToDLL_SectData(SectD, 0, SectData_I, bDesign, FALSE)) bChk = FALSE;
			if(!ConvertToDLL_SectData(SectD, 2, SectData_J, bDesign, FALSE)) bChk = FALSE;
			if(!m_DrawManager.GGet_SectionBoundary(SectData_I, dXmax_I, dXmin_I, dYmax_I, dYmin_I)) bChk = FALSE;
			if(!m_DrawManager.GGet_SectionBoundary(SectData_J, dXmax_J, dXmin_J, dYmax_J, dYmin_J)) bChk = FALSE;
			dFixBSize = max(dXmax_I-dXmin_I, dXmax_J-dXmin_J);
			dFixHSize = max(dYmax_I-dYmin_I, dYmax_J-dYmin_J);
			bFixCanvasRange = TRUE;
		}
		else if(nType==7)
		{
			DGN_GSEC_SHAPE_D    SectData_BI, SectData_BJ, SectData_AI, SectData_AJ;
			double dXmax_BI, dXmin_BI, dYmax_BI, dYmin_BI;
			double dXmax_BJ, dXmin_BJ, dYmax_BJ, dYmin_BJ;
			double dXmax_AI, dXmin_AI, dYmax_AI, dYmin_AI;
			double dXmax_AJ, dXmin_AJ, dYmax_AJ, dYmin_AJ;
			if(!ConvertToDLL_SectData(SectD, 0, SectData_BI, bDesign, FALSE)) bChk = FALSE;
			if(!ConvertToDLL_SectData(SectD, 2, SectData_BJ, bDesign, FALSE)) bChk = FALSE;		
			if(!ConvertToDLL_SectData(SectD, 0, SectData_AI, bDesign, TRUE))  bChk = FALSE;
			if(!ConvertToDLL_SectData(SectD, 2, SectData_AJ, bDesign, TRUE))  bChk = FALSE;
			if(!m_DrawManager.GGet_SectionBoundary(SectData_BI, dXmax_BI, dXmin_BI, dYmax_BI, dYmin_BI)) bChk = FALSE;
			if(!m_DrawManager.GGet_SectionBoundary(SectData_BJ, dXmax_BJ, dXmin_BJ, dYmax_BJ, dYmin_BJ)) bChk = FALSE;
			if(!m_DrawManager.GGet_SectionBoundary(SectData_AI, dXmax_AI, dXmin_AI, dYmax_AI, dYmin_AI)) bChk = FALSE;
			if(!m_DrawManager.GGet_SectionBoundary(SectData_AJ, dXmax_AJ, dXmin_AJ, dYmax_AJ, dYmin_AJ)) bChk = FALSE;			
			dFixBSize = max(max(dXmax_BI-dXmin_BI, dXmax_BJ-dXmin_BJ), max(dXmax_AI-dXmin_AI, dXmax_AJ-dXmin_AJ));
			dFixHSize = max(max(dYmax_BI-dYmin_BI, dYmax_BJ-dYmin_BJ), max(dYmax_AI-dYmin_AI, dYmax_AJ-dYmin_AJ));
			bFixCanvasRange = TRUE;
		}
	}
	else bChk = FALSE;
	
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bLineType = (SectData.Shape == D_SECT_SHAPE_REG_GEN && SectData.arLineData.GetSize() > 0);
	
	// MQC 7201 : Gen/Civil의 Dynamic Report에서만 General Shape 오프셋이 안 맞는 문제
	// 엔진을 수정하느니 그냥 이 부분에서 특수처리하기로 협의.
//   if(SectData.Shape == D_SECT_SHAPE_REG_GEN)
//   {
//     DoOffsetPolygon(SectData.arOutPolyData    , SectData.dYBar, SectData.dZBar);
//     DoOffsetPolygon(SectData.arInPolyData     , SectData.dYBar, SectData.dZBar);
//     DoOffsetPolygon(SectData.arCoreOutPolyData, SectData.dYBar, SectData.dZBar);
//     DoOffsetPolygon(SectData.arCoreInPolyData , SectData.dYBar, SectData.dZBar);
//     DoOffsetPolygon(SectData.arStlOutPolyData , SectData.dYBar, SectData.dZBar);
//     DoOffsetPolygon(SectData.arStlInPolyData  , SectData.dYBar, SectData.dZBar);
//   }

	// 그림을 그릴 공간정보
	SectOption.Canvas          = optionD.Canvas;   
	SectOption.CanvasType      = optionD.CanvasType;
	SectOption.AxisType        = bChk ? optionD.AxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType         = optionD.DimType;
	SectOption.DimShape        = optionD.DimShape;
	SectOption.DimSize         = optionD.DimSize;             // 표현할 치수선의 글자 크기
	SectOption.SectLineSize    = (bLineType ? optionD.RebarSize : optionD.SectLineSize);          // Line 형식 표현시 선의 두께
	SectOption.ChkPointSize    = optionD.ChkPointSize;
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(optionD.LengthUnit<0 ? CurIndex.nBase_Length : optionD.LengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.ZoomFactor      = optionD.ZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.bReDraw         = optionD.bReDraw;                  // 다시그릴지 여부
	SectOption.bShape          = TRUE;                   // 단면을 그릴지 여부
	SectOption.bDimShape       = optionD.bDimShape;      // 단면의 치수선을 그릴지 여부
	SectOption.bFillShape      = optionD.bFillShape;     // 단면의 내부에 색을 채울지 여부
	SectOption.bChkPoint       = optionD.bChkPoint;
	
	SectOption.bFixCanvasRange = bFixCanvasRange;
	SectOption.dFixBSize       = dFixBSize;
	SectOption.dFixHSize       = dFixHSize;

	BOOL bVersionOK = FALSE;
	BOOL bDrawChk = m_DrawManager.GDraw_Section(pDC, SectData, SectOption, bVersionOK);
	if(!bVersionOK) return 0;
	if(SectD.nStype == D_SECT_TYPE_USER && (!bChk || !bDrawChk)) return 0;
	if((SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.nStype == D_SECT_TYPE_USER) && (!bChk || !bDrawChk)) return 0;
	if(!bChk || !bDrawChk) return 1;

	return 2;
}
int CDgnDrawSection::Draw_Section_For_ElemK(CDC* pDC, T_ELEM_K ElemK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign, BOOL bAfter)
{
	T_ELEM_D ElemD;
	ElemD.Initialize();	
	T_MATL_D MatlD; 
	MatlD.Initialize();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int MatlType = 0;
	if(!pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD))	return FALSE;	
	if(!pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat,MatlD))	return FALSE;	
	if     (MatlD.Type==_T("C"))   MatlType = 1;
	else if(MatlD.Type==_T("S"))   MatlType = 2;
	else if(MatlD.Type==_T("SRC")) MatlType = 3;
	else { ASSERT(0);  return FALSE; }

	T_SECT_D SectD;  SectD.Initialize();
	if(!GetSectD_For_ElemK(ElemK, nIMJ, SectD, bDesign, MatlType)) return FALSE;

	DGN_GSEC_SHAPE_D    SectData;
	DGNDRAW_SECT_OPTION SectOption; SectOption.Initialize();
	
	BOOL bChk = ConvertToDLL_SectData(SectD, nIMJ, SectData, bDesign, bAfter);
	SectData.nMatlType = MatlType;	
	
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bLineType = (SectData.Shape == D_SECT_SHAPE_REG_GEN && SectData.arLineData.GetSize() > 0);
	// 그림을 그릴 공간정보
	SectOption.Canvas          = optionD.Canvas;               // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType      = optionD.CanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType        = bChk ? optionD.AxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType         = optionD.DimType;
	SectOption.DimShape        = optionD.DimShape;
	SectOption.DimSize         = optionD.DimSize;             // 표현할 치수선의 글자 크기
	SectOption.SectLineSize    = (bLineType ? optionD.RebarSize : optionD.SectLineSize);          // Line 형식 표현시 선의 두께
	SectOption.ChkPointSize    = optionD.ChkPointSize;
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(optionD.LengthUnit<0 ? CurIndex.nBase_Length : optionD.LengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.ZoomFactor      = optionD.ZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.bReDraw         = optionD.bReDraw;                  // 다시그릴지 여부
	SectOption.bShape          = TRUE;                   // 단면을 그릴지 여부
	SectOption.bDimShape       = optionD.bDimShape;      // 단면의 치수선을 그릴지 여부
	SectOption.bFillShape      = optionD.bDimShape;      // 단면의 내부에 색을 채울지 여부
	SectOption.bChkPoint       = optionD.bChkPoint;

	
	BOOL bVersionOK = FALSE;
	BOOL bDrawChk = m_DrawManager.GDraw_Section(pDC, SectData, SectOption, bVersionOK);	
	if(!bVersionOK) return 0;
	if(SectD.nStype == D_SECT_TYPE_USER && (!bChk || !bDrawChk)) return 0;
	if((SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.nStype == D_SECT_TYPE_USER) && (!bChk || !bDrawChk)) return 0;
	if(!bChk || !bDrawChk) return 1;

	return 2;
}

// RC (Key입력형) //////////////////
BOOL CDgnDrawSection::Draw_Section_RcBeam_For_SectK  (CDC* pDC, T_SECT_K SectK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign)
{
	_DGN_DRAW_SECT_OPTION optionData = optionD;
	
	T_SECT_D SectD;
	if(bDesign) { if(!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK,SectD)) return FALSE; }
	else        { if(!m_pDoc->m_pAttrCtrl->GetSect(SectK,SectD)) return FALSE; }
	
	T_RCHK_D RchkD;  RchkD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetRchk(SectK,RchkD))	optionData.bRebar = FALSE;

	Draw_Section_RcBeam(pDC, &SectD, &RchkD, nIMJ, optionData);
	return TRUE;
}
BOOL CDgnDrawSection::Draw_Section_RcBeam_For_ElemK  (CDC* pDC, T_ELEM_K ElemK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign)
{		
	_DGN_DRAW_SECT_OPTION optionData = optionD;

	T_ELEM_D ElemD;  ElemD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD))	return FALSE;	

	T_SECT_D SectD;  SectD.Initialize();
	if(!GetSectD_For_ElemK(ElemK, nIMJ, SectD, bDesign, 1)) return FALSE;

	BOOL bIsCpbeD = FALSE;
	T_CPBE_D CpbeD;  CpbeD.Initialize();
	bIsCpbeD = m_pDoc->m_pAttrCtrl->GetCpbe(ElemK,CpbeD);

	if(optionData.nRebarDataType == 0 || !bIsCpbeD)
	{		
		T_RCHK_D RchkD;  RchkD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetRchk(ElemD.elpro,RchkD))	optionData.bRebar = FALSE;
		Draw_Section_RcBeam(pDC, &SectD, &RchkD, nIMJ, optionData);
	}
	else 
	{
		Draw_Section_RcBeam(pDC, &SectD, &CpbeD, nIMJ, optionData);
	}
	
	return TRUE;
}
BOOL CDgnDrawSection::Draw_Section_RcColumn_For_SectK(CDC* pDC, T_SECT_K SectK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign)
{
	DGN_GSEC_POLYGON_LIST aCoreOuterPolygon;  aCoreOuterPolygon.RemoveAll();
	DGN_GSEC_POLYGON_LIST aCoreInnerPolygon;  aCoreInnerPolygon.RemoveAll();
	return Draw_Section_RcColumn_For_SectK(pDC, SectK, nIMJ, optionD, bDesign, aCoreOuterPolygon, aCoreInnerPolygon);
}
BOOL CDgnDrawSection::Draw_Section_RcColumn_For_SectK(CDC* pDC, T_SECT_K SectK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign,
		                                                  DGN_GSEC_POLYGON_LIST& aCoreOuterPolygon, DGN_GSEC_POLYGON_LIST& aCoreInnerPolygon)
{
	_DGN_DRAW_SECT_OPTION optionData = optionD;
	
	T_SECT_D SectD;
	if(!m_pDoc->m_pAttrCtrl->GetSect(SectK,SectD)) return FALSE;
	T_SECT_SECTBASE_D SectBaseD     = GetSectBaseD(SectD, nIMJ);
	
	if(SectBaseD.Shape != D_SECT_SHAPE_REG_GEN)
	{// 일반단면
		T_RCHK_D RchkD;  RchkD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetRchk(SectK,RchkD))	optionData.bRebar = FALSE;
		Draw_Section_RcColumn(pDC, &SectD, &RchkD, nIMJ, optionData, aCoreOuterPolygon, aCoreInnerPolygon);
	}
	else 
	{
		T_REBT_D RebtD;  RebtD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetRebt(SectK,RebtD))	optionData.bRebar = FALSE;
		Draw_Section_RcColumn(pDC, &SectD, &RebtD, nIMJ, optionData, aCoreOuterPolygon, aCoreInnerPolygon);
	}
	return TRUE;
}
BOOL CDgnDrawSection::Draw_Section_RcColumn_For_ElemK(CDC* pDC, T_ELEM_K ElemK, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, BOOL bDesign)
{	
	_DGN_DRAW_SECT_OPTION optionData = optionD;

	T_ELEM_D ElemD;  ElemD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD))	return FALSE;	

	T_SECT_D SectD_Org;  SectD_Org.Initialize();
	if(bDesign) { if(!m_pDoc->m_pAttrCtrl->GetSectDesign(ElemD.elpro,SectD_Org))	return FALSE;	}
	else        { if(!m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro,SectD_Org))	return FALSE;	}
	T_SECT_D SectD;  SectD.Initialize();
	if(!GetSectD_For_ElemK(ElemK, nIMJ, SectD, bDesign, 1)) return FALSE;

	T_SECT_SECTBASE_D SectBaseD     = GetSectBaseD(SectD, nIMJ);
	T_SECT_SECTBASE_D SectBaseD_Org = GetSectBaseD(SectD_Org, nIMJ);

	if((SectBaseD_Org.Stiffness.Cym+SectBaseD_Org.Stiffness.Cyp <= 0.0 && SectBaseD_Org.Stiffness.Czm+SectBaseD_Org.Stiffness.Czp <= 0.0)) return FALSE;

	if(SectBaseD.Shape != D_SECT_SHAPE_REG_GEN)
	{// 일반단면
		BOOL bIsCpcoD = FALSE;
		T_CPCO_D CpcoD;  CpcoD.Initialize();
		bIsCpcoD = m_pDoc->m_pAttrCtrl->GetCpco(ElemK,CpcoD);

		if(optionData.nRebarDataType == 0 || !bIsCpcoD)
		{		
			T_RCHK_D RchkD;  RchkD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetRchk(ElemD.elpro,RchkD))	optionData.bRebar = FALSE;
			Draw_Section_RcColumn(pDC, &SectD, &RchkD, nIMJ, optionData);
		}
		else 
		{
			Draw_Section_RcColumn(pDC, &SectD, &CpcoD, nIMJ, optionData);
		}
	}
	else
	{// 임의단면
		BOOL bIsRbteD = FALSE;
		T_RBTE_D RbteD;  RbteD.Initialize();
		bIsRbteD = m_pDoc->m_pAttrCtrl->GetRbte(ElemK,RbteD);

		if(optionData.nRebarDataType == 0 || !bIsRbteD)
		{		
			T_REBT_D RebtD;  RebtD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetRebt(ElemD.elpro,RebtD))	optionData.bRebar = FALSE;			
			if(optionData.bRebar)
			{			
				if(SectBaseD.Stiffness.Cym != SectBaseD_Org.Stiffness.Cym || SectBaseD.Stiffness.Cyp != SectBaseD_Org.Stiffness.Cyp ||
					 SectBaseD.Stiffness.Czm != SectBaseD_Org.Stiffness.Czm || SectBaseD.Stiffness.Czp != SectBaseD_Org.Stiffness.Czp)
				{
					// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
					int iPosi = (nIMJ==2 && !RebtD.bIJBothMain) ? 2 : 0;
					double dScale_y   = (SectBaseD.Stiffness.Cym+SectBaseD.Stiffness.Cyp)/(SectBaseD_Org.Stiffness.Cym+SectBaseD_Org.Stiffness.Cyp);
					double dScale_z   = (SectBaseD.Stiffness.Czm+SectBaseD.Stiffness.Czp)/(SectBaseD_Org.Stiffness.Czm+SectBaseD_Org.Stiffness.Czp);
					double dGap_y_Org = (SectBaseD_Org.Stiffness.Cym+SectBaseD_Org.Stiffness.Cyp)/2.0 - SectBaseD_Org.Stiffness.Cym;
					double dGap_z_Org = (SectBaseD_Org.Stiffness.Czm+SectBaseD_Org.Stiffness.Czp)/2.0 - SectBaseD_Org.Stiffness.Czm;
					double dGap_y     = (SectBaseD.Stiffness.Cym+SectBaseD.Stiffness.Cyp)/2.0 - SectBaseD.Stiffness.Cym;
					double dGap_z     = (SectBaseD.Stiffness.Czm+SectBaseD.Stiffness.Czp)/2.0 - SectBaseD.Stiffness.Czm;
					int nSize = RebtD.arMainRebar[iPosi].GetSize();
					T_REBT_MABR MbarD;
					for(int i=0 ; i<nSize ; i++)
					{
						MbarD = RebtD.arMainRebar[iPosi].GetAt(i);
						MbarD.dCenterY = (MbarD.dCenterY + dGap_y_Org)*dScale_y - dGap_y;
						MbarD.dCenterZ = (MbarD.dCenterZ + dGap_z_Org)*dScale_z - dGap_z;
						RebtD.arMainRebar[iPosi].SetAt(i, MbarD);
					}
				}
			}
			Draw_Section_RcColumn(pDC, &SectD, &RebtD, nIMJ, optionData);
		}
		else 
		{
			if(SectBaseD.Stiffness.Cym != SectBaseD_Org.Stiffness.Cym || SectBaseD.Stiffness.Cyp != SectBaseD_Org.Stiffness.Cyp ||
				 SectBaseD.Stiffness.Czm != SectBaseD_Org.Stiffness.Czm || SectBaseD.Stiffness.Czp != SectBaseD_Org.Stiffness.Czp)
			{
				// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
				int iPosi = (nIMJ==2 && !RbteD.bIJBothMain) ? 2 : 0;
				double dScale_y   = (SectBaseD.Stiffness.Cym+SectBaseD.Stiffness.Cyp)/(SectBaseD_Org.Stiffness.Cym+SectBaseD_Org.Stiffness.Cyp);
				double dScale_z   = (SectBaseD.Stiffness.Czm+SectBaseD.Stiffness.Czp)/(SectBaseD_Org.Stiffness.Czm+SectBaseD_Org.Stiffness.Czp);
				double dGap_y_Org = (SectBaseD_Org.Stiffness.Cym+SectBaseD_Org.Stiffness.Cyp)/2.0 - SectBaseD_Org.Stiffness.Cym;
				double dGap_z_Org = (SectBaseD_Org.Stiffness.Czm+SectBaseD_Org.Stiffness.Czp)/2.0 - SectBaseD_Org.Stiffness.Czm;
				double dGap_y     = (SectBaseD.Stiffness.Cym+SectBaseD.Stiffness.Cyp)/2.0 - SectBaseD.Stiffness.Cym;
				double dGap_z     = (SectBaseD.Stiffness.Czm+SectBaseD.Stiffness.Czp)/2.0 - SectBaseD.Stiffness.Czm;
				int nSize = RbteD.arMainRebar[iPosi].GetSize();
				T_RBTE_MABR MbarD;
				for(int i=0 ; i<nSize ; i++)
				{
					MbarD = RbteD.arMainRebar[iPosi].GetAt(i);
					MbarD.dCenterY = (MbarD.dCenterY + dGap_y_Org)*dScale_y - dGap_y;
					MbarD.dCenterZ = (MbarD.dCenterZ + dGap_z_Org)*dScale_z - dGap_z;
					RbteD.arMainRebar[iPosi].SetAt(i, MbarD);
				}
			}
			Draw_Section_RcColumn(pDC, &SectD, &RbteD, nIMJ, optionData);
		}
		
	}
	
	return TRUE;
}

// RC (Data입력형) /////////////////
void CDgnDrawSection::Draw_Section_RcBeam  (CDC* pDC, T_SECT_D* pSectD, T_RCHK_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD)
{	
#if defined(_MGEN)
	return ;// Civil만 지원 Gen에서 사용할 경우 DgnBase.dll, DgnDrawBase.dll, DgnDraw_Ex.dll 추가하여야함 (SHIN.07.12.07)
	             // 또한 Gen용 철근형식이 지원되어야 함
#endif
	
	// Dll에 넘겨줄 정보 가공
	T_SECT_SECTBASE_D        SectBaseD = GetSectBaseD(*pSectD, nIMJ);
	DGNDRAW_SECT_BEAM        SectData;
	DGNDRAW_EX_RCSECT_OPTION SectOption; SectOption.Initialize();

	BOOL bChk = ConvertToDLL_RcBeamData(SectBaseD, nIMJ, *pRebarD, SectData);	

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	// 그림을 그릴 공간정보
	SectOption.Canvas          = optionD.Canvas;              // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType      = optionD.CanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType        = bChk ? optionD.AxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType         = optionD.DimType;             // 1:전체치수선 표시, 2:B,H치수선만 표시
	SectOption.DimShape        = optionD.DimShape;            // 1:Text선위(값), 2:Text중앙(값), 3:Text선위(기호), 4:Text중앙(기호)
	SectOption.DimSize         = optionD.DimSize;             // 표현할 치수선의 글자 크기
	SectOption.RebarSize       = optionD.RebarSize;           // 표현한 철근그림의 직경
	SectOption.RebarShape      = 1;                           // 1:철근만 표시, 2:철근+철근영역 표시(선), 3:철근+철근영역 표시(영역)
	SectOption.SectLineSize    = optionD.SectLineSize;        // Line 형식 표현시 선의 두께
	SectOption.ChkPointSize    = optionD.ChkPointSize;        // 표현한 ChkPoint의 직경
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(optionD.LengthUnit<0 ? CurIndex.nBase_Length : optionD.LengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.ZoomFactor      = optionD.ZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.nTypeRebarView  = optionD.nTypeRebarView;
	SectOption.bReDraw         = optionD.bReDraw;              // 다시그릴지 여부
	SectOption.bShape          = TRUE;               // 단면을 그릴지 여부
	SectOption.bRebar          = optionD.bRebar;     // 철근을 그릴지 여부
	SectOption.bRebarPosi      = FALSE;              // 철근의 위치정보를 그릴지 여부
	SectOption.bDimShape       = optionD.bDimShape;  // 단면의 치수선을 그릴지 여부
	SectOption.bDimRebar       = optionD.bDimRebar;  // 철근의 치수선을 그릴지 여부
	SectOption.bFillShape      = optionD.bFillShape; // 단면의 내부에 색을 채울지 여부
	SectOption.bFillRebar      = optionD.bFillRebar; // 철근의 내부에 색을 채울지 여부
 
	m_DrawManager.Draw_Section_Beam(pDC, SectData, SectOption);  
}
void CDgnDrawSection::Draw_Section_RcBeam  (CDC* pDC, T_SECT_D* pSectD, T_CPBE_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD)
{
#if defined(_MGEN)
	return ;// Civil만 지원 Gen에서 사용할 경우 DgnBase.dll, DgnDrawBase.dll, DgnDraw_Ex.dll 추가하여야함 (SHIN.07.12.07)
	             // 또한 Gen용 철근형식이 지원되어야 함
#endif
	
	// Dll에 넘겨줄 정보 가공
	T_SECT_SECTBASE_D        SectBaseD = GetSectBaseD(*pSectD, nIMJ);
	DGNDRAW_SECT_BEAM        SectData;
	DGNDRAW_EX_RCSECT_OPTION SectOption; SectOption.Initialize();

	T_CPBE_D RebarD = *pRebarD;
	if(optionD.nRebarDataType == 1)
		RebarD.bNum = TRUE;
	else if(optionD.nRebarDataType == 2)
		RebarD.bNum = FALSE;
	BOOL bChk = ConvertToDLL_RcBeamData(SectBaseD, nIMJ, RebarD, SectData);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	// 그림을 그릴 공간정보
	SectOption.Canvas          = optionD.Canvas;              // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType      = optionD.CanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType        = bChk ? optionD.AxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType         = optionD.DimType;             // 1:전체치수선 표시, 2:B,H치수선만 표시
	SectOption.DimShape        = optionD.DimShape;            // 1:Text선위(값), 2:Text중앙(값), 3:Text선위(기호), 4:Text중앙(기호)
	SectOption.DimSize         = optionD.DimSize;             // 표현할 치수선의 글자 크기
	SectOption.RebarSize       = optionD.RebarSize;           // 표현한 철근그림의 직경
	SectOption.RebarShape      = 1;                           // 1:철근만 표시, 2:철근+철근영역 표시(선), 3:철근+철근영역 표시(영역)
	SectOption.SectLineSize    = optionD.SectLineSize;        // Line 형식 표현시 선의 두께
	SectOption.ChkPointSize    = optionD.ChkPointSize;        // 표현한 ChkPoint의 직경
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(optionD.LengthUnit<0 ? CurIndex.nBase_Length : optionD.LengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.ZoomFactor      = optionD.ZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.nTypeRebarView  = optionD.nTypeRebarView;
	SectOption.bReDraw         = optionD.bReDraw;              // 다시그릴지 여부
	SectOption.bShape          = TRUE;               // 단면을 그릴지 여부
	SectOption.bRebar          = optionD.bRebar;     // 철근을 그릴지 여부
	SectOption.bRebarPosi      = FALSE;              // 철근의 위치정보를 그릴지 여부
	SectOption.bDimShape       = optionD.bDimShape;  // 단면의 치수선을 그릴지 여부
	SectOption.bDimRebar       = optionD.bDimRebar;  // 철근의 치수선을 그릴지 여부
	SectOption.bFillShape      = optionD.bFillShape; // 단면의 내부에 색을 채울지 여부
	SectOption.bFillRebar      = optionD.bFillRebar; // 철근의 내부에 색을 채울지 여부
 
	m_DrawManager.Draw_Section_Beam(pDC, SectData, SectOption);  
}
void CDgnDrawSection::Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_RCHK_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD)
{
	DGN_GSEC_POLYGON_LIST aCoreOuterPolygon;  aCoreOuterPolygon.RemoveAll();
	DGN_GSEC_POLYGON_LIST aCoreInnerPolygon;  aCoreInnerPolygon.RemoveAll();
	Draw_Section_RcColumn(pDC, pSectD, pRebarD, nIMJ, optionD, aCoreOuterPolygon, aCoreInnerPolygon);
}
void CDgnDrawSection::Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_RCHK_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD,
		                                        DGN_GSEC_POLYGON_LIST& aCoreOuterPolygon, DGN_GSEC_POLYGON_LIST& aCoreInnerPolygon)
{
#if defined(_MGEN)
	return ;// Civil만 지원 Gen에서 사용할 경우 DgnBase.dll, DgnDrawBase.dll, DgnDraw_Ex.dll 추가하여야함 (SHIN.07.12.07)
	             // 또한 Gen용 철근형식이 지원되어야 함
#endif
	
	// Dll에 넘겨줄 정보 가공
	T_SECT_SECTBASE_D        SectBaseD = GetSectBaseD(*pSectD, nIMJ);
	DGNDRAW_SECT_COLUMN      SectData;
	DGNDRAW_EX_RCSECT_OPTION SectOption; SectOption.Initialize();

	BOOL bChk = ConvertToDLL_RcColumnData(SectBaseD, *pRebarD, SectData);

	if(bChk && aCoreOuterPolygon.GetSize() > 0)
	{
		m_DrawManager.ConvertToDrawDLL_DrawPolygon(aCoreOuterPolygon, SectData.aCoreOuterPolygon);
		m_DrawManager.ConvertToDrawDLL_DrawPolygon(aCoreInnerPolygon, SectData.aCoreInnerPolygon);
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	// 그림을 그릴 공간정보
	SectOption.Canvas          = optionD.Canvas;              // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType      = optionD.CanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType        = bChk ? optionD.AxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType         = optionD.DimType;             // 1:전체치수선 표시, 2:B,H치수선만 표시
	SectOption.DimShape        = optionD.DimShape;            // 1:Text선위(값), 2:Text중앙(값), 3:Text선위(기호), 4:Text중앙(기호)
	SectOption.DimSize         = optionD.DimSize;             // 표현할 치수선의 글자 크기
	SectOption.RebarSize       = optionD.RebarSize;           // 표현한 철근그림의 직경
	SectOption.RebarShape      = 1;                           // 1:철근만 표시, 2:철근+철근영역 표시(선), 3:철근+철근영역 표시(영역)
	SectOption.SectLineSize    = optionD.SectLineSize;        // Line 형식 표현시 선의 두께
	SectOption.ChkPointSize    = optionD.ChkPointSize;        // 표현한 ChkPoint의 직경
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(optionD.LengthUnit<0 ? CurIndex.nBase_Length : optionD.LengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.ZoomFactor      = optionD.ZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.nTypeRebarView  = optionD.nTypeRebarView;
	SectOption.RotAngle        = optionD.RotAngle;
	SectOption.bReDraw         = optionD.bReDraw;              // 다시그릴지 여부
	SectOption.bShape          = TRUE;               // 단면을 그릴지 여부
	SectOption.bRebar          = optionD.bRebar;     // 철근을 그릴지 여부
	SectOption.bRebarPosi      = FALSE;              // 철근의 위치정보를 그릴지 여부
	SectOption.bDimShape       = optionD.bDimShape;  // 단면의 치수선을 그릴지 여부
	SectOption.bDimRebar       = optionD.bDimRebar;  // 철근의 치수선을 그릴지 여부
	SectOption.bFillShape      = optionD.bFillShape; // 단면의 내부에 색을 채울지 여부
	SectOption.bFillRebar      = optionD.bFillRebar; // 철근의 내부에 색을 채울지 여부
 
	m_DrawManager.Draw_Section_Column(pDC, SectData, SectOption);  
}
void CDgnDrawSection::Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_CPCO_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD)
{
	DGN_GSEC_POLYGON_LIST aCoreOuterPolygon;  aCoreOuterPolygon.RemoveAll();
	DGN_GSEC_POLYGON_LIST aCoreInnerPolygon;  aCoreInnerPolygon.RemoveAll();
	Draw_Section_RcColumn(pDC, pSectD, pRebarD, nIMJ, optionD, aCoreOuterPolygon, aCoreInnerPolygon);
}
void CDgnDrawSection::Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_CPCO_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD,
		                                        DGN_GSEC_POLYGON_LIST& aCoreOuterPolygon, DGN_GSEC_POLYGON_LIST& aCoreInnerPolygon)
{
#if defined(_MGEN)
	return ;// Civil만 지원 Gen에서 사용할 경우 DgnBase.dll, DgnDrawBase.dll, DgnDraw_Ex.dll 추가하여야함 (SHIN.07.12.07)
	             // 또한 Gen용 철근형식이 지원되어야 함
#endif
	
	// Dll에 넘겨줄 정보 가공
	T_SECT_SECTBASE_D        SectBaseD = GetSectBaseD(*pSectD, nIMJ);
	DGNDRAW_SECT_COLUMN      SectData;
	DGNDRAW_EX_RCSECT_OPTION SectOption; SectOption.Initialize();

	BOOL bChk = ConvertToDLL_RcColumnData(SectBaseD, nIMJ, *pRebarD, SectData);
	
	if(bChk && aCoreOuterPolygon.GetSize() > 0)
	{
		m_DrawManager.ConvertToDrawDLL_DrawPolygon(aCoreOuterPolygon, SectData.aCoreOuterPolygon);
		m_DrawManager.ConvertToDrawDLL_DrawPolygon(aCoreInnerPolygon, SectData.aCoreInnerPolygon);
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	// 그림을 그릴 공간정보
	SectOption.Canvas          = optionD.Canvas;              // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType      = optionD.CanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType        = bChk ? optionD.AxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType         = optionD.DimType;             // 1:전체치수선 표시, 2:B,H치수선만 표시
	SectOption.DimShape        = optionD.DimShape;            // 1:Text선위(값), 2:Text중앙(값), 3:Text선위(기호), 4:Text중앙(기호)
	SectOption.DimSize         = optionD.DimSize;             // 표현할 치수선의 글자 크기
	SectOption.RebarSize       = optionD.RebarSize;           // 표현한 철근그림의 직경
	SectOption.RebarShape      = 1;                           // 1:철근만 표시, 2:철근+철근영역 표시(선), 3:철근+철근영역 표시(영역)
	SectOption.SectLineSize    = optionD.SectLineSize;        // Line 형식 표현시 선의 두께
	SectOption.ChkPointSize    = optionD.ChkPointSize;        // 표현한 ChkPoint의 직경
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(optionD.LengthUnit<0 ? CurIndex.nBase_Length : optionD.LengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.ZoomFactor      = optionD.ZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.nTypeRebarView  = optionD.nTypeRebarView;
	SectOption.bReDraw         = optionD.bReDraw;              // 다시그릴지 여부
	SectOption.bShape          = TRUE;               // 단면을 그릴지 여부
	SectOption.bRebar          = optionD.bRebar;     // 철근을 그릴지 여부
	SectOption.bRebarPosi      = FALSE;              // 철근의 위치정보를 그릴지 여부
	SectOption.bDimShape       = optionD.bDimShape;  // 단면의 치수선을 그릴지 여부
	SectOption.bDimRebar       = optionD.bDimRebar;  // 철근의 치수선을 그릴지 여부
	SectOption.bFillShape      = optionD.bFillShape; // 단면의 내부에 색을 채울지 여부
	SectOption.bFillRebar      = optionD.bFillRebar; // 철근의 내부에 색을 채울지 여부
 
	m_DrawManager.Draw_Section_Column(pDC, SectData, SectOption);  
}
void CDgnDrawSection::Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_RBTE_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD)
{
	DGN_GSEC_POLYGON_LIST aCoreOuterPolygon;  aCoreOuterPolygon.RemoveAll();
	DGN_GSEC_POLYGON_LIST aCoreInnerPolygon;  aCoreInnerPolygon.RemoveAll();
	Draw_Section_RcColumn(pDC, pSectD, pRebarD, nIMJ, optionD, aCoreOuterPolygon, aCoreInnerPolygon);
}
void CDgnDrawSection::Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_RBTE_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD,
		                                        DGN_GSEC_POLYGON_LIST& aCoreOuterPolygon, DGN_GSEC_POLYGON_LIST& aCoreInnerPolygon)
{
#if defined(_MGEN)
	return ;// Civil만 지원 Gen에서 사용할 경우 DgnBase.dll, DgnDrawBase.dll, DgnDraw_Ex.dll 추가하여야함 (SHIN.07.12.07)
	             // 또한 Gen용 철근형식이 지원되어야 함
#endif
	
	// Dll에 넘겨줄 정보 가공
	T_SECT_SECTBASE_D        SectBaseD = GetSectBaseD(*pSectD, nIMJ);
	DGNDRAW_SECT_COLUMN      SectData;
	DGNDRAW_EX_RCSECT_OPTION SectOption; SectOption.Initialize();

	BOOL bChk = ConvertToDLL_RcColumnData_Gen(SectBaseD, nIMJ, *pRebarD, SectData);

	if(bChk && aCoreOuterPolygon.GetSize() > 0)
	{
		m_DrawManager.ConvertToDrawDLL_DrawPolygon(aCoreOuterPolygon, SectData.aCoreOuterPolygon);
		m_DrawManager.ConvertToDrawDLL_DrawPolygon(aCoreInnerPolygon, SectData.aCoreInnerPolygon);
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	// 그림을 그릴 공간정보
	SectOption.Canvas          = optionD.Canvas;              // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType      = optionD.CanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType        = bChk ? optionD.AxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType         = optionD.DimType;             // 1:전체치수선 표시, 2:B,H치수선만 표시
	SectOption.DimShape        = optionD.DimShape;            // 1:Text선위(값), 2:Text중앙(값), 3:Text선위(기호), 4:Text중앙(기호)
	SectOption.DimSize         = optionD.DimSize;             // 표현할 치수선의 글자 크기
	SectOption.RebarSize       = optionD.RebarSize;           // 표현한 철근그림의 직경
	SectOption.RebarShape      = 1;                           // 1:철근만 표시, 2:철근+철근영역 표시(선), 3:철근+철근영역 표시(영역)
	SectOption.SectLineSize    = optionD.SectLineSize;        // Line 형식 표현시 선의 두께
	SectOption.ChkPointSize    = optionD.ChkPointSize;        // 표현한 ChkPoint의 직경
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(optionD.LengthUnit<0 ? CurIndex.nBase_Length : optionD.LengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.ZoomFactor      = optionD.ZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.nTypeRebarView  = optionD.nTypeRebarView;
	SectOption.bReDraw         = optionD.bReDraw;              // 다시그릴지 여부
	SectOption.bShape          = TRUE;               // 단면을 그릴지 여부
	SectOption.bRebar          = optionD.bRebar;     // 철근을 그릴지 여부
	SectOption.bRebarPosi      = FALSE;              // 철근의 위치정보를 그릴지 여부
	SectOption.bDimShape       = optionD.bDimShape;  // 단면의 치수선을 그릴지 여부
	SectOption.bDimRebar       = optionD.bDimRebar;  // 철근의 치수선을 그릴지 여부
	SectOption.bFillShape      = optionD.bFillShape; // 단면의 내부에 색을 채울지 여부
	SectOption.bFillRebar      = optionD.bFillRebar; // 철근의 내부에 색을 채울지 여부
 
	m_DrawManager.Draw_Section_Column(pDC, SectData, SectOption);  
}
void CDgnDrawSection::Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_REBT_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD)
{
	DGN_GSEC_POLYGON_LIST aCoreOuterPolygon;  aCoreOuterPolygon.RemoveAll();
	DGN_GSEC_POLYGON_LIST aCoreInnerPolygon;  aCoreInnerPolygon.RemoveAll();
	Draw_Section_RcColumn(pDC, pSectD, pRebarD, nIMJ, optionD, aCoreOuterPolygon, aCoreInnerPolygon);
}
void CDgnDrawSection::Draw_Section_RcColumn(CDC* pDC, T_SECT_D* pSectD, T_REBT_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD,
		                                        DGN_GSEC_POLYGON_LIST& aCoreOuterPolygon, DGN_GSEC_POLYGON_LIST& aCoreInnerPolygon)
{
#if defined(_MGEN)
	return ;// Civil만 지원 Gen에서 사용할 경우 DgnBase.dll, DgnDrawBase.dll, DgnDraw_Ex.dll 추가하여야함 (SHIN.07.12.07)
	             // 또한 Gen용 철근형식이 지원되어야 함
#endif
	
	// Dll에 넘겨줄 정보 가공
	T_SECT_SECTBASE_D        SectBaseD = GetSectBaseD(*pSectD, nIMJ);
	DGNDRAW_SECT_COLUMN      SectData;
	DGNDRAW_EX_RCSECT_OPTION SectOption; SectOption.Initialize();

	BOOL bChk = ConvertToDLL_RcColumnData_Gen(SectBaseD, nIMJ, *pRebarD, SectData);

	if(bChk && aCoreOuterPolygon.GetSize() > 0)
	{
		m_DrawManager.ConvertToDrawDLL_DrawPolygon(aCoreOuterPolygon, SectData.aCoreOuterPolygon);
		m_DrawManager.ConvertToDrawDLL_DrawPolygon(aCoreInnerPolygon, SectData.aCoreInnerPolygon);
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	// 그림을 그릴 공간정보
	SectOption.Canvas          = optionD.Canvas;              // CDC에 그릴 영역(CanvasType과 상관 없이 좌측 상단점을 0,0 으로 좌하단을 +방향으로 한다.)
	SectOption.CanvasType      = optionD.CanvasType;          // 0:Dlg형식, 1:Meta형식
	SectOption.AxisType        = bChk ? optionD.AxisType : 3; // 0:축 표시안함, 1:축만 표시,  2:축,중립축 둘다 표시, 3:단면표시 없이 축만표시 
	SectOption.DimType         = optionD.DimType;             // 1:전체치수선 표시, 2:B,H치수선만 표시
	SectOption.DimShape        = optionD.DimShape;            // 1:Text선위(값), 2:Text중앙(값), 3:Text선위(기호), 4:Text중앙(기호)
	SectOption.DimSize         = optionD.DimSize;             // 표현할 치수선의 글자 크기
	SectOption.RebarSize       = optionD.RebarSize;           // 표현한 철근그림의 직경
	SectOption.RebarShape      = 1;                           // 1:철근만 표시, 2:철근+철근영역 표시(선), 3:철근+철근영역 표시(영역)
	SectOption.SectLineSize    = optionD.SectLineSize;        // Line 형식 표현시 선의 두께
	SectOption.ChkPointSize    = optionD.ChkPointSize;        // 표현한 ChkPoint의 직경
	SectOption.LengthUnit_Data = ConvertToDLL_LengthUnitID(CurIndex.nBase_Length);  // 입력정보의 표현 단위계 ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.LengthUnit_Draw = ConvertToDLL_LengthUnitID(optionD.LengthUnit<0 ? CurIndex.nBase_Length : optionD.LengthUnit); // 치수선의 표현 단위계   ('D_UNITSYS_LENGTH_INDEX_MM'or 0:mm  'D_UNITSYS_LENGTH_INDEX_CM'or 1:cm 'D_UNITSYS_LENGTH_INDEX_M'or 2:m  이하동일)
	SectOption.ZoomFactor      = optionD.ZoomFactor;
	// 그림을 그리기위한 세부설정
	SectOption.nTypeRebarView  = optionD.nTypeRebarView;
	SectOption.bReDraw         = optionD.bReDraw;              // 다시그릴지 여부
	SectOption.bShape          = TRUE;               // 단면을 그릴지 여부
	SectOption.bRebar          = optionD.bRebar;     // 철근을 그릴지 여부
	SectOption.bRebarPosi      = FALSE;              // 철근의 위치정보를 그릴지 여부
	SectOption.bDimShape       = optionD.bDimShape;  // 단면의 치수선을 그릴지 여부
	SectOption.bDimRebar       = optionD.bDimRebar;  // 철근의 치수선을 그릴지 여부
	SectOption.bFillShape      = optionD.bFillShape; // 단면의 내부에 색을 채울지 여부
	SectOption.bFillRebar      = optionD.bFillRebar; // 철근의 내부에 색을 채울지 여부
 
	m_DrawManager.Draw_Section_Column(pDC, SectData, SectOption);  
}
void CDgnDrawSection::Draw_Section_RcWall  (CDC* pDC, T_SECT_D* pSectD, T_CPWA_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD, T_RCWA_D* pRcwaD)
{
	ASSERT(0); // Wall Section Draw 작업 추가 요망(by SHIN)
}
void CDgnDrawSection::Draw_Section_RcPlate (CDC* pDC, T_THIK_D* pSectD, T_REBT_D* pRebarD, int nIMJ, _DGN_DRAW_SECT_OPTION& optionD)
{
	ASSERT(0); // Plate Section Draw 작업 추가 요망(by SHIN)
}

//////////////////////////////////////////////////////////////////////////

T_SECT_SECTBASE_D CDgnDrawSection::GetSectBaseD(T_SECT_D& SectD, int nIMJ)
{
	return m_DrawManager.GetSectBaseD(SectD, nIMJ);
}
T_SECT_SECTBASE_D CDgnDrawSection::GetSectBaseD(T_SECT_D& SectD, int& nSType, int nIMJ, BOOL bAfter)
{
	return m_DrawManager.GetSectBaseD(SectD, nSType, nIMJ, bAfter);
}

T_SECT_SECTBASE_D CDgnDrawSection::GetSectBaseD(T_SECT_SECTBASE_D& SectI, T_SECT_SECTBASE_D& SectJ, int nIMJ)
{
	return m_DrawManager.GetSectBaseD(SectI, SectJ, nIMJ);
}

T_SECT_SECTBASE_D CDgnDrawSection::GetSectBaseD_For_ElemK (T_ELEM_K ElemK, int nIMJ, BOOL bDesign, int MatlType)
{
	return m_DrawManager.GetSectBaseD_For_ElemK(ElemK, nIMJ, bDesign, MatlType);
}

T_SECT_SECTBASE_D CDgnDrawSection::GetSectBaseD_For_SectK (T_SECT_K SectK, int nIMJ, BOOL bDesign)
{
	return m_DrawManager.GetSectBaseD_For_SectK(SectK, nIMJ, bDesign);
}

BOOL CDgnDrawSection::GetSectD_For_ElemK(T_ELEM_K ElemK, int nIMJ, T_SECT_D& SectD, BOOL bDesign, int MatlType)
{
	return m_DrawManager.GetSectD_For_ElemK(ElemK, nIMJ, SectD, bDesign, MatlType);
}

// Data Convert //////////////////////////////////////////////////////

BOOL CDgnDrawSection::ConvertToDLL_Polygon(T_GSEC_POLYGON& InData, DGNDRAW_GSEC_POLYGON& OutData)
{
	return m_DrawManager.ConvertToDLL_DrawPolygon(InData, OutData);
}
BOOL CDgnDrawSection::ConvertToDLL_Polygon(T_GSEC_POLYGON& InData, DGN_GSEC_POLYGON& OutData)
{
	return m_DrawManager.ConvertToDLL_Polygon(InData, OutData);
}
BOOL CDgnDrawSection::ConvertToDLL_Line(T_GSEC_LINE& InData, DGNDRAW_GSEC_LINE& OutData)
{
	return m_DrawManager.ConvertToDLL_DrawLine(InData, OutData);
}
BOOL CDgnDrawSection::ConvertToDLL_Line(T_GSEC_LINE& InData, DGN_GSEC_LINE& OutData)
{
	return m_DrawManager.ConvertToDLL_Line(InData, OutData);
}
BOOL CDgnDrawSection::ConvertToDLL_SectData(T_SECT_SECTBASE_D& InData, DGNDRAW_SECT& OutData, BOOL bDesign)
{
	return m_DrawManager.ConvertToDLL_SectDrawData(InData, OutData, bDesign);
}

BOOL CDgnDrawSection::ConvertToDLL_SectData(T_SECT_D& SectD, int nIMJ, DGN_GSEC_SHAPE_D& OutData, BOOL bDesign, BOOL bAfter)
{
	return m_DrawManager.ConvertToDLL_SectData(SectD, nIMJ, OutData, bDesign, bAfter);
}
BOOL CDgnDrawSection::ConvertToDLL_RcBeamData  (T_SECT_SECTBASE_D& SectData, int nIMJ, T_RCHK_D& RbarData, DGNDRAW_SECT_BEAM&   OutData)
{
	return m_DrawManager.ConvertToDLL_RcBeamData(SectData, nIMJ, RbarData, OutData);
}

BOOL CDgnDrawSection::ConvertToDLL_RcBeamData  (T_SECT_SECTBASE_D& SectData, int nIMJ, T_CPBE_D& RbarData, DGNDRAW_SECT_BEAM&   OutData)
{
	return m_DrawManager.ConvertToDLL_RcBeamData(SectData, nIMJ, RbarData, OutData);
}
BOOL CDgnDrawSection::ConvertToDLL_RcColumnData(T_SECT_SECTBASE_D& SectData, T_RCHK_D& RbarData, DGNDRAW_SECT_COLUMN& OutData)
{
	return m_DrawManager.ConvertToDLL_RcColumnData(SectData, RbarData, OutData);
}
BOOL CDgnDrawSection::ConvertToDLL_RcColumnData(T_SECT_SECTBASE_D& SectData, int nIMJ, T_CPCO_D& RbarData, DGNDRAW_SECT_COLUMN& OutData)
{
	return m_DrawManager.ConvertToDLL_RcColumnData(SectData, nIMJ, RbarData, OutData);
}
BOOL CDgnDrawSection::ConvertToDLL_RcColumnData_Gen(T_SECT_SECTBASE_D& SectData, int nIMJ, T_RBTE_D& RbarData, DGNDRAW_SECT_COLUMN& OutData)
{
	return m_DrawManager.ConvertToDLL_RcColumnData_Gen(SectData, nIMJ, RbarData, OutData);
}
BOOL CDgnDrawSection::ConvertToDLL_RcColumnData_Gen(T_SECT_SECTBASE_D& SectData, int nIMJ, T_REBT_D& RbarData, DGNDRAW_SECT_COLUMN& OutData)
{
	return m_DrawManager.ConvertToDLL_RcColumnData_Gen(SectData, nIMJ, RbarData, OutData);
}
BOOL CDgnDrawSection::ConvertToDLL_RcWallData  (T_SECT_SECTBASE_D& SectData, int nIMJ, T_RCWA_D& WallData, T_CPWA_D& RbarData, DGNDRAW_SECT_WALL&   OutData)
{
	return m_DrawManager.ConvertToDLL_RcWallData(SectData, nIMJ, WallData, RbarData, OutData);
}
BOOL CDgnDrawSection::ConvertToDLL_RcPlateData (T_THIK_D& ThikData, _DGNC_CPPL_D& RbarData, DGNDRAW_SECT_PLATE&  OutData)
{
	return m_DrawManager.ConvertToDLL_RcPlateData (ThikData, RbarData, OutData);
}

CString  CDgnDrawSection::ConvertToDLL_ShapeName(CString Shape)
{
	return m_DrawManager.ConvertToDLL_ShapeName(Shape);
}
int  CDgnDrawSection::ConvertToDLL_LengthUnitID(int UnitID)
{
	return m_DrawManager.ConvertToDLL_LengthUnitID(UnitID);
}
int CDgnDrawSection::ConvertToDLL_Stype(int nStype)
{
	return m_DrawManager.ConvertToDLL_Stype(nStype);
}

void CDgnDrawSection::DoOffsetPolygon(DGN_GSEC_POLYGON_LIST& aGsecPolygon, const double dX, const double dY)
{
	for(int i=0; i<aGsecPolygon.GetSize(); i++)
	{
		for(int j=0; j<aGsecPolygon[i].aVertex.GetSize(); j++)
		{
			aGsecPolygon[i].aVertex[j].dX += dX;
			aGsecPolygon[i].aVertex[j].dY += dY;
		}
	}
}
