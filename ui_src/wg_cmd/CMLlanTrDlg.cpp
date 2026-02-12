// CMLlanTrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLlanTrDlg.h"

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
#include "..\wg_db\wg_db_FlagCtrl.h"  //added by KYM
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"  //added by KYM
#include "..\wg_db\wg_db_StagCtrl.h"  //added by KYM




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMLlanTrDlg dialog


CCMLlanTrDlg::CCMLlanTrDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMLlanTrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLlanTrDlg)
	m_nSelectionMethod = 0;
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
	m_bListMode = TRUE;
	m_bInitDlg = TRUE;

	m_aCtrlList.Add(IDC_CMD_LIST);
	m_aCtrlList.Add(IDC_CMD_BTN_ADD);
	m_aCtrlList.Add(IDC_CMD_BTN_MODIFY);
	m_aCtrlList.Add(IDC_CMD_BTN_DELETE);
	m_aCtrlList.Add(IDC_CMD_BTN_COPY);
	m_aCtrlList.Add(IDC_CMD_BTN_CLOSE);

	m_aCtrlData.Add(IDC_CMD_LANE_NAME_TITLE);
	m_aCtrlData.Add(IDC_CMD_LANE_NAME_EDIT);
	m_aCtrlData.Add(IDC_CMD_2POINT_UNIT1);
	m_aCtrlData.Add(IDC_CMD_2POINT_UNIT2);
	m_aCtrlData.Add(IDC_CMD_2POINTS_EDIT1);
	m_aCtrlData.Add(IDC_CMD_2POINTS_EDIT2);
	m_aCtrlData.Add(IDC_CMD_PICKING_EDIT);
	m_aCtrlData.Add(IDC_CMD_PICKING);
	m_aCtrlData.Add(IDC_CMD_2POINTS);
	m_aCtrlData.Add(IDC_CMD_SELECT_TYPE);
	m_aCtrlData.Add(IDC_CMD_BTN_OK);
	m_aCtrlData.Add(IDC_CMD_BTN_CANCEL);
	m_aCtrlData.Add(IDC_CMD_BTN_APPLY);
	m_aCtrlData.Add(IDC_CMD_LANE_IMPACT_STC);
	m_aCtrlData.Add(IDC_CMD_LANE_IMPACT_EDT);
	m_aCtrlData.Add(IDC_CMD_FRAME2);
	m_aCtrlData.Add(IDC_CMD_OP_ADD);
	m_aCtrlData.Add(IDC_CMD_OP_INSERT);
	m_aCtrlData.Add(IDC_CMD_OP_DELETE);
	m_aCtrlData.Add(IDC_CMD_PLACEHOLDER);

	m_aCtrlSelect.Add(IDC_CMD_2POINTS);
	m_aCtrlSelect.Add(IDC_CMD_PICKING);
}


void CCMLlanTrDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLlanTrDlg)
	DDX_Control(pDX, IDC_CMD_PICKING_EDIT, m_wndPickingEdit);
	DDX_Control(pDX, IDC_CMD_2POINT_UNIT2, m_2PointUnit2);
	DDX_Control(pDX, IDC_CMD_2POINT_UNIT1, m_2PointUnit1);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT2, m_wnd2PointsEdit2);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT1, m_wnd2PointsEdit1);
	DDX_Control(pDX, IDC_CMD_LANE_NAME_EDIT, m_wndLaneName);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Radio  (pDX, IDC_CMD_2POINTS, m_nSelectionMethod);
	DDX_Control(pDX, IDC_CMD_LANE_IMPACT_EDT, m_edtFactor);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMLlanTrDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMLlanTrDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD,     OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY,  OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE,  OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_COPY,    OnCmdBtnCopy)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE,   OnCmdBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_BTN_OK,      OnCmdBtnOk)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCEL,  OnCmdBtnCancel)
	ON_BN_CLICKED(IDC_CMD_BTN_APPLY,   OnCmdBtnApply)
	ON_BN_CLICKED(IDC_CMD_2POINTS,     OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_PICKING,     OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_OP_ADD,      OnCmdOpAdd)
	ON_BN_CLICKED(IDC_CMD_OP_INSERT,   OnCmdOpInsert)
	ON_BN_CLICKED(IDC_CMD_OP_DELETE,   OnCmdOpDelete)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)

END_MESSAGE_MAP()

#define D_NUM_COL 2
void CCMLlanTrDlg::InitGrid()
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
	m_wndGrid.SetColWidthByDPI(0, 0, 30);
	m_wndGrid.SetColWidthByDPI(1, 1, 70);
	m_wndGrid.SetColWidthByDPI(2, 2, 90);

	m_wndGrid.LockUpdate(bOldLock);
	if(!bOldLock) m_wndGrid.Redraw();
}

void CCMLlanTrDlg::SetGridTitle()
{
	CString csUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_LLAN_UNIT.Eccentricity, csUnit);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_WG_CMD__ADDD__No));
	m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_WG_CMD__ADDD__Elem));
	m_wndGrid.SetValueRange(CGXRange(0, 2), _LS(IDS_WG_CMD__ADDD__Scale_nFactor));

	m_wndGrid.LockUpdate(bOldLock);
	if(!bOldLock) m_wndGrid.Redraw();
}

BOOL CCMLlanTrDlg::Grid_IsExistKey(int nKey)
{
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		if (nKey == _tstol(m_wndGrid.GetValueRowCol(i+1, 1))) return TRUE;
	}
	return FALSE;
}

void CCMLlanTrDlg::Grid_InsertItem(int nPos, T_LLANtr_BASE &item)
{
	ROWCOL nRow;
	if (nPos < 0 || nPos > m_wndGrid.GetRowCount()) nRow = m_wndGrid.GetRowCount();
	else nRow = nPos;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.InsertRows(nRow+1, 1);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 1), (long)item.ElemK);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 2), item.dFactor);
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

/////////////////////////////////////////////////////////////////////////////
// CCMLlanTrDlg implementation functions
#define COLCOUNT 1
void CCMLlanTrDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	GetDlgItem(IDC_CMD_LIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LANE_NAME_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlData, nDistY);
	ResizeDialog();
}

void CCMLlanTrDlg::ResizeDialog()
{
	UINT nLastCtrlID;
	if (m_bListMode) nLastCtrlID = IDC_CMD_BTN_CLOSE;
	else nLastCtrlID = IDC_CMD_BTN_CANCEL;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CCMLlanTrDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Lane_Name)};
	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 140;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMLlanTrDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	T_LLANtr_D LlanD;
	CArray<T_LLANtr_K, T_LLANtr_K> aLlanK;
	m_pDoc->m_pAttrCtrl->GetLlantrKeyList(aLlanK);

	for (int i = 0; i < aLlanK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetLlantr(aLlanK[i], LlanD)) continue;

		InsertItem(aLlanK[i], LlanD);
	}
}

void CCMLlanTrDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		MakeItemEx();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
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
		MakeItemEx();
		ButtonCtrl();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMLlanTrDlg::ButtonCtrl()
{
	BOOL bEnable = FALSE;
	if (m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsBaseStag())
		bEnable = TRUE;

	GetDlgItem(IDC_CMD_BTN_OK)->EnableWindow(bEnable);
}

void CCMLlanTrDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	T_LLANtr_K Key, KeyBak;
	T_LLANtr_D Data, DataBak;

	BOOL bMFD = FALSE;
	BOOL bMvcdChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;		
		switch(nCmd)
		{
		case(UR_LLANtr_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetLlantr(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_LLANtr_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetLlantr(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		case(UR_LLANtr_MFD):
			{
				pViewBuff->GetLlantr(nKey, KeyBak, DataBak);
			}
			bMFD = TRUE;
			break;
		case(UR_LLANtr_MFS):
			{
				pViewBuff->GetLlantr(nKey, Key, Data);
				DeleteItem(Key, Data);
				InsertItem(KeyBak, DataBak);
			}
			bMFD = FALSE;
			break;
		case(UR_MVCD_ADD):
		case(UR_MVCD_DEL):
			bMvcdChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	// 코드가 바뀌면 무조건 닫는다.
	if (bMvcdChanged)
	{
		DestroyWindow();
	}
}

int CCMLlanTrDlg::FindInsertionPos(CString& strName)
{
	T_LLANtr_K key;
	T_LLANtr_D data;
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		key = m_List.GetItemData(i);
		m_pDoc->m_pAttrCtrl->GetLlantr(key, data);

		if (strName.Compare(data.LineLaneName) < 0) break;
	}
	return i;    
}
BOOL CCMLlanTrDlg::InsertItem(T_LLANtr_K Key, T_LLANtr_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Data.LineLaneName);
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}
BOOL CCMLlanTrDlg::DeleteItem(T_LLANtr_K Key, T_LLANtr_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}
BOOL CCMLlanTrDlg::ModifyItem(T_LLANtr_K KeyOld, T_LLANtr_K Key, T_LLANtr_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Key, Data);
			m_List.SetItemText(nItem, i, str);
		}
		// key 재 설정 
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem=0;
		lvitem.mask = LVIF_PARAM;
		lvitem.lParam = (LPARAM)Key;
		m_List.SetItem(&lvitem);
	}
	return TRUE;
}
CString CCMLlanTrDlg::DataToStr(int i, T_LLANtr_K Key, T_LLANtr_D &Data)
{
	CString str;
	if (i == 0) str = Data.LineLaneName;
	else str = _T("");

	return str;
}

BOOL CCMLlanTrDlg::SelectElem()
{
	int nCount = m_wndGrid.GetRowCount();
	if(nCount >= 0)
	{
	  CArray<T_ELEM_K, T_ELEM_K> aElemK;
	  for (int i = 0; i < nCount; i++) aElemK.Add(_tstol(m_wndGrid.GetValueRowCol(i+1, 1)));
	  m_pDoc->m_pViewCtrl->SelectElem(NULL, aElemK, FALSE, TRUE);
	}

	return TRUE;
}

void CCMLlanTrDlg::Data2Dlg()
{
	m_wndLaneName.SetWindowText(m_Data.LineLaneName);
	InitItemDataVar();

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	int nSize = m_Data.aLaneData.GetSize();
	m_wndGrid.SetRowCount(nSize);
	for(int i=0; i<nSize; ++i)
	{
		m_wndGrid.SetValueRange(CGXRange(i+1, 1), (long)m_Data.aLaneData[i].ElemK);
		m_wndGrid.SetValueRange(CGXRange(i+1, 2), m_Data.aLaneData[i].dFactor);
	}
	if(nSize>0) m_wndGrid.SetCurrentCell(1, 1);
	m_wndGrid.LockUpdate(bOldLock);
	if(!bOldLock) m_wndGrid.Redraw();

	SelectElem();
}

BOOL CCMLlanTrDlg::Dlg2Data()
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	
	m_wndLaneName.GetWindowText(m_Data.LineLaneName);
	int nCount = m_wndGrid.GetRowCount();
	m_Data.aLaneData.SetSize(nCount);

	T_LLANtr_BASE base;
	for(int i=0; i<nCount; ++i)
	{
		base.ElemK = _tstol(m_wndGrid.GetValueRowCol(i+1, 1));
		base.dFactor = _tstof(m_wndGrid.GetValueRowCol(i+1, 2));
		m_Data.aLaneData.SetAt(i, base);
	}
	return TRUE;
}

void CCMLlanTrDlg::InitItemDataVar()
{
	m_wnd2PointsEdit1.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wnd2PointsEdit2.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
}

void CCMLlanTrDlg::InsertBaseItem(int nPos, int nType)
{
	T_LLANtr_BASE base; base.Initialize();
	base.dFactor = m_edtFactor.GetEditValue();
	
	if(nType==0) //Select by 2 Points
	{
		CArray<T_ELEM_K, T_ELEM_K> aElemKey;
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

		// Get Elem Key List
		double dbTolerance = m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();  // added by KYM
		m_pDoc->m_pDataCtrl->GetFrameElemOnLine(WX1, WY1, WZ1, WX2, WY2, WZ2, aElemKey, TRUE);
		// Remove Element which is not beam
		T_ELEM_D Data;
		for (int i = aElemKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aElemKey[i], Data);
			if (!m_pDoc->m_pAttrCtrl->IsBeam(Data.eltyp))
				aElemKey.RemoveAt(i);
		}
		if (aElemKey.GetSize() == 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_beam_element_on_line_));
			return;
		}

		int nElemKey;
		int nCount = aElemKey.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			nElemKey = aElemKey[i];
			if (Grid_IsExistKey(nElemKey))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_already), nElemKey);
				AfxMessageBox(msg);
				return;
			}
			base.ElemK = (unsigned int)nElemKey;
			Grid_InsertItem(nPos, base);
			if (nPos >= 0) nPos++;
		}
		SelectElem();    //added by KYM
		return;
	}
	else if(nType==1) //Select by Picking
	{
		int nElemKey;
		CFormulaEdit::GetEditValue(&m_wndPickingEdit, nElemKey);
		if (nElemKey <= 0)
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_not_exi), nElemKey);
			AfxMessageBox(msg);
			return;
		}
		if (Grid_IsExistKey(nElemKey))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No__d__is_already), nElemKey);
			AfxMessageBox(msg);
			return;
		}
		T_ELEM_D Data;
		m_pDoc->m_pAttrCtrl->GetElem(nElemKey, Data);
		if (!m_pDoc->m_pAttrCtrl->IsBeam(Data.eltyp))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_not_a_b), nElemKey);
			AfxMessageBox(msg);
			return;
		}
		base.ElemK = (unsigned int)nElemKey;
		Grid_InsertItem(nPos, base);
	}

	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	if (!bCurrent) ncCol = 1;
	if (nPos == -1) ncRow = m_wndGrid.GetRowCount();
	else ncRow = nPos+1;

	m_wndGrid.SetCurrentCell(ncRow, ncCol);
	SelectElem();
}

/////////////////////////////////////////////////////////////////////////////
// CCMLlanTrDlg message handlers

///////////////////////////////////////////////////////
BOOL CCMLlanTrDlg::IsRelevantCode(int nCode)
{
	if (nCode == D_MOVE_CODE_AASHTO_STAN) return TRUE;
	if (nCode == D_MOVE_CODE_AASHTO_LRFD) return TRUE;
	if (nCode == D_MOVE_CODE_TAIWAN) return TRUE;
	if (nCode == D_MOVE_CODE_KOREA) return TRUE;
	if (nCode == D_MOVE_CODE_CANADA) return TRUE;
	if (nCode == D_MOVE_CODE_PENDOT) return TRUE;
	if (nCode == D_MOVE_CODE_BS) return TRUE;
	if (nCode == D_MOVE_CODE_EURO_BS) return TRUE;
	if (nCode == D_MOVE_CODE_RUSSIA) return TRUE;
	if (nCode == D_MOVE_CODE_KOREA_LRFD_2011) return TRUE;
	if (nCode == D_MOVE_CODE_AUSTRALIA) return TRUE;
	if (nCode == D_MOVE_CODE_POLAND) return TRUE;
	if (nCode == D_MOVE_CODE_SOUTH_AFRICA) return TRUE;
	if (nCode == D_MOVE_CODE_NEWZEALAND) return TRUE;
	if (nCode == D_MOVE_CODE_BRAZIL) return TRUE;
	return FALSE;
}


void CCMLlanTrDlg::SetParamData(T_LLANtr_D& data)
{
	m_bModify = TRUE;
	m_Data = data;
}

void CCMLlanTrDlg::SetInitModify()
{
	m_csOldLineLaneName = m_Data.LineLaneName;

	Data2Dlg();

	m_bListMode = FALSE;	
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	m_wndLaneName.SetFocus();
}



void CCMLlanTrDlg::SelectMethodMan()
{
	CArray<UINT,UINT> PointArr;
	CArray<UINT,UINT> PickingArr;
	
	PointArr.Add(IDC_CMD_2POINTS_EDIT1    );
	PointArr.Add(IDC_CMD_2POINTS_EDIT2    );
	PointArr.Add(IDC_CMD_2POINT_UNIT1     );
	PointArr.Add(IDC_CMD_2POINT_UNIT2     );
	PickingArr.Add(IDC_CMD_PICKING_EDIT   );
 
	if(m_nSelectionMethod == 0)
	{
		CDlgUtil::CtrlShowHide(this,PointArr  ,TRUE);
		CDlgUtil::CtrlShowHide(this,PickingArr,FALSE);
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
	}

	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
}



////////////////////////////////////////////////////////////////////////////
// 외부에서 Data를 Setting 하는 함수  
// 단, Dialog(Modaless)는 이미 Create&Activartion 되어있는 것을 전제로 한다.
// 이 함수를 Call 해주면 입력모드(List Mode가 아님) 상태에서 
// 인자로 넘어온 Key, Data를 Setting 하여 보여준다.
// by TAE
BOOL CCMLlanTrDlg::ExternalSetting(T_LLANtr_K LlanKey)
{
	BOOL bFlag = TRUE;
	bFlag = ::IsWindow(this->GetSafeHwnd());                // 윈도우가 생성되지 않았으면
	bFlag = m_pDoc->m_pAttrCtrl->ExistLlan(LlanKey);        // 키값이 존재하지 않으면
	bFlag = m_pDoc->m_pAttrCtrl->GetLlantr(LlanKey, m_Data);  // Data를 가져올 수 없으면
	if(!bFlag) return bFlag;

	m_csOldLineLaneName = m_Data.LineLaneName;
	m_bModify = TRUE;
	Data2Dlg();

	m_bListMode = FALSE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);	
	m_wndLaneName.SetFocus();
	SelectMethodMan();
	return TRUE;

}
 

BOOL CCMLlanTrDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	
	m_bInitDlg = TRUE;
	// TODO: Add extra initialization here
	AlignControl();
	SetHeaderTitle();
	MakeItemEx();
	ButtonCtrl();

	m_2PointUnit1.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_2PointUnit2.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);

	m_edtFactor.SetUnitType(0);
	m_edtFactor.SetEditUnit(1);

	m_wnd2PointsEdit1.SetAttUcsPos();
	m_wnd2PointsEdit1.SetModeToUse(MOUSEEDIT_USE_SETP2_FIRST);
	m_wnd2PointsEdit2.SetAttUcsPos();
	m_wnd2PointsEdit2.SetModeToUse(MOUSEEDIT_USE_SETP2_SECOND);
	m_wnd2PointsEdit2.SetLButtonDownNotifyWindow(this);
	m_wnd2PointsEdit2.SetEnterNotifyWindow(this);
	m_wnd2PointsEdit1.SetNextLink(&m_wnd2PointsEdit2);
	m_wnd2PointsEdit2.SetNextLink(&m_wnd2PointsEdit1);

	m_wndPickingEdit.SetAttElemList();
	m_wndPickingEdit.SetLButtonDownNotifyWindow(this);
	m_wndPickingEdit.SetEnterNotifyWindow(this);
	m_wndPickingEdit.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_wndPickingEdit.SetMaxElemKeyNum(1);

	m_csTitle1 = _LS(IDS_CMD_LLAN_TITLE1); // _T("Traffic Line Lanes")  //2
	m_csTitle2 = _LS(IDS_CMD_LLAN_TITLE2); // _T("Define Design Traffic Lane")//2

	InitGrid();
	InitItemDataVar();

	if(m_bModify)
	{
		SetInitModify();    
		SetWindowText(m_csTitle2);          //2
	}
	else
	{
		SetWindowText(m_csTitle1);          //2
		CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
		CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLlanTrDlg::OnCmdBtnAdd() 
{
	m_bModify = FALSE;
	m_bInitDlg = TRUE;
	m_Data.Initialize();
	Data2Dlg();

	m_bListMode = FALSE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	m_wndLaneName.SetFocus();
	SelectMethodMan();
	SetWindowText(m_csTitle2);
	
}

void CCMLlanTrDlg::OnCmdBtnModify() 
{
	T_LLANtr_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}
 
	Key = (T_LLANtr_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetLlantr(Key, m_Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Moving_load_lane_key__d_d), Key);
		AfxMessageBox(msg);
		return;
	}

	m_csOldLineLaneName = m_Data.LineLaneName;

	m_bModify = TRUE;
	m_bInitDlg = TRUE;
	Data2Dlg();

	m_bListMode = FALSE;	
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	m_wndLaneName.SetFocus();
	SelectMethodMan();
	SetWindowText(m_csTitle2);
}

void CCMLlanTrDlg::OnCmdBtnDelete() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}

	CString csLineLaneName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelLlantr(csLineLaneName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	
}


void CCMLlanTrDlg::OnCmdBtnCopy() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}
	CString csLineLaneName = m_List.GetItemText(iItem, 0);

	CArray<CString,CString&> LLaneNames;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLlanList(LLaneNames);

	CString StrName;
	if(!CStrParser::MakeUniqueName(LLaneNames,csLineLaneName,StrName))
	{
		AfxMessageBox(_LS(IDS_CMD0417__Unable_to_make_lane_Name___));
		return;
	}

	
	T_LLANtr_D LaneD, SourceLaneD;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLlantr(csLineLaneName,SourceLaneD))
	{
		CString StrTe;
		StrTe.Format(_LS(IDS_CMD0417__Not_Exist_Lane__s),csLineLaneName);
		AfxMessageBox(StrTe);
		return;
	}

	LaneD = SourceLaneD;
	LaneD.LineLaneName = StrName;
		
	CDBDoc::GetDocPoint()->m_pDataCtrl->AddLlantr(LaneD);
	
}

void CCMLlanTrDlg::OnCmdBtnClose() 
{
	DestroyWindow();	
}

void CCMLlanTrDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnModify();
	*pResult = 0;
}

void CCMLlanTrDlg::OnCmdBtnOk() 
{
	BOOL CheckLlan=FALSE;
	CString strTemp;
	m_wndLaneName.GetWindowText(strTemp);
	if((m_wndGrid.GetRowCount()) && ( strTemp == _T("")))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_Lane_Name__),MB_OK);
		return;
	}

	if (!Dlg2Data()) return;
	if (m_bModify) CheckLlan = m_pDoc->m_pDataCtrl->ModifyLlantr(m_csOldLineLaneName, m_Data);
	else CheckLlan = m_pDoc->m_pDataCtrl->AddLlantr(m_Data);

	if (!CheckLlan) // 입력 데이타에 오류가 있을때 Window를 Close하지 않게 한다.
		return;

	I_GENModelBase *pIGM = CTreeMenuBarBase::GetI_GENModelST(); ASSERT(pIGM);
	pIGM->UnselectAllElem();

	m_bListMode = TRUE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	m_List.SetFocus();
	SetWindowText(m_csTitle1);	
}

void CCMLlanTrDlg::OnCmdBtnCancel() 
{
	m_bListMode = TRUE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	m_List.SetFocus();
	SetWindowText(m_csTitle1);	
}

void CCMLlanTrDlg::OnCmdBtnApply() 
{
	CString strTemp;
	m_wndLaneName.GetWindowText(strTemp);
	if((m_wndGrid.GetRowCount()) && ( strTemp == _T("")))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_Lane_Name__),MB_OK);
		return;
	}

	if (!Dlg2Data()) return;
	if (m_bModify) m_pDoc->m_pDataCtrl->ModifyLlantr(m_csOldLineLaneName, m_Data);
	else m_pDoc->m_pDataCtrl->AddLlantr(m_Data);
}

void CCMLlanTrDlg::OnCmdChangeSelectBy() 
{
	int nSelect;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nSelect);
	
	m_wnd2PointsEdit1.EnableWindow(nSelect==0);
	m_wnd2PointsEdit2.EnableWindow(nSelect==0);
	m_wndPickingEdit.EnableWindow(nSelect==1);
	
	if (nSelect == 0) m_wnd2PointsEdit1.SetFocus();
	else if(nSelect == 1) m_wndPickingEdit.SetFocus();
	
	UpdateData(TRUE);
	SelectMethodMan();
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);

	if (m_nSelectionMethod == 0) m_wnd2PointsEdit1.SetFocus();
	else if(m_nSelectionMethod == 1) m_wndPickingEdit.SetFocus();
}

LRESULT CCMLlanTrDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (!m_bListMode)
	{
		if (m_nSelectionMethod == 0)
		{
			InsertBaseItem(-1, 0);
		}
		else if (m_nSelectionMethod == 1)
		{
			InsertBaseItem(-1, 1);
		}
		else
		{

		}
	}
	return 0L;

}

void CCMLlanTrDlg::OnCmdOpAdd() 
{
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nCheck);
	InsertBaseItem(-1, nCheck);
}

void CCMLlanTrDlg::OnCmdOpInsert() 
{
	// TODO: Add your control notification handler code here
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

void CCMLlanTrDlg::OnCmdOpDelete() 
{
	// TODO: Add your control notification handler code here
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

	SelectElem();  // added by KYM
}

BOOL CCMLlanTrDlg::PreTranslateMessage(MSG* pMsg) 
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