// CMLoadCombSubGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCombSubGrid.h"

#include "..\wg_db\wg_db_AttrCtrl.h"

#include "CMLoadCombMainGrid.h"
#include "..\wg_base\wg_base_MsgDll.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombSubGrid

CCMLoadCombSubGrid::CCMLoadCombSubGrid()
{
	m_pMainGrid = 0;
	m_sKey = _LS(IDS_WG_CMD__ADDD__None);
	m_pDoc = 0;
	m_nLcomType = D_LCOMTYPE_GENERAL;
	m_nLCtype = D_LCOM_CB_GENERAL;
	m_csPostfix = _T("CB");

	m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__LoadCase));
	m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Factor));
	m_nSeismicType = LCOM_TYPE::LCOM_GENERAL;
}

CCMLoadCombSubGrid::~CCMLoadCombSubGrid()
{
}

#define CWnd CTBBrowserWnd
BEGIN_MESSAGE_MAP(CCMLoadCombSubGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCMLoadCombSubGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombSubGrid message handlers
void CCMLoadCombSubGrid::Initialize(CDBDoc* pDoc)
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
	SetLoadCaseColumnStyle(1);

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	 // column size 설정
#if defined(_RUS)
	double scale = 1.154;
#else
	double scale = 1.0;
#endif

	SetColWidthByDPI(1, 1, 255/scale);
	SetColWidthByDPI(2, 2, 50/scale);

	// column의 이동을 금지시킨다.
	GetParam()->EnableMoveCols(FALSE);
	// row resize 금지
	GetParam()->EnableTrackRowHeight(FALSE); 

	// 데이타 설정
	//MakeItemEx();
	LockUpdate(FALSE);
}

void CCMLoadCombSubGrid::SetLcomType(int nLcomType)
{
	m_nLcomType = nLcomType;
	switch(m_nLcomType)
	{
	case D_LCOMTYPE_GENERAL : 
		m_nLCtype = D_LCOM_CB_GENERAL;  m_csPostfix = _T("CB"); break;
	case D_LCOMTYPE_STEEL   : 
		m_nLCtype = D_LCOM_CB_STEEL;    m_csPostfix = _T("CBS"); break;
	case D_LCOMTYPE_CONCRETE: 
		m_nLCtype = D_LCOM_CB_CONCRETE; m_csPostfix = _T("CBC"); break;
	case D_LCOMTYPE_FDN     : 
		m_nLCtype = D_LCOM_CB_FOOTING;  m_csPostfix = _T("CBF"); break;
	case D_LCOMTYPE_SRC     : 
		m_nLCtype = D_LCOM_CB_SRC;      m_csPostfix = _T("CBR"); break;
	case D_LCOMTYPE_ALUMINUM: 
		m_nLCtype = D_LCOM_CB_ALUMINUM; m_csPostfix = _T("CBA"); break;
	case D_LCOMTYPE_STLCOMP: 
		m_nLCtype = D_LCOM_CB_STLCOMP;  m_csPostfix = _T("CBSC"); break;
	case D_LCOMTYPE_CFSTEEL   : 
		m_nLCtype = D_LCOM_CB_CFSTEEL;  m_csPostfix = _T("CBCF"); break;
	case D_LCOMTYPE_SEISMIC: 
		m_nLCtype = D_LCOM_CB_SEISMIC;  m_csPostfix = _T("CBSM"); break;
	case D_LCOMTYPE_LINEAR_EVAL: 
		m_nLCtype = D_LCOM_CB_LINEAR_EVAL;  m_csPostfix = _T("CBLN"); break;
	default: ASSERT(0);       
		m_nLCtype = D_LCOM_CB_GENERAL;  m_csPostfix = _T("CB"); break;
	}
}

void CCMLoadCombSubGrid::ChangeLcomType(int nLcomType)
{
	SetLcomType(nLcomType);
}

// Column별 헤더 이름을 설정한다.
void CCMLoadCombSubGrid::SetHeaderTitle()
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
void CCMLoadCombSubGrid::SetLoadCaseColumnStyle(ROWCOL nCol)
{
	CString szLoadCaseList = _T("");

	CString item;
	int nRangeStart;

	int nNameLen = 0, nDescLen = 0;
	CSize nTempLen;
	CDC* pDC = GetDC();
	ASSERT(pDC);

	//D_LCOM_STATIC		1 : 정적 하중 조건
	T_STLD_K Key;
	T_STLD_D Data;
	nRangeStart = D_STLD_MAXNUM*D_LCOM_STATIC;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartStld();
	while(pos != NULL)
	{
	  m_pDoc->m_pAttrCtrl->GetNextStld(pos,Key,Data);
		if (Data.LoadCaseType == _T("CS") || Data.LoadCaseType == _T("TCS")) continue;  // 시공타입은 제외

		// 내진성능평가 : E타입은 의사지진력 할당된 것만 보여줌. 수평지진력만 고려하므로 EVT 등은 고려 안 함
		if(m_nSeismicType == LCOM_SEISMIC && Data.LoadCaseType==_T("E") && !m_pDoc->m_pAttrCtrl->ExistPsll(Key))
			continue;

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

	//D_LCOM_SPECTRUM_ND	: ND용 응답 스펙트럼 하중 조건
	CArray<T_NSPC_K, T_NSPC_K> aNspcKey;
	T_NSPC_K NspcKey;
	T_NSPC_D NspcData;
	NspcData.Initialize();
	nRangeStart = D_STLD_MAXNUM*D_LCOM_SPECTRUM_ND;
	m_pDoc->m_pAttrCtrl->GetNspcKeyList(aNspcKey);
	int nNspcNum = aNspcKey.GetSize();
	for (int n = 0; n < nNspcNum; n++)
	{
		NspcKey = aNspcKey[n];
		if (m_pDoc->m_pAttrCtrl->GetNspc(NspcKey, NspcData))
		{
			if(!NspcData.bNonDissipative) continue;

			item.Format(_T("%d\t%d\t%s(NRS)\t%s\n"),
				nRangeStart+NspcKey, nRangeStart+NspcData.LoadCaseId, NspcData.LoadCaseName,
				NspcData.Description);
			szLoadCaseList += item;
			nTempLen = pDC->GetTextExtent(NspcData.LoadCaseName+_T("(NRS)"));
			if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
			nTempLen = pDC->GetTextExtent(NspcData.Description);
			if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
		}
	}

	//D_LCOM_HISTORY		3 : 시간 이력 하중 조건
	T_THIS_K ThisKey;
	T_THIS_D ThisData;
	nRangeStart = D_STLD_MAXNUM*D_LCOM_HISTORY;
#ifdef _MEC
	CArray<T_THIS_K, T_THIS_K> rThisKeyList;
	m_pDoc->GetThisKeyListAppendDummyInit(rThisKeyList);
	pos = m_pDoc->m_pAttrCtrl->GetStartThis();
	for (int i = 0; i < rThisKeyList.GetSize(); i++)
	{
		ThisKey = rThisKeyList[i];
		m_pDoc->m_pAttrCtrl->GetThis(rThisKeyList[i], ThisData);
		item.Format(_T("%d\t%d\t%s(") + _LS(IDS_DB_D_LOADCASE_HISTORY) + _T(")\t%s\n"),
			nRangeStart + ThisKey, nRangeStart + ThisData.LoadCaseId, ThisData.LoadCaseName,
			ThisData.Description);
		szLoadCaseList += item;
		nTempLen = pDC->GetTextExtent(ThisData.LoadCaseName + _T("(TH)"));
		if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
		nTempLen = pDC->GetTextExtent(ThisData.Description);
		if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;
	}
#else
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
#endif
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
	case D_MOVE_CODE_JAPAN_RAIL:
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
	/*
	int nMovingType = CProduct::GetMovingType();
	if (nMovingType == D_PRODUCT_MOVING_ORG || nMovingType == D_PRODUCT_MOVING_US)
	{
		if(nMovingType == D_PRODUCT_MOVING_US && CProduct::IsIndia())
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
		else
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
	}
	else if (nMovingType == D_PRODUCT_MOVING_JP)
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
	else if (nMovingType == D_PRODUCT_MOVING_CH)
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
	else
	{
		ASSERT(0);
	}
	*/

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
			if(!m_pDoc->m_pAttrCtrl->GetSplc(EspcKey, SplcData)) {ASSERT(0); continue;}
			if(!SplcData.bConsiderEccen) continue;

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

	//D_LCOM_ECCENSPEC_ND  : ND용 우발편심 응답 스펙트럼 하중 조건
	nRangeStart = D_STLD_MAXNUM*D_LCOM_ECCENSPEC_ND;
	m_pDoc->m_pAttrCtrl->GetNspcKeyList(aNspcKey);
	nNspcNum = aNspcKey.GetSize();
	for (int n = 0; n < nNspcNum; n++)
	{
		NspcKey = aNspcKey[n];
		if (m_pDoc->m_pAttrCtrl->GetNspc(NspcKey, NspcData))
		{
			if(!NspcData.bConsiderEccen) continue;
			if(!NspcData.bNonDissipative) continue;

			item.Format(_T("%d\t%d\t%s(NES)\t%s\n"),
				nRangeStart+NspcKey, nRangeStart+NspcData.LoadCaseId, NspcData.LoadCaseName,
				NspcData.Description);
			szLoadCaseList += item;
			nTempLen = pDC->GetTextExtent(NspcData.LoadCaseName+_T("(NES)"));
			if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
			nTempLen = pDC->GetTextExtent(NspcData.Description);
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
		LcomKey = _tstol(m_sKey);
		if (m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, LcomKey, LcomData)) 
		{
			//nNum = LcomData.LoadCombId-1;
			int i = 0;
			for (i = 0; i < aLcomKey.GetSize(); i++)
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
	ReleaseDC(pDC);
}

BOOL CCMLoadCombSubGrid::IsMainAppendRow()
{
	return m_sKey == _LS(IDS_WG_CMD__ADDD__Append_Row);
}

BOOL CCMLoadCombSubGrid::IsMainNewRow()
{
	return m_sKey == _LS(IDS_WG_CMD__ADDD__New_Row);
}

void CCMLoadCombSubGrid::SetData(CString sKey)
{
	m_sKey = sKey;

	SetLoadCaseColumnStyle(1);
	if (IsMainAppendRow() || IsMainNewRow())
	{
		if (GetRowCount() == 1) return; // already cleared
		//SetRecordCount(0);
		DeleteAllRecords();
		return;
	}

	T_LCOM_K key;
	T_LCOM_D data;

	// load data and fill each row
	key = _tstol(m_sKey);
	m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, key, data);

	int nCount = m_pDoc->m_pAttrCtrl->GetCountLcomCase(m_nLcomType, data.LoadCombName);
	if (GetRowCount() == 1 && nCount == 0) return;  // already cleared

	BOOL bOldLock = LockUpdate(TRUE);
	DeleteAllRecords();
	SetRecordCount(nCount);
	CString sValue;
	int nRangeStart;

	CMap<T_SGLD_K, T_SGLD_K, int, int> mapErect;
	m_pDoc->m_pAttrCtrl->GetErectionSelected(mapErect);

	for (int i = 0; i < nCount; i++)
	{
#if defined(_CIVIL)
		if (data.aCombination[i].AnalType == D_LCOM_STAGE)
		{
			unsigned int SgldKey = data.aCombination[i].LoadCaseKey;
			// Erection Load 및 CS로 정의된 것만 보여주기(Tendon 제외).
			if (SgldKey >= D_SGLD_ER1_CVL && SgldKey < D_SGLD_SUM_CVL)
			{
				int nTemp = 0;
				if (!mapErect.Lookup(SgldKey, nTemp)) continue;
			}
		}
#else
		if (data.aCombination[i].AnalType == D_LCOM_STAGE)
		{
			unsigned int SgldKey = data.aCombination[i].LoadCaseKey;
			// Erection Load 및 CS로 정의된 것만 보여주기(Tendon 제외).
			if (SgldKey >= D_SGLD_ER1_GEN && SgldKey < D_SGLD_SUM_GEN)
			{
				int nTemp = 0;
				if (!mapErect.Lookup(SgldKey, nTemp)) continue;
			}
		}
#endif
		nRangeStart = D_STLD_MAXNUM*data.aCombination[i].AnalType;
		sValue.Format(_T("%d"), nRangeStart+data.aCombination[i].LoadCaseKey);
		SetExpressionRowCol(i+1, 1, sValue);
		sValue.Format(_T("%f"), data.aCombination[i].Factor);
		SetExpressionRowCol(i+1, 2, sValue);

		if(data.aCombination[i].AnalType==D_LCOM_ECCENSPEC)
		{
			T_SPLC_D SplcD;
			if(!m_pDoc->m_pAttrCtrl->GetSplc(data.aCombination[i].LoadCaseKey, SplcD)) {ASSERT(0); continue;}
			if(!SplcD.bConsiderEccen) HideRows(i+1, i+1);
		}
		else if(data.aCombination[i].AnalType==D_LCOM_SPECTRUM_ND)
		{
			T_SPLC_D SplcD;
			if(!m_pDoc->m_pAttrCtrl->GetSplc(data.aCombination[i].LoadCaseKey, SplcD)) {ASSERT(0); continue;}
			if(!SplcD.bNonDissipative) HideRows(i+1, i+1);
		}
		else if(data.aCombination[i].AnalType==D_LCOM_ECCENSPEC_ND)
		{
			T_SPLC_D SplcD;
			if(!m_pDoc->m_pAttrCtrl->GetSplc(data.aCombination[i].LoadCaseKey, SplcD)) {ASSERT(0); continue;}
			if(!SplcD.bConsiderEccen || !SplcD.bNonDissipative) HideRows(i+1, i+1);
		}
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

BOOL CCMLoadCombSubGrid::OnAddNewRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnAddNewRecord(nRow);

	// MQC 6867   ////////////////////////////////////////////////////
	CString value;
	value = GetValueRowCol(nRow, 1);
	value.TrimLeft();
	value.TrimRight();
	
	if(value.IsEmpty()) return FALSE;
	//////////////////////////////////////////////////////////////////

	//SetRecordModified();
	return bRetVal;
}

BOOL CCMLoadCombSubGrid::OnModifyRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnModifyRecord(nRow);

	// MQC 6867   ////////////////////////////////////////////////////
	CString value;
	value = GetValueRowCol(nRow, 1);
	value.TrimLeft();
	value.TrimRight();

	if(value.IsEmpty()) return FALSE;
	//////////////////////////////////////////////////////////////////

	//SetRecordModified();
	return bRetVal;
}

BOOL CCMLoadCombSubGrid::OnDeleteRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnDeleteRecord(nRow);
	SetRecordModified();
	return bRetVal;
}

BOOL CCMLoadCombSubGrid::OnDeleteRecords(CRowColArray &awRows)
{
	BOOL bRetVal = CTBBrowserWnd::OnDeleteRecords(awRows);
	SetRecordModified();
	return bRetVal;
}

BOOL CCMLoadCombSubGrid::OnCanceledAddNew()
{
	BOOL bRetVal = CTBBrowserWnd::OnCanceledAddNew();
	return bRetVal;
}

BOOL CCMLoadCombSubGrid::OnCanceledModify()
{
	BOOL bRetVal = CTBBrowserWnd::OnCanceledModify();
	return bRetVal;
}

BOOL CCMLoadCombSubGrid::OnInsertEmptyRecord(ROWCOL nRow)
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

	if (ncCol == 1) // Load Case
		SetExpressionRowCol(nRow, 2, _T("1.0"));  // set default Factor value
	else if (ncCol == 2) // Factor
	{
		CString sKey;
		/*
		T_STLD_K key;
		T_STLD_D data;
		POSITION pos = m_pDoc->m_pAttrCtrl->GetStartStld();
		m_pDoc->m_pAttrCtrl->GetNextStld(pos, key, data);
		sKey.Format(_T("%d"), key);
		*/
		CGXStyle style;
		VERIFY(GetColStyle(1, style));
		CString szChoiceList = style.GetChoiceList();
		if (szChoiceList == _T("")) return TRUE;  // ChoiceList없으면 그냥 리턴

		int nPos = szChoiceList.Find('\t'); // 첫번째 분리자를 찾는다.(key)
		VERIFY(nPos != -1);
		sKey = szChoiceList.Left(nPos);
		SetExpressionRowCol(nRow, 1, sKey); // set default Load Case value
	}

	return TRUE;
}

// 셀이 수정되면 EditMode로 들어간다.
// 주1:MainGrid가 AppendRow에 있을 때는 셀이 수정되면 먼저 AddNew가 호출되어야 한다.
// 이 때는 현재 Record의 위치가 변경되었으므로 OnChangeCurrentRecord가 호출되어야
// 하는데 셀이 수정된 후 SetRecordModified가 호출되면 수정된 내용이 지워지므로 
// SetRecordModified를 먼저 수행한다.
void CCMLoadCombSubGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	SetRecordModified();  // 먼저 호출되어야 한다. 주1 참조
	CTBBrowserWnd::OnModifyCell(nRow, nCol);
}

void CCMLoadCombSubGrid::SetRecordModified()
{
	if (m_pMainGrid->IsEdit()) return;

	ROWCOL ncRow, ncCol;
	m_pMainGrid->GetCurrentCell(ncRow, ncCol);
	if (m_pMainGrid->IsAppendRow(ncRow)) 
	{
		m_pMainGrid->AddNew(ncRow);
		m_pMainGrid->OnChangeCurrentRecord(ncRow);
	}
	else m_pMainGrid->Edit(ncRow);
}

void CCMLoadCombSubGrid::OnChangeStld()
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
	T_NSPC_D nspcData;
	T_GILC_D gilcData;
	T_LCOM_D lcomData;

	int nAnalType;
	T_STLD_K nKey;
	int nSize = GetRowCount()-1;
	for (int i = nSize-1; i >= 0; i--)
	{
		nKey = _tstol(GetValueRowCol(i+1, 1));
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
		case D_LCOM_SPECTRUM_ND:
			if (!m_pDoc->m_pAttrCtrl->GetNspc(nKey, nspcData)) 
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
				case D_MOVE_CODE_JAPAN_RAIL:
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
					break;
				case D_MOVE_CODE_FRANCE:
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldfr(nKey, mvldfrData)) 
							DeleteRecord(i+1);
					}
					break;
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
				/*
				int nMovingType = CProduct::GetMovingType();
				if (nMovingType == D_PRODUCT_MOVING_ORG || nMovingType == D_PRODUCT_MOVING_US)
				{
					if(nMovingType == D_PRODUCT_MOVING_US && CProduct::IsIndia())
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldid(nKey, mvldidData)) 
							DeleteRecord(i+1);
					}
					else
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvld(nKey, mvldData)) 
							DeleteRecord(i+1);
					}
				}
				else if (nMovingType == D_PRODUCT_MOVING_JP)
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldjp(nKey, mvldjpData)) 
						DeleteRecord(i+1);
				}
				else if (nMovingType == D_PRODUCT_MOVING_CH)
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldch(nKey, mvldchData)) 
						DeleteRecord(i+1);
				}
				else 
				{
					ASSERT(0);
				}
			}
			*/
			break;
		case D_LCOM_SETTLEMENT:
			if (!m_pDoc->m_pAttrCtrl->GetSmlc(nKey, smlcData)) 
				DeleteRecord(i+1);
			break;
		case D_LCOM_STAGE:
			if (!m_pDoc->m_pAttrCtrl->GetSgld(nKey, sgldData))
				DeleteRecord(i+1);
			break;
		case D_LCOM_ECCENSPEC:
			if (!m_pDoc->m_pAttrCtrl->GetEspc(nKey, espcData)) 
				DeleteRecord(i+1);
			break;
		case D_LCOM_ECCENSPEC_ND:
			if (!m_pDoc->m_pAttrCtrl->GetNspc(nKey, nspcData)) 
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
			if (!m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, nKey, lcomData))
				DeleteRecord(i+1);
			break;
		}
	}

	SetLoadCaseColumnStyle(1);

	// 현재 셀을 다시 초기화 시킨다.
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return;
	CGXControl* pControl = this->GetControl(ncRow, ncCol);
	if (pControl) pControl->Init(ncRow, ncCol);
}

void CCMLoadCombSubGrid::GetCombItem(T_LCOM_D &data)
{
	int nAnalType;
	T_STLD_K nKey;
	data.aCombination.SetSize(GetRowCount()-1);
	for (int i = 0; i < GetRowCount()-1; i++)
	{
		nKey = _tstol(GetValueRowCol(i+1, 1));
		nAnalType = nKey / D_STLD_MAXNUM;
		data.aCombination[i].LoadCaseKey = nKey % D_STLD_MAXNUM;
		data.aCombination[i].Factor = _tstof(GetValueRowCol(i+1, 2));
		data.aCombination[i].AnalType = nAnalType;
	}
}

BOOL CCMLoadCombSubGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	// Factor가 실수(정수포함)로 입력되어 있는지 검사한다.
	if(nCol == 1)
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
		T_NSPC_D nspcData;
		T_GILC_D gilcData;
		T_LCOM_D lcomData;

		int nAnalType;
		T_STLD_K nKey;

		nKey = _tstol(GetValueRowCol(nRow, nCol));
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
		case D_LCOM_SPECTRUM_ND:
			if (!m_pDoc->m_pAttrCtrl->GetNspc(nKey, nspcData)) 
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
				case D_MOVE_CODE_JAPAN_RAIL:
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
					break;
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
				/*
				int nMovingType = CProduct::GetMovingType();
				if (nMovingType == D_PRODUCT_MOVING_ORG || nMovingType == D_PRODUCT_MOVING_US)
				{
					if(nMovingType == D_PRODUCT_MOVING_US && CProduct::IsIndia())
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvldid(nKey, mvldidData)) 
							bError = TRUE;
					}
					else
					{
						if (!m_pDoc->m_pAttrCtrl->GetMvld(nKey, mvldData)) 
							bError = TRUE;
					}
				}
				else if (nMovingType == D_PRODUCT_MOVING_JP)
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldjp(nKey, mvldjpData)) 
						bError = TRUE;
				}
				else if (nMovingType == D_PRODUCT_MOVING_CH)
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldch(nKey, mvldchData)) 
						bError = TRUE;
				}
				else 
				{
					ASSERT(0);
				}
			}
			*/
			break;
		case D_LCOM_SETTLEMENT:
			if (!m_pDoc->m_pAttrCtrl->GetSmlc(nKey, smlcData)) 
				bError = TRUE;
			break;
		case D_LCOM_STAGE:
			if (!m_pDoc->m_pAttrCtrl->GetSgld(nKey, sgldData))
				bError = TRUE;
			break;
		case D_LCOM_ECCENSPEC:
			if (!m_pDoc->m_pAttrCtrl->GetEspc(nKey, espcData)) 
				bError = TRUE;
			break;
		case D_LCOM_ECCENSPEC_ND:
			if (!m_pDoc->m_pAttrCtrl->GetNspc(nKey, nspcData)) 
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
	else if (nCol == 2)
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
	}
	return TRUE;
}

