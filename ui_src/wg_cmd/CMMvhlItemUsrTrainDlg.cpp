// CMMvhlItemUsrTrainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrTrainDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrTrainDlg dialog


CCMMvhlItemUsrTrainDlg::CCMMvhlItemUsrTrainDlg(T_MVHL_D  *pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMMvhlItemUsrTrainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrTrainDlg)
	//}}AFX_DATA_INIT

	m_pData=NULL;
	ASSERT(pData);

	m_pData = pData;

	m_pMainData.Initialize();
	m_pHeavyData.Initialize();

	// set main data to heavy data for grid setting...	
	for(int i=0; i< D_MVHL_NUMLOAD; i++)
	{
		m_pMainData.dPointLoad[i] = m_pData->dPointLoad[i];
		m_pMainData.dPointDistance[i] = m_pData->dPointDistance[i];
		m_pHeavyData.dPointLoad[i] = m_pData->dPointLoad2[i];
		m_pHeavyData.dPointDistance[i] = m_pData->dPointDistance2[i];
	}
}


void CCMMvhlItemUsrTrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrTrainDlg)
	DDX_Control(pDX, IDC_CMD_FD_UNIT, m_unitFD);
	DDX_Control(pDX, IDC_CMD_DD_UNIT, m_unitDD);
	DDX_Control(pDX, IDC_CMD_BD_UNIT, m_unitBD);
	DDX_Control(pDX, IDC_CMD_FD_EDT, m_editFD);
	DDX_Control(pDX, IDC_CMD_DD_EDT, m_editDD);
	DDX_Control(pDX, IDC_CMD_BD_EDT, m_editBD);
	DDX_Control(pDX, IDC_CMD_MVLD_TLGRID, m_wndMainGrid);
	DDX_Control(pDX, IDC_CMD_MVLD_TLGRID2, m_wndHeavyGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvhlItemUsrTrainDlg, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrTrainDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrTrainDlg message handlers

BOOL CCMMvhlItemUsrTrainDlg::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();

	// unit 초기화 	
	m_unitFD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dFD);
	m_unitDD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDD);
	m_unitBD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dBD);
	m_editFD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dFD);
	m_editDD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDD);
	m_editBD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dBD);
	
	// Main Vehicle grid 초기화 
	m_wndMainGrid.Initialize(&m_pMainData);
	// always show vertical scrollbar
	m_wndMainGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	// Heavy Vehicle grid 초기화 
	m_wndHeavyGrid.Initialize(&m_pHeavyData);
	// always show vertical scrollbar
	m_wndHeavyGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
		
	// data initialize
	SetData2Dlg();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMMvhlItemUsrTrainDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		pMsg->wParam=9;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)
		pMsg->wParam=VK_TAB;	
	return CInternationalDlg::PreTranslateMessage(pMsg);
}

void	CCMMvhlItemUsrTrainDlg::SetData2Dlg()
{
	ASSERT(m_pData); if(!m_pData) return;

	m_wndMainGrid.MakeItemEx();
	m_wndHeavyGrid.MakeItemEx();

	m_editFD.SetEditUnit(m_pData->dFD);
	m_editDD.SetEditUnit(m_pData->dDD);
	m_editBD.SetEditUnit(m_pData->dBD);

	UpdateData(FALSE);
}

BOOL	CCMMvhlItemUsrTrainDlg::SetDlg2Data()
{
	ASSERT(m_pData); if(!m_pData) return FALSE;

	// 만약 편집중이면 편집중인 데이타 저장
	if (!m_wndMainGrid.OnTerminate(D_TB_SAVE_ALWAYS)) return FALSE;
	if (!m_wndHeavyGrid.OnTerminate(D_TB_SAVE_ALWAYS)) return FALSE;

	UpdateData(TRUE);

	// set each grid data to returning data ...
	for(int i=0; i< D_MVHL_NUMLOAD; i++)
	{
		m_pData->dPointLoad[i] = m_pMainData.dPointLoad[i];
		m_pData->dPointDistance[i] = m_pMainData.dPointDistance[i];
		m_pData->dPointLoad2[i] = m_pHeavyData.dPointLoad[i];
		m_pData->dPointDistance2[i] = m_pHeavyData.dPointDistance[i];
	}

	m_pData->dFD = m_editFD.GetEditValue();
	m_pData->dDD = m_editDD.GetEditValue();
	m_pData->dBD = m_editBD.GetEditValue();

	return TRUE;
}
