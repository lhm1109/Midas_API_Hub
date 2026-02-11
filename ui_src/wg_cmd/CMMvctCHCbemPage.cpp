// CMMvctCHCbemPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctCHCbemPage.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvctCHCbemPage dialog

CCMMvctCHCbemPage::CCMMvctCHCbemPage(T_FREQ_CBEM* pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMMvctCHCbemPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctCHCbemPage)
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;

	m_bImpactFactor = TRUE;
}

void CCMMvctCHCbemPage::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctCHCbemPage)
	DDX_Control(pDX, IDC_CMD_MVCT_A_EDT,  m_edta);
	DDX_Control(pDX, IDC_CMD_MVCT_B_EDT,  m_edtb);
	DDX_Control(pDX, IDC_CMD_MVCT_L_EDT,  m_edtL);
	DDX_Control(pDX, IDC_CMD_MVCT_E_EDT,  m_edtE);
	DDX_Control(pDX, IDC_CMD_MVCT_LC_EDT,  m_edtIc);
	DDX_Control(pDX, IDC_CMD_MVCT_MC_EDT,  m_edtmc);
	DDX_Control(pDX, IDC_CMD_MVCT_POSI_BMP, m_ImgViewer_Posi);
	DDX_Control(pDX, IDC_CMD_MVCT_NEGA_BMP, m_ImgViewer_Nega);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctCHCbemPage, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMMvctCHCbemPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvctCHCbemPage::SetData2Dlg()
{
	m_edta.SetEditUnit(m_pData->da);
	m_edtb.SetEditUnit(m_pData->db);
	m_edtL.SetEditUnit(m_pData->dL);
	m_edtE.SetEditUnit(m_pData->dE);
	m_edtIc.SetEditUnit(m_pData->dIc);
	m_edtmc.SetEditUnit(m_pData->dmc);
		
	UpdateData(FALSE);
}

BOOL CCMMvctCHCbemPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->da = m_edta.GetEditValue();
	m_pData->db = m_edtb.GetEditValue();
	m_pData->dL = m_edtL.GetEditValue();
	m_pData->dE = m_edtE.GetEditValue();
	m_pData->dIc = m_edtIc.GetEditValue();
	m_pData->dmc = m_edtmc.GetEditValue();
	
	return TRUE;
}

void CCMMvctCHCbemPage::EnableDisableControls(BOOL bImpactFactor)
{
	m_bImpactFactor = bImpactFactor;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	CDlgUtil::GetCtrlAllIDbyDlg(this, aControls);
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bImpactFactor);
}

void CCMMvctCHCbemPage::InitUnit()
{
	m_edta.SetUnitType(D_UNITSYS_NONE);
	m_edtb.SetUnitType(D_UNITSYS_NONE);
	m_edtL.SetUnitType(D_UNITSYS_NONE);
	m_edtE.SetUnitType(D_UNITSYS_NONE);
	m_edtIc.SetUnitType(D_UNITSYS_NONE);
	m_edtmc.SetUnitType(D_UNITSYS_NONE);
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHCbemPage message handlers
BOOL CCMMvctCHCbemPage::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	InitUnit();
	SetData2Dlg();

	CString strImgPath1, strImgPath2;

	strImgPath1 = _T("SVG\\illustration\\Dialog\\cmd_mvct.svg");
	strImgPath2 = _T("SVG\\illustration\\Dialog\\cmd_mvctch4.svg");

	m_ImgViewer_Posi.SetImage(strImgPath1);
	m_ImgViewer_Nega.SetImage(strImgPath2);

	EnableDisableControls(m_bImpactFactor);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
