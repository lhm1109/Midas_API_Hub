// CMMvldItemLLoadJPDlg.cpp : implementation file
// 2002. 7. 19.   by TAE
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemLLoadJPDlg.h"

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
// CCMMvldItemLLoadJPDlg dialog

CCMMvldItemLLoadJPDlg::CCMMvldItemLLoadJPDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMMvldItemLLoadJPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemLLoadJPDlg)
	m_dRatio = 0.0;
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();

	arMethodCombo.Add(_LS(IDS_CMD_MVLD__Continuous_Lo)); 
	arMethodCombo.Add(_LS(IDS_CMD_MVLD__Continuous_Within_Lo)); 
	arMethodCombo.Add(_LS(IDS_CMD_MVLD__Discontinuous_Within_Lo)); 

	arLengthCombo.Add(_T("6"));
	arLengthCombo.Add(_T("10"));
}


void CCMMvldItemLLoadJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemLLoadJPDlg)
	DDX_Control(pDX, IDC_CMD_MVLD_P2_L1_UNIT, m_unitP2L1);
	DDX_Control(pDX, IDC_CMD_MVLD_UNIT_LOAD_UNIT, m_unitUnitLoad);
	DDX_Control(pDX, IDC_CMD_MVLD_UNIT_LOAD_EDIT, m_editUnitLoad);
	DDX_Control(pDX, IDC_CMD_MVLD_SHEAR_LOAD_UNIT, m_unitShear);
	DDX_Control(pDX, IDC_CMD_MVLD_SHEAR_LOAD_EDIT, m_editShear);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_SMALL_UNIT, m_unitP2Small);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_SMALL_EDIT, m_editP2Small);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_MEDIUM_UNIT, m_unitP2Medium);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_MEDIUM_EDIT2, m_editP2Medium2);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_MEDIUM_EDIT1, m_editP2Medium1);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_LARGE_UNIT, m_unitP2Large);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_SMALL_UNIT, m_unitGrSmall);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_SMALL_EDIT, m_editGrSmall);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_MEDIUM_UNIT, m_unitGrMedium);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_MEDIUM_EDIT2, m_editGrMedium2);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_MEDIUM_EDIT1, m_editGrMedium1);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_LARGE_UNIT, m_unitGrLarge);
	DDX_Control(pDX, IDC_CMD_MVLD_GROUP_LARGE_EDIT, m_editGrLarge);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_LARGE_EDIT, m_editP2Large);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_L2_EDIT, m_editP2L2);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_L1_EDIT, m_editP2L1);
	DDX_Control(pDX, IDC_CMD_MVLD_P2_L_UNIT, m_unitP2L);
	DDX_Control(pDX, IDC_CMD_MVLD_MOMENT_LOAD_UNIT, m_unitMoment);
	DDX_Control(pDX, IDC_CMD_MVLD_MOMENT_LOAD_EDIT, m_editMoment);
	DDX_Control(pDX, IDC_CMD_MVLD_ASSIGN_LENGTH_UNIT, m_unitLength);
	DDX_Control(pDX, IDC_CMD_MVLD_ASSIGN_METHOD_COMBO, m_cboMethod);
	DDX_Control(pDX, IDC_CMD_MVLD_ASSIGN_LENGTH_COMBO, m_cboLength);
	DDX_Text(pDX, IDC_CMD_MVLD_RATIO_EDIT, m_dRatio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvldItemLLoadJPDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCMMvldItemLLoadJPDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemLLoadJPDlg message handlers

/////////////////////////////////////////////////////////////////////////////
//
// External Data Control Function
//
BOOL CCMMvldItemLLoadJPDlg::GetCurDlgData(T_MVLDjp_D* pData)
{
	ASSERT(pData);

	UpdateData(TRUE);

	CString str;
	m_cboLength.GetWindowText(str);
	pData->LLoad.P1_dLength = _tstof(str);

	pData->LLoad.P1_nMethod = m_cboMethod.GetCurSel();
	pData->LLoad.dSubMainRatio = m_dRatio;
	pData->LLoad.P1_dUniformLoadMovement = m_editUnitLoad.GetEditValue();
	pData->LLoad.P1_dMomentCalcLoad = m_editMoment.GetEditValue();
	pData->LLoad.P1_dShearCalcLoad = m_editShear.GetEditValue();
	pData->LLoad.P2_dL1 = m_editP2L1.GetEditValue();
	pData->LLoad.P2_dL2 = m_editP2L2.GetEditValue();
	pData->LLoad.P2_dP2L1 = m_editP2Small.GetEditValue();
	pData->LLoad.P2_dP2L1L2_1 = m_editP2Medium1.GetEditValue();
	pData->LLoad.P2_dP2L1L2_2 = m_editP2Medium2.GetEditValue();
	pData->LLoad.P2_dP2L2 = m_editP2Large.GetEditValue();
	pData->LLoad.P2_dCrowdL1 = m_editGrSmall.GetEditValue();
	pData->LLoad.P2_dCrowdL1L2_1 = m_editGrMedium1.GetEditValue();
	pData->LLoad.P2_dCrowdL1L2_2 = m_editGrMedium2.GetEditValue();
	pData->LLoad.P2_dCrowdL2 = m_editGrLarge.GetEditValue();

	return TRUE;
}

void CCMMvldItemLLoadJPDlg::SetCurDlgData(T_MVLDjp_D* pData)
{
	ASSERT(pData);
	m_pDoc->m_pAttrCtrl->InitializeMvldjp(m_Data);
	m_Data = (*pData);
}

/////////////////////////////////////////////////////////////////////////////
//
// Initialize
//
void CCMMvldItemLLoadJPDlg::InitControlUnit()
{
	m_unitUnitLoad.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P1_dUniformLoadMovement);
	m_editUnitLoad.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P1_dUniformLoadMovement);
	m_unitShear.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P1_dShearCalcLoad);
	m_editShear.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P1_dShearCalcLoad);
	m_unitP2Small.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L1);
	m_editP2Small.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L1);
	m_unitP2Medium.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L1L2_2);
	m_editP2Medium1.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L1L2_1);
	m_editP2Medium2.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L1L2_2);
	m_unitP2Large.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L2);
	m_editP2Large.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dP2L2);
	m_unitGrSmall.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL1);
	m_editGrSmall.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL1);
	m_unitGrMedium.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL1L2_2);
	m_editGrMedium1.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL1L2_1);
	m_editGrMedium2.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL1L2_2);
	m_unitGrLarge.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL2);
	m_editGrLarge.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dCrowdL2);
	m_unitP2L.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dL2);
	m_unitP2L1.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dL1);
	m_editP2L1.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dL1);
	m_editP2L2.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P2_dL2);
	m_unitMoment.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P1_dMomentCalcLoad);
	m_editMoment.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P1_dMomentCalcLoad);
	m_unitLength.SetUnitType(CUnitCtrl::m_MVLDjp_UNIT.P1_dLength);
}

void CCMMvldItemLLoadJPDlg::InitLengthCombo()
{	
	m_cboLength.ResetContent();
	for(int i=0; i<arLengthCombo.GetSize(); i++)
	{
		m_cboLength.AddString(arLengthCombo.GetAt(i));
	}

	m_cboLength.SetCurSel(0);
}

void CCMMvldItemLLoadJPDlg::InitMethodCombo()
{
	m_cboMethod.ResetContent();

	for(int i=0; i<arMethodCombo.GetSize(); i++)
		m_cboMethod.AddString(arMethodCombo.GetAt(i));

	m_cboMethod.SetCurSel(0);
}

void CCMMvldItemLLoadJPDlg::SetData2Dlg()
{
	CString str; //= _tstof(m_Data.LLoad.P1_dLength);
	str.Format(_T("%g"), m_Data.LLoad.P1_dLength);
	m_cboLength.SetWindowText(str);
	
	if(m_Data.LLoad.P1_nMethod>=0) 
		m_cboMethod.SetCurSel(m_Data.LLoad.P1_nMethod);

	m_dRatio = m_Data.LLoad.dSubMainRatio;
	m_editUnitLoad.SetEditUnit(m_Data.LLoad.P1_dUniformLoadMovement);
	m_editMoment.SetEditUnit(m_Data.LLoad.P1_dMomentCalcLoad);
	m_editShear.SetEditUnit(m_Data.LLoad.P1_dShearCalcLoad);
	m_editP2L1.SetEditUnit(m_Data.LLoad.P2_dL1);
	m_editP2L2.SetEditUnit(m_Data.LLoad.P2_dL2);
	m_editP2Small.SetEditUnit(m_Data.LLoad.P2_dP2L1);
	m_editP2Medium1.SetEditUnit(m_Data.LLoad.P2_dP2L1L2_1);
	m_editP2Medium2.SetEditUnit(m_Data.LLoad.P2_dP2L1L2_2);
	m_editP2Large.SetEditUnit(m_Data.LLoad.P2_dP2L2);
	m_editGrSmall.SetEditUnit(m_Data.LLoad.P2_dCrowdL1);
	m_editGrMedium1.SetEditUnit(m_Data.LLoad.P2_dCrowdL1L2_1);
	m_editGrMedium2.SetEditUnit(m_Data.LLoad.P2_dCrowdL1L2_2);
	m_editGrLarge.SetEditUnit(m_Data.LLoad.P2_dCrowdL2);
	
	UpdateData(FALSE);
}

BOOL CCMMvldItemLLoadJPDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	InitControlUnit();

	InitLengthCombo();
	InitMethodCombo();
	
	SetData2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
//
// Destroy
//
BOOL CCMMvldItemLLoadJPDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_Data.Initialize();    // 왜 없는거지?
	
	return CChildDialog::DestroyWindow();
}
