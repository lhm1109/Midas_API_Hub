// CMMvldItemKRLRFD11Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemKRLRFD11Dlg.h"
#include "CMMvldSubItemDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemKRLRFD11Dlg dialog

CCMMvldItemKRLRFD11Dlg::CCMMvldItemKRLRFD11Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemKRLRFD11Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemKRLRFD11Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
	m_bModify = FALSE;
}


void CCMMvldItemKRLRFD11Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemKRLRFD11Dlg)
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME,   m_edtLoadCase);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION,     m_edtDesc);
	DDX_Control(pDX, IDC_CMD_SF1,             m_edtSF1);
	DDX_Control(pDX, IDC_CMD_SF2,             m_edtSF2);
	DDX_Control(pDX, IDC_CMD_SF3,             m_edtSF3);
	DDX_Control(pDX, IDC_CMD_SF4,             m_edtSF4);
	DDX_Control(pDX, IDC_CMD_SF5,             m_edtSF5);
	DDX_Radio  (pDX, IDC_CMD_RADIO_COMBINED,  m_nCombOpt);
	DDX_Control(pDX, IDC_CMD_LIST,            m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvldItemKRLRFD11Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemKRLRFD11Dlg)
	ON_BN_CLICKED(IDC_CMD_APPLY,       OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD,     OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY,  OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE,  OnCmdBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCMMvldItemKRLRFD11Dlg::OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos)
{
	if (!bModify)
	{
		int nCount = m_Data.arSubLoadData.GetSize();
		m_Data.arSubLoadData.Add(data);
		InsertItem(nCount, data);
	}
	else
	{
		m_Data.arSubLoadData.SetAt(nPos, data);
		ModifyItem(nPos, data);
	}
	return TRUE;
}

void CCMMvldItemKRLRFD11Dlg::InitUnit()
{
	m_edtLoadCase.SetUnitType(D_UNITSYS_NONE);
	m_edtDesc.SetUnitType(D_UNITSYS_NONE);
	
	m_edtSF1.SetUnitType(D_UNITSYS_NONE);
	m_edtSF2.SetUnitType(D_UNITSYS_NONE);
	m_edtSF3.SetUnitType(D_UNITSYS_NONE);
	m_edtSF4.SetUnitType(D_UNITSYS_NONE);
	m_edtSF5.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvldItemKRLRFD11Dlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Vehicle_class), _LS(IDS_WG_CMD__ADDD__ReducFact), _LS(IDS_WG_CMD__ADDD__Lane1), _LS(IDS_WG_CMD__ADDD__Lane2), _LS(IDS_WG_CMD__ADDD__Lane3), _LS(IDS_WG_CMD__ADDD__Lane4), _LS(IDS_WG_CMD__ADDD__Lane5), _LS(IDS_WG_CMD__ADDD__More)};
	int nColWidth[COLCOUNT] = {108, 50, 60, 60, 60, 60, 60, 48};
	
	CString title;
	int i;
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	
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

void CCMMvldItemKRLRFD11Dlg::MakeItemEx()
{
	m_List.DeleteAllItems();
	
	int nItemCount = m_Data.arSubLoadData.GetSize();
	if(nItemCount == 0) return;
	
	CString str;
	int nCount;
	
	if(nItemCount)
	{
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			InsertItem(nCount, m_Data.arSubLoadData[nCount]);
		}
	}
}

BOOL CCMMvldItemKRLRFD11Dlg::InsertItem(int nIndex, T_MVLD_BASE &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;
	
	lvitem.iItem = nIndex;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;
		
		if( i == 0 ) 
		{
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

BOOL CCMMvldItemKRLRFD11Dlg::DeleteItem(int nIndex, T_MVLD_BASE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_List.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemKRLRFD11Dlg::ModifyItem(int nIndex, T_MVLD_BASE &Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Data);
			m_List.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMMvldItemKRLRFD11Dlg::DataToStr(int i, T_MVLD_BASE &Data)
{
	CString str;
	
	T_MVHC_D mvhc;
	T_MVHL_D mvhl;
	if(i==0) 
	{
		if (Data.nVehicleType == 1)
		{
			m_pDoc->m_pAttrCtrl->GetMvhc(Data.VehicleKey, mvhc);
			str = _T("VC:")+mvhc.VehicleClassName;
		}
		else
		{
			m_pDoc->m_pAttrCtrl->GetMvhl(Data.VehicleKey, mvhl);
			str = _T("VL:")+mvhl.VehicleLoadName;
		}
	}
	else if(i==1) str.Format(_T("%g"), Data.dVehicleScaleFactor);
	else if(i>=2 && i<=6)
	{
		if(Data.aSelectedLanes.GetSize() > i-2)
		{
			if(Data.aSelectedLanes[i-2] == 0) str = _T("");
			else
			{
				str = GetLaneName(Data.aSelectedLanes[i-2]);
			}
		}
		else str = _T("");
	}
	else if (i==7)
	{
		if(Data.aSelectedLanes.GetSize() > i-2)
		{
			if (Data.aSelectedLanes[i-2] == 0) str = _T("");
			else str = _T("...");
		}
		else str = _T("");
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);
	
	return str;
}

CString CCMMvldItemKRLRFD11Dlg::GetLaneName(UINT nLaneKey)
{
	if (m_bSurface)
	{
		T_SLAN_D slan;
		m_pDoc->m_pAttrCtrl->GetSlan(nLaneKey, slan);
		return slan.LaneName;
	}
	else
	{
		T_LLAN_D llan;
		m_pDoc->m_pAttrCtrl->GetLlan(nLaneKey, llan);
		return llan.LineLaneName;
	}
	return _LS(IDS_WG_CMD__ADDD__Error);
}

void CCMMvldItemKRLRFD11Dlg::Data2Dlg()
{
	m_edtLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_edtDesc.SetWindowText(m_Data.Description);

	m_edtSF1.SetEditUnit(m_Data.dScaleFactor[0]);
	m_edtSF2.SetEditUnit(m_Data.dScaleFactor[1]);
	m_edtSF3.SetEditUnit(m_Data.dScaleFactor[2]);
	m_edtSF4.SetEditUnit(m_Data.dScaleFactor[3]);
	m_edtSF5.SetEditUnit(m_Data.dScaleFactor[4]);

	m_nCombOpt = m_Data.nCombOption;

	MakeItemEx();

	UpdateData(FALSE);
}

BOOL CCMMvldItemKRLRFD11Dlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	m_edtLoadCase.GetWindowText(m_Data.LoadCaseName);
	if(m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_edtDesc.GetWindowText(m_Data.Description);

	m_Data.dScaleFactor[0] = m_edtSF1.GetEditValue();
	m_Data.dScaleFactor[1] = m_edtSF2.GetEditValue();
	m_Data.dScaleFactor[2] = m_edtSF3.GetEditValue();
	m_Data.dScaleFactor[3] = m_edtSF4.GetEditValue();
	m_Data.dScaleFactor[4] = m_edtSF5.GetEditValue();

	m_Data.nCombOption = m_nCombOpt;

	return TRUE;
}

BOOL CCMMvldItemKRLRFD11Dlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifyMvld(m_csOldName, m_Data);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddMvld(m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemKRLRFD11Dlg message handlers

BOOL CCMMvldItemKRLRFD11Dlg::OnInitDialog() 
{
 	CDialogMove::OnInitDialog();

	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0) m_bSurface = TRUE;
	else m_bSurface = FALSE;
	    
	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.dScaleFactor[0] = 1.0;
		m_Data.dScaleFactor[1] = 0.9;
		m_Data.dScaleFactor[2] = 0.8;
		m_Data.dScaleFactor[3] = 0.7;
		m_Data.dScaleFactor[4] = 0.65;
		m_Data.dScaleFactor[5] = 0.65;
	}

	SetHeaderTitle();
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldItemKRLRFD11Dlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvldItemKRLRFD11Dlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();	
}

void CCMMvldItemKRLRFD11Dlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CCMMvldSubItemDlg dlg(this);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvldItemKRLRFD11Dlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
 
	CCMMvldSubItemDlg dlg(this);
	dlg.SetParamData(m_Data.arSubLoadData[iItem], iItem);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvldItemKRLRFD11Dlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
	m_Data.arSubLoadData.RemoveAt(iItem);
	m_List.DeleteItem(iItem);

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMMvldItemKRLRFD11Dlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnModify();	
	*pResult = 0;
}