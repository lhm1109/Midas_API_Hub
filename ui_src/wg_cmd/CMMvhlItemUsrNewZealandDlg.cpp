// CMMvhlItemUsrNewZealandDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrNewZealandDlg.h"
#include "CMMvhlItemUsrPermitDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvhlItemUsrNewZealandDlg dialog

#define COLCOUNT 3

CMMvhlItemUsrNewZealandDlg::CMMvhlItemUsrNewZealandDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CMMvhlItemUsrNewZealandDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;

	m_nLoadType = 0;

	m_Data.Initialize();

	m_bPermitLoadCall = FALSE;
}

void CMMvhlItemUsrNewZealandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMMvhlItemUsrNewZealandDlg)  
	DDX_Radio  (pDX, IDC_CMD_USER_TRK_RDO,                 m_nLoadType);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_NAME_EDT,           m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_P_EDT,		     m_edtTrkP);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_TRUCK_D_EDT,	       m_edtTrkD);
	DDX_Control(pDX, IDC_CMD_LIST,				                 m_List);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W_EDT,              m_edtLaneW);
	DDX_Control(pDX, IDC_CMD_USER_MVHL_W_UNT,              m_untLaneW);
	DDX_Control(pDX, IDC_CMD_USER_VEHI_PICTURE, m_wndPicture);
	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMMvhlItemUsrNewZealandDlg, CDialogMove)
	//{{AFX_MSG_MAP(CMMvhlItemUsrNewZealandDlg)
	ON_BN_CLICKED(IDC_CMD_USER_TRK_RDO,          OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_USER_PERMIT_RDO,       OnCmdMvhlLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD,               OnCmdMvhlAddBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_INSERT,            OnCmdMvhlInsBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY,            OnCmdMvhlModBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE,            OnCmdMvhlDelBtn)
	ON_BN_CLICKED(IDC_CMD_APPLY,                 OnCmdMvhlApplyBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST,     OnItemchangedCmdList)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CMMvhlItemUsrNewZealandDlg::InitUnit()
{
	m_edtLaneW .SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untLaneW .SetUnitType(D_UNITSYS_BASE_UNITFORCE);
}

void CMMvhlItemUsrNewZealandDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_CMD_USER_VEHI_LANE_GRP);
	aControls.Add(IDC_CMD_USER_MVHL_W_STC);
	aControls.Add(IDC_CMD_USER_MVHL_W_EDT);
	aControls.Add(IDC_CMD_USER_MVHL_W_UNT);

	CDlgUtil::CtrlShowHide(this, aControls,  m_nLoadType==0);
}

void CMMvhlItemUsrNewZealandDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CArray<UINT, UINT> aControls, aControls1; 
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_GRP);
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_P_STC);
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_P_EDT);
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_D_STC);
	aControls.Add(IDC_CMD_USER_VEHI_TRUCK_D_EDT);
	aControls.Add(IDC_CMD_LIST);
	aControls.Add(IDC_CMD_BTN_ADD);
	aControls.Add(IDC_CMD_BTN_INSERT);
	aControls.Add(IDC_CMD_BTN_MODIFY);
	aControls.Add(IDC_CMD_BTN_DELETE);

	CDlgUtil::CtrlEnableDisable(this, aControls, m_nLoadType==0);
}

void CMMvhlItemUsrNewZealandDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Load), _LS(IDS_WG_CMD__ADDD__Spacing)};
	
	// title에 단위 추가
	CString sLdUnit,sDtUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
	aTitle[1] = aTitle[1] +_T("(")+sLdUnit+_T(")");
	aTitle[2] = aTitle[2] +_T("(")+sDtUnit+_T(")");
	
	int nColWidth[COLCOUNT];
	
	CString title;
	int i;
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	
	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 70; nColWidth[2] = 80;
	
	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CMMvhlItemUsrNewZealandDlg::MakeItemEx()
{
	m_List.DeleteAllItems();
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	if (nLoadCount == 0) return;
	int i = 0;
	for (i = 0; i < nDistCount; i++)
		InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);
	
	if (nLoadCount > nDistCount)
		InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0);
}

BOOL CMMvhlItemUsrNewZealandDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
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
		
		if(i == 0) m_List.InsertItem(&lvitem);
		else       m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

BOOL CMMvhlItemUsrNewZealandDlg::DeleteItem(int nPos)
{
	if (nPos != -1) m_List.DeleteItem(nPos);
	return TRUE;
}

BOOL CMMvhlItemUsrNewZealandDlg::ModifyItem(int nPos, int nNo, double dLoad, double dDist)
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
			m_List.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}


void CMMvhlItemUsrNewZealandDlg::ChangeData()
{
	UpdateData(FALSE);

	ChangeBitMap();
	ControlsShowHide();
	EnableDisableControls();

}

void CMMvhlItemUsrNewZealandDlg::ChangeBitMap()
{	
	UpdateData(TRUE);

	CString strSVG = _T("cmd_mvhl_113.svg");
	if     (m_nLoadType == 0) strSVG = _T("cmd_mvhl_113.svg");
	
	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

//m_nLoadType      : 0=truck  1:Permit
//m_Data.nLoadType : 0=truck  1:Train  2:Permit  3:Rail
void CMMvhlItemUsrNewZealandDlg::Data2Dlg()
{
	if(m_Data.nLoadType == 2) m_nLoadType = 1;
	else                      m_nLoadType = m_Data.nLoadType;

	m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);
	m_edtLaneW .SetEditUnit(m_Data.dUniformLoad);
	
	MakeItemEx();
	
	UpdateData(FALSE);
}

//m_nLoadType      : 0=truck  1:Permit
//m_Data.nLoadType : 0=truck  1:Train  2:Permit  3:Rail
BOOL CMMvhlItemUsrNewZealandDlg::Dlg2Data()
{	
	UpdateData(TRUE);  
	m_Data.bStandard = FALSE;    

	if     (m_nLoadType == 1) m_Data.nLoadType = 2;
	else                      m_Data.nLoadType = m_nLoadType;

	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);	

	m_Data.dUniformLoad = m_edtLaneW.GetEditValue();

	return TRUE;
}

int CMMvhlItemUsrNewZealandDlg::GetLoadCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CMMvhlItemUsrNewZealandDlg::GetDistCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}
	return i;
}

BOOL CMMvhlItemUsrNewZealandDlg::GetLoadAndDistValue(double &dLoad, double &dDist)
{
	if (!CFormulaEdit::GetEditValue(&m_edtTrkP, dLoad))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_value_));
		return FALSE;
	}
	if (!CFormulaEdit::GetEditValue(&m_edtTrkD, dDist))
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

BOOL CMMvhlItemUsrNewZealandDlg::ApplyOrOK()
{
	BOOL bSuccess = FALSE;
	
	if (!Dlg2Data()) return FALSE;

	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else					 bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);

	if (!bSuccess) return FALSE;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMMvhlItemUsrNewZealandDlg message handlers

BOOL CMMvhlItemUsrNewZealandDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitUnit();
	SetHeaderTitle();
		
	Data2Dlg();

	ControlsShowHide();
	EnableDisableControls();
		
	UpdateData(FALSE);

	ChangeBitMap();

	OnCmdMvhlLoadTypeRdo();
 
	if(m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMMvhlItemUsrNewZealandDlg::OnOK()
{
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CMMvhlItemUsrNewZealandDlg::OnCmdMvhlLoadTypeRdo()
{
	UpdateData(TRUE);

	if(m_nLoadType == 1) // permit
	{
		if (m_bPermitLoadCall)
		{
			m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);
			m_bPermitLoadCall = FALSE;
			CCMMvhlItemUsrPermitDlg dlg;
			dlg.SetInitPos(D_INIT_POS_LT);
			CString csRadioText[2];
			GetDlgItem(IDC_CMD_USER_TRK_RDO)->GetWindowText(csRadioText[0]);
			GetDlgItem(IDC_CMD_USER_PERMIT_RDO)->GetWindowText(csRadioText[1]);
	
			T_MVCD_D MvcdD; MvcdD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return;

			dlg.SetModifyData(m_bModify, m_csOldName, &m_Data, m_pDoc, csRadioText, MvcdD.nCodeType);
			dlg.DoModal();
			//Type Change이면
			int nLastEvent = dlg.GetNextAction();
			if (nLastEvent == CCMMvhlItemUsrPermitDlg::CHANGE_TYPE)
			{
				CRect r = dlg.GetLastWindowPos();
				CRect rOrg;
				GetWindowRect(rOrg);
				r.right = r.left + rOrg.Width();
				r.bottom = r.top + rOrg.Height();
				MoveWindow(r);
				ShowWindow(SW_SHOW);
				m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);
				int nID = dlg.GetASType();
				m_nLoadType = nID;

				ChangeData();
			}
			else
			{
				CDialogMove::OnOK();
			}
		}
		else
		{
			m_bPermitLoadCall = TRUE;
			PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_USER_PERMIT_RDO, BN_CLICKED), 0);
		}
	}
	else
	{
		ChangeBitMap();
		ControlsShowHide();
		EnableDisableControls();
	}
}

void CMMvhlItemUsrNewZealandDlg::OnCmdMvhlAddBtn()
{
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;
	
	if (nLoadCount >= D_MVHL_NUMLOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MVHL_NUMLOAD);
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
		m_Data.dPointLoad[nLoadCount] = dLoad;
	}
	else
	{
		m_Data.dPointLoad[nLoadCount] = dLoad;
		m_Data.dPointDistance[nLoadCount] = dDist;
	}
	
	int no = nLoadCount+1;
	InsertItem(nLoadCount, no, dLoad, dDist);
	
	int nCount = m_List.GetItemCount();
	m_List.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CMMvhlItemUsrNewZealandDlg::OnCmdMvhlInsBtn()
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_inser));
		return;
	}
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;
	
	if (nLoadCount >= D_MVHL_NUMLOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MVHL_NUMLOAD);
		AfxMessageBox(csErr);
		return;
	}
	
	if (dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}
	
	// 데이타 이동하기
	for (int i = D_MVHL_NUMLOAD-1; i > iItem; i--)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i-1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i-1];
	}
	
	// 데이타 삽입한다.
	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;
	
	// List에 Item 삽입
	InsertItem(iItem, iItem+1, dLoad, dDist);
	
	// List 번호 변경
	int nCount = m_List.GetItemCount();
	CString csVal;
	for (int i = iItem+1; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_List.SetItemText(i, 0, csVal);
	}
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CMMvhlItemUsrNewZealandDlg::OnCmdMvhlModBtn()
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;
	
	if (iItem != nLoadCount-1 && dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}
	
	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;
	
	ModifyItem(iItem, iItem+1, dLoad, dDist);
}

void CMMvhlItemUsrNewZealandDlg::OnCmdMvhlDelBtn()
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	int i = iItem;
	for (i = iItem; i < D_MVHL_NUMLOAD-1; i++)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i+1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i+1];
	}
	m_Data.dPointLoad[i] = 0.0;
	m_Data.dPointDistance[i] = 0.0;
	
	m_List.DeleteItem(iItem);
	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	
	// List 번호 변경
	CString csVal;
	for (int i = iItem; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_List.SetItemText(i, 0, csVal);
	}
}

void CMMvhlItemUsrNewZealandDlg::OnCmdMvhlApplyBtn()
{
	ApplyOrOK();
}

void CMMvhlItemUsrNewZealandDlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	CString csVal;
	if (iItem == nLoadCount-1 && nLoadCount != nDistCount)  // fixed, last
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtTrkP.SetWindowText(csVal);
		m_edtTrkD.SetWindowText(_T(""));
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtTrkP.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.dPointDistance[iItem]);
		m_edtTrkD.SetWindowText(csVal);
	}
	
	*pResult = 0;
}