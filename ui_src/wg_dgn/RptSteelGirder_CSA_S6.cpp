
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "RptSteelGirder_CSA_S6.h"
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

#include "Dgn_CSGRptManager_CSA_S6.h"
#include "DgnCsgDataCtrl.h"
#include "DgnPscCommon.h"
#include "DgnDataCtrl.h"
#include "DgnCodeCtrl.h"

CRptSteelGirder_CSA_S6::CRptSteelGirder_CSA_S6()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;

	m_CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(m_CpgdD);
	int nDgnCode = m_CpgdD.iDgnCode;
	CDgnCodeCtrl::GetDefaultCpgdFactor(nDgnCode, m_CpgdD.nSubCode, m_CpgdD);

	m_pDoc->SetCivilCodeDgn(nDgnCode);
	m_pDataCtrl = new CCRCDataCtrl();
}

CRptSteelGirder_CSA_S6::~CRptSteelGirder_CSA_S6()
{
	if (m_pDataCtrl)
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
	}
}

BOOL CRptSteelGirder_CSA_S6::Execute_CSGReport_CSA_S6(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
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

		AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CHECK));  // "선택된 출력변수가 없습니다.
		return FALSE;
	}

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	CArray<T_CGRE_K, T_CGRE_K> aPrintElemKey;
	m_pDoc->m_pAttrCtrl2->GetCgreKeyList(aPrintElemKey);
	
	CArray<T_CGRV_K, T_CGRV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl2->GetCgrvKeyList(aPrintVBeamKey);

	ArrElemPairKey aRptElemPairK;
	CDBLib::ConvertToElemPairKeyList(aPrintElemKey, aPrintVBeamKey, aRptElemPairK);

	int nPrintElemSize = aRptElemPairK.GetSize();
	if (nPrintElemSize == 0)
	{
		if (m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // "출력할 요소의 설계정보가 없습니다."
		AfxMessageBox(strErrMessage);

		return FALSE;
	}

	// Span 정보 계산 
	m_pDataCtrl->Calc_LengthPosition4Span(aRptElemPairK);

	// 설계요소와 출력요소의 교집합이 존재할 경우 Report과정을 수행함. 그렇지않다면, Message처리함.
	BOOL bIsExistPrintElem = FALSE;
	CRptPlateGirder_EC4 PlateGirder_EC4;
	for (int nNum = 0; nNum < nPrintElemSize; ++nNum)
	{
		auto ElemK = aRptElemPairK[nNum];
		if (PlateGirder_EC4.Get_CheckResultAndPrintElemPositionType(ElemK))
		{
			bIsExistPrintElem = TRUE;
			break;
		}
	}
	if (!bIsExistPrintElem)
	{
		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // "출력할 요소의 설계정보가 없습니다."
		AfxMessageBox(strErrMessage);
		return FALSE;
	}

	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40 / nPrintElemSize;

	// Input Data
	CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>   aInData;   aInData.RemoveAll();
	CSG_MEMB_POSD_KSCE_LSD    MembLcomD;

	// Output Data
	CArray<CSG_RPT_RES_CSA_14, CSG_RPT_RES_CSA_14&>  aRptData;  aRptData.RemoveAll();
	CSG_RPT_RES_CSA_14  RptData;

	CSG_MEMB_RESULT_D      MembR;
	CSG_CHECK_RESULT_CODES ChkResD;

	MembR.pCSA = new CSG_MEMBPOS_RES_CSA_S6_D;

	ChkResD.pCSA = new CSG_CHECK_RES_CSA_S6;

	// Lcom Data
	//CSG_CHECK_RES_CSA_S6 rData;
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
			CDgnCodeCtrl::GetDefaultCpgdFactor(CpgdD.iDgnCode, CpgdD.nSubCode, CpgdD);

			// Pre-Combined Composite Bridge
			BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb();
			CStageInfo *pStageInfo = m_pDoc->m_pPostCtrl->GetStageInfo();
			CArray<T_STAG_K, T_STAG_K> aStagK;
			pStageInfo->GetResultSavedStagKeyList(aStagK);
			int nStageSize = aStagK.GetSize();

			for (i = 0; i < nPrintElemSize; ++i)
			{
				// MQC-18631, 철근 정보가 있는 요소와 없는 요소를 같이 출력할 때 초기화 안하여 철근이 없어도 있는 것 처럼 출력되는 오류 수정
				MembLcomD.Initialize();

				m_pDataCtrl->m_pCsgDataCtrl->ConvertCGSGlobalData(m_CpgdD, MembLcomD.Memb.Csgd);
				int nConsiderPv = MembLcomD.Memb.Csgd.nConsiderPv; // 0:직선, 1:곡선
				BOOL bCurved = (nConsiderPv == 0) ? FALSE : TRUE;

				if (nStageSize == 0 && bIsPLCB == TRUE)
				{
					MembLcomD.Memb.ChIf.bIsPLCB = TRUE;
				}

				auto  ElemK = aRptElemPairK[i];

				MembLcomD.Memb.ChIf.dLength = CDBLib::GetLengthOfBeam(ElemK);
				// 1. 출력 여부 결정
				T_CGDP_D CgdpD; CgdpD.Initialize();  // 설계 위치
				T_CGRE_D CgreD; CgreD.Initialize();  // 출력 위치
				if (ElemK.second == EN_EL_BEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetCgdp(ElemK.first, CgdpD)) continue;
					if (!m_pDoc->m_pAttrCtrl2->GetCgre(ElemK.first, CgreD)) continue;
				}
				else if (ElemK.second == EN_EL_VBEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetCgdv(ElemK.first, CgdpD)) continue;
					if (!m_pDoc->m_pAttrCtrl2->GetCgrv(ElemK.first, CgreD)) continue;
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

				int nMatlK = CDBLib::GetMaterialKey4ElemPairK(ElemK);

				if (!m_pDataCtrl->m_pCsgDataCtrl->m_mCsgMatl.Lookup(nMatlK, MembLcomD.Memb.Matl))
				{
					continue;
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

				// 3. Output Data Setting(1)
				//CSG_MEMBPOS_RES_CSA_S6_D MembResD;

				T_CCBR_D CcbrD;   //휨 응력(CS) 
				T_CCVR_D CcvrD;   //전단 응력(CS)
				T_CPBR_D CpbrD;   //휨 응력
				T_CPVR_D CpvrD;   //전단 응력
				T_CPFC_D CpfcD;   //피로
				T_CPAC_D CpacD;   //사용한계
				T_CASC_D CascD;   //전단연결재
				T_CPTS_D CptsD;   //Transverse Stiffener
				T_CPBS_D CpbsD;   //Bearing Stiffener
				T_SPCR_D SpcrD;   // Span Check

//         CSG_MEMB_RES_CSA_S6_D_FLEX          FlexResD;
//         CSG_MEMB_RES_CSA_S6_D_SHEAR         ShearResD;
//         CSG_MEMB_RES_CSA_S6_D_FATI          FatiResD;
//         CSG_MEMB_RES_CSA_S6_D_CSCR          CscrResD;       // 전단연결재


				auto _l_SetLcomInfo = [&](UINT nLcomK, int nMax, CStringW &strLcomName, CStringW &strLcomType) -> void
				{
					T_LCOM_D LcomD;
					if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, nLcomK, LcomD))
					{
						strLcomName = CT2W(LcomD.LoadCombName);
					}
					strLcomType = CRptSteelGirder::GetLoadCombType(nMax);
				};

				auto _I_bFatiMVInfo = [&](UINT nLcomK) -> BOOL
				{
					T_LCOM_D LcomData;  LcomData.Initialize();
					m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, nLcomK, LcomData);
					int nFatiLcomCount = LcomData.aCombination.GetSize();
					BOOL bIsFatiMVLoad = FALSE;
					for (int i = 0; i < nFatiLcomCount; ++i)
					{
						if (LcomData.aCombination[i].AnalType == D_LCOM_MOVING)
							bIsFatiMVLoad = TRUE;
					}
					return bIsFatiMVLoad;
				};

				// 4. Output Data Setting(2)       
				for (j = 0; j < 2; j++) // I,J
				{
					RptData.Initialize();
					MembLcomD.Memb.ChIf.nChkPos = j;
					BOOL bPosiI = j == 0 ? TRUE : FALSE;
					if (!m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, j + 1)) continue;
					if (j == 0 && (!bPrintI)) continue;
					if (j == 1 && (!bPrintJ)) continue;

					CSG_CHK_RES_DTR_PARAM  DtrR;

					//Member
					m_pDataCtrl->m_pCsgDataCtrl->MakeMemberResultData(ElemK, MembLcomD, MembR); // Check_MemberCSA(ElemK, j, MembLcomD, MembResD, rData);

					RptData.MembData.Detail_ConsF[0].dZtop = MembR.pCSA->MembR[j].Detail_ConsF[0].dZtop;
					RptData.MembData.Detail_ConsF[0].dZbot = MembR.pCSA->MembR[j].Detail_ConsF[0].dZbot;

					// 휨 
					if (pCheckGirder->ReadCPBR_CSA_S6(ElemK, CpbrD)) // 요소별 한번만 읽으면 되는데..
					{
						for (k = 0; k < 2; k++) // Positive/Negative
						{
							kk = j * 2 + k;  // 0: I-정, 1: I-부, 2: J-정, 3: J-부  
							if (!CpbrD.CpbrB[kk].bChk)
							{
								if (k == 0) RptData.RptStrnD.bMomentDir = TRUE;   //Negative
								else     RptData.RptStrnD.bMomentDir = FALSE;  //Positive
								continue; // 설계결과가 없으면 Continue                 
							}
							MembLcomD.aChkFor[0].nLcomType = 0;
							MembLcomD.aChkFor[0].nMax = CpbrD.CpbrB[kk].nMax;
							MembLcomD.bChkUlmf = CpbrD.CpbrB[kk].bChkUlmf;
							ChkResD.pCSA->StrnD.FlexD.dMa = CpbrD.CpbrB[kk].UlForLC.For[0].dForce[4];
							ChkResD.pCSA->StrnD.FlexD.dMb = CpbrD.CpbrB[kk].UlForLC.For[1].dForce[4];
							ChkResD.pCSA->StrnD.FlexD.dMc = CpbrD.CpbrB[kk].UlForLC.For[2].dForce[4];
							ChkResD.pCSA->StrnD.FlexD.dMmax = CpbrD.CpbrB[kk].dMax;
							MembLcomD.aUlmFor.SetSize(1);
							MembLcomD.aUlmFor[0].ForD[1].ForLC.dForce[4] = CpbrD.CpbrB[kk].UlForLC.For[0].dForce[4];
							MembLcomD.aUlmFor[0].ForD[2].ForLC.dForce[4] = CpbrD.CpbrB[kk].UlForLC.For[1].dForce[4];
							MembLcomD.aUlmFor[0].ForD[3].ForLC.dForce[4] = CpbrD.CpbrB[kk].UlForLC.For[2].dForce[4];

							MembLcomD.aUlmFor[0].dMmax = CpbrD.CpbrB[kk].dMax;
							ConvertCsgCalcChkForce(CpbrD.CpbrB[kk].ComFor, MembLcomD.aChkFor[0]);
							m_pDataCtrl->m_pCsgDataCtrl->Check_Strength(ElemK, j, MembLcomD, MembR, ChkResD, DtrR);

							RptData.RptStrnD.FlexD[k] = ChkResD.pCSA->StrnD.FlexD;
							RptData.RptConsD.Flex[k] = ChkResD.pCSA->ConsD.Flex;
							MembLcomD.bIsLcomULS = CpbrD.CpbrB[kk].bChk;
							_l_SetLcomInfo(CpbrD.CpbrB[kk].nOrgLcomK, CpbrD.CpbrB[kk].nMax, RptData.RptStrnD.FlexD[k].ForceD.strLcomName, RptData.RptStrnD.FlexD[k].ForceD.strLcomType);
						}
					}


					// 전단
					if (m_CpgdD.bDesignParameter[1])
					{
						if (pCheckGirder->ReadCPVR_CSA_S6(ElemK, CpvrD))
						{
							if (CpvrD.CpvrB[j].bChk)  // 0 : I-max  1: J-max
							{
								ChkResD.pCSA->StrnD.ShearD.bCheck = TRUE;
								MembLcomD.aChkFor[0].nLcomType = 0;
								MembLcomD.aChkFor[0].nMax = CpvrD.CpvrB[j].nMax;
								ConvertCsgCalcUlmForce(CpvrD.CpvrB[j].UlFor, MembLcomD.aUlmf[0]);
								ConvertCsgCalcChkForce(CpvrD.CpvrB[j].ComFor, MembLcomD.aChkFor[0]);
								m_pDataCtrl->m_pCsgDataCtrl->Check_Strength(ElemK, j, MembLcomD, MembR, ChkResD, DtrR);

								_l_SetLcomInfo(CpvrD.CpvrB[j].nOrgLcomK, CpvrD.CpvrB[j].nMax, ChkResD.pCSA->StrnD.ShearD.ForceD.strLcomName, ChkResD.pCSA->StrnD.ShearD.ForceD.strLcomType);
								RptData.RptStrnD.ShearD = ChkResD.pCSA->StrnD.ShearD;
							}
						}
					}

					// 사용한계
					if (m_CpgdD.bDesignParameter[2])
					{
						if (pCheckGirder->ReadCPAC_CSA_S6(ElemK, CpacD))
						{
							int nNum = 0;
							double dRatioFirst = 0.0;
							CSG_MEMB_RES_CSA_S6_D_SERV  ServiceFirst;
							for (k = 0; k < 2; k++) // Positive/Negative
							{
								kk = j * 2 + k;
								if (!CpacD.CpacB[kk].bChk)
								{
									ChkResD.pCSA->ServD.bCheck = FALSE;
									continue; // 설계결과가 없으면 Continue                 
								}
								ChkResD.pCSA->ServD.bCheck = TRUE;
								MembLcomD.aChkFor[0].nLcomType = 1;
								MembLcomD.aChkFor[0].nMax = CpacD.CpacB[kk].nMax;
								ConvertCsgCalcUlmForce(CpacD.CpacB[kk].UlFor, MembLcomD.aUlmf[0]);
								ConvertCsgCalcChkForce(CpacD.CpacB[kk].ComFor, MembLcomD.aChkFor[0]);
								m_pDataCtrl->m_pCsgDataCtrl->Check_Service(ElemK, j, MembLcomD, MembR, ChkResD, DtrR);

								_l_SetLcomInfo(CpacD.CpacB[kk].nOrgLcomK, CpacD.CpacB[kk].nMax, ChkResD.pCSA->ServD.ForceD.strLcomName, ChkResD.pCSA->ServD.ForceD.strLcomType);
								RptData.RptServD[k] = ChkResD.pCSA->ServD;
								MembLcomD.bIsLcomSLS = CpacD.CpacB[kk].bChk;
							}
						}

					}

					if (m_CpgdD.bChkCS)
					{
						//// 시공단계 휨 /////
						if (pCheckGirder->ReadCCBR_CSA_S6(ElemK, CcbrD))
						{
							for (k = 0; k < 2; k++) // Positive/Negative
							{
								kk = j * 2 + k;  // 0: I-정, 1: I-부, 2: J-정, 3: J-부  
								if (!CcbrD.CcbrB[kk].bChk)
								{
									if (k == 0) RptData.RptConsD.bMomentDir = TRUE;   //Negative
									else     RptData.RptConsD.bMomentDir = FALSE;  //Positive
									ChkResD.pCSA->ConsD.Flex.bChk = FALSE;
									continue; // 설계결과가 없으면 Continue                 
								}
								ChkResD.pCSA->ConsD.bCheck = TRUE;
								RptData.RptConsD.Flex[k].bChk = ChkResD.pCSA->ConsD.Flex.bChk;
								MembLcomD.aChkFor[0].nLcomType = 0;
								MembLcomD.aUlmFor.SetSize(1);
								MembLcomD.aUlmFor[0].ForD[1].ForNC.dForce[4] = CcbrD.CcbrB[kk].UlFor.For[0].dForce[4];
								MembLcomD.aUlmFor[0].ForD[2].ForNC.dForce[4] = CcbrD.CcbrB[kk].UlFor.For[1].dForce[4];
								MembLcomD.aUlmFor[0].ForD[3].ForNC.dForce[4] = CcbrD.CcbrB[kk].UlFor.For[2].dForce[4];
								MembLcomD.aUlmFor[0].dMmax = CcbrD.CcbrB[kk].dMax;
								ConvertCsgCalcForce(CcbrD.CcbrB[kk].CsFor, MembLcomD.aChkFor[0].ForNC);
								m_pDataCtrl->m_pCsgDataCtrl->Check_Constructibility(ElemK, j, MembLcomD, MembR, ChkResD, DtrR);
								RptData.RptConsD.Flex[k] = ChkResD.pCSA->ConsD.Flex;
								RptData.RptConsD.AxisR = ChkResD.pCSA->ConsD.AxisR;
								RptData.RptConsD.Comb = ChkResD.pCSA->ConsD.Comb;
								MembLcomD.bIsLcomULS = CcbrD.CcbrB[kk].bChk;

								CString strTempLcomName = Get_StageName(CcbrD.CcbrB[kk].nStageK);
								CStringW strWTempLcomName = CT2W(strTempLcomName);
								RptData.RptConsD.Flex[k].ForceD.strLcomName = strWTempLcomName;
								CString strTempN; strTempN.Format(_T("%d"), CcbrD.CcbrB[kk].nStepK);
								CStringW strWTempN(strTempN);
								RptData.RptConsD.Flex[k].ForceD.strLcomType = strWTempN;

							}
						}

						//// 시공단계 전단 ////
						if (pCheckGirder->ReadCCVR_CSA_S6(ElemK, CcvrD))
						{
							if (CcvrD.CcvrB[j].bChk)  // 0 : I-max  1: J-max
							{
								ChkResD.pCSA->ConsD.Shear.bChk = TRUE;
								MembLcomD.aChkFor[0].nLcomType = 0;
								ConvertCsgCalcUlmForce(CcvrD.CcvrB[j].UlFor, MembLcomD.aUlmf[0]);
								ConvertCsgCalcForce(CcvrD.CcvrB[j].CsFor, MembLcomD.aChkFor[0].ForNC);
								m_pDataCtrl->m_pCsgDataCtrl->Check_Constructibility(ElemK, j, MembLcomD, MembR, ChkResD, DtrR);
								RptData.RptConsD.Shear = ChkResD.pCSA->ConsD.Shear;

								CString strTempLcomName = Get_StageName(CcvrD.CcvrB[j].nStageK);
								CStringW strWTempLcomName = CT2W(strTempLcomName);
								RptData.RptConsD.Shear.ForceD.strLcomName = strWTempLcomName;
								CString strTempN; strTempN.Format(_T("%d"), CcvrD.CcvrB[j].nStepK);
								CStringW strWTempN(strTempN);
								RptData.RptConsD.Shear.ForceD.strLcomType = strWTempN;
							}
						}
					}
					// 피로한계
					if (m_CpgdD.bDesignParameter[4])
					{
						if (pCheckGirder->ReadCPFC_CSA_S6(ElemK, CpfcD))
						{

							T_CPFC_LOAD  &FLoadD_Top = CpfcD.CpfcB[j].LoadR[0];
							T_CPFC_LOAD  &FLoadD_Bot = CpfcD.CpfcB[j].LoadR[1];
							T_CPFC_STUD  &FStudD = CpfcD.CpfcB[j].StudR;
							T_CPFC_SCONN &FSconnD = CpfcD.CpfcB[j].SConnR;
							if (CpfcD.CpfcB[j].LoadR[0].bChk)   //Top
							{
								MembLcomD.LcomInf.aMVIn.RemoveAll();
								if (_I_bFatiMVInfo(CpfcD.CpfcB[j].LoadR[0].nOrgLcomK))    //FLS 하중조합 내 MV loadCase 여부 
									MembLcomD.LcomInf.aMVIn.SetSize(1);

								ChkResD.pCSA->FatiD.bCheck = TRUE;
								if (FLoadD_Top.ComForMV.nLcomSize > 0)
									MembLcomD.aChkFor.SetSize(FLoadD_Top.ComForMV.nLcomSize);

								for (int i = 0; i < FLoadD_Top.ComForMV.nLcomSize; ++i)
									ConvertCsgCalcChkForce(FLoadD_Top.ComForMV.ForMV[i], MembLcomD.aChkFor[i]);

								int nLcomSize = MembLcomD.aChkFor.GetSize();
								FLoadD_Top.ComForMV.nLcomSize = nLcomSize;
							}
							CSG_FATI_CR_RSLT FatiCrResD;
							m_pDataCtrl->m_pCsgDataCtrl->Check_Fatigue(ElemK, j, MembLcomD, MembR, ChkResD, DtrR, FatiCrResD);
							RptData.RptFatiD = ChkResD.pCSA->FatiD;
							RptData.RptFatiD.FlgR[0].ForceD = ChkResD.pCSA->FatiD.ForceD;     //Top Flange 부재력
							_l_SetLcomInfo(FLoadD_Top.nOrgLcomK, 0, RptData.RptFatiD.FlgR[0].ForceD.strLcomName, RptData.RptFatiD.FlgR[0].ForceD.strLcomType);

							if (CpfcD.CpfcB[j].LoadR[1].bChk)  //Bot
							{
								MembLcomD.LcomInf.aMVIn.RemoveAll();
								if (_I_bFatiMVInfo(CpfcD.CpfcB[j].LoadR[1].nOrgLcomK))  //FLS 하중조합 내 MV loadCase 여부 
									MembLcomD.LcomInf.aMVIn.SetSize(1);

								if (FLoadD_Bot.ComForMV.nLcomSize > 0)
									MembLcomD.aChkFor.SetSize(FLoadD_Bot.ComForMV.nLcomSize);

								for (int i = 0; i < FLoadD_Bot.ComForMV.nLcomSize; ++i)
									ConvertCsgCalcChkForce(FLoadD_Bot.ComForMV.ForMV[i], MembLcomD.aChkFor[i]);

								int nLcomSize = MembLcomD.aChkFor.GetSize();
								FLoadD_Bot.ComForMV.nLcomSize = nLcomSize;
							}

							FatiCrResD.Initialize();
							m_pDataCtrl->m_pCsgDataCtrl->Check_Fatigue(ElemK, j, MembLcomD, MembR, ChkResD, DtrR, FatiCrResD);
							if (!CpfcD.CpfcB[j].LoadR[0].bChk) RptData.RptFatiD = ChkResD.pCSA->FatiD;
							RptData.RptFatiD.FlgR[1].ForceD = ChkResD.pCSA->FatiD.ForceD;     //Bot Flange 부재력
							MembLcomD.bIsLcomFLS = CpfcD.CpfcB[j].bChk;
							_l_SetLcomInfo(FLoadD_Bot.nOrgLcomK, 0, RptData.RptFatiD.FlgR[1].ForceD.strLcomName, RptData.RptFatiD.FlgR[1].ForceD.strLcomType);

							if (CpfcD.CpfcB[j].StudR.bChk)  //Stud-Welded
							{
								MembLcomD.LcomInf.aMVIn.RemoveAll();
								if (_I_bFatiMVInfo(CpfcD.CpfcB[j].StudR.nOrgLcomK))  //FLS 하중조합 내 MV loadCase 여부 
									MembLcomD.LcomInf.aMVIn.SetSize(1);

								if (FStudD.ComForMV.nLcomSize > 0)
									MembLcomD.aChkFor.SetSize(FStudD.ComForMV.nLcomSize);

								for (int i = 0; i < FStudD.ComForMV.nLcomSize; ++i)
									ConvertCsgCalcChkForce(FStudD.ComForMV.ForMV[i], MembLcomD.aChkFor[i]);

								int nLcomSize = MembLcomD.aChkFor.GetSize();
								FStudD.ComForMV.nLcomSize = nLcomSize;
							}

							if (CpfcD.CpfcB[j].SConnR.bChk)
							{
								ConvertCsgCalcChkForce(FSconnD.FatiForMax, MembLcomD.aChkFor[ChkResD.pCSA->FatiD.SConn.nRangeMaxIdx]);
								ConvertCsgCalcChkForce(FSconnD.FatiForMin, MembLcomD.aChkFor[ChkResD.pCSA->FatiD.SConn.nRangeMinIdx]);
							}

							FatiCrResD.Initialize();
							m_pDataCtrl->m_pCsgDataCtrl->Check_Fatigue(ElemK, j, MembLcomD, MembR, ChkResD, DtrR, FatiCrResD);
							RptData.RptFatiD.StudWeld.ForceD = ChkResD.pCSA->FatiD.StudWeld.ForceD;     //Stud Base 부재력
							MembLcomD.bIsLcomFLS = CpfcD.CpfcB[j].bChk;
							_l_SetLcomInfo(FSconnD.nOrgLcomK, ChkResD.pCSA->FatiD.SConn.nRangeMaxIdx + 1, RptData.RptFatiD.SConn.ForceD_Max.strLcomName, RptData.RptFatiD.SConn.ForceD_Max.strLcomType);
							_l_SetLcomInfo(FSconnD.nOrgLcomK, ChkResD.pCSA->FatiD.SConn.nRangeMinIdx + 1, RptData.RptFatiD.SConn.ForceD_Min.strLcomName, RptData.RptFatiD.SConn.ForceD_Min.strLcomType);
						}

						MembLcomD.aChkFor.RemoveAll();
						MembLcomD.aChkFor.SetSize(1);

						MembLcomD.LcomInf.aMVIn.SetSize(1);
					}

					RptData.RptStrnD.TStfnD = ChkResD.pCSA->StrnD.TStfnD;

					//Bearing Stiffener
					MembR.pCSA->MembR[j].BStfn.Initialize();
					ChkResD.pCSA->StrnD.BStfnD.Initialize();					
					if (pCheckGirder->ReadCPBS_CSA_S6(ElemK, CpbsD))
					{
						if ((j == 0 && bPrintI) || (j == 1 && bPrintJ))
						{
							if (CpbsD.CpbsB[j].bChk)  // 0 : I-max  1: J-max
							{
								int nReacSize = 1;
								MembLcomD.aReac.SetSize(nReacSize);
								MembLcomD.aReac[0].dFz = CpbsD.CpbsB[j].dReact;
								m_pDataCtrl->m_pCsgDataCtrl->Check_BearingStiffener(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);

								RptData.RptStrnD.BStfnD = ChkResD.pCSA->StrnD.BStfnD;
							}
						}
					}	
					else
					{
						RptData.RptStrnD.BStfnD = ChkResD.pCSA->StrnD.BStfnD;
					}
										
					RptData.MembData = MembR.pCSA->MembR[j];
					RptData.MembData.bIJ = j == 0 ? FALSE : TRUE;

					GetDgnParamOption(RptData.MembData, m_CpgdD);

					SetRptData(ElemK, j, MembLcomD.Memb, RptData.MembData);

					RptData.MembData.bChkFlex = m_CpgdD.bDesignParameter[0];
					RptData.MembData.bChkShear = m_CpgdD.bDesignParameter[1];
					RptData.MembData.bChkServ = m_CpgdD.bDesignParameter[2];
					RptData.MembData.bChkFati = m_CpgdD.bDesignParameter[4];
					RptData.MembData.bChkCSCR = m_CpgdD.bDesignParameter[5];
					RptData.MembData.bChkCons = m_CpgdD.bChkCS;

					aInData.Add(MembLcomD);
					aRptData.Add(RptData);
				}
				if (m_bStopExecute)	return FALSE;
				nProgressPercent += nProgressIncrementPercent;
				Progress(0, nProgressPercent);
				Progress(2, nProgressPercent);
			}
		}

		if (nNumPrtElem == 0)
		{
			CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // "출력할 요소의 설계정보가 없습니다."
			AfxMessageBox(strErrMessage);
			return FALSE;
		}
		//RESULT-TABLE
		nProgressIncrementPercent = nPrintElemSize == 0 ? 30 : 30 / nPrintElemSize;
		int iPosiNo = 0;
		int iMaxMin = 0;

		// 계산서 출력
		CRptSteelGirder SteelGirder;
		CDgn_CSGRptManager_CSA_S6 myLSDCSGReport;
		if (!myLSDCSGReport.Print_DetailReport(iDgnCode, m_pDoc->GetProgramPath(), SteelGirder.GetSaveFileDirEx(), strPath/*SteelGirder.GetSaveFileNameEx(FALSE)*/,
			aInData, aRptData))
		{
			ASSERT(0);
			return FALSE;
		}

	}
	catch (...)
	{
		ASSERT(0);
		bSuccess = FALSE;
	}

	if (MembR.pCSA != NULL) delete MembR.pCSA;

	if (ChkResD.pCSA != NULL) delete ChkResD.pCSA;

	return bSuccess;
}


void CRptSteelGirder_CSA_S6::SetRptData(ElemPairK ElemK, int nPosi, CSG_MEMB_POSD& InData, CSG_MEMB_RES_CSA_S6_D& RptData)
{
	RptData.Memb.nElem = ElemK.first;
	RptData.Memb.nPosi = nPosi;

	double dHw = 0.0;
	double dD = 0.0;
	double dtw = 0.0;
	double dB1 = 0.0;
	double dtf1 = 0.0;
	double dB2 = 0.0;
	double dtf2 = 0.0;
	double dBf1 = 0.0;
	double dBf2 = 0.0;
	double dBf3 = 0.0;
	double dH = 0.0;

	const int nSectType = InData.PosD[nPosi].Sect.SectInfo.nStype;
	const ST_SECT_SECTBASE_D& SectBefore = InData.PosD[nPosi].Sect.SectInfo.Before;
	CSG_MEMB_GIRDER_RES_D& rGirder = RptData.Memb.GIRDER;

	switch (nSectType)
	{
	case D_SECT_TYPE_COMPO_I:
		{
			dHw = SectBefore.Size[0];
			dtw = SectBefore.Size[1];
			dB1 = SectBefore.Size[2];
			dtf1 = SectBefore.Size[3];
			dB2  = SectBefore.Size[4];
			dtf2 = SectBefore.Size[5];

			// 거더-단면
			rGirder.dbfc = dB1;            // "bfc"     
			rGirder.dbft = dB2;            // "bft"     
			rGirder.dtfc = dtf1;           // "tfc"     
			rGirder.dtft = dtf2;           // "tft"     
			rGirder.dtw = dtw;            // "tw"      
			rGirder.dH = dHw;            // "H"   
			rGirder.dD = dHw;

			rGirder.dbtop   = dB1 / 2.0; /// length of plate element : half of width of flange of I-sections.
			rGirder.dttop   = dtf1;
			rGirder.dbbot   = dB2 / 2.0; /// length of plate element : half of width of flange of I-sections.
			rGirder.dtbot   = dtf2;			
		}
		break;
	case D_SECT_TYPE_COMPO_B:
		{
			dHw  = SectBefore.Size[0];
			dtw  = SectBefore.Size[1];
			dB1  = SectBefore.Size[2];
			dBf1 = SectBefore.Size[3];
			dtf1 = SectBefore.Size[4];
			dB2  = SectBefore.Size[5];
			dBf2 = SectBefore.Size[6];
			dtf2 = SectBefore.Size[7];
			dBf3 = SectBefore.Size[8];

			rGirder.dbfc = dB1;   // "bfc"    			
			rGirder.dtfc = dtf1;  // "tfc"     
			rGirder.dtft = dtf2;  // "tft"     
			rGirder.dtw  = dtw;   // "tw"      
			rGirder.dH   = dHw;   // "H"    
			const double db1 = 0.5 * (dB1 - dB2);
			rGirder.dD = EQ0(db1) ? dHw : hypot(dHw, db1);
			
			rGirder.dbtop   = dB1 + dtw;
			rGirder.dttop   = dtf1;			
			rGirder.dbbot   = dB2 + dtw;
			rGirder.dtbot   = dtf2;			
		}
		break;
	case D_SECT_TYPE_COMPO_TUB:
		{
			dHw  = SectBefore.Size[0];
			dtw  = SectBefore.Size[1];
			dB1  = SectBefore.Size[2];
			dBf1 = SectBefore.Size[3];
			dtf1 = SectBefore.Size[4];
			dB2  = SectBefore.Size[5];
			dBf2 = SectBefore.Size[6];
			dtf2 = SectBefore.Size[7];
			dBf3 = SectBefore.Size[8];

			// 거더-단면			
			rGirder.dbfc = dBf1;   // "bfc"     						
			rGirder.dtfc = dtf1;   // "tfc"     
			rGirder.dtft = dtf2;   // "tft"     
			rGirder.dtw = dtw;     // "tw"      
			rGirder.dH = dHw;      // "H"    
			const double db1 = 0.5 * (dB1 + 2.0 * (dBf1 - dBf3) - dB2);
			rGirder.dD = EQ0(db1) ? dHw : hypot(dHw, db1);

			rGirder.dbtop = max(dBf1 - dBf3 + dtw / 2.0, dBf3 - dtw / 2.0);
			rGirder.dttop = dtf1;
			rGirder.dbbot = dB2 + dtw;
			rGirder.dtbot = dtf2;			
		}
		break;
	default:
		ASSERT(0);
		break;
	}		

	rGirder.dWTRtop = rGirder.dbtop / rGirder.dttop;
	rGirder.dWTRbot = rGirder.dbbot / rGirder.dtbot;

	// 슬래브 재원                                                 
	RptData.Memb.dBs = InData.PosD[nPosi].Sect.SectInfo.Slab.Size[0];
	RptData.Memb.dts = InData.PosD[nPosi].Sect.SectInfo.Slab.Size[1];
	RptData.Memb.dth = InData.PosD[nPosi].Sect.SectInfo.Slab.Size[2];
	RptData.Memb.dfck = InData.Matl.MatSlab.RC.dFck; // "fck"   
	RptData.Memb.dEc = InData.Matl.MatSlab.RC.dEc; // "Ec"    
	double dA = 0.0;
	for (int i = 0; i < InData.PosD[nPosi].Rbar.arRbarPosi.GetSize(); ++i)
	{
		dA += InData.PosD[nPosi].Rbar.arRbarPosi[i].dArea;
	}
	RptData.Memb.dAr = dA;  // "Ar"    
	RptData.Memb.dFyr = InData.Matl.MatSlab.RC.dFy; // "Fyr"   

	auto L_SetMatl = [](const T_DGN_MATL_CODE_STEEL& src, double dthick, OUT CSG_GIRDER_PART& rTar)
	{
		rTar.strMatType = src.csMatlName;
		rTar.dthick = dthick;
		rTar.dfy_or = src.Steel.S_Fy1;
		rTar.dfy = src.Steel.S_Fy; // 판폭두께비에 따른 fy로 결과구조체에서 채울값.
		rTar.dfu = src.Steel.S_Fu;
		rTar.strMemo = _T("-");
	};

	// 거더-상부플랜지
	L_SetMatl(InData.Matl.MatStf, dtf1, RptData.Memb.GIRDER.Top);
	
	// 거더-하부플랜지
	L_SetMatl(InData.Matl.MatSbf, dtf2, RptData.Memb.GIRDER.Bottom);
	
	// 거더-웹 
	L_SetMatl(InData.Matl.MatSw, dtw, RptData.Memb.GIRDER.Web);
	
	// 거더- 설계강도
	RptData.Memb.GIRDER.dFyc = InData.Matl.MatStf.Steel.S_Fy;
	RptData.Memb.GIRDER.dFyw = InData.Matl.MatSw.Steel.S_Fy;
	RptData.Memb.GIRDER.dFyt = InData.Matl.MatSbf.Steel.S_Fy;
	RptData.Memb.GIRDER.dEs = InData.Matl.MatStf.Steel.Elast;

	// Transverse Stiffner
	const CSG_TRST_POSD& srcTrst = InData.PosD[nPosi].Trst;
	CSG_STIFFENER& rTarTransSftn = RptData.Memb.STIFFENER_V;
	rTarTransSftn.bUseStiff = srcTrst.bUseStifWeb;
	rTarTransSftn.nShapeType = srcTrst.nStiffenerTypeWeb;
	rTarTransSftn.nType = srcTrst.nStifNumWeb;
	rTarTransSftn.ddo = srcTrst.dPitchWeb;
	rTarTransSftn.dhi = srcTrst.dSizeWeb[0];
	rTarTransSftn.dbi = srcTrst.dSizeWeb[1];
	rTarTransSftn.dtw = srcTrst.dSizeWeb[2];
	rTarTransSftn.dti = srcTrst.dSizeWeb[3];
	rTarTransSftn.dfy = srcTrst.dFyWeb;

	// Bearing Stiffener
	CSG_STIFFENER& rTarBearSftn = RptData.Memb.STIFFENER_B;
	rTarBearSftn.bUseStiff = srcTrst.bUseStifBearing;
	rTarBearSftn.nShapeType = srcTrst.nBearingType;
	rTarBearSftn.nStiffNum = srcTrst.nBearingPairNum;
	rTarBearSftn.ddo = srcTrst.dPitchBearing;
	rTarBearSftn.dhi = srcTrst.dSizeBearing[0];
	rTarBearSftn.dbi = srcTrst.dSizeBearing[1];
	rTarBearSftn.dtw = srcTrst.dSizeBearing[2];
	rTarBearSftn.dti = srcTrst.dSizeBearing[3];
	rTarBearSftn.dfy = srcTrst.dFyBearing;	
		
	// Longtidudinal Stiffner
	ST_SECT_STIFF_D& LongitStiff = InData.PosD[nPosi].Sect.SectInfo.Before.Stiffener;
	int nStiffNum = LongitStiff.aStiffShape.GetSize();
	//   RptData.Memb.aSTIFFENER_L.SetSize(nStiffNum);
	//   for(int i=0; i<nStiffNum ; ++i)
	//   {
	//     RptData.Memb.aSTIFFENER_L[i].nShapeType = LongitStiff.aStiffShape[i].nType;
	//     RptData.Memb.aSTIFFENER_L[i].strName    = LongitStiff.aStiffShape[i].strName;
	//     RptData.Memb.aSTIFFENER_L[i].dhi        = LongitStiff.aStiffShape[i].dSize[0];
	//     RptData.Memb.aSTIFFENER_L[i].dbi        = LongitStiff.aStiffShape[i].dSize[1];
	//     RptData.Memb.aSTIFFENER_L[i].dtw        = LongitStiff.aStiffShape[i].dSize[2];
	//     RptData.Memb.aSTIFFENER_L[i].dti        = LongitStiff.aStiffShape[i].dSize[3];
	//     RptData.Memb.aSTIFFENER_L[i].dArea      = LongitStiff.aStiffShape[i].Stiffness.dArea;
	//   }
	// Indata 그대로 
	BOOL bIsLongitStiff = (LongitStiff.aStiffLeft.GetSize() > 0 || LongitStiff.aStiffRight.GetSize() > 0 ||
		LongitStiff.aStiffBottom.GetSize() > 0 || LongitStiff.aStiffTop.GetSize() > 0);
	if (RptData.Memb.STIFFENER_V.ddo < cDGN_Zero && bIsLongitStiff == FALSE)  RptData.Detail_Curved.nChkStiffener = CSG_MEMB_RES_CSA_S6_D_CURVED::EN_UNSTIFF;   //둘다 없음
	if (RptData.Memb.STIFFENER_V.ddo > cDGN_Zero && bIsLongitStiff == FALSE)  RptData.Detail_Curved.nChkStiffener = CSG_MEMB_RES_CSA_S6_D_CURVED::EN_ONLY_TRANS_STIFF;   // TS만 있음
	if (RptData.Memb.STIFFENER_V.ddo > cDGN_Zero && bIsLongitStiff == TRUE)  RptData.Detail_Curved.nChkStiffener = CSG_MEMB_RES_CSA_S6_D_CURVED::EN_BOTH_TRANS_LONGI_STIFF;    // 둘다 있음

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

BOOL CRptSteelGirder_CSA_S6::GetDgnParamOption(CSG_MEMB_RES_CSA_S6_D& rRptData, const T_CPGD_D& CpgdD)
{
	rRptData.Memb.dPhi_s_Flex = CpgdD.dPhi_f;
	rRptData.Memb.dPhi_s_Shear = CpgdD.dPhi_v;
	rRptData.Memb.dPhi_s_Comp = CpgdD.dPhi_c;
	rRptData.Memb.dPhi_S_tens = CpgdD.dPhi_s_Tens;
	rRptData.Memb.dPhi_tos = CpgdD.dPhi_s_Tos;
	rRptData.Memb.dPhi_r = CpgdD.dPhi_y;
	rRptData.Memb.dPhi_SC = CpgdD.dPhi_se;
	rRptData.Memb.dPhi_Con = CpgdD.dPhi_b;
	rRptData.Memb.dPhi_ui = CpgdD.dPhi_u;

	return TRUE;
}
void CRptSteelGirder_CSA_S6::ConvertCsgCalcMVLcaseData(double dW, int nLane, CSG_LCOM_MV_IN &rData)
{
	rData.dW = dW;
	rData.nLane = nLane;
}
