#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "RptRatingKR_STL_ASD.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\AnalysisResult.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\DBCodeDef.h"

#include "Dgn_RatingRptManager.h"
#include "DgnStlRatingDataCtrl.h"
#include "DgnPscCommon.h"
#include "DgnDataCtrl.h"
#include "RptKRRating.h"
#include "DgnCsgDataCtrl.h"
#include "RptSteelGirder_LSD12.h"
#include "Dgn_SteelRptManager_KSCE.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptRatingKR_STL_ASD::CRptRatingKR_STL_ASD()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;
	m_RpasD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetRpas(m_RpasD); 
	
	T_RCDS_D RcdsD;
	if (!m_pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD)) RcdsD.Initialize();
	m_pDoc->SetCivilCodeDgn(RcdsD.nRatingCode);
	m_pDataCtrl = new CCRCDataCtrl();
	m_pRptManager = new CDgn_SteelRptManager_KSCE();
	
	m_pForcCtrl = m_pDataCtrl->Get_ForceCtrlPointer();
	if(m_pForcCtrl==NULL) {ASSERT(0); return;}

}

CRptRatingKR_STL_ASD::~CRptRatingKR_STL_ASD()
{
	if(m_pDataCtrl)
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
	}
    
	if(m_pRptManager)
	{
		delete m_pRptManager;
		m_pRptManager = NULL;
	}
}

BOOL CRptRatingKR_STL_ASD::Execute_RatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
{
	if(m_bStopExecute) return FALSE;
	Progress(3, 0);
	int nProgressPercent = 0;
	
	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);
	
	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	T_RPAS_D RpasD;
	if (!m_pDoc->m_pAttrCtrl2->GetRpas(RpasD)) RpasD.Initialize();

	int nCsgDgnCode = CSG_KSCE_LSD12;
	if (RpasD.nBridgeType==1) nCsgDgnCode = CSG_KSSC_LSD2014;
	
	BOOL bSuccess = TRUE;

//   if(!m_RpasD.bPrintParm[0] && !m_RpasD.bPrintParm[1] && !m_RpasD.bPrintParm[2] && !m_RpasD.bPrintParm[3] && !m_RpasD.bPrintParm[4])
//   {
//     if(m_bStopExecute) return FALSE;
//     nProgressPercent = 100;
//     Progress(0, nProgressPercent);
//     Progress(2, nProgressPercent);
//     
//     AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CHECK));  // _T("선택된 출력변수가 없습니다.
//     return FALSE;
//   }

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	CArray<T_RPRS_K, T_RPRS_K> aPrintElemKey;  
	m_pDoc->m_pAttrCtrl2->GetRprsKeyList(aPrintElemKey);

	int nPrintElemSize = aPrintElemKey.GetSize(); 
	if(nPrintElemSize==0)
	{
		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);
		
		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // ")출력할 요소의 설계정보가 없습니다._T("
		AfxMessageBox(strErrMessage);

		return FALSE;
	}

	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40/nPrintElemSize;

	// Input Data
	UMD_STL_KR_D    MembLcomD;

	// Output Data
	CArray<RF_RES_RF_KSCE_ASD_D,  RF_RES_RF_KSCE_ASD_D&>  aRptData;  aRptData.RemoveAll();
	RF_RES_RF_KSCE_ASD_D  RptData;

	// Composite
	CSG_MEMB_POSD_KSCE_LSD MembLcomCsdD;  // Input
	CSG_MEMB_RES_KSCE_LSD12_D MembResCsd; // Output
	CArray<CSG_MEMB_POSD_KR_D,  CSG_MEMB_POSD_KR_D&>  aRptCsdInData;  aRptCsdInData.RemoveAll();
	CArray<CSG_MEMB_POSD_KSCE_LSD,  CSG_MEMB_POSD_KSCE_LSD&>  aRptCptInData;

	MembLcomCsdD.Memb.Csgd.nDgnCode = nCsgDgnCode;

	int i=0; // 요소 
	int j=0; // I/J
	int k=0; // Positive / Negative
	int kk=0;
	int nIJ=0, nMaxMin=0;
	
	CDesignLoadRating *pCheckGirder = m_pDoc->m_pPostCtrl->GetLoadRating();

	CRptSteelGirder_LSD12 RptStlCpt;
	try
	{
		// Code Matl Code
		STL_KR_UMDC_STLOPTION InStlOpt;
		InStlOpt.strDgnCode  = CDBLib::GetStlRatCodeName(CDBLib::GetRatCode());
		InStlOpt.strMatlCode = MATLCODE_STL_KS_CIVIL;

		// Excel Output
		DGNEXCEL_KR_STL_SECT    rStrRptSectD;
		DGNEXCEL_KR_STLC_OPTION OptionData;
		D_LCOM_INFO             LcomInfo;    LcomInfo.Initialize();
		CArray<DGNEXCEL_KR_STL_SECT, DGNEXCEL_KR_STL_SECT&> arStrRptSectD;
		arStrRptSectD.RemoveAll();

		CArray<S_KR_LSD_RES_D, S_KR_LSD_RES_D&> arLSDResD;
		CArray<CSG_CHECK_RES_KSCE_LSD12,CSG_CHECK_RES_KSCE_LSD12&> aCptLSDRes;
		CArray<CSG_DRES_PRT,CSG_DRES_PRT&> aCptLSDRptRes;
		CArray<CSG_MEMB_RES_AASHTO_LRFD_D,CSG_MEMB_RES_AASHTO_LRFD_D&>  aCptKSSCRes;

		//DETATL-REPORT
		//if(m_RpasD.bPrintParm[0] || m_RpasD.bPrintParm[1] || m_RpasD.bPrintParm[2] || m_RpasD.bPrintParm[3] || m_RpasD.bPrintParm[4])
		{
			m_pDataCtrl->Get_UnbracedLength();
			if(!m_pDataCtrl->m_pStlRatingDataCtrl->Make_RatingDgnData()) return FALSE;
			
			CDgnStlRatingDataCtrl *pStlRatCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
			CDgnCsgDataCtrl *pCSGDataCtrl = m_pDataCtrl->m_pStlRatingDataCtrl->m_pCSGDataCtrl;

			m_pDataCtrl->m_pStlRatingDataCtrl->ConvertRatingGlobalData(RpasD, MembLcomCsdD.Memb.Csgd);

			CSG_MEMB_RESULT_D      MembR;
			CSG_CHECK_RESULT_CODES ChkResD;
			CSG_CHK_RES_DTR_PARAM  DtrR;

			pCSGDataCtrl->ClassNewMembResult(MembR);
			pCSGDataCtrl->ClassNewDgnResult(ChkResD);

			for(i=0; i<nPrintElemSize; ++i)
			{
				T_ELEM_K  ElemK = aPrintElemKey[i]; 
				ElemPairK EPairK(ElemK, EN_EL_BEAM);
			 
				// 1. 출력 여부 결정
				T_RROS_D RrosD; RrosD.Initialize();  // 설계 위치
				if(!m_pDoc->m_pAttrCtrl2->GetRros(ElemK, RrosD)) continue;    
				T_RPRS_D RkpoD; RkpoD.Initialize();  // 출력 위치
				if(!m_pDoc->m_pAttrCtrl2->GetRprs(ElemK, RkpoD)) continue;

				BOOL bPrintI = ((RrosD.nPosition == 0 || RrosD.nPosition == 2) && (RkpoD.nPrintChk == 0 || RkpoD.nPrintChk == 2) );
				BOOL bPrintJ = ((RrosD.nPosition == 1 || RrosD.nPosition == 2) && (RkpoD.nPrintChk == 1 || RkpoD.nPrintChk == 2) );

				BOOL bIsCompoElem = m_pDataCtrl->m_pStlRatingDataCtrl->IsCompoElem(ElemK);

				// 2. Input Data Setting(2)
				T_ELEM_D ElemD; ElemD.Initialize();
				if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;

				// material, section
				if(!m_pDataCtrl->m_pStlRatingDataCtrl->m_mRatingMatl.Lookup(ElemD.elmat, MembLcomD.Matl))
				{
					ASSERT(0); MembLcomD.Matl.Initialize();
				}
				if (!m_pDataCtrl->m_pStlRatingDataCtrl->m_mRatingSect.Lookup(ElemD.elpro, MembLcomD.Sect))
				{
					ASSERT(0); MembLcomD.Sect.Initialize();
				}

				if(bIsCompoElem)
				{
					MembResCsd.Initialize();

					pCSGDataCtrl->InitMembResult(MembR);

					if (!pCSGDataCtrl->m_mCsgMatl.Lookup(ElemD.elmat, MembLcomCsdD.Memb.Matl)) MembLcomCsdD.Memb.Matl.Initialize();
					
					CSG_SECT_ELEM CsgSect;
					if (pCSGDataCtrl->GetCsgSectElem(EPairK, CsgSect))
					{
						MembLcomCsdD.Memb.PosD[0].Sect = CsgSect.SectPos[0];
						MembLcomCsdD.Memb.PosD[1].Sect = CsgSect.SectPos[1];
						//MembLcomD.Memb.PosD[0].bSupprotNode = bSupprotNode[0];
						//MembLcomD.Memb.PosD[1].bSupprotNode = bSupprotNode[1];
					}
					
					pCSGDataCtrl->Get_CsgMembInData(EPairK, MembLcomCsdD);
					pCSGDataCtrl->MakeMemberResultData(EPairK, MembLcomCsdD, MembR);
				}
				else
				{
					//
				}

				// InData Option
				if (bIsCompoElem==TRUE)
				{
					MembLcomD.Option.iSelDgnLcom = 2; // 0=Envelope 동시발생 부재력, 1=Max/Min, 2=All 하중조합별,
					MembLcomD.Option.iChkMethod  = 0; // 0=휨검토, 1=축력검토, 2=휨&축력검토
					MembLcomD.Option.iChkAxial   = 0;  // 검토 주축 선택 (0=y, 1=z, 2=y&z)
					MembLcomD.Option.bReducingFac= FALSE;
				}
				else
				{
					MembLcomD.Option.iSelDgnLcom = 2; // 0=Envelope 동시발생 부재력, 1=Max/Min, 2=All 하중조합별,
					MembLcomD.Option.iChkMethod  = 2; // 0=휨검토, 1=축력검토, 2=휨&축력검토
					MembLcomD.Option.iChkAxial   = 2;  // 검토 주축 선택 (0=y, 1=z, 2=y&z)
				}
 
				RATING_STRP_ELEM STRP;
				RATING_GRPD_ELEM GRPD;
				RATING_STFN_ELEM STFN;
				if (!m_pDataCtrl->m_pStlRatingDataCtrl->m_mRatingStrp.Lookup(ElemK, STRP)) { STRP.Initialize(); }
				if (!m_pDataCtrl->m_pStlRatingDataCtrl->m_mRatingGrpd.Lookup(ElemK, GRPD)) { GRPD.Initialize(); }
				if (!m_pDataCtrl->m_pStlRatingDataCtrl->m_mRatingStfn.Lookup(ElemK, STFN)) { STFN.Initialize(); }
				
				// Unbraced Length
				T_KFAC_D KFacD; KFacD.Initialize();
				T_KFAC_D KAutD; KAutD.Initialize();
				CDgnDataCtrl DgnCtrl;
				BOOL bGetKfac = DgnCtrl.Get_DgnGenKfac(ElemK, KFacD, KAutD);
				if(bIsCompoElem)
				{
					pCSGDataCtrl->Get_UnbracedLengthData(EPairK, bGetKfac, KFacD, MembLcomCsdD.Memb.Ulen);
					m_pDataCtrl->m_pStlRatingDataCtrl->Get_UnbracedLengthData(EPairK, bGetKfac, KFacD, MembLcomD.Option);
				}
				else
				{
					m_pDataCtrl->m_pStlRatingDataCtrl->Get_UnbracedLengthData(EPairK, bGetKfac, KFacD, MembLcomD.Option);
				}
				
				// 3. Output Data Setting(1)
				STL_KR_RES_D ResD;
				T_RFKR_D RfkrD;
				OptionData.Initialize();
				// 4. Output Data Setting(2)       
				OptionData.strPath        = strPath;//.strPath;
				OptionData.iPrintOpt      = iPrintOpt;          // 0:출력, 1:insert
				if (bIsCompoElem && iDgnCode==STL_KSCE_LSD15)
					OptionData.iCode          = CSG_KSCE_LSD12;
				else
					OptionData.iCode          = GetCodeConvert2Engine(CDBLib::GetRatCode());//iCodeDLL;
				OptionData.bSaveImage     = TRUE;//SOption.bSaveImage;
				OptionData.bIsView        = TRUE;
				OptionData.bIsForceSheet  = FALSE;
				OptionData.iReportType    = 2;//iReportType;

				BOOL bRfkrRes = m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStressRating4TableKR05(ElemK, RfkrD);
				for(j=0; j<2; j++) // I,J
				{
					if(j==0 && (!bPrintI)) continue;
					if(j==1 && (!bPrintJ)) continue;

					MembLcomD.ElemID   = ElemK;
					MembLcomD.nPosiDgn = j==0 ? 0 : 2;
					MembLcomD.StrpSM = STRP.Strp[j];
					MembLcomD.GrpdSM = GRPD.Grpd[j];
					MembLcomD.StfnSM = STFN.Stfn[j];
				 
					if(bRfkrRes==TRUE)
					{             
						RptData.Initialize();

						if(bIsCompoElem)
						{							
							if (iDgnCode==STL_KSCE_LSD15)
							{
								MembLcomCsdD.aUlmf.SetSize(4);
								MembLcomCsdD.aChkFor.SetSize(4);

								for(k=0; k<2; k++) // 0=Positive, 1=Negative
								{
									int nRid = k*2;
									T_RFKR_BASE &Base = (j == 0) ? RfkrD.BaseI[nRid] : RfkrD.BaseJ[nRid];
									if(Base.bCheck != TRUE) continue; // 설계결과가 없으면 Continue                 
									MembLcomD.Load.LcomList.SetSize(1);
									pStlRatCtrl->ConvertRatForceCvlToEng(Base.Load, MembLcomD.Load.LcomList[0]);								
									MembLcomD.Load.Lcom2List.SetSize(1);
									pStlRatCtrl->ConvertRatForceCvlToEng(Base.Load2, MembLcomD.Load.Lcom2List[0]);

									pCSGDataCtrl->ConvertCsgUlmfCvlToEng(Base.Ulmf, MembLcomCsdD.aUlmf[0]);
									pCSGDataCtrl->ConvertCsgComfCvlToEng(Base.ChkFor, MembLcomCsdD.aChkFor[0]);

									CString str, strTmp;
									strTmp = j == 0 ? _T("I")  : _T("J") ;
									str.Format(_T("%d_%s"), ElemK, strTmp);
									MembLcomD.strName = CT2W(str);
									MembLcomD.nRatingPosi = k;

//                   CSG_CHECK_RES_KSCE_LSD12 TRes;
//                   CSG_MEMB_RES_AASHTO_LRFD_D TResKSSC;
//                   if      (nCsgDgnCode==CSG_KSCE_LSD12)   ChkResD.pLSD = &TRes;
//                   else if (nCsgDgnCode==CSG_KSSC_LSD2014) ChkResD.pAASHTO = &TResKSSC;

									if (!pCSGDataCtrl->Check_Strength(EPairK, j, MembLcomCsdD, MembR, ChkResD, DtrR)) continue;

									CSG_DRES_PRT CsgRpt;
									if (ChkResD.pLSD != NULL)
									{
										CsgRpt.MembD    = MembR.pLSD->MembR[j];
										CsgRpt.FlexD[k] = ChkResD.pLSD->StrnD;
										CsgRpt.FlexD[k].FlexD.ForceInfo_F.strLcomName = CT2W(Base.Load.strName);
										//CsgRpt.FlexD[k].FlexD.ForceInfo_F.strLcomType = ;
										RptStlCpt.SetRptData(ElemK, j, MembLcomCsdD.Memb, CsgRpt);
									}
									if (ChkResD.pAASHTO != NULL)
									{

									}

									// 내하력 평가 계산서
									if(!ConvertToDLL_RF(MembLcomD, k, Base, RptData)) return FALSE;
									CString strLcom;
									if(!ConvertToDLL_Force(RfkrD, RptData, j, strLcom)) return FALSE;


									// STL Detail 계산서 
									if(!m_pRptManager->ConvertToDLL_StlBeamCol_KCI(MembLcomD, rStrRptSectD))  return FALSE;
									if(!m_pRptManager->ConvertToDLL_StlBeamCol_Option(iDgnCode, InStlOpt, rStrRptSectD.StlOptData))    return FALSE;
									//if(!ConvertToDLL_StlBeamCol_DgnResD(ResD, rStrRptSectD.StlResData))       return FALSE;
									rStrRptSectD.StlResData.StdrBaseMaxRat.strLcomNa = strLcom;
									arStrRptSectD.Add(rStrRptSectD);


									// CSD Input data
									CSG_MEMB_POSD_KR_D MembD;
									MembD.strDgnCode = rStrRptSectD.StlOptData.strDgnCode; 
									MembD.nCalcType = MembLcomCsdD.nCalcType; 
									MembD.nSelUnit  = MembLcomCsdD.nSelUnit; 
									MembD.Memb      = MembLcomCsdD.Memb;  
									MembD.aUlmf.Copy(MembLcomCsdD.aUlmf);
									MembD.aChkFor.Copy(MembLcomCsdD.aChkFor);

									aRptCsdInData.Add(MembD);  // for CSd
									aRptCptInData.Add(MembLcomCsdD);
									// 합성 결과...
									if      (RpasD.nBridgeType==0) aCptLSDRptRes.Add(CsgRpt);  // 직선교
									else if (RpasD.nBridgeType==1) 
									{
										if (ChkResD.pAASHTO != NULL) aCptKSSCRes.Add(*ChkResD.pAASHTO);  // 곡선교
									}
									else ASSERT(0);                  
								}				
							}
							else
							{
								MembLcomCsdD.aUlmf.SetSize(4);
								MembLcomCsdD.aChkFor.SetSize(4);

								for(k=0; k<4; k++) // 0=상연-max(인장), 1=상연-min(압축), 2=하연-max(인장), 3=하연-min(압축)
								{
									T_RFKR_BASE &Base = (j == 0) ? RfkrD.BaseI[k] : RfkrD.BaseJ[k];
									if(Base.bCheck != TRUE) continue; // 설계결과가 없으면 Continue                 
									MembLcomD.Load.LcomList.SetSize(1);
									pStlRatCtrl->ConvertRatForceCvlToEng(Base.Load, MembLcomD.Load.LcomList[0]);								
									MembLcomD.Load.Lcom2List.SetSize(1);
									pStlRatCtrl->ConvertRatForceCvlToEng(Base.Load2, MembLcomD.Load.Lcom2List[0]);

									pCSGDataCtrl->ConvertCsgUlmfCvlToEng(Base.Ulmf, MembLcomCsdD.aUlmf[0]);
									pCSGDataCtrl->ConvertCsgComfCvlToEng(Base.ChkFor, MembLcomCsdD.aChkFor[0]);

									CString str, strTmp;
									strTmp = j == 0 ? _T("I")  : _T("J") ;
									str.Format(_T("%d_%s"), ElemK, strTmp);
									MembLcomD.strName = CT2W(str);
									MembLcomD.nRatingPosi = k;

									if (!pCSGDataCtrl->Check_Strength_ASD(ElemK, j, MembLcomCsdD, MembResCsd, ResD)) continue;

									// 내하력 평가 계산서
									if(!ConvertToDLL_RF(MembLcomD, k, Base, RptData)) return FALSE;
									CString strLcom;
									if(!ConvertToDLL_Force(RfkrD, RptData, j, strLcom)) return FALSE;


									// STL Detail 계산서 
									if(!m_pRptManager->ConvertToDLL_StlBeamCol_KCI(MembLcomD, rStrRptSectD))  return FALSE;
									if(!m_pRptManager->ConvertToDLL_StlBeamCol_Option(iDgnCode, InStlOpt, rStrRptSectD.StlOptData))    return FALSE;
									if(!m_pRptManager->ConvertToDLL_StlBeamCol_DgnResD(ResD, rStrRptSectD.StlResData))       return FALSE;
									rStrRptSectD.StlResData.StdrBaseMaxRat.strLcomNa = CT2W(strLcom);
									arStrRptSectD.Add(rStrRptSectD);


									// CSD Input data
									CSG_MEMB_POSD_KR_D MembD;
									MembD.strDgnCode = rStrRptSectD.StlOptData.strDgnCode; 
									MembD.nCalcType = MembLcomCsdD.nCalcType; 
									MembD.nSelUnit  = MembLcomCsdD.nSelUnit; 
									MembD.Memb      = MembLcomCsdD.Memb;  
									MembD.aUlmf.Copy(MembLcomCsdD.aUlmf);
									MembD.aChkFor.Copy(MembLcomCsdD.aChkFor);

									aRptCsdInData.Add(MembD);  // for CSd
								}				
							}							
						}
						else
						{           
							for(k=0; k<4; k++) // 0=상연-max(인장), 1=상연-min(압축), 2=하연-max(인장), 3=하연-min(압축)
							{
								if (iDgnCode==STL_KSCE_LSD15)
								{
									if (k==1 || k==3) continue; // 정,부모멘트 결과 이기 때문에 1,3은 정보가 없음
								}
								S_KR_LSD_RES_D LSDres;
								T_RFKR_BASE &Base = (j == 0) ? RfkrD.BaseI[k] : RfkrD.BaseJ[k];
								if(Base.bCheck != TRUE) continue; // 설계결과가 없으면 Continue                 
								MembLcomD.Load.LcomList.SetSize(1);
								pStlRatCtrl->ConvertRatForceCvlToEng(Base.Load, MembLcomD.Load.LcomList[0]);
								MembLcomD.Load.Lcom2List.SetSize(1);
								pStlRatCtrl->ConvertRatForceCvlToEng(Base.Load2, MembLcomD.Load.Lcom2List[0]);
								MembLcomD.Load.aCb.SetSize(1); 
								MembLcomD.Load.aCb[0] = 1.0; // 임시. Cb를 T_RFKR_BASE에 저장해서, 여기서 할당 하자.
								CString str, strTmp;
								strTmp = j == 0 ? _T("I")  : _T("J") ;
								str.Format(_T("%d_%s"), ElemK, strTmp);
								MembLcomD.strName = CT2W(str);
								MembLcomD.nRatingPosi = k;
								if (iDgnCode==STL_KSCE_LSD15)
								{
									CString strPN = k==0 ? _T("_P") : _T("_N");
									CStringW strWPN = CT2W(strPN);
									MembLcomD.strName = MembLcomD.strName + strWPN;
									m_pDataCtrl->m_pStlRatingDataCtrl->Check_Rating(MembLcomD, InStlOpt, LSDres);
									arLSDResD.Add(LSDres);
								}
								else
									m_pDataCtrl->m_pStlRatingDataCtrl->Check_Rating(MembLcomD, InStlOpt, ResD);
								
								// STL Detail 계산서 
								if(!m_pRptManager->ConvertToDLL_StlBeamCol_KCI(MembLcomD, rStrRptSectD))  return FALSE;
								if(!m_pRptManager->ConvertToDLL_StlBeamCol_Option(iDgnCode, InStlOpt, rStrRptSectD.StlOptData))    return FALSE;
								if(!m_pRptManager->ConvertToDLL_StlBeamCol_DgnResD(ResD, rStrRptSectD.StlResData))       return FALSE;
								arStrRptSectD.Add(rStrRptSectD);
								
								// 내하력 평가 계산서
								if(!ConvertToDLL_RF(MembLcomD, k, Base, RptData)) return FALSE;
								CString strLcom;
								if(!ConvertToDLL_Force(RfkrD, RptData, j, strLcom)) return FALSE;
							}							
						}
						
						aRptData.Add(RptData);
					
					}
				}
				if(m_bStopExecute)	return FALSE;
				nProgressPercent += nProgressIncrementPercent;
				Progress(0, nProgressPercent);
				Progress(2, nProgressPercent);
			}

			if (MembR.pAASHTO != NULL) delete MembR.pAASHTO;
			if (MembR.pLSD    != NULL) delete MembR.pLSD;
			if (MembR.pCSA    != NULL) delete MembR.pCSA;

			pCSGDataCtrl->ClassDelDgnResult(ChkResD);
		}

		//RESULT-TABLE
		nProgressIncrementPercent = nPrintElemSize == 0 ? 30 : 30/nPrintElemSize;

	// Check Result Data Exist
	if(aRptData.GetSize() < 1)
	{
		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // ")출력할 요소의 설계정보가 없습니다._T("
		AfxMessageBox(strErrMessage);

		return FALSE;
	}

		// 계산서 출력
		
		CDgn_RatingRptManager myReport;

		CString strDataPath = m_pDoc->GetPathName();
	  CString strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");

		if (aRptCptInData.GetSize()>0)
		{
			myReport.Print_RF_CptDetailReport(strBasePath, strDataPath, strPath, OptionData, arStrRptSectD, aRptCptInData, aCptLSDRptRes, aCptKSSCRes);
		}
        else
        {
            myReport.Print_RFReport(strBasePath, strDataPath, strPath, LcomInfo, OptionData, arStrRptSectD, aRptData, aRptCsdInData, arLSDResD);
        }
	}
	catch(...)
	{
		bSuccess = FALSE;	
	}
	return bSuccess;
}

// ADD for KSCE-RAIL-ASD04

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_STL_TRANSTFN_D(const T_TRAN_STFN_D& UstlStrn, DGNEXCEL_STL_KR_TRAN_STFN_D& ExlStfnD)
{
	ExlStfnD.bIsStfn  = UstlStrn.bIsStfn;
	ExlStfnD.iStfnNum = UstlStrn.iStfnNum;
	ExlStfnD.dFys     = UstlStrn.dFys;    // fy
	ExlStfnD.dd0      = UstlStrn.dd0;     // 간격
	ExlStfnD.db       = UstlStrn.db;      // 폭
	ExlStfnD.dtp      = UstlStrn.dtp;     // 두께
	
	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_STL_TRANSTFN(const T_USTL_TRAN_STFN& UstlOptD, DGNEXCEL_STL_KR_TRAN_STFN& ExlOptD)
{
	int i = 0;
	for(i = 0 ; i < 2 ; ++i)  
		ConvertToDLL_STL_TRANSTFN_D(UstlOptD.TranStfnD[i], ExlOptD.TranStfnD[i]);

	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_STL_POINT_SSM_D(const T_UMD_POINT_SSM& PointD, DGNEXCEL_STL_KR_POINT_SSM& ExlD)
{
	ExlD.dPointY = PointD.dPointY;
	ExlD.dPointZ = PointD.dPointZ;

	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_STL_STRP_SSM_D(const T_UMD_STRP_SSM_D& StrpD, DGNEXCEL_STL_KR_STRP_SSM_D& ExlD)
{
	int nCountPoint = StrpD.aPoint.GetSize();
	ExlD.aPoint.SetSize(nCountPoint);

	for(int i=0; i<nCountPoint; i++)
	{
		ConvertToDLL_STL_POINT_SSM_D(StrpD.aPoint[i], ExlD.aPoint[i]);
	}

	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_STL_GRPD_SUM_SSM(const T_UMD_GRPD_SUB_SSM& GrpdSubD, DGNEXCEL_STL_KR_GRPD_SUB_SSM& ExlD)
{
	ExlD.nGroupID = GrpdSubD.nGroupID;
	ExlD.strGroupName = GrpdSubD.strGroupName;
	ExlD.nGroupType = GrpdSubD.nGroupType;

	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_STL_PART_SSM(const T_UMD_PART_SSM& PartD, DGNEXCEL_STL_KR_PART_SSM& ExlD)
{
	ExlD.nType = PartD.nType;
	ExlD.nGroupID = PartD.nGroupID;
	ExlD.strPartName = PartD.strPartName;
	ExlD.nPartShape = PartD.nPartShape;
	ExlD.bStiffener = PartD.bStiffener;

	ExlD.aLine.Copy(PartD.aLine);

	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_STL_GRPD_SSM_D(const T_UMD_GRPD_SSM_D& GrpdD, DGNEXCEL_STL_KR_GRPD_SSM_D& ExlD)
{
	int nCountGrpd = GrpdD.aGrpd.GetSize();
	ExlD.aGrpd.SetSize(nCountGrpd);

	for(int i=0; i<nCountGrpd; i++)
	{
		ConvertToDLL_STL_GRPD_SUM_SSM(GrpdD.aGrpd[i], ExlD.aGrpd[i]);
	}

	int nCountPart = GrpdD.aPart.GetSize();
	ExlD.aPart.SetSize(nCountPart);

	for(int i=0; i<nCountPart; i++)
	{
		ConvertToDLL_STL_PART_SSM(GrpdD.aPart[i], ExlD.aPart[i]);
	}

	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_STL_STFN_SUB_TYPE_SSM(const T_UMD_STFN_SUB_TYPE_SSM& StfnSubTypeD, DGNEXCEL_STL_KR_STFN_SUB_TYPE_SSM& ExlD)
{
	ExlD.nStiffenerType = StfnSubTypeD.nStiffenerType;

	for(int i=0; i<20; i++)
	{
		ExlD.dSize[i] = StfnSubTypeD.dSize[i];
	}

	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_STL_STFN_SUB_SSM(const T_UMD_STFN_SUB_SSM& StfnSubD, DGNEXCEL_STL_KR_STFN_SUB_SSM& ExlD)
{
	ExlD.nType = StfnSubD.nType;
	ExlD.nStandRef = StfnSubD.nStandRef;
	ExlD.nPosonLine = StfnSubD.nPosonLine;
	ExlD.dStandRefL = StfnSubD.dStandRefL;
	ExlD.nNum = StfnSubD.nNum;
	ExlD.dCTC = StfnSubD.dCTC;

	ConvertToDLL_STL_STFN_SUB_TYPE_SSM(StfnSubD.Stiffener, ExlD.Stiffener);

	ExlD.aLine.Copy(StfnSubD.aLine);

	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_STL_STFN_SSM_D(const T_UMD_STFN_SSM_D& StfnD, DGNEXCEL_STL_KR_STFN_SSM_D& ExlD)
{
	int nCountStfn = StfnD.aStiffener.GetSize();
	ExlD.aStiffener.SetSize(nCountStfn);

	for(int i=0; i<nCountStfn; i++)
	{
		ConvertToDLL_STL_STFN_SUB_SSM(StfnD.aStiffener[i], ExlD.aStiffener[i]);
	}

	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_STL_FATI_SUB_SSM(const T_UMD_FATI_SUB_SSM& FatiSubD, DGNEXCEL_STL_KR_FATI_SUB_SSM& ExlD)
{
	ExlD.nCategory = FatiSubD.nCategory;
	ExlD.dADTT = FatiSubD.dADTT;
	ExlD.dInteration = FatiSubD.dInteration;
	ConvertToDLL_STL_POINT_SSM_D(FatiSubD.Point, ExlD.Point);

	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_STL_FATI_SSM_D(const T_UMD_FATI_SSM_D& FatiD, DGNEXCEL_STL_KR_FATI_SSM_D& ExlD)
{
	int nCountFati = FatiD.aFatigue.GetSize();
	ExlD.aFatigue.SetSize(nCountFati);

	for(int i=0; i<nCountFati; i++)
	{
		ConvertToDLL_STL_FATI_SUB_SSM(FatiD.aFatigue[i], ExlD.aFatigue[i]);
	}

	return TRUE;
}

//PMS:RATING BSC-20140321 내하력 평가
BOOL CRptRatingKR_STL_ASD::ConvertToDLL_RF(UMD_STL_KR_D &Memb, const int &nPosi, const T_RFKR_BASE& BaseD, RF_RES_RF_KSCE_ASD_D &RptD)
{
	RptD.nElem  = Memb.ElemID;
	RptD.nIJ    = Memb.nPosiDgn;
	RptD.nEstimatedMethod = m_RpasD.nSurveyMeth;
	//// 0: 상연:인장, 1: 압축, 2: 하연:인장, 3: 압축
	RptD.Stress[nPosi].strLcom       = BaseD.Load.strName;
	RptD.Stress[nPosi].dfa           = BaseD.dfa;
	RptD.Stress[nPosi].dfd           = BaseD.dfd;
	RptD.Stress[nPosi].dfl           = BaseD.dfl;
	RptD.Stress[nPosi].dImpactFactor = BaseD.d1iFactor;
	RptD.Stress[nPosi].dRF           = BaseD.dRF;

	RptD.Delta[nPosi].strLcom        = BaseD.Load.strName;
	RptD.Delta[nPosi].dCalcDelta     = BaseD.dCalcDisp;
	RptD.Delta[nPosi].dEstiDelta     = BaseD.dEstiDisp;
	RptD.Delta[nPosi].dRatioDelta    = BaseD.dRatioDisp;
	RptD.Delta[nPosi].dCalci         = BaseD.dCalcImpFactor;
	RptD.Delta[nPosi].dEstii         = BaseD.dEstiImpFactor;
	RptD.Delta[nPosi].dRatioi        = BaseD.dRatioImpFactor;
	RptD.Delta[nPosi].dKs            = BaseD.dKs;

	RptD.Ewips[nPosi].strLcom        = BaseD.Load.strName;
	RptD.Ewips[nPosi].dCalcEwips     = BaseD.dCalcEwip;
	RptD.Ewips[nPosi].dEstiEwips     = BaseD.dEstiEwip;
	RptD.Ewips[nPosi].dRatioEwips    = BaseD.dRatioEwip;
	RptD.Ewips[nPosi].dCalci         = BaseD.dCalcImpFactor;
	RptD.Ewips[nPosi].dEstii         = BaseD.dEstiImpFactor;
	RptD.Ewips[nPosi].dRatioi        = BaseD.dRatioImpFactor;
	RptD.Ewips[nPosi].dKs            = BaseD.dKs;

	CStringW strWName(BaseD.Load.strName); 
	RptD.LoadP[nPosi].strLcom        = strWName;

	CString strDesignPr; strDesignPr.Format(_T("%s-%.3f"), m_RpasD.strDgnLiveLoad, m_RpasD.dDgnLiveLoad);
	CStringW strWDesignPr(strDesignPr); 
	RptD.LoadP[nPosi].strDesignPr = strWDesignPr;
	if (fabs(BaseD.dP0 - 0.0) > 10e-5)
	{
		CString strP0; strP0.Format(_T("%s-%.3f"), m_RpasD.strDgnLiveLoad, BaseD.dP0);
		CStringW strWP0(strP0);
		RptD.LoadP[nPosi].strP0 = strWP0;
	}

	RptD.LoadP[nPosi].dKs            = BaseD.dKs;
	RptD.LoadP[nPosi].dP             = BaseD.dP;
	RptD.LoadP[nPosi].nOK            = BaseD.nOK;

	RptD.Level[nPosi].strLcom        = strWName; //BaseD.Load.strName;
	RptD.Level[nPosi].strLevel       = GetBridgeLevel(BaseD.nRankType);
	RptD.Level[nPosi].dfa            = BaseD.dfa;
	RptD.Level[nPosi].dfdl           = BaseD.dfdl;
	RptD.Level[nPosi].dRatio         = BaseD.dRatiodfa;
	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_Force(T_RFKR_D &Rfkr, RF_RES_RF_KSCE_ASD_D &rRptD, int nIJ, CString& strLcom)
{
	rRptD.arForce.RemoveAll();
	if (nIJ == 0) 
	{
		int nSize = Rfkr.arForceI.GetSize();
		rRptD.arForce.SetSize(nSize);
		for (int i = 0; i<nSize; ++i)
		{
			ConvertToDLL_RptForce(Rfkr.arForceI[i].ForceDL, rRptD.arForce[i].ForceDL);
			ConvertToDLL_RptForce(Rfkr.arForceI[i].ForceLL, rRptD.arForce[i].ForceLL);

			if(i==0) strLcom = rRptD.arForce[i].ForceLL.strName;
		}
	}
	else if (nIJ == 1) 
	{
		int nSize = Rfkr.arForceJ.GetSize();
		rRptD.arForce.SetSize(nSize);
		for (int i = 0; i<nSize; ++i)
		{
			ConvertToDLL_RptForce(Rfkr.arForceJ[i].ForceDL, rRptD.arForce[i].ForceDL);
			ConvertToDLL_RptForce(Rfkr.arForceJ[i].ForceLL, rRptD.arForce[i].ForceLL);

			if(i==0) strLcom = rRptD.arForce[i].ForceLL.strName;
		}
	}
	else
		ASSERT(0);

	return TRUE;
}

BOOL CRptRatingKR_STL_ASD::ConvertToDLL_RptForce(T_RATI_UNITLCOM &Force,  RF_RES_KS_FORCE_KSCE_ASD_D&rForce)
{
	if (&Force == NULL) return FALSE;
	rForce.bUse = Force.bUse;
	rForce.nType = Force.nType;
	rForce.nOrgLkey = Force.nOrgLkey;
	rForce.strName = Force.strName;
	rForce.strDesc = Force.strDesc;
	for (int i = 0; i<6; ++i)
	{
		rForce.dForce[i] = Force.dForce[i];
	}

	return TRUE;
}

CStringW CRptRatingKR_STL_ASD::GetBridgeLevel(const int  &nRank)
{
	CString strTemp;
	if (nRank == ENUM_LEVEL_A)
	{
		strTemp = _T("A");
	}
	else if (nRank == ENUM_LEVEL_B) 
	{
		strTemp = _T("B");
	}
	else if (nRank == ENUM_LEVEL_C) 
	{
		strTemp = _T("C");
	}
	else if (nRank == ENUM_LEVEL_D) 
	{
		strTemp = _T("D");
	}
	else if (nRank == ENUM_LEVEL_E) 
	{
		strTemp = _T("E");
	}
	else
		ASSERT(0);
	
	CStringW strWTemp(strTemp);
	return strWTemp;
}

int CRptRatingKR_STL_ASD::GetCodeConvert2Engine(const UINT &nCode)
{
	int nEngineCode = -1;
	switch (nCode)
	{
		case STL_KSCE_ASD05:
			nEngineCode = DGN_KSCE_ASD05;
		break;
		case STL_KSCE_ASD10:
			nEngineCode = DGN_KSCE_ASD10;
		break;
		case STL_KSCE_RAIL_ASD04:
			nEngineCode = DGN_KSCE_RAIL_ASD04;
		break;
		case STL_KSCE_RAIL_ASD11:
			nEngineCode = DGN_KSCE_RAIL_ASD11;
		break;
		case STL_KSCE_LSD15:
			nEngineCode = DGN_KR_BRG_LSD15;
		break;
		default:
			ASSERT(0);
			break;
	}
	return nEngineCode;
}
