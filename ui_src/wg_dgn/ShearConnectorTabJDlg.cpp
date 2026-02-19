// ExposureClassTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "ShearConnectorDlg.h"
#include "ShearConnectorTabJDlg.h"
#include "ShearConnectorAreaDlg.h"
#include "ShearConnectorFyDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorTabJDlg dialog
CShearConnectorTabJDlg::CShearConnectorTabJDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CShearConnectorTabJDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShearConnectorTabJDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CShearConnectorDlg*)pParent;

	m_Data.Initialize();
	m_bBothIJ = FALSE;
	m_nOption = 0;

	m_aFace.Add(IDC_DGN_CONNECTOR_FACE1_RDO);
	m_aFace.Add(IDC_DGN_CONNECTOR_FACE2_RDO);
	m_aFace.Add(IDC_DGN_CONNECTOR_FACE3_RDO);
	m_aFace.Add(IDC_DGN_CONNECTOR_FACE4_RDO);

}

void CShearConnectorTabJDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShearConnectorTabJDlg)
	DDX_Control(pDX, IDC_DGN_CONNECTOR_ANGLE_EDT,   m_edtAngle);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_ANGLE_UNT,   m_untAngle);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_AW_EDT,      m_edtAw);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_AW_UNT,      m_untAw);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_FY_EDT,      m_edtFy);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_FY_UNT,      m_untFy);

	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShearConnectorTabJDlg, CChildDialog)
	//{{AFX_MSG_MAP(CShearConnectorTabJDlg)
	ON_BN_CLICKED   (IDC_DGN_CONNECTOR_AW_BTN,    OnShearConnectAwBtn)
	ON_BN_CLICKED   (IDC_DGN_CONNECTOR_FY_BTN,    OnShearConnectFyBtn)
	//ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK, OnUserInputChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CShearConnectorTabJDlg::SetData(BOOL bBothIJ, int nOption)
{
	m_bBothIJ = bBothIJ;
	m_nOption = nOption;

    CDBDoc* m_pDoc = CDBDoc::GetDocPoint();
    T_PSCD_D rData;
    m_pDoc->m_pAttrCtrl->GetDgnPscd(rData);
    if (rData.iDgnCode == IRC_112_2020_PSC)
        GetDlgItem(IDC_DGN_DESIGN_CONNECTOR_FACE_TAB_GRP)->SetWindowText(_T("Surface Classification(Cl.10.3.4)"));

}

void CShearConnectorTabJDlg::GetData(T_PSCN_D &PscnD)
{
	PscnD.dAngle[1]= m_Data.dAngle[1] ;
	PscnD.dAw[1]   = m_Data.dAw[1]    ;
	PscnD.dfy[1]   = m_Data.dfy[1]    ;
}

void CShearConnectorTabJDlg::InitUnit()
{
	m_edtAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_untAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_edtAw.SetUnitType(D_UNITSYS_BASE_AREA);
	m_untAw.SetUnitType(D_UNITSYS_BASE_AREA);
	m_edtFy.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untFy.SetUnitType(D_UNITSYS_BASE_STRESS);
}



void CShearConnectorTabJDlg::ControlsEnableDisable()
{
	if(!m_bBothIJ && m_nOption==0) // add
	{                                                 
		GetDlgItem(IDC_DGN_DESIGN_CONNECTOR_FACE_TAB_GRP )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_FACE1_RDO           )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_FACE2_RDO           )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_FACE3_RDO           )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_FACE4_RDO           )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DESIGN_CONNECTOR_FACE_TAB_GRP2)->EnableWindow(TRUE);
		GetDlgItem(IDC_ANGLE_STC                         )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_ANGLE_EDT           )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_ANGLE_UNT           )->EnableWindow(TRUE);
		GetDlgItem(IDC_AW_STC                            )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_AW_EDT              )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_AW_UNT              )->EnableWindow(TRUE);
		GetDlgItem(IDC_FY_STC                            )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_FY_EDT              )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_FY_UNT              )->EnableWindow(TRUE); 
		GetDlgItem(IDC_DGN_CONNECTOR_AW_BTN              )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_FY_BTN              )->EnableWindow(TRUE);   
	}
	else  //delete
	{
		GetDlgItem(IDC_DGN_DESIGN_CONNECTOR_FACE_TAB_GRP )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_FACE1_RDO           )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_FACE2_RDO           )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_FACE3_RDO           )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_FACE4_RDO           )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DESIGN_CONNECTOR_FACE_TAB_GRP2)->EnableWindow(FALSE);
		GetDlgItem(IDC_ANGLE_STC                         )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_ANGLE_EDT           )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_ANGLE_UNT           )->EnableWindow(FALSE);
		GetDlgItem(IDC_AW_STC                            )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_AW_EDT              )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_AW_UNT              )->EnableWindow(FALSE);
		GetDlgItem(IDC_FY_STC                            )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_FY_EDT              )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_FY_UNT              )->EnableWindow(FALSE); 
		GetDlgItem(IDC_DGN_CONNECTOR_AW_BTN              )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_FY_BTN              )->EnableWindow(FALSE);   
	}

}

void CShearConnectorTabJDlg::Data2Dlg()
{

	if(m_bBothIJ)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aFace, m_Data.nFaceState[0]);
		
		m_edtAngle.SetEditUnit(m_Data.dAngle[0]);
		m_edtAw.SetEditUnit(m_Data.dAw[0]);
		m_edtFy.SetEditUnit(m_Data.dfy[0]);
	}
	else
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aFace, m_Data.nFaceState[1]);
		
		m_edtAngle.SetEditUnit(m_Data.dAngle[1]);
		m_edtAw.SetEditUnit(m_Data.dAw[1]);
		m_edtFy.SetEditUnit(m_Data.dfy[1]);
	}

}

void CShearConnectorTabJDlg::Dlg2Data()
{
	UpdateData(TRUE);

	if(m_bBothIJ)
	{
		m_Data.nFaceState[1] = m_Data.nFaceState[0];
		m_Data.dAngle[1]     = m_Data.dAngle[0];
		m_Data.dAw[1]        = m_Data.dAw[0];
		m_Data.dfy[1]        = m_Data.dfy[0];
	}
	else
	{
		CDlgUtil::CtrlRadioGetCheck(this, m_aFace, m_Data.nFaceState[1]);
		
		m_Data.dAngle[1] = m_edtAngle.GetEditValue();
		m_Data.dAw[1]    = m_edtAw.GetEditValue();
		m_Data.dfy[1]    = m_edtFy.GetEditValue();
	}

	m_pParent->m_Data =m_Data; 
}

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorTabJDlg message handlers
BOOL CShearConnectorTabJDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_Data = m_pParent->m_Data;
	m_bBothIJ = m_pParent->m_bBothIJ;
	m_nOption = m_pParent->m_nOption;
		
	InitUnit();

	ControlsEnableDisable();

	Data2Dlg();

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CShearConnectorTabJDlg::DestroyWindow() 
{
	Dlg2Data();

	return CChildDialog::DestroyWindow();
}

void CShearConnectorTabJDlg::OnShearConnectAwBtn()
{
	double dArea = 0.;

	// 대화창 호출  ShearConnectAreaDlg
	CShearConnectorAreaDlg dlg;
	dlg.DoModal();
	dlg.GetValue(dArea);

	m_edtAw.SetEditUnit(dArea);
}

void CShearConnectorTabJDlg::OnShearConnectFyBtn()
{
	double dFy = 0.;

	//대화창 호출  ShearConnectFyDlg
	CShearConnectorFyDlg dlg;
	dlg.DoModal();
	dlg.GetValue(dFy);

	m_edtFy.SetEditUnit(dFy);
}
