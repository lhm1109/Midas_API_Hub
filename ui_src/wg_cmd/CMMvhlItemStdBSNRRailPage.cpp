// CMMvhlItemStdBSNRRailPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdBSNRRailPage.h"
#include "CMMvhlItemStdBSDlg.h"
#include "CMMvhlStdEuroBSNewTempForRefactor.h"

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
// CCMMvhlItemStdBSNRRailPage dialog

CCMMvhlItemStdBSNRRailPage::CCMMvhlItemStdBSNRRailPage(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemStdBSNRRailPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdBSNRRailPage)
	m_pParent = (CCMMvhlItemStdBSDlg*) pParent;

	m_nDynFact = 1;
	m_bLongiDist = FALSE;
	m_bEccenVertLoad = FALSE;
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;
}

void CCMMvhlItemStdBSNRRailPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdBSNRRailPage)
	DDX_Control(pDX, IDC_CMD_BS_RA1_GRID, m_wndGrid1);
	DDX_Control(pDX, IDC_CMD_BS_RA1_GRID2, m_wndGrid2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_RA1_W1_EDIT, m_edtW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_RA1_W1_UNIT, m_untW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_RA1_D1_EDIT, m_edtD1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_RA1_D1_UNIT, m_untD1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_RA1_D2_EDIT, m_edtD2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_RA1_D2_UNIT, m_untD2);
	DDX_Radio  (pDX, IDC_RAIL_DYNAMIC_FACTOR_STATIC_RDO, m_nDynFact);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_RA1_VELOCITY_EDIT, m_edtTrainSpeed);
	DDX_Check  (pDX, IDC_CMD_MVHL_BS_LONGI_DIST_CHK, m_bLongiDist);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_DIST_POINT_EDT, m_edtDistPoint);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_DIST_POINT_UNT, m_untDistPoint);
	DDX_Check  (pDX, IDC_CMD_MVHL_BS_ECCEN_VERT_LOAD_CHK, m_bEccenVertLoad);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ECCEN_VERT_LOAD_EDT, m_edtEccenVertLoad);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ECCEN_VERT_LOAD_UNT, m_untEccenVertLoad);
	//}}AFX_DATA_MAP
}

BOOL CCMMvhlItemStdBSNRRailPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 13)
		pMsg->wParam = 9;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

void CCMMvhlItemStdBSNRRailPage::SetData2Dlg()
{
	T_MVHL_D MvhlD = *m_pData;
	m_wndGrid1.SetDataSource(&MvhlD,this,0,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(MvhlD,0), m_nLoadType);
	m_wndGrid2.SetDataSource(&MvhlD,this,1,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(MvhlD,1), m_nLoadType);

	m_edtW1.SetEditUnit(m_pData->dRA1dW);
	m_edtD1.SetEditUnit(m_pData->dRA1dTail);
	m_edtD2.SetEditUnit(m_pData->dRA1dL);

	m_nDynFact = m_pData->nRailDynFacType;
	m_edtTrainSpeed.SetEditUnit(m_pData->dTrainSpeed);

	m_bLongiDist = m_pData->bLongiDist;
	m_edtDistPoint.SetEditUnit(m_pData->dRailSupPoints);

	m_bEccenVertLoad = m_pData->bEccenVertLoad;
	m_edtEccenVertLoad.SetEditUnit(m_pData->dEccenVertLoad);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdBSNRRailPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_wndGrid1.SaveData();
	m_wndGrid2.SaveData();

	m_pData->dRA1dW = m_edtW1.GetEditValue();
	m_pData->dRA1dTail = m_edtD1.GetEditValue();
	m_pData->dRA1dL = m_edtD2.GetEditValue();

	m_pData->nRailDynFacType = m_nDynFact;
	m_pData->dTrainSpeed = m_edtTrainSpeed.GetEditValue();

	m_pData->bLongiDist = m_bLongiDist;
	m_pData->dRailSupPoints = m_edtDistPoint.GetEditValue();

	m_pData->bEccenVertLoad = m_bEccenVertLoad;
	m_pData->dEccenVertLoad = m_edtEccenVertLoad.GetEditValue();

	return TRUE;
}

void CCMMvhlItemStdBSNRRailPage::EnableDisableControls()
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

void CCMMvhlItemStdBSNRRailPage::SetLoadType(int nLoadType)
{
	m_nLoadType = nLoadType;
}

void CCMMvhlItemStdBSNRRailPage::InitUnit()
{
	m_edtW1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dRA1dW);
	m_untW1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dRA1dW);
	m_edtD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dRA1dTail);
	m_untD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dRA1dTail);
	m_edtD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dRA1dL);
	m_untD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dRA1dL);

	m_edtTrainSpeed.SetUnitType(D_UNITSYS_NONE);

	m_edtDistPoint.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDistPoint.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtEccenVertLoad.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untEccenVertLoad.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemStdBSNRRailPage::InitGrid()
{
	m_wndGrid1.InitGrid(153);
	m_wndGrid2.InitGrid(153);
}


BEGIN_MESSAGE_MAP(CCMMvhlItemStdBSNRRailPage, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemStdBSNRRailPage)
	ON_BN_CLICKED(IDC_RAIL_DYNAMIC_FACTOR_STATIC_RDO, OnDynFactRdo)
	ON_BN_CLICKED(IDC_RAIL_DYNAMIC_FACTOR_DYN_RDO, OnDynFactRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_LONGI_DIST_CHK, OnLongiDistChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_ECCEN_VERT_LOAD_CHK, OnEccenVertLoadChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSNRRailPage message handlers
BOOL CCMMvhlItemStdBSNRRailPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitUnit();
	InitGrid();

	SetData2Dlg();

	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdBSNRRailPage::OnDynFactRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemStdBSNRRailPage::OnLongiDistChk()
{
	EnableDisableControls();
}

void CCMMvhlItemStdBSNRRailPage::OnEccenVertLoadChk()
{
	EnableDisableControls();
}