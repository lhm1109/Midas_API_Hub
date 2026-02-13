#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "RptRating_RC_KSCE.h"

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
#include "DgnEngineProgressDlg.h"
#include "DgnPscCommon.h"
#include "DgnDataCtrl.h"
#include "RptKRRating.h"
#include "DgnRCRatingDataCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptRating_RC_KSCE::CRptRating_RC_KSCE()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;

	T_RCDC_D RcdcD;
	if (!m_pDoc->m_pAttrCtrl2->GetDgnRcdc(RcdcD)) RcdcD.Initialize();
	m_pDoc->SetCivilCodeDgn(RcdcD.nRatingCode);
	m_pDataCtrl = new CCRCDataCtrl();

	m_pForcCtrl = m_pDataCtrl->Get_ForceCtrlPointer();
	if(m_pForcCtrl==NULL) {ASSERT(0); return;}

}

CRptRating_RC_KSCE::~CRptRating_RC_KSCE()
{
	if(m_pDataCtrl)
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
	}

}

BOOL CRptRating_RC_KSCE::Excute_Print_RCRatinig_KSCE(int iDgnRatingCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
{
	BOOL bVersionOK =FALSE;

	CDgnEngineProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨
	D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
	pProInte->bUpdateTitle   = TRUE;
	pProInte->bUpdateMessage = TRUE;
	pProInte->bUpdatePercent = TRUE;

	// Report Result Data
	T_RCRatingResD_KSCE_Str_Rpt_D RCBeamRatingRptStrD; 
	T_RCRatingResD_KSCE_Rpt_D RCBeamRatingRptD; 

	int iDgnCode = GetCodeConvert2Engine(iDgnRatingCode);

	if(Make_Beam_RptD_RCRating_KSCE(iDgnRatingCode, RCBeamRatingRptStrD, RCBeamRatingRptD))
	{
		CString strDataPath = m_pDoc->GetPathName();
		CString strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");

		DGNEXCEL_KR_RCSC_OPTION OptionData;
		OptionData.Initialize();
		// 4. Output Data Setting(2)    
		CStringW strwPath(strPath);
		OptionData.strPath        = strwPath;;
		OptionData.iPrintOpt      = iPrintOpt;          // 0:출력, 1:insert
		OptionData.iCode          = iDgnCode;///*CDBLib::GetRatCode()*/;
		OptionData.bSaveImage     = TRUE;//SOption.bSaveImage;  
		OptionData.bIsView        = TRUE;
		OptionData.bIsForceSheet  = FALSE;
		//OptionData.iReportType    = 2;//iReportType;
		RCBeamRatingRptStrD.OptD = OptionData;

		T_RPAC_D RpacD;
		if (!m_pDoc->m_pAttrCtrl2->GetRpac(RpacD)) RpacD.Initialize(iDgnRatingCode);

		CDgn_RatingRptManager myReport;
		bVersionOK = myReport.Print_RC_RFReport(TRUE, strBasePath, strDataPath, strPath, RpacD.bPrintParm, RCBeamRatingRptStrD.OptD, RCBeamRatingRptStrD.LoadD, RCBeamRatingRptStrD.arRptResD, RCBeamRatingRptD.aRptData, RCBeamRatingRptStrD.arStrInD, RCBeamRatingRptStrD.arStrResD);
	}
	else {return FALSE;}

	return bVersionOK;
}

int CRptRating_RC_KSCE::GetCodeConvert2Engine(const UINT &nCode)
{
	int nEngineCode = -1;
	switch (nCode)
	{
	case KSCE_USD10_RC_RATING:
		nEngineCode = DGN_KSCE_RC_USD10;
		break;
	case KSCE_RAIL_USD11_RC_RATING:
		nEngineCode = DGN_KSCE_RC_RAIL_USD11;
		break;
	case KSCE_LSD15_RC_RATING:
		nEngineCode = DGN_KR_BRG_RC_LSD15;
		break;
	default:
		ASSERT(0);
		break;
	}
	return nEngineCode;
}


BOOL CRptRating_RC_KSCE::Excute_Print_RCRatinig_KSCE_PLATE(int iDgnRatingCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
{
	BOOL bVersionOK =FALSE;

	CDgnEngineProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨
	D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
	pProInte->bUpdateTitle   = TRUE;
	pProInte->bUpdateMessage = TRUE;
	pProInte->bUpdatePercent = TRUE;

	// Report Result Data
	T_RCRatingResD_KSCE_Str_Rpt_D RCBeamRatingRptStrD; 
	T_RCRatingResD_KSCE_Rpt_D RCBeamRatingRptD; 

	int iDgnCode = GetCodeConvert2Engine(iDgnRatingCode);

	if(Make_Beam_RptD_RCRating_KSCE_Plate(iDgnRatingCode, RCBeamRatingRptStrD, RCBeamRatingRptD))
	{
		CString strDataPath = m_pDoc->GetPathName();
		CString strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");

		DGNEXCEL_KR_RCSC_OPTION OptionData;
		OptionData.Initialize();
		// 4. Output Data Setting(2)       
		OptionData.strPath        = strPath;;
		OptionData.iPrintOpt      = iPrintOpt;          // 0:출력, 1:insert
		OptionData.iCode          = iDgnCode;///*CDBLib::GetRatCode()*/;
		OptionData.bSaveImage     = TRUE;//SOption.bSaveImage;
		OptionData.bIsView        = TRUE;
		OptionData.bIsForceSheet  = FALSE;
		//OptionData.iReportType    = 2;//iReportType;
		RCBeamRatingRptStrD.OptD = OptionData;

		T_RPAC_D RpacD;
		if (!m_pDoc->m_pAttrCtrl2->GetRpac(RpacD)) RpacD.Initialize(iDgnRatingCode);

		CDgn_RatingRptManager myReport;
		bVersionOK = myReport.Print_RC_RFReport(FALSE, strBasePath, strDataPath, strPath, RpacD.bPrintParm, RCBeamRatingRptStrD.OptD, RCBeamRatingRptStrD.LoadD, RCBeamRatingRptStrD.arRptResD, RCBeamRatingRptD.aRptData, RCBeamRatingRptStrD.arStrInD, RCBeamRatingRptStrD.arStrResD);
	}
	else {return FALSE;}

	return bVersionOK;
};


BOOL  CRptRating_RC_KSCE::Make_Beam_RptD_RCRating_KSCE(int iDgnRatingCode, T_RCRatingResD_KSCE_Str_Rpt_D  &rStrRptD, T_RCRatingResD_KSCE_Rpt_D &rRatingRptD)
{
	rStrRptD.Initailize();
	rRatingRptD.Initailize();

	T_RPAC_D RpacD;
	if(!m_pDoc->m_pAttrCtrl2->GetRpac(RpacD)) RpacD.Initialize(iDgnRatingCode); // Rating 검토는 Initial value로 수행됨.
	CDgnRCRatingDataCtrl  RCRatingDataCtrl;
	RCRatingDataCtrl.InitialData(iDgnRatingCode);

	int iDgnCode = GetCodeConvert2Engine(iDgnRatingCode);

	// Report Data Make(Get)
	CArray<T_RPRC_K, T_RPRC_K> arPrtBeamElemKey;
	m_pDoc->m_pAttrCtrl2->GetRprcKeyList(arPrtBeamElemKey);

	int nPrtElemNum = arPrtBeamElemKey.GetSize();
	int nPrtResNum = 0;
	for(int i = 0; i < nPrtElemNum; ++i)
	{
		T_RPRC_K PrtKey  = arPrtBeamElemKey[i];
		T_ELEM_K ElemK   = PrtKey; 
		T_RPRC_D RprcD;
		if(!m_pDoc->m_pAttrCtrl2->GetRprc(PrtKey, RprcD))  continue;

		// Check IJ Rpt Option
		BOOL bPrtOpt_I =(RprcD.iPrintChk == 0 || RprcD.iPrintChk == 2);
		BOOL bPrtOpt_J =(RprcD.iPrintChk == 1 || RprcD.iPrintChk == 2);

		// Checking Exist 
		T_RFRC_D RfrcD;
		if(bPrtOpt_I || bPrtOpt_J)
		{
			if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetRCRating4TableKSCE_USD_Beam(ElemK, RfrcD)) 
				continue;
		}
		else 
		{
			continue;
		}

		BOOL bExistResult_I = (RfrcD.RfrcBase[0].bCheck || RfrcD.RfrcBase[1].bCheck);
		BOOL bExistResult_J = (RfrcD.RfrcBase[2].bCheck || RfrcD.RfrcBase[3].bCheck);
		if(!bExistResult_I && !bExistResult_J)continue;

		BOOL bPrt_I = (bPrtOpt_I && bExistResult_I) ;
		BOOL bPrt_J = (bPrtOpt_J && bExistResult_J) ;

		T_RCRatingResD_KSCE_Str_D StrResD;
		DGNEXCEL_KR_BEAM_SECT RatingResStrD4Rpt;
		RF_RES_RF_KSCE_RC_D RatingResD;

		if(bPrt_I || bPrt_J)
		{
			if(!RCRatingDataCtrl.Get_RCRatingBeamOptionD(StrResD.InD.OptionData)) continue;      
			if(!RCRatingDataCtrl.Get_RCRatingBeamSectD(ElemK, StrResD.InD.SectRCData)) continue;
			if(!RCRatingDataCtrl.Get_RCRatingBeamMatlD(ElemK, StrResD.InD.MatlRCData)) continue;
		}

		int nIJ = 0;

		if(bPrt_I)
		{
			nIJ = 0;
			if(!RCRatingDataCtrl.Get_RCRatingBeamRebarD(ElemK, StrResD.InD.SectRCData, StrResD.InD.RbarRCData, nIJ)) continue;

			T_RFRC_CASE RfrcInD_I;
			RfrcInD_I.RfrcBase[0] = RfrcD.RfrcBase[0]; //Positive-Max
			RfrcInD_I.RfrcBase[1] = RfrcD.RfrcBase[1]; //Negative-Min

			RCRatingDataCtrl.Get_RCRatingBeamLcomD(RfrcInD_I.RfrcBase[0].Rating_Force_Str, StrResD.InD.LoadData);
			RCRatingDataCtrl.Get_RCRatingBeamStrength_Print( StrResD.InD, StrResD.ResD);  // 단면 체크
			ConvertBeamStrengthResToRpt_Sect(FALSE, iDgnCode, ElemK, nIJ, TRUE, StrResD.InD, StrResD.ResD, RatingResStrD4Rpt);
			rStrRptD.arRptResD.Add(RatingResStrD4Rpt); // Positive
			rStrRptD.arStrInD.Add(StrResD.InD);
			rStrRptD.arStrResD.Add(StrResD.ResD);

			RCRatingDataCtrl.Get_RCRatingBeamLcomD(RfrcInD_I.RfrcBase[1].Rating_Force_Str, StrResD.InD.LoadData);
			if(iDgnRatingCode==KSCE_LSD15_RC_RATING) 
			{
				if(fabs(StrResD.InD.LoadData.LcomList_Str[0].dForce[2]) < cDGN_Zero) StrResD.InD.LoadData.LcomList_Str[0].dForce[2] =  cDGN_Zero*10.0;
				if(fabs(StrResD.InD.LoadData.LcomList_Str[0].dForce[4]) < cDGN_Zero) StrResD.InD.LoadData.LcomList_Str[0].dForce[4] = -cDGN_Zero*10.0;
			}
			RCRatingDataCtrl.Get_RCRatingBeamStrength_Print(StrResD.InD, StrResD.ResD);   // 단면 체크
			ConvertBeamStrengthResToRpt_Sect(FALSE, iDgnCode, ElemK, nIJ, FALSE, StrResD.InD, StrResD.ResD, RatingResStrD4Rpt);
			rStrRptD.arRptResD.Add(RatingResStrD4Rpt); // Negative
			rStrRptD.arStrInD.Add(StrResD.InD);
			rStrRptD.arStrResD.Add(StrResD.ResD);

			RatingResD.arForce.SetSize(2);
			ConvertToDLL_RF(ElemK, nIJ, 0,  RpacD, RfrcInD_I.RfrcBase[0], RatingResD);
			ConvertToDLL_RF(ElemK, nIJ, 1,  RpacD, RfrcInD_I.RfrcBase[1], RatingResD);
			rRatingRptD.aRptData.Add(RatingResD);
			nPrtResNum++;
		}

		if(bPrt_J)
		{
			nIJ = 1;
			if(!RCRatingDataCtrl.Get_RCRatingBeamRebarD(ElemK, StrResD.InD.SectRCData, StrResD.InD.RbarRCData,nIJ)) continue;

			T_RFRC_CASE RfrcInD_J;
			RfrcInD_J.RfrcBase[0] = RfrcD.RfrcBase[2]; //Positive-Max
			RfrcInD_J.RfrcBase[1] = RfrcD.RfrcBase[3]; //Negative-Min

			RCRatingDataCtrl.Get_RCRatingBeamLcomD(RfrcInD_J.RfrcBase[0].Rating_Force_Str,StrResD.InD.LoadData);
			RCRatingDataCtrl.Get_RCRatingBeamStrength_Print( StrResD.InD, StrResD.ResD);
			ConvertBeamStrengthResToRpt_Sect(FALSE, iDgnCode, ElemK, nIJ, TRUE, StrResD.InD ,StrResD.ResD, RatingResStrD4Rpt);
			rStrRptD.arRptResD.Add(RatingResStrD4Rpt); // PositIve
			rStrRptD.arStrInD.Add(StrResD.InD);
			rStrRptD.arStrResD.Add(StrResD.ResD);

			RCRatingDataCtrl.Get_RCRatingBeamLcomD(RfrcInD_J.RfrcBase[1].Rating_Force_Str,StrResD.InD.LoadData);
			if(iDgnRatingCode==KSCE_LSD15_RC_RATING) 
			{
				if(fabs(StrResD.InD.LoadData.LcomList_Str[0].dForce[2]) < cDGN_Zero) StrResD.InD.LoadData.LcomList_Str[0].dForce[2] =  cDGN_Zero*10.0;
				if(fabs(StrResD.InD.LoadData.LcomList_Str[0].dForce[4]) < cDGN_Zero) StrResD.InD.LoadData.LcomList_Str[0].dForce[4] = -cDGN_Zero*10.0;
			}
			RCRatingDataCtrl.Get_RCRatingBeamStrength_Print(StrResD.InD, StrResD.ResD);
			ConvertBeamStrengthResToRpt_Sect(FALSE, iDgnCode, ElemK, nIJ, FALSE, StrResD.InD ,StrResD.ResD, RatingResStrD4Rpt);
			rStrRptD.arRptResD.Add(RatingResStrD4Rpt); // Negative
			rStrRptD.arStrInD.Add(StrResD.InD);
			rStrRptD.arStrResD.Add(StrResD.ResD);

			RatingResD.arForce.SetSize(2);
			ConvertToDLL_RF(ElemK, nIJ, 0,  RpacD, RfrcInD_J.RfrcBase[0], RatingResD);
			ConvertToDLL_RF(ElemK, nIJ, 1,  RpacD, RfrcInD_J.RfrcBase[1], RatingResD);
			rRatingRptD.aRptData.Add(RatingResD);
			nPrtResNum++;
		}
	}

	if(nPrtResNum <1 ) 
	{
		AfxMessageBox(_T("There is no result Data!"));
		return FALSE;
	}
	else 
		return TRUE;
}



BOOL  CRptRating_RC_KSCE::Make_Beam_RptD_RCRating_KSCE_Plate(int iDgnRatingCode, T_RCRatingResD_KSCE_Str_Rpt_D  &rStrRptD, T_RCRatingResD_KSCE_Rpt_D &rRatingRptD)
{
	rStrRptD.Initailize();
	rRatingRptD.Initailize();

	T_RPAC_D RpacD;
	if(!m_pDoc->m_pAttrCtrl2->GetRpac(RpacD)) RpacD.Initialize(iDgnRatingCode);
	CDgnRCRatingDataCtrl  RCRatingDataCtrl;
	RCRatingDataCtrl.InitialData(iDgnRatingCode);

	int iDgnCode = GetCodeConvert2Engine(iDgnRatingCode);

	// Report Data Make(Get)
	CArray<T_RPRP_K, T_RPRP_K> arPrtPlateMembKey;  //  plate는 여기 key값 만큼 계산 및 출력한다.
	m_pDoc->m_pAttrCtrl2->GetRprpKeyList(arPrtPlateMembKey);

	int nPrtElemNum = arPrtPlateMembKey.GetSize();
	int nPrtResNum = 0;
	for(int i = 0; i < nPrtElemNum; ++i)
	{
		T_RPRP_K PrtKey  = arPrtPlateMembKey[i];
		T_ELEM_K ElemK   = PrtKey; 
		T_RPRP_D RprpD;
		if(!m_pDoc->m_pAttrCtrl2->GetRprp(PrtKey, RprpD))  continue;

		// Check IJ Rpt Option
		BOOL bPrtOpt = (RprpD.nPrintChk == 1);

		// Checking Exist 
		T_RFRP_D RfrpD;
		if(bPrtOpt)
		{
			if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetRCRating4TableKSCE_USD_Plate(ElemK, RfrpD))
				continue;
		}
		else 
		{
			continue;
		}

		BOOL bExistResult = (RfrpD.RfrpBase[0].bCheck || RfrpD.RfrpBase[1].bCheck);
		if(!bExistResult)continue;

		BOOL bPrt = (bPrtOpt && bExistResult) ;

		T_RCRatingResD_KSCE_Str_D StrResD;
		DGNEXCEL_KR_BEAM_SECT RatingResStrD4Rpt;
		RF_RES_RF_KSCE_RC_D RatingResD;

		T_RRPL_D RrplD;
		if(!m_pDoc->m_pAttrCtrl2->GetRrpl(PrtKey, RrplD)) {ASSERT(0); continue;}
		CStringW strName(RrplD.strName);

		int nIJ = 0;

		if(bPrt)
		{
			if(!RCRatingDataCtrl.Get_RCRatingBeamOptionD(StrResD.InD.OptionData)) continue;   // option
			if(!RCRatingDataCtrl.ConvertRCRatingPlateInputData(RrplD, StrResD.InD)) continue; // sect, rebar, matl.

			T_RFRP_CASE RfrcInD_I;
			RfrcInD_I.RfrpBase[0] = RfrpD.RfrpBase[0]; //Positive-Max
			RfrcInD_I.RfrpBase[1] = RfrpD.RfrpBase[1]; //Negative-Min

			RCRatingDataCtrl.Get_RCRatingBeamLcomD(RfrcInD_I.RfrpBase[0].Rating_Force_Str,StrResD.InD.LoadData);
			RCRatingDataCtrl.Get_RCRatingBeamStrength_Print( StrResD.InD, StrResD.ResD);
			ConvertBeamStrengthResToRpt_Sect(TRUE, iDgnCode, ElemK, nIJ, TRUE,  StrResD.InD, StrResD.ResD, RatingResStrD4Rpt);
			RatingResStrD4Rpt.strName = strName;
			rStrRptD.arRptResD.Add(RatingResStrD4Rpt); // Positive
			rStrRptD.arStrInD.Add(StrResD.InD);
			rStrRptD.arStrResD.Add(StrResD.ResD);

			RCRatingDataCtrl.Get_RCRatingBeamLcomD(RfrcInD_I.RfrpBase[1].Rating_Force_Str,StrResD.InD.LoadData);
			if(iDgnRatingCode==KSCE_LSD15_RC_RATING) 
			{
				if(fabs(StrResD.InD.LoadData.LcomList_Str[0].dForce[2]) < cDGN_Zero) StrResD.InD.LoadData.LcomList_Str[0].dForce[2] =  cDGN_Zero*10.0;
				if(fabs(StrResD.InD.LoadData.LcomList_Str[0].dForce[4]) < cDGN_Zero) StrResD.InD.LoadData.LcomList_Str[0].dForce[4] = -cDGN_Zero*10.0;
			}
			RCRatingDataCtrl.Get_RCRatingBeamStrength_Print(StrResD.InD, StrResD.ResD);
			ConvertBeamStrengthResToRpt_Sect(TRUE, iDgnCode, ElemK, nIJ, FALSE, StrResD.InD, StrResD.ResD, RatingResStrD4Rpt);
			RatingResStrD4Rpt.strName = strName;
			rStrRptD.arRptResD.Add(RatingResStrD4Rpt); // Negative
			rStrRptD.arStrInD.Add(StrResD.InD);
			rStrRptD.arStrResD.Add(StrResD.ResD);

			RatingResD.strName = strName;
			RatingResD.arForce.SetSize(2);
			ConvertToDLL_RF(ElemK, nIJ, 0, RpacD, RfrcInD_I.RfrpBase[0], RatingResD);
			ConvertToDLL_RF(ElemK, nIJ, 1,  RpacD,RfrcInD_I.RfrpBase[1], RatingResD);
			rRatingRptD.aRptData.Add(RatingResD);
			nPrtResNum++;
		}
	}

	if(nPrtResNum <1 ) 
	{
		AfxMessageBox(_T("There is no result Data!"));
		return FALSE;
	}
	else 
		return TRUE;
}


BOOL  CRptRating_RC_KSCE::Print_RptD_RCRating_KSCE()
{
	return TRUE;
}

BOOL  CRptRating_RC_KSCE::Print_RptD_RCRating_KSCE_Strength()
{
	return TRUE;
}

BOOL  CRptRating_RC_KSCE::Print_RptD_RCRating_KSCE_Rating()
{
	return TRUE;
}

CStringW CRptRating_RC_KSCE::GetBridgeLevel(const int  &nRank)
{
	CString strTemp = _T("");
	switch (nRank)
	{
	case ENUM_LEVEL_A: {strTemp = _T("A"); break;}
	case ENUM_LEVEL_B: {strTemp = _T("B"); break;}
	case ENUM_LEVEL_C: {strTemp = _T("C"); break;}
	case ENUM_LEVEL_D: {strTemp = _T("D"); break;}
	case ENUM_LEVEL_E: {strTemp = _T("E"); break;}
	default:{ASSERT(0); break;}
	}

	CStringW strWTemp(strTemp);
	return strWTemp;
}

BOOL CRptRating_RC_KSCE::ConvertToDLL_RptForce(T_RATI_UNITLCOM &Force,  RF_RES_KS_FORCE_KSCE_ASD_D&rForce)
{
	if (&Force == NULL) return FALSE;
	rForce.bUse = Force.bUse;
	rForce.nType = Force.nType;
	rForce.nOrgLkey = Force.nOrgLkey;

	CStringW strWName(Force.strName);
	CStringW strWDesc(Force.strDesc);
	rForce.strName = strWName;
	rForce.strDesc = strWDesc;
	for (int i = 0; i<6; ++i)
	{
		rForce.dForce[i] = Force.dForce[i];
	}

	return TRUE;
}

CString CRptRating_RC_KSCE::GetSaveFileDirEx()
{
	CString strFileName=m_pDoc->GetPathName();

	CFileCtrl FileCtrl(strFileName);

	return FileCtrl.GetFilePath();
}

CString CRptRating_RC_KSCE::GetSaveFileNameEx(BOOL bWithoutExtension)
{
	CString strFileName=m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strFileName);

	CString strSaveFileName;  
	strSaveFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".xls");

	CFileCtrl* saveFileCtrl = new CFileCtrl(strSaveFileName);

	int count  = 0;
	while(saveFileCtrl->FileExists())
	{
		CString sCount;
		sCount.Format(_T("%d"), ++count);

		CString strSaveFileName;
		strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + sCount + _T(".xls");

		delete saveFileCtrl;
		saveFileCtrl = new CFileCtrl(strSaveFileName);
	}

	if(bWithoutExtension) 
		strSaveFileName = saveFileCtrl->GetFileNameWithoutExtension();
	else
		strSaveFileName = saveFileCtrl->GetFilePathAndName();

	delete saveFileCtrl;

	return strSaveFileName;
}

BOOL CRptRating_RC_KSCE::ConvertBeamStrengthResToRpt(const T_RCRatingResD_KSCE_Str_D &crStrResD, T_RCRatingResD_KSCE_Str_Rpt_D &rStrRptD)
{
	// Convert OptD
	rStrRptD.OptD = crStrResD.OptD;

	// Convert LoadD
	if(!ConvertBeamStrengthResToRpt_Load(crStrResD.InD.LoadData, rStrRptD.LoadD)) return FALSE;

	// Convert SectD ??
	for(int i = 0; i < 1; ++i)
	{
		// Do Convert InD and OutD
	}

	return TRUE;
}

BOOL CRptRating_RC_KSCE::ConvertBeamStrengthResToRpt_Load(const _UMD_RC_KR_BEAM_LOAD &crStrResLoadD, D_LCOM_INFO &rStrRptLoadD)
{  
	rStrRptLoadD.Initialize();

	// Strength Only
	int nLcomStrNum =     crStrResLoadD.LcomList_Str.GetSize();
	rStrRptLoadD.arLcomStrn.SetSize(nLcomStrNum);
	rStrRptLoadD.arLcaseName.SetSize(nLcomStrNum);
	rStrRptLoadD.arLcaseDesc.SetSize(nLcomStrNum);

	for(int i = 0; i < nLcomStrNum; ++i)
	{
		rStrRptLoadD.arLcomStrn[i].strName = crStrResLoadD.LcomList_Str[i].strName; 
		rStrRptLoadD.arLcomStrn[i].strDesc = _T("");   // 우선은 공백

		rStrRptLoadD.arLcaseName[i] =crStrResLoadD.LcomList_Str[i].strName; 
		rStrRptLoadD.arLcaseDesc[i] = _T("");  // 우선은 공백
	}

	return TRUE;
}

double CRptRating_RC_KSCE::Get_epsi_alw_min(double dFyr, double dEsr)
{ // CDgnCalc_KR_RCSC_Beam_KCI03::Get_epsi_alw_min 참고

	//int iCode = iDgnCode;
	double depsi_a=0.0;
	//   if(dFyr<=-1.0) dFyr = m_dFyr;
	//   if(dEsr<=-1.0) dEsr = m_dEsr;

	//if(iCode == DGN_KCI_USD07 || iDgnCode==DGN_KSCE_RC_USD10 || iDgnCode==DGN_KSCE_RC_RAIL_USD11 || iDgnCode==DGN_KCI_USD12)
	{
		if(dFyr<400.0+cDGN_Zero)
			depsi_a = 0.004;
		else 
		{
			double depsi = dEsr==0.0 ? 0.0 : dFyr/dEsr;
			depsi_a = 2.0*depsi;
		}
	}

	return depsi_a;
}


BOOL CRptRating_RC_KSCE::Get_RebaLayerInfo(_UMD_RC_KR_BEAM_RBAR& rData, double* dAsLayerTop, double* dAsLayerBot)
{  
	_UMD_RC_BEAM_REBAR_UNIT Rebar;

	int i,j;    
	for(i=0; i<2; i++)
	{
		BOOL bTop = i==0 ? TRUE : FALSE;
		for(j=0; j<3; j++)  // 현재는 3단만 고려?
		{   
			Rebar.Initialize();
			Rebar = (bTop)? rData.arTopBar.GetAt(j) : rData.arBotBar.GetAt(j);
			if(Rebar.dNum * Rebar.dDt ==0.0) continue;

			double dAsStory= Rebar.dAs_row;

			if(bTop) dAsLayerTop[j] = dAsStory;
			else     dAsLayerBot[j] = dAsStory;
		}
	}
	return TRUE;
}

BOOL CRptRating_RC_KSCE::Get_RhoLimit_Data(int iDgnCode, double dFck, double dFyr, double decu, double& dMaxRho, double& dRhob, double& dMinRho1, double& dMinRho2, double& dMinRho, double dEsr, double dDeff, double dDtmax)
{
	// CDgnCalc_KR_RCSC_Beam_KCI03::Get_RhoLimit_Data 참고

	//        기준 단위계의 값으로 입력을 받아야 함	
	double dBeta1 = Calc_Beta(iDgnCode, dFck);
	double dAlpha = Calc_Alpha(iDgnCode, dFck);
	double dFc	  = dFck;
	double dFy	  = dFyr;

	//   if(iDgnCode==DGN_KSCE_RC_USD05 || iDgnCode==DGN_KSCE_RC_RAIL_USD04)
	//   {
	//     dRhob = dAlpha*dBeta1*(dFc/dFy)*(decu/(dFy/dEsr+decu));
	//     dMaxRho = 0.75*dRhob;
	//     dMinRho1 = 0.25*sqrt(dFc)/dFy;
	//     dMinRho2 = 1.4/dFy;
	//     dMinRho  = max(dMinRho1,dMinRho2);
	//   }
	if(iDgnCode==DGN_KCI_USD07 || iDgnCode==DGN_KSCE_RC_USD10 || iDgnCode==DGN_KSCE_RC_RAIL_USD11 || iDgnCode==DGN_KCI_USD12)
	{
		dRhob = dFy < cDGN_Zero ? 0.0 : dAlpha*dBeta1*(dFc/dFy)*(decu/(dFy/dEsr+decu));
		if(dEsr > 0.0 && dDeff > 0.0 && dDtmax > 0.0)
			dMaxRho = Get_Rho_AllowRatioRbar_Rect(iDgnCode, dFck, dFyr, decu, dEsr, dDeff, dDtmax);
		else 
			dMaxRho = 0.75*dRhob;
		dMinRho1 = dFy < cDGN_Zero ? 0.0 : 0.25*sqrt(dFc)/dFy;
		dMinRho2 = dFy < cDGN_Zero ? 0.0 : 1.4/dFy;
		dMinRho  = max(dMinRho1,dMinRho2);
	}
	else
	{
		dRhob=dMinRho1=dMinRho2=dMinRho=0.0;
		return FALSE;
	}

	return TRUE;
}

double CRptRating_RC_KSCE::Get_Rho_AllowRatioRbar_Rect(int iDgnCode, double dFc, double dFyr, double decu, double dEsr, double dDeff, double dDtmax)
{
	// ※주의:이함수에서는 입력 받은 변수만으로 결과값을 계산하여야함
	int    iCode  = iDgnCode;
	double dBeta1 = Calc_Beta(iDgnCode, dFc);
	double dAlpha = Calc_Alpha(iDgnCode, dFc);
	double depsic = decu;
	double dAllowRatioRbar = Get_epsi_alw_min(dFyr, dEsr);

	// 최소허용변형률 이상이 발생되는 최대철근비
	double dRhoAllow = 0.0;
	//if(iCode==DGN_KCI_USD07 || iCode==DGN_KSCE_RC_USD10 ||  iCode==DGN_KSCE_RC_RAIL_USD11 || iCode==DGN_KCI_USD12)
	{
		if(dFyr==0.0 || (depsic+dAllowRatioRbar)==0.0 || dDeff==0.0) 
			return 0.0;
		else 
			dRhoAllow = dFyr < cDGN_Zero ? 0.0 : dAlpha*dBeta1*(dFc/dFyr)*(depsic/(depsic+dAllowRatioRbar));// * dDtmax/dDeff;
	}

	return dRhoAllow;
}


double CRptRating_RC_KSCE::Calc_Beta(int iDgnCode, double dFck)
{
	// CDgnCalcBase_RCDesign_Base::Calc_Beta(int iDgnCode, double dFck) 참고

	//   case DGN_KCI_USD07:
	//   case DGN_KSCE_RC_USD10:
	//   case DGN_KSCE_RC_RAIL_USD11:

	double dBeta1 =  (dFck<=  28.0 ? 0.85 : max(0.85-0.007*(dFck-28.0)/1.0, 0.65));

	return dBeta1;
}

double CRptRating_RC_KSCE::Calc_Alpha(int iDgnCode,  double dFck)
{
	//   case DGN_KSCE_RC_USD10:
	//   case DGN_KSCE_RC_RAIL_USD11:

	double dAlpha1 = 0.85;
	return dAlpha1;
}

double CRptRating_RC_KSCE::Get_RebarStrain_Rect(int iDgnCode, double decu, double dBc, double dDt, double dFc, double dFyr, double dAst)
{
	// CDgnCalc_KR_RCSC_Beam_KCI03::Get_RebarStrain_Rect 참고
	int    iCode  = iDgnCode;
	double dBeta1 = Calc_Beta(iCode, dFc);
	double dAlpha = Calc_Alpha(iCode, dFc);
	double dCalcC = fabs(dAlpha*dFc*dBc*dBeta1) < cDGN_Zero ? 0.0 : dFyr*dAst/(dAlpha*dFc*dBc*dBeta1); // c = a / beta1
	double depsic = decu;

	double des = 0.0;
	if(iCode==DGN_KCI_USD07 || iDgnCode==DGN_KSCE_RC_USD10 || iDgnCode==DGN_KSCE_RC_RAIL_USD11 || iDgnCode==DGN_KCI_USD12)
	{
		des = (dCalcC==0.0)? 0.0 : depsic*(dDt-dCalcC)/dCalcC;
	}
	return des;
}


BOOL CRptRating_RC_KSCE::ConvertBeamStrengthResToRpt_Sect(BOOL bPlate, int iDgnCode, T_ELEM_K ElemK, int nIJ,  BOOL bPositive, const _UMD_RC_BEAM_KCI &ChkInD, const _UMD_RC_KR_BEAM_RES &ChkResD, DGNEXCEL_KR_BEAM_SECT &ExcelSect)
{

	// Convert General Info.
	ExcelSect.strUMDName = _T(""); // 부재의 UMD를 대표하는 이름 ???
	ExcelSect.strName    = _T("");
	ExcelSect.iElemID    = ElemK;  // Element의 ID

	int nChkPos = 0;
	switch (nIJ)
	{
	case 0: nChkPos = 0; break;
	case 1: nChkPos = 2; break;
	default: ASSERT(0);  break;
	}
	ExcelSect.iCheckPos = nChkPos; // 검토위치 (1:I-End, 2:Mid, 3:J-End)
	ExcelSect.iMembType = 1;       // 1:Beam, 2:Slab
	ExcelSect.iAnalType = ChkInD.OptionData.iAnalType;  // 부재 설계 형식 Type(0=단철근보, 1=복철근보, 2=DeepBeam, 3=Corbel)
	ExcelSect.iCrackType       = 0; // 균열검토조건 (0:Dry, 1:Humidity, 2:Corrosion, 3:High Corrosion)
	ExcelSect.bConsiderStirrup = ChkInD.OptionData.bStirrup;   // 균열검토시, tc 계산에 전단철근 고려여부
	ExcelSect.strRemark = _T("");

	// Convert Material Info.
	ExcelSect.dFck       = ChkInD.MatlRCData.dFc;
	ExcelSect.dFy        = ChkInD.MatlRCData.dFyr;
	ExcelSect.dFys       = ChkInD.MatlRCData.dFys;
	ExcelSect.dEc        = ChkInD.MatlRCData.dEc;
	ExcelSect.dEs        = ChkInD.MatlRCData.dEsr;

	// Convert Section Info.
	double  dH = ChkInD.SectRCData.dSize[0];
	double  dB = ChkInD.SectRCData.dSize[1];
	ExcelSect.dH = dH;
	ExcelSect.dB = dB;

	double dAlpha             = Calc_Alpha(iDgnCode, ExcelSect.dFck);
	double dBeta1             = Calc_Beta(iDgnCode, ExcelSect.dFck);
	ExcelSect.dBeta1          = dBeta1;  
	ExcelSect.dPhiF           = ChkInD.OptionData.dPhi[0];
	ExcelSect.dPhiS           = ChkInD.OptionData.dPhi[4];
	ExcelSect.dAllowRatioRbar = Get_epsi_alw_min(ExcelSect.dFy, ExcelSect.dEs );
	double decu               = ChkInD.MatlRCData.decu; //  0.003
	ExcelSect.dEpsCon_UL      = decu;
	ExcelSect.bShearChk       = ChkInD.OptionData.bCalcShear;

	// 경량콘크리트 정보
	ExcelSect.bLambda =  ChkInD.OptionData.bLambda;
	ExcelSect.dLambda = (ChkInD.OptionData.bLambda)? ChkInD.OptionData.dLambda : 1.0;
	ExcelSect.dKcr    = ChkResD.ElemRes.dKcr;

	ExcelSect.bSubBarAssigned  = (ChkInD.RbarRCData.dSubBarArea > 0.0);
	ExcelSect.bHSubBarAssigned = (ChkInD.RbarRCData.dSubBarArea_H > 0.0);

	CString strSubBarNa       = ChkInD.RbarRCData.strSubBarNa;
	if(ExcelSect.dFys>350.0) { strSubBarNa.Replace(_T("D"), _T("H")); }
	CStringW strwSubBarNa(strSubBarNa);
	ExcelSect.stirrup.strName = strwSubBarNa;
	ExcelSect.stirrup.dSpace  = ChkInD.RbarRCData.dSubBarDist;
	ExcelSect.stirrup.dNum[0] = ChkInD.RbarRCData.dSubBarNum;

	// 수평철근(Only DeepBeam)
	CString strSubBarNa_H       = ChkInD.RbarRCData.strSubBarNa_H;
	if(ExcelSect.dFys>350.0) { strSubBarNa_H.Replace(_T("D"), _T("H")); }
	CStringW strwSubBarNa_H(strSubBarNa_H);
	ExcelSect.stirrup_H.strName = strwSubBarNa_H;
	ExcelSect.stirrup_H.dSpace  = ChkInD.RbarRCData.dSubBarDist_H;
	ExcelSect.stirrup_H.dNum[0] = ChkInD.RbarRCData.dSubBarNum_H;
	ExcelSect.dLn               = ChkInD.dLn;

	ExcelSect.bDgnLoad = ChkInD.LoadData.bDgnLoad;

	double dMy = ChkInD.LoadData.LcomList_Str[0].dForce[4]; //dMy
	double dVz = ChkInD.LoadData.LcomList_Str[0].dForce[2]; //dVz
	BOOL bTopDir=FALSE;
	BOOL bTopDir_Shear =FALSE;
	//BOOL bIsMainData, bIsSubData, bTop_MainDir, bTop_SubDir, bTop_MainDir_Shear, bTop_SubDir_Shear, bTop_MainDir_Ser, bTop_SubDir_Ser;
	//Check_IsEXData(UrbeD, SortData, bIsMainData, bIsSubData, bTop_MainDir, bTop_SubDir, bTop_MainDir_Shear, bTop_SubDir_Shear, bTop_MainDir_Ser, bTop_SubDir_Ser);
	//   if(ChkInD.OptionData.iMomType == 0) // ??
	//   {
	//     rStrRptSectD.strName_Mumax = UrbeD.Load.strName_Mumax[2];
	//     rStrRptSectD.dUserMumax    = UrbeD.Load.dMumax[2];       
	//     rStrRptSectD.strName_Vumax = UrbeD.Load.strName_Vumax[2];
	//     rStrRptSectD.dUserVumax    = UrbeD.Load.dVumax[2];       
	//     rStrRptSectD.bSignMu_Vumax = UrbeD.Load.bSignMu_Vumax;   
	//     rStrRptSectD.strName_Msmax = UrbeD.Load.strName_Msmax[2];
	//     rStrRptSectD.dUserMsmax    = UrbeD.Load.dMsmax[2];    
	//   }
	//   else 
	{
		if(!bPositive) //bTop_MainDir
		{// 상단철근 : 역방향모멘트
			ExcelSect.strName_Mumax = _T("");//UrbeD.Load.strName_Mumax[1];
			ExcelSect.dUserMumax    = dMy;   //UrbeD.Load.dMumax[1];      
			bTopDir = TRUE;
		}
		else
		{// 하단철근 : 정방향모멘트
			ExcelSect.strName_Mumax = _T("");//UrbeD.Load.strName_Mumax[0];
			ExcelSect.dUserMumax    = dMy;   //UrbeD.Load.dMumax[0]; 
			bTopDir = FALSE;
		}

		if(!bPositive)
		{// 상단철근 : 전단검토시 역방향모멘트
			ExcelSect.strName_Vumax = _T("");//UrbeD.Load.strName_Vumax[1];
			ExcelSect.dUserVumax    = dVz;   //UrbeD.Load.dVumax[1];       
			ExcelSect.bSignMu_Vumax = FALSE;  
			bTopDir_Shear = TRUE;
		}
		else
		{// 하단철근 : 전단검토시 정방향모멘트
			ExcelSect.strName_Vumax = _T("");//UrbeD.Load.strName_Vumax[0];
			ExcelSect.dUserVumax    = dVz;   //UrbeD.Load.dVumax[0];       
			ExcelSect.bSignMu_Vumax = TRUE;   
			bTopDir_Shear = FALSE;
		}

		//     if(bTop_MainDir_Ser)
		//     { 
		//       rStrRptSectD.strName_Msmax = UrbeD.Load.strName_Msmax[1];
		//       rStrRptSectD.dUserMsmax    = UrbeD.Load.dMsmax[1];    
		//     }
		//     else
		//     {
		//       rStrRptSectD.strName_Msmax = UrbeD.Load.strName_Msmax[0];
		//       rStrRptSectD.dUserMsmax    = UrbeD.Load.dMsmax[0];   
		//      }
	}


	int k, nSize;
	DGNEXCEL_KR_LCOM_STRN ExUnitLcom_Strn;
	DGNEXCEL_KR_LCOM_SERV ExUnitLcom_Serv;
	//for(i=0 ; i</*2*/1 ; i++)     // 
	//{
	//if(UrbeD.Option.iMomType == 3)  // 해석부재력 결정시(0=Mu절대값최대, 1=Mu정방향최대, 2=Mu역방향최대,3=Mu정방향,역방향최대)
	{
		CString strName; 
		if(bPlate) strName.Format(_T("Plate%d_"), ElemK);
		else       strName.Format(_T("Elem%d_"), ElemK);
		CStringW strwName(strName);

		if(!bTopDir) 
		{
			ExcelSect.strUMDName = strwName + _T("정모멘트");    // 부재의 UMD를 대표하는 이름

			ExcelSect.strName_Mumax = _T("정모멘트"); //UrbeD.Load.strName_Mumax[0];
			ExcelSect.dUserMumax    = dMy;           //UrbeD.Load.dMumax[0];
		}
		else     
		{
			ExcelSect.strUMDName = strwName + _T("부모멘트");    // 부재의 UMD를 대표하는 이름

			ExcelSect.strName_Mumax = _T("부모멘트");  //UrbeD.Load.strName_Mumax[1];
			ExcelSect.dUserMumax    = dMy; //UrbeD.Load.dMumax[1];  
		}

		if(!bTopDir_Shear) 
		{				       
			ExcelSect.strName_Vumax = _T("전단력"); //UrbeD.Load.strName_Vumax[0];
			ExcelSect.dUserVumax    = dVz;          //UrbeD.Load.dVumax[0];       
			ExcelSect.bSignMu_Vumax = TRUE;   
		}
		else
		{     
			ExcelSect.strName_Vumax = _T("전단력"); //UrbeD.Load.strName_Vumax[1];
			ExcelSect.dUserVumax    = dVz;          //UrbeD.Load.dVumax[1];       
			ExcelSect.bSignMu_Vumax = FALSE;   				
		}

		//       if(!bTopDir_Ser) 
		//       {
		//         ExcelSect.strName_Msmax = UrbeD.Load.strName_Msmax[0];
		//         ExcelSect.dUserMsmax    = UrbeD.Load.dMsmax[0];   
		//       }
		//       else
		//       {
		//         ExcelSect.strName_Msmax = UrbeD.Load.strName_Msmax[1];
		//         ExcelSect.dUserMsmax    = UrbeD.Load.dMsmax[1];    
		//       }
	}

	// Rebar 
	double  dAsLayerTop[3];
	double  dAsLayerBot[3];
	_UMD_RC_KR_BEAM_RBAR RbarRCData = ChkInD.RbarRCData;
	Get_RebaLayerInfo(RbarRCData, dAsLayerTop, dAsLayerBot); 

	_UMD_RC_KR_LCOM UmdUnitLcom;
	ExcelSect.arLcomStrn.RemoveAll();
	ExcelSect.arLcomServ.RemoveAll();
	if(ExcelSect.bDgnLoad)
	{
		nSize = 1; //ChkInD.LoadData.LcomList_Str[0]   // UrbeD.Load.StrnLcomList.GetSize();
		for(k = 0 ; k<nSize ; k++)
		{
			UmdUnitLcom = ChkInD.LoadData.LcomList_Str[0];
			if(!UmdUnitLcom.bUse || (bTopDir ? UmdUnitLcom.dForce[4] >= 0.0 : UmdUnitLcom.dForce[4] < 0.0)) continue; 

			ExUnitLcom_Strn.iDgnLcomK = k+1;
			ExUnitLcom_Strn.strName   = UmdUnitLcom.strName;
			ExUnitLcom_Strn.dPu       = UmdUnitLcom.dForce[0];
			ExUnitLcom_Strn.dMu       = UmdUnitLcom.dForce[4];
			ExUnitLcom_Strn.dVu       = UmdUnitLcom.dForce[2];
			ExcelSect.arLcomStrn.Add(ExUnitLcom_Strn);
		}
	}		

	// _UMD_RC_BEAM_LCOM_RES_KCI StrnLcomData = ChkResD.arStrnLcomData.GetAt(0);
	_UMD_RC_BEAM_LCOM_RES_KCI SortData     = ChkResD.arStrnLcomData.GetAt(0);

	if(bTopDir) 
	{
		ExcelSect.bMainBarAssigned = (SortData.dTopAsChk > 0.0); //  (RbarRCData.dAst > 0.0);
		ExcelSect.iAsRes           = SortData.iAsResN;  
		ExcelSect.iAsResVz         = SortData.iAsResV; 

		ExcelSect.dMu    =  SortData.dMuN; // SortData.dMuN;
		ExcelSect.dVu    =  SortData.dVu;  //SortData.dVu_N;
		ExcelSect.dD     = dH - (ChkResD.ddt == 0.0 ? (1.0-0.875)*dH : ChkResD.ddt);
		ExcelSect.dReqD  = dH - RbarRCData.dDtDgn;
		ExcelSect.dY     = ChkResD.ddt;

		ExcelSect.dReqAs    =  SortData.dTopAsReq; 
		ExcelSect.dAppReqAs =  SortData.dTopAsApp; 

		_UMD_RC_BEAM_REBAR_UNIT Rbar; Rbar.Initialize();
		if(RbarRCData.arTopBar.GetSize()>0) Rbar = RbarRCData.arTopBar.GetAt(0);
		double dDcmax = dH - Rbar.dDt;  
		double dMaxRho=0., dRhob=0., dMinRho1=0., dMinRho2=0., dMinRho=0.;
		Get_RhoLimit_Data(iDgnCode, ExcelSect.dFck, ExcelSect.dFy, decu, dMaxRho, dRhob, dMinRho1, dMinRho2 ,dMinRho, ExcelSect.dEs, ExcelSect.dD, dDcmax);
		ExcelSect.dDcStr  = Rbar.dDt;		

		ExcelSect.dRhoB   = dRhob;  
		ExcelSect.dMaxAs  = dMaxRho  * dB * ExcelSect.dD;
		ExcelSect.dMinAs1 = dMinRho1 * dB * ExcelSect.dD;
		ExcelSect.dMinAs2 = dMinRho2 * dB * ExcelSect.dD;
		ExcelSect.dMinAs  = dMinRho  * dB * ExcelSect.dD;
		ExcelSect.dUseAs  = SortData.dTopAsChk;
		ExcelSect.dUseA   = fabs(dAlpha * ExcelSect.dFck * dB) < cDGN_Zero? 0.0 : SortData.dTopAsChk * ExcelSect.dFys/(dAlpha * ExcelSect.dFck * dB); //!/
		ExcelSect.dUseC   = fabs(dBeta1) < cDGN_Zero? 0.0 : ExcelSect.dUseA / dBeta1;
		ExcelSect.dPhi_c_req   = SortData.dPhi_c_req_N;
		ExcelSect.dPhi_c_cal   = SortData.dPhi_c_cal_N;
		ExcelSect.da_req       = SortData.da_req_N;
		ExcelSect.dEpsilon_req = SortData.dEpsilon_req_N;
		ExcelSect.dfs_req      = SortData.dfs_req_N;
		ExcelSect.da_cal       = SortData.da_cal_N;
		ExcelSect.dEpsilon_cal = SortData.dEpsilon_cal_N;
		ExcelSect.dfs_cal      = SortData.dfs_cal_N;

		ExcelSect.dPhiMn = SortData.dpMnNChk;

		ExcelSect.dRhoKci  = dMaxRho;
		ExcelSect.dRhoFct  = fabs(dRhob) < cDGN_Zero ? 0.0 : dMaxRho/dRhob;
		ExcelSect.dEpsRbar = Get_RebarStrain_Rect(iDgnCode, decu, dB, dDcmax, ExcelSect.dFck, ExcelSect.dFy, ExcelSect.dUseAs);

		ExcelSect.arMainBar.RemoveAll();
		for(k=0; k<3; k++) // Rebar Layer.
		{
			DGNEXCEL_KR_MBAR_BEAM MbarBeam;
			MbarBeam.Initialize();
			// Change by ZINU.('06.03.05). CpbeD.bNum -> m_pMyDB->Is_RebarNum().
			MbarBeam.iArrangeType = (RbarRCData.bNum ? 0 : 1);

			Rbar = RbarRCData.arTopBar.GetAt(k);

			if(Rbar.dDt <=0.0) continue;
			CString strName1 = (Rbar.sBarNa1);
			CString strName2 = (Rbar.sBarNa2);
			CStringW strwName1(strName1);
			CStringW strwName2(strName2);
			if(ExcelSect.dFy>350.0)
			{
				strName1.Replace(_T("D"), _T("H"));
				strName2.Replace(_T("D"), _T("H"));
			}
			MbarBeam.strName1 = strwName1;
			MbarBeam.strName2 = strwName2;
			MbarBeam.dSpace   = Rbar.dCTC;
			MbarBeam.dNum     = Rbar.dNum;
			MbarBeam.dDc      = Rbar.dDt;
			MbarBeam.dAs      = Rbar.dAs_row; //dAsLayerTop[k];    
			ExcelSect.arMainBar.Add(MbarBeam);        
		}		
	}
	else             
	{ 
		ExcelSect.bMainBarAssigned = (SortData.dBotAsChk > 0.0);	
		ExcelSect.iAsRes           = SortData.iAsResP;// SortData.iAsRes;			
		ExcelSect.iAsResVz         = SortData.iAsResV; 

		ExcelSect.dMu     = SortData.dMuP;
		ExcelSect.dD      = dH - (ChkResD.ddb == 0.0 ? (1.0-0.875)*dH : ChkResD.ddb);
		ExcelSect.dReqD   = dH - RbarRCData.dDbDgn;
		ExcelSect.dY      = ChkResD.ddb;

		ExcelSect.dReqAs   = SortData.dBotAsReq;
		ExcelSect.dAppReqAs = SortData.dBotAsApp;

		//
		_UMD_RC_BEAM_REBAR_UNIT Rbar; Rbar.Initialize();
		if(RbarRCData.arBotBar.GetSize()>0) Rbar = RbarRCData.arBotBar.GetAt(0);
		double dDcmax = dH - Rbar.dDt;  
		double dMaxRho=0., dRhob=0., dMinRho1=0., dMinRho2=0., dMinRho=0.;
		Get_RhoLimit_Data(iDgnCode, ExcelSect.dFck, ExcelSect.dFy, decu, dMaxRho, dRhob, dMinRho1, dMinRho2 ,dMinRho, ExcelSect.dEs, ExcelSect.dD, dDcmax);
		ExcelSect.dDcStr  = Rbar.dDt;		

		ExcelSect.dRhoB   = dRhob;  
		ExcelSect.dMaxAs  = dMaxRho  * dB * ExcelSect.dD;
		ExcelSect.dMinAs1 = dMinRho1 * dB * ExcelSect.dD;
		ExcelSect.dMinAs2 = dMinRho2 * dB * ExcelSect.dD;
		ExcelSect.dMinAs  = dMinRho  * dB * ExcelSect.dD;
		ExcelSect.dUseAs  = SortData.dBotAsChk;
		ExcelSect.dUseA   = fabs(dAlpha * ExcelSect.dFck * dB) < cDGN_Zero? 0.0 : SortData.dBotAsChk * ExcelSect.dFys/(dAlpha * ExcelSect.dFck * dB);
		ExcelSect.dUseC   = fabs(dBeta1) < cDGN_Zero? 0.0 : ExcelSect.dUseA / dBeta1;
		ExcelSect.dPhi_c_req   = SortData.dPhi_c_req;
		ExcelSect.dPhi_c_cal   = SortData.dPhi_c_cal;
		ExcelSect.da_req       = SortData.da_req;
		ExcelSect.dEpsilon_req = SortData.dEpsilon_req;
		ExcelSect.dfs_req      = SortData.dfs_req;
		ExcelSect.da_cal       = SortData.da_cal;
		ExcelSect.dEpsilon_cal = SortData.dEpsilon_cal;
		ExcelSect.dfs_cal      = SortData.dfs_cal;

		ExcelSect.dPhiMn = SortData.dpMnPChk;			

		ExcelSect.dRhoKci  = dMaxRho;
		ExcelSect.dRhoFct  = fabs(dRhob) < cDGN_Zero ? 0.0 : dMaxRho/dRhob;
		ExcelSect.dEpsRbar = Get_RebarStrain_Rect(iDgnCode, decu, dB, dDcmax, ExcelSect.dFck, ExcelSect.dFy, ExcelSect.dUseAs);

		ExcelSect.arMainBar.RemoveAll();
		for(k=0; k<3; k++) // Rebar Layer.
		{
			DGNEXCEL_KR_MBAR_BEAM MbarBeam;
			MbarBeam.Initialize();
			// Change by ZINU.('06.03.05). CpbeD.bNum -> m_pMyDB->Is_RebarNum().
			MbarBeam.iArrangeType = (RbarRCData.bNum ? 0 : 1);

			Rbar = RbarRCData.arBotBar.GetAt(k);

			if(Rbar.dDt <=0.0) continue;
			CString strName1 = (Rbar.sBarNa1);
			CString strName2 = (Rbar.sBarNa2);
			CStringW strwName1(strName1);
			CStringW strwName2(strName2);

			if(ExcelSect.dFy>350.0)
			{
				strName1.Replace(_T("D"), _T("H"));
				strName2.Replace(_T("D"), _T("H"));
			}
			MbarBeam.strName1 = strwName1;
			MbarBeam.strName2 = strwName2;
			MbarBeam.dSpace   = Rbar.dCTC;
			MbarBeam.dNum     = Rbar.dNum;
			MbarBeam.dDc      = Rbar.dDt;
			MbarBeam.dAs      = Rbar.dAs_row; //dAsLayerBot[k];    
			ExcelSect.arMainBar.Add(MbarBeam);        
		}			
	}


	//if(bTopDir_Shear) 
	{			
		ExcelSect.iAsResVz = SortData.iAsResV;			
		ExcelSect.dVu      = SortData.dVu;

		ExcelSect.dReqAv  = SortData.dreqAv;
		ExcelSect.dMinAv  = SortData.dminAv;
		ExcelSect.dMinAv1 = SortData.dminAv1;
		ExcelSect.dMinAv2 = SortData.dminAv2;
		ExcelSect.dUseAv  = SortData.dAsVChk;		
		ExcelSect.dPhiVc  = SortData.dpVc;
		ExcelSect.dPhiVs  = SortData.dpVs;
		ExcelSect.dPhiVn  = ExcelSect.dPhiVc + ExcelSect.dPhiVs;
		ExcelSect.dVc11   = SortData.dVc11;
		ExcelSect.dVc21   = SortData.dVc21;
		ExcelSect.dLimitS = SortData.dLimS;
		ExcelSect.bShearRbarReq = SortData.bShearRbarReq;

		// DeepBeam
		//       ExcelSect.iDeepBeamRes = SortData.iDeepBeamRes;
		//       ExcelSect.dUseAv_H  = SortData.dAsVChk_H;
		//       ExcelSect.dMinAv_H  = SortData.dminAv_H;
		//       ExcelSect.dLimitS_H = SortData.dLimS_H;
		//       ExcelSect.dVnLim_DB = SortData.dVnLim_DB;
		//if(UrbeD.nType == EN_RCBEAM_DEEPBEAM) ExcelSect.dPhiVn  = min(ExcelSect.dPhiVn, SortData.dpVnLimDB_N);
	}
	//     else
	//     {
	//       ExcelSect.iAsResVz = SortData.iAsResV_P;			
	//       ExcelSect.dVu     = SortData.dVu_P;
	// 
	//       ExcelSect.dReqAv  = SortData.dreqAv_P;
	//       ExcelSect.dMinAv  = SortData.dminAv_P;
	//       ExcelSect.dMinAv1 = SortData.dMinAv1_P;
	//       ExcelSect.dMinAv2 = SortData.dMinAv2_P;
	//       ExcelSect.dUseAv  = SortData.dAvChk_P;		
	//       ExcelSect.dPhiVc  = SortData.dpVc_P;
	//       ExcelSect.dPhiVs  = SortData.dpVs_P;
	//       ExcelSect.dPhiVn  = ExcelSect.dPhiVc + ExcelSect.dPhiVs;
	//       ExcelSect.dVc11   = SortData.dVc11_P;
	//       ExcelSect.dVc21   = SortData.dVc21_P;
	//       ExcelSect.dLimitS = SortData.dLimS_P;		
	//       ExcelSect.bShearRbarReq = SortData.bShearRbarReq_P;
	// 
	//       // DeepBeam
	//       ExcelSect.iDeepBeamRes = SortData.iDeepBeamRes_P;
	//       ExcelSect.dUseAv_H  = SortData.dAvChkH_P;
	//       ExcelSect.dMinAv_H  = SortData.dminAvH_P;
	//       ExcelSect.dLimitS_H = SortData.dLimSH_P;
	//       ExcelSect.dVnLim_DB = SortData.dVnLimDB_P;
	//       if(UrbeD.nType == EN_RCBEAM_DEEPBEAM) ExcelSect.dPhiVn  = min(ExcelSect.dPhiVn, SortData.dpVnLimDB_P);
	//     }

	//     if(bTopDir_Ser)
	//     {
	//       ExcelSect.iChkSerLcom = (SortData.iLcomTC < 0) ? 1 : 2;
	//       if(iCodeID==DGN_KSCE_RC_USD05 || iCodeID==DGN_KSCE_RC_RAIL_USD04)
	//       {
	//         ExcelSect.dMs     = SortData.dTopMuw;
	//         ExcelSect.dWa     = SortData.dTopwa;
	//         ExcelSect.dW      = SortData.dTopw;
	//       }
	//       else if(iCodeID==DGN_KCI_USD07 || iCodeID==DGN_KSCE_RC_USD10 ||
	//         iCodeID==DGN_KSCE_RC_RAIL_USD11 || iCodeID==DGN_KCI_USD12)
	//       {
	//         ExcelSect.dMs     = SortData.dTopMus;
	//         ExcelSect.dSa     = SortData.dTopsa;
	//         ExcelSect.dS      = SortData.dTops;
	//         ExcelSect.dRats   = ExcelSect.dS==0.0 ? 0.0 : ExcelSect.dSa/ExcelSect.dS;
	//         ExcelSect.bSpaceOK= ExcelSect.dSa>=ExcelSect.dS ? TRUE : FALSE;
	//       }
	// 
	//       ExcelSect.dChkLimitFyr = SortData.dChkLimitFyr[0];
	//       ExcelSect.iCrkRes = SortData.iCrkRes[0];
	//       ExcelSect.dDcStr  = UrbeD.Rbar.dDt[0];		
	// 
	//       ExcelSect.dDc     = SortData.dDc[0]  ;
	//       ExcelSect.dN      = SortData.dn[0]   ;
	//       ExcelSect.dRho    = SortData.dRho[0] ;
	//       ExcelSect.dK      = SortData.dK[0]   ;
	//       ExcelSect.dx      = SortData.dx[0]   ;
	//       ExcelSect.dIcr    = SortData.dIcr[0] ;
	//       ExcelSect.dFs     = SortData.dFs[0]  ;
	//       ExcelSect.dR      = SortData.dR[0]   ;
	//       ExcelSect.dRUp    = SortData.dRUp[0] ;
	//       ExcelSect.dRDn    = SortData.dRDn[0] ;
	//       ExcelSect.dA      = SortData.dA[0]   ;
	//       ExcelSect.dNBar   = SortData.dNBar[0];
	//       ExcelSect.dSa1    = SortData.dSa1[0] ;
	//       ExcelSect.dSa2    = SortData.dSa2[0] ;
	//       ExcelSect.dCc     = SortData.dCc[0]  ;
	//       ExcelSect.iSpcRes = SortData.iSpcRes[0]; 
	//     }
	//     else
	//     {
	//       ExcelSect.iChkSerLcom = (SortData.iLcomBC < 0) ? 1 : 2;
	//       if(iCodeID==DGN_KSCE_RC_USD05 || iCodeID==DGN_KSCE_RC_RAIL_USD04)
	//       {
	//         ExcelSect.dMs     = SortData.dBotMuw;
	//         ExcelSect.dWa     = SortData.dBotwa;
	//         ExcelSect.dW      = SortData.dBotw;
	//       }
	//       else if(iCodeID==DGN_KCI_USD07 || iCodeID==DGN_KSCE_RC_USD10 ||
	//         iCodeID==DGN_KSCE_RC_RAIL_USD11  || iCodeID==DGN_KCI_USD12)
	//       {
	//         ExcelSect.dMs     = SortData.dBotMus;
	//         ExcelSect.dSa     = SortData.dBotsa;
	//         ExcelSect.dS      = SortData.dBots;
	//         ExcelSect.dRats   = ExcelSect.dS==0.0 ? 0.0 : ExcelSect.dSa/ExcelSect.dS;
	//         ExcelSect.bSpaceOK= ExcelSect.dSa>=ExcelSect.dS ? TRUE : FALSE;
	//       }
	// 
	//       ExcelSect.dChkLimitFyr = SortData.dChkLimitFyr[1];
	//       ExcelSect.iCrkRes = SortData.iCrkRes[1];
	//       ExcelSect.dDcStr  = UrbeD.Rbar.dDb[0];		
	// 
	//       ExcelSect.dDc     = SortData.dDc[1]  ;
	//       ExcelSect.dN      = SortData.dn[1]   ;
	//       ExcelSect.dRho    = SortData.dRho[1] ;
	//       ExcelSect.dK      = SortData.dK[1]   ;
	//       ExcelSect.dx      = SortData.dx[1]   ;
	//       ExcelSect.dIcr    = SortData.dIcr[1] ;
	//       ExcelSect.dFs     = SortData.dFs[1]  ;
	//       ExcelSect.dR      = SortData.dR[1]   ;
	//       ExcelSect.dRUp    = SortData.dRUp[1] ;
	//       ExcelSect.dRDn    = SortData.dRDn[1] ;
	//       ExcelSect.dA      = SortData.dA[1]   ;
	//       ExcelSect.dNBar   = SortData.dNBar[1];
	//       ExcelSect.dSa1    = SortData.dSa1[1] ;
	//       ExcelSect.dSa2    = SortData.dSa2[1] ;
	//       ExcelSect.dCc     = SortData.dCc[1]  ;
	//       ExcelSect.iSpcRes = SortData.iSpcRes[1]; 
	//     }

	//     if(i==0) ExcelSect_Main = ExcelSect;
	//     else     ExcelSect_Sub  = ExcelSect;
	//}

	return TRUE;
}

BOOL CRptRating_RC_KSCE::ConvertToDLL_RF(T_ELEM_K ElemK, int nIJ, int nPosNeg, const T_RPAC_D &crRpacD, T_RFRC_BASE& BaseD, RF_RES_RF_KSCE_RC_D& RptD)
{
	// nIJ     : 0=I, 1=J
	// nPosNeg : 0=max(정),1=min(부)
	RptD.nElem  = ElemK;
	RptD.nIJ    = nIJ;
	RptD.nEstimatedMethod = crRpacD.nSurveyMeth;

	CStringW strwLcomName(BaseD.strLcomName);
	RptD.Force[nPosNeg].strLcom       = strwLcomName;
	RptD.Force[nPosNeg].dphiMn        = BaseD.dPhiMn;
	RptD.Force[nPosNeg].dMd           = BaseD.dMd;
	RptD.Force[nPosNeg].dMl           = BaseD.dM_L;
	RptD.Force[nPosNeg].dMdm          = BaseD.dMd_m;
	RptD.Force[nPosNeg].dMa           = BaseD.dMdl;
	RptD.Force[nPosNeg].dImpactFactor = BaseD.d1_plus_i_calc;
	RptD.Force[nPosNeg].dAddReponseFactor = BaseD.dAddResponceFactor;
	RptD.Force[nPosNeg].dRF           = BaseD.dRF;

	RptD.Delta[nPosNeg].strLcom        = strwLcomName;
	RptD.Delta[nPosNeg].dCalcDelta     = BaseD.dCalc_defl;
	RptD.Delta[nPosNeg].dEstiDelta     = BaseD.dRealDispZ;
	RptD.Delta[nPosNeg].dRatioDelta    = BaseD.dDisp_rat;
	RptD.Delta[nPosNeg].dCalci         = BaseD.dImpFactor;
	RptD.Delta[nPosNeg].dEstii         = BaseD.dRealImpFactor;
	RptD.Delta[nPosNeg].dRatioi        = BaseD.di_rat;
	RptD.Delta[nPosNeg].dKs            = BaseD.dKs;

	RptD.Ewips[nPosNeg].strLcom        = strwLcomName;
	RptD.Ewips[nPosNeg].dCalcEwips     = BaseD.dCalcEwip;
	RptD.Ewips[nPosNeg].dEstiEwips     = BaseD.dEstiEwip;
	RptD.Ewips[nPosNeg].dRatioEwips    = BaseD.dEwip_rat;
	RptD.Ewips[nPosNeg].dCalci         = BaseD.dImpFactor;
	RptD.Ewips[nPosNeg].dEstii         = BaseD.dRealImpFactor;
	RptD.Ewips[nPosNeg].dRatioi        = BaseD.di_rat;
	RptD.Ewips[nPosNeg].dKs            = BaseD.dKs;

	RptD.LoadP[nPosNeg].strLcom        = strwLcomName;

	CString strDesignPr; strDesignPr.Format(_T("%s-%.3f"), crRpacD.strDgnLiveLoad, crRpacD.dDgnLiveLoad);
	CStringW strWDesignPr(strDesignPr);
	RptD.LoadP[nPosNeg].strDesignPr = strWDesignPr;

	if (fabs(BaseD.dP0 - 0.0) > 10e-5)
	{
		CString strP0; strP0.Format(_T("%s-%.3f"), crRpacD.strDgnLiveLoad, BaseD.dP0);
		CStringW strWP0(strP0); 
		RptD.LoadP[nPosNeg].strP0 = strWP0;  // 기본 내하력
	}

	RptD.LoadP[nPosNeg].dKs            = BaseD.dKs;
	RptD.LoadP[nPosNeg].dP             = BaseD.dP0a; // 공용 내하력
	RptD.LoadP[nPosNeg].nOK            = (BaseD.bOK)? 1 : 2;

	RptD.Level[nPosNeg].strLcom        = strwLcomName;
	RptD.Level[nPosNeg].strLevel       = GetBridgeLevel(BaseD.nRankType);
	RptD.Level[nPosNeg].dfa            = BaseD.dPhiMn_Rank;
	RptD.Level[nPosNeg].dfdl           = BaseD.dMu_Rank;
	RptD.Level[nPosNeg].dRatio         = BaseD.dRatioMa;

	ConvertToDLL_RptForce(BaseD.Rating_Force.ForceDL, RptD.arForce[nPosNeg].ForceDL);
	ConvertToDLL_RptForce(BaseD.Rating_Force.ForceLL, RptD.arForce[nPosNeg].ForceLL);
	RptD.arForce[nPosNeg].ForceDL.strName = strwLcomName;
	RptD.arForce[nPosNeg].ForceLL.strName = strwLcomName;

	return TRUE;
}