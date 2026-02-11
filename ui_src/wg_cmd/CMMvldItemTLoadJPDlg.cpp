// CMMvldItemTLoadJPDlg.cpp : implementation file
// 2002. 7. 19   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemTLoadJPDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemTLoadJPDlg dialog


CCMMvldItemTLoadJPDlg::CCMMvldItemTLoadJPDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMMvldItemTLoadJPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemTLoadJPDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();
}


void CCMMvldItemTLoadJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemTLoadJPDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvldItemTLoadJPDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCMMvldItemTLoadJPDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemTLoadJPDlg message handlers


/////////////////////////////////////////////////////////////////////////////
//
// External Data Control Function
//

BOOL CCMMvldItemTLoadJPDlg::GetCurDlgData(T_MVLDjp_D* pData)
{
	ASSERT(pData);

	return TRUE;
}

void CCMMvldItemTLoadJPDlg::SetCurDlgData(T_MVLDjp_D* pData)
{
	ASSERT(pData);
	m_pDoc->m_pAttrCtrl->InitializeMvldjp(m_Data);
	m_Data = (*pData);
}

/////////////////////////////////////////////////////////////////////////////
//
// Initialize
//
void CCMMvldItemTLoadJPDlg::SetData2Dlg()
{
	//m_Data;

	UpdateData(FALSE);
}

BOOL CCMMvldItemTLoadJPDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetData2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
//
// Destroy
//
BOOL CCMMvldItemTLoadJPDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_Data.Initialize();
	
	return CChildDialog::DestroyWindow();
}
