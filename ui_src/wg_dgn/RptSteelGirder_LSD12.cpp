
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "RptSteelGirder_LSD12.h"
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
#include "Dgn_CSGRptManager_LSD12.h"
#include "DgnCsgDataCtrl.h"
#include "DgnDataCtrl.h"
#include "CRCForceCtrl.h"

#include "DgnPscCommon.h"

// #include "CRCExcelOutput.h"
// 
// #include "DgnProgressDlg.h"
// #include "DgnPlateGirder.h"
//#include "CRCDataCtrl.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptSteelGirder_LSD12::CRptSteelGirder_LSD12()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;
	
	//m_bTestMode = CDBLib::CheckTheRegistryForDesignTest();
	
	m_CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(m_CpgdD);
		
	m_pDoc->SetCivilCodeDgn(m_CpgdD.iDgnCode);
	m_pDataCtrl = new CCRCDataCtrl();
	
}

CRptSteelGirder_LSD12::~CRptSteelGirder_LSD12()
{
	if(m_pDataCtrl)
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
	}
}


BOOL CRptSteelGirder_LSD12::Execute_CSGReport_LSD12(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
{
	BOOL bSuccess = TRUE;

	if(!m_CpgdD.bDesignParameter[0] && !m_CpgdD.bDesignParameter[1] && !m_CpgdD.bDesignParameter[2] && 
		 !m_CpgdD.bDesignParameter[3] && !m_CpgdD.bDesignParameter[4] && !m_CpgdD.bDesignParameter[5] && 
		 !m_CpgdD.bDesignParameter[6])
	{
	 
		AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CHECK));  // "선택된 출력변수가 없습니다.
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
	if(nPrintElemSize==0)
	{
		
		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // "출력할 요소의 설계정보가 없습니다."
		AfxMessageBox(strErrMessage);

		return FALSE;
	}

	// 설계요소와 출력요소의 교집합이 존재할 경우 Report과정을 수행함. 그렇지않다면, Message처리함.
	BOOL bIsExistPrintElem= FALSE;
	CRptPlateGirder_EC4 PlateGirder_EC4;
	for(int nNum=0; nNum<nPrintElemSize; ++nNum)
	{
		auto ElemK = aPrintElemPairKey[nNum];
		if(PlateGirder_EC4.Get_CheckResultAndPrintElemPositionType(ElemK))
		{
			bIsExistPrintElem = TRUE;
			break;
		}
	}
	if(!bIsExistPrintElem)
	{
		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // "출력할 요소의 설계정보가 없습니다."
		AfxMessageBox(strErrMessage);
		return FALSE;
	}

	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40/nPrintElemSize;

	// Input Data
	CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>   aInData;     aInData.RemoveAll();
	CArray<CSG_INDATA_SPAN, CSG_INDATA_SPAN&>             aSpanInData;     aSpanInData.RemoveAll();  
	CSG_MEMB_POSD_KSCE_LSD    MembLcomD;

	// Output Data
	CArray<CSG_DRES_PRT,  CSG_DRES_PRT&>           aRptData;     aRptData.RemoveAll();
	CArray<CSG_DRES_SPAN_PRT,  CSG_DRES_SPAN_PRT&> aSpanRptData; aSpanRptData.RemoveAll();
	CSG_DRES_PRT  RptData;


	CSG_MEMB_RESULT_D      MembR;
	CSG_CHECK_RESULT_CODES ChkResD;

	MembR.pAASHTO = new CSG_MEMBPOS_RES_AASHTO_LRFD_D;
	MembR.pLSD    = new CSG_MEMBPOS_RES_KSCE_LSD12_D;

	ChkResD.pAASHTO = new CSG_MEMB_RES_AASHTO_LRFD_D;
	ChkResD.pLSD    = new CSG_CHECK_RES_KSCE_LSD12;

	int i=0; // 요소 
	int j=0; // I/J
	int k=0; // Positive / Negative
	int kk=0;
	int nIJ=0, nMaxMin=0;
	
	m_pDataCtrl->m_pCsgDataCtrl->InitialData(iDgnCode);

	CPlateGirderDesign *pCheckGirder = m_pDoc->m_pPostCtrl->GetPlateGirder();

	try
	{    
		if(m_CpgdD.bStrength[0] || m_CpgdD.bStrength[1] || m_CpgdD.bStrength[2] || m_CpgdD.bStrength[3] || m_CpgdD.bStrength[4] || m_CpgdD.bStrength[5])
		{
			//m_pDataCtrl->Get_UnbracedLength();
			m_pDataCtrl->m_pCsgDataCtrl->Make_CSGDgnData(); 
			
			// Input Data Setting(1)
			T_CPGD_D CpgdD; CpgdD.Initialize();  
			m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);
			m_pDataCtrl->m_pCsgDataCtrl->ConvertCGSGlobalData(CpgdD, MembLcomD.Memb.Csgd);

			// Span 정보 계산 
			m_pDataCtrl->Calc_LengthPosition4Span(aPrintElemPairKey);

			// ***********************
			// DETATL-REPORT
			// ***********************
			for(i=0; i<nPrintElemSize; ++i)
			{
				auto  ElemK = aPrintElemPairKey[i];
				
				// 1. 출력 여부 결정
				BOOL bPrintI = TRUE;
				BOOL bPrintJ = TRUE;
				SetPrintPosition(ElemK, bPrintI, bPrintJ);

				int nMatlK = 0;
				if (ElemK.second == EN_EL_BEAM)
				{
					MembLcomD.Memb.ChIf.dLength = m_pDoc->calcLAVElem(ElemK.first);

					// 2. Input Data Setting(2)
					T_ELEM_D ElemD; ElemD.Initialize();
					if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD)) continue;
					nMatlK = ElemD.elmat;
				}
				else if (ElemK.second == EN_EL_VBEAM)
				{
					MembLcomD.Memb.ChIf.dLength = m_pDoc->calcLenthVbem(ElemK.first);

					T_VBEM_D VbemD; VbemD.Initialize();
					if (m_pDoc->m_pAttrCtrl2->GetVbem(ElemK.first, VbemD))
					{
						T_MATL_K SlabMatlK = 0, GirderMatlK = 0;
						m_pDoc->m_pAttrCtrl2->GetSecvMatlK(ElemK.first, SlabMatlK, GirderMatlK);
						nMatlK = GirderMatlK;
					}
				}
				else ASSERT(0);


				if(!m_pDataCtrl->m_pCsgDataCtrl->m_mCsgMatl.Lookup(nMatlK, MembLcomD.Memb.Matl))
				{
					continue;
					//MembLcomD.Memb.Matl.Initialize();
				}

				m_pDataCtrl->m_pCsgDataCtrl->GetCsgSectionSupport(ElemK, MembLcomD);

				/// - MembLcomD.Memb.PosD의 다른 데이터들 받기
				m_pDataCtrl->m_pCsgDataCtrl->Get_CsgMembInData(ElemK, MembLcomD);

				/// - 하중정보는 계산전에 받음
				int nChkLcomSize=1;
				MembLcomD.aUlmf.SetSize(nChkLcomSize);    // Unbraced Length Force
				MembLcomD.aChkFor.SetSize(nChkLcomSize);  // Check Force;

				// Unbraced Length Setting
				GetUnBracedLenthData(ElemK, MembLcomD.Memb.Ulen);
				
				// Unbraced Length Member Section
				m_pDataCtrl->m_pCsgDataCtrl->Get_CsgUlmSection(ElemK, MembLcomD);


				// 3. Output Data Setting(1)
				RptData.Initialize();
				CSG_MEMBPOS_RES_KSCE_LSD12_D &MembResD = *MembR.pLSD;
				m_pDataCtrl->m_pCsgDataCtrl->MakeMemberResData(ElemK.first, MembLcomD, MembResD.MembR[0]); // I
				m_pDataCtrl->m_pCsgDataCtrl->MakeMemberResData(ElemK.first, MembLcomD, MembResD.MembR[1]); // J  

				T_CSBR_D CsbrD; 
				T_CSVR_D CsvrD;
				T_CSAC_D CsacD;
				T_CSFC_D CsfcD;
				T_SCBR_D ScbrD;
				T_SCVR_D ScvrD;      
				T_CSCR_D CscrD;
				T_CSSL_D CsslD;

//         CSG_STRN_RES_KSCE_LSD12  StrnResD;    // 강도한계(휨)
//         CSG_STRN_RES_KSCE_LSD12_SHEAR ShearD; // 강도한계(전단)
//         CSG_SERV_RES_KSCE_LSD12  ServResD;    // 사용한계
//         CSG_FATI_RES_KSCE_LSD12  FatiResD;    // 피로한계
//         CSG_CONS_RES_KSCE_LSD12  ConsResD;    // 시공성(휨)
//         CSG_CONS_S_RES_KSCE_LSD12 ConShResD;  // 시공성(전단)
//         CSG_CSCR_RES_KSCE_LSD12  CscrResD;    // 전단연결재
//         CSG_CSSL_RES_KSCE_LSD12  CsslResD;    // 수평보강재

				// 4. Output Data Setting(2)       
				for(j=0; j<2; j++) // I,J
				{

					BOOL bPosiI = j==0 ? TRUE : FALSE;
					if(!m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, j+1)) continue;
					if(j==0 && (!bPrintI)) continue;
					if(j==1 && (!bPrintJ)) continue;

					RptData.MembD = MembResD.MembR[j];
					MembLcomD.Memb.ChIf.nElem = ElemK.first;
					MembLcomD.Memb.ChIf.nChkPos = j;
				 
					CSG_CHK_RES_DTR_PARAM  DtrR;

					// 휨 
					if(m_CpgdD.bDesignParameter[0])
					{
						if(pCheckGirder->ReadCSBR_LSD12(ElemK, CsbrD)) // 요소별 한번만 읽으면 되는데..
						{             
							if((j==0 && bPrintI) || (j==1 && bPrintJ))
							{              
								for(k=0; k<2; k++) // Positive/Negative
								{                
									kk = j*2+k;  // 0: I-정, 1: I-부, 2: J-정, 3: J-부  
									if(!CsbrD.CsbrB[kk].bChk) continue; // 설계결과가 없으면 Continue                 
									ConvertCsgCalcUlmForce(CsbrD.CsbrB[kk].UlForLC,  MembLcomD.aUlmf[0]);
									ConvertCsgCalcChkForce(CsbrD.CsbrB[kk].ComFor, MembLcomD.aChkFor[0]);
									m_pDataCtrl->m_pCsgDataCtrl->Check_Strength(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);

									RptData.FlexD[k].FlexD = ChkResD.pLSD->StrnD.FlexD;
									RptData.FlexD[k].FlexD.ForceInfo_F.strLcomName = Get_LcomName(CsbrD.CsbrB[kk].nOrgLcomK);
									RptData.FlexD[k].FlexD.ForceInfo_F.strLcomType = Get_LcomType(CsbrD.CsbrB[kk].nMax);

								}
							}
						}
					}

					// 전단
					if(m_CpgdD.bDesignParameter[1])
					{
						if(pCheckGirder->ReadCSVR_LSD12(ElemK, CsvrD))
						{
							if((j==0 && bPrintI) || (j==1 && bPrintJ))
							{
								if(CsvrD.CsvrB[j].bChk)  // 0 : I-max  1: J-max
								{
									ConvertCsgCalcUlmForce(CsvrD.CsvrB[j].UlFor,  MembLcomD.aUlmf[0]);
									ConvertCsgCalcChkForce(CsvrD.CsvrB[j].ComFor, MembLcomD.aChkFor[0]);
									m_pDataCtrl->m_pCsgDataCtrl->Check_Strength(ElemK, 0, MembLcomD, MembR, ChkResD, DtrR);

									RptData.ShearD = ChkResD.pLSD->StrnD.ShearD;
									RptData.ShearD.ForceInfo_S.strLcomName = Get_LcomName(CsvrD.CsvrB[j].nOrgLcomK);
									RptData.ShearD.ForceInfo_S.strLcomType = Get_LcomType(CsvrD.CsvrB[j].nMax);

								}
							}
						}
					}

					// 사용한계
					if(m_CpgdD.bDesignParameter[2])
					{
						if(pCheckGirder->ReadCSAC_LSD12(ElemK, CsacD))
						{
							if((j==0 && bPrintI) || (j==1 && bPrintJ))
							{
								for(k=0; k<2; k++) // Positive/Negative
								{
									kk = j*2+k;
									if(!CsacD.CsacB[kk].bChk) continue;

									ConvertCsgCalcUlmForce(CsacD.CsacB[kk].UlFor,  MembLcomD.aUlmf[0]);
									ConvertCsgCalcChkForce(CsacD.CsacB[kk].ComFor, MembLcomD.aChkFor[0]);
									m_pDataCtrl->m_pCsgDataCtrl->Check_Service(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);

									RptData.ServD[k] = ChkResD.pLSD->ServD; //ServResD;
									RptData.ServD[k].ForceInfo_Serv.strLcomName = Get_LcomName(CsacD.CsacB[kk].nOrgLcomK);
									RptData.ServD[k].ForceInfo_Serv.strLcomType = Get_LcomType(CsacD.CsacB[kk].nMax);

								}
							}
						}
					}

					// 시공성(휨)
					if(m_CpgdD.bDesignParameter[3])
					{
						if(pCheckGirder->ReadSCBR_LSD12(ElemK, ScbrD))
						{             
							if((j==0 && bPrintI) || (j==1 && bPrintJ))
							{
								for(k=0; k<2; k++) // Positive/Negative
								{
									kk = j*2+k;
									if(!ScbrD.ScbrB[kk].bChk) continue;

									//UINT StagK = ScbrD.ScbrB[kk].nStageK;      // Stage
									//UINT StepK  = ScbrD.ScbrB[kk].nStepK;      // Step 
									//GetCsgUnbracedLengthData4CS(ElemK, k, StagK, StepK, MembLcomD);
									ConvertCsgCalcUlmForce(ScbrD.ScbrB[kk].UlFor,  MembLcomD.aUlmf[0]);
									ConvertCsgCalcForce(ScbrD.ScbrB[kk].CsFor,  MembLcomD.aChkFor[0].ForNC);
									MembLcomD.aChkFor[0].nLcomType = 0; 
									m_pDataCtrl->m_pCsgDataCtrl->Check_Constructibility(ElemK, k, MembLcomD, MembR, ChkResD, DtrR);

									RptData.ConsD[k].Flexure = ChkResD.pLSD->ConsD.Flexure; //ConsResD.Flexure;
									RptData.ConsD[k].Flexure.strStage = Get_StageName(ScbrD.ScbrB[kk].nStageK);
									RptData.ConsD[k].Flexure.StepK = ScbrD.ScbrB[kk].nStepK;
								
								}
							}
						}
					}

					// 시공성(전단)
					if(m_CpgdD.bDesignParameter[3])
					{
						if(pCheckGirder->ReadSCVR_LSD12(ElemK, ScvrD))
						{
							if((j==0 && bPrintI) || (j==1 && bPrintJ))
							{
								if(ScvrD.ScvrB[j].bChk)  // 0 : I-max  1: J-max
								{
									ConvertCsgCalcUlmForce(ScvrD.ScvrB[j].UlFor,  MembLcomD.aUlmf[0]);
									ConvertCsgCalcForce(ScvrD.ScvrB[j].CsFor, MembLcomD.aChkFor[0].ForNC);
									MembLcomD.aChkFor[0].nLcomType = 0; 
									m_pDataCtrl->m_pCsgDataCtrl->Check_Constructibility(ElemK, 0, MembLcomD, MembR, ChkResD, DtrR);

									RptData.ConShearD = ChkResD.pLSD->ConsD.Shear;
									RptData.ConShearD.strStage = Get_StageName(ScvrD.ScvrB[j].nStageK);
									RptData.ConShearD.StepK = ScvrD.ScvrB[j].nStepK;

								}
							}
						}
					}

					// 피로한계
					if(m_CpgdD.bDesignParameter[4])
					{
						if(pCheckGirder->ReadCSFC_LSD12(ElemK, CsfcD))
						{
							if((j==0 && bPrintI) || (j==1 && bPrintJ))
							{
								if(CsfcD.CsfcB[j].bChk)  // 0 : I-max  1: J-max
								{

									// 하중유발 - Top, Bottom
									MembLcomD.aChkFor.RemoveAll(); MembLcomD.aChkFor.SetSize(6);
									T_CSFC_LOAD &LoadTop = CsfcD.CsfcB[j].LoadR[0];
									T_CSFC_LOAD &LoadBot = CsfcD.CsfcB[j].LoadR[1];
									T_CSFC_FLEX &FlexR   = CsfcD.CsfcB[j].FlexR;
									T_CSFC_SHEAR &ShearR = CsfcD.CsfcB[j].ShearR;
									ConvertCsgCalcChkForce(LoadTop.ComForMV.ForMV[LoadTop.nForId[0]], MembLcomD.aChkFor[0]);
									ConvertCsgCalcChkForce(LoadTop.ComForMV.ForMV[LoadTop.nForId[1]], MembLcomD.aChkFor[1]);
									ConvertCsgCalcChkForce(LoadBot.ComForMV.ForMV[LoadBot.nForId[0]], MembLcomD.aChkFor[2]);
									ConvertCsgCalcChkForce(LoadBot.ComForMV.ForMV[LoadBot.nForId[1]], MembLcomD.aChkFor[3]);
									ConvertCsgCalcChkForce(FlexR.ComFor     , MembLcomD.aChkFor[4]);
									ConvertCsgCalcChkForce(ShearR.ComFor    , MembLcomD.aChkFor[5]);

									CSG_FATI_CR_RSLT FatiCrResD;
									m_pDataCtrl->m_pCsgDataCtrl->Check_Fatigue(ElemK, k, MembLcomD, MembR, ChkResD, DtrR, FatiCrResD);
									SetFatigPrtData( CsfcD.CsfcB[j], ChkResD.pLSD->FatiD, RptData.FatiD);

								}
							}
						}
					}

					// 전단연결재
					if(m_CpgdD.bDesignParameter[5])
					{
						if(pCheckGirder->ReadCSCR_LSD12(ElemK, CscrD))
						{
							if((j==0 && bPrintI) || (j==1 && bPrintJ))
							{
								if(CscrD.CscrB[j].bChk)  // 0 : I-max  1: J-max
								{
									SetShearConnectPrtData(CscrD.CscrB[j],  RptData.CscrD);
								}
							}
						}
					}

					// 수평보강재
					int nCsslType = 0; 
					if(m_CpgdD.bDesignParameter[6])
					{
						if(pCheckGirder->ReadCSSL_LSD12(ElemK, CsslD))
						{
							if((j==0 && bPrintI) || (j==1 && bPrintJ))
							{
								if(CsslD.CsslB[j].bChk)  // 0 : I-max  1: J-max
								{
									nCsslType = CsslD.CsslB[j].nMax;
									// 부재력 데이터 없음.
								}
							}
						}
						RptData.MembD.nCsslType = nCsslType;
					}

					// 출력 option
					for(int n=0; n<7; n++)
					{
						RptData.bPrintChk[n]= m_CpgdD.bDesignParameter[n];
					}
			 
					// 데이터 변환, -> 제거하자..
					SetRptData(ElemK.first, j, MembLcomD.Memb, RptData);

					//
					aInData.Add(MembLcomD);
					aRptData.Add(RptData);
				}
				//
				if(m_bStopExecute)	return FALSE;

			} // Element


			// ***********************
			// Span
			// ***********************      
			SetSpanRptData(aSpanInData, aSpanRptData);

		}

		//RESULT-TABLE
		nProgressIncrementPercent = nPrintElemSize == 0 ? 30 : 30/nPrintElemSize;
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
		CDgn_CSGRptManager_LSD12  myLSDCSGReport;
			 
		myLSDCSGReport.Print_DetailReport(iDgnCode, m_pDoc->GetProgramPath(), SteelGirder.GetSaveFileDirEx(), strPath, 
																		 aSpanInData, aSpanRptData, aInData, aRptData/*, aFysgSNiP, aFycmSNiP, aFpsgSNiP, aFpmsSNiP, aFypcSNiP*/);

		
	}
	catch(...)
	{
		bSuccess = FALSE;	
	}

	if (MembR.pAASHTO != NULL) delete MembR.pAASHTO;
	if (MembR.pLSD    != NULL) delete MembR.pLSD;

	if (ChkResD.pAASHTO != NULL) delete ChkResD.pAASHTO;
	if (ChkResD.pLSD    != NULL) delete ChkResD.pLSD;

	return bSuccess;
}



void CRptSteelGirder_LSD12::SetFatigPrtData( T_CSFC_BASE &CsfcB, CSG_FATI_RES_KSCE_LSD12 &FatiResD, CSG_FATI_RES_KSCE_LSD12_PRT &FatiD)
{
	T_CSFC_LOAD &LoadTop = CsfcB.LoadR[0];
	T_CSFC_LOAD &LoadBot = CsfcB.LoadR[1];
	T_CSFC_FLEX &FlexR   = CsfcB.FlexR;
	T_CSFC_SHEAR &ShearR = CsfcB.ShearR;

	// 하중유발 - Top
	FatiD.FATI_LT.FatiR                           = FatiResD.FATI_L_T;
	FatiD.FATI_LT.FatiR.ForceInfo[0].iMax         = LoadTop.nfcMax;
	FatiD.FATI_LT.FatiR.ForceInfo[0].strLcomName  = Get_LcomName(LoadTop.nOrgLcomK);
	FatiD.FATI_LT.FatiR.ForceInfo[0].strLcomType  = Get_LcomType(LoadTop.nfcMax);
	FatiD.FATI_LT.FatiR.ForceInfo[1].iMax         = LoadTop.nftMax;
	FatiD.FATI_LT.FatiR.ForceInfo[1].strLcomName  = Get_LcomName(LoadTop.nOrgLcomK);
	FatiD.FATI_LT.FatiR.ForceInfo[1].strLcomType  = Get_LcomType(LoadTop.nftMax);
	
	// 하중유발 - Bot
	FatiD.FATI_LB.FatiR                           = FatiResD.FATI_L_B;
	FatiD.FATI_LB.FatiR.ForceInfo[0].iMax         = LoadBot.nfcMax;
	FatiD.FATI_LB.FatiR.ForceInfo[0].strLcomName  = Get_LcomName(LoadBot.nOrgLcomK);
	FatiD.FATI_LB.FatiR.ForceInfo[0].strLcomType  = Get_LcomType(LoadBot.nfcMax);                 
	FatiD.FATI_LB.FatiR.ForceInfo[1].iMax         = LoadBot.nftMax;
	FatiD.FATI_LB.FatiR.ForceInfo[1].strLcomName  = Get_LcomName(LoadBot.nOrgLcomK);
	FatiD.FATI_LB.FatiR.ForceInfo[1].strLcomType  = Get_LcomType(LoadBot.nftMax);

	// 변형유발 - 휨
	FatiD.FATI_DF.FatiR                        = FatiResD.FATI_D_FLEX;
	FatiD.FATI_DF.FatiR.ForceInfo.iMax         = FlexR.nMax;
	FatiD.FATI_DF.FatiR.ForceInfo.strLcomName  = Get_LcomName(FlexR.nOrgLcomK);
	FatiD.FATI_DF.FatiR.ForceInfo.strLcomType  = Get_LcomType(FlexR.nMax);
	
	// 변형유발 - 전단
	FatiD.FATI_DS.FatiR                        = FatiResD.FATI_D_SHEAR;
	FatiD.FATI_DS.FatiR.ForceInfo.iMax         = ShearR.nMax;
	FatiD.FATI_DS.FatiR.ForceInfo.strLcomName  = Get_LcomName(ShearR.nOrgLcomK);
	FatiD.FATI_DS.FatiR.ForceInfo.strLcomType  = Get_LcomType(ShearR.nMax);

}


void CRptSteelGirder_LSD12::SetShearConnectPrtData(  T_CSCR_BASE &CscrB,  CSG_CSCR_RES_KSCE_LSD12 &CscrResD)
{
	CscrResD.ForceInfo.iMax        = CscrB.nMax;
	CscrResD.ForceInfo.strLcomName = Get_LcomName(CscrB.nOrgLcomK);
	CscrResD.ForceInfo.strLcomType = Get_LcomType(CscrB.nMax);
	CscrResD.dVst                  = CscrB.dVst;
	CscrResD.dpuse_lim0            = CscrB.dp_lim1; 

}

void CRptSteelGirder_LSD12::SetRptData(int ElemK, int nPosi, CSG_MEMB_POSD& InData, CSG_DRES_PRT& RptData)
{
	RptData.MembD.Memb.nElem  = ElemK;
	RptData.MembD.Memb.nPosi  = nPosi;

	// 슬래브
	double dBc =  InData.PosD[nPosi].Sect.SectInfo.Slab.Size[0];
	double dtc =  InData.PosD[nPosi].Sect.SectInfo.Slab.Size[1];
	double dHh =  InData.PosD[nPosi].Sect.SectInfo.Slab.Size[2];
	
	// 거더
	int nSectType = InData.PosD[nPosi].Sect.SectInfo.nStype;

	double dHw  = 0.0;
	double dtw  = 0.0; 
	double dB1  = 0.0; 
	double dBf1 = 0.0; 
	double dtf1 = 0.0; 
	double dB2  = 0.0; 
	double dBf2 = 0.0; 
	double dtf2 = 0.0; 
	double dBf3 = 0.0; 

	if(nSectType==D_SECT_TYPE_COMPO_I)
	{
		dHw  =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[0]; // 복부 높이
		dtw  =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[1]; // 복부 두께
		dB1  =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[2]; // 상부플랜지 폭
		dtf1 =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[3]; // 상부플랜지 두께 
		dB2  =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[4]; // 하부플랜지 폭
		dtf2 =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[5]; // 하부플랜지 두께 
	}
	else
	{
		dHw  =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[0]; // 복부 높이
		dtw  =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[1]; // 복부 두께
		dB1  =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[2]; // 상부플랜지 폭        ||  (Tub) 상부플랜지 간격
		dBf1 =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[3]; // 상부플랜지 돌출길이  ||  (Tub) 상부플랜지 폭(한개)
		dtf1 =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[4]; // 상부플랜지 두께      ||  (Tub) 상부플랜지 두께 
		dB2  =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[5]; // 하부플랜지 폭        ||  (Tub) 하부플랜지 폭(복부사이)
		dBf2 =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[6]; // 하부플랜지 돌출길이  ||  (Tub) 하부플랜지 돌출폭
		dtf2 =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[7]; // 하부플랜지 두께      ||  (Tub) 하부플랜지 두께
		dBf3  = InData.PosD[nPosi].Sect.SectInfo.Before.Size[8];
	}

	double dD = 0.0;
	if (nSectType==D_SECT_TYPE_COMPO_I) 
	{
		dD = dHw;
	}
	else if (nSectType==D_SECT_TYPE_COMPO_B)
	{
		double db1 = 0.5*(dB1-dB2);
		dD = (fabs(db1)<cDGN_Zero) ? dHw : sqrt(dHw * dHw + db1*db1);
	}
	else if (nSectType==D_SECT_TYPE_COMPO_TUB)
	{
		double db1 = 0.5*(dB1 + 2.0*(dBf1-dBf3) -dB2); 
		dD = (fabs(db1)<cDGN_Zero) ? dHw : sqrt(dHw * dHw + db1*db1);
	}
	else ASSERT(0);

	double dfck = InData.Matl.MatSlab.RC.dFck;
	double dEc  = InData.Matl.MatSlab.RC.dEc;

	// 철근 면적
	double dAr = 0.0; 
	int nRbarSize = InData.PosD[nPosi].Rbar.arRbarPosi.GetSize();
	for(int i=0; i<nRbarSize; ++i)
	{
		DGN_RBAR_CRC Rbar = InData.PosD[nPosi].Rbar.arRbarPosi.GetAt(i);

		dAr += Rbar.dArea;
	}

	double dfyr    = InData.Matl.MatSlab.RC.dFy;   // 주철근
	double dfyrSub = InData.Matl.MatSlab.RC.dFy2;  // 전단철근


	// 슬래브 재원                                                 
	RptData.MembD.Memb.dBs  = dBc;  // "Bs"    
	RptData.MembD.Memb.dts  = dtc;  // "ts"    
	RptData.MembD.Memb.dth  = dHh;  // "th"    
	RptData.MembD.Memb.dfck = dfck; // "fck"   
	RptData.MembD.Memb.dEc  = dEc;  // "Ec"    
	RptData.MembD.Memb.dAr  = dAr;  // "Ar"    
	RptData.MembD.Memb.dFyr = dfyr; // "Fyr"   
	
	// 거더-단면
	RptData.MembD.Memb.GIRDER.dbfc  = nSectType == D_SECT_TYPE_COMPO_I ? dB1 : (nSectType == D_SECT_TYPE_COMPO_TUB ? dBf1 : dB1 + dBf1 * 2.0);            // "bfc"     
	RptData.MembD.Memb.GIRDER.dbft  = nSectType == D_SECT_TYPE_COMPO_I ? dB2 : dB2 + dBf2 * 2.0;            // "bft"     
	RptData.MembD.Memb.GIRDER.dtfc  = dtf1;           // "tfc"     
	RptData.MembD.Memb.GIRDER.dtft  = dtf2;           // "tft"     
	RptData.MembD.Memb.GIRDER.dD    = dD;            // "D"     // 복부 경사  
	RptData.MembD.Memb.GIRDER.dtw   = dtw;            // "tw"   
	RptData.MembD.Memb.GIRDER.dH    = dHw; // + dtf1 + dtf2; // 복부 연직 
	RptData.MembD.Memb.GIRDER.dtft2 = dBf2;    // 하부플랜지 돌출폭
																																 
	// 거더-상부플랜지
	RptData.MembD.Memb.GIRDER.Top.strMatType = InData.Matl.MatStf.csMatlName; // "Top_mat" 
	RptData.MembD.Memb.GIRDER.Top.dthick     = dtf1; // "Top_thk" 
	RptData.MembD.Memb.GIRDER.Top.dfy_or     = InData.Matl.MatStf.Steel.S_Fy1;  // "Top_fyo" 항복강도  - 확인하기
	RptData.MembD.Memb.GIRDER.Top.dfy        = InData.Matl.MatStf.Steel.S_Fy; // "Top_fy"  두께별 항복강도 
	RptData.MembD.Memb.GIRDER.Top.dfu        = InData.Matl.MatStf.Steel.S_Fu; // "Top_fu"  
	RptData.MembD.Memb.GIRDER.Top.strMemo    = _T("-"); // "Top_memo"
	
	// 거더-하부플랜지
	RptData.MembD.Memb.GIRDER.Bottom.strMatType = InData.Matl.MatSbf.csMatlName; // "Bot_mat" 
	RptData.MembD.Memb.GIRDER.Bottom.dthick     = dtf2; // "Bot_thk" 
	RptData.MembD.Memb.GIRDER.Bottom.dfy_or     = InData.Matl.MatSbf.Steel.S_Fy1;  // "Bot_fyo" 
	RptData.MembD.Memb.GIRDER.Bottom.dfy        = InData.Matl.MatSbf.Steel.S_Fy; // "Bot_fy"  
	RptData.MembD.Memb.GIRDER.Bottom.dfu        = InData.Matl.MatSbf.Steel.S_Fu;  // "Bot_fu"  
	RptData.MembD.Memb.GIRDER.Bottom.strMemo    = _T("-"); // "Bot_memo"
	
	// 거더-복부
	RptData.MembD.Memb.GIRDER.Web.strMatType  = InData.Matl.MatSw.csMatlName; // "Web_mat" 
	RptData.MembD.Memb.GIRDER.Web.dthick      = dtw; // "Web_thk" 
	RptData.MembD.Memb.GIRDER.Web.dfy_or      = InData.Matl.MatSw.Steel.S_Fy1;  // "Web_fyo" 
	RptData.MembD.Memb.GIRDER.Web.dfy         = InData.Matl.MatSw.Steel.S_Fy; // "Web_fy"  
	RptData.MembD.Memb.GIRDER.Web.dfu         = InData.Matl.MatSw.Steel.S_Fu;  // "Web_fu"  
	RptData.MembD.Memb.GIRDER.Web.strMemo     = _T("-"); // "Web_memo"
	
	// 거더- 설계강도
	RptData.MembD.Memb.GIRDER.dFyc  = InData.Matl.MatStf.Steel.S_Fy; //"Fyc"    
	RptData.MembD.Memb.GIRDER.dFyw  = InData.Matl.MatSw.Steel.S_Fy; //"Fyw"    
	RptData.MembD.Memb.GIRDER.dFyt  = InData.Matl.MatSbf.Steel.S_Fy; //"Fyt"    
	RptData.MembD.Memb.GIRDER.dEs   = InData.Matl.MatStf.Steel.Elast; //"Es"     

	/////
	// 단면계수- 강재단면
	SetRptData_SectStiffness( nPosi,  InData.PosD[nPosi].Sect.SectInfo.Before.Stiffness, RptData.MembD.Memb.Steel); 
	// 단면계수- 단기합성단면
	SetRptData_SectStiffness( nPosi,  InData.PosD[nPosi].Sect.RbarStiffn,    RptData.MembD.Memb.ShortComp); 
	// 단면계수- 장기합성단면
	SetRptData_SectStiffness( nPosi,  InData.PosD[nPosi].Sect.RbarStiff3n,   RptData.MembD.Memb.LongComp); 
	// 단면계수- 강재+철근(부모멘트)
	SetRptData_SectStiffness( nPosi,  InData.PosD[nPosi].Sect.NonSlabStiff,  RptData.MembD.Memb.Steel_M); 
	// 단면계수- 강재+철근(부모멘트)
	//SetRptData_SectStiffness( nPosi,  InData.PosD[nPosi].Sect.NonSlabStiff2, RptData.MembD.Memb.SteelRebarComp); 
}


void CRptSteelGirder_LSD12::SetRptData_SectStiffness(int nPosi,  DGN_SECT_STIFFNESS & InData, CSG_SectionProperty& RptData) 
{
	double dIny = InData.dIyy;
	double dZp  = InData.dCzp;
	double dZm  = InData.dCzm;
	RptData.dA      = InData.dArea;                   // "A"    
	RptData.dINA      = InData.dIyy;                    // "Iy"  
	RptData.dIzz    = InData.dIzz;                    // "Iz" 
	RptData.ddTop   = InData.dCzp;                    // "dtop" 
	RptData.dSTop   = (dZp==0.0)? 0.0 : dIny/dZp;     // "Stop" 
	RptData.ddBot   = InData.dCzm;                    // "dbot" 
	RptData.dSBot   = (dZm==0.0)? 0.0 : dIny/dZm;     // "Sbot" 

}


BOOL CRptSteelGirder_LSD12::ShouldCheckPosiIJ(int nFlag, BOOL& bCheckI, BOOL& bCheckJ)
{

	if      (nFlag==0)  {bCheckI=TRUE ; bCheckJ=FALSE;}	// I.
	else if (nFlag==1)  {bCheckI=FALSE; bCheckJ=TRUE;}	// J.
	else if (nFlag==2)  {bCheckI=TRUE ; bCheckJ=TRUE;}	// I&J.
	else	  {}	

	return TRUE;
}


void CRptSteelGirder_LSD12::ConvertCsgCalcUlmForce(T_CSGD_ULMF &For, CSG_ULMF_D &rData)
{
	ConvertCsgCalcForce( For.For[0], rData.ForD[0]);
	ConvertCsgCalcForce( For.For[1], rData.ForD[1]);
	ConvertCsgCalcForce( For.For[2], rData.ForD[2]);
}

void CRptSteelGirder_LSD12::ConvertCsgCalcChkForce(T_CSGD_COMF &For, CSG_FORCE_COMP &rData)
{
	ConvertCsgCalcForce( For.ForNC, rData.ForNC);
	ConvertCsgCalcForce( For.ForLT, rData.ForLT);
	ConvertCsgCalcForce( For.ForST, rData.ForST);
	ConvertCsgCalcForce( For.ForLC, rData.ForLC);

}


void CRptSteelGirder_LSD12::ConvertCsgCalcChkForce4Stage(int nPos, _DGN_FORC_CRC &ForNC, CSG_FORCE_COMP &rData)
{

	//m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgCalcForce(nPos, ForNC, rData.ForNC);

}


void CRptSteelGirder_LSD12::ConvertCsgCalcForce( T_CSGD_FORCE &For, CSG_FORCE_D &rData)
{
	rData.dForce[0] = For.dForce[0];
	rData.dForce[1] = For.dForce[1];
	rData.dForce[2] = For.dForce[2];
	rData.dForce[3] = For.dForce[3];
	rData.dForce[4] = For.dForce[4];
	rData.dForce[5] = For.dForce[5];
}

void CRptSteelGirder_LSD12::SetPrintPosition(ElemPairK ElemK, BOOL& bPrintI, BOOL& bPrintJ)
{
	// 1. 출력 여부 결정
	if(ElemK.first==0)
	{
		bPrintI = FALSE;
		bPrintJ = FALSE;
		return;
	}
	
	T_CGDP_D CgdpD; CgdpD.Initialize();  // 설계 위치
	T_CGRE_D CgreD; CgreD.Initialize();  // 출력 위치

	if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetCgdp(ElemK.first, CgdpD)) CgdpD.Initialize();
		if (!m_pDoc->m_pAttrCtrl2->GetCgre(ElemK.first, CgreD)) CgreD.Initialize();
	}
	else if (ElemK.second == EN_EL_VBEAM)
	{
		T_CGDV_D CgdvD; CgdvD.Initialize();
		T_CGRV_D CgrvD; CgrvD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetCgdv(ElemK.first, CgdvD)) CgdvD.Initialize();
		CgdpD=CgdvD;
		if (!m_pDoc->m_pAttrCtrl2->GetCgrv(ElemK.first, CgrvD)) CgrvD.Initialize();
		CgreD=CgrvD;
	}
	else ASSERT(0);
	
	BOOL bCheckCalcI = TRUE;
	BOOL bCheckCalcJ = TRUE;
	ShouldCheckPosiIJ(CgdpD.iDgnPosi, bCheckCalcI, bCheckCalcJ);
	BOOL bCheckPrintI = TRUE;
	BOOL bCheckPrintJ = TRUE;
	ShouldCheckPosiIJ(CgreD.iDgnPosi, bCheckPrintI, bCheckPrintJ);
	bPrintI = (bCheckCalcI && bCheckPrintI);
	bPrintJ = (bCheckCalcJ && bCheckPrintJ);

}

void CRptSteelGirder_LSD12::SetSpanRptData(CArray<CSG_INDATA_SPAN, CSG_INDATA_SPAN&>& aSpanInData, CArray<CSG_DRES_SPAN_PRT, CSG_DRES_SPAN_PRT&>& aSpanRptData)
{
	CArray<CSG_INDATA_SPAN, CSG_INDATA_SPAN&> aSpanInElem;
	CArray<CSG_DRES_SPAN_PRT, CSG_DRES_SPAN_PRT&> aSpanRptElem;
	GetSpanRptDataBase(EN_EL_BEAM, aSpanInElem, aSpanRptElem);
	aSpanInData.Append(aSpanInElem);
	aSpanRptData.Append(aSpanRptElem);
	//
	CArray<CSG_INDATA_SPAN, CSG_INDATA_SPAN&> aSpanInVBeam;
	CArray<CSG_DRES_SPAN_PRT, CSG_DRES_SPAN_PRT&> aSpanRptVBeam;
	GetSpanRptDataBase(EN_EL_VBEAM, aSpanInVBeam, aSpanRptVBeam);
	aSpanInData.Append(aSpanInVBeam);
	aSpanRptData.Append(aSpanRptVBeam);
}

void CRptSteelGirder_LSD12::GetSpanRptDataBase(const EN_BEAM_DGN_TYPE& enBeamType, CArray<CSG_INDATA_SPAN, CSG_INDATA_SPAN&>& aSpanInData,   CArray<CSG_DRES_SPAN_PRT,  CSG_DRES_SPAN_PRT&>& aSpanRptData)
{


	CSG_MEMB_POSD_KSCE_LSD  MembLcomD; 
	CSG_DRES_PRT  RptData;

	T_CSBR_D CsbrD; 
	T_CSVR_D CsvrD;
	T_CSAC_D CsacD;
	T_CSFC_D CsfcD;
	T_SCBR_D ScbrD;
	T_SCVR_D ScvrD;      
	T_CSCR_D CscrD;
	T_CSSL_D CsslD;

	CSG_MEMB_RESULT_D      MembR;
	CSG_CHECK_RESULT_CODES ChkResD;

	MembR.pLSD    = new CSG_MEMBPOS_RES_KSCE_LSD12_D;

	ChkResD.pLSD    = new CSG_CHECK_RES_KSCE_LSD12;

	
//   CSG_STRN_RES_KSCE_LSD12  StrnResD; // 강도한계
//   CSG_SERV_RES_KSCE_LSD12  ServResD; // 사용한계
//   CSG_FATI_RES_KSCE_LSD12  FatiResD; // 피로한계
//   CSG_CONS_RES_KSCE_LSD12  ConsResD; // 시공성
//   CSG_CSCR_RES_KSCE_LSD12  CscrResD; // 전단연결재
//   CSG_CSSL_RES_KSCE_LSD12  CsslResD; // 수평보강재

	CPlateGirderDesign *pCheckGirder = m_pDoc->m_pPostCtrl->GetPlateGirder();
	if (pCheckGirder==NULL) return;
	
	CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
	if (enBeamType == EN_EL_BEAM)
	{
		m_pDoc->m_pAttrCtrl->GetSpanKeyList(aSpanKeyList);
	}
	else if (enBeamType == EN_EL_VBEAM)
	{
		m_pDoc->m_pAttrCtrl->GetSpavKeyList(aSpanKeyList);
	}
	else ASSERT(0);
	
	int nSpanKeySize = aSpanKeyList.GetSize();  

	aSpanInData.RemoveAll();  //aSpanInData.SetSize(nSpanKeySize);
	aSpanRptData.RemoveAll(); //aSpanRptData.SetSize(nSpanKeySize);
	
	T_SPAN_D SpanD; 
	T_CSPR_D CsprD; 
	BOOL bPrintI=TRUE;
	BOOL bPrintJ=TRUE;
	BOOL bRptCheck = TRUE;

	CSG_INDATA_SPAN   SpanInData;
	CSG_DRES_SPAN_PRT SpanRptData;

	for(int i=0; i<nSpanKeySize; ++i)
	{
		SpanInData.Initialize();
		SpanRptData.Initialize();

		T_SPAN_K SpanK = aSpanKeyList[i]; 
		SpanPairK SPairK(SpanK, enBeamType);

		SpanD.Initialize();
		m_pDoc->m_pAttrCtrl->GetSpan(SpanK, SpanD);

		SpanInData.SpanName = SpanD.Name;

		CsprD.Initialize();
		if(!pCheckGirder->ReadCSPR_LSD12(SPairK, CsprD)) continue; 

		if(!CheckSpanData(CsprD)) continue;

		CSG_CHK_RES_DTR_PARAM  DtrR;

		int ElemK = 0;
		int nPos  = 0;
		int j = 0;

		CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&> aSpInData;
		CArray<CSG_DRES_PRT,  CSG_DRES_PRT&> aSpRptData;
		aSpInData.RemoveAll();   //aSpInData.SetSize(12);  // 12 : 휨(정), 휨(부)등의 검토항목 수 
		aSpRptData.RemoveAll();  //aSpRptData.SetSize(12);

		// ********************
		// 휨(정/부)
		// ********************
		for(int nPosNeg=0; nPosNeg<2; ++nPosNeg) // 0: 정 1: 부
		{
			ElemK = CsprD.Flex[nPosNeg].ElemK;
			nPos  = CsprD.Flex[nPosNeg].nPos; // 0: I, 1: J  
			ElemPairK EPairK(CsprD.Flex[nPosNeg].ElemK, CsprD.Flex[nPosNeg].nBeamType);
			
			// 출력 위치
			SetPrintPosition(EPairK, bPrintI, bPrintJ);
		 
			bRptCheck = TRUE;
			j = (nPos <=1)? 0 : 1; // I,J 
			if(ElemK==0)           bRptCheck = FALSE;
			if     (j==0 && (!bPrintI)) bRptCheck = FALSE;
			else if(j==1 && (!bPrintJ)) bRptCheck = FALSE;
			if(!m_CpgdD.bDesignParameter[0])                bRptCheck = FALSE;
			if(!pCheckGirder->ReadCSBR_LSD12(EPairK, CsbrD)) bRptCheck = FALSE;
			if(!CsbrD.CsbrB[nPos].bChk)                     bRptCheck = FALSE; // 설계결과가 없으면 Continue                 
			
			if(bRptCheck)
			{
				// 요소별 출력용 입력데이터
				SetElemRptData(EPairK, nPos, MembLcomD, RptData);

				MembR.pLSD->MembR[nPos] = RptData.MembD;

				ConvertCsgCalcUlmForce(CsbrD.CsbrB[nPos].UlForLC,  MembLcomD.aUlmf[0]);
				ConvertCsgCalcChkForce(CsbrD.CsbrB[nPos].ComFor, MembLcomD.aChkFor[0]);
				m_pDataCtrl->m_pCsgDataCtrl->Check_Strength(EPairK, nPosNeg, MembLcomD, MembR, ChkResD, DtrR);

				RptData.FlexD[nPosNeg].FlexD = ChkResD.pLSD->StrnD.FlexD;
				RptData.FlexD[nPosNeg].FlexD.ForceInfo_F.strLcomName = Get_LcomName(CsbrD.CsbrB[nPos].nOrgLcomK);
				RptData.FlexD[nPosNeg].FlexD.ForceInfo_F.strLcomType = Get_LcomType(CsbrD.CsbrB[nPos].nMax);

				MembLcomD.Memb.ChIf.nElem = ElemK;
				MembLcomD.Memb.ChIf.nChkPos = j;

				MembLcomD.nCalcType = (nPosNeg==0)? 0 : 1;
				aSpInData.Add(MembLcomD);
				aSpRptData.Add(RptData);
			}

		}


		// ********************
		// 전단
		// ********************
		ElemK = CsprD.Shear.ElemK;
		nPos  = CsprD.Shear.nPos; // 0: I, 1: J
		ElemPairK SEPairK(CsprD.Shear.ElemK, CsprD.Shear.nBeamType);
		// 출력 위치
		SetPrintPosition(SEPairK, bPrintI, bPrintJ);

		bRptCheck = TRUE;
		j = (nPos <=1)? 0 : 1; // I,J 
		if     (j==0 && (!bPrintI)) bRptCheck = FALSE;
		else if(j==1 && (!bPrintJ)) bRptCheck = FALSE;
		if(!pCheckGirder->ReadCSVR_LSD12(SEPairK, CsvrD)) bRptCheck = FALSE;
		if(!m_CpgdD.bDesignParameter[1])               bRptCheck = FALSE;
		if(!CsvrD.CsvrB[j].bChk)                       bRptCheck = FALSE; // 0 : I-max  1: J-max

		if(bRptCheck)
		{
			// 요소별 출력용 입력데이터
			SetElemRptData(SEPairK, nPos, MembLcomD, RptData);

			MembR.pLSD->MembR[nPos] = RptData.MembD;

			ConvertCsgCalcUlmForce(CsvrD.CsvrB[j].UlFor,  MembLcomD.aUlmf[0]);
			ConvertCsgCalcChkForce(CsvrD.CsvrB[j].ComFor, MembLcomD.aChkFor[0]);
			m_pDataCtrl->m_pCsgDataCtrl->Check_Strength(SEPairK, 0, MembLcomD, MembR, ChkResD, DtrR);

			RptData.ShearD = ChkResD.pLSD->StrnD.ShearD;
			RptData.ShearD.ForceInfo_S.strLcomName = Get_LcomName(CsvrD.CsvrB[j].nOrgLcomK);
			RptData.ShearD.ForceInfo_S.strLcomType = Get_LcomType(CsvrD.CsvrB[j].nMax);

			MembLcomD.Memb.ChIf.nElem = ElemK;
			MembLcomD.Memb.ChIf.nChkPos = j;

			MembLcomD.nCalcType = 2;
			aSpInData.Add(MembLcomD);
			aSpRptData.Add(RptData);

		}


		// ********************
		// 사용한계(정/부)
		// ********************
		for(int nPosNeg=0; nPosNeg<2; ++nPosNeg) // 0: 정 1: 부
		{
			ElemK = CsprD.Serv[nPosNeg].ElemK;
			nPos  = CsprD.Serv[nPosNeg].nPos; // 0: I, 1: J  
			ElemPairK EPairK(CsprD.Serv[nPosNeg].ElemK, CsprD.Serv[nPosNeg].nBeamType);
			// 출력 위치
			SetPrintPosition(EPairK, bPrintI, bPrintJ);
		 
			bRptCheck = TRUE;
			j = (nPos <=1)? 0 : 1; // I,J 
			if     (j==0 && (!bPrintI)) bRptCheck = FALSE;
			else if(j==1 && (!bPrintJ)) bRptCheck = FALSE;
			if(!m_CpgdD.bDesignParameter[2])                bRptCheck = FALSE;
			if(!pCheckGirder->ReadCSAC_LSD12(EPairK, CsacD))  bRptCheck = FALSE;
			if(!CsacD.CsacB[nPos].bChk)                     bRptCheck = FALSE; // 설계결과가 없으면 Continue                 
			
			if(bRptCheck)
			{
				// 요소별 출력용 입력데이터
				SetElemRptData(EPairK, nPos, MembLcomD, RptData);

				MembR.pLSD->MembR[nPos] = RptData.MembD;

				ConvertCsgCalcUlmForce(CsacD.CsacB[nPos].UlFor,  MembLcomD.aUlmf[0]);
				ConvertCsgCalcChkForce(CsacD.CsacB[nPos].ComFor, MembLcomD.aChkFor[0]);
				m_pDataCtrl->m_pCsgDataCtrl->Check_Service(EPairK, nPosNeg, MembLcomD, MembR, ChkResD, DtrR);
				
				RptData.ServD[nPosNeg] = ChkResD.pLSD->ServD; //ServResD;
				RptData.ServD[nPosNeg].ForceInfo_Serv.strLcomName = Get_LcomName(CsacD.CsacB[nPos].nOrgLcomK);
				RptData.ServD[nPosNeg].ForceInfo_Serv.strLcomType = Get_LcomType(CsacD.CsacB[nPos].nMax);

				MembLcomD.Memb.ChIf.nElem = ElemK;
				MembLcomD.Memb.ChIf.nChkPos = j;

				MembLcomD.nCalcType = (nPosNeg==0)? 3 : 4;
				aSpInData.Add(MembLcomD);
				aSpRptData.Add(RptData);

			}

		}


		// ********************
		// 시공성 휨(정/부)
		// ********************
		for(int nPosNeg=0; nPosNeg<2; ++nPosNeg) // 0: 정 1: 부
		{
			ElemK = CsprD.ConF[nPosNeg].ElemK;
			nPos  = CsprD.ConF[nPosNeg].nPos; // 0: I, 1: J    
			ElemPairK EPairK(CsprD.ConF[nPosNeg].ElemK, CsprD.ConF[nPosNeg].nBeamType);
			// 출력 위치
			SetPrintPosition(EPairK, bPrintI, bPrintJ);

			bRptCheck = TRUE;
			j = (nPos <=1)? 0 : 1; // I,J 
			if     (j==0 && (!bPrintI)) bRptCheck = FALSE;
			else if(j==1 && (!bPrintJ)) bRptCheck = FALSE;
			if(!m_CpgdD.bDesignParameter[3])                bRptCheck = FALSE;
			if(!pCheckGirder->ReadSCBR_LSD12(EPairK, ScbrD))  bRptCheck = FALSE;
			if(!ScbrD.ScbrB[nPos].bChk)                     bRptCheck = FALSE; // 설계결과가 없으면 Continue                 

			if(bRptCheck)
			{
				// 요소별 출력용 입력데이터
				SetElemRptData(EPairK, nPos, MembLcomD, RptData);
				
				MembR.pLSD->MembR[nPos] = RptData.MembD;

				ConvertCsgCalcUlmForce(ScbrD.ScbrB[nPos].UlFor,  MembLcomD.aUlmf[0]);
				//ConvertCsgCalcChkForce4Stage(ScbrD.ScbrB[nPos].CsFor,  MembLcomD.aChkFor[0]);
				m_pDataCtrl->m_pCsgDataCtrl->Check_Constructibility(EPairK, nPosNeg, MembLcomD, MembR, ChkResD, DtrR);

				RptData.ConsD[nPosNeg].Flexure = ChkResD.pLSD->ConsD.Flexure; //ConsResD.Flexure;
				RptData.ConsD[nPosNeg].Flexure.strStage = Get_StageName(ScbrD.ScbrB[nPos].nStageK);
				RptData.ConsD[nPosNeg].Flexure.StepK = ScbrD.ScbrB[nPos].nStepK;

				MembLcomD.Memb.ChIf.nElem = ElemK;
				MembLcomD.Memb.ChIf.nChkPos = j;

				MembLcomD.nCalcType = (nPosNeg==0)? 5 : 6;
				aSpInData.Add(MembLcomD);
				aSpRptData.Add(RptData);

			}
		}

		// ********************
		// 시공성 전단
		// ********************
		ElemK = CsprD.ConS.ElemK;
		nPos  = CsprD.ConS.nPos; // 0: I, 1: J  
		ElemPairK EPairCsprK(CsprD.ConS.ElemK, CsprD.ConS.nBeamType);
		// 출력 위치
		SetPrintPosition(EPairCsprK, bPrintI, bPrintJ);
		
		bRptCheck = TRUE;
		j = (nPos <=1)? 0 : 1; // I,J 
		if     (j==0 && (!bPrintI)) bRptCheck = FALSE;
		else if(j==1 && (!bPrintJ)) bRptCheck = FALSE;
		if(!pCheckGirder->ReadSCVR_LSD12(EPairCsprK, ScvrD)) bRptCheck = FALSE;
		if(!m_CpgdD.bDesignParameter[3])               bRptCheck = FALSE;
		if(!ScvrD.ScvrB[j].bChk)                       bRptCheck = FALSE; // 0 : I-max  1: J-max
		
		if(bRptCheck)
		{
			// 요소별 출력용 입력데이터
			SetElemRptData(EPairCsprK, nPos, MembLcomD, RptData);
			
			MembR.pLSD->MembR[nPos] = RptData.MembD;

			ConvertCsgCalcUlmForce(ScvrD.ScvrB[j].UlFor,  MembLcomD.aUlmf[0]);
			//ConvertCsgCalcChkForce4Stage(ScvrD.ScvrB[j].CsFor, MembLcomD.aChkFor[0]);
			m_pDataCtrl->m_pCsgDataCtrl->Check_Constructibility(EPairCsprK, 0, MembLcomD, MembR, ChkResD, DtrR);
			
			RptData.ConShearD = ChkResD.pLSD->ConsD.Shear;
			RptData.ConShearD.strStage = Get_StageName(ScvrD.ScvrB[j].nStageK);
			RptData.ConShearD.StepK = ScvrD.ScvrB[nPos].nStepK;

			MembLcomD.Memb.ChIf.nElem = ElemK;
			MembLcomD.Memb.ChIf.nChkPos = j;

			MembLcomD.nCalcType = 7;
			aSpInData.Add(MembLcomD);
			aSpRptData.Add(RptData);

		}

		// ********************
		// 피로한계(정/부)
		// ********************
		//for(nPosNeg=0; nPosNeg<2; ++nPosNeg) // 0: 정 1: 부
		{
			ElemK = CsprD.Fati[0].ElemK;
			nPos  = CsprD.Fati[0].nPos; // 0: I-max, 1: J-max,  
			ElemPairK EPairFatiK(CsprD.Fati[0].ElemK, CsprD.Fati[0].nBeamType);
			// 출력 위치
			SetPrintPosition(EPairFatiK, bPrintI, bPrintJ);
			
			bRptCheck = TRUE;
			j = (nPos <=1)? 0 : 1; // I,J 
			if     (j==0 && (!bPrintI)) bRptCheck = FALSE;
			else if(j==1 && (!bPrintJ)) bRptCheck = FALSE;
			if(!m_CpgdD.bDesignParameter[4])                bRptCheck = FALSE;
			if(!pCheckGirder->ReadCSFC_LSD12(EPairFatiK, CsfcD))  bRptCheck = FALSE;
			if(!CsacD.CsacB[nPos].bChk)                     bRptCheck = FALSE; // 설계결과가 없으면 Continue                 
			
			if(bRptCheck)
			{
				// 요소별 출력용 입력데이터
				SetElemRptData(EPairFatiK, nPos, MembLcomD, RptData);

				MembR.pLSD->MembR[nPos] = RptData.MembD;

				MembLcomD.aChkFor.RemoveAll(); MembLcomD.aChkFor.SetSize(6);
				T_CSFC_LOAD &LoadTop = CsfcD.CsfcB[j].LoadR[0];
				T_CSFC_LOAD &LoadBot = CsfcD.CsfcB[j].LoadR[1];
				T_CSFC_FLEX &FlexR   = CsfcD.CsfcB[j].FlexR;
				T_CSFC_SHEAR &ShearR = CsfcD.CsfcB[j].ShearR;
				ConvertCsgCalcChkForce(LoadTop.ComForMV.ForMV[LoadTop.nForId[0]], MembLcomD.aChkFor[0]);
				ConvertCsgCalcChkForce(LoadTop.ComForMV.ForMV[LoadTop.nForId[1]], MembLcomD.aChkFor[1]);
				ConvertCsgCalcChkForce(LoadBot.ComForMV.ForMV[LoadBot.nForId[0]], MembLcomD.aChkFor[2]);
				ConvertCsgCalcChkForce(LoadBot.ComForMV.ForMV[LoadBot.nForId[0]], MembLcomD.aChkFor[3]);
				ConvertCsgCalcChkForce(FlexR.ComFor     , MembLcomD.aChkFor[4]);
				ConvertCsgCalcChkForce(ShearR.ComFor    , MembLcomD.aChkFor[5]);

				CSG_FATI_CR_RSLT FatiCrResD;
				m_pDataCtrl->m_pCsgDataCtrl->Check_Fatigue(EPairFatiK, 0, MembLcomD, MembR, ChkResD, DtrR, FatiCrResD);
				SetFatigPrtData( CsfcD.CsfcB[j], ChkResD.pLSD->FatiD, RptData.FatiD);
				
				MembLcomD.Memb.ChIf.nElem = ElemK;
				MembLcomD.Memb.ChIf.nChkPos = j;
				
				MembLcomD.nCalcType = 8 ;
				aSpInData.Add(MembLcomD);
				aSpRptData.Add(RptData);
				
			}
		}


		// ********************
		// 전단연결재
		// ********************
		ElemK = CsprD.SCon.ElemK;
		nPos  = CsprD.SCon.nPos; // 0: I-max, 1: J-max,   
		ElemPairK EPairSconK(CsprD.SCon.ElemK, CsprD.SCon.nBeamType);
		// 출력 위치
		SetPrintPosition(EPairSconK, bPrintI, bPrintJ);
		
		bRptCheck = TRUE;
		j = (nPos <=1)? 0 : 1; // I,J 
		if     (j==0 && (!bPrintI)) bRptCheck = FALSE;
		else if(j==1 && (!bPrintJ)) bRptCheck = FALSE;
		if(!pCheckGirder->ReadCSCR_LSD12(EPairSconK, CscrD)) bRptCheck = FALSE;
		if(!m_CpgdD.bDesignParameter[5])               bRptCheck = FALSE;
		if(!CscrD.CscrB[j].bChk)                       bRptCheck = FALSE; // 0 : I-max  1: J-max
		
		if(bRptCheck)
		{
			// 요소별 출력용 입력데이터
			SetElemRptData(EPairSconK, nPos, MembLcomD, RptData);

			SetShearConnectPrtData(CscrD.CscrB[j],  RptData.CscrD);
					 
			MembLcomD.Memb.ChIf.nElem = ElemK;
			MembLcomD.Memb.ChIf.nChkPos = j;

			MembLcomD.nCalcType = 9;
			aSpInData.Add(MembLcomD);
			aSpRptData.Add(RptData);

		}


		// ********************
		// 수평보강재
		// ********************
		ElemK = CsprD.LStfn.ElemK;
		nPos  = CsprD.LStfn.nPos; // 0: I-max, 1: J-max,  
		ElemPairK EPairLStfnK(CsprD.LStfn.ElemK, CsprD.LStfn.nBeamType);
		// 출력 위치
		SetPrintPosition(EPairLStfnK, bPrintI, bPrintJ);
		
		bRptCheck = TRUE;
		j = (nPos <=1)? 0 : 1; // I,J 
		if     (j==0 && (!bPrintI)) bRptCheck = FALSE;
		else if(j==1 && (!bPrintJ)) bRptCheck = FALSE;
		if(!pCheckGirder->ReadCSSL_LSD12(EPairLStfnK, CsslD)) bRptCheck = FALSE;
		if(!m_CpgdD.bDesignParameter[5])               bRptCheck = FALSE;
		if(!CsslD.CsslB[j].bChk)                       bRptCheck = FALSE; // 0 : I-max  1: J-max
		
		if(bRptCheck)
		{
			 // 부재력 데이터 없음.

			MembLcomD.Memb.ChIf.nElem = ElemK;
			MembLcomD.Memb.ChIf.nChkPos = j;
			
			MembLcomD.nCalcType = 10;
			aSpInData.Add(MembLcomD);
			aSpRptData.Add(RptData);
		
		}

		SpanInData.aSpanRptIn.Copy(aSpInData);
		SpanRptData.aSpanPpt.Copy(aSpRptData);

		aSpanInData.Add(SpanInData);
		aSpanRptData.Add(SpanRptData);
	}

	if (MembR.pLSD    != NULL) delete MembR.pLSD;

	if (ChkResD.pLSD    != NULL) delete ChkResD.pLSD;
}


void CRptSteelGirder_LSD12::SetElemRptData(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_DRES_PRT& RptData)
{
	MembLcomD.Initialize();
	// Input Data Setting(1)
	T_CPGD_D CpgdD; CpgdD.Initialize();  
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);
	m_pDataCtrl->m_pCsgDataCtrl->ConvertCGSGlobalData(CpgdD, MembLcomD.Memb.Csgd);

 
	// 2. Input Data Setting(2)
	T_ELEM_D ElemD; ElemD.Initialize();
	if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD)) return;
	if(!m_pDataCtrl->m_pCsgDataCtrl->m_mCsgMatl.Lookup(ElemD.elmat, MembLcomD.Memb.Matl))
	{
		MembLcomD.Memb.Matl.Initialize();
	}
	
	m_pDataCtrl->m_pCsgDataCtrl->GetCsgSectionSupport(ElemK, MembLcomD);
	/// - MembLcomD.Memb.PosD의 다른 데이터들 받기
	m_pDataCtrl->m_pCsgDataCtrl->Get_CsgMembInData(ElemK, MembLcomD);
	
	/// - 하중정보는 계산전에 받음
	int nChkLcomSize=1;
	MembLcomD.aUlmf.SetSize(nChkLcomSize);    // Unbraced Length Force
	MembLcomD.aChkFor.SetSize(nChkLcomSize);  // Check Force;

	// Unbraced Length Setting
	GetUnBracedLenthData(ElemK, MembLcomD.Memb.Ulen);
	
	// Unbraced Length Member Section
	m_pDataCtrl->m_pCsgDataCtrl->Get_CsgUlmSection(ElemK, MembLcomD);
	
	// 3. Output Data Setting(1)
	RptData.Initialize();
	CSG_MEMB_RES_KSCE_LSD12_D MembResD;
	m_pDataCtrl->m_pCsgDataCtrl->MakeMemberResData(ElemK.first, MembLcomD, MembResD);   
	RptData.MembD = MembResD;


	// 데이터 변환
	SetRptData(ElemK.first, nPos, MembLcomD.Memb, RptData);

}

BOOL CRptSteelGirder_LSD12::CheckSpanData(T_CSPR_D &CsprD)
{
	int ElemK = 0;
	ElemK  = ElemK + CsprD.Flex[0].ElemK;
	ElemK  = ElemK + CsprD.Flex[1].ElemK;
	ElemK  = ElemK + CsprD.Shear.ElemK;
	ElemK  = ElemK + CsprD.Serv[0].ElemK;
	ElemK  = ElemK + CsprD.Serv[1].ElemK;
	ElemK  = ElemK + CsprD.Fati[0].ElemK;
	ElemK  = ElemK + CsprD.ConF[0].ElemK;
	ElemK  = ElemK + CsprD.ConF[1].ElemK;
	ElemK  = ElemK + CsprD.ConS.ElemK;
	ElemK  = ElemK + CsprD.SCon.ElemK;
	ElemK  = ElemK + CsprD.LStfn.ElemK;  
	
	if( ElemK==0)   return FALSE;
	else            return TRUE;
}


CStringW CRptSteelGirder_LSD12::Get_StageName(T_STAG_K StagK)
{
	CString strStageName=_T("");
	T_STAG_D StagD;
	if (m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD)) strStageName = StagD.StageName;
	
	CStringW strwText(strStageName); 
	return strwText;
}

CStringW CRptSteelGirder_LSD12::Get_LcomName(T_LCOM_K LcomK)
{
	CString strLcomName;
	T_LCOM_D LcomData;
	if(m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, LcomK, LcomData)) strLcomName = LcomData.LoadCombName;    
	
	CStringW strwText(strLcomName); 
	return strwText;
}


CStringW CRptSteelGirder_LSD12::Get_LcomType(int nMax)
{

	LPCTSTR aType[] = {_T("-"), _LS(IDS_DGN_CSD_FX_MAX), _LS(IDS_DGN_CSD_FX_MIN), _LS(IDS_DGN_CSD_FY_MAX), _LS(IDS_DGN_CSD_FY_MIN), 
							_LS(IDS_DGN_CSD_FZ_MAX), _LS(IDS_DGN_CSD_FZ_MIN), _LS(IDS_DGN_CSD_MX_MAX), _LS(IDS_DGN_CSD_MX_MIN),
							_LS(IDS_DGN_CSD_MY_MAX), _LS(IDS_DGN_CSD_MY_MIN), _LS(IDS_DGN_CSD_MZ_MAX), _LS(IDS_DGN_CSD_MZ_MIN), 
                            _LS(IDS_DGN_CSD_Mbi_MAX), _LS(IDS_DGN_CSD_Mbi_MIN), 
                            _LS(IDS_DGN_CSD_Mt_MAX), _LS(IDS_DGN_CSD_Mt_MIN), 
                            _LS(IDS_DGN_CSD_Mw_MAX), _LS(IDS_DGN_CSD_Mw_MIN), _T("-") };

    int nSize = sizeof(aType) / sizeof(LPCSTR);
	
	CString strLcomType = _T("-");	
    if ( nSize>nMax )
    {
        strLcomType = aType[nMax];
    }

	CStringW strwText(strLcomType); 
	return strwText;
}


void CRptSteelGirder_LSD12::GetUnBracedLenthData(ElemPairK ElemK, CSG_MEMB_ULEN &Ulen)
{

	T_KFAC_D KFacD; KFacD.Initialize();
	BOOL bGetKfac = m_pDataCtrl->GetGenKfactor(ElemK, KFacD);
	m_pDataCtrl->m_pCsgDataCtrl->Get_UnbracedLengthData(ElemK, bGetKfac, KFacD, Ulen);

}

// void CRptSteelGirder_LSD12::GetCsgUnbracedLengthData4CS(T_ELEM_K ElemK, int k, UINT StagK, UINT StepK,  CSG_MEMB_POSD_KSCE_LSD& MembLcomD)
// {
//   CStageInfo *pStageInfo = m_pDoc->m_pPostCtrl->GetStageInfo();
// 
//   // 1. ForcMax
//   T_STAG_STEP_INFO StageInfo; StageInfo.Initialize();
//   m_pDoc->m_pAttrCtrl->GetStagStepInfo(StagK,StageInfo);
//   int nStepSize = StageInfo.aInfo.GetSize();
//   
//   CPG_CSIF_D CsifKD;
//   CsifKD.StagK = StagK;
//   if(nStepSize==0)
//   {
//     int nFinalStepNo=0;
//     m_pDataCtrl->Get_LastStepInCurStage(StagK, nFinalStepNo);
//     CsifKD.StagK        = StagK;
//     CsifKD.bIsStep      = FALSE;
//     CsifKD.nSerialStepK = nFinalStepNo;
//   }
//   else
//   {
//     int nStep=StepK; 
//     int nSerialStep=0;
//     pStageInfo->GetStepSerialKey(StagK,nStep,nSerialStep);
//     CsifKD.StagK        = StagK;
//     CsifKD.bIsStep      = TRUE;
//     CsifKD.nStepK       = nStep;
//     CsifKD.nSerialStepK = nSerialStep;
//   }
//   
//   CString strSgldName = _LS(IDS_DB_SGLD_SUM);	// SUM.
//   T_SGLD_K SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
//   
//   _DGN_FORC_CRC ForcMax;
//   _DGN_FORC_CRC ForcMin;                
//   if(!m_pDataCtrl->m_pCsgDataCtrl->m_pForcCtrl->Get_ForceBySgld(ElemK, SgldK, StagK, CsifKD.nSerialStepK, ForcMax, ForcMin))	continue;
//   ////
// 
//   // 2. MembLcomD.aChkFor[0]
//   ConvertCsgCalcChkForce4Stage(k, ForcMax, MembLcomD.aChkFor[0]);
// 
//   // 3. MembLcomD.Memb.Ulms
//   T_SPAN_K CurSpanK=0;
//   T_ELEM_K nElemI=ElemK, nElemM=ElemK, nElemJ=ElemK;
//   int nPosI=0;
//   int nPosM=0; // 임시, 찾아야함. 
//   int nPosJ=1;
//   m_pDataCtrl->m_pCsgDataCtrl->Get_CsgUlmSection(nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, MembLcomD.Memb.Ulms);  
// 
// 
//   // 4. MembLcomD.aUlmf[0]
//   _DGN_FORC_CRC &SpanForI = ForcMax;
//   _DGN_FORC_CRC SpanForM;
//   _DGN_FORC_CRC &SpanForJ = ForcMax;
//   ConvertCsgCalcUlmForce(nPosI, nPosM, nPosJ, SpanForI, SpanForM, SpanForJ, MembLcomD.aUlmf[0]);
// 
// }