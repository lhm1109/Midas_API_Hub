// CmdAutoLoadCombCvlIRC6LSDPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlIRC6LSDPage.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "LoadCombCtrl.h"


#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "CmdAutoLoadCombCvlEuroRoadDlg.h"
#include "CmdAutoLoadCombCvlIRC_SPVHLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlIRC6LSDPage dialog
CCmdAutoLoadCombCvlIRC6LSDPage::CCmdAutoLoadCombCvlIRC6LSDPage(CWnd* pParent /*=NULL*/)
    : CChildDialog(CCmdAutoLoadCombCvlIRC6LSDPage::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCmdAutoLoadCombCvlIRC6LSDPage)
    //}}AFX_DATA_INIT
    m_pDoc = CDBDoc::GetDocPoint();

    m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

    m_nConstLoad = 0;

    m_nIRC6LVarTraffType = FALSE;
    m_bSpclVeh           = FALSE;
    m_bIRC6_IRC1122020   = FALSE;

    m_bIRC6_LSDLVarAll   = TRUE;
    m_bIRC6_LSDLVarTraff = TRUE;
    m_bIRC6_LSDLVarWind  = TRUE;
    m_bIRC6_LSDLVarTherm = TRUE;
    m_bIRC6_LSDLVarSnow  = TRUE;
    m_bIRC6_LSDLVarConst = TRUE;
    

}

void CCmdAutoLoadCombCvlIRC6LSDPage::DoDataExchange(CDataExchange* pDX)
{
    CChildDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCmdAutoLoadCombCvlIRC6LSDPage)  

    //IRC6LSD
    DDX_Check  (pDX, IDC_IRC6_LVARB_ALL_CHK2,        m_bIRC6_LSDLVarAll  );
    DDX_Check  (pDX, IDC_IRC6_LVARB_TRAFF_CHK2,      m_bIRC6_LSDLVarTraff);
    DDX_Check  (pDX, IDC_IRC6_LVARB_WIND_CHK2,       m_bIRC6_LSDLVarWind  );
    DDX_Check  (pDX, IDC_IRC6_LVARB_THERM_CHK2,      m_bIRC6_LSDLVarTherm  );
    DDX_Check  (pDX, IDC_IRC6_LVARB_SNOW_CHK2,       m_bIRC6_LSDLVarSnow   );
    DDX_Check  (pDX, IDC_IRC6_LVARB_CONST_CHK2,      m_bIRC6_LSDLVarConst  );
    DDX_Check  (pDX, IDC_IRC6_LVARB_TRAFF_ROAD_RDO2, m_nIRC6LVarTraffType);
    DDX_Check  (pDX, IDC_IRC6_LVARB_MVLOAD_SPV_CHK,  m_bSpclVeh);
    DDX_Check  (pDX, IDC_IRC6_LVARB_IRC2020_CHK,     m_bIRC6_IRC1122020);

    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlIRC6LSDPage, CChildDialog)
    //{{AFX_MSG_MAP(CCmdAutoLoadCombCvlIRC6LSDPage)
    //}}AFX_MSG_MAP

    //IRC6LSD
    ON_BN_CLICKED(IDC_IRC6_LVARB_ALL_CHK2,          OnCmdIRC6_LSDLVarbAllChk)
    ON_BN_CLICKED(IDC_IRC6_LVARB_TRAFF_CHK2,        OnCmdIRC6_LSDLVarbTraffChk)
    ON_BN_CLICKED(IDC_IRC6_LVARB_WIND_CHK2,         OnCmdIRC6_LSDLVarbWindChk)
    ON_BN_CLICKED(IDC_IRC6_LVARB_THERM_CHK2,        OnCmdIRC6_LSDLVarbThermChk)
    ON_BN_CLICKED(IDC_IRC6_LVARB_SNOW_CHK2,         OnCmdIRC6_LSDLVarbSnowChk)
    ON_BN_CLICKED(IDC_IRC6_LVARB_CONST_CHK2,        OnCmdIRC6_LSDLVarbConstChk)
    ON_BN_CLICKED(IDC_IRC6_LVARB_TRAFF_ROAD_RDO2,   OnCmdIRC6TrafficEnableDisable)
    ON_BN_CLICKED(IDC_IRC6_LVARB_TRAFF_ROAD_BTN2,   OnCmdIRC6_LSDLVarbTraffRoadBtn)
    ON_BN_CLICKED(IDC_IRC6_LVARB_MVLOAD_SPV_CHK,    OnCmdIRC6_LSDLVSpclVehChkBox)
    ON_BN_CLICKED(IDC_IRC6_LVARB_TRAFF_ROAD_BTN3,   OnCmdIRC6_LSDSpclVehBtn)
    ON_BN_CLICKED(IDC_IRC6_LVARB_IRC2020_CHK,       OnCmdIRC6_IRC1122020)


    ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()


void CCmdAutoLoadCombCvlIRC6LSDPage::OnCmdIRC6LSDEnableDisable()
{  
    GetDlgItem(IDC_IRC6_LVARB_ALL_CHK2)        ->EnableWindow(TRUE);
    GetDlgItem(IDC_IRC6_LVARB_TRAFF_CHK2)      ->EnableWindow(TRUE);   
    GetDlgItem(IDC_IRC6_LVARB_WIND_CHK2)       ->EnableWindow(TRUE);  
    GetDlgItem(IDC_IRC6_LVARB_THERM_CHK2)      ->EnableWindow(TRUE);  
    GetDlgItem(IDC_IRC6_LVARB_SNOW_CHK2)       ->EnableWindow(TRUE); 
    GetDlgItem(IDC_IRC6_LVARB_CONST_CHK2)      ->EnableWindow(TRUE); 
    GetDlgItem(IDC_IRC6_LVARB_TRAFF_GRP2)      ->EnableWindow(TRUE); 
    GetDlgItem(IDC_IRC6_LVARB_TRAFF_ROAD_RDO2) ->EnableWindow(TRUE); 
    
}

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlIRC6LSDPage message handlers
BOOL CCmdAutoLoadCombCvlIRC6LSDPage::OnInitDialog() 
{
    CChildDialog::OnInitDialog();

    UpdateData(FALSE);
    OnCmdIRC6LSDEnableDisable();   // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화
    OnCmdIRC6TrafficEnableDisable();
    return TRUE;
}

LRESULT CCmdAutoLoadCombCvlIRC6LSDPage::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
    m_pParent->AlignControl();

    return 0L;
}

//IRC6
void CCmdAutoLoadCombCvlIRC6LSDPage::OnCmdIRC6_LSDLVarbAllChk()
{
    UpdateData(TRUE);
    m_bIRC6_LSDLVarTraff = TRUE;//m_bIRC6_LSDLVarAll;
    m_bIRC6_LSDLVarWind  = m_bIRC6_LSDLVarAll;
    m_bIRC6_LSDLVarTherm = m_bIRC6_LSDLVarAll;
    m_bIRC6_LSDLVarSnow  = m_bIRC6_LSDLVarAll;
    m_bIRC6_LSDLVarConst = m_bIRC6_LSDLVarAll;
    UpdateData(FALSE);

    OnCmdIRC6TrafficEnableDisable();
}

void CCmdAutoLoadCombCvlIRC6LSDPage::OnCmdIRC6_LSDLVarbTraffChk()
{
    UpdateData(TRUE);
    m_bIRC6_LSDLVarAll = (m_bIRC6_LSDLVarTraff && m_bIRC6_LSDLVarWind && m_bIRC6_LSDLVarTherm && m_bIRC6_LSDLVarSnow && m_bIRC6_LSDLVarConst);
    if(m_bIRC6_LSDLVarAll)
    {
        UpdateData(FALSE);
        OnCmdIRC6TrafficEnableDisable();
        return;
    }

    if(!m_bIRC6_LSDLVarTraff && !m_bIRC6_LSDLVarWind && !m_bIRC6_LSDLVarTherm &&
        !m_bIRC6_LSDLVarSnow  && !m_bIRC6_LSDLVarConst)
    {
        m_bIRC6_LSDLVarTraff = TRUE;
        UpdateData(FALSE);
    }
    OnCmdIRC6TrafficEnableDisable();
}

void CCmdAutoLoadCombCvlIRC6LSDPage::OnCmdIRC6_LSDLVarbWindChk()
{
    UpdateData(TRUE);
    m_bIRC6_LSDLVarAll = (m_bIRC6_LSDLVarTraff && m_bIRC6_LSDLVarWind && m_bIRC6_LSDLVarTherm && m_bIRC6_LSDLVarSnow && m_bIRC6_LSDLVarConst);
    if(m_bIRC6_LSDLVarAll)
    {
        UpdateData(FALSE);
        OnCmdIRC6TrafficEnableDisable();
        return;
    }

    if(!m_bIRC6_LSDLVarTraff && !m_bIRC6_LSDLVarWind && !m_bIRC6_LSDLVarTherm &&
        !m_bIRC6_LSDLVarSnow  && !m_bIRC6_LSDLVarConst)
    {
        m_bIRC6_LSDLVarWind = TRUE;
        UpdateData(FALSE);
    }

    OnCmdIRC6TrafficEnableDisable();
}

void CCmdAutoLoadCombCvlIRC6LSDPage::OnCmdIRC6_LSDLVarbThermChk()
{
    UpdateData(TRUE);
    m_bIRC6_LSDLVarAll = (m_bIRC6_LSDLVarTraff && m_bIRC6_LSDLVarWind && m_bIRC6_LSDLVarTherm && m_bIRC6_LSDLVarSnow && m_bIRC6_LSDLVarConst);
    if(m_bIRC6_LSDLVarAll)
    {
        UpdateData(FALSE);
        OnCmdIRC6TrafficEnableDisable();
        return;
    }

    if(!m_bIRC6_LSDLVarTraff && !m_bIRC6_LSDLVarWind && !m_bIRC6_LSDLVarTherm &&
        !m_bIRC6_LSDLVarSnow  && !m_bIRC6_LSDLVarConst)
    {
        m_bIRC6_LSDLVarTherm = TRUE;
        UpdateData(FALSE);
    }
    OnCmdIRC6TrafficEnableDisable();
}

void CCmdAutoLoadCombCvlIRC6LSDPage::OnCmdIRC6_LSDLVarbSnowChk()
{
    UpdateData(TRUE);
    m_bIRC6_LSDLVarAll = (m_bIRC6_LSDLVarTraff && m_bIRC6_LSDLVarWind && m_bIRC6_LSDLVarTherm && m_bIRC6_LSDLVarSnow && m_bIRC6_LSDLVarConst);
    if(m_bIRC6_LSDLVarAll)
    {
        UpdateData(FALSE);
        OnCmdIRC6TrafficEnableDisable();
        return;
    }

    if(!m_bIRC6_LSDLVarTraff && !m_bIRC6_LSDLVarWind && !m_bIRC6_LSDLVarTherm &&
        !m_bIRC6_LSDLVarSnow  && !m_bIRC6_LSDLVarConst)
    {
        m_bIRC6_LSDLVarSnow = TRUE;
        UpdateData(FALSE);
    }
    OnCmdIRC6TrafficEnableDisable();
}

void CCmdAutoLoadCombCvlIRC6LSDPage::OnCmdIRC6_LSDLVarbConstChk()
{
    UpdateData(TRUE);
    m_bIRC6_LSDLVarAll = (m_bIRC6_LSDLVarTraff && m_bIRC6_LSDLVarWind && m_bIRC6_LSDLVarTherm && m_bIRC6_LSDLVarSnow && m_bIRC6_LSDLVarConst);
    if(m_bIRC6_LSDLVarAll)
    {
        UpdateData(FALSE);
        OnCmdIRC6TrafficEnableDisable();
        return;
    }

    if(!m_bIRC6_LSDLVarTraff && !m_bIRC6_LSDLVarWind && !m_bIRC6_LSDLVarTherm &&
        !m_bIRC6_LSDLVarSnow  && !m_bIRC6_LSDLVarConst)
    {
        m_bIRC6_LSDLVarConst = TRUE;
        UpdateData(FALSE);
    }
    OnCmdIRC6TrafficEnableDisable();
}

void CCmdAutoLoadCombCvlIRC6LSDPage::OnCmdIRC6_IRC1122020()
{
    UpdateData(TRUE);

    CButton *m_ctlCheck1 = (CButton*)GetDlgItem(IDC_IRC6_LVARB_IRC2020_CHK);
    m_bIRC6_IRC1122020 = (m_ctlCheck1->GetCheck() == 1) ? true : false;
    UpdateData(FALSE);
    OnCmdIRC6TrafficEnableDisable();
}

void CCmdAutoLoadCombCvlIRC6LSDPage::OnCmdIRC6_LSDLVarbTraffRoadBtn()
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

void CCmdAutoLoadCombCvlIRC6LSDPage::OnCmdIRC6TrafficEnableDisable()
{
    UpdateData(TRUE);

    GetDlgItem(IDC_IRC6_LVARB_TRAFF_ROAD_RDO2)->EnableWindow(m_bIRC6_LSDLVarTraff);
    GetDlgItem(IDC_IRC6_LVARB_TRAFF_ROAD_BTN2)->EnableWindow(m_bIRC6_LSDLVarTraff);

    GetDlgItem(IDC_IRC6_LVARB_TRAFF_ROAD_BTN2)->EnableWindow(m_nIRC6LVarTraffType);
    GetDlgItem(IDC_IRC6_LVARB_TRAFF_ROAD_BTN3)->EnableWindow(m_bSpclVeh);
}

void CCmdAutoLoadCombCvlIRC6LSDPage::OnCmdIRC6_LSDLVSpclVehChkBox()
{
   OnCmdIRC6TrafficEnableDisable();
}

void CCmdAutoLoadCombCvlIRC6LSDPage::OnCmdIRC6_LSDSpclVehBtn()
{
    CCmdAutoLoadCombCvlIRC_SPLVEHDlg dlg;

    //dlg.m_aMvldSPV.Copy(m_aMvldSPV);
    //dlg.m_aMvldSPVSel.Copy(m_aMvldSPVSel);

    if(dlg.DoModal() == IDOK)
    {
        m_aMvldSPV.Copy(dlg.m_aMvldSPV);
        m_aMvldSPVSel.Copy(dlg.m_aMvldSPVSel);
    }
}
//IRC6 END
