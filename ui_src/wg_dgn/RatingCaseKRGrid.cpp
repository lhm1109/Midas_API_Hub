// RatingCaseKRGrid.cpp: implementation of the CRatingCaseKRGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingCaseKRGrid.h"

#include "..\wg_db\wg_db_AttrCtrl.h"

#include "RatingCaseMainGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRatingCaseKRGrid::CRatingCaseKRGrid()
{
	//m_pMainGrid = 0;	
	m_sKey = _LS(IDS_WG_CMD__ADDD__None);
	m_pDoc = 0;
	m_nLcomType = D_LCOMTYPE_GENERAL;
	m_nLCtype = D_LCOM_CB_GENERAL;
	m_csPostfix = _T("CB");

	m_aTitle.Add(_LS(IDS_DGN_RATING_LOAD_CASE));//_T("Load Cases")
	m_aTitle.Add(_LS(IDS_DGN_RATING_FACTOR));//_T("Factor")
}

CRatingCaseKRGrid::~CRatingCaseKRGrid()
{

}

#define CWnd CTBBrowserWnd
BEGIN_MESSAGE_MAP(CRatingCaseKRGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CRatingCaseKRGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRatingCaseKRGrid message handlers
void CRatingCaseKRGrid::Initialize(CDBDoc* pDoc)
{
	CTBBrowserWnd::Initialize();
	m_pDoc = pDoc;

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(m_aTitle.GetSize());
	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(8))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// 컬럼 스타일 변경

	// Loadcase list
	SetLoadCaseColumnStyle(1);

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// always show vertical scrollbar
	//SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	 // column size 설정
#if defined(_US) || defined(_RUS)
	double scale = 1.154;
#else
	double scale = 1.0;
#endif

	SetColWidthByDPI(1, 1, 138/scale);
	SetColWidthByDPI(1, 2, 138/scale);
	
	// column의 이동을 금지시킨다.
	GetParam()->EnableMoveCols(FALSE);
	// row resize 금지
	GetParam()->EnableTrackRowHeight(FALSE); 

	// 데이타 설정
	//MakeItemEx();
	LockUpdate(FALSE);
}

// Column별 헤더 이름을 설정한다.
void CRatingCaseKRGrid::SetHeaderTitle()
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
void CRatingCaseKRGrid::SetLoadCaseColumnStyle(ROWCOL nCol)
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

BOOL CRatingCaseKRGrid::IsMainAppendRow()
{
	return m_sKey == _LS(IDS_WG_CMD__ADDD__Append_Row);
}

BOOL CRatingCaseKRGrid::IsMainNewRow()
{
	return m_sKey == _LS(IDS_WG_CMD__ADDD__New_Row);
}

void CRatingCaseKRGrid::SetData(T_RKLC_D &data)
{
	m_Data = data;

	SetLoadCaseColumnStyle(1);
	if (IsMainAppendRow() || IsMainNewRow())
	{
		if (GetRowCount() == 1) return; // already cleared
		
		DeleteAllRecords();
		SetRecordCount(m_Data.arLoadComb.GetSize()+1);
		return;
	}

	BOOL bOldLock = LockUpdate(TRUE);
	DeleteAllRecords();
	SetRecordCount(m_Data.arLoadComb.GetSize());  
	CString sValue;
	int nRangeStart;

	CMap<T_SGLD_K, T_SGLD_K, int, int> mapErect;
	m_pDoc->m_pAttrCtrl->GetErectionSelected(mapErect);

	for (int i = 0; i < m_Data.arLoadComb.GetSize(); i++)
	{
		if (m_Data.arLoadComb[i].AnalType == D_LCOM_STAGE)
		{
			unsigned int SgldKey = m_Data.arLoadComb[i].LoadCaseKey;
			// Erection Load 및 CS로 정의된 것만 보여주기(Tendon 제외).
			if (SgldKey >= D_SGLD_ER1_CVL && SgldKey < D_SGLD_SUM_CVL)
			{
				int nTemp = 0;
				if (!mapErect.Lookup(SgldKey, nTemp)) continue;
			}
		}

		nRangeStart = D_STLD_MAXNUM*m_Data.arLoadComb[i].AnalType;
		sValue.Format(_T("%d"), nRangeStart+m_Data.arLoadComb[i].LoadCaseKey);
		SetExpressionRowCol(i+1, 1, sValue);
		sValue.Format(_T("%f"), m_Data.arLoadComb[i].Factor);
		SetExpressionRowCol(i+1, 2, sValue);
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

BOOL CRatingCaseKRGrid::OnAddNewRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnAddNewRecord(nRow);
	
	return bRetVal;
}

BOOL CRatingCaseKRGrid::OnModifyRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnModifyRecord(nRow);
	
	return bRetVal;
}

BOOL CRatingCaseKRGrid::OnDeleteRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnDeleteRecord(nRow);
	
	return bRetVal;
}

BOOL CRatingCaseKRGrid::OnDeleteRecords(CRowColArray &awRows)
{
	BOOL bRetVal = CTBBrowserWnd::OnDeleteRecords(awRows);
	
	return bRetVal;
}

BOOL CRatingCaseKRGrid::OnCanceledAddNew()
{
	BOOL bRetVal = CTBBrowserWnd::OnCanceledAddNew();
	return bRetVal;
}

BOOL CRatingCaseKRGrid::OnCanceledModify()
{
	BOOL bRetVal = CTBBrowserWnd::OnCanceledModify();
	return bRetVal;
}

BOOL CRatingCaseKRGrid::OnInsertEmptyRecord(ROWCOL nRow)
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
		
		T_STLD_K key;
		T_STLD_D data;
		POSITION pos = m_pDoc->m_pAttrCtrl->GetStartStld();
		m_pDoc->m_pAttrCtrl->GetNextStld(pos, key, data);
		sKey.Format(_T("%d"), key);
		
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


void CRatingCaseKRGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	
	CTBBrowserWnd::OnModifyCell(nRow, nCol);
}



void CRatingCaseKRGrid::OnChangeStld()
{
	// 지워진 하중들은 제거한다.
	T_STLD_D stldData;
	T_SPLC_D splcData;
	T_THIS_D thisData;  
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
		}
	}

	SetLoadCaseColumnStyle(1);

	// 현재 셀을 다시 초기화 시킨다.
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return;
	CGXControl* pControl = this->GetControl(ncRow, ncCol);
	if (pControl) pControl->Init(ncRow, ncCol);
}

void CRatingCaseKRGrid::GetCombItem(T_RKLC_D &data)
{
	data.arLoadComb.RemoveAll();

	T_STLD_K nKey;
	T_LCOM_BASE LcomBase;
	int nAnalType;

	for (int i = 0; i < GetRowCount()-1; i++)
	{
		nKey = _ttol(GetValueRowCol(i+1, 1));
		nAnalType = nKey / D_STLD_MAXNUM;
		LcomBase.AnalType = nAnalType;
		LcomBase.Factor = _tstof(GetValueRowCol(i+1, 2));
		LcomBase.LoadCaseKey = nKey % D_STLD_MAXNUM;

		data.arLoadComb.Add(LcomBase);
	}
}

BOOL CRatingCaseKRGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	/*
	// Factor가 실수(정수포함)로 입력되어 있는지 검사한다.
	if (nCol == 2)
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
	*/
	return TRUE;
}