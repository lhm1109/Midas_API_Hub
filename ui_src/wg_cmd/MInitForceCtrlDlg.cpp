// MInitForceCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MInitForceCtrlDlg.h"

#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMInitForceCtrlDlg dialog
CMInitForceCtrlDlg::CMInitForceCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CMInitForceCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMInitForceCtrlDlg)
	m_bConsiderGeomStiff = TRUE;
	//}}AFX_DATA_INIT
}


void CMInitForceCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMInitForceCtrlDlg)
	DDX_Check  (pDX, IDC_CMD_INIT_FORCE_CHK,           m_bAddInit);
	DDX_Control(pDX, IDC_CMD_INIT_FORCE_COMBO,         m_cmbInitLC);
	DDX_Check  (pDX, IDC_CMD_INIT_FORCE_CMB_CHK,       m_bComb);
	DDX_Control(pDX, IDC_CMD_INIT_FORCE_CMB_COMBO,     m_cmbCombLC);
	DDX_Control(pDX, IDC_CMD_INIT_FORCE_CMB_SCALE_EDT, m_edtCombF);
	DDX_Control(pDX, IDC_CMD_INIT_FORCE_CMB_LIST,      m_lstComb);
	DDX_Check  (pDX, IDC_CMD_CONSIDER_GEOM_STIFF_CHK,  m_bConsiderGeomStiff);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMInitForceCtrlDlg, CDialogMove)
	//{{AFX_MSG_MAP(CMInitForceCtrlDlg)
	ON_BN_CLICKED(IDC_CMD_INIT_FORCE_BTN,         OnCmdInitForceBtn)
	ON_BN_CLICKED(IDC_CMD_INIT_FORCE_CHK,         OnCmdInitForceChk)
	ON_BN_CLICKED(IDC_CMD_INIT_FORCE_CMB_BTN,     OnCmdInitForceBtn)
	ON_BN_CLICKED(IDC_CMD_INIT_FORCE_CMB_ADD_BTN, OnCmdInitAddBtn)
	ON_BN_CLICKED(IDC_CMD_INIT_FORCE_CMB_MOD_BTN, OnCmdInitModBtn)
	ON_BN_CLICKED(IDC_CMD_INIT_FORCE_CMB_DEL_BTN, OnCmdInitDelBtn)
	ON_BN_CLICKED(IDC_CMD_INIT_FORCE_CMB_CHK,     OnCmdInitCombChk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_INIT_FORCE_CMB_LIST, OnChangedCurrentItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMInitForceCtrlDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_CMD_PDEL_loadcase), _LS(IDS_CMD_PDEL_scale) };
	int nColWidth[] = {110, 60};
	int nColNum = 2;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstComb.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstComb.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i=0; i<nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstComb.InsertColumn(i,&lvcolumn);
		m_lstComb.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CMInitForceCtrlDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_lstComb.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if(iItem==-1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CMInitForceCtrlDlg::MakeItemEx()
{
	m_lstComb.DeleteAllItems();

	int nItemCount = m_Data.nNumEfctCase;
	if(nItemCount == 0) return;

	for(int nCount=0; nCount<nItemCount; nCount++)
	{
		m_lstComb.InsertItem(nCount, _T(""));
		SetItem(nCount, m_Data.EfctCase[nCount]);
	}

	m_lstComb.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CMInitForceCtrlDlg::DataToStr(int i, T_EFCT_BASE &data)
{
	CString value;
	if(i==0) 
	{
		T_STLD_K key = data.LoadCaseKey;
		T_STLD_D data;
		m_pDoc->m_pAttrCtrl->GetStld(key, data);
		value = data.LoadCaseName;
	}
	else if(i==1) value.Format(_T("%g"), data.dblFactor);
	return value;
}

void CMInitForceCtrlDlg::SetItem(int nIndex, T_EFCT_BASE &data)
{
	LVITEM lvitem;
	CString str;
	for(int i=0; i<2; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStr(i, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_lstComb.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
}

BOOL CMInitForceCtrlDlg::ValidItem(T_EFCT_BASE& data)
{
	if(data.LoadCaseKey <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
		return FALSE;
	}

	if(data.dblFactor <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_scale_factor_));
		return FALSE;
	}
		
	return TRUE;
}

BOOL CMInitForceCtrlDlg::Dlg2Item(T_EFCT_BASE &data)
{
	UINT nLoadCaseType;
	if(!m_cmbCombLC.GetSelectedLoad(nLoadCaseType, data.LoadCaseKey))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return FALSE;
	}
	CString csScaleFactor;
	m_edtCombF.GetWindowText(csScaleFactor);
	if(!CStrParser::GetFloatNumber(csScaleFactor, data.dblFactor)) return FALSE;

	return TRUE;
}

BOOL CMInitForceCtrlDlg::Item2Dlg(T_EFCT_BASE &data)
{
	m_cmbCombLC.ChangeSelect(D_LOADCASE_STATIC, data.LoadCaseKey);
	CString csScaleFactor;
	csScaleFactor.Format(_T("%g"), data.dblFactor);

	m_edtCombF.SetWindowText(csScaleFactor);

	return TRUE;
}

void CMInitForceCtrlDlg::ChangeData()
{
	int nIndex;
	T_EFCT_BASE data;
	if(!GetSelectedItem(nIndex))
	{
		data.Initialize();
		data.dblFactor = 1.;
	}
	else data = m_Data.EfctCase[nIndex];

	Item2Dlg(data);
}
/////////////////////////////////////////////////////////////////////////////
// CMInitForceCtrlDlg message handlers

BOOL CMInitForceCtrlDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_pDoc = CDBDoc::GetDocPoint();

	if(!m_pDoc->m_pAttrCtrl->GetEfct(m_Data)) m_Data.Initialize();

	m_bAddInit = m_Data.bSaveToLoadCase;
	m_cmbInitLC.SetLoadType(D_SELECTLOAD_STATIC, FALSE, TRUE);
	m_cmbInitLC.ChangeSelect(D_SELECTLOAD_STATIC, m_Data.LoadCaseKey);
	
	m_bComb = m_Data.bIniForceComb;
	m_bConsiderGeomStiff = m_Data.bConsiderGeomStiff;

	UpdateData(FALSE);

	EnableDisableCtrl();

	SetListCtrlHeader();
	MakeItemEx();

	ChangeData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMInitForceCtrlDlg::OnCmdInitForceBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0), 0);
}

void CMInitForceCtrlDlg::OnOK() 
{
	UpdateData(TRUE);

	m_Data.bSaveToLoadCase = m_bAddInit;
	m_Data.bIniForceComb   = m_bComb;
	m_Data.bConsiderGeomStiff = m_bConsiderGeomStiff;

	UINT nLoadType;
	if(!m_cmbInitLC.GetSelectedLoad(nLoadType, m_Data.LoadCaseKey)) m_Data.LoadCaseKey = 0;
	if(!m_pDoc->m_pDataCtrl->AddEfct(m_Data)) return;

	for(int i=m_Data.nNumEfctCase; i<D_EFCT_NUMEFCT; i++) m_Data.EfctCase[i].Initialize();

	CDialogMove::OnOK();
}

void CMInitForceCtrlDlg::OnCmdInitForceChk() 
{
	UpdateData(TRUE);

	if(m_bAddInit && m_bComb)
	{
		m_bComb = FALSE;
		UpdateData(FALSE);
	}
	
	EnableDisableCtrl();
}

void CMInitForceCtrlDlg::OnCmdInitCombChk()
{
	UpdateData(TRUE);

	if(m_bAddInit && m_bComb)
	{
		m_bAddInit = FALSE;
		UpdateData(FALSE);
	}
	if(m_bComb && m_bConsiderGeomStiff)
	{
		m_bConsiderGeomStiff = FALSE;
		UpdateData(FALSE);
	}

	EnableDisableCtrl();
}

void CMInitForceCtrlDlg::EnableDisableCtrl()
{
	// Add Initial Force to Element Force
	GetDlgItem(IDC_CMD_INIT_FORCE_CHK)          ->EnableWindow(!m_bComb);
	GetDlgItem(IDC_CMD_INIT_FORCE_STC1)         ->EnableWindow(m_bAddInit);
	GetDlgItem(IDC_CMD_INIT_FORCE_COMBO)        ->EnableWindow(m_bAddInit);
	GetDlgItem(IDC_CMD_INIT_FORCE_BTN)          ->EnableWindow(m_bAddInit);

	// Initial Force Combination
	GetDlgItem(IDC_CMD_INIT_FORCE_CMB_CHK)      ->EnableWindow(!m_bAddInit);
	GetDlgItem(IDC_CMD_INIT_FORCE_CMB_STC1)     ->EnableWindow(m_bComb);
	GetDlgItem(IDC_CMD_INIT_FORCE_CMB_STC2)     ->EnableWindow(m_bComb);
	GetDlgItem(IDC_CMD_INIT_FORCE_CMB_COMBO)    ->EnableWindow(m_bComb);
	GetDlgItem(IDC_CMD_INIT_FORCE_CMB_BTN)      ->EnableWindow(m_bComb);
	GetDlgItem(IDC_CMD_INIT_FORCE_CMB_SCALE_EDT)->EnableWindow(m_bComb);
	GetDlgItem(IDC_CMD_INIT_FORCE_CMB_LIST)     ->EnableWindow(m_bComb);
	GetDlgItem(IDC_CMD_INIT_FORCE_CMB_ADD_BTN)  ->EnableWindow(m_bComb);
	GetDlgItem(IDC_CMD_INIT_FORCE_CMB_MOD_BTN)  ->EnableWindow(m_bComb);
	GetDlgItem(IDC_CMD_INIT_FORCE_CMB_DEL_BTN)  ->EnableWindow(m_bComb);

	GetDlgItem(IDC_CMD_CONSIDER_GEOM_STIFF_CHK) ->EnableWindow(!m_bComb);
}

void CMInitForceCtrlDlg::OnCmdInitAddBtn()
{
	T_EFCT_BASE data;
	
	if(m_Data.nNumEfctCase >= D_EFCT_NUMEFCT) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_add_loadcase_any_more_));
		return;
	}
	if(!Dlg2Item(data)) return;
	if(!ValidItem(data)) return;
	for(int i=0; i<m_Data.nNumEfctCase; i++)
	{
		if(m_Data.EfctCase[i].LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_cmbCombLC.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.EfctCase[m_Data.nNumEfctCase] = data;
	m_lstComb.InsertItem(m_Data.nNumEfctCase, _T(""));
	SetItem(m_Data.nNumEfctCase, data);
	m_lstComb.SetItemState(m_Data.nNumEfctCase++, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CMInitForceCtrlDlg::OnCmdInitModBtn()
{
	int nIndex;
	if(!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_EFCT_BASE data;
	if(!Dlg2Item(data)) return;
	if(!ValidItem(data)) return;
	for(int i=0; i<m_Data.nNumEfctCase; i++)
	{
		if(i == nIndex) continue;
		if(m_Data.EfctCase[i].LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_cmbCombLC.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.EfctCase[nIndex] = data;
	SetItem(nIndex, data);
	m_lstComb.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CMInitForceCtrlDlg::OnCmdInitDelBtn()
{
	int nIndex;
	if(!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	m_lstComb.DeleteItem(nIndex);
	for(int i=nIndex; i<m_Data.nNumEfctCase-1; i++) m_Data.EfctCase[i] = m_Data.EfctCase[i+1];
		
	m_Data.nNumEfctCase--;
	if(m_Data.nNumEfctCase > 0)
	{
		if(nIndex >= m_Data.nNumEfctCase) nIndex--;
		m_lstComb.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}	
}

void CMInitForceCtrlDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	ChangeData();

	*pResult = 0;
}