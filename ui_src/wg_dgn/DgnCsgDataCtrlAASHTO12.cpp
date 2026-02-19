
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "DgnCsgDataCtrlAASHTO12.h"
#include "..\wg_db\DBLib.h"
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

CDgnCsgDataCtrlAASHTO12::CDgnCsgDataCtrlAASHTO12() : CDgnCsgDataCtrl()
{ 
}

CDgnCsgDataCtrlAASHTO12::~CDgnCsgDataCtrlAASHTO12()
{
}

void CDgnCsgDataCtrlAASHTO12::SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl)
{
	CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	T_UNIT_INDEX CodeUnit = CurrUnit;

	int nRptSelUnit = CDBLib::GetReportSelUnit();
	if(nRptSelUnit == CDBLib::EN_REPORT_UNIT_US)
	{
		CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
		CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_KIP;
	}
	else
	{
		CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
	}
	m_pDataCtrl->Set_UnitIndex(CodeUnit);

	InitialData(m_nDgnCode);
}

BOOL CDgnCsgDataCtrlAASHTO12::MakeMemberResultData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RESULT_D &rData)
{
	if (rData.pAASHTO==NULL) return FALSE;

	BOOL bCurved =  (CsgMembD.Memb.Csgd.nConsiderPv==0)? FALSE : TRUE; // 0:직선, 1:곡선

	m_pCsgManager->Check_MemberResult(CsgMembD, rData);

	CsgMembD.Memb.ChIf.nChkPos = 1;
	CsgMembD.Memb.ChIf.bCurved = bCurved;
	m_pCsgManager->Check_MemberResult(CsgMembD, rData);

	//MakeMemberResDataAASHTO(ElemK, CsgMembD, rData.pAASHTO->MembR[0]);   
	//CsgMembD.Memb.ChIf.nChkPos = 1;
	//MakeMemberResDataAASHTO(ElemK, CsgMembD, rData.pAASHTO->MembR[1]);
	
	return TRUE;
}

BOOL CDgnCsgDataCtrlAASHTO12::MakeMemberResDataAASHTO(T_ELEM_K ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RES_AASHTO_LRFD_D &rData)
{
	if (!m_pCsgManager->Check_MemberResAASHTO(CsgMembD, rData)) return FALSE;
	return TRUE;
}

BOOL CDgnCsgDataCtrlAASHTO12::Check_Constructibility(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL)  return FALSE;
	if (InD.aChkFor.GetSize()>0) InD.aChkFor[0].nLcomType = 3;
	BOOL bCheck = m_pCsgManager->Check_ConstructibilityRes(InD, MembRes, ChkRes);
	DtrR.dDTR_F = ChkRes.pAASHTO->ConsD.FlexD.dRatio;
	DtrR.dDTR_S = ChkRes.pAASHTO->ConsD.ShearD.dRatio;

	return bCheck;
}

BOOL CDgnCsgDataCtrlAASHTO12::Check_Strength(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;
	if (!m_pCsgManager->Check_StrengthRes(InD, MembRes, ChkRes)) return FALSE;
	int nMomentType = ChkRes.pAASHTO->StrengthD.FlexD[0].nCase > 0 ? 0 : 1;
	BOOL bPositive = ChkRes.pAASHTO->StrengthD.FlexD[0].nCase > 0 ? TRUE : FALSE;
	//dMuy = ChkRes.pAASHTO->StrengthD.FlexD[0].ForceD.dMu;
	DtrR.dDTR_F = GetAASHTOFlexureRatio(ChkRes.pAASHTO->StrengthD, bPositive);
	DtrR.dDTR_S = ChkRes.pAASHTO->StrengthD.ShearD.dRatio;

	return TRUE;
}

BOOL CDgnCsgDataCtrlAASHTO12::Check_Service(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;
	BOOL bCheck = m_pCsgManager->Check_ServiceRes(InD, MembRes, ChkRes);
	DtrR.dDTR_Ser = ChkRes.pAASHTO->ServiceD.dRatio;

	return bCheck;
}

BOOL CDgnCsgDataCtrlAASHTO12::Check_Fatigue(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR, CSG_FATI_CR_RSLT &FatiR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;
	BOOL bCheck = m_pCsgManager->Check_FatigueRes(InD, MembRes, ChkRes);
	DtrR.dDTR_FL[0] = ChkRes.pAASHTO->FatigueD.Fat_LoadInduceD.df_fat_rat;
	DtrR.dDTR_FL[1] = ChkRes.pAASHTO->FatigueD.Fat_LoadInduceD.df_fat_rat_B;
	//
	DtrR.dDTR_F_WebShr = ChkRes.pAASHTO->FatigueD.Fat_WebD.dvcf_rat;
	
	FatiR.bChkFatigueII_IJ[nPos] = ChkRes.pAASHTO->FatigueD.Fat_LoadInduceD.bChkFatigueII;
	FatiR.bFatigueI              = IsFatigue1(ChkRes.pAASHTO->FatigueD.Fat_LoadInduceD.bFatigueI);  
	return bCheck;
}

BOOL CDgnCsgDataCtrlAASHTO12::Check_ShearConnector(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;
	BOOL bCheck = m_pCsgManager->Check_ShearConnRes(InD, MembRes, ChkRes);
	DtrR.dDTR_F_SC_f = ChkRes.pAASHTO->ShearConnD.SC_FatiD.Fat_LoadInduceD.df_fat_rat;
	DtrR.dDTR_F_SC_p = ChkRes.pAASHTO->ShearConnD.SC_PitchD.dRatio;

	return bCheck;
}

BOOL CDgnCsgDataCtrlAASHTO12::Check_Stiffener(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;
	BOOL bCheck = m_pCsgManager->Check_StiffenerRes(InD, MembRes, ChkRes);
	DtrR.dDTR_Sf    = ChkRes.pAASHTO->HS_D.dRatio;
	DtrR.dDTR_Sf_fs = ChkRes.pAASHTO->HS_D.WebD.dRatiofs;

	return bCheck;
}

BOOL CDgnCsgDataCtrlAASHTO12::Check_BearingStiffener(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;
	if (InD.aReac.GetSize()==0) return FALSE;

	ChkRes.pAASHTO->BS_D.dReact = InD.aReac[0].dFz;
	BOOL bCheck = m_pCsgManager->Check_BearingStiffenerRes(InD, MembRes, ChkRes);
    double dRat1 = fabs(ChkRes.pAASHTO->BS_D.dPhiRbn) < m_dZero ? ChkRes.pAASHTO->BS_D.dReact / m_dZero : ChkRes.pAASHTO->BS_D.dReact / ChkRes.pAASHTO->BS_D.dPhiRbn;
    double dRat2 = fabs(ChkRes.pAASHTO->BS_D.dPr) < m_dZero ? ChkRes.pAASHTO->BS_D.dReact / m_dZero : ChkRes.pAASHTO->BS_D.dReact / ChkRes.pAASHTO->BS_D.dPr;
    DtrR.dDTR_Bs = max(dRat1, dRat2);

	return bCheck;
}

void CDgnCsgDataCtrlAASHTO12::GetMy4Strength(CSG_CHECK_RESULT_CODES &ChkRes, double &dMuy)
{
	if (ChkRes.pAASHTO==NULL) return;
	int nMomentType = ChkRes.pAASHTO->StrengthD.FlexD[0].nCase > 0 ? 0 : 1;
	dMuy = ChkRes.pAASHTO->StrengthD.FlexD[nMomentType].ForceD.dMu; 
}


void CDgnCsgDataCtrlAASHTO12::ClassNewMembResult(CSG_MEMB_RESULT_D &MembR)
{
	MembR.pAASHTO = new CSG_MEMBPOS_RES_AASHTO_LRFD_D;
}

void CDgnCsgDataCtrlAASHTO12::ClassNewDgnResult(CSG_CHECK_RESULT_CODES &ChkResD)
{
	ChkResD.pAASHTO = new CSG_MEMB_RES_AASHTO_LRFD_D;
}

void CDgnCsgDataCtrlAASHTO12::InitMembResult(CSG_MEMB_RESULT_D &MembR)
{
	if (MembR.pAASHTO==NULL) return;
	MembR.pAASHTO->Initialize();
}

void CDgnCsgDataCtrlAASHTO12::InitDgnResult(CSG_CHECK_RESULT_CODES &ChkResD)
{
	if (ChkResD.pAASHTO==NULL) return;
	ChkResD.pAASHTO->Initialize();
}

BOOL CDgnCsgDataCtrlAASHTO12::FindFatiCrRes4Lcom(int nFLoadType, int nIJ, BOOL *pbChkIJ, 
																								 int nFatiLcomType, int nDgnLcomId, int nOrgLcomNo,
																								 CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
																								 CSG_MEMB_RESULT_D &MembRes,
																								 int *pnFatigueII,
																								 int *pnLoadTop, int *pnLoadBot,
																								 double *pdLoadTopRat, double *pdLoadBotRat,
																								 CSG_CHK_RES_DTR_PARAM &DtrR,
																								 CSG_CHECK_RESULT_CODES &ChkResD,
																								 T_CSCH_REF &RsltRef,
																								 CSG_FATI_CR_RSLT &FRes)
{
	if (nIJ<0 || nIJ>1) return FALSE;

	T_CSFC_BASE CsfcFa;
	T_CSCR_BASE CscrTp;
	T_CSCH_REF FatiRef;
	FatiRef.pCsfcB = &CsfcFa;
	FatiRef.pCscrB = &CscrTp;
		
	CSG_MEMB_RES_AASHTO_LRFD_FAT_LI_D &FatLI = ChkResD.pAASHTO->FatigueD.Fat_LoadInduceD;

	int nTopBot = 0;
	if (FRes.bFatigueI==TRUE) // Fatigue I
	{
		if(FRes.dLoadTopRatF1[nIJ]< FatLI.df_fat_rat)
		{ 
			FRes.dLoadTopRatF1[nIJ] = FatLI.df_fat_rat;
			SetCsfcLoadBase(nFLoadType, pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nTopBot, ChkResD, FatiRef);
			ConvertCsgFatiLoadRes(nFLoadType, 0, MembRes, ChkResD, MembLcomD, FRes.dLoadTopRatF1[nIJ], FatiRef);
			FRes.LoadTR_F1[nIJ] = CsfcFa.LoadR[0];
		}
	}
	else // Fatigue II
	{
		if(FRes.dLoadTopRatF2[nIJ]< FatLI.df_fat_rat)
		{ 
			FRes.dLoadTopRatF2[nIJ] = FatLI.df_fat_rat;
			SetCsfcLoadBase(nFLoadType, pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nTopBot, ChkResD, FatiRef);
			ConvertCsgFatiLoadRes(nFLoadType, 0, MembRes, ChkResD, MembLcomD, FRes.dLoadTopRatF2[nIJ], FatiRef);
			FRes.LoadTR_F2[nIJ] = CsfcFa.LoadR[0];
		}
	}

	nTopBot = 1;
	if (FRes.bFatigueI==TRUE) // Fatigue I
	{
		if(FRes.dLoadBotRatF1[nIJ]< FatLI.df_fat_rat_B)
		{ 
			FRes.dLoadBotRatF1[nIJ] = FatLI.df_fat_rat_B;
			SetCsfcLoadBase(nFLoadType, pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nTopBot, ChkResD, FatiRef);
			ConvertCsgFatiLoadRes(nFLoadType, 1, MembRes, ChkResD, MembLcomD, FRes.dLoadBotRatF1[nIJ], FatiRef);
			FRes.LoadBR_F1[nIJ] = CsfcFa.LoadR[1];
		}
	}
	else // Fatigue II
	{
		if(FRes.dLoadBotRatF2[nIJ]< FatLI.df_fat_rat_B)
		{ 
			FRes.dLoadBotRatF2[nIJ] = FatLI.df_fat_rat_B;
			SetCsfcLoadBase(nFLoadType, pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nTopBot, ChkResD, FatiRef);
			ConvertCsgFatiLoadRes(nFLoadType, 1, MembRes, ChkResD, MembLcomD, FRes.dLoadBotRatF2[nIJ], FatiRef);
			FRes.LoadBR_F2[nIJ] = CsfcFa.LoadR[1];
		}
	}

	return TRUE;
}

BOOL CDgnCsgDataCtrlAASHTO12::FindFatiSCCrRes4Lcom(ElemPairK ElemK, int nIJ, BOOL *pbChkIJ,
																									 int nFatiLcomType, int nDgnLcomId, int nOrgLcomNo, int nMaxMinType,
																									 CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
																									 CSG_MEMB_RESULT_D &MembRes,
																									 int *pnSCcount,
																									 double *pdSCmaxRat,
																									 CSG_CHK_RES_DTR_PARAM &DtrR,
																									 CSG_CHECK_RESULT_CODES &ChkResD,
																									 T_CSCH_REF &RsltRef)
{

	return TRUE;
}

BOOL CDgnCsgDataCtrlAASHTO12::FindFatiSCLoadCrRes4Lcom(ElemPairK ElemK, int nIJ, BOOL *pbChkIJ,
																											 int nFatiLcomType, int nDgnLcomId, int nOrgLcomNo, int nMaxMinType,
																											 CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
																											 CSG_MEMB_RESULT_D &MembRes,
																											 int *pnFatigueII_SC,
																											 int *pnSCFati,
																											 double *pdSCFatigueRat,
																											 CSG_CHK_RES_DTR_PARAM &DtrR,
																											 CSG_CHECK_RESULT_CODES &ChkMaResD,
																											 CSG_CHECK_RESULT_CODES &ChkResD,
																											 T_CSCH_REF &RsltRef,
																											 CSG_FATI_CR_RSLT &FRes)
{
	T_CSCR_BASE CscrB;
	T_CSFC_BASE CsfcTp;
	T_CSCH_REF  FatiRef;
	FatiRef.pCscrB = &CscrB;  
	FatiRef.pCsfcB = &CsfcTp;

	double dRatio_p = 0.0;
	ConvertCsgCscrRes(MembRes, ChkMaResD, MembLcomD, dRatio_p, FatiRef);
 
	Check_ShearConnector(ElemK, nIJ, MembLcomD, MembRes, ChkMaResD, DtrR);  
	SetCscrBase(pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nMaxMinType, FatiRef);		
	dRatio_p = DtrR.dDTR_F_SC_p;
	ConvertCsgCscrMembRes(MembRes, MembLcomD, dRatio_p, FatiRef);
	// AASHTO-LRFD12, 6.6.1.2.1
	
    FRes.bChkFatigueSC_II_IJ[nIJ] = IsShearConnectorFatigue2(ChkMaResD);

	int nTopBot = 0;

	CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D &SCRes = ChkMaResD.pAASHTO->ShearConnD;

	int nFLoadType = 1;

	if (FRes.bFatigueI==TRUE) // Fatigue I
	{
		if(FRes.dSCPitchF1[nIJ] < CscrB.dSCrat)
		{
			FRes.dSCPitchF1[nIJ] = CscrB.dSCrat;
			FRes.SCresF1[nIJ] = CscrB;
		}
		//
		if(FRes.dSCFatigueRatF1[nIJ] < SCRes.SC_FatiD.Fat_LoadInduceD.df_fat_rat)
		{
			FRes.dSCFatigueRatF1[nIJ] = SCRes.SC_FatiD.Fat_LoadInduceD.df_fat_rat;      
			SetCsfcLoadBase(nFLoadType, pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nTopBot, ChkMaResD, FatiRef);
			ConvertCsgFatiLoadRes(nFLoadType, 0, MembRes, ChkMaResD, MembLcomD, FRes.dSCFatigueRatF1[nIJ], FatiRef);
			FRes.SCLres_F1[nIJ] = CscrB.LoadR[0];
		}
	}
	else // Fatigue II
	{
		if(FRes.dSCPitchF2[nIJ] < CscrB.dSCrat)
		{
			FRes.dSCPitchF2[nIJ] = CscrB.dSCrat;
			FRes.SCresF2[nIJ] = CscrB;
		}
		//
		if(FRes.dSCFatigueRatF2[nIJ] < SCRes.SC_FatiD.Fat_LoadInduceD.df_fat_rat)
		{
			FRes.dSCFatigueRatF2[nIJ] = SCRes.SC_FatiD.Fat_LoadInduceD.df_fat_rat;
			SetCsfcLoadBase(nFLoadType, pbChkIJ[nIJ], nDgnLcomId, nOrgLcomNo, nTopBot, ChkMaResD, FatiRef);
			ConvertCsgFatiLoadRes(nFLoadType, 0, MembRes, ChkMaResD, MembLcomD, FRes.dSCFatigueRatF2[nIJ], FatiRef);
			FRes.SCLres_F2[nIJ] = CscrB.LoadR[0];
		}
	}

	return TRUE;
}

void CDgnCsgDataCtrlAASHTO12::DecisionFatigueRes(CSG_FATI_CR_RSLT &FRes, T_CSFC_D &CsfcResD, T_CSCR_D &CscrResD)
{
	for (int i=0; i<2; ++i)
	{
		if (FRes.bChkFatigueII_IJ[i]==TRUE)
		{
			CsfcResD.CsfcB[i].LoadR[0] = FRes.LoadTR_F2[i];
			CsfcResD.CsfcB[i].LoadR[1] = FRes.LoadBR_F2[i];
		}
		else
		{
			CsfcResD.CsfcB[i].LoadR[0] = FRes.LoadTR_F1[i];
			CsfcResD.CsfcB[i].LoadR[1] = FRes.LoadBR_F1[i];
		}
		//
		if (FRes.bChkFatigueSC_II_IJ[i]==TRUE)
		{
			CscrResD.CscrB[i] = FRes.SCresF2[i];
			CscrResD.CscrB[i].LoadR[0] = FRes.SCLres_F2[i];
		}
		else
		{
			CscrResD.CscrB[i] = FRes.SCresF1[i];
			CscrResD.CscrB[i].LoadR[0] = FRes.SCLres_F1[i];
		}      
	}
}

double CDgnCsgDataCtrlAASHTO12::GetAASHTOFlexureRatio(CSG_MEMB_RES_AASHTO_LRFD_STR_D &Inp, BOOL bPositive)
{
	double dRatio = 0.0;
	CSG_MEMB_RES_AASHTO_LRFD_STR_FLEX_D& FlexD = Inp.FlexD[bPositive==FALSE];
	switch (FlexD.nCase)
	{
    case 1:
        dRatio = (FlexD.PFCSD.dpMn < m_dZero) ? fabs(FlexD.PFCSD.dMu/m_dZero) : fabs(FlexD.PFCSD.dMu/FlexD.PFCSD.dpMn);
        break;
    case 2:
        dRatio = FlexD.PFNSD.dRatio; // DgnEngine에서 계산한 값으로 수정
        break;
    case 3:
        dRatio = FlexD.NFD.dRatio; // DgnEngine에서 계산한 값으로 수정
        break;
    case 4:
        dRatio = FlexD.NFA6.dRatio; // DgnEngine에서 계산한 값으로 수정
        break;
	default:
		break;
	}

	return dRatio;
}

void CDgnCsgDataCtrlAASHTO12::SetCsfcLoadBase(int nType, BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nTopBot, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData)
{
	if (rData.pCsfcB == NULL) return;
	if (ChkRes.pAASHTO == NULL) return;
	if (nTopBot<0 || nTopBot>1) return;

	T_CSFC_LOAD *pFLoad;

	if      (nType==0)  pFLoad = &rData.pCsfcB->LoadR[nTopBot];
	else if (nType==1)  pFLoad = &rData.pCscrB->LoadR[nTopBot];
	else 
	{
		ASSERT(0);
		return ;
	}

	pFLoad->bChk      = bChk;
	pFLoad->nLcomK    = nDgnLcomNo;
	pFLoad->nOrgLcomK = nOrgLcomNo;

	int nfcMaxMinType = nTopBot==0 ? ChkRes.pAASHTO->FatigueD.Fat_LoadInduceD.nLoadTopCMax : ChkRes.pAASHTO->FatigueD.Fat_LoadInduceD.nLoadBotCMax;
	int nftMaxMinType = nTopBot==0 ? ChkRes.pAASHTO->FatigueD.Fat_LoadInduceD.nLoadTopTMax : ChkRes.pAASHTO->FatigueD.Fat_LoadInduceD.nLoadBotTMax;
	BOOL bMaxMin = Check_LcomMaxMinType(pFLoad->nOrgLcomK);
	pFLoad->nfcMax    = (bMaxMin)? GetLcomTypeIndex4MaxMin() : nfcMaxMinType; // nfcMaxMinType+1 : 0;
	pFLoad->nftMax    = (bMaxMin)? GetLcomTypeIndex4MaxMin() : nftMaxMinType; // nftMaxMinType+1 : 0;
}

void CDgnCsgDataCtrlAASHTO12::SetCsfcFlexBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData)
{
	if (rData.pCsfcB == NULL) return;
	rData.pCsfcB->FlexR.bChk      = bChk;
	rData.pCsfcB->FlexR.nLcomK    = nDgnLcomNo;
	rData.pCsfcB->FlexR.nOrgLcomK = nOrgLcomNo;

	BOOL bMaxMin = Check_LcomMaxMinType(nOrgLcomNo);
	rData.pCsfcB->FlexR.nMax      = (bMaxMin)? GetLcomTypeIndex4MaxMin() : nMaxMinType; //nMaxMinType+1 : 0;
}

void CDgnCsgDataCtrlAASHTO12::SetCsfcShearBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData)
{
	if (ChkRes.pAASHTO == NULL) return;
	if (rData.pCsfcB == NULL) return;
	// T_CSFC_SHEAR
	rData.pCsfcB->ShearR.bChk      = bChk;
	rData.pCsfcB->ShearR.nLcomK    = nDgnLcomNo;
	rData.pCsfcB->ShearR.nOrgLcomK = nOrgLcomNo;

	int nMaxMinType = ChkRes.pAASHTO->FatigueD.Fat_WebD.nShrMax;
	BOOL bMaxMin = Check_LcomMaxMinType(rData.pCsfcB->ShearR.nOrgLcomK);
	rData.pCsfcB->ShearR.nMax      = (bMaxMin)? GetLcomTypeIndex4MaxMin() : nMaxMinType; //nMaxMinType+1 : 0;
}



void CDgnCsgDataCtrlAASHTO12::ConvertCsgScbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData)
{
	if (ChkRes.pAASHTO == NULL) return;
	if (rData.pScbrB == NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;

	CSG_MEMB_RES_AASHTO_LRFD_CONSTRUCT &ConsR = ChkRes.pAASHTO->ConsD;
	T_SCBR_BASE *pScbr = rData.pScbrB;

	for (int i = 0; i<D_CSG_FORCE_COMPONENT; ++i)
	{
		pScbr->CsFor.dForce[i] = LcomResD.aChkFor[0].ForNC.dForce[i];
	}

	for (int i = 0; i<3/*LcomResD.aUlmf.GetSize()*/; ++i)
	{
		memcpy(pScbr->UlFor.For[i].dForce, LcomResD.aUlmf[0].ForD[i].dForce, sizeof(pScbr->UlFor.For[i].dForce));
		memcpy(pScbr->UlFor.CbFor[i].dForce, LcomResD.aUlmf[0].CbForce[i].dForce, sizeof(pScbr->UlFor.CbFor[i].dForce));
	}

	for (int i = 0; i < 2; ++i)
	{
		memcpy(pScbr->UlFor.For13[i].dForce, LcomResD.aUlmf[0].For13D[i].dForce, sizeof(pScbr->UlFor.For13[i].dForce));
	}
	pScbr->UlFor.MmaxFor.dForce[4] = LcomResD.aUlmFor[0].dMmax;

	if(ConsR.FlexD.bSlenderWeb)
	{
		pScbr->dfcw     = ConsR.FlexD.dfbuc;       
		pScbr->dfcw_lim = ConsR.FlexD.dpFcrw;
	}
	else
	{
		pScbr->dfcw     = 0.0;       
		pScbr->dfcw_lim = 0.0;
	}
	pScbr->dfcf     = ConsR.FlexD.dfbucSum1;
	pScbr->dflc     = ConsR.FlexD.flcD.dfl_d;
	pScbr->dfcf_lim = ConsR.FlexD.dfbnc1;       
	pScbr->dftf     = ConsR.FlexD.dfbut;     
	pScbr->dflt     = ConsR.FlexD.fltD.dfl_d;
	pScbr->dftf_lim = ConsR.FlexD.dpFnt;       
	pScbr->dfdeck   = ConsR.DeckD.dfdeck;  // deck응력
	pScbr->dphifr   = ConsR.DeckD.dpfr;  // deck응력 제한

	pScbr->dFactorCS = LcomResD.Memb.ChIf.dFactorCS;
	//   if(fabs(pScbr->dfcw_lim) > fabs(pScbr->dfcw))   pScbr->nOK = FALSE;
	//   if(fabs(pScbr->dfcf_lim) > fabs(pScbr->dfcf))   pScbr->nOK = FALSE;
	//   if(fabs(pScbr->dftf_lim) > fabs(pScbr->dftf))   pScbr->nOK = FALSE;
	//   if(fabs(pScbr->dfdeck)   > fabs(pScbr->dphifr)) pScbr->nOK = FALSE;

	pScbr->dRatio = dRatio_F;
	pScbr->nOK  = (dRatio_F>1.0) ? 0 : 1;
}

void CDgnCsgDataCtrlAASHTO12::ConvertCsgScvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData)
{
	if (ChkRes.pAASHTO == NULL) return;
	if (rData.pScvrB == NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;

	CSG_MEMB_RES_AASHTO_LRFD_CONSTRUCT &ResD = ChkRes.pAASHTO->ConsD;
	T_SCVR_BASE *pScvr = rData.pScvrB;

	for (int i = 0; i<D_CSG_FORCE_COMPONENT; ++i)
	{
		pScvr->CsFor.dForce[i] = LcomResD.aChkFor[0].ForNC.dForce[i];
	}

	for (int i = 0; i<3/*LcomD.aUlmf.GetSize()*/; ++i)
	{
		memcpy(pScvr->UlFor.For[i].dForce, LcomResD.aUlmf[0].ForD[i].dForce, sizeof(pScvr->UlFor.For[i].dForce));
		memcpy(pScvr->UlFor.CbFor[i].dForce, LcomResD.aUlmf[0].CbForce[i].dForce, sizeof(pScvr->UlFor.CbFor[i].dForce));
	}

	pScvr->dVu = MT0(ResD.ShearD.dVuInclined) ? ResD.ShearD.dVuInclined : ResD.ShearD.dVu;
	pScvr->dVu4T  = ResD.ShearD.dVui4Mx;
	pScvr->dVuApp = ResD.ShearD.dVuiApp;
	pScvr->dVr = ResD.ShearD.dpVn;
	pScvr->nOK = ResD.ShearD.dpVn == 0.0 ? FALSE : ( ( dRatio_S < 1.0+m_dZero ) ? TRUE : FALSE );
	pScvr->dRatio = dRatio_S; 
}

void CDgnCsgDataCtrlAASHTO12::ConvertCsgCsbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData)
{
	if (MembR.pAASHTO == NULL) return;
	if (ChkRes.pAASHTO == NULL) return;
	if (rData.pCsbrB == NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;
	
	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CSBR_BASE *pCsbr = rData.pCsbrB;

	memcpy(pCsbr->ComFor.ForLT.dForce, LcomResD.aChkFor[0].ForLT.dForce, sizeof(pCsbr->ComFor.ForLT.dForce));
	memcpy(pCsbr->ComFor.ForNC.dForce, LcomResD.aChkFor[0].ForNC.dForce, sizeof(pCsbr->ComFor.ForNC.dForce));
	memcpy(pCsbr->ComFor.ForST.dForce, LcomResD.aChkFor[0].ForST.dForce, sizeof(pCsbr->ComFor.ForST.dForce));
	memcpy(pCsbr->ComFor.ForLC.dForce, LcomResD.aChkFor[0].ForLC.dForce, sizeof(pCsbr->ComFor.ForLC.dForce));

	for (int i = 0; i<3/*MembLcom.aUlmf.GetSize()*/; ++i)
	{
        memcpy(pCsbr->UlForLC.For[i].dForce, LcomResD.aUlmf[0].ForD[i].dForce, sizeof(pCsbr->UlForLC.For[i].dForce));
        memcpy(pCsbr->UlForNC.For[i].dForce, LcomResD.aUlmf[1].ForD[i].dForce, sizeof(pCsbr->UlForNC.For[i].dForce));
        memcpy(pCsbr->UlForLT.For[i].dForce, LcomResD.aUlmf[2].ForD[i].dForce, sizeof(pCsbr->UlForLT.For[i].dForce));
        memcpy(pCsbr->UlForST.For[i].dForce, LcomResD.aUlmf[3].ForD[i].dForce, sizeof(pCsbr->UlForST.For[i].dForce));

        memcpy(pCsbr->UlForLC.CbFor[i].dForce, LcomResD.aUlmf[0].CbForce[i].dForce, sizeof(pCsbr->UlForLC.CbFor[i].dForce));
        memcpy(pCsbr->UlForNC.CbFor[i].dForce, LcomResD.aUlmf[1].CbForce[i].dForce, sizeof(pCsbr->UlForNC.CbFor[i].dForce));
        memcpy(pCsbr->UlForLT.CbFor[i].dForce, LcomResD.aUlmf[2].CbForce[i].dForce, sizeof(pCsbr->UlForLT.CbFor[i].dForce));
        memcpy(pCsbr->UlForST.CbFor[i].dForce, LcomResD.aUlmf[3].CbForce[i].dForce, sizeof(pCsbr->UlForST.CbFor[i].dForce));
	}

	for (int i = 0; i < 2; ++i)
	{
		memcpy(pCsbr->UlForLC.For13[i].dForce, LcomResD.aUlmf[0].For13D[i].dForce, sizeof(pCsbr->UlForLC.For13[i].dForce));
		memcpy(pCsbr->UlForNC.For13[i].dForce, LcomResD.aUlmf[1].For13D[i].dForce, sizeof(pCsbr->UlForNC.For13[i].dForce));
		memcpy(pCsbr->UlForLT.For13[i].dForce, LcomResD.aUlmf[2].For13D[i].dForce, sizeof(pCsbr->UlForLT.For13[i].dForce));
		memcpy(pCsbr->UlForST.For13[i].dForce, LcomResD.aUlmf[3].For13D[i].dForce, sizeof(pCsbr->UlForST.For13[i].dForce));
	}
	pCsbr->UlForLC.MmaxFor.dForce[4] = LcomResD.aUlmFor[0].dMmax;

	//rData.ComFor.ForLC = MembLcom.aChkFor[0].ForLC;
	pCsbr->bChkUlmf = LcomResD.bChkUlmf;

	
	CSG_MEMB_RES_AASHTO_LRFD_D &MembRes = MembR.pAASHTO->MembR[nChkPos];
	CSG_MEMB_RES_AASHTO_LRFD_STR_FLEX_D &FlexD = bPositive ?  MembRes.StrengthD.FlexD[0] : MembRes.StrengthD.FlexD[1];
	pCsbr->nCase = FlexD.nCase;
	pCsbr->dMrat = dRatio_F;
	switch(FlexD.nCase)
	{
	case 1: //정모멘트 compact
		pCsbr->dMy    = FlexD.PFCSD.YieldMomentD.dMy;  
		pCsbr->dMp    = FlexD.PFCSD.PlasticMomentD.dMp;
		pCsbr->dMu    = FlexD.PFCSD.dMu;
		pCsbr->dMr    = FlexD.PFCSD.dpMn;
		pCsbr->dDp    = FlexD.PFCSD.Ductility.dDp;   // Dp <= 0.42Dt 일 때 OK						
		pCsbr->d042Dt = FlexD.PFCSD.Ductility.dDplim;
		pCsbr->nOK    = FlexD.PFCSD.nOK;
		break;
	case 2: //정모멘트 noncompact
		{
			double dfc  = FlexD.PFNSD.dfbuc;
			double dfcr = FlexD.PFNSD.dpFnc;
			double dft  = FlexD.PFNSD.dfbutSum;
			double dftr = FlexD.PFNSD.dpFnt;
            double dRat_c = (dfcr==0.0)? fabs(dfc/m_dZero) : fabs(dfc/dfcr);
            double dRat_t = (dftr==0.0)? fabs(dft/m_dZero) : fabs(dft/dftr);
            pCsbr->dFu    = (dRat_c >= dRat_t) ? FlexD.PFNSD.dfbuc : FlexD.PFNSD.dfbutSum; // 결과테이블에 dFu, dFr만 사용
            pCsbr->dFr    = (dRat_c >= dRat_t) ? FlexD.PFNSD.dpFnc : FlexD.PFNSD.dpFnt;

			pCsbr->dFuc   = FlexD.PFNSD.dfbuc;
			pCsbr->dFrc   = FlexD.PFNSD.dpFnc;   
			pCsbr->dMr    = (dRat_c>=dRat_t)? FlexD.PFNSD.dMr_c : FlexD.PFNSD.dMr_t;   // for draw span graph MQC 62 PCS(14.06.17)
			pCsbr->dMr    = AbsMin(FlexD.PFNSD.dMr_c, FlexD.PFNSD.dMr_t);
			pCsbr->dMu    = (dRat_c>=dRat_t)? FlexD.PFNSD.dMu_c : FlexD.PFNSD.dMu_t;   // for draw span graph MQC 62 PCS(14.06.17)

			pCsbr->dDp    = FlexD.PFNSD.Ductility.dDp;   // Dp <= 0.42Dt 일 때 OK						
			pCsbr->d042Dt = FlexD.PFNSD.Ductility.dDplim;
			pCsbr->nOK    = FlexD.PFNSD.nOK;
			break;
		}
	case 3: //부모멘트
		{
			double dfc  = FlexD.NFD.dfbucSum;
			double dfcr = FlexD.NFD.dpFnc;
			double dft  = FlexD.NFD.dfbut;
			double dftr = FlexD.NFD.dpFnt;
            double dRat_c = (dfcr==0.0)? fabs(dfc/m_dZero) : fabs(dfc/dfcr);
            double dRat_t = (dftr==0.0)? fabs(dft/m_dZero) : fabs(dft/dftr);
            pCsbr->dFu  = (dRat_c>=dRat_t)? FlexD.NFD.dfbucSum : FlexD.NFD.dfbutSum; // 결과테이블에 dFu, dFr만 사용
            pCsbr->dFr  = (dRat_c>=dRat_t)? FlexD.NFD.dpFnc : FlexD.NFD.dpFnt   ;

			pCsbr->dFuc = FlexD.NFD.dfbucSum;
			pCsbr->dFrc = FlexD.NFD.dpFnc;
			pCsbr->dMr  = (dRat_c>=dRat_t)? FlexD.NFD.dMr_c : FlexD.NFD.dMr_t;     // for draw span graph MQC 62 PCS(14.06.17)
			pCsbr->dMr  = AbsMin(FlexD.NFD.dMr_c, FlexD.NFD.dMr_t);
			pCsbr->dMu  = (dRat_c>=dRat_t)? FlexD.NFD.dMu_c : FlexD.NFD.dMu_t;     // for draw span graph MQC 62 PCS(14.06.17)

			pCsbr->nOK  = FlexD.NFD.nOK;
			break;
		}
	case 4: //부모멘트
		{
            pCsbr->dMy  = FlexD.NFA6.YieldMomentD.dMy;
            pCsbr->dMp  = FlexD.NFA6.PlasticMomentD.dMp;

            double dMu = 0.0;
            double dMr = 0.0;
            if ( FlexD.NFA6.enMnCase == EN_Mnc_FLB )
            {
                dMu = FlexD.NFA6.dMuSum_F;
                dMr = FlexD.NFA6.dpMnc_cr;
            }
            else if ( FlexD.NFA6.enMnCase == EN_Mnc_LTB )
            {
                dMu = FlexD.NFA6.dMuSum_L;
                dMr = FlexD.NFA6.dpMnc_cr;
            }
            else if ( FlexD.NFA6.enMnCase == EN_Mnt )
            {
                dMu = FlexD.NFA6.dMuy;
                dMr = FlexD.NFA6.dpMnt;
            }
            pCsbr->dMu  = dMu;
            pCsbr->dMr  = dMr;
            pCsbr->nOK  = FlexD.NFA6.nOK;
			break;
		}
	default:
		break;
	} 
}

void CDgnCsgDataCtrlAASHTO12::ConvertCsgCsvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData)
{
	if (MembR.pAASHTO == NULL) return;
	if (rData.pCsvrB == NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;

	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CSVR_BASE *pCsvr = rData.pCsvrB;

	memcpy(pCsvr->ComFor.ForLT.dForce, LcomResD.aChkFor[0].ForLT.dForce, sizeof(pCsvr->ComFor.ForLT.dForce));
	memcpy(pCsvr->ComFor.ForNC.dForce, LcomResD.aChkFor[0].ForNC.dForce, sizeof(pCsvr->ComFor.ForNC.dForce));
	memcpy(pCsvr->ComFor.ForST.dForce, LcomResD.aChkFor[0].ForST.dForce, sizeof(pCsvr->ComFor.ForST.dForce));
	memcpy(pCsvr->ComFor.ForLC.dForce, LcomResD.aChkFor[0].ForLC.dForce, sizeof(pCsvr->ComFor.ForLC.dForce));

	for (int i = 0; i<3/*LcomResD.aUlmf.GetSize()*/; ++i)
	{
		memcpy(pCsvr->UlFor.For[i].dForce, LcomResD.aUlmf[0].ForD[i].dForce, sizeof(pCsvr->UlFor.For[i].dForce));
		memcpy(pCsvr->UlFor.CbFor[i].dForce, LcomResD.aUlmf[0].CbForce[i].dForce, sizeof(pCsvr->UlFor.CbFor[i].dForce));
	}

	CSG_MEMB_RES_AASHTO_LRFD_D &MembRes = MembR.pAASHTO->MembR[nChkPos];

	CSG_MEMB_RES_AASHTO_LRFD_STR_SHEAR_D &ShrR = MembRes.StrengthD.ShearD;
	pCsvr->dVu      = ShrR.dVu;
	pCsvr->dVu4T    = ShrR.dVui4Mx;
	pCsvr->dVuApp   = ShrR.dVuiApp;
	pCsvr->dVr      = ShrR.dpVn;      
	pCsvr->ddo_lim  = 0.0;
	pCsvr->ddo      = ShrR.TrsvStfnD.dd; 
	pCsvr->dbt_lim1 = ShrR.TrsvStfnD.dbt_lim1;
	pCsvr->dbt_lim2 = ShrR.TrsvStfnD.dbt_lim2;
	pCsvr->dbt_lim3 = ShrR.TrsvStfnD.dbt_lim3;
	pCsvr->dbt      = ShrR.TrsvStfnD.dbt;      
	pCsvr->dIt_lim  = ShrR.TrsvStfnD.dItmin;
	pCsvr->dIt      = ShrR.TrsvStfnD.dIt;      
	pCsvr->dAs_lim  = 0.0;
	pCsvr->dAs      = 0.0;

	BOOL bChk_do = TRUE;
	BOOL bChk_bt = TRUE;
	BOOL bChk_It = TRUE;
	BOOL bChk_As = TRUE;
	if (ShrR.bStiffenedWeb == TRUE)
	{
		if(pCsvr->dbt >=pCsvr->dbt_lim1)
		{
			if( pCsvr->dbt_lim2 >= pCsvr->dbt)      bChk_bt = TRUE;
			else                                    bChk_bt = FALSE;

			if(LcomResD.Memb.PosD[0].Sect.SectInfo.nStype != DGN_SECT_TYPE_COMPO_B)
			{
				if(pCsvr->dbt >=pCsvr->dbt_lim3)      bChk_bt = TRUE;
				else                                  bChk_bt = FALSE;
			}
		}
		else
		{
			bChk_bt = FALSE;
		}
		bChk_It = (pCsvr->dIt_lim <=pCsvr->dIt ? TRUE : FALSE);   // OK
		bChk_As = (pCsvr->dAs_lim <=pCsvr->dAs ? TRUE : FALSE);   // OK
	}

	pCsvr->bChk_do = bChk_do;
	pCsvr->bChk_bt = bChk_bt;
	pCsvr->bChk_It = bChk_It;
	pCsvr->bChk_As = bChk_As;

	BOOL bChk1 = (bChk_do && bChk_bt && bChk_It && bChk_As) ? TRUE : FALSE;
	BOOL bChk2 = ShrR.dRatio < 1.0+m_dZero ? TRUE : FALSE;

	pCsvr->dVrat = dRatio_S;
	pCsvr->nOK  = (bChk1 && bChk2)? 1 : 0;
}

void CDgnCsgDataCtrlAASHTO12::ConvertCsgCsacRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_Ser, T_CSCH_REF &rData)
{
	if (MembR.pAASHTO == NULL) return;
	if (ChkRes.pAASHTO == NULL) return;
	if (rData.pCsacB == NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;

	T_CSAC_BASE *pCsac = rData.pCsacB;

	memcpy(pCsac->ComFor.ForLT.dForce, LcomResD.aChkFor[0].ForLT.dForce, sizeof(pCsac->ComFor.ForLT.dForce));
	memcpy(pCsac->ComFor.ForNC.dForce, LcomResD.aChkFor[0].ForNC.dForce, sizeof(pCsac->ComFor.ForNC.dForce));
	memcpy(pCsac->ComFor.ForST.dForce, LcomResD.aChkFor[0].ForST.dForce, sizeof(pCsac->ComFor.ForST.dForce));
	memcpy(pCsac->ComFor.ForLC.dForce, LcomResD.aChkFor[0].ForLC.dForce, sizeof(pCsac->ComFor.ForLC.dForce));

	for (int i = 0; i<3/*LcomResD.aUlmf.GetSize()*/; ++i)
	{
		memcpy(pCsac->UlFor.For[i].dForce, LcomResD.aUlmf[0].ForD[i].dForce, sizeof(pCsac->UlFor.For[i].dForce));
		memcpy(pCsac->UlFor.CbFor[i].dForce, LcomResD.aUlmf[0].CbForce[i].dForce, sizeof(pCsac->UlFor.CbFor[i].dForce));
	}

	CSG_MEMB_RES_AASHTO_LRFD_SER_D &SerD = ChkRes.pAASHTO->ServiceD;
	pCsac->dfcw     = SerD.dfc;       
	pCsac->dfcw_lim = SerD.dFcrw;   
	pCsac->dfcf     = SerD.dffc;       
	pCsac->dfcf_lim = SerD.dffcn;
	pCsac->dftf     = SerD.dfuSum;       
	pCsac->dftf_lim = SerD.dfftn;
	pCsac->bChk_6_10_4_4_2   = SerD.bChk_6_10_4_4_2;
	pCsac->bChk_6_10_4_4_2_4 = SerD.bChk_6_10_4_4_2_4;
	pCsac->nOK      = SerD.nOK;
	pCsac->dfrat = dRatio_Ser;

}

void CDgnCsgDataCtrlAASHTO12::ConvertCsgFatiLoadRes(int nType, int nPart, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData)
{
	if (MembR.pAASHTO == NULL) return;
	if (ChkRes.pAASHTO == NULL) return;
	if (rData.pCsfcB == NULL) return;
	if (rData.pCscrB == NULL) return;
	int nChkForSize = LcomResD.aChkFor.GetSize();

	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;
	if (nPart<0 || nPart>1) return;

	T_CSFC_LOAD *pCsfcLoad;

	if      (nType==0)  pCsfcLoad = &rData.pCsfcB->LoadR[nPart];
	else if (nType==1)  pCsfcLoad = &rData.pCscrB->LoadR[nPart];
	else 
	{
		ASSERT(0);
		return ;
	}

	CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D &ResD = nType==0 ? ChkRes.pAASHTO->FatigueD : ChkRes.pAASHTO->ShearConnD.SC_FatiD;
	CSG_MEMB_RES_AASHTO_LRFD_D &MembRes = MembR.pAASHTO->MembR[nChkPos];

	pCsfcLoad->ComForMV.nLcomSize = nChkForSize;
	if (nPart==0) // Top flange
	{
		if (ResD.Fat_LoadInduceD.nLoadTopCMax != -1 && ResD.Fat_LoadInduceD.nLoadTopCMax<nChkForSize)
		{      
			pCsfcLoad->nForId[0] = ResD.Fat_LoadInduceD.nLoadTopCMax;      
			for (int i=0; i<nChkForSize; ++i)
			{
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForLT.dForce, LcomResD.aChkFor[i].ForLT.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForLT.dForce));
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForNC.dForce, LcomResD.aChkFor[i].ForNC.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForNC.dForce));
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForST.dForce, LcomResD.aChkFor[i].ForST.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForST.dForce));
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForLC.dForce, LcomResD.aChkFor[i].ForLC.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForLC.dForce));
			}
			pCsfcLoad->ComForMV.ForMV[ResD.Fat_LoadInduceD.nLoadTopCMax].nFatigueI = LcomResD.aChkFor[ResD.Fat_LoadInduceD.nLoadTopCMax].nLcomSub;
			pCsfcLoad->ComForMV.ForMV[ResD.Fat_LoadInduceD.nLoadTopCMax].dLFRatio  = dRatio;
		}
		if (ResD.Fat_LoadInduceD.nLoadTopTMax != -1 && ResD.Fat_LoadInduceD.nLoadTopTMax<nChkForSize)
		{
			pCsfcLoad->nForId[1] = ResD.Fat_LoadInduceD.nLoadTopTMax;
			for (int i=0; i<nChkForSize; ++i)
			{
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForLT.dForce, LcomResD.aChkFor[i].ForLT.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForLT.dForce));
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForNC.dForce, LcomResD.aChkFor[i].ForNC.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForNC.dForce));
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForST.dForce, LcomResD.aChkFor[i].ForST.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForST.dForce));
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForLC.dForce, LcomResD.aChkFor[i].ForLC.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForLC.dForce));
			}
			pCsfcLoad->ComForMV.ForMV[ResD.Fat_LoadInduceD.nLoadTopTMax].nFatigueI = LcomResD.aChkFor[ResD.Fat_LoadInduceD.nLoadTopTMax].nLcomSub;
			pCsfcLoad->ComForMV.ForMV[ResD.Fat_LoadInduceD.nLoadTopTMax].dLFRatio  = dRatio;
		}

		pCsfcLoad->dfc        = ResD.Fat_LoadInduceD.dft_c;
		pCsfcLoad->dft        = ResD.Fat_LoadInduceD.dft_t;
		pCsfcLoad->dGammaDelF = ResD.Fat_LoadInduceD.dGamma_f;
		pCsfcLoad->dDelF_n    = nType==0 ? MembRes.FatigueD.Fat_LoadInduceD.dFn : MembRes.ShearConnD.SC_FatiD.Fat_LoadInduceD.dFn;
		if (ResD.Fat_LoadInduceD.bIsCheckNeed_top == FALSE) 
		{
			pCsfcLoad->nOK = 1; 
		}
		else 
		{
			pCsfcLoad->nOK = (ResD.Fat_LoadInduceD.bCheck_fat) ? 1 : 0; 
		}

		pCsfcLoad->dRatio  = dRatio;

		//AASHTO LRFD 12 STL COMPOSITE - MQC 54, CSPARK (14.06.15)
		pCsfcLoad->dGammaDelF_top = ResD.Fat_LoadInduceD.dGamma_f_top;
		pCsfcLoad->dGammaDelF_bot = ResD.Fat_LoadInduceD.dGamma_f_bot;
		pCsfcLoad->dftop_com_st   = ResD.Fat_LoadInduceD.df_top_com_st; 
		pCsfcLoad->dftop_ten_st   = ResD.Fat_LoadInduceD.df_top_ten_st;
		pCsfcLoad->dfbot_com_st   = ResD.Fat_LoadInduceD.df_bot_com_st; 
		pCsfcLoad->dfbot_ten_st   = ResD.Fat_LoadInduceD.df_bot_ten_st;
		pCsfcLoad->dMyMax_Pos     = ResD.Fat_LoadInduceD.dMymax_pos;
		pCsfcLoad->dMyMax_Neg     = ResD.Fat_LoadInduceD.dMymax_nes; 

		pCsfcLoad->bChkLoadInducedFati_top = ResD.Fat_LoadInduceD.bIsCheckNeed_top;
		pCsfcLoad->bChkLoadInducedFati_bot = ResD.Fat_LoadInduceD.bIsCheckNeed_bot;
	}
	else
	{
		if (ResD.Fat_LoadInduceD.nLoadBotCMax != -1 && ResD.Fat_LoadInduceD.nLoadBotCMax<nChkForSize)
		{
			pCsfcLoad->nForId[0] = ResD.Fat_LoadInduceD.nLoadBotCMax;
			for (int i=0; i<nChkForSize; ++i)
			{
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForLT.dForce, LcomResD.aChkFor[i].ForLT.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForLT.dForce));
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForNC.dForce, LcomResD.aChkFor[i].ForNC.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForNC.dForce));
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForST.dForce, LcomResD.aChkFor[i].ForST.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForST.dForce));
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForLC.dForce, LcomResD.aChkFor[i].ForLC.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForLC.dForce));
			}
			pCsfcLoad->ComForMV.ForMV[ResD.Fat_LoadInduceD.nLoadBotCMax].nFatigueI = LcomResD.aChkFor[ResD.Fat_LoadInduceD.nLoadBotCMax].nLcomSub;
			pCsfcLoad->ComForMV.ForMV[ResD.Fat_LoadInduceD.nLoadBotCMax].dLFRatio  = dRatio;
		}
		if (ResD.Fat_LoadInduceD.nLoadBotTMax != -1 && ResD.Fat_LoadInduceD.nLoadBotTMax<nChkForSize)
		{
			pCsfcLoad->nForId[1] = ResD.Fat_LoadInduceD.nLoadBotCMax;
			for (int i=0; i<nChkForSize; ++i)
			{
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForLT.dForce, LcomResD.aChkFor[i].ForLT.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForLT.dForce));
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForNC.dForce, LcomResD.aChkFor[i].ForNC.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForNC.dForce));
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForST.dForce, LcomResD.aChkFor[i].ForST.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForST.dForce));
				memcpy(pCsfcLoad->ComForMV.ForMV[i].ForLC.dForce, LcomResD.aChkFor[i].ForLC.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForLC.dForce));
			}
			pCsfcLoad->ComForMV.ForMV[ResD.Fat_LoadInduceD.nLoadBotTMax].nFatigueI = LcomResD.aChkFor[ResD.Fat_LoadInduceD.nLoadBotTMax].nLcomSub;
			pCsfcLoad->ComForMV.ForMV[ResD.Fat_LoadInduceD.nLoadBotTMax].dLFRatio  = dRatio;
		}

		pCsfcLoad->dfc        = ResD.Fat_LoadInduceD.dfb_c;
		pCsfcLoad->dft        = ResD.Fat_LoadInduceD.dfb_t;
		pCsfcLoad->dGammaDelF = ResD.Fat_LoadInduceD.dGamma_f;     
		pCsfcLoad->dDelF_n    = nType==0 ? MembRes.FatigueD.Fat_LoadInduceD.dFn : MembRes.ShearConnD.SC_FatiD.Fat_LoadInduceD.dFn;
		if (ResD.Fat_LoadInduceD.bIsCheckNeed_bot == FALSE) 
		{
			pCsfcLoad->nOK = 1; 
		}
		else 
		{
			pCsfcLoad->nOK = (ResD.Fat_LoadInduceD.bCheck_fat_B) ? 1 : 0;
		}

		pCsfcLoad->dRatio  = dRatio;

		//AASHTO LRFD 12 STL COMPOSITE - MQC 54, CSPARK (14.06.15)
		pCsfcLoad->dGammaDelF_top = ResD.Fat_LoadInduceD.dGamma_f_top;
		pCsfcLoad->dGammaDelF_bot = ResD.Fat_LoadInduceD.dGamma_f_bot;
		pCsfcLoad->dftop_com_st   = ResD.Fat_LoadInduceD.df_top_com_st; 
		pCsfcLoad->dftop_ten_st   = ResD.Fat_LoadInduceD.df_top_ten_st;
		pCsfcLoad->dfbot_com_st   = ResD.Fat_LoadInduceD.df_bot_com_st; 
		pCsfcLoad->dfbot_ten_st   = ResD.Fat_LoadInduceD.df_bot_ten_st;
		pCsfcLoad->dMyMax_Pos     = ResD.Fat_LoadInduceD.dMymax_pos;
		pCsfcLoad->dMyMax_Neg     = ResD.Fat_LoadInduceD.dMymax_nes; 

		pCsfcLoad->bChkLoadInducedFati_top = ResD.Fat_LoadInduceD.bIsCheckNeed_top;
		pCsfcLoad->bChkLoadInducedFati_bot = ResD.Fat_LoadInduceD.bIsCheckNeed_bot;
	}

	pCsfcLoad->bChkLoadInducedFati = ResD.Fat_LoadInduceD.bChkLoadInducedFati;
	if(!ResD.Fat_LoadInduceD.bChkLoadInducedFati) pCsfcLoad->nOK = TRUE; 

	pCsfcLoad->bPrtLoadInducedFati = ResD.Fat_LoadInduceD.bPrtLoadInducedFati;
	if(!ResD.Fat_LoadInduceD.bPrtLoadInducedFati)
	{
		pCsfcLoad->dGammaDelF = 0.0;
		pCsfcLoad->dDelF_n    = 0.0;
	}
}

void CDgnCsgDataCtrlAASHTO12::ConvertCsgFatiShearRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData)
{
	if (ChkRes.pAASHTO == NULL) return;
	if (rData.pCsfcB == NULL) return;
	CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D &ResD = ChkRes.pAASHTO->FatigueD;
	T_CSFC_SHEAR *pCsfcShear = &rData.pCsfcB->ShearR;

	memcpy(pCsfcShear->ComFor.ForLT.dForce, LcomResD.aChkFor[ResD.Fat_WebD.nShrMax].ForLT.dForce, sizeof(pCsfcShear->ComFor.ForLT.dForce));
	memcpy(pCsfcShear->ComFor.ForNC.dForce, LcomResD.aChkFor[ResD.Fat_WebD.nShrMax].ForNC.dForce, sizeof(pCsfcShear->ComFor.ForNC.dForce));
	memcpy(pCsfcShear->ComFor.ForST.dForce, LcomResD.aChkFor[ResD.Fat_WebD.nShrMax].ForST.dForce, sizeof(pCsfcShear->ComFor.ForST.dForce));
	memcpy(pCsfcShear->ComFor.ForLC.dForce, LcomResD.aChkFor[ResD.Fat_WebD.nShrMax].ForLC.dForce, sizeof(pCsfcShear->ComFor.ForLC.dForce));

	pCsfcShear->dVcf     = (fabs(ResD.Fat_WebD.dVu) > 0.0 && fabs(ResD.Fat_WebD.dVuInclined) > 0.0) ? ResD.Fat_WebD.dVuInclined : ResD.Fat_WebD.dVu;
	pCsfcShear->dVcf_lim = ResD.Fat_WebD.dVcr;
	pCsfcShear->nOK      = (ResD.Fat_WebD.dvcf_rat<1.0+m_dZero) ? 1 : 0;
	pCsfcShear->dRatio   = dRatio;
}

void CDgnCsgDataCtrlAASHTO12::ConvertCsgCscrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData)
{
	if (ChkRes.pAASHTO == NULL) return;
	if (rData.pCscrB == NULL) return;
	CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D &ResD = ChkRes.pAASHTO->FatigueD;
	T_CSCR_BASE *pCscr = rData.pCscrB;

	pCscr->dVst    = ResD.Fat_WebD.dVf_sc;
	pCscr->df_fat   = ResD.Fat_LoadInduceD.df_fat; // Top flange 응력 범위
	pCscr->df_fat_B = ResD.Fat_LoadInduceD.df_fat_B; // Bottom flange 응력 범위

	//
	CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D &SconResAASHTO = ChkRes.pAASHTO->ShearConnD;
	double dVst = 0.0;
	if(LcomResD.aChkFor.GetSize() >= 12)
	{
		dVst = fabs(LcomResD.aChkFor[4].ForLC.dForce[2] - LcomResD.aChkFor[5].ForLC.dForce[2]); // 계산 
	}
	else if(LcomResD.aChkFor.GetSize() == 1)
	{
		dVst = fabs(LcomResD.aChkFor[0].ForLC.dForce[2]); // 계산 
	}
	else ASSERT(0);

	T_CSCR_BASE *pCscrB = rData.pCscrB;

	pCscrB->dVst = dVst;

	SconResAASHTO.SC_PitchD.dVst = dVst;
	SconResAASHTO.SC_PitchD.dSig = pCscrB->df_fat_B; // bottom flange 응력 범위

}

void CDgnCsgDataCtrlAASHTO12::ConvertCsgCscrMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData)
{
	if (MembR.pAASHTO == NULL) return;
	if (rData.pCscrB == NULL) return;
	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CSCR_BASE *pCscr = rData.pCscrB;

	const CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D &SCmembR = MembR.pAASHTO->MembR[nChkPos].ShearConnD;
	pCscr->dH_D     = SCmembR.SC_TypeD.dDTR;
	pCscr->dH_D_lim = 4.0;
	pCscr->dp       = SCmembR.SC_PitchD.dp_use;
	pCscr->dp_lim1  = SCmembR.SC_PitchD.dAllowPitch;
	pCscr->dTVs     = SCmembR.SC_TRSD.dS;
	pCscr->dp_lim2  = SCmembR.SC_TRSD.dplim;
	pCscr->dEdge    = SCmembR.SC_TRSD.ddist;
	pCscr->dp_lim3  = 1.0;
	pCscr->dCover   = SCmembR.SC_CoverD.dCover;
	pCscr->dPenet   = SCmembR.SC_CoverD.dPenetrate;
	pCscr->dN       = (double)SCmembR.SC_PitchD.nNum;
	pCscr->dNreq    = ceil(SCmembR.SC_STRD.dn); 
	pCscr->nOK      = SCmembR.nOK;
	pCscr->dSCrat   = dRatio_p;
	pCscr->dGammaDelF  = SCmembR.SC_FatiD.Fat_LoadInduceD.dGamma_f_bot;
	pCscr->dpDelF_n    = SCmembR.SC_FatiD.Fat_LoadInduceD.dFn; 
	pCscr->bIsFatiChkNeed  = SCmembR.SC_FatiD.Fat_LoadInduceD.bIsCheckNeed_top;

	if(SCmembR.SC_TypeD.dH < m_dZero && SCmembR.SC_TypeD.dd < m_dZero && SCmembR.SC_PitchD.dp_use < m_dZero)
	{
		// 전단연결재 입력값이 없을 때 (-)처리
		pCscr->nOK       = 1;
		pCscr->bChk      = FALSE;
	}
}

void CDgnCsgDataCtrlAASHTO12::ConvertCsgCsslMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData)
{
	if (MembR.pAASHTO == NULL) return;
	if (rData.pCsslB == NULL) return;
    if (LcomResD.aChkFor.GetSize() == 0)  return;
	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_CSSL_BASE *pCssl = rData.pCsslB;

    memcpy(pCssl->ComFor.ForLT.dForce, LcomResD.aChkFor[0].ForLT.dForce, sizeof(pCssl->ComFor.ForLT.dForce));
    memcpy(pCssl->ComFor.ForNC.dForce, LcomResD.aChkFor[0].ForNC.dForce, sizeof(pCssl->ComFor.ForNC.dForce));
    memcpy(pCssl->ComFor.ForST.dForce, LcomResD.aChkFor[0].ForST.dForce, sizeof(pCssl->ComFor.ForST.dForce));
    memcpy(pCssl->ComFor.ForLC.dForce, LcomResD.aChkFor[0].ForLC.dForce, sizeof(pCssl->ComFor.ForLC.dForce));

    CSG_MEMB_RES_AASHTO_LRFD_LSTIFF_D& HS_D = MembR.pAASHTO->MembR[nChkPos].HS_D;
	CSG_MEMB_RES_AASHTO_LRFD_HS_LS_D &HS_W = HS_D.WebD;
    CSG_MEMB_RES_AASHTO_LRFD_HS_LS_D &HS_F = HS_D.FlgD;
    pCssl->bIsWeb  = HS_W.bChk;
	pCssl->dbl     = HS_W.dbl;
	pCssl->dbl_lim = HS_W.dbl_lim;    
	pCssl->dI      = HS_W.dIl;
	pCssl->dI_lim  = HS_W.dIl_lim;
	pCssl->dr      = HS_W.dr;
	pCssl->dr_lim  = HS_W.dr_lim;

	pCssl->dfs       = HS_W.dfs;
	pCssl->dphiRhFys = HS_W.dpfn;
    //
    pCssl->bIsFlg    = HS_F.bChk;
    pCssl->dbl_f     = HS_F.dbl;
    pCssl->dbl_lim_f = HS_F.dbl_lim;    
    pCssl->dI_f      = HS_F.dIl;
    pCssl->dI_lim_f  = HS_F.dIl_lim;

	pCssl->dSFrat  = HS_D.dRatio;
	pCssl->nOK     = HS_D.bOK ? 1 : 0;

	if( !HS_D.bChk )
	{ // 수평보강재 입력값이 없을 때 (-)처리
		pCssl->nOK       = 1;
		pCssl->bChk      = FALSE;
	}
}

void CDgnCsgDataCtrlAASHTO12::ConvertCsgBstfMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData)
{
	if (MembR.pAASHTO == NULL) return;
	if (rData.pBstfB == NULL) return;
	int nChkPos = LcomResD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return;

	T_BSTF_BASE *pBstf = rData.pBstfB;

	CSG_MEMB_RES_AASHTO_LRFD_BEARING_STIFFENER_D &BS_D = MembR.pAASHTO->MembR[nChkPos].BS_D;

	pBstf->dbt     = BS_D.dWidth_pj;
	pBstf->dbt_lim = BS_D.dWidth_al;    
	pBstf->dR      = BS_D.dReact;
	pBstf->dBr     = BS_D.dPhiRbn;
	pBstf->dPr     = BS_D.dPr;

	pBstf->nOK       = (BS_D.dRatio<=1.0)? 1 : 0;
	pBstf->bChk      =TRUE;  // 여기 들어오면 반력 검토됨.
}

BOOL CDgnCsgDataCtrlAASHTO12::IsChkFatigue2(int nFatiLcomType, CSG_CHECK_RESULT_CODES &ChkRes)
{
	if (ChkRes.pAASHTO == NULL) return FALSE;
	// AASHTO-LRFD12, 6.6.1.2.1
	const CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D &FatiResAASHTO = ChkRes.pAASHTO->FatigueD;
	BOOL bChkInducedLoad = FatiResAASHTO.Fat_LoadInduceD.bChkLoadInducedFati;

	if (nFatiLcomType==2 && bChkInducedLoad==TRUE && FatiResAASHTO.Fat_LoadInduceD.bChkFatigueII==TRUE)
		return TRUE;

	return FALSE;
}

BOOL CDgnCsgDataCtrlAASHTO12::IsChkFatigueCr(int nFatiLcomType, int nFatigueType2)
{
	if (nFatigueType2>1 && nFatiLcomType==1) return FALSE;
	return TRUE;
}

BOOL CDgnCsgDataCtrlAASHTO12::IsSkipChkScon(int nFatiLcomType, CSG_CHECK_RESULT_CODES &ChkRes)
{
	if (ChkRes.pAASHTO == NULL) return FALSE;
	CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D &SconResAASHTO = ChkRes.pAASHTO->ShearConnD;
	if(SconResAASHTO.SC_TypeD.bChannel)
	{
		if(SconResAASHTO.SC_PitchD.dADTT_SL < 1850 && nFatiLcomType==1) return TRUE;
		if(SconResAASHTO.SC_PitchD.dADTT_SL - cDGN_Zero > 1850 && nFatiLcomType==2) return TRUE;
	}
	else
	{
		if(SconResAASHTO.SC_PitchD.dADTT_SL < 960 && nFatiLcomType==1) return TRUE;
		if(SconResAASHTO.SC_PitchD.dADTT_SL - cDGN_Zero > 960 && nFatiLcomType==2) return TRUE;
	}
	return FALSE;
}

BOOL CDgnCsgDataCtrlAASHTO12::IsFatigue1(BOOL bFatigueI)
{
	return bFatigueI;
}

BOOL CDgnCsgDataCtrlAASHTO12::IsShearConnectorFatigue2(CSG_CHECK_RESULT_CODES &ChkRes)
{
    if ( ChkRes.pAASHTO == NULL ) return FALSE;
    return ChkRes.pAASHTO->ShearConnD.SC_FatiD.Fat_LoadInduceD.bChkFatigueII;
}


//////////////////////////////////////////////////////////////////////////

CDgnCsgDataCtrlAASHTO07::CDgnCsgDataCtrlAASHTO07() : CDgnCsgDataCtrlAASHTO12()
{ 
}

CDgnCsgDataCtrlAASHTO07::~CDgnCsgDataCtrlAASHTO07()
{
}

BOOL CDgnCsgDataCtrlAASHTO07::IsFatigue1(BOOL bFatigueI)
{
	return FALSE;
}
