// CmdAutoLoadCombCvlIRSPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlIRSPage.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "LoadCombCtrl.h"


#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "CmdAutoLoadCombCvlIRS_Drl_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlIRSPage dialog
CCmdAutoLoadCombCvlIRSPage::CCmdAutoLoadCombCvlIRSPage(CWnd* pParent /*=NULL*/)
    : CChildDialog(CCmdAutoLoadCombCvlIRSPage::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCmdAutoLoadCombCvlIRSPage)
    //}}AFX_DATA_INIT
    m_pDoc = CDBDoc::GetDocPoint();

    m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

    m_nConstLoad = 0;

    m_bIRSDrl     = TRUE;
    m_bIRSULSOPT  = FALSE;
    m_bChkCrp     = FALSE;

}

void CCmdAutoLoadCombCvlIRSPage::DoDataExchange(CDataExchange* pDX)
{
    CChildDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCmdAutoLoadCombCvlIRSPage)  

    //IRS
    DDX_Check  (pDX, IDC_IRS_CHK_DRL,          m_bIRSDrl);
    DDX_Check  (pDX, IDC_IRS_CHK_DRL2,         m_bIRSULSOPT);
    DDX_Check  (pDX, IDC_IRS_CHK_DRL2,         m_bChkCrp);
    DDX_Control(pDX, IDC_IRS_ULT_EDT,          m_dIRSULT);
    DDX_Control(pDX, IDC_IRS_EDT_SRVCE,        m_dIRSSRV);


    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlIRSPage, CChildDialog)
    //{{AFX_MSG_MAP(CCmdAutoLoadCombCvlIRSPage)
    //}}AFX_MSG_MAP

    ON_BN_CLICKED(IDC_IRS_CHK_DRL,    OnCmdIRSDrlEnableDisable)
    ON_BN_CLICKED(IDC_IRS_BTN_DRL_LC, OnBtnIRSDrlClicked)

    ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()


void CCmdAutoLoadCombCvlIRSPage::OnCmdIRSEnableDisable()
{  
    GetDlgItem(IDC_IRS_CHK_DRL2)    ->EnableWindow(TRUE);   
    GetDlgItem(IDC_IRS_ULT_EDT)     ->EnableWindow(TRUE);  
    GetDlgItem(IDC_IRS_EDT_SRVCE)   ->EnableWindow(TRUE); 
    GetDlgItem(IDC_IRS_CHK_DRL)     ->EnableWindow(TRUE); 
    GetDlgItem(IDC_IRS_BTN_DRL_LC)  ->EnableWindow(TRUE); 
}

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlIRSPage message handlers
BOOL CCmdAutoLoadCombCvlIRSPage::OnInitDialog() 
{
    CChildDialog::OnInitDialog();

    UpdateData(FALSE);

    OnCmdIRSSetDefault();

    OnCmdIRSEnableDisable();   // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화

    return TRUE;
}

LRESULT CCmdAutoLoadCombCvlIRSPage::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
    m_pParent->AlignControl();

    return 0L;
}


void CCmdAutoLoadCombCvlIRSPage::OnBtnIRSDrlClicked()
{

    CCmdAutoLoadCombCvlIRS_DRL_Dlg dlg;

    dlg.m_aMvldChar.Copy(m_aIRSUlt);
    dlg.m_aMvldFreq.Copy(m_aIRSServ);

    if(dlg.DoModal() == IDOK)
    {
        m_aIRSUlt.Copy(dlg.m_aMvldChar);
        m_aIRSServ.Copy(dlg.m_aMvldFreq);
    }
}


void CCmdAutoLoadCombCvlIRSPage::OnCmdIRSDrlEnableDisable()
{
    UpdateData(TRUE);
    GetDlgItem(IDC_IRS_BTN_DRL_LC)->EnableWindow(m_bIRSDrl);
}

void CCmdAutoLoadCombCvlIRSPage::OnCmdIRSSetDefault()
{
    // Dead Weight
    double dUltFact = 1.0, dSrvFact = 1.0;
    m_dIRSULT.SetEditUnit(dUltFact);
    m_dIRSSRV.SetEditUnit(dSrvFact);
}