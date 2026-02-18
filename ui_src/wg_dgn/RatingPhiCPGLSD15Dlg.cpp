// CDgnCPGDesignParamLSD12Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingPhiCPGLSD15Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\DBLib.h"

#include "..\wg_main\wg_main.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCPGDesignParamLSD12Dlg dialog


CRatingPhiCPGLSD15Dlg::CRatingPhiCPGLSD15Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CRatingPhiCPGLSD15Dlg::IDD, pParent)
{
	m_iGirdernType = 1;
	m_bWarpStress = TRUE;
	m_iBridgeType = 0;
	m_bPBTF_Eff = TRUE;
	//{{AFX_DATA_INIT(CRatingPhiCPGLSD15Dlg)
	//}}AFX_DATA_INIT
}

void CRatingPhiCPGLSD15Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRatingPhiCPGLSD15Dlg)
	DDX_Control(pDX, IDC_DGN_RATING_CPG_PARAM_PHI_Y_EDIT,					m_dPhy_y);
	DDX_Control(pDX, IDC_DGN_RATING_CPG_PARAM_PHI_U_EDIT,					m_dPhy_u);
	DDX_Control(pDX, IDC_DGN_RATING_CPG_PARAM_PHI_C_EDIT,					m_dPhy_c);
	DDX_Control(pDX, IDC_DGN_RATING_CPG_PARAM_PHI_F_EDIT,					m_dPhy_f);
	DDX_Control(pDX, IDC_DGN_RATING_CPG_PARAM_PHI_V_EDIT,					m_dPhy_v);
	DDX_Control(pDX, IDC_DGN_RATING_CPG_PARAM_PHI_SE_EDIT,				m_dPhy_se);
	DDX_Control(pDX, IDC_DGN_RATING_CPG_PARAM_PHI_B_EDIT,					m_dPhy_b);

	DDX_Radio(pDX, IDC_DGN_RATING_CPG_PARAM_SINGLE_RDO,						m_iGirdernType);
	DDX_Radio(pDX, IDC_DGN_RATING_CPG_PARAM_STRAIGHT_RDO,					m_iBridgeType);

	DDX_Check(pDX, IDC_DGN_RATING_CPG_PARAM_WARP_CONSIDER_CHK,	m_bWarpStress);
	DDX_Check(pDX, IDC_DGN_RATING_CPG_PARAM_MnLIMIT_KR_CHK,			m_b1p3RhMy);
	DDX_Check(pDX, IDC_DGN_RATING_CPG_PARAM_PBTF_EFF_CHK,				m_bPBTF_Eff);

	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CRatingPhiCPGLSD15Dlg, CInternationalDlg)
#undef CDialog
	ON_BN_CLICKED(IDC_DGN_RATING_CPG_PARAM_CODE_UPDATE_BTN,   OnDgnPhiUpdateBtn)
	ON_BN_CLICKED(IDC_DGN_RATING_CPG_PARAM_CURVED_RDO,   OnBnClickedDgnCpgParamCurvedRdo)
	ON_BN_CLICKED(IDC_DGN_RATING_CPG_PARAM_STRAIGHT_RDO, OnBnClickedDgnCpgParamStraightRdo)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDOK, OnCancel)
END_MESSAGE_MAP()

BOOL CRatingPhiCPGLSD15Dlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	Initial_Data();

	return TRUE;
}

void CRatingPhiCPGLSD15Dlg::Dlg2Data()
{
	m_Data.dPhi_y = m_dPhy_y.GetEditValue();
	m_Data.dPhi_u = m_dPhy_u.GetEditValue();
	m_Data.dPhi_c = m_dPhy_c.GetEditValue();
	m_Data.dPhi_f = m_dPhy_f.GetEditValue();
	m_Data.dPhi_v = m_dPhy_v.GetEditValue();
	m_Data.dPhi_se= m_dPhy_se.GetEditValue();
	m_Data.dPhi_b = m_dPhy_b.GetEditValue();

	m_Data.nGirderType = m_iGirdernType;
	m_Data.nBridgeType = m_iBridgeType; 

	m_Data.bConsiderTorsion = m_bWarpStress;
	m_Data.bStrengthOption[1]	= m_bPBTF_Eff;
	m_Data.bStrengthOption[2]	= m_b1p3RhMy; 

}

void CRatingPhiCPGLSD15Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialogMove::OnCancel();
}

void CRatingPhiCPGLSD15Dlg::OnOk() 
{
	UpdateData(TRUE);

	Dlg2Data();

	if(!m_pDoc->m_pDataCtrl->AddRpas(m_Data))
		return;

	CDialogMove::OnOK();	
}



void CRatingPhiCPGLSD15Dlg::Initial_Data()
{
	//GetDlgItem(IDC_DGN_RATING_CPG_PARAM_CURVED_RDO)->EnableWindow(TRUE); 

	if(m_Data.nBridgeType == 0) // for straight bridge
	{
		GetDlgItem(IDC_DGN_RATING_CPG_PARAM_WARP_CONSIDER_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RATING_CPG_PARAM_PBTF_EFF_CHK)->EnableWindow(FALSE);
	}
	else if(m_Data.nBridgeType == 1) // for Curved bridge
	{
		GetDlgItem(IDC_DGN_RATING_CPG_PARAM_WARP_CONSIDER_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_RATING_CPG_PARAM_PBTF_EFF_CHK)->EnableWindow(TRUE);
	}
	else{ASSERT(0);}

	m_dPhy_y.SetUnitType(D_UNITSYS_NONE);
	m_dPhy_u.SetUnitType(D_UNITSYS_NONE);
	m_dPhy_c.SetUnitType(D_UNITSYS_NONE);
	m_dPhy_f.SetUnitType(D_UNITSYS_NONE);
	m_dPhy_v.SetUnitType(D_UNITSYS_NONE);
	m_dPhy_se.SetUnitType(D_UNITSYS_NONE);
	m_dPhy_b.SetUnitType(D_UNITSYS_NONE);

	T_RPAS_D Data; Data.Initialize();
	Data.Set_STL_Rating_AnnexData_LSD();

	m_dPhy_y.SetEditUnit(m_Data.dPhi_y==0.0?		Data.dPhi_y			: m_Data.dPhi_y);
	m_dPhy_u.SetEditUnit(m_Data.dPhi_u==0.0?		Data.dPhi_u			: m_Data.dPhi_u);
	m_dPhy_c.SetEditUnit(m_Data.dPhi_c==0.0?		Data.dPhi_c			: m_Data.dPhi_c);
	m_dPhy_f.SetEditUnit(m_Data.dPhi_f==0.0?		Data.dPhi_f			: m_Data.dPhi_f);
	m_dPhy_v.SetEditUnit(m_Data.dPhi_v==0.0?		Data.dPhi_v			: m_Data.dPhi_v);
	m_dPhy_se.SetEditUnit(m_Data.dPhi_se==0.0?  Data.dPhi_se		: m_Data.dPhi_se);
	m_dPhy_b.SetEditUnit(m_Data.dPhi_b==0.0?		Data.dPhi_b			: m_Data.dPhi_b);

}

void CRatingPhiCPGLSD15Dlg::OnBnClickedDgnCpgParamCurvedRdo()
{

	GetDlgItem(IDC_DGN_RATING_CPG_PARAM_WARP_CONSIDER_CHK)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_RATING_CPG_PARAM_PBTF_EFF_CHK)->EnableWindow(TRUE);
}


void CRatingPhiCPGLSD15Dlg::OnBnClickedDgnCpgParamStraightRdo()
{

	GetDlgItem(IDC_DGN_RATING_CPG_PARAM_WARP_CONSIDER_CHK)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_RATING_CPG_PARAM_PBTF_EFF_CHK)->EnableWindow(FALSE);
}


void CRatingPhiCPGLSD15Dlg::OnDgnPhiUpdateBtn() 
{
	m_Data.Set_STL_Rating_AnnexData_LSD();
	Initial_Data();
}
