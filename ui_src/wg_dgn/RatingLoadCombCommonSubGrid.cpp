// RatingLoadCombCommonSubGrid.cpp : implementation file
//

#include "stdafx.h"

#include "wg_dgn.h"
#include "RatingLoadCombCommonSubGrid.h"

#include "..\wg_db\wg_db_AttrCtrl.h"

#include "RatingLoadCombCommonMainGrid.h"
#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_db\AttrCtrl2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombCommonSubGrid
CRatingLoadCombCommonSubGrid::CRatingLoadCombCommonSubGrid(int nType)
{
	m_pMainGrid = 0;
	m_sKey = _LS(IDS_WG_CMD__ADDD__None);
	m_pDoc = 0;
	m_nLcomType = D_LCOMTYPE_GENERAL;
	m_nLCtype = D_LCOM_CB_GENERAL;
	m_csPostfix = _T("CB");

	m_nGridType = nType;

	m_aTitle.RemoveAll();
	if(m_nGridType == 0)
	{
		m_aTitle.Add(_LS(IDS_CMD_STATIC_LOAD_CASES));
		m_aTitle.Add(_T("Chk.")); 
		m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Factor));
	}
	else if(m_nGridType == 1)
	{
		m_aTitle.Add(_LS(IDS_CMD_LIVE_LOAD_CASE));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Factor));
		m_aTitle.Add(_LS(IDS_CMD_GRID_TITLE_IMPACT_FACTOR_ENTER));
	}
	else
	{
		ASSERT(0);
	}
}

CRatingLoadCombCommonSubGrid::~CRatingLoadCombCommonSubGrid()
{
}

#define CWnd CTBBrowserWnd
BEGIN_MESSAGE_MAP(CRatingLoadCombCommonSubGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CRatingLoadCombCommonSubGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombCommonSubGrid message handlers
void CRatingLoadCombCommonSubGrid::Initialize(CDBDoc* pDoc)
{
	CTBBrowserWnd::Initialize();
	m_pDoc = pDoc;

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(m_aTitle.GetSize());
	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(10))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));

	// 컬럼 스타일 변경

	// Loadcase list

	SetLoadCaseColumnStyle(D_COL_RAC_LCASE);
	SetLoadCaseColumnStyle(D_COL_RAC_CHK_OR_FACTOR);

	if(m_nGridType == 0)
	{
		SetStyleRange(CGXRange().SetCols(D_COL_RAC_CHK_OR_FACTOR), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D)
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_NUMERIC)
			.SetValue(_T("0")));

		SetStyleRange(CGXRange().SetCols(D_COL_RAC_FACTOR_OR_IMPACTOR), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));
	}
	else if(m_nGridType == 1)
	{
		SetStyleRange(CGXRange().SetCols(D_COL_RAC_CHK_OR_FACTOR), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));

		SetStyleRange(CGXRange().SetCols(D_COL_RAC_FACTOR_OR_IMPACTOR), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));
	}
	else{ASSERT(0);}

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		(CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	SetScrollBarMode(SB_VERT, gxnAutomatic); 
	SetScrollBarMode(SB_HORZ, gxnAutomatic); 

	// column size 설정
#if defined(_US) || defined(_RUS)
	double scale = 1.154;
#else
	double scale = 1.0;
#endif

	auto L_SetColWidth = [this, scale] (ROWCOL nColSt, ROWCOL nColEd, int nWidth)
	{
		SetColWidth(nColSt, nColEd, globalUtils.ScaleByDPI(nWidth/scale));
	};

	if(m_nGridType == 0)
	{
		L_SetColWidth(D_COL_RAC_LCASE, D_COL_RAC_LCASE, 183);
		L_SetColWidth(D_COL_RAC_CHK_OR_FACTOR, D_COL_RAC_CHK_OR_FACTOR, 35);
		L_SetColWidth(D_COL_RAC_FACTOR_OR_IMPACTOR, D_COL_RAC_FACTOR_OR_IMPACTOR, 62);
		// coulmn resize 금지
		GetParam()->EnableTrackColWidth(FALSE);
	}
	else if(m_nGridType ==1)
	{
		SetRowHeightByDPI(0, 0, 36); // Impact Factor에 개행이 들어가면 1행을 좀더 키우게 됨

		L_SetColWidth(D_COL_RAC_LCASE, D_COL_RAC_LCASE, 156);
		L_SetColWidth(D_COL_RAC_CHK_OR_FACTOR, D_COL_RAC_CHK_OR_FACTOR, 62);
		L_SetColWidth(D_COL_RAC_FACTOR_OR_IMPACTOR, D_COL_RAC_FACTOR_OR_IMPACTOR, 62);
	}
	else{ASSERT(0);}

	// column의 이동을 금지시킨다.
	GetParam()->EnableMoveCols(FALSE);
	// row resize 금지
	GetParam()->EnableTrackRowHeight(FALSE); 

	if(m_nGridType == 1)
	{
		// 고정 Size Table 만들기
		SetRecordCount(2); 
		HideRows(1, 1, TRUE);
		m_bCanDeleteRowByPressingKey = FALSE;
		m_bCanInsertRowByPressingKey = FALSE;
		//m_bEnabledAppend = FALSE;

		SetScrollBarMode(SB_VERT, gxnDisabled); 		
	}

	// 데이타 설정
	//MakeItemEx();
	LockUpdate(FALSE);
}

// Column별 헤더 이름을 설정한다.
void CRatingLoadCombCommonSubGrid::SetHeaderTitle()
{
	CString title;
	int i;
	ROWCOL nCol;

	for(i = 0; i < GetColCount(); i++)
	{
		title = m_aTitle[i];
		nCol = i + 1;

		// 컬럼 헤더 이름 및 ID를 설정
		SetStyleRange(CGXRange(0, nCol), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
	}
}

// LoadCase Column의 Style을 지정한다.
// LoadCase 추가, 삭제, 수정시 Update를 콤보 박스에 반영하기 위해 사용하면 된다.
void CRatingLoadCombCommonSubGrid::SetLoadCaseColumnStyle(ROWCOL nCol)
{
	CString szLoadCaseList = _T("");

	POSITION pos;
	CString item;
	int nRangeStart;

	int nNameLen = 0, nDescLen = 0;
	CSize nTempLen;
	CDC* pDC = GetDC();
	ASSERT(pDC);

	if(nCol == D_COL_RAC_LCASE)
	{
		//D_LCOM_STATIC		1 : 정적 하중 조건
		T_STLD_K Key;
		T_STLD_D Data;
		nRangeStart = D_STLD_MAXNUM*D_LCOM_STATIC;
		pos = m_pDoc->m_pAttrCtrl->GetStartStld();
		while(pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextStld(pos,Key,Data);
			if (Data.LoadCaseType == _T("CS") || Data.LoadCaseType == _T("TCS")) continue;  // 시공타입은 제외
			item.Format(_T("%d\t%d\t%s(ST)\t%s\n"), 
				nRangeStart+Key, nRangeStart+Data.LoadCaseId, Data.LoadCaseName,
				Data.Description);
			szLoadCaseList += item;
			nTempLen = pDC->GetTextExtent(Data.LoadCaseName+_T("(ST)"));
			if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
			nTempLen = pDC->GetTextExtent(Data.Description);
			if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
		}

		//D_LCOM_SPECTRUM		2 : 응답 스펙트럼 하중 조건
		T_SPLC_K SplcKey;
		T_SPLC_D SplcData;
		SplcData.Initialize();
		nRangeStart = D_STLD_MAXNUM*D_LCOM_SPECTRUM;
		pos = m_pDoc->m_pAttrCtrl->GetStartSplc();
		while(pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextSplc(pos,SplcKey,SplcData);
			item.Format(_T("%d\t%d\t%s(RS)\t%s\n"), 
				nRangeStart+SplcKey, nRangeStart+SplcData.LoadCaseId, SplcData.LoadCaseName,
				SplcData.Description);
			szLoadCaseList += item;
			nTempLen = pDC->GetTextExtent(SplcData.LoadCaseName+_T("(RS)"));
			if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
			nTempLen = pDC->GetTextExtent(SplcData.Description);
			if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
		}

		//D_LCOM_HISTORY		3 : 시간 이력 하중 조건
		T_THIS_K ThisKey;
		T_THIS_D ThisData;
		nRangeStart = D_STLD_MAXNUM*D_LCOM_HISTORY;
		pos = m_pDoc->m_pAttrCtrl->GetStartThis();
		while(pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextThis(pos,ThisKey,ThisData);
			item.Format(_T("%d\t%d\t%s(TH)\t%s\n"), 
				nRangeStart+ThisKey, nRangeStart+ThisData.LoadCaseId, ThisData.LoadCaseName,
				ThisData.Description);
			szLoadCaseList += item;
			nTempLen = pDC->GetTextExtent(ThisData.LoadCaseName+_T("(TH)"));
			if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
			nTempLen = pDC->GetTextExtent(ThisData.Description);
			if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
		}


		//D_LCOM_MOVING		  4 : 이동 하중 조건
		// 코드에 따른 방식으로 수정 
		T_MVCD_D DataMvcd;
		if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
		switch (DataMvcd.nCodeType)
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
				T_MVLD_K MvldKey;
				T_MVLD_D MvldData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				pos = m_pDoc->m_pAttrCtrl->GetStartMvld();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvld(pos,MvldKey,MvldData);
					item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
						nRangeStart+MvldKey, nRangeStart+MvldData.LoadCaseId, MvldData.LoadCaseName,
						MvldData.Description);
					szLoadCaseList += item;
					nTempLen = pDC->GetTextExtent(MvldData.LoadCaseName+_T("(MV)"));
					if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
					nTempLen = pDC->GetTextExtent(MvldData.Description);
					if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
				}
			}
			break;
		case D_MOVE_CODE_CHINA:
			{
				T_MVLDch_K MvldchKey;
				T_MVLDch_D MvldchData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				pos = m_pDoc->m_pAttrCtrl->GetStartMvldch();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvldch(pos,MvldchKey,MvldchData);
					item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
						nRangeStart+MvldchKey, nRangeStart+MvldchData.LoadCaseId, MvldchData.LoadCaseName,
						MvldchData.Description);
					szLoadCaseList += item;
					nTempLen = pDC->GetTextExtent(MvldchData.LoadCaseName+_T("(MV)"));
					if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
					nTempLen = pDC->GetTextExtent(MvldchData.Description);
					if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;      
				}
			}
			break;
		case D_MOVE_CODE_JAPAN:
			{
				// japan version
				T_MVLDjp_K MvldjpKey;
				T_MVLDjp_D MvldjpData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				pos = m_pDoc->m_pAttrCtrl->GetStartMvldjp();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvldjp(pos,MvldjpKey,MvldjpData);
					item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
						nRangeStart+MvldjpKey, nRangeStart+MvldjpData.LoadCaseId, MvldjpData.LoadCaseName,
						MvldjpData.Description);
					szLoadCaseList += item;
					nTempLen = pDC->GetTextExtent(MvldjpData.LoadCaseName+_T("(MV)"));
					if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
					nTempLen = pDC->GetTextExtent(MvldjpData.Description);
					if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
				}      
			}
			break;
		case D_MOVE_CODE_INDIA:
			{
				T_MVLDid_K MvldidKey;
				T_MVLDid_D MvldidData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				pos = m_pDoc->m_pAttrCtrl->GetStartMvldid();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvldid(pos,MvldidKey,MvldidData);
					item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
						nRangeStart+MvldidKey, nRangeStart+MvldidData.LoadCaseId, MvldidData.LoadCaseName,
						MvldidData.Description);
					szLoadCaseList += item;
					nTempLen = pDC->GetTextExtent(MvldidData.LoadCaseName+_T("(MV)"));
					if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
					nTempLen = pDC->GetTextExtent(MvldidData.Description);
					if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
				}
			}
			break;
		case D_MOVE_CODE_BS:
			{
				T_MVLDbs_K MvldbsKey;
				T_MVLDbs_D MvldbsData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				pos = m_pDoc->m_pAttrCtrl->GetStartMvldbs();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvldbs(pos,MvldbsKey,MvldbsData);
					item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
						nRangeStart+MvldbsKey, nRangeStart+MvldbsData.LoadCaseId, MvldbsData.LoadCaseName,
						MvldbsData.Description);
					szLoadCaseList += item;
					nTempLen = pDC->GetTextExtent(MvldbsData.LoadCaseName+_T("(MV)"));
					if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
					nTempLen = pDC->GetTextExtent(MvldbsData.Description);
					if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
				}
			}
			break;
		case D_MOVE_CODE_FRANCE:
			{
				T_MVLDfr_K MvldfrKey;
				T_MVLDfr_D MvldfrData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				pos = m_pDoc->m_pAttrCtrl->GetStartMvldfr();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvldfr(pos, MvldfrKey, MvldfrData);
					item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
						nRangeStart+ MvldfrKey, nRangeStart+ MvldfrData.LoadCaseId, MvldfrData.LoadCaseName,
						MvldfrData.Description);
					szLoadCaseList += item;
					nTempLen = pDC->GetTextExtent(MvldfrData.LoadCaseName+_T("(MV)"));
					if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
					nTempLen = pDC->GetTextExtent(MvldfrData.Description);
					if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
				}
			}
			break;
		case D_MOVE_CODE_TRANS:
			{
				T_MVLDtr_K MvldKey;
				T_MVLDtr_D MvldData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				pos = m_pDoc->m_pAttrCtrl->GetStartMvldtr();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvldtr(pos,MvldKey,MvldData);
					item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
						nRangeStart+MvldKey, nRangeStart+MvldData.LoadCaseId, MvldData.LoadCaseName,
						MvldData.Description);
					szLoadCaseList += item;
					nTempLen = pDC->GetTextExtent(MvldData.LoadCaseName+_T("(MV)"));
					if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
					nTempLen = pDC->GetTextExtent(MvldData.Description);
					if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
				}
			}
			break;
		default:
			ASSERT(0);
			break;
		}


		//D_LCOM_SETTLEMENT	5 : ?? 하중 조건
		T_SMLC_K SmlcKey;
		T_SMLC_D SmlcData;
		nRangeStart = D_STLD_MAXNUM*D_LCOM_SETTLEMENT;
		pos = m_pDoc->m_pAttrCtrl->GetStartSmlc();
		while(pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextSmlc(pos,SmlcKey,SmlcData);
			item.Format(_T("%d\t%d\t%s(SM)\t%s\n"), 
				nRangeStart+SmlcKey, nRangeStart+SmlcData.LoadCaseId, SmlcData.LoadCaseName,
				SmlcData.Description);
			szLoadCaseList += item;
			nTempLen = pDC->GetTextExtent(SmlcData.LoadCaseName+_T("(SM)"));
			if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
			nTempLen = pDC->GetTextExtent(SmlcData.Description);
			if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
		}
		//D_LCOM_STAGE 13: 시공단계 하중 조건
		CArray<T_SGLD_K, T_SGLD_K> aSgldKey;
		T_SGLD_K SgldKey;
		T_SGLD_D SgldData;
		nRangeStart = D_STLD_MAXNUM*D_LCOM_STAGE;
		m_pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldKey);
		int nSgldNum = aSgldKey.GetSize();
		for (int n = 0; n < nSgldNum; n++)
		{
			SgldKey = aSgldKey[n];
			if (m_pDoc->m_pAttrCtrl->GetSgld(SgldKey, SgldData))
			{
				item.Format(_T("%d\t%d\t%s(CS)\t%s\n"),
					nRangeStart+SgldKey, nRangeStart+SgldData.LoadCaseId, SgldData.LoadCaseName,
					SgldData.Description);
				szLoadCaseList += item;
				nTempLen = pDC->GetTextExtent(SgldData.LoadCaseName+_T("(CS)"));
				if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
				nTempLen = pDC->GetTextExtent(SgldData.Description);
				if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
			}
		}
		//D_LCOM_ECCENSPEC 우발편심 Spectrum
		CArray<T_ESPC_K, T_ESPC_K> aEspcKey;
		T_ESPC_K EspcKey;
		T_ESPC_D EspcData;
		nRangeStart = D_STLD_MAXNUM*D_LCOM_ECCENSPEC;
		m_pDoc->m_pAttrCtrl->GetEspcKeyList(aEspcKey);
		int nEspcNum = aEspcKey.GetSize();
		for (int n = 0; n < nEspcNum; n++)
		{
			EspcKey = aEspcKey[n];
			if (m_pDoc->m_pAttrCtrl->GetEspc(EspcKey, EspcData))
			{
				item.Format(_T("%d\t%d\t%s(ES)\t%s\n"),
					nRangeStart+EspcKey, nRangeStart+EspcData.LoadCaseId, EspcData.LoadCaseName,
					EspcData.Description);
				szLoadCaseList += item;
				nTempLen = pDC->GetTextExtent(EspcData.LoadCaseName+_T("(ES)"));
				if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
				nTempLen = pDC->GetTextExtent(EspcData.Description);
				if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
			}
		}
		//D_LCOM_GRIDMODEL 격자모델 하중 케이스
		CArray<T_GILC_K, T_GILC_K> aGilcKey;
		T_GILC_K GilcKey;
		T_GILC_D GilcData;
		nRangeStart = D_STLD_MAXNUM*D_LCOM_GRIDMODEL;
		m_pDoc->m_pAttrCtrl->GetGilcKeyList(aGilcKey);
		int nGilcNum = aGilcKey.GetSize();
		for (int n = 0; n < nGilcNum; n++)
		{
			GilcKey = aGilcKey[n];
			if (m_pDoc->m_pAttrCtrl->GetGilc(GilcKey, GilcData))
			{
				item.Format(_T("%d\t%d\t%s(GI)\t%s\n"),
					nRangeStart+GilcKey, nRangeStart+GilcData.LoadCaseId, GilcData.LoadCaseName,
					GilcData.Description);
				szLoadCaseList += item;
				nTempLen = pDC->GetTextExtent(GilcData.LoadCaseName+_T("(GI)"));
				if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
				nTempLen = pDC->GetTextExtent(GilcData.Description);
				if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
			}
		}
		//D_LCOM_CRANE 19 : Crane 하중
		BOOL bExistCrane = m_pDoc->m_pAttrCtrl->GetCountCran();
		nRangeStart = D_STLD_MAXNUM*D_LCOM_CRANE;
		if(bExistCrane)
		{
			CString strLCName = _LS(IDS_CMD_CRANE);
			item.Format(_T("%d\t%d\t%s(CN)\t%s\n"), nRangeStart+1, nRangeStart+1, strLCName, _T(""));
			szLoadCaseList += item;
			if(m_pDoc->m_pAttrCtrl->IsExistCranBraking())
			{
				strLCName = _LS(IDS_CMD_CRANE_BRK);
				item.Format(_T("%d\t%d\t%s(CN)\t%s\n"), nRangeStart+2, nRangeStart+2, strLCName, _T(""));
				szLoadCaseList += item;
			}
			nTempLen = pDC->GetTextExtent(strLCName+_T("(CN)"));
			if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;   
		}
		/*
		//D_LCOM_CB_????	 : 하중 조합
		T_LCOM_K LcomKey;
		T_LCOM_D LcomData;
		CArray<T_LCOM_K, T_LCOM_K> aLcomKey;
		nRangeStart = D_STLD_MAXNUM*m_nLCtype;
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(m_nLcomType, aLcomKey);
		int nNum; // 현재 Combination 앞까지의 Combination 갯수
		if (IsMainAppendRow() || IsMainNewRow()) nNum = aLcomKey.GetSize();
		else
		{
		LcomKey = _ttol(m_sKey);
		if (m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, LcomKey, LcomData)) 
		{
		//nNum = LcomData.LoadCombId-1;
		for (int i = 0; i < aLcomKey.GetSize(); i++)
		if (LcomKey == aLcomKey[i]) break;
		nNum = i;
		}
		else nNum = 0;
		}
		for (int i = 0; i < nNum; i++)
		{
		LcomKey = aLcomKey[i];
		m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, LcomKey, LcomData);
		item.Format(_T("%d\t%d\t%s(%s)\t%s\n"), 
		nRangeStart+LcomKey, nRangeStart+LcomData.LoadCombId, LcomData.LoadCombName,
		m_csPostfix, LcomData.Description);
		szLoadCaseList += item;
		nTempLen = pDC->GetTextExtent(LcomData.LoadCombName+_T("(") + m_csPostfix + _T(")"));
		if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
		nTempLen = pDC->GetTextExtent(LcomData.Description);
		if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
		}	
		*/

		CString csSize;
		csSize.Format(_T("0,0,%d,%d"), nNameLen, nDescLen);
		SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(szLoadCaseList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("2"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_COLWIDTHS, csSize)
			.SetUserAttribute(GX_IDS_UA_TABLIST_SORTCOL, _T("1"))
			.SetValueType(GX_VT_NUMERIC)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
	}
	else if(m_nGridType == 0 && nCol == D_COL_RAC_CHK_OR_FACTOR)
	{
		SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D)
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_NUMERIC) 
			.SetValue(_T("0")));
	} 


	ReleaseDC(pDC);
}

BOOL CRatingLoadCombCommonSubGrid::IsMainAppendRow()
{
	return m_sKey == _LS(IDS_WG_CMD__ADDD__Append_Row);
}

BOOL CRatingLoadCombCommonSubGrid::IsMainNewRow()
{
	return m_sKey == _LS(IDS_WG_CMD__ADDD__New_Row);
}

BOOL CRatingLoadCombCommonSubGrid::OnAddNewRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnAddNewRecord(nRow);

	// MQC 6867   ////////////////////////////////////////////////////
	CString value;
	value = GetValueRowCol(nRow, D_COL_RAC_LCASE);
	value.TrimLeft();
	value.TrimRight();

	if(value.IsEmpty()) return FALSE;
	//////////////////////////////////////////////////////////////////

	//SetRecordModified();
	return bRetVal;
}

BOOL CRatingLoadCombCommonSubGrid::OnModifyRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnModifyRecord(nRow);

	// MQC 6867   ////////////////////////////////////////////////////
	CString value;
	value = GetValueRowCol(nRow, D_COL_RAC_LCASE);
	value.TrimLeft();
	value.TrimRight();

	if(value.IsEmpty()) return FALSE;
	//////////////////////////////////////////////////////////////////

	//SetRecordModified();
	return bRetVal;
}

BOOL CRatingLoadCombCommonSubGrid::OnDeleteRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnDeleteRecord(nRow);
	m_pMainGrid->SetModeEdit();
	return bRetVal;
}

BOOL CRatingLoadCombCommonSubGrid::OnDeleteRecords(CRowColArray &awRows)
{
	BOOL bRetVal = CTBBrowserWnd::OnDeleteRecords(awRows);
	m_pMainGrid->SetModeEdit();
	return bRetVal;
}

BOOL CRatingLoadCombCommonSubGrid::OnCanceledAddNew()
{
	BOOL bRetVal = CTBBrowserWnd::OnCanceledAddNew();
	return bRetVal;
}

BOOL CRatingLoadCombCommonSubGrid::OnCanceledModify()
{
	BOOL bRetVal = CTBBrowserWnd::OnCanceledModify();
	return bRetVal;
}

BOOL CRatingLoadCombCommonSubGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	// 여기서는 Default 값을 채워 넣는 역할을 하면 된다.
	BOOL bActive;
	ROWCOL ncRow, ncCol;
	CString value;

	// 현재 셀이 Active되어 있으면 값을 채워 넣지 않는다.
	VERIFY(GetCurrentCell(ncRow, ncCol));
	CGXControl* pControl = (CGXControl*)GetControl(ncRow, ncCol);
	if (pControl && pControl->IsActive()) bActive = TRUE;
	else bActive = FALSE;
	
	if(ncCol != D_COL_RAC_LCASE)
	{
		CString sKey;
		CGXStyle style;
		VERIFY(GetColStyle(D_COL_RAC_LCASE, style));
		CString szChoiceList = style.GetChoiceList();
		if (szChoiceList == _T("")) return TRUE;  // ChoiceList없으면 그냥 리턴
		int nPos = szChoiceList.Find('\t'); // 첫번째 분리자를 찾는다.(key)
		VERIFY(nPos != -1);
		sKey = szChoiceList.Left(nPos);
		SetExpressionRowCol(nRow, D_COL_RAC_LCASE, sKey); // set default Load Case value
	}
	
	if(m_nGridType ==  0)
	{
		if(ncCol != D_COL_RAC_CHK_OR_FACTOR)
		{
			SetExpressionRowCol(nRow, D_COL_RAC_CHK_OR_FACTOR, _T("1"));  // set  default check status
		}  
		if(ncCol != D_COL_RAC_FACTOR_OR_IMPACTOR)
		{
			SetExpressionRowCol(nRow, D_COL_RAC_FACTOR_OR_IMPACTOR, GetDefFactorVal(m_nGridType));  // set default Factor value
		}    
		
	}
	else if(m_nGridType ==  1)
	{
		if(ncCol != D_COL_RAC_CHK_OR_FACTOR)
		{
			SetExpressionRowCol(nRow, D_COL_RAC_CHK_OR_FACTOR, GetDefFactorVal(m_nGridType));  // set default Factor value
		}  
		if(ncCol != D_COL_RAC_FACTOR_OR_IMPACTOR)
		{
			SetExpressionRowCol(nRow, D_COL_RAC_FACTOR_OR_IMPACTOR, _T("0.3"));  // set default Factor value
		}

		HideRows(nRow + 1, nRow + 1, TRUE);
	}
	else
	{
		ASSERT(0);
	}    

	return TRUE;
}

// 셀이 수정되면 EditMode로 들어간다.
// 주1:MainGrid가 AppendRow에 있을 때는 셀이 수정되면 먼저 AddNew가 호출되어야 한다.
// 이 때는 현재 Record의 위치가 변경되었으므로 OnChangeCurrentRecord가 호출되어야
// 하는데 셀이 수정된 후 SetRecordModified가 호출되면 수정된 내용이 지워지므로 
// SetRecordModified를 먼저 수행한다.
void CRatingLoadCombCommonSubGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	m_pMainGrid->SetModeEdit();// 먼저 호출되어야 한다. 주1 참조
	CTBBrowserWnd::OnModifyCell(nRow, nCol);
}

void CRatingLoadCombCommonSubGrid::OnChangeStld()
{
	// 지워진 하중들은 제거한다.
	T_STLD_D stldData;
	T_SPLC_D splcData;
	T_THIS_D thisData;
	T_MVLD_D mvldData;
	T_MVLDjp_D mvldjpData;
	T_MVLDch_D mvldchData;
	T_MVLDid_D mvldidData;
	T_MVLDbs_D mvldbsData;
	T_MVLDfr_D mvldfrData;
	T_MVLDtr_D mvldtrData;
	T_SMLC_D smlcData;
	T_SGLD_D sgldData;
	T_ESPC_D espcData;
	T_GILC_D gilcData;
	T_LCOM_D lcomData;

	int nAnalType;
	T_STLD_K nKey;
	int nSize = GetRowCount()-1;
	for (int i = nSize-1; i >= 0; i--)
	{
		nKey = _ttol(GetValueRowCol(i+1, 1));
		nAnalType = nKey / D_STLD_MAXNUM;
		nKey = nKey % D_STLD_MAXNUM;
		switch(nAnalType)
		{
		case D_LCOM_STATIC: 
			if (!m_pDoc->m_pAttrCtrl->GetStld(nKey, stldData)) 
				DeleteRecord(i+1);
			break;
		case D_LCOM_SPECTRUM:
			if (!m_pDoc->m_pAttrCtrl->GetSplc(nKey, splcData)) 
				DeleteRecord(i+1);
			break;
		case D_LCOM_HISTORY:
			if (!m_pDoc->m_pAttrCtrl->GetThis(nKey, thisData)) 
				DeleteRecord(i+1);
			break;
		case D_LCOM_MOVING:
			{
				// 코드에 따른 방식으로 수정 
				T_MVCD_D DataMvcd;
				if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
				switch (DataMvcd.nCodeType)
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
						if (!m_pDoc->m_pAttrCtrl->GetMvld(nKey, mvldData)) 
							DeleteRecord(i+1);
					}
					break;
				case D_MOVE_CODE_CHINA:
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldch(nKey, mvldchData)) 
							DeleteRecord(i+1);
					}
					break;
				case D_MOVE_CODE_JAPAN:
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldjp(nKey, mvldjpData)) 
							DeleteRecord(i+1);
					}
					break;
				case D_MOVE_CODE_INDIA:
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldid(nKey, mvldidData)) 
							DeleteRecord(i+1);
					}
					break;
				case D_MOVE_CODE_BS:
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldbs(nKey, mvldbsData)) 
							DeleteRecord(i+1);
					}
				case D_MOVE_CODE_FRANCE:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldfr(nKey, mvldfrData))
						DeleteRecord(i + 1);
				}
				case D_MOVE_CODE_TRANS:
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldtr(nKey, mvldtrData)) 
							DeleteRecord(i+1);
					}
					break;
				default: ASSERT(0);
					break;
				}
			}
			break;
		case D_LCOM_SETTLEMENT:
			if (!m_pDoc->m_pAttrCtrl->GetSmlc(nKey, smlcData)) 
				DeleteRecord(i+1);
		case D_LCOM_STAGE:
			if (!m_pDoc->m_pAttrCtrl->GetSgld(nKey, sgldData))
				DeleteRecord(i+1);
			break;
		case D_LCOM_ECCENSPEC:
			if (!m_pDoc->m_pAttrCtrl->GetEspc(nKey, espcData)) 
				DeleteRecord(i+1);
			break;
		case D_LCOM_GRIDMODEL:
			if (!m_pDoc->m_pAttrCtrl->GetGilc(nKey, gilcData)) 
				DeleteRecord(i+1);
			break;
		case D_LCOM_CB_GENERAL:
		case D_LCOM_CB_STEEL:
		case D_LCOM_CB_CONCRETE:
		case D_LCOM_CB_FOOTING:
		case D_LCOM_CB_SRC:
		case D_LCOM_CB_ALUMINUM:
		case D_LCOM_CB_STLCOMP:
			ASSERT(0);
			if (!m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, nKey, lcomData))
				DeleteRecord(i+1);
			break;
		}
	}

	SetLoadCaseColumnStyle(D_COL_RAC_LCASE);
	SetLoadCaseColumnStyle(D_COL_RAC_CHK_OR_FACTOR);

	// 현재 셀을 다시 초기화 시킨다.
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return;
	CGXControl* pControl = this->GetControl(ncRow, ncCol);
	if (pControl) pControl->Init(ncRow, ncCol);
}

BOOL CRatingLoadCombCommonSubGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	// Factor가 실수(정수포함)로 입력되어 있는지 검사한다.
	if(nCol == D_COL_RAC_LCASE)
	{
		T_STLD_D stldData;
		T_SPLC_D splcData;
		T_THIS_D thisData;
		T_MVLD_D mvldData;
		T_MVLDjp_D mvldjpData;
		T_MVLDch_D mvldchData;
		T_MVLDid_D mvldidData;
		T_MVLDbs_D mvldbsData;
		T_MVLDfr_D mvldfrData;
		T_MVLDtr_D mvldtrData;
		T_SMLC_D smlcData;
		T_SGLD_D sgldData;
		T_ESPC_D espcData;
		T_GILC_D gilcData;
		T_LCOM_D lcomData;

		int nAnalType;
		T_STLD_K nKey;

		nKey = _ttol(GetValueRowCol(nRow, nCol));
		nAnalType = nKey / D_STLD_MAXNUM;
		nKey = nKey % D_STLD_MAXNUM;

		BOOL bError = FALSE;

		switch(nAnalType)
		{
		case D_LCOM_STATIC: 
			if (!m_pDoc->m_pAttrCtrl->GetStld(nKey, stldData)) 
				bError = TRUE;
			break;
		case D_LCOM_SPECTRUM:
			if (!m_pDoc->m_pAttrCtrl->GetSplc(nKey, splcData)) 
				bError = TRUE;
			break;
		case D_LCOM_HISTORY:
			if (!m_pDoc->m_pAttrCtrl->GetThis(nKey, thisData)) 
				bError = TRUE;
			break;
		case D_LCOM_MOVING:
			{
				// 코드에 따른 방식으로 수정 
				T_MVCD_D DataMvcd;
				if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
				switch (DataMvcd.nCodeType)
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
						if (!m_pDoc->m_pAttrCtrl->GetMvld(nKey, mvldData)) 
							bError = TRUE;
					}
					break;
				case D_MOVE_CODE_CHINA:
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldch(nKey, mvldchData)) 
							bError = TRUE;
					}
					break;
				case D_MOVE_CODE_JAPAN:
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldjp(nKey, mvldjpData)) 
							bError = TRUE;
					}
					break;
				case D_MOVE_CODE_INDIA:
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldid(nKey, mvldidData)) 
							bError = TRUE;
					}
					break;
				case D_MOVE_CODE_BS:
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldbs(nKey, mvldbsData)) 
							bError = TRUE;
					}
				case D_MOVE_CODE_FRANCE:
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldfr(nKey, mvldfrData)) 
							bError = TRUE;
					}
					break;
				case D_MOVE_CODE_TRANS:
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldtr(nKey, mvldtrData)) 
							bError = TRUE;
					}
					break;
				default: ASSERT(0);
					break;
				}
			}
			break;
		case D_LCOM_SETTLEMENT:
			if (!m_pDoc->m_pAttrCtrl->GetSmlc(nKey, smlcData)) 
				bError = TRUE;
		case D_LCOM_STAGE:
			if (!m_pDoc->m_pAttrCtrl->GetSgld(nKey, sgldData))
				bError = TRUE;
			break;
		case D_LCOM_ECCENSPEC:
			if (!m_pDoc->m_pAttrCtrl->GetEspc(nKey, espcData)) 
				bError = TRUE;
			break;
		case D_LCOM_GRIDMODEL:
			if (!m_pDoc->m_pAttrCtrl->GetGilc(nKey, gilcData)) 
				bError = TRUE;
			break;
		case D_LCOM_CB_GENERAL:
		case D_LCOM_CB_STEEL:
		case D_LCOM_CB_CONCRETE:
		case D_LCOM_CB_FOOTING:
		case D_LCOM_CB_SRC:
		case D_LCOM_CB_ALUMINUM:
		case D_LCOM_CB_STLCOMP:
			if (!m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, nKey, lcomData))
				bError = TRUE;
			break;
		}

		if(bError) return FALSE;

		return TRUE;
	}
	else if(nCol == D_COL_RAC_CHK_OR_FACTOR)
	{
		if(m_nGridType == 0)
		{

		}
		else if(m_nGridType == 1)
		{
			return ValidataFieldDbl(nRow, nCol);
		}
		else
		{
			ASSERT(0);
		}    
	}
	else if (nCol == D_COL_RAC_FACTOR_OR_IMPACTOR)
	{
		if(m_nGridType == 0)
		{
			return ValidataFieldDbl(nRow, nCol);
		}
		else if(m_nGridType == 1)
		{
			return ValidataFieldDbl(nRow, nCol);
		}
		else
		{
			ASSERT(0);
		}    
	}
	return TRUE;
}

// CTBBrowserWnd::OnStartEditing 참고
// Rating에서 체크박스에 눌럿을때는 AddRow 안한다.
BOOL CRatingLoadCombCommonSubGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 이미 추가 모드이거나 편집 모드이면 무시
	if (m_nEditMode == addnew || m_nEditMode == edit)
		return CGXGridWnd::OnStartEditing(nRow, nCol);

	//   if(m_nGridType == 0 && nCol==2) 
	//     return CGXGridWnd::OnStartEditing(nRow, nCol);

	CGXControl* pControl = GetControl(nRow, nCol);
	if (!pControl) return CGXGridWnd::OnStartEditing(nRow, nCol);

	// ComboBox 종류는 무시해야 한다.
	// 문제는 이것이 콤보 박스인지 아닌지 어떻게 아느냐 하는 것이다.
	// style 정보에서 가져 오는 것 : 문제 있음.
	if (pControl->IsKindOf(RUNTIME_CLASS(CGXComboBoxWnd)))
		return CGXGridWnd::OnStartEditing(nRow, nCol);

	// edit control은 실제 고쳐질때 실행
	if (pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl)))
		return CGXGridWnd::OnStartEditing(nRow, nCol);

	// Button Type(radio , check box...)
	// 이 타입은 DECLARE_DYNAMIC이 선언 안 되어 있기 때문에 아래와 같이
	// DECLARE_CONTROL, CONTROL_CLASS를 이용한다. 
	BOOL bButtonType = FALSE;

	if (pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXCheckBox))
		|| pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXRadioButton))
		) bButtonType = TRUE;

	// check box는 무조건 실행(왜냐하면 클릭되면 값이 바뀌기 때문)
	if (!bButtonType) 
		return CGXGridWnd::OnStartEditing(nRow, nCol);

	//   // Rating에서 체크박스에 눌럿을때는 AddRow 안한다.
	//   if(m_nGridType == 0 && nCol==2) return TRUE;

	// go into addnew or edit mode
	if(m_nGridType == 0 && nCol==D_COL_RAC_CHK_OR_FACTOR) 
	{
		OnModifyCell(nRow, D_COL_RAC_FACTOR_OR_IMPACTOR);  // 체크박스 변경으로는 ModifyCell이 안되어 옆 셀 값을 주었음.
		return TRUE;
	}

	//   else if (IsAppendRow(nRow)) AddNew(nRow);
	if (IsAppendRow(nRow)) AddNew(nRow);
	else Edit(nRow);
	return CGXGridWnd::OnStartEditing(nRow, nCol);
}

BOOL CRatingLoadCombCommonSubGrid::ValidataFieldDbl( ROWCOL nRow, ROWCOL nCol )
{
	CString value;
	int nErrCode;
	value = GetValueRowCol(nRow, nCol);
	value.TrimLeft();
	value.TrimRight();
	if (!IsValidDoubleNumber(value, nErrCode))
	{
		switch(nErrCode)
		{
		case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_Formula_)); break;
		case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Input_Number_)); break;
		case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Out_of_range_)); break;
		}
		return FALSE;
	}
	return TRUE;
}
