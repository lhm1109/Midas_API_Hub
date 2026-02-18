// CDgnCPGDesignParamEC2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCPGDesignParamEC2Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\DBLib.h"

#include "..\wg_main\wg_main.h"

#include "DgnCPGDesignParamBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamEC2Dlg dialog


CDgnCPGDesignParamEC2Dlg::CDgnCPGDesignParamEC2Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnCPGDesignParamEC2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCPGDesignParamEC2Dlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(pParent);
	m_pParent = (CDgnCPGDesignParamBaseDlg*)pParent;  
}

CDgnCPGDesignParamEC2Dlg::~CDgnCPGDesignParamEC2Dlg()
{
	
}

void CDgnCPGDesignParamEC2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCPGDesignParamEC2Dlg)
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_C_EDIT,		m_dGamm_C);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_S_EDIT,		m_dGamm_S);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_M0_EDIT,		m_dGamm_M0);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_M1_EDIT,		m_dGamm_M1);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_V_EDIT,		m_dGamm_V);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_FF_EDIT,		m_dGamm_Ff);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_MF_EDIT,		m_dGamm_Mf);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_MF_S_EDIT, m_dGamm_Mf_s);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMMA_MSER_EDT, m_dGamm_M_ser);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_T_LD_EDIT,      m_dt_Ld);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_K1_EDT,         m_dk1);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_K2_EDT,         m_dk2);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_K3_EDT,         m_dk3);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_KS_EDT,         m_dks);

	DDX_Check(pDX, IDC_DGN_CPG_PARAM_BEND_RESIST_CHECK,										m_bBendig_Resist);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_RESIST_VERTICAL_SHEAR_CHECK,					m_bResist_Vertical_Shear);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_RESIST_LATER_TORSION_BUCKLING_CHECK, m_bResist_Later_Torsion_Buckling);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_RESIST_TRANS_FORCE_CHECK,						m_bResist_Trans_Force);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_RESIST_LONG_SHEAR_CHECK,							m_bResist_Longi_Shear);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_RESIST_FATIGUE_CHECK,								m_bResist_Fatigue);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_STRESS_LIMIT_CHK,			     					m_bStress_Limit);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_LONGI_SHEAR_CHK,						      		m_bLongi_Shear);
	//}}AFX_DATA_MAP
}

BOOL CDgnCPGDesignParamEC2Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	CCheckDialogOpen::SetSoftwareRendering();

	Data2Dlg();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCPGDesignParamEC2Dlg::Data2Dlg()
{
	m_dGamm_C			=	m_pData->dGamma_C;
	m_dGamm_S			=	m_pData->dGamma_S;
	m_dGamm_M0		=	m_pData->dGamma_M0;
	m_dGamm_M1		=	m_pData->dGamma_M1;
	m_dGamm_V			=	m_pData->dGamma_V;
	m_dGamm_Ff		=	m_pData->dGamma_Ff;
	m_dGamm_Mf		=	m_pData->dGamma_Mf;
	m_dGamm_Mf_s	=	m_pData->dGamma_Mfs;
	m_dGamm_M_ser = m_pData->dGamma_M_ser;
	m_dt_Ld				= m_pData->dt_Ld;
	m_dk1				  = m_pData->dk1;
	m_dk2				  = m_pData->dk2;
	m_dk3				  = m_pData->dk3;
	m_dks				  = m_pData->dks;

	m_bBendig_Resist									= m_pData->bStrength[0];
	m_bResist_Vertical_Shear					= m_pData->bStrength[1];
	m_bResist_Later_Torsion_Buckling	= m_pData->bStrength[2];
	m_bResist_Trans_Force							= m_pData->bStrength[3];
	m_bResist_Longi_Shear							= m_pData->bStrength[4];
	m_bResist_Fatigue									= m_pData->bStrength[5];
	m_bStress_Limit					   				= m_pData->bService[0];
	m_bLongi_Shear						  			= m_pData->bService[1];
		
	UpdateData(FALSE);
}

void CDgnCPGDesignParamEC2Dlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_pData->dGamma_C		  = m_dGamm_C;
	m_pData->dGamma_S		  = m_dGamm_S;
	m_pData->dGamma_M0	  = m_dGamm_M0;
	m_pData->dGamma_M1	  = m_dGamm_M1;
	m_pData->dGamma_V		  = m_dGamm_V;
	m_pData->dGamma_Ff	  = m_dGamm_Ff;
	m_pData->dGamma_Mf	  = m_dGamm_Mf;
	m_pData->dGamma_Mfs   = m_dGamm_Mf_s;
	m_pData->dGamma_M_ser = m_dGamm_M_ser;
	m_pData->dt_Ld			  = m_dt_Ld;
	m_pData->dk1			    = m_dk1;
	m_pData->dk2	  		  = m_dk2;
	m_pData->dk3		  	  = m_dk3;
	m_pData->dks			    = m_dks;

	m_pData->bStrength[0] = m_bBendig_Resist;
	m_pData->bStrength[1] = m_bResist_Vertical_Shear;
	m_pData->bStrength[2] = m_bResist_Later_Torsion_Buckling;
	m_pData->bStrength[3] = m_bResist_Trans_Force;
	m_pData->bStrength[4] = m_bResist_Longi_Shear;
	m_pData->bStrength[5] = m_bResist_Fatigue;	
	m_pData->bService[0]  = m_bStress_Limit;	
	m_pData->bService[1]  = m_bLongi_Shear;	
}


BEGIN_MESSAGE_MAP(CDgnCPGDesignParamEC2Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnCPGDesignParamEC2Dlg)
	ON_BN_CLICKED(IDOK, OnOk)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamEC2Dlg message handlers

void CDgnCPGDesignParamEC2Dlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();		
	CChildDialog::OnOK();		
}

void CDgnCPGDesignParamEC2Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}




