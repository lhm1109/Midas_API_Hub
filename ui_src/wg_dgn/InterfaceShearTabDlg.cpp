#include "stdafx.h"// ExposureClassTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "InterfaceShearDlg.h"
#include "InterfaceShearTabDlg.h"
#include "InterfaceShearAreaDlg.h"
#include "ShearConnectorFyDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInterfaceShearTabDlg dialog
CInterfaceShearTabDlg::CInterfaceShearTabDlg(int nIJ, CWnd* pParent /*=NULL*/)
	: CChildDialog(CInterfaceShearTabDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInterfaceShearTabDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CInterfaceShearDlg*)pParent;

	m_Data.Initialize();
	m_bBothIJ = FALSE;
	m_nOption = 0;

	m_aFace.Add(IDC_DGN_INTERFACE_SHEAR_FACE1_RDO);
	m_aFace.Add(IDC_DGN_INTERFACE_SHEAR_FACE2_RDO);
	m_aFace.Add(IDC_DGN_INTERFACE_SHEAR_FACE3_RDO);
	m_aFace.Add(IDC_DGN_INTERFACE_SHEAR_FACE4_RDO);
	m_aFace.Add(IDC_DGN_INTERFACE_SHEAR_FACE5_RDO);
	m_aFace.Add(IDC_DGN_INTERFACE_SHEAR_FACE6_RDO);

	if(!(nIJ == 0 || nIJ == 1)) ASSERT(0);
	m_nIJ = nIJ;
}

void CInterfaceShearTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInterfaceShearTabDlg)
	DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_BVI_EDT,     m_edtBvi);
	DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_BVI_UNT,     m_untBvi);
	DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_AVF_EDT,     m_edtAvf);
	DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_AVF_UNT,     m_untAvf);
	DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_FY_EDT,      m_edtFy);
	DDX_Control(pDX, IDC_DGN_INTERFACE_SHEAR_FY_UNT,      m_untFy);


	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInterfaceShearTabDlg, CChildDialog)
	//{{AFX_MSG_MAP(CInterfaceShearTabDlg)
	ON_BN_CLICKED   (IDC_DGN_INTERFACE_SHEAR_AVF_BTN,    OnInterfaceShearAvfBtn)
	ON_BN_CLICKED   (IDC_DGN_INTERFACE_SHEAR_FY_BTN,    OnInterfaceShearFyBtn)
	//ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK, OnUserInputChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CInterfaceShearTabDlg::SetData(BOOL bBothIJ, int nOption)
{
	m_Data.bBothIJ = bBothIJ;
	m_nOption = nOption;
}

void CInterfaceShearTabDlg::GetData(T_PSCS_D &PscsD)
{
	PscsD.nFaceState[m_nIJ] = m_Data.nFaceState[m_nIJ] ;
	PscsD.dBvi[m_nIJ]       = m_Data.dBvi[m_nIJ] ;
	PscsD.dAvf[m_nIJ]       = m_Data.dAvf[m_nIJ]    ;
	PscsD.dfy[m_nIJ]        = m_Data.dfy[m_nIJ]    ;
}

void CInterfaceShearTabDlg::InitUnit()
{
	m_edtBvi.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untBvi.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtAvf.SetUnitType(D_UNITSYS_BASE_AREA);
	m_untAvf.SetUnitType(D_UNITSYS_BASE_AREA);
	m_edtFy.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untFy.SetUnitType(D_UNITSYS_BASE_STRESS);
}



void CInterfaceShearTabDlg::ControlsEnableDisable()
{
	BOOL bEnable = TRUE;
	//if(m_pParent->m_nOption==0) bEnable = TRUE;
	if(m_pParent->m_nOption==1 || (m_Data.bBothIJ && m_nIJ))
	{
		bEnable = FALSE;
	}
	else bEnable = TRUE;

	GetDlgItem(IDC_DGN_DESIGN_INTERFACE_SHEAR_TAB_GRP   )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FACE1_RDO        )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FACE2_RDO        )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FACE3_RDO        )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FACE4_RDO        )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FACE5_RDO        )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FACE6_RDO        )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_DESIGN_INTERFACE_SHEAR_TAB_GRP2  )->EnableWindow(bEnable);
	GetDlgItem(IDC_BVI_STC                              )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_BVI_EDT          )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_BVI_UNT          )->EnableWindow(bEnable);
	GetDlgItem(IDC_AVF_STC                              )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_AVF_EDT          )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_AVF_UNT          )->EnableWindow(bEnable);
	GetDlgItem(IDC_FY_STC                               )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FY_EDT           )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FY_UNT           )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_AVF_BTN          )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_INTERFACE_SHEAR_FY_BTN           )->EnableWindow(bEnable);
}

void CInterfaceShearTabDlg::Data2Dlg()
{

	CDlgUtil::CtrlRadioSetCheck(this, m_aFace, m_Data.nFaceState[m_nIJ]);

	m_edtBvi.SetEditUnit(m_Data.dBvi[m_nIJ]);
	m_edtAvf.SetEditUnit(m_Data.dAvf[m_nIJ]);
	m_edtFy.SetEditUnit(m_Data.dfy[m_nIJ]);
}

void CInterfaceShearTabDlg::Dlg2Data()
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
// CInterfaceShearTabDlg message handlers
BOOL CInterfaceShearTabDlg::OnInitDialog() 
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

BOOL CInterfaceShearTabDlg::DestroyWindow() 
{
	Dlg2Data();

	return CChildDialog::DestroyWindow();
}

void CInterfaceShearTabDlg::OnInterfaceShearAvfBtn()
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

void CInterfaceShearTabDlg::OnInterfaceShearFyBtn()
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


