// CMAnalysisOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "math.h"
#include "CMAnalysisOptionDlg_MEC.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_dbLock\LockOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMAnalysisOptionDlg_MEC dialog

CCMAnalysisOptionDlg_MEC::CCMAnalysisOptionDlg_MEC(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMAnalysisOptionDlg_MEC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMAnalysisOptionDlg_MEC)
	//}}AFX_DATA_INIT

	m_AnalOptionMEC.Initialize();
	//m_AnopD.Initialize();
}

void CCMAnalysisOptionDlg_MEC::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMAnalysisOptionDlg_MEC)
	DDX_Control(pDX, IDC_CMD_ANALYOPT_SOLVER_TYPE_CMB, m_cmbEquationSolver);
	DDX_Control(pDX, IDC_CMD_MULTI_PROCESSOR_CHK, m_ChkMultiProcessor);
	DDX_Control(pDX, IDC_CMD_PROCESSOR_NUM_EDT, m_edtspinMultiProcess);
	DDX_Control(pDX, IDC_CMD_GPU_ACCEL_CHK, m_chkGPUAcceleration);
	DDX_Control(pDX, IDC_CMD_STIFFNESS_DIAGONAL_EDT, m_edtStiffnessDiagonal);
	DDX_Control(pDX, IDC_CMD_SPRING_CONSTANT_EDT, m_edtSpringConstant);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMAnalysisOptionDlg_MEC, CDialogMove)
	//{{AFX_MSG_MAP(CCMAnalysisOptionDlg_MEC)
	ON_BN_CLICKED(IDC_CMD_MULTI_PROCESSOR_CHK, OnClickMultiProcessorChk)
	ON_BN_CLICKED(IDC_CMD_ANALYOPT_OK, OnCmdAnalyoptOk)
	ON_BN_CLICKED(IDC_CMD_ANALYOPT_CANCEL, OnCmdAnalyoptCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMAnalysisOptionDlg_MEC::OnInitDlgAnalOptionMEC()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pInitCtrl->GetAnalysisOptionMEC(m_AnalOptionMEC);

	InitializeEquationSolverCmb();
	CDlgUtil::CobxSetCurSelItemData(m_cmbEquationSolver, m_AnalOptionMEC.nAnalMethod);

	m_edtspinMultiProcess.SetRange(1, 100);

	CString str;
	str.Format(_T("%d"), m_AnalOptionMEC.nNumMultiProcessor);
	m_edtspinMultiProcess.SetWindowText(str);

	const BOOL bUseMultiProcessor = m_AnalOptionMEC.nNumMultiProcessor > 1;
	if (bUseMultiProcessor)
	{
		m_ChkMultiProcessor.SetCheck(TRUE);
	}
	else
	{
		m_ChkMultiProcessor.SetCheck(FALSE);
	}
	EnableNumProcessor(bUseMultiProcessor);

	m_chkGPUAcceleration.SetCheck(m_AnalOptionMEC.bUseGPUAccel);

	if (!pDoc->AllowCommand(D_OPTN_ID_GPU))
		m_chkGPUAcceleration.EnableWindow(FALSE);

	return TRUE;
}

BOOL CCMAnalysisOptionDlg_MEC::OnInitDlgAnop()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	pDoc->m_pAttrCtrl2->GetAnop(m_AnopD);

	m_edtStiffnessDiagonal.SetEditUnit(m_AnopD.dStiffnessDiagonal);
	m_edtSpringConstant.SetEditUnit(m_AnopD.dSpringConstant);

	return TRUE;
}

void CCMAnalysisOptionDlg_MEC::InitializeEquationSolverCmb()
{
	using ANALOPTION_MEC::EquationSolver;

	std::vector<std::pair<CString, DWORD>> vData = {
	{_T("AUTO"), EquationSolver::AUTO},
	{_T("Multi Frontal Sparse"), EquationSolver::MFS}
	};

	CDlgUtil::InitComboBox(m_cmbEquationSolver, vData);
}

void CCMAnalysisOptionDlg_MEC::EnableNumProcessor(BOOL bEnable)
{
	m_edtspinMultiProcess.EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// CCMAnalysisOptionDlg_MEC message handlers

BOOL CCMAnalysisOptionDlg_MEC::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	OnInitDlgAnalOptionMEC();

	OnInitDlgAnop();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMAnalysisOptionDlg_MEC::Dlg2Data()
{
	if (!Dlg2DataAnalOption()) return FALSE;
	if (!Dlg2DataAnop()) return FALSE;

	return TRUE;
}

BOOL CCMAnalysisOptionDlg_MEC::Dlg2DataAnalOption()
{
	// Equation Solver
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbEquationSolver, reinterpret_cast<DWORD&>(m_AnalOptionMEC.nAnalMethod));

	// Multi processor
	m_AnalOptionMEC.nNumMultiProcessor = static_cast<int>(m_edtspinMultiProcess.GetEditValue());

	// Gpu Acceleration
	m_AnalOptionMEC.bUseGPUAccel = m_chkGPUAcceleration.GetCheck();

	if (!CheckAnalOption()) return FALSE;

	return TRUE;
}

BOOL CCMAnalysisOptionDlg_MEC::Dlg2DataAnop()
{
	m_AnopD.dStiffnessDiagonal = m_edtStiffnessDiagonal.GetEditValue();
	m_AnopD.dSpringConstant = m_edtSpringConstant.GetEditValue();

	return TRUE;
}

BOOL CCMAnalysisOptionDlg_MEC::CheckAnalOption()
{
	if (m_AnalOptionMEC.nNumMultiProcessor < 0 || m_AnalOptionMEC.nNumMultiProcessor > 100)
	{
		AfxMessageBox(_LS(IDS_CMD_ANAL_OPTION_MULTI_PROCESSOR_ERROR));

		return FALSE;
	}

	return TRUE;
}

void CCMAnalysisOptionDlg_MEC::OnClickMultiProcessorChk()
{
	BOOL bChk = m_ChkMultiProcessor.GetCheck();

	if (bChk == BST_CHECKED)
	{
		using ANALOPTION_MEC::DefaultNumMultiProcessor;

		CString str;
		str.Format(_T("%d"), DefaultNumMultiProcessor);
		m_edtspinMultiProcess.SetWindowText(str);
	}
	else
	{
		CString str;
		str.Format(_T("%d"), 1);
		m_edtspinMultiProcess.SetWindowText(str);
	}

	EnableNumProcessor(bChk);
}

void CCMAnalysisOptionDlg_MEC::OnCmdAnalyoptOk()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	if (!Dlg2Data()) return;

	pDoc->m_pInitCtrl->SetAnalysisOptionMEC(m_AnalOptionMEC);
	pDoc->m_pDataCtrl->AddAnop(m_AnopD);

	CDialogMove::OnOK();
}

void CCMAnalysisOptionDlg_MEC::OnCmdAnalyoptCancel()
{
	CDialogMove::OnCancel();
}