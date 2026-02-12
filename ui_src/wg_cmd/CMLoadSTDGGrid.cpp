// CMLoadSTDGGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadSTDGGrid.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_common\wg_common_TBBrowserWndEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadSTDGGrid

CCMLoadSTDGGrid::CCMLoadSTDGGrid()
{
	m_pDoc = NULL;
	m_nOldRow = 1;
	m_bModified = FALSE;
	m_bRefreshSubGrid = TRUE;
}

CCMLoadSTDGGrid::~CCMLoadSTDGGrid()
{
}

#define CWnd CTBBrowserWnd
BEGIN_MESSAGE_MAP(CCMLoadSTDGGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCMLoadSTDGGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// CCMLoadSTDGGrid message handlers
//

void CCMLoadSTDGGrid::Initialize(CDBDoc* pDoc)
{
	CTBBrowserWnd::Initialize();

	m_pDoc = pDoc;

	LockUpdate(TRUE);

	SetColCount(2);
	SetHeaderTitle();

	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
	  .SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)    
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL     , _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL    , _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
	  .SetVertScrollBar(FALSE));

	SetStyleRange(CGXRange().SetCols(1, 1), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)    
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC));

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,(CGXStyle) pProp->sInvertNoBorder);

	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);
	GetParam()->EnableSelection(GX_SELROW);
	GetParam()->EnableTrackRowHeight(FALSE); 

	SetColWidthByDPI(1, 1, 90);
	SetColWidthByDPI(2, 2, 65);

	InitStoryData(1);

	LockUpdate(FALSE);
	Redraw();

	SetCurrentCell(1,1);
}

//////////////////////////////////////////////////////////////////////////
// SetHeaderTitle - Set Grid Header
//
//////////////////////////////////////////////////////////////////////////
void CCMLoadSTDGGrid::SetHeaderTitle()
{
	CString title;
	int i;
	ROWCOL nCol;

	CStringArray m_arTitle;
	CString strTitle[] = { _LS(IDS_CMD_ESQW_Starting_Day), _LS(IDS_CMD_ESQW_Story) };

	for(i = 0; i < 2; i++)
	{
		nCol = i + 1;
		SetStyleRange(CGXRange(0, nCol), CGXStyle()
			.SetValue(strTitle[i])
			.SetUserAttribute(ID_USER_ATTR_COL_ID, strTitle[i]));
	}
}

//////////////////////////////////////////////////////////////////////////
// InitStoryData - Story ComboBox Contents
//
//////////////////////////////////////////////////////////////////////////
void CCMLoadSTDGGrid::InitStoryData(ROWCOL nRow)  
{
	T_STOR_K nStorUpKey;
	CArray<T_STOR_D, T_STOR_D&> arStoryInfo;
	T_ESQW_ADD_STOR_BASE tempESQWBase;

	CString strStoryName = _LS(IDS_WG_CMD__ADDD__None_n);
	CString strUpStoryName;

	m_pDoc->m_pAttrCtrl->GetStorData(arStoryInfo); 
	
	if(nRow==1) strStoryName = GetComboStoryName(0);
	else  
	{
		strUpStoryName = GetValueRowCol(nRow-1, 2);
		nStorUpKey = m_pDoc->m_pAttrCtrl->GetStorKey(strUpStoryName);

		if(nStorUpKey == 0)
		{
			for(int i=nRow-2; i>0; i--) 
			{
				tempESQWBase.Initialize();
				tempESQWBase = m_ESQWData.arStory.GetAt(i-1);

				if(tempESQWBase.StoryKey > 0){ nStorUpKey = tempESQWBase.StoryKey; break;	}
			}
		}
		
		if(nStorUpKey == 0)	strStoryName = GetComboStoryName(0);
		else              	strStoryName = GetComboStoryName(nStorUpKey);
	}
	SetStyleRange(CGXRange(nRow,2), CGXStyle().SetChoiceList(strStoryName));		
}

void CCMLoadSTDGGrid::InitStoryData(ROWCOL nRow, T_STOR_K nStorKey)
{
	if(m_strOldStoryName == GetValueRowCol(nRow, 2)) return;

	CString strStoryName = GetComboStoryName(nStorKey);
		
	SetStyleRange(CGXRange(nRow,2), CGXStyle().SetChoiceList(strStoryName).SetValue(_LS(IDS_WG_CMD__ADDD__None)));		
}

void CCMLoadSTDGGrid::InitAllStoryData()
{
	T_STOR_D nStorData;
	T_STOR_K nStorUpKey;
	CString strStoryName = _LS(IDS_WG_CMD__ADDD__None_n);
	T_ESQW_ADD_STOR_BASE tempESQWBase;

	tempESQWBase.Initialize();
	tempESQWBase = m_ESQWData.arStory.GetAt(0);
	InitStoryData(1, 0); 
	
	nStorUpKey = tempESQWBase.StoryKey;

	if(tempESQWBase.StoryKey)  
	{
		m_pDoc->m_pAttrCtrl->GetStor(tempESQWBase.StoryKey, nStorData);
		SetValueRange(CGXRange(1,2), nStorData.StoryName); 
	}
	else	SetValueRange(CGXRange(1,2), _LS(IDS_WG_CMD__ADDD__None)); 

	for(int i=1; i<m_ESQWData.arStory.GetSize(); i++)  
	{
		tempESQWBase.Initialize();
		tempESQWBase = m_ESQWData.arStory.GetAt(i);

		InitStoryData(i+1, nStorUpKey);
		
		if(nStorUpKey < tempESQWBase.StoryKey) nStorUpKey = tempESQWBase.StoryKey;

		if(tempESQWBase.StoryKey)
		{
			m_pDoc->m_pAttrCtrl->GetStor(tempESQWBase.StoryKey, nStorData);
			SetValueRange(CGXRange(i+1,2), nStorData.StoryName); 
		}
		else SetValueRange(CGXRange(i+1,2), _LS(IDS_WG_CMD__ADDD__None));
	}

	InitStoryData(GetRowCount());  
}

CString CCMLoadSTDGGrid::GetComboStoryName(int nStoryKey)
{
	CArray<T_STOR_D, T_STOR_D&> arStoryInfo;
	T_STOR_D StoryInfo;
	CString strStoryName;

 	m_pDoc->m_pAttrCtrl->GetStorData(arStoryInfo); 
	
	for(int i=nStoryKey ; i < arStoryInfo.GetSize() ; i++) strStoryName = strStoryName + (arStoryInfo.GetAt(i)).StoryName +_T("\n"); 
		
	strStoryName.Insert(strStoryName.GetLength(),_LS(IDS_WG_CMD__ADDD__None_n));
	
	return strStoryName;
}

//////////////////////////////////////////////////////////////////////////
// InitESQWData - DataSetting in Grid
//
//////////////////////////////////////////////////////////////////////////
void CCMLoadSTDGGrid::InitESQWData()
{
	if(!m_ESQWData.arStory.GetSize()) return;
	
	LockUpdate(TRUE);

	CString strTemp;
	T_STOR_D StorData;

	RemoveRows(1, GetRowCount());
	SetRowCount(m_ESQWData.arStory.GetSize()+1);	
	int i = 0;
	for( i=0; i<m_ESQWData.arStory.GetSize(); i++)  
	{
		InitStoryData(i+1);
		m_pDoc->m_pAttrCtrl->GetStor((m_ESQWData.arStory.GetAt(i)).StoryKey, StorData);		
		
		if((m_ESQWData.arStory.GetAt(i)).StoryKey == 0)	SetValueRange(CGXRange(i+1,2), _LS(IDS_WG_CMD__ADDD__None));		
		else                                            SetValueRange(CGXRange(i+1,2), StorData.StoryName);
		
		strTemp.Format(_T("%g"), (m_ESQWData.arStory.GetAt(i)).dStartingDay);
		SetValueRange(CGXRange(i+1,1), strTemp);
	}

	InitStoryData(i+1);   
	LockUpdate(FALSE);
	Redraw();
	
	SetCurrentCell(1,2);

	m_nOldRow = 1;
	m_bModified = FALSE;
}

void CCMLoadSTDGGrid::AddESQWData(ROWCOL nRow)
{
	unsigned int nTemp;
	CString strMatlKey, strTemp;

	m_ESQWBase.Initialize();    
	strTemp = GetValueRowCol(nRow,2);

	if(strTemp == _LS(IDS_WG_CMD__ADDD__None)) m_ESQWBase.StoryKey = 0;
	else
	{
		nTemp = m_pDoc->m_pAttrCtrl->GetStorKey(strTemp);	
		m_ESQWBase.StoryKey = nTemp;
	}

	m_ESQWBase.dStartingDay = _tstof(GetValueRowCol(nRow,1));

	if(m_ESQWData.arStory.GetSize() >= m_nOldRow)	m_ESQWData.arStory.SetAt(nRow-1, m_ESQWBase);
	else                                        	m_ESQWData.arStory.Add(m_ESQWBase);
}

void CCMLoadSTDGGrid::ModifyESQWData(int nIndex)
{
	m_ESQWBase.Initialize();
	m_ESQWBase = m_ESQWData.arStory.GetAt(nIndex-1);
	m_ESQWBase.StoryKey = 0;
	
	m_ESQWData.arStory.SetAt(nIndex-1, m_ESQWBase);
}

//////////////////////////////////////////////////////////////////////////
// OnModifyCell
//
//////////////////////////////////////////////////////////////////////////
void CCMLoadSTDGGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	if(nCol <= 0) return;
	else if(nCol == 1)
	{
		if(nRow < GetRowCount()-1 )
		{
			T_ESQW_ADD_STOR_BASE base, tempESQWBase;
			tempESQWBase.Initialize(); base.Initialize();
		
			base = m_ESQWData.arStory.GetAt(nRow-1);
			tempESQWBase.Initialize();
			tempESQWBase.dStartingDay = _tstof(GetValueRowCol(nRow,1));
			tempESQWBase.StoryKey = base.StoryKey;			
	 
			m_ESQWData.arStory.SetAt(nRow-1, tempESQWBase);			
		}
		return;
	}

	LockUpdate(TRUE);
	BOOL bFlag = TRUE;

	CString strUpStoryName;
	CString strDownStoryName;
	T_STOR_K nStorUpKey, nStorDownKey;
	T_STOR_D nStorData;
	T_ESQW_ADD_STOR_BASE tempESQWBase;

	if(nRow == GetRowCount()) 
	{ 
		InsertRows(nRow+1,1);	
		InitStoryData(nRow+1);	
	}
	else if(nRow == GetRowCount()-1) 
	{
		CString strCellContent;
		strCellContent = GetValueRowCol(nRow, nCol);
		if(strCellContent == _T(""))
		{
			strCellContent = _LS(IDS_WG_CMD__ADDD__None);
			SetValueRange(CGXRange(nRow, nCol), strCellContent);
		}
		InitStoryData(nRow+1);
	}
	else  
	{
		strUpStoryName = GetValueRowCol(nRow, 2);
		nStorUpKey = m_pDoc->m_pAttrCtrl->GetStorKey(strUpStoryName);

		if(nStorUpKey == 0) 
		{
			int i = 0;
			for(i=nRow-1; i>0; i--)
			{
				tempESQWBase.Initialize();
				tempESQWBase = m_ESQWData.arStory.GetAt(i-1);				

				if(tempESQWBase.StoryKey > 0){ nStorUpKey = tempESQWBase.StoryKey; break; }
			}							
			InitStoryData(nRow, nStorUpKey); 

			for(i=nRow+1; i<=GetRowCount(); i++)
			{						
				if(i == GetRowCount()){	InitStoryData(i, nStorUpKey);	SetValueRange(CGXRange(i,2), _T(""));	break; }

				tempESQWBase.Initialize();
				tempESQWBase = m_ESQWData.arStory.GetAt(i-2);
				
				if(tempESQWBase.StoryKey == 0)  InitStoryData(i, nStorUpKey);
				else          
				{						
					InitStoryData(i, nStorUpKey);
					m_pDoc->m_pAttrCtrl->GetStor(tempESQWBase.StoryKey, nStorData);
					SetValueRange(CGXRange(i,2), nStorData.StoryName); 
					break;
				}
			}
		}
		else                
		{
			for(int i=nRow+1; i<=GetRowCount(); i++)   
			{			
				if(i == GetRowCount()){	InitStoryData(i, nStorUpKey); SetValueRange(CGXRange(i,2), _T("")); break; }
				
				tempESQWBase.Initialize();
				tempESQWBase = m_ESQWData.arStory.GetAt(i-1);
				nStorDownKey = tempESQWBase.StoryKey;

				if(nStorUpKey >= nStorDownKey){	InitStoryData(i, nStorUpKey); ModifyESQWData(i); }
				else                           
				{															 
					InitStoryData(i, nStorUpKey);
					m_pDoc->m_pAttrCtrl->GetStor(nStorDownKey, nStorData);
					SetValueRange(CGXRange(i,2), nStorData.StoryName); 
					break;
				}				
			}
		}
	} 
	LockUpdate(FALSE);
	Redraw();

	m_nOldRow = nRow;
	m_bModified = TRUE;
}

BOOL CCMLoadSTDGGrid::OnDeleteRecord(ROWCOL nRow)
{
	CTBBrowserWnd::OnDeleteRecord(nRow);    

	m_bModified = FALSE;

	if(nRow == m_ESQWData.arStory.GetSize())      m_ESQWData.arStory.RemoveAt(nRow-1); 
	else if(nRow < m_ESQWData.arStory.GetSize()){	m_ESQWData.arStory.RemoveAt(nRow-1); InitAllStoryData(); }
	else                                          InitStoryData(GetRowCount());

	int nCount = GetRowCount();
	if(m_nOldRow == GetRowCount()) m_nOldRow = m_nOldRow - 1;

	return TRUE;
}

BOOL CCMLoadSTDGGrid::OnDeleteRecords(CRowColArray &awRows)
{
	ROWCOL ncRow,ncCol;
	ROWCOL nToRow,nFromRow;

	m_bModified       = FALSE;
	m_bRefreshSubGrid = FALSE;
	BOOL bCheckLastRow       = FALSE;        
	BOOL bIsMoveCurrentFocus = FALSE;   

	LockUpdate(TRUE);

	GetCurrentCell(ncRow,ncCol);
	int nCount = GetRowCount();
	if(ncRow == GetRowCount()-1 || ncRow == GetRowCount()) bCheckLastRow = TRUE; // 커런트 셀이 마지막 로우로 설정되어 있으면

	nFromRow = awRows.GetAt(0);
	if(awRows.GetAt(awRows.GetSize()-1) == GetRowCount())	nToRow = awRows.GetAt(awRows.GetSize()-1)-1;
	else                                                	nToRow = awRows.GetAt(awRows.GetSize()-1);

	if((nFromRow <= ncRow) && ( ncRow <= nToRow))	bIsMoveCurrentFocus = TRUE; // 커런트 셀이 선택된 셀안에 속하면

	for(int i=nToRow; i >= nFromRow; i--){ RemoveRows(i,i); DeleteESQWData(i); }

	if(bCheckLastRow)
	{
		m_nOldRow = GetRowCount()-1;
		if(m_nOldRow == 0) m_nOldRow = 1;
		SetCurrentCell(m_nOldRow, ncCol);
	}
	else if(bIsMoveCurrentFocus) 
	{
		m_nOldRow = nFromRow - 1;
		if(m_nOldRow == 0) m_nOldRow = 1;
		SetCurrentCell(m_nOldRow, 2);
		if(nFromRow != GetRowCount()) InitAllStoryData();
	}

	LockUpdate(FALSE);
	Redraw();
	m_bRefreshSubGrid = TRUE;

	return TRUE;
}

BOOL CCMLoadSTDGGrid::DeleteESQWData(ROWCOL nRow,BOOL bEndStep)
{
	if(nRow == m_ESQWData.arStory.GetSize())  
		m_ESQWData.arStory.RemoveAt(nRow-1);
	else if(nRow < m_ESQWData.arStory.GetSize())  
		m_ESQWData.arStory.RemoveAt(nRow-1);

	return TRUE;
}

BOOL CCMLoadSTDGGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bCheckInsert = FALSE;
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
		else  
		{
			OnDeleteRecord(ncRow);
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
		OperateDataMain(ncRow, ncCol);  

	return TRUE;
}

const CString& CCMLoadSTDGGrid::GetValueRowCol(ROWCOL nRow, ROWCOL nCol)
{
	static CString value;
	value = _T("");
	CGXControl *pControl = GetControl(nRow, nCol);

	if (!pControl || !pControl->IsActive() || !pControl->GetModify())
		return CGXGridWnd::GetValueRowCol(nRow, nCol);

	pControl->GetCurrentText(value);  
	return value;
}

BOOL CCMLoadSTDGGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	m_ESQWBase.Initialize();             
	m_ESQWData.arStory.InsertAt(nRow-1, m_ESQWBase);

	if(m_bModified) AddESQWData(nRow+1);
		
	InitStoryData(nRow);
	SetCurrentCell(nRow, 2);             

	m_bModified = FALSE;							
	CTBBrowserWnd::m_nEditMode = noMode;
	--m_nOldRow;
	return TRUE;

	LockUpdate(TRUE);
	RemoveRows(nRow, nRow);
	SetCurrentCell(nRow,2);
	LockUpdate(FALSE);
	Redraw();
	CTBBrowserWnd::m_nEditMode = noMode;
	--m_nOldRow;
	return TRUE;
}

BOOL CCMLoadSTDGGrid::DoLButtonDblClk(UINT flags, CPoint point)
{
	return FALSE;
}

void CCMLoadSTDGGrid::OnInitCurrentCell(ROWCOL nRow, ROWCOL nCol)
{
	if(nCol < 1 || nRow == 0) return;
	if(m_nOldRow == nRow) return;  

	m_strOldStoryName = GetValueRowCol(nRow, 2);

	if(m_bRefreshSubGrid)
		OperateDataMain(nRow, nCol);

	CGXGridCore::OnInitCurrentCell(nRow, nCol);
}

//Main Grid의 데이타의 Validation 검사.  
BOOL CCMLoadSTDGGrid::CheckField(BOOL bFlag)
{
	if(!m_bModified && bFlag) return TRUE;
	
	CString strStoryName;
	strStoryName = GetValueRowCol(m_nOldRow,2);

	return (strStoryName != _T("") /*&& dStartDay>=0.*/);
}

BOOL CCMLoadSTDGGrid::CheckValidRow()
{
	if(m_nOldRow == GetRowCount()) return TRUE;
	
	CString strStoryName;
	strStoryName = GetValueRowCol(m_nOldRow,2);

	return (strStoryName != _T("") /*&& dStartDay>=0.*/);
}

void CCMLoadSTDGGrid::OperateDataMain(ROWCOL nRow, ROWCOL nCol)
{
	if(m_nOldRow == nRow) return;  

	if(m_bModified)  
	{
		if(CheckField(FALSE))   
		{
			AddESQWData(m_nOldRow);
			m_nOldRow = nRow;
			m_bModified = FALSE;
		}
		else																						
		{
			if(GetRowCount() == m_nOldRow) m_bModified = FALSE;
			else {
				if(!CheckValidRow())
				{
					RemoveRows(m_nOldRow, m_nOldRow);
				
					if(nRow > m_nOldRow){	SetCurrentCell(nRow-1, nCol);	m_nOldRow = nRow-1;	}
					else 	m_nOldRow = nRow;
				}
				else SetCurrentCell(m_nOldRow, nCol);
			}   
		}
	}
	else   
	{
		if((!CheckValidRow()) && (m_nOldRow != GetRowCount()))
		{
			RemoveRows(m_nOldRow,m_nOldRow);
			m_bModified = FALSE;

			if(m_nOldRow == m_ESQWData.arStory.GetSize())
				m_ESQWData.arStory.RemoveAt(m_nOldRow-1);	
			else if(m_nOldRow < m_ESQWData.arStory.GetSize()) 
			{
				m_ESQWData.arStory.RemoveAt(m_nOldRow-1);
				InitAllStoryData();
			}
			else 	InitStoryData(GetRowCount());
		}
		m_nOldRow = nRow;
	}
}

void CCMLoadSTDGGrid::EnableGrid(BOOL bFlag)
{ 
	if(bFlag == TRUE) 
	{
		SetStyleRange(CGXRange(1, 1, m_nOldRow, 1), CGXStyle()
			  .SetControl(GX_IDS_CTRL_EDIT)    
				.SetHorizontalAlignment(DT_RIGHT)
				.SetValueType(GX_VT_NUMERIC));

		SetStyleRange(CGXRange(1, 2, m_nOldRow, 2), CGXStyle()
			  .SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)    
				.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
				.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
				.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
				.SetValueType(GX_VT_STRING)
				.SetHorizontalAlignment(DT_LEFT)
			  .SetVertScrollBar(FALSE));
	
		Redraw();
		InitStoryData(1);
	}
	else 
	{
		SetStyleRange(CGXRange(1, 1, m_nOldRow, 2), CGXStyle()
			  .SetControl(GX_IDS_CTRL_STATIC)    
				.SetHorizontalAlignment(DT_RIGHT));
		
		Redraw();
	}
}
