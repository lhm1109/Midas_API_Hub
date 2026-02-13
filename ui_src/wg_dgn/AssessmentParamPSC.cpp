//////////
// AssessmentParamPSC.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_dgn.h"
#include "AssessmentParamPSC.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "AssessmentParamUserPSC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssessmentParamPSCdialog
CAssessmentParamPSC::CAssessmentParamPSC(CWnd* pParent /*=NULL*/)
	: CDialogMove(CAssessmentParamPSC::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
}

CAssessmentParamPSC::~CAssessmentParamPSC()
{

}

void CAssessmentParamPSC::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssessmentParamPSC)	
	DDX_Control(pDX, IDC_DGN_COND_FACTOR_EDT, m_edtCondFactor);
	DDX_Control(pDX, IDC_DGN_USER_CHK, m_chkUserInput);
	DDX_Control(pDX, IDC_CMD_SHR_STR_STATE_CHK, m_chkOptionShr);
	DDX_Control(pDX, IDC_DGN_FLEXURE_CHK, m_chkStrLimFlexure);
	DDX_Control(pDX, IDC_DGN_SHEAR_CHK, m_chkStrLimShear);
	DDX_Control(pDX, IDC_DGN_TORSION_CHK, m_chkStrLimTorsion);
	DDX_Control(pDX, IDC_DGN_STRS_CRACK_CHK, m_chkSevLimStrsCrack);
	DDX_Control(pDX, IDC_DGN_ULT_LIM_STATE_CHK, m_chkRptUltLim);
	DDX_Control(pDX, IDC_DGN_SERV_LIM_STATE_CHK, m_chkRptServLim);
	//DDX_Control(pDX, IDC_DGN_RATING_PARAM_STEEL_UNIT, m_TensSteelUnit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAssessmentParamPSC, CDialogMove)
	//{{AFX_MSG_MAP(CAssessmentParamPSC)
	ON_BN_CLICKED(IDC_DGN_USER_CHK, OnChkUser)
	ON_BN_CLICKED(IDC_CMD_USER_BTN, OnUser)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAssessmentParamPSC::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	if(!m_pDoc->m_pAttrCtrl->GetAspm(m_Data))
	{
		m_Data.Initialize();
	}

	SetControl();
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAssessmentParamPSC::SetControl()
{
	//m_dAlwTendonStress.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtCondFactor.SetUnitType(D_UNITSYS_NONE);

	m_aRdoGamma_m.RemoveAll();
	m_aRdoGamma_m.Add(IDC_DGN_CHR_STR_RDO);
	m_aRdoGamma_m.Add(IDC_DGN_WORST_CRE_STR_RDO);
}

void CAssessmentParamPSC::EnableCtrl()
{
	BOOL bUser = m_chkUserInput.GetCheck();
	GetDlgItem(IDC_CMD_USER_BTN)->EnableWindow(bUser);
}

void CAssessmentParamPSC::OnOk() 
{
	if(!Dlg2Data())
	{
		return;
	}

	m_pDoc->m_pDataCtrl->AddAspm(m_Data);

	CDialogMove::OnOK();
}

BOOL CAssessmentParamPSC::Dlg2Data() 
{
	m_Data.dConditionFactor = m_edtCondFactor.GetEditValue();

	m_Data.bOptionShr = m_chkOptionShr.GetCheck();
	m_Data.bUserInput = m_chkUserInput.GetCheck();
	m_Data.bStrLimFlexure = m_chkStrLimFlexure.GetCheck();
	m_Data.bStrLimShear = m_chkStrLimShear.GetCheck();
	m_Data.bStrLimTorsion = m_chkStrLimTorsion.GetCheck();
	m_Data.bSevLimStrsCrack = m_chkSevLimStrsCrack.GetCheck();
	m_Data.bRptUltLim = m_chkRptUltLim.GetCheck();
	m_Data.bRptServLim = m_chkRptServLim.GetCheck();


	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoGamma_m, m_Data.nGamma_m);

	return TRUE;
}

void CAssessmentParamPSC::Data2Dlg() 
{
	m_edtCondFactor.SetEditUnit(m_Data.dConditionFactor);


	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoGamma_m, m_Data.nGamma_m);

	m_chkOptionShr.SetCheck(m_Data.bOptionShr);
	m_chkUserInput.SetCheck(m_Data.bUserInput);
	m_chkStrLimFlexure.SetCheck(m_Data.bStrLimFlexure);
	m_chkStrLimShear.SetCheck(m_Data.bStrLimShear);
	m_chkStrLimTorsion.SetCheck(m_Data.bStrLimTorsion);
	m_chkSevLimStrsCrack.SetCheck(m_Data.bSevLimStrsCrack);
	m_chkRptUltLim.SetCheck(m_Data.bRptUltLim);
	m_chkRptServLim.SetCheck(m_Data.bRptServLim);
}

void CAssessmentParamPSC::OnChkUser() 
{
	EnableCtrl();
}

void CAssessmentParamPSC::OnUser() 
{
	CAssessmentParamUserPSC dlg;
	dlg.SetData(m_Data.Factors);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_Data.Factors);
	}
}

void CAssessmentParamPSC::OnCancel() 
{
	CDialogMove::OnCancel();
}

