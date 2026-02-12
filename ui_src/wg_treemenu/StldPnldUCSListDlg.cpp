// StldPnldUCSListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldPnldUCSListDlg.h"
#include "StldPnldDlg.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CInternationalDlg
/////////////////////////////////////////////////////////////////////////////
// CStldPnldUCSListDlg dialog


CStldPnldUCSListDlg::CStldPnldUCSListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStldPnldUCSListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldPnldUCSListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bDestroyed	= FALSE;
	m_nMode				= -1;
	m_pParent			= NULL;

	ASSERT(pParent);
	m_pParent			= (CStldPnldDlg *)pParent;
}


void CStldPnldUCSListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldPnldUCSListDlg)
	DDX_Control(pDX, IDC_TM_UCS_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldPnldUCSListDlg, CDialog)
	//{{AFX_MSG_MAP(CStldPnldUCSListDlg)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_TM_UCS_LIST_SEL_BTN, OnTmUcsListSelBtn)
	ON_BN_CLICKED(IDC_TM_UCS_LIST_CANCEL_BTN, OnTmUcsListCancelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldPnldUCSListDlg message handlers

BOOL CStldPnldUCSListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowPos(NULL,m_nX,m_nY,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);   
	for(int i=0; i<this->arList.GetSize(); i++)this->m_list.AddString(arList[i]);
	this->m_list.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldPnldUCSListDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState==WA_INACTIVE) DestroyWindow();		
}

void CStldPnldUCSListDlg::OnTmUcsListSelBtn() 
{
	int sel;

	// error check 
	if(!this->arList.GetSize()) 
	{
		this->DestroyWindow();
		return;
	}
	if( (sel=m_list.GetCurSel())==LB_ERR )
	{
		return;
	}
	if(this->arList.GetSize()<=sel)
	{
		return;
	}

	// set selection list name
	ASSERT(m_pParent);
	if(this->m_nMode==0)			m_pParent->DestroyUCSDlg(arList[sel]);
	else if(this->m_nMode==1)	m_pParent->DestroyNPLNDlg(arList[sel]);
	
	this->DestroyWindow();
}

void CStldPnldUCSListDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	delete this;
}

void CStldPnldUCSListDlg::OnTmUcsListCancelBtn() 
{
	this->DestroyWindow();	
}



BOOL CStldPnldUCSListDlg::DestroyWindow() 
{
	 if (m_bDestroyed) return TRUE;
	m_bDestroyed = TRUE;
		
	return CDialog::DestroyWindow();
}
