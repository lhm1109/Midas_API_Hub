// NtisIterationCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "NtisIterationCtrlDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNtisIterationCtrlDlg dialog


CNtisIterationCtrlDlg::CNtisIterationCtrlDlg(T_NTIS_BOUNDARY *pData, BOOL bUseBoundaryNL, CWnd* pParent /*=NULL*/)
	: CDialogMove(CNtisIterationCtrlDlg::IDD, pParent)
{
	ASSERT(pData);
	m_pData = pData;
	m_bUseBoundaryNL = bUseBoundaryNL;

	//{{AFX_DATA_INIT(CNtisIterationCtrlDlg)
	m_bConvergenceFail = FALSE;
	m_bDispNorm = FALSE;
	m_bForcNorm = FALSE;
	m_bEnrgNorm = FALSE;
	m_nRungeKutta = 0;
	//}}AFX_DATA_INIT
}


void CNtisIterationCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNtisIterationCtrlDlg)
	DDX_Check  (pDX, IDC_CMD_CONV_FAILURE_CHECK,    m_bConvergenceFail);
	DDX_Control(pDX, IDC_CMD_MAX_ITER_NUM_EDIT,     m_edtMaxIteration);
	DDX_Check  (pDX, IDC_CMD_NONLINEAR_CHK1,        m_bDispNorm);
	DDX_Check  (pDX, IDC_CMD_NONLINEAR_CHK2,        m_bForcNorm);
	DDX_Check  (pDX, IDC_CMD_NONLINEAR_CHK3,        m_bEnrgNorm);
	DDX_Control(pDX, IDC_CMD_EDIT_CONV_TOL10,       m_edtDispNorm);
	DDX_Control(pDX, IDC_CMD_EDIT_CONV_TOL11,       m_edtForcNorm);
	DDX_Control(pDX, IDC_CMD_EDIT_CONV_TOL12,       m_edtEnrgNorm);

	DDX_Control(pDX, IDC_CMD_STIFF_UPDATE_SCHEME_CBX, m_cbxStiffScheme);
	DDX_Control(pDX, IDC_CMD_NUM_OF_ITER_BEFORE_UPDATE_EDT, m_edtNumofIterStiff);
	DDX_Control(pDX, IDC_CMD_MAX_BISECT_LVL_EDT, m_edtMaxBisectLevel);
	DDX_Control(pDX, IDC_CMD_SMART_BISECTION_CHK, m_chkSmartBisection);
	DDX_Control(pDX, IDC_CMD_DIVERGENCE_THRESHOLD_EDT, m_edtDivThreshold);

	DDX_Control(pDX, IDC_CMD_USE_LINE_SEARCH_CHECK, m_chkEnableLineSearch);
	DDX_Control(pDX, IDC_LINE_SEARCH_OPT_CMB, m_cmbLineSearchOption);
	DDX_Control(pDX, IDC_CMD_USE_LINE_SEARCH_NUM_EDT,m_edtLineSearchNum);
	DDX_Control(pDX, IDC_CMD_MAX_LINE_SEARCH_ITER_EDT, m_edtMaxLineSearch);
	DDX_Control(pDX, IDC_CMD_LINE_SEARCH_TOL_EDT, m_edtLineSearchTol);

	DDX_Radio  (pDX, IDC_CMD_NTIS_ITER_FEHLBERG_RDO, m_nRungeKutta);
	DDX_Control(pDX, IDC_CMD_NTIS_ITER_BOUNDARY_NL_TOL_EDT, m_edtBoundaryNLTol);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNtisIterationCtrlDlg, CDialogMove)
	//{{AFX_MSG_MAP(CNtisIterationCtrlDlg)
	ON_BN_CLICKED(IDC_CMD_NONLINEAR_CHK1, OnCmdNonlinearChk1)
	ON_BN_CLICKED(IDC_CMD_NONLINEAR_CHK2, OnCmdNonlinearChk2)
	ON_BN_CLICKED(IDC_CMD_NONLINEAR_CHK3, OnCmdNonlinearChk3)
	ON_BN_CLICKED(IDC_CMD_USE_LINE_SEARCH_CHECK, OnUseLineSearch)
	ON_CBN_SELCHANGE(IDC_CMD_STIFF_UPDATE_SCHEME_CBX, OnCmdChangeSchemeCbx)
	ON_CBN_SELCHANGE(IDC_LINE_SEARCH_OPT_CMB, OnCmdChangeLineSearchOptCbx)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_MAX_ITER_SPIN, OnMaxIterSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNtisIterationCtrlDlg message handlers

BOOL CNtisIterationCtrlDlg::OnInitDialog() 
{
	ASSERT(m_pData);

	CDialogMove::OnInitDialog();
	SetStiffUpdateSchemeCbx();
	SetLineSearchOptCmb();

	Data2Dlg();

	GetDlgItem(IDC_CMD_NUM_OF_ITER_BEFORE_UPDATE_EDT)->EnableWindow(m_cbxStiffScheme.GetCurSel() == 0);

	// Set Control
	//-permit convergence failure
	BOOL bEnable = (m_pData->bPerformIteration);
	GetDlgItem(IDC_CMD_CONV_FAILURE_CHECK)->EnableWindow(bEnable);
	//-Minimum & Maximum Step Size;
// 	bEnable = (m_pData->nAnalMethod==3 && m_pData->nAnalType==2);
// 	GetDlgItem(IDC_CMD_MAX_SUBSTEP_STATIC)     ->ShowWindow( bEnable);
// 	GetDlgItem(IDC_CMD_MAX_SUBSTEP_EDT)        ->ShowWindow( bEnable);
// 	GetDlgItem(IDC_CMD_MIN_SUBSTEP_SIZE_STATIC)->ShowWindow(!bEnable);
// 	GetDlgItem(IDC_CMD_MIN_SUBSTEP_SIZE_EDIT)  ->ShowWindow(!bEnable);
// 	GetDlgItem(IDC_CMD_MIN_SUBSTEP_SIZE_UNIT)  ->ShowWindow(!bEnable);
	//-Energy Norm
	bEnable = (TRUE);
	GetDlgItem(IDC_CMD_NONLINEAR_CHK3) ->ShowWindow(bEnable);
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL12)->ShowWindow(bEnable);

	OnChkCriteria();
	OnUseLineSearch();

	CDlgUtil::CtrlShowHideByRect(this, IDC_CMD_NTIS_ITER_BOUNDARY_NL_GRP, m_bUseBoundaryNL, TRUE);

	if (!m_bUseBoundaryNL)
	{
		CRect rRef;
		CRect rToMove;
		int nDistY = 0;

		CArray<UINT, UINT> aControls;
		aControls.Add(IDOK);
		aControls.Add(IDC_CMD_CLOSE);

		GetDlgItem(IDC_CMD_NONLINEAR_ANAL_CONTROL_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDOK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(24);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
		
		GetDlgItem(IDOK)->GetWindowRect(rToMove);

		CRect r;
		GetWindowRect(r);
		r.bottom = rToMove.bottom + globalUtils.ScaleByDPI(8);

		MoveWindow(r);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNtisIterationCtrlDlg::OnOK() 
{
	Dlg2Data();
	
	CDialogMove::OnOK();
}

void CNtisIterationCtrlDlg::OnChkCriteria()
{
	UpdateData(TRUE);
	// convergence criteria
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL10)->EnableWindow(m_bDispNorm);
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL11)->EnableWindow(m_bForcNorm);
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL12)->EnableWindow(m_bEnrgNorm);
}

void CNtisIterationCtrlDlg::SetStiffUpdateSchemeCbx()
{
	m_cbxStiffScheme.ResetContent();
	m_cbxStiffScheme.AddString(_LS(IDS_CMD_NLCTRL_PARAM_CUSTOM));
	m_cbxStiffScheme.AddString(_LS(IDS_CMD_NLCTRL_PARAM_FULL_NEWTON_RAPSHON));
	m_cbxStiffScheme.AddString(_LS(IDS_CMD_NLCTRL_PARAM_INIT_STIFFNESS));
	m_cbxStiffScheme.SetCurSel(0);

	CString str;
	CSize sz;
	int dx = 0;
	CDC* pDC = m_cbxStiffScheme.GetDC();
	for (int i = 0; i < m_cbxStiffScheme.GetCount(); i++)
	{
		m_cbxStiffScheme.GetLBText(i, str);
		sz = pDC->GetTextExtent(str);
		if (sz.cx > dx)dx = sz.cx;
	}
	m_cbxStiffScheme.SetDroppedWidth(dx);
	m_cbxStiffScheme.ReleaseDC(pDC);
}

void CNtisIterationCtrlDlg::Data2Dlg()
{
	m_edtMaxIteration.SetRange(1,1000);
	m_edtMaxIteration.SetInteger(TRUE);
	
	m_edtDispNorm.SetUnitType(0);
	m_edtForcNorm.SetUnitType(0);
	m_edtEnrgNorm.SetUnitType(0);
	m_edtLineSearchNum.SetUnitType(0);
	m_edtBoundaryNLTol.SetUnitType(0);

	// Data2Dlg
	m_bConvergenceFail = m_pData->bPermitConvergence;
	m_edtMaxIteration.SetValue(m_pData->nMaxIterNum);
	m_bDispNorm = m_pData->bDispNorm;
	m_bForcNorm = m_pData->bForceNorm;
	m_bEnrgNorm = m_pData->bEnergyNorm;
	m_edtDispNorm.SetEditUnit(m_pData->dDispNorm);
	m_edtForcNorm.SetEditUnit(m_pData->dForceNorm);
	m_edtEnrgNorm.SetEditUnit(m_pData->dEnergyNorm);
	m_cbxStiffScheme.SetCurSel(m_pData->nStiffUpdateScheme);
	m_edtNumofIterStiff.SetEditUnit(m_pData->nIterStiffUpdate);
	m_edtMaxBisectLevel.SetEditUnit(m_pData->nMaxBisectLevel);
	m_chkSmartBisection.SetCheck(m_pData->bSmartBisection);
	m_edtDivThreshold.SetEditUnit(m_pData->nDiverThreshold);
	m_chkEnableLineSearch.SetCheck(m_pData->bEnableLineSearch);
	CDlgUtil::CobxSetCurSelItemData(m_cmbLineSearchOption, m_pData->nLineSearchOption);
	m_edtLineSearchNum.SetEditUnit(m_pData->nNumSearch);
	m_edtMaxLineSearch.SetEditUnit(m_pData->nMaxLineSearch);
	m_edtLineSearchTol.SetEditUnit(m_pData->dLineSearchTol);

	m_nRungeKutta = m_pData->nRungeKuttaMethod;
	m_edtBoundaryNLTol.SetEditUnit(m_pData->dRungeKuttaTolerance);

	UpdateData(FALSE);
}

void CNtisIterationCtrlDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->bPermitConvergence   = m_bConvergenceFail;
	m_pData->nMaxIterNum			 = m_edtMaxIteration.GetEditValue();
	m_pData->bDispNorm            = m_bDispNorm;
	m_pData->bForceNorm           = m_bForcNorm;
	m_pData->bEnergyNorm          = m_bEnrgNorm;
	
	m_pData->dDispNorm            = m_edtDispNorm.GetEditValue();
	m_pData->dForceNorm           = m_edtForcNorm.GetEditValue();
	m_pData->dEnergyNorm          = m_edtEnrgNorm.GetEditValue();

	m_pData->nStiffUpdateScheme	 = m_cbxStiffScheme.GetCurSel();
	m_pData->nIterStiffUpdate	 = (int)m_edtNumofIterStiff.GetEditValue();
	m_pData->nMaxBisectLevel		 = (int)m_edtMaxBisectLevel.GetEditValue();
	m_pData->bSmartBisection		 = m_chkSmartBisection.GetCheck();
	m_pData->nDiverThreshold		 = (int)m_edtDivThreshold.GetEditValue();

	m_pData->bEnableLineSearch = m_chkEnableLineSearch.GetCheck();
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbLineSearchOption, reinterpret_cast<DWORD&>(m_pData->nLineSearchOption));
	m_pData->nNumSearch = (int)m_edtLineSearchNum.GetEditValue();
	m_pData->nMaxLineSearch = (int)m_edtMaxLineSearch.GetEditValue();
	m_pData->dLineSearchTol = m_edtLineSearchTol.GetEditValue();

	m_pData->nRungeKuttaMethod = m_nRungeKutta;
	m_pData->dRungeKuttaTolerance = m_edtBoundaryNLTol.GetEditValue();
}

void CNtisIterationCtrlDlg::SetLineSearchOptCmb()
{
	const std::vector<std::pair<CString, DWORD>> vData = {
		{_T("Auto Line Search"), NTIS_BOUNDARY::Auto},
		{_T("User Defined"), NTIS_BOUNDARY::UserDefined}
	};
	CDlgUtil::InitComboBox(m_cmbLineSearchOption, vData);
}

void CNtisIterationCtrlDlg::EnableLineSearch(bool bEnable)
{
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_LINE_SEARCH_GRP, bEnable, TRUE);
	if (bEnable) OnCmdChangeLineSearchOptCbx();
}

void CNtisIterationCtrlDlg::OnCmdNonlinearChk1() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL10)->EnableWindow(m_bDispNorm);
}

void CNtisIterationCtrlDlg::OnCmdNonlinearChk2() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL11)->EnableWindow(m_bForcNorm);
}

void CNtisIterationCtrlDlg::OnCmdNonlinearChk3() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL12)->EnableWindow(m_bEnrgNorm);
}

void CNtisIterationCtrlDlg::OnUseLineSearch() 
{
	const bool bEnable = m_chkEnableLineSearch.GetCheck();
	EnableLineSearch(bEnable);
}

void CNtisIterationCtrlDlg::OnCmdChangeSchemeCbx()
{
	UpdateData(TRUE);
	if (m_cbxStiffScheme.GetCurSel() == 0) m_edtNumofIterStiff.SetEditUnit(5);
	if (m_cbxStiffScheme.GetCurSel() == 1) m_edtNumofIterStiff.SetEditUnit(1);
	if (m_cbxStiffScheme.GetCurSel() == 2) m_edtNumofIterStiff.SetEditUnit(51);
	GetDlgItem(IDC_CMD_NUM_OF_ITER_BEFORE_UPDATE_EDT)->EnableWindow(m_cbxStiffScheme.GetCurSel() == 0);

	UpdateData(FALSE);
}

void CNtisIterationCtrlDlg::OnCmdChangeLineSearchOptCbx()
{
    CArray<UINT, UINT> IDCtrls;
	IDCtrls.Add(IDC_CMD_USE_LINE_SEARCH_NUM_EDT);
	IDCtrls.Add(IDC_CMD_MAX_LINE_SEARCH_ITER_EDT);
	IDCtrls.Add(IDC_CMD_LINE_SEARCH_TOL_EDT);

	int nLineSearchOption;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbLineSearchOption, reinterpret_cast<DWORD&>(nLineSearchOption));

	BOOL bEnable = nLineSearchOption == NTIS_BOUNDARY::UserDefined;

	CDlgUtil::CtrlEnableDisable (this, IDCtrls, bEnable);
}

// void CNtisIterationCtrlDlg::OnMaxIterSpin(NMHDR* pNMHDR, LRESULT* pResult)
// {
// 	CDlgUtil::SetSpinValue(this, IDC_MAX_ITER_STC_EDT, 1, 1000, pNMHDR, pResult);
// }

void CNtisIterationCtrlDlg::OnCmdClose()
{
	CDialogMove::OnCancel();
}
