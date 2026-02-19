// CDgnSeisAnchorRebarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorRebarDlg.h"

#include "..\\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorRebarDlg dialog

CDgnSeisAnchorRebarDlg::CDgnSeisAnchorRebarDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisAnchorRebarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorRebarDlg)	
	//}}AFX_DATA_INIT

	m_bTension = FALSE;
	m_bShear = FALSE;
}

void CDgnSeisAnchorRebarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorRebarDlg)
	DDX_Check  (pDX, IDC_DGN_SEIS_ANCHOR_REBAR_TENS_CHK,      m_bTension);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_FYAT_EDT,      m_edtFyat);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_FYAT_UNT,      m_untFyat);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_DSAT_EDT,      m_edtdsat);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_DSAT_UNT,      m_untdsat);
	DDX_Check  (pDX, IDC_DGN_SEIS_ANCHOR_REBAR_SHEAR_CHK,     m_bShear);	
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_FYAS_L_EDT,    m_edtFyasL);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_FYAS_L_UNT,    m_untFyasL);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_DSAS_L_EDT,    m_edtdsasL);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_DSAS_L_UNT,    m_untdsasL);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_NAS_L_EDT,     m_edtNasL);	
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_FYAS_T_EDT,    m_edtFyasT);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_FYAS_T_UNT,    m_untFyasT);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_DSAS_T_EDT,    m_edtdsasT);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_DSAS_T_UNT,    m_untdsasT);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_NAS_T_EDT,     m_edtNasT);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_REBAR_BMP, m_wndPicture);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorRebarDlg::InitUnit()
{
	m_edtFyat.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dFyat);
	m_untFyat.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dFyat);
	m_edtdsat.SetUnitType(CUnitCtrl::m_ANEV_UNIT.ddsat);
	m_untdsat.SetUnitType(CUnitCtrl::m_ANEV_UNIT.ddsat);
	m_edtFyasL.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dFyasL);
	m_untFyasL.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dFyasL);
	m_edtdsasL.SetUnitType(CUnitCtrl::m_ANEV_UNIT.ddsasL);
	m_untdsasL.SetUnitType(CUnitCtrl::m_ANEV_UNIT.ddsasL);
	m_edtNasL.SetUnitType(D_UNITSYS_NONE);
	m_edtFyasT.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dFyasT);
	m_untFyasT.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dFyasT);
	m_edtdsasT.SetUnitType(CUnitCtrl::m_ANEV_UNIT.ddsasT);
	m_untdsasT.SetUnitType(CUnitCtrl::m_ANEV_UNIT.ddsasT);
	m_edtNasT.SetUnitType(D_UNITSYS_NONE);
}

void CDgnSeisAnchorRebarDlg::EnableDisableControls() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_FYAT_STC)->EnableWindow(m_bTension);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_FYAT_EDT)->EnableWindow(m_bTension);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_FYAT_UNT)->EnableWindow(m_bTension);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_DSAT_STC)->EnableWindow(m_bTension);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_DSAT_EDT)->EnableWindow(m_bTension);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_DSAT_UNT)->EnableWindow(m_bTension);


	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_FYAS_L_STC)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_FYAS_L_EDT)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_FYAS_L_UNT)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_DSAS_L_STC)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_DSAS_L_EDT)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_DSAS_L_UNT)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_NAS_L_STC)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_NAS_L_EDT)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_NAS_L_UNT)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_FYAS_T_STC)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_FYAS_T_EDT)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_FYAS_T_UNT)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_DSAS_T_STC)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_DSAS_T_EDT)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_DSAS_T_UNT)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_NAS_T_STC)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_NAS_T_EDT)->EnableWindow(m_bShear);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_REBAR_NAS_T_UNT)->EnableWindow(m_bShear);

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\Dgn_Seis_Anchor_Rebar.svg"));
}

void CDgnSeisAnchorRebarDlg::Data2Dlg() 
{ 
	m_bTension = m_Data.bTension;
	m_edtFyat.SetEditUnit(m_Data.dFyat);
	m_edtdsat.SetEditUnit(m_Data.ddsat);

	m_bShear = m_Data.bShear;
	m_edtFyasL.SetEditUnit(m_Data.dFyasL);
	m_edtdsasL.SetEditUnit(m_Data.ddsasL);
	m_edtNasL.SetEditUnit(m_Data.nNasL);
	m_edtFyasT.SetEditUnit(m_Data.dFyasT);
	m_edtdsasT.SetEditUnit(m_Data.ddsasT);
	m_edtNasT.SetEditUnit(m_Data.nNasT);

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorRebarDlg::Dlg2Data() 
{
	UpdateData(TRUE);

	m_Data.bTension = m_bTension;
	m_Data.dFyat = m_edtFyat.GetEditValue();
	m_Data.ddsat = m_edtdsat.GetEditValue();

	m_Data.bShear = m_bShear;
	m_Data.dFyasL = m_edtFyasL.GetEditValue();
	m_Data.ddsasL = m_edtdsasL.GetEditValue();
	m_Data.nNasL = m_edtNasL.GetEditValue();
	m_Data.dFyasT = m_edtFyasT.GetEditValue();
	m_Data.ddsasT = m_edtdsasT.GetEditValue();
	m_Data.nNasT = m_edtNasT.GetEditValue();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorRebarDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_REBAR_TENS_CHK,   OnDgnSeisAnchorRebarTensionChk)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_REBAR_SHEAR_CHK,  OnDgnSeisAnchorRebarShearChk)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_REBAR_OK_BTN,     OnDgnSeisAnchorRebarOKBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_REBAR_CANCEL_BTN, OnDgnSeisAnchorRebarCancelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorRebarDlg message handlers

BOOL CDgnSeisAnchorRebarDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();

	Data2Dlg();

	EnableDisableControls();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisAnchorRebarDlg::OnDgnSeisAnchorRebarTensionChk()
{
	EnableDisableControls();
}

void CDgnSeisAnchorRebarDlg::OnDgnSeisAnchorRebarShearChk()
{
	EnableDisableControls();
}

void CDgnSeisAnchorRebarDlg::OnDgnSeisAnchorRebarOKBtn()
{
	if(!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CDgnSeisAnchorRebarDlg::OnDgnSeisAnchorRebarCancelBtn()
{
	CDialogMove::OnCancel();
}