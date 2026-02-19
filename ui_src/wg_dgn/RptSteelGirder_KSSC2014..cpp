
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "RptSteelGirder_KSSC2014.h"
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
// 
#include "Dgn_CSGRptManager_KSSC2014.h"
#include "DgnCsgDataCtrl.h"
#include "DgnPscCommon.h"
#include "DgnDataCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptSteelGirder_KSSC2014::CRptSteelGirder_KSSC2014()
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

CRptSteelGirder_KSSC2014::~CRptSteelGirder_KSSC2014()
{
	if (m_pDataCtrl)
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
	}
}

BOOL CRptSteelGirder_KSSC2014::Execute_CSGReport_KSSC2014(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
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

	if (!m_CpgdD.bPrtOptSum && !m_CpgdD.bPrtOptDetail)
	{
		AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CHECK));  // _T("....   <- PCS 여기에 출력 옶션 없는 경우 출력 안하는 메세지 추가 할 것
		return FALSE;
	}

	if (!m_CpgdD.bDesignParameter[0] && !m_CpgdD.bDesignParameter[1] && !m_CpgdD.bDesignParameter[2] &&
		!m_CpgdD.bDesignParameter[3] && !m_CpgdD.bDesignParameter[4] && !m_CpgdD.bDesignParameter[5] &&
		!m_CpgdD.bDesignParameter[6])
	{
		if (m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CHECK));  // ")선택된 출력변수가 없습니다.
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

		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("출력할 요소의 설계정보가 없습니다.")
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
		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("출력할 요소의 설계정보가 없습니다.")
		AfxMessageBox(strErrMessage);
		return FALSE;
	}

	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40 / nPrintElemSize;

	// Input Data
	CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>   aInData;   aInData.RemoveAll();
	CSG_MEMB_POSD_KSCE_LSD    MembLcomD;

	// Output Data
	CArray<CSG_MEMB_RES_AASHTO_LRFD_D, CSG_MEMB_RES_AASHTO_LRFD_D&>  aRptData;  aRptData.RemoveAll();
	CSG_MEMB_RES_AASHTO_LRFD_D  RptData;


	CSG_MEMB_RESULT_D      MembR;
	CSG_CHECK_RESULT_CODES ChkResD;

	MembR.pAASHTO = new CSG_MEMBPOS_RES_AASHTO_LRFD_D;
	MembR.pLSD = new CSG_MEMBPOS_RES_KSCE_LSD12_D;

	ChkResD.pAASHTO = new CSG_MEMB_RES_AASHTO_LRFD_D;
	ChkResD.pLSD = new CSG_CHECK_RES_KSCE_LSD12;

	int i = 0; // 요소 
	int j = 0; // I/J
	int k = 0; // Positive / Negative
	int kk = 0;
	int nIJ = 0, nMaxMin = 0;

	m_pDataCtrl->m_pCsgDataCtrl->InitialData(iDgnCode);

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
			m_pDataCtrl->m_pCsgDataCtrl->ConvertCGSGlobalData(CpgdD, MembLcomD.Memb.Csgd);

			int nConsiderPv = MembLcomD.Memb.Csgd.nConsiderPv; // 0:직선, 1:곡선
			BOOL bCurved = (nConsiderPv == 0) ? FALSE : TRUE;

			// Pre-Combined Composite Bridge
			BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb();
			CStageInfo *pStageInfo = m_pDoc->m_pPostCtrl->GetStageInfo();
			CArray<T_STAG_K, T_STAG_K> aStagK;
			pStageInfo->GetResultSavedStagKeyList(aStagK);
			int nStageSize = aStagK.GetSize();
			if (nStageSize == 0 && bIsPLCB == TRUE)
			{
				MembLcomD.Memb.ChIf.bIsPLCB = TRUE;
			}

			for (i = 0; i < nPrintElemSize; ++i)
			{
				auto  ElemK = aPrintElemPairKey[i];

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
					CgdvD=CgdpD;
					if (!m_pDoc->m_pAttrCtrl2->GetCgrv(ElemK.first, CgrvD)) continue;
					CgrvD=CgreD;

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


				if (!m_pDataCtrl->m_pCsgDataCtrl->m_mCsgMatl.Lookup(nMatlK, MembLcomD.Memb.Matl))
				{
					continue;
					//MembLcomD.Memb.Matl.Initialize();
				}
				nNumPrtElem += 1;

				nNumPrtElem += 1;
				MembLcomD.Memb.ChIf.bCurved = bCurved;

				//         int nNode[2] ={0, 0};
				//         nNode[0] = ElemD.elnod[0]; 
				//         nNode[1] = ElemD.elnod[1];
				//         BOOL bSupprotNode[2] ={FALSE, FALSE};
				//         bSupprotNode[0]  = m_pDataCtrl->m_pCsgDataCtrl->CheckSupprotNode(nNode[0]);
				//         bSupprotNode[1]  = m_pDataCtrl->m_pCsgDataCtrl->CheckSupprotNode(nNode[1]);

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
				else
				{
					for (int nSectPos = 0; nSectPos < 2; ++nSectPos)
					{
						MembLcomD.Memb.PosD[nSectPos].Sect.Initialize();
						MembLcomD.Memb.PosD[nSectPos].bSupprotNode = FALSE;
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

				// 3. Output Data Setting(1)
				RptData.Initialize();
				//CSG_MEMB_RES_AASHTO_LRFD_D MembResD;
				//m_pDataCtrl->m_pCsgDataCtrl->MakeMemberResDataAASHTO(ElemK, MembLcomD, MembResD);   
				//RptData = MembResD;


				T_SCBR_D  ScbrD;
				T_SCVR_D  ScvrD;

				T_CSBR_D CsbrD;
				T_CSVR_D CsvrD;
				T_CSAC_D CsacD;
				T_CSFC_D CsfcD;
				T_CSCR_D CscrD;
				T_CSSL_D CsslD;

				//CSG_MEMB_RES_AASHTO_LRFD_STR_D         StrnResD;
				//CSG_MEMB_RES_AASHTO_LRFD_SER_D         ServResD;
				//CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D     FatiResD;
				//CSG_MEMB_RES_AASHTO_LRFD_CONSTRUCT     ConsResD;
				//CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D  CscrResD; // 전단연결재
				//CSG_MEMB_RES_AASHTO_LRFD_HS_LS_D       CsslResD; // 수평보강재

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
								ConvertCsgCalcChkForce(CsbrD.CsbrB[kk].ComFor, MembLcomD.aChkFor[0]);
								m_pDataCtrl->m_pCsgDataCtrl->Check_Strength(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);
								RptData.StrengthD.FlexD[k] = ChkResD.pAASHTO->StrengthD.FlexD[k]; //StrnResD.FlexD[k];
								T_LCOM_D LcomD;
								if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, CsbrD.CsbrB[kk].nOrgLcomK, LcomD))
								{
									RptData.StrengthD.FlexD[k].ForceD.strLcomName = CT2W(LcomD.LoadCombName);
								}
								RptData.StrengthD.FlexD[k].ForceD.strLcomType = CRptSteelGirder::GetLoadCombType(CsbrD.CsbrB[kk].nMax);
								RptData.StrengthD.SectPropD[k] = ChkResD.pAASHTO->StrengthD.SectPropD[k]; //StrnResD.SectPropD[k];
								RptData.RhD = MembR.pAASHTO->MembR[j].RhD; // MembResD.RhD;
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
								//RptData.RhD = MembResD.RhD;
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

								// 								if     (nCase_max==0) ConvertCsgCalcChkForce(CsfcD.CsfcB[j].LoadR[0].ComFor[0], MembLcomD.aChkFor[0]);
								// 								else if(nCase_max==1) ConvertCsgCalcChkForce(CsfcD.CsfcB[j].LoadR[0].ComFor[1], MembLcomD.aChkFor[0]);
								// 								else if(nCase_max==2) ConvertCsgCalcChkForce(CsfcD.CsfcB[j].LoadR[1].ComFor[0], MembLcomD.aChkFor[0]);
								// 								else if(nCase_max==3) ConvertCsgCalcChkForce(CsfcD.CsfcB[j].LoadR[1].ComFor[1], MembLcomD.aChkFor[0]);
								// 								else ASSERT(0);
								CSG_FATI_CR_RSLT FatiCrResD;
								m_pDataCtrl->m_pCsgDataCtrl->Check_Fatigue(ElemK, k, MembLcomD, MembR, ChkResD, DtrR, FatiCrResD);
								//RptData.FatigueD.aForceD.SetSize(1);
								RptData.FatigueD = ChkResD.pAASHTO->FatigueD; //FatiResD;

								// AASHTO LRFD 12 STL COMPOSITE MQC 56, CSPARK (14.06.15)
								RptData.FatigueD.Fat_LoadInduceD.dGamma_f_top = FLoadD.dGammaDelF_top;
								RptData.FatigueD.Fat_LoadInduceD.dGamma_f_bot = FLoadD.dGammaDelF_bot;
								RptData.FatigueD.Fat_LoadInduceD.dMymax_pos = FLoadD.dMyMax_Pos;
								RptData.FatigueD.Fat_LoadInduceD.dMymax_nes = FLoadD.dMyMax_Neg;
								RptData.FatigueD.Fat_LoadInduceD.df_top_com_st = FLoadD.dftop_com_st;
								RptData.FatigueD.Fat_LoadInduceD.df_top_ten_st = FLoadD.dftop_ten_st;
								RptData.FatigueD.Fat_LoadInduceD.df_bot_com_st = FLoadD.dfbot_com_st;
								RptData.FatigueD.Fat_LoadInduceD.df_bot_ten_st = FLoadD.dfbot_ten_st;

								//RptData.RhD      = MembResD.RhD;

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
									RptData.FatigueD.aForceD[0].strLcomName = CT2W(LcomD.LoadCombName);
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
								ConvertCsgCalcForce(ScbrD.ScbrB[kk].CsFor, MembLcomD.aChkFor[0].ForNC);
								m_pDataCtrl->m_pCsgDataCtrl->Check_Constructibility(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);

								if (nNum == 0)   // 정/부 중 불리한 것 출력
								{
									ConFirst = ChkResD.pAASHTO->ConsD; // ConsResD;
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
										RptData.ShearConnD.SC_FatiD.aForceD[0].strLcomName = RptData.FatigueD.aForceD[0].strLcomName;
									}
									RptData.ShearConnD.ForceD.strLcomType = CRptSteelGirder::GetLoadCombType(CscrD.CscrB[j].nMax);
									RptData.ShearConnD.SC_FatiD.aForceD[0].strLcomType = RptData.FatigueD.aForceD[0].strLcomType;
									//RptData.RhD = MembResD.RhD;
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
									RptData.HS_D = ChkResD.pAASHTO->HS_D; //CsslResD;
									//RptData.RhD = MembResD.RhD;
								}
								else
								{
									RptData.HS_D.nElemNo = ElemK.first;
									RptData.HS_D.nPosiNo = j;
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
			CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("출력할 요소의 설계정보가 없습니다.")
			AfxMessageBox(strErrMessage);
			return FALSE;
		}
		//RESULT-TABLE
		nProgressIncrementPercent = nPrintElemSize == 0 ? 30 : 30 / nPrintElemSize;
		int iPosiNo = 0;
		int iMaxMin = 0;
		/*
		for(i=0; i<nPrintElemSize; i++)
		{
			T_ELEM_K ElemK = arPrintElemKey[i];

			//Detail Report의 변수는 Calc의 변수를 가져와야 함.
			//Result Table의 변수는 Civil내 저장된 변수를 가져와야 함.
			PosxD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetPosx(ElemK, PosxD)) {}

			// 데이터 채우기...


			//
			if(m_bStopExecute)	return FALSE;
			nProgressPercent += nProgressIncrementPercent;
			//ProDlg.Progress(0, nProgressPercent);
			//ProDlg.Progress(2, nProgressPercent);
		}
		*/

		// 계산서 출력
		CRptSteelGirder SteelGirder;
		CDgn_CSGRptManager_KSSC2014 myLSDCSGReport;

		myLSDCSGReport.Print_DetailReport(iDgnCode, m_pDoc->GetProgramPath(), SteelGirder.GetSaveFileDirEx(), strPath/*SteelGirder.GetSaveFileNameEx(FALSE)*/,
			aInData, aRptData/*, aFysgSNiP, aFycmSNiP, aFpsgSNiP, aFpmsSNiP, aFypcSNiP*/);


	}
	catch (...)
	{
		bSuccess = FALSE;
	}

	if (MembR.pAASHTO != NULL) delete MembR.pAASHTO;
	if (MembR.pLSD != NULL) delete MembR.pLSD;

	if (ChkResD.pAASHTO != NULL) delete ChkResD.pAASHTO;
	if (ChkResD.pLSD != NULL) delete ChkResD.pLSD;

	return bSuccess;
}