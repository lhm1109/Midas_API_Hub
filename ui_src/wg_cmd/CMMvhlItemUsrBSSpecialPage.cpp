// CMMvhlItemUsrBSSpecialPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrBSSpecialPage.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT 3
#define D_MAX_POINT_LOAD  29

#define CDialog CInternationalDlg
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSSpecialPage dialog


CCMMvhlItemUsrBSSpecialPage::CCMMvhlItemUsrBSSpecialPage(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemUsrBSSpecialPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrBSSpecialPage)
	//}}AFX_DATA_INIT
	m_pData = NULL;
	m_pData = pData;

	m_bAxleSpacing = FALSE;
}


void CCMMvhlItemUsrBSSpecialPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrBSSpecialPage)
	DDX_Control(pDX, IDC_CMD_MVHL_BS_TRUCK_P_EDT,      m_edtP);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_TRUCK_P_UNT,      m_untP);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_TRUCK_D_EDT,      m_edtD);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_TRUCK_D_UNT,      m_untD);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_TRUCK_LOAD_LST,   m_lstTruckL);
	DDX_Check  (pDX, IDC_CMD_MVHL_BS_AXLE_SPACING_CHK, m_bAxleSpacing);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_AXLE_NUM_EDT,     m_edtAxleNum);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_MIN_SPACING_EDT,  m_edtMinSpacing);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_MIN_SPACING_UNT,  m_untMinSpacing);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_MAX_SPACING_EDT,  m_edtMaxSpacing);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_MAX_SPACING_UNT,  m_untMaxSpacing);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvhlItemUsrBSSpecialPage, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrBSSpecialPage)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_TRUCK_ADD_BTN,    OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_TRUCK_INS_BTN,    OnCmdBtnIns)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_TRUCK_MOD_BTN,    OnCmdBtnMod)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_TRUCK_DEL_BTN,    OnCmdBtnDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_MVHL_BS_TRUCK_LOAD_LST, OnItemchangedCmdList)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_AXLE_SPACING_CHK, OnCmdAxleSpacingChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemUsrBSSpecialPage::SetData2Dlg()
{
	MakeItemEx();

	m_bAxleSpacing = m_pData->bVarSpacing;
	m_edtAxleNum.SetEditUnit(m_pData->nAxleNum);
	m_edtMinSpacing.SetEditUnit(m_pData->dMinSpacing);
	m_edtMaxSpacing.SetEditUnit(m_pData->dMaxSpacing);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemUsrBSSpecialPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->bVarSpacing = m_bAxleSpacing;
	m_pData->nAxleNum = m_edtAxleNum.GetEditValue();
	m_pData->dMinSpacing = m_edtMinSpacing.GetEditValue();
	m_pData->dMaxSpacing = m_edtMaxSpacing.GetEditValue();

	return TRUE;
}

void CCMMvhlItemUsrBSSpecialPage::InitUnit()
{
	CString sLdUnit,sDtUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
	sLdUnit = _T("(")+sLdUnit+_T(")");
	sDtUnit = _T("(")+sDtUnit+_T(")");

	GetDlgItem(IDC_CMD_MVHL_BS_TRUCK_P_UNT)->SetWindowText(sLdUnit);
	GetDlgItem(IDC_CMD_MVHL_BS_TRUCK_D_UNT)->SetWindowText(sDtUnit);

	m_edtAxleNum   .SetUnitType(D_UNITSYS_NONE);
	m_edtMinSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMinSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMaxSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMaxSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemUsrBSSpecialPage::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_CMD_MVHL_NO), _LS(IDS_CMD_MVHL_P), _LS(IDS_CMD_MVHL_D)};
	
	// title에 단위 추가
// 	CString sLdUnit,sDtUnit;
// 	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
// 	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
// 	aTitle[1] = aTitle[1] +_T("(")+sLdUnit+_T(")");
// 	aTitle[2] = aTitle[2] +_T("(")+sDtUnit+_T(")");
	
	int nColWidth[COLCOUNT];
	
	CString title;
	int i=0;
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstTruckL.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstTruckL.GetSafeHwnd(), dwStyle);
	
	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 45; nColWidth[2] = 45;
	
	// Set Title
	for(i=0; i<COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstTruckL.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvhlItemUsrBSSpecialPage::MakeItemEx()
{
	m_lstTruckL.DeleteAllItems();
	
	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();
	
	if (nLoadCount == 0) return;
	int i = 0;
	for (i = 0; i < nDistCount; i++)
		InsertItem(i, i+1, m_pData->dPointLoad[i], m_pData->dPointDistance[i]);
	if (nLoadCount > nDistCount)
		InsertItem(i, i+1, m_pData->dPointLoad[i], 0.0);
}

BOOL CCMMvhlItemUsrBSSpecialPage::InsertItem(int nPos, int nNo, double dLoad, double dDist)
{
	LVITEM lvitem;
	CString str;
	
	lvitem.iItem = nPos;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		if (i == 0) str.Format(_T("%d"), nNo);
		else if (i == 1) str.Format(_T("%g"), dLoad);
		else if (i == 2)
		{
			if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
			else str.Format(_T("%g"), dDist);
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;
		
		if(i == 0) m_lstTruckL.InsertItem(&lvitem);
		else       m_lstTruckL.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

BOOL CCMMvhlItemUsrBSSpecialPage::DeleteItem(int nPos)
{
	if (nPos != -1) m_lstTruckL.DeleteItem(nPos);
	return TRUE;
}

BOOL CCMMvhlItemUsrBSSpecialPage::ModifyItem(int nPos, int nNo, double dLoad, double dDist)
{
	int nItem = nPos;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			if (i == 0) str.Format(_T("%d"), nNo);
			else if (i == 1) str.Format(_T("%g"), dLoad);
			else if (i == 2)
			{
				if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
				else str.Format(_T("%g"), dDist);
			}
			else str = _LS(IDS_WG_CMD__ADDD__Error);
			m_lstTruckL.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

BOOL CCMMvhlItemUsrBSSpecialPage::GetLoadAndDistValue(double &dLoad, double &dDist)
{
	if (!CFormulaEdit::GetEditValue(&m_edtP, dLoad))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_value_));
		return FALSE;
	}
	if (!CFormulaEdit::GetEditValue(&m_edtD, dDist))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_distance_value_));
		return FALSE;
	}
	
	if (dLoad == 0.0) // Load 0은 허용 안함
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_value_must_not_be_ze));
		return FALSE;
	}
	return TRUE;
}

int CCMMvhlItemUsrBSSpecialPage::GetTruckLoadCount()
{
	int i = 0;
	for (i=0; i<D_MAX_POINT_LOAD; i++)
	{
		if (m_pData->dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemUsrBSSpecialPage::GetTruckDistanceCount()
{
	int i = 0;
	for (i=0; i<D_MAX_POINT_LOAD; i++)
	{
		if (m_pData->dPointDistance[i] == 0.0) break;
	}
	return i;
}


/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSSpecialPage message handlers
BOOL CCMMvhlItemUsrBSSpecialPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitUnit();
	SetHeaderTitle();

	SetData2Dlg();

	OnCmdAxleSpacingChk();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrBSSpecialPage::OnCmdBtnAdd() 
{
	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;

	if (nLoadCount >= D_MAX_POINT_LOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MAX_POINT_LOAD);
		AfxMessageBox(csErr);
		return;
	}
	if (nLoadCount > nDistCount) // Last 입력된 경우 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Delete_last_item_and_try_));
		return;
	}

	if (dDist == 0.0) // Last
	{
		m_pData->dPointLoad[nLoadCount] = dLoad;
	}
	else
	{
		m_pData->dPointLoad[nLoadCount] = dLoad;
		m_pData->dPointDistance[nLoadCount] = dDist;
	}

	int no = nLoadCount+1;
	InsertItem(nLoadCount, no, dLoad, dDist);

	int nCount = m_lstTruckL.GetItemCount();
	m_lstTruckL.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrBSSpecialPage::OnCmdBtnIns() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstTruckL.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_inser));
		return;
	}
		
	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();

	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;

	if (nLoadCount >= D_MAX_POINT_LOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MAX_POINT_LOAD);
		AfxMessageBox(csErr);
		return;
	}
	
	if (dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	// 데이타 이동하기
	for (int i = D_MAX_POINT_LOAD-1; i > iItem; i--)
	{
		m_pData->dPointLoad[i] = m_pData->dPointLoad[i-1];
		m_pData->dPointDistance[i] = m_pData->dPointDistance[i-1];
	}

	// 데이타 삽입한다.
	m_pData->dPointLoad[iItem] = dLoad;
	m_pData->dPointDistance[iItem] = dDist;

	// List에 Item 삽입
	InsertItem(iItem, iItem+1, dLoad, dDist);

	// List 번호 변경
	int nCount = m_lstTruckL.GetItemCount();
	CString csVal;
	for (int i = iItem+1; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_lstTruckL.SetItemText(i, 0, csVal);
	}
	m_lstTruckL.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrBSSpecialPage::OnCmdBtnMod() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstTruckL.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}
	
	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();

	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;

	if (iItem != nLoadCount-1 && dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	m_pData->dPointLoad[iItem] = dLoad;
	m_pData->dPointDistance[iItem] = dDist;

	ModifyItem(iItem, iItem+1, dLoad, dDist);
}

void CCMMvhlItemUsrBSSpecialPage::OnCmdBtnDel() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstTruckL.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}

	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();
	int i = 0;
	for (i = iItem; i < D_MAX_POINT_LOAD-1; i++)
	{
		m_pData->dPointLoad[i] = m_pData->dPointLoad[i+1];
		m_pData->dPointDistance[i] = m_pData->dPointDistance[i+1];
	}
	m_pData->dPointLoad[i] = 0.0;
	m_pData->dPointDistance[i] = 0.0;

	m_lstTruckL.DeleteItem(iItem);
	int nCount = m_lstTruckL.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstTruckL.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	// List 번호 변경
	CString csVal;
	for (i = iItem; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_lstTruckL.SetItemText(i, 0, csVal);
	}
}

void CCMMvhlItemUsrBSSpecialPage::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_lstTruckL.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
	
	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();
	
	CString csVal;
	if (iItem == nLoadCount-1 && nLoadCount != nDistCount)  // fixed, last
	{
		csVal.Format(_T("%g"), m_pData->dPointLoad[iItem]);
		m_edtP.SetWindowText(csVal);
		m_edtD.SetWindowText(_T(""));
	}
	else
	{
		csVal.Format(_T("%g"), m_pData->dPointLoad[iItem]);
		m_edtP.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_pData->dPointDistance[iItem]);
		m_edtD.SetWindowText(csVal);
	}
	
	*pResult = 0;
}

void CCMMvhlItemUsrBSSpecialPage::OnCmdAxleSpacingChk() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_BS_AXLE_NUM_STC)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_MVHL_BS_AXLE_NUM_EDT)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_MVHL_BS_MIN_SPACING_STC)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_MVHL_BS_MIN_SPACING_EDT)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_MVHL_BS_MIN_SPACING_UNT)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_MVHL_BS_MAX_SPACING_STC)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_MVHL_BS_MAX_SPACING_EDT)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_MVHL_BS_MAX_SPACING_UNT)->EnableWindow(m_bAxleSpacing);
}

BOOL CCMMvhlItemUsrBSSpecialPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		pMsg->wParam=9;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)
		pMsg->wParam=VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}
