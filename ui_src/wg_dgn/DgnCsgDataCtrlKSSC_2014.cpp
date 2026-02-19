
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "DgnCsgDataCtrlKSSC_2014.h"
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

CDgnCsgDataCtrlKSSC_2014::CDgnCsgDataCtrlKSSC_2014() : CDgnCsgDataCtrlAASHTO12()
{ 
}

CDgnCsgDataCtrlKSSC_2014::~CDgnCsgDataCtrlKSSC_2014()
{
}

void CDgnCsgDataCtrlKSSC_2014::SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl)
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

BOOL CDgnCsgDataCtrlKSSC_2014::MakeMemberResultData(T_ELEM_K ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RESULT_D &rData)
{
	if (rData.pAASHTO==NULL) return FALSE;

	BOOL bCurved =  (CsgMembD.Memb.Csgd.nConsiderPv==0)? FALSE : TRUE; // 0:Á÷¼±, 1:°î¼±

	m_pCsgManager->Check_MemberResult(CsgMembD, rData);

	CsgMembD.Memb.ChIf.nChkPos = 1;
	CsgMembD.Memb.ChIf.bCurved = bCurved;
	m_pCsgManager->Check_MemberResult(CsgMembD, rData);

	if ( m_nDgnCode == KSSC_2014_CSG )
	{
		CString strCurveBridgeWarning;
		if ( CsgMembD.Memb.PosD[0].Cpcb.dRadius < cDGN_Zero )
		{
			CString strElem;
			strElem.Format(_T("%d"), ElemK);
			strCurveBridgeWarning.Format(_LS(IDS_DGN_CPG_CURVE_RADIRUS_ZERO_WARNING), strElem, _LS(IDS_DGN_CPG_I));
			GSaveHistoryFormatNF(strCurveBridgeWarning);
		}
		if ( CsgMembD.Memb.PosD[1].Cpcb.dRadius < cDGN_Zero )
		{
			CString strElem;
			strElem.Format(_T("%d"), ElemK);
			strCurveBridgeWarning.Format(_LS(IDS_DGN_CPG_CURVE_RADIRUS_ZERO_WARNING), strElem, _LS(IDS_DGN_CPG_J));
			GSaveHistoryFormatNF(strCurveBridgeWarning);
		}
	}

	return TRUE;
}

BOOL CDgnCsgDataCtrlKSSC_2014::Check_Constructibility(T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;
	if (InD.aChkFor.GetSize()>0) InD.aChkFor[0].nLcomType = 3;
	BOOL bCheck = m_pCsgManager->Check_ConstructibilityRes(InD, MembRes, ChkRes);
	DtrR.dDTR_F = ChkRes.pAASHTO->ConsD.FlexD.dRatio;
	DtrR.dDTR_S = ChkRes.pAASHTO->ConsD.ShearD.dRatio;

	return bCheck;
}

BOOL CDgnCsgDataCtrlKSSC_2014::Check_Strength(T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;
	BOOL bCheck = m_pCsgManager->Check_StrengthRes(InD, MembRes, ChkRes);

	double dMuy = InD.aChkFor[0].ForLC.dForce[4];
	BOOL bPositive = (dMuy>=0.0);

	DtrR.dDTR_F = GetAASHTOFlexureRatio(ChkRes.pAASHTO->StrengthD, bPositive);
	DtrR.dDTR_S = ChkRes.pAASHTO->StrengthD.ShearD.dRatio;

	return bCheck;
}

BOOL CDgnCsgDataCtrlKSSC_2014::Check_Service(T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;
	BOOL bCheck = m_pCsgManager->Check_ServiceRes(InD, MembRes, ChkRes);
	DtrR.dDTR_Ser = ChkRes.pAASHTO->ServiceD.dRatio;
	return bCheck;
}

BOOL CDgnCsgDataCtrlKSSC_2014::Check_Fatigue(T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR, CSG_FATI_CR_RSLT &FatiR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;
	BOOL bCheck = m_pCsgManager->Check_FatigueRes(InD, MembRes, ChkRes);
	DtrR.dDTR_FL[0] = ChkRes.pAASHTO->FatigueD.Fat_LoadInduceD.df_fat_rat;
	DtrR.dDTR_FL[1] = ChkRes.pAASHTO->FatigueD.Fat_LoadInduceD.df_fat_rat_B;
	//
	DtrR.dDTR_F_WebShr = ChkRes.pAASHTO->FatigueD.Fat_WebD.dvcf_rat;

    FatiR.bChkFatigueII_IJ[nPos] = ChkRes.pAASHTO->FatigueD.Fat_LoadInduceD.bChkLoadInducedFati;

	return bCheck;
}

BOOL CDgnCsgDataCtrlKSSC_2014::Check_ShearConnector(T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;
	BOOL bCheck = m_pCsgManager->Check_ShearConnRes(InD, MembRes, ChkRes);
	DtrR.dDTR_F_SC_f = ChkRes.pAASHTO->ShearConnD.SC_FatiD.Fat_LoadInduceD.df_fat_rat;
    DtrR.dDTR_F_SC_p = ChkRes.pAASHTO->ShearConnD.SC_PitchD.dRatio;

	return bCheck;
}

BOOL CDgnCsgDataCtrlKSSC_2014::Check_Stiffener(T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;
	BOOL bCheck = m_pCsgManager->Check_StiffenerRes(InD, MembRes, ChkRes);
	DtrR.dDTR_Sf    = ChkRes.pAASHTO->HS_D.dRatio;
	DtrR.dDTR_Sf_fs = ChkRes.pAASHTO->HS_D.WebD.dRatiofs;

	return bCheck;
}

// void CDgnCsgDataCtrlKSSC_2014::ClassNewMembResult(CSG_MEMB_RESULT_D &MembR)
// {
//   ASSERT(0);
// }
// 
// void CDgnCsgDataCtrlKSSC_2014::ClassNewDgnResult(CSG_CHECK_RESULT_CODES &ChkResD)
// {
//   ASSERT(0);
// 
// }
// 
// void CDgnCsgDataCtrlKSSC_2014::InitMembResult(CSG_MEMB_RESULT_D &MembR)
// {
//   MembR.pLSD->Initialize();
// }
// 
// void CDgnCsgDataCtrlKSSC_2014::InitDgnResult(CSG_CHECK_RESULT_CODES &ChkResD)
// {
//   ChkResD.pLSD->Initialize();
// }


BOOL CDgnCsgDataCtrlKSSC_2014::IsChkFatigue2(int nFatiLcomType, CSG_CHECK_RESULT_CODES &ChkRes)
{
	if (ChkRes.pAASHTO==NULL) return FALSE;
	// AASHTO-LRFD12, 6.6.1.2.1
	const CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D &FatiResAASHTO = ChkRes.pAASHTO->FatigueD;
	BOOL bChkInducedLoad = FatiResAASHTO.Fat_LoadInduceD.bChkLoadInducedFati;

	if (bChkInducedLoad==TRUE) return TRUE;

	return FALSE;
}

BOOL CDgnCsgDataCtrlKSSC_2014::IsShearConnectorFatigue2(CSG_CHECK_RESULT_CODES &ChkRes)
{
    return TRUE;
}

BOOL CDgnCsgDataCtrlKSSC_2014::IsSkipChkScon(int nFatiLcomType, CSG_CHECK_RESULT_CODES &ChkRes)
{
	return FALSE;
}

BOOL CDgnCsgDataCtrlKSSC_2014::IsBeforeSgldDL()
{
	return FALSE;
}

BOOL CDgnCsgDataCtrlKSSC_2014::IsAppErLoad()
{
	return FALSE;
}
