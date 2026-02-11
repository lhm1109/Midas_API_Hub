// CMHydrCoolElem.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMHydrCoolElem.h"
#include "CMHydrCoolElemItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMHydrCoolElem dialog


CCMHydrCoolElem::CCMHydrCoolElem(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMHydrCoolElem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMHydrCoolElem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMHydrCoolElem::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMHydrCoolElem)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMHydrCoolElem, CDialogMove)
	//{{AFX_MSG_MAP(CCMHydrCoolElem)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMHydrCoolElem message handlers

BOOL CCMHydrCoolElem::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMHydrCoolElem::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CCMHydrCoolElem::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CCMHydrCoolElemItem dlg;
	dlg.DoModal();
}
