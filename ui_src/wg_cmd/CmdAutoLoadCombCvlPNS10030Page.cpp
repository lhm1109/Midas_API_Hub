// CmdAutoLoadCombCvlSP35133302011Page.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlPNS10030Page.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "LoadCombCtrl.h"


#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_DBDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlPNS10030Page dialog
CCmdAutoLoadCombCvlPNS10030Page::CCmdAutoLoadCombCvlPNS10030Page(CWnd* pParent /*=NULL*/)
    : CChildDialog(CCmdAutoLoadCombCvlPNS10030Page::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCmdAutoLoadCombCvlPNS10030Page)
    //}}AFX_DATA_INIT
    m_pDoc = CDBDoc::GetDocPoint();

    m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

    m_nConstLoad = 0;

    m_nPNS10030DMaxRdo=0;
    m_nPNS10030DwMaxRdo=0;
    m_nPNS10030PsMaxRdo=0;
    m_nPNS10030HsMaxRdo=0;
    m_bPNS10030BaseChk = TRUE;
    m_bPNS10030DAddChk = FALSE;
    m_bPNS10030DUniqChk = FALSE;
}

void CCmdAutoLoadCombCvlPNS10030Page::DoDataExchange(CDataExchange* pDX)
{
    CChildDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCmdAutoLoadCombCvlPNS10030Page)  
    //PN-85/S-10030 (2016-2-12 ADD)
    DDX_Check  (pDX, IDC_P_CHK,                m_bPNS10030BaseChk);
    DDX_Check  (pDX, IDC_PD_CHK,               m_bPNS10030DAddChk);
    DDX_Check  (pDX, IDC_PW_CHK,               m_bPNS10030DUniqChk);
    DDX_Radio  (pDX, IDC_D_MAX_RDO,            m_nPNS10030DMaxRdo);
    DDX_Radio  (pDX, IDC_DW_MAX_RDO,           m_nPNS10030DwMaxRdo);
    DDX_Radio  (pDX, IDC_PS_MAX_RDO,           m_nPNS10030PsMaxRdo);
    DDX_Radio  (pDX, IDC_HS_MAX_RDO,           m_nPNS10030HsMaxRdo);
    DDX_Control(pDX, IDC_D_MAX_EDT,            m_dPNS10030DMaxEdt);
    DDX_Control(pDX, IDC_DW_MAX_EDT,           m_dPNS10030DwMaxEdt);
    DDX_Control(pDX, IDC_PS_MAX_EDT,           m_dPNS10030PsMaxEdt);
    DDX_Control(pDX, IDC_HS_MAX_EDT,           m_dPNS10030HsMaxEdt);
    DDX_Control(pDX, IDC_D_MIN_EDT,            m_dPNS10030DMinEdt);
    DDX_Control(pDX, IDC_DW_MIN_EDT,           m_dPNS10030DwMinEdt);
    DDX_Control(pDX, IDC_PS_MIN_EDT,           m_dPNS10030PsMinEdt);
    DDX_Control(pDX, IDC_HS_MIN_EDT,           m_dPNS10030HsMinEdt);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlPNS10030Page, CChildDialog)
    //{{AFX_MSG_MAP(CCmdAutoLoadCombCvlPNS10030Page)
    //}}AFX_MSG_MAP
    ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

void CCmdAutoLoadCombCvlPNS10030Page::SetDefault()
{

    // Dead Weight
    double dDMax = 1.2, dDMin = 0.9;
    m_dPNS10030DMaxEdt.SetEditUnit(dDMax);
    m_dPNS10030DMinEdt.SetEditUnit(dDMin);

    // Weight of Non-Structural members
    double dDwMax = 1.5, dDwMin = 0.9;
    m_dPNS10030DwMaxEdt.SetEditUnit(dDwMax);
    m_dPNS10030DwMinEdt.SetEditUnit(dDwMin);

    // Prestress
    double dPsMax = 1.2, dPsMin = 0.85;
    m_dPNS10030PsMaxEdt.SetEditUnit(dPsMax);
    m_dPNS10030PsMinEdt.SetEditUnit(dPsMin);

    // Hydrostatic Pressure
    double dWpMax = 1.2, dWpMin = 0.85;
    m_dPNS10030HsMaxEdt.SetEditUnit(dWpMax);
    m_dPNS10030HsMinEdt.SetEditUnit(dWpMin);
}



void CCmdAutoLoadCombCvlPNS10030Page::GetPNS10030LoadFactor()
{
    UpdateData(TRUE);

    // Dead Weight
    double dDMax = m_dPNS10030DMaxEdt.GetEditValue();
    double dDMin = m_dPNS10030DMinEdt.GetEditValue();

    switch(m_nPNS10030DMaxRdo)
    {
    case 0: m_nPNS10030DNum= 1; m_dPNS10030DMax = dDMax; m_dPNS10030DMin = dDMax; break; 
    case 1: m_nPNS10030DNum= 1; m_dPNS10030DMax = dDMin; m_dPNS10030DMin = dDMin; break;
    case 2: m_nPNS10030DNum= 2; m_dPNS10030DMax = dDMax; m_dPNS10030DMin = dDMin; break;
    }
    // Weight of Non-Structural Members
    double dDwMax = m_dPNS10030DwMaxEdt.GetEditValue();
    double dDwMin = m_dPNS10030DwMinEdt.GetEditValue();

    switch(m_nPNS10030DwMaxRdo)
    {
    case 0: m_nPNS10030DwNum= 1; m_nPNS10030DcNum = 1; m_dPNS10030DwMax = dDwMax; m_dPNS10030DwMin = dDwMax; m_dPNS10030DcMax = dDwMax; m_dPNS10030DcMin = dDwMax;break; 
    case 1: m_nPNS10030DwNum= 1; m_nPNS10030DcNum = 1; m_dPNS10030DwMax = dDwMin; m_dPNS10030DwMin = dDwMin; m_dPNS10030DcMax = dDwMin; m_dPNS10030DcMin = dDwMin;break; 
    case 2: m_nPNS10030DwNum= 2; m_nPNS10030DcNum = 2; m_dPNS10030DwMax = dDwMax; m_dPNS10030DwMin = dDwMin; m_dPNS10030DcMax = dDwMax; m_dPNS10030DcMin = dDwMin;break; 
    }

    // Prestress
    double dPsMax = m_dPNS10030PsMaxEdt.GetEditValue();
    double dPsMin = m_dPNS10030PsMinEdt.GetEditValue();

    switch(m_nPNS10030PsMaxRdo)
    {
    case 0: m_nPNS10030PsNum= 1; m_dPNS10030PsMax = dPsMax; m_dPNS10030PsMin = dPsMax; break; 
    case 1: m_nPNS10030PsNum= 1; m_dPNS10030PsMax = dPsMin; m_dPNS10030PsMin = dPsMin; break;
    case 2: m_nPNS10030PsNum= 2; m_dPNS10030PsMax = dPsMax; m_dPNS10030PsMin = dPsMin; break;
    }

    // Dead Weight
    double dWpMax = m_dPNS10030HsMaxEdt.GetEditValue();
    double dWpMin = m_dPNS10030HsMinEdt.GetEditValue();

    switch(m_nPNS10030HsMaxRdo)
    {
    case 0: m_nPNS10030WpNum= 1; m_dPNS10030WpMax = dWpMax; m_dPNS10030WpMin = dWpMax; break; 
    case 1: m_nPNS10030WpNum= 1; m_dPNS10030WpMax = dWpMin; m_dPNS10030WpMin = dWpMin; break;
    case 2: m_nPNS10030WpNum= 2; m_dPNS10030WpMax = dWpMax; m_dPNS10030WpMin = dWpMin; break;
    }
}



void CCmdAutoLoadCombCvlPNS10030Page::OnCmdPNS10030EnableDisable()
{  
    ////PN-85/S-10030
    //m_aCtrlPNS10030.Add(IDC_DIST_TYPE_STC);
    //m_aCtrlPNS10030.Add(IDC_P_CHK);
    //m_aCtrlPNS10030.Add(IDC_PD_CHK);
    //m_aCtrlPNS10030.Add(IDC_PW_CHK);
    //m_aCtrlPNS10030.Add(IDC_LTYPE_TXT);
    //m_aCtrlPNS10030.Add(IDC_FACTOR_TXT);
    //m_aCtrlPNS10030.Add(IDC_MAX_STC);
    //m_aCtrlPNS10030.Add(IDC_MIN_STC);
    //m_aCtrlPNS10030.Add(IDC_BOTH_STC);
    //m_aCtrlPNS10030.Add(IDC_D_STC);
    //m_aCtrlPNS10030.Add(IDC_DW_STC);
    //m_aCtrlPNS10030.Add(IDC_PS_STC);
    //m_aCtrlPNS10030.Add(IDC_HS_STC);

    GetDlgItem(IDC_D_MAX_RDO)  ->EnableWindow(TRUE);
    GetDlgItem(IDC_D_MIN_RDO)  ->EnableWindow(TRUE);   
    GetDlgItem(IDC_D_BOTH_RDO) ->EnableWindow(TRUE);  
    GetDlgItem(IDC_DW_MAX_RDO) ->EnableWindow(TRUE);  
    GetDlgItem(IDC_DW_MIN_RDO) ->EnableWindow(TRUE); 
    GetDlgItem(IDC_DW_BOTH_RDO)->EnableWindow(TRUE); 
    GetDlgItem(IDC_PS_MAX_RDO) ->EnableWindow(TRUE); 
    GetDlgItem(IDC_PS_MIN_RDO) ->EnableWindow(TRUE); 
    GetDlgItem(IDC_PS_BOTH_RDO)->EnableWindow(TRUE); 
    GetDlgItem(IDC_HS_MAX_RDO) ->EnableWindow(TRUE); 
    GetDlgItem(IDC_HS_MIN_RDO) ->EnableWindow(TRUE); 
    GetDlgItem(IDC_HS_BOTH_RDO)->EnableWindow(TRUE); 
    GetDlgItem(IDC_D_MAX_EDT)  ->EnableWindow(TRUE); 
    GetDlgItem(IDC_D_MIN_EDT)  ->EnableWindow(TRUE); 
    GetDlgItem(IDC_DW_MAX_EDT) ->EnableWindow(TRUE); 
    GetDlgItem(IDC_DW_MIN_EDT) ->EnableWindow(TRUE); 
    GetDlgItem(IDC_PS_MAX_EDT) ->EnableWindow(TRUE); 
    GetDlgItem(IDC_PS_MIN_EDT) ->EnableWindow(TRUE); 
    GetDlgItem(IDC_HS_MAX_EDT) ->EnableWindow(TRUE); 
    GetDlgItem(IDC_HS_MIN_EDT) ->EnableWindow(TRUE); 

}

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlPNS10030Page message handlers
BOOL CCmdAutoLoadCombCvlPNS10030Page::OnInitDialog() 
{
    CChildDialog::OnInitDialog();

    m_chkLoadFactor.InitControl(this, CCmdAutoLoadCombCvlPNS10030Page::IDD, IDC_CMD_SP35_13330_2011_LOAD_FACTOR_GRP,TRUE,TRUE);

    SetDefault();

    UpdateData(FALSE);
    OnCmdPNS10030EnableDisable();   // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화

    return TRUE;
}

LRESULT CCmdAutoLoadCombCvlPNS10030Page::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
    m_pParent->AlignControl();

    return 0L;
}
