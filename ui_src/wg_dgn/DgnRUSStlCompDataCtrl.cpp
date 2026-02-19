// DgnRUSStlCompDataCtrl.cpp: implementation of the CDgnRUSStlCompDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "DgnRUSStlCompDataCtrl.h"
#include "DgnRUSStlCompManager.h"
#include "CRCForceCtrl.h"
#include "DgnDataCtrl.h"
#include "DgnForceCtrl.h"
#include "DgnEngineProgressDlg.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_I_PolyMaker.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_db\ViewCtrl.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\SectDB.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\RUSStlCompGirderDesign.h"
#include "..\wg_dbLock\LockMgr.h"

#include "Dgn_DllManager_Base.h"
#include "..\dgnengine\idesign\DGN_link\ITmpDgnDrawLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FLANGE_LEFT   0
#define FLANGE_MIDDLE 1
#define FLANGE_RIGHT  2

#define SN_WEB_NoM   0
#define SN_WEB_VON   1
#define SN_WEB_RIB   2
#define SN_WEB_COM   3
#define SN_WEB_MEAN  4

#define SN_DECK      0
#define SN_RIB       1
#define SN_FLOORBEAM 2

#define SB_DECK_G      0
#define SB_DECK_L      1
#define SB_RIB         2
#define SB_FLOORBEAM_G 3
#define SB_FLOORBEAM_L 4

#define SB_WEB_DECK  0
#define SB_WEB_RIB   1


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnRUSStlCompDataCtrl::CDgnRUSStlCompDataCtrl()
{
	m_pStlCompManager = new CDgnRUSStCompManager;

	m_pDoc = CDBDoc::GetDocPoint();
	m_nDgnCode = m_pDoc->GetCivilCodeDgn();

	m_pStlCompDesign = m_pDoc->m_pPostCtrl->GetRUSStlCompGirderDesign();
}

CDgnRUSStlCompDataCtrl::~CDgnRUSStlCompDataCtrl()
{
	if (m_pStlCompManager != NULL)
	{
		delete m_pStlCompManager;
		m_pStlCompManager = NULL;
	}
}

void CDgnRUSStlCompDataCtrl::SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl)
{
	m_pDataCtrl = pDataCtrl;
	if (m_pDataCtrl == NULL) { ASSERT(0); return; }

	// Set ForceCtrl Pointer.
	m_pForcCtrl = m_pDataCtrl->Get_ForceCtrlPointer();
	if (m_pForcCtrl == NULL) { ASSERT(0); return; }

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	// Set Code Unit.
	T_UNIT_INDEX CodeUnit = CurrUnit;
	switch (m_nDgnCode)
	{
	case SNiP_20503_84_CSG:
	case SP_35_13330_11_CSG:
		CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		CodeUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
		m_pDataCtrl->Set_UnitIndex(CodeUnit);
		break;
	default: ASSERT(0); break;
	}
	InitialData(m_nDgnCode);
}

BOOL CDgnRUSStlCompDataCtrl::InitialData(int nDgnCode)
{
	if (!m_pStlCompManager->InitialData(DGNE_CODETYPE_CSG, nDgnCode)) return FALSE;

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::ExcuteStlCompGirderDesign()
{
	BOOL bDesignOK = TRUE; // FALSE -> TRUE 수정 : 판단하는 곳이 없음. 수정 필요!!!
	if (!m_pDoc->IsPostMode()) return FALSE;

	T_CPGD_D CpgdD;
	if (!m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD)) return FALSE;
	if (CpgdD.iDgnCode != SNiP_20503_84_CSG && CpgdD.iDgnCode != SP_35_13330_11_CSG) return FALSE;

	BOOL bDesign = FALSE;
	for (int i = 0; i < 5; ++i) { if (CpgdD.bDesignParameter[i]) { bDesign = TRUE; break; } }
	if (!bDesign) return FALSE;

	m_pDoc->SetDesignStatus(7);

	// Make Design Data
	if (!Make_DesignBaseData())
	{
		return FALSE;
	}

	T_STCT_D StctD; StctD.Initialize();
	m_pDoc->m_pAttrCtrl->GetStct(StctD);

	// Pre-Combined Composite Bridge
	BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb();

	// Span 정보 계산 
	ArrElemPairKey arTempElemK;
	m_pDataCtrl->Calc_LengthPosition4Span(arTempElemK);  // ???
	CDBLib::GetElemKeyList4ElemPairKey(arTempElemK, m_arElemK);

	int nLcomK4CS = 0;

	double dFactorDeadCS = 1.0;
	// AASHTO에 해당하는 내용임 SNiP에는 적용여부 확인해야함 ???
	//if (!GetDeadLoadCSMaxFactor(nLcomK4CS, dFactorDeadCS)) dFactorDeadCS = 1.0;
	m_ChkItemD.Initialize();
	STL_COMP_MEMB_POSD_SNiP MembLcomD;

	// Get SNiP and SP Table Data
	STL_SNiP_TB_D SNiP_SP_TableD;
	m_pStlCompManager->Get_StlCompTableValue(SNiP_SP_TableD);

	// Check Item Setting
	BOOL bChkStr = CpgdD.bDesignParameter[0];
	BOOL bChkStb = CpgdD.bDesignParameter[1];
	BOOL bChkFati = CpgdD.bDesignParameter[2];
	BOOL bChkServ = CpgdD.bDesignParameter[4];
	BOOL bChkShearConn = CpgdD.bDesignParameter[3];
	BOOL bChkKappaF = bChkStr || bChkStb || bChkFati || bChkServ;

	int nOrgLcomNo = 0;
	int nSerLcomCount = 0;
	int nMaxMinType = 0;
	CString strOrgLcomNa = _T("");

	// Start Element I & J Check
	INT_PTR nChkElemNum = m_arElemK.GetSize();
	for (INT_PTR nElem = 0; nElem < nChkElemNum; ++nElem)   // Repeat Element
	{
		MembLcomD.Initialize();
		Convert_GlobalDgnParamD(MembLcomD.GlobalDgnD);
		INT_PTR nChkLcomSize = 1;
		MembLcomD.CheckLcomD.aUlmf.SetSize(nChkLcomSize);
		MembLcomD.CheckLcomD.GlobalForceD.SetSize(nChkLcomSize);

		T_ELEM_K ChkElemK = m_arElemK[nElem];
		ElemPairK ChkEPairK(ChkElemK, EN_EL_BEAM);
		T_ELEM_D ChkElemD; ChkElemD.Initialize();
		if (!m_pDoc->m_pPostCtrl->GetElemPost(ChkElemK, ChkElemD)) continue;
		int nStype = m_pDoc->m_pAttrCtrl->GetRealSectType(ChkElemD.elpro);

		T_STAG_K nGdLastStagK = 0;
		int nLastStepNo = 0;
		m_pForcCtrl->GetGirderLastStageKey(ChkEPairK, nGdLastStagK, nLastStepNo);
		int nForceBeforeType = 1;

		//////////////////////////////////////////////////////////////////////////
		// Construction Stage (Before Composite) Check - Start
		//////////////////////////////////////////////////////////////////////////
		if (bChkStr || bChkStb) // 강도와 좌굴 검토가 있는 경우에만 검토함
		{
			CStageInfo *pStageInfo = m_pDoc->m_pPostCtrl->GetStageInfo();
			CArray<T_STAG_K, T_STAG_K> aStagK;
			pStageInfo->GetResultSavedStagKeyList(aStagK);
			INT_PTR nStageSize = aStagK.GetSize();

			if (nStageSize == 0 && bIsPLCB)
			{
				MembLcomD.Memb[0].ChIf.bIsPLCB = TRUE;
				MembLcomD.Memb[1].ChIf.bIsPLCB = TRUE;
				MembLcomD.Memb[0].ChIf.dFactorCS = 1.0;
				MembLcomD.Memb[1].ChIf.dFactorCS = 1.0;
				Check_ConstructionNoStage(ChkElemK, nLcomK4CS, MembLcomD);
			}
			else if (nStageSize != 0)
			{
				MembLcomD.Memb[0].ChIf.dFactorCS = dFactorDeadCS;
				MembLcomD.Memb[1].ChIf.dFactorCS = dFactorDeadCS;
				Check_ConstructionStage(ChkElemK, nLcomK4CS, MembLcomD);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Construction Stage (Before Composite)  Check - End
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// Post Construction Stage (After Composite) Check - Start
		//////////////////////////////////////////////////////////////////////////
		T_SPAN_K CurSpanK = 0;
		T_ELEM_K nElemI = ChkElemK, nElemM = ChkElemK, nElemJ = ChkElemK;
		int nPosI = 0;
		int nPosM = 0; // 임시, 찾아야함. 
		int nPosJ = 1;

		// 시공중 거더 Force

		int nFortype = 0;
		int nLcomNum = m_pForcCtrl->Get_LcomDataCount();

		ADGNFORCE aPreForceLcom;
		if (bIsPLCB)
		{
			m_pForcCtrl->Get_BeforeForce4PreCompo(ChkEPairK, aPreForceLcom, nLcomNum);
		}

		ADGNFORCE aForceLcom;
		ADGNFORCE aForceTnpr;
		ADGNFORCE aForceTnsc;
		ADGNFORCE aForceLTerm;
		ADGNFORCE aForceSTerm;
		ADGNSHRSADD aStssLcom;
		if (!m_pForcCtrl->Get_LcomDataForDesign(ChkEPairK, TRUE, TRUE, aForceLcom, aForceTnpr, aForceTnsc, aForceLTerm, aForceSTerm, aStssLcom, nLcomNum))
		{
			CString strErrMsg;
			strErrMsg.Format(_LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_DGN_LCOM)); GSaveHistoryFormatNF(strErrMsg);
			return FALSE;
		}

		ADGNFORCE aForceCreep, aForceShrinkage;
		m_pForcCtrl->Get_LcomForce4ScfpCrp(ChkEPairK, nLcomNum, aForceCreep);
		m_pForcCtrl->Get_LcomForce4ScfpShr(ChkEPairK, nLcomNum, aForceShrinkage);

		ASCFPSTRESS aScfpCr, aScfpSh, aScfpTe, aScfpRe;
		bool bExistCrp = false, bExistShr = false, bExistBST = false;
		if (!m_pForcCtrl->Get_LcomStressScfp(ChkEPairK, nLcomNum, aScfpCr, aScfpSh, aScfpTe, aScfpRe)) return FALSE;  // Get Internal Stress


		T_LCOM_K_LIST aKapaChkLcomKList, aStrnLcomKList, aFatigueOrgLcomKList, aServLcomKeyList;
		CArray<T_FATI_LCOM_K_LIST, T_FATI_LCOM_K_LIST&> aFatigueLcomKList;
		Get_DgnLcomKeyList4StlCompDesign(aForceLcom, aKapaChkLcomKList, aStrnLcomKList, aFatigueOrgLcomKList, aFatigueLcomKList, aServLcomKeyList);

		ADGNFORCE aForceEndI;
		ElemPairK ElemIPairK(nElemI, EN_EL_BEAM);
		BOOL bIsSpanForI = m_pForcCtrl->Get_LcomDataForDesign(ElemIPairK, nFortype, TRUE, TRUE, aForceEndI, nLcomNum);

		// 중간은???
		ADGNFORCE aForceMid;
		ElemPairK ElemMPairK(nElemM, EN_EL_BEAM);
		BOOL bIsSpanForM = m_pForcCtrl->Get_LcomDataForDesign(ElemMPairK, nFortype, TRUE, TRUE, aForceMid, nLcomNum);

		ADGNFORCE aForceEndJ;
		ElemPairK ElemJPairK(nElemJ, EN_EL_BEAM);
		BOOL bIsSpanForJ = m_pForcCtrl->Get_LcomDataForDesign(ElemJPairK, nFortype, TRUE, TRUE, aForceEndJ, nLcomNum);

		// Get Check Lcom Num
		INT_PTR nChkKappaChkLcomNum = aKapaChkLcomKList.GetSize();
		INT_PTR nChkStrnLcomNum = aStrnLcomKList.GetSize();
		INT_PTR nChkFatiLcomNum = aFatigueLcomKList.GetSize();
		INT_PTR nChkServLcomNum = aServLcomKeyList.GetSize();

		// 요거 확인 해야 함... 우선 선언만...
		_DGN_FORC_CRC ForD_Shr; ForD_Shr.Initialize();
		_DGN_FORC_CRC ForD_BST; ForD_BST.Initialize();

		// Set Before Composite Force
		STL_COMP_RES_BEFORE_BASE_SNiP MembBeforeBaseD[2];
		STL_COMP_RES_AFTER_BASE_SNiP MembAfterBaseD[2];

		// Get Design Length Info.
		T_EDLK_D DgnLengD;
		m_mDgnLengD.Lookup(ChkElemK, DgnLengD);

		for (int nIJ = 0; nIJ < 2; ++nIJ)   // Repeat I-End & J-End
		{
			Convert_GlobalDgnParamD(MembLcomD.GlobalDgnD);
			INT_PTR nChkLcomSize = 1;
			MembLcomD.CheckLcomD.aUlmf.SetSize(nChkLcomSize);
			MembLcomD.CheckLcomD.GlobalForceD.SetSize(nChkLcomSize);
			if (!GetDgnBaseD4Elem_IJ(ChkElemK, ChkElemD, nIJ, MembLcomD)) continue;

			T_EDLK_D DgnLengD;
			m_mDgnLengD.Lookup(ChkElemK, DgnLengD);
			Convert_Stl_CompDgnLengthData(DgnLengD, MembBeforeBaseD[0]);
			Convert_Stl_CompDgnLengthData(DgnLengD, MembBeforeBaseD[1]);
			Convert_Stl_CompDgnLengthData(DgnLengD, MembAfterBaseD[0]);
			Convert_Stl_CompDgnLengthData(DgnLengD, MembAfterBaseD[1]);

			if (!m_pStlCompManager->Make_StlCompBeforeBaseData(MembLcomD, MembBeforeBaseD[nIJ])) continue;
			MembAfterBaseD[nIJ].BeforeBaseD = MembBeforeBaseD[nIJ];
			if (!m_pStlCompManager->Make_StlCompAfterBaseData(MembLcomD, MembAfterBaseD[nIJ])) continue;
		}

		// if NA is in Slab, skip check this section(or element)
		if (!IsNAinSteelGirder(MembLcomD.Memb[0].SectD) || !IsNAinSteelGirder(MembLcomD.Memb[1].SectD)) continue;

		//////////////////////////////////////////////////////////////////////////
		//  After Composite Kappa Factor Check
		//////////////////////////////////////////////////////////////////////////
		STL_COMP_MEMB_RESD_SNiP_KAPPA KappaChkResD[2];
		if (bChkKappaF) // Kappa Check의 경우, 검토해야 하는 하중조합을 모두 한 번에 넘겨서 계산함! - 모든 하중조합에 대하여 검토해야 하므로..
		{
			T_PLASTIC_LIM_CR PlastiLimCr;
			T_PLASTIC_LIM_BASE PlasticLimBase;
			double dMaxRat[2] = { 0.0, };
			int nPlasticLimCt[2] = { 0,0 };
			STLCOMPCHKFOR_ARRAY aChkForce[2];
			aChkForce[0].SetSize(nChkKappaChkLcomNum);
			aChkForce[1].SetSize(nChkKappaChkLcomNum);

			MembLcomD.CheckLcomD.GlobalForceD.SetSize(nChkKappaChkLcomNum);
			for (INT_PTR nLcom = 0; nLcom < nChkKappaChkLcomNum; ++nLcom)  // After Composite Check
			{
				_DGN_FORC_CRC ForceNC;
				int nDgnLcomNo = aKapaChkLcomKList[nLcom];
				if (!m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)) { ASSERT(0); continue; }
				int nKappaDgnLcomKIndex = aKapaChkLcomKList[nLcom] - 1;
				if (bIsPLCB) { ForceNC = aPreForceLcom[nKappaDgnLcomKIndex]; }
				else
				{
					m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(FALSE, StctD, ChkEPairK, nGdLastStagK, nLastStepNo, nDgnLcomNo, nForceBeforeType, ForceNC);
				}
				_DGN_FORC_CRC& DgnForD = aForceLcom[nKappaDgnLcomKIndex];
				_DGN_FORC_CRC& ForD_After_DL = aForceLTerm[nKappaDgnLcomKIndex];
				_DGN_FORC_CRC& ForD_Crp = aForceCreep[nKappaDgnLcomKIndex];
				_DGN_FORC_CRC& ForD_Shr = aForceShrinkage[nKappaDgnLcomKIndex];
				_DGN_SCFP_PART_LCASE& ScfpCr = aScfpCr[nKappaDgnLcomKIndex];
				_DGN_SCFP_PART_LCASE& ScfpSh = aScfpSh[nKappaDgnLcomKIndex];
				_DGN_SCFP_PART_LCASE& ScfpTe = aScfpTe[nKappaDgnLcomKIndex];

				IsExistScfp4Lcom(D_LCOMTYPE_STLCOMP, nOrgLcomNo, bExistCrp, bExistShr, bExistBST);
				if (!bExistCrp) ScfpCr.Initialize();
				if (!bExistShr) ScfpSh.Initialize();
				if (!bExistBST) ScfpTe.Initialize();

				T_SCGD_DGN_FORCE StlCompForce;
				PlasticLimBase.nDgnLcomK = nDgnLcomNo;
				PlasticLimBase.nLcomK = nOrgLcomNo;
				PlasticLimBase.nMaxMin = nMaxMinType;

				for (int nIJ = 0; nIJ < 2; ++nIJ)
				{
					ConvertStlCompForce4SaveD(nIJ, ForceNC, DgnForD, ForD_After_DL, ForD_Crp, ForD_Shr, ForD_BST, ScfpCr, ScfpSh, ScfpTe, StlCompForce);
					ConvertStlCompCalcForce(nIJ, ForceNC, DgnForD, ForD_After_DL, ForD_Crp, ForD_Shr, ForD_BST, ScfpCr, ScfpSh, ScfpTe, aChkForce[nIJ][nLcom]);
				}

				m_pStlCompDesign->WriteStlCompLcomForce(ChkElemK, nDgnLcomNo, StlCompForce); // Kappa Check 시, 모든 하중조합을 검토하므로, 포스도 여기서만 일괄적으로 써 줌, PCS
			}

			for (int nIJ = 0; nIJ < 2; ++nIJ)
			{
				MembLcomD.CheckLcomD.GlobalForceD.Copy(aChkForce[nIJ]);
				if (!m_pStlCompManager->Check_StlCompKappaCheck(MembLcomD, MembAfterBaseD[nIJ], KappaChkResD[nIJ])) continue;
				Get_AFCr_PlasticLimitRatio(nPlasticLimCt[nIJ], PlasticLimBase, KappaChkResD[nIJ].dMaxRatio, dMaxRat[nIJ], PlastiLimCr.PlasticLimStrainCr[nIJ]);
				nPlasticLimCt[nIJ]++;
			}

			m_pStlCompDesign->WriteStlCompPlasticLimitStrainCrResult(ChkElemK, PlastiLimCr);
		}

		//////////////////////////////////////////////////////////////////////////
		//  After Composite Strength / Stability Check
		//////////////////////////////////////////////////////////////////////////
		// define variables to find Critical result
		// for strength
		int nStrn[2] = { 0, 0 }; // I & J
		double dFlgSNConcreteMaxRat[2] = { 0.0 };
		double dFlgSNRebarMaxRat[2] = { 0.0 };
		double dFlgSNRibMaxRat[4] = { 0.0 }; // 0 : I -Top . 1 : I -Bottom, 2 :  J - Top,  3: J-Bottom 
		double dFlgSNFlgMaxRat[4] = { 0.0 }; // 0 : I -Top . 1 : I -Bottom, 2 :  J - Top,  3: J-Bottom 
		double dFlgSNFBeamMaxRat[4] = { 0.0 }; // 0 : I -Top . 1 : I -Bottom, 2 :  J - Top,  3: J-Bottom 

		double dWebSNRibMaxRatL[2] = { 0.0 };
		double dWebSNDeckMaxRatL[2] = { 0.0 };
		double dWebSNDeckNoMMaxRatL[2] = { 0.0 };
		double dWebSNDeckVonMaxRatL[2] = { 0.0 };
		double dWebSNDeckCombMaxRatL[2] = { 0.0 };
		double dWebSNDeckMeanMaxRatL[2] = { 0.0 };

		double dWebSNRibMaxRatR[2] = { 0.0 };
		double dWebSNDeckMaxRatR[2] = { 0.0 };
		double dWebSNDeckNoMMaxRatR[2] = { 0.0 };
		double dWebSNDeckVonMaxRatR[2] = { 0.0 };
		double dWebSNDeckCombMaxRatR[2] = { 0.0 };
		double dWebSNDeckMeanMaxRatR[2] = { 0.0 };

		T_ACST_CONC_CR ConcStrnCr;
		T_ACST_REBAR_CR RbarStrnCr;
		T_ACST_SGFR_CR FlgStrnCr;
		T_SDSN_WEB_CR WebStrnCr;
		T_SDSB_FLG_CR FlgStabCr;
		T_SDSB_WEB_CR WebStabCr;

		// for stability
		int nStab[2] = { 0, 0 }; // I & J
		double dFlgStabRibMaxRat[4] = { 0.0 };
		double dFlgStabDeckGMaxRat[4] = { 0.0 };
		double dFlgStabDeckLMaxRat[4] = { 0.0 };
		double dFlgStabDeckMaxRat[4] = { 0.0 };
		double dWebStabRibMaxRatL[2] = { 0.0 };
		double dWebStabDeckMaxRatL[2] = { 0.0 };
		double dWebStabRibMaxRatR[2] = { 0.0 };
		double dWebStabDeckMaxRatR[2] = { 0.0 };

		double dWBStabRibRatI[2] = { 0.0 }; // L, R
		double dWBStabDeckRatI[2] = { 0.0 }; // L, R

		double dWBStabRibRatJ[2] = { 0.0 }; // L, R
		double dWBStabDeckRatJ[2] = { 0.0 }; // L, R

		T_SDSB_FLG_CR FlgStabCrR;
		T_SDSB_WEB_CR WebStabCrR;
		T_SDSB_GD_CR  GDStabCrR;

		if (bChkStr || bChkStb)
		{
			MembLcomD.CheckLcomD.GlobalForceD.SetSize(1); // Strength / Stability Check의 경우, 하나의 하중조합 단위로 검토함
			for (INT_PTR nLcom = 0; nLcom < nChkStrnLcomNum; ++nLcom)
			{
				_DGN_FORC_CRC ForceNC;
				int nDgnLcomNo = aStrnLcomKList[nLcom];
				if (!m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)) { ASSERT(0); continue; }
				int nDgnStrnLcomIndex = aStrnLcomKList[nLcom] - 1;
				if (bIsPLCB) { ForceNC = aPreForceLcom[nDgnStrnLcomIndex]; }
				else
				{
					m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(FALSE, StctD, ChkEPairK, nGdLastStagK, nLastStepNo, nDgnLcomNo, nForceBeforeType, ForceNC);
				}
				_DGN_FORC_CRC& DgnForD = aForceLcom[nDgnStrnLcomIndex];
				_DGN_FORC_CRC& ForD_After_DL = aForceLTerm[nDgnStrnLcomIndex];
				_DGN_FORC_CRC& ForD_Crp = aForceCreep[nDgnStrnLcomIndex];
				_DGN_FORC_CRC& ForD_Shr = aForceShrinkage[nDgnStrnLcomIndex];
				_DGN_SCFP_PART_LCASE& ScfpCr = aScfpCr[nDgnStrnLcomIndex];
				_DGN_SCFP_PART_LCASE& ScfpSh = aScfpSh[nDgnStrnLcomIndex];
				_DGN_SCFP_PART_LCASE& ScfpTe = aScfpTe[nDgnStrnLcomIndex];

				IsExistScfp4Lcom(D_LCOMTYPE_STLCOMP, nOrgLcomNo, bExistCrp, bExistShr, bExistBST);
				if (!bExistCrp) ScfpCr.Initialize();
				if (!bExistShr) ScfpSh.Initialize();
				if (!bExistBST) ScfpTe.Initialize();

				T_SDCR_BASE LcomKeyD;
				T_ACST_CONC_BASE ConcLcom;
				T_ACST_REBAR_BASE RbarLcom;
				T_ACST_SGFR_BASE FlgLcomKeyD;

				LcomKeyD.nDgnLcomK = nDgnLcomNo;
				LcomKeyD.nLcomK = nOrgLcomNo;
				LcomKeyD.nConcurrent = nMaxMinType;

				ConcLcom.nDgnLcomK = nDgnLcomNo;
				ConcLcom.nLcomK = nOrgLcomNo;
				ConcLcom.nConcurrent = nMaxMinType;

				RbarLcom.nDgnLcomK = nDgnLcomNo;
				RbarLcom.nLcomK = nOrgLcomNo;
				RbarLcom.nConcurrent = nMaxMinType;

				FlgLcomKeyD.nDgnLcomK = nDgnLcomNo;
				FlgLcomKeyD.nLcomK = nOrgLcomNo;
				FlgLcomKeyD.nConcurrent = nMaxMinType;

				T_SCGD_DGN_FORCE StlCompForce;
				T_ACST_CONC_CASE StrnConc;
				T_ACST_REBAR_CASE StrnRebar;
				T_ACST_SGFR_CASE StrnFlgT;
				T_ACST_SGFR_CASE StrnFlgB;
				T_SDSN_WEB_CASE StrnWebD;

				T_SDSB_FLG_CASE StabFlgB;
				T_SDSB_WEB_CASE StabWebD;

				for (int nIJ = 0; nIJ < 2; ++nIJ) // Repeat I & J
				{
					MembLcomD.nIJ = nIJ;
					ConvertStlCompCalcForce(nIJ, ForceNC, DgnForD, ForD_After_DL, ForD_Crp, ForD_Shr, ForD_BST, ScfpCr, ScfpSh, ScfpTe, MembLcomD.CheckLcomD.GlobalForceD[0]);
					Get_LocalForceD(ChkElemK, nIJ, FlgLcomKeyD.nLcomK, MembLcomD.CheckLcomD.GlobalForceD[0].LocalForceAfterD, TRUE);

					int nTop = 2 * nIJ;
					int nBot = 2 * nIJ + 1;

					if (bChkStr) // After Composite Strength Check
					{
						STL_COMP_MEMB_RESD_SNiP_AFTER_STR AfterStrResD;
						if (!m_pStlCompManager->Check_StlCompAfterStrength(MembLcomD, MembAfterBaseD[nIJ], KappaChkResD[nIJ], AfterStrResD)) continue;

						Convert_StlComp_AF_Str_Concrete(AfterStrResD, StrnConc.StrnConc[nIJ], m_ChkItemD.bRSCCheckAfter[0]);
						Convert_StlComp_AF_Str_Rebar(AfterStrResD, StrnRebar.StrnRebar[nIJ], m_ChkItemD.bRSCCheckAfter[1]);
						Convert_StlComp_AF_Str_Flg(nStype, TRUE, AfterStrResD, StrnFlgT.StrnFlgIJ[nIJ], m_ChkItemD.bRSCCheckAfter[2]);
						Convert_StlComp_AF_Str_Flg(nStype, FALSE, AfterStrResD, StrnFlgB.StrnFlgIJ[nIJ], m_ChkItemD.bRSCCheckAfter[3]);
						Convert_StlComp_AF_Str_Web(nStype, AfterStrResD, StrnWebD.StrnWeb[nIJ], m_ChkItemD.bRSCCheckAfter[4]);

						// Find Flange
						Get_AFCrStrnConcreteRatio(nStrn[nIJ], ConcLcom, StrnConc.StrnConc[nIJ].dRatio, dFlgSNConcreteMaxRat[nIJ], ConcStrnCr.StrnConcCR[nIJ]);
						Get_AFCrStrnRebarRatio(nStrn[nIJ], RbarLcom, StrnRebar.StrnRebar[nIJ].dRatio, dFlgSNRebarMaxRat[nIJ], RbarStrnCr.StrnRebarCR[nIJ]);
						Get_AFCrStrnStlGirderRatio(nStrn[nIJ], FlgLcomKeyD, StrnFlgT.StrnFlgIJ[nIJ].dCrRatio_Flg, StrnFlgB.StrnFlgIJ[nIJ].dCrRatio_Flg,
							dFlgSNFlgMaxRat[nTop], dFlgSNFlgMaxRat[nBot], FlgStrnCr.StrnFlgCR[nTop], FlgStrnCr.StrnFlgCR[nBot]);
						GetCrFlgStrnRibRatio(nStrn[nIJ], LcomKeyD, StrnFlgT.StrnFlgIJ[nIJ].dCrRatio_Rib, StrnFlgB.StrnFlgIJ[nIJ].dCrRatio_Rib,
							dFlgSNRibMaxRat[nTop], dFlgSNRibMaxRat[nBot], FlgStrnCr.RibCr[nTop], FlgStrnCr.RibCr[nBot]);

						// Find Web
						int nLR = 0;
						GetCrWebStrnDeckRatio(nStrn[nIJ], LcomKeyD, StrnWebD.StrnWeb[nIJ].WebD[nLR].dRat_Deck, dWebSNDeckMaxRatL[nIJ], WebStrnCr.DeckCrL[nIJ]); // Web Deck
						GetCrWebStrnDeckRatio(nStrn[nIJ], LcomKeyD, StrnWebD.StrnWeb[nIJ].WebD[nLR].dRat_DeckNoM, dWebSNDeckNoMMaxRatL[nIJ], WebStrnCr.DeckNoMCrL[nIJ]); // NoM
						GetCrWebStrnDeckRatio(nStrn[nIJ], LcomKeyD, StrnWebD.StrnWeb[nIJ].WebD[nLR].dRat_DeckVon, dWebSNDeckVonMaxRatL[nIJ], WebStrnCr.DeckVonCrL[nIJ]); // Von-Mises
						GetCrWebStrnDeckRatio(nStrn[nIJ], LcomKeyD, StrnWebD.StrnWeb[nIJ].WebD[nLR].dRat_DeckCom, dWebSNDeckCombMaxRatL[nIJ], WebStrnCr.DeckCombCrL[nIJ]); // Combine
						GetCrWebStrnDeckRatio(nStrn[nIJ], LcomKeyD, StrnWebD.StrnWeb[nIJ].WebD[nLR].dRat_Mean, dWebSNDeckMeanMaxRatL[nIJ], WebStrnCr.DeckMeanCrL[nIJ]); // Mean
						GetCrWebStrnRibRatio(nStrn[nIJ], LcomKeyD, StrnWebD.StrnWeb[nIJ].WebD[nLR].dRat_Rib, dWebSNRibMaxRatL[nIJ], WebStrnCr.RibCrL[nIJ]); // Web Rib

						nLR = 1;
						GetCrWebStrnDeckRatio(nStrn[nIJ], LcomKeyD, StrnWebD.StrnWeb[nIJ].WebD[nLR].dRat_Deck, dWebSNDeckMaxRatR[nIJ], WebStrnCr.DeckCrR[nIJ]); // Web Deck
						GetCrWebStrnDeckRatio(nStrn[nIJ], LcomKeyD, StrnWebD.StrnWeb[nIJ].WebD[nLR].dRat_DeckNoM, dWebSNDeckNoMMaxRatR[nIJ], WebStrnCr.DeckNoMCrR[nIJ]); // NoM
						GetCrWebStrnDeckRatio(nStrn[nIJ], LcomKeyD, StrnWebD.StrnWeb[nIJ].WebD[nLR].dRat_DeckVon, dWebSNDeckVonMaxRatR[nIJ], WebStrnCr.DeckVonCrR[nIJ]); // Von-Mises
						GetCrWebStrnDeckRatio(nStrn[nIJ], LcomKeyD, StrnWebD.StrnWeb[nIJ].WebD[nLR].dRat_DeckCom, dWebSNDeckCombMaxRatR[nIJ], WebStrnCr.DeckCombCrR[nIJ]); // Combine
						GetCrWebStrnDeckRatio(nStrn[nIJ], LcomKeyD, StrnWebD.StrnWeb[nIJ].WebD[nLR].dRat_Mean, dWebSNDeckMeanMaxRatR[nIJ], WebStrnCr.DeckMeanCrR[nIJ]); // Mean
						GetCrWebStrnRibRatio(nStrn[nIJ], LcomKeyD, StrnWebD.StrnWeb[nIJ].WebD[nLR].dRat_Rib, dWebSNRibMaxRatR[nIJ], WebStrnCr.RibCrR[nIJ]); // Web Rib

						nStrn[nIJ]++;
					}

					if (bChkStb) // After Composite Stability Check
					{
						STL_COMP_MEMB_RESD_SNiP_BEFORE_STB AfterStaResD;
						if (!m_pStlCompManager->Check_StlCompAfterStability(MembLcomD, MembAfterBaseD[nIJ], KappaChkResD[nIJ], AfterStaResD)) continue;
						Convert_StlComp_BF_Sta_Flg(nStype, FALSE, AfterStaResD, StabFlgB.StabFlg[nIJ], m_ChkItemD.bRSCCheckAfter[5]);
						Convert_StlComp_BF_Sta_Web(nStype, AfterStaResD, StabWebD.StabWeb[nIJ], m_ChkItemD.bRSCCheckAfter[6]);

						// - Rib
						GetCrFlgStabRibRatio(nStab[nIJ], LcomKeyD, 0.0, AfterStaResD.dRat_Bot_Rib, dFlgStabRibMaxRat[nTop], dFlgStabRibMaxRat[nBot], FlgStabCrR.RibCr[nTop], FlgStabCrR.RibCr[nBot]);
						// - Deck : Global, Local, max
						GetCrFlgStabDeckRatio(nStab[nIJ], LcomKeyD, 0.0, AfterStaResD.dRat_Bot_GDeck, dFlgStabDeckGMaxRat[nTop], dFlgStabDeckGMaxRat[nBot], FlgStabCrR.DeckGCr[nTop], FlgStabCrR.DeckGCr[nBot]);
						GetCrFlgStabDeckRatio(nStab[nIJ], LcomKeyD, 0.0, AfterStaResD.dRat_Bot_LDeck, dFlgStabDeckLMaxRat[nTop], dFlgStabDeckLMaxRat[nBot], FlgStabCrR.DeckLCr[nTop], FlgStabCrR.DeckLCr[nBot]);
						GetCrFlgStabDeckRatio(nStab[nIJ], LcomKeyD, 0.0, AfterStaResD.dRat_Bot, dFlgStabDeckMaxRat[nTop], dFlgStabDeckMaxRat[nBot], FlgStabCrR.DeckCr[nTop], FlgStabCrR.DeckCr[nBot]);


						if (nIJ == 0)
						{
							dWBStabRibRatI[0] = AfterStaResD.aResWebStaD[0].dRat_Cr_Rib;
							dWBStabDeckRatI[0] = AfterStaResD.aResWebStaD[0].dRat_Cr_Plate;
							dWBStabRibRatI[1] = AfterStaResD.aResWebStaD[1].dRat_Cr_Rib;
							dWBStabDeckRatI[1] = AfterStaResD.aResWebStaD[1].dRat_Cr_Plate;
						}
						else if (nIJ == 1)
						{
							dWBStabRibRatJ[0] = AfterStaResD.aResWebStaD[0].dRat_Cr_Rib;
							dWBStabDeckRatJ[0] = AfterStaResD.aResWebStaD[0].dRat_Cr_Plate;
							dWBStabRibRatJ[1] = AfterStaResD.aResWebStaD[1].dRat_Cr_Rib;
							dWBStabDeckRatJ[1] = AfterStaResD.aResWebStaD[1].dRat_Cr_Plate;
						}

						// - Web-left
						GetCrWebStabRibRatio(nStab[nIJ], LcomKeyD, dWBStabRibRatI[0], dWebStabRibMaxRatL[0], WebStabCrR.RibCrL[0]);
						GetCrWebStabDeckRatio(nStab[nIJ], LcomKeyD, dWBStabDeckRatI[0], dWebStabDeckMaxRatL[0], WebStabCrR.DeckCrL[0]);
						// - Web-right
						GetCrWebStabRibRatio(nStab[nIJ], LcomKeyD, dWBStabRibRatI[1], dWebStabRibMaxRatR[0], WebStabCrR.RibCrR[0]);
						GetCrWebStabDeckRatio(nStab[nIJ], LcomKeyD, dWBStabDeckRatI[1], dWebStabDeckMaxRatR[0], WebStabCrR.DeckCrR[0]);

						// - Web-left
						GetCrWebStabRibRatio(nStab[nIJ], LcomKeyD, dWBStabRibRatJ[0], dWebStabRibMaxRatL[1], WebStabCrR.RibCrL[1]);
						GetCrWebStabDeckRatio(nStab[nIJ], LcomKeyD, dWBStabDeckRatJ[0], dWebStabDeckMaxRatL[1], WebStabCrR.DeckCrL[1]);
						// - Web-right
						GetCrWebStabRibRatio(nStab[nIJ], LcomKeyD, dWBStabRibRatJ[1], dWebStabRibMaxRatR[1], WebStabCrR.RibCrR[1]);
						GetCrWebStabDeckRatio(nStab[nIJ], LcomKeyD, dWBStabDeckRatJ[1], dWebStabDeckMaxRatR[1], WebStabCrR.DeckCrR[1]);

						nStab[nIJ]++;
					}
				}

				// 각 하중조합에 대한 결과 저장.            
				if (bChkStr) // After Composite Strength Check
				{
					m_pStlCompDesign->WriteStlCompConcreteStrnAfterResult(ChkElemK, nDgnLcomNo, StrnConc);
					m_pStlCompDesign->WriteStlCompRebarStrnAfterResult(ChkElemK, nDgnLcomNo, StrnRebar);
					m_pStlCompDesign->WriteStlCompTopFlgStrnAfterResult(ChkElemK, nDgnLcomNo, StrnFlgT);
					m_pStlCompDesign->WriteStlCompBotFlgStrnAfterResult(ChkElemK, nDgnLcomNo, StrnFlgB);
					m_pStlCompDesign->WriteStlCompWebStrnAfterResult(ChkElemK, nDgnLcomNo, StrnWebD);
				}

				if (bChkStb) // After Composite Stability Check
				{
					m_pStlCompDesign->WriteStlCompBotFlgStabAfterResult(ChkElemK, nDgnLcomNo, StabFlgB);
					m_pStlCompDesign->WriteStlCompWebStabAfterResult(ChkElemK, nDgnLcomNo, StabWebD);
				}
			}

			// Write Critical Result for Strength and Stability
			m_pStlCompDesign->WriteStlCompStrnConcreteAfterResult(ChkElemK, ConcStrnCr);
			m_pStlCompDesign->WriteStlCompStrnRebarAfterResult(ChkElemK, RbarStrnCr);
			m_pStlCompDesign->WriteStlCompStrnFlgCrAfterResult(ChkElemK, FlgStrnCr);
			m_pStlCompDesign->WriteStlCompStrnWebCrAfterResult(ChkElemK, WebStrnCr);
			m_pStlCompDesign->WriteStlCompStabFlgCrAfterResult(ChkElemK, FlgStabCrR);
			m_pStlCompDesign->WriteStlCompStabWebCrAfterResult(ChkElemK, WebStabCrR);
		}


		//////////////////////////////////////////////////////////////////////////
		//  After Composite Fatigue Check
		//////////////////////////////////////////////////////////////////////////
		// Fatigue  Check의 경우, 하중조합 그룹단위로 검토함 (Concurrunt Force의 경우, 이를 한 묶음으로 검토)
		if (bChkFati) // After Composite Fatigue Check
		{
			INT_PTR nFatiGNum = aFatigueOrgLcomKList.GetSize();
			int nFati[2] = { 0 ,0 };
			T_FATI_CONC_CR FatiConcCr;
			T_FATI_REBAR_CR FatiRbarCr;
			T_FATI_STL_GIRDER_CR FatiSltGCr;
			double dFati_Conc_RatioMax[2] = { 0.0, };
			double dFati_Rbar_RatioMax[2] = { 0.0, };
			double dFati_StlG_RatioMax[2] = { 0.0, };

			for (INT_PTR nFatiG = 0; nFatiG < nFatiGNum; ++nFatiG)
			{
				T_FATI_CONC_CASE Fati_Conc;
				T_FATI_REBAR_CASE Fati_Rebar;
				T_FATI_STL_GIRDER_CASE Fati_StlGirder;
				int nOrgLcomK = aFatigueOrgLcomKList[nFatiG];
				const T_FATI_LCOM_K_LIST& FatiGUnit = aFatigueLcomKList[nFatiG];

				INT_PTR nFatiNum = FatiGUnit.aFatiList.GetSize();
				MembLcomD.CheckLcomD.GlobalForceD.SetSize(nFatiNum);

				T_FATI_CONC_BASE ConcLcomD;
				T_FATI_REBAR_BASE RbarLcomD;
				T_FATI_STL_GIRDER_BASE StlGirderLcomD;
				ConcLcomD.nLcomK = nOrgLcomK;
				RbarLcomD.nLcomK = nOrgLcomK;
				StlGirderLcomD.nLcomK = nOrgLcomK;
				ConcLcomD.nLcomSize = (int)nFatiNum;
				RbarLcomD.nLcomSize = (int)nFatiNum;
				StlGirderLcomD.nLcomSize = (int)nFatiNum;

				for (int nIJ = 0; nIJ < 2; ++nIJ) // Repeat Check I & J
				{
					MembLcomD.nIJ = nIJ;
					for (INT_PTR nLcom = 0; nLcom < nFatiNum; ++nLcom)
					{
						_DGN_FORC_CRC ForceNC;
						int nDgnLcomNo = FatiGUnit.aFatiList[nLcom];
						int nDgnFatiLcomIndex = FatiGUnit.aFatiList[nLcom] - 1;
						if (bIsPLCB) { ForceNC = aPreForceLcom[nDgnFatiLcomIndex]; }
						else
						{
							m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(FALSE, StctD, ChkEPairK, nGdLastStagK, nLastStepNo, nDgnLcomNo, nForceBeforeType, ForceNC);
						}
						_DGN_FORC_CRC& DgnForD = aForceLcom[nDgnFatiLcomIndex];
						_DGN_FORC_CRC& ForD_After_DL = aForceLTerm[nDgnFatiLcomIndex];
						_DGN_FORC_CRC& ForD_Crp = aForceCreep[nDgnFatiLcomIndex];
						_DGN_FORC_CRC& ForD_Shr = aForceShrinkage[nDgnFatiLcomIndex];
						_DGN_SCFP_PART_LCASE& ScfpCr = aScfpCr[nDgnFatiLcomIndex];
						_DGN_SCFP_PART_LCASE& ScfpSh = aScfpSh[nDgnFatiLcomIndex];
						_DGN_SCFP_PART_LCASE& ScfpTe = aScfpTe[nDgnFatiLcomIndex];

						if (nLcom == 0)
						{
							ConcLcomD.nDgnLcomKFirst = nDgnFatiLcomIndex + 1;
							RbarLcomD.nDgnLcomKFirst = nDgnFatiLcomIndex + 1;
							StlGirderLcomD.nDgnLcomKFirst = nDgnFatiLcomIndex + 1;
						}

						ConvertStlCompCalcForce(nIJ, ForceNC, DgnForD, ForD_After_DL, ForD_Crp, ForD_Shr, ForD_BST, ScfpCr, ScfpSh, ScfpTe, MembLcomD.CheckLcomD.GlobalForceD[nLcom]);
					}

					STL_COMP_MEMB_RESD_SNiP_AFTER_FATI FatiResD;
					if (!m_pStlCompManager->Check_StlCompFatigue(MembLcomD, MembAfterBaseD[nIJ], KappaChkResD[nIJ], FatiResD)) continue;

					Convert_StlComp_Fati_Conc(FatiResD, Fati_Conc.FatiConc[nIJ], m_ChkItemD.bRSCCheckAfter[7]);
					Convert_StlComp_Fati_Rebar(FatiResD, Fati_Rebar.FatiRebar[nIJ], m_ChkItemD.bRSCCheckAfter[7]);
					Convert_StlComp_Fati_StlGirder(FatiResD, Fati_StlGirder.FatiStlGirder[nIJ], m_ChkItemD.bRSCCheckAfter[7]);

					Get_AFCr_Fati_ConcreteRatio(nFati[nIJ], ConcLcomD, Fati_Conc.FatiConc[nIJ].dRatio, dFati_Conc_RatioMax[nIJ], FatiConcCr.FatiConc[nIJ]);
					Get_AFCr_Fati_RebarRatio(nFati[nIJ], RbarLcomD, Fati_Rebar.FatiRebar[nIJ].dRatio, dFati_Rbar_RatioMax[nIJ], FatiRbarCr.FatiRebarCR[nIJ]);
					Get_AFCr_Fati_StlGirderRatio(nFati[nIJ], StlGirderLcomD, Fati_StlGirder.FatiStlGirder[nIJ].dRatio, dFati_StlG_RatioMax[nIJ], FatiSltGCr.FatiStlGirderCR[nIJ]);

					nFati[nIJ]++;
				}

				// Write Result
				m_pStlCompDesign->WriteStlCompFatiConcAfterResult(ChkElemK, nOrgLcomK, Fati_Conc);
				m_pStlCompDesign->WriteStlCompFatiRebarAfterResult(ChkElemK, nOrgLcomK, Fati_Rebar);
				m_pStlCompDesign->WriteStlCompFatiSteelGirderAfterResult(ChkElemK, nOrgLcomK, Fati_StlGirder);
			}

			// Write Critical Fatigue Value
			m_pStlCompDesign->WriteStlCompFatiConcCrAfterResult(ChkElemK, FatiConcCr);
			m_pStlCompDesign->WriteStlCompFatiRebarCrAfterResult(ChkElemK, FatiRbarCr);
			m_pStlCompDesign->WriteStlCompFatiStlGirderCrAfterResult(ChkElemK, FatiSltGCr);
		}

		//////////////////////////////////////////////////////////////////////////
		//  After Composite Serviceability Check
		//////////////////////////////////////////////////////////////////////////
		int Serv[2] = { 0 , 0 };
		if (bChkServ)
		{
			double dServMax[2] = { 0.0 , 0.0 };
			MembLcomD.CheckLcomD.GlobalForceD.SetSize(1);
			T_CRACK_CONC_BASE LcomKeyD;
			T_CRACK_CONC_CR ServCrD;
			STL_COMP_MEMB_RESD_SNiP_AFTER_SERV ServResD;
			for (INT_PTR nLcom = 0; nLcom < nChkServLcomNum; ++nLcom)  // After Composite Check
			{
				_DGN_FORC_CRC ForceNC;
				T_CRACK_CONC_CASE ConcServD;
				int iDgnLcomNo = aServLcomKeyList[nLcom];
				CString strOrgLcomNa = _T("");
				int nOrgLcomNo = 0, nSerLcomCount = 0, nMaxMinType = 0;
				if (!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)) { ASSERT(0); continue; }
				int nDgnServLcomIndex = aServLcomKeyList[nLcom] - 1;
				if (bIsPLCB) { ForceNC = aPreForceLcom[nDgnServLcomIndex]; }
				else
				{
					m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(FALSE, StctD, ChkEPairK, nGdLastStagK, nLastStepNo, iDgnLcomNo, nForceBeforeType, ForceNC);
				}

				_DGN_FORC_CRC& DgnForD = aForceLcom[nDgnServLcomIndex];
				_DGN_FORC_CRC& ForD_After_DL = aForceLTerm[nDgnServLcomIndex];
				_DGN_FORC_CRC& ForD_Crp = aForceCreep[nDgnServLcomIndex];
				_DGN_FORC_CRC& ForD_Shr = aForceShrinkage[nDgnServLcomIndex];
				_DGN_SCFP_PART_LCASE& ScfpCr = aScfpCr[nDgnServLcomIndex];
				_DGN_SCFP_PART_LCASE& ScfpSh = aScfpSh[nDgnServLcomIndex];
				_DGN_SCFP_PART_LCASE& ScfpTe = aScfpTe[nDgnServLcomIndex];

				IsExistScfp4Lcom(D_LCOMTYPE_STLCOMP, nOrgLcomNo, bExistCrp, bExistShr, bExistBST);
				if (!bExistCrp) ScfpCr.Initialize();
				if (!bExistShr) ScfpSh.Initialize();
				if (!bExistBST) ScfpTe.Initialize();

				LcomKeyD.nDgnLcomK = iDgnLcomNo;
				LcomKeyD.nLcomK = nOrgLcomNo;
				LcomKeyD.nConcurrent = nMaxMinType;

				for (int nIJ = 0; nIJ < 2; ++nIJ)
				{
					MembLcomD.nIJ = nIJ;
					ConvertStlCompCalcForce(nIJ, ForceNC, DgnForD, ForD_After_DL, ForD_Crp, ForD_Shr, ForD_BST, ScfpCr, ScfpSh, ScfpTe, MembLcomD.CheckLcomD.GlobalForceD[0]);
					if (!m_pStlCompManager->Check_StlCompServiseability(MembLcomD, MembAfterBaseD[nIJ], KappaChkResD[nIJ], ServResD)) continue;

					Convert_StlComp_Serv_Conc(ServResD, ConcServD.CrackConc[nIJ], m_ChkItemD.bRSCCheckAfter[8]);
					Get_AFCr_ServRatio(Serv[nIJ], LcomKeyD, ServResD.dCrRatio, dServMax[nIJ], ServCrD.CrackConcCR[nIJ]);
					Serv[nIJ]++;
				}
				m_pStlCompDesign->WriteStlCompCrackConcreteAfterResult(ChkElemK, iDgnLcomNo, ConcServD);
			}

			// Write Critical Serviceability Value
			m_pStlCompDesign->WriteStlCompCrackConcCrAfterResult(ChkElemK, ServCrD);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	// Check Shear  Connector Group
	//////////////////////////////////////////////////////////////////////////

	// 전단연결재 검토의 경우,  Group 단위로 설계하므로, 위와는 별도로 설계함
	// After Composite Shear Connector Strength Check
	if (bChkShearConn)
	{
		CArray<T_SCGD_K, T_SCGD_K> aScgdD;
		m_pDoc->m_pAttrCtrl2->GetScgdKeyList(aScgdD);
		INT_PTR nChkGroupNum = aScgdD.GetSize();

		int nShearConnCt = 0;

		for (INT_PTR nGroup = 0; nGroup < nChkGroupNum; ++nGroup) // Repeat Shear Connector Group
		{
			T_SCGD_K ChkScgdK = aScgdD[nGroup];
			T_SCGD_D ScgdD;
			if (!m_pDoc->m_pAttrCtrl2->GetScgd(ChkScgdK, ScgdD)) continue;

			if (!m_mStlCompShearConnectorD.Lookup(ChkScgdK, MembLcomD.ShearConnD)) continue;

			STL_COMP_AFTER_SHEAR_CONN_BASE_SNiP ShearConnBaseD;
			m_pStlCompManager->Make_StlCompShearConnBaseData(MembLcomD, ShearConnBaseD);

			CArray<int, int> aDgnLcomList;
			Make_ShearConnGroupForceD(MembLcomD.ShearConnD, aDgnLcomList);
			INT_PTR nStrnLcomNum = aDgnLcomList.GetSize();

			MembLcomD.ShearConnD.ShearConnForceD.SetSize(1);

			T_STRD_SHEAR_CONN_BASE ShearConnLcomD;
			STL_COMP_MEMB_RESD_SNiP_SHEAR_CONN ShearConnResD;
			T_STRD_SHEAR_CONN_BASE ShearConnCr;
			double dShearConnMaxRatio = 0.0;
			T_STRD_SHEAR_CONN ShearConnRWD;

			for (INT_PTR nLcom = 0; nLcom < nStrnLcomNum; ++nLcom)  // Repeat Check Load combination Number
			{
				int nDgnLcomK = aDgnLcomList[nLcom];

				if (!m_pForcCtrl->Get_LcomType(nDgnLcomK, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)) { ASSERT(0); continue; }
				ShearConnLcomD.nDgnLcomK = nDgnLcomK;
				ShearConnLcomD.nLcomK = nOrgLcomNo;
				ShearConnLcomD.nConcurrent = nMaxMinType;

				SHEAR_CONN_GROUP_FORCE_SNIP GroupForceD;
				if (!m_mShearConnGroupForceD.Lookup(nDgnLcomK, GroupForceD)) { ASSERT(0); continue; }
				MembLcomD.ShearConnD.ShearConnForceD[0] = GroupForceD;

				if (!m_pStlCompManager->Check_StlCompShearConnector(MembLcomD, ShearConnBaseD, ShearConnResD)) continue;

				ShearConnRWD.Initialize();
				Convert_ShearConnResD(ShearConnResD, ShearConnRWD, m_ChkItemD.bRSCCheckAfter[9]);
				Get_AFCr_ShearConnectorRatio(nShearConnCt, ShearConnLcomD, ShearConnRWD.dRatio, dShearConnMaxRatio, ShearConnCr); // Critical은 0 Index만 사용
				m_pStlCompDesign->WriteStlCompShearConnGroupStrnAfterResult(ChkScgdK, nDgnLcomK, ShearConnRWD);
				nShearConnCt++;
			}

			// Write Critical Check Result Data
			m_pStlCompDesign->WriteStlCompShearConnGroupCrAfterResult(ChkScgdK, ShearConnCr);
		}
	}

	m_pStlCompDesign->WriteStlCompCheckItem(m_ChkItemD);

	return bDesignOK;
}

BOOL CDgnRUSStlCompDataCtrl::Get_StagName(int nStageK, CStringW& strStageName)
{
	T_STAG_D StageD;
	BOOL bExsit = m_pDoc->m_pAttrCtrl->GetStag(nStageK, StageD);
	CString strStageNameTmp;
	strStageNameTmp = bExsit ? StageD.StageName : _T("PreComposite");

	CStringW strStageNameTmpW(strStageNameTmp);
	strStageName = strStageNameTmpW;
	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_LcomName(int nLcomK, CStringW& strLcomName)
{
	T_LCOM_D LcomD;
	BOOL bExsit = m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, nLcomK, LcomD);

	CString strLcomNameTmp = LcomD.LoadCombName;
	CStringW strLcomNameTmpW(strLcomNameTmp);
	strLcomName = strLcomNameTmpW;
	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Check_ConstructionStage(T_ELEM_K ElemK, int nLcomK4CS, STL_COMP_MEMB_POSD_SNiP &MembInD)
{
	// Base Data Setting and Check Data
	BOOL bChkIJ[2] = { FALSE, FALSE };
	STL_COMP_CHK_POSD_SNiP ChkSubPosD;

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	if (!m_mStlCompDgnPosD_I.Lookup(ElemK, ChkSubPosD)) bChkIJ[0] = FALSE;
	else { bChkIJ[0] = IsNeedBeforeChk(ChkSubPosD); }
	if (!m_mStlCompDgnPosD_J.Lookup(ElemK, ChkSubPosD)) bChkIJ[1] = FALSE;
	else { bChkIJ[1] = IsNeedBeforeChk(ChkSubPosD); }

	if (!bChkIJ[0] && !bChkIJ[1]) return FALSE;

	T_ELEM_D ElemD;
	if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD)) return FALSE;
	int nStype = m_pDoc->m_pAttrCtrl->GetRealSectType(ElemD.elpro);


	if (!m_pStlCompDesign) { ASSERT(0); return FALSE; }
	if (!m_pStlCompManager) { ASSERT(0); return FALSE; }

	STL_COMP_RES_BEFORE_BASE_SNiP MembBeforeBaseD[2];  // I & J

	// Get Design Length Info.
	T_EDLK_D DgnLengD;
	m_mDgnLengD.Lookup(ElemK, DgnLengD);
	Convert_Stl_CompDgnLengthData(DgnLengD, MembBeforeBaseD[0]);
	Convert_Stl_CompDgnLengthData(DgnLengD, MembBeforeBaseD[1]);

	for (int nIJ = 0; nIJ < 2; ++nIJ) // I & J
	{
		//        if(bChkIJ[nIJ])
		//        {
		GetDgnBaseD4Elem_IJ(ElemK, ElemD, nIJ, MembInD);
		Get_LocalForceD(ElemK, nIJ, 0, MembInD.CheckLcomD.GlobalForceD[0].LocalForceBeforeD, FALSE);
		MembInD.Memb[nIJ].DesignD.KappaFChkInfo.bAutoCalc = FALSE; // Before Composite의 경우, Kappa factor는 항상 1.0 적용,  Plastic Strain is not allowed.
		MembInD.Memb[nIJ].DesignD.KappaFChkInfo.dUDKappa_y = 1.0;
		MembInD.Memb[nIJ].DesignD.KappaFChkInfo.dUDKappa_z = 1.0;
		if (!m_pStlCompManager->Make_StlCompBeforeBaseData(MembInD, MembBeforeBaseD[nIJ])) bChkIJ[nIJ] = FALSE;
		/*       }*/
	}
	if (MembInD.CheckLcomD.aUlmf.GetSize() < 1) MembInD.CheckLcomD.aUlmf.SetSize(1);
	if (MembInD.CheckLcomD.GlobalForceD.GetSize() < 1) MembInD.CheckLcomD.GlobalForceD.SetSize(1);


	// Start Construction Stage Check
	CStageInfo *pStageInfo = m_pDoc->m_pPostCtrl->GetStageInfo();
	CArray<T_STAG_K, T_STAG_K> aStagK;
	pStageInfo->GetResultSavedStagKeyList(aStagK);
	INT_PTR nStageSize = aStagK.GetSize();
	if (nStageSize == 0)	return FALSE;

	CString strSgldName = _LS(IDS_DB_SGLD_SUM);	// SUM.
	T_SGLD_K SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);

	ArCSIFK aCsif;

	// Find Before Composite CS Step for each Element
	for (INT_PTR i = 0; i < nStageSize; ++i)
	{
		T_STAG_K StagK = aStagK.GetAt(i);
		if (!pStageInfo->IsActiveElemInTgtStag(StagK, ElemK)) continue;

		T_CSCS_D CscsD; CscsD.Initialize();
		int nCompositeType = m_pDoc->m_pAttrCtrl->GetElemSectCompositeInfo(ElemK, StagK, CscsD);
		if (nCompositeType == 0)continue;
		else if (nCompositeType == 3)  break; // 합성전까지만 돌린다.
		// nCompositeType Variable Meaning...
		// 0:오류 또는 시공 합성 단면 아님
		// 1:시공 합성 단면 : 합성 전 (Active Stage 이전)
		// 2:시공 합성 단면 : 합성 중 (Active되고 모든 Part가 합성되기 전)
		// 3:시공 합성 단면 : 합성 후 (모든 Part가 합성된 후)

		CPG_CSIF_D CsifD;
		CsifD.StagK = StagK;

		T_STAG_STEP_INFO StageInfo; StageInfo.Initialize();
		m_pDoc->m_pAttrCtrl->GetStagStepInfo(StagK, StageInfo);
		INT_PTR nStepSize = StageInfo.aInfo.GetSize();
		if (nStepSize == 0)
		{
			int nFinalStepNo = 0;
			m_pDataCtrl->Get_LastStepInCurStage(StagK, nFinalStepNo);
			CsifD.StagK = StagK;
			CsifD.bIsStep = FALSE;
			CsifD.nSerialStepK = nFinalStepNo;
			aCsif.Add(CsifD);
		}
		else
		{
			for (int j = 0; j < nStepSize; ++j)
			{
				int nStep = j + 1;
				int nSerialStep = 0;
				if (!pStageInfo->GetStepSerialKey(StagK, nStep, nSerialStep)) continue;
				CsifD.StagK = StagK;
				CsifD.bIsStep = TRUE;
				CsifD.nStepK = nStep;
				CsifD.nSerialStepK = nSerialStep;
				aCsif.Add(CsifD);
			}
		}
	}

	int nFlgStrn[2] = { 0 };
	double dFlgSNRibMaxRat[4] = { 0.0 };
	double dFlgSNDeckMaxRat[4] = { 0.0 };
	double dFlgSNFBeamMaxRat[4] = { 0.0 };

	double dWebSNRibMaxRatL[2] = { 0.0 };
	double dWebSNDeckMaxRatL[2] = { 0.0 };
	double dWebSNDeckNoMMaxRatL[2] = { 0.0 };
	double dWebSNDeckVonMaxRatL[2] = { 0.0 };

	double dWebSNRibMaxRatR[2] = { 0.0 };
	double dWebSNDeckMaxRatR[2] = { 0.0 };
	double dWebSNDeckNoMMaxRatR[2] = { 0.0 };
	double dWebSNDeckVonMaxRatR[2] = { 0.0 };

	double dFlgStabGDMaxRat[2] = { 0.0 };
	double dFlgStabRibMaxRat[4] = { 0.0 };
	double dFlgStabDeckGMaxRat[4] = { 0.0 };
	double dFlgStabDeckLMaxRat[4] = { 0.0 };
	double dFlgStabDeckMaxRat[4] = { 0.0 };
	double dFlgStabFBeamGMaxRat[4] = { 0.0 };
	double dFlgStabFBeamLMaxRat[4] = { 0.0 };
	double dFlgStabFBeamMaxRat[4] = { 0.0 };
	double dWebStabRibMaxRatL[2] = { 0.0 };
	double dWebStabDeckMaxRatL[2] = { 0.0 };
	double dWebStabRibMaxRatR[2] = { 0.0 };
	double dWebStabDeckMaxRatR[2] = { 0.0 };

	int nBFSNRib[4] = { 0 };
	double dBFSNRibMaxRat[4] = { 0.0 };

	T_SDSN_FLG_CR FlgStrnCrR;
	T_SDSN_WEB_CR WebStrnCrR;
	T_SDSB_FLG_CR FlgStabCrR;
	T_SDSB_WEB_CR WebStabCrR;
	T_SDSB_GD_CR  GDStabCrR;

	double dFactor = 1.0;
	// Repeat Stage Step
	INT_PTR nStageStepSize = aCsif.GetSize();
	for (INT_PTR i = 0; i < nStageStepSize; ++i)
	{
		T_ELEM_K nElemI = ElemK, nElemM = ElemK, nElemJ = ElemK;
		ElemPairK nEPairI(ElemK, EN_EL_BEAM), nEPairM(ElemK, EN_EL_BEAM), nEPairJ(ElemK, EN_EL_BEAM);
		int nPosI = 0;
		int nPosM = 0; // 임시, 찾아야함. 
		int nPosJ = 1;

		DgnBeamPairK nUblmK(EN_EL_BEAM, 0);
		BOOL bCantilever = FALSE;

		m_pDataCtrl->GetUbLengthElemInfo(EPairK, nUblmK, nEPairI, nEPairM, nEPairJ, nPosI, nPosM, nPosJ, bCantilever);

		const CPG_CSIF_D &CsifKD = aCsif[i];

		Get_StlCompUlmSection(nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, MembInD.Ulms);

		_DGN_FORC_CRC ForcMax;
		_DGN_FORC_CRC ForcMin;

		if (!m_pForcCtrl->Get_CompositeForceBySgld(EPairK, SgldK, CsifKD.StagK, CsifKD.nSerialStepK, dFactor, ForcMax, ForcMin))	continue;

		double dCSLoadF = 1.1; // 시공단계 검토의 경우 Load Factor는 일괄적으로 1.1 적용 (15.08.06 /이대근 이사보님 요청)
		ForcMax *= dCSLoadF; //  dFactorCS의 경우, I & J단의 의미가 없어  I-End 값을 저장

		_DGN_FORC_CRC SpanForI = ForcMax;
		_DGN_FORC_CRC ForcMinI;
		if (ElemK != nElemI)
		{
			ElemPairK ElemIPairK(nElemI, EN_EL_BEAM);
			m_pForcCtrl->Get_CompositeForceBySgld(ElemIPairK, SgldK, CsifKD.StagK, CsifKD.nSerialStepK, dFactor, SpanForI, ForcMinI);
			SpanForI *= dCSLoadF;
		}

		_DGN_FORC_CRC SpanForM = ForcMax;
		_DGN_FORC_CRC ForcMinM;
		if (ElemK != nElemM)
		{
			ElemPairK ElemMPairK(nElemM, EN_EL_BEAM);
			m_pForcCtrl->Get_CompositeForceBySgld(ElemMPairK, SgldK, CsifKD.StagK, CsifKD.nSerialStepK, dFactor, SpanForM, ForcMinM);
			SpanForM *= dCSLoadF;
		}

		_DGN_FORC_CRC SpanForJ = ForcMax;
		_DGN_FORC_CRC ForcMinJ;
		if (ElemK != nElemJ)
		{
			ElemPairK ElemJPairK(nElemJ, EN_EL_BEAM);
			m_pForcCtrl->Get_CompositeForceBySgld(ElemJPairK, SgldK, CsifKD.StagK, CsifKD.nSerialStepK, dFactor, SpanForJ, ForcMinJ);
			SpanForJ *= dCSLoadF;
		}

		// Span을 구성하는 I,M,J Force
		ConvertStlCompCalcUlmCSForce(nPosI, nPosM, nPosJ, SpanForI, SpanForM, SpanForJ, MembInD.CheckLcomD.aUlmf[0]);
		STL_COMP_FORCE_D &ComLcomF = MembInD.CheckLcomD.GlobalForceD[0];

		T_SCGD_DGN_FORCE StlCompForce;
		T_SDSN_FLG_CASE StrnFlgT;
		T_SDSN_FLG_CASE StrnFlgB;
		T_SDSN_WEB_CASE StrnWebD;
		T_SDSB_FLG_CASE StabFlgT;
		T_SDSB_FLG_CASE StabFlgB;
		T_SDSB_WEB_CASE StabWebD;
		T_SDSB_GD_CASE  StabGD;

		T_SDCR_BASE StageKeyD;
		StageKeyD.nLcomK = CsifKD.StagK; // Lcom Key 대신 Stage Key로 채움
		StageKeyD.nDgnLcomK = CsifKD.nSerialStepK; // Lcom Key 대신nSerialStepK로 채움
		StageKeyD.nConcurrent = 0;
		StageKeyD.nLcomSize = nStageStepSize;  // Lcom Size 대신 Stage Step Size로 채움
		//StageKeyD.nDgnLcomKFirst =nDgnLcomKFirst;


		double dWBStrnDeckRatI[2] = { 0.0 }; // L, R
		double dWBStrnDeckNoMRatI[2] = { 0.0 }; // L, R
		double dWBStrnDeckVonRatI[2] = { 0.0 }; // L, R
		double dWBStrnRibRatI[2] = { 0.0 }; // L, R

		double dWBStrnDeckRatJ[2] = { 0.0 }; // L, R
		double dWBStrnDeckNoMRatJ[2] = { 0.0 }; // L, R
		double dWBStrnDeckVonRatJ[2] = { 0.0 }; // L, R
		double dWBStrnRibRatJ[2] = { 0.0 }; // L, R

		double dWBStabRibRatI[2] = { 0.0 }; // L, R
		double dWBStabDeckRatI[2] = { 0.0 }; // L, R

		double dWBStabRibRatJ[2] = { 0.0 }; // L, R
		double dWBStabDeckRatJ[2] = { 0.0 }; // L, R

		for (int nIJ = 0; nIJ < 2; ++nIJ) // Repeat I-End , J-End
		{
			//if(!bChkIJ[nIJ]) continue;
			BOOL bPosI = (nIJ == 0);
			MembInD.nIJ = nIJ;
			MembInD.Memb[nIJ].ChIf.nChkPos = nIJ;
			Get_LocalForceD(ElemK, nIJ, 0, MembInD.CheckLcomD.GlobalForceD[0].LocalForceBeforeD, FALSE);
			ConvertStlCompCalcForce4CSStage(nIJ, ForcMax, MembInD.CheckLcomD.GlobalForceD[0]);
			ConvertStlCompForce(nIJ, ForcMax, StlCompForce.Elem[nIJ].ForBC);			

			double dTFStrnRibRat = 0.0;
			double dBFStrnRibRat = 0.0;
			double dTFStrnDeckRat = 0.0;
			double dBFStrnDeckRat = 0.0;

			double dTFStrnFBRat = 0.0;
			double dBFStrnFBRat = 0.0;

			//
			double dStbGDRat = 0.0;
			double dTFStabRibRat = 0.0;
			double dBFStabRibRat = 0.0;
			double dTFStabDeckGRat = 0.0;
			double dTFStabDeckLRat = 0.0;
			double dTFStabDeckRat = 0.0;
			double dBFStabDeckGRat = 0.0;
			double dBFStabDeckLRat = 0.0;
			double dBFStabDeckRat = 0.0;
			double dTFStabFBGRat = 0.0;
			double dTFStabFBLRat = 0.0;
			double dTFStabFBRat = 0.0;
			double dBFStabFBGRat = 0.0;
			double dBFStabFBLRat = 0.0;
			double dBFStabFBRat = 0.0;

			// 실제 검토 하는 부분임.. 여기서 부터,
			if (MembInD.GlobalDgnD.bDgnCheck[0]) // Before Strength Check
			{
				STL_COMP_MEMB_RESD_SNiP_BEFORE_STR BeforeStrResD;
				m_pStlCompManager->Check_StlCompBeforeStrength(MembInD, MembBeforeBaseD[nIJ], BeforeStrResD);

				Convert_StlComp_BF_Str_Flg(nStype, TRUE, BeforeStrResD, StrnFlgT.StrnFlg[nIJ], m_ChkItemD.bRSCCheckBefore[0]);
				Convert_StlComp_BF_Str_Flg(nStype, FALSE, BeforeStrResD, StrnFlgB.StrnFlg[nIJ], m_ChkItemD.bRSCCheckBefore[1]);
				Convert_StlComp_BF_Str_Web(nStype, BeforeStrResD, StrnWebD.StrnWeb[nIJ], m_ChkItemD.bRSCCheckBefore[2]);
			}

			if (MembInD.GlobalDgnD.bDgnCheck[1]) // Before Stability Check
			{
				STL_COMP_MEMB_RESD_SNiP_BEFORE_STB BeforeStaResD;
				m_pStlCompManager->Check_StlCompBeforeStability(MembInD, MembBeforeBaseD[nIJ], BeforeStaResD);

				Convert_StlComp_BF_Sta_GD(nStype, BeforeStaResD, StabGD.StabGD[nIJ], m_ChkItemD.bRSCCheckBefore[3]); // Stability - Lateral torsional Bucking
				Convert_StlComp_BF_Sta_Flg(nStype, TRUE, BeforeStaResD, StabFlgT.StabFlg[nIJ], m_ChkItemD.bRSCCheckBefore[4]); // Stability - Top Flange - Global, Local, 
				Convert_StlComp_BF_Sta_Flg(nStype, FALSE, BeforeStaResD, StabFlgB.StabFlg[nIJ], m_ChkItemD.bRSCCheckBefore[5]); // Stability - Bot Flange 
				Convert_StlComp_BF_Sta_Web(nStype, BeforeStaResD, StabWebD.StabWeb[nIJ], m_ChkItemD.bRSCCheckBefore[6]); // Stability - Web
			}

			// Critical 값 찾기
			// *********************
			// Top
			// *********************
			T_SDSN_FLG  &StrnFlgTop = StrnFlgT.StrnFlg[nIJ];
			T_SDSB_FLG  &StabFlgTop = StabFlgT.StabFlg[nIJ];
			for (int nPart = 0; nPart < 3; ++nPart) // L,M,R
			{
				// -Strength
				T_SDPT_STRN_FLG &StrnChk = StrnFlgTop.StrnChk[nPart];
				dTFStrnRibRat = max(dTFStrnRibRat, StrnChk.dRat_Rib);
				dTFStrnDeckRat = max(dTFStrnDeckRat, StrnChk.dRat_Deck);
				dTFStrnFBRat = max(dTFStrnFBRat, StrnChk.dRat_FB);

				// -Stability
				T_SDPT_STAB_FLG &StabChk = StabFlgTop.StabChk[nPart];
				dTFStabRibRat = max(dTFStabRibRat, StabChk.dRat_Rib);
				dTFStabDeckGRat = max(dTFStabDeckGRat, StabChk.dRat_DeckG);
				dTFStabDeckLRat = max(dTFStabDeckLRat, StabChk.dRat_DeckL);
				dTFStabDeckRat = max(dTFStabDeckRat, StabChk.dRat_Deck);
				dTFStabFBGRat = max(dTFStabFBGRat, StabChk.dRat_FBG);
				dTFStabFBLRat = max(dTFStabFBLRat, StabChk.dRat_FBL);
				dTFStabFBRat = max(dTFStabFBRat, StabChk.dRat_FB);
			}

			// *********************
			// Bottom
			// *********************
			T_SDSN_FLG  &StrnFlgBot = StrnFlgB.StrnFlg[nIJ];
			T_SDSB_FLG  &StabFlgBot = StabFlgB.StabFlg[nIJ];
			for (int nPart = 0; nPart < 3; ++nPart) // L,M,R
			{
				// -Strength
				const T_SDPT_STRN_FLG &StrnChk = StrnFlgBot.StrnChk[nPart];
				dBFStrnRibRat = max(dBFStrnRibRat, StrnChk.dRat_Rib);
				dBFStrnDeckRat = max(dBFStrnDeckRat, StrnChk.dRat_Deck);
				dBFStrnFBRat = max(dBFStrnFBRat, StrnChk.dRat_FB);

				// -Stability
				const T_SDPT_STAB_FLG &StabChk = StabFlgBot.StabChk[nPart];
				dBFStabRibRat = max(dBFStabRibRat, StabChk.dRat_Rib);
				dBFStabDeckGRat = max(dBFStabDeckGRat, StabChk.dRat_DeckG);
				dBFStabDeckLRat = max(dBFStabDeckLRat, StabChk.dRat_DeckL);
				dBFStabDeckRat = max(dBFStabDeckRat, StabChk.dRat_Deck);
				dBFStabFBGRat = max(dBFStabFBGRat, StabChk.dRat_FBG);
				dBFStabFBLRat = max(dBFStabFBLRat, StabChk.dRat_FBL);
				dBFStabFBRat = max(dBFStabFBRat, StabChk.dRat_FB);
			}

			// *********************
			// Web
			// *********************
			T_SDSN_WEB  &StrnWeb = StrnWebD.StrnWeb[nIJ];
			T_SDSB_WEB  &StabWeb = StabWebD.StabWeb[nIJ];

			for (int nPart = 0; nPart < 2; ++nPart) // L,R
			{
				// -Strength
				T_SDPT_STRN_WEB &WebStrnD = StrnWeb.WebD[nPart];

				// -Stability
				T_SDPT_STAB_WEB &WebStabD = StabWeb.WebD[nPart];

				if (nIJ == 0)//I
				{
					dWBStrnRibRatI[nPart] = max(dWBStrnRibRatI[nPart], WebStrnD.dRat_Rib);
					dWBStrnDeckNoMRatI[nPart] = max(dWBStrnDeckNoMRatI[nPart], WebStrnD.dRat_DeckNoM);
					dWBStrnDeckVonRatI[nPart] = max(dWBStrnDeckVonRatI[nPart], WebStrnD.dRat_DeckVon);
					dWBStrnDeckRatI[nPart] = max(dWBStrnDeckRatI[nPart], WebStrnD.dRat_Deck);

					dWBStabRibRatI[nPart] = max(dWBStabRibRatI[nPart], WebStabD.dRat_Rib);
					dWBStabDeckRatI[nPart] = max(dWBStabDeckRatI[nPart], WebStabD.dRat_Deck);
				}
				else  //J
				{
					dWBStrnRibRatJ[nPart] = max(dWBStrnRibRatJ[nPart], WebStrnD.dRat_Rib);
					dWBStrnDeckNoMRatJ[nPart] = max(dWBStrnDeckNoMRatJ[nPart], WebStrnD.dRat_DeckNoM);
					dWBStrnDeckVonRatJ[nPart] = max(dWBStrnDeckVonRatJ[nPart], WebStrnD.dRat_DeckVon);
					dWBStrnDeckRatJ[nPart] = max(dWBStrnDeckRatJ[nPart], WebStrnD.dRat_Deck);

					dWBStabRibRatJ[nPart] = max(dWBStabRibRatJ[nPart], WebStabD.dRat_Rib);
					dWBStabDeckRatJ[nPart] = max(dWBStabDeckRatJ[nPart], WebStabD.dRat_Deck);
				}
			}

			// *********************
			// Global Deck_Stability
			// *********************
			T_SDSB_GD &GDStab = StabGD.StabGD[nIJ];
			dStbGDRat = GDStab.dRat_GD;
			int nTop = 2 * nIJ;
			int nBot = 2 * nIJ + 1;

			// - Strength
			GetCrFlgStrnRibRatio(nFlgStrn[nIJ], StageKeyD, dTFStrnRibRat, dBFStrnRibRat, dFlgSNRibMaxRat[nTop], dFlgSNRibMaxRat[nBot], FlgStrnCrR.RibCr[nTop], FlgStrnCrR.RibCr[nBot]);
			GetCrFlgStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dTFStrnDeckRat, dBFStrnDeckRat, dFlgSNDeckMaxRat[nTop], dFlgSNDeckMaxRat[nBot], FlgStrnCrR.DeckCr[nTop], FlgStrnCrR.DeckCr[nBot]);
			GetCrFlgStrnFBeamRatio(nFlgStrn[nIJ], StageKeyD, dTFStrnFBRat, dBFStrnFBRat, dFlgSNFBeamMaxRat[nTop], dFlgSNFBeamMaxRat[nBot], FlgStrnCrR.FBeamCr[nTop], FlgStrnCrR.FBeamCr[nBot]);
			// - Strength : Web-Left - rib, Deck(max), Deck(Shear stress), Deck(Combined stress),
			if (nIJ == 0)
			{
				GetCrWebStrnRibRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnRibRatI[0], dWebSNRibMaxRatL[nIJ], WebStrnCrR.RibCrL[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnDeckRatI[0], dWebSNDeckMaxRatL[nIJ], WebStrnCrR.DeckCrL[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnDeckNoMRatI[0], dWebSNDeckNoMMaxRatL[nIJ], WebStrnCrR.DeckNoMCrL[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnDeckVonRatI[0], dWebSNDeckVonMaxRatL[nIJ], WebStrnCrR.DeckVonCrL[nIJ]);
				// - Strength : Web-Right
				GetCrWebStrnRibRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnRibRatI[1], dWebSNRibMaxRatR[nIJ], WebStrnCrR.RibCrR[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnDeckRatI[1], dWebSNDeckMaxRatR[nIJ], WebStrnCrR.DeckCrR[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnDeckNoMRatI[1], dWebSNDeckNoMMaxRatR[nIJ], WebStrnCrR.DeckNoMCrR[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnDeckVonRatI[1], dWebSNDeckVonMaxRatR[nIJ], WebStrnCrR.DeckVonCrR[nIJ]);
			}
			else
			{
				GetCrWebStrnRibRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnRibRatJ[0], dWebSNRibMaxRatL[nIJ], WebStrnCrR.RibCrL[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnDeckRatJ[0], dWebSNDeckMaxRatL[nIJ], WebStrnCrR.DeckCrL[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnDeckNoMRatJ[0], dWebSNDeckNoMMaxRatL[nIJ], WebStrnCrR.DeckNoMCrL[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnDeckVonRatJ[0], dWebSNDeckVonMaxRatL[nIJ], WebStrnCrR.DeckVonCrL[nIJ]);
				// - Strength : Web-Right
				GetCrWebStrnRibRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnRibRatJ[1], dWebSNRibMaxRatR[nIJ], WebStrnCrR.RibCrR[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnDeckRatJ[1], dWebSNDeckMaxRatR[nIJ], WebStrnCrR.DeckCrR[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnDeckNoMRatJ[1], dWebSNDeckNoMMaxRatR[nIJ], WebStrnCrR.DeckNoMCrR[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStrnDeckVonRatJ[1], dWebSNDeckVonMaxRatR[nIJ], WebStrnCrR.DeckVonCrR[nIJ]);
			}

			// Stability
			// - GLT
			GetCrGDStabRatio(nFlgStrn[nIJ], StageKeyD, dStbGDRat, dFlgStabGDMaxRat[nIJ], GDStabCrR.GDCr[nIJ]);
			// - Rib
			GetCrFlgStabRibRatio(nFlgStrn[nIJ], StageKeyD, dTFStabRibRat, dBFStabRibRat, dFlgStabRibMaxRat[nTop], dFlgStabRibMaxRat[nBot], FlgStabCrR.RibCr[nTop], FlgStabCrR.RibCr[nBot]);
			// - Deck : Global, Local, max
			GetCrFlgStabDeckRatio(nFlgStrn[nIJ], StageKeyD, dTFStabDeckGRat, dBFStabDeckGRat, dFlgStabDeckGMaxRat[nTop], dFlgStabDeckGMaxRat[nBot], FlgStabCrR.DeckGCr[nTop], FlgStabCrR.DeckGCr[nBot]);
			GetCrFlgStabDeckRatio(nFlgStrn[nIJ], StageKeyD, dTFStabDeckLRat, dBFStabDeckLRat, dFlgStabDeckLMaxRat[nTop], dFlgStabDeckLMaxRat[nBot], FlgStabCrR.DeckLCr[nTop], FlgStabCrR.DeckLCr[nBot]);
			GetCrFlgStabDeckRatio(nFlgStrn[nIJ], StageKeyD, dTFStabDeckRat, dBFStabDeckRat, dFlgStabDeckMaxRat[nTop], dFlgStabDeckMaxRat[nBot], FlgStabCrR.DeckCr[nTop], FlgStabCrR.DeckCr[nBot]);
			// - FB : Global, Local, max
			GetCrFlgStabFBeamRatio(nFlgStrn[nIJ], StageKeyD, dTFStabFBGRat, dBFStabFBGRat, dFlgStabFBeamGMaxRat[nTop], dFlgStabFBeamGMaxRat[nBot], FlgStabCrR.FBeamGCr[nTop], FlgStabCrR.FBeamGCr[nBot]);
			GetCrFlgStabFBeamRatio(nFlgStrn[nIJ], StageKeyD, dTFStabFBLRat, dBFStabFBLRat, dFlgStabFBeamLMaxRat[nTop], dFlgStabFBeamLMaxRat[nBot], FlgStabCrR.FBeamLCr[nTop], FlgStabCrR.FBeamLCr[nBot]);
			GetCrFlgStabFBeamRatio(nFlgStrn[nIJ], StageKeyD, dTFStabFBRat, dBFStabFBRat, dFlgStabFBeamMaxRat[nTop], dFlgStabFBeamMaxRat[nBot], FlgStabCrR.FBeamCr[nTop], FlgStabCrR.FBeamCr[nBot]);
			if (nIJ == 0)
			{
				// - Web-left
				GetCrWebStabRibRatio(nFlgStrn[nIJ], StageKeyD, dWBStabRibRatI[0], dWebStabRibMaxRatL[nIJ], WebStabCrR.RibCrL[nIJ]);
				GetCrWebStabDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStabDeckRatI[0], dWebStabDeckMaxRatL[nIJ], WebStabCrR.DeckCrL[nIJ]);
				// - Web-right
				GetCrWebStabRibRatio(nFlgStrn[nIJ], StageKeyD, dWBStabRibRatI[1], dWebStabRibMaxRatR[nIJ], WebStabCrR.RibCrR[nIJ]);
				GetCrWebStabDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStabDeckRatI[1], dWebStabDeckMaxRatR[nIJ], WebStabCrR.DeckCrR[nIJ]);
			}
			else
			{
				// - Web-left
				GetCrWebStabRibRatio(nFlgStrn[nIJ], StageKeyD, dWBStabRibRatJ[0], dWebStabRibMaxRatL[nIJ], WebStabCrR.RibCrL[nIJ]);
				GetCrWebStabDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStabDeckRatJ[0], dWebStabDeckMaxRatL[nIJ], WebStabCrR.DeckCrL[nIJ]);
				// - Web-right
				GetCrWebStabRibRatio(nFlgStrn[nIJ], StageKeyD, dWBStabRibRatJ[1], dWebStabRibMaxRatR[nIJ], WebStabCrR.RibCrR[nIJ]);
				GetCrWebStabDeckRatio(nFlgStrn[nIJ], StageKeyD, dWBStabDeckRatJ[1], dWebStabDeckMaxRatR[nIJ], WebStabCrR.DeckCrR[nIJ]);
			}

			nFlgStrn[nIJ]++;
		}

		// 각 하중조합에 대한 결과 저장.
		m_pStlCompDesign->WriteStlCompStageForce(ElemK, CsifKD.nSerialStepK, StlCompForce);
		m_pStlCompDesign->WriteStlCompTopFlgStrnBeforeResult(ElemK, CsifKD.nSerialStepK, StrnFlgT);
		m_pStlCompDesign->WriteStlCompBotFlgStrnBeforeResult(ElemK, CsifKD.nSerialStepK, StrnFlgB);
		m_pStlCompDesign->WriteStlCompWebStrnBeforeResult(ElemK, CsifKD.nSerialStepK, StrnWebD);

		m_pStlCompDesign->WriteStlCompGDStabBeforeResult(ElemK, CsifKD.nSerialStepK, StabGD);
		m_pStlCompDesign->WriteStlCompTopFlgStabBeforeResult(ElemK, CsifKD.nSerialStepK, StabFlgT);
		m_pStlCompDesign->WriteStlCompBotFlgStabBeforeResult(ElemK, CsifKD.nSerialStepK, StabFlgB);
		m_pStlCompDesign->WriteStlCompWebStabBeforeResult(ElemK, CsifKD.nSerialStepK, StabWebD);
	}

	// 요소에서 가장 불리한 하중조합의 결과 저장.
	m_pStlCompDesign->WriteStlCompStrnFlgCrBeforeResult(ElemK, FlgStrnCrR);
	m_pStlCompDesign->WriteStlCompStrnWebCrBeforeResult(ElemK, WebStrnCrR);

	m_pStlCompDesign->WriteStlCompStabGDCrBeforeResult(ElemK, GDStabCrR);
	m_pStlCompDesign->WriteStlCompStabFlgCrBeforeResult(ElemK, FlgStabCrR);
	m_pStlCompDesign->WriteStlCompStabWebCrBeforeResult(ElemK, WebStabCrR);

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Check_ConstructionNoStage(T_ELEM_K ElemK, int nLcomK4CS, STL_COMP_MEMB_POSD_SNiP &MembInD)
{
	// Base Data Setting and Check Data
	BOOL bChkIJ[2] = { FALSE, FALSE };
	STL_COMP_CHK_POSD_SNiP ChkSubPosD;

	if (!m_mStlCompDgnPosD_I.Lookup(ElemK, ChkSubPosD)) bChkIJ[0] = FALSE;
	else { bChkIJ[0] = IsNeedBeforeChk(ChkSubPosD); }
	if (!m_mStlCompDgnPosD_J.Lookup(ElemK, ChkSubPosD)) bChkIJ[1] = FALSE;
	else { bChkIJ[1] = IsNeedBeforeChk(ChkSubPosD); }

	if (!bChkIJ[0] && !bChkIJ[1]) return FALSE;

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	T_ELEM_D ElemD;
	if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD)) return FALSE;
	int nStype = m_pDoc->m_pAttrCtrl->GetRealSectType(ElemD.elpro);


	if (!m_pStlCompDesign) { ASSERT(0); return FALSE; }
	if (!m_pStlCompManager) { ASSERT(0); return FALSE; }

	STL_COMP_RES_BEFORE_BASE_SNiP MembBeforeBaseD[2];  // I & J

	// Get Design Length Info.
	T_EDLK_D DgnLengD;
	m_mDgnLengD.Lookup(ElemK, DgnLengD);
	Convert_Stl_CompDgnLengthData(DgnLengD, MembBeforeBaseD[0]);
	Convert_Stl_CompDgnLengthData(DgnLengD, MembBeforeBaseD[1]);

	for (int nIJ = 0; nIJ < 2; ++nIJ) // I & J
	{
		//       if(bChkIJ[nIJ])
		//       {
		GetDgnBaseD4Elem_IJ(ElemK, ElemD, nIJ, MembInD);
		Get_LocalForceD(ElemK, nIJ, 0, MembInD.CheckLcomD.GlobalForceD[0].LocalForceBeforeD, FALSE);
		MembInD.Memb[nIJ].DesignD.KappaFChkInfo.bAutoCalc = FALSE; // Before Composite의 경우, Kappa factor는 항상 1.0 적용,  Plastic Strain is not allowed at before composite.
		MembInD.Memb[nIJ].DesignD.KappaFChkInfo.dUDKappa_y = 1.0;
		MembInD.Memb[nIJ].DesignD.KappaFChkInfo.dUDKappa_z = 1.0;
		if (!m_pStlCompManager->Make_StlCompBeforeBaseData(MembInD, MembBeforeBaseD[nIJ])) bChkIJ[nIJ] = FALSE;
		/*      }/*/
	}
	if (MembInD.CheckLcomD.aUlmf.GetSize() < 1) MembInD.CheckLcomD.aUlmf.SetSize(1);
	if (MembInD.CheckLcomD.GlobalForceD.GetSize() < 1) MembInD.CheckLcomD.GlobalForceD.SetSize(1);

	int nFlgStrn[2] = { 0 };
	double dFlgSNRibMaxRat[4] = { 0.0 };
	double dFlgSNDeckMaxRat[4] = { 0.0 };
	double dFlgSNFBeamMaxRat[4] = { 0.0 };

	double dWebSNRibMaxRatL[2] = { 0.0 };
	double dWebSNDeckMaxRatL[2] = { 0.0 };
	double dWebSNDeckNoMMaxRatL[2] = { 0.0 };
	double dWebSNDeckVonMaxRatL[2] = { 0.0 };

	double dWebSNRibMaxRatR[2] = { 0.0 };
	double dWebSNDeckMaxRatR[2] = { 0.0 };
	double dWebSNDeckNoMMaxRatR[2] = { 0.0 };
	double dWebSNDeckVonMaxRatR[2] = { 0.0 };

	double dFlgStabGDMaxRat[2] = { 0.0 };
	double dFlgStabRibMaxRat[4] = { 0.0 };
	double dFlgStabDeckGMaxRat[4] = { 0.0 };
	double dFlgStabDeckLMaxRat[4] = { 0.0 };
	double dFlgStabDeckMaxRat[4] = { 0.0 };
	double dFlgStabFBeamGMaxRat[4] = { 0.0 };
	double dFlgStabFBeamLMaxRat[4] = { 0.0 };
	double dFlgStabFBeamMaxRat[4] = { 0.0 };
	double dWebStabRibMaxRatL[2] = { 0.0 };
	double dWebStabDeckMaxRatL[2] = { 0.0 };
	double dWebStabRibMaxRatR[2] = { 0.0 };
	double dWebStabDeckMaxRatR[2] = { 0.0 };

	int nBFSNRib[4] = { 0 };
	double dBFSNRibMaxRat[4] = { 0.0 };

	T_SDSN_FLG_CR FlgStrnCrR;
	T_SDSN_WEB_CR WebStrnCrR;
	T_SDSB_FLG_CR FlgStabCrR;
	T_SDSB_WEB_CR WebStabCrR;
	T_SDSB_GD_CR  GDStabCrR;


	int nLcomNum = m_pForcCtrl->Get_LcomDataCount();
	ADGNFORCE aPreForceLcom;
	m_pForcCtrl->Get_BeforeForce4PreCompo(EPairK, aPreForceLcom, nLcomNum);
	double dFactorCS = 1.0;
	INT_PTR nLcomSise = aPreForceLcom.GetSize();

	_DGN_FORC_CRC ForceNC;

	for (INT_PTR i = 0; i < nLcomSise; ++i)
	{
		ForceNC = aPreForceLcom[i];

		// Span을 구성하는 I,M,J Force 
		int nPosI = 0;
		int nPosM = 0;
		int nPosJ = 0;
		_DGN_FORC_CRC SpanForI = ForceNC;  SpanForI *= dFactorCS;
		_DGN_FORC_CRC SpanForM = ForceNC;  SpanForM *= dFactorCS;
		_DGN_FORC_CRC SpanForJ = ForceNC;  SpanForJ *= dFactorCS;
		ConvertStlCompCalcUlmCSForce(nPosI, nPosM, nPosJ, SpanForI, SpanForM, SpanForJ, MembInD.CheckLcomD.aUlmf[0]);

		ForceNC = aPreForceLcom[i];
		STL_COMP_FORCE_D &ComLcomF = MembInD.CheckLcomD.GlobalForceD[0];

		T_SCGD_DGN_FORCE StlCompForce;
		T_SDSN_FLG_CASE StrnFlgT;
		T_SDSN_FLG_CASE StrnFlgB;
		T_SDSN_WEB_CASE StrnWebD;
		T_SDSB_FLG_CASE StabFlgT;
		T_SDSB_FLG_CASE StabFlgB;
		T_SDSB_WEB_CASE StabWebD;
		T_SDSB_GD_CASE  StabGD;

		T_SDCR_BASE PreCompoKeyD;
		PreCompoKeyD.nLcomK = 1; // Pre Compo Case를, 1으로 구분 (Only Use 1)
		PreCompoKeyD.nDgnLcomK = 1; // Pre Compo Case를,1으로 구분 (Only Use 1)
		PreCompoKeyD.nConcurrent = 0; // No Concurrent
		PreCompoKeyD.nLcomSize = nLcomSise;  // Lcom Size 대신 Stage Step Size로 채움
		//PreCompoKeyD.nDgnLcomKFirst =nDgnLcomKFirst;

		double dWBStrnDeckRatI[2] = { 0.0 }; // L, R
		double dWBStrnDeckNoMRatI[2] = { 0.0 }; // L, R
		double dWBStrnDeckVonRatI[2] = { 0.0 }; // L, R
		double dWBStrnRibRatI[2] = { 0.0 }; // L, R

		double dWBStrnDeckRatJ[2] = { 0.0 }; // L, R
		double dWBStrnDeckNoMRatJ[2] = { 0.0 }; // L, R
		double dWBStrnDeckVonRatJ[2] = { 0.0 }; // L, R
		double dWBStrnRibRatJ[2] = { 0.0 }; // L, R

		double dWBStabRibRatI[2] = { 0.0 }; // L, R
		double dWBStabDeckRatI[2] = { 0.0 }; // L, R

		double dWBStabRibRatJ[2] = { 0.0 }; // L, R
		double dWBStabDeckRatJ[2] = { 0.0 }; // L, R

		for (int nIJ = 0; nIJ < 2; ++nIJ) // Repeat I-End , J-End
		{
			//if(!bChkIJ[nIJ]) continue;
			BOOL bPosI = (nIJ == 0);
			MembInD.nIJ = nIJ;
			MembInD.Memb[nIJ].ChIf.nChkPos = nIJ;
			Get_LocalForceD(ElemK, nIJ, 0, MembInD.CheckLcomD.GlobalForceD[0].LocalForceBeforeD, FALSE);
			ConvertStlCompCalcForce4CSStage(nIJ, ForceNC, MembInD.CheckLcomD.GlobalForceD[0]);
			ConvertStlCompForce(nIJ, ForceNC, StlCompForce.Elem[nIJ].ForBC);

			double dTFStrnRibRat = 0.0;
			double dBFStrnRibRat = 0.0;
			double dTFStrnDeckRat = 0.0;
			double dBFStrnDeckRat = 0.0;

			double dTFStrnFBRat = 0.0;
			double dBFStrnFBRat = 0.0;

			//
			double dStbGDRat = 0.0;
			double dTFStabRibRat = 0.0;
			double dBFStabRibRat = 0.0;
			double dTFStabDeckGRat = 0.0;
			double dTFStabDeckLRat = 0.0;
			double dTFStabDeckRat = 0.0;
			double dBFStabDeckGRat = 0.0;
			double dBFStabDeckLRat = 0.0;
			double dBFStabDeckRat = 0.0;
			double dTFStabFBGRat = 0.0;
			double dTFStabFBLRat = 0.0;
			double dTFStabFBRat = 0.0;
			double dBFStabFBGRat = 0.0;
			double dBFStabFBLRat = 0.0;
			double dBFStabFBRat = 0.0;

			// 실제 검토 하는 부분임.. 여기서 부터,
			if (MembInD.GlobalDgnD.bDgnCheck[0]) // Before Strength Check
			{
				STL_COMP_MEMB_RESD_SNiP_BEFORE_STR BeforeStrResD;
				m_pStlCompManager->Check_StlCompBeforeStrength(MembInD, MembBeforeBaseD[nIJ], BeforeStrResD);

				Convert_StlComp_BF_Str_Flg(nStype, TRUE, BeforeStrResD, StrnFlgT.StrnFlg[nIJ], m_ChkItemD.bRSCCheckBefore[0]);
				Convert_StlComp_BF_Str_Flg(nStype, TRUE, BeforeStrResD, StrnFlgB.StrnFlg[nIJ], m_ChkItemD.bRSCCheckBefore[1]);
				Convert_StlComp_BF_Str_Web(nStype, BeforeStrResD, StrnWebD.StrnWeb[nIJ], m_ChkItemD.bRSCCheckBefore[2]);
			}

			if (MembInD.GlobalDgnD.bDgnCheck[1]) // Before Stability Check
			{
				STL_COMP_MEMB_RESD_SNiP_BEFORE_STB BeforeStaResD;
				m_pStlCompManager->Check_StlCompBeforeStability(MembInD, MembBeforeBaseD[nIJ], BeforeStaResD);

				Convert_StlComp_BF_Sta_GD(nStype, BeforeStaResD, StabGD.StabGD[nIJ], m_ChkItemD.bRSCCheckBefore[3]); // Stability - Lateral torsional Bucking
				Convert_StlComp_BF_Sta_Flg(nStype, TRUE, BeforeStaResD, StabFlgT.StabFlg[nIJ], m_ChkItemD.bRSCCheckBefore[4]); // Stability - Top Flange - Global, Local, 
				Convert_StlComp_BF_Sta_Flg(nStype, FALSE, BeforeStaResD, StabFlgB.StabFlg[nIJ], m_ChkItemD.bRSCCheckBefore[5]); // Stability - Bot Flange 
				Convert_StlComp_BF_Sta_Web(nStype, BeforeStaResD, StabWebD.StabWeb[nIJ], m_ChkItemD.bRSCCheckBefore[6]); // Stability - Web
			}

			// Critical 값 찾기
			// *********************
			// Top
			// *********************
			T_SDSN_FLG  &StrnFlgTop = StrnFlgT.StrnFlg[nIJ];
			T_SDSB_FLG  &StabFlgTop = StabFlgT.StabFlg[nIJ];
			for (int nPart = 0; nPart < 3; ++nPart) // L,M,R
			{
				// -Strength
				T_SDPT_STRN_FLG &StrnChk = StrnFlgTop.StrnChk[nPart];
				dTFStrnRibRat = max(dTFStrnRibRat, StrnChk.dRat_Rib);
				dTFStrnDeckRat = max(dTFStrnDeckRat, StrnChk.dRat_Deck);
				dTFStrnFBRat = max(dTFStrnFBRat, StrnChk.dRat_FB);

				// -Stability
				T_SDPT_STAB_FLG &StabChk = StabFlgTop.StabChk[nPart];
				dTFStabRibRat = max(dTFStabRibRat, StabChk.dRat_Rib);
				dTFStabDeckGRat = max(dTFStabDeckGRat, StabChk.dRat_DeckG);
				dTFStabDeckLRat = max(dTFStabDeckLRat, StabChk.dRat_DeckL);
				dTFStabDeckRat = max(dTFStabDeckRat, StabChk.dRat_Deck);
				dTFStabFBGRat = max(dTFStabFBGRat, StabChk.dRat_FBG);
				dTFStabFBLRat = max(dTFStabFBLRat, StabChk.dRat_FBL);
				dTFStabFBRat = max(dTFStabFBRat, StabChk.dRat_FB);
			}

			// *********************
			// Bottom
			// *********************
			T_SDSN_FLG  &StrnFlgBot = StrnFlgB.StrnFlg[nIJ];
			T_SDSB_FLG  &StabFlgBot = StabFlgB.StabFlg[nIJ];
			for (int nPart = 0; nPart < 3; ++nPart) // L,M,R
			{
				// -Strength
				const T_SDPT_STRN_FLG &StrnChk = StrnFlgBot.StrnChk[nPart];
				dBFStrnRibRat = max(dBFStrnRibRat, StrnChk.dRat_Rib);
				dBFStrnDeckRat = max(dBFStrnDeckRat, StrnChk.dRat_Deck);
				dBFStrnFBRat = max(dBFStrnFBRat, StrnChk.dRat_FB);

				// -Stability
				const T_SDPT_STAB_FLG &StabChk = StabFlgBot.StabChk[nPart];
				dBFStabRibRat = max(dBFStabRibRat, StabChk.dRat_Rib);
				dBFStabDeckGRat = max(dBFStabDeckGRat, StabChk.dRat_DeckG);
				dBFStabDeckLRat = max(dBFStabDeckLRat, StabChk.dRat_DeckL);
				dBFStabDeckRat = max(dBFStabDeckRat, StabChk.dRat_Deck);
				dBFStabFBGRat = max(dBFStabFBGRat, StabChk.dRat_FBG);
				dBFStabFBLRat = max(dBFStabFBLRat, StabChk.dRat_FBL);
				dBFStabFBRat = max(dBFStabFBRat, StabChk.dRat_FB);
			}

			// *********************
			// Web
			// *********************
			T_SDSN_WEB  &StrnWeb = StrnWebD.StrnWeb[nIJ];
			T_SDSB_WEB  &StabWeb = StabWebD.StabWeb[nIJ];

			for (int nPart = 0; nPart < 2; ++nPart) // L,R
			{
				// -Strength
				const T_SDPT_STRN_WEB &WebStrnD = StrnWeb.WebD[nPart];

				// -Stability
				const T_SDPT_STAB_WEB &WebStabD = StabWeb.WebD[nPart];

				if (nIJ == 0)//I
				{
					dWBStrnRibRatI[nPart] = max(dWBStrnRibRatI[nPart], WebStrnD.dRat_Rib);
					dWBStrnDeckNoMRatI[nPart] = max(dWBStrnDeckNoMRatI[nPart], WebStrnD.dRat_DeckNoM);
					dWBStrnDeckVonRatI[nPart] = max(dWBStrnDeckVonRatI[nPart], WebStrnD.dRat_DeckVon);
					dWBStrnDeckRatI[nPart] = max(dWBStrnDeckRatI[nPart], WebStrnD.dRat_Deck);

					dWBStabRibRatI[nPart] = max(dWBStabRibRatI[nPart], WebStabD.dRat_Rib);
					dWBStabDeckRatI[nPart] = max(dWBStabDeckRatI[nPart], WebStabD.dRat_Deck);
				}
				else  //J
				{
					dWBStrnRibRatJ[nPart] = max(dWBStrnRibRatJ[nPart], WebStrnD.dRat_Rib);
					dWBStrnDeckNoMRatJ[nPart] = max(dWBStrnDeckNoMRatJ[nPart], WebStrnD.dRat_DeckNoM);
					dWBStrnDeckVonRatJ[nPart] = max(dWBStrnDeckVonRatJ[nPart], WebStrnD.dRat_DeckVon);
					dWBStrnDeckRatJ[nPart] = max(dWBStrnDeckRatJ[nPart], WebStrnD.dRat_Deck);

					dWBStabRibRatJ[nPart] = max(dWBStabRibRatJ[nPart], WebStabD.dRat_Rib);
					dWBStabDeckRatJ[nPart] = max(dWBStabDeckRatJ[nPart], WebStabD.dRat_Deck);
				}
			}

			// *********************
			// Global Deck_Stability
			// *********************
			T_SDSB_GD &GDStab = StabGD.StabGD[nIJ];
			dStbGDRat = GDStab.dRat_GD;
			int nTop = 2 * nIJ;
			int nBot = 2 * nIJ + 1;

			// - Strength
			GetCrFlgStrnRibRatio(nFlgStrn[nIJ], PreCompoKeyD, dTFStrnRibRat, dBFStrnRibRat, dFlgSNRibMaxRat[nTop], dFlgSNRibMaxRat[nBot], FlgStrnCrR.RibCr[nTop], FlgStrnCrR.RibCr[nBot]);
			GetCrFlgStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dTFStrnDeckRat, dBFStrnDeckRat, dFlgSNDeckMaxRat[nTop], dFlgSNDeckMaxRat[nBot], FlgStrnCrR.DeckCr[nTop], FlgStrnCrR.DeckCr[nBot]);
			GetCrFlgStrnFBeamRatio(nFlgStrn[nIJ], PreCompoKeyD, dTFStrnFBRat, dBFStrnFBRat, dFlgSNFBeamMaxRat[nTop], dFlgSNFBeamMaxRat[nBot], FlgStrnCrR.FBeamCr[nTop], FlgStrnCrR.FBeamCr[nBot]);
			// - Strength : Web-Left - rib, Deck(max), Deck(Shear stress), Deck(Combined stress),
			if (nIJ == 0)
			{
				GetCrWebStrnRibRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnRibRatI[0], dWebSNRibMaxRatL[nIJ], WebStrnCrR.RibCrL[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnDeckRatI[0], dWebSNDeckMaxRatL[nIJ], WebStrnCrR.DeckCrL[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnDeckNoMRatI[0], dWebSNDeckNoMMaxRatL[nIJ], WebStrnCrR.DeckNoMCrL[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnDeckVonRatI[0], dWebSNDeckVonMaxRatL[nIJ], WebStrnCrR.DeckVonCrL[nIJ]);
				// - Strength : Web-Right
				GetCrWebStrnRibRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnRibRatI[1], dWebSNRibMaxRatR[nIJ], WebStrnCrR.RibCrR[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnDeckRatI[1], dWebSNDeckMaxRatR[nIJ], WebStrnCrR.DeckCrR[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnDeckNoMRatI[1], dWebSNDeckNoMMaxRatR[nIJ], WebStrnCrR.DeckNoMCrR[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnDeckVonRatI[1], dWebSNDeckVonMaxRatR[nIJ], WebStrnCrR.DeckVonCrR[nIJ]);
			}
			else
			{
				GetCrWebStrnRibRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnRibRatJ[0], dWebSNRibMaxRatL[nIJ], WebStrnCrR.RibCrL[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnDeckRatJ[0], dWebSNDeckMaxRatL[nIJ], WebStrnCrR.DeckCrL[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnDeckNoMRatJ[0], dWebSNDeckNoMMaxRatL[nIJ], WebStrnCrR.DeckNoMCrL[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnDeckVonRatJ[0], dWebSNDeckVonMaxRatL[nIJ], WebStrnCrR.DeckVonCrL[nIJ]);
				// - Strength : Web-Right
				GetCrWebStrnRibRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnRibRatJ[1], dWebSNRibMaxRatR[nIJ], WebStrnCrR.RibCrR[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnDeckRatJ[1], dWebSNDeckMaxRatR[nIJ], WebStrnCrR.DeckCrR[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnDeckNoMRatJ[1], dWebSNDeckNoMMaxRatR[nIJ], WebStrnCrR.DeckNoMCrR[nIJ]);
				GetCrWebStrnDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStrnDeckVonRatJ[1], dWebSNDeckVonMaxRatR[nIJ], WebStrnCrR.DeckVonCrR[nIJ]);
			}

			// Stability
			// - GLT
			GetCrGDStabRatio(nFlgStrn[nIJ], PreCompoKeyD, dStbGDRat, dFlgStabGDMaxRat[nIJ], GDStabCrR.GDCr[nIJ]);
			// - Rib
			GetCrFlgStabRibRatio(nFlgStrn[nIJ], PreCompoKeyD, dTFStabRibRat, dBFStabRibRat, dFlgStabRibMaxRat[nTop], dFlgStabRibMaxRat[nBot], FlgStabCrR.RibCr[nTop], FlgStabCrR.RibCr[nBot]);
			// - Deck : Global, Local, max
			GetCrFlgStabDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dTFStabDeckGRat, dBFStabDeckGRat, dFlgStabDeckGMaxRat[nTop], dFlgStabDeckGMaxRat[nBot], FlgStabCrR.DeckGCr[nTop], FlgStabCrR.DeckGCr[nBot]);
			GetCrFlgStabDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dTFStabDeckLRat, dBFStabDeckLRat, dFlgStabDeckLMaxRat[nTop], dFlgStabDeckLMaxRat[nBot], FlgStabCrR.DeckLCr[nTop], FlgStabCrR.DeckLCr[nBot]);
			GetCrFlgStabDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dTFStabDeckRat, dBFStabDeckRat, dFlgStabDeckMaxRat[nTop], dFlgStabDeckMaxRat[nBot], FlgStabCrR.DeckCr[nTop], FlgStabCrR.DeckCr[nBot]);
			// - FB : Global, Local, max
			GetCrFlgStabFBeamRatio(nFlgStrn[nIJ], PreCompoKeyD, dTFStabFBGRat, dBFStabFBGRat, dFlgStabFBeamGMaxRat[nTop], dFlgStabFBeamGMaxRat[nBot], FlgStabCrR.FBeamGCr[nTop], FlgStabCrR.FBeamGCr[nBot]);
			GetCrFlgStabFBeamRatio(nFlgStrn[nIJ], PreCompoKeyD, dTFStabFBLRat, dBFStabFBLRat, dFlgStabFBeamLMaxRat[nTop], dFlgStabFBeamLMaxRat[nBot], FlgStabCrR.FBeamLCr[nTop], FlgStabCrR.FBeamLCr[nBot]);
			GetCrFlgStabFBeamRatio(nFlgStrn[nIJ], PreCompoKeyD, dTFStabFBRat, dBFStabFBRat, dFlgStabFBeamMaxRat[nTop], dFlgStabFBeamMaxRat[nBot], FlgStabCrR.FBeamCr[nTop], FlgStabCrR.FBeamCr[nBot]);
			if (nIJ == 0)
			{
				// - Web-left
				GetCrWebStabRibRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStabRibRatI[0], dWebStabRibMaxRatL[nIJ], WebStabCrR.RibCrL[nIJ]);
				GetCrWebStabDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStabDeckRatI[0], dWebStabDeckMaxRatL[nIJ], WebStabCrR.DeckCrL[nIJ]);
				// - Web-right
				GetCrWebStabRibRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStabRibRatI[1], dWebStabRibMaxRatR[nIJ], WebStabCrR.RibCrR[nIJ]);
				GetCrWebStabDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStabDeckRatI[1], dWebStabDeckMaxRatR[nIJ], WebStabCrR.DeckCrR[nIJ]);
			}
			else
			{
				// - Web-left
				GetCrWebStabRibRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStabRibRatJ[0], dWebStabRibMaxRatL[nIJ], WebStabCrR.RibCrL[nIJ]);
				GetCrWebStabDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStabDeckRatJ[0], dWebStabDeckMaxRatL[nIJ], WebStabCrR.DeckCrL[nIJ]);
				// - Web-right
				GetCrWebStabRibRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStabRibRatJ[1], dWebStabRibMaxRatR[nIJ], WebStabCrR.RibCrR[nIJ]);
				GetCrWebStabDeckRatio(nFlgStrn[nIJ], PreCompoKeyD, dWBStabDeckRatJ[1], dWebStabDeckMaxRatR[nIJ], WebStabCrR.DeckCrR[nIJ]);
			}

			nFlgStrn[nIJ]++;
		}

		// 각 하중조합에 대한 결과 저장.
		m_pStlCompDesign->WriteStlCompStageForce(ElemK, PreCompoKeyD.nLcomK, StlCompForce);
		m_pStlCompDesign->WriteStlCompTopFlgStrnBeforeResult(ElemK, PreCompoKeyD.nLcomK, StrnFlgT);
		m_pStlCompDesign->WriteStlCompBotFlgStrnBeforeResult(ElemK, PreCompoKeyD.nLcomK, StrnFlgB);
		m_pStlCompDesign->WriteStlCompWebStrnBeforeResult(ElemK, PreCompoKeyD.nLcomK, StrnWebD);

		m_pStlCompDesign->WriteStlCompGDStabBeforeResult(ElemK, PreCompoKeyD.nLcomK, StabGD);
		m_pStlCompDesign->WriteStlCompTopFlgStabBeforeResult(ElemK, PreCompoKeyD.nLcomK, StabFlgT);
		m_pStlCompDesign->WriteStlCompBotFlgStabBeforeResult(ElemK, PreCompoKeyD.nLcomK, StabFlgB);
		m_pStlCompDesign->WriteStlCompWebStabBeforeResult(ElemK, PreCompoKeyD.nLcomK, StabWebD);
	}

	// 요소에서 가장 불리한 하중조합의 결과 저장.
	m_pStlCompDesign->WriteStlCompStrnFlgCrBeforeResult(ElemK, FlgStrnCrR);
	m_pStlCompDesign->WriteStlCompStrnWebCrBeforeResult(ElemK, WebStrnCrR);
	m_pStlCompDesign->WriteStlCompStabGDCrBeforeResult(ElemK, GDStabCrR);
	m_pStlCompDesign->WriteStlCompStabFlgCrBeforeResult(ElemK, FlgStabCrR);
	m_pStlCompDesign->WriteStlCompStabWebCrBeforeResult(ElemK, WebStabCrR);

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Make_DesignBaseData()
{
	if (!m_pDoc->IsPostMode()) return FALSE;

	T_CPGD_D CpgdD;
	if (!m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD)) return FALSE;

	int nReturn = 0;

	// Set arElemK	
	m_arElemK.RemoveAll();
	CArray<T_ELEM_K, T_ELEM_K> arElemK;
	CDBLib::GetElemforStlCompGirderGirder(arElemK);

	m_arElemK.Copy(arElemK);

	Make_ElemSpanMapKey();
	Make_ElemSpanData();
	GetDgnLengD();

	if (nReturn == 0)  nReturn = (Make_DgnSectD() ? 0 : 1);    // 추가 작업 필요
	if (nReturn == 0)  nReturn = (Make_Matl2SectD() ? 0 : 2);
	if (nReturn == 0)  nReturn = (Make_KappaFactorChkPosData() ? 0 : 3);
	if (nReturn == 0)  nReturn = (Make_FloorBeamData() ? 0 : 4);
	if (nReturn == 0)  nReturn = (Make_AddWebPropData() ? 0 : 5);
	if (nReturn == 0)  nReturn = (Make_DesignPosData() ? 0 : 6);

	if (CpgdD.bDesignParameter[2]) // Fatigue Design
	{
		if (nReturn == 0)  nReturn = (Make_FatiChkPosData() ? 0 : 7);
	}
	if (CpgdD.bDesignParameter[3]) // Shear Connector Design
	{
		if (nReturn == 0)  nReturn = (Make_ShearConnectorGroupData() ? 0 : 8);
	}
	if (CpgdD.bDesignParameter[4]) // Concrete Crack Design
	{
		if (nReturn == 0)  nReturn = (Make_LimitStateGroupParamData() ? 0 : 9);
	}

	CString strErrMsg;
	switch (nReturn)
	{
	case  1:   strErrMsg.Format(_LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_SECTION));                                            GSaveHistoryFormatNF(strErrMsg);   break;
	case  2:   strErrMsg.Format(_LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_MATERIAL));                                         GSaveHistoryFormatNF(strErrMsg);   break;
	case  3:   strErrMsg.Format(_LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_KAPPAF));                                              GSaveHistoryFormatNF(strErrMsg);   break;
	case  4:   strErrMsg.Format(_LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_FBD));                                                      GSaveHistoryFormatNF(strErrMsg);   break;
	case  5:   strErrMsg.Format(_LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_ADD_WEB_PROP));                            GSaveHistoryFormatNF(strErrMsg);   break;
	case  6:   strErrMsg.Format(_LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_DGN_POSITION));                               GSaveHistoryFormatNF(strErrMsg);   break;
	case  7:   strErrMsg.Format(_LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_FATIGUE_DGN_POSITION));            GSaveHistoryFormatNF(strErrMsg);   break;
	case  8:   strErrMsg.Format(_LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_SHEAR_CONNECTOR_GROUP));     GSaveHistoryFormatNF(strErrMsg);   break;
	case  9:   strErrMsg.Format(_LS(IDS_DGN_CPG_MAKE_DGN_DATA_ERR_LIMIT_GROUP_PARAM));                  GSaveHistoryFormatNF(strErrMsg);   break;
	default:   break;
	}

	if (nReturn != 0)  return FALSE;

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::Make_ElemSpanMapKey()
{
	m_mElemSpanK.RemoveAll();

	CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
	m_pDoc->m_pAttrCtrl->GetSpanKeyList(aSpanKeyList);

	// Span key로 돌리면서 요소를 map에 setting...
	int nSpanKeySize = aSpanKeyList.GetSize();
	T_SPAN_D SpanD;
	for (int i = 0; i < nSpanKeySize; ++i)
	{
		SpanD.Initialize();
		T_SPAN_K SpanK = aSpanKeyList[i];
		m_pDoc->m_pAttrCtrl->GetSpan(SpanK, SpanD);
		int nSpanElemNum = SpanD.aElemBase.GetSize();
		UINT iL_count = 1;
		CArray<double, double> aElemSpan;

		for (int j = 0; j < nSpanElemNum; ++j)
		{
			T_SPAN_BASE &SpanBase = SpanD.aElemBase[j];
			T_ELEM_K ElemK2 = SpanBase.ElemK;
			T_SPAN_K SpanValK = 0;
			if (m_mElemSpanK.Lookup(SpanBase.ElemK, SpanValK) == FALSE)
			{
				m_mElemSpanK.SetAt(ElemK2, SpanK);
			}
		}
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Make_ElemSpanData()
{
	m_mElemSpanD.RemoveAll();

	CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
	m_pDoc->m_pAttrCtrl->GetSpanKeyList(aSpanKeyList);

	int nSpanKeySize = aSpanKeyList.GetSize();
	T_SPAN_D SpanD;
	for (int i = 0; i < nSpanKeySize; ++i)
	{
		SpanD.Initialize();
		T_SPAN_K SpanK = aSpanKeyList[i];
		m_pDoc->m_pAttrCtrl->GetSpan(SpanK, SpanD);
		int nSpanElemNum = SpanD.aElemBase.GetSize();
		UINT iL_count = 1;

		for (int j = 0; j < nSpanElemNum; ++j)
		{
			T_SPAN_BASE &SpanBase = SpanD.aElemBase[j];
			T_ELEM_K ElemK2 = SpanBase.ElemK;

			m_mElemSpanD.SetAt(ElemK2, SpanBase);
		}
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Make_DgnSectD()
{
	m_mElemKtoDgnSectK.RemoveAll();
	m_mStlCompSect.RemoveAll();
	m_mStlCompSectElem.RemoveAll();

	T_CPGD_D CpgdD;
	if (!m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD)) CpgdD.Initialize();
	double dConcreteContributeF_1stG = CpgdD.dPsi_1stGroup;
	double dConcreteContributeF_2ndG = CpgdD.dPsi_2ndGroup;

	CArray<T_ELEM_K, T_ELEM_K> aCheckElemK;
	aCheckElemK.Copy(m_arElemK);
	INT_PTR nChkElemSize = aCheckElemK.GetSize();

	T_STL_COMP_SECT_STFN_INX  SectStfnId;
	STL_COMP_SECT_ELEMD_SNiP SectElem;


	// Make Serial DgnSectK
	//   T_SECT_K DgnSectSerialK = 0;
	//   for(INT_PTR i=0; i < nChkElemSize; ++i)
	//   {		
	//     T_ELEM_K ElemK = aCheckElemK[i];
	//     T_ELEM_D ElemD, ElemPreD;
	//     ElemD.Initialize();
	//     ElemPreD.Initialize();
	//     BOOL bGetElem    = m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD);	  
	//     BOOL bGetElemPre = m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemPreD);	  
	//     if(!bGetElem || !bGetElemPre)	continue;
	//   }

	for (INT_PTR i = 0; i < nChkElemSize; ++i)
	{
		T_ELEM_K ElemK = aCheckElemK[i];
		ElemPairK EPairK(ElemK, EN_EL_BEAM);

		T_ELEM_D ElemD, ElemPreD;
		ElemD.Initialize();
		ElemPreD.Initialize();
		BOOL bGetElem = m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD);
		BOOL bGetElemPre = m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemPreD);
		if (!bGetElem || !bGetElemPre)	continue;

		T_SECT_K SectK = ElemD.elpro;
		T_SECT_K SectPreK = ElemPreD.elpro;
		T_MATL_K MatlK = ElemPreD.elmat;
		T_MATD_D MatlD;
		m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, MatlD);
		double dEst = MatlD.Data1.Analysis.Elast;
		double dEr = MatlD.MainRebarData.B_Elast;
		double dnr = dEr < 0.0 ? 1.0 : dEst / dEr; // Rbar없는 경우, 탄성계수비 1.0으로 처리

		if (m_mStlCompSect.Lookup(SectK, SectElem)) continue;
		SectElem.Initialize();
		SectStfnId.Initialize();

		T_TSGR_K TsgrK = m_pDoc->m_pAttrCtrl->GetElemTsgrKey(ElemK);

		T_SECT_D SectD;

		if (TsgrK > 0)
		{
			T_SECT_D TSectD;
			m_pDoc->m_pAttrCtrl->GetSect(SectPreK, TSectD);

			T_TSGR_D TsgrD;
			double dTotalLength = 0.0;
			double dPositionI = 0.0;
			double dPositionJ = 0.0;
			if (m_pDoc->m_pAttrCtrl->GetTsgr(TsgrK, TsgrD))
			{
				double dTsgrTotLength = 0.0;
				m_pDoc->m_pAttrCtrl->CalcTsgrTotLength(TsgrD, ElemK, dTsgrTotLength, dPositionI);
				//         int nTsgrElemList = TsgrD.aElemList.GetSize();
				//         for(int i=0; i<nTsgrElemList; ++i)
				//         {
				//           if(TsgrD.aElemList[i]==ElemK) dPositionI = dTsgrTotLength;
				//           dTsgrTotLength += m_pDoc->calcLAVElem(TsgrD.aElemList[i]);
				//         }
				dPositionJ = dPositionI + m_pDoc->calcLAVElem(ElemD);

				m_pDoc->m_pSectDB->CalcTaperedSection(TSectD, TsgrD, dTsgrTotLength, dPositionI, dPositionJ, 0, SectD);
			}
		}
		else
		{
			m_pDoc->m_pPostCtrl->GetSectPost(SectK, SectD);
		}

		T_STPO_D StpoD;
		CSectUtil::GetStressPointSOD_Stiffener(SectD, StpoD);

		BOOL bTapered = SectD.nStype == D_SECT_TYPE_TAPERED ? TRUE : FALSE;
		int nSectType = m_pDoc->m_pAttrCtrl->GetRealSectType(SectK);

		for (int j = 0; j < 2; ++j) //  I & J
		{
			BOOL bPosiI = (j == 0);
			//if (!bTapered && j==1) continue;
			T_SECT_SECTBASE_D &SectBaseD = (!bPosiI && bTapered) ? SectD.CmpTapJ : SectD.SectBefore.SectI;

			STL_COMP_SECT_POSD &rStlCompSectPos = SectElem.SectD[j];
			rStlCompSectPos.SectInfo.bIsTapered = bTapered;

			T_SECT_STIFFNESS SpscSect[5]; // 0: Pure Sect, 1 : Positive Moment Stiffness(1stG),  2 : Negative Moment Stiffness(1stG), 1 : Positive Moment Stiffness(2ndG),  2 : Negative Moment Stiffness(2ndG)
			for (int k = 0; k < 5; ++k) SpscSect[k].Initialize();

			BOOL bSecf = FALSE;
			BOOL bPostMode = TRUE;
			BOOL bCrack = FALSE;
			int nRebarCalcMethod = 0;
			TCHAR chIJ = bPosiI ? 'I' : 'J';
			SpscSect[0] = *CSectUtil::GetProperStiffPosition(SectD, TRUE, 0, chIJ);

			bCrack = FALSE;  nRebarCalcMethod = 0;
			BOOL bStiffn1stG_Posi = CSectUtil::GetCompoStlGirderSectCR(bPostMode, bCrack, nRebarCalcMethod, dConcreteContributeF_1stG, EPairK, bPosiI, bSecf, SpscSect[1], dnr);

			bCrack = TRUE;  nRebarCalcMethod = 0;
			BOOL bStiffn1stG_Nega = CSectUtil::GetCompoStlGirderSectCR(bPostMode, bCrack, nRebarCalcMethod, dConcreteContributeF_1stG, EPairK, bPosiI, bSecf, SpscSect[2], dnr);

			bCrack = FALSE;  nRebarCalcMethod = 0;
			BOOL bStiffn2ndG_Posi = CSectUtil::GetCompoStlGirderSectCR(bPostMode, bCrack, nRebarCalcMethod, dConcreteContributeF_2ndG, EPairK, bPosiI, bSecf, SpscSect[3], dnr);

			bCrack = TRUE;  nRebarCalcMethod = 0;
			BOOL bStiffn2ndG_Nega = CSectUtil::GetCompoStlGirderSectCR(bPostMode, bCrack, nRebarCalcMethod, dConcreteContributeF_2ndG, EPairK, bPosiI, bSecf, SpscSect[4], dnr);

			BOOL bCalcOK = bStiffn1stG_Posi && bStiffn1stG_Nega && bStiffn2ndG_Posi && bStiffn2ndG_Nega;

			if (bCalcOK)
			{
				Convert_StlComp_SectData(j, &SectD, StpoD, SectStfnId.Stiff[j], SpscSect, SectElem.SectD[j]);

				T_SECT_STIFF_D SectStiff;
				SectStiff.aStiffShape.Copy(SectBaseD.SODStiffener.aStiffShape);
				CalcStiffnessOfStiffener(SectStiff, SectElem.SectD[j].SectInfo.BeforeCompSect.Stiffener);
				CalcStiffnessOfStiffener(SectStiff, SectElem.SectD[j].SectInfo.AfterCompSect.Stiffener);
				CalcStiffnessOfStiffener(SectStiff, SectElem.SectD[j].SectInfo.SlabSect.Stiffener);
			}
			Get_RebarData(ElemK, bPosiI, SectD, rStlCompSectPos);
		}

		// Set Sect.
		m_mStlCompSect.SetAt(SectK, SectElem);
		m_SectStfnIdx.SetAt(SectK, SectStfnId);
	}

	if (m_mStlCompSect.GetCount() == 0) return FALSE;

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Make_DgnSectKeyList()
{
	m_mChkSectKey.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> aChkElemD;
	if (!CDBLib::GetElemforStlCompGirderGirder(aChkElemD)) return FALSE;

	T_ELEM_D ElemD;
	INT_PTR nChkElemSize = aChkElemD.GetSize();
	for (INT_PTR nElem = 0; nElem < nChkElemSize; ++nElem)
	{
		T_ELEM_K ElemK = aChkElemD[nElem];
		ElemD.Initialize();
		if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD)) continue;

		T_SECT_K SectK = ElemD.elpro;
		if (!m_mChkSectKey.Lookup(SectK, SectK))
		{
			m_mChkSectKey.SetAt(SectK, SectK);
		}
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Make_Matl2SectD()
{
	m_mStlCompMatl2Sect.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> aChkElemD;
	if (!CDBLib::GetElemforStlCompGirderGirder(aChkElemD)) return FALSE;

	T_ELEM_D ElemD;
	INT_PTR nChkElemSize = aChkElemD.GetSize();

	for (INT_PTR nElem = 0; nElem < nChkElemSize; ++nElem)
	{
		T_ELEM_K ElemK = aChkElemD[nElem];
		ElemD.Initialize();
		if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD)) continue;

		T_SECT_K SectK = ElemD.elpro;
		T_MATL_K MatlK = ElemD.elmat;
		STL_COMP_MAT_SNiP StlCompD;
		if (m_mStlCompMatl2Sect.Lookup(ElemK, StlCompD)) continue;

		T_SDMS_D SpmsD;
		T_MATD_D MatdD;

		if (m_pDoc->m_pAttrCtrl2->GetSdms(SectK, SpmsD))   // Sect2Matl Info Exist Case
		{
			SLT_MATL_SNiP_POSD SectMatlD;
			for (int i = 0; i < 3; ++i)
			{
				MatdD.Initialize();
				if (m_pDoc->m_pAttrCtrl->GetMatlDesign(SpmsD.Part[i], MatdD))
				{
					if (i == 0) Convert_Sect2MatlD(MatdD, SectMatlD.MatTopF);
					if (i == 1) Convert_Sect2MatlD(MatdD, SectMatlD.MatWeb);
					if (i == 2) Convert_Sect2MatlD(MatdD, SectMatlD.MatBotF);
				}
			}

			for (int i = 0; i < 3; ++i)
			{
				MatdD.Initialize();
				if (m_pDoc->m_pAttrCtrl->GetMatlDesign(SpmsD.Stiff[i], MatdD))
				{
					if (i == 0) Convert_Sect2MatlD(MatdD, SectMatlD.MatTopF_Rib);
					if (i == 1) Convert_Sect2MatlD(MatdD, SectMatlD.MatWeb_Rib);
					if (i == 2) Convert_Sect2MatlD(MatdD, SectMatlD.MatBotF_Rib);
				}
			}

			MatdD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, MatdD)) continue;
			if (!Convert_Stl2StlCompMatl(MatdD, SectMatlD, StlCompD)) return FALSE;
		}
		else    // Sect2Matl Info Exist Case
		{
			MatdD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, MatdD)) continue;
			if (!Convert_Stl2StlCompMatl(MatdD, StlCompD)) return FALSE;
		}

		m_mStlCompMatl2Sect.SetAt(ElemK, StlCompD);
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Make_KappaFactorChkPosData()
{
	CArray<T_SDPS_K, T_SDPS_K> aSdpsKey;
	m_pDoc->m_pAttrCtrl2->GetSdpsKeyList(aSdpsKey);
	INT_PTR nSdpsSize = aSdpsKey.GetSize();

	if (nSdpsSize > 0)
	{
		for (INT_PTR i = 0; i < nSdpsSize; ++i)
		{
			T_SDPS_K SdpsK = aSdpsKey[i];
			T_SDPS_D SpdsD; SpdsD.Initialize();
			if (m_pDoc->m_pAttrCtrl2->GetSdps(SdpsK, SpdsD))
			{
				STL_KAPPA_INPUT_D KappaInD;
				ConvertStlCompKappaFactor(SpdsD.SdpsSub[0], KappaInD);
				m_mStlCompKappaFactor_I.SetAt(SdpsK, KappaInD);

				STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD ChkKappaPoint;
				ConvertStlCompKappaFactorChkPos(SpdsD.SdpsSub[0], ChkKappaPoint);
				m_mStlCompKappaChkPos_I.SetAt(SdpsK, ChkKappaPoint);

				if (!SpdsD.bSameIJ)
				{
					ConvertStlCompKappaFactor(SpdsD.SdpsSub[1], KappaInD);
					ConvertStlCompKappaFactorChkPos(SpdsD.SdpsSub[1], ChkKappaPoint);
				}

				m_mStlCompKappaFactor_J.SetAt(SdpsK, KappaInD);
				m_mStlCompKappaChkPos_J.SetAt(SdpsK, ChkKappaPoint);
			}
		}
	}
	else // Default 값 세팅
	{
		// Set arElemK	
		CArray<T_ELEM_K, T_ELEM_K> arElemK;
		CDBLib::GetElemforStlCompGirderGirder(arElemK);

		INT_PTR nSize = arElemK.GetSize();

		for (INT_PTR i = 0; i < nSize; ++i)
		{
			T_ELEM_K ElemK = arElemK[i];

			T_ELEM_D ElemD;	ElemD.Initialize();
			if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD)) ASSERT(0);

			T_SECT_K SectK = ElemD.elpro;

			T_SECT_D SectD;
			m_pDoc->m_pPostCtrl->GetSectPost(SectK, SectD);

			CArray<T_POINT_SSM, T_POINT_SSM&> aStressPoint;

			STL_KAPPA_INPUT_D KappaInD;
			KappaInD.bAutoCalc = TRUE;
			KappaInD.dUDKappa_y = 1.0;
			KappaInD.dUDKappa_z = 1.0;
			m_mStlCompKappaFactor_I.SetAt(ElemK, KappaInD);
			m_mStlCompKappaFactor_J.SetAt(ElemK, KappaInD);

			STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD ChkKappaPoint;
			aStressPoint.RemoveAll();
			CSectUtil::GetStressPointSOD(SectD, 0, 0, aStressPoint, TRUE);  //  default point 가져오기		
			ChkKappaPoint.Initialize();
			ConvertStlCompKappaFactorChkPosDefault(aStressPoint, ChkKappaPoint);
			m_mStlCompKappaChkPos_I.SetAt(ElemK, ChkKappaPoint);

			aStressPoint.RemoveAll();
			double dLeng = m_pDoc->calcLAVElem(ElemK);
			CSectUtil::GetStressPointSOD(SectD, dLeng, 1, aStressPoint, TRUE);  //  default point 가져오기		
			ChkKappaPoint.Initialize();
			ConvertStlCompKappaFactorChkPosDefault(aStressPoint, ChkKappaPoint);
			m_mStlCompKappaChkPos_J.SetAt(ElemK, ChkKappaPoint);
		}

	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Make_FatiChkPosData()
{
	CArray<T_SDFC_K, T_SDFC_K> aSdfcKey;
	m_pDoc->m_pAttrCtrl2->GetSdfcKeyList(aSdfcKey);
	INT_PTR nSdfcSize = aSdfcKey.GetSize();
	for (INT_PTR i = 0; i < nSdfcSize; ++i)
	{
		T_SDFC_K SdfcK = aSdfcKey[i];
		T_SDFC_D SdfcD; SdfcD.Initialize();
		if (m_pDoc->m_pAttrCtrl2->GetSdfc(SdfcK, SdfcD))
		{
			STL_COMP_FATI_CHK_POS_DGND_SNiP FatChkInfo;
			ConvertStlCompFatiCheckPos(0, SdfcD, FatChkInfo);
			m_mStlCompFatiChkPos_I.SetAt(SdfcK, FatChkInfo);

			if (!SdfcD.bSameIJ)
			{
				ConvertStlCompFatiCheckPos(1, SdfcD, FatChkInfo);
			}
			m_mStlCompFatiChkPos_J.SetAt(SdfcK, FatChkInfo);
		}
	}
	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Make_FloorBeamData()
{
	CArray<T_SDTS_K, T_SDTS_K> aSdtsKey;
	m_pDoc->m_pAttrCtrl2->GetSdtsKeyList(aSdtsKey);
	INT_PTR nSdtsSize = aSdtsKey.GetSize();

	for (INT_PTR i = 0; i < nSdtsSize; ++i)
	{
		T_SDTS_K SdtsK = aSdtsKey[i];
		T_SDTS_D SdtsD; SdtsD.Initialize();
		if (m_pDoc->m_pAttrCtrl2->GetSdts(SdtsK, SdtsD))
		{
			STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D StaFbeamD;

			ConvertStlCompFloorBeam(0, SdtsD, StaFbeamD);

			m_mStlCompFloorBeam_I.SetAt(SdtsK, StaFbeamD);

			if (!SdtsD.bBeamSameIJ)
			{
				ConvertStlCompFloorBeam(1, SdtsD, StaFbeamD);
			}
			m_mStlCompFloorBeam_J.SetAt(SdtsK, StaFbeamD);
		}
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Make_AddWebPropData()
{
	CArray<T_SDAP_K, T_SDAP_K> aSdapKey;
	m_pDoc->m_pAttrCtrl2->GetSdapKeyList(aSdapKey);
	int nSdapSize = aSdapKey.GetSize();
	for (int i = 0; i < nSdapSize; ++i)
	{
		T_SDAP_K SdapK = aSdapKey[i];
		T_SDAP_D SdapD; SdapD.Initialize();
		if (m_pDoc->m_pAttrCtrl2->GetSdap(SdapK, SdapD))
		{
			T_ELEM_D ElemD; ElemD.Initialize();
			if (!m_pDoc->m_pPostCtrl->GetElemPost(SdapK, ElemD)) continue;

			T_SECT_D SectD;
			m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro, SectD);
			int nStype = SectD.nStype;
			if (SectD.nStype == D_SECT_TYPE_TAPERED) nStype = SectD.SectBefore.nStype;

			STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM  LforWeb;
			STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM LforWebP;

			BOOL bBeforeComp = TRUE;
			ConvertStlCompWebVerticalForce(nStype, 0, SdapD, LforWeb, LforWebP, bBeforeComp);
			m_mStlCompLForWeb_I[0].SetAt(SdapK, LforWeb);
			m_mStlCompLForWebPlate_I[0].SetAt(SdapK, LforWebP);

			LforWeb.Initialize();
			LforWebP.Initialize();
			ConvertStlCompWebVerticalForce(nStype, 1, SdapD, LforWeb, LforWebP, bBeforeComp);
			m_mStlCompLForWeb_J[0].SetAt(SdapK, LforWeb);
			m_mStlCompLForWebPlate_J[0].SetAt(SdapK, LforWebP);

			LforWeb.Initialize();
			LforWebP.Initialize();

			bBeforeComp = FALSE;
			ConvertStlCompWebVerticalForce(nStype, 0, SdapD, LforWeb, LforWebP, bBeforeComp);
			m_mStlCompLForWeb_I[1].SetAt(SdapK, LforWeb);
			m_mStlCompLForWebPlate_I[1].SetAt(SdapK, LforWebP);

			LforWeb.Initialize();
			LforWebP.Initialize();
			ConvertStlCompWebVerticalForce(nStype, 1, SdapD, LforWeb, LforWebP, bBeforeComp);
			m_mStlCompLForWeb_J[1].SetAt(SdapK, LforWeb);
			m_mStlCompLForWebPlate_J[1].SetAt(SdapK, LforWebP);
		}
	}

	return TRUE;
}
BOOL CDgnRUSStlCompDataCtrl::Make_LimitStateGroupParamData()
{
	CArray<T_SERV_K, T_SERV_K> aServKey;
	m_pDoc->m_pAttrCtrl->GetServKeyList(aServKey);
	INT_PTR nServSize = aServKey.GetSize();

	for (INT_PTR i = 0; i < nServSize; ++i)
	{
		T_SERV_K ServK = aServKey[i];
		T_SERV_D ServD; ServD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetServ(ServK, ServD))
		{
			STL_COMP_LIMIT_CRACK_STATE_INPUT_D CrackD;

			ConvertStlCompLimitGroupParam(ServD, CrackD);

			m_mStlCompLimitGroupParam.SetAt(ServK, CrackD);
		}
	}

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::Make_DesignPosData()
{
	CArray<T_SDDP_K, T_SDDP_K> aSddpKey;

	m_pDoc->m_pAttrCtrl2->GetSddpKeyList(aSddpKey);
	INT_PTR nSddpSize = aSddpKey.GetSize();

	for (INT_PTR i = 0; i < nSddpSize; ++i)
	{
		T_SDDP_K SddpK = aSddpKey[i];
		T_SDDP_D SddpD; SddpD.Initialize();
		if (m_pDoc->m_pAttrCtrl2->GetSddp(SddpK, SddpD))
		{
			T_ELEM_D ElemD;
			ElemD.Initialize();
			if (!m_pDoc->m_pPostCtrl->GetElemPost(SddpK, ElemD)) continue;

			STL_COMP_SECT_ELEMD_SNiP StlCompSect;
			if (!m_mStlCompSect.Lookup(ElemD.elpro, StlCompSect)) continue;

			STL_COMP_CHK_POSD_SNiP DgnSubPosD;

			int nStype = StlCompSect.SectD->SectInfo.nStype;
			int nPosIJ = 0;
			if (ConvertStlCompSectCheckPart(nStype, nPosIJ, SddpD, DgnSubPosD))  m_mStlCompDgnPosD_I.SetAt(SddpK, DgnSubPosD);

			if (SddpD.bSameIJ)  m_mStlCompDgnPosD_J.SetAt(SddpK, DgnSubPosD);
			else
			{
				nPosIJ = 1; DgnSubPosD.Initialize();
				if (ConvertStlCompSectCheckPart(nStype, nPosIJ, SddpD, DgnSubPosD))  m_mStlCompDgnPosD_J.SetAt(SddpK, DgnSubPosD);
			}
		}
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Make_ShearConnectorGroupData()
{
	CArray<T_SCGD_K, T_SCGD_K> aScgdKeyList;
	m_pDoc->m_pAttrCtrl2->GetScgdKeyList(aScgdKeyList);

	INT_PTR nGroupNum = aScgdKeyList.GetSize();
	if (nGroupNum == 0) return TRUE;

	for (INT_PTR nGoup = 0; nGoup < nGroupNum; ++nGoup)
	{
		T_SCGD_K ScgdK = aScgdKeyList[nGoup];
		T_SCGD_D ScgdD;
		if (!m_pDoc->m_pAttrCtrl2->GetScgd(ScgdK, ScgdD)) continue;

		STL_COMP_SHEAR_CONN_INPUT_DATA_SNiP ShearConnD; ShearConnD.Initialize();
		if (!Make_ShearConnectorGroupData_Unit(ScgdD, ShearConnD)) continue;

		m_mStlCompShearConnectorD.SetAt(ScgdK, ShearConnD);
	}

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::Make_ShearConnectorGroupData_Unit(const T_SCGD_D& crScgdD, STL_COMP_SHEAR_CONN_INPUT_DATA_SNiP& rData)
{
	INT_PTR nElemSize = crScgdD.arGroupElemKey.GetSize();
	if (nElemSize < 1) return FALSE;

	CMap<T_SECT_K, T_SECT_K&, T_SECT_K, T_SECT_K&> mTempSectK;
	CMap<T_SECT_K, T_SECT_K&, T_ELEM_K, T_ELEM_K&> mTempSect2ElemK;
	T_MATL_K MatlK = 0;
	for (INT_PTR nElem = 0; nElem < nElemSize; ++nElem)
	{
		T_ELEM_K ElemK = crScgdD.arGroupElemKey[nElem];
		T_ELEM_D ElemD;
		if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD)) return FALSE;

		T_SECT_K SectK = ElemD.elpro;
		MatlK = ElemD.elmat;
		if (!rData.mElemtoSectKey.Lookup(ElemK, SectK))
		{
			rData.mElemtoSectKey.SetAt(ElemK, SectK);
		}
		T_SECT_K DummySectK;
		if (!mTempSectK.Lookup(SectK, DummySectK))
		{
			mTempSectK.SetAt(SectK, SectK);
			mTempSect2ElemK.SetAt(SectK, ElemK);
		}
	}

	T_CPGD_D CpgdD;
	if (!m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD)) ASSERT(0);

	double dConcContributeF_1stG = CpgdD.dPsi_1stGroup;
	double dConcContributeF_2ndG = CpgdD.dPsi_2ndGroup;

	if (mTempSectK.IsEmpty()) return FALSE;
	rData.mSectD.RemoveAll();
	POSITION pos = mTempSectK.GetStartPosition();
	while (pos)
	{
		T_SECT_K SectKey = 0;
		T_SECT_K SectKeyD = 0;
		mTempSectK.GetNextAssoc(pos, SectKey, SectKeyD);
		T_ELEM_K ElemK;
		mTempSect2ElemK.Lookup(SectKeyD, ElemK);

		STL_COMP_SECT_ELEMD_SNiP SectElem;
		if (!rData.mSectD.Lookup(SectKeyD, SectElem))
		{
			T_SECT_D SectD;
			if (!m_pDoc->m_pPostCtrl->GetSectPost(SectKeyD, SectD)) return FALSE;
			Make_ConvertSectD4ShearConnGroup(SectKeyD, ElemK, SectElem, dConcContributeF_1stG, dConcContributeF_2ndG);
			rData.mSectD.SetAt(SectKeyD, SectElem);
		}
	}

	// Material Data Convert
	T_MATD_D MatlD;
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, MatlD)) { ASSERT(0); return FALSE; }
	if (!Convert_Stl2StlCompMatl(MatlD, rData.MatD)) { ASSERT(0); return FALSE; }

	// Converting Shear Connector Group Data
	ConvertStlCompShearConnector(crScgdD, rData);

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Make_ConvertSectD4ShearConnGroup(T_SECT_K SectK, T_ELEM_K ElemK, STL_COMP_SECT_ELEMD_SNiP& rData, double dCCF_1st, double dCCF_2nd)
{
	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	T_TSGR_K TsgrK = m_pDoc->m_pAttrCtrl->GetElemTsgrKey(ElemK);

	T_SECT_D SectD;
	T_STL_COMP_SECT_STFN_INX  SectStfnId;
	double dConcreteContributeF_1stG = 0.0;
	double dConcreteContributeF_2ndG = 0.0;

	T_ELEM_D ElemD, ElemPreD;
	BOOL bGetElem = m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD);
	BOOL bGetElemPre = m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemPreD);
	if (!bGetElem || !bGetElemPre) ASSERT(0);

	T_SECT_K SectGetK = ElemD.elpro;
	if (SectGetK != SectK) ASSERT(0);
	T_SECT_K SectPreK = ElemPreD.elpro;

	if (TsgrK > 0)
	{
		T_SECT_D TSectD;
		m_pDoc->m_pAttrCtrl->GetSect(SectK, TSectD);

		T_TSGR_D TsgrD;
		double dTotalLength = 0.0;
		double dPositionI = 0.0;
		double dPositionJ = 0.0;
		if (m_pDoc->m_pAttrCtrl->GetTsgr(TsgrK, TsgrD))
		{
			double dTsgrTotLength = 0.0;
			m_pDoc->m_pAttrCtrl->CalcTsgrTotLength(TsgrD, ElemK, dTsgrTotLength, dPositionI);
			//       int nTsgrElemList = TsgrD.aElemList.GetSize();
			//       for(int i=0; i<nTsgrElemList; ++i)
			//       {
			//         if(TsgrD.aElemList[i]==ElemK) dPositionI = dTsgrTotLength;
			//         dTsgrTotLength += m_pDoc->calcLAVElem(TsgrD.aElemList[i]);
			//       }
			dPositionJ = dPositionI + m_pDoc->calcLAVElem(ElemD);

			m_pDoc->m_pSectDB->CalcTaperedSection(TSectD, TsgrD, dTsgrTotLength, dPositionI, dPositionJ, 0, SectD);
		}
	}
	else
	{
		m_pDoc->m_pPostCtrl->GetSectPost(SectK, SectD);
	}

	T_STPO_D StpoD;
	CSectUtil::GetStressPointSOD_Stiffener(SectD, StpoD); /// ???

	BOOL bTapered = SectD.nStype == D_SECT_TYPE_TAPERED ? TRUE : FALSE;
	int nSectType = m_pDoc->m_pAttrCtrl->GetRealSectType(SectK);

	for (int j = 0; j < 2; ++j) //  I & J
	{
		BOOL bPosiI = (j == 0);
		//if (!bTapered && j==1) continue;
		T_SECT_SECTBASE_D &SectBaseD = (!bPosiI && bTapered) ? SectD.CmpTapJ : SectD.SectBefore.SectI;

		STL_COMP_SECT_POSD &rStlCompSectPos = rData.SectD[j];
		rStlCompSectPos.SectInfo.bIsTapered = bTapered;

		T_SECT_STIFFNESS SpscSect[5]; // 0: Pure Sect, 1 : Positive Moment Stiffness(1stG),  2 : Negative Moment Stiffness(1stG), 1 : Positive Moment Stiffness(2ndG),  2 : Negative Moment Stiffness(2ndG)
		for (int k = 0; k < 5; ++k) SpscSect[k].Initialize();

		BOOL bSecf = FALSE;
		BOOL bPostMode = TRUE;
		BOOL bCrack = FALSE;
		int nRebarCalcMethod = 0;
		TCHAR chIJ = bPosiI ? 'I' : 'J';
		SpscSect[0] = *CSectUtil::GetProperStiffPosition(SectD, TRUE, 0, chIJ);
		bCrack = FALSE;  nRebarCalcMethod = 0;
		BOOL bStiffn1stG_Posi = CSectUtil::GetCompoStlGirderSectCR(bPostMode, bCrack, nRebarCalcMethod, dCCF_1st, EPairK, bPosiI, bSecf, SpscSect[1]);
		bCrack = TRUE;  nRebarCalcMethod = 0;
		BOOL bStiffn1stG_Nega = CSectUtil::GetCompoStlGirderSectCR(bPostMode, bCrack, nRebarCalcMethod, dCCF_1st, EPairK, bPosiI, bSecf, SpscSect[2]);
		bCrack = FALSE;  nRebarCalcMethod = 0;
		BOOL bStiffn2ndG_Posi = CSectUtil::GetCompoStlGirderSectCR(bPostMode, bCrack, nRebarCalcMethod, dCCF_2nd, EPairK, bPosiI, bSecf, SpscSect[3]);
		bCrack = TRUE;  nRebarCalcMethod = 0;
		BOOL bStiffn2ndG_Nega = CSectUtil::GetCompoStlGirderSectCR(bPostMode, bCrack, nRebarCalcMethod, dCCF_2nd, EPairK, bPosiI, bSecf, SpscSect[4]);
		BOOL bCalcOK = bStiffn1stG_Posi && bStiffn1stG_Nega && bStiffn2ndG_Posi && bStiffn2ndG_Nega;

		if (bCalcOK)
		{
			Convert_StlComp_SectData(j, &SectD, StpoD, SectStfnId.Stiff[j], SpscSect, rData.SectD[j]);

			T_SECT_STIFF_D SectStiff;
			SectStiff.aStiffShape.Copy(SectBaseD.SODStiffener.aStiffShape);
			CalcStiffnessOfStiffener(SectStiff, rData.SectD[j].SectInfo.BeforeCompSect.Stiffener);
			CalcStiffnessOfStiffener(SectStiff, rData.SectD[j].SectInfo.AfterCompSect.Stiffener);
		}
		Get_RebarData(ElemK, bPosiI, SectD, rStlCompSectPos);
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Make_ShearConnGroupForceD(const STL_COMP_SHEAR_CONN_INPUT_DATA_SNiP& crShearConnInD, CArray<int, int>& aDgnLcomList)
{
	m_mShearConnGroupForceD.RemoveAll();
	aDgnLcomList.RemoveAll();

	int nOrgLcomNo = 0;
	int nSerLcomCount = 0;
	int nMaxMinType = 0;
	CString strOrgLcomNa = _T("");
	bool bExistCrp = false, bExistShr = false, bExistBST = false;

	int nLcomNum = m_pForcCtrl->Get_LcomDataCount();
	INT_PTR nGElemSize = crShearConnInD.arGroupElemKey.GetSize();

	CArray<ShearConnGForD, ShearConnGForD&> arElemForceD_I, arElemForceD_J;  // array 순서와 동일하게 ElemK를 저장
	arElemForceD_I.RemoveAll(); arElemForceD_J.RemoveAll();
	arElemForceD_I.SetSize(nGElemSize);
	arElemForceD_J.SetSize(nGElemSize);

	_DGN_FORC_CRC ForceNC; // Dummy, 전단 연결재 검토에는 Before Force 사용 안 함
	BOOL bFirst = TRUE;
	for (INT_PTR nElem = 0; nElem < nGElemSize; ++nElem)
	{
		T_ELEM_K ElemK = crShearConnInD.arGroupElemKey[nElem];
		ElemPairK EPairK(ElemK, EN_EL_BEAM);

		ADGNFORCE aForceLcom;
		ADGNFORCE aForceTnpr;
		ADGNFORCE aForceTnsc;
		ADGNFORCE aForceLTerm;
		ADGNFORCE aForceSTerm;
		ADGNSHRSADD aStssLcom;
		if (!m_pForcCtrl->Get_LcomDataForDesign(EPairK, TRUE, TRUE, aForceLcom, aForceTnpr, aForceTnsc, aForceLTerm, aForceSTerm, aStssLcom, nLcomNum)) { ASSERT(0); continue; }

		ADGNFORCE aForceCreep, aForceShrinkage;
		m_pForcCtrl->Get_LcomForce4ScfpCrp(EPairK, nLcomNum, aForceCreep);
		m_pForcCtrl->Get_LcomForce4ScfpShr(EPairK, nLcomNum, aForceShrinkage);

		T_LCOM_K_LIST  aStrnLcomKList;
		Get_DgnLcomKeyList4ShearConnDesign(aForceLcom, aStrnLcomKList);  // Get DgnLcomK

		ASCFPSTRESS aScfpCr;
		ASCFPSTRESS aScfpSh;
		ASCFPSTRESS aScfpTe;
		ASCFPSTRESS aScfpRe;
		if (!m_pForcCtrl->Get_LcomStressScfp(EPairK, nLcomNum, aScfpCr, aScfpSh, aScfpTe, aScfpRe)) { ASSERT(0); continue; }  // Get Internal Stress

		ShearConnGForD ForceListD4Elem_I, ForceListD4Elem_J;

		INT_PTR nChkLcomSize = aStrnLcomKList.GetSize();
		for (INT_PTR nLcom = 0; nLcom < nChkLcomSize; ++nLcom)
		{
			int nDgnLcomIndex = aStrnLcomKList[nLcom] - 1;
			int nDgnLcomK = aStrnLcomKList[nLcom];
			_DGN_FORC_CRC& GForceD = aForceLcom[nDgnLcomIndex];
			_DGN_FORC_CRC& ForD_After_DL = aForceLTerm[nDgnLcomIndex];
			_DGN_FORC_CRC ForD_Crp = aForceCreep[nDgnLcomIndex];
			_DGN_FORC_CRC ForD_Shr = aForceShrinkage[nDgnLcomIndex];
			_DGN_FORC_CRC ForD_BST; //  ??? 요건 찾아야 함..
			_DGN_SCFP_PART_LCASE& ScfpCr = aScfpCr[nDgnLcomIndex];
			_DGN_SCFP_PART_LCASE& ScfpSh = aScfpSh[nDgnLcomIndex];
			_DGN_SCFP_PART_LCASE& ScfpTe = aScfpTe[nDgnLcomIndex];

			if (!m_pForcCtrl->Get_LcomType(nDgnLcomK, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)) { ASSERT(0); continue; }
			IsExistScfp4Lcom(D_LCOMTYPE_STLCOMP, nOrgLcomNo, bExistCrp, bExistShr, bExistBST);
			if (!bExistCrp) ScfpCr.Initialize();
			if (!bExistShr) ScfpSh.Initialize();
			if (!bExistBST) ScfpTe.Initialize();

			STL_COMP_FORCE_D ForD_I, ForD_J;
			ConvertStlCompCalcForce(0, ForceNC, GForceD, ForD_After_DL, ForD_Crp, ForD_Shr, ForD_BST, ScfpCr, ScfpSh, ScfpTe, ForD_I);
			ConvertStlCompCalcForce(1, ForceNC, GForceD, ForD_After_DL, ForD_Crp, ForD_Shr, ForD_BST, ScfpCr, ScfpSh, ScfpTe, ForD_J);
			if (!ForceListD4Elem_I.For4ElemD.Lookup(nDgnLcomK, ForD_I)) { ForceListD4Elem_I.For4ElemD.SetAt(nDgnLcomK, ForD_I); }
			if (!ForceListD4Elem_J.For4ElemD.Lookup(nDgnLcomK, ForD_J)) { ForceListD4Elem_J.For4ElemD.SetAt(nDgnLcomK, ForD_J); }

			if (bFirst) aDgnLcomList.Add(nDgnLcomK);
		}
		bFirst = FALSE;
		arElemForceD_I.SetAt(nElem, ForceListD4Elem_I);
		arElemForceD_J.SetAt(nElem, ForceListD4Elem_J);
	}

	// Convert
	INT_PTR nSize = aDgnLcomList.GetSize();
	for (INT_PTR i = 0; i < nSize; ++i)
	{
		int nDgnLcomK = aDgnLcomList[i];

		SHEAR_CONN_GROUP_FORCE_SNIP GroupForceD;
		for (UINT nElem = 0; nElem < nGElemSize; ++nElem)
		{
			T_ELEM_K ElemK = crShearConnInD.arGroupElemKey[nElem];
			SHEAR_CONN_GROUP_FORCE_UNIT_SNIP ForD;
			arElemForceD_I[nElem].For4ElemD.Lookup(nDgnLcomK, ForD.ForceD[0]);
			arElemForceD_J[nElem].For4ElemD.Lookup(nDgnLcomK, ForD.ForceD[1]);
			GroupForceD.mGroupForceD.SetAt(ElemK, ForD);
		}
		m_mShearConnGroupForceD.SetAt(nDgnLcomK, GroupForceD);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////////

BOOL CDgnRUSStlCompDataCtrl::GetDeadLoadCSMaxFactor(int &nLcomK, double &dFactor)
{
	INT_PTR nLcomSize = m_pForcCtrl->Get_LcomDataCount();
	INT_PTR nChkLcomCount = 0;

	double dMaxFactor = 0.0;
	int    nChkLcomK = -1;
	for (INT_PTR i = 0; i < nLcomSize; ++i)
	{
		INT_PTR nDgnLcomNo = i + 1;
		_DGN_LCOM DgnLcom;
		if (m_pForcCtrl->Get_DgnLcomData(nDgnLcomNo, DgnLcom))
		{
			BOOL bULSLcom = m_pForcCtrl->Is_LcomForStrn(nDgnLcomNo);
			if (bULSLcom == TRUE)
			{
				nChkLcomCount++;
				if (nChkLcomCount == 1)
				{
					nChkLcomK = DgnLcom.OriginalLcomKey;
					dMaxFactor = DgnLcom.dFactorDeadCS;
				}
				else
				{
					if (dMaxFactor < DgnLcom.dFactorDeadCS)
					{
						nChkLcomK = DgnLcom.OriginalLcomKey;
						dMaxFactor = DgnLcom.dFactorDeadCS;
					}
				}
			}
		}
	}

	nLcomK = nChkLcomK;
	dFactor = dMaxFactor;
	return (nLcomK == -1) ? FALSE : TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_DgnLcomKeyList4ShearConnDesign(const ADGNFORCE& craDgnForceLcom, T_LCOM_K_LIST& aStrnLcomKList)
{
	INT_PTR nLcomSize = craDgnForceLcom.GetSize();
	if (nLcomSize < 1) return FALSE;
	aStrnLcomKList.RemoveAll();

	int nDgnLcomNo = 0;
	int nOrgLcomNo = 0;
	int nSerLcomCount = 0;
	int nMaxMinType = 0;
	CString strOrgLcomNa = _T("");

	T_LCMT_D LcmtD;
	m_pDoc->m_pAttrCtrl2->GetLcmt(LcmtD);

	int nFatiLcomBF = -1;
	BOOL bFirst = TRUE;
	T_FATI_LCOM_K_LIST FatiUnit;

	for (int nLcom = 0; nLcom < nLcomSize; ++nLcom)
	{
		nDgnLcomNo = nLcom + 1;
		if (!m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)) { ASSERT(0); continue; }

		T_LCOM_K LcomK = nOrgLcomNo;
		if (IsLcomExist(LcomK, LcmtD.Base[2].aStrength)) aStrnLcomKList.Add(nDgnLcomNo);
	}

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::Get_DgnLcomKeyList4StlCompDesign(const ADGNFORCE& craDgnForceLcom, T_LCOM_K_LIST& aKapaChkLcomKList, T_LCOM_K_LIST& aStrnLcomKList, T_LCOM_K_LIST& aFatigueOrtLcomKList,
	CArray<T_FATI_LCOM_K_LIST, T_FATI_LCOM_K_LIST&>& aFatigueLcomKList, T_LCOM_K_LIST& aServLcomKeyList)
{
	INT_PTR nLcomSize = craDgnForceLcom.GetSize();
	if (nLcomSize < 1) return FALSE;
	aKapaChkLcomKList.RemoveAll();
	aStrnLcomKList.RemoveAll();
	aFatigueOrtLcomKList.RemoveAll();
	aFatigueLcomKList.RemoveAll();
	aServLcomKeyList.RemoveAll();

	int nDgnLcomNo = 0;
	int nOrgLcomNo = 0;
	int nSerLcomCount = 0;
	int nMaxMinType = 0;
	CString strOrgLcomNa = _T("");

	T_LCMT_D LcmtD;
	m_pDoc->m_pAttrCtrl2->GetLcmt(LcmtD);

	int nFatiLcomBF = -1;
	BOOL bFirst = TRUE;
	T_FATI_LCOM_K_LIST FatiUnit;
	CMap<T_LCOM_K, T_LCOM_K, T_FATI_LCOM_K_LIST, T_FATI_LCOM_K_LIST> mFatiGlLcomK; mFatiGlLcomK.RemoveAll();

	for (int nLcom = 0; nLcom < nLcomSize; ++nLcom)
	{
		nDgnLcomNo = nLcom + 1;
		if (!m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)) { ASSERT(0); continue; }

		T_LCOM_K LcomK = nOrgLcomNo;
		aKapaChkLcomKList.Add(nDgnLcomNo);
		if (IsLcomExist(LcomK, LcmtD.Base[2].aStrength)) aStrnLcomKList.Add(nDgnLcomNo);
		if (IsLcomExist(LcomK, LcmtD.Base[2].aFatigue))
		{
			T_FATI_LCOM_K_LIST *pFatiLcomList = &FatiUnit;

			if (mFatiGlLcomK.Lookup(nOrgLcomNo, *pFatiLcomList))
			{
				pFatiLcomList->aFatiList.Add(nDgnLcomNo);
				mFatiGlLcomK.SetAt(nOrgLcomNo, *pFatiLcomList);
			}
			else if (!mFatiGlLcomK.Lookup(nOrgLcomNo, *pFatiLcomList))
			{
				pFatiLcomList->aFatiList.RemoveAll();
				pFatiLcomList->aFatiList.Add(nDgnLcomNo);
				mFatiGlLcomK.SetAt(nOrgLcomNo, *pFatiLcomList);
			}
		}
		T_LCOM_D LcomData; LcomData.Initialize();
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, nOrgLcomNo, LcomData);
		if (LcomData.nActive == 2) { aServLcomKeyList.Add(nDgnLcomNo); }
	}

	// Convert Fatigue LcomD
	POSITION pos = mFatiGlLcomK.GetStartPosition();
	while (pos)
	{
		T_LCOM_K FatiGK = 0;
		T_FATI_LCOM_K_LIST FatiLcomList;
		mFatiGlLcomK.GetNextAssoc(pos, FatiGK, FatiLcomList);
		aFatigueOrtLcomKList.Add(FatiGK);
		aFatigueLcomKList.Add(FatiLcomList);
	}

	return FALSE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_LocalForceD(T_ELEM_K ElemK, int nIJ, int nLcomK, STL_COMP_LFORCE_SNIP &rLForce, BOOL bIsAfter)
{
	if (nIJ != 0 && nIJ != 1) return FALSE;
	BOOL bIJ = nIJ == 0 ? TRUE : FALSE;
	STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM LForD_Sigy;
	STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM  LForD_Plate;
	BOOL bFind_Sigy = FALSE;
	BOOL bFind_Plate = FALSE;
	int nUsedLcomK = bIsAfter ? nLcomK : 0; // Before ComposIte의 경우, CS step에 상관없이  무조건 하나이므로, LcomK는 0으로 통일
	if (!bIsAfter) // Before composite local Force
	{
		if (bIJ)
		{
			bFind_Sigy = m_mStlCompLForWeb_I[0].Lookup(ElemK, LForD_Sigy);
			bFind_Plate = m_mStlCompLForWebPlate_I[0].Lookup(ElemK, LForD_Plate);
		}
		else
		{
			bFind_Sigy = m_mStlCompLForWeb_J[0].Lookup(ElemK, LForD_Sigy);
			bFind_Plate = m_mStlCompLForWebPlate_J[0].Lookup(ElemK, LForD_Plate);
		}
	}
	else  // After composite local Force
	{
		if (bIJ)
		{
			bFind_Sigy = m_mStlCompLForWeb_I[1].Lookup(ElemK, LForD_Sigy);
			bFind_Plate = m_mStlCompLForWebPlate_I[1].Lookup(ElemK, LForD_Plate);
		}
		else
		{
			bFind_Sigy = m_mStlCompLForWeb_J[1].Lookup(ElemK, LForD_Sigy);
			bFind_Plate = m_mStlCompLForWebPlate_J[1].Lookup(ElemK, LForD_Plate);
		}
	}

	if (bFind_Sigy) LForD_Sigy.mWebSigYFor.Lookup(nLcomK, rLForce.WebSigy_LForD);
	if (bFind_Plate) LForD_Plate.mWebPlateFor.Lookup(nLcomK, rLForce.WebPl_LForD);

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_RebarData(T_ELEM_K ElemK, BOOL bPosIi, T_SECT_D& SectD, STL_COMP_SECT_POSD& rData)
{
	rData.Rbar.arRbarPosi.RemoveAll();
	CPG_RBAR_ELEM RbarElem;
	RbarElem.Initialize();

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	INT_PTR nIJ = bPosIi ? 0 : 1;

	for (int k = 0; k < 2; ++k) // 0=비균열, 1=균열
	{
		BOOL bCrack = k == 0 ? TRUE : FALSE;
		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
		if (!m_pDataCtrl->Get_CpgRbar4Dgn(EPairK, bPosIi, bCrack, arRbarPosi))	continue;
		// Set Rbar (0=I, 1=J).
		if (k == 0) RbarElem.RbarPosi[nIJ].arRbarPosi.Copy(arRbarPosi);
		else     RbarElem.RbarPosiCR[nIJ].arRbarPosi.Copy(arRbarPosi);
	}

	INT_PTR nRbarSize = RbarElem.RbarPosi[nIJ].arRbarPosi.GetSize();
	rData.Rbar.arRbarPosi.SetSize(nRbarSize);
	for (INT_PTR i = 0; i < nRbarSize; ++i)
	{
		Convert_RebarData(RbarElem.RbarPosi[nIJ].arRbarPosi[i], rData.Rbar.arRbarPosi[i]);
	}

	return TRUE;
}

void CDgnRUSStlCompDataCtrl::MakeSectStfnIndexInfo(int nStype, T_SECT_SECTBASE_D *pSectD, T_STL_COMP_STFN_INX_D &rData)
{
	int nStiffSubSize = pSectD->SODStiffener.aSODStiffSub.GetSize();

	CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> aStiffData;
	for (int i = 0; i < nStiffSubSize; ++i)
	{
		T_SECT_SOD_STIFF_SUB_D &PartStfnD = pSectD->SODStiffener.aSODStiffSub[i];

		MakeSectStfnPropileIndex(nStype, pSectD, PartStfnD, rData);
	}
}

void CDgnRUSStlCompDataCtrl::MakeSectStfnPropileIndex(int nStype, T_SECT_SECTBASE_D *pSectD, T_SECT_SOD_STIFF_SUB_D &PartStfn, T_STL_COMP_STFN_INX_D &rData)
{
	if (PartStfn.nDeckPart < 0 || PartStfn.nDeckPart>2) return;

	int nLStfnSize = PartStfn.aStiffData.GetSize();
	if (nLStfnSize <= 0) return;

	T_STL_COMP_STFN_INX_MAP *pStfnInxMap = NULL;

	BOOL   bReCalSpace = IsReplaceStfnIndex(nStype, PartStfn.nRefPos, PartStfn.nDeckPos, PartStfn.nDeckPart);
	if (nStype == D_SECT_TYPE_COMPO_STLG_B || nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		if (PartStfn.nDeckPos == 0) // Top
		{
			if (PartStfn.nDeckPart == 0) // Left Outstand
			{
				pStfnInxMap = &rData.StfnTL;
			}
			else if (PartStfn.nDeckPart == 1) // Internal
			{
				pStfnInxMap = &rData.StfnTM;
			}
			else if (PartStfn.nDeckPart == 2) // Right Outstand
			{
				pStfnInxMap = &rData.StfnTR;
			}
			else ASSERT(0);
		}
		else if (PartStfn.nDeckPos == 1) // web.L
		{
			pStfnInxMap = &rData.StfnWL;
		}
		else if (PartStfn.nDeckPos == 2) // web.R
		{
			pStfnInxMap = &rData.StfnWR;
		}
		else if (PartStfn.nDeckPos == 3) // Bottom
		{
			if (PartStfn.nDeckPart == 0) // Left Outstand
			{
				pStfnInxMap = &rData.StfnBL;
			}
			else if (PartStfn.nDeckPart == 1) // Internal
			{
				pStfnInxMap = &rData.StfnBM;
			}
			else if (PartStfn.nDeckPart == 2) // Right Outstand
			{
				pStfnInxMap = &rData.StfnBR;
			}
			else ASSERT(0);
		}
		else ASSERT(0);
	}
	else if (nStype == D_SECT_TYPE_COMPO_STLG_I)
	{
		if (PartStfn.nDeckPos == 0) // Top
		{
			if (PartStfn.nDeckPart == 0) // Left Outstand
			{
				pStfnInxMap = &rData.StfnTL;
			}
			else if (PartStfn.nDeckPart == 1) // Right Outstand
			{
				pStfnInxMap = &rData.StfnTR;
			}
			else ASSERT(0);
		}
		else if (PartStfn.nDeckPos == 1) // web
		{
			pStfnInxMap = &rData.StfnWL;
		}
		else if (PartStfn.nDeckPos == 2) // Bottom
		{
			if (PartStfn.nDeckPart == 0) // Left Outstand
			{
				pStfnInxMap = &rData.StfnBL;
			}
			else if (PartStfn.nDeckPart == 1) // Right Outstand
			{
				pStfnInxMap = &rData.StfnBR;
			}
			else ASSERT(0);
		}
		else ASSERT(0);
	}
	else ASSERT(0);

	CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> aLStiffD;
	aLStiffD.Copy(PartStfn.aStiffData);

	for (int i = 0; i < nLStfnSize; ++i)
	{
		int nId = (bReCalSpace == TRUE) ? nLStfnSize - 1 - i : i;
		pStfnInxMap->mStfnIdx.SetAt(aLStiffD[i].strStiffName, nId);
	}
}

BOOL CDgnRUSStlCompDataCtrl::Get_StlCompUlmSection(T_ELEM_K ElemI, T_ELEM_K ElemM, T_ELEM_K ElemJ, int nPosI, int nPosM, int nPosJ, STL_COMP_ULMS_D &rData)
{
	if (nPosI < 0 || nPosI>1) return FALSE;
	if (nPosM < 0 || nPosM>1) return FALSE; // M은 0,1,2,3,4 가능 
	if (nPosJ < 0 || nPosJ>1) return FALSE;

	T_ELEM_D ElemD; ElemD.Initialize();
	m_pDoc->m_pPostCtrl->GetElemPost(ElemI, ElemD);

	STL_COMP_SECT_ELEMD_SNiP StlCompSect; // I
	if (m_mStlCompSect.Lookup(ElemD.elpro, StlCompSect))
	{
		rData.SectD[0] = StlCompSect.SectD[nPosI];
	}

	ElemD.Initialize(); // J
	m_pDoc->m_pPostCtrl->GetElemPost(ElemJ, ElemD);

	if (m_mStlCompSect.Lookup(ElemD.elpro, StlCompSect))
	{
		rData.SectD[2] = StlCompSect.SectD[nPosJ];
	}

	ElemD.Initialize(); // M,  
	m_pDoc->m_pPostCtrl->GetElemPost(ElemM, ElemD);

	if (m_mStlCompSect.Lookup(ElemD.elpro, StlCompSect))
	{
		rData.SectD[1] = StlCompSect.SectD[nPosM];
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetDgnBaseD4Elem_IJ(T_ELEM_K ElemK, const T_ELEM_D& crElemD, int nPosIJ, STL_COMP_MEMB_POSD_SNiP& rData)
{
	if (nPosIJ > 1 || nPosIJ < 0) return FALSE;

	STL_COMP_SECT_ELEMD_SNiP SectElemD;
	if (!m_mStlCompSect.Lookup(crElemD.elpro, SectElemD)) return FALSE; // Section 정보가 없는 경우는 return FALSE
	STL_COMP_MAT_SNiP MatlElemD;
	if (!m_mStlCompMatl2Sect.Lookup(ElemK, MatlElemD)) return FALSE; // Material 정보가 없는 경우는 return FALSE

	// Get Data 
	rData.Memb[nPosIJ].SectD = SectElemD.SectD[nPosIJ];
	rData.Memb[nPosIJ].MatD = MatlElemD;

	// Get Element Data & nIJ Data
	rData.nIJ = nPosIJ;
	rData.Memb[0].ChIf.nElem = ElemK;
	rData.Memb[1].ChIf.nElem = ElemK;
	rData.Memb[0].ChIf.nChkPos = 0;
	rData.Memb[1].ChIf.nChkPos = 1;

	STL_COMP_LIMIT_CRACK_STATE_INPUT_D CrackD;
	if (m_mStlCompLimitGroupParam.Lookup(ElemK, CrackD))
	{
		rData.Memb[nPosIJ].DesignD.CrackInD = CrackD;
	}
	else rData.Memb[nPosIJ].DesignD.CrackInD.Initialize();

	STL_KAPPA_INPUT_D KappaInD;
	STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD KappaPosD;
	STL_COMP_FATI_CHK_POS_DGND_SNiP FatiPosD;
	STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D FloorBeamD;
	STL_COMP_CHK_POSD_SNiP DgnSubPosD;

	switch (nPosIJ)
	{
	case 0: // I-End
	{
		if (m_mStlCompKappaFactor_I.Lookup(ElemK, KappaInD)) { ConvertStlCompKappaFactor(KappaInD, rData.Memb[nPosIJ].DesignD.KappaFChkInfo); }
		else rData.Memb[nPosIJ].DesignD.KappaFChkInfo.Initialize();

		if (m_mStlCompKappaChkPos_I.Lookup(ElemK, KappaPosD)) { rData.Memb[nPosIJ].DesignD.KappaFChkInfo.aKappaChkPosInfo.Copy(KappaPosD.aKappaChkPosInfo); }
		else rData.Memb[nPosIJ].DesignD.KappaFChkInfo.aKappaChkPosInfo.RemoveAll();

		if (m_mStlCompFatiChkPos_I.Lookup(ElemK, FatiPosD)) { rData.Memb[nPosIJ].DesignD.FatChkPosInfo = FatiPosD; }
		else rData.Memb[nPosIJ].DesignD.FatChkPosInfo.Initialize();

		if (m_mStlCompFloorBeam_I.Lookup(ElemK, FloorBeamD)) { rData.Memb[nPosIJ].DesignD.DgnStaD_FloorBDeckP = FloorBeamD; }
		else rData.Memb[nPosIJ].DesignD.DgnStaD_FloorBDeckP.Initialize();

		if (m_mStlCompDgnPosD_I.Lookup(ElemK, DgnSubPosD)) { rData.Memb[nPosIJ].ChkPosD = DgnSubPosD; }
		else rData.Memb[nPosIJ].ChkPosD.Initialize();

		break;
	}
	case 1: // I-End
	{
		if (m_mStlCompKappaFactor_J.Lookup(ElemK, KappaInD)) { ConvertStlCompKappaFactor(KappaInD, rData.Memb[nPosIJ].DesignD.KappaFChkInfo); }
		else rData.Memb[nPosIJ].DesignD.KappaFChkInfo.Initialize();

		if (m_mStlCompKappaChkPos_J.Lookup(ElemK, KappaPosD)) { rData.Memb[nPosIJ].DesignD.KappaFChkInfo.aKappaChkPosInfo.Copy(KappaPosD.aKappaChkPosInfo); }
		else rData.Memb[nPosIJ].DesignD.KappaFChkInfo.aKappaChkPosInfo.RemoveAll();

		if (m_mStlCompFatiChkPos_J.Lookup(ElemK, FatiPosD)) { rData.Memb[nPosIJ].DesignD.FatChkPosInfo = FatiPosD; }
		else rData.Memb[nPosIJ].DesignD.FatChkPosInfo.Initialize();

		if (m_mStlCompFloorBeam_J.Lookup(ElemK, FloorBeamD)) { rData.Memb[nPosIJ].DesignD.DgnStaD_FloorBDeckP = FloorBeamD; }
		else rData.Memb[nPosIJ].DesignD.DgnStaD_FloorBDeckP.Initialize();

		if (m_mStlCompDgnPosD_J.Lookup(ElemK, DgnSubPosD)) { rData.Memb[nPosIJ].ChkPosD = DgnSubPosD; }
		else rData.Memb[nPosIJ].ChkPosD.Initialize();

		break;
	}
	default: ASSERT(0);    break;
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetDgnLengD()
{
	m_mDgnLengD.RemoveAll();
	CDBLib::GetElemforStlCompGirderGirder(m_arElemK);
	int nElemSize = m_arElemK.GetSize();
	CDgnDataCtrl DgnCtrl;

	for (int i = 0; i < nElemSize; ++i)
	{
		T_ELEM_K nElemK = 0;
		T_SPAN_K nSpanK = 0;
		T_MEMB_K nMembK = 0;
		BOOL bUnLeng = FALSE, bEffLengF = FALSE;

		nElemK = m_arElemK[i];

		T_EDLK_D DgnLengD;
		T_MEMB_D MembD; MembD.Initialize();
		T_SPAN_D SpanD; SpanD.Initialize();
		STL_MEMB_ULEN UlenD; UlenD.Initialize();
		double dDummy = 0.0;

		if (Get_UnbracedLengthData(nElemK, UlenD, bUnLeng, bEffLengF))   // First. Find Design length Data from defined Unbraced Length and Effective buckling Coeff Info.
		{
			DgnLengD.bExistUbracedInfo = TRUE;

			if (bUnLeng)
			{
				DgnLengD.dDgnLengY = UlenD.dLy;
				DgnLengD.dDgnLengZ = UlenD.dLz;
			}
			else
			{
				double dElemLength = m_pDoc->calcLAVElem(nElemK);
				DgnLengD.dDgnLengY = dElemLength;
				DgnLengD.dDgnLengZ = dElemLength;
			}

			DgnLengD.dEffBuckCoeffY = UlenD.dKy;
			DgnLengD.dEffBuckCoeffZ = UlenD.dKz;

			if (bEffLengF || bUnLeng)
			{
				DgnLengD.nSupTypeY[0] = -1; // Not apply support data, just using effective length factor.
				DgnLengD.nSupTypeY[1] = -1;

				DgnLengD.nSupTypeZ[0] = -1;
				DgnLengD.nSupTypeZ[1] = -1;
			}
			else
			{
				BOOL bSupt = FALSE;
				if (!m_pDoc->m_pAttrCtrl2->IsElemSup(nElemK, bSupt)) { return FALSE; }
				if (bSupt)
				{
					BOOL bSupportNode[2] = { FALSE, };
					BOOL bFix[2][6] = { FALSE, };

					if (!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(nElemK, bSupportNode[0], bSupportNode[1], bFix[0], bFix[1])) { return FALSE; }

					for (int nEnd = 0; nEnd < 2; ++nEnd)
					{
						if ((bFix[nEnd][0] || bFix[nEnd][1]) && bFix[nEnd][4]) { DgnLengD.nSupTypeY[nEnd] = 0; }
						else if ((bFix[nEnd][0] || bFix[nEnd][1]) && !bFix[nEnd][4]) { DgnLengD.nSupTypeY[nEnd] = 1; }
						else { DgnLengD.nSupTypeY[nEnd] = 2; }

						if ((bFix[nEnd][0] || bFix[nEnd][2]) && bFix[nEnd][5]) { DgnLengD.nSupTypeZ[nEnd] = 0; }
						else if ((bFix[nEnd][0] || bFix[nEnd][2]) && !bFix[nEnd][5]) { DgnLengD.nSupTypeZ[nEnd] = 1; }
						else { DgnLengD.nSupTypeZ[nEnd] = 2; }
					}
				}
			}
		}
		else if (m_pDoc->m_pAttrCtrl->GetMembAssigned(nElemK, nMembK)) // Second. Find Design length Data from defined Memb Info.
		{
			if (m_pDoc->m_pAttrCtrl->GetMemb(nMembK, MembD))
			{
				DgnLengD.MembK = nMembK;
				DgnLengD.bExistMembInfo = TRUE;
				GetDgnLengDFromMemb(nElemK, MembD, DgnLengD);
			}
		}
		else if (m_pDoc->m_pAttrCtrl2->GetSpanAssigned(nElemK, nSpanK)) // Third. Find Design length Data from defined Span Info.
		{
			if (m_pDoc->m_pAttrCtrl->GetSpan(nSpanK, SpanD))
			{
				DgnLengD.SpanK = nSpanK;
				DgnLengD.bExistSpanInfo = TRUE;
				GetDgnLengDFromSpan(nElemK, SpanD, DgnLengD);
			}
		}
		else  // Fourth. No Input Data
		{
			double dElemLenght = m_pDoc->calcLAVElem(nElemK);
			DgnLengD.dDgnLengY = dElemLenght;
			DgnLengD.dDgnLengZ = dElemLenght;

			DgnLengD.dEffBuckCoeffY = 1.0;
			DgnLengD.dEffBuckCoeffZ = 1.0;
			BOOL bSupt = FALSE;
			if (!m_pDoc->m_pAttrCtrl2->IsElemSup(nElemK, bSupt)) { return FALSE; }
			if (bSupt)
			{
				BOOL bSupportNode[2] = { FALSE, };
				BOOL bFix[2][6] = { FALSE, };

				if (!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(nElemK, bSupportNode[0], bSupportNode[1], bFix[0], bFix[1])) { return FALSE; }

				for (int nEnd = 0; nEnd < 2; ++nEnd)
				{
					if ((bFix[nEnd][0] || bFix[nEnd][1]) && bFix[nEnd][4]) { DgnLengD.nSupTypeY[nEnd] = 0; }
					else if ((bFix[nEnd][0] || bFix[nEnd][1]) && !bFix[nEnd][4]) { DgnLengD.nSupTypeY[nEnd] = 1; }
					else { DgnLengD.nSupTypeY[nEnd] = 2; }

					if ((bFix[nEnd][0] || bFix[nEnd][2]) && bFix[nEnd][5]) { DgnLengD.nSupTypeZ[nEnd] = 0; }
					else if ((bFix[nEnd][0] || bFix[nEnd][2]) && !bFix[nEnd][5]) { DgnLengD.nSupTypeZ[nEnd] = 1; }
					else { DgnLengD.nSupTypeZ[nEnd] = 2; }
				}
			}
		}

		m_mDgnLengD.SetAt(nElemK, DgnLengD);
	}

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::GetDgnLengDFromMemb(T_ELEM_K ElemK, const T_MEMB_D &MembD, T_EDLK_D &DgnLengD)
{
	// Get Memb Total Length
	double dLength = 0.0;
	if (m_pDoc->m_pAttrCtrl2->GetMembTotalLength(MembD, dLength))
	{
		DgnLengD.dDgnLengY = dLength;
		DgnLengD.dDgnLengZ = dLength;
	}

	// Get Memb Support Type  
	int nMembElemSize = MembD.aElemList.GetSize();
	CArray<T_ELEM_K, T_ELEM_K> aMembElemK; aMembElemK.RemoveAll(); aMembElemK.Copy(MembD.aElemList);
	CArray<T_NODE_K, T_NODE_K> aEndMembNodeK;   aEndMembNodeK.RemoveAll(); aEndMembNodeK.SetSize(2);

	if (nMembElemSize <= 0) { return FALSE; }
	else if (nMembElemSize >= 1)
	{
		T_ELEM_K ElemIK = aMembElemK[0];
		T_ELEM_K ElemJk = aMembElemK[nMembElemSize - 1];

		T_ELEM_D ElemID, ElemJD; ElemID.Initialize(); ElemJD.Initialize();
		m_pDoc->m_pAttrCtrl->GetElem(ElemIK, ElemID);
		m_pDoc->m_pAttrCtrl->GetElem(ElemJk, ElemJD);

		aEndMembNodeK[0] = ElemID.elnod[0];
		aEndMembNodeK[1] = ElemJD.elnod[1];
	}

	if (aEndMembNodeK.GetSize() != 2) { return FALSE; }

	T_NODE_K nElemNodeK[2] = { aEndMembNodeK[0], aEndMembNodeK[1] };
	int nSupTypeY[2] = { 0, };  // 0 : Fix, 1 : Hinge, 2 : Free
	int nSupTypeZ[2] = { 0, };  // 0 : Fix, 1 : Hinge, 2 : Free

	for (int i = 0; i < 2; ++i)
	{
		BOOL bFix[6] = { FALSE };
		T_NODE_K nNode = nElemNodeK[i];
		m_pDoc->m_pAttrCtrl->IsDofConstraintForDesign(nNode, bFix, FALSE);

		if ((bFix[0] || bFix[1]) && bFix[4]) { nSupTypeY[i] = 0; } // ???
		else if ((bFix[0] || bFix[1]) && !bFix[4]) { nSupTypeY[i] = 1; } // ???
		else { nSupTypeY[i] = 2; } // ???

		if ((bFix[0] || bFix[2]) && bFix[5]) { nSupTypeZ[i] = 0; } // ???
		else if ((bFix[0] || bFix[2]) && !bFix[5]) { nSupTypeZ[i] = 1; } // ???
		else { nSupTypeZ[i] = 2; } // ???

	}
	DgnLengD.nSupTypeY[0] = nSupTypeY[0];
	DgnLengD.nSupTypeZ[0] = nSupTypeZ[0];
	DgnLengD.nSupTypeY[1] = nSupTypeY[1];
	DgnLengD.nSupTypeZ[1] = nSupTypeZ[1];


	// Find Location in Memb.
	int nDevideNum = 4;
	double dTagetLength = dLength / double(nDevideNum);
	double dSumLengI = 0.0;
	for (int k = 0; k < nMembElemSize; ++k)
	{
		double dLengthDel = m_pDoc->calcLAVElem(MembD.aElemList[k]);

		double dSumLengJ = dSumLengI + dLengthDel;

		if (MembD.aElemList[k] == ElemK)
		{
			if (dSumLengI < dTagetLength || dSumLengI >(dLength - dTagetLength)) { DgnLengD.nLoInfoInMemb[0] = 1; } // ???
			if (dSumLengI < dTagetLength || dSumLengJ >(dLength - dTagetLength)) { DgnLengD.nLoInfoInMemb[1] = 1; } // ???
			break;
		}

		dSumLengI += dLengthDel;
	}

	return TRUE;
}



BOOL CDgnRUSStlCompDataCtrl::GetDgnLengDFromSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, T_EDLK_D &DgnLengD)
{
	// Get Memb Total Length
	double dSpanLength = 0.0;
	dSpanLength += m_pDoc->calcLAVElem(ElemK);

	// Get Element Index in SpanD
	int nElemIndex = 0;
	int nElemNumInSpan = SPanD.aElemBase.GetSize();
	for (int i = 0; i < nElemNumInSpan; ++i)
	{
		T_ELEM_K TempElemK = SPanD.aElemBase[i].ElemK;
		if (ElemK == TempElemK)
		{
			nElemIndex = i;
			break;
		}
	}

	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
	T_NODE_K NodeK[2] = { (unsigned int)ElemD.elnod[0], (unsigned int)ElemD.elnod[1] }; // I & J

	BOOL bSupportNode[2] = { FALSE, }; // I & j
	BOOL bFix[2][6] = { FALSE, };        // I & j
	BOOL bSpanEndFix[2][6] = { FALSE, };        // In & De Direction

	if (!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(ElemK, bSupportNode[0], bSupportNode[1], bFix[0], bFix[1])) { return FALSE; }

	// i j 가 각각 인덱스가 커지는 방향인가? 아니면 작아지는 방향인가?
	BOOL bExpandDeIndex = TRUE, bExpandInIndex = TRUE;
	int nElemIndexMin = nElemIndex == 0 ? -1 : nElemIndex - 1;
	int nElemIndexMax = nElemIndex == nElemNumInSpan - 1 ? -1 : nElemIndex + 1;

	// Check Decrease Index direction
	if (nElemIndexMin != -1)
	{
		T_ELEM_K PreElemK = SPanD.aElemBase[nElemIndexMin].ElemK;
		T_ELEM_D PreElemD;
		m_pDoc->m_pAttrCtrl->GetElem(PreElemK, PreElemD);
		T_NODE_K PreNodeK[2] = { (unsigned int)PreElemD.elnod[0], (unsigned int)PreElemD.elnod[1] }; // I & J    

		if (PreNodeK[0] == NodeK[0] || PreNodeK[1] == NodeK[0]) { bExpandDeIndex = !bSupportNode[0]; } // Index decrease direction -> I direction
		else if (PreNodeK[0] == NodeK[1] || PreNodeK[1] == NodeK[1]) { bExpandDeIndex = !bSupportNode[1]; } // Index decrease direction -> J direction
		else { ASSERT(0); }
	}
	else
	{
		bExpandDeIndex = FALSE;
	}

	if (!bExpandDeIndex)
	{
		if (bSupportNode[0])
		{
			for (int a = 0; a < 6; ++a) { bSpanEndFix[0][a] = bFix[0][a]; }
		}
		else if (bSupportNode[1])
		{
			for (int a = 0; a < 6; ++a) { bSpanEndFix[0][a] = bFix[1][a]; }
		}
		else { ASSERT(0); }
	}

	// Check Increase Index direction
	if (nElemIndexMax != -1)
	{
		T_ELEM_K NextElemK = SPanD.aElemBase[nElemIndexMax].ElemK;
		T_ELEM_D NextElemD;
		m_pDoc->m_pAttrCtrl->GetElem(NextElemK, NextElemD);
		T_NODE_K NextNodeK[2] = { (unsigned int)NextElemD.elnod[0], (unsigned int)NextElemD.elnod[1] }; // I & J    

		if (NextNodeK[0] == NodeK[0] || NextNodeK[1] == NodeK[0]) { bExpandInIndex = !bSupportNode[0]; } // Index decrease direction -> I direction
		else if (NextNodeK[0] == NodeK[1] || NextNodeK[1] == NodeK[1]) { bExpandInIndex = !bSupportNode[1]; } // Index decrease direction -> J direction
		else { ASSERT(0); }

	}
	else
	{
		bExpandInIndex = FALSE;
	}

	if (!bExpandInIndex)
	{
		if (bSupportNode[0])
		{
			for (int a = 0; a < 6; ++a) { bSpanEndFix[1][a] = bFix[0][a]; }
		}
		else if (bSupportNode[1])
		{
			for (int a = 0; a < 6; ++a) { bSpanEndFix[1][a] = bFix[1][a]; }
		}
		else { ASSERT(0); }
	}

	int nElemIndexDe = 0, nElemIndexIn = 0;

	// for Decrease Index Direction
	if (bExpandDeIndex)
	{
		int nDeRepeatNum = nElemIndexMin;
		BOOL bDeFix[2] = { FALSE, };
		BOOL bSpanDeFix[2][6] = { FALSE, };

		for (int nDe = nElemIndexMin; nDe >= 0; --nDe)
		{
			T_ELEM_K Key = SPanD.aElemBase[nDe].ElemK;
			double dL = m_pDoc->calcLAVElem(Key);
			dSpanLength += dL;

			if (!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(Key, bDeFix[0], bDeFix[1], bSpanDeFix[0], bSpanDeFix[1])) { return FALSE; }

			if (bDeFix[0] || bDeFix[1])
			{
				if (bDeFix[0])
				{
					for (int a = 0; a < 6; ++a) { bSpanEndFix[0][a] = bSpanDeFix[0][a]; }
				}
				else if (bDeFix[1])
				{
					for (int a = 0; a < 6; ++a) { bSpanEndFix[0][a] = bSpanDeFix[1][a]; }
				}
				else { ASSERT(0); }
				nElemIndexDe = nDe;
				break;
			}
		}
	}
	else
	{
		nElemIndexDe = nElemIndex;
	}

	// for Increase Index Direction
	if (bExpandInIndex)
	{
		int nDeRepeatNum = nElemIndexMax;
		BOOL bInFix[2] = { FALSE, };
		BOOL bSpanInFix[2][6] = { FALSE, };

		for (int nIn = nElemIndexMax; nIn < nElemNumInSpan; ++nIn)
		{
			T_ELEM_K Key = SPanD.aElemBase[nIn].ElemK;
			double dL = m_pDoc->calcLAVElem(Key);
			dSpanLength += dL;

			BOOL bElemSup = FALSE;
			if (!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(Key, bInFix[0], bInFix[1], bSpanInFix[0], bSpanInFix[1])) { return FALSE; }

			if (bInFix[0] || bInFix[1])
			{
				if (bInFix[0])
				{
					for (int a = 0; a < 6; ++a) { bSpanEndFix[1][a] = bSpanInFix[0][a]; }
				}
				else if (bInFix[1])
				{
					for (int a = 0; a < 6; ++a) { bSpanEndFix[1][a] = bSpanInFix[1][a]; }
				}
				else { ASSERT(0); }
				nElemIndexIn = nIn;
				break;
			}
		}
	}
	else
	{
		nElemIndexIn = nElemIndex;
	}

	DgnLengD.dDgnLengY = dSpanLength;
	DgnLengD.dDgnLengZ = dSpanLength;

	// Get Memb Support Type  
	int nSupTypeY[2] = { 0, };  // 0 : Fix, 1 : Hinge, 2 : Free
	int nSupTypeZ[2] = { 0, };  // 0 : Fix, 1 : Hinge, 2 : Free

	for (int nEnd = 0; nEnd < 2; ++nEnd)
	{
		if ((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][1]) && bSpanEndFix[nEnd][4]) { nSupTypeY[nEnd] = 0; } // ???
		else if ((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][1]) && !bSpanEndFix[nEnd][4]) { nSupTypeY[nEnd] = 1; } // ???
		else { nSupTypeY[nEnd] = 2; } // ???

		if ((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][2]) && bSpanEndFix[nEnd][5]) { nSupTypeZ[nEnd] = 0; } // ???
		else if ((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][2]) && !bSpanEndFix[nEnd][5]) { nSupTypeZ[nEnd] = 1; } // ???
		else { nSupTypeZ[nEnd] = 2; } // ???    
	}

	DgnLengD.nSupTypeY[0] = nSupTypeY[0];
	DgnLengD.nSupTypeZ[0] = nSupTypeZ[0];
	DgnLengD.nSupTypeY[1] = nSupTypeY[1];
	DgnLengD.nSupTypeZ[1] = nSupTypeZ[1];



	// Find Location in Memb.
	int nDevideNum = 4;
	double dChkLength = 0.0;
	double dTagetLength = dSpanLength / double(nDevideNum);


	double dSumLengI = 0.0;
	for (int n = nElemIndexDe; n < nElemIndexIn + 1; ++n)
	{
		double dLengthDel = m_pDoc->calcLAVElem(SPanD.aElemBase[n].ElemK);
		double dSumLengJ = dSumLengI + dLengthDel;
		if (n == nElemIndex)
		{
			if (dSumLengI < dTagetLength || dSumLengI >(dSpanLength - dTagetLength)) { DgnLengD.nLoInfoInMemb[0] = 1; } // ???
			if (dSumLengI < dTagetLength || dSumLengJ >(dSpanLength - dTagetLength)) { DgnLengD.nLoInfoInMemb[1] = 1; } // ???
			break;
		}

		dSumLengI += dLengthDel;
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetDgnSuptLengDFromSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, double &dSpanLength)
{
	// Get Memb Total Length
	dSpanLength = 0.0;

	// Get Element Index in SpanD
	int nElemIndex = 0;
	int nElemNumInSpan = SPanD.aElemBase.GetSize();
	for (int i = 0; i < nElemNumInSpan; ++i)
	{
		T_ELEM_K TempElemK = SPanD.aElemBase[i].ElemK;
		if (ElemK == TempElemK)
		{
			nElemIndex = i;
			break;
		}
	}

	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
	T_NODE_K NodeK[2] = { (unsigned int)ElemD.elnod[0], (unsigned int)ElemD.elnod[1] }; // I & J

	BOOL bSupportNode[2] = { FALSE, }; // I & j
	BOOL bFix[2][6] = { FALSE, };        // I & j

	if (!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(ElemK, bSupportNode[0], bSupportNode[1], bFix[0], bFix[1])) { return FALSE; }

	// i j 가 각각 인덱스가 커지는 방향인가? 아니면 작아지는 방향인가?
	BOOL bExpandDeIndex = TRUE, bExpandInIndex = TRUE;
	int nElemIndexMin = nElemIndex == 0 ? -1 : nElemIndex - 1;
	int nElemIndexMax = nElemIndex == nElemNumInSpan - 1 ? -1 : nElemIndex + 1;

	// Check Decrease Index direction
	if (nElemIndexMin != -1)
	{
		T_ELEM_K PreElemK = SPanD.aElemBase[nElemIndexMin].ElemK;
		T_ELEM_D PreElemD;
		m_pDoc->m_pAttrCtrl->GetElem(PreElemK, PreElemD);
		T_NODE_K PreNodeK[2] = { (unsigned int)PreElemD.elnod[0], (unsigned int)PreElemD.elnod[1] }; // I & J    

		if (PreNodeK[0] == NodeK[0] || PreNodeK[1] == NodeK[0]) { bExpandDeIndex = !bSupportNode[0]; } // Index decrease direction -> I direction
		else if (PreNodeK[0] == NodeK[1] || PreNodeK[1] == NodeK[1]) { bExpandDeIndex = !bSupportNode[1]; } // Index decrease direction -> J direction
		else { ASSERT(0); }
	}
	else
	{
		bExpandDeIndex = FALSE;
	}

	// Check Increase Index direction
	if (nElemIndexMax != -1)
	{
		T_ELEM_K NextElemK = SPanD.aElemBase[nElemIndexMax].ElemK;
		T_ELEM_D NextElemD;
		m_pDoc->m_pAttrCtrl->GetElem(NextElemK, NextElemD);
		T_NODE_K NextNodeK[2] = { (unsigned int)NextElemD.elnod[0], (unsigned int)NextElemD.elnod[1] }; // I & J    

		if (NextNodeK[0] == NodeK[0] || NextNodeK[1] == NodeK[0]) { bExpandInIndex = !bSupportNode[0]; } // Index decrease direction -> I direction
		else if (NextNodeK[0] == NodeK[1] || NextNodeK[1] == NodeK[1]) { bExpandInIndex = !bSupportNode[1]; } // Index decrease direction -> J direction
		else { ASSERT(0); }
	}
	else
	{
		bExpandInIndex = FALSE;
	}

	int nElemIndexDe = 0, nElemIndexIn = 0;

	// for Decrease Index Direction
	if (bExpandDeIndex)
	{
		int nDeRepeatNum = nElemIndexMin;
		BOOL bDeFix = FALSE;

		for (int nDe = nElemIndexMin; nDe == 0; --nDe)
		{
			T_ELEM_K Key = SPanD.aElemBase[nDe].ElemK;
			double dL = m_pDoc->calcLAVElem(Key);
			dSpanLength += dL;

			if (!m_pDoc->m_pAttrCtrl2->IsElemSup(Key, bDeFix)) { return FALSE; }
			if (bDeFix) break;
		}
	}

	// for Increase Index Direction
	if (bExpandInIndex)
	{
		int nDeRepeatNum = nElemIndexMax;
		BOOL bInFix = FALSE;

		for (int nIn = nElemIndexMax; nIn < nElemNumInSpan; ++nIn)
		{
			T_ELEM_K Key = SPanD.aElemBase[nIn].ElemK;
			double dL = m_pDoc->calcLAVElem(Key);
			dSpanLength += dL;

			BOOL bElemSup = FALSE;
			if (!m_pDoc->m_pAttrCtrl2->IsElemSup(Key, bInFix)) { return FALSE; }

			if (bInFix) break;
		}
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetDgnSuptTypeDFromSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, int nSpanEndSupType1YZ[2], int nSpanEndSupType2YZ[2])
{
	// Get Element Index in SpanD
	int nElemIndex = 0;
	int nElemNumInSpan = SPanD.aElemBase.GetSize();
	for (int i = 0; i < nElemNumInSpan; ++i)
	{
		T_ELEM_K TempElemK = SPanD.aElemBase[i].ElemK;
		if (ElemK == TempElemK)
		{
			nElemIndex = i;
			break;
		}
	}

	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
	T_NODE_K NodeK[2] = { (unsigned int)ElemD.elnod[0], (unsigned int)ElemD.elnod[1] }; // I & J

	BOOL bSupportNode[2] = { FALSE, }; // I & j
	BOOL bFix[2][6] = { FALSE, };        // I & j
	BOOL bSpanEndFix[2][6] = { FALSE, };        // In & De Direction

	if (!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(ElemK, bSupportNode[0], bSupportNode[1], bFix[0], bFix[1])) { return FALSE; }

	// i j 가 각각 인덱스가 커지는 방향인가? 아니면 작아지는 방향인가?
	BOOL bExpandDeIndex = TRUE, bExpandInIndex = TRUE;
	int nElemIndexMin = nElemIndex == 0 ? -1 : nElemIndex - 1;
	int nElemIndexMax = nElemIndex == nElemNumInSpan - 1 ? -1 : nElemIndex + 1;

	// Check Decrease Index direction
	if (nElemIndexMin != -1)
	{
		T_ELEM_K PreElemK = SPanD.aElemBase[nElemIndexMin].ElemK;
		T_ELEM_D PreElemD;
		m_pDoc->m_pAttrCtrl->GetElem(PreElemK, PreElemD);
		T_NODE_K PreNodeK[2] = { (unsigned int)PreElemD.elnod[0], (unsigned int)PreElemD.elnod[1] }; // I & J    

		if (PreNodeK[0] == NodeK[0] || PreNodeK[1] == NodeK[0]) { bExpandDeIndex = !bSupportNode[0]; } // Index decrease direction -> I direction
		else if (PreNodeK[0] == NodeK[1] || PreNodeK[1] == NodeK[1]) { bExpandDeIndex = !bSupportNode[1]; } // Index decrease direction -> J direction
		else { ASSERT(0); }
	}
	else
	{
		bExpandDeIndex = FALSE;
	}

	// Check Increase Index direction
	if (nElemIndexMax != -1)
	{
		T_ELEM_K NextElemK = SPanD.aElemBase[nElemIndexMax].ElemK;
		T_ELEM_D NextElemD;
		m_pDoc->m_pAttrCtrl->GetElem(NextElemK, NextElemD);
		T_NODE_K NextNodeK[2] = { (unsigned int)NextElemD.elnod[0], (unsigned int)NextElemD.elnod[1] }; // I & J    

		if (NextNodeK[0] == NodeK[0] || NextNodeK[1] == NodeK[0]) { bExpandInIndex = !bSupportNode[0]; } // Index decrease direction -> I direction
		else if (NextNodeK[0] == NodeK[1] || NextNodeK[1] == NodeK[1]) { bExpandInIndex = !bSupportNode[1]; } // Index decrease direction -> J direction
		else { ASSERT(0); }
	}
	else
	{
		bExpandInIndex = FALSE;
	}

	int nElemIndexDe = 0, nElemIndexIn = 0;

	// for Decrease Index Direction
	if (bExpandDeIndex)
	{
		int nDeRepeatNum = nElemIndexMin;
		BOOL bDeFix[2] = { FALSE, };
		BOOL bSpanDeFix[2][6] = { FALSE, };

		for (int nDe = nElemIndexMin; nDe == 0; --nDe)
		{
			T_ELEM_K Key = SPanD.aElemBase[nDe].ElemK;

			if (!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(Key, bDeFix[0], bDeFix[1], bSpanDeFix[0], bSpanDeFix[1])) { return FALSE; }

			if (bDeFix[0] || bDeFix[1])
			{
				if (bDeFix[0])
				{
					for (int a = 0; a < 6; ++a) { bSpanEndFix[0][a] = bSpanDeFix[0][a]; }
				}
				else if (bDeFix[1])
				{
					for (int a = 0; a < 6; ++a) { bSpanEndFix[0][a] = bSpanDeFix[1][a]; }
				}
				else { ASSERT(0); }
				break;
			}
		}
	}

	// for Increase Index Direction
	if (bExpandInIndex)
	{
		int nDeRepeatNum = nElemIndexMax;
		BOOL bInFix[2] = { FALSE, };
		BOOL bSpanInFix[2][6] = { FALSE, };

		for (int nIn = nElemIndexMax; nIn < nElemNumInSpan; ++nIn)
		{
			T_ELEM_K Key = SPanD.aElemBase[nIn].ElemK;

			BOOL bElemSup = FALSE;
			if (!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(Key, bInFix[0], bInFix[1], bSpanInFix[0], bSpanInFix[1])) { return FALSE; }

			if (bInFix[0] || bInFix[1])
			{
				if (bInFix[0])
				{
					for (int a = 0; a < 6; ++a) { bSpanEndFix[1][a] = bSpanInFix[0][a]; }
				}
				else if (bInFix[1])
				{
					for (int a = 0; a < 6; ++a) { bSpanEndFix[1][a] = bSpanInFix[1][a]; }
				}
				else { ASSERT(0); }
				break;
			}
		}
	}

	// Get Memb Support Type  
	int nSupTypeY[2] = { 0, };  // 0 : Fix, 1 : Hinge, 2 : Free
	int nSupTypeZ[2] = { 0, };  // 0 : Fix, 1 : Hinge, 2 : Free

	for (int nEnd = 0; nEnd < 2; ++nEnd)
	{
		if ((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][1]) && bSpanEndFix[nEnd][4]) { nSupTypeY[nEnd] = 0; }
		else if ((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][1]) && !bSpanEndFix[nEnd][4]) { nSupTypeY[nEnd] = 1; }
		else { nSupTypeY[nEnd] = 2; }

		if ((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][2]) && bSpanEndFix[nEnd][4]) { nSupTypeZ[nEnd] = 0; }
		else if ((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][2]) && !bSpanEndFix[nEnd][4]) { nSupTypeZ[nEnd] = 1; }
		else { nSupTypeZ[nEnd] = 2; }
	}

	nSpanEndSupType1YZ[0] = nSupTypeY[0];
	nSpanEndSupType1YZ[0] = nSupTypeZ[0];
	nSpanEndSupType2YZ[1] = nSupTypeY[1];
	nSpanEndSupType2YZ[1] = nSupTypeZ[1];


	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetDgnElemLocDInSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, const double dSpanLength, const int nStartElemIndex, const int nEndElemIndex, int nElemLocaD[2])
{
	// Find Location in Memb.

	int nDevideNum = 4;
	double dTagetLength = dSpanLength / double(nDevideNum);

	int nElemIndex = 0;
	int nElemNumInSpan = SPanD.aElemBase.GetSize();
	for (int i = 0; i < nElemNumInSpan; ++i)
	{
		T_ELEM_K TempElemK = SPanD.aElemBase[i].ElemK;
		if (ElemK == TempElemK)
		{
			nElemIndex = i;
			break;
		}
	}

	double dSumLengI = 0.0;
	for (int n = nStartElemIndex; n < nEndElemIndex + 1; ++n)
	{
		double dLengthDel = m_pDoc->calcLAVElem(SPanD.aElemBase[n].ElemK);
		double dSumLengJ = dSumLengI + dLengthDel;
		if (n == nElemIndex)
		{
			if (dSumLengI < dTagetLength || dSumLengI >(dSpanLength - dTagetLength)) { nElemLocaD[0] = 1; } // ???
			if (dSumLengI < dTagetLength || dSumLengJ >(dSpanLength - dTagetLength)) { nElemLocaD[1] = 1; } // ???
			break;
		}

		dSumLengI += dLengthDel;
	}


	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_UnbracedLengthData(T_ELEM_K ElemK, BOOL bKfac, T_KFAC_D &KFacD, STL_MEMB_ULEN &rData)
{
	T_LENG_D LengD; LengD.Initialize();
	BOOL bAutoCalc = FALSE;
	double dLength = 0.0;
	m_pForcCtrl->m_pDgnForceCtrl->m_pDgnDataCtrl->Get_DgnGenLeng(ElemK, 4, LengD, dLength);

	rData.dLy = LengD.BLy;
	rData.dLz = LengD.BLz;
	rData.dLb = LengD.BLu;

	if (bKfac)
	{
		rData.dKy = KFacD.BKy;
		rData.dKz = KFacD.BKz;
	}
	else
	{
		rData.dKy = rData.dKz = 1.0;
	}
	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_UnbracedLengthData(T_ELEM_K ElemK, STL_MEMB_ULEN &rData, BOOL &bUnLeng, BOOL &bEffLengF)
{
	BOOL bLeng = FALSE;
	BOOL bKfac = FALSE;

	T_LENG_D LengD; LengD.Initialize();
	double dLength = 0.0;
	if (m_pForcCtrl->m_pDgnForceCtrl->m_pDgnDataCtrl->Get_DgnGenLeng(ElemK, 4, LengD, dLength))
	{
		bLeng = TRUE;
		rData.dLy = LengD.BLy;
		rData.dLz = LengD.BLz;
		rData.dLb = LengD.BLu;
	}

	CDgnDataCtrl DgnCtrl;
	T_KFAC_D KFacD; KFacD.Initialize();

	if (m_pDoc->m_pAttrCtrl->GetKfac(ElemK, KFacD))
	{
		bKfac = TRUE;
	}

	if (bKfac)
	{
		rData.dKy = KFacD.BKy;
		rData.dKz = KFacD.BKz;
	}
	else
	{
		rData.dKy = rData.dKz = 1.0;
	}

	bUnLeng = bLeng;
	bEffLengF = bKfac;

	if (!bLeng && !bKfac) return FALSE;

	return TRUE;
}


void CDgnRUSStlCompDataCtrl::GetCrFlgStrnRibRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSNRibRat, double dBFSNRibRat, double &dTFlgSNRibMaxR, double &dBFlgSNRibMaxR, T_SDCR_BASE &rTRib, T_SDCR_BASE &rBRib)
{
	if (nStrnCt == 0)
	{
		dTFlgSNRibMaxR = dTFSNRibRat;
		dBFlgSNRibMaxR = dBFSNRibRat;
		rTRib = LcomKeyD;
		rBRib = LcomKeyD;
	}
	else
	{
		// Top
		if (dTFlgSNRibMaxR < dTFSNRibRat)
		{
			dTFlgSNRibMaxR = dTFSNRibRat;
			rTRib = LcomKeyD;
		}
		// Bottom
		if (dBFlgSNRibMaxR < dBFSNRibRat)
		{
			dBFlgSNRibMaxR = dBFSNRibRat;
			rBRib = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::GetCrFlgStrnDeckRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSNDeckRat, double dBFSNDeckRat, double &dTFlgSNDeckMaxR, double &dBFlgSNDeckMaxR, T_SDCR_BASE &rTDeck, T_SDCR_BASE &rBDeck)
{
	if (nStrnCt == 0)
	{
		dTFlgSNDeckMaxR = dTFSNDeckRat;
		dBFlgSNDeckMaxR = dBFSNDeckRat;
		rTDeck = LcomKeyD;
		rBDeck = LcomKeyD;
	}
	else
	{
		// Top
		if (dTFlgSNDeckMaxR < dTFSNDeckRat)
		{
			dTFlgSNDeckMaxR = dTFSNDeckRat;
			rTDeck = LcomKeyD;
		}
		// Bottom
		if (dBFlgSNDeckMaxR < dBFSNDeckRat)
		{
			dBFlgSNDeckMaxR = dBFSNDeckRat;
			rBDeck = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::GetCrFlgStrnFBeamRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSNFBeamRat, double dBFSNFBeamRat, double &dTFlgSNFBeamMaxR, double &dBFlgSNFBeamMaxR, T_SDCR_BASE &rTFbeam, T_SDCR_BASE &rBFbeam)
{
	if (nStrnCt == 0)
	{
		dTFlgSNFBeamMaxR = dTFSNFBeamRat;
		dBFlgSNFBeamMaxR = dBFSNFBeamRat;
		rTFbeam = LcomKeyD;
		rBFbeam = LcomKeyD;
	}
	else
	{
		// Top
		if (dTFlgSNFBeamMaxR < dTFSNFBeamRat)
		{
			dTFlgSNFBeamMaxR = dTFSNFBeamRat;
			rTFbeam = LcomKeyD;
		}
		// Bottom
		if (dBFlgSNFBeamMaxR < dBFSNFBeamRat)
		{
			dBFlgSNFBeamMaxR = dBFSNFBeamRat;
			rBFbeam = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::GetCrWebStrnRibRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSNRibRat, double &dWebSNRibMaxR, T_SDCR_BASE &rWRib)
{
	if (nStrnCt == 0)
	{
		dWebSNRibMaxR = dWebSNRibRat;
		rWRib = LcomKeyD;
	}
	else
	{
		if (dWebSNRibMaxR < dWebSNRibRat)
		{
			dWebSNRibMaxR = dWebSNRibRat;
			rWRib = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::GetCrWebStrnDeckRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSNDeckRat, double &dWebSNDeckMaxR, T_SDCR_BASE &rWDeck)
{
	if (nStrnCt == 0)
	{
		dWebSNDeckMaxR = dWebSNDeckRat;
		rWDeck = LcomKeyD;
	}
	else
	{
		if (dWebSNDeckMaxR < dWebSNDeckRat)
		{
			dWebSNDeckMaxR = dWebSNDeckRat;
			rWDeck = LcomKeyD;
		}
	}
}


void CDgnRUSStlCompDataCtrl::GetCrGDStabRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dStbGDRat, double &dSBGDMaxR, T_SDCR_BASE &rGDCr)
{
	if (nStrnCt == 0)
	{
		dSBGDMaxR = dStbGDRat;
		rGDCr = LcomKeyD;
	}
	else
	{
		if (dSBGDMaxR < dStbGDRat)
		{
			dSBGDMaxR = dStbGDRat;
			rGDCr = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::GetCrFlgStabRibRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSBRibRat, double dBFSBRibRat, double &dTFlgSBRibMaxR, double &dBFlgSBRibMaxR, T_SDCR_BASE &rTRib, T_SDCR_BASE &rBRib)
{
	if (nStrnCt == 0)
	{
		dTFlgSBRibMaxR = dTFSBRibRat;
		dBFlgSBRibMaxR = dBFSBRibRat;
		rTRib = LcomKeyD;
		rBRib = LcomKeyD;
	}
	else
	{
		// Top
		if (dTFlgSBRibMaxR < dTFSBRibRat)
		{
			dTFlgSBRibMaxR = dTFSBRibRat;
			rTRib = LcomKeyD;
		}
		// Bottom
		if (dBFlgSBRibMaxR < dBFSBRibRat)
		{
			dBFlgSBRibMaxR = dBFSBRibRat;
			rBRib = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::GetCrFlgStabDeckRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSBDeckRat, double dBFSBDeckRat, double &dTFlgSBDeckMaxR, double &dBFlgSBDeckMaxR, T_SDCR_BASE &rTDeck, T_SDCR_BASE &rBDeck)
{
	if (nStrnCt == 0)
	{
		dTFlgSBDeckMaxR = dTFSBDeckRat;
		dBFlgSBDeckMaxR = dBFSBDeckRat;
		rTDeck = LcomKeyD;
		rBDeck = LcomKeyD;
	}
	else
	{
		// Top
		if (dTFlgSBDeckMaxR < dTFSBDeckRat)
		{
			dTFlgSBDeckMaxR = dTFSBDeckRat;
			rTDeck = LcomKeyD;
		}
		// Bottom
		if (dBFlgSBDeckMaxR < dBFSBDeckRat)
		{
			dBFlgSBDeckMaxR = dBFSBDeckRat;
			rBDeck = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::GetCrFlgStabFBeamRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSBFBeamRat, double dBFSBFBeamRat, double &dTFlgSBFBeamMaxR, double &dBFlgSBFBeamMaxR, T_SDCR_BASE &rTFbeam, T_SDCR_BASE &rBFbeam)
{
	if (nStrnCt == 0)
	{
		dTFlgSBFBeamMaxR = dTFSBFBeamRat;
		dBFlgSBFBeamMaxR = dBFSBFBeamRat;
		rTFbeam = LcomKeyD;
		rBFbeam = LcomKeyD;
	}
	else
	{
		// Top
		if (dTFlgSBFBeamMaxR < dTFSBFBeamRat)
		{
			dTFlgSBFBeamMaxR = dTFSBFBeamRat;
			rTFbeam = LcomKeyD;
		}
		// Bottom
		if (dBFlgSBFBeamMaxR < dBFSBFBeamRat)
		{
			dBFlgSBFBeamMaxR = dBFSBFBeamRat;
			rBFbeam = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::GetCrWebStabRibRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSBRibRat, double &dWebSBRibMaxR, T_SDCR_BASE &rWRib)
{
	if (nStrnCt == 0)
	{
		dWebSBRibMaxR = dWebSBRibRat;
		rWRib = LcomKeyD;
	}
	else
	{
		if (dWebSBRibMaxR < dWebSBRibRat)
		{
			dWebSBRibMaxR = dWebSBRibRat;
			rWRib = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::GetCrWebStabDeckRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSBDeckRat, double &dWebSBDeckMaxR, T_SDCR_BASE &rWDeck)
{
	if (nStrnCt == 0)
	{
		dWebSBDeckMaxR = dWebSBDeckRat;
		rWDeck = LcomKeyD;
	}
	else
	{
		if (dWebSBDeckMaxR < dWebSBDeckRat)
		{
			dWebSBDeckMaxR = dWebSBDeckRat;
			rWDeck = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::Get_AFCr_PlasticLimitRatio(int nPlimCt, const T_PLASTIC_LIM_BASE& LcomKeyD, double dPlasticLimitRat, double& dPlasticLimitRatMax, T_PLASTIC_LIM_BASE& rPlasticLim)
{
	if (nPlimCt == 0)
	{
		dPlasticLimitRatMax = dPlasticLimitRat;
		rPlasticLim = LcomKeyD;
	}
	else
	{
		if (dPlasticLimitRatMax < dPlasticLimitRat)
		{
			dPlasticLimitRatMax = dPlasticLimitRat;
			rPlasticLim = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::Get_AFCrStrnConcreteRatio(int nStrnCt, const T_ACST_CONC_BASE &LcomKeyD, double dConcRat, double &dConcMaxRat, T_ACST_CONC_BASE &rConc)
{
	if (nStrnCt == 0)
	{
		dConcMaxRat = dConcRat;
		rConc = LcomKeyD;
	}
	else
	{
		if (dConcMaxRat < dConcRat)
		{
			dConcMaxRat = dConcRat;
			rConc = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::Get_AFCrStrnRebarRatio(int nStrnCt, const T_ACST_REBAR_BASE &LcomKeyD, double dRbarRat, double &dRbarMaxRat, T_ACST_REBAR_BASE &rRebar)
{
	if (nStrnCt == 0)
	{
		dRbarMaxRat = dRbarRat;
		rRebar = LcomKeyD;
	}
	else
	{
		if (dRbarMaxRat < dRbarRat)
		{
			dRbarMaxRat = dRbarRat;
			rRebar = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::Get_AFCrStrnStlGirderRatio(int nStrnCt, const T_ACST_SGFR_BASE &LcomKeyD, double dTFlgRat, double dBFlgRat, double &dTFlgMaxRat,
	double &dBFlgMaxRat, T_ACST_SGFR_BASE &rTDeck, T_ACST_SGFR_BASE &rBDeck)
{
	if (nStrnCt == 0)
	{
		dTFlgMaxRat = dTFlgRat;
		dBFlgMaxRat = dBFlgRat;
		rTDeck = LcomKeyD;
		rBDeck = LcomKeyD;
	}
	else
	{
		// Top
		if (dTFlgMaxRat < dTFlgRat)
		{
			dTFlgMaxRat = dTFlgRat;
			rTDeck = LcomKeyD;
		}
		// Bottom
		if (dBFlgMaxRat < dBFlgRat)
		{
			dBFlgMaxRat = dBFlgRat;
			rBDeck = LcomKeyD;
		}
	}
}

// After Fatigue
void CDgnRUSStlCompDataCtrl::Get_AFCr_Fati_ConcreteRatio(int nFatiCt, const T_FATI_CONC_BASE &LcomKeyD, double dFatiRat, double &dFatiMaxR,
	T_FATI_CONC_BASE &rData)
{
	if (nFatiCt == 0)
	{
		dFatiMaxR = dFatiRat;
		rData = LcomKeyD;
	}
	else
	{
		if (dFatiMaxR < dFatiRat)
		{
			dFatiMaxR = dFatiRat;
			rData = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::Get_AFCr_Fati_RebarRatio(int nFatiCt, const T_FATI_REBAR_BASE &LcomKeyD, double dFatiRat, double &dFatiMaxR,
	T_FATI_REBAR_BASE &rData)
{
	if (nFatiCt == 0)
	{
		dFatiMaxR = dFatiRat;
		rData = LcomKeyD;
	}
	else
	{
		if (dFatiMaxR < dFatiRat)
		{
			dFatiMaxR = dFatiRat;
			rData = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::Get_AFCr_Fati_StlGirderRatio(int nFatiCt, const T_FATI_STL_GIRDER_BASE &LcomKeyD, double dFatiRat, double &dFatiMaxR,
	T_FATI_STL_GIRDER_BASE &rData)
{
	if (nFatiCt == 0)
	{
		dFatiMaxR = dFatiRat;
		rData = LcomKeyD;
	}
	else
	{
		if (dFatiMaxR < dFatiRat)
		{
			dFatiMaxR = dFatiRat;
			rData = LcomKeyD;
		}
	}
}

// After Serviceability
void CDgnRUSStlCompDataCtrl::Get_AFCr_ServRatio(int nServCt, const T_CRACK_CONC_BASE &LcomKeyD, double dServRat, double &dServMaxR,
	T_CRACK_CONC_BASE &rData)
{
	if (nServCt == 0)
	{
		dServMaxR = dServRat;
		rData = LcomKeyD;
	}
	else
	{
		if (dServMaxR < dServRat)
		{
			dServMaxR = dServRat;
			rData = LcomKeyD;
		}
	}
}

// Shear Connector
void CDgnRUSStlCompDataCtrl::Get_AFCr_ShearConnectorRatio(int nShearConnCt, const T_STRD_SHEAR_CONN_BASE &LcomKeyD, double dGRat, double &dGMaxR,
	T_STRD_SHEAR_CONN_BASE &rData)
{
	if (nShearConnCt == 0)
	{
		dGMaxR = dGRat;
		rData = LcomKeyD;
	}
	else
	{
		if (dGMaxR < dGRat)
		{
			dGMaxR = dGRat;
			rData = LcomKeyD;
		}
	}
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_SectData(int nPos, T_SECT_D *pSectD, T_STPO_D &StpoD, T_STL_COMP_STFN_INX_D &rStfnId, T_SECT_STIFFNESS SpscSect[5], STL_COMP_SECT_POSD &rData)
{
	if (!pSectD) { ASSERT(0); return; }
	if (nPos < 0 || nPos>1) { ASSERT(0); return; }

	// Section Data Converting

	int nStype = pSectD->nStype;
	if (pSectD->nStype == D_SECT_TYPE_TAPERED) nStype = pSectD->SectBefore.nStype;
	rData.SectInfo.nStype = GetConvertSectTypeCvlToEng(nStype);
	CStringW SectShapeW(GetConvertSectShapeCvlToEng(pSectD->SectBefore.Shape));
	CStringW SectSNameW(pSectD->SName);

	rData.SectInfo.SName = SectSNameW;
	rData.SectInfo.Shape = SectShapeW;
	rData.SectInfo.nOffsetPoint = pSectD->SectBefore.nOffsetPoint;
	rData.SectInfo.nLytype = pSectD->SectBefore.nLytype;
	rData.SectInfo.nLztype = pSectD->SectBefore.nLztype;

	for (int i = 0; i < 3; ++i)
	{
		pSectD->SectAfter.SectI.Size[i] = pSectD->SectAfter.SectJ.Size[i];
		pSectD->SectAfter.SectJ.Size[i + 3] = pSectD->SectAfter.SectI.Size[i + 3];
	}

	if (nPos == 0)
	{
		Convert_StlComp_SectBaseData(nStype, &pSectD->SectAfter.SectI, &pSectD->SectBefore.SectI, StpoD, rStfnId, rData.SectInfo);
	}
	else if (nPos == 1)
	{
		if (pSectD->nStype == D_SECT_TYPE_TAPERED)
		{
			Convert_StlComp_SectBaseData(nStype, &pSectD->SectAfter.SectJ, &pSectD->SectBefore.SectJ, StpoD, rStfnId, rData.SectInfo);
			ConvertSectStiffnessData(pSectD->SectBefore.SectJ.Stiffness, rData.AfterCompSect_Pure);
		}
		else
		{
			Convert_StlComp_SectBaseData(nStype, &pSectD->SectAfter.SectI, &pSectD->SectBefore.SectI, StpoD, rStfnId, rData.SectInfo);
			ConvertSectStiffnessData(pSectD->SectBefore.SectI.Stiffness, rData.AfterCompSect_Pure);
		}
	}

	ConvertSectStiffnessData(pSectD->SectBefore.Stiffness, rData.SectInfo.BeforeCompSect.Stiffness);
	ConvertSectStiffnessData(pSectD->SectAfter.Stiffness, rData.SectInfo.AfterCompSect.Stiffness);

	rData.BeforeSect = rData.SectInfo.BeforeCompSect.Stiffness;
	rData.SectInfo.BeforeCompSect.EffStiffness = rData.SectInfo.BeforeCompSect.Stiffness;
	rData.SectInfo.AfterCompSect.EffStiffness = rData.SectInfo.AfterCompSect.Stiffness;
	rData.SectInfo.SlabSect.EffStiffness = rData.SectInfo.SlabSect.Stiffness;

	ConvertStlCompSectPolygonData(nPos, pSectD, rData.SectInfo.AfterCompSect);

	// Stiffness Converting
	//if(rData.SectInfo.bIsTapered && nPos == 1)  ConvertSectStiffnessData(pSectD->SectBefore.SectJ.Stiffness,  rData.BeforeSect);
	// else                                                                                ConvertSectStiffnessData(pSectD->SectBefore.SectI.Stiffness,  rData.BeforeSect);

	ConvertSectStiffnessData(SpscSect[0], rData.AfterCompSect_Pure);
	ConvertSectStiffnessData(SpscSect[1], rData.AfterCompSect_Posi);
	ConvertSectStiffnessData(SpscSect[2], rData.AfterCompSect_Nega);
	ConvertSectStiffnessData(SpscSect[3], rData.AfterCompSect_2ndG_Posi);
	ConvertSectStiffnessData(SpscSect[4], rData.AfterCompSect_2ndG_Nega);
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_SectBaseData(int nStype, T_SECT_SECTBASE_D*pSectAfter, T_SECT_SECTBASE_D *pSectBefore, T_STPO_D &StpoD, T_STL_COMP_STFN_INX_D &rStfnId,
	STL_COMP_SECT_INFO &rData)
{
	if (pSectAfter == NULL) return;
	CStringW SectShapeW(GetConvertSectShapeCvlToEng(pSectAfter->Shape));
	CStringW SectDBName(pSectAfter->DBName);
	CStringW SectSName(pSectAfter->SName);
	CStringW SectSName2(pSectAfter->SName2);

	rData.BeforeCompSect.nSectType = GetConvertSectTypeCvlToEng(nStype);
	rData.BeforeCompSect.Shape = SectShapeW;
	rData.BeforeCompSect.DBName = SectDBName;
	rData.BeforeCompSect.SName = SectSName;
	rData.BeforeCompSect.SName2 = SectSName2;

	rData.AfterCompSect.nSectType = GetConvertSectTypeCvlToEng(nStype);
	rData.AfterCompSect.Shape = SectShapeW;
	rData.AfterCompSect.DBName = SectDBName;
	rData.AfterCompSect.SName = SectSName;
	rData.AfterCompSect.SName2 = SectSName2;

	ConvertSectStiffnessData(pSectBefore->Stiffness, rData.BeforeCompSect.Stiffness);

	// Size Data Convert
	Convert_StlComp_SectSize(nStype, pSectBefore, pSectAfter, rData);
	memcpy(rData.BeforeCompSect.EffSize, rData.BeforeCompSect.Size, sizeof(rData.BeforeCompSect.Size));
	memcpy(rData.AfterCompSect.EffSize, rData.AfterCompSect.Size, sizeof(rData.AfterCompSect.Size));
	memcpy(rData.SlabSect.EffSize, rData.SlabSect.Size, sizeof(rData.SlabSect.Size));

	// Stiffener Data Convert
	Convert_Stl_Comp_StiffenerData(nStype, pSectBefore, StpoD, &pSectBefore->SODStiffener, rData.BeforeCompSect.Stiffener);
	Convert_Stl_Comp_StiffenerData(nStype, pSectBefore, StpoD, &pSectBefore->SODStiffener, rData.AfterCompSect.Stiffener);

	//
	MakeSectStfnIndexInfo(nStype, pSectBefore, rStfnId);
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_SectSize(int nStype, T_SECT_SECTBASE_D *pSectBefore, T_SECT_SECTBASE_D *pSectAfter, STL_COMP_SECT_INFO &rData)
{
	// Civil  Size Index 참조
	//   double Bc = rData.SectAfter.SectJ.Size[0];
	//   double tc = rData.SectAfter.SectJ.Size[1];
	//   double Hh = rData.SectAfter.SectJ.Size[2];
	// 
	//   double NiuS=rData.SectBefore.Matl_Poisson_S;
	//   double NiuC=rData.SectBefore.Matl_Poisson_C;  
	//   double Gr = 1.0+NiuS==0.0 ? 0.0 : Er*(1.0+NiuC)/(1.0+NiuS); // Gs/Gc
	// 
	//   double Sg  = i==0 ? rData.SectAfter.SectI.Size[3] : rData.SectAfter.SectJ.Size[3];
	//   double Top = i==0 ? rData.SectAfter.SectI.Size[4] : rData.SectAfter.SectJ.Size[4];
	//   double Bot = i==0 ? rData.SectAfter.SectI.Size[5] : rData.SectAfter.SectJ.Size[5];
	// 
	//   double B1 =  rData.SectBefore.SectI.Size[0]];
	//   double B2 =  rData.SectBefore.SectI.Size[1]];
	//   double B3 =  rData.SectBefore.SectI.Size[2]];
	//   double B4 =  rData.SectBefore.SectI.Size[3]];
	//   double H  = irData.SectBefore.SectI.Size[4] ;
	//   double t1 = irData.SectBefore.SectI.Size[5] ;
	//   double t2 = irData.SectBefore.SectI.Size[6] ;
	//   double tw =  rData.SectBefore.SectI.Size[7]]; 

	double dBc = pSectAfter->Size[0];
	double dtc = pSectAfter->Size[1];
	double dHh = pSectAfter->Size[2];

	double dSg = pSectAfter->Size[3];
	double dTopL = pSectAfter->Size[4];
	double dBotL = pSectAfter->Size[5];

	// Get Effective Slab Width
	double dBc_eff = 0.0;
	double dBcl_eff = 0.0;
	double dBcr_eff = 0.0;

	BOOL bIsEffSect = FALSE;
	if (!bIsEffSect)
	{
		dBc_eff = dBc; // Effective Slab Width 
	}
	else
	{
		// 우선은 채움, 확인 필요, PCS
		dBc_eff = dBc; // Effective Slab Width 
	}

	// Concrete Slab Size Convert
	rData.SlabSect.Size[0] = dBc; // Slab Width
	rData.SlabSect.Size[1] = dtc; // Slab Thickness
	rData.SlabSect.Size[2] = dHh; // Height of Slab Hunch
	rData.SlabSect.Size[3] = dSg; // Ref_Sg
	rData.SlabSect.Size[4] = dTopL; // Ref_Top
	rData.SlabSect.Size[5] = dBotL; // Ref_Bot

	if (nStype == D_SECT_TYPE_COMPO_STLG_I)
	{
		double dHw = pSectBefore->Size[4]; // Height of pure web
		double dtt = pSectBefore->Size[5]; // Thickness of Top Flange
		double dtb = pSectBefore->Size[6]; // Thickness of Bottom Flange
		double dHt = dHw + dtt + dtb; // Total Height of Steel Girder

		memcpy(rData.BeforeCompSect.Size, pSectBefore->Size, sizeof(rData.BeforeCompSect.Size));
		// Index Size Dimension (Before Composite)
		// 0 : Btl, 1 : Btr, 2 : Bbl, 3 : Bbr,  4 : Hw, 5 : t_top,  6 : t_bot, 7 : tw, 8 : Bt (= Btl + Btr),  9 : Bb(=  Bbl + Bbr), 10 : H (= Hw + t_top + t_bot),  11 : theta (Angle of Web)

		double dBtl = pSectBefore->Size[0];
		double dBtr = pSectBefore->Size[1];
		double dBt = dBtl + dBtr;

		double dBbl = pSectBefore->Size[2];
		double dBbr = pSectBefore->Size[3];
		double dBb = dBbl + dBbr;

		double dtw = pSectBefore->Size[7];

		double dSide = (dTopL + dBtl) - (dBotL + dBbl);
		double dTheta = 0.0;
		if (fabs(dSide) < m_dZero)
		{
			dTheta = 0.5*CMathFunc::m_pi;
		}
		else if (dSide > 0.0)
		{
			double dTanV = (dHw == 0.0) ? 0.0 : dSide / dHw;
			double dAngRad = atan(dTanV);
			dTheta = 0.5*CMathFunc::m_pi - dAngRad;
		}
		else if (dSide < 0.0)
		{
			double dTanV = (dHw == 0.0) ? 0.0 : dSide / dHw;
			double dAngRad = atan(dTanV);
			dTheta = 0.5*CMathFunc::m_pi + dAngRad;
		}

		dBcl_eff = dTopL - dSg + dBtl; // Effective Slab Width  Left
		dBcr_eff = dBc_eff - dBcl_eff; // Effective Slab Width  Right

		rData.BeforeCompSect.Size[8] = dBt;
		rData.BeforeCompSect.Size[9] = dBb;
		rData.BeforeCompSect.Size[10] = dHt;
		rData.BeforeCompSect.Size[11] = dTheta;

		// Index Size Dimension (After Composite)
		// 0 : Btl, 1 : Btr, 2 : Bbl, 3 : Bbr,  4 : Hw, 5 : t_top,  6 : t_bot, 7 : tw, 8 : Bt (= Btl + Btr),  9 : Bb(=  Bbl + Bbr), 10 : H (= Hw + t_top + t_bot),  11 : theta (Angle of Web) - Same for before Composite Index
		// 12 : Bc,  13 : tc,   14 : Hh , 15 : Effective Slab Width , 16 : Effective Slab Width Left,   17 : Effective Slab Width Right   - Different for before Composite Index
		rData.AfterCompSect.Size[0] = dBtl;
		rData.AfterCompSect.Size[1] = dBtr;
		rData.AfterCompSect.Size[2] = dBbl;
		rData.AfterCompSect.Size[3] = dBbr;
		rData.AfterCompSect.Size[4] = dHw;
		rData.AfterCompSect.Size[5] = dtt;
		rData.AfterCompSect.Size[6] = dtb;
		rData.AfterCompSect.Size[7] = dtw;
		rData.AfterCompSect.Size[8] = dTheta;
		rData.AfterCompSect.Size[9] = dBc;
		rData.AfterCompSect.Size[10] = dtc;
		rData.AfterCompSect.Size[11] = dHh;
		rData.AfterCompSect.Size[12] = dSg;
		rData.AfterCompSect.Size[13] = dTopL;
		rData.AfterCompSect.Size[14] = dBotL;
		rData.AfterCompSect.Size[15] = dBc_eff;
		rData.AfterCompSect.Size[16] = dBcl_eff;
		rData.AfterCompSect.Size[17] = dBcr_eff;

	}
	else if (nStype == D_SECT_TYPE_COMPO_STLG_B)
	{
		double dHw = pSectBefore->Size[6];
		double dtt = pSectBefore->Size[7];
		double dtb = pSectBefore->Size[8];
		double dHt = dHw + dtt + dtb;
		rData.BeforeCompSect.Size[1] = dHw; // Hw
		rData.BeforeCompSect.Size[2] = pSectBefore->Size[9];  // twl
		rData.BeforeCompSect.Size[3] = pSectBefore->Size[10]; // twr

		double dtwl = pSectBefore->Size[9]; // ???
		double dtwr = pSectBefore->Size[9]; // ???

		double dBtl = pSectBefore->Size[0];
		double dBtm = pSectBefore->Size[1];
		double dBtr = pSectBefore->Size[2];
		double dBt = dBtl + dBtm + dBtr;

		double dBbl = pSectBefore->Size[3];
		double dBbm = pSectBefore->Size[4];
		double dBbr = pSectBefore->Size[5];
		double dBb = dBbl + dBbm + dBbr;

		double dSideL = (dTopL + dBtl) - (dBotL + dBbl);
		double dThetaL = 0.0;
		if (fabs(dSideL) < m_dZero)
		{
			dThetaL = 0.5*CMathFunc::m_pi;
		}
		else if (dSideL > 0.0)
		{
			double dTanV = (dHw == 0.0) ? 0.0 : dSideL / dHw;
			double dAngRad = atan(dTanV);
			dThetaL = 0.5*CMathFunc::m_pi + dAngRad;
		}
		else if (dSideL < 0.0)
		{
			double dTanV = (dHw == 0.0) ? 0.0 : dSideL / dHw;
			double dAngRad = atan(dTanV);
			dThetaL = 0.5*CMathFunc::m_pi - dAngRad;
		}

		double dSideR = (dTopL + dBtl + dBtm) - (dBotL + dBbl + dBbm);
		double dThetaR = 0.0;
		if (fabs(dSideR) < m_dZero)
		{
			dThetaR = 0.5*CMathFunc::m_pi;
		}
		else if (dSideR > 0.0)
		{
			double dTanV = (dHw == 0.0) ? 0.0 : dSideR / dHw;
			double dAngRad = atan(dTanV);
			dThetaR = 0.5*CMathFunc::m_pi - dAngRad;
		}
		else if (dSideR < 0.0)
		{
			double dTanV = (dHw == 0.0) ? 0.0 : dSideR / dHw;
			double dAngRad = atan(dTanV);
			dThetaR = 0.5*CMathFunc::m_pi + dAngRad;
		}

		// Index Size Dimension (Before Composite)
		rData.BeforeCompSect.Size[0] = dBtl;
		rData.BeforeCompSect.Size[1] = dBtm;
		rData.BeforeCompSect.Size[2] = dBtr;
		rData.BeforeCompSect.Size[3] = dBbl;
		rData.BeforeCompSect.Size[4] = dBbm;
		rData.BeforeCompSect.Size[5] = dBbr;
		rData.BeforeCompSect.Size[6] = dHw;
		rData.BeforeCompSect.Size[7] = dtt;
		rData.BeforeCompSect.Size[8] = dtb;
		rData.BeforeCompSect.Size[9] = dtwl;
		rData.BeforeCompSect.Size[10] = dtwr;
		rData.BeforeCompSect.Size[11] = dThetaL;
		rData.BeforeCompSect.Size[12] = dThetaR;

		// Index Size Dimension (After Composite)
		rData.AfterCompSect.Size[0] = dBtl;
		rData.AfterCompSect.Size[1] = dBtm;
		rData.AfterCompSect.Size[2] = dBtr;
		rData.AfterCompSect.Size[3] = dBbl;
		rData.AfterCompSect.Size[4] = dBbm;
		rData.AfterCompSect.Size[5] = dBbr;
		rData.AfterCompSect.Size[6] = dHw;
		rData.AfterCompSect.Size[7] = dtt;
		rData.AfterCompSect.Size[8] = dtb;
		rData.AfterCompSect.Size[9] = dtwl;
		rData.AfterCompSect.Size[10] = dtwr;
		rData.AfterCompSect.Size[11] = dThetaL;
		rData.AfterCompSect.Size[12] = dThetaR;

		rData.AfterCompSect.Size[13] = dBc;
		rData.AfterCompSect.Size[14] = dtc;
		rData.AfterCompSect.Size[15] = dHh;

		rData.AfterCompSect.Size[16] = dSg;
		rData.AfterCompSect.Size[17] = dTopL;
		rData.AfterCompSect.Size[18] = dBotL;

		rData.AfterCompSect.Size[19] = dBc_eff;
		rData.AfterCompSect.Size[20] = dBcl_eff;
		rData.AfterCompSect.Size[21] = dBcr_eff;
	}
	else if (nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		double dHw = pSectBefore->Size[6];
		double dtt = pSectBefore->Size[7];
		double dtb = pSectBefore->Size[8];
		double dHt = dHw + dtt + dtb;

		double dtwl = pSectBefore->Size[9];
		double dtwr = pSectBefore->Size[10];

		double dBtl = pSectBefore->Size[0];
		double dBtm = pSectBefore->Size[1];
		double dBtr = pSectBefore->Size[2];
		double dBt = dBtl + dBtm + dBtr;

		double dBtl_l = pSectBefore->Size[11]; // Bf1
		double dBtr_r = pSectBefore->Size[12]; // Bf2

		double dBbl = pSectBefore->Size[3];
		double dBbm = pSectBefore->Size[4];
		double dBbr = pSectBefore->Size[5];
		double dBb = dBbl + dBbm + dBbr;

		// Left Web
		double dSideL = (dTopL + dBtl_l) - (dBotL + dBbl);
		double dThetaL = 0.0;
		if (fabs(dSideL) < m_dZero)
		{
			dThetaL = 0.5*CMathFunc::m_pi;
		}
		else if (dSideL > 0.0)
		{
			double dTanV = (dHw == 0.0) ? 0.0 : dSideL / dHw;
			double dAngRad = atan(dTanV);
			dThetaL = 0.5*CMathFunc::m_pi + dAngRad;
		}
		else if (dSideL < 0.0)
		{
			double dTanV = (dHw == 0.0) ? 0.0 : dSideL / dHw;
			double dAngRad = atan(dTanV);
			dThetaL = 0.5*CMathFunc::m_pi - dAngRad;
		}

		// Right Web
		double dSideR = (dTopL + dBtl + dBtm + dBtr - dBtr_r) - (dBotL + dBbl + dBbm);
		double dThetaR = 0.0;
		if (fabs(dSideR) < m_dZero)
		{
			dThetaR = 0.5*CMathFunc::m_pi;
		}
		else if (dSideR > 0.0)
		{
			double dTanV = (dHw == 0.0) ? 0.0 : dSideR / dHw;
			double dAngRad = atan(dTanV);
			dThetaR = 0.5*CMathFunc::m_pi - dAngRad;
		}
		else if (dSideR < 0.0)
		{
			double dTanV = (dHw == 0.0) ? 0.0 : dSideR / dHw;
			double dAngRad = atan(dTanV);
			dThetaR = 0.5*CMathFunc::m_pi + dAngRad;
		}


		// Index Size Dimension (Before Composite)
		rData.BeforeCompSect.Size[0] = dBtl;
		rData.BeforeCompSect.Size[1] = dBtm;
		rData.BeforeCompSect.Size[2] = dBtr;
		rData.BeforeCompSect.Size[3] = dBbl;
		rData.BeforeCompSect.Size[4] = dBbm;
		rData.BeforeCompSect.Size[5] = dBbr;

		rData.BeforeCompSect.Size[6] = dBtl_l;
		rData.BeforeCompSect.Size[7] = dBtr_r;

		rData.BeforeCompSect.Size[8] = dHw;
		rData.BeforeCompSect.Size[9] = dtt;
		rData.BeforeCompSect.Size[10] = dtb;
		rData.BeforeCompSect.Size[11] = dtwl;
		rData.BeforeCompSect.Size[12] = dtwr;
		rData.BeforeCompSect.Size[13] = dThetaL;
		rData.BeforeCompSect.Size[14] = dThetaR;


		// Index Size Dimension (After Composite)
		rData.AfterCompSect.Size[0] = dBtl;
		rData.AfterCompSect.Size[1] = dBtm;
		rData.AfterCompSect.Size[2] = dBtr;
		rData.AfterCompSect.Size[3] = dBbl;
		rData.AfterCompSect.Size[4] = dBbm;
		rData.AfterCompSect.Size[5] = dBbr;

		rData.AfterCompSect.Size[6] = dBtl_l;
		rData.AfterCompSect.Size[7] = dBtr_r;

		rData.AfterCompSect.Size[8] = dHw;
		rData.AfterCompSect.Size[9] = dtt;
		rData.AfterCompSect.Size[10] = dtb;
		rData.AfterCompSect.Size[11] = dtwl;
		rData.AfterCompSect.Size[12] = dtwr;
		rData.AfterCompSect.Size[13] = dThetaL;
		rData.AfterCompSect.Size[14] = dThetaR;

		rData.AfterCompSect.Size[15] = dBc;
		rData.AfterCompSect.Size[16] = dtc;
		rData.AfterCompSect.Size[17] = dHh;

		rData.AfterCompSect.Size[18] = dSg;
		rData.AfterCompSect.Size[19] = dTopL;
		rData.AfterCompSect.Size[20] = dBotL;

		rData.AfterCompSect.Size[21] = dBc_eff;
		rData.AfterCompSect.Size[22] = dBcl_eff;
		rData.AfterCompSect.Size[23] = dBcr_eff;
	}
	else ASSERT(0);

}

void CDgnRUSStlCompDataCtrl::Convert_SectStiffnessData(const T_SECT_STIFFNESS& crStiff, DGN_SECT_STIFFNESS& rData)
{
	rData.dArea = crStiff.Area;
	rData.dAsy = crStiff.Asy;
	rData.dAsz = crStiff.Asz;
	rData.dIxx = crStiff.Rxx;
	rData.dIyy = crStiff.Ryy;
	rData.dIzz = crStiff.Rzz;
	rData.dCyp = crStiff.Cyp;
	rData.dCym = crStiff.Cym;
	rData.dCzp = crStiff.Czp;
	rData.dCzm = crStiff.Czm;
	rData.dQyb = crStiff.Qyb;
	rData.dQzb = crStiff.Qzb;
	rData.dWArea = crStiff.WArea;
	rData.dx1 = crStiff.dx1;
	rData.dx2 = crStiff.dx2;
	rData.dx3 = crStiff.dx3;
	rData.dx4 = crStiff.dx4;
	rData.dy1 = crStiff.dy1;
	rData.dy2 = crStiff.dy2;
	rData.dy3 = crStiff.dy3;
	rData.dy4 = crStiff.dy4;
	rData.dCwp = crStiff.dIw;
}

void CDgnRUSStlCompDataCtrl::Convert_RebarData(_DGN_RBAR_CRC& rInD, DGN_RBAR_RC& rData)
{
	CStringW strRbarNa(rInD.strRbarNa);
	rData.strRbarNa = strRbarNa;
	rData.dArea = rInD.dArea;
	rData.dDia = rInD.dDia;
	rData.dy = rInD.dy;
	rData.dz = rInD.dz;
	rData.nPart = rInD.nPart;
}


void CDgnRUSStlCompDataCtrl::Convert_Stl_Comp_StiffenerData(int nStype, T_SECT_SECTBASE_D *pSectD, T_STPO_D &StpoD, T_SECT_SOD_STIFF_D *pStfn, ST_SECT_STIFF_D &rData)
{
	if (pSectD == NULL) return;
	if (pStfn == NULL) return;

	int nShapeSize = pStfn->aStiffShape.GetSize();
	if (nShapeSize > 0) rData.aStiffShape.SetSize(nShapeSize);
	for (int i = 0; i < nShapeSize; ++i)
	{
		Convert_StlComp_StfnShapeData(&pStfn->aStiffShape[i], rData.aStiffShape[i]);
		m_mStfnNameIndex.SetAt(pStfn->aStiffShape[i].strName, i);
	}

	int nStiffSubSize = pStfn->aSODStiffSub.GetSize();
	for (int i = 0; i < nStiffSubSize; ++i)
	{
		T_SECT_SOD_STIFF_SUB_D &PartStfnD = pStfn->aSODStiffSub[i];

		Convert_StlComp_StfnProfileData(nStype, pSectD, StpoD, PartStfnD, rData);
	}

}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_StfnShapeData(T_STIFF_SHAPE_D *pShape, ST_STIFF_SHAPE_D &rData)
{
	if (pShape == NULL) return;

	rData.strName = pShape->strName;
	rData.nType = pShape->nType;
	memcpy(rData.dSize, pShape->dSize, sizeof(rData.dSize));
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_StfnProfileData(int nStype, T_SECT_SECTBASE_D *pSectD, T_STPO_D &StpoD, T_SECT_SOD_STIFF_SUB_D &PartStfn, ST_SECT_STIFF_D &rData)
{
	if (PartStfn.nDeckPart < 0 || PartStfn.nDeckPart>2) return;

	double dWidth = 0.0;
	if (nStype == D_SECT_TYPE_COMPO_STLG_B || nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		if (PartStfn.nDeckPos == 0) // Top
		{
			if (PartStfn.nDeckPart == 0) // Left Outstand
			{
				dWidth = pSectD->Size[0];
				Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffTopLeft);
			}
			else if (PartStfn.nDeckPart == 1) // Internal
			{
				dWidth = pSectD->Size[1];
				Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffTop);

			}
			else if (PartStfn.nDeckPart == 2) // Right Outstand
			{
				dWidth = pSectD->Size[2];
				Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffTopRight);
			}
			else ASSERT(0);
		}
		else if (PartStfn.nDeckPos == 1) // web.L
		{
			dWidth = pSectD->Size[6];
			Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffLeft);
		}
		else if (PartStfn.nDeckPos == 2) // web.R
		{
			dWidth = pSectD->Size[6];
			Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffRight);
		}
		else if (PartStfn.nDeckPos == 3) // Bottom
		{
			if (PartStfn.nDeckPart == 0) // Left Outstand
			{
				dWidth = pSectD->Size[3];
				Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffBottomLeft);
			}
			else if (PartStfn.nDeckPart == 1) // Internal
			{
				dWidth = pSectD->Size[4];
				Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffBottom);

			}
			else if (PartStfn.nDeckPart == 2) // Right Outstand
			{
				dWidth = pSectD->Size[5];
				Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffBottomRigth);
			}
			else ASSERT(0);
		}
		else ASSERT(0);
	}
	else if (nStype == D_SECT_TYPE_COMPO_STLG_I)
	{
		if (PartStfn.nDeckPos == 0) // Top
		{
			if (PartStfn.nDeckPart == 0) // Left Outstand
			{
				dWidth = pSectD->Size[0];
				Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffTopLeft);
			}
			else if (PartStfn.nDeckPart == 1) // Right Outstand
			{
				dWidth = pSectD->Size[1];
				Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffTopRight);
			}
			else ASSERT(0);
		}
		else if (PartStfn.nDeckPos == 1) // web
		{
			dWidth = pSectD->Size[4];
			Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffLeft);
		}
		else if (PartStfn.nDeckPos == 2) // Bottom
		{
			if (PartStfn.nDeckPart == 0) // Left Outstand
			{
				dWidth = pSectD->Size[2];
				Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffBottomLeft);
			}
			else if (PartStfn.nDeckPart == 1) // Right Outstand
			{
				dWidth = pSectD->Size[3];
				Convert_StlComp_StfnProfileData(nStype, StpoD, dWidth, PartStfn, rData.aStiffBottomRigth);
			}
			else ASSERT(0);
		}
		else ASSERT(0);
	}
	else ASSERT(0);
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_StfnProfileData(int nStype, T_STPO_D &StpoD, double dWidth, T_SECT_SOD_STIFF_SUB_D &PartStfn, CArray<ST_STIFF_PROFILE_D, ST_STIFF_PROFILE_D&> &arData)
{
	int nProfileSize = PartStfn.aStiffData.GetSize();
	if (nProfileSize <= 0) return;

	BOOL bReCalSpace = IsReplaceStfnIndex(nStype, PartStfn.nRefPos, PartStfn.nDeckPos, PartStfn.nDeckPart);

	double dDistLastStfn = 0.0;
	if (bReCalSpace)
	{
		for (int i = 0; i < nProfileSize; ++i)
		{
			dDistLastStfn += PartStfn.aStiffData[i].dSpacing;
		}
	}

	CArray<ST_STIFF_PROFILE_D, ST_STIFF_PROFILE_D&> aStfnProD;

	aStfnProD.SetSize(nProfileSize);
	for (int i = 0; i < nProfileSize; ++i)
	{
		int nId = (bReCalSpace == TRUE) ? nProfileSize - 1 - i : i;
		T_SECT_SOD_STIFF_DATA_D OneStfnD = PartStfn.aStiffData[nId];
		if (bReCalSpace)
		{
			if (i == 0)
			{
				OneStfnD.dSpacing = dWidth - dDistLastStfn;
			}
			else
			{
				OneStfnD.dSpacing = PartStfn.aStiffData[nProfileSize - i].dSpacing;
			}
		}

		Convert_StlComp_StfnProfileData(StpoD, PartStfn.nRefPos, PartStfn.nDeckPos, PartStfn.nDeckPart, dWidth, &OneStfnD, aStfnProD[i]); // OneStfnD.nStiffPos

	}
	arData.Append(aStfnProD);

}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_StfnProfileData(T_STPO_D &StpoD, int nRefPos, int nDeckPos, int nDeckPart, double dWidth, T_SECT_SOD_STIFF_DATA_D *pStfnProf, ST_STIFF_PROFILE_D &rData)
{
	if (pStfnProf == NULL) return;
	rData.dSpacing = pStfnProf->dSpacing;
	int nShape = 0;
	if (m_mStfnNameIndex.Lookup(pStfnProf->strShapeName, nShape))
	{
		rData.nShape = nShape;
	}
	rData.bCalc = pStfnProf->bCalc;
	rData.strName = pStfnProf->strStiffName;
	rData.nPosition = pStfnProf->nStiffPos;

	CArray<T_POINT_SSM, T_POINT_SSM&> aStressPoint;
	CSectUtil::GetStressPoint4Stiiffener_SUB(StpoD, nDeckPos, nDeckPart, pStfnProf->strStiffName, aStressPoint);

	int nDSize = sizeof(rData.Point) / sizeof(DGN_GSEC_VERTEX);
	int nPtSize = aStressPoint.GetSize();

	if (nDSize < nPtSize)
	{
		ASSERT(0);
		return;
	}

	for (int i = 0; i < nPtSize; ++i)
	{
		T_POINT_SSM &PtS = aStressPoint[i];
		rData.Point[i].dX = PtS.dPointY;
		rData.Point[i].dY = PtS.dPointZ;
	}
	//DGN_GSEC_VERTEX Point[20];   // flat : 4개, T    : 8개,      // U    : 12개
}

BOOL CDgnRUSStlCompDataCtrl::ConvertStlCompSectPolygonData(int nPos, T_SECT_D *pSectD, ST_SECT_SECTBASE_D &rData)
{
	CDb_DllManager_Base DllMgr;
	DGN_GSEC_SHAPE_D SectD; SectD.Initialize();
	int nPosConv = (nPos == 0) ? 0 : 4;
	if (!DllMgr.ConvertToDLL_SectData_Pos(*pSectD, nPosConv, SectD, TRUE, TRUE)) return FALSE;

	DGN_GSEC_POLYGON_LIST    arConcOutPoly;       arConcOutPoly.RemoveAll(); // 콘크리트 슬래브
	DGN_GSEC_POLYGON_LIST    arConcInPoly;        arConcInPoly.RemoveAll();
	DGN_GSEC_POLYGON_LIST    arSteelOutPoly;      arSteelOutPoly.RemoveAll(); // Steel 거더
	DGN_GSEC_POLYGON_LIST    arSteelInPoly;       arSteelInPoly.RemoveAll();
	DGN_GSEC_LINE_LIST       arSteelLine;         arSteelLine.RemoveAll();
	DGN_GSEC_VERTEXLINE_LIST arSteelVertexLine;   arSteelVertexLine.RemoveAll();
	DGN_GSEC_POLYGON_LIST    arSteelLinePoly;     arSteelLinePoly.RemoveAll();
	DGN_POLYGON_POS_LIST     arDeckPos;           arDeckPos.RemoveAll();

	double dOutPos[6] = { 0.0, };
	BOOL bLineDetail = TRUE;

	// 여기는 작업 필요.
	if (!dgn::link::ITmpDgnDrawLib::GetInstance()->Get_SectionPolygonData(SectD, arSteelOutPoly, arSteelInPoly, arConcOutPoly, arConcInPoly, arSteelLine, arSteelVertexLine, arSteelLinePoly, dOutPos, -SectD.dYBar, -SectD.dZBar, bLineDetail)) return FALSE;
	if (!dgn::link::ITmpDgnDrawLib::GetInstance()->Get_SectionPolygonDataIsCalcStiffSOD(SectD, rData)) return FALSE;

	// Outpoly  1:슬래브, 나머지 :steel
	INT_PTR nOutSize1 = arSteelOutPoly.GetSize();
	INT_PTR nOutSize2 = arConcOutPoly.GetSize();
	rData.aOuterPolygon.SetSize(nOutSize1 + nOutSize2);
	rData.arDeckPos.SetSize(nOutSize1);

	// Steel Girder
	for (INT_PTR i = 0; i < nOutSize1; i++)
	{
		DGN_GSEC_POLYGON  POLYGON;
		int nSize = arSteelOutPoly[i].aVertex.GetSize();
		rData.aOuterPolygon[i].nMatkK = 2;
		rData.aOuterPolygon[i].aVertex.SetSize(nSize);
		for (int j = 0; j < nSize; j++)
		{
			rData.aOuterPolygon[i].aVertex[j].dX = arSteelOutPoly[i].aVertex[j].dX;
			rData.aOuterPolygon[i].aVertex[j].dY = arSteelOutPoly[i].aVertex[j].dY;
		}
	}

	int nTotOut = 0;
	// Concrete Slab
	for (INT_PTR i = 0; i < nOutSize2; i++)
	{
		nTotOut = nOutSize1 + i;
		DGN_GSEC_POLYGON  POLYGON;
		int nSize = arConcOutPoly[i].aVertex.GetSize();
		rData.aOuterPolygon[nTotOut].nMatkK = 1;
		rData.aOuterPolygon[nTotOut].aVertex.SetSize(nSize);

		for (int j = 0; j < nSize; j++)
		{
			rData.aOuterPolygon[nTotOut].aVertex[j].dX = arConcOutPoly[i].aVertex[j].dX;
			rData.aOuterPolygon[nTotOut].aVertex[j].dY = arConcOutPoly[i].aVertex[j].dY;
		}
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Convert_Lcom(T_ELEM_K ElemK, INT_PTR nPart, INT_PTR nLcomOrgK)
{


	return TRUE;
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompCalcUlmCSForce(int nPosI, int nPosM, int nPosJ, _DGN_FORC_CRC &ForI, _DGN_FORC_CRC &ForM, _DGN_FORC_CRC &ForJ, STL_COMP_ULMF_D &rData)
{
	if (nPosI < 0 || nPosI>1) return;
	if (nPosM < 0 || nPosM>1) return;
	if (nPosJ < 0 || nPosJ>1) return;

	ConvertStlCompCalcForce4CSStage(nPosI, ForI, rData.ForD[0]);
	ConvertStlCompCalcForce4CSStage(nPosM, ForM, rData.ForD[1]);
	ConvertStlCompCalcForce4CSStage(nPosJ, ForJ, rData.ForD[2]);
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompCalcForce4CSStage(int nPos, _DGN_FORC_CRC &For, STL_COMP_FORCE_D &rData)
{
	if (nPos < 0 || nPos>1) return;

	rData.ForceBefore.dForce[0] = For.dFxx[nPos];
	rData.ForceBefore.dForce[1] = For.dFyy[nPos];
	rData.ForceBefore.dForce[2] = For.dFzz[nPos];
	rData.ForceBefore.dForce[3] = For.dMux[nPos];
	rData.ForceBefore.dForce[4] = For.dMuy[nPos];
	rData.ForceBefore.dForce[5] = For.dMuz[nPos];
}



void CDgnRUSStlCompDataCtrl::ConvertStlCompCalcForce(int nPos, _DGN_FORC_CRC &ForBefore, _DGN_FORC_CRC &ForAfter, _DGN_FORC_CRC &ForAfter_DL, _DGN_FORC_CRC &ForD_Crp,
	_DGN_FORC_CRC &ForD_Shr, _DGN_FORC_CRC &ForD_BST, _DGN_SCFP_PART_LCASE &Str_Crp, _DGN_SCFP_PART_LCASE &Str_Shr,
	_DGN_SCFP_PART_LCASE &Str_BST, STL_COMP_FORCE_D &rData)
{
	rData.Initialize();
	ConvertStlCompCalcForceUnit(nPos, ForBefore, rData.ForceBefore);
	_DGN_FORC_CRC ForceAfter = ForAfter;  ForceAfter -= ForBefore;
	_DGN_FORC_CRC ForceAfterDL = ForAfter_DL;  ForceAfterDL -= ForBefore;
	ConvertStlCompCalcForceUnit(nPos, ForceAfter, rData.ForceAfter);
	ConvertStlCompCalcForceUnit(nPos, ForceAfterDL, rData.ForceAfterDL);
	ConvertStlCompCalcForceUnit(nPos, ForD_Crp, rData.ForceAfer_Crp);
	ConvertStlCompCalcForceUnit(nPos, ForD_Shr, rData.ForceAfer_Shr);
	ConvertStlCompCalcForceUnit(nPos, ForD_BST, rData.ForceAfer_BST);

	ConvertStlCompCalcStressUnit(nPos, Str_Crp, rData.CreepInterStress);
	ConvertStlCompCalcStressUnit(nPos, Str_Shr, rData.ShrinkageInterStress);
	ConvertStlCompCalcStressUnit(nPos, Str_BST, rData.TemperatureInterStress);
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompCalcForceUnit(int nPos, _DGN_FORC_CRC &For, STL_FORCE_D &rData)
{
	if (nPos < 0 || nPos>1) return;

	rData.dForce[0] = For.dFxx[nPos];
	rData.dForce[1] = For.dFyy[nPos];
	rData.dForce[2] = For.dFzz[nPos];
	rData.dForce[3] = For.dMux[nPos];
	rData.dForce[4] = For.dMuy[nPos];
	rData.dForce[5] = For.dMuz[nPos];
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompCalcForceUnit(int nPos, _DGN_FORC_CRC &For, T_CSGD_FORCE &rData)
{
	rData.dForce[0] = For.dFxx[nPos];
	rData.dForce[1] = For.dFyy[nPos];
	rData.dForce[2] = For.dFzz[nPos];
	rData.dForce[3] = For.dMux[nPos];
	rData.dForce[4] = For.dMuy[nPos];
	rData.dForce[5] = For.dMuz[nPos];
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompCalcStressUnit(int nPos, _DGN_SCFP_PART_LCASE &For, STL_COMP_INTERNAL_STRESS_2D &rData)
{
	if (nPos < 0 || nPos>1) return;
	rData.dStress_Ax = For.ForceP1.dbLStress[nPos][0];
	rData.dStress_Ax_P2 = For.ForceP2.dbLStress[nPos][0];

	for (int i = 0; i < 9; ++i)
	{
		rData.dStress_M_y[i] = For.ForceP1.dbLStress[nPos][i + 1];
		rData.dStress_M_z[i] = For.ForceP1.dbLStress[nPos][i + 10];
		rData.dStress_M_y_P2[i] = For.ForceP2.dbLStress[nPos][i + 1];
		rData.dStress_M_z_P2[i] = For.ForceP2.dbLStress[nPos][i + 10];
	}
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompCalcStressUnit(int nPos, _DGN_SCFP_PART_LCASE &For, T_SCGD_INT_STRESS &rData)
{
	if (nPos < 0 || nPos>1) return;
	rData.dStress_Ax = For.ForceP1.dbLStress[nPos][0];
	rData.dStress_Ax_P2 = For.ForceP2.dbLStress[nPos][0];

	for (int i = 0; i < 9; ++i)
	{
		rData.dStress_M_y[i] = For.ForceP1.dbLStress[nPos][i + 1];
		rData.dStress_M_z[i] = For.ForceP1.dbLStress[nPos][i + 10];
		rData.dStress_M_y_P2[i] = For.ForceP2.dbLStress[nPos][i + 1];
		rData.dStress_M_z_P2[i] = For.ForceP2.dbLStress[nPos][i + 10];
	}
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompForce(int nPos, _DGN_FORC_CRC &For, T_CSGD_FORCE &rData)
{
	rData.dForce[0] = For.dFxx[nPos];
	rData.dForce[1] = For.dFyy[nPos];
	rData.dForce[2] = For.dFzz[nPos];
	rData.dForce[3] = For.dMux[nPos];
	rData.dForce[4] = For.dMuy[nPos];
	rData.dForce[5] = For.dMuz[nPos];
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompForce4SaveD(int nPos, _DGN_FORC_CRC &ForBefore, _DGN_FORC_CRC &ForAfter, _DGN_FORC_CRC &ForAfter_DL, _DGN_FORC_CRC &ForD_Crp,
	_DGN_FORC_CRC &ForD_Shr, _DGN_FORC_CRC &ForD_BST, _DGN_SCFP_PART_LCASE &Str_Crp, _DGN_SCFP_PART_LCASE &Str_Shr,
	_DGN_SCFP_PART_LCASE &Str_BST, T_SCGD_DGN_FORCE& rData)
{
	ConvertStlCompCalcForceUnit(nPos, ForBefore, rData.Elem[nPos].ForBC);
	_DGN_FORC_CRC ForceAfter = ForAfter;  ForceAfter -= ForBefore;
	_DGN_FORC_CRC ForceAfterDL = ForAfter_DL;  ForceAfterDL -= ForBefore;
	ConvertStlCompCalcForceUnit(nPos, ForceAfter, rData.Elem[nPos].ForAC);
	ConvertStlCompCalcForceUnit(nPos, ForceAfterDL, rData.Elem[nPos].ForceAfterDL);
	ConvertStlCompCalcForceUnit(nPos, ForD_Crp, rData.Elem[nPos].ForceAfer_Crp);
	ConvertStlCompCalcForceUnit(nPos, ForD_Shr, rData.Elem[nPos].ForceAfer_Shr);
	ConvertStlCompCalcForceUnit(nPos, ForD_BST, rData.Elem[nPos].ForceAfer_BST);

	ConvertStlCompCalcStressUnit(nPos, Str_Crp, rData.Elem[nPos].InterStrCrp);
	ConvertStlCompCalcStressUnit(nPos, Str_Shr, rData.Elem[nPos].InterStrShr);
	ConvertStlCompCalcStressUnit(nPos, Str_BST, rData.Elem[nPos].InterStrBST);
}

BOOL CDgnRUSStlCompDataCtrl::Convert_GlobalDgnParamD(STL_COMP_GLOBAL_D &rData)
{
	T_CPGD_D CpgdD;
	if (!m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD)) return FALSE;

	rData.Initialize();

	// Convert Global Design Param Data
	rData.nDgnCode = GetConvertCSGCodeCvlToEng(CpgdD.iDgnCode);
	rData.nBridgeType = CpgdD.nBridgeType;
	rData.bConsidermb7bm8 = CpgdD.bConsidermb7bm8;
	rData.nConsidermb7mb8 = CpgdD.nConsidermb7mb8;
	rData.dmb7 = CpgdD.dmb7;
	rData.dmb8 = CpgdD.dmb8;
	rData.bConsidermb9 = CpgdD.bConsidermb9;
	rData.nConsidermb9 = CpgdD.nConsidermb9;
	rData.dmb9 = CpgdD.dmb9;
	rData.dPsiFactor_1stG = CpgdD.dPsi_1stGroup;
	rData.dPsiFactor_2ndG = CpgdD.dPsi_2ndGroup;

	rData.dBehaviorConF4Erection = CpgdD.dBehaviorConF4Erection;
	rData.dBehaviorConF4Operation = CpgdD.dBehaviorConF4Operation;
	rData.dGamman = CpgdD.dGamman;
	rData.nComponentSigxType = CpgdD.nComponentSigxType;
	rData.bDgnCheck[0] = CpgdD.bDesignParameter[0];
	rData.bDgnCheck[1] = CpgdD.bDesignParameter[1];
	rData.bDgnCheck[2] = CpgdD.bDesignParameter[2];
	rData.bDgnCheck[3] = CpgdD.bDesignParameter[3];
	rData.bDgnCheck[4] = CpgdD.bDesignParameter[4];

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::Convert_Stl_CompDgnLengthData(const T_EDLK_D &DgnLengD, STL_COMP_RES_BEFORE_BASE_SNiP &rData)
{
	rData.GDgnD.dDgnLengY = DgnLengD.dDgnLengY;
	rData.GDgnD.dDgnLengZ = DgnLengD.dDgnLengZ;
	rData.GDgnD.dEffLengF_Y = DgnLengD.dEffBuckCoeffY;
	rData.GDgnD.dEffLengF_Z = DgnLengD.dEffBuckCoeffZ;
	rData.GDgnD.dDgnLengZ = DgnLengD.dDgnLengZ;
	rData.GDgnD.bExistSpanInfo = DgnLengD.bExistSpanInfo;
	rData.GDgnD.bExistMembInfo = DgnLengD.bExistMembInfo;
	rData.GDgnD.bExistUbracedInfo = DgnLengD.bExistUbracedInfo;
	rData.GDgnD.nLocationI = DgnLengD.nLoInfoInMemb[0];
	rData.GDgnD.nLocationJ = DgnLengD.nLoInfoInMemb[1];
	rData.GDgnD.nSuptTypeI_Y = DgnLengD.nSupTypeY[0];
	rData.GDgnD.nSuptTypeI_Z = DgnLengD.nSupTypeZ[0];
	rData.GDgnD.nSuptTypeJ_Y = DgnLengD.nSupTypeY[1];
	rData.GDgnD.nSuptTypeJ_Z = DgnLengD.nSupTypeZ[1];

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Convert_Stl_CompDgnLengthData(const T_EDLK_D &DgnLengD, STL_COMP_RES_AFTER_BASE_SNiP &rData)
{
	rData.GDgnD.dDgnLengY = DgnLengD.dDgnLengY;
	rData.GDgnD.dDgnLengZ = DgnLengD.dDgnLengZ;
	rData.GDgnD.dEffLengF_Y = DgnLengD.dEffBuckCoeffY;
	rData.GDgnD.dEffLengF_Z = DgnLengD.dEffBuckCoeffZ;
	rData.GDgnD.dDgnLengZ = DgnLengD.dDgnLengZ;
	rData.GDgnD.bExistSpanInfo = DgnLengD.bExistSpanInfo;
	rData.GDgnD.bExistMembInfo = DgnLengD.bExistMembInfo;
	rData.GDgnD.bExistUbracedInfo = DgnLengD.bExistUbracedInfo;
	rData.GDgnD.nLocationI = DgnLengD.nLoInfoInMemb[0];
	rData.GDgnD.nLocationJ = DgnLengD.nLoInfoInMemb[1];
	rData.GDgnD.nSuptTypeI_Y = DgnLengD.nSupTypeY[0];
	rData.GDgnD.nSuptTypeI_Z = DgnLengD.nSupTypeZ[0];
	rData.GDgnD.nSuptTypeJ_Y = DgnLengD.nSupTypeY[1];
	rData.GDgnD.nSuptTypeJ_Z = DgnLengD.nSupTypeZ[1];

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::Convert_Sect2MatlD(T_MATD_D& MatD, STL_MATL_SNiP_PROP& rData)
{
	rData.strCodeName = CT2W(MatD.Data1.CodeName);
	rData.strMatlName = CT2W(MatD.Data1.CodeMatlName);
	rData.strSTLGrade = CT2W(MatD.Data1.CodeMatlName);
	rData.dRy = MatD.Data1.Design.S_Fy1;
	rData.dRyn = MatD.Data1.Design.S_Fy1;
	//rData.dRs         = MatD.Data1.Design.S_Fy;
	Convert_MatlStlPart(MatD.Data1.Analysis, MatD.Data1.Design, rData.Stl);
	rData.dE = rData.Stl.Elast;

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Convert_Stl2StlCompMatl(const T_MATD_D& crMatlD, const SLT_MATL_SNiP_POSD& crStlMatlD, STL_COMP_MAT_SNiP& rStlCompD)
{
	rStlCompD.StlMatD.MatTopF = crStlMatlD.MatTopF;
	rStlCompD.StlMatD.MatBotF = crStlMatlD.MatBotF;
	rStlCompD.StlMatD.MatWeb = crStlMatlD.MatWeb;
	rStlCompD.StlMatD.MatTopF_Rib = crStlMatlD.MatTopF_Rib;
	rStlCompD.StlMatD.MatBotF_Rib = crStlMatlD.MatBotF_Rib;
	rStlCompD.StlMatD.MatWeb_Rib = crStlMatlD.MatWeb_Rib;
	rStlCompD.StlMatD.MatFB = crStlMatlD.MatFB;

	rStlCompD.ConcMatD.ConcD.strCodeName = crMatlD.Data2.CodeName;
	rStlCompD.ConcMatD.ConcD.strMatlName = crMatlD.Data2.CodeMatlName;
	rStlCompD.ConcMatD.RbarD.strCodeName = crMatlD.Rebar_CodeName;
	rStlCompD.ConcMatD.RbarD.strMatlName = crMatlD.MainRebar_RebarName;
	rStlCompD.ConcMatD.RbarD.strSbarMatlName = crMatlD.SubRebar_RebarName;

	rStlCompD.ConcMatD.RC.dFck = crMatlD.Data2.Design.C_fc;
	rStlCompD.ConcMatD.RC.dFy = crMatlD.MainRebarData.B_fy;
	rStlCompD.ConcMatD.RC.dFys = crMatlD.SubRebarData.B_fy;
	rStlCompD.ConcMatD.RC.dEc = crMatlD.Data2.Analysis.Elast;
	rStlCompD.ConcMatD.RC.dEs = crMatlD.MainRebarData.B_Elast;
	rStlCompD.ConcMatD.RC.dEss = crMatlD.SubRebarData.B_Elast;

	if (crMatlD.Type != _T("SRC"))
	{ // src가 아니면 강합성 설계 대화창에서 모든 정보 입력해야 설계가능하도록 함
		if (rStlCompD.ConcMatD.ConcD.strCodeName == _T("")) return FALSE;
		if (rStlCompD.ConcMatD.ConcD.strMatlName == _T("")) return FALSE;
		if (rStlCompD.ConcMatD.RbarD.strCodeName == _T("")) return FALSE;
		if (rStlCompD.ConcMatD.RbarD.strMatlName == _T("")) return FALSE;
		//if(rStlCompD.ConcMatD.RbarD.strSbarMatlName==_T("")) return FALSE; // 강합성에서 sub bar 적용 안 하므로, 막음

		if (rStlCompD.ConcMatD.RC.dFck <= 0.0) return FALSE;
		if (rStlCompD.ConcMatD.RC.dFy <= 0.0) return FALSE;
		//if(rStlCompD.ConcMatD.RC.dFys <=0.0) return FALSE; // 강합성에서 sub bar 적용 안 하므로, 막음
		if (rStlCompD.ConcMatD.RC.dEc <= 0.0) return FALSE;
		if (rStlCompD.ConcMatD.RC.dEs <= 0.0) return FALSE;
		//if(rStlCompD.ConcMatD.RC.dEss <=0.0) return FALSE; // 강합성에서 sub bar 적용 안 하므로, 막음
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Convert_Stl2StlCompMatl(const T_MATD_D& crMatlD, STL_COMP_MAT_SNiP& rStlCompD)
{
	// Top Flange
	rStlCompD.StlMatD.MatTopF.strCodeName = CT2W(crMatlD.Data1.CodeName);
	rStlCompD.StlMatD.MatTopF.strMatlName = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatTopF.strSTLGrade = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatTopF.dE = crMatlD.Data1.Analysis.Elast;
	Convert_MatlStlPart(crMatlD.Data1.Analysis, crMatlD.Data1.Design, rStlCompD.StlMatD.MatTopF.Stl);

	// Bottom Flange
	rStlCompD.StlMatD.MatBotF.strCodeName = CT2W(crMatlD.Data1.CodeName);
	rStlCompD.StlMatD.MatBotF.strMatlName = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatBotF.strSTLGrade = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatBotF.dE = crMatlD.Data1.Analysis.Elast;
	Convert_MatlStlPart(crMatlD.Data1.Analysis, crMatlD.Data1.Design, rStlCompD.StlMatD.MatBotF.Stl);

	// Web 
	rStlCompD.StlMatD.MatWeb.strCodeName = CT2W(crMatlD.Data1.CodeName);
	rStlCompD.StlMatD.MatWeb.strMatlName = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatWeb.strSTLGrade = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatWeb.dE = crMatlD.Data1.Analysis.Elast;
	Convert_MatlStlPart(crMatlD.Data1.Analysis, crMatlD.Data1.Design, rStlCompD.StlMatD.MatWeb.Stl);

	// Top Flange- Stiffener
	rStlCompD.StlMatD.MatTopF_Rib.strCodeName = CT2W(crMatlD.Data1.CodeName);
	rStlCompD.StlMatD.MatTopF_Rib.strMatlName = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatTopF_Rib.strSTLGrade = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatTopF_Rib.dE = crMatlD.Data1.Analysis.Elast;
	Convert_MatlStlPart(crMatlD.Data1.Analysis, crMatlD.Data1.Design, rStlCompD.StlMatD.MatTopF_Rib.Stl);

	// Bottom Flange- Stiffener
	rStlCompD.StlMatD.MatBotF_Rib.strCodeName = CT2W(crMatlD.Data1.CodeName);
	rStlCompD.StlMatD.MatBotF_Rib.strMatlName = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatBotF_Rib.strSTLGrade = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatBotF_Rib.dE = crMatlD.Data1.Analysis.Elast;
	Convert_MatlStlPart(crMatlD.Data1.Analysis, crMatlD.Data1.Design, rStlCompD.StlMatD.MatBotF_Rib.Stl);

	// Web- Stiffener
	rStlCompD.StlMatD.MatWeb_Rib.strCodeName = CT2W(crMatlD.Data1.CodeName);
	rStlCompD.StlMatD.MatWeb_Rib.strMatlName = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatWeb_Rib.strSTLGrade = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatWeb_Rib.dE = crMatlD.Data1.Analysis.Elast;
	Convert_MatlStlPart(crMatlD.Data1.Analysis, crMatlD.Data1.Design, rStlCompD.StlMatD.MatWeb_Rib.Stl);

	// Floor Beam
	rStlCompD.StlMatD.MatFB.strCodeName = CT2W(crMatlD.Data1.CodeName);
	rStlCompD.StlMatD.MatFB.strMatlName = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatFB.strSTLGrade = CT2W(crMatlD.Data1.CodeMatlName);
	rStlCompD.StlMatD.MatFB.dE = crMatlD.Data1.Analysis.Elast;
	Convert_MatlStlPart(crMatlD.Data1.Analysis, crMatlD.Data1.Design, rStlCompD.StlMatD.MatFB.Stl);

	rStlCompD.ConcMatD.ConcD.strCodeName = crMatlD.Data2.CodeName;
	rStlCompD.ConcMatD.ConcD.strMatlName = crMatlD.Data2.CodeMatlName;
	rStlCompD.ConcMatD.RbarD.strCodeName = crMatlD.Rebar_CodeName;
	rStlCompD.ConcMatD.RbarD.strMatlName = crMatlD.MainRebar_RebarName;
	rStlCompD.ConcMatD.RbarD.strSbarMatlName = crMatlD.SubRebar_RebarName;

	rStlCompD.ConcMatD.RC.dFck = crMatlD.Data2.Design.C_fc;
	rStlCompD.ConcMatD.RC.dFy = crMatlD.MainRebarData.B_fy;
	rStlCompD.ConcMatD.RC.dFys = crMatlD.SubRebarData.B_fy;
	rStlCompD.ConcMatD.RC.dEc = crMatlD.Data2.Analysis.Elast;
	rStlCompD.ConcMatD.RC.dEs = crMatlD.MainRebarData.B_Elast;
	rStlCompD.ConcMatD.RC.dEss = crMatlD.SubRebarData.B_Elast;

	if (crMatlD.Type != _T("SRC"))
	{ // src가 아니면 강합성 설계 대화창에서 모든 정보 입력해야 설계가능하도록 함
		if (rStlCompD.ConcMatD.ConcD.strCodeName == _T("")) return FALSE;
		if (rStlCompD.ConcMatD.ConcD.strMatlName == _T("")) return FALSE;
		if (rStlCompD.ConcMatD.RbarD.strCodeName == _T("")) return FALSE;
		if (rStlCompD.ConcMatD.RbarD.strMatlName == _T("")) return FALSE;
		//if(rStlCompD.ConcMatD.RbarD.strSbarMatlName==_T("")) return FALSE;  // 강합성에서 sub bar 적용 안 하므로, 막음

		if (rStlCompD.ConcMatD.RC.dFck <= 0.0) return FALSE;
		if (rStlCompD.ConcMatD.RC.dFy <= 0.0) return FALSE;
		//if(rStlCompD.ConcMatD.RC.dFys <=0.0) return FALSE;  // 강합성에서 sub bar 적용 안 하므로, 막음
		if (rStlCompD.ConcMatD.RC.dEc <= 0.0) return FALSE;
		if (rStlCompD.ConcMatD.RC.dEs <= 0.0) return FALSE;
		//if(rStlCompD.ConcMatD.RC.dEss <=0.0) return FALSE;  // 강합성에서 sub bar 적용 안 하므로, 막음
	}

	return TRUE;
}

void CDgnRUSStlCompDataCtrl::Convert_MatlStlPart(const T_MATL_ANALYSIS& AnalMatl, const  T_MATL_DESIGN &DgnMatl, STL_MATL_SNiP &rData)
{
	rData.Elast = AnalMatl.Elast;
	rData.Poisson = AnalMatl.Poisson;
	rData.Thermal = AnalMatl.Thermal;
	rData.Density = AnalMatl.Density;
	rData.MassDensity = AnalMatl.MassDensity;
	rData.S_Fu = DgnMatl.S_Fu;
	rData.S_Fy = DgnMatl.S_Fy;
	rData.S_Fy1 = DgnMatl.S_Fy1;
	rData.S_Fy2 = DgnMatl.S_Fy2;
	rData.S_Fy3 = DgnMatl.S_Fy3;
	rData.S_Fy4 = DgnMatl.S_Fy4;
	rData.S_Fy5 = DgnMatl.S_Fy5;
	rData.S_Fy6 = DgnMatl.S_Fy6;
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompKappaFactorChkPosOne(T_POINT_SSM &InD, STL_MEMB_POSD_SNiP_CHK_KAPPA &rData)
{
	rData.bChk = TRUE;
	rData.nIndex = 0;
	rData.strFatPosName = _T("");
	rData.Point.dX = InD.dPointY;
	rData.Point.dY = InD.dPointZ;
	rData.Eff_Point.dX = InD.dPointY;
	rData.Eff_Point.dY = InD.dPointZ;
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompKappaFactorChkPosDefault(CArray<T_POINT_SSM, T_POINT_SSM&>& aStressPoint, STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD &rData)
{
	CArray<T_POINT_SSM, T_POINT_SSM&> aKappaPt; aKappaPt.RemoveAll();
	aKappaPt.Copy(aStressPoint);
	int nKappaPtSize = aKappaPt.GetSize();
	rData.aKappaChkPosInfo.SetSize(nKappaPtSize);
	for (int i = 0; i < nKappaPtSize; ++i)
	{
		ConvertStlCompKappaFactorChkPosOne(aKappaPt[i], rData.aKappaChkPosInfo[i]);
	}
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompKappaFactor(T_SDPS_SUB_D &InD, STL_KAPPA_INPUT_D &rData)
{
	rData.bAutoCalc = (InD.nCalcType == 0);
	rData.dUDKappa_y = InD.dKappay;
	rData.dUDKappa_z = InD.dKappaz;
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompKappaFactorChkPos(T_SDPS_SUB_D &InD, STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD &rData)
{
	CArray<T_POINT_SSM, T_POINT_SSM&> aKappaPt;
	aKappaPt.Copy(InD.aDefault);
	aKappaPt.Append(InD.aUserSel);
	int nKappaPtSize = aKappaPt.GetSize();
	rData.aKappaChkPosInfo.SetSize(nKappaPtSize);
	for (int i = 0; i < nKappaPtSize; ++i)
	{
		ConvertStlCompKappaFactorChkPosOne(aKappaPt[i], rData.aKappaChkPosInfo[i]);
	}
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompKappaFactor(const STL_KAPPA_INPUT_D& crInD, STL_COMP_KAPPA_INPUT_D& rData)
{
	rData.bExist = TRUE;
	rData.bAutoCalc = crInD.bAutoCalc;
	rData.dUDKappa_y = crInD.dUDKappa_y;
	rData.dUDKappa_z = crInD.dUDKappa_z;
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompFatiCheckPos(int nPos, T_SDFC_D &InD, STL_COMP_FATI_CHK_POS_DGND_SNiP &rData, double dDelta_y, double dDelta_z)
{
	if (nPos < 0 || nPos>1) return;
	double dLambda = InD.dLambda[nPos];

	// Steel Girder Part Convert
	INT_PTR nFatiSize = InD.aSdfcSub[nPos].GetSize();
	rData.StlGirderPosD.aStlGirderChkPosD.SetSize(nFatiSize);
	for (INT_PTR i = 0; i < nFatiSize; ++i)
	{
		ConvertStlCompFatiCheckPosOne(InD.aSdfcSub[nPos].GetAt(i), rData.StlGirderPosD.aStlGirderChkPosD[i], dDelta_y, dDelta_z);
		rData.StlGirderPosD.aStlGirderChkPosD[i].dLambda = dLambda;
	}

	// Concrete Fatigue Check Position Convert
	nFatiSize = InD.aConSdfcSub[nPos].GetSize();
	rData.ConcPosD.After_Point_Posi.SetSize(nFatiSize);
	rData.ConcPosD.After_Point_Nega.SetSize(nFatiSize);
	for (INT_PTR j = 0; j < nFatiSize; ++j)
	{
		rData.ConcPosD.After_Point_Posi[j].dX = InD.aConSdfcSub[nPos][j].Point.dPointY;
		rData.ConcPosD.After_Point_Posi[j].dY = InD.aConSdfcSub[nPos][j].Point.dPointZ;
		rData.ConcPosD.After_Point_Nega[j].dX = InD.aConSdfcSub[nPos][j].Point.dPointY + dDelta_y;
		rData.ConcPosD.After_Point_Nega[j].dY = InD.aConSdfcSub[nPos][j].Point.dPointZ + dDelta_z;
	}


	// Rebar Fatigue Check Position Convert
	nFatiSize = InD.aRebarSdfcSub[nPos].GetSize();
	rData.RebarPosD.After_Point_Posi.SetSize(nFatiSize);
	rData.RebarPosD.After_Point_Nega.SetSize(nFatiSize);
	for (INT_PTR k = 0; k < nFatiSize; ++k)
	{
		rData.RebarPosD.After_Point_Posi[k].dX = InD.aRebarSdfcSub[nPos][k].Point.dPointY;
		rData.RebarPosD.After_Point_Posi[k].dY = InD.aRebarSdfcSub[nPos][k].Point.dPointZ;
		rData.RebarPosD.After_Point_Nega[k].dX = InD.aRebarSdfcSub[nPos][k].Point.dPointY + dDelta_y;
		rData.RebarPosD.After_Point_Nega[k].dY = InD.aRebarSdfcSub[nPos][k].Point.dPointZ + dDelta_z;
	}

	// 차후 Shear Connector Fatigue 검토하면 추가할  것.

}

void CDgnRUSStlCompDataCtrl::ConvertStlCompFatiCheckPosOne(T_SDFC_SUB_D &InD, STL_COMP_MEMB_POSD_SNiP_FAT &rData, double dDelta_y, double dDelta_z)
{
	rData.nIndex = 0;
	rData.strFatPosName = _T("");
	rData.After_Point_Posi.dX = InD.Point.dPointY;
	rData.After_Point_Posi.dY = InD.Point.dPointZ;

	// 추가 작업
	rData.After_Point_Nega.dX = InD.Point.dPointY + dDelta_y;
	rData.After_Point_Nega.dY = InD.Point.dPointZ + dDelta_z;
	rData.dBeta = InD.dBeta;
	rData.dLambda = 0.0;
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompFloorBeam(int nPos, T_SDTS_D &InD, STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D &rData)
{
	CString strPart = _LS(IDS_CMD_SECT_SOD_TOP_LEFT);
	ConvertStlCompFloorBeamPart(nPos, strPart, InD, rData.FloorBDeckP_Dgn_StaD_TL);
	strPart = _LS(IDS_CMD_SECT_SOD_TOP_CENTER);
	ConvertStlCompFloorBeamPart(nPos, strPart, InD, rData.FloorBDeckP_Dgn_StaD_TM);
	strPart = _LS(IDS_CMD_SECT_SOD_TOP_RIGHT);
	ConvertStlCompFloorBeamPart(nPos, strPart, InD, rData.FloorBDeckP_Dgn_StaD_TR);
	strPart = _LS(IDS_CMD_SECT_SOD_BOT_LEFT);
	ConvertStlCompFloorBeamPart(nPos, strPart, InD, rData.FloorBDeckP_Dgn_StaD_BL);
	strPart = _LS(IDS_CMD_SECT_SOD_BOT_CENTER);
	ConvertStlCompFloorBeamPart(nPos, strPart, InD, rData.FloorBDeckP_Dgn_StaD_BM);
	strPart = _LS(IDS_CMD_SECT_SOD_BOT_RIGHT);
	ConvertStlCompFloorBeamPart(nPos, strPart, InD, rData.FloorBDeckP_Dgn_StaD_BR);
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompFloorBeamPart(int nPos, CString &strFBPartName, T_SDTS_D &InD, STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_D &rData)
{
	if (nPos < 0 || nPos>1) return;

	CArray<T_SDTS_BEAM_SUB_D, T_SDTS_BEAM_SUB_D&> *paBeam = (nPos == 0) ? &InD.Beam[0].aBeam : &InD.Beam[1].aBeam;
	CArray<T_SDTS_DECK_SUB_D, T_SDTS_DECK_SUB_D&> *paDeck = (nPos == 0) ? &InD.Deck[0].aDeck : &InD.Deck[1].aDeck;

	/// [CIVIL-10243] deck name이 영어로 저장되어 있는데, 다국어 문자열과 비교하면서 잘못 판단되는 문제 수정.
	const CString& strFBPartNameEng = CSectUtil::GetDeckNameMultiLang2Eng(strFBPartName);

	// Beam Part

	BOOL bFindPart = FALSE;
	int nPartSize = paBeam->GetSize();
	T_SDTS_BEAM_SUB_D FbeamPart; FbeamPart.Initialize();
	for (int i = 0; i < nPartSize; ++i)
	{
		T_SDTS_BEAM_SUB_D &SdtsBS = paBeam->GetAt(i);
		if (SdtsBS.strDeck.CompareNoCase(strFBPartNameEng) == 0)
		{
			bFindPart = TRUE;
			FbeamPart = SdtsBS;
			break;
		}
	}
	if (bFindPart == FALSE) return;

	// Get Force
	for (int i = 0; i < 3; ++i)
	{
		rData.aFB_Force[i].dN = FbeamPart.Force[i].dN;
		rData.aFB_Force[i].dMy = FbeamPart.Force[i].dMy;
		rData.aFB_Force[i].dFy = FbeamPart.Force[i].dFz;
		rData.dhc[i] = FbeamPart.Force[i].dhc;
	}

	CString strFBSectName = _T("");
	T_SDTS_SECT_D SdtsSect;
	for (int i = 0; i < InD.aSect.GetSize(); ++i)
	{
		if (InD.aSect[i].strName.CompareNoCase(FbeamPart.strSect) == 0)
		{
			SdtsSect = InD.aSect[i];
			break;
		}
	}

	// Get Section of floor beam
	rData.SectFBInfo.SName = SdtsSect.strName;
	rData.SectFBInfo.nSectType = DGN_SECT_SHAPE_INDEX_REG_H;
	int nSize = 8;
	for (int i = 0; i < nSize; ++i)
	{
		rData.SectFBInfo.Size[i] = SdtsSect.dSize[i];
	}

	T_SECT_D CalcS; CalcS.Initialize();
	T_SECT_STIFFNESS &StfnStiff = CalcS.SectBefore.SectI.Stiffness;
	CalcS.nStype = D_SECT_TYPE_REGULAR;
	CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_H;
	for (int i = 0; i < nSize; ++i) CalcS.SectBefore.SectI.Size[i] = SdtsSect.dSize[i];

	CSectUtil::CalcSectData(CalcS);

	ConvertSectStiffnessData(CalcS.SectBefore.SectI.Stiffness, rData.SectFBInfo.Stiffness);
	ConvertSectStiffnessData(CalcS.SectBefore.SectI.Stiffness, rData.SectFBInfo.EffStiffness);

	// Get Material of floor beam
	T_MATD_D MatdD; MatdD.Initialize();
	if (m_pDoc->m_pAttrCtrl->GetMatlDesign(FbeamPart.MatlK, MatdD))
	{
		Convert_Sect2MatlD(MatdD, rData.FB_MatD);
	}


	// Get Deck Part Info.
	bFindPart = FALSE;
	nPartSize = paDeck->GetSize();
	T_SDTS_DECK_SUB_D FloorBeamD; FloorBeamD.Initialize();
	for (int i = 0; i < nPartSize; ++i)
	{
		const T_SDTS_DECK_SUB_D &SdtsBS = paDeck->GetAt(i);
		if (SdtsBS.strDeck.CompareNoCase(strFBPartNameEng) == 0)
		{
			bFindPart = TRUE;
			FloorBeamD = SdtsBS;
			break;
		}
	}
	if (bFindPart == FALSE) return;

	rData.bAutoCalck = (FloorBeamD.nKType == 0);
	rData.bAutoCalca = (FloorBeamD.naType == 0);
	rData.nk = (int)FloorBeamD.dk;
	rData.da = FloorBeamD.da;
	rData.dl = FloorBeamD.dI;
	rData.dL = FloorBeamD.dL;
	rData.df = FloorBeamD.df;
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompWebVerticalForce(int nStype, int nPos, T_SDAP_D &InD, STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM &rForWeb, STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM &rForWebP, BOOL bBeforeComp)
{
	if (nPos < 0 || nPos>1) return;

	int nBetaType = bBeforeComp ? InD.nBetaTypeBefore[nPos] : InD.nBetaType[nPos];
	int nConnectType = InD.nConnectType[nPos] == 0 ? 1 : 0;

	double dBeta = 0.0;
	if (nBetaType == 0) dBeta = 0.3;
	else if (nBetaType == 1) dBeta = 0.5;
	else if (nBetaType == 2) dBeta = 0.8;
	else if (nBetaType == 3) dBeta = 2.0;
	else if (nBetaType == 4) dBeta = 1.5;
	else if (nBetaType == 5) dBeta = 2.0;
	else ASSERT(0);

	INT_PTR nDataSize = 0;
	if (bBeforeComp)  nDataSize = InD.aSdapSubBefore[nPos].GetSize(); // Before Composite Local Force
	else                            nDataSize = InD.aSdapSub[nPos].GetSize();               // After Composite Local Force

	for (INT_PTR i = 0; i < nDataSize; ++i)
	{
		T_SDAP_SUB_D* pSdapSub = NULL;
		if (bBeforeComp) pSdapSub = &InD.aSdapSubBefore[nPos][i];
		else                           pSdapSub = &InD.aSdapSub[nPos][i];

		if (!pSdapSub) continue;

		STL_LFORCE_WEB_PLATE_CHKUNIT_D WebPlateD;
		ConvertStlCompWebPlateForce(nStype, dBeta, nConnectType, *pSdapSub, WebPlateD);
		rForWebP.mWebPlateFor.SetAt(pSdapSub->LcomK, WebPlateD);

		//
		STL_LFORCE_WEB_SIGY_CHKUNIT_D WebSigyD;
		ConvertStlCompWebVerticalLcomForce(nStype, *pSdapSub, WebSigyD);
		rForWeb.mWebSigYFor.SetAt(pSdapSub->LcomK, WebSigyD);
	}
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompWebPlateForce(int nStype, double dBeta, int nConnectType, T_SDAP_SUB_D &VerFor, STL_LFORCE_WEB_PLATE_CHKUNIT_D &rData)
{
	rData.bIsExistL = TRUE;
	rData.dBeta = dBeta;
	rData.nFixType = nConnectType;

	int nPlateSizeL = VerFor.aLProp.GetSize();

	rData.aWebForceL_Pl.SetSize(nPlateSizeL);
	for (int i = 0; i < nPlateSizeL; ++i)
	{
		CovertStlCompWebPlateForceBase(VerFor.aLProp[i], rData.aWebForceL_Pl[i]);
	}

	if (nStype == D_SECT_TYPE_COMPO_STLG_B || nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		int nPlateSizeR = VerFor.aRProp.GetSize();
		rData.aWebForceR_Pl.SetSize(nPlateSizeR);
		rData.bIsExistR = TRUE;
		for (int i = 0; i < nPlateSizeR; ++i)
		{
			CovertStlCompWebPlateForceBase(VerFor.aRProp[i], rData.aWebForceR_Pl[i]);
		}
	}
}

void CDgnRUSStlCompDataCtrl::CovertStlCompWebPlateForceBase(T_SDAP_PROP_D &InD, STL_LFORCE_WEB_PLATE_D &rData)
{
	rData.da = InD.dai;
	rData.dh = InD.dhi;
	rData.dSigx = InD.dpx;
	rData.dSigy = InD.dpz;
	rData.dTauxy = InD.dpxz;
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompWebVerticalLcomForce(int nStype, T_SDAP_SUB_D &VerFor, STL_LFORCE_WEB_SIGY_CHKUNIT_D &rData)
{
	rData.bTopTypeL = VerFor.TopVert.bUse;
	rData.bBottomTypeL = VerFor.BotVert.bUse;
	rData.nAppForceTypeL = 3;
	rData.bTopTypeR = VerFor.TopVert.bUse;
	rData.bBottomTypeR = VerFor.BotVert.bUse;
	rData.nAppForceTypeR = 3;
	int nLcomSize = 1;
	rData.aWebForceL_Sigy.SetSize(nLcomSize);
	rData.aWebForceR_Sigy.SetSize(nLcomSize);

	rData.aWebForceL_Sigy_B2T.SetSize(nLcomSize);
	rData.aWebForceR_Sigy_B2T.SetSize(nLcomSize);

	//T_SDAP_VERT_D &Wfor = (VerFor.TopVert.bUse==TRUE) ? VerFor.TopVert : VerFor.BotVert;

	if (VerFor.TopVert.bUse)
	{
		ConvertStlCompWebVerticalForceBase(0, VerFor.TopVert, rData.aWebForceL_Sigy[0]);
	}
	if (VerFor.BotVert.bUse)
	{
		ConvertStlCompWebVerticalForceBase(0, VerFor.BotVert, rData.aWebForceL_Sigy_B2T[0]);
	}

	if (nStype == D_SECT_TYPE_COMPO_STLG_B || nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		if (VerFor.TopVert.bUse)
		{
			ConvertStlCompWebVerticalForceBase(1, VerFor.TopVert, rData.aWebForceR_Sigy[0]);
		}
		if (VerFor.BotVert.bUse)
		{
			ConvertStlCompWebVerticalForceBase(1, VerFor.BotVert, rData.aWebForceR_Sigy_B2T[0]);
		}
	}

}

void CDgnRUSStlCompDataCtrl::ConvertStlCompWebVerticalForceBase(int nWeb, T_SDAP_VERT_D &InD, STL_LFORCE_WEB_SIGY_D &rData)
{
	if (nWeb == 0)
	{
		rData.dF = InD.dLqi;
		rData.dP = InD.dLpi;
		rData.dLef = InD.dLapi;
	}
	else
	{
		rData.dF = InD.dRqi;
		rData.dP = InD.dRpi;
		rData.dLef = InD.dRapi;
	}
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompLimitGroupParam(const T_SERV_D&  crServD, STL_COMP_LIMIT_CRACK_STATE_INPUT_D &rData)
{
	rData.bExist = TRUE;
	rData.dLimitCrackWidth = crServD.dCrackLim[0];
}

BOOL CDgnRUSStlCompDataCtrl::ConvertStlCompSectCheckPart(int nStype, int nPos, T_SDDP_D &InD, STL_COMP_CHK_POSD_SNiP &rData)
{
	int nSize = InD.aSddpSub[nPos].GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		T_SDDP_SUB_D &ChkPart = InD.aSddpSub[nPos].GetAt(i);

		/// [CIVIL-10243] deck name이 영어로 저장되어 있는데, 다국어 문자열과 비교하면서 잘못 판단되는 문제 수정.
		const CString& strDeckLang = CSectUtil::GetDeckNameEng2MultiLang(ChkPart.strDeck);

		if (strDeckLang.CompareNoCase(_LS(IDS_DB_SECT_DECK_POS_SLAB))          == 0) rData.bConcSlab = TRUE;
		if (strDeckLang.CompareNoCase(_LS(IDS_DB_SECT_DECK_POS_REINFORCEMENT)) == 0) rData.bRebar = TRUE;

		switch (nStype)
		{
		case D_SECT_TYPE_COMPO_STLG_I:
		{
			if      (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_TOP_LEFT))  == 0) rData.bTopDeckPlateChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_TOP_RIGHT)) == 0) rData.bTopDeckPlateChk[2] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_DB_SECT_DECK_POS_WEB))   == 0) rData.bWebChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_LEFT))  == 0) rData.bBotDeckPlateChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_RIGHT)) == 0) rData.bBotDeckPlateChk[2] = TRUE;
			break;
		}
		case D_SECT_TYPE_COMPO_STLG_B:
		{
			if      (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_TOP_LEFT))      == 0) rData.bTopDeckPlateChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_TOP_CENTER))    == 0) rData.bTopDeckPlateChk[1] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_TOP_RIGHT))     == 0) rData.bTopDeckPlateChk[2] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_DB_SECT_DECK_POS_WEB_LEFT))  == 0) rData.bWebChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_DB_SECT_DECK_POS_WEB_RIGHT)) == 0) rData.bWebChk[1] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_LEFT))      == 0) rData.bBotDeckPlateChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_CENTER))    == 0) rData.bBotDeckPlateChk[1] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_RIGHT))     == 0) rData.bBotDeckPlateChk[2] = TRUE;

			break;
		}
		case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			if      (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_TOP_LEFT))       == 0) rData.bTopDeckPlateChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_TOP_RIGHT))      == 0) rData.bTopDeckPlateChk[2] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_DB_SECT_DECK_POS_WEB_LEFT))   == 0) rData.bWebChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_DB_SECT_DECK_POS_WEB_RIGHT))  == 0) rData.bWebChk[1] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_LEFT))       == 0) rData.bBotDeckPlateChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_CENTER))     == 0) rData.bBotDeckPlateChk[1] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_RIGHT))      == 0) rData.bBotDeckPlateChk[2] = TRUE;

			break;
		}
		default: ASSERT(0);     return FALSE;
		}
	}

	return TRUE;
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompShearConnector(const T_SCGD_D&  crScgdD, STL_COMP_SHEAR_CONN_INPUT_DATA_SNiP &rData)
{
	rData.bExist = TRUE;
	rData.nNum = crScgdD.nNum;
	rData.dfu = crScgdD.dfu;
	rData.dDia = crScgdD.dDia;
	rData.dhsc = crScgdD.dhsc;
	rData.dSt = crScgdD.dSpace;

	rData.GroupKey = crScgdD.ScgdKey;
	CStringW strGroupNameW(crScgdD.strShearConnGroupName);
	rData.strGroupName = strGroupNameW;
	rData.StartElemKey = crScgdD.StartElemKey;
	rData.EndElemKey = crScgdD.EndElemKey;
	rData.nStartElemIJ = crScgdD.nStartElemIJ;
	rData.nEndElemIJ = crScgdD.nEndElemIJ;
	rData.dGroupLength = crScgdD.dLength;

	rData.arGroupElemKey.Copy(crScgdD.arGroupElemKey);
}


void CDgnRUSStlCompDataCtrl::Convert_StlComp_BF_Str_Flg(int nStype, BOOL bIsTop, STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &DgnResD, T_SDSN_FLG &rData, BOOL &bStlCompCheck)
{
	int nPartSize = 0;
	switch (nStype)
	{
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		nPartSize = 3;
		break;
	default: ASSERT(0);  return;
	}

	for (int i = 0; i < nPartSize; ++i) //L, M, R
	{
		STL_MEMB_RES_SNiP_STR_DP_CHK_UNIT &StrnPartR = bIsTop ? DgnResD.DeckPMembD_Top[i] : DgnResD.DeckPMembD_Bot[i];
		Convert_BF_Str_FlgPartEngToCVL(StrnPartR, rData.StrnChk[i]);

		if (rData.StrnChk[i].bChk) bStlCompCheck = TRUE;
	}
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_BF_Str_Web(int nStype, STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &DgnResD, T_SDSN_WEB &rData, BOOL &bStlCompCheck)
{
	int nWebSize = 0;
	switch (nStype)
	{
	case D_SECT_TYPE_COMPO_STLG_I:
		nWebSize = 1;
		break;
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		nWebSize = 2;
		break;
	default: ASSERT(0);  return;
	}

	for (int i = 0; i < nWebSize; ++i) //L, R
	{
		Convert_BF_Str_WebPartEngToCVL(DgnResD.WebMembD[i], rData.WebD[i]);

		if (rData.WebD[i].bChk) bStlCompCheck = TRUE;
	}

}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_BF_Sta_Flg(int nStype, BOOL bIsTop, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &DgnResD, T_SDSB_FLG &rData, BOOL &bStlCompCheck)
{
	int nPartSize = 0;
	switch (nStype)
	{
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		nPartSize = 3;
		break;
	default: ASSERT(0);  return;
	}

	for (int i = 0; i < nPartSize; ++i) //L,M,R
	{
		STL_MEMB_RES_SNiP_STA_DP_CHK_UNIT &StabPartR = (bIsTop) ? DgnResD.DeckPMembD_Top[i] : DgnResD.DeckPMembD_Bot[i];
		Convert_StabFlgPartEngToCVL(StabPartR, rData.StabChk[i]);

		if (rData.StabChk[i].bChk) bStlCompCheck = TRUE;
	}

}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_BF_Sta_Web(int nStype, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &DgnResD, T_SDSB_WEB &rData, BOOL &bStlCompCheck)
{
	int nWebSize = 0;
	switch (nStype)
	{
	case D_SECT_TYPE_COMPO_STLG_I:
		nWebSize = 1;
		break;
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		nWebSize = 2;
		break;
	default: ASSERT(0);  return;
	}

	for (int i = 0; i < nWebSize; ++i) // L, R
	{
		Convert_StabWebPartEngToCVL(DgnResD.aResWebStaD[i], rData.WebD[i]);
		if (rData.WebD[i].bChk) bStlCompCheck = TRUE;
	}

}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_BF_Sta_GD(int nStype, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &DgnResD, T_SDSB_GD &rData, BOOL &bStlCompCheck)
{
	STL_MEMB_RES_SNiP_STA_LT &StabGD = DgnResD.GlobalSTA_LTB;

	rData.bChk = StabGD.bChk;
	rData.dRat_GD = StabGD.dRat;
	rData.bOK = (StabGD.dRat <= 1.0) ? TRUE : FALSE;

	if (rData.bChk) bStlCompCheck = TRUE;
}

void CDgnRUSStlCompDataCtrl::Convert_BF_Str_FlgPartEngToCVL(STL_MEMB_RES_SNiP_STR_DP_CHK_UNIT &StrnPartR, T_SDPT_STRN_FLG &rData)
{
	int nRibSize = StrnPartR.aResRibD.GetSize();
	int nDeckSize = StrnPartR.aDeckPlateD.GetSize();

	rData.aLSChk.SetSize(nRibSize);
	for (int i = 0; i < nRibSize; ++i)
	{
		Convert_StrnLRibEngToCVL(StrnPartR.aResRibD[i], StrnPartR.aDeckPlateD[i], rData.aLSChk[i]);
	}
	//StrnPartR.nIndexCriticalRib

	// Deck
	double dRat_Deck = 0.0, dRat_Deck_Temp = 0.0;
	BOOL bChk_Deck = FALSE;
	int nDeck_cr = 0;
	for (int i = 0; i < nDeckSize; ++i)
	{
		const STL_MEMB_RES_SNiP_STR_DP &DeckPlateD = StrnPartR.aDeckPlateD.GetAt(i);

		if (DeckPlateD.bChk) bChk_Deck = TRUE;
		dRat_Deck_Temp = DeckPlateD.dRat;
		//dRat_Deck      = max(dRat_Deck, dRat_Deck_Temp);
		if (i == 0)
		{
			nDeck_cr = i;
			dRat_Deck = dRat_Deck_Temp;
		}
		else
		{
			if (dRat_Deck < dRat_Deck_Temp)
			{
				nDeck_cr = i;
				dRat_Deck = dRat_Deck_Temp;
			}
		}
	}
	rData.nDeck_cr = nDeck_cr;
	rData.dRat_Deck = dRat_Deck;
	rData.bChk_Deck = bChk_Deck;

	// Rib
	double dRat_Rib = 0.0, dRat_Rib_Temp = 0.0;
	BOOL bChk_Rib = FALSE;
	int nRib_cr = 0;
	for (int i = 0; i < nRibSize; ++i)
	{
		const STL_MEMB_RES_SNiP_STR_RIB &RIB = StrnPartR.aResRibD.GetAt(i);
		if (RIB.bChk) bChk_Rib = TRUE;
		dRat_Rib_Temp = (RIB.nGlobalLocal == 0) ? RIB.DeckPlate_Rib.dRat : RIB.dRat;   //max(RIB.dRat_Ry, RIB.dRat_Ryn);
		//dRat_Rib      = max(dRat_Rib, dRat_Rib_Temp);
		if (i == 0)
		{
			nRib_cr = i;
			dRat_Rib = dRat_Rib_Temp;
		}
		else
		{
			if (dRat_Deck < dRat_Rib_Temp)
			{
				nRib_cr = i;
				dRat_Rib = dRat_Rib_Temp;
			}
		}
	}
	rData.nRib_cr = nRib_cr;
	rData.dRat_Rib = dRat_Rib;
	rData.bChk_Rib = bChk_Rib;


	// FB
	double dRat_FB = 0.0, dRat_FB_Temp = 0.0;
	BOOL bChk_FB = FALSE;
	int nFB_cr = 0;
	for (int i = 0; i < 3; ++i)
	{
		const STL_MEMB_RES_SNiP_STR_FB &FloorBeamD = StrnPartR.aFloorBeamD[i];
		if (FloorBeamD.bChk) bChk_FB = TRUE;
		dRat_FB_Temp = FloorBeamD.dRat;
		//dRat_FB      = max(dRat_FB, dRat_FB_Temp);
		if (i == 0)
		{
			nFB_cr = i;
			dRat_FB = dRat_FB_Temp;
		}
		else
		{
			if (dRat_FB < dRat_FB_Temp)
			{
				nFB_cr = i;
				dRat_FB = dRat_FB_Temp;
			}
		}
	}
	rData.nFB_cr = nFB_cr;
	rData.dRat_FB = dRat_FB;
	rData.bChk_FB = bChk_FB;

	double dRat[3] = { dRat_Deck, dRat_Rib, dRat_FB };
	double dRat_Max = 0.0;
	for (int i = 0; i < 3; ++i)
	{
		dRat_Max = max(dRat_Max, dRat[i]);
	}
	rData.dRat_Max = dRat_Max;
	rData.bOK = (dRat_Max <= 1.0) ? TRUE : FALSE;
	rData.bChk = (rData.bChk_Deck || rData.bChk_Rib || rData.bChk_FB) ? TRUE : FALSE;

}

void CDgnRUSStlCompDataCtrl::Convert_StrnLRibEngToCVL(STL_MEMB_RES_SNiP_STR_RIB &RibR, STL_MEMB_RES_SNiP_STR_DP &DeckR, T_SDLS_STRN_FLG &rData)
{
	rData.bChk = TRUE;
	rData.nABPoint = RibR.nABPoint;
	rData.dSigxc_r = RibR.dSigxc;
	rData.dSigxp_r = RibR.dSigxp;
	rData.dSigx_r = RibR.dSigx;
	rData.dRyr = RibR.dCapacity;
	rData.dRatioRib = RibR.dRat_Ry;
	rData.bOKrib = (rData.dRatioRib <= 1.0) ? TRUE : FALSE;
	rData.nAB1point = 0;  // ???
	rData.dSigx_d = DeckR.dSigx;
	rData.dSigy_d = DeckR.dSigy;
	rData.dTauxy_d = DeckR.dTauxy;
	rData.dSigma_d = DeckR.dSigx;
	rData.dRyd = DeckR.dCapacity;
	rData.dRatioDeck = DeckR.dRat_Com;
	rData.bOKdeck = (rData.bOKdeck <= 1.0) ? TRUE : FALSE;
	rData.dRatioMax = max(rData.dRatioRib, rData.dRatioDeck);
	rData.bOK = (rData.dRatioMax <= 1.0) ? TRUE : FALSE;

}

void CDgnRUSStlCompDataCtrl::Convert_BF_Str_WebPartEngToCVL(STL_MEMB_RES_SNiP_STR_WEB_CHK_UNIT &StrnWebR, T_SDPT_STRN_WEB &rData)
{
	rData.bChk = StrnWebR.bChk;

	// Rib
	int nRibSize = StrnWebR.aRibD.GetSize();
	rData.aWebChk.SetSize(nRibSize);
	double dRat[3] = { 0.0 };
	double dRatTemp = 0.0;
	BOOL bChk_Rib = FALSE;
	int nRib_cr = 0;
	for (int i = 0; i < nRibSize; ++i)
	{
		Convert_StrnWebPlateEngToCVL(StrnWebR.aRibD[i], rData.aWebChk[i]);
		dRatTemp = StrnWebR.aRibD[i].dRat;
		//dRat[0] = max(dRat[0], StrnWebR.aRibD[i].dRat);
		if (StrnWebR.aRibD[i].bChk) bChk_Rib = TRUE;

		if (i == 0)
		{
			nRib_cr = i;
			dRat[0] = dRatTemp;
		}
		else
		{
			if (dRat[0] < dRatTemp)
			{
				nRib_cr = i;
				dRat[0] = dRatTemp;
			}
		}

	}
	rData.nRib_cr = nRib_cr;
	rData.bChk_Rib = bChk_Rib;
	rData.dRat_Rib = dRat[0];


	//Deck - Shear/Von Mises
	dRat[1] = StrnWebR.WebNoMD.dRat;  // 4.30
	dRat[2] = StrnWebR.WebVonD.dRat;  // 4.31

	rData.bChk_DeckNoM = StrnWebR.bChk_NoM;
	rData.bChk_DeckVon = StrnWebR.bChk_Von;
	rData.bChk_Deck = (StrnWebR.bChk_NoM || StrnWebR.bChk_Von);
	rData.dRat_DeckNoM = dRat[1];
	rData.dRat_DeckVon = dRat[2];
	rData.dRat_Deck = max(dRat[1], dRat[2]);

	double dRat_max = 0.0;
	for (int i = 0; i < 3; ++i)
	{
		dRat_max = max(dRat_max, dRat[i]);
	}
	rData.bChk = (rData.bChk_Deck || rData.bChk_Rib) ? TRUE : FALSE;
	rData.dRat_Max = dRat_max;
	rData.bOK = (rData.dRat_Max <= 1.0) ? TRUE : FALSE;
}

void CDgnRUSStlCompDataCtrl::Convert_StrnWebPlateEngToCVL(STL_MEMB_RES_SNiP_STR_WEB_RIB &WebP, T_SDPS_STRN_WEB &rData)
{
	rData.bChk = TRUE;
	rData.dSigx = WebP.dSigx;
	rData.dSigy = WebP.dSigy;
	rData.dTauxy = WebP.dTauxy;
	rData.dSigsum = WebP.dDemand;

	rData.dRatioSig = WebP.dRat_Com;
	rData.dRatioTau = WebP.dRat_Sh;
	rData.dRatioMax = WebP.dRat;
	rData.bOK_Sigsum = (rData.dRatioSig <= 1.0) ? TRUE : FALSE;
	rData.bOK_Tauxy = (rData.dRatioTau <= 1.0) ? TRUE : FALSE;
	rData.bOK = (rData.dRatioMax <= 1.0) ? TRUE : FALSE;

}

void CDgnRUSStlCompDataCtrl::Convert_StabFlgPartEngToCVL(STL_MEMB_RES_SNiP_STA_DP_CHK_UNIT &StabPartR, T_SDPT_STAB_FLG &rData)
{
	// Rib
	int nRibSize = StabPartR.aLRib_Sta.GetSize();
	rData.aLSChk.SetSize(nRibSize);
	double dRat_Rib = 0.0;
	BOOL bChk_Rib = FALSE;
	int nRib_cr = 0;
	int nRibPart_cr = 0;
	for (int i = 0; i < nRibSize; ++i)
	{
		double dRat_Rib_Temp = 0.0;
		int nRibPart_cr_temp = 0;
		Convert_StabLRibEngToCVL(StabPartR.aLRib_Sta[i], rData.aLSChk[i], nRibPart_cr_temp, dRat_Rib_Temp, bChk_Rib);
		if (i == 0)
		{
			nRib_cr = i;
			dRat_Rib = dRat_Rib_Temp;
			nRibPart_cr = nRibPart_cr_temp;
		}
		else
		{
			if (dRat_Rib < dRat_Rib_Temp)
			{
				nRib_cr = i;
				dRat_Rib = dRat_Rib_Temp;
				nRibPart_cr = nRibPart_cr_temp;
			}
		}
	}
	rData.nRib_cr = nRib_cr;
	rData.nRibPan_cr = nRibPart_cr; //StabPartR.nIndexCriticalDP;

	// Deck - Global stability
	STL_MEMB_RES_SNiP_STA_GDP &GDeckPlate_Sta = StabPartR.GDeckPlate_Sta;
	BOOL bChk_DeckG = GDeckPlate_Sta.bChk;
	double dRat_DeckG = GDeckPlate_Sta.dRat;
	rData.nDeckG_cr = 0;


	// Deck - Local stability(BTR)
	int nDeckSize = StabPartR.aLDeckPlate_Sta.GetSize();
	rData.aPanChk.SetSize(nDeckSize);

	BOOL bChk_DeckL = FALSE;
	double dRat_DeckL = 0.0;
	int nDeckL_cr = 0;
	for (int i = 0; i < nDeckSize; ++i)
	{
		STL_MEMB_RES_SNiP_STA_LOCAL_BTR &LDeckPlate_Sta = StabPartR.aLDeckPlate_Sta[i];
		Convert_StabDeckEngToCVL(LDeckPlate_Sta, rData.aPanChk[i]);
		if (LDeckPlate_Sta.bChk) bChk_DeckL = TRUE;

		//dRat_DeckL = max(dRat_DeckL, LDeckPlate_Sta.dRat);
		if (i == 0)
		{
			nDeckL_cr = i;
			dRat_DeckL = LDeckPlate_Sta.dRat;
		}
		else
		{
			if (dRat_DeckL < LDeckPlate_Sta.dRat)
			{
				nDeckL_cr = i;
				dRat_DeckL = LDeckPlate_Sta.dRat;
			}
		}
	}
	rData.nDeckL_cr = nDeckL_cr;

	// Floor Beam - Global stability
	BOOL bChk_FBG = FALSE;
	double dRat_FBG = 0.0;
	int nFBG_cr = 0;
	for (int i = 0; i < 3; ++i) // C1, C2, C3 -> nIndexCriticalFB
	{
		const STL_MEMB_RES_SNiP_STA_FB &FloorBeam_Sta = StabPartR.aFloorBeam_Sta[i];

		if (FloorBeam_Sta.bChk) bChk_FBG = TRUE;

		//dRat_FBG = max(dRat_FBG, FloorBeam_Sta.dRat);
		if (i == 0)
		{
			nFBG_cr = i;
			dRat_FBG = FloorBeam_Sta.dRat;
		}
		else
		{
			if (dRat_FBG < FloorBeam_Sta.dRat)
			{
				nFBG_cr = i;
				dRat_FBG = FloorBeam_Sta.dRat;
			}
		}
	}
	rData.nFBG_cr = nFBG_cr; //StabPartR.nIndexCriticalFB;

	// Floor Beam - Local stability(BTR)
	BOOL bChk_FBL = FALSE;
	double dRat_FBL = 0.0;
	double dRat_FBLAll = 0.0;
	double dRat_FBLAll_temp = 0.0;
	int nFBSize = StabPartR.aLFB_Sta.GetSize();
	int nFBL_cr = 0;
	int nFBLPan_cr = 0;  // nFBL_cr 중 불리한 plate
	for (int i = 0; i < nFBSize; ++i)
	{
		if (!StabPartR.aLFB_Sta[i].bChk) { continue; }
		dRat_FBLAll_temp = 0.0;

		int nFBLPan_cr_temp = 0;
		int nFBPSize = StabPartR.aLFB_Sta[i].aRibPlateSTA.GetSize();
		for (int j = 0; j < nFBPSize; ++j)
		{
			if (!StabPartR.aLFB_Sta[i].aRibPlateSTA[j].bChk) { continue; }
			dRat_FBL = max(dRat_FBL, StabPartR.aLFB_Sta[i].aRibPlateSTA[j].dRat);
			bChk_FBL = TRUE;

			//dRat_FBLAll_temp = max(dRat_FBLAll_temp, StabPartR.aLFB_Sta[i].aRibPlateSTA[j].dRat);
			if (j == 0)
			{
				nFBLPan_cr_temp = j;
				dRat_FBLAll_temp = StabPartR.aLFB_Sta[i].aRibPlateSTA[j].dRat;
			}
			else
			{
				if (dRat_FBLAll_temp < StabPartR.aLFB_Sta[i].aRibPlateSTA[j].dRat)
				{
					nFBLPan_cr_temp = j;
					dRat_FBLAll_temp = StabPartR.aLFB_Sta[i].aRibPlateSTA[j].dRat;
				}
			}
		}

		if (i == 0)
		{
			nFBL_cr = i;
			dRat_FBLAll = dRat_FBLAll_temp;
			nFBLPan_cr = nFBLPan_cr_temp;
		}
		else
		{
			if (dRat_FBLAll < dRat_FBLAll_temp)
			{
				nFBL_cr = i;
				dRat_FBLAll = dRat_FBLAll_temp;
				nFBLPan_cr = nFBLPan_cr_temp;
			}
		}
	}
	rData.nFBL_cr = nFBL_cr; //StabPartR.nIndexCriticalFB_LSta;
	rData.nFBLPan_cr = nFBLPan_cr;

	// *************************************
	// 개선
	// *************************************
	rData.dRat_Rib = dRat_Rib;
	rData.bChk_Rib = bChk_Rib;

	rData.dRat_DeckG = dRat_DeckG;
	rData.dRat_DeckL = dRat_DeckL;
	rData.dRat_Deck = max(dRat_DeckG, dRat_DeckL);
	rData.bChk_DeckG = bChk_DeckG;
	rData.bChk_DeckL = bChk_DeckL;
	rData.bChk_Deck = (!bChk_DeckG && !bChk_DeckL) ? FALSE : TRUE;

	rData.dRat_FBG = dRat_FBG;
	rData.dRat_FBL = dRat_FBL;
	rData.dRat_FB = max(dRat_FBG, dRat_FBL);
	rData.bChk_FBG = bChk_FBG;
	rData.bChk_FBL = bChk_FBL;
	rData.bChk_FB = (!bChk_FBG && !bChk_FBL) ? FALSE : TRUE;

	// max
	double dRat[3] = { 0.0 };
	dRat[0] = rData.dRat_Rib;
	dRat[1] = rData.dRat_Deck;
	dRat[2] = rData.dRat_FB;

	double dRat_max = 0.0;
	for (int i = 0; i < 3; ++i)
	{
		dRat_max = max(dRat_max, dRat[i]);
	}

	rData.bChk = (rData.bChk_Deck || rData.bChk_Rib || rData.bChk_FB) ? TRUE : FALSE;
	rData.dRat_Max = dRat_max;
	rData.bOK = (dRat_max <= 1.0) ? TRUE : FALSE;
}


void CDgnRUSStlCompDataCtrl::Convert_StabLRibEngToCVL(STL_MEMB_RES_SNiP_STA_RIB_CHK_UNIT &RibR, T_SDSB_SECT_NOLS &rData, int &nRibPart_cr, double &dRat_Rib, BOOL &bChk_Rib)
{
	int nRibPartSize = RibR.aRibPlateSTA.GetSize();
	double dRat_Rib_max = 0.0;
	nRibPart_cr = 0;
	for (int i = 0; i < nRibPartSize; ++i)
	{
		if (i > 7) continue;
		T_SDSB_PAN &SdsbPan = rData.PanChk[i];
		SdsbPan.dBTRa = RibR.aRibPlateSTA[i].dBT_Rat_lim;
		SdsbPan.dRatio = RibR.aRibPlateSTA[i].dRat;
		SdsbPan.bChkBtr = (SdsbPan.dRatio <= 1.0) ? TRUE : FALSE;

		if (RibR.aRibPlateSTA[i].bChk) bChk_Rib = TRUE;
		//dRat_Rib_max = max(dRat_Rib_max, SdsbPan.dRatio);
		if (i == 0)
		{
			nRibPart_cr = i;
			dRat_Rib_max = SdsbPan.dRatio;
		}
		else
		{
			if (dRat_Rib_max < SdsbPan.dRatio)
			{
				nRibPart_cr = i;
				dRat_Rib_max = SdsbPan.dRatio;
			}
		}

	}
	dRat_Rib = dRat_Rib_max;

}

void CDgnRUSStlCompDataCtrl::Convert_StabDeckEngToCVL(STL_MEMB_RES_SNiP_STA_LOCAL_BTR &DeckR, T_SDSB_PAN &rData)
{
	rData.dBTRa = DeckR.dBT_Rat_lim;
	rData.dRatio = DeckR.dRat;
	rData.bChkBtr = (rData.dRatio <= 1.0) ? TRUE : FALSE;

}

void CDgnRUSStlCompDataCtrl::Convert_StabWebPartEngToCVL(STL_MEMB_RES_SNiP_STA_WEB &StabWebR, T_SDPT_STAB_WEB &rData)
{
	// Web_plate
	int nPanSize = StabWebR.aWebPlateD.GetSize();
	rData.aWebChk.SetSize(nPanSize);
	double dRat_Deck = 0.0;
	BOOL bChk_Deck = FALSE;
	for (int i = 0; i < nPanSize; ++i)
	{
		STL_MEMB_RES_SNiP_STA_WEB_PLATE &WebPlateD = StabWebR.aWebPlateD[i];
		Convert_StabWebPanEngToCVL(WebPlateD, rData.aWebChk[i]);

		dRat_Deck = max(dRat_Deck, WebPlateD.dRat);
		if (WebPlateD.bChk) bChk_Deck = TRUE;
	}
	rData.nPan_cr = 0; // ???


	// Web_Rib
	double dRat_Rib = 0.0;
	double dRat_RibAll = 0.0;
	BOOL bChk_Rib = FALSE;
	int nRib_cr = 0;
	int nRibSize = StabWebR.aLRib_Sta.GetSize();
	int nRibPan_cr = 0;
	for (int i = 0; i < nRibSize; ++i) // 리브 개수
	{
		STL_MEMB_RES_SNiP_STA_RIB_CHK_UNIT &LRib_Sta = StabWebR.aLRib_Sta.GetAt(i);

		int nSize = LRib_Sta.aRibPlateSTA.GetSize();
		int nRibPan_cr_temp = 0;
		for (int j = 0; j < nSize; ++j) // 리브에 포함된 plate 수
		{
			STL_MEMB_RES_SNiP_STA_LOCAL_BTR &RibPlateSTA = LRib_Sta.aRibPlateSTA.GetAt(j);

			//dRat_Rib = max(dRat_Rib, RibPlateSTA.dRat); 
			if (RibPlateSTA.bChk) bChk_Rib = TRUE;
			if (j == 0)
			{
				nRibPan_cr_temp = j;
				dRat_Rib = RibPlateSTA.dRat;
			}
			else
			{
				if (dRat_Rib < RibPlateSTA.dRat)
				{
					nRibPan_cr_temp = j;
					dRat_Rib = RibPlateSTA.dRat;
				}
			}
		}

		if (i == 0)
		{
			nRib_cr = i;
			dRat_RibAll = dRat_Rib;
			nRibPan_cr = nRibPan_cr_temp;
		}
		else
		{
			if (dRat_RibAll < dRat_Rib)
			{
				nRib_cr = i;
				dRat_RibAll = dRat_Rib;
				nRibPan_cr = nRibPan_cr_temp;
			}
		}
	}

	// *************************************
	// 개선
	// *************************************
	rData.nRib_cr = nRib_cr;
	rData.nRibPan_cr = nRibPan_cr;
	rData.dRat_Deck = dRat_Deck;
	rData.bChk_Deck = bChk_Deck;

	rData.dRat_Rib = dRat_RibAll;
	rData.bChk_Rib = bChk_Rib;

	rData.bChk = (rData.bChk_Deck || rData.bChk_Rib) ? TRUE : FALSE;

	rData.dRat_Max = max(dRat_Deck, dRat_Rib);
	rData.bOK = (rData.dRat_Max <= 1.0) ? TRUE : FALSE;
}

void CDgnRUSStlCompDataCtrl::Convert_StabWebPanEngToCVL(STL_MEMB_RES_SNiP_STA_WEB_PLATE &WebP, T_SDPS_STAB_WEB_PAN &rData)
{
	rData.bChk = WebP.bChk;
	rData.nAppItemType = WebP.nIndex;
	rData.nChkPlateType = WebP.nChkPlateType;

	rData.dSigx = WebP.dSigx;
	rData.dSigy = WebP.dSigy;
	rData.dTauxy = WebP.dTauxy;
	rData.dSigx_cr = WebP.dSigx_cr;
	rData.dSigy_cr = WebP.dSigy_cr;
	rData.dTauxy_cr = WebP.dTauxy_cr;
	rData.dComRat = WebP.dRat;
	rData.bOK = (rData.dComRat <= 1.0) ? TRUE : FALSE;
}
//////////////////////////////////////////////////////////////////////////
void CDgnRUSStlCompDataCtrl::Convert_StlComp_AF_Str_Concrete(STL_COMP_MEMB_RESD_SNiP_AFTER_STR &DgnResD, T_ACST_CONC &rData, BOOL &bStlCompCheck)
{
	rData.bCheck = DgnResD.ConcResD.bCheck;
	rData.bOK = DgnResD.ConcResD.bOK;
	rData.dEpsilon_b = DgnResD.ConcResD.dDemand;
	rData.dEpsilon_b_lim = DgnResD.ConcResD.dEpsilonb_lim;
	rData.dRatio = DgnResD.ConcResD.dRatio;
	rData.dSigma_b_Cri = DgnResD.CaseChkD.dSig_b_Cri;
	rData.dSigma_b_lim_Cri = DgnResD.CaseChkD.dmbRb_Cri;
	rData.dSigma_r_Cri = DgnResD.CaseChkD.dSig_r_Cri;
	rData.dSigma_r_lim_Cri = DgnResD.CaseChkD.dmrRr_Cri;
	rData.nCaseType = DgnResD.CaseChkD.nCaseType;
	rData.nPosNega = DgnResD.CaseChkD.bPositiveM ? 0 : 1;

	if (rData.bCheck)bStlCompCheck = TRUE;
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_AF_Str_Rebar(STL_COMP_MEMB_RESD_SNiP_AFTER_STR &DgnResD, T_ACST_REBAR &rData, BOOL &bStlCompCheck)
{
	rData.bCheck = DgnResD.RebarResD.bCheck;
	rData.bOK = DgnResD.RebarResD.bOK;
	rData.dRatio = DgnResD.RebarResD.dRatio;
	rData.dSigma_b_Cri = DgnResD.CaseChkD.dSig_b_Cri;
	rData.dSigma_b_lim_Cri = DgnResD.CaseChkD.d01mbRb_Cri;
	rData.dSigma_r = DgnResD.RebarResD.dSigma_r;
	rData.dSigma_r_lim = DgnResD.RebarResD.dmrRy;
	rData.nCaseType = DgnResD.CaseChkD.nCaseType;
	rData.nPosNega = DgnResD.CaseChkD.bPositiveM ? 0 : 1;

	if (rData.bCheck)bStlCompCheck = TRUE;
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_AF_Str_Flg(int nStype, BOOL bIsTop, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &DgnResD, T_ACST_SGFR_PART &rData, BOOL &bStlCompCheck)
{
	int nPartSize = 3;

	STL_COMP_FLXURE_CHK_CASE_SNiP_AFTER_STR& CaseChkD = DgnResD.CaseChkD;

	for (int i = 0; i < nPartSize; ++i) //L, M, R
	{
		STL_COMP_MEMB_RES_SNiP_AFTER_STR_DP_CHK_UNIT &StrnPartR = (bIsTop) ? DgnResD.TopStlGirderPart[i] : DgnResD.BotStlGirderPart[i];
		Convert_AF_StrnFlgPartEngToCVL(StrnPartR, CaseChkD, rData.StrnFlangePart[i]);
		if (rData.StrnFlangePart[i].bCheck) { bStlCompCheck = TRUE;  rData.bCheck = TRUE; }
	}
	rData.dCrRatio_Flg = max(rData.StrnFlangePart[0].dCrFlgRatio, max(rData.StrnFlangePart[1].dCrFlgRatio, rData.StrnFlangePart[2].dCrFlgRatio));
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_AF_Str_Web(int nStype, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &DgnResD, T_SDSN_WEB &rData, BOOL &bStlCompCheck)
{
	int nWebSize = 0;
	switch (nStype)
	{
	case D_SECT_TYPE_COMPO_STLG_I:
		nWebSize = 1; break;
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		nWebSize = 2; break;
	default: ASSERT(0); break;
	}

	for (int i = 0; i < nWebSize; ++i) //L, R
	{
		Convert_AF_StrnWebPartEngToCVL(DgnResD.WebMembD[i], rData.WebD[i]);
		if (rData.WebD[i].bChk) bStlCompCheck = TRUE;
	}
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_Fati_Conc(STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &DgnResD, T_FATI_CONC&rData, BOOL &bStlCompChecks)
{
	STL_COMP_CONC_FATI_RESD_SNiP_AFTER& ResConcD = DgnResD.ConcFatiResD;

	rData.bCheck = ResConcD.bCheck;
	rData.bOK = ResConcD.bOK;

	INT_PTR nCrIndex = ResConcD.nCrIndex;
	if (ResConcD.bCheck)
	{
		rData.dSigma_bf = ResConcD.ConcFatiResD[nCrIndex].dSig_bf;
		rData.dmbRb = ResConcD.ConcFatiResD[nCrIndex].dCapacity;
		rData.dRatio = ResConcD.ConcFatiResD[nCrIndex].dRatio;

		bStlCompChecks = TRUE;
	}
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_Fati_Rebar(STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &DgnResD, T_FATI_REBAR &rData, BOOL &bStlCompChecks)
{
	STL_COMP_REBAR_FATI_RESD_SNiP_AFTER& ResRebarD = DgnResD.RebarFatiResD;

	rData.bCheck = ResRebarD.bCheck;
	rData.bOK = ResRebarD.bOK;

	INT_PTR nCrIndex = ResRebarD.nCrIndex;
	if (ResRebarD.bCheck)
	{
		rData.dSigmap = ResRebarD.RebarFatiResD[nCrIndex].dSig_r;
		rData.dmsRs_lim = ResRebarD.RebarFatiResD[nCrIndex].dCapacity;
		rData.dRatio_3_94 = ResRebarD.RebarFatiResD[nCrIndex].dRatio;

		rData.dSigma_max_ef = ResRebarD.RebarFatiResD[nCrIndex].dSig_r;
		rData.dGamwRym_lim = ResRebarD.RebarFatiResD[nCrIndex].dCapacity;
		rData.dRatio_4_57 = ResRebarD.RebarFatiResD[nCrIndex].dRatio;

		rData.dRatio = ResRebarD.RebarFatiResD[nCrIndex].dRatio;

		bStlCompChecks = TRUE;
	}
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_Fati_StlGirder(STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &DgnResD, T_FATI_STL_GIRDER &rData, BOOL &bStlCompChecks)
{
	STL_COMP_STL_GIRDER_FATI_RESD_SNiP_AFTER& ResStlGirderD = DgnResD.StlGirderFatiResD;

	INT_PTR nCrIndex = ResStlGirderD.nCrIndex;
	if (ResStlGirderD.bCheck)
	{
		rData.bCheck = ResStlGirderD.bCheck;
		rData.bOK = ResStlGirderD.bOK;

		rData.dRatio = ResStlGirderD.StlGirderFatiResD[nCrIndex].dRat_Ax;

		rData.dSigmas_Top = ResStlGirderD.StlGirderFatiResD[nCrIndex].dSig_max_ef;
		rData.dGammawRy_Top = ResStlGirderD.StlGirderFatiResD[nCrIndex].dCapa_Ax;
		rData.dRatio_Top = ResStlGirderD.StlGirderFatiResD[nCrIndex].dRat_Ax;

		rData.dSigmas_Bot = ResStlGirderD.StlGirderFatiResD[nCrIndex].dSig_max_ef;
		rData.dGammawRy_Bot = ResStlGirderD.StlGirderFatiResD[nCrIndex].dCapa_Ax;
		rData.dRatio_Bot = ResStlGirderD.StlGirderFatiResD[nCrIndex].dRat_Ax;

		bStlCompChecks = TRUE;
	}
}

void CDgnRUSStlCompDataCtrl::Convert_StlComp_Serv_Conc(STL_COMP_MEMB_RESD_SNiP_AFTER_SERV &DgnResD, T_CRACK_CONC&rData, BOOL &bStlCompChecks)
{
	rData.bCheck = DgnResD.bCheck;
	rData.bOK = DgnResD.bOK;
	int nCrIndex = DgnResD.nCrIndex;
	if (DgnResD.bCheck)
	{
		rData.bCheck = DgnResD.aServResD[nCrIndex].bCheck;
		rData.bOK = DgnResD.aServResD[nCrIndex].bOK;
		rData.dRatio = DgnResD.aServResD[nCrIndex].dRatio;

		rData.bPositiveChk = DgnResD.aServResD[nCrIndex].bCheck;
		rData.dSigbf = DgnResD.aServResD[nCrIndex].dSig_bf;
		rData.dmbRb_mc2 = DgnResD.aServResD[nCrIndex].dCapacity;
		rData.dRatio_Po = DgnResD.aServResD[nCrIndex].dRatio_Po;

		rData.bNegativeChk = DgnResD.aServResD[nCrIndex].bCheck_Ne;
		rData.dSigma_cr = DgnResD.aServResD[nCrIndex].dSigma_s;
		rData.da_cr = DgnResD.aServResD[nCrIndex].da_cr;
		rData.dDelta_crd = DgnResD.aServResD[nCrIndex].dDelta_cr;
		rData.dRatio_Ne = DgnResD.aServResD[nCrIndex].dRatio_Ne;

		bStlCompChecks = TRUE;
	}
}

void CDgnRUSStlCompDataCtrl::Convert_ShearConnResD(STL_COMP_MEMB_RESD_SNiP_SHEAR_CONN& rInD, T_STRD_SHEAR_CONN& rData, BOOL &bStlCompCheck)
{
	rData.bCheck = rInD.bCheck;
	rData.bOK = rInD.bOK;
	rData.dRatio = rInD.dRatio;
	rData.GroupKey = rInD.ScgdK;
	rData.strGroupName = rInD.strGroupName;

	INT_PTR nElemSize = rInD.arGroupElemKey.GetSize();
	rData.arElemResD.SetSize(nElemSize);
	for (INT_PTR nElem = 0; nElem < nElemSize; nElem++)
	{
		T_SHEAR_CONN_ELEM_RESD ElemResD;
		ElemResD.ElemK = rInD.arGroupElemKey[nElem];
		ElemResD.dLongiSForceElem = rInD.aShearConnResD[nElem].dLongiSForceElem;
		ElemResD.dLongiSForce[0] = rInD.aShearConnResD[nElem].dLongiSForce[0];
		ElemResD.dLongiSForce[1] = rInD.aShearConnResD[nElem].dLongiSForce[1];

		rData.arElemResD.SetAt(nElem, ElemResD);
	}

	rData.StartElem = rInD.StartElemKey;
	rData.EndElem = rInD.EndElemKey;
	rData.dSlabAxialForce_Str = rInD.dLongiForce_Start;
	rData.dSlabAxialForce_End = rInD.dLongiForce_End;
	rData.dSlabAxialForce_Res = rInD.dLongiShearSum;

	rData.dGroupLength = rInD.dGroupLength;
	rData.dResistanceStud = rInD.dSrengthConn;
	rData.nNumofStud = rInD.nNum;
	rData.dReqSpace = rInD.dReqPitch;
	rData.dUserSpace = rInD.dUserPitch;
	rData.dRatio = rInD.dRatio;

	if (rInD.bCheck) bStlCompCheck = TRUE;
}

void CDgnRUSStlCompDataCtrl::Convert_AF_StrnFlgPartEngToCVL(STL_COMP_MEMB_RES_SNiP_AFTER_STR_DP_CHK_UNIT &StrnPartR,
	STL_COMP_FLXURE_CHK_CASE_SNiP_AFTER_STR& CaseTypeD, T_ACST_SGFR_ELEM &rData)
{
	// Deck
	rData.nCrFlgIndex = StrnPartR.nCrIndex4DP;
	rData.dCrFlgRatio = StrnPartR.dCrRatio_DP;
	rData.bChk_Deck = StrnPartR.bCheck_StlGirder;
	INT_PTR nDeckSize = StrnPartR.aStlGirderResD.GetSize();
	rData.aFlangeStrRes.SetSize(nDeckSize);
	for (INT_PTR nFlg = 0; nFlg < nDeckSize; ++nFlg)
	{
		Convert_AF_StrnFlgEngToCVL(StrnPartR.aStlGirderResD[nFlg], CaseTypeD, rData.aFlangeStrRes[nFlg]);
		if (rData.aFlangeStrRes[nFlg].bCheck) rData.bChk_Deck = TRUE;
	}

	// Rib
	rData.nCrRibIndex = StrnPartR.nCrIndex4Rib;
	rData.dCrRibRatio = StrnPartR.dCrRatio_Rib;
	rData.bChk_Rib = StrnPartR.bCheck_Rib;
	INT_PTR nRibSize = StrnPartR.aRibResD.GetSize();
	rData.aRibStrRes.SetSize(nRibSize);
	for (INT_PTR nRib = 0; nRib < nRibSize; ++nRib)
	{
		Convert_AF_StrnLRibEngToCVL(StrnPartR.aRibResD[nRib], rData.aRibStrRes[nRib]);
		if (rData.aRibStrRes[nRib].bChk) rData.bChk_Rib = TRUE;
	}

	rData.dCrRatio = StrnPartR.dCrRatio;
	rData.bOK = (rData.dCrRatio > 1.0) ? FALSE : TRUE;
	rData.bCheck = StrnPartR.bCheck;
}

void CDgnRUSStlCompDataCtrl::Convert_AF_StrnFlgEngToCVL(STL_COMP_STL_GIRDER_RESD_SNiP_AFTER_STR &FlgD, STL_COMP_FLXURE_CHK_CASE_SNiP_AFTER_STR& CaseTypeD, T_ACST_SGFR &rData)
{
	rData.bCheck = FlgD.bCheck;
	rData.bOK = FlgD.bOK;
	rData.bTopBot = FlgD.bTop;
	rData.nPosNega = FlgD.bIsPositiveM ? 0 : 1;
	rData.nCaseType = FlgD.nCaseType;
	rData.dSigma_b_Cri = FlgD.dSigb;
	rData.dSigma_b_lim_Cri;
	rData.dSigma_r_Cri;
	rData.dSigma_r_lim_Cri;
	rData.dRigidity_Cri;
	rData.dRigidity_lim_Cri;
	rData.dSigma = FlgD.dDemand;;
	rData.dSigma_lim = FlgD.dCapacity;
	rData.dRatio = FlgD.dRatio;
}

void CDgnRUSStlCompDataCtrl::Convert_AF_StrnLRibEngToCVL(STL_MEMB_RES_SNiP_STR_RIB &RibR, T_SDLS_STRN_FLG &rData)
{
	rData.bChk = TRUE;
	rData.nABPoint = RibR.nABPoint;
	rData.dSigxc_r = RibR.dSigxc;
	rData.dSigxp_r = RibR.dSigxp;
	rData.dSigx_r = RibR.dSigx;
	rData.dRyr = RibR.dCapacity;
	rData.dRatioRib = RibR.dRat_Ry;
	rData.bOKrib = (rData.dRatioRib <= 1.0) ? TRUE : FALSE;
	rData.nAB1point = 0;  // ???
	//     rData.dSigx_d    = DeckR.dSigx;
	//     rData.dSigy_d    = DeckR.dSigy;
	//     rData.dTauxy_d   = DeckR.dTauxy;
	//     rData.dSigma_d   = DeckR.dSigx;
	//     rData.dRyd       = DeckR.dCapacity;
	//     rData.dRatioDeck = DeckR.dRat_Com;
	rData.bOKdeck = (rData.bOKdeck <= 1.0) ? TRUE : FALSE;
	rData.dRatioMax = max(rData.dRatioRib, rData.dRatioDeck);
	rData.bOK = (rData.dRatioMax <= 1.0) ? TRUE : FALSE;
}

void CDgnRUSStlCompDataCtrl::Convert_AF_StrnWebPartEngToCVL(STL_COMP_MEMB_RES_SNiP_STR_WEB_CHK_UNIT& DgnResD, T_SDPT_STRN_WEB& rData)
{
	// Rib
	INT_PTR nRibSize = DgnResD.aRibD.GetSize();
	rData.aWebChk.SetSize(nRibSize);
	int nRib_cr = 0;
	for (INT_PTR i = 0; i < nRibSize; ++i)
	{
		Convert_StrnWebPlateEngToCVL(DgnResD.aRibD[i], rData.aWebChk[i]);
	}
	rData.nRib_cr = DgnResD.nCr_rib;
	rData.bChk_Rib = DgnResD.bChk_Rib;
	rData.dRat_Rib = DgnResD.dRat_Cr_Rib;
	rData.bOK_Rib = DgnResD.dRat_Cr_Rib > 1.0 ? FALSE : TRUE;

	//Deck - Shear/Von Mises / Comb
	rData.bChk_DeckNoM = DgnResD.bChk_NoM;
	rData.dRat_DeckNoM = DgnResD.dRat_Cr_NoM;
	rData.bOK_DeckNoM = DgnResD.dRat_Cr_NoM > 1.0 ? FALSE : TRUE;

	rData.bChk_DeckVon = DgnResD.bChk_Von;
	rData.dRat_DeckVon = DgnResD.dRat_Cr_Von;
	rData.bOK_DeckVon = DgnResD.dRat_Cr_Von > 1.0 ? FALSE : TRUE;

	rData.bChk_DeckCom = DgnResD.bChk_Com;
	rData.dRat_DeckCom = DgnResD.dRat_Cr_Com;
	rData.bOK_DeckCom = DgnResD.dRat_Cr_Com > 1.0 ? FALSE : TRUE;

	rData.bChk_DeckMean = DgnResD.bChk_Mean;
	rData.dRat_Mean = DgnResD.dRat_Cr_Mean;
	rData.bOK_DeckMean = DgnResD.dRat_Cr_Mean > 1.0 ? FALSE : TRUE;

	rData.bChk_Deck = (DgnResD.bChk_NoM || DgnResD.bChk_Von || DgnResD.bChk_Com || DgnResD.bChk_Mean);
	rData.dRat_Deck = max(max(DgnResD.dRat_Cr_NoM, rData.dRat_Mean), max(DgnResD.dRat_Cr_Von, DgnResD.dRat_Cr_Com));
	rData.bOK_Deck = (rData.bOK_DeckNoM &&  rData.bOK_DeckVon &&  rData.bOK_DeckCom);

	rData.bChk = (rData.bChk_Deck || rData.bChk_Rib);
	rData.dRat_Max = max(rData.dRat_Deck, rData.dRat_Rib);
	rData.bOK = (rData.bOK_Rib && rData.bOK_Deck);

	rData.bChk = DgnResD.bChk;
}


//////////////////////////////////////////////////////////////////////////

BOOL CDgnRUSStlCompDataCtrl::IsNeedBeforeChk(const STL_COMP_CHK_POSD_SNiP& crDgnSubPosD)
{
	for (int i = 0; i < 3; ++i)
	{
		if (crDgnSubPosD.bTopDeckPlateChk[i]) return TRUE;
		if (crDgnSubPosD.bBotDeckPlateChk[i]) return TRUE;
	}

	for (int nWeb = 0; nWeb < 2; ++nWeb)
	{
		if (crDgnSubPosD.bWebChk[nWeb]) return TRUE;
	}

	return FALSE;
}

BOOL CDgnRUSStlCompDataCtrl::IsReplaceStfnIndex(int nStype, int nRefPos, int nDeckPos, int nDeckPart)
{
	BOOL bIsReplace = FALSE;
	if (nStype == D_SECT_TYPE_COMPO_STLG_I)
	{
		// Deck Plate
		if ((nDeckPos == 0 || nDeckPos == 2) && nDeckPart == 0)
		{
			bIsReplace = (nRefPos == 0);
		}
		else if ((nDeckPos == 0 || nDeckPos == 2) && nDeckPart == 1)
		{
			bIsReplace = (nRefPos == 1);
		}

		// Web
		if (nDeckPos == 1)
		{
			bIsReplace = (nRefPos == 1);
		}
	}
	else if (nStype == D_SECT_TYPE_COMPO_STLG_B || nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		if ((nDeckPos == 0 || nDeckPos == 3) && nDeckPart == 0)
		{
			bIsReplace = (nRefPos == 0);
		}
		else if ((nDeckPos == 0 || nDeckPos == 3) && (nDeckPart == 1 || nDeckPart == 2))
		{
			bIsReplace = (nRefPos == 1);
		}
		// Web
		if ((nDeckPos == 1 || nDeckPos == 2))
		{
			bIsReplace = (nRefPos == 1);
		}
	}
	else { ASSERT(0); }


	return bIsReplace;
}

BOOL CDgnRUSStlCompDataCtrl::IsLcomExist(T_LCOM_K LcomK, CArray<T_LCOM_K, T_LCOM_K&> & aLcomList)
{
	INT_PTR nSize = aLcomList.GetSize();
	for (INT_PTR i = 0; i < nSize; ++i)
	{
		if (LcomK == aLcomList[i]) return TRUE;
	}

	return FALSE;
}

BOOL CDgnRUSStlCompDataCtrl::IsValidSectType(T_ELEM_K ElemK)
{
	T_ELEM_D ElemD;
	if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD)) return FALSE;

	int nSectType = m_pDoc->m_pAttrCtrl->GetRealSectType(ElemD.elpro);
	switch (nSectType)
	{
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		return TRUE;
	default:
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::IsNAinSteelGirder(const STL_COMP_SECT_POSD& crSectD)
{
	if (crSectD.AfterCompSect_Pure.dy1 < 0.0) { return FALSE; } // 중립축이 Slab 내에 위치하는 경우, 설계 Skip, DK & PCS. 15.05.28
	if (crSectD.AfterCompSect_Posi.dy1 < 0.0) { return FALSE; }
	if (crSectD.AfterCompSect_Nega.dy1 < 0.0) { return FALSE; }
	if (crSectD.AfterCompSect_2ndG_Posi.dy1 < 0.0) { return FALSE; }
	if (crSectD.AfterCompSect_2ndG_Nega.dy1 < 0.0) { return FALSE; }

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::IsExistScfp4Lcom(int nLcomType, int nOrgLcom, bool& bExisCrp, bool& bExisShr, bool& bExisBST)
{
	m_pForcCtrl->Get_SfcpExist(nLcomType, nOrgLcom, bExisCrp, bExisShr, bExisBST);
	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::CalcStiffnessOfStiffener(T_SECT_STIFF_D &Stfn, ST_SECT_STIFF_D &rData)
{
	int nStfnShapeSize = Stfn.aStiffShape.GetSize();
	int nConvStfnSize = rData.aStiffShape.GetSize();
	if (nStfnShapeSize < 1) return FALSE;
	if (nStfnShapeSize != nConvStfnSize) return FALSE;

	//arData.SetSize(nStfnShapeSize);
	T_SECT_D CalcS; CalcS.Initialize();
	for (int i = 0; i < nStfnShapeSize; ++i)
	{
		T_STIFF_SHAPE_D &StfnD = Stfn.aStiffShape[i];

		T_SECT_STIFFNESS &StfnStiff = CalcS.SectBefore.SectI.Stiffness;
		CalcS.nStype = D_SECT_TYPE_REGULAR;
		if (StfnD.nType == 0) CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_SB;
		else if (StfnD.nType == 1) CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_T;
		else if (StfnD.nType == 2) CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_URIB;
		else ASSERT(0);
		for (int j = 0; j < 8; ++j) CalcS.SectBefore.SectI.Size[j] = StfnD.dSize[j];

		CSectUtil::CalcSectData(CalcS);

		ConvertSectStiffnessData(StfnStiff, rData.aStiffShape[i].Stiffness);
	}

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::PrintStlCompGirderReport()
{
	BOOL bPrintOK = FALSE;

	m_pDoc->SetDesignStatus(7);

	m_pDoc = CDBDoc::GetDocPoint();
	m_nDgnCode = m_pDoc->GetCivilCodeDgn();
	m_pStlCompDesign = m_pDoc->m_pPostCtrl->GetRUSStlCompGirderDesign();

	int nDgnCode = m_pDoc->GetCivilCodeDgn();
	InitialData(nDgnCode);

	BOOL bDgnElemExist = TRUE;
	BOOL bPrintElemExist = TRUE;
	BOOL bPrintShearConnExist = TRUE;

	// design element
	CArray<T_ELEM_K, T_ELEM_K> arElemK; arElemK.RemoveAll();
	CDBLib::GetElemforStlCompGirderGirder(arElemK);
	if (arElemK.GetSize() == 0)
	{
		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("출력할 요소의 설계정보가 없습니다.")
		AfxMessageBox(strErrMessage);

		bDgnElemExist = FALSE;
	}

	// output element
	CArray<T_CGRE_K, T_CGRE_K> aOutElemKey; aOutElemKey.RemoveAll();
	m_pDoc->m_pAttrCtrl2->GetCgreKeyList(aOutElemKey);
	INT_PTR nOutElemSize = aOutElemKey.GetSize();
	CArray<T_CGRE_K, T_CGRE_K> aPrintElemKey; aPrintElemKey.RemoveAll();
	for (INT_PTR nOutElem = 0; nOutElem < nOutElemSize; nOutElem++)
	{
		T_ELEM_K ElemK = aOutElemKey[nOutElem];
		if (!IsValidSectType(ElemK)) continue;
		aPrintElemKey.Add(ElemK);
	}
	INT_PTR nPrintSize = aPrintElemKey.GetSize();

	if (nPrintSize == 0)
	{
		bPrintElemExist = FALSE;
	}

	// design Shear Connector Group
	CArray<T_SCGD_K, T_SCGD_K> arDgnShearConnGroupD;
	m_pDoc->m_pAttrCtrl2->GetScgdKeyList(arDgnShearConnGroupD);
	INT_PTR nDngShearConnGNum = arDgnShearConnGroupD.GetSize();
	if (nDngShearConnGNum == 0)
	{
		bPrintShearConnExist = FALSE;
	}

	if (nPrintSize == 0 && nDngShearConnGNum == 0)
	{
		CString strErrMessage;
		if (nPrintSize == 0) { strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER);    AfxMessageBox(strErrMessage); } // _T("출력할 요소의 설계정보가 없습니다.")
		if (nDngShearConnGNum == 0) { strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER);  AfxMessageBox(strErrMessage); } // _T("출력할 전단연결재 그룹의 설계정보가 없습니다.")
	}

	if ((bDgnElemExist && bPrintElemExist) || bPrintShearConnExist) // 설계 정보가 있거나 설계 출력 요소가 있는 경우 혹은 전단연결재 설계 정보가 있는 경우
	{
		Make_DesignBaseData();
	}
	else { return TRUE; }

	if (!m_pStlCompManager) ASSERT(0);

	STL_COMP_RPT_ELEM_RES_UNIT DgnElemResD;
	Convert_GlobalDgnParamD(DgnElemResD.InD.GlobalDgnD); // Get Global Design Param. Data
	m_pStlCompManager->Get_StlCompTableValue(DgnElemResD.InD.TBval); //  Get Table Data 

	BOOL bChkStr = DgnElemResD.InD.GlobalDgnD.bDgnCheck[0];
	BOOL bChkStb = DgnElemResD.InD.GlobalDgnD.bDgnCheck[1];
	BOOL bChkFati = DgnElemResD.InD.GlobalDgnD.bDgnCheck[2];
	BOOL bChkServ = DgnElemResD.InD.GlobalDgnD.bDgnCheck[4];
	BOOL bChkShearConn = DgnElemResD.InD.GlobalDgnD.bDgnCheck[3];
	BOOL bChkKappaF = bChkStr || bChkStb || bChkFati || bChkServ;

	STL_COMP_RPT_RES_TOTAL RptResD; // 최종 계산 결과 구조체
	RptResD.nDgnCode = GetConvertCSGCodeCvlToEng(nDgnCode);

	T_RSCD_ITEM ChkItem;
	m_pStlCompDesign->GetStlCompCheckItem(ChkItem);
	BOOL bChkShearCoonRes = ChkItem.bRSCCheckAfter[9];

	// Make Report Data for Element Design Result Data
	for (INT_PTR nPrinElem = 0; nPrinElem < nPrintSize; ++nPrinElem)
	{
		T_ELEM_K ElemK = aPrintElemKey[nPrinElem];

		// Design I/J 
		BOOL bChkIJ[2] = { FALSE };
		T_SDDP_D SddpD; SddpD.Initialize();
		if (!m_pDoc->m_pAttrCtrl2->GetSddp(ElemK, SddpD)) continue;

		// Output I/J
		T_CGRE_D CgreD;
		if (!m_pDoc->m_pAttrCtrl2->GetCgre(ElemK, CgreD)) continue;

		// 검토 여부
		bChkIJ[0] = (SddpD.aSddpSub[0].GetSize() > 0 && (CgreD.iDgnPosi == 0 || CgreD.iDgnPosi == 2));
		bChkIJ[1] = (SddpD.aSddpSub[1].GetSize() > 0 && (CgreD.iDgnPosi == 1 || CgreD.iDgnPosi == 2));

		T_ELEM_D ElemD;
		if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD)) continue;

        // Get DgnLength
        T_EDLK_D DgnLengD;
        m_mDgnLengD.Lookup(ElemK, DgnLengD);        

		for (int nIJ = 0; nIJ < 2; ++nIJ)
		{
			if (!bChkIJ[nIJ]) continue;
			DgnElemResD.BF_STR_ResD.Initialize();
			DgnElemResD.BF_STB_ResD.Initialize();
			DgnElemResD.AF_Kappa_Chk_ResD.Initialize();
			DgnElemResD.AF_STR_ResD.Initialize();
			DgnElemResD.AF_STB_ResD.Initialize();
			DgnElemResD.AF_Fati_ResD.Initialize();
			DgnElemResD.AF_Serv_ResD.Initialize();

			GetDgnBaseD4Elem_IJ(ElemK, ElemD, nIJ, DgnElemResD.InD);

            // Make Before Base D
            DgnElemResD.BF_BaseD.Initialize();
			Convert_Stl_CompDgnLengthData(DgnLengD, DgnElemResD.BF_BaseD);
            m_pStlCompManager->Make_StlCompBeforeBaseData(DgnElemResD.InD, DgnElemResD.BF_BaseD);
            // Get Before Composite Check Force 
            if(bChkStr || bChkStb)
            {
                GetRptD_BeforeResD(ElemK, nIJ, DgnElemResD.InD,  DgnElemResD.BF_BaseD, DgnElemResD.BF_STR_ResD, DgnElemResD.BF_STB_ResD);
            }

            // Make After Base 
            DgnElemResD.AF_BaseD.Initialize();
			Convert_Stl_CompDgnLengthData(DgnLengD, DgnElemResD.AF_BaseD);
            DgnElemResD.AF_BaseD.BeforeBaseD = DgnElemResD.BF_BaseD;
            m_pStlCompManager->Make_StlCompAfterBaseData(DgnElemResD.InD, DgnElemResD.AF_BaseD);

			if (bChkKappaF) // Get Kappa Factor Check Data
			{
				GetRptD_KappaChkResD(ElemK, nIJ, DgnElemResD.InD, DgnElemResD.AF_BaseD, DgnElemResD.AF_Kappa_Chk_ResD);
			}

			if (bChkStr || bChkStb || bChkFati || bChkServ) // Get All Result for after composite check result
			{
				GetRptD_AfterResD(ElemK, nIJ, DgnElemResD.InD, DgnElemResD.AF_BaseD, DgnElemResD);
			}
			RptResD.arElemRptD.Add(DgnElemResD);
		}
	}

	// Make Report Data of Design Shear Connector Group
	if (bChkShearConn & bChkShearCoonRes)
	{
		STL_COMP_MEMB_POSD_SNiP MembInD4SCG; // for Shear Connector Group
		Convert_GlobalDgnParamD(MembInD4SCG.GlobalDgnD);
		for (INT_PTR nShearConnG = 0; nShearConnG < nDngShearConnGNum; ++nShearConnG)
		{
			T_SCGD_K ScgdK = arDgnShearConnGroupD[nShearConnG];
			T_SCGD_D ScgdD;
			if (!m_pDoc->m_pAttrCtrl2->GetScgd(ScgdK, ScgdD)) { ASSERT(0); continue; }
			STL_COMP_RPT_SHEAR_CONN_GROUP_RES_UNIT ShearConnGResD;

			if (!m_mStlCompShearConnectorD.Lookup(ScgdK, ShearConnGResD.ShearConnGroup_InD)) continue;
			MembInD4SCG.ShearConnD = ShearConnGResD.ShearConnGroup_InD;
			GetRptD_ShearConnGroupResD(ScgdK, MembInD4SCG, ShearConnGResD.ShearConnGroupI_BaseD, ShearConnGResD.ShearConnGroupI_ResD);
			RptResD.arShearConnGorupRptD.Add(ShearConnGResD);
		}
	}

	// Get Unit Data & File Path
	int nSelUnit = 0;

	switch (nDgnCode)
	{
	case SNiP_20503_84_CSG:
	case SP_35_13330_11_CSG:
		break;
	default:  ASSERT(0); return FALSE;    break;
	}
	int nEngineDgnCode = GetConvertCSGCodeCvlToEng(m_nDgnCode);

#if defined(_CIVIL_RUS)
	CString strBaseFileDir = _T("");
	strBaseFileDir = m_pDoc->GetProgramPath() + _T("\\Excel Base File\\RUS\\");
#else 
	CString strBaseFileDir = _T("");
	strBaseFileDir = m_pDoc->GetProgramPath() + _T("\\Excel Base File\\US\\");
#endif
	CString strSaveFileName = GetSaveFileNameEx();
	CString strProgramDir = m_pDoc->GetProgramPath();

	BOOL bVersionOK = FALSE;
	BOOL bRegression = CLockMgr::Get_IsRegressionTest();

	//   //  임시, 지워야 함 , 박창신
	//   RptResD.arElemRptD.SetSize(1);
	//   RptResD.arShearConnGorupRptD.SetSize(1);

	// 계산서 출력
	bPrintOK = m_pStlCompManager->Print_StlCompDetailReport_RUS(nSelUnit, nEngineDgnCode, strProgramDir, strBaseFileDir, strSaveFileName, RptResD, bVersionOK, bRegression);

	return bPrintOK;
}

CString CDgnRUSStlCompDataCtrl::GetSaveFileNameEx(BOOL bWithoutExtension /* = FALSE*/)
{
	CString strFileName = m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strFileName);

	CString strSaveFileName;
	//strSaveFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".xls");
	strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + _T(".xlsx");

	CFileCtrl* saveFileCtrl = new CFileCtrl(strSaveFileName);

	int count = 0;
	while (saveFileCtrl->FileExists())
	{
		CString sCount;
		sCount.Format(_T("%d"), ++count);

		CString strSaveFileName;
		//strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + sCount + _T(".xls");
		strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + sCount + _T(".xlsx");

		delete saveFileCtrl;
		saveFileCtrl = new CFileCtrl(strSaveFileName);
	}

	if (bWithoutExtension)
		strSaveFileName = saveFileCtrl->GetFileNameWithoutExtension();
	else
		strSaveFileName = saveFileCtrl->GetFilePathAndName();

	delete saveFileCtrl;

	return strSaveFileName;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_BeforeResD(T_ELEM_K ElemK, int &nChkPosIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_BEFORE_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &rDgnStrResD, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD)
{
	rMemb.CheckLcomD.GlobalForceD.SetSize(1); // Use Only One

	T_RSCD_ITEM ChkItem;
	m_pStlCompDesign->GetStlCompCheckItem(ChkItem);
	BOOL bChkStr = (ChkItem.bRSCCheckBefore[0] || ChkItem.bRSCCheckBefore[1] || ChkItem.bRSCCheckBefore[2]);
	BOOL bChkStb = (ChkItem.bRSCCheckBefore[3] || ChkItem.bRSCCheckBefore[4] || ChkItem.bRSCCheckBefore[5] || ChkItem.bRSCCheckBefore[6]);

	if (rMemb.GlobalDgnD.bDgnCheck[0] && bChkStr)
	{
		GetRptD_BeforeStrResD(ElemK, nChkPosIJ, rMemb, rMembBase, rDgnStrResD);
	}

	if (rMemb.GlobalDgnD.bDgnCheck[1] && bChkStb)
	{
		GetRptD_BeforeStaResD(ElemK, nChkPosIJ, rMemb, rMembBase, rDgnStaResD);
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_BeforeStrResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_BEFORE_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &rDgnStrResD)
{
	// Flange Part
	GetRptD_BeforeStrFlgResD(ElemK, nIJ, rMemb, rMembBase, rDgnStrResD);

	// Web Part
	GetRptD_BeforeStrWebResD(ElemK, nIJ, rMemb, rMembBase, rDgnStrResD);

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_BeforeStrFlgResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_BEFORE_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &rDgnStrResD)
{
	// Top Flange(9)
	// - L : Deck, Rib, FB
	// - M : Deck, Rib, FB
	// - R : Deck, Rib, FB
	rMemb.CheckLcomD.GlobalForceD.SetSize(1); // 한 개로만 적용
	//******************
	// Top Flange
	//******************
	T_SDPT_STRN_FLG SDSN_Result;
	// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
	for (int nPart = 0; nPart < 3; ++nPart)
	{
		BOOL bChk = FALSE;
		// Flange : Deck
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStn_FLG = rMemb;
		Get_RptForceD_BeforeStrRes_Flg(ElemK, nIJ, TRUE, nPart, SN_DECK, MembLcomDStn_FLG, SDSN_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STR StrnD_FLG_STR_Deck;
		m_pStlCompManager->Check_StlCompBeforeStrength(MembLcomDStn_FLG, rMembBase, StrnD_FLG_STR_Deck);
		rDgnStrResD.DeckPMembD_Top[nPart].aDeckPlateD.Copy(StrnD_FLG_STR_Deck.DeckPMembD_Top[nPart].aDeckPlateD);
		rDgnStrResD.DeckPMembD_Top[nPart].nCrIndex4DP = StrnD_FLG_STR_Deck.DeckPMembD_Top[nPart].nCrIndex4DP;
		if (SDSN_Result.bChk_Deck)		bChk = TRUE;

		// Flange : Rib
		STL_COMP_MEMB_POSD_SNiP&  MembLcomDStn_Rib = rMemb;
		Get_RptForceD_BeforeStrRes_Flg(ElemK, nIJ, TRUE, nPart, SN_RIB, MembLcomDStn_Rib, SDSN_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STR StrnD_FLG_STR_Rib;
		m_pStlCompManager->Check_StlCompBeforeStrength(MembLcomDStn_Rib, rMembBase, StrnD_FLG_STR_Rib);
		rDgnStrResD.DeckPMembD_Top[nPart].aResRibD.Copy(StrnD_FLG_STR_Rib.DeckPMembD_Top[nPart].aResRibD);
		rDgnStrResD.DeckPMembD_Top[nPart].nCrIndex4Rib = StrnD_FLG_STR_Rib.DeckPMembD_Top[nPart].nCrIndex4Rib;
		if (SDSN_Result.bChk_Rib) { bChk = TRUE; }

		// Flange : Floor Beam
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStn_FB = rMemb;
		Get_RptForceD_BeforeStrRes_Flg(ElemK, nIJ, TRUE, nPart, SN_FLOORBEAM, MembLcomDStn_FB, SDSN_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STR StrnD_TopF_STR_FB;
		m_pStlCompManager->Check_StlCompBeforeStrength(MembLcomDStn_FB, rMembBase, StrnD_TopF_STR_FB);
		for (int nFB_top = 0; nFB_top < 3; ++nFB_top)
		{
			rDgnStrResD.DeckPMembD_Top[nPart].aFloorBeamD[nFB_top] = StrnD_TopF_STR_FB.DeckPMembD_Top[nPart].aFloorBeamD[nFB_top];
		}
		rDgnStrResD.DeckPMembD_Top[nPart].nCrIndex4FB = StrnD_TopF_STR_FB.DeckPMembD_Top[nPart].nCrIndex4FB;
		if (SDSN_Result.bChk_FB) { bChk = TRUE; }

		rDgnStrResD.DeckPMembD_Top[nPart].bChk = bChk;
	}

	// Bottom Flange(9)
	// - L : Deck, Rib, FB
	// - M : Deck, Rib, FB
	// - R : Deck, Rib, FB

	//******************
	// Bottom Flange 
	//******************
	for (int nPart = 0; nPart < 3; ++nPart)
	{
		// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
		BOOL bChk = FALSE;

		// Flange : Deck
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStn_FLG = rMemb;
		Get_RptForceD_BeforeStrRes_Flg(ElemK, nIJ, FALSE, nPart, SN_DECK, MembLcomDStn_FLG, SDSN_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STR StrnD_FLG_STR_Deck;
		m_pStlCompManager->Check_StlCompBeforeStrength(MembLcomDStn_FLG, rMembBase, StrnD_FLG_STR_Deck);
		rDgnStrResD.DeckPMembD_Bot[nPart].aDeckPlateD.Copy(StrnD_FLG_STR_Deck.DeckPMembD_Bot[nPart].aDeckPlateD);
		rDgnStrResD.DeckPMembD_Bot[nPart].nCrIndex4DP = StrnD_FLG_STR_Deck.DeckPMembD_Bot[nPart].nCrIndex4DP;
		if (SDSN_Result.bChk_Deck)		bChk = TRUE;

		// Flange : Rib
		STL_COMP_MEMB_POSD_SNiP&  MembLcomDStn_Rib = rMemb;
		Get_RptForceD_BeforeStrRes_Flg(ElemK, nIJ, FALSE, nPart, SN_RIB, MembLcomDStn_Rib, SDSN_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STR StrnD_FLG_STR_Rib;
		m_pStlCompManager->Check_StlCompBeforeStrength(MembLcomDStn_Rib, rMembBase, StrnD_FLG_STR_Rib);
		rDgnStrResD.DeckPMembD_Bot[nPart].aResRibD.Copy(StrnD_FLG_STR_Rib.DeckPMembD_Bot[nPart].aResRibD);
		rDgnStrResD.DeckPMembD_Bot[nPart].nCrIndex4Rib = StrnD_FLG_STR_Rib.DeckPMembD_Bot[nPart].nCrIndex4Rib;
		if (SDSN_Result.bChk_Rib) { bChk = TRUE; }

		// Flange : Floor Beam
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStn_FB = rMemb;
		Get_RptForceD_BeforeStrRes_Flg(ElemK, nIJ, FALSE, nPart, SN_FLOORBEAM, MembLcomDStn_FB, SDSN_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STR StrnD_BotF_STR_FB;
		m_pStlCompManager->Check_StlCompBeforeStrength(MembLcomDStn_FB, rMembBase, StrnD_BotF_STR_FB);
		for (int nFB_top = 0; nFB_top < 3; ++nFB_top)
		{
			rDgnStrResD.DeckPMembD_Bot[nPart].aFloorBeamD[nFB_top] = StrnD_BotF_STR_FB.DeckPMembD_Bot[nPart].aFloorBeamD[nFB_top];
		}
		rDgnStrResD.DeckPMembD_Bot[nPart].nCrIndex4FB = StrnD_BotF_STR_FB.DeckPMembD_Bot[nPart].nCrIndex4FB;
		if (SDSN_Result.bChk_FB) { bChk = TRUE; }

		rDgnStrResD.DeckPMembD_Bot[nPart].bChk = bChk;
	}
	return TRUE;
}

BOOL  CDgnRUSStlCompDataCtrl::GetRptD_BeforeStrWebResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_BEFORE_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &rDgnStrResD)
{
	// Web(6)
	// - L : Deck1, Deck2, Rib
	// - R : Deck1, Deck2, Rib

	//******************
	// Web
	//******************
	T_SDPT_STRN_WEB SDSN_ResultWeb;
	for (int nLeft = 0; nLeft < 2; ++nLeft) // 0=Left, 1: Right
	{
		BOOL bChk = FALSE;
		BOOL bLeft = (nLeft == 0) ? TRUE : FALSE;
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStn_Web_NoM = rMemb;
		Get_RptForceD_BeforeStrRes_Web(ElemK, nIJ, bLeft, SN_WEB_NoM, MembLcomDStn_Web_NoM, SDSN_ResultWeb);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STR StrnStabD_WEB_STR_NoM;
		m_pStlCompManager->Check_StlCompBeforeStrength(MembLcomDStn_Web_NoM, rMembBase, StrnStabD_WEB_STR_NoM);
		rDgnStrResD.WebMembD[nLeft].WebNoMD = StrnStabD_WEB_STR_NoM.WebMembD[nLeft].WebNoMD;
		rDgnStrResD.WebMembD[nLeft].nChkCase = (SDSN_ResultWeb.bChk_DeckNoM) ? 0 : 1;
		if (SDSN_ResultWeb.bChk_DeckNoM) { bChk = TRUE; }

		STL_COMP_MEMB_POSD_SNiP& MembLcomDStn_Web_Von = rMemb;
		Get_RptForceD_BeforeStrRes_Web(ElemK, nIJ, bLeft, SN_WEB_VON, MembLcomDStn_Web_Von, SDSN_ResultWeb);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STR StrnStabD_WEB_STR_VON;
		m_pStlCompManager->Check_StlCompBeforeStrength(MembLcomDStn_Web_Von, rMembBase, StrnStabD_WEB_STR_VON);
		rDgnStrResD.WebMembD[nLeft].WebVonD = StrnStabD_WEB_STR_VON.WebMembD[nLeft].WebVonD;
		if (SDSN_ResultWeb.bChk_DeckVon) { bChk = TRUE; }

		STL_COMP_MEMB_POSD_SNiP&  MembLcomDStn_Web_Rib = rMemb;
		Get_RptForceD_BeforeStrRes_Web(ElemK, nIJ, bLeft, SN_WEB_RIB, MembLcomDStn_Web_Rib, SDSN_ResultWeb);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STR StrnStabD_WEB_STR_Rib;
		m_pStlCompManager->Check_StlCompBeforeStrength(MembLcomDStn_Web_Rib, rMembBase, StrnStabD_WEB_STR_Rib);
		rDgnStrResD.WebMembD[nLeft].aRibD.Copy(StrnStabD_WEB_STR_Rib.WebMembD[nLeft].aRibD);
		rDgnStrResD.WebMembD[nLeft].nCr_rib = SDSN_ResultWeb.nRib_cr;
		if (SDSN_ResultWeb.bChk_Rib) { bChk = TRUE; }

		rDgnStrResD.WebMembD[nLeft].bChk = bChk;
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_BeforeStaResD(T_ELEM_K ElemK, int &nChkPosIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_BEFORE_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD)
{
	// Get Global Lateral torsional Buckling
	GetRptD_BeforeStaLTBResD(ElemK, nChkPosIJ, rMemb, rMembBase, rDgnStaResD);

	// Get Flange Stability
	GetRptD_BeforeStaFlgResD(ElemK, nChkPosIJ, rMemb, rMembBase, rDgnStaResD);

	// Get Web Stability
	GetRptD_BeforeStaWebResD(ElemK, nChkPosIJ, rMemb, rMembBase, rDgnStaResD);

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_BeforeStaLTBResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_BEFORE_BASE_SNiP &pMembBase,
	STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD)
{
	//******************
	// Global Stability 
	//******************
	T_SDSB_GD SDSB_GDR;
	STL_COMP_MEMB_POSD_SNiP MembLcomDStb_GD = rMemb;
	Get_RptForceD_BeforeStaRes_LTB(ElemK, nIJ, MembLcomDStb_GD, SDSB_GDR);

	STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_GD;
	m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_GD, pMembBase, StrnStabD_GD);
	rDgnStaResD.GlobalSTA_LTB = StrnStabD_GD.GlobalSTA_LTB;

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_BeforeStaFlgResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_BEFORE_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD)
{
	// Top Flange(15)
	// - L : Deck1, Deck2, Rib, FB1, FB2
	// - M : Deck1, Deck2, Rib, FB1, FB2
	// - R : Deck1, Deck2, Rib, FB1, FB2

	//******************
	// Top Flange 
	//******************
	T_SDPT_STAB_FLG SDSB_Result;
	for (int nPart = 0; nPart < 3; ++nPart)
	{
		// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
		BOOL bChk = FALSE;

		// Flange : DeckG
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_GFLG = rMemb;
		Set_RptForceD_BeforeStaRes_Flange(ElemK, nIJ, TRUE, nPart, SB_DECK_G, MembLcomDStb_GFLG, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_DeckG;
		m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_GFLG, rMembBase, StrnStabD_FLG_STA_DeckG);
		rDgnStaResD.DeckPMembD_Top[nPart].GDeckPlate_Sta = StrnStabD_FLG_STA_DeckG.DeckPMembD_Top[nPart].GDeckPlate_Sta;
		rDgnStaResD.DeckPMembD_Top[nPart].nIndexCriticalDP_G = SDSB_Result.nDeckG_cr;
		if (SDSB_Result.bChk_DeckG) { bChk = TRUE; }

		// Flange : DeckL
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_FLG = rMemb;
		Set_RptForceD_BeforeStaRes_Flange(ElemK, nIJ, TRUE, nPart, SB_DECK_L, MembLcomDStb_FLG, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_DeckL;
		m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_FLG, rMembBase, StrnStabD_FLG_STA_DeckL);
		rDgnStaResD.DeckPMembD_Top[nPart].aLDeckPlate_Sta.Copy(StrnStabD_FLG_STA_DeckG.DeckPMembD_Top[nPart].aLDeckPlate_Sta);
		rDgnStaResD.DeckPMembD_Top[nPart].nIndexCriticalDP_L = SDSB_Result.nDeckL_cr;
		if (SDSB_Result.bChk_DeckL) { bChk = TRUE; }

		// Flange : Rib
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_Rib = rMemb;
		Set_RptForceD_BeforeStaRes_Flange(ElemK, nIJ, TRUE, nPart, SB_RIB, MembLcomDStb_Rib, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_Rib;
		m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_Rib, rMembBase, StrnStabD_FLG_STA_Rib);
		rDgnStaResD.DeckPMembD_Top[nPart].aLRib_Sta.Copy(StrnStabD_FLG_STA_Rib.DeckPMembD_Top[nPart].aLRib_Sta);
		rDgnStaResD.DeckPMembD_Top[nPart].nIndexCriticalRib = SDSB_Result.nRib_cr;
		if (SDSB_Result.bChk_Rib) { bChk = TRUE; }

		// Flange : FloorBeam_G
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_FBG = rMemb;
		Set_RptForceD_BeforeStaRes_Flange(ElemK, nIJ, TRUE, nPart, SB_FLOORBEAM_G, MembLcomDStb_FBG, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_FBG;
		m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_FBG, rMembBase, StrnStabD_FLG_STA_FBG);
		rDgnStaResD.DeckPMembD_Top[nPart].aFloorBeam_Sta[SDSB_Result.nFBG_cr] = StrnStabD_FLG_STA_FBG.DeckPMembD_Top[nPart].aFloorBeam_Sta[SDSB_Result.nFBG_cr];
		rDgnStaResD.DeckPMembD_Top[nPart].nIndexCriticalFB = SDSB_Result.nFBG_cr;
		if (SDSB_Result.bChk_FBG) { bChk = TRUE; }

		// Flange : FloorBeam_L
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_FLL = rMemb;
		Set_RptForceD_BeforeStaRes_Flange(ElemK, nIJ, TRUE, nPart, SB_FLOORBEAM_L, MembLcomDStb_FLL, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_FBL;
		m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_FLL, rMembBase, StrnStabD_FLG_STA_FBL);

		rDgnStaResD.DeckPMembD_Top[nPart].aLFB_Sta.Copy(StrnStabD_FLG_STA_FBL.DeckPMembD_Top[nPart].aLFB_Sta);
		rDgnStaResD.DeckPMembD_Top[nPart].nIndexCriticalFB_LSta = SDSB_Result.nFBL_cr;
		if (SDSB_Result.bChk_FBL) { bChk = TRUE; }

		rDgnStaResD.DeckPMembD_Top[nPart].bChk = bChk;
	}

	// Bottom Flange(15)
	// - L : Deck1, Deck2, Rib, FB1, FB2
	// - M : Deck1, Deck2, Rib, FB1, FB2
	// - R : Deck1, Deck2, Rib, FB1, FB2

	//******************
	// Bottom Flange 
	//******************
	for (int nPart = 0; nPart < 3; ++nPart)
	{
		// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
		BOOL bChk = FALSE;

		// Flange : DeckG
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_GFLG = rMemb;
		Set_RptForceD_BeforeStaRes_Flange(ElemK, nIJ, FALSE, nPart, SB_DECK_G, MembLcomDStb_GFLG, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_DeckG;
		m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_GFLG, rMembBase, StrnStabD_FLG_STA_DeckG);
		rDgnStaResD.DeckPMembD_Bot[nPart].GDeckPlate_Sta = StrnStabD_FLG_STA_DeckG.DeckPMembD_Bot[nPart].GDeckPlate_Sta;
		rDgnStaResD.DeckPMembD_Bot[nPart].nIndexCriticalDP_G = SDSB_Result.nDeckG_cr;
		if (SDSB_Result.bChk_DeckG) { bChk = TRUE; }

		// Flange : DeckL
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_FLG = rMemb;
		Set_RptForceD_BeforeStaRes_Flange(ElemK, nIJ, FALSE, nPart, SB_DECK_L, MembLcomDStb_FLG, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_DeckL;
		m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_FLG, rMembBase, StrnStabD_FLG_STA_DeckL);
		rDgnStaResD.DeckPMembD_Bot[nPart].aLDeckPlate_Sta.Copy(StrnStabD_FLG_STA_DeckG.DeckPMembD_Bot[nPart].aLDeckPlate_Sta);
		rDgnStaResD.DeckPMembD_Bot[nPart].nIndexCriticalDP_L = SDSB_Result.nDeckL_cr;
		if (SDSB_Result.bChk_DeckL) { bChk = TRUE; }

		// Flange : Rib
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_Rib = rMemb;
		Set_RptForceD_BeforeStaRes_Flange(ElemK, nIJ, FALSE, nPart, SB_RIB, MembLcomDStb_Rib, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_Rib;
		m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_Rib, rMembBase, StrnStabD_FLG_STA_Rib);
		rDgnStaResD.DeckPMembD_Bot[nPart].aLRib_Sta.Copy(StrnStabD_FLG_STA_Rib.DeckPMembD_Bot[nPart].aLRib_Sta);
		rDgnStaResD.DeckPMembD_Bot[nPart].nIndexCriticalRib = SDSB_Result.nRib_cr;
		if (SDSB_Result.bChk_Rib) { bChk = TRUE; }

		// Flange : FloorBeam_G
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_FBG = rMemb;
		Set_RptForceD_BeforeStaRes_Flange(ElemK, nIJ, FALSE, nPart, SB_FLOORBEAM_G, MembLcomDStb_FBG, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_FBG;
		m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_FBG, rMembBase, StrnStabD_FLG_STA_FBG);
		rDgnStaResD.DeckPMembD_Bot[nPart].aFloorBeam_Sta[SDSB_Result.nFBG_cr] = StrnStabD_FLG_STA_FBG.DeckPMembD_Bot[nPart].aFloorBeam_Sta[SDSB_Result.nFBG_cr];
		rDgnStaResD.DeckPMembD_Bot[nPart].nIndexCriticalFB = SDSB_Result.nFBG_cr;
		if (SDSB_Result.bChk_FBG) { bChk = TRUE; }

		// Flange : FloorBeam_L
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_FLL = rMemb;
		Set_RptForceD_BeforeStaRes_Flange(ElemK, nIJ, FALSE, nPart, SB_FLOORBEAM_L, MembLcomDStb_FLL, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_FBL;
		m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_FLL, rMembBase, StrnStabD_FLG_STA_FBL);

		rDgnStaResD.DeckPMembD_Bot[nPart].aLFB_Sta.Copy(StrnStabD_FLG_STA_FBL.DeckPMembD_Bot[nPart].aLFB_Sta);
		rDgnStaResD.DeckPMembD_Bot[nPart].nIndexCriticalFB_LSta = SDSB_Result.nFBL_cr;
		if (SDSB_Result.bChk_FBL) { bChk = TRUE; }

		rDgnStaResD.DeckPMembD_Bot[nPart].bChk = bChk;
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_BeforeStaWebResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_BEFORE_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD)
{
	// Web(6)
	// - L : Deck, Rib
	// - R : Deck, Rib

	//******************
	// Web 
	//******************
	STL_MEMB_POSD_SNiP_OD MembLcomDStb_Web;
	T_SDPT_STAB_WEB SDSB_ResultWeb;
	for (int nLeft = 0; nLeft < 2; ++nLeft)
	{
		BOOL bChk = FALSE;
		BOOL bLeft = (nLeft == 0) ? TRUE : FALSE;

		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_WebDeck = rMemb;
		Set_RptForceD_BeforeStaRes_Web(ElemK, nIJ, bLeft, SB_WEB_DECK, MembLcomDStb_WebDeck, SDSB_ResultWeb);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_WEB_STA_Deck;
		m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_WebDeck, rMembBase, StrnStabD_WEB_STA_Deck);

		rDgnStaResD.aResWebStaD[nLeft] = StrnStabD_WEB_STA_Deck.aResWebStaD[nLeft];
		rDgnStaResD.aResWebStaD[nLeft].aLRib_Sta.RemoveAll();
		if (StrnStabD_WEB_STA_Deck.aResWebStaD[nLeft].aWebPlateD.GetSize() > 0)
		{
			bChk = StrnStabD_WEB_STA_Deck.aResWebStaD[nLeft].aWebPlateD[0].bChk;
		}

		STL_COMP_MEMB_POSD_SNiP&  MembLcomDStb_WebRib = rMemb;
		Set_RptForceD_BeforeStaRes_Web(ElemK, nIJ, bLeft, SB_WEB_RIB, MembLcomDStb_WebRib, SDSB_ResultWeb);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_WEB_STA_Rib;
		m_pStlCompManager->Check_StlCompBeforeStability(MembLcomDStb_WebRib, rMembBase, StrnStabD_WEB_STA_Rib);

		rDgnStaResD.aResWebStaD[nLeft].aLRib_Sta.Copy(StrnStabD_WEB_STA_Rib.aResWebStaD[nLeft].aLRib_Sta);
		rDgnStaResD.aResWebStaD[nLeft].nWebRibCrIndex = SDSB_ResultWeb.nRib_cr;
		if (SDSB_ResultWeb.bChk_Rib) { bChk = TRUE; }

		rDgnStaResD.aResWebStaD[nLeft].bChk = bChk;
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_KappaChkResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	T_PLASTIC_LIM_CR PlasticLim_CR;   // 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompPlasticLimitStrainCrResult(ElemK, PlasticLim_CR)) PlasticLim_CR.Initialize();
	T_PLASTIC_LIM_BASE &PlasticLim_Base = PlasticLim_CR.PlasticLimStrainCr[ix];    //0=I 1=J

	// Get Critical LcomD
	//////////////////////////////////////////////////////////////////////////
	int nLcomNum = m_pForcCtrl->Get_LcomDataCount();
	rMemb.CheckLcomD.GlobalForceD.SetSize(nLcomNum);
	for (INT_PTR nLcom = 0; nLcom < nLcomNum; ++nLcom)  // After Composite Check
	{
		int nDgnLcomNo = nLcom + 1;

		// Get Force
		T_SCGD_DGN_FORCE ForD;
		m_pStlCompDesign->GetStlCompLcomForce(ElemK, nDgnLcomNo, ForD);
		ConvertStlCompGForce2Engine(ForD.Elem[nIJ], rMemb.CheckLcomD.GlobalForceD[nLcom]);
	}
	//ConvertStlCompForce2Engine(ElemK, nIJ, 0, PlasticLim_Base.nDgnLcomK,  rMemb);

	STL_COMP_MEMB_RESD_SNiP_KAPPA PlasticLimResD;
	m_pStlCompManager->Check_StlCompKappaCheck(rMemb, rMembBase, PlasticLimResD);
	rDgnKappaChkResD = PlasticLimResD;

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_RPT_ELEM_RES_UNIT &rDgnResD)
{
	rMemb.CheckLcomD.GlobalForceD.SetSize(1); // Use Only One

	T_RSCD_ITEM ChkItem;
	m_pStlCompDesign->GetStlCompCheckItem(ChkItem);

	BOOL bChkStr = (ChkItem.bRSCCheckAfter[0] || ChkItem.bRSCCheckAfter[1] || ChkItem.bRSCCheckAfter[2] || ChkItem.bRSCCheckAfter[3] || ChkItem.bRSCCheckAfter[4]);
	BOOL bChkStb = (ChkItem.bRSCCheckAfter[5] || ChkItem.bRSCCheckAfter[6]);
	BOOL bChkFati = ChkItem.bRSCCheckAfter[7];
	BOOL bChkServ = ChkItem.bRSCCheckAfter[8];

	// Get Strength Check Result
	if (rMemb.GlobalDgnD.bDgnCheck[0] && bChkStr)
	{
		GetRptD_AfterStrResD(ElemK, nIJ, rMemb, rMembBase, rDgnResD.AF_Kappa_Chk_ResD, rDgnResD.AF_STR_ResD);
	}

	// Get Strength Check Result
	if (rMemb.GlobalDgnD.bDgnCheck[1] && bChkStb)
	{
		GetRptD_AfterStaResD(ElemK, nIJ, rMemb, rMembBase, rDgnResD.AF_Kappa_Chk_ResD, rDgnResD.AF_STB_ResD);
	}

	// Get Fatigue Check Result
	if (rMemb.GlobalDgnD.bDgnCheck[2] && bChkFati)
	{
		GetRptD_AfterFaitResD(ElemK, nIJ, rMemb, rMembBase, rDgnResD.AF_Kappa_Chk_ResD, rDgnResD.AF_Fati_ResD);
	}

	// Get Serviceability Check Result
	if (rMemb.GlobalDgnD.bDgnCheck[4] && bChkServ)
	{
		GetRptD_AfterServResD(ElemK, nIJ, rMemb, rMembBase, rDgnResD.AF_Kappa_Chk_ResD, rDgnResD.AF_Serv_ResD);
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterStrResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &rDgnResD)
{
	// Get Concrete Check Result
	GetRptD_AfterStrConcResD(ElemK, nIJ, rMemb, rMembBase, rDgnKappaChkResD, rDgnResD);

	// Get Rebar Check Result
	GetRptD_AfterStrRebarResD(ElemK, nIJ, rMemb, rMembBase, rDgnKappaChkResD, rDgnResD);

	// Get Flange Check Result
	GetRptD_AfterStrFlgResD(ElemK, nIJ, rMemb, rMembBase, rDgnKappaChkResD, rDgnResD);

	// Get Web Check Result
	GetRptD_AfterStrWebResD(ElemK, nIJ, rMemb, rMembBase, rDgnKappaChkResD, rDgnResD);

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterStrConcResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &rDgnStrResD)
{
	//******************
	// Concrete Strength
	//******************
	T_ACST_CONC STR_Conc;
	STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_Conc = rMemb;
	Get_RptForceD_AfterStrRes_Conc(ElemK, nIJ, MembLcomDStb_Conc, STR_Conc);

	STL_COMP_MEMB_RESD_SNiP_AFTER_STR StrnD_Conc;
	m_pStlCompManager->Check_StlCompAfterStrength(MembLcomDStb_Conc, rMembBase, rDgnKappaChkResD, StrnD_Conc);
	rDgnStrResD.ConcResD = StrnD_Conc.ConcResD;
	rDgnStrResD.bCheck_Conc = StrnD_Conc.ConcResD.bCheck;
	rDgnStrResD.CaseConcreteChkD = StrnD_Conc.CaseConcreteChkD;

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterStrRebarResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &rDgnStrResD)
{
	//******************
	// Rebar Strength
	//******************
	T_ACST_REBAR STR_Rebar;
	STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_Rebar = rMemb;
	Get_RptForceD_AfterStrRes_Rebar(ElemK, nIJ, MembLcomDStb_Rebar, STR_Rebar);

	STL_COMP_MEMB_RESD_SNiP_AFTER_STR StrnD_Rebar;
	m_pStlCompManager->Check_StlCompAfterStrength(MembLcomDStb_Rebar, rMembBase, rDgnKappaChkResD, StrnD_Rebar);
	rDgnStrResD.RebarResD = StrnD_Rebar.RebarResD;
	rDgnStrResD.bCheck_Rebar = StrnD_Rebar.RebarResD.bCheck;
	rDgnStrResD.CaseRebarChkD = StrnD_Rebar.CaseRebarChkD;

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterStrFlgResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &rDgnStrResD)
{
	// Top Flange(9)
	// - L : Deck, Rib, FB
	// - M : Deck, Rib, FB
	// - R : Deck, Rib, FB
	rMemb.CheckLcomD.GlobalForceD.SetSize(1); // 한 개로만 적용
	//******************
	// Top Flange
	//******************
	T_ACST_SGFR_ELEM SDSN_Result;
	// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
	for (int nPart = 0; nPart < 3; ++nPart)
	{
		BOOL bChk = FALSE;
		// Flange : Deck
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStn_FLG = rMemb;
		Get_RptForceD_AfterStrRes_Flg(ElemK, nIJ, TRUE, nPart, SN_DECK, MembLcomDStn_FLG, SDSN_Result);
		STL_COMP_MEMB_RESD_SNiP_AFTER_STR StrnD_FLG_STR_Deck;
		m_pStlCompManager->Check_StlCompAfterStrength(MembLcomDStn_FLG, rMembBase, rDgnKappaChkResD, StrnD_FLG_STR_Deck);
		rDgnStrResD.TopStlGirderPart[nPart].aStlGirderResD.Copy(StrnD_FLG_STR_Deck.TopStlGirderPart[nPart].aStlGirderResD);
		rDgnStrResD.TopStlGirderPart[nPart].nCrIndex4DP = StrnD_FLG_STR_Deck.TopStlGirderPart[nPart].nCrIndex4DP;
		rDgnStrResD.CaseTopFlangeChkD = StrnD_FLG_STR_Deck.CaseTopFlangeChkD;
		if (SDSN_Result.bChk_Deck)		bChk = TRUE;

		// Flange : Rib
		STL_COMP_MEMB_POSD_SNiP&  MembLcomDStn_Rib = rMemb;
		Get_RptForceD_AfterStrRes_Flg(ElemK, nIJ, TRUE, nPart, SN_RIB, MembLcomDStn_Rib, SDSN_Result);
		STL_COMP_MEMB_RESD_SNiP_AFTER_STR StrnD_FLG_STR_Rib;
		m_pStlCompManager->Check_StlCompAfterStrength(MembLcomDStn_Rib, rMembBase, rDgnKappaChkResD, StrnD_FLG_STR_Rib);
		rDgnStrResD.TopStlGirderPart[nPart].aRibResD.Copy(StrnD_FLG_STR_Rib.TopStlGirderPart[nPart].aRibResD);
		rDgnStrResD.TopStlGirderPart[nPart].nCrIndex4Rib = StrnD_FLG_STR_Rib.TopStlGirderPart[nPart].nCrIndex4Rib;
		if (SDSN_Result.bChk_Rib) { bChk = TRUE; }

		rDgnStrResD.TopStlGirderPart[nPart].bCheck = bChk;
	}

	// Bottom Flange(9)
	// - L : Deck, Rib, FB
	// - M : Deck, Rib, FB
	// - R : Deck, Rib, FB

	//******************
	// Bottom Flange 
	//******************
	for (int nPart = 0; nPart < 3; ++nPart)
	{
		// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
		BOOL bChk = FALSE;

		// Flange : Deck
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStn_FLG = rMemb;
		Get_RptForceD_AfterStrRes_Flg(ElemK, nIJ, FALSE, nPart, SN_DECK, MembLcomDStn_FLG, SDSN_Result);
		STL_COMP_MEMB_RESD_SNiP_AFTER_STR StrnD_FLG_STR_Deck;
		m_pStlCompManager->Check_StlCompAfterStrength(MembLcomDStn_FLG, rMembBase, rDgnKappaChkResD, StrnD_FLG_STR_Deck);
		rDgnStrResD.BotStlGirderPart[nPart].aStlGirderResD.Copy(StrnD_FLG_STR_Deck.BotStlGirderPart[nPart].aStlGirderResD);
		rDgnStrResD.BotStlGirderPart[nPart].nCrIndex4DP = StrnD_FLG_STR_Deck.BotStlGirderPart[nPart].nCrIndex4DP;
		rDgnStrResD.CaseBottomFlangeChkD = StrnD_FLG_STR_Deck.CaseBottomFlangeChkD;
		if (SDSN_Result.bChk_Deck)		bChk = TRUE;

		// Flange : Rib
		STL_COMP_MEMB_POSD_SNiP&  MembLcomDStn_Rib = rMemb;
		Get_RptForceD_AfterStrRes_Flg(ElemK, nIJ, FALSE, nPart, SN_RIB, MembLcomDStn_Rib, SDSN_Result);
		STL_COMP_MEMB_RESD_SNiP_AFTER_STR StrnD_FLG_STR_Rib;
		m_pStlCompManager->Check_StlCompAfterStrength(MembLcomDStn_Rib, rMembBase, rDgnKappaChkResD, StrnD_FLG_STR_Rib);
		rDgnStrResD.BotStlGirderPart[nPart].aRibResD.Copy(StrnD_FLG_STR_Rib.BotStlGirderPart[nPart].aRibResD);
		rDgnStrResD.BotStlGirderPart[nPart].nCrIndex4Rib = StrnD_FLG_STR_Rib.BotStlGirderPart[nPart].nCrIndex4Rib;
		if (SDSN_Result.bChk_Rib) { bChk = TRUE; }

		rDgnStrResD.BotStlGirderPart[nPart].bCheck = bChk;
	}
	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterStrWebResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &rDgnStrResD)
{
	// Web(6)
	// - L : Deck1, Deck2, Rib
	// - R : Deck1, Deck2, Rib

	//******************
	// Web
	//******************
	T_SDPT_STRN_WEB SDSN_ResultWeb;
	for (int nLeft = 0; nLeft < 2; ++nLeft) // 0=Left, 1: Right
	{
		BOOL bChk = FALSE;
		BOOL bLeft = (nLeft == 0) ? TRUE : FALSE;
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStn_Web_NoM = rMemb;
		Get_RptForceD_AfterStrRes_Web(ElemK, nIJ, bLeft, SN_WEB_NoM, MembLcomDStn_Web_NoM, SDSN_ResultWeb);
		STL_COMP_MEMB_RESD_SNiP_AFTER_STR StrnStabD_WEB_STR_NoM;
		m_pStlCompManager->Check_StlCompAfterStrength(MembLcomDStn_Web_NoM, rMembBase, rDgnKappaChkResD, StrnStabD_WEB_STR_NoM);
		rDgnStrResD.WebMembD[nLeft].WebNoMD = StrnStabD_WEB_STR_NoM.WebMembD[nLeft].WebNoMD;
		rDgnStrResD.WebMembD[nLeft].bChk_NoM = StrnStabD_WEB_STR_NoM.WebMembD[nLeft].WebNoMD.bChk;
		rDgnStrResD.WebMembD[nLeft].nChkCase = (SDSN_ResultWeb.bChk_DeckNoM) ? 0 : 1;
		if (SDSN_ResultWeb.bChk_DeckNoM) { bChk = TRUE; }

		STL_COMP_MEMB_POSD_SNiP& MembLcomDStn_Web_Von = rMemb;
		Get_RptForceD_AfterStrRes_Web(ElemK, nIJ, bLeft, SN_WEB_VON, MembLcomDStn_Web_Von, SDSN_ResultWeb);
		STL_COMP_MEMB_RESD_SNiP_AFTER_STR StrnStabD_WEB_STR_VON;
		m_pStlCompManager->Check_StlCompAfterStrength(MembLcomDStn_Web_Von, rMembBase, rDgnKappaChkResD, StrnStabD_WEB_STR_VON);
		rDgnStrResD.WebMembD[nLeft].WebVonD = StrnStabD_WEB_STR_VON.WebMembD[nLeft].WebVonD;
		rDgnStrResD.WebMembD[nLeft].bChk_Von = StrnStabD_WEB_STR_VON.WebMembD[nLeft].WebVonD.bCheck;
		if (SDSN_ResultWeb.bChk_DeckVon) { bChk = TRUE; }

		STL_COMP_MEMB_POSD_SNiP& MembLcomDStn_Web_Com = rMemb;
		Get_RptForceD_AfterStrRes_Web(ElemK, nIJ, bLeft, SN_WEB_COM, MembLcomDStn_Web_Com, SDSN_ResultWeb);
		STL_COMP_MEMB_RESD_SNiP_AFTER_STR StrnStabD_WEB_STR_Com;
		m_pStlCompManager->Check_StlCompAfterStrength(MembLcomDStn_Web_Com, rMembBase, rDgnKappaChkResD, StrnStabD_WEB_STR_Com);
		rDgnStrResD.WebMembD[nLeft].WebCombineD = StrnStabD_WEB_STR_Com.WebMembD[nLeft].WebCombineD;
		rDgnStrResD.WebMembD[nLeft].bChk_Com = StrnStabD_WEB_STR_Com.WebMembD[nLeft].WebCombineD.bChk;
		if (SDSN_ResultWeb.bChk_DeckCom) { bChk = TRUE; }

		STL_COMP_MEMB_POSD_SNiP& MembLcomDStn_Web_Mean = rMemb;
		Get_RptForceD_AfterStrRes_Web(ElemK, nIJ, bLeft, SN_WEB_MEAN, MembLcomDStn_Web_Mean, SDSN_ResultWeb);
		STL_COMP_MEMB_RESD_SNiP_AFTER_STR StrnStabD_WEB_STR_Mean;
		m_pStlCompManager->Check_StlCompAfterStrength(MembLcomDStn_Web_Mean, rMembBase, rDgnKappaChkResD, StrnStabD_WEB_STR_Mean);
		rDgnStrResD.WebMembD[nLeft].WebMeanD = StrnStabD_WEB_STR_Mean.WebMembD[nLeft].WebMeanD;
		rDgnStrResD.WebMembD[nLeft].bChk_Mean = StrnStabD_WEB_STR_Mean.WebMembD[nLeft].WebMeanD.bChk;
		if (SDSN_ResultWeb.bChk_DeckMean) { bChk = TRUE; }

		STL_COMP_MEMB_POSD_SNiP&  MembLcomDStn_Web_Rib = rMemb;
		Get_RptForceD_AfterStrRes_Web(ElemK, nIJ, bLeft, SN_WEB_RIB, MembLcomDStn_Web_Rib, SDSN_ResultWeb);
		STL_COMP_MEMB_RESD_SNiP_AFTER_STR StrnStabD_WEB_STR_Rib;
		m_pStlCompManager->Check_StlCompAfterStrength(MembLcomDStn_Web_Rib, rMembBase, rDgnKappaChkResD, StrnStabD_WEB_STR_Rib);
		rDgnStrResD.WebMembD[nLeft].aRibD.Copy(StrnStabD_WEB_STR_Rib.WebMembD[nLeft].aRibD);
		rDgnStrResD.WebMembD[nLeft].nCr_rib = SDSN_ResultWeb.nRib_cr;
		rDgnStrResD.WebMembD[nLeft].bChk_Rib = SDSN_ResultWeb.bChk_Rib;
		if (SDSN_ResultWeb.bChk_Rib) { bChk = TRUE; }

		rDgnStrResD.WebMembD[nLeft].bChk = bChk;
	}

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterStaResD(T_ELEM_K ElemK, int &nChkPosIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnResD)
{
	// Get After Composite Stability Flange
	GetRptD_AfterStaFlgResD(ElemK, nChkPosIJ, rMemb, rMembBase, rDgnKappaChkResD, rDgnResD);

	// Get After Composite Stability Web
	GetRptD_AfterStaWebResD(ElemK, nChkPosIJ, rMemb, rMembBase, rDgnKappaChkResD, rDgnResD);

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterStaFlgResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD)
{
	//******************
	// Bottom Flange 
	//******************
	T_SDPT_STAB_FLG SDSB_Result;
	for (int nPart = 0; nPart < 3; ++nPart)
	{
		// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
		BOOL bChk = FALSE;
		const BOOL bIsTop = FALSE; // After의 경우 Bottom Flange 만 검토함

		// Flange : DeckG
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_GFLG = rMemb;
		Get_RptForceD_AfterStaRes_Flange(ElemK, nIJ, bIsTop, nPart, SB_DECK_G, MembLcomDStb_GFLG, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_DeckG;
		m_pStlCompManager->Check_StlCompAfterStability(MembLcomDStb_GFLG, rMembBase, rDgnKappaChkResD, StrnStabD_FLG_STA_DeckG);
		rDgnStaResD.DeckPMembD_Bot[nPart].GDeckPlate_Sta = StrnStabD_FLG_STA_DeckG.DeckPMembD_Bot[nPart].GDeckPlate_Sta;
		rDgnStaResD.DeckPMembD_Bot[nPart].nIndexCriticalDP_G = SDSB_Result.nDeckG_cr;
		if (SDSB_Result.bChk_DeckG) { bChk = TRUE; }

		// Flange : DeckL
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_FLG = rMemb;
		Get_RptForceD_AfterStaRes_Flange(ElemK, nIJ, bIsTop, nPart, SB_DECK_L, MembLcomDStb_FLG, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_DeckL;
		m_pStlCompManager->Check_StlCompAfterStability(MembLcomDStb_FLG, rMembBase, rDgnKappaChkResD, StrnStabD_FLG_STA_DeckL);
		rDgnStaResD.DeckPMembD_Bot[nPart].aLDeckPlate_Sta.Copy(StrnStabD_FLG_STA_DeckL.DeckPMembD_Bot[nPart].aLDeckPlate_Sta);
		rDgnStaResD.DeckPMembD_Bot[nPart].nIndexCriticalDP_L = SDSB_Result.nDeckL_cr;
		if (SDSB_Result.bChk_DeckL) { bChk = TRUE; }

		// Flange : Rib
		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_Rib = rMemb;
		Get_RptForceD_AfterStaRes_Flange(ElemK, nIJ, bIsTop, nPart, SB_RIB, MembLcomDStb_Rib, SDSB_Result);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_FLG_STA_Rib;
		m_pStlCompManager->Check_StlCompAfterStability(MembLcomDStb_Rib, rMembBase, rDgnKappaChkResD, StrnStabD_FLG_STA_Rib);
		rDgnStaResD.DeckPMembD_Bot[nPart].aLRib_Sta.Copy(StrnStabD_FLG_STA_Rib.DeckPMembD_Bot[nPart].aLRib_Sta);
		rDgnStaResD.DeckPMembD_Bot[nPart].nIndexCriticalRib = SDSB_Result.nRib_cr;
		if (SDSB_Result.bChk_Rib) { bChk = TRUE; }

		rDgnStaResD.DeckPMembD_Bot[nPart].bChk = bChk;
	}
	return TRUE;

}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterStaWebResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD)
{
	// Web(6)
	// - L : Deck, Rib
	// - R : Deck, Rib

	//******************
	// Web 
	//******************
	STL_MEMB_POSD_SNiP_OD MembLcomDStb_Web;
	T_SDPT_STAB_WEB SDSB_ResultWeb;
	for (int nLeft = 0; nLeft < 2; ++nLeft)
	{
		BOOL bChk = FALSE;
		BOOL bLeft = (nLeft == 0) ? TRUE : FALSE;

		STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_WebDeck = rMemb;
		Set_RptForceD_AfterStaRes_Web(ElemK, nIJ, bLeft, SB_WEB_DECK, MembLcomDStb_WebDeck, SDSB_ResultWeb);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_WEB_STA_Deck;
		m_pStlCompManager->Check_StlCompAfterStability(MembLcomDStb_WebDeck, rMembBase, rDgnKappaChkResD, StrnStabD_WEB_STA_Deck);

		rDgnStaResD.aResWebStaD[nLeft] = StrnStabD_WEB_STA_Deck.aResWebStaD[nLeft];
		rDgnStaResD.aResWebStaD[nLeft].aLRib_Sta.RemoveAll();
		if (StrnStabD_WEB_STA_Deck.aResWebStaD[nLeft].aWebPlateD.GetSize() > 0)
		{
			bChk = StrnStabD_WEB_STA_Deck.aResWebStaD[nLeft].aWebPlateD[0].bChk;
		}

		STL_COMP_MEMB_POSD_SNiP&  MembLcomDStb_WebRib = rMemb;
		Set_RptForceD_AfterStaRes_Web(ElemK, nIJ, bLeft, SB_WEB_RIB, MembLcomDStb_WebRib, SDSB_ResultWeb);
		STL_COMP_MEMB_RESD_SNiP_BEFORE_STB StrnStabD_WEB_STA_Rib;
		m_pStlCompManager->Check_StlCompAfterStability(MembLcomDStb_WebRib, rMembBase, rDgnKappaChkResD, StrnStabD_WEB_STA_Rib);

		rDgnStaResD.aResWebStaD[nLeft].aLRib_Sta.Copy(StrnStabD_WEB_STA_Rib.aResWebStaD[nLeft].aLRib_Sta);
		rDgnStaResD.aResWebStaD[nLeft].nWebRibCrIndex = SDSB_ResultWeb.nRib_cr;
		if (SDSB_ResultWeb.bChk_Rib) { bChk = TRUE; }

		rDgnStaResD.aResWebStaD[nLeft].bChk = bChk;
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterFaitResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &rDgnResD)
{
	// Get Fatigue Check Result of Concrete
	GetRptD_AfterFaitConcResD(ElemK, nIJ, rMemb, rMembBase, rDgnKappaChkResD, rDgnResD);

	// Get Fatigue Check Result of Rebar
	GetRptD_AfterFaitRebarResD(ElemK, nIJ, rMemb, rMembBase, rDgnKappaChkResD, rDgnResD);

	// Get Fatigue Check Result of Steel Girder
	GetRptD_AfterFaitStlGirderResD(ElemK, nIJ, rMemb, rMembBase, rDgnKappaChkResD, rDgnResD);

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterFaitConcResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &rDgnResD)
{
	STL_COMP_MEMB_POSD_SNiP& MembFatiD = rMemb;
	Get_RptForceD_AfterFatiRes_Conc(ElemK, nIJ, MembFatiD);

	STL_COMP_MEMB_RESD_SNiP_AFTER_FATI FatiResD;
	m_pStlCompManager->Check_StlCompFatigue(MembFatiD, rMembBase, rDgnKappaChkResD, FatiResD);

	if (FatiResD.ConcFatiResD.bCheck)
	{
		rDgnResD.ConcFatiResD = FatiResD.ConcFatiResD;
		rDgnResD.bCheck = TRUE;
	}
	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterFaitRebarResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &rDgnResD)
{
	STL_COMP_MEMB_POSD_SNiP& MembFatiD = rMemb;
	Get_RptForceD_AfterFatiRes_Rebar(ElemK, nIJ, MembFatiD);

	STL_COMP_MEMB_RESD_SNiP_AFTER_FATI FatiResD;
	m_pStlCompManager->Check_StlCompFatigue(MembFatiD, rMembBase, rDgnKappaChkResD, FatiResD);

	if (FatiResD.RebarFatiResD.bCheck)
	{
		rDgnResD.RebarFatiResD = FatiResD.RebarFatiResD;
		rDgnResD.bCheck = TRUE;
	}
	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterFaitStlGirderResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &rDgnResD)
{
	STL_COMP_MEMB_POSD_SNiP& MembFatiD = rMemb;
	Get_RptForceD_AfterFatiRes_StlGirder(ElemK, nIJ, MembFatiD);

	STL_COMP_MEMB_RESD_SNiP_AFTER_FATI FatiResD;
	m_pStlCompManager->Check_StlCompFatigue(MembFatiD, rMembBase, rDgnKappaChkResD, FatiResD);

	if (FatiResD.StlGirderFatiResD.bCheck)
	{
		rDgnResD.StlGirderFatiResD = FatiResD.StlGirderFatiResD;
		rDgnResD.bCheck = TRUE;
	}
	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_AfterServResD(T_ELEM_K ElemK, int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb, STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_SERV &rDgnResD)
{
	//******************
	// Serviceability Check Result
	//******************
	T_CRACK_CONC Serv_Conc;
	STL_COMP_MEMB_POSD_SNiP& MembLcomDStb_Conc = rMemb;
	Get_RptForceD_AfterServRes_Conc(ElemK, nIJ, MembLcomDStb_Conc, Serv_Conc);

	STL_COMP_MEMB_RESD_SNiP_AFTER_SERV ServD_Conc;
	m_pStlCompManager->Check_StlCompServiseability(MembLcomDStb_Conc, rMembBase, rDgnKappaChkResD, ServD_Conc);
	rDgnResD = ServD_Conc;

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptD_ShearConnGroupResD(T_SCGD_K ScgdK, STL_COMP_MEMB_POSD_SNiP& rMembD, STL_COMP_AFTER_SHEAR_CONN_BASE_SNiP& rShearConnBaseD,
	STL_COMP_MEMB_RESD_SNiP_SHEAR_CONN& rDgnResD)
{
	//******************
	// Shear Connector Group  Check Result
	//******************
	// Get  Input Data
	if (!m_mStlCompShearConnectorD.Lookup(ScgdK, rMembD.ShearConnD)) { ASSERT(0); return FALSE; }

	// Make Base Data
	if (!m_pStlCompManager->Make_StlCompShearConnBaseData(rMembD, rShearConnBaseD)) { ASSERT(0); return FALSE; }

	// Get Check Force Data
	rMembD.ShearConnD.ShearConnForceD.SetSize(1);
	if (!GetRptForceD_ShearConnGroupResD(ScgdK, rMembD.ShearConnD, rMembD.ShearConnD.ShearConnForceD[0])) { ASSERT(0); return FALSE; }

	m_pStlCompManager->Check_StlCompShearConnector(rMembD, rShearConnBaseD, rDgnResD);

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_RptForceD_BeforeStrRes_Flg(T_ELEM_K ElemK, int nIJ, BOOL bTop, int nFlangePart, int nDeckRibFB, STL_COMP_MEMB_POSD_SNiP &MembLcomD,
	T_SDPT_STRN_FLG &SDSN_Result)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J
	int nTopBot = (bTop) ? 0 : 1;
	int nIndex = nTopBot + ix * 2;

	//******************
	// Flange
	//******************
	T_SDSN_FLG_CR FLG_CR;    // Flange 파트별 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompStrnFlgCrBeforeResult(ElemK, FLG_CR)) FLG_CR.Initialize();

	T_SDCR_BASE SdcrB;
	if (nDeckRibFB == SN_DECK)
	{
		SdcrB = FLG_CR.DeckCr[nIndex];    //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom
	}
	else if (nDeckRibFB == SN_RIB)
	{
		SdcrB = FLG_CR.RibCr[nIndex];
	}
	else if (nDeckRibFB == SN_FLOORBEAM)
	{
		SdcrB = FLG_CR.FBeamCr[nIndex];
	}
	else ASSERT(0);

	// 설계 위한 하중 변환
	CStringW strStageName;
	Get_StagName(SdcrB.nLcomK, strStageName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strStageName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strStageName;
	ConvertStlCompForce2Engine(ElemK, nIJ, 0, SdcrB.nDgnLcomK, MembLcomD, FALSE);

	// Flange 파트별 검토 결과
	T_SDSN_FLG_CASE SDSN_FLG;
	if (bTop)
	{
		if (!m_pStlCompDesign->GetStlCompTopFlgStrnBeforeResult(ElemK, SdcrB.nDgnLcomK, SDSN_FLG)) SDSN_FLG.Initialize(); // 실제 저장은 Stage Key
	}
	else
	{
		if (!m_pStlCompDesign->GetStlCompBotFlgStrnBeforeResult(ElemK, SdcrB.nDgnLcomK, SDSN_FLG)) SDSN_FLG.Initialize(); // 실제 저장은 Stage Key
	}
	T_SDSN_FLG SDSN_FLANGE = SDSN_FLG.StrnFlg[ix]; // I, J	
	SDSN_Result.Initialize();
	SDSN_Result = SDSN_FLANGE.StrnChk[nFlangePart]; // I-L,R // Box-L,M,R 

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_RptForceD_BeforeStrRes_Web(T_ELEM_K ElemK, int nIJ, BOOL bLeft, int nWebChkType, STL_COMP_MEMB_POSD_SNiP &MembLcomD,
	T_SDPT_STRN_WEB &SDSN_Result)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	SDSN_Result.Initialize();

	//******************
	// Web
	//******************
	T_SDSN_WEB_CR WEB_CR;   // Web 파트별 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompStrnWebCrBeforeResult(ElemK, WEB_CR)) WEB_CR.Initialize();

	T_SDCR_BASE SdcrB;
	if (nWebChkType == SN_WEB_NoM)
	{
		SdcrB = (bLeft) ? WEB_CR.DeckNoMCrL[ix] : WEB_CR.DeckNoMCrR[ix];    //0=I 1=J
	}
	else if (nWebChkType == SN_WEB_VON)
	{
		SdcrB = (bLeft) ? WEB_CR.DeckVonCrL[ix] : WEB_CR.DeckVonCrR[ix];    //0=I 1=J
	}
	else if (nWebChkType == SN_WEB_RIB)
	{
		SdcrB = (bLeft) ? WEB_CR.RibCrL[ix] : WEB_CR.RibCrR[ix];    //0=I 1=J
	}
	else ASSERT(0);

	// 설계 위한 하중 변환
	CStringW strStageName;
	Get_StagName(SdcrB.nLcomK, strStageName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strStageName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strStageName;
	Get_LocalForceD(ElemK, nIJ, 0, MembLcomD.CheckLcomD.GlobalForceD[0].LocalForceBeforeD, FALSE);
	ConvertStlCompForce2Engine(ElemK, nIJ, 0, SdcrB.nDgnLcomK, MembLcomD, FALSE);

	// Web 파트별 검토 결과
	T_SDSN_WEB_CASE SDSN_WEB;
	if (!m_pStlCompDesign->GetStlCompWebStrnBeforeResult(ElemK, SdcrB.nDgnLcomK, SDSN_WEB)) SDSN_WEB.Initialize();
	T_SDSN_WEB SDSN_Web_Result = SDSN_WEB.StrnWeb[ix]; // I, J
	SDSN_Result.Initialize();
	SDSN_Result = (bLeft) ? SDSN_Web_Result.WebD[0] : SDSN_Web_Result.WebD[1];

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_RptForceD_BeforeStaRes_LTB(T_ELEM_K ElemKey, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_SDSB_GD &SDSB_GDR)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	T_SDSB_GD_CR GD_CR;   // 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompStabGDCrBeforeResult(ElemKey, GD_CR)) GD_CR.Initialize();
	T_SDCR_BASE &SdcrB_GD = GD_CR.GDCr[ix];    //0=I 1=J

	// 설계 위한 하중 변환
	CStringW strStageName;
	Get_StagName(SdcrB_GD.nLcomK, strStageName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strStageName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strStageName;
	ConvertStlCompForce2Engine(ElemKey, nIJ, 0, SdcrB_GD.nDgnLcomK, MembLcomD, FALSE);

	// Flange 파트별 검토 결과
	T_SDSB_GD_CASE SDSB_GD;
	if (!m_pStlCompDesign->GeStlCompGDStabBeforeResult(ElemKey, SdcrB_GD.nDgnLcomK, SDSB_GD)) SDSB_GD.Initialize();

	SDSB_GDR.Initialize();
	SDSB_GDR = SDSB_GD.StabGD[ix]; // I, J	

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Set_RptForceD_BeforeStaRes_Flange(T_ELEM_K ElemK, int nIJ, BOOL bTop, int nFlangePart, int nDeckRibFB, STL_COMP_MEMB_POSD_SNiP &MembLcomD,
	T_SDPT_STAB_FLG &SDSB_Result)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J
	int nTopBot = (bTop) ? 0 : 1;
	int nIndex = nTopBot + ix * 2;

	//******************
	// Flange
	//******************
	T_SDSB_FLG_CR FLG_CR;    // Flange 파트별 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompStabFlgCrBeforeResult(ElemK, FLG_CR)) FLG_CR.Initialize();

	T_SDCR_BASE SdcrB;
	if (nDeckRibFB == SB_DECK_G)
	{
		SdcrB = FLG_CR.DeckGCr[nIndex];    //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom
	}
	else if (nDeckRibFB == SB_DECK_L)
	{
		SdcrB = FLG_CR.DeckLCr[nIndex];
	}
	else if (nDeckRibFB == SB_RIB)
	{
		SdcrB = FLG_CR.RibCr[nIndex];
	}
	else if (nDeckRibFB == SB_FLOORBEAM_G)
	{
		SdcrB = FLG_CR.FBeamGCr[nIndex];
	}
	else if (nDeckRibFB == SB_FLOORBEAM_L)
	{
		SdcrB = FLG_CR.FBeamLCr[nIndex];
	}
	else ASSERT(0);

	// 설계 위한 하중 변환
	CStringW strStageName;
	Get_StagName(SdcrB.nLcomK, strStageName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strStageName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strStageName;
	ConvertStlCompForce2Engine(ElemK, nIJ, 0, SdcrB.nDgnLcomK, MembLcomD, FALSE);

	// Flange 파트별 검토 결과
	T_SDSB_FLG_CASE SDSB_FLG;
	if (bTop)
	{
		if (!m_pStlCompDesign->GetStlCompTopFlgStabBeforeResult(ElemK, SdcrB.nDgnLcomK, SDSB_FLG)) SDSB_FLG.Initialize();
	}
	else
	{
		if (!m_pStlCompDesign->GetStlCompBotFlgStabBeforeResult(ElemK, SdcrB.nDgnLcomK, SDSB_FLG)) SDSB_FLG.Initialize();
	}
	T_SDSB_FLG SDSB_FLANGE = SDSB_FLG.StabFlg[ix]; // I, J	
	SDSB_Result.Initialize();
	SDSB_Result = SDSB_FLANGE.StabChk[nFlangePart]; // I-L,R // Box-L,M,R 

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Set_RptForceD_BeforeStaRes_Web(T_ELEM_K ElemK, int nIJ, BOOL bLeft, int nWebChkType, STL_COMP_MEMB_POSD_SNiP &MembLcomD,
	T_SDPT_STAB_WEB &SDSB_Result)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	//******************
	// Web
	//******************
	T_SDSB_WEB_CR WEB_CR;   // Web 파트별 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompStabWebCrBeforeResult(ElemK, WEB_CR)) WEB_CR.Initialize();

	T_SDCR_BASE SdcrB;
	if (nWebChkType == SB_WEB_DECK)
	{
		SdcrB = (bLeft) ? WEB_CR.DeckCrL[ix] : WEB_CR.DeckCrR[ix];
	}
	else if (nWebChkType == SB_WEB_RIB)
	{
		SdcrB = (bLeft) ? WEB_CR.RibCrL[ix] : WEB_CR.RibCrR[ix];
	}
	else ASSERT(0);

	// 설계 위한 하중 변환
	CStringW strStageName;
	Get_StagName(SdcrB.nLcomK, strStageName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strStageName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strStageName;
	Get_LocalForceD(ElemK, nIJ, 0, MembLcomD.CheckLcomD.GlobalForceD[0].LocalForceBeforeD, FALSE);
	ConvertStlCompForce2Engine(ElemK, nIJ, 0, SdcrB.nDgnLcomK, MembLcomD, FALSE);

	// Web 파트별 검토 결과
	T_SDSB_WEB_CASE SDSB_WEB;
	if (!m_pStlCompDesign->GetStlCompWebStabBeforeResult(ElemK, SdcrB.nDgnLcomK, SDSB_WEB)) SDSB_WEB.Initialize();
	T_SDSB_WEB SDSB_Web_Result = SDSB_WEB.StabWeb[ix]; // I, J
	SDSB_Result.Initialize();
	SDSB_Result = (bLeft) ? SDSB_Web_Result.WebD[0] : SDSB_Web_Result.WebD[1];

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_RptForceD_AfterStrRes_Conc(T_ELEM_K ElemKey, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_ACST_CONC &rSTR_Conc)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	T_ACST_CONC_CR Conc_CR;   // 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompStrnConcreteCrAfterResult(ElemKey, Conc_CR)) Conc_CR.Initialize();
	T_ACST_CONC_BASE &SdcrB_Conc = Conc_CR.StrnConcCR[ix];    //0=I 1=J

	CStringW strLcomName;
	Get_LcomName(SdcrB_Conc.nLcomK, strLcomName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].nMax = SdcrB_Conc.nConcurrent;
	ConvertStlCompForce2Engine(ElemKey, nIJ, 0, SdcrB_Conc.nDgnLcomK, MembLcomD);

	T_ACST_CONC_CASE SDSB_Conc;
	if (!m_pStlCompDesign->GetStlCompConcreteStrnAfterResult(ElemKey, SdcrB_Conc.nDgnLcomK, SDSB_Conc)) SDSB_Conc.Initialize();

	rSTR_Conc.Initialize();
	rSTR_Conc = SDSB_Conc.StrnConc[ix]; // I, J	

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_RptForceD_AfterStrRes_Rebar(T_ELEM_K ElemKey, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_ACST_REBAR &rSTR_Rbar)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	T_ACST_REBAR_CR Rebar_CR;   // 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompStrnRebarCrAfterResult(ElemKey, Rebar_CR)) Rebar_CR.Initialize();
	T_ACST_REBAR_BASE &SdcrB_Rebar = Rebar_CR.StrnRebarCR[ix];    //0=I 1=J


	CStringW strLcomName;
	Get_LcomName(SdcrB_Rebar.nLcomK, strLcomName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].nMax = SdcrB_Rebar.nConcurrent;
	ConvertStlCompForce2Engine(ElemKey, nIJ, 0, SdcrB_Rebar.nDgnLcomK, MembLcomD);

	T_ACST_REBAR_CASE SDSB_Rbar;
	if (!m_pStlCompDesign->GetStlCompRebarStrnAfterResult(ElemKey, SdcrB_Rebar.nDgnLcomK, SDSB_Rbar)) SDSB_Rbar.Initialize();

	rSTR_Rbar.Initialize();
	rSTR_Rbar = SDSB_Rbar.StrnRebar[ix]; // I, J	

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_RptForceD_AfterStrRes_Flg(T_ELEM_K ElemK, int nIJ, BOOL bTop, int nFlangePart, int nDeckRibFB, STL_COMP_MEMB_POSD_SNiP &MembLcomD,
	T_ACST_SGFR_ELEM &rStr_Flg_Result)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J
	int nTopBot = (bTop) ? 0 : 1;
	int nIndex = nTopBot + ix * 2;

	//******************
	// Flange
	//******************
	T_ACST_SGFR_CR FLG_CR;    // Flange 파트별 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompStrnFlgCrAfterResult(ElemK, FLG_CR)) FLG_CR.Initialize();

	T_ACST_SGFR_BASE SdcrB;
	T_SDCR_BASE SdcrB_Rib;
	int nType = 0;
	if (nDeckRibFB == SN_DECK)
	{
		SdcrB = FLG_CR.StrnFlgCR[nIndex];    //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom
	}
	else if (nDeckRibFB == SN_RIB)
	{
		SdcrB_Rib = FLG_CR.RibCr[nIndex];
		nType = 1;
	}
	else ASSERT(0);

	// 설계 위한 하중 변환
	int nDgnLcomK = nType == 0 ? SdcrB.nDgnLcomK : SdcrB_Rib.nDgnLcomK;
	int nLcomK = nType == 0 ? SdcrB.nLcomK : SdcrB_Rib.nLcomK;
	CStringW strLcomName;
	Get_LcomName(nLcomK, strLcomName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].nMax = nType == 0 ? SdcrB.nConcurrent : SdcrB_Rib.nConcurrent;
	ConvertStlCompForce2Engine(ElemK, nIJ, 0, nDgnLcomK, MembLcomD);

	// Flange 파트별 검토 결과
	T_ACST_SGFR_CASE SDSN_FLG;
	if (bTop)
	{
		if (!m_pStlCompDesign->GetStlCompTopFlgStrnAfterResult(ElemK, nDgnLcomK, SDSN_FLG)) SDSN_FLG.Initialize(); // 실제 저장은 Stage Key
	}
	else
	{
		if (!m_pStlCompDesign->GetStlCompBotFlgStrnAfterResult(ElemK, nDgnLcomK, SDSN_FLG)) SDSN_FLG.Initialize(); // 실제 저장은 Stage Key
	}
	T_ACST_SGFR_PART SDSN_FLANGE = SDSN_FLG.StrnFlgIJ[ix]; // I, J	
	rStr_Flg_Result.Initialize();
	rStr_Flg_Result = SDSN_FLANGE.StrnFlangePart[nFlangePart]; // I-L,R // Box-L,M,R 

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_RptForceD_AfterStrRes_Web(T_ELEM_K ElemK, int nIJ, BOOL bLeft, int nWebChkType, STL_COMP_MEMB_POSD_SNiP &MembLcomD,
	T_SDPT_STRN_WEB &SDSN_Result)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	SDSN_Result.Initialize();

	//******************
	// Web
	//******************
	T_SDSN_WEB_CR WEB_CR;   // Web 파트별 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompStrnWebCrAfterResult(ElemK, WEB_CR)) WEB_CR.Initialize();

	T_SDCR_BASE SdcrB;
	if (nWebChkType == SN_WEB_NoM)
	{
		SdcrB = (bLeft) ? WEB_CR.DeckNoMCrL[ix] : WEB_CR.DeckNoMCrR[ix];    //0=I 1=J
	}
	else if (nWebChkType == SN_WEB_VON)
	{
		SdcrB = (bLeft) ? WEB_CR.DeckVonCrL[ix] : WEB_CR.DeckVonCrR[ix];    //0=I 1=J
	}
	else if (nWebChkType == SN_WEB_COM)
	{
		SdcrB = (bLeft) ? WEB_CR.DeckCombCrL[ix] : WEB_CR.DeckCombCrR[ix];    //0=I 1=J
	}
	else if (nWebChkType == SN_WEB_MEAN)
	{
		SdcrB = (bLeft) ? WEB_CR.DeckMeanCrL[ix] : WEB_CR.DeckMeanCrR[ix];    //0=I 1=J
	}
	else if (nWebChkType == SN_WEB_RIB)
	{
		SdcrB = (bLeft) ? WEB_CR.RibCrL[ix] : WEB_CR.RibCrR[ix];    //0=I 1=J
	}
	else ASSERT(0);

	// 설계 위한 하중 변환
	CStringW strLcomName;
	Get_LcomName(SdcrB.nLcomK, strLcomName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].nMax = SdcrB.nConcurrent;
	Get_LocalForceD(ElemK, nIJ, SdcrB.nLcomK, MembLcomD.CheckLcomD.GlobalForceD[0].LocalForceAfterD, TRUE);
	ConvertStlCompForce2Engine(ElemK, nIJ, 0, SdcrB.nDgnLcomK, MembLcomD);

	// Web 파트별 검토 결과
	T_SDSN_WEB_CASE SDSN_WEB;
	if (!m_pStlCompDesign->GetStlCompWebStrnAfterResult(ElemK, SdcrB.nDgnLcomK, SDSN_WEB)) SDSN_WEB.Initialize();
	T_SDSN_WEB SDSN_Web_Result = SDSN_WEB.StrnWeb[ix]; // I, J
	SDSN_Result.Initialize();
	SDSN_Result = (bLeft) ? SDSN_Web_Result.WebD[0] : SDSN_Web_Result.WebD[1];

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_RptForceD_AfterStaRes_Flange(T_ELEM_K ElemK, int nIJ, BOOL bTop, int nFlangePart, int nDeckRibFB, STL_COMP_MEMB_POSD_SNiP &MembLcomD,
	T_SDPT_STAB_FLG &SDSB_Result)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J
	int nTopBot = (bTop) ? 0 : 1;
	int nIndex = nTopBot + ix * 2;

	//******************
	// Flange
	//******************
	T_SDSB_FLG_CR FLG_CR;    // Flange 파트별 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompStabFlgCrAfterResult(ElemK, FLG_CR)) FLG_CR.Initialize();

	T_SDCR_BASE SdcrB;
	if (nDeckRibFB == SB_DECK_G)
	{
		SdcrB = FLG_CR.DeckGCr[nIndex];    //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom
	}
	else if (nDeckRibFB == SB_DECK_L)
	{
		SdcrB = FLG_CR.DeckLCr[nIndex];
	}
	else if (nDeckRibFB == SB_RIB)
	{
		SdcrB = FLG_CR.RibCr[nIndex];
	}
	else ASSERT(0);

	// 설계 위한 하중 변환
	CStringW strLcomName;
	Get_LcomName(SdcrB.nLcomK, strLcomName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].nMax = SdcrB.nConcurrent;
	ConvertStlCompForce2Engine(ElemK, nIJ, 0, SdcrB.nDgnLcomK, MembLcomD);

	// Flange 파트별 검토 결과
	T_SDSB_FLG_CASE SDSB_FLG;
	if (bTop)
	{
		ASSERT(0); // After의 경우, Top의  Stability는 고려하지 않는다.   PCS
	}
	else
	{
		if (!m_pStlCompDesign->GetStlCompBotFlgStabAfterResult(ElemK, SdcrB.nDgnLcomK, SDSB_FLG)) SDSB_FLG.Initialize();
	}
	T_SDSB_FLG SDSB_FLANGE = SDSB_FLG.StabFlg[ix]; // I, J	
	SDSB_Result.Initialize();
	SDSB_Result = SDSB_FLANGE.StabChk[nFlangePart]; // I-L,R // Box-L,M,R 

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::Set_RptForceD_AfterStaRes_Web(T_ELEM_K ElemK, int nIJ, BOOL bLeft, int nWebChkType, STL_COMP_MEMB_POSD_SNiP &MembLcomD,
	T_SDPT_STAB_WEB &SDSB_Result)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	//******************
	// Web
	//******************
	T_SDSB_WEB_CR WEB_CR;   // Web 파트별 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompStabWebCrAfterResult(ElemK, WEB_CR)) WEB_CR.Initialize();

	T_SDCR_BASE SdcrB;
	if (nWebChkType == SB_WEB_DECK)
	{
		SdcrB = (bLeft) ? WEB_CR.DeckCrL[ix] : WEB_CR.DeckCrR[ix];
	}
	else if (nWebChkType == SB_WEB_RIB)
	{
		SdcrB = (bLeft) ? WEB_CR.RibCrL[ix] : WEB_CR.RibCrR[ix];
	}
	else ASSERT(0);

	// 설계 위한 하중 변환
	CStringW strLcomName;
	Get_LcomName(SdcrB.nLcomK, strLcomName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].nMax = SdcrB.nConcurrent;
	Get_LocalForceD(ElemK, nIJ, SdcrB.nLcomK, MembLcomD.CheckLcomD.GlobalForceD[0].LocalForceAfterD, TRUE);
	ConvertStlCompForce2Engine(ElemK, nIJ, 0, SdcrB.nDgnLcomK, MembLcomD);

	// Web 파트별 검토 결과
	T_SDSB_WEB_CASE SDSB_WEB;
	if (!m_pStlCompDesign->GetStlCompWebStabAfterResult(ElemK, SdcrB.nDgnLcomK, SDSB_WEB)) SDSB_WEB.Initialize();
	T_SDSB_WEB SDSB_Web_Result = SDSB_WEB.StabWeb[ix]; // I, J
	SDSB_Result.Initialize();
	SDSB_Result = (bLeft) ? SDSB_Web_Result.WebD[0] : SDSB_Web_Result.WebD[1];

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::Get_RptForceD_AfterFatiRes_Conc(T_ELEM_K ElemK, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	int nLcomSize = 1;
	MembLcomD.CheckLcomD.GlobalForceD.RemoveAll();

	T_FATI_CONC_CR FatiCrR;
	m_pStlCompDesign->GetStlCompFatiConcCrAfterResult(ElemK, FatiCrR);
	T_FATI_CONC_BASE &SDCR_BASE = FatiCrR.FatiConc[ix];

	int nFatiConcSize = SDCR_BASE.nLcomSize;

	STLCOMPCHKFOR_ARRAY aChkForce;
	aChkForce.SetSize(nFatiConcSize);

	T_SCGD_DGN_FORCE StlCompForce;
	for (int i = 0; i < nFatiConcSize; ++i)
	{
		StlCompForce.Initialize();
		int nDgnLcomId = SDCR_BASE.nDgnLcomKFirst + i;
		m_pStlCompDesign->GetStlCompLcomForce(ElemK, nDgnLcomId, StlCompForce);

		ConvertStlCompGForce2Engine(StlCompForce.Elem[nIJ], aChkForce[i]);
	}

	INT_PTR nFatiLcomSize = aChkForce.GetSize();
	if (nFatiLcomSize < 1) nFatiLcomSize = 1;
	MembLcomD.CheckLcomD.GlobalForceD.SetSize(nFatiLcomSize);
	MembLcomD.CheckLcomD.GlobalForceD.Copy(aChkForce);

	CStringW strLcomName;
	Get_LcomName(SDCR_BASE.nLcomK, strLcomName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].nMax = SDCR_BASE.nConcurrent;

	T_LCOM_D LcomD;
	if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, SDCR_BASE.nLcomK, LcomD) && nFatiLcomSize > 0)
	{
		CStringW strLcomNameW(LcomD.LoadCombName);
		for (int i = 0; i < nFatiLcomSize; ++i)
		{
			MembLcomD.CheckLcomD.GlobalForceD[i].strLcomName = strLcomNameW;
		}
		CStringW strLcomTypeW(Get_LcomType(SDCR_BASE.nConcurrent));
		MembLcomD.CheckLcomD.GlobalForceD[0].strLcomType = strLcomTypeW;
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_RptForceD_AfterFatiRes_Rebar(T_ELEM_K ElemK, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	int nLcomSize = 1;
	MembLcomD.CheckLcomD.GlobalForceD.RemoveAll();

	T_FATI_REBAR_CR FatiCrR;
	m_pStlCompDesign->GetStlCompFatiRebarCrAfterResult(ElemK, FatiCrR);
	T_FATI_REBAR_BASE &SDCR_BASE = FatiCrR.FatiRebarCR[ix];

	int nFatiConcSize = SDCR_BASE.nLcomSize;

	STLCOMPCHKFOR_ARRAY aChkForce;
	aChkForce.SetSize(nFatiConcSize);

	T_SCGD_DGN_FORCE StlCompForce;
	for (int i = 0; i < nFatiConcSize; ++i)
	{
		StlCompForce.Initialize();
		int nDgnLcomId = SDCR_BASE.nDgnLcomKFirst + i;
		m_pStlCompDesign->GetStlCompLcomForce(ElemK, nDgnLcomId, StlCompForce);

		ConvertStlCompGForce2Engine(StlCompForce.Elem[nIJ], aChkForce[i]);
	}

	INT_PTR nFatiLcomSize = aChkForce.GetSize();
	if (nFatiLcomSize < 1) nFatiLcomSize = 1;
	MembLcomD.CheckLcomD.GlobalForceD.SetSize(nFatiLcomSize);
	MembLcomD.CheckLcomD.GlobalForceD.Copy(aChkForce);

	CStringW strLcomName;
	Get_LcomName(SDCR_BASE.nLcomK, strLcomName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].nMax = SDCR_BASE.nConcurrent;

	T_LCOM_D LcomD;
	if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, SDCR_BASE.nLcomK, LcomD))
	{
		CStringW strLcomNameW(LcomD.LoadCombName);
		for (int i = 0; i < nFatiLcomSize; ++i)
		{
			MembLcomD.CheckLcomD.GlobalForceD[i].strLcomName = strLcomNameW;
		}
		CStringW strLcomTypeW(Get_LcomType(SDCR_BASE.nConcurrent));
		MembLcomD.CheckLcomD.GlobalForceD[0].strLcomType = strLcomTypeW;
	}

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::Get_RptForceD_AfterFatiRes_StlGirder(T_ELEM_K ElemK, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	int nLcomSize = 1;
	MembLcomD.CheckLcomD.GlobalForceD.RemoveAll();

	T_FATI_STL_GIRDER_CR FatiCrR;
	m_pStlCompDesign->GetStlCompFatiStlGirderCrAfterResult(ElemK, FatiCrR);
	T_FATI_STL_GIRDER_BASE &SDCR_BASE = FatiCrR.FatiStlGirderCR[ix];

	int nFatiConcSize = SDCR_BASE.nLcomSize;

	STLCOMPCHKFOR_ARRAY aChkForce;
	aChkForce.SetSize(nFatiConcSize);

	T_SCGD_DGN_FORCE StlCompForce;
	for (int i = 0; i < nFatiConcSize; ++i)
	{
		StlCompForce.Initialize();
		int nDgnLcomId = SDCR_BASE.nDgnLcomKFirst + i;
		m_pStlCompDesign->GetStlCompLcomForce(ElemK, nDgnLcomId, StlCompForce);

		ConvertStlCompGForce2Engine(StlCompForce.Elem[nIJ], aChkForce[i]);
	}

	INT_PTR nFatiLcomSize = aChkForce.GetSize();
	if (nFatiLcomSize < 1) nFatiLcomSize = 1;
	MembLcomD.CheckLcomD.GlobalForceD.SetSize(nFatiLcomSize);
	MembLcomD.CheckLcomD.GlobalForceD.Copy(aChkForce);

	CStringW strLcomName;
	Get_LcomName(SDCR_BASE.nLcomK, strLcomName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].nMax = SDCR_BASE.nConcurrent;

	T_LCOM_D LcomD;
	if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, SDCR_BASE.nLcomK, LcomD))
	{
		CStringW strLcomNameW(LcomD.LoadCombName);
		for (int i = 0; i < nFatiLcomSize; ++i)
		{
			MembLcomD.CheckLcomD.GlobalForceD[i].strLcomName = strLcomNameW;
		}
		CStringW strLcomTypeW(Get_LcomType(SDCR_BASE.nConcurrent));
		MembLcomD.CheckLcomD.GlobalForceD[0].strLcomType = strLcomTypeW; // 대표값으로 Index 0에 저장
	}

	return TRUE;
}


BOOL CDgnRUSStlCompDataCtrl::Get_RptForceD_AfterServRes_Conc(T_ELEM_K ElemK, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_CRACK_CONC &rSTR_Conc)
{
	if (!m_pStlCompDesign) ASSERT(0);
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	T_CRACK_CONC_CR Conc_CR;   // 가장 불리한 Lcom
	if (!m_pStlCompDesign->GetStlCompCrackConcCrAfterResult(ElemK, Conc_CR)) Conc_CR.Initialize();
	T_CRACK_CONC_BASE &SdcrB_Conc = Conc_CR.CrackConcCR[ix];    //0=I 1=J

	// Convert Design Force
	CStringW strLcomName;
	Get_LcomName(SdcrB_Conc.nLcomK, strLcomName);
	MembLcomD.CheckLcomD.GlobalForceD[0].strLcomName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].strStepName = strLcomName;
	MembLcomD.CheckLcomD.GlobalForceD[0].nMax = SdcrB_Conc.nConcurrent;
	ConvertStlCompForce2Engine(ElemK, nIJ, 0, SdcrB_Conc.nDgnLcomK, MembLcomD);

	T_CRACK_CONC_CASE SDSB_Conc;
	int nLSNumElem = 0; // Dummy
	if (!m_pStlCompDesign->GeStlCompCrackConcResult(ElemK, SdcrB_Conc.nDgnLcomK, SDSB_Conc)) SDSB_Conc.Initialize();

	rSTR_Conc.Initialize();
	rSTR_Conc = SDSB_Conc.CrackConc[ix]; // I, J	

	return TRUE;
}

BOOL CDgnRUSStlCompDataCtrl::GetRptForceD_ShearConnGroupResD(T_SCGD_K ScgdK, STL_COMP_SHEAR_CONN_INPUT_DATA_SNiP& rShearConnInD,
	SHEAR_CONN_GROUP_FORCE_SNIP& rShearConnGForceD)
{
	int nOrgLcomNo = 0;
	int nSerLcomCount = 0;
	int nMaxMinType = 0;
	CString strOrgLcomNa = _T("");
	bool bExistCrp = false, bExistShr = false, bExistBST = false;

	// Get Critical Load Data
	T_STRD_SHEAR_CONN_BASE ShearConnCr;
	m_pStlCompDesign->GetStlCompShearConnGroupCrAfterResult(ScgdK, ShearConnCr);
	int nDgnLcomK_Cr = ShearConnCr.nDgnLcomK;

	CStringW strLcomName;
	Get_LcomName(ShearConnCr.nLcomK, strLcomName);

	// Make Group Force Data for Report
	int nLcomNum = m_pForcCtrl->Get_LcomDataCount();
	INT_PTR nGElemSize = rShearConnInD.arGroupElemKey.GetSize();
	CArray<ShearConnGForD, ShearConnGForD&> arElemForceD_I, arElemForceD_J;  // array 순서와 동일하게 ElemK를 저장
	arElemForceD_I.RemoveAll(); arElemForceD_J.RemoveAll();
	arElemForceD_I.SetSize(nGElemSize);
	arElemForceD_J.SetSize(nGElemSize);

	_DGN_FORC_CRC ForceNC; // Dummy, 전단 연결재 검토에는 Before Force 사용 안 함
	for (INT_PTR nElem = 0; nElem < nGElemSize; ++nElem)
	{
		T_ELEM_K ElemK = rShearConnInD.arGroupElemKey[nElem];
		ElemPairK EPairK(ElemK, EN_EL_BEAM);

		ADGNFORCE aForceLcom;
		ADGNFORCE aForceTnpr;
		ADGNFORCE aForceTnsc;
		ADGNFORCE aForceLTerm;
		ADGNFORCE aForceSTerm;
		ADGNSHRSADD aStssLcom;
		if (!m_pForcCtrl->Get_LcomDataForDesign(EPairK, TRUE, TRUE, aForceLcom, aForceTnpr, aForceTnsc, aForceLTerm, aForceSTerm, aStssLcom, nLcomNum)) { ASSERT(0); continue; }

		ADGNFORCE aForceCreep, aForceShrinkage;
		m_pForcCtrl->Get_LcomForce4ScfpCrp(EPairK, nLcomNum, aForceCreep);
		m_pForcCtrl->Get_LcomForce4ScfpShr(EPairK, nLcomNum, aForceShrinkage);

		T_LCOM_K_LIST  aStrnLcomKList;
		Get_DgnLcomKeyList4ShearConnDesign(aForceLcom, aStrnLcomKList);  // Get DgnLcomK

		ASCFPSTRESS aScfpCr;
		ASCFPSTRESS aScfpSh;
		ASCFPSTRESS aScfpTe;
		ASCFPSTRESS aScfpRe;
		if (!m_pForcCtrl->Get_LcomStressScfp(EPairK, nLcomNum, aScfpCr, aScfpSh, aScfpTe, aScfpRe)) { ASSERT(0); continue; }  // Get Internal Stress

		ShearConnGForD ForceListD4Elem_I, ForceListD4Elem_J;

		int nDgnLcomIndex = nDgnLcomK_Cr - 1;
		_DGN_FORC_CRC& GForceD = aForceLcom[nDgnLcomIndex];
		_DGN_FORC_CRC& ForD_After_DL = aForceLTerm[nDgnLcomIndex];
		_DGN_FORC_CRC& ForD_Crp = aForceCreep[nDgnLcomIndex];
		_DGN_FORC_CRC& ForD_Shr = aForceShrinkage[nDgnLcomIndex];
		_DGN_FORC_CRC ForD_BST; //  ??? 요건 찾아야 함..
		_DGN_SCFP_PART_LCASE& ScfpCr = aScfpCr[nDgnLcomIndex];
		_DGN_SCFP_PART_LCASE& ScfpSh = aScfpSh[nDgnLcomIndex];
		_DGN_SCFP_PART_LCASE& ScfpTe = aScfpTe[nDgnLcomIndex];

		if (!m_pForcCtrl->Get_LcomType(nDgnLcomK_Cr, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)) { ASSERT(0); continue; }
		IsExistScfp4Lcom(D_LCOMTYPE_STLCOMP, nOrgLcomNo, bExistCrp, bExistShr, bExistBST);
		if (!bExistCrp) ScfpCr.Initialize();
		if (!bExistShr) ScfpSh.Initialize();
		if (!bExistBST) ScfpTe.Initialize();

		STL_COMP_FORCE_D ForD_I, ForD_J;
		ConvertStlCompCalcForce(0, ForceNC, GForceD, ForD_After_DL, ForD_Crp, ForD_Shr, ForD_BST, ScfpCr, ScfpSh, ScfpTe, ForD_I);
		ForD_I.strLcomName = strLcomName; ForD_I.strStepName = strLcomName;
		ConvertStlCompCalcForce(1, ForceNC, GForceD, ForD_After_DL, ForD_Crp, ForD_Shr, ForD_BST, ScfpCr, ScfpSh, ScfpTe, ForD_J);
		ForD_J.strLcomName = strLcomName; ForD_J.strStepName = strLcomName;

		if (!ForceListD4Elem_I.For4ElemD.Lookup(nDgnLcomK_Cr, ForD_I)) { ForceListD4Elem_I.For4ElemD.SetAt(nDgnLcomK_Cr, ForD_I); }
		if (!ForceListD4Elem_J.For4ElemD.Lookup(nDgnLcomK_Cr, ForD_J)) { ForceListD4Elem_J.For4ElemD.SetAt(nDgnLcomK_Cr, ForD_J); }

		arElemForceD_I.SetAt(nElem, ForceListD4Elem_I);
		arElemForceD_J.SetAt(nElem, ForceListD4Elem_J);
	}

	// Convert
	for (UINT nElem = 0; nElem < nGElemSize; ++nElem)
	{
		T_ELEM_K ElemK = rShearConnInD.arGroupElemKey[nElem];
		SHEAR_CONN_GROUP_FORCE_UNIT_SNIP ForD;
		arElemForceD_I[nElem].For4ElemD.Lookup(nDgnLcomK_Cr, ForD.ForceD[0]);
		arElemForceD_J[nElem].For4ElemD.Lookup(nDgnLcomK_Cr, ForD.ForceD[1]);
		rShearConnGForceD.mGroupForceD.SetAt(ElemK, ForD);
	}

	return TRUE;

}

CString CDgnRUSStlCompDataCtrl::Get_LcomType(int nConcurrent)
{
	CString strLcomType = _T("-");
	if (nConcurrent == 0)   strLcomType = _T("-");
	else if (nConcurrent == 2)   strLcomType = _T("Max/Min");
	else if (nConcurrent == 12) 	strLcomType = _T("-"); //?

	return strLcomType;
}

//////////////////////////////////////////////////////////////////////////
// Convert for Report Print
//////////////////////////////////////////////////////////////////////////

void CDgnRUSStlCompDataCtrl::ConvertStlCompForce2Engine(T_ELEM_K ElemK, int nIJ, int nChkPos, int nDgnLcomK, STL_COMP_MEMB_POSD_SNiP &MembLcomD, BOOL bIsAfter)
{
	T_SCGD_DGN_FORCE rData;

	if (bIsAfter) { m_pStlCompDesign->GetStlCompLcomForce(ElemK, nDgnLcomK, rData); }
	else { m_pStlCompDesign->GetStlCompStageForce(ElemK, nDgnLcomK, rData); }

	if (MembLcomD.CheckLcomD.GlobalForceD.GetSize() < 1) MembLcomD.CheckLcomD.GlobalForceD.SetSize(1);

	ConvertStlCompGForce2Engine(rData.Elem[nIJ], MembLcomD.CheckLcomD.GlobalForceD[0]);
	int nLcomK = bIsAfter ? nDgnLcomK : 0; // Before Local Force Key는 모든 CS단계에서 0으로 통일
	if (!bIsAfter)Get_LocalForceD(ElemK, nIJ, nLcomK, MembLcomD.CheckLcomD.GlobalForceD[0].LocalForceBeforeD, FALSE);
	else Get_LocalForceD(ElemK, nIJ, nLcomK, MembLcomD.CheckLcomD.GlobalForceD[0].LocalForceAfterD);
}

void CDgnRUSStlCompDataCtrl::ConvertStlCompGForce2Engine(T_SCGD_DGN_FORCE_UNIT &Elem, STL_COMP_FORCE_D &ChkForD)
{
	// Convert Global Force 
	for (int i = 0; i < 6; ++i)
	{
		ChkForD.ForceBefore.dForce[i] = Elem.ForBC.dForce[i];
		ChkForD.ForceAfter.dForce[i] = Elem.ForAC.dForce[i];
		ChkForD.ForceAfterDL.dForce[i] = Elem.ForceAfterDL.dForce[i];
		ChkForD.ForceAfer_Crp.dForce[i] = Elem.ForceAfer_Crp.dForce[i];
		ChkForD.ForceAfer_Shr.dForce[i] = Elem.ForceAfer_Shr.dForce[i];
		ChkForD.ForceAfer_BST.dForce[i] = Elem.ForceAfer_BST.dForce[i];
	}

	// Convert  Internal Stress
	ChkForD.CreepInterStress.dStress_Ax = Elem.InterStrCrp.dStress_Ax;
	ChkForD.CreepInterStress.dStress_Ax_P2 = Elem.InterStrCrp.dStress_Ax_P2;
	ChkForD.ShrinkageInterStress.dStress_Ax = Elem.InterStrShr.dStress_Ax;
	ChkForD.ShrinkageInterStress.dStress_Ax_P2 = Elem.InterStrShr.dStress_Ax_P2;
	ChkForD.TemperatureInterStress.dStress_Ax = Elem.InterStrBST.dStress_Ax;
	ChkForD.TemperatureInterStress.dStress_Ax_P2 = Elem.InterStrBST.dStress_Ax_P2;

	for (int i = 0; i < 9; ++i)
	{
		ChkForD.CreepInterStress.dStress_M_y[i] = Elem.InterStrCrp.dStress_M_y[i];
		ChkForD.CreepInterStress.dStress_M_z[i] = Elem.InterStrCrp.dStress_M_z[i];
		ChkForD.CreepInterStress.dStress_M_y_P2[i] = Elem.InterStrCrp.dStress_M_y_P2[i];
		ChkForD.CreepInterStress.dStress_M_z_P2[i] = Elem.InterStrCrp.dStress_M_z_P2[i];

		ChkForD.ShrinkageInterStress.dStress_M_y[i] = Elem.InterStrShr.dStress_M_y[i];
		ChkForD.ShrinkageInterStress.dStress_M_z[i] = Elem.InterStrShr.dStress_M_z[i];
		ChkForD.ShrinkageInterStress.dStress_M_y_P2[i] = Elem.InterStrShr.dStress_M_y_P2[i];
		ChkForD.ShrinkageInterStress.dStress_M_z_P2[i] = Elem.InterStrShr.dStress_M_z_P2[i];

		ChkForD.TemperatureInterStress.dStress_M_y[i] = Elem.InterStrBST.dStress_M_y[i];
		ChkForD.TemperatureInterStress.dStress_M_z[i] = Elem.InterStrBST.dStress_M_z[i];
		ChkForD.TemperatureInterStress.dStress_M_y_P2[i] = Elem.InterStrBST.dStress_M_y_P2[i];
		ChkForD.TemperatureInterStress.dStress_M_z_P2[i] = Elem.InterStrBST.dStress_M_z_P2[i];
	}
}