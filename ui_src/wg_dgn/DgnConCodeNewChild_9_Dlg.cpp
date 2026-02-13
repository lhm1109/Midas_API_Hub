// DgnConCodeNewChild_9_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeNewChild_9_Dlg.h"

#include "DgnConCodeEC8InfoDlg.h"
#include "DgnConWallEnvelopMethod.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "DgnDataCtrl.h"
#include "NationalAnnexTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild9Dlg dialog


CDgnConCodeNewChild9Dlg::CDgnConCodeNewChild9Dlg(CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/)
	: CDconChildDialog(CDgnConCodeNewChild9Dlg::IDD, pParent, pData, bExistData)
{
	//{{AFX_DATA_INIT(CDgnConCodeNewChild9Dlg)
// 	m_pData = pData;
	
	m_dUf = 0.6;
	m_dMrft = 0.0;
	m_dGamma_rdForBeam = 0.0;
	m_dGamma_rdForColm = 0.0;
	m_dGamma_rdForWall = 0.0;
	m_dGamma_rdForJoint = 0.0;
	m_dStorColmWeakBeamFactor = 1.3;

	m_bSingaporeAndMore = FALSE;  // 반대로 설정하여 첫 호출 시 항상 실행
	m_bGermanyOrSpain = TRUE;     // 반대로 설정하여 첫 호출 시 항상 실행
	m_bLimitShearMoved = FALSE;   // 초기엔 Limit shear 이동되지 않음
	m_bApplyEurocodeEdited = FALSE;
	m_bTorsionDesign = FALSE;
	m_bStrongColmWeakBeam = FALSE;
	m_bShearWallAlphaMax = TRUE;
	m_bVedPrimaryMemb = FALSE;

	m_GroupKey = 0;
	m_DconEC8Data.Initialize();
	m_nPMCurveMethod = 1;
	m_nCheckPos = EN_SELECT_CHECK_POS_BOT;
	m_nFsMethod = EN_FSMETHOD_2_3_FY;
	m_nNDDesignMethod = ND_MC;
	m_dApproximateAlpha = 0.9;

	m_bShowPMCurveMethod = TRUE;

	m_aSlendernessCtrls.RemoveAll();
	m_aSlendernessCtrls.Add(IDC_DGN_SLENDERNESS_LIMIT_STC);
	m_aSlendernessCtrls.Add(IDC_DGN_LAMBDA_LIMIT_STC);
	m_aSlendernessCtrls.Add(IDC_DGN_SLENDERNESS_A_STC);
	m_aSlendernessCtrls.Add(IDC_DGN_SLENDERNESS_A_EDT);
	m_aSlendernessCtrls.Add(IDC_DGN_SLENDERNESS_AUTO_A_CHK);
	m_aSlendernessCtrls.Add(IDC_DGN_SLENDERNESS_B_STC);
	m_aSlendernessCtrls.Add(IDC_DGN_SLENDERNESS_B_EDT);
	m_aSlendernessCtrls.Add(IDC_DGN_SLENDERNESS_C_STC);
	m_aSlendernessCtrls.Add(IDC_DGN_SLENDERNESS_C_EDT);
	m_aSlendernessCtrls.Add(IDC_DGN_LAMBDA_LIMIT_NTC2012_STC);
	m_aSlendernessCtrls.Add(IDC_DGN_LAMBDA_LIMIT_NTC2012_PROP_STC);
	m_aSlendernessCtrls.Add(IDC_DGN_SLENDERNESS_AUTO_C_CHK);

	//PMS:4562 (Tel. 4022) MNET:XXXX-BSCHOI-20121108 #1
	m_aSlendernessCtrlsNTC2012.RemoveAll();
	m_aSlendernessCtrlsNTC2012.Add(IDC_DGN_SLENDERNESS_LIMIT_STC);
	m_aSlendernessCtrlsNTC2012.Add(IDC_DGN_LAMBDA_LIMIT_NTC2012_STC);
	m_aSlendernessCtrlsNTC2012.Add(IDC_DGN_LAMBDA_LIMIT_NTC2012_PROP_STC);
	m_aSlendernessCtrlsNTC2012.FreeExtra();

	m_aBCJointGroup.RemoveAll();
	m_aBCJointGroup.Add(IDC_DGN_EUROCODE204_N17_GRB);
	m_aBCJointGroup.Add(IDC_DGN_EUROCODE204_N17);
	m_aBCJointGroup.Add(IDC_DGN_EUROCODE204_N11);
	m_aBCJointGroup.Add(IDC_DGN_EUROCODE204_N22);
	m_aBCJointGroup.Add(IDC_DGN_EUROCODE204_CONFINED_CHK);
	m_aBCJointGroup.Add(IDC_DGN_EUROCODE204_NOT_CONFINED_CHK);
	m_aBCJointGroup.FreeExtra();

	m_aSelectPosGroup.RemoveAll();
	m_aSelectPosGroup.Add(IDC_DGN_EUROCODE204_N19_GRB);
	m_aSelectPosGroup.Add(IDC_DGN_EUROCODE204_TOP_RDO);
	m_aSelectPosGroup.Add(IDC_DGN_EUROCODE204_BOT_RDO);
//   m_aStrongColmWeakBeam.RemoveAll();
//   m_aStrongColmWeakBeam.Add(IDC_DGN_COLM_WEAK_BEAM_CHK);

	m_aStrongColm.RemoveAll();
	m_aStrongColm.Add(IDC_DGN_EUROCODE204_N0);
	m_aStrongColm.Add(IDC_DGN_EUROCODE204_N1);
	m_aStrongColm.Add(IDC_DGN_EUROCODE204_N2);
	m_aStrongColm.Add(IDC_DGN_EUROCODE204_N3);
	m_aStrongColm.Add(IDC_DGN_COLM_WEAK_BEAM_CHK);

	m_aSelectDuctility.RemoveAll();
	m_aSelectDuctility.Add(IDC_DGN_EUROCODE204_N4);
	m_aSelectDuctility.Add(IDC_DGN_EUROCODE204_N5);
	m_aSelectDuctility.Add(IDC_DGN_EUROCODE204_N6);

	m_aDuctilityRadio.RemoveAll();
	m_aDuctilityRadio.Add(IDC_DGN_EUROCODE204_N5);
	m_aDuctilityRadio.Add(IDC_DGN_EUROCODE204_N6);
	m_aDuctilityRadio.Add(IDC_DGN_EUROCODE204_ND_DUCT_RDO);

	m_aShearForce.RemoveAll();
	m_aShearForce.Add(IDC_DGN_EUROCODE204_N7);
	m_aShearForce.Add(IDC_DGN_SHEAR_GAMMA_GRB);
	m_aShearForce.Add(IDC_DGN_EUROCODE204_N9);
	m_aShearForce.Add(IDC_DGN_EUROCODE204_N10);
	m_aShearForce.Add(IDC_DGN_EUROCODE204_N12);
	m_aShearForce.Add(IDC_DGN_EUROCODE204_N13);
	m_aShearForce.Add(IDC_DGN_EUROCODE204_N8);
	m_aShearForce.Add(IDC_DGN_EUROCODE204_N21);
	m_aShearForce.Add(IDC_DGN_SHEAR_ALPHA_CHK);
	m_aShearForce.Add(IDC_DGN_SHEAR_VED_CHK);

	m_aNonDissipative.RemoveAll();
	m_aNonDissipative.Add(IDC_DGN_EUROCODE204_ND_STC);
	m_aNonDissipative.Add(IDC_DGN_EUROCODE204_ND_CMB);
	m_aNonDissipative.Add(IDC_DGN_EUROCODE204_ND_BTN);

	m_aDesignMethodND.RemoveAll();
	m_aDesignMethodND.Add(IDC_DGN_EUROCODE204_ND_METHOD_STC);
	m_aDesignMethodND.Add(IDC_DGN_EUROCODE204_ND_METHOD1_RDO);
	m_aDesignMethodND.Add(IDC_DGN_EUROCODE204_ND_METHOD2_RDO);
	m_aDesignMethodND.Add(IDC_DGN_EUROCODE204_ND_METHOD2_ETC);
	m_aDesignMethodND.Add(IDC_DGN_EUROCODE204_ND_METHOD2_STC);

	m_aSecondarySeis.RemoveAll();
	m_aSecondarySeis.Add(IDC_DGN_EUROCODE204_N14);
	m_aSecondarySeis.Add(IDC_DGN_EUROCODE204_N15);
	m_aSecondarySeis.Add(IDC_DGN_EUROCODE204_N16);
	
	m_aEnvelopMethod.RemoveAll();
	m_aEnvelopMethod.Add(IDC_DGN_EUROCODE204_N24);
	m_aEnvelopMethod.Add(IDC_DGN_EUROCODE204_EM_BTN);
	
	m_aFrictionCoeff.RemoveAll();
	m_aFrictionCoeff.Add(IDC_DGN_EUROCODE204_N19);
	m_aFrictionCoeff.Add(IDC_DGN_EUROCODE204_N20);

	m_aTorsionDesign.RemoveAll();
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TORSION);
	
	m_aMomentRedistr.RemoveAll();
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_STATIC);
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_FACTOR);

	m_aConsiderShear.RemoveAll();
	m_aConsiderShear.Add(IDC_DGN_EUROCODE204_USEVC);
	m_aConsiderShear.Add(IDC_EUROCODE204_USEVC_WALL);
	m_aConsiderShear.Add(IDC_EUROCODE204_USEVC_COLM);
	m_aConsiderShear.Add(IDC_EUROCODE204_USEVC_BEAM);
	
	m_aLimitShearStr.RemoveAll();
	m_aLimitShearStr.Add(IDC_DGN_DGN_EUROCODE204_LIMIT_STRG_CHK);

	m_aPMCurveMethod.RemoveAll();
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD);
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD_P_RADIO);
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD_MP_RADIO);		
}

void CDgnConCodeNewChild9Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDconChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeNewChild9Dlg)
	DDX_Check(pDX, IDC_DGN_EUROCODE204_N18, m_bNTCFlag);
	DDX_Check(pDX, IDC_DGN_RCCTRL_SPECIAL,  m_bSpecialEQ);
	DDX_Check(pDX, IDC_DGN_RCCTRL_TORSION,  m_bTorsionDesign);
	DDX_Check(pDX, IDC_DGN_EUROCODE204_N17, m_bBCJointDgn);
	DDX_Check(pDX, IDC_DGN_COLM_WEAK_BEAM_CHK,  m_bStrongColmWeakBeam);
	DDX_Check(pDX, IDC_DGN_SHEAR_ALPHA_CHK,	m_bShearWallAlphaMax);
	DDX_Check(pDX, IDC_DGN_SHEAR_VED_CHK,	m_bVedPrimaryMemb);

	DDX_Radio(pDX, IDC_DGN_EUROCODE204_N5, m_iDuctility);
	DDX_Radio(pDX, IDC_DGN_EUROCODE204_ND_METHOD1_RDO, m_nNDDesignMethod);

	DDX_Text(pDX, IDC_DGN_EUROCODE204_N20, m_dUf);
	DDX_Text(pDX, IDC_DGN_RCCTRL_MRFT_FACTOR, m_dMrft);
	DDX_Text(pDX, IDC_DGN_EUROCODE204_N10, m_dGamma_rdForBeam);
	DDX_Text(pDX, IDC_DGN_EUROCODE204_N13, m_dGamma_rdForColm);
	DDX_Text(pDX, IDC_DGN_EUROCODE204_N21, m_dGamma_rdForWall);
	DDX_Text(pDX, IDC_DGN_EUROCODE204_N22, m_dGamma_rdForJoint);
	DDX_Text(pDX, IDC_DGN_EUROCODE204_N2, m_dStorColmWeakBeamFactor);
	DDX_Text(pDX, IDC_DGN_EUROCODE204_ND_METHOD2_ETC, m_dApproximateAlpha);

	DDX_Control(pDX, IDC_EUROCODE204_USEVC_WALL, m_ChkUseVcWall);// PMS:DNKIM-20121030
	DDX_Control(pDX, IDC_EUROCODE204_USEVC_COLM, m_ChkUseVcColm);
	DDX_Control(pDX, IDC_EUROCODE204_USEVC_BEAM, m_ChkUseVcBeam);

	DDX_Control(pDX, IDC_DGN_STRUT_ANGLE_EDT, m_edtStrutAngle);
	DDX_Control(pDX, IDC_DGN_PHIEF_EDT, m_edtEffectivePhi);
	DDX_Control(pDX, IDC_DGN_SLENDERNESS_A_EDT, m_edtSlendLimitA);
	DDX_Control(pDX, IDC_DGN_SLENDERNESS_B_EDT, m_edtSlendLimitB);
	DDX_Control(pDX, IDC_DGN_SLENDERNESS_C_EDT, m_edtSlendLimitC);
	DDX_Control(pDX, IDC_DGN_SLENDERNESS_AUTO_A_CHK, m_chkAutoCalcA);
	DDX_Control(pDX, IDC_DGN_SLENDERNESS_AUTO_C_CHK, m_chkAutoCalcC);

	DDX_Control(pDX, IDC_DGN_EUROCODE204_ND_CMB, m_cmbNDGroup);
	DDX_Control(pDX, IDC_DGN_EUROCODE204_N15, m_cmbGroup);
	DDX_Control(pDX, IDC_DGN_EUROCODE204_N17, m_chkApplyBCJDesign);
	DDX_Control(pDX, IDC_DGN_EUROCODE204_N18, m_chkApplyNTC);
	DDX_Control(pDX, IDC_DGN_EUROCODE204_N99, m_cmbNTCType);
	DDX_Control(pDX, IDC_DGN_NATIONAL_ANNEX_COMBO, m_ComboNationAnnex);
	DDX_Control(pDX, IDC_DGN_DGN_EUROCODE204_LIMIT_STRG_CHK, m_ChkLimitShearStrgConc);
	DDX_Control(pDX, IDC_DGN_EUROCODE204_CONFINED_CHK, m_chkConfinedJoint);
	DDX_Control(pDX, IDC_DGN_EUROCODE204_NOT_CONFINED_CHK, m_chkNotConfinedJoint);
	DDX_Control(pDX, IDC_DGN_SHEAR_ALPHA_CHK, m_chkShearWallAlphaMax);
	DDX_Control(pDX, IDC_DGN_SHEAR_VED_CHK, m_chkVedPrimaryMemb);

	DDX_Control(pDX, IDC_DGN_SEIS_PARAM_GRB, m_grpSeisParam);

	DDX_Radio(pDX, IDC_DGN_PM_CURVE_METHOD_P_RADIO, m_nPMCurveMethod);
	DDX_Radio(pDX, IDC_DGN_EUROCODE204_ND_METHOD1_RDO, m_nNDDesignMethod);
	DDX_Radio(pDX, IDC_DGN_EUROCODE204_TOP_RDO, m_nCheckPos);	

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConCodeNewChild9Dlg, CDconChildDialog)
	//{{AFX_MSG_MAP(CDgnConCodeNewChild9Dlg)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_SPECIAL, OnDgnRcctrlSpecial)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N18, OnChkApplyNTC)
	ON_BN_CLICKED(IDC_DGN_EC8_BTN, OnBtnEC8Data)
	ON_BN_CLICKED(IDC_DGN_SLENDERNESS_AUTO_A_CHK, OnChkAutoCalcA)
	ON_BN_CLICKED(IDC_DGN_SLENDERNESS_AUTO_C_CHK, OnChkAutoCalcC)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N5, OnDuctilityBtn)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N6, OnDuctilityBtn)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_ND_DUCT_RDO, OnDuctilityBtn)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_ND_BTN, OnBtnGrup)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N16, OnBtnGrup)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N17, OnBCJoint)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_ND_METHOD1_RDO, OnNDDesignMethod)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_ND_METHOD2_RDO, OnNDDesignMethod)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_CONFINED_CHK, OnBCConfinedJoint)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_NOT_CONFINED_CHK, OnBCNotConfinedJoint)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_EM_BTN, OnBtnWallEnvelopMethod)
	ON_CBN_SETFOCUS(IDC_DGN_EUROCODE204_ND_CMB, OnSetNDGrupCombo)
	ON_CBN_SETFOCUS(IDC_DGN_EUROCODE204_N15, OnSetGrupCombo)
	ON_CBN_SELCHANGE(IDC_DGN_EUROCODE204_N99, OnSelchangeNTCType)
	ON_CBN_SELCHANGE(IDC_DGN_NATIONAL_ANNEX_COMBO, OnSelchangeNationalAnnexCombo)
	ON_CBN_SELCHANGE(IDC_DGN_EUROCODE204_ND_CMB, OnSelchangeNDGrupCombo)
	ON_CBN_SELCHANGE(IDC_DGN_EUROCODE204_N15, OnSelchangeGrupCombo)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild9Dlg message handlers

BOOL CDgnConCodeNewChild9Dlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	CDconChildDialog::OnInitDialog();
	
	InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeNewChild9Dlg::SetComboBox()
{
	m_ComboNationAnnex.ResetContent();

	const auto vString = CNationalAnnexTool::GetRcNationalAnnex();
	const auto nNA = vString.size();
	for ( int i = 0; i<vString.size(); ++i )
	{
		CDlgUtil::CobxAddItem(m_ComboNationAnnex, vString[i], CNationalAnnexTool::ConvertNationalAnnex(vString[i]));
	}
	CDlgUtil::CobxSetCurSelItemData(m_ComboNationAnnex, m_pData->nNationalAnnex);

	m_cmbNTCType.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbNTCType, _LS(IDS_DGN_NTC2008), RCS_NTC08);
	CDlgUtil::CobxAddItem(m_cmbNTCType, _LS(IDS_DGN_NTC2012), RCS_NTC12);
	CDlgUtil::CobxAddItem(m_cmbNTCType, _LS(IDS_DGN_NTC2018), RCS_NTC18);
	CDlgUtil::CobxSetCurSelItemData(m_cmbNTCType, RCS_NTC12);
}
void CDgnConCodeNewChild9Dlg::InitCtrl()
{
	m_grpSeisParam.InitControl(this, CDgnConCodeNewChild9Dlg::IDD, IDC_DGN_SEIS_PARAM_GRB, TRUE, TRUE);
	m_grpSeisParam.SetFoldState(FALSE); // 무조건 펼친다

	SetComboBox();

	m_edtStrutAngle.SetUnitType(D_UNITSYS_NONE);
	m_edtEffectivePhi.SetUnitType(D_UNITSYS_NONE);
	m_edtSlendLimitA.SetUnitType(D_UNITSYS_NONE);
	m_edtSlendLimitB.SetUnitType(D_UNITSYS_NONE);
	m_edtSlendLimitC.SetUnitType(D_UNITSYS_NONE);

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	if(!m_bExistData)
	{
		int nNTCType = m_pData->nNTCType;
		CDlgUtil::CobxSetCurSelItemData(m_cmbNTCType, m_pData->nNTCType);
		int nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_ComboNationAnnex, m_ComboNationAnnex.GetCurSel());
		if(nNationalAnnex==dgn::def::enNationalAnnex::Italy && nNTCType == RCS_NTC18)
		{
			m_bNTCFlag = TRUE;
			m_iDuctility = 1;
			m_bSpecialEQ = TRUE;
		}
		else m_bSpecialEQ = FALSE;
		m_DconEC8Data.bBehaviorFact = TRUE;// PMS:4490-DANAKIM-20120619
		m_pDgnDataCtrl->Get_DgnConMrftDef(strCode, m_dMrft);
	}
	else
	{
		m_dUf = m_pData->dUf; 
		m_dMrft = m_pData->dMrft;
		m_bNTCFlag = m_pData->bNTCFlag;
		m_bBCJointDgn = m_pData->bBCJointDgn;
		m_bTorsionDesign = m_pData->bTorsionDesign;
		m_dGamma_rdForBeam = m_pData->dGamma_rdForBeam;
		m_dGamma_rdForColm = m_pData->dGamma_rdForColm;
		m_dGamma_rdForWall = m_pData->dGamma_rdForWall;
		m_dGamma_rdForJoint = m_pData->dGamma_rdForJoint;
		m_bStrongColmWeakBeam = m_pData->bStrongColmWeakBeam;
		m_dStorColmWeakBeamFactor = m_pData->dStorColmWeakBeamFactor;
		m_bShearWallAlphaMax = m_pData->bShearWallAlphaMax;
		m_bVedPrimaryMemb = m_pData->bVedPrimaryMemb;

		m_nNDDesignMethod = m_pData->nNDDesignMethod;
		m_dApproximateAlpha = m_pData->dApproximateAlpha;
		
		m_NDGroupKey = m_pData->NonDissipativeGroupKey;	
		m_GroupKey = m_pData->SecondaryGroupKey;	

		// Set EC8 Data
		m_DconEC8Data.nSpecialWall    = m_pData->nSpecialWall;
		m_DconEC8Data.iBndrElemMethod = m_pData->iBndrElemMethod;
		m_DconEC8Data.dCd             = m_pData->dCd;
		m_DconEC8Data.dIe             = m_pData->dIe;

		m_DconEC8Data.iFrameType = m_pData->iFrameType;
		m_DconEC8Data.dAua1 = m_pData->dAua1;
		m_DconEC8Data.SplcK = m_pData->SplcK;
		m_DconEC8Data.dSoilFactor = m_pData->dSoilFactor;
		m_DconEC8Data.dT[0] = m_pData->dT[0];
		m_DconEC8Data.dT[1] = m_pData->dT[1];
		m_DconEC8Data.dT[2] = m_pData->dT[2];
		m_DconEC8Data.dAgR = m_pData->dAgR;
		m_DconEC8Data.dI = m_pData->dI;
		m_DconEC8Data.dDampingRat = m_pData->dDampingRat;
		m_DconEC8Data.bBehaviorFact = m_pData->bBehaviorFact;
		m_DconEC8Data.dBehaviorFactor_q = m_pData->dBehaviorFactor_q;
		m_DconEC8Data.dBehaviorFactor_qo = m_pData->dBehaviorFactor_qo;
		m_DconEC8Data.bT1User = m_pData->bT1User;
		m_DconEC8Data.dT1_x = m_pData->dT1_x;
		m_DconEC8Data.dT1_y = m_pData->dT1_y;

		m_nPMCurveMethod = m_pData->nPMCurveMethod;
		m_nCheckPos = m_pData->nCheckPos;
		m_nFsMethod = m_pData->nFsMethod4Beam;

		int nNTCType = m_pData->nNTCType;
		if (nNTCType <= 2)
		{
			CDlgUtil::CobxSetCurSelItemData(m_cmbNTCType, nNTCType);
		}
		else
		{
			ASSERT(0);
			CDlgUtil::CobxSetCurSelItemData(m_cmbNTCType, RCS_NTC08);
		}

	}

	m_ChkUseVcWall.SetCheck(m_pData->bUseVcWall);// PMS:DNKIM-20121030
	m_ChkUseVcColm.SetCheck(m_pData->bUseVcColm);
	m_ChkUseVcBeam.SetCheck(m_pData->bUseVcBeam);
	m_ChkLimitShearStrgConc.SetCheck(m_pData->bLimitShearStrengthConc);

	m_edtStrutAngle.SetEditUnit(m_pData->dStrutAngle);
	m_edtEffectivePhi.SetEditUnit(m_pData->dPhi_ef);
	m_edtSlendLimitA.SetEditUnit(m_pData->dA);
	m_edtSlendLimitB.SetEditUnit(m_pData->dB);
	m_edtSlendLimitC.SetEditUnit(m_pData->dC);
	m_chkAutoCalcA.SetCheck(m_pData->bAutoA);
	m_chkAutoCalcC.SetCheck(m_pData->bAutoC);
	UpdateData(FALSE);
	
	SetShowLimitShear();
	ShowSlendernessLimit();
	ShowPMCurveMethod();
	ResizeChildDlg();

	OnSetNDGrupCombo();
	OnSetGrupCombo();
	OnDuctilityBtn();
	OnNDDesignMethod();
	OnChkApplyNTC();
	//  OnChkBehaviorFactor();
	OnChkAutoCalcA();
	OnChkAutoCalcC();
	//(Tel. 4022) MNET:XXXX-BSCHOI-20121024 #1
	SetNTC2008EnableControl();
	m_chkApplyBCJDesign.SetCheck(m_pData->bBCJointDgn);
	m_chkConfinedJoint.SetCheck(m_pData->bConfinedJoint);
	m_chkNotConfinedJoint.SetCheck(m_pData->bNotConfinedJoint);
	OnBCJoint();
}

void CDgnConCodeNewChild9Dlg::OnDgnRcctrlSpecial() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ResizeChildDlg();

	SelchangeNTCType(FALSE);
}

void CDgnConCodeNewChild9Dlg::OnChkApplyNTC()
{
	BOOL bEnable = m_chkApplyNTC.GetCheck();

	if(m_strDCH.IsEmpty() && m_strDCM.IsEmpty())
	{
		GetDlgItem(IDC_DGN_EUROCODE204_N5)->GetWindowText(m_strDCH);
		GetDlgItem(IDC_DGN_EUROCODE204_N6)->GetWindowText(m_strDCM);
	}


	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);
	if(bEnable)
	{
		GetDlgItem(IDC_DGN_EUROCODE204_N5)->SetWindowText(_LS(IDS_DGN_CDA_HIGH_DUCTILITY_CLASS));
		GetDlgItem(IDC_DGN_EUROCODE204_N6)->SetWindowText(_LS(IDS_DGN_CDB_MEDIUM_DUCTILITY_CLASS));
		GetDlgItem(IDC_DGN_EUROCODE204_N99)->EnableWindow(bEnable);
		if(strCode==_T("Eurocode2:04")  || strCode==_T("Eurocode2-2:05")) //Eurocode일때만 아래 스트링 찍어줄려고, 나머지 기준은 Special Provision을 찍어주기 위함.
		{
			GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->SetWindowText(_LS(IDS_DGN_TEXT_APPLY_SEISMIC_DGN));
		}

		int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());
		int nCmdDraft = nNTCType == RCS_NTC12 ? SW_SHOW : SW_HIDE;
		GetDlgItem(IDC_DGN_NTC2012_DRAFT_STATIC)->ShowWindow(nCmdDraft);
	}
	else
	{
		GetDlgItem(IDC_DGN_EUROCODE204_N5)->SetWindowText(m_strDCH);
		GetDlgItem(IDC_DGN_EUROCODE204_N6)->SetWindowText(m_strDCM);
		GetDlgItem(IDC_DGN_EUROCODE204_N99)->EnableWindow(bEnable);
		if(strCode==_T("Eurocode2:04")  || strCode==_T("Eurocode2-2:05")) 
		{
			GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->SetWindowText(_LS(IDS_DGN_TEXT_APPLY_EC8_04_CAPACITY_DGN));
		}
		GetDlgItem(IDC_DGN_NTC2012_DRAFT_STATIC)->ShowWindow(SW_HIDE);//임시처리 
	}

	SelchangeNTCType(TRUE);
}

void CDgnConCodeNewChild9Dlg::OnBtnGrup() 
{
	// TODO: Add your control notification handler code here
	//CreateOrActivateDlg(CDBDoc::GetDocPoint(),CMStrtGrupDefDlg::IDD,this);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_GROUP, 0));
}

void CDgnConCodeNewChild9Dlg::SetGrupComboBox(MComboBox& cobx)
{
	cobx.ResetContent();
	cobx.SetItemData(cobx.AddString(_T("None")), 0);
	cobx.SetCurSel(0);

	CArray<T_GRUP_K, T_GRUP_K> aKey;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupKeyList(aKey);
	int nCount = aKey.GetSize();
	if (nCount < 1) return;

	T_GRUP_D Data;
	for (int i = 0; i < nCount; ++i)
	{
		if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(aKey[i], Data)) continue;
		int ix = cobx.AddString(Data.GroupName);
		cobx.SetItemData(ix, aKey[i]);
	}
}

void CDgnConCodeNewChild9Dlg::ShowPMCurveMethod()
{
	int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());
	CString strDesignCode = m_pData->DesignCode;
	int nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_ComboNationAnnex, m_ComboNationAnnex.GetCurSel());

	BOOL bIsNTC2018			= strDesignCode == _T("Eurocode2:04") && nNationalAnnex == dgn::def::enNationalAnnex::Italy && nNTCType == RCS_NTC18 && m_bNTCFlag;
	BOOL bIsEuroRecommend	= strDesignCode == _T("Eurocode2:04") && nNationalAnnex == dgn::def::enNationalAnnex::Recommended;

	BOOL bShowPMCurve = (bIsNTC2018 || bIsEuroRecommend) ? TRUE : FALSE;

	if(m_bShowPMCurveMethod == bShowPMCurve) return;

	m_bShowPMCurveMethod = bShowPMCurve;
}

void CDgnConCodeNewChild9Dlg::OnSetNDGrupCombo()
{
	SetGrupComboBox(m_cmbNDGroup);
	
	T_GRUP_D Data;
	if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(m_NDGroupKey, Data))
	{
		m_cmbNDGroup.SelectString(0, Data.GroupName);
	}
}

void CDgnConCodeNewChild9Dlg::OnSetGrupCombo()
{
	SetGrupComboBox(m_cmbGroup);
	
	T_GRUP_D Data;
	if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(m_GroupKey, Data))
	{
		m_cmbGroup.SelectString(0, Data.GroupName);
	}
}

void CDgnConCodeNewChild9Dlg::OnSelchangeNTCType()
{
	SelchangeNTCType(TRUE);
}

void CDgnConCodeNewChild9Dlg::SelchangeNTCType(BOOL bResize)
{
	if(bResize)
	{
		ShowPMCurveMethod();
		ResizeChildDlg();
	}

	if (m_cmbNTCType.GetCurSel() < 0) return;

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	BOOL bNTCon = m_chkApplyNTC.GetCheck() == TRUE ? TRUE : FALSE;
	int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());

	BOOL bAfterNTC2012 = nNTCType == RCS_NTC08 ? FALSE : TRUE;
	int nCmdDraft      = bNTCon && nNTCType == RCS_NTC12 ? SW_SHOW : SW_HIDE;
	
	if (bAfterNTC2012 && bNTCon == TRUE)
	{
		CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrls, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrlsNTC2012, !m_bGermanyOrSpain);		
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrls, !m_bGermanyOrSpain);
		CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrlsNTC2012, FALSE);
		if(!m_bGermanyOrSpain)
			GetDlgItem(IDC_DGN_SLENDERNESS_LIMIT_STC)->ShowWindow(SW_SHOW);	  
	}

	GetDlgItem(IDC_DGN_NTC2012_DRAFT_STATIC)->ShowWindow(nCmdDraft);	

	// MQC 15839 : Eurocode2:04, NTC2008인 경우 A=0.7, B=1.1로 값을 고정해서 비활성화
	const BOOL bNTC2008 = nNTCType == RCS_NTC08 && bNTCon==TRUE;
	if(bNTC2008)
	{
		m_edtSlendLimitA.SetEditUnit(0.7);
		m_edtSlendLimitB.SetEditUnit(1.1);
		m_chkAutoCalcA.SetCheck(FALSE);
	}
	GetDlgItem(IDC_DGN_SLENDERNESS_A_EDT     )->EnableWindow(bNTC2008 ? FALSE : !m_chkAutoCalcA.GetCheck());
	GetDlgItem(IDC_DGN_SLENDERNESS_B_EDT     )->EnableWindow(!bNTC2008);
	GetDlgItem(IDC_DGN_SLENDERNESS_AUTO_A_CHK)->EnableWindow(!bNTC2008);

	m_chkApplyBCJDesign.SetCheck(m_bSpecialEQ);

	const BOOL bNTC2018 = (nNTCType==RCS_NTC18 && bNTCon);
	if(!bNTC2018)
	{
		if(m_iDuctility==2) m_iDuctility = 0;
	}

	EnableDisableAlphaMax();

	m_chkShearWallAlphaMax.SetCheck(m_bShearWallAlphaMax);
	m_chkVedPrimaryMemb.SetCheck(m_bVedPrimaryMemb);
	CDlgUtil::CtrlRadioSetCheck(this, m_aDuctilityRadio, m_iDuctility);

	BOOL bSeisParamFolded = m_grpSeisParam.GetFoldState();
	GetDlgItem(IDC_DGN_EUROCODE204_ND_DUCT_RDO)->ShowWindow(!bSeisParamFolded && m_bSpecialEQ && bNTCon);
	GetDlgItem(IDC_DGN_EUROCODE204_ND_DUCT_RDO)->EnableWindow(bNTC2018);

	OnBCJoint();
	OnDuctilityBtn();
	OnNDDesignMethod();
}

void CDgnConCodeNewChild9Dlg::OnBCJoint()//PMS:4562 (Tel. 4022) MNET:XXXX-BSCHOI-20121108 #1
{
	BOOL bJoint = m_chkApplyBCJDesign.GetCheck();
	GetDlgItem(IDC_DGN_EUROCODE204_N11)->EnableWindow(bJoint);
	GetDlgItem(IDC_DGN_EUROCODE204_N22)->EnableWindow(bJoint);
	GetDlgItem(IDC_DGN_EUROCODE204_CONFINED_CHK)->EnableWindow(bJoint);
	GetDlgItem(IDC_DGN_EUROCODE204_NOT_CONFINED_CHK)->EnableWindow(bJoint);
	CDlgUtil::CtrlEnableDisable(this, m_aSelectPosGroup, bJoint);
}

void CDgnConCodeNewChild9Dlg::OnNDDesignMethod()
{
	UpdateData(TRUE);

	BOOL bEnable = (m_nNDDesignMethod==1);
	GetDlgItem(IDC_DGN_EUROCODE204_ND_METHOD2_ETC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_EUROCODE204_ND_METHOD2_STC)->EnableWindow(bEnable);
}

void CDgnConCodeNewChild9Dlg::OnBCConfinedJoint()
{
	BOOL bConfined = m_chkConfinedJoint.GetCheck();
	BOOL bNotConfined = m_chkNotConfinedJoint.GetCheck();
	if(!bConfined && !bNotConfined)
	{
		m_chkNotConfinedJoint.SetCheck(TRUE);
	}
}

void CDgnConCodeNewChild9Dlg::OnBCNotConfinedJoint()
{
	BOOL bConfined = m_chkConfinedJoint.GetCheck();
	BOOL bNotConfined = m_chkNotConfinedJoint.GetCheck();
	if(!bConfined && !bNotConfined)
	{
		m_chkConfinedJoint.SetCheck(TRUE);
	}
}

void CDgnConCodeNewChild9Dlg::OnBtnWallEnvelopMethod()
{
	CDgnConWallEnvelopMethod Dlg;
	Dlg.SetMethod(m_pData->nWallDgnEnvelop);

	if (Dlg.DoModal()==IDOK)
	{
		m_pData->nWallDgnEnvelop = Dlg.GetMethod();
	}
}
void CDgnConCodeNewChild9Dlg::OnDuctilityBtn()
{
	UpdateData(TRUE);

	if(m_iDuctility == 0)
	{
		if(!m_bExistData) // PMS:xxxx-Seungjun-20100422 저장된 데이터가 있는 경우 초기값 세팅하지 않음.
		{
			int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());
			if ((nNTCType == RCS_NTC12 || nNTCType==RCS_NTC18) && m_chkApplyNTC.GetCheck() == TRUE)
			{
				m_dGamma_rdForBeam = 1.2;
				m_dGamma_rdForColm = 1.3;
				m_dGamma_rdForWall = 1.2;
				m_dGamma_rdForJoint = 1.2;
			}
			else
			{
				m_dGamma_rdForBeam = 1.2;
				m_dGamma_rdForColm = 1.3;
				m_dGamma_rdForWall = 1.2;
				m_dGamma_rdForJoint = 1.2;
			}
		}

		GetDlgItem(IDC_DGN_EUROCODE204_N8)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EUROCODE204_N21)->EnableWindow(TRUE);
	}
	else
	{
		if(!m_bExistData) // PMS:xxxx-Seungjun-20100422 저장된 데이터가 있는 경우 초기값 세팅하지 않음.
		{
			int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());
			if ((nNTCType == RCS_NTC12 || nNTCType == RCS_NTC18) && m_chkApplyNTC.GetCheck() == TRUE)
			{
				m_dGamma_rdForBeam  = 1.1;
				m_dGamma_rdForColm  = 1.1;
				m_dGamma_rdForWall  = 1.2;
				m_dGamma_rdForJoint = 1.1;
			}
			else
			{
				m_dGamma_rdForBeam  = 1.0;
				m_dGamma_rdForColm  = 1.1;
				m_dGamma_rdForWall  = 1.2;
				m_dGamma_rdForJoint = 1.2;
			}
		}

		GetDlgItem(IDC_DGN_EUROCODE204_N8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EUROCODE204_N21)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_DGN_EUROCODE204_ND_STC)->EnableWindow(m_iDuctility != 2);
	GetDlgItem(IDC_DGN_EUROCODE204_ND_CMB)->EnableWindow(m_iDuctility != 2);
	GetDlgItem(IDC_DGN_EUROCODE204_ND_BTN)->EnableWindow(m_iDuctility != 2);

	UpdateData(FALSE);

	EnableDisableAlphaMax();
}

void CDgnConCodeNewChild9Dlg::OnSelchangeNationalAnnexCombo() 
{
	//SelchangeDgnCtrldataDesigncode(FALSE);
	SetShowLimitShear();
	ShowSlendernessLimit();
	ShowPMCurveMethod();
	ResizeChildDlg();

	SetNTC2008EnableControl();
}

void CDgnConCodeNewChild9Dlg::OnSelchangeNDGrupCombo()
{
	int nSel = m_cmbNDGroup.GetCurSel();
	int nCompareSel = m_cmbGroup.GetCurSel();
	if(nSel!=0 && nSel==nCompareSel)
	{
		m_cmbGroup.SetCurSel(0);
	}
}

void CDgnConCodeNewChild9Dlg::OnSelchangeGrupCombo()
{
	int nSel = m_cmbGroup.GetCurSel();
	int nCompareSel = m_cmbNDGroup.GetCurSel();
	if(nSel!=0 && nSel==nCompareSel)
	{
		m_cmbNDGroup.SetCurSel(0);
	}
}

void CDgnConCodeNewChild9Dlg::SetNTC2008EnableControl()
{
	UpdateData(TRUE);

	BOOL bItaly = FALSE;
	if(CDlgUtil::CobxGetCurSelItemData(m_ComboNationAnnex, m_ComboNationAnnex.GetCurSel()) == dgn::def::enNationalAnnex::Italy)
		bItaly = TRUE;

	if(bItaly)
	{
		m_chkApplyNTC.EnableWindow(TRUE);
		SelchangeNTCType(FALSE);
	}
	else
	{
		m_cmbNTCType.EnableWindow(FALSE);
		m_chkApplyNTC.EnableWindow(FALSE);
		m_chkApplyNTC.SetCheck(FALSE);
		OnChkApplyNTC();
	}
}

void CDgnConCodeNewChild9Dlg::OnBtnEC8Data()
{
	CDgnConCodeEC8InfoDlg Dlg;
	Dlg.SetDconData(&m_DconEC8Data);
	
	if(Dlg.DoModal()==IDOK)
	{
		int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());
		if(m_DconEC8Data.bBehaviorFact==FALSE && m_DconEC8Data.dBehaviorFactor_q<1.5 && nNTCType==RCS_NTC18 && m_bNTCFlag)
		{
			m_iDuctility = 2;
			UpdateData(FALSE);

			OnDuctilityBtn();
		}
	}
}

void CDgnConCodeNewChild9Dlg::OnChkAutoCalcA()
{
	BOOL bCheck = m_chkAutoCalcA.GetCheck();
	if (bCheck)
	{
		double dPhi_ef = m_edtEffectivePhi.GetEditValue();
		if (fabs(dPhi_ef) < cDgn_Zero)  m_edtSlendLimitA.SetEditUnit(0.7);
		else
		{
			double dAutoA = 1.0 / (1.0+0.2*dPhi_ef);
			dAutoA *= pow(10.0, 3);
			dAutoA += 0.5;
			dAutoA = (int)dAutoA;
			dAutoA *= pow(10.0, -3);
			m_edtSlendLimitA.SetEditUnit(dAutoA);
		}
	}
	GetDlgItem(IDC_DGN_SLENDERNESS_A_EDT)->EnableWindow(!bCheck);
}
void CDgnConCodeNewChild9Dlg::OnChkAutoCalcC()
{
	BOOL bCheck = m_chkAutoCalcC.GetCheck();
	GetDlgItem(IDC_DGN_SLENDERNESS_C_EDT)->EnableWindow(!bCheck);
}

void CDgnConCodeNewChild9Dlg::UpdateChildData()
{
	CDconChildDialog::UpdateChildData();
	m_dMrft               = m_pData->dMrft;
	m_bStrongColmWeakBeam = m_pData->bStrongColmWeakBeam;
	m_nPMCurveMethod		= m_pData->nPMCurveMethod;
	UpdateData(FALSE);
}

void CDgnConCodeNewChild9Dlg::ChangeDesignCode()
{
	CDconChildDialog::ChangeDesignCode();
	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);

	SetShowLimitShear();
	ShowPMCurveMethod();
	ResizeChildDlg();

	OnDuctilityBtn();
	OnNDDesignMethod();
	OnChkApplyNTC();
	//  OnChkBehaviorFactor();
	OnChkAutoCalcA();
	OnChkAutoCalcC();
	//(Tel. 4022) MNET:XXXX-BSCHOI-20121024 #1
	SetNTC2008EnableControl();


	m_pDgnDataCtrl->Get_DgnConMrftDef(strCode, m_dMrft);

	UpdateData(FALSE);
}

void CDgnConCodeNewChild9Dlg::SetShowLimitShear()
{
	int nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_ComboNationAnnex, m_ComboNationAnnex.GetCurSel());
	BOOL bLimitShear = (nNationalAnnex == dgn::def::enNationalAnnex::Singapore ||
	                     nNationalAnnex == dgn::def::enNationalAnnex::Greece ||
	                     nNationalAnnex == dgn::def::enNationalAnnex::Malaysia ||
	                     nNationalAnnex == dgn::def::enNationalAnnex::UnitedKingdom ||
	                     nNationalAnnex == dgn::def::enNationalAnnex::Norway);

	CDlgUtil::CtrlShowHide(this, m_aLimitShearStr, bLimitShear);

	// Update text for Norway (C60/75 instead of C50/60)
	CButton* pBtn = (CButton*)GetDlgItem(IDC_DGN_DGN_EUROCODE204_LIMIT_STRG_CHK);
	if (pBtn)
	{
		if (nNationalAnnex == dgn::def::enNationalAnnex::Norway)
			pBtn->SetWindowText(_LS(IDS_IDD_DGN_CON_CODE_CHILD9_DLG_IDC_DGN_EUROCODE204_LIMIT_STRG_CHK_NORWAY));
		else
			pBtn->SetWindowText(_LS(IDS_IDD_DGN_CON_CODE_CHILD9_DLG_IDC_DGN_EUROCODE204_LIMIT_STRG_CHK));
	}

	m_bSingaporeAndMore = bLimitShear;

}
void CDgnConCodeNewChild9Dlg::ShowSlendernessLimit()
{
	int nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_ComboNationAnnex, m_ComboNationAnnex.GetCurSel());
	BOOL bGermanyOrSpain = (nNationalAnnex == dgn::def::enNationalAnnex::Germany || 
	                         nNationalAnnex == dgn::def::enNationalAnnex::Spain);
	if(m_bGermanyOrSpain == bGermanyOrSpain) return;

	CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrls, !bGermanyOrSpain);
	CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrlsNTC2012, !bGermanyOrSpain);

	m_bGermanyOrSpain = bGermanyOrSpain;

}
void CDgnConCodeNewChild9Dlg::ResizeChildDlg()
{ 
	int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());
	BOOL bNTC2018 = FALSE;
	if (m_chkApplyNTC.GetCheck() && nNTCType==RCS_NTC18) bNTC2018 = TRUE;

	CDlgUtil::CtrlShowHideByRect(this, IDC_DGN_SEIS_PARAM_GRB, m_bSpecialEQ, TRUE);
	if(!bNTC2018)
	{
		CDlgUtil::CtrlShowHide(this, m_aDesignMethodND,	SW_HIDE);
		CDlgUtil::CtrlShowHide(this, m_aNonDissipative,	SW_HIDE);
		GetDlgItem(IDC_DGN_EUROCODE204_ND_DUCT_RDO)->ShowWindow(SW_HIDE);
	}

	// Germany, Spain 에서 Apply Eurocode8:04 체크시 Slenderness Limit 여백 메꾸기
	UINT nIDRef = IDC_DGN_PHIEF_STC;
	UINT nIDTgt = IDC_DGN_SEIS_PARAM_GRB;
	UINT nIDRef_dist = IDC_DGN_SLENDERNESS_LIMIT_STC;

	CRect RectRef, RectTgt, RectDist;


	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);
	GetDlgItem(nIDRef_dist)->GetWindowRect(&RectDist);
	ScreenToClient(RectRef);
	ScreenToClient(RectTgt);

	int nSpace = (int)RectRef.Height() / 2;;
	int nDistY = (int)RectDist.Height();

	CArray<CArray<UINT, UINT>*, CArray<UINT, UINT>*> aControlLists;
	aControlLists.Add(&m_aBCJointGroup);
	aControlLists.Add(&m_aSelectPosGroup);
	aControlLists.Add(&m_aStrongColm);
	aControlLists.Add(&m_aSelectDuctility);
	aControlLists.Add(&m_aSecondarySeis);
	aControlLists.Add(&m_aShearForce);
	aControlLists.Add(&m_aEnvelopMethod);
	aControlLists.Add(&m_aFrictionCoeff);

	if (!m_bApplyEurocodeEdited && m_bSpecialEQ && m_bGermanyOrSpain)
	{
		int nMoveDistY = -nDistY;

		for (int i = 0; i < aControlLists.GetCount(); i++)
		{
			CDlgUtil::CtrlMoveDistY(this, *aControlLists[i], nMoveDistY);
		}
		RectTgt.top = RectRef.bottom + nSpace;
		RectTgt.bottom = RectTgt.bottom + nMoveDistY;
		GetDlgItem(nIDTgt)->MoveWindow(RectTgt, TRUE);
		m_bApplyEurocodeEdited = TRUE;
	}
	else if (m_bApplyEurocodeEdited && !(m_bSpecialEQ && m_bGermanyOrSpain))
	{
		for (int i = 0; i < aControlLists.GetCount(); i++)
		{
			CDlgUtil::CtrlMoveDistY(this, *aControlLists[i], nDistY);
		}

		nIDRef = IDC_DGN_SLENDERNESS_LIMIT_STC;
		GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
		ScreenToClient(RectRef);
		RectTgt.top = RectRef.bottom + nSpace;
		RectTgt.bottom = RectTgt.bottom + nDistY;
		GetDlgItem(nIDTgt)->MoveWindow(RectTgt, TRUE);
		m_bApplyEurocodeEdited = FALSE;
	}
	/////////////////////////////////////////////////////////////

	BOOL bSeisParamFolded = m_grpSeisParam.GetFoldState();

	/////////////////////////////////////////////////////////////
	nIDRef = IDC_DGN_EUROCODE204_ND_METHOD_STC;
	nIDTgt = IDC_DGN_EUROCODE204_N15;
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);
	nDistY = 0;
	if(!bSeisParamFolded)
	{
		if(bNTC2018)
		{
			if(RectRef.top == RectTgt.top)
			{
				GetDlgItem(IDC_DGN_EUROCODE204_ND_CMB)->GetWindowRect(&RectRef);

				CRect RectCtrl1;
				CRect RectCtrl2;
				GetDlgItem(IDC_DGN_EUROCODE204_ND_METHOD1_RDO)->GetWindowRect(&RectCtrl1);
				GetDlgItem(IDC_DGN_EUROCODE204_ND_METHOD2_RDO)->GetWindowRect(&RectCtrl2);
				int nSpace = RectCtrl2.top - RectCtrl1.bottom; // Font 변경에 따라 컨트롤 간 상하 간격이 변동되는 것 반영.
				nDistY = RectRef.bottom - RectTgt.top + nSpace;
			}
		}
		else if (!(m_bSpecialEQ && m_bGermanyOrSpain))
		{
			if(RectRef.top != RectTgt.top)
			{
				nDistY = RectRef.top - RectTgt.top;
			}
		}
	}

	if(nDistY!=0)
	{
		CDlgUtil::CtrlMoveDistY(this, m_aSecondarySeis, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aShearForce, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aEnvelopMethod, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aFrictionCoeff, nDistY);
	}
	/////////////////////////////////////////////////////////////

	if(!bSeisParamFolded)
	{
		nIDRef = IDC_DGN_EUROCODE204_N20;
		nIDTgt = IDC_DGN_SEIS_PARAM_GRB;
		GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
		GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);
		ScreenToClient(RectRef);
		ScreenToClient(RectTgt);
		RectTgt.bottom = RectRef.bottom + RectRef.Height()/3;
		GetDlgItem(nIDTgt)->MoveWindow(RectTgt, TRUE);
	}

	/////////////////////////////////////////////////////////////

	GetDlgItem(IDC_DGN_EC8_BTN)->EnableWindow(m_bSpecialEQ);
	GetDlgItem(IDC_DGN_SHEAR_ALPHA_CHK)->EnableWindow(m_bSpecialEQ);
	GetDlgItem(IDC_DGN_SHEAR_VED_CHK)->EnableWindow(bNTC2018);
	
	nIDRef = IDC_DGN_SLENDERNESS_LIMIT_STC;
	nIDTgt = IDC_DGN_RCCTRL_TORSION; 

	// 기준점 결정
	if(m_bSpecialEQ) 
	{
		// Italy NTC2018: Seis Param이 기준
		nIDRef = IDC_DGN_SEIS_PARAM_GRB;
	}
	else if(m_bGermanyOrSpain)
	{
		// Germany/Spain: Slenderness Limit이 숨겨져 있으므로
		nIDRef = IDC_DGN_PHIEF_STC;
	}
	// 그 외: Slenderness Limit 기준
	
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);
	nSpace = (int)RectTgt.Height()/2;
	nDistY = RectRef.bottom - RectTgt.top + nSpace;

	if(nDistY!=0)
	{
		CDlgUtil::CtrlMoveDistY(this, m_aTorsionDesign, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aLimitShearStr, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aConsiderShear, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aPMCurveMethod ,nDistY);	
	}

	// Limit Shear가 숨겨진 경우, 추가 여백 제거 (단, 상태 변경 시에만)
	BOOL bShouldMoveLimitShear = (!m_bSingaporeAndMore || m_bGermanyOrSpain);
	if (bShouldMoveLimitShear != m_bLimitShearMoved)
	{
		int nMoveSpace = (int)RectTgt.Height() * 1.5;
		int nMoveDist = bShouldMoveLimitShear ? -1*nMoveSpace : nMoveSpace;

		CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nMoveDist);
		CDlgUtil::CtrlMoveDistY(this, m_aConsiderShear, nMoveDist);
		CDlgUtil::CtrlMoveDistY(this, m_aPMCurveMethod, nMoveDist);
		
		CRect rMove;
		GetClientRect(rMove);
		rMove.bottom += nMoveDist;
		MoveWindow(rMove, TRUE);
		
		m_bLimitShearMoved = bShouldMoveLimitShear;
	}

	CDlgUtil::CtrlShowHide(this, m_aTorsionDesign, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aLimitShearStr, m_bSingaporeAndMore);
	CDlgUtil::CtrlShowHide(this, m_aMomentRedistr, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aConsiderShear, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aPMCurveMethod, m_bShowPMCurveMethod);
	
	// m_bShowPMCurveMethod에 따라 위치 변경
	nIDRef = IDC_DGN_EUROCODE204_USEVC;
	if(m_bShowPMCurveMethod) nIDRef = IDC_DGN_PM_CURVE_METHOD;
	
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	ScreenToClient(RectRef);
	GetClientRect(&RectTgt);
	RectTgt.bottom = RectRef.bottom + nSpace;
	MoveWindow(RectTgt, TRUE);

	Invalidate();
	
	CDconChildDialog::ResizeChildDlg();
}

BOOL CDgnConCodeNewChild9Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_ComboNationAnnex, m_ComboNationAnnex.GetCurSel());

	m_pData->dMrft       = m_dMrft;
	m_pData->bNTCFlag    = m_bNTCFlag;
	m_pData->nNTCType    = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());
	m_pData->bBCJointDgn = m_bBCJointDgn;
	m_pData->bTorsionDesign = m_bTorsionDesign;
	m_pData->bConfinedJoint = m_chkConfinedJoint.GetCheck();
	m_pData->bNotConfinedJoint = m_chkNotConfinedJoint.GetCheck();

	m_pData->dStrutAngle = m_edtStrutAngle.GetEditValue();
	m_pData->dPhi_ef = m_edtEffectivePhi.GetEditValue();
	m_pData->dA = m_edtSlendLimitA.GetEditValue();
	m_pData->dB = m_edtSlendLimitB.GetEditValue();
	m_pData->dC = m_edtSlendLimitC.GetEditValue();
	m_pData->bAutoA = m_chkAutoCalcA.GetCheck();
	m_pData->bAutoC = m_chkAutoCalcC.GetCheck();

	m_pData->dGamma_rdForBeam  = m_dGamma_rdForBeam;
	m_pData->dGamma_rdForColm  = m_dGamma_rdForColm;
	m_pData->dGamma_rdForWall  = m_dGamma_rdForWall;
	m_pData->dGamma_rdForJoint = m_dGamma_rdForJoint;
	m_pData->bStrongColmWeakBeam     = m_bStrongColmWeakBeam;
	m_pData->dStorColmWeakBeamFactor = m_dStorColmWeakBeamFactor;
	m_pData->bShearWallAlphaMax = m_bShearWallAlphaMax;
	m_pData->bVedPrimaryMemb = m_bVedPrimaryMemb;

	m_pData->dUf     = m_dUf;

	int ix = m_cmbNDGroup.GetCurSel();
	if (ix == CB_ERR) m_pData->NonDissipativeGroupKey = 0;
	else              m_pData->NonDissipativeGroupKey = m_cmbNDGroup.GetItemData(ix);  

	ix = m_cmbGroup.GetCurSel();
	if (ix == CB_ERR) m_pData->SecondaryGroupKey = 0;
	else              m_pData->SecondaryGroupKey = m_cmbGroup.GetItemData(ix);  

	m_pData->nNDDesignMethod = m_nNDDesignMethod;
	m_pData->dApproximateAlpha = m_dApproximateAlpha;

	m_NDGroupKey = m_pData->NonDissipativeGroupKey;
	m_GroupKey = m_pData->SecondaryGroupKey;

	m_pData->bUseVcWall = m_ChkUseVcWall.GetCheck();
	m_pData->bUseVcColm = m_ChkUseVcColm.GetCheck();
	m_pData->bUseVcBeam = m_ChkUseVcBeam.GetCheck();

	// 싱가폴 일때만 저장함
    if ( m_pData->nNationalAnnex == dgn::def::enNationalAnnex::Singapore )
		m_pData->bLimitShearStrengthConc = m_ChkLimitShearStrgConc.GetCheck();

	// Save EC8 Data
	m_pData->iFrameType = m_DconEC8Data.iFrameType;
	m_pData->dAua1 = m_DconEC8Data.dAua1;
	m_pData->SplcK = m_DconEC8Data.SplcK;
	m_pData->dSoilFactor = m_DconEC8Data.dSoilFactor;
	m_pData->dT[0] = m_DconEC8Data.dT[0];
	m_pData->dT[1] = m_DconEC8Data.dT[1];
	m_pData->dT[2] = m_DconEC8Data.dT[2];
	m_pData->dAgR = m_DconEC8Data.dAgR;
	m_pData->dI = m_DconEC8Data.dI;
	m_pData->dDampingRat = m_DconEC8Data.dDampingRat;
	m_pData->bBehaviorFact = m_DconEC8Data.bBehaviorFact;
	m_pData->dBehaviorFactor_q = m_DconEC8Data.dBehaviorFactor_q;
	m_pData->dBehaviorFactor_qo = m_DconEC8Data.dBehaviorFactor_qo;

	m_pData->bT1User = m_DconEC8Data.bT1User;
	m_pData->dT1_x = m_DconEC8Data.dT1_x;
	m_pData->dT1_y = m_DconEC8Data.dT1_y;

	m_pData->nPMCurveMethod = m_nPMCurveMethod;
	m_pData->nCheckPos = m_nCheckPos;
	m_pData->nFsMethod4Beam = m_nFsMethod;
	return CDconChildDialog::Dlg2Data();

}

LRESULT CDgnConCodeNewChild9Dlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	BOOL bSeisParamFolded = m_grpSeisParam.GetFoldState();
	if (bSeisParamFolded)
	{
		ResizeChildDlg();
	}
	else
	{
		SelchangeNTCType(TRUE);
	}

	return 0L;
}

void CDgnConCodeNewChild9Dlg::EnableDisableAlphaMax()
{
	int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());
	BOOL bNTC2018 = FALSE;
	if (m_chkApplyNTC.GetCheck() && nNTCType==RCS_NTC18) bNTC2018 = TRUE;

	BOOL bEnableAlphaMax = FALSE;
	if(m_iDuctility==0 || (bNTC2018 && m_iDuctility!=2))
	{
		bEnableAlphaMax = TRUE;
	}
	GetDlgItem(IDC_DGN_SHEAR_ALPHA_CHK)->EnableWindow(bEnableAlphaMax);
}