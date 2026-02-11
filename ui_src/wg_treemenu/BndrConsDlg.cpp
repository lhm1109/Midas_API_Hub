// BndrConsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrConsDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"

#include "..\wg_cmd\ExportFunc.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrConsDlg dialog


CBndrConsDlg::CBndrConsDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrConsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrConsDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT
	m_FlagAll[0] = m_FlagAll[1] = FALSE;
	for (int i = 0; i < 7; i++)
		m_Flag[i] = FALSE;
}

void CBndrConsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrConsDlg)
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Radio(pDX, IDC_TM_CONS_OPT_ADD, m_nOption);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_TM_CONS_DALL, m_FlagAll[0]);
	DDX_Check(pDX, IDC_TM_CONS_RALL, m_FlagAll[1]);
	UINT nID[] = {
		IDC_TM_CONS_DX, IDC_TM_CONS_DY, IDC_TM_CONS_DZ,
		IDC_TM_CONS_RX, IDC_TM_CONS_RY, IDC_TM_CONS_RZ, IDC_TM_CONS_RW,
	};
	for (int i = 0; i < 7; i++)
		DDX_Check(pDX, nID[i], m_Flag[i]);
	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC1, m_wndPicture);
}

void CBndrConsDlg::Execute()
{
	UpdateData(TRUE);
	CArray<T_NODE_K, T_NODE_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedNodeKeyList(aSelKey);
	if (aSelKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		return;
	}
	T_CONS_D data;
	data.Initialize();

	T_BNGR_K  BngrK;

	m_wndGroupCombo.GetSelectedBngr(BngrK);
	data.GroupKey = BngrK;


	for (int i = 0; i < 7; i++)
	{
		data.Constraint[i] = (m_Flag[i]) ? '1' : '0';
	}

	BOOL bSuccess = FALSE;
	if (m_nOption == 0 || m_nOption == 1) // add or replace
		bSuccess = m_pDoc->m_pDataCtrl->AddCons(aSelKey, data, (m_nOption == 0) ? FALSE : TRUE);
	else if (m_nOption == 2)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelCons(aSelKey, BngrK);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

BEGIN_MESSAGE_MAP(CBndrConsDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrConsDlg)
	ON_BN_CLICKED(IDC_TM_CONS_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_CONS_DALL, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_CONS_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_CONS_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_CONS_DX, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_CONS_DY, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_CONS_DZ, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_CONS_RALL, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_CONS_RX, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_CONS_RY, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_CONS_RZ, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_CONS_RW, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrConsDlg message handlers

void CBndrConsDlg::SetModifyData(T_CONS_D data)
{
	for (int i=0; i < 7; i++)
	{
		if(data.Constraint[i] == '0') m_Flag[i] = 0;
		else m_Flag[i] = 1;
	}
	m_nOption = 1; 	// replace

	// Groupµµ Setting
	T_BNGR_D  bngrD;
	m_pDoc->m_pAttrCtrl->GetBngr(data.GroupKey, bngrD);
	
	for(int i=0; i<m_wndGroupCombo.GetCount(); i++)
	{
		CString str;
		m_wndGroupCombo.GetLBText(i, str);
		if(str.CompareNoCase(bngrD.GroupName)==0) { m_wndGroupCombo.SetCurSel(i); break; }
	}
	UpdateData(FALSE);
}

BOOL CBndrConsDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	
	SetControl_7thDOF();

	m_nOption = 0;
	UpdateData(FALSE);
	m_wndPicture.SetImage("SVG\\illustration\\Dialog\\tm_cons 1.svg");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void  CBndrConsDlg::SetControl_7thDOF()
{
	BOOL bHide = FALSE;
	if (!CSectDB::IsEnable7thDOF())
		bHide = TRUE;
	
	if(bHide)
	{
		GetDlgItem(IDC_TM_CONS_RW)->ShowWindow(SW_HIDE);

		CWnd  *pWnd = GetDlgItem(IDC_TM_CONS_RW);
		CRect rcWnd;
		pWnd->GetWindowRect(&rcWnd); 
		this->ScreenToClient(&rcWnd);
		int nDy = rcWnd.Height() * -1.5;
				
		MoveControls(nDy);
		ResizeControls(nDy);
	}
}

void CBndrConsDlg::MoveControls(int nDy)
{
// 	CRect Rect[2];
// 	CWnd *pWnd[2];
// 	pWnd[0] = GetDlgItem(IDC_TM_EXECUTE);
// 	pWnd[1] = GetDlgItem(IDC_TM_CLOSE);
// 	
// 	for(int i=0; i<2; i++)
// 	{
// 		pWnd[i]->GetWindowRect(Rect[i]); 
// 		Rect[i].top += nDy ; 
// 		ScreenToClient(Rect[i]);
// 		pWnd[i]->SetWindowPos(NULL, Rect[i].left, Rect[i].top, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
// 	}
}

void CBndrConsDlg::ResizeControls(int nDy)
{
	CRect Rect[3];
	CWnd *pWnd[3];
	
	pWnd[0] = GetDlgItem(IDC_TM_TYPE_GRB);
	pWnd[1] = GetDlgItem(IDC_TM_OPT_GRB);
	pWnd[2] = this;
	for(int i=0; i<3; i++)
	{
		pWnd[i]->GetWindowRect(Rect[i]);
		Rect[i].SetRect(Rect[i].left,Rect[i].top,Rect[i].right,Rect[i].bottom+nDy);
		ScreenToClient(Rect[i]); 
		pWnd[i]->SetWindowPos(NULL, Rect[i].left, Rect[i].top, Rect[i].Width(), Rect[i].Height(),SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	}
}

void CBndrConsDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	
}

void CBndrConsDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CBndrConsDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nOption == 2) // delete 
	{
		EnableValueEdit(FALSE);
		this->m_wndGroupCombo.ShowHideAll(TRUE);
	}
	else
	{
		EnableValueEdit(TRUE);		
		this->m_wndGroupCombo.ShowHideAll(FALSE);
	}
}

void CBndrConsDlg::OnToggleFlag() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();

	UpdateData(TRUE);

	int i;
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_CONS_DALL:
		for(int i=0; i < 3; i++) m_Flag[i] = m_FlagAll[0];
		break;
	case IDC_TM_CONS_DX: case IDC_TM_CONS_DY: case IDC_TM_CONS_DZ:
		m_FlagAll[0] = m_Flag[0] && m_Flag[1] && m_Flag[2];
		break;
	case IDC_TM_CONS_RALL:
		for (i = 3; i < 7; i++) m_Flag[i] = m_FlagAll[1];
		break;
	case IDC_TM_CONS_RX: case IDC_TM_CONS_RY: case IDC_TM_CONS_RZ: case IDC_TM_CONS_RW:
		m_FlagAll[1] = m_Flag[3] && m_Flag[4] && m_Flag[5] && m_Flag[6];
		break;
	}
	UpdateData(FALSE);
}

void CBndrConsDlg::EnableValueEdit(BOOL bEnable)
{
	UINT nID[] = {
		IDC_TM_CONS_DALL, IDC_TM_CONS_DX, IDC_TM_CONS_DY, IDC_TM_CONS_DZ,
		IDC_TM_CONS_RALL, IDC_TM_CONS_RX, IDC_TM_CONS_RY, IDC_TM_CONS_RZ, IDC_TM_CONS_RW,
	};

	CWnd* pWnd;
	for (int i= 0; i < 9; i++)
	{
		pWnd = GetDlgItem(nID[i]);
		pWnd->EnableWindow(bEnable);
	}
}

BOOL CBndrConsDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_CONS_K Key;
	T_CONS_D data;
	Key.keymap = key;
	m_pDoc->m_pAttrCtrl->GetCons(Key, data);

	SetModifyData(data);
	return TRUE;
}

BOOL CBndrConsDlg::ExternalInit_by_SelectLabel(UINT key)
{
	return ExternalInit(key);
}

void CBndrConsDlg::OnTmDefineGroupButton() 
{
	// TODO: Add your control notification handler code here
	CreateOrActivateDlg(m_pDoc,CCMBndrGrupDefDlg::IDD);
}
