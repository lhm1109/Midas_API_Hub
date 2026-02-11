// CMMvctCHCablPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctCHCablPage.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvctCHCablPage dialog

CCMMvctCHCablPage::CCMMvctCHCablPage(T_FREQ_CABL* pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMMvctCHCablPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctCHCablPage)
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;

	m_bImpactFactor = TRUE;
}

void CCMMvctCHCablPage::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctCHCablPage)
	DDX_Control(pDX, IDC_CMD_MVCT_A_EDT,  m_edta);
	DDX_Control(pDX, IDC_CMD_MVCT_L_EDT,  m_edtL);
	DDX_Control(pDX, IDC_CMD_MVCT_BMP,    m_ImgViewer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctCHCablPage, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMMvctCHCablPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvctCHCablPage::SetData2Dlg()
{
	m_edta.SetEditUnit(m_pData->da);
	m_edtL.SetEditUnit(m_pData->dL);
	
	UpdateData(FALSE);
}

BOOL CCMMvctCHCablPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->da = m_edta.GetEditValue();
	m_pData->dL = m_edtL.GetEditValue();
		
	return TRUE;
}

void CCMMvctCHCablPage::EnableDisableControls(BOOL bImpactFactor)
{
	m_bImpactFactor = bImpactFactor;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	CDlgUtil::GetCtrlAllIDbyDlg(this, aControls);
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bImpactFactor);
}

void CCMMvctCHCablPage::InitUnit()
{
	m_edta.SetUnitType(D_UNITSYS_NONE);
	m_edtL.SetUnitType(D_UNITSYS_NONE);
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHCablPage message handlers
BOOL CCMMvctCHCablPage::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	InitUnit();
	SetData2Dlg();

	CString strImgPath = _T("SVG\\illustration\\Dialog\\cmd_mvctch8.svg");

	m_ImgViewer.SetImage(strImgPath);

	EnableDisableControls(m_bImpactFactor);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
