// CMovingFactorBSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdMovingFactorBSDlg.h"

#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMovingFactorBSDlg dialog


CMovingFactorBSDlg::CMovingFactorBSDlg(CWnd* pParent /*=NULL*/)
    : CDialogMove(CMovingFactorBSDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMovingFactorBSDlg)
}

CMovingFactorBSDlg::~CMovingFactorBSDlg()
{
}


void CMovingFactorBSDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogMove::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMovingFactorBSDlg)
    //}}AFX_DATA_MAP
}

BOOL CMovingFactorBSDlg::OnInitDialog() 
{
    CDialogMove::OnInitDialog();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CMovingFactorBSDlg, CDialogMove)
    //{{AFX_MSG_MAP(CMovingFactorBSDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMovingFactorBSDlg message handlers

void CMovingFactorBSDlg::OnOK() 
{

    CDialogMove::OnOK();
}

void CMovingFactorBSDlg::OnCancel() 
{
    // TODO: Add extra cleanup here

    CDialogMove::OnCancel();
}
