
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "RptSteelGirder_AASHTO12.h"
#include "RptPlateGirder_EC4.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\AnalysisResult.h"
#include "..\wg_db\StageInfo.h"

#include "..\wg_db\DBLib.h"
#include "..\wg_db\PlateGirderDesign.h"
#include "..\wg_base\TestEnvMgr.h"      

// 
#include "Dgn_CSGRptManager_AASHTO12.h"
#include "DgnCsgDataCtrl.h"
#include "DgnPscCommon.h"
#include "DgnDataCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptSteelGirder_AASHTO12::CRptSteelGirder_AASHTO12()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;

	//m_bTestMode = CDBLib::CheckTheRegistryForDesignTest();

	m_CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(m_CpgdD);

	int nDgnCode = m_CpgdD.iDgnCode;
	if (CDBLib::IsCSGCodeLSD_KR(m_CpgdD.iDgnCode) && m_CpgdD.nConsiderPv == 1)
	{
		nDgnCode = KSSC_2014_CSG;
	}
	else if (m_CpgdD.iDgnCode == KSSC_2014_CSG)
	{
		nDgnCode = KSSC_2014_CSG;
	}

	m_pDoc->SetCivilCodeDgn(nDgnCode);
	m_pDataCtrl = new CCRCDataCtrl();

}

CRptSteelGirder_AASHTO12::CRptSteelGirder_AASHTO12(int nDgnCode)
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;

	m_pDoc->SetCivilCodeDgn(nDgnCode);
	m_pDataCtrl = new CCRCDataCtrl();
}

CRptSteelGirder_AASHTO12::~CRptSteelGirder_AASHTO12()
{
	if (m_pDataCtrl)
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
	}
}

BOOL CRptSteelGirder_AASHTO12::Execute_CSGReport_AASHTO12(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
{
	if (m_bStopExecute) return FALSE;
	Progress(3, 0);
	int nProgressPercent = 0;

	if (m_bStopExecute) return FALSE;
	nProgressPercent += 5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	if (m_bStopExecute) return FALSE;
	nProgressPercent += 5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	BOOL bSuccess = TRUE;

	if (!m_CpgdD.bDesignParameter[0] && !m_CpgdD.bDesignParameter[1] && !m_CpgdD.bDesignParameter[2] &&
		!m_CpgdD.bDesignParameter[3] && !m_CpgdD.bDesignParameter[4] && !m_CpgdD.bDesignParameter[5] &&
		!m_CpgdD.bDesignParameter[6])
	{
		if (m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CHECK));  // _T("선택된 출력변수가 없습니다.
		return FALSE;
	}

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	CArray<T_CGRE_K, T_CGRE_K> aPrintElemKey;
	m_pDoc->m_pAttrCtrl2->GetCgreKeyList(aPrintElemKey);

	ArrElemPairKey aPrintElemPairKey;
	CDBLib::ConvertToElemPairKey(EN_EL_BEAM, aPrintElemKey, aPrintElemPairKey);


	CArray<T_CGRV_K, T_CGRV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl2->GetCgrvKeyList(aPrintVBeamKey);
	if (aPrintVBeamKey.GetSize() > 0)
	{
		ArrElemPairKey aPrintVBeamPairKey;
		CDBLib::ConvertToElemPairKey(EN_EL_VBEAM, aPrintVBeamKey, aPrintVBeamPairKey);
		aPrintElemPairKey.Append(aPrintVBeamPairKey);
	}

	int nPrintElemSize = aPrintElemPairKey.GetSize();
	if (nPrintElemSize == 0)
	{
		if (m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // ")출력할 요소의 설계정보가 없습니다._T("
		AfxMessageBox(strErrMessage);

		return FALSE;
	}

	// Span 정보 계산 
	m_pDataCtrl->Calc_LengthPosition4Span(aPrintElemPairKey);

	// 설계요소와 출력요소의 교집합이 존재할 경우 Report과정을 수행함. 그렇지않다면, Message처리함.
	BOOL bIsExistPrintElem = FALSE;
	CRptPlateGirder_EC4 PlateGirder_EC4;
	for (int nNum = 0; nNum < nPrintElemSize; ++nNum)
	{
		auto ElemK = aPrintElemPairKey[nNum];
		if (PlateGirder_EC4.Get_CheckResultAndPrintElemPositionType(ElemK))
		{
			bIsExistPrintElem = TRUE;
			break;
		}
	}
	if (!bIsExistPrintElem)
	{
		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // ")출력할 요소의 설계정보가 없습니다._T("
		AfxMessageBox(strErrMessage);
		return FALSE;
	}

	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40 / nPrintElemSize;

	// Input Data
	CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>   aInData;   aInData.RemoveAll();

	// Output Data
	CArray<CSG_MEMB_RES_AASHTO_LRFD_D, CSG_MEMB_RES_AASHTO_LRFD_D&>  aRptData;  aRptData.RemoveAll();
	CSG_MEMB_RES_AASHTO_LRFD_D  RptData;


	CSG_MEMB_RESULT_D      MembR;
	CSG_CHECK_RESULT_CODES ChkResD;

	MembR.pAASHTO = new CSG_MEMBPOS_RES_AASHTO_LRFD_D;

	ChkResD.pAASHTO = new CSG_MEMB_RES_AASHTO_LRFD_D;

	int i = 0; // 요소 
	int j = 0; // I/J
	int k = 0; // Positive / Negative
	int kk = 0;
	int nIJ = 0, nMaxMin = 0;

	//m_pDataCtrl->m_pCsgDataCtrl->InitialData(iDgnCode);

	CPlateGirderDesign *pCheckGirder = m_pDoc->m_pPostCtrl->GetPlateGirder();

	try
	{
		int nNumPrtElem = 0;
		//DETATL-REPORT
		if (m_CpgdD.bStrength[0] || m_CpgdD.bStrength[1] || m_CpgdD.bStrength[2] || m_CpgdD.bStrength[3] || m_CpgdD.bStrength[4] || m_CpgdD.bStrength[5])
		{
			m_pDataCtrl->Get_UnbracedLength();
			m_pDataCtrl->m_pCsgDataCtrl->Make_CSGDgnData();

			// Input Data Setting(1)
			T_CPGD_D CpgdD; CpgdD.Initialize();
			m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

			// Pre-Combined Composite Bridge
			BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb();
			CStageInfo *pStageInfo = m_pDoc->m_pPostCtrl->GetStageInfo();
			CArray<T_STAG_K, T_STAG_K> aStagK;
			pStageInfo->GetResultSavedStagKeyList(aStagK);
			int nStageSize = aStagK.GetSize();

			for (i = 0; i < nPrintElemSize; ++i)
			{
				auto  ElemK = aPrintElemPairKey[i];

				CSG_MEMB_POSD_KSCE_LSD    MembLcomD;
				m_pDataCtrl->m_pCsgDataCtrl->ConvertCGSGlobalData(CpgdD, MembLcomD.Memb.Csgd);

				int nConsiderPv = MembLcomD.Memb.Csgd.nConsiderPv; // 0:직선, 1:곡선
				BOOL bCurved = (nConsiderPv == 0) ? FALSE : TRUE;

				if (nStageSize == 0 && bIsPLCB == TRUE)
				{
					MembLcomD.Memb.ChIf.bIsPLCB = TRUE;
				}

				// 1. 출력 여부 결정
				T_CGDP_D CgdpD; CgdpD.Initialize();  // 설계 위치
				T_CGRE_D CgreD; CgreD.Initialize();  // 출력 위치

				int nMatlK = 0;
				if (ElemK.second == EN_EL_BEAM)
				{
					MembLcomD.Memb.ChIf.dLength = m_pDoc->calcLAVElem(ElemK.first);

					if (!m_pDoc->m_pAttrCtrl->GetCgdp(ElemK.first, CgdpD)) continue;
					if (!m_pDoc->m_pAttrCtrl2->GetCgre(ElemK.first, CgreD)) continue;
				
					T_ELEM_D ElemD; ElemD.Initialize();
					if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD)) continue;
					nMatlK = ElemD.elmat;
				}
				else if (ElemK.second == EN_EL_VBEAM)
				{
					MembLcomD.Memb.ChIf.dLength = m_pDoc->calcLenthVbem(ElemK.first);

					T_CGDV_D CgdvD; CgdvD.Initialize();
					T_CGRV_D CgrvD; CgrvD.Initialize();
					if (!m_pDoc->m_pAttrCtrl->GetCgdv(ElemK.first, CgdvD)) continue;
					CgdpD=CgdvD;
					if (!m_pDoc->m_pAttrCtrl2->GetCgrv(ElemK.first, CgrvD)) continue;
					CgreD=CgrvD;

					T_VBEM_D VbemD; VbemD.Initialize();
					if (m_pDoc->m_pAttrCtrl2->GetVbem(ElemK.first, VbemD))
					{
						T_MATL_K SlabMatlK = 0, GirderMatlK = 0;
						m_pDoc->m_pAttrCtrl2->GetSecvMatlK(ElemK.first, SlabMatlK, GirderMatlK);
						nMatlK = GirderMatlK;
					}
				}
				else ASSERT(0);

				
				BOOL bCheckCalcI = TRUE;
				BOOL bCheckCalcJ = TRUE;
				ShouldCheckPosiIJ(CgdpD.iDgnPosi, bCheckCalcI, bCheckCalcJ);
				BOOL bCheckPrintI = TRUE;
				BOOL bCheckPrintJ = TRUE;
				ShouldCheckPosiIJ(CgreD.iDgnPosi, bCheckPrintI, bCheckPrintJ);
				BOOL bPrintI = (bCheckCalcI && bCheckPrintI);
				BOOL bPrintJ = (bCheckCalcJ && bCheckPrintJ);

				// 2. Input Data Setting(2)
				int nSelUnit = CDBLib::GetReportSelUnit();
				MembLcomD.nSelUnit = nSelUnit;


				if (!m_pDataCtrl->m_pCsgDataCtrl->m_mCsgMatl.Lookup(nMatlK, MembLcomD.Memb.Matl))
				{
					continue;
					//MembLcomD.Memb.Matl.Initialize();
				}
				nNumPrtElem += 1;

				nNumPrtElem += 1;
				MembLcomD.Memb.ChIf.bCurved = bCurved;

				int nNode[2] = { 0, 0 };
				BOOL bSupprotNode[2] = { FALSE, FALSE };
				m_pDataCtrl->m_pCsgDataCtrl->CheckSupprotNodeElem(ElemK, nNode);
				bSupprotNode[0] = nNode[0];
				bSupprotNode[1] = nNode[1];
				bool bInteriorSupport[2] = { false, false };
				m_pDataCtrl->m_pCsgDataCtrl->CheckInteriorSupportNodeElem(ElemK, bInteriorSupport);

				const auto& itSect4E = m_pDataCtrl->m_pCsgDataCtrl->m_mCsgElemSect.find(ElemK);
				if (itSect4E != m_pDataCtrl->m_pCsgDataCtrl->m_mCsgElemSect.end())
				{
					const auto& itSect = m_pDataCtrl->m_pCsgDataCtrl->m_mCsgSect.find(itSect4E->second);
					if (itSect != m_pDataCtrl->m_pCsgDataCtrl->m_mCsgSect.end())
					{
						MembLcomD.Memb.PosD[0].Sect = itSect->second.SectPos[0];
						MembLcomD.Memb.PosD[1].Sect = itSect->second.SectPos[1];
						MembLcomD.Memb.PosD[0].bSupprotNode = bSupprotNode[0];
						MembLcomD.Memb.PosD[1].bSupprotNode = bSupprotNode[1];
						MembLcomD.Memb.PosD[0].bInteriorSupport = bInteriorSupport[0];
						MembLcomD.Memb.PosD[1].bInteriorSupport = bInteriorSupport[1];
					}
				}
				/// - MembLcomD.Memb.PosD의 다른 데이터들 받기
				m_pDataCtrl->m_pCsgDataCtrl->Get_CsgMembInData(ElemK, MembLcomD);
				// Unbraced Length
				// Unbraced Length Setting
				T_KFAC_D KFacD; KFacD.Initialize();
				BOOL bGetKfac = m_pDataCtrl->GetGenKfactor(ElemK, KFacD);
				m_pDataCtrl->m_pCsgDataCtrl->Get_UnbracedLengthData(ElemK, bGetKfac, KFacD, MembLcomD.Memb.Ulen);

				// Unbraced Length For Section
				T_SPAN_K CurSpanK = 0;
				ElemPairK nElemI = ElemK, nElemM = ElemK, nElemJ = ElemK;
				int nPosI = 0;
				int nPosM = 0; // 임시, 찾아야함. 
				int nPosJ = 1;

				DgnBeamPairK nUblmK(EN_EL_BEAM, 0);
				BOOL bCantilever = FALSE;

				m_pDataCtrl->GetUbLengthElemInfo(ElemK, nUblmK, nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, bCantilever);

				m_pDataCtrl->m_pCsgDataCtrl->Get_CsgUlmSection(nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, MembLcomD.Memb.Ulms);


				/// - 하중정보는 계산전에 받음
				int nChkLcomSize = 1;
				MembLcomD.aUlmf.SetSize(5);    // Unbraced Length Force
				MembLcomD.aChkFor.SetSize(nChkLcomSize);  // Check Force;
				MembLcomD.aUlmFor.SetSize(nChkLcomSize);

				// 3. Output Data Setting(1)
				RptData.Initialize();
				//CSG_MEMB_RES_AASHTO_LRFD_D MembResD;
				m_pDataCtrl->m_pCsgDataCtrl->MakeMemberResultData(ElemK, MembLcomD, MembR);

				T_SCBR_D  ScbrD;
				T_SCVR_D  ScvrD;

				T_CSBR_D CsbrD;
				T_CSVR_D CsvrD;
				T_CSAC_D CsacD;
				T_CSFC_D CsfcD;
				T_CSCR_D CscrD;
				T_CSSL_D CsslD;
				T_BSTF_D BstfD;

				//         CSG_MEMB_RES_AASHTO_LRFD_STR_D         StrnResD;
				//         CSG_MEMB_RES_AASHTO_LRFD_SER_D         ServResD;
				//         CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D     FatiResD;
				//         CSG_MEMB_RES_AASHTO_LRFD_CONSTRUCT     ConsResD;
				//         CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D  CscrResD;       // 전단연결재
				//         CSG_MEMB_RES_AASHTO_LRFD_HS_LS_D       CsslResD;       // 수평보강재
				//         CSG_MEMB_RES_AASHTO_LRFD_BEARING_STIFFENER_D BstfResD; // Bearing Stiffener

								// 4. Output Data Setting(2)       
				for (j = 0; j < 2; j++) // I,J
				{
					MembLcomD.Memb.ChIf.nChkPos = j;
					BOOL bPosiI = j == 0 ? TRUE : FALSE;
					if (!m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, j + 1)) continue;
					if (j == 0 && (!bPrintI)) continue;
					if (j == 1 && (!bPrintJ)) continue;

					CSG_CHK_RES_DTR_PARAM  DtrR;

					// 휨 
					if (m_CpgdD.bDesignParameter[0])
					{
						if (pCheckGirder->ReadCSBR_LSD12(ElemK, CsbrD)) // 요소별 한번만 읽으면 되는데..
						{
							for (k = 0; k < 2; k++) // Positive/Negative
							{
								kk = j * 2 + k;  // 0: I-정, 1: I-부, 2: J-정, 3: J-부  
								if (!CsbrD.CsbrB[kk].bChk) continue; // 설계결과가 없으면 Continue                 
								MembLcomD.aChkFor[0].nLcomType = 0;
								MembLcomD.aChkFor[0].nMax = CsbrD.CsbrB[kk].nMax;//!!!
								MembLcomD.bChkUlmf = CsbrD.CsbrB[kk].bChkUlmf;
								ConvertCsgCalcUlmForce(CsbrD.CsbrB[kk].UlForLC, MembLcomD.aUlmf[0]);
								ConvertCsgCalcUlmForce(CsbrD.CsbrB[kk].UlForNC, MembLcomD.aUlmf[1]);
								ConvertCsgCalcUlmForce(CsbrD.CsbrB[kk].UlForLT, MembLcomD.aUlmf[2]);
								ConvertCsgCalcUlmForce(CsbrD.CsbrB[kk].UlForST, MembLcomD.aUlmf[3]);
								MembLcomD.aUlmFor[0].dMmax = CsbrD.CsbrB[kk].UlForLC.MmaxFor.GetMy();
								ConvertCsgCalcChkForce(CsbrD.CsbrB[kk].ComFor, MembLcomD.aChkFor[0]);
								m_pDataCtrl->m_pCsgDataCtrl->Check_Strength(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);
								RptData.StrengthD.FlexD[k] = ChkResD.pAASHTO->StrengthD.FlexD[k];
								T_LCOM_D LcomD;
								if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, CsbrD.CsbrB[kk].nOrgLcomK, LcomD))
								{
									RptData.StrengthD.FlexD[k].ForceD.strLcomName = CT2W(LcomD.LoadCombName);
								}
								RptData.StrengthD.FlexD[k].ForceD.strLcomType = CRptSteelGirder::GetLoadCombType(CsbrD.CsbrB[kk].nMax);
								RptData.StrengthD.SectPropD[k] = ChkResD.pAASHTO->StrengthD.SectPropD[k];
								RptData.RhD = MembR.pAASHTO->MembR[j].RhD;
							}
						}
					}

					// 전단
					if (m_CpgdD.bDesignParameter[1])
					{
						if (pCheckGirder->ReadCSVR_LSD12(ElemK, CsvrD))
						{
							if (CsvrD.CsvrB[j].bChk)  // 0 : I-max  1: J-max
							{
								MembLcomD.aChkFor[0].nLcomType = 0;
								MembLcomD.aChkFor[0].nMax = CsvrD.CsvrB[j].nMax;//!!!
								ConvertCsgCalcUlmForce(CsvrD.CsvrB[j].UlFor, MembLcomD.aUlmf[0]);
								ConvertCsgCalcChkForce(CsvrD.CsvrB[j].ComFor, MembLcomD.aChkFor[0]);
								m_pDataCtrl->m_pCsgDataCtrl->Check_Strength(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);
								RptData.StrengthD.ShearD = ChkResD.pAASHTO->StrengthD.ShearD;
								T_LCOM_D LcomD;
								if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, CsvrD.CsvrB[j].nOrgLcomK, LcomD))
								{
									RptData.StrengthD.ShearD.ForceD.strLcomName = CT2W(LcomD.LoadCombName);
								}
								RptData.StrengthD.ShearD.ForceD.strLcomType = CRptSteelGirder::GetLoadCombType(CsvrD.CsvrB[j].nMax);
							}
						}
					}

					// 사용한계
					if (m_CpgdD.bDesignParameter[2])
					{
						if (pCheckGirder->ReadCSAC_LSD12(ElemK, CsacD))
						{
							int nNum = 0;
							double dRatioFirst = 0.0;
							int kk_max_po = 0;
							int kk_max_ne = 0;
							CSG_MEMB_RES_AASHTO_LRFD_SER_D  ServiceFirst;
							for (k = 0; k < 2; k++) // Positive/Negative
							{
								kk = j * 2 + k;
								if (!CsacD.CsacB[kk].bChk) continue;
								MembLcomD.aChkFor[0].nLcomType = 1;
								MembLcomD.aChkFor[0].nMax = CsacD.CsacB[kk].nMax;//!!!
								ConvertCsgCalcUlmForce(CsacD.CsacB[kk].UlFor, MembLcomD.aUlmf[0]);
								ConvertCsgCalcChkForce(CsacD.CsacB[kk].ComFor, MembLcomD.aChkFor[0]);
								m_pDataCtrl->m_pCsgDataCtrl->Check_Service(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);

								if (k == 0)
								{
									if (nNum == 0)   // 정/부 중 불리한 것 출력
									{
										ServiceFirst = ChkResD.pAASHTO->ServiceD;
										dRatioFirst = ChkResD.pAASHTO->ServiceD.dRatio;

										RptData.ServiceD = ChkResD.pAASHTO->ServiceD;
										kk_max_po = kk;
									}
									else
									{
										if (dRatioFirst < ChkResD.pAASHTO->ServiceD.dRatio)
										{
											RptData.ServiceD = ChkResD.pAASHTO->ServiceD;
											kk_max_po = kk;
										}
										else
										{
											RptData.ServiceD = ServiceFirst;
										}
									}
								}
								else if (k == 1)
								{
									if (nNum == 0)   // 정/부 중 불리한 것 출력
									{
										ServiceFirst = ChkResD.pAASHTO->ServiceD;
										dRatioFirst = ChkResD.pAASHTO->ServiceD.dRatio;

										RptData.ServiceD_rpt = ChkResD.pAASHTO->ServiceD;
										kk_max_ne = kk;
									}
									else
									{
										if (dRatioFirst < ChkResD.pAASHTO->ServiceD.dRatio)
										{
											RptData.ServiceD_rpt = ChkResD.pAASHTO->ServiceD;
											kk_max_ne = kk;
										}
										else
										{
											RptData.ServiceD_rpt = ServiceFirst;
										}
									}
								}

								nNum += 1;

								if (k == 0)
								{
									T_LCOM_D LcomD;
									if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, CsacD.CsacB[kk_max_po].nOrgLcomK, LcomD))
									{
										RptData.ServiceD.ForceD.strLcomName = CT2W(LcomD.LoadCombName);
									}
									RptData.ServiceD.ForceD.strLcomType = CRptSteelGirder::GetLoadCombType(CsacD.CsacB[kk_max_po].nMax);

								}
								else if (k == 1)
								{
									T_LCOM_D LcomD;
									if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, CsacD.CsacB[kk_max_ne].nOrgLcomK, LcomD))
									{
										RptData.ServiceD_rpt.ForceD.strLcomName = CT2W(LcomD.LoadCombName);
									}
									RptData.ServiceD_rpt.ForceD.strLcomType = CRptSteelGirder::GetLoadCombType(CsacD.CsacB[kk_max_ne].nMax);
								}
							}
						}
					}

					// 피로한계
					if (m_CpgdD.bDesignParameter[4])
					{
						if (pCheckGirder->ReadCSFC_LSD12(ElemK, CsfcD))
						{
							if (CsfcD.CsfcB[j].bChk)
							{
								// 하중유발피로                

								int nCase_max = GetMaxRatioCase(CsfcD.CsfcB[j]);

								int nTopBot = (nCase_max == 0 || nCase_max == 1) ? 0 : 1;
								T_CSFC_LOAD &FLoadD = CsfcD.CsfcB[j].LoadR[nTopBot];

								if (FLoadD.ComForMV.nLcomSize > 0) MembLcomD.aChkFor.SetSize(FLoadD.ComForMV.nLcomSize);
								for (int i = 0; i < FLoadD.ComForMV.nLcomSize; ++i)
								{
									MembLcomD.aChkFor[i].nLcomType = 2;
									ConvertCsgCalcChkForce(FLoadD.ComForMV.ForMV[i], MembLcomD.aChkFor[i]);
								}

								CSG_FATI_CR_RSLT FatiCrResD;
								m_pDataCtrl->m_pCsgDataCtrl->Check_Fatigue(ElemK, k, MembLcomD, MembR, ChkResD, DtrR, FatiCrResD);
								RptData.FatigueD = ChkResD.pAASHTO->FatigueD;

								// AASHTO LRFD 12 STL COMPOSITE MQC 56, CSPARK (14.06.15)
								RptData.FatigueD.Fat_LoadInduceD.dGamma_f_top = FLoadD.dGammaDelF_top;
								RptData.FatigueD.Fat_LoadInduceD.dGamma_f_bot = FLoadD.dGammaDelF_bot;
								RptData.FatigueD.Fat_LoadInduceD.dMymax_pos = FLoadD.dMyMax_Pos;
								RptData.FatigueD.Fat_LoadInduceD.dMymax_nes = FLoadD.dMyMax_Neg;
								RptData.FatigueD.Fat_LoadInduceD.df_top_com_st = FLoadD.dftop_com_st;
								RptData.FatigueD.Fat_LoadInduceD.df_top_ten_st = FLoadD.dftop_ten_st;
								RptData.FatigueD.Fat_LoadInduceD.df_bot_com_st = FLoadD.dfbot_com_st;
								RptData.FatigueD.Fat_LoadInduceD.df_bot_ten_st = FLoadD.dfbot_ten_st;

								MembLcomD.aChkFor.RemoveAll();
								MembLcomD.aChkFor.SetSize(1);
								ConvertCsgCalcChkForce(CsfcD.CsfcB[j].ShearR.ComFor, MembLcomD.aChkFor[0]);
								FatiCrResD.Initialize();
								m_pDataCtrl->m_pCsgDataCtrl->Check_Fatigue(ElemK, k, MembLcomD, MembR, ChkResD, DtrR, FatiCrResD);
								RptData.FatigueD.Fat_WebD = ChkResD.pAASHTO->FatigueD.Fat_WebD;

								// Moment Lcom Name
								T_LCOM_D LcomD;
								if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, FLoadD.nOrgLcomK, LcomD))
								{
									CStringW strLcomName(LcomD.LoadCombName);
									RptData.FatigueD.aForceD[0].strLcomName = strLcomName;
								}
								RptData.FatigueD.aForceD[0].strLcomType = CRptSteelGirder::GetLoadCombType(CsfcD.CsfcB[j].LoadR[0].nftMax);
								RptData.FatigueD.aForceD[1].strLcomType = CRptSteelGirder::GetLoadCombType(CsfcD.CsfcB[j].LoadR[0].nfcMax);
								RptData.FatigueD.aForceD[2].strLcomType = CRptSteelGirder::GetLoadCombType(CsfcD.CsfcB[j].LoadR[1].nftMax);
								RptData.FatigueD.aForceD[3].strLcomType = CRptSteelGirder::GetLoadCombType(CsfcD.CsfcB[j].LoadR[1].nfcMax);

								// Shear Lcom Name
								if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, CsfcD.CsfcB[j].ShearR.nOrgLcomK, LcomD))
								{
									CStringW strLcomName(LcomD.LoadCombName);
									RptData.FatigueD.Fat_WebD.strLcomName = strLcomName;
								}
							}
						}
						MembLcomD.aChkFor.RemoveAll();
						MembLcomD.aChkFor.SetSize(1);

					}

					if (m_CpgdD.bDesignParameter[3])
					{
						// 시공성 - 휨 
						if (pCheckGirder->ReadSCBR_LSD12(ElemK, ScbrD))
						{
							int nNum = 0;
							double dRatioFirst = 0.0;
							int kk_max = 0;
							CSG_MEMB_RES_AASHTO_LRFD_CONSTRUCT  ConFirst;

							for (k = 0; k < 2; k++) // Positive/Negative
							{
								kk = j * 2 + k;
								if (!ScbrD.ScbrB[kk].bChk) continue;
								MembLcomD.Memb.ChIf.dFactorCS = ScbrD.ScbrB[kk].dFactorCS;
								MembLcomD.aChkFor[0].nLcomType = 3;
								ConvertCsgCalcUlmForce(ScbrD.ScbrB[kk].UlFor, MembLcomD.aUlmf[0]);
								ConvertCsgCalcUlmForce(ScbrD.ScbrB[kk].UlFor, MembLcomD.aUlmf[1]); // NC
								ConvertCsgCalcForce(ScbrD.ScbrB[kk].CsFor, MembLcomD.aChkFor[0].ForNC);
								MembLcomD.aUlmFor[0].dMmax = ScbrD.ScbrB[kk].UlFor.MmaxFor.GetMy();
								m_pDataCtrl->m_pCsgDataCtrl->Check_Constructibility(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);

								if (nNum == 0)   // 정/부 중 불리한 것 출력
								{
									ConFirst = ChkResD.pAASHTO->ConsD;
									dRatioFirst = ChkResD.pAASHTO->ConsD.FlexD.dRatio;

									RptData.ConsD = ChkResD.pAASHTO->ConsD;
									kk_max = kk;
								}
								else
								{
									if (dRatioFirst < ChkResD.pAASHTO->ConsD.FlexD.dRatio)
									{
										RptData.ConsD = ChkResD.pAASHTO->ConsD;
										kk_max = kk;
									}
									else
									{
										RptData.ConsD = ConFirst;
									}
								}

								nNum += 1;
							}

							CString strTempLcomName = Get_StageName(ScbrD.ScbrB[kk_max].nStageK);
							CStringW strWTempLcomName(strTempLcomName);
							RptData.ConsD.ForceD.strLcomName = strWTempLcomName;
							CString strTempType; strTempType.Format(_T("%d"), ScbrD.ScbrB[kk_max].nStepK);
							CStringW strWTempType(strTempType);
							RptData.ConsD.ForceD.strLcomType = strWTempType;

						}
						// 시공성 - 전단
						if (pCheckGirder->ReadSCVR_LSD12(ElemK, ScvrD))
						{
							if (ScvrD.ScvrB[j].bChk)  // 0 : I-max  1: J-max
							{
								MembLcomD.aChkFor[0].nLcomType = 3;
								ConvertCsgCalcUlmForce(ScvrD.ScvrB[j].UlFor, MembLcomD.aUlmf[0]);
								ConvertCsgCalcUlmForce(ScvrD.ScvrB[j].UlFor, MembLcomD.aUlmf[1]); // NC
								ConvertCsgCalcForce(ScvrD.ScvrB[j].CsFor, MembLcomD.aChkFor[0].ForNC);
								m_pDataCtrl->m_pCsgDataCtrl->Check_Constructibility(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);

								RptData.ConsD.ShearD = ChkResD.pAASHTO->ConsD.ShearD;
								CString strTempLcomName = Get_StageName(ScvrD.ScvrB[j].nStageK);
								CStringW strWTempLcomName(strTempLcomName);
								RptData.ConsD.ShearD.ForceD.strLcomName = strWTempLcomName;
								CString strTempN; strTempN.Format(_T("%d"), ScvrD.ScvrB[j].nStepK);
								CStringW strWTempN(strTempN);
								RptData.ConsD.ShearD.ForceD.strLcomType = strWTempN;
							}
						}
					}

					if (m_CpgdD.bDesignParameter[5])
					{
						// 전단연결재
						if (pCheckGirder->ReadCSCR_LSD12(ElemK, CscrD))
						{
							if ((j == 0 && bPrintI) || (j == 1 && bPrintJ))
							{
								if (CscrD.CscrB[j].bChk)  // 0 : I-max  1: J-max
								{
									MembLcomD.aChkFor[0].nLcomType = 0;
									MembLcomD.aChkFor[0].nMax = CscrD.CscrB[j].nMax;//!!!
									ChkResD.pAASHTO->ShearConnD.SC_PitchD.dSig = CscrD.CscrB[j].df_fat_B; // bottom flange 응력 범위
									ChkResD.pAASHTO->ShearConnD.SC_PitchD.dVst = CscrD.CscrB[j].dVst; // 전단 응력 범위

									if (CscrD.CscrB[j].LoadR[0].ComForMV.nLcomSize > 0)  MembLcomD.aChkFor.SetSize(CscrD.CscrB[j].LoadR[0].ComForMV.nLcomSize);
									for (int a = 0; a < CscrD.CscrB[j].LoadR[0].ComForMV.nLcomSize; ++a)
									{
										ConvertCsgCalcChkForce(CscrD.CscrB[j].LoadR[0].ComForMV.ForMV[a], MembLcomD.aChkFor[a]);
									}

									m_pDataCtrl->m_pCsgDataCtrl->Check_ShearConnector(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);
									RptData.ShearConnD = ChkResD.pAASHTO->ShearConnD;


									T_LCOM_D LcomD;
									if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, CscrD.CscrB[j].nOrgLcomK, LcomD))
									{
										RptData.ShearConnD.ForceD.strLcomName = CT2W(LcomD.LoadCombName);
										if (RptData.FatigueD.aForceD.GetSize() > 0) RptData.ShearConnD.SC_FatiD.aForceD[0].strLcomName = RptData.FatigueD.aForceD[0].strLcomName;
									}
									RptData.ShearConnD.ForceD.strLcomType = CRptSteelGirder::GetLoadCombType(CscrD.CscrB[j].nMax);
									if (RptData.FatigueD.aForceD.GetSize() > 0) RptData.ShearConnD.SC_FatiD.aForceD[0].strLcomType = RptData.FatigueD.aForceD[0].strLcomType;
								}
								else
								{
									RptData.ShearConnD.nElemNo = ElemK.first;
									RptData.ShearConnD.nPosiNo = j;
								}
							}
						}

						//수평보강재
						if (pCheckGirder->ReadCSSL_LSD12(ElemK, CsslD))
						{
							if ((j == 0 && bPrintI) || (j == 1 && bPrintJ))
							{
								if (CsslD.CsslB[j].bChk)  // 0 : I-max  1: J-max
								{
									MembLcomD.aChkFor[0].nLcomType = 0;
									MembLcomD.aChkFor[0].nMax = CsslD.CsslB[j].nMax;//!!!
									ConvertCsgCalcChkForce(CsslD.CsslB[j].ComFor, MembLcomD.aChkFor[0]);
									m_pDataCtrl->m_pCsgDataCtrl->Check_Stiffener(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);
									RptData.HS_D = ChkResD.pAASHTO->HS_D;
								}
								else
								{
									RptData.HS_D.nElemNo = ElemK.first;
									RptData.HS_D.nPosiNo = j;
								}
							}
						}

						//Bearing Stiffener
						if (pCheckGirder->ReadBSTF_LSD12(ElemK, BstfD))
						{
							if ((j == 0 && bPrintI) || (j == 1 && bPrintJ))
							{
								if (BstfD.BstfB[j].bChk)  // 0 : I-max  1: J-max
								{
									int nReacSize = 1;
									MembLcomD.aReac.SetSize(nReacSize);
									MembLcomD.aReac[0].dFz = BstfD.BstfB[j].dR;
									m_pDataCtrl->m_pCsgDataCtrl->Check_BearingStiffener(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);

									SetBstfPrintData(ElemK.first, j, BstfD.BstfB[j].nOrgLcomK, ChkResD.pAASHTO->BS_D);

									RptData.BS_D = ChkResD.pAASHTO->BS_D;
								}
							}
						}
					}

					SetRptData(ElemK.first, j, MembLcomD.Memb, RptData);

					RptData.StrengthD.bChkFlexure = m_CpgdD.bDesignParameter[0];
					RptData.StrengthD.bChkShear = m_CpgdD.bDesignParameter[1];
					RptData.ServiceD.bChkService = m_CpgdD.bDesignParameter[2];
					RptData.ServiceD_rpt.bChkService = m_CpgdD.bDesignParameter[2];
					RptData.ConsD.bChkCons = m_CpgdD.bDesignParameter[3];
					RptData.FatigueD.bChkFati = m_CpgdD.bDesignParameter[4];
					RptData.ShearConnD.bChkSC = m_CpgdD.bDesignParameter[5];
					RptData.HS_D.bChk = m_CpgdD.bDesignParameter[5];

					aInData.Add(MembLcomD);
					aRptData.Add(RptData);
				}
				//
				if (m_bStopExecute)	return FALSE;
				nProgressPercent += nProgressIncrementPercent;
				Progress(0, nProgressPercent);
				Progress(2, nProgressPercent);

			}
		}

		if (nNumPrtElem == 0)
		{
			CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // ")출력할 요소의 설계정보가 없습니다._T("
			AfxMessageBox(strErrMessage);
			return FALSE;
		}
		//RESULT-TABLE
		nProgressIncrementPercent = nPrintElemSize == 0 ? 30 : 30 / nPrintElemSize;
		int iPosiNo = 0;
		int iMaxMin = 0;

		// 계산서 출력
		CRptSteelGirder SteelGirder;
		CDgn_CSGRptManager_AASHTO12 myLSDCSGReport;

		myLSDCSGReport.Print_DetailReport(iDgnCode, m_pDoc->GetProgramPath(), SteelGirder.GetSaveFileDirEx(), strPath,
			aInData, aRptData);
	}
	catch (...)
	{
		bSuccess = FALSE;
	}

	if (MembR.pAASHTO != NULL) delete MembR.pAASHTO;

	if (ChkResD.pAASHTO != NULL) delete ChkResD.pAASHTO;

	return bSuccess;
}


int CRptSteelGirder_AASHTO12::GetMaxRatioCase(T_CSFC_BASE &CsfcB)
{
	int nCase_max = 0;
	int nNum = 0;
	double dLFRatio_max = 0.0;
	double dLFRatio = 0.0;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			int nForId = CsfcB.LoadR[i].nForId[j];
			dLFRatio = CsfcB.LoadR[i].ComForMV.ForMV[nForId].dLFRatio;
			if (nNum == 0)
			{
				dLFRatio_max = dLFRatio;
				nCase_max = nNum;
			}
			else
			{
				if (dLFRatio_max < dLFRatio)
				{
					dLFRatio_max = dLFRatio;;
					nCase_max = nNum;
				}
			}

			nNum += 1;
		}
	}

	return nCase_max;
}

void CRptSteelGirder_AASHTO12::SetBstfPrintData(int ElemK, int nPosi, UINT nOrgLcomK, CSG_MEMB_RES_AASHTO_LRFD_BEARING_STIFFENER_D& BstfResD)
{
	T_LCOM_D LcomD;
	if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, nOrgLcomK, LcomD))
	{
		CStringW strLcomNameW(LcomD.LoadCombName);
		BstfResD.strLcomName = strLcomNameW;
	}
	BstfResD.nElemNo = ElemK;
	BstfResD.nPosiNo = nPosi;  //I,J
	BstfResD.strMatName = _T("");

}


void CRptSteelGirder_AASHTO12::SetRptData(int ElemK, int nPosi, CSG_MEMB_POSD& InData, CSG_MEMB_RES_AASHTO_LRFD_D& RptData)
{
	RptData.Memb.nElem = ElemK;
	RptData.Memb.nPosi = nPosi;

	double dHw = 0.0;
	double dD = 0.0, dD2 = 0.0;
	double dtw = 0.0, dtw2 = 0.0;
	double dB1 = 0.0;
	double dtf1 = 0.0;
	double dB2 = 0.0;
	double dtf2 = 0.0;
	double dBf1 = 0.0;
	double dBf2 = 0.0;
	double dBf3 = 0.0;
	double dH = 0.0;
	double dbfc = 0.0, dbfc2 = 0.0;
	double dbft = 0.0;
	double dSg = 0.0, dStop = 0.0, dSbot = 0.0;

	int nSectType = InData.PosD[nPosi].Sect.SectInfo.nStype;
	switch(nSectType)
	{
		case D_SECT_TYPE_COMPO_I:
		{
			dHw  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[0];
			dtw  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[1];
			dB1  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[2];
			dtf1 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[3];
			dB2  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[4];
			dtf2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[5];
			dbfc = dB1;
			dbft = dB2;
			dD   = dHw;

			dStop = (InData.PosD[nPosi].Sect.SectInfo.Slab.Size[0] - dB1) * 0.5;
			dSbot = (InData.PosD[nPosi].Sect.SectInfo.Slab.Size[0] - dB2) * 0.5;

			break;
		}
		case D_SECT_TYPE_COMPO_STLG_I:
		{
			dHw  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[4];
			dtw  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[7];
			dB1  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[0] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[1];
			dtf1 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[5];
			dB2  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[2] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[3];
			dtf2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[6]; // 0.0이면 dtf1
			dbfc = dB1;
			dbft = dB2;

			const double dx1 = InData.PosD[nPosi].Sect.SectInfo.After.Size[4] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[0];
			const double dy1 = dtf2 + InData.PosD[nPosi].Sect.SectInfo.Before.Size[4];
			const double dx2 = InData.PosD[nPosi].Sect.SectInfo.After.Size[5] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[2];
			const double dy2 = dtf2;

			if (InData.PosD[nPosi].Sect.SectInfo.Before.nSymmType == 1)
			{
				dD = sqrt(pow(dx1 - dx2, 2.0) + pow(dy1 - dy2, 2.0));
			}
			else if (InData.PosD[nPosi].Sect.SectInfo.Before.nSymmType == 0)
			{
				dD = dHw;
			}
			else ASSERT(0);			

			dSg   = InData.PosD[nPosi].Sect.SectInfo.After.Size[3]; // for type2
			dStop = InData.PosD[nPosi].Sect.SectInfo.After.Size[4];
			dSbot = InData.PosD[nPosi].Sect.SectInfo.After.Size[5];

			break;
		}
		case D_SECT_TYPE_STLG_I:
		{
			dHw = InData.PosD[nPosi].Sect.SectInfo.Before.Size[4];
			dtw = InData.PosD[nPosi].Sect.SectInfo.Before.Size[7];
			dB1 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[0] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[1];
			dtf1 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[5];
			dB2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[2] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[3];
			dtf2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[6]; // 0.0이면 dtf1
			dbfc = dB1;
			dbft = dB2;

			const double dx1 = InData.PosD[nPosi].Sect.SectInfo.After.Size[4] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[0];
			const double dy1 = dtf2 + InData.PosD[nPosi].Sect.SectInfo.Before.Size[4];
			const double dx2 = InData.PosD[nPosi].Sect.SectInfo.After.Size[5] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[2];
			const double dy2 = dtf2;
			if (InData.PosD[nPosi].Sect.SectInfo.Before.nSymmType == 1)
			{
				dD = sqrt(pow(dx1 - dx2, 2.0) + pow(dy1 - dy2, 2.0));
			}
			else if (InData.PosD[nPosi].Sect.SectInfo.Before.nSymmType == 0)
			{
				dD = dHw;
			}
			else ASSERT(0);
			

			dSg = InData.PosD[nPosi].Sect.SectInfo.After.Size[3]; // for type2
			dStop = InData.PosD[nPosi].Sect.SectInfo.After.Size[4];
			dSbot = InData.PosD[nPosi].Sect.SectInfo.After.Size[5];

			break;
		}
		case D_SECT_TYPE_COMPO_B:
		{
			dHw  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[0];
			dtw  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[1];
			dB1  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[2];
			dBf1 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[3];
			dtf1 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[4];
			dB2  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[5];
			dBf2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[6];
			dtf2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[7];
			dbfc = dB1 + dBf1 * 2.0;
			dbft = dB2 + dBf2 * 2.0;

			double db1 = 0.5*(dB1 - dB2);
			dD = (fabs(db1) < cDGN_Zero) ? dHw : sqrt(dHw * dHw + db1 * db1);

			dStop = (InData.PosD[nPosi].Sect.SectInfo.Slab.Size[0] - dB1) * 0.5 - dBf1;
			dSbot = (InData.PosD[nPosi].Sect.SectInfo.Slab.Size[0] - dB2) * 0.5 - dBf2;

			break;
		}
		case D_SECT_TYPE_COMPO_TUB:
		{
			dHw  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[0];
			dtw  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[1];
			dB1  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[2];
			dBf1 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[3];
			dtf1 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[4];
			dB2  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[5];
			dBf2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[6];
			dtf2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[7];
			dBf3 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[8];
			dbfc = dBf1;
			dbft = dB2 + dBf2 * 2.0;

			double db1 = 0.5*(dB1 + 2.0*(dBf1 - dBf3) - dB2);
			dD = (fabs(db1) < cDGN_Zero) ? dHw : sqrt(dHw * dHw + db1 * db1);

			dStop = (InData.PosD[nPosi].Sect.SectInfo.Slab.Size[0] - dB1) * 0.5 - dBf1;
			dSbot = (InData.PosD[nPosi].Sect.SectInfo.Slab.Size[0] - dB2) * 0.5 - dBf2;

			break;
		}
		case D_SECT_TYPE_COMPO_STLG_B:
		{
			dHw  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[6];
			dB1  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[1];
			dtf1 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[7];
			dB2  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[4];			
			dtf2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[8];
			dbfc = InData.PosD[nPosi].Sect.SectInfo.Before.Size[0] +
				InData.PosD[nPosi].Sect.SectInfo.Before.Size[1] +
				InData.PosD[nPosi].Sect.SectInfo.Before.Size[2];
			dbft = InData.PosD[nPosi].Sect.SectInfo.Before.Size[3] +
				InData.PosD[nPosi].Sect.SectInfo.Before.Size[4] +
				InData.PosD[nPosi].Sect.SectInfo.Before.Size[5];

			double dx1 = InData.PosD[nPosi].Sect.SectInfo.After.Size[4] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[0];
			double dx2 = InData.PosD[nPosi].Sect.SectInfo.After.Size[5] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[3];
			dD  = EQ(dx1, dx2) ? dHw : hypot(dHw, (dx1-dx2));
			dtw = InData.PosD[nPosi].Sect.SectInfo.Before.Size[9];

			dx1 += InData.PosD[nPosi].Sect.SectInfo.Before.Size[1];
			dx2 += InData.PosD[nPosi].Sect.SectInfo.Before.Size[4];
			dD2  = EQ(dx1, dx2) ? dHw : hypot(dHw, (dx1-dx2));
			dtw2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[10];

			dSg   = InData.PosD[nPosi].Sect.SectInfo.After.Size[3]; // for type2
			dStop = InData.PosD[nPosi].Sect.SectInfo.After.Size[4];
			dSbot = InData.PosD[nPosi].Sect.SectInfo.After.Size[5];

			break;
		}
		case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			dHw  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[6];
			dB1  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[1];
			dtf1 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[7];
			dB2  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[4];
			dtf2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[8];
			
			dbft = InData.PosD[nPosi].Sect.SectInfo.Before.Size[3] +
				InData.PosD[nPosi].Sect.SectInfo.Before.Size[4] +
				InData.PosD[nPosi].Sect.SectInfo.Before.Size[5];

			dtw  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[9]; // 9, 10
			dtw2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[10];

			double dx1 = InData.PosD[nPosi].Sect.SectInfo.After.Size[4] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[11] - 0.5*dtw;
			double dx2 = InData.PosD[nPosi].Sect.SectInfo.After.Size[5] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[3] - 0.5*dtw;
			dD = EQ(dx1, dx2) ? dHw : hypot(dHw, (dx1-dx2));
			
			dx1 = InData.PosD[nPosi].Sect.SectInfo.After.Size[4] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[0] 
				+ InData.PosD[nPosi].Sect.SectInfo.Before.Size[1] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[2]
				- InData.PosD[nPosi].Sect.SectInfo.Before.Size[12] + 0.5*dtw2;
			dx2 = InData.PosD[nPosi].Sect.SectInfo.After.Size[5] + InData.PosD[nPosi].Sect.SectInfo.Before.Size[3]
				+ InData.PosD[nPosi].Sect.SectInfo.Before.Size[4] + 0.5*dtw2;
			dD2 = EQ(dx1, dx2) ? dHw : hypot(dHw, (dx1-dx2));
			
			
			dbfc  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[0];
			dbfc2 = InData.PosD[nPosi].Sect.SectInfo.Before.Size[2];

			dSg   = InData.PosD[nPosi].Sect.SectInfo.After.Size[3]; // for type2
			dStop = InData.PosD[nPosi].Sect.SectInfo.After.Size[4];
			dSbot = InData.PosD[nPosi].Sect.SectInfo.After.Size[5];

			break;
		}
		default: ASSERT(0); break;
	}

	// 슬래브 재원                                                 
	RptData.Memb.dBs = InData.PosD[nPosi].Sect.SectInfo.Slab.Size[0];
	RptData.Memb.dts = InData.PosD[nPosi].Sect.SectInfo.Slab.Size[1];
	RptData.Memb.dth = InData.PosD[nPosi].Sect.SectInfo.Slab.Size[2] + dtf1;
	RptData.Memb.dfck = InData.Matl.MatSlab.RC.dFck; // _T("fck")   
	RptData.Memb.dEc = InData.Matl.MatSlab.RC.dEc; // ")Ec_T("    
	double dA = 0.0;
	for (int i = 0; i < InData.PosD[nPosi].Rbar.arRbarPosi.GetSize(); ++i)
	{
		dA += InData.PosD[nPosi].Rbar.arRbarPosi[i].dArea;
	}
	RptData.Memb.dAr = dA;  // ")Ar_T("    
	RptData.Memb.dFyr = InData.Matl.MatSlab.RC.dFy; // ")Fyr_T("   //!!!
	RptData.Memb.dSg  = dSg; // for type2

	int nDgnCode = m_pDoc->GetCivilCodeDgn();

	// 거더-단면
	RptData.Memb.GIRDER.dbfc = dbfc;              // ")bfc_T("     
	RptData.Memb.GIRDER.dbfc2= dbfc2;              // ")bfc_T("     
	RptData.Memb.GIRDER.dbft = dbft;              // ")bft_T("     
	RptData.Memb.GIRDER.dtfc = dtf1;              // ")tfc_T("     
	RptData.Memb.GIRDER.dtft = dtf2;              // ")tft_T("     
	RptData.Memb.GIRDER.dD   = dD;                // ")D_T("       
	RptData.Memb.GIRDER.dtw  = dtw;               // ")tw_T("      
	RptData.Memb.GIRDER.dD2  = dD2;                // ")D_T("       
	RptData.Memb.GIRDER.dtw2 = dtw2;               // ")tw_T("      
	RptData.Memb.GIRDER.dH   = dHw + dtf1 + dtf2; // ")H_T("  
	RptData.Memb.GIRDER.dStop = dStop;
	RptData.Memb.GIRDER.dSbot = dSbot;

	// 거더-상부플랜지
	RptData.Memb.GIRDER.Top.strMatType = InData.Matl.MatStf.csMatlName; // ")Top_mat_T(" 
	RptData.Memb.GIRDER.Top.dthick = dtf1;// ")Top_thk_T(" 
	RptData.Memb.GIRDER.Top.dfy_or = InData.Matl.MatStf.Steel.S_Fy1; // ")Top_fyo_T(" 
	RptData.Memb.GIRDER.Top.dfy = InData.Matl.MatStf.Steel.S_Fy; // ")Top_fy_T("  판폭두께비에 따른 fy로 결과구조체에서 채울값.
	RptData.Memb.GIRDER.Top.dfu = InData.Matl.MatStf.Steel.S_Fu; // ")Top_fu_T("  
	RptData.Memb.GIRDER.Top.strMemo = GetThicknessString(nDgnCode, dtf1);

	// 거더-하부플랜지
	RptData.Memb.GIRDER.Bottom.strMatType = InData.Matl.MatSbf.csMatlName; // ")Bot_mat_T(" 
	RptData.Memb.GIRDER.Bottom.dthick = dtf2;// ")Bot_thk_T(" 
	RptData.Memb.GIRDER.Bottom.dfy_or = InData.Matl.MatSbf.Steel.S_Fy1; // ")Bot_fyo_T(" 
	RptData.Memb.GIRDER.Bottom.dfy = InData.Matl.MatSbf.Steel.S_Fy; // ")Bot_fy_T("  판폭두께비에 따른 fy로 결과구조체에서 채울
	RptData.Memb.GIRDER.Bottom.dfu = InData.Matl.MatSbf.Steel.S_Fu; // ")Bot_fu_T("  
	RptData.Memb.GIRDER.Bottom.strMemo = GetThicknessString(nDgnCode, dtf2);

	// 거더-웹 
	RptData.Memb.GIRDER.Web.strMatType = InData.Matl.MatSw.csMatlName; // ")Web_mat_T(" 
	RptData.Memb.GIRDER.Web.dthick  = dtw;// ")Web_thk_T(" 
	RptData.Memb.GIRDER.Web.dthick2 = dtw2;// ")Web_thk_T(" 
	RptData.Memb.GIRDER.Web.dfy_or  = InData.Matl.MatSw.Steel.S_Fy1; // ")Web_fyo_T(" 
	RptData.Memb.GIRDER.Web.dfy     = InData.Matl.MatSw.Steel.S_Fy; // ")Web_fy_T("  판폭두께비에 따른 fy로 결과구조체에서 채울
	RptData.Memb.GIRDER.Web.dfu     = InData.Matl.MatSw.Steel.S_Fu; // ")Web_fu_T("  
	RptData.Memb.GIRDER.Web.strMemo = GetThicknessString(nDgnCode, dtw);
	RptData.Memb.GIRDER.Web.strMemo2= GetThicknessString(nDgnCode, dtw2);

	// 거더- 설계강도
	RptData.Memb.GIRDER.dFyc = InData.Matl.MatStf.Steel.S_Fy; //")Fyc_T("    
	RptData.Memb.GIRDER.dFyw = InData.Matl.MatSw.Steel.S_Fy; //")Fyw_T("    
	RptData.Memb.GIRDER.dFyw2= InData.Matl.MatSw.Steel.S_Fy2; //")Fyw_T("    
	RptData.Memb.GIRDER.dFyt = InData.Matl.MatSbf.Steel.S_Fy; //")Fyt_T("    
	RptData.Memb.GIRDER.dEs = InData.Matl.MatStf.Steel.Elast; //")Es_T("  

	// Transverse Stiffener
	RptData.Memb.STIFFENER_V.bUseStiff = InData.PosD[nPosi].Trst.bUseStifWeb;
	RptData.Memb.STIFFENER_V.nShapeType = InData.PosD[nPosi].Trst.nStiffenerTypeWeb;
	RptData.Memb.STIFFENER_V.nType = InData.PosD[nPosi].Trst.nStifNumWeb;
	RptData.Memb.STIFFENER_V.ddo = InData.PosD[nPosi].Trst.dPitchWeb;
	RptData.Memb.STIFFENER_V.dhi = InData.PosD[nPosi].Trst.dSizeWeb[0];
	RptData.Memb.STIFFENER_V.dbi = InData.PosD[nPosi].Trst.dSizeWeb[1];
	RptData.Memb.STIFFENER_V.dtw = InData.PosD[nPosi].Trst.dSizeWeb[2];
	RptData.Memb.STIFFENER_V.dti = InData.PosD[nPosi].Trst.dSizeWeb[3];
	RptData.Memb.STIFFENER_V.dfy = InData.PosD[nPosi].Trst.dFyWeb;

	// Longitudinal Stiffener


	/////
	// 단면계수- 강재단면
	SetRptData_SectStiffness(nPosi, InData.PosD[nPosi].Sect.SectInfo.Before.Stiffness, RptData.Memb.Steel);
	// 단면계수- 단기합성단면
	SetRptData_SectStiffness(nPosi, InData.PosD[nPosi].Sect.RbarStiffn, RptData.Memb.ShortComp);
	// 단면계수- 장기합성단면
	SetRptData_SectStiffness(nPosi, InData.PosD[nPosi].Sect.RbarStiff3n, RptData.Memb.LongComp);
	// 단면계수- 강재단면(부머모멘트)
	SetRptData_SectStiffness(nPosi, InData.PosD[nPosi].Sect.NonSlabStiff, RptData.Memb.Steel_M);
	// 단면계수- 강재+철근(부모멘트)
	SetRptData_SectStiffness(nPosi, InData.PosD[nPosi].Sect.NonSlabStiff2, RptData.Memb.SteelRebarComp);
}


void CRptSteelGirder_AASHTO12::SetRptData_SectStiffness(int nPosi, DGN_SECT_STIFFNESS & InData, CSG_SectionProperty& RptData)
{
	double dIny = InData.dIyy;
	double dZp = InData.dCzp;
	double dZm = InData.dCzm;
	RptData.dA = InData.dArea;                   // ")A_T("    
	RptData.dINA = InData.dIyy;                    // _T("I")
	RptData.dIzz = InData.dIzz;                    // ")Iz_T("   
	RptData.ddTop = InData.dCzp;                    // ")dtop_T(" 
	RptData.dSTop = (fabs(dZp - 0.0) < CSG_Zero) ? 0.0 : fabs(dIny / dZp);     // ")Stop_T(" 
	RptData.ddBot = InData.dCzm;                    // ")dbot_T(" 
	RptData.dSBot = (fabs(dZm - 0.0) < CSG_Zero || dZm < 0.0) ? 0.0 : dIny / dZm;     // ")Sbot_T(" 
	RptData.dSlt = (fabs(InData.dCym) < CSG_Zero) ? 0.0 : InData.dIzz / InData.dCym;
	RptData.dSrt = (fabs(InData.dCyp) < CSG_Zero) ? 0.0 : InData.dIzz / InData.dCyp;
}


BOOL CRptSteelGirder_AASHTO12::ShouldCheckPosiIJ(int nFlag, BOOL& bCheckI, BOOL& bCheckJ)
{

	if (nFlag == 0) { bCheckI = TRUE; bCheckJ = FALSE; }	// I.
	else if (nFlag == 1) { bCheckI = FALSE; bCheckJ = TRUE; }	// J.
	else if (nFlag == 2) { bCheckI = TRUE; bCheckJ = TRUE; }	// I&J.
	else	              ASSERT(0);

	return TRUE;
}

void CRptSteelGirder_AASHTO12::ConvertCsgCalcUlmForce(T_CSGD_ULMF &For, CSG_ULMF_D &rData)
{
	ConvertCsgCalcForce(For.For[0], rData.ForD[0]);
	ConvertCsgCalcForce(For.For[1], rData.ForD[1]);
	ConvertCsgCalcForce(For.For[2], rData.ForD[2]);
	ConvertCsgCalcForce(For.For13[0], rData.For13D[0]);
	ConvertCsgCalcForce(For.For13[1], rData.For13D[1]);
	ConvertCsgCalcForce(For.CbFor[0], rData.CbForce[0]);
	ConvertCsgCalcForce(For.CbFor[1], rData.CbForce[1]);
	ConvertCsgCalcForce(For.CbFor[2], rData.CbForce[2]);
}

void CRptSteelGirder_AASHTO12::ConvertCsgCalcChkForce(T_CSGD_COMF &For, CSG_FORCE_COMP &rData)
{
	rData.nLcomSub = For.nFatigueI;
	ConvertCsgCalcForce(For.ForNC, rData.ForNC);
	ConvertCsgCalcForce(For.ForLT, rData.ForLT);
	ConvertCsgCalcForce(For.ForST, rData.ForST);
	ConvertCsgCalcForce(For.ForLC, rData.ForLC);
}

void CRptSteelGirder_AASHTO12::ConvertCsgCalcForce(T_CSGD_FORCE &For, CSG_FORCE_D &rData)
{
	rData.dForce[0] = For.dForce[0];
	rData.dForce[1] = For.dForce[1];
	rData.dForce[2] = For.dForce[2];
	rData.dForce[3] = For.dForce[3];
	rData.dForce[4] = For.dForce[4];
	rData.dForce[5] = For.dForce[5];
	rData.dForce[6] = For.dForce[6];
	rData.dForce[7] = For.dForce[7];
	rData.dForce[8] = For.dForce[8];
}

CString CRptSteelGirder_AASHTO12::Get_StageName(T_STAG_K StagK)
{
	CString strStageName = _T("");
	T_STAG_D StagD;
	if (m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD)) strStageName = StagD.StageName;

	return strStageName;
}

CString CRptSteelGirder_AASHTO12::GetThicknessString(int nDgnCode, double dt)
{
	CString str = _T("");
	if ( nDgnCode == KDS_24_14_31_2018_CSG )
	{
		str = GetThicknessKDS(dt);
	}

	return str;
}

CString CRptSteelGirder_AASHTO12::GetThicknessKDS(double dt)
{
	CString str = _T("");
	if ( LE(dt, 16.0) )
	{
		str = _LS(IDS_DGN_RPT_STL_Thick_16_below);
	}
	else if ( LE(dt, 40.0) )
	{
		str = _LS(IDS_DGN_RPT_STL_Thick_16_excess_40_below);
	}
	else if ( LE(dt, 75.0) )
	{
		str = _LS(IDS_DGN_RPT_STL_Thick_40_excess_75_below);
	}
	else if ( LE(dt, 100.0) )
	{
		str = _LS(IDS_DGN_RPT_STL_Thick_75_excess_100_below);
	}
	else if ( MT(dt, 100.0) )
	{
		str = _LS(IDS_DGN_RPT_STL_Thick_100_excess);
	}

	return str;
}
