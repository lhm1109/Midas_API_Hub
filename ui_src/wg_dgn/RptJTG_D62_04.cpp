// RptJTG_D62_04.cpp: implementation of the CRptJTG_D62_04 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "RptJTG_D62_04.h"
#include <winbase.h>


#include "..\wg_xl\XmlDocument.h"
#include "..\wg_xl\XslDocument.h"
#include "..\wg_xl\ReportData.h"

//#include "..\wg_base\wg_base_MsgDll.h"
//#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\ReleaseInfo.h"
#include "..\wg_db\mvload.h"


#if _MSC_VER >= 1700  // vc++11.0 //sjhuh-2014-02-21
using namespace std;
#include "..\MIT_Lib\rptostream.h"	  // for rptwofstream
#else
#include "..\MIT_Lib\rptostream.h"	// for rptwofstream
#endif

#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptJTG_D62_04::CRptJTG_D62_04()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);

	m_PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(m_PscdD);

	if ( m_PscdD.iDgnCode==JTG_D62_04 || m_PscdD.iDgnCode==CJJ_11_2011 )
	{
		// Set ForceCtrl Pointer.
		m_pForcCtrl = m_DataCtrl.Get_ForceCtrlPointer();
		if ( m_pForcCtrl==NULL ) { ASSERT(0); return; }

		SetDataCtrlPointer(&m_DataCtrl, m_pForcCtrl);

		m_pJTG_D62_04 = m_DataCtrl.Get_DgnJtgPointer();
	}

	Set_PrintFlag(TRUE);
	Set_InitData();
	m_pJTG_D62_04->Set_ReportFlag(TRUE);

	/////////////////////////////////////////////////

	CString strFilePathName = GetFilePathName();
	m_pXmlDoc->SetFilePathName(strFilePathName);
	m_pXslDoc->SetFilePathName(strFilePathName);

	m_TextPadding1CssK = 0;
	m_TextPadding2CssK = 0;
	m_TextValueCssK = 0;
	m_TextPicCaptionCssK = 0;
	m_TDCenterCssK = 0;

	m_strLength = _T("mm");
	m_strForce  = _T("kN");
	m_strMoment = _T("kN-m");
	m_strMpa    = _T("MPa");

	m_dForcekN   = 0.001;
	m_dMomentkNM = 0.000001;
	m_dLengthM   = 0.001;
	m_dDensitykNM = m_dForcekN / pow(m_dLengthM, 3.0);

	m_RptBdcy.Initialize();
	m_RptCums.Initialize();
	m_RptCrmt.Initialize();
}

CRptJTG_D62_04::~CRptJTG_D62_04()
{

}
/*
void CRptJTG_D62_04::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl, CCRCForceCtrl* pForcCtrl)
{
	m_pDoc        = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	m_pDataCtrl   = pDataCtrl;
	m_pForcCtrl   = pForcCtrl;
	if(!m_pDoc->IsPostMode())	ASSERT(0);
}
*/
CString CRptJTG_D62_04::GetFilePathName()
{
	CString strPathName = m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strPathName);
	CString strFilePathName=FileCtrl.GetFilePathAndNameWithoutExtension() + _T("_rpt");

	return strFilePathName;
}

BOOL CRptJTG_D62_04::Get_DetailBdcyPos(ElemPairK ElemK)
{
	// Get Posx.
	T_POSX_D PosxD;
	PosxD.Initialize();
	if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
	}
	else if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
	}
	else ASSERT(0);

	BOOL bPrtI=FALSE, bPrtJ=FALSE;
	if ( PosxD.iMomChk[0]==0 )	return FALSE;											// NONE.
	else if ( PosxD.iMomChk[0]==1 ) { bPrtI=TRUE; bPrtJ=FALSE; }	// I.
	else if ( PosxD.iMomChk[0]==2 ) { bPrtI=FALSE; bPrtJ=TRUE; }	// J.
	else if ( PosxD.iMomChk[0]==3 ) { bPrtI=TRUE; bPrtJ=TRUE; }	// I&J.
	else	ASSERT(0);
	// Get Bdcy.
	T_BDCY_JTG BdcyJtgD;
	BdcyJtgD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBdcyJtgData(ElemK, BdcyJtgD) )	return FALSE;
	BdcyJtgD.BdcyBase[0].bCHK = (BdcyJtgD.BdcyBase[0].bCHK && bPrtI ? TRUE : FALSE);	// 0=I-MAX.
	BdcyJtgD.BdcyBase[1].bCHK = FALSE;									  	                            // 1=I-MIN.
	BdcyJtgD.BdcyBase[2].bCHK = (BdcyJtgD.BdcyBase[2].bCHK && bPrtJ ? TRUE : FALSE);	// 2=J-MAX.
	BdcyJtgD.BdcyBase[3].bCHK = FALSE;										                              // 3=J-MIN.

	BOOL bExecutePrt = FALSE;
	for ( int i=0; i<4; i++ )
	{
		if ( BdcyJtgD.BdcyBase[i].bCHK ) bExecutePrt = TRUE;
	}

	if ( !bExecutePrt ) return FALSE;
	// Print on Calculating.
	T_DETAIL_STR DetailStr; DetailStr.Initialize();//
	return m_pDataCtrl->Get_DgnJtgPointer()->Get_JtgBdcy(ElemK, BdcyJtgD, DetailStr, bPrtI, bPrtJ);
}

BOOL CRptJTG_D62_04::Get_DetailBdcyNeg(ElemPairK ElemK)
{
	// Get Posx.
	T_POSX_D PosxD;
	PosxD.Initialize();
	if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
	}
	else if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
	}
	else ASSERT(0);
	BOOL bPrtI=FALSE, bPrtJ=FALSE;
	if ( PosxD.iMomChk[1]==0 )	return FALSE;											// NONE.
	else if ( PosxD.iMomChk[1]==1 ) { bPrtI=TRUE; bPrtJ=FALSE; }	// I.
	else if ( PosxD.iMomChk[1]==2 ) { bPrtI=FALSE; bPrtJ=TRUE; }	// J.
	else if ( PosxD.iMomChk[1]==3 ) { bPrtI=TRUE; bPrtJ=TRUE; }	// I&J.
	else	ASSERT(0);
	// Get Bdcy.
	T_BDCY_JTG BdcyJtgD;
	BdcyJtgD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBdcyJtgData(ElemK, BdcyJtgD) )	return FALSE;
	BdcyJtgD.BdcyBase[0].bCHK = FALSE;								  		                          // 0=I-MAX.
	BdcyJtgD.BdcyBase[1].bCHK = (BdcyJtgD.BdcyBase[1].bCHK && bPrtI ? TRUE : FALSE);	// 1=I-MIN.
	BdcyJtgD.BdcyBase[2].bCHK = FALSE;										                            // 2=J-MAX.
	BdcyJtgD.BdcyBase[3].bCHK = (BdcyJtgD.BdcyBase[3].bCHK && bPrtJ ? TRUE : FALSE);	// 3=J-MIN.

	BOOL bExecutePrt = FALSE;
	for ( int i=0; i<4; i++ )
	{
		if ( BdcyJtgD.BdcyBase[i].bCHK ) bExecutePrt = TRUE;
	}

	if ( !bExecutePrt ) return FALSE;

	// Print on Calculating.
	T_DETAIL_STR DetailStr; DetailStr.Initialize();//
	return m_pDataCtrl->Get_DgnJtgPointer()->Get_JtgBdcy(ElemK, BdcyJtgD, DetailStr, bPrtI, bPrtJ);
}

//!/
BOOL CRptJTG_D62_04::MakeData()
{
	if ( !CReportBase::MakeData() ) return FALSE;

	T_UNIT_INDEX CodeIndex;
	CodeIndex = m_DataCtrl.Get_CodeUnitIndex();

	m_strForceUnit = CUnitCtrl::m_strForce;
	m_strMomentUnit = CUnitCtrl::m_strMoment;
	m_strLengthUnit = CUnitCtrl::m_strLength;

	if ( CodeIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM && CodeIndex.nBase_Force == D_UNITSYS_FORCE_INDEX_N )
	{
		m_strForce  = _T("kN");
		m_strForceN = _T("N");
		m_strMoment = _T("kN-m");
		m_strMomentN= _T("N-mm");
		m_strLength = _T("mm");
		m_strStress = _T("Mpa");

		m_strLengthUnit = _T("(mm)");
		m_strLengthUnitM = _T("(m)");
		m_strAngleUnitRad = _T("(rad)");
		m_strForceUnit = _T("(kN)");
		m_strMomentUnit = _T("(kN-m)");
		m_strStressUnit = _T("(MPa)");
		m_strForceUnitN = _T("(N)");
		m_strMomentUnitN = _T("(N-mm)");
		m_strDensityUnit = _T("(kN/m3)");

		m_arStressUnit.RemoveAll();
		AddChar(m_arStressUnit, _T("(N/mm2)"), 1, 5, 1);
		m_arstrAreaUnit.RemoveAll();
		AddChar(m_arstrAreaUnit, _T("(mm2)"), 1, 3, 1);
		m_arstrStifUnit.RemoveAll();
		AddChar(m_arstrStifUnit, _T("(mm4)"), 1, 3, 1);
		m_arstrSectUnit.RemoveAll();
		AddChar(m_arstrSectUnit, _T("(mm3)"), 1, 3, 1);

	}
	/*
	else if(CodeIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM && CodeIndex.nBase_Force == D_UNITSYS_FORCE_INDEX_KG)
	{
		m_strLengthUnit = _T("(cm)");
		m_strForceUnit = _T("(tonf)");
		m_strMomentUnit = _T("(tonf-m)");
		m_arStressUnit.RemoveAll();
		AddChar(m_arStressUnit, _T("(kgf/cm2)"), 1, 7, 1);
	}
	*/
	else ASSERT(0);

	InitialarCharK();

	if ( !MakeDataFromDB() ) return FALSE;

	InitializeData();
	if ( !MakeCoverData() ) return FALSE;
	if ( !MakeContents() ) return FALSE;
	if ( !MakeCodes() ) return FALSE;
	if ( !MakeDesignParameter() ) return FALSE;
	if ( !MakeMaterialData() ) return FALSE;
	if ( !MakeModelData() ) return FALSE;
	if ( !MakeLComData() ) return FALSE;
	if ( !MakeCheckResultTable() ) return FALSE;
	////for china
#if defined(_CH)
	if ( !MakeDetailReportData() ) return FALSE;
#endif

	// Change to Original Unit
	//m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);

	return TRUE;
}

BOOL CRptJTG_D62_04::MakeDataFromDB()
{
	int i;
	GetMaterialProperty();

	// Element.  
	m_iElemNum = 0;
	m_arElemKeyList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetElemKeyList(m_arElemKeyList);
	for ( i=0; i<m_arElemKeyList.GetSize(); i++ )
	{
		T_ELEM_D ElemD;
		T_ELEM_K ElemK = m_arElemKeyList.GetAt(i);
		if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) ) continue;
		if ( m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp) )
		{
			m_iElemNum++;
		}
	}
	// Node.
	CArray<T_NODE_K, T_NODE_K> NodeKList; NodeKList.RemoveAll();
	m_pDoc->m_pPostCtrl->GetNodeKeyListPost(NodeKList);
	m_iNodeNum = NodeKList.GetSize();
	// Tendon.
	m_iTendonNum = m_arTdnaK.GetSize();
	// Boundary.  
	m_iBoundaryNum = m_pDoc->m_pAttrCtrl->GetCountCons();;
	// Stage.  
	m_arStagK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetStagKeyList(m_arStagK);
	m_iStageNum = m_arStagK.GetSize();

	// Group.
	m_arGroupKeyList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetGrupKeyList(m_arGroupKeyList);

	return TRUE;
}

void CRptJTG_D62_04::InitializeData()
{
	m_pRptData->m_TitleType[3] = 1; // Title4의 형식을 (1)로 변경
	m_iChapNum = 1;
}

BOOL CRptJTG_D62_04::MakeDefaultStyle()
{
	m_pRptData->InitializeRptData();
	if ( !m_pRptData->MakeDefaultStyle() ) return FALSE;

	CString strCssName = _T("");
	T_CSS_K CssK=0;
	T_CSS_RPT CssD; CssD.Initialize();
	//Cover SuperTitle
	CssK = 0;
	CssD.Initialize();
	strCssName = _T("SuperTitle");
	m_pRptData->GetNameToCssK(strCssName, CssK);
	if ( m_pRptData->GetCssData(CssK, CssD) )
	{
		CssD.strFontSize   = _T("20pt");
		CssD.strFontStyle  = _T("normal");
		CssD.strFontWeight = _T("bold");
		CssD.strLineHeight = _T("20px");
		CssD.strTopMargin  = _T("100px");
		m_pRptData->AddCssData(CssK, CssD);
	}

	//Cover Title
	CssK = 0;
	CssD.Initialize();
	strCssName = _T("Title");
	m_pRptData->GetNameToCssK(strCssName, CssK);
	if ( m_pRptData->GetCssData(CssK, CssD) )
	{
		CssD.strFontSize    = _T("36pt");
		CssD.strFontFamily  = _T("arial");
		CssD.strFontWeight  = _T("bold");
		CssD.strtxtDeco     = _T("underline");
		CssD.strLineHeight   = _T("30pt");//_T("40px");
		CssD.strBottomMargin = _T("50pt");//_T("300px");
		CssD.strTopMargin    = _T("3.75pt");//_T("150px");
		m_pRptData->AddCssData(CssK, CssD);
	}

	// Add by GAY.('06.05.30). Redefine Cover Sub Title. 
	CssK = 0;
	CssD.Initialize();
	strCssName = _T("SubTitle");
	m_pRptData->GetNameToCssK(strCssName, CssK);
	if ( m_pRptData->GetCssData(CssK, CssD) )
	{
		CssD.strFontSize    = _T("15pt");
		CssD.strFontFamily  = _T("arial");
		CssD.strFontWeight  = _T("bold");
		CssD.strLineHeight   = _T("30pt");//_T("40px");
		CssD.strBottomMargin = _T("0pt");//_T("300px");
		CssD.strTopMargin    = _T("0pt");//_T("150px");
		m_pRptData->AddCssData(CssK, CssD);
	}

	// Commented by GAY.('06.05.30). Set table cell text color as Black.
	/*
	CssK = 0;
	T_CSS_RPT DefaultCellCssD; DefaultCellCssD.Initialize();
	strCssName = _T("Cell");
	m_pRptData->GetNameToCssK(strCssName, CssK);
	if(m_pRptData->GetCssData(CssK,DefaultCellCssD))
	{
	  DefaultCellCssD.strColor     = _T("#0000ff");
		m_pRptData->AddCssData(CssK, DefaultCellCssD);
	}
	*/
	////////////////////////////Only JTG_D62_04 Report/////////////////////////////////////////////
	strCssName = _T("Text");
	CssK = 0;
	m_pRptData->GetNameToCssK(strCssName, CssK);
	T_CSS_RPT DefaultTextCssD; DefaultTextCssD.Initialize();
	if ( m_pRptData->GetCssData(CssK, DefaultTextCssD) )
	{
		T_CSS_RPT TextValueCssD;
		TextValueCssD = DefaultTextCssD;
		TextValueCssD.strStyleName = _T("TextValue");
		TextValueCssD.strColor     = _T("#0000ff");
		m_pRptData->AddCssData(m_TextValueCssK, TextValueCssD);

		T_CSS_RPT TextPadding1CssD; TextPadding1CssD = DefaultTextCssD;
		TextPadding1CssD.strStyleName = _T("TextPadding1");
		TextPadding1CssD.strColor     = _T("#0000ff");
		TextPadding1CssD.strLeftMargin = _T("40px");
		m_pRptData->AddCssData(m_TextPadding1CssK, TextPadding1CssD);

		T_CSS_RPT TextPadding2CssD; TextPadding2CssD = DefaultTextCssD;
		TextPadding2CssD.strStyleName = _T("TextPadding2");
		TextPadding2CssD.strColor     = _T("#0000ff");
		TextPadding2CssD.strLeftMargin = _T("50px");
		m_pRptData->AddCssData(m_TextPadding2CssK, TextPadding2CssD);
	}

	strCssName = _T("Caption");
	CssK = 0;
	m_pRptData->GetNameToCssK(strCssName, CssK);
	T_CSS_RPT DefaultCaptionCssD; DefaultCaptionCssD.Initialize();
	if ( m_pRptData->GetCssData(CssK, DefaultCaptionCssD) )
	{
		T_CSS_RPT TextPicCaptionCssD; TextPicCaptionCssD = DefaultCaptionCssD;
		TextPicCaptionCssD.strStyleName = _T("TextPicCaption");
		TextPicCaptionCssD.strTextAlign = _T("center");
		m_pRptData->AddCssData(m_TextPicCaptionCssK, TextPicCaptionCssD);
	}

	strCssName = _T("Cell");
	CssK = 0;
	m_pRptData->GetNameToCssK(strCssName, CssK);
	T_CSS_RPT DefaultTDCssD; DefaultTDCssD.Initialize();
	if ( m_pRptData->GetCssData(CssK, DefaultTDCssD) )
	{
		T_CSS_RPT TDCenterCssD; TDCenterCssD = DefaultTDCssD;
		TDCenterCssD.strStyleName = _T("Cell_Center");
		TDCenterCssD.strTextAlign = _T("center");
		m_pRptData->AddCssData(m_TDCenterCssK, TDCenterCssD);
	}

	T_CSS_K ColCssK=0;
	T_CSS_RPT ColCss; ColCss.Initialize();
	ColCss.strStyleName=_T("Div2Col");
	ColCss.strWidth = _T("50%");
	m_pRptData->AddCssData(ColCssK, ColCss);

	// Add by GAY.('06.05.30). Set Background color as white.
	strCssName = _T("RowHeader");
	CssK = 0;
	m_pRptData->GetNameToCssK(strCssName, CssK);
	if ( m_pRptData->GetCssData(CssK, CssD) )
	{
		CssD.strBgColor = _LSX(#FFFFFF);
		m_pRptData->AddCssData(CssK, CssD);
	}

	// Add by GAY.('06.05.30). Set table cell text color as black.
	strCssName = _T("Cell_Center");
	CssK = 0;
	m_pRptData->GetNameToCssK(strCssName, CssK);
	if ( m_pRptData->GetCssData(CssK, CssD) )
	{
		CssD.strColor = _LSX(#000000);
		m_pRptData->AddCssData(CssK, CssD);
	}

	// Add by GAY.('06.05.30). Set Page-Break-Before Attribute : FALSE.
	strCssName = _T("제목1");
	CssK = 0;
	CssD.Initialize();
	m_pRptData->GetNameToCssK(strCssName, CssK);
	if ( m_pRptData->GetCssData(CssK, CssD) )
	{
		CssD.bPageBreak = FALSE;
		m_pRptData->AddCssData(CssK, CssD);
	}

	// 제목1 + Set Page-Break-Before Attribute : TRUE.  
	CssK = 0;
	CssD.strStyleName = _T("제목1_PB");
	CssD.bPageBreak = TRUE;
	m_pRptData->AddCssData(CssK, CssD);

	return TRUE;
}

// 0-1. 표지 Data 구성.
BOOL CRptJTG_D62_04::MakeCoverData()
{
	T_REPORT_RPT reportD;
	reportD.Initialize();
	m_pRptData->GetReportData(reportD);

	T_PJCF_D pjcfD;
	if ( !m_pDoc->m_pAttrCtrl->GetPjcf(pjcfD) )
	{
		pjcfD.Initialize();
	}

	reportD.strTitle.Format(_T("<br/>%s %s"), pjcfD.strProject, _LS(IDS_DGN_PSC_RPTJTG_Report));
	reportD.TitleCssK = m_pRptData->m_DefaultCss.CoverTitleCssK;

	// Change by GAY.('06.05.29). Project Information에 데이터가 입력되지 않았을 경우에는 _T("XXX")로 표기 요구. (From China)
	CString strName1 = _T(" : ") + ((pjcfD.strReviewName[0].GetLength() > 0)? pjcfD.strReviewName[0] : _T("XXX"));
	CString strName2 = _T(" : ") + ((pjcfD.strReviewName[1].GetLength() > 0)? pjcfD.strReviewName[1] : _T("XXX"));
	CString strName3 = _T(" : ") + ((pjcfD.strReviewName[4].GetLength() > 0)? pjcfD.strReviewName[4] : _T("XXX"));

	CString strText = _T("<br/><br/><br/>");
	strText += _LS(IDS_DGN_PSC_RPTJTG_Engineer) + strName1 + _T("<br/>");
	strText += _LS(IDS_DGN_PSC_RPTJTG_Examination) + strName2 + _T("<br/>");
	strText += _LS(IDS_DGN_PSC_RPTJTG_Approved) + strName3 + _T("<br/>");
	strText += _T("<br/><br/><br/><br/><br/><br/>") + pjcfD.strUser + _T("<br/>");
	strText += GetFileDate() + _T("<br/>");

	reportD.strSubTitle = strText;
	reportD.SubTitleCssK = m_pRptData->m_DefaultCss.CoverSubTitleCssK;

	reportD.strDate = GetFileDate();
	reportD.DateCssK = m_pRptData->m_DefaultCss.CoverDateCssK;

	m_pRptData->ModifyReportData(reportD);

	return TRUE;
}

// 0-2. 목차 Data를 구성.
BOOL CRptJTG_D62_04::MakeContents()
{
	return TRUE;
}

// 1. 설계기준 Data를 구성함.
BOOL CRptJTG_D62_04::MakeCodes()
{
	T_CHAP_K ChapK, Chap2K;
	T_CHAP_RPT ChapD; ChapD.Initialize();
	T_CHAP_RPT Chap2D; Chap2D.Initialize();

	T_CSS_K cssK;
	CString cssName = _T("제목1_PB");
	m_pRptData->GetNameToCssK(cssName, cssK);

	ChapD = MakeChapter(1, m_iChapNum++, _LS(IDS_DGN_PSC_RPTJTG_Design_Standard), cssK);

	CStringArray arStringCode;
	//arStringCode.SetSize(6);
	arStringCode.Add(_LS(IDS_DGN_PSC_RPTJTG_St_Name_B01_2004_Include_Bracket));
	arStringCode.Add(_LS(IDS_DGN_PSC_RPTJTG_St_Name_D60_2004_Info));
	arStringCode.Add(_LS(IDS_DGN_PSC_RPTJTG_St_Name_D62_2004_Info));
	arStringCode.Add(_LS(IDS_DGN_PSC_RPTJTJ_St_Name_024_85_Include_Bracket));
	arStringCode.Add(_LS(IDS_DGN_PSC_RPTJTJ_St_Name_004_89_Include_Bracket));

	if ( m_PscdD.iDgnCode == CJJ_11_2011 )
	{
		arStringCode.Add(_LS(IDS_DGN_PSC_RPTCJJ_St_Name_11_2011_Include_Bracket));////add by juhonghua 2012-3-22.
	}

	int iChap2DCount = 1;
	for ( int i=0; i<arStringCode.GetSize(); i++ )
	{
		Chap2D = MakeChapter(2, iChap2DCount, arStringCode[i]);

		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
		iChap2DCount++;
	}

	m_pRptData->AddChapData(ChapK, ChapD);

	T_REPORT_RPT Report; Report.Initialize();
	m_pRptData->GetReportData(Report);
	Report.arChapK.Add(ChapK);
	m_pRptData->ModifyReportData(Report);

	return TRUE;
}

// 2. 설계자료 Data를 구성함.
BOOL CRptJTG_D62_04::MakeDesignParameter()
{
	T_CHAP_K ChapK, Chap2K;
	T_CHAP_RPT ChapD; ChapD.Initialize();
	T_CHAP_RPT Chap2D; Chap2D.Initialize();

	CStringArray arString; arString.RemoveAll();
	GetDesignStringPara(arString);
	if ( arString.GetSize()==0 ) return FALSE;

	ChapD = MakeChapter(1, m_iChapNum++, _LS(IDS_DGN_PSC_RPTJTG_Design_Information));

	CStringArray arStrParam;
	arStrParam.SetSize(5);
	CString strProductNameVer;
	strProductNameVer.Format(CString(_LS(IDS_DGN_PSC_RPTJTG_MIDAS_Civil_Title))+_LS(IDS_DGN_PSC_RPTJTG_Release_Number), D_RELEASE_NO);
	arStrParam[0].Format(_T("%s : %s"), _LS(IDS_DGN_PSC_RPTJTG_Used_Program), strProductNameVer);
	arStrParam[1].Format(_T("%s : %s"), _LS(IDS_DGN_PSC_RPTJTG_Member_Force), arString.GetAt(0));
	arStrParam[2].Format(_T("%s : %s"), _LS(IDS_DGN_PSC_RPTJTG_Bridge_Type), arString.GetAt(1));
	arStrParam[3].Format(_T("%s : %s"), _LS(IDS_DGN_PSC_RPTJTG_Safety_Factor), arString.GetAt(2));
	arStrParam[4].Format(_T("%s : %s"), _LS(IDS_DGN_PSC_RPTJTG_Pouring_Method), arString.GetAt(3));

	int iChap2DCount = 1;
	for ( int i=0; i<5; i++ )
	{
		Chap2D = MakeChapter(2, iChap2DCount, arStrParam[i]);

		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
		iChap2DCount++;
	}

	m_pRptData->AddChapData(ChapK, ChapD);

	T_REPORT_RPT Report; Report.Initialize();
	m_pRptData->GetReportData(Report);
	Report.arChapK.Add(ChapK);
	m_pRptData->ModifyReportData(Report);

	return TRUE;
}

// 3. 재질 Data를 구성함.
BOOL CRptJTG_D62_04::MakeMaterialData()
{
	if ( m_arMatElem.GetSize()==0 ) return FALSE;

	CString strTitle, strText;

	T_CHAP_K ChapK, Chap2K;
	T_PARA_K ParaK;
	T_CHAP_RPT ChapD; ChapD.Initialize();
	T_CHAP_RPT Chap2D; Chap2D.Initialize();
	T_PARA_RPT ParaD; ParaD.Initialize();

	CStringArray arString; arString.RemoveAll();
	GetDesignStringPara(arString);
	if ( arString.GetSize()==0 ) return FALSE;

	ChapD = MakeChapter(1, m_iChapNum++, _LS(IDS_DGN_PSC_RPTJTG_Material));

	int iChap2DCount = 1;

	Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_DGN_PSC_RPTJTG_Concrete));
	if ( MakeConcData(iChap2DCount, ParaD) )
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap2D.arParaK.Add(ParaK);
		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
	}

	Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_DGN_PSC_RPTJTG_Tendon));
	if ( MakePrestressRebar(iChap2DCount, ParaD) )
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap2D.arParaK.Add(ParaK);
		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
	}

	// Changed by GAY.('06.05.30). China requested merged rebar table. (MainRebar + SubRebar).
	Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_DGN_PSC_RPTJTG_Normal_Rebar));
	if ( MakeRebar(iChap2DCount, ParaD) )
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap2D.arParaK.Add(ParaK);
		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
	}

	/*
	Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_DGN_PSC_RPTJTG_Longitudinal_Rbar));
	if(MakeTensionRebar(iChap2DCount, ParaD))
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap2D.arParaK.Add(ParaK);
		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
	}

	Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_DGN_PSC_RPTJTG_Stirrup_Rbar));
	if(MakeShearRebar(iChap2DCount, ParaD))
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap2D.arParaK.Add(ParaK);
		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
	}
	*/

	m_pRptData->AddChapData(ChapK, ChapD);

	T_REPORT_RPT Report; Report.Initialize();
	m_pRptData->GetReportData(Report);
	Report.arChapK.Add(ChapK);
	m_pRptData->ModifyReportData(Report);

	return TRUE;
}

// 4. 모델에 대한 간단한 소개 Data 생성.
BOOL CRptJTG_D62_04::MakeModelData()
{
	CString strTitle, strText;

	T_CHAP_K ChapK, Chap2K;
	T_PARA_K ParaK;
	T_CHAP_RPT ChapD; ChapD.Initialize();
	T_CHAP_RPT Chap2D; Chap2D.Initialize();
	T_PARA_RPT ParaD; ParaD.Initialize();

	CStringArray arString; arString.RemoveAll();
	GetDesignStringPara(arString);
	if ( arString.GetSize()==0 ) return FALSE;

	ChapD = MakeChapter(1, m_iChapNum++, _LS(IDS_DGN_PSC_RPTJTG_Model_Simple_Intro));

	int iChap2DCount = 1;

	// Beam Element Number.
	strText.Format(_LS(IDS_DGN_PSC_RPTJTG_Beam_Element_Num), m_iElemNum);
	Chap2D = MakeChapter(2, iChap2DCount++, strText);
	m_pRptData->AddChapData(Chap2K, Chap2D);
	ChapD.arSubChapK.Add(Chap2K);

	// Node Number.
	strText.Format(_LS(IDS_DGN_PSC_RPTJTG_Node_Num), m_iNodeNum);
	Chap2D = MakeChapter(2, iChap2DCount++, strText);
	m_pRptData->AddChapData(Chap2K, Chap2D);
	ChapD.arSubChapK.Add(Chap2K);

	// Tendon Number.
	strText.Format(_LS(IDS_DGN_PSC_RPTJTG_Tendon_Num), m_iTendonNum);
	Chap2D = MakeChapter(2, iChap2DCount++, strText);
	m_pRptData->AddChapData(Chap2K, Chap2D);
	ChapD.arSubChapK.Add(Chap2K);

	// Support Number.
	strText.Format(_LS(IDS_DGN_PSC_RPTJTG_Support_Num), m_iBoundaryNum);
	Chap2D = MakeChapter(2, iChap2DCount++, strText);
	m_pRptData->AddChapData(Chap2K, Chap2D);
	ChapD.arSubChapK.Add(Chap2K);

	// Stage Number.
	if ( m_iStageNum > 0 )
	{
		strText.Format(_LS(IDS_DGN_PSC_RPTJTG_Stage_Num), m_iStageNum);
		Chap2D = MakeChapter(2, iChap2DCount++, strText);

		if ( MakeStageGroupSummaryData(iChap2DCount, ParaD) )
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);
		}

		/*
		int iChap3DCount = 1;

		// Element Group
		Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Element_Group_Activation_Summary));
		if(MakeElemGroupData(iChap3DCount, ParaD))
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap3D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap3K, Chap3D);
			Chap2D.arSubChapK.Add(Chap3K);
		}

		// Boundary Group
		Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Boundary_Group_Activation_Summary));
		if(MakeBoundaryGroupData(iChap3DCount, ParaD))
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap3D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap3K, Chap3D);
			Chap2D.arSubChapK.Add(Chap3K);
		}

		// Load Group
		Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Load_Group_Activation_Summary));
		if(MakeLoadGroupData(iChap3DCount, ParaD))
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap3D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap3K, Chap3D);
			Chap2D.arSubChapK.Add(Chap3K);
		}
		*/

		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
	}

	MakeServiceStageLoadData(iChap2DCount++, ChapD);

	m_pRptData->AddChapData(ChapK, ChapD);

	T_REPORT_RPT Report; Report.Initialize();
	m_pRptData->GetReportData(Report);
	Report.arChapK.Add(ChapK);
	m_pRptData->ModifyReportData(Report);

	return TRUE;
}

// 5. 하중조합 설명 Data 구성함.
BOOL CRptJTG_D62_04::MakeLComData()
{
	CString strText;
	T_CHAP_K ChapK, Chap2K, Chap3K;
	T_PARA_K ParaK;
	T_CHAP_RPT ChapD; ChapD.Initialize();
	T_CHAP_RPT Chap2D; Chap2D.Initialize();
	T_CHAP_RPT Chap3D; Chap3D.Initialize();
	T_PARA_RPT ParaD; ParaD.Initialize();

	CStringArray arString; arString.RemoveAll();
	GetDesignStringPara(arString);
	if ( arString.GetSize()==0 ) return FALSE;

	ChapD = MakeChapter(1, m_iChapNum++, _LS(IDS_DGN_PSC_RPTJTG_Load_Combination_Description));

	int iChap2DCount = 1;
	Chap2D = MakeChapter(2, iChap2DCount++, _LS(IDS_DGN_PSC_RPTJTG_Load_Case_Description));

	int iChap3DCount = 1;

	// Static Load Cases.
	Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Static_Load_Cases));
	if ( MakeStaticLoadCaseData(iChap3DCount, ParaD) )
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap3D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap3K, Chap3D);
		Chap2D.arSubChapK.Add(Chap3K);
	}

	// Response Spectrum.
	Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Response_Spectrum_Load_Cases));
	if ( MakeResponseSpectrumData(iChap3DCount, ParaD) )
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap3D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap3K, Chap3D);
		Chap2D.arSubChapK.Add(Chap3K);
	}

	// Time History Analysis.
	Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Time_History_Load_Cases));
	if ( MakeTimeHistoryData(iChap3DCount, ParaD) )
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap3D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap3K, Chap3D);
		Chap2D.arSubChapK.Add(Chap3K);
	}

	// Moving Load Cases.
	Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Moving_Load_Cases));
	if ( MakeMovingLoadData(iChap3DCount, ParaD) )
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap3D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap3K, Chap3D);
		Chap2D.arSubChapK.Add(Chap3K);
	}

	// Settlement Load Cases.
	Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Settlement_Load_Cases));
	if ( MakeSettlementData(iChap3DCount, ParaD) )
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap3D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap3K, Chap3D);
		Chap2D.arSubChapK.Add(Chap3K);
	}

	// Grid Load Cases.
	Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Grid_Load_Cases));
	if ( MakeGridLoadData(iChap3DCount, ParaD) )
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap3D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap3K, Chap3D);
		Chap2D.arSubChapK.Add(Chap3K);
	}

	m_pRptData->AddChapData(Chap2K, Chap2D);
	ChapD.arSubChapK.Add(Chap2K);

	/////////////////////////////////////////////
	Chap2D = MakeChapter(2, iChap2DCount++, _LS(IDS_DGN_PSC_RPTJTG_Load_Combination_Description));
	iChap3DCount = 1;

	Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_LoadComb_CaseNames));
	if ( MakeLoadCombCaseData(iChap3DCount, ParaD) )
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap3D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap3K, Chap3D);
		Chap2D.arSubChapK.Add(Chap3K);
	}

	Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_LoadComb));
	if ( MakeLoadCombData(iChap3DCount, ParaD) )
	{
		m_pRptData->AddParaData(ParaK, ParaD);
		Chap3D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap3K, Chap3D);
		Chap2D.arSubChapK.Add(Chap3K);
	}

	m_pRptData->AddChapData(Chap2K, Chap2D);
	ChapD.arSubChapK.Add(Chap2K);

	m_pRptData->AddChapData(ChapK, ChapD);

	T_REPORT_RPT Report; Report.Initialize();
	m_pRptData->GetReportData(Report);
	Report.arChapK.Add(ChapK);
	m_pRptData->ModifyReportData(Report);

	return TRUE;
}

// 6. 검토결과 Table Data 구성.
BOOL CRptJTG_D62_04::MakeCheckResultTable()
{
	if ( !CheckExistResultTableItem() ) return TRUE;

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	T_UNIT_INDEX CodeIndex = m_DataCtrl.Get_CodeUnitIndex();
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeIndex);
	//++++++++++++++++++++++++++++++++++++++++++++

	CString strTitle, strText;
	T_CHAP_K ChapK, Chap2K;
	T_PARA_K ParaK;
	T_CHAP_RPT ChapD; ChapD.Initialize();
	T_CHAP_RPT Chap2D; Chap2D.Initialize();
	T_PARA_RPT ParaD; ParaD.Initialize();

	T_PSCD_D pscdD;
	pscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(pscdD);

	ChapD = MakeChapter(1, m_iChapNum++, _LS(IDS_DGN_PSC_RPTJTG_CheckResult_Table));

	// Modify by GAY. MNET:2444. ('06.09.08). Apply print option.
	// T_PSCD_D > bMomPrint.
	//    0:시공단계 정단면 응력검토, 1:인장영역철근의 인장응력검토, 2:사용단계 정단면 균열검토.
	//    3:사용단계 경사단면 균열검토, 4:사용단계 정단면 압축응력 검토, 5:사용단계 경사단면 압축주응력검토.    
	// T_PSCD_D > bShrPrint.
	//    0:사용단계 균열폭 검토, 1:사용단계 정단면휨검토, 2:사용단계 경사단면 전단검토, 3:사용단계 비틀림검토.

	int iChap2DCount = 1;

	// _T("시공단계 정단면 응력검토")  
	if ( pscdD.bMomPrint[0] )
	{
		Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_TB_RESULTVIEW_JTG_FYSG));
		if ( MakeConstructStageVertStressData(iChap2DCount, ParaD) )
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap2K, Chap2D);
			ChapD.arSubChapK.Add(Chap2K);
		}
	}

	// _T("인장영역철근의 인장응력검토")
	if ( pscdD.bMomPrint[1] )
	{
		Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_TB_RESULTVIEW_JTG_FYPC));
		if ( MakeTensRebarStressData(iChap2DCount, ParaD) )
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap2K, Chap2D);
			ChapD.arSubChapK.Add(Chap2K);
		}
	}

	// _T("사용단계 정단면 균열검토")
	if ( pscdD.iBrgType < 2 && pscdD.bMomPrint[2] )  // Only Bridge Type is Fully PSC or Partially PSC(A).
	{
		Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_TB_RESULTVIEW_JTG_FYCM_MIN));
		if ( MakeServiceStageCrackData(iChap2DCount, ParaD) )
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap2K, Chap2D);
			ChapD.arSubChapK.Add(Chap2K);
		}
	}

	// _T("사용단계 경사단면 균열검토")
	if ( pscdD.bMomPrint[3] )
	{
		Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_TB_RESULTVIEW_JTG_FPMS_MIN));
		if ( MakeServiceStageCrackSlopeData(iChap2DCount, ParaD) )
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap2K, Chap2D);
			ChapD.arSubChapK.Add(Chap2K);
		}
	}

	// _T("사용단계 정단면 압축응력검토")
	if ( pscdD.bMomPrint[4] )
	{
		Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_TB_RESULTVIEW_JTG_FYCM_MAX));
		if ( MakeServiceStageCompStressData(iChap2DCount, ParaD) )
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap2K, Chap2D);
			ChapD.arSubChapK.Add(Chap2K);
		}
	}

	// _T("사용단계 경사단면 압축주응력검토")
	if ( pscdD.bMomPrint[5] )
	{
		Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_TB_RESULTVIEW_JTG_FPMS_MAX));
		if ( MakeServiceStageCompStressSlopeData(iChap2DCount, ParaD) )
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap2K, Chap2D);
			ChapD.arSubChapK.Add(Chap2K);
		}
	}

	if ( pscdD.iBrgType == 2 && pscdD.bShrPrint[0] )  // Only Partially PSC(B).
	{
		// _T("사용단계 균열폭검토")
		Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_TB_RESULTVIEW_JTG_BDCW));
		if ( MakeServiceStageCrackWidthData(iChap2DCount, ParaD) )
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap2K, Chap2D);
			ChapD.arSubChapK.Add(Chap2K);
		}
	}

	// _T("사용단계 정단면 휨검토")
	if ( pscdD.bShrPrint[1] )
	{
		Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_TB_RESULTVIEW_JTG_BDCY));
		if ( MakeServiceStageMomentCheckData(iChap2DCount, ParaD) )
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap2K, Chap2D);
			ChapD.arSubChapK.Add(Chap2K);
		}
	}

	// _T("사용단계 경사단면 전단검토")
	if ( pscdD.bShrPrint[2] )
	{
		Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_TB_RESULTVIEW_JTG_CUMS));
		if ( MakeServiceStageShearCheckData(iChap2DCount, ParaD) )
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap2K, Chap2D);
			ChapD.arSubChapK.Add(Chap2K);
		}
	}

	// _T("사용단계 비틀림검토")
	if ( pscdD.bShrPrint[3] )
	{
		Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_TB_RESULTVIEW_JTG_CRMT));
		if ( MakeServiceStageTorsionCheckData(iChap2DCount, ParaD) )
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap2K, Chap2D);
			ChapD.arSubChapK.Add(Chap2K);
		}
	}

	if ( pscdD.bShrPrint[1] )
	{
		Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_TB_RESULTVIEW_JTG_CSCS));
		if ( MakeServiceStageCompressionCheckData(iChap2DCount, ParaD) )
		{
			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);
			m_pRptData->AddChapData(Chap2K, Chap2D);
			ChapD.arSubChapK.Add(Chap2K);
		}
	}

	m_pRptData->AddChapData(ChapK, ChapD);

	T_REPORT_RPT Report; Report.Initialize();
	m_pRptData->GetReportData(Report);
	Report.arChapK.Add(ChapK);
	m_pRptData->ModifyReportData(Report);

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Set Data by Current Units.
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	//++++++++++++++++++++++++++++++++++++++++++++

	return TRUE;
}

// 3.1. 콘크리트 재질 Data를 구성.
// Add by GAY.('06.05.30). China request Elastic Modulus and Density Output.
BOOL CRptJTG_D62_04::MakeConcData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	CString strText;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;
	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.CssK = m_pRptData->m_DefaultCss.THCssK;

	thData.arTDK.RemoveAll();
	strText = _LS(IDS_DGN_PSC_RPTJTG_Strength_Grade);   //_T("강도등급")
	thData.arTDK.Add(AddTDString(strText, 1, 1, 2));
	strText = _LS(IDS_DGN_PSC_RPTJTG_Elastic_Modulus);
	thData.arTDK.Add(AddTDwithUnit(strText, m_strStressUnit, 1, 1, 2));  // _T("탄성계수")  
	arCharK.RemoveAll();
	AddChar(arCharK, m_strDensityUnit, 1, 5, 1);
	strText = _LS(IDS_DGN_PSC_RPTJTG_Density);
	thData.arTDK.Add(AddTDwithUnit(strText, arCharK, 1, 1, 2));  // _T("단위중량")    
	strText = _LS(IDS_DGN_PSC_RPTJTG_Thermal_Coeff);    // _T("선팽창계수")
	thData.arTDK.Add(AddTDString(strText, 1, 1, 2));
	strText = _LS(IDS_DGN_PSC_RPTJTG_Standard_Value);   //_T("표준치")
	thData.arTDK.Add(AddTDString(strText, 1, 2, 1));
	strText = _LS(IDS_DGN_PSC_RPTJTG_Design_Value);     //_T("설계치")
	thData.arTDK.Add(AddTDString(strText, 1, 2, 1));

	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	thData.arTDK.RemoveAll();
	arCharK.RemoveAll();
	//strText.Format(,m_strStressUnit);
	AddChar(arCharK, _T("fck"), 2, 1, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
	arCharK.RemoveAll();
	AddChar(arCharK, _T("ftk"), 2, 1, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
	arCharK.RemoveAll();
	AddChar(arCharK, _T("fcd"), 2, 1, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
	arCharK.RemoveAll();
	AddChar(arCharK, _T("ftd"), 2, 1, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));

	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	for ( int i=0; i<m_arMatElem.GetSize(); i++ )
	{
		ElemPairK EPairK(m_arMatElem[i], EN_EL_BEAM);
		_PSC_MATL_ELEM MatlElem; MatlElem.Initialize();
		m_pJTG_D62_04->Get_PscMatlElem(EPairK, MatlElem);
		CString strConc, strfck, strftk, strfcd, strftd, strThermal, strEc, strDensity;

		strConc = MatlElem.MatdD.Data1.CodeMatlName;
		strfck.Format(_T("%.2f"), MatlElem.dfck);
		strftk.Format(_T("%.2f"), MatlElem.dftk);
		strfcd.Format(_T("%.2f"), MatlElem.dfcd);
		strftd.Format(_T("%.2f"), MatlElem.dftd);
		strThermal.Format(_T("%.3e"), MatlElem.MatdD.Data1.Analysis.Thermal);
		strEc.Format(_T("%.2f"), MatlElem.MatdD.Data1.Analysis.Elast);
		strDensity.Format(_T("%.2f"), MatlElem.MatdD.Data1.Analysis.Density * m_dDensitykNM);

		tdData.Initialize();
		tdData.arTDK.Add(AddTDString(strConc, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strEc, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strDensity, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strThermal, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strfck, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strftk, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strfcd, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strftd, 0, 1, 1, m_TDCenterCssK));
		m_pRptData->AddTRData(tdK, tdData);
		tableD.arTRK.Add(tdK);

		iExistCount += 1;
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 3.2. 프리스트레스 철근.
BOOL CRptJTG_D62_04::MakePrestressRebar(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	CString strText;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;
	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	thData.arTDK.RemoveAll();

	strText = _LS(IDS_DGN_PSC_RPTJTG_Tendon);
	thData.arTDK.Add(AddTDString(strText, 1));
	strText = _LS(IDS_DGN_PSC_RPTJTG_Elastic_Modulus);
	thData.arTDK.Add(AddTDwithUnit(strText, m_strStressUnit, 1)); // _T("탄성계수")
	strText = _LS(IDS_DGN_PSC_RPTJTG_Density);          // _T("단위중량")
	arCharK.RemoveAll();
	AddChar(arCharK, m_strDensityUnit, 1, 5, 1);
	thData.arTDK.Add(AddTDwithUnit(strText, arCharK, 1));
	strText = _LS(IDS_DGN_PSC_RPTJTG_Thermal_Coeff);    // _T("선팽창계수")
	thData.arTDK.Add(AddTDString(strText, 1));
	arCharK.RemoveAll();
	AddChar(arCharK, _T("fpk"), 2, 1, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
	arCharK.RemoveAll();
	AddChar(arCharK, _T("fpd"), 2, 1, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
	arCharK.RemoveAll();
	AddChar(arCharK, _T("f'pd"), 2, 2, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	int iTdntNumber = m_pJTG_D62_04->m_arTndnTdnt.GetSize();
	for ( int i=0; i<iTdntNumber; i++ )
	{
		_PSC_TNDN_TDNT TndnTdnt;
		TndnTdnt = m_pJTG_D62_04->m_arTndnTdnt.GetAt(i);

		CString strName, strfpk, strfpdt, strfpdc, strEc, strDensity, strThermal;
		strName = TndnTdnt.TendonTypeName;
		strfpk.Format(_T("%.2lf"), TndnTdnt.TndnTdna.dfpk);
		strfpdt.Format(_T("%.2lf"), TndnTdnt.TndnTdna.dfpdt);
		strfpdc.Format(_T("%.2lf"), TndnTdnt.TndnTdna.dfpdc);

		double dEc=0.0, dDensity=0.0, dThermal=0.0;
		GetTendonMatlData(strName, dEc, dDensity, dThermal);
		strEc.Format(_T("%.2lf"), dEc);
		strDensity.Format(_T("%.2lf"), dDensity);
		strThermal.Format(_T("%.3e"), dThermal);

		tdData.Initialize();
		tdData.arTDK.Add(AddTDString(strName, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strEc, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strDensity, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strThermal, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strfpk, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strfpdt, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strfpdc, 0, 1, 1, m_TDCenterCssK));
		m_pRptData->AddTRData(tdK, tdData);
		tableD.arTRK.Add(tdK);

		iExistCount += 1;
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 3.3. 종방향인장 보통철근 Data 구성.
BOOL CRptJTG_D62_04::MakeTensionRebar(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	CString strText;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;
	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.Initialize();
	thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Rbar), 1));

	arCharK.RemoveAll();
	AddChar(arCharK, _T("fsk"), 2, 1, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
	arCharK.RemoveAll();
	AddChar(arCharK, _T("fsd"), 2, 1, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
	arCharK.RemoveAll();
	AddChar(arCharK, _T("f'sd"), 2, 2, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	_PSC_MATL_ELEM MatlElem; MatlElem.Initialize();
	CString strName, strfskM, strfsdtM, strfsdcM;

	int iCount = m_arMatElem.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		ElemPairK EPairK(m_arMatElem[i], EN_EL_BEAM);
		if ( m_pJTG_D62_04->Get_PscMatlElem(EPairK, MatlElem) )
		{
			strName = MatlElem.MatdD.MainRebar_RebarName;
			strfskM.Format(_T("%.2lf"), MatlElem.dfskM);
			strfsdtM.Format(_T("%.2lf"), MatlElem.dfsdtM);
			strfsdcM.Format(_T("%.2lf"), MatlElem.dfsdcM);

			tdData.Initialize();
			tdData.arTDK.Add(AddTDString(strName, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strfskM, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strfsdtM, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strfsdcM, 0, 1, 1, m_TDCenterCssK));
			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			iExistCount += 1;
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// 3.4. 전단철근 및 구조철근 Data 구성.
BOOL CRptJTG_D62_04::MakeShearRebar(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	CString strText;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;
	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.Initialize();
	thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Stirrup), 1));

	arCharK.RemoveAll();
	AddChar(arCharK, _T("fsk"), 2, 1, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
	arCharK.RemoveAll();
	AddChar(arCharK, _T("fsd"), 2, 1, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
	arCharK.RemoveAll();
	AddChar(arCharK, _T("f'sd"), 2, 2, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	_PSC_MATL_ELEM MatlElem; MatlElem.Initialize();
	CString strfskS, strfsdtS, strfsdcS;

	int iCount = m_arMatElem.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		ElemPairK EPairK(m_arMatElem[i], EN_EL_BEAM);
		if ( m_pJTG_D62_04->Get_PscMatlElem(EPairK, MatlElem) )
		{
			strfskS.Format(_T("%.2lf"), MatlElem.dfskS);
			strfsdtS.Format(_T("%.2lf"), MatlElem.dfsdtS);
			strfsdcS.Format(_T("%.2lf"), MatlElem.dfsdcS);

			tdData.Initialize();
			tdData.arTDK.Add(AddTDString(MatlElem.MatdD.SubRebar_RebarName, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strfskS, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strfsdtS, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strfsdcS, 0, 1, 1, m_TDCenterCssK));
			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			iExistCount += 1;
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 3.3. 보통철근 Data의 구성.
BOOL CRptJTG_D62_04::MakeRebar(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	CString strText;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;
	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.Initialize();
	thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Normal_Rebar), 1));

	// Modify by GAY. MNET:2628. ('07.01.22). Delete _T("Thermal Coefficient") Item.
	strText = _LS(IDS_DGN_PSC_RPTJTG_Elastic_Modulus);
	thData.arTDK.Add(AddTDwithUnit(strText, m_strStressUnit, 1));
	strText = _LS(IDS_DGN_PSC_RPTJTG_Density);
	arCharK.RemoveAll();
	AddChar(arCharK, m_strDensityUnit, 1, 5, 1);
	thData.arTDK.Add(AddTDwithUnit(strText, arCharK, 1));
	/*
	strText = _LS(IDS_DGN_PSC_RPTJTG_Thermal_Coeff);
	thData.arTDK.Add(AddTDString(strText, 1));
	*/
	arCharK.RemoveAll();
	AddChar(arCharK, _T("fsk"), 2, 1, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
	arCharK.RemoveAll();
	AddChar(arCharK, _T("fsd"), 2, 1, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
	arCharK.RemoveAll();
	AddChar(arCharK, _T("f'sd"), 2, 2, 2);
	thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	_PSC_MATL_ELEM MatlElem; MatlElem.Initialize();
	CString strfsk, strfsdt, strfsdc, strEs, strDensity;//, strThermal;      

	int iCount = m_arMatElem.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		ElemPairK EPairK(m_arMatElem[i], EN_EL_BEAM);
		if ( m_pJTG_D62_04->Get_PscMatlElem(EPairK, MatlElem) )
		{
			// Main Rebar.
			// Modify by GAY. MNET:2638. ('07.01.22). Set Rebar Elastic Modulus and Density from MainRebarData.      
			strEs.Format(_T("%.2lf"), MatlElem.MatdD.MainRebarData.B_Elast);
			strDensity.Format(_T("%.2lf"), MatlElem.MatdD.MainRebarData.B_Density * m_dDensitykNM);
			//strThermal.Format(_T("%.3e"), MatlElem.MatdD.Data1.Analysis.Thermal);

			CString strName = MatlElem.MatdD.MainRebar_RebarName;
			strfsk.Format(_T("%.2lf"), MatlElem.dfskM);
			strfsdt.Format(_T("%.2lf"), MatlElem.dfsdtM);
			strfsdc.Format(_T("%.2lf"), MatlElem.dfsdcM);

			tdData.Initialize();
			tdData.arTDK.Add(AddTDString(strName, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strEs, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strDensity, 0, 1, 1, m_TDCenterCssK));
			//tdData.arTDK.Add(AddTDString(strThermal, 0, 1, 1, m_TDCenterCssK));        
			tdData.arTDK.Add(AddTDString(strfsk, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strfsdt, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strfsdc, 0, 1, 1, m_TDCenterCssK));
			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			// Sub Rebar. 
			// Modify by GAY. MNET:2638. ('07.01.22). Set Rebar Elastic Modulus and Density from SubRebarData.
			strEs.Format(_T("%.2lf"), MatlElem.MatdD.SubRebarData.B_Elast);
			strDensity.Format(_T("%.2lf"), MatlElem.MatdD.SubRebarData.B_Density * m_dDensitykNM);
			//strThermal.Format(_T("%.3e"), MatlElem.MatdD.Data1.Analysis.Thermal);

			strName = MatlElem.MatdD.SubRebar_RebarName;
			strfsk.Format(_T("%.2lf"), MatlElem.dfskS);
			strfsdt.Format(_T("%.2lf"), MatlElem.dfsdtS);
			strfsdc.Format(_T("%.2lf"), MatlElem.dfsdcS);

			tdData.Initialize();
			tdData.arTDK.Add(AddTDString(strName, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strEs, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strDensity, 0, 1, 1, m_TDCenterCssK));
			//tdData.arTDK.Add(AddTDString(strThermal, 0, 1, 1, m_TDCenterCssK));    
			tdData.arTDK.Add(AddTDString(strfsk, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strfsdt, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strfsdc, 0, 1, 1, m_TDCenterCssK));
			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			iExistCount += 1;
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// 4.5. Stage Data 구성.
BOOL CRptJTG_D62_04::MakeStageGroupSummaryData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	CString strText;
	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.Initialize();
	thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Stage_Name), 1, 1, 2));
	CString aHeadNames1[] = { _LS(IDS_DGN_PSC_RPTJTG_Element_Group),
		_LS(IDS_DGN_PSC_RPTJTG_Boundary_Group), _LS(IDS_DGN_PSC_RPTJTG_Load_Group) };
	for ( int i=0; i<3; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 2, 1));
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);


	thData.Initialize();
	for ( int i=0; i<3; i++ )
	{
		thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Active), 1, 1, 1));
		thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Deactive), 1, 1, 1));
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	for ( int i=0; i<m_iStageNum; i++ )
	{
		tdData.Initialize();

		CString strActive;
		CString strDeactive;

		T_STAG_D stagD;
		if ( !m_pDoc->m_pAttrCtrl->GetStag(m_arStagK[i], stagD) ) stagD.Initialize();

		tdData.arTDK.Add(AddTDString(stagD.StageName, 0, 1, 1, m_TDCenterCssK));

		CArray<T_TEXT_K, T_TEXT_K> arActiveK;
		CArray<T_TEXT_K, T_TEXT_K> arDeactiveK;
		GetElementGroupList(m_arStagK[i], arActiveK, arDeactiveK);
		tdData.arTDK.Add(AddTD(arActiveK, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTD(arDeactiveK, 0, 1, 1, m_TDCenterCssK));

		GetBoundaryGroupList(m_arStagK[i], arActiveK, arDeactiveK);
		tdData.arTDK.Add(AddTD(arActiveK, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTD(arDeactiveK, 0, 1, 1, m_TDCenterCssK));

		GetLoadGroupList(m_arStagK[i], arActiveK, arDeactiveK);
		tdData.arTDK.Add(AddTD(arActiveK, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTD(arDeactiveK, 0, 1, 1, m_TDCenterCssK));

		tdData.CssK = m_TDCenterCssK;
		m_pRptData->AddTRData(tdK, tdData);
		tableD.arTRK.Add(tdK);

		iExistCount += 1;
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 4.5.1. Element Group Activation Summary Data 구성.
BOOL CRptJTG_D62_04::MakeElemGroupData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	CString strText;
	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	thData.Initialize();
	thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Group_Name), 1, 1, 2));

	for ( int i=0; i<m_iStageNum; i++ )
	{
		T_STAG_K stagK = m_arStagK[i];
		T_STAG_D stagD; stagD.Initialize();
		if ( !m_pDoc->m_pAttrCtrl->GetStag(stagK, stagD) ) continue;
		thData.arTDK.Add(AddTDString(stagD.StageName, 1, 2, 1));
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	thData.Initialize();
	for ( int i=0; i<m_iStageNum; i++ )
	{
		thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Used_Active), 1));  // _T("A")   
		thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Used_Total), 1));   // _T("T")
	}
	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	int iCount = m_arGroupKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		T_GRUP_K grupK = m_arGroupKeyList[i];
		CString strGrupName = m_pDoc->m_pAttrCtrl->GetGrupName(grupK);

		CArray<int, int> arCondition; arCondition.RemoveAll();
		CArray<int, int> arSummary; arSummary.RemoveAll();

		tdData.Initialize();
		tdData.arTDK.Add(AddTDString(strGrupName));
		for ( int j=0; j<m_iStageNum; j++ )
		{
			T_STAG_K stagK = m_arStagK.GetAt(j);
			T_STAG_D stagD; stagD.Initialize();
			if ( !m_pDoc->m_pAttrCtrl->GetStag(stagK, stagD) ) continue;

			int iCondition=0;
			int nActive = stagD.aActiveElem.GetSize();
			for ( int k=0; k<nActive; k++ )
			{
				if ( stagD.aActiveElem[k].GrupKey == grupK ) iCondition = 1;
			}

			int nDeactive = stagD.aDeactiveElem.GetSize();
			for ( int k=0; k<nDeactive; k++ )
			{
				if ( stagD.aDeactiveElem[k].GrupKey == grupK ) iCondition = -1;
			}

			arCondition.Add(iCondition);
			if ( iCondition == 1 ) arSummary.Add(1);
			else if ( iCondition == -1 ) arSummary.Add(-1);
			else
			{
				if ( j>0 )
				{
					if ( arSummary[j-1] == 1 ) arSummary.Add(1);
					else if ( arSummary[j-1] == -1 ) arSummary.Add(-1);
					else arSummary.Add(0);
				}
				else arSummary.Add(0);
			}
		}

		for ( int j=0; j<m_iStageNum; j++ )
		{
			CString strCondition = GetStageActivationType(arCondition[j]);
			CString strSummary   = GetStageTotalType(arSummary[j]);

			tdData.arTDK.Add(AddTDString(strCondition, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strSummary, 0, 1, 1, m_TDCenterCssK));
		}

		m_pRptData->AddTRData(tdK, tdData);
		tableD.arTRK.Add(tdK);

		iExistCount += 1;
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 4.5.2. Boundary Group Activation Summary Data 구성.
BOOL CRptJTG_D62_04::MakeBoundaryGroupData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	CString strText;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;
	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	thData.Initialize();
	thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Group_Name), 1, 1, 2));

	CArray<T_BNGR_K, T_BNGR_K> arBngrKeyList; arBngrKeyList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetBngrKeyList(arBngrKeyList);
	int iBngrCount = arBngrKeyList.GetSize();

	if ( iBngrCount>0 )
	{
		for ( int i=0; i<m_iStageNum; i++ )
		{
			T_STAG_K stagK = m_arStagK.GetAt(i);
			T_STAG_D stagD; stagD.Initialize();
			if ( !m_pDoc->m_pAttrCtrl->GetStag(stagK, stagD) ) continue;
			thData.arTDK.Add(AddTDString(stagD.StageName, 1, 2, 1));
		}

		thData.CssK = m_pRptData->m_DefaultCss.THCssK;
		m_pRptData->AddTRData(thK, thData);
		tableD.arTHK.Add(thK);

		thData.Initialize();
		for ( int i=0; i<m_iStageNum; i++ )
		{
			thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Used_Active), 1));  // _T("A")   
			thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Used_Total), 1));   // _T("T")
		}
		thData.CssK = m_pRptData->m_DefaultCss.THCssK;
		m_pRptData->AddTRData(thK, thData);
		tableD.arTHK.Add(thK);

		for ( int i=0; i<iBngrCount; i++ )
		{
			T_BNGR_K bngrK = arBngrKeyList[i];
			CString strGrupName = m_pDoc->m_pAttrCtrl->GetBngrName(bngrK);

			CArray<int, int> arCondition; arCondition.RemoveAll();
			CArray<int, int> arSummary; arSummary.RemoveAll();

			tdData.Initialize();
			tdData.arTDK.Add(AddTDString(strGrupName));
			for ( int j=0; j<m_iStageNum; j++ )
			{
				T_STAG_K stagK = m_arStagK.GetAt(j);
				T_STAG_D stagD; stagD.Initialize();
				if ( !m_pDoc->m_pAttrCtrl->GetStag(stagK, stagD) ) continue;

				int iCondition=0;
				int nActive = stagD.aActiveBndr.GetSize();
				for ( int k=0; k<nActive; k++ )
				{
					if ( stagD.aActiveBndr[k].BngrKey == bngrK ) iCondition = 1;
				}

				int nDeactive = stagD.aDeactiveBndr.GetSize();
				for ( int k=0; k<nDeactive; k++ )
				{
					if ( stagD.aDeactiveBndr[k]== bngrK ) iCondition = -1;
				}
				arCondition.Add(iCondition);
				if ( iCondition == 1 ) arSummary.Add(1);
				else if ( iCondition == -1 ) arSummary.Add(-1);
				else
				{
					if ( j>0 )
					{
						if ( arSummary[j-1] == 1 ) arSummary.Add(1);
						else if ( arSummary[j-1] == -1 ) arSummary.Add(-1);
						else arSummary.Add(0);
					}
					else arSummary.Add(0);
				}
			}

			for ( int j=0; j<m_iStageNum; j++ )
			{
				CString strCondition = GetStageActivationType(arCondition[j]);
				CString strSummary   = GetStageTotalType(arSummary[j]);

				tdData.arTDK.Add(AddTDString(strCondition, 0, 1, 1, m_TDCenterCssK));
				tdData.arTDK.Add(AddTDString(strSummary, 0, 1, 1, m_TDCenterCssK));

				iExistCount += 1;
			}
			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 4.5.3. Load Group Activation Summary Data 구성.
BOOL CRptJTG_D62_04::MakeLoadGroupData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	CString strText;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;
	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	thData.Initialize();
	thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Group_Name), 1, 1, 2));

	CArray<T_LDGR_K, T_LDGR_K> arLdgrKeyList; arLdgrKeyList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetLdgrKeyList(arLdgrKeyList);
	int iLoadCount = arLdgrKeyList.GetSize();

	if ( iLoadCount>0 )
	{
		for ( int i=0; i<m_iStageNum; i++ )
		{
			T_STAG_K stagK = m_arStagK.GetAt(i);
			T_STAG_D stagD; stagD.Initialize();
			if ( !m_pDoc->m_pAttrCtrl->GetStag(stagK, stagD) ) continue;
			thData.arTDK.Add(AddTDString(stagD.StageName, 1, 2, 1));
		}

		thData.CssK = m_pRptData->m_DefaultCss.THCssK;
		m_pRptData->AddTRData(thK, thData);
		tableD.arTHK.Add(thK);

		thData.Initialize();
		for ( int i=0; i<m_iStageNum; i++ )
		{
			thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Used_Active), 1));  // _T("A")   
			thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Used_Total), 1));   // _T("T")
		}

		thData.CssK = m_pRptData->m_DefaultCss.THCssK;
		m_pRptData->AddTRData(thK, thData);
		tableD.arTHK.Add(thK);

		for ( int i=0; i<iLoadCount; i++ )
		{
			T_LDGR_K ldgrK = arLdgrKeyList.GetAt(i);
			CString strGrupName = m_pDoc->m_pAttrCtrl->GetLdgrName(ldgrK);

			CArray<int, int> arCondition; arCondition.RemoveAll();
			CArray<int, int> arSummary; arSummary.RemoveAll();

			tdData.Initialize();
			tdData.arTDK.Add(AddTDString(strGrupName));
			for ( int j=0; j<m_iStageNum; j++ )
			{
				T_STAG_K stagK = m_arStagK.GetAt(j);
				T_STAG_D stagD; stagD.Initialize();
				if ( !m_pDoc->m_pAttrCtrl->GetStag(stagK, stagD) ) continue;

				int iCondition=0;
				int nActive = stagD.aActiveLoad.GetSize();
				for ( int k=0; k<nActive; k++ )
				{
					if ( stagD.aActiveLoad[k].LdgrKey == ldgrK ) iCondition = 1;
				}

				int nDeactive = stagD.aDeactiveLoad.GetSize();
				for ( int k=0; k<nDeactive; k++ )
				{
					if ( stagD.aDeactiveLoad[k].LdgrKey == ldgrK ) iCondition = -1;
				}
				arCondition.Add(iCondition);
				if ( iCondition == 1 ) arSummary.Add(1);
				else if ( iCondition == -1 ) arSummary.Add(-1);
				else
				{
					if ( j>0 )
					{
						if ( arSummary[j-1] == 1 ) arSummary.Add(1);
						else if ( arSummary[j-1] == -1 ) arSummary.Add(-1);
						else arSummary.Add(0);
					}
					else arSummary.Add(0);
				}
			}

			for ( int j=0; j<m_iStageNum; j++ )
			{
				CString strCondition = GetStageActivationType(arCondition[j]);
				CString strSummary   = GetStageTotalType(arSummary[j]);

				tdData.arTDK.Add(AddTDString(strCondition, 0, 1, 1, m_TDCenterCssK));
				tdData.arTDK.Add(AddTDString(strSummary, 0, 1, 1, m_TDCenterCssK));

				iExistCount += 1;
			}

			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 4.6. 사용단계에 대한 Data 구성.
BOOL CRptJTG_D62_04::MakeServiceStageLoadData(int iChapID, T_CHAP_RPT& chapD)
{
	T_CHAP_K chap2K, chap3K;
	T_CHAP_RPT chap2D = MakeChapter(2, iChapID, _LS(IDS_DGN_PSC_RPTJTG_Service_Stage));
	T_CHAP_RPT chap3D;

	// 활하중 Type.
	int iChapCount = 01;
	CString strText = _T("");

	T_PARA_K paraK;
	T_PARA_RPT paraD;
	paraD.Initialize();
	paraD.SeqID = 1;
	paraD.strTitle = _T("Paragraph");

	if ( MakeLiveLoadData(paraD) )
	{
		chap3D = MakeChapter(3, iChapCount++, _LS(IDS_DGN_PSC_RPTJTG_LiveLoad_Type));

		m_pRptData->AddParaData(paraK, paraD);
		chap3D.arParaK.Add(paraK);
		m_pRptData->AddChapData(chap3K, chap3D);
		chap2D.arSubChapK.Add(chap3K);

		// 충격계수.    
		strText.Format(_T("%s : %s"), _LS(IDS_DGN_PSC_RPTJTG_Impact_Coeff), GetImpactCoefficient());
		chap3D = MakeChapter(3, iChapCount++, strText);
		m_pRptData->AddChapData(chap3K, chap3D);
		chap2D.arSubChapK.Add(chap3K);
	}

	// 온도하중
	int iSubChapCount = 1;
	if ( CheckExistTemperatureLoad() )
	{
		chap3D = MakeChapter(3, iChapCount++, _LS(IDS_DGN_PSC_RPTJTG_Temperature_Load));

		MakeSystemTempData(iSubChapCount, paraD);
		MakeNodalTempData(iSubChapCount, paraD);
		MakeElemTempData(iSubChapCount, paraD);
		MakeTempGradientData(iSubChapCount, paraD);
		MakeBeamSectTempData(iSubChapCount, paraD);

		m_pRptData->AddParaData(paraK, paraD);
		chap3D.arParaK.Add(paraK);

		m_pRptData->AddChapData(chap3K, chap3D);
		chap2D.arSubChapK.Add(chap3K);
	}

	// 지점침하.
	if ( MakeSpecifiedDispData(paraD) )
	{
		chap3D = MakeChapter(3, iChapCount++, _LS(IDS_DGN_PSC_RPTJTG_Settlement_Support));

		m_pRptData->AddParaData(paraK, paraD);
		chap3D.arParaK.Add(paraK);
		m_pRptData->AddChapData(chap3K, chap3D);
		chap2D.arSubChapK.Add(chap3K);
	}

	m_pRptData->AddChapData(chap2K, chap2D);
	chapD.arSubChapK.Add(chap2K);

	return TRUE;
}

// 4.6.1. 활하중 Type Table 구성.
BOOL CRptJTG_D62_04::MakeLiveLoadData(T_PARA_RPT& paraD)
{
	int iExistCount = 0;

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.Initialize();
	thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Vehicular_Load_Name), 1, 1, 1));
	thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Vehicular_Load_Type), 1, 1, 1));
	thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Description), 1, 1, 1));

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	// Table Data.
	tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

	T_MVHL_D mvhlD;
	CArray<T_MVHL_K, T_MVHL_K> arMvhlKeyList;
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(arMvhlKeyList);

	int iCount = arMvhlKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		if ( m_pDoc->m_pAttrCtrl->GetMvhl(arMvhlKeyList[i], mvhlD) )
		{
			tdData.Initialize();

			CString strDescript = GetLiveLoadTypeDescription(mvhlD.VehicleTypeName);
			tdData.arTDK.Add(AddTDString(mvhlD.VehicleLoadName, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(mvhlD.VehicleTypeName, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(strDescript, 0, 1, 1, m_TDCenterCssK));

			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			iExistCount += 1;
		}
	}

	if ( iExistCount > 0 )
	{
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

// 4.6.3.1) System Temperature Load Data 구성.
BOOL CRptJTG_D62_04::MakeSystemTempData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;

	T_TEXT_K textK;
	T_TEXT_RPT textD;
	textD.CssK = m_pRptData->m_DefaultCss.TextCssK;
	textD.nCheck = 1;
	textD.strTitle = _T("Text");

	paraD.Initialize();

	T_STMP_K stmpK;
	T_STMP_D stmpD;
	CString strText = _T("");

	CArray<T_STLD_K, T_STLD_K> arStldKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKeyList);

	int iCount = arStldKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		if ( m_pDoc->m_pAttrCtrl->GetStmp(arStldKeyList[i], stmpK, stmpD) )
		{
			if ( strText.GetLength() > 0 )
			{
				strText += _T(", ");
			}
			strText.Format(_T("%s%.1f %s"), strText, stmpD.dblSysTemp, _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
			iExistCount += 1;
		}
	}

	if ( iExistCount > 0 )
	{
		textD.strText.Format(_T("%d) %s : %s"), iChapID++, _LS(IDS_DGN_PSC_RPTJTG_System_Temperature), strText);
		m_pRptData->AddTextData(textK, textD);
		paraD.arContentK.Add(textK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 4.6.3.2) Nodal Temperature Load Data 구성.
BOOL CRptJTG_D62_04::MakeNodalTempData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;

	CString strText = _T("");

	T_TEXT_K textK;
	T_TEXT_RPT textD;
	textD.CssK = m_pRptData->m_DefaultCss.TextCssK;
	textD.nCheck = 1;
	textD.strTitle = _T("Text");

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.Initialize();

	thData.arTDK.Add(AddTDString(_LS(IDS_TB_NTMP_Node), 1, 1, 1));
	thData.arTDK.Add(AddTDString(_LS(IDS_TB_NTMP_LoadCase), 1, 1, 1));
	strText.Format(_T("%s (%s)"), _LS(IDS_TB_NTMP_Temperature), _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
	thData.arTDK.Add(AddTDString(strText, 1, 1, 1));

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	// Table Data.
	tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

	CArray<T_NODE_K, T_NODE_K> arNodeKeyList;
	m_pDoc->m_pAttrCtrl->GetNodeKeyList(arNodeKeyList);

	int iCount = arNodeKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		CArray<T_NTMP_K, T_NTMP_K> arNtmpKeyList;
		CArray<T_NTMP_D, T_NTMP_D&> arNtmpDataList;

		m_pDoc->m_pAttrCtrl->GetNtmp(arNodeKeyList[i], arNtmpKeyList, arNtmpDataList);

		int iNtmpCount = arNtmpKeyList.GetSize();
		for ( int j=0; j<iNtmpCount; j++ )
		{
			tdData.arTDK.RemoveAll();

			strText.Format(_T("%d"), arNodeKeyList[i]);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText = GetStaticLoadCaseName(arNtmpDataList[j].LoadCaseKey);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText.Format(_T("%.1f"), arNtmpDataList[j].dblNodalTemp);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			iExistCount += 1;
		}
	}

	if ( iExistCount > 0 )
	{
		textD.strText.Format(_T("%d) %s"), iChapID++, _LS(IDS_DGN_PSC_RPTJTG_Nodal_Temperature));
		m_pRptData->AddTextData(textK, textD);
		paraD.arContentK.Add(textK);

		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 4.6.3.3) Element Temperature Load Data 구성.
BOOL CRptJTG_D62_04::MakeElemTempData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;

	CString strText = _T("");

	T_TEXT_K textK;
	T_TEXT_RPT textD;
	textD.CssK = m_pRptData->m_DefaultCss.TextCssK;
	textD.nCheck = 1;
	textD.strTitle = _T("Text");

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header. 
	thData.Initialize();

	thData.arTDK.Add(AddTDString(_LS(IDS_TB_ELEM), 1, 1, 1));
	thData.arTDK.Add(AddTDString(_LS(IDS_TB_LOADCASE), 1, 1, 1));
	strText.Format(_T("%s (%s)"), _LS(IDS_TB_TEMPERATURE), _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
	thData.arTDK.Add(AddTDString(strText, 1, 1, 1));

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	// Table Data.
	tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);

	int iCount = arElemKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		CArray<T_ETMP_K, T_ETMP_K> arEtmpKeyList;
		CArray<T_ETMP_D, T_ETMP_D&> arEtmpDataList;

		m_pDoc->m_pAttrCtrl->GetEtmp(arElemKeyList[i], arEtmpKeyList, arEtmpDataList);

		int iEtmpCount = arEtmpKeyList.GetSize();
		for ( int j=0; j<iEtmpCount; j++ )
		{
			tdData.arTDK.RemoveAll();

			strText.Format(_T("%d"), arElemKeyList[i]);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText = GetStaticLoadCaseName(arEtmpDataList[j].LoadCaseKey);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText.Format(_T("%.1f"), arEtmpDataList[j].dblElemTemp);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			iExistCount += 1;
		}
	}

	if ( iExistCount > 0 )
	{
		textD.strText.Format(_T("%d) %s"), iChapID++, _LS(IDS_DGN_PSC_RPTJTG_Elem_Temperature));
		m_pRptData->AddTextData(textK, textD);
		paraD.arContentK.Add(textK);

		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 4.6.3.4) Temperature Gradient Data 구성.
BOOL CRptJTG_D62_04::MakeTempGradientData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;

	CString strText = _T("");

	T_TEXT_K textK;
	T_TEXT_RPT textD;
	textD.CssK = m_pRptData->m_DefaultCss.TextCssK;
	textD.nCheck = 1;
	textD.strTitle = _T("Text");

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	thData.arTDK.RemoveAll();

	thData.arTDK.Add(AddTDString(_LS(IDS_TB_GTMP_Elem), 1, 1, 1));
	thData.arTDK.Add(AddTDString(_LS(IDS_TB_GTMP_LoadCase), 1, 1, 1));
	strText.Format(_T("%s (%s)"), _LS(IDS_TB_GTMP_Tz), _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
	thData.arTDK.Add(AddTDString(strText, 1, 1, 1));
	strText.Format(_T("%s (%s)"), _LS(IDS_TB_GTMP_Ty), _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
	thData.arTDK.Add(AddTDString(strText, 1, 1, 1));
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	// Table Data.
	tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);

	int iCount = arElemKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		CArray<T_GTMP_K, T_GTMP_K> arGtmpKeyList;
		CArray<T_GTMP_D, T_GTMP_D&> arGtmpDataList;

		m_pDoc->m_pAttrCtrl->GetGtmp(arElemKeyList[i], arGtmpKeyList, arGtmpDataList);

		int iEtmpCount = arGtmpKeyList.GetSize();
		for ( int j=0; j<iEtmpCount; j++ )
		{
			tdData.arTDK.RemoveAll();

			strText.Format(_T("%d"), arElemKeyList[i]);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText = GetStaticLoadCaseName(arGtmpDataList[j].LoadCaseKey);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText.Format(_T("%.1f"), arGtmpDataList[j].dTz);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText.Format(_T("%.1f"), arGtmpDataList[j].dTy);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			iExistCount += 1;
		}
	}

	if ( iExistCount > 0 )
	{
		textD.strText.Format(_T("%d) %s"), iChapID++, _LS(IDS_DGN_PSC_RPTJTG_Temp_Gradient));
		m_pRptData->AddTextData(textK, textD);
		paraD.arContentK.Add(textK);

		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 4.6.3.4) Beam Section Temperature Data 구성.
BOOL CRptJTG_D62_04::MakeBeamSectTempData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;

	CString strText = _T("");

	T_TEXT_K textK;
	T_TEXT_RPT textD;
	textD.CssK = m_pRptData->m_DefaultCss.TextCssK;
	textD.nCheck = 1;
	textD.strTitle = _T("Text");

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	thData.arTDK.RemoveAll();
	thData.arTDK.Add(AddTDString(_LS(IDS_TB_ELEM), 1, 1, 2));
	thData.arTDK.Add(AddTDString(_LS(IDS_TB_LOADCASE), 1, 1, 2));
	thData.arTDK.Add(AddTDString(_LS(IDS_TB_BTMP_Direction), 1, 1, 2));

	CString aHeadNames[] = { _T("1st T"), _T("2rd T"), _T("3rd T"), _T("4th T"), _T("5th T") };
	for ( int i=0; i<5; i++ )
	{
		strText.Format(_T("%s1 (%s)"), aHeadNames[i], _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
		thData.arTDK.Add(AddTDString(strText, 1, 1, 1));
	}
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	thData.arTDK.RemoveAll();
	for ( int i=0; i<5; i++ )
	{
		strText.Format(_T("%s2 (%s)"), aHeadNames[i], _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
		thData.arTDK.Add(AddTDString(strText, 1, 1, 1));
	}
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	// Table Data.
	tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);

	int iCount = arElemKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		CArray<T_BTMP_K, T_BTMP_K> arBtmpKeyList;
		CArray<T_BTMP_D, T_BTMP_D&> arBtmpDataList;

		m_pDoc->m_pAttrCtrl->GetBtmp(arElemKeyList[i], arBtmpKeyList, arBtmpDataList);

		int iEtmpCount = arBtmpKeyList.GetSize();
		for ( int j=0; j<iEtmpCount; j++ )
		{
			tdData.arTDK.RemoveAll();

			strText.Format(_T("%d"), arElemKeyList[i]);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

			strText = GetStaticLoadCaseName(arBtmpDataList[j].LoadCaseKey);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(GetBeamSectTempLoadDirection(arBtmpDataList[j].nDirection), 0, 1, 2, m_TDCenterCssK));

			for ( int k=0; k<5; k++ )
			{
				strText.Format(_T("%.1f"), arBtmpDataList[j].SectTmp[k].dT1);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
			}
			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			tdData.arTDK.RemoveAll();
			for ( int k=0; k<5; k++ )
			{
				strText.Format(_T("%.1f"), arBtmpDataList[j].SectTmp[k].dT2);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
			}
			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			iExistCount += 1;
		}
	}

	if ( iExistCount > 0 )
	{
		textD.strText.Format(_T("%d) %s"), iChapID++, _LS(IDS_DGN_PSC_RPTJTG_BeamSect_Temperature));
		m_pRptData->AddTextData(textK, textD);
		paraD.arContentK.Add(textK);

		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 4.6.4. 지점침하 Data를 구성함.
BOOL CRptJTG_D62_04::MakeSpecifiedDispData(T_PARA_RPT& paraD)
{
	int iExistCount = 0;

	CString strText = _T("");
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	thData.arTDK.RemoveAll();

	CString aHeadNames[] = { _LS(IDS_TB_SDSP_Node), _LS(IDS_TB_SDSP_LoadCase),
		_LS(IDS_TB_SDSP_DX_n), _LS(IDS_TB_SDSP_DY_n), _LS(IDS_TB_SDSP_DZ_n),
		_LS(IDS_TB_SDSP_RX_n), _LS(IDS_TB_SDSP_RY_n), _LS(IDS_TB_SDSP_RZ_n) };
	for ( int i=0; i<8; i++ )
	{
		if ( i<2 )
		{
			thData.arTDK.Add(AddTDString(aHeadNames[i], 1, 1, 1));
		}
		else if ( i>4 )
		{
			thData.arTDK.Add(AddTDwithUnit(aHeadNames[i], m_strAngleUnitRad, 1, 1, 1));
		}
		else
		{
			thData.arTDK.Add(AddTDwithUnit(aHeadNames[i], m_strLengthUnitM, 1, 1, 1));
		}
	}

	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	// Table Data.
	tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

	CArray<T_NODE_K, T_NODE_K> arNodeKeyList;
	m_pDoc->m_pAttrCtrl->GetNodeKeyList(arNodeKeyList);

	int iCount = arNodeKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		CArray<T_SDSP_K, T_SDSP_K> arSdspKeyList;
		CArray<T_SDSP_D, T_SDSP_D&> arSdspDataList;

		m_pDoc->m_pAttrCtrl->GetSdsp(arNodeKeyList[i], arSdspKeyList, arSdspDataList);

		int iSdspCount = arSdspKeyList.GetSize();
		for ( int j=0; j<iSdspCount; j++ )
		{
			tdData.arTDK.RemoveAll();

			strText.Format(_T("%d"), arNodeKeyList[i]);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText = GetStaticLoadCaseName(arSdspDataList[j].LoadCaseKey);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			for ( int k=0; k<6; k++ )
			{
				double dFactor = (k<3)? m_dLengthM : 1.0;
				strText.Format(_T("%.3f"), arSdspDataList[j].Displacement[k] * dFactor);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
			}

			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			iExistCount += 1;
		}
	}

	if ( iExistCount > 0 )
	{
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 5.1.1. Static Load Cases 설명
BOOL CRptJTG_D62_04::MakeStaticLoadCaseData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	CString aHeadNames[] = { _LS(IDS_DGN_PSC_RPTJTG_Number), _LS(IDS_DGN_PSC_RPTJTG_Name),
		_LS(IDS_DGN_PSC_RPTJTG_Type), _LS(IDS_DGN_PSC_RPTJTG_Description) };
	for ( int i=0; i<4; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames[i], 1));
	}

	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CArray<T_STLD_K, T_STLD_K> arStldKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKeyList);

	int iCount = arStldKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		T_STLD_K stldK = arStldKeyList.GetAt(i);
		T_STLD_D stldD;
		stldD.Initialize();
		if ( !m_pDoc->m_pAttrCtrl->GetStld(stldK, stldD) ) continue;

		CString LoadCaseId = _T("");
		LoadCaseId.Format(_T("%d"), stldD.LoadCaseId);
		CString LoadCaseName = stldD.LoadCaseName;
		CString LoadCaseType = m_pDoc->m_pLoadDB->GetLoadTypeName(stldD.LoadCaseType);
		CString Description  = stldD.Description;

		tdData.Initialize();
		tdData.arTDK.Add(AddTDString(LoadCaseId, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(LoadCaseName, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(LoadCaseType, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(Description));
		m_pRptData->AddTRData(tdK, tdData);
		tableD.arTRK.Add(tdK);

		iExistCount += 1;
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 5.1.2. Response Spectrum Data를 구성함.
BOOL CRptJTG_D62_04::MakeResponseSpectrumData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = 0;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	CString aHeadNames[] = { _LS(IDS_DGN_PSC_RPTJTG_Number), _LS(IDS_DGN_PSC_RPTJTG_Name),
		_LS(IDS_DGN_PSC_RPTJTG_Spectrum_Function) };

	for ( int i=0; i<3; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames[i], 1));
	}

	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CArray<T_SPLC_K, T_SPLC_K> arSplcKeyList;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(arSplcKeyList);

	int iCount = arSplcKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		T_SPLC_K splcK = arSplcKeyList.GetAt(i);
		T_SPLC_D splcD; splcD.Initialize();
		if ( !m_pDoc->m_pAttrCtrl->GetSplc(splcK, splcD) ) continue;

		CString strSpecFuncName=_T("");
		int iSubCount = splcD.aSpfcKey.GetSize();
		for ( int j=0; j<iSubCount; j++ )
		{
			T_SPFC_K spfcK = splcD.aSpfcKey.GetAt(j);
			T_SPFC_D spfcD; spfcD.Initialize();
			if ( !m_pDoc->m_pAttrCtrl->GetSpfc(spfcK, spfcD) ) continue;

			if ( j==0 ) strSpecFuncName = spfcD.SpecFuncName;
			else     strSpecFuncName += _T(", ")+spfcD.SpecFuncName;
		}

		CString strLoadCaseId = _T("");
		strLoadCaseId.Format(_T("%d"), splcD.LoadCaseId);

		tdData.Initialize();
		tdData.arTDK.Add(AddTDString(strLoadCaseId, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(splcD.LoadCaseName, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strSpecFuncName, 0, 1, 1, m_TDCenterCssK));
		m_pRptData->AddTRData(tdK, tdData);
		tableD.arTRK.Add(tdK);

		iExistCount += 1;
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 5.1.3. Time History Load Cases Data를 구성함.
BOOL CRptJTG_D62_04::MakeTimeHistoryData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	CString aHeadNames[] = { _LS(IDS_DGN_PSC_RPTJTG_Number), _LS(IDS_DGN_PSC_RPTJTG_Name),
		_LS(IDS_DGN_PSC_RPTJTG_Description) };
	for ( int i=0; i<3; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames[i], 1));
	}

	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CArray<T_THIS_K, T_THIS_K> arThisKeyList;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(arThisKeyList);

	int iCount = arThisKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		T_THIS_K thisK = arThisKeyList.GetAt(i);
		T_THIS_D thisD; thisD.Initialize();
		if ( !m_pDoc->m_pAttrCtrl->GetThis(thisK, thisD) ) continue;

		CString strLoadCaseId = _T("");
		strLoadCaseId.Format(_T("%d"), thisD.LoadCaseId);

		tdData.Initialize();
		tdData.arTDK.Add(AddTDString(strLoadCaseId, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(thisD.LoadCaseName, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(thisD.Description, 0, 1, 1, m_TDCenterCssK));
		m_pRptData->AddTRData(tdK, tdData);
		tableD.arTRK.Add(tdK);

		iExistCount++;
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 5.1.4. Moving Load Case Data를 구성함.
BOOL CRptJTG_D62_04::MakeMovingLoadData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	CString aHeadNames[] = { _LS(IDS_DGN_PSC_RPTJTG_Number), _LS(IDS_DGN_PSC_RPTJTG_Name),
		_LS(IDS_DGN_PSC_RPTJTG_Description) };

	for ( int i=0; i<3; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames[i], 1));
	}

	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CString strLoadCaseId = _T("");
	CString strLoadCaseName = _T("");
	CString strDescription  = _T("");

	// Get Moving Load Code  
	T_MVCD_D MvcdD;
	m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD);
	int nCodeType = MvcdD.nCodeType;

	if ( nCodeType==D_MOVE_CODE_JAPAN )
	{
		CArray<T_MVLDjp_K, T_MVLDjp_K> arMvldjpKeyList;
		m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(arMvldjpKeyList);

		int iCount = arMvldjpKeyList.GetSize();
		for ( int i=0; i<arMvldjpKeyList.GetSize(); i++ )
		{
			T_MVLDjp_K mvldjpK = arMvldjpKeyList.GetAt(i);
			T_MVLDjp_D mvldjpD;
			if ( !m_pDoc->m_pAttrCtrl->GetMvldjp(mvldjpK, mvldjpD) ) continue;

			strLoadCaseId.Format(_T("%d"), mvldjpD.LoadCaseId);
			strLoadCaseName = mvldjpD.LoadCaseName;
			strDescription  = mvldjpD.Description;

			iExistCount += 1;
		}
	}
	else if ( nCodeType==D_MOVE_CODE_CHINA )
	{
		CArray<T_MVLDch_K, T_MVLDch_K> arMvldchKeyList;
		m_pDoc->m_pAttrCtrl->GetMvldchKeyList(arMvldchKeyList);

		int iCount = arMvldchKeyList.GetSize();
		for ( int i=0; i<iCount; i++ )
		{
			T_MVLDch_K mvldchK = arMvldchKeyList.GetAt(i);
			T_MVLDch_D mvldchD; mvldchD.Initialize();
			if ( !m_pDoc->m_pAttrCtrl->GetMvldch(mvldchK, mvldchD) ) continue;

			strLoadCaseId.Format(_T("%d"), mvldchD.LoadCaseId);
			strLoadCaseName = mvldchD.LoadCaseName;
			strDescription  = mvldchD.Description;

			iExistCount += 1;
		}
	}
	else if ( nCodeType==D_MOVE_CODE_INDIA ) // India
	{
		CArray<T_MVLDid_K, T_MVLDid_K> arMvldidKeyList;
		m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arMvldidKeyList);

		int iCount = arMvldidKeyList.GetSize();
		for ( int i=0; i<iCount; i++ )
		{
			T_MVLDid_K mvldidK = arMvldidKeyList.GetAt(i);
			T_MVLDid_D mvldidD; mvldidD.Initialize();
			if ( !m_pDoc->m_pAttrCtrl->GetMvldid(mvldidK, mvldidD) ) continue;

			strLoadCaseId.Format(_T("%d"), mvldidD.LoadCaseId);
			strLoadCaseName = mvldidD.LoadCaseName;
			strDescription  = mvldidD.Description;

			iExistCount += 1;
		}
	}
	else if ( nCodeType==D_MOVE_CODE_BS ) // Bs
	{
		CArray<T_MVLDbs_K, T_MVLDbs_K> arMvldbsKeyList;
		m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(arMvldbsKeyList);

		int iCount = arMvldbsKeyList.GetSize();
		for ( int i=0; i<iCount; i++ )
		{
			T_MVLDbs_K mvldbsK = arMvldbsKeyList.GetAt(i);
			T_MVLDbs_D mvldbsD; mvldbsD.Initialize();
			if ( !m_pDoc->m_pAttrCtrl->GetMvldbs(mvldbsK, mvldbsD) ) continue;

			strLoadCaseId.Format(_T("%d"), mvldbsD.LoadCaseId);
			strLoadCaseName = mvldbsD.LoadCaseName;
			strDescription  = mvldbsD.Description;

			iExistCount += 1;
		}
	}
	else if (nCodeType == D_MOVE_CODE_FRANCE) // France
	{
		CArray<T_MVLDfr_K, T_MVLDfr_K> arMvldfrKeyList;
		m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(arMvldfrKeyList);

		int iCount = arMvldfrKeyList.GetSize();
		for (int i = 0; i < iCount; i++)
		{
			T_MVLDfr_K mvldfrK = arMvldfrKeyList.GetAt(i);
			T_MVLDfr_D mvldfrD; mvldfrD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetMvldfr(mvldfrK, mvldfrD)) continue;

			strLoadCaseId.Format(_T("%d"), mvldfrD.LoadCaseId);
			strLoadCaseName = mvldfrD.LoadCaseName;
			strDescription = mvldfrD.Description;

			iExistCount += 1;
		}
	}
	else if ( nCodeType==D_MOVE_CODE_TRANS ) // transverse
	{
		CArray<T_MVLDtr_K, T_MVLDtr_K> arMvldtrKeyList;
		m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(arMvldtrKeyList);

		int iCount = arMvldtrKeyList.GetSize();
		for ( int i=0; i<iCount; i++ )
		{
			T_MVLDtr_K mvldtrK = arMvldtrKeyList.GetAt(i);
			T_MVLDtr_D mvldtrD; mvldtrD.Initialize();
			if ( !m_pDoc->m_pAttrCtrl->GetMvldtr(mvldtrK, mvldtrD) ) continue;

			strLoadCaseId.Format(_T("%d"), mvldtrD.LoadCaseId);
			strLoadCaseName = mvldtrD.LoadCaseName;
			strDescription  = mvldtrD.Description;

			iExistCount += 1;
		}
	}
	else
	{
		CArray<T_MVLD_K, T_MVLD_K> arMvldKeyList;
		m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMvldKeyList);

		int iCount = arMvldKeyList.GetSize();
		for ( int i=0; i<iCount; i++ )
		{
			T_MVLD_K mvldK = arMvldKeyList.GetAt(i);
			T_MVLD_D mvldD; mvldD.Initialize();
			if ( !m_pDoc->m_pAttrCtrl->GetMvld(mvldK, mvldD) ) continue;

			strLoadCaseId.Format(_T("%d"), mvldD.LoadCaseId);
			strLoadCaseName = mvldD.LoadCaseName;
			strDescription  = mvldD.Description;

			iExistCount += 1;
		}
	}

	tdData.Initialize();
	tdData.arTDK.Add(AddTDString(strLoadCaseId, 0, 1, 1, m_TDCenterCssK));
	tdData.arTDK.Add(AddTDString(strLoadCaseName, 0, 1, 1, m_TDCenterCssK));
	tdData.arTDK.Add(AddTDString(strDescription, 0, 1, 1, m_TDCenterCssK));
	m_pRptData->AddTRData(tdK, tdData);
	tableD.arTRK.Add(tdK);

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 5.1.5. Settlement Load Cases Data를 구성함.
BOOL CRptJTG_D62_04::MakeSettlementData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	CString aHeadNames[] = { _LS(IDS_DGN_PSC_RPTJTG_Number), _LS(IDS_DGN_PSC_RPTJTG_Name),
		_LS(IDS_DGN_PSC_RPTJTG_Description) };

	for ( int i=0; i<3; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames[i], 1));
	}

	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CArray<T_SMLC_K, T_SMLC_K> arSmlcKeyList;
	m_pDoc->m_pAttrCtrl->GetSmlcKeyList(arSmlcKeyList);

	int iCount = arSmlcKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		T_SMLC_K smlcK = arSmlcKeyList.GetAt(i);
		T_SMLC_D smlcD; smlcD.Initialize();
		if ( !m_pDoc->m_pAttrCtrl->GetSmlc(smlcK, smlcD) ) continue;

		CString strLoadCaseId = _T("");
		strLoadCaseId.Format(_T("%d"), smlcD.LoadCaseId);

		tdData.Initialize();
		tdData.arTDK.Add(AddTDString(strLoadCaseId, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(smlcD.LoadCaseName, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(smlcD.Description, 0, 1, 1, m_TDCenterCssK));
		m_pRptData->AddTRData(tdK, tdData);
		tableD.arTRK.Add(tdK);

		iExistCount += 1;
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 5.1.6. Grid Load Cases Data를 구성함.
BOOL CRptJTG_D62_04::MakeGridLoadData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	CString aHeadNames[] = { _LS(IDS_DGN_PSC_RPTJTG_Number), _LS(IDS_DGN_PSC_RPTJTG_Name),
		_LS(IDS_DGN_PSC_RPTJTG_Type), _LS(IDS_DGN_PSC_RPTJTG_Description) };
	for ( int i=0; i<3; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames[i], 1));
	}

	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CArray<T_GILC_K, T_GILC_K> arGilcKeyList;
	m_pDoc->m_pAttrCtrl->GetGilcKeyList(arGilcKeyList);

	CString aLoadType[5] = { _LS(IDS_TMWK_GRID_LOAD_DEAD), _LS(IDS_TMWK_GRID_LOAD_ODEAD),
		_LS(IDS_TMWK_GRID_LOAD_VLIVE), _LS(IDS_TMWK_GRID_LOAD_CLIVE), _LS(IDS_TMWK_GRID_LOAD_OLIVE) };

	int iCount = arGilcKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		T_GILC_K gilcK = arGilcKeyList.GetAt(i);
		T_GILC_D gilcD; gilcD.Initialize();
		if ( !m_pDoc->m_pAttrCtrl->GetGilc(gilcK, gilcD) ) continue;

		CString strLoadCaseId = _T("");
		strLoadCaseId.Format(_T("%d"), gilcD.LoadCaseId);

		tdData.Initialize();
		tdData.arTDK.Add(AddTDString(strLoadCaseId, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(gilcD.LoadCaseName, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(aLoadType[gilcD.nLoadCaseType], 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(gilcD.Description, 0, 1, 1, m_TDCenterCssK));
		m_pRptData->AddTRData(tdK, tdData);
		tableD.arTRK.Add(tdK);

		iExistCount += 1;
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 5.2.1 하중조합 Case List Data를 구성함.
//   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
//   paraD : 구성된 Paragraph Data.
//   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.   
BOOL CRptJTG_D62_04::MakeLoadCombCaseData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.Initialize();

	CString aHeadNames[] = { _LS(IDS_DGN_PSC_RPTJTG_Name), _LS(IDS_DGN_PSC_RPTJTG_Description) };
	for ( int i=0; i<2; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames[i], 1, 1, 1));
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CMap<CString, LPCTSTR, CString, CString&> mapLcom;
	GetLoadCombinationCases(mapLcom);

	// Table Data.
	CString strKey  = _T("");
	CString strData = _T("");
	tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

	POSITION pos = mapLcom.GetStartPosition();
	while ( pos != NULL )
	{
		mapLcom.GetNextAssoc(pos, strKey, strData);
		tdData.Initialize();

		tdData.arTDK.Add(AddTDString(strKey, 0, 1, 1, m_TDCenterCssK));
		tdData.arTDK.Add(AddTDString(strData, 0, 1, 1, m_TDCenterCssK));

		m_pRptData->AddTRData(tdK, tdData);
		tableD.arTRK.Add(tdK);

		iExistCount += 1;
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// 5.2 하중조합 설명 Data를 구성함.
BOOL CRptJTG_D62_04::MakeLoadCombData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;

	// Table Header.
	thData.Initialize();

	CString aHeadNames[] = { _LS(IDS_DGN_PSC_RPTJTG_Name), _LS(IDS_DGN_PSC_RPTJTG_Active),
		_LS(IDS_TB_ELASTIC), _LS(IDS_DGN_PSC_RPTJTG_Description) };
	for ( int i=0; i<4; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames[i], 1, 1, 1));
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	// Table Data.
	CString strText = _T("");
	tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

	CArray<T_LCOM_K, T_LCOM_K> arLcomKeyList;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arLcomKeyList);

	int iCount = arLcomKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		T_LCOM_D lcomD;
		if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, arLcomKeyList[i], lcomD) )
		{
			tdData.Initialize();

			tdData.arTDK.Add(AddTDString(lcomD.LoadCombName, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(GetLcombActiveType(lcomD.nActive), 0, 1, 1, m_TDCenterCssK));
			strText = (lcomD.bElasticStage)? _T("Yes") : _T("No");
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
			tdData.arTDK.Add(AddTDString(lcomD.Description, 0, 1, 1));

			tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			iExistCount += 1;
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 6.1. 시공단계 정단면 수직방향 응력 검토 Data를 구성함.
BOOL CRptJTG_D62_04::MakeConstructStageVertStressData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;

	// Table Header.
	thData.Initialize();

	CString strText = _T("");

	CString aHeadNames1[] = { _LS(IDS_TB_PSCD_FYSG_JTG_ELEM), _LS(IDS_TB_PSCD_FYSG_JTG_PART),
		_LS(IDS_TB_PSCD_FYSG_JTG_MAX_MIN), _LS(IDS_TB_PSCD_FYSG_JTG_STAGE),
		_LS(IDS_TB_PSCD_FYSG_JTG_OK) };
	for ( int i=0; i<5; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 2));
	}

	CString aHeadNames2[] = { _LS(IDS_TB_PSCD_FYSG_JTG_FT), _LS(IDS_TB_PSCD_FYSG_JTG_FTL),
		_LS(IDS_TB_PSCD_FYSG_JTG_FTR) };
	for ( int i=0; i<3; i++ )
	{
		thData.arTDK.Add(AddTDwithUnit(aHeadNames2[i], m_strStressUnit, 1, 1, 1));
	}

	thData.arTDK.Add(AddTDwithUnit(_LS(IDS_TB_PSCD_FYSG_JTG_FMAX), m_strStressUnit, 1, 1, 2));
	thData.arTDK.Add(AddTDwithUnit(_LS(IDS_TB_PSCD_FYSG_JTG_ALW), m_strStressUnit, 1, 1, 2));

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	thData.Initialize();

	CString aHeadNames3[] = { _LS(IDS_TB_PSCD_FYSG_JTG_FB), _LS(IDS_TB_PSCD_FYSG_JTG_FBL),
		_LS(IDS_TB_PSCD_FYSG_JTG_FBR) };
	for ( int i=0; i<3; i++ )
	{
		thData.arTDK.Add(AddTDwithUnit(aHeadNames3[i], m_strStressUnit, 1, 1, 1));
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	for ( int i=0; i<m_iElemNum; i++ )
	{
		ElemPairK EPairK(m_arElemKeyList[i], EN_EL_BEAM);
		T_FYSG_D fysgD;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFysgData(EPairK, fysgD);

		for ( int j=0; j<4; j++ )
		{
			tdData.Initialize();

			T_FYSG_BASE fysgBase = fysgD.FysgBase[j];

			if ( fysgBase.bCHK )
			{
				strText.Format(_T("%d"), EPairK.first);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				strText = (j<2)? _T("I") : _T("J");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				strText = (j%2 > 0)? _T("Min") : _T("Max");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				T_STAG_D stagD;
				m_pDoc->m_pAttrCtrl->GetStag(fysgBase.StagK, stagD);
				tdData.arTDK.Add(AddTDString(stagD.StageName, 0, 1, 2, m_TDCenterCssK));

				strText = (fysgBase.bOK)? _T("OK") : _T("NG");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				strText.Format(_T("%.3f"), fysgBase.dFT);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText.Format(_T("%.3f"), fysgBase.dFTL);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText.Format(_T("%.3f"), fysgBase.dFTR);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText.Format(_T("%.3f"), fysgBase.dFMAX);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				strText.Format(_T("%.3f"), fysgBase.dALW);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				tdData.CssK = m_TDCenterCssK;
				m_pRptData->AddTRData(tdK, tdData);
				tableD.arTRK.Add(tdK);

				tdData.Initialize();

				strText.Format(_T("%.3f"), fysgBase.dFB);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText.Format(_T("%.3f"), fysgBase.dFBL);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText.Format(_T("%.3f"), fysgBase.dFBR);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
				m_pRptData->AddTRData(tdK, tdData);
				tableD.arTRK.Add(tdK);

				iExistCount += 1;
			}
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 6.2. 인장구역철근의 인장응력 검토 Data를 구성함.
//   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
//   paraD : 구성된 Paragraph Data.
//   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.   
BOOL CRptJTG_D62_04::MakeTensRebarStressData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();
	CString strText = _T("");

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;

	// Table Header.
	thData.Initialize();

	CString aHeadNames1[] = { _LS(IDS_TB_PSCD_FYPC_JTG_TNDN), _LS(IDS_TB_PSCD_FYPC_JTG_OK) };
	for ( int i=0; i<2; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
	}

	CString aHeadNames2[] = { _LS(IDS_TB_PSCD_FYPC_JTG_FDL1), _LS(IDS_TB_PSCD_FYPC_JTG_FLL),
		_LS(IDS_TB_PSCD_FYPC_JTG_AFDL1), _LS(IDS_TB_PSCD_FYPC_JTG_AFLL) };
	for ( int i=0; i<4; i++ )
	{
		thData.arTDK.Add(AddTDwithUnit(aHeadNames2[i], m_strStressUnit, 1, 1, 1));
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	for ( int i=0; i<m_iTendonNum; i++ )
	{
		T_FYPC_JTG fypcJTGD;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFypcJtgData(m_arTdnaK[i], fypcJTGD);

		if ( fypcJTGD.bCHK )
		{
			tdData.Initialize();
			strText.Format(_T("%d"), m_arTdnaK[i]);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText = (fypcJTGD.bOK)? _T("OK") : _T("NG");
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText.Format(_T("%.3f"), fypcJTGD.dFDL1);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText.Format(_T("%.3f"), fypcJTGD.dFLL);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText.Format(_T("%.3f"), fypcJTGD.dAFDL1);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			strText.Format(_T("%.3f"), fypcJTGD.dAFLL);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			tdData.CssK = m_TDCenterCssK;
			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);

			iExistCount += 1;
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 6.3. 사용단계 정단면 균열 검토 Data를 구성함.
BOOL CRptJTG_D62_04::MakeServiceStageCrackData(int& iChapID, T_PARA_RPT& paraD)
{
	return MakeFycmJTGData(TRUE, iChapID, paraD);
}


// 6.4. 사용단계 경사단면 균열폭 검토
BOOL CRptJTG_D62_04::MakeServiceStageCrackSlopeData(int& iChapID, T_PARA_RPT& paraD)
{
	// Modify by GAY. MNET:2397. ('06.08.17). To Output Crack result, input bMax parameter as _T("TRUE").
	return MakeFymsJTGData(TRUE, iChapID, paraD);
}

// 6.5. 사용단계 정단면 압축응력 검토 Data를 구성함.
BOOL CRptJTG_D62_04::MakeServiceStageCompStressData(int& iChapID, T_PARA_RPT& paraD)
{
	return MakeFycmJTGData(FALSE, iChapID, paraD);
}

// 6.6. 사용단계 경사단면 압축응력 검토 Data를 구성함.
BOOL CRptJTG_D62_04::MakeServiceStageCompStressSlopeData(int& iChapID, T_PARA_RPT& paraD)
{
	return MakeFymsJTGData(FALSE, iChapID, paraD);
}

// 6.7. 사용단계 균열폭 검토 Data를 구성함.
BOOL CRptJTG_D62_04::MakeServiceStageCrackWidthData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;

	// Table Header.
	thData.Initialize();

	CString aHeadNames1[] = { _LS(IDS_TB_PSCD_BDCW_JTG_ELEM), _LS(IDS_TB_PSCD_BDCW_JTG_PART),
		_LS(IDS_TB_PSCD_BDCW_JTG_MAX_MIN), _LS(IDS_TB_PSCD_BDCW_JTG_LOAD),
		_LS(IDS_TB_PSCD_BDCW_JTG_TYPE), _LS(IDS_TB_PSCD_BDCW_JTG_OK) };

	for ( int i=0; i<6; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
	}

	CString aHeadNames2[] = { _LS(IDS_TB_PSCD_BDCW_JTG_FT), _LS(IDS_TB_PSCD_BDCW_JTG_FB),
		_LS(IDS_TB_PSCD_BDCW_JTG_FRT) };

	for ( int i=0; i<3; i++ )
	{
		thData.arTDK.Add(AddTDwithUnit(aHeadNames2[i], m_strStressUnit, 1, 1, 1));
	}

	thData.arTDK.Add(AddTDwithUnit(_LS(IDS_TB_PSCD_BDCW_JTG_WC), m_strLengthUnit, 1, 1, 1));
	thData.arTDK.Add(AddTDwithUnit(_LS(IDS_TB_PSCD_BDCW_JTG_AWC), m_strLengthUnit, 1, 1, 1));

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CString strText = _T("");
	for ( int i=0; i<m_iElemNum; i++ )
	{
		ElemPairK EPairK(m_arElemKeyList[i], EN_EL_BEAM);
		T_BDCW_D bdcwD;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBdcwData(EPairK, bdcwD);

		for ( int j=0; j<4; j++ )
		{
			tdData.Initialize();

			T_BDCW_BASE bdcwBase = bdcwD.BdcwBase[j];

			if ( bdcwBase.bCHK )
			{
				strText.Format(_T("%d"), EPairK.first);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (j<2)? _T("I") : _T("J");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				int index = (j%2==0)? 0 : 1;
				strText = GetTopBottomType(index);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = GetLoadCombName(bdcwBase.LcomK);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = GetLoadCombType(bdcwBase.iMax);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (bdcwBase.bOK)? _T("OK") : _T("NG");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				double dFP1[] = { bdcwBase.dFT, bdcwBase.dFB, bdcwBase.dFRT };
				for ( int k=0; k<3; k++ )
				{
					strText.Format(_T("%.3f"), dFP1[k]);
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
				}

				double dWs[] = { bdcwBase.dWC, bdcwBase.dAWC };
				for ( int k=0; k<2; k++ )
				{
					strText.Format(_T("%.4f"), dWs[k]);
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
				}

				tdData.CssK = m_TDCenterCssK;
				m_pRptData->AddTRData(tdK, tdData);
				tableD.arTRK.Add(tdK);

				iExistCount += 1;
			}
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 6.8. 사용단계 정단면 휨검토 Data를 구성함.
BOOL CRptJTG_D62_04::MakeServiceStageMomentCheckData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;

	// Table Header.
	thData.Initialize();

	CString aHeadNames1[] = { _LS(IDS_TB_PSCD_BDCY_JTG_ELEM), _LS(IDS_TB_PSCD_BDCY_JTG_PART),
		_LS(IDS_TB_PSCD_BDCY_JTG_MAX_MIN), _LS(IDS_TB_PSCD_BDCY_JTG_LOAD),
		_LS(IDS_TB_PSCD_BDCY_JTG_TYPE), _LS(IDS_TB_PSCD_BDCY_JTG_OK) };
	for ( int i=0; i<6; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
	}

	CString aHeadNames2[] = { _LS(IDS_TB_PSCD_BDCY_JTG_MUY), _LS(IDS_TB_PSCD_BDCY_JTG_MNY) };
	for ( int i=0; i<2; i++ )
	{
		thData.arTDK.Add(AddTDwithUnit(aHeadNames2[i], m_strMomentUnit, 1, 1, 1));
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CString strText = _T("");
	for ( int i=0; i<m_iElemNum; i++ )
	{
		ElemPairK EPairK(m_arElemKeyList[i], EN_EL_BEAM);
		T_BDCY_JTG bdcyD;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBdcyJtgData(EPairK, bdcyD);

		for ( int j=0; j<4; j++ )
		{
			tdData.Initialize();

			T_BDCY_JTG_BASE bdcyBase = bdcyD.BdcyBase[j];

			if ( bdcyBase.bCHK )
			{
				strText.Format(_T("%d"), EPairK.first);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (j<2)? _T("I") : _T("J");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				int index = (j%2==0)? 0 : 1;
				strText = GetMaxMinType(index);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = GetLoadCombName(bdcyBase.LcomK);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = GetLoadCombType(bdcyBase.iMax);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (bdcyBase.bOK)? _T("OK") : _T("NG");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				double dFP1[] = { bdcyBase.drMuy, bdcyBase.dMny };
				for ( int k=0; k<2; k++ )
				{
					strText.Format(_T("%.3f"), dFP1[k] * m_dMomentkNM);
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
				}

				tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
				m_pRptData->AddTRData(tdK, tdData);
				tableD.arTRK.Add(tdK);

				iExistCount += 1;
			}
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 6.9. 사용단계 경사단면 전단검토 Data를 구성함.
BOOL CRptJTG_D62_04::MakeServiceStageShearCheckData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;

	// Table Header.
	thData.Initialize();

	CString aHeadNames1[] = { _LS(IDS_TB_PSCD_CUMS_JTG_ELEM), _LS(IDS_TB_PSCD_CUMS_JTG_PART),
		_LS(IDS_TB_PSCD_CUMS_JTG_PART), _LS(IDS_TB_PSCD_CUMS_JTG_LOAD),
		_LS(IDS_TB_PSCD_CUMS_JTG_TYPE), _LS(IDS_TB_PSCD_CUMS_JTG_OK),
		_LS(IDS_TB_PSCD_CUMS_JTG_VU), _LS(IDS_TB_PSCD_CUMS_JTG_VN),
		_LS(IDS_TB_PSCD_CUMS_JTG_SECT_OK) , _LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD) };
	for ( int i=0; i<10; i++ )
	{
		if ( i==6 || i==7 )
		{
			thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strForceUnit, 1, 1, 1));
		}
		else
		{
			thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
		}
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CString strText = _T("");
	for ( int i=0; i<m_iElemNum; i++ )
	{
		ElemPairK EPairK(m_arElemKeyList[i], EN_EL_BEAM);
		T_CUMS_JTG cumsD;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCumsJtgData(EPairK, cumsD);

		for ( int j=0; j<4; j++ )
		{
			tdData.Initialize();

			T_CUMS_JTG_BASE cumsBase = cumsD.CumsBase[j];

			if ( cumsBase.bCHK )
			{
				strText.Format(_T("%d"), EPairK.first);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (j<2)? _T("I") : _T("J");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				int index = (j%2==0)? 0 : 1;
				strText = GetMaxMinType(index);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = GetLoadCombName(cumsBase.LcomK);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = GetLoadCombType(cumsBase.iMax);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (cumsBase.bOK)? _T("OK") : _T("NG");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				double dFP1[] = { cumsBase.drVu, cumsBase.dVn };
				for ( int k=0; k<2; k++ )
				{
					strText.Format(_T("%.3f"), dFP1[k] * m_dForcekN);
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
				}

				strText = (fabs(cumsBase.drVu) <= cumsBase.dShrSect)? _T("OK") : _T("NG");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (fabs(cumsBase.drVu) <= cumsBase.dShrLoad)? _LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD_SKIP) : _LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD_CHECK);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
				m_pRptData->AddTRData(tdK, tdData);
				tableD.arTRK.Add(tdK);

				iExistCount += 1;
			}
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 6.10. 사용단계 비틀림 검토 Data를 구성함.
BOOL CRptJTG_D62_04::MakeServiceStageTorsionCheckData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;

	// Table Header.
	thData.Initialize();

	CString aHeadNames1[] = { _LS(IDS_TB_PSCD_CRMT_JTG_ELEM), _LS(IDS_TB_PSCD_CRMT_JTG_PART),
		_LS(IDS_TB_PSCD_CRMT_JTG_MAX_MIN), _LS(IDS_TB_PSCD_CRMT_JTG_LOAD),
		_LS(IDS_TB_PSCD_CRMT_JTG_TYPE), _LS(IDS_TB_PSCD_CRMT_JTG_OK),
		_LS(IDS_TB_PSCD_CRMT_JTG_TU), _LS(IDS_TB_PSCD_CRMT_JTG_TN),
		_LS(IDS_TB_PSCD_CRMT_JTG_VU), _LS(IDS_TB_PSCD_CRMT_JTG_VN),
		_LS(IDS_TB_PSCD_CRMT_JTG_SECT_OK), _LS(IDS_TB_PSCD_CRMT_JTG_TORLOAD) };
	for ( int i=0; i<12; i++ )
	{
		if ( i==6 || i==7 )
		{
			thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strMomentUnit, 1, 1, 1));
		}
		else if ( i==8 || i==9 )
		{
			thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strForceUnit, 1, 1, 1));
		}
		else
		{
			thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
		}
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CString strText = _T("");
	for ( int i=0; i<m_iElemNum; i++ )
	{
		ElemPairK EPairK(m_arElemKeyList[i], EN_EL_BEAM);
		T_CRMT_JTG crmtD;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCrmtJtgData(EPairK, crmtD);

		for ( int j=0; j<6; j++ )
		{
			tdData.Initialize();

			T_CRMT_JTG_BASE crmtBase = crmtD.CrmtBase[j];

			if ( crmtBase.bCHK )
			{
				strText.Format(_T("%d"), EPairK.first);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (j<2)? _T("I") : _T("J");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				int index = j%3;
				strText = GetMaxMinTypeShear(index);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = GetLoadCombName(crmtBase.LcomK);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = GetLoadCombType(crmtBase.iMax);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (crmtBase.bOK)? _T("OK") : _T("NG");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				double dFP1[] = { crmtBase.drTu, crmtBase.dTn, crmtBase.drVu, crmtBase.dVn };
				for ( int k=0; k<4; k++ )
				{
					double factor = (k<2)? m_dMomentkNM : m_dForcekN;
					strText.Format(_T("%.3f"), dFP1[k] * factor);
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
				}

				strText = (fabs(crmtBase.dShrTorVal) <= crmtBase.dTorSect)? _T("OK") : _T("NG");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (fabs(crmtBase.dShrTorVal) <= crmtBase.dTorLoad)? _LS(IDS_TB_PSCD_CRMT_JTG_TORLOAD_SKIP) : _LS(IDS_TB_PSCD_CRMT_JTG_TORLOAD_CHECK);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				tdData.CssK = m_TDCenterCssK;
				m_pRptData->AddTRData(tdK, tdData);
				tableD.arTRK.Add(tdK);

				iExistCount += 1;
			}
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
BOOL CRptJTG_D62_04::MakeServiceStageCompressionCheckData(int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;

	// Table Header.
	thData.Initialize();

	CString aHeadNames1[] = { _LS(IDS_TB_PSCD_BDCY_JTG_ELEM), _LS(IDS_TB_PSCD_BDCY_JTG_PART), _LS(IDS_TB_PSCD_BDCW_JTG_LOAD),
		_LS(IDS_TB_PSCD_BDCW_JTG_TYPE),_LS(IDS_TB_PSCD_BDCY_JTG_OK),
		_T("x"),_T("rNd"), _T("e"), _T("e'"),  _T("Nn") };

	CString aMaxMinName[]= { _T("菉懃-Fxmin"),_T("튤懃-Fxmin(My)"),_T("튤懃-Fxmin(Mz)"),_T("튤懃-Mymax"),_T("튤懃-Mymin") };

	for ( int i=0; i<10; i++ )
	{
		if ( i==5|| i==7 || i==8 )
		{
			thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strLengthUnit, 1, 1, 1));
		}
		else if ( i==6 || i==9 )
		{
			thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strForceUnit, 1, 1, 1));
		}
		else
		{
			thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
		}
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CString strText = _T("");
	for ( int i=0; i<m_iElemNum; i++ )
	{
		ElemPairK EPairK(m_arElemKeyList[i], EN_EL_BEAM);
		T_CSTCS_CH Cscs;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCscsJtgData(EPairK, Cscs);

		for ( int j=0; j<10; j++ )
		{
			if ( j== 2 || j ==7 ) continue;//鬧姦딜Mz돨駱炬

			tdData.Initialize();

			T_CSTCS_CH_BASE cscsBase = Cscs.cstcsBase[j];

			if ( cscsBase.bCHK )
			{
				strText.Format(_T("%d"), EPairK.first);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (j<5)? _T("I") : _T("J");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = GetLoadCombName(cscsBase.LcomK);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				//        strText = GetLoadCombType(cscsBase.iMax);
				strText =aMaxMinName[j%5];
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (cscsBase.bOK)? _T("OK") : _T("NG");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText.Format(_T("%.3f"), cscsBase.dx);//dx
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				for ( int k=0; k<4; k++ )
				{
					if ( cscsBase.drNd < 0 ) {//is tensile
						if ( k==0 ) strText.Format(_T("%.3f"), cscsBase.drNd * m_dForcekN); else strText = _T("-");
					}
					else {
						if ( k ==0 ) strText.Format(_T("%.3f"), cscsBase.drNd * m_dForcekN);
						if ( k ==1 ) strText.Format(_T("%.3f"), cscsBase.drNd < 1.0E-07 ? 0.0 : cscsBase.drNde /cscsBase.drNd);
						if ( k ==2 ) strText.Format(_T("%.3f"), cscsBase.drNd < 1.0E-07 ? 0.0 : cscsBase.drNde1 /cscsBase.drNd);
						if ( k ==3 ) strText.Format(_T("%.3f"), cscsBase.dNn* m_dForcekN);
					}
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
				}

				tdData.CssK = m_TDCenterCssK;
				m_pRptData->AddTRData(tdK, tdData);
				tableD.arTRK.Add(tdK);

				iExistCount += 1;
			}
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// T_FYCM_JTG Data 출력관련 정보를 구성함.
BOOL CRptJTG_D62_04::MakeFycmJTGData(BOOL bMax, int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;

	// Table Header.
	thData.Initialize();

	if ( bMax )
	{
		CString aHeadNames1[] = { _LS(IDS_TB_PSCD_FYCM_JTG_ELEM), _LS(IDS_TB_PSCD_FYCM_JTG_PART),
			_LS(IDS_TB_PSCD_FYCM_JTG_LOAD), _LS(IDS_TB_PSCD_Short_Long),
			_LS(IDS_TB_PSCD_FYCM_JTG_TYPE), _LS(IDS_TB_PSCD_FYCM_JTG_OK) };
		for ( int i=0; i<6; i++ )
		{
			thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 2));
		}
	}
	else
	{
		CString aHeadNames1[] = { _LS(IDS_TB_PSCD_FYCM_JTG_ELEM), _LS(IDS_TB_PSCD_FYCM_JTG_PART),
			_LS(IDS_TB_PSCD_FYCM_JTG_LOAD), _LS(IDS_TB_PSCD_FYCM_JTG_TYPE), _LS(IDS_TB_PSCD_FYCM_JTG_OK) };
		for ( int i=0; i<5; i++ )
		{
			thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 2));
		}
	}

	CString strText = _T("");
	CString aHeadNames2[] = { _LS(IDS_TB_PSCD_FYCM_JTG_FT), _LS(IDS_TB_PSCD_FYCM_JTG_FTL),
		_LS(IDS_TB_PSCD_FYCM_JTG_FTR) };
	for ( int i=0; i<3; i++ )
	{
		thData.arTDK.Add(AddTDwithUnit(aHeadNames2[i], m_strStressUnit, 1, 1, 1));
	}

	thData.arTDK.Add(AddTDwithUnit(_LS(IDS_TB_PSCD_FYCM_JTG_FMAX), m_strStressUnit, 1, 1, 2));
	thData.arTDK.Add(AddTDwithUnit(_LS(IDS_TB_PSCD_FYCM_JTG_ALW), m_strStressUnit, 1, 1, 2));

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	thData.Initialize();
	CString aHeadNames3[] = { _LS(IDS_TB_PSCD_FYCM_JTG_FB), _LS(IDS_TB_PSCD_FYCM_JTG_FBL),
		_LS(IDS_TB_PSCD_FYCM_JTG_FB) };
	for ( int i=0; i<3; i++ )
	{
		thData.arTDK.Add(AddTDwithUnit(aHeadNames3[i], m_strStressUnit, 1, 1, 1));
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	LPCTSTR aLoadTerm[] = { _LS(IDS_TB_PSCD_SHORT), _LS(IDS_TB_PSCD_LONG) };

	for ( int i=0; i<m_iElemNum; i++ )
	{
		ElemPairK EPairK(m_arElemKeyList[i], EN_EL_BEAM);
		T_FYCM_JTG fycmD;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFycmJtgData(EPairK, fycmD);

		// Modify by GAY. MNET:2397. ('06.08.17).
		// Add LongTerm result only if crack check in service stage.
		// Reference this index. (See struct T_FYCM_JTG) 단기 : 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN. 장기 : 4=I-MIN, 5=J-MIN
		if ( bMax )
		{
			for ( int j=4; j<6; j++ )
			{
				tdData.Initialize();

				T_FYCM_JTG_BASE fycmBase = fycmD.FycmBase[j];

				if ( fycmBase.bCHK )
				{
					strText.Format(_T("%d"), EPairK.first);
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

					strText = (j==4)? _T("I") : _T("J");
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

					strText = GetLoadCombName(fycmBase.LcomK);
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

					if ( bMax )
					{
						tdData.arTDK.Add(AddTDString(aLoadTerm[fycmBase.iLoadTerm-1], 0, 1, 2, m_TDCenterCssK));
					}

					tdData.arTDK.Add(AddTDString(GetLoadCombType(fycmBase.iMax), 0, 1, 2, m_TDCenterCssK));

					strText = (fycmBase.bOK)? _T("OK") : _T("NG");
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

					double dFT[] = { fycmBase.dFT, fycmBase.dFTL, fycmBase.dFTR, fycmBase.dFMAX, fycmBase.dALW };
					for ( int k=0; k<5; k++ )
					{
						strText.Format(_T("%.3f"), dFT[k]);
						int iSubIndex = (k>2)? 2 : 1;  // dFMAX, dALW는 RowMerge 형태로 출력.
						tdData.arTDK.Add(AddTDString(strText, 0, 1, iSubIndex, m_TDCenterCssK));
					}

					tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
					m_pRptData->AddTRData(tdK, tdData);
					tableD.arTRK.Add(tdK);

					tdData.Initialize();

					double dFB[] = { fycmBase.dFB, fycmBase.dFBL, fycmBase.dFBR };
					for ( int k=0; k<3; k++ )
					{
						strText.Format(_T("%.3f"), dFB[k]);
						tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
					}

					tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
					m_pRptData->AddTRData(tdK, tdData);
					tableD.arTRK.Add(tdK);

					iExistCount += 1;
				}
			}
		}

		for ( int j=0; j<2; j++ )
		{
			tdData.Initialize();

			int index = (bMax)? 2*j+1 : 2*j;
			T_FYCM_JTG_BASE fycmBase = fycmD.FycmBase[index];

			if ( fycmBase.bCHK )
			{
				strText.Format(_T("%d"), m_arElemKeyList[i]);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				strText = (j<1)? _T("I") : _T("J");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				strText = GetLoadCombName(fycmBase.LcomK);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				if ( bMax )
				{
					tdData.arTDK.Add(AddTDString(aLoadTerm[fycmBase.iLoadTerm-1], 0, 1, 2, m_TDCenterCssK));
				}

				tdData.arTDK.Add(AddTDString(GetLoadCombType(fycmBase.iMax), 0, 1, 2, m_TDCenterCssK));

				strText = (fycmBase.bOK)? _T("OK") : _T("NG");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				double dFT[] = { fycmBase.dFT, fycmBase.dFTL, fycmBase.dFTR, fycmBase.dFMAX, fycmBase.dALW };
				for ( int k=0; k<5; k++ )
				{
					strText.Format(_T("%.3f"), dFT[k]);
					int iSubIndex = (k>2)? 2 : 1;  // dFMAX, dALW는 RowMerge 형태로 출력.
					tdData.arTDK.Add(AddTDString(strText, 0, 1, iSubIndex, m_TDCenterCssK));
				}

				tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
				m_pRptData->AddTRData(tdK, tdData);
				tableD.arTRK.Add(tdK);

				tdData.Initialize();

				double dFB[] = { fycmBase.dFB, fycmBase.dFBL, fycmBase.dFBR };
				for ( int k=0; k<3; k++ )
				{
					strText.Format(_T("%.3f"), dFB[k]);
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
				}

				tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
				m_pRptData->AddTRData(tdK, tdData);
				tableD.arTRK.Add(tdK);

				iExistCount += 1;
			}
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// T_FYMS_JTG Data 출력관련 정보를 구성함.
BOOL CRptJTG_D62_04::MakeFymsJTGData(BOOL bMax, int& iChapID, T_PARA_RPT& paraD)
{
	int iExistCount = FALSE;
	paraD.Initialize();

	T_TABLE_K tableK;
	T_TABLE_RPT tableD;
	tableD.Initialize();
	tableD.nCheck = 1;
	tableD.strTitle = _T("Table");

	T_TR_K thK, tdK;
	T_TR_RPT thData, tdData;
	CArray<T_CHAR_K, T_CHAR_K> arCharK;

	// Table Header.
	thData.Initialize();

	CString aHeadNames1[] = { _LS(IDS_TB_PSCD_FPMS_JTG_ELEM), _LS(IDS_TB_PSCD_FPMS_JTG_PART),
		_LS(IDS_TB_PSCD_FPMS_JTG_LOAD), _LS(IDS_TB_PSCD_FPMS_JTG_MAX_MIN) };
	for ( int i=0; i<4; i++ )
	{
		thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 2));
	}

	CString aHeadNames2[] = { _LS(IDS_TB_PSCD_FPMS_JTG_FMAX), _LS(IDS_TB_PSCD_FPMS_JTG_AFP) };
	for ( int i=0; i<2; i++ )
	{
		thData.arTDK.Add(AddTDwithUnit(aHeadNames2[i], m_strStressUnit, 1, 1, 2));
	}

	CString aHeadNames3[] = { _LS(IDS_TB_PSCD_FPMS_JTG_FP1), _LS(IDS_TB_PSCD_FPMS_JTG_FP2),
		_LS(IDS_TB_PSCD_FPMS_JTG_FP3), _LS(IDS_TB_PSCD_FPMS_JTG_FP4),
		_LS(IDS_TB_PSCD_FPMS_JTG_FP5), _LS(IDS_TB_PSCD_FPMS_JTG_FP6),
		_LS(IDS_TB_PSCD_FPMS_JTG_FP7), _LS(IDS_TB_PSCD_FPMS_JTG_FP8),
		_LS(IDS_TB_PSCD_FPMS_JTG_FP9), _LS(IDS_TB_PSCD_FPMS_JTG_FP10) };
	for ( int i=0; i<5; i++ )
	{
		thData.arTDK.Add(AddTDwithUnit(aHeadNames3[i], m_strStressUnit, 1, 1, 1));
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	thData.Initialize();
	for ( int i=5; i<10; i++ )
	{
		thData.arTDK.Add(AddTDwithUnit(aHeadNames3[i], m_strStressUnit, 1, 1, 1));
	}

	thData.CssK = m_pRptData->m_DefaultCss.THCssK;
	m_pRptData->AddTRData(thK, thData);
	tableD.arTHK.Add(thK);

	CString strText = _T("");
	for ( int i=0; i<m_iElemNum; i++ )
	{
		ElemPairK EPairK(m_arElemKeyList[i], EN_EL_BEAM);
		T_FPMS_JTG fysmD;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFpmsJtgData(EPairK, fysmD);

		for ( int j=0; j<2; j++ )
		{
			tdData.Initialize();

			int index = (bMax)? j*2+1 : j*2;
			T_FPMS_JTG_BASE fysmBase = fysmD.FpmsBase[index];

			if ( fysmBase.bCHK )
			{
				strText.Format(_T("%d"), EPairK.first);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				strText = (j<1)? _T("I") : _T("J");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				strText = GetLoadCombName(fysmBase.LcomK);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				strText = (fysmBase.bOK)? _T("OK") : _T("NG");
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				strText.Format(_T("%.3f"), fysmBase.dFMAX);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				strText.Format(_T("%.3f"), fysmBase.dAFP);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

				double dFP1[] = { fysmBase.dFP1, fysmBase.dFP2, fysmBase.dFP3, fysmBase.dFP4, fysmBase.dFP5 };
				for ( int k=0; k<5; k++ )
				{
					strText.Format(_T("%.3f"), dFP1[k]);
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
				}

				tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
				m_pRptData->AddTRData(tdK, tdData);
				tableD.arTRK.Add(tdK);

				tdData.Initialize();
				double dFp6[] = { fysmBase.dFP6, fysmBase.dFP7, fysmBase.dFP8, fysmBase.dFP9, fysmBase.dFP10 };
				for ( int k=0; k<5; k++ )
				{
					strText.Format(_T("%.3f"), dFp6[k]);
					tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
				}

				tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
				m_pRptData->AddTRData(tdK, tdData);
				tableD.arTRK.Add(tdK);

				iExistCount += 1;
			}
		}
	}

	if ( iExistCount > 0 )
	{
		iChapID++;
		m_pRptData->AddTableData(tableK, tableD);
		paraD.arContentK.Add(tableK);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//***************************************************************
// Get Data.
//***************************************************************

// 현재 Data 파일의 날짜를 가져옴.
CString CRptJTG_D62_04::GetFileDate()
{
	CString strDate = _T("");
	CString strPath = m_pDoc->GetPathName();

	if ( !strPath.IsEmpty() )
	{
		CFileCtrl file(strPath);
		CFileStatus status;
		file.GetFileStatus(status);

		int iYY = status.m_mtime.GetYear();
		int iMM = status.m_mtime.GetMonth();

#if defined(_CH) || defined(_ORG)
		strDate = GetFileDateForChina(iYY, iMM);
#else
		strDate.Format(_LS(IDS_DGN_PSC_RPTJTG_Date), iYY, iMM);
#endif        
	}

	return strDate;
}

// 충격계수 값을 가져옴.
CString CRptJTG_D62_04::GetImpactCoefficient()
{
	// Modify by GAY. MNET:2404. ('06.08.22). 
	CMvLoad mvLoad;

	CString strText = _T("");
	strText.Format(_T("%.3f"), mvLoad.CalcMovingImpactFactor());

	return strText;
}

// Beam Section Temperature Load의 방향성 구함.
CString CRptJTG_D62_04::GetBeamSectTempLoadDirection(int iDir)
{
	CString strText = _T("");
	if ( iDir == 0 )
	{
		strText = _LS(IDS_TB_BTMP_Local_y);
	}
	else if ( iDir == 1 )
	{
		strText = _LS(IDS_TB_BTMP_Local_z);
	}
	else
	{
		strText = _LS(IDS_TB_BTMP_None);
	}
	return strText;
}

// 하중조합의 Active Type String 형식으로 구성.
CString CRptJTG_D62_04::GetLcombActiveType(int iActiveType)
{
	CString strText = _T("");

	if ( iActiveType == 1 )
	{
		strText = _LS(IDS_DGN_PSC_RPTJTG_Lcom_StrengthStress);
	}
	else if ( iActiveType == 2 )
	{
		strText = _LS(IDS_DGN_PSC_RPTJTG_Lcom_Servicability);
	}
	else
	{
		strText = _T("Not Defined");
	}

	return strText;
}

// Stage의 Activation Type을 String 형식으로 변환.
CString CRptJTG_D62_04::GetStageActivationType(int iCondition)
{
	CString strText = _T("");

	if ( iCondition==1 )       strText= _T("O");
	else if ( iCondition==-1 ) strText= _T("X");
	else if ( iCondition==0 )  strText= _T("");

	return strText;
}

// Stage의 Total Type을 String 형식으로 변환.
CString CRptJTG_D62_04::GetStageTotalType(int iSummary)
{
	return (iSummary == 1)? _T("O") : _T("");
}

// 설계용 하중조합 Key를 입력받아 실제 하중조합의 이름 구함.
CString CRptJTG_D62_04::GetLoadCombName(T_LCOM_K lcomK)
{
	T_LCOM_K orglcomK;
	T_LCOM_D lcomD;
	m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetOrgLcomK(lcomK, orglcomK);
	if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, orglcomK, lcomD) )
	{
		return lcomD.LoadCombName;
	}
	else
	{
		return _T("-");
	}
}


// Static Load Case의 이름 구함.
//   stldK : Static Load Case의 Key.
CString CRptJTG_D62_04::GetStaticLoadCaseName(T_STLD_K stldK)
{
	T_STLD_D stldD;
	if ( m_pDoc->m_pAttrCtrl->GetStld(stldK, stldD) )
	{
		return stldD.LoadCaseName;
	}
	else
	{
		return _T("-");
	}
}


// 하중조합의 Type 을 String 형식으로 변환.
//   iMaxType : 하중조합 Type (이동하중이 포함된 하중조합에서 최대, 최소 부재력 성분을 출력).
CString CRptJTG_D62_04::GetLoadCombType(int iMaxType)
{
	LPCTSTR aType[] = { _T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN),
										 _LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
										 _LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN) };

	return aType[iMaxType];
}


// 위치의 Type을 String 형식으로 변환.
CString CRptJTG_D62_04::GetTopBottomType(int iLocType)
{
	LPCTSTR aType[] = { _LS(IDS_TB_PSCD_TOP), _LS(IDS_TB_PSCD_BOTTOM) };
	return (iLocType < 0 || iLocType >= 2)? _T("Not Exist") : aType[iLocType];
}


// Min/Max Type을 String 형식으로 변환.
CString CRptJTG_D62_04::GetMaxMinType(int iMaxType)
{
	LPCTSTR aType[]= { _LS(IDS_TB_PSCD_Max), _LS(IDS_TB_PSCD_Min) };
	return (iMaxType < 0 || iMaxType >= 2)? _T("Not Exist") : aType[iMaxType];
}


// 비틀림 검토시, Min/Max Type을 String 형식으로 변환.
//   iMaxType : 최대/최소 Type.
CString CRptJTG_D62_04::GetMaxMinTypeShear(int iMaxType)
{
	LPCTSTR aType[]= { _LS(IDS_TB_PSCD_VMax), _LS(IDS_TB_PSCD_VMin),_LS(IDS_TB_PSCD_TMax) };
	return (iMaxType < 0 || iMaxType >= 3)? _T("Not Exist") : aType[iMaxType];
}

// 해당 stage에서의 Element Group List를 얻음.
void CRptJTG_D62_04::GetElementGroupList(T_STAG_K stagK, CArray<T_TEXT_K, T_TEXT_K>& arActiveK, CArray<T_TEXT_K, T_TEXT_K>& arDeactiveK)
{
	arActiveK.RemoveAll();
	arDeactiveK.RemoveAll();

	T_TEXT_K textK;
	T_TEXT_RPT textD;

	T_STAG_D stagD;
	if ( !m_pDoc->m_pAttrCtrl->GetStag(stagK, stagD) ) return;

	int iCount = m_arGroupKeyList.GetSize();
	for ( int j=0; j<iCount; j++ )
	{
		T_GRUP_K grupK = m_arGroupKeyList[j];
		CString strGrupName = m_pDoc->m_pAttrCtrl->GetGrupName(grupK);

		int nActive = stagD.aActiveElem.GetSize();
		for ( int k=0; k<nActive; k++ )
		{
			if ( stagD.aActiveElem[k].GrupKey == grupK )
			{
				textD.Initialize();
				textD.strText = strGrupName;
				m_pRptData->AddTextData(textK, textD);
				arActiveK.Add(textK);
			}
		}
		int nDeactive = stagD.aDeactiveElem.GetSize();
		for ( int k=0; k<nDeactive; k++ )
		{
			if ( stagD.aDeactiveElem[k].GrupKey == grupK )
			{
				textD.Initialize();
				textD.strText = strGrupName;
				m_pRptData->AddTextData(textK, textD);
				arDeactiveK.Add(textK);
			}
		}
	}

	if ( arActiveK.GetSize() < 1 )
	{
		textD.Initialize();
		textD.strText = _T("-");
		m_pRptData->AddTextData(textK, textD);
		arActiveK.Add(textK);
	}

	if ( arDeactiveK.GetSize() < 1 )
	{
		textD.Initialize();
		textD.strText = _T("-");
		m_pRptData->AddTextData(textK, textD);
		arDeactiveK.Add(textK);
	}
}


// 해당 stage에서의 Boundary Group List를 얻음.
void CRptJTG_D62_04::GetBoundaryGroupList(T_STAG_K stagK, CArray<T_TEXT_K, T_TEXT_K>& arActiveK, CArray<T_TEXT_K, T_TEXT_K>& arDeactiveK)
{
	arActiveK.RemoveAll();
	arDeactiveK.RemoveAll();

	T_TEXT_K textK;
	T_TEXT_RPT textD;

	T_STAG_D stagD;
	if ( !m_pDoc->m_pAttrCtrl->GetStag(stagK, stagD) ) return;

	CArray<T_BNGR_K, T_BNGR_K> arBngrKeyList; arBngrKeyList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetBngrKeyList(arBngrKeyList);
	int iBngrCount = arBngrKeyList.GetSize();

	for ( int i=0; i<iBngrCount; i++ )
	{
		T_BNGR_K bngrK = arBngrKeyList[i];
		CString strGrupName = m_pDoc->m_pAttrCtrl->GetBngrName(bngrK);

		int nActive = stagD.aActiveBndr.GetSize();
		for ( int k=0; k<nActive; k++ )
		{
			if ( stagD.aActiveBndr[k].BngrKey == bngrK )
			{
				textD.Initialize();
				textD.strText = strGrupName;
				m_pRptData->AddTextData(textK, textD);
				arActiveK.Add(textK);
			}
		}

		int nDeactive = stagD.aDeactiveBndr.GetSize();
		for ( int k=0; k<nDeactive; k++ )
		{
			if ( stagD.aDeactiveBndr[k]== bngrK )
			{
				textD.Initialize();
				textD.strText = strGrupName;
				m_pRptData->AddTextData(textK, textD);
				arDeactiveK.Add(textK);
			}
		}
	}

	if ( arActiveK.GetSize() < 1 )
	{
		textD.Initialize();
		textD.strText = _T("-");
		m_pRptData->AddTextData(textK, textD);
		arActiveK.Add(textK);
	}

	if ( arDeactiveK.GetSize() < 1 )
	{
		textD.Initialize();
		textD.strText = _T("-");
		m_pRptData->AddTextData(textK, textD);
		arDeactiveK.Add(textK);
	}
}

// 해당 stage에서의 Load Group List를 얻음.
void CRptJTG_D62_04::GetLoadGroupList(T_STAG_K stagK, CArray<T_TEXT_K, T_TEXT_K>& arActiveK, CArray<T_TEXT_K, T_TEXT_K>& arDeactiveK)
{
	arActiveK.RemoveAll();
	arDeactiveK.RemoveAll();

	T_TEXT_K textK;
	T_TEXT_RPT textD;

	T_STAG_D stagD;
	if ( !m_pDoc->m_pAttrCtrl->GetStag(stagK, stagD) ) return;

	CArray<T_BNGR_K, T_BNGR_K> arLdgrKeyList; arLdgrKeyList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetLdgrKeyList(arLdgrKeyList);
	int iLdgrCount = arLdgrKeyList.GetSize();

	for ( int i=0; i<iLdgrCount; i++ )
	{
		T_LDGR_K ldgrK = arLdgrKeyList[i];
		CString strGrupName = m_pDoc->m_pAttrCtrl->GetLdgrName(ldgrK);

		int nActive = stagD.aActiveLoad.GetSize();
		for ( int k=0; k<nActive; k++ )
		{
			if ( stagD.aActiveLoad[k].LdgrKey == ldgrK )
			{
				textD.Initialize();
				textD.strText = strGrupName;
				m_pRptData->AddTextData(textK, textD);
				arActiveK.Add(textK);
			}
		}

		int nDeactive = stagD.aDeactiveLoad.GetSize();
		for ( int k=0; k<nDeactive; k++ )
		{
			if ( stagD.aDeactiveLoad[k].LdgrKey == ldgrK )
			{
				textD.Initialize();
				textD.strText = strGrupName;
				m_pRptData->AddTextData(textK, textD);
				arDeactiveK.Add(textK);
			}
		}
	}

	if ( arActiveK.GetSize() < 1 )
	{
		textD.Initialize();
		textD.strText = _T("-");
		m_pRptData->AddTextData(textK, textD);
		arActiveK.Add(textK);
	}

	if ( arDeactiveK.GetSize() < 1 )
	{
		textD.Initialize();
		textD.strText = _T("-");
		m_pRptData->AddTextData(textK, textD);
		arDeactiveK.Add(textK);
	}
}

// 해당 숫자를 중국한자로 바꾸어줌.
//   iNumber : 바꿀 숫자.
CString CRptJTG_D62_04::GetNumberCharacterForChina(int iNumber)
{
	CString result = _T("");
	switch ( iNumber )
	{
	case 0:
		result = _T("쥐");
		break;
	case 1:
		result = _T("寧");
		break;
	case 2:
		result = _T("랗");
		break;
	case 3:
		result = _T("힛");
		break;
	case 4:
		result = _T("愷");
		break;
	case 5:
		result = _T("巧");
		break;
	case 6:
		result = _T("짇");
		break;
	case 7:
		result = _T("펌");
		break;
	case 8:
		result = _T("검");
		break;
	case 9:
		result = _T("씽");
		break;
	default:
		result = _T("-");
		break;
	}
	return result;
}

// 현재 Data 파일의 날짜 표기. (중국Version).
//   iYY : 년도.
//   iMM : 달.
CString CRptJTG_D62_04::GetFileDateForChina(int iYY, int iMM)
{
	TCHAR i_str[32] = _T("");
	wsprintf(i_str, _T("%d"), iYY);

	CString result = _T("");
	CString str = i_str;
	for ( int i=0; i<str.GetLength(); i++ )
	{
		CString strEach = str.GetAt(i);
		int index = _ttoi(strEach);
		result += GetNumberCharacterForChina(index);
	}
	result += _T("쾨 ");

	wsprintf(i_str, _T("%d"), iMM);
	str = i_str;
	for ( int i=0; i<str.GetLength(); i++ )
	{
		CString strEach = str.GetAt(i);
		int index = _ttoi(strEach);
		result += GetNumberCharacterForChina(index);
	}
	result += _T("墩");

	return result;
}

// Tendon의 재료특성치 가져옴.
BOOL CRptJTG_D62_04::GetTendonMatlData(CString strTndnName, double& dEc, double& dDensity, double& dThermal)
{
	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	T_UNIT_INDEX CodeIndex = m_DataCtrl.Get_CodeUnitIndex();
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeIndex);
	//++++++++++++++++++++++++++++++++++++++++++++

	dEc = 0.0;
	dDensity = 0.0;
	dThermal = 0.0;

	T_TDNT_D tdntD;
	if ( !m_pDoc->m_pAttrCtrl->GetTdnt(strTndnName, tdntD) ) return FALSE;
	T_MATL_D matlD;
	if ( !m_pDoc->m_pAttrCtrl->GetMatl(tdntD.MatlKey, matlD) ) return FALSE;

	dEc = matlD.Data1.Analysis.Elast;
	dDensity = matlD.Data1.Analysis.Density * m_dDensitykNM;
	dThermal = matlD.Data1.Analysis.Thermal;

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Set Data by Current Units.
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	//++++++++++++++++++++++++++++++++++++++++++++

	return TRUE;
}

// 활하중 Type의 설명을 가져옴.
CString CRptJTG_D62_04::GetLiveLoadTypeDescription(CString strTypeName)
{
	CString result = _T("");

	int index = -1;

	CString aTypeNames1[] = { _T("CH-CD"), _T("CH-CL"), _T("CH-RQ") };
	CString aTypeDecripts1[] = { _LS(IDS_DGN_PSC_RPTJTG_LiveType_CH_CD),
		_LS(IDS_DGN_PSC_RPTJTG_LiveType_CH_CL),
		_LS(IDS_DGN_PSC_RPTJTG_LiveType_CH_RQ) };

	for ( int i=0; i<3; i++ )
	{
		if ( strTypeName == aTypeNames1[i] )
		{
			index = i;
			break;
		}
	}
	if ( index > -1 && index < 3 ) return aTypeDecripts1[index];

	CString aTypeNames2[] = { _T("QC-10"), _T("QC-15"), _T("QC-20"), _T("QC-C20"), _T("LD-50"), _T("GC-80"), _T("GC-100"), _T("GC-120") };
	CString aTypeDescripts2[] = { _LS(IDS_DGN_PSC_RPTJTG_LiveType_QC_10),
		_LS(IDS_DGN_PSC_RPTJTG_LiveType_QC_15), _LS(IDS_DGN_PSC_RPTJTG_LiveType_QC_20),
		_LS(IDS_DGN_PSC_RPTJTG_LiveType_QC_C20), _LS(IDS_DGN_PSC_RPTJTG_LiveType_LD_50),
		_LS(IDS_DGN_PSC_RPTJTG_LiveType_GC_80), _LS(IDS_DGN_PSC_RPTJTG_LiveType_GC_100),
		_LS(IDS_DGN_PSC_RPTJTG_LiveType_GC_120) };

	for ( int i=0; i<8; i++ )
	{
		if ( strTypeName == aTypeNames2[i] )
		{
			index = i;
			break;
		}
	}
	if ( index > -1 && index < 8 ) return aTypeDescripts2[index];

	CString aTypeNames3[] = { _T("C-AL"), _T("C-BL"), _T("C-AD(20)"), _T("C-BD(20)"), _T("C-AD(150)"), _T("C-BD(15)"), _T("CH-SL"), _T("CH-NL"), _T("CH-HL") };
	CString aTypeDescripts3[] = { _LS(IDS_DGN_PSC_RPTJTG_LiveType_C_AL),
		_LS(IDS_DGN_PSC_RPTJTG_LiveType_C_BL), _LS(IDS_DGN_PSC_RPTJTG_LiveType_C_AD_20),
		_LS(IDS_DGN_PSC_RPTJTG_LiveType_C_BD_20), _LS(IDS_DGN_PSC_RPTJTG_LiveType_C_AD_150),
		_LS(IDS_DGN_PSC_RPTJTG_LiveType_C_BD_150), _LS(IDS_DGN_PSC_RPTJTG_LiveType_CH_SL),
		_LS(IDS_DGN_PSC_RPTJTG_LiveType_CH_NL), _LS(IDS_DGN_PSC_RPTJTG_LiveType_CH_HL) };

	for ( int i=0; i<9; i++ )
	{
		if ( strTypeName == aTypeNames3[i] )
		{
			index = i;
			break;
		}
	}
	if ( index > -1 && index < 8 ) return aTypeDescripts3[index];

	return result;
}

// 하중조합시, 각 하중조합에 쓰이는 기호의 설명.
CString CRptJTG_D62_04::GetLoadCaseDescription(CString strCaseName)
{
	CString result = _T("");
	if ( strCaseName == _T("USER") )  result = _LS(IDS_DBLIB__ADDED__User_Defined_Load);
#if defined(_CIVIL)
	else if ( strCaseName == _T("D") )     result = _LS(IDS_DBLIB__ADDED__Dead_Load);
#else
	else if ( strCaseName == _T("D") )     result = _LS(IDS_DBLIB__ADDED__Dead_Load_Gen);
#endif
	else if ( strCaseName == _T("DC") )    result = _LS(IDS_DBLIB__ADDED__Dead_Load_of_Component_and_Attachment);
	else if ( strCaseName == _T("DW") )    result = _LS(IDS_DBLIB__ADDED__Dead_Load_of_Wearing_Surfaces_and_Utilities);
	else if ( strCaseName == _T("DD") )    result = _LS(IDS_DBLIB__ADDED__Downdrag);
	else if ( strCaseName == _T("LS") )    result = _LS(IDS_DBLIB__ADDED__Live_Load);
	else if ( strCaseName == _T("LC") )    result = _LS(IDS_DBLIB__ADDED__Crawler_Induced_Live_Load);
	else if ( strCaseName == _T("EP") )	  result = _LS(IDS_DBLIB__ADDED__Earth_Pressure);
	else if ( strCaseName == _T("EH") )    result = _LS(IDS_DBLIB__ADDED__Horizontal_Earth_Pressure);
	else if ( strCaseName == _T("EV") )    result = _LS(IDS_DBLIB__ADDED__Vertical_Earth_Pressure);
	else if ( strCaseName == _T("ES") )    result = _LS(IDS_DBLIB__ADDED__Earth_Surcharge_Load);
	else if ( strCaseName == _T("EL") )    result = _LS(IDS_DBLIB__ADDED__Locked_in_Erection_Stresses);
#if defined(_CIVIL)
	else if ( strCaseName == _T("L") )     result = _LS(IDS_DBLIB__ADDED__Live_Load);
#else
	else if ( strCaseName == _T("L") )     result = _LS(IDS_DBLIB__ADDED__Live_Load_Gen);
#endif
	else if ( strCaseName == _T("IL") )    result = _LS(IDS_DBLIB__ADDED__Live_Load_Impact);
	else if ( strCaseName == _T("LC") )    result = _LS(IDS_DBLIB__ADDED__Crawler_Induced_Live_Load);
	else if ( strCaseName == _T("LSC") )   result = _LS(IDS_DBLIB__ADDED__Crawler_Induced_Earth_Pressure);
	else if ( strCaseName == _T("LS") )    result = _LS(IDS_DBLIB__ADDED__Live_Load_Surcharge);
	else if ( strCaseName == _T("LP") )    result = _LS(IDS_DBLIB__ADDED__Live_Load_Overload);
	else if ( strCaseName == _T("ILP") )   result = _LS(IDS_DBLIB__ADDED__Live_Load_Impact_Overload);
	else if ( strCaseName == _T("W") )     result = _LS(IDS_DBLIB__ADDED__Wind_Load_on_Structure);
	else if ( strCaseName == _T("WL") )    result = _LS(IDS_DBLIB__ADDED__Wind_Load_on_Live_Load);
	else if ( strCaseName == _T("BRK") )   result = _LS(IDS_DBLIB__ADDED__Braking_Load);
	else if ( strCaseName == _T("E") )     result = _LS(IDS_DBLIB__ADDED__Earthquake);
	else if ( strCaseName == _T("CF") )    result = _LS(IDS_DBLIB__ADDED__Centrifugal_Force);
	else if ( strCaseName == _T("CO") )    result = _LS(IDS_DBLIB__ADDED__Collision_Load);
	else if ( strCaseName == _T("CT") )    result = _LS(IDS_DBLIB__ADDED__Vehicular_Collision_Force);
	else if ( strCaseName == _T("CV") )    result = _LS(IDS_DBLIB__ADDED__Vessel_Collision_Force);
	else if ( strCaseName == _T("STL") )   result = _LS(IDS_DBLIB__ADDED__Settlement);
	else if ( strCaseName == _T("CR") )    result = _LS(IDS_DBLIB__ADDED__Creep);
	else if ( strCaseName == _T("SH") )    result = _LS(IDS_DBLIB__ADDED__Shrinkage);
	else if ( strCaseName == _T("T") )     result = _LS(IDS_DBLIB__ADDED__Temperature);
	else if ( strCaseName == _T("TPG") )   result = _LS(IDS_DBLIB__ADDED__Temperature_Gradient);
	else if ( strCaseName == _T("B") )     result = _LS(IDS_DBLIB__ADDED__Buoyancy);
	else if ( strCaseName == _T("WP") )    result = _LS(IDS_DBLIB__ADDED__Ground_Water_Pressure);
	else if ( strCaseName == _T("FP") )    result = _LS(IDS_DBLIB__ADDED__Fluid_Pressure);
	else if ( strCaseName == _T("SF") )    result = _LS(IDS_DBLIB__ADDED__Stream_Flow_Pressure);
	else if ( strCaseName == _T("WPR") )   result = _LS(IDS_DBLIB__ADDED__Wave_Pressure);
	else if ( strCaseName == _T("PS") )    result = _LS(IDS_DBLIB__ADDED__Prestress);
	else if ( strCaseName == _T("S") )     result = _LS(IDS_DBLIB__ADDED__Snow_Load);
	else if ( strCaseName == _T("ER") )    result = _LS(IDS_DBLIB__ADDED__Erection_Load);
	else if ( strCaseName == _T("CRL") )   result = _LS(IDS_DBLIB__ADDED__Crowd_Load);
	else if ( strCaseName == _T("IP") )    result = _LS(IDS_DBLIB__ADDED__Ice_Pressure);
	else if ( strCaseName == _T("FR") )    result = _LS(IDS_DBLIB__ADDED__Friction);
	else if ( strCaseName == _T("RS") )    result = _LS(IDS_DBLIB__ADDED__Rib_Shortening);
	else if ( strCaseName == _T("BK") )    result = _LS(IDS_DBLIB__ADDED__Longitudinal_Force_from_Live_Load);
	else if ( strCaseName == _T("GE") )    result = _LS(IDS_DBLIB__ADDED__Grade_Effect);
	else if ( strCaseName == _T("LF") )    result = _LS(IDS_DBLIB__ADDED__LONGI_FORCE);
	else if ( strCaseName == _T("RF") )    result = _LS(IDS_DBLIB__ADDED__RAKING_FORCE);
	else                            result = _T("Not Defined");

	return result;
}



// 하중조합의 기호 설명.
//   mapLcom : 하중조합 기호 및 설명을 담고 있는 Map.
void CRptJTG_D62_04::GetLoadCombinationCases(CMap<CString, LPCTSTR, CString, CString&>& mapLcom)
{
	CString strDescript = _T("");
	CString strText     = _T("");

	// D_LCOM_STATIC : Static Load Case.
	CArray<T_STLD_K, T_STLD_K> arStldKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKeyList);

	int iStld = arStldKeyList.GetSize();
	for ( int i=0; i<iStld; i++ )
	{
		T_STLD_D stld;
		if ( m_pDoc->m_pAttrCtrl->GetStld(arStldKeyList[i], stld) && stld.LoadCaseType != _LSX(CS) )
		{
			mapLcom.SetAt(stld.LoadCaseType, stld.LoadCaseName);
		}
	}

	//D_LCOM_SPECTRUM	: Response Spectrum Case.
	CArray<T_SPLC_K, T_SPLC_K> arSplcKeyList;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(arSplcKeyList);

	int iSplc = arSplcKeyList.GetSize();
	for ( int i=0; i<iSplc; i++ )
	{
		T_SPLC_D splcD;
		if ( m_pDoc->m_pAttrCtrl->GetSplc(arSplcKeyList[i], splcD) )
		{
			strText.Format(_T("RS[%d]"), splcD.LoadCaseId);
			mapLcom.SetAt(strText, splcD.LoadCaseName);
		}
	}

	//D_LCOM_HISTORY : Time Histroy Analysis.  
	CArray<T_THIS_K, T_THIS_K> arThisKeyList;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(arThisKeyList);

	int iThis = arThisKeyList.GetSize();
	for ( int i=0; i<iThis; i++ )
	{
		T_THIS_D thisD;
		if ( m_pDoc->m_pAttrCtrl->GetThis(arThisKeyList[i], thisD) )
		{
			strText.Format(_T("TH[%d]"), thisD.LoadCaseId);
			mapLcom.SetAt(strText, thisD.LoadCaseName);
		}
	}

	//D_LCOM_MOVING : Moving Load Anlaysis.
	T_MVCD_D mvcdD;
	if ( !m_pDoc->m_pAttrCtrl->GetMvcd(mvcdD) ) mvcdD.Initialize();
	switch ( mvcdD.nCodeType )
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
		{
			CArray<T_MVLD_K, T_MVLD_K> arMvldKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMvldKeyList);

			int iMvld = arMvldKeyList.GetSize();
			for ( int i=0; i<iMvld; i++ )
			{
				T_MVLD_D mvldD;
				if ( m_pDoc->m_pAttrCtrl->GetMvld(arMvldKeyList[i], mvldD) )
				{
					strText.Format(_T("M[%d]"), mvldD.LoadCaseId);
					mapLcom.SetAt(strText, mvldD.LoadCaseName);
				}
			}
		}
		break;
	case D_MOVE_CODE_CHINA:
		{
			CArray<T_MVLDch_K, T_MVLDch_K> arMvldchKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldchKeyList(arMvldchKeyList);

			int iMvldch = arMvldchKeyList.GetSize();
			for ( int i=0; i<iMvldch; i++ )
			{
				T_MVLDch_D mvldchD;
				if ( m_pDoc->m_pAttrCtrl->GetMvldch(arMvldchKeyList[i], mvldchD) )
				{
					strText.Format(_T("M[%d]"), mvldchD.LoadCaseId);
					mapLcom.SetAt(strText, mvldchD.LoadCaseName);
				}
			}
		}
		break;
	case D_MOVE_CODE_JAPAN:
		{
			CArray<T_MVLDjp_K, T_MVLDjp_K> arMvldjpKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(arMvldjpKeyList);

			int iMvldjp = arMvldjpKeyList.GetSize();
			for ( int i=0; i<iMvldjp; i++ )
			{
				T_MVLDjp_D mvldjpD;
				if ( m_pDoc->m_pAttrCtrl->GetMvldjp(arMvldjpKeyList[i], mvldjpD) )
				{
					strText.Format(_T("M[%d]"), mvldjpD.LoadCaseId);
					mapLcom.SetAt(strText, mvldjpD.LoadCaseName);
				}
			}
		}
		break;
	case D_MOVE_CODE_INDIA:
		{
			CArray<T_MVLDid_K, T_MVLDid_K> arMvldidKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arMvldidKeyList);

			int iMvldid = arMvldidKeyList.GetSize();
			for ( int i=0; i<iMvldid; i++ )
			{
				T_MVLDid_D mvldidD;
				if ( m_pDoc->m_pAttrCtrl->GetMvldid(arMvldidKeyList[i], mvldidD) )
				{
					strText.Format(_T("M[%d]"), mvldidD.LoadCaseId);
					mapLcom.SetAt(strText, mvldidD.LoadCaseName);
				}
			}
		}
		break;
	case D_MOVE_CODE_BS:
		{
			CArray<T_MVLDbs_K, T_MVLDbs_K> arMvldbsKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(arMvldbsKeyList);

			int iMvldbs = arMvldbsKeyList.GetSize();
			for ( int i=0; i<iMvldbs; i++ )
			{
				T_MVLDbs_D mvldbsD;
				if ( m_pDoc->m_pAttrCtrl->GetMvldbs(arMvldbsKeyList[i], mvldbsD) )
				{
					strText.Format(_T("M[%d]"), mvldbsD.LoadCaseId);
					mapLcom.SetAt(strText, mvldbsD.LoadCaseName);
				}
			}
		}
		break;
	case D_MOVE_CODE_FRANCE:
		{
			CArray<T_MVLDfr_K, T_MVLDfr_K> arMvldfrKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(arMvldfrKeyList);

			int iMvldfr = arMvldfrKeyList.GetSize();
			for ( int i=0; i<iMvldfr; i++ )
			{
				T_MVLDfr_D mvldfrD;
				if ( m_pDoc->m_pAttrCtrl->GetMvldfr(arMvldfrKeyList[i], mvldfrD) )
				{
					strText.Format(_T("M[%d]"), mvldfrD.LoadCaseId);
					mapLcom.SetAt(strText, mvldfrD.LoadCaseName);
				}
			}
		}
		break;
	case D_MOVE_CODE_TRANS:
		{
			CArray<T_MVLDtr_K, T_MVLDtr_K> arMvldtrKeyList;
			m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(arMvldtrKeyList);

			int iMvldtr = arMvldtrKeyList.GetSize();
			for ( int i=0; i<iMvldtr; i++ )
			{
				T_MVLDtr_D mvldtrD;
				if ( m_pDoc->m_pAttrCtrl->GetMvldtr(arMvldtrKeyList[i], mvldtrD) )
				{
					strText.Format(_T("M[%d]"), mvldtrD.LoadCaseId);
					mapLcom.SetAt(strText, mvldtrD.LoadCaseName);
				}
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	//D_LCOM_SETTLEMENT	: Settlement Displacement.
	CArray<T_SMLC_K, T_SMLC_K> arSmlcKeyList;
	m_pDoc->m_pAttrCtrl->GetSmlcKeyList(arSmlcKeyList);

	int iSmlc = arSmlcKeyList.GetSize();
	for ( int i=0; i<iSmlc; i++ )
	{
		T_SMLC_D smlcD;
		if ( m_pDoc->m_pAttrCtrl->GetSmlc(arSmlcKeyList[i], smlcD) )
		{
			mapLcom.SetAt(_T("SM"), smlcD.LoadCaseName);
		}
	}

	//D_LCOM_STAGE : Construction Stage Analysis.
	CArray<T_SGLD_K, T_SGLD_K> arSgldKeyList;
	m_pDoc->m_pAttrCtrl->GetSgldKeyList(arSgldKeyList);

	int iSgld = arSgldKeyList.GetSize();
	for ( int i=0; i<iSgld; i++ )
	{
		T_SGLD_D sgldD;
		if ( m_pDoc->m_pAttrCtrl->GetSgld(arSgldKeyList[i], sgldD) )
		{
			CString strText = GetConstructionStageLoadCaseSymbol(arSgldKeyList[i]);
			mapLcom.SetAt(strText, sgldD.LoadCaseName + _LSX((CS)));
		}
	}

	//D_LCOM_ECCENSPEC : 우발편심 응답 스펙트럼 하중 조건
	CArray<T_ESPC_K, T_ESPC_K> arEspcKeyList;
	m_pDoc->m_pAttrCtrl->GetEspcKeyList(arEspcKeyList);

	int iEspc = arEspcKeyList.GetSize();
	for ( int i=0; i<iEspc; i++ )
	{
		T_ESPC_D espcD;
		if ( m_pDoc->m_pAttrCtrl->GetEspc(arEspcKeyList[i], espcD) )
		{
			mapLcom.SetAt(espcD.LoadCaseName, espcD.LoadCaseName + _LSX((ES)));
		}
	}

	//D_LCOM_GRIDMODEL  : 격자모델 하중 조건    
	CArray<T_GILC_K, T_GILC_K> arGilcKeyList;
	m_pDoc->m_pAttrCtrl->GetGilcKeyList(arGilcKeyList);

	int iGlic = arGilcKeyList.GetSize();
	for ( int i=0; i<iGlic; i++ )
	{
		T_GILC_D gilcD;
		if ( m_pDoc->m_pAttrCtrl->GetGilc(arGilcKeyList[i], gilcD) )
		{
			mapLcom.SetAt(gilcD.LoadCaseName, gilcD.LoadCaseName + _LSX((GI)));
		}
	}
}

// 시공단계 하중 Case의 기호 얻어옴.
CString CRptJTG_D62_04::GetConstructionStageLoadCaseSymbol(T_SGLD_K sgldK)
{
	CString aSymbols[] = { _T("cD"), _T("cEL1"), _T("cEL2"), _T("cEL3"), _T("cEL4"), _T("cEL5"), _T("cEL6"), _T("cEL7"), _T("cEL8"), _T("cEL9"), _T("cEL10"), _T("cEL11"), _T("cEL12"), _T("cEL13"), _T("cEL14"), _T("cEL15"), _T("cEL16"), _T("cEL17"), _T("cEL18"), _T("cEL19"), _T("cEL20"),
					  _T("cTP"), _T("cTS"), _T("cCR"), _T("cCR"), _T("cSH"), _T("cSH"), _T("SUM") };
	return (sgldK >= D_SGLD_DL_CVL && sgldK <= D_SGLD_SUM_CVL)? aSymbols[sgldK-1] : _T("");
}

// 온도하중 조건이 존재하는가..  
BOOL CRptJTG_D62_04::CheckExistTemperatureLoad()
{
	int iTempCount = 0;
	CArray<T_STLD_K, T_STLD_K> arStldKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKeyList);

	// System Temperature.
	T_STMP_K stmpK;
	T_STMP_D stmpD;
	int iCount = arStldKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		if ( m_pDoc->m_pAttrCtrl->GetStmp(arStldKeyList[i], stmpK, stmpD) )
		{
			iTempCount += 1;
		}
	}

	// Nodal Temperature.
	CArray<T_NODE_K, T_NODE_K> arNodeKeyList;
	m_pDoc->m_pAttrCtrl->GetNodeKeyList(arNodeKeyList);

	iCount = arNodeKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		CArray<T_NTMP_K, T_NTMP_K> arNtmpKeyList;
		CArray<T_NTMP_D, T_NTMP_D&> arNtmpDataList;

		m_pDoc->m_pAttrCtrl->GetNtmp(arNodeKeyList[i], arNtmpKeyList, arNtmpDataList);
		iTempCount += arNtmpKeyList.GetSize();
	}

	// Element Temperature.
	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);

	iCount = arElemKeyList.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		CArray<T_ETMP_K, T_ETMP_K> arEtmpKeyList;
		CArray<T_ETMP_D, T_ETMP_D&> arEtmpDataList;

		m_pDoc->m_pAttrCtrl->GetEtmp(arElemKeyList[i], arEtmpKeyList, arEtmpDataList);
		iTempCount += arEtmpKeyList.GetSize();

		CArray<T_GTMP_K, T_GTMP_K> arGtmpKeyList;
		CArray<T_GTMP_D, T_GTMP_D&> arGtmpDataList;

		m_pDoc->m_pAttrCtrl->GetGtmp(arElemKeyList[i], arGtmpKeyList, arGtmpDataList);
		iTempCount += arGtmpKeyList.GetSize();

		CArray<T_BTMP_K, T_BTMP_K> arBtmpKeyList;
		CArray<T_BTMP_D, T_BTMP_D&> arBtmpDataList;

		m_pDoc->m_pAttrCtrl->GetBtmp(arElemKeyList[i], arBtmpKeyList, arBtmpDataList);
		iTempCount += arBtmpKeyList.GetSize();
	}

	return (iTempCount > 0)? TRUE : FALSE;
}


BOOL CRptJTG_D62_04::CheckExistResultTableItem()
{
	int iTableCount = 0;

	// PSC Design Parameter.
	T_PSCD_D pscdD;
	if ( !m_pDoc->m_pAttrCtrl->GetDgnPscd(pscdD) ) pscdD.Initialize();

	for ( int i=0; i<6; i++ )
	{
		if ( pscdD.bMomPrint[i] ) iTableCount += 1;
	}

	for ( int i=0; i<4; i++ )
	{
		if ( pscdD.bShrPrint[i] ) iTableCount += 1;
	}

	return (iTableCount > 0)? TRUE : FALSE;
}


//***************************************************************
// Some Helper Functions.
//***************************************************************

// T_CHAP_RPT Data 생성.
T_CHAP_RPT CRptJTG_D62_04::MakeChapter(int iLevel, int iChapID, CString strText, T_CSS_K cssK/*0*/)
{
	T_CHAP_RPT chapD;
	chapD.Initialize();
	chapD.nLevel = iLevel;
	chapD.SeqID  = iChapID;
	chapD.TitlK  = AddTitle(strText, iLevel, cssK);
	return chapD;
}

BOOL CRptJTG_D62_04::GetDesignStringPara(CStringArray& arrString)
{
	CString strText;
	// 설계단면력 성분.  
	if ( m_PscdD.iDimType==0 )
	{
		strText = _LS(IDS_DGN_PSC_RPTJTG_Two_Dimension); // _T("2차원")
		arrString.Add(strText);
	}
	else if ( m_PscdD.iDimType==1 )
	{
		strText = _LS(IDS_DGN_PSC_RPTJTG_Two_Dimension_Torque); // _T("2차원+토크")
		arrString.Add(strText);
	}
	else if ( m_PscdD.iDimType==2 )
	{
		strText = _LS(IDS_DGN_PSC_RPTJTG_Three_Dimension); // _T("3차원")
		arrString.Add(strText);
	}
	else ASSERT(0);

	// 교종
	if ( m_PscdD.iBrgType==0 )
	{
		strText = _LS(IDS_DGN_PSC_RPTJTG_Fully_PSC); // _T("Fully PSC")
		arrString.Add(strText);
	}
	else if ( m_PscdD.iBrgType==1 )
	{
		strText = _LS(IDS_DGN_PSC_RPTJTG_Partially_PSC_A); // _T("Partially PSC(A)")
		arrString.Add(strText);
	}
	else if ( m_PscdD.iBrgType==2 )
	{
		strText = _LS(IDS_DGN_PSC_RPTJTG_Partially_PSC_B);  // _T("Partially PSC(B)")
		arrString.Add(strText);
	}
	else ASSERT(0);

	// 도로교함 안전등급

	if ( m_PscdD.iDgnCode == CJJ_11_2011 && m_PscdD.bExistSpecialLoad )
	{
		if ( m_PscdD.iSafeType==0 )
		{
			strText = _LS(IDS_DGN_DESIGN_PARM_SPECIALLOAD1); // _T("1급")
			arrString.Add(strText);
		}
		else if ( m_PscdD.iSafeType==1 )
		{
			strText = _LS(IDS_DGN_DESIGN_PARM_SPECIALLOAD2); //_T("2급")
			arrString.Add(strText);
		}
		else if ( m_PscdD.iSafeType==2 )
		{
			strText = _LS(IDS_DGN_DESIGN_PARM_SPECIALLOAD3); // _T("3급")
			arrString.Add(strText);
		}
		else ASSERT(0);
	}
	else
	{
		if ( m_PscdD.iSafeType==0 )
		{
			strText = _LS(IDS_DGN_PSC_RPTJTG_1Grade); // _T("1급")
			arrString.Add(strText);
		}
		else if ( m_PscdD.iSafeType==1 )
		{
			strText = _LS(IDS_DGN_PSC_RPTJTG_2Grade); //_T("2급")
			arrString.Add(strText);
		}
		else if ( m_PscdD.iSafeType==2 )
		{
			strText = _LS(IDS_DGN_PSC_RPTJTG_3Grade); // _T("3급")
			arrString.Add(strText);
		}
		else ASSERT(0);
	}


	// 타설방식
	if ( m_PscdD.iCastType==0 )
	{
		strText = _LS(IDS_DGN_PSC_RPTJTG_Precast); // _T("Precast")
		arrString.Add(strText);
	}
	else if ( m_PscdD.iCastType==1 )
	{
		strText = _LS(IDS_DGN_PSC_RPTJTG_Cast_in_Place); // _T("Cast-in-place")
		arrString.Add(strText);
	}
	else ASSERT(0);

	return TRUE;
}

BOOL CRptJTG_D62_04::GetMaterialProperty()
{
	if ( m_arElemK.GetSize()==0 ) return FALSE; // m_arElemK는 PSC 설계 대상 요소만. 
	m_arMatElem.RemoveAll();

	// Concrete, Rebar
	CString strHeadName;
	T_MATD_D MatdD;
	std::map<CString, T_ELEM_K> mapMatl2Elem;
	for ( int i=0; i<m_arElemK.GetSize(); i++ )
	{
		auto ElemK = m_arElemK.GetAt(i);		
		MatdD.Initialize();
		if ( !m_DataCtrl.Get_ConMatd(ElemK, MatdD) )	continue;

		mapMatl2Elem[MatdD.Name] = ElemK.first;
	}

	for ( std::map<CString, T_ELEM_K>::iterator pit = mapMatl2Elem.begin(); pit != mapMatl2Elem.end(); ++pit )
	{
		m_arMatElem.Add(pit->second);
	}

	m_arTdnaK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetTdnaKeyList(m_arTdnaK);

	m_arTdntK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetTdntKeyList(m_arTdntK);

	return TRUE;
}

CString CRptJTG_D62_04::GetModifyString(int iMaxLength, CString strText)
{
	CString str = strText;
	int iCurrLength = strText.GetLength();
	for ( int i=0; i<iMaxLength-iCurrLength; i++ )
		str += _T("　");

	return str;
}

T_TD_K CRptJTG_D62_04::AddTDwithUnit(CArray<T_CHAR_K, T_CHAR_K>& arCharK, CString strUnit, int iTDType/*0*/, int iMergeCols/*1*/, int iMergeRows/*1*/, T_CSS_K CssK/*0*/)
{
	CArray<T_TEXT_K, T_TEXT_K> arTextK; arTextK.RemoveAll();

	T_TEXT_K TextK;
	T_TEXT_RPT TextD; TextD.Initialize();
	for ( int i=0; i<arCharK.GetSize(); i++ ) TextD.arCharK.Add(arCharK.GetAt(i));
	m_pRptData->AddTextData(TextK, TextD);
	arTextK.Add(TextK);

	TextD.Initialize();
	TextD.strText = strUnit;
	m_pRptData->AddTextData(TextK, TextD);
	arTextK.Add(TextK);

	return AddTD(arTextK, iTDType, iMergeCols, iMergeRows, CssK);
}

T_TD_K CRptJTG_D62_04::AddTDwithUnit(CArray<T_CHAR_K, T_CHAR_K>& arCharK, CArray<T_CHAR_K, T_CHAR_K>& arCharUnitK, int iTDType/*0*/, int iMergeCols/*1*/, int iMergeRows/*1*/, T_CSS_K CssK/*0*/)
{
	CArray<T_TEXT_K, T_TEXT_K> arTextK; arTextK.RemoveAll();

	T_TEXT_K TextK;
	T_TEXT_RPT TextD; TextD.Initialize();
	for ( int i=0; i<arCharK.GetSize(); i++ ) TextD.arCharK.Add(arCharK.GetAt(i));
	m_pRptData->AddTextData(TextK, TextD);
	arTextK.Add(TextK);

	TextD.Initialize();
	for ( int i=0; i<arCharUnitK.GetSize(); i++ ) TextD.arCharK.Add(arCharUnitK.GetAt(i));
	m_pRptData->AddTextData(TextK, TextD);
	arTextK.Add(TextK);

	return AddTD(arTextK, iTDType, iMergeCols, iMergeRows, CssK);
}

T_TD_K CRptJTG_D62_04::AddTDwithUnit(LPCTSTR strText, CString strUnit, int iTDType/*0*/, int iMergeCols/*1*/, int iMergeRows/*1*/, T_CSS_K CssK/*0*/)
{
	CArray<T_TEXT_K, T_TEXT_K> arTextK; arTextK.RemoveAll();

	T_TEXT_K TextK;
	T_TEXT_RPT TextD; TextD.Initialize();
	TextD.strText = strText;
	m_pRptData->AddTextData(TextK, TextD);
	arTextK.Add(TextK);

	TextD.Initialize();
	TextD.strText = strUnit;
	m_pRptData->AddTextData(TextK, TextD);
	arTextK.Add(TextK);

	return AddTD(arTextK, iTDType, iMergeCols, iMergeRows, CssK);
}

T_TD_K CRptJTG_D62_04::AddTDwithUnit(LPCTSTR strText, CArray<T_CHAR_K, T_CHAR_K>& arCharUnitK, int iTDType/*0*/, int iMergeCols/*1*/, int iMergeRows/*1*/, T_CSS_K CssK/*0*/)
{
	CArray<T_TEXT_K, T_TEXT_K> arTextK; arTextK.RemoveAll();

	T_TEXT_K TextK;
	T_TEXT_RPT TextD; TextD.Initialize();
	TextD.strText = strText;
	m_pRptData->AddTextData(TextK, TextD);
	arTextK.Add(TextK);

	TextD.Initialize();
	for ( int i=0; i<arCharUnitK.GetSize(); i++ ) TextD.arCharK.Add(arCharUnitK.GetAt(i));
	m_pRptData->AddTextData(TextK, TextD);
	arTextK.Add(TextK);

	return AddTD(arTextK, iTDType, iMergeCols, iMergeRows, CssK);
}

void CRptJTG_D62_04::InitialarCharK()
{
	CString strText;

	m_arCharK_TndnComp.RemoveAll();
	m_arCharK_MnConR.RemoveAll();
	m_arCharK_MnConT.RemoveAll();
	m_arCharK_MnStl.RemoveAll();
	m_arCharK_MnPs.RemoveAll();

	m_arCharK_MnStlT.RemoveAll();
	m_arCharK_MnPsT.RemoveAll();
	m_arCharK_MnStlT2.RemoveAll();
	m_arCharK_MnPsT2.RemoveAll();
	m_arCharK_MnPs2.RemoveAll();

	// f'pd-σ'p0
	strText = _T("f'pd - ");
	AddChar(m_arCharK_TndnComp, strText, 2, 2, 2);
	strText = _T("σ'p0");
	AddChar(m_arCharK_TndnComp, strText, 2, 3, 2);

	// Mn_Con Rect
	strText = _T("fcdbx(");
	AddChar(m_arCharK_MnConR, strText, 2, 1, 2);
	strText = _T("h0 - x/2)");
	AddChar(m_arCharK_MnConR, strText, 2, 1, 1);

	// Mn_Con T
	strText = _T("fcd");
	AddChar(m_arCharK_MnConT, strText, 2, 1, 2);
	strText = _T("[bx(h0 - x/2) + (");
	AddChar(m_arCharK_MnConT, strText, 2, 5, 1);
	strText = _T("b'f - b)");
	AddChar(m_arCharK_MnConT, strText, 2, 2, 1);
	strText = _T("h'f(");
	AddChar(m_arCharK_MnConT, strText, 2, 2, 1);
	strText = _T("h0 - ");
	AddChar(m_arCharK_MnConT, strText, 2, 1, 1);
	strText = _T("h'f/2 )]");
	AddChar(m_arCharK_MnConT, strText, 2, 2, 1);

	// Mn_stl
	strText = _T("f'sd");
	AddChar(m_arCharK_MnStl, strText, 2, 2, 2);
	strText = _T("A's");
	AddChar(m_arCharK_MnStl, strText, 2, 2, 1);
	strText = _T("(h0 - ");
	AddChar(m_arCharK_MnStl, strText, 2, 2, 1);
	strText = _T("a's)");
	AddChar(m_arCharK_MnStl, strText, 2, 2, 1);

	// Mn_ps
	strText = _T("(f'pd - ");
	AddChar(m_arCharK_MnPs, strText, 2, 3, 2);
	strText = _T("σ'p0)");
	AddChar(m_arCharK_MnPs, strText, 2, 3, 2);
	strText = _T("A'p ");
	AddChar(m_arCharK_MnPs, strText, 2, 2, 1);
	strText = _T("(h0 - ");
	AddChar(m_arCharK_MnPs, strText, 2, 2, 1);
	strText = _T("a'p)");
	AddChar(m_arCharK_MnPs, strText, 2, 2, 1);

	// Mn_psT
	strText = _T("fpd");
	AddChar(m_arCharK_MnPsT, strText, 2, 1, 2);
	strText = _T("Ap");
	AddChar(m_arCharK_MnPsT, strText, 2, 1, 1);
	strText = _T("(h - ap - a')");
	AddChar(m_arCharK_MnPsT, strText, 2, 6, 1);

	// Mn_stlT
	strText = _T("fsd");
	AddChar(m_arCharK_MnStlT, strText, 2, 1, 2);
	strText = _T("As");
	AddChar(m_arCharK_MnStlT, strText, 2, 1, 1);
	strText = _T("(h - as - a'");
	AddChar(m_arCharK_MnStlT, strText, 2, 6, 1);

	// Mn_psT2
	strText = _T("fpd");
	AddChar(m_arCharK_MnPsT2, strText, 2, 1, 2);
	strText = _T("Ap");
	AddChar(m_arCharK_MnPsT2, strText, 2, 1, 1);
	strText = _T("(h - ap - ");
	AddChar(m_arCharK_MnPsT2, strText, 2, 6, 1);
	strText = _T("a's)");
	AddChar(m_arCharK_MnPsT2, strText, 2, 2, 1);

	// Mn_stlT2
	strText = _T("fsd");
	AddChar(m_arCharK_MnStlT2, strText, 2, 1, 2);
	strText = _T("As");
	AddChar(m_arCharK_MnStlT2, strText, 2, 1, 1);
	strText = _T("(h - as - ");
	AddChar(m_arCharK_MnStlT2, strText, 2, 6, 1);
	strText = _T("a's");
	AddChar(m_arCharK_MnStlT2, strText, 2, 2, 1);

	// Mn_ps2
	strText = _T("(f'pd - ");
	AddChar(m_arCharK_MnPs2, strText, 2, 3, 2);
	strText = _T("σ'p0)");
	AddChar(m_arCharK_MnPs2, strText, 2, 3, 2);
	strText = _T("A'p ");
	AddChar(m_arCharK_MnPs2, strText, 2, 2, 1);
	strText = _T("(a'p - ");
	AddChar(m_arCharK_MnPs2, strText, 2, 3, 1);
	strText = _T("a's)");
	AddChar(m_arCharK_MnPs2, strText, 2, 2, 1);

	// Vn
	m_arCharK_Vn.RemoveAll();
	strText = _T("Vcs + ");
	AddChar(m_arCharK_Vn, strText, 2, 1, 2);
	strText = _T("Vsb + ");
	AddChar(m_arCharK_Vn, strText, 2, 1, 2);
	strText = _T("Vpb");
	AddChar(m_arCharK_Vn, strText, 2, 1, 2);

	m_arCharK_Vcs.RemoveAll();
	// Vcs
	strText = _T("Vcs = ");
	AddChar(m_arCharK_Vcs, strText, 2, 1, 2);
	strText = _T("α1");
	AddChar(m_arCharK_Vcs, strText, 2, 2, 1);
	strText = _T("α2");
	AddChar(m_arCharK_Vcs, strText, 2, 2, 1);
	strText = _T("α3");
	AddChar(m_arCharK_Vcs, strText, 2, 2, 1);
	strText = _T("(0.45×10^ -3)");
	m_arCharK_Vcs.Add(AddChar(strText));
	strText = _T("bh0");
	AddChar(m_arCharK_Vcs, strText, 2, 2, 1);
	strText = _T("√((2+0.6P)√(");
	m_arCharK_Vcs.Add(AddChar(strText));
	strText = _T("fcu,k)");
	AddChar(m_arCharK_Vcs, strText, 2, 1, 4);
	strText = _T("ρsv");
	AddChar(m_arCharK_Vcs, strText, 2, 2, 2);
	strText = _T("fsv)");
	AddChar(m_arCharK_Vcs, strText, 2, 1, 2);

	m_arCharK_Vsb.RemoveAll();
	// Vsb
	strText = _T("Vsb = 0.75×");
	AddChar(m_arCharK_Vsb, strText, 2, 1, 2);
	strText = _T("10-3");
	AddChar(m_arCharK_Vsb, strText, 1, 2, 2);
	strText = _T("fsdΣ");
	AddChar(m_arCharK_Vsb, strText, 2, 1, 2);
	strText = _T("Asb");
	AddChar(m_arCharK_Vsb, strText, 2, 1, 2);
	strText = _T("SinΘs");
	AddChar(m_arCharK_Vsb, strText, 2, 5, 1);

	m_arCharK_Vpb.RemoveAll();
	// Vpb
	strText = _T("Vpb = 0.75×");
	AddChar(m_arCharK_Vpb, strText, 2, 1, 2);
	strText = _T("10-3");
	AddChar(m_arCharK_Vpb, strText, 1, 2, 2);
	strText = _T("fpdΣ");
	AddChar(m_arCharK_Vpb, strText, 2, 1, 2);
	strText = _T("Apb");
	AddChar(m_arCharK_Vpb, strText, 2, 1, 2);
	strText = _T("SinΘp");
	AddChar(m_arCharK_Vpb, strText, 2, 5, 1);

	// Sect Chk
	m_arCharK_SectChk.RemoveAll();
	strText = _T("0.51×");
	m_arCharK_SectChk.Add(AddChar(strText));
	strText = _T("10-3√(");
	AddChar(m_arCharK_SectChk, strText, 1, 2, 2);
	strText = _T("fcu,k)");
	AddChar(m_arCharK_SectChk, strText, 2, 1, 4);
	strText =_T("bh0");
	AddChar(m_arCharK_SectChk, strText, 2, 2, 1);

	// Load Chk
	m_arCharK_LoadChk.RemoveAll();
	strText = _T("0.50×");
	m_arCharK_LoadChk.Add(AddChar(strText));
	strText = _T("10-3");
	AddChar(m_arCharK_LoadChk, strText, 1, 2, 2);
	strText = _T("α2");
	AddChar(m_arCharK_LoadChk, strText, 2, 2, 1);
	strText = _T("ftd");
	AddChar(m_arCharK_LoadChk, strText, 2, 1, 2);
	strText =_T("bh0");
	AddChar(m_arCharK_LoadChk, strText, 2, 2, 1);

	// Pure Tn1
	m_arCharK_PureTn1.RemoveAll();
	strText = _T("0.35");
	m_arCharK_PureTn1.Add(AddChar(strText));
	strText = _T("βa");
	AddChar(m_arCharK_PureTn1, strText, 2, 2, 1);
	strText = _T("ftd");
	AddChar(m_arCharK_PureTn1, strText, 2, 1, 2);
	strText = _T("Wt + 1.2√(ζ)");
	m_arCharK_PureTn1.Add(AddChar(strText));
	strText = _T("fsv");
	AddChar(m_arCharK_PureTn1, strText, 2, 1, 2);
	strText =_T("Asv1");
	AddChar(m_arCharK_PureTn1, strText, 2, 1, 3);
	strText =_T("Acor/");
	AddChar(m_arCharK_PureTn1, strText, 2, 1, 3);
	strText =_T("Sv");
	AddChar(m_arCharK_PureTn1, strText, 2, 1, 1);

	// Pure Tn2
	m_arCharK_PureTn2.RemoveAll();
	strText = _T("0.35");
	m_arCharK_PureTn2.Add(AddChar(strText));
	strText = _T("βa");
	AddChar(m_arCharK_PureTn2, strText, 2, 2, 1);
	strText = _T("ftd");
	AddChar(m_arCharK_PureTn2, strText, 2, 1, 2);
	strText = _T("Wt + 1.2√(ζ)");
	m_arCharK_PureTn2.Add(AddChar(strText));
	strText = _T("fsv");
	AddChar(m_arCharK_PureTn2, strText, 2, 1, 2);
	strText =_T("Asv1");
	AddChar(m_arCharK_PureTn2, strText, 2, 1, 3);
	strText =_T("Acor/");
	AddChar(m_arCharK_PureTn2, strText, 2, 1, 3);
	strText =_T("Sv + 0.05(");
	AddChar(m_arCharK_PureTn2, strText, 2, 1, 1);
	strText = _T("Np0/");
	AddChar(m_arCharK_PureTn2, strText, 2, 1, 2);
	strText = _T("A0)");
	AddChar(m_arCharK_PureTn2, strText, 2, 1, 1);
	strText = _T("Wt");
	AddChar(m_arCharK_PureTn2, strText, 2, 1, 1);

	// Zeta
	m_arCharK_Zeta.RemoveAll();
	strText = _T("ζ = ");
	m_arCharK_Zeta.Add(AddChar(strText));
	strText = _T("fsd");
	AddChar(m_arCharK_Zeta, strText, 2, 1, 2);
	strText =_T("Ast");
	AddChar(m_arCharK_Zeta, strText, 2, 1, 2);
	strText =_T("Sv/(");
	AddChar(m_arCharK_Zeta, strText, 2, 1, 1);
	strText = _T("fsv");
	AddChar(m_arCharK_Zeta, strText, 2, 1, 2);
	strText =_T("Asv1");
	AddChar(m_arCharK_Zeta, strText, 2, 1, 3);
	strText =_T("Ucor)");
	AddChar(m_arCharK_Zeta, strText, 2, 1, 3);

	// VTLoad
	m_arCharK_VTLoad.RemoveAll();
	strText = _T("γ0");
	AddChar(m_arCharK_VTLoad, strText, 2, 2, 1);
	strText =_T("Vd/(b");
	AddChar(m_arCharK_VTLoad, strText, 2, 1, 1);
	strText =_T("h0) + ");
	AddChar(m_arCharK_VTLoad, strText, 2, 1, 1);
	strText = _T("γ0");
	AddChar(m_arCharK_VTLoad, strText, 2, 2, 1);
	strText =_T("Td/(");
	AddChar(m_arCharK_VTLoad, strText, 2, 1, 1);
	strText =_T("Wt)");
	AddChar(m_arCharK_VTLoad, strText, 2, 1, 1);

	// TorSect Chk
	m_arCharK_TorSectChk.RemoveAll();
	strText = _T("0.51×");
	m_arCharK_TorSectChk.Add(AddChar(strText));
	strText = _T("10-3√(");
	AddChar(m_arCharK_TorSectChk, strText, 1, 2, 2);
	strText = _T("fcu,k)");
	AddChar(m_arCharK_TorSectChk, strText, 2, 1, 4);

	// TorLoad Chk
	m_arCharK_TorLoadChk.RemoveAll();
	strText = _T("0.50×");
	m_arCharK_TorLoadChk.Add(AddChar(strText));
	strText = _T("10-3");
	AddChar(m_arCharK_TorLoadChk, strText, 1, 2, 2);
	strText = _T("α2");
	AddChar(m_arCharK_TorLoadChk, strText, 2, 2, 1);
	strText = _T("ftd");
	AddChar(m_arCharK_TorLoadChk, strText, 2, 1, 2);

	// T_Vn
	m_arCharK_TorVn.RemoveAll();
	strText = _T("α1");
	AddChar(m_arCharK_TorVn, strText, 2, 2, 1);
	strText = _T("α2");
	AddChar(m_arCharK_TorVn, strText, 2, 2, 1);
	strText = _T("α3");
	AddChar(m_arCharK_TorVn, strText, 2, 2, 1);
	strText = _T("((10-2");
	m_arCharK_TorVn.Add(AddChar(strText));
	strText = _T("βt)/20)");
	AddChar(m_arCharK_TorVn, strText, 2, 2, 1);
	strText = _T("bh0");
	AddChar(m_arCharK_TorVn, strText, 2, 2, 1);
	strText = _T("√((2+0.6P)√(");
	m_arCharK_TorVn.Add(AddChar(strText));
	strText = _T("fcu,k)");
	AddChar(m_arCharK_TorVn, strText, 2, 1, 4);
	strText = _T("ρsv");
	AddChar(m_arCharK_TorVn, strText, 2, 2, 2);
	strText = _T("fsv)");
	AddChar(m_arCharK_TorVn, strText, 2, 1, 2);

	//  Tn
	m_arCharK_Tn.RemoveAll();
	strText = _T("βt(");
	AddChar(m_arCharK_Tn, strText, 2, 2, 1);
	strText = _T("0.35");
	m_arCharK_Tn.Add(AddChar(strText));
	strText = _T("βa");
	AddChar(m_arCharK_Tn, strText, 2, 2, 1);
	strText = _T("ftd + 0.05");
	AddChar(m_arCharK_Tn, strText, 2, 1, 2);
	strText = _T("Np0/");
	AddChar(m_arCharK_Tn, strText, 2, 1, 2);
	strText = _T("A0)");
	AddChar(m_arCharK_Tn, strText, 2, 1, 1);
	strText = _T("Wt + 1.2√(ζ)");
	AddChar(m_arCharK_Tn, strText, 2, 1, 1);
	strText = _T("fsv");
	AddChar(m_arCharK_Tn, strText, 2, 1, 2);
	strText =_T("Asv1");
	AddChar(m_arCharK_Tn, strText, 2, 1, 3);
	strText =_T("Acor/");
	AddChar(m_arCharK_Tn, strText, 2, 1, 3);
	strText =_T("Sv");
	AddChar(m_arCharK_Tn, strText, 2, 1, 1);

	//  Tn(박스형)
	m_arCharK_TnBox.RemoveAll();
	strText = _T("βt(");
	AddChar(m_arCharK_TnBox, strText, 2, 2, 1);
	strText = _T("0.35");
	m_arCharK_TnBox.Add(AddChar(strText));
	strText = _T("βa");
	AddChar(m_arCharK_TnBox, strText, 2, 2, 1);
	strText = _T("ftd + 0.05");
	AddChar(m_arCharK_TnBox, strText, 2, 1, 2);
	strText = _T("Np0/");
	AddChar(m_arCharK_TnBox, strText, 2, 1, 2);
	strText = _T("A0)");
	AddChar(m_arCharK_TnBox, strText, 2, 1, 1);
	strText = _T("βa");
	AddChar(m_arCharK_TnBox, strText, 2, 2, 1);
	strText = _T("Wt + 1.2√(ζ)");
	AddChar(m_arCharK_TnBox, strText, 2, 1, 1);
	strText = _T("fsv");
	AddChar(m_arCharK_TnBox, strText, 2, 1, 2);
	strText =_T("Asv1");
	AddChar(m_arCharK_TnBox, strText, 2, 1, 3);
	strText =_T("Acor/");
	AddChar(m_arCharK_TnBox, strText, 2, 1, 3);
	strText =_T("Sv");
	AddChar(m_arCharK_TnBox, strText, 2, 1, 1);

}
// 7.Detail report
BOOL CRptJTG_D62_04::MakeDetailReportData()
{
	CString strPathName=m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strPathName);
	CString strInputFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".mrpt");

	//++++++++++++++++++++++++++++++
	CFileFind finder;
	// start working for files
	BOOL bWorking = finder.FindFile(strInputFilePathName);
	if ( !bWorking ) { return TRUE; }
	finder.FindNextFile();
	int ilength = finder.GetLength();
	if ( ilength <=0 ) { return TRUE; }
	//+++++++++++++++++++++++++++++++++++++++++++  
	T_CHAP_K chap1K;
	T_CHAP_RPT chap1D;  chap1D.Initialize();

	T_CHAP_K chap2K;
	T_CHAP_RPT chap2D;  chap2D.Initialize();

	T_PARA_K paraK;
	T_PARA_RPT paraD;   paraD.Initialize();

	T_TEXT_K textK;
	T_TEXT_RPT textD;   textD.Initialize();

	int iSubChapIdx = 1;
	chap1D = MakeChapter(1, m_iChapNum++, _T("圈玖셕炬법넋"));
	//////////////////////////////////////////////////////////////////////////
	CString strline;
	wifstream OpenFile(strInputFilePathName);
	while ( !OpenFile.eof() )
	{
		TCHAR buf[1024] = { 0 };
		OpenFile.getline(buf, sizeof(buf));
		strline = buf;

		if ( strline.Left(7)== _T("**BEGIN") ) {
			// Start Chapter Level2-1.
			strline.Replace(_T("**BEGIN"), _T(""));
			chap2D = MakeChapter(2, iSubChapIdx++, strline);
			paraD.Initialize();
			textD.Initialize();
			textD.CssK = m_pRptData->m_DefaultCss.TextCssK;
		}
		else if ( strline.Left(5)== _T("**END") ) {
			chap2D.arParaK.Add(paraK);
			m_pRptData->AddChapData(chap2K, chap2D);
			chap1D.arSubChapK.Add(chap2K);
		}
		else {
			strline.Replace(_T("  "), _T("&#160;&#160;"));
			strline.Replace(_T("<"), _T("＜"));
			textD.strText = strline;
			m_pRptData->AddTextData(textK, textD);
			paraD.arContentK.Add(textK);
			m_pRptData->AddParaData(paraK, paraD);
		}
	}
	OpenFile.close();

	m_pRptData->AddChapData(chap1K, chap1D);

	//  AddChapterToReport(chap1K);
	T_REPORT_RPT rptD; rptD.Initialize();
	m_pRptData->GetReportData(rptD);
	rptD.arChapK.Add(chap1K);
	m_pRptData->ModifyReportData(rptD);

	return TRUE;
}

