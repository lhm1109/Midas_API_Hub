// BndrIelcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrIelcDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrIelcDlg dialog


CBndrIelcDlg::CBndrIelcDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrIelcDlg::IDD, pParent)
{
	m_pDoc = 0;
	//{{AFX_DATA_INIT(CBndrIelcDlg)
	//}}AFX_DATA_INIT
	m_aCtrlOp.Add(IDC_TM_OPTION_ADD);
	m_aCtrlOp.Add(IDC_TM_OPTION_DELETE);
}


void CBndrIelcDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrIelcDlg)
	DDX_Control(pDX, IDC_TM_IELC_LC_CMB,       m_LoadCase);
	//}}AFX_DATA_MAP
}


//--------------------------------------------------------------------------
// Implementations
BEGIN_MESSAGE_MAP(CBndrIelcDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrIelcDlg)
	ON_BN_CLICKED(IDC_TM_IELC_LC_BTN, OnTmIelcBtnLc)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrIelcDlg message handlers

BOOL CBndrIelcDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_LoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOp, 0);
	
	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrIelcDlg::OnTmIelcBtnLc() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CBndrIelcDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}
void CBndrIelcDlg::Execute()
{
	// TODO: Add your control notification handler code here
	int nOp = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOp, nOp);

	CArray<T_ELEM_K, T_ELEM_K> aKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyList(aKey);

	if (aKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
		return;
	}

	unsigned int nLoadCaseType, nLoadCaseKey;
	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	BOOL bSuccess = FALSE;
	T_IELC_D IelcData; IelcData.bIgnore = TRUE;

	if (nOp == 0)       // Add
		bSuccess = m_pDoc->m_pDataCtrl->AddIelc(aKey, nLoadCaseKey, IelcData);
	else if (nOp == 1)  // Delete
		bSuccess = m_pDoc->m_pDataCtrl->DelIelc(aKey, nLoadCaseKey);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}
void CBndrIelcDlg::OnTmExecute() 
{
	Execute();
}
