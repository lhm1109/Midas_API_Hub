#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCPGDesignParamIRC22_15_Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"
#include "..\wg_base\wg_base_MsgDll.h"

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

CDgnCPGDesignParamIRC22_15_Dlg::CDgnCPGDesignParamIRC22_15_Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnCPGDesignParamIRC22_15_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCPGDesignParamIRC22_15_Dlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(pParent);
	m_pParent = (CDgnCPGDesignParamBaseDlg*)pParent;  
}

CDgnCPGDesignParamIRC22_15_Dlg::~CDgnCPGDesignParamIRC22_15_Dlg()
{

}

void CDgnCPGDesignParamIRC22_15_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCPGDesignParamIRC22_15_Dlg)
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_C_EDIT,		m_dGamm_C);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_S_EDIT,		m_dGamm_C_Acci);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_M0_EDIT,		m_dGamm_M0);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_M1_EDIT,		m_dGamm_M1);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_V_EDIT,		m_dGamm_s);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_FF_EDIT,		m_dGamm_V);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_MF_EDIT,		m_dGamm_Ff);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMM_MF_S_EDIT, m_dGamm_Mf);
	/*DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMMA_MSER_EDT, m_dGamm_M_ser);*/
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_T_LD_EDIT,      m_dLoadCycles);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_K1_EDT,         m_dk1);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_K3_EDT,         m_dk3);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_K4_EDT,         m_dk4);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_K6_EDT2,        m_dk6);
	//Deflection Control
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_DEFLN_CTRL,     m_dDeflnCtrl);
	DDX_Control(pDX, IDC_DGN_CPG_PARAM_DEFLN_CTRL_UNIT,     m_DeflUnit);

	//Crack Width
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_K3_CRACK_EDT,   m_dCrackk3);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_K4_CRACK_EDT,   m_dCrackk4);

	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_CMB,   m_cmbIRCExpos);

	DDX_Check(pDX, IDC_DGN_CPG_PARAM_BEND_RESIST_CHECK,										m_bBendig_Resist);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_RESIST_VERTICAL_SHEAR_CHECK,					m_bResist_Vertical_Shear);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_RESIST_LATER_TORSION_BUCKLING_CHECK, m_bResist_Later_Torsion_Buckling);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_RESIST_TRANS_FORCE_CHECK,						m_bResist_Trans_Force);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_RESIST_LONG_SHEAR_CHECK,							m_bResist_Longi_Shear);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_RESIST_FATIGUE_CHECK,								m_bResist_Fatigue);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_RESIST_FATIGUE_CHECK2,								m_bPostBucklTensField);
	
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_STRESS_LIMIT_CHK,			     					m_bStress_Limit);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_LONGI_SHEAR_CHK,						      		m_bLongi_Shear);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_LONGI_SHEAR_CHK2,						      	m_bDeflnCtrl);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_LONGI_SHEAR_CHK3,						      	m_bCrackWidthChk);
	//}}AFX_DATA_MAP
}

BOOL CDgnCPGDesignParamIRC22_15_Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	CCheckDialogOpen::SetSoftwareRendering();
	GetDlgItem(IDC_DGN_CPG_PARAM_K4_EDT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_CPG_PARAM_T_LD_EDIT)->EnableWindow(FALSE);//CIVIL-5418
 
	Data2Dlg();	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCPGDesignParamIRC22_15_Dlg::Data2Dlg()
{
	if ( m_pData->dGamma_M0_IRC<0.0000009 || abs(m_pData->dGamma_M0_IRC)>400)
	{
		m_pData->dGamma_M0_IRC = 1.1;
	}
	if (m_pData->dGamma_M1_IRC<0.0000009 || abs(m_pData->dGamma_M1_IRC)>400)
	{
		m_pData->dGamma_M1_IRC = 1.25;
	}
	if (m_pData->dGamma_S == 1.15 && m_pData->dGamma_Mfs == 1.0 && m_pData->dGamma_M_ser == 1.0) SetDefaultData();
	else InitComboBox_IRC112();
	m_DeflUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_dGamm_C			=	m_pData->dGamma_C;
	m_dGamm_C_Acci			=	m_pData->dGamma_S;
	m_dGamm_M0		=	m_pData->dGamma_M0_IRC;
	m_dGamm_M1		=	m_pData->dGamma_M1_IRC;

//  	m_dGamm_M0		=	m_pData->dGamma_M0_IRC;
//  	m_dGamm_M1		=	m_pData->dGamma_M1_IRC;
	m_dGamm_s			=	m_pData->dGamma_V;
	m_dGamm_V		=	m_pData->dGamma_Ff;
	m_dGamm_Ff		=	m_pData->dGamma_Mf;
	m_dGamm_Mf	=	m_pData->dGamma_Mfs;
	/*m_dGamm_M_ser = m_pData->dGamma_M_ser;*/
	m_dLoadCycles			   = m_pData->dLoadCycles			;
	m_dk1                = m_pData->dk1;
	m_dk3                = m_pData->dk2;
	m_dk4                = 1.0;
	m_dk6                = m_pData->dks;

	m_dDeflnCtrl         = m_pData->dSteelStressLim;

	m_dCrackk3           = m_pData->dConcStressLim	 ;
	m_dCrackk4           = m_pData->dk3	 ;

	m_iExpClass          = m_pData->nConsiderPv;
	m_cmbIRCExpos.SetCurSel(m_iExpClass);
 
// 	m_dSteelStressLim		 = m_pData->dSteelStressLim	;
// 	m_dConcStressLim		 = m_pData->dConcStressLim	 ;
	//m_dk3				  = m_pData->dk3;
	//m_dks				  = m_pData->dks;

	m_bBendig_Resist									= m_pData->bStrength[0];
	m_bResist_Vertical_Shear					= m_pData->bStrength[1];
	m_bResist_Later_Torsion_Buckling	= m_pData->bStrength[2];
	m_bResist_Trans_Force							= m_pData->bStrength[3];
	m_bResist_Longi_Shear							= m_pData->bStrength[4];
	m_bResist_Fatigue									= m_pData->bStrength[5];
	m_bPostBucklTensField             = m_pData->bPBTFA4SH;

	m_bStress_Limit					   				= m_pData->bService[0];
	m_bLongi_Shear						  			= m_pData->bService[1];
	m_bDeflnCtrl                      = m_pData->bConsidermb7bm8;
	m_bCrackWidthChk                  = m_pData->bConsidermb9;

	UpdateData(FALSE);
}

void CDgnCPGDesignParamIRC22_15_Dlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_pData->dGamma_C		  = m_dGamm_C;
	m_pData->dGamma_S		  = m_dGamm_C_Acci;//m_dGamm_S;
	m_pData->dGamma_M0_IRC	  =m_dGamm_M0;
	m_pData->dGamma_M1_IRC	  =m_dGamm_M1;
	m_pData->dGamma_M0	  = m_dGamm_M0;
	m_pData->dGamma_M1	  = m_dGamm_M1;
	m_pData->dGamma_V		  = m_dGamm_s;//m_dGamm_V;
	m_pData->dGamma_Ff	  = m_dGamm_V;//m_dGamm_Ff;
	m_pData->dGamma_Mf	  = m_dGamm_Ff;//m_dGamm_Mf;
	m_pData->dGamma_Mfs   = m_dGamm_Mf;//m_dGamm_Mf_s;
	m_pData->dGamma_M_ser = 0.0001;
	m_pData->dLoadCycles				= m_dLoadCycles			 ;
	m_pData->dk1          = m_dk1;
	m_pData->dk2          = m_dk3;
	m_pData->dks          = m_dk6;
	

	m_pData->dSteelStressLim	  = m_dDeflnCtrl;//m_dSteelStressLim	 ;
	m_pData->dConcStressLim		  = m_dCrackk3;//m_dConcStressLim	 ;
	m_pData->dk3	        = m_dCrackk4;

	m_iExpClass          = m_cmbIRCExpos.GetCurSel();
	m_pData->nConsiderPv  = m_iExpClass;
	//m_pData->dk3		  	  = m_dk3;
	//m_pData->dks			    = m_dks;

	m_pData->bStrength[0] = m_bBendig_Resist;
	m_pData->bStrength[1] = m_bResist_Vertical_Shear;
	m_pData->bStrength[2] = m_bResist_Later_Torsion_Buckling;
	m_pData->bStrength[3] = m_bResist_Trans_Force;
	m_pData->bStrength[4] = m_bResist_Longi_Shear;
	m_pData->bStrength[5] = m_bResist_Fatigue;	
	m_pData->bPBTFA4SH    = m_bPostBucklTensField;

	m_pData->bService[0]  = m_bStress_Limit;	
	m_pData->bService[1]  = m_bLongi_Shear;	
	m_pData->bConsidermb7bm8  =  m_bDeflnCtrl       ;
	m_pData->bConsidermb9     =  m_bCrackWidthChk   ;
}


BEGIN_MESSAGE_MAP(CDgnCPGDesignParamIRC22_15_Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnCPGDesignParamIRC22_15_Dlg)
	ON_BN_CLICKED(IDOK, OnOk)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamIRC22_15_Dlg message handlers

void CDgnCPGDesignParamIRC22_15_Dlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();		
	CChildDialog::OnOK();		
}

void CDgnCPGDesignParamIRC22_15_Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CChildDialog::OnCancel();
}


void CDgnCPGDesignParamIRC22_15_Dlg::InitComboBox_IRC112()
{
	CString strCombo[4] = {_LS(IDS_DGN_PSC_EXPOSURE_CLASS_Moderate), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Severe), _LS(IDS_DGN_PSC_EXPOSURE_CLASS_Very_Severe),
		_LS(IDS_DGN_PSC_EXPOSURE_CLASS_Extreme)};

	m_cmbIRCExpos.ResetContent();
	

	for(int i=0; i<4; i++)
	{
		m_cmbIRCExpos.AddString(strCombo[i]);
		
	}
	m_cmbIRCExpos.SetCurSel(0);
}

void CDgnCPGDesignParamIRC22_15_Dlg::SetDefaultData()
{
	//Default Data
	m_pData->dGamma_C = 1.5;
	m_pData->dGamma_S = 1.2;
	m_pData->dGamma_M0_IRC = 1.1;
	m_pData->dGamma_M1_IRC = 1.25;
	m_pData->dGamma_M0  = 1.1;
	m_pData->dGamma_M1  = 1.25;
	m_pData->dGamma_V = 1.15;
	m_pData->dGamma_Ff = 1.25;
	m_pData->dGamma_Mf = 1.0;
	m_pData->dGamma_Mfs = 1.35;
	m_pData->dk1 = 0.48;
	m_pData->dk2 = 0.8;
	m_pData->dks = 0.87;
	m_pData->dSteelStressLim = 600.;
	m_pData->dConcStressLim = 3.4;
	m_pData->dk3	= 0.425;
	m_pData->nConsiderPv = 0;
	m_pData->dLoadCycles = 2000000;//CIVIL-5418
	InitComboBox_IRC112();
}