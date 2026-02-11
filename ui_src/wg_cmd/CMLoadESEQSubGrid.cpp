// CMLoadESEQSubGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadESEQSubGrid.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_common\wg_common_TBBrowserWndEx.h"

#include "CMLoadESEQMainGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQSubGrid
CCMLoadESEQSubGrid::CCMLoadESEQSubGrid()
{
	m_arTitle.Add(_LS(IDS_WG_CMD__ADDD__Type));
	m_arTitle.Add(_LS(IDS_WG_CMD__ADDD__Material));
	m_arTitle.Add(_LS(IDS_WG_CMD__ADDD__Story));

	m_arMatlType.Add(_LS(IDS_WG_CMD__ADDD__All));
	m_arMatlType.Add(_LS(IDS_WG_CMD__ADDD__Steel));
	m_arMatlType.Add(_LS(IDS_WG_CMD__ADDD__Concrete));
	m_arMatlType.Add(_LS(IDS_WG_CMD__ADDD__SRC));
	m_arMatlType.Add(_LS(IDS_WG_CMD__ADDD__USER));

	m_strMatlType = _LS(IDS_WG_CMD__ADDD__All_nSteel_nConcrete_nSRC_nUSER);

	m_nOldRow = 1;
	m_bModified = FALSE;
}

CCMLoadESEQSubGrid::~CCMLoadESEQSubGrid()
{
}

#define CWnd CTBBrowserWnd
BEGIN_MESSAGE_MAP(CCMLoadESEQSubGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCMLoadESEQSubGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQSubGrid message handlers
void CCMLoadESEQSubGrid::Initialize(CDBDoc* pDoc)
{
	CTBBrowserWnd::Initialize();
	
	m_pDoc = pDoc;

	GetParam()->EnableTrackRowHeight(FALSE); 
	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(3);
	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// type ComboBox 에 Item 입력
	CString strType = _LS(IDS_WG_CMD__ADD2__Mat_Type_nMat_Name);

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)    
			.SetChoiceList(strType)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetControl(GX_IDS_CTRL_TEXTFIT)
//			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)    
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)    
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			);

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetCurrentCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// Row, Column 이동을 불가능하게 한다.
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// Column Header가 선택이 되지 않게 한다.
	GetParam()->EnableSelection(GX_SELROW);
//	GetParam()->EnableSelection(GX_SELTABLE | GX_SELROW);

	// Column 크기 설정
	//SetColWidth(1, 1, 90);
	//SetColWidth(2, 2, 125);
	//SetColWidth(3, 3, 90);
	SetColWidthByDPI(1, 1, 80);
	SetColWidthByDPI(2, 2, 90);
	SetColWidthByDPI(3, 3, 80);

	InitMatlData();
	InitGridData();

	LockUpdate(FALSE);
	Redraw();

	SetCurrentCell(1,1);

	//SetReadOnly();
}

// Project에 저장되어 있는 Material정보를 읽어와 m_strMatlName변수에 저장한다.
void CCMLoadESEQSubGrid::InitMatlData()
{	
	CString strMaterialName;
	CString strNumTemp;

	CArray<T_MATL_K, T_MATL_K> arMatlKey;
	T_MATL_D MatlData;

	m_pDoc->m_pAttrCtrl->GetMatlKeyList(arMatlKey);
	
	for(int i=0 ; i < arMatlKey.GetSize() ; i++)
	{
		m_pDoc->m_pAttrCtrl->GetMatl(arMatlKey.GetAt(i), MatlData);
			
		strNumTemp.Format(_T("%d: "), arMatlKey.GetAt(i));

		if(i == arMatlKey.GetSize()-1)
			strMaterialName = strMaterialName + strNumTemp + MatlData.Name;
		else
			strMaterialName = strMaterialName + strNumTemp + MatlData.Name + _T("\n");
	}  
	m_strMatlName = strMaterialName;
}

void CCMLoadESEQSubGrid::SetHeaderTitle()
{
	CString title;
	int i;
	ROWCOL nCol;

	for(i = 0; i < GetColCount(); i++)
	{
		title = m_arTitle[i];
		nCol = i + 1;

		SetStyleRange(CGXRange(0, nCol), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_arTitle[i]));
	}
}

// 3번째 Column에 나타나는 Story정보를 가지고 초기화 시킨다.
void CCMLoadESEQSubGrid::InitGridData()
{
	CArray<T_STOR_D, T_STOR_D&> arStoryInfo;
	T_STOR_D StoryInfo;
	CString strStoryName=_T("");

 	m_pDoc->m_pAttrCtrl->GetStorData(arStoryInfo); 
	
	for(int i=0 ; i < arStoryInfo.GetSize() ; i++)
		if(i == arStoryInfo.GetSize()-1)
			strStoryName = strStoryName + (arStoryInfo.GetAt(i)).StoryName;
		else
			strStoryName = strStoryName + (arStoryInfo.GetAt(i)).StoryName +_T("\n");
		
	SetStyleRange(CGXRange().SetCols(3), CGXStyle().SetChoiceList(strStoryName));	
}

// MainGrid에서 마지막 Row가 선택되었을때 SubGrid초기화 시킨.
void CCMLoadESEQSubGrid::InitESEQData()
{
	SetReadOnly(FALSE);
	LockUpdate(TRUE);
	RemoveRows(1, GetRowCount());
	SetRowCount(1);
	LockUpdate(FALSE);
	Redraw();

	m_nOldRow = 1;
	m_bModified = FALSE;

	SetCurrentCell(1, 1, GX_UPDATENOW|GX_NOSETFOCUS);
	SetReadOnly(TRUE);
}

// MainGrid에서 선택된 Row에 대한 상세 정보(T_ESEQ_BASE)를 읽어와서 SubGrid에 보여준다.
void CCMLoadESEQSubGrid::InitESEQData(T_ESEQ_BASE& ESEQBase, int nNum)
{
	SetReadOnly(FALSE);

	CString strType;
	CString strTemp;
	CString strNoTemp;
	BOOL bExistData = FALSE;
	T_ESEQ_SEL ESEQSel;
	T_MATL_D TempMatlData;
	T_STOR_D TempStorData;

	TempMatlData.Initialize();
	TempStorData.Initialize();

	/* 
	for(int i=0; i<15; i++)
	{
		ESEQSel.Initialize();
		ESEQSel = ESEQBase.EseqSel[i];

		if(!i && (ESEQSel.nType != 0))
			bExistData = TRUE;

		if((ESEQSel.nType == 0) && (i))
		{
			if((i==1) && !bExistData) i--;
			break;
		}
	}
	*/

	LockUpdate(TRUE);

	RemoveRows(1, GetRowCount());
	SetRowCount(nNum+1/*i+1*/);

	for(int i=0; i<nNum; i++)
	{
		ESEQSel.Initialize();
		ESEQSel = ESEQBase.EseqSel[i];

		if((ESEQSel.nType == 0) && (i != 0 ))
			break;

		if(ESEQSel.nType == 1) 
		{
			strTemp = _LS(IDS_WG_CMD__ADD2__Mat_Type); 
			SetValueRange(CGXRange(i+1,1), strTemp);
			ChangeMaterialData(i+1);

			switch(ESEQSel.KeyMatl)
			{
				case 1:		strTemp = _LS(IDS_WG_CMD__ADDD__All);         break;
				case 2:		strTemp = _LS(IDS_WG_CMD__ADDD__Steel);			 break;
				case 3:		strTemp = _LS(IDS_WG_CMD__ADDD__Concrete);		 break;
				case 4:		strTemp = _LS(IDS_WG_CMD__ADDD__SRC);			   break;
				case 5:		strTemp = _LS(IDS_WG_CMD__ADDD__USER);		  	 break;
			}
			SetValueRange(CGXRange(i+1,2), strTemp);
		}
		else if(ESEQSel.nType == 2) 
		{
			strTemp = _LS(IDS_WG_CMD__ADD2__Mat_Name);
			SetValueRange(CGXRange(i+1,1), strTemp);
			ChangeMaterialData(i+1);

			if(ESEQSel.KeyMatl)
			{
				m_pDoc->m_pAttrCtrl->GetMatl(ESEQSel.KeyMatl, TempMatlData);
				strNoTemp.Format(_T("%d: "),ESEQSel.KeyMatl);
				strTemp = strNoTemp + TempMatlData.Name;
				SetValueRange(CGXRange(i+1,2), strTemp);
			}
		}	
		
		if(ESEQSel.KeyStorModel)
		{
			m_pDoc->m_pAttrCtrl->GetStor(ESEQSel.KeyStorModel, TempStorData);
			SetValueRange(CGXRange(i+1,3), TempStorData.StoryName);
		}
	}

	LockUpdate(FALSE);
	Redraw();

	m_nOldRow = 1;

	SetCurrentCell(1, 1, GX_UPDATENOW|GX_NOSETFOCUS);
	SetReadOnly(TRUE);
}

// Matl.Type, Matl.Name 항목을 선택함에 따라 2번째 Column에 나타나는 정보를 다르게 설정한다.
void CCMLoadESEQSubGrid::ChangeMaterialData(ROWCOL nRow)
{
	CString strType;
	strType = GetValueRowCol(nRow, 1);
	CString strNumTemp;

	LockUpdate(TRUE);

	if(strType == _LS(IDS_WG_CMD__ADD2__Mat_Type))
	{
		SetStyleRange(CGXRange(nRow,2), CGXStyle()
		 .SetChoiceList(m_strMatlType)
		 .SetValue(_T(""))
			);  
	}
	else if(strType == _LS(IDS_WG_CMD__ADD2__Mat_Name))
	{
		SetStyleRange(CGXRange(nRow,2), CGXStyle()
			.SetChoiceList(m_strMatlName)
			.SetValue(_T(""))
			);  
	}
	LockUpdate(FALSE);
	RedrawRowCol(nRow,2);
//	Redraw();
}

void CCMLoadESEQSubGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	if(nCol == 1)               // 첫번째 Column 내용변경시 두번째 Column의 Data변환시킴.
		ChangeMaterialData(nRow);

	if(nRow == GetRowCount())   // 새로운 Row 추가.
		SetRowCount(nRow + 1);

	m_nOldRow = nRow;
	m_bModified = TRUE;

	m_pESEQMainGrid->m_bModified = TRUE;
}

// Sub Grid내 데이타의 Validation을 Check한다.
BOOL CCMLoadESEQSubGrid::CheckField(BOOL bFlag)
{
	if(!m_bModified && bFlag) return TRUE;   // bFlag=False이면 m_bModified와 상관없이 전체를 검사
	if(m_nOldRow == GetRowCount()) return TRUE;

	CString strStoryName;
	CString strType;
	CString strMaterial;

	strType = GetValueRowCol(m_nOldRow,1);
	strMaterial = GetValueRowCol(m_nOldRow,2);
	strStoryName = GetValueRowCol(m_nOldRow,3);  // 여기서는 StoryName 이 None이 될 수 없다.

	return ((strType != _T("")) && (strMaterial != _T("")) && (strStoryName != _T(""))) || ((strType == _T("")) && (strMaterial == _T("")) && (strStoryName == _T("")));
}

// Sub Grid내 데이타의 Validation을 Check한다.
BOOL CCMLoadESEQSubGrid::CheckAllBlankField(BOOL bFlag)  // TRUE-Main,, FALSE-Sub Main에서 Insert Row가 되었을때 
{																												 // 아무데이타도 안 넣고 Row이동을 할때 지워버린다.
	if(bFlag && GetRowCount() != 1) return FALSE;  // FALSE가 좋은 개념이다.

	CString strStoryName;
	CString strType;
	CString strMaterial;

	strType = GetValueRowCol(m_nOldRow,1);
	strMaterial = GetValueRowCol(m_nOldRow,2);
	strStoryName = GetValueRowCol(m_nOldRow,3);  // 여기서는 StoryName 이 None이 될 수 없다.

	return ((strType == _T("")) && (strMaterial == _T("")) && (strStoryName == _T("")));
}

// Sub Grid내 데이타의 Validation을 Check한다.
BOOL CCMLoadESEQSubGrid::CheckExistBlankField()
{
//	if(!m_bModified && m_nOldRow == GetRowCount()) return FALSE;  // FALSE가 좋은 개념이다.
	if(!m_bModified && m_nOldRow == GetRowCount() && m_nOldRow != 1) return FALSE;  // FALSE가 좋은 개념이다.

	CString strStoryName;
	CString strType;
	CString strMaterial;

	strType = GetValueRowCol(m_nOldRow,1);
	strMaterial = GetValueRowCol(m_nOldRow,2);
	strStoryName = GetValueRowCol(m_nOldRow,3);  // 여기서는 StoryName 이 None이 될 수 없다.

	if(m_nOldRow == 1)
		return !((strType == _T("")) && (strMaterial == _T("")) && (strStoryName == _T("")));
	else
		return ((strType == _T("")) && (strMaterial == _T("")) && (strStoryName == _T("")));
}

// Row 가 Delete되었을때 호출.
BOOL CCMLoadESEQSubGrid::OnDeleteRecord(ROWCOL nRow)
{
	m_bModified = FALSE;
	CTBBrowserWnd::OnDeleteRecord(nRow);

	if(GetRowCount() == 2)
	{
		CString str1,str2,str3;
		str1 = GetValueRowCol(1,1);
		str2 = GetValueRowCol(1,2);
		str3 = GetValueRowCol(1,3);

		if((str1 == _T("")) && (str2 == _T("")) && (str3 == _T("")))
		{
			RemoveRows(2,2);
			m_nOldRow = 1;
			SetCurrentCell(1,1);
		}
	}

	m_pESEQMainGrid->m_bModified = TRUE;

	return TRUE;
}

BOOL CCMLoadESEQSubGrid::OnDeleteRecords(CRowColArray &awRows)
{
	m_bModified = FALSE;

	LockUpdate(TRUE);

	BOOL bIsMoveCurrentFocus = FALSE;   // CurrentCell이 바뀌는지 검사하기위한 부분.
	BOOL bCheckLastRow = FALSE;         // 마지막 Row에 Focus가 있는지 검사한다.
	ROWCOL ncRow,ncCol;
	GetCurrentCell(ncRow,ncCol);
	if(ncRow == GetRowCount()) bCheckLastRow = TRUE;

	ROWCOL nToRow, nFromRow;

	if(awRows.GetAt(awRows.GetSize()-1) == GetRowCount())
		nToRow = awRows.GetAt(awRows.GetSize()-1)-1;
	else
		nToRow = awRows.GetAt(awRows.GetSize()-1);

	nFromRow = awRows.GetAt(0);

	if((nFromRow <= ncRow) && ( ncRow <= nToRow))   // CurrentCell이 바뀌어야 하는지 검사
		bIsMoveCurrentFocus = TRUE;

	for(int i=nToRow; i >= nFromRow; i--)
	{
		RemoveRows(i,i);
	}
	
	if(GetRowCount() == 2)
	{
		CString str1,str2,str3;
		str1 = GetValueRowCol(1,1);
		str2 = GetValueRowCol(1,2);
		str3 = GetValueRowCol(1,3);

		if((str1 == _T("")) && (str2 == _T("")) && (str3 == _T("")))
			RemoveRows(2,2);
	}

	if(bCheckLastRow) SetCurrentCell(GetRowCount(), ncCol);
	else if(bIsMoveCurrentFocus) SetCurrentCell(nFromRow,1);

	LockUpdate(FALSE);
	Redraw();
	m_pESEQMainGrid->m_bModified = TRUE;
	return TRUE;	
}

/*
// Main Grid와 같이 Keyboard입력을 받을수 있게 한다.
BOOL CCMLoadESEQSubGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bCheckInsert = FALSE;
	// CheckField에는 m_nOldRow에 대해 Check를 하는데 InsertRow가 되면서 m_nOldRow가 가르키는 곳이
	// Insert 된 Row 가 된다. 
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	if((nMessage == WM_KEYDOWN) && (nChar == VK_DELETE)) 
	{
		CRowColArray awRows;

		GetSelectedRows(awRows, TRUE, FALSE);
		int nSize = awRows.GetSize();
		
		if (nSize > 0)
		{
			if (nSize > 0 && awRows.GetAt(0) == 0) { nSize--; awRows.RemoveAt(0); }
			if (nSize == 0) return TRUE;
			OnDeleteRecords(awRows);
		}
		else  // 이 경우는 현재 레코드 삭제이다.
		{
			OnDeleteRecord(ncRow);  // CurrentCell이 포함된 레코드를 삭제한다.
		}
		return TRUE;
	}
	
	if((nMessage == WM_KEYDOWN) && (nChar == VK_INSERT))  
	{
		if(ncRow == GetRowCount()) return TRUE;

		bCheckInsert = TRUE;
		++m_nOldRow;
	}

	CTBBrowserWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	if(bCheckInsert) return TRUE;

	GetCurrentCell(ncRow, ncCol);

	if(ncRow != m_nOldRow)
		OperateDataSub(ncRow, ncCol);

	return TRUE;
}
*/

void CCMLoadESEQSubGrid::OperateDataSub(ROWCOL nRow, ROWCOL nCol)
{
	if(m_nOldRow == nRow) return;
	
	if(m_bModified)
	{
		if(!CheckField(FALSE))
		{
			SetCurrentCell(m_nOldRow ,nCol);	
		}
		else 
		{
			m_bModified = FALSE;
			m_nOldRow = nRow;
		}
	}
	else
	{
		m_nOldRow = nRow;
	}
}

const CString& CCMLoadESEQSubGrid::GetValueRowCol(ROWCOL nRow, ROWCOL nCol)  // Override
{
	static CString value;
	value = _T("");
	CGXControl *pControl = GetControl(nRow, nCol);

	if (!pControl || !pControl->IsActive() || !pControl->GetModify())
		return CGXGridWnd::GetValueRowCol(nRow, nCol);  
	
	pControl->GetCurrentText(value);
	return value;
}

BOOL CCMLoadESEQSubGrid::OnInsertEmptyRecord(ROWCOL nRow)  // Override
{
	if(!CheckField())
	{
		LockUpdate(TRUE);
		RemoveRows(nRow, nRow);
		SetCurrentCell(nRow,1);
		LockUpdate(FALSE);
		Redraw();
		CTBBrowserWnd::m_nEditMode = noMode;   
		--m_nOldRow;
		return TRUE;
	}
	m_bModified = TRUE;
	CTBBrowserWnd::m_nEditMode = noMode;  // addnew 모드일때는 삽입된 Row를 지우고 원상복귀한다.
	--m_nOldRow;

	return TRUE;
}

void CCMLoadESEQSubGrid::OnInitCurrentCell(ROWCOL nRow, ROWCOL nCol)
{
	if(nRow == 0) return;
	if(m_nOldRow == nRow) return;  

	if(CheckAllBlankField(FALSE))                    // Load쪽에 층이 있을때 Model쪽에 
	{																					 	 // 데이타가 없을수 있다.	
		RemoveRows(m_nOldRow,m_nOldRow);
		m_nOldRow = nRow;
		m_pESEQMainGrid->m_bModified = TRUE;
		m_bModified = FALSE;
	}

	OperateDataSub(nRow, nCol);

	CGXGridCore::OnInitCurrentCell(nRow, nCol);
}



BOOL CCMLoadESEQSubGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	/*
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	CGXControl* pControl;
	ROWCOL i, j;
	int nSize;


	BOOL bCtl = GetKeyState(VK_CONTROL) & 0x8000;
	BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;

	if (bCtl || bShift) 
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	VERIFY(GetCurrentCell(nRow, nCol));
	pControl = GetControl(nRow, nCol);
	if (nMessage == WM_KEYDOWN)
	{
		// 셀이 ACTIVE인 상태에서 좌우 화살표 키가 눌러지면 셀 이동을 하지 않는다.
		if (nChar == VK_LEFT || nChar == VK_RIGHT)
		{
			ROWCOL nRow, nCol;
			if (GetCurrentCell(nRow, nCol))
			{
				CGXControl* pControl = GetControl(nRow, nCol);
				if (pControl && pControl->IsActive()) return TRUE;
			}
		}

		switch(nChar)
		{
		case VK_ESCAPE:
			if (m_nEditMode != noMode)  // 추가모드 또는 수정모드에서
			{
				if (!pControl || !pControl->IsActive()) // 편집중인 셀이 없으면
				{
					CancelEdit(); // 레코드 편집을 취소한다.
					return TRUE;
				}
			}
			// 편집모드 아닌 상태에서 편집중인 셀이 없으면 ESC는 무시한다.
			// 이렇게 함으로써 셀이 편집되지 않았을 때 OnCancelEditing과 
			// OnCanceledEditing이 호출 되는 것을 막는다.
			else if (!pControl || !pControl->IsActive()) 
			{
				// Select된 것을 해제한다.
				SelectRange(CGXRange().SetTable(), FALSE);
				return TRUE;
			}
			break;
		case VK_RETURN: case VK_RIGHT: case VK_TAB:
			// 열의 위치에 상관 없이 동작 : 두번째 버전
			// 마지막 셀이면 그냥 리턴
			if (IsAppendRow(nRow) && nCol == GetColCount()) return TRUE;

			i = nRow; j = nCol+1; // 현재 행 다음 열
			if (j > GetColCount()) { i++; j = 1; }

			m_bValidField = m_bValidRecord = TRUE; m_sWarningText.Empty();
			while (i <= GetRowCount())
			{
				while (j <= GetColCount())
				{
					// 열이 선택될 수 있는지 조사
					if (CanSelectCurrentCell(FALSE, i, j, nRow, nCol))
					{
						if (CanSelectCurrentCell(TRUE, i, j, nRow, nCol))
						{
							SetCurrentCell(i, j); return TRUE;
							return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
						}
						// 이 경우는 레코드 추가시 같은 키의 중복에 의해 레코드가 합쳐진 경우
						// CanSelectCurrentCell(FALSE...)가 불리기 전보다 Row Count가 1 작아진다.
						// CTBOffs을 참고(SetCurrentCell 함수 부분)
						// 이 경우 새로 설정하려는 row가 Row Count보다 커지는 경우가 발생
						// 이걸 막기 위해서 Append Row(마지막 Row)에 SetCurrentCell을 한다.
						if (i > GetRowCount())
						{
							SetCurrentCell(GetRowCount(), j); return TRUE;
						}
					}
					if (!m_bValidField || !m_bValidRecord)
					{
						m_bValidField = m_bValidRecord = TRUE;
						if (m_sWarningText.GetLength() > 0) DisplayWarningText();
						
						// Edit Control이면 포커스를 다시 설정한다.
						BOOL bActive = pControl->IsActive();
						pControl->SetActive(FALSE);
						pControl->Refresh();
						if (bActive) CWnd::PostMessage(WM_SET_CELL_ACTIVE, 0, 0);
						return TRUE;
					}
					j++;  // 다음 열을 조사
				}
				i++;  // 다음 줄로 이동
				j = 1;
			}
			break;
		case VK_LEFT:
			// 열의 위치에 상관 없이 동작 : 두번째 버전
			// 첫번째 셀이면 그냥 리턴
			if (nRow == 1 && nCol == 1) return TRUE;

			i = nRow; j = nCol-1; // 현재 행 이전 열
			if (j < 1) { i--; j = GetColCount(); }

			m_bValidField = m_bValidRecord = TRUE; m_sWarningText.Empty();

			while (i >= 1)
			{
				while (j >= 1)
				{
					// 열이 선택될 수 있는지 조사
					if (CanSelectCurrentCell(FALSE, i, j, nRow, nCol))
					{
						if (CanSelectCurrentCell(TRUE, i, j, nRow, nCol))
						{
							SetCurrentCell(i, j); return TRUE;
							return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
						}
					}
					if (!m_bValidField || !m_bValidRecord)
					{
						m_bValidRecord = TRUE;
						if (m_sWarningText.GetLength() > 0) DisplayWarningText();

						pControl->Refresh();
						return TRUE;
					}
					j--;  // 이전 열을 조사
				}
				i--;  // 이전 줄로 이동
				j = GetColCount();
			}
			break;
		case VK_INSERT: // 편집중인 레코드 앞에 addnew 레코드 추가
			if (!CanInsertRowByPressingKey()) return TRUE;
			if (IsLockEdit()) return TRUE;  // display only mode면 do nothing!
			if (!InsertEmptyRecord(nRow)) FALSE;
			break;
		case VK_DELETE: // 편집중인 레코드 삭제
			if (!CanDeleteRowByPressingKey()) return TRUE;
			// 먼저 범위가 선택되어 있는지 본다. 범위가 우선한다.
			if (IsLockEdit()) return TRUE;  // display only mode면 do nothing!
			m_sWarningText.Empty();
			GetSelectedRows(awRows, TRUE, FALSE);
			nSize = awRows.GetSize();
			
			if (nSize > 0)
			{
				// Append Row는 뺀다.
				if (IsAppendRow(awRows.GetAt(nSize-1))) { nSize--; awRows.RemoveAt(nSize); }
				// Title Row도 뺀다.
				if (nSize > 0 && awRows.GetAt(0) == 0) { nSize--; awRows.RemoveAt(0); }
				if (nSize == 0) return TRUE;
				if (!OnDeleteRecords(awRows))
					if (m_sWarningText.GetLength() > 0) DisplayWarningText();
			}
			else  // 이 경우는 현재 레코드 삭제이다.
			{
				if (IsAppendRow(nRow)) return TRUE;
				if (!OnDeleteRecord(nRow))
					if (m_sWarningText.GetLength() > 0) DisplayWarningText();
			}
			return TRUE;
			break;
		case VK_DOWN:
			// Alt-Down 키를 이용하면 되므로 이 항목은 없앤다.
			break;
		case VK_HOME:
			SetRightValidCell(nRow, 0);
			return TRUE;
			break;
		case VK_END:
			SetLeftValidCell(nRow, GetColCount()+1);
			return TRUE;
			break;
		}
	}
	else if (nMessage == WM_CHAR)
	{
		//VERIFY(GetCurrentCell(nRow, nCol));
		//pControl = GetControl(nRow, nCol);
		switch(nChar)
		{
		// 셀이 Inactive 상태에서 Edit 셀에 스페이스를 눌렀을 때 편집을 활성화 시킨다.
		case VK_SPACE:
			if (pControl && !pControl->IsActive() 
				&& pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl)))
			{
				if (!pControl->IsInit()) pControl->Init(nRow, nCol);
				CString value = GetExpressionRowCol(nRow, nCol);
				pControl->SetActive(TRUE);
				pControl->Refresh(); // 반드시 호출
				if (!pControl->IsKindOf(CONTROL_CLASS(CGXComboBox)))  // combo box가 아니면
					((CGXEditControl*)pControl)->SetSel(0, value.GetLength());
				return TRUE;
			}
			break;
		}
	}
	*/
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}
