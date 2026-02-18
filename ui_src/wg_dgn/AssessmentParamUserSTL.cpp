//////////
// AssessmentParamUserSTL.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_dgn.h"
#include "AssessmentParamUserSTL.h"

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
// CAssessmentParamUserSTLdialog
CAssessmentParamUserSTL::CAssessmentParamUserSTL(CWnd* pParent /*=NULL*/)
	: CDialogMove(CAssessmentParamUserSTL::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
}

CAssessmentParamUserSTL::~CAssessmentParamUserSTL()
{

}

void CAssessmentParamUserSTL::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssessmentParamUserSTL)	

	DDX_Control(pDX, IDC_DGN_CS_ULS_REIN_EDT		, m_edtCsUlsReinforce);
	DDX_Control(pDX, IDC_DGN_CS_ULS_CONC_EDT		, m_edtCsUlsConcrete);
	DDX_Control(pDX, IDC_DGN_CS_ULS_STRUCT_STL_EDT	, m_edtCsUlsStructStl);
	DDX_Control(pDX, IDC_DGN_CS_ULS_STIFF_COMP_EDT	, m_edtCsUlsStiffComp);
	DDX_Control(pDX, IDC_DGN_CS_ULS_STIFF_TENS_EDT	, m_edtCsUlsStiffTens);
	DDX_Control(pDX, IDC_DGN_CS_ULS_SHEAR_CONC_EDT	, m_edtCsUlsShearConc);
	
	DDX_Control(pDX, IDC_DGN_CS_SLS_REIN_EDT		, m_edtCsSlsReinforce);
	DDX_Control(pDX, IDC_DGN_CS_SLS_CONC_EDT		, m_edtCsSlsConcrete);
	DDX_Control(pDX, IDC_DGN_CS_SLS_STRUCT_STL_EDT	, m_edtCsSlsStructStl);
	DDX_Control(pDX, IDC_DGN_CS_SLS_STIFF_COMP_EDT	, m_edtCsSlsStiffComp);
	DDX_Control(pDX, IDC_DGN_CS_SLS_STIFF_TENS_EDT	, m_edtCsSlsStiffTens);
	DDX_Control(pDX, IDC_DGN_CS_SLS_SHEAR_CONC_EDT	, m_edtCsSlsShearConc);
	
	DDX_Control(pDX, IDC_DGN_WS_ULS_REIN_EDT		, m_edtWsUlsReinforce);
	DDX_Control(pDX, IDC_DGN_WS_ULS_CONC_EDT		, m_edtWsUlsConcrete);
	DDX_Control(pDX, IDC_DGN_WS_ULS_STRUCT_STL_EDT	, m_edtWsUlsStructStl);
	DDX_Control(pDX, IDC_DGN_WS_ULS_STIFF_COMP_EDT	, m_edtWsUlsStiffComp);
	DDX_Control(pDX, IDC_DGN_WS_ULS_STIFF_TENS_EDT	, m_edtWsUlsStiffTens);
	DDX_Control(pDX, IDC_DGN_WS_ULS_SHEAR_CONC_EDT	, m_edtWsUlsShearConc);
	
	DDX_Control(pDX, IDC_DGN_WS_SLS_REIN_EDT		, m_edtWsSlsReinforce);
	DDX_Control(pDX, IDC_DGN_WS_SLS_CONC_EDT		, m_edtWsSlsConcrete);
	DDX_Control(pDX, IDC_DGN_WS_SLS_STRUCT_STL_EDT	, m_edtWsSlsStructStl);
	DDX_Control(pDX, IDC_DGN_WS_SLS_STIFF_COMP_EDT	, m_edtWsSlsStiffComp);
	DDX_Control(pDX, IDC_DGN_WS_SLS_STIFF_TENS_EDT	, m_edtWsSlsStiffTens);
	DDX_Control(pDX, IDC_DGN_WS_SLS_SHEAR_CONC_EDT	, m_edtWsSlsShearConc);
	
	DDX_Control(pDX, IDC_CMD_GAMMA_F3_ULS_EDT, m_edtUlsGamma_f3);
	DDX_Control(pDX, IDC_CMD_GAMMA_F3_SLS_EDT, m_edtSlsGamma_f3);
	//DDX_Control(pDX, IDC_DGN_RATING_PARAM_STEEL_UNIT, m_TensSteelUnit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAssessmentParamUserSTL, CDialogMove)
	//{{AFX_MSG_MAP(CAssessmentParamUserSTL)
	ON_BN_CLICKED(IDC_DGN_UPDATE_BY_CODE_BTN, OnUpdateByCode)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAssessmentParamUserSTL::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	SetControl();
	SetText();
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAssessmentParamUserSTL::SetControl()
{
	//m_dAlwTendonStress.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_edtCsUlsReinforce	.SetUnitType(D_UNITSYS_NONE);
	m_edtCsUlsConcrete	.SetUnitType(D_UNITSYS_NONE);
	m_edtCsUlsStructStl	.SetUnitType(D_UNITSYS_NONE);
	m_edtCsUlsStiffComp	.SetUnitType(D_UNITSYS_NONE);
	m_edtCsUlsStiffTens	.SetUnitType(D_UNITSYS_NONE);
	m_edtCsUlsShearConc	.SetUnitType(D_UNITSYS_NONE);
	m_edtCsSlsReinforce	.SetUnitType(D_UNITSYS_NONE);
	m_edtCsSlsConcrete	.SetUnitType(D_UNITSYS_NONE);
	m_edtCsSlsStructStl	.SetUnitType(D_UNITSYS_NONE);
	m_edtCsSlsStiffComp	.SetUnitType(D_UNITSYS_NONE);
	m_edtCsSlsStiffTens	.SetUnitType(D_UNITSYS_NONE);
	m_edtCsSlsShearConc	.SetUnitType(D_UNITSYS_NONE);
	m_edtWsUlsReinforce	.SetUnitType(D_UNITSYS_NONE);
	m_edtWsUlsConcrete	.SetUnitType(D_UNITSYS_NONE);
	m_edtWsUlsStructStl	.SetUnitType(D_UNITSYS_NONE);
	m_edtWsUlsStiffComp	.SetUnitType(D_UNITSYS_NONE);
	m_edtWsUlsStiffTens	.SetUnitType(D_UNITSYS_NONE);
	m_edtWsUlsShearConc	.SetUnitType(D_UNITSYS_NONE);
	m_edtWsSlsReinforce	.SetUnitType(D_UNITSYS_NONE);
	m_edtWsSlsConcrete	.SetUnitType(D_UNITSYS_NONE);
	m_edtWsSlsStructStl	.SetUnitType(D_UNITSYS_NONE);
	m_edtWsSlsStiffComp	.SetUnitType(D_UNITSYS_NONE);
	m_edtWsSlsStiffTens	.SetUnitType(D_UNITSYS_NONE);
	m_edtWsSlsShearConc	.SetUnitType(D_UNITSYS_NONE);
	m_edtUlsGamma_f3	.SetUnitType(D_UNITSYS_NONE);
	m_edtSlsGamma_f3	.SetUnitType(D_UNITSYS_NONE);
}

void CAssessmentParamUserSTL::SetText()
{
	this->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_TITLE));

	
	GetDlgItem(IDC_DGN_UPDATE_BY_CODE_BTN)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_UPDATE_BY_CODE));
	GetDlgItem(IDC_CMD_GAMMA_M_GRP)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_GAMMA_M));


	GetDlgItem(IDC_CMD_GAMMA_M_CS_GRP)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_CHR_STR));
	GetDlgItem(IDC_DGN_CS_ULS_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_ULS));
	GetDlgItem(IDC_DGN_CS_SLS_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_SLS));
	GetDlgItem(IDC_DGN_CS_REIN_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_REINF));
	GetDlgItem(IDC_DGN_CS_CONC_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_CONC));
	GetDlgItem(IDC_DGN_CS_STRUCT_STL_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_STRUCT_STL));
	GetDlgItem(IDC_DGN_CS_STIFF_COMP_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_STIFF_COMP));
	GetDlgItem(IDC_DGN_CS_STIFF_TENS_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_STIFF_TENS));
	GetDlgItem(IDC_DGN_CS_SHEAR_CONC_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_SHEAR_CONC));


	GetDlgItem(IDC_CMD_GAMMA_M_WS_GRP)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_WORST_CRE_STR));
	GetDlgItem(IDC_DGN_WS_ULS_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_ULS));
	GetDlgItem(IDC_DGN_WS_SLS_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_SLS));
	GetDlgItem(IDC_DGN_WS_REIN_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_REINF));
	GetDlgItem(IDC_DGN_WS_CONC_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_CONC));
	GetDlgItem(IDC_DGN_WS_STRUCT_STL_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_STRUCT_STL));
	GetDlgItem(IDC_DGN_WS_STIFF_COMP_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_STIFF_COMP));
	GetDlgItem(IDC_DGN_WS_STIFF_TENS_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_STIFF_TENS));
	GetDlgItem(IDC_DGN_WS_SHEAR_CONC_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_SHEAR_CONC));

	GetDlgItem(IDC_CMD_GAMMA_F3_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_GAMMA_F3));
	GetDlgItem(IDC_CMD_GAMMA_F3_ULS_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_ULS));
	GetDlgItem(IDC_CMD_GAMMA_F3_SLS_STC)->SetWindowText(_LS(IDS_DGN_ASSESSMENT_PARAM_USER_STL_SLS));
}

void CAssessmentParamUserSTL::OnOk()
{
	if (!Dlg2Data())
	{
		return;
	}

	CDialogMove::OnOK();
}

BOOL CAssessmentParamUserSTL::Dlg2Data()
{
	m_Data.dCsUlsReinforce = m_edtCsUlsReinforce.GetEditValue();
	m_Data.dCsUlsConcrete = m_edtCsUlsConcrete.GetEditValue();
	m_Data.dCsUlsStructStl = m_edtCsUlsStructStl.GetEditValue();
	m_Data.dCsUlsStiffComp = m_edtCsUlsStiffComp.GetEditValue();
	m_Data.dCsUlsStiffTens = m_edtCsUlsStiffTens.GetEditValue();
	m_Data.dCsUlsShearConc = m_edtCsUlsShearConc.GetEditValue();
	m_Data.dCsSlsReinforce = m_edtCsSlsReinforce.GetEditValue();
	m_Data.dCsSlsConcrete = m_edtCsSlsConcrete.GetEditValue();
	m_Data.dCsSlsStructStl = m_edtCsSlsStructStl.GetEditValue();
	m_Data.dCsSlsStiffComp = m_edtCsSlsStiffComp.GetEditValue();
	m_Data.dCsSlsStiffTens = m_edtCsSlsStiffTens.GetEditValue();
	m_Data.dCsSlsShearConc = m_edtCsSlsShearConc.GetEditValue();
	m_Data.dWsUlsReinforce = m_edtWsUlsReinforce.GetEditValue();
	m_Data.dWsUlsConcrete = m_edtWsUlsConcrete.GetEditValue();
	m_Data.dWsUlsStructStl = m_edtWsUlsStructStl.GetEditValue();
	m_Data.dWsUlsStiffComp = m_edtWsUlsStiffComp.GetEditValue();
	m_Data.dWsUlsStiffTens = m_edtWsUlsStiffTens.GetEditValue();
	m_Data.dWsUlsShearConc = m_edtWsUlsShearConc.GetEditValue();
	m_Data.dWsSlsReinforce = m_edtWsSlsReinforce.GetEditValue();
	m_Data.dWsSlsConcrete = m_edtWsSlsConcrete.GetEditValue();
	m_Data.dWsSlsStructStl = m_edtWsSlsStructStl.GetEditValue();
	m_Data.dWsSlsStiffComp = m_edtWsSlsStiffComp.GetEditValue();
	m_Data.dWsSlsStiffTens = m_edtWsSlsStiffTens.GetEditValue();
	m_Data.dWsSlsShearConc = m_edtWsSlsShearConc.GetEditValue();;
	m_Data.dUlsGamma_f3 = m_edtUlsGamma_f3.GetEditValue();
	m_Data.dSlsGamma_f3 = m_edtSlsGamma_f3.GetEditValue();

	return TRUE;
}

void CAssessmentParamUserSTL::Data2Dlg()
{
	m_edtCsUlsReinforce.SetEditUnit(m_Data.dCsUlsReinforce);
	m_edtCsUlsConcrete.SetEditUnit(m_Data.dCsUlsConcrete);
	m_edtCsUlsStructStl.SetEditUnit(m_Data.dCsUlsStructStl);
	m_edtCsUlsStiffComp.SetEditUnit(m_Data.dCsUlsStiffComp);
	m_edtCsUlsStiffTens.SetEditUnit(m_Data.dCsUlsStiffTens);
	m_edtCsUlsShearConc.SetEditUnit(m_Data.dCsUlsShearConc);
	m_edtCsSlsReinforce.SetEditUnit(m_Data.dCsSlsReinforce);
	m_edtCsSlsConcrete.SetEditUnit(m_Data.dCsSlsConcrete);
	m_edtCsSlsStructStl.SetEditUnit(m_Data.dCsSlsStructStl);
	m_edtCsSlsStiffComp.SetEditUnit(m_Data.dCsSlsStiffComp);
	m_edtCsSlsStiffTens.SetEditUnit(m_Data.dCsSlsStiffTens);
	m_edtCsSlsShearConc.SetEditUnit(m_Data.dCsSlsShearConc);
	m_edtWsUlsReinforce.SetEditUnit(m_Data.dWsUlsReinforce);
	m_edtWsUlsConcrete.SetEditUnit(m_Data.dWsUlsConcrete);
	m_edtWsUlsStructStl.SetEditUnit(m_Data.dWsUlsStructStl);
	m_edtWsUlsStiffComp.SetEditUnit(m_Data.dWsUlsStiffComp);
	m_edtWsUlsStiffTens.SetEditUnit(m_Data.dWsUlsStiffTens);
	m_edtWsUlsShearConc.SetEditUnit(m_Data.dWsUlsShearConc);
	m_edtWsSlsReinforce.SetEditUnit(m_Data.dWsSlsReinforce);
	m_edtWsSlsConcrete.SetEditUnit(m_Data.dWsSlsConcrete);
	m_edtWsSlsStructStl.SetEditUnit(m_Data.dWsSlsStructStl);
	m_edtWsSlsStiffComp.SetEditUnit(m_Data.dWsSlsStiffComp);
	m_edtWsSlsStiffTens.SetEditUnit(m_Data.dWsSlsStiffTens);
	m_edtWsSlsShearConc.SetEditUnit(m_Data.dWsSlsShearConc);
	m_edtUlsGamma_f3.SetEditUnit(m_Data.dUlsGamma_f3);
	m_edtSlsGamma_f3.SetEditUnit(m_Data.dSlsGamma_f3);
}

void CAssessmentParamUserSTL::OnUpdateByCode()
{
	m_Data.Initialize();
	Data2Dlg();
}

void CAssessmentParamUserSTL::OnCancel()
{
	CDialogMove::OnCancel();
}

void CAssessmentParamUserSTL::SetData(T_RPSS_FT_D& Data)
{
	m_Data = Data;
}

void CAssessmentParamUserSTL::GetData(T_RPSS_FT_D& Data)
{
	Data = m_Data;
}
