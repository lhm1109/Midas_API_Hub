// ThisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ThisDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThisDlg dialog


CThisDlg::CThisDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CThisDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThisDlg)
	m_nAnalType = 0;
	//}}AFX_DATA_INIT
}


void CThisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThisDlg)
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME, m_wndLoadcaseName);
	DDX_Control(pDX, IDC_THIS_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_wndDesc);
	DDX_Control(pDX, IDC_CMD_DAMPING_LIST, m_DampingList);
	DDX_Control(pDX, IDC_CMD_DAMPING_EDIT, m_wndDamping);
	DDX_Control(pDX, IDC_CMD_MODE_EDIT, m_wndMode);
	DDX_Control(pDX, IDC_CMD_DAMPING_ALL_MODES, m_wndDampAllMode);
	DDX_Control(pDX, IDC_CMD_OUTPUT_STEP_EDIT, m_wndOutputStep);
	DDX_Control(pDX, IDC_CMD_TIME_INC_UNIT, m_wndTimeIncUnit);
	DDX_Control(pDX, IDC_CMD_TIME_INC_EDIT, m_wndTimeInc);
	DDX_Control(pDX, IDC_CMD_END_TIME_UNIT, m_wndEndTimeUnit);
	DDX_Control(pDX, IDC_CMD_END_TIME_EDIT, m_wndEndTime);
	DDX_Radio(pDX, IDC_CMD_ANAL_TYPE_TRANSIENT, m_nAnalType);
	//}}AFX_DATA_MAP
}

void CThisDlg::SetListCtrlHeader()
{
	CString aTitle[] = { _LS(IDS_CMD_THIS_loadcase), _LS(IDS_CMD_THIS_analysis_type), _T("") };
//18japan  CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__LoadCase), _T("Analysis Type"), _T("")};
	int nColWidth[] = {90, 95};
	int nColNum = 2;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CThisDlg::GetSelectedData(int &nIndex)
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CThisDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	CArray<T_THIS_K, T_THIS_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(aKeyList, FALSE);
	int nCount = aKeyList.GetSize();
	T_THIS_K key;
	T_THIS_D data;
	for (int i = 0; i < nCount; i++)
	{    
		key = aKeyList[i];
		m_pDoc->m_pAttrCtrl->GetThis(key, data);
		m_List.InsertItem(i, _T(""));
		SetItem(i, key, data);
	}
	m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CThisDlg::DataToStr(int i, T_THIS_D &data)
{
	CString value;
	CString aAnalType[] = {_LS(IDS_WG_CMD__ADDD__Transient), _LS(IDS_WG_CMD__ADDD__Periodic)};
	if (i == 0) value = data.LoadCaseName;
	else if (i == 1)
	{
		if (data.nAnalType >=1 && data.nAnalType <= 2) 
			value = aAnalType[data.nAnalType-1];
		else value = _LS(IDS_WG_CMD__ADDD__Error);
	}
	return value;
}

void CThisDlg::SetItem(int nIndex, T_THIS_K key, T_THIS_D &data)
{
	LVITEM lvitem;
	CString str;
	lvitem.iItem=nIndex;
	for(int i = 0; i < 2; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		m_List.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
	m_List.SetItemData(nIndex, (DWORD)key);
}

BOOL CThisDlg::ValidData(T_THIS_D& data)
{
	if (data.LoadCaseName.IsEmpty()) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Loadcase_name_is_not_inpu));
		return FALSE;
	}
	if (data.dEndTime < 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___End_of_time_must_be_great));
		return FALSE;
	}
	if (data.dDelta < 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Time_increment_must_be_gr));
		return FALSE;
	}
	if (data.nOut < 1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Out_must_be_greater_than_));
		return FALSE;
	}
		
	return TRUE;
}

BOOL CThisDlg::Dlg2Data(T_THIS_D &data)
{
	m_wndLoadcaseName.GetWindowText(data.LoadCaseName);
	data.dEndTime = m_wndEndTime.GetEditValue();
	data.dDelta = m_wndTimeInc.GetEditValue();
	data.nOut = m_wndOutputStep.GetEditValue();
	data.nAnalType = m_nAnalType+1;
	data.dDampAll = m_wndDampAllMode.GetEditValue();
	data.aDampData.Copy(m_Data.aDampData);
	m_wndDesc.GetWindowText(data.Description);

	return TRUE;
}

BOOL CThisDlg::Data2Dlg(T_THIS_D &data)
{
	m_Data = data;

	CString csVal;
	m_wndLoadcaseName.SetWindowText(data.LoadCaseName);
	csVal.Format(_T("%g"), data.dEndTime);
	m_wndEndTime.SetWindowText(csVal);
	csVal.Format(_T("%g"), data.dDelta);
	m_wndTimeInc.SetWindowText(csVal);
	csVal.Format(_T("%d"), data.nOut);
	m_wndOutputStep.SetWindowText(csVal);
	ChangeAnalType(data.nAnalType);
	csVal.Format(_T("%g"), data.dDampAll);
	m_wndDampAllMode.SetWindowText(csVal);
	//m_Data.aDampData.Copy(data.aDampData);
	MakeItemExDamping();
	m_wndDesc.SetWindowText(data.Description);

	return TRUE;
}

BOOL CThisDlg::ChangeAnalType(int nAnalType)
{
	if (nAnalType == 0) nAnalType = 1;
	CButton* pRadio1 = (CButton*)GetDlgItem(IDC_CMD_ANAL_TYPE_TRANSIENT);
	CButton* pRadio2 = (CButton*)GetDlgItem(IDC_CMD_ANAL_TYPE_PERIODIC);
	if (!pRadio1 || !pRadio2) return FALSE;
	if (!pRadio1->GetSafeHwnd() || !pRadio2->GetSafeHwnd()) return FALSE;

	m_nAnalType = nAnalType-1;
	if (nAnalType == 1)
	{
		pRadio1->SetCheck(TRUE);
		pRadio2->SetCheck(FALSE);
	}
	else
	{
		pRadio1->SetCheck(FALSE);
		pRadio2->SetCheck(TRUE);
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// For Damping
void CThisDlg::SetListCtrlHeaderDamping()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Mode), _LS(IDS_WG_CMD__ADDD__Damping)};
	int nColWidth[] = {72, 96};
	int nColNum = 2;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_DampingList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_DampingList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_DampingList.InsertColumn(i,&lvcolumn);
		m_DampingList.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CThisDlg::GetSelectedDataDamping(int &nIndex)
{
	int iItem = m_DampingList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CThisDlg::MakeItemExDamping()
{
	m_DampingList.DeleteAllItems();

	int nItemCount = m_Data.aDampData.GetSize();
	if(nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_DampingList.InsertItem(nCount, _T(""));
		SetItemDamping(nCount, m_Data.aDampData[nCount]);
	}
	m_DampingList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CThisDlg::DataToStrDamping(int i, T_THIS_BASE &data)
{
	CString value;
	if (i == 0) value.Format(_T("%d"), data.nModeId);
	else if (i == 1) value.Format(_T("%g"), data.dDamping);
	else value = _LS(IDS_WG_CMD__ADDD__Error);
	return value;
}

void CThisDlg::SetItemDamping(int nIndex, T_THIS_BASE &data)
{
	LVITEM lvitem;
	CString str;
	for(int i = 0; i < 3; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStrDamping(i, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_DampingList.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
}

BOOL CThisDlg::Dlg2ThisBase(T_THIS_BASE &data)
{
	data.nModeId = (int)m_wndMode.GetEditValue();
	if (data.nModeId <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Mode_must_be_greater_than));
		return FALSE;
	}
	data.dDamping = m_wndDamping.GetEditValue();

	return TRUE;
}

BOOL CThisDlg::ThisBase2Dlg(T_THIS_BASE &data)
{
	CString csVal;
	csVal.Format(_T("%d"), data.nModeId);
	m_wndMode.SetWindowText(csVal);
	csVal.Format(_T("%g"), data.dDamping);
	m_wndDamping.SetWindowText(csVal);

	return TRUE;
}

void CThisDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
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
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CThisDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bThisChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_THIS_ADD):
		case(UR_THIS_DEL):
		case(UR_THIS_MFD):
		case(UR_THIS_MFS):
				bThisChanged = TRUE;
				break;
		default:
			break;
		}
	} // end of while

	if (bThisChanged) MakeItemEx();
}

BEGIN_MESSAGE_MAP(CThisDlg, CDialogMove)
	//{{AFX_MSG_MAP(CThisDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_END_TIME_SPIN, OnDeltaposDouble)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_OUTPUT_STEP_SPIN, OnDeltaposInteger)
	ON_BN_CLICKED(IDC_CMD_ANAL_TYPE_TRANSIENT, OnChangeAnalType)
	ON_BN_CLICKED(IDC_CMD_DAMPING_ADD, OnDampingAdd)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MODIFY, OnDampingModify)
	ON_BN_CLICKED(IDC_CMD_DAMPING_DELETE, OnDampingDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_SPEC_EIGV_DLG, OnCmdSpecEigvDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_TIME_INC_SPIN, OnDeltaposDouble)
	ON_BN_CLICKED(IDC_CMD_ANAL_TYPE_PERIODIC, OnChangeAnalType)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_MODE_SPIN, OnDeltaposInteger)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_DAMPING_LIST, OnChangedCurThisBase)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_THIS_LIST, OnChangeCurThis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThisDlg message handlers

BOOL CThisDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_Data.Initialize();

	// ListCtrl의 헤더를 설정한다.
	SetListCtrlHeader();
	SetListCtrlHeaderDamping();

	m_wndEndTime.SetUnitType(CUnitCtrl::m_THIS_UNIT.dEndTime);
	m_wndEndTimeUnit.SetUnitType(CUnitCtrl::m_THIS_UNIT.dEndTime);
	m_wndTimeInc.SetUnitType(CUnitCtrl::m_THIS_UNIT.dDelta);
	m_wndTimeIncUnit.SetUnitType(CUnitCtrl::m_THIS_UNIT.dDelta);
	m_wndEndTime.SetWindowText(_T("1"));
	m_wndTimeInc.SetWindowText(_T("1"));
	m_wndOutputStep.SetWindowText(_T("1"));
	m_wndMode.SetWindowText(_T("1"));

	// ListCtrl의 값을 설정
	MakeItemEx();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CThisDlg::OnDeltaposDouble(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	CEditUnit* pEdit;
	if (nID == IDC_CMD_END_TIME_SPIN) pEdit = &m_wndEndTime;
	else if (nID == IDC_CMD_TIME_INC_SPIN) pEdit = &m_wndTimeInc;
	else 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Unknowon_spin_button));
		return;
	}
	CString csVal;
	double dblValue = pEdit->GetEditValue();
	dblValue -= pNMUpDown->iDelta;
	if (dblValue < 0.0) dblValue = 0.0;
	csVal.Format(_T("%g"), dblValue);
	pEdit->SetWindowText(csVal);
	
	*pResult = 0;
}

void CThisDlg::OnDeltaposInteger(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	CEditUnit* pEdit;
	if (nID == IDC_CMD_OUTPUT_STEP_SPIN) pEdit = &m_wndOutputStep;
	else if (nID == IDC_CMD_MODE_SPIN) pEdit = &m_wndMode;
	else 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Unknowon_spin_button));
		return;
	}
	CString csVal;
	int      iVal = (int)pEdit->GetEditValue();
	iVal -= pNMUpDown->iDelta;
	if (iVal < 1) iVal = 1;
	csVal.Format(_T("%d"), iVal);
	pEdit->SetWindowText(csVal);
	
	*pResult = 0;
}

void CThisDlg::OnChangeAnalType() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	if (nID == IDC_CMD_ANAL_TYPE_TRANSIENT) m_nAnalType = 0;
	else if (nID == IDC_CMD_ANAL_TYPE_PERIODIC) m_nAnalType = 1;
}

void CThisDlg::OnDampingAdd() 
{
	// TODO: Add your control notification handler code here
	T_THIS_BASE data;
	
	if (!Dlg2ThisBase(data)) return;
	int nSize = m_Data.aDampData.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (m_Data.aDampData[i].nModeId == data.nModeId)
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Mode__d_already_exist_), data.nModeId);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.aDampData.Add(data);
	m_DampingList.InsertItem(nSize, _T(""));
	SetItemDamping(nSize, data);
	m_DampingList.SetItemState(nSize, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	

	CString csVal;
	csVal.Format(_T("%d"), data.nModeId+1);
	m_wndMode.SetWindowText(csVal);
	GotoDlgCtrl(&m_wndDamping);
}

void CThisDlg::OnDampingModify() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedDataDamping(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_modal_damping_ratio__));
		return;
	}

	T_THIS_BASE data;
	if (!Dlg2ThisBase(data)) return;

	int nSize = m_Data.aDampData.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (i == nIndex) continue;  // 변경 대상은 비교 제외
		if (m_Data.aDampData[i].nModeId == data.nModeId)
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Mode__d_already_exist_), data.nModeId);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.aDampData[nIndex] = data;
	SetItemDamping(nIndex, data);
	m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CThisDlg::OnDampingDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedDataDamping(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_modal_damping_ratio__));
		return;
	}

	m_DampingList.DeleteItem(nIndex);
	m_Data.aDampData.RemoveAt(nIndex);
	int nSize = m_Data.aDampData.GetSize();
	if (nSize > 0)
	{
		if (nIndex >= nSize) nIndex--;
		m_DampingList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}	
}

void CThisDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	T_THIS_D data;
	
	if (!Dlg2Data(data)) return;
	if (!ValidData(data)) return;

	int nCount = m_List.GetItemCount();
	if (m_pDoc->m_pDataCtrl->AddThis(data))
		m_List.SetItemState(nCount, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CThisDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedData(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_loadcase));
		return;
	}

	T_THIS_D data;
	if (!Dlg2Data(data)) return;
	if (!ValidData(data)) return;
	
	data.LoadCaseId = m_Data.LoadCaseId;
	CString csLCname = m_Data.LoadCaseName;
	if (m_pDoc->m_pDataCtrl->ModifyThis(csLCname, data))
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CThisDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedData(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_loadcase));
		return;
	}
	int nCount = m_List.GetItemCount();
	if (m_pDoc->m_pDataCtrl->DelThis(m_Data.LoadCaseName))
	{
		if (nIndex >= nCount-1) nIndex--;
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CThisDlg::OnCmdClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CThisDlg::OnCmdSpecEigvDlg() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_EIGENVALUE, 0));	
}

void CThisDlg::OnChangedCurThisBase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex;
	T_THIS_BASE data;
	if (!GetSelectedDataDamping(nIndex))
		data.Initialize();
	else data = m_Data.aDampData[nIndex];

	// 값을 설정한다.
	ThisBase2Dlg(data);

	*pResult = 0;
}

void CThisDlg::OnChangeCurThis(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	T_THIS_D data;
	int nIndex;
	if (!GetSelectedData(nIndex))
		data.Initialize();
	else 
	{
		T_THIS_K key = m_List.GetItemData(nIndex);
		m_pDoc->m_pAttrCtrl->GetThis(key, data);
	}

	// 값을 설정한다.
	Data2Dlg(data);

	*pResult = 0;
}
