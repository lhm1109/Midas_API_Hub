// ExposureClassTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "ShearConnectorDlg.h"
#include "ShearConnectorTabJ_IRS_Dlg.h"
#include "ShearConnectorAreaDlg.h"
#include "ShearConnectorFyDlg.h"
#include "ShearConnectorFckIRSDlg.h"
#include "ShearConnectorBarAreaIRSDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorTabJ_IRS_Dlg dialog
CShearConnectorTabJ_IRS_Dlg::CShearConnectorTabJ_IRS_Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CShearConnectorTabJ_IRS_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShearConnectorTabJ_IRS_Dlg)
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

void CShearConnectorTabJ_IRS_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShearConnectorTabJ_IRS_Dlg)

	DDX_Control(pDX, IDC_DGN_CONNECTOR_ANGLE_EDT2, m_edtSL		);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_ANGLE_UNT2, m_untSL		);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_AW_EDT2,    m_edtASL	);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_AW_UNT2,    m_untASL	);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_FY_EDT2,    m_edtAst	);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_FY_UNT2,    m_untAst	);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_ANGLE_EDT,  m_edtAe		);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_ANGLE_UNT,  m_untAe		);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_AW_EDT,     m_edtFck		);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_AW_UNT,     m_untFck		);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_FY_EDT,     m_edtFy		);
	DDX_Control(pDX, IDC_DGN_CONNECTOR_FY_UNT,     m_untFy		);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShearConnectorTabJ_IRS_Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CShearConnectorTabJ_IRS_Dlg)
	ON_BN_CLICKED   (IDC_DGN_CONNECTOR_FY_BTN,    OnShearConnectFyBtn)
	ON_BN_CLICKED   (IDC_DGN_CONNECTOR_AW_BTN,    OnShearConnectFckBtn)
	ON_BN_CLICKED   (IDC_DGN_CONNECTOR_AW_BTN3,    OnShearConnectAeBtn)

	ON_BN_CLICKED   (IDC_DGN_CONNECTOR_FY_BTN2,    OnShearConnectorBarAreaEdtAst)
	ON_BN_CLICKED   (IDC_DGN_CONNECTOR_AW_BTN2,    OnShearConnectorBarAreaEdtASL)
	//ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB_USER_CHK, OnUserInputChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CShearConnectorTabJ_IRS_Dlg::SetData(BOOL bBothIJ, int nOption)
{
	m_bBothIJ = bBothIJ;
	m_nOption = nOption;
}

void CShearConnectorTabJ_IRS_Dlg::GetData(T_PSCN_D &PscnD)
{
	
	PscnD.IRS_SConn.dSL[1] 	 = m_Data.IRS_SConn.dSL[1] ;
	PscnD.IRS_SConn.dASL[1]	 = m_Data.IRS_SConn.dASL[1];
	PscnD.IRS_SConn.dSt[1] 	 = m_Data.IRS_SConn.dSt[1] ;
	PscnD.IRS_SConn.dAe[1] 	 = m_Data.IRS_SConn.dAe[1] ;
	PscnD.IRS_SConn.dFck[1]	 = m_Data.IRS_SConn.dFck[1];
	PscnD.IRS_SConn.dFy[1] 	 = m_Data.IRS_SConn.dFy[1];
// 	
// 	PscnD.dAngle[1]= m_Data.dAngle[1] ;
// 	PscnD.dAw[1]   = m_Data.dAw[1]    ;
// 	PscnD.dfy[1]   = m_Data.dfy[1]    ;
}

void CShearConnectorTabJ_IRS_Dlg::InitUnit()
{
		m_edtSL	.SetUnitType(D_UNITSYS_NONE		  );
		m_untSL	.SetUnitType(D_UNITSYS_NONE		  );
		m_edtASL.SetUnitType(D_UNITSYS_BASE_AREA	);
		m_untASL.SetUnitType(D_UNITSYS_BASE_AREA	);
		m_edtAst.SetUnitType(D_UNITSYS_BASE_AREA	);
		m_untAst.SetUnitType(D_UNITSYS_BASE_AREA	);
		m_edtAe	.SetUnitType(D_UNITSYS_BASE_AREA  );
		m_untAe	.SetUnitType(D_UNITSYS_BASE_AREA  );
		m_edtFck.SetUnitType(D_UNITSYS_BASE_STRESS);
		m_untFck.SetUnitType(D_UNITSYS_BASE_STRESS);
		m_edtFy	.SetUnitType(D_UNITSYS_BASE_STRESS);
		m_untFy	.SetUnitType(D_UNITSYS_BASE_STRESS);

	//m_edtAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	//m_untAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	//m_edtAw.SetUnitType(D_UNITSYS_BASE_AREA);
	//m_untAw.SetUnitType(D_UNITSYS_BASE_AREA);
	//m_edtFy.SetUnitType(D_UNITSYS_BASE_STRESS);
	//m_untFy.SetUnitType(D_UNITSYS_BASE_STRESS);
}



void CShearConnectorTabJ_IRS_Dlg::ControlsEnableDisable()
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

		GetDlgItem(IDC_DGN_CONNECTOR_FY_EDT2 			  )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_FY_BTN2              )->EnableWindow(TRUE); 
		GetDlgItem(IDC_DGN_CONNECTOR_AW_BTN2              )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_AW_BTN3              )->EnableWindow(TRUE);
		GetDlgItem(IDC_FY_STC2                            )->EnableWindow(TRUE);
		GetDlgItem(IDC_AW_STC2                            )->EnableWindow(TRUE);
		GetDlgItem(IDC_ANGLE_STC2                         )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONNECTOR_FY_UNT2              )->EnableWindow(TRUE);
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
		
		GetDlgItem(IDC_DGN_CONNECTOR_FY_EDT2 			  )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_FY_BTN2              )->EnableWindow(FALSE); 
		GetDlgItem(IDC_DGN_CONNECTOR_AW_BTN2              )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_AW_BTN3              )->EnableWindow(FALSE);
		GetDlgItem(IDC_FY_STC2                            )->EnableWindow(FALSE);
		GetDlgItem(IDC_AW_STC2                            )->EnableWindow(FALSE);
		GetDlgItem(IDC_ANGLE_STC2                         )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONNECTOR_FY_UNT2              )->EnableWindow(FALSE);
// 		GetDlgItem(IDC_FY_STC213						  )->EnableWindow(FALSE);
// 		GetDlgItem(IDC_ANGLE_STC210						  )->EnableWindow(FALSE);
														 
		 
	}

}

void CShearConnectorTabJ_IRS_Dlg::Data2Dlg()
{

	if(m_bBothIJ)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aFace, m_Data.nFaceState[0]);
		
		m_edtSL	.SetEditUnit(m_Data.IRS_SConn.dSL[0] );		
		m_edtASL.SetEditUnit(m_Data.IRS_SConn.dASL[0]);		
		m_edtAst.SetEditUnit(m_Data.IRS_SConn.dSt[0] );		
		m_edtAe	.SetEditUnit(m_Data.IRS_SConn.dAe[0] );		
		m_edtFck.SetEditUnit(m_Data.IRS_SConn.dFck[0]);		
		m_edtFy	.SetEditUnit(m_Data.IRS_SConn.dFy[0]);
		

		/*	m_edtAngle.SetEditUnit(m_Data.dAngle[0]);
		m_edtAw.SetEditUnit(m_Data.dAw[0]);
		m_edtFy.SetEditUnit(m_Data.dfy[0]);*/
	}
	else
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aFace, m_Data.nFaceState[1]);
		
		m_edtSL	.SetEditUnit(m_Data.IRS_SConn.dSL[1] );		
		m_edtASL.SetEditUnit(m_Data.IRS_SConn.dASL[1]);		
		m_edtAst.SetEditUnit(m_Data.IRS_SConn.dSt[1] );		
		m_edtAe	.SetEditUnit(m_Data.IRS_SConn.dAe[1] );		
		m_edtFck.SetEditUnit(m_Data.IRS_SConn.dFck[1]);		
		m_edtFy	.SetEditUnit(m_Data.IRS_SConn.dFy[1] );

		/*m_edtAngle.SetEditUnit(m_Data.dAngle[1]);
		m_edtAw.SetEditUnit(m_Data.dAw[1]);
		m_edtFy.SetEditUnit(m_Data.dfy[1]);*/
	}

}

void CShearConnectorTabJ_IRS_Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	if(m_bBothIJ)
	{
		m_Data.nFaceState[1] = m_Data.nFaceState[0];

		m_Data.IRS_SConn.dSL[1] 		= m_Data.IRS_SConn.dSL[0] ;
		m_Data.IRS_SConn.dASL[1]		= m_Data.IRS_SConn.dASL[0];
		m_Data.IRS_SConn.dSt[1] 		= m_Data.IRS_SConn.dSt[0] ;
		m_Data.IRS_SConn.dAe[1] 		= m_Data.IRS_SConn.dAe[0] ;
		m_Data.IRS_SConn.dFck[1]		= m_Data.IRS_SConn.dFck[0];
		m_Data.IRS_SConn.dFy[1]  		= m_Data.IRS_SConn.dFy[0] ;
							 
		/*m_Data.dAngle[1]     = m_Data.dAngle[0];
		m_Data.dAw[1]        = m_Data.dAw[0];
		m_Data.dfy[1]        = m_Data.dfy[0];*/
	}
	else
	{
		CDlgUtil::CtrlRadioGetCheck(this, m_aFace, m_Data.nFaceState[1]);
		
		m_Data.IRS_SConn.dSL[1] 	 =  m_edtSL	.GetEditValue();
		m_Data.IRS_SConn.dASL[1]	 =  m_edtASL.GetEditValue();
		m_Data.IRS_SConn.dSt[1] 	 =  m_edtAst.GetEditValue();
		m_Data.IRS_SConn.dAe[1] 	 =  m_edtAe	.GetEditValue();
		m_Data.IRS_SConn.dFck[1]	 =  m_edtFck.GetEditValue();
		m_Data.IRS_SConn.dFy[1]  	 =  m_edtFy	.GetEditValue();

		/*m_Data.dAngle[1] = m_edtAngle.GetEditValue();
		m_Data.dAw[1]    = m_edtAw.GetEditValue();
		m_Data.dfy[1]    = m_edtFy.GetEditValue();*/
	}

	m_pParent->m_Data.IRS_SConn =m_Data.IRS_SConn; 
}

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorTabJ_IRS_Dlg message handlers
BOOL CShearConnectorTabJ_IRS_Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_Data.IRS_SConn = m_pParent->m_Data.IRS_SConn;
	m_bBothIJ = m_pParent->m_bBothIJ;
	m_nOption = m_pParent->m_nOption;
		
	InitUnit();

	ControlsEnableDisable();

	Data2Dlg();

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CShearConnectorTabJ_IRS_Dlg::DestroyWindow() 
{
	Dlg2Data();

	return CChildDialog::DestroyWindow();
}

void CShearConnectorTabJ_IRS_Dlg::OnShearConnectAwBtn()
{
	double dArea = 0.;

	// 대화창 호출  ShearConnectAreaDlg
	CShearConnectorAreaDlg dlg;
	dlg.DoModal();
	dlg.GetValue(dArea);

	m_edtAst.SetEditUnit(dArea);
}

void CShearConnectorTabJ_IRS_Dlg::OnShearConnectFyBtn()
{
	double dFy = 0.;

	//대화창 호출  ShearConnectFyDlg
	CShearConnectorFyDlg dlg;
	dlg.DoModal();
	dlg.GetValue(dFy);

	m_edtFy.SetEditUnit(dFy);
}


void CShearConnectorTabJ_IRS_Dlg::OnShearConnectFckBtn()
{
	double dFck = 0.;

	// 대화창 호출  ShearConnectFyDlg
	CShearConnectorFckIRSDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetValue(dFck);
		m_edtFck.SetEditUnit(dFck);
	}
}

void CShearConnectorTabJ_IRS_Dlg::OnShearConnectAeBtn()
{
	double dAe= 0.;

	// 대화창 호출  ShearConnectFyDlg
	CShearConnectorAreaDlg dlg;
	;
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetValue(dAe);
		m_edtAe.SetEditUnit(dAe);
	}
}

void CShearConnectorTabJ_IRS_Dlg::OnShearConnectorBarAreaEdtAst()
{
	double dAst = 0.;

	// 대화창 호출  ShearConnectFyDlg
	CShearConnectorBarAreaIRSDlg dlg;

	if(dlg.DoModal()==IDOK)
	{
		dlg.GetValue(dAst);
		m_edtAst.SetEditUnit(dAst);
	}
}

void CShearConnectorTabJ_IRS_Dlg::OnShearConnectorBarAreaEdtASL()
{
	double dAsl = 0.;

	// 대화창 호출  ShearConnectFyDlg
	CShearConnectorBarAreaIRSDlg dlg;
	
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetValue(dAsl);
		m_edtASL.SetEditUnit(dAsl);
	}
}