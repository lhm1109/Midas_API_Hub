//////////
// AssessmentParamSTL.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_dgn.h"
#include "AssessmentParamSTL.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "AssessmentParamUserSTL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssessmentParamSTLdialog
CAssessmentParamSTL::CAssessmentParamSTL(CWnd* pParent /*=NULL*/)
	: CDialogMove(CAssessmentParamSTL::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
}

CAssessmentParamSTL::~CAssessmentParamSTL()
{

}

void CAssessmentParamSTL::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssessmentParamSTL)	
	DDX_Control(pDX, IDC_DGN_COND_FACTOR_EDT, m_edtCondFactor);
	DDX_Control(pDX, IDC_DGN_USER_CHK, m_chkUserInput);
	DDX_Control(pDX, IDC_DGN_ULS_FLEXURE_CHK, m_chkUlsFlexure);
	DDX_Control(pDX, IDC_DGN_ULS_SHEAR_CHK, m_chkUlsShear);
	DDX_Control(pDX, IDC_DGN_ULS_F_S_T_CHK, m_chkUlsFST);
	DDX_Control(pDX, IDC_DGN_ULS_L_SHEAR_CHECK, m_chkUlsLgShear);
	DDX_Control(pDX, IDC_DGN_SLS_STRS_CHK, m_chkSlsStress);
	DDX_Control(pDX, IDC_DGN_SLS_L_SHEAR_CHECK, m_chkSlsLgShear);
	DDX_Control(pDX, IDC_DGN_ULT_LIM_STATE_CHK, m_chkRptUltLim);
	DDX_Control(pDX, IDC_DGN_SERV_LIM_STATE_CHK, m_chkRptServLim);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAssessmentParamSTL, CDialogMove)
	//{{AFX_MSG_MAP(CAssessmentParamSTL)
	ON_BN_CLICKED(IDC_DGN_USER_CHK, OnChkUser)
	ON_BN_CLICKED(IDC_CMD_USER_BTN, OnUser)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAssessmentParamSTL::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	if (!m_pDoc->m_pAttrCtrl2->GetRpss(m_Data))
	{
		m_Data.Initialize();
	}

	SetControl();
	SetText();
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAssessmentParamSTL::SetControl()
{
	//m_dAlwTendonStress.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtCondFactor.SetUnitType(D_UNITSYS_NONE);

	m_aRdoGamma_m.RemoveAll();
	m_aRdoGamma_m.Add(IDC_DGN_CHR_STR_RDO);
	m_aRdoGamma_m.Add(IDC_DGN_WORST_CRE_STR_RDO);
}

void CAssessmentParamSTL::SetText()
{
	this->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_TITLE));
	GetDlgItem(IDC_DGN_COND_FACTOR_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_FC));
	GetDlgItem(IDC_CMD_GAMMA_M_GRP)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_MATL_STR));
	GetDlgItem(IDC_DGN_CHR_STR_RDO)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_CHR_STR));
	GetDlgItem(IDC_DGN_WORST_CRE_STR_RDO)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_WORST_CRE_STR));
	GetDlgItem(IDC_DGN_USER_CHK)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_USER_INPUT));
	GetDlgItem(IDC_CMD_USER_BTN)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_TITLE));
	GetDlgItem(IDC_CMD_STR_LIM_STATE_GRP)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_ULS));

	GetDlgItem(IDC_DGN_ULS_FLEXURE_CHK)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_FLEXURE));
	GetDlgItem(IDC_DGN_ULS_SHEAR_CHK)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_SHEAR));
	GetDlgItem(IDC_DGN_ULS_F_S_T_CHK)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_F_S_T));
	GetDlgItem(IDC_DGN_ULS_L_SHEAR_CHECK)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_L_SHEAR));

	GetDlgItem(IDC_CMD_SERV_LIM_STATE_GRP)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_SLS));
	GetDlgItem(IDC_DGN_SLS_STRS_CHK)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_STRS));
	GetDlgItem(IDC_DGN_SLS_L_SHEAR_CHECK)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_L_SHEAR));

	GetDlgItem(IDC_CMD_DET_RPT_GRP)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_DET_RPT));
	GetDlgItem(IDC_DGN_ULT_LIM_STATE_CHK)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_ULS));
	GetDlgItem(IDC_DGN_SERV_LIM_STATE_CHK)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_STL_SLS));

}

void CAssessmentParamSTL::EnableCtrl()
{
	BOOL bUser = m_chkUserInput.GetCheck();
	GetDlgItem(IDC_CMD_USER_BTN)->EnableWindow(bUser);
}

void CAssessmentParamSTL::OnOk()
{
	if (!Dlg2Data())
	{
		return;
	}

	m_pDoc->m_pDataCtrl->AddRpss(m_Data);

	CDialogMove::OnOK();
}

BOOL CAssessmentParamSTL::Dlg2Data()
{
	m_Data.dConditionFactor = m_edtCondFactor.GetEditValue();
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoGamma_m, m_Data.nGamma_m);
	m_Data.bUserInput = m_chkUserInput.GetCheck();

	m_Data.bUlsFlexure = m_chkUlsFlexure.GetCheck();
	m_Data.bUlsShear = m_chkUlsShear.GetCheck();
	m_Data.bUlsFST = m_chkUlsFST.GetCheck();
	m_Data.bUlsLgShear = m_chkUlsLgShear.GetCheck();
	m_Data.bSlsStress = m_chkSlsStress.GetCheck();
	m_Data.bSlsLgShear = m_chkSlsLgShear.GetCheck();
	m_Data.bRptUltLim = m_chkRptUltLim.GetCheck();
	m_Data.bRptServLim = m_chkRptServLim.GetCheck();

	return TRUE;
}

void CAssessmentParamSTL::Data2Dlg()
{
	m_edtCondFactor.SetEditUnit(m_Data.dConditionFactor);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoGamma_m, m_Data.nGamma_m);
	m_chkUserInput.SetCheck(m_Data.bUserInput);

	m_chkUlsFlexure.SetCheck(m_Data.bUlsFlexure);
	m_chkUlsShear.SetCheck(m_Data.bUlsShear);
	m_chkUlsFST.SetCheck(m_Data.bUlsFST);
	m_chkUlsLgShear.SetCheck(m_Data.bUlsLgShear);
	m_chkSlsStress.SetCheck(m_Data.bSlsStress);
	m_chkSlsLgShear.SetCheck(m_Data.bSlsLgShear);
	m_chkRptUltLim.SetCheck(m_Data.bRptUltLim);
	m_chkRptServLim.SetCheck(m_Data.bRptServLim);
}

void CAssessmentParamSTL::OnChkUser()
{
	EnableCtrl();
}

void CAssessmentParamSTL::OnUser()
{
	CAssessmentParamUserSTL dlg;
	dlg.SetData(m_Data.Factors);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_Data.Factors);
	}
}

void CAssessmentParamSTL::OnCancel()
{
	CDialogMove::OnCancel();
}

