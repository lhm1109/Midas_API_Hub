// CMovingFactorTMH7Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdMovingFactorTMH7Dlg.h"

#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMovingFactorTMH7Dlg dialog


CMovingFactorTMH7Dlg::CMovingFactorTMH7Dlg(CWnd* pParent /*=NULL*/)
    : CDialogMove(CMovingFactorTMH7Dlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMovingFactorTMH7Dlg)
}

CMovingFactorTMH7Dlg::~CMovingFactorTMH7Dlg()
{
}


void CMovingFactorTMH7Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogMove::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMovingFactorTMH7Dlg)
    //}}AFX_DATA_MAP
}

BOOL CMovingFactorTMH7Dlg::OnInitDialog() 
{
    CDialogMove::OnInitDialog();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CMovingFactorTMH7Dlg, CDialogMove)
    //{{AFX_MSG_MAP(CMovingFactorTMH7Dlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMovingFactorTMH7Dlg message handlers

void CMovingFactorTMH7Dlg::OnOK() 
{

    CDialogMove::OnOK();
}

void CMovingFactorTMH7Dlg::OnCancel() 
{
    // TODO: Add extra cleanup here

    CDialogMove::OnCancel();
}
