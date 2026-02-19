// ExposureClassTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "ShearConnectorDlg.h"
#include "ShearConnectorTabJ_BS_Dlg.h"
#include "ShearConnectorAreaBSDlg.h"
#include "ShearConnectorFyDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorTabJ_BS_Dlg dialog
CShearConnectorTabJ_BS_Dlg::CShearConnectorTabJ_BS_Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CShearConnectorTabJ_BS_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShearConnectorTabJ_BS_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CShearConnectorDlg*)pParent;

	m_Data.Initialize();
	m_bBothIJ = FALSE;
	m_nOption = 0;
}

void CShearConnectorTabJ_BS_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShearConnectorTabJ_BS_Dlg)
	DDX_Radio  (pDX, IDC_DGN_DESIGN_CONNECTOR_PLANE1_RDO,   m_nPlane);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_SHEAR_LS_EDT, m_edtLs);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_SHEAR_LS_UNT, m_untLs);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_SHEAR_AE_EDT, m_edtAe);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_SHEAR_AE_UNT, m_untAe);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_SHEAR_FY_EDT, m_edtFy);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_SHEAR_FY_UNT, m_untFy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShearConnectorTabJ_BS_Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CShearConnectorTabJ_BS_Dlg)
	ON_BN_CLICKED(IDC_DGN_DESIGN_CONNECTOR_SHEAR_AE_BTN, OnShearConnectAeBtn)
	ON_BN_CLICKED(IDC_DGN_DESIGN_CONNECTOR_SHEAR_FY_BTN, OnShearConnectFyBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CShearConnectorTabJ_BS_Dlg::SetData(BOOL bBothIJ, int nOption)
{
	m_bBothIJ = bBothIJ;
	m_nOption = nOption;
}

void CShearConnectorTabJ_BS_Dlg::GetData(T_PSCN_D &PscnD)
{
	PscnD.nFaceState[1] = m_Data.nFaceState[1];
	PscnD.dLs[1] = m_Data.dLs[1];
	PscnD.dAe[1] = m_Data.dAe[1];
	PscnD.dfy[1] = m_Data.dfy[1];
}

void CShearConnectorTabJ_BS_Dlg::InitUnit()
{
	m_edtLs.SetUnitType(CUnitCtrl::m_PSCN_UNIT.dLs);
	m_untLs.SetUnitType(CUnitCtrl::m_PSCN_UNIT.dLs);
	m_edtAe.SetUnitType(CUnitCtrl::m_PSCN_UNIT.dAe);
	m_untAe.SetUnitType(CUnitCtrl::m_PSCN_UNIT.dAe);
	m_edtFy.SetUnitType(CUnitCtrl::m_PSCN_UNIT.dfy);
	m_untFy.SetUnitType(CUnitCtrl::m_PSCN_UNIT.dfy);
}

void CShearConnectorTabJ_BS_Dlg::ControlsEnableDisable()
{
	BOOL bEnable = FALSE;
	if (!m_bBothIJ && m_nOption == 0) bEnable = TRUE;

	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_DESIGN_CONNECTOR_PLANE_GRP, bEnable, TRUE);
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_DESIGN_CONNECTOR_SHEAR_GRP, bEnable, TRUE);
}

void CShearConnectorTabJ_BS_Dlg::Data2Dlg()
{
	if(m_bBothIJ)
	{
		m_nPlane = m_Data.nFaceState[0];

		m_edtLs.SetEditUnit(m_Data.dLs[0]);
		m_edtAe.SetEditUnit(m_Data.dAe[0]);
		m_edtFy.SetEditUnit(m_Data.dfy[0]);
	}
	else
	{
		m_nPlane = m_Data.nFaceState[1];

		m_edtLs.SetEditUnit(m_Data.dLs[1]);
		m_edtAe.SetEditUnit(m_Data.dAe[1]);
		m_edtFy.SetEditUnit(m_Data.dfy[1]);
	}

	UpdateData(FALSE);
}

void CShearConnectorTabJ_BS_Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	if(m_bBothIJ)
	{
		m_Data.nFaceState[1] = m_Data.nFaceState[0];
		m_Data.dLs[1] = m_Data.dLs[0];
		m_Data.dAe[1] = m_Data.dAe[0];
		m_Data.dfy[1] = m_Data.dfy[0];
	}
	else
	{
		m_Data.nFaceState[1] = m_nPlane;

		m_Data.dLs[1] = m_edtLs.GetEditValue();
		m_Data.dAe[1] = m_edtAe.GetEditValue();
		m_Data.dfy[1] = m_edtFy.GetEditValue();
	}

	m_pParent->m_Data =m_Data; 
}

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorTabJ_BS_Dlg message handlers
BOOL CShearConnectorTabJ_BS_Dlg::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	m_Data = m_pParent->m_Data;
	m_bBothIJ = m_pParent->m_bBothIJ;
	m_nOption = m_pParent->m_nOption;
		
	InitUnit();
	ControlsEnableDisable();

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CShearConnectorTabJ_BS_Dlg::DestroyWindow()
{
	Dlg2Data();

	return CChildDialog::DestroyWindow();
}

void CShearConnectorTabJ_BS_Dlg::OnShearConnectAeBtn()
{
	double dArea = 0.0;

	CShearConnectorAreaBSDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetValue(dArea);
		m_edtAe.SetEditUnit(dArea);
	}
}

void CShearConnectorTabJ_BS_Dlg::OnShearConnectFyBtn()
{
	double dFy = 0.0;

	CShearConnectorFyDlg dlg;
	dlg.DoModal();
	dlg.GetValue(dFy);

	m_edtFy.SetEditUnit(dFy);
}
