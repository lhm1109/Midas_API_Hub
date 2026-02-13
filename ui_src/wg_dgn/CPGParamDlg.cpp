// CPGParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGParamDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\DBLib.h"

#include "..\wg_main\wg_main.h"
#include "NationalAnnexTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPGParamDlg dialog


CCPGParamDlg::CCPGParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCPGParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGParamDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}

CCPGParamDlg::~CCPGParamDlg()
{
	
}

void CCPGParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGParamDlg)
	DDX_Control(pDX, IDC_DGN_CPG_PARAM_CODE_COMBO, m_DesignCode);
	DDX_Control(pDX, IDC_DGN_CPG_NA_COMBO, m_SubCode);
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

BOOL CCPGParamDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CCheckDialogOpen::SetSoftwareRendering();

	m_DesignCode.ResetContent();
    m_SubCode.ResetContent();
	
	CStringArray arCsgCodeNa;
	CDBLib::GetCsgCodeNameList(arCsgCodeNa);
	for (int i = 0; i < arCsgCodeNa.GetSize(); ++i)
	{
#if defined(_RUS)
		CInitCtrl::ConvertEngRusDesignCode(arCsgCodeNa[i]);
#endif
		m_DesignCode.AddString(arCsgCodeNa[i]);
	}

    auto vString = CNationalAnnexTool::GetCpgNationalAnnex();
    for ( int i = 0; i<vString.size(); ++i )
    {
        m_SubCode.SetItemData(m_SubCode.AddString(vString[i]), CNationalAnnexTool::ConvertNationalAnnex(vString[i]));
    }

	m_MainData.Initialize();
	if(m_pDoc->m_pAttrCtrl->ExistCpgd())
	{
		m_pDoc->m_pAttrCtrl->GetCpgd(m_MainData);
	}
	else
	{
		m_pDoc->m_pAttrCtrl2->GetDgnCpgd(m_MainData);
	}
	
	LoadDlgData();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPGParamDlg::LoadDlgData()
{
	int Index=0;
	CString strCodeName = CDBLib::GetCsgCodeName(m_MainData.iDgnCode);
#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(strCodeName);
#endif
	if(strCodeName!=_T(""))	Index = m_DesignCode.FindStringExact(-1,strCodeName);
	m_DesignCode.SetCurSel(Index);	

    CString strNACode = CNationalAnnexTool::ConvertNationalAnnex(m_MainData.nSubCode);
    if ( strNACode!=_T("") ) Index = m_SubCode.FindStringExact(-1, strNACode);
    m_SubCode.SetCurSel(Index);

	m_dGamm_C			=	m_MainData.dGamma_C;
	m_dGamm_S			=	m_MainData.dGamma_S;
	m_dGamm_M0		=	m_MainData.dGamma_M0;
	m_dGamm_M1		=	m_MainData.dGamma_M1;
	m_dGamm_V			=	m_MainData.dGamma_V;
	m_dGamm_Ff		=	m_MainData.dGamma_Ff;
	m_dGamm_Mf		=	m_MainData.dGamma_Mf;
	m_dGamm_Mf_s	=	m_MainData.dGamma_Mfs;
	m_dGamm_M_ser = m_MainData.dGamma_M_ser;
	m_dt_Ld				= m_MainData.dt_Ld;
	m_dk1				  = m_MainData.dk1;
	m_dk2				  = m_MainData.dk2;
	m_dk3				  = m_MainData.dk3;
	m_dks				  = m_MainData.dks;

	m_bBendig_Resist									= m_MainData.bStrength[0];
	m_bResist_Vertical_Shear					= m_MainData.bStrength[1];
	m_bResist_Later_Torsion_Buckling	= m_MainData.bStrength[2];
	m_bResist_Trans_Force							= m_MainData.bStrength[3];
	m_bResist_Longi_Shear							= m_MainData.bStrength[4];
	m_bResist_Fatigue									= m_MainData.bStrength[5];
	m_bStress_Limit					   				= m_MainData.bService[0];
	m_bLongi_Shear						  			= m_MainData.bService[1];
		
	UpdateData(FALSE);
}

void CCPGParamDlg::SaveDlgData()
{
	UpdateData(TRUE);

	int nIndex = m_DesignCode.GetCurSel();
	CString strCodeNa=_T("");
	m_DesignCode.GetLBText(nIndex, strCodeNa);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCodeNa);
#endif
	m_MainData.iDgnCode = CDBLib::GetCsgCodeNo(strCodeNa);
    m_MainData.nSubCode = m_SubCode.GetItemData(m_SubCode.GetCurSel());

	m_MainData.dGamma_C		  = m_dGamm_C;
	m_MainData.dGamma_S		  = m_dGamm_S;
	m_MainData.dGamma_M0	  = m_dGamm_M0;
	m_MainData.dGamma_M1	  = m_dGamm_M1;
	m_MainData.dGamma_V		  = m_dGamm_V;
	m_MainData.dGamma_Ff	  = m_dGamm_Ff;
	m_MainData.dGamma_Mf	  = m_dGamm_Mf;
	m_MainData.dGamma_Mfs   = m_dGamm_Mf_s;
	m_MainData.dGamma_M_ser = m_dGamm_M_ser;
	m_MainData.dt_Ld			  = m_dt_Ld;
	m_MainData.dk1			    = m_dk1;
	m_MainData.dk2	  		  = m_dk2;
	m_MainData.dk3		  	  = m_dk3;
	m_MainData.dks			    = m_dks;

	m_MainData.bStrength[0] = m_bBendig_Resist;
	m_MainData.bStrength[1] = m_bResist_Vertical_Shear;
	m_MainData.bStrength[2] = m_bResist_Later_Torsion_Buckling;
	m_MainData.bStrength[3] = m_bResist_Trans_Force;
	m_MainData.bStrength[4] = m_bResist_Longi_Shear;
	m_MainData.bStrength[5] = m_bResist_Fatigue;	
	m_MainData.bService[0]  = m_bStress_Limit;	
	m_MainData.bService[1]  = m_bLongi_Shear;	
}


BEGIN_MESSAGE_MAP(CCPGParamDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCPGParamDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPGParamDlg message handlers

void CCPGParamDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	SaveDlgData();		

	if(!m_pDoc->m_pDataCtrl->AddCpgd(m_MainData))
		return;

	CDialogMove::OnOK();		
}

void CCPGParamDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}




