
// DgnStlRatingDataCtrl.cpp: implementation of the CDgnStlRatingDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnRCRatingDataCtrl.h"
#include "CRCForceCtrl.h"
//#include "Dgn_CSGManager.h"
#include "Dgn_RatingManager.h"
#include "DgnDataCtrl.h"
#include "DgnCodeCtrl.h"
#include "DgnForceCtrl.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\SectDB.h"
#include "..\wg_db\PostCtrl.h"
//#include "..\wg_db\PlateGirderDesign.h"
//#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
//#include "..\wg_base\wg_base_I_PolyMaker.h"
//#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_db\DesignLoadRating.h"
//#include "..\wg_umd\UMDC_DesignManagerBase.h"
#include "..\wg_db\SelfWgt.h"

const double DgnZero  = 1.0e-07;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnRCRatingDataCtrl::CDgnRCRatingDataCtrl() : CDgnPscCommon()
{
	m_nDgnCode = m_iCivilCode;

	m_pRatingManager = new CDgn_RatingManager;
	m_pDgnDataCtrl = new CDgnDataCtrl;
	
	// Make Rebar DB Data
	m_pDoc->m_pMatlDB->Create_RebarData(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_N,TRUE);
}

CDgnRCRatingDataCtrl::~CDgnRCRatingDataCtrl()
{
	if(m_pRatingManager!=NULL) 
	{
		delete m_pRatingManager;
		m_pRatingManager = NULL;
	}

	if(m_pDgnDataCtrl!=NULL) 
	{
		delete m_pDgnDataCtrl;
		m_pDgnDataCtrl = NULL;
	}

}

void CDgnRCRatingDataCtrl::SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl)
{
	CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	T_UNIT_INDEX CodeUnit = CurrUnit;
	switch (m_nDgnCode)
	{
	case KSCE_USD10_RC_RATING:
	case KSCE_RAIL_USD11_RC_RATING:
	case KSCE_LSD15_RC_RATING:
		{
			CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
			CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
			m_pDataCtrl->Set_UnitIndex(CodeUnit);
			break;
		}    
	default: ASSERT(0); break;
	}

	InitialData(m_nDgnCode);

}

	void CDgnRCRatingDataCtrl::SetDataCtrlPointer4Rating(int nCodeRC, CCRCDataCtrl* pDataCtrl)
	{
		CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);
		// Set Code Unit.
		T_UNIT_INDEX CodeUnit; 
		CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
		m_pDataCtrl->Set_UnitIndex(CodeUnit);	

		m_iCivilCode = nCodeRC;
	}

BOOL CDgnRCRatingDataCtrl::InitialData(int nDgnCode)
{
	if(!m_pRatingManager->InitialData(DGNE_CODETYPE_RC, nDgnCode)) return FALSE;
	return TRUE;
}


BOOL CDgnRCRatingDataCtrl::MakeRCRatingDesign()
{
	// Beam 
	if(!MakeRCRatingBeamDesign()) return FALSE;

 // Plate
	if(!MakeRCRatingPlateDesign()) return FALSE;

		return TRUE;
}

BOOL CDgnRCRatingDataCtrl::ConvertRCRatingInputData()
{
//   // Beam 
//   if(!ConvertRCRatingBeamInputData()) return FALSE;
// 
//   // Plate
//   if(!ConvertRCRatingPlateInputData()) return FALSE;

		return TRUE;
}


BOOL CDgnRCRatingDataCtrl::ConvertRCRatingResultData()
{

	// Beam 
	if(!ConvertRCRatingBeamResultData()) return FALSE;

	// Plate
	if(!ConvertRCRatingPlateResultData()) return FALSE;

		return TRUE;
}


BOOL CDgnRCRatingDataCtrl::ExcuteRCRatingDesign(int nType)
{
	// Beam 
	if(nType==0)
	{
		if(!ExcuteRCBeamRatingDesign()){return FALSE;}
	}
	else if(nType==1)
	{
		// Plate
		if(!ExcuteRCPlateRatingDesign()){return FALSE;}
	}
	else ASSERT(0);

	return TRUE;
}

// RC Beam
BOOL CDgnRCRatingDataCtrl::MakeRCRatingBeamDesign()
{
		return TRUE;
}

BOOL CDgnRCRatingDataCtrl::ConvertRCRatingBeamInputData(T_ELEM_K ElemK, _UMD_RC_BEAM_KCI &rData, int nIJ /*= 0*/)
{
//   if(!Get_RCRatingBeamSectD(ElemK, rData.SectRCData)) return FALSE;
//   if(!Get_RCRatingBeamRebarD(ElemK, rData.SectRCData, rData.RbarRCData, nIJ)) return FALSE;
//   if(!Get_RCRatingBeamMatlD(ElemK, rData.MatlRCData)) return FALSE;

	return TRUE;
}


BOOL CDgnRCRatingDataCtrl::ConvertRCRatingBeamResultData()
{
		return TRUE;
}

BOOL CDgnRCRatingDataCtrl::ExcuteRCBeamRatingDesign()
{
	CArray<T_ELEM_K, T_ELEM_K> aDgnElemK; aDgnElemK.RemoveAll();
	m_pDataCtrl->Get_ElemListForBlrDgn_RC(FALSE, aDgnElemK);
	INT_PTR nDgnElemNum =  aDgnElemK.GetSize();
	if(nDgnElemNum < 1) return FALSE;

	// Get Rating Case Key List
	CArray<T_RKLC_K, T_RKLC_K>  RCaseKeyList; RCaseKeyList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetRklcKeyList(RCaseKeyList);
	INT_PTR nRCaseSize = RCaseKeyList.GetSize();
	if(nRCaseSize < 1) return FALSE;
 
	int iRcaseNum = m_pForcCtrl->Get_LcomDataCount();

	T_RPAC_D RpacD;    
	if(!m_pDoc->m_pAttrCtrl2->GetRpac(RpacD)) RpacD.Initialize(m_nDgnCode);  
	double dDgnLiveLoad = RpacD.dDgnLiveLoad;
	int nSurveyMethod = RpacD.nSurveyMeth;
			

	_UMD_RC_BEAM_KCI InData;
	if(!Get_RCRatingBeamOptionD(InData.OptionData)) return FALSE;  // Input - Option

	// Repeat Elem.
	for(INT_PTR i = 0; i <nDgnElemNum; ++i)
	{
		T_ELEM_K ElemK = aDgnElemK[i];
		ElemPairK EPairK(ElemK, EN_EL_BEAM);

		// Get InData
		//ConvertRCRatingBeamInputData(ElemK, InData);
		if(!Get_RCRatingBeamSectD(ElemK, InData.SectRCData)) return FALSE; // Input - Sect
		if(!Get_RCRatingBeamMatlD(ElemK, InData.MatlRCData)) 
	{
		GSaveHistoryFormatNF(_LS(IDS_DGN_CON_BEAM_Rebar_Info_Err)); 
		return FALSE;
	}

		double dElast = InData.MatlRCData.dEc;

		// Get Rating Option
		T_RROC_D RrocD;
		if(!m_pDoc->m_pAttrCtrl2->GetRroc(ElemK, RrocD)) continue;

		// Make Check Force (Rating Case)
		FORCE_LRKR_ELEM  ForceLrkrElem, ForceLrkrElem_Str; 
	    STRESS_LRKR_ELEM StrssLrkrElem;

		BOOL bRrocKey = TRUE;
		if(!m_pDoc->m_pAttrCtrl2->GetRroc(ElemK, RrocD)) bRrocKey = FALSE;

		const auto& itForceElem = m_pForcCtrl->m_arBlrdForceElemKR.find(EPairK);
		if (itForceElem == m_pForcCtrl->m_arBlrdForceElemKR.end()) continue;
		ForceLrkrElem = itForceElem->second;

		ForceLrkrElem_Str.Initialize(); // For Strength Check
		const auto& itForceElem_Str = m_pForcCtrl->m_arBlrdForceElemKR_Str.find(EPairK);
		if (itForceElem_Str == m_pForcCtrl->m_arBlrdForceElemKR_Str.end()) continue;
		ForceLrkrElem_Str = itForceElem_Str->second;

		StrssLrkrElem.Initialize();
		const auto& itStressElem = m_pForcCtrl->m_arBlrdStressElemKR.find(EPairK);
		if (itStressElem == m_pForcCtrl->m_arBlrdStressElemKR.end()) continue;
		StrssLrkrElem = itStressElem->second;

		CArray<FORCE_RCASE_KR,FORCE_RCASE_KR> arForceRCase; arForceRCase.RemoveAll();
		arForceRCase.Copy(ForceLrkrElem.arForceRCase);

		CArray<FORCE_RCASE_KR,FORCE_RCASE_KR> arForceRCase_Str; arForceRCase_Str.RemoveAll();
		arForceRCase_Str.Copy(ForceLrkrElem_Str.arForceRCase);

		CArray<STRESS_RCASE_KR,STRESS_RCASE_KR> arStessRCase; arStessRCase.RemoveAll();
		arStessRCase.Copy(StrssLrkrElem.arStressRCase);

		double dEndPosiM_P0a = 1.0/pow(m_dZero,10);
		double dEndNegaM_P0a = 1.0/pow(m_dZero,10);

		T_RFRC_BASE PosiM_Rfrc; PosiM_Rfrc.Initialize(); 
		T_RFRC_BASE NegaM_Rfrc; NegaM_Rfrc.Initialize();

		T_RFRC_CASE PosiMinRfrcD; PosiMinRfrcD.Initialize();
		T_RFRC_CASE NegaMinRfrcD; NegaMinRfrcD.Initialize();
	 
		// 최소 RF를 못 찾으면 이대로 할당.
		PosiM_Rfrc.ElemK = ElemK;
		NegaM_Rfrc.ElemK = ElemK;

		T_RFRC_CASE RfrcCase;

		// Repeat I & J
		for(int k = 0; k < 2; ++k)
		{
			if(k == 0 && RrocD.nPosition == 1) continue;    // Skip for I
			if(k == 1 && RrocD.nPosition == 0) continue;    // Skip for J

			CString strPosi;
			if     (k == 0) strPosi = _T("I");
			else if(k == 1) strPosi = _T("J");
			else ASSERT(0);

			CString strName; strName.Format(_T("E%i-%s"), ElemK, strPosi);
			CStringW strwName(strName);
			InData.strName = strwName;
			if(!Get_RCRatingBeamRebarD(ElemK, InData.SectRCData, InData.RbarRCData, k)) continue; // Input - Rebar
			
			int iGetData = 0;

			// Critical Rating Case Load for Positive/Negative
			FORCE_RCASE_KR RCaseForce_Positive; RCaseForce_Positive.Initialize();
			FORCE_RCASE_KR RCaseForce_Negative; RCaseForce_Negative.Initialize();

			// Repeat Each Rating Case
			for(int j=0; j<iRcaseNum; ++j)
			{
				int iRcaseNo = j+1;
				int iOrgRcaseNo=0, iRatingCase=0, iRcaseTypeDL=0, iRcaseTypeLL=0;
				if(!m_pForcCtrl->Get_RcaseType(iRcaseNo, iOrgRcaseNo, iRatingCase, iRcaseTypeDL, iRcaseTypeLL)) ASSERT(0);

				iGetData++;

				T_RKLC_D RklcD; RklcD.Initialize();
				m_pDoc->m_pAttrCtrl->GetRklc(iOrgRcaseNo,RklcD);

				// Get Disp.
				double dIdispZ=0.0, dJdispZ=0.0; 
				m_pDataCtrl->Get_Deflection4NodeOfElem(EPairK, iRcaseNo, dIdispZ, dJdispZ, ENUM_KS05);

				FORCE_RCASE_KR  ForceRcase  = arForceRCase[iGetData-1];
				FORCE_RCASE_KR  ForceRcase_Str  = arForceRCase_Str[iGetData-1];
				ForceRcase_Str.ForceG.Initialize();
				ForceRcase_Str.ForceG += ForceRcase_Str.ForceLL;
				ForceRcase_Str.ForceG *= (1.0+RklcD.dImpFactor);
				ForceRcase_Str.ForceG += ForceRcase_Str.ForceDL;
	      STRESS_RCASE_KR StressRcase = arStessRCase[iGetData-1];
				//FORCE_RCASE_KR  ForceRcase_Str  = arForceRCase_Str[iGetData-1];

				RfrcCase.Initialize();

				int nConcurrent = m_pDataCtrl->Get_ConCurrType(iRcaseTypeDL, iRcaseTypeLL);
				RfrcCase.RfrcBase[k].bCheck =(bRrocKey)? m_pDataCtrl->Get_ChkElemFlag4RatingKR(RrocD, k) : FALSE;

				RfrcCase.RfrcBase[k].bCheck2    = TRUE; 
				RfrcCase.RfrcBase[k].ElemK      = ElemK;
				RfrcCase.RfrcBase[k].nRCaseK    = iOrgRcaseNo;
				RfrcCase.RfrcBase[k].nDgnRCaseK = iRcaseNo;
				RfrcCase.RfrcBase[k].nConcurrent= nConcurrent;
				RfrcCase.RfrcBase[k].dMd        = ForceRcase.ForceDL.dMuy[k];
				RfrcCase.RfrcBase[k].dMd_m      = ForceRcase.ForceDL.dMuy[k];
				RfrcCase.RfrcBase[k].dM_L       = ForceRcase.ForceLL.dMuy[k];
				RfrcCase.RfrcBase[k].dCalc_defl = k==0 ? dIdispZ : dJdispZ;
				RfrcCase.RfrcBase[k].dRealDispZ = k==0 ? RrocD.dRealDeflectionI : RrocD.dRealDeflectionJ; 
				RfrcCase.RfrcBase[k].dImpFactor = RklcD.dImpFactor;
				RfrcCase.RfrcBase[k].dRealImpFactor     = k==0 ? RrocD.dRealCrashCoeffI : RrocD.dRealCrashCoeffJ;
				RfrcCase.RfrcBase[k].dDgnLoad           = dDgnLiveLoad;
				RfrcCase.RfrcBase[k].dAddResponceFactor = k==0 ? RrocD.dAdjustmentFactorI : RrocD.dAdjustmentFactorJ;


				double dStl = StressRcase.StressLL.dStl[k];
				double dStr = StressRcase.StressLL.dStr[k];
				double dSbr = StressRcase.StressLL.dSbr[k];
				double dSbl = StressRcase.StressLL.dSbl[k];
				double dmax01 = max(dStl, dStr);
				double dmax02 = max(dSbr, dSbl);
				RfrcCase.RfrcBase[k].dFl       = max(dmax01, dmax02); // +: 활하중에 의한 인장응력 중 가장 큰 것 
				RfrcCase.RfrcBase[k].dEstiEwip = (k==0)? RrocD.dStrainI * 0.001 : RrocD.dStrainJ * 0.001;
				RfrcCase.RfrcBase[k].dCalcEwip =  dElast < m_dZero ? 0.0 : RfrcCase.RfrcBase[k].dFl / dElast;
				RfrcCase.RfrcBase[k].dMdl      = RfrcCase.RfrcBase[k].dMd + RfrcCase.RfrcBase[k].dM_L;
				RfrcCase.RfrcBase[k].strLcomName.Format(_T("%s(%s)"), RklcD.strCaseName, GetMaxMinLcomType(nConcurrent));

				// Convert LoadD : 휨강도 계산용. 활하중으로만 계산
				//CString strLcomName = RfrcCase.RfrcBase[k].strLcomName;
				Get_RCRatingBeamLcomD(ForceRcase, InData.LoadData, k, TRUE);

				// Get Strength Data
				_UMD_RC_KR_BEAM_RES EngineResD; EngineResD.Initialize();
				m_pRatingManager->Check_MemberRes_KSCE_USD_RCRating_Beam(InData, EngineResD);
				double dMuy4Chk =  RfrcCase.RfrcBase[k].dMd  + RfrcCase.RfrcBase[k].dM_L ;
				double dCapacity = 0.0;
				if(m_nDgnCode == KSCE_LSD15_RC_RATING)
				{
					dCapacity = dMuy4Chk > 0.0 ? EngineResD.ElemRes.StrnResD[0].dpMn : EngineResD.ElemRes.StrnResD[1].dpMn;
				}
				else
				{
					dCapacity = dMuy4Chk > 0.0 ?EngineResD.arStrnLcomData[0].dpMnPChk : EngineResD.arStrnLcomData[0].dpMnNChk;
				}

				// Calculating Rating  Result
				Calc_RatingResult(nSurveyMethod, dCapacity, RfrcCase.RfrcBase[k]);
				ConvertForce2Engine(ForceRcase,     RfrcCase.RfrcBase[k].Rating_Force,     k);
				ConvertForce2Engine(ForceRcase_Str, RfrcCase.RfrcBase[k].Rating_Force_Str, k);
				 
				// Find Max(Positive) & Min(Negative) Case
				T_RFRC_BASE RfrcBase = RfrcCase.RfrcBase[k];
				CString strLcomType_max, strLcomType_min;
				if(RfrcBase.bCheck2) 
				{
					m_pDoc->m_pAttrCtrl->GetRklc(RfrcBase.nRCaseK,RklcD);
					RfrcBase.strLcomName.Format(_T("%s(%s)"), RklcD.strCaseName, GetMaxMinLcomType(RfrcBase.nConcurrent));

					if(RfrcBase.dM_L> -DgnZero) // Positive
					{
						if(dEndPosiM_P0a>RfrcBase.dP0a)
						{
							dEndPosiM_P0a       = RfrcBase.dP0a;
							PosiM_Rfrc          = RfrcBase;					
							RCaseForce_Positive = ForceRcase_Str;
						}
					}
					else if(RfrcBase.dM_L<0.0) // Negative
					{
						if(dEndNegaM_P0a>RfrcBase.dP0a)
						{
							dEndNegaM_P0a       = RfrcBase.dP0a;
							NegaM_Rfrc          = RfrcBase;
							RCaseForce_Negative = ForceRcase_Str;
						}
					}
				}
			} //iRcaseNum

			// Get Safety Rank
			int nSafetyRank = 0;
			double dRat_MnMu = 0.0;

			// Make Safety Rank for Positive
			_UMD_RC_KR_BEAM_RES EngineResD_Postive; EngineResD_Postive.Initialize();
			Get_RCRatingBeamLcomD(RCaseForce_Positive, InData.LoadData, k);
			m_pRatingManager->Check_MemberRes_KSCE_USD_RCRating_Beam(InData, EngineResD_Postive);
			double dMu_Posi = InData.LoadData.LcomList_Str[0].dForce[4];
			BOOL bMposi = (dMu_Posi>=0.0);
			double dMn_Posi = 0.0;
			if(m_nDgnCode == KSCE_LSD15_RC_RATING)
			{
				dMn_Posi = bMposi ? EngineResD_Postive.ElemRes.StrnResD[0].dpMn : EngineResD_Postive.ElemRes.StrnResD[1].dpMn;
			}
			else
			{
				dMn_Posi = bMposi ? EngineResD_Postive.arStrnLcomData[0].dpMnPChk : EngineResD_Postive.arStrnLcomData[0].dpMnNChk;
			}
			GetRatingRank(bMposi, dMu_Posi, dMn_Posi, PosiM_Rfrc.dP0a,dRat_MnMu, nSafetyRank);
			PosiM_Rfrc.dMu_Rank    = dMu_Posi;
			PosiM_Rfrc.dPhiMn_Rank = dMn_Posi;
			PosiM_Rfrc.dRatioMa    = dRat_MnMu;
			PosiM_Rfrc.nRankType   = nSafetyRank;

			// Make Safety Rank for Negative
			_UMD_RC_KR_BEAM_RES EngineResD_Negative; EngineResD_Negative.Initialize();
			Get_RCRatingBeamLcomD(RCaseForce_Negative, InData.LoadData, k);
			m_pRatingManager->Check_MemberRes_KSCE_USD_RCRating_Beam(InData, EngineResD_Negative);
			double dMu_Nega = InData.LoadData.LcomList_Str[0].dForce[4];
			bMposi = (dMu_Nega>=0.0);
			double dMn_Nega = 0.0;
			if(m_nDgnCode == KSCE_LSD15_RC_RATING)
			{
				dMn_Nega = bMposi ? EngineResD_Negative.ElemRes.StrnResD[0].dpMn : EngineResD_Negative.ElemRes.StrnResD[1].dpMn;
			}
			else
			{
				dMn_Nega = bMposi ? EngineResD_Negative.arStrnLcomData[0].dpMnPChk : EngineResD_Negative.arStrnLcomData[0].dpMnNChk;
			}
			GetRatingRank(bMposi, dMu_Nega, dMn_Nega, NegaM_Rfrc.dP0a,dRat_MnMu, nSafetyRank);
			NegaM_Rfrc.dMu_Rank    = dMu_Nega;
			NegaM_Rfrc.dPhiMn_Rank = dMn_Nega;
			NegaM_Rfrc.dRatioMa    = dRat_MnMu;
			NegaM_Rfrc.nRankType   = nSafetyRank;

			// Save Critical Result
			PosiMinRfrcD.RfrcBase[k] = PosiM_Rfrc;
			NegaMinRfrcD.RfrcBase[k] = NegaM_Rfrc;

		} // k (I/J)

	 // Write Result
	 int iConCurr = 0; 
	 int iMaxMin = 0; 
	 T_RKLC_K RklcKey = 0;
	 iMaxMin = 1; // Max (Positive)
	 m_pDoc->m_pPostCtrl->GetLoadRating()->WriteRCRatingResult4KR_USD(ElemK, RklcKey, iMaxMin, iConCurr, PosiMinRfrcD);

	 iMaxMin = 2; // Min (Negative)
	 m_pDoc->m_pPostCtrl->GetLoadRating()->WriteRCRatingResult4KR_USD(ElemK, RklcKey, iMaxMin, iConCurr, NegaMinRfrcD);
	 
	}

	return TRUE;
}


CString CDgnRCRatingDataCtrl::GetMaxMinLcomType(const int &iLcomType)
{
	CString strLcomType=_T("");
	if(iLcomType==0)        strLcomType = _T("-");
	else if(iLcomType==1)   strLcomType = _T("max");
	else if(iLcomType==2)   strLcomType = _T("min");
	else if(iLcomType==3)   strLcomType = _T("max");
	else if(iLcomType==4)   strLcomType = _T("min");
	else if(iLcomType==5)   strLcomType = _T("max");
	else if(iLcomType==6)   strLcomType = _T("min");
	else if(iLcomType==7)   strLcomType = _T("max");
	else if(iLcomType==8)   strLcomType = _T("min");
	else if(iLcomType==9)   strLcomType = _T("max");
	else if(iLcomType==10)  strLcomType = _T("min");
	else if(iLcomType==11)  strLcomType = _T("max");
	else if(iLcomType==12)  strLcomType = _T("min");
	else                    strLcomType = _T("-");

	return strLcomType;
}


BOOL CDgnRCRatingDataCtrl::Get_RCRatingBeamStrength_Print(_UMD_RC_BEAM_KCI &InData4Rpt,  _UMD_RC_KR_BEAM_RES &DgnEngineResD)
{
	BOOL bCalcOK=TRUE;

	// Get Strength Check Result 
	DgnEngineResD.Initialize();
	bCalcOK = m_pRatingManager->Check_MemberRes_KSCE_USD_RCRating_Beam(InData4Rpt, DgnEngineResD);
	
	return bCalcOK;  
}

// RC Plate
BOOL CDgnRCRatingDataCtrl::MakeRCRatingPlateDesign()
{
		return TRUE;
}

BOOL CDgnRCRatingDataCtrl::ConvertRCRatingPlateInputData(T_RRPL_D &RrplD, _UMD_RC_BEAM_KCI &rData)
{
	if(!Get_RCRatingPlateSectD(RrplD, rData.SectRCData)) return FALSE;
	if(!Get_RCRatingPlateRebarD(RrplD, rData.SectRCData, rData.RbarRCData)) return FALSE;
	if(!Get_RCRatingPlateMatlD(RrplD, rData.MatlRCData))
	{
	  GSaveHistoryFormatNF(_LS(IDS_DGN_CON_BEAM_Rebar_Info_Err)); 
	  return FALSE;
	}

	return TRUE;
}

BOOL CDgnRCRatingDataCtrl::ConvertRCRatingPlateResultData()
{
	return TRUE;
}

BOOL CDgnRCRatingDataCtrl::ExcuteRCPlateRatingDesign()
{
	//CArray<T_ELEM_K, T_ELEM_K> aDgnElemK; aDgnElemK.RemoveAll();
	
	CArray<T_RRPL_K, T_RRPL_K> aDgnMembK; aDgnMembK.RemoveAll();
	
	m_pDataCtrl->Get_ElemListForBlrDgn_RC_Plate(FALSE, aDgnMembK);
	int nDgnMemNum =  aDgnMembK.GetSize();
	if(nDgnMemNum < 1) return FALSE;

	// Get Rating Case Key List
	CArray<T_RKLC_K, T_RKLC_K>  RCaseKeyList; RCaseKeyList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetRklcKeyList(RCaseKeyList);
	INT_PTR nRCaseSize = RCaseKeyList.GetSize();
	if(nRCaseSize < 1) return FALSE;

	//int iRcaseNum = m_pForcCtrl->Get_LcomDataCount();
	int iRcaseNum = m_pForcCtrl->m_pDgnForceCtrl->m_iLCBAddDesignNum;

	T_RPAC_D RpacD;    
	if(!m_pDoc->m_pAttrCtrl2->GetRpac(RpacD)) RpacD.Initialize(m_nDgnCode);  
	double dDgnLiveLoad = RpacD.dDgnLiveLoad;
	int nSurveyMethod = RpacD.nSurveyMeth;

	_UMD_RC_BEAM_KCI InData;
	if(!Get_RCRatingBeamOptionD(InData.OptionData)) return FALSE;

	// Repeat Membk.
	for(int i = 0; i <nDgnMemNum; ++i)
	{
		T_RRPL_K RrplK = aDgnMembK[i];

		T_RRPL_D RrplD;
		if(!m_pDoc->m_pAttrCtrl2->GetRrpl(RrplK, RrplD)) 
		{
			AfxMessageBox(_T("There is no Rrpl Data."));
			continue;
		}

		// Get InData

		// 단면, 철근, 재료
		CString strName; strName.Format(_T("Memb%i-%s"), RrplK, RrplD.strName);
		CStringW cstingwName(strName);
		InData.strName = cstingwName;

		if(!ConvertRCRatingPlateInputData(RrplD, InData)) 
		{
			AfxMessageBox(_T("There is no Beam Input Data."));
			continue;
		}
		
		double dElast = InData.MatlRCData.dEc;

		// Get Rating Option
		T_RROP_D RropD;
		if(!m_pDoc->m_pAttrCtrl2->GetRrop(RrplK, RropD)) continue;

		if(!m_pDataCtrl->Get_RcplDataRC(RrplK)) continue; // plate 멤버의 하중 정보

		double dEndPosiM_P0a = 1.0/pow(m_dZero,10);
		double dEndNegaM_P0a = 1.0/pow(m_dZero,10);

		T_RFRC_BASE PosiM_Rfrp; PosiM_Rfrp.Initialize(); 
		T_RFRC_BASE NegaM_Rfrp; NegaM_Rfrp.Initialize();

		T_RFRP_CASE PosiMinRfrpD; PosiMinRfrpD.Initialize();
		T_RFRP_CASE NegaMinRfrpD; NegaMinRfrpD.Initialize();

		// 최소 RF를 못 찾으면 이대로 할당.
		PosiM_Rfrp.ElemK = RrplK;
		NegaM_Rfrp.ElemK = RrplK;

		T_RFRP_CASE RfrpCase;

		//  I/J 구분없다. 
		for(int k = 0; k < 1; ++k)
		{
			if( RropD.nRatingChk == 0) continue;    
			
			int iGetData = 0;

			// Critical Rating Case Load for Positive/Negative
			FORCE_RCASE_KR RCaseForce_Positive; RCaseForce_Positive.Initialize();
			FORCE_RCASE_KR RCaseForce_Negative; RCaseForce_Negative.Initialize();

			// Repeat Each Rating Case
			for(int j=0; j<iRcaseNum; ++j)
			{
				int iRcaseNo = j+1;
				int iOrgRcaseNo=0, iRatingCase=0, iRcaseTypeDL=0, iRcaseTypeLL=0;
				if(!m_pForcCtrl->Get_RcaseType(iRcaseNo, iOrgRcaseNo, iRatingCase, iRcaseTypeDL, iRcaseTypeLL)) ASSERT(0);

				iGetData++;

				T_RKLC_D RklcD; RklcD.Initialize();
				m_pDoc->m_pAttrCtrl->GetRklc(iOrgRcaseNo, RklcD);

				int iLcomNo = iRcaseNo; //j+1;

				// Plate Force
				Set_InputDataByLcom(RrplK, iLcomNo);  // (중요) 하중조합별로 돌면서 부재력 만들어낸다.

				FORCE_RCASE_KR  ForceRcase;     //  = arForceRCase[iGetData-1];
				STRESS_RCASE_KR StressRcase;    // = arStessRCase[iGetData-1];
				FORCE_RCASE_KR  ForceRcase_Str; //  = arForceRCase_Str[iGetData-1];

				Make_FORCE_RCASE_Plate(RrplD, FALSE, ForceRcase);      // RC Plate Rating용 부재력 받아옴.
				Make_FORCE_RCASE_Plate(RrplD, TRUE,  ForceRcase_Str);  // RC Plate 등급검토용 부재력 받아옴.

				ForceRcase_Str.ForceG.Initialize();
				ForceRcase_Str.ForceG += ForceRcase_Str.ForceLL;
				ForceRcase_Str.ForceG *= (1.0+RklcD.dImpFactor);
				ForceRcase_Str.ForceG += ForceRcase_Str.ForceDL;

				// Get Disp.
				double dDisp=0.0;
				T_NODE_K MinNodeK=0;
				m_pDataCtrl->Get_Deflection4Plate(RrplK, iRcaseNo, MinNodeK, dDisp);

				RfrpCase.Initialize();

				int nConcurrent = m_pDataCtrl->Get_ConCurrType(iRcaseTypeDL, iRcaseTypeLL);
				RfrpCase.RfrpBase[k].bCheck = TRUE; //(bRrocKey)? m_pDataCtrl->Get_ChkElemFlag4RatingKR(RrocD, k) : FALSE;

				RfrpCase.RfrpBase[k].bCheck2= TRUE; 
				RfrpCase.RfrpBase[k].ElemK              = RrplK;
				RfrpCase.RfrpBase[k].nRCaseK            = iOrgRcaseNo;
				RfrpCase.RfrpBase[k].nDgnRCaseK         = iRcaseNo;
				RfrpCase.RfrpBase[k].nConcurrent        = nConcurrent;
				RfrpCase.RfrpBase[k].dMd                = ForceRcase.ForceDL.dMuy[k];
				RfrpCase.RfrpBase[k].dMd_m              = ForceRcase.ForceDL.dMuy[k];
				RfrpCase.RfrpBase[k].dM_L               = ForceRcase.ForceLL.dMuy[k];
				RfrpCase.RfrpBase[k].dCalc_defl         = dDisp;
				RfrpCase.RfrpBase[k].MinNodeK           = MinNodeK;
				RfrpCase.RfrpBase[k].dRealDispZ         = RropD.dDefl;
				RfrpCase.RfrpBase[k].dImpFactor         = RklcD.dImpFactor;
				RfrpCase.RfrpBase[k].dRealImpFactor     = RropD.dImpactFactor;
				RfrpCase.RfrpBase[k].dDgnLoad           = dDgnLiveLoad;
				RfrpCase.RfrpBase[k].dAddResponceFactor = RropD.dAdjustmentFactor;

				// Test
				double dH = InData.SectRCData.dSize[0]; // 두께
				double dB = InData.SectRCData.dSize[1]; // 폭
				double dZ = dB * dH * dH/6.0;
				double dMy = RfrpCase.RfrpBase[k].dM_L;

				double dStl = -1*dMy/dZ; //StressRcase.StressLL.dStl[k];
				double dStr = -1*dMy/dZ; //StressRcase.StressLL.dStr[k];
				double dSbr = dMy/dZ; //StressRcase.StressLL.dSbr[k];
				double dSbl = dMy/dZ; //StressRcase.StressLL.dSbl[k];
				double dmax01 = max(dStl, dStr);
				double dmax02 = max(dSbr, dSbl);
				RfrpCase.RfrpBase[k].dFl       = max(dmax01, dmax02);   // +: 활하중에 의한 인장응력 중 가장 큰 것 :+ 인장
				RfrpCase.RfrpBase[k].dEstiEwip = RropD.dStrain * 0.001; 
				RfrpCase.RfrpBase[k].dCalcEwip = dElast < m_dZero ? 0.0 : RfrpCase.RfrpBase[k].dFl / dElast;
				RfrpCase.RfrpBase[k].dMdl      = ForceRcase.ForceG.dMuy[k]; //RfrpCase.RfrpBase[k].dMd + RfrpCase.RfrpBase[k].dM_L;
				RfrpCase.RfrpBase[k].strLcomName.Format(_T("%s(%s)"), RklcD.strCaseName, GetMaxMinLcomType(nConcurrent));

				// Convert LoadD : 휨강도 검토용.(활하중을 기준으로 정/부 결정)
				//CString strLcomName = RfrpCase.RfrpBase[k].strLcomName;
				Get_RCRatingBeamLcomD(ForceRcase, InData.LoadData, k);

				// Get Strength Data
				_UMD_RC_KR_BEAM_RES EngineResD; EngineResD.Initialize();
				m_pRatingManager->Check_MemberRes_KSCE_USD_RCRating_Beam(InData, EngineResD); // 강도 검토
				double dMuy4Chk =  /*RfrpCase.RfrpBase[k].dMd  +*/ RfrpCase.RfrpBase[k].dM_L ;
				double dCapacity = 0.0;
				if(m_nDgnCode == KSCE_LSD15_RC_RATING)
				{
					dCapacity = dMuy4Chk > 0.0 ? EngineResD.ElemRes.StrnResD[0].dpMn : EngineResD.ElemRes.StrnResD[1].dpMn;
				}
				else
				{
					dCapacity = dMuy4Chk > 0.0 ? EngineResD.arStrnLcomData[0].dpMnPChk : EngineResD.arStrnLcomData[0].dpMnNChk;
				}

				// Calculating Rating  Result
				Calc_RatingResult(nSurveyMethod, dCapacity, RfrpCase.RfrpBase[k]);
				ConvertForce2Engine(ForceRcase,     RfrpCase.RfrpBase[k].Rating_Force,     k);
				ConvertForce2Engine(ForceRcase_Str, RfrpCase.RfrpBase[k].Rating_Force_Str, k);

				// Find Max(Positive) & Min(Negative) Case
				T_RFRC_BASE RfrcBase = RfrpCase.RfrpBase[k];
				CString strLcomType_max, strLcomType_min;
				if(RfrcBase.bCheck2) 
				{
					m_pDoc->m_pAttrCtrl->GetRklc(RfrcBase.nRCaseK,RklcD);
					RfrcBase.strLcomName.Format(_T("%s(%s)"), RklcD.strCaseName, GetMaxMinLcomType(RfrcBase.nConcurrent));

					if(RfrcBase.dM_L> -DgnZero) // Positive
					{
						if(dEndPosiM_P0a>RfrcBase.dP0a)
						{
							dEndPosiM_P0a       = RfrcBase.dP0a;
							PosiM_Rfrp          = RfrcBase;					
							RCaseForce_Positive = ForceRcase_Str;
						}
					}
					else if(RfrcBase.dM_L<0.0) // Negative
					{
						if(dEndNegaM_P0a>RfrcBase.dP0a)
						{
							dEndNegaM_P0a       = RfrcBase.dP0a;
							NegaM_Rfrp          = RfrcBase;
							RCaseForce_Negative = ForceRcase_Str;
						}
					}
				}
			} // iRcaseNum

			// Get Safety Rank
			int nSafetyRank = 0;
			double dRat_MnMu = 0.0;

			// plate
			// Make Safety Rank for Positive
			_UMD_RC_KR_BEAM_RES EngineResD_Postive; EngineResD_Postive.Initialize();
			Get_RCRatingBeamLcomD(RCaseForce_Positive, InData.LoadData, k);
			m_pRatingManager->Check_MemberRes_KSCE_USD_RCRating_Beam(InData, EngineResD_Postive);
			double dMu_Posi = RCaseForce_Positive.ForceG.dMuy[k]; //InData.LoadData.LcomList_Str[0].dForce[4];
			BOOL bMposi = (dMu_Posi>=0.0);
			double dMn_Posi = 0.0;
			if(m_nDgnCode == KSCE_LSD15_RC_RATING)
			{
				dMn_Posi = bMposi ? EngineResD_Postive.ElemRes.StrnResD[0].dpMn : EngineResD_Postive.ElemRes.StrnResD[1].dpMn;
			}
			else
			{
				dMn_Posi = bMposi ? EngineResD_Postive.arStrnLcomData[0].dpMnPChk : EngineResD_Postive.arStrnLcomData[0].dpMnNChk;
			}
			GetRatingRank(bMposi, dMu_Posi, dMn_Posi, PosiM_Rfrp.dP0a,dRat_MnMu, nSafetyRank);
			PosiM_Rfrp.dMu_Rank    = dMu_Posi;
			PosiM_Rfrp.dPhiMn_Rank = dMn_Posi;
			PosiM_Rfrp.dRatioMa    = dRat_MnMu;
			PosiM_Rfrp.nRankType   = nSafetyRank;

			// Make Safety Rank for Negative
			_UMD_RC_KR_BEAM_RES EngineResD_Negative; EngineResD_Negative.Initialize();
			Get_RCRatingBeamLcomD(RCaseForce_Negative, InData.LoadData, k);
			m_pRatingManager->Check_MemberRes_KSCE_USD_RCRating_Beam(InData, EngineResD_Negative);
			double dMu_Nega = RCaseForce_Negative.ForceG.dMuy[k];
			bMposi = (dMu_Nega>=0.0);
			double dMn_Nega = 0.0;
			if(m_nDgnCode == KSCE_LSD15_RC_RATING)
			{
				dMn_Nega = bMposi ? EngineResD_Negative.ElemRes.StrnResD[0].dpMn : EngineResD_Negative.ElemRes.StrnResD[1].dpMn;
			}
			else
			{
				dMn_Nega = bMposi ? EngineResD_Negative.arStrnLcomData[0].dpMnPChk : EngineResD_Negative.arStrnLcomData[0].dpMnNChk;
			}
			GetRatingRank(bMposi, dMu_Nega, dMn_Nega, NegaM_Rfrp.dP0a,dRat_MnMu, nSafetyRank);
			NegaM_Rfrp.dMu_Rank    = dMu_Nega;
			NegaM_Rfrp.dPhiMn_Rank = dMn_Nega;
			NegaM_Rfrp.dRatioMa    = dRat_MnMu;
			NegaM_Rfrp.nRankType   = nSafetyRank;

			// Save Critical Result
			PosiMinRfrpD.RfrpBase[k] = PosiM_Rfrp;
			NegaMinRfrpD.RfrpBase[k] = NegaM_Rfrp;

		} // k (I/J)


		// Write Plate Result
		int iConCurr = 0; 
		int iMaxMin  = 0; 
		T_RKLC_K RklcKey = 0;
		iMaxMin = 1; // Max (Positive)
		m_pDoc->m_pPostCtrl->GetLoadRating()->WriteRCPlateRatingResult4KR_USD(RrplK, RklcKey, iMaxMin, iConCurr, PosiMinRfrpD);

		iMaxMin = 2; // Min (Negative)
		m_pDoc->m_pPostCtrl->GetLoadRating()->WriteRCPlateRatingResult4KR_USD(RrplK, RklcKey, iMaxMin, iConCurr, NegaMinRfrpD);

	} // DgnMemb


	return TRUE;
}

	BOOL CDgnRCRatingDataCtrl::ConvertDesignCode(_UMD_RC_BEAM_KCI &rData)
	{

		return TRUE;
	}

	BOOL CDgnRCRatingDataCtrl::ConvertCivilDgnCode2DgnCodeStr(int nCivilDgnCode, CString &strDgnCode, int &nEngineDgnCode)
	{
		strDgnCode =_T("");
		nEngineDgnCode = 0;

		switch (nCivilDgnCode)
		{
		case KSCE_USD10_RC_RATING:
			{
				nEngineDgnCode = KSCE_RC_USD10;
				break;
			}
		case KSCE_RAIL_USD11_RC_RATING:
			{
				nEngineDgnCode = KSCE_RC_RAIL_USD11;
				break;
			}
		case KSCE_LSD15_RC_RATING:
			{
				nEngineDgnCode = KR_LRFD11_RC;
				break;
			}
		default: ASSERT(FALSE);
			break;
		}

		strDgnCode = CDgnCodeCtrl::GetConCodeName(nEngineDgnCode);
		if (strDgnCode.IsEmpty()) return FALSE;

		return TRUE;
	}

BOOL CDgnRCRatingDataCtrl::Set_InputDataByLcom(T_MEMB_K RrplNo, int iLcomNo)
{
	// Get Load Combination Data.
	_DGN_LCOM LcomDesign;
	LcomDesign.Initialize();

	CDgnForceCtrl *pDgnForceCtrl = m_pForcCtrl->m_pDgnForceCtrl;


	pDgnForceCtrl->Set_LcomDataForRrpl(RrplNo);
	pDgnForceCtrl->m_parLcomData->Lookup(iLcomNo, LcomDesign);
//   pDgnForceCtrl->m_iLcomNo	 = LcomDesign.DesignLcomKey;
//   pDgnForceCtrl->m_iLcomType = LcomDesign.LcomUlData.nActive;
//   pDgnForceCtrl->m_iLcomType = 1;
	pDgnForceCtrl->Set_ForceMomentBySeperate_Rrpl(RrplNo, LcomDesign);	

	return TRUE;
}

BOOL CDgnRCRatingDataCtrl::Make_FORCE_RCASE_Plate(T_RRPL_D &RrplD, BOOL bAllDL, FORCE_RCASE_KR  &ForceRcase)
{
	CDgnForceCtrl *pDgnForceCtrl = m_pForcCtrl->m_pDgnForceCtrl;

	double dUnitWidth = 1000.0; //m_pMyDB->Get_UnitWidthLength();

	double dMp = 0.0;
	double dMm = 0.0;
	double dVu = 0.0;

	double dMp_DL = 0.0;
	double dMm_DL = 0.0;
	double dVu_DL = 0.0;

	double dMp_ML = 0.0;
	double dMm_ML = 0.0;
	double dVu_ML = 0.0;

	if(RrplD.iDir == 0)//설계 방향(0=X-Dir, 1=Y-Dir)
	{          
//     dMp = max(0.0, pDgnForceCtrl->m_Mxx_Rcpl[0]);
//     dMm = min(0.0, pDgnForceCtrl->m_Mxx_Rcpl[1]);
//     dVu = (fabs(pDgnForceCtrl->m_Vxx_Rcpl[0]) > fabs(pDgnForceCtrl->m_Vxx_Rcpl[1])) ? pDgnForceCtrl->m_Vxx_Rcpl[0] : pDgnForceCtrl->m_Vxx_Rcpl[1];
// 
//     dMp_DL = max(0.0, pDgnForceCtrl->m_MxxDL_Rcpl[0]);
//     dMm_DL = min(0.0, pDgnForceCtrl->m_MxxDL_Rcpl[1]);
//     dVu_DL = (fabs(pDgnForceCtrl->m_VxxDL_Rcpl[0]) > fabs(pDgnForceCtrl->m_VxxDL_Rcpl[1])) ? pDgnForceCtrl->m_VxxDL_Rcpl[0] : pDgnForceCtrl->m_VxxDL_Rcpl[1];
// 
//     dMp_ML = max(0.0, pDgnForceCtrl->m_MxxML_Rcpl[0]);
//     dMm_ML = min(0.0, pDgnForceCtrl->m_MxxML_Rcpl[1]);
//     dVu_ML = (fabs(pDgnForceCtrl->m_VxxML_Rcpl[0]) > fabs(pDgnForceCtrl->m_VxxML_Rcpl[1])) ? pDgnForceCtrl->m_VxxML_Rcpl[0] : pDgnForceCtrl->m_VxxML_Rcpl[1];

		dMp = max(0.0, pDgnForceCtrl->m_Mxx_Rcpl[0]);
		dMm = min(0.0, pDgnForceCtrl->m_Mxx_Rcpl[1]);
		dVu = (fabs(pDgnForceCtrl->m_Vxx_Rcpl[0]) > fabs(pDgnForceCtrl->m_Vxx_Rcpl[1])) ? pDgnForceCtrl->m_Vxx_Rcpl[0] : pDgnForceCtrl->m_Vxx_Rcpl[1];

		//dMp = dMp_DL + dMp_ML
		dMp_DL = pDgnForceCtrl->m_MxxDL_Rcpl[0]; 
		dMm_DL = pDgnForceCtrl->m_MxxDL_Rcpl[1]; 
		dVu_DL = (fabs(pDgnForceCtrl->m_Vxx_Rcpl[0]) > fabs(pDgnForceCtrl->m_Vxx_Rcpl[1])) ? pDgnForceCtrl->m_VxxDL_Rcpl[0] : pDgnForceCtrl->m_VxxDL_Rcpl[1];

		dMp_ML = pDgnForceCtrl->m_MxxML_Rcpl[0];
		dMm_ML = pDgnForceCtrl->m_MxxML_Rcpl[1];
		dVu_ML = (fabs(pDgnForceCtrl->m_Vxx_Rcpl[0]) > fabs(pDgnForceCtrl->m_Vxx_Rcpl[1])) ? pDgnForceCtrl->m_VxxML_Rcpl[0] : pDgnForceCtrl->m_VxxML_Rcpl[1];
 }
	else
	{
//     dMp = max(0.0, pDgnForceCtrl->m_Myy_Rcpl[0]);
//     dMm = min(0.0, pDgnForceCtrl->m_Myy_Rcpl[1]);
//     dVu = (fabs(pDgnForceCtrl->m_Vyy_Rcpl[0]) > fabs(pDgnForceCtrl->m_Vyy_Rcpl[1])) ? pDgnForceCtrl->m_Vyy_Rcpl[0] : pDgnForceCtrl->m_Vyy_Rcpl[1];
// 
//     dMp_DL = max(0.0, pDgnForceCtrl->m_MyyDL_Rcpl[0]);
//     dMm_DL = min(0.0, pDgnForceCtrl->m_MyyDL_Rcpl[1]);
//     dVu_DL = (fabs(pDgnForceCtrl->m_VyyDL_Rcpl[0]) > fabs(pDgnForceCtrl->m_VyyDL_Rcpl[1])) ? pDgnForceCtrl->m_VyyDL_Rcpl[0] : pDgnForceCtrl->m_VyyDL_Rcpl[1];
// 
//     dMp_ML = max(0.0, pDgnForceCtrl->m_MyyML_Rcpl[0]);
//     dMm_ML = min(0.0, pDgnForceCtrl->m_MyyML_Rcpl[1]);
//     dVu_ML = (fabs(pDgnForceCtrl->m_VyyML_Rcpl[0]) > fabs(pDgnForceCtrl->m_VyyML_Rcpl[1])) ? pDgnForceCtrl->m_VyyML_Rcpl[0] : pDgnForceCtrl->m_VyyML_Rcpl[1];

		dMp = max(0.0, pDgnForceCtrl->m_Myy_Rcpl[0]);
		dMm = min(0.0, pDgnForceCtrl->m_Myy_Rcpl[1]);
		dVu = (fabs(pDgnForceCtrl->m_Vyy_Rcpl[0]) > fabs(pDgnForceCtrl->m_Vyy_Rcpl[1])) ? pDgnForceCtrl->m_Vyy_Rcpl[0] : pDgnForceCtrl->m_Vyy_Rcpl[1];

		dMp_DL = pDgnForceCtrl->m_MyyDL_Rcpl[0];
		dMm_DL = pDgnForceCtrl->m_MyyDL_Rcpl[1];
		dVu_DL = (fabs(pDgnForceCtrl->m_Vyy_Rcpl[0]) > fabs(pDgnForceCtrl->m_Vyy_Rcpl[1])) ? pDgnForceCtrl->m_VyyDL_Rcpl[0] : pDgnForceCtrl->m_VyyDL_Rcpl[1];

		dMp_ML = pDgnForceCtrl->m_MyyML_Rcpl[0];
		dMm_ML = pDgnForceCtrl->m_MyyML_Rcpl[1];
		dVu_ML = (fabs(pDgnForceCtrl->m_Vyy_Rcpl[0]) > fabs(pDgnForceCtrl->m_Vyy_Rcpl[1])) ? pDgnForceCtrl->m_VyyML_Rcpl[0] : pDgnForceCtrl->m_VyyML_Rcpl[1];


	}

	dMp *= dUnitWidth;
	dMm *= dUnitWidth;
	dVu *= dUnitWidth;
	dMp_DL *= dUnitWidth;
	dMm_DL *= dUnitWidth;
	dVu_DL *= dUnitWidth;
	dMp_ML *= dUnitWidth;
	dMm_ML *= dUnitWidth;
	dVu_ML *= dUnitWidth;

	// 부재력 : 축방향력(Fx, Fy, Fz), 모멘트(Mx, My, Mz)
	int nPOSI_CRC = 0;
	ForceRcase.ForceDL.dFxx[nPOSI_CRC] = 0.0;
	ForceRcase.ForceDL.dFyy[nPOSI_CRC] = 0.0;
	ForceRcase.ForceDL.dFzz[nPOSI_CRC] = dVu_DL;
	ForceRcase.ForceDL.dMux[nPOSI_CRC] = 0.0;
	ForceRcase.ForceDL.dMuy[nPOSI_CRC] = (fabs(dMp_DL) >= fabs(dMm_DL))?  dMp_DL : dMm_DL;
	ForceRcase.ForceDL.dMuz[nPOSI_CRC] = 0.0;

	ForceRcase.ForceLL.dFxx[nPOSI_CRC] = 0.0;
	ForceRcase.ForceLL.dFyy[nPOSI_CRC] = 0.0;
	ForceRcase.ForceLL.dFzz[nPOSI_CRC] = dVu_ML;
	ForceRcase.ForceLL.dMux[nPOSI_CRC] = 0.0;
	ForceRcase.ForceLL.dMuy[nPOSI_CRC] = (fabs(dMp_ML) >= fabs(dMm_ML))?  dMp_ML : dMm_ML;
	ForceRcase.ForceLL.dMuz[nPOSI_CRC] = 0.0;
	
	ForceRcase.ForceG.dFxx[nPOSI_CRC] = 0.0;
	ForceRcase.ForceG.dFyy[nPOSI_CRC] = 0.0;
	ForceRcase.ForceG.dFzz[nPOSI_CRC] = dVu;
	ForceRcase.ForceG.dMux[nPOSI_CRC] = 0.0;
	ForceRcase.ForceG.dMuy[nPOSI_CRC] = (fabs(dMp) >= fabs(dMm))?  dMp : dMm;
	ForceRcase.ForceG.dMuz[nPOSI_CRC] = 0.0;

	return TRUE;
}



BOOL  CDgnRCRatingDataCtrl::Get_RCRatingBeamOptionD(_UMD_RC_KR_BEAM_OPTION &DgnOptD)
{
	CString strDgnCode = _T("");
	int iCode = 0;
 
	if(!ConvertCivilDgnCode2DgnCodeStr(m_nDgnCode, strDgnCode, iCode)) return FALSE;

	// Code Option
	CStringW stringw(strDgnCode);
	DgnOptD.strDgnCode = stringw;

	// Set default
	double pdPhi[6]={0.0};
	if(!m_pDoc->m_pDgnDataCtrl->Get_DgnConPhiDef(strDgnCode, pdPhi))  ASSERT(0);
	for(int i=0; i<6; i++)	DgnOptD.dPhi[i] = pdPhi[i];  // φ (0:Bending, 1:Tension, 2:Comp Spiral, 3:Comp Tie, 4:Shear)	
	DgnOptD.bLongTerm = TRUE;
	DgnOptD.dAlpha_cc = 0.85;

	if(strDgnCode == _T("KSCE-LSD15"))
	{
		T_RPAC_D RpacD;    
		if(!m_pDoc->m_pAttrCtrl2->GetRpac(RpacD)) RpacD.Initialize(m_nDgnCode);  

		if(RpacD.bUserInputData)
		{
			DgnOptD.bLongTerm = RpacD.bCheckCofLongTerm;
			DgnOptD.dAlpha_cc = RpacD.dAlphacc;  // 0.85;
			DgnOptD.dPhi[0]   = RpacD.dPhi_c[0]; // 0.65
			DgnOptD.dPhi[5]   = RpacD.dPhi_s[0]; // 0.90
		}
	}

	return TRUE;
}

BOOL CDgnRCRatingDataCtrl::Get_RCRatingBeamSectD(T_ELEM_K ElemK, _UMD_RC_KR_BEAM_SECT &DgnSectD)
{
	T_ELEM_D ElemData; ElemData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemData))	ASSERT(FALSE);

	// SectD
	T_SECT_D SectD; SectD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(ElemData.elpro, SectD)) return FALSE;
	if(!ConvertRCRatingBeamSectD(SectD, DgnSectD)) return FALSE;

	return TRUE;
}

BOOL CDgnRCRatingDataCtrl::Get_RCRatingPlateSectD(T_RRPL_D &RrplD, _UMD_RC_KR_BEAM_SECT &DgnSectD)
{
	DgnSectD.Initialize();
	int nElemSize = RrplD.arElemK.GetSize();
	// Get Thik.(설계영역의 Thikness Key가 동일한지 검토)
	int iThik = -1;
	T_ELEM_D eData;
	for(int i=0; i<nElemSize ; i++)
	{
		eData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetElem(RrplD.arElemK.GetAt(i), eData))	ASSERT(0);
		if(i==0)
		{ iThik = eData.elpro; }
		else
		{ if(iThik != eData.elpro) return FALSE; }// Thikness Key가 다르면 종료
	}			

	double dThik = 0.0;
	double dUnitWidth = 1000.0; // mm   Get_UnitWidthLength();
	T_THIK_D ThikD; ThikD.Initialize();
	m_pDoc->m_pAttrCtrl->GetThik(iThik, ThikD);
	m_pDoc->m_pAttrCtrl->GetThikForDesign(ThikD, dThik);

	// CUMDC_DataBase::Get_SectDataRC_Plate 참고
	double dD[8] ={0.0};
	dD[0] = dThik;
	dD[1] = dUnitWidth;
	dD[2] = 0.0;
	dD[3] = 0.0;
	dD[4] = 0.0;
	dD[5] = 0.0;
	dD[6] = 0.0;
	dD[7] = 0.0;
	double dArea = dThik*dUnitWidth;
	if(dArea<=0.0)  return FALSE;

	double dAsy	 = 5.0/6.0* dArea; 
	double dAsz	 = 5.0/6.0* dArea;
	double dIyy  = dUnitWidth*dThik*dThik*dThik/12.0;
	double dIzz  = dThik*dUnitWidth*dUnitWidth*dUnitWidth/12.0;
	double dYbar = dUnitWidth/2.0;
	double dZbar = dThik/2.0;
	double dRoy	 = sqrt(dIyy/dArea);
	double dRoz  = sqrt(dIzz/dArea);

	CStringW cstingw(DGN_SECT_SHAPE_REG_SB);
	DgnSectD.strShp = cstingw;
	for(int i=0 ; i<8 ; i++)
	{
		DgnSectD.dSize[i] = dD[i];
	}
	DgnSectD.dArea  = dArea;
	DgnSectD.dIyy   = dIyy;	//	yy축에 대한 단면2차모멘트  (default=0.0, ) (Check: Value>=0)
	DgnSectD.dIzz   = dIzz;	//	zz축에 대한 단면2차모멘트  (default=0.0, ) (Check: Value>=0)
	DgnSectD.dRoy   = dRoy;	//	yy축에 대한 단면회전반경  (default=0.0, ) (Check: Value>=0)
	DgnSectD.dRoz   = dRoz;	//	zz축에 대한 단면회전반경  (default=0.0, ) (Check: Value>=0)

	DgnSectD.dCenterY  = dYbar;
	DgnSectD.dCenterZ  = dZbar;

	return TRUE;
}


BOOL CDgnRCRatingDataCtrl::ConvertRCRatingBeamSectD(const T_SECT_D &SectD, _UMD_RC_KR_BEAM_SECT &DgnSectD)
{
	DgnSectD.Initialize();

	CStringW cstringw(SectD.SectBefore.Shape);
	DgnSectD.strShp = cstringw;

	if(DgnSectD.strShp == DGN_SECT_SHAPE_REG_SB)
	{
		DgnSectD.dSize[0] = SectD.SectBefore.SectI.Size[0];
		DgnSectD.dSize[1] =  SectD.SectBefore.SectI.Size[1];
		DgnSectD.dCenterZ = SectD.SectBefore.Stiffness.Czm;
		DgnSectD.dCenterY = SectD.SectBefore.Stiffness.Cym;
		DgnSectD.dArea    = SectD.SectBefore.Stiffness.Area;
		
		double dArea = SectD.SectBefore.SectI.Stiffness.Area;
		DgnSectD.dRoy = dArea  < DgnZero  ? 0.0 : sqrt(SectD.SectBefore.SectI.Stiffness.Ryy / dArea);
		DgnSectD.dRoz = dArea  < DgnZero  ? 0.0 : sqrt(SectD.SectBefore.SectI.Stiffness.Rzz/ dArea);                    
		DgnSectD.dIyy = SectD.SectBefore.SectI.Stiffness.Ryy;                                                                                          
		DgnSectD.dIzz = SectD.SectBefore.SectI.Stiffness.Rzz ; 
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnRCRatingDataCtrl::Get_RCRatingBeamRebarD(T_ELEM_K ElemK,  const _UMD_RC_KR_BEAM_SECT &DgnSectD, _UMD_RC_KR_BEAM_RBAR &DgnRebarD, int nPosIJ)
{
	// ElemD
	T_ELEM_D ElemD; ElemD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	{ASSERT(0); return FALSE; }	

	// RebarD
	T_RCHK_D RebarD; RebarD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetRchk(ElemD.elpro, RebarD)) return FALSE;
	if(!ConvertRCRatingBeamRebarD(RebarD, DgnSectD, DgnRebarD, nPosIJ))  return FALSE;

	return TRUE;
}


BOOL CDgnRCRatingDataCtrl::Get_RCRatingPlateRebarD(T_RRPL_D &RrplD, const _UMD_RC_KR_BEAM_SECT &DgnSectD, _UMD_RC_KR_BEAM_RBAR &DgnRebarD)
{
	DgnRebarD.Initialize();

	T_CPBE_D RebarD;  RebarD.Initialize(); 

	// Data Convert
	RebarD.bNum = RrplD.Rebar.bNum;

	for(int i=0; i<3; i++)
	{
		for(int j=0; j<10; j++)
		{
			RebarD.dDt[i][j] = RrplD.Rebar.dDt[i][j];
			RebarD.dDb[i][j] = RrplD.Rebar.dDb[i][j];
			RebarD.dTopBarNum[i][j] = RrplD.Rebar.dTopBarNum[i][j];
			RebarD.dBotBarNum[i][j] = RrplD.Rebar.dBotBarNum[i][j];
			RebarD.dTopBarCTC[i][j] = RrplD.Rebar.dTopBarCTC[i][j];
			RebarD.dBotBarCTC[i][j] = RrplD.Rebar.dBotBarCTC[i][j];
			RebarD.strTopBarNa1[i][j] = RrplD.Rebar.strTopBarNa1[i][j];
			RebarD.strTopBarNa2[i][j] = RrplD.Rebar.strTopBarNa2[i][j];
			RebarD.strBotBarNa1[i][j] = RrplD.Rebar.strBotBarNa1[i][j];
			RebarD.strBotBarNa2[i][j] = RrplD.Rebar.strBotBarNa2[i][j];
		}

		RebarD.dSubBarNum[i] = RrplD.Rebar.dSubBarNum[i];
		RebarD.dSubBarDist[i] = RrplD.Rebar.dSubBarDist[i];
		RebarD.strSubBarNa[i] = RrplD.Rebar.strSubBarNa[i];
		RebarD.bChkSubBar[i] = RrplD.Rebar.bChkSubBar[i];
		RebarD.dSubBarAngle[i] = RrplD.Rebar.dSubBarAngle[i];
	}

	int nDgnPos = 0;
	DgnRebarD.bNum = TRUE;

	// Sub Rebar
	CStringW strwSubBarNa(RebarD.strSubBarNa[nDgnPos]);
	DgnRebarD.strSubBarNa =  strwSubBarNa;
	DgnRebarD.dSubBarNum  = (RebarD.dSubBarDist[nDgnPos] < DgnZero) ? 0.0 : RebarD.dSubBarNum[nDgnPos];	
	DgnRebarD.dSubBarDist =  RebarD.dSubBarDist[nDgnPos];  // stirrup의 길이방향 간격	
	DgnRebarD.dSubBarArea = m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.strSubBarNa[nDgnPos]);	// stirrup철근 1개의 단면적
	DgnRebarD.dSubBarDia  = m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.strSubBarNa[nDgnPos]);   // stirrup철근 1개의 직경
	DgnRebarD.dtheta = RebarD.dSubBarAngle[nDgnPos];
	if(m_nDgnCode==KSCE_LSD15_RC_RATING)
	{
		if(DgnRebarD.dtheta < 22.0 || DgnRebarD.dtheta > 45.0) DgnRebarD.dtheta = 29.75; // 스트럿/주철근 사이각 default
	}

	// Top Main Rebar
	double dAst = 0.0, dAstDt = 0.0;;

	DgnRebarD.dDtDgn = RebarD.dDt[nDgnPos][0];

	int nTopLayerNum = 0;
	for(int i = 0; i <3/*10*/; ++i) // 우선 3단으로 제한
	{
		_UMD_RC_BEAM_REBAR_UNIT RbarUnitTop; RbarUnitTop.Initialize();
		//if(RebarD.dDt[nDgnPos][i] > DgnZero)
		{
			RbarUnitTop.dDt    = RebarD.dDt[nDgnPos][i];          
			RbarUnitTop.dNum   = RebarD.dTopBarNum[nDgnPos][i];
			RbarUnitTop.dArea1 = (RebarD.strTopBarNa1[nDgnPos][i] == _T("") ? 0.0                : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.strTopBarNa1[nDgnPos][i]));
			RbarUnitTop.dArea2 = (RebarD.strTopBarNa2[nDgnPos][i] == _T("") ? RbarUnitTop.dArea1 : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.strTopBarNa2[nDgnPos][i]));
			RbarUnitTop.dDia1  = (RebarD.strTopBarNa1[nDgnPos][i] == _T("") ? 0.0                : m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.strTopBarNa1[nDgnPos][i]));
			RbarUnitTop.dDia2  = (RebarD.strTopBarNa2[nDgnPos][i] == _T("") ? RbarUnitTop.dDia1  : m_pDoc->m_pMatlDB->Get_RebarDia( RebarD.strTopBarNa2[nDgnPos][i]));
			CStringW sBarNa1w(RebarD.strTopBarNa1[nDgnPos][i]); 
			CStringW sBarNa2w(RebarD.strTopBarNa2[nDgnPos][i]);
			RbarUnitTop.sBarNa1 = sBarNa1w;
			RbarUnitTop.sBarNa2 = sBarNa2w;
			RbarUnitTop.dCTC = 0.0; // ???
			RbarUnitTop.dAs_row = RbarUnitTop.dNum < DgnZero ? 0.0 : (RbarUnitTop.dArea1+RbarUnitTop.dArea2)/2.0 * RbarUnitTop.dNum;
			dAst   += min(RbarUnitTop.dArea1, RbarUnitTop.dArea2) *  RbarUnitTop.dNum;
			dAstDt += dAst * RbarUnitTop.dDt;

			if(nTopLayerNum == 0)
			{
				double dBc            = DgnSectD.dSize[1];
				double dNum           = RbarUnitTop.dNum;
				double dMainBarDia    = min(RbarUnitTop.dDia1, RbarUnitTop.dDia2);
				double dSubBarDia     = DgnRebarD.dSubBarDia;
				double dDst           = 50.0; // mm unit, ???
				if(dNum>1.0)                  DgnRebarD.dOutRbarSpace_Top = (dBc - 2.0*dDst - 2.0*dSubBarDia - dMainBarDia)/(dNum-1.0);
				else if(0.0<dNum && dNum<1.0) DgnRebarD.dOutRbarSpace_Top = (dBc - 2.0*dDst - 2.0*dSubBarDia)/dNum;
				else                          DgnRebarD.dOutRbarSpace_Top = 0.0;					
			}          

			DgnRebarD.arTopBar.Add(RbarUnitTop);
			nTopLayerNum++;
		}
	}
	DgnRebarD.dAst = dAst;
	DgnRebarD.ddt = dAst < DgnZero ? 0.0 : dAstDt / dAst;

	// Bottom Main Rebar
	double dAsb = 0.0, dAsbDb = 0.0;
	DgnRebarD.dDbDgn = RebarD.dDb[nDgnPos][0];

	int nBotLayerNum = 0;
	for(int i = 0; i <3/*10*/; ++i)
	{
		_UMD_RC_BEAM_REBAR_UNIT RbarUnitBot; RbarUnitBot.Initialize();
		//if(RebarD.dDb[nDgnPos][i] > DgnZero)
		{
			RbarUnitBot.dDt     = RebarD.dDb[nDgnPos][i];          
			RbarUnitBot.dNum    = RebarD.dBotBarNum[nDgnPos][i];
			RbarUnitBot.dArea1  = (RebarD.strBotBarNa1[nDgnPos][i] == _T("") ? 0.0                : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.strBotBarNa1[nDgnPos][i]));
			RbarUnitBot.dArea2  = (RebarD.strBotBarNa2[nDgnPos][i] == _T("") ? RbarUnitBot.dArea1 : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.strBotBarNa2[nDgnPos][i]));
			RbarUnitBot.dDia1   = (RebarD.strBotBarNa1[nDgnPos][i] == _T("") ? 0.0                : m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.strBotBarNa1[nDgnPos][i]));
			RbarUnitBot.dDia2   = (RebarD.strBotBarNa2[nDgnPos][i] == _T("") ? RbarUnitBot.dDia1  : m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.strBotBarNa2[nDgnPos][i]));
			CStringW sBarNa1w(RebarD.strBotBarNa1[nDgnPos][i]); 
			CStringW sBarNa2w(RebarD.strBotBarNa2[nDgnPos][i]);
			RbarUnitBot.sBarNa1 = sBarNa1w;
			RbarUnitBot.sBarNa2 = sBarNa2w;
			RbarUnitBot.dCTC = 0.0; // ???
			RbarUnitBot.dAs_row = RbarUnitBot.dNum < DgnZero ? 0.0 : (RbarUnitBot.dArea1+RbarUnitBot.dArea2)/2.0 * RbarUnitBot.dNum;
			dAsb   += min(RbarUnitBot.dArea1, RbarUnitBot.dArea2) *  RbarUnitBot.dNum;
			dAsbDb += dAsb * RbarUnitBot.dDt;

			if(nBotLayerNum == 0)
			{
				double dBc             = DgnSectD.dSize[1];
				double dNum            = RbarUnitBot.dNum;
				double dMainBarDia     = min(RbarUnitBot.dDia1, RbarUnitBot.dDia2);
				double dSubBarDia      = DgnRebarD.dSubBarDia;
				double dDst            = 50.0; // mm unit, ???
				if(dNum>1.0)                  DgnRebarD.dOutRbarSpace_Bot = (dBc - 2.0*dDst - 2.0*dSubBarDia - dMainBarDia)/(dNum-1.0);
				else if(0.0<dNum && dNum<1.0) DgnRebarD.dOutRbarSpace_Bot = (dBc - 2.0*dDst - 2.0*dSubBarDia)/dNum;
				else                          DgnRebarD.dOutRbarSpace_Bot = 0.0;					
			}          

			DgnRebarD.arBotBar.Add(RbarUnitBot);
			nBotLayerNum++;
		}
	}

	DgnRebarD.dAsb = dAsb;
	DgnRebarD.ddb = dAsb < DgnZero ? 0.0 : dAsbDb / dAsb;

	return TRUE;
}

BOOL CDgnRCRatingDataCtrl::ConvertRCRatingBeamRebarD(const T_RCHK_D &RebarD, const _UMD_RC_KR_BEAM_SECT &DgnSectD,  _UMD_RC_KR_BEAM_RBAR &DgnRebarD, int nPosIJ)
{
	int nDgnPos = nPosIJ == 0 ? 0 : 2; // 0 : I, 2 : J
	DgnRebarD.Initialize();

	DgnRebarD.bNum = TRUE;

	// Sub Rebar
	DgnRebarD.strSubBarNa = RebarD.BEAM.strSubBarNa[nDgnPos];
	DgnRebarD.dSubBarNum  = (RebarD.BEAM.dSubBarDist[nDgnPos] < DgnZero) ? 0.0 : RebarD.BEAM.dSubBarNum[nDgnPos];	
	DgnRebarD.dSubBarDist =  RebarD.BEAM.dSubBarDist[nDgnPos];  // stirrup의 길이방향 간격	
	DgnRebarD.dSubBarArea = m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.BEAM.strSubBarNa[nDgnPos]);	 // stirrup철근 1개의 단면적
	DgnRebarD.dSubBarDia  = m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.BEAM.strSubBarNa[nDgnPos]);   // stirrup철근 1개의 직경
	DgnRebarD.dtheta = RebarD.BEAM.dSubBarAngle[nDgnPos];
	if(m_nDgnCode==KSCE_LSD15_RC_RATING)
	{
		if(DgnRebarD.dtheta < 22.0 || DgnRebarD.dtheta > 45.0) DgnRebarD.dtheta = 29.75; // 스트럿/주철근 사이각 default
	}

	// Sub Rebar for Horizontal direction
	//     DgnRebarD.dSubBarArea_H = RebarD.BEAM;
	//     DgnRebarD.dSubBarDia_H = RebarD.BEAM;
	//     DgnRebarD.dSubBarDist_H=RebarD.BEAM;
	//     DgnRebarD.dSubBarNum_H = RebarD.BEAM;

	// Top Main Rebar
	double dAst = 0.0, dAstDt = 0.0;;

	DgnRebarD.dDtDgn = RebarD.BEAM.dDt[nDgnPos][0];

	int nTopLayerNum = 0;
	for(int i = 0; i <3/*10*/; ++i)
	{
		_UMD_RC_BEAM_REBAR_UNIT RbarUnitTop; RbarUnitTop.Initialize();
		//if(RebarD.BEAM.dDt[nDgnPos][i] > DgnZero)
		{
			RbarUnitTop.dDt     = RebarD.BEAM.dDt[nDgnPos][i];          
			RbarUnitTop.dNum    = RebarD.BEAM.iTopBarNum[nDgnPos][i];
			RbarUnitTop.dArea1  = (RebarD.BEAM.strTopBarNa1[nDgnPos][i] == _T("") ? 0.0                : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.BEAM.strTopBarNa1[nDgnPos][i]));
			RbarUnitTop.dArea2  = (RebarD.BEAM.strTopBarNa2[nDgnPos][i] == _T("") ? RbarUnitTop.dArea1 : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.BEAM.strTopBarNa2[nDgnPos][i]));
			RbarUnitTop.dDia1   = (RebarD.BEAM.strTopBarNa1[nDgnPos][i] == _T("") ? 0.0                : m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.BEAM.strTopBarNa1[nDgnPos][i]));
			RbarUnitTop.dDia2   = (RebarD.BEAM.strTopBarNa2[nDgnPos][i] == _T("") ? RbarUnitTop.dDia1  : m_pDoc->m_pMatlDB->Get_RebarDia( RebarD.BEAM.strTopBarNa2[nDgnPos][i]));
			RbarUnitTop.sBarNa1 = RebarD.BEAM.strTopBarNa1[nDgnPos][i];
			RbarUnitTop.sBarNa2 = RebarD.BEAM.strTopBarNa2[nDgnPos][i];
			RbarUnitTop.dCTC    = 0.0; // ???
			RbarUnitTop.dAs_row = RbarUnitTop.dNum < DgnZero ? 0.0 : (RbarUnitTop.dArea1+RbarUnitTop.dArea2)/2.0 * RbarUnitTop.dNum;
			dAst   += RbarUnitTop.dAs_row;
			dAstDt += RbarUnitTop.dAs_row * RbarUnitTop.dDt;

			if(nTopLayerNum == 0)
			{
				double dBc             = DgnSectD.dSize[1];
				double dNum            = RbarUnitTop.dNum;
				double dMainBarDia     = min(RbarUnitTop.dDia1, RbarUnitTop.dDia2);
				double dSubBarDia      = DgnRebarD.dSubBarDia;
				double dDst            = 50.0; // mm unit, ???
				if(dNum>1.0)                  DgnRebarD.dOutRbarSpace_Top = (dBc - 2.0*dDst - 2.0*dSubBarDia - dMainBarDia)/(dNum-1.0);
				else if(0.0<dNum && dNum<1.0) DgnRebarD.dOutRbarSpace_Top = (dBc - 2.0*dDst - 2.0*dSubBarDia)/dNum;
				else                          DgnRebarD.dOutRbarSpace_Top = 0.0;					
			}          

			DgnRebarD.arTopBar.Add(RbarUnitTop);
			nTopLayerNum++;
		}
	}
	DgnRebarD.dAst = dAst;
	DgnRebarD.ddt = dAst < DgnZero ? 0.0 : dAstDt / dAst;

	// Bottom Main Rebar
	double dAsb = 0.0, dAsbDb = 0.0;
	DgnRebarD.dDbDgn = RebarD.BEAM.dDb[nDgnPos][0];

	int nBotLayerNum = 0;
	for(int i = 0; i <3/*10*/; ++i)
	{
		_UMD_RC_BEAM_REBAR_UNIT RbarUnitBot; RbarUnitBot.Initialize();
	 // if(RebarD.BEAM.dDb[nDgnPos][i] > DgnZero)
		{
			RbarUnitBot.dDt = RebarD.BEAM.dDb[nDgnPos][i];          
			RbarUnitBot.dNum = RebarD.BEAM.iBotBarNum[nDgnPos][i];
			RbarUnitBot.dArea1 = (RebarD.BEAM.strBotBarNa1[nDgnPos][i] == _T("") ? 0.0             : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.BEAM.strBotBarNa1[nDgnPos][i]));
			RbarUnitBot.dArea2 = (RebarD.BEAM.strBotBarNa2[nDgnPos][i] == _T("") ? RbarUnitBot.dArea1 : m_pDoc->m_pMatlDB->Get_RebarArea(RebarD.BEAM.strBotBarNa2[nDgnPos][i]));
			RbarUnitBot.dDia1  = (RebarD.BEAM.strBotBarNa1[nDgnPos][i] == _T("") ? 0.0             : m_pDoc->m_pMatlDB->Get_RebarDia(RebarD.BEAM.strBotBarNa1[nDgnPos][i]));
			RbarUnitBot.dDia2  = (RebarD.BEAM.strBotBarNa2[nDgnPos][i] == _T("") ? RbarUnitBot.dDia1  : m_pDoc->m_pMatlDB->Get_RebarDia( RebarD.BEAM.strBotBarNa2[nDgnPos][i]));
			RbarUnitBot.sBarNa1 = RebarD.BEAM.strBotBarNa1[nDgnPos][i];
			RbarUnitBot.sBarNa2 = RebarD.BEAM.strBotBarNa2[nDgnPos][i];
			RbarUnitBot.dCTC = 0.0; // ???
			RbarUnitBot.dAs_row = RbarUnitBot.dNum < DgnZero ? 0.0 : (RbarUnitBot.dArea1+RbarUnitBot.dArea2)/2.0 * RbarUnitBot.dNum;
			dAsb   += RbarUnitBot.dAs_row;
			dAsbDb += RbarUnitBot.dAs_row * RbarUnitBot.dDt;

			if(nBotLayerNum == 0)
			{
				double dBc             = DgnSectD.dSize[1];
				double dNum            = RbarUnitBot.dNum;
				double dMainBarDia     = min(RbarUnitBot.dDia1, RbarUnitBot.dDia2);
				double dSubBarDia      = DgnRebarD.dSubBarDia;
				double dDst            = 50.0; // mm unit, ???
				if(dNum>1.0)                  DgnRebarD.dOutRbarSpace_Bot = (dBc - 2.0*dDst - 2.0*dSubBarDia - dMainBarDia)/(dNum-1.0);
				else if(0.0<dNum && dNum<1.0) DgnRebarD.dOutRbarSpace_Bot = (dBc - 2.0*dDst - 2.0*dSubBarDia)/dNum;
				else                          DgnRebarD.dOutRbarSpace_Bot = 0.0;					
			}          

			DgnRebarD.arBotBar.Add(RbarUnitBot);
			nBotLayerNum++;
		}
	}
	
	DgnRebarD.dAsb = dAsb;
	DgnRebarD.ddb = dAsb < DgnZero ? 0.0 : dAsbDb / dAsb;

	return TRUE;
}

BOOL CDgnRCRatingDataCtrl::Get_RCRatingBeamMatlD(T_ELEM_K ElemK,  _UMD_RC_KR_BEAM_MATL &DgnMatlD)
{
	T_ELEM_D ElemD; ElemD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	{ ASSERT(0); return FALSE; }	

	T_MATD_D MatlD; MatlD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatlD)) return FALSE;

	T_MATL_CONCRETE cData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetConcreteData(MatlD.Data1.CodeName, MatlD.Data1.CodeMatlName, cData);

	// Check Design Material Data
	if(MatlD.Data1.Design.C_fc < cDGN_Zero){return FALSE;}
	if(MatlD.MainRebarData.B_fy < cDGN_Zero){return FALSE;}

	DgnMatlD.dFc   = MatlD.Data1.Design.C_fc;
	DgnMatlD.dFyr  = MatlD.MainRebarData.B_fy;
	DgnMatlD.dFys  = MatlD.SubRebarData.B_fy;
	DgnMatlD.dEc   = cData.Elast;
	DgnMatlD.dEsr  = MatlD.MainRebarData.B_Elast;
	DgnMatlD.dEsv  = MatlD.SubRebarData.B_Elast;
	DgnMatlD.decu  = 0.003; // USD-10, Rail-11

	return TRUE;
}


BOOL CDgnRCRatingDataCtrl::Get_RCRatingPlateMatlD(T_RRPL_D &RrplD, _UMD_RC_KR_BEAM_MATL &DgnMatlD)
{

	if(RrplD.arElemK.GetSize()==0) return FALSE;
	T_ELEM_K ElemK = RrplD.arElemK.GetAt(0);

	T_ELEM_D ElemD; ElemD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	{ ASSERT(0); return FALSE; }	

	T_MATD_D MatlD; MatlD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatlD)) return FALSE;

	T_MATL_CONCRETE cData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetConcreteData(MatlD.Data1.CodeName, MatlD.Data1.CodeMatlName, cData);

	// Check Design Material Data
	if(MatlD.Data1.Design.C_fc < cDGN_Zero){return FALSE;}
	if(MatlD.MainRebarData.B_fy < cDGN_Zero){return FALSE;}

	DgnMatlD.dFc   = MatlD.Data1.Design.C_fc;
	DgnMatlD.dFyr  = MatlD.MainRebarData.B_fy;
	DgnMatlD.dFys  = MatlD.SubRebarData.B_fy;
	DgnMatlD.dEc   = cData.Elast;
	DgnMatlD.dEsr  = MatlD.MainRebarData.B_Elast;
	DgnMatlD.dEsv  = MatlD.SubRebarData.B_Elast;
	DgnMatlD.decu  = 0.003;

	return TRUE;
}


BOOL CDgnRCRatingDataCtrl::Get_RCRatingBeamLcomD(const FORCE_RCASE_KR &DBForceD, _UMD_RC_KR_BEAM_LOAD &Force4DgnEngine, int nPosIJ/*=0*/, BOOL bUseOnlyML/*=FALSE*/)
{
	Force4DgnEngine.Initialize();
	Force4DgnEngine.LcomList_Str.SetSize(1);
	Force4DgnEngine.LcomList_Ser.SetSize(1);

	_UMD_RC_KR_LCOM &DgnForce_Str = Force4DgnEngine.LcomList_Str[0];
	_UMD_RC_KR_LCOM &DgnForce_Ser= Force4DgnEngine.LcomList_Ser[0];

	//CStringW strWLcomName(strLcomName);  

	DgnForce_Str.bUse    = TRUE;	  // 설계시 검토여부
	DgnForce_Str.nType   = 1;	  // 하중종류 1:Strn  2:Serv
	DgnForce_Str.strName = _T(""); //strWLcomName;  //하중조합이름 ???

	DgnForce_Ser.bUse    = TRUE;	  // 설계시 검토여부
	DgnForce_Ser.nType   = 2;	  // 하중종류 1:Strn  2:Serv 
	DgnForce_Ser.strName = _T(""); //strWLcomName; // 하중조합이름 ???

	// 부재력 : 축방향력(Fx, Fy, Fz), 모멘트(Mx, My, Mz)
	const  _DGN_FORC_CRC &ForceValDL = DBForceD.ForceDL;
	const  _DGN_FORC_CRC &ForceValLL = DBForceD.ForceLL;
	const  _DGN_FORC_CRC &ForceValG  = DBForceD.ForceG;
	
	int nPOSI_CRC = (nPosIJ == 1);
	if(bUseOnlyML)
	{
		DgnForce_Str.dForce[0] =  ForceValLL.dFxx[nPOSI_CRC];
		DgnForce_Str.dForce[1] =  ForceValLL.dFyy[nPOSI_CRC];
		DgnForce_Str.dForce[2] =  ForceValLL.dFzz[nPOSI_CRC];
		DgnForce_Str.dForce[3] =  ForceValLL.dMux[nPOSI_CRC];
		DgnForce_Str.dForce[4] =  ForceValLL.dMuy[nPOSI_CRC];
		DgnForce_Str.dForce[5] =  ForceValLL.dMuz[nPOSI_CRC];
	}
	else
	{
		DgnForce_Str.dForce[0] = ForceValG.dFxx[nPOSI_CRC];
		DgnForce_Str.dForce[1] = ForceValG.dFyy[nPOSI_CRC];
		DgnForce_Str.dForce[2] = ForceValG.dFzz[nPOSI_CRC];
		DgnForce_Str.dForce[3] = ForceValG.dMux[nPOSI_CRC];
		DgnForce_Str.dForce[4] = ForceValG.dMuy[nPOSI_CRC];
		DgnForce_Str.dForce[5] = ForceValG.dMuz[nPOSI_CRC];
//     DgnForce_Str.dForce[0] = ForceValDL.dFxx[nPOSI_CRC] + ForceValLL.dFxx[nPOSI_CRC];
//     DgnForce_Str.dForce[1] = ForceValDL.dFyy[nPOSI_CRC] + ForceValLL.dFyy[nPOSI_CRC];
//     DgnForce_Str.dForce[2] = ForceValDL.dFzz[nPOSI_CRC] + ForceValLL.dFzz[nPOSI_CRC];
//     DgnForce_Str.dForce[3] = ForceValDL.dMux[nPOSI_CRC] + ForceValLL.dMux[nPOSI_CRC];
//     DgnForce_Str.dForce[4] = ForceValDL.dMuy[nPOSI_CRC] + ForceValLL.dMuy[nPOSI_CRC];
//     DgnForce_Str.dForce[5] = ForceValDL.dMuz[nPOSI_CRC] + ForceValLL.dMuz[nPOSI_CRC];
	}


//   DgnForce_Ser.dForce[0] = ForceValDL.dFxx[nPOSI_CRC] + ForceValLL.dFxx[nPOSI_CRC];
//   DgnForce_Ser.dForce[1] = ForceValDL.dFyy[nPOSI_CRC] + ForceValLL.dFyy[nPOSI_CRC];
//   DgnForce_Ser.dForce[2] = ForceValDL.dFzz[nPOSI_CRC] + ForceValLL.dFzz[nPOSI_CRC];
//   DgnForce_Ser.dForce[3] = ForceValDL.dMux[nPOSI_CRC] + ForceValLL.dMux[nPOSI_CRC];
//   DgnForce_Ser.dForce[4] = ForceValDL.dMuy[nPOSI_CRC] + ForceValLL.dMuy[nPOSI_CRC];
//   DgnForce_Ser.dForce[5] = ForceValDL.dMuz[nPOSI_CRC] + ForceValLL.dMuz[nPOSI_CRC];

	return TRUE;
}

BOOL CDgnRCRatingDataCtrl::Get_RCRatingBeamLcomD(const RATING_FORCE &DBForceD, _UMD_RC_KR_BEAM_LOAD &Force4DgnEngine)
{
	Force4DgnEngine.Initialize();
	Force4DgnEngine.LcomList_Str.SetSize(1);
	Force4DgnEngine.LcomList_Ser.SetSize(1);

	_UMD_RC_KR_LCOM &DgnForce_Str = Force4DgnEngine.LcomList_Str[0];
	_UMD_RC_KR_LCOM &DgnForce_Ser = Force4DgnEngine.LcomList_Ser[0]; // 지금은 여기에 데이터가 없어야 한다. Rating은 강도만 검토.

//  CStringW strWLcomName(strLcomName);  

	DgnForce_Str.bUse    = TRUE;	  // 설계시 검토여부
	DgnForce_Str.nType   = 1;	  // 하중종류 1:Strn  2:Serv
//   CString strNameStr   = _T(""); // 하중조합이름 ???
//   CStringW cstringwStr(strNameStr);
	DgnForce_Str.strName = _T("");

	DgnForce_Ser.bUse    = TRUE;	  // 설계시 검토여부
	DgnForce_Ser.nType   = 2;	  // 하중종류 1:Strn  2:Serv 
//   CString strNameSer   = _T(""); // 하중조합이름 ???
//   CStringW cstringwSer(strNameSer);
	DgnForce_Ser.strName = _T(""); 

	// 부재력 : 축방향력(Fx, Fy, Fz), 모멘트(Mx, My, Mz)
	const  T_RATI_UNITLCOM &ForceValDL = DBForceD.ForceDL;
	const  T_RATI_UNITLCOM &ForceValLL = DBForceD.ForceLL;
	const  T_RATI_UNITLCOM &ForceValG  = DBForceD.ForceG;

	DgnForce_Str.dForce[0] = ForceValG.dForce[0];
	DgnForce_Str.dForce[1] = ForceValG.dForce[1];
	DgnForce_Str.dForce[2] = ForceValG.dForce[2]; // Vz
	DgnForce_Str.dForce[3] = ForceValG.dForce[3];
	DgnForce_Str.dForce[4] = ForceValG.dForce[4]; // My
	DgnForce_Str.dForce[5] = ForceValG.dForce[5];

//   DgnForce_Str.dForce[0] = ForceValDL.dForce[0] + ForceValLL.dForce[0];
//   DgnForce_Str.dForce[1] = ForceValDL.dForce[1] + ForceValLL.dForce[1];
//   DgnForce_Str.dForce[2] = ForceValDL.dForce[2] + ForceValLL.dForce[2]; // Vz
//   DgnForce_Str.dForce[3] = ForceValDL.dForce[3] + ForceValLL.dForce[3];
//   DgnForce_Str.dForce[4] = ForceValDL.dForce[4] + ForceValLL.dForce[4]; // My
//   DgnForce_Str.dForce[5] = ForceValDL.dForce[5] + ForceValLL.dForce[5];

//   DgnForce_Ser.dForce[0] = ForceValDL.dForce[0] + ForceValLL.dForce[0];
//   DgnForce_Ser.dForce[1] = ForceValDL.dForce[1] + ForceValLL.dForce[1];
//   DgnForce_Ser.dForce[2] = ForceValDL.dForce[2] + ForceValLL.dForce[2];
//   DgnForce_Ser.dForce[3] = ForceValDL.dForce[3] + ForceValLL.dForce[3];
//   DgnForce_Ser.dForce[4] = ForceValDL.dForce[4] + ForceValLL.dForce[4];
//   DgnForce_Ser.dForce[5] = ForceValDL.dForce[5] + ForceValLL.dForce[5];

	return TRUE;
}


BOOL CDgnRCRatingDataCtrl::ConvertForce2Engine(FORCE_RCASE_KR &Force, RATING_FORCE &rData, const int &nIJ)
{
	// Convert Dead Load
	rData.ForceDL.dForce[0] = Force.ForceDL.dFxx[nIJ];
	rData.ForceDL.dForce[1] = Force.ForceDL.dFyy[nIJ];
	rData.ForceDL.dForce[2] = Force.ForceDL.dFzz[nIJ];
	rData.ForceDL.dForce[3] = Force.ForceDL.dMux[nIJ];
	rData.ForceDL.dForce[4] = Force.ForceDL.dMuy[nIJ];
	rData.ForceDL.dForce[5] = Force.ForceDL.dMuz[nIJ];

	// Convert Live load
	rData.ForceLL.dForce[0] = Force.ForceLL.dFxx[nIJ];
	rData.ForceLL.dForce[1] = Force.ForceLL.dFyy[nIJ];
	rData.ForceLL.dForce[2] = Force.ForceLL.dFzz[nIJ];
	rData.ForceLL.dForce[3] = Force.ForceLL.dMux[nIJ];
	rData.ForceLL.dForce[4] = Force.ForceLL.dMuy[nIJ];
	rData.ForceLL.dForce[5] = Force.ForceLL.dMuz[nIJ];

	// Convert Gravity load
	rData.ForceG.dForce[0] = Force.ForceG.dFxx[nIJ];
	rData.ForceG.dForce[1] = Force.ForceG.dFyy[nIJ];
	rData.ForceG.dForce[2] = Force.ForceG.dFzz[nIJ];
	rData.ForceG.dForce[3] = Force.ForceG.dMux[nIJ];
	rData.ForceG.dForce[4] = Force.ForceG.dMuy[nIJ];
	rData.ForceG.dForce[5] = Force.ForceG.dMuz[nIJ];

	return TRUE;
}

BOOL CDgnRCRatingDataCtrl::GetRatingRank(BOOL bPositiveM, const double &dMu, const double &dMn, const double &dP0a, double &dRatMuMn, int &nSafetyRank)
{
// initialize
	nSafetyRank = 0;
	dRatMuMn = 0.0;
	double dMc = 0.0;
	if (bPositiveM)
	{
		dMc = dMu<0.0 ? 0.0 : dMu;
	}
	else
	{
		dMc = dMu>0.0 ? 0.0 : dMu;
	}

// Get Rank
	dRatMuMn = fabs(dMc) < DgnZero ? fabs(dMn/DgnZero) : fabs(dMn/dMc);

	if(dRatMuMn > 1.0) {nSafetyRank = 0;} // 'A' Rank
	else if(dRatMuMn > 0.9)
	{
		if(fabs(dP0a) > fabs(dMc))
		{
			nSafetyRank = 1; // 'B' Rank
		}
		else
		{
			nSafetyRank = 2; // 'C' Rank
		}
	}
	else if(dRatMuMn > 0.75)
	{
			nSafetyRank = 3; // 'D' Rank
	}
	else
	{
			nSafetyRank = 4; // 'E' Rank
	}


	return TRUE;
}


BOOL CDgnRCRatingDataCtrl::Calc_RatingResult(int nSurveyMeth, double dpCapacity, T_RFRC_BASE& RfrcBaseD)
{
	double dRF = 0.0; // 내하율
	double dP0 = 0.0; // 기본내하력
	double dP0a = 0.0; // 공용내하력

	//  Set Data
	double dM_L= RfrcBaseD.dM_L;
	double dMd = RfrcBaseD.dMd;
	double dMd_m = dMd;
	double di_calc=RfrcBaseD.dImpFactor;
	double di_real=RfrcBaseD.dRealImpFactor;
	double dAddResponceF = RfrcBaseD.dAddResponceFactor;
	double dP_L = RfrcBaseD.dDgnLoad; // 설계활하중(예,DB-18)
	
	// Disp.
	double dDispZ_real = RfrcBaseD.dRealDispZ;
	double dDispZ_calc = RfrcBaseD.dCalc_defl;  
	double dDisp_rat = 0.0;
	dDisp_rat = fabs(dDispZ_real) < DgnZero ? 0.0 : fabs(dDispZ_calc/dDispZ_real);
	RfrcBaseD.dDisp_rat = dDisp_rat;

	// Strain.
	double dCalcEwip = RfrcBaseD.dCalcEwip;
	double dEstiEwip = RfrcBaseD.dEstiEwip;
	double dEwip_rat = fabs(dEstiEwip) < m_dZero ? 0.0 : fabs(dCalcEwip/dEstiEwip);
	RfrcBaseD.dEwip_rat = dEwip_rat;

	// Impact Factor
	double di_rat = 0.0;
	double d1_plus_i_calc = 1.0+di_calc;
	di_rat = d1_plus_i_calc/(1.0+di_real);

	// 실측 보정계수 nSurveyMeth = 0:처짐 1:변형률
	double dKs = 0.0;
	if(nSurveyMeth == 0) dKs = dDisp_rat*di_rat *dAddResponceF;
	else                 dKs = dEwip_rat*di_rat *dAddResponceF;

	// Consider Additional Response Factor
	double dKs_Add = 0.0;
	dKs_Add = dKs;

	// Converting Capacity
	RfrcBaseD.dPhiMn = dpCapacity;
	if(dM_L>=0.0) // Positive moment
	{
		// M_L이 Positive 일 때 Md가 Negative면 0
		if(dMd<0.0) dMd_m = 0.0; 
	}
	else
	{
		if(dMd>0.0) dMd_m = 0.0;
		dpCapacity = -1.0*dpCapacity;
	}
	
	// 내하율(R.F.)의 계산.
	dRF = fabs(dM_L*d1_plus_i_calc) < DgnZero ? 0.0 : (fabs(dpCapacity) - fabs(dMd_m))/(fabs(dM_L)*d1_plus_i_calc);

	// 기본내하력 
	dP0 = dRF*dP_L;

	// 공용내하력
	dP0a = dP0*dKs_Add;

	// Save Result
	RfrcBaseD.dMd_m = dMd_m;
	RfrcBaseD.d1_plus_i_calc = d1_plus_i_calc;
	RfrcBaseD.di_rat = di_rat;
	RfrcBaseD.dRF = dRF;
	RfrcBaseD.dP0 = dP0;
	RfrcBaseD.dKs = dKs_Add;
	RfrcBaseD.dP0a = dP0a;
	RfrcBaseD.bOK = dP0a<dP_L ? FALSE : TRUE;

	return TRUE;
}