// CMMvldItemYLoadJPDlg.cpp : implementation file
// 2002 .7. 19   by TAE
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemYLoadJPDlg.h"
#include "CMMvldItemGrid.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemYLoadJPDlg dialog

CCMMvldItemYLoadJPDlg::CCMMvldItemYLoadJPDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMMvldItemYLoadJPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemYLoadJPDlg)
	m_bLoadCheck = FALSE;
	m_dRatio = 0.0;
	//}}AFX_DATA_INIT

	m_pGrid = NULL;
	m_pGrid = new CCMvldItemJPGrid();

	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();
}

CCMMvldItemYLoadJPDlg::~CCMMvldItemYLoadJPDlg()
{
	if(m_pGrid != NULL) { delete m_pGrid; m_pGrid = NULL; }
}

void CCMMvldItemYLoadJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemYLoadJPDlg)
	DDX_Control(pDX, IDC_CMD_MVLD_LOAD_MOVEMENT_UNIT, m_unitMoveDist);
	DDX_Control(pDX, IDC_CMD_MVLD_LOAD_MOVEMENT, m_editMoveDist);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_L1_UNIT, m_unitP2L1);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_LARGE_UNIT, m_unitGrLarge);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_LARGE_EDIT, m_editGrLarge);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_SMALL_UNIT, m_unitP2Small);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_SMALL_EDIT, m_editP2Small);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_MEDIUM_UNIT, m_unitP2Medium);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_MEDIUM_EDIT2, m_editP2Medium2);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_MEDIUM_EDIT1, m_editP2Medium1);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_LARGE_UNIT, m_unitP2Large);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_LARGE_EDIT, m_editP2Large);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_L2_EDIT, m_editP2L2);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_L1_EDIT, m_editP2L1);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_L_UNIT, m_unitP2L);
	DDX_Control(pDX, IDC_CMD_MVLD_LOAD_LENGTH_UNIT, m_unitLength);
	DDX_Control(pDX, IDC_CMD_MVLD_LOAD_LENGTH_EDIT, m_editLength);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_SMALL_UNIT, m_unitGrSmall);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_SMALL_EDIT, m_editGrSmall);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_MEDIUM_UNIT, m_unitGrMedium);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_MEDIUM_EDIT2, m_editGrMedium2);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_MEDIUM_EDIT1, m_editGrMedium1);
	DDX_Check(pDX, IDC_CMD_MVLD_LOAD_CHECK, m_bLoadCheck);
	DDX_Text(pDX, IDC_CMD_MVLD_RATIO_EDIT, m_dRatio);
	DDX_Control(pDX, IDC_CMD_MVLD_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvldItemYLoadJPDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCMMvldItemYLoadJPDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemYLoadJPDlg message handlers


/////////////////////////////////////////////////////////////////////////////
//
// External Data Control Function
//

BOOL CCMMvldItemYLoadJPDlg::GetCurDlgData(T_MVLDjp_D* pData)
{
	ASSERT(pData);

	UpdateData(TRUE);

	pData->MLoad.bLoading = m_bLoadCheck;
	pData->MLoad.dMovingDistance = m_editMoveDist.GetEditValue();
	pData->MLoad.dTotalLength = m_editLength.GetEditValue();
	pData->MLoad.dSubMainRatio = m_dRatio;
	pData->MLoad.P2_dL1 = m_editP2L1.GetEditValue();
	pData->MLoad.P2_dL2 = m_editP2L2.GetEditValue();
	pData->MLoad.P2_dP2L1 = m_editP2Small.GetEditValue();
	pData->MLoad.P2_dCrowdL1L2_1 = m_editP2Medium1.GetEditValue();
	pData->MLoad.P2_dCrowdL1L2_2 = m_editP2Medium2.GetEditValue();
	pData->MLoad.P2_dP2L2 = m_editP2Large.GetEditValue();
	pData->MLoad.P2_dCrowdL1 = m_editGrSmall.GetEditValue();
	pData->MLoad.P2_dCrowdL1L2_1 = m_editGrMedium1.GetEditValue();
	pData->MLoad.P2_dCrowdL1L2_2 = m_editGrMedium2.GetEditValue();
	pData->MLoad.P2_dCrowdL2 = m_editGrLarge.GetEditValue();

	// *^^*
	// 그리드 데이터도 가져와야 함..
	for(int i=0; i<T_MVLDjp_NUM_MVLDjp_MLOAD; i++)
	{
		pData->MLoad.Load[i].dLocation = m_Data.MLoad.Load[i].dLocation;
		pData->MLoad.Load[i].dLoadValue = m_Data.MLoad.Load[i].dLoadValue;
	}
	pData->MLoad.nLoadNum =  m_pGrid->GetRowCount() - 1;

	return TRUE;
}

void CCMMvldItemYLoadJPDlg::SetCurDlgData(T_MVLDjp_D* pData)
{
	ASSERT(pData);
	m_pDoc->m_pAttrCtrl->InitializeMvldjp(m_Data);
	m_Data = (T_MVLDjp_D)(*pData);
}



/////////////////////////////////////////////////////////////////////////////
//
// Initialize
//
void CCMMvldItemYLoadJPDlg::InitControlUnit()
{
	m_editMoveDist.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.dMovingDistance);
	m_unitMoveDist.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.dMovingDistance);
	m_unitLength.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P1_dLength);
	m_editLength.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P1_dLength);
	m_unitGrSmall.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL1);
	m_editGrSmall.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL1);
	m_unitGrMedium.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL1L2_2);
	m_editGrMedium1.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL1L2_1);
	m_editGrMedium2.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL1L2_2);
	m_unitGrLarge.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL2);
	m_editGrLarge.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL2);
	m_unitP2Small.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L1);
	m_editP2Small.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L1);
	m_unitP2Medium.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L1L2_2);
	m_editP2Medium1.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L1L2_1);
	m_editP2Medium2.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L1L2_2);
	m_unitP2Large.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L2);
	m_editP2Large.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L2);
	m_editP2L2.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dL2);
	m_editP2L1.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dL1);
	m_unitP2L.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dL2);
	m_unitP2L1.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dL1);
}

void CCMMvldItemYLoadJPDlg::SetData2Dlg()
{
	m_bLoadCheck = m_Data.MLoad.bLoading;
	m_editMoveDist.SetEditUnit(m_Data.MLoad.dMovingDistance);
	m_editLength.SetEditUnit(m_Data.MLoad.dTotalLength);
	m_dRatio = m_Data.MLoad.dSubMainRatio;
	m_editP2L1.SetEditUnit(m_Data.MLoad.P2_dL1);
	m_editP2L2.SetEditUnit(m_Data.MLoad.P2_dL2);
	m_editP2Small.SetEditUnit(m_Data.MLoad.P2_dP2L1);
	m_editP2Medium1.SetEditUnit(m_Data.MLoad.P2_dCrowdL1L2_1);
	m_editP2Medium2.SetEditUnit(m_Data.MLoad.P2_dCrowdL1L2_2);
	m_editP2Large.SetEditUnit(m_Data.MLoad.P2_dP2L2);
	m_editGrSmall.SetEditUnit(m_Data.MLoad.P2_dCrowdL1);
	m_editGrMedium1.SetEditUnit(m_Data.MLoad.P2_dCrowdL1L2_1);
	m_editGrMedium2.SetEditUnit(m_Data.MLoad.P2_dCrowdL1L2_2);
	m_editGrLarge.SetEditUnit(m_Data.MLoad.P2_dCrowdL2);

	// *^^*
	// Grid도 초기화 해야함

	UpdateData(FALSE);
}

BOOL CCMMvldItemYLoadJPDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_pGrid != NULL) { delete m_pGrid; m_pGrid = NULL; }
	m_pGrid->Initialize(&m_Data);

	// 그리드 초기화
	for(int i=0; i<T_MVLDjp_NUM_MVLDjp_MLOAD; i++)
	{
		if(m_Data.MLoad.Load[i].dLoadValue == 0.0) break;

		m_pGrid->InsertRecordByKey(i+1, m_Data.MLoad.Load[i]);
	}

	InitControlUnit();

	SetData2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
//
// DestroyWindow
//
BOOL CCMMvldItemYLoadJPDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_Data.Initialize();
	
	return CChildDialog::DestroyWindow();
}
