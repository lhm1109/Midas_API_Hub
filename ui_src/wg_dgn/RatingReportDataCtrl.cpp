#include "stdafx.h"
#include "RatingReportDataCtrl.h"

#include "..\..\dgnengine\src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\..\dgnengine\src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"
#include "..\..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_CS457_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\AnalysisResult.h"

#include "CRCDataCtrl.h"
#include "DgnDataCtrl.h"
#include "DgnCsgDataCtrl.h"
#include "DgnStlRatingDataCtrl.h"
#include "RptSteelGirder_AASHTO12.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CRatingReportDataCtrl::CRatingReportDataCtrl(CCRCDataCtrl* pDataCtrl, int nDgnCode)
{
	m_pDoc = CDBDoc::GetDocPoint();

	if (m_pDoc != NULL)
	{
		m_pAttrCtrl = m_pDoc->m_pAttrCtrl;
		m_pAttrCtrl2 = m_pDoc->m_pAttrCtrl2;
		m_pPostCtrl = m_pDoc->m_pPostCtrl;
		m_pDgnResult = m_pPostCtrl->GetAnalysisResult();
	}

	m_pDataCtrl = pDataCtrl;
	m_nDgnCode = nDgnCode;
}

CRatingReportDataCtrl::~CRatingReportDataCtrl()
{

}

bool CRatingReportDataCtrl::Get_CS457_DetailResult(const ArrElemPairKey& aPrtElemK, CSG_RPT_CS457_D& rData)
{
	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	ArrElemPairKey arElemK;
	if (!m_pDataCtrl->Get_ElemListForBlrDgn_STL(FALSE, arElemK))	return false;
	m_pDataCtrl->SetCheckElementData(arElemK);

	m_pDataCtrl->MakeUnbracedLengthMembData();
	m_pDataCtrl->MakeUnbracedLengthPosition();


	CRptSteelGirder_AASHTO12 AASHTORpt(m_nDgnCode);

	T_RPSS_D RpssD; 
	if (!m_pDoc->m_pAttrCtrl2->GetRpss(RpssD))
	{
		RpssD.Initialize();
	}

	pRatingCtrl->Make_RatingDgnData();

	CSG_EFF_BREADTH_RATIO_SPAN_D EffBreadthR;
	CSG_C4_VALUE_TABLE_CS457 C4Value;

	CSG_MEMB_RESULT_D      MembR;
	CSG_CHECK_RESULT_CODES ChkResD;

	switch (m_nDgnCode)
	{
	case CS457_R1_CSG:
		{
			MembR.pCS457 = new CSG_MEMBPOS_RES_CS457_D;
			ChkResD.pCS457 = new CSG_CHECK_RES_CS457;	
		}
		break;
	case NR_GN_CIV_025_06_CSG:
		{
			MembR.pNRGN = new CSG_MEMBPOS_RES_NR_GN_CIV_025_D;
			ChkResD.pNRGN = new CSG_CHECK_RES_NR_GN_CIV_025;
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	const INT_PTR nSizeElem = aPrtElemK.GetSize();
	ArrElemPairKey aUblmElemK;
	BOOL bCantilever = FALSE;	
	for (INT_PTR i = 0; i < nSizeElem; ++i)
	{
		CSG_MEMB_POSD_KSCE_LSD MembLcomD;
		MembLcomD.aChkFor.SetSize(1);
		pRatingCtrl->ConvertRatingGlobalData(RpssD, MembLcomD.Memb.Csgd);

		auto  ElemK = aPrtElemK[i];

		// 출력 옵션 셋팅
		// 1. 출력 여부 결정
		T_SRAS_D SrasD; SrasD.Initialize();  // 설계 위치
		T_SRAR_D SrarD; SrarD.Initialize();  // 출력 위치

		int nMatlK = 0;
		if (ElemK.second == EN_EL_BEAM)
		{
			if (!m_pDoc->m_pAttrCtrl2->GetSras(ElemK.first, SrasD)) continue;
			if (!m_pDoc->m_pAttrCtrl2->GetSrar(ElemK.first, SrarD)) continue;

			T_ELEM_D ElemD; ElemD.Initialize();
			if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD)) continue;
			nMatlK = ElemD.elmat;
		}
		else if (ElemK.second == EN_EL_VBEAM)
		{
			T_SRCV_D SrcvD; SrcvD.Initialize();
			if (!m_pDoc->m_pAttrCtrl2->GetSrcv(ElemK.first, SrcvD)) continue;
			SrasD=SrcvD;
			//
			T_SRRV_D SrrvD; SrrvD.Initialize();
			if (!m_pDoc->m_pAttrCtrl2->GetSrrv(ElemK.first, SrrvD)) continue;
			SrarD=SrrvD;
			
			T_VBEM_D VbemD; VbemD.Initialize();
			if (m_pDoc->m_pAttrCtrl2->GetVbem(ElemK.first, VbemD))
			{
				T_MATL_K SlabMatlK = 0, GirderMatlK = 0;
				m_pDoc->m_pAttrCtrl2->GetSecvMatlK(ElemK.first, SlabMatlK, GirderMatlK);
				nMatlK = GirderMatlK;
			}
		}
		else ASSERT(0);

		BOOL bPrtIJ[2] = { FALSE, FALSE };
		bPrtIJ[0] = ((SrasD.nPos == 0 || SrasD.nPos == 2) && (SrarD.iPrintChk == 0 || SrarD.iPrintChk == 2));
		bPrtIJ[1] = ((SrasD.nPos == 1 || SrasD.nPos == 2) && (SrarD.iPrintChk == 1 || SrarD.iPrintChk == 2));

		int nNode[2] = { 0, 0 };
		BOOL bSupprotNode[2] = { FALSE, FALSE };
		pRatingCtrl->m_pCSGDataCtrl->CheckSupprotNodeElem(ElemK, nNode);
		bSupprotNode[0] = nNode[0];
		bSupprotNode[1] = nNode[1];
		bool bInteriorSupport[2] = { false, false };
		pRatingCtrl->m_pCSGDataCtrl->CheckInteriorSupportNodeElem(ElemK, bInteriorSupport);

		if (!pRatingCtrl->m_pCSGDataCtrl->m_mCsgMatl.Lookup(nMatlK, MembLcomD.Memb.Matl)) MembLcomD.Memb.Matl.Initialize();

		double dVuFactor = 1.0;

		const auto& itSect4E = pRatingCtrl->m_pCSGDataCtrl->m_mCsgElemSect.find(ElemK);
		if (itSect4E != pRatingCtrl->m_pCSGDataCtrl->m_mCsgElemSect.end())
		{
			const auto& itSect = pRatingCtrl->m_pCSGDataCtrl->m_mCsgSect.find(itSect4E->second);
			if (itSect != pRatingCtrl->m_pCSGDataCtrl->m_mCsgSect.end())
			{
				MembLcomD.Memb.PosD[0].Sect = itSect->second.SectPos[0];
				MembLcomD.Memb.PosD[1].Sect = itSect->second.SectPos[1];
				MembLcomD.Memb.PosD[0].bSupprotNode = bSupprotNode[0];
				MembLcomD.Memb.PosD[1].bSupprotNode = bSupprotNode[1];
				MembLcomD.Memb.PosD[0].bInteriorSupport = bInteriorSupport[0];
				MembLcomD.Memb.PosD[1].bInteriorSupport = bInteriorSupport[1];
				if (itSect->second.SectPos[0].SectInfo.nStype == DGN_SECT_TYPE_COMPO_B ||
					itSect->second.SectPos[0].SectInfo.nStype == DGN_SECT_TYPE_COMPO_TUB)
					dVuFactor = 0.5;
			}
		}

		pRatingCtrl->m_pCSGDataCtrl->Get_CsgMembInData(ElemK, MembLcomD);

		T_SPAN_K CurSpanK = 0;
		ElemPairK EPairI = ElemK, EPairM = ElemK, EPairJ = ElemK;
		int nPosI = 0;
		int nPosM = 0;
		int nPosJ = 1;

		BOOL bChkUblm = FALSE;
		BOOL bCantilever = FALSE;
		BOOL bMakeMembFor = FALSE;
		BOOL b2SideBgg = FALSE;
		DgnBeamPairK nUblmK(static_cast<EN_BEAM_DGN_TYPE>(ElemK.second), 0);

		DgnBeamPairK nCurUblmK(static_cast<EN_BEAM_DGN_TYPE>(ElemK.second), 0);
		m_pDataCtrl->GetUbLengthElemInfo(ElemK, nUblmK, nCurUblmK, EPairI, EPairM, EPairJ, nPosI, nPosM, nPosJ, bChkUblm, bCantilever, bMakeMembFor, b2SideBgg, aUblmElemK);
		MembLcomD.bChkUlmf = true;

		T_KFAC_D KFacD; KFacD.Initialize();
		BOOL bGetKfac = m_pDataCtrl->GetGenKfactor(ElemK, KFacD);
		pRatingCtrl->m_pCSGDataCtrl->Get_UnbracedLengthData(ElemK, bGetKfac, KFacD, MembLcomD.Memb.Ulen);
		//
		pRatingCtrl->m_pCSGDataCtrl->Get_CsgUlmSection(EPairI, EPairM, EPairJ, nPosI, nPosM, nPosJ, MembLcomD.Memb.Ulms);

		MembLcomD.Memb.ChIf.bCantilever = bCantilever;

		T_UFMT_D UfmtD;
		if (!m_pDoc->m_pAttrCtrl->GetUfmt(ElemK.first, UfmtD))
		{
			UfmtD.Initialize();
		}

		MembLcomD.Memb.ChIf.enMembType = [](int nMembType)
		{
			switch (nMembType)
			{
			case 0: return EN_Chk_Memb_Main_Girder;
			case 1: return EN_Chk_Memb_Cross_Girder;
			case 2: return EN_Chk_Memb_Rail_Bearer;
			default: ASSERT(0); return EN_Chk_Memb_None;				
			}
		}(UfmtD.nMemberType);

		pRatingCtrl->m_pCSGDataCtrl->MakeMemberResultData(ElemK, MembLcomD, MembR);
		
		switch (m_nDgnCode)
		{
		case CS457_R1_CSG:
			{
				MembR.pCS457->MembR[0].pC4 = &C4Value;
				MembR.pCS457->MembR[1].pC4 = &C4Value;
			}
			break;
		case NR_GN_CIV_025_06_CSG:
			{
				MembR.pNRGN->MembR[0].pC4 = &C4Value;
				MembR.pNRGN->MembR[1].pC4 = &C4Value;
			}
			break;
		default:
			ASSERT(0);
			break;
		}		

		for (int j = 0; j < 2; ++j)
		{
			//PscMembD.Pos[j].CalcD.bTypeC = AsopD.bTypeC;
			//m_pDataCtrl->m_pPscDataCtrl->Get_PscDataPosi(ElemK, j, PscMembD.Pos[j], bComposite);
		}

		for (int j = 0; j < 2; ++j)
		{
			if (bPrtIJ[j])
			{
				MembLcomD.Memb.ChIf.nChkPos = j;
				CSG_RPT_CS457_POS RptResD;
				RptResD.PosD = MembLcomD.Memb;
				CSG_MEMB_RES_AASHTO_LRFD_D AASHTOMemb;
				switch (m_nDgnCode)
				{
				case CS457_R1_CSG:
					{
						AASHTOMemb.Memb.dCzp_rb = MembR.pCS457->MembR[j].Memb.dCzp_rb;
						AASHTOMemb.Memb.dCzp_rb3 = MembR.pCS457->MembR[j].Memb.dCzp_rb3;
						AASHTORpt.SetRptData(ElemK.first, j, MembLcomD.Memb, AASHTOMemb);
						MembR.pCS457->MembR[j].Memb = AASHTOMemb.Memb;
						RptResD.MembR = MembR.pCS457->MembR[j];
						for (int k = 0; k < 2; ++k)
						{
							int nPosNeg = k + 1;
							Get_CS457_DetailFlex(ElemK, j, nPosNeg, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
						}
						Get_CS457_DetailShear(ElemK, j, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
						Get_CS457_DetailCombinedMV(ElemK, j, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
						Get_CS457_DetailLongShear(TRUE, ElemK, j, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
						//GetDetailShrr(ElemK, PscMembD, j, RptResD);
						//GetDetailTorr(ElemK, PscMembD, j, RptResD);

						int nComTen = 1;
						Get_CS457_DetailFlange4Box(ElemK, j, nComTen, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
						Get_CS457_DetailFlangeStiffener4Box(ElemK, j, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
						for (int k = 0; k < 2; ++k)
						{
							int nPosNeg = k + 1;
							Get_CS457_DetailWebYieldBuckling4Box(ElemK, j, nPosNeg, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
						}

						for (int k = 0; k < 2; ++k)
						{
							nComTen = k + 1;
							Get_CS457_DetailSLS(ElemK, j, nComTen, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
						}

						Get_CS457_DetailLongShear(FALSE, ElemK, j, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
					}
					break;
				case NR_GN_CIV_025_06_CSG:
					{
						AASHTOMemb.Memb.dCzp_rb = MembR.pNRGN->MembR[j].Memb.dCzp_rb;
						AASHTOMemb.Memb.dCzp_rb3 = MembR.pNRGN->MembR[j].Memb.dCzp_rb3;
						AASHTORpt.SetRptData(ElemK.first, j, MembLcomD.Memb, AASHTOMemb);
						MembR.pNRGN->MembR[j].Memb = AASHTOMemb.Memb;
						RptResD.MembR = MembR.pNRGN->MembR[j];
						for (int k = 0; k < 2; ++k)
						{
							int nPosNeg = k + 1;
							Get_NR2006_DetailFlex(ElemK, j, nPosNeg, MembLcomD, *MembR.pNRGN, *ChkResD.pNRGN, RptResD);
						}
						Get_NR2006_DetailShear(ElemK, j, MembLcomD, *MembR.pNRGN, *ChkResD.pNRGN, RptResD);

						Get_NR2006_DetailFastener(ElemK, j, MembLcomD, *MembR.pNRGN, *ChkResD.pNRGN, RptResD);						
						
						Get_NR2006_DetailBearingStiffner(ElemK, j, enNR_TS_Check::EN_NR_TS_Web_Yielding, MembLcomD, *MembR.pNRGN, *ChkResD.pNRGN, RptResD);
						Get_NR2006_DetailBearingStiffner(ElemK, j, enNR_TS_Check::EN_NR_TS_Yielding, MembLcomD, *MembR.pNRGN, *ChkResD.pNRGN, RptResD);
						Get_NR2006_DetailBearingStiffner(ElemK, j, enNR_TS_Check::EN_NR_TS_Buckling, MembLcomD, *MembR.pNRGN, *ChkResD.pNRGN, RptResD);						
					}
					break;
				default:
					break;
				}
				

				rData.aRptD.Add(RptResD);
			}
		}
	}

	if (MembR.pCS457 != NULL)
	{
		delete MembR.pCS457;
	}

	if (ChkResD.pCS457 != NULL)
	{
		delete ChkResD.pCS457;
	}

	if (MembR.pNRGN != NULL)
	{
		delete MembR.pNRGN;
	}

	if (ChkResD.pNRGN != NULL)
	{
		delete ChkResD.pNRGN;
	}

	return true;
}

#pragma region /// for CS457
bool CRatingReportDataCtrl::Get_CS457_DetailFlex(ElemPairK ElemK, const int& nPos, const int& nPosNeg, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASFC_CSG_BS_CASE AsfcCaseD;
	int nPN = nPosNeg;
	m_pDgnResult->GetFlexuralStrengthRating4CS454CSG(ElemK, 0, nPN, 0, AsfcCaseD);
	if (nPosNeg == 1)
	{
		rData.bFlexPos = AsfcCaseD.AsfcB[nPos].ComD.bChk;
		if (!rData.bFlexPos) { return false; }
	}
	else
	{
		rData.bFlexNeg = AsfcCaseD.AsfcB[nPos].ComD.bChk;
		if (!rData.bFlexNeg) { return false; }
	}

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pCS457 = &TMembRes;

	CSG_STRN_RES_CS457 StrnD;
	CSG_CHECK_RES_CS457 TRes;
	TRes.pStrnD = &StrnD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pCS457 = &TRes;
	CString strRCase = GetRatingCaseName(AsfcCaseD.AsfcB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsfcCaseD.AsfcB[nPos].ComD.unConcurrent);

	MembLcomD.aChkFor[0].nLcomType = 0;
	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);

	ConvertCsgDgnForce(AsfcCaseD.AsfcB[nPos].ComD, MembLcomD);

	CSG_CHK_RES_DTR_PARAM DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
	TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomName = CT2W(strRCase);
	TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomType = CT2W(strRCaseSubType);

	int nPosNegId = nPosNeg == 1 ? 0 : 1;
	rData.FlexD[nPosNegId] = TChkResD.pCS457->pStrnD->FlexD;
	rData.FlexCompFlg = TChkResD.pCS457->pStrnD->FlexD;

	return true;
}

bool CRatingReportDataCtrl::Get_CS457_DetailShear(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASSC_CSG_BS_CASE AsscCaseD;
	m_pDgnResult->GetShearStrengthRating4CS454CSG(ElemK, 0, 0, AsscCaseD);
	rData.bShear = AsscCaseD.AsscB[nPos].ComD.bChk;
	if (!rData.bShear) { return false; }

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pCS457 = &TMembRes;

	CSG_STRN_RES_CS457 StrnD;
	CSG_CHECK_RES_CS457 TRes;
	TRes.pStrnD = &StrnD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pCS457 = &TRes;
	CString strRCase = GetRatingCaseName(AsscCaseD.AsscB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsscCaseD.AsscB[nPos].ComD.unConcurrent);

	MembLcomD.aChkFor[0].nLcomType = 0;
	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);

	ConvertCsgDgnForce(AsscCaseD.AsscB[nPos].ComD, MembLcomD);

	CSG_CHK_RES_DTR_PARAM  DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
	TChkResD.pCS457->pStrnD->ShearD.ForStrD.strLcomName = CT2W(strRCase);
	TChkResD.pCS457->pStrnD->ShearD.ForStrD.strLcomType = CT2W(strRCaseSubType);

	rData.ShearD = TChkResD.pCS457->pStrnD->ShearD;

	return true;
}

bool CRatingReportDataCtrl::Get_CS457_DetailCombinedMV(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASCB_CSG_BS_CASE AscbCaseD;
	m_pDgnResult->GetCombinedMVStrength4CS454_CSGResult(ElemK, 0, 0, AscbCaseD);
	rData.bCombine = AscbCaseD.AscbB[nPos].ComD.bChk;
	if (!rData.bCombine) { return false; }

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pCS457 = &TMembRes;

	CSG_STRN_RES_CS457 StrnD;
	CSG_CHECK_RES_CS457 TRes;
	TRes.pStrnD = &StrnD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pCS457 = &TRes;
	CString strRCase = GetRatingCaseName(AscbCaseD.AscbB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AscbCaseD.AscbB[nPos].ComD.unConcurrent);

	MembLcomD.aChkFor[0].nLcomType = 0;
	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);

	ConvertCsgDgnForce(AscbCaseD.AscbB[nPos].ComD, MembLcomD);
	m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgComfCvlToEng(AscbCaseD.AscbB[nPos].ForcePanelM, MembLcomD.PanelM);
	m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgComfCvlToEng(AscbCaseD.AscbB[nPos].ForcePanelV, MembLcomD.PanelV);

	CSG_CHK_RES_DTR_PARAM DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
	TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomName = CT2W(strRCase);
	TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomType = CT2W(strRCaseSubType);
	TChkResD.pCS457->pStrnD->CombD.ForStrD.strLcomName = TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomName;
	TChkResD.pCS457->pStrnD->CombD.ForStrD.strLcomType = TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomType;

	rData.CombD = TChkResD.pCS457->pStrnD->CombD;
	rData.CombFlex = TChkResD.pCS457->pStrnD->FlexD;

	return true;
}

bool CRatingReportDataCtrl::Get_CS457_DetailLongShear(BOOL bULS, ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASLS_CSG_BS_CASE AslsCaseD;
	if (bULS)
	{
		m_pDgnResult->GetLongShear4CS454_CSGResult(ElemK, 0, 0, AslsCaseD);
		rData.bSCon = AslsCaseD.AslsB[nPos].ComD.bChk;
		if (!rData.bSCon) { return false; }
	}
	else
	{
		m_pDgnResult->GetLongShearSLS4CS454_CSGResult(ElemK, 0, 0, AslsCaseD);
		rData.bSConServ = AslsCaseD.AslsB[nPos].ComD.bChk;
		if (!rData.bSConServ) { return false; }
	}

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pCS457 = &TMembRes;

	CSG_CHECK_RES_CS457 TRes;
	CSG_STRN_RES_CS457 StrnD;
	TRes.pStrnD = &StrnD;
	CSG_SCON_RES_CS457 SconD;
	TRes.pSconD = &SconD;
	CSG_SCON_RES_CS457 ServSconD;
	TRes.pServSconD = &ServSconD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pCS457 = &TRes;

	CString strRCase = GetRatingCaseName(AslsCaseD.AslsB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AslsCaseD.AslsB[nPos].ComD.unConcurrent);

	if (bULS)
	{
		MembLcomD.aChkFor[0].nLcomType = 0;
	}
	else
	{
		MembLcomD.aChkFor[0].nLcomType = 1;
	}

	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);
	ConvertCsgDgnForce(AslsCaseD.AslsB[nPos].ComD, MembLcomD);

	CSG_CHK_RES_DTR_PARAM DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_ShearConnector(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);

	if (bULS)
	{
		TChkResD.pCS457->pSconD->ForStrD.strLcomName = CT2W(strRCase);
		TChkResD.pCS457->pSconD->ForStrD.strLcomType = CT2W(strRCaseSubType);
		rData.SConD = *TChkResD.pCS457->pSconD;
	}
	else
	{
		TChkResD.pCS457->pServSconD->ForStrD.strLcomName = CT2W(strRCase);
		TChkResD.pCS457->pServSconD->ForStrD.strLcomType = CT2W(strRCaseSubType);
		rData.SConServD = *TChkResD.pCS457->pServSconD;
	}

	return true;
}

bool CRatingReportDataCtrl::Get_CS457_DetailFlange4Box(ElemPairK ElemK, const int& nPos, const int& nComTen, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASFG_CSG_BS_D AsfgD;
	int nCT = nComTen;
	m_pDgnResult->GetFlangeWithLStiffenerBoxShape4CS454_CSGResult(ElemK, 0, nCT, 0, AsfgD);
	T_ASFG_CSG_BS_BASE AsfgB = AsfgD.AsfgB[nPos * 2]; // 압축
	rData.bFlexCompFlg = AsfgB.ComD.bChk;
	if (!rData.bFlexCompFlg) { return false; }

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pCS457 = &TMembRes;

	CSG_STRN_RES_CS457 StrnD;
	CSG_CHECK_RES_CS457 TRes;
	TRes.pStrnD = &StrnD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pCS457 = &TRes;
	CString strRCase = GetRatingCaseName(AsfgB.ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsfgB.ComD.unConcurrent);

	MembLcomD.aChkFor[0].nLcomType = 0;
	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);

	ConvertCsgDgnForce(AsfgB.ComD, MembLcomD);

	CSG_CHK_RES_DTR_PARAM DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
	TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomName = CT2W(strRCase);
	TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomType = CT2W(strRCaseSubType);

	rData.FlexCompFlg = TChkResD.pCS457->pStrnD->FlexD;

	return true;
}

bool CRatingReportDataCtrl::Get_CS457_DetailFlangeStiffener4Box(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASFS_CSG_BS_CASE AsfsD;
	m_pDgnResult->GetFlangeStiffenerWithLStiffenerBoxShape4CS454_CSGResult(ElemK, 0, 0, AsfsD);
	rData.bFlexFlgStiff = AsfsD.AsfsB[nPos].ComD.bChk;
	if (!rData.bFlexFlgStiff) { return false; }

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pCS457 = &TMembRes;

	CSG_STRN_RES_CS457 StrnD;
	CSG_CHECK_RES_CS457 TRes;
	TRes.pStrnD = &StrnD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pCS457 = &TRes;
	CString strRCase = GetRatingCaseName(AsfsD.AsfsB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsfsD.AsfsB[nPos].ComD.unConcurrent);

	MembLcomD.aChkFor[0].nLcomType = 0;
	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);

	ConvertCsgDgnForce(AsfsD.AsfsB[nPos].ComD, MembLcomD);

	CSG_CHK_RES_DTR_PARAM DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
	TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomName = CT2W(strRCase);
	TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomType = CT2W(strRCaseSubType);

	rData.FlexFlgStiff = TChkResD.pCS457->pStrnD->FlexD;

	return true;
}

bool CRatingReportDataCtrl::Get_CS457_DetailWebYieldBuckling4Box(ElemPairK ElemK, const int& nPos, const int& nPosNeg, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASWY_CSG_BS_D AswyD;
	int nPN = nPosNeg;
	m_pDgnResult->GetWebYieldingWithLStiffenerBoxShape4CS454_CSGResult(ElemK, 0, nPN, 0, AswyD);
	rData.bFlexWeb = AswyD.AswyB[nPos].ComD.bChk;
	if (!rData.bFlexWeb) { return false; }

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pCS457 = &TMembRes;

	CSG_STRN_RES_CS457 StrnD;
	CSG_CHECK_RES_CS457 TRes;
	TRes.pStrnD = &StrnD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pCS457 = &TRes;
	CString strRCase = GetRatingCaseName(AswyD.AswyB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AswyD.AswyB[nPos].ComD.unConcurrent);

	MembLcomD.aChkFor[0].nLcomType = 0;
	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);

	ConvertCsgDgnForce(AswyD.AswyB[nPos].ComD, MembLcomD);
	for (int i = 0; i < 2; ++i)
	{
		m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgComfCvlToEng(AswyD.AswyB[nPos].Panelij[i], MembLcomD.Panelij[i]);
	}

	CSG_CHK_RES_DTR_PARAM DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
	TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomName = CT2W(strRCase);
	TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomType = CT2W(strRCaseSubType);

	int nPosNegId = nPosNeg == 1 ? 0 : 1;
	rData.FlexWeb[nPosNegId] = TChkResD.pCS457->pStrnD->FlexD;

	return true;
}

bool CRatingReportDataCtrl::Get_CS457_DetailSLS(ElemPairK ElemK, const int& nPos, const int& nComTen, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASSL_CSG_BS_CASE AsslCaseD;
	int nCT = nComTen;
	m_pDgnResult->GetSLSRating4CS454CSG(ElemK, 0, nCT, 0, AsslCaseD);
	if (nComTen == 1)
	{
		rData.bServStrComp = AsslCaseD.AsslB[nPos].ComD.bChk;
		if (!rData.bServStrComp) { return false; }
	}
	else
	{
		rData.bServStrTens = AsslCaseD.AsslB[nPos].ComD.bChk;
		if (!rData.bServStrTens) { return false; }
	}

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }

	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pCS457 = &TMembRes;

	CSG_SERV_RES_CS457 ServD;
	CSG_CHECK_RES_CS457 TRes;
	TRes.pServD = &ServD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pCS457 = &TRes;
	CString strRCase = GetRatingCaseName(AsslCaseD.AsslB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsslCaseD.AsslB[nPos].ComD.unConcurrent);

	MembLcomD.aChkFor[0].nLcomType = 1;
	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);

	ConvertCsgDgnForce(AsslCaseD.AsslB[nPos].ComD, MembLcomD);

	CSG_CHK_RES_DTR_PARAM  DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_Service(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
	ServD.ForStrD.strLcomName = CT2W(strRCase);
	ServD.ForStrD.strLcomType = CT2W(strRCaseSubType);

	int nChkIndex = nComTen == 1 ? 0 : 1;
	rData.ServStr[nChkIndex] = *TChkResD.pCS457->pServD;

	return true;
}
#pragma endregion

#pragma region /// for NR2006
bool CRatingReportDataCtrl::Get_NR2006_DetailFlex(ElemPairK ElemK, const int& nPos, const int& nPosNeg, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASFC_CSG_BS_CASE AsfcCaseD;
	int nPN = nPosNeg;
	m_pDgnResult->GetFlexuralStrengthRating4CS454CSG(ElemK, 0, nPN, 0, AsfcCaseD);
	if (nPosNeg == 1)
	{
		rData.bFlexPos = AsfcCaseD.AsfcB[nPos].ComD.bChk;
		if (!rData.bFlexPos) { return false; }
	}
	else
	{
		rData.bFlexNeg = AsfcCaseD.AsfcB[nPos].ComD.bChk;
		if (!rData.bFlexNeg) { return false; }
	}

	const T_CSGF_MEMB& MembForD = AsfcCaseD.AsfcB[nPos].ComD.ForD;

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	T_ALCS_D AlcsD; AlcsD.Initialize();
	m_pAttrCtrl2->GetAlcs(AsfcCaseD.AsfcB[nPos].ComD.RCaseK, AlcsD);

	CSG_MEMBPOS_RES_NR_GN_CIV_025_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pNRGN = &TMembRes;

	CSG_STRN_RES_CS457 StrnD;
	CSG_CHECK_RES_NR_GN_CIV_025 TRes;
	TRes.pStrnD = &StrnD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pNRGN = &TRes;
	CString strRCase = GetRatingCaseName(AsfcCaseD.AsfcB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsfcCaseD.AsfcB[nPos].ComD.unConcurrent);

	ElemPairK EMymaxPairK(MembForD.ElemK4Mymax, EN_EL_BEAM);
	int nPos4Mymax = GetSectPos4MyMax(MembForD.nPos4Mymax);
	pRatingCtrl->m_pCSGDataCtrl->Get_CsgUlmsData4MyMax(EMymaxPairK, nPos4Mymax, MembLcomD.Memb.Ulms);

	MembLcomD.aChkFor[0].nLcomType = 3; // 강합성이 없어서 3으로 할당, 강합성 들어오면 판단 필요
	MembLcomD.aChkFor[0].enLLType = GetNRLiveLoadType(AlcsD.nLiveType);
	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);

	ConvertCsgDgnForce(AsfcCaseD.AsfcB[nPos].ComD, MembLcomD);
	ConvertCsgDngReaction(ElemK, AsfcCaseD.AsfcB[nPos].ComD.RCaseK, nPos, AsfcCaseD.AsfcB[nPos].ComD.unConcurrent, MembLcomD);


	CSG_CHK_RES_DTR_PARAM DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
	TChkResD.pNRGN->pStrnD->FlexD.ForStrD.strLcomName = CT2W(strRCase);
	TChkResD.pNRGN->pStrnD->FlexD.ForStrD.strLcomType = CT2W(strRCaseSubType);

	int nPosNegId = nPosNeg == 1 ? 0 : 1;
	rData.FlexD[nPosNegId] = TChkResD.pNRGN->pStrnD->FlexD;
	rData.FlexCompFlg = TChkResD.pNRGN->pStrnD->FlexD;

	return true;
}

bool CRatingReportDataCtrl::Get_NR2006_DetailShear(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASSC_CSG_BS_CASE AsscCaseD;
	m_pDgnResult->GetShearStrengthRating4CS454CSG(ElemK, 0, 0, AsscCaseD);
	rData.bShear = AsscCaseD.AsscB[nPos].ComD.bChk;
	if (!rData.bShear) { return false; }

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	T_ALCS_D AlcsD; AlcsD.Initialize();
	m_pAttrCtrl2->GetAlcs(AsscCaseD.AsscB[nPos].ComD.RCaseK, AlcsD);

	const T_CSGF_MEMB& MembForD = AsscCaseD.AsscB[nPos].ComD.ForD;

	CSG_MEMBPOS_RES_NR_GN_CIV_025_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pNRGN = &TMembRes;

	CSG_STRN_RES_CS457 StrnD;
	CSG_CHECK_RES_NR_GN_CIV_025 TRes;
	TRes.pStrnD = &StrnD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pNRGN = &TRes;
	CString strRCase = GetRatingCaseName(AsscCaseD.AsscB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsscCaseD.AsscB[nPos].ComD.unConcurrent);

	ElemPairK EMymaxPairK(MembForD.ElemK4Mymax, EN_EL_BEAM);
	int nPos4Mymax = GetSectPos4MyMax(MembForD.nPos4Mymax);
	pRatingCtrl->m_pCSGDataCtrl->Get_CsgUlmsData4MyMax(EMymaxPairK, nPos4Mymax, MembLcomD.Memb.Ulms);

	MembLcomD.aChkFor[0].nLcomType = 3;
	MembLcomD.aChkFor[0].enLLType = GetNRLiveLoadType(AlcsD.nLiveType);
	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);

	ConvertCsgDgnForce(AsscCaseD.AsscB[nPos].ComD, MembLcomD);

	CSG_CHK_RES_DTR_PARAM  DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
	TChkResD.pNRGN->pStrnD->ShearD.ForStrD.strLcomName = CT2W(strRCase);
	TChkResD.pNRGN->pStrnD->ShearD.ForStrD.strLcomType = CT2W(strRCaseSubType);

	rData.ShearD = TChkResD.pNRGN->pStrnD->ShearD;

	return true;
}

bool CRatingReportDataCtrl::Get_NR2006_DetailFastener(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASSF_SG_BS_CASE AssfCaseD;
	m_pDgnResult->GetStrengthFastenerRating4NR_GN_CIV(ElemK, 0, 0, AssfCaseD);
	rData.bFastener = AssfCaseD.AssfB[nPos].ComD.bChk;
	if (!rData.bFastener) { return false; }

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	T_ALCS_D AlcsD; AlcsD.Initialize();
	m_pAttrCtrl2->GetAlcs(AssfCaseD.AssfB[nPos].ComD.RCaseK, AlcsD);

	const T_CSGF_MEMB& MembForD = AssfCaseD.AssfB[nPos].ComD.ForD;

	CSG_MEMBPOS_RES_NR_GN_CIV_025_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pNRGN = &TMembRes;

	CSG_STRN_RES_CS457 StrnD;
	CSG_CHECK_RES_NR_GN_CIV_025 TRes;
	TRes.pStrnD = &StrnD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pNRGN = &TRes;
	CString strRCase = GetRatingCaseName(AssfCaseD.AssfB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AssfCaseD.AssfB[nPos].ComD.unConcurrent);

	ElemPairK EMymaxPairK(MembForD.ElemK4Mymax, EN_EL_BEAM);
	int nPos4Mymax = GetSectPos4MyMax(MembForD.nPos4Mymax);
	pRatingCtrl->m_pCSGDataCtrl->Get_CsgUlmsData4MyMax(EMymaxPairK, nPos4Mymax, MembLcomD.Memb.Ulms);

	MembLcomD.aChkFor[0].nLcomType = 3;
	MembLcomD.aChkFor[0].enLLType = GetNRLiveLoadType(AlcsD.nLiveType);
	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);

	ConvertCsgDgnForce(AssfCaseD.AssfB[nPos].ComD, MembLcomD);

	CSG_CHK_RES_DTR_PARAM  DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
	TChkResD.pNRGN->pStrnD->FastenerD.ForStrD.strLcomName = CT2W(strRCase);
	TChkResD.pNRGN->pStrnD->FastenerD.ForStrD.strLcomType = CT2W(strRCaseSubType);

	rData.FastenerD = TChkResD.pNRGN->pStrnD->FastenerD;

	return true;
}

bool CRatingReportDataCtrl::Get_NR2006_DetailCombinedMV(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASCB_CSG_BS_CASE AscbCaseD;
	m_pDgnResult->GetCombinedMVStrength4CS454_CSGResult(ElemK, 0, 0, AscbCaseD);
	rData.bCombine = AscbCaseD.AscbB[nPos].ComD.bChk;
	if (!rData.bCombine) { return false; }

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	T_ALCS_D AlcsD; AlcsD.Initialize();
	m_pAttrCtrl2->GetAlcs(AscbCaseD.AscbB[nPos].ComD.RCaseK, AlcsD);

	const T_CSGF_MEMB& MembForD = AscbCaseD.AscbB[nPos].ComD.ForD;

	CSG_MEMBPOS_RES_NR_GN_CIV_025_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pNRGN = &TMembRes;

	CSG_STRN_RES_CS457 StrnD;
	CSG_CHECK_RES_NR_GN_CIV_025 TRes;
	TRes.pStrnD = &StrnD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pNRGN = &TRes;
	CString strRCase = GetRatingCaseName(AscbCaseD.AscbB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AscbCaseD.AscbB[nPos].ComD.unConcurrent);

	ElemPairK EMymaxPairK(MembForD.ElemK4Mymax, EN_EL_BEAM);
	int nPos4Mymax = GetSectPos4MyMax(MembForD.nPos4Mymax);
	pRatingCtrl->m_pCSGDataCtrl->Get_CsgUlmsData4MyMax(EMymaxPairK, nPos4Mymax, MembLcomD.Memb.Ulms);

	MembLcomD.aChkFor[0].nLcomType = 3;
	MembLcomD.aChkFor[0].enLLType = GetNRLiveLoadType(AlcsD.nLiveType);
	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);

	ConvertCsgDgnForce(AscbCaseD.AscbB[nPos].ComD, MembLcomD);
	m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgComfCvlToEng(AscbCaseD.AscbB[nPos].ForcePanelM, MembLcomD.PanelM);
	m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgComfCvlToEng(AscbCaseD.AscbB[nPos].ForcePanelV, MembLcomD.PanelV);

	CSG_CHK_RES_DTR_PARAM DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
	TChkResD.pNRGN->pStrnD->FlexD.ForStrD.strLcomName = CT2W(strRCase);
	TChkResD.pNRGN->pStrnD->FlexD.ForStrD.strLcomType = CT2W(strRCaseSubType);
	TChkResD.pNRGN->pStrnD->CombD.ForStrD.strLcomName = TChkResD.pNRGN->pStrnD->FlexD.ForStrD.strLcomName;
	TChkResD.pNRGN->pStrnD->CombD.ForStrD.strLcomType = TChkResD.pNRGN->pStrnD->FlexD.ForStrD.strLcomType;

	rData.CombD = TChkResD.pNRGN->pStrnD->CombD;
	rData.CombFlex = TChkResD.pNRGN->pStrnD->FlexD;

	return true;
}

bool CRatingReportDataCtrl::Get_NR2006_DetailLongShear(BOOL bULS, ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASLS_CSG_BS_CASE AslsCaseD;
	if (bULS)
	{
		m_pDgnResult->GetLongShear4CS454_CSGResult(ElemK, 0, 0, AslsCaseD);
		rData.bSCon = AslsCaseD.AslsB[nPos].ComD.bChk;
		if (!rData.bSCon) { return false; }
	}
	else
	{
		m_pDgnResult->GetLongShearSLS4CS454_CSGResult(ElemK, 0, 0, AslsCaseD);
		rData.bSConServ = AslsCaseD.AslsB[nPos].ComD.bChk;
		if (!rData.bSConServ) { return false; }
	}

	T_ALCS_D AlcsD; AlcsD.Initialize();
	m_pAttrCtrl2->GetAlcs(AslsCaseD.AslsB[nPos].ComD.RCaseK, AlcsD);

	const T_CSGF_MEMB& MembForD = AslsCaseD.AslsB[nPos].ComD.ForD;

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	CSG_MEMBPOS_RES_NR_GN_CIV_025_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pNRGN = &TMembRes;

	CSG_CHECK_RES_NR_GN_CIV_025 TRes;
	CSG_STRN_RES_CS457 StrnD;
	TRes.pStrnD = &StrnD;	
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pNRGN = &TRes;

	CString strRCase = GetRatingCaseName(AslsCaseD.AslsB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AslsCaseD.AslsB[nPos].ComD.unConcurrent);

	ElemPairK EMymaxPairK(MembForD.ElemK4Mymax, EN_EL_BEAM);
	int nPos4Mymax = GetSectPos4MyMax(MembForD.nPos4Mymax);
	pRatingCtrl->m_pCSGDataCtrl->Get_CsgUlmsData4MyMax(EMymaxPairK, nPos4Mymax, MembLcomD.Memb.Ulms);

	if (bULS)
	{
		MembLcomD.aChkFor[0].nLcomType = 3;
	}
	else
	{
		MembLcomD.aChkFor[0].nLcomType = 1;
	}
	MembLcomD.aChkFor[0].enLLType = GetNRLiveLoadType(AlcsD.nLiveType);

	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);
	ConvertCsgDgnForce(AslsCaseD.AslsB[nPos].ComD, MembLcomD);

	CSG_CHK_RES_DTR_PARAM DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_ShearConnector(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
		
	return true;
}

bool CRatingReportDataCtrl::Get_NR2006_DetailBearingStiffner(ElemPairK ElemK, const int& nPos, const enNR_TS_Check& enTSType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData)
{
	T_ASBS_SG_BS_CASE AsbsD;	
	m_pDgnResult->GetBearingStiffenerRating4NR_GN_CIV(ElemK, 0, 0, enTSType, AsbsD);
	T_ASBS_SG_BS_BASE AsbsB = AsbsD.AsbsB[nPos]; // 압축

	switch (enTSType)
	{
	case enNR_TS_Check::EN_NR_TS_Web_Yielding:
	{
		rData.bTsWeb = AsbsB.ComD.bChk;
		break;
	}
	case enNR_TS_Check::EN_NR_TS_Yielding:
	{
		rData.bTsStiff = AsbsB.ComD.bChk;
		break;
	}
	case enNR_TS_Check::EN_NR_TS_Buckling:
	{
		rData.bTsBuck = AsbsB.ComD.bChk;
		break;
	}
	default: ASSERT(0); break;
	}

	if (!AsbsB.ComD.bChk) { return false; }

	T_ALCS_D AlcsD; AlcsD.Initialize();
	m_pAttrCtrl2->GetAlcs(AsbsD.AsbsB[nPos].ComD.RCaseK, AlcsD);

	const T_CSGF_MEMB& MembForD = AsbsD.AsbsB[nPos].ComD.ForD;

	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
	if (!pRatingCtrl) { return false; }
	if (!pRatingCtrl->m_pCSGDataCtrl) { return false; }

	CSG_MEMBPOS_RES_NR_GN_CIV_025_D TMembRes = MembResD;
	CSG_MEMB_RESULT_D TMembR;
	TMembR.pNRGN = &TMembRes;

	CSG_STRN_RES_CS457 StrnD;
	CSG_CHECK_RES_NR_GN_CIV_025 TRes;
	TRes.pStrnD = &StrnD;
	CSG_CHECK_RESULT_CODES TChkResD;
	TChkResD.pNRGN = &TRes;
	CString strRCase = GetRatingCaseName(AsbsD.AsbsB[nPos].ComD.RCaseK);
	CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsbsD.AsbsB[nPos].ComD.unConcurrent);

	ElemPairK EMymaxPairK(MembForD.ElemK4Mymax, EN_EL_BEAM);
	int nPos4Mymax = GetSectPos4MyMax(MembForD.nPos4Mymax);
	pRatingCtrl->m_pCSGDataCtrl->Get_CsgUlmsData4MyMax(EMymaxPairK, nPos4Mymax, MembLcomD.Memb.Ulms);

	MembLcomD.aChkFor[0].nLcomType = 3;
	MembLcomD.aChkFor[0].enLLType = GetNRLiveLoadType(AlcsD.nLiveType);
	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aUlmf.SetSize(6);

	ConvertCsgDgnForce(AsbsD.AsbsB[nPos].ComD, MembLcomD);
	ConvertCsgDngReaction(ElemK, AsbsD.AsbsB[nPos].ComD.RCaseK, nPos, AsbsD.AsbsB[nPos].ComD.unConcurrent, MembLcomD);

	CSG_CHK_RES_DTR_PARAM DtrR;
	pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
	
	TChkResD.pNRGN->pStrnD->TranStiffD.ForStrD.strLcomName = CT2W(strRCase);
	TChkResD.pNRGN->pStrnD->TranStiffD.ForStrD.strLcomType = CT2W(strRCaseSubType);

	switch (enTSType)
	{
	case enNR_TS_Check::EN_NR_TS_Web_Yielding: rData.TsWeb = TChkResD.pNRGN->pStrnD->TranStiffD; break;
	case enNR_TS_Check::EN_NR_TS_Yielding: rData.TsStiff = TChkResD.pNRGN->pStrnD->TranStiffD; break;
	case enNR_TS_Check::EN_NR_TS_Buckling: rData.TsBuck = TChkResD.pNRGN->pStrnD->TranStiffD; break;
	default: ASSERT(0); break;
	}

	return true;
}

#pragma endregion

int CRatingReportDataCtrl::GetSectPos4MyMax(int nPos4Mymax)
{
	return nPos4Mymax < 2 ? 0 : 1;
}

enNRLiveLoadType CRatingReportDataCtrl::GetNRLiveLoadType(int nLiveType)
{
	return (nLiveType == 0) ? EN_NR_LIVE_LOAD_RAI : EN_NR_LIVE_LOAD_Wagon;
}

void CRatingReportDataCtrl::ConvertCsgDgnForce(T_ASCD_CSG_BS_BASE& TComD, CSG_MEMB_POSD_KSCE_LSD& rData)
{
	CDgnCsgDataCtrl* pCsgDataCtrl = m_pDataCtrl->m_pCsgDataCtrl;

	CSG_FORCE_COMP& ForStrEng = rData.aChkFor[0];
	pCsgDataCtrl->ConvertCsgComfCvlToEng(TComD.ForD.ChkFor, ForStrEng);
	pCsgDataCtrl->ConvertCsgForceCvlToEng(TComD.ForD.MVFor, ForStrEng.ForMV);
	pCsgDataCtrl->ConvertCsgForceCvlToEng(TComD.ForD.MVForStd, ForStrEng.ForSV);

	CSG_FORCE_COMP ForStrULEng;
	pCsgDataCtrl->ConvertCsgComfCvlToEng(TComD.ForD.MembFor[0], ForStrULEng);
	pCsgDataCtrl->ConvertCsgUlmForce(0, ForStrULEng, rData.aUlmf);

	ForStrULEng.Initialize();
	pCsgDataCtrl->ConvertCsgComfCvlToEng(TComD.ForD.MembFor[1], ForStrULEng);
	pCsgDataCtrl->ConvertCsgUlmForce(1, ForStrULEng, rData.aUlmf);

	ForStrULEng.Initialize();
	pCsgDataCtrl->ConvertCsgComfCvlToEng(TComD.ForD.MembFor[2], ForStrULEng);
	pCsgDataCtrl->ConvertCsgUlmForce(2, ForStrULEng, rData.aUlmf);

	ForStrULEng.Initialize();
	pCsgDataCtrl->ConvertCsgComfCvlToEng(TComD.ForD.MembFor13[0], ForStrULEng);
	pCsgDataCtrl->ConvertCsgUlm13Force(0, ForStrULEng, rData.aUlmf);

	ForStrULEng.Initialize();
	pCsgDataCtrl->ConvertCsgComfCvlToEng(TComD.ForD.MembFor13[1], ForStrULEng);
	pCsgDataCtrl->ConvertCsgUlm13Force(1, ForStrULEng, rData.aUlmf);

	ForStrULEng.Initialize();
	pCsgDataCtrl->ConvertCsgComfCvlToEng(TComD.ForD.MyMaxFor, ForStrULEng);
	pCsgDataCtrl->ConvertCsgUlmMyMaxForce(ForStrULEng, rData.aUlmf);
}

void CRatingReportDataCtrl::ConvertCsgDngReaction(ElemPairK ElemK, T_ALCS_K AlcsK, int nPosIJ, int nMaxMinType, CSG_MEMB_POSD_KSCE_LSD& rData)
{
	CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;

	T_ALCS_D AlcsD; AlcsD.Initialize();
	if (m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD))
	{
		T_REAC_D DLre; DLre.Initialize();
		T_REAC_D SLLre; SLLre.Initialize();
		T_REAC_D DLLre; DLLre.Initialize();
		pRatingCtrl->MakeReactionRCase(AlcsD, ElemK, nPosIJ, nMaxMinType, DLre, SLLre, DLLre, rData);		
	}
}

CString CRatingReportDataCtrl::GetRatingCaseName(T_ALCS_K AlcsK)
{
	T_ALCS_D AlcsD;
	if (m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD))
	{
		return AlcsD.strCaseName;
	}
	return _T("");
}