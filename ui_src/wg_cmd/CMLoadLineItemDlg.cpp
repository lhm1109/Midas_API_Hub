// CMLoadLineItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadLineItemDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h" 
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h" 


#include "CMLoadLineListDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_NUM_COL     3
#define D_ZERO        10e-3
/////////////////////////////////////////////////////////////////////////////
// CCMLoadLineItemDlg dialog


CCMLoadLineItemDlg::CCMLoadLineItemDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMLoadLineItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadLineItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
}


void CCMLoadLineItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadLineItemDlg)
	DDX_Control(pDX, IDC_CMD_LANE_NAME_EDIT, m_wndName);
	DDX_Control(pDX, IDC_CMD_2POINT_UNIT1, m_wndPointUnit);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT1, m_wndPointEdit);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMLoadLineItemDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMLoadLineItemDlg)
	ON_BN_CLICKED(IDC_CMD_OP_ADD, OnCmdOpAdd)
	ON_BN_CLICKED(IDC_CMD_OP_INSERT, OnCmdOpInsert)
	ON_BN_CLICKED(IDC_CMD_OP_DELETE, OnCmdOpDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_OK, OnCmdBtnOk)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnCmdBtnCancel)
	ON_BN_CLICKED(IDC_CMD_BTN_APPLY, OnCmdBtnApply)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadLineItemDlg message handlers

BOOL CCMLoadLineItemDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitGrid();
	
	m_wndPointUnit.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	InitItemDataVar();

	m_wndPointEdit.SetAttUcsPos();
	m_wndPointEdit.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndPointEdit.SetLButtonDownNotifyWindow(this);
	m_wndPointEdit.SetEnterNotifyWindow(this);
	m_wndPointEdit.SetNextLink(&m_wndPointEdit);

	if (!m_bModify)
		m_wndName.SetWindowText(CreateGirderName());

	m_wndName.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadLineItemDlg::InitGrid()
{
	m_wndGrid.Initialize(&m_Data.aData);

	//m_wndGrid.MakeItemEx();

	/*
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

	m_wndGrid.SetRowHeight(0, 0, 32);
	m_wndGrid.SetDrawingTechnique(gxDrawUsingMemDC);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_wndGrid.SetColCount(D_NUM_COL);
	SetGridTitle();
	m_wndGrid.SetColWidth(0, 0, 40);
	m_wndGrid.SetColWidth(1, 1, 57);
	m_wndGrid.SetColWidth(2, 2, 57);
	m_wndGrid.SetColWidth(3, 3, 57);

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
	*/
}

/*
void CCMLoadLineItemDlg::SetGridTitle()
{
	CString csUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_WG_CMD__ADDD__No));
	m_wndGrid.SetValueRange(CGXRange(0, 1), CString(_LS(IDS_CMD_DIRX))+csUnit+_T(")"));
	m_wndGrid.SetValueRange(CGXRange(0, 2), CString(_LS(IDS_CMD_DIRY))+csUnit+_T(")"));
	m_wndGrid.SetValueRange(CGXRange(0, 3), CString(_LS(IDS_CMD_DIRZ))+csUnit+_T(")"));
	
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}
*/

void CCMLoadLineItemDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
	case D_UPDATE_EXE_START: 
	case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		DestroyWindow();
		break;
	default:
		break;
	}
}

void CCMLoadLineItemDlg::UpdateUnit()
{
	T_UNIT_SYSTEM us;
	m_wndGrid.SetHeaderTitle(us, FALSE);
	m_wndGrid.MakeItemEx();

	/*
	SetGridTitle();

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	int nUnitTypeOffset = D_UNITSYS_BASE_LENGTH;    
	double dCoord;
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		for (int k=0; k<3; k++)
		{
			dCoord = _tstof(m_wndGrid.GetValueRowCol(i+1, k+1));
			dCoord = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitTypeOffset, dCoord);
			m_wndGrid.SetValueRange(CGXRange(i+1, k+1), dCoord);
		}
	}
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
	*/
}

void CCMLoadLineItemDlg::SetModify(BOOL bModify)
{
	m_bModify = bModify;
}

void CCMLoadLineItemDlg::OnCmdOpAdd()
{
	InsertBaseItem(-1);
}

void CCMLoadLineItemDlg::OnCmdOpInsert()
{
	ROWCOL nRow, nCol;
	if (!m_wndGrid.GetCurrentCell(nRow, nCol)) 
		nRow = -1;
	InsertBaseItem(nRow);
}

void CCMLoadLineItemDlg::OnCmdOpDelete()
{
	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	CRowColArray awRows;
	m_wndGrid.GetSelectedRows(awRows, TRUE, TRUE);
	int nCount = awRows.GetSize();
	if (nCount < 1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_item_to_delete_));
		return;
	}

	CArray<int, int> aRow;
	aRow.SetSize(nCount);
	for (int i=0; i<nCount; i++)
		aRow[i] = awRows[i];
	if (!m_wndGrid.DelData(aRow, FALSE)) ASSERT(0);

	/*
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
	*/
}

void CCMLoadLineItemDlg::OnCmdBtnOk()
{
	if (!Execute())
		return;
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	//InitDlg();
	//ShowWindow(SW_HIDE);
	DestroyWindow();
}

void CCMLoadLineItemDlg::OnCmdBtnCancel()
{
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	//InitDlg();
	//ShowWindow(SW_HIDE);
	DestroyWindow();
}

void CCMLoadLineItemDlg::OnCmdBtnApply()
{
	if (!Execute())
		return;
	InitDlg();
}

BOOL CCMLoadLineItemDlg::Execute()
{
	BOOL CheckMdgr=FALSE;
	CString strTemp;
	m_wndName.GetWindowText(strTemp);

	if (strTemp == _T(""))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD_LOAD_LINE_NAME_MISS));
		return FALSE;
	}

	if (m_wndGrid.GetRowCount() == 0)
	{
		CString str;
		str.Format(_LS(IDS_WG_CMD_LOAD_LINE_DATA_NUMBER_ZERO), strTemp);
		GSaveHistoryFormatNF(str);
		return FALSE;
	}

	if (!Dlg2Data()) return FALSE;
	if (m_bModify) CheckMdgr = m_pDoc->m_pDataCtrl->ModifyLdln(m_csOldName, m_Data);
	else CheckMdgr = m_pDoc->m_pDataCtrl->AddLdln(m_Data);
	m_bModify = FALSE;

	return TRUE;
}

void CCMLoadLineItemDlg::SetInitModify(T_LDLN_K key)
{
	m_pDoc->m_pAttrCtrl->GetLdln(key, m_Data);
	m_csOldName = m_Data.Name;

	Data2Dlg();
	m_wndName.SetFocus();
}

void CCMLoadLineItemDlg::InitItemDataVar()
{
	m_wndPointEdit.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
}

void CCMLoadLineItemDlg::Data2Dlg()
{
	m_wndName.SetWindowText(m_Data.Name);
	InitItemDataVar();
	UpdateData(FALSE);

	m_wndGrid.MakeItemEx();
	/*
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	int nSize = m_Data.aData.GetSize();
	m_wndGrid.SetRowCount(nSize);
	
	for (int i = 0; i < nSize; i++)
	{
		m_wndGrid.SetValueRange(CGXRange(i+1, 1), (double)m_Data.aData[i].x);
		m_wndGrid.SetValueRange(CGXRange(i+1, 2), (double)m_Data.aData[i].y);
		m_wndGrid.SetValueRange(CGXRange(i+1, 3), (double)m_Data.aData[i].z);
	}
	if (nSize > 0) m_wndGrid.SetCurrentCell(1, 1);
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
	*/

	//SelectNode();			// added by KYM
}

BOOL CCMLoadLineItemDlg::Dlg2Data()
{
	//T_LDLN_BASE item;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	UpdateData(TRUE);
	m_wndName.GetWindowText(m_Data.Name);

	/*
	int nCount = m_wndGrid.GetRowCount();
	m_Data.aData.SetSize(nCount);
	for (int i=0; i<nCount; i++)
	{
		item.x = _tstol(m_wndGrid.GetValueRowCol(i+1, 1));
		item.y = _tstol(m_wndGrid.GetValueRowCol(i+1, 2));
		item.z = _tstol(m_wndGrid.GetValueRowCol(i+1, 3));
		m_Data.aData.SetAt(i, item);
	}
	*/
	return TRUE;
}

void CCMLoadLineItemDlg::InitDlg()
{
	//m_wndName.SetWindowText(_T(""));
	m_Data.Initialize();
	m_wndGrid.MakeItemEx();
	m_wndName.SetWindowText(CreateGirderName());
	//m_wndGrid.SetRowCount(0);
	
	m_wndName.SetFocus();

	CMouseEdit::DisConnectAssocWindowST();
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	InitItemDataVar();
	Invalidate();
}

void CCMLoadLineItemDlg::InsertBaseItem(int nPos)
{
	T_LDLN_BASE item;
	double ux, uy, uz;
	double WX1, WY1, WZ1;

	if (!CFormulaEdit::GetPosition(&m_wndPointEdit, ux, uy, uz))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___1st_position_is_invalid_));
		return;
	}
	m_wndPointEdit.GetWcsPos(ux, uy, uz, WX1, WY1, WZ1);

	if (IsExistPoint(WX1, WY1, WZ1))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD_ERROR_POINT_ALREADY_EXIST));
		AfxMessageBox(msg);
		return;
	}
	item.x = WX1;
	item.y = WY1;
	item.z = WZ1;
	InsertGridItem(nPos, item);
	// focus처리등 해줄것. current cell
}

BOOL CCMLoadLineItemDlg::IsExistPoint(double dX, double dY, double dZ)
{
	int nCount = m_wndGrid.GetRowCount();
	double dTmpX, dTmpY, dTmpZ;
	double tmp;
	for (int i=0; i<nCount-1; i++)
	{
		tmp = _tstof(m_wndGrid.GetValueRowCol(i+1, 2));
		dTmpX = tmp - dX;
		tmp = _tstof(m_wndGrid.GetValueRowCol(i+1, 3));
		dTmpY = tmp - dY;
		tmp = _tstof(m_wndGrid.GetValueRowCol(i+1, 4));
		dTmpZ = tmp - dZ;
		
		tmp = (dTmpX*dTmpX + dTmpY*dTmpY + dTmpZ*dTmpZ);
		if (tmp <= D_ZERO)
			return TRUE;
	}
	return FALSE;
}

void CCMLoadLineItemDlg::InsertGridItem(int nPos, T_LDLN_BASE& item)
{
	int nRow;
	if (nPos < 0 || nPos > m_wndGrid.GetRowCount()) 
		nRow = m_wndGrid.GetRowCount();
	else nRow = nPos;

	if (!m_wndGrid.AddData(nRow, item)) ASSERT(0);

	/*
	ROWCOL nRow;
	if (nPos < 0 || nPos > m_wndGrid.GetRowCount()) nRow = m_wndGrid.GetRowCount();
	else nRow = nPos;

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	m_wndGrid.InsertRows(nRow+1, 1);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 1), item.x); 
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 2), item.y); 
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 3), item.z);

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
	*/
}

void CCMLoadLineItemDlg::SetItemData(CString& strName)
{
	T_LDLN_D rData;
	m_pDoc->m_pAttrCtrl->GetLdln(strName, rData);

	ROWCOL nRow;
	m_wndName.SetWindowText(rData.Name);
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	for (int i=0; i<rData.aData.GetSize(); i++)
	{
		nRow = m_wndGrid.GetRowCount();
		m_wndGrid.InsertRows(nRow+1, 1);
		m_wndGrid.SetValueRange(CGXRange(nRow+1, 1), rData.aData[i].x); 
		m_wndGrid.SetValueRange(CGXRange(nRow+1, 2), rData.aData[i].y); 
		m_wndGrid.SetValueRange(CGXRange(nRow+1, 3), rData.aData[i].z);
	}
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

BOOL CCMLoadLineItemDlg::DestroyWindow() 
{
	//((CCMLoadLineListDlg*)GetParent())->m_pWndChild = NULL;
	return CCMDlgBase::DestroyWindow();
}

BOOL CCMLoadLineItemDlg::PreTranslateMessage(MSG* pMsg) 
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

LRESULT CCMLoadLineItemDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	InsertBaseItem(-1);
	return 0L;
}

////////////////////////////////////////////////////////////////////////////
// 외부에서 Data를 Setting 하는 함수  
// 단, Dialog(Modaless)는 이미 Create&Activartion 되어있는 것을 전제로 한다.
// 이 함수를 Call 해주면 입력모드(List Mode가 아님) 상태에서 
// 인자로 넘어온 Key, Data를 Setting 하여 보여준다.
// by TAE
BOOL CCMLoadLineItemDlg::ExternalSetting(T_LDLN_K LdlnKey)
{  
	BOOL bFlag = TRUE;
	bFlag = ::IsWindow(this->GetSafeHwnd());                // 윈도우가 생성되지 않았으면
	bFlag = m_pDoc->m_pAttrCtrl->ExistLdln(LdlnKey);        // 키값이 존재하지 않으면
	bFlag = m_pDoc->m_pAttrCtrl->GetLdln(LdlnKey, m_Data);  // Data를 가져올 수 없으면
	if(!bFlag) return bFlag;

	m_csOldName = m_Data.Name;
	m_bModify = TRUE;
	Data2Dlg();

	return TRUE;
}

CString CCMLoadLineItemDlg::CreateGirderName()
{
	CString strName;
	int nCount = m_pDoc->m_pAttrCtrl->GetCountLdln() + 1;
	strName.Format(_T("L%d"), nCount);

	while(m_pDoc->m_pAttrCtrl->ExistLdln(strName))
	{
		nCount++;
		strName.Format(_T("L%d"), nCount);
	}
	return strName;
}
