// CShearConnectorTabI_BS_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "ShearConnectorDlg.h"
#include "ShearConnectorTabI_BS_Dlg.h"
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
// CShearConnectorTabI_BS_Dlg dialog
CShearConnectorTabI_BS_Dlg::CShearConnectorTabI_BS_Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CShearConnectorTabI_BS_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShearConnectorTabI_BS_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CShearConnectorDlg*)pParent;

	m_Data.Initialize();
	m_nOption = 0;
}

void CShearConnectorTabI_BS_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShearConnectorTabI_BS_Dlg)
	DDX_Radio  (pDX, IDC_DGN_DESIGN_CONNECTOR_PLANE2_RDO,   m_nPlane);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_SHEAR_LS_EDT, m_edtLs);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_SHEAR_LS_UNT, m_untLs);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_SHEAR_AE_EDT, m_edtAe);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_SHEAR_AE_UNT, m_untAe);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_SHEAR_FY_EDT, m_edtFy);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_SHEAR_FY_UNT, m_untFy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShearConnectorTabI_BS_Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CShearConnectorTabI_BS_Dlg)
	ON_BN_CLICKED   (IDC_DGN_DESIGN_CONNECTOR_SHEAR_AE_BTN,    OnShearConnectAeBtn)
	ON_BN_CLICKED   (IDC_DGN_DESIGN_CONNECTOR_SHEAR_FY_BTN,    OnShearConnectFyBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CShearConnectorTabI_BS_Dlg::SetData(BOOL bBothIJ, int nOption)
{
	m_nOption = nOption;
}

void CShearConnectorTabI_BS_Dlg::GetData(T_PSCN_D &PscnD)
{
	PscnD.nFaceState[0]= m_Data.nFaceState[0];
	PscnD.dLs[0] = m_Data.dLs[0];
	PscnD.dAe[0] = m_Data.dAe[0];
	PscnD.dfy[0] = m_Data.dfy[0];
}

void CShearConnectorTabI_BS_Dlg::InitUnit()
{
	m_edtLs.SetUnitType(CUnitCtrl::m_PSCN_UNIT.dLs);
	m_untLs.SetUnitType(CUnitCtrl::m_PSCN_UNIT.dLs);
	m_edtAe.SetUnitType(CUnitCtrl::m_PSCN_UNIT.dAe);
	m_untAe.SetUnitType(CUnitCtrl::m_PSCN_UNIT.dAe);
	m_edtFy.SetUnitType(CUnitCtrl::m_PSCN_UNIT.dfy);
	m_untFy.SetUnitType(CUnitCtrl::m_PSCN_UNIT.dfy);
}

void CShearConnectorTabI_BS_Dlg::ControlsEnableDisable()
{
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_DESIGN_CONNECTOR_PLANE_GRP, m_nOption == 0, TRUE);
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_DESIGN_CONNECTOR_SHEAR_GRP, m_nOption == 0, TRUE);
}

void CShearConnectorTabI_BS_Dlg::Data2Dlg()
{
	if(m_Data.nFaceState[0] == 0)
	{
		m_nPlane = 2;
	}
	else if(m_Data.nFaceState[0] == 1)
	{
		m_nPlane = 0;
	}
	else if(m_Data.nFaceState[0] == 2)
	{
		m_nPlane = 1;
	}
	else
	{
		ASSERT(0);
		m_nPlane = 0;
	}

	m_edtLs.SetEditUnit(m_Data.dLs[0]);
	m_edtAe.SetEditUnit(m_Data.dAe[0]);
	m_edtFy.SetEditUnit(m_Data.dfy[0]);
	
	UpdateData(FALSE);
}

void CShearConnectorTabI_BS_Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	// [!주의!] DB 에는 Monolithic Construction 이 0번이다.. [CIVIL-2060] 에서 순서가 바뀌었는데 설계쪽 코드가 복잡하게 엮여 있어서 UI 에서 convert 하도록 처리 했다.
	if(m_nPlane == 0)
	{
		m_Data.nFaceState[0] = 1;
	}
	else if(m_nPlane == 1)
	{
		m_Data.nFaceState[0] = 2;
	}
	else if(m_nPlane == 2)
	{
		m_Data.nFaceState[0] = 0;
	}
	else
	{
		ASSERT(0);
		m_Data.nFaceState[0] = 0;
	}
	
	m_Data.dLs[0] = m_edtLs.GetEditValue();
	m_Data.dAe[0] = m_edtAe.GetEditValue();
	m_Data.dfy[0] = m_edtFy.GetEditValue();

	m_pParent->m_Data = m_Data;
}

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorTabI_BS_Dlg message handlers
BOOL CShearConnectorTabI_BS_Dlg::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	m_Data = m_pParent->m_Data;
	m_nOption = m_pParent->m_nOption;
		
	InitUnit();
	ControlsEnableDisable();

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CShearConnectorTabI_BS_Dlg::DestroyWindow()
{
	Dlg2Data();

	return CChildDialog::DestroyWindow();
}

void CShearConnectorTabI_BS_Dlg::OnShearConnectAeBtn()
{
	double dArea = 0.0;

	CShearConnectorAreaBSDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetValue(dArea);
		m_edtAe.SetEditUnit(dArea);
	}
}

void CShearConnectorTabI_BS_Dlg::OnShearConnectFyBtn()
{
	double dFy = 0.0;

	CShearConnectorFyDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetValue(dFy);
		m_edtFy.SetEditUnit(dFy);
	}
}


