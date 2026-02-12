// ExtTypeLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ExtTypeLoadDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtTypeLoadDlg dialog


CExtTypeLoadDlg::CExtTypeLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CExtTypeLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtTypeLoadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	m_mAllExld.InitHashTable(97);
	m_mSelExld.InitHashTable(31);
}


void CExtTypeLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtTypeLoadDlg)
	DDX_Control(pDX, IDC_CMD_SELECTED_LIST, m_listSelProfiles);
	DDX_Control(pDX, IDC_CMD_ALL_LIST, m_listAllProfiles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtTypeLoadDlg, CDialogMove)
	//{{AFX_MSG_MAP(CExtTypeLoadDlg)
	ON_BN_CLICKED(IDC_CMD_REMOVE_DATA, OnCmdRemoveData)
	ON_BN_CLICKED(IDC_CMD_SEL_BUTTON, OnCmdSelButton)
	ON_BN_CLICKED(IDC_CMD_UNSEL_BUTTON, OnCmdUnselButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtTypeLoadDlg message handlers

BOOL CExtTypeLoadDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc->m_pAttrCtrl->GetStldKeyList4Exld(m_aAllExld);
	for (int i = 0; i < m_aAllExld.GetSize(); i++)
		m_mAllExld.SetAt(m_aAllExld[i], 0);

	// Data2Dlg
	if (!m_pDoc->m_pAttrCtrl->GetExld(m_ExldD)) 
	{
		m_ExldD.Initialize();
		GetDlgItem(IDC_CMD_REMOVE_DATA)->EnableWindow(FALSE);
	}
	Data2Dlg();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExtTypeLoadDlg::OnCmdRemoveData() 
{
	// TODO: Add your control notification handler code here
	if (!m_pDoc->m_pDataCtrl->DelExld()) return;
	GetDlgItem(IDC_CMD_REMOVE_DATA)->EnableWindow(FALSE);
}

void CExtTypeLoadDlg::OnCmdSelButton() 
{
	// TODO: Add your control notification handler code here
	BOOL bSel = FALSE;
	for (int i = m_listAllProfiles.GetCount()-1; i >= 0; i--)
	{
		if (m_listAllProfiles.GetSel(i) > 0)
		{
			m_mSelExld.SetAt(m_listAllProfiles.GetItemData(i), 0);
			m_listAllProfiles.DeleteString(i);
			bSel = TRUE;
		}
	}
	if (bSel) InitSelList();
}

void CExtTypeLoadDlg::OnCmdUnselButton() 
{
	// TODO: Add your control notification handler code here
	BOOL bSel = FALSE;
	for (int i = m_listSelProfiles.GetCount()-1; i >= 0; i--)
	{
		if (m_listSelProfiles.GetSel(i) > 0)
		{
			m_mSelExld.RemoveKey(m_listSelProfiles.GetItemData(i));
			m_listSelProfiles.DeleteString(i);
			bSel = TRUE;
		}
	}
	if (bSel) InitAllList();
}

void CExtTypeLoadDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	if (!m_pDoc->m_pDataCtrl->AddExld(m_ExldD)) return;
	CDialogMove::OnOK();
}

/////////////////////////////////////////////////////////////////////
// User Defined Functions
void CExtTypeLoadDlg::Data2Dlg()
{
	for (int i = 0; i < m_ExldD.nNumExldCase; i++)
	{
		if (m_mAllExld.Exist(m_ExldD.ExldCase[i]))
			m_mSelExld.SetAt(m_ExldD.ExldCase[i], 0);
	}
	InitAllList();
	InitSelList();
}

BOOL CExtTypeLoadDlg::Dlg2Data()
{
	m_ExldD.Initialize();
	int nCount = m_listSelProfiles.GetCount();

	m_ExldD.nNumExldCase = nCount;
	m_ExldD.ExldCase.SetSize(nCount);
	for (int i = 0; i < nCount; i++)
		m_ExldD.ExldCase[i] = m_listSelProfiles.GetItemData(i);
	return TRUE;
}

void CExtTypeLoadDlg::InitAllList()
{
	m_listAllProfiles.ResetContent();

	int ix;
	T_STLD_D data;
	for (int i=0; i<m_aAllExld.GetSize(); i++)
	{
		if (m_mSelExld.Exist(m_aAllExld[i])) continue;
		m_pDoc->m_pAttrCtrl->GetStld(m_aAllExld[i], data);
		ix = m_listAllProfiles.AddString(data.LoadCaseName);
		m_listAllProfiles.SetItemData(ix, m_aAllExld[i]);
	}
	if (m_listAllProfiles.GetCount() > 0)
		m_listAllProfiles.SetCurSel(0);
}

void CExtTypeLoadDlg::InitSelList()
{
	m_listSelProfiles.ResetContent();

	int ix;
	T_STLD_D data;

	for (int i=0; i<m_aAllExld.GetSize(); i++)
	{
		if (!m_mSelExld.Exist(m_aAllExld[i])) continue;
		m_pDoc->m_pAttrCtrl->GetStld(m_aAllExld[i], data);
		ix = m_listSelProfiles.AddString(data.LoadCaseName);
		m_listSelProfiles.SetItemData(ix, m_aAllExld[i]);
	}
	if (m_listSelProfiles.GetCount() > 0) m_listSelProfiles.SetCurSel(0);
}