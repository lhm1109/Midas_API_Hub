// CDgnCPGDesignParamLSD12Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCPGDesignParamLSD12Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\DBCodeDef.h"

#include "..\wg_main\wg_main.h"

#include "DgnCPGDesignParamBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamLSD12Dlg dialog


CDgnCPGDesignParamLSD12Dlg::CDgnCPGDesignParamLSD12Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnCPGDesignParamLSD12Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCPGDesignParamLSD12Dlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(pParent);
	m_pParent = (CDgnCPGDesignParamBaseDlg*)pParent;  
}

CDgnCPGDesignParamLSD12Dlg::~CDgnCPGDesignParamLSD12Dlg()
{
	
}

void CDgnCPGDesignParamLSD12Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCPGDesignParamLSD12Dlg)
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_Y_EDIT,		m_dPhy_y);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_U_EDIT,		m_dPhy_u);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_C_EDIT,		m_dPhy_c);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_F_EDIT,		m_dPhy_f);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_V_EDIT,		m_dPhy_v);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_SE_EDIT,	m_dPhy_se);
	DDX_Text(pDX, IDC_DGN_CPG_PARAM_PHI_B_EDIT,		m_dPhy_b);

	//DDX_Text(pDX, IDC_DGN_CPG_PARAM_GAMMA_EDIT,   m_dGamma);

	DDX_Radio(pDX, IDC_DGN_CPG_PARAM_SINGLE_RDO,   m_GirdernType);
	DDX_Radio(pDX, IDC_DGN_CPG_PARAM_STRAIGHT_RDO,   m_BridgeType);

	DDX_Check(pDX, IDC_DGN_CPG_PARAM_FLEX_CHK,					m_bFlexure);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_SHEAR_CHK,					m_bShear);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_SERVICE_CHK,       m_bServiceLimitSate);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_CONSTRUCT_CHK,			m_bConstructibility);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_FATIGUE_CHK,				m_bFatigueLimitState);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_SUB_CHK,						m_bSubMembers);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK,				m_bWarpStress);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_PBTF_EFF_CHK,						m_bPBTF_Eff);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_MnLIMIT_KR_CHK,       m_b1p3RhMy);

	DDX_Check(pDX, IDC_DGN_CPG_PARAM_PTR_OPT_SUM_CHK,		m_bPtrOpt_Sum);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_PTR_OPT_DETAIL_CHK,       m_bPtrOpt_Detail);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_TOR_WARPING_CHK, m_bTorWarping);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_A6_CHK, m_bAppendix6);
	DDX_Check(pDX, IDC_DGN_CPG_PARAM_PM_CHK, m_bCompReinforcement);


	//DDX_Check(pDX, IDC_DGN_CPG_PARAM_CONSTRUCTSTAGE_CHK,m_bConstructionStage);
	//}}AFX_DATA_MAP
}

BOOL CDgnCPGDesignParamLSD12Dlg::OnInitDialog() 
{
	MInitCombo initCombo;

	CChildDialog::OnInitDialog();
	CCheckDialogOpen::SetSoftwareRendering();

	GetDlgItem(IDC_DGN_CPG_PARAM_CURVED_RDO)->EnableWindow(TRUE); 

	m_aCtrlCompRein.RemoveAll();
	m_aCtrlLimitStat.RemoveAll();
	m_aCtrlDgnParam.RemoveAll();
	m_aCtrlPrintOpt.RemoveAll();

	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlCompRein, IDC_DGN_CPG_PARAM_PM_GRP, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlLimitStat, IDC_WG_DGN_STATIC1, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlDgnParam, IDC_DNG_CPG_PARAM_GROUP2, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlPrintOpt, IDC_DNG_CPG_PARAM_PRT_OPT_GROUP, TRUE);


	int nCode = m_pParent->GetCurSelDgnCode();

	AlignCtrl(nCode);
	ShowHideCtrl(nCode);
// 	if(CProduct::GetTestEnvValue(_ULS(CSD Test)) == _ULS(yes))
// 	{
// 		GetDlgItem(IDC_DGN_CPG_PARAM_CURVED_RDO)->EnableWindow(TRUE); 
// 	}
// 	else
// 	{ 
// 		GetDlgItem(IDC_DGN_CPG_PARAM_CURVED_RDO)->EnableWindow(FALSE);	
// 	}

	Data2Dlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCPGDesignParamLSD12Dlg::AlignCtrl(int nCode)
{
// 	// make control group
// 	CArray<UINT, UINT> m_aCtrlCompRein;
// 	CArray<UINT, UINT> m_aCtrlLimitStat;
// 	CArray<UINT, UINT> m_aCtrlDgnParam;
// 	CArray<UINT, UINT> m_aCtrlPrintOpt;
// 
// 	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlCompRein, IDC_DGN_CPG_PARAM_PM_GRP, TRUE);
// 	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlLimitStat, IDC_WG_DGN_STATIC1, TRUE);
// 	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlDgnParam, IDC_DNG_CPG_PARAM_GROUP2, TRUE);
// 	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlPrintOpt, IDC_DNG_CPG_PARAM_PRT_OPT_GROUP, TRUE);

	CRect rRef;
	CRect rToMove;
	int nDistY;
	CRect rectResize;
	CArray<UINT, UINT> aControls;

	if (nCode == KSCE_LSD15_CSG)
	{
		//1) Girder Type for Box/Tub Section
		/// - resize
		GetDlgItem(IDC_DNG_CPG_PARAM_GROUP4)->GetWindowRect(rectResize);
		GetDlgItem(IDC_DGN_CPG_PARAM_SINGLE_RDO)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
		ScreenToClient(rectResize);
		GetDlgItem(IDC_DNG_CPG_PARAM_GROUP4)->MoveWindow(rectResize);

		//2) Bridge Type
		aControls.RemoveAll();
		aControls.Add(IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK);

		GetDlgItem(IDC_DGN_CPG_PARAM_STRAIGHT_RDO)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		aControls.RemoveAll();
		aControls.Add(IDC_WG_DGN_STATIC0);
		aControls.Add(IDC_DGN_CPG_PARAM_STRAIGHT_RDO);
		aControls.Add(IDC_DGN_CPG_PARAM_CURVED_RDO);
		aControls.Add(IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK);

		GetDlgItem(IDC_DNG_CPG_PARAM_GROUP4)->GetWindowRect(rRef);
		GetDlgItem(IDC_WG_DGN_STATIC0)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		//3) Option for Limit State  
		GetDlgItem(IDC_WG_DGN_STATIC0)->GetWindowRect(rRef);
		GetDlgItem(IDC_WG_DGN_STATIC1)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlLimitStat, nDistY);


		aControls.RemoveAll();
		aControls.Add(IDC_DGN_CPG_PARAM_MnLIMIT_KR_CHK);
		aControls.Add(IDC_DGN_CPG_PARAM_PBTF_EFF_CHK);
		aControls.Add(IDC_DGN_CPG_PARAM_TOR_WARPING_CHK);

		GetDlgItem(IDC_DGN_CPG_PARAM_A6_CHK)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CPG_PARAM_MnLIMIT_KR_CHK)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		/// - resize
		GetDlgItem(IDC_WG_DGN_STATIC1)->GetWindowRect(rectResize);
		GetDlgItem(IDC_DGN_CPG_PARAM_PBTF_EFF_CHK)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
		ScreenToClient(rectResize);
		GetDlgItem(IDC_WG_DGN_STATIC1)->MoveWindow(rectResize);
	}
	else if (nCode == KDS_24_14_31_2018_CSG)
	{
		//1) Girder Type for Box/Tub Section
		aControls.RemoveAll();
		aControls.Add(IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK);

		GetDlgItem(IDC_DGN_CPG_PARAM_SINGLE_RDO)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		// - resize
		CRect rectResize;
		GetDlgItem(IDC_DNG_CPG_PARAM_GROUP4)->GetWindowRect(rectResize);
		GetDlgItem(IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
		ScreenToClient(rectResize);
		GetDlgItem(IDC_DNG_CPG_PARAM_GROUP4)->MoveWindow(rectResize);

		//2) Bridge Type - Hide

		// Option for Plastic Moment -Move
		GetDlgItem(IDC_DNG_CPG_PARAM_GROUP4)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CPG_PARAM_PM_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlCompRein, nDistY);

		//3) Option for Limit State  
		aControls.RemoveAll();
		aControls.Add(IDC_DGN_CPG_PARAM_MnLIMIT_KR_CHK);
		aControls.Add(IDC_DGN_CPG_PARAM_PBTF_EFF_CHK);
		aControls.Add(IDC_DGN_CPG_PARAM_TOR_WARPING_CHK);
	
		GetDlgItem(IDC_WG_DGN_STATIC10)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CPG_PARAM_MnLIMIT_KR_CHK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(5);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		GetDlgItem(IDC_DGN_CPG_PARAM_PM_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_WG_DGN_STATIC1)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlLimitStat, nDistY);
	}
	else
		ASSERT(0);

	// Dgn Param
	GetDlgItem(IDC_WG_DGN_STATIC1)->GetWindowRect(rRef);
	GetDlgItem(IDC_DNG_CPG_PARAM_GROUP2)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlDgnParam, nDistY);

	// Print Option
	GetDlgItem(IDC_DNG_CPG_PARAM_GROUP2)->GetWindowRect(rRef);
	GetDlgItem(IDC_DNG_CPG_PARAM_PRT_OPT_GROUP)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlPrintOpt, nDistY);

	// ResizeDialog	
#if 0
	CRect rectRef, rectDlg;
	if (nCode == KSCE_LSD15_CSG && m_pData->nConsiderPv == 0)
	{
		GetDlgItem(IDC_DNG_CPG_PARAM_GROUP2)->GetWindowRect(rectRef);
	}
	else
	{
		GetDlgItem(IDC_DNG_CPG_PARAM_PRT_OPT_GROUP)->GetWindowRect(rectRef);
	}
	GetWindowRect(rectDlg);
	rectDlg.bottom = rectRef.bottom;
	MoveWindow(rectDlg);
#else
	UINT nLastCtrlID;
	nLastCtrlID = IDC_DNG_CPG_PARAM_PRT_OPT_GROUP;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom;// +globalUtils.ScaleByDPI(8);
	MoveWindow(r);
#endif

}

void CDgnCPGDesignParamLSD12Dlg::ShowHideCtrl(int nCode)
{
// 	CArray<UINT, UINT> aCtrlCompRein;
// 	CDlgUtil::GetCtrlIDByIncRect(this, aCtrlCompRein, IDC_DGN_CPG_PARAM_PM_GRP, TRUE);

	if (nCode == KSCE_LSD15_CSG)
	{
		// Limit State
		GetDlgItem(IDC_DGN_CPG_PARAM_A6_CHK           )->ShowWindow(FALSE);
		GetDlgItem(IDC_WG_DGN_STATIC10                 )->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_CPG_PARAM_TOR_WARPING_CHK)->ShowWindow(FALSE);

		CDlgUtil::CtrlShowHide(this, m_aCtrlCompRein, FALSE);
		//CDlgUtil::CtrlEnableDisable(this, aCtrlCompRein, FALSE);
	}
	else if (nCode == KDS_24_14_31_2018_CSG)
	{
		// Bridge Type
		GetDlgItem(IDC_WG_DGN_STATIC0            )->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_CPG_PARAM_STRAIGHT_RDO)->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_CPG_PARAM_CURVED_RDO  )->ShowWindow(FALSE);

		CDlgUtil::CtrlEnableDisable(this, m_aCtrlCompRein, TRUE);
	}
	else
		ASSERT(0);

	if (nCode == KSCE_LSD15_CSG)
	{
		if (m_pData->nConsiderPv == 0) // for straight bridge
		{
			GetDlgItem(IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_CPG_PARAM_PBTF_EFF_CHK)->EnableWindow(FALSE);

			GetDlgItem(IDC_DNG_CPG_PARAM_PRT_OPT_GROUP)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_CPG_PARAM_PTR_OPT_SUM_CHK)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_CPG_PARAM_PTR_OPT_DETAIL_CHK)->EnableWindow(FALSE);
		}
		else if (m_pData->nConsiderPv == 1) // for Curved bridge
		{
			GetDlgItem(IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_CPG_PARAM_PBTF_EFF_CHK)->EnableWindow(TRUE);

			GetDlgItem(IDC_DNG_CPG_PARAM_PRT_OPT_GROUP)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_CPG_PARAM_PTR_OPT_SUM_CHK)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_CPG_PARAM_PTR_OPT_DETAIL_CHK)->EnableWindow(TRUE);
		}
		else { ASSERT(0); }
	}
	else if (nCode == KDS_24_14_31_2018_CSG)
	{
		GetDlgItem(IDC_DNG_CPG_PARAM_PRT_OPT_GROUP)->ShowWindow(TRUE);
		GetDlgItem(IDC_DGN_CPG_PARAM_PTR_OPT_SUM_CHK)->ShowWindow(TRUE);
		GetDlgItem(IDC_DGN_CPG_PARAM_PTR_OPT_DETAIL_CHK)->ShowWindow(TRUE);
	}
	else { ASSERT(0); }
}

void CDgnCPGDesignParamLSD12Dlg::Data2Dlg()
{
	m_dPhy_y = m_pData->dPhi_y;
	m_dPhy_u = m_pData->dPhi_u;
	m_dPhy_c = m_pData->dPhi_c;
	m_dPhy_f = m_pData->dPhi_f;
	m_dPhy_v = m_pData->dPhi_v;
	m_dPhy_se = m_pData->dPhi_se;
	m_dPhy_b = m_pData->dPhi_b;
	m_GirdernType = m_pData->nGirderType;
	m_BridgeType  = m_pData->nConsiderPv;

	m_bFlexure           = m_pData->bDesignParameter[0];
	m_bShear             = m_pData->bDesignParameter[1];
	m_bServiceLimitSate  = m_pData->bDesignParameter[2];
	m_bConstructibility  = m_pData->bDesignParameter[3];
	m_bFatigueLimitState = m_pData->bDesignParameter[4];
	m_bSubMembers        = m_pData->bDesignParameter[5];
	m_bTorWarping        = m_pData->bDesignParameter[6];

	m_bAppendix6 = m_pData->bAppendix6;

	m_bWarpStress = m_pData->bDistortion;
	m_bPBTF_Eff = m_pData->bPBTFA4SH;
	m_b1p3RhMy  = m_pData->b1p3RhMy;     

	m_bPtrOpt_Sum = m_pData->bPrtOptSum;
	m_bPtrOpt_Detail =  m_pData->bPrtOptDetail;

	m_bCompReinforcement = m_pData->bCompReinforcement;
		
	UpdateData(FALSE);
}

void CDgnCPGDesignParamLSD12Dlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_pData->dPhi_y = m_dPhy_y;
	m_pData->dPhi_u = m_dPhy_u;
	m_pData->dPhi_c = m_dPhy_c;
	m_pData->dPhi_f = m_dPhy_f;
	m_pData->dPhi_v = m_dPhy_v;
	m_pData->dPhi_se= m_dPhy_se;
	m_pData->dPhi_b = m_dPhy_b;
	m_pData->nGirderType = m_GirdernType;
	m_pData->nConsiderPv = m_BridgeType; 

	if ( m_pParent->m_strCurCode == CSGCODE_KDS_24_14_31_2018 )
	{
		m_pData->iDgnCode = KDS_24_14_31_2018_CSG;
	}
	else
	{
		if(m_BridgeType == 1)     {m_pData->iDgnCode = KSSC_2014_CSG; }
	    else                      {m_pData->iDgnCode = KSCE_LSD15_CSG;}
	}

	m_pData->bDesignParameter[0] = m_bFlexure          ;
	m_pData->bDesignParameter[1] = m_bShear            ;
	m_pData->bDesignParameter[2] = m_bServiceLimitSate ;
	m_pData->bDesignParameter[3] = m_bConstructibility ;
	m_pData->bDesignParameter[4] = m_bFatigueLimitState;
	m_pData->bDesignParameter[5] = m_bSubMembers       ; 
	m_pData->bDesignParameter[6] = m_bTorWarping;

	m_pData->bAppendix6 = m_bAppendix6;

	m_pData->bDistortion = m_bWarpStress;
	m_pData->bPBTFA4SH = m_bPBTF_Eff;
	m_pData->b1p3RhMy  = m_b1p3RhMy; 

	m_pData->bPrtOptSum = m_bPtrOpt_Sum;
	m_pData->bPrtOptDetail = m_bPtrOpt_Detail;

	m_pData->bCompReinforcement = m_bCompReinforcement;
}

BEGIN_MESSAGE_MAP(CDgnCPGDesignParamLSD12Dlg, CChildDialog)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_DGN_CPG_PARAM_CURVED_RDO, &CDgnCPGDesignParamLSD12Dlg::OnBnClickedDgnCpgParamCurvedRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_PARAM_STRAIGHT_RDO, &CDgnCPGDesignParamLSD12Dlg::OnBnClickedDgnCpgParamStraightRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_PARAM_SINGLE_RDO, OnSelectGirderType)
	ON_BN_CLICKED(IDC_DGN_CPG_PARAM_MULTI_RDO, OnSelectGirderType)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamLSD12Dlg message handlers

void CDgnCPGDesignParamLSD12Dlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();		
	CChildDialog::OnOK();		
}

void CDgnCPGDesignParamLSD12Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}

void CDgnCPGDesignParamLSD12Dlg::OnBnClickedDgnCpgParamCurvedRdo()
{
	GetDlgItem(IDC_DNG_CPG_PARAM_PRT_OPT_GROUP)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CPG_PARAM_PTR_OPT_SUM_CHK)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CPG_PARAM_PTR_OPT_DETAIL_CHK)->EnableWindow(TRUE);

	GetDlgItem(IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CPG_PARAM_PBTF_EFF_CHK)->EnableWindow(TRUE);
	m_pData->iDgnCode = KSSC_2014_CSG;
}


void CDgnCPGDesignParamLSD12Dlg::OnBnClickedDgnCpgParamStraightRdo()
{
	GetDlgItem(IDC_DNG_CPG_PARAM_PRT_OPT_GROUP)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_CPG_PARAM_PTR_OPT_SUM_CHK)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_CPG_PARAM_PTR_OPT_DETAIL_CHK)->EnableWindow(FALSE);

	GetDlgItem(IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_CPG_PARAM_PBTF_EFF_CHK)->EnableWindow(FALSE);

	m_pData->iDgnCode = KSCE_LSD15_CSG;
}


void CDgnCPGDesignParamLSD12Dlg::OnSelectGirderType()
{
	int nCode = m_pParent->GetCurSelDgnCode();
	if(nCode != KDS_24_14_31_2018_CSG)
	{
		return;
	}

	UpdateData(TRUE);

	if (m_GirdernType == 0)
	{
		m_bWarpStress = 1;
		GetDlgItem(IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_PARAM_WARP_CONSIDER_CHK)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}