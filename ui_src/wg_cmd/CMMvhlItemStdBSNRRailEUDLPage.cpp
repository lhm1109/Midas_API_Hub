// CMMvhlItemStdBSNRRailEUDLPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdBSNRRailEUDLPage.h"
#include "CMMvhlItemStdBSDlg.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CInternationalDlg
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSNRRailEUDLPage dialog

CCMMvhlItemStdBSNRRailEUDLPage::CCMMvhlItemStdBSNRRailEUDLPage(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemStdBSNRRailEUDLPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdBSNRRailEUDLPage)
	m_pParent = (CCMMvhlItemStdBSDlg*) pParent;

	m_nDynFact = 1;
	//}}AFX_DATA_INIT

	m_bLongiDist = FALSE;
	m_bEccenVertLoad = FALSE;

	m_pData = NULL;
	m_pData = pData;
}

void CCMMvhlItemStdBSNRRailEUDLPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdBSNRRailEUDLPage)
	DDX_Control(pDX, IDC_CMD_MVHL_BS_RA1_EUDL_EDIT, m_edtW);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_RA1_EUDL_UNIT, m_untW);
	DDX_Radio  (pDX, IDC_RAIL_DYNAMIC_FACTOR_STATIC_RDO, m_nDynFact);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_RA1_VELOCITY_EDIT, m_edtTrainSpeed);
	DDX_Check(pDX, IDC_CMD_MVHL_BS_LONGI_DIST_CHK, m_bLongiDist);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_DIST_POINT_EDT, m_edtDistPoint);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_DIST_POINT_UNT, m_untDistPoint);
	DDX_Check(pDX, IDC_CMD_MVHL_BS_ECCEN_VERT_LOAD_CHK, m_bEccenVertLoad);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ECCEN_VERT_LOAD_EDT, m_edtEccenVertLoad);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ECCEN_VERT_LOAD_UNT, m_untEccenVertLoad);
	//}}AFX_DATA_MAP
}

BOOL CCMMvhlItemStdBSNRRailEUDLPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 13)
		pMsg->wParam = 9;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

void CCMMvhlItemStdBSNRRailEUDLPage::SetData2Dlg()
{
	m_edtW.SetEditUnit(m_pData->dEUDL);
	
	m_nDynFact = m_pData->nRailDynFacType;
	m_edtTrainSpeed.SetEditUnit(m_pData->dTrainSpeed);

	m_bLongiDist = m_pData->bLongiDist;
	m_edtDistPoint.SetEditUnit(m_pData->dRailSupPoints);

	m_bEccenVertLoad = m_pData->bEccenVertLoad;
	m_edtEccenVertLoad.SetEditUnit(m_pData->dEccenVertLoad);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdBSNRRailEUDLPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->dEUDL = m_edtW.GetEditValue();

	m_pData->nRailDynFacType = m_nDynFact;
	m_pData->dTrainSpeed = m_edtTrainSpeed.GetEditValue();

	m_pData->bLongiDist = m_bLongiDist;
	m_pData->dRailSupPoints = m_edtDistPoint.GetEditValue();

	m_pData->bEccenVertLoad = m_bEccenVertLoad;
	m_pData->dEccenVertLoad = m_edtEccenVertLoad.GetEditValue();

	return TRUE;
}

void CCMMvhlItemStdBSNRRailEUDLPage::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_BS_RA1_VELOCITY_STATIC)->EnableWindow(m_nDynFact == 1);
	GetDlgItem(IDC_CMD_MVHL_BS_RA1_VELOCITY_EDIT)->EnableWindow(m_nDynFact == 1);
	GetDlgItem(IDC_CMD_MVHL_BS_RA1_VELOCITY_UNIT)->EnableWindow(m_nDynFact == 1);

	GetDlgItem(IDC_CMD_MVHL_BS_DIST_POINT_STC)->EnableWindow(m_bLongiDist);
	GetDlgItem(IDC_CMD_MVHL_BS_DIST_POINT_EDT)->EnableWindow(m_bLongiDist);
	GetDlgItem(IDC_CMD_MVHL_BS_DIST_POINT_UNT)->EnableWindow(m_bLongiDist);

	GetDlgItem(IDC_CMD_MVHL_BS_ECCEN_VERT_LOAD_EDT)->EnableWindow(m_bEccenVertLoad);
	GetDlgItem(IDC_CMD_MVHL_BS_ECCEN_VERT_LOAD_UNT)->EnableWindow(m_bEccenVertLoad);
}

void CCMMvhlItemStdBSNRRailEUDLPage::InitUnit()
{
	m_edtW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dEUDL);
	m_untW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dEUDL);

	m_edtTrainSpeed.SetUnitType(D_UNITSYS_NONE);

	m_edtDistPoint.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDistPoint.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtEccenVertLoad.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untEccenVertLoad.SetUnitType(D_UNITSYS_BASE_LENGTH);
}


BEGIN_MESSAGE_MAP(CCMMvhlItemStdBSNRRailEUDLPage, CDialog)
	//{{AFX_MSG_MAP(Cv)
	ON_BN_CLICKED(IDC_RAIL_DYNAMIC_FACTOR_STATIC_RDO, OnDynFactRdo)
	ON_BN_CLICKED(IDC_RAIL_DYNAMIC_FACTOR_DYN_RDO, OnDynFactRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_LONGI_DIST_CHK, OnLongiDistChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_ECCEN_VERT_LOAD_CHK, OnEccenVertLoadChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSNRRailEUDLPage message handlers
BOOL CCMMvhlItemStdBSNRRailEUDLPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitUnit();

	SetData2Dlg();

	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdBSNRRailEUDLPage::OnDynFactRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemStdBSNRRailEUDLPage::OnLongiDistChk()
{
	EnableDisableControls();
}

void CCMMvhlItemStdBSNRRailEUDLPage::OnEccenVertLoadChk()
{
	EnableDisableControls();
}