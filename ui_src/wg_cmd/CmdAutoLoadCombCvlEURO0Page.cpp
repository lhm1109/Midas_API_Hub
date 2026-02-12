// CmdAutoLoadCombCvlEURO0Page.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlEURO0Page.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "LoadCombCtrl.h"


#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "CmdAutoLoadCombCvlEuroRoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlEURO0Page dialog
CCmdAutoLoadCombCvlEURO0Page::CCmdAutoLoadCombCvlEURO0Page(CWnd* pParent /*=NULL*/)
    : CChildDialog(CCmdAutoLoadCombCvlEURO0Page::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCmdAutoLoadCombCvlEURO0Page)
    //}}AFX_DATA_INIT
    m_pDoc = CDBDoc::GetDocPoint();

    m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

    m_nConstLoad = 0;

    m_nEuroPerm = 0;
    m_nEuroPres = 0;
    m_nEuroSettle = 0;

    m_nEC4Permanent = 0;
    m_nEC4Prestress = 0;
    m_nEC4Settlement = 0;
    m_nEC4LVarTraffType = 0;

    m_bEC4LVarAll = TRUE;
    m_bEC4LVarTraff = TRUE;
    m_bEC4LVarWind = TRUE;
    m_bEC4LVarTherm = TRUE;
    m_bEC4LVarSnow = TRUE;
    m_bEC4LVarConst = TRUE;

}

void CCmdAutoLoadCombCvlEURO0Page::DoDataExchange(CDataExchange* pDX)
{
    CChildDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCmdAutoLoadCombCvlEURO0Page)  
    DDX_Radio  (pDX, IDC_EC4_TRAFF_100_RDO,   m_nEuroPerm);
    DDX_Radio  (pDX, IDC_EC4_PRSTR_100_RDO,   m_nEuroPres);
    DDX_Radio  (pDX, IDC_EC4_SETTL_100_RDO,   m_nEuroSettle);
    DDX_Check  (pDX, IDC_EC4_LVARB_ALL_CHK,   m_bEC4LVarAll);
    DDX_Check  (pDX, IDC_EC4_LVARB_TRAFF_CHK, m_bEC4LVarTraff);
    DDX_Check  (pDX, IDC_EC4_LVARB_WIND_CHK,  m_bEC4LVarWind);
    DDX_Check  (pDX, IDC_EC4_LVARB_THERM_CHK, m_bEC4LVarTherm);
    DDX_Check  (pDX, IDC_EC4_LVARB_SNOW_CHK,  m_bEC4LVarSnow);
    DDX_Check  (pDX, IDC_EC4_LVARB_CONST_CHK, m_bEC4LVarConst);
    DDX_Radio  (pDX, IDC_EC4_LVARB_TRAFF_ROAD_RDO, m_nEC4LVarTraffType);

    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlEURO0Page, CChildDialog)
    //{{AFX_MSG_MAP(CCmdAutoLoadCombCvlEURO0Page)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_EC4_LVARB_ALL_CHK,          OnCmdEC4LVarbAllChk)
    ON_BN_CLICKED(IDC_EC4_LVARB_TRAFF_CHK,        OnCmdEC4LVarbTraffChk)
    ON_BN_CLICKED(IDC_EC4_LVARB_WIND_CHK,         OnCmdEC4LVarbWindChk)
    ON_BN_CLICKED(IDC_EC4_LVARB_THERM_CHK,        OnCmdEC4LVarbThermChk)
    ON_BN_CLICKED(IDC_EC4_LVARB_SNOW_CHK,         OnCmdEC4LVarbSnowChk)
    ON_BN_CLICKED(IDC_EC4_LVARB_CONST_CHK,        OnCmdEC4LVarbConstChk)
    ON_BN_CLICKED(IDC_EC4_LVARB_TRAFF_ROAD_RDO,   OnCmdEuroTrafficEnableDisable)
    ON_BN_CLICKED(IDC_EC4_LVARB_TRAFF_RAIL_RDO,   OnCmdEuroTrafficEnableDisable)
    ON_BN_CLICKED(IDC_EC4_LVARB_TRAFF_ROAD_BTN,   OnCmdEC4LVarbTraffRoadBtn)

    ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

void CCmdAutoLoadCombCvlEURO0Page::GetEuroLoadFactor()
{
    UpdateData(TRUE);

    // Permanent Load
    switch ( m_nEuroPerm )
    {
    case 0: m_nEC4Permanent = 0; break;
    case 1: m_nEC4Permanent = 1; break;
    case 2: m_nEC4Permanent = 2; break;
    }

    // Prestress
    switch ( m_nEuroPres )
    {
    case 0: m_nEC4Prestress = 0; break;
    case 1: m_nEC4Prestress = 1; break;
    case 2: m_nEC4Prestress = 2; break;
    }

    // Settlement
    switch ( m_nEuroSettle )
    {
    case 0: m_nEC4Settlement = 0; break;
    case 1: m_nEC4Settlement = 1; break;
    case 2: m_nEC4Settlement = 2; break;
    }

}

void CCmdAutoLoadCombCvlEURO0Page::OnCmdEURO0EnableDisable()
{  
    GetDlgItem(IDC_EC4_TRAFF_100_RDO)        ->EnableWindow(TRUE);
    GetDlgItem(IDC_EC4_TRAFF_135_RDO)        ->EnableWindow(TRUE);   
    GetDlgItem(IDC_EC4_TRAFF_BOTH_RDO)       ->EnableWindow(TRUE);  
    GetDlgItem(IDC_EC4_PRSTR_100_RDO)        ->EnableWindow(TRUE);  
    GetDlgItem(IDC_EC4_PRSTR_135_RDO)        ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_PRSTR_BOTH_RDO)       ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_SETTL_100_RDO)        ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_SETTL_135_RDO)        ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_SETTL_BOTH_RDO)       ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_LVARB_ALL_CHK)        ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_LVARB_TRAFF_CHK)      ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_LVARB_WIND_CHK)       ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_LVARB_THERM_CHK)      ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_LVARB_SNOW_CHK)       ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_LVARB_CONST_CHK)      ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_LVARB_TRAFF_ROAD_RDO) ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_LVARB_TRAFF_ROAD_BTN) ->EnableWindow(TRUE); 
    GetDlgItem(IDC_EC4_LVARB_TRAFF_RAIL_RDO) ->EnableWindow(TRUE); 
}

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlEURO0Page message handlers
BOOL CCmdAutoLoadCombCvlEURO0Page::OnInitDialog() 
{
    CChildDialog::OnInitDialog();

    UpdateData(FALSE);
    GetEuroLoadFactor();
    OnCmdEURO0EnableDisable();   // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화

    return TRUE;
}

LRESULT CCmdAutoLoadCombCvlEURO0Page::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
    m_pParent->AlignControl();

    return 0L;
}

void CCmdAutoLoadCombCvlEURO0Page::OnCmdEC4LVarbAllChk()
{
    UpdateData(TRUE);
    m_bEC4LVarTraff = TRUE;//m_bEC4LVarAll;
    m_bEC4LVarWind  = m_bEC4LVarAll;
    m_bEC4LVarTherm = m_bEC4LVarAll;
    m_bEC4LVarSnow  = m_bEC4LVarAll;
    m_bEC4LVarConst = m_bEC4LVarAll;
    UpdateData(FALSE);

    OnCmdEuroTrafficEnableDisable();
}

void CCmdAutoLoadCombCvlEURO0Page::OnCmdEC4LVarbTraffChk()
{
    UpdateData(TRUE);
    m_bEC4LVarAll = (m_bEC4LVarTraff && m_bEC4LVarWind && m_bEC4LVarTherm && m_bEC4LVarSnow && m_bEC4LVarConst);
    if(m_bEC4LVarAll)
    {
        UpdateData(FALSE);
        OnCmdEuroTrafficEnableDisable();
        return;
    }

    if(!m_bEC4LVarTraff && !m_bEC4LVarWind && !m_bEC4LVarTherm &&
        !m_bEC4LVarSnow  && !m_bEC4LVarConst)
    {
        m_bEC4LVarTraff = TRUE;
        UpdateData(FALSE);
    }
    OnCmdEuroTrafficEnableDisable();
}

void CCmdAutoLoadCombCvlEURO0Page::OnCmdEC4LVarbWindChk()
{
    UpdateData(TRUE);
    m_bEC4LVarAll = (m_bEC4LVarTraff && m_bEC4LVarWind && m_bEC4LVarTherm && m_bEC4LVarSnow && m_bEC4LVarConst);
    if(m_bEC4LVarAll)
    {
        UpdateData(FALSE);
        OnCmdEuroTrafficEnableDisable();
        return;
    }

    if(!m_bEC4LVarTraff && !m_bEC4LVarWind && !m_bEC4LVarTherm &&
        !m_bEC4LVarSnow  && !m_bEC4LVarConst)
    {
        m_bEC4LVarWind = TRUE;
        UpdateData(FALSE);
    }

    OnCmdEuroTrafficEnableDisable();
}

void CCmdAutoLoadCombCvlEURO0Page::OnCmdEC4LVarbThermChk()
{
    UpdateData(TRUE);
    m_bEC4LVarAll = (m_bEC4LVarTraff && m_bEC4LVarWind && m_bEC4LVarTherm && m_bEC4LVarSnow && m_bEC4LVarConst);
    if(m_bEC4LVarAll)
    {
        UpdateData(FALSE);
        OnCmdEuroTrafficEnableDisable();
        return;
    }

    if(!m_bEC4LVarTraff && !m_bEC4LVarWind && !m_bEC4LVarTherm &&
        !m_bEC4LVarSnow  && !m_bEC4LVarConst)
    {
        m_bEC4LVarTherm = TRUE;
        UpdateData(FALSE);
    }
    OnCmdEuroTrafficEnableDisable();
}

void CCmdAutoLoadCombCvlEURO0Page::OnCmdEC4LVarbSnowChk()
{
    UpdateData(TRUE);
    m_bEC4LVarAll = (m_bEC4LVarTraff && m_bEC4LVarWind && m_bEC4LVarTherm && m_bEC4LVarSnow && m_bEC4LVarConst);
    if(m_bEC4LVarAll)
    {
        UpdateData(FALSE);
        OnCmdEuroTrafficEnableDisable();
        return;
    }

    if(!m_bEC4LVarTraff && !m_bEC4LVarWind && !m_bEC4LVarTherm &&
        !m_bEC4LVarSnow  && !m_bEC4LVarConst)
    {
        m_bEC4LVarSnow = TRUE;
        UpdateData(FALSE);
    }
    OnCmdEuroTrafficEnableDisable();
}

void CCmdAutoLoadCombCvlEURO0Page::OnCmdEC4LVarbConstChk()
{
    UpdateData(TRUE);
    m_bEC4LVarAll = (m_bEC4LVarTraff && m_bEC4LVarWind && m_bEC4LVarTherm && m_bEC4LVarSnow && m_bEC4LVarConst);
    if(m_bEC4LVarAll)
    {
        UpdateData(FALSE);
        OnCmdEuroTrafficEnableDisable();
        return;
    }

    if(!m_bEC4LVarTraff && !m_bEC4LVarWind && !m_bEC4LVarTherm &&
        !m_bEC4LVarSnow  && !m_bEC4LVarConst)
    {
        m_bEC4LVarConst = TRUE;
        UpdateData(FALSE);
    }
    OnCmdEuroTrafficEnableDisable();
}

void CCmdAutoLoadCombCvlEURO0Page::OnCmdEC4LVarbTraffRoadBtn()
{
    CCmdAutoLoadCombCvlEuroRoadDlg dlg;

    dlg.m_aMvldChar.Copy(m_aMvldChar);
    dlg.m_aMvldFreq.Copy(m_aMvldFreq);

    if(dlg.DoModal() == IDOK)
    {
        m_aMvldChar.Copy(dlg.m_aMvldChar);
        m_aMvldFreq.Copy(dlg.m_aMvldFreq);
    }
}

void CCmdAutoLoadCombCvlEURO0Page::OnCmdEuroTrafficEnableDisable()
{
    UpdateData(TRUE);

    GetDlgItem(IDC_EC4_LVARB_TRAFF_ROAD_RDO)->EnableWindow(m_bEC4LVarTraff);
    GetDlgItem(IDC_EC4_LVARB_TRAFF_ROAD_BTN)->EnableWindow(m_bEC4LVarTraff);
    GetDlgItem(IDC_EC4_LVARB_TRAFF_RAIL_RDO)->EnableWindow(m_bEC4LVarTraff);

    GetDlgItem(IDC_EC4_LVARB_TRAFF_ROAD_BTN)->EnableWindow(m_nEC4LVarTraffType==0);
}
