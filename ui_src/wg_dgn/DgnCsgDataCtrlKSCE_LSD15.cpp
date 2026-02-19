
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "DgnCsgDataCtrlKSCE_LSD15.h"
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

CDgnCsgDataCtrlKSCE_LSD15::CDgnCsgDataCtrlKSCE_LSD15() : CDgnCsgDataCtrl()
{ 
}

CDgnCsgDataCtrlKSCE_LSD15::~CDgnCsgDataCtrlKSCE_LSD15()
{
}

void CDgnCsgDataCtrlKSCE_LSD15::SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl)
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

BOOL CDgnCsgDataCtrlKSCE_LSD15::MakeMemberResultData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RESULT_D &rData)
{
	if (rData.pLSD==NULL) return FALSE;

	m_pCsgManager->Check_MemberResult(CsgMembD, rData);

	CsgMembD.Memb.ChIf.nChkPos = 1;
	m_pCsgManager->Check_MemberResult(CsgMembD, rData);

	return TRUE;
}

BOOL CDgnCsgDataCtrlKSCE_LSD15::Check_Constructibility(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pLSD==NULL) return FALSE;
	if (ChkRes.pLSD==NULL) return FALSE;
	if (InD.aChkFor.GetSize()>0) InD.aChkFor[0].nLcomType = 0;
	BOOL bCheck = m_pCsgManager->Check_ConstructibilityRes(InD, MembRes, ChkRes);
	DtrR.dDTR_F = ChkRes.pLSD->ConsD.Flexure.dRatio_F;
	DtrR.dDTR_S = ChkRes.pLSD->ConsD.Shear.dRatio_S;

	return bCheck;
}

BOOL CDgnCsgDataCtrlKSCE_LSD15::Check_Strength(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pLSD==NULL) return FALSE;
	if (ChkRes.pLSD==NULL) return FALSE;
	BOOL bCheck = m_pCsgManager->Check_StrengthRes(InD, MembRes, ChkRes);
	DtrR.dDTR_F = ChkRes.pLSD->StrnD.FlexD.dRatio_F;
	DtrR.dDTR_S = ChkRes.pLSD->StrnD.ShearD.dRatio_S;
	return bCheck;
}

BOOL CDgnCsgDataCtrlKSCE_LSD15::Check_Service(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pLSD==NULL) return FALSE;
	if (ChkRes.pLSD==NULL) return FALSE;
	BOOL bCheck = m_pCsgManager->Check_ServiceRes(InD, MembRes, ChkRes);
	DtrR.dDTR_Ser = ChkRes.pLSD->ServD.dRatio;

	return bCheck;
}

BOOL CDgnCsgDataCtrlKSCE_LSD15::Check_Fatigue(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR, CSG_FATI_CR_RSLT &FatiR)
{
	if (MembRes.pLSD==NULL) return FALSE;
	if (ChkRes.pLSD==NULL) return FALSE;
	BOOL bCheck = m_pCsgManager->Check_FatigueRes(InD, MembRes, ChkRes);
	DtrR.dDTR_FL[0] = ChkRes.pLSD->FatiD.FATI_L_T.df_fat_rat;
	DtrR.dDTR_FL[1] = ChkRes.pLSD->FatiD.FATI_L_B.df_fat_rat_B;
	//
	DtrR.dDTR_F_Flex = ChkRes.pLSD->FatiD.FATI_D_FLEX.dFcf_rat;
	//
	DtrR.dDTR_F_WebShr = ChkRes.pLSD->FatiD.FATI_D_SHEAR.dvcf_rat;
	//
	DtrR.dDTR_F_SC_p = ChkRes.pLSD->FatiD.CSCR.dRatio_p;
	return bCheck;
}

void CDgnCsgDataCtrlKSCE_LSD15::GetMy4Strength(CSG_CHECK_RESULT_CODES &ChkRes, double &dMuy)
{
}


void CDgnCsgDataCtrlKSCE_LSD15::ClassNewMembResult(CSG_MEMB_RESULT_D &MembR)
{
	MembR.pLSD = new CSG_MEMBPOS_RES_KSCE_LSD12_D;
}

void CDgnCsgDataCtrlKSCE_LSD15::ClassNewDgnResult(CSG_CHECK_RESULT_CODES &ChkResD)
{
	ChkResD.pLSD = new CSG_CHECK_RES_KSCE_LSD12;
}

void CDgnCsgDataCtrlKSCE_LSD15::InitMembResult(CSG_MEMB_RESULT_D &MembR)
{
	MembR.pLSD->Initialize();
}

void CDgnCsgDataCtrlKSCE_LSD15::InitDgnResult(CSG_CHECK_RESULT_CODES &ChkResD)
{
	ChkResD.pLSD->Initialize();
}



void CDgnCsgDataCtrlKSCE_LSD15::SetCsfcLoadBase(int nType, BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nTopBot, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData)
{
	if (ChkRes.pLSD==NULL) return;
	if (rData.pCsfcB==NULL) return;
	if (nType==1) return;
	CSG_FATI_RES_KSCE_LSD12 &FatiR = ChkRes.pLSD->FatiD;

	int nfcMaxMinType = 0;
	int nftMaxMinType = 0;
	if (nTopBot==0)
	{
		nfcMaxMinType = FatiR.FATI_L_T.nLoadTopCMax;
		nftMaxMinType = FatiR.FATI_L_T.nLoadTopTMax;
	}
	else
	{
		nfcMaxMinType = FatiR.FATI_L_B.nLoadBotCMax;
		nftMaxMinType = FatiR.FATI_L_B.nLoadBotTMax;
	}

	// T_CSFC_LOAD
	T_CSFC_LOAD *pCsfcLoad;

	if      (nType==0)  pCsfcLoad = &rData.pCsfcB->LoadR[nTopBot];
	else if (nType==1)  pCsfcLoad = &rData.pCscrB->LoadR[nTopBot];
	else 
	{
		ASSERT(0);
		return ;
	}

	pCsfcLoad->bChk      = bChk;
	pCsfcLoad->nLcomK    = nDgnLcomNo;
	pCsfcLoad->nOrgLcomK = nOrgLcomNo;

	BOOL bMaxMin = Check_LcomMaxMinType(pCsfcLoad->nOrgLcomK);
	pCsfcLoad->nfcMax    = (bMaxMin)? GetLcomTypeIndex4MaxMin() : nfcMaxMinType; // nfcMaxMinType+1 : 0;
	pCsfcLoad->nftMax    = (bMaxMin)? GetLcomTypeIndex4MaxMin() : nftMaxMinType; // nftMaxMinType+1 : 0;
}

void CDgnCsgDataCtrlKSCE_LSD15::SetCsfcFlexBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData)
{
	if (ChkRes.pLSD==NULL) return;
	if (rData.pCsfcB==NULL) return;
	// T_CSFC_FLEX
	rData.pCsfcB->FlexR.bChk      = bChk;
	rData.pCsfcB->FlexR.nLcomK    = nDgnLcomNo;
	rData.pCsfcB->FlexR.nOrgLcomK = nOrgLcomNo;

	int nMaxMinType = ChkRes.pLSD->FatiD.FATI_D_FLEX.nFlexMax;

	BOOL bMaxMin = Check_LcomMaxMinType(rData.pCsfcB->FlexR.nOrgLcomK);
	rData.pCsfcB->FlexR.nMax      = (bMaxMin)? GetLcomTypeIndex4MaxMin() : nMaxMinType; //nMaxMinType+1 : 0;
}

void CDgnCsgDataCtrlKSCE_LSD15::SetCsfcShearBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData)
{
	if (ChkRes.pLSD==NULL) return;
	if (rData.pCsfcB==NULL) return;
	// T_CSFC_SHEAR
	rData.pCsfcB->ShearR.bChk      = bChk;
	rData.pCsfcB->ShearR.nLcomK    = nDgnLcomNo;
	rData.pCsfcB->ShearR.nOrgLcomK = nOrgLcomNo;

	int nMaxMinType = ChkRes.pLSD->FatiD.FATI_D_SHEAR.nWebShrMax;
	BOOL bMaxMin = Check_LcomMaxMinType(rData.pCsfcB->ShearR.nOrgLcomK);
	rData.pCsfcB->ShearR.nMax      = (bMaxMin)? GetLcomTypeIndex4MaxMin() : nMaxMinType; //nMaxMinType+1 : 0;
}



void CDgnCsgDataCtrlKSCE_LSD15::ConvertCsgScbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomD, double dRatio_F, T_CSCH_REF &rData)
{
	if (ChkRes.pLSD==NULL) return;
	if (rData.pScbrB==NULL) return;
	if (LcomD.aChkFor.GetSize() == 0)  return;

	T_SCBR_BASE *pScbr = rData.pScbrB;
	CSG_CONS_RES_KSCE_LSD12 &ConsR = ChkRes.pLSD->ConsD;

	for (int i = 0; i<D_CSG_FORCE_COMPONENT; ++i)
	{
		pScbr->CsFor.dForce[i] = LcomD.aChkFor[0].ForNC.dForce[i];
	}

	for (int i = 0; i<3/*LcomD.aUlmf.GetSize()*/; ++i)
	{
		memcpy(pScbr->UlFor.For[i].dForce, LcomD.aUlmf[0].ForD[i].dForce, sizeof(pScbr->UlFor.For[i].dForce));
		memcpy(pScbr->UlFor.CbFor[i].dForce, LcomD.aUlmf[0].CbForce[i].dForce, sizeof(pScbr->UlFor.CbFor[i].dForce));
	}

	int nSetType = LcomD.Memb.PosD[0].Sect.SectInfo.nStype;

	BOOL bChcek = FALSE;
	if     (nSetType==DGN_SECT_TYPE_COMPO_I)  bChcek = TRUE;
	else if(nSetType==DGN_SECT_TYPE_COMPO_TUB && bPositive) bChcek = TRUE;


	// 복부
	pScbr->dfcw     = ConsR.Flexure.dfcw;       
	pScbr->dfcw_lim = ConsR.Flexure.dfcw_lim;   

	// 압축 플랜지 : I, Tub(정모멘트만)
	pScbr->dfcf     = (bChcek)? ConsR.Flexure.dfc        : 0.0 ;       
	pScbr->dfcf_lim = (bChcek)? ConsR.Flexure.FlexD.dFcr : 0.0 ;       

	// 인장 플랜지 : I
	pScbr->dftf     = (nSetType==DGN_SECT_TYPE_COMPO_I)? ConsR.Flexure.dft         : 0.0 ;        
	pScbr->dftf_lim = (nSetType==DGN_SECT_TYPE_COMPO_I)?  ConsR.Flexure.FlexD.dFtr : 0.0 ;    

	BOOL Chk1 =  (fabs(pScbr->dfcw) <= pScbr->dfcw_lim);
	BOOL Chk2 =  (fabs(pScbr->dfcf) <= pScbr->dfcf_lim);
	BOOL Chk3 =  (fabs(pScbr->dftf) <= pScbr->dftf_lim);
	pScbr->nOK = (Chk1 && Chk2 && Chk3)? 1 : 0;
	pScbr->dRatio = dRatio_F;
}

void CDgnCsgDataCtrlKSCE_LSD15::ConvertCsgScvrRes(CSG_MEMB_RESULT_D &Memb, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData)
{
	if (ChkRes.pLSD==NULL) return;
	if (rData.pScvrB==NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;

	T_SCVR_BASE *pScvr = rData.pScvrB;
	CSG_CONS_RES_KSCE_LSD12 &ConsR = ChkRes.pLSD->ConsD;

	for (int i = 0; i<D_CSG_FORCE_COMPONENT; ++i)
	{
		pScvr->CsFor.dForce[i] = LcomResD.aChkFor[0].ForNC.dForce[i];
	}

	for (int i = 0; i<3/*LcomD.aUlmf.GetSize()*/; ++i)
	{
		memcpy(pScvr->UlFor.For[i].dForce, LcomResD.aUlmf[0].ForD[i].dForce, sizeof(pScvr->UlFor.For[i].dForce));
		memcpy(pScvr->UlFor.CbFor[i].dForce, LcomResD.aUlmf[0].CbForce[i].dForce, sizeof(pScvr->UlFor.CbFor[i].dForce));
	}

    pScvr->dVu = MT0(ConsR.Shear.dVui) ? ConsR.Shear.dVui : ConsR.Shear.dVu;
	pScvr->dVr = ConsR.Shear.dVr;
	pScvr->nOK = ConsR.Shear.bOK_S;
	pScvr->dRatio = dRatio_S;
}

void CDgnCsgDataCtrlKSCE_LSD15::ConvertCsgCsbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData)
{
	if (MembR.pLSD==NULL) return;
	if (ChkRes.pLSD==NULL) return;
	if (rData.pCsbrB==NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;
	if (LcomResD.Memb.ChIf.nChkPos<0 || LcomResD.Memb.ChIf.nChkPos>1) return;

	CSG_MEMB_RES_KSCE_LSD12_D &MembRes = MembR.pLSD->MembR[LcomResD.Memb.ChIf.nChkPos];
	CSG_STRN_RES_KSCE_LSD12 &StrnD = ChkRes.pLSD->StrnD;
	T_CSBR_BASE *pCsbr = rData.pCsbrB;

	memcpy(pCsbr->ComFor.ForLT.dForce, LcomResD.aChkFor[0].ForLT.dForce, sizeof(pCsbr->ComFor.ForLT.dForce));
	memcpy(pCsbr->ComFor.ForNC.dForce, LcomResD.aChkFor[0].ForNC.dForce, sizeof(pCsbr->ComFor.ForNC.dForce));
	memcpy(pCsbr->ComFor.ForST.dForce, LcomResD.aChkFor[0].ForST.dForce, sizeof(pCsbr->ComFor.ForST.dForce));
	memcpy(pCsbr->ComFor.ForLC.dForce, LcomResD.aChkFor[0].ForLC.dForce, sizeof(pCsbr->ComFor.ForLC.dForce));

	for (int i = 0; i<3/*LcomResD.aUlmf.GetSize()*/; ++i)
	{
		memcpy(pCsbr->UlForLC.For[i].dForce, LcomResD.aUlmf[0].ForD[i].dForce, sizeof(pCsbr->UlForLC.For[i].dForce));
		memcpy(pCsbr->UlForLC.CbFor[i].dForce, LcomResD.aUlmf[0].CbForce[i].dForce, sizeof(pCsbr->UlForLC.CbFor[i].dForce));
	}

	double dMuy = pCsbr->ComFor.ForLC.dForce[4];
	BOOL bPositiveM = (dMuy>=0.0);
	CSG_MEMB_RES_KSCE_LSD12_D_FLEX &FlexD = bPositiveM ? MembRes.Detail_F[0] : MembRes.Detail_F[1];

	CSG_PlasticMoment &PLMoment = bPositiveM ?  MembRes.PLMoment[0] : MembRes.PLMoment[1];

	int nErrType =  StrnD.FlexD.nErrType;
	if(nErrType<=0)
	{
		pCsbr->dMy = StrnD.FlexD.dMy_tot;  
		pCsbr->dMp = PLMoment.dMp;
		pCsbr->dMu = dMuy;
		pCsbr->dMr = StrnD.FlexD.dMr;

		double dfc =  fabs(StrnD.FlexD.dfc);
		double dft =  fabs(StrnD.FlexD.dft);
		double dFcr = StrnD.FlexD.dFcr;
		double dFtr = StrnD.FlexD.dFtr;
		double dRatioC = (dFcr==0.0)? 0.0 : dfc/dFcr;
		double dRatioT = (dFtr==0.0)? 0.0 : dft/dFtr;

		pCsbr->dFu = (dRatioC>=dRatioT)? dfc : dft; 
		pCsbr->dFr = (dRatioC>=dRatioT)? dFcr: dFtr; 

		pCsbr->dMrat = dRatio_F;
		pCsbr->nOK = StrnD.FlexD.bOK_F ? 1 : 0;
	}
	else
	{
		pCsbr->dMy = 0.0;  
		pCsbr->dMp = 0.0;  
		pCsbr->dMu = 0.0;  
		pCsbr->dMr = 0.0;  
		pCsbr->dFu = 0.0;  
		pCsbr->dFr = 0.0;  
		pCsbr->dMrat = 0.0;  
		pCsbr->nOK = 0;
	}

}

void CDgnCsgDataCtrlKSCE_LSD15::ConvertCsgCsvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData)
{
	if (MembR.pLSD==NULL) return;
	if (ChkRes.pLSD==NULL) return;
	if (rData.pCsvrB==NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;
	if (LcomResD.Memb.ChIf.nChkPos<0 || LcomResD.Memb.ChIf.nChkPos>1) return;
	
	CSG_MEMB_RES_KSCE_LSD12_D &MembRes = MembR.pLSD->MembR[LcomResD.Memb.ChIf.nChkPos];
	CSG_STRN_RES_KSCE_LSD12 &StrnD = ChkRes.pLSD->StrnD;
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

	CSG_CUMS_D &ShrR = MembRes.CumsD;
	CSG_STRN_RES_KSCE_LSD12_SHEAR &ShearD = StrnD.ShearD;
	pCsvr->dVu      = ShearD.dVu; //rData.ComFor.ForLT.dForce[4];      
	pCsvr->dVr      = ShearD.dVr;
	pCsvr->ddo_lim  = (ShrR.bChkShr_do)? ShrR.ddo_lim : 0.0;  
	pCsvr->ddo      = ShrR.ddo;      
	pCsvr->dbt_lim1 = ShrR.dbt_lim01; 
	pCsvr->dbt_lim2 = ShrR.dbt_lim02; 
	pCsvr->dbt_lim3 = ShrR.dbt_lim03; 
	pCsvr->dbt      = ShrR.dbt;      
	pCsvr->dIt_lim  = ShrR.dIt_lim;  
	pCsvr->dIt      = ShrR.dIt;      
	pCsvr->dAs_lim  = ShearD.dAs_lim;  
	pCsvr->dAs      = ShearD.dAs;  

	BOOL bChk_do = ShrR.bCheck_do;  //(rData.ddo_lim >=rData.ddo ? TRUE : FALSE);   
	BOOL bChk_bt;
	if(pCsvr->dbt >=pCsvr->dbt_lim1)
	{
		if( pCsvr->dbt_lim2 >= pCsvr->dbt  && pCsvr->dbt >=pCsvr->dbt_lim3)
		{
			bChk_bt = TRUE;
		}
		else
		{
			bChk_bt = FALSE;
		}
	}
	else
	{
		bChk_bt = FALSE;
	}
	BOOL bChk_It = (pCsvr->dIt_lim <=pCsvr->dIt ? TRUE : FALSE);   // OK
	BOOL bChk_As = (pCsvr->dAs_lim <=pCsvr->dAs ? TRUE : FALSE);   // OK

	pCsvr->bChk_do = bChk_do;
	pCsvr->bChk_bt = bChk_bt;
	pCsvr->bChk_It = bChk_It;
	pCsvr->bChk_As = bChk_As;
	pCsvr->dVrat   = dRatio_S;

	pCsvr->nOK      = ShearD.bOK_S ? 1 : 0;
	pCsvr->nOK      = (bChk_do && bChk_bt && bChk_It && bChk_As) ? 1 : 0;
}

void CDgnCsgDataCtrlKSCE_LSD15::ConvertCsgCsacRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_Ser, T_CSCH_REF &rData)
{
	if (MembR.pLSD==NULL) return;
	if (ChkRes.pLSD==NULL) return;
	if (rData.pCsacB==NULL) return;
	if (LcomResD.aChkFor.GetSize() == 0)  return;
	if (LcomResD.Memb.ChIf.nChkPos<0 || LcomResD.Memb.ChIf.nChkPos>1) return;

	CSG_MEMB_RES_KSCE_LSD12_D &MembRes = MembR.pLSD->MembR[LcomResD.Memb.ChIf.nChkPos];
	CSG_SERV_RES_KSCE_LSD12 &ServR = ChkRes.pLSD->ServD;
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

	double dMuy = pCsac->ComFor.ForLC.dForce[4];
	int nIndex = (dMuy>=0.0)? 0 : 1;
	pCsac->dfcw     = ServR.dfcw;       
	pCsac->dfcw_lim = ServR.dfcw_lim;   
	pCsac->dfcf     = ServR.dfc;       
	pCsac->dfcf_lim = MembRes.Detail_Serv[nIndex].d095Fy_c;  
	pCsac->dftf     = ServR.dft;       
	pCsac->dftf_lim = MembRes.Detail_Serv[nIndex].d095Fy_t;  
	pCsac->nOK      = ServR.bOK ? 1 : 0;
	pCsac->dfrat = dRatio_Ser;
}

void CDgnCsgDataCtrlKSCE_LSD15::ConvertCsgFatiLoadRes(int nType, int nPart, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData)
{
	if (nType==1) return;
	if (MembR.pLSD==NULL) return;
	if (ChkRes.pLSD==NULL) return;
	if (rData.pCsfcB == NULL) return;
	if (rData.pCscrB == NULL) return;
	int nChkForSize = LcomResD.aChkFor.GetSize();
	if (LcomResD.Memb.ChIf.nChkPos<0 || LcomResD.Memb.ChIf.nChkPos>1) return;

	CSG_MEMB_RES_KSCE_LSD12_D &MembRes = MembR.pLSD->MembR[LcomResD.Memb.ChIf.nChkPos];
	T_CSFC_LOAD *pCsfcLoad;

	if      (nType==0)  pCsfcLoad = &rData.pCsfcB->LoadR[nPart];
	else if (nType==1)  pCsfcLoad = &rData.pCscrB->LoadR[nPart];
	else 
	{
		ASSERT(0);
		return ;
	}

	if (nPart==0) // Top flange
	{
		CSG_FATI_RES_KSCE_LSD12_LOAD_INDUCE_TOP &FATI= ChkRes.pLSD->FatiD.FATI_L_T;
		pCsfcLoad->ComForMV.nLcomSize = nChkForSize;
		pCsfcLoad->nForId[0] = FATI.nLoadTopCMax;
		pCsfcLoad->nForId[1] = FATI.nLoadTopTMax;
		for (int i=0; i<nChkForSize; ++i)
		{      
			memcpy(pCsfcLoad->ComForMV.ForMV[i].ForLT.dForce, LcomResD.aChkFor[i].ForLT.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForLT.dForce));
			memcpy(pCsfcLoad->ComForMV.ForMV[i].ForNC.dForce, LcomResD.aChkFor[i].ForNC.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForNC.dForce));
			memcpy(pCsfcLoad->ComForMV.ForMV[i].ForST.dForce, LcomResD.aChkFor[i].ForST.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForST.dForce));
			memcpy(pCsfcLoad->ComForMV.ForMV[i].ForLC.dForce, LcomResD.aChkFor[i].ForLC.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForLC.dForce));
		}

		//     if (FATI.nLoadTopCMax<nChkForSize)
		//     {
		//       memcpy(rData.ComFor[0].ForLT.dForce, LcomResD.aChkFor[FATI.nLoadTopCMax].ForLT.dForce, sizeof(rData.ComFor[0].ForLT.dForce));
		//       memcpy(rData.ComFor[0].ForNC.dForce, LcomResD.aChkFor[FATI.nLoadTopCMax].ForNC.dForce, sizeof(rData.ComFor[0].ForNC.dForce));
		//       memcpy(rData.ComFor[0].ForST.dForce, LcomResD.aChkFor[FATI.nLoadTopCMax].ForST.dForce, sizeof(rData.ComFor[0].ForST.dForce));
		//       memcpy(rData.ComFor[0].ForLC.dForce, LcomResD.aChkFor[FATI.nLoadTopCMax].ForLC.dForce, sizeof(rData.ComFor[0].ForLC.dForce));
		//     }
		//     if (FATI.nLoadTopTMax<nChkForSize)
		//     {
		//       memcpy(rData.ComFor[1].ForLT.dForce, LcomResD.aChkFor[FATI.nLoadTopTMax].ForLT.dForce, sizeof(rData.ComFor[1].ForLT.dForce));
		//       memcpy(rData.ComFor[1].ForNC.dForce, LcomResD.aChkFor[FATI.nLoadTopTMax].ForNC.dForce, sizeof(rData.ComFor[1].ForNC.dForce));
		//       memcpy(rData.ComFor[1].ForST.dForce, LcomResD.aChkFor[FATI.nLoadTopTMax].ForST.dForce, sizeof(rData.ComFor[1].ForST.dForce));
		//       memcpy(rData.ComFor[1].ForLC.dForce, LcomResD.aChkFor[FATI.nLoadTopTMax].ForLC.dForce, sizeof(rData.ComFor[1].ForLC.dForce));
		//     }

		pCsfcLoad->dfc        = FATI.dft_c;
		pCsfcLoad->dft        = FATI.dft_t;
		pCsfcLoad->dGammaDelF = FATI.df_fat;     
		pCsfcLoad->dDelF_n    = MembRes.Detail_Fati.dDelta_Fn;
		pCsfcLoad->nOK        = (FATI.bCheck_fat) ? 1 : 0;
		pCsfcLoad->dRatio     = dRatio;
	}
	else  // Bottom flange
	{  
		CSG_FATI_RES_KSCE_LSD12_LOAD_INDUCE_BOT &FATI= ChkRes.pLSD->FatiD.FATI_L_B;

		pCsfcLoad->ComForMV.nLcomSize = nChkForSize;
		pCsfcLoad->nForId[0] = FATI.nLoadBotCMax;
		pCsfcLoad->nForId[1] = FATI.nLoadBotTMax;
		for (int i=0; i<nChkForSize; ++i)
		{      
			memcpy(pCsfcLoad->ComForMV.ForMV[i].ForLT.dForce, LcomResD.aChkFor[i].ForLT.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForLT.dForce));
			memcpy(pCsfcLoad->ComForMV.ForMV[i].ForNC.dForce, LcomResD.aChkFor[i].ForNC.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForNC.dForce));
			memcpy(pCsfcLoad->ComForMV.ForMV[i].ForST.dForce, LcomResD.aChkFor[i].ForST.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForST.dForce));
			memcpy(pCsfcLoad->ComForMV.ForMV[i].ForLC.dForce, LcomResD.aChkFor[i].ForLC.dForce, sizeof(pCsfcLoad->ComForMV.ForMV[i].ForLC.dForce));
		}

		pCsfcLoad->dfc        = FATI.dfb_c;
		pCsfcLoad->dft        = FATI.dfb_t;
		pCsfcLoad->dGammaDelF = FATI.df_fat_B;     
		pCsfcLoad->dDelF_n    = MembRes.Detail_Fati.dDelta_Fn;
		pCsfcLoad->nOK        = (FATI.bCheck_fat_B) ? 1 : 0;
		pCsfcLoad->dRatio     = dRatio;
	}
}

void CDgnCsgDataCtrlKSCE_LSD15::ConvertCsgFatiFlexRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData)
{
	if (MembR.pLSD==NULL) return;
	if (ChkRes.pLSD==NULL) return;
	if (rData.pCsfcB==NULL) return;
	CSG_FATI_RES_KSCE_LSD12_DEFORM_INDUCED_FLEX &FATI= ChkRes.pLSD->FatiD.FATI_D_FLEX;
	T_CSFC_FLEX *pCsfcFlex = &rData.pCsfcB->FlexR;

	memcpy(pCsfcFlex->ComFor.ForLT.dForce, LcomResD.aChkFor[FATI.nFlexMax].ForLT.dForce, sizeof(pCsfcFlex->ComFor.ForLT.dForce));
	memcpy(pCsfcFlex->ComFor.ForNC.dForce, LcomResD.aChkFor[FATI.nFlexMax].ForNC.dForce, sizeof(pCsfcFlex->ComFor.ForNC.dForce));
	memcpy(pCsfcFlex->ComFor.ForST.dForce, LcomResD.aChkFor[FATI.nFlexMax].ForST.dForce, sizeof(pCsfcFlex->ComFor.ForST.dForce));
	memcpy(pCsfcFlex->ComFor.ForLC.dForce, LcomResD.aChkFor[FATI.nFlexMax].ForLC.dForce, sizeof(pCsfcFlex->ComFor.ForLC.dForce));

	pCsfcFlex->nMax = FATI.nFlexMax;
	pCsfcFlex->dfcf = FATI.dFcf;
	pCsfcFlex->dFyw = FATI.dFr;
	pCsfcFlex->nOK  = (FATI.dFcf_rat<1.0+m_dZero) ? 1 : 0;
	pCsfcFlex->dRatio = dRatio;

}

void CDgnCsgDataCtrlKSCE_LSD15::ConvertCsgFatiShearRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData)
{
	if (ChkRes.pLSD==NULL) return;
	if (rData.pCsfcB==NULL) return;
	CSG_FATI_RES_KSCE_LSD12_DEFORM_INDUCED_SHEAR &FATI= ChkRes.pLSD->FatiD.FATI_D_SHEAR;
	T_CSFC_SHEAR *pCsfcShear = &rData.pCsfcB->ShearR;

	memcpy(pCsfcShear->ComFor.ForLT.dForce, LcomResD.aChkFor[FATI.nWebShrMax].ForLT.dForce, sizeof(pCsfcShear->ComFor.ForLT.dForce));
	memcpy(pCsfcShear->ComFor.ForNC.dForce, LcomResD.aChkFor[FATI.nWebShrMax].ForNC.dForce, sizeof(pCsfcShear->ComFor.ForNC.dForce));
	memcpy(pCsfcShear->ComFor.ForST.dForce, LcomResD.aChkFor[FATI.nWebShrMax].ForST.dForce, sizeof(pCsfcShear->ComFor.ForST.dForce));
	memcpy(pCsfcShear->ComFor.ForLC.dForce, LcomResD.aChkFor[FATI.nWebShrMax].ForLC.dForce, sizeof(pCsfcShear->ComFor.ForLC.dForce));

	pCsfcShear->nMax     = FATI.nWebShrMax;
	pCsfcShear->dVcf     = FATI.dVcf;
	pCsfcShear->dVcf_lim = FATI.dVr;
	pCsfcShear->nOK      = (FATI.dvcf_rat<1.0+m_dZero) ? 1 : 0;
	pCsfcShear->dRatio   = dRatio;
}

void CDgnCsgDataCtrlKSCE_LSD15::ConvertCsgCscrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData) 
{
	if (ChkRes.pLSD==NULL) return;
	if (rData.pCscrB==NULL) return;
	CSG_FATI_RES_KSCE_LSD12 &FatiR = ChkRes.pLSD->FatiD;
	T_CSCR_BASE *pCscr = rData.pCscrB;
	
	pCscr->dp_lim1 = FatiR.CSCR.dpuse_lim0;
	pCscr->dVst    = FatiR.CSCR.dVst;
	pCscr->nOK     = (FatiR.CSCR.bOK) ? 1 : 0;
	pCscr->dSCrat = dRatio_p;

}

void CDgnCsgDataCtrlKSCE_LSD15::ConvertCsgCscrMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData)
{
	if (MembR.pLSD==NULL) return;
	if (rData.pCscrB==NULL) return;
	int nPos = LcomResD.Memb.ChIf.nChkPos;
	if (nPos<0 || nPos>1) return;

	CSG_CPSC_POSD &SconD = LcomResD.Memb.PosD[nPos].Scon;

	T_CSCR_BASE *pCscr = rData.pCscrB;

	CSG_MEMB_RES_KSCE_LSD12_D &MembRes = MembR.pLSD->MembR[nPos];  

	const CSG_MEMB_RES_KSCE_LSD12_D_CSCR &SCmembR = MembRes.Detail_CSCR;
	pCscr->dH_D     = SCmembR.dH_d;
	pCscr->dH_D_lim = SCmembR.dH_d_lim;
	pCscr->dp       = SconD.dPitch;

	pCscr->dp_lim2  = SCmembR.dpuse_lim1;
	pCscr->dp_lim3  = SCmembR.dpuse_lim2; 
	pCscr->dN       = (double)SconD.nNum;
	pCscr->dNreq    = max(SCmembR.dN_req_po, SCmembR.dN_req_ne);
	BOOL bOK01  = pCscr->nOK;
	pCscr->nOK   = bOK01;//(bOK01 && pCscr->dN>=pCscr->dNreq)? 1 : 0;

	BOOL bCheck = (SconD.nNum==0 && SconD.dPitch==0.0 && SconD.dSpace==0.0)? FALSE : TRUE;
	pCscr->bChk  = bCheck;
}

void CDgnCsgDataCtrlKSCE_LSD15::ConvertCsgCsslMembNoLcomRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSBR_D &CsbrResD, T_CSCH_REF &rData)
{
	if (MembR.pLSD==NULL) return;
	if (rData.pCsslB==NULL) return;
	int nPos = LcomResD.Memb.ChIf.nChkPos;
	if (nPos<0 || nPos>1) return;

	T_CSSL_BASE *pCssl = rData.pCsslB;

	CSG_MEMB_RES_KSCE_LSD12_D &MembRes = MembR.pLSD->MembR[nPos];

	BOOL bTop=FALSE;
	BOOL bBot=FALSE;

	if(nPos==0) //i
	{
		bTop = CsbrResD.CsbrB[0].bChk;
		bBot = CsbrResD.CsbrB[1].bChk;
	}
	else // j
	{
		bTop = CsbrResD.CsbrB[2].bChk;
		bBot = CsbrResD.CsbrB[3].bChk;
	}

	int nWeb=0;
	CSG_MEMB_RES_KSCE_LSD12_D_CSSL  &LStfnR_Top = (nWeb==0) ? MembRes.Detail_CSSL[0] : MembRes.Detail_CSSL2[0];
	CSG_MEMB_RES_KSCE_LSD12_D_CSSL  &LStfnR_Bot = (nWeb==0) ? MembRes.Detail_CSSL[1] : MembRes.Detail_CSSL2[1];

	CSG_MEMB_RES_KSCE_LSD12_D_CSSL  LStfnR;
	int nCSSLType = 0;
	if(bTop && bBot)
	{
		LStfnR    = (LStfnR_Top.ddo >= LStfnR_Bot.ddo)? LStfnR_Top : LStfnR_Bot;
		nCSSLType = (LStfnR_Top.ddo >= LStfnR_Bot.ddo)? 0 : 1;
	}
	else if(bTop)
	{
		LStfnR = LStfnR_Top;
		nCSSLType = 0;
	}
	else if(bBot)
	{
		LStfnR = LStfnR_Bot;
		nCSSLType = 1;
	}
	else ASSERT(0);


	pCssl->nMax      = nCSSLType; // 0: Top, 1: Bot
	pCssl->dbl       = LStfnR.dbl;
	pCssl->dbl_lim   = LStfnR.dbl_lim;    
	pCssl->dI        = LStfnR.dIl;
	pCssl->dI_lim    = LStfnR.dIl_lim;   
	pCssl->dr        = LStfnR.dr_to;
	pCssl->dr_lim    = LStfnR.dr_lim;

	BOOL bCKk1 = (pCssl->dbl <=pCssl->dbl_lim)? TRUE : FALSE;
	BOOL bCKk2 = (pCssl->dI  >=pCssl->dI_lim)?  TRUE : FALSE;
	BOOL bCKk3 = (pCssl->dr  >=pCssl->dr_lim)?  TRUE : FALSE;

	pCssl->nOK  = (bCKk1 && bCKk2 && bCKk3) ? 1 : 0;

	pCssl->bChk = (LStfnR.dbl==0.0 && LStfnR.dtp==0.0)? FALSE : TRUE;

}

BOOL CDgnCsgDataCtrlKSCE_LSD15::IsChkFatiTranFlex()
{
	return TRUE;
}

BOOL CDgnCsgDataCtrlKSCE_LSD15::IsSCMembChk()
{
	return TRUE;
}