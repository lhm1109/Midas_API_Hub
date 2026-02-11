// CMMvctCHUserPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctCHUserPage.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvctCHUserPage dialog

CCMMvctCHUserPage::CCMMvctCHUserPage(T_FREQ_USER* pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMMvctCHUserPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctCHUserPage)
	//}}AFX_DATA_INIT
	
	m_pData = NULL;
	m_pData = pData;

	m_bImpactFactor = TRUE;
}

void CCMMvctCHUserPage::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctCHUserPage)
	DDX_Control(pDX, IDC_CMD_MVCT_USER_F_EDT,     m_edtf);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctCHUserPage, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMMvctCHUserPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvctCHUserPage::SetData2Dlg()
{
	m_edtf.SetEditUnit(m_pData->df);

	UpdateData(FALSE);
}

BOOL CCMMvctCHUserPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->df = m_edtf.GetEditValue();
		
	return TRUE;
}

void CCMMvctCHUserPage::EnableDisableControls(BOOL bImpactFactor)
{
	m_bImpactFactor = bImpactFactor;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	CDlgUtil::GetCtrlAllIDbyDlg(this, aControls);
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bImpactFactor);
}

void CCMMvctCHUserPage::InitUnit()
{
	m_edtf.SetUnitType(D_UNITSYS_NONE);
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHUserPage message handlers
BOOL CCMMvctCHUserPage::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	InitUnit();
	SetData2Dlg();

	EnableDisableControls(m_bImpactFactor);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}