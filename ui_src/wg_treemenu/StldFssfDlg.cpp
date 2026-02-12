// StldFssfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldFssfDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_base\DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CMenuBarChildDlg
/////////////////////////////////////////////////////////////////////////////
// CStldFssfDlg dialog


CStldFssfDlg::CStldFssfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStldFssfDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldFssfDlg)
	m_nOption = 0;
	m_dUserFactor = 0;
}


void CStldFssfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldFssfDlg)
	DDX_Control(pDX, IDC_WG_TM_FSSF_CALC_COMBO, m_wndProtection);
	DDX_Text(pDX, IDC_WG_TM_FSSF_USER_EDIT, m_dUserFactor);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
}


BEGIN_MESSAGE_MAP(CStldFssfDlg, CDialog)
	//{{AFX_MSG_MAP(CStldFssfDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_WG_TM_FSSF_RADIO1, &CStldFssfDlg::IsAutoCalc)
	ON_BN_CLICKED(IDC_WG_TM_FSSF_RADIO2, &CStldFssfDlg::IsAutoCalc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldFssfDlg message handlers
BOOL CStldFssfDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);
	
	CString strProtectType[] = {_LS(IDS_WG_TREEMENU_Fire_FSSF_TYPE1),_LS(IDS_WG_TREEMENU_Fire_FSSF_TYPE2),_LS(IDS_WG_TREEMENU_Fire_FSSF_TYPE3),
		_LS(IDS_WG_TREEMENU_Fire_FSSF_TYPE4),_LS(IDS_WG_TREEMENU_Fire_FSSF_TYPE5),_LS(IDS_WG_TREEMENU_Fire_FSSF_TYPE6)};
	for(int i = 0;i < 6;i++)
		m_wndProtection.SetItemData(m_wndProtection.AddString(strProtectType[i]),i+1);
	m_wndProtection.SetCurSel(0);
	// Set Default Values
	m_nOption = 0;  // Default : Add

	((CButton*)GetDlgItem(IDC_WG_TM_FSSF_RADIO1))->SetCheck(TRUE);
	IsAutoCalc();

	UpdateData(FALSE);
	return TRUE;
}

void CStldFssfDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
		return;
	}
	int nProtectType = m_wndProtection.GetItemData(m_wndProtection.GetCurSel());
	T_FSSF_D data;
	data.bAutoCalc = m_bAutoCalc;
	if(m_bAutoCalc)
		data.nAutoType = nProtectType;
	else
		data.dFactor = m_dUserFactor;
	BOOL bSuccess;
	if (m_nOption == 0)
	{
		bSuccess = m_pDoc->m_pDataCtrl->AddFssf(aSelKey, data);
	}
	else
	{
		bSuccess = m_pDoc->m_pDataCtrl->DelFssf(aSelKey);
	}
	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CStldFssfDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldFssfDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_OPT_ADD: m_nOption = 0; break;
	case IDC_TM_OPT_DELETE: m_nOption = 1; break;
	}
}

void CStldFssfDlg::IsAutoCalc()
{
	m_bAutoCalc = ((CButton*)GetDlgItem(IDC_WG_TM_FSSF_RADIO1))->GetCheck();
	CArray<UINT,UINT> aCtrlID;
	aCtrlID.Add(IDC_WG_TM_FSSF_CALC_STATIC);
	aCtrlID.Add(IDC_WG_TM_FSSF_CALC_COMBO);
	CDlgUtil::CtrlEnableDisable(this,IDC_WG_TM_FSSF_USER_EDIT,!m_bAutoCalc);
	CDlgUtil::CtrlEnableDisable(this,aCtrlID,m_bAutoCalc);
}
