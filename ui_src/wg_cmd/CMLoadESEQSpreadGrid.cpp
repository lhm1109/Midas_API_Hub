// CMLoadESEQSpreadGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadESEQSpreadGrid.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQSpreadGrid

CCMLoadESEQSpreadGrid::CCMLoadESEQSpreadGrid()
{
	m_pDoc = NULL;

	m_strMatlType = _LS(IDS_WG_CMD__ADDD__All_nSteel_nConcrete_nSRC_nUSER);
	m_bInsertMode = FALSE;
	m_bModified = FALSE;	
	m_bStepModified = FALSE;
	m_bSameStepData = FALSE;
	m_nOldRow = 3;
}

CCMLoadESEQSpreadGrid::~CCMLoadESEQSpreadGrid()
{
}

void CCMLoadESEQSpreadGrid::SetHeaderTitle()
{
	CString title;

	title = _LS(IDS_WG_CMD__ADDD__Step);
	SetStyleRange(CGXRange(0, 1), CGXStyle()
		.SetValue(title)
		.SetUserAttribute(1, title));
	
	title = _LS(IDS_WG_CMD__ADDD__Model);
	SetStyleRange(CGXRange(0, 2), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(1, title));

	title = _LS(IDS_WG_CMD__ADDD__Type);
	SetStyleRange(CGXRange(1, 2), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(1, title));

	title = _LS(IDS_WG_CMD__ADDD__Material);
	SetStyleRange(CGXRange(1, 3), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(1, title));

	title = _LS(IDS_WG_CMD__ADDD__Story__To_);
	SetStyleRange(CGXRange(1, 4), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(1, title));

	title = _LS(IDS_WG_CMD__ADDD__Load);
	SetStyleRange(CGXRange(0, 5), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(1, title));
	
	title = _LS(IDS_WG_CMD__ADDD__Story_for_Dead_Load);
	SetStyleRange(CGXRange(1, 5), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(1, title));

	title = _LS(IDS_WG_CMD__ADDD__Start);
	SetStyleRange(CGXRange(2, 5), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(1, title));

	title = _LS(IDS_WG_CMD__ADDD__End);
	SetStyleRange(CGXRange(2, 6), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(1, title));

	// 추가 
	title = _LS(IDS_WG_CMD__ADDD__Stage_Duration);
	SetStyleRange(CGXRange(0, 7), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(1, title));

	title = _LS(IDS_WG_CMD__ADDD__Member_Age);
	SetStyleRange(CGXRange(0, 8), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(1, title));
}

#define CWnd CCMLoadESEQTBGrid
BEGIN_MESSAGE_MAP(CCMLoadESEQSpreadGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCMLoadESEQSpreadGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQSpreadGrid message handlers

void CCMLoadESEQSpreadGrid::Initialize(CDBDoc* pDoc)
{
	CCMLoadESEQTBGrid::Initialize();
	
	m_pDoc = pDoc;

	GetParam()->EnableTrackRowHeight(FALSE); 
	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(8);
	SetRowCount(3);

//	HideCols(0,0);

	SetFrozenRows(2,2);

	SetCoveredCellsRowCol(0,0,2,0);
	SetCoveredCellsRowCol(0,1,2,1);  // Step

	SetCoveredCellsRowCol(0,2,0,4);  // Model
	SetCoveredCellsRowCol(1,2,2,2);
	SetCoveredCellsRowCol(1,3,2,3);
	SetCoveredCellsRowCol(1,4,2,4);
	
	SetCoveredCellsRowCol(0,5,0,6);  // Load
	SetCoveredCellsRowCol(1,5,1,6);
	// 추가 
	SetCoveredCellsRowCol(0,7,2,7);
	SetCoveredCellsRowCol(0,8,2,8);

	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// Step 컬럼의 스타일 변경
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0));

	// type ComboBox 에 Item 입력
	CString strType = _LS(IDS_WG_CMD__ADD2__Mat_Type_nMat_Name);

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(strType)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetControl(GX_IDS_CTRL_TEXTFIT)
//      .SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
	
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			);

	SetStyleRange(CGXRange().SetCols(5), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)    
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetIncludeEnabled(TRUE)
			.SetEnabled(FALSE)
			.SetHorizontalAlignment(DT_LEFT)
			);
	
	SetStyleRange(CGXRange().SetCols(6), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)    
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetIncludeEnabled(TRUE)
			.SetEnabled(TRUE)
			.SetHorizontalAlignment(DT_LEFT)
			.SetVertScrollBar(FALSE)
			);

	// 추가 
	SetStyleRange(CGXRange().SetCols(7, 8), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0));

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,(CGXStyle) pProp->sInvertNoBorder);
	pProp->SetMarkColHeader(FALSE);     

	// 현재 셀 설정
	m_bPureCellChange = FALSE;
	SetCurrentCell(3, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// Row,Column 이동을 불가능하게 한다.
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// MultiSelect를 못하게 한다.
	GetParam()->EnableSelection(GX_SELROW);

	// Column 크기 설정
	SetColWidthByDPI(1, 1, 40);
	SetColWidthByDPI(2, 2, 80);
	SetColWidthByDPI(3, 3, 100);
	SetColWidthByDPI(4, 4, 75);
	SetColWidthByDPI(5, 5, 75);
	SetColWidthByDPI(6, 6, 75);
	// 추가 
	SetColWidthByDPI(7, 7, 60);
	SetColWidthByDPI(8, 8, 60);

	InitStoryData(); 
	InitMatlData();

	LockUpdate(FALSE);
	Redraw();

	SetCurrentCell(3,1);
	m_bPureCellChange = TRUE;
}

// 초기에 설정되는 값을 위한부분.
void CCMLoadESEQSpreadGrid::InitStoryData()
{
	CString strStoryName;

	strStoryName = GetComboStoryName(0);
	SetStyleRange(CGXRange(3,5,3,6), CGXStyle().SetChoiceList(strStoryName));	

	strStoryName.TrimLeft(_LS(IDS_WG_CMD__ADDD__None_n));
	SetStyleRange(CGXRange().SetCols(4), CGXStyle().SetChoiceList(strStoryName));
}

// Load쪽 Story 값을 가져오는 부분.
CString CCMLoadESEQSpreadGrid::GetComboStoryName(int nStoryKey)
{
	CArray<T_STOR_D, T_STOR_D&> arStoryInfo;
	T_STOR_D StoryInfo;
	CString strStoryName;

 	m_pDoc->m_pAttrCtrl->GetStorData(arStoryInfo); 
	
	// nStoryKey값과 같은 Key값을 가지는 Story가 몇번째 인지 찾는다.  
	int j = 0,i=0;
	for(j=0; j<arStoryInfo.GetSize(); j++)
	{
		StoryInfo = arStoryInfo.GetAt(j);
		
		if(!nStoryKey || nStoryKey == m_pDoc->m_pAttrCtrl->GetStorKey(StoryInfo.StoryName))
			break;
	}

	for(i=j+1 ; i < arStoryInfo.GetSize() ; i++)
		if(i == arStoryInfo.GetSize()-1)
			strStoryName = strStoryName + (arStoryInfo.GetAt(i)).StoryName;
		else
			strStoryName = strStoryName + (arStoryInfo.GetAt(i)).StoryName +_T("\n"); 
		
	strStoryName.Insert(0,_LS(IDS_WG_CMD__ADDD__None_n));
	return strStoryName;
}

void CCMLoadESEQSpreadGrid::InitMatlData()
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

// Matl.Type, Matl.Name 항목을 선택함에 따라 2번째 Column에 나타나는 정보를 다르게 설정한다.
void CCMLoadESEQSpreadGrid::ChangeMaterialData(ROWCOL nRow)
{
	CString strType;
	strType = GetValueRowCol(nRow, 2);
	CString strNumTemp;

	LockUpdate(TRUE);

	if(strType == _LS(IDS_WG_CMD__ADD2__Mat_Type))
	{
		SetStyleRange(CGXRange(nRow,3), CGXStyle()
		 .SetChoiceList(m_strMatlType)
		 .SetValue(_T(""))
			);  
	}
	else if(strType == _LS(IDS_WG_CMD__ADD2__Mat_Name))
	{
		SetStyleRange(CGXRange(nRow,3), CGXStyle()
			.SetChoiceList(m_strMatlName)
			.SetValue(_T(""))
			);  
	}
	LockUpdate(FALSE);
	RedrawRowCol(nRow,3);
}

// Normal Form, DB 에서 값을 받아와서 Spread Sheet에 나타내준다.
void CCMLoadESEQSpreadGrid::InitESEQData()
{
	// ESEQ 에 관한 데이타
	LockUpdate(TRUE);

	CString strTemp;
	T_STOR_D StorData;
	CString strType;
	CString strNoTemp;
	CString strOldStoryName;
	T_STOR_K OldStoryKey;
	T_STOR_D OldStoryData;

	T_MATL_D TempMatlData;
	T_STOR_D TempStorData;

	int nRowIndex=0;

	// RowCount
	for(int i=0; i<m_ESEQData.arEseq.GetSize(); i++)
	{
		m_ESEQBase = m_ESEQData.arEseq.GetAt(i);
		for(int j=0; j<15; j++)
		{
			if((m_ESEQBase.EseqSel[j].nType) || (j==0))   // Model 족의 데이타가 하나도 안들어갈 수도 있다.
				nRowIndex++;
			else
				break;
		}
	}

	RemoveRows(3, GetRowCount());
	SetRowCount(nRowIndex+3);	
	nRowIndex = 3;

	for(int i=0; i<m_ESEQData.arEseq.GetSize(); i++)  // 데이타 실제 입력
	{
		// MainGrid 쪽 데이타 입력.
		m_pDoc->m_pAttrCtrl->GetStor((m_ESEQData.arEseq.GetAt(i)).KeyStorLoad, StorData);				
		strNoTemp.Format(_T("%d"), i+1);

		// SubGrid 쪽 데이타 입력.	
		TempMatlData.Initialize();
		TempStorData.Initialize();
		m_ESEQBase.Initialize();
		m_ESEQBase = m_ESEQData.arEseq.GetAt(i);

		for(int j=0; j<15; j++)
		{
			m_ESEQSel.Initialize();
			m_ESEQSel = m_ESEQBase.EseqSel[j];

			if((m_ESEQSel.nType == 0) && (j !=0 ))
				break;

			SetValueRange(CGXRange(nRowIndex, 1), strNoTemp);

			if(!j)   // Story 정보 재설정.
			{
				if(m_ESEQBase.KeyStorLoad == 0)  // Story Key 에 0 값을 가질수 있다.
				{
					SetValueRange(CGXRange(nRowIndex,5), _LS(IDS_WG_CMD__ADDD__None));	 // Start Story Name;	
					SetValueRange(CGXRange(nRowIndex,6), _LS(IDS_WG_CMD__ADDD__None));		
				}
				else
				{
					if(nRowIndex == 3)
					{
						SetValueRange(CGXRange(nRowIndex,5), _LS(IDS_WG_CMD__ADDD__Base));		// Start Story Name;	
						SetValueRange(CGXRange(nRowIndex,6), StorData.StoryName);		
						strOldStoryName = StorData.StoryName;
					}
					else
					{
						OldStoryKey = m_pDoc->m_pAttrCtrl->GetStorKey(strOldStoryName);									
						m_pDoc->m_pAttrCtrl->GetStor(OldStoryKey+1, OldStoryData);
						SetValueRange(CGXRange(nRowIndex,5), OldStoryData.StoryName);		// Start Story Name;	
						SetValueRange(CGXRange(nRowIndex,6), StorData.StoryName);		
						strOldStoryName = StorData.StoryName;
					}
				}
				InitStoryData(nRowIndex);

				// 추가 
				strTemp.Format(_T("%g"), m_ESEQBase.dDuration);
				SetValueRange(CGXRange(nRowIndex,7), strTemp);		
				strTemp.Format(_T("%g"), m_ESEQBase.dAge);
			 	SetValueRange(CGXRange(nRowIndex,8), strTemp);	
			}
			else // 같은 Step은 Story 설정을 안한다.
			{
				SetStyleRange(CGXRange(nRowIndex,6), CGXStyle().SetEnabled(FALSE));
				// 추가 
				SetStyleRange(CGXRange(nRowIndex,7), CGXStyle().SetEnabled(FALSE));
				SetStyleRange(CGXRange(nRowIndex,8), CGXStyle().SetEnabled(FALSE));
//				SetStyleRange(CGXRange(nRowIndex,5,nRowIndex,6), CGXStyle().SetEnabled(FALSE));
			}

			if(m_ESEQSel.nType == 1) 
			{
				strTemp = _LS(IDS_WG_CMD__ADD2__Mat_Type); 
				SetValueRange(CGXRange(nRowIndex,2), strTemp);
				ChangeMaterialData(nRowIndex);

				switch(m_ESEQSel.KeyMatl)
				{
					case 1:		strTemp = _LS(IDS_WG_CMD__ADDD__All);         break;
					case 2:		strTemp = _LS(IDS_WG_CMD__ADDD__Steel);			 break;
					case 3:		strTemp = _LS(IDS_WG_CMD__ADDD__Concrete);		 break;
					case 4:		strTemp = _LS(IDS_WG_CMD__ADDD__SRC);			   break;
					case 5:		strTemp = _LS(IDS_WG_CMD__ADDD__USER);		  	 break;
				}
				SetValueRange(CGXRange(nRowIndex,3), strTemp);
			}
			else if(m_ESEQSel.nType == 2) 
			{
				strTemp = _LS(IDS_WG_CMD__ADD2__Mat_Name);
				SetValueRange(CGXRange(nRowIndex,2), strTemp);
				ChangeMaterialData(nRowIndex);

				m_pDoc->m_pAttrCtrl->GetMatl(m_ESEQSel.KeyMatl, TempMatlData);
				strTemp.Format(_T("%d: "),m_ESEQSel.KeyMatl);
				strTemp = strTemp + TempMatlData.Name;
				SetValueRange(CGXRange(nRowIndex,3), strTemp);
			}	
			
			m_pDoc->m_pAttrCtrl->GetStor(m_ESEQSel.KeyStorModel, TempStorData);
			SetValueRange(CGXRange(nRowIndex,4), TempStorData.StoryName);
			nRowIndex++;
		}

	}

	LockUpdate(FALSE);
	Redraw();

	m_nOldRow = 3;
	m_bInsertMode = FALSE;
	m_bModified = FALSE;	
	m_bStepModified = FALSE;

	m_bPureCellChange = FALSE;
	SetCurrentCell(3, 1);  //Focus 받는것 수정하기.
	m_bPureCellChange = TRUE;
}

// 멍청한 사용자가 Step값을 아무렇게나 넣을경우 순서대로 입력되도록 고치는 부분.
void CCMLoadESEQSpreadGrid::CheckStepData(ROWCOL nRow)
{
	CString strUpStep, strCurStep;

	strUpStep = GetValueRowCol(GetRowCount()-2,1);
	strCurStep = GetValueRowCol(GetRowCount()-1,1);

	if(_ttoi(strCurStep) - _ttoi(strUpStep) > 1)
	{
		strUpStep.Format(_T("%d"), _ttoi(strUpStep)+1);
		SetValueRange(CGXRange(nRow,1), strUpStep);
	}
}

// Step 데이타를 지우거나 수정할 경우 Row의 순서를 바꿔주고 Load쪽 Story정보를 필요한 경우 재 설정한다.
void CCMLoadESEQSpreadGrid::ChangeStepData(ROWCOL nRow, ROWCOL nCol)
{
	CString startStory, endStory;
	BOOL bCheck=FALSE;    // 수정 .Insert모드시 nRow일때는 건너뜀
	BOOL bIsStepKey=FALSE;
	BOOL bIsDeleteOneStep = FALSE;
	ROWCOL nStepRow = 0;
	ROWCOL nToRow = 0;
	CString str;
	BOOL bFlag=TRUE;
	str = GetValueRowCol(nRow,1);

	m_bPureCellChange = FALSE;

	LockUpdate(TRUE);

	// Step 데이타를 지워 버렸을 경우.
	if(str == _T(""))
	{
		CString strStep;

		if(CheckCellEnable(nRow)) // 하위 Step데이타와 같은 값으로.
		{
			strStep = GetValueRowCol(nRow-1, 1);
			strStep.Format(_T("%d"), _ttoi(strStep) + 1);
			SetValueRange(CGXRange(nRow, 1), strStep);
		}
		else
		{
			strStep = GetValueRowCol(nRow-1, 1);
			SetValueRange(CGXRange(nRow, 1), strStep);
		}
		LockUpdate(FALSE);
		RedrawRowCol(nRow,1);
		m_bPureCellChange = TRUE;
		return;
	}

	CString strTemp;

	if(CheckCellEnable(nRow))  // 현재 Step의 Story데이타를 저장한다.
	{
		startStory = GetValueRowCol(nRow,5);
		endStory = GetValueRowCol(nRow,6);
		bIsStepKey = TRUE;
	}

	if(CheckCellEnable(nRow+1) && (nRow+1 != GetRowCount()))
		bIsDeleteOneStep = TRUE;

	for(int i=3; i<(signed int)GetRowCount(); i++)
	{
		if(i == nRow) continue;   //자기자신은 건넌다.
		strTemp = GetValueRowCol(i, 1);

		if((str == strTemp) && bFlag)
		{
			SetStyleRange(CGXRange(nRow,6),CGXStyle().SetEnabled(FALSE));			
			SetValueRange(CGXRange(nRow,5,nRow,6),_T(""));
			bCheck = TRUE;
			bFlag = FALSE;
		}

		if(bCheck && (str == strTemp))
			nStepRow = i;
	}

	ROWCOL nRowTemp;	
	if(bCheck)    // 똑같은 Step이 있을때.
	{
		if(nRow >= nStepRow)  nRowTemp = nRow + 1; // 수정된 Row보다 더 뒤에 Insert될 경우.
		else                  nRowTemp = nRow;

		if(nRow < nStepRow)  // 수정된 Row보다 더 뒤에 Insert될 경우.
		{
			if(nRow != nStepRow)
				MoveRows(nRow, nRow, nStepRow);
			m_nOldRow = nStepRow;	  
			m_bModified = TRUE;
		}
		else                 // 수정된 Row보다 더 앞에 Insert될 경우.
		{
			if(nRow != nStepRow+1)
				MoveRows(nRow, nRow, nStepRow+1);
			m_nOldRow = nStepRow+1;
			m_bModified = TRUE;
		}

//		if(!CheckCellEnable(nRow))
		if(bIsStepKey && (nRowTemp != GetRowCount()))
		{
			SetStyleRange(CGXRange(nRowTemp,6),CGXStyle().SetEnabled(TRUE));			
			InitStoryData(nRowTemp);
			SetValueRange(CGXRange(nRowTemp,5), startStory);
			SetValueRange(CGXRange(nRowTemp,6), endStory);  
		}
	}
	else    // 똑같은 Step이 없을때.
	{
 	  nRowTemp = nRow;
		if(nRow < GetRowCount()-1)   // Step을 변경했는데 같은 Step이 없어서 마지막에 Insert될 경우.
		{
			if(nRow != GetRowCount()-1)
				MoveRows(nRow,nRow, GetRowCount()-1);
			m_nOldRow = GetRowCount()-1;
			SetCurrentCell(GetRowCount()-1, nCol);
			m_bModified = TRUE;

			if(!CheckCellEnable(nRow))
			{
				SetStyleRange(CGXRange(nRow,6),CGXStyle().SetEnabled(TRUE));			
				InitStoryData(nRow);
				SetValueRange(CGXRange(nRow,5), startStory);
				SetValueRange(CGXRange(nRow,6), endStory);
			}
		}
		else                         // 가장 마지막데이타의 Step을 변경 한경우.
		{
			m_bModified = TRUE;
			m_nOldRow = GetRowCount()-1;
		}

		CheckStepData(m_nOldRow);

		SetStyleRange(CGXRange(m_nOldRow,6),CGXStyle().SetEnabled(TRUE));			
		InitStoryData(m_nOldRow);
		SetValueRange(CGXRange(m_nOldRow,5), _LS(IDS_WG_CMD__ADDD__None));
		SetValueRange(CGXRange(m_nOldRow,6), _LS(IDS_WG_CMD__ADDD__None));
	}

	if(bIsDeleteOneStep)
	{
		InitStoryData(nRowTemp);
		SetStartStoryName(nRowTemp);
		StepRenumberAll(nRowTemp);
	}

	LockUpdate(FALSE);
	Redraw();

	m_bPureCellChange = TRUE;
}

// Load Story정보에서 Start나 End에 일반 Story정보와 None이 같이 있지 못하도록 한다.
void CCMLoadESEQSpreadGrid::ChangeStoryData(ROWCOL nRow, ROWCOL nCol)
{
	BOOL bStartEnd;  // True for 6, FALSE for 5
	ROWCOL nNewCol;
	CString strStory;
	CString strCompare;
	T_STOR_K nStoryKey;

	LockUpdate(TRUE);
	strStory = GetValueRowCol(nRow, nCol);

	bStartEnd = nCol - 5;
	if(bStartEnd)	nNewCol = 5;
	else          nNewCol = 6;

	if(strStory == _LS(IDS_WG_CMD__ADDD__None))   // None으로 바꾸었을때.
	{
		strCompare = GetValueRowCol(nRow, nNewCol);
		if(strCompare != _LS(IDS_WG_CMD__ADDD__None))
			SetValueRange(CGXRange(nRow, nNewCol), strStory);

		InitStoryData(nRow, 0);
	}
	else	// 다른층으로 바꾸었을때.
	{
		if(bStartEnd) // End Story를 바꾸었을때
		{
			strCompare = GetValueRowCol(nRow, nNewCol);
			
			if(strCompare == _LS(IDS_WG_CMD__ADDD__None))
				SetStartStoryName(nRow);

			nStoryKey = m_pDoc->m_pAttrCtrl->GetStorKey(strStory);
			InitStoryData(nRow, nStoryKey);
		}
		else  // Start Story를 바꾸었을때
		{
			strCompare = GetValueRowCol(nRow, nNewCol);
			
			if(strCompare == _LS(IDS_WG_CMD__ADDD__None))
			{
				SetValueRange(CGXRange(nRow, nNewCol), strStory);
				SetStartStoryName(nRow);
				nStoryKey = m_pDoc->m_pAttrCtrl->GetStorKey(strStory);
				InitStoryData(nRow, nStoryKey);
			}
			else
				SetStartStoryName(nRow);
		}
	}	
	LockUpdate(FALSE);
	RedrawRowCol(CGXRange(3,5,GetRowCount(),6));
}

void CCMLoadESEQSpreadGrid::DeleteInvalidStepData(ROWCOL nRow)
{
	CString strLoadStoryName;
	CString strModel1,strModel2,strModel3;
	for(int i=GetRowCount()-1; i>nRow; i--)
	{
		if(CheckCellEnable(i))
		{
			strLoadStoryName = GetValueRowCol(i,6);
			strModel1 = GetValueRowCol(i,2);	
			strModel2 = GetValueRowCol(i,3);
			strModel3 = GetValueRowCol(i,4);   
			if((strModel1 == _T("")) &&	(strModel1 == _T("")) && (strModel1 == _T("")) && (strLoadStoryName == _LS(IDS_WG_CMD__ADDD__None)))	// Model 족의 데이타를 검사한다.
			{
				RemoveRows(i,i);
				if(i != GetRowCount())
				{
					InitStoryData(i);
					SetStyleRange(CGXRange(i,6), CGXStyle().SetEnabled(TRUE));
					SetValueRange(CGXRange(i,5,i,6), _LS(IDS_WG_CMD__ADDD__None));
				}
			}
		}
	}
}

// 수정된 Load Story 정보때문에 Story정보가 바뀌어야 할때.
void CCMLoadESEQSpreadGrid::InitStoryData(ROWCOL nRow, int nStoryKey)
{
	CString strStory;
	CString strStoryList;
	int nStoryUpKey = 0;
	int nStoryDownKey = 0;
	T_STOR_D StorData;

	if(nStoryKey)  // Story를 설정되어있는 층으로 바꿨을 경우.
	{
		BOOL bDeleteInvalidStepData = FALSE;

		for(int i=nRow+1; i<GetRowCount(); i++)
			if(CheckCellEnable(i))
			{
				strStory = GetValueRowCol(i, 6);

				strStoryList = GetComboStoryName(nStoryKey);
				SetStyleRange(CGXRange(i,5,i,6), CGXStyle().SetChoiceList(strStoryList));	
				if(strStory == _LS(IDS_WG_CMD__ADDD__None)) continue;

				nStoryDownKey = m_pDoc->m_pAttrCtrl->GetStorKey(strStory);
				if(nStoryKey >= nStoryDownKey)
				{
					SetValueRange(CGXRange(i, 5, i, 6), _LS(IDS_WG_CMD__ADDD__None));
					bDeleteInvalidStepData = TRUE;
					continue;
				}
				else
					SetStartStoryName(i);

				break;
			}	
			
		// Load쪽에 None인 데이타와 Model쪽에 아무내용도 없을경우 지워버린다.
		if(bDeleteInvalidStepData)
			DeleteInvalidStepData(nRow);
	}
	else           // Story를 None으로 바꿨을때.
	{
		for(int i=3; i<nRow; i++)
			if(CheckCellEnable(i))
			{
				strStory = GetValueRowCol(i, 6);
				if(strStory == _LS(IDS_WG_CMD__ADDD__None)) continue;

				strStory = GetValueRowCol(i,6);
				nStoryUpKey = m_pDoc->m_pAttrCtrl->GetStorKey(strStory);
			}

		m_pDoc->m_pAttrCtrl->GetStor(nStoryUpKey+1, StorData);
		
		for(int i=nRow+1; i<GetRowCount(); i++)
			if(CheckCellEnable(i))
			{
				strStory = GetValueRowCol(i, 6);
				if(strStory == _LS(IDS_WG_CMD__ADDD__None)) continue;

				strStoryList = GetComboStoryName(nStoryUpKey);
				SetStyleRange(CGXRange(i,5,i,6), CGXStyle().SetChoiceList(strStoryList));	
				SetValueRange(CGXRange(i,5,i,5), StorData.StoryName);
				break;
			}
	}
}

// 새로 입력되는 Row를 위해서.
void CCMLoadESEQSpreadGrid::InitStoryData(ROWCOL nRow)  
{
	// Story 정보 가져온다.
	T_STOR_K nStorUpKey = 0;
	CString strStoryName = _LS(IDS_WG_CMD__ADDD__None_n);
	CString strUpStoryName;

	
	if(nRow==3)   //First Row는 무조건 Story 정보를 모두 다 가지고 있다.
	{
		strStoryName = GetComboStoryName(0);
	}
	else  // 아닌경우는 Upper Row의 정보를 참조해서 Story 정보를 가지고 있어야 한다.
	{
		int i;

		for(i = nRow-1; i>2; i--)
		{
			if(CheckCellEnable(i))
			{
				strUpStoryName = GetValueRowCol(i, 6);	
				nStorUpKey = m_pDoc->m_pAttrCtrl->GetStorKey(strUpStoryName);

				if(nStorUpKey > 0)
					break;
			}
		}
	
		if(nStorUpKey == 0)   // Upper Row의 Story가 전부 None인 경우 
			strStoryName = GetComboStoryName(0);
		else                  // Upper Row중 Story정보를 가지고 있는경우.
			strStoryName = GetComboStoryName(nStorUpKey);
	}
		
	SetStyleRange(CGXRange(nRow,5,nRow,6), CGXStyle().SetChoiceList(strStoryName));		
}

// Load쪽 Start Story 이름을 나타내는 부분.
void CCMLoadESEQSpreadGrid::SetStartStoryName(ROWCOL nRow)
{
	if(!CheckCellEnable(nRow))
		return;

	CString strStoryList;
	CGXStyle typeStyle;
	int nLength;

	GetStyleRowCol(nRow, 5, typeStyle, gxCopy, GX_VALUE_TEXT);			
	strStoryList = typeStyle.GetChoiceList();

	if(strStoryList.GetLength() > 4)
		strStoryList.TrimLeft(_LS(IDS_WG_CMD__ADDD__None_n));

	nLength = strStoryList.Find(_T("\n"),0);  
	if(nLength == -1) nLength = strStoryList.GetLength();

	strStoryList = strStoryList.Left(nLength);
	SetValueRange(CGXRange(nRow, 5), strStoryList);
}

// 현재 Row의 Load쪽 Story를 입력, 수정할수 있는 상태인지 검사하는 부분.
BOOL CCMLoadESEQSpreadGrid::CheckCellEnable(ROWCOL nRow)
{
	BOOL bCheckEnable = TRUE;
	CGXStyle typeStyle;
	GetStyleRowCol(nRow, 6, typeStyle, gxCopy, GX_VALUE_TEXT);

	if(typeStyle.GetIncludeEnabled())
			bCheckEnable = typeStyle.GetEnabled();

	return bCheckEnable;
}

// Cell 내용이 수정되었을 경우.
void CCMLoadESEQSpreadGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	m_bModified = TRUE;
	CString strUpStep,strDownStep;
	BOOL bUpCellEnable,bDownCellEnable;
	BOOL bSameStepData=FALSE;

	switch(nCol)
	{
		case 1:
			m_bStepModified = TRUE;
			if(nRow == GetRowCount())  // Last Row일때 현재 Row의 Validation을 검사해서 Row를 추가할지 결정.
			{
				ROWCOL ncRow, ncCol;
				GetCurrentCell(ncRow, ncCol);
				CGXControl* pControl = GetControl(ncRow, ncCol);

				BOOL bOldLock = LockUpdate(TRUE); // 수정이 종료된 후 다시 그린다.
				BOOL bActive = FALSE, bEdit = FALSE;
				CString val; int nStart, nEnd;  // 현재 셀의 텍스트와 caret 위치
				if (pControl && pControl->IsActive()) bActive = TRUE;
				if (bActive) 
				{
					bEdit = pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl));
					if (bEdit) ((CGXEditControl*)pControl)->GetSel(nStart, nEnd);
					pControl->GetCurrentText(val);
					pControl->SetActive(FALSE);
				}

				InsertRows(nRow+1,1);

				if (bActive) 
				{
					pControl->SetActive(TRUE);
					pControl->SetCurrentText(val);
					if (bEdit) ((CGXEditControl*)pControl)->SetSel(nStart, nEnd);
				}

				LockUpdate(bOldLock);
				if (!bOldLock) RedrawRowCol(nRow,1);
			}

			strUpStep = GetValueRowCol(nRow,1);
			strDownStep = GetValueRowCol(nRow+1,1);

			bUpCellEnable = CheckCellEnable(nRow);
			bDownCellEnable = CheckCellEnable(nRow+1);
			if(bUpCellEnable && bDownCellEnable)
			{
				if(_ttoi(strDownStep) - _ttoi(strUpStep) == 1)
					bSameStepData=TRUE;
			}
			else if(bUpCellEnable && !bDownCellEnable)
			{
				if(_ttoi(strDownStep) == _ttoi(strUpStep))
					bSameStepData=TRUE;
			}
			m_bSameStepData = bSameStepData;

			if((strUpStep != _T("")) && !bSameStepData)  // Step 데이타 수정시 Delete 가 되면서 ModifyCell이 먼저 호출된다.
				if(!CheckCellEnable(nRow) || (strUpStep != strDownStep)) // Modify가 되는데 원래값과 똑같을때.
					ChangeEnableDisableLoadStory(nRow);
			break;
		case 2:
			ChangeMaterialData(nRow);
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
		case 6:
			CString strCellContent;
			strCellContent = GetValueRowCol(nRow, nCol);
			if(strCellContent == _T(""))
			{
				strCellContent = _LS(IDS_WG_CMD__ADDD__None);
				SetValueRange(CGXRange(nRow, nCol), strCellContent);
			}
			ChangeStoryData(nRow, nCol);
			break;
	}

	CGXGridWnd::OnModifyCell(nRow, nCol);
}

// 하나의 Row가 삭제되었을 때.
BOOL CCMLoadESEQSpreadGrid::OnDeleteRecord(ROWCOL nRow)
{
	CString strCurStep;
	CString strStartStory;	
	CString strEndStory;	
	BOOL bCheckEnable;
	CRowColArray awRows;

	m_bModified = FALSE;
	LockUpdate(TRUE);

	bCheckEnable = CheckCellEnable(nRow);
	if(bCheckEnable)
	{
		strCurStep = GetValueRowCol(nRow, 1);
		strStartStory = GetValueRowCol(nRow, 5);	
		strEndStory = GetValueRowCol(nRow, 6);	
	}

	CCMLoadESEQTBGrid::OnDeleteRecord(nRow);
	
	if(nRow == GetRowCount())
	{
		LockUpdate(FALSE);
		Redraw();
		return TRUE;
	}

	if(bCheckEnable)  // Renumbering 필요할 수 도 있다.
	{
		if(GetValueRowCol(nRow,1) == strCurStep) // Renumbering 필요없고, Enabled로 만든다.
		{
//			SetStyleRange(CGXRange(nRow, 5, nRow, 6), CGXStyle().SetEnabled(TRUE));
			SetStyleRange(CGXRange(nRow, 6), CGXStyle().SetEnabled(TRUE));
			SetValueRange(CGXRange(nRow, 5), strStartStory);
			SetValueRange(CGXRange(nRow, 6), strEndStory);
			InitStoryData(nRow);
		}
		else
		{
			InitStoryData(nRow);
			SetStartStoryName(nRow);
			StepRenumber(nRow);
		}
	}
	
	LockUpdate(FALSE);
	Redraw();
	return TRUE;
}

// 여러개의 Row가 삭제되었을 때.
BOOL CCMLoadESEQSpreadGrid::OnDeleteRecords(CRowColArray &awRows)
{
	CString strEndStory;	
	BOOL bCheckEnable;

	m_bModified = FALSE;
	LockUpdate(TRUE);
	m_bPureCellChange = FALSE;

	BOOL bIsMoveCurrentFocus = FALSE;   // CurrentCell이 바뀌는지 검사하기위한 부분.
	BOOL bCheckLastRow = FALSE;         // 마지막 Row에 Focus가 있는지 검사한다.
	ROWCOL ncRow,ncCol;
	GetCurrentCell(ncRow,ncCol);
	if(ncRow == GetRowCount()) bCheckLastRow = TRUE;  // 현재 Row가 GetRowCount()와 같으면 Delete후 Focus를 잃어버린다.
																										// 그럴경우 죽는데 그것을 방지하기 위해 Flag사용.

	bCheckEnable = CheckCellEnable(awRows.GetAt(awRows.GetSize()-1)+1); // 선택된 바로 다음 Row가 Enable인지 체크
	if(!bCheckEnable)
	{
		for(int j=awRows.GetAt(awRows.GetSize()-1); j>2; j--)
			if(CheckCellEnable(j))
			{
				strEndStory = GetValueRowCol(j,6);
				SetStyleRange(CGXRange(awRows.GetAt(awRows.GetSize()-1)+1,6), CGXStyle().SetEnabled(TRUE));
				break;
			}
	}

	ROWCOL nToRow,nFromRow;

	if(awRows.GetAt(awRows.GetSize()-1) == GetRowCount())
		nToRow = awRows.GetAt(awRows.GetSize()-1)-1;
	else
		nToRow = awRows.GetAt(awRows.GetSize()-1);
	
	nFromRow = awRows.GetAt(0);

	if((nFromRow <= ncRow) && ( ncRow <= nToRow))   // CurrentCell이 바뀌어야 하는지 검사
		bIsMoveCurrentFocus = TRUE;

	 // 실제로 지운다.
	for(int i=nToRow; i >= nFromRow; i--)
		RemoveRows(i,i);

	if(bCheckLastRow) SetCurrentCell(GetRowCount(), ncCol);   // If문이 이상한데 LastRow일경우 위에서 Delete할 Row를 
	else if(bIsMoveCurrentFocus) SetCurrentCell(nFromRow,1);  // 결정할때 LastRow는 제거하기 때문에 각각 If문으로 한다.

	// Story정보를 재 설정한다.
	InitStoryData(nFromRow);

	if(nFromRow != GetRowCount())
	{
		SetStartStoryName(nFromRow);
	}
	StepRenumberAll(nFromRow);

	if(strEndStory != _T(""))
		SetValueRange(CGXRange(nFromRow, 6), strEndStory);

	m_bPureCellChange = TRUE;
	LockUpdate(FALSE);
	Redraw();
	return TRUE;
}

// 한 Step전부의 내용이 삭제되었거나 Step값이 수정되었을 경우 Step번호를 재 설정한다.
void CCMLoadESEQSpreadGrid::StepRenumber(ROWCOL nRow)
{
	CString strTemp;

//	LockUpdate(TRUE);
	strTemp = GetValueRowCol(nRow,1);
	if((nRow == 3) && strTemp == _T("1"))
		return;

	for(int i=nRow; i<GetRowCount(); i++)
	{
		strTemp = GetValueRowCol(i,1);
		strTemp.Format(_T("%d"), _ttoi(strTemp) -1);	
		SetValueRange(CGXRange(i,1), strTemp);
	}

//	LockUpdate(FALSE);  // OndeleteRecord, ChangeStepData에서 불린다.
	RedrawRowCol(CGXRange(3,1,GetRowCount(),1));
}

// 한 Step전부의 내용이 삭제되었거나 Step값이 수정되었을 경우 Step번호를 재 설정한다.
void CCMLoadESEQSpreadGrid::StepRenumberAll(ROWCOL nRow)
{
	BOOL bSameStep = FALSE;
	CString strUpStep, strCurStep, strDownStep;
	CString strTempStep;
	
	for(int i=nRow; i<GetRowCount(); i++)
	{
		strUpStep = GetValueRowCol(i-1,1);
		strCurStep = GetValueRowCol(i,1);
		strDownStep = GetValueRowCol(i+1,1);

		if(_ttoi(strCurStep) - _ttoi(strUpStep) > 1)
		{
			if(bSameStep)
				SetValueRange(CGXRange(i,1), strUpStep);
			else
			{
				strUpStep.Format(_T("%d"), _ttoi(strUpStep)+1);
				SetValueRange(CGXRange(i,1), strUpStep);
			}
		}
		else
		{
			if(bSameStep)
				SetValueRange(CGXRange(i,1), strUpStep);
		}

		if(strCurStep == strDownStep)	bSameStep = TRUE;
		else bSameStep = FALSE;
	}
}

// Main Grid와 같이 Keyboard입력을 받을수 있게 한다.
BOOL CCMLoadESEQSpreadGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	// ESC 키는 완전히 죽인다.
	if(nChar == VK_ESCAPE)
		return FALSE;
	
	BOOL bCheckInsert = FALSE;
	// CheckField에는 m_nOldRow에 대해 Check를 하는데 InsertRow가 되면서 m_nOldRow가 가르키는 곳이
	// Insert 된 Row 가 된다. 
	if((nMessage == WM_KEYDOWN) && (nChar == VK_INSERT))  
	{
		if(CheckField()) 
		{
			ROWCOL ncRow, ncCol;
			GetCurrentCell(ncRow, ncCol);
			if(ncRow == GetRowCount()) return TRUE;
			m_bInsertMode = TRUE;
			bCheckInsert = TRUE;
//			++m_nOldRow;
		}
		else return TRUE;
	}
	
	ROWCOL nRow,nCol;
	GetCurrentCell(nRow, nCol);

	CCMLoadESEQTBGrid::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);  // OnInsertEmptyRecord 함수 호출

	ROWCOL ncRow,ncCol;
	GetCurrentCell(ncRow, ncCol);

	if(bCheckInsert) 
	{
		if(ncRow != 3)
		{
			m_bModified = TRUE;
		}
		return TRUE;
	}

//	if(ncRow != m_nOldRow)           // OperateSpread 수정
//	if((nCol != ncCol) || (nRow != ncRow))
	if(nRow != ncRow)
		OperateDataSpread(ncRow, ncCol);

	return TRUE;
}

void CCMLoadESEQSpreadGrid::OperateDataSpread(ROWCOL nRow, ROWCOL nCol)
{
	if(nRow < 3) return;
	
	m_bPureCellChange = FALSE;

	if(m_nOldRow == nRow)
	{
		m_bPureCellChange = TRUE;
		return;  
	}

	if(m_bModified)
	{
		if(!CheckField())
		{
			if(CheckAllBlankRow(m_nOldRow))
			{
				RemoveRows(m_nOldRow,m_nOldRow);
				Redraw();
			}
			else
			{
				if(CheckCellEnable(m_nOldRow))
					SetCurrentCell(m_nOldRow ,nCol);	
				else
				{
					if(nCol > 4) nCol = 4;
					SetCurrentCell(m_nOldRow, nCol);
				}
			}
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

	m_bPureCellChange = TRUE;
}

// Sub Grid내 데이타의 Validation을 Check한다.
BOOL CCMLoadESEQSpreadGrid::CheckField()
{
	if(!m_bModified) return TRUE;

	BOOL bCheckLoad = TRUE;
	BOOL bCheckModel = TRUE;
	BOOL bCheckDataValid = TRUE;  // Model쪽에 데이타가 하나도 없는데 Load 쪽 스토리가 _LS(IDS_WG_CMD__ADDD__None)일경우.
	BOOL bCheckEtc = TRUE;

	CString str1,str2,str3,str4,str5,str6,str7,str8;

	str1 = GetValueRowCol(m_nOldRow,1);  // Step Number
	str2 = GetValueRowCol(m_nOldRow,2);  // Model 쪽 Type
	str3 = GetValueRowCol(m_nOldRow,3);  // Model 쪽 Material
	str4 = GetValueRowCol(m_nOldRow,4);  // Model 쪽 Story   여기서는 StoryName 이 None이 될 수 없다.
	str5 = GetValueRowCol(m_nOldRow,5);  // Load Start Story
	str6 = GetValueRowCol(m_nOldRow,6);  // Load End Story
	str7 = GetValueRowCol(m_nOldRow,7);  // Stage Duration
	str8 = GetValueRowCol(m_nOldRow,8);  // Member Age

	if (CheckCellEnable(m_nOldRow))
	{
		bCheckLoad = ((str1 != _T("")) && (str5 != _T("")) && (str6 != _T("")));		
		bCheckModel = ((str2 != _T("")) && (str3 != _T("")) && (str4 != _T(""))) || ((str2 == _T("")) && (str3 == _T("")) && (str4 == _T("")));
		bCheckEtc = (str7 != _T("")) && (str8 != _T(""));
		if (bCheckEtc) bCheckEtc = (_tstof(str7) >= 0.0) && (_tstof(str8) >= 0.0);
	}
	else
	{
		bCheckLoad = (str1 != _T(""));
		bCheckModel = ((str2 != _T("")) && (str3 != _T("")) && (str4 != _T("")));
		//bCheckEtc = (str7 != _T("")) && (str8 != _T(""));
		//if (bCheckEtc) bCheckEtc = (_tstof(str7) >= 0.0) && (_tstof(str8) >= 0.0);
	}

	if(!CheckValidRow(m_nOldRow))  // Story가 None이면.
		bCheckDataValid = (str2 != _T("")) && (str3 != _T("")) && (str4 != _T(""));			

	BOOL bOK = bCheckLoad && bCheckModel && bCheckDataValid && bCheckEtc;
	if (!bOK) AfxMessageBox(_LS(IDS_CMD_ERROR_Input_Grid_Data));
	return bOK;
}

// Check Field랑 같이 
BOOL CCMLoadESEQSpreadGrid::CheckValidRow(ROWCOL nRow)
{
	int i = nRow;
	for(i=nRow; i>2 ; i--)
	{
		if(CheckCellEnable(i))
			break;
	}

	CString strTempStoryName = GetValueRowCol(i,6);
	
	if(strTempStoryName == _LS(IDS_WG_CMD__ADDD__None))
		return FALSE;
	else
		return TRUE;
}

// 전체 Row가 아무데이타가 없는지 확인한다.
BOOL CCMLoadESEQSpreadGrid::CheckAllBlankRow(ROWCOL nRow)
{
	CString str1,str2,str3,str4,str5,str6;

	str1 = GetValueRowCol(m_nOldRow,1);  // Step Number
	str2 = GetValueRowCol(m_nOldRow,2);  // Model 쪽 Type
	str3 = GetValueRowCol(m_nOldRow,3);  // Model 쪽 Material
	str4 = GetValueRowCol(m_nOldRow,4);  // Model 쪽 Story   여기서는 StoryName 이 None이 될 수 없다.
	str5 = GetValueRowCol(m_nOldRow,5);  // Load Start Story
	str6 = GetValueRowCol(m_nOldRow,6);  // Load End Story

	return ((str1 == _T("")) && (str2 == _T("")) && (str3 == _T("")) && (str4 == _T("")) && (str5 == _T("")) && (str6 == _T("")));
}

const CString& CCMLoadESEQSpreadGrid::GetValueRowCol(ROWCOL nRow, ROWCOL nCol)  // Override
{
	static CString value;
	value = _T("");

	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	if (ncRow != nRow || ncCol != nCol)
		return CGXGridWnd::GetValueRowCol(nRow, nCol);

	CGXControl *pControl = GetControl(nRow, nCol);

	if (!pControl || !pControl->IsActive() || !pControl->GetModify())
		return CGXGridWnd::GetValueRowCol(nRow, nCol);

	pControl->GetCurrentText(value);  
	return value;
}

BOOL CCMLoadESEQSpreadGrid::OnInsertEmptyRecord(ROWCOL nRow)  // Override
{
	// Row가 입력되어 값을 저장할 수 있도록 CArray에 초기값을 가진 데이타를 Add/Insert 한다.
	if(!m_bInsertMode) return FALSE;

	m_bPureCellChange = FALSE;	

	if(nRow != 3)
	{
		CString strStepNo;
		strStepNo = GetValueRowCol(nRow-1, 1);
		SetValueRange(CGXRange(nRow,1), strStepNo);
		SetStyleRange(CGXRange(nRow,6), CGXStyle().SetEnabled(FALSE));
	}
		
	SetCurrentCell(nRow, 1);             
	m_bModified = TRUE;
	CCMLoadESEQTBGrid::m_nEditMode = noMode;
	m_bInsertMode = FALSE;

	m_bPureCellChange = TRUE;
	return TRUE;
}

BOOL CCMLoadESEQSpreadGrid::SaveESEQData()
{
	int nCountModel=0;

	if(!CheckField())
		return FALSE;

	CString strTemp;
	CString strPreStep;
	CString strPostStep;
	m_ESEQData.Initialize();
	m_ESEQBase.Initialize();

	for(int i=3; i <= GetRowCount(); i++)
	{
		strPostStep = GetValueRowCol(i, 1);		

		if((strPreStep != strPostStep) && (i != 3))
		{
			m_ESEQData.arEseq.Add(m_ESEQBase);
			
			if(i == GetRowCount())  break;

			m_ESEQBase.Initialize();
			strTemp = GetValueRowCol(i, 6);
			if(strTemp == _LS(IDS_WG_CMD__ADDD__None))
				m_ESEQBase.KeyStorLoad = 0;
			else
				m_ESEQBase.KeyStorLoad = m_pDoc->m_pAttrCtrl->GetStorKey(strTemp);

			// 추가 
			strTemp = GetValueRowCol(i, 7);
			m_ESEQBase.dDuration = _tstof(strTemp);
			strTemp = GetValueRowCol(i, 8);
			m_ESEQBase.dAge = _tstof(strTemp);

			nCountModel = 0;
		}
		
		if (i==3)
		{
			strTemp = GetValueRowCol(i, 6);
			m_ESEQBase.KeyStorLoad = m_pDoc->m_pAttrCtrl->GetStorKey(strTemp);
		}
		
		m_ESEQSel.Initialize();
		GetModelData(i, m_ESEQSel);	
		m_ESEQBase.EseqSel[nCountModel] = m_ESEQSel;
		
		nCountModel++;
		strPreStep = GetValueRowCol(i, 1);		
	}

	return TRUE;
}

void CCMLoadESEQSpreadGrid::GetModelData(ROWCOL nRow, T_ESEQ_SEL& ESEQSel)
{
	unsigned int nTemp;
	CString strMatlKey;

	if(GetValueRowCol(nRow, 2) == _LS(IDS_WG_CMD__ADD2__Mat_Type))
	{
		ESEQSel.nType = 1;
				
		if(GetValueRowCol(nRow, 3) == _LS(IDS_WG_CMD__ADDD__All))
			m_ESEQSel.KeyMatl = 1;
		else if(GetValueRowCol(nRow, 3) == _LS(IDS_WG_CMD__ADDD__Steel))
			m_ESEQSel.KeyMatl = 2;
		else if(GetValueRowCol(nRow, 3) == _LS(IDS_WG_CMD__ADDD__Concrete))
			m_ESEQSel.KeyMatl = 3;
		else if(GetValueRowCol(nRow, 3) == _LS(IDS_WG_CMD__ADDD__SRC))
			m_ESEQSel.KeyMatl = 4;
		else if(GetValueRowCol(nRow, 3) == _LS(IDS_WG_CMD__ADDD__USER))
			m_ESEQSel.KeyMatl = 5;
	}
	else if(GetValueRowCol(nRow, 2) == _LS(IDS_WG_CMD__ADD2__Mat_Name))
	{
		m_ESEQSel.nType = 2;
		strMatlKey = GetValueRowCol(nRow, 3);
		strMatlKey = strMatlKey.Left(strMatlKey.Find(':', 0));
		m_ESEQSel.KeyMatl = _ttoi(strMatlKey);
	}
	
	CString strStorName;
	strStorName = GetValueRowCol(nRow, 4);
	if(strStorName != _T(""))
	{
		nTemp = m_pDoc->m_pAttrCtrl->GetStorKey(strStorName);
		m_ESEQSel.KeyStorModel = nTemp;
	}
}

void CCMLoadESEQSpreadGrid::OnInitCurrentCell(ROWCOL nRow, ROWCOL nCol)
{
	if(m_bStepModified && !m_bSameStepData)
	{
		if((m_nOldRow != nRow) || (nCol != 1))
		{
			m_bStepModified = FALSE;
			ChangeStepData(m_nOldRow, nCol);
		}
		else return;
	} 

	if(m_bPureCellChange)
		OperateDataSpread(nRow, nCol);
	
	CGXGridCore::OnInitCurrentCell(nRow, nCol);
}

void CCMLoadESEQSpreadGrid::ChangeEnableDisableLoadStory(ROWCOL nRow)
{
	m_bPureCellChange = FALSE;
	CString strCurrentStep;
	CString strStartStory, strEndStory;
	strCurrentStep = GetValueRowCol(nRow,1);

	BOOL bCheckStory=FALSE;
	BOOL bStepExist=FALSE;
	CString strCompareStep;
//	LockUpdate(TRUE);

	if(CheckCellEnable(nRow) && (nRow < GetRowCount() - 1) && !CheckCellEnable(nRow+1))
	{
		strStartStory = GetValueRowCol(nRow, 5);	
		strEndStory = GetValueRowCol(nRow, 6);	
		bCheckStory = TRUE;
	}  

	for(int i=3; i<(signed int)GetRowCount(); i++)
	{
		if(i == nRow) continue;   //자기자신은 건넌다.
		strCompareStep = GetValueRowCol(i, 1);

		if(strCurrentStep == strCompareStep)
		{
			SetValueRange(CGXRange(nRow,5,nRow,6),_T(""));
			SetStyleRange(CGXRange(nRow,6),CGXStyle().SetEnabled(FALSE));
			bStepExist=TRUE;
			break;
		}
	}

	if(!bStepExist)
	{
		SetValueRange(CGXRange(nRow,5,nRow,6),_LS(IDS_WG_CMD__ADDD__None));
		SetStyleRange(CGXRange(nRow,6),CGXStyle().SetEnabled(TRUE));
	}

	if(bCheckStory)
	{
		SetStyleRange(CGXRange(nRow+1, 6), CGXStyle().SetEnabled(TRUE));
		InitStoryData(nRow+1);
		SetValueRange(CGXRange(nRow+1, 5), strStartStory);
		SetValueRange(CGXRange(nRow+1, 6), strEndStory);
	}

//	LockUpdate(FALSE);
//	RedrawRowCol(CGXRange(nRow,5,nRow+1,6));
	m_bPureCellChange = TRUE;
}
