// ThisIterationCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ThisIterationCtrlDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThisIterationCtrlDlg dialog


CThisIterationCtrlDlg::CThisIterationCtrlDlg(T_THIS_D *pData, CWnd* pParent /*=NULL*/)
	: CDialogMove(CThisIterationCtrlDlg::IDD, pParent)
{
	ASSERT(pData);
	m_pData = pData;

	//{{AFX_DATA_INIT(CThisIterationCtrlDlg)
	m_bConvergenceFail = FALSE;
	m_bDispNorm = FALSE;
	m_bForcNorm = FALSE;
	m_bEnrgNorm = FALSE;
	m_bUseLineSearch = FALSE;
	m_nRungeKutta = 0;

	m_aLineSearch.RemoveAll();
	m_aLineSearch.Add(IDC_CMD_USE_LINE_SEARCH_NUM_STATIC);
	m_aLineSearch.Add(IDC_CMD_USE_LINE_SEARCH_NUM_EDT);
	m_aLineSearch.Add(IDC_CMD_USE_LINE_SEARCH_MAX_STATIC);
	m_aLineSearch.FreeExtra();

	//}}AFX_DATA_INIT
}


void CThisIterationCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThisIterationCtrlDlg)
	DDX_Check  (pDX, IDC_CMD_CONV_FAILURE_CHECK,    m_bConvergenceFail);
	DDX_Control(pDX, IDC_CMD_MIN_SUBSTEP_SIZE_EDIT, m_edtMinStepSize);
	DDX_Control(pDX, IDC_CMD_MAX_ITER_NUM_EDIT,     m_edtMaxIteration);
	DDX_Check  (pDX, IDC_CMD_NONLINEAR_CHK1,        m_bDispNorm);
	DDX_Check  (pDX, IDC_CMD_NONLINEAR_CHK2,        m_bForcNorm);
	DDX_Check  (pDX, IDC_CMD_NONLINEAR_CHK3,        m_bEnrgNorm);
	DDX_Control(pDX, IDC_CMD_EDIT_CONV_TOL10,       m_edtDispNorm);
	DDX_Control(pDX, IDC_CMD_EDIT_CONV_TOL11,       m_edtForcNorm);
	DDX_Control(pDX, IDC_CMD_EDIT_CONV_TOL12,       m_edtEnrgNorm);
	DDX_Control(pDX, IDC_CMD_MAX_SUBSTEP_EDT,       m_edtMaxStepSize);
	DDX_Radio  (pDX, IDC_CMD_FEHLBERG_RDO,          m_nRungeKutta);
	DDX_Control(pDX, IDC_CMD_TOLERANCE_EDT,         m_edtRungeKutaTolerance);
	DDX_Check  (pDX, IDC_CMD_USE_LINE_SEARCH_CHECK, m_bUseLineSearch);
	DDX_Control(pDX, IDC_CMD_USE_LINE_SEARCH_NUM_EDT,m_edtLineSearchNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThisIterationCtrlDlg, CDialogMove)
	//{{AFX_MSG_MAP(CThisIterationCtrlDlg)
	ON_BN_CLICKED(IDC_CMD_NONLINEAR_CHK1, OnCmdNonlinearChk1)
	ON_BN_CLICKED(IDC_CMD_NONLINEAR_CHK2, OnCmdNonlinearChk2)
	ON_BN_CLICKED(IDC_CMD_NONLINEAR_CHK3, OnCmdNonlinearChk3)
	ON_BN_CLICKED(IDC_CMD_USE_LINE_SEARCH_CHECK, OnUseLineSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThisIterationCtrlDlg message handlers

BOOL CThisIterationCtrlDlg::OnInitDialog() 
{
	ASSERT(m_pData);

	CDialogMove::OnInitDialog();
	
	m_edtMaxIteration.SetRange(1,1000);
	m_edtMaxIteration.SetInteger(TRUE);

	// Initialize Controls
	CArray<UINT, UINT> aCtrlID;
	aCtrlID.Add(IDC_CMD_MAX_SUBSTEP_STATIC);
	aCtrlID.Add(IDC_CMD_MAX_SUBSTEP_EDT);
	aCtrlID.FreeExtra();
	CRect rRef, rMove;
	GetDlgItem(IDC_CMD_MIN_SUBSTEP_SIZE_EDIT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MAX_SUBSTEP_EDT)->GetWindowRect(rMove);
	int nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, aCtrlID, nDistY);
	
	GetDlgItem(IDOK)->GetWindowRect(rRef);
	GetWindowRect(rMove);
	rMove.bottom = rRef.bottom+12;
	MoveWindow(rMove);

	m_edtMinStepSize.SetUnitType(0);
	m_edtDispNorm.SetUnitType(0);
	m_edtForcNorm.SetUnitType(0);
	m_edtEnrgNorm.SetUnitType(0);
	m_edtMaxStepSize.SetUnitType(0);
	m_edtRungeKutaTolerance.SetUnitType(0);
	m_edtLineSearchNum.SetUnitType(0);

	// Data2Dlg
	m_bConvergenceFail = (m_pData->nAnalMethod==1) ? FALSE : m_pData->BoundaryNL_Old.bPermitConvergence;
	m_bDispNorm        = m_pData->BoundaryNL_Old.bDispNorm;
	m_bForcNorm        = m_pData->BoundaryNL_Old.bForceNorm;
	m_bEnrgNorm        = m_pData->BoundaryNL_Old.bEnergyNorm;
	m_nRungeKutta      = m_pData->BoundaryNL_Old.nRungeKuttaMethod;
	m_edtMinStepSize.SetEditUnit(m_pData->BoundaryNL_Old.dMinSubStepSize);
	m_edtMaxIteration.SetValue(m_pData->BoundaryNL_Old.nMaxIterNum);
	m_edtDispNorm.SetEditUnit(m_pData->BoundaryNL_Old.dDispNorm);
	m_edtForcNorm.SetEditUnit(m_pData->BoundaryNL_Old.dForceNorm);
	m_edtEnrgNorm.SetEditUnit(m_pData->BoundaryNL_Old.dEnergyNorm);
	m_edtMaxStepSize.SetEditUnit(m_pData->BoundaryNL_Old.nMaxNumSubStep);
	m_edtRungeKutaTolerance.SetEditUnit(m_pData->BoundaryNL_Old.dRungeKuttaTolerance);

	m_bUseLineSearch = m_pData->BoundaryNL_Old.bUseLineSearch;
	m_edtLineSearchNum.SetEditUnit(m_pData->BoundaryNL_Old.nNumSearch);

	UpdateData(FALSE);

	// Set Control
	//-permit convergence failure
	BOOL bEnable = (m_pData->nAnalType==2 && m_pData->nAnalMethod!=1 && m_pData->BoundaryNL_Old.bPerformIteration);
	GetDlgItem(IDC_CMD_CONV_FAILURE_CHECK)->EnableWindow(bEnable);
	//-Minimum & Maximum Step Size;
	bEnable = (m_pData->nAnalMethod==3 && m_pData->nAnalType==2);
	GetDlgItem(IDC_CMD_MAX_SUBSTEP_STATIC)     ->ShowWindow( bEnable);
	GetDlgItem(IDC_CMD_MAX_SUBSTEP_EDT)        ->ShowWindow( bEnable);
	GetDlgItem(IDC_CMD_MIN_SUBSTEP_SIZE_STATIC)->ShowWindow(!bEnable);
	GetDlgItem(IDC_CMD_MIN_SUBSTEP_SIZE_EDIT)  ->ShowWindow(!bEnable);
	GetDlgItem(IDC_CMD_MIN_SUBSTEP_SIZE_UNIT)  ->ShowWindow(!bEnable);
	//-Energy Norm
	bEnable = (m_pData->nAnalMethod != 1);
	GetDlgItem(IDC_CMD_NONLINEAR_CHK3) ->ShowWindow(bEnable);
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL12)->ShowWindow(bEnable);
	// convergence criteria
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL10)->EnableWindow(m_bDispNorm);
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL11)->EnableWindow(m_bForcNorm);
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL12)->EnableWindow(m_bEnrgNorm);

	// Use Line Search Method  
	BOOL bChk = FALSE;

	if (m_pData->nAnalType == D_THIS_ANALTYPE_NONLINEAR)
	{
		if (m_pData->nAnalMethod == D_THIS_ANALMETHOD_MODAL)
		{
			bChk = TRUE;
		}
		else if (m_pData->nAnalMethod == D_THIS_ANALMETHOD_STATIC)
		{
			if (m_pData->StaticCtrl.nCtrlMethod == 1) bChk = TRUE;
		}
	}

	GetDlgItem(IDC_CMD_USE_LINE_SEARCH_CHECK)->EnableWindow(!bChk);

	OnUseLineSearch();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CThisIterationCtrlDlg::OnOK() 
{
	// Dlg2Data
	UpdateData(TRUE);
	m_pData->BoundaryNL_Old.bPermitConvergence   = m_bConvergenceFail;
	m_pData->BoundaryNL_Old.bDispNorm            = m_bDispNorm;
	m_pData->BoundaryNL_Old.bForceNorm           = m_bForcNorm;
	m_pData->BoundaryNL_Old.bEnergyNorm          = m_bEnrgNorm;
	m_pData->BoundaryNL_Old.nRungeKuttaMethod    = m_nRungeKutta;
	m_pData->BoundaryNL_Old.dMinSubStepSize      = m_edtMinStepSize.GetEditValue();
	m_pData->BoundaryNL_Old.nMaxIterNum          = m_edtMaxIteration.GetEditValueInt();
	m_pData->BoundaryNL_Old.dDispNorm            = m_edtDispNorm.GetEditValue();
	m_pData->BoundaryNL_Old.dForceNorm           = m_edtForcNorm.GetEditValue();
	m_pData->BoundaryNL_Old.dEnergyNorm          = m_edtEnrgNorm.GetEditValue();
	m_pData->BoundaryNL_Old.nMaxNumSubStep       = (int)m_edtMaxStepSize.GetEditValue();
	m_pData->BoundaryNL_Old.dRungeKuttaTolerance = m_edtRungeKutaTolerance.GetEditValue();

	m_pData->BoundaryNL_Old.bUseLineSearch       = m_bUseLineSearch;
	m_pData->BoundaryNL_Old.nNumSearch           = m_edtLineSearchNum.GetEditValue();
	
	CDialogMove::OnOK();
}

void CThisIterationCtrlDlg::OnCmdNonlinearChk1() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL10)->EnableWindow(m_bDispNorm);
}

void CThisIterationCtrlDlg::OnCmdNonlinearChk2() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL11)->EnableWindow(m_bForcNorm);
}

void CThisIterationCtrlDlg::OnCmdNonlinearChk3() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_EDIT_CONV_TOL12)->EnableWindow(m_bEnrgNorm);
}

void CThisIterationCtrlDlg::OnUseLineSearch() 
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aLineSearch, m_bUseLineSearch);
}


