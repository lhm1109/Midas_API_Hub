// RatingCaseSubGrid.cpp: implementation of the CRatingCaseSubGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingCaseSubGrid.h"

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

CRatingCaseSubGrid::CRatingCaseSubGrid()
{
	//m_pMainGrid = 0;
	m_sKey = _LS(IDS_WG_CMD__ADDD__None);
	m_pDoc = 0;
	m_nLcomType = D_LCOMTYPE_GENERAL;
	m_nLCtype = D_LCOM_CB_GENERAL;
	m_csPostfix = _T("CB");

	m_aTitle.Add(_LS(IDS_DGN_RATING_LOAD_CASE));//_T("Load Cases")
}

CRatingCaseSubGrid::~CRatingCaseSubGrid()
{

}

#define CWnd CTBBrowserWnd
BEGIN_MESSAGE_MAP(CRatingCaseSubGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CRatingCaseSubGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRatingCaseSubGrid message handlers
void CRatingCaseSubGrid::Initialize(CDBDoc* pDoc)
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

	SetColWidth(1, 1, globalUtils.ScaleByDPI(95/scale));
	
	// column의 이동을 금지시킨다.
	GetParam()->EnableMoveCols(FALSE);
	// row resize 금지
	GetParam()->EnableTrackRowHeight(FALSE); 

	// 데이타 설정
	//MakeItemEx();
	LockUpdate(FALSE);
}

// Column별 헤더 이름을 설정한다.
void CRatingCaseSubGrid::SetHeaderTitle()
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
void CRatingCaseSubGrid::SetLoadCaseColumnStyle(ROWCOL nCol)
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
		//if (Data.LoadCaseType == _T("CS")) continue;  // 시공타입은 제외
		item.Format(_T("%d\t%d\t%s(ST)\t%s\n"), 
			nRangeStart+Key, nRangeStart+Data.LoadCaseId, Data.LoadCaseName,
			Data.Description);
		szLoadCaseList += item;
		nTempLen = pDC->GetTextExtent(Data.LoadCaseName+_T("(ST)"));
		if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
		nTempLen = pDC->GetTextExtent(Data.Description);
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

		if (SgldKey == D_SGLD_CP_CVL || SgldKey == D_SGLD_SP_CVL || SgldKey == D_SGLD_SUM_CVL) continue; // Creep Primary, Shrinkage Primary, Summation 는 뺀다.

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

BOOL CRatingCaseSubGrid::IsMainAppendRow()
{
	return m_sKey == _LS(IDS_WG_CMD__ADDD__Append_Row);
}

BOOL CRatingCaseSubGrid::IsMainNewRow()
{
	return m_sKey == _LS(IDS_WG_CMD__ADDD__New_Row);
}

void CRatingCaseSubGrid::SetData(T_RTLD_D &Data)
{
	m_Data = Data;

	SetLoadCaseColumnStyle(1);
	if (IsMainAppendRow() || IsMainNewRow())
	{
		if (GetRowCount() == 1) return; // already cleared
		
		DeleteAllRecords();
		SetRecordCount(m_Data.Combination.GetSize()+1);
		return;
	}
	
	BOOL bOldLock = LockUpdate(TRUE);
	DeleteAllRecords();
	SetRecordCount(m_Data.Combination.GetSize());
	
	CString sValue;
	int nRangeStart;

	CMap<T_SGLD_K, T_SGLD_K, int, int> mapErect;
	m_pDoc->m_pAttrCtrl->GetErectionSelected(mapErect);

	for (int i = 0; i < m_Data.Combination.GetSize(); i++)
	{
		if (m_Data.Combination[i].AnalType == D_LCOM_STAGE)
		{
			unsigned int SgldKey = m_Data.Combination[i].LoadCaseKey;
			// Erection Load 및 CS로 정의된 것만 보여주기(Tendon 제외).
			if (SgldKey >= D_SGLD_ER1_CVL && SgldKey < D_SGLD_SUM_CVL)
			{
				int nTemp = 0;
				if (!mapErect.Lookup(SgldKey, nTemp)) continue;
			}
		}

		nRangeStart = D_STLD_MAXNUM*m_Data.Combination[i].AnalType;
		sValue.Format(_T("%d"), nRangeStart+m_Data.Combination[i].LoadCaseKey);
		SetExpressionRowCol(i+1, 1, sValue);
		//sValue.Format(_T("%f"), data.Combination[i].Factor);
		//SetExpressionRowCol(i+1, 2, sValue);
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

/*
void CRatingCaseSubGrid::SetData(CString sKey)
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
	key = _ttol(m_sKey);
	m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, key, data);

	int nCount = m_pDoc->m_pAttrCtrl->GetCountLcomCase(m_nLcomType, data.LoadCombName);
	if (GetRowCount() == 1 && nCount == 0) return;  // already cleared

	BOOL bOldLock = LockUpdate(TRUE);
	DeleteAllRecords();
	SetRecordCount(nCount);
	CString sValue;
	int nRangeStart;

	for (int i = 0; i < nCount; i++)
	{
		nRangeStart = D_STLD_MAXNUM*MainData.Combination[i].AnalType;
		sValue.Format(_T("%d"), nRangeStart+MainData.Combination[i].LoadCaseKey);
		SetExpressionRowCol(i+1, 1, sValue);
		//sValue.Format(_T("%f"), data.Combination[i].Factor);
		//SetExpressionRowCol(i+1, 2, sValue);
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}
*/

BOOL CRatingCaseSubGrid::OnAddNewRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnAddNewRecord(nRow);
	//SetRecordModified();
	return bRetVal;
}

BOOL CRatingCaseSubGrid::OnModifyRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnModifyRecord(nRow);
	//SetRecordModified();
	return bRetVal;
}

BOOL CRatingCaseSubGrid::OnDeleteRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnDeleteRecord(nRow);
	//SetRecordModified();
	return bRetVal;
}

BOOL CRatingCaseSubGrid::OnDeleteRecords(CRowColArray &awRows)
{
	BOOL bRetVal = CTBBrowserWnd::OnDeleteRecords(awRows);
	//SetRecordModified();
	return bRetVal;
}

BOOL CRatingCaseSubGrid::OnCanceledAddNew()
{
	BOOL bRetVal = CTBBrowserWnd::OnCanceledAddNew();
	return bRetVal;
}

BOOL CRatingCaseSubGrid::OnCanceledModify()
{
	BOOL bRetVal = CTBBrowserWnd::OnCanceledModify();
	return bRetVal;
}

BOOL CRatingCaseSubGrid::OnInsertEmptyRecord(ROWCOL nRow)
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

	/*
	if (ncCol == 1) // Load Case
		//SetExpressionRowCol(nRow, 2, _T("1.0"));  // set default Factor value
	
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
	}*/

	return TRUE;
}

// 셀이 수정되면 EditMode로 들어간다.
// 주1:MainGrid가 AppendRow에 있을 때는 셀이 수정되면 먼저 AddNew가 호출되어야 한다.
// 이 때는 현재 Record의 위치가 변경되었으므로 OnChangeCurrentRecord가 호출되어야
// 하는데 셀이 수정된 후 SetRecordModified가 호출되면 수정된 내용이 지워지므로 
// SetRecordModified를 먼저 수행한다.
void CRatingCaseSubGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	//SetRecordModified();  // 먼저 호출되어야 한다. 주1 참조
	CTBBrowserWnd::OnModifyCell(nRow, nCol);
}

/*
void CRatingCaseSubGrid::SetRecordModified()
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
*/

void CRatingCaseSubGrid::OnChangeStld()
{
	// 지워진 하중들은 제거한다.
	T_STLD_D stldData;
	T_SGLD_D sgldData;
	
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
		case D_LCOM_STAGE:
			if (!m_pDoc->m_pAttrCtrl->GetSgld(nKey, sgldData))
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

void CRatingCaseSubGrid::GetCombItem(T_RTLD_D &data)
{
	data.Combination.RemoveAll();

	T_STLD_K nKey;
	T_LCOM_BASE LcomBase;

	for (int i = 0; i < GetRowCount()-1; i++)
	{
		nKey = _ttol(GetValueRowCol(i+1, 1));
		
		LcomBase.AnalType = nKey / D_STLD_MAXNUM;
		LcomBase.Factor = 1.0;
		LcomBase.LoadCaseKey = nKey % D_STLD_MAXNUM;

		data.Combination.Add(LcomBase);
	}
}

BOOL CRatingCaseSubGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
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