#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCPGDesignParamIRC22Dlg.h"

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

CDgnCPGDesignParamIRC22Dlg::CDgnCPGDesignParamIRC22Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnCPGDesignParamIRC22Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCPGDesignParamIRC22Dlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(pParent);
	m_pParent = (CDgnCPGDesignParamBaseDlg*)pParent;  
}

CDgnCPGDesignParamIRC22Dlg::~CDgnCPGDesignParamIRC22Dlg()
{

}

void CDgnCPGDesignParamIRC22Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCPGDesignParamIRC22Dlg)
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_C_EDIT,		m_dGamm_C);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_S_EDIT,		m_dGamm_S);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_M0_EDIT,		m_dGamma_M0_IRC);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_M1_EDIT,		m_dGamma_M1_IRC);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_V_EDIT,		m_dGamm_V);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_FF_EDIT,		m_dGamm_Ff);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_MF_EDIT,		m_dGamm_Mf);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_MF_S_EDIT, m_dGamm_Mf_s);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMMA_MSER_EDT, m_dGamm_M_ser);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_T_LD_EDIT,      m_dLoadCycles);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_K1_EDT,         m_dSteelStressLim);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_K2_EDT,         m_dConcStressLim);
	//DDX_Text(pDX, IDC_DGN_CPG_PARAM_K3_EDT,         m_dk3);
	//DDX_Text(pDX, IDC_DGN_CPG_PARAM_KS_EDT,         m_dks);

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

BOOL CDgnCPGDesignParamIRC22Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	CCheckDialogOpen::SetSoftwareRendering();

	Data2Dlg();	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCPGDesignParamIRC22Dlg::Data2Dlg()
{
	if ( m_pData->dGamma_M0_IRC<0.0000009 || abs(m_pData->dGamma_M0_IRC)>400)
	{
		m_pData->dGamma_M0_IRC = 1.1;
	}
	if (m_pData->dGamma_M1_IRC<0.0000009 || abs(m_pData->dGamma_M1_IRC)>400)
	{
		m_pData->dGamma_M1_IRC = 1.25;
	}

	m_dGamm_C			=	m_pData->dGamma_C;
	m_dGamm_S			=	m_pData->dGamma_S;
	m_dGamma_M0_IRC		=	m_pData->dGamma_M0_IRC;
	m_dGamma_M1_IRC		=	m_pData->dGamma_M1_IRC;
 	m_dGamm_M0		=	m_pData->dGamma_M0_IRC;
 	m_dGamm_M1		=	m_pData->dGamma_M1_IRC;
	m_dGamm_V			=	m_pData->dGamma_V;
	m_dGamm_Ff		=	m_pData->dGamma_Ff;
	m_dGamm_Mf		=	m_pData->dGamma_Mf;
	m_dGamm_Mf_s	=	m_pData->dGamma_Mfs;
	m_dGamm_M_ser = m_pData->dGamma_M_ser;
	m_dLoadCycles			   = m_pData->dLoadCycles			;
	m_dSteelStressLim		 = m_pData->dSteelStressLim	;
	m_dConcStressLim		 = m_pData->dConcStressLim	 ;
	//m_dk3				  = m_pData->dk3;
	//m_dks				  = m_pData->dks;

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

void CDgnCPGDesignParamIRC22Dlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_pData->dGamma_C		  = m_dGamm_C;
	m_pData->dGamma_S		  = m_dGamm_S;
	m_pData->dGamma_M0_IRC	  =m_dGamma_M0_IRC;
	m_pData->dGamma_M1_IRC	  =m_dGamma_M1_IRC;
	m_pData->dGamma_M0	  =m_dGamma_M0_IRC;
	m_pData->dGamma_M1	  =m_dGamma_M1_IRC;
	m_pData->dGamma_V		  = m_dGamm_V;
	m_pData->dGamma_Ff	  = m_dGamm_Ff;
	m_pData->dGamma_Mf	  = m_dGamm_Mf;
	m_pData->dGamma_Mfs   = m_dGamm_Mf_s;
	m_pData->dGamma_M_ser = m_dGamm_M_ser;
	m_pData->dLoadCycles				= m_dLoadCycles			 ;
	m_pData->dSteelStressLim	  = m_dSteelStressLim	 ;
	m_pData->dConcStressLim		  = m_dConcStressLim	 ;
	//m_pData->dk3		  	  = m_dk3;
	//m_pData->dks			    = m_dks;

	m_pData->bStrength[0] = m_bBendig_Resist;
	m_pData->bStrength[1] = m_bResist_Vertical_Shear;
	m_pData->bStrength[2] = m_bResist_Later_Torsion_Buckling;
	m_pData->bStrength[3] = m_bResist_Trans_Force;
	m_pData->bStrength[4] = m_bResist_Longi_Shear;
	m_pData->bStrength[5] = m_bResist_Fatigue;	
	m_pData->bService[0]  = m_bStress_Limit;	
	m_pData->bService[1]  = m_bLongi_Shear;	
}


BEGIN_MESSAGE_MAP(CDgnCPGDesignParamIRC22Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnCPGDesignParamIRC22Dlg)
	ON_BN_CLICKED(IDOK, OnOk)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamIRC22Dlg message handlers

void CDgnCPGDesignParamIRC22Dlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();		
	CChildDialog::OnOK();		
}

void CDgnCPGDesignParamIRC22Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CChildDialog::OnCancel();
}


