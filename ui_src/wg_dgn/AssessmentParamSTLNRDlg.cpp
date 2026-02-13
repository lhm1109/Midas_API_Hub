//////////
// AssessmentParamSTLNRDlg.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_dgn.h"
#include "AssessmentParamSTLNRDlg.h"
#include "AssessmentParamUserSTLNRDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssessmentParamSTLNRDlgdialog
CAssessmentParamSTLNRDlg::CAssessmentParamSTLNRDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CAssessmentParamSTLNRDlg::IDD, pParent)
{
	m_nFomula = 0;
    m_nGamma_m = 0;
	m_nSpanType = 0;
	m_bEqA4 = TRUE;

	m_bUser = TRUE;

	m_bUlsFlexure = TRUE;
	m_bUlsShear = TRUE;
	m_bUlsLgShear = TRUE;
	m_bUlsintBearStiff = TRUE;


	m_pDoc = CDBDoc::GetDocPoint();
}

CAssessmentParamSTLNRDlg::~CAssessmentParamSTLNRDlg()
{

}

void CAssessmentParamSTLNRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssessmentParamSTLNRDlg)	

	DDX_Radio  (pDX, IDC_DGN_NR_RDO, m_nFomula);
	DDX_Control(pDX, IDC_DGN_REQ_ROUTE_AVAIL_NUM_CMB, m_cmbReqRouteAvail);
	DDX_Control(pDX, IDC_DGN_COND_FACTOR_EDT, m_edtCondFactor);
	DDX_Radio  (pDX, IDC_DGN_CHR_STR_RDO,     m_nGamma_m);
	DDX_Radio  (pDX, IDC_DGN_SIMPLE_SUP_RDO,  m_nSpanType);
	DDX_Control(pDX, IDC_DGN_K2_EDT, m_edtk2);
	DDX_Control(pDX, IDC_DGN_K3_EDT, m_edtk3);
	DDX_Control(pDX, IDC_DGN_U_FRAME_SPACING_EDT, m_edtUframeL);
	DDX_Control(pDX, IDC_DGN_U_FRAME_SPACING_UNT, m_untUframeL);
	DDX_Check  (pDX, IDC_DGN_EQA4_CHK, m_bEqA4);
	DDX_Check  (pDX, IDC_DGN_USER_CHK, m_bUser);
	DDX_Check  (pDX, IDC_DGN_ULS_FLEXURE_CHK,   m_bUlsFlexure);
	DDX_Check  (pDX, IDC_DGN_ULS_SHEAR_CHK,     m_bUlsShear);
	DDX_Check  (pDX, IDC_DGN_ULS_L_SHEAR_CHECK, m_bUlsLgShear);
	DDX_Check  (pDX, IDC_DGN_ULS_INT_BEAR_STIFF_CHK, m_bUlsintBearStiff);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAssessmentParamSTLNRDlg, CDialogMove)
	//{{AFX_MSG_MAP(CAssessmentParamSTLNRDlg)
	ON_BN_CLICKED(IDC_DGN_USER_CHK, OnChkUser)
	ON_BN_CLICKED(IDC_CMD_USER_BTN, OnUser)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAssessmentParamSTLNRDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	if (!m_pDoc->m_pAttrCtrl2->GetRpss(m_Data))
	{
		m_Data.Initialize();
	}

	SetControl();

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAssessmentParamSTLNRDlg::SetControl()
{
	m_edtk2.SetUnitType(D_UNITSYS_NONE);
	m_edtk3.SetUnitType(D_UNITSYS_NONE);

	m_edtUframeL.SetUnitType(CUnitCtrl::m_RPSS_UNIT.dUframeS);
	m_untUframeL.SetUnitType(CUnitCtrl::m_RPSS_UNIT.dUframeS);

	m_cmbReqRouteAvail.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA1),  0);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA2),  1);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA3),  2);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA4),  3);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA5),  4);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA6),  5);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA7),  6);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA8),  7);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA9),  8);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA10), 9);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA11), 10);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA12), 11);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA13), 12);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA14), 13);
	CDlgUtil::CobxAddItem(m_cmbReqRouteAvail, _LS(IDS_DGN_RPSS_REQ_ROUTE_AVAIL_NUM_RA15), 14);
	m_cmbReqRouteAvail.SetCurSel(0);
}

void CAssessmentParamSTLNRDlg::EnableCtrl()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_USER_BTN)->EnableWindow(m_bUser);
}

void CAssessmentParamSTLNRDlg::OnOk()
{
	if (!Dlg2Data())
	{
		return;
	}

	if (!m_pDoc->m_pDataCtrl->AddRpss(m_Data))
	{
		ASSERT(0);
		return;
	}

	CDialogMove::OnOK();
}

BOOL CAssessmentParamSTLNRDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.nFormula = m_nFomula;
	//m_Data.nReqRouteAvailNum = m_cmbReqRouteAvail.GetCurSel();
	if (!CDlgUtil::CobxGetItemDataByCurSel(m_cmbReqRouteAvail, (DWORD&)m_Data.nReqRouteAvailNum)) m_Data.nReqRouteAvailNum = 0;

	m_Data.dConditionFactor = m_edtCondFactor.GetEditValue();
	m_Data.nGamma_m = m_nGamma_m;
	m_Data.nSpanType = m_nSpanType;

	m_Data.dk2 = m_edtk2.GetEditValue();
	m_Data.dk3 = m_edtk3.GetEditValue();
	m_Data.dUframeS = m_edtUframeL.GetEditValue();

	m_Data.bEqA4 = m_bEqA4;

	m_Data.bUserInput = m_bUser;

	m_Data.bUlsFlexure = m_bUlsFlexure;
	m_Data.bUlsShear = m_bUlsShear;
	m_Data.bUlsLgShear = m_bUlsLgShear;
	m_Data.bUlsIntBearStiff = m_bUlsintBearStiff;

	return TRUE;
}

void CAssessmentParamSTLNRDlg::Data2Dlg()
{
	m_nFomula = m_Data.nFormula;
	//m_cmbReqRouteAvail.SetCurSel(m_Data.nReqRouteAvailNum);
	CDlgUtil::CobxSetCurSelItemData(m_cmbReqRouteAvail, (DWORD&)m_Data.nReqRouteAvailNum);

	m_edtCondFactor.SetEditUnit(m_Data.dConditionFactor);
	m_nGamma_m = m_Data.nGamma_m;
	m_nSpanType = m_Data.nSpanType;
	m_edtk2.SetEditUnit(m_Data.dk2);
	m_edtk3.SetEditUnit(m_Data.dk3);
	m_edtUframeL.SetEditUnit(m_Data.dUframeS);

	m_bEqA4 = m_Data.bEqA4;

	m_bUser = m_Data.bUserInput;

	m_bUlsFlexure = m_Data.bUlsFlexure;
	m_bUlsShear = m_Data.bUlsShear;
	m_bUlsLgShear = m_Data.bUlsLgShear;
	m_bUlsintBearStiff = m_Data.bUlsIntBearStiff;

	UpdateData(FALSE);
}

void CAssessmentParamSTLNRDlg::OnChkUser()
{
	EnableCtrl();
}

void CAssessmentParamSTLNRDlg::OnUser()
{
	CAssessmentParamUserSTLNRDlg dlg;
	dlg.SetData(m_Data.Factors);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_Data.Factors);
	}
}

void CAssessmentParamSTLNRDlg::OnCancel()
{
	CDialogMove::OnCancel();
}