// CMGridMainGirderItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridMainGirderItemDlg.h"
#include "CMGridMainGirderListDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_strParser.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h" 
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h" 
#include "..\wg_db\wg_db_StagCtrl.h" 

#include "..\wg_base\wg_base_NumericOptimizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_NUM_COL     4
/////////////////////////////////////////////////////////////////////////////
// CCMGXMainGirderGridWnd grid
CCMGXMainGirderGridWnd::CCMGXMainGirderGridWnd()
	: CCMGXGridWnd2()
{
	m_bStartModify = FALSE;
}

BOOL CCMGXMainGirderGridWnd::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	CCMGXGridWnd2::OnStartEditing(nRow, nCol);

	m_strOld = GetValueRowCol(nRow, nCol);
	m_bStartModify = TRUE;
	return TRUE;
}

BOOL CCMGXMainGirderGridWnd::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	CCMGXGridWnd2::OnEndEditing(nRow, nCol);

	if (m_bStartModify)
	{
		CCMGridMainGirderItemDlg* pParent = (CCMGridMainGirderItemDlg*)GetParent();
		if (!pParent->GridCellChanged(nRow, nCol))
		{
			m_bStartModify = FALSE;
			BOOL bOldLock = LockUpdate(TRUE);
			SetValueRange(CGXRange(nRow, 1), m_strOld); 
			LockUpdate(bOldLock);
			if (!bOldLock) Redraw();
			return TRUE;
		}
		m_bStartModify = FALSE;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCMGridMainGirderItemDlg dialog
CCMGridMainGirderItemDlg::CCMGridMainGirderItemDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMGridMainGirderItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGridMainGirderItemDlg)
	m_nSelectionMethod = 0;
	//}}AFX_DATA_INIT
	m_aCtrlSelect.Add(IDC_CMD_2POINTS);
	m_aCtrlSelect.Add(IDC_CMD_PICKING);
	m_aCtrlSelect.Add(IDC_CMD_ELEMSELECT);

	m_bModify = FALSE;
}


void CCMGridMainGirderItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridMainGirderItemDlg)
	DDX_Control(pDX, IDC_CMD_2POINT_UNIT2, m_2PointUnit2);
	DDX_Control(pDX, IDC_CMD_2POINT_UNIT1, m_2PointUnit1);
	DDX_Control(pDX, IDC_CMD_LANE_NAME_EDIT, m_wndLaneName);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT1, m_wnd2PointsEdit1);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT2, m_wnd2PointsEdit2);
	DDX_Control(pDX, IDC_CMD_PICKING_EDIT, m_wndPickingEdit);
	DDX_Control(pDX, IDC_CMD_ELEMSELECT_EDIT, m_wndNodeSelectEdit);
	DDX_Radio(pDX, IDC_CMD_2POINTS, m_nSelectionMethod);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridMainGirderItemDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMGridMainGirderItemDlg)
	ON_BN_CLICKED(IDC_CMD_OP_ADD, OnCmdOpAdd)
	ON_BN_CLICKED(IDC_CMD_OP_INSERT, OnCmdOpInsert)
	ON_BN_CLICKED(IDC_CMD_OP_DELETE, OnCmdOpDelete)
	ON_BN_CLICKED(IDC_CMD_2POINTS, OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_PICKING, OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_ELEMSELECT, OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_BTN_OK, OnCmdBtnOk)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnCmdBtnCancel)
	ON_BN_CLICKED(IDC_CMD_BTN_APPLY, OnCmdBtnApply)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGridMainGirderItemDlg message handlers

void CCMGridMainGirderItemDlg::OnCmdOpAdd()
{
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nCheck);
	InsertBaseItem(-1, nCheck);
}

void CCMGridMainGirderItemDlg::OnCmdOpInsert()
{
	ROWCOL nRow, nCol;
	if (m_wndGrid.GetCurrentCell(nRow, nCol))
	{
		nRow = nRow-1;
	}
	else nRow = -1;
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nCheck);
	InsertBaseItem(nRow, nCheck);
}

void CCMGridMainGirderItemDlg::SetModify(BOOL bModify)
{
	m_bModify = bModify;
}

void CCMGridMainGirderItemDlg::OnCmdOpDelete()
{
	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	CRowColArray awRows;
	m_wndGrid.GetSelectedRows(awRows, TRUE, TRUE);
	if (awRows.GetSize() < 1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_item_to_delete_));
		return;
	}
	ROWCOL nRow;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	for (int i = awRows.GetSize()-1; i >= 0; i--)
	{
		nRow = awRows.GetAt(i);
		if (nRow == 0) continue;
		m_wndGrid.RemoveRows(nRow, nRow);
	}

	int nRowCount = m_wndGrid.GetRowCount();
	if (bCurrent && nRowCount > 0)
	{
		if (nRowCount < ncRow) ncRow = nRowCount;
		m_wndGrid.SetCurrentCell(ncRow, ncCol);
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();

	SelectNode();  // added by KYM
}

void CCMGridMainGirderItemDlg::OnCmdChangeSelectBy()
{
	UpdateData(TRUE);
	SelectMethodMan();
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);

	if (m_nSelectionMethod == 0) m_wnd2PointsEdit1.SetFocus();
	else if(m_nSelectionMethod == 1) m_wndPickingEdit.SetFocus();
	else m_wndNodeSelectEdit.SetFocus();
}

void CCMGridMainGirderItemDlg::OnCmdBtnOk()
{
	if (!OnExecute())
		return;
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	DestroyWindow();
}

void CCMGridMainGirderItemDlg::OnCmdBtnCancel()
{
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	DestroyWindow();
}

void CCMGridMainGirderItemDlg::OnCmdBtnApply()
{
	if (!OnExecute())
		return;
	InitDlg();
}

BOOL CCMGridMainGirderItemDlg::OnExecute()
{
	BOOL CheckMdgr=FALSE;
	CString strTemp;
	m_wndLaneName.GetWindowText(strTemp);

	if (strTemp == _T(""))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD_MAIN_GIRDER_GIRDER_NAME_MISS));
		return FALSE;
	}

	if (m_wndGrid.GetRowCount() == 0)
	{
		CString str;
		str.Format(_LS(IDS_WG_CMD_MAIN_GIRDER_DATA_NUMBER_ZERO), strTemp);
		GSaveHistoryFormatNF(str);
		return FALSE;
	}

	if (!Dlg2Data()) return FALSE;
	if (m_bModify) CheckMdgr = m_pDoc->m_pDataCtrl->ModifyMgdr(m_csOldLaneName, m_Data);
	else CheckMdgr = m_pDoc->m_pDataCtrl->AddMgdr(m_Data);
	m_bModify = FALSE;  //Modify후에는 Add모드로..
	return TRUE;
}

void CCMGridMainGirderItemDlg::InitDlg()
{
	//m_wndLaneName.SetWindowText(_T(""));
	m_wndLaneName.SetWindowText(CreateGirderName());
	m_wndGrid.SetRowCount(0);
	m_wndLaneName.SetFocus();
	
	CMouseEdit::DisConnectAssocWindowST();
	
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	InitItemDataVar();
	Invalidate();
}

BOOL CCMGridMainGirderItemDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	
	m_2PointUnit1.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_2PointUnit2.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	
	m_wnd2PointsEdit1.SetAttUcsPos();
	m_wnd2PointsEdit1.SetModeToUse(MOUSEEDIT_USE_SETP2_FIRST);
	m_wnd2PointsEdit2.SetAttUcsPos();
	m_wnd2PointsEdit2.SetModeToUse(MOUSEEDIT_USE_SETP2_SECOND);
	m_wnd2PointsEdit2.SetLButtonDownNotifyWindow(this);
	m_wnd2PointsEdit2.SetEnterNotifyWindow(this);
	m_wnd2PointsEdit1.SetNextLink(&m_wnd2PointsEdit2);
	m_wnd2PointsEdit2.SetNextLink(&m_wnd2PointsEdit1);

	m_wndPickingEdit.SetAttNodeList();
	m_wndPickingEdit.SetLButtonDownNotifyWindow(this);
	m_wndPickingEdit.SetEnterNotifyWindow(this);
	m_wndPickingEdit.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndPickingEdit.SetMaxNodeKeyNum(1);	

	//Data2Dlg();
	InitGrid();
	InitItemDataVar();
	ButtonCtrl();

	if (!m_bModify)
		m_wndLaneName.SetWindowText(CreateGirderName());

	m_wndLaneName.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGridMainGirderItemDlg::SetInitModify(T_MGDR_K key)
{
	m_pDoc->m_pAttrCtrl->GetMgdr(key, m_Data);
	m_csOldLaneName = m_Data.Name;

	Data2Dlg();
	m_wndLaneName.SetFocus();
}

void CCMGridMainGirderItemDlg::InitGrid()
{
	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);

	CGXGridParam* pParam = m_wndGrid.GetParam();
	ASSERT_VALID(pParam);

	CGXProperties* pProp = pParam->GetProperties();
	ASSERT_VALID(pProp);

	pParam->EnableUndo(FALSE);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	// Row and column headers
	m_wndGrid.ColHeaderStyle().SetReadOnly(FALSE).SetEnabled(FALSE)
														.SetFont(CGXFont_GC().SetBold(FALSE));
	m_wndGrid.RowHeaderStyle().SetEnabled(FALSE);
	m_wndGrid.ChangeStandardStyle(CGXStyle().SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetAllowEnter(FALSE));
	
	// Special grid settings
	pParam->EnableMoveRows(TRUE);                  // rows cannot be dragged
	pParam->EnableMoveCols(FALSE);                 // columns can be dragged
	pParam->EnableTrackColWidth(FALSE);             // disable resizing
	pParam->EnableTrackRowHeight(FALSE);
	pParam->EnableThumbTrack(FALSE);               // thumb-tracking is disabled
	pParam->SetNumberedColHeaders(FALSE);
	// Excel과 같은 셀 선택 : DoLButtonDblClk 참조
	pParam->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	pParam->SetHideCurrentCell(GX_HIDE_NEVER);

	// removing rows is not undoable for this kind of grid
	pParam->SetRemoveColsFlags(FALSE);
	pParam->SetRemoveRowsFlags(FALSE);

	pProp->SetMarkColHeader(FALSE);    // Turn off pressed button effect for column headers
	pProp->SetPrintRowHeaders(FALSE);  // Don't print column headers

	m_wndGrid.SetRowHeightByDPI(0, 0, 32);
	m_wndGrid.SetDrawingTechnique(gxDrawUsingMemDC);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_wndGrid.SetColCount(D_NUM_COL);
	SetGridTitle();
	m_wndGrid.SetColWidthByDPI(0, 0, 27);
	m_wndGrid.SetColWidthByDPI(1, 1, 48);
	m_wndGrid.SetColWidthByDPI(2, 2, 48);
	m_wndGrid.SetColWidthByDPI(3, 3, 48);
	m_wndGrid.SetColWidthByDPI(4, 4, 48);

	m_wndGrid.SetStyleRange(CGXRange().SetCols(2, 4), CGXStyle()
			.SetEnabled(FALSE));

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMGridMainGirderItemDlg::SetGridTitle()
{
	CString csUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_WG_CMD__ADDD__No));
	m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_CMD0417__Node));
	m_wndGrid.SetValueRange(CGXRange(0, 2), CString(_LS(IDS_CMD_DIRX))+csUnit+_T(")"));
	m_wndGrid.SetValueRange(CGXRange(0, 3), CString(_LS(IDS_CMD_DIRY))+csUnit+_T(")"));
	m_wndGrid.SetValueRange(CGXRange(0, 4), CString(_LS(IDS_CMD_DIRZ))+csUnit+_T(")"));
	/*
	m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_WG_CMD__ADDD__No));
	m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_WG_CMD__ADDD__Elem));
	m_wndGrid.SetValueRange(CGXRange(0, 2), CString(_LS(IDS_WG_CMD__ADDD__Eccen__n_))+csUnit+_T(")"));
	m_wndGrid.SetValueRange(CGXRange(0, 3), _LS(IDS_WG_CMD__ADDD__Impact_nFactor));
	*/
	
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMGridMainGirderItemDlg::InitItemDataVar()
{
	m_wnd2PointsEdit1.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wnd2PointsEdit2.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wndPickingEdit.SetWindowText(_T(""));
}

void CCMGridMainGirderItemDlg::SelectMethodMan()
{
	CArray<UINT,UINT> PointArr;
	CArray<UINT,UINT> PickingArr;
	CArray<UINT,UINT> ElemSelArr;
	
	PointArr.Add(IDC_CMD_2POINTS_EDIT1    );
	PointArr.Add(IDC_CMD_2POINTS_EDIT2    );
	PointArr.Add(IDC_CMD_2POINT_UNIT1     );
	PointArr.Add(IDC_CMD_2POINT_UNIT2     );
	PickingArr.Add(IDC_CMD_PICKING_EDIT   );
	ElemSelArr.Add(IDC_CMD_ELEMSELECT_EDIT);


	if(m_nSelectionMethod == 0)
	{
		CDlgUtil::CtrlShowHide(this,PointArr  ,TRUE);
		CDlgUtil::CtrlShowHide(this,PickingArr,FALSE);
		CDlgUtil::CtrlShowHide(this,ElemSelArr,FALSE);
	}
	else if( m_nSelectionMethod == 1)
	{
		CRect DRect,SRect;
		
		m_wnd2PointsEdit1.GetWindowRect(&DRect);
		m_wndPickingEdit.GetWindowRect(&SRect);

		int Dx = DRect.left - SRect.left;

		CDlgUtil::CtrlShowHide(this,PointArr  ,FALSE);
		CDlgUtil::CtrlMoveDistX(this,PickingArr,Dx);
		CDlgUtil::CtrlShowHide(this,PickingArr,TRUE);
		CDlgUtil::CtrlShowHide(this,ElemSelArr,FALSE); 
	}
	else
	{
		CRect DRect, SRect;
		m_wnd2PointsEdit1 .GetWindowRect(&DRect);
		m_wndNodeSelectEdit.GetWindowRect(&SRect);

		int Dx = DRect.left - SRect.left;

		CDlgUtil::CtrlShowHide(this,PointArr  ,FALSE);
		CDlgUtil::CtrlShowHide(this,PickingArr,FALSE);
		CDlgUtil::CtrlMoveDistX(this,ElemSelArr,Dx);
		CDlgUtil::CtrlShowHide(this,ElemSelArr,TRUE); 
	}
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
}

void CCMGridMainGirderItemDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_BEFORE:
	case D_UPDATE_BUFFER_AFTER:
		break;
	case D_UPDATE_UNIT:
		UpdateUnit();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		//DestroyWindow();
		ButtonCtrl();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMGridMainGirderItemDlg::UpdateUnit()
{
	SetGridTitle();
	
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	int nUnitTypeOffset = D_UNITSYS_BASE_LENGTH;    
	double dCoord;
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		for (int k=0; k<3; k++)
		{
			dCoord = _tstof(m_wndGrid.GetValueRowCol(i+1, k+2));
			dCoord = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitTypeOffset, dCoord);
			m_wndGrid.SetValueRange(CGXRange(i+1, k+2), dCoord);
		}
	}
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMGridMainGirderItemDlg::ButtonCtrl()
{
	BOOL bEnable = FALSE;
	if (m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsBaseStag())
		bEnable = TRUE;
	
	GetDlgItem(IDC_CMD_BTN_OK)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_BTN_APPLY)->EnableWindow(bEnable);
}

void CCMGridMainGirderItemDlg::Data2Dlg()
{
	m_wndLaneName.SetWindowText(m_Data.Name);
	InitItemDataVar();
		
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	int nSize = m_Data.aData.GetSize();
	m_wndGrid.SetRowCount(nSize);
	
	T_NODE_K NodeK;
	T_NODE_D NodeD;
	for (int i = 0; i < nSize; i++)
	{
		NodeK = m_Data.aData[i].KeyNode;
		m_pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD);
		m_wndGrid.SetValueRange(CGXRange(i+1, 1), (long)NodeK);
		m_wndGrid.SetValueRange(CGXRange(i+1, 2), (double)NodeD.x);
		m_wndGrid.SetValueRange(CGXRange(i+1, 3), (double)NodeD.y);
		m_wndGrid.SetValueRange(CGXRange(i+1, 4), (double)NodeD.z);
	}
	if (nSize > 0) m_wndGrid.SetCurrentCell(1, 1);
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();

	UpdateData(FALSE);
	SelectNode();			// added by KYM
}

BOOL CCMGridMainGirderItemDlg::Dlg2Data()
{
	T_MGDR_BASE item;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	UpdateData(TRUE);
	m_wndLaneName.GetWindowText(m_Data.Name);

	int nCount = m_wndGrid.GetRowCount();
	m_Data.aData.SetSize(nCount);
	for (int i=0; i<nCount; i++)
	{
		item.KeyNode = _ttol(m_wndGrid.GetValueRowCol(i+1, 1));
		m_Data.aData.SetAt(i, item);
	}
	return TRUE;
}

void CCMGridMainGirderItemDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	BOOL bOldLock = m_wndGrid.LockUpdate(FALSE);
	if (bOldLock) m_wndGrid.Redraw();
	m_wndGrid.LockUpdate(bOldLock);
	// Do not call CCMDlgBase::OnPaint() for painting messages
}

void CCMGridMainGirderItemDlg::InsertBaseItem(int nPos, int nType)
{
	T_MGDR_BASE item;

	if (nType == 0)  // Select By 2 Points
	{
		CArray<T_NODE_K, T_NODE_K> aNodeKey;  
		double ux, uy, uz;
		double WX1, WY1, WZ1;
		double WX2, WY2, WZ2;

		if (!CFormulaEdit::GetPosition(&m_wnd2PointsEdit1, ux, uy, uz))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___1st_position_is_invalid_));
			return;
		}
		m_wnd2PointsEdit1.GetWcsPos(ux, uy, uz, WX1, WY1, WZ1);
		if (!CFormulaEdit::GetPosition(&m_wnd2PointsEdit2, ux, uy, uz))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___2nd_position_is_invalid_));
			return;
		}
		m_wnd2PointsEdit2.GetWcsPos(ux, uy, uz, WX2, WY2, WZ2);

		// Get Node Key List
		// 2000.6.5 Add by scchoi
		double dbTolerance = m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();
		m_pDoc->m_pDataCtrl->GetNodeBy2Point(WX1, WY1, WZ1, WX2, WY2, WZ2, dbTolerance, aNodeKey, TRUE);
		if (aNodeKey.GetSize() == 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_Node_on_line_));
			return;
		}  

		int nNodeKey;
		int nCount = aNodeKey.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			nNodeKey = aNodeKey[i];
			if (Grid_IsExistKey(nNodeKey))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_already_ex), nNodeKey);
				AfxMessageBox(msg);
				return;
			}
			item.KeyNode = (unsigned int)nNodeKey;  
			Grid_InsertItem(nPos, item);
			if (nPos >= 0) nPos++;
		}
		SelectNode();
		return;
	}
	else if(nType == 1) // Select By Picking
	{
		int nNodeKey;
		CFormulaEdit::GetEditValue(&m_wndPickingEdit, nNodeKey);
		/**
		CString strValue;
		CString strValue2;
		m_wndPickingEdit.GetWindowText(strValue);
		int nIndex = strValue.ReverseFind(',');
		if (nIndex != -1)
		{
			strValue2 = strValue.Right(nIndex);
			strValue = strValue2;
		}
		CFormulaEdit::GetFormulaValue(strValue, nNodeKey);
		**/

		if (nNodeKey <= 0)
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_not_exist_), nNodeKey);
			AfxMessageBox(msg);
			return;
		}
		if (Grid_IsExistKey(nNodeKey))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_already_ex), nNodeKey);
			AfxMessageBox(msg);
			return;
		}
		T_NODE_D Data;
		m_pDoc->m_pAttrCtrl->GetNode(nNodeKey, Data);
		item.KeyNode = (unsigned int)nNodeKey;  
		Grid_InsertItem(nPos, item);
	}
	else if(nType == 2) // Select By Select Node
	{
		CArray<T_NODE_K, T_NODE_K> arNodeKeyList;
		CArray<long, long> arNodeList;

		CString strNodeNumber;
	  m_wndNodeSelectEdit.GetWindowText(strNodeNumber);

		m_pDoc->m_pAttrCtrl->GetNodeKeyList(arNodeKeyList);

		CNumericOptimizer optimizer;
		optimizer.SelectNumByStrBase((long*)arNodeKeyList.GetData(), (long)arNodeKeyList.GetSize(),
			                            strNodeNumber, arNodeList);
		if (arNodeList.GetSize() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_node_));
			return;
		}

		for(int i=0; i<arNodeList.GetSize(); i++)
		{
			if (Grid_IsExistKey((UINT)arNodeList[i]))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No__d__is_already_ex), (UINT)arNodeList[i]);
				AfxMessageBox(msg);
				return;
			} 
		}
		T_NODE_D Data;
		int nInsertPos = nPos;
		for(int i=0; i<arNodeList.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetNode((UINT)arNodeList[i], Data);
			item.KeyNode = (UINT)arNodeList[i];
			Grid_InsertItem(nPos, item);
			if(nInsertPos >= 0) nPos++;
		}
	}

	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	if (!bCurrent) ncCol = 1;

	if (nPos == -1) ncRow = m_wndGrid.GetRowCount();
	else ncRow = nPos+1;

	m_wndGrid.SetCurrentCell(ncRow, ncCol);

	SelectNode();
}

void CCMGridMainGirderItemDlg::SelectNode() 
{
	int nCount = m_wndGrid.GetRowCount();
	if(nCount >= 0)
	{
	  CArray<T_NODE_K, T_NODE_K>arSelNode;
	  for (int i = 0; i < nCount; i++) arSelNode.Add(_ttol(m_wndGrid.GetValueRowCol(i+1, 1)));
	  m_pDoc->m_pViewCtrl->SelectNode(NULL, arSelNode, TRUE, TRUE);
	}
}

BOOL CCMGridMainGirderItemDlg::Grid_IsExistKey(int nKey)
{
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		if (nKey == _ttol(m_wndGrid.GetValueRowCol(i+1, 1))) return TRUE;
	}
	return FALSE;
}

void CCMGridMainGirderItemDlg::Grid_InsertItem(int nPos, T_MGDR_BASE &item)
{
	ROWCOL nRow;
	if (nPos < 0 || nPos > m_wndGrid.GetRowCount()) nRow = m_wndGrid.GetRowCount();
	else nRow = nPos;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	T_NODE_D node;
	m_pDoc->m_pAttrCtrl->GetNode(item.KeyNode, node);
	m_wndGrid.InsertRows(nRow+1, 1);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 1), (long)item.KeyNode); 
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 2), (double)node.x); 
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 3), (double)node.y); 
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 4), (double)node.z);

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

LRESULT CCMGridMainGirderItemDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if(m_nSelectionMethod == 0)
	{
		InsertBaseItem(-1, 0);
	}
	else if(m_nSelectionMethod == 1)
	{
		InsertBaseItem(-1, 1);
		//m_wnd2PointsEdit1.SetFocus();
	}
	return 0L;
}


BOOL CCMGridMainGirderItemDlg::DestroyWindow() 
{
	//((CCMGridMainGirderListDlg*)GetParent())->m_pWndChild = NULL;
	return CCMDlgBase::DestroyWindow();
}

BOOL CCMGridMainGirderItemDlg::GridCellChanged(ROWCOL nRow, ROWCOL nCol)
{
	CString str;
	str = m_wndGrid.GetValueRowCol(nRow, nCol);

	CString str2;
	for (int i=1; i<=m_wndGrid.GetRowCount(); i++)
	{
		if (i == nRow)
			continue;
		str2 = m_wndGrid.GetValueRowCol(i, nCol);

		if (str.Compare(str2) == 0)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_already_ex), _ttoi(str));
			return FALSE;
		}
	}

	T_NODE_K key;
	T_NODE_D node;
	key = (UINT)_ttoi(str);
	if (!m_pDoc->m_pAttrCtrl->GetNode(key, node))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD0417__Node_number__s__is_not_valid_), key);
		return FALSE;
	}

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	
	m_wndGrid.SetValueRange(CGXRange(nRow, 2), (double)node.x); 
	m_wndGrid.SetValueRange(CGXRange(nRow, 3), (double)node.y); 
	m_wndGrid.SetValueRange(CGXRange(nRow, 4), (double)node.z);

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
	return TRUE;
}

BOOL CCMGridMainGirderItemDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pWnd = GetFocus();
		if (pWnd && pWnd->GetSafeHwnd() && IsWindow(pWnd->GetSafeHwnd()) &&
			pMsg->wParam == VK_DELETE && pWnd->GetDlgCtrlID() == IDC_CMD_PLACEHOLDER)
		{
			OnCmdOpDelete();
			return TRUE;
		}
	}	
	return CCMDlgBase::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////////////////////////
// 외부에서 Data를 Setting 하는 함수  
// 단, Dialog(Modaless)는 이미 Create&Activartion 되어있는 것을 전제로 한다.
// 이 함수를 Call 해주면 입력모드(List Mode가 아님) 상태에서 
// 인자로 넘어온 Key, Data를 Setting 하여 보여준다.
// by TAE
BOOL CCMGridMainGirderItemDlg::ExternalSetting(T_MGDR_K MgdrKey)
{  
	BOOL bFlag = TRUE;
	bFlag = ::IsWindow(this->GetSafeHwnd());                // 윈도우가 생성되지 않았으면
	bFlag = m_pDoc->m_pAttrCtrl->ExistMgdr(MgdrKey);        // 키값이 존재하지 않으면
	bFlag = m_pDoc->m_pAttrCtrl->GetMgdr(MgdrKey, m_Data);  // Data를 가져올 수 없으면
	if(!bFlag) return bFlag;

	m_csOldLaneName = m_Data.Name;
	m_bModify = TRUE;
	Data2Dlg();

	return TRUE;
}

CString CCMGridMainGirderItemDlg::CreateGirderName()
{
	CString strName;
	int nCount = m_pDoc->m_pAttrCtrl->GetCountMgdr() + 1;
	strName.Format(_T("G%d"), nCount);

	while(m_pDoc->m_pAttrCtrl->ExistMgdr(strName))
	{
		nCount++;
		strName.Format(_T("G%d"), nCount);
	}
	return strName;
}
