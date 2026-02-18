#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "RptRating_PSC.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\AnalysisResult.h"
#include "..\wg_db\DBLib.h"

#include "Dgn_RatingRptManager.h"
//#include "DgnStlRatingDataCtrl.h"
#include "DgnPscCommon.h"
#include "DgnDataCtrl.h"
#include "RptKRRating.h"
//#include "DgnCsgDataCtrl.h"
#include "RptRatingKR_STL_ASD.h"
#include "Rating_RKMC_XLOut.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptRating_PSC::CRptRating_PSC()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;
	m_RakrD.Initialize();  
	m_pDoc->m_pAttrCtrl2->GetRakr(m_RakrD);  // Design Parameter
	
	T_RACD_D RacdD;
	if (!m_pDoc->m_pAttrCtrl2->GetRacd(RacdD)) RacdD.Initialize();
	m_pDoc->SetCivilCodeDgn(RacdD.nRatingCode);
	m_pDataCtrl = new CCRCDataCtrl();
	
	m_pForcCtrl = m_pDataCtrl->Get_ForceCtrlPointer();
	if(m_pForcCtrl==NULL) {ASSERT(0); return;}
	
}

CRptRating_PSC::~CRptRating_PSC()
{
	if(m_pDataCtrl)
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
	}
}

BOOL CRptRating_PSC::Execute_RatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
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
	
	BOOL bSuccess = TRUE;

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	CArray<T_RKPO_K, T_RKPO_K> aPrintElemKey; 
	m_pDoc->m_pAttrCtrl->GetRkpoKeyList(aPrintElemKey);  
	
	CArray<T_RKPV_K, T_RKPV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl->GetRkpvKeyList(aPrintVBeamKey);

	ArrElemPairKey aPrintElemPairK;
	CDBLib::ConvertToElemPairKeyList(aPrintElemKey, aPrintVBeamKey, aPrintElemPairK);

	int nPrintElemSize = aPrintElemPairK.GetSize();
	if(nPrintElemSize==0)
	{
		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);
		
		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("출력할 요소의 설계정보가 없습니다.")
		AfxMessageBox(strErrMessage);
		
		return FALSE;
	}

	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40/nPrintElemSize;
	
	// Input Data
	// UMD_STL_KR_D    MembLcomD;
	
	// Output Data - rating
	DGNEXCEL_KR_RCSC_OPTION OptionData;
	CArray<RF_RES_RF_KSCE_PSC_D,  RF_RES_RF_KSCE_PSC_D&>  aRptData;  aRptData.RemoveAll();
	RF_RES_RF_KSCE_PSC_D  RptData;
	
	try
	{
		for(int i=0; i<nPrintElemSize; ++i)
		{
			auto  ElemK = aPrintElemPairK[i];

			OptionData.Initialize();
			// 4. Output Data Setting(2)       
			OptionData.strPath        = strPath;//.strPath;
			OptionData.iPrintOpt      = iPrintOpt;
			//OptionData.iCode          = GetCodeConvert2Engine(CDBLib::GetRatCode());//iCodeDLL;
			OptionData.bSaveImage     = TRUE;//SOption.bSaveImage;
			OptionData.bIsView        = TRUE;
			OptionData.bIsForceSheet  = FALSE;
			OptionData.nRecType       = 0;//iReportType;

			// 1. 출력 여부 결정
			T_RKCE_D RkceD; RkceD.Initialize();  // 설계 위치
			T_RKPO_D RkpoD; RkpoD.Initialize();  // 출력 위치
			if (ElemK.second == EN_EL_BEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetRkce(ElemK.first, RkceD)) continue;
				if (!m_pDoc->m_pAttrCtrl->GetRkpo(ElemK.first, RkpoD)) continue;
			}
			else if (ElemK.second == EN_EL_VBEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetRkcv(ElemK.first, RkceD)) continue;
				if (!m_pDoc->m_pAttrCtrl->GetRkpv(ElemK.first, RkpoD)) continue;
			}
			else ASSERT(0);

			
			BOOL bPrintI = ((RkceD.nChkPosition == 0 || RkceD.nChkPosition == 2) && (RkpoD.iPrintChk == 0 || RkpoD.iPrintChk == 2) );
			BOOL bPrintJ = ((RkceD.nChkPosition == 1 || RkceD.nChkPosition == 2) && (RkpoD.iPrintChk == 1 || RkpoD.iPrintChk == 2) );

			T_RKMC_D RkmcD; RkmcD.Initialize();
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4TableKR05(ElemK, RkmcD);

			for(int j=0; j<2; j++) // I,J
			{
				RptData.Initialize();  RptData.arForce.SetSize(2);

				if(j==0 && (!bPrintI)) continue;
				if(j==1 && (!bPrintJ)) continue;

				//T_RKMC_CASE Case = 

				for(int k=0; k<2; k++) // Max/min
				{
					int nPosi = 2*j + k;   //  0=I-max(정), 1=I-min(부), 2=J-max(정), 3=J-min(부)
					T_RKMC_BASE &Base = RkmcD.RkmcBase[nPosi];
					if(Base.bCheck != TRUE) continue; 

					// 내하력 평가 계산서
 					if(!ConvertToDLL_RF(ElemK, j, k, Base, RptData)) return FALSE;
				}

				aRptData.Add(RptData);
			}

			//////////////////////////////////////////////////////////////////////////
			if(m_bStopExecute)	return FALSE;
			nProgressPercent += nProgressIncrementPercent;
			Progress(0, nProgressPercent);
			Progress(2, nProgressPercent);
		}

		if(aRptData.GetSize() < 1)
		{
			bSuccess = FALSE;	
		}
		else
		{
			// 계산서 출력	 	
			CString strDataPath = m_pDoc->GetPathName();
			CString strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");

			CRating_RKMC_XLOut myReport;   
			myReport.SetDataCtrlPointer(m_pDataCtrl);
			myReport.Print_RFReport(strBasePath, strDataPath, strPath, iPrintOpt, aRptData);   // strPath : 경로+파일이름
		}
	}
	catch (...)
	{
		bSuccess = FALSE;	
	}

	return bSuccess;
}



BOOL CRptRating_PSC::ConvertToDLL_RF(ElemPairK ElemK, int nIJ, int nPosNeg, T_RKMC_BASE& BaseD, RF_RES_RF_KSCE_PSC_D& RptD)
{
	// nIJ     : 0=I, 1=J
	// nPosNeg : 0=max(정),1=min(부)
	RptD.nElem  = ElemK.first;
	RptD.nIJ    = nIJ;
	RptD.nEstimatedMethod = m_RakrD.nSurveyMeth;
	
	RptD.Force[nPosNeg].strLcom       = BaseD.strLcomName;
	RptD.Force[nPosNeg].dphiMn        = BaseD.dPhiMn;
	RptD.Force[nPosNeg].dMd           = BaseD.dMd;
	RptD.Force[nPosNeg].dMl           = BaseD.dM_L;
	RptD.Force[nPosNeg].dImpactFactor = BaseD.d1_plus_i_calc;
	RptD.Force[nPosNeg].dRF           = BaseD.dRF;
	
	RptD.Delta[nPosNeg].strLcom        = BaseD.strLcomName;
	RptD.Delta[nPosNeg].dCalcDelta     = BaseD.dCalc_defl;
	RptD.Delta[nPosNeg].dEstiDelta     = BaseD.dRealDispZ;
	RptD.Delta[nPosNeg].dRatioDelta    = BaseD.dDisp_rat;
	RptD.Delta[nPosNeg].dCalci         = BaseD.dImpFactor;
	RptD.Delta[nPosNeg].dEstii         = BaseD.dRealImpFactor;
	RptD.Delta[nPosNeg].dRatioi        = BaseD.di_rat;
	RptD.Delta[nPosNeg].dKs            = BaseD.dKs;
	
	RptD.Ewips[nPosNeg].strLcom        = BaseD.strLcomName;
	RptD.Ewips[nPosNeg].dCalcEwips     = BaseD.dCalcEwip;
	RptD.Ewips[nPosNeg].dEstiEwips     = BaseD.dEstiEwip;
	RptD.Ewips[nPosNeg].dRatioEwips    = BaseD.dEwip_rat;
	RptD.Ewips[nPosNeg].dCalci         = BaseD.dImpFactor;
	RptD.Ewips[nPosNeg].dEstii         = BaseD.dRealImpFactor;
	RptD.Ewips[nPosNeg].dRatioi        = BaseD.di_rat;
	RptD.Ewips[nPosNeg].dKs            = BaseD.dKs;
	
	RptD.LoadP[nPosNeg].strLcom        = BaseD.strLcomName;

	CString strDesignPr; strDesignPr.Format(_T("%s-%.3f"), m_RakrD.strDgnLiveLoad, m_RakrD.dDgnLiveLoad);
	CStringW strWDesignPr(strDesignPr);
	RptD.LoadP[nPosNeg].strDesignPr = strWDesignPr;
	if (fabs(BaseD.dP0 - 0.0) > 10e-5)
	{
		CString strP0; strP0.Format(_T("%s-%.3f"), m_RakrD.strDgnLiveLoad, BaseD.dP0);  // 기본 내하력
		CStringW strWP0(strP0); 
		RptD.LoadP[nPosNeg].strP0 = strWP0; 
	}
	
	RptD.LoadP[nPosNeg].dKs            = BaseD.dKs;
	RptD.LoadP[nPosNeg].dP             = BaseD.dP0a; // 공용 내하력
	RptD.LoadP[nPosNeg].nOK            = (BaseD.bOK)? 1 : 0;
	
	RptD.Level[nPosNeg].strLcom        = BaseD.strLcomName;
	CRptRatingKR_STL_ASD RptRatingKR_STL; 
	RptD.Level[nPosNeg].strLevel       = RptRatingKR_STL.GetBridgeLevel(BaseD.nRankType);
	RptD.Level[nPosNeg].dfa            = BaseD.dPhiMn_Rank;
	RptD.Level[nPosNeg].dfdl           = BaseD.Rating_Force_Str.ForceDL.dForce[4] + BaseD.Rating_Force_Str.ForceLL.dForce[4];
	RptD.Level[nPosNeg].dRatio         = BaseD.dRatioMa;

	RptRatingKR_STL.ConvertToDLL_RptForce(BaseD.Rating_Force.ForceDL, RptD.arForce[nPosNeg].ForceDL);
	RptRatingKR_STL.ConvertToDLL_RptForce(BaseD.Rating_Force.ForceLL, RptD.arForce[nPosNeg].ForceLL);
	RptD.arForce[nPosNeg].ForceDL.strName = BaseD.strLcomName;
	RptD.arForce[nPosNeg].ForceLL.strName = BaseD.strLcomName;

	return TRUE;
}