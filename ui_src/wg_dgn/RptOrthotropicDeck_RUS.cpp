
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "RptOrthotropicDeck_RUS.h"
// #include "RptPlateGirder_EC4.h"
#include "DgnDataCtrl.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\AnalysisResult.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\StlOrthDeckDesign.h"

#include "..\wg_db\DBLib.h"
#include "Dgn_OrthotropicDeckRptManager.h"

#define FLANGE_LEFT   0
#define FLANGE_MIDDLE 1
#define FLANGE_RIGHT  2

#define SN_WEB_NoM   0
#define SN_WEB_VON   1
#define SN_WEB_RIB   2

#define SN_DECK      0
#define SN_RIB       1
#define SN_FLOORBEAM 2

#define SB_DECK_G      0
#define SB_DECK_L      1
#define SB_RIB         2
#define SB_FLOORBEAM_G 3
#define SB_FLOORBEAM_L 4

#define SB_WEB_DECK  0
#define SB_WEB_RIB   1


//#include "..\wg_db\PlateGirderDesign.h"
// 
//#include "Dgn_CSGRptManager_AASHTO12.h"
// #include "DgnCsgDataCtrl.h"
// #include "DgnPscCommon.h"
// #include "DgnDataCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptOrthotropicDeck_RUS::CRptOrthotropicDeck_RUS()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;
	
	//m_bTestMode = CDBLib::CheckTheRegistryForDesignTest();
	
//   m_SoddD.Initialize();
//   m_pDoc->m_pAttrCtrl2->GetDgnSodd(m_SoddD); 
// 	
//   int nDgnCode = m_SoddD.nDesignCode;
// 	
//   m_pDoc->SetCivilCodeDgn(nDgnCode);
	m_pDataCtrl = new CCRCDataCtrl();
	m_pDataCtrl->Get_SodDataCtrlPointer()->SetDataCtrlPointer(m_pDataCtrl);

}

CRptOrthotropicDeck_RUS::~CRptOrthotropicDeck_RUS()
{
	if(m_pDataCtrl)
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
	}
}



BOOL CRptOrthotropicDeck_RUS::Execute_OrthotropicDeckRpt_RUS(int iDgnCode, CString sFileName, int iPrintOpt, BOOL bSaveImage)
{
	// Input Data
	CArray<STL_MEMB_POSD_SNiP_OD, STL_MEMB_POSD_SNiP_OD&>   aInData;   aInData.RemoveAll();
	STL_MEMB_POSD_SNiP_OD    MembLcomD;

	// Output Data
	CArray<STL_MEMB_RES_SNiP_OD,  STL_MEMB_RES_SNiP_OD&>  aRptData;  aRptData.RemoveAll();
	STL_MEMB_RES_SNiP_OD  RptData;
 
	CDgnSodDataCtrl *pSodDataCtrl = m_pDataCtrl->m_pSodDataCtrl;

	// Input data
	pSodDataCtrl->InitialData(iDgnCode);
	pSodDataCtrl->Make_SodDgnData();
	
	// design element
	CArray<T_ELEM_K,T_ELEM_K> arElemK; arElemK.RemoveAll(); 
	pSodDataCtrl->Get_rElemK(arElemK);
	if(arElemK.GetSize()==0)
	{
		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("출력할 요소의 설계정보가 없습니다.")
		AfxMessageBox(strErrMessage);
		
		return FALSE;
	}

	// output element
	CArray<T_SDDO_K, T_SDDO_K> aPrintElemKey; aPrintElemKey.RemoveAll();
	m_pDoc->m_pAttrCtrl2->GetSddoKeyList(aPrintElemKey);
	int nPrintElemSize =	aPrintElemKey.GetSize();
	if(nPrintElemSize==0)
	{
		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("출력할 요소의 설계정보가 없습니다.")
		AfxMessageBox(strErrMessage);
		
		return FALSE;
	}

	// Table 정보
	CDgn_OrthotropicDeckManager* pSodManager = pSodDataCtrl->m_pSodManager;
	pSodManager->GetTableValueSNiP(MembLcomD.TBval);

	// design parameter
	pSodDataCtrl->ConvertSODGlobalData(MembLcomD.Memb.GDgn);
	if(!MembLcomD.Memb.GDgn.bDgnParam[0] && !MembLcomD.Memb.GDgn.bDgnParam[1] && !MembLcomD.Memb.GDgn.bDgnParam[2])
	{
		AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CHECK));  // _T("선택된 출력변수가 없습니다.
		return FALSE;
	}


	int i=0; // 요소 
	int j=0; // I/J
	int k=0; // Positive / Negative
	int kk=0;
	int nIJ=0, nMaxMin=0;

	for(i=0; i<nPrintElemSize; ++i)
	{
		T_ELEM_K ElemK = aPrintElemKey.GetAt(i); // arElemK[i];

	// Design I/J 
	BOOL bChkIJ[2]={FALSE};
	T_SDDP_D SddpD; SddpD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetSddp(ElemK, SddpD)) continue;

	// Output I/J
	T_SDDO_D SddoD;
	if(!m_pDoc->m_pAttrCtrl2->GetSddo(ElemK, SddoD)) continue;

	// 검토 여부
	bChkIJ[0] = (SddpD.aSddpSub[0].GetSize()>0 && (SddoD.iDgnPosi==0 || SddoD.iDgnPosi==2));
	bChkIJ[1] = (SddpD.aSddpSub[1].GetSize()>0 && (SddoD.iDgnPosi==1 || SddoD.iDgnPosi==2));  

		T_ELEM_D ElemD; ElemD.Initialize();
		if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD)) continue;
		
		T_SECT_D SectD;
		m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro,SectD);
		int nStype = SectD.nStype;
		if (SectD.nStype==D_SECT_TYPE_TAPERED) nStype = SectD.SectBefore.nStype;


		STL_SECT_ELEMD_SNiP_OD SodSect;
		if (pSodDataCtrl->m_mSodSect.Lookup(ElemD.elpro, SodSect))
		{
		MembLcomD.Memb.PosD[0].SectInfo = SodSect.SectD[0];
		MembLcomD.Memb.PosD[1].SectInfo = SodSect.SectD[1];
		//MembLcomD.Memb.PosD[0].bSupprotNode = bSupprotNode[0];
		//MembLcomD.Memb.PosD[1].bSupprotNode = bSupprotNode[1];
		}
		else ASSERT(0);

		// Get Design Length Info.
		T_EDLK_D DgnLengD;
		pSodDataCtrl->m_mDgnLengD.Lookup(ElemK, DgnLengD);
		
		for(j=0; j<2; j++) // I,J
		{
			if(bChkIJ[j]==FALSE) continue;

			//요소,절점 점보
			MembLcomD.Memb.ChIf.nElem   = ElemK;
			MembLcomD.Memb.ChIf.nChkPos = j;

			// 절점별 데이타
			pSodDataCtrl->Get_SodPosData(ElemK, ElemD.elpro, nStype, j, MembLcomD.Memb);

			// 부재력 세팅
			MembLcomD.aUlmf.RemoveAll();    MembLcomD.aUlmf.SetSize(1);
			MembLcomD.aChkFor.RemoveAll();  MembLcomD.aChkFor.SetSize(1);

			STL_MEMB_POSD_SNiP_OD* pMembLcomDOrg = new STL_MEMB_POSD_SNiP_OD(); *pMembLcomDOrg = MembLcomD;
			STL_MEMB_POSD_SNiP_OD* pMembLcomDFat = new STL_MEMB_POSD_SNiP_OD(); *pMembLcomDFat = MembLcomD;

			// 기본적인 값들..
			STL_RES_BASE_SNiP_OD_SNiP MembResD[2];
			pSodDataCtrl->ConvertSodDgnLengthData(DgnLengD, MembResD[j]);     // Get Design Length Info.
			MembLcomD.Memb.ChIf.nChkPos = j;
			pSodManager->Check_MemberResSNiP(MembLcomD, MembResD[j]);

			RptData.OD_Res_SNiP_Calc_BaseD = MembResD[j];

			// 설계를 위해 다시 세팅!!
			MembLcomD.aUlmf.RemoveAll();
			MembLcomD.aChkFor.RemoveAll();

			// Kapa 계산
			aKAPAF_PonitInfo KAPAF_PonitInfo; KAPAF_PonitInfo.RemoveAll();
			BOOL bAllKappaF = TRUE;
			Set_SodForceData4Kapa(ElemK, nIJ, pSodManager, MembResD[j], *pMembLcomDOrg, KAPAF_PonitInfo, bAllKappaF);

			RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.KAPAF_PonitInfo.Copy(KAPAF_PonitInfo);
			RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF;
			RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.KAPAF_PonitInfo.Copy(KAPAF_PonitInfo);
			RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;

			// Strength
			if(MembLcomD.Memb.GDgn.bDgnParam[0])
			{
				// Detail data
				Set_SodForceDataSTN_Result_FLG(ElemK, j, pSodManager, MembResD[j], *pMembLcomDOrg, MembLcomD, RptData, bAllKappaF);  
				Set_SodForceDataSTN_Result_WLG(ElemK, j, pSodManager, MembResD[j], *pMembLcomDOrg, MembLcomD, RptData, bAllKappaF);  
			}

			// Stability
			if(MembLcomD.Memb.GDgn.bDgnParam[1])
			{	
				// Detail data
				Set_SodForceDataSTB_Result_FLG(ElemK, j, pSodManager, MembResD[j], *pMembLcomDOrg, MembLcomD, RptData, bAllKappaF);  
				Set_SodForceDataSTB_Result_WLG(ElemK, j, pSodManager, MembResD[j], *pMembLcomDOrg, MembLcomD, RptData, bAllKappaF);  
			}

			// Fatigue
			if(MembLcomD.Memb.GDgn.bDgnParam[2])
			{
				CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
				T_SDFG_CR FAT_CR;   // 가장 불리한 Fatigue Lcom
				if(!pCheckSod->GetSodFatiPosCrResult(ElemK, FAT_CR)) FAT_CR.Initialize();				
				T_SDFG_CASE SDFG;   // 파트별 검토 결과
				T_SDCR_BASE SdfgB  = FAT_CR.FatiCr[nIJ];   
				if(!pCheckSod->GetSodChkFatiResult(ElemK, SdfgB.nLcomK, SDFG)) SDFG.Initialize();

				Set_SodForceDataFatigue(ElemK, j,  *pMembLcomDFat);
				STL_MEMB_RES_SNiP_FAT FatiResD;
				pSodManager->Check_StlOrthDeckFatigueSNiP(*pMembLcomDFat, MembResD[j], FatiResD);

				T_SDFG_BASE	rData  =  SDFG.FatiB[nIJ]; 
				if(rData.bChk)
				{
					RptData.OD_Res_SNiP_Fat_D = FatiResD;
					//RptData.OD_Res_SNiP_Fat_D.bChkFat = rData.bChk;
					MembLcomD.aChkFor.Add(pMembLcomDFat->aChkFor[0]);
				}

			}

			if(pMembLcomDOrg != nullptr){delete pMembLcomDOrg;}
			if(pMembLcomDFat != nullptr){delete pMembLcomDFat;}
			// ******
			aInData.Add(MembLcomD);
			aRptData.Add(RptData);
		}

	}


	// 계산서 출력
	CRptOrthotropicDeck ODDgn;  
	CDgn_OrthotropicDeckRptManager myODDReport;
	
	CString strProgramDir = m_pDoc->GetProgramPath();

#if defined(_CIVIL_RUS)
		CString strBaseFileDir = _T("");
		if     (iDgnCode==SNiP_20503_84_SOD     || iDgnCode==SP_35_13330_11_SOD)    strBaseFileDir = m_pDoc->GetProgramPath() + _T("\\Excel Base File\\RUS\\");
		//else if(iDgnCode==SNiP_20503_84_SOD || iDgnCode==SP_35_13330_11_SOD)      strBaseFileDir = m_pDoc->GetProgramPath() + ")\\Excel Base File\\RUS(MKS)\\_T(";
		else ASSERT(0);
#else 
		CString strBaseFileDir = _T("");
		if     (iDgnCode==SNiP_20503_84_SOD     || iDgnCode==SP_35_13330_11_SOD)    strBaseFileDir = m_pDoc->GetProgramPath() + _T("\\Excel Base File\\US\\");
		//else if(iDgnCode==SNiP_20503_84_SOD || iDgnCode==SP_35_13330_11_SOD)      strBaseFileDir = m_pDoc->GetProgramPath() + ")\\Excel Base File\\US(MKS)\\_T(";
		else ASSERT(0);
#endif

	sFileName = ODDgn.GetSaveFileNameEx();
	myODDReport.Print_DetailReport(iDgnCode,  strProgramDir, strBaseFileDir, sFileName, aInData, aRptData);


	return TRUE;
}


void CRptOrthotropicDeck_RUS::ConvertSodForce2Engine(T_ELEM_K ElemKey, int nIJ,  int nChkPos, T_SDCR_BASE SDCR_BASE, STL_MEMB_POSD_SNiP_OD &MembLcomD)
{
	// nChkPos==0 :aChkFor 크기를 1로 한다.
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
	
	T_SODD_DGN_FORCE rData;
	pCheckSod->GetSodLcomForce(ElemKey, SDCR_BASE.nDgnLcomK, rData);
	
	int nSize = nChkPos + 1;
	if(MembLcomD.aUlmf.GetSize() < nSize){MembLcomD.aUlmf.SetSize(nSize);}
	if(MembLcomD.aChkFor.GetSize() < nSize){MembLcomD.aChkFor.SetSize(nSize);}

	ConvertSodMemberForce2Engine(rData.Ulmf, MembLcomD.aUlmf[nChkPos]);
	ConvertSodGlobalForce2Engine(ElemKey, rData.Elem[nIJ], SDCR_BASE, MembLcomD.aChkFor[nChkPos]);
	ConvertSodGlobalForce2Engine_Kapa(ElemKey, nIJ, SDCR_BASE, MembLcomD.aChkFor[nChkPos]);
	Get_LocalForceData(ElemKey, SDCR_BASE.nLcomK, nIJ, MembLcomD.aChkFor[nChkPos]); 
}


void CRptOrthotropicDeck_RUS::ConvertSodMemberForce2Engine(T_SODD_ULMF &MFor,  STL_ULMF_D &rData)
{
	for (int i=0; i<6; ++i)
	{
		rData.ForD[0].dForce[i] = MFor.For[0].dForce[i];
		rData.ForD[1].dForce[i] = MFor.For[2].dForce[i];
		rData.ForD[2].dForce[i] = MFor.For[4].dForce[i];
	}
}

void CRptOrthotropicDeck_RUS::ConvertSodGlobalForce2Engine(T_ELEM_K ElemK, T_SODD_FORCE &Elem, T_SDCR_BASE SDCR_BASE, STL_LCOM_FORCE_OD &ChkForOD)
{
	T_LCOM_D LcomD; LcomD.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, SDCR_BASE.nLcomK, LcomD))
	{
		ChkForOD.strLcomName = LcomD.LoadCombName;
	}

	ChkForOD.strLcomType = Get_LcomType(SDCR_BASE.nConcurrent);

	STL_FORCE_D FORCE; FORCE.Initialize();
	memcpy(FORCE.dForce, Elem.dForce, sizeof(FORCE.dForce)); 
	FORCE.strLcomName	= LcomD.LoadCombName;
	FORCE.nMax			= SDCR_BASE.nConcurrent;

	ChkForOD.GlobalForceD.SetSize(1);
	ChkForOD.GlobalForceD_DL.SetSize(1);

	ChkForOD.GlobalForceD.SetAt(0, FORCE);
}

CString CRptOrthotropicDeck_RUS::Get_LcomType(int nConcurrent)
{
	CString strLcomType= _T("-");
	if     (nConcurrent==0)		strLcomType = _T("-");
	else if(nConcurrent==2)		strLcomType = _T("Max/Min");
	else if(nConcurrent==12)	strLcomType = _T("-"); //?
	
	return strLcomType;
}

void CRptOrthotropicDeck_RUS::ConvertSodGlobalForce2Engine_Kapa(T_ELEM_K ElemKey, int nIJ, T_SDCR_BASE SDCR_BASE, STL_LCOM_FORCE_OD &ChkForOD)
{

	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();

	int nLcomSize = SDCR_BASE.nLcomSize;  // concurrent 크기

	STLCHKFOR_ARRAY aChkForceI;		aChkForceI.RemoveAll();		aChkForceI.SetSize(nLcomSize);
	STLCHKFOR_ARRAY aChkForceJ;		aChkForceJ.RemoveAll();		aChkForceJ.SetSize(nLcomSize);
	STLCHKFOR_ARRAY aChkDLForceI;	aChkDLForceI.RemoveAll();	aChkDLForceI.SetSize(nLcomSize);
	STLCHKFOR_ARRAY aChkDLForceJ;	aChkDLForceJ.RemoveAll();	aChkDLForceJ.SetSize(nLcomSize);

	T_SODD_DGN_FORCE SodForce;
	for(int i=0; i < nLcomSize; ++i)
	{
		SodForce.Initialize();
		int nDgnLcomId = SDCR_BASE.nDgnLcomKFirst + i;
		pCheckSod->GetSodLcomForce(ElemKey, nDgnLcomId, SodForce);

		ConvertSodCheckForce(SodForce.Elem[0], aChkForceI[i]);
		ConvertSodCheckForce(SodForce.ElemDL[0], aChkDLForceI[i]);
		ConvertSodCheckForce(SodForce.Elem[1], aChkForceJ[i]);
		ConvertSodCheckForce(SodForce.ElemDL[1], aChkDLForceJ[i]);
	}

	STLCHKFOR_ARRAY aChkForce; aChkForce.RemoveAll();
	STLCHKFOR_ARRAY aChkDLForce; aChkDLForce.RemoveAll();
	if(nIJ==0)
	{
		aChkForce.Copy(aChkForceI);
		aChkDLForce.Copy(aChkDLForceI);
	}
	else
	{
		aChkForce.Copy(aChkForceJ);
		aChkDLForce.Copy(aChkDLForceJ);
	}
	ConvertSodCalcChkForceRpt(aChkForce, aChkDLForce, ChkForOD);
}


void CRptOrthotropicDeck_RUS::ConvertSodCalcChkForceRpt(STLCHKFOR_ARRAY& aChkForce, STLCHKFOR_ARRAY& aChkDLForce, STL_LCOM_FORCE_OD& ChkForOD)
{

	for(int i=0; i<aChkForce.GetSize(); ++i)
	{
		ChkForOD.GlobalForceD.Add(aChkForce[i]); 
	}
	
	if (aChkDLForce.GetSize()>0)
	{
	  ChkForOD.GlobalForceD_DL.SetAt(0,aChkDLForce[0]);
	  for(int i=0; i<aChkForce.GetSize(); ++i)  // GlobalForceD_DL은 GlobalForceD 개수에 맞춰서 저장한다.
	  {
		  ChkForOD.GlobalForceD_DL.Add(aChkDLForce[0]); 
	  }
	}

}

void CRptOrthotropicDeck_RUS::Get_FlexBuckMembForceD(T_ELEM_K ElemKey, int nIJ, int nDgnLcomK, STL_MEMB_POSD_SNiP_OD &MembLcomD)
{
	T_FLBK_D FlbkD;
	if(m_pDoc->m_pAttrCtrl2->GetFlbk(ElemKey, FlbkD))	
	{
		STL_POSD_DGND_SNiP_FB_MEMB_TYPE *pInDataFB = &MembLcomD.Memb.PosD[nIJ].PosDDgnInfo.FlexBuckMembType;
		pInDataFB->bExistD		= true;
		pInDataFB->nMembType	= FlbkD.nMembType;

		T_MEMB_K MembK;
		if(m_pDoc->m_pAttrCtrl->GetMembAssigned(ElemKey, MembK))
		{
			T_MEMB_D MembD;
			if(m_pDoc->m_pAttrCtrl->GetMemb(MembK, MembD))
			{
				int nElemSubSize = MembD.aElemList.GetSize();
				pInDataFB->aMembCompD.SetSize(nElemSubSize);

				for(int nElemSub = 0; nElemSub < nElemSubSize; ++nElemSub)
				{
					T_ELEM_K ElemSubK = MembD.aElemList[nElemSub];
					pInDataFB->aMembCompD[nElemSub].dLength = m_pDoc->calcLAVElem(ElemSubK);
					Get_FlexBuckMembForceD_Sub(ElemSubK, nDgnLcomK, pInDataFB->aMembCompD[nElemSub]);
				}
			}
		}
		else
		{
			pInDataFB->aMembCompD.SetSize(1);
			pInDataFB->aMembCompD[0].dLength = m_pDoc->calcLAVElem(ElemKey);
			Get_FlexBuckMembForceD_Sub(ElemKey, nDgnLcomK, pInDataFB->aMembCompD[0]);
		}
	}
	else
	{
		return;
	}
}

void CRptOrthotropicDeck_RUS::Get_FlexBuckMembForceD_Sub(T_ELEM_K ElemKey, int nDgnLcomK, STL_POSD_DGND_SNiP_MEMB &MembForExD)
{
	CStlOrthDeckDesign* pSodDesign = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
	if(pSodDesign)
	{
		T_SODD_FORCE_EX ForceExD; ForceExD.Initialize();
		pSodDesign->GetSodLcomForce_EX(ElemKey, nDgnLcomK, ForceExD);
		ConvertForceEx2DgnExForD(ForceExD, MembForExD);
	}
}

void CRptOrthotropicDeck_RUS::ConvertForceEx2DgnExForD(const T_SODD_FORCE_EX& crInD,  STL_POSD_DGND_SNiP_MEMB& rData)
{
	for(int nPos = 0; nPos < 5; ++nPos)
	{
		for(int nFor = 0; nFor < 6; ++nFor)
		{
			rData.dForce[nPos][nFor] = crInD.ForD[nPos].dForce[nFor];
		}
	}
}


BOOL CRptOrthotropicDeck_RUS::Get_LocalForceData(T_ELEM_K ElemK, T_LCOM_K nOrgLcomNo, int nPos, STL_LCOM_FORCE_OD &rData)
{
	if (nPos<0 || nPos>1) return FALSE;
	
//  if (rData.GetSize()==0) return FALSE;
	
//  STL_LCOM_FORCE_OD &StlLcomF = rData.aChkFor[0];
	
	if (nPos==0)
	{
		STL_LFORCE_RIBDECK_CHKUNIT_LCOM RibDeckLFor;
		if (m_pDataCtrl->m_pSodDataCtrl->m_mSodLForRibDeck.Lookup(ElemK, RibDeckLFor))
		{
			RibDeckLFor.mRibDeckFor.Lookup(nOrgLcomNo, rData.LocalForceD.RibDeckUnit_LForD);      
		}
		//
		STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM WebSigyLFor;
		if (m_pDataCtrl->m_pSodDataCtrl->m_mSodLForWeb.Lookup(ElemK, WebSigyLFor))
		{
			WebSigyLFor.mWebSigYFor.Lookup(nOrgLcomNo, rData.LocalForceD.WebSigy_LForD);
		}
		//
		STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM WebPlatLfor;
		if (m_pDataCtrl->m_pSodDataCtrl->m_mSodLForWebPlate.Lookup(ElemK, WebPlatLfor))
		{
			WebPlatLfor.mWebPlateFor.Lookup(nOrgLcomNo, rData.LocalForceD.WebPl_LForD);
		}
	}
	else
	{
		STL_LFORCE_RIBDECK_CHKUNIT_LCOM RibDeckLFor;
		if (m_pDataCtrl->m_pSodDataCtrl->m_mSodLForRibDeckJ.Lookup(ElemK, RibDeckLFor))
		{
			RibDeckLFor.mRibDeckFor.Lookup(nOrgLcomNo, rData.LocalForceD.RibDeckUnit_LForD);      
		}
		//
		STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM WebSigyLFor;
		if (m_pDataCtrl->m_pSodDataCtrl->m_mSodLForWebJ.Lookup(ElemK, WebSigyLFor))
		{
			WebSigyLFor.mWebSigYFor.Lookup(nOrgLcomNo, rData.LocalForceD.WebSigy_LForD);
		}
		//
		STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM WebPlatLfor;
		if (m_pDataCtrl->m_pSodDataCtrl->m_mSodLForWebPlateJ.Lookup(ElemK, WebPlatLfor))
		{
			WebPlatLfor.mWebPlateFor.Lookup(nOrgLcomNo, rData.LocalForceD.WebPl_LForD);
		}
	}
	
	return TRUE;
}

BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataFatigue(T_ELEM_K ElemK, int nIJ,  STL_MEMB_POSD_SNiP_OD &MembLcomD)
{
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	MembLcomD.aUlmf.RemoveAll();
	MembLcomD.aChkFor.RemoveAll();
	int nLcomSize=1;
	MembLcomD.aUlmf.SetSize(nLcomSize);
	MembLcomD.aChkFor.SetSize(nLcomSize);
	MembLcomD.aChkFor[0].nLcomType = 1;

	T_SDFG_CR FatiCrR;
	pCheckSod->GetSodFatiPosCrResult(ElemK, FatiCrR);
	T_SDCR_BASE &SDCR_BASE = FatiCrR.FatiCr[ix];

	int nFatiConcSize = SDCR_BASE.nLcomSize;  

	STLCHKFOR_ARRAY aChkForceI; aChkForceI.RemoveAll();
	aChkForceI.SetSize(nFatiConcSize);
	STLCHKFOR_ARRAY aChkForceJ; aChkForceJ.RemoveAll(); 
	aChkForceJ.SetSize(nFatiConcSize);
	//
	STLCHKFOR_ARRAY aChkDLForceI;  aChkDLForceI.RemoveAll();
	aChkDLForceI.SetSize(nFatiConcSize);
	STLCHKFOR_ARRAY aChkDLForceJ;  aChkDLForceJ.RemoveAll();
	aChkDLForceJ.SetSize(nFatiConcSize);

	T_SODD_DGN_FORCE SodForce;
	for(int i=0; i < nFatiConcSize; ++i)
	{
		SodForce.Initialize();
		int nDgnLcomId = SDCR_BASE.nDgnLcomKFirst + i;
		pCheckSod->GetSodLcomForce(ElemK, nDgnLcomId, SodForce);

		ConvertSodCheckForce(SodForce.Elem[0], aChkForceI[i]);
		ConvertSodCheckForce(SodForce.ElemDL[0], aChkDLForceI[i]);
		ConvertSodCheckForce(SodForce.Elem[1], aChkForceJ[i]);
		ConvertSodCheckForce(SodForce.ElemDL[1], aChkDLForceJ[i]);
	}

	if (ix==0)
	{
		MembLcomD.aChkFor[0].GlobalForceD.Copy(aChkForceI);
		MembLcomD.aChkFor[0].GlobalForceD_DL.Copy(aChkDLForceI);
	}
	else
	{
		MembLcomD.aChkFor[0].GlobalForceD.Copy(aChkForceJ);
		MembLcomD.aChkFor[0].GlobalForceD_DL.Copy(aChkDLForceJ);
	}

	T_LCOM_D LcomD;
	if(m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, SDCR_BASE.nLcomK, LcomD))
	{
		MembLcomD.aChkFor[0].strLcomName = LcomD.LoadCombName;
	}
	
	MembLcomD.aChkFor[0].strLcomType = Get_LcomType(SDCR_BASE.nConcurrent);

	return TRUE;
}

void CRptOrthotropicDeck_RUS::ConvertSodCheckForce(T_SODD_FORCE &Indata, STL_FORCE_D &Outdata)
{
	memcpy(Outdata.dForce, Indata.dForce, sizeof(Outdata.dForce));
}


BOOL CRptOrthotropicDeck_RUS::Set_SodForceData4Kapa(T_ELEM_K ElemK, int nIJ, CDgn_OrthotropicDeckManager* pSodManager, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_POSD_SNiP_OD &MembLcomD, aKAPAF_PonitInfo &KAPAF_PonitInfo, BOOL &bAllKappaF)
{
	// -> 전체 : 60
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();

	STL_MEMB_POSD_SNiP_OD MembLcomD4KapaTemp = MembLcomD;

 	MembLcomD4KapaTemp.aUlmf.RemoveAll();
 	MembLcomD4KapaTemp.aChkFor.RemoveAll();

	if(MembLcomD.Memb.GDgn.bDgnParam[0]) 
	{
		Set_SodForceDataSTN_TFL_4Kapa(ElemK, nIJ, /*pSodManager,*/MembLcomD, MembLcomD4KapaTemp);
		Set_SodForceDataSTN_BFL_4Kapa(ElemK, nIJ, /*pSodManager,*/MembLcomD, MembLcomD4KapaTemp);
		Set_SodForceDataSTN_WEB_4Kapa(ElemK, nIJ, /*pSodManager,*/MembLcomD, MembLcomD4KapaTemp);
	}
	if(MembLcomD.Memb.GDgn.bDgnParam[1]) 
	{
		Set_SodForceDataSTB_TFL_4Kapa(ElemK, nIJ, /*pSodManager,*/MembLcomD, MembLcomD4KapaTemp);
		Set_SodForceDataSTB_BFL_4Kapa(ElemK, nIJ, /*pSodManager,*/MembLcomD, MembLcomD4KapaTemp);
		Set_SodForceDataSTB_WEB_4Kapa(ElemK, nIJ, /*pSodManager,*/MembLcomD, MembLcomD4KapaTemp);
	}
	if(MembLcomD.Memb.GDgn.bDgnParam[2]) 
	{
		STL_MEMB_POSD_SNiP_OD MembLcomDFat = MembLcomD;
		Set_SodForceDataFatigue(ElemK, nIJ,  MembLcomDFat);
		
		T_SDFG_CR FAT_CR;   // 가장 불리한 Fatigue Lcom
		if(!pCheckSod->GetSodFatiPosCrResult(ElemK, FAT_CR)) FAT_CR.Initialize();
		
		T_SDFG_CASE SDFG; // Flange 파트별 검토 결과
		T_SDCR_BASE SdfgB  = FAT_CR.FatiCr[nIJ];   
		if(!pCheckSod->GetSodChkFatiResult(ElemK, SdfgB.nLcomK, SDFG)) SDFG.Initialize();
		
	  T_SDFG_BASE	rData  =  SDFG.FatiB[nIJ]; 
		if(rData.bChk)
		{
			MembLcomD4KapaTemp.aChkFor.Add(MembLcomDFat.aChkFor[0]);
		}
	}
	
	aKAPAF_PonitInfo KAPAF_PonitInfo_Temp;

	STL_MEMB_POSD_SNiP_OD MembLcomD4Kapa;  MembLcomD4Kapa = MembLcomD; 
	
	double dCheckKappa_Rat = 0.0;
	BOOL bAllKappa = TRUE;
	int nChkForSize = MembLcomD4KapaTemp.aChkFor.GetSize();
	for(int i=0; i<nChkForSize; ++i)
	{
		MembLcomD4Kapa.aChkFor.RemoveAll();
		MembLcomD4Kapa.aChkFor.Add(MembLcomD4KapaTemp.aChkFor[i]);

		 
		STL_RES_BASE_SNiP_OD_SNiP MembResD4Kapa = MembResD;

		KAPAF_PonitInfo_Temp.RemoveAll();
		double dCheckKappa_Rat_Temp = 0.0;
		pSodManager->Check_KappaF_StlOrthDeckSNiP(MembLcomD4Kapa, MembResD4Kapa, bAllKappa, dCheckKappa_Rat_Temp, KAPAF_PonitInfo_Temp);

		if(dCheckKappa_Rat < dCheckKappa_Rat_Temp)
		{
			dCheckKappa_Rat = dCheckKappa_Rat_Temp; 
			KAPAF_PonitInfo.Copy(KAPAF_PonitInfo_Temp);
		}

	}

	bAllKappaF = bAllKappa;

	return TRUE;
}

BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTN_TFL_4Kapa(T_ELEM_K ElemK, int nIJ, /*CDgn_OrthotropicDeckManager* pSodManager,*/ STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_POSD_SNiP_OD &MembLcomD4Kapa)
{
	
	// 강도
	// - TopFlange
	T_SDPT_STRN_FLG SDSN_Result;
	STL_MEMB_POSD_SNiP_OD MembLcomDStn_FLG;
	for(int nPart=0; nPart<3; ++nPart)     // L,M,R
	{
		// Flange : Deck
		MembLcomDStn_FLG = MembLcomD;
		Set_SodForceDataSTN_Flange(ElemK, nIJ, 0, nPart, SN_DECK, MembLcomDStn_FLG, SDSN_Result);  
		if(SDSN_Result.bChk_Deck)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStn_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStn_FLG_Deck.aUlmf[0]);     // use only 0
		}
		
		// Flange : Rib
		MembLcomDStn_FLG = MembLcomD;
		Set_SodForceDataSTN_Flange(ElemK, nIJ, 0, nPart, SN_RIB, MembLcomDStn_FLG, SDSN_Result);  
		if(SDSN_Result.bChk_Rib)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStn_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStn_FLG_Rib.aUlmf[0]);     // use only 0
		}
		
		// Flange : Floor Beam
		MembLcomDStn_FLG = MembLcomD;
		Set_SodForceDataSTN_Flange(ElemK, nIJ, 0, nPart, SN_FLOORBEAM, MembLcomDStn_FLG, SDSN_Result);  
		if(SDSN_Result.bChk_FB)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStn_FLG.aChkFor[0]);  // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStn_TopF_FB.aUlmf[0]);      // use only 0
		}
	}

	return TRUE;
}
BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTN_BFL_4Kapa(T_ELEM_K ElemK, int nIJ, /*CDgn_OrthotropicDeckManager* pSodManager,*/ STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_POSD_SNiP_OD &MembLcomD4Kapa)
{
	
	// 강도
	// - BotFlange
	T_SDPT_STRN_FLG SDSN_Result;
	STL_MEMB_POSD_SNiP_OD MembLcomDStn_FLG;
	for(int nPart=0; nPart<3; ++nPart)     // L,M,R
	{
		// Flange : Deck
		MembLcomDStn_FLG = MembLcomD;
		Set_SodForceDataSTN_Flange(ElemK, nIJ, 1, nPart, SN_DECK, MembLcomDStn_FLG, SDSN_Result);  
		if(SDSN_Result.bChk_Deck)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStn_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStn_FLG_Deck.aUlmf[0]);     // use only 0
		}
		
		// Flange : Rib
		MembLcomDStn_FLG = MembLcomD;
		Set_SodForceDataSTN_Flange(ElemK, nIJ, 1, nPart, SN_RIB, MembLcomDStn_FLG, SDSN_Result);  
		if(SDSN_Result.bChk_Rib)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStn_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStn_FLG_Rib.aUlmf[0]);     // use only 0
		}
		
		// Flange : Floor Beam
		MembLcomDStn_FLG = MembLcomD;
		Set_SodForceDataSTN_Flange(ElemK, nIJ, 1, nPart, SN_FLOORBEAM, MembLcomDStn_FLG, SDSN_Result);  
		if(SDSN_Result.bChk_FB)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStn_FLG.aChkFor[0]);  // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStn_TopF_FB.aUlmf[0]);      // use only 0
		}
	}
	return TRUE;
}
BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTN_WEB_4Kapa(T_ELEM_K ElemK, int nIJ, /*CDgn_OrthotropicDeckManager* pSodManager,*/ STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_POSD_SNiP_OD &MembLcomD4Kapa)
{
	T_SDPT_STRN_WEB SDSN_ResultWeb;
	STL_MEMB_POSD_SNiP_OD MembLcomDStn_Web;
	for(int nLeft=0; nLeft<2; ++nLeft) // 0=Left, 1: Right
	{
		BOOL bLeft = (nLeft==0)? TRUE : FALSE;
		
		MembLcomDStn_Web = MembLcomD;
		Set_SodForceDataSTN_Web(ElemK, nIJ, bLeft, SN_WEB_NoM, MembLcomDStn_Web, SDSN_ResultWeb);  
		//STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_WEB_STR_NoM;
		if(SDSN_ResultWeb.bChk_DeckNoM)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStn_Web.aChkFor[0]);  // use only 0  
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStn_Web_NoM.aUlmf[0]);      // use only 0
		}
		
		MembLcomDStn_Web = MembLcomD;
		Set_SodForceDataSTN_Web(ElemK, nIJ, bLeft, SN_WEB_VON, MembLcomDStn_Web, SDSN_ResultWeb);  
		if(SDSN_ResultWeb.bChk_DeckVon)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStn_Web.aChkFor[0]);  // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStn_Web_VON.aUlmf[0]);      // use only 0
		}
		
		MembLcomDStn_Web = MembLcomD;
		Set_SodForceDataSTN_Web(ElemK, nIJ, bLeft, SN_WEB_RIB, MembLcomDStn_Web, SDSN_ResultWeb);  
		if(SDSN_ResultWeb.bChk_Rib)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStn_Web.aChkFor[0]);  // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStn_Web_Rib.aUlmf[0]);      // use only 0
		}
	}
	
	return TRUE;
}

BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTB_TFL_4Kapa(T_ELEM_K ElemK, int nIJ, /*CDgn_OrthotropicDeckManager* pSodManager,*/ STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_POSD_SNiP_OD &MembLcomD4Kapa)
{
	//******************
	// Global Stability 
	//******************
	T_SDSB_GD SDSB_GDR, SDSB_GFB, SDSB_GFTB;

	// Flexural Buckling
	STL_MEMB_POSD_SNiP_OD MembLcomDStb_GD = MembLcomD;
	Set_SodForceDataSTB_FB(ElemK, nIJ, MembLcomDStb_GD, SDSB_GFB);
	if(SDSB_GFB.bChk)
	{
		MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_GD.aChkFor[0]); // use only 0
		//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_GD.aUlmf[0]);     // use only 0
	}

	// Flexural Torsional Buckling
	MembLcomDStb_GD.Initialize();
	Set_SodForceDataSTB_FTB(ElemK, nIJ, MembLcomDStb_GD, SDSB_GFTB);
	if(SDSB_GFTB.bChk)
	{
		MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_GD.aChkFor[0]); // use only 0
		//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_GD.aUlmf[0]);     // use only 0
	}

	// Lateral Torsional Buckling
	MembLcomDStb_GD.Initialize();
	Set_SodForceDataSTB_GD(ElemK, nIJ, MembLcomDStb_GD, SDSB_GDR);
	if(SDSB_GDR.bChk)
	{
		MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_GD.aChkFor[0]); // use only 0
		//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_GD.aUlmf[0]);     // use only 0
	}
		
	//******************
	//  Flange 
	//******************
	STL_MEMB_POSD_SNiP_OD MembLcomDStb_FLG;
	T_SDPT_STAB_FLG SDSB_Result;
	for(int nPart=0; nPart<3; ++nPart)
	{
		// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
	
		// Flange : DeckG
		MembLcomDStb_FLG = MembLcomD;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, 0, nPart, SB_DECK_G, MembLcomDStb_FLG, SDSB_Result);  
		if(SDSB_Result.bChk_DeckG)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_FLG_DeckG.aUlmf[0]);     // use only 0
		}
		
		// Flange : DeckL
		MembLcomDStb_FLG = MembLcomD;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, 0, nPart, SB_DECK_L, MembLcomDStb_FLG, SDSB_Result);  
		if(SDSB_Result.bChk_DeckL)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_FLG_DeckL.aUlmf[0]);     // use only 0
		}
		
		// Flange : Rib
		MembLcomDStb_FLG = MembLcomD;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, 0, nPart, SB_RIB, MembLcomDStb_FLG, SDSB_Result);  
		if(SDSB_Result.bChk_Rib)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_FLG_Rib.aUlmf[0]);     // use only 0
		}
		
		// Flange : FloorBeam_G
		MembLcomDStb_FLG = MembLcomD;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, 0, nPart, SB_FLOORBEAM_G, MembLcomDStb_FLG, SDSB_Result);  
		if(SDSB_Result.bChk_FBG)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_FLG_FBG.aUlmf[0]);     // use only 0
		}
		
		// Flange : FloorBeam_L
		MembLcomDStb_FLG = MembLcomD;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, 0, nPart, SB_FLOORBEAM_L, MembLcomDStb_FLG, SDSB_Result);  
		if(SDSB_Result.bChk_FBL)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_FLG_FBL.aUlmf[0]);     // use only 0
		}
	}

	return TRUE;
}

BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTB_BFL_4Kapa(T_ELEM_K ElemK, int nIJ, /*CDgn_OrthotropicDeckManager* pSodManager,*/ STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_POSD_SNiP_OD &MembLcomD4Kapa)
{
	
	//******************
	//  Flange 
	//******************
	STL_MEMB_POSD_SNiP_OD MembLcomDStb_FLG;
	T_SDPT_STAB_FLG SDSB_Result;
	for(int nPart=0; nPart<3; ++nPart)
	{
		// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
		
		// Flange : DeckG
		MembLcomDStb_FLG = MembLcomD;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, 1, nPart, SB_DECK_G, MembLcomDStb_FLG, SDSB_Result);  
		if(SDSB_Result.bChk_DeckG)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_FLG_DeckG.aUlmf[0]);     // use only 0
		}
		
		// Flange : DeckL
		MembLcomDStb_FLG = MembLcomD;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, 1, nPart, SB_DECK_L, MembLcomDStb_FLG, SDSB_Result);  
		if(SDSB_Result.bChk_DeckL)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_FLG_DeckL.aUlmf[0]);     // use only 0
		}
		
		// Flange : Rib
		MembLcomDStb_FLG = MembLcomD;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, 1, nPart, SB_RIB, MembLcomDStb_FLG, SDSB_Result);  
		if(SDSB_Result.bChk_Rib)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_FLG_Rib.aUlmf[0]);     // use only 0
		}
		
		// Flange : FloorBeam_G
		MembLcomDStb_FLG = MembLcomD;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, 1, nPart, SB_FLOORBEAM_G, MembLcomDStb_FLG, SDSB_Result);  
		if(SDSB_Result.bChk_FBG)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_FLG_FBG.aUlmf[0]);     // use only 0
		}
		
		// Flange : FloorBeam_L
		MembLcomDStb_FLG = MembLcomD;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, 1, nPart, SB_FLOORBEAM_L, MembLcomDStb_FLG, SDSB_Result);  
		if(SDSB_Result.bChk_FBL)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_FLG.aChkFor[0]); // use only 0
			//MembLcomD4Kapa.aUlmf.Append(MembLcomDStb_FLG_FBL.aUlmf[0]);     // use only 0
		}
	}

	return TRUE;
}


BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTB_WEB_4Kapa(T_ELEM_K ElemK, int nIJ, /*CDgn_OrthotropicDeckManager* pSodManager,*/ STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_POSD_SNiP_OD &MembLcomD4Kapa)
{
	
	//******************
	// Web 
	//******************
	STL_MEMB_POSD_SNiP_OD MembLcomDStb_Web;
	T_SDPT_STAB_WEB SDSB_ResultWeb;
	for(int nLeft=0; nLeft<2; ++nLeft)
	{
		BOOL bLeft = (nLeft==0)? TRUE : FALSE;
		
		MembLcomDStb_Web= MembLcomD;
		Set_SodForceDataSTB_Web(ElemK, nIJ, bLeft, SB_WEB_DECK, MembLcomDStb_Web, SDSB_ResultWeb);  
		if(SDSB_ResultWeb.bChk_Deck)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_Web.aChkFor[0]);  // use only 0  
			//MembLcomD.aUlmf.Append(MembLcomDStb_Web_Deck.aUlmf[0]);      // use only 0
		}	
		
		MembLcomDStb_Web = MembLcomD;
		Set_SodForceDataSTB_Web(ElemK, nIJ, bLeft, SB_WEB_RIB, MembLcomDStb_Web, SDSB_ResultWeb);  
		if(SDSB_ResultWeb.bChk_Rib)
		{
			MembLcomD4Kapa.aChkFor.Add(MembLcomDStb_Web.aChkFor[0]);  // use only 0  
			//MembLcomD.aUlmf.Append(MembLcomDStb_Web_Rib.aUlmf[0]);      // use only 0
		}		
	}
	return TRUE;
}

BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTN_Result_FLG(T_ELEM_K ElemK, int nIJ, CDgn_OrthotropicDeckManager* pSodManager, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_POSD_SNiP_OD &MembLcomDStn, STL_MEMB_POSD_SNiP_OD &MembLcomD,
																												 STL_MEMB_RES_SNiP_OD &RptData, BOOL bAllKappaF)
{

	// Top Flange(9)
	// - L : Deck, Rib, FB
	// - M : Deck, Rib, FB
	// - R : Deck, Rib, FB
	
	//******************
	// Top Flange
	//******************
	STL_MEMB_POSD_SNiP_OD MembLcomDStn_FLG; 
	T_SDPT_STRN_FLG SDSN_Result;
	for(int nPart=0; nPart<3; ++nPart)
	{
		// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
		BOOL bChk=FALSE;
		// Flange : Deck
		MembLcomDStn_FLG = MembLcomDStn;
		Set_SodForceDataSTN_Flange(ElemK, nIJ, TRUE, nPart, SN_DECK, MembLcomDStn_FLG, SDSN_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STR_Deck; StrnStabD_FLG_STR_Deck.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STR_Deck.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(MembLcomDStn_FLG, MembResD, StrnStabD_FLG_STR_Deck);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].aDeckPlateD.Copy(StrnStabD_FLG_STR_Deck.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].aDeckPlateD);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].nCrIndex4DP = SDSN_Result.nDeck_cr;
        RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].bChk_DP = SDSN_Result.bChk_Deck;
		if(SDSN_Result.bChk_Deck)
		{
			MembLcomD.aChkFor.Add(MembLcomDStn_FLG.aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(MembLcomDStn_FLG.aUlmf[0]);     // use only 0
			bChk=TRUE;
		}

		// Flange : Rib
		MembLcomDStn_FLG = MembLcomDStn;
		Set_SodForceDataSTN_Flange(ElemK, nIJ, TRUE, nPart, SN_RIB, MembLcomDStn_FLG, SDSN_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STR_Rib; StrnStabD_FLG_STR_Rib.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STR_Rib.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(MembLcomDStn_FLG, MembResD, StrnStabD_FLG_STR_Rib);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].aResRibD.Copy(StrnStabD_FLG_STR_Rib.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].aResRibD);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].nCrIndex4Rib = SDSN_Result.nRib_cr;
        RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].bChk_Rib = SDSN_Result.bChk_Rib;
		if(SDSN_Result.bChk_Rib)
		{
			MembLcomD.aChkFor.Add(MembLcomDStn_FLG.aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(MembLcomDStn_FLG.aUlmf[0]);     // use only 0
			bChk=TRUE;
		}
		
		// Flange : Floor Beam
		MembLcomDStn_FLG = MembLcomDStn;
		Set_SodForceDataSTN_Flange(ElemK, nIJ, TRUE, nPart, SN_FLOORBEAM, MembLcomDStn_FLG, SDSN_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_TopF_STR_FB; StrnStabD_TopF_STR_FB.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_TopF_STR_FB.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(MembLcomDStn_FLG, MembResD, StrnStabD_TopF_STR_FB);
		for(int nFB_top = 0; nFB_top < 3; ++nFB_top)
		{
			RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].aFloorBeamD[nFB_top] = StrnStabD_TopF_STR_FB.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].aFloorBeamD[nFB_top];
		}
		
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].nCrIndex4FB = SDSN_Result.nFB_cr;
        RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].bChk_FB = SDSN_Result.bChk_FB;
		if(SDSN_Result.bChk_FB)
		{
			MembLcomD.aChkFor.Add(MembLcomDStn_FLG.aChkFor[0]);  // use only 0
			MembLcomD.aUlmf.Add(MembLcomDStn_FLG.aUlmf[0]);      // use only 0
			bChk=TRUE;
		}

		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Top[nPart].bChk = bChk;
	}

	// Bottom Flange(9)
	// - L : Deck, Rib, FB
	// - M : Deck, Rib, FB
	// - R : Deck, Rib, FB
	
	//******************
 	// Bottom Flange 
	//******************
	for(int nPart=0; nPart<3; ++nPart)
	{
		// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
		BOOL bChk=FALSE;
		
		// Flange : Deck
		MembLcomDStn_FLG= MembLcomDStn;
		Set_SodForceDataSTN_Flange(ElemK, nIJ, FALSE, nPart, SN_DECK, MembLcomDStn_FLG, SDSN_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STR_Deck; StrnStabD_FLG_STR_Deck.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STR_Deck.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(MembLcomDStn_FLG, MembResD, StrnStabD_FLG_STR_Deck);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].aDeckPlateD.Copy(StrnStabD_FLG_STR_Deck.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].aDeckPlateD);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].nCrIndex4DP = SDSN_Result.nDeck_cr;
        RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].bChk_DP = SDSN_Result.bChk_Deck;
		if(SDSN_Result.bChk_Deck)
		{
			MembLcomD.aChkFor.Add(MembLcomDStn_FLG.aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(MembLcomDStn_FLG.aUlmf[0]);     // use only 0
			bChk=TRUE;
		}
		
		// Flange : Rib
		MembLcomDStn_FLG = MembLcomDStn;
		Set_SodForceDataSTN_Flange(ElemK, nIJ, FALSE, nPart, SN_RIB, MembLcomDStn_FLG, SDSN_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STR_Rib; StrnStabD_FLG_STR_Rib.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STR_Rib.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(MembLcomDStn_FLG, MembResD, StrnStabD_FLG_STR_Rib);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].aResRibD.Copy(StrnStabD_FLG_STR_Rib.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].aResRibD);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].nCrIndex4Rib = SDSN_Result.nRib_cr;
        RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].bChk_Rib = SDSN_Result.bChk_Rib;
		if(SDSN_Result.bChk_Rib)
		{
			MembLcomD.aChkFor.Add(MembLcomDStn_FLG.aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(MembLcomDStn_FLG.aUlmf[0]);     // use only 0
			bChk=TRUE;
		}
		
		// Flange : Floor Beam
		MembLcomDStn_FLG = MembLcomDStn;
		Set_SodForceDataSTN_Flange(ElemK, nIJ, FALSE, nPart, SN_FLOORBEAM, MembLcomDStn_FLG, SDSN_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_TopF_STR_FB; StrnStabD_TopF_STR_FB.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_TopF_STR_FB.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(MembLcomDStn_FLG, MembResD, StrnStabD_TopF_STR_FB);
		for(int nFB_bot = 0; nFB_bot < 3; ++nFB_bot)
		{
			RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].aFloorBeamD[nFB_bot] = StrnStabD_TopF_STR_FB.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].aFloorBeamD[nFB_bot];
		}		
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].nCrIndex4FB = SDSN_Result.nFB_cr;
        RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].bChk_FB = SDSN_Result.bChk_FB;
		if(SDSN_Result.bChk_FB)
		{
			MembLcomD.aChkFor.Add(MembLcomDStn_FLG.aChkFor[0]);  // use only 0
			MembLcomD.aUlmf.Add(MembLcomDStn_FLG.aUlmf[0]);      // use only 0
			bChk=TRUE;
		}

		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[nPart].bChk = bChk;
	}

	return TRUE;
}
BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTN_Result_WLG(T_ELEM_K ElemK, int nIJ, CDgn_OrthotropicDeckManager* pSodManager, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_POSD_SNiP_OD &MembLcomDStn, STL_MEMB_POSD_SNiP_OD &MembLcomD,
																												 STL_MEMB_RES_SNiP_OD &RptData, BOOL bAllKappaF)
{

	// Web(6)
	// - L : Deck1, Deck2, Rib
	// - R : Deck1, Deck2, Rib

	//******************
	// Web
	//******************
	STL_MEMB_POSD_SNiP_OD MembLcomDStn_Web;
	T_SDPT_STRN_WEB SDSN_ResultWeb;
	for(int nLeft=0; nLeft<2; ++nLeft) // 0=Left, 1: Right
	{
		BOOL bChk=FALSE;
		BOOL bLeft = (nLeft==0)? TRUE : FALSE;

		MembLcomDStn_Web = MembLcomDStn;
		Set_SodForceDataSTN_Web(ElemK, nIJ, bLeft, SN_WEB_NoM, MembLcomDStn_Web, SDSN_ResultWeb);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_WEB_STR_NoM; StrnStabD_WEB_STR_NoM.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_WEB_STR_NoM.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(MembLcomDStn_Web, MembResD, StrnStabD_WEB_STR_NoM);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.WebMembD[nLeft].WebNoMD = StrnStabD_WEB_STR_NoM.OD_Res_SNiP_StrengthD.WebMembD[nLeft].WebNoMD;
		//RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.WebMembD[nLeft].nChkCase = (StrnStabD_WEB_STR_NoM.OD_Res_SNiP_StrengthD.WebMembD[nLeft].WebNoMD.bChk)? 1 : 0;
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.WebMembD[nLeft].nChkCase = (SDSN_ResultWeb.bChk_DeckNoM)? 0 : 1;
        RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.WebMembD[nLeft].bChk_NoM = SDSN_ResultWeb.bChk_DeckNoM;
		if(SDSN_ResultWeb.bChk_DeckNoM)
		{
			MembLcomD.aChkFor.Add(MembLcomDStn_Web.aChkFor[0]);  // use only 0  
			MembLcomD.aUlmf.Add(MembLcomDStn_Web.aUlmf[0]);      // use only 0
 			bChk= TRUE;
		}

		MembLcomDStn_Web = MembLcomDStn;
		Set_SodForceDataSTN_Web(ElemK, nIJ, bLeft, SN_WEB_VON, MembLcomDStn_Web, SDSN_ResultWeb);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_WEB_STR_VON; StrnStabD_WEB_STR_VON.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_WEB_STR_VON.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(MembLcomDStn_Web, MembResD, StrnStabD_WEB_STR_VON);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.WebMembD[nLeft].WebVonD = StrnStabD_WEB_STR_VON.OD_Res_SNiP_StrengthD.WebMembD[nLeft].WebVonD;
        RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.WebMembD[nLeft].bChk_Von = SDSN_ResultWeb.bChk_DeckVon;
        if(SDSN_ResultWeb.bChk_DeckVon)
		{
			MembLcomD.aChkFor.Add(MembLcomDStn_Web.aChkFor[0]);  // use only 0
			MembLcomD.aUlmf.Add(MembLcomDStn_Web.aUlmf[0]);      // use only 0
			bChk= TRUE;
		}

		MembLcomDStn_Web = MembLcomDStn;
		Set_SodForceDataSTN_Web(ElemK, nIJ, bLeft, SN_WEB_RIB, MembLcomDStn_Web, SDSN_ResultWeb);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_WEB_STR_Rib; StrnStabD_WEB_STR_Rib.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_WEB_STR_Rib.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(MembLcomDStn_Web, MembResD, StrnStabD_WEB_STR_Rib);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.WebMembD[nLeft].aRibD.Copy(StrnStabD_WEB_STR_Rib.OD_Res_SNiP_StrengthD.WebMembD[nLeft].aRibD);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.WebMembD[nLeft].nCr_rib = SDSN_ResultWeb.nRib_cr;
        RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.WebMembD[nLeft].bChk_Rib = SDSN_ResultWeb.bChk_Rib;
		if(SDSN_ResultWeb.bChk_Rib)
		{
			MembLcomD.aChkFor.Add(MembLcomDStn_Web.aChkFor[0]);  // use only 0
			MembLcomD.aUlmf.Add(MembLcomDStn_Web.aUlmf[0]);      // use only 0
			bChk= TRUE;
		}

		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StrengthD.WebMembD[nLeft].bChk = bChk;
	}

	return TRUE;
}

BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTN_Flange(T_ELEM_K ElemKey, int nIJ, BOOL bTop, int nFlangePart, int nDeckRibFB, STL_MEMB_POSD_SNiP_OD  &MembLcomD, T_SDPT_STRN_FLG &SDSN_Result)
{
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
	int ix = (nIJ == 0) ? 0 : 1;  // I,J
	int nTopBot =(bTop)? 0 : 1;
	int nIndex = nTopBot+ix*2;

	//******************
	// Flange
	//******************
	T_SDSN_FLG_CR FLG_CR;    // Flange 파트별 가장 불리한 Lcom
	if(!pCheckSod->GetSodStrnFlgCrResult(ElemKey, FLG_CR)) FLG_CR.Initialize();

	T_SDCR_BASE SdcrB;
	if(nDeckRibFB==SN_DECK)
	{
	  SdcrB  = FLG_CR.DeckCr[nIndex];    //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom
	}
	else if(nDeckRibFB==SN_RIB)
	{
		SdcrB  = FLG_CR.RibCr[nIndex];   
	}
	else if(nDeckRibFB==SN_FLOORBEAM)
	{
		SdcrB  = FLG_CR.FBeamCr[nIndex];   
	}
	else ASSERT(0);

	// 설계 위한 하중 변환 - aChkFor가 13개로 변환된다..  
	ConvertSodForce2Engine(ElemKey, nIJ, 0, SdcrB,  MembLcomD);

	// Flange 파트별 검토 결과
	T_SDSN_FLG_CASE SDSN_FLG; 
	if(bTop) 
	{ 
		if(!pCheckSod->GetSodTopFlgStrnResult(ElemKey, SdcrB.nDgnLcomK, 0, SDSN_FLG)) SDSN_FLG.Initialize();
	}
	else
	{
		if(!pCheckSod->GetSodBotFlgStrnResult(ElemKey, SdcrB.nDgnLcomK, 0, SDSN_FLG)) SDSN_FLG.Initialize();
	}
	T_SDSN_FLG SDSN_FLANGE = SDSN_FLG.StrnFlg[ix]; // I, J	
	SDSN_Result.Initialize();
	SDSN_Result = SDSN_FLANGE.StrnChk[nFlangePart]; // I-L,R // Box-L,M,R 

	return TRUE;
}

BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTN_Web(T_ELEM_K ElemKey, int nIJ, BOOL bLeft, int nWebChkType, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDPT_STRN_WEB &SDSN_Result)
{
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	SDSN_Result.Initialize();

	//******************
	// Web
	//******************
	T_SDSN_WEB_CR WEB_CR;   // Web 파트별 가장 불리한 Lcom
	if(!pCheckSod->GetSodStrnWebCrResult(ElemKey, WEB_CR)) WEB_CR.Initialize();

	T_SDCR_BASE SdcrB;
	if(nWebChkType == SN_WEB_NoM)
	{
		SdcrB  = (bLeft)? WEB_CR.DeckNoMCrL[ix] : WEB_CR.DeckNoMCrR[ix];    //0=I 1=J
	}
	else if(nWebChkType == SN_WEB_VON)
	{
		SdcrB  = (bLeft)? WEB_CR.DeckVonCrL[ix] : WEB_CR.DeckVonCrR[ix];    //0=I 1=J
	}
	else if(nWebChkType == SN_WEB_RIB)
	{
		SdcrB  = (bLeft)? WEB_CR.RibCrL[ix] : WEB_CR.RibCrR[ix];    //0=I 1=J
	}
	else ASSERT(0);

	// 설계 위한 하중 변환 - aChkFor가 13개로 변환된다.. 
	ConvertSodForce2Engine(ElemKey, nIJ, 0, SdcrB,  MembLcomD);

	// Web 파트별 검토 결과
	T_SDSN_WEB_CASE SDSN_WEB; 
	if(!pCheckSod->GetSodWebStrnResult(ElemKey, SdcrB.nDgnLcomK, 0, SDSN_WEB)) SDSN_WEB.Initialize();
	T_SDSN_WEB SDSN_Web_Result =  SDSN_WEB.StrnWeb[ix]; // I, J
	SDSN_Result.Initialize();
	SDSN_Result = (bLeft)? SDSN_Web_Result.WebD[0] : SDSN_Web_Result.WebD[1];

	return TRUE;
}


BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTB_Result_FLG(T_ELEM_K ElemK, int nIJ, CDgn_OrthotropicDeckManager* pSodManager, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_POSD_SNiP_OD &MembLcomDStb, STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_RES_SNiP_OD &RptData, BOOL bAllKappaF)
{
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();

	//******************
	// Global Stability 
	//******************
	T_SDSB_GD SDSB_GDR;
	STL_MEMB_POSD_SNiP_OD* pMembLcomDStb_tmp = new STL_MEMB_POSD_SNiP_OD() ; *pMembLcomDStb_tmp =MembLcomDStb;
	STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_GD; 

	// Flexural Buckling
	Set_SodForceDataSTB_FB(ElemK, nIJ, *pMembLcomDStb_tmp, SDSB_GDR);
	StrnStabD_GD.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_GD.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
	pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_GD);
	RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.GloblaSTA_FB =  StrnStabD_GD.OD_Res_SNiP_StabilityD.GloblaSTA_FB;

	// Flexural Torsional Buckling
	Set_SodForceDataSTB_FTB(ElemK, nIJ, *pMembLcomDStb_tmp, SDSB_GDR);
	StrnStabD_GD.Initialize(); StrnStabD_GD.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_GD.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
	pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_GD);
	RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.GloblaSTA_FTB =  StrnStabD_GD.OD_Res_SNiP_StabilityD.GloblaSTA_FTB;

	// Lateral Torsional Buckling
	Set_SodForceDataSTB_GD(ElemK, nIJ, *pMembLcomDStb_tmp, SDSB_GDR);
	StrnStabD_GD.Initialize(); StrnStabD_GD.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_GD.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
	pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_GD);
	RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.GlobalSTA_LTB =  StrnStabD_GD.OD_Res_SNiP_StabilityD.GlobalSTA_LTB;
	if(SDSB_GDR.bChk)
	{
		MembLcomD.aChkFor.Add(pMembLcomDStb_tmp->aChkFor[0]); // use only 0
		MembLcomD.aUlmf.Add(pMembLcomDStb_tmp->aUlmf[0]);     // use only 0
	}		

	// Top Flange(15)
	// - L : Deck1, Deck2, Rib, FB1, FB2
	// - M : Deck1, Deck2, Rib, FB1, FB2
	// - R : Deck1, Deck2, Rib, FB1, FB2

	//******************
	// Top Flange 
	//******************
	pMembLcomDStb_tmp->Initialize();
	T_SDPT_STAB_FLG SDSB_Result;
	for(int nPart=0; nPart<3; ++nPart)
	{
		// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
		BOOL bChk=FALSE;

		// Flange : DeckG
		*pMembLcomDStb_tmp = MembLcomDStb;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, TRUE, nPart, SB_DECK_G, *pMembLcomDStb_tmp, SDSB_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STA_DeckG; StrnStabD_FLG_STA_DeckG.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STA_DeckG.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_FLG_STA_DeckG);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].GDeckPlate_Sta = StrnStabD_FLG_STA_DeckG.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].GDeckPlate_Sta;
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].nIndexCriticalDP_G = SDSB_Result.nDeckG_cr;
		if(SDSB_Result.bChk_DeckG)
		{
			MembLcomD.aChkFor.Add(pMembLcomDStb_tmp->aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(pMembLcomDStb_tmp->aUlmf[0]);     // use only 0
			bChk=TRUE;
		}

		// Flange : DeckL
		*pMembLcomDStb_tmp = MembLcomDStb;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, TRUE, nPart, SB_DECK_L, *pMembLcomDStb_tmp, SDSB_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STA_DeckL; StrnStabD_FLG_STA_DeckL.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STA_DeckL.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_FLG_STA_DeckL);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].aLDeckPlate_Sta.Copy(StrnStabD_FLG_STA_DeckL.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].aLDeckPlate_Sta);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].nIndexCriticalDP_L = SDSB_Result.nDeckL_cr;
		if(SDSB_Result.bChk_DeckL)
		{
			MembLcomD.aChkFor.Add(pMembLcomDStb_tmp->aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(pMembLcomDStb_tmp->aUlmf[0]);     // use only 0
			bChk=TRUE;
		}

		// Flange : Rib
		*pMembLcomDStb_tmp = MembLcomDStb;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, TRUE, nPart, SB_RIB, *pMembLcomDStb_tmp, SDSB_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STA_Rib; StrnStabD_FLG_STA_Rib.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STA_Rib.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_FLG_STA_Rib);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].aLRib_Sta.Copy(StrnStabD_FLG_STA_Rib.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].aLRib_Sta);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].nIndexCriticalRib = SDSB_Result.nRib_cr;
		if(SDSB_Result.bChk_Rib)
		{
			MembLcomD.aChkFor.Add(pMembLcomDStb_tmp->aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(pMembLcomDStb_tmp->aUlmf[0]);     // use only 0
			bChk=TRUE;
		}

		// Flange : FloorBeam_G
		*pMembLcomDStb_tmp = MembLcomDStb;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, TRUE, nPart, SB_FLOORBEAM_G, *pMembLcomDStb_tmp, SDSB_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STA_FBG; StrnStabD_FLG_STA_FBG.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STA_FBG.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_FLG_STA_FBG);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].aFloorBeam_Sta[SDSB_Result.nFBG_cr] = StrnStabD_FLG_STA_FBG.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].aFloorBeam_Sta[SDSB_Result.nFBG_cr];
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].nIndexCriticalFB = SDSB_Result.nFBG_cr;
		if(SDSB_Result.bChk_FBG)
		{
			MembLcomD.aChkFor.Add(pMembLcomDStb_tmp->aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(pMembLcomDStb_tmp->aUlmf[0]);     // use only 0
			bChk=TRUE;
		}

		// Flange : FloorBeam_L
		*pMembLcomDStb_tmp = MembLcomDStb;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, TRUE, nPart, SB_FLOORBEAM_L, *pMembLcomDStb_tmp, SDSB_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STA_FBL; StrnStabD_FLG_STA_FBL.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STA_FBL.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_FLG_STA_FBL);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].aLFB_Sta.Copy(StrnStabD_FLG_STA_FBG.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].aLFB_Sta);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].nIndexCriticalFB_LSta = SDSB_Result.nFBL_cr;
		if(SDSB_Result.bChk_FBL)
		{
			MembLcomD.aChkFor.Add(pMembLcomDStb_tmp->aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(pMembLcomDStb_tmp->aUlmf[0]);     // use only 0
			bChk=TRUE;
		}

		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Top[nPart].bChk = bChk;

	}

	// Bottom Flange(15)
	// - L : Deck1, Deck2, Rib, FB1, FB2
	// - M : Deck1, Deck2, Rib, FB1, FB2
	// - R : Deck1, Deck2, Rib, FB1, FB2

	//******************
	// Bottom Flange 
	//******************
	for(int nPart=0; nPart<3; ++nPart)
	{
		// nPart=0: FLANGE_LEFT 1:FLANGE_MIDDLE, 2:FLANGE_RIGHT
		BOOL bChk=FALSE;
		
		// Flange : DeckG
		*pMembLcomDStb_tmp = MembLcomDStb;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, FALSE, nPart, SB_DECK_G, *pMembLcomDStb_tmp, SDSB_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STA_DeckG; StrnStabD_FLG_STA_DeckG.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STA_DeckG.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_FLG_STA_DeckG);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].GDeckPlate_Sta = StrnStabD_FLG_STA_DeckG.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].GDeckPlate_Sta;
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].nIndexCriticalDP_G = SDSB_Result.nDeckG_cr;
		if(SDSB_Result.bChk_DeckG)
		{
			MembLcomD.aChkFor.Add(pMembLcomDStb_tmp->aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(pMembLcomDStb_tmp->aUlmf[0]);     // use only 0
			bChk=TRUE;
		}
		
		// Flange : DeckL
		*pMembLcomDStb_tmp = MembLcomDStb;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, FALSE, nPart, SB_DECK_L, *pMembLcomDStb_tmp, SDSB_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STA_DeckL; StrnStabD_FLG_STA_DeckL.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STA_DeckL.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_FLG_STA_DeckL);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].aLDeckPlate_Sta.Copy(StrnStabD_FLG_STA_DeckL.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].aLDeckPlate_Sta);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].nIndexCriticalDP_L = SDSB_Result.nDeckL_cr;
		if(SDSB_Result.bChk_DeckL)
		{
			MembLcomD.aChkFor.Add(pMembLcomDStb_tmp->aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(pMembLcomDStb_tmp->aUlmf[0]);     // use only 0
			bChk=TRUE;
		}
		
		// Flange : Rib
		*pMembLcomDStb_tmp = MembLcomDStb;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, FALSE, nPart, SB_RIB, *pMembLcomDStb_tmp, SDSB_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STA_Rib; StrnStabD_FLG_STA_Rib.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STA_Rib.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_FLG_STA_Rib);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].aLRib_Sta.Copy(StrnStabD_FLG_STA_Rib.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].aLRib_Sta);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].nIndexCriticalRib = SDSB_Result.nRib_cr;
		if(SDSB_Result.bChk_Rib)
		{
			MembLcomD.aChkFor.Add(pMembLcomDStb_tmp->aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(pMembLcomDStb_tmp->aUlmf[0]);     // use only 0
			bChk=TRUE;
		}
		
		// Flange : FloorBeam_G
		*pMembLcomDStb_tmp = MembLcomDStb;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, FALSE, nPart, SB_FLOORBEAM_G, *pMembLcomDStb_tmp, SDSB_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STA_FBG; StrnStabD_FLG_STA_FBG.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STA_FBG.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_FLG_STA_FBG);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].aFloorBeam_Sta[SDSB_Result.nFBG_cr] = StrnStabD_FLG_STA_FBG.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].aFloorBeam_Sta[SDSB_Result.nFBG_cr];
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].nIndexCriticalFB = SDSB_Result.nFBG_cr;
		if(SDSB_Result.bChk_FBG)
		{
			MembLcomD.aChkFor.Add(pMembLcomDStb_tmp->aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(pMembLcomDStb_tmp->aUlmf[0]);     // use only 0
			bChk=TRUE;
		}
		
		// Flange : FloorBeam_L
		*pMembLcomDStb_tmp = MembLcomDStb;
		Set_SodForceDataSTB_Flange(ElemK, nIJ, FALSE, nPart, SB_FLOORBEAM_L, *pMembLcomDStb_tmp, SDSB_Result);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_FLG_STA_FBL; StrnStabD_FLG_STA_FBL.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_FLG_STA_FBL.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(*pMembLcomDStb_tmp, MembResD, StrnStabD_FLG_STA_FBL);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].aLFB_Sta.Copy(StrnStabD_FLG_STA_FBG.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].aLFB_Sta);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].nIndexCriticalFB_LSta = SDSB_Result.nFBL_cr;
		if(SDSB_Result.bChk_FBG)
		{
			MembLcomD.aChkFor.Add(pMembLcomDStb_tmp->aChkFor[0]); // use only 0
			MembLcomD.aUlmf.Add(pMembLcomDStb_tmp->aUlmf[0]);     // use only 0
			bChk=TRUE;
		}		

		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[nPart].bChk = bChk;

	}

	if(pMembLcomDStb_tmp != nullptr){delete pMembLcomDStb_tmp;}

	return TRUE;
}
BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTB_Result_WLG(T_ELEM_K ElemK, int nIJ, CDgn_OrthotropicDeckManager* pSodManager, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_POSD_SNiP_OD &MembLcomDStb, STL_MEMB_POSD_SNiP_OD &MembLcomD, STL_MEMB_RES_SNiP_OD &RptData, BOOL bAllKappaF)
{
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();

 	// Web(6)
	// - L : Deck, Rib
	// - R : Deck, Rib

	//******************
	// Web 
	//******************
	STL_MEMB_POSD_SNiP_OD MembLcomDStb_Web;
	T_SDPT_STAB_WEB SDSB_ResultWeb;
	for(int nLeft=0; nLeft<2; ++nLeft)
	{
		BOOL bChk=FALSE;
		BOOL bLeft = (nLeft==0)? TRUE : FALSE;
		
		MembLcomDStb_Web = MembLcomDStb;
		Set_SodForceDataSTB_Web(ElemK, nIJ, bLeft, SB_WEB_DECK, MembLcomDStb_Web, SDSB_ResultWeb);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_WEB_STA_Deck; StrnStabD_WEB_STA_Deck.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_WEB_STA_Deck.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(MembLcomDStb_Web, MembResD, StrnStabD_WEB_STA_Deck);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.aResWebStaD[nLeft] = StrnStabD_WEB_STA_Deck.OD_Res_SNiP_StabilityD.aResWebStaD[nLeft];
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.aResWebStaD[nLeft].aLRib_Sta.RemoveAll();
		if(SDSB_ResultWeb.bChk_Deck)
		{
			MembLcomD.aChkFor.Add(MembLcomDStb_Web.aChkFor[0]);  // use only 0  
			MembLcomD.aUlmf.Add(MembLcomDStb_Web.aUlmf[0]);      // use only 0
			bChk= TRUE;
		}	
		
		MembLcomDStb_Web = MembLcomDStb;
		Set_SodForceDataSTB_Web(ElemK, nIJ, bLeft, SB_WEB_RIB, MembLcomDStb_Web, SDSB_ResultWeb);  
		STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD_WEB_STA_Rib; StrnStabD_WEB_STA_Rib.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF; StrnStabD_WEB_STA_Rib.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;
		pSodManager->Check_StlOrthDeckSNiP(MembLcomDStb_Web, MembResD, StrnStabD_WEB_STA_Rib);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.aResWebStaD[nLeft].aLRib_Sta.Copy(StrnStabD_WEB_STA_Rib.OD_Res_SNiP_StabilityD.aResWebStaD[nLeft].aLRib_Sta);
		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.aResWebStaD[nLeft].nWebRibCrIndex = SDSB_ResultWeb.nRib_cr;
		if(SDSB_ResultWeb.bChk_Rib)
		{
			MembLcomD.aChkFor.Add(MembLcomDStb_Web.aChkFor[0]);  // use only 0  
			MembLcomD.aUlmf.Add(MembLcomDStb_Web.aUlmf[0]);      // use only 0
			bChk= TRUE;
		}		

		RptData.OD_Res_SNiP_Str_Sta_D.OD_Res_SNiP_StabilityD.aResWebStaD[nLeft].bChk = bChk;
	}


	return TRUE;
}


BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTB_FB(T_ELEM_K ElemKey, int nIJ, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDSB_GD &SDSB_GDR)
{
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
	int ix = (nIJ == 0) ? 0 : 1;  // I,J


	T_SDSB_GD_CR GD_CR;   // 가장 불리한 Lcom
	if(!pCheckSod->GetSodStabFBCrResult(ElemKey, GD_CR)) GD_CR.Initialize();
	T_SDCR_BASE &SdcrB_GD  = GD_CR.GDCr[ix];    //0=I 1=J

	// 설계 위한 하중 변환
	ConvertSodForce2Engine(ElemKey, nIJ, 0, SdcrB_GD,  MembLcomD);
	Get_FlexBuckMembForceD(ElemKey, nIJ, SdcrB_GD.nDgnLcomK, MembLcomD);

	T_SDSB_GD_CASE SDSB_GD; 
	if(!pCheckSod->GetSodFBStabResult(ElemKey, SdcrB_GD.nDgnLcomK, SDSB_GD)) SDSB_GD.Initialize();

	SDSB_GDR.Initialize();
	SDSB_GDR = SDSB_GD.StabGD[ix]; // I, J	

	return TRUE;
}


BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTB_FTB(T_ELEM_K ElemKey, int nIJ, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDSB_GD &SDSB_GDR)
{
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	T_SDSB_GD_CR GD_CR;   // 가장 불리한 Lcom
	if(!pCheckSod->GetSodStabFTBCrResult(ElemKey, GD_CR)) GD_CR.Initialize();
	T_SDCR_BASE &SdcrB_GD  = GD_CR.GDCr[ix];    //0=I 1=J

	// 설계 위한 하중 변환
	ConvertSodForce2Engine(ElemKey, nIJ, 0, SdcrB_GD,  MembLcomD);

	T_SDSB_GD_CASE SDSB_GD; 
	if(!pCheckSod->GetSodFTBStabResult(ElemKey, SdcrB_GD.nDgnLcomK, SDSB_GD)) SDSB_GD.Initialize();

	SDSB_GDR.Initialize();
	SDSB_GDR = SDSB_GD.StabGD[ix]; // I, J	

	return TRUE;
}


BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTB_GD(T_ELEM_K ElemKey, int nIJ, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDSB_GD &SDSB_GDR)
{
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	T_SDSB_GD_CR GD_CR;   // 가장 불리한 Lcom
	if(!pCheckSod->GetSodStabGDCrResult(ElemKey, GD_CR)) GD_CR.Initialize();
	T_SDCR_BASE &SdcrB_GD  = GD_CR.GDCr[ix];    //0=I 1=J
	
	// 설계 위한 하중 변환 - aChkFor가 13개로 변환된다
	ConvertSodForce2Engine(ElemKey, nIJ, 0, SdcrB_GD,  MembLcomD);

	// Flange 파트별 검토 결과
	T_SDSB_GD_CASE SDSB_GD; 
	if(!pCheckSod->GetSodGDStabResult(ElemKey, SdcrB_GD.nDgnLcomK, SDSB_GD)) SDSB_GD.Initialize();

	SDSB_GDR.Initialize();
	SDSB_GDR = SDSB_GD.StabGD[ix]; // I, J	

	return TRUE;
}

BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTB_Flange(T_ELEM_K ElemKey, int nIJ, BOOL bTop, int nFlangePart, int nDeckRibFB, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDPT_STAB_FLG &SDSB_Result)
{
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
	int ix = (nIJ == 0) ? 0 : 1;  // I,J
	int nTopBot =(bTop)? 0 : 1;
	int nIndex = nTopBot+ix*2;

	//******************
	// Flange
	//******************
	T_SDSB_FLG_CR FLG_CR;    // Flange 파트별 가장 불리한 Lcom
	if(!pCheckSod->GetSodStabFlgCrResult(ElemKey, FLG_CR)) FLG_CR.Initialize();

	T_SDCR_BASE SdcrB;
	if(nDeckRibFB==SB_DECK_G)
	{
	  SdcrB  = FLG_CR.DeckGCr[nIndex];    //0=I-Top, 1=I-Bottom, 2=J-Top, 3=J-Bottom
	}
	else if(nDeckRibFB==SB_DECK_L)
	{
		SdcrB  = FLG_CR.DeckLCr[nIndex];   
	}
	else if(nDeckRibFB==SB_RIB)
	{
		SdcrB  = FLG_CR.RibCr[nIndex];   
	}
	else if(nDeckRibFB==SB_FLOORBEAM_G)
	{
		SdcrB  = FLG_CR.FBeamGCr[nIndex];   
	}
	else if(nDeckRibFB==SB_FLOORBEAM_L)
	{
		SdcrB  = FLG_CR.FBeamLCr[nIndex];   
	}
	else ASSERT(0);

	// 설계 위한 하중 변환 - aChkFor가 13개로 변환된다
	ConvertSodForce2Engine(ElemKey, nIJ, 0, SdcrB,  MembLcomD);

	// Flange 파트별 검토 결과
	T_SDSB_FLG_CASE SDSB_FLG; 
	if(bTop) 
	{ 
		if(!pCheckSod->GetSodTopFlgStabResult(ElemKey, SdcrB.nDgnLcomK, 0, 0, SDSB_FLG)) SDSB_FLG.Initialize();
	}
	else
	{
		if(!pCheckSod->GetSodBotFlgStabResult(ElemKey, SdcrB.nDgnLcomK, 0, 0, SDSB_FLG)) SDSB_FLG.Initialize();
	}
	T_SDSB_FLG SDSB_FLANGE = SDSB_FLG.StabFlg[ix]; // I, J	
	SDSB_Result.Initialize();
	SDSB_Result = SDSB_FLANGE.StabChk[nFlangePart]; // I-L,R // Box-L,M,R 

	return TRUE;
}

BOOL CRptOrthotropicDeck_RUS::Set_SodForceDataSTB_Web(T_ELEM_K ElemKey, int nIJ, BOOL bLeft, int nWebChkType, STL_MEMB_POSD_SNiP_OD &MembLcomD, T_SDPT_STAB_WEB &SDSB_Result)
{
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
	int ix = (nIJ == 0) ? 0 : 1;  // I,J

	//******************
	// Web
	//******************
	T_SDSB_WEB_CR WEB_CR;   // Web 파트별 가장 불리한 Lcom
	if(!pCheckSod->GetSodStabWebCrResult(ElemKey, WEB_CR)) WEB_CR.Initialize();

	T_SDCR_BASE SdcrB;
	if(nWebChkType == SB_WEB_DECK)
	{
		SdcrB  = (bLeft)? WEB_CR.DeckCrL[ix] : WEB_CR.DeckCrR[ix];  
	}
	else if(nWebChkType == SB_WEB_RIB)
	{
		SdcrB  = (bLeft)? WEB_CR.RibCrL[ix] : WEB_CR.RibCrR[ix];    
	}
	else ASSERT(0);

	// 설계 위한 하중 변환 - aChkFor가 13개로 변환된다..  수정 필요?
	ConvertSodForce2Engine(ElemKey, nIJ, 0, SdcrB,  MembLcomD);

	// Web 파트별 검토 결과
	T_SDSB_WEB_CASE SDSB_WEB; 
	if(!pCheckSod->GetSodWebStabResult(ElemKey, SdcrB.nDgnLcomK, 0, SDSB_WEB)) SDSB_WEB.Initialize();
	T_SDSB_WEB SDSB_Web_Result =  SDSB_WEB.StabWeb[ix]; // I, J
	SDSB_Result.Initialize();
	SDSB_Result = (bLeft)? SDSB_Web_Result.WebD[0] : SDSB_Web_Result.WebD[1];

	return TRUE;
}


