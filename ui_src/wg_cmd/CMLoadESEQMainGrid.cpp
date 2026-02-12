// CMLoadESEQMainGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadESEQMainGrid.h"

#include "CMLoadESEQSubGrid.h"
#include "CMStageSpreadGrid.h"

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
// CCMLoadESEQMainGrid
CCMLoadESEQMainGrid::CCMLoadESEQMainGrid()
{
	m_pDoc = NULL;
	m_pESEQSubGrid = 0;
	m_pSpreadGrid = 0;

	m_nOldRow = 1;
	m_bModified = FALSE;
	m_bRefreshSubGrid = TRUE;
}

CCMLoadESEQMainGrid::~CCMLoadESEQMainGrid()
{
}

#define CWnd CTBBrowserWnd
BEGIN_MESSAGE_MAP(CCMLoadESEQMainGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCMLoadESEQMainGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQMainGrid message handlers

void CCMLoadESEQMainGrid::Initialize(CDBDoc* pDoc)
{
	CTBBrowserWnd::Initialize();

	m_pDoc = pDoc;

	GetParam()->EnableTrackRowHeight(FALSE); 
	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetRowHeightByDPI(0, 0, 36);
	SetColCount(4);
	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// Step 컬럼의 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, 1), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0)
			.SetEnabled(FALSE)
			.SetDraw3dFrame(gxFrameRaised)  // row Header처럼 작동해서 사용자에게 혼란을 주기 때문에 3D 효과만 준다.
			);
//  SetFrozenCols(1, 1);

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
//			.SetControl(GX_IDS_CTRL_TEXTFIT)
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)    
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetVertScrollBar(FALSE)
//			.SetValue(_LS(IDS_WG_CMD__ADDD__None))
			);

	// 추가 
	SetStyleRange(CGXRange().SetCols(3, 4), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0));

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
//	GetParam()->EnableSelection(GX_SELCELL);

	// Column 크기 설정
	SetColWidthByDPI(1, 1, 40);
	SetColWidthByDPI(2, 2, 100);
	// 추가 
	SetColWidthByDPI(3, 3, 62);
	SetColWidthByDPI(4, 4, 62);

	InitStoryData(1);

	LockUpdate(FALSE);
	Redraw();

	SetCurrentCell(1,2);

	SetModeDisplayOnly(TRUE); // display only mode로 전환
}

void CCMLoadESEQMainGrid::SetHeaderTitle()
{
	CString title;
	int i;
	ROWCOL nCol;

	CString strTitle[] = { _LS(IDS_CMD_ESEQ_step), _LS(IDS_CMD_ESEQ_Story_for_Dead_Load),  
		_LS(IDS_WG_CMD__ADDD__Stage_Duration), _LS(IDS_WG_CMD__ADDD__Member_Age)};
	for(i = 0; i < 4; i++)
	{
		nCol = i + 1;
		SetStyleRange(CGXRange(0, nCol), CGXStyle()
			.SetValue(strTitle[i])
			.SetUserAttribute(ID_USER_ATTR_COL_ID, strTitle[i]));
	}
}

// 새로 입력되는 Row를 위해서.
void CCMLoadESEQMainGrid::InitStoryData(ROWCOL nRow)  
{
	// Story 정보 가져온다.
	CArray<T_STOR_D, T_STOR_D&> arStoryInfo;
	T_STOR_K nStorUpKey;
	T_STOR_D StoryInfo;
	CString strStoryName = _LS(IDS_WG_CMD__ADDD__None_n);
	CString strUpStoryName;
	T_ESEQ_BASE tempESEQBase;

	m_pDoc->m_pAttrCtrl->GetStorData(arStoryInfo); 
	
	if(nRow==1)   //First Row는 무조건 Story 정보를 모두 다 가지고 있다.
		strStoryName = GetComboStoryName(0);
	else  // 아닌경우는 Upper Row의 정보를 참조해서 Story 정보를 가지고 있어야 한다.
	{
		int i;

		strUpStoryName = GetValueRowCol(nRow-1, 2);
		nStorUpKey = m_pDoc->m_pAttrCtrl->GetStorKey(strUpStoryName);

		if(nStorUpKey == 0)  // Story Name을 None으로 선택했을 경우.
			for(i=nRow-2; i>0; i--)  // 임시 DB에서 이전 Step의 None이 아닌 Story Name을 찾는다.  
			{
				tempESEQBase.Initialize();
				tempESEQBase = m_ESEQData.arEseq.GetAt(i-1);				

				if(tempESEQBase.KeyStorLoad > 0)
				{
					nStorUpKey = tempESEQBase.KeyStorLoad;
					break;
				}
			}								
		
		if(nStorUpKey == 0)   // Upper Row의 Story가 전부 None인 경우 
			strStoryName = GetComboStoryName(0);
		else                  // Upper Row중 Story정보를 가지고 있는경우.
			strStoryName = GetComboStoryName(nStorUpKey);
	}
		
	SetStyleRange(CGXRange(nRow,2), CGXStyle().SetChoiceList(strStoryName));		
}

// Row의 Story정보가 바뀌었을때 (OnModifyCell 에서 주로 호출)
void CCMLoadESEQMainGrid::InitStoryData(ROWCOL nRow, T_STOR_K nStorKey)
{
	// Story 정보 가져온다.
	if(m_strOldStoryName == GetValueRowCol(nRow, 2))
		return;

	CString strStoryName = GetComboStoryName(nStorKey);
		
	SetStyleRange(CGXRange(nRow,2), CGXStyle().SetChoiceList(strStoryName).SetValue(_LS(IDS_WG_CMD__ADDD__None)));		
}

// Row의 Story정보중 하나가 Delete되었을때 전부다 갱신하는 기능. 
void CCMLoadESEQMainGrid::InitAllStoryData()
{
	// Story 정보 가져온다.
	T_STOR_D nStorData;
	CString strStoryName = _LS(IDS_WG_CMD__ADDD__None_n);
	T_ESEQ_BASE tempESEQBase;
	T_STOR_K nStorUpKey;

	tempESEQBase.Initialize();
	tempESEQBase = m_ESEQData.arEseq.GetAt(0);
	InitStoryData(1, 0);  //First Row는 모든 층별 정보를 가지고 있다.
	
	nStorUpKey=tempESEQBase.KeyStorLoad;

	if(tempESEQBase.KeyStorLoad)  // First Row의 선택된 값이 None이 아니면 / 이면 
	{
		m_pDoc->m_pAttrCtrl->GetStor(tempESEQBase.KeyStorLoad, nStorData);
		SetValueRange(CGXRange(1,2), nStorData.StoryName); 
	}
	else
		SetValueRange(CGXRange(1,2), _LS(IDS_WG_CMD__ADDD__None)); 

	// Second Row부터 Last Row 바로 전까지 이전 Story정보와 비교하면서 Cell에 Story정보를 입력한다.
	for(int i=1; i<m_ESEQData.arEseq.GetSize(); i++)  
	{
		tempESEQBase.Initialize();
		tempESEQBase = m_ESEQData.arEseq.GetAt(i);
		InitStoryData(i+1, nStorUpKey);
		
		if(nStorUpKey < tempESEQBase.KeyStorLoad) 
			nStorUpKey = tempESEQBase.KeyStorLoad;

		if(tempESEQBase.KeyStorLoad)
		{
			m_pDoc->m_pAttrCtrl->GetStor(tempESEQBase.KeyStorLoad, nStorData);
			SetValueRange(CGXRange(i+1,2), nStorData.StoryName); 
		}
		else
			SetValueRange(CGXRange(i+1,2), _LS(IDS_WG_CMD__ADDD__None));
	}

	InitStoryData(GetRowCount());   // 마지막에 있는 Row의 값 재정의 
}

CString CCMLoadESEQMainGrid::GetComboStoryName(int nStoryKey)
{
	int nKey;
	CString csStoryList(_T(""));

	T_STOR_D StorD;
	CArray<T_STOR_K, T_STOR_K> aStorK;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nNum = aStorK.GetSize();
	if (nStoryKey == 0) nKey = 0;
	else 
	{
		int i = 0;
		for (i=0; i<nNum; i++)
			if (nStoryKey == aStorK[i]) { nKey = i; break; }
		ASSERT(i != nNum);
	}
	for (int i=nKey; i<nNum; i++)
	{
		if (i==nKey) continue;
		m_pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD);
		csStoryList += StorD.StoryName;
		csStoryList += _T("\n");
	}
	csStoryList += _LS(IDS_WG_CMD__ADDD__None_n);
	return csStoryList;
}

// CCMLoadESEQDlg 에서 호출된다. m_ESEQData 하는 임시 DB변수에 ESEQ데이타 정보가 DB에 있다면
// m_ESEQData 변수에 Copy하고 m_ESEQData를 참조해서 Main,Sub Grid에 데이타를 표시한다.
void CCMLoadESEQMainGrid::InitESEQData()
{  
	SetModeDisplayOnly(FALSE); 

	// ESEQ 에 관한 데이타
	LockUpdate(TRUE);

	CString strTemp;
	T_STOR_D StorData;

	RemoveRows(1, GetRowCount());
	SetRowCount(m_ESEQData.arEseq.GetSize()+1);	
	int nRowCount = GetRowCount();
	int i = 0;
	for(i=0; i<m_ESEQData.arEseq.GetSize(); i++)  // 데이타 실제 입력
	{
		InitStoryData(i+1);

		m_pDoc->m_pAttrCtrl->GetStor((m_ESEQData.arEseq.GetAt(i)).KeyStorLoad, StorData);		
		
		// modified by TAE
		T_KEYSTEP keystep;
		if (!m_pSpreadGrid->GetKeyByIndex(i, 0, keystep)) ASSERT(0);
		strTemp.Format(_T("%ld"), keystep.key.serial);
		SetValueRange(CGXRange(i+1,1), strTemp);

		if((m_ESEQData.arEseq.GetAt(i)).KeyStorLoad == 0)  // Story Key 에 0 값을 가질수 있다.
			SetValueRange(CGXRange(i+1,2), _LS(IDS_WG_CMD__ADDD__None));		
		else
			SetValueRange(CGXRange(i+1,2), StorData.StoryName);		
		// 추가 
		strTemp.Format(_T("%g"), m_ESEQData.arEseq[i].dDuration);
		SetValueRange(CGXRange(i+1,3), strTemp);		
		strTemp.Format(_T("%g"), m_ESEQData.arEseq[i].dAge);
		SetValueRange(CGXRange(i+1,4), strTemp);	
	}

	InitStoryData(i+1);    // Last Row의 Story정보 설정.

	LockUpdate(FALSE);
	Redraw();
	
	SetCurrentCell(1,2);

	if(m_ESEQData.arEseq.GetSize())
		m_pESEQSubGrid->InitESEQData(m_ESEQData.arEseq.GetAt(0), m_pSpreadGrid->m_aStepNum[0]);
	else
		m_pESEQSubGrid->InitESEQData();

	m_nOldRow = 1;
	m_bModified = FALSE;

	SetModeDisplayOnly(TRUE); 
}

//Main Grid의 데이타의 Validation 검사.  
BOOL CCMLoadESEQMainGrid::CheckField(BOOL bFlag)
{
	/*
	if (!m_bModified && bFlag) return TRUE;

	BOOL bStory, bStage, bAge;
	CString strStory, strStage, strAge;
	strStory = GetValueRowCol(m_nOldRow,2);
	strStage = GetValueRowCol(m_nOldRow,3);
	strAge   = GetValueRowCol(m_nOldRow,4);

	if (strStory == _LS(IDS_WG_CMD__ADDD__None))
		bStory = !m_pESEQSubGrid->CheckExistBlankField();
	else bStory = (strStory != _T(""));

	bStage = (strStage != _T(""));
	if (bStage) bStage = (_tstof(strStage) >= 0.0);
	bAge = (strAge != _T(""));
	if (bAge) bAge = (_tstof(strAge) >= 0.0);

	return bStory && bStage && bAge;
	*/
	return TRUE;
}

//Main Grid의 데이타의 Validation 검사.  
BOOL CCMLoadESEQMainGrid::CheckValidRow()
{
	/*
	if (m_nOldRow == GetRowCount()) return TRUE;
	
	BOOL bStory, bStage, bAge;
	CString strStory, strStage, strAge;
	strStory = GetValueRowCol(m_nOldRow,2);
	strStage = GetValueRowCol(m_nOldRow,3);
	strAge   = GetValueRowCol(m_nOldRow,4);

	bStory = (strStory != _T(""));
	bStage = (strStage != _T(""));
	if (bStage) bStage = (_tstof(strStage) >= 0.0);
	bAge = (strAge != _T(""));
	if (bAge) bAge = (_tstof(strAge) >= 0.0);

	return bStory && bStage && bAge;
	*/
	return TRUE;
}

// 임시 DB인 m_ESEQData에 새로입력된 Row에 대한 정보를 입력한다.
void CCMLoadESEQMainGrid::AddESEQData(ROWCOL nRow)
{
	unsigned int nTemp;
	CString strMatlKey;
	CString strTemp;
	int nRowNumber=0;

	m_ESEQBase.Initialize();    // m_ESEQData에 Add/Insert될 변수인 m_ESEQBase 초기화.

	strTemp = GetValueRowCol(nRow,2);

	if(strTemp == _LS(IDS_WG_CMD__ADDD__None))
		m_ESEQBase.KeyStorLoad = 0;
	else
	{
		nTemp = m_pDoc->m_pAttrCtrl->GetStorKey(strTemp);	
		m_ESEQBase.KeyStorLoad = nTemp;
	}

	// 추가
	strTemp = GetValueRowCol(nRow,3);
	m_ESEQBase.dDuration = _tstof(strTemp);
	strTemp = GetValueRowCol(nRow,4);
	m_ESEQBase.dAge = _tstof(strTemp);

	for(int i=0; i<m_pESEQSubGrid->GetRowCount()-1; i++)
	{
		++nRowNumber;
		m_ESEQSel.Initialize();

		if(m_pESEQSubGrid->GetValueRowCol(i+1,1) == _LS(IDS_WG_CMD__ADD2__Mat_Type))
		{
			m_ESEQSel.nType = 1;
				
			if(m_pESEQSubGrid->GetValueRowCol(i+1,2) == _LS(IDS_WG_CMD__ADDD__All))
				m_ESEQSel.KeyMatl = 1;
			else if(m_pESEQSubGrid->GetValueRowCol(i+1,2) == _LS(IDS_WG_CMD__ADDD__Steel))
				m_ESEQSel.KeyMatl = 2;
			else if(m_pESEQSubGrid->GetValueRowCol(i+1,2) == _LS(IDS_WG_CMD__ADDD__Concrete))
				m_ESEQSel.KeyMatl = 3;
			else if(m_pESEQSubGrid->GetValueRowCol(i+1,2) == _LS(IDS_WG_CMD__ADDD__SRC))
				m_ESEQSel.KeyMatl = 4;
			else if(m_pESEQSubGrid->GetValueRowCol(i+1,2) == _LS(IDS_WG_CMD__ADDD__USER))
				m_ESEQSel.KeyMatl = 5;
		}
		else if(m_pESEQSubGrid->GetValueRowCol(i+1,1) == _LS(IDS_WG_CMD__ADD2__Mat_Name))
		{
			m_ESEQSel.nType = 2;
			strMatlKey = m_pESEQSubGrid->GetValueRowCol(i+1, 2);
			strMatlKey = strMatlKey.Left(strMatlKey.Find(':', 0));
			m_ESEQSel.KeyMatl = _ttoi(strMatlKey);               // Material Name의 Key값을 가져온다.			
		}

		strTemp = m_pESEQSubGrid->GetValueRowCol(i+1,3);
		nTemp = m_pDoc->m_pAttrCtrl->GetStorKey(strTemp);	
		m_ESEQSel.KeyStorModel = nTemp;

		if((i == 0) && (m_ESEQSel.nType == 0) && (m_ESEQSel.KeyMatl == 0) && (m_ESEQSel.KeyStorModel == 0))
			--nRowNumber;
		else
			m_ESEQBase.EseqSel[nRowNumber-1] = m_ESEQSel;
	}

	if(m_ESEQData.arEseq.GetSize() >= m_nOldRow)     // Add Or Modify 
		m_ESEQData.arEseq.SetAt(nRow-1, m_ESEQBase);
	else
		m_ESEQData.arEseq.Add(m_ESEQBase);
}

// Sub Grid의 정보가 그대로인데 Main Grid의 정보가 바뀌었을때 m_ESEQData에 저장되어 있는 값을 갱신한다.
// OnModifyCell에서 호출이 되는데 Upper Row에서 Story정보를 바꿨을때 Lower Row가 변해야 되는경우 호출.
void CCMLoadESEQMainGrid::ModifyESEQData(int nIndex)
{
	m_ESEQBase.Initialize();
	m_ESEQBase = m_ESEQData.arEseq.GetAt(nIndex-1);
	m_ESEQBase.KeyStorLoad = 0;
	
	m_ESEQData.arEseq.SetAt(nIndex-1, m_ESEQBase);
}

void CCMLoadESEQMainGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	LockUpdate(TRUE);
	BOOL bFlag = TRUE;

	CString strUpStoryName;
	CString strDownStoryName;
	T_STOR_K nStorUpKey, nStorDownKey;
	T_STOR_D nStorData;
	T_ESEQ_BASE tempESEQBase;

	if(nRow == GetRowCount())  // Last Row일때 현재 Row의 Validation을 검사해서 Row를 추가할지 결정.
	{
//		if(CheckField(FALSE))        
//		{
			SetValueRange(CGXRange(nRow,1), nRow); 
			InsertRows(nRow+1,1);
			InitStoryData(nRow+1);
//		}
	}
	else if(nRow == GetRowCount()-1)  // Last Row의 바로 위 Row가 변경되었을 경우 Last Row의 Story정보 변경.
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
	else  // 중간에서 바뀌었을때 그 다음 Row의 데이타를 갱신한다.  //시작
	{
		strUpStoryName = GetValueRowCol(nRow, 2);
		nStorUpKey = m_pDoc->m_pAttrCtrl->GetStorKey(strUpStoryName);

		if(nStorUpKey == 0)  // Story Name을 None으로 선택했을 경우.
		{
			for(int i=nRow-1; i>0; i--)
			{
				tempESEQBase.Initialize();
				tempESEQBase = m_ESEQData.arEseq.GetAt(i-1);				

				if(tempESEQBase.KeyStorLoad > 0)
				{
					nStorUpKey = tempESEQBase.KeyStorLoad;
					break;
				}
			}								// 이전 Step의 Story를 검사해 None을 포함한 가장큰 StoryKey를 찾는다.

			InitStoryData(nRow, nStorUpKey);  // 현재 Row의 Story Data 변경.

			for(int i=nRow+1; i<=GetRowCount(); i++) // None이 아닌 Data가 나올때 까지 초기화 시킴.
			{						
				if(i == GetRowCount())   // 모두 None인경우 
				{
					InitStoryData(i, nStorUpKey);
					SetValueRange(CGXRange(i,2), _T(""));
					break;
				}

				tempESEQBase.Initialize();
				tempESEQBase = m_ESEQData.arEseq.GetAt(i-1);
				
				if(tempESEQBase.KeyStorLoad == 0)  // None인경우 위에서 선택된 가장큰 StoryKey 인 nStorUpKey에 따라
				{																	 // Story Data 적용.		
					InitStoryData(i, nStorUpKey);
				}
				else          // None으로 바꿨을때 영향을 받을 Row는 Index가 선택된 Row의 Index보다 큰 Row중 
				{							// None 이 아닌 Story를 가진 Row이다. 그 Row의 Story 정보를 갱신한다.				
					InitStoryData(i, nStorUpKey);
					m_pDoc->m_pAttrCtrl->GetStor(tempESEQBase.KeyStorLoad, nStorData);
					SetValueRange(CGXRange(i,2), nStorData.StoryName); 
					break;
				}
			}
		}
		else                 // Story Name이 None이 아닌값으로 변경했을 경우.
		{
			BOOL bDeleteInvalidStepData=FALSE;
			for(int i=nRow+1; i<=GetRowCount(); i++)   
			{			
				if(i == GetRowCount())
				{
					InitStoryData(i, nStorUpKey);
					SetValueRange(CGXRange(i,2), _T(""));
					break;
				}
				
				tempESEQBase.Initialize();
				tempESEQBase = m_ESEQData.arEseq.GetAt(i-1);
				nStorDownKey = tempESEQBase.KeyStorLoad;

				if(nStorUpKey >= nStorDownKey) // 바뀐 StoryKey보다 작은 StoryKey를 가진 Row의 Story정보를 
				{															 // 전부 None으로 바꾸고 Cell의 Story정보를 Update한다.
					InitStoryData(i, nStorUpKey);
					ModifyESEQData(i);
					bDeleteInvalidStepData = TRUE;
				}
				else                           // 그보다 큰 StoryKey값을 가진 Row가 나타나면 선택할수 있는 
				{															 // Story 정보만 Update하고 선택된 Story Name은 바뀌지 않는다.
					InitStoryData(i, nStorUpKey);
					m_pDoc->m_pAttrCtrl->GetStor(nStorDownKey, nStorData);
					SetValueRange(CGXRange(i,2), nStorData.StoryName); 
					break;
				}				
			}
			// Main 쪽에 None인 데이타와 Sub쪽에 아무내용도 없을경우 지워버린다.
			if(bDeleteInvalidStepData)
				DeleteInvalidStepData(nRow);
		}
	}    // 중간에서 바뀌었을때 그 다음 Row의 데이타를 갱신한다.  //끝   

	LockUpdate(FALSE);
	Redraw();

	m_nOldRow = nRow;
	m_bModified = TRUE;
}

// Main 쪽에 None인 데이타와 Sub쪽에 아무내용도 없을경우 지워버린다.
void CCMLoadESEQMainGrid::DeleteInvalidStepData(ROWCOL nRow)
{
	T_ESEQ_BASE tempESEQBase;

	BOOL bCheckData = FALSE; // Update Grid를 위해서.
	BOOL bCheckData1 = TRUE;
	BOOL bCheckData2 = TRUE;
	for(int i = GetRowCount()-1; i>nRow; i--)
	{
		tempESEQBase.Initialize();
		tempESEQBase = m_ESEQData.arEseq.GetAt(i-1);				
		
		if(!tempESEQBase.KeyStorLoad)  // KeyStorLoad 가 None일때.
		{
			bCheckData1 = tempESEQBase.EseqSel[0].nType;						
			bCheckData1 = tempESEQBase.EseqSel[0].KeyMatl;
			bCheckData1 = tempESEQBase.EseqSel[0].KeyStorModel;

			bCheckData2 = tempESEQBase.EseqSel[1].nType;						
			bCheckData2 = tempESEQBase.EseqSel[1].KeyMatl;
			bCheckData2 = tempESEQBase.EseqSel[1].KeyStorModel;

			if(!bCheckData1 && !bCheckData2) // Delete
			{
				m_ESEQData.arEseq.RemoveAt(i-1);
				bCheckData = TRUE;
			}
			else if(!bCheckData1 && bCheckData2)
			{
				int j = 0;
				for(j = 0; j<13; j++)
				{
					m_ESEQData.arEseq[i-1].EseqSel[j] = m_ESEQData.arEseq[i-1].EseqSel[j+1];

					bCheckData2 = tempESEQBase.EseqSel[j+2].nType;						
					bCheckData2 = tempESEQBase.EseqSel[j+2].KeyMatl;
					bCheckData2 = tempESEQBase.EseqSel[j+2].KeyStorModel;

					if(!bCheckData2) break;
				}
				
				if(bCheckData2)
				{
					m_ESEQData.arEseq[i-1].EseqSel[j] = m_ESEQData.arEseq[i-1].EseqSel[j+1];

					m_ESEQData.arEseq[i-1].EseqSel[j+1].nType = 0;
					m_ESEQData.arEseq[i-1].EseqSel[j+1].KeyMatl = 0;
					m_ESEQData.arEseq[i-1].EseqSel[j+1].KeyStorModel = 0;
				}
				else
				{
					m_ESEQData.arEseq[i-1].EseqSel[j+1].nType = 0;
					m_ESEQData.arEseq[i-1].EseqSel[j+1].KeyMatl = 0;
					m_ESEQData.arEseq[i-1].EseqSel[j+1].KeyStorModel = 0;
				}

				bCheckData = TRUE;
			}
		}
	}
	
	if(bCheckData)
	{
		AddESEQData(nRow);
		RefreshESEQData(nRow);
	}
}

//Main 쪽의 데이타를 다시 화면에 나타낸다. Refresh
void CCMLoadESEQMainGrid::RefreshESEQData(ROWCOL nRow)
{
	if(!m_ESEQData.arEseq.GetSize()) return;
	
	CString strTemp;
	T_STOR_D StorData;

	RemoveRows(nRow+1, GetRowCount());
	SetRowCount(m_ESEQData.arEseq.GetSize()+1);	
	int i = nRow;
	for(i=nRow; i<m_ESEQData.arEseq.GetSize(); i++)  // 데이타 실제 입력
	{
		InitStoryData(i+1);

		m_pDoc->m_pAttrCtrl->GetStor((m_ESEQData.arEseq.GetAt(i)).KeyStorLoad, StorData);		
		
		strTemp.Format(_T("%d"), i+1);
		SetValueRange(CGXRange(i+1,1), strTemp);
		if((m_ESEQData.arEseq.GetAt(i)).KeyStorLoad == 0)  // Story Key 에 0 값을 가질수 있다.
			SetValueRange(CGXRange(i+1,2), _LS(IDS_WG_CMD__ADDD__None));		
		else
			SetValueRange(CGXRange(i+1,2), StorData.StoryName);		

		// 추가 
		strTemp.Format(_T("%g"), m_ESEQData.arEseq[i].dDuration);
		SetValueRange(CGXRange(i+1,3), StorData.StoryName);		
		strTemp.Format(_T("%g"), m_ESEQData.arEseq[i].dAge);
		SetValueRange(CGXRange(i+1,4), StorData.StoryName);		
	}

	InitStoryData(i+1);    // Last Row의 Story정보 설정.
}

// Main Grid의 Step Column의 Index 다시 매김.
void CCMLoadESEQMainGrid::GridRenumber()
{
	LockUpdate(TRUE);

	CString strTemp;
	for(int i=1; i<GetRowCount(); i++)
	{
		strTemp.Format(_T("%d"), i);
		SetValueRange(CGXRange(i,1), strTemp);
	}

	LockUpdate(FALSE);
	RedrawRowCol(CGXRange(1,1,GetRowCount(),1));
}

BOOL CCMLoadESEQMainGrid::OnDeleteRecord(ROWCOL nRow)
{
	CTBBrowserWnd::OnDeleteRecord(nRow);    

	m_bModified = FALSE;
	// 임시 DB에 있는 삭제할 Row에 대한 저장된 Row의 데이타를 지우고 다른 Row 상세정보를 SubGrid에 보인다.
	if(nRow == m_ESEQData.arEseq.GetSize())  // GetSize()와 같은면 지운후 LastRow에 Focus가 간다.
	{
		m_ESEQData.arEseq.RemoveAt(nRow-1);
		m_pESEQSubGrid->InitESEQData();
	}
	else if(nRow < m_ESEQData.arEseq.GetSize())  // 작은경우 그 위치에 오는 상세정보를 SubGrid에 나타낸다.
	{
		m_ESEQData.arEseq.RemoveAt(nRow-1);
		InitAllStoryData();
		m_pESEQSubGrid->InitESEQData(m_ESEQData.arEseq.GetAt(nRow-1), m_pSpreadGrid->m_aStepNum[nRow-1]);
	}
	else  	// 마지막 데이타를 저장하지 않고 Delete할 경우엔 DB의 데이타에는 아무런 변화가 없다.
	{
	  InitStoryData(GetRowCount());
		m_pESEQSubGrid->InitESEQData();
	}

	GridRenumber();   // Step Column에 나오는 Index번호 다시매김.

	return TRUE;
}

BOOL CCMLoadESEQMainGrid::OnDeleteRecords(CRowColArray &awRows)
{
	m_bModified = FALSE;
	m_bRefreshSubGrid = FALSE;
	LockUpdate(TRUE);

	BOOL bIsMoveCurrentFocus = FALSE;   // CurrentCell이 바뀌는지 검사하기위한 부분.
	BOOL bCheckLastRow = FALSE;         // 마지막 Row에 Focus가 있는지 검사한다.
	ROWCOL ncRow,ncCol;
	GetCurrentCell(ncRow,ncCol);
	if(ncRow == GetRowCount()) bCheckLastRow = TRUE;

	ROWCOL nToRow,nFromRow;

	if(awRows.GetAt(awRows.GetSize()-1) == GetRowCount())
		nToRow = awRows.GetAt(awRows.GetSize()-1)-1;
	else
		nToRow = awRows.GetAt(awRows.GetSize()-1);

	nFromRow = awRows.GetAt(0);

	if((nFromRow <= ncRow) && ( ncRow <= nToRow))  // CurrentCell이 바뀌어야 하는지 검사
		bIsMoveCurrentFocus = TRUE;

	for(int i=nToRow; i >= nFromRow; i--)
	{
		RemoveRows(i,i);
		DeleteESEQData(i);
	}

	GridRenumber();   // Step Column에 나오는 Index번호 다시매김.

	if(bCheckLastRow)
	{
		m_nOldRow = GetRowCount();
		SetCurrentCell(GetRowCount(), ncCol);
		m_pESEQSubGrid->InitESEQData();
	}
	else if(bIsMoveCurrentFocus)  // TRUE - CurrentCell이 지워졌으므로 새로운 CurrentCell을 정한다.
	{
		m_nOldRow = nFromRow;
		SetCurrentCell(nFromRow,2);
//		InitAllStoryData();
//		m_pESEQSubGrid->InitESEQData(m_ESEQData.arEseq.GetAt(nFromRow-1));
		if(nFromRow == GetRowCount())
			m_pESEQSubGrid->InitESEQData();
		else
		{
			InitAllStoryData();
			m_pESEQSubGrid->InitESEQData(m_ESEQData.arEseq.GetAt(nFromRow-1), m_pSpreadGrid->m_aStepNum[nFromRow-1]);
		}
	}

	LockUpdate(FALSE);
	Redraw();

	m_bRefreshSubGrid = TRUE;
	return TRUE;
}

BOOL CCMLoadESEQMainGrid::DeleteESEQData(ROWCOL nRow,BOOL bEndStep)
{
	if(nRow == m_ESEQData.arEseq.GetSize())  // GetSize()와 같은면 지운후 LastRow에 Focus가 간다.
		m_ESEQData.arEseq.RemoveAt(nRow-1);
	else if(nRow < m_ESEQData.arEseq.GetSize())  // 작은경우 그 위치에 오는 상세정보를 SubGrid에 나타낸다.
		m_ESEQData.arEseq.RemoveAt(nRow-1);

	return TRUE;
}

/*
// Insert, Delete, Right, Left, Home, End, Tab, Space, Enter 키를 받을 수 있다.
BOOL CCMLoadESEQMainGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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

	// Insert키가 눌렀을때 다른 기능은 안하고 바로 OnInsertEmptyRecord()를 호출하도록 한다.
	if(bCheckInsert) return TRUE;

	GetCurrentCell(ncRow, ncCol);

	if(ncRow != m_nOldRow)
		OperateDataMain(ncRow, ncCol);  // Mouse Click과 같이 다른 Cell,Row가 선택되었을때 SubGrid와의 동기화

	return TRUE;
}
*/

void CCMLoadESEQMainGrid::OperateDataMain(ROWCOL nRow, ROWCOL nCol)
{
	// Mouse Click, Key Input 을 할경우 Row가 바뀌지 않으면 그대로 Return해버린다.
	if(m_nOldRow == nRow) return;  

	if(m_bModified)  // 이전 Cell이 수정되었을 경우.
	{
//		if(CheckField() && m_pESEQSubGrid->CheckField(FALSE))  //Main,Sub Grid의 데이타 정확함을 검사한다.
		if(CheckField(FALSE) && m_pESEQSubGrid->CheckField(FALSE))   // Model 쪽 데이타가 없어도 된다.
		{
			AddESEQData(m_nOldRow);

			// Main Grid에서 데이타가 있는 Row를 선택했을경우 Sub Grid에 데이타를 보여준다.
			// 아니면 그냥 빈 Row하나만 보여준다.
			if(nRow == GetRowCount())                        
				m_pESEQSubGrid->InitESEQData();
			else
				m_pESEQSubGrid->InitESEQData(m_ESEQData.arEseq.GetAt(nRow-1), m_pSpreadGrid->m_aStepNum[nRow-1]);

			m_nOldRow = nRow;
			m_bModified = FALSE;
		}
		else																							//Main,Sub Grid의 데이타에 이상이 있다. 
		{
			// Main Grid에서 마지막 Row가 Modify 되었어도 None이라는 데이타를 선택했을경우
			// Modify가 안되었다고 생각하고 선택한 Row의 상세정보를 Sub Grid에 보여준다.
			if(GetRowCount() == m_nOldRow)                  
			{
				m_bModified = FALSE;
				m_pESEQSubGrid->InitESEQData(m_ESEQData.arEseq.GetAt(nRow-1), m_pSpreadGrid->m_aStepNum[nRow-1]);
			}
			else
			{
				// Delete Main, Sub 그리드이 내용이 전부 아닐때.
				// 수정을 하는데 초기값과 같이 수정을 했을경우 그 Row를 지워 버린다.
//				if(!CheckField(FALSE) && m_pESEQSubGrid->CheckAllField())
				if(!CheckValidRow() && m_pESEQSubGrid->CheckAllBlankField())
				{
					RemoveRows(m_nOldRow, m_nOldRow);
					GridRenumber();

					// 지워질 Row를 기준으로 선택한 Row의 Index값이 -1만큼 줄어들 수 있다.
					if(nRow > m_nOldRow)   
					{
						SetCurrentCell(nRow-1, nCol);	
						m_nOldRow = nRow-1;

						if(nRow-1 == GetRowCount())
							m_pESEQSubGrid->InitESEQData();
						else
							m_pESEQSubGrid->InitESEQData(m_ESEQData.arEseq.GetAt(nRow-2), m_pSpreadGrid->m_aStepNum[nRow-2]);					
					}
					else  // 선택된 Row의 Index 값이 변화가 없어 DB에서 데이타를 읽어 SubGrid에 상세정보를 보인다.
					{
						m_nOldRow = nRow;
						m_pESEQSubGrid->InitESEQData(m_ESEQData.arEseq.GetAt(nRow-1), m_pSpreadGrid->m_aStepNum[nRow-1]);
					}  
				}
				else           // Valid한 데이타가 남아 있다면 계속 Edit할 수 있게 이전의 Row를 다시 선택한다.
				{
					SetCurrentCell(m_nOldRow, nCol);
				}
			}   
		}
	}
	else   // m_bModified = FALSE 일때  // 이전Cell이 수정되지 않았을때.
	{
//		if((!CheckField(FALSE)) && (m_pESEQSubGrid->CheckAllField()) && (m_nOldRow != GetRowCount()))
		if((!CheckValidRow()) && (m_pESEQSubGrid->CheckAllBlankField()) && (m_nOldRow != GetRowCount()))
		{
			RemoveRows(m_nOldRow,m_nOldRow);
			m_bModified = FALSE;

			if(m_nOldRow == m_ESEQData.arEseq.GetSize())  // GetSize()와 같은면 지운후 LastRow에 Focus가 간다.
			{
				m_ESEQData.arEseq.RemoveAt(m_nOldRow-1);
				m_pESEQSubGrid->InitESEQData();
			}
			else if(m_nOldRow < m_ESEQData.arEseq.GetSize())  // 작은경우 그 위치에 오는 상세정보를 SubGrid에 나타낸다.
			{
				m_ESEQData.arEseq.RemoveAt(m_nOldRow-1);
				InitAllStoryData();
				m_pESEQSubGrid->InitESEQData(m_ESEQData.arEseq.GetAt(m_nOldRow-1), m_pSpreadGrid->m_aStepNum[nRow-1]);
			}
			else  	// 마지막 데이타를 저장하지 않고 Delete할 경우엔 DB의 데이타에는 아무런 변화가 없다.
			{
				InitStoryData(GetRowCount());
				m_pESEQSubGrid->InitESEQData();
			}

			GridRenumber();   // Step Column에 나오는 Index번호 다시매김.
		}
		
		if(nRow == GetRowCount())
			m_pESEQSubGrid->InitESEQData();
		else
			m_pESEQSubGrid->InitESEQData(m_ESEQData.arEseq.GetAt(nRow-1), m_pSpreadGrid->m_aStepNum[nRow-1]);

		m_nOldRow = nRow;
	}
}

const CString& CCMLoadESEQMainGrid::GetValueRowCol(ROWCOL nRow, ROWCOL nCol)
{
	static CString value;
	value = _T("");
	CGXControl *pControl = GetControl(nRow, nCol);

	if (!pControl || !pControl->IsActive() || !pControl->GetModify())
		return CGXGridWnd::GetValueRowCol(nRow, nCol);

	pControl->GetCurrentText(value);  
	return value;
}

BOOL CCMLoadESEQMainGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	// Row가 입력되어 값을 저장할 수 있도록 CArray에 초기값을 가진 데이타를 Add/Insert 한다.
//	if((CheckField()) && (m_pESEQSubGrid->CheckField(FALSE))) 
	if(CheckField() || m_pESEQSubGrid->CheckField(FALSE))  // Model 쪽 데이타가 없어도 OK
	{
		m_ESEQBase.Initialize();             
		m_ESEQData.arEseq.InsertAt(nRow-1, m_ESEQBase);

		if(m_bModified)                      // 이전에 작업중이던 Row 데이타를 저장한다.
			AddESEQData(nRow+1);
		
		m_pESEQSubGrid->InitESEQData();
		
		InitStoryData(nRow);
		SetCurrentCell(nRow, 2);             // MainGrid 데이타 초기화

		GridRenumber();

//		m_bModified = TRUE;                 // Insert 된 열에 데이타가 정상적으로 입력되어야 한다.
		m_bModified = FALSE;									// Insert 된 열에 아무 데이타가 없을때 자동으로 지운다.
		CTBBrowserWnd::m_nEditMode = noMode;
		--m_nOldRow;
		return TRUE;
	}

	//작업중인 Row의 데이타가 정확하지 않을때 Row를 Insert하지 않는다.
	LockUpdate(TRUE);
	RemoveRows(nRow, nRow);
	SetCurrentCell(nRow,2);
	LockUpdate(FALSE);
	Redraw();
	CTBBrowserWnd::m_nEditMode = noMode;
	--m_nOldRow;
	return TRUE;
}

BOOL CCMLoadESEQMainGrid::DoLButtonDblClk(UINT flags, CPoint point)
{
	return FALSE;
}

void CCMLoadESEQMainGrid::OnInitCurrentCell(ROWCOL nRow, ROWCOL nCol)
{
	if(nCol <= 1 || nRow == 0) return;
	if(m_nOldRow == nRow) return;  

	m_strOldStoryName = GetValueRowCol(nRow, 2);

	if(m_bRefreshSubGrid)
		OperateDataMain(nRow, nCol);

	CGXGridCore::OnInitCurrentCell(nRow, nCol);
}




BOOL CCMLoadESEQMainGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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
