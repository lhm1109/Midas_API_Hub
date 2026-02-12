// LoadCaseUnderMainGrid.cpp: implementation of the CLoadCaseUnderMainGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "LoadCaseUnderMainGrid.h"
#include "CmdAutoLoadUnderGroundDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define D_MAX_COL 3
enum ColumnIndex
{
	COLINDEX_NO		= 1,
	COLINDEX_LOAD	= 2, 
	COLINDEX_DIR	= 3,
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoadCaseUnderMainGrid::CLoadCaseUnderMainGrid()
{
	m_nWndWidth = 0;

	AddColInfo(_LS(IDS_TB_POSTDISP_No_),	t_integer, 0);
	AddColInfo(_LS(IDS_TB_BMLD_LoadCase),	t_string,  0);
	AddColInfo(_LS(IDS_TB_BMLD_Direction),	t_string,  0);

	m_aPrimaryKey.Add(0);
	m_aData.RemoveAll();
	m_pDoc = CDBDoc::GetDocPoint();
}

CLoadCaseUnderMainGrid::~CLoadCaseUnderMainGrid()
{

}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CLoadCaseUnderMainGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CLoadCaseUnderMainGrid)
	ON_WM_LBUTTONDOWN()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Interface
void CLoadCaseUnderMainGrid::Initialize(CWnd* pParent, int nWidth)
{
	m_pParent = pParent;
	m_nWndWidth = nWidth;

	SetTitle();

	T_UNIT_SYSTEM us;
	CTBCommon::Initialize(m_pDoc, us);

	SetRowHeightByDPI(0, 0, 20);
	SetHeaderTitle(us, TRUE);
	
	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE); 
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
												(CGXStyle) pProp->sInvertNoBorder);

	LockUpdate(TRUE);

	SetFrozenCols(1, 1);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont().SetSize(9))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont().SetBold(FALSE)));
	ChangeRowHeaderStyle(CGXStyle()
		.SetFont(CGXFont().SetBold(FALSE))
		.SetHorizontalAlignment(DT_RIGHT)
		.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(FALSE);

	SetStyleRange(CGXRange().SetCols(COLINDEX_NO), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(0)
		.SetEnabled(FALSE));

	// 컬럼 스타일 변경
	SetComboColumn();
	
	SetScrollBarMode(SB_HORZ, gxnDisabled);
	SetScrollBarMode(SB_VERT, gxnEnabled);

	LockUpdate(FALSE);
	Redraw();

	LockUpdate(FALSE);
}

void CLoadCaseUnderMainGrid::SetTitle()
{
	m_aTitle.RemoveAll();
	m_aTitle.Add(_LS(IDS_TB_POSTDISP_No_));
	m_aTitle.Add(_LS(IDS_TB_BMLD_LoadCase));
	m_aTitle.Add(_LS(IDS_TB_BMLD_Direction));
}

void CLoadCaseUnderMainGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	if(bInit == FALSE)    return;
	
	int nSizeNo		= (int)m_nWndWidth*0.15;
	int nSizeLoad	= (int)m_nWndWidth*0.33;
	int nSizeDir	= (int)m_nWndWidth*0.33;

	int aSize[3] = { nSizeNo, nSizeLoad, nSizeDir };
	
	aSize[0]=nSizeNo;
	aSize[1]=nSizeLoad; 
	aSize[2]=nSizeDir;
	
	CString csTitle;
	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
	{
		csTitle = m_aTitle[i];
		SetStyleRange(CGXRange(0, i+1), CGXStyle().SetValue(csTitle).SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidth(i+1, i+1, aSize[i]);
	}
}

void CLoadCaseUnderMainGrid::Data2Grid()
{
	int nSize = m_aData.GetSize();
	SetRowCount(nSize+1);
		
	CString strLoadName = _T("");

	for (int i=0; i<nSize; i++)
	{
		if(!GetLoadCaseName(m_aData[i], strLoadName)) return;

		CString strNo;
		strNo.Format(_T("%d"), i+1);
		SetValueRange(CGXRange(i+1, COLINDEX_NO), strNo);
		SetValueRange(CGXRange(i+1, COLINDEX_LOAD), strLoadName);
		SetValueRange(CGXRange(i+1, COLINDEX_DIR), GetDirTitle(m_aData[i].nDir));
	}

	CString strTemp = _T("");
	CStringArray aStrTemp; aStrTemp.RemoveAll();
	aStrTemp.Add(strTemp);
	aStrTemp.Add(strTemp);
	aStrTemp.Add(strTemp);
	SetDataToRow(nSize+1, aStrTemp);
}

void CLoadCaseUnderMainGrid::Grid2Data()
{
	int nSize = GetRowCount() - 1;
	if(m_aData.GetSize() != nSize) { ASSERT(0); return; }

	OnTerminate(D_TB_SAVE_ALWAYS);

	for(int i = 0; i < nSize; i++)
	{
		CString strValue = GetValueRowCol(i+1, COLINDEX_LOAD);

		m_pDoc->m_pAttrCtrl2->GetLoadCaseKeyAndType4Uglc(strValue, m_aData[i].LoadCaseK, m_aData[i].nLoadCaseType);
		strValue = GetValueRowCol(i+1, COLINDEX_DIR);
		m_aData[i].nDir = GetDirByTitle(strValue);
	}
}

void CLoadCaseUnderMainGrid::SetData(T_UGLC_D& UglcD)
{
	m_aData.RemoveAll();

	int nSize = UglcD.aUglcBaseLoad.GetSize();
	if(nSize < 0) return;

	m_aData.Copy(UglcD.aUglcBaseLoad);
}

BOOL CLoadCaseUnderMainGrid::GetData(T_UGLC_D& UglcD)
{
	UglcD.aUglcBaseLoad.RemoveAll();
	UglcD.aUglcBaseLoad.Copy(m_aData);
	return TRUE;
}

void CLoadCaseUnderMainGrid::DeleteRecordAndData(ROWCOL nRow)
{
	DeleteRecord(nRow);
	int nindex = nRow-1;
	if(nindex < m_aData.GetCount())
	{
		m_aData.RemoveAt(nindex);
	}
}

//////////////////////////////////////////////////////////////////////
// Operation

//////////////////////////////////////////////////////////////////////
// Override

BOOL CLoadCaseUnderMainGrid::AddToDB(ROWCOL nRow)
{
	T_UGLC_BASE_LOAD data;

	CString strVal;
	strVal.Format(_T("%d"), nRow);
	SetValueRange(CGXRange(nRow, COLINDEX_NO), strVal);

	if(!GetValue(nRow, data)) return FALSE;

	int nPos = 0;
	if (FindData(data, nPos))
	{
		CString strLoadCaseName = _T("");
		if(!GetLoadCaseName(data, strLoadCaseName)) return FALSE;
		CString strMsg = _T("");
		strMsg.Format(_LS(IDS_CMD_ERR_SAME_LC_SAME_DIR), strLoadCaseName, GetDirTitle(data.nDir));
		AfxMessageBox(strMsg);
		return FALSE;
	}

	VERIFY(InsertRecord(nRow));
	m_aData.Add(data);
	if(!SetValue(nRow, data)) return FALSE;

	CString sKey = GetKeyForRow(nRow, COLINDEX_NO);
	((CCmdAutoLoadUnderGroundDlg*)m_pParent)->SetData_CheckBoxList(sKey, data.aSeisStldK, data.aStacStldK);
	return TRUE;
}

BOOL CLoadCaseUnderMainGrid::DeleteFromDB(ROWCOL nRow)
{
	DeleteRecordAndData(nRow);

	CString str;
	int nSize  = GetRowCount();
	for(int i=0; i<nSize-1; i++)
	{
		str.Format(_T("%d"), i+1);
		SetExpressionRowCol(i+1, 1, str);
	}

	return TRUE;
}

BOOL CLoadCaseUnderMainGrid::DeleteFromDB(CRowColArray& awRows)
{
	for (int i = awRows.GetSize() - 1; i >= 0; i--)
	{
		DeleteRecordAndData(awRows[i]);
	}

	CString str;
	int nSize  = GetRowCount();
	for(int i=0; i<nSize-1; i++)
	{
		str.Format(_T("%d"), i+1);
		SetExpressionRowCol(i+1, 1, str);
	}

	CheckCurrentRecordChanged();

	return TRUE;
}

BOOL CLoadCaseUnderMainGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));
	
	int nIndex = nRow - 1;
	ASSERT(nIndex >= 0 && nIndex < m_aData.GetSize());
	T_UGLC_BASE_LOAD data;
	GetValue(nRow, data);
	
	// 수정한다. 실패하면 에러 리턴
	return ModifyData(nIndex, nIndex, data);
}

CString CLoadCaseUnderMainGrid::GetNewKey()
{
	int nSize = GetRowCount() - 1;
	CString value;
	value.Format(_T("%d"), nSize);
	return value;
}

long CLoadCaseUnderMainGrid::GetCountData()
{
	return m_aData.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// CLD_CTRL_WIND_ShapeFactGrid Implementation
BOOL CLoadCaseUnderMainGrid::GetValue(ROWCOL nRow, T_UGLC_BASE_LOAD &data)
{
	ROWCOL nCol;
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		if(i==0) continue;
		ConvStrToData(i, str, data);
		if(str.IsEmpty()) return FALSE;
	}
	return TRUE;
}

BOOL CLoadCaseUnderMainGrid::SetValue(ROWCOL nRow, T_UGLC_BASE_LOAD &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, data, awCols);
}

BOOL CLoadCaseUnderMainGrid::SetValue(CGXData* pData, ROWCOL nRow, T_UGLC_BASE_LOAD &data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) str.Format(_T("%d"), nRow);
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	return TRUE;
}

void CLoadCaseUnderMainGrid::ConvStrToData(int i, CString& value, T_UGLC_BASE_LOAD& data)
{
	if(i == 0) return;
	else if(i == 1) 
	{
		m_pDoc->m_pAttrCtrl2->GetLoadCaseKeyAndType4Uglc(value, data.LoadCaseK, data.nLoadCaseType);
	}
	else if(i == 2)
	{
		data.nDir = GetDirByTitle(value);
	}
}

void CLoadCaseUnderMainGrid::ConvDataToStr(int i, T_UGLC_BASE_LOAD& data, CString& value)
{
	if(i == 0) return;
	else if (i == 1)
	{
		GetLoadCaseName(data, value);
	}
	else if(i == 2) value = GetDirTitle(data.nDir);
}

BOOL CLoadCaseUnderMainGrid::FindData(T_UGLC_BASE_LOAD& data, int& nPos)
{
	int nSize = m_aData.GetSize();
	int i = 0;
	for(i=0; i<nSize; i++)
	{
		if(m_aData[i].LoadCaseK != data.LoadCaseK) continue;
		if(m_aData[i].nDir != data.nDir) continue;
		nPos = i;
		return TRUE;
	}

	nPos = i;
	return FALSE;
}

CString CLoadCaseUnderMainGrid::GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol)
{
	CString sKey;

	if (IsAppendRow(ncRow)) sKey = _ULS(Append Row);
	else if (m_nEditMode == addnew) sKey = _ULS(New Row);
	else if (m_nEditMode == edit) sKey = GetValueRowColBeforeEdit(ncRow, nkCol);
	else sKey = GetValueRowCol(ncRow, nkCol);

	return sKey;
}

BOOL CLoadCaseUnderMainGrid::CheckCurrentRecordChanged()
{
	ROWCOL ncRow, nkCol;
	CString sKey;
	GetCurrentCell(ncRow, nkCol);
	GetColumnNoFor(GetKeyColID(), nkCol);
	sKey = GetKeyForRow(ncRow, COLINDEX_NO);
	if (((CCmdAutoLoadUnderGroundDlg*)m_pParent)->GetKey() != sKey) 
	{
		OnChangeCurrentRecord(ncRow);
		return TRUE;
	}
	return FALSE;
}

BOOL CLoadCaseUnderMainGrid::OnPasteFromClipboard(const CGXRange &range)
{
	return CTBCommon::OnPasteFromClipboard(range);
}

BOOL CLoadCaseUnderMainGrid::Copy()
{
	return CTBCommon::Copy();
}

BOOL CLoadCaseUnderMainGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{  
	return CGXGridWnd::OnEndEditing(nRow, nCol);
}

void CLoadCaseUnderMainGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	CString sKey = GetKeyForRow(nRow, COLINDEX_NO);
	//m_pSubGrid->SetData(sKey);
	if(m_aData.GetSize()<nRow) return;
	((CCmdAutoLoadUnderGroundDlg*)m_pParent)->SetData_CheckBoxList(sKey, m_aData[nRow-1].aSeisStldK, m_aData[nRow-1].aStacStldK);
}

BOOL CLoadCaseUnderMainGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	CheckCurrentRecordChanged();
	return TRUE;
}

void CLoadCaseUnderMainGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// Serial No 칼럼이 선택되어졌는지 본다.
	ROWCOL nhRow, nhCol, nkCol;
	int ht = HitTest(point, &nhRow, &nhCol);
	if (ht != GX_NOHIT && ht != GX_VERTLINE)
	{
		GetColumnNoFor(1, nkCol);
		if (nhCol == nkCol && IsFrozenCol(nhCol)) // Serial No column이면
		{
			// Row header의 rectangle을 얻는다.
			CRect r = CalcRectFromRowColEx(nhRow, 0, nhRow, 0);  
			point.x = (r.left+r.right)/2;
		}
	}

	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnLButtonDown(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
	return;
}

void CLoadCaseUnderMainGrid::SetComboColumn()
{
	CString strChoiceList = _T("");
	CString strTemp = _T("");
	
	// Static
	T_STLD_D StldD;
	T_STLD_K_LIST aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	int nCount = m_pDoc->m_pAttrCtrl->GetCountStld();
	if (nCount > 0)
	{
		for (int i = 0; i < nCount; i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], StldD)) { ASSERT(0); continue; }
			if(StldD.LoadCaseType != _T("E")) continue;

			m_pDoc->m_pAttrCtrl2->GetLoadCaseName4Uglc(aStldK[i], D_LOADCASE_STATIC, strTemp);
			strTemp+=_T("\n");
			strChoiceList += strTemp;
		}
	}

	// Spectrum.
	T_SPLC_D SplcD;
	CArray<T_SPLC_K, T_SPLC_K> aSplcK;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
	nCount = aSplcK.GetSize();
	if (nCount > 0)
	{
		for (int i = 0; i < nCount; i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetSplc(aSplcK[i], SplcD)) { ASSERT(0); continue; }

			m_pDoc->m_pAttrCtrl2->GetLoadCaseName4Uglc(aSplcK[i], D_LOADCASE_SPECTRUM, strTemp);
			strTemp+=_T("\n");
			strChoiceList += strTemp;
		}
	}
	
	SetStyleRange(CGXRange().SetCols(COLINDEX_LOAD), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(strChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetValue(_T(""))
		);

	strChoiceList = _T("");
	strTemp.Format(_T("%s\n"), _LS(IDS_CMD_PLUS));
	strChoiceList += strTemp;
	strTemp.Format(_T("%s\n"), _LS(IDS_CMD_MINUS));
	strChoiceList += strTemp;

	SetStyleRange(CGXRange().SetCols(COLINDEX_DIR), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(strChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetValue(_T(""))
		);
}

void CLoadCaseUnderMainGrid::SetDataToRow(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
		SetValueRange(CGXRange(nRow, i+1), raValue[i]);
}

BOOL CLoadCaseUnderMainGrid::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
		raValue[i] = GetEditingValue(nRow, i+1);

	return TRUE;
}

CString CLoadCaseUnderMainGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString strNum(_T(""));
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if(GetCurrentCell(ncRow, ncCol))
	{
		if(ncRow==nRow && ncCol==nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if(pControl && pControl->IsActive())
				bGet = pControl->GetValue(strNum);
		}
	}
	if(!bGet) strNum = GetValueRowCol(nRow, nCol);

	return strNum;
}

BOOL CLoadCaseUnderMainGrid::ModifyData(int nOldIndex, int nIndex, T_UGLC_BASE_LOAD &data)
{
	CArray<int, int> aOldIndex, aIndex;
	CArray<T_UGLC_BASE_LOAD, T_UGLC_BASE_LOAD&> aData;
	aOldIndex.Add(nOldIndex);
	aIndex.Add(nIndex);
	aData.Add(data);

	int nPos = 0;
	if (FindData(data, nPos))
	{
		if(nIndex != nPos)
		{
			CString strMsg = _T("");
			CString strName = _T("");
			if(!GetLoadCaseName(data, strName)) return FALSE;
			strMsg.Format(_LS(IDS_CMD_ERR_SAME_LC_SAME_DIR), strName, GetDirTitle(data.nDir));
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}

	m_aData[nIndex].LoadCaseK = data.LoadCaseK;
	m_aData[nIndex].nLoadCaseType = data.nLoadCaseType;
	m_aData[nIndex].nDir = data.nDir;

	return TRUE;
}

BOOL CLoadCaseUnderMainGrid::GetLoadCaseName(const T_UGLC_BASE_LOAD& BaseLoad, CString& strLoadCaseName)
{
	return m_pDoc->m_pAttrCtrl2->GetLoadCaseName4Uglc(BaseLoad.LoadCaseK, BaseLoad.nLoadCaseType, strLoadCaseName);
}

CString CLoadCaseUnderMainGrid::GetDirTitle(int nDir)
{
	return nDir==UG_PLUS ? _LS(IDS_CMD_PLUS) : _LS(IDS_CMD_MINUS);
}

int CLoadCaseUnderMainGrid::GetDirByTitle(const CString& strTitle)
{
	if(strTitle==_LS(IDS_CMD_PLUS)) return UG_PLUS;
	if(strTitle==_LS(IDS_CMD_MINUS)) return UG_MINUS;

	ASSERT(0);
	return -1;
}
