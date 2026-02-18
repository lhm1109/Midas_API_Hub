//////////
// AssessmentParamUserSTLNRDlg.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_dgn.h"
#include "AssessmentParamUserSTLNRDlg.h"

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
// CAssessmentParamUserSTLNRDlgdialog
CAssessmentParamUserSTLNRDlg::CAssessmentParamUserSTLNRDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CAssessmentParamUserSTLNRDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
}

CAssessmentParamUserSTLNRDlg::~CAssessmentParamUserSTLNRDlg()
{

}

void CAssessmentParamUserSTLNRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssessmentParamUserSTLNRDlg)	

	DDX_Control(pDX, IDC_DGN_CS_ULS_STEEL_EDT,       m_edtCsUlsSteel);
	DDX_Control(pDX, IDC_DGN_CS_ULS_BUCK_STIFF_EDT,  m_edtCsUlsBuckStiff);
	DDX_Control(pDX, IDC_DGN_CS_ULS_YIELD_STIFF_EDT, m_edtCsUlsYieldStiff);
	DDX_Control(pDX, IDC_DGN_CS_ULS_WF_RIVET_EDT,    m_edtCsUlsWFRivet);
	DDX_Control(pDX, IDC_DGN_CS_ULS_OTHER_RIVET_EDT, m_edtCsUlsOtherRivet);
		
	DDX_Control(pDX, IDC_DGN_WS_ULS_STEEL_EDT,       m_edtWsUlsSteel);
	DDX_Control(pDX, IDC_DGN_WS_ULS_BUCK_STIFF_EDT,  m_edtWsUlsBuckStiff);
	DDX_Control(pDX, IDC_DGN_WS_ULS_YIELD_STIFF_EDT, m_edtWsUlsYieldStiff);
	DDX_Control(pDX, IDC_DGN_WS_ULS_WF_RIVET_EDT,    m_edtWsUlsWFRivet);
	DDX_Control(pDX, IDC_DGN_WS_ULS_OTHER_RIVET_EDT, m_edtWsUlsOtherRivet);
		
	DDX_Control(pDX, IDC_CMD_GAMMA_F3_ULS_EDT, m_edtUlsGamma_f3);

	//DDX_Control(pDX, IDC_DGN_RATING_PARAM_STEEL_UNIT, m_TensSteelUnit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAssessmentParamUserSTLNRDlg, CDialogMove)
	//{{AFX_MSG_MAP(CAssessmentParamUserSTLNRDlg)
	ON_BN_CLICKED(IDC_DGN_UPDATE_BY_CODE_BTN, OnUpdateByCode)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAssessmentParamUserSTLNRDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	SetControl();

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAssessmentParamUserSTLNRDlg::SetControl()
{
	m_edtCsUlsSteel.SetUnitType(D_UNITSYS_NONE);
	m_edtCsUlsBuckStiff.SetUnitType(D_UNITSYS_NONE);
	m_edtCsUlsYieldStiff.SetUnitType(D_UNITSYS_NONE);
	m_edtCsUlsWFRivet.SetUnitType(D_UNITSYS_NONE);
	m_edtCsUlsOtherRivet.SetUnitType(D_UNITSYS_NONE);

	m_edtWsUlsSteel.SetUnitType(D_UNITSYS_NONE);
	m_edtWsUlsBuckStiff.SetUnitType(D_UNITSYS_NONE);
	m_edtWsUlsYieldStiff.SetUnitType(D_UNITSYS_NONE);
	m_edtWsUlsWFRivet.SetUnitType(D_UNITSYS_NONE);
	m_edtWsUlsOtherRivet.SetUnitType(D_UNITSYS_NONE);

	m_edtUlsGamma_f3	.SetUnitType(D_UNITSYS_NONE);
}

void CAssessmentParamUserSTLNRDlg::OnOk()
{
	if (!Dlg2Data())
	{
		return;
	}

	CDialogMove::OnOK();
}

BOOL CAssessmentParamUserSTLNRDlg::Dlg2Data()
{
	m_Data.dCsUlsSteel = m_edtCsUlsSteel.GetEditValue();
	m_Data.dCsUlsBuckStiff = m_edtCsUlsBuckStiff.GetEditValue();
	m_Data.dCsUlsYieldStiff = m_edtCsUlsYieldStiff.GetEditValue();
	m_Data.dCsUlsWFRivet = m_edtCsUlsWFRivet.GetEditValue();
	m_Data.dCsUlsOtherRivet = m_edtCsUlsOtherRivet.GetEditValue();

	m_Data.dWsUlsSteel = m_edtWsUlsSteel.GetEditValue();
	m_Data.dWsUlsBuckStiff = m_edtWsUlsBuckStiff.GetEditValue();
	m_Data.dWsUlsYieldStiff = m_edtWsUlsYieldStiff.GetEditValue();
	m_Data.dWsUlsWFRivet = m_edtWsUlsWFRivet.GetEditValue();
	m_Data.dWsUlsOtherRivet = m_edtWsUlsOtherRivet.GetEditValue();

	m_Data.dUlsGamma_f3 = m_edtUlsGamma_f3.GetEditValue();

	return TRUE;
}

void CAssessmentParamUserSTLNRDlg::Data2Dlg()
{
	m_edtCsUlsSteel.SetEditUnit(m_Data.dCsUlsSteel);
	m_edtCsUlsBuckStiff.SetEditUnit(m_Data.dCsUlsBuckStiff);
	m_edtCsUlsYieldStiff.SetEditUnit(m_Data.dCsUlsYieldStiff);
	m_edtCsUlsWFRivet.SetEditUnit(m_Data.dCsUlsWFRivet);
	m_edtCsUlsOtherRivet.SetEditUnit(m_Data.dCsUlsOtherRivet);

	m_edtWsUlsSteel.SetEditUnit(m_Data.dWsUlsSteel);
	m_edtWsUlsBuckStiff.SetEditUnit(m_Data.dWsUlsBuckStiff);
	m_edtWsUlsYieldStiff.SetEditUnit(m_Data.dWsUlsYieldStiff);
	m_edtWsUlsWFRivet.SetEditUnit(m_Data.dWsUlsWFRivet);
	m_edtWsUlsOtherRivet.SetEditUnit(m_Data.dWsUlsOtherRivet);

	m_edtUlsGamma_f3.SetEditUnit(m_Data.dUlsGamma_f3);
}

void CAssessmentParamUserSTLNRDlg::OnUpdateByCode()
{
	m_Data.Initialize();
	Data2Dlg();
}

void CAssessmentParamUserSTLNRDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CAssessmentParamUserSTLNRDlg::SetData(T_RPSS_FT_D& Data)
{
	m_Data = Data;
}

void CAssessmentParamUserSTLNRDlg::GetData(T_RPSS_FT_D& Data)
{
	Data = m_Data;
}
