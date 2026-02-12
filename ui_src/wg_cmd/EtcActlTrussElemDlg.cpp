// EtcActlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "EtcActlTrussElemDlg.h"
#include "CMNXLoadStepDlg.h"
#include "CMNlctNSAdvNonlinearParam.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEtcActlTrussElemDlg dialog

CEtcActlTrussElemDlg::CEtcActlTrussElemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CEtcActlTrussElemDlg::IDD, pParent)
{
	m_nLoadStepType = 0;
	m_bDisp = FALSE;
	m_bLoad = FALSE;
	m_bWork = FALSE;
}

void CEtcActlTrussElemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEtcActlTrussElemDlg)
	DDX_Control(pDX, IDC_CMD_ACTL_EXTOL_UNIT, m_unitExTolerance);
	DDX_Control(pDX, IDC_CMD_ACTL_EXTOL_EDIT, m_edtExTolerance);
	DDX_Control(pDX, IDC_CMD_ACTL_EXLCASE_EDIT, m_edtExLoadCase);

	DDX_Radio(pDX, IDC_CMD_NLAC_NUMBER_OF_INC_LOAD_RDO, m_nLoadStepType);
	DDX_Control(pDX, IDC_CMD_NLAC_EDIT_NUM_INCR_LOAD, m_edtNumberOfIncrements);
	DDX_Control(pDX, IDC_CMD_NLAC_CBX_INC_METHOD_LOAD, m_cmbIntermediateOutputRequest);
	DDX_Check(pDX, IDC_CMD_NLAC_DISP_CHK, m_bDisp);
	DDX_Check(pDX, IDC_CMD_NLAC_LOAD_CHK, m_bLoad);
	DDX_Check(pDX, IDC_CMD_NLAC_WORK_CHK, m_bWork);
	DDX_Control(pDX, IDC_CMD_NLAC_DISP_ED, m_edtDisp);
	DDX_Control(pDX, IDC_CMD_NLAC_LOAD_ED, m_edtLoad);
	DDX_Control(pDX, IDC_CMD_NLAC_WORK_ED, m_edtWork);
	//}}AFX_DATA_MAP
}

void CEtcActlTrussElemDlg::SetData(T_ACTL_D& data)
{
	m_Data = data;
}

void CEtcActlTrussElemDlg::GetData(T_ACTL_D& data)
{
	data = m_Data;
}

BEGIN_MESSAGE_MAP(CEtcActlTrussElemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CEtcActlTrussElemDlg)
	ON_BN_CLICKED(IDC_CMD_NLAC_NUMBER_OF_INC_LOAD_RDO, OnCmdNlctAcasLoadStepTypeRdo)
	ON_BN_CLICKED(IDC_CMD_NLAC_MANUAL_USER_STEP_LOAD_RDO, OnCmdNlctAcasLoadStepTypeRdo)
	ON_BN_CLICKED(IDC_CMD_NLAC_DISP_CHK, OnCmdNlctAcasDispChk)
	ON_BN_CLICKED(IDC_CMD_NLAC_LOAD_CHK, OnCmdNlctAcasLoadChk)
	ON_BN_CLICKED(IDC_CMD_NLAC_WORK_CHK, OnCmdNlctAcasWorkChk)
	ON_BN_CLICKED(IDC_CMD_NLAC_BTN_LOAD_STEP_LOAD, OnCmdNlctAcasLoadStepsBtn)
	ON_BN_CLICKED(IDC_CMD_NLAC_BTN_ADV_NON_SETTING, OnCmdNlctAcasAdvancedBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEtcActlTrussElemDlg message handlers

BOOL CEtcActlTrussElemDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();

	m_edtExTolerance.SetUnitType(CUnitCtrl::m_ACTL_UNIT.dblTrussTolerance);
	m_unitExTolerance.SetUnitType(CUnitCtrl::m_ACTL_UNIT.dblTrussTolerance);

	InitComboBox();
	InitDialogData();
	SetCtrlText();
	UpdateData(FALSE);

	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEtcActlTrussElemDlg::InitComboBox()
{
	m_cmbIntermediateOutputRequest.ResetContent();
	m_cmbIntermediateOutputRequest.AddString(_LS(IDS_CMD_COMBO_OUTPUT_REQUEST_TYPE1));
	m_cmbIntermediateOutputRequest.AddString(_LS(IDS_CMD_COMBO_OUTPUT_REQUEST_TYPE2));
	m_cmbIntermediateOutputRequest.SetCurSel(0);
}

void CEtcActlTrussElemDlg::InitUnit()
{
	m_edtNumberOfIncrements.SetUnitType(D_UNITSYS_NONE);
	m_edtDisp.SetUnitType(D_UNITSYS_NONE);
	m_edtLoad.SetUnitType(D_UNITSYS_NONE);
	m_edtWork.SetUnitType(D_UNITSYS_NONE);
}

void CEtcActlTrussElemDlg::InitDialogData()
{
	m_edtExLoadCase.SetRange(0, SHRT_MAX);
	m_edtExLoadCase.SetValue(m_Data.nIterPerLoad);
	m_edtExLoadCase.SetInteger(TRUE);
	m_edtExTolerance.SetEditUnit(m_Data.dblTrussTolerance);

	m_nLoadStepType = m_Data.nLoadStepType;
	m_edtNumberOfIncrements.SetEditUnit(m_Data.nNumberOfIncrement);
	m_cmbIntermediateOutputRequest.SetCurSel(m_Data.nIntermediateOutputRequest);
	m_bDisp = m_Data.bConvFlag[0];
	m_bLoad = m_Data.bConvFlag[1];
	m_bWork = m_Data.bConvFlag[2];
	m_edtDisp.SetEditUnit(m_Data.dConvValue[0]);
	m_edtLoad.SetEditUnit(m_Data.dConvValue[1]);
	m_edtWork.SetEditUnit(m_Data.dConvValue[2]);
}

void CEtcActlTrussElemDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_NLAC_EDIT_NUM_INCR_LOAD)->EnableWindow(m_nLoadStepType == 0);
	GetDlgItem(IDC_CMD_NLAC_STATIC_INTERMEDIATE_OUTPUT_LOAD)->EnableWindow(m_nLoadStepType == 0); //
	GetDlgItem(IDC_CMD_NLAC_CBX_INC_METHOD_LOAD)->EnableWindow(m_nLoadStepType == 0);

	//GetDlgItem(IDC_CMD_NLAC_BTN_LOAD_STEP_LOAD)->EnableWindow(m_nLoadStepType == 1); //

	OnCmdNlctAcasDispChk();
	OnCmdNlctAcasLoadChk();
	OnCmdNlctAcasWorkChk();
}

void CEtcActlTrussElemDlg::OnOK()
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	m_edtExLoadCase.GetEditValue(m_Data.nIterPerLoad);
	m_Data.dblTrussTolerance = m_edtExTolerance.GetEditValue();

	m_Data.nLoadStepType = m_nLoadStepType;
	m_Data.nNumberOfIncrement = m_edtNumberOfIncrements.GetEditValue();
	m_Data.nIntermediateOutputRequest = m_cmbIntermediateOutputRequest.GetCurSel();

	m_Data.bConvFlag[0] = m_bDisp;
	m_Data.bConvFlag[1] = m_bLoad;
	m_Data.bConvFlag[2] = m_bWork;
	m_Data.dConvValue[0] = m_edtDisp.GetEditValue();
	m_Data.dConvValue[1] = m_edtLoad.GetEditValue();
	m_Data.dConvValue[2] = m_edtWork.GetEditValue();

	CDialogMove::OnOK();
}

void CEtcActlTrussElemDlg::OnCmdNlctAcasLoadStepTypeRdo()
{
	EnableDisableControls();
}

void CEtcActlTrussElemDlg::OnCmdNlctAcasDispChk()
{
	UpdateData(TRUE);

	if (!m_bDisp && !m_bLoad && !m_bWork)
	{
		m_bDisp = TRUE;

		UpdateData(FALSE);
	}

	GetDlgItem(IDC_CMD_NLAC_DISP_ED)->EnableWindow(m_bDisp);
}

void CEtcActlTrussElemDlg::OnCmdNlctAcasLoadChk()
{
	UpdateData(TRUE);

	if (!m_bDisp && !m_bLoad && !m_bWork)
	{
		m_bLoad = TRUE;

		UpdateData(FALSE);
	}

	GetDlgItem(IDC_CMD_NLAC_LOAD_ED)->EnableWindow(m_bLoad);
}

void CEtcActlTrussElemDlg::OnCmdNlctAcasWorkChk()
{
	UpdateData(TRUE);

	if (!m_bDisp && !m_bLoad && !m_bWork)
	{
		m_bWork = TRUE;

		UpdateData(FALSE);
	}

	GetDlgItem(IDC_CMD_NLAC_WORK_ED)->EnableWindow(m_bWork);
}

void CEtcActlTrussElemDlg::OnCmdNlctAcasLoadStepsBtn()
{
	CMNXLoadStepDlg dlg;
	dlg.SetData(m_Data, 0);
	dlg.SetInitPos(D_INIT_POS_RT);

	if (dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_Data);
	}
}

void CEtcActlTrussElemDlg::OnCmdNlctAcasAdvancedBtn()
{
	CMNlctNSAdvNonlinearParam dlg(0);
	dlg.SetData(m_Data.AdvancedParam);
	dlg.SetInitPos(D_INIT_POS_RT);

	if (dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_Data.AdvancedParam);
	}
}

void CEtcActlTrussElemDlg::SetCtrlText() const
{
	GetDlgItem(IDC_CMD_NLAC_GBX_LOAD_STEPS_FRM)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_NLAC_GBX_LOAD_STEPS_FRM));
	GetDlgItem(IDC_CMD_NLAC_NUM_INCR_STC)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_NLAC_NUM_INCR_STC));
	GetDlgItem(IDC_CMD_NLAC_NUMBER_OF_INC_LOAD_RDO)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_NLAC_NUMBER_OF_INC_LOAD_RDO));
	GetDlgItem(IDC_CMD_NLAC_MANUAL_USER_STEP_LOAD_RDO)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_NLAC_MANUAL_USER_STEP_LOAD_RDO));
	GetDlgItem(IDC_CMD_NLAC_STATIC_INTERMEDIATE_OUTPUT_LOAD)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_NLAC_STATIC_INTERMEDIATE_OUTPUT_LOAD));
	GetDlgItem(IDC_CMD_NLAC_BTN_LOAD_STEP_LOAD)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_NLAC_BTN_LOAD_STEP_LOAD));
	GetDlgItem(IDC_CMD_NLAC_CONVERGENCE_FRM)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_NLAC_CONVERGENCE_FRM));
	GetDlgItem(IDC_CMD_NLAC_DISP_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_NLAC_DISP_CHK));
	GetDlgItem(IDC_CMD_NLAC_LOAD_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_NLAC_LOAD_CHK));
	GetDlgItem(IDC_CMD_NLAC_WORK_CHK)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_NLAC_WORK_CHK));
	GetDlgItem(IDC_CMD_NLAC_BTN_ADV_NON_SETTING)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_NLAC_BTN_ADV_NON_SETTING));
	GetDlgItem(IDOK)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDOK));
	GetDlgItem(IDCANCEL)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDCANCEL));
	GetDlgItem(IDC_CMD_ACTL_EXLCASE_STATIC)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_ACTL_EXLCASE_STATIC));
	GetDlgItem(IDC_CMD_ACTL_EXTOL_STATIC)->SetWindowText(_LS(IDS_CMD_ACTL_TRUSS_ELEM_IDC_CMD_ACTL_EXTOL_STATIC));
} 