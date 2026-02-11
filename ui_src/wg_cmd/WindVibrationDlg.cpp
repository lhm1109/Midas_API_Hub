// WindVibrationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindVibrationDlg.h"

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindVibrationDlg dialog
CWindVibrationZfDlg::CWindVibrationZfDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindVibrationZfDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWindVibrationZfDlg)
	//}}AFX_DATA_INIT
}


void CWindVibrationZfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindVibrationDlg)
	DDX_Control(pDX, IDC_ETC_WIND_VIBRATION_ZF_BMP, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWindVibrationZfDlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindVibrationZfDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindVibrationDlg message handlers

BOOL CWindVibrationZfDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_vibration_reference.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWindVibrationZfDlg::OnCmdClose() 
{
	CDialogMove::OnClose();
}

/////////////////////////////////////////////////////////////////////////////
// CWindVibrationDlg dialog
CWindVibrationDlg::CWindVibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindVibrationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWindVibrationDlg)
	m_dBreadthX = 0.0;
	m_dBreadthY = 0.0;
	m_dFreqX = 0.0;
	m_dFreqY = 0.0;
	m_dFreqT = 0.0;
	m_dMassX = 0.0;
	m_dMassY = 0.0;
	m_dMoment = 0.0;
	m_dZf = 0.0;
	//}}AFX_DATA_INIT
}


void CWindVibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindVibrationDlg)
	DDX_Control(pDX, IDC_ETC_WIND_EDIT1, m_edtBreadthX);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT2, m_edtBreadthY);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT3, m_edtFreqX);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT4, m_edtFreqY);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT5, m_edtFreqT);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT6, m_edtMassX);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT7, m_edtMassY);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT8, m_edtMoment);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT9, m_edtZf);
	DDX_Control(pDX, IDC_ETC_WIND_UNIT1, m_wndBreadthXUnit);
	DDX_Control(pDX, IDC_ETC_WIND_UNIT2, m_wndBreadthYUnit);
	DDX_Control(pDX, IDC_ETC_WIND_UNIT6, m_wndMassXUnit);
	DDX_Control(pDX, IDC_ETC_WIND_UNIT7, m_wndMassYUnit);
	DDX_Control(pDX, IDC_ETC_WIND_UNIT8, m_wndMoment);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWindVibrationDlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindVibrationDlg)
	ON_BN_CLICKED(IDC_ETC_WIND_ZF_BTN, OnCmdZfBtn)
	ON_BN_CLICKED(IDC_ETC_WIND_IMPORT_BTN, OnCmdImportBtn)
	ON_BN_CLICKED(IDC_ETC_WIND_OK, OnCmdOk)
	ON_BN_CLICKED(IDC_ETC_WIND_CANCEL, OnCmdCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindVibrationDlg message handlers

BOOL CWindVibrationDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);

	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_TITLE_STC3, m_nResponseCheck==1 || (m_nBuildingType==1 && m_nAcrossCheck==1));
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_TITLE_STC4, m_nResponseCheck==1 || (m_nBuildingType==1 && m_nAcrossCheck==1));
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_TITLE_STC5, m_nTorsionalCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_TITLE_STC6, m_nResponseCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_TITLE_STC7, m_nResponseCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_TITLE_STC8, m_nResponseCheck==1 && m_nTorsionalCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_TITLE_STC9, (m_nBuildingType==0&&m_nResponseCheck) || m_nBuildingType==1);

	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_EDIT3, m_nResponseCheck==1 || (m_nBuildingType==1 && m_nAcrossCheck==1));
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_EDIT4, m_nResponseCheck==1 || (m_nBuildingType==1 && m_nAcrossCheck==1));
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_EDIT5, m_nTorsionalCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_EDIT6, m_nResponseCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_EDIT7, m_nResponseCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_EDIT8, m_nResponseCheck==1 && m_nTorsionalCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_EDIT9, (m_nBuildingType==0&&m_nResponseCheck) || m_nBuildingType==1);

	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_UNIT3, m_nResponseCheck==1 || (m_nBuildingType==1 && m_nAcrossCheck==1));
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_UNIT4, m_nResponseCheck==1 || (m_nBuildingType==1 && m_nAcrossCheck==1));
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_UNIT5, m_nTorsionalCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_UNIT6, m_nResponseCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_UNIT7, m_nResponseCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_UNIT8, m_nResponseCheck==1 && m_nTorsionalCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_ZF_BTN, (m_nBuildingType==0&&m_nResponseCheck) || m_nBuildingType==1);
	
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_IMPORT_BTN, ((m_nBuildingType==0&&m_nResponseCheck) || m_nBuildingType==1) && CDBDoc::GetDocPoint()->m_pPostCtrl->IsPostEnable());

	InitialData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWindVibrationDlg::InitialData()
{
	m_edtBreadthX.SetEditUnit(m_dBreadthX);
	m_edtBreadthY.SetEditUnit(m_dBreadthY);
	m_edtFreqX.SetEditUnit(m_dFreqX);
	m_edtFreqY.SetEditUnit(m_dFreqY);
	m_edtFreqT.SetEditUnit(m_dFreqT);
	m_edtMassX.SetEditUnit(m_dMassX);
	m_edtMassY.SetEditUnit(m_dMassY);
	m_edtMoment.SetEditUnit(m_dMoment);
	m_edtZf.SetEditUnit(m_dZf);
	
	m_wndBreadthXUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndBreadthYUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndMassXUnit.SetUnitType(D_UNITSYS_BASE_MASS);
	m_wndMassYUnit.SetUnitType(D_UNITSYS_BASE_MASS);
	m_wndMoment.SetUnitType(D_UNITSYS_BASE_ROMASS);

	UpdateData(FALSE);
}

void CWindVibrationDlg::OnCmdZfBtn() 
{
	CWindVibrationZfDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CWindVibrationDlg::OnCmdImportBtn() 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pPostCtrl->IsPostEnable()) return;

	int nModeNum = pDoc->m_pPostCtrl->GetNumEigvPost();
	T_MPAM_D MpamD;
	int nMode_MaxMassX = 0;
	int nMode_MaxMassY = 0;
	int nMode_MaxMassT = 0;
	double dMaxMass_x = 0.0;
	double dMaxMass_y = 0.0;
	double dMaxMass_t = 0.0;
	for (int i = 0; i < nModeNum; i++)
	{
		if(!pDoc->m_pPostCtrl->GetEiMpam(i+1, MpamD)) { ASSERT(0); continue; }

		if(dMaxMass_x<MpamD.dblMpam[0]) { dMaxMass_x = MpamD.dblMpam[0]; nMode_MaxMassX = i; }
		if(dMaxMass_y<MpamD.dblMpam[1]) { dMaxMass_y = MpamD.dblMpam[1]; nMode_MaxMassY = i; }
		if(dMaxMass_t<MpamD.dblMpam[5]) { dMaxMass_t = MpamD.dblMpam[5]; nMode_MaxMassT = i; }
	}

	T_FREQ_D FreqD;
//  T_EMMR_D EmmrD;

	if(!pDoc->m_pPostCtrl->GetEiFreq(nMode_MaxMassX+1, FreqD)) { ASSERT(0); FreqD.Initialize(); }
	//if(!pDoc->m_pPostCtrl->GetEiEmmr(nMode_MaxMassX+1, EmmrD)) { ASSERT(0); EmmrD.Initialize(); }
	m_dFreqX = FreqD.dblFreq[1];
	//m_dMassX = EmmrD.dblEmmrMass[0];

	if(!pDoc->m_pPostCtrl->GetEiFreq(nMode_MaxMassY+1, FreqD)) { ASSERT(0); FreqD.Initialize(); }
	//if(!pDoc->m_pPostCtrl->GetEiEmmr(nMode_MaxMassY+1, EmmrD)) { ASSERT(0); EmmrD.Initialize(); }
	m_dFreqY = FreqD.dblFreq[1];
	//m_dMassY = EmmrD.dblEmmrMass[1];

	if(!pDoc->m_pPostCtrl->GetEiFreq(nMode_MaxMassT+1, FreqD)) { ASSERT(0); FreqD.Initialize(); }
	//if(!pDoc->m_pPostCtrl->GetEiEmmr(nMode_MaxMassT+1, EmmrD)) { ASSERT(0); EmmrD.Initialize(); }
	m_dFreqT = FreqD.dblFreq[1];
	//m_dMoment = EmmrD.dblEmmrMass[5];

	m_edtFreqX.SetEditUnit(m_dFreqX);
	m_edtFreqY.SetEditUnit(m_dFreqY);
	m_edtFreqT.SetEditUnit(m_dFreqT);
//   m_edtMassX.SetEditUnit(m_dMassX);
//   m_edtMassY.SetEditUnit(m_dMassY);
//   m_edtMoment.SetEditUnit(m_dMoment);
}

void CWindVibrationDlg::OnCmdOk() 
{
	UpdateData(TRUE);
	
	m_dBreadthX	= m_edtBreadthX.GetEditValue();
	m_dBreadthY	= m_edtBreadthY.GetEditValue();
	m_dFreqX		= m_edtFreqX.GetEditValue();
	m_dFreqY		= m_edtFreqY.GetEditValue();
	m_dFreqT		= m_edtFreqT.GetEditValue();
	m_dMassX		= m_edtMassX.GetEditValue();
	m_dMassY		= m_edtMassY.GetEditValue();
	m_dMoment		= m_edtMoment.GetEditValue();
	m_dZf				= m_edtZf.GetEditValue();

	CDialogMove::OnOK();
}

void CWindVibrationDlg::OnCmdCancel() 
{
	CDialogMove::OnCancel();		
}
