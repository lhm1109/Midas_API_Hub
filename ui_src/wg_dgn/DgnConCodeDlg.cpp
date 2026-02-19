// DgnConCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"//Pinakin for Unit conversion of subrex
#include "..\wg_db\DBCodeDef.h"

#include "..\dgnengine\src\DgnBase\DgnBaseDef.h"

#include "DgnConCodeEC8InfoDlg.h"
#include "DgnConCodeSeisDefDlg.h"
#include "DgnDataCtrl.h"
#include "NationalAnnexTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeDlg dialog
using namespace dgn::def;

CDgnConCodeDlg::CDgnConCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConCodeDlg)
	m_bSpecialEQ = FALSE;
	m_iColmMethod = -1;
	m_iSafeLevel = -1;
	m_iEqGrade = -1;
	m_iPmCurve = -1;
	m_iEqCategory = -1;
	m_iSlabType = -1;
	m_dMrft = 0.0;
	m_dTrft = 0.0;
	m_iAlphaType1 = -1;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dConShearStrengthRatio = 0.0;
	m_iFrameType = -1;
	m_dStorColmWeakBeamFactor = 0.0;
	m_dGamma_rdForBeam = 0.0;
	m_dGamma_rdForColm = 0.0;
	m_dGamma_rdForWall = 0.0;
	m_dGamma_rdForJoint = 0.0;
	m_iDuctility = -1;
	m_bTorsionDesign = FALSE;
	m_bBCJointDgn = TRUE;

	m_bNTCFlag = FALSE;
	m_bUseVcWall = TRUE;
	m_bUseVcColm = TRUE;
	m_bUseVcBeam = TRUE;

	m_dUf = 0.6;
	m_bSpecialWall = FALSE;
	m_nSpecialWallMtd = 0;
	m_nBndrElemMethod = 0;

	m_dCd = 4.5;
	m_dIe = 1.2;
	m_nExposureFactor = -1;
	m_nPouringMethod = -1;

	m_nExposure = 0;

	m_bmb7mb8 = FALSE;
	m_nmb7mb8Type = 0;
	m_bmb9 = FALSE;
	m_nmb9Type = 0;
	m_bChkPM = FALSE;

	//m_dedtFOS=3.0;//Pinakin
	//m_dedtSubRex=15000.0;//Pinakin
	//}}AFX_DATA_INIT

	m_DconEC8Data.Initialize();
	m_GroupKey = 0;

	m_aStrutAngleCtrls.Add(IDC_DGN_STRUT_ANGLE_STC);
	m_aStrutAngleCtrls.Add(IDC_DGN_STRUT_ANGLE_EDT);
	m_aStrutAngleCtrls.Add(IDC_DGN_STRUT_ANGLE_UNT);

	m_aEffectivePhiCtrls.Add(IDC_DGN_PHIEF_STC);
	m_aEffectivePhiCtrls.Add(IDC_DGN_PHIEF_EDT);

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

	m_aNTCCheckCombo.RemoveAll();
	m_aNTCCheckCombo.Add(IDC_DGN_EUROCODE204_N18);
	m_aNTCCheckCombo.Add(IDC_DGN_EUROCODE204_N99);
	m_aNTCCheckCombo.Add(IDC_DGN_NTC2012_DRAFT_STATIC);
	m_aNTCCheckCombo.FreeExtra();

	m_aSpecialProvision.RemoveAll();
	m_aSpecialProvision.Add(IDC_DGN_RCCTRL_SPECIAL);
	m_aSpecialProvision.Add(IDC_DGN_EC8_BTN);
	m_aSpecialProvision.FreeExtra();

	//PMS:4562 (Tel. 4022) MNET:XXXX-BSCHOI-20121108 #1
	m_aSlendernessCtrlsNTC2012.RemoveAll();
	m_aSlendernessCtrlsNTC2012.Add(IDC_DGN_SLENDERNESS_LIMIT_STC);
	m_aSlendernessCtrlsNTC2012.Add(IDC_DGN_LAMBDA_LIMIT_NTC2012_STC);
	m_aSlendernessCtrlsNTC2012.Add(IDC_DGN_LAMBDA_LIMIT_NTC2012_PROP_STC);
	m_aSlendernessCtrlsNTC2012.FreeExtra();

	// PMS:DNKIM-20121030
	m_aUseVc.RemoveAll();
	m_aUseVc.Add(IDC_DGN_EUROCODE204_USEVC);
	m_aUseVc.Add(IDC_EUROCODE204_USEVC_WALL);
	m_aUseVc.Add(IDC_EUROCODE204_USEVC_COLM);
	m_aUseVc.Add(IDC_EUROCODE204_USEVC_BEAM);
	m_aUseVc.FreeExtra();


	m_aBCJointGroup.RemoveAll();
	m_aBCJointGroup.Add(IDC_DGN_EUROCODE204_N17);
	m_aBCJointGroup.Add(IDC_DGN_EUROCODE204_N11);
	m_aBCJointGroup.Add(IDC_DGN_EUROCODE204_N22);
	m_aBCJointGroup.FreeExtra();

	m_aRdoSCWBMethod.RemoveAll();
	m_aRdoSCWBMethod.Add(IDC_DGN_SCWB_METHOD_DGN_STREN_RDO);
	m_aRdoSCWBMethod.Add(IDC_DGN_SCWB_METHOD_NOR_STREN_RDO);
	m_aRdoSCWBMethod.FreeExtra();

	m_aCtrlSCWBMethod.RemoveAll();
	m_aCtrlSCWBMethod.Add(IDC_DGN_SCWB_METHOD_GRP);
	m_aCtrlSCWBMethod.Append(m_aRdoSCWBMethod);
	m_aCtrlSCWBMethod.FreeExtra();

	m_aExposure.RemoveAll();
	m_aExposure.Add(IDC_DGN_EXPOSURE_STC);
	m_aExposure.Add(IDC_DGN_EXPOSURE_DRY_RDO);
	m_aExposure.Add(IDC_DGN_EXPOSURE_ETC_RDO);

	m_aCondiFactor.RemoveAll();
	m_aCondiFactor.Add(IDC_DGN_MB7_MB8_GRP);
	m_aCondiFactor.Add(IDC_DGN_MB7_MB8_CHK);
	m_aCondiFactor.Add(IDC_DGN_MB7_MB8_RDO1);
	m_aCondiFactor.Add(IDC_DGN_MB7_MB8_RDO2);
	m_aCondiFactor.Add(IDC_DGN_MB7_MB8_RDO3);
	m_aCondiFactor.Add(IDC_DGN_MB7_MB8_RDO4);
	m_aCondiFactor.Add(IDC_DGN_MB7_STC);
	m_aCondiFactor.Add(IDC_DGN_MB7_EDT);
	m_aCondiFactor.Add(IDC_DGN_MB8_STC);
	m_aCondiFactor.Add(IDC_DGN_MB8_EDT);
	m_aCondiFactor.Add(IDC_DGN_MB9_GRP);
	m_aCondiFactor.Add(IDC_DGN_MB9_CHK);
	m_aCondiFactor.Add(IDC_DGN_MB9_RDO1);
	m_aCondiFactor.Add(IDC_DGN_MB9_RDO2);
	m_aCondiFactor.Add(IDC_DGN_MB9_STC);
	m_aCondiFactor.Add(IDC_DGN_MB9_EDT);
	m_aCondiFactor.FreeExtra();

	m_aCtrlWallBoundMtd.RemoveAll();
	m_aCtrlWallBoundMtd.Add(IDC_DGN_WALLTYPE_N2);
	m_aCtrlWallBoundMtd.Add(IDC_DGN_WALLTYPE_N3);
	m_aCtrlWallBoundMtd.Add(IDC_DGN_WALLTYPE_N4);
	m_aCtrlWallBoundMtd.Add(IDC_DGN_WALLTYPE_N5);
	m_aCtrlWallBoundMtd.Add(IDC_DGN_WALLTYPE_N6);
	m_aCtrlWallBoundMtd.Add(IDC_DGN_WALLTYPE_N7);
	m_aCtrlWallBoundMtd.Add(IDC_DGN_WALLTYPE_N8);
	m_aCtrlWallBoundMtd.FreeExtra();

	m_aRdoIS1392Sel.RemoveAll();
	m_aRdoIS1392Sel.Add(IDC_DGN_IS204_N26);
	m_aRdoIS1392Sel.Add(IDC_DGN_IS204_N27);
	m_aRdoIS1392Sel.FreeExtra();

	m_aCtrlIS3370_GrpA.RemoveAll();
	m_aCtrlIS3370_GrpA.Add(IDC_DGN_MB9_GRP2);
	m_aCtrlIS3370_GrpA.Add(IDC_DGN_MB9_CHK2);
	m_aCtrlIS3370_GrpA.Add(IDC_DGN_RCCTRL_IS456MAT_STATIC2);
	m_aCtrlIS3370_GrpA.Add(IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR2);
	m_aCtrlIS3370_GrpA.Add(IDC_DGN_RCCTRL_IS456MAT_STATIC3);
	m_aCtrlIS3370_GrpA.Add(IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR3);
	m_aCtrlIS3370_GrpA.Add(IDC_DGN_RCCTRL_IS456MAT_STATIC4);
	m_aCtrlIS3370_GrpA.Add(IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR4);
	m_aCtrlIS3370_GrpA.FreeExtra();

	m_aCtrlIS3370_GrpB.RemoveAll();
	m_aCtrlIS3370_GrpB.Add(IDC_DGN_RCCTRL_IS456MAT_STATIC5);
	m_aCtrlIS3370_GrpB.Add(IDC_DGN_WALLTYPE_N15);
	m_aCtrlIS3370_GrpB.Add(IDC_DGN_IS456MAT_GROUP2);
	m_aCtrlIS3370_GrpB.Add(IDC_DGN_IS456MAT_GROUP3);
	m_aCtrlIS3370_GrpB.FreeExtra();

	m_aCtrlIS3370_RadAB.RemoveAll();
	m_aCtrlIS3370_RadAB.Add(IDC_DGN_IS3370_N28);
	m_aCtrlIS3370_RadAB.Add(IDC_DGN_IS3370_N29);
	m_aCtrlIS3370_RadAB.FreeExtra();

	m_aCtrlIS3370.RemoveAll();
	m_aCtrlIS3370.Append(m_aCtrlIS3370_RadAB);
	m_aCtrlIS3370.Append(m_aCtrlIS3370_GrpA);
	m_aCtrlIS3370.Append(m_aCtrlIS3370_GrpB);
	m_aCtrlIS3370.FreeExtra();

	m_aRdoBeamDgn.RemoveAll();
	m_aRdoBeamDgn.Add(IDC_DGN_BEAMDGN_REINFORCEMENT1_RDO);
	m_aRdoBeamDgn.Add(IDC_DGN_BEAMDGN_REINFORCEMENT2_RDO);
	m_aRdoBeamDgn.FreeExtra();

	m_aRdoColumnDgn.RemoveAll();
	m_aRdoColumnDgn.Add(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE1_RDO);
	m_aRdoColumnDgn.Add(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE2_RDO);
	m_aRdoColumnDgn.FreeExtra();

	m_aAnnexCtrls.RemoveAll();
	m_aAnnexCtrls.Add(IDC_CRC_DGN_ANNEX_AASHTO_STC);
	m_aAnnexCtrls.Add(IDC_CRC_DGN_ANNEX_AASHTO_CMB);
	m_aAnnexCtrls.FreeExtra();

	//   m_arAllGroup.RemoveAll();
	//   m_aAllGroupK.RemoveAll();
	//   m_aExcludeGroupK.RemoveAll();
	//   m_aNotSFSRGroupK.RemoveAll();

	// PMS:xxxx-Seungjun-20100422 저장된 데이터가 있는 경우 초기값 세팅하지 않음.
	m_bExistData = FALSE;

	m_aCodeEnable.RemoveAll();
	m_nOldCodeIndex = 0;

	m_pDgnDataCtrl = new CDgnDataCtrl;
}

CDgnConCodeDlg::~CDgnConCodeDlg()
{
	_SAFE_DELETE(m_pDgnDataCtrl);
}

#define IDCON0(n)\
	arID0.Add(IDC_DGN_SLABTYPE##n);
#define IDCON1(n)\
	arID1.Add(IDC_DGN_COLM_METHOD##n);
#define IDCON2(n)\
	arID2.Add(IDC_DGN_SAFETY_LEVEL##n);
#define IDCON3(n)\
	arID3.Add(IDC_DGN_EQ_GRADE##n);
#define IDCON4(n)\
	arID4.Add(IDC_DGN_PM_CURVE##n);
#define IDCON6(n)\
	arID6.Add(IDC_DGN_EQ_CATEGORY##n);
#define IDCON7(n)\
	arID7.Add(IDC_DGN_EQ_FACTOR##n);
#define IDCON8(n)\
	arID8.Add(IDC_DGN_EQ_ACI##n);
#define IDCON9(n)\
	arID9.Add(IDC_DGN_EUROCODE204_N##n);
#define IDCON10(n)\
	arID10.Add(IDC_DGN_WALLTYPE_N##n);
#define IDCON10PART(n)\
	arID10_part.Add(IDC_DGN_KCI_USD07_N##n);
#define IDCON11(n)\
	arID11.Add(IDC_DGN_AASHTO07_PARAM##n);
#define IDCON12(n)\
	arID12.Add(IDC_DGN_IS204_N##n);
#define IDCON13(n)\
	arID13.Add(IDC_DGN_IS3370_N##n);

void CDgnConCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeDlg)
	DDX_Control(pDX, IDC_DGN_CTRLDATA_DESIGNCODE, m_DesignCode);
	DDX_Check(pDX, IDC_DGN_RCCTRL_SPECIAL, m_bSpecialEQ);
	DDX_Radio(pDX, IDC_DGN_COLM_METHOD1, m_iColmMethod);
	DDX_Radio(pDX, IDC_DGN_SAFETY_LEVEL1, m_iSafeLevel);
	DDX_Radio(pDX, IDC_DGN_EQ_GRADE1, m_iEqGrade);
	DDX_Radio(pDX, IDC_DGN_PM_CURVE1, m_iPmCurve);
	DDX_Radio(pDX, IDC_DGN_EQ_CATEGORY1, m_iEqCategory);
	DDX_Radio(pDX, IDC_DGN_SLABTYPE1, m_iSlabType);
	DDX_Text(pDX, IDC_DGN_RCCTRL_MRFT_FACTOR, m_dMrft);
	DDX_Text(pDX, IDC_DGN_RCCTRL_TRFT_FACTOR, m_dTrft);
	DDX_Radio(pDX, IDC_DGN_EQ_FACTOR1, m_iAlphaType1);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR7, m_dAlpha1);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR10, m_dAlpha2);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR14, m_dConShearStrengthRatio);
	DDX_Radio(pDX, IDC_DGN_EQ_ACI1, m_iFrameType);
	DDX_Radio(pDX, IDC_DGN_EUROCODE204_N5, m_iDuctility);
	DDX_Control(pDX, IDC_DGN_EUROCODE204_N15, m_cmbGroup);
	DDX_Text(pDX, IDC_DGN_EUROCODE204_N2, m_dStorColmWeakBeamFactor);
	DDX_Text(pDX, IDC_DGN_EUROCODE204_N10, m_dGamma_rdForBeam);
	DDX_Text(pDX, IDC_DGN_EUROCODE204_N13, m_dGamma_rdForColm);
	DDX_Text(pDX, IDC_DGN_EUROCODE204_N21, m_dGamma_rdForWall);
	DDX_Text(pDX, IDC_DGN_EUROCODE204_N22, m_dGamma_rdForJoint);
	DDX_Check(pDX, IDC_DGN_RCCTRL_TORSION, m_bTorsionDesign);
	DDX_Check(pDX, IDC_DGN_EUROCODE204_N17, m_bBCJointDgn);

	DDX_Check(pDX, IDC_DGN_EUROCODE204_N18, m_bNTCFlag);
	DDX_Control(pDX, IDC_EUROCODE204_USEVC_WALL, m_ChkUseVcWall); // PMS:DNKIM-20121030
	DDX_Control(pDX, IDC_EUROCODE204_USEVC_COLM, m_ChkUseVcColm);
	DDX_Control(pDX, IDC_EUROCODE204_USEVC_BEAM, m_ChkUseVcBeam);

	DDX_Control(pDX, IDC_DGN_RCCTRL_TORSION, m_btnTorsion);
	DDX_Control(pDX, IDC_DGN_NATIONAL_ANNEX_COMBO, m_ComboNationAnnex);
	DDX_Text(pDX, IDC_DGN_EUROCODE204_N20, m_dUf);
	DDX_Control(pDX, IDC_CRC_DGN_ANNEX_AASHTO_CMB, m_DesignAnnex);
	//(Tel. 4022) MNET:XXXX-BSCHOI-20121015 #F1
	DDX_Control(pDX, IDC_DGN_EUROCODE204_N18, m_chkApplyNTC);
	DDX_Control(pDX, IDC_DGN_EUROCODE204_N17, m_chkApplyBCJDesign);
	DDX_Control(pDX, IDC_DGN_EUROCODE204_N99, m_cmbNTCType);
	//  DDX_Control(pDX, IDC_DGN_EUROCODE204_N23, m_chkBehaviorFactor);
	DDX_Control(pDX, IDC_DGN_STRUT_ANGLE_EDT, m_edtStrutAngle);
	DDX_Control(pDX, IDC_DGN_PHIEF_EDT, m_edtEffectivePhi);
	//  DDX_Control(pDX, IDC_DGN_EUROCODE204_N24, m_edtBehaviorFactor);
	DDX_Control(pDX, IDC_DGN_SLENDERNESS_A_EDT, m_edtSlendLimitA);
	DDX_Control(pDX, IDC_DGN_SLENDERNESS_B_EDT, m_edtSlendLimitB);
	DDX_Control(pDX, IDC_DGN_SLENDERNESS_C_EDT, m_edtSlendLimitC);
	DDX_Control(pDX, IDC_DGN_SLENDERNESS_AUTO_A_CHK, m_chkAutoCalcA);
	DDX_Control(pDX, IDC_DGN_SLENDERNESS_AUTO_C_CHK, m_chkAutoCalcC);

	DDX_Check(pDX, IDC_DGN_WALLTYPE_N1, m_bSpecialWall);
	DDX_Radio(pDX, IDC_DGN_WALLTYPE_N12, m_nSpecialWallMtd);
	DDX_Radio(pDX, IDC_DGN_WALLTYPE_N3, m_nBndrElemMethod);
	DDX_Control(pDX, IDC_DGN_WALLTYPE_N5, m_CmbCd);
	DDX_Control(pDX, IDC_DGN_WALLTYPE_N7, m_CmbIe);

	DDX_Radio(pDX, IDC_DGN_AASHTO07_PARAM1, m_nExposureFactor);
	DDX_Radio(pDX, IDC_DGN_AASHTO07_PARAM4, m_nPouringMethod);

	DDX_Check(pDX, IDC_DGN_KCI_USD07_N2, m_bSubBeam);
	DDX_Check(pDX, IDC_DGN_KCI_USD07_N3, m_bCantilever);
	DDX_Check(pDX, IDC_DGN_KCI_USD07_N4, m_bUnderBmColm);

	DDX_Radio(pDX, IDC_DGN_EXPOSURE_DRY_RDO, m_nExposure);
	DDX_Control(pDX, IDC_DGN_BRIDGE_TYPE_CMB, m_cmbBridgeType);
	DDX_Control(pDX, IDC_DGN_DG_EDT, m_EdtDg);
	DDX_Control(pDX, IDC_DGN_DG_UNT, m_UntDg);
	DDX_Control(pDX, IDC_DGN_PL_CMB, m_CmbPL);
	DDX_Control(pDX, IDC_DGN_SL_CMB, m_CmbSL);
	DDX_Control(pDX, IDC_DGN_KC_EDT, m_EdtKc);
	DDX_Control(pDX, IDC_DGN_STYPE_CMB, m_CmbSType);
	DDX_Check(pDX, IDC_DGN_MB7_MB8_CHK, m_bmb7mb8);
	DDX_Radio(pDX, IDC_DGN_MB7_MB8_RDO1, m_nmb7mb8Type);
	DDX_Control(pDX, IDC_DGN_MB7_EDT, m_edtmb7);
	DDX_Control(pDX, IDC_DGN_MB8_EDT, m_edtmb8);
	DDX_Check(pDX, IDC_DGN_MB9_CHK, m_bmb9);
	DDX_Radio(pDX, IDC_DGN_MB9_RDO1, m_nmb9Type);
	DDX_Control(pDX, IDC_DGN_MB9_EDT, m_edtmb9);
	DDX_Check(pDX, IDC_DGN_PM_CURVE_CHK, m_bChkPM);
	DDX_Control(pDX, IDC_DGN_MB9_CHK2, m_chkIS3370);

	DDX_Control(pDX, IDC_DGN_DGN_EUROCODE204_LIMIT_STRG_CHK, m_ChkLimitShearStrgConc);
	DDX_Control(pDX, IDC_DGN_RCCTRL_IS456MAT_SUBGRDREXFACTOR, m_dedtSubRex); //Pinakin added for IS456 MAT design
	DDX_Control(pDX, IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR, m_dedtFOS); //Pinakin added for IS456 MAT design
	DDX_Control(pDX, IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX_MODULUS_UNIT, m_ctrlUnit);
	//Pinakin added for IS456 MAT design

	DDX_Radio(pDX, IDC_DGN_IS204_N26, m_iIs13920Sel);
	DDX_Radio(pDX, IDC_DGN_IS3370_N28, m_iIS3370Sel);
	DDX_Control(pDX, IDC_DGN_WALLTYPE_N15, m_cmbIS3370AnnexB2mm);
	DDX_Control(pDX, IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR2, m_dedtESS);
	DDX_Control(pDX, IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR3, m_dedtETT);
	DDX_Control(pDX, IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR4, m_dedtT1);
	//DDX_Radio  (pDX, IDC_DGN_IS3370_N29,  m_iIS3370Sel);

	//}}AFX_DATA_MAP
	CArray<UINT, UINT> arID0;  IDCON0(0)IDCON0(1)IDCON0(2);
	CArray<UINT, UINT> arID1;  IDCON1(0)IDCON1(1)IDCON1(2)IDCON1(3);
	CArray<UINT, UINT> arID2;  IDCON2(0)IDCON2(1)IDCON2(2)IDCON2(3);
	CArray<UINT, UINT> arID3;  IDCON3(0)IDCON3(1)IDCON3(2)IDCON3(3)IDCON3(4)IDCON3(5)IDCON3(6);
	CArray<UINT, UINT> arID4;  IDCON4(0)IDCON4(1)IDCON4(2);
	CArray<UINT, UINT> arID6;  IDCON6(0)IDCON6(1)IDCON6(2)IDCON6(3)IDCON6(4);
	CArray<UINT, UINT> arID7;  IDCON7(0)IDCON7(1)IDCON7(2)IDCON7(3)IDCON7(4)IDCON7(5)IDCON7(6)IDCON7(7)IDCON7(8)IDCON7(9)IDCON7(10)IDCON7(11)IDCON7(12)IDCON7(13)IDCON7(14)IDCON7(15)IDCON7(16)IDCON7(17)IDCON7(18)IDCON7(19)IDCON7(20)IDCON7(21);
	CArray<UINT, UINT> arID8;  IDCON8(0)IDCON8(1)IDCON8(2)IDCON8(3);
	//CArray<UINT,UINT> arID9;  IDCON9(18)IDCON9(1)IDCON9(2)IDCON9(3)IDCON9(4)IDCON9(5)IDCON9(6)IDCON9(7)IDCON9(8)IDCON9(9)IDCON9(10)IDCON9(11)IDCON9(12)IDCON9(13)IDCON9(14)IDCON9(15)IDCON9(16)IDCON9(17)IDCON9(0)IDCON9(19)IDCON9(20)IDCON9(21)IDCON9(22)IDCON9(99);
	CArray<UINT, UINT> arID9;  IDCON9(0)IDCON9(14)IDCON9(1)IDCON9(2)IDCON9(3)IDCON9(4)IDCON9(5)IDCON9(6)IDCON9(7)IDCON9(8)IDCON9(9)IDCON9(10)IDCON9(12)IDCON9(13)IDCON9(15)IDCON9(16)IDCON9(19)IDCON9(20)IDCON9(21);
	CArray<UINT, UINT> arID10; IDCON10(0)IDCON10(1)IDCON10(2)IDCON10(3)IDCON10(4)IDCON10(5)IDCON10(6)IDCON10(7)IDCON10(8)IDCON10(12)IDCON10(13)IDCON10(14);
	CArray<UINT, UINT> arID10_part;  IDCON10PART(1)IDCON10PART(2)IDCON10PART(3)IDCON10PART(4);
	CArray<UINT, UINT> arID11; IDCON11(0)IDCON11(1)IDCON11(2)IDCON11(3)IDCON11(4)IDCON11(5);
	CArray<UINT, UINT> arID12; IDCON12(25)IDCON12(26) IDCON12(27);
	CArray<UINT, UINT> arID13; IDCON13(28)IDCON13(29);


	int i = 0;
	for (i = 0; i < iDgn_ConID0; i++) m_iID0[i] = arID0[i];
	for (i = 0; i < iDgn_ConID1; i++) m_iID1[i] = arID1[i];
	for (i = 0; i < iDgn_ConID2; i++) m_iID2[i] = arID2[i];
	for (i = 0; i < iDgn_ConID3; i++) m_iID3[i] = arID3[i];
	for (i = 0; i < iDgn_ConID4; i++) m_iID4[i] = arID4[i];
	for (i = 0; i < iDgn_ConID6; i++) m_iID6[i] = arID6[i];
	for (i = 0; i < iDgn_ConID7; i++) m_iID7[i] = arID7[i];
	for (i = 0; i < iDgn_ConID8; i++) m_iID8[i] = arID8[i];
	for (i = 0; i < iDgn_ConID9; i++) m_iID9[i] = arID9[i];
	for (i = 0; i < iDgn_ConID10; i++) m_iID10[i] = arID10[i];
	for (i = 0; i < iDgn_ConID10_part; i++) m_iID10_part[i] = arID10_part[i];
	for (i = 0; i < iDgn_ConID11; i++) m_iID11[i] = arID11[i];
	for (i = 0; i < iDgn_ConID12; i++) m_iID12[i] = arID12[i];
	for (i = 0; i < iDgn_ConID13; i++) m_iID13[i] = arID13[i];
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConCodeDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConCodeDlg)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_SPECIAL, OnDgnRcctrlSpecial)
	ON_CBN_SELCHANGE(IDC_DGN_CTRLDATA_DESIGNCODE, OnSelchangeDgnCtrldataDesigncode)
	ON_CBN_SELCHANGE(IDC_DGN_EUROCODE204_N99, OnSelchangeNTCType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR1, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR2, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR3, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR4, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_ACI1, OnChangeFrameType)
	ON_BN_CLICKED(IDC_DGN_EQ_ACI2, OnChangeFrameType)
	ON_BN_CLICKED(IDC_DGN_EQ_ACI3, OnChangeFrameType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR12, OnUpdateByCode)
	ON_CBN_SETFOCUS(IDC_DGN_EUROCODE204_N15, OnSetGrupCombo)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N5, OnDuctilityBtn)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N6, OnDuctilityBtn)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N16, OnBtnGrup)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N17, OnBCJoint)
	//ON_BN_CLICKED(IDC_DGN_WALLTYPE_N11, OnBtnNewGrup) // MNET:4076-SHJUNG-20100609 - 이 기능 안 쓴다. 다른 옵션으로 대체.
	ON_BN_CLICKED(IDC_DGN_RCCTRL_TORSION, OnChangeTorsion)
	ON_BN_CLICKED(IDC_DGN_EC8_BTN, OnBtnEC8Data)
	ON_CBN_SELCHANGE(IDC_DGN_NATIONAL_ANNEX_COMBO, OnSelchangeNationalAnnexCombo)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N18, OnChkApplyNTC)
	ON_CBN_SELCHANGE(IDC_CRC_DGN_ANNEX_AASHTO_CMB, OnSelchangeDgnCtrldataDesignAnnexAashto)

	// PMS:DNKIM-20121030
	ON_BN_CLICKED(IDC_EUROCODE204_USEVC_WALL, OnChkUseVc)
	ON_BN_CLICKED(IDC_EUROCODE204_USEVC_COLM, OnChkUseVc)
	ON_BN_CLICKED(IDC_EUROCODE204_USEVC_BEAM, OnChkUseVc)


	//  ON_BN_CLICKED(IDC_DGN_EUROCODE204_N23, OnChkBehaviorFactor)
	ON_BN_CLICKED(IDC_DGN_SLENDERNESS_AUTO_A_CHK, OnChkAutoCalcA)
	ON_BN_CLICKED(IDC_DGN_SLENDERNESS_AUTO_C_CHK, OnChkAutoCalcC)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N1, OnChkSpecialWall)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N12, OnChkSpecialWall)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N13, OnChkSpecialWall)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N14, OnChkSpecialWall)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N3, OnChangeBndrElemMethodType)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N8, OnChangeBndrElemMethodType)
	ON_CBN_SELCHANGE(IDC_DGN_WALLTYPE_N5, OnSelchangeCdCmb)
	ON_CBN_SELCHANGE(IDC_DGN_WALLTYPE_N7, OnSelchangeIeCmb)
	ON_CBN_EDITCHANGE(IDC_DGN_WALLTYPE_N5, OnEditchangeCdCmb)
	ON_CBN_EDITCHANGE(IDC_DGN_WALLTYPE_N7, OnEditchangeIeCmb)
	ON_BN_CLICKED(IDC_DGN_AASHTO07_PARAM1, OnChangeExposureFactor)
	ON_BN_CLICKED(IDC_DGN_AASHTO07_PARAM2, OnChangeExposureFactor)
	ON_BN_CLICKED(IDC_DGN_AASHTO07_PARAM4, OnChangePouringMethod)
	ON_BN_CLICKED(IDC_DGN_AASHTO07_PARAM5, OnChangePouringMethod)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_ADV_BTN, OnDgnCtrlDataAdvBtn)

	ON_BN_CLICKED(IDC_DGN_MB7_MB8_CHK, OnConditionFactormb7mb8Chk)
	ON_BN_CLICKED(IDC_DGN_MB7_MB8_RDO1, OnConditionFactormb7mb8TypeRdo)
	ON_BN_CLICKED(IDC_DGN_MB7_MB8_RDO2, OnConditionFactormb7mb8TypeRdo)
	ON_BN_CLICKED(IDC_DGN_MB7_MB8_RDO3, OnConditionFactormb7mb8TypeRdo)
	ON_BN_CLICKED(IDC_DGN_MB7_MB8_RDO4, OnConditionFactormb7mb8TypeRdo)
	ON_BN_CLICKED(IDC_DGN_MB9_CHK, OnConditionFactormb9Chk)
	ON_BN_CLICKED(IDC_DGN_MB9_RDO1, OnConditionFactormb9TypeRdo)
	ON_BN_CLICKED(IDC_DGN_MB9_RDO2, OnConditionFactormb9TypeRdo)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DGN_IS204_N26, OnChangeIS13920_DuctileCode) //&CDgnConCodeDlg::OnBnClickedDgnIs204N25
	ON_BN_CLICKED(IDC_DGN_IS204_N27, OnChangeIS13920_DuctileCode)


	ON_BN_CLICKED(IDC_DGN_MB9_CHK2, OnChangeIS3370_CrackCheck)
	ON_BN_CLICKED(IDC_DGN_IS3370_N28, OnConditionIS3370TypeRdo)
	ON_BN_CLICKED(IDC_DGN_IS3370_N29, OnConditionIS3370AnnexBRdo)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeDlg message handlers

BOOL CDgnConCodeDlg::OnInitDialog()
{
	MInitCombo initCombo;

	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	m_DesignCode.ResetContent();
	CStringArray ConCodeName;
	CDBLib::GetConCodeNameList(ConCodeName, &m_aCodeEnable);
	// Change, Jaeoh('07.12.26)
	int iCodeSize = ConCodeName.GetSize();
#if defined(_CIVIL)
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	for (int i = 0; i < ConCodeName.GetSize(); i++)
	{
		//(2009.01.15) Add by Unsang :: MNET.3908참고, US Units 구분
		CString strConCodeName = CDBLib::ViewConvertCodeName(ConCodeName[i]);
		if (strConCodeName != _T("GB50068-2018"))
		{
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(strConCodeName);
#endif
			m_DesignCode.AddString(strConCodeName);
		}
	}

	// civil 리본메뉴로 Design, MODS가 합쳐지면서 code list가 국가에 관련없이 다 나오게 함. 20120118 KIMJM
	//   if(rPref.DgnCode.ConcCode==_T("KSCE-USD05") || rPref.DgnCode.ConcCode==_T("KSCE-RAIL-USD04") || rPref.DgnCode.ConcCode==_T("KCI-USD07"))
	//   {
	//     for(int i=0; i<iCodeSize; i++)
	//     {
	//       //if(ConCodeName[i]==_T("KSCE-USD05") || ConCodeName[i]==_T("KSCE-RAIL-USD04") || ConCodeName[i]==_T("KCI-USD07"))
	//       //  m_DesignCode.AddString(ConCodeName[i]);
	// 
	//       //(2009.01.15) Add by Unsang :: MNET.3908참고, US Units 구분
	//       strConCodeName = CDBLib::ViewConvertCodeName(ConCodeName[i]);
	//       if(strConCodeName==_T("KSCE-USD05") || strConCodeName==_T("KSCE-RAIL-USD04") || strConCodeName==_T("KCI-USD07"))
	//         m_DesignCode.AddString(strConCodeName);
	//     }
	//   }
	//   else
	//   {
	//     for(int i=0; i<iCodeSize; i++)
	//     {
	//       //if(!(ConCodeName[i]==_T("KSCE-USD05") || ConCodeName[i]==_T("KSCE-RAIL-USD04") || ConCodeName[i]==_T("KCI-USD07")))
	//       //  m_DesignCode.AddString(ConCodeName[i]);
	// 
	//       //(2009.01.15) Add by Unsang :: MNET.3908참고, US Units 구분
	//       strConCodeName = CDBLib::ViewConvertCodeName(ConCodeName[i]);
	//       if(!(strConCodeName==_T("KSCE-USD05") || strConCodeName==_T("KSCE-RAIL-USD04") || strConCodeName==_T("KCI-USD07")))
	//         m_DesignCode.AddString(strConCodeName);
	//     }
	//   } 
#elif defined(_MGEN)
	for (int i = 0; i < ConCodeName.GetSize(); i++) m_DesignCode.AddString(ConCodeName[i]);
	//SetComboCdAndIe();
#endif
	CDlgUtil::CobxAdjustListBoxWidth(m_DesignCode);

	m_edtStrutAngle.SetUnitType(D_UNITSYS_NONE);
	m_edtEffectivePhi.SetUnitType(D_UNITSYS_NONE);
	m_dedtFOS.SetUnitType(D_UNITSYS_NONE);
	m_dedtSubRex.SetUnitType(D_UNITSYS_BASE_DENSITY);
	//  m_edtBehaviorFactor.SetUnitType(D_UNITSYS_NONE);
	m_edtSlendLimitA.SetUnitType(D_UNITSYS_NONE);
	m_edtSlendLimitB.SetUnitType(D_UNITSYS_NONE);
	m_edtSlendLimitC.SetUnitType(D_UNITSYS_NONE);
	m_edtmb7.SetEditUnit(D_UNITSYS_NONE);
	m_edtmb8.SetEditUnit(D_UNITSYS_NONE);
	m_edtmb9.SetEditUnit(D_UNITSYS_NONE);

	m_dedtESS.SetEditUnit(D_UNITSYS_NONE);
	m_dedtETT.SetEditUnit(D_UNITSYS_NONE);
	m_dedtT1.SetEditUnit(D_UNITSYS_NONE);
	//(2009.06.12) Add by Unsang :: Civil/Gen의 National Annex구별. 
	// Civil// 0:Recommened 1:UK 2:IT
	// GEN  // 0:IT 1:Recommened

	m_ComboNationAnnex.ResetContent();
	auto vString = CNationalAnnexTool::GetRcNationalAnnex();
	for (int i = 0; i < vString.size(); ++i)
	{
		m_ComboNationAnnex.SetItemData(m_ComboNationAnnex.AddString(vString[i]),
		                               CNationalAnnexTool::ConvertNationalAnnex(vString[i]));
	}
	m_ComboNationAnnex.SetCurSel(0);

	m_cmbBridgeType.ResetContent();
	m_cmbBridgeType.AddString(_LS(IDS_DGN_BRIDGE_TYPE_ROADWAY));
	m_cmbBridgeType.AddString(_LS(IDS_DGN_BRIDGE_TYPE_RAILROAD));
	m_cmbBridgeType.SetCurSel(0);
	InitAnnexCtrl(m_ComboNationAnnex.GetCurSel());

	m_cmbIS3370AnnexB2mm.ResetContent();
	m_cmbIS3370AnnexB2mm.AddString(_LS(IDS_DGN_IS3370_ANNEXB_02));
	m_cmbIS3370AnnexB2mm.AddString(_LS(IDS_DGN_IS3370_ANNEXB_01));
	m_cmbIS3370AnnexB2mm.SetCurSel(1);

	m_iColmMethod = 0;
	m_iSafeLevel = 0;
	m_iEqGrade = 0;
	m_iPmCurve = 0;
	m_iEqCategory = 0;
	m_iSlabType = 0;
	m_iAlphaType1 = 0;
	m_iFrameType = 0;
	m_iDuctility = 0;
	m_GroupKey = 0;
	m_dStorColmWeakBeamFactor = 1.3;
	m_nExposureFactor = 0;
	m_nPouringMethod = 0;
	m_iIs13920Sel = 0;
	m_iIS3370Sel = 0;
	m_bIS3370AnnexA = TRUE; //If TRUE Annex A ; else Annex B	
	//(Tel. 4022) MNET:XXXX-BSCHOI-20121015 #1 ApplyNTC
	m_cmbNTCType.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbNTCType, _LS(IDS_DGN_NTC2008), RCS_NTC08);
	CDlgUtil::CobxAddItem(m_cmbNTCType, _LS(IDS_DGN_NTC2012), RCS_NTC12);
	CDlgUtil::CobxAddItem(m_cmbNTCType, _LS(IDS_DGN_NTC2018), RCS_NTC18);
	int nMovingType = CProduct::GetMovingType();
	if (nMovingType == D_PRODUCT_MOVING_US)
	{
		int EnCase = RCS_NTC18;
		if (CProduct::IsIndia() || CProduct::IsTaiwan()) EnCase = RCS_NTC12;
		CDlgUtil::CobxSetCurSelItemData(m_cmbNTCType, EnCase);
	}
	else CDlgUtil::CobxSetCurSelItemData(m_cmbNTCType, RCS_NTC12);

	if (m_pDoc->m_pAttrCtrl->ExistDcon()) Initial_ExistData();
	else Initial_Data();

	EnableDisableControls();
	EnableWindowBySpecialWall();

	GetDlgItem(IDC_DGN_CTRLDATA_ADV_BTN)->ShowWindow(SW_HIDE);

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeDlg::SetComboAnnexAS()
{
	m_DesignAnnex.ResetContent();
	auto vString = CNationalAnnexTool::GetRcASAmendment();
	for (int i = 0; i < vString.size(); ++i)
	{
		CDlgUtil::CobxAddItem(m_DesignAnnex, vString[i], CNationalAnnexTool::ConvertRcASAmendment(vString[i]));
	}
	m_DesignAnnex.SetCurSel(0);
}

void CDgnConCodeDlg::SetComboAnnexAASHTO()
{
	m_DesignAnnex.ResetContent();
	auto vString = CNationalAnnexTool::GetRcAASHTOAmendment();
	for (int i = 0; i < vString.size(); ++i)
	{
		CDlgUtil::CobxAddItem(m_DesignAnnex, vString[i], CNationalAnnexTool::ConvertAASHTOAmendment(vString[i]));
	}
	CDlgUtil::CobxSetCurSelItemData(m_DesignAnnex, Amend_Recommended);
}

void CDgnConCodeDlg::SetComboCdAndIe()
{
	T_DCON_D DconD;
	BOOL bExistDcon = m_pDoc->m_pAttrCtrl->GetDcon(DconD);

	m_CmbCd.ResetContent();
	m_CmbCd.AddString(_T("1.25"));
	m_CmbCd.AddString(_T("1.50"));
	m_CmbCd.AddString(_T("2.00"));
	m_CmbCd.AddString(_T("2.50"));
	m_CmbCd.AddString(_T("3.00"));
	m_CmbCd.AddString(_T("3.25"));
	m_CmbCd.AddString(_T("4.00"));
	m_CmbCd.AddString(_T("4.50"));
	m_CmbCd.AddString(_T("5.00"));
	m_CmbCd.AddString(_T("5.50"));
	m_CmbCd.AddString(_T("6.00"));
	m_CmbCd.AddString(_T("6.50"));

	// Modify by GAY. MQC:5022. (`12.07.30). Cd, Ie 변경 후, Dlg Open시에 초기값 설정되는 문제 수정.
	if (!bExistDcon) m_CmbCd.SetCurSel(7);
	else
	{
		CString strText = _T("");
		strText.Format(_T("%.2f"), m_dCd);
		m_CmbCd.SetWindowText(strText);
	}

	m_CmbIe.ResetContent();
	m_CmbIe.AddString(_T("1.00"));
	m_CmbIe.AddString(_T("1.20"));
	m_CmbIe.AddString(_T("1.50"));

	// Modify by GAY. MQC:5022. (`12.07.30). Cd, Ie 변경 후, Dlg Open시에 초기값 설정되는 문제 수정.
	if (!bExistDcon) m_CmbIe.SetCurSel(1);
	else
	{
		CString strText = _T("");
		strText.Format(_T("%.2f"), m_dIe);
		m_CmbIe.SetWindowText(strText);
	}
}

void CDgnConCodeDlg::SetComboCdAngle_ACI318()
{
	T_DCON_D DconD;
	BOOL bExistDcon = m_pDoc->m_pAttrCtrl->GetDcon(DconD);

	m_CmbCd.ResetContent();
	m_CmbCd.AddString(_T("1.00"));
	m_CmbCd.AddString(_T("1.25"));
	m_CmbCd.AddString(_T("1.50"));
	m_CmbCd.AddString(_T("1.75"));
	m_CmbCd.AddString(_T("2.00"));
	m_CmbCd.AddString(_T("2.25"));
	m_CmbCd.AddString(_T("2.50"));
	m_CmbCd.AddString(_T("3.00"));
	m_CmbCd.AddString(_T("3.25"));
	m_CmbCd.AddString(_T("3.50"));
	m_CmbCd.AddString(_T("4.00"));
	m_CmbCd.AddString(_T("4.50"));
	m_CmbCd.AddString(_T("5.00"));
	m_CmbCd.AddString(_T("5.50"));
	m_CmbCd.AddString(_T("6.00"));
	m_CmbCd.AddString(_T("6.50"));

	// Modify by GAY. MQC:5022. (`12.07.30). Cd, Ie 변경 후, Dlg Open시에 초기값 설정되는 문제 수정.
	if (!bExistDcon) m_CmbCd.SetCurSel(11);
	else
	{
		CString strText = _T("");
		strText.Format(_T("%.2f"), m_dCd);
		m_CmbCd.SetWindowText(strText);
	}

	m_CmbIe.ResetContent();
	m_CmbIe.AddString(_T("1.00"));
	m_CmbIe.AddString(_T("1.25"));
	m_CmbIe.AddString(_T("1.50"));

	// Modify by GAY. MQC:5022. (`12.07.30). Cd, Ie 변경 후, Dlg Open시에 초기값 설정되는 문제 수정.
	if (!bExistDcon) m_CmbIe.SetCurSel(1);
	else
	{
		CString strText = _T("");
		strText.Format(_T("%.2f"), m_dIe);
		m_CmbIe.SetWindowText(strText);
	}
}

void CDgnConCodeDlg::Initial_Data()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	//(2009.01.15) Add by Unsang :: MNET.3908참고, US Units 구분
	CString strConCodeName = CDBLib::ViewConvertCodeName(rPref.DgnCode.ConcCode);

#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(strConCodeName);
#endif

	int Index = m_DesignCode.FindStringExact(-1, strConCodeName);
	if (Index != -1) m_DesignCode.SetCurSel(Index);
	m_nOldCodeIndex = Index;

	T_DCON_D rData;
	rData.Initialize();

	const CString& strCode = rPref.DgnCode.ConcCode;
	m_ComboNationAnnex.SetCurSel(rPref.DgnCode.nConcNationalAnnex);

	if (rData.DesignCode == CONCODE_AASHTO_LRFD17) SetComboAnnexAASHTO();
	else if (rData.DesignCode == CONCODE_AS5100_5_17) SetComboAnnexAS();
	CDlgUtil::CobxSetCurSelItemData(m_DesignAnnex, rData.nNationalAnnex);

	int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());
	int nNationalAnnex = m_ComboNationAnnex.GetCurSel();
	if (strCode == CONCODE_EC2_04 && nNationalAnnex == 0 && nNTCType == RCS_NTC18)
	{
		m_bNTCFlag = 1;
		m_iDuctility = 1;
		m_bSpecialEQ = TRUE;
	}
	else m_bSpecialEQ = FALSE;
	// Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (GB50010-02).
	m_pDgnDataCtrl->Get_DgnConChinaDef(rPref.DgnCode.ConcCode, m_iSafeLevel, m_iEqGrade);

	CString strWinCode = _T("");
	m_DesignCode.GetWindowText(strWinCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strWinCode);
#endif
	strWinCode = CDBLib::GetConvertCodeName(strWinCode);

	auto L_IsInitFrameType1 = [strWinCode]() -> bool
	{
		if (strWinCode == CONCODE_KCI_USD07) { return true; }
		if (strWinCode == CONCODE_KCI_USD12) { return true; }
		if (strWinCode == CONCODE_KDS_41_30_2018) { return true; }
		if (strWinCode == CONCODE_KDS_41_20_2022) { return true; }
		if (strWinCode == CONCODE_ACI318_08) { return true; }
		if (strWinCode == CONCODE_ACI318_11) { return true; }
		if (strWinCode == CONCODE_ACI318_14) { return true; }
		if (strWinCode == CONCODE_ACI318M_14) { return true; }
		if (strWinCode == CONCODE_ACI318_19) { return true; }
		if (strWinCode == CONCODE_ACI318M_19) { return true; }
		if (strWinCode == CONCODE_ACI318_25) { return true; }
		if (strWinCode == CONCODE_ACI318M_25) { return true; }
		if (strWinCode == CONCODE_NSCP_2015) { return true; }
		if (strWinCode == CONCODE_NSR_10) { return true; }
		if (strWinCode == CONCODE_NTC_DCEC_2017) { return true; }
		if (strWinCode == CONCODE_NTC_DCEC_2023) { return true; }

		return false;
	};

	if (L_IsInitFrameType1()) { m_iFrameType = 1; }

	m_edtStrutAngle.SetEditUnit(rData.dStrutAngle);
	m_edtEffectivePhi.SetEditUnit(rData.dPhi_ef);
	//   m_chkBehaviorFactor.SetCheck(rData.bBehaviorFact);
	//   m_edtBehaviorFactor.SetEditUnit(rData.dBehaviorFactor_q);
	m_edtSlendLimitA.SetEditUnit(rData.dA);
	m_edtSlendLimitB.SetEditUnit(rData.dB);
	m_edtSlendLimitC.SetEditUnit(rData.dC);
	m_chkAutoCalcA.SetCheck(rData.bAutoA);
	m_chkAutoCalcC.SetCheck(rData.bAutoC);

	m_bSubBeam = rData.bNonSeisSubBeam;
	m_bCantilever = rData.bNonSeisCantilever;
	m_bUnderBmColm = rData.bNonSeisUnderGround;

	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, rData.nSCWBDgnMethod);
	if (strWinCode == CONCODE_TWN_USD100 || strWinCode == CONCODE_TWN_USD112) // PMS:4385-PARKHJ-20111025 : 대만대리점 요청사항.
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, 1); // 1: Norminal Strength
	}

	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoBeamDgn, rData.nBeamDgn);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoColumnDgn, rData.nColumnDgn);

	m_DconEC8Data.bBehaviorFact = TRUE; // PMS:4490-DANAKIM-20120619

	m_ChkUseVcWall.SetCheck(rData.bUseVcWall); // PMS:DNKIM-20121030
	m_ChkUseVcColm.SetCheck(rData.bUseVcColm);
	m_ChkUseVcBeam.SetCheck(rData.bUseVcBeam);
	// 	if(strWinCode == Eurocode2_04)
	// 		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, 0)

	//   CArray<UINT, UINT> aAllStructGroupK;
	//   m_pDoc->m_pAttrCtrl->GetGrupKeyList(aAllStructGroupK);
	//   m_aAllGroupK.RemoveAll();
	//   m_arAllGroup.RemoveAll();
	//   for(int i=0; i<aAllStructGroupK.GetSize(); i++)
	//   {
	//     T_GRUP_K GrupK; GrupK = aAllStructGroupK[i];
	//     T_GRUP_D GrupD;
	//     if(!m_pDoc->m_pAttrCtrl->GetGrup(GrupK, GrupD)) {ASSERT(0); continue;}
	//     m_aAllGroupK.Add(GrupK);
	//     m_arAllGroup.SetAt(GrupK, GrupD);
	//   }
	//   m_aExcludeGroupK.RemoveAll();
	//   m_aNotSFSRGroupK.RemoveAll();

	// Exposure Condition
	m_nExposure = rData.nExposure;

	// Bridge Type
	m_cmbBridgeType.SetCurSel(rData.nBridgeType);
	m_CmbPL.SetCurSel(static_cast<int>(rData.enPL));
	m_CmbSL.SetCurSel(static_cast<int>(rData.enSL));
	m_CmbSType.SetCurSel(static_cast<int>(rData.enSType));
	m_EdtDg.SetEditUnit(rData.dAggregateSize);
	m_EdtKc.SetEditUnit(rData.dkc);

	m_bmb7mb8 = rData.bmb7mb8;
	m_nmb7mb8Type = rData.nmb7mb8Type;
	m_edtmb7.SetEditUnit(rData.dmb7);
	m_edtmb8.SetEditUnit(rData.dmb8);
	m_bmb9 = rData.bmb9;
	m_nmb9Type = rData.nmb9Type;
	m_edtmb9.SetEditUnit(rData.dmb9);
	m_bChkPM = rData.bChkPM;

	m_ChkLimitShearStrgConc.SetCheck(rData.bLimitShearStrengthConc);
	if (strCode == CONCODE_IS456_2000) //Pinakin Added for IS456 MAT
	{
		m_dedtFOS.SetEditUnit(rData.dFOS);
		//m_edtStrutAngle.SetEditUnit(rData.dStrutAngle);
		//m_dedtSubRex=rData.dSubRex;
		m_dedtSubRex.SetEditUnit(rData.dSubRex);
		m_ctrlUnit.SetUnitType(D_UNITSYS_BASE_DENSITY);
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoIS1392Sel, rData.iSubCode);
		//IS3370
		m_chkIS3370.SetCheck(rData.bIS3370Sel);
		m_iIS3370Sel = rData.bIS3370AnnexA == TRUE ? 0 : 1;
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlIS3370_RadAB, m_iIS3370Sel);
		m_dedtESS.SetEditUnit(rData.dIS3370AnnexA_ecs);
		m_dedtETT.SetEditUnit(rData.dIS3370AnnexA_etc);
		m_dedtT1.SetEditUnit(rData.dIS3370AnnexA_t1);
		m_cmbIS3370AnnexB2mm.SetCurSel(rData.iIS3370AnnexB_cw);
	}

	OnSetGrupCombo();
	EnableWindowBySpecialWall();
	// Change by ZINU.('02.7.8). For AIJ-WSD99, GB50010-02.
	// Change by ParkBong.(06.05.18) 
	SelchangeDgnCtrldataDesigncode(TRUE);
	UpdateData(FALSE);
	OnDuctilityBtn();
	OnChkApplyNTC();
	//  OnChkBehaviorFactor();
	OnChkAutoCalcA();
	OnChkAutoCalcC();
	OnBCJoint();
	SetNTC2008EnableControl();
}

void CDgnConCodeDlg::Initial_ExistData()
{
	T_DCON_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcon(rData);

	// civil 리본메뉴로 Design, MODS가 합쳐지면서 code list가 국가에 관련없이 다 나오게 함. 20120118 KIMJM
	//   // Add, Jaeoh. [12/29/2008]
	// #if defined(_CIVIL)
	// 	T_PREFERENCE rPref;
	// 	rPref.Initialize();
	// 	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	//   //(2009.01.15) Add by Unsang :: MNET.3908참고, US Units 구분
	//   rPref.DgnCode.ConcCode = CDBLib::ViewConvertCodeName(rPref.DgnCode.ConcCode);
	//   
	//   if(CDBLib::IsRCCode4CivilKR(rData.DesignCode))
	//   {
	//     if(!CDBLib::IsRCCode4CivilKR(rPref.DgnCode.ConcCode))
	//     {
	//       m_pDoc->m_pDataCtrl->DelDcon();
	//       rData.Initialize();
	//       rData.DesignCode = rPref.DgnCode.ConcCode;
	//     }
	//   }
	//   else
	//   {
	//     if(CDBLib::IsRCCode4CivilKR(rPref.DgnCode.ConcCode))
	//     {
	//       m_pDoc->m_pDataCtrl->DelDcon();
	//       rData.Initialize();
	//       rData.DesignCode = rPref.DgnCode.ConcCode;
	//     }
	//   }
	// #endif

	m_bSpecialEQ = rData.bSpecialSeismic;
	// Change by ZINU.('02.8.27).
	const int iStatus = GetDataStatus(rData.DesignCode);
	m_iColmMethod = (iStatus == 1 || iStatus == 9 ? rData.nClass : 0);
	m_iSafeLevel = (iStatus == 2 ? rData.nClass : 0);
	m_iEqGrade = (iStatus == 3 ? rData.nClass : 0);
	m_iPmCurve = (iStatus == 4 ? rData.nClass : 0);
	m_iEqCategory = (iStatus == 6 || iStatus == 7 || iStatus == 8 || iStatus == 19 ? rData.nClass : 0);
	m_iSlabType = rData.iSlabType;
	m_iFrameType = (iStatus == 10 || iStatus == 18 || iStatus == 20 ? rData.nClass : 0);
	m_iDuctility = (iStatus == 17 ? rData.nClass : 0);

	// Coded by Seungjun MNet:No.2591 ('20061207)	For SCWB Design.
	/*
	if(iStatus==4 || iStatus==13 || iStatus==15)  m_iPmCurve = rData.nClass;
	else if(iStatus==10)                          m_iPmCurve = rData.nClass%10;
	else                                          m_iPmCurve = 0;
*/
	int Index = 0;
	//(2009.01.15) Add by Unsang :: MNET.3908참고, US Units 구분
	CString strDesignCode = CDBLib::ViewConvertCodeName(rData.DesignCode);

#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(strDesignCode);
#endif

	if (strDesignCode != _T("")) Index = m_DesignCode.FindStringExact(-1, strDesignCode);
	// Add, Jaeoh (06.12.06)
	/*
	#if defined(_CIVIL)
		#if defined(_ORG)
			if(Index==-1)
				Index=0;
		#endif
	#endif
		*/
	m_DesignCode.SetCurSel(Index);
	m_nOldCodeIndex = Index;
	m_ComboNationAnnex.SetCurSel(
		m_ComboNationAnnex.FindStringExact(-1, CNationalAnnexTool::ConvertNationalAnnex(rData.nNationalAnnex)));

	if (rData.DesignCode == CONCODE_AASHTO_LRFD17) SetComboAnnexAASHTO();
	else if (rData.DesignCode == CONCODE_AS5100_5_17) SetComboAnnexAS();
	CDlgUtil::CobxSetCurSelItemData(m_DesignAnnex, rData.nNationalAnnex);

	m_dStorColmWeakBeamFactor = rData.dStorColmWeakBeamFactor;
	m_dGamma_rdForBeam = rData.dGamma_rdForBeam;
	m_dGamma_rdForColm = rData.dGamma_rdForColm;
	m_dGamma_rdForWall = rData.dGamma_rdForWall;
	m_dGamma_rdForJoint = rData.dGamma_rdForJoint;
	m_GroupKey = rData.SecondaryGroupKey;

	m_bTorsionDesign = rData.bTorsionDesign;
	m_bBCJointDgn = rData.bBCJointDgn;
	m_bNTCFlag = rData.bNTCFlag;
	m_dUf = rData.dUf;

	m_edtStrutAngle.SetEditUnit(rData.dStrutAngle);
	m_edtEffectivePhi.SetEditUnit(rData.dPhi_ef);
	//   m_chkBehaviorFactor.SetCheck(rData.bBehaviorFact);
	//   m_edtBehaviorFactor.SetEditUnit(rData.dBehaviorFactor_q);

	m_edtSlendLimitA.SetEditUnit(rData.dA);
	m_edtSlendLimitB.SetEditUnit(rData.dB);
	m_edtSlendLimitC.SetEditUnit(rData.dC);
	m_chkAutoCalcA.SetCheck(rData.bAutoA);
	m_chkAutoCalcC.SetCheck(rData.bAutoC);

	OnSetGrupCombo();

	// Set KCI-USD07
	m_dCd = rData.dCd >= 1.0 ? rData.dCd : 1.0;
	m_dIe = rData.dIe >= 1.0 ? rData.dIe : 1.0;
	if (rData.DesignCode == CONCODE_NSR_10)
	{
		m_nSpecialWallMtd = rData.nSpecialWall;
		m_DconEC8Data.nSpecialWall = m_nSpecialWallMtd;
	}
	else
	{
		m_bSpecialWall = rData.nSpecialWall > 0 ? TRUE : FALSE;
		m_DconEC8Data.nSpecialWall = m_bSpecialWall;
	}

	m_nBndrElemMethod = (rData.iBndrElemMethod == 0 || rData.iBndrElemMethod == 1) ? rData.iBndrElemMethod : 0;
	m_DconEC8Data.iBndrElemMethod = m_nBndrElemMethod;
	m_DconEC8Data.dCd = m_dCd;
	m_DconEC8Data.dIe = m_dIe;

	m_nExposureFactor = rData.iExposureFactor;
	m_nPouringMethod = rData.iPouringMethod;

	m_bSubBeam = rData.bNonSeisSubBeam;
	m_bCantilever = rData.bNonSeisCantilever;
	m_bUnderBmColm = rData.bNonSeisUnderGround;

	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, rData.nSCWBDgnMethod);
	//   if (rData.DesignCode==_T("TWN-USD100") || rData.DesignCode==_T("TWN-USD112")) // PMS:4385-PARKHJ-20111025 : 대만대리점 요청사항.
	//     CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, 1);  // 1: Norminal Strength

	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoBeamDgn, rData.nBeamDgn);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoColumnDgn, rData.nColumnDgn);

	EnableWindowBySpecialWall();

	CString strTemp = _T("");
	strTemp.Format(_T("%.2f"), m_dCd);
	m_CmbCd.SetWindowText(strTemp);
	strTemp.Format(_T("%.2f"), m_dIe);
	m_CmbIe.SetWindowText(strTemp);

	// Set EC8 Data
	m_DconEC8Data.iFrameType = rData.iFrameType;
	m_DconEC8Data.dAua1 = rData.dAua1;
	m_DconEC8Data.SplcK = rData.SplcK;
	m_DconEC8Data.dSoilFactor = rData.dSoilFactor;
	m_DconEC8Data.dT[0] = rData.dT[0];
	m_DconEC8Data.dT[1] = rData.dT[1];
	m_DconEC8Data.dT[2] = rData.dT[2];
	m_DconEC8Data.dAgR = rData.dAgR;
	m_DconEC8Data.dI = rData.dI;
	m_DconEC8Data.dDampingRat = rData.dDampingRat;
	m_DconEC8Data.bBehaviorFact = rData.bBehaviorFact;
	m_DconEC8Data.dBehaviorFactor_q = rData.dBehaviorFactor_q;
	//(Tel. 4022) MNET:XXXX-BSCHOI-20121023 #1 
	int nNTCType = rData.nNTCType;
	if (nNTCType <= 2)
	{
		CDlgUtil::CobxSetCurSelItemData(m_cmbNTCType, nNTCType);
		//m_cmbNTCType.SetCurSel(nNTCType);
	}
	else
	{
		ASSERT(0);
		CDlgUtil::CobxSetCurSelItemData(m_cmbNTCType, RCS_NTC08);
		//m_cmbNTCType.SetCurSel(0);
	}

	// PMS:DNKIM-20121030
	m_ChkUseVcWall.SetCheck(rData.bUseVcWall);
	m_ChkUseVcColm.SetCheck(rData.bUseVcColm);
	m_ChkUseVcBeam.SetCheck(rData.bUseVcBeam);

	// Exposure Condition
	m_nExposure = rData.nExposure;

	// Bridge Type
	m_cmbBridgeType.SetCurSel(rData.nBridgeType);
	m_CmbPL.SetCurSel(static_cast<int>(rData.enPL));
	m_CmbSL.SetCurSel(static_cast<int>(rData.enSL));
	m_CmbSType.SetCurSel(static_cast<int>(rData.enSType));
	m_EdtDg.SetEditUnit(rData.dAggregateSize);
	m_EdtKc.SetEditUnit(rData.dkc);

	m_bmb7mb8 = rData.bmb7mb8;
	m_nmb7mb8Type = rData.nmb7mb8Type;
	m_edtmb7.SetEditUnit(rData.dmb7);
	m_edtmb8.SetEditUnit(rData.dmb8);
	m_bmb9 = rData.bmb9;
	m_nmb9Type = rData.nmb9Type;
	m_edtmb9.SetEditUnit(rData.dmb9);
	m_bChkPM = rData.bChkPM;

	m_ChkLimitShearStrgConc.SetCheck(rData.bLimitShearStrengthConc);


	//   CArray<UINT, UINT> aAllStructGroupK;
	//   m_pDoc->m_pAttrCtrl->GetGrupKeyList(aAllStructGroupK);
	//   m_aAllGroupK.RemoveAll();
	//   m_arAllGroup.RemoveAll();
	//   for(int i=0; i<aAllStructGroupK.GetSize(); i++)
	//   {
	//     T_GRUP_K GrupK; GrupK = aAllStructGroupK[i];
	//     T_GRUP_D GrupD;
	//     if(!m_pDoc->m_pAttrCtrl->GetGrup(GrupK, GrupD)) {ASSERT(0); continue;}
	//     m_aAllGroupK.Add(GrupK);
	//     m_arAllGroup.SetAt(GrupK, GrupD);
	//   }

	//   m_aExcludeGroupK.RemoveAll();
	//   m_aNotSFSRGroupK.RemoveAll();
	//   for(i=0; i<rData.aExcludeGroup.GetSize(); i++)
	//   {
	//     T_GRUP_K GrupK; GrupK = rData.aExcludeGroup[i];
	//     T_GRUP_D GrupD;
	//     if(m_arAllGroup.Lookup(GrupK, GrupD)) m_aExcludeGroupK.Add(GrupK);
	//   }
	//   for(i=0; i<rData.aNotSFSRGroup.GetSize(); i++)
	//   {
	//     T_GRUP_K GrupK; GrupK = rData.aNotSFSRGroup[i];
	//     T_GRUP_D GrupD;
	//     if(m_arAllGroup.Lookup(GrupK, GrupD)) m_aNotSFSRGroupK.Add(GrupK);
	//   }

	UpdateData(FALSE);
	// Change by ZINU.('02.7.8). For AIJ-WSD99, GB50010-02.
	// Change by ParkBong.(06.05.18)
	UpdateData(TRUE);
	CString strCode = _T("");
	m_DesignCode.GetWindowText(strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	strCode = CDBLib::GetConvertCodeName(strCode);
	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	// PMS:xxxx-Seungjun-20100422 저장된 데이터가 있는 경우 초기값 세팅하지 않음.
	m_bExistData = TRUE; //gamma_rd값이 저장이 안되는 일이 발생하여 위치 변경.
	SelchangeDgnCtrldataDesigncode(FALSE);
	UpdateData(FALSE);
	if (strCode == CONCODE_IS456_2000)
	{
		//Pinakin Added for IS456 MAT check the unit conversion
		//m_dedtFOS=3.0;
		// m_dedtSubRex  = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, 15000.0);*/
		m_dedtFOS.SetEditUnit(rData.dFOS);
		m_dedtSubRex.SetEditUnit(rData.dSubRex);
		m_ctrlUnit.SetUnitType(D_UNITSYS_BASE_DENSITY);

		m_iIs13920Sel = rData.iSubCode;
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoIS1392Sel, m_iIs13920Sel);

		m_chkIS3370.SetCheck(rData.bIS3370Sel);
		m_iIS3370Sel = rData.bIS3370AnnexA == FALSE ? 1 : 0;
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlIS3370_RadAB, m_iIS3370Sel);
		m_dedtESS.SetEditUnit(rData.dIS3370AnnexA_ecs);
		m_dedtETT.SetEditUnit(rData.dIS3370AnnexA_etc);
		m_dedtT1.SetEditUnit(rData.dIS3370AnnexA_t1);
		m_cmbIS3370AnnexB2mm.SetCurSel(rData.iIS3370AnnexB_cw);
	}

	OnDuctilityBtn();
	OnChkApplyNTC();
	//  OnChkBehaviorFactor();
	OnChkAutoCalcA();
	OnChkAutoCalcC();
	//(Tel. 4022) MNET:XXXX-BSCHOI-20121024 #1
	SetNTC2008EnableControl();
	m_chkApplyBCJDesign.SetCheck(rData.bBCJointDgn);
	OnBCJoint();
	m_bExistData = FALSE;
}

void CDgnConCodeDlg::OnOK()
{
	UpdateData(TRUE);
	T_DCON_D rData;
	rData.Initialize();

	// Exist Concrete Control Data at DB.
	if (m_pDoc->m_pAttrCtrl->ExistDcon()) m_pDoc->m_pAttrCtrl->GetDcon(rData);

	// Get Design Code.
	int nNA = m_ComboNationAnnex.GetItemData(m_ComboNationAnnex.GetCurSel());
	if (rData.nNationalAnnex != nNA)
	{
		rData.Initialize();
	}

	if (rData.DesignCode == CONCODE_AASHTO_LRFD17 || rData.DesignCode == CONCODE_AS5100_5_17)
	{
		rData.nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAnnex, m_DesignAnnex.GetCurSel());
	}
	else
	{
		rData.nNationalAnnex = nNA;
	}

	int Index = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(Index, rData.DesignCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(rData.DesignCode);
#endif
	//(2009.01.15) Add by Unsang :: MNET.3908참고, US Units 구분
	rData.DesignCode = CDBLib::GetConvertCodeName(rData.DesignCode);

	// Get Special Seismic and Uni Axis.
	rData.bSpecialSeismic = m_bSpecialEQ;
	// Change by ZINU.('02.8.27).
	int iStatus = GetDataStatus(rData.DesignCode);
	if (iStatus == 1 || iStatus == 9) rData.nClass = m_iColmMethod;
	else if (iStatus == 2) rData.nClass = m_iSafeLevel;
	else if (iStatus == 3) rData.nClass = m_iEqGrade;
	else if (iStatus == 4) rData.nClass = m_iPmCurve;
	else if (iStatus == 6) rData.nClass = m_iEqCategory;
	else if (iStatus == 7) rData.nClass = m_iEqCategory;
	else if (iStatus == 8) rData.nClass = m_iEqCategory;
	else if (iStatus == 10) rData.nClass = m_iFrameType;
	else if (iStatus == 17) rData.nClass = m_iDuctility; //Eurocode2:04
	else if (iStatus == 18) rData.nClass = m_iFrameType; //KCI-USD07
	else if (iStatus == 19) rData.nClass = m_iEqCategory;
	else if (iStatus == 20) rData.nClass = m_iFrameType; // Modify by GAY. PMS:4450. ('12.05.14). ACI318-08, 11.
		//else if(iStatus==13)          rData.nClass = m_iPmCurve;  // Coded by Seungjun MNet:No.2591 ('20061207)	For SCWB Design.
		//else if(iStatus==15)          rData.nClass = m_iPmCurve;  // Coded by Seungjun MNet:No.2591 ('20061207)	For SCWB Design.
	else rData.nClass = 0;

	rData.iSlabType = m_iSlabType;
	rData.dMrft = m_dMrft;
	rData.dTrft = m_dTrft;
	rData.iAlphaType = m_iAlphaType1;
	rData.dAlpha1 = m_dAlpha1;
	rData.dAlpha2 = m_dAlpha2;
	rData.dConShearStrengthRatio = m_dConShearStrengthRatio;

	rData.dStorColmWeakBeamFactor = m_dStorColmWeakBeamFactor;
	rData.dGamma_rdForBeam = m_dGamma_rdForBeam;
	rData.dGamma_rdForColm = m_dGamma_rdForColm;
	rData.dGamma_rdForWall = m_dGamma_rdForWall;
	rData.dGamma_rdForJoint = m_dGamma_rdForJoint;

	rData.bTorsionDesign = m_bTorsionDesign;
	rData.bBCJointDgn = m_bBCJointDgn;
	rData.bNTCFlag = m_bNTCFlag;
	rData.dUf = m_dUf;
	rData.dStrutAngle = m_edtStrutAngle.GetEditValue();
	rData.dPhi_ef = m_edtEffectivePhi.GetEditValue();
	//   rData.bBehaviorFact = m_chkBehaviorFactor.GetCheck();
	//   rData.dBehaviorFactor_q = m_edtBehaviorFactor.GetEditValue();

	rData.dA = m_edtSlendLimitA.GetEditValue();
	rData.dB = m_edtSlendLimitB.GetEditValue();
	rData.dC = m_edtSlendLimitC.GetEditValue();
	rData.bAutoA = m_chkAutoCalcA.GetCheck();
	rData.bAutoC = m_chkAutoCalcC.GetCheck();

	// Add by ZINU.('03.11.18). For Strength Reduction Factor by Code.
	for (int i = 0; i < 5; i++) rData.dPhi[i] = 0.0;

	// Save KCI-USD07
	CString strNewValue = _T("");
	m_CmbCd.GetWindowText(strNewValue);
	m_dCd = _ttof(strNewValue);
	m_CmbIe.GetWindowText(strNewValue);
	m_dIe = _ttof(strNewValue);

	if (rData.DesignCode == CONCODE_NSR_10)
	{
		rData.nSpecialWall = m_nSpecialWallMtd;
	}
	else
	{
		rData.nSpecialWall = m_bSpecialWall;
	}
	rData.iBndrElemMethod = m_nBndrElemMethod;
	rData.dCd = m_dCd;
	rData.dIe = m_dIe;

	// Save EC8 Data
	rData.iFrameType = m_DconEC8Data.iFrameType;
	rData.dAua1 = m_DconEC8Data.dAua1;
	rData.SplcK = m_DconEC8Data.SplcK;
	rData.dSoilFactor = m_DconEC8Data.dSoilFactor;
	rData.dT[0] = m_DconEC8Data.dT[0];
	rData.dT[1] = m_DconEC8Data.dT[1];
	rData.dT[2] = m_DconEC8Data.dT[2];
	rData.dAgR = m_DconEC8Data.dAgR;
	rData.dI = m_DconEC8Data.dI;
	rData.dDampingRat = m_DconEC8Data.dDampingRat;
	rData.bBehaviorFact = m_DconEC8Data.bBehaviorFact;
	rData.dBehaviorFactor_q = m_DconEC8Data.dBehaviorFactor_q;

	rData.iExposureFactor = m_nExposureFactor;
	rData.iPouringMethod = m_nPouringMethod;

	// MNET:4076-SHJUNG-20100604
	// Eurocode2:04은 원래 컨트롤 사용.
	// Secondary Group => aExcludeGroup에 채워준다.
	if (rData.DesignCode == CONCODE_EC2_04)
	{
		//     m_aExcludeGroupK.RemoveAll();
		//     m_aNotSFSRGroupK.RemoveAll();

		int ix = m_cmbGroup.GetCurSel();
		if (ix == CB_ERR) rData.SecondaryGroupKey = 0;
		else rData.SecondaryGroupKey = m_cmbGroup.GetItemData(ix);

		m_GroupKey = rData.SecondaryGroupKey;

		//(Tel. 4022) MNET:XXXX-BSCHOI-20121015 #1
		rData.nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());

		// PMS:DNKIM-20121030// PMS:DNKIM-20121030
		rData.bUseVcWall = m_ChkUseVcWall.GetCheck();
		rData.bUseVcColm = m_ChkUseVcColm.GetCheck();
		rData.bUseVcBeam = m_ChkUseVcBeam.GetCheck();

		if (m_ComboNationAnnex.GetCurSel() == 3) rData.bLimitShearStrengthConc = m_ChkLimitShearStrgConc.GetCheck();

		//     if(m_GroupKey !=0 )
		//     {      
		//       m_aExcludeGroupK.Add(m_GroupKey);
		//     }
	}

	// Exposure Condition
	rData.nExposure = m_nExposure;

	// Bridge Type
	rData.nBridgeType = m_cmbBridgeType.GetCurSel();
	rData.enPL = static_cast<dgn::def::enProtectionLevelEU>(m_CmbPL.GetCurSel());
	rData.enSL = static_cast<dgn::def::enServiceLifeEU>(m_CmbSL.GetCurSel());
	rData.enSType = static_cast<dgn::def::enStructTypeEU>(m_CmbSType.GetCurSel());
	rData.dAggregateSize = m_EdtDg.GetEditValue();
	rData.dkc = m_EdtKc.GetEditValue();

	rData.bmb7mb8 = m_bmb7mb8;
	rData.nmb7mb8Type = m_nmb7mb8Type;
	rData.dmb7 = m_edtmb7.GetEditValue();
	rData.dmb8 = m_edtmb8.GetEditValue();
	rData.bmb9 = m_bmb9;
	rData.nmb9Type = m_nmb9Type;
	rData.dmb9 = m_edtmb9.GetEditValue();
	rData.bChkPM = m_bChkPM;
	//   rData.aExcludeGroup.Copy(m_aExcludeGroupK);
	//   rData.aNotSFSRGroup.Copy(m_aNotSFSRGroupK);

	const CString& strCode = rData.DesignCode;
	if (strCode == CONCODE_KCI_USD07 || strCode == CONCODE_KCI_USD12 ||
		strCode == CONCODE_KDS_41_30_2018 || strCode == CONCODE_KDS_41_20_2022 ||
		strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 ||
		strCode == CONCODE_ACI318_14 || strCode == CONCODE_ACI318M_14 || strCode == CONCODE_NSR_10 ||
		strCode == CONCODE_NSCP_2015 || strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318M_19 ||
		strCode == CONCODE_ACI318_25 || strCode == CONCODE_ACI318M_25 ||
		strCode == CONCODE_NTC_DCEC_2017 || strCode == CONCODE_NTC_DCEC_2023)
	{
		rData.bNonSeisSubBeam = m_bSubBeam;
		rData.bNonSeisCantilever = m_bCantilever;
		rData.bNonSeisUnderGround = m_bUnderBmColm;
	}
	if (strCode == CONCODE_IS456_2000) //Pinakin Added for IS456 MAT
	{
		rData.dSubRex = m_dedtSubRex.GetEditValue();
		//	  rData.dFOS				=m_dedtFOS;
		rData.dFOS = m_dedtFOS.GetEditValue();
		rData.iSubCode = m_iIs13920Sel;

		rData.bIS3370Sel = m_chkIS3370.GetCheck();
		rData.bIS3370AnnexA = m_iIS3370Sel == 0 ? TRUE : FALSE;
		rData.dIS3370AnnexA_ecs = m_dedtESS.GetEditValue();
		rData.dIS3370AnnexA_etc = m_dedtETT.GetEditValue();
		rData.dIS3370AnnexA_t1 = m_dedtT1.GetEditValue();
		rData.iIS3370AnnexB_cw = m_cmbIS3370AnnexB2mm.GetCurSel();
	}
	if (strCode == CONCODE_IRC112_2011 || strCode == CONCODE_IRC112_2020) //Pinakin Added for IS456 MAT
	{
		rData.dAlphacc = 0.67;
		rData.bChkPM = m_bChkPM = TRUE;
	}
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSCWBMethod, rData.nSCWBDgnMethod);

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoBeamDgn, rData.nBeamDgn);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoColumnDgn, rData.nColumnDgn);

	// Save Concrete Control Data at DB.
	BOOL bCheck = m_pDoc->m_pDataCtrl->AddDcon(rData);
	/*
	if(bCheck)
	{
		CArray<T_GRUP_K, T_GRUP_K> aAddGropKList;   aAddGropKList.RemoveAll();
		CArray<T_GRUP_D, T_GRUP_D&> aAddGropDList;  aAddGropDList.RemoveAll();
		for(int i=0; i<m_aAllGroupK.GetSize(); i++)
		{
			T_GRUP_K GrupK = m_aAllGroupK[i];
			T_GRUP_D GrupD;
			T_GRUP_D GrupOldD;
			if(!m_arAllGroup.Lookup(GrupK, GrupD)) {ASSERT(0); continue;}
			if(m_pDoc->m_pAttrCtrl->GetGrup(GrupK, GrupOldD))
			{
				// 기존에 있던 데이터에 대하여 변경된 것은 GrupId 뿐이므로
				// GrupId가 변경되지 않았다면 Data를 수정할 필요가 없다.
				if(GrupOldD.nGroupId == GrupD.nGroupId) continue;
				aAddGropKList.Add(GrupK);
				aAddGropDList.Add(GrupD);
			}
			else
			{
				aAddGropKList.Add(GrupK);
				aAddGropDList.Add(GrupD);
			}
		}

		if(aAddGropKList.GetSize()>0)
		{
			// 이미 GrupId에 대해서 맞춰진 데이터들이므로 GrupId를 재할당하지 않도록 함수를 따로 만들었다.
			m_pDoc->m_pDataCtrl->AddGrupAndModifyGrupID(aAddGropKList, aAddGropDList);
		}
	}
	*/
	if (bCheck)
	{
		CDialogMove::OnOK();
	}
}

void CDgnConCodeDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnConCodeDlg::OnDgnRcctrlSpecial()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	m_nOldAnnexIdx = m_DesignAnnex.GetCurSel();
	OnSelchangeDgnCtrldataDesigncode();
	m_DesignAnnex.SetCurSel(m_nOldAnnexIdx);
	//SetNTC2008EnableControl();
}

void CDgnConCodeDlg::OnChangeAlphaType()
{
	UpdateData(TRUE);

	ChangeDlgItemInfo();

	UpdateData(FALSE);
}

void CDgnConCodeDlg::OnChangeFrameType()
{
	UpdateData(TRUE);

	CString strCode = _T("");
	m_DesignCode.GetWindowText(strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	strCode = CDBLib::GetConvertCodeName(strCode);
	m_iAlphaType1 = 0;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);

	//ChangeDlgItemInfo();
	SelchangeDgnCtrldataDesigncode(FALSE);

	UpdateData(FALSE);
}

void CDgnConCodeDlg::SelchangeDgnCtrldataDesigncode(BOOL bUpdateDef)
{
	CRect rRef;
	CRect rMove;
	CRect RectEC8Btn;
	GetDlgItem(IDC_DGN_EC8_BTN)->GetWindowRect(&RectEC8Btn);
	int nDlgRight = RectEC8Btn.right;

	CString strCode = _T("");
	m_DesignCode.GetWindowText(strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	//(2009.01.15) Add by Unsang :: MNET.3908참고, US Units 구분
	strCode = CDBLib::GetConvertCodeName(strCode);

#if defined(_MGEN)
	if (strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 ||
		strCode == CONCODE_ACI318_14 || strCode == CONCODE_ACI318M_14 || strCode == CONCODE_NSR_10 ||
		strCode == CONCODE_NSCP_2015 || strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318M_19 ||
		strCode == CONCODE_ACI318_25 || strCode == CONCODE_ACI318M_25 ||
		strCode == CONCODE_NTC_DCEC_2017 || strCode == CONCODE_NTC_DCEC_2023)
	{
		SetComboCdAngle_ACI318();
	}
	else
	{
		SetComboCdAndIe();
	}
#endif

	SetBndrElemClimitDesc(strCode);

	int iStatus = GetDataStatus(strCode);
	//Initialize Parameter by Changing Code
	T_DCON_D rData;
	rData.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistDcon()) m_pDoc->m_pAttrCtrl->GetDcon(rData);
	//(2009.01.15) Add by Unsang :: MNET.3908참고, US Units 구분
	rData.DesignCode = CDBLib::GetConvertCodeName(rData.DesignCode);

	if (strCode == CONCODE_AASHTO_LRFD17) SetComboAnnexAASHTO();
	else if (strCode == CONCODE_AS5100_5_17) SetComboAnnexAS();
	if (!bUpdateDef) CDlgUtil::CobxSetCurSelItemData(m_DesignAnnex, rData.nNationalAnnex);

	if (rData.DesignCode == strCode)
	{
		m_dMrft = rData.dMrft;
		m_dTrft = rData.dTrft;
		m_iAlphaType1 = rData.iAlphaType;
		m_dAlpha1 = rData.dAlpha1;
		m_dAlpha2 = rData.dAlpha2;
		m_dConShearStrengthRatio = rData.dConShearStrengthRatio;
	}
	else
	{
		const bool bCodeOK = [strCode]() -> bool
		{
			if (strCode == CONCODE_KCI_USD07) { return true; }
			if (strCode == CONCODE_KCI_USD12) { return true; }
			if (strCode == CONCODE_KDS_41_30_2018) { return true; }
			if (strCode == CONCODE_KDS_41_20_2022) { return true; }
			if (strCode == CONCODE_ACI318_08) { return true; }
			if (strCode == CONCODE_ACI318_11) { return true; }
			if (strCode == CONCODE_ACI318_14) { return true; }
			if (strCode == CONCODE_ACI318M_14) { return true; }
			if (strCode == CONCODE_ACI318_19) { return true; }
			if (strCode == CONCODE_ACI318M_19) { return true; }
			if (strCode == CONCODE_ACI318_25) { return true; }
			if (strCode == CONCODE_ACI318M_25) { return true; }
			if (strCode == CONCODE_NSR_10) { return true; }
			if (strCode == CONCODE_NSCP_2015) { return true; }
			if (strCode == CONCODE_NTC_DCEC_2017) { return true; }
			if (strCode == CONCODE_NTC_DCEC_2023) { return true; }

			return false;
		}();

		if (bUpdateDef && bCodeOK)
		{
			m_iAlphaType1 = 1;
			m_dAlpha1 = 1.0;
			m_dAlpha2 = (strCode == CONCODE_NTC_DCEC_2017 || strCode == CONCODE_NTC_DCEC_2023) ? 1.0 : 2.0;
			m_iFrameType = 1;
			m_nSpecialWallMtd = 2;
		}
	}
	// Add by ParkBong.('06.05.18). Add bUpdateDef Option. 
	if (bUpdateDef)
	{
		// Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (GB50010-02).
		m_pDgnDataCtrl->Get_DgnConChinaDef(strCode, m_iSafeLevel, m_iEqGrade);
		m_pDgnDataCtrl->Get_DgnConMrftDef(strCode, m_dMrft);
		m_pDgnDataCtrl->Get_DgnConTrftDef(strCode, m_dTrft); //m_dTrft = 1.0;//TW에서만 사용
		if (iStatus == 1 || iStatus == 9) m_pDgnDataCtrl->Get_DgnConSufsDef(
			strCode, m_iColmMethod, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
		else if (iStatus == 2) m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iSafeLevel, m_iAlphaType1, m_dAlpha1,
		                                                         m_dAlpha2);
		else if (iStatus == 3) m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iEqGrade, m_iAlphaType1, m_dAlpha1,
		                                                         m_dAlpha2);
		else if (iStatus == 4) m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iPmCurve, m_iAlphaType1, m_dAlpha1,
		                                                         m_dAlpha2);
		else if (iStatus == 6) m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iEqCategory, m_iAlphaType1, m_dAlpha1,
		                                                         m_dAlpha2);
		else if (iStatus == 7) m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iEqCategory, m_iAlphaType1, m_dAlpha1,
		                                                         m_dAlpha2);
		else if (iStatus == 8) m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iEqCategory, m_iAlphaType1, m_dAlpha1,
		                                                         m_dAlpha2);
		else if (iStatus == 10) m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1,
		                                                          m_dAlpha2);
		else if (iStatus == 18) m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1,
		                                                          m_dAlpha2);
		else if (iStatus == 19) m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iEqCategory, m_iAlphaType1, m_dAlpha1,
		                                                          m_dAlpha2);
		else if (iStatus == 20) m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1,
		                                                          m_dAlpha2); // ACI318-08,11.
		else m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, 0, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	}

	int i = 0;
	GetDlgItem(IDC_DGN_REFPOS)->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID0; i++) GetDlgItem(m_iID0[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID1; i++) GetDlgItem(m_iID1[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID2; i++) GetDlgItem(m_iID2[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID3; i++) GetDlgItem(m_iID3[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID4; i++) GetDlgItem(m_iID4[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID6; i++) GetDlgItem(m_iID6[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID7; i++) GetDlgItem(m_iID7[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID8; i++) GetDlgItem(m_iID8[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID9; i++) GetDlgItem(m_iID9[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID10; i++) GetDlgItem(m_iID10[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID10_part; i++) GetDlgItem(m_iID10_part[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID11; i++) GetDlgItem(m_iID11[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID12; i++) GetDlgItem(m_iID12[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_ConID13; i++) GetDlgItem(m_iID13[i])->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_DGN_EUROCODE204_LIMIT_STRG_CHK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_MOMENT_GRP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SHEAR_GRP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CRACK_GRP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_DG_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_DG_EDT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_DG_UNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_KC_EDT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_WMAX_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_PL_CMB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SL_CMB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STYPE_CMB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_BRIDGE_TYPE_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_BRIDGE_TYPE_CMB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_BEAMDGN_GRP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_BEAMDGN_REINFORCEMENT1_RDO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_BEAMDGN_REINFORCEMENT2_RDO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_COLUMNDGN_GRP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE1_RDO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE2_RDO)->ShowWindow(SW_HIDE);

	CDlgUtil::CtrlShowHide(this, m_aStrutAngleCtrls, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aEffectivePhiCtrls, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrls, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrlsNTC2012, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aBCJointGroup, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aUseVc, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aCtrlIS3370, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aCondiFactor, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aAnnexCtrls, FALSE);

	CRect RectDlg, RectOk, RectCn, RectOrg, RectMrft0, RectTorsion, RectMrft1;
	CRect RectLimitStrg;
	CRect RectSCWBMethod;
	CRect RectTorsionGoup, RectTrftSt, RectTrft;
	CRect rStrutAngle, rEffectivePhi, rSlenderness;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK)->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);
	GetDlgItem(IDC_DGN_DGN_EUROCODE204_LIMIT_STRG_CHK)->GetWindowRect(&RectLimitStrg);
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->GetWindowRect(&RectMrft0);
	GetDlgItem(IDC_DGN_SCWB_METHOD_GRP)->GetWindowRect(&RectSCWBMethod);
	GetDlgItem(IDC_DGN_RCCTRL_TORSION)->GetWindowRect(&RectTorsion);
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->GetWindowRect(&RectMrft1);
	GetDlgItem(IDC_DGN_TORSION_GROUP)->GetWindowRect(&RectTorsionGoup);
	GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->GetWindowRect(&RectTrftSt);
	GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->GetWindowRect(&RectTrft);

	//Pinakin-IS456 MAT Design
	CRect IS456MATGROUP, IS456MATSTATTEXT1, IS456MATSTATTEXT2, IS456MATSOILFACTOR, IS456MATSUBGRDREXFACTOR,
	      IS456MATSUBGRADEUNIT;
	GetDlgItem(IDC_DGN_IS456MAT_GROUP)->GetWindowRect(&IS456MATGROUP);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC)->GetWindowRect(&IS456MATSTATTEXT1);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX)->GetWindowRect(&IS456MATSTATTEXT2);

	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR)->GetWindowRect(&IS456MATSOILFACTOR);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_SUBGRDREXFACTOR)->GetWindowRect(&IS456MATSUBGRDREXFACTOR);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX_MODULUS_UNIT)->GetWindowRect(&IS456MATSUBGRADEUNIT);
	// Eurocode2:04 에서 nNationalAnnex 추가로 인하여...
	// DgnCode Combo와 내진 CheckBox는 위치가 고정이라서 중간에 무었인가 넣기위해서 작업됨
	int nBCOffset = 0;
	BOOL bSingapore = FALSE;

	if (strCode == CONCODE_EC2_2_05) // Only Civil.
	{
		return SetEC2Ctrl(m_ComboNationAnnex.GetItemData(m_ComboNationAnnex.GetCurSel()));
	}

	if (strCode == CONCODE_NSR_10)
	{
		CRect rRef, rToMove;
		GetDlgItem(IDC_DGN_WALLTYPE_N14)->GetWindowRect(rRef);

		// Wall
		GetDlgItem(m_aCtrlWallBoundMtd[0])->GetWindowRect(rToMove);
		int nDistX = rRef.left - rToMove.left;
		int nDistY = rRef.bottom + globalUtils.ScaleByDPI(10) - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlWallBoundMtd, nDistX, nDistY, TRUE);

		CRect rtShearWall, rtBoundaryElem;
		CWnd* pWndShearWall = GetDlgItem(IDC_DGN_WALLTYPE_N0);
		CWnd* pWndBoudaryElem = GetDlgItem(IDC_DGN_WALLTYPE_N2);
		pWndShearWall->GetWindowRect(rtShearWall);
		pWndBoudaryElem->GetWindowRect(rtBoundaryElem);
		rtShearWall.bottom = rtBoundaryElem.bottom + globalUtils.ScaleByDPI(10);
		ScreenToClient(rtShearWall);
		pWndShearWall->MoveWindow(rtShearWall, FALSE);
	}
	else
	{
		CRect rRef, rToMove;
		GetDlgItem(IDC_DGN_WALLTYPE_N13)->GetWindowRect(rRef);

		// Wall
		GetDlgItem(m_aCtrlWallBoundMtd[0])->GetWindowRect(rToMove);
		int nDistX = rRef.left - rToMove.left;
		int nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlWallBoundMtd, nDistX, nDistY, TRUE);

		CRect rtShearWall, rtBoundaryElem;
		CWnd* pWndShearWall = GetDlgItem(IDC_DGN_WALLTYPE_N0);
		CWnd* pWndBoudaryElem = GetDlgItem(IDC_DGN_WALLTYPE_N2);
		pWndShearWall->GetWindowRect(rtShearWall);
		pWndBoudaryElem->GetWindowRect(rtBoundaryElem);
		rtShearWall.bottom = rtBoundaryElem.bottom + globalUtils.ScaleByDPI(10);
		ScreenToClient(rtShearWall);
		pWndShearWall->MoveWindow(rtShearWall, FALSE);
	}

	if (strCode == CONCODE_EC2_04)
	{
		if (m_bSpecialEQ)
		{
			GetDlgItem(IDC_DGN_EC8_BTN)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_DGN_EC8_BTN)->EnableWindow(FALSE);
		}

		bSingapore = (m_ComboNationAnnex.GetCurSel() == 3);
		int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());
		BOOL bNTC2012 = (m_ComboNationAnnex.GetCurSel() == 0 && m_bNTCFlag && nNTCType == RCS_NTC12) ? TRUE : FALSE;
		if (bNTC2012)
		{
			GetDlgItem(IDC_DGN_NTC2012_DRAFT_STATIC)->ShowWindow(SW_SHOW); //임시처리
		}
		else
		{
			GetDlgItem(IDC_DGN_NTC2012_DRAFT_STATIC)->ShowWindow(SW_HIDE); //임시처리  
		}

		//PMS:4562 (Tel. 4022) MNET:XXXX-BSCHOI-20121108 #1
		CRect RectDgnCode, RectSeis, RectRefpos, RectNationStatic, RectNationCombo, RectEC08Btn;
		GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(&RectDgnCode);
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&RectSeis);
		GetDlgItem(IDC_DGN_EC8_BTN)->GetWindowRect(&RectEC08Btn);
		GetDlgItem(IDC_DGN_REFPOS)->GetWindowRect(&RectRefpos);
		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->GetWindowRect(&RectNationStatic);
		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->GetWindowRect(&RectNationCombo);

		CRect rNationStatic;
		rNationStatic.left = RectNationStatic.left;
		rNationStatic.right = RectNationStatic.right;
		rNationStatic.top = RectDgnCode.bottom + globalUtils.ScaleByDPI(8);
		rNationStatic.bottom = rNationStatic.top + RectNationStatic.Height();

		CRect rNationCombo;
		rNationCombo.left = RectNationCombo.left;
		rNationCombo.right = RectNationCombo.right;
		rNationCombo.top = RectDgnCode.bottom + globalUtils.ScaleByDPI(8);
		rNationCombo.bottom = rNationCombo.top + RectNationCombo.Height();

		CRect rSeis;
		rSeis.left = RectSeis.left;
		rSeis.right = RectSeis.right;
		rSeis.top = rNationCombo.bottom + globalUtils.ScaleByDPI(8);
		rSeis.bottom = rSeis.top + RectSeis.Height();

		CRect rEC08Btn;
		rEC08Btn.left = RectEC08Btn.left;
		rEC08Btn.right = RectEC08Btn.right;
		rEC08Btn.top = rNationCombo.bottom + globalUtils.ScaleByDPI(8);
		rEC08Btn.bottom = rEC08Btn.top + RectEC08Btn.Height();

		ScreenToClient(rNationStatic);
		ScreenToClient(rNationCombo);
		ScreenToClient(rEC08Btn);

		CRect rNTCCheckBox, rSpecialProvision;
		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->MoveWindow(rNationStatic.left, rNationStatic.top,
		                                                      rNationStatic.Width(), rNationStatic.Height());
		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->MoveWindow(rNationCombo.left, rNationCombo.top, rNationCombo.Width(),
		                                                     rNationCombo.Height());
		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->GetWindowRect(&rNationStatic);

		int nDistY = 0;

		GetDlgItem(IDC_DGN_EUROCODE204_N18)->GetWindowRect(rNTCCheckBox);
		nDistY = rNationStatic.bottom + globalUtils.ScaleByDPI(8) - rNTCCheckBox.top;
		CDlgUtil::CtrlMoveDistY(this, m_aNTCCheckCombo, nDistY);

		GetDlgItem(IDC_DGN_EUROCODE204_N18)->GetWindowRect(rNTCCheckBox);
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(rSpecialProvision);
		nDistY = rNTCCheckBox.bottom + globalUtils.ScaleByDPI(8) - rSpecialProvision.top;
		CDlgUtil::CtrlMoveDistY(this, m_aSpecialProvision, nDistY);

		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&rSeis);
		GetDlgItem(IDC_DGN_STRUT_ANGLE_STC)->GetWindowRect(rStrutAngle);
		nDistY = rSeis.bottom + globalUtils.ScaleByDPI(8) - rStrutAngle.top;
		CDlgUtil::CtrlMoveDistY(this, m_aStrutAngleCtrls, nDistY);

		GetDlgItem(IDC_DGN_STRUT_ANGLE_STC)->GetWindowRect(rStrutAngle);
		GetDlgItem(IDC_DGN_PHIEF_STC)->GetWindowRect(rEffectivePhi);
		nDistY = rStrutAngle.bottom + globalUtils.ScaleByDPI(8) - rEffectivePhi.top;
		CDlgUtil::CtrlMoveDistY(this, m_aEffectivePhiCtrls, nDistY);

		GetDlgItem(IDC_DGN_PHIEF_STC)->GetWindowRect(rEffectivePhi);
		GetDlgItem(IDC_DGN_SLENDERNESS_LIMIT_STC)->GetWindowRect(rSlenderness);
		nDistY = rEffectivePhi.bottom + globalUtils.ScaleByDPI(8) - rSlenderness.top;
		CDlgUtil::CtrlMoveDistY(this, m_aSlendernessCtrls, nDistY);

		UpdateData(FALSE);
		OnSelchangeNTCType();

		GetDlgItem(IDC_DGN_SLENDERNESS_LIMIT_STC)->GetWindowRect(rSlenderness);
		CRect rRefpos;
		rRefpos.left = RectRefpos.left;
		rRefpos.right = RectRefpos.right;
		rRefpos.top = rSlenderness.bottom + globalUtils.ScaleByDPI(8);
		rRefpos.bottom = rRefpos.top + RectRefpos.Height();

		ScreenToClient(rRefpos);
		GetDlgItem(IDC_DGN_REFPOS)->MoveWindow(rRefpos.left, rRefpos.top, rRefpos.Width(), rRefpos.Height());

		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->ShowWindow(SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aStrutAngleCtrls, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aEffectivePhiCtrls, TRUE);


		//(Tel. 4022) MNET:XXXX-BSCHOI-20121024 #1
		CRect rBCJointGroup;
		GetDlgItem(IDC_DGN_EUROCODE204_N17)->GetWindowRect(rBCJointGroup);
		nDistY = rSlenderness.bottom + globalUtils.ScaleByDPI(8) - rBCJointGroup.top;
		CDlgUtil::CtrlMoveDistY(this, m_aBCJointGroup, nDistY);
		CDlgUtil::CtrlShowHide(this, m_aBCJointGroup, TRUE);
		nBCOffset = rBCJointGroup.Height() + globalUtils.ScaleByDPI(8);

		GetDlgItem(IDC_DGN_DGN_EUROCODE204_LIMIT_STRG_CHK)->ShowWindow(bSingapore);

		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->ShowWindow(SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aNTCCheckCombo, TRUE);

		GetDlgItem(IDC_DGN_NTC2012_DRAFT_STATIC)->ShowWindow(bNTC2012 ? SW_SHOW : SW_HIDE);
	}
	else if (strCode == CONCODE_IRC112_2011 || strCode == CONCODE_IRC112_2020) // Only Civil.IRC112 RC DEsign Pinakin
	{
		CRect RectDgnCode, RectSeis, RectRefpos, RectNationStatic, RectNationCombo, RectEC08Btn;
		GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(&RectDgnCode);
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&RectSeis);
		GetDlgItem(IDC_DGN_EC8_BTN)->GetWindowRect(&RectEC08Btn);
		GetDlgItem(IDC_DGN_REFPOS)->GetWindowRect(&RectRefpos);
		//GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->GetWindowRect(&RectNationStatic);
		//GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->GetWindowRect(&RectNationCombo);

		// Add StrutAngle to Calc Shear Resistence.
		CRect RectAngleChk, RectAngleEdt, RectAngleUnt;
		GetDlgItem(IDC_DGN_STRUT_ANGLE_STC)->GetWindowRect(&RectAngleChk);
		GetDlgItem(IDC_DGN_STRUT_ANGLE_EDT)->GetWindowRect(&RectAngleEdt);
		GetDlgItem(IDC_DGN_STRUT_ANGLE_UNT)->GetWindowRect(&RectAngleUnt);

		//
		CRect rAngleChk = RectAngleChk;
		rAngleChk.left = RectSeis.left;
		rAngleChk.top = RectDgnCode.bottom + globalUtils.ScaleByDPI(8);
		rAngleChk.bottom = rAngleChk.top + RectAngleChk.Height();
		//
		CRect rAngleEdt = RectAngleEdt;
		rAngleEdt.top = RectDgnCode.bottom + globalUtils.ScaleByDPI(8);
		rAngleEdt.bottom = rAngleEdt.top + RectAngleEdt.Height();
		//
		CRect rAngleUnt = RectAngleUnt;
		rAngleUnt.top = RectDgnCode.bottom + globalUtils.ScaleByDPI(8);
		rAngleUnt.bottom = rAngleUnt.top + RectAngleUnt.Height();

		CRect rSeis;
		rSeis.left = RectSeis.left;
		rSeis.right = RectSeis.right;
		rSeis.top = rAngleChk.bottom + globalUtils.ScaleByDPI(8);
		rSeis.bottom = rSeis.top + RectSeis.Height();

		CRect rEC08Btn;
		rEC08Btn.left = RectEC08Btn.left;
		rEC08Btn.right = RectEC08Btn.right;
		rEC08Btn.top = rAngleChk.bottom + globalUtils.ScaleByDPI(8);
		rEC08Btn.bottom = rEC08Btn.top + RectEC08Btn.Height();

		CRect rRefpos;
		rRefpos.left = RectRefpos.left;
		rRefpos.right = RectRefpos.right;
		rRefpos.top = rSeis.bottom + globalUtils.ScaleByDPI(8);
		rRefpos.bottom = rRefpos.top + RectRefpos.Height();

		//ScreenToClient(rNationStatic);
		//ScreenToClient(rNationCombo);
		ScreenToClient(rSeis);
		ScreenToClient(rEC08Btn);
		//
		ScreenToClient(rAngleChk);
		ScreenToClient(rAngleEdt);
		ScreenToClient(rAngleUnt);

		ScreenToClient(rRefpos);

		//GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->MoveWindow(rNationStatic.left, rNationStatic.top, rNationStatic.Width(), rNationStatic.Height());  
		//GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->MoveWindow(rNationCombo.left, rNationCombo.top, rNationCombo.Width(), rNationCombo.Height());  
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->MoveWindow(rSeis.left, rSeis.top, rSeis.Width(), rSeis.Height());
		GetDlgItem(IDC_DGN_EC8_BTN)->MoveWindow(rEC08Btn.left, rEC08Btn.top, rEC08Btn.Width(), rEC08Btn.Height());

		GetDlgItem(IDC_DGN_STRUT_ANGLE_STC)->MoveWindow(rAngleChk.left, rAngleChk.top, rAngleChk.Width(),
		                                                rAngleChk.Height());
		GetDlgItem(IDC_DGN_STRUT_ANGLE_EDT)->MoveWindow(rAngleEdt.left, rAngleEdt.top, rAngleEdt.Width(),
		                                                rAngleEdt.Height());
		GetDlgItem(IDC_DGN_STRUT_ANGLE_UNT)->MoveWindow(rAngleUnt.left, rAngleUnt.top, rAngleUnt.Width(),
		                                                rAngleUnt.Height());

		GetDlgItem(IDC_DGN_REFPOS)->MoveWindow(rRefpos.left, rRefpos.top, rRefpos.Width(), rRefpos.Height());

		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->ShowWindow(SW_HIDE);

		CDlgUtil::CtrlShowHide(this, m_aStrutAngleCtrls, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrls, TRUE);

		GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->ShowWindow(SW_HIDE);
	}
	else if (strCode == CONCODE_BS5400_90)
	{
		ShowBSCtrl();
		MoveBSCtrl();
		return;
	}
	else if (strCode == CONCODE_TMH07_89)
	{
		ShowBSCtrl();
		MoveBSCtrl();
		return;
	}
	else if (strCode == CONCODE_AASHTO_LRFD12 || strCode == CONCODE_AASHTO_LRFD16 || strCode == CONCODE_AASHTO_LRFD17 ||
		strCode == CONCODE_AASHTO_LRFD20 || strCode == CONCODE_AASHTO_LRFD24)
	{
		CRect RectDgnCode, RectSeis, RectRefpos, RectEC08Btn, RectPMChk;
		GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(&RectDgnCode);
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&RectSeis);
		GetDlgItem(IDC_DGN_REFPOS)->GetWindowRect(&RectRefpos);
		GetDlgItem(IDC_DGN_PM_CURVE_CHK)->GetWindowRect(&RectPMChk);

		CRect rSeis;

		BOOL bShowAmendent = FALSE;
		if (strCode == CONCODE_AASHTO_LRFD17) bShowAmendent = TRUE;

		if (bShowAmendent)
		{
			CRect rRef;
			CRect rToMove;
			int nDistX, nDistY;
			GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(rRef);
			GetDlgItem(IDC_CRC_DGN_ANNEX_AASHTO_CMB)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, m_aAnnexCtrls, nDistX, nDistY);
			CRect RectAnnexAASHTO;
			GetDlgItem(IDC_CRC_DGN_ANNEX_AASHTO_CMB)->GetWindowRect(&RectAnnexAASHTO);

			rSeis.left = RectSeis.left;
			rSeis.right = RectSeis.right;
			rSeis.top = RectAnnexAASHTO.bottom + globalUtils.ScaleByDPI(8);
			rSeis.bottom = rSeis.top + RectSeis.Height();
		}
		else
		{
			rSeis.left = RectSeis.left;
			rSeis.right = RectSeis.right;
			rSeis.top = RectDgnCode.bottom + globalUtils.ScaleByDPI(8);
			rSeis.bottom = rSeis.top + RectSeis.Height();
		}

		// MQC.20033 컨트롤 겹침 수정
		CRect rPMChk;
		rPMChk.left = RectPMChk.left;
		rPMChk.right = RectPMChk.right;
		rPMChk.top = rSeis.bottom + globalUtils.ScaleByDPI(8);
		rPMChk.bottom = rPMChk.top + RectPMChk.Height();

		CRect rRefpos;
		rRefpos.left = RectRefpos.left;
		rRefpos.right = RectRefpos.right;
		rRefpos.top = (iStatus > 0 ? rSeis.bottom : rPMChk.bottom) + globalUtils.ScaleByDPI(8);
		rRefpos.bottom = rRefpos.top + RectRefpos.Height();

		ScreenToClient(rSeis);
		ScreenToClient(rRefpos);
		// MQC.20033 컨트롤 겹침 수정
		if (iStatus == 0) ScreenToClient(rPMChk);

		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->MoveWindow(rSeis.left, rSeis.top, rSeis.Width(), rSeis.Height());
		GetDlgItem(IDC_DGN_REFPOS)->MoveWindow(rRefpos.left, rRefpos.top, rRefpos.Width(), rRefpos.Height());
		// MQC.20033 컨트롤 겹침 수정
		if (iStatus == 0) GetDlgItem(IDC_DGN_PM_CURVE_CHK)->MoveWindow(rPMChk.left, rPMChk.top, rPMChk.Width(),
		                                                               rPMChk.Height());

		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->ShowWindow(SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aNTCCheckCombo, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aAnnexCtrls, bShowAmendent);
	}
	else if (strCode == CONCODE_AS5100_5_17) // Only Civil.AS 5100.5:17
	{
		CRect rRef;
		CRect rToMove;
		int nDistX, nDistY;
		GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(rRef);
		GetDlgItem(IDC_CRC_DGN_ANNEX_AASHTO_CMB)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistXY(this, m_aAnnexCtrls, nDistX, nDistY);
		CRect RectAnnex;
		GetDlgItem(IDC_CRC_DGN_ANNEX_AASHTO_CMB)->GetWindowRect(&RectAnnex);
		CDlgUtil::CtrlShowHide(this, m_aAnnexCtrls, TRUE);

		CRect RectDgnCode, RectDgnCodeCmb, RectShearGrp, RectMrftStatic, RectMrftFactor, RectRefpos;
		CRect RectDgStc, RectDgEdt, RectDgUnt;
		CRect RectPmCurveChk;
		GetDlgItem(IDC_WG_DGN_STATIC0)->GetWindowRect(&RectDgnCode);
		GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(&RectDgnCodeCmb);
		GetDlgItem(IDC_DGN_SHEAR_GRP)->GetWindowRect(&RectShearGrp);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->GetWindowRect(&RectMrftStatic);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->GetWindowRect(&RectMrftFactor);
		GetDlgItem(IDC_DGN_REFPOS)->GetWindowRect(&RectRefpos);
		GetDlgItem(IDC_DGN_DG_STC)->GetWindowRect(&RectDgStc);
		GetDlgItem(IDC_DGN_DG_EDT)->GetWindowRect(&RectDgEdt);
		GetDlgItem(IDC_DGN_DG_UNT)->GetWindowRect(&RectDgUnt);
		GetDlgItem(IDC_DGN_PM_CURVE_CHK)->GetWindowRect(&RectPmCurveChk);

		// set text
		GetDlgItem(IDC_DGN_SHEAR_GRP)->SetWindowText(_LS(IDS_DGN_MAXAGG_SIZE));
		GetDlgItem(IDC_DGN_DG_STC)->SetWindowText(_LS(IDS_DGN_MAXAGG_SIZE_DG));

		CRect rShearGrp;
		rShearGrp.left = RectShearGrp.left;
		rShearGrp.right = RectShearGrp.right;
		rShearGrp.top = RectAnnex.bottom + globalUtils.ScaleByDPI(8);
		rShearGrp.bottom = rShearGrp.top + RectShearGrp.Height();

		int nDgStcOffsetX = RectDgStc.left - RectShearGrp.left;
		int nDgStcOffsetY = RectDgStc.top - RectShearGrp.top;
		int nDgEdtOffsetX = RectDgEdt.left - RectShearGrp.left;
		int nDgEdtOffsetY = RectDgEdt.top - RectShearGrp.top;
		int nDgUntOffsetX = RectDgUnt.left - RectShearGrp.left;
		int nDgUntOffsetY = RectDgUnt.top - RectShearGrp.top;

		CRect rDgStc, rDgEdt, rDgUnt;
		rDgStc.left = rShearGrp.left + nDgStcOffsetX;
		rDgStc.top = rShearGrp.top + nDgStcOffsetY;
		rDgStc.right = rDgStc.left + RectDgStc.Width();
		rDgStc.bottom = rDgStc.top + RectDgStc.Height();

		rDgEdt.left = rShearGrp.left + nDgEdtOffsetX;
		rDgEdt.top = rShearGrp.top + nDgEdtOffsetY;
		rDgEdt.right = rDgEdt.left + RectDgEdt.Width();
		rDgEdt.bottom = rDgEdt.top + RectDgEdt.Height();

		rDgUnt.left = rShearGrp.left + nDgUntOffsetX;
		rDgUnt.top = rShearGrp.top + nDgUntOffsetY;
		rDgUnt.right = rDgUnt.left + RectDgUnt.Width();
		rDgUnt.bottom = rDgUnt.top + RectDgUnt.Height();

		CRect rPmCurveChk;
		rPmCurveChk.left = RectMrftStatic.left;
		rPmCurveChk.right = rPmCurveChk.left + RectPmCurveChk.Width();
		rPmCurveChk.top = rShearGrp.bottom + globalUtils.ScaleByDPI(8);
		rPmCurveChk.bottom = rPmCurveChk.top + RectPmCurveChk.Height();

		CRect rMrftStatic;
		rMrftStatic.left = RectMrftStatic.left;
		rMrftStatic.right = RectMrftStatic.right;
		rMrftStatic.top = rPmCurveChk.bottom + globalUtils.ScaleByDPI(8);
		rMrftStatic.bottom = rMrftStatic.top + RectMrftStatic.Height();

		CRect rMrftFactor;
		rMrftFactor.left = RectMrftFactor.left;
		rMrftFactor.right = RectMrftFactor.right;
		rMrftFactor.top = rMrftStatic.top;
		rMrftFactor.bottom = rMrftStatic.bottom;

		CRect rRefpos;
		rRefpos.left = RectRefpos.left;
		rRefpos.right = RectRefpos.right;
		rRefpos.top = rMrftStatic.bottom + globalUtils.ScaleByDPI(8);
		rRefpos.bottom = rRefpos.top + RectRefpos.Height();

		ScreenToClient(rShearGrp);
		ScreenToClient(rDgStc);
		ScreenToClient(rDgEdt);
		ScreenToClient(rDgUnt);
		ScreenToClient(rPmCurveChk);
		ScreenToClient(rMrftStatic);
		ScreenToClient(rMrftFactor);
		ScreenToClient(rRefpos);

		GetDlgItem(IDC_DGN_SHEAR_GRP)->MoveWindow(rShearGrp.left, rShearGrp.top, rShearGrp.Width(), rShearGrp.Height());
		GetDlgItem(IDC_DGN_DG_STC)->MoveWindow(rDgStc.left, rDgStc.top, rDgStc.Width(), rDgStc.Height());
		GetDlgItem(IDC_DGN_DG_EDT)->MoveWindow(rDgEdt.left, rDgEdt.top, rDgEdt.Width(), rDgEdt.Height());
		GetDlgItem(IDC_DGN_DG_UNT)->MoveWindow(rDgUnt.left, rDgUnt.top, rDgUnt.Width(), rDgUnt.Height());
		GetDlgItem(IDC_DGN_PM_CURVE_CHK)->MoveWindow(rPmCurveChk.left, rPmCurveChk.top, rPmCurveChk.Width(),
			rPmCurveChk.Height());
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->MoveWindow(rMrftStatic.left, rMrftStatic.top, rMrftStatic.Width(),
			rMrftStatic.Height());
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->MoveWindow(rMrftFactor.left, rMrftFactor.top, rMrftFactor.Width(),
			rMrftFactor.Height());
		GetDlgItem(IDC_DGN_REFPOS)->MoveWindow(rRefpos.left, rRefpos.top, rRefpos.Width(), rRefpos.Height());

		GetDlgItem(IDC_DGN_SHEAR_GRP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_DG_STC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_DG_EDT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_DG_UNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_PM_CURVE_CHK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->ShowWindow(SW_SHOW);

		// hide
		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_EC8_BTN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->ShowWindow(SW_HIDE);

		CDlgUtil::CtrlShowHide(this, m_aStrutAngleCtrls, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aEffectivePhiCtrls, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrls, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aBCJointGroup, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aUseVc, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aNTCCheckCombo, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlIS3370, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCondiFactor, FALSE);
	}
	else
	{
		CRect RectDgnCode, RectSeis, RectRefpos, RectEC08Btn;
		GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(&RectDgnCode);
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&RectSeis);
		GetDlgItem(IDC_DGN_EC8_BTN)->GetWindowRect(&RectEC08Btn);
		GetDlgItem(IDC_DGN_REFPOS)->GetWindowRect(&RectRefpos);

		CRect rSeis;
		rSeis.left = RectSeis.left;
		rSeis.right = RectSeis.right;
		rSeis.top = RectDgnCode.bottom + globalUtils.ScaleByDPI(8);
		rSeis.bottom = rSeis.top + RectSeis.Height();

		CRect rEC08Btn;
		rEC08Btn.left = RectEC08Btn.left;
		rEC08Btn.right = RectEC08Btn.right;
		rEC08Btn.top = RectDgnCode.bottom + globalUtils.ScaleByDPI(8);
		rEC08Btn.bottom = rEC08Btn.top + RectEC08Btn.Height();

		CRect rRefpos;
		rRefpos.left = RectRefpos.left;
		rRefpos.right = RectRefpos.right;
		rRefpos.top = rSeis.bottom + globalUtils.ScaleByDPI(8);
		rRefpos.bottom = rRefpos.top + RectRefpos.Height();

		ScreenToClient(rSeis);
		ScreenToClient(rEC08Btn);
		ScreenToClient(rRefpos);

		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->MoveWindow(rSeis.left, rSeis.top, rSeis.Width(), rSeis.Height());
		GetDlgItem(IDC_DGN_EC8_BTN)->MoveWindow(rEC08Btn.left, rEC08Btn.top, rEC08Btn.Width(), rEC08Btn.Height());
		GetDlgItem(IDC_DGN_REFPOS)->MoveWindow(rRefpos.left, rRefpos.top, rRefpos.Width(), rRefpos.Height());

		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->ShowWindow(SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aNTCCheckCombo, FALSE);
	}

	CRect RectRef;
	GetDlgItem(IDC_DGN_REFPOS)->GetWindowRect(&RectRef);
	// EQ
	if (iStatus == 1) RectOrg = MoveCtrl(1, m_iID1, RectRef);
	else if (iStatus == 2) RectOrg = MoveCtrl(2, m_iID2, RectRef);
	else if (iStatus == 3) RectOrg = MoveCtrl(3, m_iID3, RectRef);
	else if (iStatus == 4) RectOrg = MoveCtrl(4, m_iID4, RectRef);
	else if (iStatus == 6) RectOrg = MoveCtrl(6, m_iID6, RectRef);
	else if (iStatus == 7) RectOrg = MoveCtrl(7, m_iID6, RectRef);
	else if (iStatus == 8) RectOrg = MoveCtrl(8, m_iID6, RectRef);
	else if (iStatus == 9)
	{
		RectOrg = MoveCtrl(9, m_iID7, RectRef);
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = nDlgRight;
		RectOrg = MoveCtrl(1, m_iID1, RectRef);
	}
	else if (iStatus == 10)
	{
#if defined(_CIVIL)
		//        GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&RectOrg);
		RectOrg = MoveCtrl(10, m_iID8, RectRef);
#else
		RectOrg = MoveCtrl(10, m_iID8, RectRef);
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = nDlgRight;
		RectOrg = MoveCtrl(9, m_iID7, RectRef);
		// Coded by Seungjun MNet:No.2591 ('20061207)	For SCWB Design.
		/*
		RectRef.top    = RectOrg.bottom + (RectOrg.left-RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom-RectOrg.top);
		RectRef.left   = RectOrg.left;
		RectRef.right  = nDlgRight;
		RectOrg = MoveCtrl(4,m_iID4, RectRef);
		*/

#endif
	}
	else if (iStatus == 18)
	{
#if defined(_CIVIL)
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&RectOrg);
#else
		if (m_iFrameType == 2)
		{
			RectOrg = MoveCtrl(10, m_iID8, RectRef);
		}
		else
		{
			RectOrg = MoveCtrl(10, m_iID8, RectRef);
			RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
			RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
			RectRef.left = RectOrg.left;
			RectRef.right = nDlgRight;
			RectOrg = MoveCtrl(12, m_iID10, RectRef);
			RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
			RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
			RectRef.left = RectOrg.left;
			RectRef.right = nDlgRight;
			RectOrg = MoveCtrl(9, m_iID7, RectRef);
			RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
			RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
			RectRef.left = RectOrg.left;
			RectRef.right = nDlgRight;
			RectOrg = MoveCtrl(13, m_iID10_part, RectRef);
		}
		//RectOrg = MoveCtrl(9,m_iID7, RectRef);
#endif
	}
	else if (iStatus == 20) // ACI318-08, 11.
	{
#if defined(_CIVIL)
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&RectOrg);
#else
		RectOrg = MoveCtrl(10, m_iID8, RectRef);
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = nDlgRight;
		RectOrg = MoveCtrl(12, m_iID10, RectRef);
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = nDlgRight;
		RectOrg = MoveCtrl(9, m_iID7, RectRef);
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = nDlgRight;
		RectOrg = MoveCtrl(13, m_iID10_part, RectRef);
#endif
	}
	else if (iStatus == 11 || iStatus == 12 || iStatus == 13 || iStatus == 14 || iStatus == 15)
	{
#if defined(_CIVIL)
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&RectOrg);
#else
		RectOrg = MoveCtrl(9, m_iID7, RectRef);
#endif
	}
	else if (iStatus == 17) //Eurocode2:04
	{
		//(Tel. 4022) MNET:XXXX-BSCHOI-20121024 #1
		RectRef.top += nBCOffset;
		RectOrg = MoveCtrl(11, m_iID9, RectRef);
		GetDlgItem(IDC_DGN_EUROCODE204_N19)->GetWindowRect(&RectOrg);
		RectOrg.bottom -= nBCOffset;
	}
	else if (iStatus == 19)
	{
		RectOrg = MoveCtrl(6, m_iID6, RectRef);
		RectRef.top = RectOrg.bottom + globalUtils.ScaleByDPI(8);

		// MQC.20033 컨트롤 겹침 수정
		if (strCode == CONCODE_AASHTO_LRFD12 || strCode == CONCODE_AASHTO_LRFD16 || strCode == CONCODE_AASHTO_LRFD17 ||
			strCode == CONCODE_AASHTO_LRFD20 || strCode == CONCODE_AASHTO_LRFD24)
		{
			CRect RectPMChk;
			GetDlgItem(IDC_DGN_PM_CURVE_CHK)->GetWindowRect(&RectPMChk);

			CRect rPMChk;
			rPMChk.left = RectPMChk.left;
			rPMChk.right = RectPMChk.right;
			rPMChk.top = RectRef.top;
			rPMChk.bottom = rPMChk.top + RectPMChk.Height();

			RectRef.top = RectOrg.bottom + globalUtils.ScaleByDPI(8);

			ScreenToClient(rPMChk);
			GetDlgItem(IDC_DGN_PM_CURVE_CHK)->MoveWindow(rPMChk.left, rPMChk.top, rPMChk.Width(), rPMChk.Height());
		}
	}
	else if (iStatus == 16) //IS456 Ductile COde Update
	{
		if (strCode == CONCODE_IS456_2000)
		{
			RectOrg = MoveCtrl(15, m_iID12, RectRef);
			RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
			RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
			RectRef.left = RectOrg.left;
			RectRef.right = nDlgRight;
			RectOrg = MoveCtrl(9, m_iID7, RectRef);
		}
		else
		{
#if defined(_CIVIL)
			GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&RectOrg);
#else
			RectOrg = MoveCtrl(9, m_iID7, RectRef);
#endif
		}
		// GetDlgItem(IDC_DGN_EUROCODE204_N19)->GetWindowRect(&RectOrg);
		//RectOrg.bottom -= nBCOffset;
	}
	else
	{
		if (strCode == CONCODE_AASHTO_LRFD12 || strCode == CONCODE_AASHTO_LRFD16 || strCode == CONCODE_AASHTO_LRFD17 ||
			strCode == CONCODE_AASHTO_LRFD20 || strCode == CONCODE_AASHTO_LRFD24)
		{
			GetDlgItem(IDC_DGN_PM_CURVE_CHK)->GetWindowRect(&RectOrg);
		}
		else
		{
			GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&RectOrg);
		}
	}

	//Slab Type
#if defined(_MGEN)
	if (iStatus == 2 || iStatus == 3)
	{
		CRect* pRectCng = new CRect[iDgn_ConID0];

		CRect Rect1;
		for (int i = 0; i < iDgn_ConID0; i++)
		{
			GetDlgItem(m_iID0[i])->ShowWindow(SW_SHOW);
			GetDlgItem(m_iID0[i])->GetWindowRect(&(pRectCng[i]));
			if (i == 0) Rect1 = pRectCng[i];
			pRectCng[i].top += (RectOrg.bottom - RectOrg.top) + 0.5 * RectOk.Height() - (Rect1.top - RectOrg.top);
			pRectCng[i].bottom += (RectOrg.bottom - RectOrg.top) + 0.5 * RectOk.Height() - (Rect1.top - RectOrg.top);
		}
		RectOrg = pRectCng[0];
		for (int i = 0; i < iDgn_ConID0; i++)
		{
			ScreenToClient(pRectCng[i]);
			GetDlgItem(m_iID0[i])->SetWindowPos(NULL, pRectCng[i].left, pRectCng[i].top, pRectCng[i].Width(),
			                                    pRectCng[i].Height(), SWP_NOSIZE | SWP_NOZORDER);
		}
		delete pRectCng;
	}
#endif

	// Data
	//   if(strCode == _T("AASHTO-LRFD07") || strCode == _T("AASHTO-LRFD12"))
	//   {    
	//     RectOrg = MoveCtrl(14,m_iID11, RectRef);
	//     GetDlgItem(IDC_DGN_AASHTO07_PARAM5)->GetWindowRect(&RectOrg);
	//   }

	int nLimitStrg = 0;
	if (bSingapore) nLimitStrg = 1.5 * RectLimitStrg.Height();
	// MRFT
	int iMrftHeight = RectMrft0.Height();
	int iTorsionHeight = RectTorsion.Height();
	int iTorsionGrupHeight = RectTorsionGoup.Height();

	const int nOffsetOK = globalUtils.ScaleByDPI(16) + globalUtils.ScaleByDPI(8);

	CRect rDlg;
	rDlg.left = RectDlg.left;
	rDlg.top = RectDlg.top;
	rDlg.right = nDlgRight + (RectOrg.left - RectDlg.left);
	rDlg.bottom = RectOrg.bottom + nOffsetOK + RectOk.Height() + iMrftHeight + globalUtils.ScaleByDPI(8) + nLimitStrg;

	CRect rMrft0, rMrft1, rTorsion;
	BOOL bTWN_USD92 = FALSE;

	// Add, Jaeoh. (06.12.06)
#if defined(_CIVIL)
	if (strCode == CONCODE_KSCE_USD10 || strCode == CONCODE_KSCE_RAIL_USD11 || strCode == CONCODE_KCI_USD12 || 
		strCode == CONCODE_KSCE_LSD15 || strCode == CONCODE_KDS_24_14_21_2022 || strCode == CONCODE_KDS_14_20_00_2022)
	{
		rDlg.bottom = RectOrg.bottom; // + globalUtils.ScaleByDPI(8);

		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_PM_CURVE_CHK)->ShowWindow(SW_HIDE);
	}
	else if (strCode == CONCODE_KSCE_USD05 || strCode == CONCODE_KSCE_RAIL_USD04 || strCode == CONCODE_KCI_USD07)
	{
		rDlg.bottom = RectOrg.bottom + RectOk.Height() + nOffsetOK;
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_PM_CURVE_CHK)->ShowWindow(SW_HIDE);
	}
	else if (strCode == CONCODE_AS5100_5_17)
	{
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->GetWindowRect(&RectOrg);
		rDlg.bottom = RectOrg.bottom + RectOk.Height() + nOffsetOK;

		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_DGN_PM_CURVE_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->ShowWindow(SW_SHOW);
		if (strCode == CONCODE_AASHTO_LRFD12 || strCode == CONCODE_AASHTO_LRFD16 || strCode == CONCODE_AASHTO_LRFD17 ||
			strCode == CONCODE_AASHTO_LRFD20 || strCode == CONCODE_AASHTO_LRFD24 ||
			strCode == CONCODE_IS456_2000)
		{
			GetDlgItem(IDC_DGN_PM_CURVE_CHK)->ShowWindow(SW_SHOW);
		}

		if (strCode == CONCODE_BS5400_90)
		{
			GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_PM_CURVE_CHK)->ShowWindow(SW_SHOW);
		}

		if (strCode == CONCODE_TMH07_89)
		{
			GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_PM_CURVE_CHK)->ShowWindow(SW_SHOW);
		}

		// MQC.20033 컨트롤 겹침 수정
		if (strCode == CONCODE_AASHTO_LRFD12 || strCode == CONCODE_AASHTO_LRFD16 || strCode == CONCODE_AASHTO_LRFD17 ||
			strCode == CONCODE_AASHTO_LRFD20 || strCode == CONCODE_AASHTO_LRFD24)
		{
			if (iStatus == 19)
				rDlg.bottom += RectMrft0.Height();
		}
	}
	//081030 sshan:M_net:3780
	GetDlgItem(IDC_DGN_RCCTRL_TORSION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_TORSION_GROUP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->ShowWindow(SW_HIDE);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSCWBMethod, FALSE); // PMS:4230-HSSHIM-20101105
#else

	GetDlgItem(IDC_DGN_RCCTRL_TORSION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_TORSION_GROUP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->ShowWindow(SW_HIDE);
	//Pinakin IS456
	GetDlgItem(IDC_DGN_IS456MAT_GROUP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_SUBGRDREXFACTOR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX_MODULUS_UNIT)->ShowWindow(SW_HIDE);

	//END Pinakin IS456
	CDlgUtil::CtrlShowHide(this, m_aCtrlSCWBMethod, FALSE); // PMS:4230-HSSHIM-20101105
	if (strCode == CONCODE_TWN_USD92 || strCode == CONCODE_TWN_USD100 || strCode == CONCODE_TWN_USD112)
	//TWN-USD92, TWN-USD100
	{
		rDlg.bottom = RectOrg.bottom + (int)(2.5 * RectOk.Height() + iMrftHeight + iTorsionGrupHeight);
		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TORSION_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->ShowWindow(SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aCtrlSCWBMethod, TRUE); // PMS:4230-HSSHIM-20101105
		if ((strCode == CONCODE_TWN_USD100 || strCode == CONCODE_TWN_USD112) &&
			!m_pDoc->m_pAttrCtrl->ExistDcon())
		{
			CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, 1);
			// 1: Norminal Strength // PMS:4385-PARKHJ-20111025 : 대만대리점 요청사항.
		}
		bTWN_USD92 = TRUE;
		OnChangeTorsion();
	}
	// Seungjun '151126 PMS.5165 ACI318-08, 11 보 비틀림 설계 추가
	else if (strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 ||
		strCode == CONCODE_ACI318_14 || strCode == CONCODE_ACI318M_14 || strCode == CONCODE_NSCP_2015 ||
		strCode == CONCODE_KCI_USD07 || strCode == CONCODE_KCI_USD12 ||
		strCode == CONCODE_KDS_41_30_2018 || strCode == CONCODE_KDS_41_20_2022 ||
		strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318M_19 ||
		strCode == CONCODE_ACI318_25 || strCode == CONCODE_ACI318M_25 ||
		strCode == CONCODE_NTC_DCEC_2017 || strCode == CONCODE_NTC_DCEC_2023)
	{
		rDlg.bottom = RectOrg.bottom + (int)(2.5 * RectOk.Height() + iMrftHeight + iTorsionGrupHeight);
		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TORSION_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->ShowWindow(SW_SHOW);
		OnChangeTorsion();
	}
	else if (strCode == CONCODE_EC2_04)
	{
		if (!m_bSpecialEQ)
		{
			int nRectAngleHeight = rStrutAngle.Height() + 8;
			int nRectPhiEfHeight = rEffectivePhi.Height() + 8;
			int nRectSlendHeight = rSlenderness.Height() + 8;
			GetDlgItem(IDC_DGN_RCCTRL_TORSION)->ShowWindow(SW_SHOW);
			rDlg.bottom = RectOrg.bottom + (int)(2.5 * RectOk.Height() + iMrftHeight + nRectAngleHeight +
				nRectPhiEfHeight + nRectSlendHeight + nLimitStrg + iTorsionHeight);
		}
		else
		{
			GetDlgItem(IDC_DGN_RCCTRL_TORSION)->ShowWindow(SW_SHOW);
			rDlg.bottom = RectOrg.bottom + (int)(2.5 * RectOk.Height() + iMrftHeight + nLimitStrg + iTorsionHeight);
		}
	}
	else if (strCode == CONCODE_GB50010_10)
	{
		rDlg.bottom = RectOrg.bottom + (int)(2.5 * RectOk.Height() + iMrftHeight + iTorsionGrupHeight);
		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_TORSION_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->ShowWindow(SW_SHOW);
		OnChangeTorsion();
	}
	else if (strCode == CONCODE_IS456_2000) //Pinakin
	{
		rTorsion.left = RectTorsion.left;
		rTorsion.right = RectTorsion.right;
		rTorsion.top = rDlg.bottom - (int)(1.3 * RectTorsionGoup.Height() + 1.0 * RectMrft0.Height() + 2.5 * RectOk.
			Height());
		rTorsion.bottom = rTorsion.top + RectTorsion.Height();

		IS456MATGROUP.left = RectTorsionGoup.left;
		IS456MATGROUP.right = RectTorsionGoup.right;
		IS456MATGROUP.top = rDlg.bottom - (int)(1.0 * RectTorsionGoup.Height() + 1.0 * RectMrft0.Height() + 2.5 * RectOk
			.Height());
		IS456MATGROUP.bottom = IS456MATGROUP.top + RectTorsionGoup.Height();

		IS456MATSTATTEXT1.left = RectTrftSt.left;
		IS456MATSTATTEXT1.right = RectTrftSt.right;
		IS456MATSTATTEXT1.top = rDlg.bottom - (int)(1.2 * RectTrftSt.Height() + 1.0 * RectMrft0.Height() + 2.5 * RectOk.
			Height());
		IS456MATSTATTEXT1.bottom = IS456MATSTATTEXT1.top + RectTrftSt.Height();

		IS456MATSTATTEXT2.left = RectTrftSt.left;
		IS456MATSTATTEXT2.right = RectTrftSt.right;
		IS456MATSTATTEXT2.top = rDlg.bottom - (int)(0.05 * RectTrftSt.Height() + 1.0 * RectMrft0.Height() + 2.5 * RectOk
			.Height());
		IS456MATSTATTEXT2.bottom = IS456MATSTATTEXT2.top + RectTrftSt.Height();

		IS456MATSOILFACTOR.left = RectTrft.left - 75;
		IS456MATSOILFACTOR.right = RectTrft.right - 75;
		IS456MATSOILFACTOR.top = rDlg.bottom - (int)(1.3 * RectTrft.Height() + 1.0 * RectMrft0.Height() + 2.5 * RectOk.
			Height());
		IS456MATSOILFACTOR.bottom = IS456MATSOILFACTOR.top + RectTrft.Height();

		IS456MATSUBGRDREXFACTOR.left = RectTrft.left - 75;
		IS456MATSUBGRDREXFACTOR.right = RectTrft.right - 75;
		IS456MATSUBGRDREXFACTOR.top = rDlg.bottom - (int)(0.15 * RectTrft.Height() + 1.0 * RectMrft0.Height() + 2.5 *
			RectOk.Height());
		IS456MATSUBGRDREXFACTOR.bottom = IS456MATSUBGRDREXFACTOR.top + RectTrft.Height();

		IS456MATSUBGRADEUNIT.left = RectTrft.left + 10;
		IS456MATSUBGRADEUNIT.right = RectTrft.right + 10;
		IS456MATSUBGRADEUNIT.top = rDlg.bottom - (int)(0.05 * RectTrft.Height() + 1.0 * RectMrft0.Height() + 2.5 *
			RectOk.Height());
		IS456MATSUBGRADEUNIT.bottom = IS456MATSUBGRADEUNIT.top + RectTrft.Height();

		ScreenToClient(rTorsion);
		ScreenToClient(IS456MATGROUP);
		ScreenToClient(IS456MATSTATTEXT1);
		ScreenToClient(IS456MATSTATTEXT2);
		ScreenToClient(IS456MATSOILFACTOR);
		ScreenToClient(IS456MATSUBGRDREXFACTOR);
		ScreenToClient(IS456MATSUBGRADEUNIT);

		CRect rSpecialProvision;
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(rSpecialProvision);
		int nOffsetY = rSpecialProvision.bottom + globalUtils.ScaleByDPI(20) - rSpecialProvision.top;

		// GetDlgItem(IDC_DGN_RCCTRL_TORSION)->SetWindowPos(NULL, rTorsion.left, rTorsion.top +nOffsetY, rTorsion.Width(), rTorsion.Height(), SWP_NOSIZE);
		GetDlgItem(IDC_DGN_IS456MAT_GROUP)->SetWindowPos(NULL, IS456MATGROUP.left, IS456MATGROUP.top + nOffsetY,
		                                                 IS456MATGROUP.Width(), IS456MATGROUP.Height(), SWP_NOSIZE);
		GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC)->SetWindowPos(NULL, IS456MATSTATTEXT1.left,
		                                                         IS456MATSTATTEXT1.top + nOffsetY,
		                                                         IS456MATSTATTEXT1.Width(), IS456MATSTATTEXT1.Height(),
		                                                         SWP_NOSIZE);
		GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR)->SetWindowPos(NULL, IS456MATSOILFACTOR.left,
		                                                             IS456MATSOILFACTOR.top + nOffsetY,
		                                                             IS456MATSOILFACTOR.Width(),
		                                                             IS456MATSOILFACTOR.Height(), SWP_NOSIZE);
		GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX)->SetWindowPos(
			NULL, IS456MATSTATTEXT2.left, IS456MATSTATTEXT2.top + nOffsetY, IS456MATSTATTEXT2.Width(),
			IS456MATSTATTEXT2.Height(), SWP_NOSIZE);
		GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_SUBGRDREXFACTOR)->SetWindowPos(
			NULL, IS456MATSUBGRDREXFACTOR.left, IS456MATSUBGRDREXFACTOR.top + nOffsetY, IS456MATSUBGRDREXFACTOR.Width(),
			IS456MATSUBGRDREXFACTOR.Height(), SWP_NOSIZE);
		GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX_MODULUS_UNIT)->SetWindowPos(
			NULL, IS456MATSUBGRADEUNIT.left, IS456MATSUBGRADEUNIT.top + nOffsetY, IS456MATSUBGRADEUNIT.Width(),
			IS456MATSUBGRADEUNIT.Height(), SWP_NOSIZE);

		// GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX_MODULUS_UNIT)->SetWindowText(CString(D_UNITSYS_BASE_DENSITY));

		GetDlgItem(IDC_DGN_IS456MAT_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_SUBGRDREXFACTOR)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX_MODULUS_UNIT)->ShowWindow(SW_SHOW);

		rDlg.bottom = RectOrg.bottom + (int)(2.5 * RectOk.Height() + iMrftHeight + iTorsionGrupHeight);
	}
#endif

	rMrft0.left = RectTorsionGoup.left;
	rMrft0.right = RectMrft0.right;
	rMrft0.top = rDlg.bottom - RectMrft0.Height() - nOffsetOK - RectOk.Height();
	rMrft0.bottom = rMrft0.top + RectMrft0.Height();

	rMrft1.left = RectMrft1.left;
	rMrft1.right = RectTorsionGoup.right;
	rMrft1.top = rMrft0.top;
	rMrft1.bottom = rMrft0.bottom;

	CRect rTrftSt, rTrft1, rTorsionGrup;

	const int nGapControlsY = globalUtils.ScaleByDPI(8);
	int nOffsetY = 0;
	if (CDBLib::IsConCodeForTorsionCheck(strCode)) // Seungjun '151126 PMS.5165 ACI318-08, 11 보 비틀림 설계 추가
	{
		if (bTWN_USD92)
		{
			//---------------------------------------
			// PMS:4230-HSSHIM-20101105
			int nMoveY = 0;
			//if (GetDlgItem(IDC_DGN_EQ_FACTOR20)->IsWindowVisible())    // Shear for Design Group Box
			if (m_bSpecialEQ)
			{
				CRect rcSrc(0, 0, 0, 0);
				CRect rcTgt(0, 0, 0, 0);

				GetDlgItem(IDC_DGN_SCWB_METHOD_GRP)->GetWindowRect(rcSrc);
				GetDlgItem(IDC_DGN_EQ_FACTOR15)->GetWindowRect(rcTgt);

				nMoveY = (rcTgt.bottom + nGapControlsY) - rcSrc.top;
				CDlgUtil::CtrlMoveDistY(this, m_aCtrlSCWBMethod, nMoveY);

				nOffsetY += rcSrc.Height() + nGapControlsY;
			}
			//---------------------------------------
		}

		int nRectLimitSingapore = bSingapore ? RectLimitStrg.Height() : 0;
		const int nOffsetIS = globalUtils.ScaleByDPI(14); /// 디버그에서 확인한 값을 토대로.

		rTorsion.left = RectTorsion.left;
		rTorsion.right = RectTorsion.right;
		rTorsion.top = rDlg.bottom - (RectTorsionGoup.Height() + nOffsetIS + RectMrft0.Height() + RectOk.Height() +
			nOffsetOK + nRectLimitSingapore);
		rTorsion.bottom = rTorsion.top + RectTorsion.Height();

		rTorsionGrup.left = RectTorsionGoup.left;
		rTorsionGrup.right = RectTorsionGoup.right;
		rTorsionGrup.top = rDlg.bottom - (RectTorsionGoup.Height() + nOffsetIS + RectMrft0.Height() + RectOk.Height() +
			nOffsetOK + nRectLimitSingapore);
		rTorsionGrup.bottom = rTorsionGrup.top + RectTorsionGoup.Height();

		rTrftSt.left = RectTrftSt.left;
		rTrftSt.right = RectTrftSt.right;
		rTrftSt.top = rDlg.bottom - (RectTrftSt.Height() + nOffsetIS + RectMrft0.Height() + RectOk.Height() + nOffsetOK
			+ nRectLimitSingapore);
		rTrftSt.bottom = rTrftSt.top + RectTrftSt.Height();

		rTrft1.left = RectTrft.left;
		rTrft1.right = RectTrft.right;
		rTrft1.top = rDlg.bottom - (RectTrft.Height() + nOffsetIS + RectMrft0.Height() + RectOk.Height() + nOffsetOK +
			nRectLimitSingapore);
		rTrft1.bottom = rTrft1.top + RectTrft.Height();

		ScreenToClient(rTorsion);
		ScreenToClient(rTorsionGrup);
		ScreenToClient(rTrftSt);
		ScreenToClient(rTrft1);

		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->SetWindowPos(NULL, rTorsion.left, rTorsion.top + nOffsetY, rTorsion.Width(),
		                                                 rTorsion.Height(), SWP_NOSIZE);
		GetDlgItem(IDC_DGN_TORSION_GROUP)->SetWindowPos(NULL, rTorsionGrup.left, rTorsionGrup.top + nOffsetY,
		                                                rTorsionGrup.Width(), rTorsionGrup.Height(), SWP_NOSIZE);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->SetWindowPos(NULL, rTrftSt.left, rTrftSt.top + nOffsetY,
		                                                     rTrftSt.Width(), rTrftSt.Height(), SWP_NOSIZE);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->SetWindowPos(NULL, rTrft1.left, rTrft1.top + nOffsetY, rTrft1.Width(),
		                                                     rTrft1.Height(), SWP_NOSIZE);
	}

	if (strCode == CONCODE_NSR_10 || strCode == CONCODE_NTC_DCEC_2017 || strCode == CONCODE_NTC_DCEC_2023)
	{
		GetDlgItem(IDC_DGN_EQ_ACI1)->SetWindowText(_LS(IDS_DGN_CODE_WALL_SHEAR_TYPE_DES));
		GetDlgItem(IDC_DGN_EQ_ACI2)->SetWindowText(_LS(IDS_DGN_CODE_WALL_SHEAR_TYPE_DMO));
		GetDlgItem(IDC_DGN_EQ_ACI3)->SetWindowText(_LS(IDS_DGN_CODE_WALL_SHEAR_TYPE_DMI));

		GetDlgItem(IDC_DGN_WALLTYPE_N12)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WALLTYPE_N13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WALLTYPE_N14)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DGN_EQ_ACI1)->SetWindowText(_LS(IDS_DGN_CODE_FRME_SHEAR_TYPE_SPM));
		GetDlgItem(IDC_DGN_EQ_ACI2)->SetWindowText(_LS(IDS_DGN_CODE_FRME_SHEAR_TYPE_IMF));
		GetDlgItem(IDC_DGN_EQ_ACI3)->SetWindowText(_LS(IDS_DGN_CODE_FRME_SHEAR_TYPE_OMF));

		GetDlgItem(IDC_DGN_WALLTYPE_N12)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WALLTYPE_N13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WALLTYPE_N14)->ShowWindow(SW_HIDE);
	}

	if ((strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 ||
			strCode == CONCODE_ACI318_14 || strCode == CONCODE_ACI318M_14 ||
			strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318M_19 ||
			strCode == CONCODE_ACI318_25 || strCode == CONCODE_ACI318M_25 ||
			strCode == CONCODE_NSCP_2015 ||
			(strCode == CONCODE_KCI_USD12 && m_iFrameType != 2) ||
			(strCode == CONCODE_KCI_USD07 && m_iFrameType != 2) ||
			(strCode == CONCODE_KDS_41_30_2018 && m_iFrameType != 2) ||
			(strCode == CONCODE_KDS_41_20_2022 && m_iFrameType != 2))
		&& m_bSpecialEQ)
	{
		GetDlgItem(IDC_DGN_WALLTYPE_N1)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DGN_WALLTYPE_N1)->ShowWindow(SW_HIDE);
	}

	rDlg.bottom += nBCOffset;
	rDlg.bottom += nOffsetY;

	ScreenToClient(rMrft0);
	ScreenToClient(rMrft1);

	if (bSingapore)
	{
		CRect rLimitStrg;
		rLimitStrg.left = RectLimitStrg.left;
		rLimitStrg.right = RectLimitStrg.right;
		rLimitStrg.top = rDlg.bottom - (int)(RectLimitStrg.Height() + 2.5 * RectMrft0.Height() + 2.5 * RectOk.Height());
		rLimitStrg.bottom = rLimitStrg.top + RectLimitStrg.Height();
		ScreenToClient(rLimitStrg);

		CRect rLimit;
		rLimit.top = rLimitStrg.top + nOffsetY + nBCOffset;
		rLimit.bottom = rLimit.top + rLimitStrg.Height();

		GetDlgItem(IDC_DGN_DGN_EUROCODE204_LIMIT_STRG_CHK)->SetWindowPos(
			NULL, rLimitStrg.left, rLimit.top, rLimitStrg.Width(), rLimitStrg.Height(), SWP_NOSIZE);
	}

	CRect rMRFT;
	rMRFT.top = rMrft0.top + nOffsetY + nBCOffset;
	rMRFT.bottom = rMRFT.top + rMrft0.Height();

	GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->SetWindowPos(NULL, rMrft0.left, rMRFT.top, rMrft0.Width(), rMrft0.Height(),
	                                                     SWP_NOSIZE);
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->SetWindowPos(NULL, rMrft1.left, rMRFT.top, rMrft1.Width(), rMrft1.Height(),
	                                                     SWP_NOSIZE);
	if (strCode == CONCODE_IS456_2000) //Pinakin Beam Torsion design
	{
		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->SetWindowPos(NULL, rMrft0.left, rMRFT.top + globalUtils.ScaleByDPI(30),
		                                                 rMrft0.Width(), rMrft0.Height(), SWP_NOSIZE);
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->SetWindowText(_T("Beam Torsion Design"));
		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->ShowWindow(SW_SHOW);

		bTWN_USD92 = TRUE;
		OnChangeTorsion();

		//IS3370 Only in Civil
#if defined(_CIVIL)
		GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(rRef);
		//For Consider Axial Moment Interaction for plate column
		CRect RectPMChk;
		GetDlgItem(IDC_DGN_PM_CURVE_CHK)->GetWindowRect(&RectPMChk);
		CRect rPMChk;
		CRect TorsionDesignPos;
		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->GetWindowRect(&TorsionDesignPos);

		rPMChk.left = RectPMChk.left;
		rPMChk.right = RectPMChk.right;
		rPMChk.top = TorsionDesignPos.bottom + globalUtils.ScaleByDPI(6);
		rPMChk.bottom = rPMChk.top + RectPMChk.Height();

		ScreenToClient(rPMChk);
		GetDlgItem(IDC_DGN_PM_CURVE_CHK)->MoveWindow(rPMChk.left, rPMChk.top, rPMChk.Width(), rPMChk.Height());
		//For IS3370 Internal Controls
		CRect r3370window;
		GetDlgItem(m_aCtrlIS3370_GrpA[0])->GetWindowRect(r3370window);
		int nMoveY = (rRef.bottom + rMRFT.bottom + globalUtils.ScaleByDPI(30)) - r3370window.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlIS3370, nMoveY);
		CDlgUtil::CtrlShowHide(this, m_aCtrlIS3370, TRUE);

		rDlg.bottom += r3370window.Height() + globalUtils.ScaleByDPI(70);

		//GetDlgItem(IDC_DGN_MB9_GRP2)->SetWindowPos(NULL, rMrft0.left, rMRFT.top+rTorsion.bottom+5., rMrft0.Width(), rMrft0.Height(), SWP_NOSIZE);
		//GetDlgItem(IDC_DGN_MB9_GRP2)->ShowWindow(SW_SHOW);
#endif
	}
	else if (strCode == CONCODE_IRS)
	{
		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->SetWindowPos(NULL, rMrft0.left, rMRFT.top + globalUtils.ScaleByDPI(30),
		                                                 rMrft0.Width(), rMrft0.Height(), SWP_NOSIZE);
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->SetWindowText(_T("Beam Torsion Design"));
		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->ShowWindow(SW_SHOW);

		bTWN_USD92 = TRUE;
		OnChangeTorsion();
		rDlg.bottom += globalUtils.ScaleByDPI(24);
	}
	// PMS:DNKIM-20121030
	BOOL bEurocode204 = (strCode == CONCODE_EC2_04) ? TRUE : FALSE;
	if (bEurocode204)
	{
		int nMove = globalUtils.ScaleByDPI(m_bSpecialEQ ? 10 : 5);
		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->SetWindowPos(NULL, rMrft0.left,
		                                                 rMRFT.top - rTorsion.Height() - nMove - nLimitStrg,
		                                                 rTorsion.Width(), rTorsion.Height(), SWP_NOSIZE);
	}

	CRect rUseVcGroup;
	GetDlgItem(IDC_DGN_EUROCODE204_USEVC)->GetWindowRect(rUseVcGroup);
	ScreenToClient(rUseVcGroup);

	int nMargin = globalUtils.ScaleByDPI(8);
	int nMoveY = (rMRFT.bottom + nMargin) - rUseVcGroup.top;
	CDlgUtil::CtrlMoveDistY(this, m_aUseVc, nMoveY);
	CDlgUtil::CtrlShowHide(this, m_aUseVc, bEurocode204);

	// Exposure Condition
	BOOL bExposure = FALSE;
#if !defined(_CIVIL)
	bExposure = (strCode == CONCODE_KCI_USD12 || strCode == CONCODE_KDS_41_30_2018 || strCode == CONCODE_KDS_41_20_2022)
		            ? TRUE
		            : FALSE;
#endif

	CRect rExposure;
	GetDlgItem(IDC_DGN_EXPOSURE_STC)->GetWindowRect(rExposure);
	ScreenToClient(rExposure);

	nMoveY = (rMRFT.bottom + nMargin) - rExposure.top;
	CDlgUtil::CtrlMoveDistY(this, m_aExposure, nMoveY);
	CDlgUtil::CtrlShowHide(this, m_aExposure, bExposure);

	// RUSSIA - BridgeType
	BOOL bRussia = FALSE;
#if defined(_CIVIL)
	if (strCode == CONCODE_SNIP_2_05_03_84) bRussia = TRUE;
	else if (strCode == CONCODE_SP_35_13330_2011) bRussia = TRUE;
	else if (strCode == CONCODE_SNIP_2_05_03_84_MKS) bRussia = TRUE;
	else if (strCode == CONCODE_SP_35_13330_2011_MKS) bRussia = TRUE;
	else bRussia = FALSE;
#endif
	if (bRussia)
	{
		GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_BRIDGE_TYPE_CMB)->GetWindowRect(rMove);
		int nMoveX = rRef.right - rMove.right;
		nMoveY = (rRef.bottom + nMargin) - rMove.top;
		CArray<UINT, UINT> aCmb;
		aCmb.RemoveAll();
		aCmb.Add(IDC_DGN_BRIDGE_TYPE_CMB);
		CDlgUtil::CtrlMoveDistX(this, aCmb, nMoveX);
		CDlgUtil::CtrlMoveDistY(this, aCmb, nMoveY);

		GetDlgItem(IDC_DGN_BRIDGE_TYPE_STC)->GetWindowRect(rMove);
		nMoveY = (rRef.bottom + nMargin) - rMove.top;
		aCmb.RemoveAll();
		aCmb.Add(IDC_DGN_BRIDGE_TYPE_STC);
		CDlgUtil::CtrlMoveDistY(this, aCmb, nMoveY);

		GetDlgItem(IDC_DGN_BRIDGE_TYPE_CMB)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_MB7_MB8_GRP)->GetWindowRect(rMove);
		nMoveY = (rRef.bottom + nMargin) - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCondiFactor, nMoveY);

		GetDlgItem(IDC_DGN_MB9_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(rMove);
		nMoveY = (rRef.bottom + nMargin) - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aSpecialProvision, nMoveY);

		CArray<UINT, UINT> aControls;
		aControls.RemoveAll();
		aControls.Add(IDC_DGN_RCCTRL_MRFT_STATIC);
		aControls.Add(IDC_DGN_RCCTRL_MRFT_FACTOR);

		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->GetWindowRect(rMove);
		nMoveY = (rRef.bottom + nMargin) - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, aControls, nMoveY);

		CDlgUtil::CtrlShowHide(this, m_aCondiFactor, bRussia);
		GetDlgItem(IDC_DGN_BRIDGE_TYPE_STC)->ShowWindow(bRussia);
		GetDlgItem(IDC_DGN_BRIDGE_TYPE_CMB)->ShowWindow(bRussia);
	}

	CRect rDgnComb;
	GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(rDgnComb);
	ScreenToClient(rDgnComb);

	if (bEurocode204)
	{
		rDlg.bottom += rUseVcGroup.Height() + nMargin;
	}

	if (bExposure)
	{
		rDlg.bottom += rExposure.Height() + nMargin;
	}

	if (bRussia)
	{
		GetDlgItem(IDC_DGN_MB7_MB8_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->GetWindowRect(rMove);
		nMoveY = rMove.bottom - rRef.top;
		rDlg.bottom += nMoveY - nMargin;
	}

	if (strCode == CONCODE_KSCE_USD10 || strCode == CONCODE_KSCE_RAIL_USD11 || strCode == CONCODE_KCI_USD12 || 
		strCode == CONCODE_KSCE_LSD15 || strCode == CONCODE_KDS_24_14_21_2022 || strCode == CONCODE_KDS_14_20_00_2022)
	{
		rDlg.bottom += rDgnComb.Height() + nMargin;
	}
	else if (strCode == CONCODE_SP_35_13330_2011 || strCode == CONCODE_SP_35_13330_2011_MKS ||
		strCode == CONCODE_SNIP_2_05_03_84 || strCode == CONCODE_SNIP_2_05_03_84_MKS)
	{
		rDlg.bottom -= globalUtils.ScaleByDPI(14);
	}
	else if (strCode == CONCODE_IS456_2000)
	{
		rDlg.bottom -= globalUtils.ScaleByDPI(10);
	}

	CRect rOk;
	rOk.left = RectOk.left;
	rOk.right = RectOk.right;
	rOk.top = rDlg.bottom - RectOk.Height() - globalUtils.ScaleByDPI(8);
	rOk.bottom = rOk.top + RectOk.Height();

	CRect rCn;
	rCn.left = RectCn.left;
	rCn.right = RectCn.right;
	rCn.top = rOk.top;
	rCn.bottom = rOk.bottom;

	ScreenToClient(rDlg);
	ScreenToClient(rOk);
	ScreenToClient(rCn);
	SetWindowPos(NULL, rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDOK)->SetWindowPos(NULL, rOk.left, rOk.top, rOk.Width(), rOk.Height(), SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rCn.left, rCn.top, rCn.Width(), rCn.Height(), SWP_NOSIZE);

	Invalidate();

	ChangeDlgItemInfo();
}

void CDgnConCodeDlg::OnSelchangeDgnCtrldataDesigncode()
{
	UpdateData(TRUE);

	int Index = m_DesignCode.GetCurSel();
	if (m_aCodeEnable[Index] == FALSE)
	{
		CString strCodeName = _T("");
		m_DesignCode.GetLBText(Index, strCodeName);
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), strCodeName);
		GSaveHistoryFormatNF(strErrMsg);
		m_DesignCode.SetCurSel(m_nOldCodeIndex);

		return;
	}

	m_nOldCodeIndex = Index;

	SelchangeDgnCtrldataDesigncode(TRUE);
	EnableWindowBySpecialWall();
	UpdateData(FALSE);

	//   CString strCode=_T("");
	//   m_DesignCode.GetWindowText(strCode);
	//   int iStatus	= GetDataStatus(strCode);
	//   if(strCode == _T("Eurocode2:04"))//_T("Eurocode2:04")
	//   {
	//     UpdateData(TRUE);
	//     if(m_bSpecialEQ)
	//       GetDlgItem(IDC_DGN_EC8_BTN)->EnableWindow(TRUE);
	//     else
	//       GetDlgItem(IDC_DGN_EC8_BTN)->EnableWindow(FALSE);
	//   }
}

void CDgnConCodeDlg::OnSelchangeNTCType()
{
	CString strCode = _T("");
	m_DesignCode.GetWindowText(strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	strCode = CDBLib::GetConvertCodeName(strCode);

	BOOL bNTCon = m_chkApplyNTC.GetCheck() == TRUE ? TRUE : FALSE;
	int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());

	BOOL bAfterNTC2012 = nNTCType == RCS_NTC08 ? FALSE : TRUE;
	int nCmdDraft = bNTCon && nNTCType == RCS_NTC12 ? SW_SHOW : SW_HIDE;
	if (bAfterNTC2012 && bNTCon == TRUE)
	{
		CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrls, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrlsNTC2012, TRUE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrls, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aSlendernessCtrlsNTC2012, FALSE);
		GetDlgItem(IDC_DGN_SLENDERNESS_LIMIT_STC)->ShowWindow(SW_SHOW);
	}

	GetDlgItem(IDC_DGN_NTC2012_DRAFT_STATIC)->ShowWindow(nCmdDraft);

	// MQC 15839 : Eurocode2:04, NTC2008인 경우 A=0.7, B=1.1로 값을 고정해서 비활성화
	const BOOL bNTC2008 = nNTCType == RCS_NTC08 && bNTCon == TRUE;
	if (bNTC2008)
	{
		m_edtSlendLimitA.SetEditUnit(0.7);
		m_edtSlendLimitB.SetEditUnit(1.1);
		m_chkAutoCalcA.SetCheck(FALSE);
	}
	GetDlgItem(IDC_DGN_SLENDERNESS_A_EDT)->EnableWindow(bNTC2008 ? FALSE : !m_chkAutoCalcA.GetCheck());
	GetDlgItem(IDC_DGN_SLENDERNESS_B_EDT)->EnableWindow(!bNTC2008);
	GetDlgItem(IDC_DGN_SLENDERNESS_AUTO_A_CHK)->EnableWindow(!bNTC2008);

	if (m_bSpecialEQ)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aBCJointGroup, TRUE);
		m_chkApplyBCJDesign.SetCheck(TRUE);
	}
	//   else if (!m_bSpecialEQ && bNTCon == TRUE && bAfterNTC2012)
	//   {
	//     CDlgUtil::CtrlEnableDisable(this, m_aBCJointGroup, TRUE);
	//     m_chkApplyBCJDesign.SetCheck(TRUE);
	//   }
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_aBCJointGroup, FALSE);
		m_chkApplyBCJDesign.SetCheck(FALSE);
	}

	OnDuctilityBtn();
}

void CDgnConCodeDlg::OnSelchangeNationalAnnexCombo()
{
	SelchangeDgnCtrldataDesigncode(FALSE);
	SetNTC2008EnableControl();
}

void CDgnConCodeDlg::OnSelchangeDgnCtrldataDesignAnnexAashto()
{
	// 특별한 동작 없음.
}

void CDgnConCodeDlg::OnUpdateByCode()
{
	UpdateData(TRUE);

	CString strCode = _T("");
	m_DesignCode.GetWindowText(strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	strCode = CDBLib::GetConvertCodeName(strCode);
	m_iAlphaType1 = 0;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dConShearStrengthRatio = 0.0;

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);

	ChangeDlgItemInfo();

	UpdateData(FALSE);
}

void CDgnConCodeDlg::OnDuctilityBtn()
{
	UpdateData(TRUE);

	if (m_iDuctility == 0)
	{
		if (!m_bExistData) // PMS:xxxx-Seungjun-20100422 저장된 데이터가 있는 경우 초기값 세팅하지 않음.
		{
			int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());
			if ((nNTCType == RCS_NTC12 || nNTCType == RCS_NTC18) && m_chkApplyNTC.GetCheck() == TRUE)
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
		if (!m_bExistData) // PMS:xxxx-Seungjun-20100422 저장된 데이터가 있는 경우 초기값 세팅하지 않음.
		{
			int nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());
			if ((nNTCType == RCS_NTC12 || nNTCType == RCS_NTC18) && m_chkApplyNTC.GetCheck() == TRUE)
			{
				m_dGamma_rdForBeam = 1.1;
				m_dGamma_rdForColm = 1.1;
				m_dGamma_rdForWall = 1.2;
				m_dGamma_rdForJoint = 1.1;
			}
			else
			{
				m_dGamma_rdForBeam = 1.0;
				m_dGamma_rdForColm = 1.1;
				m_dGamma_rdForWall = 1.2;
				m_dGamma_rdForJoint = 1.2;
			}
		}

		GetDlgItem(IDC_DGN_EUROCODE204_N8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EUROCODE204_N21)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}

CRect CDgnConCodeDlg::MoveCtrl(int iKind, int* pID, CRect Rect0)
{
	CRect RectOrg, SDRHideRect, SDRShowRect;
	int iArSize = 0;
	if (iKind == 1) iArSize = iDgn_ConID1;
	else if (iKind == 2) iArSize = iDgn_ConID2;
	else if (iKind == 3) iArSize = iDgn_ConID3;
	else if (iKind == 4) iArSize = iDgn_ConID4;
	else if (iKind == 6) iArSize = iDgn_ConID6;
	else if (iKind == 7) iArSize = iDgn_ConID6;
	else if (iKind == 8) iArSize = iDgn_ConID6;
	else if (iKind == 9) iArSize = iDgn_ConID7;
	else if (iKind == 10)iArSize = iDgn_ConID8;
	else if (iKind == 11)iArSize = iDgn_ConID9; //Eurocode2:02
	else if (iKind == 12)iArSize = iDgn_ConID10; //KCI-USD07
	else if (iKind == 13)iArSize = iDgn_ConID10_part; //KCI-USD07
	else if (iKind == 14)iArSize = iDgn_ConID11; //AASHTO-LRFD07
	else if (iKind == 15)iArSize = iDgn_ConID12; //Indian IS13920 Ductile Code
	else
		ASSERT(0);

	CRect* pRectCng = new CRect[iArSize];

	CRect Rect1;
	for (int i = 0; i < iArSize; i++)
	{
		GetDlgItem(pID[i])->ShowWindow(SW_SHOW);
		GetDlgItem(pID[i])->GetWindowRect(&(pRectCng[i]));
		if (i == 0) Rect1 = pRectCng[i];
		pRectCng[i].top -= Rect1.top - Rect0.top;
		pRectCng[i].bottom -= Rect1.top - Rect0.top;
		pRectCng[i].left -= Rect1.left - Rect0.left;
		pRectCng[i].right -= Rect1.left - Rect0.left;
	}

	if (iKind == 9)
	{
		SDRHideRect = pRectCng[18];
		SDRShowRect = pRectCng[19];

		CString strCode = _T("");
		m_DesignCode.GetWindowText(strCode);
#if defined(_RUS)
		CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
		strCode = CDBLib::GetConvertCodeName(strCode);
		int iStatus = GetDataStatus(strCode);

		if ((iStatus == 10 && m_iFrameType == 0) || (iStatus == 18 && m_iFrameType == 0) || (iStatus == 20 &&
			m_iFrameType == 0) || iStatus == 15)
			pRectCng[0].bottom = SDRShowRect.top;
		else
			pRectCng[0].bottom = SDRHideRect.top;
	}
	RectOrg = pRectCng[0];

	for (int i = 0; i < iArSize; i++)
	{
		ScreenToClient(pRectCng[i]);
		GetDlgItem(pID[i])->SetWindowPos(NULL, pRectCng[i].left, pRectCng[i].top, pRectCng[i].Width(),
		                                 pRectCng[i].Height(), SWP_NOSIZE | SWP_NOZORDER);
	}

	if (iKind == 9)
	{
		SDRControl();
		/*
		CString strCode=_T("");
		m_DesignCode.GetWindowText(strCode);
		int iStatus	= GetDataStatus(strCode);

		if((iStatus==10 && m_iFrameType==0) || iStatus==15)
			RectOrg.bottom = SDRShowRect.top;
		else
			RectOrg.bottom = SDRHideRect.top;		*/

		/*
		CRect GroupRect = RectOrg;
		ScreenToClient(GroupRect);
		GetDlgItem(pID[0])->SetWindowPos(NULL, GroupRect.left, GroupRect.top, GroupRect.Width(), GroupRect.Height(), SWP_NOSIZE | SWP_NOZORDER);*/
	}

	delete[] pRectCng;
	return RectOrg;
}

void CDgnConCodeDlg::SDRControl()
{
	GetDlgItem(IDC_DGN_EQ_FACTOR15)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EQ_FACTOR16)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EQ_FACTOR17)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EQ_FACTOR18)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EQ_FACTOR19)->ShowWindow(SW_HIDE);

	CArray<UINT, UINT> arCtrl;
	static CArray<UINT, UINT> arCtrlSDR;
	static BOOL bFirst = TRUE;

	if (bFirst)
	{
		arCtrlSDR.RemoveAll();
		CDlgUtil::GetCtrlIDByIncRect(this, arCtrl, IDC_DGN_EQ_FACTOR15, FALSE);
		for (int i = 0; i < arCtrl.GetSize(); ++i)
		{
			if (arCtrl[i] == IDC_DGN_EQ_FACTOR11 ||
				arCtrl[i] == IDC_DGN_EQ_FACTOR1 ||
				arCtrl[i] == IDC_DGN_EQ_FACTOR2 ||
				arCtrl[i] == IDC_DGN_EQ_FACTOR3 ||
				arCtrl[i] == IDC_DGN_EQ_FACTOR4 ||
				arCtrl[i] == IDC_DGN_EQ_FACTOR5 ||
				arCtrl[i] == IDC_DGN_EQ_FACTOR6 ||
				arCtrl[i] == IDC_DGN_EQ_FACTOR7 ||
				arCtrl[i] == IDC_DGN_EQ_FACTOR8 ||
				arCtrl[i] == IDC_DGN_EQ_FACTOR9 ||
				arCtrl[i] == IDC_DGN_EQ_FACTOR10)
				arCtrlSDR.Add(arCtrl[i]);
		}
		bFirst = FALSE;
	}

	CWnd* pSDR = GetDlgItem(IDC_DGN_EQ_FACTOR11);
	CWnd* pSDRHide = GetDlgItem(IDC_DGN_EQ_FACTOR16);
	CWnd* pSDRShow = GetDlgItem(IDC_DGN_EQ_FACTOR17);
	CRect SDRRect, SDRHideRect, SDRShowRect;
	pSDR->GetWindowRect(SDRRect);
	pSDRHide->GetWindowRect(SDRHideRect);
	pSDRShow->GetWindowRect(SDRShowRect);

	CString strCode = _T("");
	m_DesignCode.GetWindowText(strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	strCode = CDBLib::GetConvertCodeName(strCode);
	int iStatus = GetDataStatus(strCode);

	int DY_SDR = 0;

	if ((iStatus == 10 && m_iFrameType == 0) || (iStatus == 18 && m_iFrameType == 0) || (iStatus == 20 && m_iFrameType
		== 0) || iStatus == 15)
	{
		DY_SDR = SDRShowRect.top - SDRRect.top;
		GetDlgItem(IDC_DGN_EQ_FACTOR0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_EQ_FACTOR20)->ShowWindow(SW_SHOW);
	}
	else
	{
		DY_SDR = SDRHideRect.top - SDRRect.top;
		GetDlgItem(IDC_DGN_EQ_FACTOR0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_EQ_FACTOR20)->ShowWindow(SW_HIDE);
	}

	CDlgUtil::CtrlMoveDistY(this, arCtrlSDR, DY_SDR, TRUE);

	/*
	CRect GroupRect, EditRect, TestRect;
	GetDlgItem(IDC_DGN_EQ_FACTOR0)->GetWindowRect(GroupRect);
	GetDlgItem(IDC_DGN_EQ_FACTOR10)->GetWindowRect(EditRect);
	GetDlgItem(IDC_DGN_TEST)->GetWindowRect(TestRect);

	TestRect.top = GroupRect.bottom;
	TestRect.bottom = EditRect.bottom + 5;
	TestRect.left = GroupRect.bottom;
	TestRect.right = GroupRect.bottom;
	ScreenToClient(TestRect);
	GetDlgItem(IDC_DGN_TEST)->SetWindowPos(NULL, TestRect.left, TestRect.top, TestRect.Width(), TestRect.Height(), SWP_NOSIZE | SWP_NOZORDER);
	GetDlgItem(IDC_DGN_TEST)->ShowWindow(SW_SHOW);	*/
}

void CDgnConCodeDlg::ChangeDlgItemInfo()
{
	CString strCode = _T("");
	int iIndex = m_DesignCode.GetCurSel();
	if (iIndex >= 0) m_DesignCode.GetLBText(iIndex, strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	strCode = CDBLib::GetConvertCodeName(strCode);

	CString strTxt = _T("");
	int iStatus = GetDataStatus(strCode);
	// Coded by SeungJun MNET:3433  '20080515

	GetDlgItem(IDC_DGN_EC8_BTN)->ShowWindow(SW_HIDE);

	if (strCode == CONCODE_IS456_2000)
	{
		//strTxt = _LS(IDS_DGN_TEXT_APPLY_IS13920_DGN);
		strTxt = _LS(IDS_DGN_TEXT_APPLY_SEISMIC_DGN);
		GetDlgItem(IDC_DGN_IS204_N25)->SetWindowText(_LS(IDS_DGN_TEXT_APPLY_IS_DUCT_DGN));
		GetDlgItem(IDC_DGN_IS204_N26)->SetWindowText(_LS(IDS_DGN_TEXT_APPLY_IS13920_DGN));
		GetDlgItem(IDC_DGN_IS204_N27)->SetWindowText(_LS(IDS_DGN_TEXT_APPLY_IS13920_2016_DGN));
	}
	else
	{
		if (iStatus == 5)
		{
			strTxt = _LS(IDS_DGN_TEXT_CHECKCODE_JTJ);
		}
		else if (strCode == CONCODE_EC2_04)
		{
			GetDlgItem(IDC_DGN_EC8_BTN)->ShowWindow(SW_SHOW);
			BOOL bEnable = m_chkApplyNTC.GetCheck();
			if (bEnable == TRUE)
			{
				strTxt = _LS(IDS_DGN_TEXT_APPLY_SEISMIC_DGN);
			}
			else
			{
				strTxt = _LS(IDS_DGN_TEXT_APPLY_EC8_04_CAPACITY_DGN); //_T("Apply EC8:04 Capacity Design")
			}
		}
		else
		{
			strTxt = _LS(IDS_DGN_TEXT_APPLY_SEISMIC_DGN);
		}
	}
	GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->SetWindowText(strTxt);
	// Add by ZINU.('03.02.25). For NON-Seismic Code.
	if (iStatus < 0)
	{
		m_bSpecialEQ = FALSE;
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->EnableWindow(TRUE);
	}
#if defined(_CIVIL)
	if (strCode == CONCODE_IS456_2000)
	{
		CDlgUtil::CtrlEnableDisable(this, IDC_DGN_RCCTRL_SPECIAL, FALSE);
	}
#endif
	if (iStatus == 6 || iStatus == 7 || iStatus == 8 || iStatus == 19)
	{
		for (int i = 0; i < iDgn_ConID6; i++)
		{
			CString strName = _T("");
			CString strText = _T("");
			if (iStatus == 6 || iStatus == 19) strName = _LS(IDS_DGN_TEXT_SEISMIC_ZONE_AASHTO);
			else if (iStatus == 7) strName = _LS(IDS_DGN_TEXT_SEISMIC_ZONE_CSA);
			else if (iStatus == 8) strName = _LS(IDS_DGN_TEXT_SEISMIC_ZONE_AASHTO96);
			else
				ASSERT(0);
			if (i == 0) strText.Format(_T("Select %s"), strName);
			else
			{
				if (iStatus == 8)
				{
					CString strGrade = _T("");
					if (i == 1) strGrade = _T("A");
					else if (i == 2) strGrade = _T("B");
					else if (i == 3) strGrade = _T("C");
					else if (i == 4) strGrade = _T("D");
					strText.Format(_T("%s %s"), strName, strGrade);
				}
				else strText.Format(_T("%s %d"), strName, i);
			}
			GetDlgItem(m_iID6[i])->SetWindowText(strText);
		}
	}

#if defined(_MGEN)
	if (iStatus == 9 || iStatus == 10 || iStatus == 11 || iStatus == 12 || iStatus == 13 ||
		iStatus == 14 || iStatus == 15 || iStatus == 16 || iStatus == 18 || iStatus == 20)
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(FALSE);

		if (m_iAlphaType1 == 0 || m_iAlphaType1 == 1)
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(TRUE);
		}
		else if (m_iAlphaType1 == 2)
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(TRUE);
		}
	}

	CString strQ = _T("");
	if (iStatus == 9)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_AIJ));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_AIJ));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_AIJ));
	}
	else if (iStatus == 10 || iStatus == 18 || iStatus == 20)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_ACI));
		if (m_iFrameType == 0) //Speical
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_SPECIAL_ACI));
			GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_ACI));
		}
		else //Intermediate, Ordinary
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_ACI));
			GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_ACI));
		}
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_ACI));
	}
	else if (iStatus == 12)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_KSCE));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_KSCE));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_KSCE));
	}
	else if (iStatus == 11 || iStatus == 13 || iStatus == 14)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_ACI));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_ACI));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_ACI));
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_ACI));
	}
	else if (iStatus == 15)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_TWN));
	}
	else if (iStatus == 16)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_IS));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_IS));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_IS));
	}

	if (strQ != _T(""))
	{
		CString strMax = _T(""), strMin = _T(""), strQ1 = _T(""), strQ2 = _T("");
		strQ1.Format(_T("%s1"), strQ);
		strQ2.Format(_T("%s2"), strQ);
		strMax.Format(_T("MAX(%s,%s)"), strQ1, strQ2);
		strMin.Format(_T("MIN(%s,%s)"), strQ1, strQ2);
		GetDlgItem(IDC_DGN_EQ_FACTOR1)->SetWindowText(strMax);
		GetDlgItem(IDC_DGN_EQ_FACTOR2)->SetWindowText(strMin);
		GetDlgItem(IDC_DGN_EQ_FACTOR3)->SetWindowText(strQ1);
		GetDlgItem(IDC_DGN_EQ_FACTOR4)->SetWindowText(strQ2);
	}

	// Add by ZINU.('08.03.20). NO:3292, Multiply R by Vc (Vu1 > 0.5*Vu2).
	// ACI318-Series only Special Moment Frame, TWN-USD92.
	if ((iStatus == 10 && m_iFrameType == 0) || (iStatus == 18 && m_iFrameType == 0) || (iStatus == 20 && m_iFrameType
		== 0) || iStatus == 15)
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_EQ_FACTOR14)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_EQ_FACTOR21)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_EQ_FACTOR14)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_EQ_FACTOR21)->ShowWindow(SW_HIDE);
	}

	//SDRControl();

#endif
	//CDlgUtil::CtrlEnableDisable(this,IDC_DGN_RCCTRL_SPECIAL,FALSE);
}

int CDgnConCodeDlg::GetDataStatus(CString strCode)
{
	int iStatus = 0;
	// Add by ZINU.('02.9.26). To calculate PM Curve by neutral NODE or AXIS (Only Korean USD Code).
	BOOL bKoreanUSD = FALSE;
	// Change by ZINU.('02.7.8). For AIJ-WSD99, GB50010-02.
	if (strCode == CONCODE_AIJ_WSD99) iStatus = (m_bSpecialEQ ? 9 : 1);
	else if (strCode == CONCODE_GB50010_02) iStatus = (m_bSpecialEQ ? 3 : 2);
	else if (strCode == CONCODE_GB50010_10) iStatus = (m_bSpecialEQ ? 3 : 2);
	else if (bKoreanUSD) iStatus = 4;
	else if (strCode == CONCODE_JTJ023_85) iStatus = 5;
	else if (strCode == CONCODE_AASHTO_LRFD02) iStatus = (m_bSpecialEQ ? 6 : 0);
	else if (strCode == CONCODE_AASHTO_LRFD07) iStatus = (m_bSpecialEQ ? 19 : 0);
	else if (strCode == CONCODE_AASHTO_LRFD12) iStatus = (m_bSpecialEQ ? 19 : 0);
	else if (strCode == CONCODE_AASHTO_LRFD16) iStatus = (m_bSpecialEQ ? 19 : 0);
	else if (strCode == CONCODE_AASHTO_LRFD17) iStatus = (m_bSpecialEQ ? 19 : 0);
	else if (strCode == CONCODE_AASHTO_LRFD20) iStatus = (m_bSpecialEQ ? 19 : 0);
	else if (strCode == CONCODE_AASHTO_LRFD24) iStatus = (m_bSpecialEQ ? 19 : 0);
	else if (strCode == CONCODE_AREMA_2023) iStatus = -1;
	else if (strCode == CONCODE_CSA_S6_00) iStatus = (m_bSpecialEQ ? 7 : 0);
	else if (strCode == CONCODE_CSA_S6_14) iStatus = (m_bSpecialEQ ? 7 : 0);
	else if (strCode == CONCODE_CSA_S6_19) iStatus = (m_bSpecialEQ ? 7 : 0);
	else if (strCode == CONCODE_AASHTO_LFD96) iStatus = (m_bSpecialEQ ? 8 : 0);
	else if (strCode == CONCODE_BS8110_97) iStatus = -1; // Non-seismic.
	else if (strCode == CONCODE_EC2) iStatus = -1; // Non-seismic.
	else if (strCode == CONCODE_EC2_04) iStatus = /*17;*/(m_bSpecialEQ ? 17 : 0); // Coded by sshan ('20080108)
	else if (strCode == CONCODE_IRC21_2000) iStatus = -1; // Non-seismic.
	else if (strCode == CONCODE_IRC112_2011) iStatus = -1; // Non-seismic.
	else if (strCode == CONCODE_IRC112_2020) iStatus = -1; // Non-seismic.
	else if (strCode == CONCODE_AS5100_5_17) iStatus = -1; // Non-seismic.
	else if (strCode == CONCODE_ACI318_89) iStatus = (m_bSpecialEQ ? 10 : 0);
	else if (strCode == CONCODE_ACI318_95) iStatus = (m_bSpecialEQ ? 10 : 0);
	else if (strCode == CONCODE_ACI318_99) iStatus = (m_bSpecialEQ ? 10 : 0);
	else if (strCode == CONCODE_ACI318_02) iStatus = (m_bSpecialEQ ? 10 : 0);
	else if (strCode == CONCODE_ACI318_05) iStatus = (m_bSpecialEQ ? 10 : 0);
	else if (strCode == CONCODE_ACI318_08) iStatus = (m_bSpecialEQ ? 20 : 0); // Add by GAY. PMS:4450. ('12.03.12)
	else if (strCode == CONCODE_ACI318_11) iStatus = (m_bSpecialEQ ? 20 : 0); // Add by GAY. PMS:4450. ('12.03.12)
	else if (strCode == CONCODE_ACI318_14) iStatus = (m_bSpecialEQ ? 20 : 0);
	else if (strCode == CONCODE_ACI318M_14) iStatus = (m_bSpecialEQ ? 20 : 0);
	else if (strCode == CONCODE_ACI318_19) iStatus = (m_bSpecialEQ ? 20 : 0);
	else if (strCode == CONCODE_ACI318M_19) iStatus = (m_bSpecialEQ ? 20 : 0);
	else if (strCode == CONCODE_ACI318_25) iStatus = (m_bSpecialEQ ? 20 : 0);
	else if (strCode == CONCODE_ACI318M_25) iStatus = (m_bSpecialEQ ? 20 : 0);
	else if (strCode == CONCODE_NSR_10) iStatus = (m_bSpecialEQ ? 20 : 0); // by GAY. PMS:5009. ('15.04.03).
	else if (strCode == CONCODE_NSCP_2015) iStatus = (m_bSpecialEQ ? 20 : 0);
	else if (strCode == CONCODE_NTC_DCEC_2017) iStatus = (m_bSpecialEQ ? 20 : 0);
	else if (strCode == CONCODE_NTC_DCEC_2023) iStatus = (m_bSpecialEQ ? 20 : 0);
	else if (strCode == CONCODE_AIK_USD94) iStatus = (m_bSpecialEQ ? 11 : 0);
	else if (strCode == CONCODE_KSCE_USD96) iStatus = (m_bSpecialEQ ? 12 : 0);
	else if (strCode == CONCODE_KSCE_USD05) iStatus = -1; // Non-seismic.
	else if (strCode == CONCODE_KSCE_RAIL_USD04) iStatus = -1; // Non-seismic.
	else if (strCode == CONCODE_KCI_USD99) iStatus = (m_bSpecialEQ ? 13 : 0);
	else if (strCode == CONCODE_KCI_USD03) iStatus = (m_bSpecialEQ ? 13 : 0);
	else if (strCode == CONCODE_KCI_USD07 || strCode == CONCODE_KCI_USD12 ||
		strCode == CONCODE_KDS_41_30_2018 || strCode == CONCODE_KDS_41_20_2022)
	{
#if defined(_CIVIL)
		iStatus = -1; // Non-seismic.
#else
		iStatus = (m_bSpecialEQ ? 18 : 0);
#endif
	}
	else if (strCode == CONCODE_CSA_A23_3_94) iStatus = (m_bSpecialEQ ? 14 : 0);
	else if (strCode == CONCODE_TWN_USD92) iStatus = (m_bSpecialEQ ? 15 : 0);
	else if (strCode == CONCODE_TWN_USD100) iStatus = (m_bSpecialEQ ? 15 : 0);
	else if (strCode == CONCODE_TWN_USD112) iStatus = (m_bSpecialEQ ? 15 : 0);
	else if (strCode == CONCODE_IS456_2000) iStatus = (m_bSpecialEQ ? 16 : 0);
	else if (strCode == CONCODE_EC2_2_05) iStatus = -1; // Non-seismic.
	else if (strCode == CONCODE_SNIP_2_05_03_84) iStatus = -1; // Non-seismic.
	else if (strCode == CONCODE_SP_35_13330_2011)iStatus = -1; // Non-seismic.
	else if (strCode == CONCODE_SNIP_2_05_03_84_MKS)iStatus = -1; // Non-seismic.
	else if (strCode == CONCODE_SP_35_13330_2011_MKS)iStatus = -1; // Non-seismic.
	else iStatus = 0;

	// MNET:XXXX-HSSHIM-20111129
	// iGen에서는 일부 Control을 감춘다.
	// 계산정보 대화상자에서 따로 설정할 수 있기 때문에 단일화 시키기 위함.
#if defined(_IGEN)
	if (strCode == CONCODE_AIJ_WSD99)
	{
		iStatus = 0;
	}
#endif

	return iStatus;
}

void CDgnConCodeDlg::OnSetGrupCombo()
{
	m_cmbGroup.ResetContent();
	m_cmbGroup.SetItemData(m_cmbGroup.AddString(_T("None")), 0);
	m_cmbGroup.SetCurSel(0);

	CArray<T_GRUP_K, T_GRUP_K> aKey;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupKeyList(aKey);
	int nCount = aKey.GetSize();
	if (nCount < 1) return;

	T_GRUP_D Data;
	int ixInit = 0;
	for (int i = 0; i < nCount; ++i)
	{
		if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(aKey[i], Data)) continue;
		int ix = m_cmbGroup.AddString(Data.GroupName);
		if (aKey[i] == m_GroupKey) ixInit = ix; // 초기값
		m_cmbGroup.SetItemData(ix, aKey[i]);
	}
	m_cmbGroup.SetCurSel(ixInit);
}

void CDgnConCodeDlg::OnBtnGrup()
{
	// TODO: Add your control notification handler code here
	//CreateOrActivateDlg(CDBDoc::GetDocPoint(),CMStrtGrupDefDlg::IDD,this);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_GROUP, 0));
}

void CDgnConCodeDlg::OnBCJoint() //PMS:4562 (Tel. 4022) MNET:XXXX-BSCHOI-20121108 #1
{
	if (m_chkApplyBCJDesign.GetCheck())
	{
		GetDlgItem(IDC_DGN_EUROCODE204_N11)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EUROCODE204_N22)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_EUROCODE204_N11)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EUROCODE204_N22)->EnableWindow(FALSE);
	}
}

/*
// MNET:4076-SHJUNG-20100609 - 이 기능 안 쓴다. 다른 옵션으로 대체.
void CDgnConCodeDlg::OnBtnNewGrup()
{
	// TODO: Add your control notification handler code here
	//CreateOrActivateDlg(CDBDoc::GetDocPoint(),CMStrtGrupDefDlg::IDD,this);
	//AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_GROUP, 0));

	CString strCode=_T("");
	m_DesignCode.GetWindowText(strCode);
	strCode = CDBLib::GetConvertCodeName(strCode);

	CDgnConCodeSeisDefDlg dlg(strCode);
	dlg.SetGroupData(m_aExcludeGroupK, m_aNotSFSRGroupK);
	dlg.SetGroupData(m_aAllGroupK, m_arAllGroup);
	if(dlg.DoModal() == IDOK)
	{
		//dlg.GetGroupData(m_arAllGroup, m_aExcludeGroupK, m_aNotSFSRGroupK);
		dlg.GetGroupData(m_aAllGroupK, m_aExcludeGroupK, m_aNotSFSRGroupK);
		dlg.GetGroupData(m_arAllGroup);
	}
}
*/

void CDgnConCodeDlg::OnChangeTorsion()
{
	if (m_btnTorsion.GetCheck())
	{
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->EnableWindow(FALSE);
	}
}


void CDgnConCodeDlg::OnBtnEC8Data()
{
	CDgnConCodeEC8InfoDlg Dlg;
	Dlg.SetDconData(&m_DconEC8Data);
	if (Dlg.DoModal() == IDOK)
	{
	}
}

void CDgnConCodeDlg::OnChkApplyNTC()
{
	BOOL bEnable = m_chkApplyNTC.GetCheck();

	if (m_strDCH.IsEmpty() && m_strDCM.IsEmpty())
	{
		GetDlgItem(IDC_DGN_EUROCODE204_N5)->GetWindowText(m_strDCH);
		GetDlgItem(IDC_DGN_EUROCODE204_N6)->GetWindowText(m_strDCM);
	}

	CString strCode = _T("");
	m_DesignCode.GetWindowText(strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	strCode = CDBLib::GetConvertCodeName(strCode);

	if (bEnable)
	{
		GetDlgItem(IDC_DGN_EUROCODE204_N5)->SetWindowText(_LS(IDS_DGN_CDA_HIGH_DUCTILITY_CLASS));
		GetDlgItem(IDC_DGN_EUROCODE204_N6)->SetWindowText(_LS(IDS_DGN_CDB_MEDIUM_DUCTILITY_CLASS));
		GetDlgItem(IDC_DGN_EUROCODE204_N99)->EnableWindow(bEnable);
		if (strCode == CONCODE_EC2_04 || strCode == CONCODE_EC2_2_05)
		//Eurocode일때만 아래 스트링 찍어줄려고, 나머지 기준은 Special Provision을 찍어주기 위함.
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
		if (strCode == CONCODE_EC2_04 || strCode == CONCODE_EC2_2_05)
		{
			GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->SetWindowText(_LS(IDS_DGN_TEXT_APPLY_EC8_04_CAPACITY_DGN));
		}
		GetDlgItem(IDC_DGN_NTC2012_DRAFT_STATIC)->ShowWindow(SW_HIDE); //임시처리 
	}

	OnSelchangeNTCType();
}

// void CDgnConCodeDlg::OnChkBehaviorFactor()
// {
//   BOOL bEnable = m_chkBehaviorFactor.GetCheck();
//   GetDlgItem(IDC_DGN_EUROCODE204_N24)->EnableWindow(bEnable);
// }


void CDgnConCodeDlg::OnChkUseVc()
{
	m_bUseVcWall = (m_ChkUseVcWall.GetCheck()) ? TRUE : FALSE;
	m_bUseVcColm = (m_ChkUseVcColm.GetCheck()) ? TRUE : FALSE;
	m_bUseVcBeam = (m_ChkUseVcBeam.GetCheck()) ? TRUE : FALSE;
}

void CDgnConCodeDlg::OnChkAutoCalcA()
{
	BOOL bCheck = m_chkAutoCalcA.GetCheck();
	if (bCheck)
	{
		double dPhi_ef = m_edtEffectivePhi.GetEditValue();
		if (fabs(dPhi_ef) < cDGN_Zero) m_edtSlendLimitA.SetEditUnit(0.7);
		else
		{
			double dAutoA = 1.0 / (1.0 + 0.2 * dPhi_ef);
			dAutoA *= pow(10.0, 3);
			dAutoA += 0.5;
			dAutoA = (int)dAutoA;
			dAutoA *= pow(10.0, -3);
			m_edtSlendLimitA.SetEditUnit(dAutoA);
		}
	}
	GetDlgItem(IDC_DGN_SLENDERNESS_A_EDT)->EnableWindow(!bCheck);
}

void CDgnConCodeDlg::OnChkAutoCalcC()
{
	BOOL bCheck = m_chkAutoCalcC.GetCheck();
	GetDlgItem(IDC_DGN_SLENDERNESS_C_EDT)->EnableWindow(!bCheck);
}

void CDgnConCodeDlg::SetNTC2008EnableControl()
{
	UpdateData(TRUE);

	BOOL bItaly = FALSE;
	if (m_ComboNationAnnex.GetCurSel() == 0)
		bItaly = TRUE;

	if (bItaly)
	{
		m_chkApplyNTC.EnableWindow(TRUE);
		OnSelchangeNTCType();
	}
	else
	{
		m_cmbNTCType.EnableWindow(FALSE);
		m_chkApplyNTC.EnableWindow(FALSE);
		m_chkApplyNTC.SetCheck(FALSE);
		OnChkApplyNTC();
	}
}

void CDgnConCodeDlg::SetEC2Ctrl(int nAnnex)
{
	const auto strStc = [&nAnnex]()
	{
		switch (nAnnex)
		{
		case CzechRepublic:
			return _LS(IDS_DGN_CON_CODE_Protection_Level);
		case Sweden:
			return _LS(IDS_DGN_CON_CODE_Service_Life);
		case Norway:
			return _LS(IDS_DGN_CON_CODE_Coefficient_kc);
		case Luxembourg:
		case Belgium:
			return _LS(IDS_DGN_CON_CODE_Structure_Type);
		case Germany:
			return _LS(IDS_DGN_CON_CODE_Bridge_Type);
		default: return CString(_T(""));
		}
	}();
	GetDlgItem(IDC_DGN_WMAX_STC)->SetWindowText(strStc);

	ShowEC2Ctrl(nAnnex);
	MoveEC2Ctrl(nAnnex);

	return;
}

void CDgnConCodeDlg::ShowEC2Ctrl(int nAnnex)
{
	GetDlgItem(IDC_DGN_EC8_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_MOMENT_GRP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_PM_CURVE_CHK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SHEAR_GRP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BEAMDGN_GRP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BEAMDGN_REINFORCEMENT1_RDO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BEAMDGN_REINFORCEMENT2_RDO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_COLUMNDGN_GRP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE1_RDO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE2_RDO)->ShowWindow(SW_SHOW);

	CDlgUtil::CtrlShowHide(this, m_aStrutAngleCtrls, TRUE);
	ShowAnnexCtrl(nAnnex);

	return;
}

void CDgnConCodeDlg::MoveEC2Ctrl(int nAnnex)
{
	CRect RectDgnCode;
	GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(&RectDgnCode);

	CRect RectNationStc;
	GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->GetWindowRect(&RectNationStc);
	const auto nNationStcH = RectNationStc.Height();
	RectNationStc.top = RectDgnCode.bottom + globalUtils.ScaleByDPI(10);
	RectNationStc.bottom = RectNationStc.top + nNationStcH;

	CRect RectNationCmb;
	GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->GetWindowRect(&RectNationCmb);
	const auto nNationCmbH = RectNationCmb.Height();
	RectNationCmb.top = RectDgnCode.bottom + globalUtils.ScaleByDPI(10);
	RectNationCmb.bottom = RectNationCmb.top + nNationCmbH;

	CRect RectMomentGrp;
	GetDlgItem(IDC_DGN_MOMENT_GRP)->GetWindowRect(&RectMomentGrp);
	const auto nGrpH = RectMomentGrp.Height();
	RectMomentGrp.top = RectNationCmb.bottom + globalUtils.ScaleByDPI(10);
	RectMomentGrp.bottom = RectMomentGrp.top + nGrpH;

	//
	CRect RectBeamDgnGrp;
	GetDlgItem(IDC_DGN_BEAMDGN_GRP)->GetWindowRect(&RectBeamDgnGrp);
	const auto nBeamDgnH = RectBeamDgnGrp.Height();
	RectBeamDgnGrp.top = RectMomentGrp.bottom + globalUtils.ScaleByDPI(10);
	RectBeamDgnGrp.bottom = RectBeamDgnGrp.top + nBeamDgnH;

	CRect RectColumnDgnGrp;
	GetDlgItem(IDC_DGN_COLUMNDGN_GRP)->GetWindowRect(&RectColumnDgnGrp);
	const auto nColDgnH = RectColumnDgnGrp.Height();
	RectColumnDgnGrp.top = RectBeamDgnGrp.bottom + globalUtils.ScaleByDPI(10);
	RectColumnDgnGrp.bottom = RectColumnDgnGrp.top + nColDgnH;
	//

	CRect RectPMChk;
	GetDlgItem(IDC_DGN_PM_CURVE_CHK)->GetWindowRect(&RectPMChk);
	const auto nPMChkH = RectPMChk.Height();
	RectPMChk.top = RectColumnDgnGrp.bottom + globalUtils.ScaleByDPI(10);
	RectPMChk.bottom = RectPMChk.top + nPMChkH;

	CRect RectShearGrp;
	GetDlgItem(IDC_DGN_SHEAR_GRP)->GetWindowRect(&RectShearGrp);
	RectShearGrp.top = RectPMChk.bottom + globalUtils.ScaleByDPI(10);
	RectShearGrp.bottom = IsExistDgCtrl(nAnnex) ? RectShearGrp.top + nGrpH * 1.5 : RectShearGrp.top + nGrpH;

	CRect RectCrackGrp;
	GetDlgItem(IDC_DGN_CRACK_GRP)->GetWindowRect(&RectCrackGrp);
	RectCrackGrp.top = RectShearGrp.bottom + globalUtils.ScaleByDPI(10);
	RectCrackGrp.bottom = RectCrackGrp.top + nGrpH;

	ScreenToClient(RectNationStc);
	ScreenToClient(RectNationCmb);
	ScreenToClient(RectMomentGrp);
	ScreenToClient(RectBeamDgnGrp);
	ScreenToClient(RectColumnDgnGrp);
	ScreenToClient(RectPMChk);
	ScreenToClient(RectShearGrp);
	ScreenToClient(RectCrackGrp);
	GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->MoveWindow(RectNationStc.left, RectNationStc.top, RectNationStc.Width(),
	                                                      RectNationStc.Height());
	GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->MoveWindow(RectNationCmb.left, RectNationCmb.top, RectNationCmb.Width(),
	                                                     RectNationCmb.Height());
	GetDlgItem(IDC_DGN_MOMENT_GRP)->MoveWindow(RectMomentGrp.left, RectMomentGrp.top, RectMomentGrp.Width(),
	                                           RectMomentGrp.Height());
	GetDlgItem(IDC_DGN_BEAMDGN_GRP)->MoveWindow(RectBeamDgnGrp.left, RectBeamDgnGrp.top, RectBeamDgnGrp.Width(),
	                                            RectBeamDgnGrp.Height());
	GetDlgItem(IDC_DGN_COLUMNDGN_GRP)->MoveWindow(RectColumnDgnGrp.left, RectColumnDgnGrp.top, RectColumnDgnGrp.Width(),
	                                              RectColumnDgnGrp.Height());
	GetDlgItem(IDC_DGN_PM_CURVE_CHK)->MoveWindow(RectPMChk.left, RectPMChk.top, RectPMChk.Width(), RectPMChk.Height());
	GetDlgItem(IDC_DGN_SHEAR_GRP)->MoveWindow(RectShearGrp.left, RectShearGrp.top, RectShearGrp.Width(),
	                                          RectShearGrp.Height());
	GetDlgItem(IDC_DGN_CRACK_GRP)->MoveWindow(RectCrackGrp.left, RectCrackGrp.top, RectCrackGrp.Width(),
	                                          RectCrackGrp.Height());

	CRect RectDgStc;
	GetDlgItem(IDC_DGN_DG_STC)->GetWindowRect(&RectDgStc);
	const auto nTopDg = RectDgStc.top;
	const auto nLeftDg = RectDgStc.left;
	//
	CRect RectMrftStc;
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->GetWindowRect(&RectMrftStc);
	const auto nTopMrft = RectMrftStc.top;

	std::vector<int> vnMrftID;
	vnMrftID.push_back(IDC_DGN_RCCTRL_MRFT_STATIC);
	vnMrftID.push_back(IDC_DGN_RCCTRL_MRFT_FACTOR);
	ClientToScreen(RectMomentGrp);
	for (const auto nID : vnMrftID)
	{
		CRect Rect;
		GetDlgItem(nID)->GetWindowRect(&Rect);
		const auto nMoveY = nTopMrft - RectMomentGrp.bottom + globalUtils.ScaleByDPI(30);
		const auto nWidth = Rect.Width();
		Rect.left = (nID == IDC_DGN_RCCTRL_MRFT_STATIC)
			            ? nLeftDg
			            : RectMomentGrp.right - nWidth - globalUtils.ScaleByDPI(10);
		Rect.right = Rect.left + nWidth;
		Rect.top -= nMoveY;
		Rect.bottom -= nMoveY;

		ScreenToClient(Rect);
		GetDlgItem(nID)->MoveWindow(Rect.left, Rect.top, Rect.Width(), Rect.Height());
	}
	//
	CRect BeamRectType1, BeamRectType2;
	GetDlgItem(IDC_DGN_BEAMDGN_REINFORCEMENT1_RDO)->GetWindowRect(&BeamRectType1);
	GetDlgItem(IDC_DGN_BEAMDGN_REINFORCEMENT2_RDO)->GetWindowRect(&BeamRectType2);
	const auto nBeamTopType1 = BeamRectType1.top;
	const auto nBeamTopType2 = BeamRectType2.top;

	std::vector<int> vnBeamDgnID;
	vnBeamDgnID.push_back(IDC_DGN_BEAMDGN_REINFORCEMENT1_RDO);
	vnBeamDgnID.push_back(IDC_DGN_BEAMDGN_REINFORCEMENT2_RDO);
	ClientToScreen(RectBeamDgnGrp);
	for (const auto nID : vnBeamDgnID)
	{
		CRect Rect;
		GetDlgItem(nID)->GetWindowRect(&Rect);
		auto nTopTypeTemp = nBeamTopType1; // (nID == IDC_DGN_BEAMDGN_REINFORCEMENT1_RDO) ? nTopType1 : nTopType2;
		const auto nMoveY = nTopTypeTemp - RectBeamDgnGrp.top - globalUtils.ScaleByDPI(20);
		Rect.left = Rect.left;
		Rect.top -= nMoveY;
		Rect.bottom -= nMoveY;

		ScreenToClient(Rect);
		GetDlgItem(nID)->MoveWindow(Rect.left, Rect.top, Rect.Width(), Rect.Height());
	}
	//
	CRect RectType1, RectType2;
	GetDlgItem(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE1_RDO)->GetWindowRect(&RectType1);
	GetDlgItem(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE2_RDO)->GetWindowRect(&RectType2);
	const auto nTopType1 = RectType1.top;
	const auto nTopType2 = RectType2.top;

	std::vector<int> vnColumnDgnID;
	vnColumnDgnID.push_back(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE1_RDO);
	vnColumnDgnID.push_back(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE2_RDO);
	ClientToScreen(RectColumnDgnGrp);
	for (const auto nID : vnColumnDgnID)
	{
		CRect Rect;
		GetDlgItem(nID)->GetWindowRect(&Rect);
		auto nTopTypeTemp = nTopType1; // (nID == IDC_DGN_COLUMNDGN_BIAXIAL_TYPE1_RDO) ? nTopType1 : nTopType2;
		const auto nMoveY = nTopTypeTemp - RectColumnDgnGrp.top - globalUtils.ScaleByDPI(20);
		Rect.left = Rect.left;
		Rect.top -= nMoveY;
		Rect.bottom -= nMoveY;

		ScreenToClient(Rect);
		GetDlgItem(nID)->MoveWindow(Rect.left, Rect.top, Rect.Width(), Rect.Height());
	}
	//
	CRect RectAngleChk;
	GetDlgItem(IDC_DGN_STRUT_ANGLE_STC)->GetWindowRect(&RectAngleChk);
	const auto nTopAngle = RectAngleChk.top;

	std::vector<int> vnAngleID;
	vnAngleID.push_back(IDC_DGN_STRUT_ANGLE_STC);
	vnAngleID.push_back(IDC_DGN_STRUT_ANGLE_EDT);
	vnAngleID.push_back(IDC_DGN_STRUT_ANGLE_UNT);
	ClientToScreen(RectShearGrp);
	for (const auto nID : vnAngleID)
	{
		CRect Rect;
		GetDlgItem(nID)->GetWindowRect(&Rect);
		const auto nMoveY = nTopAngle - RectShearGrp.top - globalUtils.ScaleByDPI(25);
		Rect.left = (nID == IDC_DGN_STRUT_ANGLE_STC) ? nLeftDg : Rect.left;
		Rect.top -= nMoveY;
		Rect.bottom -= nMoveY;

		ScreenToClient(Rect);
		GetDlgItem(nID)->MoveWindow(Rect.left, Rect.top, Rect.Width(), Rect.Height());
	}

	std::vector<int> vnDgID;
	vnDgID.push_back(IDC_DGN_DG_STC);
	vnDgID.push_back(IDC_DGN_DG_EDT);
	vnDgID.push_back(IDC_DGN_DG_UNT);

	auto nLeftDgEdt = 0;
	for (const auto nID : vnDgID)
	{
		CRect Rect;
		GetDlgItem(nID)->GetWindowRect(&Rect);
		if (nID == IDC_DGN_DG_EDT) nLeftDgEdt = Rect.left;
		const auto nMoveY = nTopDg - RectShearGrp.bottom + globalUtils.ScaleByDPI(30);
		Rect.top -= nMoveY;
		Rect.bottom -= nMoveY;

		ScreenToClient(Rect);
		GetDlgItem(nID)->MoveWindow(Rect.left, Rect.top, Rect.Width(), Rect.Height());
	}

	CRect RectWmaxCmb;
	GetDlgItem(IDC_DGN_PL_CMB)->GetWindowRect(&RectWmaxCmb);

	std::vector<int> vnWmaxID;
	vnWmaxID.push_back(IDC_DGN_WMAX_STC);
	vnWmaxID.push_back(IDC_DGN_PL_CMB);
	ClientToScreen(RectCrackGrp);
	for (const auto nID : vnWmaxID)
	{
		CRect Rect;
		GetDlgItem(nID)->GetWindowRect(&Rect);
		const auto nMoveY = RectWmaxCmb.top - RectCrackGrp.bottom + globalUtils.ScaleByDPI(35);
		Rect.top -= nMoveY;
		Rect.bottom -= nMoveY;

		ScreenToClient(Rect);
		GetDlgItem(nID)->MoveWindow(Rect.left, Rect.top, Rect.Width(), Rect.Height());
	}

	vnWmaxID.clear();
	vnWmaxID.push_back(IDC_DGN_KC_EDT);
	vnWmaxID.push_back(IDC_DGN_SL_CMB);
	vnWmaxID.push_back(IDC_DGN_STYPE_CMB);
	vnWmaxID.push_back(IDC_DGN_BRIDGE_TYPE_CMB);
	GetDlgItem(IDC_DGN_PL_CMB)->GetWindowRect(&RectWmaxCmb);
	for (const auto nID : vnWmaxID)
	{
		CRect Rect;
		GetDlgItem(nID)->GetWindowRect(&Rect);
		Rect.top = RectWmaxCmb.top;
		Rect.bottom = RectWmaxCmb.bottom;
		Rect.left = (nID == IDC_DGN_KC_EDT) ? nLeftDgEdt : RectWmaxCmb.left;
		Rect.right = RectWmaxCmb.right;

		ScreenToClient(Rect);
		GetDlgItem(nID)->MoveWindow(Rect.left, Rect.top, Rect.Width(), Rect.Height());
	}

	CRect RectOrg;
	const auto nBotID = IsExistWmaxCtrl(nAnnex) ? IDC_DGN_CRACK_GRP : IDC_DGN_SHEAR_GRP;
	GetDlgItem(nBotID)->GetWindowRect(&RectOrg);

	CRect RectDlg, RectOk, RectCn;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK)->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);

	CRect rDlg;
	rDlg.left = RectDlg.left;
	rDlg.top = RectDlg.top;
	rDlg.right = RectOrg.right + (RectOrg.left - RectDlg.left);
	rDlg.bottom = RectOrg.bottom + RectOk.Height() + globalUtils.ScaleByDPI(24);

	CRect rOk;
	rOk.left = RectOk.left;
	rOk.right = RectOk.right;
	rOk.top = rDlg.bottom - RectOk.Height() - globalUtils.ScaleByDPI(8);
	rOk.bottom = rOk.top + RectOk.Height();

	CRect rCn;
	rCn.left = RectCn.left;
	rCn.right = RectCn.right;
	rCn.top = rOk.top;
	rCn.bottom = rOk.bottom;

	ScreenToClient(rDlg);
	ScreenToClient(rOk);
	ScreenToClient(rCn);
	SetWindowPos(NULL, rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDOK)->SetWindowPos(NULL, rOk.left, rOk.top, rOk.Width(), rOk.Height(), SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rCn.left, rCn.top, rCn.Width(), rCn.Height(), SWP_NOSIZE);

	return;
}

void CDgnConCodeDlg::ShowBSCtrl()
{
	GetDlgItem(IDC_DGN_EC8_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_NATIONAL_ANNEX_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_NATIONAL_ANNEX_COMBO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_MOMENT_GRP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_FACTOR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_PM_CURVE_CHK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SHEAR_GRP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_BEAMDGN_GRP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BEAMDGN_REINFORCEMENT1_RDO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BEAMDGN_REINFORCEMENT2_RDO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_COLUMNDGN_GRP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE1_RDO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE2_RDO)->ShowWindow(SW_SHOW);

	CDlgUtil::CtrlShowHide(this, m_aStrutAngleCtrls, FALSE);
	return;
}

void CDgnConCodeDlg::MoveBSCtrl()
{
	CRect RectDgnCode;
	GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(&RectDgnCode);

	CRect RectMomentGrp;
	GetDlgItem(IDC_DGN_MOMENT_GRP)->GetWindowRect(&RectMomentGrp);
	const auto nGrpH = RectMomentGrp.Height();
	RectMomentGrp.top = RectDgnCode.bottom + 10;
	RectMomentGrp.bottom = RectMomentGrp.top + nGrpH;

	//
	CRect RectBeamDgnGrp;
	GetDlgItem(IDC_DGN_BEAMDGN_GRP)->GetWindowRect(&RectBeamDgnGrp);
	const auto nBeamDgnH = RectBeamDgnGrp.Height();
	RectBeamDgnGrp.top = RectMomentGrp.bottom + 10;
	RectBeamDgnGrp.bottom = RectBeamDgnGrp.top + nBeamDgnH;

	CRect RectColumnDgnGrp;
	GetDlgItem(IDC_DGN_COLUMNDGN_GRP)->GetWindowRect(&RectColumnDgnGrp);
	const auto nColDgnH = RectColumnDgnGrp.Height();
	RectColumnDgnGrp.top = RectBeamDgnGrp.bottom + 10;
	RectColumnDgnGrp.bottom = RectColumnDgnGrp.top + nColDgnH;
	//

	CRect RectPMChk;
	GetDlgItem(IDC_DGN_PM_CURVE_CHK)->GetWindowRect(&RectPMChk);
	const auto nPMChkH = RectPMChk.Height();
	RectPMChk.top = RectColumnDgnGrp.bottom + 10;
	RectPMChk.bottom = RectPMChk.top + nPMChkH;

	ScreenToClient(RectMomentGrp);
	ScreenToClient(RectBeamDgnGrp);
	ScreenToClient(RectColumnDgnGrp);
	ScreenToClient(RectPMChk);
	GetDlgItem(IDC_DGN_MOMENT_GRP)->MoveWindow(RectMomentGrp.left, RectMomentGrp.top, RectMomentGrp.Width(),
	                                           RectMomentGrp.Height());
	GetDlgItem(IDC_DGN_BEAMDGN_GRP)->MoveWindow(RectBeamDgnGrp.left, RectBeamDgnGrp.top, RectBeamDgnGrp.Width(),
	                                            RectBeamDgnGrp.Height());
	GetDlgItem(IDC_DGN_COLUMNDGN_GRP)->MoveWindow(RectColumnDgnGrp.left, RectColumnDgnGrp.top, RectColumnDgnGrp.Width(),
	                                              RectColumnDgnGrp.Height());
	GetDlgItem(IDC_DGN_PM_CURVE_CHK)->MoveWindow(RectPMChk.left, RectPMChk.top, RectPMChk.Width(), RectPMChk.Height());

	CRect RectDgStc;
	GetDlgItem(IDC_DGN_DG_STC)->GetWindowRect(&RectDgStc);
	const auto nTopDg = RectDgStc.top;
	const auto nLeftDg = RectDgStc.left;
	//
	CRect RectMrftStc;
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->GetWindowRect(&RectMrftStc);
	const auto nTopMrft = RectMrftStc.top;

	std::vector<int> vnMrftID;
	vnMrftID.push_back(IDC_DGN_RCCTRL_MRFT_STATIC);
	vnMrftID.push_back(IDC_DGN_RCCTRL_MRFT_FACTOR);
	ClientToScreen(RectMomentGrp);
	for (const auto nID : vnMrftID)
	{
		CRect Rect;
		GetDlgItem(nID)->GetWindowRect(&Rect);
		const auto nMoveY = nTopMrft - RectMomentGrp.bottom + 30;
		const auto nWidth = Rect.Width();
		Rect.left = (nID == IDC_DGN_RCCTRL_MRFT_STATIC) ? nLeftDg : RectMomentGrp.right - nWidth - 10;
		Rect.right = Rect.left + nWidth;
		Rect.top -= nMoveY;
		Rect.bottom -= nMoveY;

		ScreenToClient(Rect);
		GetDlgItem(nID)->MoveWindow(Rect.left, Rect.top, Rect.Width(), Rect.Height());
	}
	//
	CRect BeamRectType1, BeamRectType2;
	GetDlgItem(IDC_DGN_BEAMDGN_REINFORCEMENT1_RDO)->GetWindowRect(&BeamRectType1);
	GetDlgItem(IDC_DGN_BEAMDGN_REINFORCEMENT2_RDO)->GetWindowRect(&BeamRectType2);
	const auto nBeamTopType1 = BeamRectType1.top;
	const auto nBeamTopType2 = BeamRectType2.top;

	std::vector<int> vnBeamDgnID;
	vnBeamDgnID.push_back(IDC_DGN_BEAMDGN_REINFORCEMENT1_RDO);
	vnBeamDgnID.push_back(IDC_DGN_BEAMDGN_REINFORCEMENT2_RDO);
	ClientToScreen(RectBeamDgnGrp);
	for (const auto nID : vnBeamDgnID)
	{
		CRect Rect;
		GetDlgItem(nID)->GetWindowRect(&Rect);
		auto nTopTypeTemp = nBeamTopType1; // (nID == IDC_DGN_BEAMDGN_REINFORCEMENT1_RDO) ? nTopType1 : nTopType2;
		const auto nMoveY = nTopTypeTemp - RectBeamDgnGrp.top - 20;
		Rect.left = Rect.left;
		Rect.top -= nMoveY;
		Rect.bottom -= nMoveY;

		ScreenToClient(Rect);
		GetDlgItem(nID)->MoveWindow(Rect.left, Rect.top, Rect.Width(), Rect.Height());
	}
	//
	CRect RectType1, RectType2;
	GetDlgItem(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE1_RDO)->GetWindowRect(&RectType1);
	GetDlgItem(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE2_RDO)->GetWindowRect(&RectType2);
	const auto nTopType1 = RectType1.top;
	const auto nTopType2 = RectType2.top;

	std::vector<int> vnColumnDgnID;
	vnColumnDgnID.push_back(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE1_RDO);
	vnColumnDgnID.push_back(IDC_DGN_COLUMNDGN_BIAXIAL_TYPE2_RDO);
	ClientToScreen(RectColumnDgnGrp);
	for (const auto nID : vnColumnDgnID)
	{
		CRect Rect;
		GetDlgItem(nID)->GetWindowRect(&Rect);
		auto nTopTypeTemp = nTopType1; // (nID == IDC_DGN_COLUMNDGN_BIAXIAL_TYPE1_RDO) ? nTopType1 : nTopType2;
		const auto nMoveY = nTopTypeTemp - RectColumnDgnGrp.top - 20;
		Rect.left = Rect.left;
		Rect.top -= nMoveY;
		Rect.bottom -= nMoveY;

		ScreenToClient(Rect);
		GetDlgItem(nID)->MoveWindow(Rect.left, Rect.top, Rect.Width(), Rect.Height());
	}
	CRect RectOrg;
	const auto nBotID = IDC_DGN_PM_CURVE_CHK;
	GetDlgItem(nBotID)->GetWindowRect(&RectOrg);

	CRect RectDlg, RectOk, RectCn;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK)->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);

	CRect rDlg;
	rDlg.left = RectDlg.left;
	rDlg.top = RectDlg.top;
	rDlg.right = RectOrg.right + (RectOrg.left - RectDlg.left) + globalUtils.ScaleByDPI(8);
	rDlg.bottom = RectOrg.bottom + 2.5 * RectOk.Height();

	CRect rOk;
	rOk.left = RectOk.left;
	rOk.right = RectOk.right;
	//rOk.top = rDlg.bottom - (int)(1.75*RectOk.Height());
	//rOk.bottom = rOk.top + RectOk.Height();
	rOk.top = rDlg.bottom - RectOk.Height() - globalUtils.ScaleByDPI(8);
	rOk.bottom = rOk.top + RectOk.Height();

	CRect rCn;
	rCn.left = RectCn.left; // -nOffsetx;
	rCn.right = RectCn.right;
	rCn.top = rOk.top;
	rCn.bottom = rOk.bottom;

	ScreenToClient(rDlg);
	ScreenToClient(rOk);
	ScreenToClient(rCn);
	SetWindowPos(NULL, rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDOK)->SetWindowPos(NULL, rOk.left, rOk.top, rOk.Width(), rOk.Height(), SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rCn.left, rCn.top, rCn.Width(), rCn.Height(), SWP_NOSIZE);

	return;
}

void CDgnConCodeDlg::ShowAnnexCtrl(int nAnnex)
{
	const auto bDGKC = (nAnnex == Norway);
	GetDlgItem(IDC_DGN_DG_STC)->ShowWindow(bDGKC);
	GetDlgItem(IDC_DGN_DG_EDT)->ShowWindow(bDGKC);
	GetDlgItem(IDC_DGN_DG_UNT)->ShowWindow(bDGKC);
	GetDlgItem(IDC_DGN_KC_EDT)->ShowWindow(bDGKC);

	const auto bWmax = IsExistWmaxCtrl(nAnnex);
	const auto bPL = (nAnnex == CzechRepublic);
	const auto bSL = (nAnnex == Sweden);
	const auto bSType = (nAnnex == Luxembourg || nAnnex == Belgium);
	const auto bBType = (nAnnex == Germany);
	GetDlgItem(IDC_DGN_CRACK_GRP)->ShowWindow(bWmax);
	GetDlgItem(IDC_DGN_WMAX_STC)->ShowWindow(bWmax);
	GetDlgItem(IDC_DGN_PL_CMB)->ShowWindow(bPL);
	GetDlgItem(IDC_DGN_SL_CMB)->ShowWindow(bSL);
	GetDlgItem(IDC_DGN_STYPE_CMB)->ShowWindow(bSType);
	GetDlgItem(IDC_DGN_BRIDGE_TYPE_CMB)->ShowWindow(bBType);

	return;
}

void CDgnConCodeDlg::InitAnnexCtrl(int nAnnex)
{
	m_EdtDg.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_UntDg.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_EdtKc.SetUnitType(D_UNITSYS_NONE);

	if (!m_CmbPL.GetCount())
	{
		m_CmbPL.AddString(_T("1"));
		m_CmbPL.AddString(_T("2"));
		m_CmbPL.AddString(_T("3"));
	}

	if (!m_CmbSL.GetCount())
	{
		m_CmbSL.AddString(_T("L100"));
		m_CmbSL.AddString(_T("L50"));
		if (nAnnex != Norway)
		{
			m_CmbSL.AddString(_T("L20"));
		}
	}

	if (!m_CmbSType.GetCount())
	{
		m_CmbSType.AddString(_LS(IDS_DGN_CON_CODE_Road_superstructure));
		m_CmbSType.AddString(_LS(IDS_DGN_CON_CODE_Railway_superstructure));
		m_CmbSType.AddString(_LS(IDS_DGN_CON_CODE_Other_superstructure));
		m_CmbSType.AddString(_LS(IDS_DGN_CON_CODE_Piles_and_Columns));
		m_CmbSType.AddString(_LS(IDS_DGN_CON_CODE_Earth_retaining_structure));
		m_CmbSType.AddString(_LS(IDS_DGN_CON_CODE_Anchorage_reactions));
	}

	if (!m_cmbBridgeType.GetCount())
	{
		m_cmbBridgeType.AddString(_LS(IDS_DGN_BRIDGE_TYPE_ROADWAY));
		m_cmbBridgeType.AddString(_LS(IDS_DGN_BRIDGE_TYPE_RAILROAD));
	}

	return;
}

bool CDgnConCodeDlg::IsExistWmaxCtrl(int nAnnex) const
{
	switch (nAnnex)
	{
	case CzechRepublic:
	case Sweden:
	case Norway:
	case Luxembourg:
	case Belgium:
	case Germany:
		return true;
	default: return false;
	}
}

bool CDgnConCodeDlg::IsExistDgCtrl(int nAnnex) const
{
	return (nAnnex == Norway);
}

void CDgnConCodeDlg::SetBndrElemClimitDesc(const CString& strCodeName)
{
	CString strClimit = _LS(IDS_DGN_CODE_WALL_SHEAR_TYPE_BEM_CLMT);
	if (strCodeName == CONCODE_ACI318_14 || strCodeName == CONCODE_ACI318M_14 ||
		strCodeName == CONCODE_ACI318_19 || strCodeName == CONCODE_ACI318M_19 ||
		strCodeName == CONCODE_ACI318_25 || strCodeName == CONCODE_ACI318M_25 ||
		strCodeName == CONCODE_NSCP_2015 || strCodeName == CONCODE_NTC_DCEC_2017 || strCodeName ==
		CONCODE_NTC_DCEC_2023)
	{
		strClimit = _LS(IDS_DGN_CODE_WALL_SHEAR_TYPE_BEM_CLMT_ACI14);
	}

	GetDlgItem(IDC_DGN_WALLTYPE_N3)->SetWindowText(strClimit);
}

void CDgnConCodeDlg::OnChangeBndrElemMethodType()
{
	UpdateData(TRUE);
	EnableWindowByBndrElemMethod();
}

void CDgnConCodeDlg::EnableWindowBySpecialWall()
{
	BOOL bSpecial = FALSE;
	CString strCode = _T("");
	m_DesignCode.GetWindowText(strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	if (strCode == _T("NSR-10"))
	{
		CArray<UINT, UINT> aShearWallRdo;
		aShearWallRdo.RemoveAll();
		aShearWallRdo.Add(IDC_DGN_WALLTYPE_N12);
		aShearWallRdo.Add(IDC_DGN_WALLTYPE_N13);
		aShearWallRdo.Add(IDC_DGN_WALLTYPE_N14);
		aShearWallRdo.FreeExtra();

		int nCheck = 2;
		CDlgUtil::CtrlRadioGetCheck(this, aShearWallRdo, nCheck);
		bSpecial = (nCheck != 2);

		CString strFckLimit = _T("");
		switch (nCheck)
		{
		case 0:
		case 2:
			strFckLimit = _LS(IDS_DGN_CODE_WALL_SHEAR_TYPE_BEM_FCKLMT02);
			break;
		case 1:
			strFckLimit = _LS(IDS_DGN_CODE_WALL_SHEAR_TYPE_BEM_FCKLMT03);
			break;
		default: ASSERT(0);
			break;
		}

		GetDlgItem(IDC_DGN_WALLTYPE_N8)->SetWindowText(strFckLimit);
	}
	else
	{
		bSpecial = m_bSpecialWall;
	}

	SetBndrElemClimitDesc(strCode);

	if (bSpecial)
	{
		GetDlgItem(IDC_DGN_WALLTYPE_N2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALLTYPE_N3)->EnableWindow(TRUE);
		if (m_nBndrElemMethod == 0)
		{
			GetDlgItem(IDC_DGN_WALLTYPE_N4)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_WALLTYPE_N5)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_WALLTYPE_N6)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_WALLTYPE_N7)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_DGN_WALLTYPE_N4)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_WALLTYPE_N5)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_WALLTYPE_N6)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_WALLTYPE_N7)->EnableWindow(FALSE);
		}
		GetDlgItem(IDC_DGN_WALLTYPE_N8)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_WALLTYPE_N2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N6)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N8)->EnableWindow(FALSE);
	}
}

void CDgnConCodeDlg::GetDataFromDlg(T_DCON_D& rData)
{
	// Get Design Code.
	int Index = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(Index, rData.DesignCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(rData.DesignCode);
#endif
	//(2009.01.15) Add by Unsang :: MNET.3908참고, US Units 구분
	rData.DesignCode = CDBLib::GetConvertCodeName(rData.DesignCode);

	if (rData.DesignCode == CONCODE_AASHTO_LRFD17 || rData.DesignCode == CONCODE_AS5100_5_17)
	{
		rData.nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAnnex, m_DesignAnnex.GetCurSel());
	}
	else
	{
		rData.nNationalAnnex = m_ComboNationAnnex.GetCurSel();
	}

	// Get Special Seismic and Uni Axis.
	rData.bSpecialSeismic = m_bSpecialEQ;
	// Change by ZINU.('02.8.27).
	int iStatus = GetDataStatus(rData.DesignCode);
	if (iStatus == 1 || iStatus == 9) rData.nClass = m_iColmMethod;
	else if (iStatus == 2) rData.nClass = m_iSafeLevel;
	else if (iStatus == 3) rData.nClass = m_iEqGrade;
	else if (iStatus == 4) rData.nClass = m_iPmCurve;
	else if (iStatus == 6) rData.nClass = m_iEqCategory;
	else if (iStatus == 7) rData.nClass = m_iEqCategory;
	else if (iStatus == 8) rData.nClass = m_iEqCategory;
	else if (iStatus == 10) rData.nClass = m_iFrameType;
	else if (iStatus == 17) rData.nClass = m_iDuctility; //Eurocode2:04
	else if (iStatus == 18) rData.nClass = m_iFrameType; //KCI-USD07
	else if (iStatus == 19) rData.nClass = m_iEqCategory;
	else if (iStatus == 20) rData.nClass = m_iFrameType; // Modify by GAY. PMS:4450. ('12.05.14). ACI318-08, 11.
		//else if(iStatus==13)          rData.nClass = m_iPmCurve;  // Coded by Seungjun MNet:No.2591 ('20061207)	For SCWB Design.
		//else if(iStatus==15)          rData.nClass = m_iPmCurve;  // Coded by Seungjun MNet:No.2591 ('20061207)	For SCWB Design.
	else rData.nClass = 0;

	rData.iSlabType = m_iSlabType;
	rData.dMrft = m_dMrft;
	rData.dTrft = m_dTrft;
	rData.iAlphaType = m_iAlphaType1;
	rData.dAlpha1 = m_dAlpha1;
	rData.dAlpha2 = m_dAlpha2;
	rData.dConShearStrengthRatio = m_dConShearStrengthRatio;

	rData.dStorColmWeakBeamFactor = m_dStorColmWeakBeamFactor;
	rData.dGamma_rdForBeam = m_dGamma_rdForBeam;
	rData.dGamma_rdForColm = m_dGamma_rdForColm;
	rData.dGamma_rdForWall = m_dGamma_rdForWall;
	rData.dGamma_rdForJoint = m_dGamma_rdForJoint;

	rData.bTorsionDesign = m_bTorsionDesign;
	rData.bBCJointDgn = m_bBCJointDgn;
	rData.bNTCFlag = m_bNTCFlag;
	rData.dUf = m_dUf;
	rData.dStrutAngle = m_edtStrutAngle.GetEditValue();
	rData.dPhi_ef = m_edtEffectivePhi.GetEditValue();
	//   rData.bBehaviorFact = m_chkBehaviorFactor.GetCheck();
	//   rData.dBehaviorFactor_q = m_edtBehaviorFactor.GetEditValue();

	rData.dA = m_edtSlendLimitA.GetEditValue();
	rData.dB = m_edtSlendLimitB.GetEditValue();
	rData.dC = m_edtSlendLimitC.GetEditValue();
	rData.bAutoA = m_chkAutoCalcA.GetCheck();
	rData.bAutoC = m_chkAutoCalcC.GetCheck();

	// Add by ZINU.('03.11.18). For Strength Reduction Factor by Code.
	for (int i = 0; i < 5; i++) rData.dPhi[i] = 0.0;

	// Save KCI-USD07
	CString strNewValue = _T("");
	m_CmbCd.GetWindowText(strNewValue);
	m_dCd = _ttof(strNewValue);
	m_CmbIe.GetWindowText(strNewValue);
	m_dIe = _ttof(strNewValue);

	if (rData.DesignCode == CONCODE_NSR_10)
	{
		rData.nSpecialWall = m_nSpecialWallMtd;
	}
	else
	{
		rData.nSpecialWall = m_bSpecialWall;
	}
	rData.iBndrElemMethod = m_nBndrElemMethod;
	rData.dCd = m_dCd;
	rData.dIe = m_dIe;

	// Save EC8 Data
	rData.iFrameType = m_DconEC8Data.iFrameType;
	rData.dAua1 = m_DconEC8Data.dAua1;
	rData.SplcK = m_DconEC8Data.SplcK;
	rData.dSoilFactor = m_DconEC8Data.dSoilFactor;
	rData.dT[0] = m_DconEC8Data.dT[0];
	rData.dT[1] = m_DconEC8Data.dT[1];
	rData.dT[2] = m_DconEC8Data.dT[2];
	rData.dAgR = m_DconEC8Data.dAgR;
	rData.dI = m_DconEC8Data.dI;
	rData.dDampingRat = m_DconEC8Data.dDampingRat;
	rData.bBehaviorFact = m_DconEC8Data.bBehaviorFact;
	rData.dBehaviorFactor_q = m_DconEC8Data.dBehaviorFactor_q;

	rData.iExposureFactor = m_nExposureFactor;
	rData.iPouringMethod = m_nPouringMethod;

	// MNET:4076-SHJUNG-20100604
	// Eurocode2:04은 원래 컨트롤 사용.
	// Secondary Group => aExcludeGroup에 채워준다.
	if (rData.DesignCode == CONCODE_EC2_04)
	{
		//     m_aExcludeGroupK.RemoveAll();
		//     m_aNotSFSRGroupK.RemoveAll();

		int ix = m_cmbGroup.GetCurSel();
		if (ix == CB_ERR) rData.SecondaryGroupKey = 0;
		else rData.SecondaryGroupKey = m_cmbGroup.GetItemData(ix);

		m_GroupKey = rData.SecondaryGroupKey;

		//(Tel. 4022) MNET:XXXX-BSCHOI-20121015 #1
		rData.nNTCType = m_cmbNTCType.GetItemData(m_cmbNTCType.GetCurSel());

		// PMS:DNKIM-20121030// PMS:DNKIM-20121030
		rData.bUseVcWall = m_ChkUseVcWall.GetCheck();
		rData.bUseVcColm = m_ChkUseVcColm.GetCheck();
		rData.bUseVcBeam = m_ChkUseVcBeam.GetCheck();

		if (m_ComboNationAnnex.GetCurSel() == 3) rData.bLimitShearStrengthConc = m_ChkLimitShearStrgConc.GetCheck();

		//     if(m_GroupKey !=0 )
		//     {      
		//       m_aExcludeGroupK.Add(m_GroupKey);
		//     }
	}

	// Exposure Condition
	rData.nExposure = m_nExposure;

	// Bridge Type
	rData.nBridgeType = m_cmbBridgeType.GetCurSel();
	rData.enPL = static_cast<dgn::def::enProtectionLevelEU>(m_CmbPL.GetCurSel());
	rData.enSL = static_cast<dgn::def::enServiceLifeEU>(m_CmbSL.GetCurSel());
	rData.enSType = static_cast<dgn::def::enStructTypeEU>(m_CmbSType.GetCurSel());
	rData.dAggregateSize = m_EdtDg.GetEditValue();
	rData.dkc = m_EdtKc.GetEditValue();

	rData.bmb7mb8 = m_bmb7mb8;
	rData.nmb7mb8Type = m_nmb7mb8Type;
	rData.dmb7 = m_edtmb7.GetEditValue();
	rData.dmb8 = m_edtmb8.GetEditValue();
	rData.bmb9 = m_bmb9;
	rData.nmb9Type = m_nmb9Type;
	rData.dmb9 = m_edtmb9.GetEditValue();
	rData.bChkPM = m_bChkPM;
	//   rData.aExcludeGroup.Copy(m_aExcludeGroupK);
	//   rData.aNotSFSRGroup.Copy(m_aNotSFSRGroupK);

	const CString& strCode = rData.DesignCode;
	if (strCode == CONCODE_KCI_USD07 || strCode == CONCODE_KCI_USD12 ||
		strCode == CONCODE_KDS_41_30_2018 || strCode == CONCODE_KDS_41_20_2022 ||
		strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 ||
		strCode == CONCODE_ACI318_14 || strCode == CONCODE_ACI318M_14 || strCode == CONCODE_NSR_10 ||
		strCode == CONCODE_NSCP_2015 || strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318M_19 ||
		strCode == CONCODE_ACI318_25 || strCode == CONCODE_ACI318M_25)
	{
		rData.bNonSeisSubBeam = m_bSubBeam;
		rData.bNonSeisCantilever = m_bCantilever;
		rData.bNonSeisUnderGround = m_bUnderBmColm;
	}
	if (strCode == CONCODE_IS456_2000) //Pinakin Added for IS456 MAT
	{
		rData.dSubRex = m_dedtSubRex.GetEditValue();
		//	  rData.dFOS				=m_dedtFOS;
		rData.dFOS = m_dedtFOS.GetEditValue();
		rData.iSubCode = m_iIs13920Sel;

		rData.bIS3370Sel = m_chkIS3370.GetCheck();
		rData.bIS3370AnnexA = m_iIS3370Sel == 0 ? TRUE : FALSE;
		rData.dIS3370AnnexA_ecs = m_dedtESS.GetEditValue();
		rData.dIS3370AnnexA_etc = m_dedtETT.GetEditValue();
		rData.dIS3370AnnexA_t1 = m_dedtT1.GetEditValue();
		rData.iIS3370AnnexB_cw = m_cmbIS3370AnnexB2mm.GetCurSel();
	}
	if (strCode == CONCODE_IRC112_2011) //Pinakin Added for IS456 MAT
	{
		rData.dAlphacc = 0.67;
		rData.bChkPM = m_bChkPM = TRUE;
	}

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSCWBMethod, rData.nSCWBDgnMethod);

	return;
}

void CDgnConCodeDlg::OnChkSpecialWall()
{
	UpdateData(TRUE);
	EnableWindowBySpecialWall();
}

void CDgnConCodeDlg::EnableWindowByBndrElemMethod()
{
	if (m_nBndrElemMethod == 1)
	{
		GetDlgItem(IDC_DGN_WALLTYPE_N4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N6)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N7)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_WALLTYPE_N4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALLTYPE_N5)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALLTYPE_N6)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALLTYPE_N7)->EnableWindow(TRUE);
	}
}

void CDgnConCodeDlg::OnSelchangeCdCmb()
{
	UpdateData(TRUE);
	CString strNewValue = _T("");
	m_CmbCd.GetWindowText(strNewValue);
	m_dCd = _ttof(strNewValue);
}

void CDgnConCodeDlg::OnSelchangeIeCmb()
{
	UpdateData(TRUE);
	CString strNewValue = _T("");
	m_CmbIe.GetWindowText(strNewValue);
	m_dIe = _ttof(strNewValue);
}

void CDgnConCodeDlg::OnEditchangeCdCmb()
{
	CString strNewValue = _T("");
	m_CmbCd.GetWindowText(strNewValue);

	double dValue = _ttof(strNewValue);
	//   int nErrCode = 0;
	//   if(dValue<1 || !IsValidDoubleNumber(strNewValue, nErrCode))
	//   {
	//     strNewValue.Format(_T("%.2f"), m_dCd);
	//     m_CmbCd.SetWindowText(strNewValue);
	//     return;
	//   }

	m_dCd = dValue;
}

void CDgnConCodeDlg::OnEditchangeIeCmb()
{
	CString strNewValue = _T("");
	m_CmbIe.GetWindowText(strNewValue);

	double dValue = _ttof(strNewValue);
	//   int nErrCode = 0;
	//   if(dValue<1 || !IsValidDoubleNumber(strNewValue, nErrCode))
	//   {
	//     strNewValue.Format(_T("%.2f"), m_dIe);
	//     m_CmbIe.SetWindowText(strNewValue);
	//     return;
	//   }

	m_dIe = dValue;
}

void CDgnConCodeDlg::OnChangeExposureFactor()
{
	UpdateData(TRUE);
}

void CDgnConCodeDlg::OnChangePouringMethod()
{
	UpdateData(TRUE);
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CDgnConCodeDlg::IsValidDoubleNumber(CString value, int& nErrCode)
{
	value.TrimLeft();
	value.TrimRight();
	if (value == _T("")) return TRUE; // 공백은 0으로 생각하여 TRUE 리턴
	// 수식이면 변환, 수식이 아니면 TRUE 반환(수식은 첫자가 '=' 임)
	if (!ConvertFormulaToValue(value))
	{
		nErrCode = 1;
		return FALSE;
	}

	int ecode;
	// 숫자인지 문자열인지 검사
	if (!IsNumeric(value, ecode))
	{
		if (ecode == 1) nErrCode = 3;
		else nErrCode = 2;
		return FALSE;
	}

	TCHAR* num = new TCHAR[value.GetLength() + 1];
	TCHAR* endp;
	double n;

	wsprintf(num, value);
	n = wcstod(num, &endp);
	delete[] num;

	// 실수 범위 검사
	if (n == HUGE_VAL || n == -HUGE_VAL)
	{
		nErrCode = 3;
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnConCodeDlg::ConvertFormulaToValue(CString& value)
{
	if (value.GetLength() > 0 && value.GetAt(0) == _T('='))
	{
		CGXFormulaSheet fs;
		CGXFormula fm;
		if (!fs.ParseExpression(value, fm))
		{
			return FALSE;
		}
		if (!fs.EvaluateExpression(value, fm))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDgnConCodeDlg::IsNumeric(CString str, int& ecode)
{
	if (str.GetLength() < 1) return FALSE;

	TCHAR* num = new TCHAR[str.GetLength() + 1];
	TCHAR* endp;
	double n;

	wsprintf(num, str);
	n = wcstod(num, &endp);
	BOOL bAllScaned = endp == &num[str.GetLength()];
	delete[] num;

	if (n == HUGE_VAL || n == -HUGE_VAL)
		ecode = 1;
	else if (!bAllScaned)
		ecode = 2;
	else ecode = 0;

	if (ecode) return FALSE;
	else return TRUE;
}

void CDgnConCodeDlg::OnDgnCtrlDataAdvBtn()
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_IMPL_JUD_CALC_SECT_RC, 0), 0);
}

void CDgnConCodeDlg::OnConditionFactormb7mb8Chk()
{
	EnableDisableControls();
}

void CDgnConCodeDlg::OnConditionFactormb7mb8TypeRdo()
{
	EnableDisableControls();
}

void CDgnConCodeDlg::OnConditionFactormb9Chk()
{
	EnableDisableControls();
}

void CDgnConCodeDlg::OnConditionFactormb9TypeRdo()
{
	EnableDisableControls();
}

void CDgnConCodeDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_MB7_MB8_RDO1)->EnableWindow(m_bmb7mb8);
	GetDlgItem(IDC_DGN_MB7_MB8_RDO2)->EnableWindow(m_bmb7mb8);
	GetDlgItem(IDC_DGN_MB7_MB8_RDO3)->EnableWindow(m_bmb7mb8);
	GetDlgItem(IDC_DGN_MB7_MB8_RDO4)->EnableWindow(m_bmb7mb8);
	GetDlgItem(IDC_DGN_MB7_STC)->EnableWindow(m_bmb7mb8 && m_nmb7mb8Type == 3);
	GetDlgItem(IDC_DGN_MB7_EDT)->EnableWindow(m_bmb7mb8 && m_nmb7mb8Type == 3);
	GetDlgItem(IDC_DGN_MB8_STC)->EnableWindow(m_bmb7mb8 && m_nmb7mb8Type == 3);
	GetDlgItem(IDC_DGN_MB8_EDT)->EnableWindow(m_bmb7mb8 && m_nmb7mb8Type == 3);

	GetDlgItem(IDC_DGN_MB9_RDO1)->EnableWindow(m_bmb9);
	GetDlgItem(IDC_DGN_MB9_RDO2)->EnableWindow(m_bmb9);
	GetDlgItem(IDC_DGN_MB9_STC)->EnableWindow(m_bmb9 && m_nmb9Type == 1);
	GetDlgItem(IDC_DGN_MB9_EDT)->EnableWindow(m_bmb9 && m_nmb9Type == 1);

	//GetDlgItem(IDC_DGN_MB9_GRP2)->EnableWindow(m_chkIS3370);
	//GetDlgItem(IDC_DGN_MB9_CHK2)->EnableWindow(m_chkIS3370);
	GetDlgItem(IDC_DGN_IS456MAT_GROUP3)->EnableWindow(m_chkIS3370.GetCheck() && m_iIS3370Sel == 0);
	GetDlgItem(IDC_DGN_IS3370_N28)->EnableWindow(m_chkIS3370.GetCheck());
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC2)->EnableWindow(m_chkIS3370.GetCheck() && m_iIS3370Sel == 0);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR2)->EnableWindow(m_chkIS3370.GetCheck() && m_iIS3370Sel == 0);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC3)->EnableWindow(m_chkIS3370.GetCheck() && m_iIS3370Sel == 0);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR3)->EnableWindow(m_chkIS3370.GetCheck() && m_iIS3370Sel == 0);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC4)->EnableWindow(m_chkIS3370.GetCheck() && m_iIS3370Sel == 0);
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR4)->EnableWindow(m_chkIS3370.GetCheck() && m_iIS3370Sel == 0);

	GetDlgItem(IDC_DGN_IS3370_N29)->EnableWindow(m_chkIS3370.GetCheck());
	GetDlgItem(IDC_DGN_RCCTRL_IS456MAT_STATIC5)->EnableWindow(m_chkIS3370.GetCheck() && m_iIS3370Sel != 0);
	GetDlgItem(IDC_DGN_WALLTYPE_N15)->EnableWindow(m_chkIS3370.GetCheck() && m_iIS3370Sel != 0);
	GetDlgItem(IDC_DGN_IS456MAT_GROUP2)->EnableWindow(m_chkIS3370.GetCheck() && m_iIS3370Sel != 0);
}


void CDgnConCodeDlg::OnChangeIS13920_DuctileCode()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoIS1392Sel, m_iIs13920Sel);
	UpdateData(FALSE);
}


void CDgnConCodeDlg::OnBnClickedDgnIs204N25()
{
	// TODO: Add your control notification handler code here
}


void CDgnConCodeDlg::OnChangeIS3370_CrackCheck()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlIS3370_RadAB, m_iIS3370Sel);
	UpdateData(FALSE);

	EnableDisableControls();
}

void CDgnConCodeDlg::OnConditionIS3370TypeRdo()
{
	UpdateData(TRUE);
	m_iIS3370Sel = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlIS3370_RadAB, m_iIS3370Sel);
	//CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlIS3370_RadAB, m_iIS3370Sel);
	UpdateData(FALSE);
	EnableDisableControls();
}

void CDgnConCodeDlg::OnConditionIS3370AnnexBRdo()
{
	UpdateData(TRUE);
	m_iIS3370Sel = 1;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlIS3370_RadAB, m_iIS3370Sel);
	//CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlIS3370_RadAB, m_iIS3370Sel);
	UpdateData(FALSE);
	EnableDisableControls();
}
