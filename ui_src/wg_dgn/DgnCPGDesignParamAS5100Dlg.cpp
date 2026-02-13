// CDgnCPGDesignParamAS5100Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCPGDesignParamAS5100Dlg.h"

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

#include "..\wg_base\TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamAS5100Dlg dialog


CDgnCPGDesignParamAS5100Dlg::CDgnCPGDesignParamAS5100Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnCPGDesignParamAS5100Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCPGDesignParamAS5100Dlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(pParent);
	m_pParent = (CDgnCPGDesignParamBaseDlg*)pParent;  
}

CDgnCPGDesignParamAS5100Dlg::~CDgnCPGDesignParamAS5100Dlg()
{
	
}

void CDgnCPGDesignParamAS5100Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCPGDesignParamAS5100Dlg)
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_AS5100_FLEXURE_EDIT,	m_dPhy_FL);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_AS5100_SHEAR_EDIT,		m_dPhy_SH);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_AS5100_STIFFENER_EDIT,	m_dPhy_ST);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_AS5100_SHEARCON_EDIT,   m_dPhy_SC);
	
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_AS5100_FLEXURE_CHK, m_bUlt_FL);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_AS5100_SHEAR_CHK,   m_bUlt_FH);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_AS5100_F_S_T_CHK,   m_bUlt_FST);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_AS5100_L_SHEAR_CHK, m_bUlt_LS);

	DDX_Check(pDX, IDC_DGN_CPG_PARAM_AS5100_SER_STRESS_CHK,  m_bSer_ST);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_AS5100_SER_L_SHEAR_CHK, m_bSer_LS);
	//}}AFX_DATA_MAP
}

BOOL CDgnCPGDesignParamAS5100Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	CCheckDialogOpen::SetSoftwareRendering();

	Data2Dlg();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCPGDesignParamAS5100Dlg::Data2Dlg()
{
	m_dPhy_FL = m_pData->dPhi_f;    // flexure
	m_dPhy_SH = m_pData->dPhi_v;    // shear
	m_dPhy_ST = m_pData->dPhi_b;    // siffener 
	m_dPhy_SC = m_pData->dPhi_se;   // shear connector

	m_bUlt_FL = m_pData->bDesignParameter[0];
	m_bUlt_FH = m_pData->bDesignParameter[1];
	m_bUlt_FST= m_pData->bDesignParameter[2];
	m_bUlt_LS = m_pData->bDesignParameter[3];

	m_bSer_ST = m_pData->bService[0];
	m_bSer_LS = m_pData->bService[1];

	UpdateData(FALSE);
}

void CDgnCPGDesignParamAS5100Dlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_pData->dPhi_f = m_dPhy_FL;
	m_pData->dPhi_v = m_dPhy_SH;
	m_pData->dPhi_b = m_dPhy_ST;
	m_pData->dPhi_se= m_dPhy_SC;

	m_pData->bDesignParameter[0] = m_bUlt_FL;
	m_pData->bDesignParameter[1] = m_bUlt_FH;
	m_pData->bDesignParameter[2] = m_bUlt_FST;
	m_pData->bDesignParameter[3] = m_bUlt_LS;

	m_pData->bService[0] = m_bSer_ST;
	m_pData->bService[1] = m_bSer_LS;
}


BEGIN_MESSAGE_MAP(CDgnCPGDesignParamAS5100Dlg, CChildDialog)
//{{AFX_MSG_MAP(CDgnCPGDesignParamAS5100Dlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamAS5100Dlg message handlers

void CDgnCPGDesignParamAS5100Dlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();		
	CChildDialog::OnOK();		
}

void CDgnCPGDesignParamAS5100Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}



