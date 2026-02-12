// CMMvctCHSuspPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctCHSuspPage.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvctCHSuspPage dialog

CCMMvctCHSuspPage::CCMMvctCHSuspPage(T_FREQ_SUSP* pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMMvctCHSuspPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctCHSuspPage)
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;

	m_bImpactFactor = TRUE;
}

void CCMMvctCHSuspPage::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctCHSuspPage)
	DDX_Control(pDX, IDC_CMD_MVCT_L_EDT,    m_edtL);
	DDX_Control(pDX, IDC_CMD_MVCT_E_EDT,    m_edtE);
	DDX_Control(pDX, IDC_CMD_MVCT_I_EDT,    m_edtI);
	DDX_Control(pDX, IDC_CMD_MVCT_HG_EDT,   m_edtHg);
	DDX_Control(pDX, IDC_CMD_MVCT_M_EDT,    m_edtm);
	DDX_Control(pDX, IDC_CMD_MVCT_BMP,      m_ImgViewer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctCHSuspPage, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMMvctCHSuspPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvctCHSuspPage::SetData2Dlg()
{
	m_edtL .SetEditUnit(m_pData->dL);
	m_edtE .SetEditUnit(m_pData->dE);
	m_edtI .SetEditUnit(m_pData->dI);
	m_edtHg.SetEditUnit(m_pData->dHg);
	m_edtm .SetEditUnit(m_pData->dm);
	
	UpdateData(FALSE);
}

BOOL CCMMvctCHSuspPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->dL  = m_edtL .GetEditValue();
	m_pData->dE  = m_edtE .GetEditValue();
	m_pData->dI  = m_edtI .GetEditValue();
	m_pData->dHg = m_edtHg.GetEditValue();
	m_pData->dm  = m_edtm .GetEditValue();
		
	return TRUE;
}

void CCMMvctCHSuspPage::EnableDisableControls(BOOL bImpactFactor)
{
	m_bImpactFactor = bImpactFactor;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	CDlgUtil::GetCtrlAllIDbyDlg(this, aControls);
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bImpactFactor);
}

void CCMMvctCHSuspPage::InitUnit()
{
	m_edtL .SetUnitType(D_UNITSYS_NONE);
	m_edtE .SetUnitType(D_UNITSYS_NONE);
	m_edtI .SetUnitType(D_UNITSYS_NONE);
	m_edtHg.SetUnitType(D_UNITSYS_NONE);
	m_edtm .SetUnitType(D_UNITSYS_NONE);
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHSuspPage message handlers
BOOL CCMMvctCHSuspPage::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	InitUnit();
	SetData2Dlg();

	CString strImgPath = _T("SVG\\illustration\\Dialog\\cmd_mvctch9.svg");

	m_ImgViewer.SetImage(strImgPath);

	EnableDisableControls(m_bImpactFactor);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}