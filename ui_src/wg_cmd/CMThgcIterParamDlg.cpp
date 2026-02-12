// CMThgcIterParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThgcIterParamDlg.h"
#include "..\wg_base\DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThgcIterParamDlg dialog


CCMThgcIterParamDlg::CCMThgcIterParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMThgcIterParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMThgcIterParamDlg)
	//}}AFX_DATA_INIT
	m_aLineSearch.RemoveAll();
	m_aLineSearch.Add(IDC_CMD_THGC_USE_LINE_SEARCH_GRP);
	m_aLineSearch.Add(IDC_CMD_THGC_USE_LINE_SEARCH_NUM_STATIC);
	m_aLineSearch.Add(IDC_CMD_THGC_USE_LINE_SEARCH_NUM_EDT);
	m_aLineSearch.Add(IDC_CMD_THGC_USE_LINE_SEARCH_MAX_STATIC);
	m_aLineSearch.FreeExtra();

}

void CCMThgcIterParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThgcIterParamDlg)
	DDX_Control(pDX, IDC_PERMIT_CHK       , m_chkPermitFail);
	DDX_Control(pDX, IDC_SUBSTEP_EDT      , m_edtSubstep);
	DDX_Control(pDX, IDC_MAX_ITER_STC_EDT , m_edtMaxIter);
	DDX_Control(pDX, IDC_DISLP_NORM_CHK   , m_chkDispNorm);
	DDX_Control(pDX, IDC_FORCE_NORM_CHK   , m_chkForcNorm);
	DDX_Control(pDX, IDC_ENERGY_NORM_CHK  , m_chkEngyNorm);
	DDX_Control(pDX, IDC_DISLP_NORM_EDT   , m_edtDispNorm);
	DDX_Control(pDX, IDC_FORCE_NORM_EDT   , m_edtForcNorm);
	DDX_Control(pDX, IDC_ENERGY_NORM_EDT  , m_edtEngyNorm);    

	DDX_Control(pDX, IDC_CMD_THGC_USE_LINE_SEARCH_CHECK , m_chkUseLineSearch);
	DDX_Control(pDX, IDC_CMD_THGC_USE_LINE_SEARCH_NUM_EDT,m_edtLineSearchNum);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMThgcIterParamDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMThgcIterParamDlg)
	ON_BN_CLICKED(IDC_DISLP_NORM_CHK    , OnChkCriteria)
	ON_BN_CLICKED(IDC_FORCE_NORM_CHK    , OnChkCriteria)
	ON_BN_CLICKED(IDC_ENERGY_NORM_CHK   , OnChkCriteria)
	ON_BN_CLICKED(IDC_CMD_THGC_USE_LINE_SEARCH_CHECK   , OnUseLineSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThgcIterParamDlg message handlers

BOOL CCMThgcIterParamDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_edtMaxIter.SetRange(1, 1000);
	m_edtMaxIter.SetInteger(TRUE);

	Data2Dlg();
	OnChkCriteria();  
	OnUseLineSearch();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThgcIterParamDlg::Data2Dlg()
{
	m_chkPermitFail.SetCheck(m_ThgcD.bPermitFail);
	m_edtSubstep.SetEditUnit(m_ThgcD.nSubstep);
	m_edtMaxIter.SetValue(m_ThgcD.nMaxIterNum);
	m_chkDispNorm.SetCheck(m_ThgcD.bDisplNorm==TRUE ? 1 : 0);
	m_chkForcNorm.SetCheck(m_ThgcD.bForceNorm==TRUE ? 1 : 0);
	m_chkEngyNorm.SetCheck(m_ThgcD.bEnergyNorm==TRUE ? 1 : 0);
	m_edtDispNorm.SetEditUnit(m_ThgcD.dDisplNorm);
	m_edtForcNorm.SetEditUnit(m_ThgcD.dForceNorm);
	m_edtEngyNorm.SetEditUnit(m_ThgcD.dEnergyNorm);

	m_chkUseLineSearch.SetCheck(m_ThgcD.bUseLineSearch);
	m_edtLineSearchNum.SetEditUnit(m_ThgcD.nNumSearch);
}

void CCMThgcIterParamDlg::Dlg2Data()
{
	m_ThgcD.bPermitFail = m_chkPermitFail.GetCheck()==1? TRUE : FALSE;
	m_ThgcD.nSubstep = m_edtSubstep.GetEditValue();
	m_ThgcD.nMaxIterNum = m_edtMaxIter.GetEditValueInt();
	m_ThgcD.bDisplNorm  = m_chkDispNorm.GetCheck() == 1 ? TRUE : FALSE;
	m_ThgcD.bForceNorm  = m_chkForcNorm.GetCheck()==1 ? TRUE : FALSE;
	m_ThgcD.bEnergyNorm = m_chkEngyNorm.GetCheck()==1 ? TRUE : FALSE;
	m_ThgcD.dDisplNorm  = m_edtDispNorm.GetEditValue();
	m_ThgcD.dForceNorm  = m_edtForcNorm.GetEditValue();
	m_ThgcD.dEnergyNorm = m_edtEngyNorm.GetEditValue();

	m_ThgcD.bUseLineSearch = m_chkUseLineSearch.GetCheck()==1? TRUE : FALSE;
	m_ThgcD.nNumSearch     = m_edtLineSearchNum.GetEditValue();
}

void CCMThgcIterParamDlg::OnOK()
{
	Dlg2Data();

	CDialogMove::OnOK();
}

void CCMThgcIterParamDlg::OnChkCriteria()
{
	BOOL bEnableD = m_chkDispNorm.GetCheck() ? TRUE: FALSE;
	GetDlgItem(IDC_DISLP_NORM_EDT )->EnableWindow(bEnableD);
	BOOL bEnableF = m_chkForcNorm.GetCheck() ? TRUE: FALSE;
	GetDlgItem(IDC_FORCE_NORM_EDT )->EnableWindow(bEnableF);
	BOOL bEnableE = m_chkEngyNorm.GetCheck() ? TRUE: FALSE;
	GetDlgItem(IDC_ENERGY_NORM_EDT )->EnableWindow(bEnableE);

	if(!bEnableD && !bEnableF && ! bEnableE)
	{
		m_chkDispNorm.SetCheck(1);
		bEnableD = m_chkDispNorm.GetCheck() ? TRUE: FALSE;
		GetDlgItem(IDC_DISLP_NORM_EDT )->EnableWindow(bEnableD);
	}
}

void CCMThgcIterParamDlg::OnUseLineSearch()
{
//  UpdateData(TRUE);
	BOOL bEnable = m_chkUseLineSearch.GetCheck() ? TRUE: FALSE;
	CDlgUtil::CtrlEnableDisable(this, m_aLineSearch, bEnable);
}