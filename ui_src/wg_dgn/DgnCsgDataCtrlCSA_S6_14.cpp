
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "DgnCsgDataCtrlCSA_S6_14.h"
#include "DgnCsgDataCtrl.h"
#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "Dgn_CSGManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnCsgDataCtrlCSA_S6_14::CDgnCsgDataCtrlCSA_S6_14() : CDgnCsgDataCtrl()
{ 
}

CDgnCsgDataCtrlCSA_S6_14::~CDgnCsgDataCtrlCSA_S6_14()
{
}

void CDgnCsgDataCtrlCSA_S6_14::SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl)
{
	CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	T_UNIT_INDEX CodeUnit = CurrUnit;

	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
	CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;

	m_pDataCtrl->Set_UnitIndex(CodeUnit);

	InitialData(m_nDgnCode);
}

BOOL CDgnCsgDataCtrlCSA_S6_14::MakeMemberResultData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RESULT_D &rData)
{
	if (rData.pCSA==NULL) return FALSE;

	m_pCsgManager->Check_MemberResult(CsgMembD, rData);

	CsgMembD.Memb.ChIf.nChkPos = 1;
	m_pCsgManager->Check_MemberResult(CsgMembD, rData);

	return TRUE;
}

BOOL CDgnCsgDataCtrlCSA_S6_14::Check_Constructibility(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pCSA==NULL) return FALSE;
	if (ChkRes.pCSA==NULL) return FALSE;
	if (InD.aChkFor.GetSize()>0) InD.aChkFor[0].nLcomType = 3;
	InD.Memb.ChIf.nChkPos = nPos;
	BOOL bCheck = m_pCsgManager->Check_ConstructibilityRes(InD, MembRes, ChkRes);
	DtrR.dDTR_F = ChkRes.pCSA->ConsD.Comb.dRatio;
	DtrR.dDTR_S = ChkRes.pCSA->ConsD.Shear.dCombRat;

	return bCheck;
}


BOOL CDgnCsgDataCtrlCSA_S6_14::Check_Strength(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pCSA==NULL) return FALSE;
	if (ChkRes.pCSA==NULL) return FALSE;
	InD.Memb.ChIf.nChkPos = nPos;
	BOOL bCheck = m_pCsgManager->Check_StrengthRes(InD, MembRes, ChkRes);
	DtrR.dDTR_F = ChkRes.pCSA->StrnD.FlexD.dRatio_F;
	DtrR.dDTR_S = ChkRes.pCSA->StrnD.ShearD.dRat_z;

	return bCheck;
}

BOOL CDgnCsgDataCtrlCSA_S6_14::Check_Service(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pCSA==NULL) return FALSE;
	if (ChkRes.pCSA==NULL) return FALSE;
	InD.Memb.ChIf.nChkPos = nPos;
	BOOL bCheck = m_pCsgManager->Check_ServiceRes(InD, MembRes, ChkRes);
	DtrR.dDTR_Ser = ChkRes.pCSA->ServD.dRatio;

	return bCheck;
}

BOOL CDgnCsgDataCtrlCSA_S6_14::Check_Fatigue(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR, CSG_FATI_CR_RSLT &FatiR)
{
	if (MembRes.pCSA==NULL) return FALSE;
	if (ChkRes.pCSA==NULL) return FALSE;
	InD.Memb.ChIf.nChkPos = nPos;
	BOOL bCheck = m_pCsgManager->Check_FatigueRes(InD, MembRes, ChkRes);
	DtrR.dDTR_FL[0] = !ChkRes.pCSA->FatiD.FlgR[0].bFatiDisregard ? ChkRes.pCSA->FatiD.FlgR[0].dRatio : 0.0;
	DtrR.dDTR_FL[1] = !ChkRes.pCSA->FatiD.FlgR[1].bFatiDisregard ? ChkRes.pCSA->FatiD.FlgR[1].dRatio : 0.0;
	//
	DtrR.dDTR_F_LS = ChkRes.pCSA->FatiD.StudWeld.dRatio;
	//
	DtrR.dDTR_F_SC_p = ChkRes.pCSA->FatiD.SConn.dRatio;

	return bCheck;
}

BOOL CDgnCsgDataCtrlCSA_S6_14::Check_ShearConnector(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	// 호출을 막고 있었음, 필요하면 연결~
	//if (!m_pCsgManager->Check_ShearConnRes(InD, MembRes, ChkRes)) return FALSE;
	return TRUE;
}

BOOL CDgnCsgDataCtrlCSA_S6_14::Check_BearingStiffener(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD& InD, CSG_MEMB_RESULT_D& MembRes, CSG_CHECK_RESULT_CODES& ChkRes, CSG_CHK_RES_DTR_PARAM& DtrR)
{
	if (MembRes.pCSA == NULL) return FALSE;
	if (ChkRes.pCSA == NULL) return FALSE;
	if (InD.aReac.GetSize() == 0) return FALSE;
	
	auto L_GetRatio = [this](double dReq, double dOwn)
	{
		return fabs(dOwn) < m_dZero ? dReq / m_dZero : dReq / dOwn;
	};

	ChkRes.pCSA->StrnD.BStfnD.dReact = InD.aReac[0].dFz;
	BOOL bCheck = m_pCsgManager->Check_BearingStiffenerRes(InD, MembRes, ChkRes);
	const CSG_STRN_RES_CSA_S6_BEAR_STFN& BStfn = ChkRes.pCSA->StrnD.BStfnD;
	const double dRatBr = L_GetRatio(BStfn.dReact, BStfn.dBr);
	const double dRatCr = L_GetRatio(BStfn.dReact, BStfn.dCr);
	DtrR.dDTR_Bs = max(dRatBr, dRatCr);

	return TRUE;
}

void CDgnCsgDataCtrlCSA_S6_14::GetMy4Strength(CSG_CHECK_RESULT_CODES &ChkRes, double &dMuy)
{
}


void CDgnCsgDataCtrlCSA_S6_14::ClassNewMembResult(CSG_MEMB_RESULT_D &MembR)
{
	MembR.pCSA = new CSG_MEMBPOS_RES_CSA_S6_D;
}

void CDgnCsgDataCtrlCSA_S6_14::ClassNewDgnResult(CSG_CHECK_RESULT_CODES &ChkResD)
{
	ChkResD.pCSA = new CSG_CHECK_RES_CSA_S6;
}

void CDgnCsgDataCtrlCSA_S6_14::InitMembResult(CSG_MEMB_RESULT_D &MembR)
{
	MembR.pCSA->Initialize();
}

void CDgnCsgDataCtrlCSA_S6_14::InitDgnResult(CSG_CHECK_RESULT_CODES &ChkResD)
{
	ChkResD.pCSA->Initialize();
}



void CDgnCsgDataCtrlCSA_S6_14::SetScbrBase(BOOL bChk, int nLcomK4CS,  int nStageK,    int nStepK,      T_CSCH_REF &rData)
{
	if (rData.pCcbrB == NULL) return;
	// T_CCBR_BASE
	rData.pCcbrB->bChk      = bChk;
	rData.pCcbrB->nOrgLcomK = nLcomK4CS;
	rData.pCcbrB->nStageK   = nStageK;
	rData.pCcbrB->nStepK    = nStepK;
}

void CDgnCsgDataCtrlCSA_S6_14::SetScvrBase(BOOL bChk, int nLcomK4CS,  int nStageK,    int nStepK,      T_CSCH_REF &rData)
{
	if (rData.pCcvrB == NULL) return;
	// T_CCVR_BASE
	rData.pCcvrB->bChk      = bChk;
	rData.pCcvrB->nOrgLcomK = nLcomK4CS;
	rData.pCcvrB->nStageK   = nStageK;
	rData.pCcvrB->nStepK    = nStepK;
}

void CDgnCsgDataCtrlCSA_S6_14::SetCsbrBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
	if (rData.pCpbrB == NULL) return;
	// T_CPBR_BASE
	rData.pCpbrB->bChk      = bChk;
	rData.pCpbrB->nLcomK	  = nDgnLcomNo;
	rData.pCpbrB->nOrgLcomK = nOrgLcomNo;		    
	rData.pCpbrB->nMax	    = nMaxMinType;
}

void CDgnCsgDataCtrlCSA_S6_14::SetCsvrBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
	// T_CPVR_BASE
	rData.pCpvrB->bChk      = bChk;
	rData.pCpvrB->nLcomK	  = nDgnLcomNo;
	rData.pCpvrB->nOrgLcomK = nOrgLcomNo;		    
	rData.pCpvrB->nMax	    = nMaxMinType;
}

void CDgnCsgDataCtrlCSA_S6_14::SetCsslBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{

}

void CDgnCsgDataCtrlCSA_S6_14::SetCsacBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
	if (rData.pCpacB == NULL) return;
	// T_CPAC_BASE
	rData.pCpacB->bChk      = bChk;
	rData.pCpacB->nLcomK	  = nDgnLcomNo;
	rData.pCpacB->nOrgLcomK = nOrgLcomNo;		    
	rData.pCpacB->nMax	    = nMaxMinType;
}

void CDgnCsgDataCtrlCSA_S6_14::SetCsfcBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{

}

void CDgnCsgDataCtrlCSA_S6_14::SetCscrBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
	if (rData.pCpfcB == NULL) return;
	rData.pCpfcB->SConnR.bChk      = bChk;
	rData.pCpfcB->SConnR.nLcomK	   = nDgnLcomNo;
	rData.pCpfcB->SConnR.nOrgLcomK = nOrgLcomNo;		    
	rData.pCpfcB->SConnR.nMax	     = nMaxMinType;
}

void CDgnCsgDataCtrlCSA_S6_14::SetCsfcLoadBase(int nType, BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nTopBot, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData)
{
	if (rData.pCpfcB == NULL) return;
	if (nType==1) return;
	// T_CPFC_LOAD
	T_CPFC_LOAD &CpfcLoad = nTopBot==0 ? rData.pCpfcB->LoadR[0] : rData.pCpfcB->LoadR[1];

	CpfcLoad.bChk      = bChk;
	CpfcLoad.nLcomK    = nDgnLcomNo;
	CpfcLoad.nOrgLcomK = nOrgLcomNo;

}

void CDgnCsgDataCtrlCSA_S6_14::SetCpfcStudBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
	if (rData.pCpfcB == NULL) return;
	// T_CPFC_STUD
	rData.pCpfcB->StudR.bChk      = bChk;
	rData.pCpfcB->StudR.nLcomK	  = nDgnLcomNo;
	rData.pCpfcB->StudR.nOrgLcomK = nOrgLcomNo;		    
	rData.pCpfcB->StudR.nMax	    = nMaxMinType;
}

void CDgnCsgDataCtrlCSA_S6_14::SetCpfcSconBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData)
{
	if (rData.pCpfcB == NULL) return;
	// T_CPFC_SCONN
	rData.pCpfcB->SConnR.bChk      = bChk;
	rData.pCpfcB->SConnR.nLcomK	   = nDgnLcomNo;
	rData.pCpfcB->SConnR.nOrgLcomK = nOrgLcomNo;		    
	rData.pCpfcB->SConnR.nMax	     = nMaxMinType;
}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgScbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData)
{
	if (MembR.pCSA == NULL) return;
	if (ChkRes.pCSA == NULL) return;
	if (rData.pCcbrB == NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;
	if (LcomResD.aUlmFor.GetSize() == 0) return;

	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CCBR_BASE *pCcbr = rData.pCcbrB;

	CSG_CONS_RES_CSA_S6 &ConsD = ChkRes.pCSA->ConsD;

	ConvertCSGDForce(LcomResD.aChkFor[0].ForNC, pCcbr->CsFor);
	ConvertCSGEngToUblmForce(1, LcomResD.aUlmFor[0], pCcbr->UlFor);
	pCcbr->dMax = LcomResD.aUlmFor[0].dMmax;
	int nDir = (bPositive==TRUE) ? 0 : 1;

	pCcbr->dCr     = ConsD.AxisR.dCr1;
	pCcbr->dMry    = ConsD.Flex.dMry;
	pCcbr->dMrz    = ConsD.Flex.dMrz;
	pCcbr->dRatio  = ConsD.Comb.dRat1;
    if ( MembR.pCSA->MembR[nDir].bChkCurved )
    {
        pCcbr->dComb1  = ConsD.Flex.CurvedFlex.dMry_rat;
        pCcbr->dComb2  = ConsD.Flex.CurvedFlex.dMfw_rat;
    }
    else
    {
        pCcbr->dComb1  = ConsD.Flex.dRatioY;    //  Mfy/Mry
        pCcbr->dComb2  = ConsD.Flex.dRatioZ;    //  Mfy/Mry
    }
	pCcbr->nOK  = (pCcbr->dComb1 + pCcbr->dComb2 <1.0+cDGN_Zero) ? 1 : 0;
}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgScvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData)
{
	if (MembR.pCSA == NULL) return;
	if (ChkRes.pCSA == NULL) return;
	if (rData.pCcvrB == NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;
	if (LcomResD.aUlmFor.GetSize() == 0) return;

	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CCVR_BASE *pCcvr = rData.pCcvrB;

	CSG_CONS_RES_CSA_S6 &ConsD = ChkRes.pCSA->ConsD;

	CSG_MEMB_RES_CSA_S6_D &MembRes = MembR.pCSA->MembR[nChkPos];

	ConvertCSGDForce(LcomResD.aChkFor[0].ForNC, pCcvr->CsFor);
	ConvertCSGEngToUblmForce(1, LcomResD.aUlmFor[0], pCcvr->UlFor);
	pCcvr->dMax = LcomResD.aUlmFor[0].dMmax;

	pCcvr->dVu      = pCcvr->CsFor.dForce[2];
	pCcvr->dVr      = MembRes.Detail_S[0].dVr;
	pCcvr->dMr      = ConsD.Flex.dMry;
	pCcvr->dRatio_v = ConsD.Shear.dRat_z;
	pCcvr->dRatio_m = ConsD.Flex.dRatioY;
	pCcvr->dRatio_c = ConsD.Shear.dCombRat;
	pCcvr->dRatio   = ConsD.Shear.dRatio;

	pCcvr->nOK  = (pCcvr->dRatio<1.0+cDGN_Zero) ? 1 : 0;
}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgCsbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData)
{
	if (MembR.pCSA == NULL) return;
	if (ChkRes.pCSA == NULL) return;
	if (rData.pCpbrB == NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;
	if (LcomResD.aUlmFor.GetSize() == 0) return;

	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CPBR_BASE *pCpbr = rData.pCpbrB;

	CSG_MEMB_RES_CSA_S6_D &MembRes = MembR.pCSA->MembR[nChkPos];

	CSG_CHECK_RES_CSA_S6 *pResD = ChkRes.pCSA;


	ConvertCSGDCOMF(LcomResD.aChkFor[0], pCpbr->ComFor);
	ConvertCSGEngToUblmForce(0, LcomResD.aUlmFor[0], pCpbr->UlForLC);
	pCpbr->dMax = LcomResD.aUlmFor[0].dMmax;

	CSG_MEMB_RES_CSA_S6_D_FLEX &MembFlex = (bPositive) ? MembRes.Detail_F[0] : MembRes.Detail_F[1];  

	ConvertCsgFlangeClassCSA(MembRes, *pResD, pCpbr->ClassD.nTopCL, pCpbr->ClassD.nBottomCL);
	pCpbr->ClassD.nWebCL = pResD->StrnD.FlexD.ClassD.nClassW;
	pCpbr->ClassD.nClass = pResD->StrnD.FlexD.ClassD.nClass;


	pCpbr->dMy   = MembFlex.dMey;  
	pCpbr->dMp   = MembFlex.dMpy;
	pCpbr->dMu   = pResD->StrnD.FlexD.ForceD.dMu;
	pCpbr->dMr   = pResD->StrnD.FlexD.dMr;  // dMr
	pCpbr->dMrat = pResD->StrnD.FlexD.dRat_My; // MQC 17834, Total Checking Table에서 Moment Ratio는 주축에 대한 Ratio로 표기
    pCpbr->dMfw_Mrz_rat = pResD->StrnD.FlexD.dRat_Mz;
	pCpbr->nOK   = (pResD->StrnD.FlexD.bOK_F==TRUE) ? 1 : 0;

}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgCsvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData)
{
	if (MembR.pCSA == NULL) return;
	if (ChkRes.pCSA == NULL) return;
	if (rData.pCpvrB == NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;
	if (LcomResD.aUlmFor.GetSize() == 0) return;

	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CPVR_BASE *pCpvr = rData.pCpvrB;

	CSG_MEMB_RES_CSA_S6_D &MembRes = MembR.pCSA->MembR[nChkPos];

	CSG_CHECK_RES_CSA_S6 *pResD = ChkRes.pCSA;

	ConvertCSGDCOMF(LcomResD.aChkFor[0], pCpvr->ComFor);
	ConvertCSGEngToUblmForce(0, LcomResD.aUlmFor[0], pCpvr->UlFor);
	pCpvr->dMax = LcomResD.aUlmFor[0].dMmax;

	ConvertCsgFlangeClassCSA(MembRes, *pResD, pCpvr->ClassD.nTopCL, pCpvr->ClassD.nBottomCL);
	pCpvr->ClassD.nWebCL = pResD->StrnD.FlexD.ClassD.nClassW;
	pCpvr->ClassD.nClass = pResD->StrnD.FlexD.ClassD.nClass;

	CSG_MEMB_RES_CSA_S6_D_SHEAR &MembShear = MembRes.Detail_S[0]; // 여기도 Moment 방향을 찾아야 하나?

	pCpvr->dVu      = pResD->StrnD.ShearD.ForceD.dVu;
	pCpvr->dVr      = MembShear.dVr;
	pCpvr->dMf      = pResD->StrnD.FlexD.ForceD.dMu;
	pCpvr->dMr      = pResD->StrnD.FlexD.dMr_ca;
	pCpvr->dRatio_v = pResD->StrnD.ShearD.dRat_z;
	pCpvr->dRatio_m = pResD->StrnD.FlexD.dRatio_F;
	pCpvr->dRatio_c = pResD->StrnD.ShearD.dCombRat;  
    pCpvr->dMfw_Mrz_rat = pResD->StrnD.FlexD.dRat_Mz;

	pCpvr->nOK  = (pResD->StrnD.ShearD.bOK==TRUE) ? 1 : 0;

}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgCsacRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_Ser, T_CSCH_REF &rData)
{
	if (MembR.pCSA == NULL) return;
	if (ChkRes.pCSA == NULL) return;
	if (rData.pCpacB == NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;
	if (LcomResD.aUlmFor.GetSize() == 0) return;

	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CPAC_BASE *pCpac = rData.pCpacB;

	CSG_MEMB_RES_CSA_S6_D &MembRes = MembR.pCSA->MembR[nChkPos];

	ConvertCSGDCOMF(LcomResD.aChkFor[0], pCpac->ComFor);
	ConvertCSGEngToUblmForce(0, LcomResD.aUlmFor[0], pCpac->UlFor);
	pCpac->dMax = LcomResD.aUlmFor[0].dMmax;

	CSG_SERV_RES_CSA_S6 &ServR = ChkRes.pCSA->ServD;

	double dS=0.0, dS3n=0.0, dSn=0.0, dSp=0.0;

	if (ServR.bChkTop==TRUE)
	{
		dS   = MembRes.Memb.Steel.dSTop;
		dS3n = MembRes.Memb.LongComp.dSTop;
		dSn  = MembRes.Memb.ShortComp.dSTop;
		dSp  = MembRes.Memb.SteelRebarComp.dSTop;
	}
	else
	{
		dS   = MembRes.Memb.Steel.dSBot;
		dS3n = MembRes.Memb.LongComp.dSBot;
		dSn  = MembRes.Memb.ShortComp.dSBot;
		dSp  = MembRes.Memb.SteelRebarComp.dSBot;
	}    

	ConvertCsgFlangeClassCSA(MembRes, *ChkRes.pCSA, pCpac->ClassD.nTopCL, pCpac->ClassD.nBottomCL);
	pCpac->ClassD.nWebCL = ServR.ClassD.nClassW;
	pCpac->ClassD.nClass = ServR.ClassD.nClass;

	int nDir = (ServR.bPositiveM==TRUE) ? 0 : 1;
	pCpac->dStress = ServR.dfmax;
	pCpac->d09Fy   = MembRes.Detail_F[nDir].dFsa_pd;
	pCpac->dRat    = ServR.dRatio;
	pCpac->dS      = dS;       
	pCpac->dS3n    = dS3n;   
	pCpac->dSn     = dSn;       
	pCpac->dSp     = dSp;   
	pCpac->nOK     = (ServR.bOK==TRUE) ? 1 : 0;

}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgFatiLoadRes(int nType, int nPart, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData)
{
	if (nType==1) return;
	if (MembR.pCSA == NULL) return;
	if (ChkRes.pCSA == NULL) return;
	if (rData.pCpfcB == NULL) return;
	if (nPart<0 || nPart>1) return;

	T_CPFC_LOAD *pCpfcLoad = &rData.pCpfcB->LoadR[nPart];

	CSG_CHECK_RES_CSA_S6 *pResD = ChkRes.pCSA;

	int nLCSize = LcomResD.aChkFor.GetSize();
	pCpfcLoad->ComForMV.nLcomSize = nLCSize;
	for (int i=0; i<nLCSize; ++i)
	{
		ConvertCSGDCOMF(LcomResD.aChkFor[i], pCpfcLoad->ComForMV.ForMV[i]);
	}
	

	int nId = nPart;
	CSG_FATI_LOAD_IND_RES_CSA_S6 &FatiFlgR = pResD->FatiD.FlgR[nId];
    if (!FatiFlgR.bFatiDisregard)
    {
        pCpfcLoad->bChk       = FatiFlgR.dFsr < cDGN_Zero ? FALSE : TRUE;
        pCpfcLoad->dfsr       = FatiFlgR.dfsr;
        pCpfcLoad->dStress_fa = FatiFlgR.dStress_fsr;
        pCpfcLoad->dFsr       = FatiFlgR.dFsr;
        pCpfcLoad->dRatio     = FatiFlgR.dRatio;
        pCpfcLoad->nOK        = (FatiFlgR.bOK==TRUE) ? 1 : 0;
    }
    else
    {
        pCpfcLoad->Initialize();
        pCpfcLoad->bChk = FALSE;
        pCpfcLoad->nOK  = 1;
    }
}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgFatiStudRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData)
{
	if (ChkRes.pCSA == NULL) return;
	if (rData.pCpfcB == NULL) return;
	int nLCSize = LcomResD.aChkFor.GetSize();
	if (nLCSize == 0)  return;

	T_CPFC_STUD *pCpfcStud = &rData.pCpfcB->StudR;

	pCpfcStud->ComForMV.nLcomSize = nLCSize;
	for (int i=0; i<nLCSize; ++i)
	{
		ConvertCSGDCOMF(LcomResD.aChkFor[i], pCpfcStud->ComForMV.ForMV[i]);
	}

	//  ConvertCSGDCOMF(LcomResD.aChkFor[0], pCpfcStud->ComFor);
	//  ConvertCSGEngToUlmForce(LcomResD.aUlmf[0], pCpfcStud->FlexR.UlFor);

	CSG_CHECK_RES_CSA_S6 *pResD = ChkRes.pCSA;

	pCpfcStud->dfsr       = pResD->FatiD.StudWeld.dfsr;
	pCpfcStud->dFsr       = pResD->FatiD.StudWeld.dFsr;
	pCpfcStud->dStress_fa = pResD->FatiD.StudWeld.dStress_fsr;
	pCpfcStud->dRatio     = pResD->FatiD.StudWeld.dRatio;
	pCpfcStud->nOK        = (pResD->FatiD.StudWeld.bOK==TRUE) ? 1 : 0;

}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgFatiSconRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData)
{
	if (MembR.pCSA == NULL) return;
	if (ChkRes.pCSA == NULL) return;
	if (rData.pCpfcB == NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;

	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;
	
	T_CPFC_BASE *pCpfc = rData.pCpfcB;

	CSG_MEMB_RES_CSA_S6_D &MembRes = MembR.pCSA->MembR[nChkPos];

	CSG_CHECK_RES_CSA_S6 *pResD = ChkRes.pCSA;

	if(!(pResD->FatiD.SConn.nRangeMaxIdx < 0 || pResD->FatiD.SConn.nRangeMinIdx < 0))
	{
		ConvertCSGDCOMF(LcomResD.aChkFor[pResD->FatiD.SConn.nRangeMaxIdx], pCpfc->SConnR.FatiForMax);
		ConvertCSGDCOMF(LcomResD.aChkFor[pResD->FatiD.SConn.nRangeMinIdx], pCpfc->SConnR.FatiForMin);
	}
	int nLCSize = LcomResD.aChkFor.GetSize();
	pCpfc->SConnR.ComForMV.nLcomSize = nLCSize;
	for (int i=0; i<nLCSize; ++i)
	{
		ConvertCSGDCOMF(LcomResD.aChkFor[i], pCpfc->SConnR.ComForMV.ForMV[i]);
	}

	//ConvertCSGEngToUlmForce(LcomResD.aUlmf[0], pCpfc->SConnR.UlFor);

	CSG_FATI_SHEAR_CONN_RES_CSA_S6 &SconD = pResD->FatiD.SConn;
	CSG_MEMB_RES_CSA_S6_D_CSCR &SConIn = MembRes.Detail_CSCR;

	pCpfc->SConnR.dFsr_D  = SconD.LoadR.dFsr;
	pCpfc->SConnR.dtau_rs = SconD.dtau_rs;
	pCpfc->SConnR.dVsc    = SconD.dVsc;
	pCpfc->SConnR.dRatio  = SconD.dRatio;
	pCpfc->SConnR.nOK     = SconD.bOK;
}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgCscrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData)
{
	if (MembR.pCSA == NULL) return;
	if (ChkRes.pCSA == NULL) return;
	if (rData.pCpfcB == NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;

	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CPFC_BASE *pCpfc = rData.pCpfcB;

	CSG_MEMB_RES_CSA_S6_D &MembRes = MembR.pCSA->MembR[nChkPos];

	CSG_CHECK_RES_CSA_S6 *pResD = ChkRes.pCSA;

	if(!(pResD->FatiD.SConn.nRangeMaxIdx < 0 || pResD->FatiD.SConn.nRangeMinIdx < 0))
	{
		ConvertCSGDCOMF(LcomResD.aChkFor[pResD->FatiD.SConn.nRangeMaxIdx], pCpfc->SConnR.FatiForMax);
		ConvertCSGDCOMF(LcomResD.aChkFor[pResD->FatiD.SConn.nRangeMinIdx], pCpfc->SConnR.FatiForMin);
	}
	int nLCSize = LcomResD.aChkFor.GetSize();
	pCpfc->SConnR.ComForMV.nLcomSize = nLCSize;
	for (int i=0; i<nLCSize; ++i)
	{
		ConvertCSGDCOMF(LcomResD.aChkFor[i], pCpfc->SConnR.ComForMV.ForMV[i]);
	}

	//ConvertCSGEngToUlmForce(LcomResD.aUlmf[0], pCpfc->SConnR.UlFor);

	CSG_FATI_SHEAR_CONN_RES_CSA_S6 &SconD = pResD->FatiD.SConn;
	CSG_MEMB_RES_CSA_S6_D_CSCR &SConIn = MembRes.Detail_CSCR;

	pCpfc->SConnR.dFsr_D  = SconD.LoadR.dFsr;
	pCpfc->SConnR.dtau_rs = SconD.dtau_rs;
	pCpfc->SConnR.dVsc    = SconD.dVsc;
	pCpfc->SConnR.dRatio  = SconD.dRatio;
	pCpfc->SConnR.nOK     = SconD.bOK;

}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgCscrMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData)
{
	if (MembR.pCSA == NULL) return;
	if (rData.pCascB==NULL) return;
	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CASC_BASE *pCasc = rData.pCascB;

	CSG_MEMB_RES_CSA_S6_D &MembRes = MembR.pCSA->MembR[nChkPos];

	CSG_MEMB_RES_CSA_S6_D_CSCR &SConRes = MembRes.Detail_CSCR;

	//   double dNreq = 0.0;
	//   if (nMcount_p>0 && nMcount_n>0)
	//   {
	//     dNreq = SConRes.dNreq;
	//   }
	//   else if (nMcount_p>0)
	//   {
	//     dNreq = SConRes.dNreq_p;
	//   }
	//   else if (nMcount_n>0)
	//   {
	//     dNreq = SConRes.dNreq_n;
	//   }

	pCasc->bChk   = SConRes.bChk_hdr ? TRUE : FALSE;
	pCasc->dhdr   = SConRes.dhdr;
	pCasc->dqr    = SConRes.dqr;
	pCasc->dN     = SConRes.dNuse;
	pCasc->dN_req = SConRes.dNreq;
	pCasc->dSCrat = (SConRes.dNreq<m_dZero) ? SConRes.dNuse/m_dZero : SConRes.dNuse/SConRes.dNreq;
	pCasc->nOK    = (SConRes.bChk_hdr && SConRes.bChk_S && SConRes.dNreq < SConRes.dNuse) ? 1 : 0;
}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgCsslMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData)
{
	if (MembR.pCSA == NULL) return;
	if (rData.pCplsB==NULL) return;
	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CPLS_BASE *pCpls = rData.pCplsB;

	CSG_MEMB_RES_CSA_S6_D &MembRes = MembR.pCSA->MembR[nChkPos];

	CSG_MEMB_RES_CSA_S6_D_CHK_L_STFN &LStfnD = MembRes.LStfnWeb;
	pCpls->bChk     = LStfnD.da > 0.0 ? TRUE : FALSE;
	pCpls->da       = LStfnD.da;
	pCpls->dhp      = LStfnD.dhp;
	pCpls->da_1p5hp = LStfnD.da_lim;
	pCpls->dwtr     = LStfnD.dwtr_ls;
	pCpls->dwtr_lim = LStfnD.dwtr_ls_lim;
	pCpls->dhpl     = LStfnD.dbl;
	pCpls->d30t     = LStfnD.dw_ps_ls_lim;
	pCpls->dI_ls    = LStfnD.dI_ls;
	pCpls->dI_ls_lim= LStfnD.dI_ls_lim;
	pCpls->dr       = LStfnD.dr_ls;
	pCpls->dr_lim   = LStfnD.dr_ls_lim;
	//pCpls->dRat     = LStfnD.;
	pCpls->nOK      = LStfnD.bOK ? 1 : 0;

}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgCptsMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData)
{
	if (MembR.pCSA == NULL) return;
	if (rData.pCptsB==NULL) return;
	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CPTS_BASE *pCpts = rData.pCptsB;

	CSG_MEMB_RES_CSA_S6_D &MembRes = MembR.pCSA->MembR[nChkPos];

	CSG_MEMB_RES_CSA_S6_D_CHK_T_STFN &TStfnD = MembRes.TStfn;
	pCpts->bChk     = TStfnD.dhwr > 0.0 ? TRUE : FALSE;
	pCpts->dhwr     = TStfnD.dhwr;
	pCpts->dhwr_lim = TStfnD.dhwr_lim;
	pCpts->da       = MembRes.LStfnWeb.da;
	pCpts->da_lim   = TStfnD.da_lim;
	//pCpts->dAs      = TStfnD.dAs;
	//pCpts->dht      = TStfnD.;
	//pCpts->dht_lim  = TStfnD.;
	pCpts->dIt      = TStfnD.dI_ts;
	pCpts->dIt_lim  = TStfnD.dI_ts_lim;    
	//pCpts->dVf      = TStfnD.0;
	//pCpts->dVr      = TStfnD.0;
	pCpts->dwtr     = TStfnD.dwtr_ts;
	pCpts->dwtr_lim = TStfnD.dwtr_ts_lim;
	pCpts->dwp      = TStfnD.dwp;
	pCpts->d30t     = TStfnD.dw_ps_ts_lim;
	//pCpts->dRat     = TStfnD.0;
	pCpts->nOK      = (TStfnD.bOK) ? 1 : 0;
}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgCptsResLcom(CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData)
{
	if (ChkRes.pCSA == NULL) return;
	if (rData.pCptsB==NULL) return;
	if (rData.pCpvrB==NULL) return;
	T_CPTS_BASE *pCpts = rData.pCptsB;

	T_CPVR_BASE *pCpvr = rData.pCpvrB;
	CSG_CHECK_RES_CSA_S6 *pResD = ChkRes.pCSA;

	pCpts->dVf = pCpvr->dVu;
	pCpts->dVr = pCpvr->dVr;
	pCpts->dAs = pResD->StrnD.TStfnD.dAs;
	pCpts->nOK = pResD->StrnD.TStfnD.bOK;
}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgBstfMembRes(CSG_MEMB_RESULT_D& MembR, CSG_MEMB_POSD_KSCE_LSD& LcomResD, T_CSCH_REF& rData)
{
	if (MembR.pCSA == NULL) return;
	if (rData.pCpbsB == NULL) return;
	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos < 0 || nChkPos>1) return;

	T_CPBS_BASE* pCpbs = rData.pCpbsB;

	const CSG_MEMB_RES_CSA_S6_D_CHK_B_STFN& BStfn = MembR.pCSA->MembR[nChkPos].BStfn;
	pCpbs->bChk = BStfn.bCheck;	
	pCpbs->dWTR = BStfn.dWTR;
	pCpbs->dWTRLimit = BStfn.dWTRLimit;
	pCpbs->dReact = BStfn.dReact;
	pCpbs->dBr = BStfn.dBr;
	pCpbs->dCr = BStfn.dCr;
	pCpbs->dRatio = BStfn.dRatio;
	pCpbs->nOK = BStfn.dRatio > 1.0 ? false : true;
}

void CDgnCsgDataCtrlCSA_S6_14::ConvertCsgFlangeClassCSA(CSG_MEMB_RES_CSA_S6_D &MembRes, CSG_CHECK_RES_CSA_S6 &ChkRes, int &nTopClass, int &nBotClass)
{
	nTopClass = MembRes.Detail_F[0].nClassTF;
	nBotClass = MembRes.Detail_F[0].nClassBF;
}


BOOL CDgnCsgDataCtrlCSA_S6_14::IsChkFatiLoadStud()
{
	return TRUE;
}

BOOL CDgnCsgDataCtrlCSA_S6_14::IsSCMembChk()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CDgnCsgDataCtrlCSA_S6_10::CDgnCsgDataCtrlCSA_S6_10() : CDgnCsgDataCtrlCSA_S6_14()
{ 
}

CDgnCsgDataCtrlCSA_S6_10::~CDgnCsgDataCtrlCSA_S6_10()
{
}
