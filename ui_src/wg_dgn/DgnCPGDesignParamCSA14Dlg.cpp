// CDgnCPGDesignParamCSA14Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCPGDesignParamCSA14Dlg.h"

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
// CDgnCPGDesignParamCSA14Dlg dialog


CDgnCPGDesignParamCSA14Dlg::CDgnCPGDesignParamCSA14Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnCPGDesignParamCSA14Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCPGDesignParamCSA14Dlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(pParent);
	m_pParent = (CDgnCPGDesignParamBaseDlg*)pParent;  
}

CDgnCPGDesignParamCSA14Dlg::~CDgnCPGDesignParamCSA14Dlg()
{

}

void CDgnCPGDesignParamCSA14Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCPGDesignParamCSA14Dlg)
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_FLEXURAL_EDIT,		m_dPhi_s_Flex);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_SHEAR_EDIT,		    m_dPhi_s_Shear);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_COMPRESSION_EDIT,	m_dPhi_s_Comp);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_TENSION_EDIT,	  	m_dPhi_s_Tens);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_TORSION_EDIT,	  	m_dPhi_s_Tors);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_R_EDIT,	      m_dPhi_r);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_SC_EDIT,	  	m_dPhi_sc);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_C_EDIT,	    	m_dPhi_c);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_U_EDIT,		    m_dPhi_u);

	//DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMMA_EDIT,   m_dGamma);

	DDX_Radio(pDX, IDC_DGN_CPG_PARAM_SINGLE_RDO,   m_GirdernType);
	//DDX_Radio(pDX, IDC_DGN_CPG_PARAM_STRAIGHT_RDO, m_BridgeType);

	DDX_Check(pDX, IDC_DGN_CPG_PARAM_FLEX_CHK,					m_bFlexure);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_SHEAR_CHK,					m_bShear);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_FATIGUE_CHK,				m_bFatigueLimitState);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_STIFF_CHK,					m_bStiffener);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_SERV_CHK,					m_bServiceLimitSate);
	
	//DDX_Check(pDX, IDC_DGN_CPG_PARAM_CONSTRUCTSTAGE_CHK,m_bConstructionStage);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_StVT_Stress_CHK,   m_bDistortion);
	DDX_Check(pDX, IDC_DGN_CPG_CURVED_CHK,              m_bCurvedGirder);
	DDX_Check(pDX, IDC_DGN_CPG_CONS_CHK,                m_bConstructionStage);
	
	//}}AFX_DATA_MAP
}

BOOL CDgnCPGDesignParamCSA14Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	CCheckDialogOpen::SetSoftwareRendering();

	Data2Dlg();	

	GetDlgItem(IDC_DGN_CPG_PARAM_PHI_U_EDIT)->EnableWindow(FALSE);
	OnSelectGirderType();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDgnCPGDesignParamCSA14Dlg::Data2Dlg()
{
	m_dPhi_s_Flex  = m_pData->dPhi_f ;
	m_dPhi_s_Shear = m_pData->dPhi_v ;
	m_dPhi_s_Comp  = m_pData->dPhi_c ;
	m_dPhi_s_Tens  = m_pData->dPhi_s_Tens;
	m_dPhi_s_Tors  = m_pData->dPhi_s_Tos ;
	m_dPhi_r       = m_pData->dPhi_y ;
	m_dPhi_sc      = m_pData->dPhi_se ;
	m_dPhi_c       = m_pData->dPhi_b;
	m_dPhi_u       = m_pData->dPhi_u;
	//m_dGamma = m_pData->dCSLcbFactor[0];
	m_GirdernType = m_pData->nGirderType;
	//m_BridgeType  = m_pData->nConsiderPv;

	m_bFlexure           = m_pData->bDesignParameter[0];
	m_bShear             = m_pData->bDesignParameter[1];
	m_bServiceLimitSate  = m_pData->bDesignParameter[2];
	m_bFatigueLimitState = m_pData->bDesignParameter[4];
	m_bStiffener         = m_pData->bDesignParameter[5];
	//m_bConstructionStage = m_pData->bDesignParameter[6];
	m_bDistortion       = m_pData->bDistortion;       
	m_bConstructionStage = m_pData->bChkCS;
	m_bCurvedGirder      = m_pData->nConsiderPv;


	UpdateData(FALSE);
}

void CDgnCPGDesignParamCSA14Dlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_pData->dPhi_f      = m_dPhi_s_Flex ;
	m_pData->dPhi_v      = m_dPhi_s_Shear;
	m_pData->dPhi_c      = m_dPhi_s_Comp ;
	m_pData->dPhi_s_Tens = m_dPhi_s_Tens ;
	m_pData->dPhi_s_Tos  = m_dPhi_s_Tors ;
	m_pData->dPhi_y      = m_dPhi_r      ;
	m_pData->dPhi_se     = m_dPhi_sc     ;
	m_pData->dPhi_b      = m_dPhi_c      ;
	m_pData->dPhi_u      = m_dPhi_u      ;
	//m_pData->dCSLcbFm_dPhi_u  = m_actor[0] = m_dGamma;
	m_pData->nGirderType = m_GirdernType;
	//m_pData->nConsiderPv = m_BridgeType;

	m_pData->bDesignParameter[0] = m_bFlexure          ;
	m_pData->bDesignParameter[1] = m_bShear            ;
	m_pData->bDesignParameter[2] = m_bServiceLimitSate ;
	m_pData->bDesignParameter[4] = m_bFatigueLimitState;
	m_pData->bDesignParameter[5] = m_bStiffener        ; 
	
	//m_pData->bDesignParameter[6] = m_bConstructionStage;

	m_pData->bDistortion  = m_bDistortion;       
	m_pData->bChkCS       = m_bConstructionStage;
	m_pData->nConsiderPv  = m_bCurvedGirder;

}


BEGIN_MESSAGE_MAP(CDgnCPGDesignParamCSA14Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnCPGDesignParamCSA14Dlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_DGN_CPG_PARAM_SINGLE_RDO,   OnSelectGirderType)
	ON_BN_CLICKED(IDC_DGN_CPG_PARAM_MULTI_RDO,    OnSelectGirderType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamCSA14Dlg message handlers

void CDgnCPGDesignParamCSA14Dlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();		
	CChildDialog::OnOK();		
}

void CDgnCPGDesignParamCSA14Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CChildDialog::OnCancel();
}


void CDgnCPGDesignParamCSA14Dlg::OnSelectGirderType() 
{
	UpdateData(TRUE);

	if(m_GirdernType==0) 
	{
		m_bDistortion = 1;
		GetDlgItem(IDC_DGN_CPG_PARAM_StVT_Stress_CHK)->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_DGN_CPG_PARAM_StVT_Stress_CHK)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}




