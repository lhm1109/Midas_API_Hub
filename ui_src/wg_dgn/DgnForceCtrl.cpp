#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnForceCtrl.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\ArrayUtil.h"

#include "..\wg_base\wg_base_GTF.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\wg_db_MathLib.h"
#include "..\wg_db\wg_db_DesignResult.h"
#include "..\wg_db\wg_db_StoryData.h"
#include "..\wg_db\SCWBRatio.h"
#include "..\wg_db\IncoreSolver.h"
#include "..\wg_db\TempDataStore.h"
#include "..\wg_db\SeisEvalStruct.h"
#include "..\wg_db\DampDgnMgr_CH.h"

#include "DgnDataCtrl.h"
#include "DgnCodeCtrl.h"
#include "..\MIT_Lib\rptostream.h"
using namespace std;

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_HCBM

#include "..\wg_base\wg_base_ServiceProvider.h"
#include "..\wg_jud\Jud_ServiceDef.h"// ServiceAgencyJud사용

#include "..\..\dgnengine\idesign\DGN_lib\DGNCompare.h"

#include <tuple>
#include <vector>

#define _PROFILING
#include "profilewrite.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

enum EN_GB_SEISGRADE
{
	EN_GB_SEISGRADE_SPECIAL_GRADE1 = 1,
	EN_GB_SEISGRADE_FORT9_GRADE1,
	EN_GB_SEISGRADE_GRADE1,
	EN_GB_SEISGRADE_GRADE2,
	EN_GB_SEISGRADE_GRADE3,
	EN_GB_SEISGRADE_GRADE4
};

enum EN_DGNLCOM_IDX
{
	EN_DGNLOM_IDX_TOTL = 0,
	EN_DGNLOM_IDX_STRN,
	EN_DGNLOM_IDX_SERV,
	EN_DGNLOM_IDX_SPEC,
	EN_DGNLOM_IDX_VERT,
	EN_DGNLOM_IDX_STRN_UG,
	EN_DGNLOM_IDX_SERV_UG,
	EN_DGNLOM_IDX_SPEC_UG,
	EN_DGNLOM_IDX_END
};

CDgnForceCtrl::CDgnForceCtrl()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pDgnDataCtrl = new CDgnDataCtrl;

	m_iLcomStrn = 0;
	m_iLcomServ = 0;
	m_iLcomSpec = 0;
	m_iLcomVert = 0;
	m_iLcomStrnUG = 0;
	m_iLcomServUG = 0;
	m_iLcomSpecUG = 0;

	m_bOnlyLongTerm = FALSE;
	m_iLCBDesignNum = 0;
	m_iLCBServiceNum = 0;
	m_iLCBAddDesignNum = 0;
	m_nCode_Length = 0; m_nCode_Force = 0;
	m_nCode_Category = 0;
	m_strDgnCode = _T("");
	m_iDgnCode = 0;
	m_iDgnFrameType = 0;
	m_iDgnMembAxis = 0;
	m_iElemNo = 0;
	m_iElemType = 0;
	m_iElemKind = 0;
	m_iElemSubKind = 0;
	m_iElemSubKindAuto = 0;
	m_iPosiNo = 0;
	m_iPosiNoNM = 0;
	m_iPosiNoPM = 0;
	m_iPosiNoV = 0;
	m_strMatType = _T("");
	m_iSpectrumXNo = 0;
	m_iSpectrumYNo = 0;
	m_dSpectrumXFactor = 0.0;
	m_dSpectrumYFactor = 0.0;
	m_bEqSpecial = FALSE;
	m_bSectReduction = FALSE;
	m_arReduList.RemoveAll();
	m_dReduFactor = 0.0;
	m_bReduAxl = FALSE; m_bReduMom = FALSE; m_bReduShr = FALSE;
	m_bFreeTensComp = FALSE;
	m_iFreeTensComp = 0;
	m_dUserCtcUpFactor = 0.0;
	m_dUserSueqLcasN = 0.0;
	m_dUserSueqLcasM = 0.0;
	m_dUserSueqLcasV = 0.0;
	m_dUserSueqLcomN = 0.0;
	m_dUserSueqLcomM = 0.0;
	m_dUserSueqLcomV = 0.0;
	m_dSueqLcasN = 0.0; m_dSueqLcasM = 0.0; m_dSueqLcasV = 0.0;
	m_dSueqLcomN = 0.0; m_dSueqLcomM = 0.0; m_dSueqLcomV = 0.0;
	m_dCodeLcasN = 0.0; m_dCodeLcasM = 0.0; m_dCodeLcasV = 0.0;
	m_dCodeLcomN = 0.0; m_dCodeLcomM = 0.0; m_dCodeLcomV = 0.0;
	m_iCantilever = 0;
	m_bSubBeamForNoMpr = FALSE;
	m_bLowMembForNoMpr = FALSE;
	m_iOrgLcomKey = 0;
	m_bLL = FALSE; m_bWL = FALSE; m_bWLL = FALSE; m_bEL = FALSE; m_bTL = FALSE; m_bICE = FALSE;
	m_iAlphaType = 0;
	m_dAlpha1 = 0.0; m_dAlpha2 = 0.0, m_dAlpha2_Colm = 0.0;
	m_dOrgAlpha1 = 0.0; m_dOrgAlpha2 = 0.0, m_dOrgAlpha2_Colm = 0.0;

	m_bPSBeam = FALSE;
	m_bFromBiToUni = FALSE;

	m_bDampCheck = FALSE;

	m_parLcomData = NULL;
	m_arLcomTotl.InitHashTable(HASHSIZELCOM);
	m_arLcomStrn.InitHashTable(HASHSIZELCOM);
	m_arLcomSpec.InitHashTable(HASHSIZELCOM);
	m_arLcomVert.InitHashTable(HASHSIZELCOM);
	m_arLcomStrnUG.InitHashTable(HASHSIZELCOM);
	m_arLcomSpecUG.InitHashTable(HASHSIZELCOM);

	m_arStepData.InitHashTable(HASHSIZEELEM);

	// Add by ZINU.('07.07.31). NO:2921, Elem+Lcom Group(T_DLCM_D) Design for PDMS.
	m_arLcomGrup.InitHashTable(HASHSIZELCOM);

	m_arElemStlGrup.RemoveAll();
	m_arElemConGrup.RemoveAll();
	m_arElemConWallGrup.RemoveAll();
	m_arElemSrcGrup.RemoveAll();
	m_arElemAluGrup.RemoveAll();
	m_arElemScmGrup.RemoveAll();
	m_arElemCfsGrup.RemoveAll();

	m_arLcomNoGrup.InitHashTable(HASHSIZELCOM);

	_DGN_LCOM LcomDesign; LcomDesign.Initialize();
	CArray<T_DLCM_K, T_DLCM_K> arDlcmK;
	T_DLCM_D DlcmD;
	T_GRUP_D GrupD;
	m_pDoc->m_pAttrCtrl->GetDlcmKeyList(arDlcmK);

	int iLcomStlGrup = 0;
	int iLcomConGrup = 0;
	int iLcomSrcGrup = 0;
	int iLcomAluGrup = 0;
	int iLcomScmGrup = 0;
	int iLcomCfsGrup = 0;

	auto l_InitLcomGrup = [](const T_LCOM_K_LIST& aLcomK, OUT CArray<_DGN_LCOM_MAP*, _DGN_LCOM_MAP*>& raDgnLcom, OUT int& rnLcomGrup) -> int
	{
		const INT_PTR nLcom = aLcomK.GetSize();
		if (nLcom > 0)
		{
			_DGN_LCOM DgnLcom;
			_DGN_LCOM_MAP* pDgnLcomStlM = new _DGN_LCOM_MAP;
			for (INT_PTR m = 0; m < nLcom; ++m)
			{
				pDgnLcomStlM->SetAt(aLcomK[m], DgnLcom);
			}
			raDgnLcom.Add(pDgnLcomStlM);
			rnLcomGrup++;
		}
		return rnLcomGrup;
	};

	const INT_PTR nDlcm = arDlcmK.GetSize();
	for (INT_PTR i = 0; i < nDlcm; ++i)
	{
		if (m_pDoc->m_pAttrCtrl->GetDlcm(arDlcmK[i], DlcmD))
		{
			int nLcomStl = l_InitLcomGrup(DlcmD.aLcomK_stl, m_arLcomStlGrup, iLcomStlGrup); // Steel.
			int nLcomCon = l_InitLcomGrup(DlcmD.aLcomK_con, m_arLcomConGrup, iLcomConGrup); // Concrete.
			int nLcomSrc = l_InitLcomGrup(DlcmD.aLcomK_src, m_arLcomSrcGrup, iLcomSrcGrup); // SRC.
			int nLcomAlu = l_InitLcomGrup(DlcmD.aLcomK_alu, m_arLcomAluGrup, iLcomAluGrup); // Aluminum
			int nLcomScm = l_InitLcomGrup(DlcmD.aLcomK_scm, m_arLcomScmGrup, iLcomScmGrup); // Steel Composite
			int nLcomCfs = l_InitLcomGrup(DlcmD.aLcomK_cfs, m_arLcomCfsGrup, iLcomCfsGrup); // Cold Formed Steel

			// Set Elem <-> LcomGrup Serial No.
			const INT_PTR nGrup = DlcmD.aGrupK.GetSize();
			for (INT_PTR j = 0; j < nGrup; ++j)
			{
				T_GRUP_K GrupK = DlcmD.aGrupK[j];
				if (!m_pDoc->m_pAttrCtrl->GetGrup(GrupK, GrupD)) { continue; }
				// Set Elem Group.

				const INT_PTR nGrupElem = GrupD.arKeyElem.GetSize();
				for (INT_PTR k = 0; k < nGrupElem; ++k)
				{
					T_ELEM_K ElemK = GrupD.arKeyElem[k];
					// If Wall, Get Internal Wall ID.
					UINT nInternalWallId;
					BOOL bWall = FALSE;
					T_ELEM_D ElemD;
					ElemD.Initialize();
					m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
					if (m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))	// Wall.
					{
						if (!m_pDoc->m_pPostCtrl->GetInternalWallId(ElemK, nInternalWallId)) { continue; }
						ElemK = nInternalWallId;
						bWall = TRUE;
					}

					if (nLcomStl)	// Steel.
					{
						CIntMap amInt;
						m_arElemStlGrup.Lookup(ElemK, amInt);
						int iKey = iLcomStlGrup - 1;
						amInt.SetAt(iKey, iKey);	// Lcom Group SerialNo.
						m_arElemStlGrup.SetAt(ElemK, amInt);
					}
					if (nLcomCon)	// Concrete.
					{
						CIntMap amInt;
						if (!bWall)  m_arElemConGrup.Lookup(ElemK, amInt);
						else        m_arElemConWallGrup.Lookup(ElemK, amInt);
						int iKey = iLcomConGrup - 1;
						amInt.SetAt(iKey, iKey);	// Lcom Group SerialNo.
						if (!bWall)  m_arElemConGrup.SetAt(ElemK, amInt);
						else        m_arElemConWallGrup.SetAt(ElemK, amInt);
					}
					if (nLcomSrc)	// SRC.
					{
						CIntMap amInt;
						m_arElemSrcGrup.Lookup(ElemK, amInt);
						int iKey = iLcomSrcGrup - 1;
						amInt.SetAt(iKey, iKey);	// Lcom Group SerialNo.
						m_arElemSrcGrup.SetAt(ElemK, amInt);
					}
					if (nLcomAlu)	// ALU.
					{
						CIntMap amInt;
						m_arElemAluGrup.Lookup(ElemK, amInt);
						int iKey = iLcomAluGrup - 1;
						amInt.SetAt(iKey, iKey);	// Lcom Group SerialNo.
						m_arElemAluGrup.SetAt(ElemK, amInt);
					}
					if (nLcomScm)	// SCM.
					{
						CIntMap amInt;
						m_arElemScmGrup.Lookup(ElemK, amInt);
						int iKey = iLcomScmGrup - 1;
						amInt.SetAt(iKey, iKey);	// Lcom Group SerialNo.
						m_arElemScmGrup.SetAt(ElemK, amInt);
					}
					if (nLcomCfs)	// CFS.
					{
						CIntMap amInt;
						m_arElemCfsGrup.Lookup(ElemK, amInt);
						int iKey = iLcomCfsGrup - 1;
						amInt.SetAt(iKey, iKey);	// Lcom Group SerialNo.
						m_arElemCfsGrup.SetAt(ElemK, amInt);
					}
				}
			}
		}
	}

	m_dMomRedisFact = 0.0;
	m_dTorReduFact = 0.0;
	m_dGammard_GB10 = 0.0;
	m_dModMyi = 0.0;
	m_dModMyj = 0.0;

	for (int i = 0; i < 2; ++i)
	{
		m_apElemK_y[i].RemoveAll();
		m_apElemK_z[i].RemoveAll();
	}
	m_bExistLctb = FALSE;

	m_bExistLctb_CVL = FALSE;
	m_bUse_NLcomb = FALSE;

	m_FstlBaseMap.RemoveAll();
	m_FbemBaseMap.RemoveAll();
	m_FcolBaseMap.RemoveAll();
	m_FbrcBaseMap.RemoveAll();
	m_FwalBaseMap.RemoveAll();
	m_FscmBaseMap.RemoveAll();
	m_FsbmBaseMap.RemoveAll();

	m_arSlcmSds.InitHashTable(101);
	m_arSlcmSpecial.InitHashTable(101);
	m_arSlcmVertical.InitHashTable(101);
	m_arSlcfStldK.InitHashTable(HASHSIZELCOM);
	m_arSlcfSplcK.InitHashTable(HASHSIZELCOM);
	m_arSlcfStldLLK.InitHashTable(HASHSIZELCOM);

	m_arExistLL.RemoveAll();
	m_arExistEL.RemoveAll();
	m_mapSeisLoodCase.RemoveAll();
	m_bExistLL = FALSE;
	m_bExistEL = FALSE;

	// Add by GAY. PMS:3787. ('09.12.07).
	m_bExistSeis = FALSE;
	m_bExistSplc = FALSE;

	// Add by ZINU.('05.07.02). Consider Type for EL at Stage.
	T_STCT_D StctD;
	StctD.Initialize();
	BOOL bGetStct = m_pDoc->m_pAttrCtrl->GetStct(StctD);
	m_strStctEL.Empty();
	m_strStctEL2.Empty();
	m_strStctEL3.Empty();
	if (bGetStct == TRUE)
	{
		m_strStctEL = StctD.LoadCaseType_NotUse;	// D,DC,DW, L, T,TPG.
		m_strStctEL2 = StctD.LoadCaseType2_NotUse;
		m_strStctEL3 = StctD.LoadCaseType3_NotUse;
	}

	// Coded by Seungjun MNet:No.2591 ('20061130) For SCWB Design of Column.
	m_arEqBM.InitHashTable(HASHSIZEELEM);
	//   m_arGirMny.InitHashTable(HASHSIZEELEM);
	//   m_arGirMnz.InitHashTable(HASHSIZEELEM);
		// Coded by Seungjun ('20070302) MNet:No.2591.
	m_arGirMnyCW.InitHashTable(HASHSIZEELEM);
	m_arGirMnyCCW.InitHashTable(HASHSIZEELEM);
	m_arGirMnzCW.InitHashTable(HASHSIZEELEM);
	m_arGirMnzCCW.InitHashTable(HASHSIZEELEM);
	// Coded by Seungjun ('20070306) MNet:No.2591.
	m_arGirMpryCW.InitHashTable(HASHSIZEELEM);
	m_arGirMpryCCW.InitHashTable(HASHSIZEELEM);
	m_arGirMprzCW.InitHashTable(HASHSIZEELEM);
	m_arGirMprzCCW.InitHashTable(HASHSIZEELEM);

	m_arUpGirMnyCW.InitHashTable(HASHSIZEELEM);
	m_arUpGirMnyCCW.InitHashTable(HASHSIZEELEM);
	m_arUpGirMnzCW.InitHashTable(HASHSIZEELEM);
	m_arUpGirMnzCCW.InitHashTable(HASHSIZEELEM);
	// Coded by Seungjun ('20070306) MNet:No.2591.
	m_arUpGirMpryCW.InitHashTable(HASHSIZEELEM);
	m_arUpGirMpryCCW.InitHashTable(HASHSIZEELEM);
	m_arUpGirMprzCW.InitHashTable(HASHSIZEELEM);
	m_arUpGirMprzCCW.InitHashTable(HASHSIZEELEM);
	// PMS:3956-Seungjun-20090603 EC8:04 Seismic Design
	m_arGirAsTopy.InitHashTable(HASHSIZEELEM);
	m_arGirAsBoty.InitHashTable(HASHSIZEELEM);
	m_arGirAsTopy.InitHashTable(HASHSIZEELEM);
	m_arGirAsBotz.InitHashTable(HASHSIZEELEM);

	m_mBeamSumAsFyY.clear();
	m_mBeamSumAsFyZ.clear();

	m_arSeismicElemList.InitHashTable(HASHSIZEELEM);
	m_arSecondaryElemList.InitHashTable(HASHSIZEELEM);
	m_arNotSFRSElemList.InitHashTable(HASHSIZEELEM);
	m_mSeisClassFram.InitHashTable(HASHSIZEELEM);
	m_mSeisClassWall.InitHashTable(HASHSIZEELEM);
	m_mSeisClassSlab.InitHashTable(HASHSIZEELEM);

	// Coded by Seungjun MNet:No.2591 ('20061207)	For SCWB Design.
	m_iPMCurveType = 0;
	// Coded by Seungjun ('20070126) MNet:No.2619 For Taiwan. Design cosidered the compression rebars.
	//m_iDRBDType = DRBD_None;
	// Coded by Seungjun ('20070628) MNet:No.2831.
	CString strTitle = _T("");
	m_pDoc->GetDesignTitleGen(ID_DGN_GEN_SEIS, strTitle, m_bSclmCode);

	m_SpfcK = 0;
	m_dTB = 0.15;         // the lower limit of the period of the constant spectral acceleration branch
	m_dTC = 0.50;         // the upper limit of the period of the constant spectral acceleration branch
	m_dTD = 2.00;         // the value defining the beginning of the constant displacement response range of the spectrum
	m_dAgR = 0.08;        // reference peak ground acceleration on type A ground. unit : g
	m_dGamma_I = 1.0;    // important factor
	m_dDampingRat = 5.0; // viscous damping ration(%)
	m_dSoilF = 1.2;      // the soil factor
	m_daua1 = 1.0;         // Behaviour Factor
	m_dq0 = 3.0;
	m_dNuf = 0.6;
	m_dSCWBFactor = 1.3;
	m_iFrameType = 1;    // For EC8:04 0:Frame system 1:Dual system 2:Coupled wall system 3:Uncoupled wall system 4:Torsionally flexible system 5:Inverted pendulum system
	m_bJointDgn = FALSE;   // For EC8:04 Joint Design
	m_bNAEQDgn = FALSE;    // For EC8:04 National Annex. capacity design. ex) Italy:NTC2008.
	m_nNTCType = 0;       // NTC 2008 or NTC2012
	m_dEigenPeriod = 0.0;  // Eigen-value Period at 1st-Mode.
	m_dEigenXPeriod = 0.0;  // Eigen-value Period at 1st-Mode Global X direction.
	m_dEigenYPeriod = 0.0;  // Eigen-value Period at 1st-Mode Global Y direction..
	m_arWallMyFunc.InitHashTable(HASHSIZEELEM);
	m_arWallMzFunc.InitHashTable(HASHSIZEELEM);
	m_arWallFzFunc.InitHashTable(HASHSIZEELEM);
	m_dShearStrutAngle = 0.0;
	m_bAutoBehaviorFact = FALSE;
	m_dBehaviorFact_q = 0.0;
	m_dBehaviorFact_qo = 0.0;


	// EC2:04 for Slenderness limit for moment magnification.
	m_dAValue = 0.7;
	m_dBValue = 1.1;
	m_dCValue = 1.0;
	m_bAutoA = TRUE;
	m_bAutoC = TRUE;

	// PMS:4020-Seungjun-20090727 EC2:04 Slab/Wall Design.
	Init_PlateData();

	m_nCrackDef = 0;
	m_nLongTermType = 0;

	// PMS:4182-Seungjun-20100608 KBC2009 Seismic design.
	m_bAutoSeisSubBeam = FALSE;
	m_bAutoSeisCantilever = FALSE;
	m_bAutoSeisUnderBeamColm = FALSE;

	// Add by SHIN - 2011.06.14 : PMS:4215 : 일본2차설계시 1차설계에서 응력할증되는 Elem와 Posi정보
	m_ExpandElemD.Initialize();
	m_ExpandLoad.Initialize();
	// Seungjun-20110411 일본2차설계용 단부 거리 계산 방법. 0-강역(현재 그대로), 1-face to face(순길이), 2-node to node(모델링 길이)
	Init_LengType();

	// Add/Modify by Seungjun PMS.4511  '20120821 for JFD
	m_GroundBeamForce.Initialize();

	// Seungjun-20130821 - 하중조합내 포함된 하중케이스를 풀어 쓸건지 아닐지 판단하는 변수
	// 한번 세팅되면 변경되지 않으므로 생성자에서 세팅.
	// Gen에서는 NComb에 포함된 각각의 load type으로 부재력을 만듬
	// Civil/UMD에서는 NComb을 하나의 load type으로 생각하고 부재력을 만든다.
#if defined(_CIVIL)
	m_bUse_NLcomb = FALSE;
#else
	m_bUse_NLcomb = TRUE;
#endif

	m_mSbdoVectr.RemoveAll();
	m_bWoodArmer = TRUE;

	//BSC
	m_nBase_Length = 0;
	m_nBase_Force = 0;
	m_iDgnSubCode = RECOMMENDED;
	m_dGamma_rd_Joint = 0.0;
	m_BMuy = 0.0;
	m_BMuz = 0.0;

	for (int i = 0; i < CONST_DGN_iPOSNUM; ++i)
	{
		m_dMmaxML[i] = 0.0;
		m_dMminML[i] = 0.0;
		m_dMmaxN[i] = 0.0;
		m_dViN[i] = 0.0;
		m_dMmaxP[i] = 0.0;
		m_dViP[i] = 0.0;
		m_FxxLctb[i] = 0.0;
		m_FyyLctb[i] = 0.0;
		m_FzzLctb[i] = 0.0;
		m_BMxLctb[i] = 0.0;
		m_BMyLctb[i] = 0.0;
		m_BMzLctb[i] = 0.0;
		m_FxxUFLctb[i] = 0.0;
		m_FyyUFLctb[i] = 0.0;
		m_FxyUFLctb[i] = 0.0;
		m_VxxUFLctb[i] = 0.0;
		m_VyyUFLctb[i] = 0.0;
		m_MxxUFLctb[i] = 0.0;
		m_MyyUFLctb[i] = 0.0;
		m_MxyUFLctb[i] = 0.0;
		m_MbiLctb[i] = 0.0;
		m_TstLctb[i] = 0.0;
		m_TwpLctb[i] = 0.0;
	}

	m_bAbnormalForce = 0;
	m_iTowerElem = 0;

	for (int i = 0; i < 4; ++i)
	{
		m_dMxx[i] = 0.0;
		m_dMyy[i] = 0.0;
		m_dMxy[i] = 0.0;
		m_dFxx[i] = 0.0;
		m_dFyy[i] = 0.0;
		m_dFxy[i] = 0.0;
		m_dVxx[i] = 0.0;
		m_dVyy[i] = 0.0;
		m_dSigMaxTop[i] = 0.0;
		m_dSigMinTop[i] = 0.0;
		m_dSigMaxBot[i] = 0.0;
		m_dSigMinBot[i] = 0.0;
	}

	for (int i = 0; i < 2; ++i)
	{
		m_Fxx_Rcpl[i] = 0.0;
		m_Fyy_Rcpl[i] = 0.0;
		m_Fxy_Rcpl[i] = 0.0;
		m_Vxx_Rcpl[i] = 0.0;
		m_Vyy_Rcpl[i] = 0.0;
		m_Mxx_Rcpl[i] = 0.0;
		m_Myy_Rcpl[i] = 0.0;
		m_Mxy_Rcpl[i] = 0.0;
		m_FxxDL_Rcpl[i] = 0.0;
		m_FyyDL_Rcpl[i] = 0.0;
		m_FxyDL_Rcpl[i] = 0.0;
		m_VxxDL_Rcpl[i] = 0.0;
		m_VyyDL_Rcpl[i] = 0.0;
		m_MxxDL_Rcpl[i] = 0.0;
		m_MyyDL_Rcpl[i] = 0.0;
		m_MxyDL_Rcpl[i] = 0.0;
		m_FxxML_Rcpl[i] = 0.0;
		m_FyyML_Rcpl[i] = 0.0;
		m_FxyML_Rcpl[i] = 0.0;
		m_VxxML_Rcpl[i] = 0.0;
		m_VyyML_Rcpl[i] = 0.0;
		m_MxxML_Rcpl[i] = 0.0;
		m_MyyML_Rcpl[i] = 0.0;
		m_MxyML_Rcpl[i] = 0.0;
	}
	m_Rcpl_Strp.Initialize();

	m_iLcomKeyTran = 0;
	m_iStldKeySelf = 0;
	m_iRcsTypeKind = 0;
	m_iSrcTypeKind = 0;

	m_bChkStatic = FALSE;
	m_bEndLCase = FALSE;

	m_nMaxMinType = DGN_LCOM_MAX;

	m_nULSnELS = 0;
	m_nShearnAdhesive = 0;

	CString strWoodArmerBefore = CProduct::GetTestEnvValue(_ULS(WABeforeSmoothing)); // Yes면 부재력 우드아머 먼저 계산하고 스무딩
	SetWoodArmerBefore(strWoodArmerBefore.CompareNoCase(_ULS(yes)) == 0 ? TRUE : FALSE);

	m_bService = false;

	m_mAdjIntwall.clear();

	m_mElemMatd.clear();
	m_mWallMatd.clear();

	SetCurrentUnit();

	m_pAnalRsltConverter = nullptr;
	m_pAnalRsltCtrl = nullptr;
}

CDgnForceCtrl::~CDgnForceCtrl()
{
	// Add by ZINU.('07.08.06). NO:2921, delete Lcom Group.    
	const INT_PTR nLcomStl = m_arLcomStlGrup.GetSize();
	for (INT_PTR i = 0; i < nLcomStl; ++i)
	{
		delete m_arLcomStlGrup[i];
	}

	const INT_PTR nLcomCon = m_arLcomConGrup.GetSize();
	for (INT_PTR i = 0; i < nLcomCon; ++i)
	{
		delete m_arLcomConGrup[i];
	}

	const INT_PTR nLcomSrc = m_arLcomSrcGrup.GetSize();
	for (INT_PTR i = 0; i < nLcomSrc; ++i)
	{
		delete m_arLcomSrcGrup[i];
	}

	const INT_PTR nLcomAlu = m_arLcomAluGrup.GetSize();
	for (INT_PTR i = 0; i < nLcomAlu; ++i)
	{
		delete m_arLcomAluGrup[i];
	}

	const INT_PTR nLcomScm = m_arLcomScmGrup.GetSize();
	for (INT_PTR i = 0; i < nLcomScm; ++i)
	{
		delete m_arLcomScmGrup[i];
	}

	const INT_PTR nLcomCfs = m_arLcomCfsGrup.GetSize();
	for (INT_PTR i = 0; i < nLcomCfs; ++i)
	{
		delete m_arLcomCfsGrup[i];
	}

	delete m_pDgnDataCtrl;

	m_QuadTree.Initialize();

	Del_DgnAnalRslt();
}

// iOpt (0=Cmy,Cmz, 1=Cb).
// Change by cylee.('05.10.12). fabs re-coded -> re-delete sign and other fuctions that require sign are revised. see for example ACSCode_AASHTO_LFD96.cpp(497).
// Change by ZINU. ('05.07.27). fabs -> Maintain Sign.
double CDgnForceCtrl::Get_EndMy1(int iOpt) const { int i = (fabs(m_BMye[iOpt][m_iPosiNo][0]) < fabs(m_BMye[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMye[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMy2(int iOpt) const { int i = (fabs(m_BMye[iOpt][m_iPosiNo][0]) > fabs(m_BMye[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMye[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMz1(int iOpt) const { int i = (fabs(m_BMze[iOpt][m_iPosiNo][0]) < fabs(m_BMze[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMze[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMz2(int iOpt) const { int i = (fabs(m_BMze[iOpt][m_iPosiNo][0]) > fabs(m_BMze[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMze[iOpt][m_iPosiNo][i]); }

double CDgnForceCtrl::Get_EndMy1D(int iOpt) const { int i = (fabs(m_BMyeD[iOpt][m_iPosiNo][0]) < fabs(m_BMyeD[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMyeD[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMy2D(int iOpt) const { int i = (fabs(m_BMyeD[iOpt][m_iPosiNo][0]) > fabs(m_BMyeD[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMyeD[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMz1D(int iOpt) const { int i = (fabs(m_BMzeD[iOpt][m_iPosiNo][0]) < fabs(m_BMzeD[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMzeD[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMz2D(int iOpt) const { int i = (fabs(m_BMzeD[iOpt][m_iPosiNo][0]) > fabs(m_BMzeD[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMzeD[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMy1G(int iOpt) const { int i = (fabs(m_BMyeD[iOpt][m_iPosiNo][0] + m_BMyeL[iOpt][m_iPosiNo][0]) < fabs(m_BMyeD[iOpt][m_iPosiNo][4] + m_BMyeL[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMyeD[iOpt][m_iPosiNo][i] + m_BMyeL[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMy2G(int iOpt) const { int i = (fabs(m_BMyeD[iOpt][m_iPosiNo][0] + m_BMyeL[iOpt][m_iPosiNo][0]) > fabs(m_BMyeD[iOpt][m_iPosiNo][4] + m_BMyeL[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMyeD[iOpt][m_iPosiNo][i] + m_BMyeL[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMz1G(int iOpt) const { int i = (fabs(m_BMzeD[iOpt][m_iPosiNo][0] + m_BMzeL[iOpt][m_iPosiNo][0]) < fabs(m_BMzeD[iOpt][m_iPosiNo][4] + m_BMzeL[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMzeD[iOpt][m_iPosiNo][i] + m_BMzeL[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMz2G(int iOpt) const { int i = (fabs(m_BMzeD[iOpt][m_iPosiNo][0] + m_BMzeL[iOpt][m_iPosiNo][0]) > fabs(m_BMzeD[iOpt][m_iPosiNo][4] + m_BMzeL[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMzeD[iOpt][m_iPosiNo][i] + m_BMzeL[iOpt][m_iPosiNo][i]); }

double CDgnForceCtrl::Get_EndMy1DByMy(int iOpt) const { int i = (fabs(m_BMye[iOpt][m_iPosiNo][0]) < fabs(m_BMye[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMyeD[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMy2DByMy(int iOpt) const { int i = (fabs(m_BMye[iOpt][m_iPosiNo][0]) > fabs(m_BMye[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMyeD[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMz1DByMz(int iOpt) const { int i = (fabs(m_BMze[iOpt][m_iPosiNo][0]) < fabs(m_BMze[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMzeD[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMz2DByMz(int iOpt) const { int i = (fabs(m_BMze[iOpt][m_iPosiNo][0]) > fabs(m_BMze[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMzeD[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMy1GByMy(int iOpt) const { int i = (fabs(m_BMye[iOpt][m_iPosiNo][0] + m_BMye[iOpt][m_iPosiNo][0]) < fabs(m_BMye[iOpt][m_iPosiNo][4] + m_BMye[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMyeD[iOpt][m_iPosiNo][i] + m_BMyeL[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMy2GByMy(int iOpt) const { int i = (fabs(m_BMye[iOpt][m_iPosiNo][0] + m_BMye[iOpt][m_iPosiNo][0]) > fabs(m_BMye[iOpt][m_iPosiNo][4] + m_BMye[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMyeD[iOpt][m_iPosiNo][i] + m_BMyeL[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMz1GByMz(int iOpt) const { int i = (fabs(m_BMze[iOpt][m_iPosiNo][0] + m_BMze[iOpt][m_iPosiNo][0]) < fabs(m_BMze[iOpt][m_iPosiNo][4] + m_BMze[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMzeD[iOpt][m_iPosiNo][i] + m_BMzeL[iOpt][m_iPosiNo][i]); }
double CDgnForceCtrl::Get_EndMz2GByMz(int iOpt) const { int i = (fabs(m_BMze[iOpt][m_iPosiNo][0] + m_BMze[iOpt][m_iPosiNo][0]) > fabs(m_BMze[iOpt][m_iPosiNo][4] + m_BMze[iOpt][m_iPosiNo][4]) ? 0 : 4); return fabs(m_BMzeD[iOpt][m_iPosiNo][i] + m_BMzeL[iOpt][m_iPosiNo][i]); }

double CDgnForceCtrl::Get_EndMye(int iOpt, int iPart)	const { return m_BMye[iOpt][m_iPosiNo][iPart]; }
double CDgnForceCtrl::Get_EndMze(int iOpt, int iPart)	const { return m_BMze[iOpt][m_iPosiNo][iPart]; }
double CDgnForceCtrl::Get_EndMye(int iOpt, int iPosi, int iPart) const { return m_BMye[iOpt][iPosi][iPart]; }
double CDgnForceCtrl::Get_EndMze(int iOpt, int iPosi, int iPart) const { return m_BMze[iOpt][iPosi][iPart]; }
double CDgnForceCtrl::Get_EndMyeD(int iOpt, int iPart)	const { return m_BMyeD[iOpt][m_iPosiNo][iPart]; }
double CDgnForceCtrl::Get_EndMzeD(int iOpt, int iPart)	const { return m_BMzeD[iOpt][m_iPosiNo][iPart]; }
double CDgnForceCtrl::Get_EndMyeL(int iOpt, int iPart)	const { return m_BMyeL[iOpt][m_iPosiNo][iPart]; }
double CDgnForceCtrl::Get_EndMyeLU(int iOpt, int iPart)	const { return m_BMyeLU[iOpt][m_iPosiNo][iPart]; }// MQC.25444 Load cases using load combinations 부재력 Non-sway로 고려
double CDgnForceCtrl::Get_EndMzeL(int iOpt, int iPart)	const { return m_BMzeL[iOpt][m_iPosiNo][iPart]; }
double CDgnForceCtrl::Get_EndMzeLU(int iOpt, int iPart)	const { return m_BMzeLU[iOpt][m_iPosiNo][iPart]; }// MQC.25444 Load cases using load combinations 부재력 Non-sway로 고려
double CDgnForceCtrl::Get_EndMyeG(int iOpt, int iPart)	const { return m_BMyeD[iOpt][m_iPosiNo][iPart] + m_BMyeL[iOpt][m_iPosiNo][iPart]; }
double CDgnForceCtrl::Get_EndMzeG(int iOpt, int iPart)	const { return m_BMzeD[iOpt][m_iPosiNo][iPart] + m_BMzeL[iOpt][m_iPosiNo][iPart]; }
double CDgnForceCtrl::Get_EndMyeR(int iOpt, int iPart)	const { return m_BMyeR[iOpt][m_iPosiNo][iPart]; }
double CDgnForceCtrl::Get_EndMzeR(int iOpt, int iPart)	const { return m_BMzeR[iOpt][m_iPosiNo][iPart]; }
double CDgnForceCtrl::Get_EndMyeGL(int iOpt, int iPart)	const { return m_BMyeD[iOpt][m_iPosiNo][iPart] + m_BMyeL[iOpt][m_iPosiNo][iPart] + m_BMyeLU[iOpt][m_iPosiNo][iPart]; }
double CDgnForceCtrl::Get_EndMzeGL(int iOpt, int iPart)	const { return m_BMzeD[iOpt][m_iPosiNo][iPart] + m_BMzeL[iOpt][m_iPosiNo][iPart] + m_BMzeLU[iOpt][m_iPosiNo][iPart]; }

void CDgnForceCtrl::Reset_Force()
{
	Reset_ForceMajor();
	Reset_ForceMinor();

	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		m_Fxx[i] = 0.0;   m_BMx[i] = 0.0;
		m_FxxD[i] = 0.0;   m_BMxD[i] = 0.0;
		m_FxxL[i] = 0.0;   m_BMxL[i] = 0.0;
		m_FxxR[i] = 0.0;   m_BMxR[i] = 0.0;
		m_FxxML[i] = 0.0;   m_BMxML[i] = 0.0;
		m_FxxTP[i] = 0.0;   m_BMxTP[i] = 0.0;
		m_FxxTS[i] = 0.0;   m_BMxTS[i] = 0.0;
		m_FxxLU[i] = 0.0;   m_BMxLU[i] = 0.0;
		m_dVdu[i] = 0.0;   m_dMdu[i] = 0.0;	// Force by Unfactored Self Weight.
		m_dVdf[i] = 0.0;   m_dMdf[i] = 0.0;	// Force by Factored Self Weight.
		m_FxxGB[i] = 0.0;					  				// by 1.2*(D+0.5L) for GB50010-02.
		m_FzzIS[i] = 0.0;					  				// by 1.2*(D+L) for IS456-2000.
		// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
		m_Mbi[i] = 0.0;   m_Tst[i] = 0.0; m_Twp[i] = 0.0;
		m_MbiD[i] = 0.0;   m_TstD[i] = 0.0; m_TwpD[i] = 0.0;
		m_MbiL[i] = 0.0;   m_TstL[i] = 0.0; m_TwpL[i] = 0.0;
		m_MbiR[i] = 0.0;   m_TstR[i] = 0.0; m_TwpR[i] = 0.0;
		m_MbiML[i] = 0.0;   m_TstML[i] = 0.0; m_TwpML[i] = 0.0;
		m_MbiTP[i] = 0.0;   m_TstTP[i] = 0.0; m_TwpTP[i] = 0.0;
		m_MbiTS[i] = 0.0;   m_TstTS[i] = 0.0; m_TwpTS[i] = 0.0;
		m_MbiLU[i] = 0.0;   m_TstLU[i] = 0.0; m_TwpLU[i] = 0.0;
		/***
				// NOT Reset Because These values are saved MAX while Looping.
				m_dMmaxML[i]=0.0; m_dMminML[i]=0.0;
				m_dMmaxN[i]=0.0; m_dViN[i]=0.0;	// Negative Maximum Moment per Position.
				m_dMmaxP[i]=0.0; m_dViP[i]=0.0;	// Positive Maximum Moment per Position.
		***/
	}
	for (int i = 0; i < 5; i++)
	{
		m_Fxy_UF[i] = 0.0;
		m_FxyD_UF[i] = 0.0;
		m_FxyL_UF[i] = 0.0;
		m_FxyML_UF[i] = 0.0;
		m_FxyR_UF[i] = 0.0;
		m_FxyTP_UF[i] = 0.0;
		m_FxyTS_UF[i] = 0.0;
		m_FxyLU_UF[i] = 0.0;
		m_Mxy_UF[i] = 0.0;
		m_MxyD_UF[i] = 0.0;
		m_MxyL_UF[i] = 0.0;
		m_MxyML_UF[i] = 0.0;
		m_MxyR_UF[i] = 0.0;
		m_MxyTP_UF[i] = 0.0;
		m_MxyTS_UF[i] = 0.0;
		m_MxyLU_UF[i] = 0.0;
	}
}

void CDgnForceCtrl::Reset_ForceMajor()
{
	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		m_Fzz[i] = 0.0; m_BMy[i] = 0.0;
		m_FzzD[i] = 0.0; m_BMyD[i] = 0.0;
		m_FzzL[i] = 0.0; m_BMyL[i] = 0.0;
		m_FzzR[i] = 0.0; m_BMyR[i] = 0.0;
		m_FzzML[i] = 0.0; m_BMyML[i] = 0.0;
		m_FzzTP[i] = 0.0; m_BMyTP[i] = 0.0;
		m_FzzTS[i] = 0.0; m_BMyTS[i] = 0.0;
		m_FzzLU[i] = 0.0; m_BMyLU[i] = 0.0;
		m_FzzOrgR[i] = 0.0;

		for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			m_BMye[0][i][j] = 0.0; m_BMye[1][i][j] = 0.0;
			m_BMyeD[0][i][j] = 0.0; m_BMyeD[1][i][j] = 0.0;
			m_BMyeL[0][i][j] = 0.0; m_BMyeL[1][i][j] = 0.0;
			m_BMyeML[0][i][j] = 0.0; m_BMyeML[1][i][j] = 0.0;
			m_BMyeR[0][i][j] = 0.0; m_BMyeR[1][i][j] = 0.0;
			m_BMyeTP[0][i][j] = 0.0; m_BMyeTP[1][i][j] = 0.0;
			m_BMyeTS[0][i][j] = 0.0; m_BMyeTS[1][i][j] = 0.0;
			m_BMyeLU[0][i][j] = 0.0; m_BMyeLU[1][i][j] = 0.0;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		m_Fxx_UF[i] = 0.0;
		m_Vxx_UF[i] = 0.0;
		m_Mxx_UF[i] = 0.0;
		m_FxxD_UF[i] = 0.0;
		m_VxxD_UF[i] = 0.0;
		m_MxxD_UF[i] = 0.0;
		m_FxxL_UF[i] = 0.0;
		m_VxxL_UF[i] = 0.0;
		m_MxxL_UF[i] = 0.0;
		m_FxxML_UF[i] = 0.0;
		m_VxxML_UF[i] = 0.0;
		m_MxxML_UF[i] = 0.0;
		m_FxxR_UF[i] = 0.0;
		m_VxxR_UF[i] = 0.0;
		m_MxxR_UF[i] = 0.0;
		m_FxxTP_UF[i] = 0.0;
		m_VxxTP_UF[i] = 0.0;
		m_MxxTP_UF[i] = 0.0;
		m_FxxTS_UF[i] = 0.0;
		m_VxxTS_UF[i] = 0.0;
		m_MxxTS_UF[i] = 0.0;
		m_FxxLU_UF[i] = 0.0;
		m_VxxLU_UF[i] = 0.0;
		m_MxxLU_UF[i] = 0.0;
	}
	// Add by ZINU.('05.02.24). For Max/Min Moments at 1/2 of Beam.
	m_BMyMidUp = 0.0; m_BMyMidDn = 0.0;
}

void CDgnForceCtrl::Reset_ForceMinor()
{
	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		m_Fyy[i] = 0.0; m_BMz[i] = 0.0;
		m_FyyD[i] = 0.0; m_BMzD[i] = 0.0;
		m_FyyL[i] = 0.0; m_BMzL[i] = 0.0;
		m_FyyR[i] = 0.0; m_BMzR[i] = 0.0;
		m_FyyML[i] = 0.0; m_BMzML[i] = 0.0;
		m_FyyTP[i] = 0.0; m_BMzTP[i] = 0.0;
		m_FyyTS[i] = 0.0; m_BMzTS[i] = 0.0;
		m_FyyLU[i] = 0.0; m_BMzLU[i] = 0.0;

		// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
		m_FyyOrgR[i] = 0.0; //m_FzzOrgR[i]=0.0;
		for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			m_BMze[0][i][j] = 0.0; m_BMze[1][i][j] = 0.0;
			m_BMzeD[0][i][j] = 0.0; m_BMzeD[1][i][j] = 0.0;
			m_BMzeL[0][i][j] = 0.0; m_BMzeL[1][i][j] = 0.0;
			m_BMzeML[0][i][j] = 0.0; m_BMzeML[1][i][j] = 0.0;
			m_BMzeR[0][i][j] = 0.0; m_BMzeR[1][i][j] = 0.0;
			m_BMzeTP[0][i][j] = 0.0; m_BMzeTP[1][i][j] = 0.0;
			m_BMzeTS[0][i][j] = 0.0; m_BMzeTS[1][i][j] = 0.0;
			m_BMzeLU[0][i][j] = 0.0; m_BMzeLU[1][i][j] = 0.0;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		m_Fyy_UF[i] = 0.0;
		m_Vyy_UF[i] = 0.0;
		m_Myy_UF[i] = 0.0;
		m_FyyD_UF[i] = 0.0;
		m_VyyD_UF[i] = 0.0;
		m_MyyD_UF[i] = 0.0;
		m_FyyL_UF[i] = 0.0;
		m_VyyL_UF[i] = 0.0;
		m_MyyL_UF[i] = 0.0;
		m_FyyML_UF[i] = 0.0;
		m_VyyML_UF[i] = 0.0;
		m_MyyML_UF[i] = 0.0;
		m_FyyR_UF[i] = 0.0;
		m_VyyR_UF[i] = 0.0;
		m_MyyR_UF[i] = 0.0;
		m_FyyTP_UF[i] = 0.0;
		m_VyyTP_UF[i] = 0.0;
		m_MyyTP_UF[i] = 0.0;
		m_FyyTS_UF[i] = 0.0;
		m_VyyTS_UF[i] = 0.0;
		m_MyyTS_UF[i] = 0.0;
		m_FyyLU_UF[i] = 0.0;
		m_VyyLU_UF[i] = 0.0;
		m_MyyLU_UF[i] = 0.0;

		m_FxyUFOrgR[i] = 0.0;
		m_VxxUFOrgR[i] = 0.0;
		m_VyyUFOrgR[i] = 0.0;
	}

	// PMS:4020-Seungjun-20090708 Italy Slab/Wall Design.
	//InitPlateData();
}

void CDgnForceCtrl::Get_CTCAppliedForce(int iElemK)
{
	double dfactor = 1.0;
	m_pDgnDataCtrl->Get_CTCfactor4RCcolumnCivil(iElemK, m_nCode_Length, dfactor);

	// ForceMajor
	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		m_Fzz[i] *= dfactor;
		m_BMy[i] *= dfactor;
		for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			m_BMye[0][i][j] *= dfactor;
			m_BMye[1][i][j] *= dfactor;
		}
		MultiplyFzz(i, dfactor);
		MultiplyBMy(i, dfactor);
		MultiplyBMye(i, dfactor);
	}
	for (int i = 0; i < 5; i++)
	{
		m_Fxx_UF[i] *= dfactor;
		m_Vxx_UF[i] *= dfactor;
		m_Mxx_UF[i] *= dfactor;
		MultiplyFxxUF(i, dfactor);
		MultiplyVxxUF(i, dfactor);
		MultiplyMxxUF(i, dfactor);
	}

	// ForceMinor
	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		m_Fyy[i] *= dfactor;
		m_BMz[i] *= dfactor;
		for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			m_BMze[0][i][j] *= dfactor;
			m_BMze[1][i][j] *= dfactor;
		}
		MultiplyFyy(i, dfactor);
		MultiplyBMz(i, dfactor);
		MultiplyBMze(i, dfactor);
		// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
		m_FyyOrgR[i] = 0.0;
		m_FzzOrgR[i] = 0.0;
	}
	for (int i = 0; i < 5; i++)
	{
		m_Fyy_UF[i] *= dfactor;
		m_Vyy_UF[i] *= dfactor;
		m_Myy_UF[i] *= dfactor;
		MultiplyFyyUF(i, dfactor);
		MultiplyVyyUF(i, dfactor);
		MultiplyMyyUF(i, dfactor);

		m_FxyUFOrgR[i] *= dfactor;
		m_VxxUFOrgR[i] *= dfactor;
		m_VyyUFOrgR[i] *= dfactor;
	}

	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		m_Fxx[i] *= dfactor;
		m_BMx[i] *= dfactor;
		MultiplyFxx(i, dfactor);
		MultiplyBMx(i, dfactor);

		// Force by Unfactored Self Weight.
		m_dVdu[i] *= dfactor;
		m_dMdu[i] *= dfactor;
		// Force by Factored Self Weight.
		m_dVdf[i] *= dfactor;
		m_dMdf[i] *= dfactor;
		m_FxxGB[i] *= dfactor; // by 1.2*(D+0.5L) for GB50010-02.

		// Add by GAY. ('13.05.28). PMS:4632. Torsion + Warping for 7 DOF.        
		m_Mbi[i] *= dfactor;
		m_Tst[i] *= dfactor;
		m_Twp[i] *= dfactor;
		MultiplyMbi(i, dfactor);
		MultiplyTst(i, dfactor);
		MultiplyTwp(i, dfactor);
	}

	for (int i = 0; i < 5; i++)
	{
		m_Fxy_UF[i] *= dfactor;
		m_Mxy_UF[i] *= dfactor;
		MultiplyFxyUF(i, dfactor);
		MultiplyMxyUF(i, dfactor);
	}
}

void CDgnForceCtrl::Reset_AxialForceForTensComp(int iPosiNo, double dAxialStrn)
{
	// Add by ZINU.('01.12.06). To Tens/Comp only element force by User.
	// 0:General, 1:Tens-only, 2:Comp-only.
	if (m_iFreeTensComp == 1 || m_iFreeTensComp == 2)
	{
		double dCompRat = dAxialStrn * 0.0001;
		double dAxlForc = m_Fxx[iPosiNo];
		BOOL bAxlLimOK = (m_bFreeTensComp ? TRUE : (dCompRat > fabs(dAxlForc) ? TRUE : FALSE));
		BOOL bReset = FALSE;
		if (m_iFreeTensComp == 1 && dAxlForc < 0.0 && bAxlLimOK)	bReset = TRUE;
		if (m_iFreeTensComp == 2 && dAxlForc > 0.0 && bAxlLimOK)	bReset = TRUE;
		if (bReset)
		{
			m_Fxx[iPosiNo] = 0.0;
			m_FxxD[iPosiNo] = 0.0;
			m_FxxL[iPosiNo] = 0.0;
			m_FxxR[iPosiNo] = 0.0;
			m_FxxML[iPosiNo] = 0.0;
			m_FxxTP[iPosiNo] = 0.0;
			m_FxxTS[iPosiNo] = 0.0;
			m_FxxLU[iPosiNo] = 0.0;
		}
	}
}

void CDgnForceCtrl::Reset_MomentFromBiToUni(int iPosiNo)
{
	if (!m_bFromBiToUni)	return;

	auto l_MomentSRSS = [](double* adMy, double* adMz, int nPos) -> double
	{
		return sqrt(pow(adMy[nPos], 2) + pow(adMz[nPos], 2));
	};

	// Moments.    
	m_BMy[iPosiNo] = l_MomentSRSS(m_BMy, m_BMz, iPosiNo);
	m_BMyD[iPosiNo] = l_MomentSRSS(m_BMyD, m_BMzD, iPosiNo);
	m_BMyL[iPosiNo] = l_MomentSRSS(m_BMyL, m_BMzL, iPosiNo);
	m_BMyR[iPosiNo] = l_MomentSRSS(m_BMyR, m_BMzR, iPosiNo);
	m_BMyML[iPosiNo] = l_MomentSRSS(m_BMyML, m_BMzML, iPosiNo);
	m_BMyTP[iPosiNo] = l_MomentSRSS(m_BMyTP, m_BMzTP, iPosiNo);
	m_BMyTS[iPosiNo] = l_MomentSRSS(m_BMyTS, m_BMzTS, iPosiNo);
	m_BMyLU[iPosiNo] = l_MomentSRSS(m_BMyLU, m_BMzLU, iPosiNo);
	m_BMz[iPosiNo] = 0.0;
	m_BMzD[iPosiNo] = 0.0;
	m_BMzL[iPosiNo] = 0.0;
	m_BMzR[iPosiNo] = 0.0;
	m_BMzML[iPosiNo] = 0.0;
	m_BMzTP[iPosiNo] = 0.0;
	m_BMzTS[iPosiNo] = 0.0;
	m_BMzLU[iPosiNo] = 0.0;

	// End Moments.
	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		m_BMye[0][iPosiNo][i] = l_MomentSRSS(m_BMye[0][iPosiNo], m_BMze[0][iPosiNo], i);
		m_BMye[1][iPosiNo][i] = l_MomentSRSS(m_BMye[1][iPosiNo], m_BMze[1][iPosiNo], i);
		m_BMyeD[0][iPosiNo][i] = l_MomentSRSS(m_BMyeD[0][iPosiNo], m_BMzeD[0][iPosiNo], i);
		m_BMyeD[1][iPosiNo][i] = l_MomentSRSS(m_BMyeD[1][iPosiNo], m_BMzeD[1][iPosiNo], i);
		m_BMyeL[0][iPosiNo][i] = l_MomentSRSS(m_BMyeL[0][iPosiNo], m_BMzeL[0][iPosiNo], i);
		m_BMyeL[1][iPosiNo][i] = l_MomentSRSS(m_BMyeL[1][iPosiNo], m_BMzeL[1][iPosiNo], i);
		m_BMyeML[0][iPosiNo][i] = l_MomentSRSS(m_BMyeML[0][iPosiNo], m_BMzeML[0][iPosiNo], i);
		m_BMyeML[1][iPosiNo][i] = l_MomentSRSS(m_BMyeML[1][iPosiNo], m_BMzeML[1][iPosiNo], i);
		m_BMyeR[0][iPosiNo][i] = l_MomentSRSS(m_BMyeR[0][iPosiNo], m_BMzeR[0][iPosiNo], i);
		m_BMyeR[1][iPosiNo][i] = l_MomentSRSS(m_BMyeR[1][iPosiNo], m_BMzeR[1][iPosiNo], i);
		m_BMyeTP[0][iPosiNo][i] = l_MomentSRSS(m_BMyeTP[0][iPosiNo], m_BMzeTP[0][iPosiNo], i);
		m_BMyeTP[1][iPosiNo][i] = l_MomentSRSS(m_BMyeTP[1][iPosiNo], m_BMzeTP[1][iPosiNo], i);
		m_BMyeTS[0][iPosiNo][i] = l_MomentSRSS(m_BMyeTS[0][iPosiNo], m_BMzeTS[0][iPosiNo], i);
		m_BMyeTS[1][iPosiNo][i] = l_MomentSRSS(m_BMyeTS[1][iPosiNo], m_BMzeTS[1][iPosiNo], i);
		m_BMyeLU[0][iPosiNo][i] = l_MomentSRSS(m_BMyeLU[0][iPosiNo], m_BMzeLU[0][iPosiNo], i);
		m_BMyeLU[1][iPosiNo][i] = l_MomentSRSS(m_BMyeLU[1][iPosiNo], m_BMzeLU[1][iPosiNo], i);
		m_BMze[0][iPosiNo][i] = 0.0;
		m_BMze[1][iPosiNo][i] = 0.0;
		m_BMzeD[0][iPosiNo][i] = 0.0;
		m_BMzeD[1][iPosiNo][i] = 0.0;
		m_BMzeL[0][iPosiNo][i] = 0.0;
		m_BMzeL[1][iPosiNo][i] = 0.0;
		m_BMzeML[0][iPosiNo][i] = 0.0;
		m_BMzeML[1][iPosiNo][i] = 0.0;
		m_BMzeR[0][iPosiNo][i] = 0.0;
		m_BMzeR[1][iPosiNo][i] = 0.0;
		m_BMzeTP[0][iPosiNo][i] = 0.0;
		m_BMzeTP[1][iPosiNo][i] = 0.0;
		m_BMzeTS[0][iPosiNo][i] = 0.0;
		m_BMzeTS[1][iPosiNo][i] = 0.0;
		m_BMzeLU[0][iPosiNo][i] = 0.0;
		m_BMzeLU[1][iPosiNo][i] = 0.0;
	}
}

void CDgnForceCtrl::Reset_MomentRedistribution()
{
	if (m_strMatType != _T("C"))				return;
	if (m_dMomRedisFact <= 0.0)	return;

	BOOL bChinaCode = (m_iDgnCode == GB50010_02 || m_iDgnCode == GB50010_10 || m_iDgnCode == GB50010_19) ? TRUE : FALSE;
	// Only Beam Moment about Major Axis.
	double dMyi = (bChinaCode ? m_BMyD[0] + m_BMyL[0] : m_BMy[0]);	// Member I.
	double dMyj = (bChinaCode ? m_BMyD[4] + m_BMyL[4] : m_BMy[4]);	// Member J.
	// End Moments.
	double dModMyi = 0.0, dModMyj = 0.0;
	if (dMyi < 0.0 && dMyj < 0.0)	// Remember Member End(I,J).
	{
		dModMyi = fabs(dMyi) * (1.0 - m_dMomRedisFact);
		dModMyj = fabs(dMyj) * (1.0 - m_dMomRedisFact);
	}
	else if (dMyi < 0.0)
	{
		dModMyi = fabs(dMyi) * (1.0 - m_dMomRedisFact);
		dModMyj = fabs(dMyi) * (1.0 - m_dMomRedisFact);
	}
	else if (dMyj < 0.0)
	{
		dModMyi = fabs(dMyj) * (1.0 - m_dMomRedisFact);
		dModMyj = fabs(dMyj) * (1.0 - m_dMomRedisFact);
	}
	// Moments.
	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		double dCurMy = (bChinaCode ? m_BMyD[i] + m_BMyL[i] : m_BMy[i]);
		double dModMy = dModMyi + i * (dModMyj - dModMyi) / (double)(CONST_DGN_iPOSNUM - 1);
		double dRatMy = (dCurMy == 0.0 ? 1.0 : (dCurMy + dModMy) / dCurMy);	// Existing Sign(+/-).
		if (bChinaCode)
		{
			m_BMy[i] += dModMy;
			m_BMyD[i] *= dRatMy;
			m_BMyL[i] *= dRatMy;
			m_BMyR[i] *= 1.0;
			m_BMyML[i] *= 1.0;
			m_BMyTP[i] *= 1.0;
			m_BMyTS[i] *= 1.0;
			m_BMyLU[i] *= 1.0;
			// Add by ZINU.('05.09.08). Set Max/Min-M at Member(1/2).
			if (i == 2)	// For Mid-Region.
			{
				m_BMyMidUp += dModMy;	// Existing Sign(+/-).
				m_BMyMidDn += dModMy;	// Existing Sign(+/-).
			}
		}
		else
		{
			m_BMy[i] *= dRatMy;
			m_BMyD[i] *= dRatMy;
			m_BMyL[i] *= dRatMy;
			m_BMyR[i] *= dRatMy;
			m_BMyML[i] *= dRatMy;
			m_BMyTP[i] *= dRatMy;
			m_BMyTS[i] *= dRatMy;
			m_BMyLU[i] *= dRatMy;
			// Add by ZINU.('05.09.08). Set Max/Min-M at Member(1/2).
			if (i == 2)	// For Mid-Region.
			{
				m_BMyMidUp *= (m_BMyMidUp == 0.0 ? 1.0 : (m_BMyMidUp + dModMy) / m_BMyMidUp);	// Existing Sign(+/-).
				m_BMyMidDn *= (m_BMyMidDn == 0.0 ? 1.0 : (m_BMyMidDn + dModMy) / m_BMyMidDn);	// Existing Sign(+/-).
			}
		}
	}
}

// Add by GAY. MNET:3780. ('09.03.20). Apply Torsion Reduction.
void CDgnForceCtrl::Reset_TorsionReduction()
{
	if (m_strMatType != _T("C")) return;
	if (m_dTorReduFact <= 0.0) return;
	if (!CDgnCodeCtrl::IsConCodeTorsionCheck(m_iDgnCode)) return; // Seungjun '151126 PMS.5165 ACI318-08, 11 보 비틀림 설계 추가

	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		m_BMx[i] *= m_dTorReduFact;
		MultiplyBMx(i, m_dTorReduFact);
	}
}

void CDgnForceCtrl::Reset_UncertainlyLoadCombFact(int iPosiNo)
{
	if (m_strMatType != _T("C")) return;
	if (m_dGammard_GB10 <= 0.0) return;
	if (!(m_iDgnCode == GB50010_10 || m_iDgnCode == GB50010_19)) return;

	double dGammard = m_dGammard_GB10;
	// Forces.
	MultiplyFxx(iPosiNo, dGammard);
	MultiplyFyy(iPosiNo, dGammard);
	MultiplyFzz(iPosiNo, dGammard);
	// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
	m_FyyOrgR[iPosiNo] *= dGammard;
	m_FzzOrgR[iPosiNo] *= dGammard;
	// Moments.
	MultiplyBMx(iPosiNo, dGammard);
	MultiplyBMy(iPosiNo, dGammard);
	MultiplyBMz(iPosiNo, dGammard);
	// Add by GAY. ('13.05.09). PMS:4632. Member force for 7-DOF elements.	
	MultiplyMbi(iPosiNo, dGammard);
	MultiplyTst(iPosiNo, dGammard);
	MultiplyTwp(iPosiNo, dGammard);
	// End Moments.
	MultiplyBMye(iPosiNo, dGammard);
	MultiplyBMze(iPosiNo, dGammard);
}


bool CDgnForceCtrl::Set_LcomDataForDesign(int iLcomKind, BOOL bIncludeService, BOOL bWall/*=FALSE*/, int iDgnNo/*=-1*/, BOOL bStre/*TRUE*/, BOOL bServ/*TRUE*/)
{
	//(2009.10.30) Modify by Unsang :: EC2-2:05에서는 기본적으로 bIncludeService가 TRUE이나, 
	//                                 StrnLcom이 존재하지 않을 경우 ServLcom이 존재해도 bIncludeService는 FALSE로 처리함.
	// Agree with DKLEE.

	// iLcomKind = D_LCOMTYPE_STEEL, D_LCOMTYPE_CONCRETE, D_LCOMTYPE_SRC, D_LCOMTYPE_ALUMINUM, D_LCOMTYPE_STLCOMP
	// Save Total Load Combination Name by Serial Number(No LcomKey).	

	 // bCivil_KCI_USD12의 경우는 다시확인!! Parkks
	BOOL bIsCivil = FALSE;
#if defined(_CIVIL)
	bIsCivil = TRUE;
#endif
	BOOL bCivil_KCI_USD12 = (bIsCivil && m_iDgnCode == KCI_USD12) ? TRUE : FALSE;

	CArray<T_LCOM_K, T_LCOM_K> arLcomKey;
	arLcomKey.RemoveAll();
	int iTotalLcom = 0;
	if ((m_iDgnCode == KSCE_RC_USD05 || m_iDgnCode == KSCE_RC_RAIL_USD04 || m_iDgnCode == KCI_USD07_CVL || bCivil_KCI_USD12) && iLcomKind == D_LCOMTYPE_CONCRETE)
	{ //여기는 Civil전용임.
		// KSCE-USD05, 
		if (iDgnNo == D_RC_GEN)
		{
			CArray<T_LCOM_K, T_LCOM_K> arLcomTotalKey;
			m_pDoc->m_pAttrCtrl->GetLcomKeyList(iLcomKind, arLcomTotalKey);
			T_LCOM_D LcomData;
			int nSize = static_cast<int>(arLcomTotalKey.GetSize());
			if (bStre)
			{
				for (int i = 0; i < nSize; i++)
				{
					LcomData.Initialize();
					m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, arLcomTotalKey[i], LcomData);
					if (LcomData.nActive == 0)	continue;
					if (LcomData.LoadCombType != 0) continue;
					if (LcomData.nActive == 1)
						arLcomKey.Add(arLcomTotalKey[i]);
				}
			}
			if (bServ)
			{
				for (int i = 0; i < nSize; i++)
				{
					LcomData.Initialize();
					m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, arLcomTotalKey[i], LcomData);
					if (LcomData.nActive == 0)	continue;
					if (LcomData.LoadCombType != 0) continue;
					if (LcomData.nActive == 2)
						arLcomKey.Add(arLcomTotalKey[i]);
				}
			}
			iTotalLcom = static_cast<int>(arLcomKey.GetSize());
		}
		else if (iDgnNo == D_RC_BEAM)
		{
			T_RCBG_D RcbgD;
			m_pDoc->m_pAttrCtrl->GetRcbg(RcbgD);
			arLcomKey.Append(RcbgD.StreLcom);
			arLcomKey.Append(RcbgD.ServLcom);
			iTotalLcom = static_cast<int>(arLcomKey.GetSize());
		}
		else if (iDgnNo == D_RC_COLUMN) // Column
		{
			T_RCCG_D RccgD;
			m_pDoc->m_pAttrCtrl->GetRccg(RccgD);
			arLcomKey.Append(RccgD.StreLcom);
			arLcomKey.Append(RccgD.ServLcom);
			iTotalLcom = static_cast<int>(arLcomKey.GetSize());
		}
		else if (iDgnNo == D_RC_PLATE) // Plate(Rcpl)
		{
			T_RCPG_D RcpgD;
			m_pDoc->m_pAttrCtrl->GetRcpg(RcpgD);
			arLcomKey.Append(RcpgD.StreLcom);
			arLcomKey.Append(RcpgD.ServLcom);
			iTotalLcom = static_cast<int>(arLcomKey.GetSize());
		}
		else if (iDgnNo == D_RC_WALL) // Wall(Frame)
		{
			T_RCWG_D RcwgD;
			m_pDoc->m_pAttrCtrl->GetRcwg(RcwgD);
			arLcomKey.Append(RcwgD.StreLcom);
			arLcomKey.Append(RcwgD.ServLcom);
			iTotalLcom = static_cast<int>(arLcomKey.GetSize());
		}
		// Add by GAY. MNET:2668/2673. ('07.07.11). Sort Lcom Key List.
		qsort(arLcomKey.GetData(), arLcomKey.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
	}
	else if ((m_iDgnCode == KSCE_ASD05 || m_iDgnCode == KSCE_RAIL_ASD04 || m_iDgnCode == KSCE_ASD10 || m_iDgnCode == KSCE_RAIL_ASD11 || m_iDgnCode == KR_BRG_LSD15 || m_iDgnCode == KDS_24_14_31_2018 ||
		m_iDgnCode == KDS_24_14_30_2019) && iLcomKind == D_LCOMTYPE_STEEL)
	{
		m_iLcomStrn = 0;
		m_iLcomServ = 0;
		m_iLcomSpec = 0;
		m_iLcomVert = 0;
		m_iLcomStrnUG = 0;
		m_iLcomServUG = 0;
		m_iLcomSpecUG = 0;
		m_iLCBDesignNum = 0;
		m_iLCBServiceNum = 0;
		m_iLCBAddDesignNum = 0;

		if (iDgnNo == -1)
		{
			if (m_iDgnCode == KSCE_ASD10 || m_iDgnCode == KDS_24_14_30_2019)
			{
				T_LCOM_D LcomD;
				CArray<T_LCOM_K, T_LCOM_K> arTmpLcomK; arTmpLcomK.RemoveAll();
				m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STEEL, arTmpLcomK);
				int nSize = static_cast<int>(arTmpLcomK.GetSize());
				for (int i = 0; i < nSize; ++i)
				{
					LcomD.Initialize();
					T_LCOM_K LcomK = arTmpLcomK[i];
					if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, LcomK, LcomD)) { continue; }
					else
					{
						if (LcomD.nActive == 1) { arLcomKey.Add(LcomK); }
					}
				}
			}
			else if (m_iDgnCode == KR_BRG_LSD15 || m_iDgnCode == KDS_24_14_31_2018 )
			{
				T_LCMT_D LcmtD;
				if (m_pDoc->m_pAttrCtrl2->GetLcmt(LcmtD))
				{
					INT_PTR nSize = LcmtD.Base[0].aStrength.GetSize();
					for (int i = 0; i < nSize; ++i)
					{
						arLcomKey.Add(LcmtD.Base[0].aStrength[i]);
					}
				}
				else
				{
					T_LCOM_D LcomD;
					CArray<T_LCOM_K, T_LCOM_K> arTmpLcomK; arTmpLcomK.RemoveAll();
					m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STEEL, arTmpLcomK);
					INT_PTR nSize = arTmpLcomK.GetSize();
					for (INT_PTR i = 0; i < nSize; ++i)
					{
						LcomD.Initialize();
						T_LCOM_K LcomK = arTmpLcomK[i];
						if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, LcomK, LcomD)) { continue; }
						else
						{
							if (LcomD.nActive == 1) { arLcomKey.Add(LcomK); }
						}
					}
				}
			}
			else
			{
				T_STLC_D StlcD;
				StlcD.Initialize();
				m_pDoc->m_pAttrCtrl->GetStlc(StlcD);
				arLcomKey.Copy(StlcD.StreLcom);
			}
		}
		else
		{
			CArray<T_LCOM_K, T_LCOM_K> arLcomTotalKey;
			m_pDoc->m_pAttrCtrl->GetLcomKeyList(iLcomKind, arLcomTotalKey);
			T_LCOM_D LcomData;
			int nSize = arLcomTotalKey.GetSize();
			for (int i = 0; i < nSize; i++)
			{
				if (m_iDgnCode == KR_BRG_LSD15 || m_iDgnCode == KDS_24_14_31_2018 )
				{
					arLcomKey.Add(arLcomTotalKey[i]);
				}
				else
				{
					LcomData.Initialize();
					m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, arLcomTotalKey[i], LcomData);
					if (LcomData.nActive == 0)	continue;
					if (LcomData.LoadCombType != 0) continue;
					if (LcomData.nActive == 1)
						arLcomKey.Add(arLcomTotalKey[i]);
				}
			}
		}
		iTotalLcom = static_cast<int>(arLcomKey.GetSize());
		// Add by GAY. MNET:2729. ('07.07.11). Sort Lcom Key List.
		qsort(arLcomKey.GetData(), arLcomKey.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
	}
	else
	{
		BOOL bSlab = m_iRcsTypeKind / 10 == 8 ? TRUE : FALSE;
		BOOL bMeshWall = m_iRcsTypeKind / 10 == 9 ? TRUE : FALSE;
		BOOL bStrip = (m_iRcsTypeKind == RCS_STCK || m_iRcsTypeKind == RCS_STDN) ? TRUE : FALSE;
		BOOL bShell = m_iRcsTypeKind / 10 == 70 ? TRUE : FALSE;
		// PMS:4020-Seungjun-20090910 EC2:04 Slab/Wall Design.
		// 선택된 하중조합만 가져옴. T_MLCB_D이 없을 경우 DL, LL 하중조합만 설계수행.
		if (bSlab || bMeshWall || bStrip || bShell)  // Slab design or meshed wall.
		{
			iTotalLcom = GetLcomKeyListMeshedMemb(m_iRcsTypeKind, m_nCrackDef, iLcomKind, arLcomKey);
		}
		else
		{
			m_pDoc->m_pAttrCtrl->GetLcomKeyList(iLcomKind, arLcomKey);
			iTotalLcom = m_pDoc->m_pAttrCtrl->GetCountLcom(iLcomKind);
		}
	}

	if (iTotalLcom > 0)	Write_GDisplayAnalString(_LS(IDS_DGN_COM_START_CREATE_LCOM), FALSE, TRUE);

	// Modify by GAY. PMS:3787. ('09.12.07). 정적/동적 지진하중 입력 여부 확인.  
	m_bExistSplc = m_pDoc->m_pAttrCtrl->GetCountSplc() > 0 ? TRUE : FALSE;
	m_bExistSeis = m_pDoc->m_pAttrCtrl->GetCountSeis() > 0 ? TRUE : FALSE;

	m_arLcomTotl.RemoveAll();
	m_arLcomStrn.RemoveAll();
	m_arLcomSpec.RemoveAll();
	m_arLcomVert.RemoveAll();
	m_arLcomStrnUG.RemoveAll();
	m_arLcomSpecUG.RemoveAll();

	// Save Active Load Combination Name.
	int iLcomAllDgnNum = 0;	// Active All Lcom.	
	CArray<T_LCOM_K, T_LCOM_K> arActiveLcomKey;

	// Eurocode2-2:05에서는 StrnLcom이 존재할 경우에만 service Chk를 수행함.
	if (IsIncludeServiceCode())
	{
		bIncludeService = FALSE;
		for (int i = 0; i < iTotalLcom; ++i)
		{
			T_LCOM_D LcomD;
			if (!m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, arLcomKey.GetAt(i), LcomD)) { continue; }

			if (IsLcomStrn(LcomD.nActive))
			{
				bIncludeService = TRUE;
				break;
			}
			else
			{
				continue;
			}
		}
	}

	for (int i = 0; i < iTotalLcom; ++i)
	{
		const T_LCOM_K LcomK = arLcomKey[i];
		T_LCOM_D LcomD;
		if (!m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, LcomK, LcomD)) { continue; }
		// Check if Serviceability is used.

		BOOL bLcomOK = FALSE;
		if (m_iDgnCode == KR_BRG_LSD15 || m_iDgnCode == KDS_24_14_31_2018 )
		{
			arActiveLcomKey.Add(LcomK);
			iLcomAllDgnNum++;
		}
		else
		{
			if (iLcomKind == D_LCOMTYPE_LINEAR_EVAL || iLcomKind == D_LCOMTYPE_SEISMIC)
			{
				bLcomOK = (LcomD.nActive > 0) ? TRUE : FALSE;
			}
			else
			{
				if (bIncludeService)
				{
					bLcomOK = (LcomD.nActive > 0 ? TRUE : FALSE);
				}
				else
				{
					bLcomOK = IsLcomStrn(LcomD.nActive) ? TRUE : FALSE;
				}
			}

			// Change by ZINU.('07.07.31). NO:2903, No Include if SRSS, Envelope.
			if (bLcomOK && !(LcomD.LoadCombType == 1 || LcomD.LoadCombType == 3))
			{				
				// Need not NormalizeLcom() because it gets LoadCombName.
				arActiveLcomKey.Add(arLcomKey.GetAt(i));
				iLcomAllDgnNum++;
			}
		}
		// Change by ZINU.('05.06.15). nActive (0=Inactive, 1=Strength/Stress, 2=Serviceability, 3=Special, 4=Vertical).
		GDisplayAnalStringSamePosFormat(_LS(IDS_DGN_COM_READ_LCB), i + 1, iTotalLcom);
	}

	GDisplayAnalString(_T(""));

	if (iLcomAllDgnNum == 0)
	{
		return false;
	}

	// Add by ZINU.('05.06.15). Consider Existing LL, EL.
	m_arExistLL.RemoveAll();
	m_arExistEL.RemoveAll();
	m_mapSeisLoodCase.RemoveAll();
	// Add by ZINU.('05.05.11). Special Load Combination Factor.
	m_arSlcfStldK.RemoveAll();
	m_arSlcfSplcK.RemoveAll();
	m_arSlcfStldLLK.RemoveAll();
	m_arSlcmSds.RemoveAll();
	m_arSlcmSpecial.RemoveAll();
	m_arSlcmVertical.RemoveAll();
	m_mapSlcmType.RemoveAll();
	m_mapSlcmTypeStrip.RemoveAll();
	m_mUlct.RemoveAll();

	// Coded by Seungjun ('20070529) MNet:No.XXXX. //^0^//
	BOOL bIncludeLcom = FALSE;
	switch (m_iDgnCode)
	{
	case KSSC_ASD03:
	case KSSC_LSD09:
	case KSSC_LSD16:
	case KDS_41_31_2019:
	case KDS_41_30_10_2022:
	case KCI_USD03:
	case KCI_USD07:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case AIK_SRC2K:
	case ACI318_05:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case AISC_LRFD10:
	case AISC_ASD10:
	case AISC_LRFD16:
	case AISC_ASD16:
	case AISC_LRFD22:
	case AISC_ASD22:
	case NSR_10:
	case NSCP_2015_C:
	case IS456_2000:
	case NTC_DCEC_2017:
	case NTC_DCEC_2023:
	case TWN_USD112:
	case IS800_2007:
		bIncludeLcom = TRUE;
		break;
	default:
		break;
	}

	SetSlcmTypeMap(m_mapSlcmType);
	SetSpecVertLcomInfoMap(bWall);
	SetUlctMap(m_mUlct);

	// Modify, Jaeoh. [2/4/2009]
	BOOL bCheckCraneLcom = FALSE;
#if defined(_CIVIL)
	bCheckCraneLcom = FALSE;
#else
	bCheckCraneLcom = TRUE;
#endif

	m_bExistCraneLcom = FALSE;
	m_bChkConCurForce12 = FALSE;

	// PMS:3956-Seungjun-20090629 EC8:04 Seismic Design.
	m_bNTC08Wall = FALSE;
	// Wall, Eurocode2:04, NTC2008, Capaciti Design, Strength Type
	if (m_iDgnCode == Eurocode2_04 && m_iDgnSubCode == ITALY && m_bNAEQDgn && (m_nNTCType == RCS_NTC08 || m_nNTCType == RCS_NTC12) && m_iRcsTypeKind / 10 == 4 && IsEqSpecialProv())
	{
		double dq = Get_qForEC4();
		// q>2.0
		m_bNTC08Wall = dq > 2.0 ? TRUE : FALSE;
	}

	CMapEx<T_LCOM_K, T_LCOM_K, T_LCOM_K, T_LCOM_K&> mStrengthLcomKey;
	CMapEx<T_LCOM_K, T_LCOM_K, T_LCOM_K, T_LCOM_K&> mFatigueLcomKey;
	if (m_iDgnCode == SNiP_20503_84RC || m_iDgnCode == SP_35_13330_11RC || m_iDgnCode == SNiP_20503_84RC_MKS || m_iDgnCode == SP_35_13330_11RC_MKS)
	{
		T_LCMT_D LcmtD; LcmtD.Initialize();
		if (!m_pDoc->m_pAttrCtrl2->GetLcmt(LcmtD))
		{
			m_pDoc->m_pAttrCtrl2->GetLcmtbyLcomType(D_LCOMTYPE_CONCRETE, LcmtD);
		}

		int nStrnLcomSize = LcmtD.Base[1].aStrength.GetSize();
		for (int i = 0; i < nStrnLcomSize; ++i)
		{
			T_LCOM_K nStrnKey = LcmtD.Base[1].aStrength[i];
			mStrengthLcomKey.SetAt(nStrnKey, nStrnKey);
		}

		int nFatiLcomSize = LcmtD.Base[1].aFatigue.GetSize();
		for (int i = 0; i < nFatiLcomSize; ++i)
		{
			T_LCOM_K nFatiKey = LcmtD.Base[1].aFatigue[i];
			mFatigueLcomKey.SetAt(nFatiKey, nFatiKey);
		}
	}

	int anLcomSizeLcom[EN_DGNLOM_IDX_END] = { 0, };
	for (int i = 0; i < iLcomAllDgnNum; i++)
	{
		const T_LCOM_K CurLcomK = arActiveLcomKey.GetAt(i);
		T_LCOM_D LcomD;
		// Get LCB Data by LcomName.
		m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, CurLcomK, LcomD);

		T_LCOM_D_UL	LcomUlData;
		if (!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomD, LcomUlData)) { ASSERT(0); continue; }

		// LCTB Normalize
		CArray<T_LCOM_BASE, T_LCOM_BASE&> LctbCombination;
		GetLcomDataNormailizedLctb(LcomUlData.Combination, LctbCombination);

		// Moving Load Flag.(if Exist, it is TRUE)
		BOOL bMaxMinLcase = m_pDoc->m_pAttrCtrl->IsMaxMinComb(LcomD);
		BOOL bExistCrane = bCheckCraneLcom ? m_pDoc->m_pAttrCtrl->IsCraneComb(LcomD) : FALSE;
		BOOL bConCurrent12 = m_pDoc->m_pAttrCtrl->IsConCurrentComb12(LcomD);
		BOOL bStlCodeCur12 = IsSteelConcurrentForceCode() ? TRUE : FALSE;

#if !defined(_CIVIL)
		bConCurrent12 = bConCurrent12 && bStlCodeCur12;
#endif

		int iDgnLcomType = 0;
		int iSeismicType = 0;
		// Get Loadcase Number of this LcomUlData.
		// Set Lcom Type by Load case(D,L,W,E,...).
		SetLcomTypeIni();
		for (int j = 0; j < LctbCombination.GetSize(); j++)
		{
			SetLcomTypeGen(LctbCombination[j], iDgnLcomType, LcomUlData.LoadCombId);
			SetLcomTypeJTJ(LctbCombination[j], iDgnLcomType);
			SetLcomTypeIRC(LctbCombination[j], iDgnLcomType);
			SetLcomTypeAST(LctbCombination[j], iDgnLcomType);
			SetSeismicType(LctbCombination[j], iSeismicType);
		}

		// PMS:3956-Seungjun-20090629 EC8:04 Seismic Design.
		BOOL bNTCMinMax = FALSE;
		if (m_bNTC08Wall && iSeismicType > 0 && !IsLcomServ(LcomUlData.nActive))
		{
			bNTCMinMax = TRUE;
		}

		T_LCOM_K nTempStrnLcomK = 0;
		T_LCOM_K nTempFatiLcomK = 0;
		if (m_iDgnCode == SNiP_20503_84RC || m_iDgnCode == SP_35_13330_11RC || m_iDgnCode == SNiP_20503_84RC_MKS || m_iDgnCode == SP_35_13330_11RC_MKS)
		{
			if (LcomUlData.nActive == 1 &&
				!(mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK) == TRUE ||
					mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK) == TRUE)) continue;
		}

		// Add by ZINU.('02.12.03). For Lateral Displacement Control.
		BOOL bLongTerm = (iDgnLcomType % 10 == 0 ? TRUE : FALSE);
		BOOL bAddLcom = (((m_bOnlyLongTerm && bLongTerm) || !m_bOnlyLongTerm) ? TRUE : FALSE);

		_DGN_LCOM LcomDesign;
		LcomDesign.Initialize();
		if (bAddLcom && bMaxMinLcase == FALSE && bNTCMinMax == FALSE)  // PMS:3956-Seungjun-20090629 EC8:04 Seismic Design.
		{
			// Primarily make the same Number Load Combination before Design.			
			LcomDesign.nLcomKind = iLcomKind;
			LcomDesign.DesignLcomNa.Format(_T("%4d"), LcomUlData.LoadCombId);
			LcomDesign.bMaxLcom = TRUE;
			LcomDesign.OriginalLcomKey = CurLcomK;
			LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUlData;
			LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
			LcomDesign.iServiceLcomType = LcomUlData.nServLcomType;
			LcomDesign.dAllowableStressFactor = LcomUlData.dLcomFactor;
			if (mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK)) LcomDesign.nStrnSubType = 1;
			if (mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK)) LcomDesign.nFatigueType = 1;

			SetDgnLcomMap(LcomDesign, bIncludeLcom, anLcomSizeLcom); // Save Lcom Data for Design.

			GDisplayAnalStringSamePosFormat(_LS(IDS_DGN_COM_CREATE_LCB), anLcomSizeLcom[EN_DGNLOM_IDX_TOTL]);
		}
		else if (bAddLcom && bMaxMinLcase == TRUE)
		{
			if (bExistCrane || bConCurrent12)
			{
				int iCompSize = GetForceComponentSize(bExistCrane, bConCurrent12, m_iDgnCode, iDgnNo == D_RC_PLATE_BC ? TRUE : FALSE);

				CArray<CString, CString&> strComp;
				GetLcomCompTypeString(iDgnNo, strComp);
				// If Crane Load is exist, Consider Concurrent Member Force.        
				for (int j = 0; j < iCompSize * 2; j++)
				{
					LcomDesign.nLcomKind = iLcomKind;
					CString strSign = (j < iCompSize) ? _T("+") : _T("-");
					LcomDesign.DesignLcomNa.Format(_T("%4d%s%s"), LcomUlData.LoadCombId, strSign, strComp[j%iCompSize]);
					LcomDesign.bMaxLcom = (j < iCompSize) ? TRUE : FALSE;
					LcomDesign.OriginalLcomKey = CurLcomK;
					LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
					LcomDesign.bLongTerm = bLongTerm;
					LcomDesign.iDgnLcomType = iDgnLcomType;
					LcomDesign.iSeismicType = iSeismicType;
					LcomDesign.LcomUlData = LcomUlData;
					LcomDesign.iLcomCompType = (j%iCompSize) + 1;
					LcomDesign.iServiceLcomType = LcomUlData.nServLcomType;
					LcomDesign.dAllowableStressFactor = LcomUlData.dLcomFactor;
					if (mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK)) LcomDesign.nStrnSubType = 1;
					if (mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK)) LcomDesign.nFatigueType = 1;

					SetDgnLcomMap(LcomDesign, bIncludeLcom, anLcomSizeLcom);
				}
			}
			else
			{
				// Primarily make the same Number Load Combination before Design.
				// Set LcomNa = LcomNo + _T("+(Max)/-(Min)")
				CString strLcomNa = _T("");
				strLcomNa.Format(_T("%3d"), LcomUlData.LoadCombId);
				LcomDesign.nLcomKind = iLcomKind;
				LcomDesign.DesignLcomNa = strLcomNa + _T("+");
				LcomDesign.bMaxLcom = TRUE;
				LcomDesign.OriginalLcomKey = CurLcomK;
				LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
				LcomDesign.bLongTerm = bLongTerm;
				LcomDesign.iDgnLcomType = iDgnLcomType;
				LcomDesign.iSeismicType = iSeismicType;
				LcomDesign.LcomUlData = LcomUlData;
				LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
				LcomDesign.iServiceLcomType = LcomUlData.nServLcomType;
				LcomDesign.dAllowableStressFactor = LcomUlData.dLcomFactor;
				if (mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK)) LcomDesign.nStrnSubType = 1;
				if (mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK)) LcomDesign.nFatigueType = 1;

				// Save Lcom Data for Design.
				SetDgnLcomMap(LcomDesign, bIncludeLcom, anLcomSizeLcom);

				/*
				strOut.Format(_LS(IDS_DGN_COM_CREATE_LCB), iLcomTotl);
				GDisplayAnalStringSamePos(strOut);
				*/
				// if Moving Load exists, Add Load Combination at Last.
				LcomDesign.nLcomKind = iLcomKind;
				LcomDesign.DesignLcomNa = strLcomNa + _T("-");
				LcomDesign.bMaxLcom = FALSE;
				LcomDesign.OriginalLcomKey = CurLcomK;
				LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
				LcomDesign.bLongTerm = bLongTerm;
				LcomDesign.iDgnLcomType = iDgnLcomType;
				LcomDesign.iSeismicType = iSeismicType;
				LcomDesign.LcomUlData = LcomUlData;
				LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
				LcomDesign.iServiceLcomType = LcomUlData.nServLcomType;
				LcomDesign.dAllowableStressFactor = LcomUlData.dLcomFactor;
				if (mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK)) LcomDesign.nStrnSubType = 1;
				if (mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK)) LcomDesign.nFatigueType = 1;

				// Save Lcom Data for Design.
				SetDgnLcomMap(LcomDesign, bIncludeLcom, anLcomSizeLcom);

				CString strOut = _T(""); strOut.Format(_LS(IDS_DGN_COM_CREATE_LCB), anLcomSizeLcom[EN_DGNLOM_IDX_TOTL]);
				GDisplayAnalStringSamePos(strOut);
			}
		}
		else if (bAddLcom && bNTCMinMax == TRUE)
		{
			// Primarily make the same Number Load Combination before Design.
			// Set LcomNa = LcomNo + _T("+(Max)/-(Min)")
			LcomDesign.nLcomKind = iLcomKind;
			CString strLcomNa = _T("");
			strLcomNa.Format(_T("%3d"), LcomUlData.LoadCombId);
			LcomDesign.DesignLcomNa = strLcomNa + _T("+");
			LcomDesign.bMaxLcom = TRUE;
			LcomDesign.OriginalLcomKey = CurLcomK;
			LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUlData;
			LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
			LcomDesign.iServiceLcomType = LcomUlData.nServLcomType;
			if (mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK)) LcomDesign.nStrnSubType = 1;
			if (mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK)) LcomDesign.nFatigueType = 1;

			// Save Lcom Data for Design.
			SetDgnLcomMap(LcomDesign, bIncludeLcom, anLcomSizeLcom);

			// if Moving Load exists, Add Load Combination at Last.
			LcomDesign.nLcomKind = iLcomKind;
			LcomDesign.DesignLcomNa = strLcomNa + _T("-");
			LcomDesign.bMaxLcom = FALSE;
			LcomDesign.OriginalLcomKey = CurLcomK;
			LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUlData;
			LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
			LcomDesign.iServiceLcomType = LcomUlData.nServLcomType;
			if (mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK)) LcomDesign.nStrnSubType = 1;
			if (mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK)) LcomDesign.nFatigueType = 1;

			// Save Lcom Data for Design.
			SetDgnLcomMap(LcomDesign, bIncludeLcom, anLcomSizeLcom);

			CString strOut = _T(""); strOut.Format(_LS(IDS_DGN_COM_CREATE_LCB), anLcomSizeLcom[EN_DGNLOM_IDX_TOTL]);
			GDisplayAnalStringSamePos(strOut);
		}
	}
	// Calculate Load Combination Number for Design.
	m_parLcomData = &m_arLcomTotl;
	m_iLCBAddDesignNum = anLcomSizeLcom[EN_DGNLOM_IDX_TOTL];
	m_iLcomStrn = anLcomSizeLcom[EN_DGNLOM_IDX_STRN];
	m_iLcomServ = anLcomSizeLcom[EN_DGNLOM_IDX_SERV];
	m_iLcomSpec = anLcomSizeLcom[EN_DGNLOM_IDX_SPEC];
	m_iLcomVert = anLcomSizeLcom[EN_DGNLOM_IDX_VERT];
	m_iLcomStrnUG = anLcomSizeLcom[EN_DGNLOM_IDX_STRN_UG];
	m_iLcomServUG = anLcomSizeLcom[EN_DGNLOM_IDX_SERV_UG];
	m_iLcomSpecUG = anLcomSizeLcom[EN_DGNLOM_IDX_SPEC_UG];

	if (iTotalLcom > 0)	GDisplayAnalString(_LS(IDS_DGN_COM_END_CREATE_LCOM));

	// Coded by Seungjun ('20070807) MNet:No.2921.    
	_DGN_LCOM_MAP* paLcom[5] = { &m_arLcomStrn, &m_arLcomSpec, &m_arLcomVert, &m_arLcomStrnUG, &m_arLcomSpecUG };
	for (auto paLcomSrc : paLcom)
	{
		POSITION PosLcom = paLcomSrc->GetStartPosition();
		while (PosLcom)
		{
			T_LCOM_K nIndexLcom = 0;
			_DGN_LCOM LcomDesignSrc;
			paLcomSrc->GetNextAssoc(PosLcom, nIndexLcom, LcomDesignSrc);

			CArray<_DGN_LCOM_MAP*, _DGN_LCOM_MAP*> aDgnLcomMap;
			if (!GetDgnLcomGroupMap(iLcomKind, aDgnLcomMap))
			{
				continue;
			}

			bool bInclude = false;
			const INT_PTR nSizeGrupCurr = aDgnLcomMap.GetSize();
			for (INT_PTR j = 0; j < nSizeGrupCurr; ++j)
			{
				// Target.
				_DGN_LCOM_MAP* paLcomTar = aDgnLcomMap[j];
				_DGN_LCOM LcomDesignTar;
				if (paLcomTar->Lookup(LcomDesignSrc.OriginalLcomKey, LcomDesignTar))
				{
					bInclude = true;
				}
			}
			if (!bInclude)
			{
				m_arLcomNoGrup.SetAt(LcomDesignSrc.OriginalLcomKey, LcomDesignSrc);
			}
		}
	}
	return true;
}

//Damp Performance-Dgn CH
void CDgnForceCtrl::Set_LcomDataDamp_CH(int iLcomKind, BOOL bIncludeService, BOOL bWall/*=FALSE*/, int iDgnNo/*=-1*/, BOOL bStre/*TRUE*/, BOOL bServ/*TRUE*/)
{
	BOOL bIsCivil = FALSE;
	BOOL bCivil_KCI_USD12 = FALSE;

	CArray<T_LCOM_D, T_LCOM_D &> aLcomD;
	if (!CDampDesignMgr_CH::Instance()->GetDampLoadComb(aLcomD))
	{
		return;
	}

	int iTotalLcom = aLcomD.GetSize();
	if (iTotalLcom > 0)	Write_GDisplayAnalString(_LS(IDS_DGN_COM_START_CREATE_LCOM), FALSE, TRUE);

	m_bExistSplc = m_pDoc->m_pAttrCtrl->GetCountSplc() > 0 ? TRUE : FALSE;
	m_bExistSeis = m_pDoc->m_pAttrCtrl->GetCountSeis() > 0 ? TRUE : FALSE;

	m_arLcomTotl.RemoveAll();
	m_arLcomStrn.RemoveAll();
	m_arLcomSpec.RemoveAll();
	m_arLcomVert.RemoveAll();
	m_arLcomStrnUG.RemoveAll();
	m_arLcomSpecUG.RemoveAll();

	// Save Active Load Combination Name.
	int iLcomAllDgnNum = iTotalLcom;	// Active All Lcom.	
	CArray<T_LCOM_K, T_LCOM_K> arActiveLcomKey;

	for (int i = 0; i < iTotalLcom; ++i)
	{
		const T_LCOM_K LcomK = aLcomD[i].LoadCombId;
		T_LCOM_D LcomD = aLcomD[i];

		BOOL bLcomOK = FALSE;
		if (bIncludeService)
		{
			bLcomOK = (LcomD.nActive > 0 ? TRUE : FALSE);
		}
		else
		{
			bLcomOK = IsLcomStrn(LcomD.nActive) ? TRUE : FALSE;
		}

		// No Include if SRSS, Envelope.
		//if (bLcomOK && !(LcomD.LoadCombType == 1 || LcomD.LoadCombType == 3))
		//{
		//	// Need not NormalizeLcom() because it gets LoadCombName.
		//	arActiveLcomKey.Add(arLcomKey.GetAt(i));
		//	iLcomAllDgnNum++;
		//}
		// Change by ZINU.('05.06.15). nActive (0=Inactive, 1=Strength/Stress, 2=Serviceability, 3=Special, 4=Vertical).
		GDisplayAnalStringSamePosFormat(_LS(IDS_DGN_COM_READ_LCB), i + 1, iTotalLcom);
	}

	GDisplayAnalString(_T(""));


	//Consider Existing LL, EL.
	m_arExistLL.RemoveAll();
	m_arExistEL.RemoveAll();
	m_mapSeisLoodCase.RemoveAll();
	//Special Load Combination Factor.
	m_arSlcfStldK.RemoveAll();
	m_arSlcfSplcK.RemoveAll();
	m_arSlcfStldLLK.RemoveAll();
	m_arSlcmSds.RemoveAll();
	m_arSlcmSpecial.RemoveAll();
	m_arSlcmVertical.RemoveAll();
	m_mapSlcmType.RemoveAll();
	m_mapSlcmTypeStrip.RemoveAll();
	m_mUlct.RemoveAll();

	BOOL bIncludeLcom = FALSE;

	SetSlcmTypeMap(m_mapSlcmType);
	SetSpecVertLcomInfoMap(bWall);
	SetUlctMap(m_mUlct);

	BOOL bCheckCraneLcom = TRUE;

	m_bExistCraneLcom = FALSE;
	m_bChkConCurForce12 = FALSE;

	// PMS:3956-Seungjun-20090629 EC8:04 Seismic Design.
	m_bNTC08Wall = FALSE;

	CMapEx<T_LCOM_K, T_LCOM_K, T_LCOM_K, T_LCOM_K&> mStrengthLcomKey;
	CMapEx<T_LCOM_K, T_LCOM_K, T_LCOM_K, T_LCOM_K&> mFatigueLcomKey;

	int anLcomSizeLcom[EN_DGNLOM_IDX_END] = { 0, };
	for (int i = 0; i < iLcomAllDgnNum; i++)
	{
		const T_LCOM_K CurLcomK = aLcomD[i].LoadCombId;
		T_LCOM_D& LcomD = aLcomD[i];

		T_LCOM_D_UL	LcomUlData;
		if (!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomD, LcomUlData)) { ASSERT(0); continue; }

		for(int j = LcomUlData.Combination.GetSize() - 1; j >= 0; j--)
		{
			if(AbsEQ(LcomUlData.Combination[j].Factor, 0.0))
				LcomUlData.Combination.RemoveAt(j);
		}
		if(LcomUlData.Combination.IsEmpty())
		{
			ASSERT(FALSE);
			continue;
		}

		// LCTB Normalize
		CArray<T_LCOM_BASE, T_LCOM_BASE&> LctbCombination;
		GetLcomDataNormailizedLctb(LcomUlData.Combination, LctbCombination);

		// Moving Load Flag.(if Exist, it is TRUE)
		BOOL bMaxMinLcase = FALSE;// m_pDoc->m_pAttrCtrl->IsMaxMinComb(LcomD);
		BOOL bExistCrane = bCheckCraneLcom ? m_pDoc->m_pAttrCtrl->IsCraneComb(LcomD) : FALSE;
		BOOL bConCurrent12 = m_pDoc->m_pAttrCtrl->IsConCurrentComb12(LcomD);
		BOOL bStlCodeCur12 = FALSE;

		bConCurrent12 = bConCurrent12 && bStlCodeCur12;

		int iDgnLcomType = 0;
		int iSeismicType = 0;
		// Get Loadcase Number of this LcomUlData.
		// Set Lcom Type by Load case(D,L,W,E,...).
		SetLcomTypeIni();
		for (int j = 0; j < LctbCombination.GetSize(); j++)
		{
			SetLcomTypeGen(LctbCombination[j], iDgnLcomType, LcomUlData.LoadCombId);
			SetLcomTypeJTJ(LctbCombination[j], iDgnLcomType);
			SetLcomTypeIRC(LctbCombination[j], iDgnLcomType);
			SetLcomTypeAST(LctbCombination[j], iDgnLcomType);
			SetSeismicType(LctbCombination[j], iSeismicType);
		}

		// PMS:3956-Seungjun-20090629 EC8:04 Seismic Design.
		BOOL bNTCMinMax = FALSE;
		if (m_bNTC08Wall && iSeismicType > 0 && !IsLcomServ(LcomUlData.nActive))
		{
			bNTCMinMax = TRUE;
		}

		T_LCOM_K nTempStrnLcomK = 0;
		T_LCOM_K nTempFatiLcomK = 0;

		// Add by ZINU.('02.12.03). For Lateral Displacement Control.
		BOOL bLongTerm = (iDgnLcomType % 10 == 0 ? TRUE : FALSE);
		BOOL bAddLcom = (((m_bOnlyLongTerm && bLongTerm) || !m_bOnlyLongTerm) ? TRUE : FALSE);

		_DGN_LCOM LcomDesign;
		LcomDesign.Initialize();
		if (bAddLcom && bMaxMinLcase == FALSE && bNTCMinMax == FALSE)  // PMS:3956-Seungjun-20090629 EC8:04 Seismic Design.
		{
			// Primarily make the same Number Load Combination before Design.			
			LcomDesign.nLcomKind = iLcomKind;
			LcomDesign.DesignLcomNa.Format(_T("%4d"), LcomUlData.LoadCombId);
			LcomDesign.bMaxLcom = TRUE;
			LcomDesign.OriginalLcomKey = CurLcomK;
			LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUlData;
			LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
			LcomDesign.iServiceLcomType = LcomUlData.nServLcomType;
			if (mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK)) LcomDesign.nStrnSubType = 1;
			if (mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK)) LcomDesign.nFatigueType = 1;

			SetDgnLcomMap(LcomDesign, bIncludeLcom, anLcomSizeLcom); // Save Lcom Data for Design.

			GDisplayAnalStringSamePosFormat(_LS(IDS_DGN_COM_CREATE_LCB), anLcomSizeLcom[EN_DGNLOM_IDX_TOTL]);
		}
		else if (bAddLcom && bMaxMinLcase == TRUE)
		{
			if (bExistCrane || bConCurrent12)
			{
				int iCompSize = GetForceComponentSize(bExistCrane, bConCurrent12, m_iDgnCode, iDgnNo == D_RC_PLATE_BC ? TRUE : FALSE);

				CArray<CString, CString&> strComp;
				GetLcomCompTypeString(iDgnNo, strComp);
				// If Crane Load is exist, Consider Concurrent Member Force.        
				for (int j = 0; j < iCompSize * 2; j++)
				{
					LcomDesign.nLcomKind = iLcomKind;
					CString strSign = (j < iCompSize) ? _T("+") : _T("-");
					LcomDesign.DesignLcomNa.Format(_T("%4d%s%s"), LcomUlData.LoadCombId, strSign, strComp[j%iCompSize]);
					LcomDesign.bMaxLcom = (j < iCompSize) ? TRUE : FALSE;
					LcomDesign.OriginalLcomKey = CurLcomK;
					LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
					LcomDesign.bLongTerm = bLongTerm;
					LcomDesign.iDgnLcomType = iDgnLcomType;
					LcomDesign.iSeismicType = iSeismicType;
					LcomDesign.LcomUlData = LcomUlData;
					LcomDesign.iLcomCompType = (j%iCompSize) + 1;
					LcomDesign.iServiceLcomType = LcomUlData.nServLcomType;
					if (mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK)) LcomDesign.nStrnSubType = 1;
					if (mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK)) LcomDesign.nFatigueType = 1;

					SetDgnLcomMap(LcomDesign, bIncludeLcom, anLcomSizeLcom);
				}
			}
			else
			{
				// Primarily make the same Number Load Combination before Design.
				// Set LcomNa = LcomNo + _T("+(Max)/-(Min)")
				CString strLcomNa = _T("");
				strLcomNa.Format(_T("%3d"), LcomUlData.LoadCombId);
				LcomDesign.nLcomKind = iLcomKind;
				LcomDesign.DesignLcomNa = strLcomNa + _T("+");
				LcomDesign.bMaxLcom = TRUE;
				LcomDesign.OriginalLcomKey = CurLcomK;
				LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
				LcomDesign.bLongTerm = bLongTerm;
				LcomDesign.iDgnLcomType = iDgnLcomType;
				LcomDesign.iSeismicType = iSeismicType;
				LcomDesign.LcomUlData = LcomUlData;
				LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
				LcomDesign.iServiceLcomType = LcomUlData.nServLcomType;
				if (mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK)) LcomDesign.nStrnSubType = 1;
				if (mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK)) LcomDesign.nFatigueType = 1;

				// Save Lcom Data for Design.
				SetDgnLcomMap(LcomDesign, bIncludeLcom, anLcomSizeLcom);

				/*
				strOut.Format(_LS(IDS_DGN_COM_CREATE_LCB), iLcomTotl);
				GDisplayAnalStringSamePos(strOut);
				*/
				// if Moving Load exists, Add Load Combination at Last.
				LcomDesign.nLcomKind = iLcomKind;
				LcomDesign.DesignLcomNa = strLcomNa + _T("-");
				LcomDesign.bMaxLcom = FALSE;
				LcomDesign.OriginalLcomKey = CurLcomK;
				LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
				LcomDesign.bLongTerm = bLongTerm;
				LcomDesign.iDgnLcomType = iDgnLcomType;
				LcomDesign.iSeismicType = iSeismicType;
				LcomDesign.LcomUlData = LcomUlData;
				LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
				LcomDesign.iServiceLcomType = LcomUlData.nServLcomType;
				if (mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK)) LcomDesign.nStrnSubType = 1;
				if (mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK)) LcomDesign.nFatigueType = 1;

				// Save Lcom Data for Design.
				SetDgnLcomMap(LcomDesign, bIncludeLcom, anLcomSizeLcom);

				CString strOut = _T(""); strOut.Format(_LS(IDS_DGN_COM_CREATE_LCB), anLcomSizeLcom[EN_DGNLOM_IDX_TOTL]);
				GDisplayAnalStringSamePos(strOut);
			}
		}
		else if (bAddLcom && bNTCMinMax == TRUE)
		{
			// Primarily make the same Number Load Combination before Design.
			// Set LcomNa = LcomNo + _T("+(Max)/-(Min)")
			LcomDesign.nLcomKind = iLcomKind;
			CString strLcomNa = _T("");
			strLcomNa.Format(_T("%3d"), LcomUlData.LoadCombId);
			LcomDesign.DesignLcomNa = strLcomNa + _T("+");
			LcomDesign.bMaxLcom = TRUE;
			LcomDesign.OriginalLcomKey = CurLcomK;
			LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUlData;
			LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
			LcomDesign.iServiceLcomType = LcomUlData.nServLcomType;
			if (mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK)) LcomDesign.nStrnSubType = 1;
			if (mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK)) LcomDesign.nFatigueType = 1;

			// Save Lcom Data for Design.
			SetDgnLcomMap(LcomDesign, bIncludeLcom, anLcomSizeLcom);

			// if Moving Load exists, Add Load Combination at Last.
			LcomDesign.nLcomKind = iLcomKind;
			LcomDesign.DesignLcomNa = strLcomNa + _T("-");
			LcomDesign.bMaxLcom = FALSE;
			LcomDesign.OriginalLcomKey = CurLcomK;
			LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUlData;
			LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
			LcomDesign.iServiceLcomType = LcomUlData.nServLcomType;
			if (mStrengthLcomKey.Lookup(CurLcomK, nTempStrnLcomK)) LcomDesign.nStrnSubType = 1;
			if (mFatigueLcomKey.Lookup(CurLcomK, nTempFatiLcomK)) LcomDesign.nFatigueType = 1;

			// Save Lcom Data for Design.
			SetDgnLcomMap(LcomDesign, bIncludeLcom, anLcomSizeLcom);

			CString strOut = _T(""); strOut.Format(_LS(IDS_DGN_COM_CREATE_LCB), anLcomSizeLcom[EN_DGNLOM_IDX_TOTL]);
			GDisplayAnalStringSamePos(strOut);
		}
	}
	// Calculate Load Combination Number for Design.
	m_parLcomData = &m_arLcomTotl;
	m_iLCBAddDesignNum = anLcomSizeLcom[EN_DGNLOM_IDX_TOTL];
	m_iLcomStrn = anLcomSizeLcom[EN_DGNLOM_IDX_STRN];
	m_iLcomServ = anLcomSizeLcom[EN_DGNLOM_IDX_SERV];
	m_iLcomSpec = anLcomSizeLcom[EN_DGNLOM_IDX_SPEC];
	m_iLcomVert = anLcomSizeLcom[EN_DGNLOM_IDX_VERT];
	m_iLcomStrnUG = anLcomSizeLcom[EN_DGNLOM_IDX_STRN_UG];
	m_iLcomServUG = anLcomSizeLcom[EN_DGNLOM_IDX_SERV_UG];
	m_iLcomSpecUG = anLcomSizeLcom[EN_DGNLOM_IDX_SPEC_UG];

	if (iTotalLcom > 0)	GDisplayAnalString(_LS(IDS_DGN_COM_END_CREATE_LCOM));

	// Coded by Seungjun ('20070807) MNet:No.2921.    
	_DGN_LCOM_MAP* paLcom[5] = { &m_arLcomStrn, &m_arLcomSpec, &m_arLcomVert, &m_arLcomStrnUG, &m_arLcomSpecUG };
	for (auto paLcomSrc : paLcom)
	{
		POSITION PosLcom = paLcomSrc->GetStartPosition();
		while (PosLcom)
		{
			T_LCOM_K nIndexLcom = 0;
			_DGN_LCOM LcomDesignSrc;
			paLcomSrc->GetNextAssoc(PosLcom, nIndexLcom, LcomDesignSrc);

			CArray<_DGN_LCOM_MAP*, _DGN_LCOM_MAP*> aDgnLcomMap;
			if (!GetDgnLcomGroupMap(iLcomKind, aDgnLcomMap))
			{
				continue;
			}

			bool bInclude = false;
			const INT_PTR nSizeGrupCurr = aDgnLcomMap.GetSize();
			for (INT_PTR j = 0; j < nSizeGrupCurr; ++j)
			{
				// Target.
				_DGN_LCOM_MAP* paLcomTar = aDgnLcomMap[j];
				_DGN_LCOM LcomDesignTar;
				if (paLcomTar->Lookup(LcomDesignSrc.OriginalLcomKey, LcomDesignTar))
				{
					bInclude = true;
				}
			}
			if (!bInclude)
			{
				m_arLcomNoGrup.SetAt(LcomDesignSrc.OriginalLcomKey, LcomDesignSrc);
			}
		}
	}
}

// RC Rating
void CDgnForceCtrl::Set_LcomDataForDesignRCRating(int iLcomKind, BOOL bIncludeService, BOOL bWall/*=FALSE*/, int iDgnNo/*-1*/, BOOL bStre/*TRUE*/, BOOL bServ/*TRUE*/)
{
	CArray<T_LCOM_K, T_LCOM_K> arLcomKey;
	int iTotalLcom = 0;

	if (CDBLib::IsRcRatingCodeKRSince10(m_iDgnCode))
	{
		if (iDgnNo == D_RC_PLATE)
		{
			CArray<T_RKLC_K, T_RKLC_K> arRklcKey;
			m_pDoc->m_pAttrCtrl->GetRklcKeyList(arRklcKey);

			int nRlcsKeySize = arRklcKey.GetSize();

			arLcomKey.Copy(arRklcKey);

			iTotalLcom = arLcomKey.GetSize();
		}

		qsort(arLcomKey.GetData(), arLcomKey.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
	}
	else ASSERT(0);

	if (iTotalLcom > 0)	Write_GDisplayAnalString(_LS(IDS_DGN_COM_START_CREATE_LCOM), FALSE, TRUE);

	m_arLcomTotl.RemoveAll();
	m_arLcomStrn.RemoveAll();
	m_arLcomSpec.RemoveAll();
	m_arLcomVert.RemoveAll();
	// Save Active Load Combination Name.
	int iLcomAllDgnNum = 0;	// Active All Lcom.

	CArray<T_LCOM_K, T_LCOM_K> arActiveLcomKey;

	int i = 0;
	for (i = 0; i < iTotalLcom; i++)
	{
		// Need not NormalizeLcom() because it gets LoadCombName.
		arActiveLcomKey.Add(arLcomKey.GetAt(i));
		iLcomAllDgnNum++;
	}
	if (iLcomAllDgnNum == 0)	return;	// Apply if LCB for Design is over 0.

	m_bChkConCurForce12 = FALSE;

	int anLcomSize[EN_DGNLOM_IDX_END] = { 0, };

	for (i = 0; i < iLcomAllDgnNum; i++)
	{
		T_RKLC_D RklcData;
		RklcData.Initialize();
		m_pDoc->m_pAttrCtrl->GetRklc(i + 1, RklcData);
		RklcData.iRatingCase = 1;  // Strength rating case

		// Get LCB Data 
		T_LCOM_D LcomData; LcomData.Initialize();
		//	m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, arActiveLcomKey.GetAt(i), LcomData);
		int iLoadCaseNum = RklcData.arLoadComb.GetSize();
		if (iLoadCaseNum == 0) continue;
		LcomData.LoadCombName = RklcData.strCaseName;
		LcomData.LoadCombId = i + 1;
		LcomData.nActive = 1;
		LcomData.aCombination.SetSize(iLoadCaseNum);
		for (int nNum = 0; nNum < iLoadCaseNum; ++nNum)
		{
			LcomData.aCombination[nNum] = RklcData.arLoadComb.GetAt(nNum);
		}
		LcomData.aCombination.Add(RklcData.MoveLoadCase);
		//

		T_LCOM_D_UL	LcomUlData; LcomUlData.Initialize();
		if (!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomData, LcomUlData)) { ASSERT(0); continue; }

		// LCTB Normalize
		CArray<T_LCOM_BASE, T_LCOM_BASE&> LctbCombination;
		GetLcomDataNormailizedLctb(LcomUlData.Combination, LctbCombination);

		// Moving Load Flag.(if Exist, it is TRUE)
		BOOL bMaxMinLcase = m_pDoc->m_pAttrCtrl->IsMaxMinComb(LcomData);
		BOOL bConCurrent12 = m_pDoc->m_pAttrCtrl->IsConCurrentComb12(LcomData) && (m_iDgnCode == KSCE_ASD05 || m_iDgnCode == KSCE_RAIL_ASD04 || m_iDgnCode == KSCE_ASD10 || m_iDgnCode == KSCE_RAIL_ASD11 || m_iDgnCode == KR_BRG_LSD15 || m_iDgnCode == KDS_24_14_31_2018 ||
			m_iDgnCode == KDS_24_14_30_2019);

		int iDgnLcomType = 0, iSeismicType = 0;
		// Get Loadcase Number of this LcomUlData.
		// Set Lcom Type by Load case(D,L,W,E,...).
		SetLcomTypeIni();
		for (int j = 0; j < LctbCombination.GetSize(); j++)
		{
			SetLcomTypeGen(LctbCombination[j], iDgnLcomType, LcomUlData.LoadCombId);
			SetLcomTypeJTJ(LctbCombination[j], iDgnLcomType);
			SetLcomTypeIRC(LctbCombination[j], iDgnLcomType);
			SetLcomTypeAST(LctbCombination[j], iDgnLcomType);
			SetSeismicType(LctbCombination[j], iSeismicType);
		}
		// Add by ZINU.('02.12.03). For Lateral Displacement Control.
		BOOL bLongTerm = (iDgnLcomType % 10 == 0 ? TRUE : FALSE);
		BOOL bAddLcom = (((m_bOnlyLongTerm && bLongTerm) || !m_bOnlyLongTerm) ? TRUE : FALSE);

		_DGN_LCOM LcomDesign;
		LcomDesign.Initialize();
		if (bAddLcom && bMaxMinLcase == FALSE)
		{
			// Primarily make the same Number Load Combination before Design.
			CString strLcomNa = _T("");
			strLcomNa.Format(_T("%4d"), LcomUlData.LoadCombId);
			LcomDesign.nLcomKind = iLcomKind;
			LcomDesign.DesignLcomNa = strLcomNa;
			LcomDesign.bMaxLcom = TRUE;
			LcomDesign.OriginalLcomKey = arActiveLcomKey.GetAt(i);
			LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUlData;
			LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
			LcomDesign.aRatChkStatic.Copy(RklcData.arStaticLoadComb);
			// Save Lcom Data for Design.
			SetDgnLcomMap(LcomDesign, TRUE, anLcomSize);  // 여기에서는 무조건 special/vertical 고려하도록 되어있음.

			CString strOut = _T(""); strOut.Format(_LS(IDS_DGN_COM_CREATE_LCB), anLcomSize[EN_DGNLOM_IDX_TOTL]);
			GDisplayAnalStringSamePos(strOut);
		}
		else if (bAddLcom && bMaxMinLcase == TRUE)
		{
			// Primarily make the same Number Load Combination before Design.
			// Set LcomNa = LcomNo + _T("+(Max)/-(Min)")
			CString strLcomNa = _T("");
			strLcomNa.Format(_T("%3d"), LcomUlData.LoadCombId);
			LcomDesign.nLcomKind = iLcomKind;
			LcomDesign.DesignLcomNa = strLcomNa + _T("+");
			LcomDesign.bMaxLcom = TRUE;
			LcomDesign.OriginalLcomKey = arActiveLcomKey.GetAt(i);
			LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUlData;
			LcomDesign.aRatChkStatic.Copy(RklcData.arStaticLoadComb);
			// Save Lcom Data for Design.
			SetDgnLcomMap(LcomDesign, TRUE, anLcomSize);  // 여기에서는 무조건 special/vertical 고려하도록 되어있음.			
			/*
			strOut.Format(_LS(IDS_DGN_COM_CREATE_LCB), iLcomTotl);
			GDisplayAnalStringSamePos(strOut);
			*/
			// if Moving Load exists, Add Load Combination at Last.
			LcomDesign.nLcomKind = iLcomKind;
			LcomDesign.DesignLcomNa = strLcomNa + _T("-");
			LcomDesign.bMaxLcom = FALSE;
			LcomDesign.OriginalLcomKey = arActiveLcomKey.GetAt(i);
			LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUlData;
			// Save Lcom Data for Design.
			SetDgnLcomMap(LcomDesign, TRUE, anLcomSize);  // 여기에서는 무조건 special/vertical 고려하도록 되어있음.			

			CString strOut = _T(""); strOut.Format(_LS(IDS_DGN_COM_CREATE_LCB), anLcomSize[EN_DGNLOM_IDX_TOTL]);
			GDisplayAnalStringSamePos(strOut);
		}
	}


	// Calculate Load Combination Number for Design.
	m_parLcomData = &m_arLcomTotl;
	m_iLCBAddDesignNum = anLcomSize[EN_DGNLOM_IDX_TOTL];
	m_iLcomStrn = anLcomSize[EN_DGNLOM_IDX_STRN];
	m_iLcomServ = anLcomSize[EN_DGNLOM_IDX_SERV];
	m_iLcomSpec = anLcomSize[EN_DGNLOM_IDX_SPEC];
	m_iLcomVert = anLcomSize[EN_DGNLOM_IDX_VERT];
	if (iTotalLcom > 0)	GDisplayAnalString(_LS(IDS_DGN_COM_END_CREATE_LCOM));

	// iLcomKind = D_LCOMTYPE_STEEL, D_LCOMTYPE_CONCRETE, D_LCOMTYPE_SRC.
	if (iLcomKind == D_LCOMTYPE_CONCRETE)
	{
		if (m_bSclmCode / 100 > 0)            m_bSclmCode = TRUE;
		else                             m_bSclmCode = FALSE;
	}
	else                               m_bSclmCode = FALSE;

}

// Use function if Civil and KSCE-USD05
void CDgnForceCtrl::Set_LcomDataForDesignRC(int iLcomKind, BOOL bIncludeService, BOOL bWall/*=FALSE*/, int iDgnNo/*-1*/, BOOL bStre/*TRUE*/, BOOL bServ/*TRUE*/)
{
	// iLcomKind = D_LCOMTYPE_STEEL, D_LCOMTYPE_CONCRETE, D_LCOMTYPE_SRC.
	// Save Total Load Combination Name by Serial Number(No LcomKey).
	CArray<T_LCOM_K, T_LCOM_K> arLcomKey;
	int iTotalLcom = 0;
	if (m_iDgnCode == KSCE_RC_USD05 || m_iDgnCode == KSCE_RC_RAIL_USD04 || m_iDgnCode == KCI_USD07 || m_iDgnCode == KR_LRFD11_RC || m_iDgnCode == KDS_24_14_21_2022 ||
		m_iDgnCode == KSCE_RC_USD10 || m_iDgnCode == KSCE_RC_RAIL_USD11 || m_iDgnCode == KCI_USD07_CVL || m_iDgnCode == KCI_USD12)
	{
		// KSCE-USD05, 
		if (iDgnNo == 0)
		{
			CArray<T_LCOM_K, T_LCOM_K> arLcomTotalKey;
			m_pDoc->m_pAttrCtrl->GetLcomKeyList(iLcomKind, arLcomTotalKey);
			T_LCOM_D LcomData;
			int nSize = arLcomTotalKey.GetSize();
			if (bStre)
			{
				for (int i = 0; i < nSize; i++)
				{
					LcomData.Initialize();
					m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, arLcomTotalKey[i], LcomData);
					if (LcomData.nActive == 0)	continue;
					if (LcomData.LoadCombType != 0) continue;
					if (LcomData.nActive == 1)
						arLcomKey.Add(arLcomTotalKey[i]);
				}
			}
			if (bServ)
			{
				for (int i = 0; i < nSize; i++)
				{
					LcomData.Initialize();
					m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, arLcomTotalKey[i], LcomData);
					if (LcomData.nActive == 0)	continue;
					if (LcomData.LoadCombType != 0) continue;
					if (LcomData.nActive == 2)
						arLcomKey.Add(arLcomTotalKey[i]);
				}
			}
			iTotalLcom = arLcomKey.GetSize();
		}
		else if (iDgnNo == D_RC_BEAM)
		{
			T_RCBG_D RcbgD;
			m_pDoc->m_pAttrCtrl->GetRcbg(RcbgD);
			arLcomKey.Append(RcbgD.StreLcom);
			arLcomKey.Append(RcbgD.ServLcom);
			iTotalLcom = arLcomKey.GetSize();
		}
		else if (iDgnNo == D_RC_COLUMN) // Column
		{
			T_RCCG_D RccgD;
			m_pDoc->m_pAttrCtrl->GetRccg(RccgD);
			arLcomKey.Append(RccgD.StreLcom);
			arLcomKey.Append(RccgD.ServLcom);
			iTotalLcom = arLcomKey.GetSize();
		}
		else if (iDgnNo == D_RC_PLATE) // Plate(Rcpl)
		{
#if defined(_CIVIL)   
			if (m_iDgnCode == KSCE_RC_USD05 || m_iDgnCode == KSCE_RC_RAIL_USD04 || m_iDgnCode == KCI_USD07 || m_iDgnCode == KCI_USD07_CVL)
			{
				T_RCPG_D RcpgD;
				m_pDoc->m_pAttrCtrl->GetRcpg(RcpgD);
				arLcomKey.Append(RcpgD.StreLcom);
				arLcomKey.Append(RcpgD.ServLcom);
				iTotalLcom = arLcomKey.GetSize();
			}
			else if (m_iDgnCode == KSCE_RC_USD10 || m_iDgnCode == KSCE_RC_RAIL_USD11 || m_iDgnCode == KR_LRFD11_RC || m_iDgnCode == KCI_USD12 || m_iDgnCode == KDS_24_14_21_2022)
			{
				CArray<T_LCOM_K, T_LCOM_K> arLcomTotalKey;
				m_pDoc->m_pAttrCtrl->GetLcomKeyList(iLcomKind, arLcomTotalKey);
				T_LCOM_D LcomData;
				int nSize = arLcomTotalKey.GetSize();
				if (bStre)
				{
					for (int i = 0; i < nSize; i++)
					{
						LcomData.Initialize();
						m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, arLcomTotalKey[i], LcomData);
						if (LcomData.nActive == 0)	continue;
						if (LcomData.LoadCombType != 0) continue;
						if (LcomData.nActive == 1)
							arLcomKey.Add(arLcomTotalKey[i]);
					}
				}
				if (bServ)
				{
					for (int i = 0; i < nSize; i++)
					{
						LcomData.Initialize();
						m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, arLcomTotalKey[i], LcomData);
						if (LcomData.nActive == 0)	continue;
						if (LcomData.LoadCombType != 0) continue;
						if (LcomData.nActive == 2)
							arLcomKey.Add(arLcomTotalKey[i]);
					}
				}
				iTotalLcom = arLcomKey.GetSize();
			}
			else ASSERT(0);

#else
			T_RCPG_D RcpgD;
			m_pDoc->m_pAttrCtrl->GetRcpg(RcpgD);
			arLcomKey.Append(RcpgD.StreLcom);
			arLcomKey.Append(RcpgD.ServLcom);
			iTotalLcom = arLcomKey.GetSize();
#endif
		}
		else if (iDgnNo == D_RC_WALL) // Wall(Frame)
		{
			T_RCWG_D RcwgD;
			m_pDoc->m_pAttrCtrl->GetRcwg(RcwgD);
			arLcomKey.Append(RcwgD.StreLcom);
			arLcomKey.Append(RcwgD.ServLcom);
			iTotalLcom = arLcomKey.GetSize();
		}
		// Add by GAY. MNET:2668/2673. ('07.07.11). Sort Lcom Key List.
		qsort(arLcomKey.GetData(), arLcomKey.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
	}
	else ASSERT(0);

	if (iTotalLcom > 0)	Write_GDisplayAnalString(_LS(IDS_DGN_COM_START_CREATE_LCOM), FALSE, TRUE);

	m_arLcomTotl.RemoveAll();
	m_arLcomStrn.RemoveAll();
	m_arLcomSpec.RemoveAll();
	m_arLcomVert.RemoveAll();
	// Save Active Load Combination Name.
	int iLcomAllDgnNum = 0;	// Active All Lcom.
	T_LCOM_D LcomData;
	CArray<T_LCOM_K, T_LCOM_K> arActiveLcomKey;

	for (int i = 0; i < iTotalLcom; ++i)
	{
		LcomData.Initialize();
		m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, arLcomKey.GetAt(i), LcomData);
		// Check if Serviceability is used.		
		bool bLcomOK = false;
		if (bIncludeService)	bLcomOK = (LcomData.nActive > 0) ? true : false;
		else				bLcomOK = IsLcomStrn(LcomData.nActive);
		// Modify by GAY. MNET:2903. ('07.08.27). No Include if SRSS, Envelope. (See Set_LcomDataForDesign(...))
		if (bLcomOK && !(LcomData.LoadCombType == 1 || LcomData.LoadCombType == 3))
		{
			// Need not NormalizeLcom() because it gets LoadCombName.
			arActiveLcomKey.Add(arLcomKey.GetAt(i));
			iLcomAllDgnNum++;
		}
		CString strOut = _T("");
		strOut.Format(_LS(IDS_DGN_COM_READ_LCB), i + 1, iTotalLcom);
		GDisplayAnalStringSamePos(strOut);
	}
	GDisplayAnalString(_T(""));

	if (iLcomAllDgnNum == 0)	return;	// Apply if LCB for Design is over 0.
	// Add by ZINU.('05.06.15). Consider Existing LL, EL.
	m_arExistLL.RemoveAll();
	m_arExistEL.RemoveAll();
	m_mapSeisLoodCase.RemoveAll();
	// Add by ZINU.('05.05.11). Special Load Combination Factor.
	m_arSlcfStldK.RemoveAll();
	m_arSlcfSplcK.RemoveAll();
	m_arSlcfStldLLK.RemoveAll();
	m_arSlcmSds.RemoveAll();
	m_arSlcmSpecial.RemoveAll();
	m_arSlcmVertical.RemoveAll();
	m_mapSlcmType.RemoveAll();
	m_mUlct.RemoveAll();

	SetSlcmTypeMap(m_mapSlcmType);
	SetSpecVertLcomInfoMap(bWall);
	SetUlctMap(m_mUlct);

	m_bChkConCurForce12 = FALSE;

	int anLcomSize[EN_DGNLOM_IDX_END] = { 0, };

	T_LCOM_D_UL	LcomUlData;
	for (int i = 0; i < iLcomAllDgnNum; i++)
	{
		LcomData.Initialize();
		// Get LCB Data by LcomName.
		m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, arActiveLcomKey.GetAt(i), LcomData);
		LcomUlData.Initialize();
		if (!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomData, LcomUlData)) { ASSERT(0); continue; }

		// LCTB Normalize
		CArray<T_LCOM_BASE, T_LCOM_BASE&> LctbCombination;
		GetLcomDataNormailizedLctb(LcomUlData.Combination, LctbCombination);

		// Moving Load Flag.(if Exist, it is TRUE)
		BOOL bMaxMinLcase = m_pDoc->m_pAttrCtrl->IsMaxMinComb(LcomData);
		BOOL bConCurrent12 = m_pDoc->m_pAttrCtrl->IsConCurrentComb12(LcomData) && (m_iDgnCode == KSCE_ASD05 || m_iDgnCode == KSCE_RAIL_ASD04 || m_iDgnCode == KSCE_ASD10 || m_iDgnCode == KSCE_RAIL_ASD11 || m_iDgnCode == KR_BRG_LSD15 ||
			m_iDgnCode == KDS_24_14_30_2019);

		int iDgnLcomType = 0, iSeismicType = 0;
		// Get Loadcase Number of this LcomUlData.
		// Set Lcom Type by Load case(D,L,W,E,...).
		SetLcomTypeIni();
		for (int j = 0; j < LctbCombination.GetSize(); j++)
		{
			SetLcomTypeGen(LctbCombination[j], iDgnLcomType, LcomUlData.LoadCombId);
			SetLcomTypeJTJ(LctbCombination[j], iDgnLcomType);
			SetLcomTypeIRC(LctbCombination[j], iDgnLcomType);
			SetLcomTypeAST(LctbCombination[j], iDgnLcomType);
			SetSeismicType(LctbCombination[j], iSeismicType);
		}
		// Add by ZINU.('02.12.03). For Lateral Displacement Control.
		BOOL bLongTerm = (iDgnLcomType % 10 == 0 ? TRUE : FALSE);
		BOOL bAddLcom = (((m_bOnlyLongTerm && bLongTerm) || !m_bOnlyLongTerm) ? TRUE : FALSE);

		_DGN_LCOM LcomDesign;
		LcomDesign.Initialize();
		if (bAddLcom && bMaxMinLcase == FALSE)
		{
			// Primarily make the same Number Load Combination before Design.
			CString strLcomNa = _T("");
			strLcomNa.Format(_T("%4d"), LcomUlData.LoadCombId);
			LcomDesign.nLcomKind = iLcomKind;
			LcomDesign.DesignLcomNa = strLcomNa;
			LcomDesign.bMaxLcom = TRUE;
			LcomDesign.OriginalLcomKey = arActiveLcomKey.GetAt(i);
			LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUlData;
			LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
			// Save Lcom Data for Design.
			SetDgnLcomMap(LcomDesign, TRUE, anLcomSize);  // 여기에서는 무조건 special/vertical 고려하도록 되어있음.			

			CString strOut = _T(""); strOut.Format(_LS(IDS_DGN_COM_CREATE_LCB), anLcomSize[EN_DGNLOM_IDX_TOTL]);
			GDisplayAnalStringSamePos(strOut);
		}
		else if (bAddLcom && bMaxMinLcase == TRUE)
		{
			// Primarily make the same Number Load Combination before Design.
			// Set LcomNa = LcomNo + _T("+(Max)/-(Min)")
			CString strLcomNa = _T("");
			strLcomNa.Format(_T("%3d"), LcomUlData.LoadCombId);
			LcomDesign.nLcomKind = iLcomKind;
			LcomDesign.DesignLcomNa = strLcomNa + _T("+");
			LcomDesign.bMaxLcom = TRUE;
			LcomDesign.OriginalLcomKey = arActiveLcomKey.GetAt(i);
			LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUlData;
			// Save Lcom Data for Design.
			SetDgnLcomMap(LcomDesign, TRUE, anLcomSize);  // 여기에서는 무조건 special/vertical 고려하도록 되어있음.			

			/*
			strOut.Format(_LS(IDS_DGN_COM_CREATE_LCB), iLcomTotl);
			GDisplayAnalStringSamePos(strOut);
			*/
			// if Moving Load exists, Add Load Combination at Last.
			LcomDesign.nLcomKind = iLcomKind;
			LcomDesign.DesignLcomNa = strLcomNa + _T("-");
			LcomDesign.bMaxLcom = FALSE;
			LcomDesign.OriginalLcomKey = arActiveLcomKey.GetAt(i);
			LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUlData;
			// Save Lcom Data for Design.
			SetDgnLcomMap(LcomDesign, TRUE, anLcomSize);  // 여기에서는 무조건 special/vertical 고려하도록 되어있음.			

			CString strOut = _T(""); strOut.Format(_LS(IDS_DGN_COM_CREATE_LCB), anLcomSize[EN_DGNLOM_IDX_TOTL]);
			GDisplayAnalStringSamePos(strOut);
		}
	}
	// Calculate Load Combination Number for Design.
	m_parLcomData = &m_arLcomTotl;
	m_iLCBAddDesignNum = anLcomSize[EN_DGNLOM_IDX_TOTL];
	m_iLcomStrn = anLcomSize[EN_DGNLOM_IDX_STRN];
	m_iLcomServ = anLcomSize[EN_DGNLOM_IDX_SERV];
	m_iLcomSpec = anLcomSize[EN_DGNLOM_IDX_SPEC];
	m_iLcomVert = anLcomSize[EN_DGNLOM_IDX_VERT];
	if (iTotalLcom > 0)	GDisplayAnalString(_LS(IDS_DGN_COM_END_CREATE_LCOM));

	// iLcomKind = D_LCOMTYPE_STEEL, D_LCOMTYPE_CONCRETE, D_LCOMTYPE_SRC.
	if (iLcomKind == D_LCOMTYPE_CONCRETE)
	{
		if (m_bSclmCode / 100 > 0)            m_bSclmCode = TRUE;
		else                             m_bSclmCode = FALSE;
	}
	else if (iLcomKind == D_LCOMTYPE_STEEL)
	{
		if ((m_bSclmCode / 10) % 10 > 0)        m_bSclmCode = TRUE;
		else                             m_bSclmCode = FALSE;
	}
	else if (iLcomKind == D_LCOMTYPE_SRC)
	{
		if (m_bSclmCode % 10 > 0)             m_bSclmCode = TRUE;
		else                             m_bSclmCode = FALSE;
	}
	else if (iLcomKind == D_LCOMTYPE_ALUMINUM)
	{
		if (m_bSclmCode % 10 > 0)             m_bSclmCode = TRUE;
		else                             m_bSclmCode = FALSE;
	}
	else if (iLcomKind == D_LCOMTYPE_STLCOMP)
	{
		if (m_bSclmCode % 10 > 0)             m_bSclmCode = TRUE;
		else                             m_bSclmCode = FALSE;
	}
	else                               m_bSclmCode = FALSE;
}

void CDgnForceCtrl::Set_LcomDataForDesignTower()
{
#if defined(_TOWER)
	// Save Total Load Combination Name by Serial Number(No LcomKey).
	CArray<T_LCOM_K, T_LCOM_K> arLcomKey;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STEEL, arLcomKey);
	int iTotalLcom = m_pDoc->m_pAttrCtrl->GetCountLcom(D_LCOMTYPE_STEEL);
	if (iTotalLcom > 0)	Write_GDisplayAnalString(_LS(IDS_DGN_COM_START_CREATE_LCOM), FALSE, TRUE);

	// Save Active Load Combination Name.
	int iLcomAllDgnNum = 0;	// Active All Lcom.
	T_LCOM_D LcomData;
	CArray<T_LCOM_K, T_LCOM_K> arActiveLcomKey;
	m_iLCBDesignNum = 0;
	for (int i = 0; i < iTotalLcom; i++)
	{
		LcomData.Initialize();
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, arLcomKey.GetAt(i), LcomData);
		if (LcomData.nActive > 0)
		{
			// Need not NormalizeLcom() because it gets LoadCombName.
			arActiveLcomKey.Add(arLcomKey.GetAt(i));
			iLcomAllDgnNum++;
		}
		CString strTemp = _T("");
		strTemp.Format(_LS(IDS_DGN_COM_READ_LCB), i + 1, iTotalLcom);
		GDisplayAnalStringSamePos(strTemp);
	}
	GDisplayAnalString(_T(""));
	if (iLcomAllDgnNum == 0)	return;	// Apply if LCB for Design is over 0.

	int iLcomTotl = 0;
	T_LCOM_D_UL	LcomUlData;
	for (i = 0; i < iLcomAllDgnNum; i++)
	{
		LcomData.Initialize();
		// Get LCB Data and Key by LcomName.
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, arActiveLcomKey.GetAt(i), LcomData);
		// Change by ZINU.('02.2.15). T_LCOM_D -> T_LCOM_D_UL
		LcomUlData.Initialize();
		if (!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomData, LcomUlData)) { ASSERT(0); continue; }

		_DGN_LCOM LcomDesign;
		LcomDesign.Initialize();
		iLcomTotl++;
		// Primarily make the same Number Load Combination before Design.
		CString strLcomNa = _T("");
		strLcomNa.Format(_T("%4d"), LcomUlData.LoadCombId);
		LcomDesign.nLcomKind = D_LCOMTYPE_STEEL;
		LcomDesign.DesignLcomNa = strLcomNa;
		LcomDesign.DesignLcomKey = iLcomTotl;
		LcomDesign.bMaxLcom = TRUE;	// If tower, Always TRUE.
		LcomDesign.OriginalLcomKey = arActiveLcomKey.GetAt(i);
		LcomDesign.OriginalLcomNo = LcomUlData.LoadCombId;
		LcomDesign.LcomUlData = LcomUlData;
		// Save Lcom Data for Design.
		m_parLcomData->SetAt(iLcomTotl, LcomDesign);
		CString strTemp = _T("");
		strTemp.Format(_LS(IDS_DGN_COM_CREATE_LCB), iLcomTotl);
		GDisplayAnalStringSamePos(strTemp);
	}
	// Calculate Load Combination Number for Design.
	m_iLCBDesignNum = iLcomTotl;
	m_iLCBAddDesignNum = m_parLcomData->GetCount();
	if (iTotalLcom > 0)	GDisplayAnalString(_LS(IDS_DGN_COM_END_CREATE_LCOM));
#endif
}

void CDgnForceCtrl::Set_LcomDataForDesignSeisEval(int nLcomKind, UINT unType, BOOL bWall)
{
	T_LCOM_K_LIST aLcomK;
	aLcomK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(nLcomKind, aLcomK);
	int nTotlLcom = m_pDoc->m_pAttrCtrl->GetCountLcom(nLcomKind);

	// Modify by GAY. PMS:3787. ('09.12.07). 정적/동적 지진하중 입력 여부 확인.  
	m_bExistSplc = m_pDoc->m_pAttrCtrl->GetCountSplc() > 0 ? TRUE : FALSE;
	m_bExistSeis = m_pDoc->m_pAttrCtrl->GetCountSeis() > 0 ? TRUE : FALSE;

	m_arLcomTotl.RemoveAll();
	m_arLcomStrn.RemoveAll();
	m_arLcomSpec.RemoveAll();
	m_arLcomVert.RemoveAll();

	int nLcomActive = 0;
	T_LCOM_K_LIST aActiveLcomK;
	T_LCOM_D LcomD;

	for (int i = 0; i < nTotlLcom; ++i)
	{
		T_LCOM_K LcomK = aLcomK[i];
		LcomD.Initialize();
		m_pDoc->m_pAttrCtrl->GetLcom(nLcomKind, LcomK, LcomD);

		if (nLcomKind == D_LCOMTYPE_LINEAR_EVAL)
		{
			const bool bLinLcomK = [](UINT unEvalType, int nActive)
			{
				switch (nActive)
				{
				case EN_LNEVAL_GRAV:
				{
					return unEvalType == EN_EVAL_GRAV ? true : false;
				}
				case EN_LNEVAL_SEIS:
				case EN_LNEVAL_SPEC:
				case EN_LNEVAL_VERT:
				{
					return unEvalType == EN_EVAL_SEIS ? true : false;
				}
				default:
				{
					ASSERT(0);
					return false;
				}
				}
			}(unType, LcomD.nActive);

			if (bLinLcomK && !(LcomD.LoadCombType == 1 || LcomD.LoadCombType == 3))
			{
				// Need not NormalizeLcom() because it gets LoadCombName.			
				aActiveLcomK.Add(LcomK);
				nLcomActive++;
			}
		}
		else if (nLcomKind == D_LCOMTYPE_SEISMIC)
		{
			const bool b1stLcomOK = [](int nActive)
			{
				switch (nActive)
				{
				case EN_EVAL_GENE:
				case EN_EVAL_VERT:
					return true;
				default:
					return false;
				}
			}(LcomD.nActive);

			if (b1stLcomOK && !(LcomD.LoadCombType == 1 || LcomD.LoadCombType == 3))
			{
				// Need not NormalizeLcom() because it gets LoadCombName.			
				aActiveLcomK.Add(LcomK);
				nLcomActive++;
			}
		}
		else continue;
	}

	if (nLcomActive == 0)	return;	// Apply if LCB for Design is over 0.

	// Add by ZINU.('05.06.15). Consider Existing LL, EL.
	m_arExistLL.RemoveAll();
	m_arExistEL.RemoveAll();
	m_mapSeisLoodCase.RemoveAll();
	// Add by ZINU.('05.05.11). Special Load Combination Factor.
	m_arSlcfStldK.RemoveAll();
	m_arSlcfSplcK.RemoveAll();
	m_arSlcfStldLLK.RemoveAll();
	m_arSlcmSds.RemoveAll();
	m_arSlcmSpecial.RemoveAll();
	m_arSlcmVertical.RemoveAll();
	m_mapSlcmType.RemoveAll();
	m_mapSlcmTypeStrip.RemoveAll();
	m_mUlct.RemoveAll();

	SetSlcmTypeMap(m_mapSlcmType);
	SetSpecVertLcomInfoMap(bWall);
	SetUlctMap(m_mUlct);

	// Modify, Jaeoh. [2/4/2009]
	BOOL bCheckCraneLcom = FALSE;
#if defined(_CIVIL)
	bCheckCraneLcom = FALSE;
#else
	bCheckCraneLcom = TRUE;
#endif

	m_bExistCraneLcom = FALSE;
	m_bChkConCurForce12 = FALSE;
	m_bNTC08Wall = FALSE;

	int nLcomTotl = 0;
	int nLcomStrn = 0, nLcomServ = 0, nLcomSpec = 0, nLcomVert = 0;

	T_LCOM_D_UL	LcomUL;
	for (int i = 0; i < nLcomActive; ++i)
	{
		T_LCOM_K ActiveLcomK = aActiveLcomK[i];

		LcomD.Initialize();
		// Get LCB Data by LcomName.
		m_pDoc->m_pAttrCtrl->GetLcom(nLcomKind, ActiveLcomK, LcomD);
		LcomUL.Initialize();
		if (!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomD, LcomUL)) { ASSERT(FALSE); continue; }

		// LCTB Normalize
		CArray<T_LCOM_BASE, T_LCOM_BASE&> aLctbComb;
		GetLcomDataNormailizedLctb(LcomUL.Combination, aLctbComb);

		// Moving Load Flag.(if Exist, it is TRUE)
		BOOL bMaxMinLcase = m_pDoc->m_pAttrCtrl->IsMaxMinComb(LcomD);
		BOOL bExistCrane = bCheckCraneLcom ? m_pDoc->m_pAttrCtrl->IsCraneComb(LcomD) : FALSE;
		BOOL bConCurrent12 = m_pDoc->m_pAttrCtrl->IsConCurrentComb12(LcomD);
		BOOL bStlCodeCur12 = FALSE;
		bConCurrent12 = bConCurrent12 && bStlCodeCur12;

		int iDgnLcomType = 0, iSeismicType = 0;
		// Get Loadcase Number of this LcomUlData.
		// Set Lcom Type by Load case(D,L,W,E,...).
		SetLcomTypeIni();
		int nLctb = aLctbComb.GetSize();
		for (int j = 0; j < aLctbComb.GetSize(); ++j)
		{
			SetLcomTypeGen(aLctbComb[j], iDgnLcomType, LcomUL.LoadCombId);
			SetLcomTypeJTJ(aLctbComb[j], iDgnLcomType);
			SetLcomTypeIRC(aLctbComb[j], iDgnLcomType);
			SetLcomTypeAST(aLctbComb[j], iDgnLcomType);
			SetSeismicType(aLctbComb[j], iSeismicType);
		}

		// Add by ZINU.('02.12.03). For Lateral Displacement Control.
		BOOL bLongTerm = (iDgnLcomType % 10 == 0 ? TRUE : FALSE);
		BOOL bAddLcom = (((m_bOnlyLongTerm && bLongTerm) || !m_bOnlyLongTerm) ? TRUE : FALSE);

		_DGN_LCOM LcomDesign;
		LcomDesign.Initialize();
		if (bAddLcom && bMaxMinLcase == FALSE)
		{
			// Primarily make the same Number Load Combination before Design.
			CString strLcomNa = _T("");
			strLcomNa.Format(_T("%4d"), LcomUL.LoadCombId);
			LcomDesign.nLcomKind = nLcomKind;
			LcomDesign.DesignLcomNa = strLcomNa;
			LcomDesign.bMaxLcom = TRUE;
			LcomDesign.OriginalLcomKey = ActiveLcomK;
			LcomDesign.OriginalLcomNo = LcomUL.LoadCombId;
			LcomDesign.bLongTerm = bLongTerm;
			LcomDesign.iDgnLcomType = iDgnLcomType;
			LcomDesign.iSeismicType = iSeismicType;
			LcomDesign.LcomUlData = LcomUL;
			LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
			LcomDesign.iServiceLcomType = LcomUL.nServLcomType;
			LcomDesign.nActive = LcomUL.nActive;

			// Save Lcom Data for Design.
			switch (nLcomKind)
			{
			case D_LCOMTYPE_SEISMIC:     SetEval1stLcomMap(LcomDesign, nLcomTotl, nLcomStrn, nLcomVert);            break;
			case D_LCOMTYPE_LINEAR_EVAL: SetEvalLinLcomMap(LcomDesign, unType, nLcomTotl, nLcomStrn, nLcomSpec, nLcomVert); break;
			default: ASSERT(0); break;
			}
		}
		else if (bAddLcom && bMaxMinLcase == TRUE)
		{
			if (bExistCrane || bConCurrent12)
			{
				int iCompSize = GetForceComponentSize(bExistCrane, bConCurrent12, m_iDgnCode, FALSE);  /// 내진성능평가라서... Plate는 안함..

				CArray<CString, CString&> strComp;
				GetLcomCompTypeString(D_RC_GEN, strComp);
				// If Crane Load is exist, Consider Concurrent Member Force.        
				for (int j = 0; j < iCompSize * 2; j++)
				{
					CString strSign = (j < iCompSize) ? _T("+") : _T("-");
					LcomDesign.nLcomKind = nLcomKind;
					LcomDesign.DesignLcomNa.Format(_T("%4d%s%s"), LcomUL.LoadCombId, strSign, strComp[j%iCompSize]);
					LcomDesign.bMaxLcom = (j < iCompSize) ? TRUE : FALSE;
					LcomDesign.OriginalLcomKey = ActiveLcomK;
					LcomDesign.OriginalLcomNo = LcomUL.LoadCombId;
					LcomDesign.bLongTerm = bLongTerm;
					LcomDesign.iDgnLcomType = iDgnLcomType;
					LcomDesign.iSeismicType = iSeismicType;
					LcomDesign.LcomUlData = LcomUL;
					LcomDesign.iLcomCompType = (j%iCompSize) + 1;
					LcomDesign.iServiceLcomType = LcomUL.nServLcomType;
					LcomDesign.nActive = LcomUL.nActive;

					switch (nLcomKind)
					{
					case D_LCOMTYPE_SEISMIC:     SetEval1stLcomMap(LcomDesign, nLcomTotl, nLcomStrn, nLcomVert); break;
					case D_LCOMTYPE_LINEAR_EVAL: SetEvalLinLcomMap(LcomDesign, unType, nLcomTotl, nLcomStrn, nLcomSpec, nLcomVert); break;
					default: ASSERT(0); break;
					}
				}
			}
			else
			{
				// Primarily make the same Number Load Combination before Design.
				// Set LcomNa = LcomNo + _T("+(Max)/-(Min)")
				CString strLcomNa = _T("");
				strLcomNa.Format(_T("%3d"), LcomUL.LoadCombId);
				LcomDesign.nLcomKind = nLcomKind;
				LcomDesign.DesignLcomNa = strLcomNa + _T("+");
				LcomDesign.bMaxLcom = TRUE;
				LcomDesign.OriginalLcomKey = ActiveLcomK;
				LcomDesign.OriginalLcomNo = LcomUL.LoadCombId;
				LcomDesign.bLongTerm = bLongTerm;
				LcomDesign.iDgnLcomType = iDgnLcomType;
				LcomDesign.iSeismicType = iSeismicType;
				LcomDesign.LcomUlData = LcomUL;
				LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
				LcomDesign.iServiceLcomType = LcomUL.nServLcomType;
				LcomDesign.nActive = LcomUL.nActive;

				// Save Lcom Data for Design.
				switch (nLcomKind)
				{
				case D_LCOMTYPE_SEISMIC:     SetEval1stLcomMap(LcomDesign, nLcomTotl, nLcomStrn, nLcomVert); break;
				case D_LCOMTYPE_LINEAR_EVAL: SetEvalLinLcomMap(LcomDesign, unType, nLcomTotl, nLcomStrn, nLcomSpec, nLcomVert); break;
				default: ASSERT(0); break;
				}

				// if Moving Load exists, Add Load Combination at Last.
				LcomDesign.nLcomKind = nLcomKind;
				LcomDesign.DesignLcomNa = strLcomNa + _T("-");
				LcomDesign.bMaxLcom = FALSE;
				LcomDesign.OriginalLcomKey = ActiveLcomK;
				LcomDesign.OriginalLcomNo = LcomUL.LoadCombId;
				LcomDesign.bLongTerm = bLongTerm;
				LcomDesign.iDgnLcomType = iDgnLcomType;
				LcomDesign.iSeismicType = iSeismicType;
				LcomDesign.LcomUlData = LcomUL;
				LcomDesign.iLcomCompType = 0;  // Not Consider Concurrent Force.
				LcomDesign.iServiceLcomType = LcomUL.nServLcomType;
				LcomDesign.nActive = LcomUL.nActive;

				// Save Lcom Data for Design.
				switch (nLcomKind)
				{
				case D_LCOMTYPE_SEISMIC:     SetEval1stLcomMap(LcomDesign, nLcomTotl, nLcomStrn, nLcomVert); break;
				case D_LCOMTYPE_LINEAR_EVAL: SetEvalLinLcomMap(LcomDesign, unType, nLcomTotl, nLcomStrn, nLcomSpec, nLcomVert); break;
				default: ASSERT(0); break;
				}
			}
		}
	}

	// Calculate Load Combination Number for Design.
	m_parLcomData = &m_arLcomTotl;
	m_iLCBAddDesignNum = nLcomTotl;
	m_iLcomStrn = nLcomStrn;
	m_iLcomServ = nLcomServ;
	m_iLcomSpec = nLcomSpec;
	m_iLcomVert = nLcomVert;
	m_iLcomStrnUG = nLcomStrn;
	m_iLcomServUG = nLcomServ;
	m_iLcomSpecUG = nLcomSpec;

	// Coded by Seungjun ('20070807) MNet:No.2921.
	_DGN_LCOM_MAP* paLcom[3] = { &m_arLcomStrn, &m_arLcomSpec, &m_arLcomVert };
	for (auto paLcomSrc : paLcom)
	{
		POSITION PosLcom = paLcomSrc->GetStartPosition();
		while (PosLcom)
		{
			T_LCOM_K nIndexLcom = 0;
			_DGN_LCOM LcomDesignSrc;
			paLcomSrc->GetNextAssoc(PosLcom, nIndexLcom, LcomDesignSrc);

			CArray<_DGN_LCOM_MAP*, _DGN_LCOM_MAP*> aDgnLcomMap;
			if (!GetDgnLcomGroupMap(nLcomKind, aDgnLcomMap))
			{
				continue;
			}

			bool bInclude = false;
			const INT_PTR nSizeGrupCurr = aDgnLcomMap.GetSize();
			for (INT_PTR j = 0; j < nSizeGrupCurr; ++j)
			{
				_DGN_LCOM_MAP* paLcomTar = aDgnLcomMap[j];
				_DGN_LCOM LcomDesignTar;
				if (paLcomTar->Lookup(LcomDesignSrc.OriginalLcomKey, LcomDesignTar))
				{
					bInclude = true;
				}
			}

			if (!bInclude)
			{
				m_arLcomNoGrup.SetAt(LcomDesignSrc.OriginalLcomKey, LcomDesignSrc);
			}
		}
	}
}

BOOL CDgnForceCtrl::IsColdFormDesign(T_ELEM_K ElemK, BOOL bSteel)
{
	if (!bSteel) return FALSE;
	if (ElemK == 0) { ASSERT(0); return FALSE; }

	BOOL bColdFormCode = FALSE;
	switch (m_iDgnCode)
	{
	case GB50018_25: bColdFormCode = TRUE; break;
	case GB50018_02: bColdFormCode = TRUE; break;
	case Eurocode3_1_3_06: bColdFormCode = TRUE; break;
	case AISI_CFSD86: bColdFormCode = TRUE; break;
	case AIK_CFSD98: bColdFormCode = TRUE; break;
	case AISI_CFSD08: bColdFormCode = TRUE; break;
	}
	if (!bColdFormCode) return FALSE;

	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }

	T_SECT_D SectD;
	if (!m_pDgnDataCtrl->Get_DgnStlSect(ElemK, SectD)) { ASSERT(0); return FALSE; }

	CString csSectShape = SectD.SectBefore.Shape;
	//
	if (m_iDgnCode == GB50018_02 || m_iDgnCode == GB50018_25)
	{
		if (csSectShape == D_SECT_SHAPE_REG_L) return TRUE;
		if (csSectShape == D_SECT_SHAPE_REG_C) return TRUE;
		if (csSectShape == D_SECT_SHAPE_REG_T) return TRUE;
		if (csSectShape == D_SECT_SHAPE_REG_L) return TRUE;
		if (csSectShape == D_SECT_SHAPE_REG_P) return TRUE;
		if (csSectShape == D_SECT_SHAPE_REG_2L) return TRUE;
		if (csSectShape == D_SECT_SHAPE_REG_2C) return TRUE;
	}
	if (csSectShape == D_SECT_SHAPE_REG_CC) return TRUE;
	if (csSectShape == D_SECT_SHAPE_REG_UP) return TRUE;
	if (csSectShape == D_SECT_SHAPE_REG_Z) return TRUE;

	return FALSE;
}

bool CDgnForceCtrl::IsLcomStrn(int nActive)
{
	switch (nActive)
	{
	case EN_DGN_STRN:
	case EN_DGN_SPEC:
	case EN_DGN_VERT:
	case EN_DGN_STRE:
	case EN_DGN_UG_STRN:
	case EN_DGN_UG_SPEC:
	{
		return true;
	}
	case EN_DGN_NONE:
	case EN_DGN_SERV:
	case EN_DGN_UG_SERV:
	{
		return false;
	}
	default:
	{
		ASSERT(0);
		return false;
	}
	}
}

bool CDgnForceCtrl::IsLcomServ(int nActive)
{
	switch (nActive)
	{
	case EN_DGN_SERV:
	case EN_DGN_UG_SERV:
	{
		return true;
	}
	case EN_DGN_NONE:
	case EN_DGN_STRN:
	case EN_DGN_SPEC:
	case EN_DGN_VERT:
	case EN_DGN_STRE:
	case EN_DGN_UG_STRN:
	case EN_DGN_UG_SPEC:
	{
		return false;
	}
	default:
	{
		ASSERT(0);
		return false;
	}
	}
}

bool CDgnForceCtrl::IsDgnLcomStrn(int nLcomType, int nDgnLcomNo)
{
	if (m_parLcomData == nullptr) { return false; }

	_DGN_LCOM LcomDesign;
	if (!m_parLcomData->Lookup(nDgnLcomNo, LcomDesign)) { ASSERT(0); return false; }

	T_LCOM_D LcomD;
	if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, LcomDesign.OriginalLcomKey, LcomD)) { ASSERT(0); return false; }

	return IsLcomStrn(LcomD.nActive);
}

bool CDgnForceCtrl::IsDgnLcomServ(int nLcomType, int nDgnLcomNo)
{
	if (m_parLcomData == nullptr) { return false; }

	_DGN_LCOM LcomDesign;
	if (!m_parLcomData->Lookup(nDgnLcomNo, LcomDesign)) { ASSERT(0); return false; }

	T_LCOM_D LcomD;
	if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, LcomDesign.OriginalLcomKey, LcomD)) { ASSERT(0); return false; }

	return IsLcomServ(LcomD.nActive);
}

int CDgnForceCtrl::GetSlcmTypeSlab(T_ELEM_K ElemK)
{
	int nSlcmType = 0;
	if (!m_mapSlcmType.Lookup(ElemK, nSlcmType))
	{
		nSlcmType = -1;
	}
	return nSlcmType;
}

void CDgnForceCtrl::Set_LcomDataForElem(int iElem)
{
	m_arLcomGrup.RemoveAll();
	// Add by ZINU.('05.06.15). Separate Special, Vertical from Strength/Stress.
	// iElem = Truss/Beam(ElemK), Wall(nInternalWallId).

	if (iElem == 0)
	{
		m_parLcomData = &m_arLcomTotl;
		// Calculate Load Combination Number for Design.
		m_iLCBDesignNum = m_parLcomData->GetCount();
		m_iLCBServiceNum = m_iLcomServ;
		m_iLCBAddDesignNum = m_iLCBDesignNum;
	}
	else
	{
		// Coded by Seungjun ('20070806) MNet:No.2921. Set Material Type. Transfered from Set_ForceMomentBySeperate().
		const bool bWall = IsWall();
		if (m_strMatType == _T(""))
		{
			T_MATD_D MatdD;
			MatdD.Initialize();
			if (bWall)
			{
				T_WALL_KEY WallKey;
				m_pDoc->m_pPostCtrl->GetWallKey(iElem, WallKey);
				int nMatl = 0;
				GetDgnWallMatd(WallKey, nMatl, MatdD);
			}
			else
			{
				GetDgnElemMatd(iElem, MatdD);
			}
			m_strMatType = MatdD.Type;	// S, C, SRC, USER
		}

		const EN_DGNLCOM_APP enLcomApp = GetAppDgnLcomTypeByMemb(iElem);

		double dFactor = 0.0;
		BOOL bStl = (m_strMatType == _T("S") ? TRUE : FALSE);
		BOOL bCon = (m_strMatType == _T("C") ? TRUE : FALSE);
		BOOL bSrc = (m_strMatType == _T("SRC") ? TRUE : FALSE);
		BOOL bAlu = (m_strMatType == _T("A") ? TRUE : FALSE);
		BOOL bCfd = IsColdFormDesign(iElem, bStl);

		// Add by ZINU.('07.08.02). NO:2921, Elem+Lcom Group(T_DLCM_D) Design for PDMS.
		int iStlGrup = m_arLcomStlGrup.GetSize();
		int iCfdGrup = m_arLcomCfsGrup.GetSize();
		int iConGrup = m_arLcomConGrup.GetSize();
		int iSrcGrup = m_arLcomSrcGrup.GetSize();
		int iAluGrup = m_arLcomAluGrup.GetSize();

		int iLcomGrupCurr = 0;
		if (bStl)	iLcomGrupCurr = iStlGrup;
		else if (bCfd)	iLcomGrupCurr = iCfdGrup;
		else if (bCon)	iLcomGrupCurr = iConGrup;
		else if (bSrc)	iLcomGrupCurr = iSrcGrup;
		else if (bAlu)	iLcomGrupCurr = iAluGrup;
		if (iLcomGrupCurr)	// If Exist Lcom Group.
		{
			CIntMap amInt;
			if (bStl) m_arElemStlGrup.Lookup(iElem, amInt);
			else if (bCfd)	m_arElemCfsGrup.Lookup(iElem, amInt);
			else if (bCon)
			{
				if (!bWall)
					m_arElemConGrup.Lookup(iElem, amInt);
				else
					m_arElemConWallGrup.Lookup(iElem, amInt);
			}
			else if (bSrc)	m_arElemSrcGrup.Lookup(iElem, amInt);
			else if (bAlu)	m_arElemAluGrup.Lookup(iElem, amInt);

			int iamInt = amInt.GetCount();
			// If iamInt > 0, Elem including Group / Else, Elem excluding Group.
			BOOL bIncludeElemInGrup = (iamInt > 0 ? TRUE : FALSE);

			T_LCOM_K iIndexLcom = 0;

			// Source.
			_DGN_LCOM_MAP* parLcomSrc = GetAppDgnLcomMap(enLcomApp);

			int iLcomGrup = 0;
			int iLcomServGrup = 0;
			for (int i = 0; i < iLcomGrupCurr; ++i)
			{
				int iGetNo = 0;
				if (bIncludeElemInGrup)
				{
					if (!amInt.Lookup(i, iGetNo))	continue;
				}
				else
				{
					iGetNo = i;
				}

				// Target.
				_DGN_LCOM_MAP* parLcomTar = nullptr;
				if (bStl) parLcomTar = m_arLcomStlGrup[iGetNo];
				else if (bCfd) parLcomTar = m_arLcomCfsGrup[iGetNo];
				else if (bCon) parLcomTar = m_arLcomConGrup[iGetNo];
				else if (bSrc) parLcomTar = m_arLcomSrcGrup[iGetNo];
				else if (bAlu) parLcomTar = m_arLcomAluGrup[iGetNo];
				else	ASSERT(0);

				if (parLcomTar == nullptr) continue;

				const INT_PTR nSizeSrc = parLcomSrc->GetCount();
				for (INT_PTR j = 0; j < nSizeSrc; ++j)
				{
					_DGN_LCOM LcomDesignSrc;
					if (!parLcomSrc->Lookup(j + 1, LcomDesignSrc)) { continue; }

					_DGN_LCOM LcomDesignTar;
					BOOL bLookupLcom = parLcomTar->Lookup(LcomDesignSrc.OriginalLcomKey, LcomDesignTar);
					if (bIncludeElemInGrup ? !bLookupLcom : bLookupLcom) 	continue;
					if (!bIncludeElemInGrup)
					{
						if (!m_arLcomNoGrup.Lookup(LcomDesignSrc.OriginalLcomKey, LcomDesignTar)) continue;
					}
					//else if(!bLookupLcom) continue;
					// Set Target.
					if (IsLcomServ(LcomDesignSrc.LcomUlData.nActive)) { iLcomServGrup++; }
					iLcomGrup++;
					LcomDesignSrc.DesignLcomKey = iLcomGrup;
					m_arLcomGrup.SetAt(iLcomGrup, LcomDesignSrc);
				}
			}
			// Calculate Load Combination Number for Design.
			m_parLcomData = &m_arLcomGrup;
			m_iLCBDesignNum = iLcomGrup;
			m_iLCBServiceNum = iLcomServGrup;
			m_iLCBAddDesignNum = m_iLCBDesignNum;
		}
		else	// NOT-Exist Dlcm.
		{
			m_parLcomData = GetAppDgnLcomMap(enLcomApp);
			m_iLCBDesignNum = GetSizeAppDgnLcomMap(enLcomApp);
			// Calculate Load Combination Number for Design.
			m_iLCBServiceNum = GetSizeAppDgnLcomMapServ(enLcomApp);
			m_iLCBAddDesignNum = m_iLCBDesignNum;
		}
	}
}

void CDgnForceCtrl::Set_LcomDataForElemRC(int iElem)
{
	//!/ Set_LcomDataForDesignRC() 이 함수에서 Control해주면 Set_LcomDataForElemRC() 함수는 사용안함.

	// Add by ZINU.('05.06.15). Separate Special, Vertical from Strength/Stress.
	// iElem = Truss/Beam(ElemK), Wall(nInternalWallId). 
	if (iElem == 0)
	{
		m_parLcomData = &m_arLcomTotl;
		// Calculate Load Combination Number for Design.
		m_iLCBDesignNum = m_parLcomData->GetCount();
		m_iLCBServiceNum = m_iLcomServ;
		m_iLCBAddDesignNum = m_iLCBDesignNum;
	}
	else
	{
		const EN_DGNLCOM_APP enLcomApp = GetAppDgnLcomTypeByMemb(iElem);
		m_parLcomData = GetAppDgnLcomMap(enLcomApp);
		m_iLCBDesignNum = GetSizeAppDgnLcomMap(enLcomApp);

		// Calculate Load Combination Number for Design.
		m_iLCBServiceNum = GetSizeAppDgnLcomMapServ(enLcomApp);
		m_iLCBAddDesignNum = m_iLCBDesignNum;
	}
}

void CDgnForceCtrl::Cal_Wght_02Q0_Weak_GB()
{
	if (!GetOkEqGB())	return;	// Only GB50010-02 and EQ.

	// Call Execute Functions.
	// 전중비, 0.2Q0, 박약층 & 층전단재하능력.
	CStoryDisplDrift Drift;
	m_mDesignInfo.RemoveAll();
	int nStoryFilterType = D_STORY_FILTER_TYPE_SPACE;
	Drift.StoryStiffnessRegularity(m_mDesignInfo);

	// Get Eigen-value Period at 1st-Mode.
	T_FREQ_D FreqD;
	if (!m_pDoc->m_pPostCtrl->GetEiFreq(1, FreqD)) { FreqD.Initialize(); ASSERT(0); }
	m_dEigenPeriod = FreqD.dblFreq[2];

	CString csDesignOutput = CProduct::GetTestEnvValue(_ULS(Design Output));
	if (csDesignOutput == _ULS(yes))
	{
		rptwofstream fout(_T("zzzDataForGYUSUN.txt"), ios::trunc);
		T_STOR_DGN_K StorDgnK;
		T_STOR_DGN   StorDgnD;
		// Looping by Response Spectrums.
		T_SPLC_D SplcD;
		CArray<T_SPLC_K, T_SPLC_K> aSplcK;
		m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
		for (int i = 0; i < aSplcK.GetSize(); i++)
		{
			SplcD.Initialize();
			m_pDoc->m_pAttrCtrl->GetSplc(aSplcK[i], SplcD);

			// Looping by Stories.
			int nMdulSize = Drift.m_aMdulIndex.GetSize();
			for (int i = 0; i < nMdulSize; ++i)
			{
				int nCurMdulIndex = Drift.m_aMdulIndex.GetAt(i);
				CArray<T_STOR_BASE, T_STOR_BASE&> *paStoryBase;
				if (!Drift.m_mStoryBase.Lookup(nCurMdulIndex, paStoryBase)) { ASSERT(0); continue; }

				int nStorySize = paStoryBase->GetSize();
				for (int j = 0; j < nStorySize; ++j)
				{
					T_STOR_BASE StoryBase = paStoryBase->GetAt(j);
					if (nStoryFilterType == D_STORY_FILTER_TYPE_SPACE) { if (!Drift.IsActiveStoryByFilter_Space(nStorySize, j)) continue; }  // 공간 기준 층 filterring
					else { if (!Drift.IsActiveStoryByFilter_Level(nCurMdulIndex, StoryBase.StorK)) continue; }  // Level 기준 층 filterring

					// Print Data.
					StorDgnK.Set(StoryBase.StorK, aSplcK[i], nCurMdulIndex);
					if (!m_mDesignInfo.Lookup(StorDgnK, StorDgnD))	continue;
					CString str1 = _T(""); str1.Format(_T("%-10s"), SplcD.LoadCaseName);
					CString strStorName = StoryBase.strStorName;   if (nMdulSize > 1) strStorName.Format(_T("%s:%s"), StoryBase.strMdulName, StoryBase.strStorName);
					CString str2 = _T(""); str2.Format(_T("%-10s"), strStorName);
					CString str3 = _T(""); str3.Format(_T("%8g, %8g"), StorDgnD.dCtrlRatio[0], StorDgnD.dCtrlRatio[1]);
					CString str4 = _T(""); str4.Format(_T("%8g, %8g"), StorDgnD.dLambda[0], StorDgnD.dLambda[1]);
					CString str5 = _T(""); str5.Format(_T("%8g, %8g"), StorDgnD.dFactor[0], StorDgnD.dFactor[1]);
					if (j == 0)	fout << _T("LOADCASE = ") << str1 << _T(",  STORY = ") << str2 << _T(",  (0.2*Q0)/Qi = ") << str3 << _T(",  전중비 = ") << str4 << _T(",  박약층 = ") << str5 << endl;
					else			fout << _T("                     STORY = ") << str2 << _T(",  (0.2*Q0)/Qi = ") << str3 << _T(",  전중비 = ") << str4 << _T(",  박약층 = ") << str5 << endl;
				}
			}
		}
	}
}

void CDgnForceCtrl::Set_Wght_02Q0_Weak_GB(int iElemNo, T_ELEM_K ElemK/*0*/)
{
	if (!GetOkEqGB())	return;	// Only GB50010-02 and EQ. 
	// Get ElemK.
	// Wall (iElemKey is ElemK, iElemNo is InternalWallid).
	int iElemKey = (ElemK > 0 ? ElemK : iElemNo); // Wall if ElemK > 0.
	// Change by ZINU.('06.10.31). Get Pseudo Story Key.
	//  Real Story    Pseudo Story
	//  |        |    +--------+
	//  |   1F   | -> |   2F   |
	//  +--------+    |        | 
	T_STOR_K StorK;
	//m_pDoc->m_pPostCtrl->GetStorByElem(iElemKey, StorK);
	//int iMdul = 0;
	//if (!m_pDoc->m_pPostCtrl->GetStorByElem_space(iElemKey, StorK, iMdul))  { ASSERT(0);  return; }
	//CStoryData storyData(m_pDoc);
	CStoryData* pStoryData = m_pDoc->GetStoryData();
	if (!pStoryData->GetStorByElem(iElemKey, StorK)) { ASSERT(0);  return; }


	CArray<T_SPLC_K, T_SPLC_K> aSplcK;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
	int nSplcSize = aSplcK.GetSize();
	if (nSplcSize < 1) return;

	_DGN_SHRR_K ShrrK;
	_DGN_SHRR_D ShrrD;

	// Get Eigen-value Period at 1st-Mode.
	T_FREQ_D FreqD;
	if (!m_pDoc->m_pPostCtrl->GetEiFreq(1, FreqD)) { FreqD.Initialize(); ASSERT(0); }
	m_dEigenPeriod = FreqD.dblFreq[2];

	for (int i = 0; i < nSplcSize; i++)
	{
		// Add by ZINU.('06.10.27). EQ-Grade(Acceleration), Period by Eigen-value Analysis.
		T_SPLC_D SplcD;
		if (!m_pDoc->m_pAttrCtrl->GetSplc(aSplcK[i], SplcD)) { SplcD.Initialize(); continue; }

		ShrrK.Key.LcasK = aSplcK[i];

		ShrrK.Key.ElemK = iElemNo;
		// Divided ratios by beta-angle.
		// See wg_db > CSCWBRatio::GetStrengthByBetaAngle(...)
		// Element의 local vector 계산 : // Modify by GAY. PMS:4267. ('12.03.08). 계산 Source 삭제.
		T_ELEM_D ElemD;
		// Get Data from DesignInfo.
		T_STOR_DGN_K StorDgnK;
		T_STOR_DGN   StorDgnD;
		StorDgnK.Set(StorK, aSplcK[i]);
		StorDgnD.Initialize();
		if (!m_mDesignInfo.Lookup(StorDgnK, StorDgnD))	StorDgnD.Initialize();

		// Change by ZINU.('08.09.19). NO:2340, Only Global Factors by EQ Lcase, Not Change Global -> Local.
		if (SplcD.nDirection != 1) continue; // 1=X-Y, 2=Z.

		double dpWght[2] = { 0.,0. }, dp02Q0[2] = { 0.,0. }, dpWeak[2] = { 0.,0. };
		for (int j = 0; j < 2; j++)	// 0=X-Dir, 1=Y-Dir.
		{
			// Keep Divide LambdaRat.
//       //add by maxiao 2012-2-21. GB 50011-2010 5.2.5
//       if (bCH2010 && max(StorDgnD.dFactor[j], 1.0) > 1) dLambdaRat = 1.15 * dLambdaRat;

			dpWght[j] = max(StorDgnD.dLambda[j], 1.0);  // 전중비 Shear/WeightSum.
//       dpWght[j] = max(StorDgnD.dLambda[j]==0.0 ? 0.0 : dLambdaRat/StorDgnD.dLambda[j], 1.0);  // 전중비 Shear/WeightSum.
			dp02Q0[j] = max(StorDgnD.dCtrlRatio[j], 1.0);        	// (0.2*Q0)/Qi.
			dpWeak[j] = max(StorDgnD.dFactor[j], 1.0);			      // 박약층 & 층전단재하능력.
		}
		int iAngle = (int)fabs(SplcD.dblAngle) % 180;
		double dWght = (iAngle > 45 && iAngle < 135 ? dpWght[1] : dpWght[0]);
		double d02Q0 = (iAngle > 45 && iAngle < 135 ? dp02Q0[1] : dp02Q0[0]);
		double dWeak = (iAngle > 45 && iAngle < 135 ? dpWeak[1] : dpWeak[0]);
		//////////////////////////////////////////////////////////////////////////
		//add by maxiao 2012-2-24.  only Frame-Column & Frame-Beam connect to Frame-Column do 0.2Q0
		BOOL b02Q0 = FALSE;
		BOOL bIsFrme = (m_pDoc->m_pAttrCtrl->IsFrameType(m_iElemType) ? TRUE : FALSE);
		if (bIsFrme)
		{
			if (m_iElemKind == D_MBTP_COLUMN) b02Q0 = TRUE;//Column
			else
			{
				if (m_iElemKind == D_MBTP_BEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetElem(iElemNo, ElemD)) { ASSERT(0); continue; }
					for (int ii = 0; ii < 2; ii++)
					{
						CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
						m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(ElemD.elnod[ii], arKeyElem);   //절점에 연결된 요소번호
						for (int jj = 0; jj < arKeyElem.GetSize(); jj++)
						{
							int iCurElemNo = arKeyElem.GetAt(jj);
							if (iCurElemNo == iElemNo) continue;

							int iMemberType = m_pDoc->m_pAttrCtrl->GetMemberType(iCurElemNo);
							if (m_pDoc->m_pAttrCtrl->IsFrameType(iMemberType))
							{
								if (m_pDgnDataCtrl->Get_DgnGenMbtp(iCurElemNo) == 1) { b02Q0 = TRUE; break; }
							}
						}//connect Column
					}//Node
				}//Frame-Beam connect to Frame-Column do 0.2Q0
			}//Column
		}//Frame

		T_DCON_D DconD; DconD.Initialize();  m_pDoc->m_pAttrCtrl->GetDcon(DconD);
		if (!b02Q0 && DconD.DesignCode == _T("GB/T50010-10")) d02Q0 = 1.0;
		//////////////////////////////////////////////////////////////////////////

		// 박약층 & 층전단재하능력.
		ShrrD.ShrrBase[0].dRatAx = dWeak;
		ShrrD.ShrrBase[0].dRatMy = dWeak;
		ShrrD.ShrrBase[0].dRatMz = dWeak;
		ShrrD.ShrrBase[0].dRatVy = dWeak;
		ShrrD.ShrrBase[0].dRatVz = dWeak;
		// 전중비 Shear/WeightSum.
		ShrrD.ShrrBase[1].dRatAx = dWght;
		ShrrD.ShrrBase[1].dRatMy = dWght;
		ShrrD.ShrrBase[1].dRatMz = dWght;
		ShrrD.ShrrBase[1].dRatVy = dWght;
		ShrrD.ShrrBase[1].dRatVz = dWght;
		// (0.2*Q0)/Qi.
		ShrrD.ShrrBase[2].dRatAx = d02Q0;
		ShrrD.ShrrBase[2].dRatMy = d02Q0;
		ShrrD.ShrrBase[2].dRatMz = d02Q0;
		ShrrD.ShrrBase[2].dRatVy = d02Q0;
		ShrrD.ShrrBase[2].dRatVz = d02Q0;
		/*
				double dWght[2]={0.,0.}, d02Q0[2]={0.,0.}, dWeak[2]={0.,0.};
				for(j=0; j<2; j++)	// 0=X-Dir, 1=Y-Dir.
				{
					// Keep Divide LambdaRat.
					dWght[j] = max(StorDgnD.dLambda[j]==0.0 ? 0.0 : dLambdaRat/StorDgnD.dLambda[j], 1.0);  // 전중비 Shear/WeightSum.
					d02Q0[j] = max(StorDgnD.dCtrlRatio[j], 1.0);        	// (0.2*Q0)/Qi.
					dWeak[j] = max(StorDgnD.dFactor[j], 1.0);			        // 박약층 & 층전단재하능력.
				}
				// Modify by GAY. MNET:XXXX. ('07.02.14). Replace StorDgnD.dFactor -> dWeak, StorDgnD.dLambda -> dWght, StorDgnD.dCtrlRatio -> d02Q0.
				// Modify by ParkBong. MNET:2340.('08.09.10)
				// 수정내용:원래 Gen에서는 하중조건(지진하중조건)별로 X,Y방향의 전중비를 계산하여 그 값을 국부좌표계로 변환하여 부재력확대합니다.
				//          전중비는 Vector값이 아닌 Scalar값으로서 부재력확대시 국부좌표계에 의하여 방향만 변환하고 값을 변환하지 말아야 합니다.
				//          즉 전체좌표계에서 계산된 전중비값을 직접 사용하여야 한다는것입니다.직접 사용하기 위하여서는 해당 전중비값이 어떤 국부
				//          좌표계방향의 부재력에 영향을 주고 X,Y방향의 어떤값을 사용하여야 하는것입니다.
				// 0=X-Dir, 1=Y-Dir.
				// Change by ZINU.('06.10.25). Separate ux,uy,uz (1->3).
				// 박약층 & 층전단재하능력.
				double duxWeak = (fabs(dWeak[0]) > 1.0+cDgn_Zero ? 1.0 : 0.0);  // 해당값이 있으면 방향.Vector를 1로하고 값이 없으면 방향.Vector을 0으로 함.
				double duyWeak = (fabs(dWeak[1]) > 1.0+cDgn_Zero ? 1.0 : 0.0);  // 해당값이 있으면 방향.Vector를 1로하고 값이 없으면 방향.Vector을 0으로 함.
				double duzWeak = 0.0;                                           // 해당값이 있으면 방향.Vector를 1로하고 값이 없으면 방향.Vector을 0으로 함.
				CMathFunc::mathGCS2UCS(duxWeak,duyWeak,duzWeak, FrameLocalVector);//방향을 국부좌표계로 변환합니다.
				//방향을 변환하여 해당 방향에 영향이 있으면 즉duxWght(duyWght,duzWght)값이 1보다 크면 실제계산된 전체좌표계값을 사용하고 아니면 확대하지 않습니다.
				ShrrD.ShrrBase[0].dRatAx = (fabs(duxWeak) < cDgn_Zero ? 1.0 : max(dWeak[0],dWeak[1]));  //
				ShrrD.ShrrBase[0].dRatMy = (fabs(duzWeak) < cDgn_Zero ? 1.0 : max(dWeak[0],dWeak[1]));  // Change by ZINU.('08.09.08). duyWeak -> duzWeak Agree with Lee,Dae-Keun.
				ShrrD.ShrrBase[0].dRatMz = (fabs(duyWeak) < cDgn_Zero ? 1.0 : max(dWeak[0],dWeak[1]));  // Change by ZINU.('08.09.08). duzWeak -> duyWeak Agree with Lee,Dae-Keun.
				ShrrD.ShrrBase[0].dRatVy = (fabs(duyWeak) < cDgn_Zero ? 1.0 : max(dWeak[0],dWeak[1]));
				ShrrD.ShrrBase[0].dRatVz = (fabs(duzWeak) < cDgn_Zero ? 1.0 : max(dWeak[0],dWeak[1]));
				//double duxWeak = (fabs(dWeak[0]) > 1.0+cDgn_Zero ? fabs(dWeak[0]) : 0.0);
				//double duyWeak = (fabs(dWeak[1]) > 1.0+cDgn_Zero ? fabs(dWeak[1]) : 0.0);
				//double duzWeak = 0.0;
				//CMathFunc::mathGCS2UCS(duxWeak,duyWeak,duzWeak, FrameLocalVector);
				//ShrrD.ShrrBase[0].dRatAx = max(fabs(duxWeak), 1.0);
				//ShrrD.ShrrBase[0].dRatMy = max(fabs(duzWeak), 1.0); // Change by ZINU.('08.09.08). duyWeak -> duzWeak Agree with Lee,Dae-Keun.
				//ShrrD.ShrrBase[0].dRatMz = max(fabs(duyWeak), 1.0); // Change by ZINU.('08.09.08). duzWeak -> duyWeak Agree with Lee,Dae-Keun.
				//ShrrD.ShrrBase[0].dRatVy = max(fabs(duyWeak), 1.0);
				//ShrrD.ShrrBase[0].dRatVz = max(fabs(duzWeak), 1.0);

				// 전중비 Shear/WeightSum.
				double duxWght = (fabs(dWght[0]) > 1.0+cDgn_Zero ? 1.0 : 0.0);// 해당값이 있으면 방향.Vector를 1로하고 값이 없으면 방향.Vector을 0으로 함.
				double duyWght = (fabs(dWght[1]) > 1.0+cDgn_Zero ? 1.0 : 0.0);// 해당값이 있으면 방향.Vector를 1로하고 값이 없으면 방향.Vector을 0으로 함.
				double duzWght = 0.0;                                         // 해당값이 있으면 방향.Vector를 1로하고 값이 없으면 방향.Vector을 0으로 함.
				CMathFunc::mathGCS2UCS(duxWght,duyWght,duzWght, FrameLocalVector);//방향을 국부좌표계로 변환합니다.
				//방향을 변환하여 해당 방향에 영향이 있으면 즉duxWght(duyWght,duzWght)값이 1보다 크면 실제계산된 전체좌표계값을 사용하고 아니면 확대하지 않습니다.
				ShrrD.ShrrBase[1].dRatAx = (fabs(duxWght) < cDgn_Zero ? 1.0 : max(dWght[0],dWght[1]));
				ShrrD.ShrrBase[1].dRatMy = (fabs(duzWght) < cDgn_Zero ? 1.0 : max(dWght[0],dWght[1])); // Change by ZINU.('08.09.08). duyWght -> duzWght Agree with Lee,Dae-Keun.
				ShrrD.ShrrBase[1].dRatMz = (fabs(duyWght) < cDgn_Zero ? 1.0 : max(dWght[0],dWght[1])); // Change by ZINU.('08.09.08). duzWght -> duyWght Agree with Lee,Dae-Keun.
				ShrrD.ShrrBase[1].dRatVy = (fabs(duyWght) < cDgn_Zero ? 1.0 : max(dWght[0],dWght[1]));
				ShrrD.ShrrBase[1].dRatVz = (fabs(duzWght) < cDgn_Zero ? 1.0 : max(dWght[0],dWght[1]));
				//double duxWght = (fabs(dWght[0]) > fabs(dWght[1]) ? (fabs(dWght[0]) > 1.0+cDgn_Zero ? fabs(dWght[0]) : 0.0) : 0.0);
				//double duyWght = (fabs(dWght[0]) < fabs(dWght[1]) ? (fabs(dWght[1]) > 1.0+cDgn_Zero ? fabs(dWght[1]) : 0.0) : 0.0);
				//double duzWght = 0.0;
				//CMathFunc::mathGCS2UCS(duxWght,duyWght,duzWght, FrameLocalVector);
				//ShrrD.ShrrBase[1].dRatAx = max(fabs(duxWght), 1.0);
				//ShrrD.ShrrBase[1].dRatMy = max(fabs(duzWght), 1.0); // Change by ZINU.('08.09.08). duyWght -> duzWght Agree with Lee,Dae-Keun.
				//ShrrD.ShrrBase[1].dRatMz = max(fabs(duyWght), 1.0); // Change by ZINU.('08.09.08). duzWght -> duyWght Agree with Lee,Dae-Keun.
				//ShrrD.ShrrBase[1].dRatVy = max(fabs(duyWght), 1.0);
				//ShrrD.ShrrBase[1].dRatVz = max(fabs(duzWght), 1.0);

				// (0.2*Q0)/Qi.
				double dux02Q0 = (fabs(d02Q0[0]) > 1.0+cDgn_Zero ? 1.0 : 0.0);// 해당값이 있으면 방향.Vector를 1로하고 값이 없으면 방향.Vector을 0으로 함.
				double duy02Q0 = (fabs(d02Q0[1]) > 1.0+cDgn_Zero ? 1.0 : 0.0);// 해당값이 있으면 방향.Vector를 1로하고 값이 없으면 방향.Vector을 0으로 함.
				double duz02Q0 = 0.0;                                         // 해당값이 있으면 방향.Vector를 1로하고 값이 없으면 방향.Vector을 0으로 함.
				CMathFunc::mathGCS2UCS(dux02Q0,duy02Q0,duz02Q0, FrameLocalVector);//방향을 국부좌표계로 변환합니다.
				//방향을 변환하여 해당 방향에 영향이 있으면 즉duxWght(duyWght,duzWght)값이 1보다 크면 실제계산된 전체좌표계값을 사용하고 아니면 확대하지 않습니다.
				ShrrD.ShrrBase[2].dRatAx = 1.0;
				ShrrD.ShrrBase[2].dRatMy = (fabs(duz02Q0) < cDgn_Zero ? 1.0 : max(d02Q0[0],d02Q0[1]));max(fabs(duz02Q0), 1.0); // Change by ZINU.('08.09.08). duy02Q0 -> duz02Q0 Agree with Lee,Dae-Keun.
				ShrrD.ShrrBase[2].dRatMz = (fabs(duy02Q0) < cDgn_Zero ? 1.0 : max(d02Q0[0],d02Q0[1]));max(fabs(duy02Q0), 1.0); // Change by ZINU.('08.09.08). duz02Q0 -> duy02Q0 Agree with Lee,Dae-Keun.
				ShrrD.ShrrBase[2].dRatVy = (fabs(duy02Q0) < cDgn_Zero ? 1.0 : max(d02Q0[0],d02Q0[1]));max(fabs(duy02Q0), 1.0);
				ShrrD.ShrrBase[2].dRatVz = (fabs(duz02Q0) < cDgn_Zero ? 1.0 : max(d02Q0[0],d02Q0[1]));max(fabs(duz02Q0), 1.0);
				//double dux02Q0 = (fabs(d02Q0[0]) > 1.0+cDgn_Zero ? fabs(d02Q0[0]) : 0.0);
				//double duy02Q0 = (fabs(d02Q0[1]) > 1.0+cDgn_Zero ? fabs(d02Q0[1]) : 0.0);
				//double duz02Q0 = 0.0;
				//CMathFunc::mathGCS2UCS(dux02Q0,duy02Q0,duz02Q0, FrameLocalVector);
				//ShrrD.ShrrBase[2].dRatAx = max(fabs(dux02Q0), 1.0);
				//ShrrD.ShrrBase[2].dRatMy = max(fabs(duz02Q0), 1.0); // Change by ZINU.('08.09.08). duy02Q0 -> duz02Q0 Agree with Lee,Dae-Keun.
				//ShrrD.ShrrBase[2].dRatMz = max(fabs(duy02Q0), 1.0); // Change by ZINU.('08.09.08). duz02Q0 -> duy02Q0 Agree with Lee,Dae-Keun.
				//ShrrD.ShrrBase[2].dRatVy = max(fabs(duy02Q0), 1.0);
				//ShrrD.ShrrBase[2].dRatVz = max(fabs(duz02Q0), 1.0);
		*/

		// Set ShrrD.
		m_mShrrData.SetAt(ShrrK.KeyMap, ShrrD);
	}
}

BOOL CDgnForceCtrl::Get_Wght_02Q0_Weak_GB(int iElemNo, int iLcasNo, _DGN_SHRR_D& ShrrD)
{
	ShrrD.Initialize();
	if (!GetOkEqGB())	return FALSE;	// Only GB50010-02 and EQ.
	if (m_bDampCheck) return FALSE;

	if (m_mShrrData.GetCount() > 0)
	{
		_DGN_SHRR_K ShrrK;
		ShrrK.Key.ElemK = iElemNo;
		ShrrK.Key.LcasK = iLcasNo;
		if (!m_mShrrData.Lookup(ShrrK.KeyMap, ShrrD))	ShrrD.Initialize();
	}
	return TRUE;
}

void CDgnForceCtrl::Set_BoundaryElemCondition(int iElemNo)
{
	// Add by ZINU.('08.09.26). Maintain Current Unit when intermidate return.
	CCurUnitSaver Save(TRUE);

	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = m_nCode_Length;
	CngIndex.nBase_Force = m_nCode_Force;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	// <Remember> Data를 받아올때는 Code Unit기준
	//////////////////////////////////////////////

	// Add by ZINU.('08.09.01). NO:3664, Check Bottom Wall at Same Wall-ID if Boundary Element.
	// Set Condition by Bottom Wall in case of Same Wall-ID.
	// Lcom including EQ Lcase.
	_DGN_BOUND_ELEM BoundaryElem; BoundaryElem.Initialize();
	BOOL bBoundaryElem = FALSE;

	if (!m_pDoc->m_pAttrCtrl->IsWall(m_iElemType)) { ASSERT(0); return; } // Only wall.

	// Get User Wall ID.
	T_WALL_K WallK;
	if (!m_pDoc->m_pPostCtrl->GetWallKey(iElemNo, WallK.keymap)) { ASSERT(0); return; }
	// Get Internal Wall-Id List at Wall ID.
	T_WALL_D WallD;
	T_ELEM_D ElemD;
	T_NODE_D NodeD;
	CArray<unsigned int, unsigned int> arInternalWallIdList;
	CArray<T_ELEM_K, T_ELEM_K> arElemList;
	if (!m_pDoc->m_pPostCtrl->GetInternalWallIdList(WallK.key.wallid, arInternalWallIdList)) { ASSERT(0); return; }
	int iInternalWallIdSize = arInternalWallIdList.GetSize();
	BOOL bFirst = TRUE;
	int iBotElemK = 0;
	double dBotZ = 0.0;
	T_WALL_K WallK2;
	for (int i = 0; i < iInternalWallIdSize; i++)
	{
		if (!m_pDoc->m_pPostCtrl->GetWallKey(arInternalWallIdList[i], WallK2.keymap)) continue;
		if (!m_pDoc->m_pPostCtrl->GetWallElemList(WallK2.keymap, arElemList)) continue;
		//if (!m_pDoc->m_pAttrCtrl->GetElem(arElemList[0], ElemD))   continue;  // Only First Element.
		//if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD))  continue;  // Only First Node.
		//if (bFirst) { dBotZ = NodeD.z; bFirst = FALSE; }
		//if (NodeD.z < dBotZ + cDgn_Zero)
		//{
		//	iBotElemK = arElemList[0];
		//	dBotZ = NodeD.z;
		//}
		if(WallK2.keymap == WallK.keymap)
		{
			iBotElemK = arElemList[0];
			break;
		}
	}
	// Get Internal-Wall ID & Elem Data.
	unsigned int iBotElemNo = 0;
	if (!m_pDoc->m_pPostCtrl->GetInternalWallId(iBotElemK, iBotElemNo)) { ASSERT(0); return; }
	if (!m_pDoc->m_pAttrCtrl->GetElem(iBotElemK, ElemD)) { ASSERT(0); return; }
	// Get KeyWall.
	if (!m_pDoc->m_pPostCtrl->GetWallKey(iBotElemNo, WallK.keymap)) { ASSERT(0); return; }
	// Get Wall-ID, Story Name.
	T_STOR_D StorD;
	if (!m_pDoc->m_pAttrCtrl->GetStor(WallK.key.storid, StorD)) { ASSERT(0); return; }
	// Get fck.
	T_MATD_D MatdD;
	if (!GetDgnWallMatd(WallK.keymap, ElemD.elmat, MatdD)) { ASSERT(0); return; }
	double dfck = MatdD.Data1.Design.C_fc;
	// Get Area.
	if (!m_pDoc->m_pPostCtrl->GetWallData(WallK.keymap, WallD)) { ASSERT(0); return; }
	double dLw = WallD.dLength;
	double dTw = WallD.dThickness;
	double dAc = dTw * dLw;
	double dSy = dTw * pow(dLw, 2) / 6.;

	// Set Lcom Type.
	Set_LcomDataForElem(iBotElemNo);

	T_LCOM_D LcomD;
	T_STRW_D StrwMaxD; StrwMaxD.Initialize();
	T_STRW_D StrwMinD; StrwMinD.Initialize();
	T_STRW_D StrwAbsD; StrwAbsD.Initialize();
	double dSigmaMax = 0.0;
	// Get Load Combination Data.
	_DGN_LCOM LcomDesign;
	for (int i = 0; i < m_iLCBAddDesignNum; i++)
	{
		// Set End Moment and Force and Moment Data at All Position.
		if (!m_parLcomData->Lookup(i + 1, LcomDesign)) continue;
		if (LcomDesign.iSeismicType == 0)  continue; // Only Including EQ Lcase.
		// Get Force by Lcom.
		if (!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_COMB_CONCRETE, LcomDesign.OriginalLcomKey, LcomD))	continue;
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);
		if (!m_pDoc->m_pPostCtrl->GetStrwNew(iBotElemNo, &StrwMaxD, &StrwMinD, &StrwAbsD))	continue;

		double dFxxI = StrwMaxD.dblForce[1][0];
		double dMuyI = StrwMaxD.dblForce[1][4];
		double dFxxJ = StrwMaxD.dblForce[0][0];
		double dMuyJ = StrwMaxD.dblForce[0][4];
		// Check Axial Stress, dFxx = Comp(-), Tens(+).
		double dSigma_xI = (-1)*dFxxI / dAc + fabs(dMuyI) / dSy;
		double dSigma_xJ = (-1)*dFxxJ / dAc + fabs(dMuyJ) / dSy;
		double dSigma_x = max(dSigma_xI, dSigma_xJ);
		if (dSigma_x > 0.2*dfck && MT(dSigma_x, dSigmaMax))
		{
			dSigmaMax = dSigma_x;
			bBoundaryElem = TRUE;
			BoundaryElem.bContinue = TRUE;
			BoundaryElem.iWallId = WallK.key.wallid;  // Wall-ID.
			BoundaryElem.strStorNa = StorD.StoryName;   // Story Name.
			BoundaryElem.d02fck = 0.2*dfck;
			BoundaryElem.iPosiNo = (dSigma_xI > dSigma_xJ ? 0 : 1); // 0=Bot, 1=Top.
			BoundaryElem.strLcomNa = LcomDesign.DesignLcomNa;
			BoundaryElem.dPu = (-1)*(dSigma_xI > dSigma_xJ ? dFxxI : dFxxJ);
			BoundaryElem.dMuy = (dSigma_xI > dSigma_xJ ? dMuyI : dMuyJ);
			BoundaryElem.dLw = dLw;
			BoundaryElem.dTw = dTw;
			BoundaryElem.dSigma_x = dSigma_x;
		}
		else if (dSigma_x > 0.15*dfck && !bBoundaryElem && MT(dSigma_x, dSigmaMax))
		{
			dSigmaMax = dSigma_x;
			bBoundaryElem = FALSE;
			BoundaryElem.bContinue = TRUE;
			BoundaryElem.iWallId = WallK.key.wallid;  // Wall-ID.
			BoundaryElem.strStorNa = StorD.StoryName;   // Story Name.
			BoundaryElem.d02fck = 0.2 * dfck;
			BoundaryElem.iPosiNo = ( dSigma_xI > dSigma_xJ ? 0 : 1 ); // 0=Bot, 1=Top.
			BoundaryElem.strLcomNa = LcomDesign.DesignLcomNa;
			BoundaryElem.dPu = ( -1 ) * ( dSigma_xI > dSigma_xJ ? dFxxI : dFxxJ );
			BoundaryElem.dMuy = ( dSigma_xI > dSigma_xJ ? dMuyI : dMuyJ );
			BoundaryElem.dLw = dLw;
			BoundaryElem.dTw = dTw;
			BoundaryElem.dSigma_x = dSigma_x;
		}
		else if(MT(dSigma_x, dSigmaMax))
		{
			dSigmaMax = dSigma_x;
			bBoundaryElem = FALSE;
			BoundaryElem.bContinue = FALSE;
			BoundaryElem.iWallId = WallK.key.wallid;  // Wall-ID.
			BoundaryElem.strStorNa = StorD.StoryName;   // Story Name.
			BoundaryElem.d02fck = 0.2 * dfck;
			BoundaryElem.iPosiNo = ( dSigma_xI > dSigma_xJ ? 0 : 1 ); // 0=Bot, 1=Top.
			BoundaryElem.strLcomNa = LcomDesign.DesignLcomNa;
			BoundaryElem.dPu = ( -1 ) * ( dSigma_xI > dSigma_xJ ? dFxxI : dFxxJ );
			BoundaryElem.dMuy = ( dSigma_xI > dSigma_xJ ? dMuyI : dMuyJ );
			BoundaryElem.dLw = dLw;
			BoundaryElem.dTw = dTw;
			BoundaryElem.dSigma_x = dSigma_x;
		}
	}
	// Set Boundary Element.
	m_mBoundaryElemChk.SetAt(iElemNo, bBoundaryElem);
	m_mBoundaryElemData.SetAt(iElemNo, BoundaryElem);
}

BOOL CDgnForceCtrl::Get_BoundaryElemCondition(int iElemNo, _DGN_BOUND_ELEM& BoundaryElem, BOOL bGetData/*=FALSE*/)
{
	BOOL bBoundaryElem = FALSE;
	// Only EQ.
	if (!m_bEqSpecial) return FALSE;
	if (!m_mBoundaryElemChk.Lookup(iElemNo, bBoundaryElem))
	{
		Set_BoundaryElemCondition(iElemNo);
		if (!m_mBoundaryElemChk.Lookup(iElemNo, bBoundaryElem)) { ASSERT(0); return FALSE; }
	}
	if (bGetData)
	{
		BoundaryElem.Initialize();
		if (!m_mBoundaryElemData.Lookup(iElemNo, BoundaryElem)) { /*ASSERT(0);*/ return FALSE; }
	}
	return bBoundaryElem || BoundaryElem.bContinue;
}

void CDgnForceCtrl::Set_ForceMomentBySeperate(int iElem, int iPosi, _DGN_LCOM& LcomDesign)
{
	_DGN_EXPAND_LOAD_ELEM ExpandElemD;  ExpandElemD.Initialize();
	if (m_pDgnDataCtrl->IsJUD())
	{// Add by SHIN 11.06.15		
		T_LCOM_K LcomK = LcomDesign.OriginalLcomKey;
		if (!Get_ExpandLoadElemD(LcomK, iElem, ExpandElemD)) ExpandElemD.Initialize();
	}
	Set_ForceMomentBySeperate(iElem, iPosi, LcomDesign, ExpandElemD);
}

void CDgnForceCtrl::Set_ForceMomentBySeperate(int iElem, int iPosi, _DGN_LCOM& LcomDesign, _DGN_EXPAND_LOAD_ELEM& ExpandElemD)
{
	InitExpandElem();
	if (ExpandElemD.bIsExpand)
	{
		SetExpandElem(ExpandElemD);
	}

	// Add by ZINU.('03.02.15).
	if (iPosi >= 0)
	{
		m_iPosiNo = iPosi;
		m_iPosiNoNM = iPosi;
		m_iPosiNoPM = iPosi;
		m_iPosiNoV = iPosi;
	}

	Reset_Force();
	m_iOrgLcomKey = LcomDesign.OriginalLcomKey;
#if defined(_TOWER)
	Get_ForceMomentTower(iElem, iPosi, LcomDesign);
#else
	Get_ForceMoment(iElem, iPosi, LcomDesign);
#endif
}

// Coded by ZINU.('06.10.10) PROFILING.
void CDgnForceCtrl::ProfilerDestroy()
{
	ProfileWrite::profilingEnd();
}

void CDgnForceCtrl::Profiling(TCHAR* strTxt)
{
	PROFILING(strTxt);
}

void CDgnForceCtrl::Get_ForceMoment(int iElem, int iPosi, _DGN_LCOM& LcomDesign)
{
	//*/PROFILING(_T("Get_ForceMoment"));
	// Change by ZINU.('05.05.16). Speed up for Making Forces.
	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bCngUnit = FALSE;
	if (CurIndex.nBase_Length != m_nCode_Length || CurIndex.nBase_Force != m_nCode_Force)
	{
		CngIndex.nBase_Length = m_nCode_Length;
		CngIndex.nBase_Force = m_nCode_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
		bCngUnit = TRUE;
	}
	//++++++++++++++++++++++++++++++++++++++++++++

	// Add/Modify by Seungjun PMS.4511  '20120821 for JFD
	m_GroundBeamForce.Initialize();
	if (m_pDoc->EnableJFD())
	{
		if (!m_pDoc->m_pPostCtrl->GetDesignResult()->ReadConcreteDesignForceGroundBeam(iElem, m_GroundBeamForce))
			m_GroundBeamForce.Initialize();

		if (!m_GroundBeamForce.bCheck) m_GroundBeamForce.Initialize();
	}

	// Check Max Force or Min Force.
	BOOL bMaxLcom = LcomDesign.bMaxLcom;
	// Remove by ZINU.('05.06.22). For Special, Vertical.
//*/	if(LcomDesign.DesignLcomKey > m_iLCBAddDesignNum)	ASSERT(0);
	// Add by ZINU.('03.11.20). Set ElemList for Cb,Cmy,Cmz.
	SetElemListForBeamEndMoment(iElem);
	// Add by ZINU.('03.11.07). Get Element No/Position between 1/4 and 3/4 at Member.
	SetMaxMinMomentPosiForBeam(iElem, bMaxLcom, LcomDesign.nLcomKind);
	// Add by GAY. MNET:2683. ('07.05.04). Set Load Component Type to consider Crane Load.
	m_pDgnDataCtrl->m_pMembCtrl->Set_LcomCompNo(LcomDesign.iLcomCompType);

	T_LCOM_D_UL LcomUlData = LcomDesign.LcomUlData;
	int iLcomUlNum = LcomUlData.Combination.GetSize();
	// Add by ZINU.('05.06.15). Consider Existing LL, EL.
	if (!m_arExistLL.Lookup(LcomUlData.LoadCombId, m_bExistLL))	m_bExistLL = FALSE;
	if (!m_arExistEL.Lookup(LcomUlData.LoadCombId, m_bExistEL))	m_bExistEL = FALSE;

	// PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.
	m_dNTC08LcasN = 1.0;
	if (m_bNTC08Wall && IsEqSpecialLcom(LcomDesign.DesignLcomKey) && IsLcomStrn(LcomUlData.nActive))
	{
		m_dNTC08LcasN = LcomDesign.bMaxLcom ? 1.5 : 0.5;
	}

	int i = 0;
	// Add by ZINU.('04.02.13). For Creating Lcase by Lcom (Load Contribution).
	m_bExistLctb = FALSE;
	_DGN_LCOM_LCTB LcomLctb;
	// Change by ZINU.('05.05.23). Get arLctbK at MembCtrl. (To speed up).
	m_pDgnDataCtrl->m_pMembCtrl->GetLcomLctb(LcomDesign.OriginalLcomKey, LcomLctb);
	int iLctbNum = LcomLctb.arLctbK.GetSize();

	// Gen에서는 NComb에 포함된 각각의 load type으로 부재력을 만듬
	// Civil/UMD에서는 NComb을 하나의 load type으로 생각하고 부재력을 만든다.
	//  그리고 NComb의 User type으로 만들어지는 부재력은 Nonsway로 한다.
#if defined(_CIVIL)
	if (iLctbNum > 0) m_bExistLctb_CVL = TRUE;
	//m_bUse_NLcomb = FALSE; // Seungjun-20130821 : 여기서 할당하면 이전에 이 변수 쓰는 곳이 제대로 동작하지 않음.
#else
	//m_bUse_NLcomb = TRUE; // Seungjun-20130821 : 여기서 할당하면 이전에 이 변수 쓰는 곳이 제대로 동작하지 않음.
#endif

	if (iLctbNum > 0 && m_bUse_NLcomb)	m_bExistLctb = TRUE;
	SetService(IsLcomServ(LcomUlData.nActive) ? true : false);

	if (m_bExistLctb)	// Non-linear.
	{
		Sta_ForceByLctb(iElem, bMaxLcom);
		for (i = 0; i < iLctbNum; i++)
		{
			T_LCTB_D LctbD; LctbD.Initialize();
			m_pDoc->m_pAttrCtrl->GetLctb(LcomLctb.arLctbK[i], LctbD);
			for (int j = 0; j < LctbD.aLctbBase.GetSize(); j++)
			{
				unsigned int iAnalType = LctbD.aLctbBase[j].iAnalType;
				unsigned int iLcaseKey = LctbD.aLctbBase[j].LoadCaseKey;
				double dFactor = LctbD.aLctbBase[j].dFactor;
				Cng_ForceByLctb(iElem, iPosi, iAnalType, iLcaseKey, dFactor, bMaxLcom);
			}
		}
		End_ForceByLctb();
		for (i = 0; i < iLctbNum; i++)
		{
			T_LCTB_D LctbD; LctbD.Initialize();
			m_pDoc->m_pAttrCtrl->GetLctb(LcomLctb.arLctbK[i], LctbD);
			for (int j = 0; j < LctbD.aLctbBase.GetSize(); j++)
			{
				// Change by ZINU.('05.05.03). Consider SRSS (iAnalType==D_LCOM_CB_STEEL, D_LCOM_CB_CONCRETE, D_LCOM_CB_SRC).
				unsigned int iAnalType = LctbD.aLctbBase[j].iAnalType;
				unsigned int iLcaseKey = LctbD.aLctbBase[j].LoadCaseKey;
				double dFactor = LctbD.aLctbBase[j].dFactor;

				switch (iAnalType)
				{
				case D_LCOM_STATIC: Get_StaticForce(iElem, iPosi, iLcaseKey, dFactor); break;
				case D_LCOM_SPECTRUM: Get_SpectrumForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType); break;
				case D_LCOM_SPECTRUM_ND: Get_SpectrumForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType); break;
				case D_LCOM_ECCENSPEC: Get_SpectrumForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType); break;
				case D_LCOM_ECCENSPEC_ND: Get_SpectrumForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType); break;
				case D_LCOM_HISTORY: Get_RemnantForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
				case D_LCOM_MOVING: Get_MovingForce(iElem, iPosi, iLcaseKey, dFactor, bMaxLcom); break;
				case D_LCOM_SETTLEMENT: Get_RemnantForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
				case D_LCOM_CRANE: Get_CraneForce(iElem, iPosi, iLcaseKey, dFactor, bMaxLcom); break;
				case D_LCOM_STAGE: Get_StageForce(iElem, iPosi, iLcaseKey, dFactor); break;
				case D_LCOM_CB_STEEL: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
				case D_LCOM_CB_CFSTEEL: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
				case D_LCOM_CB_CONCRETE: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
				case D_LCOM_CB_SRC: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
				case D_LCOM_CB_ALUMINUM: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
				case D_LCOM_CB_STLCOMP: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
				case D_LCOM_CB_SEISMIC: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
				case D_LCOM_CB_LINEAR_EVAL: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
				default: ASSERT(0); break;
				}
			}
		}
	}
	else	// Linear.
	{
		int nRatChkStaticSize = LcomDesign.aRatChkStatic.GetSize();
		m_bEndLCase = FALSE;
		for (i = 0; i < iLcomUlNum; i++)
		{
			// Change by ZINU.('05.05.03). Consider SRSS (iAnalType==D_LCOM_CB_STEEL, D_LCOM_CB_CONCRETE, D_LCOM_CB_SRC).
			// LcomUlData.Combination[i].AnalType is...
			// D_LCOM_STATIC       1 (Live Load in case of Column - Reduction Factor(Axial) 고려)
			// D_LCOM_SPECTRUM     2 (Earthquake - Scale Up Factor(All) 고려)
			// D_LCOM_ECCENSPEC		14 (Earthquake - Scale Up Factor(All) 고려)
			// D_LCOM_HISTORY      3 
			// D_LCOM_MOVING       4 (Load Combination - Max, Min 두개로 계산)
			// D_LCOM_SETTLEMENT   5 (Load Combination - Max, Min 두개로 계산)
			// D_LCOM_STAGE       13
			// In case of Concrete, if _T("E(STATIC)") Exists, Fyy*2, Fzz*2 (내진설계 규준)
			unsigned int iAnalType = LcomUlData.Combination[i].AnalType;
			unsigned int iLcaseKey = LcomUlData.Combination[i].LoadCaseKey;
			double dFactor = LcomUlData.Combination[i].Factor;
			if (IsRatingCode())
			{
				if (i < nRatChkStaticSize) m_bChkStatic = LcomDesign.aRatChkStatic[i];
				if (i == iLcomUlNum - 1) m_bEndLCase = TRUE;
			}
			switch (iAnalType)
			{
			case D_LCOM_STATIC: Get_StaticForce(iElem, iPosi, iLcaseKey, dFactor); break;
			case D_LCOM_SPECTRUM: Get_SpectrumForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType); break;
			case D_LCOM_SPECTRUM_ND: Get_SpectrumForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType); break;
			case D_LCOM_ECCENSPEC: Get_SpectrumForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType); break;
			case D_LCOM_ECCENSPEC_ND: Get_SpectrumForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType); break;
			case D_LCOM_HISTORY: Get_RemnantForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
			case D_LCOM_MOVING: Get_MovingForce(iElem, iPosi, iLcaseKey, dFactor, bMaxLcom); break;
			case D_LCOM_SETTLEMENT: Get_RemnantForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
			case D_LCOM_CRANE: Get_CraneForce(iElem, iPosi, iLcaseKey, dFactor, bMaxLcom); break;
			case D_LCOM_STAGE: Get_StageForce(iElem, iPosi, iLcaseKey, dFactor); break;
			case D_LCOM_CB_STEEL: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
			case D_LCOM_CB_CFSTEEL: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
			case D_LCOM_CB_CONCRETE: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
			case D_LCOM_CB_SRC: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
			case D_LCOM_CB_ALUMINUM: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
			case D_LCOM_CB_STLCOMP: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
			case D_LCOM_CB_SEISMIC: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
			case D_LCOM_CB_LINEAR_EVAL: Get_LcomForce(iElem, iPosi, iLcaseKey, dFactor, iAnalType, bMaxLcom); break;
			default: ASSERT(0); break;
			}
		}
	}
	// Save Forces.
	Save_TotalForce(iElem, LcomDesign.iSeismicType, LcomDesign.bLongTerm);
	// Add by ZINU.('03.10.31). By (D+0.5L) for GB50010-02.
	Get_DeadLiveGB(iElem, iPosi);
	Get_DeadLiveIS(iElem, iPosi);

	// Change by ZINU.('05.05.16). Speed up for Making Forces.
	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Set Data by Current Units.
	if (bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	//++++++++++++++++++++++++++++++++++++++++++++
	SetService(false);
}

void CDgnForceCtrl::Get_ForceMomentTower(int iElemNo, int iPosiNo, _DGN_LCOM& LcomDesign)
{
#if defined(_TOWER)
	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = m_nCode_Length;
	CngIndex.nBase_Force = m_nCode_Force;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	//++++++++++++++++++++++++++++++++++++++++++++

	// Get Force Data by LCB.
	T_LCOM_D LcomData;
	LcomData.Initialize();
	m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, m_iOrgLcomKey, LcomData);
	// Save Normal, Abnormal Data.
	m_bAbnormalForce = CDBLib::IsAbnormalLcom(LcomData);
	// Get Force.
	T_STRT_D StrtMaxData, StrtMinData;
	// Change by ZINU.('02.2.7). ElemType (0:주주재, 1:복재, 2:암주재, 3:암대각재, 4:암조재, 5:보조재).
	if (m_iTowerElem == 5)	// Redundant.
	{
		CArray<T_NODE_K, T_NODE_K>	arNodeKey;
		CArray<T_ELEM_K, T_ELEM_K>	arElemKey;
		T_ELEM_D ElemData;
		ElemData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetElem(iElemNo, ElemData);
		ASSERT(bCheck);
		for (int i = 0; i < D_ELEM_MAXNOD; i++)
		{
			if (ElemData.elnod[i] > 0)	arNodeKey.Add(ElemData.elnod[i]);
		}
		m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(arNodeKey, arElemKey);

		double dPuMax = 0.0, dPuMin = 0.0;
		for (i = 0; i < arElemKey.GetSize(); i++)
		{
			T_ELEM_K ElemKey = arElemKey.GetAt(i);
			int iTowerElem = 0;
			m_aTowerTypeData.Lookup((int&)ElemKey, iTowerElem);
			double dAxialMax = 0.0, dAxialMin = 0.0;
			if (iTowerElem != 5)	// If not Redundant.
			{
				StrtMaxData.Initialize();
				StrtMinData.Initialize();
				GetStrtDgn(ElemKey, D_LOADCASE_COMB_STEEL, m_iOrgLcomKey, &StrtMaxData, &StrtMinData);
				dAxialMax = max(StrtMaxData.dblForce[0], StrtMaxData.dblForce[1]);
				dAxialMin = min(StrtMaxData.dblForce[0], StrtMaxData.dblForce[1]);
			}
			if (i == 0) { dPuMax = dAxialMax; dPuMin = dAxialMin; }
			if (dPuMax < dAxialMax)	dPuMax = dAxialMax;
			if (dPuMin > dAxialMin)	dPuMin = dAxialMin;
		}
		// Set Force. If compression exists, Pu is PuMin(-). 
		StrtMaxData.Initialize();
		double dDp = 0.03;	// Distributed Percent(3%).
		if (dPuMin < 0) { StrtMaxData.dblForce[0] = dDp * dPuMin; StrtMaxData.dblForce[1] = dDp * dPuMin; }	// Compression.
		else { StrtMaxData.dblForce[0] = dDp * dPuMax; StrtMaxData.dblForce[1] = dDp * dPuMax; }	// Tension.
	}
	else
	{
		StrtMaxData.Initialize();
		StrtMinData.Initialize();
		GetStrtDgn(iElemNo, D_LOADCASE_COMB_STEEL, m_iOrgLcomKey, &StrtMaxData, &StrtMinData);
	}
	// Save Forces and Moments by Position.
	int iPosStaNo = 0;
	int iPosTotNo = 0;
	if (iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
	else
	{
		iPosStaNo = iPosiNo;
		iPosTotNo = iPosiNo + 1;
	}
	for (int i = iPosStaNo; i < iPosTotNo; i++)
	{
		if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
		{
			m_FxxD[i] = StrtMaxData.dblForce[i / 4];
			m_FyyD[i] = 0.0;
			m_FzzD[i] = 0.0;
			m_BMyD[i] = 0.0;
			m_BMzD[i] = 0.0;
		}
	}
	// Save Forces.
	Save_TotalForce(iElemNo, LcomDesign.iSeismicType, LcomDesign.bLongTerm);
	Save_TotalForceByLctb(iElemNo, FALSE);

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Set Data by Current Units.
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	//++++++++++++++++++++++++++++++++++++++++++++
#endif
}

bool CDgnForceCtrl::IsDeadLoadType(CString strLcaseType, int iAnalType, int iLcaseKey)
{
	bool bDL = false;

	if (iAnalType == D_LCOM_STATIC)
	{
		bDL = CDBLib::IsStldDL(strLcaseType);
		if (bDL) return bDL;

		// Live load는 제외
		if (CDBLib::IsStldLL(strLcaseType)) return bDL;
	}

	// Eurocode only
	if (m_iDgnCode == Eurocode2_04 || m_iDgnCode == Eurocode2_2_05)
	{
		// Member type is beam
		if (m_iElemKind == D_MBTP_BEAM)
		{
			// And Serviceability Load combination.
			if (IsService())
			{
				T_LCTP_D LctpD; LctpD.Initialize();
				if (m_pDoc->m_pAttrCtrl->GetLctp(LctpD))
				{
					const INT_PTR nLong = LctpD.arLongKey.GetSize();
					for (INT_PTR i = 0; i < nLong; ++i)
					{
						BOOL bTypeOK = (iAnalType == LctpD.arLongType[i] ? TRUE : FALSE);
						BOOL bKeyOK = (iLcaseKey == LctpD.arLongKey[i] ? TRUE : FALSE);
						if (bTypeOK && bKeyOK) { bDL = true; break; }
					}
				}
			}
		}
	}
	return bDL;
}

bool CDgnForceCtrl::IsLiveLoadType(CString strLcaseType, int iAnalType)
{
	if(iAnalType == D_LCOM_STATIC)
	{
		if (CDBLib::IsStldLL(strLcaseType)) return true;
		if (CDBLib::IsStldSL(strLcaseType)) return true;
		if (CDBLib::IsStldRL(strLcaseType)) return true;
		return false;
	}
	return false;
}

BOOL CDgnForceCtrl::Get_StaticForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor)
{
	//*/PROFILING(_T("Get_StaticForce"));
	if (iLcaseKey == 0) { ASSERT(0); return FALSE; }
	if (dFactor == 0.0) {/*ASSERT(0);*/ return FALSE; }

	T_DCON_D DconD; DconD.Initialize();  
	m_pDoc->m_pAttrCtrl->GetDcon(DconD);

	int iLcaseType = D_LOADCASE_STATIC;
	// Get Load Case Type(D,L,W,E,...).
	T_STLD_D Lcase;
	Lcase.Initialize();
	m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, Lcase);

	// Change by ZINU.('02.11.26).
	T_STRB_D  StrbMaxData, StrbMinData;
	T_STRT_D  StrtMaxData, StrtMinData;
	T_STRW_D  StrwMaxData, StrwMinData;
	T_STRP_DL StrpMaxData, StrpMinData;
	if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
	{
		StrbMaxData.Initialize();
		StrbMinData.Initialize();
		GetStrbDgn(iElemNo, iLcaseType, iLcaseKey, &StrbMaxData, &StrbMinData);

		// Add/Modify by Seungjun PMS.4511  '20120821 for JFD
		if (m_pDoc->EnableJFD() && m_GroundBeamForce.bCheck)
		{
			_ICS_LCAS_K IcsKey;
			IcsKey.Key.iType = 0;
			IcsKey.Key.LcasK = iLcaseKey;
			int nLcase = m_GroundBeamForce.FiffBase[0].GetSize();
			for (int i = 0; i < nLcase; i++)
			{
				T_FIFF_BASE FiffBaseD;
				FiffBaseD = m_GroundBeamForce.FiffBase[0].GetAt(i);
				_ICS_LCAS_K IcsKey2;
				IcsKey2.Key.iType = FiffBaseD.LcasType;
				IcsKey2.Key.LcasK = FiffBaseD.LcasK;
				if (FiffBaseD.bCheck && IcsKey2.KeyMap == IcsKey.KeyMap)
				{
					for (int j = 0; j < 4; j++)
					{
						int ni = 0, nj = 0;
						if (j == 0) { ni = 0; nj = 1; }
						else if (j == 1) { ni = 1; nj = 2; }
						else if (j == 2) { ni = 2; nj = 3; }
						else if (j == 2) { ni = 3; nj = 4; }

						T_FIFF_BASE FiffBaseDi, FiffBaseDj;
						FiffBaseDi = m_GroundBeamForce.FiffBase[ni].GetAt(i);
						FiffBaseDj = m_GroundBeamForce.FiffBase[nj].GetAt(i);

						StrbMaxData.dblForce[j][2] += FiffBaseDi.dFzz;
						StrbMaxData.dblForce[j][4] += FiffBaseDi.dMuy;
						StrbMaxData.dblForce[j][8] += FiffBaseDj.dFzz;
						StrbMaxData.dblForce[j][10] += FiffBaseDj.dMuy;
					}
				}
			}
		}
	}
	else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
	{
		StrtMaxData.Initialize();
		StrtMinData.Initialize();
		GetStrtDgn(iElemNo, iLcaseType, iLcaseKey, &StrtMaxData, &StrtMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
	{
		StrwMaxData.Initialize();
		StrwMinData.Initialize();
		GetStrwDgn(iElemNo, iLcaseType, iLcaseKey, &StrwMaxData, &StrwMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Type.
	{
		StrpMaxData.Initialize();
		StrpMinData.Initialize();
		GetStrpDgn(iElemNo, iLcaseType, iLcaseKey, &StrpMaxData, &StrpMinData);
	}
	// Save Forces and Moments by Position.
	int iPosStaNo = 0;
	int iPosTotNo = 0;
	if (iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
	else
	{
		iPosStaNo = iPosiNo;
		iPosTotNo = iPosiNo + 1;
	}

	// 하중조합에 포함된 Load Case가 Lctb의 Load Case인지 판정
	BOOL bIsLctbCase = FALSE;
	T_LCTB_K LctbK = 0;
	if (m_pDoc->m_pAttrCtrl->GetLctbKeyFromStldKey(iLcaseKey, LctbK))	bIsLctbCase = TRUE;

	BOOL bIsJUD = m_pDgnDataCtrl->IsJUD();

	// Change by ZINU.('04.03.13). Dead Type (D -> D,DC,DW).
	//if(CDBLib::IsStldDL(Lcase.LoadCaseType)) // Dead Load.
	if (IsDeadLoadType(Lcase.LoadCaseType, D_LCOM_STATIC, iLcaseKey))
	{
		for (int i = iPosStaNo; i < iPosTotNo; i++)
		{
			if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
			{
				// End Moments.
				if (!Get_BeamEndMoment(_T("D"), i, dFactor, TRUE, TRUE, iLcaseType, iLcaseKey, &StrbMaxData))	return FALSE;
				// Forces, Moments.								
				AddBeamForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrbMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
				AddBeamForceDof7(i, m_MbiD, m_TstD, m_TwpD, StrbMaxData, dFactor, dFactor, dFactor);
			}
			else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
			{
				// End Moments.
				if (!Get_WallEndMoment(_T("D"), i, dFactor, TRUE, iLcaseKey, &StrwMaxData))	return FALSE;
				// Forces, Moments. // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design. (m_dNTC08LcasN)
				AddWallForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrwMaxData, dFactor*m_dNTC08LcasN, dFactor, dFactor, dFactor, dFactor, dFactor);
			}
			else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
			{
				// End Moments.
				if (!Get_TrussEndMoment(_T("D"), i))	return FALSE;
				// Forces, Moments.
				m_FxxD[i] += StrtMaxData.dblForce[i / 4] * dFactor;
			}
			else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Plate.
			{
				// End Moments.
				if (!Get_PlateEndMoment(_T("D"), i))	return FALSE;
				if (IsRatingCode())
				{
					if (IsEndLCase())
					{
						if (i < 4)
						{
							AddPlatForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxML_UF, m_FyyML_UF, m_FxyML_UF, m_VxxML_UF, m_VyyML_UF, m_MxxML_UF, m_MyyML_UF, m_MxyML_UF, StrpMaxData,
								dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
					}
					else if (IsCheckStaticDL())
					{
						AddPlateDLForce(i, StrpMaxData, dFactor);
					}
					else
					{
						if (i < 4)
						{
							AddPlatForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxR_UF, m_FyyR_UF, m_FxyR_UF, m_VxxR_UF, m_VyyR_UF, m_MxxR_UF, m_MyyR_UF, m_MxyR_UF, StrpMaxData,
								dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
					}
				}
				else
				{
					// Forces, Moments. (4 node)
					if (i < 4)
					{
						AddPlatForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{
						AddPlatForceUF(i, m_FxxD_UF, m_FyyD_UF, m_FxyD_UF, m_VxxD_UF, m_VyyD_UF, m_MxxD_UF, m_MyyD_UF, m_MxyD_UF, StrpMaxData,
							dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
				}
			}
		}
	}
	//else if (CDBLib::IsStldLL(Lcase.LoadCaseType)) // Live Load.
	else if(IsLiveLoadType(Lcase.LoadCaseType, D_LCOM_STATIC))
	{
		// Change by ZINU.('00.05.11). L -> L,LR.
		// Change by ZINU.('03.02.11). For China Standard agree with Kye CJ.
		// 1. Column, Wall -> Beam, Column, Brace, Wall.
		// 2. Axial -> Axial, Moment, Shear.
		// Only Anlysis type is Static Load Case.
		double dReduP = 1.0;
		double dReduM = 1.0;
		double dReduV = 1.0;
		BOOL bIsFrme = (m_pDoc->m_pAttrCtrl->IsFrameType(m_iElemType) ? TRUE : FALSE);
		BOOL bIsWall = (m_iElemKind == D_MBTP_WALL && m_pDoc->m_pAttrCtrl->IsWall(m_iElemType) ? TRUE : FALSE);
		BOOL bIsPlate = (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType) ? TRUE : FALSE);
		if (bIsFrme || bIsWall || bIsPlate)
		{
			int iReduLcaseNum = m_arReduList.GetSize();
			if (iReduLcaseNum == 0)
			{
				dReduP = (m_bReduAxl ? m_dReduFactor : 1.0);
				dReduM = (m_bReduMom ? m_dReduFactor : 1.0);
				dReduV = (m_bReduShr ? m_dReduFactor : 1.0);
			}
			else
			{
				for (int i = 0; i < iReduLcaseNum; i++)
				{
					int iLcaseKey = m_arReduList.GetAt(i);
					if (Lcase.LoadCaseId != iLcaseKey)	continue;
					dReduP = (m_bReduAxl ? m_dReduFactor : 1.0);
					dReduM = (m_bReduMom ? m_dReduFactor : 1.0);
					dReduV = (m_bReduShr ? m_dReduFactor : 1.0);
				}
			}
		}


		for (int i = iPosStaNo; i < iPosTotNo; i++)
		{
			if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType)) // Beam Type.
			{
				// End Moments.
				if (!Get_BeamEndMoment(_T("L"), i, dFactor*dReduM, TRUE, TRUE, iLcaseType, iLcaseKey, &StrbMaxData))	return FALSE;
				// Forces, Moments.
				AddBeamForce(i, m_FxxL, m_FyyL, m_FzzL, m_BMxL, m_BMyL, m_BMzL, StrbMaxData,
					dFactor*dReduP, dFactor*dReduV, dFactor*dReduV, dFactor*dReduM, dFactor*dReduM, dFactor*dReduM);
				AddBeamForceDof7(i, m_MbiL, m_TstL, m_TwpL, StrbMaxData, dFactor*dReduM, dFactor*dReduM, dFactor*dReduM);
			}
			else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType)) // Wall Type.
			{
				// End Moments.
				if (!Get_WallEndMoment(_T("L"), i, dFactor*dReduM, TRUE, iLcaseKey, &StrwMaxData))	return FALSE;
				// Forces, Moments. // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.(m_dNTC08LcasN)
				AddWallForce(i, m_FxxL, m_FyyL, m_FzzL, m_BMxL, m_BMyL, m_BMzL, StrwMaxData,
					dFactor*dReduP*m_dNTC08LcasN, dFactor*dReduV, dFactor*dReduV, dFactor*dReduM, dFactor*dReduM, dFactor*dReduM);
			}
			else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType)) // Truss Type.
			{
				// End Moments.
				if (!Get_TrussEndMoment(_T("L"), i))	return FALSE;
				// Forces, Moments.
				m_FxxL[i] += StrtMaxData.dblForce[i / 4] * dFactor * dReduP;
			}
			else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Plate.
			{
				// End Moments.
				if (!Get_PlateEndMoment(_T("L"), i))	return FALSE;
				if (IsRatingCode())
				{
					if (IsEndLCase())
					{
						// Forces, Moments. (4 node)
						if (i < 4)
						{
							AddPlatForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, StrpMaxData,
								dFactor*dReduP, dFactor*dReduP, dFactor*dReduV, dFactor*dReduM, dFactor*dReduM, dFactor*dReduM);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxML_UF, m_FyyML_UF, m_FxyML_UF, m_VxxML_UF, m_VyyML_UF, m_MxxML_UF, m_MyyML_UF, m_MxyML_UF, StrpMaxData,
								dFactor*dReduP, dFactor*dReduP, dFactor*dReduV, dFactor*dReduV, dFactor*dReduV, dFactor*dReduM, dFactor*dReduM, dFactor*dReduM);
						}
					}
					else if (IsCheckStaticDL())
					{
						AddPlateDLForce(i, StrpMaxData, dFactor, dReduP, dReduV, dReduM);
					}
					else
					{
						// Forces, Moments. (4 node)
						if (i < 4)
						{
							AddPlatForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, StrpMaxData,
								dFactor*dReduP, dFactor*dReduP, dFactor*dReduV, dFactor*dReduM, dFactor*dReduM, dFactor*dReduM);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxR_UF, m_FyyR_UF, m_FxyR_UF, m_VxxR_UF, m_VyyR_UF, m_MxxR_UF, m_MyyR_UF, m_MxyR_UF, StrpMaxData,
								dFactor*dReduP, dFactor*dReduP, dFactor*dReduV, dFactor*dReduV, dFactor*dReduV, dFactor*dReduM, dFactor*dReduM, dFactor*dReduM);
						}
					}

				}
				else
				{
					// Forces, Moments. (4 node)
					if (i < 4)
					{
						AddPlatForce(i, m_FxxL, m_FyyL, m_FzzL, m_BMxL, m_BMyL, m_BMzL, StrpMaxData,
							dFactor*dReduP, dFactor*dReduP, dFactor*dReduV, dFactor*dReduM, dFactor*dReduM, dFactor*dReduM);
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{
						AddPlatForceUF(i, m_FxxL_UF, m_FyyL_UF, m_FxyL_UF, m_VxxL_UF, m_VyyL_UF, m_MxxL_UF, m_MyyL_UF, m_MxyL_UF, StrpMaxData,
							dFactor*dReduP, dFactor*dReduP, dFactor*dReduV, dFactor*dReduV, dFactor*dReduV, dFactor*dReduM, dFactor*dReduM, dFactor*dReduM);
					}
				}
			}
		}
	}
	else if (m_bExistLctb_CVL && bIsLctbCase && Lcase.LoadCaseType == _T("USER"))  // Civil의 Lctb에 있는 User type만 nonsway로 한다.
	{                                                                       // Static Load Case의 User type은 sway로 한다.
		for (int i = iPosStaNo; i < iPosTotNo; i++)
		{
			if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
			{
				// End Moments.
				if (!Get_BeamEndMoment(_T("USER"), i, dFactor, TRUE, TRUE, iLcaseType, iLcaseKey, &StrbMaxData))	return FALSE;
				// Forces, Moments.				
				AddBeamForce(i, m_FxxLU, m_FyyLU, m_FzzLU, m_BMxLU, m_BMyLU, m_BMzLU, StrbMaxData,
					dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
			}
			else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
			{
				// End Moments.
				if (!Get_WallEndMoment(_T("USER"), i, dFactor, TRUE, iLcaseKey, &StrwMaxData))	return FALSE;
				// Forces, Moments. // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design. (m_dNTC08LcasN)
				AddWallForce(i, m_FxxLU, m_FyyLU, m_FzzLU, m_BMxLU, m_BMyLU, m_BMzLU, StrwMaxData,
					dFactor*m_dNTC08LcasN, dFactor, dFactor, dFactor, dFactor, dFactor);
			}
			else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
			{
				// End Moments.
				if (!Get_TrussEndMoment(_T("USER"), i))	return FALSE;
				// Forces, Moments.
				m_FxxLU[i] += StrtMaxData.dblForce[i / 4] * dFactor;
			}
			else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Plate.
			{
				// End Moments.
				if (!Get_PlateEndMoment(_T("USER"), i))	return FALSE;
				// Forces, Moments. (4 node)
				if (i < 4)
				{
					AddPlatForce(i, m_FxxLU, m_FyyLU, m_FzzLU, m_BMxLU, m_BMyLU, m_BMzLU, StrpMaxData,
						dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
				}
				// Unit Forces (cent + 4 node)
				if (i < 5)
				{
					AddPlatForceUF(i, m_FxxLU_UF, m_FyyLU_UF, m_FxyLU_UF, m_VxxLU_UF, m_VyyLU_UF, m_MxxLU_UF, m_MyyLU_UF, m_MxyLU_UF, StrpMaxData,
						dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
				}
			}
		}
	}
	else // etc. Loadcase include _T("E").
	{
		BOOL bLcaseEQ = CDBLib::IsStldEL(Lcase.LoadCaseType);
		Get_EqFactorForLcase(bLcaseEQ);
		// Change by ZINU.('05.06.16). Move wg_cmd/CLoadCombCtrl.
/*
		// Add by ZINU.('05.05.12). Special Load Combination Factor.
		if(m_bExistEL)	dFactor *= Get_SpecialLcomOmega(iElemNo, iLcaseKey, D_LCOM_STATIC);
*/
// Add by ZINU.('06.09.08). NO:2340. Consider Factor for Lcase(RS) at GB50010-02 (전중비, 0.2Q0, 박약층).
		_DGN_SHRR_D ShrrD;
		ShrrD.Initialize();
		if (bLcaseEQ)	Get_Wght_02Q0_Weak_GB(iElemNo, iLcaseKey, ShrrD);

		/// 지진하중 조건인 경우, 추가로 고려해야 하는 계수들 (ShrrD : 중국기준인 경우, 전중비, 0.2Q0, 박약층에 의한 확대계수)        

		double dRFx = 1.0, dRFy = 1.0, dRFz = 1.0, dRMx = 1.0, dRMy = 1.0, dRMz = 1.0;
		GetRSFactor(DconD, iElemNo, iLcaseType, Lcase.LoadCaseType, dRFx, dRFy, dRFz, dRMx, dRMy, dRMz);

		for (int i = iPosStaNo; i < iPosTotNo; i++)
		{
			if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType)) // Beam Type.
			{
				double dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T;
				dExFactor_P = dExFactor_V = dExFactor_M = dExFactor_T = dFactor;
				if (bIsJUD) { GetExpandEqFactor(iElemNo, i, Lcase.LoadCaseType, dFactor, dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T); } // PMS:4215 - 일본2차설계시 1차설계 지진하중 할증
				// End Moments.
				if (!Get_BeamEndMoment(_T("R"), i, dExFactor_M*m_dSueqLcasM*m_dCodeLcasM, TRUE, TRUE, iLcaseType, iLcaseKey, &StrbMaxData))	return FALSE;
				// Forces, Moments.
				if (m_bExistLctb)
				{
					// Seungjun-20121231 : iGen 비선형 부재의 경우도 할증계수 적용
					dExFactor_P = bIsJUD ? fabs(dExFactor_P) : 1.0;
					dExFactor_V = bIsJUD ? fabs(dExFactor_V) : 1.0;
					dExFactor_M = bIsJUD ? fabs(dExFactor_M) : 1.0;
					dExFactor_T = bIsJUD ? fabs(dExFactor_T) : 1.0;

					_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
					if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
					m_FxxR[i] += LctbPos.dRemnFxx * dExFactor_P * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx * dRFx;
					m_FyyR[i] += LctbPos.dRemnFyy * dExFactor_V * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy * ShrrD.ShrrBase[2].dRatVy * dRFy;
					m_FzzR[i] += LctbPos.dRemnFzz * dExFactor_V * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz * dRFz;
					m_BMxR[i] += LctbPos.dRemnBMx * dExFactor_T * dRMx;
					m_BMyR[i] += LctbPos.dRemnBMy * dExFactor_M * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy * dRMy;
					m_BMzR[i] += LctbPos.dRemnBMz * dExFactor_M * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMz * ShrrD.ShrrBase[1].dRatMz * ShrrD.ShrrBase[2].dRatMz * dRMz;
					m_MbiR[i] += LctbPos.dRemnMbi; //!!!
					m_TstR[i] += LctbPos.dRemnTst; //!!!
					m_TwpR[i] += LctbPos.dRemnTwp; //!!!
					// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
					m_FyyOrgR[i] += LctbPos.dRemnFyy * dExFactor_V * m_dSueqLcasV;
					m_FzzOrgR[i] += LctbPos.dRemnFzz * dExFactor_V * m_dSueqLcasV;
				}
				else
				{
					double dFactFx = dExFactor_P * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx * dRFx;
					double dFactFy = dExFactor_V * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy * ShrrD.ShrrBase[2].dRatVy * dRFy;
					double dFactFz = dExFactor_V * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz * dRFz;
					double dFactMx = dExFactor_T * dRMx;
					double dFactMy = dExFactor_M * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy * dRMy;
					double dFactMz = dExFactor_M * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMz * ShrrD.ShrrBase[1].dRatMz * ShrrD.ShrrBase[2].dRatMz * dRMz;
					AddBeamForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, StrbMaxData, dFactFx, dFactFy, dFactFz, dFactMx, dFactMy, dFactMz);

					AddBeamForceDof7(i, m_MbiR, m_TstR, m_TwpR, StrbMaxData); ///!!! 자유도별 확대계수가 없음.. 추가 확인 필요.

					// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
					int nPart = 0;
					int nKind = 0;
					int nKind7 = 0;
					GetAnalForceComponentIndex(i, nPart, nKind, nKind7);
					m_FyyOrgR[i] += StrbMaxData.dblForce[nPart][nKind + 1] * dExFactor_V * m_dSueqLcasV;
					m_FzzOrgR[i] += StrbMaxData.dblForce[nPart][nKind + 2] * dExFactor_V * m_dSueqLcasV;
				}
			}
			else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType)) // Wall Type.
			{
				// Modify by GAY. PMS:XXXX. ('12.05.09). 중국 요청사항 : 전단벽 내력을 조정할 때는 박약층조정계수와 전중비조정계수는 전단력뿐만아니라 축력과 모멘트도 조절해야 함.
				// End Moments.
				if (!Get_WallEndMoment(_T("R"), i, dFactor*m_dSueqLcasM*m_dCodeLcasM, TRUE, iLcaseKey, &StrwMaxData))	return FALSE;
				// Forces, Moments.
				if (m_bExistLctb)
				{
					_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
					if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
					m_FxxR[i] += LctbPos.dRemnFxx * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx * m_dNTC08LcasN;  // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.
					m_FyyR[i] += LctbPos.dRemnFyy * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy;
					m_FzzR[i] += LctbPos.dRemnFzz * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz;
					m_BMxR[i] += LctbPos.dRemnBMx;
					m_BMyR[i] += LctbPos.dRemnBMy * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy;
					m_BMzR[i] += LctbPos.dRemnBMz * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMz * ShrrD.ShrrBase[1].dRatMz;
					// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
					m_FyyOrgR[i] += LctbPos.dRemnFyy * m_dSueqLcasV;
					m_FzzOrgR[i] += LctbPos.dRemnFzz * m_dSueqLcasV;
				}
				else
				{
					double dFactFx = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx * m_dNTC08LcasN;  // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.
					double dFactFy = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy;
					double dFactFz = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz;
					double dFactMx = dFactor;
					double dFactMy = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy;
					double dFactMz = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMz * ShrrD.ShrrBase[1].dRatMz;
					AddWallForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, StrwMaxData, dFactFx, dFactFy, dFactFz, dFactMx, dFactMy, dFactMz);
					// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
					int iPart = (i < 4 ? 1 : 0);	// I=1, J=0.
					m_FyyOrgR[i] += StrwMaxData.dblForce[iPart][1] * dFactor * m_dSueqLcasV;
					m_FzzOrgR[i] += StrwMaxData.dblForce[iPart][2] * dFactor * m_dSueqLcasV;
				}
			}
			else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType)) // Truss Type.
			{
				// End Moments.
				if (!Get_TrussEndMoment(_T("R"), i))	return FALSE;
				// Forces, Moments.
				if (m_bExistLctb)
				{
					_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
					if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
					m_FxxR[i] += LctbPos.dRemnFxx * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
				}
				else
				{
					m_FxxR[i] += StrtMaxData.dblForce[i / 4] * dFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
				}
			}
			else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Plate.
			{
				// End Moments.
				if (!Get_PlateEndMoment(_T("R"), i))	return FALSE;
				if (m_bExistLctb)
				{
					_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
					if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
					if (i < 4)
					{
						//component : 0:Fx, 1:Fy, 2:Fz, 3:Mx, 4:My, 5:Mz
						m_FxxR[i] += LctbPos.dRemnFxx * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
						m_FyyR[i] += LctbPos.dRemnFyy * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
						m_FzzR[i] += LctbPos.dRemnFzz * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
						m_BMxR[i] += LctbPos.dRemnBMx * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
						m_BMyR[i] += LctbPos.dRemnBMy * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
						m_BMzR[i] += LctbPos.dRemnBMz;
						// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
						m_FzzOrgR[i] += LctbPos.dRemnFzz * m_dSueqLcasV;
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{//component : 14:Fxx, 16:Fyy, 17:Fxy, 20:Mxx, 21:Myy, 26:Vxx,  27:Vyy
						m_FxxR_UF[i] += LctbPos.dRemnFxxUF * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
						m_FyyR_UF[i] += LctbPos.dRemnFyyUF * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
						m_FxyR_UF[i] += LctbPos.dRemnFxyUF * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy * ShrrD.ShrrBase[2].dRatVy;
						m_VxxR_UF[i] += LctbPos.dRemnVxxUF * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
						m_VyyR_UF[i] += LctbPos.dRemnVyyUF * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
						m_MxxR_UF[i] += LctbPos.dRemnMxxUF * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
						m_MyyR_UF[i] += LctbPos.dRemnMyyUF * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
						m_MxyR_UF[i] += LctbPos.dRemnMxyUF;
						// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
						m_FxyUFOrgR[i] += LctbPos.dRemnFxyUF * m_dSueqLcasV;
						m_VxxUFOrgR[i] += LctbPos.dRemnVxxUF * m_dSueqLcasV;
						m_VyyUFOrgR[i] += LctbPos.dRemnVyyUF * m_dSueqLcasV;
					}
				}
				else
				{
					if (IsRatingCode() && (IsEndLCase() || IsCheckStaticDL()))
					{
						if (IsEndLCase())
						{
							if (i < 4)
							{
								double dFactFx = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
								double dFactFy = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
								double dFactFz = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
								double dFactMx = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
								double dFactMy = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
								double dFactMz = dFactor;
								AddPlatForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, StrpMaxData,
									dFactFx, dFactFy, dFactFz, dFactMx, dFactMy, dFactMz);
								// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
								m_FzzOrgR[i] += StrpMaxData.dblForce[i][2] * dFactor * m_dSueqLcasV;
							}
							// Unit Forces (cent + 4 node)
							if (i < 5)
							{
								double dFactFxx = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
								double dFactFyy = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
								double dFactFxy = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy * ShrrD.ShrrBase[2].dRatVy;
								double dFactVxx = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
								double dFactVyy = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
								double dFactMxx = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
								double dFactMyy = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
								double dFactMxy = dFactor;
								AddPlatForceUF(i, m_FxxML_UF, m_FyyML_UF, m_FxyML_UF, m_VxxML_UF, m_VyyML_UF, m_MxxML_UF, m_MyyML_UF, m_MxyML_UF, StrpMaxData,
									dFactFxx, dFactFyy, dFactFxy, dFactVxx, dFactVyy, dFactMxx, dFactMyy, dFactMxy);
								// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
								m_FxyUFOrgR[i] += StrpMaxData.dblStress[i][16] * dFactor * m_dSueqLcasV;
								m_VxxUFOrgR[i] += StrpMaxData.dblStress[i][26] * dFactor * m_dSueqLcasV;
								m_VyyUFOrgR[i] += StrpMaxData.dblStress[i][27] * dFactor * m_dSueqLcasV;
							}
						}
						else if (IsCheckStaticDL())
						{
							if (i < 4)
							{
								double dFactFx = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
								double dFactFy = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
								double dFactFz = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
								double dFactMx = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
								double dFactMy = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
								double dFactMz = dFactor;
								AddPlatForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrpMaxData, dFactFx, dFactFy, dFactFz, dFactMx, dFactMy, dFactMz);
								// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
								m_FzzOrgR[i] += StrpMaxData.dblForce[i][2] * dFactor * m_dSueqLcasV;
							}
							// Unit Forces (cent + 4 node)
							if (i < 5)
							{
								//component : 14:Fxx, 16:Fyy, 17:Fxy, 20:Mxx, 21:Myy, 26:Vxx,  27:Vyy
								double dFactFxx = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
								double dFactFyy = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
								double dFactFxy = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy * ShrrD.ShrrBase[2].dRatVy;
								double dFactVxx = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
								double dFactVyy = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
								double dFactMxx = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
								double dFactMyy = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
								double dFactMxy = dFactor;
								AddPlatForceUF(i, m_FxxD_UF, m_FyyD_UF, m_FxyD_UF, m_VxxD_UF, m_VyyD_UF, m_MxxD_UF, m_MyyD_UF, m_MxyD_UF, StrpMaxData,
									dFactFxx, dFactFyy, dFactFxy, dFactVxx, dFactVyy, dFactMxx, dFactMyy, dFactMxy);
								// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
								m_FxyUFOrgR[i] += StrpMaxData.dblStress[i][16] * dFactor * m_dSueqLcasV;
								m_VxxUFOrgR[i] += StrpMaxData.dblStress[i][26] * dFactor * m_dSueqLcasV;
								m_VyyUFOrgR[i] += StrpMaxData.dblStress[i][27] * dFactor * m_dSueqLcasV;
							}
						}
					}
					else
					{
						if (i < 4)
						{
							double dFactFx = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
							double dFactFy = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
							double dFactFz = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
							double dFactMx = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
							double dFactMy = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
							double dFactMz = dFactor;
							AddPlatForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, StrpMaxData,
								dFactFx, dFactFy, dFactFz, dFactMx, dFactMy, dFactMz);
							// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
							m_FzzOrgR[i] += StrpMaxData.dblForce[i][2] * dFactor * m_dSueqLcasV;
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							double dFactFxx = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
							double dFactFyy = dFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
							double dFactFxy = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy * ShrrD.ShrrBase[2].dRatVy;
							double dFactVxx = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
							double dFactVyy = dFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
							double dFactMxx = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
							double dFactMyy = dFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
							double dFactMxy = dFactor;
							AddPlatForceUF(i, m_FxxR_UF, m_FyyR_UF, m_FxyR_UF, m_VxxR_UF, m_VyyR_UF, m_MxxR_UF, m_MyyR_UF, m_MxyR_UF, StrpMaxData,
								dFactFxx, dFactFyy, dFactFxy, dFactVxx, dFactVyy, dFactMxx, dFactMyy, dFactMxy);
							// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
							m_FxyUFOrgR[i] += StrpMaxData.dblStress[i][16] * dFactor * m_dSueqLcasV;
							m_VxxUFOrgR[i] += StrpMaxData.dblStress[i][26] * dFactor * m_dSueqLcasV;
							m_VyyUFOrgR[i] += StrpMaxData.dblStress[i][27] * dFactor * m_dSueqLcasV;
						}
					}
				}
			}
		}
	}

	// Add by ZINU.('03.01.09). Factored and Unfactored Forces by Self Weight.
	if (iLcaseKey == m_iStldKeySelf)
	{
		for (int i = iPosStaNo; i < iPosTotNo; i++)
		{
			if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
			{
				int iPart = (i < 4 ? i : i - 1);
				int iKind = (i < 4 ? 0 : 6);
				m_dVdu[i] = StrbMaxData.dblForce[iPart][iKind + 2] * 1.0;
				m_dMdu[i] = StrbMaxData.dblForce[iPart][iKind + 4] * 1.0;
				m_dVdf[i] = StrbMaxData.dblForce[iPart][iKind + 2] * dFactor;
				m_dMdf[i] = StrbMaxData.dblForce[iPart][iKind + 4] * dFactor;
			}
			else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
			{
				int iPart = (i < 4 ? 1 : 0);	// I=1, J=0.
				m_dVdu[i] = StrwMaxData.dblForce[iPart][2] * 1.0;
				m_dMdu[i] = StrwMaxData.dblForce[iPart][4] * 1.0;
				m_dVdf[i] = StrwMaxData.dblForce[iPart][2] * dFactor;
				m_dMdf[i] = StrwMaxData.dblForce[iPart][4] * dFactor;
			}
			else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
			{
				m_dVdu[i] = 0.0;
				m_dMdu[i] = 0.0;
				m_dVdf[i] = 0.0;
				m_dMdf[i] = 0.0;
			}
			else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Plate.
			{
				m_dVdu[i] = StrpMaxData.dblForce[i][2] * 1.0;
				m_dMdu[i] = StrpMaxData.dblForce[i][4] * 1.0;
				m_dVdf[i] = StrpMaxData.dblForce[i][2] * dFactor;
				m_dMdf[i] = StrpMaxData.dblForce[i][4] * dFactor;
			}
		}
	}

	return TRUE;
}

BOOL CDgnForceCtrl::Get_SpectrumForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor, int iAnalType)
{
	//*/PROFILING(_T("Get_SpectrumForce"));
	if (iLcaseKey == 0) { ASSERT(0); return FALSE; }
	if (dFactor == 0.0) { ASSERT(0); return FALSE; }
	if (iAnalType == 0) { ASSERT(0); return FALSE; }
	// Add by ZINU.('03.11.26). Distinguish D_LOADCASE_ECCENSPEC from D_LOADCASE_SPECTRUM.
	int iLcaseType = 0;
	switch (iAnalType)
	{
	case D_LCOM_SPECTRUM: { iLcaseType = D_LOADCASE_SPECTRUM;     } break;
	case D_LCOM_SPECTRUM_ND: { iLcaseType = D_LOADCASE_SPECTRUM_ND;  } break;
	case D_LCOM_ECCENSPEC: { iLcaseType = D_LOADCASE_ECCENSPEC;    } break;
	case D_LCOM_ECCENSPEC_ND: { iLcaseType = D_LOADCASE_ECCENSPEC_ND; } break;
	default: ASSERT(0); break;
	}

	T_DCON_D DconD; DconD.Initialize();
	m_pDgnDataCtrl->Get_DgnConDcon(DconD);

	double dRFx=1.0, dRFy=1.0, dRFz=1.0, dRMx=1.0, dRMy=1.0, dRMz=1.0;
	CString strLcaseType = _T("");
	GetRSFactor(DconD, iElemNo, iLcaseType, strLcaseType, dRFx, dRFy, dRFz, dRMx, dRMy, dRMz);
	double adRMFactor[6] = { dRFx, dRFy, dRFz, dRMx, dRMy, dRMz };

	// Add by ZINU.('03.01.11).	
	Get_EqFactorForLcase(TRUE);
	// Change by ZINU('00.5.18).
	// Get Scale Up Factor of Load Case.
	double dScaleUpFactor = 1.0;	// Included at Load Case Factor.

	// Change by ZINU.('02.11.26).
	T_STRB_D  StrbMaxData, StrbMinData;
	T_STRT_D  StrtMaxData, StrtMinData;
	T_STRW_D  StrwMaxData, StrwMinData;
	T_STRP_DL StrpMaxData, StrpMinData;
	if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
	{
		StrbMaxData.Initialize();
		StrbMinData.Initialize();
		GetStrbDgn(iElemNo, iLcaseType, iLcaseKey, &StrbMaxData, &StrbMinData);

		// Add/Modify by Seungjun PMS.4511  '20120821 for JFD
		if (m_pDoc->EnableJFD() && m_GroundBeamForce.bCheck)
		{
			_ICS_LCAS_K IcsKey;
			IcsKey.Key.iType = (iAnalType == D_LCOM_SPECTRUM || iAnalType == D_LCOM_SPECTRUM_ND) ? 1 : 2;
			IcsKey.Key.LcasK = iLcaseKey;
			int nLcase = m_GroundBeamForce.FiffBase[0].GetSize();
			for (int i = 0; i < nLcase; i++)
			{
				T_FIFF_BASE FiffBaseD;
				FiffBaseD = m_GroundBeamForce.FiffBase[0].GetAt(i);
				_ICS_LCAS_K IcsKey2;
				IcsKey2.Key.iType = FiffBaseD.LcasType;
				IcsKey2.Key.LcasK = FiffBaseD.LcasK;
				if (FiffBaseD.bCheck && IcsKey2.KeyMap == IcsKey.KeyMap)
				{
					for (int j = 0; j < 4; j++)
					{
						int ni = 0, nj = 0;
						if (j == 0) { ni = 0; nj = 1; }
						else if (j == 1) { ni = 1; nj = 2; }
						else if (j == 2) { ni = 2; nj = 3; }
						else if (j == 2) { ni = 3; nj = 4; }

						T_FIFF_BASE FiffBaseDi, FiffBaseDj;
						FiffBaseDi = m_GroundBeamForce.FiffBase[ni].GetAt(i);
						FiffBaseDj = m_GroundBeamForce.FiffBase[nj].GetAt(i);

						StrbMaxData.dblForce[j][2] += FiffBaseDi.dFzz;
						StrbMaxData.dblForce[j][4] += FiffBaseDi.dMuy;
						StrbMaxData.dblForce[j][8] += FiffBaseDj.dFzz;
						StrbMaxData.dblForce[j][10] += FiffBaseDj.dMuy;
					}
				}
			}
		}
	}
	else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
	{
		StrtMaxData.Initialize();
		StrtMinData.Initialize();
		GetStrtDgn(iElemNo, iLcaseType, iLcaseKey, &StrtMaxData, &StrtMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
	{
		StrwMaxData.Initialize();
		StrwMinData.Initialize();
		GetStrwDgn(iElemNo, iLcaseType, iLcaseKey, &StrwMaxData, &StrwMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Type.
	{
		StrpMaxData.Initialize();
		StrpMinData.Initialize();
		GetStrpDgn(iElemNo, iLcaseType, iLcaseKey, &StrpMaxData, &StrpMinData);
	}

	// Save Forces and Moments by Position.
	int iPosStaNo = 0;
	int iPosTotNo = 0;
	if (iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
	else
	{
		iPosStaNo = iPosiNo;
		iPosTotNo = iPosiNo + 1;
	}
	// Change by ZINU.('05.06.16). Move wg_cmd/CLoadCombCtrl.
/*
	// Add by ZINU.('05.05.12). Special Load Combination Factor.
	if(m_bExistEL)	dFactor *= Get_SpecialLcomOmega(iElemNo, iLcaseKey, iAnalType);
*/
// Add by ZINU.('06.09.08). NO:2340. Consider Factor for Lcase(RS) at GB50010-02 (전중비, 0.2Q0, 박약층).
	_DGN_SHRR_D ShrrD;
	ShrrD.Initialize();
	Get_Wght_02Q0_Weak_GB(iElemNo, iLcaseKey, ShrrD);

	T_STLD_D Lcase;
	Lcase.Initialize();
	m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, Lcase);
	BOOL bIsJUD = m_pDgnDataCtrl->IsJUD();

	for (int i = iPosStaNo; i < iPosTotNo; i++)
	{
		if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType)) // Beam Type.
		{
			double dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T;
			dExFactor_P = dExFactor_V = dExFactor_M = dExFactor_T = dFactor;
			if (bIsJUD) { GetExpandEqFactor(iElemNo, i, Lcase.LoadCaseType, dFactor, dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T); } // PMS:4215 - 일본2차설계시 1차설계 지진하중 할증
			// End Moments.
			if (!Get_BeamEndMoment(_T("R"), i, dExFactor_M*dScaleUpFactor*m_dSueqLcasM*m_dCodeLcasM, TRUE, TRUE, iLcaseType, iLcaseKey, &StrbMaxData, adRMFactor))	return FALSE;
			// Forces, Moments.
			if (m_bExistLctb)
			{
				// Seungjun-20121231 : iGen 비선형 부재의 경우도 할증계수 적용
				dExFactor_P = bIsJUD ? fabs(dExFactor_P) : 1.0;
				dExFactor_V = bIsJUD ? fabs(dExFactor_V) : 1.0;
				dExFactor_M = bIsJUD ? fabs(dExFactor_M) : 1.0;
				dExFactor_T = bIsJUD ? fabs(dExFactor_T) : 1.0;

				_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
				if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
				double dFxx = LctbPos.dRemnFxx * dExFactor_P * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx * dRFx;
				double dFyy = LctbPos.dRemnFyy * dExFactor_V * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy * ShrrD.ShrrBase[2].dRatVy * dRFy;
				double dFzz = LctbPos.dRemnFzz * dExFactor_V * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz * dRFz;
				double dBMx = LctbPos.dRemnBMx * dExFactor_T * dScaleUpFactor * dRMx;
				double dBMy = LctbPos.dRemnBMy * dExFactor_M * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy * dRMy;
				double dBMz = LctbPos.dRemnBMz * dExFactor_M * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMz * ShrrD.ShrrBase[1].dRatMz * ShrrD.ShrrBase[2].dRatMz * dRMz;
				// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
				double dMbi = LctbPos.dRemnMbi * dScaleUpFactor;  //!!! 지진부재력 조정계수 고려 범위 확인 요망.
				double dTst = LctbPos.dRemnTst * dScaleUpFactor; //!!!
				double dTwp = LctbPos.dRemnTwp * dScaleUpFactor; //!!!
				// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
				double dFyyOrg = LctbPos.dRemnFyy * dExFactor_V * dScaleUpFactor * m_dSueqLcasV;
				double dFzzOrg = LctbPos.dRemnFzz * dExFactor_V * dScaleUpFactor * m_dSueqLcasV;
				if (IsDeadLoadType(_T(""), iAnalType, iLcaseKey))
				{
					AddDeadSum(i, dFxx, dFyy, dFzz, dBMx, dBMy, dBMz, dMbi, dTst, dTwp);
				}
				else
				{
					AddRemnantSum(i, dFxx, dFyy, dFzz, dBMx, dBMy, dBMz, dMbi, dTst, dTwp, dFyyOrg, dFzzOrg);
				}
			}
			else
			{
				double dFactFx = dExFactor_P * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx * dRFx;
				double dFactFy = dExFactor_V * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy * ShrrD.ShrrBase[2].dRatVy * dRFy;
				double dFactFz = dExFactor_V * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz * dRFz;
				double dFactMx = dExFactor_T * dScaleUpFactor * dRMx;
				double dFactMy = dExFactor_M * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy * dRMy;
				double dFactMz = dExFactor_M * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMz * ShrrD.ShrrBase[1].dRatMz * ShrrD.ShrrBase[2].dRatMz * dRMz;
				if (IsDeadLoadType(_T(""), iAnalType, iLcaseKey))
				{
					AddBeamForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrbMaxData, dFactFx, dFactFy, dFactFz, dFactMx, dFactMy, dFactMz);
					AddBeamForceDof7(i, m_MbiD, m_TstD, m_TwpD, StrbMaxData, dScaleUpFactor, dScaleUpFactor, dScaleUpFactor); //!!! 지진부재력 조정계수 고려 범위 확인 요망.				
				}
				else
				{
					AddBeamForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, StrbMaxData, dFactFx, dFactFy, dFactFz, dFactMx, dFactMy, dFactMz);
					// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
					AddBeamForceDof7(i, m_MbiR, m_TstR, m_TwpR, StrbMaxData, dScaleUpFactor, dScaleUpFactor, dScaleUpFactor); //!!! 지진부재력 조정계수 고려 범위 확인 요망.				
					// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
					int iPart = 0;
					int iKind = 0;
					int iKind7 = 0;
					GetAnalForceComponentIndex(i, iPart, iKind, iKind7);
					m_FyyOrgR[i] += StrbMaxData.dblForce[iPart][iKind + 1] * dExFactor_V * dScaleUpFactor * m_dSueqLcasV;
					m_FzzOrgR[i] += StrbMaxData.dblForce[iPart][iKind + 2] * dExFactor_V * dScaleUpFactor * m_dSueqLcasV;
				}
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType)) // Wall Type.
		{
			// Modify by GAY. PMS:XXXX. ('12.05.09). 중국 요청사항 : 전단벽 내력을 조정할 때는 박약층조정계수와 전중비조정계수는 전단력뿐만아니라 축력과 모멘트도 조절해야 함.
			// End Moments.
			if (!Get_WallEndMoment(_T("R"), i, dFactor*dScaleUpFactor*m_dSueqLcasM*m_dCodeLcasM, TRUE, iLcaseKey, &StrwMaxData))	return FALSE;
			// Forces, Moments.
			if (m_bExistLctb)
			{
				_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
				if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
				m_FxxR[i] += LctbPos.dRemnFxx * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx * m_dNTC08LcasN;  // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.
				m_FyyR[i] += LctbPos.dRemnFyy * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy;
				m_FzzR[i] += LctbPos.dRemnFzz * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz;
				m_BMxR[i] += LctbPos.dRemnBMx * dScaleUpFactor;
				m_BMyR[i] += LctbPos.dRemnBMy * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy;
				m_BMzR[i] += LctbPos.dRemnBMz * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMz * ShrrD.ShrrBase[1].dRatMz;
				// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
				m_FyyOrgR[i] += LctbPos.dRemnFyy * dScaleUpFactor * m_dSueqLcasV;
				m_FzzOrgR[i] += LctbPos.dRemnFzz * dScaleUpFactor * m_dSueqLcasV;
			}
			else
			{
				double dFactFx = dFactor * dScaleUpFactor * m_dSueqLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx * m_dNTC08LcasN;  // MQC:3190-Seungjun-20100225 코딩누락. m_dNTC08LcasN
				double dFactFy = dFactor * dScaleUpFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy;
				double dFactFz = dFactor * dScaleUpFactor * m_dSueqLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz;
				double dFactMx = dFactor * dScaleUpFactor;
				double dFactMy = dFactor * dScaleUpFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy;
				double dFactMz = dFactor * dScaleUpFactor * m_dSueqLcasM * ShrrD.ShrrBase[0].dRatMz * ShrrD.ShrrBase[1].dRatMz;
				AddWallForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, StrwMaxData, dFactFx, dFactFy, dFactFz, dFactMx, dFactMy, dFactMz);
				// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
				int iPart = (i < 4 ? 1 : 0);	// I=1, J=0.
				m_FyyOrgR[i] += StrwMaxData.dblForce[iPart][1] * dFactor * dScaleUpFactor * m_dSueqLcasV;
				m_FzzOrgR[i] += StrwMaxData.dblForce[iPart][2] * dFactor * dScaleUpFactor * m_dSueqLcasV;
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType)) // Truss Type.
		{
			// End Moments.
			if (!Get_TrussEndMoment(_T("R"), i))	return FALSE;
			// Forces, Moments.
			if (m_bExistLctb)
			{
				_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
				if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos)) {}//	ASSERT(0);
				m_FxxR[i] += LctbPos.dRemnFxx * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
				m_FyyR[i] += 0.0;
				m_FzzR[i] += 0.0;
				m_BMxR[i] += 0.0;
				m_BMyR[i] += 0.0;
				m_BMzR[i] += 0.0;
			}
			else
			{
				m_FxxR[i] += StrtMaxData.dblForce[i / 4] * dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
				m_FyyR[i] += 0.0;
				m_FzzR[i] += 0.0;
				m_BMxR[i] += 0.0;
				m_BMyR[i] += 0.0;
				m_BMzR[i] += 0.0;
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Plate.
		{
			// End Moments.
			if (!Get_PlateEndMoment(_T("R"), i))	return FALSE;
			// Forces, Moments.
			if (m_bExistLctb)
			{
				_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
				if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
				if (i < 4)
				{//component : 0:Fx, 1:Fy, 2:Fz, 3:Mx, 4:My, 5:Mz
					m_FxxR[i] += LctbPos.dRemnFxx * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
					m_FyyR[i] += LctbPos.dRemnFyy * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
					m_FzzR[i] += LctbPos.dRemnFzz * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
					m_BMxR[i] += LctbPos.dRemnBMx * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
					m_BMyR[i] += LctbPos.dRemnBMy * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
					m_BMzR[i] += LctbPos.dRemnBMz * dScaleUpFactor;
					// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
					m_FzzOrgR[i] += LctbPos.dRemnFzz * dScaleUpFactor * m_dSueqLcasV;
				}
				// Unit Forces (cent + 4 node)
				if (i < 5)
				{//component : 14:Fxx, 16:Fyy, 17:Fxy, 20:Mxx, 21:Myy, 26:Vxx,  27:Vyy
					m_FxxR_UF[i] += LctbPos.dRemnFxxUF * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
					m_FyyR_UF[i] += LctbPos.dRemnFyyUF * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
					m_FxyR_UF[i] += LctbPos.dRemnFxyUF * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy * ShrrD.ShrrBase[2].dRatVy;
					m_VxxR_UF[i] += LctbPos.dRemnVxxUF * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
					m_VyyR_UF[i] += LctbPos.dRemnVyyUF * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
					m_MxxR_UF[i] += LctbPos.dRemnMxxUF * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
					m_MyyR_UF[i] += LctbPos.dRemnMyyUF * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
					m_MxyR_UF[i] += LctbPos.dRemnMxyUF * dScaleUpFactor;
					// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
					m_FxyUFOrgR[i] += LctbPos.dRemnFxyUF * dScaleUpFactor * m_dSueqLcasV;
					m_VxxUFOrgR[i] += LctbPos.dRemnVxxUF * dScaleUpFactor * m_dSueqLcasV;
					m_VyyUFOrgR[i] += LctbPos.dRemnVyyUF * dScaleUpFactor * m_dSueqLcasV;
				}
			}
			else
			{
				if (IsRatingCode() && (IsCheckStaticDL() || IsEndLCase()))
				{
					if (IsEndLCase())
					{
						if (i < 4)
						{
							double dFactFx = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
							double dFactFy = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
							double dFactFz = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
							double dFactMx = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
							double dFactMy = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
							double dFactMz = dFactor * dScaleUpFactor;
							AddPlatForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, StrpMaxData, dFactFx, dFactFy, dFactFz, dFactMx, dFactMy, dFactMz);
							// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
							m_FzzOrgR[i] += StrpMaxData.dblForce[i][2] * dFactor * dScaleUpFactor * m_dSueqLcasV;
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							double dFactFxx = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
							double dFactFyy = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
							double dFactFxy = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy * ShrrD.ShrrBase[2].dRatVy;
							double dFactVxx = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
							double dFactVyy = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
							double dFactMxx = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
							double dFactMyy = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
							double dFactMxy = dFactor * dScaleUpFactor;
							AddPlatForceUF(i, m_FxxML_UF, m_FyyML_UF, m_FxyML_UF, m_VxxML_UF, m_VyyML_UF, m_MxxML_UF, m_MyyML_UF, m_MxyML_UF, StrpMaxData,
								dFactFxx, dFactFyy, dFactFxy, dFactVxx, dFactVyy, dFactMxx, dFactMyy, dFactMxy);
							// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
							m_FxyUFOrgR[i] += StrpMaxData.dblStress[i][16] * dFactor * dScaleUpFactor * m_dSueqLcasV;
							m_VxxUFOrgR[i] += StrpMaxData.dblStress[i][26] * dFactor * dScaleUpFactor * m_dSueqLcasV;
							m_VyyUFOrgR[i] += StrpMaxData.dblStress[i][27] * dFactor * dScaleUpFactor * m_dSueqLcasV;
						}
					}
					else if (IsCheckStaticDL())
					{
						if (i < 4)
						{//component : 0:Fx, 1:Fy, 2:Fz, 3:Mx, 4:My, 5:Mz
							double dFactFx = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
							double dFactFy = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
							double dFactFz = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
							double dFactMx = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
							double dFactMy = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
							double dFactMz = dFactor * dScaleUpFactor;
							AddPlatForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrpMaxData,
								dFactFx, dFactFy, dFactFz, dFactMx, dFactMy, dFactMz);
							// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
							m_FzzOrgR[i] += StrpMaxData.dblForce[i][2] * dFactor * dScaleUpFactor * m_dSueqLcasV;
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							double dFactFxx = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
							double dFactFyy = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
							double dFactFxy = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy * ShrrD.ShrrBase[2].dRatVy;
							double dFactVxx = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
							double dFactVyy = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
							double dFactMxx = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
							double dFactMyy = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
							double dFactMxy = dFactor * dScaleUpFactor;
							AddPlatForceUF(i, m_FxxD_UF, m_FyyD_UF, m_FxyD_UF, m_VxxD_UF, m_VyyD_UF, m_MxxD_UF, m_MyyD_UF, m_MxyD_UF, StrpMaxData,
								dFactFxx, dFactFyy, dFactFxy, dFactVxx, dFactVyy, dFactMxx, dFactMyy, dFactMxy);
							// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
							m_FxyUFOrgR[i] += StrpMaxData.dblStress[i][16] * dFactor * dScaleUpFactor * m_dSueqLcasV;
							m_VxxUFOrgR[i] += StrpMaxData.dblStress[i][26] * dFactor * dScaleUpFactor * m_dSueqLcasV;
							m_VyyUFOrgR[i] += StrpMaxData.dblStress[i][27] * dFactor * dScaleUpFactor * m_dSueqLcasV;
						}
					}
				}
				else
				{
					if (i < 4)
					{
						double dFactFx = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
						double dFactFy = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
						double dFactFz = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
						double dFactMx = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
						double dFactMy = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
						double dFactMz = dFactor * dScaleUpFactor;
						AddPlatForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, StrpMaxData, dFactFx, dFactFy, dFactFz, dFactMx, dFactMy, dFactMz);
						// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
						m_FzzOrgR[i] += StrpMaxData.dblForce[i][2] * dFactor * dScaleUpFactor * m_dSueqLcasV;
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{
						double dFactFxx = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
						double dFactFyy = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * ShrrD.ShrrBase[0].dRatAx * ShrrD.ShrrBase[1].dRatAx;
						double dFactFxy = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVy * ShrrD.ShrrBase[1].dRatVy * ShrrD.ShrrBase[2].dRatVy;
						double dFactVxx = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
						double dFactVyy = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV * ShrrD.ShrrBase[0].dRatVz * ShrrD.ShrrBase[1].dRatVz * ShrrD.ShrrBase[2].dRatVz;
						double dFactMxx = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
						double dFactMyy = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM * ShrrD.ShrrBase[0].dRatMy * ShrrD.ShrrBase[1].dRatMy * ShrrD.ShrrBase[2].dRatMy;
						double dFactMxy = dFactor * dScaleUpFactor;
						AddPlatForceUF(i, m_FxxR_UF, m_FyyR_UF, m_FxyR_UF, m_VxxR_UF, m_VyyR_UF, m_MxxR_UF, m_MyyR_UF, m_MxyR_UF, StrpMaxData,
							dFactFxx, dFactFyy, dFactFxy, dFactVxx, dFactVyy, dFactMxx, dFactMyy, dFactMxy);
						// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
						m_FxyUFOrgR[i] += StrpMaxData.dblStress[i][16] * dFactor * dScaleUpFactor * m_dSueqLcasV;
						m_VxxUFOrgR[i] += StrpMaxData.dblStress[i][26] * dFactor * dScaleUpFactor * m_dSueqLcasV;
						m_VyyUFOrgR[i] += StrpMaxData.dblStress[i][27] * dFactor * dScaleUpFactor * m_dSueqLcasV;
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CDgnForceCtrl::Get_MovingForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor, BOOL bMaxLcom)
{
	if (iLcaseKey == 0) { ASSERT(0); return FALSE; }
	if (dFactor == 0.0) { ASSERT(0); return FALSE; }
	int iLcaseType = D_LOADCASE_MOVING;
	// Change by ZINU.('02.11.26).
	T_STRB_D  StrbMaxData, StrbMinData;
	T_STRT_D  StrtMaxData, StrtMinData;
	T_STRW_D  StrwMaxData, StrwMinData;
	T_STRP_DL StrpMaxData, StrpMinData;
	if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
	{
		StrbMaxData.Initialize();
		StrbMinData.Initialize();
		GetStrbDgn(iElemNo, iLcaseType, iLcaseKey, &StrbMaxData, &StrbMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
	{
		StrtMaxData.Initialize();
		StrtMinData.Initialize();
		GetStrtDgn(iElemNo, iLcaseType, iLcaseKey, &StrtMaxData, &StrtMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
	{
		StrwMaxData.Initialize();
		StrwMinData.Initialize();
		GetStrwDgn(iElemNo, iLcaseType, iLcaseKey, &StrwMaxData, &StrwMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Type.
	{
		StrpMaxData.Initialize();
		StrpMinData.Initialize();
		GetStrpDgn(iElemNo, iLcaseType, iLcaseKey, &StrpMaxData, &StrpMinData);
	}

	// Save Forces and Moments by Position.
	int iPosStaNo = 0;
	int iPosTotNo = 0;
	if (iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
	else
	{
		iPosStaNo = iPosiNo;
		iPosTotNo = iPosiNo + 1;
	}

	BOOL bSteelMatl = (m_strMatType == _T("S")) ? TRUE : FALSE;

	T_STLD_D Lcase;
	Lcase.Initialize();
	m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, Lcase);
	BOOL bIsJUD = m_pDgnDataCtrl->IsJUD();

	if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType)) // Beam Type.
	{
		const T_STRB_D& CurStrb = bMaxLcom ? StrbMaxData : StrbMinData;
		for (int i = iPosStaNo; i < iPosTotNo; i++)
		{
			double dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T;
			dExFactor_P = dExFactor_V = dExFactor_M = dExFactor_T = dFactor;
			if (bIsJUD) { GetExpandEqFactor(iElemNo, i, Lcase.LoadCaseType, dFactor, dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T); } // PMS:4215 - 일본2차설계시 1차설계 지진하중 할증
			// End Moments.
			if (!Get_BeamEndMoment(_T("ML"), i, dExFactor_M, bMaxLcom, TRUE, iLcaseType, iLcaseKey, (bMaxLcom ? &StrbMaxData : &StrbMinData)))	return FALSE;
			// Forces, Moments.
			if (IsDeadLoadType(_T(""), D_LCOM_MOVING, iLcaseKey))
			{
				AddBeamForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, CurStrb, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
				// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
				AddBeamForceDof7(i, m_MbiD, m_TstD, m_TwpD, CurStrb);  //!!!                    
			}
			else
			{
				AddBeamForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, CurStrb, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
				// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
				AddBeamForceDof7(i, m_MbiML, m_TstML, m_TwpML, CurStrb);  //!!!        
			}
		}
	}
	else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType)) // Wall Type.
	{
		const T_STRW_D& CurStrw = bMaxLcom ? StrwMaxData : StrwMinData;
		for (int i = iPosStaNo; i < iPosTotNo; i++)
		{
			// End Moments.
			if (!Get_WallEndMoment(_T("ML"), i, dFactor, TRUE, iLcaseKey, (bMaxLcom ? &StrwMaxData : &StrwMinData)))	return FALSE;
			// Forces, Moments.
			// PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.                
			AddWallForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, CurStrw,
				dFactor*m_dNTC08LcasN, dFactor, dFactor, dFactor, dFactor, dFactor);
		}
	}
	else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType)) // Truss Type.
	{
		for (int i = iPosStaNo; i < iPosTotNo; i++)
		{
			// End Moments.
			if (!Get_TrussEndMoment(_T("ML"), i))	return FALSE;
			// Forces, Moments.
			m_FxxML[i] += (bMaxLcom ? StrtMaxData.dblForce[i / 4] : StrtMinData.dblForce[i / 4]) * dFactor;
		}
	}
	else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Plate.
	{
		const T_STRP_DL& CurStrp = bMaxLcom ? StrpMaxData : StrpMinData;
		for (int i = iPosStaNo; i < iPosTotNo; i++)
		{
			// End Moments.		
			if (!Get_PlateEndMoment(_T("ML"), i))	return FALSE;
			if (IsRatingCode())
			{
				if (IsEndLCase())
				{
					// Forces, Moments. (4 node)
					if (i < 4)
					{
						AddPlatForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, CurStrp, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{
						AddPlatForceUF(i, m_FxxML_UF, m_FyyML_UF, m_FxyML_UF, m_VxxML_UF, m_VyyML_UF, m_MxxML_UF, m_MyyML_UF, m_MxyML_UF, CurStrp,
							dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
				}
				else if (IsCheckStaticDL())
				{
					// Forces, Moments. (4 node)
					if (i < 4)
					{
						AddPlatForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, CurStrp, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{
						AddPlatForceUF(i, m_FxxD_UF, m_FyyD_UF, m_FxyD_UF, m_VxxD_UF, m_VyyD_UF, m_MxxD_UF, m_MyyD_UF, m_MxyD_UF, CurStrp,
							dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
				}
				else
				{
					// Forces, Moments. (4 node)
					if (i < 4)
					{
						AddPlatForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, CurStrp, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{
						AddPlatForceUF(i, m_FxxR_UF, m_FyyR_UF, m_FxyR_UF, m_VxxR_UF, m_VyyR_UF, m_MxxR_UF, m_MyyR_UF, m_MxyR_UF, CurStrp,
							dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
				}
			}
			else
			{
				// Forces, Moments. (4 node)
				if (i < 4)
				{
					AddPlatForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, CurStrp, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
				}
				// Unit Forces (cent + 4 node)
				if (i < 5)
				{
					AddPlatForceUF(i, m_FxxML_UF, m_FyyML_UF, m_FxyML_UF, m_VxxML_UF, m_VyyML_UF, m_MxxML_UF, m_MyyML_UF, m_MxyML_UF, CurStrp,
						dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
				}
			}
		}
	}

	return TRUE;
}

BOOL CDgnForceCtrl::Get_RemnantForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor, int iAnalType, BOOL bMaxLcom)
{
	if (iLcaseKey == 0) { ASSERT(0); return FALSE; }
	if (dFactor == 0.0) { ASSERT(0); return FALSE; }
	if (iAnalType == 0) { ASSERT(0); return FALSE; }
	// Add by ZINU.('02.11.14). NOT LcomType BUT LcaseType.
	int iLcaseType = 0;
	if (iAnalType == D_LCOM_SETTLEMENT)		iLcaseType = D_LOADCASE_SETTLE;
	else if (iAnalType == D_LCOM_HISTORY)	iLcaseType = D_LOADCASE_HISTORY;
	else	ASSERT(0);
	// Change by ZINU.('02.11.26).
	T_STRB_D  StrbMaxData, StrbMinData;
	T_STRT_D  StrtMaxData, StrtMinData;
	T_STRW_D  StrwMaxData, StrwMinData;
	T_STRP_DL StrpMaxData, StrpMinData;
	if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
	{
		StrbMaxData.Initialize();
		StrbMinData.Initialize();
		GetStrbDgn(iElemNo, iLcaseType, iLcaseKey, &StrbMaxData, &StrbMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
	{
		StrtMaxData.Initialize();
		StrtMinData.Initialize();
		GetStrtDgn(iElemNo, iLcaseType, iLcaseKey, &StrtMaxData, &StrtMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
	{
		StrwMaxData.Initialize();
		StrwMinData.Initialize();
		GetStrwDgn(iElemNo, iLcaseType, iLcaseKey, &StrwMaxData, &StrwMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Type.
	{
		StrpMaxData.Initialize();
		StrpMinData.Initialize();
		GetStrpDgn(iElemNo, iLcaseType, iLcaseKey, &StrpMaxData, &StrpMinData);
	}

	// Save Forces and Moments by Position.
	int iPosStaNo = 0;
	int iPosTotNo = 0;
	if (iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
	else
	{
		iPosStaNo = iPosiNo;
		iPosTotNo = iPosiNo + 1;
	}

	T_STLD_D Lcase;
	Lcase.Initialize();
	m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, Lcase);
	BOOL bIsJUD = m_pDgnDataCtrl->IsJUD();

	const T_STRB_D& CurStrb = bMaxLcom ? StrbMaxData : StrbMinData;
	const T_STRW_D& CurStrw = bMaxLcom ? StrwMaxData : StrwMinData;
	const T_STRP_DL& CurStrp = bMaxLcom ? StrpMaxData : StrpMinData;

	for (int i = iPosStaNo; i < iPosTotNo; i++)
	{
		if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType)) // Beam Type.
		{
			double dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T;
			dExFactor_P = dExFactor_V = dExFactor_M = dExFactor_T = dFactor;
			if (bIsJUD) { GetExpandEqFactor(iElemNo, i, Lcase.LoadCaseType, dFactor, dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T); } // PMS:4215 - 일본2차설계시 1차설계 지진하중 할증
			// End Moments.
			if (!Get_BeamEndMoment(_T("R"), i, dExFactor_M, bMaxLcom, TRUE, iLcaseType, iLcaseKey, (bMaxLcom ? &StrbMaxData : &StrbMinData)))	return FALSE;
			// Forces, Moments.
			if (m_bExistLctb)
			{
				// Seungjun-20121231 : iGen 비선형 부재의 경우도 할증계수 적용
				dExFactor_P = bIsJUD ? fabs(dExFactor_P) : 1.0;
				dExFactor_V = bIsJUD ? fabs(dExFactor_V) : 1.0;
				dExFactor_M = bIsJUD ? fabs(dExFactor_M) : 1.0;
				dExFactor_T = bIsJUD ? fabs(dExFactor_T) : 1.0;

				_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
				if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);

				double dFxx = LctbPos.dRemnFxx * dExFactor_P;
				double dFyy = LctbPos.dRemnFyy * dExFactor_V;
				double dFzz = LctbPos.dRemnFzz * dExFactor_V;
				double dBMx = LctbPos.dRemnBMx * dExFactor_T;
				double dBMy = LctbPos.dRemnBMy * dExFactor_M;
				double dBMz = LctbPos.dRemnBMz * dExFactor_M;
				// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
				double dMbi = LctbPos.dRemnMbi; //!!!
				double dTst = LctbPos.dRemnTst; //!!!
				double dTwp = LctbPos.dRemnTwp; //!!!
				// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
				double dFyyOrg = LctbPos.dRemnFyy * dExFactor_V;
				double dFzzOrg = LctbPos.dRemnFzz * dExFactor_V;
				if (IsDeadLoadType(_T(""), iAnalType, iLcaseKey))
				{
					AddDeadSum(i, dFxx, dFyy, dFzz, dBMx, dBMy, dBMz, dMbi, dTst, dTwp);
				}
				else
				{
					AddRemnantSum(i, dFxx, dFyy, dFzz, dBMx, dBMy, dBMz, dMbi, dTst, dTwp, dFyyOrg, dFzzOrg);
				}
			}
			else
			{
				if (IsDeadLoadType(_T(""), iAnalType, iLcaseKey))
				{
					AddBeamForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, CurStrb, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
					AddBeamForceDof7(i, m_MbiD, m_TstD, m_TwpD, CurStrb); //!!! 부재력 조정계수 고려 범위 확인 요망.                
				}
				else
				{
					AddBeamForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, CurStrb, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
					// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
					AddBeamForceDof7(i, m_MbiR, m_TstR, m_TwpR, CurStrb); //!!! 부재력 조정계수 고려 범위 확인 요망.                
					// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
					int nPart = 0;
					int nKind = 0;
					int nKind7 = 0;
					GetAnalForceComponentIndex(i, nPart, nKind, nKind7);
					m_FyyOrgR[i] += CurStrb.dblForce[nPart][nKind + 1] * dExFactor_V;
					m_FzzOrgR[i] += CurStrb.dblForce[nPart][nKind + 2] * dExFactor_V;
				}
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType)) // Wall Type.
		{
			// End Moments.
			if (!Get_WallEndMoment(_T("R"), i, dFactor, TRUE, iLcaseKey, (bMaxLcom ? &StrwMaxData : &StrwMinData)))	return FALSE;
			// Forces, Moments.
			if (m_bExistLctb)
			{
				_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
				if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
				m_FxxR[i] += (LctbPos.dRemnFxx * m_dNTC08LcasN);  // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.
				m_FyyR[i] += LctbPos.dRemnFyy;
				m_FzzR[i] += LctbPos.dRemnFzz;
				m_BMxR[i] += LctbPos.dRemnBMx;
				m_BMyR[i] += LctbPos.dRemnBMy;
				m_BMzR[i] += LctbPos.dRemnBMz;
				// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
				m_FyyOrgR[i] += LctbPos.dRemnFyy;
				m_FzzOrgR[i] += LctbPos.dRemnFzz;
			}
			else
			{
				AddWallForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, CurStrw, dFactor*m_dNTC08LcasN, dFactor, dFactor, dFactor, dFactor, dFactor);
				// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
				int nPart = (i < 4 ? 1 : 0);	// I=1, J=0.
				m_FyyOrgR[i] += CurStrw.dblForce[nPart][1] * dFactor;
				m_FzzOrgR[i] += CurStrw.dblForce[nPart][2] * dFactor;
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType)) // Truss Type.
		{
			// End Moments.
			if (!Get_TrussEndMoment(_T("R"), i))	return FALSE;
			// Forces, Moments.
			if (m_bExistLctb)
			{
				_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
				if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
				m_FxxR[i] += LctbPos.dRemnFxx;

			}
			else
			{
				m_FxxR[i] += (bMaxLcom ? StrtMaxData.dblForce[i / 4] : StrtMinData.dblForce[i / 4]) * dFactor;
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Plate.
		{
			// End Moments.
			if (!Get_PlateEndMoment(_T("R"), i))	return FALSE;
			// Forces, Moments.
			if (m_bExistLctb)
			{
				_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
				if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
				if (i < 4)
				{//component : 0:Fx, 1:Fy, 2:Fz, 3:Mx, 4:My, 5:Mz
					m_FxxR[i] += LctbPos.dRemnFxx;
					m_FyyR[i] += LctbPos.dRemnFyy;
					m_FzzR[i] += LctbPos.dRemnFzz;
					m_BMxR[i] += LctbPos.dRemnBMx;
					m_BMyR[i] += LctbPos.dRemnBMy;
					m_BMzR[i] += LctbPos.dRemnBMz;
					// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
					m_FzzOrgR[i] += LctbPos.dRemnFzz;
				}
				// Unit Forces (cent + 4 node)
				if (i < 5)
				{//component : 14:Fxx, 16:Fyy, 17:Fxy, 20:Mxx, 21:Myy, 26:Vxx,  27:Vyy
					m_FxxR_UF[i] += LctbPos.dRemnFxxUF;
					m_FyyR_UF[i] += LctbPos.dRemnFyyUF;
					m_FxyR_UF[i] += LctbPos.dRemnFxyUF;
					m_VxxR_UF[i] += LctbPos.dRemnVxxUF;
					m_VyyR_UF[i] += LctbPos.dRemnVyyUF;
					m_MxxR_UF[i] += LctbPos.dRemnMxxUF;
					m_MyyR_UF[i] += LctbPos.dRemnMyyUF;
					m_MxyR_UF[i] += LctbPos.dRemnMxyUF;
					// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
					m_FxyUFOrgR[i] += LctbPos.dRemnFxyUF;
					m_VxxUFOrgR[i] += LctbPos.dRemnVxxUF;
					m_VyyUFOrgR[i] += LctbPos.dRemnVyyUF;
				}
			}
			else
			{
				if (IsRatingCode() && IsCheckStaticDL())
				{
					if (i < 4)
					{
						AddPlatForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, CurStrp, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
						m_FzzOrgR[i] += CurStrp.dblForce[i][2] * dFactor;
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{
						AddPlatForceUF(i, m_FxxD_UF, m_FyyD_UF, m_FxyD_UF, m_VxxD_UF, m_VyyD_UF, m_MxxD_UF, m_MyyD_UF, m_MxyD_UF, CurStrp,
							dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
						m_FxyUFOrgR[i] += CurStrp.dblStress[i][16] * dFactor;
						m_VxxUFOrgR[i] += CurStrp.dblStress[i][26] * dFactor;
						m_VyyUFOrgR[i] += CurStrp.dblStress[i][27] * dFactor;
					}
				}
				else
				{
					if (i < 4)
					{
						AddPlatForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, CurStrp, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
						m_FzzOrgR[i] += CurStrp.dblForce[i][2] * dFactor;
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{
						AddPlatForceUF(i, m_FxxR_UF, m_FyyR_UF, m_FxyR_UF, m_VxxR_UF, m_VyyR_UF, m_MxxR_UF, m_MyyR_UF, m_MxyR_UF, CurStrp,
							dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
						m_FxyUFOrgR[i] += CurStrp.dblStress[i][16] * dFactor;
						m_VxxUFOrgR[i] += CurStrp.dblStress[i][26] * dFactor;
						m_VyyUFOrgR[i] += CurStrp.dblStress[i][27] * dFactor;
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CDgnForceCtrl::Get_StageForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor)
{
	if (iLcaseKey == 0) { ASSERT(0); return FALSE; }
	if (dFactor == 0.0) { ASSERT(0); return FALSE; }
	// Assumed Dead Load by ZINU agree with Young-Hyun,Ko.
	// Add by ZINU.('02.12.11). To distinguish primary from secondary.
	T_STCT_D StctD; StctD.Initialize();
	m_pDoc->m_pAttrCtrl->GetStct(StctD);
	T_SGLD_D SgldData;
	m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldPost(iLcaseKey, SgldData);
	CString strErLCType = _T("");
	int iTndnLcaseType = 0;	// 1=SUM, 2=Tendon Primary, 3=Tendon Secondary, 4=Live Load.
	if (SgldData.LoadCaseName == _LS(IDS_DB_SGLD_SUM))			iTndnLcaseType = 1;
	else if (SgldData.LoadCaseName == _LS(IDS_DB_SGLD_TP))	iTndnLcaseType = 2;
	else if (SgldData.LoadCaseName == _LS(IDS_DB_SGLD_TS))	iTndnLcaseType = 3;
	else if (SgldData.LoadCaseName == _LS(IDS_DB_SGLD_LL)) iTndnLcaseType = 4;
	else if (GetErectionLoadType(SgldData.LoadCaseName, StctD, strErLCType))
	{
		if (CDBLib::IsStldLL(strErLCType)) iTndnLcaseType = 4;
	}

	int iLcaseType = D_LOADCASE_STAGE;
	// Change by ZINU.('02.11.26).
	T_STRB_D  StrbMaxData, StrbMinData;
	T_STRT_D  StrtMaxData, StrtMinData;
	T_STRW_D  StrwMaxData, StrwMinData;
	T_STRP_DL StrpMaxData, StrpMinData;
	if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
	{
		StrbMaxData.Initialize();
		StrbMinData.Initialize();
		GetStrbDgn(iElemNo, iLcaseType, iLcaseKey, &StrbMaxData, &StrbMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
	{
		StrtMaxData.Initialize();
		StrtMinData.Initialize();
		GetStrtDgn(iElemNo, iLcaseType, iLcaseKey, &StrtMaxData, &StrtMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
	{
		StrwMaxData.Initialize();
		StrwMinData.Initialize();
		GetStrwDgn(iElemNo, iLcaseType, iLcaseKey, &StrwMaxData, &StrwMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Type.
	{
		StrpMaxData.Initialize();
		StrpMinData.Initialize();
		GetStrpDgn(iElemNo, iLcaseType, iLcaseKey, &StrpMaxData, &StrpMinData);
	}

	// Save Forces and Moments by Position.
	int iPosStaNo = 0;
	int iPosTotNo = 0;
	if (iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
	else
	{
		iPosStaNo = iPosiNo;
		iPosTotNo = iPosiNo + 1;
	}

	// Add by ZINU.('04.02.06). For Live Load Reduction Factor at Stage.
	double dReduAxlFactor = 1.0;
	double dReduMomFactor = 1.0;
	double dReduShrFactor = 1.0;
	if (iTndnLcaseType == 4)	// Live Load.
	{
		// Change by ZINU.('00.05.11). L -> L,LR.
		// Change by ZINU.('03.02.11). For China Standard agree with Kye CJ.
		// 1. Column, Wall -> Beam, Column, Brace, Wall.
		// 2. Axial -> Axial, Moment, Shear.
		// If Stage, Live Load is always included. 
		dReduAxlFactor = (m_bReduAxl ? m_dReduFactor : 1.0);
		dReduMomFactor = (m_bReduMom ? m_dReduFactor : 1.0);
		dReduShrFactor = (m_bReduShr ? m_dReduFactor : 1.0);
	}

	T_STLD_D Lcase;
	Lcase.Initialize();
	m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, Lcase);
	BOOL bIsJUD = m_pDgnDataCtrl->IsJUD();

	for (int i = iPosStaNo; i < iPosTotNo; i++)
	{
		if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType)) // Beam Type.
		{
			double dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T;
			dExFactor_P = dExFactor_V = dExFactor_M = dExFactor_T = dFactor;
			if (bIsJUD) { GetExpandEqFactor(iElemNo, i, Lcase.LoadCaseType, dFactor, dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T); } // PMS:4215 - 일본2차설계시 1차설계 지진하중 할증

			if (iTndnLcaseType == 2)	// Primary.
			{
				// End Moments.
				if (!Get_BeamEndMoment(_T("TP"), i, dExFactor_M, TRUE, TRUE, iLcaseType, iLcaseKey, &StrbMaxData))	return FALSE;
				if (IsDeadLoadType(_T(""), D_LCOM_STAGE, iLcaseKey))
				{
					AddBeamForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrbMaxData, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
					AddBeamForceDof7(i, m_MbiD, m_TstD, m_TwpD, StrbMaxData); //!!! 부재력 조정계수 고려범위 확인 요망.
				}
				else
				{
					// Forces, Moments.
					AddBeamForce(i, m_FxxTP, m_FyyTP, m_FzzTP, m_BMxTP, m_BMyTP, m_BMzTP, StrbMaxData, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
					// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
					AddBeamForceDof7(i, m_MbiTP, m_TstTP, m_TwpTP, StrbMaxData); //!!! 부재력 조정계수 고려범위 확인 요망.
				}
			}
			else if (iTndnLcaseType == 3)	// Secondary.
			{
				// End Moments.
				if (!Get_BeamEndMoment(_T("TS"), i, dExFactor_M, TRUE, TRUE, iLcaseType, iLcaseKey, &StrbMaxData))	return FALSE;
				if (IsDeadLoadType(_T(""), D_LCOM_STAGE, iLcaseKey))
				{
					AddBeamForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrbMaxData, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
					AddBeamForceDof7(i, m_MbiD, m_TstD, m_TwpD, StrbMaxData); //!!! 부재력 조정계수 고려범위 확인 요망.
				}
				else
				{
					// Forces, Moments.
					AddBeamForce(i, m_FxxTS, m_FyyTS, m_FzzTS, m_BMxTS, m_BMyTS, m_BMzTS, StrbMaxData, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
					// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
					AddBeamForceDof7(i, m_MbiTS, m_TstTS, m_TwpTS, StrbMaxData); //!!! 부재력 조정계수 고려범위 확인 요망.				
				}
			}
			else if (iTndnLcaseType == 4)	// Live Load.
			{
				// End Moments.
				if (!Get_BeamEndMoment(_T("L"), i, dExFactor_M, TRUE, TRUE, iLcaseType, iLcaseKey, &StrbMaxData))	return FALSE;
				// Forces, Moments.
				double dFactP = dExFactor_P * dReduAxlFactor;
				double dFactV = dExFactor_V * dReduShrFactor;
				double dFactT = dExFactor_T * dReduMomFactor;
				double dFactM = dExFactor_M * dReduMomFactor;
				if (IsDeadLoadType(_T(""), D_LCOM_STAGE, iLcaseKey))
				{
					AddBeamForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrbMaxData, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
					AddBeamForceDof7(i, m_MbiD, m_TstD, m_TwpD, StrbMaxData); //!!! 부재력 조정계수 고려범위 확인 요망.
				}
				else
				{
					AddBeamForce(i, m_FxxL, m_FyyL, m_FzzL, m_BMxL, m_BMyL, m_BMzL, StrbMaxData, dFactP, dFactV, dFactV, dFactT, dFactM, dFactM);
					// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
					AddBeamForceDof7(i, m_MbiL, m_TstL, m_TwpL, StrbMaxData); //!!! 부재력 조정계수 고려범위 확인 요망.
				}
			}
			else
			{
				// End Moments.
				if (!Get_BeamEndMoment(_T("D"), i, dExFactor_M, TRUE, TRUE, iLcaseType, iLcaseKey, &StrbMaxData))	return FALSE;
				// Forces, Moments.
				AddBeamForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrbMaxData, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
				// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
				AddBeamForceDof7(i, m_MbiD, m_TstD, m_TwpD, StrbMaxData); //!!! 부재력 조정계수 고려범위 확인 요망.
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType)) // Wall Type.
		{
			if (iTndnLcaseType == 2)	// Primary.
			{
				// End Moments.
				if (!Get_WallEndMoment(_T("TP"), i, dFactor, TRUE, iLcaseKey, &StrwMaxData))	return FALSE;
				// Forces, Moments. // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design. (m_dNTC08LcasN)
				AddWallForce(i, m_FxxTP, m_FyyTP, m_FzzTP, m_BMxTP, m_BMyTP, m_BMzTP, StrwMaxData, dFactor*m_dNTC08LcasN, dFactor, dFactor, dFactor, dFactor, dFactor);
			}
			else if (iTndnLcaseType == 3)	// Secondary.
			{
				// End Moments.
				if (!Get_WallEndMoment(_T("TS"), i, dFactor, TRUE, iLcaseKey, &StrwMaxData))	return FALSE;
				// Forces, Moments. // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design. (m_dNTC08LcasN)
				AddWallForce(i, m_FxxTS, m_FyyTS, m_FzzTS, m_BMxTS, m_BMyTS, m_BMzTS, StrwMaxData, dFactor*m_dNTC08LcasN, dFactor, dFactor, dFactor, dFactor, dFactor);
			}
			else if (iTndnLcaseType == 4)	// Live Load.
			{
				// End Moments.
				if (!Get_WallEndMoment(_T("L"), i, dFactor, TRUE, iLcaseKey, &StrwMaxData))	return FALSE;
				// Forces, Moments.                
				double dFactP = dFactor * dReduAxlFactor * m_dNTC08LcasN;  // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.
				double dFactV = dFactor * dReduShrFactor;
				double dFactM = dFactor * dReduMomFactor;
				AddWallForce(i, m_FxxL, m_FyyL, m_FzzL, m_BMxL, m_BMyL, m_BMzL, StrwMaxData, dFactP, dFactV, dFactV, dFactM, dFactM, dFactM);
			}
			else
			{
				// End Moments.
				if (!Get_WallEndMoment(_T("D"), i, dFactor, TRUE, iLcaseKey, &StrwMaxData))	return FALSE;
				// Forces, Moments.  // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design. (m_dNTC08LcasN)
				AddWallForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrwMaxData, dFactor*m_dNTC08LcasN, dFactor, dFactor, dFactor, dFactor, dFactor);
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType)) // Truss Type.
		{
			if (iTndnLcaseType == 2)	// Primary.
			{
				// End Moments.
				if (!Get_TrussEndMoment(_T("TP"), i))	return FALSE;
				// Forces, Moments.
				m_FxxTP[i] += StrtMaxData.dblForce[i / 4] * dFactor;
			}
			else if (iTndnLcaseType == 3)	// Secondary.
			{
				// End Moments.
				if (!Get_TrussEndMoment(_T("TS"), i))	return FALSE;
				// Forces, Moments.
				m_FxxTS[i] += StrtMaxData.dblForce[i / 4] * dFactor;
			}
			else if (iTndnLcaseType == 4)	// Live Load.
			{
				// End Moments.
				if (!Get_TrussEndMoment(_T("L"), i))	return FALSE;
				// Forces, Moments.
				m_FxxL[i] += StrtMaxData.dblForce[i / 4] * dFactor * dReduAxlFactor;
			}
			else
			{
				// End Moments.
				if (!Get_TrussEndMoment(_T("D"), i))	return FALSE;
				// Forces, Moments.
				m_FxxD[i] += StrtMaxData.dblForce[i / 4] * dFactor;
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Plate.
		{
			if (iTndnLcaseType == 2)	// Primary.			
			{
				// End Moments.
				if (!Get_PlateEndMoment(_T("TP"), i))	return FALSE;
				if (IsRatingCode())
				{
					if (IsEndLCase())
					{
						// Forces, Moments. (4 node)
						if (i < 4)
						{
							AddPlatForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxML_UF, m_FyyML_UF, m_FxyML_UF, m_VxxML_UF, m_VyyML_UF, m_MxxML_UF, m_MyyML_UF, m_MxyML_UF, StrpMaxData,
								dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
					}
					else if (IsCheckStaticDL())
					{
						// Forces, Moments. (4 node)
						if (i < 4)
						{
							AddPlatForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxD_UF, m_FyyD_UF, m_FxyD_UF, m_VxxD_UF, m_VyyD_UF, m_MxxD_UF, m_MyyD_UF, m_MxyD_UF, StrpMaxData,
								dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
					}
					else
					{
						// Forces, Moments. (4 node)
						if (i < 4)
						{
							AddPlatForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxR_UF, m_FyyR_UF, m_FxyR_UF, m_VxxR_UF, m_VyyR_UF, m_MxxR_UF, m_MyyR_UF, m_MxyR_UF, StrpMaxData,
								dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
					}
				}
				else
				{
					// Forces, Moments. (4 node)
					if (i < 4)
					{
						AddPlatForce(i, m_FxxTP, m_FyyTP, m_FzzTP, m_BMxTP, m_BMyTP, m_BMzTP, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{
						AddPlatForceUF(i, m_FxxTP_UF, m_FyyTP_UF, m_FxyTP_UF, m_VxxTP_UF, m_VyyTP_UF, m_MxxTP_UF, m_MyyTP_UF, m_MxyTP_UF, StrpMaxData,
							dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
				}
			}
			else if (iTndnLcaseType == 3)	// Secondary.
			{
				// End Moments.
				if (!Get_PlateEndMoment(_T("TS"), i))	return FALSE;
				if (IsRatingCode())
				{
					if (IsEndLCase())
					{
						// Forces, Moments. (4 node)
						if (i < 4)
						{
							AddPlatForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxML_UF, m_FyyML_UF, m_FxyML_UF, m_VxxML_UF, m_VyyML_UF, m_MxxML_UF, m_MyyML_UF, m_MxyML_UF, StrpMaxData,
								dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
					}
					else if (IsCheckStaticDL())
					{
						// Forces, Moments. (4 node)
						if (i < 4)
						{
							AddPlatForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxD_UF, m_FyyD_UF, m_FxyD_UF, m_VxxD_UF, m_VyyD_UF, m_MxxD_UF, m_MyyD_UF, m_MxyD_UF, StrpMaxData,
								dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
					}
					else
					{
						// Forces, Moments. (4 node)
						if (i < 4)
						{
							AddPlatForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxR_UF, m_FyyR_UF, m_FxyR_UF, m_VxxR_UF, m_VyyR_UF, m_MxxR_UF, m_MyyR_UF, m_MxyR_UF, StrpMaxData,
								dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
					}
				}
				else
				{
					// Forces, Moments. (4 node)
					if (i < 4)
					{
						AddPlatForce(i, m_FxxTS, m_FyyTS, m_FzzTS, m_BMxTS, m_BMyTS, m_BMzTS, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{
						AddPlatForceUF(i, m_FxxTS_UF, m_FyyTS_UF, m_FxyTS_UF, m_VxxTS_UF, m_VyyTS_UF, m_MxxTS_UF, m_MyyTS_UF, m_MxyTS_UF, StrpMaxData,
							dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
				}
			}
			else if (iTndnLcaseType == 4)	// Live Load.
			{
				// End Moments.
				if (!Get_PlateEndMoment(_T("L"), i))	return FALSE;
				// Forces, Moments. (4 node)
				if (IsRatingCode() && IsCheckStaticDL())
				{
					double dFactP = dFactor * dReduAxlFactor;
					double dFactV = dFactor * dReduShrFactor;
					double dFactM = dFactor * dReduMomFactor;

					if (IsEndLCase())
					{
						if (i < 4)
						{
							AddPlatForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyTS, m_BMzML, StrpMaxData, dFactP, dFactP, dFactV, dFactM, dFactM, dFactM);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxML_UF, m_FyyML_UF, m_FxyML_UF, m_VxxML_UF, m_VyyML_UF, m_MxxML_UF, m_MyyML_UF, m_MxyML_UF, StrpMaxData,
								dFactP, dFactP, dFactV, dFactV, dFactV, dFactM, dFactM, dFactM);
						}
					}
					else if (IsCheckStaticDL())
					{
						if (i < 4)
						{
							AddPlatForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrpMaxData, dFactP, dFactP, dFactV, dFactM, dFactM, dFactM);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxD_UF, m_FyyD_UF, m_FxyD_UF, m_VxxD_UF, m_VyyD_UF, m_MxxD_UF, m_MyyD_UF, m_MxyD_UF, StrpMaxData,
								dFactP, dFactP, dFactV, dFactV, dFactV, dFactM, dFactM, dFactM);
						}
					}
					else
					{
						if (i < 4)
						{
							AddPlatForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, StrpMaxData, dFactP, dFactP, dFactV, dFactM, dFactM, dFactM);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxR_UF, m_FyyR_UF, m_FxyR_UF, m_VxxR_UF, m_VyyR_UF, m_MxxR_UF, m_MyyR_UF, m_MxyR_UF, StrpMaxData,
								dFactP, dFactP, dFactV, dFactV, dFactV, dFactM, dFactM, dFactM);
						}
					}
				}
				else
				{
					double dFactP = dFactor * dReduAxlFactor;
					double dFactV = dFactor * dReduShrFactor;
					double dFactM = dFactor * dReduMomFactor;
					if (i < 4)
					{
						AddPlatForce(i, m_FxxL, m_FyyL, m_FzzL, m_BMxL, m_BMyL, m_BMzL, StrpMaxData, dFactP, dFactP, dFactV, dFactM, dFactM, dFactM);
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{
						AddPlatForceUF(i, m_FxxL_UF, m_FyyL_UF, m_FxyL_UF, m_VxxL_UF, m_VyyL_UF, m_MxxL_UF, m_MyyL_UF, m_MxyL_UF, StrpMaxData,
							dFactP, dFactP, dFactV, dFactV, dFactV, dFactM, dFactM, dFactM);
					}
				}
			}
			else
			{
				// End Moments.
				if (!Get_PlateEndMoment(_T("D"), i))	return FALSE;
				if (IsRatingCode())
				{
					if (IsEndLCase())
					{
						// Forces, Moments. (4 node)
						if (i < 4)
						{
							AddPlatForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxML_UF, m_FyyML_UF, m_FxyML_UF, m_VxxML_UF, m_VyyML_UF, m_MxxML_UF, m_MyyML_UF, m_MxyML_UF, StrpMaxData,
								dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
					}
					else if (IsCheckStaticDL())
					{
						AddPlateDLForce(i, StrpMaxData, dFactor);
					}
					else
					{
						// Forces, Moments. (4 node)
						if (i < 4)
						{
							AddPlatForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
						// Unit Forces (cent + 4 node)
						if (i < 5)
						{
							AddPlatForceUF(i, m_FxxR_UF, m_FyyR_UF, m_FxyR_UF, m_VxxR_UF, m_VyyR_UF, m_MxxR_UF, m_MyyR_UF, m_MxyR_UF, StrpMaxData,
								dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
						}
					}
				}
				else
				{
					// Forces, Moments. (4 node)
					if (i < 4)
					{
						AddPlatForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrpMaxData, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
					// Unit Forces (cent + 4 node)
					if (i < 5)
					{
						AddPlatForceUF(i, m_FxxD_UF, m_FyyD_UF, m_FxyD_UF, m_VxxD_UF, m_VyyD_UF, m_MxxD_UF, m_MyyD_UF, m_MxyD_UF, StrpMaxData,
							dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
					}
				}
			}
		}
	}

	// Change by ZINU.('03.02.15). Factored and Unfactored Forces by Self Weight.
	// Only SUM at Transfer Stage.
	if (m_iOrgLcomKey == m_iLcomKeyTran && iTndnLcaseType == 1 && m_iStldKeySelf > 0)
	{
		iLcaseType = D_LOADCASE_STATIC;
		iLcaseKey = m_iStldKeySelf;

		T_STRB_D  StrbMaxData, StrbMinData;
		T_STRT_D  StrtMaxData, StrtMinData;
		T_STRW_D  StrwMaxData, StrwMinData;
		T_STRP_DL StrpMaxData, StrpMinData;

		if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
		{
			StrbMaxData.Initialize();
			StrbMinData.Initialize();
			GetStrbDgn(iElemNo, iLcaseType, iLcaseKey, &StrbMaxData, &StrbMinData);
		}
		else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
		{
			StrtMaxData.Initialize();
			StrtMinData.Initialize();
			GetStrtDgn(iElemNo, iLcaseType, iLcaseKey, &StrtMaxData, &StrtMinData);
		}
		else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
		{
			StrwMaxData.Initialize();
			StrwMinData.Initialize();
			GetStrwDgn(iElemNo, iLcaseType, iLcaseKey, &StrwMaxData, &StrwMinData);
		}
		else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Type.
		{
			StrpMaxData.Initialize();
			StrpMinData.Initialize();
			GetStrpDgn(iElemNo, iLcaseType, iLcaseKey, &StrpMaxData, &StrpMinData);
		}
		// Save Forces and Moments by Position.
		int iPosStaNo = 0;
		int iPosTotNo = 0;
		if (iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
		else
		{
			iPosStaNo = iPosiNo;
			iPosTotNo = iPosiNo + 1;
		}

		// Factored Force is NOT used at Transfer Stage.
		for (int i = iPosStaNo; i < iPosTotNo; i++)
		{
			if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
			{
				int iPart = 0;
				int iKind = 0;
				int iKind7 = 0;
				GetAnalForceComponentIndex(i, iPart, iKind, iKind7);
				m_dVdu[i] = StrbMaxData.dblForce[iPart][iKind + 2] * 1.0;
				m_dMdu[i] = StrbMaxData.dblForce[iPart][iKind + 4] * 1.0;
				m_dVdf[i] = 0.0;
				m_dMdf[i] = 0.0;
			}
			else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
			{
				int iPart = (i < 4 ? 1 : 0);	// I=1, J=0.
				m_dVdu[i] = StrwMaxData.dblForce[iPart][2] * 1.0;
				m_dMdu[i] = StrwMaxData.dblForce[iPart][4] * 1.0;
				m_dVdf[i] = 0.0;
				m_dMdf[i] = 0.0;
			}
			else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
			{
				m_dVdu[i] = 0.0;
				m_dMdu[i] = 0.0;
				m_dVdf[i] = 0.0;
				m_dMdf[i] = 0.0;
			}
			else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Type.
			{
				m_dVdu[i] = StrpMaxData.dblForce[i][2] * 1.0;
				m_dMdu[i] = StrpMaxData.dblForce[i][4] * 1.0;
				m_dVdf[i] = 0.0;
				m_dMdf[i] = 0.0;
			}
		}
	}
	return TRUE;
}

// Add by ZINU.('05.05.03). Consider SRSS, ENV (iAnalType==D_LCOM_CB_STEEL, D_LCOM_CB_CONCRETE, D_LCOM_CB_SRC).
BOOL CDgnForceCtrl::Get_LcomForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor, int iAnalType, BOOL bMaxLcom)
{
	if (iLcaseKey == 0) { ASSERT(0); return FALSE; }
	if (dFactor == 0.0) { ASSERT(0); return FALSE; }
	if (iAnalType == 0) { ASSERT(0); return FALSE; }
	// Get Force Data by Load Case.
	int iLcaseType = 0;
	switch (iAnalType)
	{
	case D_LCOM_CB_STEEL:       iLcaseType = D_LOADCASE_COMB_STEEL;       break;
	case D_LCOM_CB_CFSTEEL:     iLcaseType = D_LOADCASE_COMB_CFSTEEL;     break;
	case D_LCOM_CB_CONCRETE:    iLcaseType = D_LOADCASE_COMB_CONCRETE;    break;
	case D_LCOM_CB_SRC:         iLcaseType = D_LOADCASE_COMB_SRC;         break;
	case D_LCOM_CB_ALUMINUM:    iLcaseType = D_LOADCASE_COMB_ALUMINUM;    break;
	case D_LCOM_CB_STLCOMP:     iLcaseType = D_LOADCASE_COMB_STLCOMP;     break;
	case D_LCOM_CB_SEISMIC:     iLcaseType = D_LOADCASE_COMB_SEISMIC;     break;
	case D_LCOM_CB_LINEAR_EVAL: iLcaseType = D_LOADCASE_COMB_LINEAR_EVAL; break;
	default: ASSERT(0); break;
	}

	// Change by ZINU.('07.07.09). NO:2903, Distinguish Envelope from SRSS.
	BOOL bLcaseEQ = GetLoadCombType(iLcaseKey, iAnalType) == 3 ? TRUE : FALSE;	// SRSS.
	Get_EqFactorForLcase(bLcaseEQ);

	// Change by ZINU.('02.11.26).
	T_STRB_D  StrbMaxData, StrbMinData;
	T_STRT_D  StrtMaxData, StrtMinData;
	T_STRW_D  StrwMaxData, StrwMinData;
	T_STRP_DL StrpMaxData, StrpMinData;
	if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
	{
		StrbMaxData.Initialize();
		StrbMinData.Initialize();
		GetStrbDgn(iElemNo, iLcaseType, iLcaseKey, &StrbMaxData, &StrbMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
	{
		StrtMaxData.Initialize();
		StrtMinData.Initialize();
		GetStrtDgn(iElemNo, iLcaseType, iLcaseKey, &StrtMaxData, &StrtMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
	{
		StrwMaxData.Initialize();
		StrwMinData.Initialize();
		GetStrwDgn(iElemNo, iLcaseType, iLcaseKey, &StrwMaxData, &StrwMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Type.
	{
		StrpMaxData.Initialize();
		StrpMinData.Initialize();
		GetStrpDgn(iElemNo, iLcaseType, iLcaseKey, &StrpMaxData, &StrpMinData);
	}

	// Save Forces and Moments by Position.
	int iPosStaNo = 0;
	int iPosTotNo = 0;
	if (iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
	else
	{
		iPosStaNo = iPosiNo;
		iPosTotNo = iPosiNo + 1;
	}
	// Change by ZINU.('05.06.16). Move wg_cmd/CLoadCombCtrl.
/*
	// Add by ZINU.('05.05.12). Special Load Combination Factor.
	if(m_bExistEL)	dFactor *= Get_SpecialLcomOmega(iElemNo, iLcaseKey, iAnalType);
*/

	T_STLD_D Lcase;
	Lcase.Initialize();
	m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, Lcase);
	BOOL bIsJUD = m_pDgnDataCtrl->IsJUD();

	const T_STRB_D&  CurStrb = bMaxLcom ? StrbMaxData : StrbMinData;
	const T_STRW_D&  CurStrw = bMaxLcom ? StrwMaxData : StrwMinData;
	const T_STRP_DL& CurStrp = bMaxLcom ? StrpMaxData : StrpMinData;

	double dScaleUpFactor = 1.0;
	for (int i = iPosStaNo; i < iPosTotNo; i++)
	{
		if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType)) // Beam Type.
		{
			double dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T;
			dExFactor_P = dExFactor_V = dExFactor_M = dExFactor_T = dFactor;
			if (bIsJUD) { GetExpandEqFactor(iElemNo, i, Lcase.LoadCaseType, dFactor, dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T); } // PMS:4215 - 일본2차설계시 1차설계 지진하중 할증
			// End Moments.
			if (!Get_BeamEndMoment(_T("R"), i, dExFactor_M*dScaleUpFactor*m_dSueqLcasM*m_dCodeLcasM, bMaxLcom, TRUE, iLcaseType, iLcaseKey, (bMaxLcom ? &StrbMaxData : &StrbMinData)))	return FALSE;
			// Forces, Moments.
			if (m_bExistLctb)
			{
				// Seungjun-20121231 : iGen 비선형 부재의 경우도 할증계수 적용
				dExFactor_P = bIsJUD ? fabs(dExFactor_P) : 1.0;
				dExFactor_V = bIsJUD ? fabs(dExFactor_V) : 1.0;
				dExFactor_M = bIsJUD ? fabs(dExFactor_M) : 1.0;
				dExFactor_T = bIsJUD ? fabs(dExFactor_T) : 1.0;

				_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
				if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
				double dFxx = LctbPos.dRemnFxx * dExFactor_P * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN;
				double dFyy = LctbPos.dRemnFyy * dExFactor_V * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV;
				double dFzz = LctbPos.dRemnFzz * dExFactor_V * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV;
				double dBMx = LctbPos.dRemnBMx * dExFactor_T * dScaleUpFactor;
				double dBMy = LctbPos.dRemnBMy * dExFactor_M * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM;
				double dBMz = LctbPos.dRemnBMz * dExFactor_M * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM;
				// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
				double dMbi = LctbPos.dRemnMbi * dScaleUpFactor;  //!!! 부재력 조정계수 고려범위 확인 요망.
				double dTst = LctbPos.dRemnTst * dScaleUpFactor; //!!!
				double dTwp = LctbPos.dRemnTwp * dScaleUpFactor; //!!!
				// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
				double dFyyOrg = LctbPos.dRemnFyy * dExFactor_V * dScaleUpFactor * m_dSueqLcasV;
				double dFzzOrg = LctbPos.dRemnFzz * dExFactor_V * dScaleUpFactor * m_dSueqLcasV;

				if (IsDeadLoadType(_T(""), iAnalType, iLcaseKey))
				{
					AddDeadSum(i, dFxx, dFyy, dFzz, dBMx, dBMy, dBMz, dMbi, dTst, dTwp);
				}
				else
				{
					AddRemnantSum(i, dFxx, dFyy, dFzz, dBMx, dBMy, dBMz, dMbi, dTst, dTwp, dFyyOrg, dFzzOrg);
				}
			}
			else
			{
				double dFactP = dExFactor_P * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN;
				double dFactV = dExFactor_V * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV;
				double dFactT = dExFactor_T * dScaleUpFactor;
				double dFactM = dExFactor_M * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM;
				if (IsDeadLoadType(_T(""), iAnalType, iLcaseKey))
				{
					AddBeamForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrbMaxData, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
					AddBeamForceDof7(i, m_MbiD, m_TstD, m_TwpD, StrbMaxData); //!!! 부재력 조정계수 고려범위 확인 요망.
				}
				else
				{
					AddBeamForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, CurStrb, dFactP, dFactV, dFactV, dFactT, dFactM, dFactM);
					// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
					AddBeamForceDof7(i, m_MbiR, m_TstR, m_TwpR, CurStrb, dScaleUpFactor, dScaleUpFactor, dScaleUpFactor);

					// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
					int iPart = 0;
					int iKind = 0;
					int iKind7 = 0;
					GetAnalForceComponentIndex(i, iPart, iKind, iKind7);
					m_FyyOrgR[i] += CurStrb.dblForce[iPart][iKind + 1] * dExFactor_V * dScaleUpFactor * m_dSueqLcasV;
					m_FzzOrgR[i] += CurStrb.dblForce[iPart][iKind + 2] * dExFactor_V * dScaleUpFactor * m_dSueqLcasV;
				}
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType)) // Wall Type.
		{
			// End Moments.
			if (!Get_WallEndMoment(_T("R"), i, dFactor*dScaleUpFactor*m_dSueqLcasM*m_dCodeLcasM, TRUE, iLcaseKey, (bMaxLcom ? &StrwMaxData : &StrwMinData)))	return FALSE;
			// Forces, Moments.
			if (m_bExistLctb)
			{
				_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
				if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
				m_FxxR[i] += LctbPos.dRemnFxx * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * m_dNTC08LcasN;  // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.
				m_FyyR[i] += LctbPos.dRemnFyy * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV;
				m_FzzR[i] += LctbPos.dRemnFzz * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV;
				m_BMxR[i] += LctbPos.dRemnBMx * dScaleUpFactor;
				m_BMyR[i] += LctbPos.dRemnBMy * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM;
				m_BMzR[i] += LctbPos.dRemnBMz * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM;
				// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
				m_FyyOrgR[i] += LctbPos.dRemnFyy * dScaleUpFactor * m_dSueqLcasV;
				m_FzzOrgR[i] += LctbPos.dRemnFzz * dScaleUpFactor * m_dSueqLcasV;
			}
			else
			{
				double dFactP = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN * m_dNTC08LcasN;  // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.
				double dFactV = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV;
				double dFactT = dFactor * dScaleUpFactor;
				double dFactM = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM;
				AddWallForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, CurStrw, dFactP, dFactV, dFactV, dFactT, dFactM, dFactM);
				// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
				int iPart = (i < 4 ? 1 : 0);	// I=1, J=0.
				m_FyyOrgR[i] += CurStrw.dblForce[iPart][1] * dFactor * dScaleUpFactor * m_dSueqLcasV;
				m_FzzOrgR[i] += CurStrw.dblForce[iPart][2] * dFactor * dScaleUpFactor * m_dSueqLcasV;
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType)) // Truss Type.
		{
			// End Moments.
			if (!Get_TrussEndMoment(_T("R"), i))	return FALSE;
			// Forces, Moments.
			if (m_bExistLctb)
			{
				_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
				if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
				m_FxxR[i] += LctbPos.dRemnFxx * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN;
			}
			else
			{
				m_FxxR[i] += (bMaxLcom ? StrtMaxData.dblForce[i / 4] : StrtMinData.dblForce[i / 4]) * dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN;
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Plate.
		{
			// End Moments.
			if (!Get_PlateEndMoment(_T("R"), i))	return FALSE;
			if (m_bExistLctb)
			{
				_DGN_LCTB_POS LctbPos; LctbPos.Initialize();
				if (!m_arRemnPos[i].Lookup(iLcaseKey, LctbPos))	ASSERT(0);
				if (i < 4)
				{//component : 0:Fx, 1:Fy, 2:Fz, 3:Mx, 4:My, 5:Mz
					m_FxxR[i] += LctbPos.dRemnFxx * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN;
					m_FyyR[i] += LctbPos.dRemnFyy * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN;
					m_FzzR[i] += LctbPos.dRemnFzz * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV;
					m_BMxR[i] += LctbPos.dRemnBMx * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM;
					m_BMyR[i] += LctbPos.dRemnBMy * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM;
					m_BMzR[i] += LctbPos.dRemnBMz * dScaleUpFactor;
					// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
					m_FzzOrgR[i] += LctbPos.dRemnFzz * dScaleUpFactor * m_dSueqLcasV;
				}
				// Unit Forces (cent + 4 node)
				if (i < 5)
				{//component : 14:Fxx, 16:Fyy, 17:Fxy, 20:Mxx, 21:Myy, 26:Vxx,  27:Vyy
					m_FxxR_UF[i] += LctbPos.dRemnFxxUF * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN;
					m_FyyR_UF[i] += LctbPos.dRemnFyyUF * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN;
					m_FxyR_UF[i] += LctbPos.dRemnFxyUF * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV;
					m_VxxR_UF[i] += LctbPos.dRemnVxxUF * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV;
					m_VyyR_UF[i] += LctbPos.dRemnVyyUF * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV;
					m_MxxR_UF[i] += LctbPos.dRemnMxxUF * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM;
					m_MyyR_UF[i] += LctbPos.dRemnMyyUF * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM;
					m_MxyR_UF[i] += LctbPos.dRemnMxyUF * dScaleUpFactor;
					// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
					m_FxyUFOrgR[i] += LctbPos.dRemnFxyUF * dScaleUpFactor * m_dSueqLcasV;
					m_VxxUFOrgR[i] += LctbPos.dRemnVxxUF * dScaleUpFactor * m_dSueqLcasV;
					m_VyyUFOrgR[i] += LctbPos.dRemnVyyUF * dScaleUpFactor * m_dSueqLcasV;
				}
			}
			else
			{
				double dFactP = dFactor * dScaleUpFactor * m_dSueqLcasN * m_dCodeLcasN;
				double dFactV = dFactor * dScaleUpFactor * m_dSueqLcasV * m_dCodeLcasV;
				double dFactM = dFactor * dScaleUpFactor * m_dSueqLcasM * m_dCodeLcasM;

				if (i < 4)
				{
					AddPlatForce(i, m_FxxR, m_FyyR, m_FzzR, m_BMxR, m_BMyR, m_BMzR, CurStrp, dFactP, dFactP, dFactV, dFactM, dFactM, dFactor);
					// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
					m_FzzOrgR[i] += CurStrp.dblForce[i][2] * dFactor * dScaleUpFactor * m_dSueqLcasV;
				}
				// Unit Forces (cent + 4 node)
				if (i < 5)
				{
					AddPlatForceUF(i, m_FxxR_UF, m_FyyR_UF, m_FxyR_UF, m_VxxR_UF, m_VyyR_UF, m_MxxR_UF, m_MyyR_UF, m_MxyR_UF, CurStrp,
						dFactP, dFactP, dFactV, dFactV, dFactV, dFactM, dFactM, dFactor);
					// Add by SHIN.('07.06.28). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
					m_FxyUFOrgR[i] += CurStrp.dblStress[i][16] * dFactor * dScaleUpFactor * m_dSueqLcasV;
					m_VxxUFOrgR[i] += CurStrp.dblStress[i][26] * dFactor * dScaleUpFactor * m_dSueqLcasV;
					m_VyyUFOrgR[i] += CurStrp.dblStress[i][27] * dFactor * dScaleUpFactor * m_dSueqLcasV;
				}
			}
		}
	}

	return TRUE;
}

BOOL CDgnForceCtrl::Get_CraneForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor, BOOL bMaxLcom)
{
	if (iLcaseKey == 0) { ASSERT(0); return FALSE; }
	if (dFactor == 0.0) { ASSERT(0); return FALSE; }
	int iLcaseType = D_LOADCASE_CRANE;

	T_STRB_D  StrbMaxData, StrbMinData;
	T_STRT_D  StrtMaxData, StrtMinData;
	T_STRW_D  StrwMaxData, StrwMinData;
	T_STRP_DL StrpMaxData, StrpMinData;
	if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
	{
		StrbMaxData.Initialize();
		StrbMinData.Initialize();
		GetStrbDgn(iElemNo, iLcaseType, iLcaseKey, &StrbMaxData, &StrbMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
	{
		StrtMaxData.Initialize();
		StrtMinData.Initialize();
		GetStrtDgn(iElemNo, iLcaseType, iLcaseKey, &StrtMaxData, &StrtMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
	{
		StrwMaxData.Initialize();
		StrwMinData.Initialize();
		GetStrwDgn(iElemNo, iLcaseType, iLcaseKey, &StrwMaxData, &StrwMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Type.
	{
		StrpMaxData.Initialize();
		StrpMinData.Initialize();
		GetStrpDgn(iElemNo, iLcaseType, iLcaseKey, &StrpMaxData, &StrpMinData);
	}

	// Save Forces and Moments by Position.
	int iPosStaNo = 0;
	int iPosTotNo = 0;
	if (iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
	else
	{
		iPosStaNo = iPosiNo;
		iPosTotNo = iPosiNo + 1;
	}

	T_STLD_D Lcase;
	Lcase.Initialize();
	m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, Lcase);
	BOOL bIsJUD = m_pDgnDataCtrl->IsJUD();

	for (int i = iPosStaNo; i < iPosTotNo; i++)
	{
		if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
		{
			double dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T;
			dExFactor_P = dExFactor_V = dExFactor_M = dExFactor_T = dFactor;
			if (bIsJUD) { GetExpandEqFactor(iElemNo, i, Lcase.LoadCaseType, dFactor, dExFactor_P, dExFactor_V, dExFactor_M, dExFactor_T); } // PMS:4215 - 일본2차설계시 1차설계 지진하중 할증

			T_STRB_D StrbD = bMaxLcom ? StrbMaxData : StrbMinData;
			// End Moments.
			if (!Get_BeamEndMoment(_T("CN"), i, dExFactor_M, bMaxLcom, TRUE, iLcaseType, iLcaseKey, &StrbD))	return FALSE;

			if (IsDeadLoadType(_T(""), D_LCOM_CRANE, iLcaseKey))
			{
				AddBeamForce(i, m_FxxD, m_FyyD, m_FzzD, m_BMxD, m_BMyD, m_BMzD, StrbD, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
				AddBeamForceDof7(i, m_MbiD, m_TstD, m_TwpD, StrbD); //!!! 부재력 조정계수 고려범위 확인 요망.			
			}
			else
			{
				// Forces, Moments.
				AddBeamForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, StrbD, dExFactor_P, dExFactor_V, dExFactor_V, dExFactor_T, dExFactor_M, dExFactor_M);
				// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
				AddBeamForceDof7(i, m_MbiML, m_TstML, m_TwpML, StrbD); //!!! 부재력 조정계수 고려범위 확인 요망.			
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
		{
			T_STRW_D StrwD = bMaxLcom ? StrwMaxData : StrwMinData;
			// End Moments.
			if (!Get_WallEndMoment(_T("CN"), i, dFactor, TRUE, iLcaseKey, &StrwD))	return FALSE;
			// Forces, Moments. // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design. (m_dNTC08LcasN)
			AddWallForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, StrwD, dFactor*m_dNTC08LcasN, dFactor, dFactor, dFactor, dFactor, dFactor);
		}
		else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
		{
			T_STRT_D StrtD = bMaxLcom ? StrtMaxData : StrtMinData;
			// End Moments.
			if (!Get_TrussEndMoment(_T("CN"), i))	return FALSE;
			// Forces, Moments.
			m_FxxML[i] += StrtD.dblForce[i / 4] * dFactor;
		}
		else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Plate.
		{
			T_STRP_DL StrpD = bMaxLcom ? StrpMaxData : StrpMinData;
			// End Moments.		
			if (!Get_PlateEndMoment(_T("CN"), i))	return FALSE;
			// Forces, Moments. (4 node)
			if (i < 4)
			{
				AddPlatForce(i, m_FxxML, m_FyyML, m_FzzML, m_BMxML, m_BMyML, m_BMzML, StrpD, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
			}
			// Unit Forces (cent + 4 node)
			if (i < 5)
			{
				AddPlatForceUF(i, m_FxxML_UF, m_FyyML_UF, m_FxyML_UF, m_VxxML_UF, m_VyyML_UF, m_MxxML_UF, m_MyyML_UF, m_MxyML_UF, StrpD,
					dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor, dFactor);
			}
		}
	}

	return TRUE;
}

BOOL CDgnForceCtrl::Get_DeadLiveGB(int iElemNo, int iPosiNo)
{
	// Modify by GAY. PMS:4267. ('11.12.12). for GB50010-02 -> for China Code.
	BOOL bChinaCode = (m_iDgnCode == GB50010_02 || m_iDgnCode == GB50010_10 || m_iDgnCode == GB50010_19) ? TRUE : FALSE;
	BOOL bColm = m_pDgnDataCtrl->Get_DgnGenMbtp(iElemNo) == 1 ? TRUE : FALSE;
	BOOL bWall = m_pDoc->m_pAttrCtrl->IsWall(m_iElemType);
	if (!bColm && !bWall)        return FALSE;	// if not column nor wall, return False;
	if (!bChinaCode)             return FALSE;	// Only GB50010-02.-> Only China Code.

	CArray<T_STLD_K, T_STLD_K> aStldKeyList;
	m_pDoc->m_pPostCtrl->GetStldKeyList(aStldKeyList);
	for (int i = 0; i < aStldKeyList.GetSize(); i++)
	{
		int iLcaseType = D_LOADCASE_STATIC;
		T_STLD_K iLcaseKey = aStldKeyList.GetAt(i);
		// Dummy Force by (D+0.5L).
		// Get Load Case Type(D,L,E,W,...).
		T_STLD_D Lcase;
		Lcase.Initialize();
		m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, Lcase);
		// Only Dead, Live Load.  1.2*(D+0.5L).
		// Change by ZINU.('08.04.19). MQC:1776, 1.0, 0.5 -> 1.2*1.0, 1.2*0.5.
		double dFactor = 0.0;
		if (Lcase.LoadCaseType == _T("D"))				dFactor = 1.2*1.0;
		else if (Lcase.LoadCaseType == _T("L"))	dFactor = 1.2*0.5;
		else	continue;

		if (bWall)
		{
			// Wall Force.
			T_STRW_D StrwMaxData, StrwMinData;
			StrwMaxData.Initialize();
			StrwMinData.Initialize();
			GetStrwDgn(iElemNo, iLcaseType, iLcaseKey, &StrwMaxData, &StrwMinData);
			// Save Forces and Moments by Position.
			int iPosStaNo = 0;
			int iPosTotNo = 0;
			if (iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
			else
			{
				iPosStaNo = iPosiNo;
				iPosTotNo = iPosiNo + 1;
			}
			for (int j = iPosStaNo; j < iPosTotNo; j++)
			{
				int iPart = (j < 4 ? 1 : 0);	// I=1, J=0.
				m_FxxGB[j] += StrwMaxData.dblForce[iPart][0] * dFactor;
			}
		}
		else if (bColm)
		{
			// Column Force.
			T_STRB_D StrbMaxData, StrbMinData;
			StrbMaxData.Initialize();
			StrbMinData.Initialize();
			GetStrbDgn(iElemNo, iLcaseType, iLcaseKey, &StrbMaxData, &StrbMinData);
			// Save Forces and Moments by Position.
			int iPosStaNo = 0;
			int iPosTotNo = 0;
			if (iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
			else
			{
				iPosStaNo = iPosiNo;
				iPosTotNo = iPosiNo + 1;
			}
			for (int j = iPosStaNo; j < iPosTotNo; j++)
			{
				int iPart = (j < 4 ? j : j - 1);
				int iKind = (j < 4 ? 0 : 6);
				m_FxxGB[j] += StrbMaxData.dblForce[iPart][iKind + 0] * dFactor;
			}
		}
		else ASSERT(0);
	}
	return TRUE;
}

BOOL CDgnForceCtrl::Get_DeadLiveIS(int iElemNo, int iPosiNo)
{
	// Added by Maharshi for IS456 RC beam design only
	if( m_iDgnCode != IS456_2000 ) return FALSE;
	BOOL bWall = m_pDoc->m_pAttrCtrl->IsWall(m_iElemType);
	if(bWall) { return FALSE; }
	if(m_pDgnDataCtrl->Get_DgnGenMbtp(iElemNo)!=2) return FALSE;

	CArray<T_STLD_K, T_STLD_K> aStldKeyList;
	m_pDoc->m_pPostCtrl->GetStldKeyList(aStldKeyList);
	for(int i = 0; i < aStldKeyList.GetSize(); i++)
	{
		int iLcaseType = D_LOADCASE_STATIC;
		T_STLD_K iLcaseKey = aStldKeyList.GetAt(i);
		// Dummy Force by (D+0.5L).
		// Get Load Case Type(D,L,E,W,...).
		T_STLD_D Lcase;
		Lcase.Initialize();
		m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, Lcase);

		double dFactor = 0.0;
		if(Lcase.LoadCaseType == _T("D"))		dFactor = 1.2;
		else if(Lcase.LoadCaseType == _T("L"))	dFactor = 1.2;
		else if(Lcase.LoadCaseType == _T("LR")) dFactor = 1.2;
		else	continue;

		
		// Beam Force.
		T_STRB_D StrbMaxData, StrbMinData;
		StrbMaxData.Initialize();
		StrbMinData.Initialize();
		GetStrbDgn(iElemNo, iLcaseType, iLcaseKey, &StrbMaxData, &StrbMinData);
		// Save Forces and Moments by Position.
		int iPosStaNo = 0;
		int iPosTotNo = 0;
		if(iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
		else
		{
			iPosStaNo = iPosiNo;
			iPosTotNo = iPosiNo + 1;
		}
		for(int j = iPosStaNo; j < iPosTotNo; j++)
		{
			int iPart = ( j < 4 ? j : j - 1 );
			int iKind = ( j < 4 ? 2 : 8 );
			m_FzzIS[j] += StrbMaxData.dblForce[iPart][iKind + 0] * dFactor;
		}
	}
	return TRUE;
}

// Add by ZINU.('03.11.11). Get M1,M2 for Cb,Cmy,Cmz.
BOOL CDgnForceCtrl::Get_BeamEndMoment(CString strForceType, int iPosiNo, double dFactor, BOOL bMaxLcom, BOOL bGetLctb, int iLcaseType, int iLcaseKey, T_STRB_D* pStrbData, double* pRMFactor)
{
	//*/PROFILING(_T("Get_BeamEndMoment"));
	// End Moments.
	if (!m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType)) { ASSERT(0); return FALSE; }	// Only Beam Type.

	BOOL bIsJUD = m_pDgnDataCtrl->IsJUD(); // Add by SHIN - PMS:4215  
	int nIMJ = iPosiNo == 0 ? 0 : (iPosiNo == 4 ? 2 : 1);

	T_ELEM_K StaElemKeyPrev = 0, EndElemKeyPrev = 0;
	T_ELEM_K StaElemKeyNext = 0, EndElemKeyNext = 0;
	T_STRB_D StrbD;
	for (int j = 0; j < 2; j++)	// 0-y, 1-z.
	{
		// Change by ZINU.('05.07.14). Separate None from Memb/Hcbm.
		BOOL bElemOnly = !(m_pDgnDataCtrl->m_pMembCtrl->m_bMembAssigned || m_pDgnDataCtrl->m_pMembCtrl->m_bHcbmAssigned);
		if (!bElemOnly && m_apElemK_z[0].GetSize() == 0) bElemOnly = TRUE;
		if (bElemOnly)	StrbD = *pStrbData;
		else
		{
			int iElemCount = m_pDgnDataCtrl->m_pMembCtrl->GetElemCountByMembPosi(m_apElemK_z[0].GetAt(0), iPosiNo);
			// zList for My, yList for Mz.
			StaElemKeyPrev = (j == 0 ? m_apElemK_z[0].GetAt(iElemCount) : m_apElemK_y[0].GetAt(iElemCount));
			EndElemKeyPrev = (j == 0 ? m_apElemK_z[1].GetAt(iElemCount) : m_apElemK_y[1].GetAt(iElemCount));
			if (StaElemKeyPrev != StaElemKeyNext || EndElemKeyPrev != EndElemKeyNext)
			{
				T_STRB_D StrMaxData, StrMinData;
				StrMaxData.Initialize();
				StrMinData.Initialize();
				GetStrbDgn(StaElemKeyPrev, EndElemKeyPrev, iLcaseType, iLcaseKey, &StrMaxData, &StrMinData);
				StrbD = (bMaxLcom ? StrMaxData : StrMinData);
			}
			StaElemKeyNext = StaElemKeyPrev;
			EndElemKeyNext = EndElemKeyPrev;
		}
		///////////////////////
		// Get Moments.
		double dMy[CONST_DGN_iPOSNUM] = { StrbD.dblForce[0][4], StrbD.dblForce[1][4], StrbD.dblForce[2][4], StrbD.dblForce[3][4], StrbD.dblForce[3][10] };
		double dMz[CONST_DGN_iPOSNUM] = { StrbD.dblForce[0][5], StrbD.dblForce[1][5], StrbD.dblForce[2][5], StrbD.dblForce[3][5], StrbD.dblForce[3][11] };

		////////////////////////////////////////////////////////////////////////// Add by SHIN //
		if (bIsJUD && j == 1)
		{
			double dLbMax = 0.0;
			CArray<double, double> arSpace;
			BOOL bIsBlcf = FALSE;
			T_ELEM_K BlcfElemK = 0;
			for (int k = 0; k < 2; k++)
			{
				for (int m = 0; m < m_apElemK_y[0].GetSize(); m++)
				{
					if (m_pDgnDataCtrl->m_pMembCtrl->Get_Blcf_SpaceList(m_apElemK_y[k].GetAt(m), arSpace, dLbMax, nIMJ)) { BlcfElemK = m_apElemK_y[k].GetAt(m);  bIsBlcf = TRUE;  break; }
				}
				if (BlcfElemK != 0) break;
			}

			if (bIsBlcf)
			{
				double dOffsetI = 0.0, dOffsetJ = 0.0;

				T_LENG_D LengD; LengD.Initialize();
				double dLen = 0., dNetLy = 0., dNetLz = 0.;
				double dOffSetLy[2] = { 0. }, dOffSetLz[2] = { 0. }; // 0:i, 1:j				
				if (m_pDgnDataCtrl->Get_DgnGenLeng(BlcfElemK, 0, LengD, dLen, dNetLy, dNetLz, dOffSetLy, dOffSetLz, 1, 1))
				{
					dOffsetI = fabs(dOffSetLy[0]);
				}
				if (m_pDgnDataCtrl->Get_DgnGenLeng(BlcfElemK, 4, LengD, dLen, dNetLy, dNetLz, dOffSetLy, dOffSetLz, 1, 1))
				{
					dOffsetJ = fabs(dOffSetLy[1]);
				}

				double dMy_Memb[CONST_DGN_iPOSNUM] = { pStrbData->dblForce[0][4], pStrbData->dblForce[1][4], pStrbData->dblForce[2][4], pStrbData->dblForce[3][4], pStrbData->dblForce[3][10] };
				double dMz_Memb[CONST_DGN_iPOSNUM] = { pStrbData->dblForce[0][5], pStrbData->dblForce[1][5], pStrbData->dblForce[2][5], pStrbData->dblForce[3][5], pStrbData->dblForce[3][11] };
				double dMLength[CONST_DGN_iPOSNUM] = { 0.0 };

				double dLength = 0.0;
				for (int k = 0; k < arSpace.GetSize(); k++) dLength += arSpace[k];
				for (int k = 0; k < CONST_DGN_iPOSNUM; k++) dMLength[k] = dOffsetI + double(k)*(dLength - dOffsetI - dOffsetJ) / 4.0;
				double dPosiLength = dMLength[iPosiNo];

				double dLenghtS = 0.0, dLengthE = 0.0;
				double dCurrLength = 0.0;
				for (int k = 0; k < arSpace.GetSize(); k++)
				{
					dCurrLength += arSpace[k];
					if (dCurrLength >= dPosiLength)
					{
						dLenghtS = dCurrLength - arSpace[k];  dLengthE = dCurrLength;  break;
					}
				}
				for (int k = 0; k < CONST_DGN_iPOSNUM; k++)
				{
					dCurrLength = dLenghtS + double(k)*(dLengthE - dLenghtS) / 4.0;
					dMy[k] = Get_MomentInterpolate(dMy_Memb, dMLength, dCurrLength);
					dMz[k] = Get_MomentInterpolate(dMz_Memb, dMLength, dCurrLength);
				}
			}
		}

		// Set Moments.
		for (int k = 0; k < CONST_DGN_iPOSNUM; k++)
		{
			dMy[k] *= dFactor;
			dMz[k] *= dFactor;
			if (pRMFactor)
			{
				dMy[k] *= pRMFactor[4];
				dMz[k] *= pRMFactor[5];
			}
			// Change by ZINU.('05.07.30). Check !(m_bExistLctb && bGetLctb).
			if (!(m_bExistLctb && bGetLctb))
			{
				// Use m_apElemK_y for Cb.
				if (strForceType == _T("D"))
				{
					if (j == 0) { m_BMyeD[0][iPosiNo][k] += dMy[k]; m_BMzeD[1][iPosiNo][k] += dMz[k]; }
					else { m_BMzeD[0][iPosiNo][k] += dMz[k]; m_BMyeD[1][iPosiNo][k] += dMy[k]; }
				}
				else if (strForceType == _T("L"))
				{
					if (j == 0) { m_BMyeL[0][iPosiNo][k] += dMy[k]; m_BMzeL[1][iPosiNo][k] += dMz[k]; }
					else { m_BMzeL[0][iPosiNo][k] += dMz[k]; m_BMyeL[1][iPosiNo][k] += dMy[k]; }
				}
				else if (strForceType == _T("R"))
				{
					if ( !m_bExistLctb )
					{
						if ( j == 0 ) { m_BMyeR[0][iPosiNo][k] += dMy[k]; m_BMzeR[1][iPosiNo][k] += dMz[k]; }
						else { m_BMzeR[0][iPosiNo][k] += dMz[k]; m_BMyeR[1][iPosiNo][k] += dMy[k]; }
					}							
				}
				else if (strForceType == _T("ML"))
				{
					if (j == 0) { m_BMyeML[0][iPosiNo][k] += dMy[k]; m_BMzeML[1][iPosiNo][k] += dMz[k]; }
					else { m_BMzeML[0][iPosiNo][k] += dMz[k]; m_BMyeML[1][iPosiNo][k] += dMy[k]; }
				}
				else if (strForceType == _T("TP"))
				{
					if (j == 0) { m_BMyeTP[0][iPosiNo][k] += dMy[k]; m_BMzeTP[1][iPosiNo][k] += dMz[k]; }
					else { m_BMzeTP[0][iPosiNo][k] += dMz[k]; m_BMyeTP[1][iPosiNo][k] += dMy[k]; }
				}
				else if (strForceType == _T("TS"))
				{
					if (j == 0) { m_BMyeTS[0][iPosiNo][k] += dMy[k]; m_BMzeTS[1][iPosiNo][k] += dMz[k]; }
					else { m_BMzeTS[0][iPosiNo][k] += dMz[k]; m_BMyeTS[1][iPosiNo][k] += dMy[k]; }
				}
				else if (strForceType == _T("CN"))
				{
					if (j == 0) { m_BMyeML[0][iPosiNo][k] += dMy[k]; m_BMzeML[1][iPosiNo][k] += dMz[k]; }
					else { m_BMzeML[0][iPosiNo][k] += dMz[k]; m_BMyeML[1][iPosiNo][k] += dMy[k]; }
				}
				else if (strForceType == _T("USER"))
				{
					if (j == 0) { m_BMyeLU[0][iPosiNo][k] += dMy[k]; m_BMzeLU[1][iPosiNo][k] += dMz[k]; }
					else { m_BMzeLU[0][iPosiNo][k] += dMz[k]; m_BMyeLU[1][iPosiNo][k] += dMy[k]; }
				}
				else	ASSERT(0);
			}

			// Add by ZINU.('04.02.13). For Load Contributions.
			if (m_bExistLctb)
			{
				_DGN_LCTB_END LctbEnd;
				if (bGetLctb)	// Get Force.
				{
					if (strForceType == _T("R"))	// Remnant.
					{
						LctbEnd.Initialize();
						if (!m_arRemnEnd[iPosiNo][k].Lookup(iLcaseKey, LctbEnd))	ASSERT(0);
						if (j == 0)
						{
							m_BMyeR[0][iPosiNo][k] += LctbEnd.dRemnBMye[0];
							m_BMzeR[1][iPosiNo][k] += LctbEnd.dRemnBMze[1];
						}
						else
						{
							m_BMzeR[0][iPosiNo][k] += LctbEnd.dRemnBMze[0];
							m_BMyeR[1][iPosiNo][k] += LctbEnd.dRemnBMye[1];
						}
					}
				}
				else	// Set Force.
				{
					if (strForceType == _T("R"))	// Remnant.
					{
						LctbEnd.Initialize();
						m_arRemnEnd[iPosiNo][k].Lookup(iLcaseKey, LctbEnd);
						if (j == 0) { LctbEnd.dRemnBMye[0] = dMy[k]; LctbEnd.dRemnBMze[1] = dMz[k]; }
						else { LctbEnd.dRemnBMze[0] = dMz[k]; LctbEnd.dRemnBMye[1] = dMy[k]; }
						m_arRemnEnd[iPosiNo][k].SetAt(iLcaseKey, LctbEnd);
					}
					else	// Dead(D,TP,TS), Live(L,ML).
					{
						if (j == 0) { m_BMyeLctb[0][iPosiNo][k] -= dMy[k]; m_BMzeLctb[1][iPosiNo][k] -= dMz[k]; }
						else { m_BMzeLctb[0][iPosiNo][k] -= dMz[k]; m_BMyeLctb[1][iPosiNo][k] -= dMy[k]; }
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL CDgnForceCtrl::Get_WallEndMoment(CString strForceType, int iPosiNo, double dFactor, BOOL bGetLctb, int iLcaseKey, T_STRW_D* pStrwData)
{
	// End Moments.
	if (!m_pDoc->m_pAttrCtrl->IsWall(m_iElemType)) { ASSERT(0); return FALSE; }	// Only Wall Type.

	// Get End Moments.
	double dMyi = pStrwData->dblForce[1][4] * dFactor;
	double dMzi = pStrwData->dblForce[1][5] * dFactor;
	double dMyj = pStrwData->dblForce[0][4] * dFactor;
	double dMzj = pStrwData->dblForce[0][5] * dFactor;
	// Moments.
	// Change by ZINU.('05.07.30). Check !(m_bExistLctb && bGetLctb).
	if (!(m_bExistLctb && bGetLctb))
	{
		if (strForceType == _T("D"))
		{
			m_BMyeD[0][iPosiNo][0] += dMyi;  m_BMyeD[1][iPosiNo][0] += dMyi;
			m_BMyeD[0][iPosiNo][4] += dMyj;  m_BMyeD[1][iPosiNo][4] += dMyj;
			m_BMzeD[0][iPosiNo][0] += dMzi;  m_BMzeD[1][iPosiNo][0] += dMzi;
			m_BMzeD[0][iPosiNo][4] += dMzj;  m_BMzeD[1][iPosiNo][4] += dMzj;
		}
		else if (strForceType == _T("L"))
		{
			m_BMyeL[0][iPosiNo][0] += dMyi;  m_BMyeL[1][iPosiNo][0] += dMyi;
			m_BMyeL[0][iPosiNo][4] += dMyj;  m_BMyeL[1][iPosiNo][4] += dMyj;
			m_BMzeL[0][iPosiNo][0] += dMzi;  m_BMzeL[1][iPosiNo][0] += dMzi;
			m_BMzeL[0][iPosiNo][4] += dMzj;  m_BMzeL[1][iPosiNo][4] += dMzj;
		}
		else if (strForceType == _T("R"))
		{
			m_BMyeR[0][iPosiNo][0] += dMyi;  m_BMyeR[1][iPosiNo][0] += dMyi;
			m_BMyeR[0][iPosiNo][4] += dMyj;  m_BMyeR[1][iPosiNo][4] += dMyj;
			m_BMzeR[0][iPosiNo][0] += dMzi;  m_BMzeR[1][iPosiNo][0] += dMzi;
			m_BMzeR[0][iPosiNo][4] += dMzj;  m_BMzeR[1][iPosiNo][4] += dMzj;
		}
		else if (strForceType == _T("ML"))
		{
			m_BMyeML[0][iPosiNo][0] += dMyi;  m_BMyeML[1][iPosiNo][0] += dMyi;
			m_BMyeML[0][iPosiNo][4] += dMyj;  m_BMyeML[1][iPosiNo][4] += dMyj;
			m_BMzeML[0][iPosiNo][0] += dMzi;  m_BMzeML[1][iPosiNo][0] += dMzi;
			m_BMzeML[0][iPosiNo][4] += dMzj;  m_BMzeML[1][iPosiNo][4] += dMzj;
		}
		else if (strForceType == _T("TP"))
		{
			m_BMyeTP[0][iPosiNo][0] += dMyi;  m_BMyeTP[1][iPosiNo][0] += dMyi;
			m_BMyeTP[0][iPosiNo][4] += dMyj;  m_BMyeTP[1][iPosiNo][4] += dMyj;
			m_BMzeTP[0][iPosiNo][0] += dMzi;  m_BMzeTP[1][iPosiNo][0] += dMzi;
			m_BMzeTP[0][iPosiNo][4] += dMzj;  m_BMzeTP[1][iPosiNo][4] += dMzj;
		}
		else if (strForceType == _T("TS"))
		{
			m_BMyeTS[0][iPosiNo][0] += dMyi;  m_BMyeTS[1][iPosiNo][0] += dMyi;
			m_BMyeTS[0][iPosiNo][4] += dMyj;  m_BMyeTS[1][iPosiNo][4] += dMyj;
			m_BMzeTS[0][iPosiNo][0] += dMzi;  m_BMzeTS[1][iPosiNo][0] += dMzi;
			m_BMzeTS[0][iPosiNo][4] += dMzj;  m_BMzeTS[1][iPosiNo][4] += dMzj;
		}
		else if (strForceType == _T("CN"))
		{
			m_BMyeML[0][iPosiNo][0] += dMyi;  m_BMyeML[1][iPosiNo][0] += dMyi;
			m_BMyeML[0][iPosiNo][4] += dMyj;  m_BMyeML[1][iPosiNo][4] += dMyj;
			m_BMzeML[0][iPosiNo][0] += dMzi;  m_BMzeML[1][iPosiNo][0] += dMzi;
			m_BMzeML[0][iPosiNo][4] += dMzj;  m_BMzeML[1][iPosiNo][4] += dMzj;
		}
		else if (strForceType == _T("USER"))
		{
			m_BMyeLU[0][iPosiNo][0] += dMyi;  m_BMyeLU[1][iPosiNo][0] += dMyi;
			m_BMyeLU[0][iPosiNo][4] += dMyj;  m_BMyeLU[1][iPosiNo][4] += dMyj;
			m_BMzeLU[0][iPosiNo][0] += dMzi;  m_BMzeLU[1][iPosiNo][0] += dMzi;
			m_BMzeLU[0][iPosiNo][4] += dMzj;  m_BMzeLU[1][iPosiNo][4] += dMzj;
		}
		else	ASSERT(0);
	}
	// Add by ZINU.('04.02.13). For Load Contributions.
	if (m_bExistLctb)
	{
		_DGN_LCTB_END LctbEnd;
		if (bGetLctb)	// Get Force.
		{
			if (strForceType == _T("R"))	// Remnant.
			{
				LctbEnd.Initialize();
				if (!m_arRemnEnd[iPosiNo][0].Lookup(iLcaseKey, LctbEnd))	ASSERT(0);
				m_BMyeR[0][iPosiNo][0] += LctbEnd.dRemnBMye[0] - dMyi;
				m_BMyeR[1][iPosiNo][0] += LctbEnd.dRemnBMye[1] - dMyi;
				m_BMzeR[0][iPosiNo][0] += LctbEnd.dRemnBMze[0] - dMzi;
				m_BMzeR[1][iPosiNo][0] += LctbEnd.dRemnBMze[1] - dMzi;
				LctbEnd.Initialize();
				if (!m_arRemnEnd[iPosiNo][4].Lookup(iLcaseKey, LctbEnd))	ASSERT(0);
				m_BMyeR[0][iPosiNo][4] += LctbEnd.dRemnBMye[0] - dMyj;
				m_BMyeR[1][iPosiNo][4] += LctbEnd.dRemnBMye[1] - dMyj;
				m_BMzeR[0][iPosiNo][4] += LctbEnd.dRemnBMze[0] - dMzj;
				m_BMzeR[1][iPosiNo][4] += LctbEnd.dRemnBMze[1] - dMzj;
			}
		}
		else	// Set Force.
		{
			if (strForceType == _T("R"))	// Remnant.
			{
				LctbEnd.Initialize();
				LctbEnd.dRemnBMye[0] = dMyi; LctbEnd.dRemnBMye[1] = dMyi;
				LctbEnd.dRemnBMze[0] = dMzi; LctbEnd.dRemnBMze[1] = dMzi;
				m_arRemnEnd[iPosiNo][0].SetAt(iLcaseKey, LctbEnd);
				LctbEnd.Initialize();
				LctbEnd.dRemnBMye[0] = dMyj; LctbEnd.dRemnBMye[1] = dMyj;
				LctbEnd.dRemnBMze[0] = dMzj; LctbEnd.dRemnBMze[1] = dMzj;
				m_arRemnEnd[iPosiNo][4].SetAt(iLcaseKey, LctbEnd);
			}
			else	// Dead(D,TP,TS), Live(L,ML).
			{
				m_BMyeLctb[0][iPosiNo][0] -= dMyi;  m_BMyeLctb[1][iPosiNo][0] -= dMyi;
				m_BMyeLctb[0][iPosiNo][4] -= dMyj;  m_BMyeLctb[1][iPosiNo][4] -= dMyj;
				m_BMzeLctb[0][iPosiNo][0] -= dMzi;  m_BMzeLctb[1][iPosiNo][0] -= dMzi;
				m_BMzeLctb[0][iPosiNo][4] -= dMzj;  m_BMzeLctb[1][iPosiNo][4] -= dMzj;
			}
		}
	}
	return TRUE;
}

BOOL CDgnForceCtrl::Get_TrussEndMoment(CString strForceType, int iPosiNo)
{
	// End Moments.
	if (!m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType)) { ASSERT(0); return FALSE; }	// Only Truss Type.
	// Not Sum Force.
	return TRUE;
}
BOOL CDgnForceCtrl::Get_PlateEndMoment(CString strForceType, int iPosiNo)
{
	// End Moments.
	if (!m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType)) { ASSERT(0); return FALSE; }	// Only Plate Type.
	// Not Sum Force.
	return TRUE;
}


void CDgnForceCtrl::AddBeamForce(int nPosi, double* adFx, double* adFy, double* adFz, double* adMx, double* adMy, double* adMz,
	const T_STRB_D& CurStrb, double dFactFx, double dFactFy, double dFactFz, double dFactMx, double dFactMy, double dFactMz)
{
	int nPart = 0;
	int nKind = 0;
	int nKind7 = 0;
	GetAnalForceComponentIndex(nPosi, nPart, nKind, nKind7);

	adFx[nPosi] += CurStrb.dblForce[nPart][nKind + 0] * dFactFx;
	adFy[nPosi] += CurStrb.dblForce[nPart][nKind + 1] * dFactFy;
	adFz[nPosi] += CurStrb.dblForce[nPart][nKind + 2] * dFactFz;
	adMx[nPosi] += CurStrb.dblForce[nPart][nKind + 3] * dFactMx;
	adMy[nPosi] += CurStrb.dblForce[nPart][nKind + 4] * dFactMy;
	adMz[nPosi] += CurStrb.dblForce[nPart][nKind + 5] * dFactMz;
}

void CDgnForceCtrl::AddStaticForceIS(int nPosi, double* adFx, double* adFy, double* adFz, double* adMx, double* adMy, double* adMz,
	const T_STRB_D& CurStrb, double dFactFx, double dFactFy, double dFactFz, double dFactMx, double dFactMy, double dFactMz)
{
	T_DSTL_D DstlD;
	DstlD.Initialize();
	m_pDgnDataCtrl->Get_DgnStlDstl(DstlD);
	if (m_pDgnDataCtrl->GetStlCodeEnum(DstlD.DesignCode) == IS800_2007) {
		if (nPosi == -1) {
			adFx[0] = 0; adFy[0] = 0; adFz[0] = 0; adMx[0] = 0; adMy[0] = 0; adMz[0] = 0;
			adFx[1] = 0; adFy[1] = 0; adFz[1] = 0; adMx[1] = 0; adMy[1] = 0; adMz[1] = 0;
			adFx[2] = 0; adFy[2] = 0; adFz[2] = 0; adMx[2] = 0; adMy[2] = 0; adMz[2] = 0;
			adFx[3] = 0; adFy[3] = 0; adFz[3] = 0; adMx[3] = 0; adMy[3] = 0; adMz[3] = 0;
			adFx[4] = 0; adFy[4] = 0; adFz[4] = 0; adMx[4] = 0; adMy[4] = 0; adMz[4] = 0;
		}
		else {
			int nPart = 0;
			int nKind = 0;
			int nKind7 = 0;
			GetAnalForceComponentIndex(nPosi, nPart, nKind, nKind7);

			adFx[nPosi] += CurStrb.dblForce[nPart][nKind + 0] * dFactFx;
			adFy[nPosi] += CurStrb.dblForce[nPart][nKind + 1] * dFactFy;
			adFz[nPosi] += CurStrb.dblForce[nPart][nKind + 2] * dFactFz;
			adMx[nPosi] += CurStrb.dblForce[nPart][nKind + 3] * dFactMx;
			adMy[nPosi] += CurStrb.dblForce[nPart][nKind + 4] * dFactMy;
			adMz[nPosi] += CurStrb.dblForce[nPart][nKind + 5] * dFactMz;
		}
	}
	
}


void CDgnForceCtrl::AddBeamForceDof7(int nPosi, double* adMbi, double* adTst, double* adTwp, const T_STRB_D& CurStrb, double dFactMbi, double dFactTst, double dFactTwp)
{
	int nPart = 0;
	int nKind = 0;
	int nKind7 = 0;
	GetAnalForceComponentIndex(nPosi, nPart, nKind, nKind7);

	adMbi[nPosi] += CurStrb.dblForce[nPart][nKind7 + 0] * dFactMbi;
	adTst[nPosi] += CurStrb.dblForce[nPart][nKind7 + 2] * dFactTst;
	adTwp[nPosi] += CurStrb.dblForce[nPart][nKind7 + 4] * dFactTwp;
}

void CDgnForceCtrl::AddWallForce(int nPosi, double* adFx, double* adFy, double* adFz, double* adMx, double* adMy, double* adMz,
	const T_STRW_D& CurStrw, double dFactFx, double dFactFy, double dFactFz, double dFactMx, double dFactMy, double dFactMz)
{
	int nPart = (nPosi < 4 ? 1 : 0);	// I=1, J=0.
	adFx[nPosi] += CurStrw.dblForce[nPart][0] * dFactFx;
	adFy[nPosi] += CurStrw.dblForce[nPart][1] * dFactFy;
	adFz[nPosi] += CurStrw.dblForce[nPart][2] * dFactFz;
	adMx[nPosi] += CurStrw.dblForce[nPart][3] * dFactMx;
	adMy[nPosi] += CurStrw.dblForce[nPart][4] * dFactMy;
	adMz[nPosi] += CurStrw.dblForce[nPart][5] * dFactMz;
}

void CDgnForceCtrl::AddPlatForce(int nPosi, double* adFx, double* adFy, double* adFz, double* adMx, double* adMy, double* adMz,
	const T_STRP_DL& Strp, double dFactFx, double dFactFy, double dFactFz, double dFactMx, double dFactMy, double dFactMz)
{
	//component : 0:Fx, 1:Fy, 2:Fz, 3:Mx, 4:My, 5:Mz
	adFx[nPosi] += Strp.dblForce[nPosi][0] * dFactFx;
	adFy[nPosi] += Strp.dblForce[nPosi][1] * dFactFy;
	adFz[nPosi] += Strp.dblForce[nPosi][2] * dFactFz;
	adMx[nPosi] += Strp.dblForce[nPosi][3] * dFactMx;
	adMy[nPosi] += Strp.dblForce[nPosi][4] * dFactMy;
	adMz[nPosi] += Strp.dblForce[nPosi][5] * dFactMz;
}

void CDgnForceCtrl::AddPlatForceUF(int nPosi, double* adFxx, double* adFyy, double* adFxy, double* adVxx, double* adVyy, double* adMxx, double* adMyy, double* adMxy,
	const T_STRP_DL& Strp, double dFactFxx, double dFactFyy, double dFactFxy, double dFactVxx, double dFactVyy, double dFactMxx, double dFactMyy, double dFactMxy)
{
	//component : 14:Fxx, 16:Fyy, 17:Fxy, 20:Mxx, 21:Myy, 26:Vxx,  27:Vyy
	adFxx[nPosi] += Strp.dblStress[nPosi][14] * dFactFxx;
	adFyy[nPosi] += Strp.dblStress[nPosi][15] * dFactFyy;
	adFxy[nPosi] += Strp.dblStress[nPosi][16] * dFactFxy;
	adVxx[nPosi] += Strp.dblStress[nPosi][26] * dFactVxx;
	adVyy[nPosi] += Strp.dblStress[nPosi][27] * dFactVyy;
	adMxx[nPosi] += Strp.dblStress[nPosi][20] * dFactMxx;
	adMyy[nPosi] += Strp.dblStress[nPosi][21] * dFactMyy;
	adMxy[nPosi] += Strp.dblStress[nPosi][22] * dFactMxy;
}


void CDgnForceCtrl::MultiplyFxx(int nPosi, double dFactor)
{
	m_FxxD[nPosi] *= dFactor;
	m_FxxL[nPosi] *= dFactor;
	m_FxxR[nPosi] *= dFactor;
	m_FxxML[nPosi] *= dFactor;
	m_FxxTP[nPosi] *= dFactor;
	m_FxxTS[nPosi] *= dFactor;
	m_FxxLU[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyFyy(int nPosi, double dFactor)
{
	m_FyyD[nPosi] *= dFactor;
	m_FyyL[nPosi] *= dFactor;
	m_FyyR[nPosi] *= dFactor;
	m_FyyML[nPosi] *= dFactor;
	m_FyyTP[nPosi] *= dFactor;
	m_FyyTS[nPosi] *= dFactor;
	m_FyyLU[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyFzz(int nPosi, double dFactor)
{
	m_FzzD[nPosi] *= dFactor;
	m_FzzL[nPosi] *= dFactor;
	m_FzzR[nPosi] *= dFactor;
	m_FzzML[nPosi] *= dFactor;
	m_FzzTP[nPosi] *= dFactor;
	m_FzzTS[nPosi] *= dFactor;
	m_FzzLU[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyBMx(int nPosi, double dFactor)
{
	m_BMxD[nPosi] *= dFactor;
	m_BMxL[nPosi] *= dFactor;
	m_BMxR[nPosi] *= dFactor;
	m_BMxML[nPosi] *= dFactor;
	m_BMxTP[nPosi] *= dFactor;
	m_BMxTS[nPosi] *= dFactor;
	m_BMxLU[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyBMy(int nPosi, double dFactor)
{
	m_BMyD[nPosi] *= dFactor;
	m_BMyL[nPosi] *= dFactor;
	m_BMyR[nPosi] *= dFactor;
	m_BMyML[nPosi] *= dFactor;
	m_BMyTP[nPosi] *= dFactor;
	m_BMyTS[nPosi] *= dFactor;
	m_BMyLU[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyBMz(int nPosi, double dFactor)
{
	m_BMzD[nPosi] *= dFactor;
	m_BMzL[nPosi] *= dFactor;
	m_BMzR[nPosi] *= dFactor;
	m_BMzML[nPosi] *= dFactor;
	m_BMzTP[nPosi] *= dFactor;
	m_BMzTS[nPosi] *= dFactor;
	m_BMzLU[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyMbi(int nPosi, double dFactor)
{
	m_MbiD[nPosi] *= dFactor;
	m_MbiL[nPosi] *= dFactor;
	m_MbiR[nPosi] *= dFactor;
	m_MbiML[nPosi] *= dFactor;
	m_MbiTP[nPosi] *= dFactor;
	m_MbiTS[nPosi] *= dFactor;
	m_MbiLU[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyTst(int nPosi, double dFactor)
{
	m_TstD[nPosi] *= dFactor;
	m_TstL[nPosi] *= dFactor;
	m_TstR[nPosi] *= dFactor;
	m_TstML[nPosi] *= dFactor;
	m_TstTP[nPosi] *= dFactor;
	m_TstTS[nPosi] *= dFactor;
	m_TstLU[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyTwp(int nPosi, double dFactor)
{
	m_TwpD[nPosi] *= dFactor;
	m_TwpL[nPosi] *= dFactor;
	m_TwpR[nPosi] *= dFactor;
	m_TwpML[nPosi] *= dFactor;
	m_TwpTP[nPosi] *= dFactor;
	m_TwpTS[nPosi] *= dFactor;
	m_TwpLU[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyBMye(int nPosi, double dFactor)
{
	for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
	{
		m_BMyeD[0][nPosi][j] *= dFactor; m_BMyeD[1][nPosi][j] *= dFactor;
		m_BMyeL[0][nPosi][j] *= dFactor; m_BMyeL[1][nPosi][j] *= dFactor;
		m_BMyeML[0][nPosi][j] *= dFactor; m_BMyeML[1][nPosi][j] *= dFactor;
		m_BMyeR[0][nPosi][j] *= dFactor; m_BMyeR[1][nPosi][j] *= dFactor;
		m_BMyeTP[0][nPosi][j] *= dFactor; m_BMyeTP[1][nPosi][j] *= dFactor;
		m_BMyeTS[0][nPosi][j] *= dFactor; m_BMyeTS[1][nPosi][j] *= dFactor;
		m_BMyeLU[0][nPosi][j] *= dFactor; m_BMyeLU[1][nPosi][j] *= dFactor;
	}
}

void CDgnForceCtrl::MultiplyBMze(int nPosi, double dFactor)
{
	for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
	{
		m_BMzeD[0][nPosi][j] *= dFactor; m_BMzeD[1][nPosi][j] *= dFactor;
		m_BMzeL[0][nPosi][j] *= dFactor; m_BMzeL[1][nPosi][j] *= dFactor;
		m_BMzeML[0][nPosi][j] *= dFactor; m_BMzeML[1][nPosi][j] *= dFactor;
		m_BMzeR[0][nPosi][j] *= dFactor; m_BMzeR[1][nPosi][j] *= dFactor;
		m_BMzeTP[0][nPosi][j] *= dFactor; m_BMzeTP[1][nPosi][j] *= dFactor;
		m_BMzeTS[0][nPosi][j] *= dFactor; m_BMzeTS[1][nPosi][j] *= dFactor;
		m_BMzeLU[0][nPosi][j] *= dFactor; m_BMzeLU[1][nPosi][j] *= dFactor;
	}
}

void CDgnForceCtrl::MultiplyFxxUF(int nPosi, double dFactor)
{
	m_FxxD_UF[nPosi] *= dFactor;
	m_FxxL_UF[nPosi] *= dFactor;
	m_FxxR_UF[nPosi] *= dFactor;
	m_FxxML_UF[nPosi] *= dFactor;
	m_FxxTP_UF[nPosi] *= dFactor;
	m_FxxTS_UF[nPosi] *= dFactor;
	m_FxxLU_UF[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyFyyUF(int nPosi, double dFactor)
{
	m_FyyD_UF[nPosi] *= dFactor;
	m_FyyL_UF[nPosi] *= dFactor;
	m_FyyR_UF[nPosi] *= dFactor;
	m_FyyML_UF[nPosi] *= dFactor;
	m_FyyTP_UF[nPosi] *= dFactor;
	m_FyyTS_UF[nPosi] *= dFactor;
	m_FyyLU_UF[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyFxyUF(int nPosi, double dFactor)
{
	m_FxyD_UF[nPosi] *= dFactor;
	m_FxyL_UF[nPosi] *= dFactor;
	m_FxyR_UF[nPosi] *= dFactor;
	m_FxyML_UF[nPosi] *= dFactor;
	m_FxyTP_UF[nPosi] *= dFactor;
	m_FxyTS_UF[nPosi] *= dFactor;
	m_FxyLU_UF[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyVxxUF(int nPosi, double dFactor)
{
	m_VxxD_UF[nPosi] *= dFactor;
	m_VxxL_UF[nPosi] *= dFactor;
	m_VxxR_UF[nPosi] *= dFactor;
	m_VxxML_UF[nPosi] *= dFactor;
	m_VxxTP_UF[nPosi] *= dFactor;
	m_VxxTS_UF[nPosi] *= dFactor;
	m_VxxLU_UF[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyVyyUF(int nPosi, double dFactor)
{
	m_VyyD_UF[nPosi] *= dFactor;
	m_VyyL_UF[nPosi] *= dFactor;
	m_VyyR_UF[nPosi] *= dFactor;
	m_VyyML_UF[nPosi] *= dFactor;
	m_VyyTP_UF[nPosi] *= dFactor;
	m_VyyTS_UF[nPosi] *= dFactor;
	m_VyyLU_UF[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyMxxUF(int nPosi, double dFactor)
{
	m_MxxD_UF[nPosi] *= dFactor;
	m_MxxL_UF[nPosi] *= dFactor;
	m_MxxR_UF[nPosi] *= dFactor;
	m_MxxML_UF[nPosi] *= dFactor;
	m_MxxTP_UF[nPosi] *= dFactor;
	m_MxxTS_UF[nPosi] *= dFactor;
	m_MxxLU_UF[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyMyyUF(int nPosi, double dFactor)
{
	m_MyyD_UF[nPosi] *= dFactor;
	m_MyyL_UF[nPosi] *= dFactor;
	m_MyyR_UF[nPosi] *= dFactor;
	m_MyyML_UF[nPosi] *= dFactor;
	m_MyyTP_UF[nPosi] *= dFactor;
	m_MyyTS_UF[nPosi] *= dFactor;
	m_MyyLU_UF[nPosi] *= dFactor;
}

void CDgnForceCtrl::MultiplyMxyUF(int nPosi, double dFactor)
{
	m_MxyD_UF[nPosi] *= dFactor;
	m_MxyL_UF[nPosi] *= dFactor;
	m_MxyR_UF[nPosi] *= dFactor;
	m_MxyML_UF[nPosi] *= dFactor;
	m_MxyTP_UF[nPosi] *= dFactor;
	m_MxyTS_UF[nPosi] *= dFactor;
	m_MxyLU_UF[nPosi] *= dFactor;
}

void CDgnForceCtrl::GetLcomDataNormailizedLctb(const T_LCOM_BASE_LIST& aSrcLcomBase, OUT T_LCOM_BASE_LIST& raTarLcomBase)
{
	// LCTB Normalize
	raTarLcomBase.RemoveAll();
	int nSize = aSrcLcomBase.GetSize();
	for (int j = 0; j < nSize; ++j)
	{
		T_LCOM_BASE CurLcomBase = aSrcLcomBase[j];
		T_LCTB_K LctbK = 0;
		if (CurLcomBase.AnalType == D_LCOM_STATIC && m_bUse_NLcomb && m_pDoc->m_pAttrCtrl->GetLctbKeyFromStldKey(CurLcomBase.LoadCaseKey, LctbK))
		{
			T_LCTB_D LctbD;
			if (!m_pDoc->m_pAttrCtrl->GetLctb(LctbK, LctbD)) continue;

			int nLctb = LctbD.aLctbBase.GetSize();
			for (int k = 0; k < nLctb; ++k)
			{
				const T_LCTB_BASE& CurLctbBase = LctbD.aLctbBase[k];

				T_LCOM_BASE LcomBaseD;
				LcomBaseD.AnalType = CurLctbBase.iAnalType;
				LcomBaseD.LoadCaseKey = CurLctbBase.LoadCaseKey;
				LcomBaseD.Factor = CurLctbBase.dFactor*CurLcomBase.Factor; // Seungjun-20130821 : Normalize시 계수 고려
				raTarLcomBase.Add(LcomBaseD);
			}
		}
		else
		{
			raTarLcomBase.Add(CurLcomBase);
		}
	}
}

int CDgnForceCtrl::GetLcomKeyListMeshedMemb(OUT T_LCOM_K_LIST& raLcomK)
{
	return GetLcomKeyListMeshedMemb(m_iRcsTypeKind, m_nCrackDef, D_LCOMTYPE_CONCRETE, raLcomK);
}

int CDgnForceCtrl::GetLcomKeyListMeshedMemb(int nRcsTypeKind, int nCrackDef, int nLcomKind, OUT T_LCOM_K_LIST& raLcomK)
{
	raLcomK.RemoveAll();

	T_MLCB_D MlcbD; MlcbD.Initialize();
	m_pDoc->m_pAttrCtrl->Get_DefaultMlcb(MlcbD);
	// PMS:4020-Seungjun-20090910 EC2:04 Slab/Wall Design.
	// 선택된 하중조합만 가져옴. T_MLCB_D이 없을 경우 DL, LL 하중조합만 설계수행.
	switch (nRcsTypeKind)
	{
	case RCS_SDGN:
	case RCS_SWAF:
	{
		raLcomK.Copy(MlcbD.aSlabStreK);
	}
	break;
	case RCS_SLCK:
	case RCS_SSCK:
	{
		raLcomK.Copy(MlcbD.aSlabStreK);
		if (IsApplyNonDissipative()) { raLcomK.Append(MlcbD.aSlabELSStreK); }
	}
	break;
	case RCS_SSEV:
	{
		if (nCrackDef == 0) { raLcomK.Copy(MlcbD.aSlabServK); }
		else { raLcomK.Copy(MlcbD.aSlabCracK); } // for Cracked deflection.
	}
	break;
	case RCS_MWDN:
	case RCS_MWCK:
	case RCS_MWWM:
	{
		raLcomK.Copy(MlcbD.aWallStreK);
	}
	break;
	case RCS_STCK:
	case RCS_STDN:
	{
		raLcomK.Copy(MlcbD.aSlabStreK);
	}
	break;
	case RCS_SHFD:
	case RCS_SHFC:
	case RCS_SHSC:
	{
		// PMS.5537 Civil Shell Design Eurocode2_2_05
		if (m_iDgnCode == Eurocode2_2_05)
		{
			CArray<T_LCOM_K, T_LCOM_K> aTotalLcomK;
			m_pDoc->m_pAttrCtrl->GetLcomKeyList(nLcomKind, aTotalLcomK);
			T_LCOM_D LcomData;
			int nSize = static_cast<int>(aTotalLcomK.GetSize());
			for (int i = 0; i < nSize; i++)
			{
				LcomData.Initialize();
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, aTotalLcomK[i], LcomData);
				if (LcomData.nActive == 0)      continue;
				if (LcomData.LoadCombType != 0) continue;
				if (LcomData.nActive == 1) { raLcomK.Add(aTotalLcomK[i]); }
			}
		}
		else
		{
			raLcomK.Copy(MlcbD.aShellStreK);
		}
	}
	break;
	default:
		ASSERT(0);
		break;
	}

	return static_cast<int>(raLcomK.GetSize());
}

void CDgnForceCtrl::Sta_ForceByLctb(int iElemNo, BOOL bMaxLcom)
{
	// Add by ZINU.('04.02.13). For Load Contributions.
	if (!m_bExistLctb) { ASSERT(0); return; }

	int i = 0, j = 0, k = 0;
	for (i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		m_FxxLctb[i] = 0.0; m_FyyLctb[i] = 0.0; m_FzzLctb[i] = 0.0;
		m_BMxLctb[i] = 0.0; m_BMyLctb[i] = 0.0; m_BMzLctb[i] = 0.0;
		m_FxxUFLctb[i] = 0.0;
		m_FyyUFLctb[i] = 0.0;
		m_FxyUFLctb[i] = 0.0;
		m_VxxUFLctb[i] = 0.0;
		m_VyyUFLctb[i] = 0.0;
		m_MxxUFLctb[i] = 0.0;
		m_MyyUFLctb[i] = 0.0;
		m_MxyUFLctb[i] = 0.0;
		// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
		m_MbiLctb[i] = 0.0;
		m_TstLctb[i] = 0.0;
		m_TwpLctb[i] = 0.0;

		m_arRemnPos[i].RemoveAll();
		m_arRemnPos[i].RemoveAll();
		for (j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			for (k = 0; k < 2; k++)
			{
				m_BMyeLctb[k][i][j] = 0.0;
				m_BMzeLctb[k][i][j] = 0.0;
			}
			m_arRemnEnd[i][j].RemoveAll();
		}
	}
	// Get Force Data by Lcom.
	int iLcaseType = 0;
	if (m_strMatType == _T("S"))			iLcaseType = IsColdFormDesign(iElemNo, TRUE) ? D_LOADCASE_COMB_CFSTEEL : D_LOADCASE_COMB_STEEL;
	else if (m_strMatType == _T("C"))		iLcaseType = D_LOADCASE_COMB_CONCRETE;
	else if (m_strMatType == _T("SRC"))	iLcaseType = D_LOADCASE_COMB_SRC;
	else if (m_strMatType == _T("A"))	  iLcaseType = D_LOADCASE_COMB_ALUMINUM;
	else	ASSERT(0);

	T_STRB_D StrbMaxData, StrbMinData;
	T_STRT_D StrtMaxData, StrtMinData;
	T_STRW_D StrwMaxData, StrwMinData;
	if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
	{
		StrbMaxData.Initialize();
		StrbMinData.Initialize();
		GetStrbDgn(iElemNo, iLcaseType, m_iOrgLcomKey, &StrbMaxData, &StrbMinData);

		for (i = 0; i < CONST_DGN_iPOSNUM; i++)
		{
			///////////////////////////////////
			// For End Moments.
			T_ELEM_K StaElemKeyPrev = 0, EndElemKeyPrev = 0;
			T_ELEM_K StaElemKeyNext = 0, EndElemKeyNext = 0;
			T_STRB_D StrbD;
			for (j = 0; j < 2; j++)	// 0-y, 1-z.
			{
				int iElemCount = m_pDgnDataCtrl->m_pMembCtrl->GetElemCountByMembPosi(m_apElemK_z[0].GetAt(0), i);
				// zList for My, yList for Mz.
				StaElemKeyPrev = (j == 0 ? m_apElemK_z[0].GetAt(iElemCount) : m_apElemK_y[0].GetAt(iElemCount));
				EndElemKeyPrev = (j == 0 ? m_apElemK_z[1].GetAt(iElemCount) : m_apElemK_y[1].GetAt(iElemCount));
				if (StaElemKeyPrev != StaElemKeyNext || EndElemKeyPrev != EndElemKeyNext)
				{
					T_STRB_D StrMaxData, StrMinData;
					StrMaxData.Initialize();
					StrMinData.Initialize();
					GetStrbDgn(StaElemKeyPrev, EndElemKeyPrev, iLcaseType, m_iOrgLcomKey, &StrMaxData, &StrMinData);
					StrbD.Initialize();
					StrbD = (bMaxLcom ? StrMaxData : StrMinData);
				}
				StaElemKeyNext = StaElemKeyPrev;
				EndElemKeyNext = EndElemKeyPrev;
				// Get Moments.
				double dMy[CONST_DGN_iPOSNUM] = { StrbD.dblForce[0][4], StrbD.dblForce[1][4], StrbD.dblForce[2][4], StrbD.dblForce[3][4], StrbD.dblForce[3][10] };
				double dMz[CONST_DGN_iPOSNUM] = { StrbD.dblForce[0][5], StrbD.dblForce[1][5], StrbD.dblForce[2][5], StrbD.dblForce[3][5], StrbD.dblForce[3][11] };
				// Set Moments.
				for (k = 0; k < CONST_DGN_iPOSNUM; k++)
				{
					// Use m_apElemK_y for Cb.
					if (j == 0) { m_BMyeLctb[0][i][k] += dMy[k];  m_BMzeLctb[1][i][k] += dMz[k]; }
					else { m_BMzeLctb[0][i][k] += dMz[k];  m_BMyeLctb[1][i][k] += dMy[k]; }
				}
			}
			const T_STRB_D& CurStrbD = bMaxLcom ? StrbMaxData : StrbMinData;
			///////////////////////////////////
			// Forces, Moments.			
			int iPart = 0;
			int iKind = 0;
			int iKind7 = 0;
			GetAnalForceComponentIndex(i, iPart, iKind, iKind7);
			m_FxxLctb[i] = CurStrbD.dblForce[iPart][iKind + 0];
			m_FyyLctb[i] = CurStrbD.dblForce[iPart][iKind + 1];
			m_FzzLctb[i] = CurStrbD.dblForce[iPart][iKind + 2];
			m_BMxLctb[i] = CurStrbD.dblForce[iPart][iKind + 3];
			m_BMyLctb[i] = CurStrbD.dblForce[iPart][iKind + 4];
			m_BMzLctb[i] = CurStrbD.dblForce[iPart][iKind + 5];
			// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
			m_MbiLctb[i] = CurStrbD.dblForce[iPart][iKind7 + 0];
			m_TstLctb[i] = CurStrbD.dblForce[iPart][iKind7 + 2];
			m_TwpLctb[i] = CurStrbD.dblForce[iPart][iKind7 + 4];
		}
	}
	else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
	{
		StrwMaxData.Initialize();
		StrwMinData.Initialize();
		GetStrwDgn(iElemNo, iLcaseType, m_iOrgLcomKey, &StrwMaxData, &StrwMinData);
		const T_STRW_D& CurStrwD = bMaxLcom ? StrwMaxData : StrwMinData;
		for (i = 0; i < CONST_DGN_iPOSNUM; i++)
		{
			// Forces, Moments.
			int iPart = (i < 4 ? 1 : 0);	// I=1, J=0.
			m_FxxLctb[i] = CurStrwD.dblForce[iPart][0] * m_dNTC08LcasN;  // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.
			m_FyyLctb[i] = CurStrwD.dblForce[iPart][1];
			m_FzzLctb[i] = CurStrwD.dblForce[iPart][2];
			m_BMxLctb[i] = CurStrwD.dblForce[iPart][3];
			m_BMyLctb[i] = CurStrwD.dblForce[iPart][4];
			m_BMzLctb[i] = CurStrwD.dblForce[iPart][5];
		}
	}
	else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
	{
		StrtMaxData.Initialize();
		StrtMinData.Initialize();
		GetStrtDgn(iElemNo, iLcaseType, m_iOrgLcomKey, &StrtMaxData, &StrtMinData);
		const T_STRT_D& CurStrtD = bMaxLcom ? StrtMaxData : StrtMinData;
		for (i = 0; i < CONST_DGN_iPOSNUM; i++)
		{
			// Forces, Moments.
			m_FxxLctb[i] += CurStrtD.dblForce[i / 4];
			m_FyyLctb[i] = 0.0;
			m_FzzLctb[i] = 0.0;
			m_BMxLctb[i] = 0.0;
			m_BMyLctb[i] = 0.0;
			m_BMzLctb[i] = 0.0;
		}
	}
}

void CDgnForceCtrl::Cng_ForceByLctb(int iElemNo, int iPosiNo, int iAnalType, int iLcaseKey, double dFactor, BOOL bMaxLcom)
{
	// Add by ZINU.('04.02.13). For Load Contributions.
	if (!m_bExistLctb) { ASSERT(0); return; }

	int iLcaseType = 0;
	// Change by ZINU.('05.05.30). Only iAnalType==D_LCOM_STATIC.
	if (iAnalType == D_LCOM_STATIC)
	{
		iLcaseType = D_LOADCASE_STATIC;
	}
	else
	{
		return;
	}

	T_STRB_D  StrbMaxData, StrbMinData;
	T_STRT_D  StrtMaxData, StrtMinData;
	T_STRW_D  StrwMaxData, StrwMinData;
	T_STRP_DL StrpMaxData, StrpMinData;
	if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
	{
		StrbMaxData.Initialize();
		StrbMinData.Initialize();
		GetStrbDgn(iElemNo, iLcaseType, iLcaseKey, &StrbMaxData, &StrbMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
	{
		StrtMaxData.Initialize();
		StrtMinData.Initialize();
		GetStrtDgn(iElemNo, iLcaseType, iLcaseKey, &StrtMaxData, &StrtMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
	{
		StrwMaxData.Initialize();
		StrwMinData.Initialize();
		GetStrwDgn(iElemNo, iLcaseType, iLcaseKey, &StrwMaxData, &StrwMinData);
	}
	else if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Type.
	{
		StrpMaxData.Initialize();
		StrpMinData.Initialize();
		GetStrpDgn(iElemNo, iLcaseType, iLcaseKey, &StrpMaxData, &StrpMinData);
	}
	// Save Forces and Moments by Position.
	int iPosStaNo = 0;
	int iPosTotNo = 0;
	if (iPosiNo == -1)	iPosTotNo = CONST_DGN_iPOSNUM;
	else
	{
		iPosStaNo = iPosiNo;
		iPosTotNo = iPosiNo + 1;
	}

	// Check Gravity Force.
	// Change by ZINU.('04.03.13). Dead Type (D -> D,DC,DW).
	// Change by ZINU.('05.07.30). Change GravityForce Option. (IsStldLL, Is_StctEL_DL, Is_StctEL_LL).
	CString strForceType = _T("");
	if (iAnalType == D_LCOM_STATIC)
	{
		// Get Load Case Type(D,L,W,E,...).
		T_STLD_D StldD;
		StldD.Initialize();
		m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, StldD);
		if (IsDeadLoadType(StldD.LoadCaseType, D_LCOM_STATIC, iLcaseKey))
		{
			strForceType = _T("D");
		}
		else if (IsLiveLoadType(StldD.LoadCaseType, D_LCOM_STATIC))
		{
			strForceType = _T("L");
		}
	}
	else if (iAnalType == D_LCOM_MOVING)	strForceType = _T("ML");
	else if (iAnalType == D_LCOM_STAGE)
	{
		CString strErLCType = _T("");
		T_STCT_D StctD; StctD.Initialize();
		m_pDoc->m_pAttrCtrl->GetStct(StctD);
		T_SGLD_D SgldD;
		SgldD.Initialize();
		m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldPost(iLcaseKey, SgldD);
		// 1=SUM, 2=Tendon Primary, 3=Tendon Secondary, 4=Live Load.
		if (SgldD.LoadCaseName == _LS(IDS_DB_SGLD_SUM))			strForceType = _T("D");
		else if (SgldD.LoadCaseName == _LS(IDS_DB_SGLD_TP))	strForceType = _T("TP");
		else if (SgldD.LoadCaseName == _LS(IDS_DB_SGLD_TS))	strForceType = _T("TS");
		else if (SgldD.LoadCaseName == _LS(IDS_DB_SGLD_LL))    strForceType = _T("L");
		else if (GetErectionLoadType(SgldD.LoadCaseName, StctD, strErLCType))
		{
			if (CDBLib::IsStldDL(strErLCType)) strForceType = _T("D");
			else if (CDBLib::IsStldLL(strErLCType)) strForceType = _T("L");
		}
	}
	BOOL bGravityForce = (strForceType != _T("") ? TRUE : FALSE);
	if (!bGravityForce)	strForceType = _T("R");

	// Set Force Type.
	for (int i = iPosStaNo; i < iPosTotNo; i++)
	{
		if (m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	// Beam Type.
		{
			T_STRB_D CurStrbD = bMaxLcom ? StrbMaxData : StrbMinData;
			// End Moments.
			if (!Get_BeamEndMoment(strForceType, i, dFactor, bMaxLcom, FALSE, iLcaseType, iLcaseKey, &CurStrbD))	return;
			// Forces, Moments.
			int iPart = 0;
			int iKind = 0;
			int iKind7 = 0;
			GetAnalForceComponentIndex(i, iPart, iKind, iKind7);

			if (bGravityForce)
			{
				m_FxxLctb[i] -= CurStrbD.dblForce[iPart][iKind + 0] * dFactor;
				m_FyyLctb[i] -= CurStrbD.dblForce[iPart][iKind + 1] * dFactor;
				m_FzzLctb[i] -= CurStrbD.dblForce[iPart][iKind + 2] * dFactor;
				m_BMxLctb[i] -= CurStrbD.dblForce[iPart][iKind + 3] * dFactor;
				m_BMyLctb[i] -= CurStrbD.dblForce[iPart][iKind + 4] * dFactor;
				m_BMzLctb[i] -= CurStrbD.dblForce[iPart][iKind + 5] * dFactor;
				// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
				m_MbiLctb[i] -= CurStrbD.dblForce[iPart][iKind7 + 0] * dFactor;
				m_TstLctb[i] -= CurStrbD.dblForce[iPart][iKind7 + 2] * dFactor;
				m_TwpLctb[i] -= CurStrbD.dblForce[iPart][iKind7 + 4] * dFactor;
			}
			else
			{
				_DGN_LCTB_POS LctbPos;  LctbPos.Initialize();
				LctbPos.dRemnFxx = CurStrbD.dblForce[iPart][iKind + 0] * dFactor;
				LctbPos.dRemnFyy = CurStrbD.dblForce[iPart][iKind + 1] * dFactor;
				LctbPos.dRemnFzz = CurStrbD.dblForce[iPart][iKind + 2] * dFactor;
				LctbPos.dRemnBMx = CurStrbD.dblForce[iPart][iKind + 3] * dFactor;
				LctbPos.dRemnBMy = CurStrbD.dblForce[iPart][iKind + 4] * dFactor;
				LctbPos.dRemnBMz = CurStrbD.dblForce[iPart][iKind + 5] * dFactor;
				// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
				LctbPos.dRemnMbi = CurStrbD.dblForce[iPart][iKind7 + 0] * dFactor;
				LctbPos.dRemnTst = CurStrbD.dblForce[iPart][iKind7 + 2] * dFactor;
				LctbPos.dRemnTwp = CurStrbD.dblForce[iPart][iKind7 + 4] * dFactor;
				m_arRemnPos[i].SetAt(iLcaseKey, LctbPos);
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall Type.
		{
			T_STRW_D& CurStrwD = bMaxLcom ? StrwMaxData : StrwMinData;
			// End Moments.
			if (!Get_WallEndMoment(strForceType, i, dFactor, FALSE, iLcaseKey, &CurStrwD))	return;
			// Forces, Moments.
			int iPart = (i < 4 ? 1 : 0);	// I=1, J=0.
			if (bGravityForce)
			{
				m_FxxLctb[i] -= CurStrwD.dblForce[iPart][0] * dFactor * m_dNTC08LcasN;  // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.
				m_FyyLctb[i] -= CurStrwD.dblForce[iPart][1] * dFactor;
				m_FzzLctb[i] -= CurStrwD.dblForce[iPart][2] * dFactor;
				m_BMxLctb[i] -= CurStrwD.dblForce[iPart][3] * dFactor;
				m_BMyLctb[i] -= CurStrwD.dblForce[iPart][4] * dFactor;
				m_BMzLctb[i] -= CurStrwD.dblForce[iPart][5] * dFactor;
			}
			else
			{
				_DGN_LCTB_POS LctbPos;  LctbPos.Initialize();
				LctbPos.dRemnFxx = CurStrwD.dblForce[iPart][0] * dFactor * m_dNTC08LcasN;  // PMS:3956-Seungjun-20090701 EC8:04 Seismic Design.
				LctbPos.dRemnFyy = CurStrwD.dblForce[iPart][1] * dFactor;
				LctbPos.dRemnFzz = CurStrwD.dblForce[iPart][2] * dFactor;
				LctbPos.dRemnBMx = CurStrwD.dblForce[iPart][3] * dFactor;
				LctbPos.dRemnBMy = CurStrwD.dblForce[iPart][4] * dFactor;
				LctbPos.dRemnBMz = CurStrwD.dblForce[iPart][5] * dFactor;
				m_arRemnPos[i].SetAt(iLcaseKey, LctbPos);
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsTruss(m_iElemType))	// Truss Type.
		{
			T_STRT_D& CurStrtD = bMaxLcom ? StrtMaxData : StrtMinData;
			// End Moments.
			if (!Get_TrussEndMoment(strForceType, i))	return;
			// Forces, Moments.
			if (bGravityForce)
			{
				m_FxxLctb[i] -= CurStrtD.dblForce[i / 4] * dFactor;
				m_FyyLctb[i] -= 0.0;
				m_FzzLctb[i] -= 0.0;
				m_BMxLctb[i] -= 0.0;
				m_BMyLctb[i] -= 0.0;
				m_BMzLctb[i] -= 0.0;
			}
			else
			{
				_DGN_LCTB_POS LctbPos;  LctbPos.Initialize();
				LctbPos.dRemnFxx = CurStrtD.dblForce[i / 4] * dFactor;
				LctbPos.dRemnFyy = 0.0;
				LctbPos.dRemnFzz = 0.0;
				LctbPos.dRemnBMx = 0.0;
				LctbPos.dRemnBMy = 0.0;
				LctbPos.dRemnBMz = 0.0;
				m_arRemnPos[i].SetAt(iLcaseKey, LctbPos);
			}
		}
		if (m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType))	// Plate Type.
		{
			T_STRP_DL& CurStrpD = bMaxLcom ? StrpMaxData : StrpMinData;
			// End Moments.
			if (!Get_PlateEndMoment(strForceType, i))	return;
			// Forces, Moments.			
			if (bGravityForce)
			{
				if (i < 4)
				{//component : 0:Fx, 1:Fy, 2:Fz, 3:Mx, 4:My, 5:Mz
					m_FxxLctb[i] -= CurStrpD.dblForce[i][0] * dFactor;
					m_FyyLctb[i] -= CurStrpD.dblForce[i][1] * dFactor;
					m_FzzLctb[i] -= CurStrpD.dblForce[i][2] * dFactor;
					m_BMxLctb[i] -= CurStrpD.dblForce[i][3] * dFactor;
					m_BMyLctb[i] -= CurStrpD.dblForce[i][4] * dFactor;
					m_BMzLctb[i] -= CurStrpD.dblForce[i][5] * dFactor;
				}
				if (i < 5)
				{//component : 14:Fxx, 16:Fyy, 17:Fxy, 20:Mxx, 21:Myy, 26:Vxx,  27:Vyy
					m_FxxUFLctb[i] -= CurStrpD.dblStress[i][14] * dFactor;
					m_FyyUFLctb[i] -= CurStrpD.dblStress[i][15] * dFactor;
					m_FxyUFLctb[i] -= CurStrpD.dblStress[i][16] * dFactor;
					m_VxxUFLctb[i] -= CurStrpD.dblStress[i][26] * dFactor;
					m_VyyUFLctb[i] -= CurStrpD.dblStress[i][27] * dFactor;
					m_MxxUFLctb[i] -= CurStrpD.dblStress[i][20] * dFactor;
					m_MyyUFLctb[i] -= CurStrpD.dblStress[i][21] * dFactor;
					m_MxyUFLctb[i] -= CurStrpD.dblStress[i][22] * dFactor;
				}
			}
			else
			{
				_DGN_LCTB_POS LctbPos;  LctbPos.Initialize();
				if (i < 4)
				{//component : 0:Fx, 1:Fy, 2:Fz, 3:Mx, 4:My, 5:Mz					
					LctbPos.dRemnFxx = CurStrpD.dblForce[i][0] * dFactor;
					LctbPos.dRemnFyy = CurStrpD.dblForce[i][1] * dFactor;
					LctbPos.dRemnFzz = CurStrpD.dblForce[i][2] * dFactor;
					LctbPos.dRemnBMx = CurStrpD.dblForce[i][3] * dFactor;
					LctbPos.dRemnBMy = CurStrpD.dblForce[i][4] * dFactor;
					LctbPos.dRemnBMz = CurStrpD.dblForce[i][5] * dFactor;
				}
				if (i < 5)
				{//component : 14:Fxx, 16:Fyy, 17:Fxy, 20:Mxx, 21:Myy, 26:Vxx,  27:Vyy
					LctbPos.dRemnFxxUF -= CurStrpD.dblStress[i][14] * dFactor;
					LctbPos.dRemnFyyUF -= CurStrpD.dblStress[i][15] * dFactor;
					LctbPos.dRemnFxyUF -= CurStrpD.dblStress[i][16] * dFactor;
					LctbPos.dRemnVxxUF -= CurStrpD.dblStress[i][26] * dFactor;
					LctbPos.dRemnVyyUF -= CurStrpD.dblStress[i][27] * dFactor;
					LctbPos.dRemnMxxUF -= CurStrpD.dblStress[i][20] * dFactor;
					LctbPos.dRemnMyyUF -= CurStrpD.dblStress[i][21] * dFactor;
					LctbPos.dRemnMxyUF -= CurStrpD.dblStress[i][22] * dFactor;
				}
				m_arRemnPos[i].SetAt(iLcaseKey, LctbPos);
			}
		}
	}
}

void CDgnForceCtrl::End_ForceByLctb()
{
	// Add by ZINU.('04.02.13). For Load Contributions.
	if (!m_bExistLctb) { ASSERT(0); return; }

	int i = 0, j = 0, k = 0;
	POSITION Pos;
	int iIndex = 0;	// iLcasNo.
	/////////////////////////////////////////
	// Set Forces at m_arRemnPos.
	_DGN_LCTB_POS LctbPos;
	double dSumFxx[CONST_DGN_iPOSNUM], dSumFyy[CONST_DGN_iPOSNUM], dSumFzz[CONST_DGN_iPOSNUM];
	double dSumBMx[CONST_DGN_iPOSNUM], dSumBMy[CONST_DGN_iPOSNUM], dSumBMz[CONST_DGN_iPOSNUM];
	// Add by GAY. ('13.05.28). PMS:4632. Torsion + Warping for 7 DOF.
	double dSumMbi[CONST_DGN_iPOSNUM], dSumTst[CONST_DGN_iPOSNUM], dSumTwp[CONST_DGN_iPOSNUM];
	double dRatFxx[CONST_DGN_iPOSNUM], dRatFyy[CONST_DGN_iPOSNUM], dRatFzz[CONST_DGN_iPOSNUM];
	double dRatBMx[CONST_DGN_iPOSNUM], dRatBMy[CONST_DGN_iPOSNUM], dRatBMz[CONST_DGN_iPOSNUM];
	// Add by GAY. ('13.05.28). PMS:4632. Torsion + Warping for 7 DOF.
	double dRatMbi[CONST_DGN_iPOSNUM], dRatTst[CONST_DGN_iPOSNUM], dRatTwp[CONST_DGN_iPOSNUM];
	for (i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		dSumFxx[i] = 0.0; dSumFyy[i] = 0.0; dSumFzz[i] = 0.0;
		dSumBMx[i] = 0.0; dSumBMy[i] = 0.0; dSumBMz[i] = 0.0;
		dSumMbi[i] = 0.0; dSumTst[i] = 0.0; dSumTwp[i] = 0.0;
		dRatFxx[i] = 0.0; dRatFyy[i] = 0.0; dRatFzz[i] = 0.0;
		dRatBMx[i] = 0.0; dRatBMy[i] = 0.0; dRatBMz[i] = 0.0;
		dRatMbi[i] = 0.0; dRatTst[i] = 0.0; dRatTwp[i] = 0.0;
	}
	// Get Force by SUM [R1+R2+R3+...].
	for (i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		Pos = m_arRemnPos[i].GetStartPosition();
		while (Pos)
		{
			LctbPos.Initialize();
			m_arRemnPos[i].GetNextAssoc(Pos, iIndex, LctbPos);
			dSumFxx[i] += LctbPos.dRemnFxx;
			dSumFyy[i] += LctbPos.dRemnFyy;
			dSumFzz[i] += LctbPos.dRemnFzz;
			dSumBMx[i] += LctbPos.dRemnBMx;
			dSumBMy[i] += LctbPos.dRemnBMy;
			dSumBMz[i] += LctbPos.dRemnBMz;
			dSumMbi[i] += LctbPos.dRemnMbi;
			dSumTst[i] += LctbPos.dRemnTst;
			dSumTwp[i] += LctbPos.dRemnTwp;
		}
	}
	// Get Rat = [LCB-(D+L)]/SUM.
	for (i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		dRatFxx[i] = (dSumFxx[i] == 0.0 ? 0.0 : m_FxxLctb[i] / dSumFxx[i]);
		dRatFyy[i] = (dSumFyy[i] == 0.0 ? 0.0 : m_FyyLctb[i] / dSumFyy[i]);
		dRatFzz[i] = (dSumFzz[i] == 0.0 ? 0.0 : m_FzzLctb[i] / dSumFzz[i]);
		dRatBMx[i] = (dSumBMx[i] == 0.0 ? 0.0 : m_BMxLctb[i] / dSumBMx[i]);
		dRatBMy[i] = (dSumBMy[i] == 0.0 ? 0.0 : m_BMyLctb[i] / dSumBMy[i]);
		dRatBMz[i] = (dSumBMz[i] == 0.0 ? 0.0 : m_BMzLctb[i] / dSumBMz[i]);
		// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
		dRatMbi[i] = (dSumMbi[i] == 0.0 ? 0.0 : m_MbiLctb[i] / dSumMbi[i]);
		dRatTst[i] = (dSumTst[i] == 0.0 ? 0.0 : m_TstLctb[i] / dSumTst[i]);
		dRatTwp[i] = (dSumTwp[i] == 0.0 ? 0.0 : m_TwpLctb[i] / dSumTwp[i]);
	}
	// Set Force (R1+R2+R3+...).
	for (i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		Pos = m_arRemnPos[i].GetStartPosition();
		while (Pos)
		{
			LctbPos.Initialize();
			m_arRemnPos[i].GetNextAssoc(Pos, iIndex, LctbPos);
			LctbPos.dRemnFxx *= dRatFxx[i];
			LctbPos.dRemnFyy *= dRatFyy[i];
			LctbPos.dRemnFzz *= dRatFzz[i];
			LctbPos.dRemnBMx *= dRatBMx[i];
			LctbPos.dRemnBMy *= dRatBMy[i];
			LctbPos.dRemnBMz *= dRatBMz[i];
			LctbPos.dRemnMbi *= dRatMbi[i];
			LctbPos.dRemnTst *= dRatTst[i];
			LctbPos.dRemnTwp *= dRatTwp[i];
			m_arRemnPos[i].SetAt(iIndex, LctbPos);
		}
	}
	/////////////////////////////////////////
	// Set Forces at m_arRemnEnd.
	_DGN_LCTB_END LctbEnd;
	double dSumBMye[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM], dSumBMze[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	double dRatBMye[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM], dRatBMze[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	for (i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		for (j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			for (k = 0; k < 2; k++)
			{
				dSumBMye[k][i][j] = 0.0; dSumBMze[k][i][j] = 0.0;
				dRatBMye[k][i][j] = 0.0; dRatBMze[k][i][j] = 0.0;
			}
		}
	}
	// Get Force by SUM [R1+R2+R3+...].
	for (i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		for (j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			Pos = m_arRemnEnd[i][j].GetStartPosition();
			while (Pos)
			{
				LctbEnd.Initialize();
				m_arRemnEnd[i][j].GetNextAssoc(Pos, iIndex, LctbEnd);
				dSumBMye[0][i][j] += LctbEnd.dRemnBMye[0];
				dSumBMye[1][i][j] += LctbEnd.dRemnBMye[1];
				dSumBMze[0][i][j] += LctbEnd.dRemnBMze[0];
				dSumBMze[1][i][j] += LctbEnd.dRemnBMze[1];
			}
		}
	}
	// Get Rat = [LCB-(D+L)]/SUM.
	for (i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		for (j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			dRatBMye[0][i][j] = (dSumBMye[0][i][j] == 0.0 ? 0.0 : m_BMyeLctb[0][i][j] / dSumBMye[0][i][j]);
			dRatBMye[1][i][j] = (dSumBMye[1][i][j] == 0.0 ? 0.0 : m_BMyeLctb[1][i][j] / dSumBMye[1][i][j]);
			dRatBMze[0][i][j] = (dSumBMze[0][i][j] == 0.0 ? 0.0 : m_BMzeLctb[0][i][j] / dSumBMze[0][i][j]);
			dRatBMze[1][i][j] = (dSumBMze[1][i][j] == 0.0 ? 0.0 : m_BMzeLctb[1][i][j] / dSumBMze[1][i][j]);
		}
	}
	// Set Force (R1+R2+R3+...).
	for (i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		for (j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			Pos = m_arRemnEnd[i][j].GetStartPosition();
			while (Pos)
			{
				LctbEnd.Initialize();
				m_arRemnEnd[i][j].GetNextAssoc(Pos, iIndex, LctbEnd);
				LctbEnd.dRemnBMye[0] *= dRatBMye[0][i][j];
				LctbEnd.dRemnBMye[1] *= dRatBMye[1][i][j];
				LctbEnd.dRemnBMze[0] *= dRatBMze[0][i][j];
				LctbEnd.dRemnBMze[1] *= dRatBMze[1][i][j];
				m_arRemnEnd[i][j].SetAt(iIndex, LctbEnd);
			}
		}
	}
}

BOOL CDgnForceCtrl::Get_ZeroFactor(double& dZeroLen, double& dZeroFor)
{
	// Set by N,mm.
	dZeroLen = 0.0;
	switch (m_nCode_Length)
	{
	case(D_UNITSYS_LENGTH_INDEX_M): {dZeroLen = 1. / 1000.;				break; }
	case(D_UNITSYS_LENGTH_INDEX_CM): {dZeroLen = 1. / 10.;					break; }
	case(D_UNITSYS_LENGTH_INDEX_MM): {dZeroLen = 1.0;						break; }
	case(D_UNITSYS_LENGTH_INDEX_IN): {dZeroLen = 1. / 25.4;				break; }
	case(D_UNITSYS_LENGTH_INDEX_FT): {dZeroLen = 1. / (25.4*12.);	break; }
	}
	dZeroFor = 0.0;
	switch (m_nCode_Force)
	{
	case(D_UNITSYS_FORCE_INDEX_KG): {dZeroFor = 1. / 9.80665;		break; }
	case(D_UNITSYS_FORCE_INDEX_TON): {dZeroFor = 1. / 9806.65;		break; }
	case(D_UNITSYS_FORCE_INDEX_N): {dZeroFor = 1.0;					break; }
	case(D_UNITSYS_FORCE_INDEX_KN): {dZeroFor = 1. / 1000.0;		break; }
	case(D_UNITSYS_FORCE_INDEX_LBF): {dZeroFor = 1. / 4.448223;	break; }
	case(D_UNITSYS_FORCE_INDEX_KIP): {dZeroFor = 1. / 4448.223;	break; }
	}
	if (dZeroLen*dZeroFor == 0.0)	return FALSE;
	return TRUE;
}

void CDgnForceCtrl::Save_TotalForce(int iElem, int iSeismicType, BOOL bLongTerm)
{
	ApplyLoadContribution();

	BOOL bPlate = m_pDoc->m_pAttrCtrl->IsPlate(m_iElemType);

	// Frame, Wall /////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		ApplyScaleUpFactorForColumn(i); // Civil Only.

		ApplyEQFactorForLcom(iElem, iSeismicType, i, bPlate);

		ApplySafeFactorGB(i);

		// Add by ZINU.('02.5.23). Include Moving Live Load to Live Load.
		// Forces.
		m_FxxL[i] += m_FxxML[i];
		m_FyyL[i] += m_FyyML[i];
		m_FzzL[i] += m_FzzML[i];
		// Moments.
		m_BMxL[i] += m_BMxML[i];
		m_BMyL[i] += m_BMyML[i];
		m_BMzL[i] += m_BMzML[i];
		// Add by GAY. ('13.05.28). PMS:4632. Torsion+Warping for 7 DOF.
		m_MbiL[i] += m_MbiML[i];
		m_TstL[i] += m_TstML[i];
		m_TwpL[i] += m_TwpML[i];
		// End Moments.
		for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			m_BMyeL[0][i][j] += m_BMyeML[0][i][j];
			m_BMyeL[1][i][j] += m_BMyeML[1][i][j];
			m_BMzeL[0][i][j] += m_BMzeML[0][i][j];
			m_BMzeL[1][i][j] += m_BMzeML[1][i][j];
		}
		// Add by ZINU.('02.12.11). Include Tendon Primary/Secondary Load to Dead Load.
		// Forces.
		m_FxxD[i] += m_FxxTP[i] + m_FxxTS[i];
		m_FyyD[i] += m_FyyTP[i] + m_FyyTS[i];
		m_FzzD[i] += m_FzzTP[i] + m_FzzTS[i];
		// Moments.
		m_BMxD[i] += m_BMxTP[i] + m_BMxTS[i];
		m_BMyD[i] += m_BMyTP[i] + m_BMyTS[i];
		m_BMzD[i] += m_BMzTP[i] + m_BMzTS[i];
		// Add by GAY. ('13.05.28). PMS:4632. Torsion+Warping for 7 DOF.
		m_MbiD[i] += m_MbiTP[i] + m_MbiTS[i];
		m_TstD[i] += m_TstTP[i] + m_TstTS[i];
		m_TwpD[i] += m_TwpTP[i] + m_TwpTS[i];
		// End Moments.
		for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			m_BMyeD[0][i][j] += m_BMyeTP[0][i][j] + m_BMyeTS[0][i][j];
			m_BMyeD[1][i][j] += m_BMyeTP[1][i][j] + m_BMyeTS[1][i][j];
			m_BMzeD[0][i][j] += m_BMzeTP[0][i][j] + m_BMzeTS[0][i][j];
			m_BMzeD[1][i][j] += m_BMzeTP[1][i][j] + m_BMzeTS[1][i][j];
		}

		SetSmallValueAsZero(i);

		ChangeAxialForceSign(i, bPlate);

		m_Fxx[i] = m_FxxD[i] + m_FxxL[i] + m_FxxR[i] + m_FxxLU[i];
		m_Fyy[i] = m_FyyD[i] + m_FyyL[i] + m_FyyR[i] + m_FyyLU[i];
		m_Fzz[i] = m_FzzD[i] + m_FzzL[i] + m_FzzR[i] + m_FzzLU[i];
		m_BMx[i] = m_BMxD[i] + m_BMxL[i] + m_BMxR[i] + m_BMxLU[i];
		m_BMy[i] = m_BMyD[i] + m_BMyL[i] + m_BMyR[i] + m_BMyLU[i];
		m_BMz[i] = m_BMzD[i] + m_BMzL[i] + m_BMzR[i] + m_BMzLU[i];
		// Add by GAY. ('13.05.28). PMS:4632. Torsion+Warping for 7 DOF.
		m_Mbi[i] = m_MbiD[i] + m_MbiL[i] + m_MbiR[i];
		m_Tst[i] = m_TstD[i] + m_TstL[i] + m_TstR[i];
		m_Twp[i] = m_TwpD[i] + m_TwpL[i] + m_TwpR[i];
		for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			m_BMye[0][i][j] = m_BMyeD[0][i][j] + m_BMyeL[0][i][j] + m_BMyeR[0][i][j] + m_BMyeLU[0][i][j];
			m_BMye[1][i][j] = m_BMyeD[1][i][j] + m_BMyeL[1][i][j] + m_BMyeR[1][i][j] + m_BMyeLU[1][i][j];
			m_BMze[0][i][j] = m_BMzeD[0][i][j] + m_BMzeL[0][i][j] + m_BMzeR[0][i][j] + m_BMzeLU[0][i][j];
			m_BMze[1][i][j] = m_BMzeD[1][i][j] + m_BMzeL[1][i][j] + m_BMzeR[1][i][j] + m_BMzeLU[1][i][j];
		}
	}

	// Coded by Seungjun ('20070727) MNet:No.XXXX.
	// Tension Only Member.
	if (m_iFreeTensComp == 1)
	{
		double dFxx = (m_Fxx[0] + m_Fxx[CONST_DGN_iPOSNUM - 1]) / 2.0;
		double dFxxD = (m_FxxD[0] + m_FxxD[CONST_DGN_iPOSNUM - 1]) / 2.0;
		double dFxxL = (m_FxxL[0] + m_FxxL[CONST_DGN_iPOSNUM - 1]) / 2.0;
		double dFxxR = (m_FxxR[0] + m_FxxR[CONST_DGN_iPOSNUM - 1]) / 2.0;
		double dFxxML = (m_FxxML[0] + m_FxxML[CONST_DGN_iPOSNUM - 1]) / 2.0;
		double dFxxTP = (m_FxxTP[0] + m_FxxTP[CONST_DGN_iPOSNUM - 1]) / 2.0;
		double dFxxTS = (m_FxxTS[0] + m_FxxTS[CONST_DGN_iPOSNUM - 1]) / 2.0;
		double dFxxLU = (m_FxxLU[0] + m_FxxLU[CONST_DGN_iPOSNUM - 1]) / 2.0;

		m_Fxx[0] = m_Fxx[CONST_DGN_iPOSNUM - 1] = dFxx;
		m_FxxD[0] = m_FxxD[CONST_DGN_iPOSNUM - 1] = dFxxD;
		m_FxxL[0] = m_FxxL[CONST_DGN_iPOSNUM - 1] = dFxxL;
		m_FxxR[0] = m_FxxR[CONST_DGN_iPOSNUM - 1] = dFxxR;
		m_FxxML[0] = m_FxxML[CONST_DGN_iPOSNUM - 1] = dFxxML;
		m_FxxTP[0] = m_FxxTP[CONST_DGN_iPOSNUM - 1] = dFxxTP;
		m_FxxTS[0] = m_FxxTS[CONST_DGN_iPOSNUM - 1] = dFxxTS;
		m_FxxLU[0] = m_FxxLU[CONST_DGN_iPOSNUM - 1] = dFxxLU;
	}

	SaveTotalForceForPlate(iElem, bPlate, iSeismicType);

	// Add by ZINU.('03.07.04). Moment Redistribution Factor by Kye CJ.
	// Change by ZINU.('05.09.08). Looping In -> Out.
	if (!bPlate) // Change by SHIN
	{
		Reset_MomentRedistribution();
	}

	Reset_TorsionReduction();

	// Add by ZINU.('03.01.08). For 2D or 3D Design.
	switch (m_iDgnMembAxis)
	{
	case 0: Reset_Force(); break; // NOT Design.
	case 1: Reset_ForceMinor(); break; // Only y-axis.
	case 2: Reset_ForceMajor(); break; // Only z-axis.
	case 3: break;
	default: ASSERT(0); break;
	}
}

BOOL CDgnForceCtrl::GetErectionLoadType(CString strErName, T_STCT_D &rStct, CString &strLCType)
{
	strLCType = _T("");
	for (INT_PTR i = 0; i < rStct.aErection.GetSize(); ++i)
	{
		const T_STCT_EREC &ErecD = rStct.aErection[i];
		if (strErName.CompareNoCase(ErecD.strErection) == 0)
		{
			if (ErecD.nNumStctCase > 0) // 혹시라도 strErName에 erection이 아닌 load case인데 strErection과 이름이 같을 수 있기 때문에 
			{
				strLCType = ErecD.LoadCaseType;
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CDgnForceCtrl::ApplyLoadContribution()
{
	// Add by ZINU.('05.08.24). D+L만의 부재력으로 구성된 경우, 나머지부재력에 대한 비율로는 처리가 안되므로 수정.
	// Change by ZINU.('06.11.03). D+L+R인 경우, R의 부재력이 0.0인 경우는 D+L만 있는 경우와 동일하게 처리.
	if (!m_bExistLctb) return;

	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		// If m_arRemnPos is NOT Existed, Calculate using m_FxxLctb,...
		// Change by ZINU.('06.11.03). R의 부재력이 0.0인 경우, ...Lctb[i]로 저장.
		//if(m_arRemnPos[i].GetCount() > 0)	continue;
		if (m_FxxR[i] == 0.0)	m_FxxR[i] = m_FxxLctb[i];
		if (m_FyyR[i] == 0.0)	m_FyyR[i] = m_FyyLctb[i];
		if (m_FzzR[i] == 0.0)	m_FzzR[i] = m_FzzLctb[i];
		if (m_BMxR[i] == 0.0)	m_BMxR[i] = m_BMxLctb[i];
		if (m_BMyR[i] == 0.0)	m_BMyR[i] = m_BMyLctb[i];
		if (m_BMzR[i] == 0.0)	m_BMzR[i] = m_BMzLctb[i];
		// Add by GAY. ('13.05.09). PMS:4632. Member forces 7-DOF element.
		if (m_MbiR[i] == 0.0)  m_MbiR[i] = m_MbiLctb[i];
		if (m_TstR[i] == 0.0)  m_TstR[i] = m_TstLctb[i];
		if (m_TwpR[i] == 0.0)  m_TwpR[i] = m_TwpLctb[i];

		for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			// If m_arRemnEnd is NOT Existed, Calculate using m_BMyeLctb,...
			// Change by ZINU.('06.11.03). R의 부재력이 0.0인 경우, ...Lctb[i]로 저장.
			//if(m_arRemnEnd[i][j].GetCount() > 0)	continue;
			if (m_BMyeR[0][i][j] == 0.0)	m_BMyeR[0][i][j] = m_BMyeLctb[0][i][j];
			if (m_BMyeR[1][i][j] == 0.0)	m_BMyeR[1][i][j] = m_BMyeLctb[1][i][j];
			if (m_BMzeR[0][i][j] == 0.0)	m_BMzeR[0][i][j] = m_BMzeLctb[0][i][j];
			if (m_BMzeR[1][i][j] == 0.0)	m_BMzeR[1][i][j] = m_BMzeLctb[1][i][j];
		}
	}
	for (int i = 0; i < 5; i++)
	{
		if (m_FxxR_UF[i] == 0.0)  m_FxxR_UF[i] = m_FxxUFLctb[i];
		if (m_FyyR_UF[i] == 0.0)  m_FyyR_UF[i] = m_FyyUFLctb[i];
		if (m_FxyR_UF[i] == 0.0)  m_FxyR_UF[i] = m_FxyUFLctb[i];
		if (m_VxxR_UF[i] == 0.0)  m_VxxR_UF[i] = m_VxxUFLctb[i];
		if (m_VyyR_UF[i] == 0.0)  m_VyyR_UF[i] = m_VyyUFLctb[i];
		if (m_MxxR_UF[i] == 0.0)  m_MxxR_UF[i] = m_MxxUFLctb[i];
		if (m_MyyR_UF[i] == 0.0)  m_MyyR_UF[i] = m_MyyUFLctb[i];
		if (m_MxyR_UF[i] == 0.0)  m_MxyR_UF[i] = m_MxyUFLctb[i];
	}
}

void CDgnForceCtrl::ApplyScaleUpFactorForColumn(int i)
{
	// Add by ZINU.('02.5.24). Apply Scale up Factor for column. (Only Civil).
	if (m_dUserCtcUpFactor > 1.0)
	{
		// Forces.
		MultiplyFxx(i, m_dUserCtcUpFactor);
		MultiplyFyy(i, m_dUserCtcUpFactor);
		MultiplyFzz(i, m_dUserCtcUpFactor);
		// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
		m_FyyOrgR[i] *= m_dUserCtcUpFactor; m_FzzOrgR[i] *= m_dUserCtcUpFactor;
		// Moments.
		MultiplyBMx(i, m_dUserCtcUpFactor);
		MultiplyBMy(i, m_dUserCtcUpFactor);
		MultiplyBMz(i, m_dUserCtcUpFactor);
		// Add by GAY. ('13.05.28). PMS:4632. Torsion + Warping for 7 DOF.
		MultiplyMbi(i, m_dUserCtcUpFactor);
		MultiplyTst(i, m_dUserCtcUpFactor);
		MultiplyTwp(i, m_dUserCtcUpFactor);
		// End Moments.
		MultiplyBMye(i, m_dUserCtcUpFactor);
		MultiplyBMze(i, m_dUserCtcUpFactor);
	}
}

void CDgnForceCtrl::ApplyEQFactorForLcom(int iElem, int iSeismicType, int i, BOOL bPlate)
{
	if (iSeismicType < 1) return; // Only EQ Lcom.

	if (!Get_EqFactorForLcom(iElem, i, iSeismicType)) ASSERT(0);
	// Add by ZINU.('02.7.13). See GB50010-02 11.4.7. p.176.

	if (!bPlate)
	{// Beam, Wall, Truss
		// Forces.
		if (m_dSueqLcomN > 1.0)
		{
			m_FxxD[i] *= m_dSueqLcomN; m_FxxL[i] *= m_dSueqLcomN; m_FxxR[i] *= m_dSueqLcomN; m_FxxML[i] *= m_dSueqLcomN; m_FxxTP[i] *= m_dSueqLcomN; m_FxxTS[i] *= m_dSueqLcomN;
		}
		if (m_dSueqLcomV > 1.0)
		{
			m_FyyD[i] *= m_dSueqLcomV; m_FyyL[i] *= m_dSueqLcomV; m_FyyR[i] *= m_dSueqLcomV; m_FyyML[i] *= m_dSueqLcomV; m_FyyTP[i] *= m_dSueqLcomV; m_FyyTS[i] *= m_dSueqLcomV; m_FyyOrgR[i] *= m_dSueqLcomV;
			m_FzzD[i] *= m_dSueqLcomV; m_FzzL[i] *= m_dSueqLcomV; m_FzzR[i] *= m_dSueqLcomV; m_FzzML[i] *= m_dSueqLcomV; m_FzzTP[i] *= m_dSueqLcomV; m_FzzTS[i] *= m_dSueqLcomV; m_FzzOrgR[i] *= m_dSueqLcomV;
		}

		if (m_dCodeLcomN > 1.0)
		{
			m_FxxD[i] *= m_dCodeLcomN; m_FxxL[i] *= m_dCodeLcomN; m_FxxR[i] *= m_dCodeLcomN; m_FxxML[i] *= m_dCodeLcomN; m_FxxTP[i] *= m_dCodeLcomN; m_FxxTS[i] *= m_dCodeLcomN;
		}
		if (m_dCodeLcomV > 1.0)
		{
			m_FyyD[i] *= m_dCodeLcomV; m_FyyL[i] *= m_dCodeLcomV; m_FyyR[i] *= m_dCodeLcomV; m_FyyML[i] *= m_dCodeLcomV; m_FyyTP[i] *= m_dCodeLcomV; m_FyyTS[i] *= m_dCodeLcomV;
			m_FzzD[i] *= m_dCodeLcomV; m_FzzL[i] *= m_dCodeLcomV; m_FzzR[i] *= m_dCodeLcomV; m_FzzML[i] *= m_dCodeLcomV; m_FzzTP[i] *= m_dCodeLcomV; m_FzzTS[i] *= m_dCodeLcomV;
		}

		// Moments.
		if (m_dSueqLcomM > 1.0)
		{
			// Not used m_BMxxD, ...
			m_BMyD[i] *= m_dSueqLcomM; m_BMyL[i] *= m_dSueqLcomM; m_BMyR[i] *= m_dSueqLcomM; m_BMyML[i] *= m_dSueqLcomM; m_BMyTP[i] *= m_dSueqLcomM; m_BMyTS[i] *= m_dSueqLcomM;
			m_BMzD[i] *= m_dSueqLcomM; m_BMzL[i] *= m_dSueqLcomM; m_BMzR[i] *= m_dSueqLcomM; m_BMzML[i] *= m_dSueqLcomM; m_BMzTP[i] *= m_dSueqLcomM; m_BMzTS[i] *= m_dSueqLcomM;
		}

		if (m_dCodeLcomM > 1.0)
		{
			// Not used m_BMxxD, ...
			m_BMyD[i] *= m_dCodeLcomM; m_BMyL[i] *= m_dCodeLcomM; m_BMyR[i] *= m_dCodeLcomM; m_BMyML[i] *= m_dCodeLcomM; m_BMyTP[i] *= m_dCodeLcomM; m_BMyTS[i] *= m_dCodeLcomM;
			m_BMzD[i] *= m_dCodeLcomM; m_BMzL[i] *= m_dCodeLcomM; m_BMzR[i] *= m_dCodeLcomM; m_BMzML[i] *= m_dCodeLcomM; m_BMzTP[i] *= m_dCodeLcomM; m_BMzTS[i] *= m_dCodeLcomM;
		}
	}
	else
	{// Plate
		// Forces.
		if (m_dSueqLcomN > 1.0)
		{
			m_FxxD[i] *= m_dSueqLcomN; m_FxxL[i] *= m_dSueqLcomN; m_FxxR[i] *= m_dSueqLcomN; m_FxxML[i] *= m_dSueqLcomN; m_FxxTP[i] *= m_dSueqLcomN; m_FxxTS[i] *= m_dSueqLcomN;
			m_FyyD[i] *= m_dSueqLcomN; m_FyyL[i] *= m_dSueqLcomN; m_FyyR[i] *= m_dSueqLcomN; m_FyyML[i] *= m_dSueqLcomN; m_FyyTP[i] *= m_dSueqLcomN; m_FyyTS[i] *= m_dSueqLcomN;
		}
		if (m_dSueqLcomV > 1.0)
		{
			m_FzzD[i] *= m_dSueqLcomV; m_FzzL[i] *= m_dSueqLcomV; m_FzzR[i] *= m_dSueqLcomV; m_FzzML[i] *= m_dSueqLcomV; m_FzzTP[i] *= m_dSueqLcomV; m_FzzTS[i] *= m_dSueqLcomV; m_FzzOrgR[i] *= m_dSueqLcomV;
		}

		if (m_dCodeLcomN > 1.0)
		{
			m_FxxD[i] *= m_dCodeLcomN; m_FxxL[i] *= m_dCodeLcomN; m_FxxR[i] *= m_dCodeLcomN; m_FxxML[i] *= m_dCodeLcomN; m_FxxTP[i] *= m_dCodeLcomN; m_FxxTS[i] *= m_dCodeLcomN;
			m_FyyD[i] *= m_dCodeLcomN; m_FyyL[i] *= m_dCodeLcomN; m_FyyR[i] *= m_dCodeLcomN; m_FyyML[i] *= m_dCodeLcomN; m_FyyTP[i] *= m_dCodeLcomN; m_FyyTS[i] *= m_dCodeLcomN;
		}
		if (m_dCodeLcomV > 1.0)
		{
			m_FzzD[i] *= m_dCodeLcomV; m_FzzL[i] *= m_dCodeLcomV; m_FzzR[i] *= m_dCodeLcomV; m_FzzML[i] *= m_dCodeLcomV; m_FzzTP[i] *= m_dCodeLcomV; m_FzzTS[i] *= m_dCodeLcomV;
		}

		// Moments.				
		if (m_dSueqLcomM > 1.0)
		{
			m_BMxD[i] *= m_dSueqLcomM; m_BMxL[i] *= m_dSueqLcomM; m_BMxR[i] *= m_dSueqLcomM; m_BMxML[i] *= m_dSueqLcomM; m_BMxTP[i] *= m_dSueqLcomM; m_BMxTS[i] *= m_dSueqLcomM;
			m_BMyD[i] *= m_dSueqLcomM; m_BMyL[i] *= m_dSueqLcomM; m_BMyR[i] *= m_dSueqLcomM; m_BMyML[i] *= m_dSueqLcomM; m_BMyTP[i] *= m_dSueqLcomM; m_BMyTS[i] *= m_dSueqLcomM;
			// Not used m_BMzD, ...
		}

		if (m_dCodeLcomM > 1.0)
		{
			m_BMxD[i] *= m_dCodeLcomM; m_BMxL[i] *= m_dCodeLcomM; m_BMxR[i] *= m_dCodeLcomM; m_BMxML[i] *= m_dCodeLcomM; m_BMxTP[i] *= m_dCodeLcomM; m_BMxTS[i] *= m_dCodeLcomM;
			m_BMyD[i] *= m_dCodeLcomM; m_BMyL[i] *= m_dCodeLcomM; m_BMyR[i] *= m_dCodeLcomM; m_BMyML[i] *= m_dCodeLcomM; m_BMyTP[i] *= m_dCodeLcomM; m_BMyTS[i] *= m_dCodeLcomM;
			// Not used m_BMzD, ...
		}
	}

	// End Moments.
	for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
	{
		if (m_dSueqLcomM > 1.0)
		{
			m_BMyeD[0][i][j] *= m_dSueqLcomM; m_BMyeD[1][i][j] *= m_dSueqLcomM; m_BMzeD[0][i][j] *= m_dSueqLcomM; m_BMzeD[1][i][j] *= m_dSueqLcomM;
			m_BMyeL[0][i][j] *= m_dSueqLcomM; m_BMyeL[1][i][j] *= m_dSueqLcomM; m_BMzeL[0][i][j] *= m_dSueqLcomM; m_BMzeL[1][i][j] *= m_dSueqLcomM;
			m_BMyeML[0][i][j] *= m_dSueqLcomM; m_BMyeML[1][i][j] *= m_dSueqLcomM; m_BMzeML[0][i][j] *= m_dSueqLcomM; m_BMzeML[1][i][j] *= m_dSueqLcomM;
			m_BMyeR[0][i][j] *= m_dSueqLcomM; m_BMyeR[1][i][j] *= m_dSueqLcomM; m_BMzeR[0][i][j] *= m_dSueqLcomM; m_BMzeR[1][i][j] *= m_dSueqLcomM;
			m_BMyeTP[0][i][j] *= m_dSueqLcomM; m_BMyeTP[1][i][j] *= m_dSueqLcomM; m_BMzeTP[0][i][j] *= m_dSueqLcomM; m_BMzeTP[1][i][j] *= m_dSueqLcomM;
			m_BMyeTS[0][i][j] *= m_dSueqLcomM; m_BMyeTS[1][i][j] *= m_dSueqLcomM; m_BMzeTS[0][i][j] *= m_dSueqLcomM; m_BMzeTS[1][i][j] *= m_dSueqLcomM;
			m_BMyeLU[0][i][j] *= m_dSueqLcomM; m_BMyeLU[1][i][j] *= m_dSueqLcomM; m_BMzeLU[0][i][j] *= m_dSueqLcomM; m_BMzeLU[1][i][j] *= m_dSueqLcomM;
		}

		if (m_dCodeLcomM > 1.0)
		{
			m_BMyeD[0][i][j] *= m_dCodeLcomM; m_BMyeD[1][i][j] *= m_dCodeLcomM; m_BMzeD[0][i][j] *= m_dCodeLcomM; m_BMzeD[1][i][j] *= m_dCodeLcomM;
			m_BMyeL[0][i][j] *= m_dCodeLcomM; m_BMyeL[1][i][j] *= m_dCodeLcomM; m_BMzeL[0][i][j] *= m_dCodeLcomM; m_BMzeL[1][i][j] *= m_dCodeLcomM;
			m_BMyeML[0][i][j] *= m_dCodeLcomM; m_BMyeML[1][i][j] *= m_dCodeLcomM; m_BMzeML[0][i][j] *= m_dCodeLcomM; m_BMzeML[1][i][j] *= m_dCodeLcomM;
			m_BMyeR[0][i][j] *= m_dCodeLcomM; m_BMyeR[1][i][j] *= m_dCodeLcomM; m_BMzeR[0][i][j] *= m_dCodeLcomM; m_BMzeR[1][i][j] *= m_dCodeLcomM;
			m_BMyeTP[0][i][j] *= m_dCodeLcomM; m_BMyeTP[1][i][j] *= m_dCodeLcomM; m_BMzeTP[0][i][j] *= m_dCodeLcomM; m_BMzeTP[1][i][j] *= m_dCodeLcomM;
			m_BMyeTS[0][i][j] *= m_dCodeLcomM; m_BMyeTS[1][i][j] *= m_dCodeLcomM; m_BMzeTS[0][i][j] *= m_dCodeLcomM; m_BMzeTS[1][i][j] *= m_dCodeLcomM;
			m_BMyeLU[0][i][j] *= m_dCodeLcomM; m_BMyeLU[1][i][j] *= m_dCodeLcomM; m_BMzeLU[0][i][j] *= m_dCodeLcomM; m_BMzeLU[1][i][j] *= m_dCodeLcomM;
		}
	}
}

void CDgnForceCtrl::ApplySafeFactorGB(int i)
{
	// Add by ZINU.('02.7.10). See GB50010-02 3.2.3. p.11.
	int iNoEqGB = GetNoEqGB();
	if (iNoEqGB < 1) return;

	double dSafeFactorGB = 1.0;
	switch (iNoEqGB)
	{
	case 1: dSafeFactorGB = 1.1; break;
	case 2: dSafeFactorGB = 1.0; break;
	case 3: dSafeFactorGB = 0.9; break;
	default: ASSERT(0); break;
	}

	// Forces.
	MultiplyFxx(i, dSafeFactorGB);
	MultiplyFyy(i, dSafeFactorGB);
	MultiplyFzz(i, dSafeFactorGB);
	// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
	m_FyyOrgR[i] *= dSafeFactorGB; m_FzzOrgR[i] *= dSafeFactorGB;
	// Moments.
	MultiplyBMx(i, dSafeFactorGB);
	MultiplyBMy(i, dSafeFactorGB);
	MultiplyBMz(i, dSafeFactorGB);
	// Add by GAY. ('13.05.28). PMS:4632.  Torsion+Warping for 7 DOF.	
	MultiplyMbi(i, dSafeFactorGB);
	MultiplyTst(i, dSafeFactorGB);
	MultiplyTwp(i, dSafeFactorGB);
	// End Moments.
	MultiplyBMye(i, dSafeFactorGB);
	MultiplyBMze(i, dSafeFactorGB);

	Reset_UncertainlyLoadCombFact(i);
}

void CDgnForceCtrl::SetSmallValueAsZero(int i)
{
	// Add by ZINU.('01.6.18). If force is under cDgn_Zero, force is Zero.
	// Change by ZINU.('04.12.06). dDGN_Zero -> dDgn_LimF.
	// Change by ZINU.('05.05.23). Add if under dZeroF, dZeroM (N,mm).
	double dZeroLen = 0.0, dZeroFor = 0.0;
	if (!Get_ZeroFactor(dZeroLen, dZeroFor))	ASSERT(0);
	// Change by ZINU.('06.08.01). dDgn_LimF -> 0.01 at dZeroF, dDgn_LimF -> 1.0 at dZeroM.
	double dZeroF = 0.01 * dZeroFor;						// Zero Force.
	double dZeroM = 1.0  * (dZeroFor*dZeroLen);	// Zero Moment.

	// Forces.
	if (IsRegardZeroForce(m_FxxD, i, dDgn_LimF, dZeroF)) m_FxxD[i] = 0.0;
	if (IsRegardZeroForce(m_FxxL, i, dDgn_LimF, dZeroF)) m_FxxL[i] = 0.0;
	if (IsRegardZeroForce(m_FxxR, i, dDgn_LimF, dZeroF)) m_FxxR[i] = 0.0;
	if (IsRegardZeroForce(m_FxxML, i, dDgn_LimF, dZeroF)) m_FxxML[i] = 0.0;
	if (IsRegardZeroForce(m_FxxTP, i, dDgn_LimF, dZeroF)) m_FxxTP[i] = 0.0;
	if (IsRegardZeroForce(m_FxxTS, i, dDgn_LimF, dZeroF)) m_FxxTS[i] = 0.0;
	if (IsRegardZeroForce(m_FxxLU, i, dDgn_LimF, dZeroF)) m_FxxLU[i] = 0.0;

	if (IsRegardZeroForce(m_FyyD, i, dDgn_LimF, dZeroF)) m_FyyD[i] = 0.0;
	if (IsRegardZeroForce(m_FyyL, i, dDgn_LimF, dZeroF)) m_FyyL[i] = 0.0;
	if (IsRegardZeroForce(m_FyyR, i, dDgn_LimF, dZeroF)) m_FyyR[i] = 0.0;
	if (IsRegardZeroForce(m_FyyML, i, dDgn_LimF, dZeroF)) m_FyyML[i] = 0.0;
	if (IsRegardZeroForce(m_FyyTP, i, dDgn_LimF, dZeroF)) m_FyyTP[i] = 0.0;
	if (IsRegardZeroForce(m_FyyTS, i, dDgn_LimF, dZeroF)) m_FyyTS[i] = 0.0;
	if (IsRegardZeroForce(m_FyyLU, i, dDgn_LimF, dZeroF)) m_FyyLU[i] = 0.0;

	if (IsRegardZeroForce(m_FzzD, i, dDgn_LimF, dZeroF)) m_FzzD[i] = 0.0;
	if (IsRegardZeroForce(m_FzzL, i, dDgn_LimF, dZeroF)) m_FzzL[i] = 0.0;
	if (IsRegardZeroForce(m_FzzR, i, dDgn_LimF, dZeroF)) m_FzzR[i] = 0.0;
	if (IsRegardZeroForce(m_FzzML, i, dDgn_LimF, dZeroF)) m_FzzML[i] = 0.0;
	if (IsRegardZeroForce(m_FzzTP, i, dDgn_LimF, dZeroF)) m_FzzTP[i] = 0.0;
	if (IsRegardZeroForce(m_FzzTS, i, dDgn_LimF, dZeroF)) m_FzzTS[i] = 0.0;
	if (IsRegardZeroForce(m_FzzLU, i, dDgn_LimF, dZeroF)) m_FzzLU[i] = 0.0;

	// Change by ZINU.('04.06.11). Set Zero to MinMoment if My/Mz or Mz/My is under dDGN_Zero.
	// Change by ZINU.('04.12.06). dDGN_Zero -> dDgn_LimF.
	// Moments.
	if (IsRegardZeroTorsion(m_BMxD, i, dDgn_LimF, dZeroM)) m_BMxD[i] = 0.0;
	if (IsRegardZeroTorsion(m_BMxL, i, dDgn_LimF, dZeroM)) m_BMxL[i] = 0.0;
	if (IsRegardZeroTorsion(m_BMxR, i, dDgn_LimF, dZeroM)) m_BMxR[i] = 0.0;
	if (IsRegardZeroTorsion(m_BMxML, i, dDgn_LimF, dZeroM)) m_BMxML[i] = 0.0;
	if (IsRegardZeroTorsion(m_BMxTP, i, dDgn_LimF, dZeroM)) m_BMxTP[i] = 0.0;
	if (IsRegardZeroTorsion(m_BMxTS, i, dDgn_LimF, dZeroM)) m_BMxTS[i] = 0.0;
	if (IsRegardZeroTorsion(m_BMxLU, i, dDgn_LimF, dZeroM)) m_BMxLU[i] = 0.0;

	bool bAdj = m_strMatType == _T("C") ? true : false;
	const bool bWall = m_pDoc->m_pAttrCtrl->IsWall(m_iElemType) ? true : false;

	auto L_SetMoment0_OrthoAxis = [](OUT double& rdCurrent, double dOrtho)
	{
		if (fabs(rdCurrent) < dDgn_LimF * fabs(dOrtho)) { rdCurrent = 0.0; }
	};

	if (!bWall && bAdj)
	{	
		L_SetMoment0_OrthoAxis(m_BMyD[i], m_BMzD[i]);
		L_SetMoment0_OrthoAxis(m_BMyL[i], m_BMzL[i]);
		L_SetMoment0_OrthoAxis(m_BMyR[i], m_BMzR[i]);		
		L_SetMoment0_OrthoAxis(m_BMyML[i], m_BMzML[i]);
		L_SetMoment0_OrthoAxis(m_BMyTP[i], m_BMzTP[i]);
		L_SetMoment0_OrthoAxis(m_BMyTS[i], m_BMzTS[i]);
		L_SetMoment0_OrthoAxis(m_BMyLU[i], m_BMzLU[i]);

		L_SetMoment0_OrthoAxis(m_BMzD[i], m_BMyD[i]);
		L_SetMoment0_OrthoAxis(m_BMzL[i], m_BMyL[i]);
		L_SetMoment0_OrthoAxis(m_BMzR[i], m_BMyR[i]);
		L_SetMoment0_OrthoAxis(m_BMzML[i], m_BMyML[i]);
		L_SetMoment0_OrthoAxis(m_BMzTP[i], m_BMyTP[i]);
		L_SetMoment0_OrthoAxis(m_BMzTS[i], m_BMyTS[i]);
		L_SetMoment0_OrthoAxis(m_BMzLU[i], m_BMyLU[i]);
	}

	auto L_SetMoment0_EachAxis = [dZeroM](OUT double& rdMoment)
	{
		if (fabs(rdMoment) < dZeroM) { rdMoment = 0.0; }
	};

	L_SetMoment0_EachAxis(m_BMyD[i]);
	L_SetMoment0_EachAxis(m_BMyL[i]);
	L_SetMoment0_EachAxis(m_BMyR[i]);
	L_SetMoment0_EachAxis(m_BMyML[i]);
	L_SetMoment0_EachAxis(m_BMyTP[i]);
	L_SetMoment0_EachAxis(m_BMyTS[i]);
	L_SetMoment0_EachAxis(m_BMyLU[i]);

	L_SetMoment0_EachAxis(m_BMzD[i]);
	L_SetMoment0_EachAxis(m_BMzL[i]);
	L_SetMoment0_EachAxis(m_BMzR[i]);
	L_SetMoment0_EachAxis(m_BMzML[i]);
	L_SetMoment0_EachAxis(m_BMzTP[i]);
	L_SetMoment0_EachAxis(m_BMzTS[i]);
	L_SetMoment0_EachAxis(m_BMzLU[i]);

	// Add by GAY. ('13.05.28). PMS:4632.  Torsion+Warping for 7 DOF.
	if (IsRegardZeroTorsion(m_MbiD, i, dDgn_LimF, dZeroM)) m_MbiD[i] = 0.0;
	if (IsRegardZeroTorsion(m_MbiL, i, dDgn_LimF, dZeroM)) m_MbiL[i] = 0.0;
	if (IsRegardZeroTorsion(m_MbiR, i, dDgn_LimF, dZeroM)) m_MbiR[i] = 0.0;
	if (IsRegardZeroTorsion(m_MbiML, i, dDgn_LimF, dZeroM)) m_MbiML[i] = 0.0;
	if (IsRegardZeroTorsion(m_MbiTP, i, dDgn_LimF, dZeroM)) m_MbiTP[i] = 0.0;
	if (IsRegardZeroTorsion(m_MbiTS, i, dDgn_LimF, dZeroM)) m_MbiTS[i] = 0.0;
	if (IsRegardZeroTorsion(m_MbiLU, i, dDgn_LimF, dZeroM)) m_MbiLU[i] = 0.0;
	
	// End Moments.
	if (bAdj)
	{
		for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
		{
			for (int m = 0; m < 2; ++m)
			{
				L_SetMoment0_OrthoAxis(m_BMyeD[m][i][j], m_BMzeD[m][i][j]);
				L_SetMoment0_OrthoAxis(m_BMyeL[m][i][j], m_BMzeL[m][i][j]);
				L_SetMoment0_OrthoAxis(m_BMyeML[m][i][j], m_BMzeML[m][i][j]);
				L_SetMoment0_OrthoAxis(m_BMyeR[m][i][j], m_BMzeR[m][i][j]);
				L_SetMoment0_OrthoAxis(m_BMyeTP[m][i][j], m_BMzeTP[m][i][j]);
				L_SetMoment0_OrthoAxis(m_BMyeTS[m][i][j], m_BMzeTS[m][i][j]);
				L_SetMoment0_OrthoAxis(m_BMyeLU[m][i][j], m_BMzeLU[m][i][j]);

				L_SetMoment0_OrthoAxis(m_BMzeD[m][i][j], m_BMyeD[m][i][j]);
				L_SetMoment0_OrthoAxis(m_BMzeL[m][i][j], m_BMyeL[m][i][j]);
				L_SetMoment0_OrthoAxis(m_BMzeML[m][i][j], m_BMyeML[m][i][j]);
				L_SetMoment0_OrthoAxis(m_BMzeR[m][i][j], m_BMyeR[m][i][j]);
				L_SetMoment0_OrthoAxis(m_BMzeTP[m][i][j], m_BMyeTP[m][i][j]);
				L_SetMoment0_OrthoAxis(m_BMzeTS[m][i][j], m_BMyeTS[m][i][j]);
				L_SetMoment0_OrthoAxis(m_BMzeLU[m][i][j], m_BMyeLU[m][i][j]);
			}					
		}
	}

	for (int j = 0; j < CONST_DGN_iPOSNUM; j++)
	{
		for (int m = 0; m < 2; ++m)
		{
			L_SetMoment0_EachAxis(m_BMyeD[m][i][j]);
			L_SetMoment0_EachAxis(m_BMyeL[m][i][j]);
			L_SetMoment0_EachAxis(m_BMyeML[m][i][j]);
			L_SetMoment0_EachAxis(m_BMyeR[m][i][j]);
			L_SetMoment0_EachAxis(m_BMyeTP[m][i][j]);
			L_SetMoment0_EachAxis(m_BMyeTS[m][i][j]);
			L_SetMoment0_EachAxis(m_BMyeLU[m][i][j]);

			L_SetMoment0_EachAxis(m_BMzeD[m][i][j]);
			L_SetMoment0_EachAxis(m_BMzeL[m][i][j]);
			L_SetMoment0_EachAxis(m_BMzeML[m][i][j]);
			L_SetMoment0_EachAxis(m_BMzeR[m][i][j]);
			L_SetMoment0_EachAxis(m_BMzeTP[m][i][j]);
			L_SetMoment0_EachAxis(m_BMzeTS[m][i][j]);
			L_SetMoment0_EachAxis(m_BMzeLU[m][i][j]);
		}		
	}	
}

void CDgnForceCtrl::ChangeAxialForceSign(int i, BOOL bPlate)
{
	// if Concrete, (+) is Compression. else Steel/SRC, (+) is Tension.
	if (bPlate) return;
	if (m_strMatType != _T("C")) return;

	m_FxxD[i] = (-1.0) * m_FxxD[i];
	m_FxxL[i] = (-1.0) * m_FxxL[i];
	m_FxxR[i] = (-1.0) * m_FxxR[i];
	m_FxxML[i] = (-1.0) * m_FxxML[i];
	m_FxxTP[i] = (-1.0) * m_FxxTP[i];
	m_FxxTS[i] = (-1.0) * m_FxxTS[i];
	m_FxxLU[i] = (-1.0) * m_FxxLU[i];
}

void CDgnForceCtrl::SaveTotalForceForPlate(int iElem, BOOL bPlate, int iSeismicType)
{
	if (!bPlate) return;

	PLATE_UNIT_FORCE maxUF;
	GetMaxPlateUnitForce(maxUF);

	// Add by SHIN('07.06.29) MNET:2668 For RC-Plate Dgn
	for (int i = 0; i < 5; i++)
	{
		ApplyScaleUpFactorForPlate(i); //Civil Only.

		ApplyEQFactorForLcomPlate(iElem, iSeismicType, i);

		ApplySafeFactorGBPlate(i);

		// Add by ZINU.('02.5.23). Include Moving Live Load to Live Load.
		// Forces.
		m_FxxL_UF[i] += m_FxxML_UF[i];
		m_FyyL_UF[i] += m_FyyML_UF[i];
		m_FxyL_UF[i] += m_FxyML_UF[i];
		m_VxxL_UF[i] += m_VxxML_UF[i];
		m_VyyL_UF[i] += m_VyyML_UF[i];
		// Moments.
		m_MxxL_UF[i] += m_MxxML_UF[i];
		m_MyyL_UF[i] += m_MyyML_UF[i];
		m_MxyL_UF[i] += m_MxyML_UF[i];

		// Add by ZINU.('02.12.11). Include Tendon Primary/Secondary Load to Dead Load.
		// Forces.
		m_FxxD_UF[i] += m_FxxTP_UF[i] + m_FxxTS_UF[i];
		m_FyyD_UF[i] += m_FyyTP_UF[i] + m_FyyTS_UF[i];
		m_FxyD_UF[i] += m_FxyTP_UF[i] + m_FxyTS_UF[i];
		m_VxxD_UF[i] += m_VxxTP_UF[i] + m_VxxTS_UF[i];
		m_VyyD_UF[i] += m_VyyTP_UF[i] + m_VyyTS_UF[i];
		// Moments.
		m_MxxD_UF[i] += m_MxxTP_UF[i] + m_MxxTS_UF[i];
		m_MyyD_UF[i] += m_MyyTP_UF[i] + m_MyyTS_UF[i];
		m_MxyD_UF[i] += m_MxyTP_UF[i] + m_MxyTS_UF[i];

		SetSmallValueAsZeroPlate(i, maxUF);

		ChangeAxialForceSignPlate(i);

		m_Fxx_UF[i] = m_FxxD_UF[i] + m_FxxL_UF[i] + m_FxxR_UF[i] + m_FxxLU_UF[i];
		m_Fyy_UF[i] = m_FyyD_UF[i] + m_FyyL_UF[i] + m_FyyR_UF[i] + m_FyyLU_UF[i];
		m_Fxy_UF[i] = m_FxyD_UF[i] + m_FxyL_UF[i] + m_FxyR_UF[i] + m_FxyLU_UF[i];
		m_Vxx_UF[i] = m_VxxD_UF[i] + m_VxxL_UF[i] + m_VxxR_UF[i] + m_VxxLU_UF[i];
		m_Vyy_UF[i] = m_VyyD_UF[i] + m_VyyL_UF[i] + m_VyyR_UF[i] + m_VyyLU_UF[i];
		m_Mxx_UF[i] = m_MxxD_UF[i] + m_MxxL_UF[i] + m_MxxR_UF[i] + m_MxxLU_UF[i];
		m_Myy_UF[i] = m_MyyD_UF[i] + m_MyyL_UF[i] + m_MyyR_UF[i] + m_MyyLU_UF[i];
		m_Mxy_UF[i] = m_MxyD_UF[i] + m_MxyL_UF[i] + m_MxyR_UF[i] + m_MxyLU_UF[i];
	}
}

void CDgnForceCtrl::ApplyScaleUpFactorForPlate(int i)
{
	// Add by ZINU.('02.5.24). Apply Scale up Factor for column. (Only Civil).
	if (m_dUserCtcUpFactor > 1.0)
	{
		// Note - by SHIN : Plate 요소는 CTC고려하지 않음
	}
}

void CDgnForceCtrl::ApplyEQFactorForLcomPlate(int iElem, int iSeismicType, int i)
{
	if (iSeismicType < 1) return; // Only EQ Lcom.

	// Note - by SHIN : Plate 요소는 현재(07.06.29) ChinaCode를 지원하지 않으므로 EqFactor값이 1.0이 넘어옴
	//                  (m_dSueqLcomN,m_dSueqLcomM,m_dSueqLcomV,m_dCodeLcomN,m_dCodeLcomM,m_dCodeLcomV)
	if (!Get_EqFactorForLcom(iElem, i, iSeismicType))	ASSERT(0);
	// Add by ZINU.('02.7.13). See GB50010-02 11.4.7. p.176.
	// Forces.
	if (m_dSueqLcomN > 1.0)
	{
		MultiplyFxxUF(i, m_dSueqLcomN);
		MultiplyFyyUF(i, m_dSueqLcomN);
	}
	if (m_dSueqLcomV > 1.0)
	{
		MultiplyFxyUF(i, m_dSueqLcomV);
		MultiplyVxxUF(i, m_dSueqLcomV);
		MultiplyVyyUF(i, m_dSueqLcomV);
		m_FxyUFOrgR[i] *= m_dSueqLcomV;
		m_VxxUFOrgR[i] *= m_dSueqLcomV;
		m_VyyUFOrgR[i] *= m_dSueqLcomV;
	}

	if (m_dCodeLcomN > 1.0)
	{
		MultiplyFxxUF(i, m_dCodeLcomN);
		MultiplyFyyUF(i, m_dCodeLcomN);
	}
	if (m_dCodeLcomV > 1.0)
	{
		MultiplyFxyUF(i, m_dCodeLcomV);
		MultiplyVxxUF(i, m_dCodeLcomV);
		MultiplyVyyUF(i, m_dCodeLcomV);
	}

	// Moments.			
	if (m_dSueqLcomM > 1.0)
	{
		MultiplyMyyUF(i, m_dSueqLcomM);
		MultiplyMxxUF(i, m_dSueqLcomM);
		// Not used m_MxyD_UF, ...
	}

	if (m_dCodeLcomM > 1.0)
	{
		MultiplyMyyUF(i, m_dCodeLcomM);
		MultiplyMxxUF(i, m_dCodeLcomM);
		// Not used m_MxyD_UF, ...
	}
}

void CDgnForceCtrl::ApplySafeFactorGBPlate(int i)
{
	// Add by ZINU.('02.7.10). See GB50010-02 3.2.3. p.11.
	int iNoEqGB = GetNoEqGB();
	if (iNoEqGB < 1) return;

	ASSERT(0); // Note - by SHIN : Plate 요소는 현재(07.06.29) ChinaCode를 지원하지 않으므로 이곳으로 들어 오지 않음

	double dSafeFactorGB = 1.0;
	switch (iNoEqGB)
	{
	case 1: dSafeFactorGB = 1.1; break;
	case 2: dSafeFactorGB = 1.0; break;
	case 3: dSafeFactorGB = 0.9; break;
	default: ASSERT(0); break;
	}

	// Forces.
	MultiplyFxxUF(i, dSafeFactorGB);
	MultiplyFyyUF(i, dSafeFactorGB);
	MultiplyFxyUF(i, dSafeFactorGB);
	MultiplyVxxUF(i, dSafeFactorGB);
	MultiplyVyyUF(i, dSafeFactorGB);
	// Add by ZINU.('05.06.25). FxyUFR, VxxUFR, VyyUFR at Original Lcom.
	m_FxyUFOrgR[i] *= dSafeFactorGB; m_VxxUFOrgR[i] *= dSafeFactorGB; m_VyyUFOrgR[i] *= dSafeFactorGB;
	// Moments.
	MultiplyMxxUF(i, dSafeFactorGB);
	MultiplyMyyUF(i, dSafeFactorGB);
	MultiplyMxyUF(i, dSafeFactorGB);
}

void CDgnForceCtrl::GetMaxPlateUnitForce(PLATE_UNIT_FORCE &rMaxUF)
{
	rMaxUF.Initialize();

	for (int i = 0; i < 5; i++)
	{
		rMaxUF.dFxxD = max(rMaxUF.dFxxD, fabs(m_FxxD_UF[i]));
		rMaxUF.dFxxL = max(rMaxUF.dFxxL, fabs(m_FxxL_UF[i]));
		rMaxUF.dFxxR = max(rMaxUF.dFxxR, fabs(m_FxxR_UF[i]));
		rMaxUF.dFxxML = max(rMaxUF.dFxxML, fabs(m_FxxML_UF[i]));
		rMaxUF.dFxxTP = max(rMaxUF.dFxxTP, fabs(m_FxxTP_UF[i]));
		rMaxUF.dFxxTS = max(rMaxUF.dFxxTS, fabs(m_FxxTS_UF[i]));
		rMaxUF.dFxxLU = max(rMaxUF.dFxxLU, fabs(m_FxxLU_UF[i]));

		rMaxUF.dFyyD = max(rMaxUF.dFyyD, fabs(m_FyyD_UF[i]));
		rMaxUF.dFyyL = max(rMaxUF.dFyyL, fabs(m_FyyL_UF[i]));
		rMaxUF.dFyyR = max(rMaxUF.dFyyR, fabs(m_FyyR_UF[i]));
		rMaxUF.dFyyML = max(rMaxUF.dFyyML, fabs(m_FyyML_UF[i]));
		rMaxUF.dFyyTP = max(rMaxUF.dFyyTP, fabs(m_FyyTP_UF[i]));
		rMaxUF.dFyyTS = max(rMaxUF.dFyyTS, fabs(m_FyyTS_UF[i]));
		rMaxUF.dFyyLU = max(rMaxUF.dFyyLU, fabs(m_FyyLU_UF[i]));

		rMaxUF.dFxyD = max(rMaxUF.dFxyD, fabs(m_FxyD_UF[i]));
		rMaxUF.dFxyL = max(rMaxUF.dFxyL, fabs(m_FxyL_UF[i]));
		rMaxUF.dFxyR = max(rMaxUF.dFxyR, fabs(m_FxyR_UF[i]));
		rMaxUF.dFxyML = max(rMaxUF.dFxyML, fabs(m_FxyML_UF[i]));
		rMaxUF.dFxyTP = max(rMaxUF.dFxyTP, fabs(m_FxyTP_UF[i]));
		rMaxUF.dFxyTS = max(rMaxUF.dFxyTS, fabs(m_FxyTS_UF[i]));
		rMaxUF.dFxyLU = max(rMaxUF.dFxyLU, fabs(m_FxyLU_UF[i]));

		rMaxUF.dVxxD = max(rMaxUF.dVxxD, fabs(m_VxxD_UF[i]));
		rMaxUF.dVxxL = max(rMaxUF.dVxxL, fabs(m_VxxL_UF[i]));
		rMaxUF.dVxxR = max(rMaxUF.dVxxR, fabs(m_VxxR_UF[i]));
		rMaxUF.dVxxML = max(rMaxUF.dVxxML, fabs(m_VxxML_UF[i]));
		rMaxUF.dVxxTP = max(rMaxUF.dVxxTP, fabs(m_VxxTP_UF[i]));
		rMaxUF.dVxxTS = max(rMaxUF.dVxxTS, fabs(m_VxxTS_UF[i]));
		rMaxUF.dVxxLU = max(rMaxUF.dVxxLU, fabs(m_VxxLU_UF[i]));

		rMaxUF.dVyyD = max(rMaxUF.dVyyD, fabs(m_VyyD_UF[i]));
		rMaxUF.dVyyL = max(rMaxUF.dVyyL, fabs(m_VyyL_UF[i]));
		rMaxUF.dVyyR = max(rMaxUF.dVyyR, fabs(m_VyyR_UF[i]));
		rMaxUF.dVyyML = max(rMaxUF.dVyyML, fabs(m_VyyML_UF[i]));
		rMaxUF.dVyyTP = max(rMaxUF.dVyyTP, fabs(m_VyyTP_UF[i]));
		rMaxUF.dVyyTS = max(rMaxUF.dVyyTS, fabs(m_VyyTS_UF[i]));
		rMaxUF.dVyyLU = max(rMaxUF.dVyyLU, fabs(m_VyyLU_UF[i]));

		rMaxUF.dMxxD = max(rMaxUF.dMxxD, fabs(m_MxxD_UF[i]));
		rMaxUF.dMxxL = max(rMaxUF.dMxxL, fabs(m_MxxL_UF[i]));
		rMaxUF.dMxxR = max(rMaxUF.dMxxR, fabs(m_MxxR_UF[i]));
		rMaxUF.dMxxML = max(rMaxUF.dMxxML, fabs(m_MxxML_UF[i]));
		rMaxUF.dMxxTP = max(rMaxUF.dMxxTP, fabs(m_MxxTP_UF[i]));
		rMaxUF.dMxxTS = max(rMaxUF.dMxxTS, fabs(m_MxxTS_UF[i]));
		rMaxUF.dMxxLU = max(rMaxUF.dMxxLU, fabs(m_MxxLU_UF[i]));

		rMaxUF.dMyyD = max(rMaxUF.dMyyD, fabs(m_MyyD_UF[i]));
		rMaxUF.dMyyL = max(rMaxUF.dMyyL, fabs(m_MyyL_UF[i]));
		rMaxUF.dMyyR = max(rMaxUF.dMyyR, fabs(m_MyyR_UF[i]));
		rMaxUF.dMyyML = max(rMaxUF.dMyyML, fabs(m_MyyML_UF[i]));
		rMaxUF.dMyyTP = max(rMaxUF.dMyyTP, fabs(m_MyyTP_UF[i]));
		rMaxUF.dMyyTS = max(rMaxUF.dMyyTS, fabs(m_MyyTS_UF[i]));
		rMaxUF.dMyyLU = max(rMaxUF.dMyyLU, fabs(m_MyyLU_UF[i]));

		rMaxUF.dMxyD = max(rMaxUF.dMxyD, fabs(m_MxyD_UF[i]));
		rMaxUF.dMxyL = max(rMaxUF.dMxyL, fabs(m_MxyL_UF[i]));
		rMaxUF.dMxyR = max(rMaxUF.dMxyR, fabs(m_MxyR_UF[i]));
		rMaxUF.dMxyML = max(rMaxUF.dMxyML, fabs(m_MxyML_UF[i]));
		rMaxUF.dMxyTP = max(rMaxUF.dMxyTP, fabs(m_MxyTP_UF[i]));
		rMaxUF.dMxyTS = max(rMaxUF.dMxyTS, fabs(m_MxyTS_UF[i]));
		rMaxUF.dMxyLU = max(rMaxUF.dMxyLU, fabs(m_MxyLU_UF[i]));
	}
}

void CDgnForceCtrl::SetSmallValueAsZeroPlate(int i, const PLATE_UNIT_FORCE &rMaxUF)
{
	// Add by ZINU.('01.6.18). If force is under cDgn_Zero, force is Zero.
	// Change by ZINU.('04.12.06). dDGN_Zero -> dDgn_LimF.
	// Change by ZINU.('05.05.23). Add if under dZeroF, dZeroM (N,mm).
	double dZeroLen = 0.0, dZeroFor = 0.0;
	if (!Get_ZeroFactor(dZeroLen, dZeroFor))	ASSERT(0);

	// Change by ZINU.('06.08.01). dDgn_LimF -> 0.01 at dZeroF, dDgn_LimF -> 1.0 at dZeroM.
	double dZeroF = 0.01 * dZeroFor;						// Zero Force.
	double dZeroM = 1.0  * (dZeroFor*dZeroLen);	// Zero Moment.

	// Forces.
	if (fabs(m_FxxD_UF[i]) < dDgn_LimF*rMaxUF.dFxxD || fabs(m_FxxD_UF[i]) < dZeroF)     m_FxxD_UF[i] = 0.0;
	if (fabs(m_FxxL_UF[i]) < dDgn_LimF*rMaxUF.dFxxL || fabs(m_FxxL_UF[i]) < dZeroF)     m_FxxL_UF[i] = 0.0;
	if (fabs(m_FxxR_UF[i]) < dDgn_LimF*rMaxUF.dFxxR || fabs(m_FxxR_UF[i]) < dZeroF)     m_FxxR_UF[i] = 0.0;
	if (fabs(m_FxxML_UF[i]) < dDgn_LimF*rMaxUF.dFxxML || fabs(m_FxxML_UF[i]) < dZeroF)     m_FxxML_UF[i] = 0.0;
	if (fabs(m_FxxTP_UF[i]) < dDgn_LimF*rMaxUF.dFxxTP || fabs(m_FxxTP_UF[i]) < dZeroF)     m_FxxTP_UF[i] = 0.0;
	if (fabs(m_FxxTS_UF[i]) < dDgn_LimF*rMaxUF.dFxxTS || fabs(m_FxxTS_UF[i]) < dZeroF)     m_FxxTS_UF[i] = 0.0;
	if (fabs(m_FxxLU_UF[i]) < dDgn_LimF*rMaxUF.dFxxLU || fabs(m_FxxLU_UF[i]) < dZeroF)     m_FxxLU_UF[i] = 0.0;

	if (fabs(m_FyyD_UF[i]) < dDgn_LimF*rMaxUF.dFyyD || fabs(m_FyyD_UF[i]) < dZeroF)     m_FyyD_UF[i] = 0.0;
	if (fabs(m_FyyL_UF[i]) < dDgn_LimF*rMaxUF.dFyyL || fabs(m_FyyL_UF[i]) < dZeroF)     m_FyyL_UF[i] = 0.0;
	if (fabs(m_FyyR_UF[i]) < dDgn_LimF*rMaxUF.dFyyR || fabs(m_FyyR_UF[i]) < dZeroF)     m_FyyR_UF[i] = 0.0;
	if (fabs(m_FyyML_UF[i]) < dDgn_LimF*rMaxUF.dFyyML || fabs(m_FyyML_UF[i]) < dZeroF)     m_FyyML_UF[i] = 0.0;
	if (fabs(m_FyyTP_UF[i]) < dDgn_LimF*rMaxUF.dFyyTP || fabs(m_FyyTP_UF[i]) < dZeroF)     m_FyyTP_UF[i] = 0.0;
	if (fabs(m_FyyTS_UF[i]) < dDgn_LimF*rMaxUF.dFyyTS || fabs(m_FyyTS_UF[i]) < dZeroF)     m_FyyTS_UF[i] = 0.0;
	if (fabs(m_FyyLU_UF[i]) < dDgn_LimF*rMaxUF.dFyyLU || fabs(m_FyyLU_UF[i]) < dZeroF)     m_FyyLU_UF[i] = 0.0;

	if (fabs(m_FxyD_UF[i]) < dDgn_LimF*rMaxUF.dFxyD || fabs(m_FxyD_UF[i]) < dZeroF)     m_FxyD_UF[i] = 0.0;
	if (fabs(m_FxyL_UF[i]) < dDgn_LimF*rMaxUF.dFxyL || fabs(m_FxyL_UF[i]) < dZeroF)     m_FxyL_UF[i] = 0.0;
	if (fabs(m_FxyR_UF[i]) < dDgn_LimF*rMaxUF.dFxyR || fabs(m_FxyR_UF[i]) < dZeroF)     m_FxyR_UF[i] = 0.0;
	if (fabs(m_FxyML_UF[i]) < dDgn_LimF*rMaxUF.dFxyML || fabs(m_FxyML_UF[i]) < dZeroF)     m_FxyML_UF[i] = 0.0;
	if (fabs(m_FxyTP_UF[i]) < dDgn_LimF*rMaxUF.dFxyTP || fabs(m_FxyTP_UF[i]) < dZeroF)     m_FxyTP_UF[i] = 0.0;
	if (fabs(m_FxyTS_UF[i]) < dDgn_LimF*rMaxUF.dFxyTS || fabs(m_FxyTS_UF[i]) < dZeroF)     m_FxyTS_UF[i] = 0.0;
	if (fabs(m_FxyLU_UF[i]) < dDgn_LimF*rMaxUF.dFxyLU || fabs(m_FxyLU_UF[i]) < dZeroF)     m_FxyLU_UF[i] = 0.0;

	if (fabs(m_VxxD_UF[i]) < dDgn_LimF*rMaxUF.dVxxD || fabs(m_VxxD_UF[i]) < dZeroF)     m_VxxD_UF[i] = 0.0;
	if (fabs(m_VxxL_UF[i]) < dDgn_LimF*rMaxUF.dVxxL || fabs(m_VxxL_UF[i]) < dZeroF)     m_VxxL_UF[i] = 0.0;
	if (fabs(m_VxxR_UF[i]) < dDgn_LimF*rMaxUF.dVxxR || fabs(m_VxxR_UF[i]) < dZeroF)     m_VxxR_UF[i] = 0.0;
	if (fabs(m_VxxML_UF[i]) < dDgn_LimF*rMaxUF.dVxxML || fabs(m_VxxML_UF[i]) < dZeroF)     m_VxxML_UF[i] = 0.0;
	if (fabs(m_VxxTP_UF[i]) < dDgn_LimF*rMaxUF.dVxxTP || fabs(m_VxxTP_UF[i]) < dZeroF)     m_VxxTP_UF[i] = 0.0;
	if (fabs(m_VxxTS_UF[i]) < dDgn_LimF*rMaxUF.dVxxTS || fabs(m_VxxTS_UF[i]) < dZeroF)     m_VxxTS_UF[i] = 0.0;
	if (fabs(m_VxxLU_UF[i]) < dDgn_LimF*rMaxUF.dVxxLU || fabs(m_VxxLU_UF[i]) < dZeroF)     m_VxxLU_UF[i] = 0.0;

	if (fabs(m_VyyD_UF[i]) < dDgn_LimF*rMaxUF.dVyyD || fabs(m_VyyD_UF[i]) < dZeroF)     m_VyyD_UF[i] = 0.0;
	if (fabs(m_VyyL_UF[i]) < dDgn_LimF*rMaxUF.dVyyL || fabs(m_VyyL_UF[i]) < dZeroF)     m_VyyL_UF[i] = 0.0;
	if (fabs(m_VyyR_UF[i]) < dDgn_LimF*rMaxUF.dVyyR || fabs(m_VyyR_UF[i]) < dZeroF)     m_VyyR_UF[i] = 0.0;
	if (fabs(m_VyyML_UF[i]) < dDgn_LimF*rMaxUF.dVyyML || fabs(m_VyyML_UF[i]) < dZeroF)     m_VyyML_UF[i] = 0.0;
	if (fabs(m_VyyTP_UF[i]) < dDgn_LimF*rMaxUF.dVyyTP || fabs(m_VyyTP_UF[i]) < dZeroF)     m_VyyTP_UF[i] = 0.0;
	if (fabs(m_VyyTS_UF[i]) < dDgn_LimF*rMaxUF.dVyyTS || fabs(m_VyyTS_UF[i]) < dZeroF)     m_VyyTS_UF[i] = 0.0;
	if (fabs(m_VyyLU_UF[i]) < dDgn_LimF*rMaxUF.dVyyLU || fabs(m_VyyLU_UF[i]) < dZeroF)     m_VyyLU_UF[i] = 0.0;
	// Change by ZINU.('04.06.11). Set Zero to MinMoment if My/Mz or Mz/My is under dDGN_Zero.
	// Change by ZINU.('04.12.06). dDGN_Zero -> dDgn_LimF.
	// Moments.
	if (fabs(m_MxxD_UF[i]) < dDgn_LimF*fabs(m_MyyD_UF[i]) || fabs(m_MxxD_UF[i]) < dZeroM)			m_MxxD_UF[i] = 0.0;
	if (fabs(m_MxxL_UF[i]) < dDgn_LimF*fabs(m_MyyL_UF[i]) || fabs(m_MxxL_UF[i]) < dZeroM)			m_MxxL_UF[i] = 0.0;
	if (fabs(m_MxxR_UF[i]) < dDgn_LimF*fabs(m_MyyR_UF[i]) || fabs(m_MxxR_UF[i]) < dZeroM)			m_MxxR_UF[i] = 0.0;
	if (fabs(m_MxxML_UF[i]) < dDgn_LimF*fabs(m_MyyML_UF[i]) || fabs(m_MxxML_UF[i]) < dZeroM)			m_MxxML_UF[i] = 0.0;
	if (fabs(m_MxxTP_UF[i]) < dDgn_LimF*fabs(m_MyyTP_UF[i]) || fabs(m_MxxTP_UF[i]) < dZeroM)			m_MxxTP_UF[i] = 0.0;
	if (fabs(m_MxxTS_UF[i]) < dDgn_LimF*fabs(m_MyyTS_UF[i]) || fabs(m_MxxTS_UF[i]) < dZeroM)			m_MxxTS_UF[i] = 0.0;
	if (fabs(m_MxxLU_UF[i]) < dDgn_LimF*fabs(m_MyyLU_UF[i]) || fabs(m_MxxLU_UF[i]) < dZeroM)			m_MxxLU_UF[i] = 0.0;

	if (fabs(m_MyyD_UF[i]) < dDgn_LimF*fabs(m_MxxD_UF[i]) || fabs(m_MyyD_UF[i]) < dZeroM)			m_MyyD_UF[i] = 0.0;
	if (fabs(m_MyyL_UF[i]) < dDgn_LimF*fabs(m_MxxL_UF[i]) || fabs(m_MyyL_UF[i]) < dZeroM)			m_MyyL_UF[i] = 0.0;
	if (fabs(m_MyyR_UF[i]) < dDgn_LimF*fabs(m_MxxR_UF[i]) || fabs(m_MyyR_UF[i]) < dZeroM)			m_MyyR_UF[i] = 0.0;
	if (fabs(m_MyyML_UF[i]) < dDgn_LimF*fabs(m_MxxML_UF[i]) || fabs(m_MyyML_UF[i]) < dZeroM)			m_MyyML_UF[i] = 0.0;
	if (fabs(m_MyyTP_UF[i]) < dDgn_LimF*fabs(m_MxxTP_UF[i]) || fabs(m_MyyTP_UF[i]) < dZeroM)			m_MyyTP_UF[i] = 0.0;
	if (fabs(m_MyyTS_UF[i]) < dDgn_LimF*fabs(m_MxxTS_UF[i]) || fabs(m_MyyTS_UF[i]) < dZeroM)			m_MyyTS_UF[i] = 0.0;
	if (fabs(m_MyyLU_UF[i]) < dDgn_LimF*fabs(m_MxxLU_UF[i]) || fabs(m_MyyLU_UF[i]) < dZeroM)			m_MyyLU_UF[i] = 0.0;

	if (fabs(m_MxyD_UF[i]) < dDgn_LimF*rMaxUF.dMxyD || fabs(m_MxyD_UF[i]) < dZeroM)			m_MxyD_UF[i] = 0.0;
	if (fabs(m_MxyL_UF[i]) < dDgn_LimF*rMaxUF.dMxyL || fabs(m_MxyL_UF[i]) < dZeroM)			m_MxyL_UF[i] = 0.0;
	if (fabs(m_MxyR_UF[i]) < dDgn_LimF*rMaxUF.dMxyR || fabs(m_MxyR_UF[i]) < dZeroM)			m_MxyR_UF[i] = 0.0;
	if (fabs(m_MxyML_UF[i]) < dDgn_LimF*rMaxUF.dMxyML || fabs(m_MxyML_UF[i]) < dZeroM)			m_MxyML_UF[i] = 0.0;
	if (fabs(m_MxyTP_UF[i]) < dDgn_LimF*rMaxUF.dMxyTP || fabs(m_MxyTP_UF[i]) < dZeroM)			m_MxyTP_UF[i] = 0.0;
	if (fabs(m_MxyTS_UF[i]) < dDgn_LimF*rMaxUF.dMxyTS || fabs(m_MxyTS_UF[i]) < dZeroM)			m_MxyTS_UF[i] = 0.0;
	if (fabs(m_MxyLU_UF[i]) < dDgn_LimF*rMaxUF.dMxyLU || fabs(m_MxyLU_UF[i]) < dZeroM)			m_MxyLU_UF[i] = 0.0;
}

void CDgnForceCtrl::ChangeAxialForceSignPlate(int i)
{
	// if Concrete, (+) is Compression. else Steel/SRC, (+) is Tension.
	if (m_strMatType != _T("C")) return;

	m_FxxD_UF[i] = (-1.0) * m_FxxD_UF[i];
	m_FxxL_UF[i] = (-1.0) * m_FxxL_UF[i];
	m_FxxR_UF[i] = (-1.0) * m_FxxR_UF[i];
	m_FxxML_UF[i] = (-1.0) * m_FxxML_UF[i];
	m_FxxTP_UF[i] = (-1.0) * m_FxxTP_UF[i];
	m_FxxTS_UF[i] = (-1.0) * m_FxxTS_UF[i];
	m_FxxLU_UF[i] = (-1.0) * m_FxxLU_UF[i];

	m_FyyD_UF[i] = (-1.0) * m_FyyD_UF[i];
	m_FyyL_UF[i] = (-1.0) * m_FyyL_UF[i];
	m_FyyR_UF[i] = (-1.0) * m_FyyR_UF[i];
	m_FyyML_UF[i] = (-1.0) * m_FyyML_UF[i];
	m_FyyTP_UF[i] = (-1.0) * m_FyyTP_UF[i];
	m_FyyTS_UF[i] = (-1.0) * m_FyyTS_UF[i];
	m_FyyLU_UF[i] = (-1.0) * m_FyyLU_UF[i];
}
// Add by ZINU.('02.12.27). To Reset Max/Min Moment by Moveing Live Load.
void CDgnForceCtrl::Reset_MaxMinByTotalLcom()
{
	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		// For PS-Beam Shear.
		m_dMmaxN[i] = 0.0; m_dViN[i] = 0.0;
		m_dMmaxP[i] = 0.0; m_dViP[i] = 0.0;
		// For Fatigue.
		m_dMmaxML[i] = 0.0;
		m_dMminML[i] = 0.0;
	}
}

// Add by ZINU.('02.12.27). To Save Max/Min Moment by Moveing Live Load.
void CDgnForceCtrl::Save_MaxMinByTotalLcom(int iElemNo, int iLcomNo, BOOL bRcsPsbm)
{
	_DGN_LCOM LcomDesign;
	LcomDesign.Initialize();
	Set_LcomDataForElem(iElemNo);
	if (!m_parLcomData->Lookup(iLcomNo, LcomDesign))	ASSERT(0);
	int iActive = LcomDesign.LcomUlData.nActive;
	for (int i = 0; i < CONST_DGN_iPOSNUM; i++)
	{
		// For PS-Beam Shear.
		if ((iActive == 1 || iActive == 3 || iActive == 4) && bRcsPsbm)	// Strength.
		{
			if (min(m_BMy[i] - m_dMdf[i], 0.0) <= m_dMmaxN[i]) { m_dMmaxN[i] = m_BMy[i] - m_dMdf[i]; m_dViN[i] = m_Fzz[i] - m_dVdf[i]; }
			if (max(m_BMy[i] - m_dMdf[i], 0.0) >= m_dMmaxP[i]) { m_dMmaxP[i] = m_BMy[i] - m_dMdf[i]; m_dViP[i] = m_Fzz[i] - m_dVdf[i]; }
		}
		// For Fatigue.
		if (iActive == 2)	// Serviceability.
		{
			if (m_BMyML[i] >= m_dMmaxML[i])	m_dMmaxML[i] = m_BMyML[i];
			if (m_BMyML[i] <= m_dMminML[i])	m_dMminML[i] = m_BMyML[i];
		}
	}
}

// Add by ZINU.('03.11.07). To Search Max/Min Moment Position between 1/4 and 3/4.
void CDgnForceCtrl::SetMaxMinMomentPosiForBeam(int iElem, BOOL bMaxLcom, int nLcomKind)
{
	//*/PROFILING(_T("SetMaxMinMomentPosiForBeam"));
	// Add by ZINU.('05.07.28). Initialize Data per Lcom.
	m_pDgnDataCtrl->m_pMembCtrl->InitializeMaxMinMomentForBeam();

	// Change by ZINU.('05.06.14). ElemType -> MatlType, ElemType, MembType. 
	// Change by ZINU.('06.06.16). Con, SRC -> All Matl, Agree with 이대근GJN, 김동숙DRN.
	//if(!(m_strMatType==_T("C") || m_strMatType==_T("SRC")))	return;	// Only Con, SRC.
	if (!m_pDoc->m_pAttrCtrl->IsBeam(m_iElemType))	return;		// Only Beam Elem.
	if (m_iElemKind != D_MBTP_BEAM)	return;										// Only Beam (Member Type).
	// add by Seungjun (`06.05.22)
	if (m_bPSBeam)                 return;


	CArray<int, int> aMidList[2];	// 0=Element No, 1=Position.
	// Change by ZINU.('04.04.10). Set Initialized Data at SetMaxMinMomentForBeam().
	int iElemPosNeg[2] = { 0,0 }, iPosiPosNeg[2] = { 0,0 };
	double dMomyPosNeg[2] = { 0.,0. };
	if (m_pDgnDataCtrl->m_pMembCtrl->GetElemPosiAtMembMid(iElem, aMidList))
	{
		// Get Force Data by Lcom.
		int iLcaseType = 0;
		// [GEN-1272] 하중조합 type에 따라 iLcaseType을 match함.
		switch (nLcomKind)
		{
		case D_LCOMTYPE_STEEL:       iLcaseType = D_LOADCASE_COMB_STEEL;       break;
		case D_LCOMTYPE_CONCRETE:    iLcaseType = D_LOADCASE_COMB_CONCRETE;    break;
		case D_LCOMTYPE_FDN:         iLcaseType = D_LOADCASE_COMB_FDN;         break;
		case D_LCOMTYPE_SRC:         iLcaseType = D_LOADCASE_COMB_SRC;         break;
		case D_LCOMTYPE_ALUMINUM:    iLcaseType = D_LOADCASE_COMB_ALUMINUM;    break;
		case D_LCOMTYPE_STLCOMP:     iLcaseType = D_LOADCASE_COMB_STLCOMP;     break;
		case D_LCOMTYPE_CFSTEEL:     iLcaseType = D_LOADCASE_COMB_CFSTEEL;     break;
		case D_LCOMTYPE_SEISMIC:     iLcaseType = D_LOADCASE_COMB_SEISMIC;     break;
		case D_LCOMTYPE_LINEAR_EVAL: iLcaseType = D_LOADCASE_COMB_LINEAR_EVAL; break;
		default: ASSERT(0); break;
		}

		double dMaxMom = 0.0, dMinMom = 0.0;
		// Change by ZINU.('04.01.17). Change Looping Condition.
		T_STRB_D StrMaxData, StrMinData;
		int iMidListNum = aMidList[0].GetSize();
		for (int i = 0; i < iMidListNum; i++)
		{
			T_ELEM_K iElemPrev = (i == 0 ? 0 : aMidList[0].GetAt(i - 1));
			T_ELEM_K iElemCurr = aMidList[0].GetAt(i);	// ElemNo.
			int iPosiCurr = aMidList[1].GetAt(i);	// PosiNo.
			if (iElemPrev != iElemCurr)
			{
				// Get Force.
				StrMaxData.Initialize();
				StrMinData.Initialize();
				m_pDgnDataCtrl->m_pMembCtrl->GetStrbElem(iElemCurr, iLcaseType, m_iOrgLcomKey, &StrMaxData, &StrMinData);
			}
			int iPart = (iPosiCurr < CONST_DGN_iPOSNUM - 1 ? iPosiCurr : iPosiCurr - 1);
			int iKind = (iPosiCurr < CONST_DGN_iPOSNUM - 1 ? 0 : 6);
			// Change by ZINU.('05.02.24). Separate MyP from MyN.	
			double dMy = (bMaxLcom ? StrMaxData.dblForce[iPart][iKind + 4] : StrMinData.dblForce[iPart][iKind + 4]);
			if (dMy > dMomyPosNeg[0]) { dMomyPosNeg[0] = dMy; iElemPosNeg[0] = iElemCurr; iPosiPosNeg[0] = iPosiCurr; }	// Positive.
			if (dMy < dMomyPosNeg[1]) { dMomyPosNeg[1] = dMy; iElemPosNeg[1] = iElemCurr; iPosiPosNeg[1] = iPosiCurr; }	// Negative.
		}
	}
	// Set Max/Min ElemNo, PosiNo at Beam Mid(1/4~3/4).
	// Change by ZINU.('05.02.24). Set m_BMyMidUp, m_BMyMidDn.
	m_BMyMidUp = dMomyPosNeg[0];
	m_BMyMidDn = dMomyPosNeg[1];
	m_pDgnDataCtrl->m_pMembCtrl->SetMaxMinMomentForBeam(iElemPosNeg, iPosiPosNeg);
}

// Add by ZINU.('03.11.11). Get M1,M2 for Cb,Cmy,Cmz.
void CDgnForceCtrl::SetElemListForBeamEndMoment(int iElem)
{
	//*/PROFILING(_T("SetElemListForBeamEndMoment"));
	for (int i = 0; i < 2; i++)
	{
		m_apElemK_y[i].RemoveAll();
		m_apElemK_z[i].RemoveAll();
	}
	T_MEMB_K MembKey;
	if (m_pDoc->m_pAttrCtrl->GetHcbmAssigned(iElem, MembKey))	// Exist Haunched Beam.
	{
		if (!m_pDgnDataCtrl->m_pMembCtrl->GetUnbracedMoment(iElem, m_apElemK_y, m_apElemK_z))	ASSERT(0);
	}
	else if (m_pDoc->m_pAttrCtrl->GetMembAssigned(iElem, MembKey))	// Exist Member.
	{
		if (!m_pDgnDataCtrl->m_pMembCtrl->GetUnbracedMoment(iElem, m_apElemK_y, m_apElemK_z))	ASSERT(0);
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			m_apElemK_y[i].Add(iElem);
			m_apElemK_z[i].Add(iElem);
		}
	}
}

void CDgnForceCtrl::Write_GDisplayAnalString(CString strTxt, BOOL bClearPre/*TRUE*/, BOOL bSpacePost/*TRUE*/)
{
	if ( CDBLib::IsIncludeMessageSkipRange() ) return;
	GActiveAnalysisViewer();
	if (bClearPre == TRUE)		GClearAnalStrings();
	GDisplayAnalString(strTxt);
	if (bSpacePost == TRUE)	GDisplayAnalString(_T(""));
}

CString CDgnForceCtrl::Get_PosiNa(int iPosiNo)
{
	switch (iPosiNo)
	{
	case 0: return _T("I");
	case 1: return _T("1/4");
	case 2: return _T("1/2");
	case 3: return _T("3/4");
	case 4: return _T("J");
	default: { ASSERT(0); return _T(""); }
	}
}

int CDgnForceCtrl::Get_PosiNo(CString strPosiNa)
{
	if (strPosiNa == _T("I")) { return 0; }
	if (strPosiNa == _T("1/4")) { return 1; }
	if (strPosiNa == _T("1/2")) { return 2; }
	if (strPosiNa == _T("3/4")) { return 3; }
	if (strPosiNa == _T("J")) { return 4; }

	return 0;
}

CString CDgnForceCtrl::Get_LcomNa(int iLcomNo, int iElemNo/*=0*/)
{
	if (iElemNo == 0)	iElemNo = m_iElemNo;

	_DGN_LCOM LcomDesign;
	LcomDesign.Initialize();
	Set_LcomDataForElem(iElemNo);
	m_parLcomData->Lookup(iLcomNo, LcomDesign);
	return LcomDesign.DesignLcomNa;
}

int CDgnForceCtrl::Get_LcomNo(CString strLcomNa, int iElemNo/*=0*/)
{
	if (iElemNo == 0)	iElemNo = m_iElemNo;
	int iLcomNo = 0;
	_DGN_LCOM LcomDesign;
	T_LCOM_K iIndexLcom = 0;
	Set_LcomDataForElem(iElemNo);
	POSITION PosLcom = m_parLcomData->GetStartPosition();
	while (PosLcom)
	{
		LcomDesign.Initialize();
		m_parLcomData->GetNextAssoc(PosLcom, iIndexLcom, LcomDesign);
		if (strLcomNa == LcomDesign.DesignLcomNa)
		{
			iLcomNo = LcomDesign.DesignLcomKey;
			break;
		}
	}
	return iLcomNo;
}

int CDgnForceCtrl::GetOkEqACI()
{
	// Modify by GAY. PMS:4076. ('10.01.26). Add Design Code <KCI-USD07>
	// Modify by GAY. PMS:4450. ('12.03.13). Add Design Code <ACI318-08>, <ACI318-11>
	BOOL bIsCodeOK = FALSE;
	switch (m_iDgnCode)
	{
	case ACI318_89:
	case ACI318_95:
	case ACI318_99:
	case ACI318_02:
	case ACI318_05:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case NSR_10: // by GAY. PMS:5009. ('15.03.31). add Colombia design code.
	case NSCP_2015_C:
	case KCI_USD07:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case IS456_2000:
	case NTC_DCEC_2017:
	case NTC_DCEC_2023:
		bIsCodeOK = TRUE;
		break;
	default:
		break;
	}

	BOOL bIsEqOK = (m_bEqSpecial ? TRUE : FALSE);
	// Include TWN-USD92, TWN-USD100
	// by GAY. ('14.11.27). include TWN-SRC92, TWN-SRC100 in case of SRC Beam.
	if ((m_iDgnCode == TWN_USD92 || m_iDgnCode == TWN_USD100 || m_iDgnCode == TWN_USD112 || m_iDgnCode == TWN_SRC92 || m_iDgnCode == TWN_SRC100) && m_bEqSpecial)
	{
		return 1;	// Same Special.
	}
	else
	{
		return (bIsCodeOK && bIsEqOK ? m_iDgnClass : 0);
	}
}
BOOL CDgnForceCtrl::IsChinaCode_StlEQNEW()
{
	if (m_iDgnCode == GBJ17_88 || m_iDgnCode == GB50017_03 || m_iDgnCode == GB50017_15 || m_iDgnCode == GB50018_02 || m_iDgnCode == GB50018_25)
		return TRUE;
	return FALSE;
}
BOOL CDgnForceCtrl::IsChinaCode()
{
	BOOL bIsChinaOK = FALSE;
	// Change by ParkBong.('06.11.20). Remove Material Checking Option. //add by maxiao(2015-9-25)GB50017-15
	BOOL bIsStlOK = (/*m_strMatType==_T("S")   && */(m_iDgnCode == GBJ17_88 || m_iDgnCode == GB50017_03 || m_iDgnCode == GB50017_15 || m_iDgnCode == GB50018_02 || m_iDgnCode == GB50018_25 || m_iDgnCode == GB50017_17 || m_iDgnCode == GB51249_2017 || m_iDgnCode == JGJ209_2010) ? TRUE : FALSE);
	BOOL bIsConOK = (/*m_strMatType==_T("C")   && */(m_iDgnCode == GB50010_02 || m_iDgnCode == GB50010_10 || m_iDgnCode == GB50010_19) ? TRUE : FALSE);
	BOOL bIsSrcOK = (/*m_strMatType==_T("SRC") && */m_iDgnCode == JGJ138_01 ? TRUE : FALSE);
	if (bIsStlOK || bIsConOK || bIsSrcOK)	bIsChinaOK = TRUE;

	return bIsChinaOK;
}

int CDgnForceCtrl::GetOkEqGB()
{
	if (IsChinaCode() && m_bEqSpecial)
	{
		if (IsChinaCode_StlEQNEW() && m_iDgnClass == 5)
			return 0;
		return m_iDgnClass;
	}
	return  0;
}
int CDgnForceCtrl::GetNoEqGB()
{
	if (IsChinaCode() && !m_bEqSpecial)
	{
		if (m_bDampCheck)
			return 0;
		else if (IsChinaCode_StlEQNEW() && m_iDgnClass == 5)
			return 0;
		return m_iDgnClass;
	}
	return  0;
}

BOOL CDgnForceCtrl::IsFrmSupBeamGB()
{
	BOOL bOkElemKind = (m_iElemKind == D_MBTP_BEAM ? TRUE : FALSE);
	// Beam Supporting Frame(2).
	BOOL bOkSubKind = (m_iElemSubKind == 2 ? TRUE : FALSE);
	if (bOkElemKind && bOkSubKind)	return TRUE;
	else													return FALSE;
}

BOOL CDgnForceCtrl::IsCornerColmGB()
{
	BOOL bOkElemKind = (m_iElemKind == D_MBTP_COLUMN ? TRUE : FALSE);
	// Corner Column(2), Lower Corner Column(3).
	BOOL bOkSubKind = (m_iElemSubKind == 2 || m_iElemSubKind == 3 ? TRUE : FALSE);
	if (bOkElemKind && bOkSubKind)	return TRUE;
	else													return FALSE;
}

BOOL CDgnForceCtrl::IsFrmSupColmGB()
{
	BOOL bOkElemKind = (m_iElemKind == D_MBTP_COLUMN ? TRUE : FALSE);
	// Column Supporting Frame(4).
	BOOL bOkSubKind = (m_iElemSubKind == 4 ? TRUE : FALSE);
	if (bOkElemKind && bOkSubKind)	return TRUE;
	else													return FALSE;
}

BOOL CDgnForceCtrl::IsStrRgnWallGB()
{
	BOOL bOkElemKind = m_pDoc->m_pAttrCtrl->IsWall(m_iElemType);
	// 0=General Shear Wall (Strengthened part), 2=Shear Wall at Complicated Tall Building (Strengthened part).
	BOOL bOkSubKind = (m_iElemSubKind == 0 || m_iElemSubKind == 2 || m_iElemSubKind == 4 ? TRUE : FALSE);
	if (bOkElemKind && bOkSubKind)	return TRUE;
	else													return FALSE;
}

BOOL CDgnForceCtrl::IsCoTallWallGB()
{
	BOOL bOkElemKind = m_pDoc->m_pAttrCtrl->IsWall(m_iElemType);
	// Shear Wall at Complicated Tall Building (2=Strengthened part, 3=Ordinary part).
	BOOL bOkSubKind = (m_iElemSubKind == 2 || m_iElemSubKind == 3 ? TRUE : FALSE);
	if (bOkElemKind && bOkSubKind)	return TRUE;
	else													return FALSE;
}

BOOL CDgnForceCtrl::IsShortWallGB()
{
	BOOL bOkElemKind = m_pDoc->m_pAttrCtrl->IsWall(m_iElemType);
	// Short Shear Wall (4=Strengthened part, 5=Ordinary part).
	BOOL bOkSubKind = (m_iElemSubKind == 4 || m_iElemSubKind == 5 ? TRUE : FALSE);
	if (bOkElemKind && bOkSubKind) return TRUE;
	else                          return FALSE;
}

int CDgnForceCtrl::Cng_ElemSubKindColmGB(int iElem, int iPosi)
{
	// Modify by GAY. PMS:4267. ('11.12.12). for GB50010_02 -> for China Code.
	BOOL bChinaCode = (m_iDgnCode == GB50010_02 || m_iDgnCode == GB50010_10 || m_iDgnCode == GB50010_19) ? TRUE : FALSE;
	int iSubKind = m_iElemSubKind;
	if (!bChinaCode)												            return iSubKind;
	if (!(m_iElemKind == D_MBTP_COLUMN && GetOkEqGB()))	return iSubKind;
	if (iElem <= 0) { ASSERT(0);	return iSubKind; }
	if (iPosi < 0) { ASSERT(0);	return iSubKind; }

	T_ELEM_D ElemD;
	ElemD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(iElem, ElemD))	ASSERT(0);

	T_NODE_D NodeD1, NodeD2;
	// 1st Node.
	NodeD1.Initialize();
	m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1);
	// 2nd Node.
	NodeD2.Initialize();
	m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2);

	BOOL bCngSubKind = FALSE;
	//Change by Park.Bong('08.01.24)
	//Lower Frame Column && Control force in top section    => 1.4
	//Lower Frame Column && Control force in bottom section => 1.5
	if (NodeD1.z > NodeD2.z && iPosi == 0)	                        bCngSubKind = TRUE;
	else if (NodeD2.z > NodeD1.z && iPosi == CONST_DGN_iPOSNUM - 1)	bCngSubKind = TRUE;
	if (bCngSubKind && (iSubKind == 1 || iSubKind == 3))
	{
		// If bCngSubKin=TRUE and,
		// a. Lower Frame Column(1) -> Frame Column(0).
		// b. Lower Corner Column(3) -> Corner Column(2).
		if (iSubKind == 1)				iSubKind = 0;
		else if (iSubKind == 3)	iSubKind = 2;
	}
	return iSubKind;
}

// Add by ZINU.('03.01.11).
BOOL CDgnForceCtrl::Get_EqFactorForLcase(BOOL bLcaseEQ)
{
	double dSueqLcasN = 1.0;
	double dSueqLcasM = 1.0;
	double dSueqLcasV = 1.0;
	double dCodeLcasN = 1.0;
	double dCodeLcasM = 1.0;
	double dCodeLcasV = 1.0;
	if (bLcaseEQ)	// Only EQ Lcase.
	{
		if (IsChinaCode())
		{
			int nSeisGrade = GetOkEqGB();
			if (m_iDgnCode == GB50010_10 || m_iDgnCode == GB50010_19)
			{
				if (m_iElemKind == D_MBTP_BEAM && m_iElemSubKind == 2) // Beam Supporting Frame(2).
				{
					Get_SeisLcasForceFactor_BeamSupportFrame_GB50010_10(nSeisGrade, dCodeLcasN, dCodeLcasM, dCodeLcasV);
				}

				BOOL bFrameStruct = IsFrameStructure_GB10();
				if (m_iElemKind == D_MBTP_COLUMN && m_iElemSubKind == 4 && !bFrameStruct) // Column Supporting Frame(4).
				{
					Get_SeisLcasForceFactor_ColmSupportFrame_GB50010_10(nSeisGrade, dCodeLcasN);
				}
			}
		}
		else
		{
			int iMemType = m_iElemKind;
			// Change by GAY.('06.01.18). Consider in SRC Section, too.
			if ((m_strMatType == _T("C") || m_strMatType == _T("SRC")) && m_bEqSpecial && (iMemType == D_MBTP_COLUMN || iMemType == D_MBTP_BEAM))
			{
				// Change by ZINU.('04.02.04). See Get_DgnConSufsDef() at CDgnDataCtrl.
				dCodeLcasN = 1.0;
				dCodeLcasM = 1.0;
				dCodeLcasV = Get_Alpha2();// m_dAlpha2;
			}
		}
		// Modify by GAY. MQC:1860. ('08.05.23). Set User Input Scale up Factors for Earthquake.
		dSueqLcasN = max(1.0, m_dUserSueqLcasN);
		dSueqLcasM = max(1.0, m_dUserSueqLcasM);
		dSueqLcasV = max(1.0, m_dUserSueqLcasV);
	}
	// Save Data.
	m_dSueqLcasN = dSueqLcasN;
	m_dSueqLcasM = dSueqLcasM;
	m_dSueqLcasV = dSueqLcasV;
	if (m_dSueqLcasN*m_dSueqLcasM*m_dSueqLcasV == 0.0)	return FALSE;
	m_dCodeLcasN = dCodeLcasN;
	m_dCodeLcasM = dCodeLcasM;
	m_dCodeLcasV = dCodeLcasV;
	if (m_dCodeLcasN*m_dCodeLcasM*m_dCodeLcasV == 0.0)	return FALSE;

	return TRUE;
}

double CDgnForceCtrl::Get_Alpha2() // for Ve2
{
	double dAlpha2 = m_dAlpha2;
	if(m_iDgnCode == ACI318_14 || m_iDgnCode == ACI318M_14 ||
		m_iDgnCode == ACI318_19 || m_iDgnCode == ACI318M_19 ||
		m_iDgnCode == ACI318_25 || m_iDgnCode == ACI318M_25 ||
		m_iDgnCode == NSR_10 || m_iDgnCode == NSCP_2015_C || m_iDgnCode == IS456_2000 ||
		m_iDgnCode == NTC_DCEC_2017 || m_iDgnCode == NTC_DCEC_2023)
	{
		if (m_iElemKind == D_MBTP_COLUMN)  dAlpha2 = m_dAlpha2_Colm;
	}
	return dAlpha2;
}

// Add by ZINU.('02.12.30). Agree with Kye CJ.
BOOL CDgnForceCtrl::Get_EqFactorForLcom(int iElem, int iPosi, int iSeismicType)
{
	// Add by ZINU.('04.01.05). Change SubKind of Column Member.
	m_iElemSubKindAuto = Cng_ElemSubKindColmGB(iElem, iPosi);	// Only GB50010_02.

	double dSueqLcomN = 1.0;
	double dSueqLcomM = 1.0;
	double dSueqLcomV = 1.0;
	double dCodeLcomN = 1.0;
	double dCodeLcomM = 1.0;
	double dCodeLcomV = 1.0;
	if (iSeismicType > 0)	// Only EQ Lcom.
	{
		if (IsChinaCode() && !m_bDampCheck)
		{
			int iMemType = m_iElemKind;
			int iSubType = m_iElemSubKindAuto;
			int iOkEqGB = GetOkEqGB();
			if (m_iDgnCode == GBJ17_88 || m_iDgnCode == GB50017_03 || m_iDgnCode == GB50017_15 || m_iDgnCode == GB50017_17 || m_iDgnCode == JGJ209_2010)	// Steel.//add by maxiao(2015-9-25)GB50017-15
			{
				// Change by ZINU.('04.01.08). by Kye BJ.
				/*
				if(iOkEqGB==1)			{dCodeLcomN = 1.0;  dCodeLcomM = 1.2;  dCodeLcomV = 1.2;}	// Intensity 9.
				else if(iOkEqGB==2)	{dCodeLcomN = 1.0;  dCodeLcomM = 1.1;  dCodeLcomV = 1.1;}	// Intensity 8.
				else if(iOkEqGB==3)	{dCodeLcomN = 1.0;  dCodeLcomM = 1.0;  dCodeLcomV = 1.0;}	// Intensity 7.
				else if(iOkEqGB==4)	{dCodeLcomN = 1.0;  dCodeLcomM = 1.0;  dCodeLcomV = 1.0;}	// Intensity 6.
				*/
				if (iOkEqGB == 1) { dCodeLcomN = 1.0;  dCodeLcomM = 1.0;  dCodeLcomV = 1.0; }	// Intensity 9.
				else if (iOkEqGB == 2) { dCodeLcomN = 1.0;  dCodeLcomM = 1.0;  dCodeLcomV = 1.0; }	// Intensity 8.
				else if (iOkEqGB == 3) { dCodeLcomN = 1.0;  dCodeLcomM = 1.0;  dCodeLcomV = 1.0; }	// Intensity 7.
				else if (iOkEqGB == 4) { dCodeLcomN = 1.0;  dCodeLcomM = 1.0;  dCodeLcomV = 1.0; }	// Intensity 6.
			}
			else if (m_iDgnCode == GB50010_02)	// RC.
			{
				if (iMemType == D_MBTP_BEAM)	// Beam.
				{
					if (iSubType == 0 || iSubType == 2)	// Frame Beam, Beam Supporting Frame.
					{
						if (iOkEqGB == 1) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.56; }	// 특1.
						else if (iOkEqGB == 2) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.33; }	// 9도 및 1급 Frame구조.
						else if (iOkEqGB == 3) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3; }	// 1.
						else if (iOkEqGB == 4) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.2; }	// 2.
						else if (iOkEqGB == 5) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.1; }	// 3.
						else if (iOkEqGB == 6) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; }	// 4.
					}
					else if (iSubType == 1)	// Lintel Beam.
					{
						if (iOkEqGB == 1) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3; }	// 특1.
						else if (iOkEqGB == 2) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3; }	// 9도 및 1급 Frame구조.
						else if (iOkEqGB == 3) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3; }	// 1.
						else if (iOkEqGB == 4) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.2; }	// 2.
						else if (iOkEqGB == 5) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.1; }	// 3.
						else if (iOkEqGB == 6) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; }	// 4.
					}
					else	ASSERT(0);
				}
				else if (iMemType == D_MBTP_COLUMN)	// Column.
				{
					if (iSubType == 0 || iSubType == 2)	// Frame Column(0), Corner Column(2).
					{
						if (iOkEqGB == 1) { dCodeLcomN = 1.0;   dCodeLcomM = 1.68;  dCodeLcomV = 2.352; }	// 특1.
						else if (iOkEqGB == 2) { dCodeLcomN = 1.0;   dCodeLcomM = 1.452; dCodeLcomV = 2.108; }	// 9도 및 1급 Frame구조.
						else if (iOkEqGB == 3) { dCodeLcomN = 1.0;   dCodeLcomM = 1.4;   dCodeLcomV = 1.96; }	// 1.
						else if (iOkEqGB == 4) { dCodeLcomN = 1.0;   dCodeLcomM = 1.2;   dCodeLcomV = 1.44; }	// 2.
						else if (iOkEqGB == 5) { dCodeLcomN = 1.0;   dCodeLcomM = 1.1;   dCodeLcomV = 1.21; }	// 3.
						else if (iOkEqGB == 6) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; }	// 4.
						dCodeLcomM *= (iSubType == 2 && iOkEqGB <= 5 ? 1.1 : 1.0);
						dCodeLcomV *= (iSubType == 2 && iOkEqGB <= 5 ? 1.1 : 1.0);
					}
					else if (iSubType == 1 || iSubType == 3)	// Lower Frame Column(1), Lower Corner Column(3).
					{
						if (iOkEqGB == 1) { dCodeLcomN = 1.0;   dCodeLcomM = 1.8;   dCodeLcomV = 2.52; }	// 특1.
						else if (iOkEqGB == 2) { dCodeLcomN = 1.0;   dCodeLcomM = 1.597; dCodeLcomV = 2.319; }	// 9도 및 1급 Frame구조.
						else if (iOkEqGB == 3) { dCodeLcomN = 1.0;   dCodeLcomM = 1.5;   dCodeLcomV = 2.1; }	// 1.
						else if (iOkEqGB == 4) { dCodeLcomN = 1.0;   dCodeLcomM = 1.25;  dCodeLcomV = 1.5; }	// 2.
						else if (iOkEqGB == 5) { dCodeLcomN = 1.0;   dCodeLcomM = 1.15;  dCodeLcomV = 1.265; }	// 3.
						else if (iOkEqGB == 6) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; }	// 4.
						dCodeLcomM *= (iSubType == 3 && iOkEqGB <= 5 ? 1.1 : 1.0);
						dCodeLcomV *= (iSubType == 3 && iOkEqGB <= 5 ? 1.1 : 1.0);
					}
					else if (iSubType == 4)	// Column Supporting Frame(4).
					{
						if (iOkEqGB == 1) { dCodeLcomN = 1.8;   dCodeLcomM = 1.8;   dCodeLcomV = 2.1; }	// 특1.
						else if (iOkEqGB == 2) { dCodeLcomN = 1.5;   dCodeLcomM = 1.5;   dCodeLcomV = 2.1; }	// 9도 및 1급 Frame구조.
						else if (iOkEqGB == 3) { dCodeLcomN = 1.5;   dCodeLcomM = 1.5;   dCodeLcomV = 2.1; }	// 1.
						else if (iOkEqGB == 4) { dCodeLcomN = 1.2;   dCodeLcomM = 1.25;  dCodeLcomV = 1.5; }	// 2.
						else if (iOkEqGB == 5) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; }	// 3.
						else if (iOkEqGB == 6) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; }	// 4.
					}
					else	ASSERT(0);
				}
				else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))	// Wall.
				{
					if (iSubType == 0)	// General Shear Wall (Strengthened part).
					{
						if (iOkEqGB == 1) { dCodeLcomN = 1.0;   dCodeLcomM = 1.1;   dCodeLcomV = 1.9; }	// 특1.
						else if (iOkEqGB == 2) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.7; }	// 9도 및 1급 Frame구조.
						else if (iOkEqGB == 3) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.6; }	// 1.
						else if (iOkEqGB == 4) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.4; }	// 2.
						else if (iOkEqGB == 5) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.2; }	// 3.
						else if (iOkEqGB == 6) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; }	// 4.
					}
					else if (iSubType == 1 || iSubType == 3)	// 1=General Shear Wall (Ordinary part), 3=Shear Wall at Complicated Tall Building (Ordinary part).
					{
						if (iOkEqGB == 1) { dCodeLcomN = 1.0;   dCodeLcomM = 1.3;   dCodeLcomV = 1.2; }	// 특1.
						else if (iOkEqGB == 2) { dCodeLcomN = 1.0;   dCodeLcomM = 1.2;   dCodeLcomV = 1.0; }	// 9도 및 1급 Frame구조.
						else if (iOkEqGB == 3) { dCodeLcomN = 1.0;   dCodeLcomM = 1.2;   dCodeLcomV = 1.0; }	// 1.
						else if (iOkEqGB == 4) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; }	// 2.
						else if (iOkEqGB == 5) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; }	// 3.
						else if (iOkEqGB == 6) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; }	// 4.
					}
					else if (iSubType == 2)	// Shear Wall at Complicated Tall Building (Strengthened part).
					{
						if (iOkEqGB == 1) { dCodeLcomN = 1.0;   dCodeLcomM = 1.8;   dCodeLcomV = 1.9; }	// 특1.
						else if (iOkEqGB == 2) { dCodeLcomN = 1.0;   dCodeLcomM = 1.5;   dCodeLcomV = 1.7; }	// 9도 및 1급 Frame구조.
						else if (iOkEqGB == 3) { dCodeLcomN = 1.0;   dCodeLcomM = 1.5;   dCodeLcomV = 1.6; }	// 1.
						else if (iOkEqGB == 4) { dCodeLcomN = 1.0;   dCodeLcomM = 1.25;  dCodeLcomV = 1.4; }	// 2.
						else if (iOkEqGB == 5) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.2; }	// 3.
						else if (iOkEqGB == 6) { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; }	// 4.
					}
					else	ASSERT(0);
				}
			}
			else if (m_iDgnCode == GB50010_10 || m_iDgnCode == GB50010_19) // Add by GAY. PMS:4267. ('11.12.12).
			{
				BOOL bFrameStruct = IsFrameStructure_GB10();

				if (iMemType == D_MBTP_BEAM && IsApplyEqSpecialForce(m_iElemNo)) // Modify by GAY. PMS:4267. ('12.02.23). Subbeam은 내진부재력 조정계수 고려하지 않음. (with 후효무)
				{
					Get_SeisLcomForceFactor_Beam_GB50010_10(iSubType, iOkEqGB, dCodeLcomN, dCodeLcomM, dCodeLcomV);
				}
				else if (iMemType == D_MBTP_COLUMN)
				{
					Get_SeisLcomForceFactor_Colm_GB50010_10(iSubType, iOkEqGB, bFrameStruct, dCodeLcomN, dCodeLcomM, dCodeLcomV);
				}
				else if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))
				{
					Get_SeisLcomForceFactor_Wall_GB50010_10(iSubType, iOkEqGB, dCodeLcomN, dCodeLcomM, dCodeLcomV);
				}
			}
			else if (m_iDgnCode == JGJ138_01)	// SRC.
			{
				// Change by ZINU.('04.01.08). by Kye BJ.
				/*
				if(iOkEqGB==1)			{dCodeLcomN = 1.0;  dCodeLcomM = 1.2;  dCodeLcomV = 1.2;}	// Intensity 9.
				else if(iOkEqGB==2)	{dCodeLcomN = 1.0;  dCodeLcomM = 1.1;  dCodeLcomV = 1.1;}	// Intensity 8.
				else if(iOkEqGB==3)	{dCodeLcomN = 1.0;  dCodeLcomM = 1.0;  dCodeLcomV = 1.0;}	// Intensity 7.
				else if(iOkEqGB==4)	{dCodeLcomN = 1.0;  dCodeLcomM = 1.0;  dCodeLcomV = 1.0;}	// Intensity 6.
				*/
				if (iOkEqGB == 1) { dCodeLcomN = 1.0;  dCodeLcomM = 1.0;  dCodeLcomV = 1.0; }	// Intensity 9.
				else if (iOkEqGB == 2) { dCodeLcomN = 1.0;  dCodeLcomM = 1.0;  dCodeLcomV = 1.0; }	// Intensity 8.
				else if (iOkEqGB == 3) { dCodeLcomN = 1.0;  dCodeLcomM = 1.0;  dCodeLcomV = 1.0; }	// Intensity 7.
				else if (iOkEqGB == 4) { dCodeLcomN = 1.0;  dCodeLcomM = 1.0;  dCodeLcomV = 1.0; }	// Intensity 6.
			}
			else	ASSERT(0);
		}
		else
		{
			// Not used.
		}
		// Modify by GAY. MQC:1860. ('08.07.18). Set User Input Scale up Factors for Earthquake.
		dSueqLcomN = max(1.0, m_dUserSueqLcomN);
		dSueqLcomM = max(1.0, m_dUserSueqLcomM);
		dSueqLcomV = max(1.0, m_dUserSueqLcomV);
	}
	// Save Data.
	m_dSueqLcomN = dSueqLcomN;
	m_dSueqLcomM = dSueqLcomM;
	m_dSueqLcomV = dSueqLcomV;
	if (m_dSueqLcomN*m_dSueqLcomM*m_dSueqLcomV == 0.0)	return FALSE;
	m_dCodeLcomN = dCodeLcomN;
	m_dCodeLcomM = dCodeLcomM;
	m_dCodeLcomV = dCodeLcomV;
	if (m_dCodeLcomN*m_dCodeLcomM*m_dCodeLcomV == 0.0)	return FALSE;

	return TRUE;
}

// Add by ZINU.('03.01.16). For EQ Type(0=No, 1=Lateral, 2=Vertical).
int CDgnForceCtrl::GetLcomTypeEq(int iLcomNo)
{
	// 1=XY or XY+Z, 2=Z.
	_DGN_LCOM LcomDesign;
	LcomDesign.Initialize();
	Set_LcomDataForElem(m_iElemNo);
	m_parLcomData->Lookup(iLcomNo, LcomDesign);

	return LcomDesign.iSeismicType;
}

BOOL CDgnForceCtrl::IsEqSpecialLcom(int iLcomNo)
{
	_DGN_LCOM LcomDesign;
	LcomDesign.Initialize();
	Set_LcomDataForElem(m_iElemNo);
	m_parLcomData->Lookup(iLcomNo, LcomDesign);

	return (m_bEqSpecial && LcomDesign.iSeismicType > 0 ? TRUE : FALSE);
}

//(Tel. 4022) MNET:XXXX-BSCHOI-20121025 #1
BOOL CDgnForceCtrl::IsEqSeismicLcomForBCJoint(int iLcomNo)
{
	_DGN_LCOM LcomDesign;
	LcomDesign.Initialize();
	Set_LcomDataForElem(m_iElemNo);
	m_parLcomData->Lookup(iLcomNo, LcomDesign);

	const bool bLcomStrn = IsLcomStrn(LcomDesign.LcomUlData.nActive);

	return (LcomDesign.iSeismicType > 0 && bLcomStrn) ? TRUE : FALSE;
}

CString CDgnForceCtrl::GetDgnLcomName(int nLcomNo)
{
	_DGN_LCOM LcomDesign;
	if (!m_parLcomData->Lookup(nLcomNo, LcomDesign)) return _T("");
	return LcomDesign.DesignLcomNa;
}

BOOL CDgnForceCtrl::IsEqSpecialProv() { return m_bEqSpecial; }

BOOL CDgnForceCtrl::IsEqSpecialElem(T_KEY ElemK, bool bWall)
{
	// MQC:3339-Seungjun-20100310 EC2:04일 경우 지하부재 capacity design에서 제외.
	if (m_iDgnCode == Eurocode2_04 && IsUnderGroundMemb(ElemK)) return FALSE; // check underground.
	if (m_iDgnCode == IS456_2000 && IsUnderGroundMemb(ElemK)) return FALSE; // check underground.
	if (!m_bEqSpecial) return FALSE;

	T_ELEM_K ElemNo = ElemK;
	BOOL bWallType = m_iRcsTypeKind / 10 == 4 || bWall ? TRUE : FALSE;
	BOOL bSecondaryElem = FALSE;
	if (bWallType)
	{
		T_WALL_K InWallK;
		InWallK.keymap = 0;
		CArray<T_ELEM_K, T_ELEM_K> arElemList;
		arElemList.RemoveAll();
		if (!m_pDoc->m_pPostCtrl->GetWallKey(ElemK, InWallK.keymap)) { ASSERT(0); }
		if (!m_pDoc->m_pPostCtrl->GetWallElemList(InWallK.keymap, arElemList)) { ASSERT(0); }
		int iNum = static_cast<int>(arElemList.GetSize());
		ElemNo = 0;

		// Modify by GAY. PMS:4076. ('10.05.07). 벽체는 해당 Element 중에 하나라도 Secondary로 고려되면, 설계 Unit 모두 Secondary로 고려.
		for (int i = 0; i < iNum; i++)
		{
			ElemNo = arElemList[i];
			if (m_arSecondaryElemList.Lookup(ElemNo, bSecondaryElem))
			{
				bSecondaryElem = TRUE;
				break;
			}
		}

		if (GetSeisClassWall(ElemK) == EN_DGN_SEIS_CLASS::NONDISSIPATIVE) { bSecondaryElem = TRUE; }
	}
	else
	{
		if (m_arSecondaryElemList.Lookup(ElemNo, bSecondaryElem))
		{
			bSecondaryElem = TRUE;
		}
		else if (m_iDgnCode == Eurocode2_04)
		{
			bSecondaryElem = IsUnderGroungElem() ? TRUE : FALSE;
		}
		else if (m_iDgnCode == IS456_2000)
		{
			bSecondaryElem = IsUnderGroungElem() ? TRUE : FALSE;
		}
		// PMS:4182-Seungjun-20100608 KBC2009 Seismic design.
		else
		{
			bSecondaryElem = TRUE;
			BOOL rVal = FALSE;
			// 1. if user input Seismic design elem.
			if (m_arSeismicElemList.Lookup(ElemK, rVal))
				bSecondaryElem = FALSE;
			// 2. if user input not-SFRS design elem.
			else if (m_arNotSFRSElemList.Lookup(ElemK, rVal) && m_iDgnClass == 1)
				bSecondaryElem = FALSE;
			// 3. Auto
			else if (m_iDgnCode == AIJ_WSD99)
			{// [4215-145 및 76] MQC(Alpha/Beta) 일본측 요청사항(2011.12.22 MSN)으로 지중기둥 및 지중보도 AIJ-WSD99일때 적용되도록 수정합니다.
				if (m_iElemKind == D_MBTP_COLUMN)
				{
					bSecondaryElem = FALSE;
				}
				else if (m_iElemKind == D_MBTP_BEAM)
				{
					bSecondaryElem = (IsCaltileverBeam() || IsSubBeam()) ? TRUE : FALSE;
				}
			}
			else
			{
				if (m_iElemKind == D_MBTP_COLUMN)
				{
					bSecondaryElem = IsUnderGroungElem() ? TRUE : FALSE;
				}
				else if (m_iElemKind == D_MBTP_BEAM)
				{
					bSecondaryElem = (IsUnderGroungElem() || IsCaltileverBeam() || IsSubBeam()) ? TRUE : FALSE;
				}
			}
		}

		if (GetSeisClassFram(ElemK) == EN_DGN_SEIS_CLASS::NONDISSIPATIVE) { bSecondaryElem = TRUE; }
	}

	return (!bSecondaryElem) ? TRUE : FALSE;
}

int CDgnForceCtrl::Get_AutoEqSpecialElemType(T_KEY ElemK)
{
	// MQC:3339-Seungjun-20100310 EC2:04일 경우 지하부재 capacity design에서 제외.
	if (m_iDgnCode == Eurocode2_04 && IsUnderGroundMemb(ElemK)) return 1; // check underground.
	if (!m_bEqSpecial) return 0;

	T_ELEM_K ElemNo = ElemK;
	BOOL bWall = m_iRcsTypeKind / 10 == 4 ? TRUE : FALSE;
	BOOL bSecondaryElem = FALSE;
	if (bWall)
	{
		return 0;
	}
	else
	{
		/*
		if(m_arSecondaryElemList.Lookup(ElemNo, bSecondaryElem))
		{
			return 0;
		}
		// PMS:4182-Seungjun-20100608 KBC2009 Seismic design.
		else
		*/
		{
			bSecondaryElem = TRUE;
			BOOL rVal = FALSE;
			// 1. if user input Seismic design elem.
			//if(m_arSeismicElemList.Lookup(ElemK, rVal))
				//return 0;
			// 2. if user input not-SFRS design elem.
			//else if(m_arNotSFRSElemList.Lookup(ElemK, rVal) && m_iDgnClass==1)
				//return 0;
			// 3. Auto
			//else
			{
				if (m_iElemKind == D_MBTP_COLUMN)
				{
					bSecondaryElem = IsUnderGroungElem() ? TRUE : FALSE;
				}
				else if (m_iElemKind == D_MBTP_BEAM)
				{
					bSecondaryElem = (IsUnderGroungElem() || IsCaltileverBeam() || IsSubBeam()) ? TRUE : FALSE;
				}
			}
		}
	}

	return (bSecondaryElem) ? 1 : 0;
}

// Modify by GAY. PMS:4076. ('10.04.28). Is Member designated as Seismic-Force-Resisting-System
BOOL CDgnForceCtrl::IsEqSFRSElem(T_KEY ElemK)
{
	if (m_iRcsTypeKind / 10 == 4) return FALSE;  // 벽체는 해당사항 없음. (KBC2009, 0520.9는 골조부재에 해당).

	BOOL bSFSRElem = TRUE;
	BOOL rVal = FALSE;
	if (m_arNotSFRSElemList.Lookup(ElemK, rVal)) bSFSRElem = FALSE;

	return (m_bEqSpecial && m_iDgnClass == 1 && bSFSRElem) ? TRUE : FALSE;
}

BOOL CDgnForceCtrl::GetMomByEqSpecial(int iLcomType, double& dMp, double& dMm, double& dVu, int& rnPosMp, int& rnPosMm)
{
	double dTu = 0.0;

	return GetMomByEqSpecial(iLcomType, dMp, dMm, dVu, dTu, rnPosMp, rnPosMm);
}

BOOL CDgnForceCtrl::GetMomByEqSpecial(int iLcomType, double& dMp, double& dMm, double& dVu, double& dTu, int& rnPosMp, int& rnPosMm)
{
	rnPosMp = rnPosMm = m_iPosiNo;

	double dMpOrg = fabs(max(m_BMy[m_iPosiNo], 0.0));
	double dMmOrg = fabs(min(m_BMy[m_iPosiNo], 0.0));
	double dVuOrg = fabs(m_Fzz[m_iPosiNo]);

	// Change by ZINU.('05.08.08). Modify Design Forces.
	// 1. For I(J), Set Moment at 1/4(3/4).
	// 2. From Positions to Regions.
	if (m_iPosiNo == 0 || m_iPosiNo == 2 || m_iPosiNo == 4)	// at I,1/2,J.
	{
		///////////////////////////////////////
		// Set My(+/-).
		double dMyPos = 0.0, dMyNeg = 0.0;
		switch (m_iPosiNo)
		{
		case 0:
		{
			dMyPos = max(m_BMy[1], 0.0);
			dMyNeg = min(m_BMy[1], 0.0);
			if (m_BMy[1] > 0.0) { rnPosMp = 1; }
			if (m_BMy[1] < 0.0) { rnPosMm = 1; }
		}
		break;
		case 2:
		{
			dMyPos = max(m_BMy[1], m_BMy[3]);
			dMyNeg = min(m_BMy[1], m_BMy[3]);
			rnPosMp = (m_BMy[1] > m_BMy[3]) ? 1 : 3;
			rnPosMm = (m_BMy[1] < m_BMy[3]) ? 1 : 3;
		}
		break;
		case 4:
		{
			dMyPos = max(0.0, m_BMy[3]);
			dMyNeg = min(0.0, m_BMy[3]);
			if (m_BMy[3] > 0.0) { rnPosMp = 3; }
			if (m_BMy[3] < 0.0) { rnPosMm = 3; }
		}
		break;
		default:
			ASSERT(0);
			break;
		}

		rnPosMp = dMyPos > 0.0 && fabs(dMyPos) > dMpOrg ? rnPosMp : m_iPosiNo;
		rnPosMm = dMyNeg < 0.0 && fabs(dMyNeg) > dMmOrg ? rnPosMm : m_iPosiNo;

		dMpOrg = (dMyPos > 0.0 ? max(fabs(dMyPos), dMpOrg) : dMpOrg);
		dMmOrg = (dMyNeg < 0.0 ? max(fabs(dMyNeg), dMmOrg) : dMmOrg);
		///////////////////////////////////////
		// Set Vu.
		double dVuCmp = 0.0;
		if (m_iPosiNo == 0)			dVuCmp = fabs(m_Fzz[1]);
		else if (m_iPosiNo == 2)	dVuCmp = max(fabs(m_Fzz[1]), fabs(m_Fzz[3]));
		else if (m_iPosiNo == 4)	dVuCmp = fabs(m_Fzz[3]);
		else	ASSERT(0);
		dVuOrg = max(dVuCmp, dVuOrg);
	}
	if (m_iPosiNo == 2)	// at 1/2.
	{
		// Change by ZINU.('05.02.24). Set Moments(at 1/2) by m_BMyMidUp, m_BMyMidDn.
		// Change by ZINU.('05.06.02). Compare Orginal Moment.
		// dMpOrg = fabs(m_BMyMidUp) -> max(fabs(m_BMyMidUp), dMpOrg).
		// dMmOrg = fabs(m_BMyMidDn) -> max(fabs(m_BMyMidDn), dMmOrg).
		dMpOrg = max(fabs(m_BMyMidUp), dMpOrg);
		dMmOrg = max(fabs(m_BMyMidDn), dMmOrg);
	}

	// PMS:tttt-Seungjun-20100310 
	const bool bLcomStrn = IsLcomStrn(iLcomType);
	BOOL bApplyEqSpecial = FALSE;
	if ((m_iDgnCode == Eurocode2_04 || m_iDgnCode == KCI_USD07 || m_iDgnCode == KCI_USD12 ||
		m_iDgnCode == KDS_41_30_2018 || m_iDgnCode == KDS_41_20_2022 || m_iDgnCode == IS456_2000) && bLcomStrn)
	{
		if (IsEqSpecialElem(m_iElemNo))
		{
			bApplyEqSpecial = TRUE;
		}
	}
	else if (IsEqSpecialProv() && bLcomStrn)
	{
		bApplyEqSpecial = TRUE;
	}

	// MQC.2335-Seungjun-20090204	
	if (bApplyEqSpecial)
	{
		double dEndIF = 1.0, dEndJF = 1.0, dMidF = 0.0;
		if (GetSpecialFactor(dEndIF, dEndJF, dMidF))
		{
			if (m_pDgnDataCtrl->m_pMembCtrl->m_bHcbmAssigned) dMidF = 0.0;

			double dMmi = 0.0, dMmj = 0.0, dMij = 0.0;
			for (int i = 0; i < 5; ++i)
			{
				switch (i)
				{
					// i-end
				case 0:
				case 1:
					dMmi = min(dMmi, m_BMy[i]); // I negative max
					dMij = max(dMij, fabs(m_BMy[i])); // I, J max
					break;
					// mid
				case 2:
					break;
					// j-end
				case 3:
				case 4:
					dMmj = min(dMmj, m_BMy[i]); // J negative max
					dMij = max(dMij, fabs(m_BMy[i])); // I, J max
					break;
				default:
					break;
				}
			}

			// Check Moments.
			double dMuLimI = dEndIF * fabs(dMmi);	// I Limit.
			double dMuLimJ = dEndJF * fabs(dMmj);	// J Limit.
			double dMuLimA = dMidF * dMij;	// All.
			if (dMuLimA > dMpOrg)	dMpOrg = dMuLimA;
			if (dMuLimA > dMmOrg)	dMmOrg = dMuLimA;
			if (m_iPosiNo == 0 && dMuLimI > dMpOrg)	dMpOrg = dMuLimI;
			if (m_iPosiNo == 4 && dMuLimJ > dMpOrg)	dMpOrg = dMuLimJ;
		}
	}

	// Save Changed Moments.
	dMp = dMpOrg;
	dMm = dMmOrg;
	dVu = dVuOrg;

	// Add by GAY. MNET:3780. ('08.10.28). Save Maximum Torsion Moment : Maximum Value for All-Position.
	dTu = max(max(fabs(m_BMx[0]), fabs(m_BMx[1])), max(m_BMx[2], m_BMx[3]));
	dTu = max(dTu, fabs(m_BMx[4]));
	if(m_iDgnCode == IS456_2000)
	{
		switch(m_iPosiNo)
		{
		case 0:
			dTu = max(fabs(m_BMx[0]), fabs(m_BMx[1]));
			break;
		case 2:
			dTu = max(max(fabs(m_BMx[1]), fabs(m_BMx[2])), fabs(m_BMx[3]));
		case 4:
			dTu = max(fabs(m_BMx[3]), fabs(m_BMx[4]));
		default:
			break;
		}
	}

	return (dMp + dMm + dVu > 0.0 ? TRUE : FALSE);
}

// MQC.2335-Seungjun-20090204
BOOL CDgnForceCtrl::GetSpecialFactor(double& dEndIF, double& dEndJF, double& dMiddleF)
{
	dEndIF = 0.0;
	dEndJF = 0.0;
	dMiddleF = 0.0;
	int iEqGrade = GetOkEqACI();

	// 설계 후 철근량을 기준에 제시하는 조건에 따라 맞춰주는 개념이므로 
	// 하중조합에 지진하중 포함 여부와 관계없이 모든 하중조합에 대하여 적용함. discuss with 이진우CJ, 이은경GJ, 김승준DR
	if (IsEqSpecialProv())  // Seismic Design, Strength/Stress (NOT Serviceability).
	{
		double dEndF = 0.0;
		double dMidF = 0.0;
		switch (m_iDgnCode)
		{
		case AIK_USD94: // See AIK-USD94 A2.1(2)(3) p.159~160.
		{
			dEndF = 1.0 / 3.0;
			dMidF = 1.0 / 5.0;
		}
		break;
		case KSCE_USD96: // See KSCE-USD96 II.4.2(2) p.356.
		{
			dEndF = 1.0 / 2.0;
			dMidF = 1.0 / 4.0;
		}
		break;
		case KCI_USD99: // See KCI-USD99 21.3.2(1) p.290.
		case KCI_USD03: // See KCI-USD03 21.3.2(1) p.376.
		{
			dEndF = 1.0 / 3.0;
			dMidF = 1.0 / 5.0;
		}
		break;
		case KCI_USD07:
		case KCI_USD12:
		case KDS_41_30_2018:
		case KDS_41_20_2022:
		{
			GetSeisReqMomentFactor_KCI_USD07_Series(m_iElemNo, iEqGrade, dEndF, dMidF);
		}
		break;
		case KCI_USD07_CVL:
		{
			dEndF = 1.0 / 3.0;
			dMidF = 1.0 / 5.0;
		}
		break;
		case KSCE_RC_USD05:
		{
			dEndF = 1.0 / 3.0;
			dMidF = 1.0 / 5.0;
		}
		break;
		case KSCE_RC_RAIL_USD04:
		{
			dEndF = 1.0 / 3.0;
			dMidF = 1.0 / 5.0;
		}
		break;
		case TWN_USD92: // See Taiwan-USD 15.4.2.2 p.15-6.
		case TWN_USD100:
		case TWN_USD112:
		{
			// MNET:3803-Seungjun-20090204
			dEndF = 1.0 / 2.0;
			dMidF = 1.0 / 4.0;
		}
		break;
		case ACI318_89:
		case ACI318_95:
		case ACI318_99:
		case ACI318_02:
		case ACI318_05:
		case ACI318_08:
		case ACI318_11:
		case ACI318_14:
		case ACI318M_14:
		case ACI318_19:
		case ACI318M_19:
		case ACI318_25:
		case ACI318M_25:
		case NSR_10: // Add by GAY. PMS:4450. ('12.03.13)
		case NSCP_2015_C:
		{
			GetSeisReqMomentFactor_ACI318_Series(m_iElemNo, m_iDgnCode, iEqGrade, dEndF, dMidF);
		}
		break;
		case CSA_A233_94: // See CSA-A23.3-94 21.3.2.2 p.141.
		case IS456_2000: // See IS13920:1993 6.2.3~4 p.3.
		{
			dEndF = 1.0 / 2.0;
			dMidF = 1.0 / 4.0;
		}
		break;
		case Eurocode2_04: // PMS:3956-Seungjun-20090224 EC8:04 Seismic Design
		{
			dEndF = 1.0 / 2.0;
			dMidF = 1.0 / 4.0;
		}
		break;
		case AIK_WSD2K:
		case AIJ_WSD99:
		case BS8110_97:
		case Eurocode2:
		case JTJ023_85:
		case AASHTO_LC96:
		case AASHTO_LC02:
		case AASHTO_LC12:
		case AASHTO_LC16:
		case AASHTO_LC17:
		case GB50010_02:
		case GB50010_10:
		case GB50010_19:
		case IRC_21_00:
		case IRC_112_11:
		case IRC_112_20:
		case TWN_BRG_LC90:
		case CSA_S6_00:
		case CSA_S6_10:
		case CSA_S6_14_RC:
		case CSA_S6_19_RC:
			//SRC
		case AIK_SRC2K: // Not Consider for *Mn(+), *Mn(-) in SRC Beam Design.
		case AIJ_SRC01:
		case TWN_SRC92:
		case TWN_SRC100:
		{
			// Not Applied.
			dEndF = 0.0;
			dMidF = 0.0;
		}
		break;
		case NTC_DCEC_2017:
		case NTC_DCEC_2023:
		{
			GetSeisReqMomentFactor_NTC_Series(m_iElemNo, m_iDgnCode, iEqGrade, dEndF, dMidF);
		}
		break;
		default:
		{
			ASSERT(0);
		}
		break;
		}

		// Coded by Seungjun ('20070515) MNet:No.XXXX. For Haunched Beam.
		double dHcbmIF = 1.0;
		double dHcbmJF = 1.0;
		if (m_pDgnDataCtrl->m_pMembCtrl->m_bHcbmAssigned)
		{
			int iHcbmPartNo = m_pDgnDataCtrl->m_pMembCtrl->GetHcbmPartNo(m_iElemNo);
			if (iHcbmPartNo == 0 && m_iPosiNo == 0)
			{
				dHcbmIF = 1.0;
				dHcbmJF = 0.0;
			}
			else if (iHcbmPartNo == 2 && m_iPosiNo == 4)
			{
				dHcbmIF = 0.0;
				dHcbmJF = 1.0;
			}
			else
			{
				dHcbmIF = 0.0;
				dHcbmJF = 0.0;
			}
		}
		dEndIF = dEndF * dHcbmIF;
		dEndJF = dEndF * dHcbmJF;
		dMiddleF = dMidF;
	}

	return max(max(dEndIF, dEndJF), dMiddleF) < cDgn_Zero ? FALSE : TRUE;
}

BOOL CDgnForceCtrl::GetForceKCI03(double& dMp, double& dMm, double& dVu)
{
	const int nPos = m_iPosiNo;
	dMp = max(m_BMy[nPos], 0.0);
	dMm = min(m_BMy[nPos], 0.0);
	dVu = m_Fzz[nPos];

	return TRUE;
}

BOOL CDgnForceCtrl::GetForce_IS456(double& dMp, double& dMn, double& dVu)
{
	const int nPos = m_iPosiNo;
	dMp = max(m_BMy[nPos], 0.0);
	dMn = min(m_BMy[nPos], 0.0);
	dVu = m_Fzz[nPos];

	return TRUE;
}
BOOL CDgnForceCtrl::GetForce_IRS(double& dMp, double& dMn, double& dVu, double& dTu, int iElemNo)
{
	dMp = max(m_BMy[m_iPosiNo], 0.0);
	dMn = min(m_BMy[m_iPosiNo], 0.0);
	dVu = m_Fzz[m_iPosiNo];
	dTu = 0.;
	m_bExistLctb = TRUE;
	Sta_ForceByLctb(iElemNo, TRUE);
	dTu = max(max(fabs(m_BMx[0]), fabs(m_BMx[1])), max(m_BMx[2], m_BMx[3]));
	dTu = max(dTu, fabs(m_BMx[4]));

	if (abs(dTu) > 0.0) return TRUE;
	T_LCOM_D LcomData;
	CArray<T_LCOM_K, T_LCOM_K> arLcomKey;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arLcomKey);
	for (int i = 0; i < arLcomKey.GetSize(); ++i)
	{
		LcomData.Initialize();
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, arLcomKey[i], LcomData);
		if (LcomData.nActive != 1)	continue;
		T_STRB_D strb;
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomData);
		if (m_pDoc->m_pPostCtrl->GetStrbNew(iElemNo, &strb, 0, 0, FALSE))
		{
			dTu = max(dTu, max(max(abs(strb.dblForce[0][3]), abs(strb.dblForce[1][3])), max(abs(strb.dblForce[2][3]), abs(strb.dblForce[3][3])))); // Pure torsion
		}
	}

	return TRUE;
}
BOOL CDgnForceCtrl::IsLongTerm(int iLcomNo)
{
	// TRUE=LongTerm, FALSE=ShortTerm.
	_DGN_LCOM LcomDesign;
	LcomDesign.Initialize();
	Set_LcomDataForElem(m_iElemNo);
	m_parLcomData->Lookup(iLcomNo, LcomDesign);

	return LcomDesign.bLongTerm;
}

void CDgnForceCtrl::SetLcomTypeGen(T_LCOM_BASE& LcomBase, int& iDgnLcomType, int iOrgLcomNo)
{
	// Add by ZINU.('04.02.02). For Lcom Type (L,S,X+/-,Y+/-).
	int iLcomTypeRmn = (iDgnLcomType / 10) * 10;
	int iLcomTypeOrg = iDgnLcomType % 10;
	int iLcomTypeGen = 0;
	// 0 = Long Term.
	// 1 = Short Term.
	// 2 = X+.
	// 3 = X-.
	// 4 = Y+.
	// 5 = Y-.
	int iAnalType = LcomBase.AnalType;
	int iLcaseKey = LcomBase.LoadCaseKey;
	double dFactor = LcomBase.Factor;

	T_LCTP_D LctpD; LctpD.Initialize();
	if (m_pDoc->m_pAttrCtrl->GetLctp(LctpD))
	{
		BOOL bSearchLong = FALSE;

		const INT_PTR nLong = LctpD.arLongKey.GetSize();
		for (INT_PTR i = 0; i < nLong; ++i)
		{
			BOOL bTypeOK = (iAnalType == LctpD.arLongType[i] ? TRUE : FALSE);
			BOOL bKeyOK = (iLcaseKey == LctpD.arLongKey[i] ? TRUE : FALSE);
			if (bTypeOK && bKeyOK) { bSearchLong = TRUE; break; }
		}
		iLcomTypeGen = (bSearchLong ? 0 : 1);

		const INT_PTR nShtX = LctpD.arShtXKey.GetSize();
		for (INT_PTR i = 0; i < nShtX; ++i)
		{
			if (iAnalType != LctpD.arShtXType[i])	continue;
			if (iLcaseKey == LctpD.arShtXKey[i])	iLcomTypeGen = max(iLcomTypeGen, (dFactor > 0.0 ? 2 : 3));
		}

		const INT_PTR nShtY = LctpD.arShtYKey.GetSize();
		for (INT_PTR i = 0; i < nShtY; ++i)
		{
			if (iAnalType != LctpD.arShtYType[i])	continue;
			if (iLcaseKey == LctpD.arShtYKey[i])	iLcomTypeGen = max(iLcomTypeGen, (dFactor > 0.0 ? 4 : 5));
		}
	}
	else	// Default.
	{
		if (iAnalType == D_LCOM_STATIC)
		{
			T_STLD_D StldD; StldD.Initialize();
			m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, StldD);
			CString strLcType = StldD.LoadCaseType;
			// Dead(D), Live(L), Roof Live(LR).
			// Change by ZINU.('04.03.13). Dead Type (D -> D,DC,DW).
			BOOL bLongTypeLc = CDBLib::IsStldLongTerm(strLcType);
			iLcomTypeGen = max(iLcomTypeGen, (bLongTypeLc ? 0 : 1));
		}
		else	iLcomTypeGen = 1;
	}
	iLcomTypeGen *= 1;	// 일의자리.
	iDgnLcomType = iLcomTypeRmn + max(iLcomTypeOrg, iLcomTypeGen);

	// Add by ZINU.('05.05.12). Special Load Combination Type.
	if (iAnalType == D_LCOM_STATIC)
	{
		T_STLD_D StldD; StldD.Initialize();
		m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, StldD);
		CString strLcType = StldD.LoadCaseType;
		if (CDBLib::IsStldLL(strLcType))			m_arExistLL.SetAt(iOrgLcomNo, TRUE);
		else if (CDBLib::IsStldEL(strLcType))
		{
			m_arExistEL.SetAt(iOrgLcomNo, TRUE);
			int rVal = 0;
			if (m_mapSeisLoodCase.Lookup(iOrgLcomNo, rVal)) rVal += 10;
			else rVal = 10;
			m_mapSeisLoodCase.SetAt(iOrgLcomNo, rVal);
		}
	}
	else if (iAnalType == D_LCOM_SPECTRUM || iAnalType == D_LCOM_ECCENSPEC)
	{
		m_arExistEL.SetAt(iOrgLcomNo, TRUE);
		int rVal = 0;
		if (m_mapSeisLoodCase.Lookup(iOrgLcomNo, rVal)) rVal += 1;
		else rVal = 1;
		m_mapSeisLoodCase.SetAt(iOrgLcomNo, rVal);
	}
	else if (iAnalType == D_LCOM_MOVING)
	{
		m_arExistLL.SetAt(iOrgLcomNo, TRUE);
	}
	else if (iAnalType == D_LCOM_CB_STEEL || iAnalType == D_LCOM_CB_CFSTEEL || iAnalType == D_LCOM_CB_CONCRETE || iAnalType == D_LCOM_CB_SRC)
	{
		// Change by ZINU.('07.07.09). NO:2903, Distinguish Envelope from SRSS.
		if (GetLoadCombType(iLcaseKey, iAnalType) == 3)	m_arExistEL.SetAt(iOrgLcomNo, TRUE);	// SRSS.
	}
}

int CDgnForceCtrl::GetLcomTypeGen(int iLcomNo)
{
	_DGN_LCOM LcomDesign;
	LcomDesign.Initialize();
	Set_LcomDataForElem(m_iElemNo);
	m_parLcomData->Lookup(iLcomNo, LcomDesign);
	return (LcomDesign.iDgnLcomType % 10);	// 일의자리.
}

int CDgnForceCtrl::GetLcomTypeCode(int iLcomNo)
{
	_DGN_LCOM LcomDesign;
	LcomDesign.Initialize();
	Set_LcomDataForElem(m_iElemNo);
	m_parLcomData->Lookup(iLcomNo, LcomDesign);
	return (LcomDesign.iDgnLcomType % 100) / 10;	// 백,십의자리.
}

void CDgnForceCtrl::SetLcomTypeJTJ(T_LCOM_BASE& LcomBase, int& iDgnLcomType)
{
	if (m_iDgnCode != JTJ025_86)	return;
	// Add by ZINU.('02.9.11). For JTJ025-86.
	int iLcomTypeRmn = (iDgnLcomType / 1000) * 1000 + iDgnLcomType % 10;
	int iLcomTypeOrg = (iDgnLcomType / 10) * 10;
	// See JTJ021-89 3.2.1.
	int iLcomTypeJTJ = 0;
	// LcomType 5 (지진하중).
	// LcomType 4 (시공단계하중조합).
	// LcomType 3 (선박,표류물 충격력).
	// LcomType 3 (이심력, 제동하중).

	int iAnalType = LcomBase.AnalType;
	int iLcaseKey = LcomBase.LoadCaseKey;

	if (iAnalType == D_LCOM_STATIC)
	{
		T_STLD_D StldD; StldD.Initialize();
		m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, StldD);
		CString strLcType = StldD.LoadCaseType;
		if (strLcType == _T("E"))				iLcomTypeJTJ = 5;
		else if (strLcType == _T("EVT"))	iLcomTypeJTJ = 5;
		else if (strLcType == _T("CV"))	iLcomTypeJTJ = 3;
		else if (strLcType == _T("CO"))	iLcomTypeJTJ = 3;
		else if (strLcType == _T("CT"))	iLcomTypeJTJ = 3;
		else if (strLcType == _T("IL"))	iLcomTypeJTJ = 3;
		else if (strLcType == _T("CF"))	iLcomTypeJTJ = 2;
		else if (strLcType == _T("BRK"))	iLcomTypeJTJ = 2;
		else											iLcomTypeJTJ = 1;
	}
	else if (iAnalType == D_LCOM_SPECTRUM)		iLcomTypeJTJ = 5;
	else if (iAnalType == D_LCOM_ECCENSPEC)	iLcomTypeJTJ = 5;
	else if (iAnalType == D_LCOM_STAGE)			iLcomTypeJTJ = 4;
	else if (iAnalType == D_LCOM_CB_STEEL || iAnalType == D_LCOM_CB_CFSTEEL || iAnalType == D_LCOM_CB_CONCRETE || iAnalType == D_LCOM_CB_SRC)
	{
		// Change by ZINU.('07.07.09). NO:2903, Distinguish Envelope from SRSS.
		if (GetLoadCombType(iLcaseKey, iAnalType) == 3)	iLcomTypeJTJ = 5;	// If SRSS, Assume EQ.
	}
	else if (iAnalType == D_LCOM_CB_ALUMINUM || iAnalType == D_LCOM_CB_STLCOMP) ASSERT(0);
	else																	iLcomTypeJTJ = 1;
	iLcomTypeJTJ *= 10;	// 백,십의자리.
	iDgnLcomType = iLcomTypeRmn + max(iLcomTypeOrg, iLcomTypeJTJ);
}

void CDgnForceCtrl::SetLcomTypeIRC(T_LCOM_BASE& LcomBase, int& iDgnLcomType)
{
	if (m_iDgnCode != IRC_21_00)	return;
	// Add by ZINU.('03.09.22). For IRC:21-2000.
	int iLcomTypeRmn = (iDgnLcomType / 1000) * 1000 + iDgnLcomType % 10;
	int iLcomTypeOrg = (iDgnLcomType / 10) * 10;
	// See IRC:6-2000 Table1 p.8.
	int iLcomTypeIRC = 0;
	// LcomType 4 (Vehicle Collision Load, Seismic)		1.50.
	// LcomType 3 (Wind, Wave Pressure) 							1.33.
	// LcomType 2 (Temperature, Deformation Effects)	1.15.
	// LcomType 1 (General)														1.00.

	int iAnalType = LcomBase.AnalType;
	int iLcaseKey = LcomBase.LoadCaseKey;

	if (iAnalType == D_LCOM_STATIC)
	{
		T_STLD_D StldD; StldD.Initialize();
		m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, StldD);
		CString strLcType = StldD.LoadCaseType;
		if (strLcType == _T("E"))				iLcomTypeIRC = 4;
		else if (strLcType == _T("EVT"))	iLcomTypeIRC = 4;
		else if (strLcType == _T("CO"))	iLcomTypeIRC = 4;
		else if (strLcType == _T("CT"))	iLcomTypeIRC = 4;
		else if (strLcType == _T("W"))		iLcomTypeIRC = 3;
		else if (strLcType == _T("WL"))	iLcomTypeIRC = 3;
		else if (strLcType == _T("WPR"))	iLcomTypeIRC = 3;
		else if (strLcType == _T("T"))		iLcomTypeIRC = 2;
		else if (strLcType == _T("RS"))	iLcomTypeIRC = 2;
		else if (strLcType == _T("TPG"))	iLcomTypeIRC = 2;
		else											iLcomTypeIRC = 1;
	}
	else if (iAnalType == D_LCOM_SPECTRUM)		iLcomTypeIRC = 4;
	else if (iAnalType == D_LCOM_ECCENSPEC)	iLcomTypeIRC = 4;
	else if (iAnalType == D_LCOM_CB_STEEL || iAnalType == D_LCOM_CB_CFSTEEL || iAnalType == D_LCOM_CB_CONCRETE || iAnalType == D_LCOM_CB_SRC)
	{
		// Change by ZINU.('07.07.09). NO:2903, Distinguish Envelope from SRSS.
		if (GetLoadCombType(iLcaseKey, iAnalType) == 3)	iLcomTypeIRC = 4;	// If SRSS, Assume EQ.
	}
	else if (iAnalType == D_LCOM_CB_ALUMINUM || iAnalType == D_LCOM_CB_STLCOMP) ASSERT(0);
	else																	iLcomTypeIRC = 1;
	iLcomTypeIRC *= 10;	// 백,십의자리.
	iDgnLcomType = iLcomTypeRmn + max(iLcomTypeOrg, iLcomTypeIRC);
}

void CDgnForceCtrl::SetLcomTypeAST(T_LCOM_BASE& LcomBase, int& iDgnLcomType)
{
	if (!(m_iDgnCode == AASHTO_AS96 || m_iDgnCode == TWN_BRG_AS90))	return;
	int iLcomTypeRmn = (iDgnLcomType / 1000) * 1000 + iDgnLcomType % 10;
	int iLcomTypeOrg = (iDgnLcomType / 10) * 10;
	// See AASHTO-Standard 96 3.18.2.3 p.30.
	int iLcomTypeAST = 0;
	// LcomType 1  (General)																1.00.
	// LcomType 11 (2 * Live Load)													1.50.
	// LcomType 12																					***		// Not used.
	// LcomType 2  (Wind load on structure)									1.25.
	// LcomType 3  (LcomType2 + Wind load on live load)			1.25.
	// LcomType 4  (Rib shortening, Shrinkage, Temperature)	1.25.
	// LcomType 5  (LcomType2 + LcomType4)									1.40.
	// LcomType 6  (LcomType3 + LcomType4)									1.40.
	// LcomType 7  (Earthquake)															1.33.
	// LcomType 8  (Ice pressure)														1.40.
	// LcomType 9  (LcomType2 + Ice pressure)								1.50.
	// LcomType 10 (General for Culvert)										1.00.	// Not used.

	int iAnalType = LcomBase.AnalType;
	int iLcaseKey = LcomBase.LoadCaseKey;
	double dFactor = LcomBase.Factor;

	if (iAnalType == D_LCOM_STATIC)
	{
		T_STLD_D StldD; StldD.Initialize();
		m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, StldD);
		CString strLcType = StldD.LoadCaseType;
		BOOL bLL = CDBLib::IsStldLL(strLcType);
		BOOL bWL = CDBLib::IsStldWL(strLcType);
		BOOL bEL = CDBLib::IsStldEL(strLcType);
		BOOL bTL = CDBLib::IsStldTL(strLcType);
		BOOL bIP = (strLcType == _T("IP") ? TRUE : FALSE);
		if (bLL && dFactor > 2.0 - cDgn_Zero)			m_bLL = TRUE;
		else if (bWL && dFactor > 1.0 - cDgn_Zero)	m_bWL = TRUE;
		else if (bWL && dFactor < 1.0 - cDgn_Zero)	m_bWLL = TRUE;
		else if (bEL)	m_bEL = TRUE;
		else if (bTL)	m_bTL = TRUE;
		else if (bIP)	m_bICE = TRUE;
	}
	else if (iAnalType == D_LCOM_SPECTRUM)		m_bEL = TRUE;
	else if (iAnalType == D_LCOM_ECCENSPEC)	m_bEL = TRUE;
	else if (iAnalType == D_LCOM_CB_STEEL || iAnalType == D_LCOM_CB_CFSTEEL || iAnalType == D_LCOM_CB_CONCRETE || iAnalType == D_LCOM_CB_SRC)
	{
		// Change by ZINU.('07.07.09). NO:2903, Distinguish Envelope from SRSS.
		if (GetLoadCombType(iLcaseKey, iAnalType) == 3)	m_bEL = TRUE;	// If SRSS, Assume EQ.
	}
	else if (iAnalType == D_LCOM_CB_ALUMINUM || iAnalType == D_LCOM_CB_STLCOMP) ASSERT(0);

	if (m_bWL && m_bICE)			iLcomTypeAST = 9;
	else if (m_bWLL && m_bTL)iLcomTypeAST = 6;
	else if (m_bWL && m_bTL)	iLcomTypeAST = 5;
	else if (m_bICE)					iLcomTypeAST = 8;
	else if (m_bEL)					iLcomTypeAST = 7;
	else if (m_bTL)					iLcomTypeAST = 4;
	else if (m_bWLL)					iLcomTypeAST = 3;
	else if (m_bWL)					iLcomTypeAST = 2;
	else if (m_bLL)					iLcomTypeAST = 11;
	else										iLcomTypeAST = 1;

	iLcomTypeAST *= 10;	// 백,십의자리.
	iDgnLcomType = iLcomTypeRmn + max(iLcomTypeOrg, iLcomTypeAST);
}

BOOL CDgnForceCtrl::Is_StctEL_DL(int nErLoad)
{
	auto L_IsStctEL = [](const CString& strStctEL) -> BOOL
	{
		if (strStctEL == _T("D")) return TRUE;
		if (strStctEL == _T("DC")) return TRUE;
		if (strStctEL == _T("DW")) return TRUE;

		return FALSE;
	};

	switch (nErLoad)
	{
	case 0: { return L_IsStctEL(m_strStctEL); }
	case 1: { return L_IsStctEL(m_strStctEL2); }
	case 2: { return L_IsStctEL(m_strStctEL3); }
	default: { return FALSE; }
	}
}

BOOL CDgnForceCtrl::Is_StctEL_LL(int nErLoad)
{
	auto L_IsStctLL = [](const CString& strStctEL) -> BOOL
	{
		if (strStctEL == _T("L")) return TRUE;
		if (strStctEL == _T("")) return TRUE;

		return FALSE;
	};

	switch (nErLoad)
	{
	case 0: { return L_IsStctLL(m_strStctEL); }
	case 1: { return L_IsStctLL(m_strStctEL2); }
	case 2: { return L_IsStctLL(m_strStctEL3); }
	default: { return FALSE; }
	}
}

BOOL CDgnForceCtrl::Is_StctEL_TL(int nErLoad)
{
	auto L_IsStctTL = [](const CString& strStctEL) -> BOOL
	{
		if (strStctEL == _T("T")) return TRUE;
		if (strStctEL == _T("TPG")) return TRUE;

		return FALSE;
	};

	switch (nErLoad)
	{
	case 0: { return L_IsStctTL(m_strStctEL); }
	case 1: { return L_IsStctTL(m_strStctEL2); }
	case 2: { return L_IsStctTL(m_strStctEL3); }
	default: { return FALSE; }
	}
}

void CDgnForceCtrl::SetLcomTypeIni()
{
	m_bLL = FALSE;	// 2*Live load.
	m_bWL = FALSE;	// Wind load on structure.
	m_bWLL = FALSE;	// 0.3*Wind load.
	m_bEL = FALSE;	// Earthquake.
	m_bTL = FALSE;	// Temperature.
	m_bICE = FALSE;	// Ice pressure.

	m_bExistLL = FALSE;	// Live Load for Special Load Combination Type.
}

void CDgnForceCtrl::SetSeismicType(T_LCOM_BASE& LcomBase, int& iSeismicType)
{
	// Add by ZINU.('03.01.17). For Vertical Eq.
	int iAnalType = LcomBase.AnalType;
	int iLcaseKey = LcomBase.LoadCaseKey;

	switch (iAnalType)
	{
	case D_LCOM_STATIC:
	{
		if (iSeismicType == EN_DGN_SEIS_NONE || iSeismicType == EN_DGN_SEIS_Z)
		{
			T_STLD_D StldD; StldD.Initialize();
			m_pDoc->m_pPostCtrl->GetStld(iLcaseKey, StldD);
			const CString& strLcType = StldD.LoadCaseType;
			if (strLcType == _T("EVT")) { iSeismicType = EN_DGN_SEIS_Z; }	// Only Z.
			else if (strLcType == _T("E")) { iSeismicType = EN_DGN_SEIS_XYZ; }	// XY, XY+Z.
		}
	}
	break;
	case D_LCOM_SPECTRUM:
	case D_LCOM_ECCENSPEC:
	{
		if (iSeismicType == EN_DGN_SEIS_NONE || iSeismicType == EN_DGN_SEIS_Z)
		{
			T_SPLC_D SplcD;
			m_pDoc->m_pAttrCtrl->GetSplc(iLcaseKey, SplcD);
			iSeismicType = SplcD.nDirection;	// XY(1), Z(2).
		}
	}
	break;
	case D_LCOM_SPECTRUM_ND:
	case D_LCOM_ECCENSPEC_ND:
	{
		if (iSeismicType == EN_DGN_SEIS_NONE || iSeismicType == EN_DGN_SEIS_Z)
		{
			T_NSPC_D NspcD;
			m_pDoc->m_pAttrCtrl->GetNspc(iLcaseKey, NspcD);
			iSeismicType = NspcD.nDirection;	// XY(1), Z(2).
		}
	}
	break;
	case D_LCOM_CB_STEEL:
	case D_LCOM_CB_CFSTEEL:
	case D_LCOM_CB_CONCRETE:
	case D_LCOM_CB_SRC:
	case D_LCOM_CB_ALUMINUM:
	case D_LCOM_CB_STLCOMP:
	{
		// Change by ZINU.('07.07.09). NO:2903, Distinguish Envelope from SRSS.
		if (GetLoadCombType(iLcaseKey, iAnalType) == 3)	// SRSS.
		{
			if (iSeismicType == EN_DGN_SEIS_NONE || iSeismicType == EN_DGN_SEIS_Z) { iSeismicType = EN_DGN_SEIS_XYZ; }	// XY, XY+Z.
		}
	}
	break;
	default:
		break;
	}
}

// Add by ZINU.('02.8.24).
int CDgnForceCtrl::IsCantilever(T_ELEM_K ElemKey)
{
	int iCantilever = 0;
	// Cantilever(+), No Cantilever(-).
	// Lateral restraint only(1).
	// Torsional restraint only(2).
	// Lateral and torsional restraint(3).
	// Built in laterally and torsionally(4).

	if (CTempDataStore_IsCantilever::GetInstance()->Get(ElemKey, iCantilever))
		return iCantilever;

	BOOL bReverseDir = FALSE;
	T_ELEM_K ElemShowKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	m_pDgnDataCtrl->m_pMembCtrl->GetElemListByIncludeElem(ElemKey, bReverseDir, ElemShowKey, aElemList);

	// Search Nodes (I,J) at Member.
	int iEdgeNode[2] = { 0,0 };
	T_ELEM_D eData;
	// Start Element.
	T_ELEM_K ElemKeySta = aElemList.GetAt(0);
	eData.Initialize();
	m_pDoc->m_pAttrCtrl->GetElem(ElemKeySta, eData);
	if (!m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp)) { CTempDataStore_IsCantilever::GetInstance()->Set(ElemKey, iCantilever);	return iCantilever; }
	iEdgeNode[0] = (bReverseDir ? eData.elnod[1] : eData.elnod[0]);
	// End Element.
	T_ELEM_K ElemKeyEnd = aElemList.GetAt(aElemList.GetSize() - 1);
	eData.Initialize();
	m_pDoc->m_pAttrCtrl->GetElem(ElemKeyEnd, eData);
	if (!m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp)) { CTempDataStore_IsCantilever::GetInstance()->Set(ElemKey, iCantilever);	return iCantilever; }
	iEdgeNode[1] = (bReverseDir ? eData.elnod[0] : eData.elnod[1]);

	if (!(iEdgeNode[0] > 0 && iEdgeNode[1] > 0)) { CTempDataStore_IsCantilever::GetInstance()->Set(ElemKey, iCantilever); return iCantilever; }

	// Change by ZINU.('03.02.06). Search Supports (I,J) at Member.
	int iEdgeSupp[2] = { 0 };
	BOOL bFixSupp[2] = { FALSE };

	for (int i = 0; i < 2; i++)
	{
		BOOL bFix[6] = { FALSE };
		m_pDoc->m_pAttrCtrl->IsDofConstraintForDesign(iEdgeNode[i], bFix);
		BOOL bLatFix = (bFix[0] || bFix[1] || bFix[2] ? TRUE : FALSE);
		BOOL bTorFix = (bFix[3] || bFix[4] || bFix[5] ? TRUE : FALSE);
		if (bLatFix && bTorFix)	iEdgeSupp[i] = 3;
		else if (bTorFix)				iEdgeSupp[i] = 2;
		else if (bLatFix)				iEdgeSupp[i] = 1;
		else										iEdgeSupp[i] = 0;

		// Check Support.
		T_CONS_D ConsData;
		ConsData.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetConsNode(iEdgeNode[i], ConsData))
		{
			bFixSupp[i] = TRUE;
			for (int j = 0; j < 6; j++)
			{
				if (ConsData.Constraint[j] != '1')	bFixSupp[i] = FALSE;
			}
		}
	}
	// Save Cantilever Data.
	if (iEdgeSupp[0] * iEdgeSupp[1] > 0)
	{
		if (bFixSupp[0] && iEdgeSupp[1] == 3)		iCantilever = 4;
		else if (bFixSupp[1] && iEdgeSupp[0] == 3)	iCantilever = 4;
		else									iCantilever = min(iEdgeSupp[0], iEdgeSupp[1]);
		iCantilever *= -1;
	}
	else
	{
		iCantilever = max(iEdgeSupp[0], iEdgeSupp[1]);	// The other Edge.
	}

	CTempDataStore_IsCantilever::GetInstance()->Set(ElemKey, iCantilever);
	return iCantilever;	// If over 0, Cantilever.
}


BOOL CDgnForceCtrl::IsSubBeamForNoMpr(T_ELEM_K ElemK, int nCantilever)
{
	/// [MQC 22637] IsCantilever(T_ELEM_K ElemKey) 함수에서 m_bSubBeamForNoMpr 변수와 관련된 부분만 추출하여 함수 구성.
	// PMS:4182-Seungjun-20100608 KBC2009 Seismic design.
	if (nCantilever > 0) return FALSE;  /// 캔틸레버인 경우에는 bSubBeamForNoMpr = FALSE; (아마도 우선순위가 Cantilever가 먼저인 듯...)

	T_NODE_K aEdgeNodeK[2] = { 0, 0 };
	if (!m_pDgnDataCtrl->m_pMembCtrl->GetEdgeNodeAtMemb(ElemK, aEdgeNodeK)) { ASSERT(0); return FALSE; }

	BOOL bSubBeamForNoMpr = FALSE;
	for (int i = 0; i < 2; i++)
	{
		BOOL bFix[6] = { FALSE };
		m_pDoc->m_pAttrCtrl->IsDofConstraintForDesign(aEdgeNodeK[i], bFix);
		// Add by ZINU.('08.03.22). NO:3294, NOT Apply Mpr if Sub-Beam.
		if (!bFix[2]) bSubBeamForNoMpr = TRUE;  // Ends-Supported(DZ) Beam.
	}
	return bSubBeamForNoMpr;
}

BOOL CDgnForceCtrl::IsSimplySupBeam(BOOL bMajorAxis)
{
	BOOL bSimplySupBeam = FALSE;
	// MQC:3699-Seungjun-20101104 simple support beam인지 판단할 때 캘틸레버가 아니고야 양단 모멘트가 0.0인 경우로 수정.
	if (m_iCantilever > 0) return bSimplySupBeam; // cantilever.
	// Maximum End Moments at Member.
	double dMmax = 0.0;
	if (bMajorAxis)	dMmax = max(fabs(m_BMy[0]), fabs(m_BMy[4]));
	else						dMmax = max(fabs(m_BMz[0]), fabs(m_BMz[4]));
	if (dMmax < cDgn_Zero && m_iElemType == BEAM_EL/* && m_iCantilever==0*/)	bSimplySupBeam = TRUE;

	return bSimplySupBeam;
}

BOOL CDgnForceCtrl::IsUnderGroundMemb(T_ELEM_K ElemKey)
{
	if (ElemKey == 0) { return FALSE; }

	if (m_iElemKind == D_MBTP_WALL)
	{
		// PMS:tttt-Seungjun-20100310 
		//return FALSE; // check underground. // not apply on 780.
		T_WALL_K InWallK;
		InWallK.keymap = 0;
		CArray<T_ELEM_K, T_ELEM_K> arElemList;
		arElemList.RemoveAll();
		if (!m_pDoc->m_pPostCtrl->GetWallKey(ElemKey, InWallK.keymap)) { ASSERT(0); return FALSE; }
		if (!m_pDoc->m_pPostCtrl->GetWallElemList(InWallK.keymap, arElemList)) { ASSERT(0); return FALSE; }

		T_ELEM_D ElemD;
		if (!m_pDoc->m_pAttrCtrl->GetElem(arElemList[0], ElemD)) { ASSERT(0); return FALSE; }

		T_BLDC_D BldcD;
		if (!m_pDoc->m_pAttrCtrl->GetBldc(BldcD))  BldcD.Initialize();
		BOOL bUnderGroundEnd[2] = { FALSE,FALSE };
		T_NODE_K EdgeNodeK[2] = { static_cast<T_NODE_K>(ElemD.elnod[0]), static_cast<T_NODE_K>(ElemD.elnod[3]) };
		T_NODE_D EdgeNodeD;
		for (int i = 0; i < 2; i++)
		{
			EdgeNodeD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetNode(EdgeNodeK[i], EdgeNodeD)) return FALSE;
			if (EdgeNodeD.z < BldcD.dBaseLevel + cDgn_Zero) bUnderGroundEnd[i] = TRUE;
		}
		return (bUnderGroundEnd[0] && bUnderGroundEnd[1]);
	}
	else
	{
		const int nMbtp = m_pDoc->m_pAttrCtrl->GetMemberType(ElemKey);
		switch (nMbtp)
		{
		case D_MBTP_BEAM:
		case D_MBTP_COLUMN:
		case D_MBTP_BRACE:
		{
			// Only Beam, Column. -> 지하구조물 하중조합 추가하면서 Brace도 열기.
			T_BLDC_D BldcD;
			if (!m_pDoc->m_pAttrCtrl->GetBldc(BldcD))  BldcD.Initialize();
			BOOL bUnderGroundEnd[2] = { FALSE,FALSE };
			// Modify by GAY. MQC:1911. ('08.06.02). Get Edge Node Key for current member.
			T_NODE_K EdgeNodeK[2] = { 0, 0 };
			T_NODE_D EdgeNodeD;
			if (!m_pDgnDataCtrl->m_pMembCtrl->GetEdgeNodeAtMemb(ElemKey, EdgeNodeK)) { ASSERT(0); return FALSE; }
			for (int i = 0; i < 2; i++)
			{
				EdgeNodeD.Initialize();
				if (!m_pDoc->m_pAttrCtrl->GetNode(EdgeNodeK[i], EdgeNodeD)) return FALSE;
				if (EdgeNodeD.z < BldcD.dBaseLevel + cDgn_Zero) bUnderGroundEnd[i] = TRUE;
			}
			return (bUnderGroundEnd[0] && bUnderGroundEnd[1]);
		}
		default:
		{
			//ASSERT(0);
			return FALSE;
		}
		}
	}
}

// Add by ZINU.('01.8.27).
BOOL CDgnForceCtrl::IsSingleCurve(double dM04, double dM14, double dM24, double dM34, double dM44)
{
	// See AIK-ASD83 2.1.4. p.71.
	BOOL bSingleCurve = TRUE;

	// Modify by GAY. MNET:2355. ('06.07.27). 벽인 경우에는 단부 모멘트만 비교한다.
	if (m_pDoc->m_pAttrCtrl->IsWall(m_iElemType))
	{
		if (bSingleCurve && dM04*dM44 < 0.0)	bSingleCurve = FALSE;
	}
	else
	{
		// Modify by GAY. MNET:2374. ('06.08.03). Check condition for curvature.
		int iPositive = 0;
		if (dM04 > 0.0) iPositive += 1;
		if (dM14 > 0.0) iPositive += 1;
		if (dM24 > 0.0) iPositive += 1;
		if (dM34 > 0.0) iPositive += 1;
		if (dM44 > 0.0) iPositive += 1;

		int iNegative = 0;
		if (dM04 < 0.0) iNegative += 1;
		if (dM14 < 0.0) iNegative += 1;
		if (dM24 < 0.0) iNegative += 1;
		if (dM34 < 0.0) iNegative += 1;
		if (dM44 < 0.0) iNegative += 1;

		if (iPositive > 0 && iNegative > 0) bSingleCurve = FALSE;
	}
	return bSingleCurve;
}

//Is j-node Top Position?
BOOL CDgnForceCtrl::IsJNodeTop(T_ELEM_K ElemKey)
{
	T_ELEM_D eData;
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemKey, eData))  eData.Initialize();

	// Add by GAY.('06.02.28). Assign Element Node Type. Where is J Node Location?
	T_NODE_D nodeI, nodeJ;
	if (eData.eltyp == WALL_EL)
	{
		if (!m_pDoc->m_pAttrCtrl->GetNode(eData.elnod[0], nodeI)) ASSERT(0);
		if (!m_pDoc->m_pAttrCtrl->GetNode(eData.elnod[3], nodeJ)) ASSERT(0);
	}
	else
	{
		if (!m_pDoc->m_pAttrCtrl->GetNode(eData.elnod[0], nodeI)) ASSERT(0);
		if (!m_pDoc->m_pAttrCtrl->GetNode(eData.elnod[1], nodeJ)) ASSERT(0);
	}

	return (nodeI.z < nodeJ.z) ? TRUE : FALSE;
}

// Add by ZINU.('02.12.03). For Lateral Displacement Control.
void CDgnForceCtrl::SetLongTermDgn(BOOL bLongTerm)
{
	if (m_bOnlyLongTerm != bLongTerm)	m_bOnlyLongTerm = bLongTerm;
}

bool CDgnForceCtrl::IsUseLcomUG(T_ELEM_K ElemKorUnInterWallID)
{
	T_MATD_D MatdD;
	if (m_iRcsTypeKind == RCS_WDGN || m_iRcsTypeKind == RCS_WCHK)
	{
		T_WALL_KEY WallKey;
		m_pDoc->m_pPostCtrl->GetWallKey(ElemKorUnInterWallID, WallKey);
		int nMatl = 0;
		GetDgnWallMatd(WallKey, nMatl, MatdD);
		m_iElemKind = D_MBTP_WALL;
	}
	else
	{
		GetDgnElemMatd(ElemKorUnInterWallID, MatdD);

		T_MBTP_D MbtpD;
		m_pDgnDataCtrl->Get_DgnGenMbtp(ElemKorUnInterWallID, MbtpD);
		m_iElemKind = MbtpD.nMbType;
		m_iElemSubKind = MbtpD.nSubType;
	}	

	const bool bCodeLcomUG = [](int nCode, const CString& strMatlType) -> bool
	{
		switch (nCode)
		{
		case KDS_41_31_2019: return strMatlType == _LSX(S) ? true : false;
		case KDS_41_30_10_2022: return strMatlType == _LSX(S) ? true : false;
		case KDS_41_30_2018: return strMatlType == _LSX(C) ? true : false;
		case KDS_41_20_2022: return strMatlType == _LSX(C) ? true : false;
		default: return false;
		}
	}(m_iDgnCode, MatdD.Type);

	if (!bCodeLcomUG) { return false; }

	const bool bApplyLcomUG = [this](const CString& strMatlType) -> bool
	{
		if (strMatlType == _LSX(C))
		{
			T_DCON_D DconD;
			m_pDgnDataCtrl->Get_DgnConDcon(DconD);

			return DconD.bUseUGLcom4UGMemb ? true : false;
		}

		if (strMatlType == _LSX(S))
		{
			T_DSTL_D DstlD;
			m_pDgnDataCtrl->Get_DgnStlDstl(DstlD);

			return DstlD.bUseUGLcom4UGMemb ? true : false;
		}

		return false;
	}(MatdD.Type);

	int nUlct = 0;
	if (!GetUnderGroundTypeByUlct(ElemKorUnInterWallID, nUlct))
	{
		if (!bApplyLcomUG) { return false; }
		return IsUnderGroundMemb(ElemKorUnInterWallID) ? true : false;
	}

	return nUlct == EN_ULCT_UNDERGROUND ? true : false;
}

BOOL CDgnForceCtrl::IsTransferStage(int iLcomNo)
{
	Set_LcomDataForElem(m_iElemNo);

	_DGN_LCOM LcomDesign;
	if (!m_parLcomData->Lookup(iLcomNo, LcomDesign)) { return FALSE; }

	return (LcomDesign.OriginalLcomKey == m_iLcomKeyTran) ? TRUE : FALSE;
}

// Add by ZINU.('02.12.21). To Check Final Lcom for Service.
BOOL CDgnForceCtrl::IsFinalSerLcom(int iLcomNo)
{
	Set_LcomDataForElem(m_iElemNo);

	int iLcomSerCount = 0;
	const INT_PTR nSizeLcom = m_parLcomData->GetCount();
	for (INT_PTR i = 0; i < nSizeLcom; ++i)
	{
		_DGN_LCOM LcomDesign;
		m_parLcomData->Lookup(i + 1, LcomDesign);
		if (IsLcomServ(LcomDesign.LcomUlData.nActive)) { iLcomSerCount++; }
		if (iLcomNo == i + 1) { break; }
	}

	return (iLcomSerCount == m_iLCBServiceNum) ? TRUE : FALSE;
}

BOOL CDgnForceCtrl::GetStrbDgn(T_ELEM_K Key, int iLcaseType, int LcaseK, T_STRB_D* pDataMax, T_STRB_D* pDataMin)
{
	//*/PROFILING(_T("GetStrbDgn1"));
	pDataMax->Initialize();
	pDataMin->Initialize();
	// Select Lcom.	
	BOOL bChk = FALSE;
	if (m_iOrgLcomKey == m_iLcomKeyTran)	// Transfer Stage.
	{
		int iStepNo = 0;
		m_arStepData.Lookup(Key, iStepNo);
		if (iStepNo <= 0) { ASSERT(0); return FALSE; }
		bChk = m_pDgnDataCtrl->m_pMembCtrl->GetStrbDgn(Key, D_LOADCASE_TRANSFER, iStepNo, pDataMax, pDataMin);
	}
	else	// Service Load Stage.
	{
		bChk = m_pDgnDataCtrl->m_pMembCtrl->GetStrbDgn(Key, iLcaseType, LcaseK, pDataMax, pDataMin);
	}

	// Add by SHIN //////////////////////////////////////////////////////////////////////////
	// PMS:4215 - 일본2차설계(iGen)의 기능중 _T("계산정보>>단면공통>>단면산정>>단부위치설정") 기능구현을 위해서
	//            Load Case별로 장기,단기에 따라 단부부재력을 실질적으론 다른위치의 부재력을 합하여 줄수 있습니다.
	if (m_pDgnDataCtrl->IsJUD()) // Add by SHIN - PMS:4215  
	{
		int  nTermID = 1;                      // Add by SHIN - PMS:4215  장기:0, 단기:1 (해당항목은 m_dOffSetRyi, Rzi, Ryj, Rzj의 장기 단기 배열과 연관됩니다.) 
		T_LCTP_D LctpD; LctpD.Initialize();
		T_STLD_D StldD; StldD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetLctp(LctpD))
		{
			int nLong = static_cast<int>(LctpD.arLongKey.GetSize());
			for (int i = 0; i < nLong; ++i)
			{
				if (LcaseK == LctpD.arLongKey[i]) { nTermID = 0;  break; }
			}
		}
		else if (m_pDoc->m_pAttrCtrl->GetStld(LcaseK, StldD))
		{
			if (CDBLib::IsStldLongTerm(StldD.LoadCaseType)) { nTermID = 0; }			
		}

		double dMyMax[CONST_DGN_iPOSNUM] = { pDataMax->dblForce[0][4], pDataMax->dblForce[1][4], pDataMax->dblForce[2][4], pDataMax->dblForce[3][4], pDataMax->dblForce[3][10] };
		double dMzMax[CONST_DGN_iPOSNUM] = { pDataMax->dblForce[0][5], pDataMax->dblForce[1][5], pDataMax->dblForce[2][5], pDataMax->dblForce[3][5], pDataMax->dblForce[3][11] };
		double dMyMin[CONST_DGN_iPOSNUM] = { pDataMin->dblForce[0][4], pDataMin->dblForce[1][4], pDataMin->dblForce[2][4], pDataMin->dblForce[3][4], pDataMin->dblForce[3][10] };
		double dMzMin[CONST_DGN_iPOSNUM] = { pDataMin->dblForce[0][5], pDataMin->dblForce[1][5], pDataMin->dblForce[2][5], pDataMin->dblForce[3][5], pDataMin->dblForce[3][11] };

		pDataMax->dblForce[0][4] = Get_MomentInterpolate(dMyMax[0], dMyMax[4], dMyMax[2], m_dOffSetRyi[nTermID], 1.0);
		pDataMax->dblForce[0][5] = Get_MomentInterpolate(dMzMax[0], dMzMax[4], dMzMax[2], m_dOffSetRzi[nTermID], 1.0);
		pDataMax->dblForce[3][10] = Get_MomentInterpolate(dMyMax[4], dMyMax[0], dMyMax[2], m_dOffSetRyj[nTermID], 1.0);
		pDataMax->dblForce[3][11] = Get_MomentInterpolate(dMzMax[4], dMzMax[0], dMzMax[2], m_dOffSetRzj[nTermID], 1.0);
		pDataMin->dblForce[0][4] = Get_MomentInterpolate(dMyMin[0], dMyMin[4], dMyMin[2], m_dOffSetRyi[nTermID], 1.0);
		pDataMin->dblForce[0][5] = Get_MomentInterpolate(dMzMin[0], dMzMin[4], dMzMin[2], m_dOffSetRzi[nTermID], 1.0);
		pDataMin->dblForce[3][10] = Get_MomentInterpolate(dMyMin[4], dMyMin[0], dMyMin[2], m_dOffSetRyj[nTermID], 1.0);
		pDataMin->dblForce[3][11] = Get_MomentInterpolate(dMzMin[4], dMzMin[0], dMzMin[2], m_dOffSetRzj[nTermID], 1.0);
	}
	return bChk;
}

BOOL CDgnForceCtrl::GetStrtDgn(T_ELEM_K Key, int iLcaseType, int LcaseK, T_STRT_D* pDataMax, T_STRT_D* pDataMin)
{
	pDataMax->Initialize();
	pDataMin->Initialize();
	// Select Lcom.
	if (m_iOrgLcomKey == m_iLcomKeyTran)	// Transfer Stage.
	{
		int iStepNo = 0;
		m_arStepData.Lookup(Key, iStepNo);
		if (iStepNo <= 0) { ASSERT(0); return FALSE; }
		return m_pDgnDataCtrl->m_pMembCtrl->GetStrtDgn(Key, D_LOADCASE_TRANSFER, iStepNo, pDataMax, pDataMin);
	}
	else	// Service Load Stage.
	{
		return m_pDgnDataCtrl->m_pMembCtrl->GetStrtDgn(Key, iLcaseType, LcaseK, pDataMax, pDataMin);
	}
}

BOOL CDgnForceCtrl::GetStrwDgn(T_ELEM_K Key, int iLcaseType, int LcaseK, T_STRW_D* pDataMax, T_STRW_D* pDataMin)
{
	pDataMax->Initialize();
	pDataMin->Initialize();
	// Select Lcom.
	if (m_iOrgLcomKey == m_iLcomKeyTran)	// Transfer Stage.
	{
		int iStepNo = 0;
		m_arStepData.Lookup(Key, iStepNo);
		if (iStepNo <= 0) { ASSERT(0); return FALSE; }
		return m_pDgnDataCtrl->m_pMembCtrl->GetStrwDgn(Key, D_LOADCASE_TRANSFER, iStepNo, pDataMax, pDataMin);
	}
	else	// Service Load Stage.
	{
		return m_pDgnDataCtrl->m_pMembCtrl->GetStrwDgn(Key, iLcaseType, LcaseK, pDataMax, pDataMin);
	}
}

BOOL CDgnForceCtrl::GetStrpDgn(T_ELEM_K Key, int iLcaseType, int LcaseK, T_STRP_DL* pDataMax, T_STRP_DL* pDataMin)
{
	pDataMax->Initialize();
	pDataMin->Initialize();
	// Select Lcom.
	if (m_iOrgLcomKey == m_iLcomKeyTran)	// Transfer Stage.
	{
		int iStepNo = 0;
		m_arStepData.Lookup(Key, iStepNo);
		if (iStepNo <= 0) { ASSERT(0); return FALSE; }
		return m_pDgnDataCtrl->m_pMembCtrl->GetStrpDgn(Key, D_LOADCASE_TRANSFER, iStepNo, pDataMax, pDataMin);
	}
	else	// Service Load Stage.
	{
		return m_pDgnDataCtrl->m_pMembCtrl->GetStrpDgn(Key, iLcaseType, LcaseK, pDataMax, pDataMin);
	}
}

BOOL CDgnForceCtrl::GetStrbDgn(T_ELEM_K StaElemK, T_ELEM_K EndElemK, int iLcaseType, int LcaseK, T_STRB_D* pDataMax, T_STRB_D* pDataMin)
{
	//*/PROFILING(_T("GetStrbDgn2"));
	pDataMax->Initialize();
	pDataMin->Initialize();
	// Select Lcom.
	BOOL bChk = FALSE;
	if (m_iOrgLcomKey == m_iLcomKeyTran)	// Transfer Stage.
	{
		int iStepNo = 0;
		m_arStepData.Lookup(StaElemK, iStepNo);	// Assume StaElemK.
		if (iStepNo <= 0) { ASSERT(0); return FALSE; }
		bChk = m_pDgnDataCtrl->m_pMembCtrl->GetStrbDgn(StaElemK, EndElemK, D_LOADCASE_TRANSFER, iStepNo, pDataMax, pDataMin);
	}
	else	// Service Load Stage.
	{
		bChk = m_pDgnDataCtrl->m_pMembCtrl->GetStrbDgn(StaElemK, EndElemK, iLcaseType, LcaseK, pDataMax, pDataMin);
	}

	// Add by SHIN //////////////////////////////////////////////////////////////////////////
	// PMS:4215 - 일본2차설계(iGen)의 기능중 _T("계산정보>>단면공통>>단면산정>>단부위치설정") 기능구현을 위해서
	//            Load Case별로 장기,단기에 따라 단부부재력을 실질적으론 다른위치의 부재력을 합하여 줄수 있습니다.
	if (m_pDgnDataCtrl->IsJUD()) // Add by SHIN - PMS:4215  
	{
		int  nTermID = 1;                      // Add by SHIN - PMS:4215  장기:0, 단기:1 (해당항목은 m_dOffSetRyi, Rzi, Ryj, Rzj의 장기 단기 배열과 연관됩니다.) 
		T_LCTP_D LctpD; LctpD.Initialize();
		T_STLD_D StldD; StldD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetLctp(LctpD))
		{
			int nLong = static_cast<int>(LctpD.arLongKey.GetSize());
			for (int i = 0; i < nLong; ++i)
			{
				if (LcaseK == LctpD.arLongKey[i]) { nTermID = 0;  break; }
			}
		}
		else if (m_pDoc->m_pAttrCtrl->GetStld(LcaseK, StldD))
		{
			if (IsDeadLoadType(StldD.LoadCaseType, D_LCOM_STATIC, LcaseK) || IsLiveLoadType(StldD.LoadCaseType, D_LCOM_STATIC)) { nTermID = 0; }
		}

		double dMyMax[CONST_DGN_iPOSNUM] = { pDataMax->dblForce[0][4], pDataMax->dblForce[1][4], pDataMax->dblForce[2][4], pDataMax->dblForce[3][4], pDataMax->dblForce[3][10] };
		double dMzMax[CONST_DGN_iPOSNUM] = { pDataMax->dblForce[0][5], pDataMax->dblForce[1][5], pDataMax->dblForce[2][5], pDataMax->dblForce[3][5], pDataMax->dblForce[3][11] };
		double dMyMin[CONST_DGN_iPOSNUM] = { pDataMin->dblForce[0][4], pDataMin->dblForce[1][4], pDataMin->dblForce[2][4], pDataMin->dblForce[3][4], pDataMin->dblForce[3][10] };
		double dMzMin[CONST_DGN_iPOSNUM] = { pDataMin->dblForce[0][5], pDataMin->dblForce[1][5], pDataMin->dblForce[2][5], pDataMin->dblForce[3][5], pDataMin->dblForce[3][11] };

		if (StaElemK == m_apElemK_y[0].GetAt(0))
		{
			pDataMax->dblForce[0][4] = Get_MomentInterpolate(dMyMax[0], dMyMax[4], dMyMax[2], m_dOffSetRyi[nTermID], 1.0);
			pDataMax->dblForce[0][5] = Get_MomentInterpolate(dMzMax[0], dMzMax[4], dMzMax[2], m_dOffSetRzi[nTermID], 1.0);
			pDataMin->dblForce[0][4] = Get_MomentInterpolate(dMyMin[0], dMyMin[4], dMyMin[2], m_dOffSetRyi[nTermID], 1.0);
			pDataMin->dblForce[0][5] = Get_MomentInterpolate(dMzMin[0], dMzMin[4], dMzMin[2], m_dOffSetRzi[nTermID], 1.0);
		}

		if (EndElemK == m_apElemK_y[1].GetAt(m_apElemK_y[1].GetSize() - 1))
		{
			pDataMax->dblForce[3][10] = Get_MomentInterpolate(dMyMax[4], dMyMax[0], dMyMax[2], m_dOffSetRyj[nTermID], 1.0);
			pDataMax->dblForce[3][11] = Get_MomentInterpolate(dMzMax[4], dMzMax[0], dMzMax[2], m_dOffSetRzj[nTermID], 1.0);
			pDataMin->dblForce[3][10] = Get_MomentInterpolate(dMyMin[4], dMyMin[0], dMyMin[2], m_dOffSetRyj[nTermID], 1.0);
			pDataMin->dblForce[3][11] = Get_MomentInterpolate(dMzMin[4], dMzMin[0], dMzMin[2], m_dOffSetRzj[nTermID], 1.0);
		}
	}

	return bChk;
}

double CDgnForceCtrl::Get_GravelDia()
{
	double dGravelDia = 0.0;
	// 1 in or 2.5 cm.
	switch (m_nCode_Length)
	{
	case(D_UNITSYS_LENGTH_INDEX_M): {dGravelDia = 0.025;	break; }
	case(D_UNITSYS_LENGTH_INDEX_CM): {dGravelDia = 2.5;		break; }
	case(D_UNITSYS_LENGTH_INDEX_MM): {dGravelDia = 25.0;		break; }
	case(D_UNITSYS_LENGTH_INDEX_IN): {dGravelDia = 1.0;		break; }
	case(D_UNITSYS_LENGTH_INDEX_FT): {dGravelDia = 1. / 12.;	break; }
	}

	if (m_iDgnCode == TWN_SRC92 || m_iDgnCode == TWN_SRC100) dGravelDia = 2.0;
	if (dGravelDia == 0.0)	ASSERT(0);

	return dGravelDia;
}

double CDgnForceCtrl::Calc_RbarToStlDist(double dBarDia)
{
	double dClearDist = 0.0;
	switch (m_iDgnCode)
	{
	case SSRC79: { dClearDist = 1.0; } break; // 1.0 in.
	case AIK_SRC2K: { dClearDist = 2.5; } break; // 2.5 cm.
	case AIJ_SRC01: { dClearDist = max(25., 1.25*Get_GravelDia()); } break; // max(25 mm, Gravel Dia).
	case JGJ138_01: { dClearDist = 25.; } break; // 25 mm.
	case TWN_SRC92: { dClearDist = max(2.5, 1.25*Get_GravelDia()); } break; // max(2.5cm, Gravel Dia).
	case TWN_SRC100: { dClearDist = max(2.5, 1.25*Get_GravelDia()); } break; // max(2.5cm, Gravel Dia).
	default: ASSERT(0); break;
	}

	return dClearDist + dBarDia / 2.0; // dBarToStlDist
}

double CDgnForceCtrl::Calc_ConcToStlDist()
{
	double dConToStlDist = 0.0;
	switch (m_iDgnCode)
	{
	case SSRC79: { dConToStlDist = 0.0; } break;
	case AIK_SRC2K: { dConToStlDist = 5.0; } break; // See AIK-SRC2K 5.1.2(1) p.72, 5 cm.
	case AIJ_SRC01: { dConToStlDist = 50.; } break; // See AIJ-SRC01 12(2) p.6, 50 mm.
	case JGJ138_01: { dConToStlDist = 0.0; } break;
	case TWN_SRC92: { dConToStlDist = 5.0; } break; // See TWN-SRC92 4.5.1 p.33, 5 cm.
	case TWN_SRC100: { dConToStlDist = 5.0; } break; // See TWN-SRC92 4.5.1 p.33, 5 cm.
	default: ASSERT(0); break;
	}
	return dConToStlDist;
}

void CDgnForceCtrl::InitSavedDgnForce()
{
	m_FstlBaseMap.RemoveAll();
	m_FbemBaseMap.RemoveAll();
	m_FcolBaseMap.RemoveAll();
	m_FbrcBaseMap.RemoveAll();
	m_FwalBaseMap.RemoveAll();
	m_FscmBaseMap.RemoveAll();
	m_FsbmBaseMap.RemoveAll();
}

BOOL CDgnForceCtrl::Get_SpecialLcomOmega(int iLcasNo, int iAnalType, double& dOmega)
{
	double dSubOmega = 1.0;
	switch (iAnalType)
	{
	case D_LCOM_STATIC:
	{
		m_arSlcfStldK.Lookup(iLcasNo, dSubOmega);
	}
	break;
	case D_LCOM_SPECTRUM:
	{
		m_arSlcfSplcK.Lookup(iLcasNo, dSubOmega);
	}
	break;
	case D_LCOM_ECCENSPEC:
	{
		m_arSlcfSplcK.Lookup(iLcasNo, dSubOmega);
	}
	break;
	case D_LCOM_CB_STEEL:
	case D_LCOM_CB_CFSTEEL:
	case D_LCOM_CB_CONCRETE:
	case D_LCOM_CB_SRC:
	case D_LCOM_CB_ALUMINUM:
	case D_LCOM_CB_STLCOMP:
	{
		int nLcomType = m_pDoc->m_pAttrCtrl2->GetLcomTypeByAnalType(iAnalType);

		T_LCOM_D LcomD;
		if (m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, iLcasNo, LcomD))
		{
			int nComb = static_cast<int>(LcomD.aCombination.GetSize());
			for (int i = 0; i < nComb; ++i)
			{
				int iSubLcaseKey = LcomD.aCombination[i].LoadCaseKey;
				int iSubAnalType = LcomD.aCombination[i].AnalType;
				if (iSubLcaseKey == 0)	break;
				if (!Get_SpecialLcomOmega(iSubLcaseKey, iSubAnalType, dSubOmega))	break;
				dSubOmega = max(dOmega, dSubOmega);
			}
		}
	}
	break;
	default:  ASSERT(0); break;
	}
	// Compare Omega.
	dOmega = max(dOmega, dSubOmega);

	return TRUE;
}

double CDgnForceCtrl::Get_SpecialLcomOmega(int iElemNo, int iLcasNo, int iAnalType)
{
	double dOmega = 1.0;
	double dFactor = 0.0;
	if (m_arSlcmSpecial.Lookup(iElemNo, dFactor))
	{
		Get_SpecialLcomOmega(iLcasNo, iAnalType, dOmega);
	}
	return dOmega;
}

BOOL CDgnForceCtrl::Get_SpecialLcomFactor(int iElemNo, double& dFactor, double& dSds)
{
	dFactor = 0.0;
	m_arSlcmSpecial.Lookup(iElemNo, dFactor);

	dSds = 0.0;
	m_arSlcmSds.Lookup(iElemNo, dSds);

	return (dFactor*dSds > 0.0) ? TRUE : FALSE;
}

BOOL CDgnForceCtrl::Get_VerticalLcomFactor(int iElemNo, double& dFactor)
{
	dFactor = 0.0;
	return m_arSlcmVertical.Lookup(iElemNo, dFactor) ? TRUE : FALSE;
}

void CDgnForceCtrl::SetSpecVertLcomInfoMap(BOOL bWall)
{
	T_SLCF_D SlcfD;
	if (m_pDoc->m_pAttrCtrl->GetSlcf(SlcfD))
	{
		int nSize = static_cast<int>(SlcfD.aLoadPattern.GetSize());
		for (int i = 0; i < nSize; ++i)
		{
			const T_SOSF_BASE& SosfBase = SlcfD.aLoadPattern.GetAt(i);
			switch (SosfBase.nLoadCaseType)
			{
			case 0: { m_arSlcfStldK.SetAt(SosfBase.LoadCaseKey, SosfBase.dScaleFactor); } break;
			case 1: { m_arSlcfSplcK.SetAt(SosfBase.LoadCaseKey, SosfBase.dScaleFactor); } break;
			case 2: { m_arSlcfStldLLK.SetAt(SosfBase.LoadCaseKey, SosfBase.dScaleFactor); } break;
			default: ASSERT(0); break;
			}
		}

		T_SLCM_K SlcmK;	// =ElemK.
		T_SLCM_D SlcmD;
		T_ELEM_D ElemD;
		POSITION PosSlcm = m_pDoc->m_pAttrCtrl->GetStartSlcm();
		while (PosSlcm)
		{
			SlcmD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNextSlcm(PosSlcm, SlcmK, SlcmD);
			ElemD.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(SlcmK, ElemD);
			if (bWall && m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))	// Wall.
			{
				unsigned int nInternalWallId = 0;
				if (!m_pDoc->m_pPostCtrl->GetInternalWallId(SlcmK, nInternalWallId))	continue;
				// 0=Special Seismic Load Effect, 1=Vertical Seismic Forces.
				if (SlcmD.nType == 0)
				{
					double dSds = 0.0;
					m_arSlcmSds.Lookup(nInternalWallId, dSds);
					m_arSlcmSds.SetAt(nInternalWallId, max(SlcfD.dSDS, dSds));
					double dSpecial = 0.0;
					m_arSlcmSpecial.Lookup(nInternalWallId, dSpecial);
					m_arSlcmSpecial.SetAt(nInternalWallId, max(SlcfD.dDLoadFactor, dSpecial));
				}
				else if (SlcmD.nType == 1)
				{
					double dVertical = 0.0;
					m_arSlcmVertical.Lookup(nInternalWallId, dVertical);
					m_arSlcmVertical.SetAt(nInternalWallId, max(SlcfD.dVLoadFactor, dVertical));
				}
			}
			else if (!bWall && m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))	// Truss, Beam.
			{
				// 0=Special Seismic Load Effect, 1=Vertical Seismic Forces.
				if (SlcmD.nType == 0)
				{
					m_arSlcmSds.SetAt(SlcmK, SlcfD.dSDS);
					m_arSlcmSpecial.SetAt(SlcmK, SlcfD.dDLoadFactor);
				}
				else if (SlcmD.nType == 1)	m_arSlcmVertical.SetAt(SlcmK, SlcfD.dVLoadFactor);
				else	ASSERT(0);
			}
		}
	}
}

void CDgnForceCtrl::SetSlcmTypeMap()
{
	SetSlcmTypeMap(m_mapSlcmType);
}

int CDgnForceCtrl::GetSlcmType(const int& iElem)
{
	int nType = -1;
	T_ELEM_D ElemD;
	if(!m_pDoc->m_pAttrCtrl->GetElem(iElem, ElemD)) { ASSERT(0); return nType; }
	if(m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))	// Wall.
	{
		UINT nInternalWallId = 0;
		if(m_pDoc->m_pPostCtrl->GetInternalWallId(iElem, nInternalWallId))
		{
			T_WALL_KEY WallKey = 0;
			if(m_pDoc->m_pPostCtrl->GetWallKey(nInternalWallId, WallKey))
			{
				if(!m_mapSlcmType.Lookup(WallKey, nType)) nType = -1;
			}
		}
	}
	else if (m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))	// Truss, Beam.
	{
		if(!m_mapSlcmType.Lookup(iElem, nType)) nType = -1;
	}

	return nType;
}

void CDgnForceCtrl::SetSlcmTypeMap(CMap<UINT, UINT, int, int>& rmapSlcmType)
{
	rmapSlcmType.RemoveAll();

	/// for BEAM/ COLUMN/ BRACE/ WALL	
	POSITION PosSlcm = m_pDoc->m_pAttrCtrl->GetStartSlcm();
	while (PosSlcm)
	{
		T_SLCM_K SlcmK = 0;	// =ElemK.
		T_SLCM_D SlcmD;
		m_pDoc->m_pAttrCtrl->GetNextSlcm(PosSlcm, SlcmK, SlcmD);

		T_ELEM_D ElemD;
		if (!m_pDoc->m_pAttrCtrl->GetElem(SlcmK, ElemD)) { ASSERT(0); continue; }
		if (m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))	// Wall.
		{
			UINT nInternalWallId = 0;
			if (!m_pDoc->m_pPostCtrl->GetInternalWallId(SlcmK, nInternalWallId)) { continue; }

			T_WALL_KEY WallKey = 0;
			if (!m_pDoc->m_pPostCtrl->GetWallKey(nInternalWallId, WallKey)) { continue; }

			int nType = -1;
			if(rmapSlcmType.Lookup(WallKey, nType))
			{
				if(nType == 0) continue; // if already set as Special Seismic Load Effect, skip.
			}
			rmapSlcmType.SetAt(WallKey, SlcmD.nType);
		}
		else if (m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))	// Truss, Beam.
		{
			rmapSlcmType.SetAt(SlcmK, SlcmD.nType);
		}
	}

	/// for MESHED-SLAB.
	POSITION PosSsla = m_pDoc->m_pAttrCtrl2->GetStartSsla();
	while (PosSsla)
	{
		T_SSLA_K SslaK = 0;
		T_SSLA_D SslaD;

		SslaD.Initialize();
		m_pDoc->m_pAttrCtrl2->GetNextSsla(PosSsla, SslaK, SslaD);

		T_SSLC_D SslcD;
		if (!m_pDoc->m_pAttrCtrl2->GetSslc(SslaD.SslcK, SslcD)) { continue; }

		if (SslcD.nMembType == 0) // column strip에 대한 정보는 strip 설계 시에 설정함.
		{
			rmapSlcmType.SetAt(SslaK, SslcD.nSeisType);
		}
	}
}

void CDgnForceCtrl::SetSlcmTypeMapStrip(T_MSTR_K MstrK)
{
	// strip member에 정의된 하중조합 type을 그 영역에 포함된 요소번호와 matching.
	// 어떤 plate 요소는 여러 개의 strip에 속할 수 있으므로, 이렇게 반영함.

	T_MSTR_D MstrD;
	if (!m_pDoc->m_pAttrCtrl2->GetMstr(MstrK, MstrD)) { ASSERT(0); return; }

	T_MEMB_D MembD;
	if (!m_pDoc->m_pAttrCtrl->GetMemb(MstrD.MembK, MembD)) { ASSERT(0); return; }

	BOOL bFind = FALSE;
	T_ELEM_K ElemK = MembD.aElemList[0];  // 부재의 설계변수는 대표요소의 설계변수를 따름. (보/기둥 적용규칙 그대로 따름)
	T_SSLA_K SslaK;
	T_SSLA_D SslaD;
	T_SSLC_D SslcD;
	POSITION PosSsla = m_pDoc->m_pAttrCtrl2->GetStartSsla();
	while (PosSsla)
	{
		SslaD.Initialize();
		m_pDoc->m_pAttrCtrl2->GetNextSsla(PosSsla, SslaK, SslaD);

		if (ElemK == SslaK)
		{
			if (!m_pDoc->m_pAttrCtrl2->GetSslc(SslaD.SslcK, SslcD)) { continue; }
			if (SslcD.nMembType != 1) { continue; }

			bFind = TRUE;
			break;
		}
	}

	if (bFind)
	{
		T_ELEM_K_LIST aElemK;
		int nElem = m_pDoc->m_pAttrCtrl2->GetElemKIncludedInStrip(MstrK, aElemK);

		int nSpec = 0;
		int nVert = 0;
		for (int i = 0; i < nElem; ++i)
		{
			m_mapSlcmType.SetAt(aElemK[i], SslcD.nSeisType);
		}

		m_mapSlcmTypeStrip.SetAt(MstrK, SslcD.nSeisType);
	}
}

BOOL CDgnForceCtrl::IsElemSpecSeisLcom(int nElemNo)
{
	UINT unKey = nElemNo;
	if (m_iRcsTypeKind == RCS_WDGN || m_iRcsTypeKind == RCS_WCHK)
	{
		if (!m_pDoc->m_pPostCtrl->GetWallKey(nElemNo, unKey)) return FALSE;
	}

	int nSlcmType = 0;
	if (!m_mapSlcmType.Lookup(unKey, nSlcmType)) return FALSE;
	return nSlcmType == 0 ? TRUE : FALSE;
}

BOOL CDgnForceCtrl::IsElemVertSeisLcom(int nElemNo)
{
	UINT unKey = nElemNo;
	if (m_iRcsTypeKind == RCS_WDGN || m_iRcsTypeKind == RCS_WCHK)
	{
		if (!m_pDoc->m_pPostCtrl->GetWallKey(nElemNo, unKey)) return FALSE;
	}

	int nSlcmType = 0;
	if (!m_mapSlcmType.Lookup(unKey, nSlcmType)) return FALSE;
	return nSlcmType == 1 ? TRUE : FALSE;
}

BOOL CDgnForceCtrl::IsStripSpecSeisLcom(T_MSTR_K MstrK)
{
	int nSlcmType = 0;
	if (!m_mapSlcmTypeStrip.Lookup(MstrK, nSlcmType)) return FALSE;
	return nSlcmType == 0 ? TRUE : FALSE;
}

BOOL CDgnForceCtrl::IsStripVertSeisLcom(T_MSTR_K MstrK)
{
	int nSlcmType = 0;
	if (!m_mapSlcmTypeStrip.Lookup(MstrK, nSlcmType)) return FALSE;
	return nSlcmType == 1 ? TRUE : FALSE;
}

int CDgnForceCtrl::GetStripSeisLcomType(T_MSTR_K MstrK)
{
	int nSlcmType = 0;
	if (!m_mapSlcmTypeStrip.Lookup(MstrK, nSlcmType)) return -1;
	return nSlcmType;
}

void CDgnForceCtrl::SetDgnLcomListForCurElem(int ElemK)
{
	m_parLcomData = &m_arLcomStrn;

	const EN_DGNLCOM_APP enLcomApp = GetAppDgnLcomTypeByMemb(ElemK);
	m_parLcomData = GetAppDgnLcomMap(enLcomApp);
}

EN_DGNLCOM_APP CDgnForceCtrl::GetAppDgnLcomTypeByMemb(T_ELEM_K ElemK) // in case of Wall, ElemK = unInternalWalID
{
	UINT unKey = ElemK;
#if !defined(_CIVIL) //CIVIL에서는 Wall 을 가져오는 수단이 없음, 현재 GEN에서는 가능
	if (m_iRcsTypeKind == RCS_WDGN || m_iRcsTypeKind == RCS_WCHK)
	{
		if (!m_pDoc->m_pPostCtrl->GetWallKey(ElemK, unKey))
		{
			ASSERT(0);
			return EN_DGNLCOM_APP::NONE;
		}
	}
#endif

	const bool bUseLcomUG = IsUseLcomUG(ElemK);

	int nSlcmType = 0;
	if (m_bSclmCode && m_mapSlcmType.Lookup(unKey, nSlcmType))
	{
		switch (nSlcmType)
		{
		case 0:
		{
			return bUseLcomUG ? EN_DGNLCOM_APP::SPEC_SERV_UG : EN_DGNLCOM_APP::SPEC_SERV;
		}
		case 1:
		{
			return bUseLcomUG ? EN_DGNLCOM_APP::NONE : EN_DGNLCOM_APP::VERT_SERV; /// 지하구조물 검토 대상 부재에 Vertical로 적용 된 경우는 Skip
		}
		default:
		{
			ASSERT(0);
			return EN_DGNLCOM_APP::NONE;
		}
		}
	}

	return bUseLcomUG ? EN_DGNLCOM_APP::STRN_SERV_UG : EN_DGNLCOM_APP::STRN_SERV;
}

_DGN_LCOM_MAP* CDgnForceCtrl::GetAppDgnLcomMap(const EN_DGNLCOM_APP enDgnLcomApp)
{
	_DGN_LCOM_MAP* paDgnLcom = nullptr;
	switch (enDgnLcomApp)
	{
	case EN_DGNLCOM_APP::NONE: break;
	case EN_DGNLCOM_APP::STRN_SERV: { paDgnLcom = &m_arLcomStrn; } break;
	case EN_DGNLCOM_APP::SPEC_SERV: { paDgnLcom = &m_arLcomSpec; } break;
	case EN_DGNLCOM_APP::VERT_SERV: { paDgnLcom = &m_arLcomVert; } break;
	case EN_DGNLCOM_APP::STRN_SERV_UG: { paDgnLcom = &m_arLcomStrnUG; } break;
	case EN_DGNLCOM_APP::SPEC_SERV_UG: { paDgnLcom = &m_arLcomSpecUG; } break;
	default: { ASSERT(0); } break;
	}
	return paDgnLcom;
}

int CDgnForceCtrl::GetSizeAppDgnLcomMap(const EN_DGNLCOM_APP enDgnLcomApp)
{
	switch (enDgnLcomApp)
	{
	case EN_DGNLCOM_APP::NONE: { return 0; }
	case EN_DGNLCOM_APP::STRN_SERV: { return m_iLcomStrn; }
	case EN_DGNLCOM_APP::SPEC_SERV: { return m_iLcomSpec; }
	case EN_DGNLCOM_APP::VERT_SERV: { return m_iLcomVert; }
	case EN_DGNLCOM_APP::STRN_SERV_UG: { return m_iLcomStrnUG; }
	case EN_DGNLCOM_APP::SPEC_SERV_UG: { return m_iLcomSpecUG; }
	default: { ASSERT(0); return 0.0; }
	}
}

int CDgnForceCtrl::GetSizeAppDgnLcomMapServ(const EN_DGNLCOM_APP enDgnLcomApp)
{
	switch (enDgnLcomApp)
	{
	case EN_DGNLCOM_APP::NONE: { return 0.0; }
	case EN_DGNLCOM_APP::STRN_SERV: { return m_iLcomServ; }
	case EN_DGNLCOM_APP::SPEC_SERV: { return m_iLcomServ; }
	case EN_DGNLCOM_APP::VERT_SERV: { return m_iLcomServ; }
	case EN_DGNLCOM_APP::STRN_SERV_UG: { return m_iLcomServUG; }
	case EN_DGNLCOM_APP::SPEC_SERV_UG: { return m_iLcomServUG; }
	default: { ASSERT(0); return 0.0; }
	}
}

bool CDgnForceCtrl::IsSameDgnLcomTypeInSlab(const T_ELEM_K_LIST& aElemK)
{
	const INT_PTR nSize = aElemK.GetSize();
	if (nSize < 2) { return true; }

	auto L_IsSlabND = [this](T_ELEM_K ElemK) -> bool
	{
		return GetSeisClassSlab(ElemK) == EN_DGN_SEIS_CLASS::NONDISSIPATIVE ? true : false;
	};

	const T_ELEM_K FirstElemK = aElemK[0];
	const bool bFirstND = L_IsSlabND(FirstElemK);
	const int nFirstSlcm = GetSlcmTypeSlab(FirstElemK);

	for (INT_PTR i = 1; i < nSize; ++i)
	{
		const T_ELEM_K CurElemK = aElemK[i];

		const bool bCurND = L_IsSlabND(CurElemK);
		if (bFirstND != bCurND) { return false; }

		const int nCurSlcm = GetSlcmTypeSlab(CurElemK);
		if (nFirstSlcm != nCurSlcm) { return false; }
	}

	return true;
}

bool CDgnForceCtrl::IsSameDgnLcomTypeInSlab(const _DGN_PERI_PART& PeriPart)
{
	T_ELEM_K_LIST aPeriElemK;
	const INT_PTR nSize = PeriPart.aResList.GetSize();
	for (INT_PTR i = 0; i < nSize; ++i)
	{
		aPeriElemK.Add(PeriPart.aResList[i].iSlabNo);
	}
	CArrayUtil::RemoveDup(aPeriElemK);

	return IsSameDgnLcomTypeInSlab(aPeriElemK);
}

bool CDgnForceCtrl::IsValidLcomSlabStrn(int nULSnELS, bool bExcludeSeis, const _DGN_LCOM& LcomDesign)
{
	const T_LCOM_K OrgLcomK = LcomDesign.OriginalLcomKey;
	if (m_nSelectLcomK != 0) // 선택된 LcomK만 수행.
	{
		if (OrgLcomK != m_nSelectLcomK) { return false; }
	}

	const int nActive = LcomDesign.LcomUlData.nActive;
	if (IsLcomServ(nActive)) { return false; }

	const bool bIsLcomULS = [bExcludeSeis, &LcomDesign]() -> bool
	{
		if (!bExcludeSeis) { return true; }
		switch (LcomDesign.iSeismicType)
		{
		case EN_DGN_SEIS_XYZ:
		case EN_DGN_SEIS_Z:
			return false;
		case EN_DGN_SEIS_NONE:
			return true;
		default:
			ASSERT(0);
			return true;
		}
	}();

	switch (nULSnELS)
	{
	case EN_LSIDX_ULS:
		return nActive == EN_DGN_STRE ? false : bIsLcomULS;
	case EN_LSIDX_ELS:
		return nActive != EN_DGN_STRE ? false : true;
	default:
		ASSERT(0);
		return false;
	}
}

bool CDgnForceCtrl::IsConcurrentLcomIndexFor7Dof(const _DGN_LCOM& LcomDesign)
{
	if (LcomDesign.iLcomCompType == 7 || LcomDesign.iLcomCompType == 8 || LcomDesign.iLcomCompType == 9)
	{
		return true;
	}
	return false;
}

// Coded by Seungjun MNet:No.2591 ('20061130)	For SCWB Design.
// Only Concrete.
BOOL CDgnForceCtrl::Get_EQBendingMoment(BOOL bDuctileDesign)
{
	const int iEqGrade = GetOkEqACI();
	// by GAY. PMS:5009. ('15.03.31). add Colombia design code.       
	switch (m_iDgnCode)
	{
	case KCI_USD07:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case ACI318_89:
	case ACI318_95:
	case ACI318_99:
	case ACI318_02:
	case ACI318_05:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case NSCP_2015_C:
	case IS456_2000:
	{
		if (iEqGrade != 1) { return TRUE; }
	}
	break;
	case NSR_10:
	{
		if (!(iEqGrade == 1 || iEqGrade == 2)) { return TRUE; }
	}
	break;
	case NTC_DCEC_2017:
	case NTC_DCEC_2023:
	{
		if (!(iEqGrade == 1 || iEqGrade == 2 || iEqGrade == 3)) { return TRUE; }
	}
	break;
	default:
		break;
	}

	CCurUnitSaver Save(TRUE);

	// Coded by Seungjun MNet:No.2591 ('20061215)	For SCWB Design For Test.
	BOOL bTestPrint = FALSE;
	CString strTestFile = _T("");
	if (CDBLib::CheckTheDesignTestType(2))// && m_pDoc->GetDesignStatus()==8)
	{
		strTestFile.Format(_T("zzzDgn_MNet2591SCWB.txt"));
		rptwofstream fout(strTestFile, ios::trunc);
		bTestPrint = TRUE;
	}

	// Get Load Combination Key.
	CArray<T_LCOM_K, T_LCOM_K> arLcomK;
	arLcomK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arLcomK);
	if (arLcomK.GetSize() < 0) return FALSE;

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bCngUnit = FALSE;
	if (CurIndex.nBase_Length != m_nCode_Length || CurIndex.nBase_Force != m_nCode_Force)
	{
		CngIndex.nBase_Length = m_nCode_Length;
		CngIndex.nBase_Force = m_nCode_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
		bCngUnit = TRUE;
	}
	//++++++++++++++++++++++++++++++++++++++++++++

	// Coded by Seungjun ('20070614) MNet:No.2583. 수정
	if (bDuctileDesign)
	{
		// Get Beam End Bending Strength. (Probable Strength)
		SCWBRatio SCWBRat;

		if(iEqGrade == 1 && IsJointCodeUseVcol(m_iDgnCode)) // for joint
		{
			if (!SCWBRat.Get_GirderBM(m_arGirVey, m_arGirVey, m_arGirVez, m_arGirVez, EN_FORCE_SHEAR))
			{
				// <Remember> Set Data by Current Units.
				if (bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
				return FALSE;
			}
		}
		// Coded by Seungjun ('20070302) MNet:No.2086.
		// Get Flexural Design Strength.
		if (!SCWBRat.Get_GirderBM(m_arGirMnyCW, m_arGirMnyCCW, m_arGirMnzCW, m_arGirMnzCCW, EN_FORCE_MOMENT_DGN))
		{
			// <Remember> Set Data by Current Units.
			if (bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
			return FALSE;
		}
		// Get Probable Moment.
		if (!SCWBRat.Get_GirderBM(m_arGirMpryCW, m_arGirMpryCCW, m_arGirMprzCW, m_arGirMprzCCW, EN_FORCE_MOMENT_PROP))
		{
			// <Remember> Set Data by Current Units.
			if (bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
			return FALSE;
		}
		// PMS:3956-Seungjun-20090603 EC8:04 Seismic Design
		if (m_iDgnCode == Eurocode2_04 || m_iDgnCode == ACI318_14 || m_iDgnCode == ACI318M_14 ||
			m_iDgnCode == ACI318_19 || m_iDgnCode == ACI318M_19 || m_iDgnCode == NSR_10 ||
			m_iDgnCode == ACI318_25 || m_iDgnCode == ACI318M_25 ||
			m_iDgnCode == NSCP_2015_C || m_iDgnCode == IS456_2000 || m_iDgnCode == NTC_DCEC_2017 || m_iDgnCode == NTC_DCEC_2023)
		{
			T_DCON_D DconD;
			m_pDgnDataCtrl->Get_DgnConDcon(DconD);
			SCWBRat.Set_UseUpperAngle(DconD.nCheckPos == EN_SELECT_CHECK_POS_BOT ? TRUE : FALSE);

			if (!SCWBRat.Get_GirderAs(m_arGirAsTopy, m_arGirAsBoty, m_arGirAsTopz, m_arGirAsBotz))
			{
				// <Remember> Set Data by Current Units.
				if (bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
				return FALSE;
			}
			if (!SCWBRat.Get_GirderAsMap(DconD, m_mBeamSumAsFyY, m_mBeamSumAsFyZ))
			{
				if (bCngUnit) m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
				return FALSE;
			}
		}

		// 상부기둥의 beta angle에 맞춰서 보강도 계산.
		SCWBRat.Set_UseUpperAngle(TRUE);
		if (iEqGrade == 1 && IsJointCodeUseVcol(m_iDgnCode)) // for joint
		{
			if (!SCWBRat.Get_GirderBM(m_arUpGirVey, m_arUpGirVey, m_arUpGirVez, m_arUpGirVez, EN_FORCE_SHEAR))
			{
				// <Remember> Set Data by Current Units.
				if (bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
				return FALSE;
			}
		}
		// Get Flexural Design Strength.
		if (!SCWBRat.Get_GirderBM(m_arUpGirMnyCW, m_arUpGirMnyCCW, m_arUpGirMnzCW, m_arUpGirMnzCCW, EN_FORCE_MOMENT_DGN))
		{
			// <Remember> Set Data by Current Units.
			if (bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
			return FALSE;
		}
		// Get Probable Moment.
		if (!SCWBRat.Get_GirderBM(m_arUpGirMpryCW, m_arUpGirMpryCCW, m_arUpGirMprzCW, m_arUpGirMprzCCW, EN_FORCE_MOMENT_PROP))
		{
			// <Remember> Set Data by Current Units.
			if (bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
			return FALSE;
		}
		SCWBRat.Set_UseUpperAngle(FALSE);
	}

	// Get Element Key List.
	CArray<T_ELEM_K, T_ELEM_K> arElemK;
	arElemK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemK);
	// Get Static Load Case Key List.
	CArray<T_STLD_K, T_STLD_K> arStldK;
	arStldK.RemoveAll();
	m_pDoc->m_pPostCtrl->GetStldKeyList(arStldK);
	// Get Spectrum Load Case Key List.
	CArray<T_SPLC_K, T_SPLC_K> arSplcK;
	arSplcK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(arSplcK);
	// Get SRSS Type Load Combination.
	CArray<unsigned int, unsigned int> arSrssK;
	arSrssK.RemoveAll();
	for (int k = 0; k < arLcomK.GetSize(); k++)
	{
		// Change by ZINU.('07.07.09). NO:2903, Distinguish Envelope from SRSS.
		unsigned int iLcaseKey = arLcomK.GetAt(k);
		// 주의 - SRSS의 경우 하중조합 Key를 하중조건 Key처럼 사용. 
		if (GetLoadCombType(iLcaseKey, D_LCOM_CB_CONCRETE) == 3)	arSrssK.Add(iLcaseKey);
	}

	// Get Bending moment of element.
	int i = 0, j = 0;
	int iElemCount = 0, iLcaseCount = 0;
	iElemCount = arElemK.GetSize();

	for (i = 0; i < iElemCount; i++)
	{
		// 제한사항
		T_ELEM_K ElemK = arElemK.GetAt(i);

		// Check the element.
		/**/int iMBType = 0;
		/**/if (!m_pDgnDataCtrl->IsRBeamOrColumn(ElemK, iMBType))  continue;

		// Set Union Key.
		_DGN_SCWB_K SCWBK;
		SCWBK.Key._ElemInfo.ElemD.ElemK = arElemK.GetAt(i);
		/**/SCWBK.Key._ElemInfo.ElemD.MembType = iMBType;
		// For Static Load Case.
		iLcaseCount = arStldK.GetSize();
		for (j = 0; j < iLcaseCount; j++)
		{
			T_STLD_D StldD;
			if (!m_pDoc->m_pAttrCtrl->GetStld(arStldK.GetAt(j), StldD))    continue;
			// Only Earquake Load.
			if (!CDBLib::IsStldEL(StldD.LoadCaseType))                     continue;

			// Set Union Key.
			SCWBK.Key._LoadInfo.LoadD.LcaseK = arStldK.GetAt(j);
			SCWBK.Key._LoadInfo.LoadD.LcaseType = D_LCOM_STATIC;  // Static.

			// Get Member Force
			_DGN_SCWB_D SCWBDI, SCWBDJ;
			if (Get_SCWBBeamEndMoment(SCWBDI, SCWBDJ, ElemK, D_LOADCASE_STATIC, arStldK.GetAt(j)))
			{
				// I-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 0;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDI);
				Test_TextOutPutForSCWB(bTestPrint, strTestFile, SCWBK, SCWBDI); /*TEST*/
				// J-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 1;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDJ);
				Test_TextOutPutForSCWB(bTestPrint, strTestFile, SCWBK, SCWBDJ); /*TEST*/
			}
		}
		// For Spectrum Load Case.
		iLcaseCount = arSplcK.GetSize();
		for (j = 0; j < iLcaseCount; j++)
		{
			// Set Union Key.
			SCWBK.Key._LoadInfo.LoadD.LcaseK = arSplcK.GetAt(j);

			// Get Member Force
			_DGN_SCWB_D SCWBDI, SCWBDJ;
			if (Get_SCWBBeamEndMoment(SCWBDI, SCWBDJ, ElemK, D_LOADCASE_SPECTRUM, arSplcK.GetAt(j)))
			{
				SCWBK.Key._LoadInfo.LoadD.LcaseType = D_LCOM_SPECTRUM;  // Spectrum.

				// I-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 0;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDI);
				Test_TextOutPutForSCWB(bTestPrint, strTestFile, SCWBK, SCWBDI); /*TEST*/
				// J-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 1;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDJ);
				Test_TextOutPutForSCWB(bTestPrint, strTestFile, SCWBK, SCWBDJ); /*TEST*/
			}
			if (Get_SCWBBeamEndMoment(SCWBDI, SCWBDJ, ElemK, D_LOADCASE_ECCENSPEC, arSplcK.GetAt(j)))
			{
				SCWBK.Key._LoadInfo.LoadD.LcaseType = D_LCOM_ECCENSPEC; // 우발편심.

				// I-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 0;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDI);
				Test_TextOutPutForSCWB(bTestPrint, strTestFile, SCWBK, SCWBDI); /*TEST*/
				// J-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 1;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDJ);
				Test_TextOutPutForSCWB(bTestPrint, strTestFile, SCWBK, SCWBDJ); /*TEST*/
			}
		}
		// For SRSS Load Combination.
		for (j = 0; j < arSrssK.GetSize(); j++)
		{
			// Set Union Key.
			SCWBK.Key._LoadInfo.LoadD.LcaseK = arSrssK.GetAt(j);
			SCWBK.Key._LoadInfo.LoadD.LcaseType = D_LCOM_CB_CONCRETE; // 주의 - Analtype과 Lcasetype 주의할 것.

			// Get Member Force
			_DGN_SCWB_D SCWBDI, SCWBDJ;
			if (Get_SCWBBeamEndMoment(SCWBDI, SCWBDJ, ElemK, D_LOADCASE_COMB_CONCRETE, arSrssK.GetAt(j)))
			{
				// I-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 0;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDI);
				Test_TextOutPutForSCWB(bTestPrint, strTestFile, SCWBK, SCWBDI); /*TEST*/
				// J-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 1;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDJ);
				Test_TextOutPutForSCWB(bTestPrint, strTestFile, SCWBK, SCWBDJ); /*TEST*/
			}
		}
	}

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Set Data by Current Units.
	if (bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	//++++++++++++++++++++++++++++++++++++++++++++

	return TRUE;;
}

BOOL CDgnForceCtrl::Get_EQBendingMoment4IS()
{
	CCurUnitSaver Save(TRUE);

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bCngUnit = FALSE;
	if (CurIndex.nBase_Length != m_nCode_Length || CurIndex.nBase_Force != m_nCode_Force)
	{
		CngIndex.nBase_Length = m_nCode_Length;
		CngIndex.nBase_Force = m_nCode_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
		bCngUnit = TRUE;
	}
	//++++++++++++++++++++++++++++++++++++++++++++

	// Get Beam End Bending Strength. (Probable Strength)
	SCWBRatio SCWBRat;
	// Coded by Seungjun ('20070302) MNet:No.2086.
	// Get Flexural Design Strength.
	if (!SCWBRat.Get_GirderBM(m_arGirMnyCW, m_arGirMnyCCW, m_arGirMnzCW, m_arGirMnzCCW, EN_FORCE_MOMENT_DGN))
	{
		// <Remember> Set Data by Current Units.
		//if(bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
		//return FALSE;      
	}
	SCWBRat.Set_UseUpperAngle(TRUE);
	if (!SCWBRat.Get_GirderBM(m_arUpGirMnyCW, m_arUpGirMnyCCW, m_arUpGirMnzCW, m_arUpGirMnzCCW, EN_FORCE_MOMENT_DGN))
	{
		// <Remember> Set Data by Current Units.
		//if(bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
		//return FALSE;      
	}

	SCWBRat.Set_UseUpperAngle(FALSE);

	// Get Load Combination Key.
	CArray<T_LCOM_K, T_LCOM_K> arLcomK;
	arLcomK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arLcomK);
	if (arLcomK.GetSize() < 0) return FALSE;

	// Get Element Key List.
	CArray<T_ELEM_K, T_ELEM_K> arElemK;
	arElemK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemK);
	// Get Static Load Case Key List.
	CArray<T_STLD_K, T_STLD_K> arStldK;
	arStldK.RemoveAll();
	m_pDoc->m_pPostCtrl->GetStldKeyList(arStldK);
	// Get Spectrum Load Case Key List.
	CArray<T_SPLC_K, T_SPLC_K> arSplcK;
	arSplcK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(arSplcK);
	// Get SRSS Type Load Combination.
	CArray<unsigned int, unsigned int> arSrssK;
	arSrssK.RemoveAll();
	for (int k = 0; k < arLcomK.GetSize(); k++)
	{
		// Change by ZINU.('07.07.09). NO:2903, Distinguish Envelope from SRSS.
		unsigned int iLcaseKey = arLcomK.GetAt(k);
		// 주의 - SRSS의 경우 하중조합 Key를 하중조건 Key처럼 사용. 
		if (GetLoadCombType(iLcaseKey, D_LCOM_CB_CONCRETE) == 3)	arSrssK.Add(iLcaseKey);
	}

	// Get Bending moment of element.
	int i = 0, j = 0;
	int iElemCount = 0, iLcaseCount = 0;
	iElemCount = arElemK.GetSize();

	for (i = 0; i < iElemCount; i++)
	{
		// 제한사항
		T_ELEM_K ElemK = arElemK.GetAt(i);

		// Check the element.
		int iMBType = 0;
		if (!m_pDgnDataCtrl->IsRBeamOrColumn(ElemK, iMBType))  continue;

		// Set Union Key.
		_DGN_SCWB_K SCWBK;
		SCWBK.Key._ElemInfo.ElemD.ElemK = arElemK.GetAt(i);
		SCWBK.Key._ElemInfo.ElemD.MembType = iMBType;
		// For Static Load Case.
		iLcaseCount = arStldK.GetSize();
		for (j = 0; j < iLcaseCount; j++)
		{
			T_STLD_D StldD;
			if (!m_pDoc->m_pAttrCtrl->GetStld(arStldK.GetAt(j), StldD))    continue;

			// Set Union Key.
			SCWBK.Key._LoadInfo.LoadD.LcaseK = arStldK.GetAt(j);
			SCWBK.Key._LoadInfo.LoadD.LcaseType = D_LCOM_STATIC;  // Static.

			// Get Member Force
			_DGN_SCWB_D SCWBDI, SCWBDJ;
			if (Get_SCWBBeamEndMoment(SCWBDI, SCWBDJ, ElemK, D_LOADCASE_STATIC, arStldK.GetAt(j)))
			{
				// I-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 0;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDI);
				// J-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 1;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDJ);
			}
		}
		// For Spectrum Load Case.
		iLcaseCount = arSplcK.GetSize();
		for (j = 0; j < iLcaseCount; j++)
		{
			// Set Union Key.
			SCWBK.Key._LoadInfo.LoadD.LcaseK = arSplcK.GetAt(j);

			// Get Member Force
			_DGN_SCWB_D SCWBDI, SCWBDJ;
			if (Get_SCWBBeamEndMoment(SCWBDI, SCWBDJ, ElemK, D_LOADCASE_SPECTRUM, arSplcK.GetAt(j)))
			{
				SCWBK.Key._LoadInfo.LoadD.LcaseType = D_LCOM_SPECTRUM;  // Spectrum.

				// I-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 0;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDI);
				// J-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 1;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDJ);
			}
			if (Get_SCWBBeamEndMoment(SCWBDI, SCWBDJ, ElemK, D_LOADCASE_ECCENSPEC, arSplcK.GetAt(j)))
			{
				SCWBK.Key._LoadInfo.LoadD.LcaseType = D_LCOM_ECCENSPEC; // 우발편심.

				// I-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 0;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDI);
				// J-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 1;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDJ);
			}
		}
		// For SRSS Load Combination.
		for (j = 0; j < arSrssK.GetSize(); j++)
		{
			// Set Union Key.
			SCWBK.Key._LoadInfo.LoadD.LcaseK = arSrssK.GetAt(j);
			SCWBK.Key._LoadInfo.LoadD.LcaseType = D_LCOM_CB_CONCRETE; // 주의 - Analtype과 Lcasetype 주의할 것.

			// Get Member Force
			_DGN_SCWB_D SCWBDI, SCWBDJ;
			if (Get_SCWBBeamEndMoment(SCWBDI, SCWBDJ, ElemK, D_LOADCASE_COMB_CONCRETE, arSrssK.GetAt(j)))
			{
				// I-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 0;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDI);
				// J-End
				SCWBK.Key._ElemInfo.ElemD.PosiK = 1;
				m_arEqBM.SetAt(SCWBK.KeyMap, SCWBDJ);
			}
		}
	}

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Set Data by Current Units.
	if (bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	//++++++++++++++++++++++++++++++++++++++++++++

	return TRUE;;
}

BOOL CDgnForceCtrl::Get_GirderAsMap_ExistJointCheck(const T_DCON_D& DconD)
{
	CCurUnitSaver Save(TRUE);

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bCngUnit = FALSE;
	if (CurIndex.nBase_Length != m_nCode_Length || CurIndex.nBase_Force != m_nCode_Force)
	{
		CngIndex.nBase_Length = m_nCode_Length;
		CngIndex.nBase_Force = m_nCode_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
		bCngUnit = TRUE;
	}
	//++++++++++++++++++++++++++++++++++++++++++++

	// Get Beam End Bending Strength. (Probable Strength)
	SCWBRatio SCWBRat;
	if (!SCWBRat.Get_GirderAsMap_ByREBB(DconD, m_mBeamSumAsFyY, m_mBeamSumAsFyZ))
	{
		if (bCngUnit) m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
		return FALSE;
	}

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Set Data by Current Units.
	if (bCngUnit)	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	//++++++++++++++++++++++++++++++++++++++++++++

	return TRUE;
}

// Coded by Seungjun MNet:No.2591 ('20061214)	For SCWB Design.
void CDgnForceCtrl::Test_TextOutPutForSCWB(BOOL bPrint, CString strFileName, _DGN_SCWB_K& ScwbK, _DGN_SCWB_D& ScwbD)
{
	if (strFileName == _T("")) return;
	// for test
	if (bPrint)
	{
		CString strString = _T("");
		CString strElemK = _T(""), strLcasK = _T(""), strLcasT = _T(""), strPosiK = _T(""), strMBType = _T("");
		CString strNodeK = _T(""), strEQMy = _T(""), strEQMz = _T("");
		strElemK.Format(_T("-. Elememt       No. %d\n"), ScwbK.Key._ElemInfo.ElemD.ElemK);
		strPosiK.Format(_T("-. Position      No. %d\n"), ScwbK.Key._ElemInfo.ElemD.PosiK);
		strMBType.Format(_T("-. Member Type     . %d\n"), ScwbK.Key._ElemInfo.ElemD.MembType);
		strLcasT.Format(_T("-. Loadcase Type No. %d\n"), ScwbK.Key._LoadInfo.LoadD.LcaseType);
		strLcasK.Format(_T("-. Loadcase      No. %d\n"), ScwbK.Key._LoadInfo.LoadD.LcaseK);
		strNodeK.Format(_T("   Node No. %d\n"), ScwbD.NodeK);
		strEQMy.Format(_T("   My = %lf\n"), ScwbD.dMyEQ);
		strEQMz.Format(_T("   Mz = %lf\n\n"), ScwbD.dMzEQ);

		rptwofstream fout(strFileName, ios::app);
		fout << strElemK << strPosiK << strMBType << strLcasT << strLcasK << strNodeK << strEQMy << strEQMz << endl;
	}
}

// Coded by Seungjun MNet:No.2591 ('20061130)	For SCWB Design.
BOOL CDgnForceCtrl::Get_SCWBBeamEndMoment(_DGN_SCWB_D& ScwbDI, _DGN_SCWB_D& ScwbDJ, T_ELEM_K Elemk, int LcaseType, int LcaseKey)
{
	T_ELEM_D ElemD;
	BOOL bReverseDir = FALSE;
	T_ELEM_K ElemShowKey;
	CArray<T_ELEM_K, T_ELEM_K> arElemList; arElemList.RemoveAll();
	m_pDgnDataCtrl->m_pMembCtrl->GetElemListByIncludeElem(Elemk, bReverseDir, ElemShowKey, arElemList);

	if (!bReverseDir)  ElemShowKey = arElemList.GetAt(0);
	else              ElemShowKey = arElemList.GetAt(arElemList.GetSize() - 1);

	ElemD.Initialize();
	m_pDoc->m_pAttrCtrl->GetElem(ElemShowKey, ElemD);

	T_NODE_K aEdgeNode[2] = { 0, 0 };
	m_pDgnDataCtrl->m_pMembCtrl->GetEdgeNodeAtMemb(ElemShowKey, aEdgeNode);

	// I-End
	ScwbDI.Initialize();

	if (!bReverseDir)  ScwbDI.NodeK = aEdgeNode[0];
	else              ScwbDI.NodeK = aEdgeNode[1];

	T_STRB_D StrbMaxData, StrbMinData;
	StrbMaxData.Initialize();
	StrbMinData.Initialize();
	if (!m_pDgnDataCtrl->m_pMembCtrl->GetStrbDgn(ElemShowKey, LcaseType, LcaseKey, &StrbMaxData, &StrbMinData))	return FALSE;

	double dStrength[2] = { 0. };
	double dPu = 0.0, dPX = 0.0, dPY = 0.0, dPZ = 0.0;

	dPX = 0.0;
	if (!bReverseDir)
	{
		if (m_pDoc->m_pAttrCtrl->GetMemberType(Elemk) == D_MBTP_COLUMN)
		{
			dPu = StrbMaxData.dblForce[0][0];
			dPY = StrbMaxData.dblForce[0][4];
			dPZ = StrbMaxData.dblForce[0][5];
			Get_StrengthUCStoGCS(Elemk, dPu, dStrength, dPY, dPZ);
		}
		else
		{
			dPY = StrbMaxData.dblForce[0][4];
			Get_StrengthUCStoGCS(Elemk, dStrength, dPX, dPY, dPZ);
			// MQC:3546-Seungjun-20100720 보 부재력 summation 오류 ; 하중조합에 2개 이상의 지진하중이 있을 경우 / 절점 양단의 모멘트 부호가 동일할 경우.
			if (dPY < 0.0)
			{
				dStrength[0] = (-1.)*fabs(dStrength[0]);
				dStrength[1] = (-1.)*fabs(dStrength[1]);
			}
			else
			{
				dStrength[0] = fabs(dStrength[0]);
				dStrength[1] = fabs(dStrength[1]);
			}
		}
	}
	else
	{
		if (m_pDoc->m_pAttrCtrl->GetMemberType(Elemk) == D_MBTP_COLUMN)
		{
			dPu = StrbMaxData.dblForce[3][6];
			dPY = StrbMaxData.dblForce[3][10];
			dPZ = StrbMaxData.dblForce[3][11];
			Get_StrengthUCStoGCS(Elemk, dPu, dStrength, dPY, dPZ);
		}
		else
		{
			dPY = StrbMaxData.dblForce[3][10];
			Get_StrengthUCStoGCS(Elemk, dStrength, dPX, dPY, dPZ);
			// MQC:3546-Seungjun-20100720 보 부재력 summation 오류 ; 하중조합에 2개 이상의 지진하중이 있을 경우 / 절점 양단의 모멘트 부호가 동일할 경우.
			if (dPY < 0.0)
			{
				dStrength[0] = (-1.)*fabs(dStrength[0]);
				dStrength[1] = (-1.)*fabs(dStrength[1]);
			}
			else
			{
				dStrength[0] = fabs(dStrength[0]);
				dStrength[1] = fabs(dStrength[1]);
			}
		}
	}
	ScwbDI.dPu = dPu;
	ScwbDI.dMyEQ = dStrength[0];
	ScwbDI.dMzEQ = dStrength[1];

	// J-End
	if (arElemList.GetSize() > 1)
	{
		ElemD.Initialize();
		m_pDoc->m_pAttrCtrl->GetElem(ElemShowKey, ElemD);

		if (!bReverseDir)  ElemShowKey = arElemList.GetAt(arElemList.GetSize() - 1);
		else              ElemShowKey = arElemList.GetAt(0);

		StrbMaxData.Initialize();
		StrbMinData.Initialize();
		if (!m_pDgnDataCtrl->m_pMembCtrl->GetStrbDgn(ElemShowKey, LcaseType, LcaseKey, &StrbMaxData, &StrbMinData))	return FALSE;
	}

	ScwbDJ.Initialize();

	if (!bReverseDir)  ScwbDJ.NodeK = aEdgeNode[1];
	else              ScwbDJ.NodeK = aEdgeNode[0];

	dPX = 0.0;

	if (!bReverseDir)
	{
		if (m_pDoc->m_pAttrCtrl->GetMemberType(Elemk) == D_MBTP_COLUMN)
		{
			dPu = StrbMaxData.dblForce[3][6];
			dPY = StrbMaxData.dblForce[3][10];
			dPZ = StrbMaxData.dblForce[3][11];
			Get_StrengthUCStoGCS(Elemk, dPu, dStrength, dPY, dPZ);
		}
		else
		{
			dPY = StrbMaxData.dblForce[3][10];
			Get_StrengthUCStoGCS(Elemk, dStrength, dPX, dPY, dPZ);
			// MQC:3546-Seungjun-20100720 보 부재력 summation 오류 ; 하중조합에 2개 이상의 지진하중이 있을 경우 / 절점 양단의 모멘트 부호가 동일할 경우.
			if (dPY < 0.0)
			{
				dStrength[0] = (-1.)*fabs(dStrength[0]);
				dStrength[1] = (-1.)*fabs(dStrength[1]);
			}
			else
			{
				dStrength[0] = fabs(dStrength[0]);
				dStrength[1] = fabs(dStrength[1]);
			}
		}
	}
	else
	{
		if (m_pDoc->m_pAttrCtrl->GetMemberType(Elemk) == D_MBTP_COLUMN)
		{
			dPu = StrbMaxData.dblForce[0][0];
			dPY = StrbMaxData.dblForce[0][4];
			dPZ = StrbMaxData.dblForce[0][5];
			Get_StrengthUCStoGCS(Elemk, dPu, dStrength, dPY, dPZ);
		}
		else
		{
			dPY = StrbMaxData.dblForce[0][4];
			Get_StrengthUCStoGCS(Elemk, dStrength, dPX, dPY, dPZ);
			// MQC:3546-Seungjun-20100720 보 부재력 summation 오류 ; 하중조합에 2개 이상의 지진하중이 있을 경우 / 절점 양단의 모멘트 부호가 동일할 경우.
			if (dPY < 0.0)
			{
				dStrength[0] = (-1.)*fabs(dStrength[0]);
				dStrength[1] = (-1.)*fabs(dStrength[1]);
			}
			else
			{
				dStrength[0] = fabs(dStrength[0]);
				dStrength[1] = fabs(dStrength[1]);
			}
		}
	}
	ScwbDJ.dPu = dPu;
	ScwbDJ.dMyEQ = dStrength[0];
	ScwbDJ.dMzEQ = dStrength[1];

	return TRUE;
}

// Coded by Seungjun MNet:No.2591 ('20070416)	For SCWB Design.
// beta-angle에 따라서 분력을 계산해준다.
BOOL CDgnForceCtrl::Get_StrengthUCStoGCS(T_ELEM_K ElemK, double dPu, double dStrength[2], double dPX, double dPY)
{
	// 아래 함수 참조...
	// wg_db ; BOOL CStoryDisplDrift::GetShearStrength(T_ELEM_K ElemK, double dTol, double dStrength[2])
	dStrength[0] = dStrength[1] = 0.0;
	dPu = fabs(dPu) < cDgn_Zero ? 1.0 : dPu;

	// Element의 local vector 계산
	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }
	double dRotMat[2][2];
	double dTheta = (-1.)*ElemD.angle*CMathFunc::m_trrad;

	// Make Rotation Matrix.
	//┌                           ┐
	//│  cos(Theta)   sin(Theta)  │
	//│                           │
	//│ -sin(theta)   cos(Theta)  │
	//└                           ┘
	dRotMat[0][0] = cos(dTheta);
	dRotMat[0][1] = -sin(dTheta);
	dRotMat[1][0] = sin(dTheta);
	dRotMat[1][1] = cos(dTheta);

	// Calculate Eccentricity Length.
	double dey = dPX / dPu;
	double dez = dPY / dPu;
	/*
		double dei[2] = {0.};
		Get_StrengthUCStoGCS(ElemK, dei, 0.0, dez, dey);
	*/
	double deX = dRotMat[0][0] * dey + dRotMat[0][1] * dez;
	double deY = dRotMat[1][0] * dey + dRotMat[1][1] * dez;

	// Local to Global.
	dStrength[0] = deY * dPu;
	dStrength[1] = deX * dPu;

	return TRUE;
}

BOOL CDgnForceCtrl::Get_StrengthGCStoUCS(T_ELEM_K ElemK, double dPu, double dStrength[2], double dPX, double dPY, BOOL bABS/*FALSE*/)
{
	// 아래 함수 참조...
	// wg_db ; BOOL CStoryDisplDrift::GetShearStrength(T_ELEM_K ElemK, double dTol, double dStrength[2])
	dStrength[0] = dStrength[1] = 0.0;
	dPu = fabs(dPu) < cDgn_Zero ? 1.0 : dPu;

	// Element의 local vector 계산
	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }
	double dRotMat[2][2];
	double dTheta = ElemD.angle*CMathFunc::m_trrad;

	// Make Rotation Matrix.
	//┌                           ┐
	//│  cos(Theta)   sin(Theta)  │
	//│                           │
	//│ -sin(theta)   cos(Theta)  │
	//└                           ┘
	dRotMat[0][0] = cos(dTheta);
	dRotMat[0][1] = sin(dTheta);
	dRotMat[1][0] = -sin(dTheta);
	dRotMat[1][1] = cos(dTheta);

	// Calculate Eccentricity Length.
	double dey = dPX / dPu;
	double dez = dPY / dPu;

	double deX = 0.0;
	double deY = 0.0;

	if (bABS)
	{
		deX = fabs(dRotMat[0][0] * dey) + fabs(dRotMat[0][1] * dez);
		deY = fabs(dRotMat[1][0] * dey) + fabs(dRotMat[1][1] * dez);
	}
	else
	{
		deX = dRotMat[0][0] * dey + dRotMat[0][1] * dez;
		deY = dRotMat[1][0] * dey + dRotMat[1][1] * dez;
	}

	// Global to Local.
	dStrength[0] = deY * dPu;
	dStrength[1] = deX * dPu;

	return TRUE;
}

// Coded by Seungjun MNet:No.2591 ('20061130)	For SCWB Design.
// beta-angle에 따라서 분력을 계산해준다.
BOOL CDgnForceCtrl::Get_StrengthUCStoGCS(T_ELEM_K ElemK, double dStrength[2], double dPX, double dPY, double dPZ)
{
	// 아래 함수 참조...
	// wg_db ; BOOL CStoryDisplDrift::GetShearStrength(T_ELEM_K ElemK, double dTol, double dStrength[2])

	dStrength[0] = dStrength[1] = 0.0;

	// Element의 local vector 계산
	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }
	double dLocalVector[3][3];
	if (!m_pDoc->calcElemLocalVector(ElemD, dLocalVector)) { ASSERT(0); return FALSE; }

	// Local to Global.
	CMathFunc::mathUCS2GCS(dPX, dPY, dPZ, dLocalVector);
	//   dStrength[0] = fabs(dPY);
	//   dStrength[1] = fabs(dPZ);
	dStrength[0] = fabs(dPX);
	dStrength[1] = fabs(dPY);

	return TRUE;
}

BOOL CDgnForceCtrl::Get_StrengthGCStoUCS(T_ELEM_K ElemK, double dStrength[2], double dPX, double dPY, double dPZ)
{
	// 아래 함수 참조...
	// wg_db ; BOOL CStoryDisplDrift::GetShearStrength(T_ELEM_K ElemK, double dTol, double dStrength[2])

	dStrength[0] = dStrength[1] = 0.0;

	// Element의 local vector 계산
	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }
	double dLocalVector[3][3];
	if (!m_pDoc->calcElemLocalVector(ElemD, dLocalVector)) { ASSERT(0); return FALSE; }

	// Local to Global.
	CMathFunc::mathGCS2UCS(dPX, dPY, dPZ, dLocalVector);
	dStrength[0] = fabs(dPY);
	dStrength[1] = fabs(dPZ);

	return TRUE;
}

BOOL CDgnForceCtrl::IsValidLcom(const BOOL &b7DOF, const int &nLcomCompType)
{
	if (!b7DOF)
	{
		if (nLcomCompType == 7 || nLcomCompType == 8 || nLcomCompType == 9)
		{
			return FALSE;
		}
	}
	return TRUE;
}

CString CDgnForceCtrl::Make_LcomDescript(const T_LCOM_D_UL& LcomUL, OUT CArray<CString, CString&>& raLcasLcomName)
{
	raLcasLcomName.RemoveAll();

	CString strLcomDescript = _T("");

	const INT_PTR nSizeComb = LcomUL.Combination.GetSize();
	for (INT_PTR i = 0; i < nSizeComb; ++i)
	{
		const T_LCOM_BASE& LcomBase = LcomUL.Combination[i];

		const double dFact = LcomBase.Factor;
		CString strLcas = GetLcasLcomName(LcomBase.LoadCaseKey, LcomBase.AnalType);

		if (dgn::UQ0(dFact) && strLcas.GetLength() > 0)
		{
			CString strTmp = _T("");
			strTmp = strLcomDescript;

			//(2008.02.18) Add by Unsang :: Modify Lcom Print Style, By Asked msKim!!
			//strSign  = (LcomBase.Factor > 0.0 && strLcomDescript.GetLength() > 0)? _T(" + ") : _T("");
			const CString& strSign = strLcomDescript.GetLength() > 0 ? _T(" + ") : _T("");

			strLcomDescript.Format(_T("%s%s(%.3f)[%s]"), strTmp, strSign, dFact, strLcas);

			raLcasLcomName.Add(strLcas);
		}
	}
	return strLcomDescript;
}

CString CDgnForceCtrl::Make_LcomDescript(const T_LCOM_D& LcomD, OUT T_KEY_LIST& raDgnLcaseK)
{
	raDgnLcaseK.RemoveAll();

	CString strLcomDescript = _T("");

	const INT_PTR nSizeComb = LcomD.aCombination.GetSize();
	for (INT_PTR i = 0; i < nSizeComb; ++i)
	{
		const T_LCOM_BASE& LcomBase = LcomD.aCombination[i];
		if (LcomBase.LoadCaseKey == 0) break;

		const double dFact = LcomBase.Factor;
		const CString& strName = GetLcasLcomName(LcomBase.LoadCaseKey, LcomBase.AnalType);

		if (dgn::UQ0(dFact) && strName.GetLength() > 0)
		{
			CString strTmp = _T("");
			strTmp = strLcomDescript;
			//(2008.02.18) Add by Unsang :: Modify Lcom Print Style, By Asked msKim!!
			//strSign  = (LcomBase.Factor > 0.0 && strLcomDescript.GetLength() > 0)? _T(" + ") : _T("");            
			const CString& strSign = strLcomDescript.GetLength() > 0 ? _T(" + ") : _T("");

			strLcomDescript.Format(_T("%s%s(%.3f)[%s]"), strTmp, strSign, dFact, strName);

			_DGN_LCASE_K DgnLcaseK;  // AnalType + LcaseKey의 조합 Key.
			DgnLcaseK.Key.AnalType = LcomBase.AnalType;
			DgnLcaseK.Key.LcaseKey = LcomBase.LoadCaseKey;
			raDgnLcaseK.Add(DgnLcaseK.KeyMap);
		}
	}
	return strLcomDescript;
}

BOOL CDgnForceCtrl::Get_LcaseNameDescript(UINT DgnLcasK, CString& strLcasName, CString& strLcasDesc)
{
	strLcasName = _T("");
	strLcasDesc = _T("");

	_DGN_LCASE_K DgnK;
	DgnK.KeyMap = DgnLcasK;
	unsigned int iAnalType = DgnK.Key.AnalType;
	unsigned int iLcaseKey = DgnK.Key.LcaseKey;

	switch (iAnalType)
	{
	case D_LCOM_STATIC:
	{
		T_STLD_D StldD;
		if (!m_pDoc->m_pAttrCtrl->GetStld(iLcaseKey, StldD)) break;
		strLcasName = StldD.LoadCaseName;
		strLcasDesc = StldD.Description;
		break;
	}
	case D_LCOM_SPECTRUM:
	{
		T_SPLC_D SplcD;
		if (!m_pDoc->m_pAttrCtrl->GetSplc(iLcaseKey, SplcD)) break;
		strLcasName = SplcD.LoadCaseName/* + _T("(RS)")*/;
		strLcasDesc = SplcD.Description;
		break;
	}
	case D_LCOM_ECCENSPEC:
	{
		T_SPLC_D SplcD;
		if (!m_pDoc->m_pAttrCtrl->GetSplc(iLcaseKey, SplcD)) break;
		strLcasName = SplcD.LoadCaseName/* + _T("(ES)")*/;
		strLcasDesc = SplcD.Description;
		break;
	}
	case D_LCOM_HISTORY:
	{
		T_THIS_D ThisD;
		if (!m_pDoc->m_pAttrCtrl->GetThis(iLcaseKey, ThisD)) break;
		strLcasName = ThisD.LoadCaseName;
		strLcasDesc = ThisD.Description;
		break;
	}
	case D_LCOM_CRANE:
	{
		T_CRAN_D CranD;
		if (!m_pDoc->m_pAttrCtrl->GetCran(iLcaseKey, CranD)) break;
		strLcasName = CranD.LoadTypeName;
		strLcasDesc = _T(" "); // Blank Type!! (Not exist description)
		break;
	}
	case D_LCOM_MOVING:
	{
		T_MVCD_D MvcdD;
		if (!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) break;
		switch (MvcdD.nCodeType)
		{
		case D_MOVE_CODE_NONE:
			break;
		case D_MOVE_CODE_KOREA:
		case D_MOVE_CODE_AASHTO_STAN:
		case D_MOVE_CODE_AASHTO_LRFD:
		case D_MOVE_CODE_TAIWAN:
		case D_MOVE_CODE_CANADA:
		case D_MOVE_CODE_PENDOT:
		case D_MOVE_CODE_EURO_BS:
		case D_MOVE_CODE_RUSSIA:
		case D_MOVE_CODE_KOREA_LRFD_2011:
		case D_MOVE_CODE_AUSTRALIA:
		case D_MOVE_CODE_POLAND:
		case D_MOVE_CODE_SOUTH_AFRICA:
		case D_MOVE_CODE_NEWZEALAND:
		case D_MOVE_CODE_BRAZIL:
		{
			T_MVLD_D MvldD;
			if (!m_pDoc->m_pAttrCtrl->GetMvld(iLcaseKey, MvldD)) break;
			strLcasName = MvldD.LoadCaseName;
			strLcasDesc = MvldD.Description;

		}
		break;
		case D_MOVE_CODE_JAPAN:
		{
			T_MVLDjp_D MvldJPD;
			m_pDoc->m_pAttrCtrl->InitializeMvldjp(MvldJPD);
			if (!m_pDoc->m_pAttrCtrl->GetMvldjp(iLcaseKey, MvldJPD)) break;
			strLcasName = MvldJPD.LoadCaseName;
			strLcasDesc = MvldJPD.Description;
		}
		break;
		case D_MOVE_CODE_CHINA:
		{
			T_MVLDch_D MvldCHD;
			if (!m_pDoc->m_pAttrCtrl->GetMvldch(iLcaseKey, MvldCHD)) break;
			strLcasName = MvldCHD.LoadCaseName;
			strLcasDesc = MvldCHD.Description;
		}
		break;
		case D_MOVE_CODE_INDIA:
		{
			T_MVLDid_D MvldIDD;
			if (!m_pDoc->m_pAttrCtrl->GetMvldid(iLcaseKey, MvldIDD)) break;
			strLcasName = MvldIDD.LoadCaseName;
			strLcasDesc = MvldIDD.Description;
		}
		break;
		case D_MOVE_CODE_BS:
		{
			T_MVLDbs_D MvldBSD;
			if (!m_pDoc->m_pAttrCtrl->GetMvldbs(iLcaseKey, MvldBSD)) break;
			strLcasName = MvldBSD.LoadCaseName;
			strLcasDesc = MvldBSD.Description;
		}
		break;
		case D_MOVE_CODE_FRANCE:
		{
			T_MVLDfr_D MvldfrD;
			if (!m_pDoc->m_pAttrCtrl->GetMvldfr(iLcaseKey, MvldfrD)) break;
			strLcasName = MvldfrD.LoadCaseName;
			strLcasDesc = MvldfrD.Description;
		}
		break;
		case D_MOVE_CODE_TRANS:
		{
			T_MVLDtr_D MvldTRD;
			if (!m_pDoc->m_pAttrCtrl->GetMvldtr(iLcaseKey, MvldTRD)) break;
			strLcasName = MvldTRD.LoadCaseName;
			strLcasDesc = MvldTRD.Description;
		}
		break;
		default:
			ASSERT(0); break;
		}
		break;
	}
	case D_LCOM_SETTLEMENT:
	{
		T_SMLC_D SmlcD;
		if (!m_pDoc->m_pAttrCtrl->GetSmlc(iLcaseKey, SmlcD)) break;
		strLcasName = SmlcD.LoadCaseName;
		strLcasDesc = SmlcD.Description;
		break;
	}
	case D_LCOM_STAGE:
	{
		T_SGLD_D SgldD;
		if (!m_pDoc->m_pAttrCtrl->GetSgld(iLcaseKey, SgldD)) break;
		strLcasName = SgldD.LoadCaseName;
		strLcasDesc = SgldD.Description;
		break;
	}
	case D_LCOM_CB_STEEL:
	{
		T_LCOM_D LcomD;
		if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, iLcaseKey, LcomD)) break;
		strLcasName = LcomD.LoadCombName;
		strLcasDesc = LcomD.Description;
		break;
	}
	case D_LCOM_CB_CFSTEEL:
	{
		T_LCOM_D LcomD;
		if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CFSTEEL, iLcaseKey, LcomD)) break;
		strLcasName = LcomD.LoadCombName;
		strLcasDesc = LcomD.Description;
		break;
	}
	case D_LCOM_CB_CONCRETE:
	{
		T_LCOM_D LcomD;
		if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, iLcaseKey, LcomD)) break;
		strLcasName = LcomD.LoadCombName;
		strLcasDesc = LcomD.Description;
		break;
	}
	case D_LCOM_CB_SRC:
	{
		T_LCOM_D LcomD;
		if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SRC, iLcaseKey, LcomD)) break;
		strLcasName = LcomD.LoadCombName;
		strLcasDesc = LcomD.Description;
		break;
	}
	case D_LCOM_CB_ALUMINUM:
	{
		T_LCOM_D LcomD;
		if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_ALUMINUM, iLcaseKey, LcomD)) break;
		strLcasName = LcomD.LoadCombName;
		strLcasDesc = LcomD.Description;
		break;
	}
	case D_LCOM_CB_STLCOMP:
	{
		T_LCOM_D LcomD;
		if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, iLcaseKey, LcomD)) break;
		strLcasName = LcomD.LoadCombName;
		strLcasDesc = LcomD.Description;
		break;
	}
	}
	return TRUE;
}

int CDgnForceCtrl::GetLoadCombType(T_LCOM_K LcomK, int nAnalType)
{
	// Add by ZINU.('07.07.09). NO:2903, Distinguish Envelope from SRSS.
	// return 0=Add(Default), 1=Envelope, 2=ABS, 3=SRSS.
	const int nLcomType = m_pDoc->m_pAttrCtrl2->GetLcomTypeByAnalType(nAnalType);

	T_LCOM_D LcomD;
	if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, LcomK, LcomD)) { ASSERT(0); return 0; }
	return LcomD.LoadCombType;
}

void CDgnForceCtrl::SetEval1stLcomMap(_DGN_LCOM& LcomDesign, OUT int& rnTotl, OUT int& rnStrn, OUT int& rnVert)
{
	const int nActive = LcomDesign.LcomUlData.nActive;

	if (nActive > EN_EVAL_NONE)
	{
		rnTotl++;
		LcomDesign.DesignLcomKey = rnTotl;
		m_arLcomTotl.SetAt(rnTotl, LcomDesign);
	}

	switch (nActive)
	{
	case EN_EVAL_GENE:
	{
		rnStrn++;
		LcomDesign.DesignLcomKey = rnStrn;
		LcomDesign.LcomUlData.nActive = EN_DGN_STRN;
		m_arLcomStrn.SetAt(rnStrn, LcomDesign);

		m_arLcomStrnUG.SetAt(rnStrn, LcomDesign);
	}
	break;
	case EN_EVAL_VERT:
	{
		rnVert++;
		LcomDesign.DesignLcomKey = rnVert;
		LcomDesign.LcomUlData.nActive = EN_DGN_VERT;
		m_arLcomVert.SetAt(rnVert, LcomDesign);
	}
	break;
	default:
		ASSERT(0);
		break;
	}
}

void CDgnForceCtrl::SetEvalLinLcomMap(_DGN_LCOM& LcomDesign, UINT unType, OUT int& rnTotl, OUT int& rnStrn, OUT int& rnSpec, OUT int& rnVert)
{
	const int nActive = LcomDesign.LcomUlData.nActive;
	if (nActive > EN_LNEVAL_NONE)
	{
		rnTotl++;
		LcomDesign.DesignLcomKey = rnTotl;
		m_arLcomTotl.SetAt(rnTotl, LcomDesign);
	}

	if (unType == 0 && nActive == EN_LNEVAL_GRAV)
	{
		rnStrn++;
		LcomDesign.DesignLcomKey = rnStrn;
		LcomDesign.LcomUlData.nActive = EN_DGN_STRN;
		m_arLcomStrn.SetAt(rnStrn, LcomDesign);

		m_arLcomStrnUG.SetAt(rnStrn, LcomDesign);
	}

	if (unType == 1)
	{
		switch (nActive)
		{
		case EN_LNEVAL_SEIS:
		{
			rnStrn++;
			LcomDesign.DesignLcomKey = rnStrn;
			LcomDesign.LcomUlData.nActive = EN_DGN_STRN;
			m_arLcomStrn.SetAt(rnStrn, LcomDesign);

			m_arLcomStrnUG.SetAt(rnStrn, LcomDesign);
		}
		break;
		case EN_LNEVAL_SPEC:
		{
			rnSpec++;
			LcomDesign.DesignLcomKey = rnSpec;
			LcomDesign.LcomUlData.nActive = EN_DGN_SPEC;
			m_arLcomSpec.SetAt(rnSpec, LcomDesign);

			m_arLcomSpecUG.SetAt(rnStrn, LcomDesign);
		}
		break;
		case EN_LNEVAL_VERT:
		{
			rnVert++;
			LcomDesign.DesignLcomKey = rnVert;
			LcomDesign.LcomUlData.nActive = EN_DGN_VERT;
			m_arLcomVert.SetAt(rnVert, LcomDesign);
		}
		break;
		default:
			ASSERT(0);
			break;
		}
	}
}

void CDgnForceCtrl::SetDgnLcomMap(_DGN_LCOM& LcomDesign, BOOL bIncludeLcom, OUT int ranSizeLcom[])
{
	const int nActive = LcomDesign.LcomUlData.nActive;
	if (nActive > EN_DGN_NONE)
	{
		int& rnTotl = ranSizeLcom[EN_DGNLOM_IDX_TOTL];
		rnTotl += 1;
		LcomDesign.DesignLcomKey = rnTotl;
		m_arLcomTotl.SetAt(rnTotl, LcomDesign);
	}

	switch (nActive)
	{
	case EN_DGN_STRN:
	case EN_DGN_SERV:
	case EN_DGN_STRE:
	{
		int& rnStrn = ranSizeLcom[EN_DGNLOM_IDX_STRN];
		rnStrn += 1;
		LcomDesign.DesignLcomKey = rnStrn;
		m_arLcomStrn.SetAt(rnStrn, LcomDesign);
	}
	break;
	case EN_DGN_UG_STRN:
	case EN_DGN_UG_SERV:
	{
		int& rnStrn = ranSizeLcom[EN_DGNLOM_IDX_STRN_UG];
		rnStrn += 1;
		LcomDesign.DesignLcomKey = rnStrn;
		m_arLcomStrnUG.SetAt(rnStrn, LcomDesign);
	}
	break;
	default:
		break;
	}

	if (nActive == EN_DGN_SERV)
	{
		ranSizeLcom[EN_DGNLOM_IDX_SERV] += 1;
	}

	if (nActive == EN_DGN_UG_SERV)
	{
		ranSizeLcom[EN_DGNLOM_IDX_SERV_UG] += 1;
	}

	if (bIncludeLcom)
	{
		switch (nActive)
		{
		case EN_DGN_SPEC:
		case EN_DGN_SERV:
		{
			int& rnSpec = ranSizeLcom[EN_DGNLOM_IDX_SPEC];
			rnSpec += 1;
			LcomDesign.DesignLcomKey = rnSpec;
			m_arLcomSpec.SetAt(rnSpec, LcomDesign);
		}
		break;
		case EN_DGN_UG_SPEC:
		case EN_DGN_UG_SERV:
		{
			int& rnSpec = ranSizeLcom[EN_DGNLOM_IDX_SPEC_UG];
			rnSpec += 1;
			LcomDesign.DesignLcomKey = rnSpec;
			m_arLcomSpecUG.SetAt(rnSpec, LcomDesign);
		}
		break;
		default:
			break;
		}

		switch (nActive)
		{
		case EN_DGN_VERT:
		case EN_DGN_SERV:
		{
			int& rnVert = ranSizeLcom[EN_DGNLOM_IDX_VERT];
			rnVert += 1;
			LcomDesign.DesignLcomKey = rnVert;
			m_arLcomVert.SetAt(rnVert, LcomDesign);
		}
		break;
		default:
			break;
		}
	}
}

bool CDgnForceCtrl::IsExistLcomULS(bool bExcludeSeis)
{
	if (m_parLcomData == nullptr) { return false; }

	auto L_IsULS = [](const int nActive)->bool
	{
		switch (nActive)
		{
		case EN_DGN_STRN:
		case EN_DGN_SPEC:
		case EN_DGN_VERT:
		case EN_DGN_UG_STRN:
		case EN_DGN_UG_SPEC:
			return true;
		case EN_DGN_NONE:
		case EN_DGN_SERV:
		case EN_DGN_UG_SERV:
		case EN_DGN_STRE:
			return false;
		default:
			ASSERT(0);
			return false;
		}
	};

	auto L_IsValidLcom = [bExcludeSeis](const _DGN_LCOM& DgnLcom)
	{
		if (!bExcludeSeis) { return true; }

		switch (DgnLcom.iSeismicType)
		{
		case EN_DGN_SEIS_XYZ:
		case EN_DGN_SEIS_Z:
			return false;
		case EN_DGN_NONE:
			return true;
		default:
			ASSERT(0);
			return true;
		}
	};

	const INT_PTR nLcom = m_parLcomData->GetCount();
	for (INT_PTR i = 0; i < nLcom; ++i)
	{
		_DGN_LCOM LcomDesign;
		if (!m_parLcomData->Lookup(i + 1, LcomDesign)) { continue; }
		if (!L_IsULS(LcomDesign.LcomUlData.nActive)) { continue; }
		if (L_IsValidLcom(LcomDesign))
		{
			return true;
		}
	}
	return false;
}

bool CDgnForceCtrl::IsExistLcomELS()
{
	if (m_parLcomData == nullptr) { return false; }

	auto L_IsELS = [](const int nActive)->bool
	{
		switch (nActive)
		{
		case EN_DGN_NONE:
		case EN_DGN_STRN:
		case EN_DGN_SPEC:
		case EN_DGN_VERT:
		case EN_DGN_UG_STRN:
		case EN_DGN_UG_SPEC:
		case EN_DGN_SERV:
		case EN_DGN_UG_SERV:
			return false;
		case EN_DGN_STRE:
			return true;
		default:
			ASSERT(0);
			return false;
		}
	};

	const INT_PTR nLcom = m_parLcomData->GetCount();
	for (INT_PTR i = 0; i < nLcom; ++i)
	{
		_DGN_LCOM LcomDesign;
		if (!m_parLcomData->Lookup(i + 1, LcomDesign)) { continue; }

		if (L_IsELS(LcomDesign.LcomUlData.nActive))
		{
			return true;
		}
	}
	return false;
}

// PMS:3956-Seungjun-20090224 EC8:04 Seismic Design
double CDgnForceCtrl::Get_ElasticResponseSpectrum(int iSeismicType, double dPeriod)
{
	// Get Eigen-value Period at 1st-Mode.
	if (m_dEigenPeriod == 0.0)
	{
		T_FREQ_D FreqD;
		if (!m_pDoc->m_pPostCtrl->GetEiFreq(1, FreqD)) { FreqD.Initialize(); }
		m_dEigenPeriod = FreqD.dblFreq[2];
	}

	double dGravity = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KG, D_UNITSYS_LENGTH_INDEX_M, CUnitCtrl::m_STYP_UNIT.dblGravity, 9.8);

	BOOL bOK = FALSE;
	double dSe = 0.0;
	if (m_SpfcK != 0)
	{
		T_SPFC_D SpfcD; SpfcD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetSpfc(m_SpfcK, SpfcD))
		{
			if (SpfcD.nSpecCodeType == D_SPFC_CODE_USER)
			{
				int nValue = SpfcD.arSpecFuncData.GetSize();
				if (nValue > 1)
				{
					for (int i = 0; i < (nValue - 1); i++)
					{
						T_SPFC_BASE BaseSpecD1 = SpfcD.arSpecFuncData[i];
						T_SPFC_BASE BaseSpecD2 = SpfcD.arSpecFuncData[i + 1];

						if (BaseSpecD1.dblPeriod <= dPeriod && dPeriod <= BaseSpecD2.dblPeriod)
						{
							dSe = fabs(BaseSpecD1.dblValue + (BaseSpecD2.dblValue - BaseSpecD1.dblValue)*(dPeriod - BaseSpecD1.dblPeriod) / (BaseSpecD2.dblPeriod - BaseSpecD1.dblPeriod));
							bOK = TRUE;
							break;
						}
						if (i == (nValue - 2))
						{
							dSe = BaseSpecD2.dblValue;
							bOK = TRUE;
						}
					}
					if (bOK) return (dSe*dGravity);
				}
			}
		}
	}

	double dag = m_dGamma_I * m_dAgR*dGravity;
	double dEta = sqrt(10. / (5. + m_dDampingRat));
	dEta = max(dEta, 0.55);

	//m_dEigenPeriod
	if (dPeriod < m_dTB + cDgn_Zero)
	{
		if (iSeismicType == 1) // XY
		{
			dSe = dag * m_dSoilF*(1.0 + (dPeriod / m_dTB)*(2.5*dEta - 1.0));
		}
		else if (iSeismicType == 2) // Z
		{
			dSe = dag * (1.0 + (dPeriod / m_dTB)*(3.0*dEta - 1.0));
		}
	}
	else if (dPeriod < m_dTC + cDgn_Zero)
	{
		if (iSeismicType == 1) // XY
		{
			dSe = dag * m_dSoilF*2.5*dEta;
		}
		else if (iSeismicType == 2) // Z
		{
			dSe = dag * 3.0*dEta;
		}
	}
	else if (dPeriod < m_dTD + cDgn_Zero)
	{
		if (iSeismicType == 1) // XY
		{
			dSe = dag * m_dSoilF*2.5*dEta*(m_dTC / dPeriod);
		}
		else if (iSeismicType == 2) // Z
		{
			dSe = dag * 3.0*dEta*(m_dTC / dPeriod);
		}
	}
	else if (m_dEigenPeriod < 4.0 + cDgn_Zero)
	{
		if (iSeismicType == 1) // XY
		{
			dSe = dag * m_dSoilF*2.5*dEta*(m_dTC*m_dTD / dPeriod / dPeriod);
		}
		else if (iSeismicType == 2) // Z
		{
			dSe = dag * 3.0*dEta*(m_dTC*m_dTD / dPeriod / dPeriod);
		}
	}

	return dSe;
}

double CDgnForceCtrl::Get_BehaviourFactor(int iStrucType, int iDgnClass, double dAlphau_1, BOOL bNTC, int nNTCType)
{
	if (!m_bAutoBehaviorFact) { return m_dBehaviorFact_qo; }
	// by GAY. MQC:7831. NTC2012 처리 부족 보완. 
	const bool bCalcDCM = [bNTC, iDgnClass, nNTCType]() -> bool
	{
		if (bNTC)
		{
			if (iDgnClass == EN_EC8_DCH) return true;
			if (nNTCType == RCS_NTC12)  return true;
			if (nNTCType == RCS_NTC18)  return true;
			return false; // 2012는 CD_T("A")로 적용. ///## SWE, SGP 확인필요.
		}
		else
		{
			return (iDgnClass == EN_EC8_DCM) ? true : false;
		}
	}();

	dAlphau_1 = min(dAlphau_1, 1.5);

	//if(!m_bBehaviorFact) return m_dBehaviorFactor_q; // User defined behaviour factor.
	//(Tel. 4022) MNET:XXXX-BSCHOI-20121030 #5    
	/// see [BS EN 1998-1:2004, 5.2.2.2 (2), Table 5.1]
	switch (iStrucType)
	{
	case EN_EC8_FRAME_SYSTEM:
	case EN_EC8_DUAL_SYSTEM:
	case EN_EC8_COUPLED_WALL_SYSTEM:
		return bCalcDCM ? 3.0*dAlphau_1 : 4.5*dAlphau_1;
	case EN_EC8_UNCOUPLED_WALL_SYSTEM:
		return bCalcDCM ? 3.0 : 4.0*dAlphau_1;
	case EN_EC8_TORSIONAL_FLEX_SYSYEM:
		return bCalcDCM ? 2.0 : 3.0;
	case EN_EC8_INVERTED_PENDULUM_SYSTEM:
		return bCalcDCM ? 1.5 : 2.0;
	default:
		ASSERT(0);
		return 1.0;
	}
}

double CDgnForceCtrl::Get_PrevailingFactor(int iStrucType)
{
	/// see [BS EN 1998-1:2004, 5.2.2.2 (11)]
	switch (iStrucType)
	{
	case EN_EC8_FRAME_SYSTEM:
	case EN_EC8_DUAL_SYSTEM:
	{
		return 1.0;
	}
	case EN_EC8_COUPLED_WALL_SYSTEM:
	case EN_EC8_UNCOUPLED_WALL_SYSTEM:
	case EN_EC8_TORSIONAL_FLEX_SYSYEM:
	case EN_EC8_INVERTED_PENDULUM_SYSTEM:
	{
		// Modify by GAY. PMS:3787. ('09.12.06). 전체 System 내의 모든 벽체에 대해서 계산.
		const int iNumWall = m_pDoc->m_pPostCtrl->GetCountInternalWallId();

		double dSumhwi = 0.0;
		double dSumlwi = 0.0;
		for (int i = 0; i < iNumWall; i++)
		{
			T_WALL_KEY WallK;
			if (!m_pDoc->m_pPostCtrl->GetWallKey(i + 1, WallK))    continue;
			T_WALL_D WallD;
			if (!m_pDoc->m_pPostCtrl->GetWallData(WallK, WallD)) continue;

			dSumhwi += WallD.dHeight;
			dSumlwi += WallD.dLength;
		}

		const double dalpha_0 = dgn::SafeDiv(dSumhwi, dSumlwi);
		const double dKw = min((1.0 + dalpha_0) / 3.0, 1.0);
		return max(dKw, 0.5);
	}
	default:
	{
		ASSERT(0);
		return 1.0;
	}
	}
}

void CDgnForceCtrl::Get_WallMomBySpecialProv(int iElem, _DGN_LCOM& LcomDesign, double& dMuy, double& dMuz)
{
	T_DCON_D DconD;
	m_pDgnDataCtrl->Get_DgnConDcon(DconD);

	switch (DconD.nWallDgnEnvelop)
	{
	case EN_ENVELOP_METHOD_MAX:
		Get_WallMomBySpecialProv_UseSlopeMax(iElem, LcomDesign, dMuy, dMuz);
		break;
	case EN_ENVELOP_METHOD_PRE:
		Get_WallMomBySpecialProv_UseSlopePrevious(iElem, LcomDesign, dMuy, dMuz);
		break;
	default:
		ASSERT(0);
		break;
	}
}

void CDgnForceCtrl::Get_DGN_WALL_D_Map(const CArray<UINT, UINT>& aIntWallID, OUT CMapEx<int, int, _DGN_WALL_D, _DGN_WALL_D&>& rmDgnWallD,
	OUT int& rnBotElemK, OUT int& rnTopElemK, OUT double& rdBotZ, OUT double& rdTopZ)
{
	rmDgnWallD.RemoveAll();
	rnBotElemK = 0;
	rnTopElemK = 0;
	rdBotZ = 0.0;
	rdTopZ = 0.0;

	const INT_PTR nSize = aIntWallID.GetSize();
	if (nSize < 1) return;

	rmDgnWallD.InitHashTable(GF_GetHashSizeByCount(nSize));

	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;
	CPostCtrl* pPostCtrl = m_pDoc->m_pPostCtrl;

	T_ELEM_D ElemD;
	T_NODE_D NodeD_Top;
	T_NODE_D NodeD_Bot;
	T_ELEM_K_LIST aElemK;
	T_WALL_K InWallK;

	BOOL bFirst = TRUE;
	for (INT_PTR i = 0; i < nSize; ++i)
	{
		const UINT unCurID = aIntWallID[i];
		if (IsUnderGroundMemb(unCurID)) continue;      // PMS:tttt-Seungjun-20100310  // check underground.// not apply on 780.
		//if(!IsEqSpecialElem(arInternalWallIdList[i]))  continue;  // do not consider. agree with 이혜연DR
		if (!pPostCtrl->GetWallKey(unCurID, InWallK.keymap)) continue;
		if (!pPostCtrl->GetWallElemList(InWallK.keymap, aElemK)) continue;

		const T_ELEM_K RepElemK = aElemK[0]; /// 좌표를 딸 거라 첫번째 요소를 대표요소로 고려하여 확인하면 될거다.
		if (!pAttrCtrl->GetElem(RepElemK, ElemD)) continue;
		if (!pAttrCtrl->GetNode(ElemD.elnod[0], NodeD_Bot)) continue;
		if (!pAttrCtrl->GetNode(ElemD.elnod[3], NodeD_Top)) continue;

		_DGN_WALL_D WallD;
		WallD.Initialize();
		WallD.InternalWallId = unCurID;
		WallD.arElemList.Copy(aElemK);
		WallD.BotNodeK = ElemD.elnod[0];
		WallD.TopNodeK = ElemD.elnod[3];
		rmDgnWallD.SetAt(unCurID, WallD);

		if (bFirst)
		{
			rdBotZ = NodeD_Bot.z;
			rdTopZ = NodeD_Top.z;
			bFirst = FALSE;
		}
		if (NodeD_Bot.z < rdBotZ + cDgn_Zero)
		{
			rnBotElemK = RepElemK;
			rdBotZ = NodeD_Bot.z;
		}
		if (NodeD_Top.z > rdTopZ - cDgn_Zero)
		{
			rnTopElemK = RepElemK;
			rdTopZ = NodeD_Top.z;
		}
	}
}

// PMS:3956-Seungjun-20090309 EC8:04 Seismic Design
void CDgnForceCtrl::Get_WallMomBySpecialProv_UseSlopeMax(int iElem, _DGN_LCOM& LcomDesign, double& dMuy, double& dMuz)
{
	dMuy = m_BMy[m_iPosiNo];
	dMuz = m_BMz[m_iPosiNo];

	// Only EC2:04 Special Provision.
	if (!IsCalcWallDesignForce(iElem, LcomDesign)) { return; }

	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = m_nCode_Length;
	CngIndex.nBase_Force = m_nCode_Force;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	// <Remember> Data를 받아올때는 Code Unit기준
	//////////////////////////////////////////////

	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;
	CPostCtrl* pPostCtrl = m_pDoc->m_pPostCtrl;

	// Get User Wall ID and Wall Data.
	T_WALL_K WallK;
	if (!m_pDoc->m_pPostCtrl->GetWallKey(iElem, WallK.keymap)) { return; }

	//void CDgnForceCtrl::Set_BoundaryElemCondition(int iElemNo)
	// Get Internal Wall-Id List at Wall ID.
	CArray<UINT, UINT> aInternalWallID;
	if (!pPostCtrl->GetInternalWallIdList(WallK.key.wallid, aInternalWallID)) return;
	if (aInternalWallID.GetSize() == 0) return;

	_DGN_WDLF_K FuncKeyMap;
	FuncKeyMap.Key.wallid = WallK.key.wallid;
	FuncKeyMap.Key.Lcomk = LcomDesign.DesignLcomKey;

	_DGN_WDLF_D FuncD;
	_DGN_WDLF_D FuncDy;
	_DGN_WDLF_D FuncDz;

	double dBotZ = 0.0;
	double dTopZ = 0.0;
	if (!m_arWallMyFunc.Lookup(FuncKeyMap.KeyMap, FuncDy))
	{
		// Get Geometry information
		int iBotElemK = 0;
		int iTopElemK = 0;
		CMapEx<int, int, _DGN_WALL_D, _DGN_WALL_D&> mDgnWall;
		Get_DGN_WALL_D_Map(aInternalWallID, mDgnWall, iBotElemK, iTopElemK, dBotZ, dTopZ);
		const double dTotalHeight = fabs(dTopZ - dBotZ);

		// Get Internal-Wall ID & Elem Data.
		UINT iBotElemNo = 0;
		if (!pPostCtrl->GetInternalWallId(iBotElemK, iBotElemNo)) { ASSERT(0); return; }
		UINT iTopElemNo = 0;
		if (!pPostCtrl->GetInternalWallId(iTopElemK, iTopElemNo)) { ASSERT(0); return; }
		// Get KeyWall.
		T_WALL_K InWallK;
		if (!pPostCtrl->GetWallKey(iBotElemNo, InWallK.keymap)) { ASSERT(0); return; }
		// Get Length.
		T_WALL_D InWallD;
		if (!pPostCtrl->GetWallData(InWallK.keymap, InWallD)) { ASSERT(0); return; }

		const double dLw = InWallD.dLength;
		if (m_iDgnCode == Eurocode2_04)
		{
			if (!IsSlenderWallByEC8(dTotalHeight, dLw)) { return; }
		}

		// Wall/Dual system 동일하게 처리.
		int iPosiNo = m_iPosiNo;
		int iPosiNoV = m_iPosiNoV;
		Set_ForceMomentBySeperate(iBotElemNo, 0, LcomDesign);
		double dBotMuy = m_BMy[0];
		double dBotMuz = m_BMz[0];
		double dBotVuz = m_Fzz[0];
		if (iBotElemNo != iTopElemNo)
		{
			Set_ForceMomentBySeperate(iTopElemNo, 4, LcomDesign);
		}

		double dTopMuy = m_BMy[4];
		double dTopMuz = m_BMz[4];
		double dTopVuz = m_Fzz[4];

		// EC8:04 5.4.3.4.2 hcr=a1
		auto l_Calc_a1 = [this, &dLw](const double& dMu, const double& dVu, const double& dWallHeight, const double& dCurWallHeight)
		{
			double dhcr = 0.0;
			if (m_iDgnCode == NTC_DCEC_2017)
			{
				// 7.4.2.2
				dhcr = max(dLw, SafeDivAbs(dMu, 4.0*dVu));
			}
			else
			{
				// EC8:04 5.4.3.4.2 hcr=a1
				dhcr = max(dLw, dWallHeight / 6.0);
				dhcr = min(dhcr, 2.0*dLw);

				int iStoryNum = GetStoryCountAboveGround(); // Modify by GAY. PMS:3787. ('09.12.05). 지상층 Story의 총개수.
				if (iStoryNum < 7)
					dhcr = min(dhcr, dCurWallHeight);
				else
					dhcr = min(dhcr, 2.0*dCurWallHeight);
			}
			return dhcr;
		};

		// Eurocode only
		if (m_iFrameType == EN_EC8_DUAL_SYSTEM) // only dual system
		{
			// Major Shear
			FuncD.Initialize();
			FuncD.da1 = 0.0;
			FuncD.dF1 = fabs(dBotVuz);
			FuncD.dF2 = max(fabs(dTopVuz), fabs(dBotVuz / 2.0));
			FuncD.dBotZ = dBotZ;
			FuncD.dTopZ = dTopZ;
		}

		const double da1 = l_Calc_a1(dBotMuy, dBotVuz, dTotalHeight, InWallD.dHeight);

		// Major Moment
		FuncDy.Initialize();
		FuncDy.da1   = da1;
		FuncDy.dF1   = dBotMuy;
		FuncDy.dF2   = dTopMuy;
		FuncDy.dBotZ = dBotZ;
		FuncDy.dTopZ = dTopZ;
		std::array<double, 4> maxDy = {dBotMuy, dBotVuz, dBotZ, InWallD.dHeight};
		// Minor Moment
		FuncDz.Initialize();
		FuncDz.da1   = da1;
		FuncDz.dF1   = dBotMuz;
		FuncDz.dF2   = dTopMuz;
		FuncDz.dBotZ = dBotZ;
		FuncDz.dTopZ = dTopZ;
		std::array<double, 4> maxDz = {dBotMuz, dBotVuz, dBotZ, InWallD.dHeight};
		
		std::map<std::pair<double, int>, std::array<double, 2>> mForce; // moment, moment
		std::map<std::pair<double, int>, double> mForceV; // shear
		double d3rdV = 0.0;
		POSITION pos = mDgnWall.GetStartPosition();
		while (pos)
		{
			int iInternalWallId = 0;
			_DGN_WALL_D WallD;
			mDgnWall.GetNextAssoc(pos, iInternalWallId, WallD);
			T_NODE_D NodeD_Bot;
			if (!pAttrCtrl->GetNode(WallD.BotNodeK, NodeD_Bot)) continue;
			T_NODE_D NodeD_Top;
			if (!pAttrCtrl->GetNode(WallD.TopNodeK, NodeD_Top)) continue;

			Set_ForceMomentBySeperate(iInternalWallId, -1, LcomDesign);
			
			const double dWallHeight = fabs(NodeD_Top.z - NodeD_Bot.z);
			const double dPartLeng   = dWallHeight / 4.0;
			for (int kk = 0; kk < 5; ++kk)
			{
				double dz = NodeD_Bot.z + kk * dPartLeng;

				const double dMuy = m_BMy[kk];
				const double dMuz = m_BMz[kk];
				const double dVuz = m_Fzz[kk];

				// envelop 찾기 위한 부재력 저장
				const auto pairK = std::make_pair(dz, kk);
				mForce[pairK]  = std::array<double, 2>{dMuy, dMuz};
				mForceV[pairK] = dVuz;

				if (kk == 0 || kk == 4)
				{
					// 현재 벽체의 높이, 만약 j단이면 바로 위에 연결된 벽체의 높이.
					const double dPosWallHeight = [&]()
					{
						if(kk!=4) return dWallHeight;
						// upper wall
						POSITION pos2 = mDgnWall.GetStartPosition();
						while (pos2)
						{
							int iUpperInternalWallId = 0;
							_DGN_WALL_D UpperWallD;
							mDgnWall.GetNextAssoc(pos2, iUpperInternalWallId, UpperWallD);
							if(WallD.TopNodeK == UpperWallD.BotNodeK)
							{
								T_NODE_D UpperNodeD_Bot;
								if (!pAttrCtrl->GetNode(UpperWallD.BotNodeK, UpperNodeD_Bot)) break;
								T_NODE_D UpperNodeD_Top;
								if (!pAttrCtrl->GetNode(UpperWallD.TopNodeK, UpperNodeD_Top)) break;
								return fabs(UpperNodeD_Top.z - UpperNodeD_Bot.z);
							}
						}
						return dWallHeight;
					}();

					// find max moment
					auto l_SetMaxForce = [&dPosWallHeight, &dz](const double& dMu, const double& dVu, std::array<double, 4>& maxD)
					{
						if (dgn::AbsEQ(dMu, maxD[0]))
						{
							if (dgn::MT(dz, maxD[2]))
							{
								maxD[0] = dMu;
								maxD[1] = dVu;
								maxD[2] = dz;
								maxD[3] = dPosWallHeight;
							}
						}
						else if (dgn::AbsMT(dMu, maxD[0]))
						{
							maxD[0] = dMu;
							maxD[1] = dVu;
							maxD[2] = dz;
							maxD[3] = dPosWallHeight;
						}
					};

					l_SetMaxForce(dMuy, dVuz, maxDy);
					l_SetMaxForce(dMuz, dVuz, maxDz);
				}

				if (dgn::AbsEQ(dVuz, FuncD.dF1))
				{
					if (dgn::MT(dz, FuncD.dBotZ))
					{
						FuncD.dF1   = fabs(dVuz);
						FuncD.dBotZ = dz;
						FuncD.dF2 = max(FuncD.dF2, 0.5*FuncD.dF1);
					}
				}
				else if (dgn::AbsMT(dVuz, FuncD.dF1))
				{
					FuncD.dF1 = fabs(dVuz);
					FuncD.dBotZ = dz;
					FuncD.dF2 = max(FuncD.dF2, 0.5*FuncD.dF1);
				}
			}
		}

		// 최대부재력 지점에서 부터 envelop 만들기
		auto l_Calc_MaxMomentEnvelop = [&l_Calc_a1, &mForce](_DGN_WDLF_D& FuncD, const std::array<double, 4>& maxD, const bool& bMajor)
		{
			if(EQ(FuncD.dTopZ, maxD[2])) // 상단이 최대값인 경우
			{
				FuncD.dF1   = FuncD.dF2   = maxD[0];
				FuncD.dBotZ = FuncD.dTopZ = maxD[2];
				FuncD.da1   = 0.0;
			}
			else
			{
				if(UQ(FuncD.dBotZ, maxD[2])) // 최대값이 하단이 아닌 경우
				{
					const double dHeight = FuncD.dTopZ - maxD[2];

					FuncD.dF1   = maxD[0];
					FuncD.dBotZ = maxD[2];
					FuncD.da1   = l_Calc_a1(FuncD.dF1, maxD[1], dHeight, maxD[3]);
				}

				const int Idx = bMajor ? 0 : 1;
				const double dBotMu = FuncD.dF1;
				const double dBotZ  = FuncD.dBotZ;

				// 1. 최대부재력과 최대부재력 상부 각 포지션의 부재력의 기울기를 계산.
				// 2. 기울기가 가장 큰 경우로 envelope 결정.
				double dMaxInc = dgn::SafeDivAbs(fabs(FuncD.dF2) - fabs(FuncD.dF1), FuncD.dTopZ - dBotZ);
				for(const auto& value : mForce)
				{
					if(!(value.first.second==0 || value.first.second==4)) continue;

					const double dz  = value.first.first;
					const double dMu = value.second[Idx];

					if(LE(dz, dBotZ))	continue;
					const double dGrd = dgn::SafeDivAbs(fabs(dMu) - fabs(dBotMu), dz - dBotZ);
					if (LT(dGrd, dMaxInc))
					{
						dMaxInc     = dGrd;
						FuncD.dF2   = dMu;
						FuncD.dTopZ = dz;
					}
				}
			}
		};

		l_Calc_MaxMomentEnvelop(FuncDy, maxDy, true);
		l_Calc_MaxMomentEnvelop(FuncDz, maxDz, false);
		
		// Eurocode only
		if (m_iFrameType == EN_EC8_DUAL_SYSTEM) // only dual system
		{
			Calc_MaxShearEnvelop(mForceV, dTopVuz, FuncD);
			m_arWallFzFunc.SetAt(FuncKeyMap.KeyMap, FuncD);
		}
		m_arWallMyFunc.SetAt(FuncKeyMap.KeyMap, FuncDy);
		m_arWallMzFunc.SetAt(FuncKeyMap.KeyMap, FuncDz);

		m_iPosiNo = iPosiNo;
		m_iPosiNoV = iPosiNoV;
		Set_ForceMomentBySeperate(iElem, -1, LcomDesign);
	}

	// Get Geometry Information
	T_ELEM_K_LIST aElemK;
	if (!pPostCtrl->GetWallElemList(WallK.keymap, aElemK)) return;
	T_ELEM_D ElemD;
	if (!pAttrCtrl->GetElem(aElemK[0], ElemD)) return;  // Only First Element.
	T_NODE_D NodeD_Bot;
	if (!pAttrCtrl->GetNode(ElemD.elnod[0], NodeD_Bot)) return;  // Only First Node.
	T_NODE_D NodeD_Top;
	if (!pAttrCtrl->GetNode(ElemD.elnod[3], NodeD_Top)) return;  // Only End Node

	dBotZ = NodeD_Bot.z;
	dTopZ = NodeD_Top.z;

	auto L_CalcMomentByFunc = [](const _DGN_WDLF_D& WdlfD, double dMdgn, double dx) -> double
	{
		double da1 = WdlfD.da1 + WdlfD.dBotZ;
		double dM = 0.0;
		if (LE(dx, da1))
		{
			dM = WdlfD.dF1;
		}
		else
		{
			const double dGradiant = dgn::SafeDiv(fabs(WdlfD.dF2) - fabs(WdlfD.dF1), WdlfD.dTopZ - WdlfD.dBotZ);
			dM = dGradiant * (dx - da1) + fabs(WdlfD.dF1);
			if(WdlfD.dF1 < 0.0) dM *= -1.0;
		}
		return fabs(dMdgn) > fabs(dM) ? dMdgn : dM;
	};

	const double dStep = (dTopZ - dBotZ) / 4.0;
	const double dx    = dBotZ + dStep * m_iPosiNo;
	// Major Moment    
	if (m_arWallMyFunc.Lookup(FuncKeyMap.KeyMap, FuncD))
	{
		dMuy = L_CalcMomentByFunc(FuncD, dMuy, dx);
	}
	// Minor Moment
	if (m_arWallMzFunc.Lookup(FuncKeyMap.KeyMap, FuncD))
	{
		dMuz = L_CalcMomentByFunc(FuncD, dMuz, dx);
	}
}

void CDgnForceCtrl::Get_WallMomBySpecialProv_UseSlopePrevious(int iElem, _DGN_LCOM& LcomDesign, double& dMuy, double& dMuz)
{
	dMuy = m_BMy[m_iPosiNo];
	dMuz = m_BMz[m_iPosiNo];

	// Only EC2:04 Special Provision.
	if (!IsCalcWallDesignForce(iElem, LcomDesign)) { return; }

	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;
	CPostCtrl* pPostCtrl = m_pDoc->m_pPostCtrl;

	// Get User Wall ID and Wall Data.
	T_WALL_K WallK;
	if (!pPostCtrl->GetWallKey(iElem, WallK.keymap)) { return; }

	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = m_nCode_Length;
	CngIndex.nBase_Force = m_nCode_Force;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	// <Remember> Data를 받아올때는 Code Unit기준
	//////////////////////////////////////////////

	// Get Length.
	T_WALL_D WallD;
	if (!pPostCtrl->GetWallData(WallK.keymap, WallD)) { return; }

	const double dTotalHeight = GetWallTotalHeight(WallK.key.wallid);
	const double dLw = WallD.dLength;

	if (m_iDgnCode == Eurocode2_04)
	{
		if (!IsSlenderWallByEC8(dTotalHeight, dLw)) { return; }
	}

	if (m_mAdjIntwall.count(iElem) == 0)
	{
		MakeAdjacentInternalWallIDMap(WallK.key.wallid);
	}

	std::unordered_map<UINT, ADJ_INTWALLID_K>::iterator itr = m_mAdjIntwall.find(iElem);
	if (itr == m_mAdjIntwall.end())
	{
		return;
	}

	const int nPosiNo = m_iPosiNo;
	const int nPosiNoV = m_iPosiNoV;

	const ADJ_INTWALLID_K& AdjWall = itr->second;

	const UINT unBotIntWall = AdjWall.first;
	double dBotMuy = 0.0;
	double dBotMuz = 0.0;
	if (unBotIntWall != 0)
	{
		Set_ForceMomentBySeperate(unBotIntWall, 4, LcomDesign);
		dBotMuy = m_BMy[4];
		dBotMuz = m_BMz[4];
	}

	const UINT unTopIntWall = AdjWall.second;
	double dTopMuy = 0.0;
	double dTopMuz = 0.0;
	if (unTopIntWall != 0)
	{
		Set_ForceMomentBySeperate(unTopIntWall, 0, LcomDesign);
		dTopMuy = m_BMy[0];
		dTopMuz = m_BMz[0];
	}

	Set_ForceMomentBySeperate(iElem, -1, LcomDesign);

	dBotMuy = max(fabs(dBotMuy), fabs(m_BMy[0]));
	dBotMuz = max(fabs(dBotMuz), fabs(m_BMz[0]));

	dTopMuy = max(fabs(dTopMuy), fabs(m_BMy[4]));
	dTopMuz = max(fabs(dTopMuz), fabs(m_BMz[4]));

	dMuy = max(dBotMuy, dTopMuy);
	dMuz = max(dBotMuz, dTopMuz);

	m_iPosiNo = nPosiNo;
	m_iPosiNoV = nPosiNoV;
}

double CDgnForceCtrl::GetWallTotalHeight(int nUserWallId)
{
	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;
	CPostCtrl* pPostCtrl = m_pDoc->m_pPostCtrl;

	CArray<UINT, UINT> aIntWallID;
	if (!pPostCtrl->GetInternalWallIdList(nUserWallId, aIntWallID)) { return 0.0; }

	std::vector<double> vdLevel;
	const INT_PTR nSizeInter = aIntWallID.GetSize();
	for (INT_PTR m = 0; m < nSizeInter; ++m)
	{
		const UINT unCurID = aIntWallID[m];
		if (IsUnderGroundMemb(unCurID)) continue; // PMS:tttt-Seungjun-20100310  // check underground.// not apply on 780.

		T_WALL_K InWallK;
		if (!pPostCtrl->GetWallKey(unCurID, InWallK.keymap)) continue;
		T_ELEM_K_LIST aElemK;
		if (!pPostCtrl->GetWallElemList(InWallK.keymap, aElemK)) continue;
		T_ELEM_D ElemD;
		if (!pAttrCtrl->GetElem(aElemK[0], ElemD)) continue;  // Only First Element.
		T_NODE_D BotNodeD;
		if (!pAttrCtrl->GetNode(ElemD.elnod[0], BotNodeD)) continue;
		T_NODE_D TopNodeD;
		if (!pAttrCtrl->GetNode(ElemD.elnod[3], TopNodeD)) continue;

		vdLevel.emplace_back(BotNodeD.z);
		vdLevel.emplace_back(TopNodeD.z);
	}
	std::sort(vdLevel.begin(), vdLevel.end());

	if (vdLevel.size() < 1) { return 0.0; }

	return fabs(vdLevel.back() - vdLevel.front());
}

// PMS:3956-Seungjun-20090313 EC8:04 Seismic Design
double CDgnForceCtrl::Get_WallShrBySpecialProv(int iElem, _DGN_LCOM& LcomDesign, const double dFlxRat, const double dVuzorg)
{
	// Only EC2:04 Special Provision(EC8:04).
	if (!IsCalcWallDesignForce(iElem, LcomDesign)) { return dVuzorg; }

	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = m_nCode_Length;
	CngIndex.nBase_Force = m_nCode_Force;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	// <Remember> Data를 받아올때는 Code Unit기준
	//////////////////////////////////////////////

	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;
	CPostCtrl* pPostCtrl = m_pDoc->m_pPostCtrl;

	// Get User Wall ID and Wall Data.
	T_WALL_K WallK;
	if (!pPostCtrl->GetWallKey(iElem, WallK.keymap)) { return dVuzorg; }
	T_WALL_D WallD;
	if (!pPostCtrl->GetWallData(WallK.keymap, WallD)) { ASSERT(0); return dVuzorg; }

	// Get Internal Wall-Id List at Wall ID.
	CArray<UINT, UINT> aInternalWallID;
	if (!pPostCtrl->GetInternalWallIdList(WallK.key.wallid, aInternalWallID)) { return dVuzorg; }
	if (aInternalWallID.GetSize() == 0) return dVuzorg;

	// Make key for envelop force function.
	_DGN_WDLF_K FuncKeyMap;
	FuncKeyMap.Key.wallid = WallK.key.wallid;
	FuncKeyMap.Key.Lcomk = LcomDesign.DesignLcomKey;

	bool bSlender = true;
	double dBotZ = 0.0;
	double dTopZ = 0.0;

	_DGN_WDLF_D FuncD;
	if (!m_arWallFzFunc.Lookup(FuncKeyMap.KeyMap, FuncD))
	{
		int iBotElemK = 0;
		int iTopElemK = 0;
		CMapEx<int, int, _DGN_WALL_D, _DGN_WALL_D&> mDgnWall;
		Get_DGN_WALL_D_Map(aInternalWallID, mDgnWall, iBotElemK, iTopElemK, dBotZ, dTopZ);

		const double dTotalHeight = fabs(dTopZ - dBotZ);

		// Get Internal-Wall ID & Elem Data.
		UINT unBotElemNo = 0;
		if (!pPostCtrl->GetInternalWallId(iBotElemK, unBotElemNo)) { ASSERT(0); return dVuzorg; }
		UINT unTopElemNo = 0;
		if (!pPostCtrl->GetInternalWallId(iTopElemK, unTopElemNo)) { ASSERT(0); return dVuzorg; }
		// Get KeyWall.
		T_WALL_K InWallK;
		if (!pPostCtrl->GetWallKey(unBotElemNo, InWallK.keymap)) { ASSERT(0); return dVuzorg; }
		// Get Length.
		T_WALL_D InWallD;
		if (!pPostCtrl->GetWallData(InWallK.keymap, InWallD)) { ASSERT(0); return dVuzorg; }

		const double dLw = InWallD.dLength;
		// Only slender wall (hw/lw > 2)
		bSlender = IsSlenderWallByEC8(dTotalHeight, dLw);

		if (m_iFrameType == EN_EC8_DUAL_SYSTEM) // Dual
		{
			int iPosiNo = m_iPosiNo;
			Set_ForceMomentBySeperate(unBotElemNo, 0, LcomDesign);
			double dBotVuz = m_Fzz[0];
			if (unBotElemNo != unTopElemNo)
			{
				Set_ForceMomentBySeperate(unTopElemNo, 4, LcomDesign);
			}
			m_iPosiNo = iPosiNo;
			double dTopVuz = m_Fzz[4];
			Set_ForceMomentBySeperate(iElem, -1, LcomDesign);

			FuncD.Initialize();
			FuncD.da1 = 0.0;
			FuncD.dF1 = fabs(dBotVuz);
			FuncD.dF2 = max(fabs(dTopVuz), fabs(dBotVuz / 2.0));
			FuncD.dBotZ = dBotZ;
			FuncD.dTopZ = dTopZ;

			std::map<std::pair<double, int>, double> mForce; // shear
			double d3rdV = 0.0;
			POSITION pos = mDgnWall.GetStartPosition();
			while (pos)
			{
				int iInternalWallId = 0;
				_DGN_WALL_D DgnWallD;
				mDgnWall.GetNextAssoc(pos, iInternalWallId, DgnWallD);
				if (iInternalWallId == unBotElemNo) continue;
				if (iInternalWallId == unTopElemNo) continue;
				T_NODE_D NodeD_Bot;
				if (!pAttrCtrl->GetNode(DgnWallD.BotNodeK, NodeD_Bot)) continue;
				T_NODE_D NodeD_Top;
				if (!pAttrCtrl->GetNode(DgnWallD.TopNodeK, NodeD_Top)) continue;

				Set_ForceMomentBySeperate(iInternalWallId, -1, LcomDesign);

				const double dPartLeng = fabs(NodeD_Top.z - NodeD_Bot.z) / 4.0;
				for (int kk = 0; kk < 5; kk++)
				{
					double dz = NodeD_Bot.z + kk * dPartLeng;
					const double dVuz = m_Fzz[kk];

					// envelop 찾기 위한 부재력 저장
					const auto pairK = std::make_pair(dz, kk);
					mForce[pairK] = dVuz;

					if (dgn::AbsEQ(dVuz, FuncD.dF1))
					{
						if (dgn::MT(dz, FuncD.dBotZ))
						{
							FuncD.dF1 = fabs(dVuz);
							FuncD.dBotZ = dz;
							FuncD.dF2 = max(FuncD.dF2, 0.5*FuncD.dF1);
						}
					}
					else if (dgn::AbsMT(dVuz, FuncD.dF1))
					{
						FuncD.dF1 = fabs(dVuz);
						FuncD.dBotZ = dz;
						FuncD.dF2 = max(FuncD.dF2, 0.5*FuncD.dF1);
					}
				}
			}

			Calc_MaxShearEnvelop(mForce, dTopVuz, FuncD);
			m_arWallFzFunc.SetAt(FuncKeyMap.KeyMap, FuncD);

			Set_ForceMomentBySeperate(iElem, -1, LcomDesign);
		}
		else
		{
			FuncD.Initialize();
			FuncD.da1 = 0.0;
			FuncD.dF1 = 0.0;
			FuncD.dF2 = 0.0;
			FuncD.dBotZ = dBotZ;
			FuncD.dTopZ = dTopZ;
			m_arWallFzFunc.SetAt(FuncKeyMap.KeyMap, FuncD);
		}
	}
	else
	{
		const double dTotalHw = fabs(FuncD.dTopZ - FuncD.dBotZ);
		// Only slender wall (hw/lw > 2)
		bSlender = IsSlenderWallByEC8(dTotalHw, WallD.dLength);
	}

	// Get Geometry Information
	T_ELEM_K_LIST aElemK;
	if (!pPostCtrl->GetWallElemList(WallK.keymap, aElemK)) return dVuzorg;
	T_ELEM_D ElemD;
	if (!pAttrCtrl->GetElem(aElemK[0], ElemD)) return dVuzorg;  // Only First Element.
	T_NODE_D NodeD_Bot;
	if (!pAttrCtrl->GetNode(ElemD.elnod[0], NodeD_Bot)) return dVuzorg;  // Only First Node.
	T_NODE_D NodeD_Top;
	if (!pAttrCtrl->GetNode(ElemD.elnod[3], NodeD_Top)) return dVuzorg;  // Only End Node

	dBotZ = NodeD_Bot.z;
	dTopZ = NodeD_Top.z;

	const double dStep = (dTopZ - dBotZ) / 4.0;
	const double dx = dBotZ + dStep * m_iPosiNo;

	auto L_GetPeroid = [](double dAngle, double dT1X, double dT1Y)
	{
		// 090701. 벽체의 방향에 따라 주기 사용. agree with 이대근CJ, Stefano in Italy.
		// 1. Global-X축과 45도 이내에 있는 경우 Global-X방향 주기 사용.
		// 2. Global-Y축과 45도 이내에 있는 경우 Global-Y방향 주기 사용.
		// 3. 45도 선상에 있는 경우 Global-X, Global-Y 중 큰 값 사용.
		const double dAngleAbs = fabs(dAngle);
		if (-cDgn_Zero < dAngleAbs && dAngleAbs < 45.0) { return dT1X; }
		if (315.0 < dAngleAbs && dAngleAbs < 360.0 + cDgn_Zero) { return dT1X; }
		if (135.0 < dAngleAbs && dAngleAbs < 225.0) { return dT1X; }
		if (45.0 < dAngleAbs && dAngleAbs < 135.0) { return dT1Y; }
		if (225.0 < dAngleAbs && dAngleAbs < 315.0) { return dT1Y; }
		return max(dT1X, dT1Y);  // 45, 135, 225, 315도 축선상에 존재할 경우.
	};

	auto L_GetMagFactor = [](double dq, double dGammaRd, double dRatMRdMEd, double dSeTc, double dSeT1)
	{
		/// EN 1998-1:2004, 5.5.2.4.1(7) EQ(5.25) p.109
		const double dParam1 = dgn::SafeDiv(dGammaRd, dq)*dRatMRdMEd;
		const double dParam2 = dgn::SafeDiv(dSeTc, dSeT1);
		const double dEsp = dq * sqrt(dParam1*dParam1 + 0.1*dParam2*dParam2);
		return min(max(dEsp, 1.5), dq);
	};

	BOOL bNTC18 = (m_bNAEQDgn && m_nNTCType == RCS_NTC18) ? TRUE : FALSE;
	double dVuz = fabs(dVuzorg);
	if (m_iFrameType == EN_EC8_DUAL_SYSTEM) // only dual system and slender wall
	{
		// Minor Shear
		if (m_arWallFzFunc.Lookup(FuncKeyMap.KeyMap, FuncD)) // get envelope
		{
			double da = FuncD.dBotZ + fabs(FuncD.dTopZ - FuncD.dBotZ) / 3.0;
			double dV = 0.0;
			if (m_iDgnClass == EN_EC8_DCH || (m_iDgnClass == EN_EC8_DCM && m_bNAEQDgn && m_nNTCType == RCS_NTC12)) //(Tel. 4022) MNET:XXXX-BSCHOI-20121023 #5 // DCH or DCM && NTC2012
			{
				T_NODE_D EdNode;
				if (!pAttrCtrl->GetNode(ElemD.elnod[1], EdNode)) { return dVuzorg; } // Only First Node.
				const double dAngle = CMathFunc::mathCrossAngle2D(1.0, 0.0, WallD.ux, WallD.uy);
				const double dT1 = L_GetPeroid(dAngle, m_dEigenXPeriod, m_dEigenYPeriod);

				_DGN_WDLF_D OrgFuncD = FuncD;
				double dVed = dVuzorg;
				const double dFlx = dFlxRat;
				const double dq = Get_qForEC4();
				const double dGamma_rd = m_dAlpha1; // for DCH
				if (bSlender)  // Slender
				{
					const double dSeTc = Get_ElasticResponseSpectrum(LcomDesign.iSeismicType, m_dTC);
					const double dSeT1 = Get_ElasticResponseSpectrum(LcomDesign.iSeismicType, dT1);
					const double dEsp = L_GetMagFactor(dq, dGamma_rd, dFlx, dSeTc, dSeT1);

					// EC8:04 5.4.2.4 Figure 5.4 p.94
					if (dx < FuncD.dBotZ)
					{
						dV = dEsp * FuncD.dF1;
						double dLim = 0.5*dEsp*FuncD.dF1;
						dVed = fabs(dV) > fabs(dLim) ? dV : dLim;
					}
					else if (dx < da - cDgn_Zero)
					{
						dV = dEsp * dVed;
						double dLim = 0.5*dEsp*FuncD.dF1;
						dVed = fabs(dV) > fabs(dLim) ? dV : dLim;
					}
					else
					{
						double dLim = 0.5*dEsp*FuncD.dF1;
						double dF13 = max(dEsp*FuncD.dF13, dLim);
						double dFTop = max(dEsp*FuncD.dF2, dLim);
						double dGradiant = dgn::SafeDiv(dFTop - dF13, 2.0*(da - FuncD.dBotZ));
						dV = dGradiant * (dx - da) + dF13;
						dVed = fabs(dEsp*dVed) > fabs(dV) ? dEsp * dVed : dV;
					}
				}
				else  // squat
				{
					// EC8:04 5.5.2.4.2((2) Eq(5.26) p.109
					dV = dGamma_rd * dFlx * dVed;
					dVed = fabs(dV) > 0.0 ? min(fabs(dV), fabs(dq*dVed)) : dq * dVed;
				}

				dVuz = fabs(dVed);
			}
			else if (m_iDgnClass == EN_EC8_DCM)
			{
				if (!bNTC18)  // DCM : [MQC 18831] 이탈리아의 요청으로 NTC2018+CD_T("B") 인 경우에는 해석부재력을 그대로 사용.		
				{
					if (bSlender)
					{
						// EC8:04 5.4.2.4 Figure 5.4 p.94
						if (dx < FuncD.dBotZ)
						{
							dV = 1.5*FuncD.dF1;
							double dTop = 1.5*FuncD.dF2;
							double dLim = max(0.75*FuncD.dF1, dTop);
							dVuz = fabs(dV) > fabs(dLim) ? dV : dLim;
						}
						else if (dx < da + cDgn_Zero)
						{
							dV = 1.5*dVuz;
							double dTop = 1.5*FuncD.dF2;
							double dLim = max(0.75*FuncD.dF1, dTop);
							dVuz = fabs(dV) > fabs(dLim) ? dV : dLim;
						}
						else
						{
							double dLim = 0.75*FuncD.dF1;
							double dF13 = max(1.5*FuncD.dF13, dLim);
							double dFTop = max(1.5*FuncD.dF2, dLim);
							double dGradiant = dgn::SafeDiv(dFTop - dF13, 2.0*(da - FuncD.dBotZ));
							dV = dGradiant * (dx - da) + dF13;
							dVuz = fabs(1.5*dVuz) > fabs(dV) ? 1.5*dVuz : dV;
						}
					}
					else
					{
						dVuz *= 1.5;
					}
				}
			}
			else ASSERT(0);
		}
	}
	else // Wall system
	{
		// Minor axis
		double dV = 0.0;
		if (m_iDgnClass == EN_EC8_DCH || (m_iDgnClass == EN_EC8_DCM && m_bNAEQDgn && m_nNTCType == RCS_NTC12)) //(Tel. 4022) MNET:XXXX-BSCHOI-20121023 #5 // DCH or DCM && NTC2012
		{
			T_NODE_D EdNode;
			if (!pAttrCtrl->GetNode(ElemD.elnod[1], EdNode)) return dVuzorg;  // Only First Node.
			const double dAngle = math_lib::angle_2d(NodeD_Bot.x, NodeD_Bot.y, EdNode.x, EdNode.y);
			const double dT1 = L_GetPeroid(dAngle, m_dEigenXPeriod, m_dEigenYPeriod);

			_DGN_WDLF_D OrgFuncD = FuncD;
			double dVed = dVuzorg;
			const double dFlx = dFlxRat;
			const double dq = Get_qForEC4();
			const double dGamma_rd = m_dAlpha1; // for DCH
			
			if (bSlender)
			{
				const double dSeTc = Get_ElasticResponseSpectrum(LcomDesign.iSeismicType, m_dTC);
				const double dSeT1 = Get_ElasticResponseSpectrum(LcomDesign.iSeismicType, dT1);
				// EC8:04 5.5.2.4.1(7) Eq(5.25) p.109
				const double dEsp = L_GetMagFactor(dq, dGamma_rd, dFlx, dSeTc, dSeT1);
				// EC8:04 5.5.2.4.1(7) Eq(5.24) p.108
				dVed *= dEsp;
			}
			else
			{
				// EC8:04 5.5.2.4.2((2) Eq(5.26) p.109
				dV = dGamma_rd * dFlx * dVed;
				dVed = fabs(dV) > 0.0 ? min(fabs(dV), fabs(dq*dVed)) : dq * dVed;
			}

			dVuz = fabs(dVed);
		}
		else if (m_iDgnClass == EN_EC8_DCM)
		{
			if (!bNTC18)  // [MQC 18831] 이탈리아의 요청으로 NTC2018+CD_T("B") 인 경우에는 해석부재력을 그대로 사용.
			{ 
				dVuz *= 1.5; 
			}
		}
		else  ASSERT(0);
	}
	return dVuz;
}

void CDgnForceCtrl::Calc_MaxShearEnvelop(const std::map<std::pair<double, int>, double>& mForce, const double& dTopVuz, _DGN_WDLF_D& FuncD)
{
	if (EQ(FuncD.dTopZ, FuncD.dBotZ)) // 상단이 최대값인 경우
	{
		FuncD.dF2 = FuncD.dF1;
		FuncD.dF13 = FuncD.dF1;
	}
	else
	{
		const double dThirdHeight = FuncD.dBotZ + (FuncD.dTopZ - FuncD.dBotZ) / 3.0;

		auto pairUPvalue = std::make_pair(FuncD.dTopZ, fabs(dTopVuz));
		auto pairDNvalue = std::make_pair(FuncD.dBotZ, fabs(FuncD.dF1));
		for (const auto& value : mForce)
		{
			const double dz = value.first.first;
			const double dVu = fabs(value.second);

			if (EQ(dz, dThirdHeight))
			{
				if (EQ(pairUPvalue.second, pairDNvalue.second))
				{
					if (MT(dVu, pairUPvalue.second))
					{
						pairUPvalue.first = dz;
						pairUPvalue.second = dVu;
						pairDNvalue.first = dz;
						pairDNvalue.second = dVu;
					}
				}
				else
				{
					pairUPvalue.first = dz;
					pairUPvalue.second = dVu;
					pairDNvalue.first = dz;
					pairDNvalue.second = dVu;
				}
			}
			else if (MT(dz, dThirdHeight))
			{
				if (EQ(pairUPvalue.first, dz))
				{
					if (EQ(pairUPvalue.second, dVu))
					{
						if (value.first.second == 0)
						{
							pairUPvalue.first = dz;
							pairUPvalue.second = dVu;
						}
					}
					else if (MT(pairUPvalue.second, dVu))
					{
						pairUPvalue.first = dz;
						pairUPvalue.second = dVu;
					}
				}
				else if (MT(pairUPvalue.first, dz))
				{
					pairUPvalue.first = dz;
					pairUPvalue.second = dVu;
				}
			}
			else
			{
				if (EQ(pairDNvalue.first, dz))
				{
					if (EQ(pairDNvalue.second, dVu))
					{
						if (value.first.second == 4)
						{
							pairDNvalue.first = dz;
							pairDNvalue.second = dVu;
						}
					}
					else if (MT(pairDNvalue.second, dVu))
					{
						pairDNvalue.first = dz;
						pairDNvalue.second = dVu;
					}
				}
				else if (LT(pairDNvalue.first, dz))
				{
					pairDNvalue.first = dz;
					pairDNvalue.second = dVu;
				}
			}
		}

		const double dV1 = pairDNvalue.second;
		const double dV2 = pairUPvalue.second;
		const double dh1 = pairDNvalue.first;
		const double dh2 = pairUPvalue.first;
		FuncD.dF13 = (dV2 - dV1)*dgn::SafeDiv(dThirdHeight - dh1, dh2 - dh1) + dV1;
	}
}

void CDgnForceCtrl::Set_nth_ModePeriod(int iMode)
{
	// Get Eigen-value Period at 1st-Mode.
	m_dEigenPeriod = 0.0;
	T_FREQ_D FreqD;
	if (!m_pDoc->m_pPostCtrl->GetEiFreq(iMode, FreqD)) { FreqD.Initialize(); m_dEigenPeriod = -1.0; return; }
	m_dEigenPeriod = FreqD.dblFreq[2];
}

void CDgnForceCtrl::Set_Init_ModePeriod()
{
	m_dEigenPeriod = 0.0;
	m_dEigenXPeriod = 0.0;
	m_dEigenYPeriod = 0.0;
}

double CDgnForceCtrl::Get_qForEC4()
{
	// Modify by GAY. PMS:3787. ('09.12.06). 사용자가 입력하는 값은 q.
	if (!m_bAutoBehaviorFact)
	{
		return m_dBehaviorFact_q;
	}
	else
	{
		const double dkw = Get_PrevailingFactor(m_iFrameType);
		const double dq0 = Get_BehaviourFactor(m_iFrameType, m_iDgnClass, m_daua1, m_bNAEQDgn, m_nNTCType);
		return max(dq0*dkw, 1.5);
	}
}

void CDgnForceCtrl::Set_1st_ModePeriodByDir()
{
	// Get Eigen-value Period at 1st-Mode.
	m_dEigenXPeriod = 0.0;
	m_dEigenYPeriod = 0.0;

	T_DCON_D DconD;
	if (!m_pDgnDataCtrl->Get_DgnConDcon(DconD)) { DconD.Initialize(); }
	if (DconD.bT1User)
	{
		Set_Period_UserT1(DconD);
		return;
	}

	m_pDoc->m_pPostCtrl->GetEiPeriodByDir(m_dEigenXPeriod, m_dEigenYPeriod);
}

double CDgnForceCtrl::Get_FinalPeriod(double dPeriodAnal, double dPeriodCode, double dFact)
{
	// See CLateralLoad::CalcPeriodGeneral(	
	if (dPeriodAnal*dPeriodCode > 0.0)
	{
		return min(dPeriodAnal, dPeriodCode*dFact);
	}
	return max(dPeriodAnal, dPeriodCode);
}

// Add by GAY. PMS:3787. ('09.12.06). Static Seismic Load의 기본주기
void CDgnForceCtrl::Get_Period_StaticSeismicByDir(T_KEY SeisK, double& dPeriodX, double& dPeriodY)
{
	dPeriodX = 0.0;
	dPeriodY = 0.0;

	// See wg_db\DB_ST_DT_SEIS.h
	T_SEIS_D SeisD;
	if (m_pDoc->m_pAttrCtrl->GetSeis(SeisK, SeisD))
	{
		switch (SeisD.nCodeType)
		{
		case KS_E_1992:
		{
			const T_SEIS_KS1992& SeisCode = SeisD.KS1992;
			double dFact = 1.2;
			dPeriodX = Get_FinalPeriod(SeisCode.dPeriodAnalysis_X, SeisCode.dPeriodCode_X, dFact);
			dPeriodY = Get_FinalPeriod(SeisCode.dPeriodAnalysis_Y, SeisCode.dPeriodCode_Y, dFact);
		}
		break;
		case UBC_E_1991:
		{
			const T_SEIS_UBC1991& SeisCode = SeisD.UBC1991;
			dPeriodX = max(SeisCode.dPeriodAnalysis_X, SeisCode.dPeriodCode_X);
			dPeriodY = max(SeisCode.dPeriodAnalysis_Y, SeisCode.dPeriodCode_Y);
		}
		break;
		case UBC_E_1997:
		{
			const T_SEIS_UBC1997& SeisCode = SeisD.UBC1997;
			double dFact = (fabs(SeisCode.dSeismicZoneFactor - 0.40) < 1.0e-6) ? 1.3 : 1.4;
			dPeriodX = Get_FinalPeriod(SeisCode.dPeriodAnalysis_X, SeisCode.dPeriod_X, dFact);
			dPeriodY = Get_FinalPeriod(SeisCode.dPeriodAnalysis_Y, SeisCode.dPeriod_Y, dFact);
		}
		break;
		case ATC306_E:
		{
			const T_SEIS_ATC306& SeisCode = SeisD.ATC306;
			double dFact = 1.2;
			dPeriodX = Get_FinalPeriod(SeisCode.dPeriodAnalysis_X, SeisCode.dPeriodCode_X, dFact);
			dPeriodY = Get_FinalPeriod(SeisCode.dPeriodAnalysis_Y, SeisCode.dPeriodCode_Y, dFact);
		}
		break;
		case KS_E_2000:
		{
			const T_SEIS_KS2000& SeisCode = SeisD.KS2000;
			double dFact = 1.2;
			dPeriodX = Get_FinalPeriod(SeisCode.dPeriodAnalysis_X, SeisCode.dPeriodCode_X, dFact);
			dPeriodY = Get_FinalPeriod(SeisCode.dPeriodAnalysis_Y, SeisCode.dPeriodCode_Y, dFact);
		}
		break;
		case JIS_E_YY:
		{
			const T_SEIS_JIS& SeisCode = SeisD.JP;
			dPeriodX = SeisCode.dPeriodCode_X;
			dPeriodY = SeisCode.dPeriodCode_Y;
		}
		break;
		case IBC_E_2000:
		{
			//**// Temp : Code에 의한 주기만 고려. (이 함수는 EC2:04에서만 사용하므로)
			const T_SEIS_IBC2000& SeisCode = SeisD.IBC2000;
			dPeriodX = SeisCode.dPeriodCode_X;
			dPeriodY = SeisCode.dPeriodCode_Y;
		}
		break;
		case EURO_E_1996:
		{
			const T_SEIS_EURO1996& SeisCode = SeisD.EURO1996;
			dPeriodX = SeisCode.dPeriod_X;
			dPeriodY = SeisCode.dPeriod_Y;
		}
		break;
		case CH_E_2001:
		{
			const T_SEIS_CH2002& SeisCode = SeisD.CH2002;
			dPeriodX = SeisCode.dPeriod_X;
			dPeriodY = SeisCode.dPeriod_Y;
		}
		break;
		case NBC_E_1995:
		{
			//**// Temp : Code에 의한 주기만 고려. (이 함수는 EC2:04에서만 사용하므로)
			const T_SEIS_NBC1995& SeisCode = SeisD.NBC1995;
			dPeriodX = SeisCode.dPeriodCode_X;
			dPeriodY = SeisCode.dPeriodCode_Y;
		}
		break;
		case IS_E_2002:
		{
			const T_SEIS_IS2002& SeisCode = SeisD.IS2002;
			dPeriodX = SeisCode.dPeriod_X;
			dPeriodY = SeisCode.dPeriod_Y;
		}
		break;
		case TAIWAN_E_1999:
		{
			const T_SEIS_TAIWAN86& SeisCode = SeisD.TAIWAN86;
			dPeriodX = SeisCode.dPeriodFinalX;
			dPeriodY = SeisCode.dPeriodFinalY;
		}
		break;
		case KBC_E_2005:
		{
			const T_SEIS_KBC2005& SeisCode = SeisD.KBC2005;
			dPeriodX = SeisCode.dPeriodFinalX;
			dPeriodY = SeisCode.dPeriodFinalY;
		}
		break;
		case CHSH_E_2003:
		{
			const T_SEIS_CHSH2003& SeisCode = SeisD.CHSH2003;
			dPeriodX = SeisCode.dPeriod_X;
			dPeriodY = SeisCode.dPeriod_Y;
		}
		break;
		case TAIWAN_E_2006:
		{
			const T_SEIS_TAIWAN02& SeisCode = SeisD.TAIWAN02;
			dPeriodX = SeisCode.dPeriodFinalX;
			dPeriodY = SeisCode.dPeriodFinalY;
		}
		break;
		case EURO_E_2004:
		{
			const T_SEIS_EURO2004& SeisCode = SeisD.EURO2004;
			dPeriodX = SeisCode.dPeriod_X;
			dPeriodY = SeisCode.dPeriod_Y;
		}
		break;
		case KBC_E_2009:
		{
			const T_SEIS_KBC2009& SeisCode = SeisD.KBC2009;
			dPeriodX = SeisCode.dPeriodFinalX;
			dPeriodY = SeisCode.dPeriodFinalY;
		}
		break;
		case TAIWAN_E_2011:
		{
			const T_SEIS_TAIWAN02& SeisCode = SeisD.TAIWAN11;
			dPeriodX = SeisCode.dPeriodFinalX;
			dPeriodY = SeisCode.dPeriodFinalY;
		}
		break;
		case TAIWAN_E_2022:
		{
			const T_SEIS_TAIWAN22& SeisCode = SeisD.TAIWAN22;
			dPeriodX = SeisCode.dPeriodFinalX;
			dPeriodY = SeisCode.dPeriodFinalY;
		}
		break;
		case NTC_E_2008:
		{
			const T_SEIS_NTC2008& SeisCode = SeisD.NTC2008;
			dPeriodX = SeisCode.dPeriod_X;
			dPeriodY = SeisCode.dPeriod_Y;
		}
		break;
		case CH_E_2010:
		{
			const T_SEIS_CH2010& SeisCode = SeisD.CH2010;
			dPeriodX = SeisCode.dPeriod_X;
			dPeriodY = SeisCode.dPeriod_Y;
		}
		break;
		case IBC_E_2009:
		{
			const T_SEIS_IBC2012& SeisCode = SeisD.IBC2009;
			dPeriodX = SeisCode.dPeriodFinalX;
			dPeriodY = SeisCode.dPeriodFinalY;
		}
		break;
		case IBC_E_2012:
		{
			const T_SEIS_IBC2012& SeisCode = SeisD.IBC2012;
			dPeriodX = SeisCode.dPeriodFinalX;
			dPeriodY = SeisCode.dPeriodFinalY;
		}
		break;
		case NSCP_E_2024:
			{
				const T_SEIS_IBC2012& SeisCode = SeisD.NSCP2024;
				dPeriodX = SeisCode.dPeriodFinalX;
				dPeriodY = SeisCode.dPeriodFinalY;
			}
			break;
		case NTC_E_2012:
		{
			const T_SEIS_NTC2008& SeisCode = SeisD.NTC2012;
			dPeriodX = SeisCode.dPeriod_X;
			dPeriodY = SeisCode.dPeriod_Y;
		}
		break;
		case NSR_E_2010:
		{
			const T_SEIS_NSR2010& SeisCode = SeisD.NSR2010;
			dPeriodX = SeisCode.dPeriodFinalX;
			dPeriodY = SeisCode.dPeriodFinalY;
		}
		break;
		case KBC_E_2016:
		{
			const T_SEIS_KBC2015& SeisCode = SeisD.KBC2015;
			dPeriodX = SeisCode.dPeriodFinalX;
			dPeriodY = SeisCode.dPeriodFinalY;
		}
		break;
		case P100_E_2013:
		{
			const T_SEIS_P100_2013& SeisCode = SeisD.P100_2013;
			dPeriodX = SeisCode.dPeriod_X;
			dPeriodY = SeisCode.dPeriod_Y;
		}
		break;
		case IS_E_2016:
		{
			const T_SEIS_IS1893_2016& SeisCode = SeisD.IS1893_2016;
			dPeriodX = SeisCode.dPeriod_X;
			dPeriodY = SeisCode.dPeriod_Y;
		}
		break;
		case NTC_E_2017:
		{
			const T_SEIS_NTC2008& SeisCode = SeisD.NTC2018;
			dPeriodX = SeisCode.dPeriod_X;
			dPeriodY = SeisCode.dPeriod_Y;
		}
		break;
		case KDS_E_2019:
		{
			const T_SEIS_KDS2019& SeisCode = SeisD.KDS2019;
			dPeriodX = SeisCode.dPeriodFinalX;
			dPeriodY = SeisCode.dPeriodFinalY;
		}
		break;
		case IS_E_2025:
		{
			const T_SEIS_IS1893_2025& SeisCode = SeisD.IS2025;
			dPeriodX = SeisCode.dPeriod_X;
			dPeriodY = SeisCode.dPeriod_Y;
		}
		break;
		default: ASSERT(0);
			break;
		}
	}
}

// Add by GAY. PMS:3787. ('09.12.07). 해당 하중조합에 정의된 Static Seismic Load를 검색하여 방향별 가장 큰 주기 얻어오기.
void CDgnForceCtrl::Set_Period_StaticSeismicByLcom(const T_LCOM_D_UL& LcomDUL)
{
	if ((m_bExistSplc && m_dEigenPeriod > 0.0) || !m_bExistSeis) { return; }

	T_DCON_D DconD;
	if (!m_pDgnDataCtrl->Get_DgnConDcon(DconD)) { DconD.Initialize(); }
	if (DconD.bT1User)
	{
		Set_Period_UserT1(DconD);
		return;
	}

	double dMaxFact = 0.0;
	double dTX = 0.0;
	double dTY = 0.0;

	const INT_PTR nSizeBase = LcomDUL.Combination.GetSize();
	for (INT_PTR i = 0; i < nSizeBase; ++i)
	{
		const T_LCOM_BASE& LcomBase = LcomDUL.Combination[i];

		if (LcomBase.AnalType != D_LCOM_STATIC) continue;

		T_STLD_D StldD;
		if (m_pDoc->m_pAttrCtrl->GetStld(LcomBase.LoadCaseKey, StldD))
		{
			double dCurTX = 0.0;
			double dCurTY = 0.0;
			if (StldD.LoadCaseType == _T("E"))
			{
				Get_Period_StaticSeismicByDir(LcomBase.LoadCaseKey, dCurTX, dCurTY);
				const double dFact = fabs(LcomBase.Factor);
				if (dgn::LE(dMaxFact, dFact))
				{
					dMaxFact = dFact;
					if (dTX < dCurTX) { dTX = dCurTX; }
					if (dTY < dCurTY) { dTY = dCurTY; }
				}
			}
		}
	}
	m_dEigenXPeriod = dTX;
	m_dEigenYPeriod = dTY;
	m_dEigenPeriod = max(dTX, dTY);
}

void CDgnForceCtrl::Set_Period_StaticSeismicByLcom(int iLcomNo)
{
	_DGN_LCOM LcomDesign;
	if (m_parLcomData->Lookup(iLcomNo, LcomDesign))
	{
		Set_Period_StaticSeismicByLcom(LcomDesign.LcomUlData);
	}
}

void CDgnForceCtrl::Set_Period_UserT1(const T_DCON_D& DconD)
{
	m_dEigenXPeriod = DconD.dT1_x;
	m_dEigenYPeriod = DconD.dT1_y;
	m_dEigenPeriod = max(DconD.dT1_x, DconD.dT1_y);
}

BOOL CDgnForceCtrl::IsApplyEqSpecialForceRCBeam()
{
	return (m_bSubBeamForNoMpr || m_iCantilever > 0 || m_bLowMembForNoMpr) ? FALSE : TRUE;
}

BOOL CDgnForceCtrl::IsApplyEqSpecialForceRCColm()
{
	return (m_bLowMembForNoMpr) ? FALSE : TRUE;
}

BOOL CDgnForceCtrl::IsNTC2018()
{
	if (m_iDgnCode != Eurocode2_04) return FALSE;
	if (m_iDgnSubCode != ITALY)     return FALSE;
	if (!m_bNAEQDgn)                return FALSE;
	return m_nNTCType == RCS_NTC18 ? TRUE : FALSE;
}

BOOL CDgnForceCtrl::IsApplyNonDissipative()
{
	if (!IsNTC2018()) { return FALSE; }
	if (!m_bEqSpecial) { return FALSE; }
	return TRUE;
}

// PMS:4182-Seungjun-20100608 KBC2009 Seismic design.
BOOL CDgnForceCtrl::IsSubBeam()
{
	return (m_bAutoSeisSubBeam && m_bSubBeamForNoMpr) ? TRUE : FALSE;
}

// PMS:4182-Seungjun-20100608 KBC2009 Seismic design.
BOOL CDgnForceCtrl::IsCaltileverBeam()
{
	return (m_bAutoSeisCantilever && m_iCantilever > 0) ? TRUE : FALSE;
}

// PMS:4182-Seungjun-20100608 KBC2009 Seismic design.
BOOL CDgnForceCtrl::IsUnderGroungElem()
{
	return (m_bAutoSeisUnderBeamColm && m_bLowMembForNoMpr) ? TRUE : FALSE;
}

BOOL CDgnForceCtrl::IsApplyEqSpecialForce(T_ELEM_K ElemK)
{
	//BOOL rVal = FALSE;
	//return (m_arSecondaryElemList.Lookup(ElemK, rVal))? FALSE : TRUE;    
	if (m_iDgnCode == Eurocode2_04)
	{
		if (m_iElemKind == D_MBTP_COLUMN)
		{
			return (IsUnderGroungElem() ? FALSE : TRUE);
		}
		else if (m_iElemKind == D_MBTP_BEAM)
		{
			return (IsUnderGroungElem() || IsCaltileverBeam() || IsSubBeam()) ? FALSE : TRUE;
		}
		return FALSE;
	}
	else if (m_iDgnCode == IS456_2000)
	{
		if (m_iElemKind == D_MBTP_COLUMN)
		{
			return (IsUnderGroungElem() ? FALSE : TRUE);
		}
		else if (m_iElemKind == D_MBTP_BEAM)
		{
			return (IsUnderGroungElem() || IsCaltileverBeam() || IsSubBeam()) ? FALSE : TRUE;
		}
		return FALSE;
	}
	else
	{
		return IsEqSpecialElem(ElemK);
	}
}

int CDgnForceCtrl::GetStoryCountAboveGround()
{
	double dGroundLevel = 0.0;
	T_BLDC_D BldcD;
	BldcD.Initialize();

	if (m_pDoc->m_pAttrCtrl->GetBldc(BldcD))
	{
		dGroundLevel = BldcD.bUseBaseLevel ? BldcD.dBaseLevel : 0.0;
	}

	// Check Story Level.  
	double dFactUnit = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_BASE_LENGTH, 1.0);
	double dZero_Lim = 1.0E-07*dFactUnit; // 1.0e-7m.
	int iStoryCount = 0;
	T_STOR_K StorK;
	T_STOR_D StorD;
	POSITION Pos = m_pDoc->m_pAttrCtrl->GetStartStor();
	while (Pos)
	{
		StorD.Initialize();
		m_pDoc->m_pAttrCtrl->GetNextStor(Pos, StorK, StorD);
		if (StorD.dStoryLevel > dGroundLevel + dZero_Lim) iStoryCount++;
	}
	return iStoryCount;
}

void CDgnForceCtrl::GetExpandEqFactor(T_ELEM_K ElemK, int nPosi, CString strLcaseType, double dFactor, double& dExFactor_P, double& dExFactor_V, double& dExFactor_M, double& dExFactor_T)
{
	dExFactor_P = dExFactor_V = dExFactor_M = dExFactor_T = m_bExistLctb ? 1.0 : dFactor;

	if (!m_ExpandElemD.bIsExpand)      return;
	if (m_ExpandElemD.ElemK != ElemK)  return;

	if (!CDBLib::IsStldEL(strLcaseType)) return;

	int nIMJ;
	if (nPosi == 0) nIMJ = 0;
	else if (nPosi == 4) nIMJ = 2;
	else              nIMJ = 1;

	if (m_ExpandElemD.bExPosi[nIMJ])
	{
		if (m_ExpandElemD.nExpandType[nIMJ] == 0) dExFactor_P = dExFactor_V = dExFactor_M = dExFactor_T = dFactor * m_ExpandElemD.dFactor[nIMJ];
		else                                     dExFactor_P = dExFactor_V = dExFactor_M = dExFactor_T = (dFactor >= 0.0 ? 1.0 : -1.0) * m_ExpandElemD.dFactor[nIMJ];
	}

	return;
}

void CDgnForceCtrl::GetRSFactor(const T_DCON_D& Dcon, T_ELEM_K ElemK, int iLcaseType, CString strLcaseType, double& dRFx, double& dRFy, double& dRFz, double& dRMx, double& dRMy, double& dRMz)
{
	dRFx = dRFy = dRFz = dRMx = dRMy = dRMz = 1.0;

	switch (m_iDgnCode)
	{
	case AASHTO_LC24:
	case AASHTO_LC20:
	case AASHTO_LC17:
	case AASHTO_LC16:
	case AASHTO_LC12:
	//case AASHTO_LC07:
	//case AASHTO_LC96:
		break;
	default:
		return;
	}

	if ( Dcon.nClass == 0) return; // 0 = seismic zone 1

	switch (iLcaseType)
	{
	case D_LOADCASE_SPECTRUM:
		break;
	case D_LOADCASE_STATIC:
	{
		if (!CDBLib::IsStldEL(strLcaseType)) return;
		break;
	}		
	default:
		return;
	}	

	T_RMFR_D RmfrD; RmfrD.Initialize();
	if (m_pDoc->m_pAttrCtrl->GetRmfr(ElemK, RmfrD))
	{
		dRFx = SafeDiv(1.0, RmfrD.dFx);
		dRFy = SafeDiv(1.0, RmfrD.dFy);
		dRFz = SafeDiv(1.0, RmfrD.dFz);
		dRMx = SafeDiv(1.0, RmfrD.dMx);
		dRMy = SafeDiv(1.0, RmfrD.dMy);
		dRMz = SafeDiv(1.0, RmfrD.dMz);
	}
}

void CDgnForceCtrl::SetExpandElem(_DGN_EXPAND_LOAD_ELEM& ExpandElemD)
{
	m_ExpandElemD = ExpandElemD;
}
void CDgnForceCtrl::InitExpandElem()
{
	m_ExpandElemD.Initialize();
}

void CDgnForceCtrl::Make_ExpandLoad()
{
	Init_ExpandLoad();
	_DGN_EXPAND_LOAD_D_FOR_SERV_AGEN ExpandLoad_SA;	ExpandLoad_SA.Initialize();
	ExpandLoad_SA.nLcomType = D_LCOMTYPE_STEEL;
	ExpandLoad_SA.bUserElem = FALSE;
	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
	if (pServiceHeadOffice->ReqService(_ULS(jud), D_JUD_GET_EXPANDLOAD, (void*)&ExpandLoad_SA) == 0)
	{
		m_ExpandLoad = ExpandLoad_SA.ExpandLoadD;
	}
}
void CDgnForceCtrl::Make_ExpandLoad_Elem(T_ELEM_K ElemK)
{
	Init_ExpandLoad();
	_DGN_EXPAND_LOAD_D_FOR_SERV_AGEN ExpandLoad_SA;	ExpandLoad_SA.Initialize();
	ExpandLoad_SA.nLcomType = D_LCOMTYPE_STEEL;
	ExpandLoad_SA.bUserElem = TRUE;
	ExpandLoad_SA.arElemK.Add(ElemK);
	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
	if (pServiceHeadOffice->ReqService(_ULS(jud), D_JUD_GET_EXPANDLOAD, (void*)&ExpandLoad_SA) == 0)
	{
		m_ExpandLoad = ExpandLoad_SA.ExpandLoadD;
	}
}
void CDgnForceCtrl::Init_ExpandLoad()
{
	m_ExpandLoad.Initialize();
}
BOOL CDgnForceCtrl::Get_ExpandLoadElemD(T_LCOM_K LcomK, T_ELEM_K ElemK, _DGN_EXPAND_LOAD_ELEM& ExpandElemD)
{
	ExpandElemD.Initialize();
	if (!m_ExpandLoad.bIsExpand) return FALSE;
	_DGN_EXPAND_LOAD_LCOM ExpandLcomD;
	if (!m_ExpandLoad.arLcomList.Lookup(LcomK, ExpandLcomD)) return FALSE;
	if (!ExpandLcomD.bIsExpand) return FALSE;
	if (!ExpandLcomD.arElemList.Lookup(ElemK, ExpandElemD)) return FALSE;
	return TRUE;
}

// PMS:4215, PMS:4230-Seungjun-20110411 일본 2차 설계 위치별 부재력
double CDgnForceCtrl::Get_MomentInterpolate(double dM1, double dM2, double dMmid, double dLx, double dL)
{
	if (dL <= 0.0) return (dM1 + dM2 / 2.0);
	double dM0 = (dM1 + dM2) / 2.0 - dMmid;
	return (dM1 + (dM2 - dM1 - 4.0*dM0)*dLx / dL + 4.0*dM0*(dLx*dLx) / (dL*dL));
}

double CDgnForceCtrl::Get_MomentInterpolate(double dM[5], double dL[5], double dPosiLen)
{
	if (dPosiLen <= dL[0]) return dM[0];
	if (dPosiLen >= dL[4]) return dM[4];
	if (dPosiLen <= dL[1]) return Get_MomentInterpolate(dM[0], dM[2], dM[1], (dPosiLen - dL[0]), (dL[2] - dL[0]));
	if (dPosiLen >= dL[3]) return Get_MomentInterpolate(dM[2], dM[4], dM[3], (dPosiLen - dL[2]), (dL[4] - dL[2]));
	return Get_MomentInterpolate(dM[1], dM[3], dM[2], (dPosiLen - dL[1]), (dL[3] - dL[1]));
}

void CDgnForceCtrl::Set_JUDLengType(int ElemKey)
{
	Init_LengType();
	if (!Get_JUDLengType(ElemKey, m_nMLengTypei, m_nMLengTypej))
	{
		Init_LengType();  return;
	}

	T_LENG_D LengD; LengD.Initialize();
	double dLen = 0., dNetLy = 0., dNetLz = 0.;
	double dOffSetLy[2] = { 0. }, dOffSetLz[2] = { 0. }; // 0:i, 1:j

	for (int i = 0; i < 2; i++)
	{
		if (m_pDgnDataCtrl->Get_DgnGenLeng(ElemKey, 0, LengD, dLen, dNetLy, dNetLz, dOffSetLy, dOffSetLz, m_nMLengTypei[i], m_nMLengTypej[i]))
		{
			m_dOffSetRyi[i] = dgn::LE0(dNetLy) ? 0.0 : dOffSetLy[0] / dNetLy;
			m_dOffSetRzi[i] = dgn::LE0(dNetLz) ? 0.0 : dOffSetLz[0] / dNetLz;
		}
		if (m_pDgnDataCtrl->Get_DgnGenLeng(ElemKey, 4, LengD, dLen, dNetLy, dNetLz, dOffSetLy, dOffSetLz, m_nMLengTypei[i], m_nMLengTypej[i]))
		{
			m_dOffSetRyj[i] = dgn::LE0(dNetLy) ? 0.0 : dOffSetLy[1] / dNetLy;
			m_dOffSetRzj[i] = dgn::LE0(dNetLz) ? 0.0 : dOffSetLz[1] / dNetLz;
		}
	}
}

BOOL CDgnForceCtrl::Get_JUDLengType(int ElemKey, int nMLengTypei[2], int nMLengTypej[2])
{
	nMLengTypei[0] = nMLengTypej[0] = nMLengTypei[1] = nMLengTypej[1] = 0;

	T_JDSC_D JdscD;  JdscD.Initialize();
	if (!m_pDoc->m_pAttrCtrl2->GetJdsc(JdscD)) JdscD.Initialize();

	T_DCON_D DconD;  DconD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetDcon(DconD)) DconD.Initialize();

	T_ELEM_D ElemD;
	int nMembType = -1;
	CString strMatType = _T("");
	if (m_pDoc->m_pAttrCtrl->GetElem(ElemKey, ElemD))
	{
		if (ElemD.eltyp == WALL_EL) nMembType = D_MBTP_WALL;
		else
		{
			if (m_pDoc->m_pAttrCtrl->ExistMbtp(ElemKey))
			{
				T_MBTP_D MbtpD;
				if (!m_pDoc->m_pAttrCtrl->GetMbtp(ElemKey, MbtpD)) { ASSERT(0); MbtpD.Initialize(); }
				nMembType = MbtpD.nMbType;
			}
			else
			{
				nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(ElemKey);
			}
		}

		T_MATD_D MatdD;
		MatdD.Initialize();
		if (nMembType == D_MBTP_WALL)
		{
			T_WALL_KEY WallKey;
			m_pDoc->m_pPostCtrl->GetWallKey(ElemKey, WallKey);
			int nMatl = 0;
			GetDgnWallMatd(WallKey, nMatl, MatdD);
		}
		else
		{
			GetDgnElemMatd(ElemKey, MatdD);
		}
		strMatType = MatdD.Type;	// S, C, SRC, USER
	}

	if (nMembType == D_MBTP_BEAM)
	{
		if (strMatType == _T("C") || strMatType == _T("SRC"))
		{
			nMLengTypei[0] = nMLengTypej[0] = DconD.nBeamEndLocRC;
			nMLengTypei[1] = nMLengTypej[1] = DconD.nBeamEndLocRC_ST;
		}
		else if (strMatType == _T("S"))
		{
			nMLengTypei[0] = nMLengTypej[0] = JdscD.nBeamEndLocS;
			nMLengTypei[1] = nMLengTypej[1] = JdscD.nBeamEndLocS_ST;
		}
	}
	else if (nMembType == D_MBTP_COLUMN)
	{
		if (strMatType == _T("C") || strMatType == _T("SRC"))
		{
			nMLengTypei[0] = nMLengTypej[0] = DconD.nColmEndLocRC;
			nMLengTypei[1] = nMLengTypej[1] = DconD.nColmEndLocRC_ST;
		}
		else if (strMatType == _T("S"))
		{
			nMLengTypei[0] = nMLengTypej[0] = JdscD.nColmEndLocS;
			nMLengTypei[1] = nMLengTypej[1] = JdscD.nColmEndLocS_ST;
		}

		T_BPLT_K BpltKey;
		if (m_pDoc->m_pAttrCtrl2->GetBpltKeyByElemKey(ElemKey, BpltKey))
		{
			// Add/Modify by Seungjun MQC:4762 '20120704 노출주각이고 강역길이가 고려될 경우 검정위치는 무조건 강역단으로...
			T_BPLT_D BpltD;
			m_pDoc->m_pAttrCtrl2->GetBplt(BpltKey, BpltD);
			BOOL bRigid = (BpltD.nType != 0/*BPLT_TYPE_None*/ && BpltD.dRigidLength > 0.0) ? TRUE : FALSE;

			int nBpltIJ = 0;
			if (m_pDoc->m_pAttrCtrl2->GetBpltPosi(BpltKey, nBpltIJ))
			{
				if (strMatType == _T("C") || strMatType == _T("SRC"))
				{
					if (nBpltIJ == 0)
					{
						nMLengTypei[0] = bRigid ? 0 : JdscD.nPedtEndLocRC;
						nMLengTypei[1] = bRigid ? 0 : JdscD.nPedtEndLocRC_ST;
					}
					else
					{
						nMLengTypej[0] = bRigid ? 0 : JdscD.nPedtEndLocRC;
						nMLengTypej[1] = bRigid ? 0 : JdscD.nPedtEndLocRC_ST;
					}
				}
				else if (strMatType == _T("S"))
				{
					if (nBpltIJ == 0)
					{
						nMLengTypei[0] = bRigid ? 0 : JdscD.nPedtEndLocS;
						nMLengTypei[1] = bRigid ? 0 : JdscD.nPedtEndLocS_ST;
					}
					else
					{
						nMLengTypej[0] = bRigid ? 0 : JdscD.nPedtEndLocS;
						nMLengTypej[1] = bRigid ? 0 : JdscD.nPedtEndLocS_ST;
					}
				}
			}
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

void CDgnForceCtrl::Get_SeisLcomForceFactor_Beam_GB50010_10(int nSubType, int nSeisGrade, double& dCodeLcomN, double& dCodeLcomM, double& dCodeLcomV)
{
	dCodeLcomN = 1.0;
	dCodeLcomM = 1.0;
	dCodeLcomV = 1.0;

	// See 2010_신기준_내력조정계수_고공.xlsx (Building 적용 계수)
	/// by GAY. ('14.10.16). MQC:8801.  후효무가 보내준 내력조정계수.xlsx 파일로 반영 요청.
	BOOL bFortification9 = Get_SeisFortificationMax() == 5 ? TRUE : FALSE;
	if (bFortification9)
	{
		if (nSubType == 0)	// Frame Beam.
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.56; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3;  } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3;  } break;
			default: break;
			}
		}
		else if (nSubType == 1)	// Lintel Beam.
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3; } break;
			default: break;
			}
		}
		else if (nSubType == 2) // Beam Supporting Frame. (consider factor for seismic load _T("CASE") not load combination)
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			default: break;
			}
		}
		else	ASSERT(0);
	}
	else
	{
		if (nSubType == 0)	// Frame Beam.
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.56; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3;  } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3;  } break;
			case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.2;  } break;
			case EN_GB_SEISGRADE_GRADE3: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.1;  } break;
			default: break;
			}
		}
		else if (nSubType == 1)	// Lintel Beam.
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.3; } break;
			case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.2; } break;
			case EN_GB_SEISGRADE_GRADE3: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.1; } break;
			default: break;
			}
		}
		else if (nSubType == 2) // Beam Supporting Frame. (consider factor for seismic load _T("CASE") not load combination)
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			default: break;
			}
		}
		else	ASSERT(0);
	}
}

void CDgnForceCtrl::Get_SeisLcomForceFactor_Colm_GB50010_10(int nSubType, int nSeisGrade, BOOL bFrameStruct, double& dCodeLcomN, double& dCodeLcomM, double& dCodeLcomV)
{
	dCodeLcomN = 1.0;
	dCodeLcomM = 1.0;
	dCodeLcomV = 1.0;

	// See 2010_신기준_내력조정계수_고공.xlsx (Building 적용 계수)
	/// by GAY. ('14.10.16). MQC:8801.  후효무가 보내준 내력조정계수.xlsx 파일로 반영 요청.
	BOOL bFortification9 = Get_SeisFortificationMax() == 5 ? TRUE : FALSE;

	if (bFortification9)
	{
		if (nSubType == 0 || nSubType == 2)	// Frame Column(0), Corner Column(2).
		{
			// same factor for frame structure and NON-frame structure.
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 2.04;  dCodeLcomV = 3.672; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.70;  dCodeLcomV = 2.55;  } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.70;  dCodeLcomV = 2.55;  } break;
			default: break;
			}
			dCodeLcomM *= (nSubType == 2 && nSeisGrade <= 6 ? 1.1 : 1.0);
			dCodeLcomV *= (nSubType == 2 && nSeisGrade <= 6 ? 1.1 : 1.0);
		}
		else if (nSubType == 1 || nSubType == 3)	// Lower Frame Column(1), Lower Corner Column(3).
		{
			// same factor for frame structure and NON-frame structure.
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 2.04;  dCodeLcomV = 3.672; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.70;  dCodeLcomV = 2.55;  } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.70;  dCodeLcomV = 2.55;  } break;
			default: break;
			}
			dCodeLcomM *= (nSubType == 3 && nSeisGrade <= 6 ? 1.1 : 1.0);
			dCodeLcomV *= (nSubType == 3 && nSeisGrade <= 6 ? 1.1 : 1.0);
		}
		else if (nSubType == 4)	// Column Supporting Frame(4).
		{
			// not applied factor for fortification 9.
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;  dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;  dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;  dCodeLcomV = 1.0; } break;
			default: break;
			}
		}
		else	ASSERT(0);
	}
	else
	{
		if (nSubType == 0 || nSubType == 2)	// Frame Column(0), Corner Column(2).
		{
			if (bFrameStruct) // Frame 구조형식인 경우.
			{
				switch (nSeisGrade)
				{
				case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 2.04;   dCodeLcomV = 3.672; } break;
				case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.70;   dCodeLcomV = 2.55;  } break;
				case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.70;   dCodeLcomV = 2.55;  } break;
				case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.50;   dCodeLcomV = 1.95;  } break;
				case EN_GB_SEISGRADE_GRADE3: { dCodeLcomN = 1.0;   dCodeLcomM = 1.30;   dCodeLcomV = 1.56;  } break;
				case EN_GB_SEISGRADE_GRADE4: { dCodeLcomN = 1.0;   dCodeLcomM = 1.20;   dCodeLcomV = 1.32;  } break;
				default: break;
				}
			}
			else // 기타 구조형식인 경우.
			{
				switch (nSeisGrade)
				{
				case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.68;   dCodeLcomV = 2.822; } break;
				case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.70;   dCodeLcomV = 2.55;  } break;
				case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.40;   dCodeLcomV = 1.96;  } break;
				case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.20;   dCodeLcomV = 1.44;  } break;
				case EN_GB_SEISGRADE_GRADE3: { dCodeLcomN = 1.0;   dCodeLcomM = 1.10;   dCodeLcomV = 1.21;  } break;
				case EN_GB_SEISGRADE_GRADE4: { dCodeLcomN = 1.0;   dCodeLcomM = 1.10;   dCodeLcomV = 1.21;  } break;
				default: break;
				}
			}
			dCodeLcomM *= (nSubType == 2 && nSeisGrade <= 6 ? 1.1 : 1.0);
			dCodeLcomV *= (nSubType == 2 && nSeisGrade <= 6 ? 1.1 : 1.0);
		}
		else if (nSubType == 1 || nSubType == 3)	// Lower Frame Column(1), Lower Corner Column(3).
		{
			if (bFrameStruct) // Frame 구조형식인 경우.
			{
				switch (nSeisGrade)
				{
				case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 2.04;   dCodeLcomV = 3.672; } break;
				case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.70;   dCodeLcomV = 2.55;  } break;
				case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.70;   dCodeLcomV = 2.55;  } break;
				case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.50;   dCodeLcomV = 1.95;  } break;
				case EN_GB_SEISGRADE_GRADE3: { dCodeLcomN = 1.0;   dCodeLcomM = 1.30;   dCodeLcomV = 1.56;  } break;
				case EN_GB_SEISGRADE_GRADE4: { dCodeLcomN = 1.0;   dCodeLcomM = 1.20;   dCodeLcomV = 1.32;  } break;
				default: break;
				}
			}
			else // 기타 구조형식인 경우.
			{
				switch (nSeisGrade)
				{
				case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.68;   dCodeLcomV = 2.822; } break;
				case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.70;   dCodeLcomV = 2.55;  } break;
				case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.40;   dCodeLcomV = 1.96;  } break;
				case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.20;   dCodeLcomV = 1.44;  } break;
				case EN_GB_SEISGRADE_GRADE3: { dCodeLcomN = 1.0;   dCodeLcomM = 1.10;   dCodeLcomV = 1.21;  } break;
				case EN_GB_SEISGRADE_GRADE4: { dCodeLcomN = 1.0;   dCodeLcomM = 1.10;   dCodeLcomV = 1.21;  } break;
				default: break;
				}
			}
			dCodeLcomM *= (nSubType == 3 && nSeisGrade <= 6 ? 1.1 : 1.0);
			dCodeLcomV *= (nSubType == 3 && nSeisGrade <= 6 ? 1.1 : 1.0);
		}
		else if (nSubType == 4)	// Column Supporting Frame(4).
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.68;   dCodeLcomV = 2.822; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.40;   dCodeLcomV = 1.960; } break;
			case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.20;   dCodeLcomV = 1.440; } break;
			default: break;
			}
		}
		else	ASSERT(0);
	}
}

void CDgnForceCtrl::Get_SeisLcomForceFactor_Wall_GB50010_10(int nSubType, int nSeisGrade, double& dCodeLcomN, double& dCodeLcomM, double& dCodeLcomV)
{
	dCodeLcomN = 1.0;
	dCodeLcomM = 1.0;
	dCodeLcomV = 1.0;

	BOOL bFortification9 = Get_SeisFortificationMax() == 5 ? TRUE : FALSE;

	// See 2010_신기준_내력조정계수_고공.xlsx (Building 적용 계수)
	/// by GAY. ('14.10.16). MQC:8801.  후효무가 보내준 내력조정계수.xlsx 파일로 반영 요청.
	if (bFortification9)
	{
		if (nSubType == 0)	// General Shear Wall (Strengthened part).
		{
			// See [GB/T50010-10, 6.2.8], [JGJ3-10, 7.2.6]
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.1;   dCodeLcomV = 1.9; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.6; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.6; } break;
			default: break;
			}
		}
		else if (nSubType == 1)	// 1=General Shear Wall (Ordinary part)
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.3;   dCodeLcomV = 1.4; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.2;   dCodeLcomV = 1.3; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.2;   dCodeLcomV = 1.3; } break;
			default: break;
			}
		}
		else if (nSubType == 2)	// Shear Wall at Complicated Tall Building (Strengthened part).
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			default: break;
			}
		}
		else if (nSubType == 3) //  3=Shear Wall at Complicated Tall Building (Ordinary part).
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			default: break;
			}
		}
		else if (nSubType == 4) // Short Wall (Strengthened Part).
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.1;   dCodeLcomV = 1.9; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.6; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.6; } break;
			default: break;
			}
		}
		else if (nSubType == 5) // Short Wall (Ordinary Part).
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.3;   dCodeLcomV = 1.4; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.2;   dCodeLcomV = 1.4; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.2;   dCodeLcomV = 1.4; } break;
			default: break;
			}
		}
		else	ASSERT(0);
	}
	else
	{
		if (nSubType == 0)	// General Shear Wall (Strengthened part).
		{
			// See [GB/T50010-10, 6.2.8], [JGJ3-10, 7.2.6]
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.1;   dCodeLcomV = 1.9; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.6; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.6; } break;
			case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.4; } break;
			case EN_GB_SEISGRADE_GRADE3: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.2; } break;
			default: break;
			}
		}
		else if (nSubType == 1)	// 1=General Shear Wall (Ordinary part)
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.3;   dCodeLcomV = 1.4; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.2;   dCodeLcomV = 1.3; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.2;   dCodeLcomV = 1.3; } break;
			case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_GRADE3: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			default: break;
			}
		}
		else if (nSubType == 2)	// Shear Wall at Complicated Tall Building (Strengthened part).
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.8;   dCodeLcomV = 1.9; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.6; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.5;   dCodeLcomV = 1.6; } break;
			case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.3;   dCodeLcomV = 1.4; } break;
			case EN_GB_SEISGRADE_GRADE3: { dCodeLcomN = 1.0;   dCodeLcomM = 1.1;   dCodeLcomV = 1.2; } break;
			default: break;
			}
		}
		else if (nSubType == 3) //  3=Shear Wall at Complicated Tall Building (Ordinary part).
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.3;   dCodeLcomV = 1.4; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.2;   dCodeLcomV = 1.3; } break;
			case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_GRADE3: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			default: break;
			}
		}
		else if (nSubType == 4) // Short Wall (Strengthened Part).
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.1;   dCodeLcomV = 1.9; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.0; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.6; } break;
			case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.4; } break;
			case EN_GB_SEISGRADE_GRADE3: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.2; } break;
			default: break;
			}
		}
		else if (nSubType == 5) // Short Wall (Ordinary Part).
		{
			switch (nSeisGrade)
			{
			case EN_GB_SEISGRADE_SPECIAL_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.3;   dCodeLcomV = 1.4; } break;
			case EN_GB_SEISGRADE_FORT9_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.2;   dCodeLcomV = 1.4; } break;
			case EN_GB_SEISGRADE_GRADE1: { dCodeLcomN = 1.0;   dCodeLcomM = 1.2;   dCodeLcomV = 1.4; } break;
			case EN_GB_SEISGRADE_GRADE2: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.2; } break;
			case EN_GB_SEISGRADE_GRADE3: { dCodeLcomN = 1.0;   dCodeLcomM = 1.0;   dCodeLcomV = 1.1; } break;
			default: break;
			}
		}
		else	ASSERT(0);
	}
}

void CDgnForceCtrl::Get_SeisLcasForceFactor_BeamSupportFrame_GB50010_10(int nSeisGrade, double& dCodeLcasN, double& dCodeLcasM, double& dCodeLcasV)
{
	dCodeLcasN = 1.0;
	dCodeLcasM = 1.0;
	dCodeLcasV = 1.0;

	BOOL bFortification9 = Get_SeisFortificationMax() == 5 ? TRUE : FALSE;
	if (!bFortification9)
	{
		switch (nSeisGrade)
		{
		case 1: dCodeLcasN = 1.9; dCodeLcasM = 1.9; dCodeLcasV = 1.9; break; // special grade 1.        
		case 3: dCodeLcasN = 1.6; dCodeLcasM = 1.6; dCodeLcasV = 1.6; break; // grade 1.
		case 4: dCodeLcasN = 1.3; dCodeLcasM = 1.3; dCodeLcasV = 1.3; break; // grade 2.
		default: break;
		}
	}
}

void CDgnForceCtrl::Get_SeisLcasForceFactor_ColmSupportFrame_GB50010_10(int nSeisGrade, double& dCodeLcasN)
{
	dCodeLcasN = 1.0;

	BOOL bFortification9 = Get_SeisFortificationMax() == 5 ? TRUE : FALSE;
	if (!bFortification9)
	{
		switch (nSeisGrade)
		{
		case 1: dCodeLcasN = 1.8; break; // special grade 1.        
		case 3: dCodeLcasN = 1.5; break; // grade 1.
		case 4: dCodeLcasN = 1.2; break; // grade 2.
		default: break;
		}
	}
}

UINT CDgnForceCtrl::Get_SeisFortificationMax()
{
	UINT unFortification = 0;

	// get maximum fortification grade.    
	int nSizeSeis = m_pDoc->m_pAttrCtrl->GetCountSeis();
	if (nSizeSeis > 0)
	{
		T_SEIS_K SeisK;
		T_SEIS_D SeisD;
		POSITION pos = m_pDoc->m_pAttrCtrl->GetStartSeis();
		while (pos != nullptr)
		{
			m_pDoc->m_pAttrCtrl->GetNextSeis(pos, SeisK, SeisD);
			switch (SeisD.nCodeType)
			{
			case CH_E_2001: { unFortification = max(unFortification, SeisD.CH2002.nSeisFortification);   } break;
			case CHSH_E_2003: { unFortification = max(unFortification, SeisD.CHSH2003.nSeisFortification); } break;
			case CH_E_2010: { unFortification = max(unFortification, SeisD.CH2010.nSeisFortification);   } break;
			default: ASSERT(0); break;
			}
		}
	}

	T_SPFC_D SpfcD;
	int nSizeSpec = m_pDoc->m_pAttrCtrl->GetCountSpfc();
	if (nSizeSpec > 0)
	{
		T_SPFC_K SpfcK;
		T_SPFC_D SpfcD;
		POSITION pos = m_pDoc->m_pAttrCtrl->GetStartSpfc();
		while (pos != nullptr)
		{
			m_pDoc->m_pAttrCtrl->GetNextSpfc(pos, SpfcK, SpfcD);
			switch (SpfcD.nSpecCodeType)
			{
			case D_SPFC_CODE_CH2002: { unFortification = max(unFortification, SpfcD.CodeParam.CH2002.nSeisFortification);   } break;
			case D_SPFC_CODE_CHSH2003: { unFortification = max(unFortification, SpfcD.CodeParam.CHSH2003.nSeisFortification); } break;
			case D_SPFC_CODE_CH2010: { unFortification = max(unFortification, SpfcD.CodeParam.CH2010.nSeisFortification);   } break;
			case D_SPFC_CODE_CH_GB_T_51408_2021: { unFortification = max(unFortification, SpfcD.CodeParam.GB_T_51408_2021.nSeisFortification);   } break;
			default: /*ASSERT(0);*/ break;
			}
		}
	}

	return unFortification;
}

BOOL CDgnForceCtrl::IsFrameStructure_GB10()
{
	BOOL bFrameStruct = FALSE;

	T_DCTL_D DctlD;
	if (!m_pDgnDataCtrl->Get_DgnGenDctl(DctlD)) DctlD.Initialize();

	int nStructMatlType = DctlD.nStructMatlType;
	int nStructType = DctlD.nStructType;
	if (nStructMatlType == 0) // 철근콘크리트조.
	{
		if (nStructType == 0) bFrameStruct = TRUE;
	}
	else if (nStructMatlType == 1) // 철골철근콘크리트조(혼합구조).
	{
	}
	else if (nStructMatlType == 2 || nStructMatlType == 3) // 강구조.
	{
		if (nStructType == 0) bFrameStruct = TRUE;
	}
	else ASSERT(0);

	return bFrameStruct;
}

void CDgnForceCtrl::GetAnalForceComponentIndex(const int &nPosi, int &rnPart, int &rnDOF6, int &rnDOF7)
{
	if (nPosi < 4) { rnPart = nPosi; rnDOF6 = 0; rnDOF7 = 12; }
	else { rnPart = nPosi - 1; rnDOF6 = 6; rnDOF7 = 13; }

	//─────┬─────────────────────────────
	// Component│    I=0   │   1/4=1  │  2/4=2   │  3/4=3   │   J=4
	//───────────────────────────────────
	//     Fx   │ 0  │ 0  │ 1  │ 0  │ 2  │ 0  │ 3  │ 0  │ 3  │ 6  
	//     Fy   │ 0  │ 1  │ 1  │ 1  │ 2  │ 1  │ 3  │ 1  │ 3  │ 7  
	//     Fz   │ 0  │ 2  │ 1  │ 2  │ 2  │ 2  │ 3  │ 2  │ 3  │ 8  
	//    BMx   │ 0  │ 3  │ 1  │ 3  │ 2  │ 3  │ 3  │ 3  │ 3  │ 9  
	//    BMy   │ 0  │ 4  │ 1  │ 4  │ 2  │ 4  │ 3  │ 4  │ 3  │10  
	//    BMz   │ 0  │ 5  │ 1  │ 5  │ 2  │ 5  │ 3  │ 5  │ 3  │11  
	//───────────────────────────────────
	//     W    │ 0  │12  │ 1  │12  │ 2  │12  │ 3  │12  │ 3  │13
	//     Mt   │ 0  │14  │ 1  │14  │ 2  │14  │ 3  │14  │ 3  │15  
	//     Mw   │ 0  │16  │ 1  │16  │ 2  │16  │ 3  │16  │ 3  │17  	
	//───────────────────────────────────
}

BOOL CDgnForceCtrl::IsElem7DOF(const T_ELEM_K &ElemK)
{
	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }
	if (!m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp)) { return FALSE; }

	T_SECT_D SectD;
	if (m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD))
	{
		return SectD.SectBefore.bConsiderWarpingEffect ? TRUE : FALSE;
	}

	return FALSE;
}

void CDgnForceCtrl::GetLcomCompTypeString(int nDgnNo, CArray<CString, CString&> &raCompString)
{
	raCompString.RemoveAll();

	if (nDgnNo == D_RC_PLATE_BC)
	{
		CString strComponent = _T("FX");	raCompString.Add(strComponent);
		strComponent = _T("FY");  raCompString.Add(strComponent);
		strComponent = _T("FXY"); raCompString.Add(strComponent);
		strComponent = _T("MX");  raCompString.Add(strComponent);
		strComponent = _T("MY");  raCompString.Add(strComponent);
		strComponent = _T("MXY"); raCompString.Add(strComponent);
		strComponent = _T("VX");  raCompString.Add(strComponent);
		strComponent = _T("VY");  raCompString.Add(strComponent);
	}
	else
	{
		CString strComponent = _T("FX");	raCompString.Add(strComponent);
		strComponent = _T("FY");  raCompString.Add(strComponent);
		strComponent = _T("FZ");  raCompString.Add(strComponent);
		strComponent = _T("MX");  raCompString.Add(strComponent);
		strComponent = _T("MY");  raCompString.Add(strComponent);
		strComponent = _T("MZ");  raCompString.Add(strComponent);
		strComponent = _T("Mbi"); raCompString.Add(strComponent);
		strComponent = _T("Tst"); raCompString.Add(strComponent);
		strComponent = _T("Twp"); raCompString.Add(strComponent);
		strComponent = _T("SX");  raCompString.Add(strComponent);
	}
}

BOOL CDgnForceCtrl::IsRegardZeroTorsion(const double *pdTorsion, const int &nPosi, const double &dLimit, const double &dTolZero)
{
	if (fabs(pdTorsion[nPosi]) < dTolZero) return  TRUE;
	if (fabs(pdTorsion[nPosi]) < dLimit*max(fabs(pdTorsion[0]), fabs(pdTorsion[CONST_DGN_iPOSNUM - 1]))) return TRUE;

	return FALSE;
}

BOOL CDgnForceCtrl::IsRegardZeroForce(const double *pdForce, const int &nPosi, const double &dLimit, const double &dTolZero)
{
	if (fabs(pdForce[nPosi]) < dTolZero) return TRUE;
	if (fabs(pdForce[nPosi]) < dLimit*max(fabs(pdForce[0]), fabs(pdForce[CONST_DGN_iPOSNUM - 1]))) return TRUE;

	return FALSE;
}

BOOL CDgnForceCtrl::IsRegardZeroMoment(const double &dValue, const int &nPosi, const double &dLimit, const double &dTolZero, const double &dValueOrtho)
{
	// Change by ZINU.('04.06.11). Set Zero to MinMoment if My/Mz or Mz/My is under dDGN_Zero.
	if (fabs(dValue) < dTolZero) return TRUE;
	if (fabs(dValue) < dLimit*fabs(dValueOrtho)) return TRUE;

	return FALSE;
}

void CDgnForceCtrl::GetActiveStripList(T_MSTR_K_LIST &aDgnStripList)
{
	m_PlateKeyList.RemoveAll();

	int nDgnElem = aDgnStripList.GetSize();
	if (nDgnElem > 0)
	{
		m_PlateKeyList.Copy(aDgnStripList);
	}
	else
	{
		//m_pDoc->m_pViewCtrl->GetAllActiveMstr(m_PlateKeyList);
		m_pDgnDataCtrl->m_pMembCtrl->GetStripListForDgn(m_PlateKeyList);
	}

	m_pDoc->m_pPostCtrl->GetDesignResult()->SetDesignedMstrKList(m_PlateKeyList); // 설계할 때 사용했던 strip키를 뷰에서 사용
}

T_MSTR_K CDgnForceCtrl::GetMstrKByElemK(T_ELEM_K ElemK)
{
	T_ELEM_K_LIST aElemK;
	T_MSTR_K MstrK = 0;
	T_MSTR_D MstrD;   MstrD.Initialize();
	POSITION pos = m_pDoc->m_pAttrCtrl2->GetStartMstr();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetNextMstr(pos, MstrK, MstrD);
		int nElem = m_pDoc->m_pAttrCtrl->GetElemKByMemb(MstrD.MembK, aElemK);

		for (int i = 0; i < nElem; ++i)
		{
			if (aElemK[i] == ElemK) { return MstrK; }
		}
	}
	return 0;
}

void CDgnForceCtrl::Reset_RcplForce()
{
	m_Rcpl_Strp.Initialize();
	for (int i = 0; i < 2; i++)
	{
		m_Fxx_Rcpl[i] = 0.0;
		m_Fyy_Rcpl[i] = 0.0;
		m_Fxy_Rcpl[i] = 0.0;
		m_Vxx_Rcpl[i] = 0.0;
		m_Vyy_Rcpl[i] = 0.0;
		m_Mxx_Rcpl[i] = 0.0;
		m_Myy_Rcpl[i] = 0.0;
		m_Mxy_Rcpl[i] = 0.0;
		m_FxxDL_Rcpl[i] = 0.0;
		m_FyyDL_Rcpl[i] = 0.0;
		m_FxyDL_Rcpl[i] = 0.0;
		m_VxxDL_Rcpl[i] = 0.0;
		m_VyyDL_Rcpl[i] = 0.0;
		m_MxxDL_Rcpl[i] = 0.0;
		m_MyyDL_Rcpl[i] = 0.0;
		m_MxyDL_Rcpl[i] = 0.0;

		m_FxxML_Rcpl[i] = 0.0;
		m_FyyML_Rcpl[i] = 0.0;
		m_FxyML_Rcpl[i] = 0.0;
		m_VxxML_Rcpl[i] = 0.0;
		m_VyyML_Rcpl[i] = 0.0;
		m_MxxML_Rcpl[i] = 0.0;
		m_MyyML_Rcpl[i] = 0.0;
		m_MxyML_Rcpl[i] = 0.0;
	}
}

void CDgnForceCtrl::Reset_RrplForce()
{
	m_Rcpl_Strp.Initialize();
	for (int i = 0; i < 2; i++)
	{
		m_Fxx_Rcpl[i] = 0.0;
		m_Fyy_Rcpl[i] = 0.0;
		m_Fxy_Rcpl[i] = 0.0;
		m_Vxx_Rcpl[i] = 0.0;
		m_Vyy_Rcpl[i] = 0.0;
		m_Mxx_Rcpl[i] = 0.0;
		m_Myy_Rcpl[i] = 0.0;
		m_Mxy_Rcpl[i] = 0.0;
		m_FxxDL_Rcpl[i] = 0.0;
		m_FyyDL_Rcpl[i] = 0.0;
		m_FxyDL_Rcpl[i] = 0.0;
		m_VxxDL_Rcpl[i] = 0.0;
		m_VyyDL_Rcpl[i] = 0.0;
		m_MxxDL_Rcpl[i] = 0.0;
		m_MyyDL_Rcpl[i] = 0.0;
		m_MxyDL_Rcpl[i] = 0.0;
		m_FxxML_Rcpl[i] = 0.0;
		m_FyyML_Rcpl[i] = 0.0;
		m_FxyML_Rcpl[i] = 0.0;
		m_VxxML_Rcpl[i] = 0.0;
		m_VyyML_Rcpl[i] = 0.0;
		m_MxxML_Rcpl[i] = 0.0;
		m_MyyML_Rcpl[i] = 0.0;
		m_MxyML_Rcpl[i] = 0.0;
	}
}


void CDgnForceCtrl::Set_LcomDataForRcpl(int iRcpl)
{
	// Add by SHIN('07.06.28) MNET:2668 -> CDgnForceCtrl::Set_LcomDataForElem 대처용
	if (iRcpl == 0)
	{
		m_parLcomData = &m_arLcomTotl;
		// Calculate Load Combination Number for Design.
		m_iLCBDesignNum = m_parLcomData->GetCount();
		m_iLCBServiceNum = m_iLcomServ;
		m_iLCBAddDesignNum = m_iLCBDesignNum;
	}
	else
	{
		T_RCPL_D RcplD;
		if (!m_pDoc->m_pAttrCtrl->GetRcpl(iRcpl, RcplD))
		{
			RcplD.Initialize();
		}

		int nLcomAppMax = 0;
		const INT_PTR nSizeElem = RcplD.arElemK.GetSize();
		for (INT_PTR i = 0; i < nSizeElem; ++i)
		{
			const T_ELEM_K CurElemK = RcplD.arElemK[i];

			const EN_DGNLCOM_APP enLcomApp = GetAppDgnLcomTypeByMemb(CurElemK);
			nLcomAppMax = max(nLcomAppMax, static_cast<int>(enLcomApp));
		}

		// by SHIN('07.06.28) MNET:2668 : 현재 Civil RC Plate설계에서는 특별하중조합이 고려되지 않습니다. 해당 하중조합이 들어 가는 경우 확인 바랍니다.
		const EN_DGNLCOM_APP enLcomAppMax = static_cast<EN_DGNLCOM_APP>(nLcomAppMax);
		if (enLcomAppMax != EN_DGNLCOM_APP::STRN_SERV) { ASSERT(0); }

		m_parLcomData = GetAppDgnLcomMap(enLcomAppMax);
		m_iLCBDesignNum = GetSizeAppDgnLcomMap(enLcomAppMax);

		// Calculate Load Combination Number for Design.
		m_iLCBServiceNum = GetSizeAppDgnLcomMapServ(enLcomAppMax);
		m_iLCBAddDesignNum = m_iLCBDesignNum;
	}
}

void CDgnForceCtrl::Set_LcomDataForRrpl(int iRrpl)
{
	// Add by SHIN('07.06.28) MNET:2668 -> CDgnForceCtrl::Set_LcomDataForElem 대처용
	if (iRrpl == 0)
	{
		m_parLcomData = &m_arLcomTotl;
		// Calculate Load Combination Number for Design.
		m_iLCBDesignNum = m_parLcomData->GetCount();
		m_iLCBServiceNum = m_iLcomServ;
		m_iLCBAddDesignNum = m_iLCBDesignNum;
	}
	else
	{
		T_RRPL_D RrplD;
		if (!m_pDoc->m_pAttrCtrl2->GetRrpl(iRrpl, RrplD))
		{
			RrplD.Initialize();
		}

		int nLcomAppMax = 0;
		const INT_PTR nSizeElem = RrplD.arElemK.GetSize();
		for (INT_PTR i = 0; i < nSizeElem; ++i)
		{
			const T_ELEM_K CurElemK = RrplD.arElemK[i];

			const EN_DGNLCOM_APP enLcomApp = GetAppDgnLcomTypeByMemb(CurElemK);
			nLcomAppMax = max(nLcomAppMax, static_cast<int>(enLcomApp));
		}

		const EN_DGNLCOM_APP enLcomAppMax = static_cast<EN_DGNLCOM_APP>(nLcomAppMax);
		if (enLcomAppMax != EN_DGNLCOM_APP::STRN_SERV) { ASSERT(0); }

		m_parLcomData = GetAppDgnLcomMap(enLcomAppMax);
		m_iLCBDesignNum = GetSizeAppDgnLcomMap(enLcomAppMax);

		// Calculate Load Combination Number for Design.
		m_iLCBServiceNum = GetSizeAppDgnLcomMapServ(enLcomAppMax);
		m_iLCBAddDesignNum = m_iLCBDesignNum;
	}
}

void CDgnForceCtrl::Set_ForceMomentBySeperate_Rcpl(int iRcpl, _DGN_LCOM& LcomDesign)
{
	// Add by SHIN('07.06.28) MNET:2668 -> CDgnForceCtrl::Set_ForceMomentBySeperate 대처용
	// Add by ZINU.('03.02.15).
	m_iPosiNo = 0;
	m_iPosiNoNM = 0;
	m_iPosiNoPM = 0;
	m_iPosiNoV = 0;

	// Add by ZINU.('05.09.05). Set m_strMatType if _T("").
	if (m_strMatType == _T(""))
	{
		T_RCPL_D RcplD;  RcplD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetRcpl(iRcpl, RcplD)) {}

		if (RcplD.arElemK.GetSize() <= 0) { ASSERT(0); Reset_Force(); Reset_RcplForce(); return; }

		// Get Matl.
		// 설계영역의 재질이 동일한지 검토
		int nElemSize = RcplD.arElemK.GetSize();
		int iMatl = -1;
		if (nElemSize > 0)
		{
			T_ELEM_D eData;
			eData.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetElem(RcplD.arElemK.GetAt(0), eData))	ASSERT(0);
			iMatl = eData.elmat;
		}
		// Get Matl.
		T_MATD_D MatdD;
		MatdD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(iMatl, MatdD)) {}
		m_strMatType = MatdD.Type;
	}
	Reset_RcplForce();
	m_iOrgLcomKey = LcomDesign.OriginalLcomKey;

	Get_ForceMoment_Rcpl(iRcpl, LcomDesign);
}

void CDgnForceCtrl::Set_ForceMomentBySeperate_Rrpl(int iRrpl, _DGN_LCOM& LcomDesign)
{
	// Add by SHIN('07.06.28) MNET:2668 -> CDgnForceCtrl::Set_ForceMomentBySeperate 대처용
	// Add by ZINU.('03.02.15).
	m_iPosiNo = 0;
	m_iPosiNoNM = 0;
	m_iPosiNoPM = 0;
	m_iPosiNoV = 0;

	// Add by ZINU.('05.09.05). Set m_strMatType if _T("").
	if (m_strMatType == _T(""))
	{
		T_RRPL_D RrplD;  RrplD.Initialize();
		if (!m_pDoc->m_pAttrCtrl2->GetRrpl(iRrpl, RrplD)) {}

		if (RrplD.arElemK.GetSize() <= 0) { ASSERT(0); Reset_Force(); Reset_RrplForce(); return; }

		// Get Matl.
		// 설계영역의 재질이 동일한지 검토
		int nElemSize = RrplD.arElemK.GetSize();
		int iMatl = -1;
		if (nElemSize > 0)
		{
			T_ELEM_D eData;
			eData.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetElem(RrplD.arElemK.GetAt(0), eData))	ASSERT(0);
			iMatl = eData.elmat;
		}
		// Get Matl.
		T_MATD_D MatdD;
		MatdD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(iMatl, MatdD)) {}
		m_strMatType = MatdD.Type;
	}
	Reset_RrplForce();
	m_iOrgLcomKey = LcomDesign.OriginalLcomKey;

	Get_ForceMoment_Rrpl(iRrpl, LcomDesign);

}

void CDgnForceCtrl::Get_ForceMoment_Rcpl(int iRcpl, _DGN_LCOM& LcomDesign)
{
	// Add by SHIN('07.06.28) MNET:2668 -> CDgnForceCtrl::Get_ForceMoment 대처용
	T_RCPL_D RcplD;  RcplD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetRcpl(iRcpl, RcplD)) {}

	_DGN_PSTR_LCOMRC RcplForce;
	if (!Get_DgnPlateForcDataList(RcplD.arElemK, LcomDesign, RcplForce))
	{
		RcplForce.Initialize(); //산출에 실패하였을경우 초기화
	}

	m_Rcpl_Strp = RcplForce;
	m_Fxx_Rcpl[0] = RcplForce.FxxUF[0];  	m_Fxx_Rcpl[1] = RcplForce.FxxUF[1];
	m_Fyy_Rcpl[0] = RcplForce.FyyUF[0];  	m_Fyy_Rcpl[1] = RcplForce.FyyUF[1];
	m_Fxy_Rcpl[0] = RcplForce.FxyUF[0];  	m_Fxy_Rcpl[1] = RcplForce.FxyUF[1];
	m_Vxx_Rcpl[0] = RcplForce.VxxUF[0];  	m_Vxx_Rcpl[1] = RcplForce.VxxUF[1];
	m_Vyy_Rcpl[0] = RcplForce.VyyUF[0];   	m_Vyy_Rcpl[1] = RcplForce.VyyUF[1];
	m_Mxx_Rcpl[0] = RcplForce.MxxUF[0];  	m_Mxx_Rcpl[1] = RcplForce.MxxUF[1];
	m_Myy_Rcpl[0] = RcplForce.MyyUF[0];  	m_Myy_Rcpl[1] = RcplForce.MyyUF[1];
	m_Mxy_Rcpl[0] = RcplForce.MxyUF[0];  	m_Mxy_Rcpl[1] = RcplForce.MxyUF[1];

	//   m_FxxDL_Rcpl[0]  =RcplForce.FxxUFDL[0];  		m_FxxDL_Rcpl[1]  =RcplForce.FxxUFDL[1];  
	//   m_FyyDL_Rcpl[0]  =RcplForce.FyyUFDL[0];  		m_FyyDL_Rcpl[1]  =RcplForce.FyyUFDL[1];  
	//   m_FxyDL_Rcpl[0]  =RcplForce.FxyUFDL[0];  		m_FxyDL_Rcpl[1]  =RcplForce.FxyUFDL[1];  
	//   m_VxxDL_Rcpl[0]  =RcplForce.VxxUFDL[0];  		m_VxxDL_Rcpl[1]  =RcplForce.VxxUFDL[1];  
	//   m_VyyDL_Rcpl[0]  =RcplForce.VyyUFDL[0];   	m_VyyDL_Rcpl[1]  =RcplForce.VyyUFDL[1];                                          
	//   m_MxxDL_Rcpl[0]  =RcplForce.MxxUFDL[0];  		m_MxxDL_Rcpl[1]  =RcplForce.MxxUFDL[1];  
	//   m_MyyDL_Rcpl[0]  =RcplForce.MyyUFDL[0];  		m_MyyDL_Rcpl[1]  =RcplForce.MyyUFDL[1];  
	//   m_MxyDL_Rcpl[0]  =RcplForce.MxyUFDL[0];  		m_MxyDL_Rcpl[1]  =RcplForce.MxyUFDL[1];  

	//   m_FxxML_Rcpl[0]  =RcplForce.FxxUFML[0];  		m_FxxML_Rcpl[1]  =RcplForce.FxxUFML[1];  
	//   m_FyyML_Rcpl[0]  =RcplForce.FyyUFML[0];  		m_FyyML_Rcpl[1]  =RcplForce.FyyUFML[1];  
	//   m_FxyML_Rcpl[0]  =RcplForce.FxyUFML[0];  		m_FxyML_Rcpl[1]  =RcplForce.FxyUFML[1];  
	//   m_VxxML_Rcpl[0]  =RcplForce.VxxUFML[0];  		m_VxxML_Rcpl[1]  =RcplForce.VxxUFML[1];  
	//   m_VyyML_Rcpl[0]  =RcplForce.VyyUFML[0];   	m_VyyML_Rcpl[1]  =RcplForce.VyyUFML[1];                                          
	m_MxxML_Rcpl[0] = RcplForce.MxxUFML[0];		m_MxxML_Rcpl[1] = RcplForce.MxxUFML[1];
	m_MyyML_Rcpl[0] = RcplForce.MyyUFML[0];		m_MyyML_Rcpl[1] = RcplForce.MyyUFML[1];
	m_MxyML_Rcpl[0] = RcplForce.MyyUFML[0];		m_MxyML_Rcpl[1] = RcplForce.MxyUFML[1];
}


void CDgnForceCtrl::Get_ForceMoment_Rrpl(int iRrpl, _DGN_LCOM& LcomDesign)
{// Add by SHIN('07.06.28) MNET:2668 -> CDgnForceCtrl::Get_ForceMoment 대처용
	T_RRPL_D RrplD;  RrplD.Initialize();
	if (!m_pDoc->m_pAttrCtrl2->GetRrpl(iRrpl, RrplD)) {}

	_DGN_PSTR_LCOMRC RcplForce;

	int nType = 1; // MV load case 기준으로 찾는다
	if (!Get_DgnPlateForcDataList(RrplD.arElemK, LcomDesign, RcplForce, nType))
	{
		RcplForce.Initialize();
	}//산출에 실패하였을경우 초기화
	m_Rcpl_Strp = RcplForce;
	m_Fxx_Rcpl[0] = RcplForce.FxxUF[0];  		m_Fxx_Rcpl[1] = RcplForce.FxxUF[1];
	m_Fyy_Rcpl[0] = RcplForce.FyyUF[0];  		m_Fyy_Rcpl[1] = RcplForce.FyyUF[1];
	m_Fxy_Rcpl[0] = RcplForce.FxyUF[0];  		m_Fxy_Rcpl[1] = RcplForce.FxyUF[1];
	m_Vxx_Rcpl[0] = RcplForce.VxxUF[0];  		m_Vxx_Rcpl[1] = RcplForce.VxxUF[1];
	m_Vyy_Rcpl[0] = RcplForce.VyyUF[0];   	m_Vyy_Rcpl[1] = RcplForce.VyyUF[1];
	m_Mxx_Rcpl[0] = RcplForce.MxxUF[0];  		m_Mxx_Rcpl[1] = RcplForce.MxxUF[1];
	m_Myy_Rcpl[0] = RcplForce.MyyUF[0];  		m_Myy_Rcpl[1] = RcplForce.MyyUF[1];
	m_Mxy_Rcpl[0] = RcplForce.MxyUF[0];  		m_Mxy_Rcpl[1] = RcplForce.MxyUF[1];

	m_FxxDL_Rcpl[0] = RcplForce.FxxUFDL[0];  		m_FxxDL_Rcpl[1] = RcplForce.FxxUFDL[1];
	m_FyyDL_Rcpl[0] = RcplForce.FyyUFDL[0];  		m_FyyDL_Rcpl[1] = RcplForce.FyyUFDL[1];
	m_FxyDL_Rcpl[0] = RcplForce.FxyUFDL[0];  		m_FxyDL_Rcpl[1] = RcplForce.FxyUFDL[1];
	m_VxxDL_Rcpl[0] = RcplForce.VxxUFDL[0];  		m_VxxDL_Rcpl[1] = RcplForce.VxxUFDL[1];
	m_VyyDL_Rcpl[0] = RcplForce.VyyUFDL[0];   	m_VyyDL_Rcpl[1] = RcplForce.VyyUFDL[1];
	m_MxxDL_Rcpl[0] = RcplForce.MxxUFDL[0];  		m_MxxDL_Rcpl[1] = RcplForce.MxxUFDL[1];
	m_MyyDL_Rcpl[0] = RcplForce.MyyUFDL[0];  		m_MyyDL_Rcpl[1] = RcplForce.MyyUFDL[1];
	m_MxyDL_Rcpl[0] = RcplForce.MxyUFDL[0];  		m_MxyDL_Rcpl[1] = RcplForce.MxyUFDL[1];

	m_FxxML_Rcpl[0] = RcplForce.FxxUFML[0];  		m_FxxML_Rcpl[1] = RcplForce.FxxUFML[1];
	m_FyyML_Rcpl[0] = RcplForce.FyyUFML[0];  		m_FyyML_Rcpl[1] = RcplForce.FyyUFML[1];
	m_FxyML_Rcpl[0] = RcplForce.FxyUFML[0];  		m_FxyML_Rcpl[1] = RcplForce.FxyUFML[1];
	m_VxxML_Rcpl[0] = RcplForce.VxxUFML[0];  		m_VxxML_Rcpl[1] = RcplForce.VxxUFML[1];
	m_VyyML_Rcpl[0] = RcplForce.VyyUFML[0];   	m_VyyML_Rcpl[1] = RcplForce.VyyUFML[1];
	m_MxxML_Rcpl[0] = RcplForce.MxxUFML[0];		m_MxxML_Rcpl[1] = RcplForce.MxxUFML[1];
	m_MyyML_Rcpl[0] = RcplForce.MyyUFML[0];		m_MyyML_Rcpl[1] = RcplForce.MyyUFML[1];
	m_MxyML_Rcpl[0] = RcplForce.MyyUFML[0];		m_MxyML_Rcpl[1] = RcplForce.MxyUFML[1];
}

BOOL CDgnForceCtrl::IsCheckStaticDL()
{
	return m_bChkStatic;
}

BOOL CDgnForceCtrl::IsRatingCode()
{
	switch (m_iDgnCode)
	{
	case KSCE_USD10_RC_RATING:
	case KSCE_RAIL_USD11_RC_RATING:
	case KSCE_LSD15_RC_RATING:
	{
		return TRUE;
	}
	}
	return FALSE;
}

BOOL CDgnForceCtrl::IsEndLCase()
{
	return m_bEndLCase;
}

void CDgnForceCtrl::AddPlateDLForce(int i, T_STRP_DL &StrpMaxData, double dFactor)
{
	// Forces, Moments. (4 node)
	if (i < 4)
	{//component : 0:Fx, 1:Fy, 2:Fz, 3:Mx, 4:My, 5:Mz
		m_FxxD[i] += StrpMaxData.dblForce[i][0] * dFactor;
		m_FyyD[i] += StrpMaxData.dblForce[i][1] * dFactor;
		m_FzzD[i] += StrpMaxData.dblForce[i][2] * dFactor;
		m_BMxD[i] += StrpMaxData.dblForce[i][3] * dFactor;
		m_BMyD[i] += StrpMaxData.dblForce[i][4] * dFactor;
		m_BMzD[i] += StrpMaxData.dblForce[i][5] * dFactor;
	}
	// Unit Forces (cent + 4 node)
	if (i < 5)
	{//component : 14:Fxx, 16:Fyy, 17:Fxy, 20:Mxx, 21:Myy, 26:Vxx,  27:Vyy
		m_FxxD_UF[i] += StrpMaxData.dblStress[i][14] * dFactor;
		m_FyyD_UF[i] += StrpMaxData.dblStress[i][15] * dFactor;
		m_FxyD_UF[i] += StrpMaxData.dblStress[i][16] * dFactor;
		m_VxxD_UF[i] += StrpMaxData.dblStress[i][26] * dFactor;
		m_VyyD_UF[i] += StrpMaxData.dblStress[i][27] * dFactor;
		m_MxxD_UF[i] += StrpMaxData.dblStress[i][20] * dFactor;
		m_MyyD_UF[i] += StrpMaxData.dblStress[i][21] * dFactor;
		m_MxyD_UF[i] += StrpMaxData.dblStress[i][22] * dFactor;
	}
}

void CDgnForceCtrl::AddPlateDLForce(int i, T_STRP_DL &StrpMaxData, double dFactor, double dReduAxlFactor, double dReduShrFactor, double dReduMomFactor)
{
	// DL로 분류할 대 dReduAxlFactor 이것들을 고려할 필요가 있을까 ???
	// Forces, Moments. (4 node)
	if (i < 4)
	{//component : 0:Fx, 1:Fy, 2:Fz, 3:Mx, 4:My, 5:Mz
		m_FxxD[i] += StrpMaxData.dblForce[i][0] * dFactor * dReduAxlFactor;
		m_FyyD[i] += StrpMaxData.dblForce[i][1] * dFactor * dReduAxlFactor;
		m_FzzD[i] += StrpMaxData.dblForce[i][2] * dFactor * dReduShrFactor;
		m_BMxD[i] += StrpMaxData.dblForce[i][3] * dFactor * dReduMomFactor;
		m_BMyD[i] += StrpMaxData.dblForce[i][4] * dFactor * dReduMomFactor;
		m_BMzD[i] += StrpMaxData.dblForce[i][5] * dFactor * dReduMomFactor;
	}
	// Unit Forces (cent + 4 node)
	if (i < 5)
	{//component : 14:Fxx, 16:Fyy, 17:Fxy, 20:Mxx, 21:Myy, 26:Vxx,  27:Vyy
		m_FxxD_UF[i] += StrpMaxData.dblStress[i][14] * dFactor * dReduAxlFactor;
		m_FyyD_UF[i] += StrpMaxData.dblStress[i][15] * dFactor * dReduAxlFactor;
		m_FxyD_UF[i] += StrpMaxData.dblStress[i][16] * dFactor * dReduShrFactor;
		m_VxxD_UF[i] += StrpMaxData.dblStress[i][26] * dFactor * dReduShrFactor;
		m_VyyD_UF[i] += StrpMaxData.dblStress[i][27] * dFactor * dReduShrFactor;
		m_MxxD_UF[i] += StrpMaxData.dblStress[i][20] * dFactor * dReduMomFactor;
		m_MyyD_UF[i] += StrpMaxData.dblStress[i][21] * dFactor * dReduMomFactor;
		m_MxyD_UF[i] += StrpMaxData.dblStress[i][22] * dFactor * dReduMomFactor;
	}
}


BOOL CDgnForceCtrl::Get_DgnPlateForcDataList(CArray<T_ELEM_K, T_ELEM_K>& arElemK, _DGN_LCOM& LcomDesign, _DGN_PSTR_LCOMRC& RcplForce, int nType)
{
	RcplForce.Initialize();
	int			i, j, elem_num;
	//double		max_pforc=-cRCSC_Upon, min_pforc=cRCSC_Upon, len;
	BOOL		has_content = FALSE;
	T_ELEM_K	elem_key;
	T_ELEM_D	elem_data;
	T_STRP_DL	pforc, pforc_DL, pforc_ML;

	CArray<T_ELEM_K, T_ELEM_K>                     arResPlateElemK;//부재력 결과 값을 가지는 Elem 들
	CArray<UINT, UINT>                            PlateKeyList;
	CMap<T_ELEM_K, T_ELEM_K, T_STRP_DL, T_STRP_DL&>	mapPForcData;
	CMap<T_ELEM_K, T_ELEM_K, T_STRP_DL, T_STRP_DL&>	mapPForcData_DL;
	CMap<T_ELEM_K, T_ELEM_K, T_STRP_DL, T_STRP_DL&>	mapPForcData_ML;
	CMap <T_ELEM_K, T_ELEM_K, int, int>	            mapNodesInElem; // elem의 노드개수

	PlateKeyList.RemoveAll();
	mapPForcData.RemoveAll();
	mapPForcData_DL.RemoveAll();
	mapPForcData_ML.RemoveAll();
	mapNodesInElem.RemoveAll();

	arResPlateElemK.Copy(arElemK);

	elem_num = arResPlateElemK.GetSize();

	for (i = 0; i < elem_num; i++)
	{
		elem_key = arResPlateElemK.GetAt(i);

		m_pDoc->m_pAttrCtrl->GetElem(elem_key, elem_data);
		if (m_pDoc->m_pAttrCtrl->IsPlate(elem_data.eltyp))
		{
			Set_ForceMomentBySeperate(elem_key, -1, LcomDesign);
			Get_STRP_DgnForcCtrl(pforc);
			Get_STRP_DgnForcCtrl_ML(pforc_ML);
			Get_STRP_DgnForcCtrl_DL(pforc_DL);
		}
		else
		{
			continue;
		}

		mapPForcData.SetAt(elem_key, pforc);
		mapPForcData_ML.SetAt(elem_key, pforc_ML);
		mapPForcData_DL.SetAt(elem_key, pforc_DL);
	}

	// 최대, 최소값을 산출함
	RcplForce.Initialize();
	int nElemKSize = arElemK.GetSize();
	T_ELEM_K ElemKey;
	for (i = 0; i < nElemKSize; i++)
	{
		ElemKey = arElemK.GetAt(i);
		mapPForcData.Lookup(ElemKey, pforc);
		mapPForcData_DL.Lookup(ElemKey, pforc_DL);
		mapPForcData_ML.Lookup(ElemKey, pforc_ML);

		T_STRP_DL *pForce = nType == 1 ? &pforc_ML : &pforc; // nType = 1 : ML 기준, nType != 1 : D+L 

		double *pFxxUF[2];
		double *pFyyUF[2];
		double *pFxyUF[2];
		double *pVxxUF[2];
		double *pVyyUF[2];
		double *pMxxUF[2];
		double *pMyyUF[2];
		double *pMxyUF[2];

		if (nType == 1)
		{
			for (j = 0; j < 2; ++j)
			{
				pFxxUF[j] = &RcplForce.FxxUFML[j];
				pFyyUF[j] = &RcplForce.FyyUFML[j];
				pFxyUF[j] = &RcplForce.FxyUFML[j];
				pVxxUF[j] = &RcplForce.VxxUFML[j];
				pVyyUF[j] = &RcplForce.VyyUFML[j];
				pMxxUF[j] = &RcplForce.MxxUFML[j];
				pMyyUF[j] = &RcplForce.MyyUFML[j];
				pMxyUF[j] = &RcplForce.MxyUFML[j];
			}
		}
		else
		{
			for (j = 0; j < 2; ++j)
			{
				pFxxUF[j] = &RcplForce.FxxUF[j];
				pFyyUF[j] = &RcplForce.FyyUF[j];
				pFxyUF[j] = &RcplForce.FxyUF[j];
				pVxxUF[j] = &RcplForce.VxxUF[j];
				pVyyUF[j] = &RcplForce.VyyUF[j];
				pMxxUF[j] = &RcplForce.MxxUF[j];
				pMyyUF[j] = &RcplForce.MyyUF[j];
				pMxyUF[j] = &RcplForce.MxyUF[j];
			}
		}
		for (j = 0; j < 5; j++)// Cent. + 4Node
		{
			/*
						// Total
						// FxxUF : 14
						if(RcplForce.FxxUF[0] <= pforc.dblStress[j][14])
						{ RcplForce.FxxUF[0]   = pforc.dblStress[j][14];   RcplForce.FxxUFKey[0] = ElemKey; }
						if(RcplForce.FxxUF[1] >= pforc.dblStress[j][14])
						{ RcplForce.FxxUF[1]   = pforc.dblStress[j][14];   RcplForce.FxxUFKey[1] = ElemKey; }
						// FyyUF : 15
						if(RcplForce.FyyUF[0] <= pforc.dblStress[j][15])
						{ RcplForce.FyyUF[0]   = pforc.dblStress[j][15];   RcplForce.FyyUFKey[0] = ElemKey; }
						if(RcplForce.FyyUF[1] >= pforc.dblStress[j][15])
						{ RcplForce.FyyUF[1]   = pforc.dblStress[j][15];   RcplForce.FyyUFKey[1] = ElemKey; }
						// FxyUF : 16
						if(RcplForce.FxyUF[0] <= pforc.dblStress[j][16])
						{ RcplForce.FxyUF[0]   = pforc.dblStress[j][16];   RcplForce.FxyUFKey[0] = ElemKey; }
						if(RcplForce.FxyUF[1] >= pforc.dblStress[j][16])
						{ RcplForce.FxyUF[1]   = pforc.dblStress[j][16];   RcplForce.FxyUFKey[1] = ElemKey; }
						// VxxUF : 26
						if(RcplForce.VxxUF[0] <= pforc.dblStress[j][26])
						{ RcplForce.VxxUF[0]   = pforc.dblStress[j][26];   RcplForce.VxxUFKey[0] = ElemKey; }
						if(RcplForce.VxxUF[1] >= pforc.dblStress[j][26])
						{ RcplForce.VxxUF[1]   = pforc.dblStress[j][26];   RcplForce.VxxUFKey[1] = ElemKey; }
						// VyyUF : 27
						if(RcplForce.VyyUF[0] <= pforc.dblStress[j][27])
						{ RcplForce.VyyUF[0]   = pforc.dblStress[j][27];   RcplForce.VyyUFKey[0] = ElemKey; }
						if(RcplForce.VyyUF[1] >= pforc.dblStress[j][27])
						{ RcplForce.VyyUF[1]   = pforc.dblStress[j][27];   RcplForce.VyyUFKey[1] = ElemKey; }
						// MxxUF : 20
						if(RcplForce.MxxUF[0] <= pforc.dblStress[j][20])
						{ RcplForce.MxxUF[0]   = pforc.dblStress[j][20];   RcplForce.MxxUFKey[0] = ElemKey; }
						if(RcplForce.MxxUF[1] >= pforc.dblStress[j][20])
						{ RcplForce.MxxUF[1]   = pforc.dblStress[j][20];   RcplForce.MxxUFKey[1] = ElemKey; }
						// MyyUF : 21
						if(RcplForce.MyyUF[0] <= pforc.dblStress[j][21])
						{ RcplForce.MyyUF[0]   = pforc.dblStress[j][21];   RcplForce.MyyUFKey[0] = ElemKey; }
						if(RcplForce.MyyUF[1] >= pforc.dblStress[j][21])
						{ RcplForce.MyyUF[1]   = pforc.dblStress[j][21];   RcplForce.MyyUFKey[1] = ElemKey; }
						// MxyUF : 22
						if(RcplForce.MxyUF[0] <= pforc.dblStress[j][22])
						{ RcplForce.MxyUF[0]   = pforc.dblStress[j][22];   RcplForce.MxyUFKey[0] = ElemKey; }
						if(RcplForce.MxyUF[1] >= pforc.dblStress[j][22])
						{ RcplForce.MxyUF[1]   = pforc.dblStress[j][22];   RcplForce.MxyUFKey[1] = ElemKey; }

						// DL
						// FxxUFDL : 14
						if(RcplForce.FxxUFDL[0] <= pforc_DL.dblStress[j][14])
						{ RcplForce.FxxUFDL[0]   = pforc_DL.dblStress[j][14];   RcplForce.FxxUFDLKey[0] = ElemKey; }
						if(RcplForce.FxxUFDL[1] >= pforc_DL.dblStress[j][14])
						{ RcplForce.FxxUFDL[1]   = pforc_DL.dblStress[j][14];   RcplForce.FxxUFDLKey[1] = ElemKey; }
						// FyyUFDL : 15
						if(RcplForce.FyyUFDL[0] <= pforc_DL.dblStress[j][15])
						{ RcplForce.FyyUFDL[0]   = pforc_DL.dblStress[j][15];   RcplForce.FyyUFDLKey[0] = ElemKey; }
						if(RcplForce.FyyUFDL[1] >= pforc_DL.dblStress[j][15])
						{ RcplForce.FyyUFDL[1]   = pforc_DL.dblStress[j][15];   RcplForce.FyyUFDLKey[1] = ElemKey; }
						// FxyUFDL : 16
						if(RcplForce.FxyUFDL[0] <= pforc_DL.dblStress[j][16])
						{ RcplForce.FxyUFDL[0]   = pforc_DL.dblStress[j][16];   RcplForce.FxyUFDLKey[0] = ElemKey; }
						if(RcplForce.FxyUFDL[1] >= pforc_DL.dblStress[j][16])
						{ RcplForce.FxyUFDL[1]   = pforc_DL.dblStress[j][16];   RcplForce.FxyUFDLKey[1] = ElemKey; }
						// VxxUFDL : 26
						if(RcplForce.VxxUFDL[0] <= pforc_DL.dblStress[j][26])
						{ RcplForce.VxxUFDL[0]   = pforc_DL.dblStress[j][26];   RcplForce.VxxUFDLKey[0] = ElemKey; }
						if(RcplForce.VxxUFDL[1] >= pforc_DL.dblStress[j][26])
						{ RcplForce.VxxUFDL[1]   = pforc_DL.dblStress[j][26];   RcplForce.VxxUFDLKey[1] = ElemKey; }
						// VyyUFDL : 27
						if(RcplForce.VyyUFDL[0] <= pforc_DL.dblStress[j][27])
						{ RcplForce.VyyUFDL[0]   = pforc_DL.dblStress[j][27];   RcplForce.VyyUFDLKey[0] = ElemKey; }
						if(RcplForce.VyyUFDL[1] >= pforc_DL.dblStress[j][27])
						{ RcplForce.VyyUFDL[1]   = pforc_DL.dblStress[j][27];   RcplForce.VyyUFDLKey[1] = ElemKey; }
						// MxxUFDL : 20
						if(RcplForce.MxxUFDL[0] <= pforc_DL.dblStress[j][20])
						{ RcplForce.MxxUFDL[0]   = pforc_DL.dblStress[j][20];   RcplForce.MxxUFDLKey[0] = ElemKey; }
						if(RcplForce.MxxUFDL[1] >= pforc_DL.dblStress[j][20])
						{ RcplForce.MxxUFDL[1]   = pforc_DL.dblStress[j][20];   RcplForce.MxxUFDLKey[1] = ElemKey; }
						// MyyUFDL : 21
						if(RcplForce.MyyUFDL[0] <= pforc_DL.dblStress[j][21])
						{ RcplForce.MyyUFDL[0]   = pforc_DL.dblStress[j][21];   RcplForce.MyyUFDLKey[0] = ElemKey; }
						if(RcplForce.MyyUFDL[1] >= pforc_DL.dblStress[j][21])
						{ RcplForce.MyyUFDL[1]   = pforc_DL.dblStress[j][21];   RcplForce.MyyUFDLKey[1] = ElemKey; }
						// MxyUFDL : 22
						if(RcplForce.MxyUFDL[0] <= pforc_DL.dblStress[j][22])
						{ RcplForce.MxyUFDL[0]   = pforc_DL.dblStress[j][22];   RcplForce.MxyUFDLKey[0] = ElemKey; }
						if(RcplForce.MxyUFDL[1] >= pforc_DL.dblStress[j][22])
						{ RcplForce.MxyUFDL[1]   = pforc_DL.dblStress[j][22];   RcplForce.MxyUFDLKey[1] = ElemKey; }

						// Moving
						if(RcplForce.FxxUFML[0] <= pforc_ML.dblStress[j][14])
						{ RcplForce.FxxUFML[0]   = pforc_ML.dblStress[j][14];   RcplForce.FxxUFMLKey[0] = ElemKey; }
						if(RcplForce.FxxUFML[1] >= pforc_ML.dblStress[j][14])
						{ RcplForce.FxxUFML[1]   = pforc_ML.dblStress[j][14];   RcplForce.FxxUFMLKey[1] = ElemKey; }
						// FyyUFDL : 15
						if(RcplForce.FyyUFML[0] <= pforc_ML.dblStress[j][15])
						{ RcplForce.FyyUFML[0]   = pforc_ML.dblStress[j][15];   RcplForce.FyyUFMLKey[0] = ElemKey; }
						if(RcplForce.FyyUFML[1] >= pforc_ML.dblStress[j][15])
						{ RcplForce.FyyUFML[1]   = pforc_ML.dblStress[j][15];   RcplForce.FyyUFMLKey[1] = ElemKey; }
						// FxyUFDL : 16
						if(RcplForce.FxyUFML[0] <= pforc_ML.dblStress[j][16])
						{ RcplForce.FxyUFML[0]   = pforc_ML.dblStress[j][16];   RcplForce.FxyUFMLKey[0] = ElemKey; }
						if(RcplForce.FxyUFML[1] >= pforc_ML.dblStress[j][16])
						{ RcplForce.FxyUFML[1]   = pforc_ML.dblStress[j][16];   RcplForce.FxyUFMLKey[1] = ElemKey; }
						// VxxUFDL : 26
						if(RcplForce.VxxUFML[0] <= pforc_ML.dblStress[j][26])
						{ RcplForce.VxxUFML[0]   = pforc_ML.dblStress[j][26];   RcplForce.VxxUFMLKey[0] = ElemKey; }
						if(RcplForce.VxxUFML[1] >= pforc_ML.dblStress[j][26])
						{ RcplForce.VxxUFML[1]   = pforc_ML.dblStress[j][26];   RcplForce.VxxUFMLKey[1] = ElemKey; }
						// VyyUFDL : 27
						if(RcplForce.VyyUFML[0] <= pforc_ML.dblStress[j][27])
						{ RcplForce.VyyUFML[0]   = pforc_ML.dblStress[j][27];   RcplForce.VyyUFMLKey[0] = ElemKey; }
						if(RcplForce.VyyUFML[1] >= pforc_ML.dblStress[j][27])
						{ RcplForce.VyyUFML[1]   = pforc_ML.dblStress[j][27];   RcplForce.VyyUFMLKey[1] = ElemKey; }
						// MxxUFML : 20
						if(RcplForce.MxxUFML[0] <= pforc_ML.dblStress[j][20])
						{ RcplForce.MxxUFML[0]   = pforc_ML.dblStress[j][20];   RcplForce.MxxUFMLKey[0] = ElemKey; }
						if(RcplForce.MxxUFML[1] >= pforc_ML.dblStress[j][20])
						{ RcplForce.MxxUFML[1]   = pforc_ML.dblStress[j][20];   RcplForce.MxxUFMLKey[1] = ElemKey; }
						// MyyUF : 21
						if(RcplForce.MyyUFML[0] <= pforc_ML.dblStress[j][21])
						{ RcplForce.MyyUFML[0]   = pforc_ML.dblStress[j][21];   RcplForce.MyyUFMLKey[0] = ElemKey; }
						if(RcplForce.MyyUFML[1] >= pforc_ML.dblStress[j][21])
						{ RcplForce.MyyUFML[1]   = pforc_ML.dblStress[j][21];   RcplForce.MyyUFMLKey[1] = ElemKey; }
						// MxyUF : 22
						if(RcplForce.MxyUFML[0] <= pforc_ML.dblStress[j][22])
						{ RcplForce.MxyUFML[0]   = pforc_ML.dblStress[j][22];   RcplForce.MxyUFMLKey[0] = ElemKey; }
						if(RcplForce.MxyUFML[1] >= pforc_ML.dblStress[j][22])
						{ RcplForce.MxyUFML[1]   = pforc_ML.dblStress[j][22];   RcplForce.MxyUFMLKey[1] = ElemKey; }
			 */
			 //////////////////////////////////////////////////////////////////////////   
						 // FxxUF : 14
			if (*pFxxUF[0] <= pforc.dblStress[j][14])
			{
				RcplForce.FxxUF[0] = pforc.dblStress[j][14];      RcplForce.FxxUFKey[0] = ElemKey;  // Total
				RcplForce.FxxUFDL[0] = pforc_DL.dblStress[j][14];   RcplForce.FxxUFDLKey[0] = ElemKey;  // DL
				RcplForce.FxxUFML[0] = pforc_ML.dblStress[j][14];   RcplForce.FxxUFMLKey[0] = ElemKey;  // LL
			}
			if (*pFxxUF[1] >= pforc.dblStress[j][14])
			{
				RcplForce.FxxUF[1] = pforc.dblStress[j][14];      RcplForce.FxxUFKey[1] = ElemKey;
				RcplForce.FxxUFDL[1] = pforc_DL.dblStress[j][14];   RcplForce.FxxUFDLKey[1] = ElemKey;
				RcplForce.FxxUFML[1] = pforc_ML.dblStress[j][14];   RcplForce.FxxUFMLKey[1] = ElemKey;
			}
			// FyyUF : 15
			if (*pFyyUF[0] <= pforc.dblStress[j][15])
			{
				RcplForce.FyyUF[0] = pforc.dblStress[j][15];      RcplForce.FyyUFKey[0] = ElemKey;
				RcplForce.FyyUFDL[0] = pforc_DL.dblStress[j][15];   RcplForce.FyyUFDLKey[0] = ElemKey;
				RcplForce.FyyUFML[0] = pforc_ML.dblStress[j][15];   RcplForce.FyyUFMLKey[0] = ElemKey;
			}
			if (*pFyyUF[1] >= pforc.dblStress[j][15])
			{
				RcplForce.FyyUF[1] = pforc.dblStress[j][15];      RcplForce.FyyUFKey[1] = ElemKey;
				RcplForce.FyyUFDL[1] = pforc_DL.dblStress[j][15];   RcplForce.FyyUFDLKey[1] = ElemKey;
				RcplForce.FyyUFML[1] = pforc_ML.dblStress[j][15];   RcplForce.FyyUFMLKey[1] = ElemKey;
			}
			// FxyUF : 16
			if (*pFxyUF[0] <= pforc.dblStress[j][16])
			{
				RcplForce.FxyUF[0] = pforc.dblStress[j][16];      RcplForce.FxyUFKey[0] = ElemKey;
				RcplForce.FxyUFDL[0] = pforc_DL.dblStress[j][16];   RcplForce.FxyUFDLKey[0] = ElemKey;
				RcplForce.FxyUFML[0] = pforc_ML.dblStress[j][16];   RcplForce.FxyUFMLKey[0] = ElemKey;
			}
			if (*pFxyUF[1] >= pforc.dblStress[j][16])
			{
				RcplForce.FxyUF[1] = pforc.dblStress[j][16];      RcplForce.FxyUFKey[1] = ElemKey;
				RcplForce.FxyUFDL[1] = pforc_DL.dblStress[j][16];   RcplForce.FxyUFDLKey[1] = ElemKey;
				RcplForce.FxyUFML[1] = pforc_ML.dblStress[j][16];   RcplForce.FxyUFMLKey[1] = ElemKey;
			}
			// VxxUF : 26
			if (*pVxxUF[0] <= pforc.dblStress[j][26])
			{
				RcplForce.VxxUF[0] = pforc.dblStress[j][26];      RcplForce.VxxUFKey[0] = ElemKey;
				RcplForce.VxxUFDL[0] = pforc_DL.dblStress[j][26];   RcplForce.VxxUFDLKey[0] = ElemKey;
				RcplForce.VxxUFML[0] = pforc_ML.dblStress[j][26];   RcplForce.VxxUFMLKey[0] = ElemKey;
			}
			if (*pVxxUF[1] >= pforc.dblStress[j][26])
			{
				RcplForce.VxxUF[1] = pforc.dblStress[j][26];      RcplForce.VxxUFKey[1] = ElemKey;
				RcplForce.VxxUFDL[1] = pforc_DL.dblStress[j][26];   RcplForce.VxxUFDLKey[1] = ElemKey;
				RcplForce.VxxUFML[1] = pforc_ML.dblStress[j][26];   RcplForce.VxxUFMLKey[1] = ElemKey;
			}
			// VyyUF : 27
			if (*pVyyUF[0] <= pforc.dblStress[j][27])
			{
				RcplForce.VyyUF[0] = pforc.dblStress[j][27];      RcplForce.VyyUFKey[0] = ElemKey;
				RcplForce.VyyUFDL[0] = pforc_DL.dblStress[j][27];   RcplForce.VyyUFDLKey[0] = ElemKey;
				RcplForce.VyyUFML[0] = pforc_ML.dblStress[j][27];   RcplForce.VyyUFMLKey[0] = ElemKey;
			}
			if (*pVyyUF[1] >= pforc.dblStress[j][27])
			{
				RcplForce.VyyUF[1] = pforc.dblStress[j][27];      RcplForce.VyyUFKey[1] = ElemKey;
				RcplForce.VyyUFDL[1] = pforc_DL.dblStress[j][27];   RcplForce.VyyUFDLKey[1] = ElemKey;
				RcplForce.VyyUFML[1] = pforc_ML.dblStress[j][27];   RcplForce.VyyUFMLKey[1] = ElemKey;
			}
			// MxxUF : 20
			if (*pMxxUF[0] <= pforc.dblStress[j][20])
			{
				RcplForce.MxxUF[0] = pforc.dblStress[j][20];      RcplForce.MxxUFKey[0] = ElemKey;
				RcplForce.MxxUFDL[0] = pforc_DL.dblStress[j][20];   RcplForce.MxxUFDLKey[0] = ElemKey;
				RcplForce.MxxUFML[0] = pforc_ML.dblStress[j][20];   RcplForce.MxxUFMLKey[0] = ElemKey;
			}
			if (*pMxxUF[1] >= pforc.dblStress[j][20])
			{
				RcplForce.MxxUF[1] = pforc.dblStress[j][20];      RcplForce.MxxUFKey[1] = ElemKey;
				RcplForce.MxxUFDL[1] = pforc_DL.dblStress[j][20];   RcplForce.MxxUFDLKey[1] = ElemKey;
				RcplForce.MxxUFML[1] = pforc_ML.dblStress[j][20];   RcplForce.MxxUFMLKey[1] = ElemKey;
			}
			// MyyUF : 21
			if (*pMyyUF[0] <= pforc.dblStress[j][21])
			{
				RcplForce.MyyUF[0] = pforc.dblStress[j][21];      RcplForce.MyyUFKey[0] = ElemKey;
				RcplForce.MyyUFDL[0] = pforc_DL.dblStress[j][21];   RcplForce.MyyUFDLKey[0] = ElemKey;
				RcplForce.MyyUFML[0] = pforc_ML.dblStress[j][21];   RcplForce.MyyUFMLKey[0] = ElemKey;
			}
			if (*pMyyUF[1] >= pforc.dblStress[j][21])
			{
				RcplForce.MyyUF[1] = pforc.dblStress[j][21];      RcplForce.MyyUFKey[1] = ElemKey;
				RcplForce.MyyUFDL[1] = pforc_DL.dblStress[j][21];   RcplForce.MyyUFDLKey[1] = ElemKey;
				RcplForce.MyyUFML[1] = pforc_ML.dblStress[j][21];   RcplForce.MyyUFMLKey[1] = ElemKey;
			}
			// MxyUF : 22
			if (*pMxyUF[0] <= pforc.dblStress[j][22])
			{
				RcplForce.MxyUF[0] = pforc.dblStress[j][22];      RcplForce.MxyUFKey[0] = ElemKey;
				RcplForce.MxyUFDL[0] = pforc_DL.dblStress[j][22];   RcplForce.MxyUFDLKey[0] = ElemKey;
				RcplForce.MxyUFML[0] = pforc_ML.dblStress[j][22];   RcplForce.MxyUFMLKey[0] = ElemKey;
			}
			if (*pMxyUF[1] >= pforc.dblStress[j][22])
			{
				RcplForce.MxyUF[1] = pforc.dblStress[j][22];      RcplForce.MxyUFKey[1] = ElemKey;
				RcplForce.MxyUFDL[1] = pforc_DL.dblStress[j][22];   RcplForce.MxyUFDLKey[1] = ElemKey;
				RcplForce.MxyUFML[1] = pforc_ML.dblStress[j][22];   RcplForce.MxyUFMLKey[1] = ElemKey;
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}
	return TRUE;
}

void CDgnForceCtrl::Get_STRP_DgnForcCtrl(T_STRP_DL& StrpDL)
{
	StrpDL.Initialize();

	for (int i = 0; i < 4; i++)
	{
		StrpDL.dblForce[i][0] = m_Fxx[i];
		StrpDL.dblForce[i][1] = m_Fyy[i];
		StrpDL.dblForce[i][2] = m_Fzz[i];
		StrpDL.dblForce[i][3] = m_BMx[i];
		StrpDL.dblForce[i][4] = m_BMy[i];
		StrpDL.dblForce[i][5] = m_BMz[i];
	}
	for (int i = 0; i < 5; i++)
	{
		// 나머지 항목중 필요할때에 추가할것
		StrpDL.dblStress[i][14] = m_Fxx_UF[i];
		StrpDL.dblStress[i][15] = m_Fyy_UF[i];
		StrpDL.dblStress[i][16] = m_Fxy_UF[i];
		StrpDL.dblStress[i][26] = m_Vxx_UF[i];
		StrpDL.dblStress[i][27] = m_Vyy_UF[i];
		StrpDL.dblStress[i][20] = m_Mxx_UF[i];
		StrpDL.dblStress[i][21] = m_Myy_UF[i];
		StrpDL.dblStress[i][22] = m_Mxy_UF[i];
	}
}
void CDgnForceCtrl::Get_STRP_DgnForcCtrl_DL(T_STRP_DL& StrpDL)
{
	StrpDL.Initialize();
	int i;
	for (i = 0; i < 4; i++)
	{
		StrpDL.dblForce[i][0] = m_FxxD[i];
		StrpDL.dblForce[i][1] = m_FyyD[i];
		StrpDL.dblForce[i][2] = m_FzzD[i];
		StrpDL.dblForce[i][3] = m_BMxD[i];
		StrpDL.dblForce[i][4] = m_BMyD[i];
		StrpDL.dblForce[i][5] = m_BMzD[i];
	}
	for (i = 0; i < 5; i++)
	{
		// 나머지 항목중 필요할때에 추가할것
		StrpDL.dblStress[i][14] = m_FxxD_UF[i];
		StrpDL.dblStress[i][15] = m_FyyD_UF[i];
		StrpDL.dblStress[i][16] = m_FxyD_UF[i];
		StrpDL.dblStress[i][26] = m_VxxD_UF[i];
		StrpDL.dblStress[i][27] = m_VyyD_UF[i];
		StrpDL.dblStress[i][20] = m_MxxD_UF[i];
		StrpDL.dblStress[i][21] = m_MyyD_UF[i];
		StrpDL.dblStress[i][22] = m_MxyD_UF[i];
	}
}
void CDgnForceCtrl::Get_STRP_DgnForcCtrl_ML(T_STRP_DL& StrpDL)
{
	StrpDL.Initialize();
	int i;
	for (i = 0; i < 4; i++)
	{
		StrpDL.dblForce[i][0] = m_FxxML[i];
		StrpDL.dblForce[i][1] = m_FyyML[i];
		StrpDL.dblForce[i][2] = m_FzzML[i];
		StrpDL.dblForce[i][3] = m_BMxML[i];
		StrpDL.dblForce[i][4] = m_BMyML[i];
		StrpDL.dblForce[i][5] = m_BMzML[i];
	}
	for (i = 0; i < 5; i++)
	{
		StrpDL.dblStress[i][14] = m_FxxML_UF[i];
		StrpDL.dblStress[i][15] = m_FyyML_UF[i];
		StrpDL.dblStress[i][16] = m_FxyML_UF[i];
		StrpDL.dblStress[i][26] = m_VxxML_UF[i];
		StrpDL.dblStress[i][27] = m_VyyML_UF[i];

		StrpDL.dblStress[i][20] = m_MxxML_UF[i];
		StrpDL.dblStress[i][21] = m_MyyML_UF[i];
		StrpDL.dblStress[i][22] = m_MxyML_UF[i];
	}
}

double CDgnForceCtrl::GetMovingLiveLoad_Rcpl(T_RCPL_K RcplK)
{// Add by SHIN('07.06.28) MNET:2668 -> CDgnForceCtrl::m_BMyML 대처용	
	double dMyMLMax, dMyMLMin = 0.0;
	T_RCPL_D RcplD;  RcplD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetRcpl(RcplK, RcplD)) return FALSE;
	if (RcplD.iDir == 0)//설계 방향(0=X-Dir, 1=Y-Dir)
	{
		dMyMLMax = m_MxxML_Rcpl[0];
		dMyMLMin = m_MxxML_Rcpl[1];
	}
	else
	{
		dMyMLMax = m_MyyML_Rcpl[0];
		dMyMLMin = m_MyyML_Rcpl[1];
	}
	return max(fabs(dMyMLMax), fabs(dMyMLMin));
}

INT_PTR CDgnForceCtrl::Get_StldKeyListByLcaseType(const CString& strType, OUT T_KEY_LIST& raStldK)
{
	raStldK.RemoveAll();

	CArray<T_STLD_K, T_STLD_K> aStldK;
	aStldK.RemoveAll();
	m_pDoc->m_pPostCtrl->GetStldKeyList(aStldK);
	const INT_PTR nStld = aStldK.GetSize();
	for (INT_PTR i = 0; i < nStld; ++i)
	{
		T_STLD_K StldK = aStldK[i];
		T_STLD_D StldD;
		if (!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) { continue; }
		if (StldD.LoadCaseType == strType)
		{
			raStldK.Add(StldK);
		}
	}
	return raStldK.GetSize();
}

BOOL CDgnForceCtrl::Get_BeamForcebyStld(T_KEY ElemK, const T_KEY_LIST& aStldK, int nComType, double rForce[CONST_DGN_iPOSNUM])
{
	// nComType : 0=Fxx, 1=Fyy, 2=Fzz, 3=Mxx, 4=Myy, 5=Mzz
	for (int i = 0; i < CONST_DGN_iPOSNUM; ++i)
	{
		rForce[i] = 0.0;
	}

	const INT_PTR nStld = aStldK.GetSize();
	if (nStld < 1) { return FALSE; }

	int iLcaseType = D_LOADCASE_STATIC;
	T_STRB_D StrbMax; StrbMax.Initialize();
	T_STRB_D StrbMin; StrbMin.Initialize();

	for (INT_PTR m = 0; m < nStld; ++m)
	{
		T_STLD_K StldK = aStldK[m];
		if (!GetStrbDgn(ElemK, iLcaseType, StldK, &StrbMax, &StrbMin)) { continue; }

		for (int i = 0; i < CONST_DGN_iPOSNUM; ++i)
		{
			// Forces, Moments.
			int iPart = 0;
			int iKind = 0;
			int iKind7 = 0;
			GetAnalForceComponentIndex(i, iPart, iKind, iKind7);
			rForce[i] += StrbMax.dblForce[iPart][iKind + nComType];
		}
	}
	return TRUE;
}


int CDgnForceCtrl::GetForceComponentSize(BOOL bExistCrane, BOOL bConCurrent12, int nDgnCode, BOOL bPlateBC)
{
	int nCompSize = 1;
	if (bExistCrane)  // 동시발생 부재력 14 Cases인 경우.
	{
		m_bExistCraneLcom = TRUE;
		nCompSize = 10;	 /// Fx, Fy, Fz, Mx, My, Mz, Mbi, Tst, Twp, Sigm_bChkConCurForce12 = TRUma
	}
	if (bConCurrent12)  // 동시발생 부재력 12 Case인 경우.
	{
		m_bChkConCurForce12 = TRUE;
		if (bPlateBC) //nDgnNo == D_RC_PLATE_BC
		{
			nCompSize = 8; // postctrl.h 의 GetSpcfNew_L 함수 참고
		}
		else
		{
			switch (nDgnCode)
			{
			case KSCE_RC_USD05:
			case KSCE_RC_RAIL_USD04:
			case KCI_USD07:
			case KCI_USD07_CVL:
			case KCI_USD12:
			case KDS_41_30_2018:
			case KDS_41_20_2022:
			case KSCE_RC_USD10:
			case KSCE_RC_RAIL_USD11:
			case KR_LRFD11_RC:
			case KDS_24_14_21_2022:
			case KSCE_ASD05:
			case KSCE_RAIL_ASD04:
			case KSCE_ASD10:
			case KDS_24_14_30_2019:
			case KSCE_RAIL_ASD11:
			case KR_BRG_LSD15:
			case KDS_24_14_31_2018:
				nCompSize = 6;  /// Fx, Fy, Fz, Mx, My, Mz
				break;
			default:
				nCompSize = 9;  /// Fx, Fy, Fz, Mx, My, Mz, Mbi, Tst, Twp
				break;
			}
		}
	}
	return nCompSize;
}


EN_DGN_SEIS_CLASS CDgnForceCtrl::GetSeisClassFram(T_ELEM_K ElemK)
{
	EN_DGN_SEIS_CLASS enClass = EN_DGN_SEIS_CLASS::NONE;

	T_ELEM_K ElemShowK = ElemK;
	m_pDgnDataCtrl->m_pMembCtrl->GetShowElemByIncludeElem(ElemK, ElemShowK);

	if (m_mSeisClassFram.Lookup(ElemShowK, enClass))
	{
		return enClass;
	}
	return EN_DGN_SEIS_CLASS::PRIMARY; /// Secondary / Non-dissipative 가 정의되지 않았으면 Primary 로 본다.
}

EN_DGN_SEIS_CLASS CDgnForceCtrl::GetSeisClassWall(int nInternalWallID)
{
	EN_DGN_SEIS_CLASS enClass = EN_DGN_SEIS_CLASS::NONE;
	if (m_mSeisClassWall.Lookup(nInternalWallID, enClass))
	{
		return enClass;
	}
	return EN_DGN_SEIS_CLASS::PRIMARY; /// Secondary / Non-dissipative 가 정의되지 않았으면 Primary 로 본다.
}

EN_DGN_SEIS_CLASS CDgnForceCtrl::GetSeisClassSlab(T_ELEM_K ElemK)
{
	EN_DGN_SEIS_CLASS enClass = EN_DGN_SEIS_CLASS::NONE;
	if (m_mSeisClassSlab.Lookup(ElemK, enClass))
	{
		return enClass;
	}
	return EN_DGN_SEIS_CLASS::PRIMARY; /// Secondary / Non-dissipative 가 정의되지 않았으면 Primary 로 본다.
}

void CDgnForceCtrl::SetSeisClassMap()
{
	m_mSeisClassFram.RemoveAll();
	m_mSeisClassWall.RemoveAll();
	m_mSeisClassSlab.RemoveAll();

	T_DCON_D DconD;
	m_pDgnDataCtrl->Get_DgnConDcon(DconD);

	if (!IsApplyNonDissipative()) return;

	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;
	if (pAttrCtrl == nullptr) return;

	T_ELEM_D ElemD;
	T_GRUP_D GrupD;
	if (pAttrCtrl->GetGrup(DconD.SecondaryGroupKey, GrupD))
	{
		INT_PTR nElem = GrupD.arKeyElem.GetSize();
		for (INT_PTR i = 0; i < nElem; ++i)
		{
			T_ELEM_K ElemK = GrupD.arKeyElem[i];
			if (!pAttrCtrl->GetElem(ElemK, ElemD)) continue;

			const int nElType = ElemD.eltyp;
			if (pAttrCtrl->IsWall(nElType))
			{
				UINT unInternalWallID = 0;
				if (!m_pDoc->m_pPostCtrl->GetInternalWallId(ElemK, unInternalWallID)) continue;
				m_mSeisClassWall.SetAt(unInternalWallID, EN_DGN_SEIS_CLASS::SECONDARY);
			}
			else if (pAttrCtrl->IsFrameType(nElType))
			{
				m_mSeisClassFram.SetAt(ElemK, EN_DGN_SEIS_CLASS::SECONDARY);
			}
			else if (pAttrCtrl->IsPlate(nElType))
			{
				m_mSeisClassSlab.SetAt(ElemK, EN_DGN_SEIS_CLASS::SECONDARY);
			}
			else
			{
				continue;
			}
		}
	}

	if (IsApplyNonDissipative())
	{
		if (m_iDgnClass == EN_EC8_DCL) /// Low Ductility 인 경우, Secondary가 아닌 부재는 ND로 설계.
		{
			CMembCtrl* pMembCtrl = m_pDgnDataCtrl->m_pMembCtrl;

			T_ELEM_K_LIST aFramK;
			pMembCtrl->GetElemListForDgn(FALSE, aFramK);
			INT_PTR nFram = aFramK.GetSize();
			for (INT_PTR i = 0; i < nFram; ++i)
			{
				T_ELEM_K CurElemK = aFramK[i];
				EN_DGN_SEIS_CLASS CurClass;
				if (m_mSeisClassFram.Lookup(CurElemK, CurClass) && CurClass == EN_DGN_SEIS_CLASS::SECONDARY) continue;
				m_mSeisClassFram.SetAt(CurElemK, EN_DGN_SEIS_CLASS::NONDISSIPATIVE);
			}

			T_ELEM_K_LIST aWallK;
			pMembCtrl->GetElemListForDgn(TRUE, aWallK);
			INT_PTR nWall = aWallK.GetSize();
			for (INT_PTR i = 0; i < nWall; ++i)
			{
				UINT CurInWallID = aWallK[i];
				EN_DGN_SEIS_CLASS CurClass;
				if (m_mSeisClassWall.Lookup(CurInWallID, CurClass) && CurClass == EN_DGN_SEIS_CLASS::SECONDARY) continue;
				m_mSeisClassWall.SetAt(CurInWallID, EN_DGN_SEIS_CLASS::NONDISSIPATIVE);
			}

			T_ELEM_K_LIST aSlabK;
			pMembCtrl->GetPlateListForDgn(aSlabK, FALSE); /// ND 설계는 Slab만 고려.
			INT_PTR nSlab = aSlabK.GetSize();
			for (INT_PTR i = 0; i < nSlab; ++i)
			{
				T_ELEM_K CurElemK = aSlabK[i];
				EN_DGN_SEIS_CLASS CurClass;
				if (m_mSeisClassSlab.Lookup(CurElemK, CurClass) && CurClass == EN_DGN_SEIS_CLASS::SECONDARY) continue;
				m_mSeisClassSlab.SetAt(CurElemK, EN_DGN_SEIS_CLASS::NONDISSIPATIVE);
			}
		}
		else
		{
			if (pAttrCtrl->GetGrup(DconD.NonDissipativeGroupKey, GrupD))
			{
				int nElem = GrupD.arKeyElem.GetSize();
				for (int i = 0; i < nElem; ++i)
				{
					T_ELEM_K ElemK = GrupD.arKeyElem[i];
					if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;

					const int nElType = ElemD.eltyp;
					if (m_pDoc->m_pAttrCtrl->IsWall(nElType))
					{
						UINT unInWallID = 0;
						if (!m_pDoc->m_pPostCtrl->GetInternalWallId(ElemK, unInWallID)) continue;
						m_mSeisClassWall.SetAt(unInWallID, EN_DGN_SEIS_CLASS::NONDISSIPATIVE);
					}
					else if (pAttrCtrl->IsFrameType(nElType))
					{
						m_mSeisClassFram.SetAt(ElemK, EN_DGN_SEIS_CLASS::NONDISSIPATIVE);
					}
					else if (pAttrCtrl->IsPlate(nElType))
					{
						m_mSeisClassSlab.SetAt(ElemK, EN_DGN_SEIS_CLASS::NONDISSIPATIVE);
					}
					else
					{
						continue;
					}
				}
			}
		}
	}
}

void CDgnForceCtrl::GetSeisReqMomentFactor_KCI_USD07_Series(T_ELEM_K ElemK, int nEqGrade, double& rdEndF, double& rdMidF)
{
	rdEndF = 0.0;
	rdMidF = 0.0;

	if (!IsEqSpecialElem(ElemK))  // Modify by GAY. PMS:4076. ('10.03.04). Not Applied for Non-Seismic Design Member.     
	{
		rdEndF = 0.0;
		rdMidF = 0.0;
		return;
	}

	if (nEqGrade == 1 && !IsEqSFRSElem(ElemK))  // Modify by GAY. PMS:4076. ('10.03.04). Not Applied for Not SFRS Member.
	{
		rdEndF = 0.0;
		rdMidF = 0.0;
		return;
	}

	// Modify by GAY. PMS:4076. ('10.01.19). For KBC2009 Seismic Design.
	switch (nEqGrade)
	{
	case 1: // See KCI-USD07 Appendix II.4.2(2).
	{
		rdEndF = 1.0 / 2.0;
		rdMidF = 1.0 / 4.0;
	}
	break;
	case 2: // See KCI-USD07 21.3.2(1).
	{
		rdEndF = 1.0 / 3.0;
		rdMidF = 1.0 / 5.0;
	}
	break;
	case 3: // Not Applied.
	{
		rdEndF = 0.0;
		rdMidF = 0.0;
	}
	break;
	default:
	{
		ASSERT(0);
	}
	break;
	}
}

void CDgnForceCtrl::GetSeisReqMomentFactor_ACI318_Series(T_ELEM_K ElemK, int nCode, int nEqGrade, double& rdEndF, double& rdMidF)
{
	// Change by ZINU.('04.02.14). For Seismic Design.
	// See ACI318-89(한국어판) 21.3.2.2 p.160.
	// See ACI318-95(한국어판) 21.8.4.1 p.308.
	// See ACI318-99 21.3.2.2 p.308.
	// See ACI318-02 21.3.2.2 p.314.
	// See ACI318-11 21.5.2.2 (SMF), 21.3.4.1 (IMF)
	// by GAY. PMS:5009. ('15.03.31). add Colombia design code.

	const bool bConsiderCode = [](int nCode) -> bool
	{
		switch (nCode)
		{
		case ACI318_08:
		case ACI318_11:
		case ACI318_14:
		case ACI318M_14:
		case ACI318_19:
		case ACI318M_19:
		case ACI318_25:
		case ACI318M_25:
		case NSR_10:
		case NSCP_2015_C:
			return true;
		default:
			return false;
		}

	}(m_iDgnCode);

	// Add by GAY. PMS:4450. ('12.04.03).
	if (bConsiderCode)
	{
		if (!IsEqSpecialElem(ElemK))
		{
			// Modify by GAY. PMS:4076. ('10.03.04). Not Applied for Non-Seismic Design Member.     
			rdEndF = 0.0;
			rdMidF = 0.0;
			return;
		}
		if (nEqGrade == 1 && !IsEqSFRSElem(ElemK)) // Modify by GAY. PMS:4076. ('10.03.04). Not Applied for Not SFRS Member.
		{
			rdEndF = 0.0;
			rdMidF = 0.0;
			return;
		}
	}

	switch (nEqGrade)
	{
	case 1:
	{
		rdEndF = 1.0 / 2.0;
		rdMidF = 1.0 / 4.0;
	}
	break;
	case 2:
	{
		rdEndF = 1.0 / 3.0;
		rdMidF = 1.0 / 5.0;
	}
	break;
	case 3:
	{
		rdEndF = 0.0;
		rdMidF = 0.0;
	}
	break; // Not Applied. 
	default:
	{
		ASSERT(0);
	}
	break;
	}
}

void CDgnForceCtrl::GetSeisReqMomentFactor_NTC_Series(T_ELEM_K ElemK, int nCode, int nEqGrade, double& rdEndF, double& rdMidF)
{
	const bool bConsiderCode = [](int nCode) -> bool
	{
		switch (nCode)
		{
		case NTC_DCEC_2017:
			return true;
		default:
			return false;
		}

	}(m_iDgnCode);

	if (bConsiderCode)
	{
		if (!IsEqSpecialElem(ElemK))
		{
			// Modify by GAY. PMS:4076. ('10.03.04). Not Applied for Non-Seismic Design Member.     
			rdEndF = 0.0;
			rdMidF = 0.0;
			return;
		}
		if (nEqGrade == 1 && !IsEqSFRSElem(ElemK)) // Modify by GAY. PMS:4076. ('10.03.04). Not Applied for Not SFRS Member.
		{
			rdEndF = 0.0;
			rdMidF = 0.0;
			return;
		}
	}

	switch (nEqGrade)
	{
	case 1:
	{
		// 9.2.2
		rdEndF = 1.0 / 2.0;
		rdMidF = 1.0 / 4.0;
	}
	break;
	case 2:
	{
		// 8.2.2
		rdEndF = 1.0 / 3.0;
		rdMidF = 1.0 / 4.0;
	}
	break;
	case 3:
	{
		rdEndF = 0.0;
		rdMidF = 0.0;
	}
	break; // Not Applied. 
	default:
	{
		ASSERT(0);
	}
	break;
	}
}

void CDgnForceCtrl::SetUlctMap(OUT CMap<UINT, UINT, int, int>& rmUlct)
{
	rmUlct.RemoveAll();

	/// for BEAM/ COLUMN/ BRACE/ WALL	
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartUlct();
	while (pos)
	{
		T_ULCT_K UlctK = 0;	// =ElemK.
		T_ULCT_D UlctD;
		m_pDoc->m_pAttrCtrl->GetNextUlct(pos, UlctK, UlctD);

		T_ELEM_D ElemD;
		if (!m_pDoc->m_pAttrCtrl->GetElem(UlctK, ElemD)) { ASSERT(0); continue; }
		if (m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))	// Wall.
		{
			UINT unInterWallID = 0;
			if (!m_pDoc->m_pPostCtrl->GetInternalWallId(UlctK, unInterWallID)) { continue; }

			T_WALL_KEY WallKey = 0;
			if (!m_pDoc->m_pPostCtrl->GetWallKey(unInterWallID, WallKey)) { continue; }

			rmUlct.SetAt(WallKey, UlctD.nType);
		}
		else if (m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))	// Truss, Beam.
		{
			rmUlct.SetAt(UlctK, UlctD.nType);
		}
	}
}

bool CDgnForceCtrl::GetUnderGroundTypeByUlct(UINT ElemKorUnInterWallID, OUT int& rnType)
{
	UINT unKey = ElemKorUnInterWallID;
	if (m_iRcsTypeKind == RCS_WDGN || m_iRcsTypeKind == RCS_WCHK)
	{
		if (!m_pDoc->m_pPostCtrl->GetWallKey(ElemKorUnInterWallID, unKey))
		{
			ASSERT(0);
			return false;
		}
	}

	rnType = 0;
	return m_mUlct.Lookup(unKey, rnType);
}

bool CDgnForceCtrl::IsWall()
{
	switch (m_iRcsTypeKind)
	{
	case RCS_WDGN:
	case RCS_WCHK:
		return true;
	default:
		return false;
	}
}

bool CDgnForceCtrl::IsCalcWallDesignForce(int nElemNo, const _DGN_LCOM& LcomDesign)
{
	if (m_iDgnCode != Eurocode2_04 && m_iDgnCode != NTC_DCEC_2017) { return false; }
	if (!IsEqSpecialLcom(LcomDesign.DesignLcomKey)) { return false; }
	if (!IsEqSpecialElem(nElemNo)) { return false; }
	if (!m_pDoc->m_pAttrCtrl->IsWall(m_iElemType)) { return false; } // Only wall.
	if (IsUnderGroundMemb(nElemNo)) { return false; }
	if (m_iDgnCode == Eurocode2_04)
	{
		// dual system or wall system.
		switch (m_iFrameType)
		{
		case EN_EC8_FRAME_SYSTEM:
		case EN_EC8_TORSIONAL_FLEX_SYSYEM:
		case EN_EC8_INVERTED_PENDULUM_SYSTEM:
			return false;
		case EN_EC8_DUAL_SYSTEM:
		case EN_EC8_COUPLED_WALL_SYSTEM:
		case EN_EC8_UNCOUPLED_WALL_SYSTEM:
			return true;
		default:
			ASSERT(0);
			return false;
		}
	}

	return true;
}

bool CDgnForceCtrl::IsSlenderWallByEC8(double dTotalHeight, double dLength)
{
	const double dHw_Lw = dgn::SafeDiv(dTotalHeight, dLength);
	return dgn::MT(dHw_Lw, 2.0);
}

void CDgnForceCtrl::MakeAdjacentInternalWallIDMap(int nUserWallId)
{
	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;
	CPostCtrl* pPostCtrl = m_pDoc->m_pPostCtrl;

	CArray<UINT, UINT> aIntWallID;
	if (!pPostCtrl->GetInternalWallIdList(nUserWallId, aIntWallID)) { return; }

	std::vector<std::pair<double, UINT>> vIntIDLevel;
	const INT_PTR nSizeInter = aIntWallID.GetSize();
	for (INT_PTR m = 0; m < nSizeInter; ++m)
	{
		const UINT unCurID = aIntWallID[m];

		T_WALL_K InWallK;
		if (!pPostCtrl->GetWallKey(unCurID, InWallK.keymap)) continue;
		T_ELEM_K_LIST aElemK;
		if (!pPostCtrl->GetWallElemList(InWallK.keymap, aElemK)) continue;
		T_ELEM_D ElemD;
		if (!pAttrCtrl->GetElem(aElemK[0], ElemD)) continue;  // Only First Element.
		T_NODE_D BotNodeD;
		if (!pAttrCtrl->GetNode(ElemD.elnod[0], BotNodeD)) continue;
		T_NODE_D TopNodeD;
		if (!pAttrCtrl->GetNode(ElemD.elnod[3], TopNodeD)) continue;

		vIntIDLevel.emplace_back(std::pair<double, UINT>(BotNodeD.z, unCurID));
	}
	std::sort(vIntIDLevel.begin(), vIntIDLevel.end());

	const size_t nSizeSort = vIntIDLevel.size();
	for (int m = 0; m < nSizeSort; ++m)
	{
		const UINT unCur = vIntIDLevel[m].second;
		const UINT unBot = m == 0 ? 0 : vIntIDLevel[m - 1].second;
		const UINT unTop = m == nSizeSort - 1 ? 0 : vIntIDLevel[m + 1].second;

		m_mAdjIntwall.insert(std::make_pair(unCur, ADJ_INTWALLID_K(unBot, unTop)));
	}
}

bool CDgnForceCtrl::GetDgnLcomGroupMap(int nLcomKind, OUT CArray<_DGN_LCOM_MAP*, _DGN_LCOM_MAP*>& raLcomMap)
{
	raLcomMap.RemoveAll();

	bool bExist = true;
	switch (nLcomKind)
	{
	case D_LCOMTYPE_STEEL:
	{
		raLcomMap.Copy(m_arLcomStlGrup);
	}
	break;
	case D_LCOMTYPE_CONCRETE:
	{
		raLcomMap.Copy(m_arLcomConGrup);
	}
	break;
	case D_LCOMTYPE_SRC:
	{
		raLcomMap.Copy(m_arLcomSrcGrup);
	}
	break;
	case D_LCOMTYPE_ALUMINUM:
	{
		raLcomMap.Copy(m_arLcomAluGrup);
	}
	break;
	case D_LCOMTYPE_STLCOMP:
	{
		raLcomMap.Copy(m_arLcomScmGrup);
	}
	break;
	case D_LCOMTYPE_CFSTEEL:
	{
		raLcomMap.Copy(m_arLcomCfsGrup);
	}
	break;
	default:
	{
		bExist = false;
	}
	break;
	}
	return bExist;
}

CString CDgnForceCtrl::GetLcasLcomName(T_KEY LcasLcomK, int nLcomAnalType)
{
	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;

	CString strName = _T("");
	switch (nLcomAnalType)
	{
	case D_LCOM_STATIC:
	{
		T_STLD_D StldD;
		if (pAttrCtrl->GetStld(LcasLcomK, StldD))
		{
			strName = StldD.LoadCaseName;
		}
	}
	break;
	case D_LCOM_SPECTRUM:
	{
		T_SPLC_D SplcD;
		if (pAttrCtrl->GetSplc(LcasLcomK, SplcD))
		{
			strName = SplcD.LoadCaseName/* + _T("(RS)")*/;
		}
	}
	break;
	case D_LCOM_ECCENSPEC:
	{
		T_SPLC_D SplcD;
		if (pAttrCtrl->GetSplc(LcasLcomK, SplcD))
		{
			strName = SplcD.LoadCaseName/* + _T("(ES)")*/;
		}
	}
	break;
	case D_LCOM_HISTORY:
	{
		T_THIS_D ThisD;
		if (pAttrCtrl->GetThis(LcasLcomK, ThisD))
		{
			strName = ThisD.LoadCaseName;
		}
	}
	break;
	case D_LCOM_CRANE:
	{
		T_CRAN_D CranD;
		if (pAttrCtrl->GetCran(LcasLcomK, CranD))
		{
			strName = CranD.LoadTypeName;
		}
	}
	break;
	case D_LCOM_MOVING:
	{
		strName = GetMovingLcaseName(LcasLcomK);
	}
	break;
	case D_LCOM_SETTLEMENT:
	{
		T_SMLC_D SmlcD;
		if (pAttrCtrl->GetSmlc(LcasLcomK, SmlcD))
		{
			strName = SmlcD.LoadCaseName;
		}
	}
	break;
	case D_LCOM_STAGE:
	{
		T_SGLD_D SgldD;
		if (!m_pDoc->m_pAttrCtrl->GetSgld(LcasLcomK, SgldD))
		{
			strName = SgldD.LoadCaseName;
		}
	}
	break;
	case D_LCOM_CB_STEEL:
	case D_LCOM_CB_CFSTEEL:
	case D_LCOM_CB_CONCRETE:
	case D_LCOM_CB_SRC:
	case D_LCOM_CB_ALUMINUM:
	case D_LCOM_CB_STLCOMP:
	case D_LCOM_CB_SEISMIC:
	case D_LCOM_CB_LINEAR_EVAL:
	{
		const int nLcomType = m_pDoc->m_pAttrCtrl2->GetLcomTypeByAnalType(nLcomAnalType);
		T_LCOM_D LcomD;
		if (pAttrCtrl->GetLcom(nLcomType, LcasLcomK, LcomD))
		{
			strName = LcomD.LoadCombName;
		}
	}
	break;
	default:
	{
		ASSERT(0);
	}
	break;
	}
	return strName;
}

CString CDgnForceCtrl::GetMovingLcaseName(T_KEY LcasK)
{
	T_MVCD_D MvcdD;
	if (!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) { return _T(""); }

	CString strName = _T("");
	switch (MvcdD.nCodeType)
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
	{
		T_MVLD_D MvldD;
		if (m_pDoc->m_pAttrCtrl->GetMvld(LcasK, MvldD))
		{
			strName = MvldD.LoadCaseName;
		}
	}
	break;
	case D_MOVE_CODE_JAPAN:
	{
		T_MVLDjp_D MvldJPD;
		m_pDoc->m_pAttrCtrl->InitializeMvldjp(MvldJPD);
		if (m_pDoc->m_pAttrCtrl->GetMvldjp(LcasK, MvldJPD))
		{
			strName = MvldJPD.LoadCaseName;
		}
	}
	break;
	case D_MOVE_CODE_CHINA:
	{
		T_MVLDch_D MvldCHD;
		if (m_pDoc->m_pAttrCtrl->GetMvldch(LcasK, MvldCHD))
		{
			strName = MvldCHD.LoadCaseName;
		}
	}
	break;
	case D_MOVE_CODE_INDIA:
	{
		T_MVLDid_D MvldIDD;
		if (m_pDoc->m_pAttrCtrl->GetMvldid(LcasK, MvldIDD))
		{
			strName = MvldIDD.LoadCaseName;
		}
	}
	break;
	case D_MOVE_CODE_BS:
	{
		T_MVLDbs_D MvldBSD;
		if (m_pDoc->m_pAttrCtrl->GetMvldbs(LcasK, MvldBSD))
		{
			strName = MvldBSD.LoadCaseName;
		}
	}
	break;
	case D_MOVE_CODE_FRANCE:
	{
		T_MVLDfr_D MvldfrD;
		if (m_pDoc->m_pAttrCtrl->GetMvldfr(LcasK, MvldfrD))
		{
			strName = MvldfrD.LoadCaseName;
		}
	}
	break;
	case D_MOVE_CODE_TRANS:
	{
		T_MVLDtr_D MvldTRD;
		if (m_pDoc->m_pAttrCtrl->GetMvldtr(LcasK, MvldTRD))
		{
			strName = MvldTRD.LoadCaseName;
		}
	}
	break;
	default:
	{
		ASSERT(0);
	}
	break;
	}
	return strName;
}

void CDgnForceCtrl::AddDeadSum(const int& iPosi,
	const double& dFxx, const double& dFyy, const double& dFzz,
	const double& dBMx, const double& dBMy, const double& dBMz,
	const double& dMbi, const double& dTst, const double& dTwp)
{
	m_FxxD[iPosi] += dFxx;
	m_FyyD[iPosi] += dFyy;
	m_FzzD[iPosi] += dFzz;
	m_BMxD[iPosi] += dBMx;
	m_BMyD[iPosi] += dBMy;
	m_BMzD[iPosi] += dBMz;
	m_MbiD[iPosi] += dMbi;
	m_TstD[iPosi] += dTst;
	m_TwpD[iPosi] += dTwp;
}

void CDgnForceCtrl::AddLiveSum(const int& iPosi,
	const double& dFxx, const double& dFyy, const double& dFzz,
	const double& dBMx, const double& dBMy, const double& dBMz,
	const double& dMbi, const double& dTst, const double& dTwp)
{
	m_FxxL[iPosi] += dFxx;
	m_FyyL[iPosi] += dFyy;
	m_FzzL[iPosi] += dFzz;
	m_BMxL[iPosi] += dBMx;
	m_BMyL[iPosi] += dBMy;
	m_BMzL[iPosi] += dBMz;
	m_MbiL[iPosi] += dMbi;
	m_TstL[iPosi] += dTst;
	m_TwpL[iPosi] += dTwp;
}

void CDgnForceCtrl::AddMovingLiveSum(const int& iPosi,
	const double& dFxx, const double& dFyy, const double& dFzz,
	const double& dBMx, const double& dBMy, const double& dBMz,
	const double& dMbi, const double& dTst, const double& dTwp)
{
	m_FxxML[iPosi] += dFxx;
	m_FyyML[iPosi] += dFyy;
	m_FzzML[iPosi] += dFzz;
	m_BMxML[iPosi] += dBMx;
	m_BMyML[iPosi] += dBMy;
	m_BMzML[iPosi] += dBMz;
	m_MbiML[iPosi] += dMbi;
	m_TstML[iPosi] += dTst;
	m_TwpML[iPosi] += dTwp;
}

void CDgnForceCtrl::AddRemnantSum(const int& iPosi,
	const double& dFxx, const double& dFyy, const double& dFzz,
	const double& dBMx, const double& dBMy, const double& dBMz,
	const double& dMbi, const double& dTst, const double& dTwp,
	const double& dFyyOrg, const double& dFzzOrg)
{
	m_FxxR[iPosi] += dFxx;
	m_FyyR[iPosi] += dFyy;
	m_FzzR[iPosi] += dFzz;
	m_BMxR[iPosi] += dBMx;
	m_BMyR[iPosi] += dBMy;
	m_BMzR[iPosi] += dBMz;
	m_MbiR[iPosi] += dMbi;
	m_TstR[iPosi] += dTst;
	m_TwpR[iPosi] += dTwp;
	m_FyyOrgR[iPosi] += dFyyOrg;
	m_FzzOrgR[iPosi] += dFzzOrg;
}

void CDgnForceCtrl::SetDataForSeismic()
{
	T_DCON_D DconD;
	DconD.Initialize();
	m_pDgnDataCtrl->Get_DgnConDcon(DconD);

	m_iDgnCode = m_pDgnDataCtrl->GetConCodeEnum(DconD.DesignCode);
	m_iDgnClass = DconD.nClass + 1;
	m_bEqSpecial = DconD.bSpecialSeismic;

	// PMS:4182-Seungjun-20100608 KBC2009 Seismic design.    
	switch (m_iDgnCode)
	{
	case KCI_USD07:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case IS456_2000:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case NSR_10:
	case NSCP_2015_C:
	case TWN_USD112:
	case NTC_DCEC_2017:
	case NTC_DCEC_2023:
	{
		// T_DCON_D Data setting.
		m_bAutoSeisSubBeam = DconD.bNonSeisSubBeam;
		m_bAutoSeisCantilever = DconD.bNonSeisCantilever;
		m_bAutoSeisUnderBeamColm = DconD.bNonSeisUnderGround;
	}
	break;
	case GB50010_10:
	case GB50010_19:
	{
		m_bAutoSeisSubBeam = TRUE;
		m_bAutoSeisCantilever = FALSE;
		m_bAutoSeisUnderBeamColm = FALSE;
	}
	break;
	case AIJ_WSD99:
	case CSA_A233_94:
	case AIK_WSD2K:
	case Eurocode2_04:
	{
		m_bAutoSeisSubBeam = TRUE;
		m_bAutoSeisCantilever = TRUE;
		m_bAutoSeisUnderBeamColm = TRUE;
	}
	break;
	default:
	{
		m_bAutoSeisSubBeam = FALSE;
		m_bAutoSeisCantilever = FALSE;
		m_bAutoSeisUnderBeamColm = FALSE;
	}
	break;
	}

	if (m_iDgnCode == Eurocode2_04 || m_iDgnCode == Eurocode2_2_05) // PMS.5537 Civil Shell Design Eurocode2_2_05
	{
		T_GRUP_K GroupKey = DconD.SecondaryGroupKey;
		T_GRUP_D GrupD; GrupD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetGrup(GroupKey, GrupD))
		{
			int iElemNum = GrupD.arKeyElem.GetSize();
			for (int kk = 0; kk < iElemNum; kk++)
			{
				T_ELEM_K ElemK = GrupD.arKeyElem[kk];
				m_arSecondaryElemList.SetAt(ElemK, TRUE);
			}
		}
	}
}

void CDgnForceCtrl::SetDataForSeismic4Steel()
{
	T_DSTL_D DstlD;
	DstlD.Initialize();
	m_pDgnDataCtrl->Get_DgnStlDstl(DstlD);

	m_iDgnCode = m_pDgnDataCtrl->GetStlCodeEnum(DstlD.DesignCode);
	m_bEqSpecial = DstlD.bSpecialSeismic;

	if (m_iDgnCode == Eurocode3_05 || m_iDgnCode == Eurocode3_2_05)
	{
		T_GRUP_K GroupKey = DstlD.NonSeismicGroupKey;
		T_GRUP_D GrupD; GrupD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetGrup(GroupKey, GrupD))
		{
			int iElemNum = GrupD.arKeyElem.GetSize();
			for (int kk = 0; kk < iElemNum; kk++)
			{
				T_ELEM_K ElemK = GrupD.arKeyElem[kk];
				m_arSecondaryElemList.SetAt(ElemK, TRUE);
			}
		}
	}
	else if (m_iDgnCode == IS800_2007)
	{
		CArray<T_ELEM_K, T_ELEM_K> aAllKeys;
		CArray<T_ELEM_K, T_ELEM_K> aSlcmKeys;
		m_pDoc->m_pAttrCtrl->GetElemKeyList(aAllKeys, FALSE);
		POSITION PosSlcm = m_pDoc->m_pAttrCtrl->GetStartSlcm();
		while (PosSlcm)
		{
			T_SLCM_K SlcmK = 0;	// =ElemK.
			T_SLCM_D SlcmD;
			m_pDoc->m_pAttrCtrl->GetNextSlcm(PosSlcm, SlcmK, SlcmD);

			T_ELEM_D ElemD;
			if (!m_pDoc->m_pAttrCtrl->GetElem(SlcmK, ElemD)) { ASSERT(0); continue; }
			if (m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))	// Wall.
			{
				continue;
			}
			else if (m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))	// Truss, Beam.
			{
				aSlcmKeys.Add(SlcmK);
			}
		}
		for (int i = 0; i < aAllKeys.GetSize(); i++)
		{
			T_ELEM_K currKey = aAllKeys.GetAt(i);
			bool bSpecialKey = false;
			for (int j = 0; j < aSlcmKeys.GetSize(); j++) 
			{
				if (currKey == aSlcmKeys.GetAt(j))
				{
					bSpecialKey = true; break;
				}
			}
			if (!bSpecialKey) { m_arSecondaryElemList.SetAt(currKey, TRUE); }
		}
	}
}

void CDgnForceCtrl::SetMemberDataForSeismic(const UINT& iMemb)
{
	m_iElemKind = m_pDoc->m_pAttrCtrl->GetMemberType(iMemb);
	m_iCantilever = IsCantilever(iMemb);
	m_bSubBeamForNoMpr = IsSubBeamForNoMpr(iMemb, m_iCantilever);
	m_bLowMembForNoMpr = IsUnderGroundMemb(iMemb);
}

bool CDgnForceCtrl::GetDgnElemMatd(T_ELEM_K ElemK, OUT T_MATD_D& rMatdD)
{
	rMatdD.Initialize();

	if (m_mElemMatd.count(ElemK) > 0)
	{
		rMatdD = m_mElemMatd[ElemK];
		return true;
	}

	if (!m_pDgnDataCtrl->Get_DgnGenMatd(ElemK, rMatdD))
	{
		return false;
	}

	m_mElemMatd[ElemK] = rMatdD;
	return true;
}

bool CDgnForceCtrl::GetDgnWallMatd(T_WALL_KEY WallKey, OUT int& rnMatlK, OUT T_MATD_D& rMatdD)
{
	rnMatlK = 0;
	rMatdD.Initialize();

	if (m_mWallMatd.count(WallKey) > 0)
	{
		rMatdD = m_mWallMatd[WallKey];
		return true;
	}

	if (!m_pDgnDataCtrl->Get_DgnWalMatd(WallKey, rnMatlK, rMatdD))
	{
		return false;
	}

	m_mWallMatd[WallKey] = rMatdD;
	return true;
}

bool CDgnForceCtrl::GetPerformDgnLcomIdx(const UINT& iSerialKey, UINT& DgnLcomIdx)
{
	_DGN_LCOM CurDgnLcom;
	if (!m_parLcomData->Lookup(iSerialKey, CurDgnLcom))
	{
		ASSERT(0);
		return m_mPerformDgnLcomIdx.Lookup(iSerialKey, DgnLcomIdx);
	}

	POSITION pos = m_mPerformDgnLcomIdx.GetStartPosition();
	while (pos)
	{
		UINT iKey = 0;
		m_mPerformDgnLcomIdx.GetNextAssoc(pos, iKey, DgnLcomIdx);
		_DGN_LCOM DgnLcomD;
		if (!m_arLcomTotl.Lookup(iKey, DgnLcomD)) continue;
		if (CurDgnLcom.bMaxLcom == DgnLcomD.bMaxLcom &&
			CurDgnLcom.OriginalLcomKey == DgnLcomD.OriginalLcomKey &&
			CurDgnLcom.iLcomCompType == DgnLcomD.iLcomCompType)
		{
			return m_mPerformDgnLcomIdx.Lookup(iKey, DgnLcomIdx);
		}
	}
	return m_mPerformDgnLcomIdx.Lookup(iSerialKey, DgnLcomIdx);
}

void CDgnForceCtrl::SetCurrentUnit()
{
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(m_CurUnit);
}

T_UNIT_INDEX CDgnForceCtrl::GetCurrentUnit() const
{
	return m_CurUnit;
}

#include "DgnAnalRsltConverter.h"
#include "DgnAnalRsltCtrl.h"
bool CDgnForceCtrl::Set_DgnAnalRslt(T_KEY_LIST& aElem)
{
	//중립단위계
	CCurUnitSaver Save(TRUE);
	T_UNIT_INDEX ChangeIndex;
	ChangeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
	ChangeIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(ChangeIndex);

	CString csDirectProduct = CProduct::GetTestEnvValue(_ULS(DirectProduct));
	bool bDirectProduct = csDirectProduct.CompareNoCase(_ULS(no)) == 0 ? false : true;

	if (m_pAnalRsltConverter == nullptr)
	{
		T_KEY_LIST aForceElem;
		const BOOL bWithoutDummy = TRUE;
		m_pDoc->m_pAttrCtrl->GetElemKeyList(aForceElem, bWithoutDummy);

		m_pAnalRsltConverter = new CDgnAnalRsltConverter(m_pDoc, aForceElem, bDirectProduct);
	}
	if (m_pAnalRsltCtrl == nullptr && m_pAnalRsltConverter != nullptr)
	{
		m_pAnalRsltCtrl = new CDgnAnalRsltCtrl(m_pAnalRsltConverter, m_pDoc);
		if (!m_pAnalRsltCtrl->Initialize())
		{
			ASSERT(0); return false;
		}
	}

	return (m_pAnalRsltConverter == nullptr || m_pAnalRsltCtrl == nullptr) ? false : true;
}

void CDgnForceCtrl::Del_DgnAnalRslt()
{
	if (m_pAnalRsltCtrl != nullptr)
	{
		if (!m_pAnalRsltCtrl->Finalize()) ASSERT(0);
	}
	if (m_pAnalRsltConverter != nullptr)
	{
		if (!m_pAnalRsltConverter->Finalize()) ASSERT(0);
		SafeDelete(m_pAnalRsltConverter);
	}
	SafeDelete(m_pAnalRsltCtrl);
}

bool CDgnForceCtrl::IsJointCodeUseVcol(const int& iCode)
{
	switch (iCode)
	{
	case ACI318_14:
	case ACI318M_14:
	case NSR_10:
	case NSCP_2015_C:
	case NTC_DCEC_2017:
	case NTC_DCEC_2023:
		return true;
	default:
		break;
	}

	return false;
}

// PMS:4020-Seungjun-20090723 EC2:04 Slab/Wall Design.
UINT CDgnForceCtrl::Get_LowerColmNo(UINT NodeK)
{
	UINT ElemK = 0;

	CArray<T_ELEM_K, T_ELEM_K> arElemList;
	m_pDoc->m_pAttrCtrl->GetConnectedElem(NodeK, arElemList);  // 절점에 연결된 element의 key를 가져온다.

	T_NODE_D JointNode;
	m_pDoc->m_pAttrCtrl->GetNode(NodeK, JointNode);

	BOOL bIsJUD = m_pDgnDataCtrl->IsJUD();
	int nElem = arElemList.GetSize();
	for ( int IEL = 0; IEL < nElem; IEL++ )
	{
		T_ELEM_K KeyElem = arElemList.GetAt(IEL);
		int iCBM = bIsJUD ? m_pDoc->m_pAttrCtrl->GetMemberType_JUD(KeyElem) : m_pDoc->m_pAttrCtrl->GetMemberType(KeyElem);
		if ( iCBM == D_MBTP_COLUMN || iCBM == D_MBTP_BRACE ) // Seungjun-20140218 : 펀칭 검토시 truss도 포함
		{
			T_ELEM_D elemD; elemD.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(KeyElem, elemD);

			T_NODE_D OppsiNode; OppsiNode.Initialize();
			if ( elemD.elnod[0] == NodeK )
			{
				m_pDoc->m_pAttrCtrl->GetNode(elemD.elnod[1], OppsiNode);
			}
			else
			{
				m_pDoc->m_pAttrCtrl->GetNode(elemD.elnod[0], OppsiNode);
			}

			if ( JointNode.z > OppsiNode.z )
			{
				ElemK = arElemList[IEL];
				if ( iCBM == D_MBTP_COLUMN || IEL == (nElem - 1) ) // brace보다 column 우선
				{
					break;
				}
			}
		}
	}

	return ElemK;
}

// MQC:3642-Seungjun-20100908 상부 기둥이 존재하는 경우.
UINT CDgnForceCtrl::Get_UpperColmNo(UINT NodeK)
{
	UINT ElemK = 0;

	CArray<T_ELEM_K, T_ELEM_K> arElemList;
	m_pDoc->m_pAttrCtrl->GetConnectedElem(NodeK, arElemList);  // 절점에 연결된 element의 key를 가져온다.

	T_NODE_D JointNode;
	m_pDoc->m_pAttrCtrl->GetNode(NodeK, JointNode);

	BOOL bIsJUD = m_pDgnDataCtrl->IsJUD();
	int nElem = arElemList.GetSize();
	for ( int IEL = 0; IEL < nElem; IEL++ )
	{
		T_ELEM_K KeyElem = arElemList.GetAt(IEL);
		int iCBM = bIsJUD ? m_pDoc->m_pAttrCtrl->GetMemberType_JUD(KeyElem) : m_pDoc->m_pAttrCtrl->GetMemberType(KeyElem);
		if ( iCBM == D_MBTP_COLUMN || iCBM == D_MBTP_BRACE ) // Seungjun-20140218 : 펀칭 검토시 truss도 포함
		{
			T_ELEM_D elemD; elemD.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(KeyElem, elemD);

			T_NODE_D OppsiNode; OppsiNode.Initialize();
			if ( elemD.elnod[0] == NodeK )
			{
				m_pDoc->m_pAttrCtrl->GetNode(elemD.elnod[1], OppsiNode);
			}
			else
			{
				m_pDoc->m_pAttrCtrl->GetNode(elemD.elnod[0], OppsiNode);
			}

			if ( JointNode.z < OppsiNode.z )
			{
				ElemK = arElemList[IEL];
				if ( iCBM == D_MBTP_COLUMN || IEL == (nElem - 1) ) // brace보다 column 우선
					break;
			}
		}
	}

	return ElemK;
}

bool CDgnForceCtrl::Is_BeamPoint(UINT NodeK, bool bExcludeEbeam)
{
	CArray<T_ELEM_K, T_ELEM_K> arElemList;
	m_pDoc->m_pAttrCtrl->GetConnectedElem(NodeK, arElemList);  // 절점에 연결된 element의 key를 가져온다.

	BOOL bIsJUD = m_pDgnDataCtrl->IsJUD();
	int nElem = arElemList.GetSize();
	for (int IEL = 0; IEL < nElem; IEL++)
	{
		T_ELEM_K KeyElem = arElemList.GetAt(IEL);
		int iCBM = bIsJUD ? m_pDoc->m_pAttrCtrl->GetMemberType_JUD(KeyElem) : m_pDoc->m_pAttrCtrl->GetMemberType(KeyElem);
		if (iCBM == D_MBTP_BEAM) // Seungjun-20140218 : 펀칭 검토시 truss도 포함
		{
			if(bExcludeEbeam)
			{
				if(m_pDoc->m_pAttrCtrl2->IsEBeamK(KeyElem)) { continue; }

				T_MEMB_K MembK = 0;
				if (m_pDoc->m_pAttrCtrl->GetMembAssigned(KeyElem, MembK))
				{
					if (m_pDoc->m_pAttrCtrl2->GetMstrKByMemb(MembK) > 0)       { continue; }
				}
			}
			return true;
		}
	}

	return false;
}

CString CDgnForceCtrl::Get_OrgLcomName(const _DGN_LCOM& Data, bool bPlate)
{
	CString strLcomName = Data.LcomUlData.LoadCombName;

	if(Data.iLcomCompType==0)
	{
		const int  nLength = Data.DesignLcomNa.GetLength();
		TCHAR endChar = Data.DesignLcomNa.GetAt(nLength - 1);
		     if (endChar == '+') strLcomName += _T("+");
		else if (endChar == '-') strLcomName += _T("-");
	}
	else
	{
		CArray<CString, CString&> aCompString;
		GetLcomCompTypeString(bPlate ? D_RC_PLATE_BC : D_RC_GEN, aCompString);

		const int     iLcomCompType = Data.iLcomCompType-1;
		const CString strSign = Data.bMaxLcom ? _T("+") : _T("-");
		const CString strComp = aCompString[iLcomCompType];
		strLcomName += (strSign+strComp);
	}

	return strLcomName;
}

bool CDgnForceCtrl::IsIncludeServiceCode()
{
	switch(m_iDgnCode)
	{
		case IRC_112_11:
		case IRC_112_20:
		return true;
		case Eurocode2_2_05:
		return false;
		case IS456_2000:
		case ACI318_14:
		case ACI318M_14:
		case ACI318_19:
		case ACI318M_19:
		case ACI318_25:
		case ACI318M_25:
		case NSR_10:
		case NSCP_2015_C:
		case NTC_DCEC_2017:
		case NTC_DCEC_2023:
		case TWN_USD112:
		return m_iRcsTypeKind == RCS_SSEV ? false : true;
		default:
		return false;
	}
}

bool CDgnForceCtrl::IsIncludeEnvelopCode()
{
	switch (m_iDgnCode)
	{
	case AASHTO_LS17:
	case AASHTO_LS20:
	case AASHTO_LS24:
		return true;
	default:
		return false;
	}
	return false;
}

bool CDgnForceCtrl::IsSteelConcurrentForceCode()
{
	switch (m_iDgnCode)
	{
	case KSCE_ASD05:
	case KSCE_RAIL_ASD04:
	case KSCE_ASD10:
	case KSCE_RAIL_ASD11:
	case KR_BRG_LSD15:
	case KDS_24_14_31_2018:
	case KDS_24_14_30_2019:
	case AASHTO_LS17:
	case AASHTO_LS20:
	case AASHTO_LS24:
		return true;
	default:
		return false;
	}

	return false;
}