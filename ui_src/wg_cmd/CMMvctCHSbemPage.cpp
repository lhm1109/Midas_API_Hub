// CMMvctCHSbemPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctCHSbemPage.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvctCHSbemPage dialog

CCMMvctCHSbemPage::CCMMvctCHSbemPage(T_FREQ_SBEM* pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMMvctCHSbemPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctCHSbemPage)
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;

	m_bImpactFactor = TRUE;
}

void CCMMvctCHSbemPage::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctCHSbemPage)
	DDX_Control(pDX, IDC_CMD_MVCT_L_EDT,    m_edtL);
	DDX_Control(pDX, IDC_CMD_MVCT_E_EDT,    m_edtE);
	DDX_Control(pDX, IDC_CMD_MVCT_LC_EDT,   m_edtIc);
	DDX_Control(pDX, IDC_CMD_MVCT_MC_EDT,   m_edtmc);
	DDX_Control(pDX, IDC_CMD_MVCT_BMP,      m_ImgViewer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctCHSbemPage, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMMvctCHSbemPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvctCHSbemPage::SetData2Dlg()
{
	m_edtL.SetEditUnit(m_pData->dL);
	m_edtE.SetEditUnit(m_pData->dE);
	m_edtIc.SetEditUnit(m_pData->dIc);
	m_edtmc.SetEditUnit(m_pData->dmc);
	
	UpdateData(FALSE);
}

BOOL CCMMvctCHSbemPage::SetDlg2Data()
{
	UpdateData(TRUE);
		
	m_pData->dL  = m_edtL.GetEditValue();
	m_pData->dE  = m_edtE.GetEditValue();
	m_pData->dIc = m_edtIc.GetEditValue();
	m_pData->dmc = m_edtmc.GetEditValue();
	
	return TRUE;
}

void CCMMvctCHSbemPage::EnableDisableControls(BOOL bImpactFactor)
{
	m_bImpactFactor = bImpactFactor;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	CDlgUtil::GetCtrlAllIDbyDlg(this, aControls);
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bImpactFactor);
}

void CCMMvctCHSbemPage::InitUnit()
{
	m_edtL.SetUnitType(D_UNITSYS_NONE);
	m_edtE.SetUnitType(D_UNITSYS_NONE);
	m_edtIc.SetUnitType(D_UNITSYS_NONE);
	m_edtmc.SetUnitType(D_UNITSYS_NONE);
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHSbemPage message handlers
BOOL CCMMvctCHSbemPage::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	InitUnit();
	SetData2Dlg();

	CString strImgPath = _T("SVG\\illustration\\Dialog\\cmd_mvctch1.svg");

	m_ImgViewer.SetImage(strImgPath);

	EnableDisableControls(m_bImpactFactor);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}