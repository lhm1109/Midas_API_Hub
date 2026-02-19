//////////
// AssessmentParamUserPSC.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_dgn.h"
#include "AssessmentParamUserPSC.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssessmentParamUserPSCdialog
CAssessmentParamUserPSC::CAssessmentParamUserPSC(CWnd* pParent /*=NULL*/)
	: CDialogMove(CAssessmentParamUserPSC::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
}

CAssessmentParamUserPSC::~CAssessmentParamUserPSC()
{

}

void CAssessmentParamUserPSC::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssessmentParamUserPSC)	
	DDX_Control(pDX, IDC_DGN_STR_REBAR_TNDN_EDT, m_edtCsTndn);
	DDX_Control(pDX, IDC_DGN_STR_CONC_EDT, m_edtCsConcrete);
	DDX_Control(pDX, IDC_DGN_STR_SHEAR_CONC_EDT, m_edtCsShearConc);
	DDX_Control(pDX, IDC_DGN_WORST_REBAR_TNDN_EDT, m_edtWsTndn);
	DDX_Control(pDX, IDC_DGN_WORST_CONC_EDT, m_edtWsConcrete);
	DDX_Control(pDX, IDC_DGN_WORST_SHEAR_CONC_EDT, m_edtWsShearConc);
	DDX_Control(pDX, IDC_DGN_TRI_COMP_STRS_EDT, m_edtSfTriCmp);
	DDX_Control(pDX, IDC_DGN_UNI_COMP_STRS_EDT, m_edtSfUniformCmp);
	DDX_Control(pDX, IDC_DGN_PRE_TENS_EDT, m_edtSfPreTens);
	DDX_Control(pDX, IDC_DGN_POST_TENS_EDT, m_edtSfPostTens);
	//DDX_Control(pDX, IDC_DGN_RATING_PARAM_STEEL_UNIT, m_TensSteelUnit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAssessmentParamUserPSC, CDialogMove)
	//{{AFX_MSG_MAP(CAssessmentParamUserPSC)
	ON_BN_CLICKED(IDC_DGN_UPDATE_BY_CODE_BTN, OnUpdateByCode)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAssessmentParamUserPSC::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	SetControl();
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAssessmentParamUserPSC::SetControl()
{
	//m_dAlwTendonStress.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_edtCsTndn.SetUnitType(D_UNITSYS_NONE);
	m_edtCsConcrete.SetUnitType(D_UNITSYS_NONE);
	m_edtCsShearConc.SetUnitType(D_UNITSYS_NONE);
	m_edtWsTndn.SetUnitType(D_UNITSYS_NONE);
	m_edtWsConcrete.SetUnitType(D_UNITSYS_NONE);
	m_edtWsShearConc.SetUnitType(D_UNITSYS_NONE);
	m_edtSfTriCmp.SetUnitType(D_UNITSYS_NONE);
	m_edtSfUniformCmp.SetUnitType(D_UNITSYS_NONE);
	m_edtSfPreTens.SetUnitType(D_UNITSYS_NONE);
	m_edtSfPostTens.SetUnitType(D_UNITSYS_NONE);
}

void CAssessmentParamUserPSC::OnOk() 
{
	if(!Dlg2Data())
	{
		return;
	}

	CDialogMove::OnOK();
}

BOOL CAssessmentParamUserPSC::Dlg2Data() 
{
	m_Data.dCsTndn = m_edtCsTndn.GetEditValue();
	m_Data.dCsConcrete = m_edtCsConcrete.GetEditValue();
	m_Data.dCsShearConc = m_edtCsShearConc.GetEditValue();
	m_Data.dWsTndn = m_edtWsTndn.GetEditValue();
	m_Data.dWsConcrete = m_edtWsConcrete.GetEditValue();
	m_Data.dWsShearConc = m_edtWsShearConc.GetEditValue();
	m_Data.dSfTriCmp = m_edtSfTriCmp.GetEditValue();
	m_Data.dSfUniformCmp = m_edtSfUniformCmp.GetEditValue();
	m_Data.dSfPreTens = m_edtSfPreTens.GetEditValue();
	m_Data.dSfPostTens = m_edtSfPostTens.GetEditValue();

	return TRUE;
}

void CAssessmentParamUserPSC::Data2Dlg() 
{
	m_edtCsTndn.SetEditUnit(m_Data.dCsTndn);
	m_edtCsConcrete.SetEditUnit(m_Data.dCsConcrete);
	m_edtCsShearConc.SetEditUnit(m_Data.dCsShearConc);
	m_edtWsTndn.SetEditUnit(m_Data.dWsTndn);
	m_edtWsConcrete.SetEditUnit(m_Data.dWsConcrete);
	m_edtWsShearConc.SetEditUnit(m_Data.dWsShearConc);
	m_edtSfTriCmp.SetEditUnit(m_Data.dSfTriCmp);
	m_edtSfUniformCmp.SetEditUnit(m_Data.dSfUniformCmp);
	m_edtSfPreTens.SetEditUnit(m_Data.dSfPreTens);
	m_edtSfPostTens.SetEditUnit(m_Data.dSfPostTens);
}

void CAssessmentParamUserPSC::OnUpdateByCode() 
{
	m_Data.Initialize();
	Data2Dlg();
}

void CAssessmentParamUserPSC::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CAssessmentParamUserPSC::SetData(T_ASPM_FT_D& Data)
{
	m_Data = Data;
}

void CAssessmentParamUserPSC::GetData(T_ASPM_FT_D& Data)
{
	Data = m_Data;
}
