// CMMvhlItemStdBSNRWagonPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdBSNRWagonPage.h"
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
// CCMMvhlItemStdBSNRWagonPage dialog

CCMMvhlItemStdBSNRWagonPage::CCMMvhlItemStdBSNRWagonPage(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemStdBSNRWagonPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdBSNRWagonPage)
	m_pParent = (CCMMvhlItemStdBSDlg*) pParent;

	m_nLoadCase = 0;
	m_nDynFact = 1;
	m_bLongiDist = FALSE;
	m_bEccenVertLoad = FALSE;
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;
}

void CCMMvhlItemStdBSNRWagonPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdBSNRWagonPage)
	DDX_Control(pDX, IDC_CMD_BS_RA1_GRID, m_wndGrid);
	DDX_Radio  (pDX, IDC_ALLMODEL1_CASE_SINGLE_RDO, m_nLoadCase);
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

BOOL CCMMvhlItemStdBSNRWagonPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 13)
		pMsg->wParam = 9;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

void CCMMvhlItemStdBSNRWagonPage::SetData2Dlg()
{
	T_MVHL_D MvhlD = *m_pData;
	m_wndGrid.SetDataSource(&MvhlD,this,0,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(MvhlD,0), m_nLoadType);

	m_nLoadCase = m_pData->nLM1Case;
	m_nDynFact = m_pData->nRailDynFacType;
	m_edtTrainSpeed.SetEditUnit(m_pData->dTrainSpeed);

	m_bLongiDist = m_pData->bLongiDist;
	m_edtDistPoint.SetEditUnit(m_pData->dRailSupPoints);

	m_bEccenVertLoad = m_pData->bEccenVertLoad;
	m_edtEccenVertLoad.SetEditUnit(m_pData->dEccenVertLoad);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdBSNRWagonPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_wndGrid.SaveData();

	m_pData->nLM1Case = m_nLoadCase;
	m_pData->nRailDynFacType = m_nDynFact;
	m_pData->dTrainSpeed = m_edtTrainSpeed.GetEditValue();

	m_pData->bLongiDist = m_bLongiDist;
	m_pData->dRailSupPoints = m_edtDistPoint.GetEditValue();

	m_pData->bEccenVertLoad = m_bEccenVertLoad;
	m_pData->dEccenVertLoad = m_edtEccenVertLoad.GetEditValue();

	return TRUE;
}

void CCMMvhlItemStdBSNRWagonPage::EnableDisableControls()
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

void CCMMvhlItemStdBSNRWagonPage::SetLoadType(int nLoadType)
{
	m_nLoadType = nLoadType;
}

void CCMMvhlItemStdBSNRWagonPage::InitUnit()
{
	m_edtTrainSpeed.SetUnitType(D_UNITSYS_NONE);

	m_edtDistPoint.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDistPoint.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtEccenVertLoad.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untEccenVertLoad.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemStdBSNRWagonPage::InitGrid()
{
	m_wndGrid.InitGrid(153);
}


BEGIN_MESSAGE_MAP(CCMMvhlItemStdBSNRWagonPage, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemStdBSNRWagonPage)
	ON_BN_CLICKED(IDC_RAIL_DYNAMIC_FACTOR_STATIC_RDO, OnDynFactRdo)
	ON_BN_CLICKED(IDC_RAIL_DYNAMIC_FACTOR_DYN_RDO, OnDynFactRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_LONGI_DIST_CHK, OnLongiDistChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_ECCEN_VERT_LOAD_CHK, OnEccenVertLoadChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSNRWagonPage message handlers
BOOL CCMMvhlItemStdBSNRWagonPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitUnit();
	InitGrid();

	SetData2Dlg();

	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdBSNRWagonPage::OnDynFactRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemStdBSNRWagonPage::OnLongiDistChk()
{
	EnableDisableControls();
}

void CCMMvhlItemStdBSNRWagonPage::OnEccenVertLoadChk()
{
	EnableDisableControls();
}