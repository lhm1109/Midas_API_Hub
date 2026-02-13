// DgnSteelRatingOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSteelRatingOptionDlg.h"

#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_main\wg_mainres2.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSteelRatingOptionDlg dialog

const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;
const int OPTION_ADD_REPLACE = 0;
const int OPTION_DELETE = 1;

CDgnSteelRatingOptionDlg::CDgnSteelRatingOptionDlg(CWnd* pParent )
	: CMenuBarChildDlg(CDgnSteelRatingOptionDlg::IDD, pParent)
{
	m_nOption   = OPTION_ADD_REPLACE;
	m_nElemType = ELEM_TYPE_ELEMENT;
	m_nPosition = 0;
	m_nCalcType = 0;

	m_pDoc = CDBDoc::GetDocPoint();

	m_nRatingCode = 0;
	m_bShowDispl  = FALSE;	
}

void CDgnSteelRatingOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_AR,            m_nOption);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	DDX_Radio(pDX, IDC_DGN_I_RADIO,       m_nPosition);
	DDX_Radio(pDX, IDC_DGN_AUTO_RDO,      m_nCalcType);
	DDX_Control(pDX, IDC_DGN_DROOP_EDIT,    m_edtDisplI);
	DDX_Control(pDX, IDC_DGN_DROOP_UNIT,    m_untDisplI);	
	DDX_Control(pDX, IDC_DGN_DROOP_J_EDIT,  m_edtDisplJ);
	DDX_Control(pDX, IDC_DGN_DROOP_J_UNIT,  m_untDisplJ);		
	DDX_Control(pDX, IDC_DGN_STRAIN_EDIT,   m_edtStrainI);
	DDX_Control(pDX, IDC_DGN_STRAIN_J_EDIT, m_edtStrainJ);
	DDX_Control(pDX, IDC_DGN_IMPACT_EDIT_I,   m_edtImpact_I);
	DDX_Control(pDX, IDC_DGN_IMPACT_EDIT_J,   m_edtImpact_J);
	DDX_Control(pDX, IDC_DGN_ADD_REPONCE_FATOR_EDIT_I,   m_edtAddResponceF_I);
	DDX_Control(pDX, IDC_DGN_ADD_REPONCE_FATOR_EDIT_J,   m_edtAddResponceF_J);
	DDX_Control(pDX, IDC_DGN_K_EDT,         m_edtK);
}

void CDgnSteelRatingOptionDlg::InitUnit()
{
	m_edtDisplI.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_edtDisplJ.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_untDisplI.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_untDisplJ.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	
	m_edtStrainI.SetUnitType(D_UNITSYS_NONE);	
	m_edtStrainJ.SetUnitType(D_UNITSYS_NONE);	
	
	m_edtImpact_I.SetUnitType(D_UNITSYS_NONE);	
	m_edtImpact_J.SetUnitType(D_UNITSYS_NONE);	
	m_edtAddResponceF_I.SetUnitType(D_UNITSYS_NONE);	
	m_edtAddResponceF_J.SetUnitType(D_UNITSYS_NONE);	
	m_edtK.SetUnitType(D_UNITSYS_NONE);	
	
	CString szTemp;
	szTemp.Format(_T("%s%s"), _LS(IDS_DGN_CHAR_MU), _LS(IDS_DGN_CHAR_EPSILON));
	GetDlgItem(IDC_DGN_STRAIN_UNIT)->SetWindowText(szTemp);
	GetDlgItem(IDC_DGN_STRAIN_J_UNIT)->SetWindowText(szTemp);
}

void CDgnSteelRatingOptionDlg::InitSetiing()
{
	// Rating Code
	T_RCDS_D RcdsD; RcdsD.Initialize();	
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);
	m_nRatingCode = RcdsD.nRatingCode;
		
	// Strain or Displacement
	T_RPAS_D RpsaD;	RpsaD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetRpas(RpsaD))
	{
		RpsaD.Initialize();
	}		
	
	if( m_nRatingCode==STL_AASHTO_LRFD12 || m_nRatingCode==STL_AASHTO_LRFD19 )
	{
		if(RpsaD.nMeasurement == 0) m_bShowDispl = FALSE; // Strain
		else                        m_bShowDispl = TRUE;  // Displacement
	}
	else
	{
		if(RpsaD.nSurveyMeth == 0) m_bShowDispl = TRUE;  // 처짐 
		else                       m_bShowDispl = FALSE; // 변형률
	}
	
	// Controls ID
	m_aAutoUser.RemoveAll();
	m_aAutoUser.Add(IDC_DGN_AUTO_RDO);
	m_aAutoUser.Add(IDC_DGN_USER_RDO);
	
	m_aStrain_I.RemoveAll();
	m_aStrain_I.Add(IDC_DGN_STRAIN_I_STATIC);
	m_aStrain_I.Add(IDC_DGN_STRAIN_EDIT);
	m_aStrain_I.Add(IDC_DGN_STRAIN_UNIT);
	
	m_aStrain_J.RemoveAll();
	m_aStrain_J.Add(IDC_DGN_STRAIN_J_STATIC);
	m_aStrain_J.Add(IDC_DGN_STRAIN_J_EDIT);
	m_aStrain_J.Add(IDC_DGN_STRAIN_J_UNIT);
	
	m_aDispl_I.RemoveAll();
	m_aDispl_I.Add(IDC_DGN_DROOP_I_STATIC);
	m_aDispl_I.Add(IDC_DGN_DROOP_EDIT);
	m_aDispl_I.Add(IDC_DGN_DROOP_UNIT);
	
	m_aDispl_J.RemoveAll();
	m_aDispl_J.Add(IDC_DGN_DROOP_J_STATIC);
	m_aDispl_J.Add(IDC_DGN_DROOP_J_EDIT);
	m_aDispl_J.Add(IDC_DGN_DROOP_J_UNIT);
	
	m_aImpact_I.RemoveAll();
	m_aImpact_I.Add(IDC_DGN_FACTOR_STATIC_I);
	m_aImpact_I.Add(IDC_DGN_IMPACT_EDIT_I);

	m_aImpact_J.RemoveAll();
	m_aImpact_J.Add(IDC_DGN_FACTOR_STATIC_J);
	m_aImpact_J.Add(IDC_DGN_IMPACT_EDIT_J);

	m_aAddReponce_I.RemoveAll();
	m_aAddReponce_I.Add(IDC_DGN_ADD_RESPONCE_FACTOR_I);
	m_aAddReponce_I.Add(IDC_DGN_ADD_REPONCE_FATOR_EDIT_I);

	m_aAddReponce_J.RemoveAll();
	m_aAddReponce_J.Add(IDC_DGN_ADD_RESPONCE_FACTOR_J);
	m_aAddReponce_J.Add(IDC_DGN_ADD_REPONCE_FATOR_EDIT_J);
	
	m_aK.RemoveAll();
	m_aK.Add(IDC_DGN_K_STC);
	m_aK.Add(IDC_DGN_K_EDT);
	
	m_aEtc.RemoveAll();
	m_aEtc.Add(IDC_DGN_EXECUTE);
	m_aEtc.Add(IDC_DGN_CLOSE);

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_DEFINE_GROUP);
	m_aPositionCtrl.Add(IDC_POSITION_GROUP);
	m_aPositionCtrl.Add(IDC_DGN_I_RADIO);
	m_aPositionCtrl.Add(IDC_DGN_J_RADIO);
	m_aPositionCtrl.Add(IDC_DGN_IJ_RADIO);
	m_aPositionCtrl.Add(IDC_FACTOR_GROUP);
	m_aPositionCtrl.Add(IDC_DGN_DROOP_I_STATIC);
	m_aPositionCtrl.Add(IDC_DGN_DROOP_EDIT);
	m_aPositionCtrl.Add(IDC_DGN_DROOP_UNIT);
	m_aPositionCtrl.Add(IDC_DGN_DROOP_J_STATIC);
	m_aPositionCtrl.Add(IDC_DGN_DROOP_J_EDIT);
	m_aPositionCtrl.Add(IDC_DGN_DROOP_J_UNIT);
	m_aPositionCtrl.Add(IDC_DGN_FACTOR_STATIC_I);
	m_aPositionCtrl.Add(IDC_DGN_IMPACT_EDIT_I);
	m_aPositionCtrl.Add(IDC_DGN_FACTOR_STATIC_J);
	m_aPositionCtrl.Add(IDC_DGN_IMPACT_EDIT_J);
	m_aPositionCtrl.Add(IDC_DGN_ADD_RESPONCE_FACTOR_I);
	m_aPositionCtrl.Add(IDC_DGN_ADD_REPONCE_FATOR_EDIT_I);
	m_aPositionCtrl.Add(IDC_DGN_ADD_RESPONCE_FACTOR_J);
	m_aPositionCtrl.Add(IDC_DGN_ADD_REPONCE_FATOR_EDIT_J);
	m_aPositionCtrl.Add(IDC_DGN_AUTO_RDO);
	m_aPositionCtrl.Add(IDC_DGN_USER_RDO);
	m_aPositionCtrl.Add(IDC_DGN_K_STC);
	m_aPositionCtrl.Add(IDC_DGN_K_EDT);
	m_aPositionCtrl.Add(IDC_DGN_STRAIN_I_STATIC);
	m_aPositionCtrl.Add(IDC_DGN_STRAIN_EDIT);
	m_aPositionCtrl.Add(IDC_DGN_STRAIN_UNIT);
	m_aPositionCtrl.Add(IDC_DGN_STRAIN_J_STATIC);
	m_aPositionCtrl.Add(IDC_DGN_STRAIN_J_EDIT);
	m_aPositionCtrl.Add(IDC_DGN_STRAIN_J_UNIT);
	m_aPositionCtrl.Add(IDC_DGN_EXECUTE);
	m_aPositionCtrl.Add(IDC_DGN_CLOSE);
}

void CDgnSteelRatingOptionDlg::InitText()
{
	if( m_nRatingCode==STL_AASHTO_LRFD12 || m_nRatingCode==STL_AASHTO_LRFD19 )
	{
		GetDlgItem(IDC_FACTOR_GROUP       )->SetWindowText(_LS(IDS_DGN_STL_RATING_OPTION_AASHTO_FACTOR_GROUP));
		GetDlgItem(IDC_DGN_DROOP_I_STATIC )->SetWindowText(_LS(IDS_DGN_STL_RATING_OPTION_AASHTO_DISPL_I));
		GetDlgItem(IDC_DGN_DROOP_J_STATIC )->SetWindowText(_LS(IDS_DGN_STL_RATING_OPTION_AASHTO_DISPL_J));
		GetDlgItem(IDC_DGN_STRAIN_I_STATIC)->SetWindowText(_LS(IDS_DGN_STL_RATING_OPTION_AASHTO_STRAIN_I));
		GetDlgItem(IDC_DGN_STRAIN_J_STATIC)->SetWindowText(_LS(IDS_DGN_STL_RATING_OPTION_AASHTO_STRAIN_J));
		GetDlgItem(IDC_DGN_FACTOR_STATIC_I )->SetWindowText(_LS(IDS_DGN_STL_RATING_OPTION_AASHTO_IMPACT_FACTOR_I));
		GetDlgItem(IDC_DGN_FACTOR_STATIC_J )->SetWindowText(_LS(IDS_DGN_STL_RATING_OPTION_AASHTO_IMPACT_FACTOR_J));
		GetDlgItem(IDC_DGN_ADD_RESPONCE_FACTOR_I)->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_I));
		GetDlgItem(IDC_DGN_ADD_RESPONCE_FACTOR_J)->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_J));
	}
	else
	{
		GetDlgItem(IDC_FACTOR_GROUP       )->SetWindowText(_LS(IDS_DGN_STL_RATING_OPTION_FACTOR_GROUP));
		GetDlgItem(IDC_DGN_DROOP_I_STATIC )->SetWindowText(_LS(IDS_DGN_STL_RATING_OPTION_DISPL_I));
		GetDlgItem(IDC_DGN_DROOP_J_STATIC )->SetWindowText(_LS(IDS_DGN_STL_RATING_OPTION_DISPL_J));
		GetDlgItem(IDC_DGN_STRAIN_I_STATIC)->SetWindowText(_LS(IDS_DGN_STL_RATING_OPTION_STRAIN_I));
		GetDlgItem(IDC_DGN_STRAIN_J_STATIC)->SetWindowText(_LS(IDS_DGN_STL_RATING_OPTION_STRAIN_J));
		GetDlgItem(IDC_DGN_FACTOR_STATIC_I )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_IMPACT_FACTOR_I));
		GetDlgItem(IDC_DGN_FACTOR_STATIC_J )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_IMPACT_FACTOR_J));
		GetDlgItem(IDC_DGN_ADD_RESPONCE_FACTOR_I)->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_I));
		GetDlgItem(IDC_DGN_ADD_RESPONCE_FACTOR_J)->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_J));
	}
}

void CDgnSteelRatingOptionDlg::InitDefaultValue()
{
	T_RROS_D RrosD; RrosD.Initialize();
	
	const double dDefVal = 0.001;

	m_edtDisplI .SetEditUnit(m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dDefVal));
	m_edtDisplJ .SetEditUnit(m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dDefVal));
	m_edtStrainI.SetEditUnit(RrosD.dStrainI);
	m_edtStrainJ.SetEditUnit(RrosD.dStrainJ);
	m_edtImpact_I .SetEditUnit(RrosD.dImpactFactor);
	m_edtImpact_J .SetEditUnit(RrosD.dImpactFactor_J);
	m_edtAddResponceF_I .SetEditUnit(RrosD.dAddReponceF_I);
	m_edtAddResponceF_J .SetEditUnit(RrosD.dAddReponceF_J);
	m_edtK      .SetEditUnit(RrosD.dAdjustmentFactor);
}

#define DLG_TOP_MARGIN 20
#define DLG_GROUP_MARGIN 28
void CDgnSteelRatingOptionDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY = 0;

	GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DEFINE_GROUP)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;
	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || m_nRatingCode != STL_AASHTO_LRFD19)
	{
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
	}
	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);

	if( m_nRatingCode==STL_AASHTO_LRFD12 || m_nRatingCode==STL_AASHTO_LRFD19 )
	{
		// Auto, User
		GetDlgItem(IDC_FACTOR_GROUP)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_AUTO_RDO)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top + DLG_TOP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aAutoUser, nDistY);

		// Measured Displ
		GetDlgItem(IDC_DGN_USER_RDO  )->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_DROOP_EDIT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + DLG_GROUP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aDispl_I, nDistY);
		
		GetDlgItem(IDC_DGN_DROOP_EDIT  )->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_DROOP_J_EDIT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + DLG_GROUP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aDispl_J, nDistY);
		
		// Measured Strain 
		GetDlgItem(IDC_DGN_DROOP_EDIT )->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_STRAIN_EDIT)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aStrain_I, nDistY);
		
		GetDlgItem(IDC_DGN_DROOP_J_EDIT )->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_STRAIN_J_EDIT)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aStrain_J, nDistY);
		
		// Measured Dynamic Factor - I
		GetDlgItem(IDC_DGN_DROOP_J_EDIT)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_IMPACT_EDIT_I )->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + DLG_GROUP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aImpact_I, nDistY);	

		// Measured Dynamic Factor - J
		GetDlgItem(IDC_DGN_IMPACT_EDIT_I)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_IMPACT_EDIT_J )->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + DLG_GROUP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aImpact_J, nDistY);	

		// Additional Responce Factor - I
		GetDlgItem(IDC_DGN_IMPACT_EDIT_J)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_ADD_REPONCE_FATOR_EDIT_I)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + DLG_GROUP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aAddReponce_I, nDistY);	

		// Additional Responce Factor - J
		GetDlgItem(IDC_DGN_ADD_REPONCE_FATOR_EDIT_I)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_ADD_REPONCE_FATOR_EDIT_J)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + DLG_GROUP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aAddReponce_J, nDistY);	

		// Adjustment Factor, K
		GetDlgItem(IDC_DGN_DROOP_EDIT)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_K_EDT     )->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aK, nDistY);
	}
	else
	{
		// 실측 처짐량
		GetDlgItem(IDC_FACTOR_GROUP  )->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_DROOP_EDIT)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top + DLG_TOP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aDispl_I, nDistY);

		GetDlgItem(IDC_DGN_DROOP_EDIT  )->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_DROOP_J_EDIT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + DLG_GROUP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aDispl_J, nDistY);
		
		// 실측 변형률 
		GetDlgItem(IDC_DGN_DROOP_EDIT )->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_STRAIN_EDIT)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aStrain_I, nDistY);
		
		GetDlgItem(IDC_DGN_DROOP_J_EDIT )->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_STRAIN_J_EDIT)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aStrain_J, nDistY);

		// 실측 충격 계수
		GetDlgItem(IDC_DGN_DROOP_J_EDIT)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_IMPACT_EDIT_I )->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + DLG_GROUP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aImpact_I, nDistY);	

		// Measured Dynamic Factor - J
		GetDlgItem(IDC_DGN_IMPACT_EDIT_I)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_IMPACT_EDIT_J )->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + DLG_GROUP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aImpact_J, nDistY);	

		// Additional Responce Factor - I
		GetDlgItem(IDC_DGN_IMPACT_EDIT_J)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_ADD_REPONCE_FATOR_EDIT_I)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + DLG_GROUP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aAddReponce_I, nDistY);	

		// Additional Responce Factor - J
		GetDlgItem(IDC_DGN_ADD_REPONCE_FATOR_EDIT_I)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_ADD_REPONCE_FATOR_EDIT_J )->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + DLG_GROUP_MARGIN * globalUtils.GetDPIScale();
		CDlgUtil::CtrlMoveDistY(this, m_aAddReponce_J, nDistY);	
	}

	// 실측 보정계수 Group Box
	GetDlgItem(IDC_FACTOR_GROUP)->GetWindowRect(rToMove);
	GetDlgItem(IDC_DGN_ADD_REPONCE_FATOR_EDIT_J)->GetWindowRect(rRef);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	
	rToMove.bottom = rToMove.top + nDistY;
	
	ScreenToClient(rToMove);
	GetDlgItem(IDC_FACTOR_GROUP)->MoveWindow(rToMove);
	
	// Define Rating Group Box
	GetDlgItem(IDC_DEFINE_GROUP)->GetWindowRect(rToMove);
	GetDlgItem(IDC_FACTOR_GROUP)->GetWindowRect(rRef);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	
	rToMove.bottom = rToMove.top + nDistY;
	
	ScreenToClient(rToMove);
	GetDlgItem(IDC_DEFINE_GROUP)->MoveWindow(rToMove);

	// Apply, Close
	GetDlgItem(IDC_DEFINE_GROUP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_EXECUTE )->GetWindowRect(rToMove);
	
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);	

	RedrawWindow();
}

void CDgnSteelRatingOptionDlg::EnableDisableControls()
{
	CDlgUtil::CtrlEnableDisable(this, m_aPositionCtrl, (m_nOption == OPTION_ADD_REPLACE));
	if(m_nOption == OPTION_ADD_REPLACE)
	{
		GetDlgItem(IDC_DGN_DROOP_I_STATIC)->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_DROOP_EDIT    )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_DROOP_UNIT    )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_DROOP_J_STATIC)->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_DROOP_J_EDIT  )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_DROOP_J_UNIT  )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		
		GetDlgItem(IDC_DGN_STRAIN_I_STATIC)->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_STRAIN_EDIT    )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_STRAIN_UNIT    )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_STRAIN_J_STATIC)->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_STRAIN_J_EDIT  )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_STRAIN_J_UNIT  )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);

		GetDlgItem(IDC_DGN_FACTOR_STATIC_I)->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_IMPACT_EDIT_I  )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_FACTOR_STATIC_J)->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_IMPACT_EDIT_J  )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);

		GetDlgItem(IDC_DGN_ADD_RESPONCE_FACTOR_I     )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_ADD_REPONCE_FATOR_EDIT_I  )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_ADD_RESPONCE_FACTOR_J     )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_ADD_REPONCE_FATOR_EDIT_J  )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
	}

	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CLOSE)->EnableWindow(TRUE);
}

void CDgnSteelRatingOptionDlg::ControlsShowHide()
{
	if( m_nRatingCode==STL_AASHTO_LRFD12 || m_nRatingCode==STL_AASHTO_LRFD19 )
	{
		CDlgUtil::CtrlShowHide(this, m_aAutoUser, TRUE);
		
		CDlgUtil::CtrlShowHide(this, m_aDispl_I,  m_nCalcType==0 &&  m_bShowDispl);
		CDlgUtil::CtrlShowHide(this, m_aDispl_J,  m_nCalcType==0 &&  m_bShowDispl);
		CDlgUtil::CtrlShowHide(this, m_aStrain_I, m_nCalcType==0 && !m_bShowDispl);
		CDlgUtil::CtrlShowHide(this, m_aStrain_J, m_nCalcType==0 && !m_bShowDispl);
		
		CDlgUtil::CtrlShowHide(this, m_aImpact_I,   m_nCalcType==0);
		CDlgUtil::CtrlShowHide(this, m_aImpact_J,       FALSE);
		CDlgUtil::CtrlShowHide(this, m_aAddReponce_I,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aAddReponce_J,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aK,        m_nCalcType==1);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aAutoUser,       FALSE);
		CDlgUtil::CtrlShowHide(this, m_aImpact_I,       TRUE);	
		CDlgUtil::CtrlShowHide(this, m_aImpact_J,       TRUE);	
		CDlgUtil::CtrlShowHide(this, m_aAddReponce_I,   TRUE);
		CDlgUtil::CtrlShowHide(this, m_aAddReponce_J,   TRUE);
		CDlgUtil::CtrlShowHide(this, m_aK,              FALSE);	
		
		CDlgUtil::CtrlShowHide(this, m_aDispl_I,   m_bShowDispl);
		CDlgUtil::CtrlShowHide(this, m_aDispl_J,   m_bShowDispl);
		CDlgUtil::CtrlShowHide(this, m_aStrain_I, !m_bShowDispl);
		CDlgUtil::CtrlShowHide(this, m_aStrain_J, !m_bShowDispl);
	}

	if( m_nRatingCode==STL_AASHTO_LRFD12 || m_nRatingCode==STL_AASHTO_LRFD19 )
	{
		GetDlgItem(IDC_DGN_STRAIN_UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STRAIN_J_UNIT)->ShowWindow(SW_HIDE);
	}
	else
	{
		if(m_bShowDispl)
		{
			GetDlgItem(IDC_DGN_STRAIN_UNIT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_STRAIN_J_UNIT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_DROOP_UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_DROOP_J_UNIT)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_DGN_DROOP_UNIT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_DROOP_J_UNIT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_STRAIN_UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STRAIN_J_UNIT)->ShowWindow(SW_SHOW);
		}

	}

	// Element Type
	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || m_nRatingCode != STL_AASHTO_LRFD19)
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}
}


BEGIN_MESSAGE_MAP(CDgnSteelRatingOptionDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_AR,       OnDgnArDelRdo)
	ON_BN_CLICKED(IDC_DGN_DEL,      OnDgnArDelRdo)	
	ON_BN_CLICKED(IDC_DGN_I_RADIO,	OnDgnPositionRdo)	
	ON_BN_CLICKED(IDC_DGN_J_RADIO,	OnDgnPositionRdo)	
	ON_BN_CLICKED(IDC_DGN_IJ_RADIO, OnDgnPositionRdo)	
	ON_BN_CLICKED(IDC_DGN_AUTO_RDO, OnDgnCalcTypeRdo)	
	ON_BN_CLICKED(IDC_DGN_USER_RDO, OnDgnCalcTypeRdo)	
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSteelRatingOptionDlg message handlers

BOOL CDgnSteelRatingOptionDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	InitUnit();
	InitSetiing();
	InitText();
	InitDefaultValue();
	AlignControl();
	EnableDisableControls();
	ControlsShowHide();	
	
	UpdateData(FALSE);

	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
		
	return TRUE;
}

void CDgnSteelRatingOptionDlg::Execute()
{
	auto SetCommonData = [this](auto& rData) {
		UpdateData(TRUE);
		rData.nPosition = m_nPosition;
		rData.nCalcType = m_nCalcType;
		rData.dDefI = m_edtDisplI.GetEditValue();
		rData.dDefJ = m_edtDisplJ.GetEditValue();
		rData.dStrainI = m_edtStrainI.GetEditValue();
		rData.dStrainJ = m_edtStrainJ.GetEditValue();
		rData.dImpactFactor = m_edtImpact_I.GetEditValue();
		rData.dImpactFactor_J = m_edtImpact_J.GetEditValue();
		rData.dAddReponceF_I = m_edtAddResponceF_I.GetEditValue();
		rData.dAddReponceF_J = m_edtAddResponceF_J.GetEditValue();
		rData.dAdjustmentFactor = m_edtK.GetEditValue();
	};

	auto ExcuteAddReplace = [this, &SetCommonData](auto& selKeys, bool isElement) {
		if (isElement)
		{
			T_RROS_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddRros(selKeys, rData)) Initial_SelectItem();
		}
		else
		{
			T_RROV_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddRrov(selKeys, rData)) Initial_SelectItem();
		}
		};

	auto ExcuteDelete = [this](auto& selKeys, bool isElement) {
		bool success = isElement ?
			m_pDoc->m_pDataCtrl->DelRros(selKeys) :
			m_pDoc->m_pDataCtrl->DelRrov(selKeys);
		if (success) Initial_SelectItem();
	};

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	if (m_nElemType == ELEM_TYPE_ELEMENT)
	{
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) {
			AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
			return;
		}
		if (m_nOption == OPTION_ADD_REPLACE)
			ExcuteAddReplace(aSelKey, true);
		else
			ExcuteDelete(aSelKey, true);
	}
	else if (m_nElemType == ELEM_TYPE_VBEAM)
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) {
			AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
			return;
		}

		if (m_nOption == OPTION_ADD_REPLACE)
			ExcuteAddReplace(aSelKey, false);
		else
			ExcuteDelete(aSelKey, false);
	}
}

void CDgnSteelRatingOptionDlg::Initial_SelectItem()
{
	// Unselected All.
	if (m_pDoc == NULL)	return;
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}
void CDgnSteelRatingOptionDlg::OnDgnArDelRdo() 
{
	UpdateData(TRUE);
	EnableDisableControls();
}

void CDgnSteelRatingOptionDlg::OnDgnPositionRdo() 
{
	UpdateData(TRUE);
	EnableDisableControls();
}

void CDgnSteelRatingOptionDlg::OnDgnCalcTypeRdo() 
{
	UpdateData(TRUE);
	EnableDisableControls();
	ControlsShowHide();
}

void CDgnSteelRatingOptionDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == ELEM_TYPE_ELEMENT ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);

	EnableDisableControls();
}


void CDgnSteelRatingOptionDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			{
				UpdateBuffer();
				break;
			}
		case D_UPDATE_UNIT:
			// do something...
			break;
		default:
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

void CDgnSteelRatingOptionDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	
	BOOL bSpfcChanged = FALSE;
	BOOL bStorChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		switch(nCmd)
		{
		case(UR_RCDS_ADD):
		case(UR_RCDS_DEL): // Code 변경시
		case(UR_RPAS_ADD):
		case(UR_RPAS_DEL): // Option변경시
			InitSetiing();
			InitText();
			AlignControl();
			EnableDisableControls();
			ControlsShowHide();	
			break;		
		default:
		  break;
		}
	}
}
