// CDgnSeisReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisReportDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\BrdgSeisEvalResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisReportDlg dialog


CDgnSeisReportDlg::CDgnSeisReportDlg(CWnd* pParent /*=NULL*/)
    : CDialogMove(CDgnSeisReportDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDgnSeisReportDlg)
    for(int i=0; i<8; ++i)
    {
        m_bItems[i] = TRUE;
    }
    //}}AFX_DATA_INIT
}

void CDgnSeisReportDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogMove::DoDataExchange(pDX);	
    //{{AFX_DATA_MAP(CDgnSaveReportoExcelWithEMFDlg)

    DDX_Control(pDX, IDC_DGN_SEIS_REPORT_COVER       , m_BtnItems[0]);
    DDX_Control(pDX, IDC_DGN_SEIS_REPORT_SUMMARY     , m_BtnItems[1]);
    DDX_Control(pDX, IDC_DGN_SEIS_REPORT_INFO        , m_BtnItems[2]);
    DDX_Control(pDX, IDC_DGN_SEIS_REPORT_ANAL        , m_BtnItems[3]);
    DDX_Control(pDX, IDC_DGN_SEIS_REPORT_ELASTIC_EQ  , m_BtnItems[4]);
    DDX_Control(pDX, IDC_DGN_SEIS_REPORT_COMBINED    , m_BtnItems[5]);
    DDX_Control(pDX, IDC_DGN_SEIS_REPORT_PIER_EVAL   , m_BtnItems[6]);
    DDX_Control(pDX, IDC_DGN_SEIS_REPORT_BEARING_EVAL, m_BtnItems[7]);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnSeisReportDlg, CDialogMove)
    //{{AFX_MSG_MAP(CDgnSaveReportoExcelWithEMFDlg)
    ON_BN_CLICKED(IDC_DGN_SEIS_REPORT_COVER_BTN, OnPrintReportBtn)
    ON_BN_CLICKED(IDC_DGN_SEIS_REPORT_SEL_BTN,   OnSelectAllBtn  )
    ON_BN_CLICKED(IDC_DGN_SEIS_REPORT_UNSEL_BTN, OnUnselectAllBtn)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSaveReportoExcelWithEMFDlg message handlers
BOOL CDgnSeisReportDlg::OnInitDialog() 
{
    CDialogMove::OnInitDialog();

    for(int i=0; i<8; ++i)
    {
        m_BtnItems[i].SetCheck(m_bItems[i]);
    }
    m_bExistRes[0] = TRUE;
    m_bExistRes[0] = TRUE;

    CBrdgSeisEvalResult* pResult = CDBDoc::GetDocPoint()->m_pPostCtrl->GetBrdgSeisEvalResult(); if(!pResult) ASSERT(0);
    if(!pResult->ExistResult())
    {
        m_BtnItems[6].SetCheck(FALSE);
        m_BtnItems[6].EnableWindow(FALSE);
        m_bExistRes[0] = FALSE;
    }
    if(!pResult->ExistBearingResult())
    {
        m_BtnItems[7].SetCheck(FALSE);
        m_BtnItems[7].EnableWindow(FALSE);
        m_bExistRes[1] = FALSE;
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisReportDlg::OnPrintReportBtn() 
{
    // TODO: Add your control notification handler code here
    BOOL bPrint = FALSE;
    for(int i=0; i<8; ++i)
    {
        m_bItems[i] = m_BtnItems[i].GetCheck();
        if(m_bItems[i]) bPrint = TRUE;
    }

    if(!bPrint) CDialogMove::OnCancel();
    CDialogMove::OnOK();
}

void CDgnSeisReportDlg::OnSelectAllBtn() 
{
    // TODO: Add your control notification handler code here
    for(int i=0; i<8; ++i)
    {
        if(i==6 && m_bExistRes[0])       m_bItems[i] = TRUE;
        else if(i==7 && m_bExistRes[1])  m_bItems[i] = TRUE;
        else                             m_bItems[i] = TRUE;
        m_BtnItems[i].SetCheck(m_bItems[i]);
    }
}

void CDgnSeisReportDlg::OnUnselectAllBtn() 
{
    // TODO: Add your control notification handler code here
    for(int i=0; i<8; ++i)
    {
        m_bItems[i] = FALSE;
        m_BtnItems[i].SetCheck(m_bItems[i]);
    }
}