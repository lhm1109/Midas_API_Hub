// CDgnCPGDesignParamAASHTO07Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCPGDesignParamAASHTO07Dlg.h"

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
// CDgnCPGDesignParamAASHTO07Dlg dialog


CDgnCPGDesignParamAASHTO07Dlg::CDgnCPGDesignParamAASHTO07Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnCPGDesignParamAASHTO07Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCPGDesignParamAASHTO07Dlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(pParent);
	m_pParent = (CDgnCPGDesignParamBaseDlg*)pParent;  
}

CDgnCPGDesignParamAASHTO07Dlg::~CDgnCPGDesignParamAASHTO07Dlg()
{
	
}

void CDgnCPGDesignParamAASHTO07Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCPGDesignParamAASHTO07Dlg)
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_Y_EDIT,		m_dPhy_y);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_U_EDIT,		m_dPhy_u);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_C_EDIT,		m_dPhy_c);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_F_EDIT,		m_dPhy_f);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_V_EDIT,		m_dPhy_v);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_SE_EDIT,	m_dPhy_se);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_B_EDIT,		m_dPhy_b);

	//DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMMA_EDIT,   m_dGamma);

	DDX_Radio(pDX, IDC_DGN_CPG_PARAM_SINGLE_RDO,   m_GirdernType);
	//DDX_Radio(pDX, IDC_DGN_CPG_PARAM_STRAIGHT_RDO, m_BridgeType);

	DDX_Check(pDX, IDC_DGN_CPG_PARAM_FLEX_CHK,					m_bFlexure);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_SHEAR_CHK,					m_bShear);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_SERVICE_CHK,       m_bServiceLimitSate);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_CONSTRUCT_CHK,			m_bConstructibility);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_FATIGUE_CHK,				m_bFatigueLimitState);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_SUB_CHK,						m_bSubMembers);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_TOR_WARPING_CHK,m_bTorWarping);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_A6_CHK,            m_bAppendix6);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_StVT_Stress_CHK,   m_bDistortion);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_MnLIMIT_CHK,       m_b1p3RhMy);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_PostBuckling_CHK,  m_bPBTFA4SH);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_LONG_TERM_SECT_PROP_CHK,  m_bLongTermSectProp);
	//}}AFX_DATA_MAP
}

BOOL CDgnCPGDesignParamAASHTO07Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	CCheckDialogOpen::SetSoftwareRendering();

	Data2Dlg();	
	OnSelectGirderType();

	ShowHideControls();
	AlignCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDgnCPGDesignParamAASHTO07Dlg::Data2Dlg()
{
	m_dPhy_y = m_pData->dPhi_y;
	m_dPhy_u = m_pData->dPhi_u;
	m_dPhy_c = m_pData->dPhi_c;
	m_dPhy_f = m_pData->dPhi_f;
	m_dPhy_v = m_pData->dPhi_v;
	m_dPhy_se = m_pData->dPhi_se;
	m_dPhy_b = m_pData->dPhi_b;
	//m_dGamma = m_pData->dCSLcbFactor[0];
	m_GirdernType = m_pData->nGirderType;
	//m_BridgeType  = m_pData->nConsiderPv;

	m_bFlexure           = m_pData->bDesignParameter[0];
	m_bShear             = m_pData->bDesignParameter[1];
	m_bServiceLimitSate  = m_pData->bDesignParameter[2];
	m_bConstructibility  = m_pData->bDesignParameter[3];
	m_bFatigueLimitState = m_pData->bDesignParameter[4];
	m_bSubMembers        = m_pData->bDesignParameter[5];
	m_bTorWarping = m_pData->bDesignParameter[6];

	m_bAppendix6         = m_pData->bAppendix6;
		
	m_bDistortion       = m_pData->bDistortion;       
	m_b1p3RhMy          = m_pData->b1p3RhMy;          
	m_bPBTFA4SH        = m_pData->bPBTFA4SH;        
	m_bLongTermSectProp = m_pData->bLongTermSectProp;

	UpdateData(FALSE);
}

void CDgnCPGDesignParamAASHTO07Dlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_pData->dPhi_y = m_dPhy_y;
	m_pData->dPhi_u = m_dPhy_u;
	m_pData->dPhi_c = m_dPhy_c;
	m_pData->dPhi_f = m_dPhy_f;
	m_pData->dPhi_v = m_dPhy_v;
	m_pData->dPhi_se= m_dPhy_se;
	m_pData->dPhi_b = m_dPhy_b;
	//m_pData->dCSLcbFactor[0] = m_dGamma;
	m_pData->nGirderType = m_GirdernType;
	//m_pData->nConsiderPv = m_BridgeType;

	m_pData->bDesignParameter[0] = m_bFlexure          ;
	m_pData->bDesignParameter[1] = m_bShear            ;
	m_pData->bDesignParameter[2] = m_bServiceLimitSate ;
	m_pData->bDesignParameter[3] = m_bConstructibility ;
	m_pData->bDesignParameter[4] = m_bFatigueLimitState;
	m_pData->bDesignParameter[5] = m_bSubMembers       ; 
	m_pData->bDesignParameter[6] = m_bTorWarping;

	m_pData->bAppendix6 = m_bAppendix6;

	m_pData->bDistortion  = m_bDistortion;       
	m_pData->b1p3RhMy     = m_b1p3RhMy;          
	m_pData->bPBTFA4SH   = m_bPBTFA4SH;        
	m_pData->bLongTermSectProp = m_bLongTermSectProp;
}

void CDgnCPGDesignParamAASHTO07Dlg::ShowHideControls()
{
	int nCode = m_pParent->GetCurSelDgnCode();
	
	if (nCode == AASHTO_LRFD17_CSG || nCode == AASHTO_LRFD20_CSG )
	{
		GetDlgItem(IDC_DGN_CPG_PARAM_TOR_WARPING_CHK)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_PARAM_TOR_WARPING_CHK)->ShowWindow(SW_HIDE);
	}

	if (nCode == AASHTO_LRFD16_CSG || nCode == AASHTO_LRFD17_CSG || nCode == AASHTO_LRFD20_CSG )
	{
		GetDlgItem(IDC_DGN_CPG_PARAM_LONG_TERM_SECT_PROP_CHK)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_PARAM_LONG_TERM_SECT_PROP_CHK)->ShowWindow(SW_HIDE);
	}
}

void CDgnCPGDesignParamAASHTO07Dlg::AlignCtrl()
{
	// Group Box 크기
	CRect rectGrp, rectRef, rectMove;
	int nYDist;

	int nCode = m_pParent->GetCurSelDgnCode();

	GetDlgItem(IDC_WG_DGN_STATIC0)->GetWindowRect(rectGrp);

	if(nCode == AASHTO_LRFD16_CSG )
	{
		GetDlgItem(IDC_DGN_CPG_PARAM_LONG_TERM_SECT_PROP_CHK)->GetWindowRect(rectRef);
		GetDlgItem(IDC_WG_DGN_STATIC0)->GetWindowRect(rectMove);
		nYDist = rectRef.bottom - rectMove.bottom + globalUtils.ScaleByDPI(4);
	}
	else if(nCode == AASHTO_LRFD17_CSG || nCode == AASHTO_LRFD20_CSG )
	{
		GetDlgItem(IDC_DGN_CPG_PARAM_TOR_WARPING_CHK)->GetWindowRect(rectRef);
		GetDlgItem(IDC_WG_DGN_STATIC0)->GetWindowRect(rectMove);
		nYDist = rectRef.bottom - rectMove.bottom + globalUtils.ScaleByDPI(4);
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_PARAM_PostBuckling_CHK)->GetWindowRect(rectRef);
		GetDlgItem(IDC_WG_DGN_STATIC0)->GetWindowRect(rectMove);
		nYDist = rectRef.bottom - rectMove.bottom + globalUtils.ScaleByDPI(4);
	}
	
	rectGrp.bottom = rectGrp.bottom + nYDist;

	ScreenToClient(rectGrp);
	GetDlgItem(IDC_WG_DGN_STATIC0)->MoveWindow(rectGrp);

	// Ctrl 위치 수정
	CArray<UINT,UINT> aCtrl;		
	aCtrl.Add(IDC_DNG_CPG_PARAM_GROUP2);
	aCtrl.Add(IDC_DGN_CPG_PARAM_FLEX_CHK);
	aCtrl.Add(IDC_DGN_CPG_PARAM_SHEAR_CHK);
	aCtrl.Add(IDC_DGN_CPG_PARAM_SERVICE_CHK);
	aCtrl.Add(IDC_DGN_CPG_PARAM_CONSTRUCT_CHK);
	aCtrl.Add(IDC_DGN_CPG_PARAM_FATIGUE_CHK);
	aCtrl.Add(IDC_DGN_CPG_PARAM_SUB_CHK);

	GetDlgItem(IDC_WG_DGN_STATIC0)->GetWindowRect(rectRef);
	GetDlgItem(IDC_DNG_CPG_PARAM_GROUP2)->GetWindowRect(rectMove);
	nYDist = rectRef.bottom - rectMove.top + globalUtils.ScaleByDPI(4);

	CDlgUtil::CtrlMoveDistY(this, aCtrl, nYDist);

	// ResizeDialog
	UINT nLastCtrlID;
	nLastCtrlID = IDC_DNG_CPG_PARAM_GROUP2;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom;// +globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

BEGIN_MESSAGE_MAP(CDgnCPGDesignParamAASHTO07Dlg, CChildDialog)
//{{AFX_MSG_MAP(CDgnCPGDesignParamAASHTO07Dlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_DGN_CPG_PARAM_SINGLE_RDO,   OnSelectGirderType)
	ON_BN_CLICKED(IDC_DGN_CPG_PARAM_MULTI_RDO,    OnSelectGirderType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamAASHTO07Dlg message handlers

void CDgnCPGDesignParamAASHTO07Dlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();		
	CChildDialog::OnOK();		
}

void CDgnCPGDesignParamAASHTO07Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}


void CDgnCPGDesignParamAASHTO07Dlg::OnSelectGirderType() 
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




