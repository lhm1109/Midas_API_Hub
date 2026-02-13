// ExposureClassTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "InterfaceShearDlg.h"
#include "InterfaceShearTabDlg_CSA.h"
#include "InterfaceShearAreaDlg.h"
#include "ShearConnectorFyDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInterfaceShearTabDlg_CSA dialog
CInterfaceShearTabDlg_CSA::CInterfaceShearTabDlg_CSA(int nIJ, CWnd* pParent /*T_SECT_D& SectD*/ /*=NULL*/) : CChildDialog(CInterfaceShearTabDlg_CSA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInterfaceShearTabDlg_CSA)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CInterfaceShearDlg*)pParent;
	m_Data.Initialize();
	m_bBothIJ = FALSE;
	m_nOption = 0;
	m_bAuto   = TRUE;
//  m_edtBvi  = GetAutoBvi(SectD);

	m_aFace.Add(IDC_DGN_INTERFACE_SHEAR_FACE1_CSA_RDO);
	m_aFace.Add(IDC_DGN_INTERFACE_SHEAR_FACE2_CSA_RDO);
	m_aFace.Add(IDC_DGN_INTERFACE_SHEAR_FACE3_CSA_RDO);

	if(!(nIJ == 0 || nIJ == 1)) ASSERT(0);
	m_nIJ = nIJ;
}

void CInterfaceShearTabDlg_CSA::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInterfaceShearTabDlg_CSA)
	DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_BVI_CSA_EDT,     m_edtBvi);
	DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_BVI_CSA_UNT,     m_untBvi);
	DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_AVF_CSA_EDT,     m_edtAvf);
	DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_AVF_CSA_UNT,     m_untAvf);
	//DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_AVF2_CSA_UNT,    m_untAvf2);
	DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_FY_CSA_EDT,      m_edtFy);
	DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_FY_CSA_UNT,      m_untFy);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInterfaceShearTabDlg_CSA, CChildDialog)
	//{{AFX_MSG_MAP(CInterfaceShearTabDlg_CSA)
	ON_BN_CLICKED   (IDC_DGN_INTERFACE_SHEAR_AVF_CSA_BTN,    OnInterfaceShearAvfBtn)
	ON_BN_CLICKED   (IDC_DGN_INTERFACE_SHEAR_FY_CSA_BTN,    OnInterfaceShearFyBtn)
	//ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK, OnUserInputChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CInterfaceShearTabDlg_CSA::SetData(BOOL bBothIJ, int nOption)
{
	m_Data.bBothIJ = bBothIJ;
	m_nOption = nOption;
}

void CInterfaceShearTabDlg_CSA::GetData(T_PSCS_D &PscsD)
{
	PscsD.nFaceState[m_nIJ] = m_Data.nFaceState[m_nIJ] ;
	PscsD.dAvf[m_nIJ]       = m_Data.dAvf[m_nIJ]    ;
	PscsD.dfy[m_nIJ]        = m_Data.dfy[m_nIJ]    ;
}

void CInterfaceShearTabDlg_CSA::InitUnit()
{
	m_edtAvf.SetUnitType(D_UNITSYS_BASE_AREA);
	m_untAvf.SetUnitType(D_UNITSYS_BASE_UNIT_AREA);
 // m_untAvf2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtFy.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untFy.SetUnitType(D_UNITSYS_BASE_STRESS);
}



void CInterfaceShearTabDlg_CSA::ControlsEnableDisable()
{
	BOOL bEnable = TRUE;
	//if(m_pParent->m_nOption==0) bEnable = TRUE;
	if(m_pParent->m_nOption==1 || (m_Data.bBothIJ && m_nIJ))
	{
		bEnable = FALSE;
	}
	else bEnable = TRUE;

	GetDlgItem(IDC_DGN_DESIGN_INTERFACE_SHEAR_TAB_CSA_GRP   )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FACE1_CSA_RDO        )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FACE2_CSA_RDO        )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FACE3_CSA_RDO        )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_DESIGN_INTERFACE_SHEAR_TAB_CSA_GRP2  )->EnableWindow(bEnable);
	GetDlgItem(IDC_BVI_CSA_STC                              )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_BVI_CSA_EDT          )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_BVI_CSA_UNT          )->EnableWindow(bEnable);
	GetDlgItem(IDC_AVF_CSA_STC                              )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_AVF_CSA_EDT          )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_AVF_CSA_UNT          )->EnableWindow(bEnable);
	GetDlgItem(IDC_FY_CSA_STC                               )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FY_CSA_EDT           )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FY_CSA_UNT           )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_AVF_CSA_BTN          )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FY_CSA_BTN           )->EnableWindow(bEnable);
}

void CInterfaceShearTabDlg_CSA::Data2Dlg()
{

	CDlgUtil::CtrlRadioSetCheck(this, m_aFace, m_Data.nFaceState[m_nIJ]);

	m_edtBvi.SetEditUnit(m_Data.dBvi[m_nIJ]);
	m_edtAvf.SetEditUnit(m_Data.dAvf[m_nIJ]);
	m_edtFy.SetEditUnit(m_Data.dfy[m_nIJ]);
}

void CInterfaceShearTabDlg_CSA::Dlg2Data()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlRadioGetCheck(this, m_aFace, m_Data.nFaceState[m_nIJ]);
	m_Data.dBvi[m_nIJ]  = m_edtBvi.GetEditValue();
	m_Data.dAvf[m_nIJ]  = m_edtAvf.GetEditValue();
	m_Data.dfy[m_nIJ]   = m_edtFy.GetEditValue();


	m_pParent->m_Data.nFaceState[m_nIJ] = m_Data.nFaceState[m_nIJ]; 
	m_pParent->m_Data.dBvi[m_nIJ]       = m_Data.dBvi[m_nIJ]; 
	m_pParent->m_Data.dAvf[m_nIJ]       = m_Data.dAvf[m_nIJ]; 
	m_pParent->m_Data.dfy[m_nIJ]        = m_Data.dfy[m_nIJ]; 

}

/////////////////////////////////////////////////////////////////////////////
// CInterfaceShearTabDlg_CSA message handlers
BOOL CInterfaceShearTabDlg_CSA::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_Data = m_pParent->m_Data;
	//m_bBothIJ = m_pParent->m_bBothIJ;
	m_nOption = m_pParent->m_nOption;

	InitUnit();

	ControlsEnableDisable();

	Data2Dlg();

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CInterfaceShearTabDlg_CSA::DestroyWindow() 
{
	Dlg2Data();

	return CChildDialog::DestroyWindow();
}

void CInterfaceShearTabDlg_CSA::OnInterfaceShearAvfBtn()
{
	double dArea = 0.0;

	// 대화창 호출  ShearConnectAreaDlg
	CInterfaceShearAreaDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetValue(dArea);
		m_edtAvf.SetEditUnit(dArea);
	}

}

void CInterfaceShearTabDlg_CSA::OnInterfaceShearFyBtn()
{
	double dFy = 0.0;

	// 대화창 호출  ShearConnectFyDlg
	CShearConnectorFyDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetValue(dFy);
		m_edtFy.SetEditUnit(dFy);
	}
}

// double CInterfaceShearTabDlg_CSA::GetAutoBvi(T_SECT_D& SectD)
// {
//   CString sShape = SectD.SectBefore.Shape;
//   double dAutoBvi = 0.0;
// 
//   if(sShape == D_SECT_SHAPE_COMPO_CI)
//   {
//     T_SECT_D_PSC_I Composite_I;
//     Composite_I.Set(SectD,TRUE,TRUE);
//     dAutoBvi = Composite_I.BL2 + Composite_I.BR2;
//     return dAutoBvi;
//   }
//   else if(sShape == D_SECT_SHAPE_COMPO_CT)
//   {
//     T_SECT_D_PSC_T Composite_T;
//     Composite_T.Set(SectD,TRUE,TRUE);
//     dAutoBvi = Composite_T.BL4 + Composite_T.BR4;
//     return dAutoBvi;
//   }
//   else if(sShape ==D_SECT_SHAPE_COMPO_PC)
//   {
//     return dAutoBvi;
//   }
//   else
//     return;
// }
