// CMHydrCoolElemItem.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMHydrCoolElemItem.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"


#include "..\wg_common\wg_common_TBGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMHydrCoolElemItem dialog


CCMHydrCoolElemItem::CCMHydrCoolElemItem(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMHydrCoolElemItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMHydrCoolElemItem)
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
	m_bListMode = TRUE;
	m_nCurStartSel = -1;

	m_aCtrlList.Add(IDC_CMD_LIST);
	m_aCtrlList.Add(IDC_CMD_BTN_ADD);
	m_aCtrlList.Add(IDC_CMD_BTN_MODIFY);
	m_aCtrlList.Add(IDC_CMD_BTN_DELETE);
	m_aCtrlList.Add(IDC_CMD_BTN_CLOSE);

	m_aCtrlData.Add(IDC_CMD_NAME_TITLE);
	m_aCtrlData.Add(IDC_CMD_COOL_NAME);
	m_aCtrlData.Add(IDC_CMD_COOLW_TITLE);
	m_aCtrlData.Add(IDC_CMD_SHEAT_TITLE);
	m_aCtrlData.Add(IDC_CMD_COOL_SPECIFIC);
	m_aCtrlData.Add(IDC_CMD_COOL_SPECIFIC_UNIT);
	m_aCtrlData.Add(IDC_CMD_DENSITY_TITLE);
	m_aCtrlData.Add(IDC_CMD_COOL_DENSITY);
	m_aCtrlData.Add(IDC_CMD_COOL_DENSITY_UNIT);
	m_aCtrlData.Add(IDC_CMD_INLET_TITLE);
	m_aCtrlData.Add(IDC_CMD_COOL_INLET);
	m_aCtrlData.Add(IDC_CMD_COOL_INLET_UNIT);
	m_aCtrlData.Add(IDC_CMD_FRATE_TITLE);
	m_aCtrlData.Add(IDC_CMD_COOL_FRATE);
	m_aCtrlData.Add(IDC_CMD_COOL_FRATE_UNIT);
	m_aCtrlData.Add(IDC_CMD_INFLOW_TITLE);
	m_aCtrlData.Add(IDC_CMD_START_TITLE);
	m_aCtrlData.Add(IDC_CMD_COOL_TSTART);
	//m_aCtrlData.Add(IDC_CMD_COOL_TSTART_SPIN);
	m_aCtrlData.Add(IDC_CMD_END_TITLE);
	m_aCtrlData.Add(IDC_CMD_COOL_TEND);
	//m_aCtrlData.Add(IDC_CMD_COOL_TEND_SPIN);
	m_aCtrlData.Add(IDC_CMD_COOL_TIME_UNIT);
	m_aCtrlData.Add(IDC_CMD_TEXT_STAGE1);     // 추가 v650
	m_aCtrlData.Add(IDC_CMD_TEXT_STAGE2);     // 추가 
	m_aCtrlData.Add(IDC_COBX_START_STAGE);    // 추가 
	m_aCtrlData.Add(IDC_COBX_END_STAGE);      // 추가 
	m_aCtrlData.Add(IDC_CMD_COOL_TIME_UNIT2); // 추가 
	m_aCtrlData.Add(IDC_CMD_COOLP_TITLE);
	m_aCtrlData.Add(IDC_CMD_DIA_TITLE);
	m_aCtrlData.Add(IDC_CMD_COOL_DIA);
	m_aCtrlData.Add(IDC_CMD_COOL_DIA_UNIT);
	m_aCtrlData.Add(IDC_CMD_CONVECT_TITLE);
	m_aCtrlData.Add(IDC_CMD_COOL_CONVECT);
	m_aCtrlData.Add(IDC_CMD_COOL_CONVECT_UNIT);
	m_aCtrlData.Add(IDC_CMD_SELECT_TITLE);
	m_aCtrlData.Add(IDC_CMD_COOL_SEL2P);
	m_aCtrlData.Add(IDC_CMD_COOL_SEL2P_EDIT1);
	m_aCtrlData.Add(IDC_CMD_COOL_SEL2P_EDIT1_UNIT);
	m_aCtrlData.Add(IDC_CMD_COOL_SEL2P_EDIT2);
	m_aCtrlData.Add(IDC_CMD_COOL_SEL2P_EDIT2_UNIT);
	m_aCtrlData.Add(IDC_CMD_COOL_SELPICK);
	m_aCtrlData.Add(IDC_CMD_COOL_SELPICK_EDIT);
	m_aCtrlData.Add(IDC_CMD_COOL_SELNODE);
	m_aCtrlData.Add(IDC_CMD_COOL_SELNODE_EDIT);
	m_aCtrlData.Add(IDC_CMD_PLACEHOLDER);
	m_aCtrlData.Add(IDC_CMD_COOL_ADD);
	m_aCtrlData.Add(IDC_CMD_COOL_INSERT);
	m_aCtrlData.Add(IDC_CMD_COOL_DEL);
	m_aCtrlData.Add(IDC_CMD_BTN_OK);
	m_aCtrlData.Add(IDC_CMD_BTN_CANCEL);
	m_aCtrlData.Add(IDC_CMD_BTN_APPLY);

	m_aCtrlSelect.Add(IDC_CMD_COOL_SEL2P);
	m_aCtrlSelect.Add(IDC_CMD_COOL_SELPICK);
	m_aCtrlSelect.Add(IDC_CMD_COOL_SELNODE);
}


void CCMHydrCoolElemItem::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMHydrCoolElemItem)
	DDX_Control(pDX, IDC_COBX_END_STAGE, m_cobxEndStage);
	DDX_Control(pDX, IDC_COBX_START_STAGE, m_cobxStartStage);
	DDX_Control(pDX, IDC_CMD_COOL_CONVECT_UNIT, m_dConvectUnit);
	DDX_Control(pDX, IDC_CMD_COOL_DIA_UNIT, m_dDiaUnit);
	DDX_Control(pDX, IDC_CMD_COOL_SEL2P_EDIT2_UNIT, m_2PointEditUnit2);
	DDX_Control(pDX, IDC_CMD_COOL_SEL2P_EDIT1_UNIT, m_2PointEditUnit1);
	DDX_Control(pDX, IDC_CMD_COOL_TIME_UNIT, m_TimeUnit);
	DDX_Control(pDX, IDC_CMD_COOL_TIME_UNIT2, m_TimeUnit2);
	DDX_Control(pDX, IDC_CMD_COOL_FRATE_UNIT, m_dFRateUnit);
	DDX_Control(pDX, IDC_CMD_COOL_INLET_UNIT, m_dInletUnit);
	DDX_Control(pDX, IDC_CMD_COOL_DENSITY_UNIT, m_dDensityUnit);
	DDX_Control(pDX, IDC_CMD_COOL_SPECIFIC_UNIT, m_dSHeatUnit);
	DDX_Control(pDX, IDC_CMD_COOL_CONVECT, m_dConvect);
	DDX_Control(pDX, IDC_CMD_COOL_DIA, m_dDia);
	DDX_Control(pDX, IDC_CMD_COOL_FRATE, m_dFRate);
	DDX_Control(pDX, IDC_CMD_COOL_INLET, m_dInlet);
	DDX_Control(pDX, IDC_CMD_COOL_DENSITY, m_dDensity);
	DDX_Control(pDX, IDC_CMD_COOL_SPECIFIC, m_dSHeat);
	DDX_Control(pDX, IDC_CMD_COOL_TEND, m_ITEnd);
	DDX_Control(pDX, IDC_CMD_COOL_TSTART, m_ITStart);
	//DDX_Control(pDX, IDC_CMD_COOL_TEND_SPIN, m_ITEndSpin);
	//DDX_Control(pDX, IDC_CMD_COOL_TSTART_SPIN, m_ITStartSpin);
	DDX_Control(pDX, IDC_CMD_COOL_SELPICK_EDIT, m_wndPickingEdit);
	DDX_Control(pDX, IDC_CMD_COOL_SELNODE_EDIT, m_wndNodeSelectEdit);
	DDX_Control(pDX, IDC_CMD_COOL_SEL2P_EDIT2, m_wnd2PointsEdit2);
	DDX_Control(pDX, IDC_CMD_COOL_SEL2P_EDIT1, m_wnd2PointsEdit1);
	DDX_Control(pDX, IDC_CMD_COOL_NAME, m_wndCoolName);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMHydrCoolElemItem, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMHydrCoolElemItem)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_OK, OnCmdBtnOk)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnCmdBtnCancel)
	ON_BN_CLICKED(IDC_CMD_BTN_APPLY, OnCmdBtnApply)
	ON_BN_CLICKED(IDC_CMD_COOL_SEL2P, OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_COOL_ADD, OnCmdCoolAdd)
	ON_BN_CLICKED(IDC_CMD_COOL_DEL, OnCmdCoolDel)
	ON_BN_CLICKED(IDC_CMD_COOL_INSERT, OnCmdCoolInsert)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_COOL_SELNODE, OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_COOL_SELPICK, OnCmdChangeSelectBy)
	ON_CBN_SELCHANGE(IDC_COBX_START_STAGE, OnSelchangeCobxStartStage)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMHydrCoolElemItem message handlers

BOOL CCMHydrCoolElemItem::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	
	AlignControl();
	SetHeaderTitle1();
	MakeItemEx();


	// set unit /////////////////////////////
	m_dConvectUnit.SetUnitType(CUnitCtrl::m_HPCE_UNIT.dConvecCoef);
	m_dDiaUnit.SetUnitType(CUnitCtrl::m_HPCE_UNIT.dDiameter);
	m_2PointEditUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_2PointEditUnit1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_TimeUnit.SetUnitType(CUnitCtrl::m_HPCE_UNIT.nStartTime);
	m_TimeUnit2.SetUnitType(CUnitCtrl::m_HPCE_UNIT.nStartTime);
	m_dFRateUnit.SetUnitType(CUnitCtrl::m_HPCE_UNIT.dFlowRate);
	m_dInletUnit.SetUnitType(CUnitCtrl::m_HPCE_UNIT.dTemper);
	m_dDensityUnit.SetUnitType(CUnitCtrl::m_HPCE_UNIT.dDensity);
	m_dSHeatUnit.SetUnitType(CUnitCtrl::m_HPCE_UNIT.dHeat);
	/////////////////////////////////////////

	// set edit /////////////////////////////
	m_dConvect.SetUnitType(CUnitCtrl::m_HPCE_UNIT.dConvecCoef);
	m_dDia.SetUnitType(CUnitCtrl::m_HPCE_UNIT.dDiameter);
	m_dFRate.SetUnitType(CUnitCtrl::m_HPCE_UNIT.dFlowRate);
	m_dInlet.SetUnitType(CUnitCtrl::m_HPCE_UNIT.dTemper);
	m_dDensity.SetUnitType(CUnitCtrl::m_HPCE_UNIT.dDensity);
	m_dSHeat.SetUnitType(CUnitCtrl::m_HPCE_UNIT.dHeat);
	/////////////////////////////////////////

	int nSelect = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlSelect, nSelect);
	m_wnd2PointsEdit1.EnableWindow(nSelect==0);
	m_wnd2PointsEdit2.EnableWindow(nSelect==0);
	m_wndPickingEdit.EnableWindow(nSelect==1);
	m_wndNodeSelectEdit.EnableWindow(nSelect==2);

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

	m_ITEnd.SetRange(0, 10000);
	m_ITEnd.SetValue(1);
	m_ITEnd.SetInteger(TRUE);
	m_ITStart.SetRange(0, 10000);
	m_ITStart.SetValue(1);
	m_ITStart.SetInteger(TRUE);
	//m_ITStartSpin.SetRange(0, 10000);
	//m_ITStartSpin.SetBase(1);

	//m_ITEndSpin.SetRange(0, 10000);
	//m_ITEndSpin.SetBase(1);

	InitGrid();
	InitItemDataVar();
	SetStageCobx();

	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMHydrCoolElemItem::InitItemDataVar()
{
	m_wnd2PointsEdit1.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wnd2PointsEdit2.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wndPickingEdit.SetWindowText(_T(""));
}

void CCMHydrCoolElemItem::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	GetDlgItem(IDC_CMD_LIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_COOL_NAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlData, nDistY);
	
	ResizeDialog();
}

void CCMHydrCoolElemItem::ResizeDialog()
{
	UINT nLastCtrlID;
	if (m_bListMode) nLastCtrlID = IDC_CMD_BTN_CLOSE;
	else nLastCtrlID = IDC_CMD_BTN_CANCEL;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

void CCMHydrCoolElemItem::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		MakeItemEx();
		//Data2Dlg();
		//UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
//    UpdateUnit();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMHydrCoolElemItem::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_HPCE_K Key, KeyBak;
	T_HPCE_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
	  switch(nCmd)
	  {
		case(UR_HPCE_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetHpce(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_HPCE_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetHpce(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		case(UR_HPCE_MFD):
			{
				pViewBuff->GetHpce(nKey, KeyBak, DataBak);
			}
			bMFD = TRUE;
			break;
		case(UR_HPCE_MFS):
			{
				pViewBuff->GetHpce(nKey, Key, Data);
				DeleteItem(Key, Data);
				InsertItem(KeyBak, DataBak);
			}
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}

void CCMHydrCoolElemItem::UpdateUnit() 
{
}
////////////////////////////////////////////////////////////////////////////












#define MAINLIST_COLCOUNT 1
/////////////////////////////////////////////////////////////////////////////
// List dlg
void CCMHydrCoolElemItem::SetHeaderTitle1()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Name)};
	int nColWidth[MAINLIST_COLCOUNT];

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
	for(i = 0; i < MAINLIST_COLCOUNT; i++)
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

void CCMHydrCoolElemItem::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountHpce();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_HPCE_K Key;
		T_HPCE_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartHpce();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextHpce(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetHpce(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

BOOL CCMHydrCoolElemItem::DeleteItem(T_HPCE_K Key, T_HPCE_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMHydrCoolElemItem::InsertItem(T_HPCE_K Key, T_HPCE_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Data.nSeq);
	for(int i = 0; i < MAINLIST_COLCOUNT; i++)
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

int CCMHydrCoolElemItem::FindInsertionPos(int nID)
{
	T_HPCE_K key;
	T_HPCE_D data;
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		key = m_List.GetItemData(i);
		m_pDoc->m_pAttrCtrl->GetHpce(key, data);
		if (nID < data.nSeq) break;
	}
	return i;
}

CString CCMHydrCoolElemItem::DataToStr(int i, T_HPCE_K Key, T_HPCE_D &Data)
{
	CString str;
	if (i == 0) str = Data.strName;
	else str = _T("");

	return str;
}

void CCMHydrCoolElemItem::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CCMHydrCoolElemItem::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	m_bModify = FALSE;
	m_Data.Initialize();
	Data2Dlg(); // initialize

	m_bListMode = FALSE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	m_wndCoolName.SetFocus();
}

void CCMHydrCoolElemItem::OnCmdBtnModify() 
{
	T_HPCE_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_Cooling_Eleme));
		return;
	}
 
	Key = (T_HPCE_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetHpce(Key, m_Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Cooling_Element_key__d_do), Key);
		AfxMessageBox(msg);
		return;
	}

	m_csOldCElemName = m_Data.strName;
	
	m_bModify = TRUE;
	Data2Dlg();

	m_bListMode = FALSE;	
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);	
	m_wndCoolName.SetFocus();	
}

void CCMHydrCoolElemItem::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_Cooling_Eleme));
		return;
	}

	CString csLineLaneName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelHpce(csLineLaneName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMHydrCoolElemItem::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnModify();
	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////












////////////////////////////////////////////////////////////////////////////
// Item dlg
void CCMHydrCoolElemItem::InitGrid()
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

	m_wndGrid.SetRowHeight(0, 0, globalUtils.ScaleByDPI(16));
	m_wndGrid.SetDrawingTechnique(gxDrawUsingMemDC);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_wndGrid.SetColCount(1);
	SetGridTitle();
	m_wndGrid.SetColWidth(0, 0, globalUtils.ScaleByDPI(40));
	m_wndGrid.SetColWidth(1, 1, globalUtils.ScaleByDPI(95));
	m_wndGrid.EnableOleDataSource(GX_DNDDISABLED); // copy & paste 금지

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMHydrCoolElemItem::SetGridTitle()
{
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_WG_CMD__ADDD__No));
	m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_WG_CMD__ADDD__Node));
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMHydrCoolElemItem::InsertBaseItem(int nPos, int nType)
{
	T_HPCE_BASE item;

	if (nType == 0)  // Select By 2 Points
	{
		CArray<T_NODE_K, T_NODE_K> aNodeKey;  
		double ux, uy, uz;
		double WX1, WY1, WZ1;
		double WX2, WY2, WZ2;

		if (!CFormulaEdit::GetPosition(&m_wnd2PointsEdit1, ux, uy, uz))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___1st_position_is_invalid_));
			return;
		}
		m_wnd2PointsEdit1.GetWcsPos(ux, uy, uz, WX1, WY1, WZ1);
		if (!CFormulaEdit::GetPosition(&m_wnd2PointsEdit2, ux, uy, uz))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___2nd_position_is_invalid_));
			return;
		}
		m_wnd2PointsEdit2.GetWcsPos(ux, uy, uz, WX2, WY2, WZ2);

		// Get Node Key List
		// 2000.6.5 Add by scchoi
	  double dbTolerance = m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();
	  m_pDoc->m_pDataCtrl->GetNodeBy2Point(WX1, WY1, WZ1, WX2, WY2, WZ2, dbTolerance, aNodeKey, TRUE);

		if (aNodeKey.GetSize() == 0)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_Node_on_line_));
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
				GSaveHistoryFormatNF(msg);
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
		if (nNodeKey <= 0)
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_not_exist_), nNodeKey);
			GSaveHistoryFormatNF(msg);
			return;
		}
		if (Grid_IsExistKey(nNodeKey))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_already_ex), nNodeKey);
			GSaveHistoryFormatNF(msg);
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

		if(arNodeList.GetSize() <= 0)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_node_));
			return;
		}

		for(int i=0; i<arNodeList.GetSize(); i++)
			if (Grid_IsExistKey((UINT)arNodeList[i]))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No__d__is_already_ex), (UINT)arNodeList[i]);
				GSaveHistoryFormatNF(msg);
				return;
			} 
		int nInsertPos = nPos;
		for(int i=0; i<arNodeList.GetSize(); i++)
		{
			T_NODE_D Data;

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

BOOL CCMHydrCoolElemItem::Grid_IsExistKey(int nKey)
{
	int nCount = m_wndGrid.GetRowCount();
	for (int i=0; i < nCount; i++)
	{
		if (nKey == _tstol(m_wndGrid.GetValueRowCol(i+1, 1))) return TRUE;
	}
	return FALSE;
}

void CCMHydrCoolElemItem::Grid_InsertItem(int nPos, T_HPCE_BASE &item)
{
	ROWCOL nRow;
	if (nPos < 0 || nPos > m_wndGrid.GetRowCount()) nRow = m_wndGrid.GetRowCount();
	else nRow = nPos;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	m_wndGrid.InsertRows(nRow+1, 1);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 1), (long)item.KeyNode); 

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMHydrCoolElemItem::SelectNode() 
{
	int nCount = m_wndGrid.GetRowCount();
	if(nCount > 0)
	{
	  CArray<T_NODE_K, T_NODE_K>arSelNode;
	  for (int i = 0; i < nCount; i++)arSelNode.Add(_tstol(m_wndGrid.GetValueRowCol(i+1, 1)));
	  m_pDoc->m_pViewCtrl->SelectNode(NULL, arSelNode, TRUE, TRUE);
	}
}

BOOL CCMHydrCoolElemItem::Dlg2Data()
{
	T_HPCE_BASE item;

	m_wndCoolName.GetWindowText(m_Data.strName);
	m_Data.dConvecCoef = m_dConvect.GetEditValue();
	m_Data.dDiameter = m_dDia.GetEditValue();
	m_Data.dFlowRate = m_dFRate.GetEditValue();
	m_Data.dTemper = m_dInlet.GetEditValue();
	m_Data.dDensity = m_dDensity.GetEditValue();
	m_Data.dHeat = m_dSHeat.GetEditValue();

	T_HSTG_K StartHstgK, EndHstgK;
	int nStartTime, nEndTime;
	CString str;
	m_ITStart.GetWindowText(str);
	nStartTime = _ttoi(str);
	m_ITEnd.GetWindowText(str);
	nEndTime = _ttoi(str);
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_cobxStartStage, (DWORD&)StartHstgK))
		StartHstgK = 0;
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_cobxEndStage, (DWORD&)EndHstgK))
		EndHstgK = 0;
	m_Data.nStartTime = CalcTime(StartHstgK, nStartTime);
	m_Data.nEndTime = CalcTime(EndHstgK, nEndTime);

	int nCount = m_wndGrid.GetRowCount();
	m_Data.aNode.SetSize(nCount);
	for (int i = 0; i < nCount; i++)
	{
		item.KeyNode = _tstol(m_wndGrid.GetValueRowCol(i+1, 1));
		m_Data.aNode.SetAt(i, item);
	}
	return TRUE;
}

void CCMHydrCoolElemItem::Data2Dlg()
{
	m_wndCoolName.SetWindowText(m_Data.strName);
	CString str;
	str.Format(_T("%g"), m_Data.dConvecCoef); m_dConvect.SetWindowText(str);
	str.Format(_T("%g"), m_Data.dDiameter);   m_dDia.SetWindowText(str);
	str.Format(_T("%g"), m_Data.dFlowRate);   m_dFRate.SetWindowText(str);
	str.Format(_T("%g"), m_Data.dTemper);     m_dInlet.SetWindowText(str);
	str.Format(_T("%g"), m_Data.dDensity);    m_dDensity.SetWindowText(str);
	str.Format(_T("%g"), m_Data.dHeat);       m_dSHeat.SetWindowText(str);
	// Stage, Time으로 변경됨(Dialog에서만)
	//str.Format(_T("%d"), m_Data.nStartTime);  m_ITStart.SetWindowText(str);
	//str.Format(_T("%d"), m_Data.nEndTime);    m_ITEnd.SetWindowText(str);
	T_HSTG_K StartHstgK, EndHstgK;
	int nStartTime, nEndTime;
	CalcStageTime(m_Data.nStartTime, StartHstgK, nStartTime);
	CalcStageTime(m_Data.nEndTime, EndHstgK, nEndTime);
	if (StartHstgK != 0) CDlgUtil::CobxSetCurSelItemData(m_cobxStartStage, StartHstgK);
	if (EndHstgK != 0) CDlgUtil::CobxSetCurSelItemData(m_cobxEndStage, EndHstgK);
	str.Format(_T("%d"), nStartTime); m_ITStart.SetWindowText(str);
	str.Format(_T("%d"), nEndTime); m_ITEnd.SetWindowText(str);
	InitItemDataVar();

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	int nSize = m_Data.aNode.GetSize();

	m_wndGrid.SetRowCount(nSize);
	for (int i=0; i < nSize; i++)
	{ 
		m_wndGrid.SetValueRange(CGXRange(i+1, 1), (long)m_Data.aNode[i].KeyNode);  
	}
	if (nSize > 0) m_wndGrid.SetCurrentCell(1, 1);
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();

	SelectNode();
}

void CCMHydrCoolElemItem::OnCmdBtnOk() 
{
	BOOL CheckCElem=FALSE;
	CString strTemp;
	m_wndCoolName.GetWindowText(strTemp);

	if((m_wndGrid.GetRowCount()) && ( strTemp == _T("")))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_Cooling_Element_Na),MB_OK);
		return;
	}

	if (!Dlg2Data()) return;
	if (m_bModify) CheckCElem = m_pDoc->m_pDataCtrl->ModifyHpce(m_csOldCElemName, m_Data);
	else CheckCElem = m_pDoc->m_pDataCtrl->AddHpce(m_Data);

	if (!CheckCElem)   // 입력 데이타에 오류가 있을때 Window를 Close하지 않게 한다.
		return; 


	m_bListMode = TRUE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	m_List.SetFocus();
}

void CCMHydrCoolElemItem::OnCmdBtnCancel() 
{
	// TODO: Add your control notification handler code here
	m_bListMode = TRUE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	m_List.SetFocus();
}

void CCMHydrCoolElemItem::OnCmdBtnApply() 
{
	BOOL CheckCElem=FALSE;
	CString strTemp;
	m_wndCoolName.GetWindowText(strTemp);

	if((m_wndGrid.GetRowCount()) && ( strTemp == _T("")))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_Cooling_Element_Na),MB_OK);
		return;
	}

	if (!Dlg2Data()) return;
	if (m_bModify) CheckCElem = m_pDoc->m_pDataCtrl->ModifyHpce(m_csOldCElemName, m_Data);
	else CheckCElem = m_pDoc->m_pDataCtrl->AddHpce(m_Data);
}

void CCMHydrCoolElemItem::OnCmdChangeSelectBy() 
{
	// TODO: Add your control notification handler code here
	int nSelect;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nSelect);
	
	m_wnd2PointsEdit1.EnableWindow(nSelect==0);
	m_wnd2PointsEdit2.EnableWindow(nSelect==0);
	m_wndPickingEdit.EnableWindow(nSelect==1);
	m_wndNodeSelectEdit.EnableWindow(nSelect==2);
	
	if (nSelect == 0) m_wnd2PointsEdit1.SetFocus();
	else if(nSelect == 1) m_wndPickingEdit.SetFocus();	
	else m_wndNodeSelectEdit.SetFocus();
}

LRESULT CCMHydrCoolElemItem::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	int nType;
	if (wParam == m_wnd2PointsEdit2.GetDlgCtrlID()) nType = 0;
	else if (wParam == m_wndPickingEdit.GetDlgCtrlID()) nType = 1;
	else if (wParam == m_wndNodeSelectEdit.GetDlgCtrlID()) nType = 2;
	else ASSERT(0);

	InsertBaseItem(-1, nType);
	if (nType==1) m_wnd2PointsEdit1.SetFocus();

	return 0L;
}

BOOL CCMHydrCoolElemItem::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pWnd = GetFocus();
		if (pWnd && pWnd->GetSafeHwnd() && IsWindow(pWnd->GetSafeHwnd()) &&
			pMsg->wParam == VK_DELETE && pWnd->GetDlgCtrlID() == IDC_CMD_PLACEHOLDER)
		{
			OnCmdCoolDel();
			return TRUE;
		}
	}
	return CCMDlgBase::PreTranslateMessage(pMsg);
}

void CCMHydrCoolElemItem::OnCmdCoolAdd() 
{
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nCheck);
	InsertBaseItem(-1, nCheck);
}

void CCMHydrCoolElemItem::OnCmdCoolInsert() 
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

void CCMHydrCoolElemItem::OnCmdCoolDel() 
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
}

void CCMHydrCoolElemItem::PostNcDestroy() 
{
	
	CCMDlgBase::PostNcDestroy();
	CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
}

// nStartIx : Start Stage의 선택 Index
void CCMHydrCoolElemItem::SetStageCobx(BOOL bOnlyEnd, int nStartIx)
{
	if (!bOnlyEnd) m_cobxStartStage.ResetContent();
	m_cobxEndStage.ResetContent();

	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	CArray<T_HSTG_K, T_HSTG_K> aHstgK;
	T_HSTG_D HstgD;
	pAttrCtrl->GetHstgKeyList(aHstgK);
	int nHstg = aHstgK.GetSize();
	for (int i = 0;i < nHstg; i++)
	{
		if (!pAttrCtrl->GetHstg(aHstgK[i], HstgD)) continue;
		if (!bOnlyEnd) CDlgUtil::CobxAddItem(m_cobxStartStage, HstgD.StageName, aHstgK[i]);
		if (i >= nStartIx)
			CDlgUtil::CobxAddItem(m_cobxEndStage, HstgD.StageName, aHstgK[i]);
	}
	if (!bOnlyEnd)
	{
		if (nHstg > 0) 
		{
			m_cobxStartStage.SetCurSel(nStartIx);
			m_nCurStartSel = nStartIx;
		}
		else m_nCurStartSel = -1;
		m_cobxEndStage.SetCurSel(0);
	}
	else m_nCurStartSel = nStartIx;
}

void CCMHydrCoolElemItem::OnSelchangeCobxStartStage() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_cobxStartStage.GetCurSel();
	if (nSel != m_nCurStartSel)
	{
		T_HSTG_K EndHstgK;
		if (!CDlgUtil::CobxGetItemDataByCurSel(m_cobxEndStage, (DWORD&)EndHstgK))
			EndHstgK = 0;
		SetStageCobx(TRUE, nSel);
		if (EndHstgK != 0)
		{
			if (CDlgUtil::CobxSetCurSelItemData(m_cobxEndStage, EndHstgK) == CB_ERR)
				m_cobxEndStage.SetCurSel(0);
		}
	}
}

// Stage와 시간을 주면 Total Time을 계산
int CCMHydrCoolElemItem::CalcTime(T_HSTG_K HstgK, int nTime) const
{
	int nTotalTime = 0;
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;

	CArray<T_HSTG_K, T_HSTG_K> aHstgK;
	T_HSTG_D HstgD;
	pAttrCtrl->GetHstgKeyList(aHstgK);
	int nHstg = aHstgK.GetSize();

	if (HstgK == 0 || !pAttrCtrl->ExistHstg(HstgK))
		return nTime;

	for (int i = 0; i < nHstg; i++)
	{
		if (aHstgK[i] == HstgK) break;
		VERIFY(pAttrCtrl->GetHstg(aHstgK[i], HstgD));
		int nStep = HstgD.aStep.GetSize();
		if (nStep < 1) continue;
		nTotalTime += (int)HstgD.aStep[nStep-1];
	}
	nTotalTime += nTime;
	return nTotalTime;
}

// Total Time을 주면 Stage와 시간을 계산
void CCMHydrCoolElemItem::CalcStageTime(int nTotalTime, T_HSTG_K& rHstgK, int& rnTime) const
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;

	int nCumulatedTime = 0;
	CArray<T_HSTG_K, T_HSTG_K> aHstgK;
	T_HSTG_D HstgD;
	pAttrCtrl->GetHstgKeyList(aHstgK);
	int nHstg = aHstgK.GetSize();
	if (nHstg == 0) 
	{
		rHstgK = 0;
		rnTime = nTotalTime;
		return;
	}
	int nTempTime;
	int i = 0;
	for (i = 0; i < nHstg; i++)
	{
		VERIFY(pAttrCtrl->GetHstg(aHstgK[i], HstgD));
		int nStep = HstgD.aStep.GetSize();
		if (nStep < 1) continue;
		nTempTime = HstgD.aStep[nStep-1];
		nCumulatedTime += nTempTime;
		if (nTotalTime <= nCumulatedTime)
		{
			rHstgK = aHstgK[i];
			rnTime = nTotalTime - (nCumulatedTime-nTempTime); // 총시간 - 이전Stage까지 누적 시간
			return;
		}
	}
	// 마지막 Stage Over하는 경우
	rHstgK = aHstgK[i-1];
	rnTime = nTotalTime - (nCumulatedTime-nTempTime);
}
