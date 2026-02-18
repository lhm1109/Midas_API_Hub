#include "stdafx.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "RptKRRating.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\AnalysisResult.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\IRegressionInterface.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\FileCtrl.h"


#include "Dgn_RatingRptManager.h"
//#include "CRCExcelOutput.h"

#include "DgnProgressDlg.h"
#include "DgnPlateGirder.h"
//#include "CRCDataCtrl.h"
#include "RptRatingKR_STL_ASD.h"
#include "RptRating_PSC.h"
#include "RptRatingSTL_AASHTO_LRFR11.h"
#include "RptRatingSTL_AASHTO_LRFR19.h"
#include "RptRatingPSC_AASHTO_LRFR12.h"
#include "RptRatingPSC_AASHTO_LRFR19.h"
#include "RptRating_RC_KSCE.h"
#include "RptRatingPSC_BD21_01.h"
#include "RptRatingSTL_NR2006.h"
#include "..\wg_dbLock\LockMgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptKRRating::CRptKRRating()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;

	m_UnitSaver.Save();
}

CRptKRRating::~CRptKRRating()
{

}



void CRptKRRating::Start(CString sFileName, int iPrintOpt, BOOL bSaveImage)
{
	CCurUnitSaver Save(TRUE);

	int nDgnCode = CDBLib::GetRatCode(); 	

	SetCodeUnit(nDgnCode);

    BOOL bRegression = CLockMgr::Get_IsRegressionTest();

    if ( bRegression )
    {
        _REGRESSION_TEST_D* pInfoD = IRegressionInterface::Instance()->GetPtrRTInfo();
        if ( pInfoD->arOutFile_Trg.GetSize() > 1 )
        {
            sFileName = pInfoD->arOutFile_Trg[1];
        }
    }

	if(nDgnCode==AASHTO_LRFD05_PSC_RATING || nDgnCode==AASHTO_LRFD12_PSC_RATING )
	{
		T_RACD_D Data;  Data.Initialize();	
		m_pDoc->m_pAttrCtrl2->GetDgnRacd(Data);	

		CRptRatingPSC_AASHTO_LRFR12 RatingRpt;
		RatingRpt.Execute_RatingReport(Data.nRatingCode, sFileName, iPrintOpt, bSaveImage);
				
	}
    else if ( nDgnCode==AASHTO_LRFD19_PSC_RATING )
    {
        T_RACD_D Data;  Data.Initialize();
        m_pDoc->m_pAttrCtrl2->GetDgnRacd(Data);

        CRptRatingPSC_AASHTO_LRFR19 RatingRpt;
        RatingRpt.Execute_RatingReport(Data.nRatingCode, sFileName, iPrintOpt, bSaveImage);
    }
	else if(CDBLib::IsPscRatingCodeKRSince10(nDgnCode))
	{
		// ******************************************************
		// 05 기준은  CRating_RKMC_XLOut::PrintReport 에서 출력됨.
		// ******************************************************

		CRptRating_PSC Rating_PSC;
		Rating_PSC.Execute_RatingReport( nDgnCode, sFileName, iPrintOpt, bSaveImage);
	}
	else if (nDgnCode==STL_KSCE_ASD05 || nDgnCode==STL_KSCE_ASD10 || nDgnCode==STL_KSCE_RAIL_ASD04 || nDgnCode==STL_KSCE_RAIL_ASD11 ||
					 nDgnCode==STL_KSCE_LSD15)
	{
		T_RCDS_D Data;  Data.Initialize();	
		m_pDoc->m_pAttrCtrl2->GetDgnRcds(Data);	
		
		CRptRatingKR_STL_ASD Rating_ASD;
		Rating_ASD.Execute_RatingReport( Data.nRatingCode, sFileName, iPrintOpt, bSaveImage);
	
	}
	else if ( nDgnCode==STL_AASHTO_LRFD12 )
	{
		T_RCDS_D Data;  Data.Initialize();	
		m_pDoc->m_pAttrCtrl2->GetDgnRcds(Data);	

		CRptRatingSTL_AASHTO_LRFR11 RatingRpt;
		RatingRpt.Execute_RatingReport(Data.nRatingCode, sFileName, iPrintOpt, bSaveImage);
	}
    else if ( nDgnCode==STL_AASHTO_LRFD19 )
    {
        T_RCDS_D Data;  Data.Initialize();
        m_pDoc->m_pAttrCtrl2->GetDgnRcds(Data);

        CRptRatingSTL_AASHTO_LRFR19 RatingRpt;
        RatingRpt.Execute_RatingReport(Data.nRatingCode, sFileName, iPrintOpt, bSaveImage);
    }
	else if(CDBLib::IsRcRatingCodeKRSince10(nDgnCode))
	{
		T_RCDC_D Data;  Data.Initialize();	
		m_pDoc->m_pAttrCtrl2->GetDgnRcdc(Data);	

		CRptRating_RC_KSCE RcratingRpt;  
		// Beam
		if(m_pDoc->IsRCRating_KSCE_USD_Beam())
		{
			RcratingRpt.Excute_Print_RCRatinig_KSCE(Data.nRatingCode, sFileName, iPrintOpt, bSaveImage);
		}
		// Plate
		if(m_pDoc->IsRCRating_KSCE_USD_Plate())  
		{
			//////////////////////////////////////////////////////////////////////////
			CString strXlsx = _T(".xlsx");
			int iRight = sFileName.Find(strXlsx);
			if (iRight < 0)
			{
				strXlsx = _T(".xls");
				iRight = sFileName.Find(strXlsx);
			}

			if (iRight >= 0)
			{
				CString strRight = sFileName.Left(iRight);
				sFileName = strRight + _T("_Plate") + strXlsx;
			}
			//////////////////////////////////////////////////////////////////////////

			RcratingRpt.Excute_Print_RCRatinig_KSCE_PLATE(Data.nRatingCode, sFileName, iPrintOpt, bSaveImage);
		}

	}
    else if(CDBLib::IsPscRatingCodeBS(nDgnCode)) // PMS.5862 BS BD21 Assessment
    {
        CRptRatingPSC_BD21_01 RatingRpt(nDgnCode);
        RatingRpt.Execute_RatingReport(nDgnCode, sFileName, iPrintOpt, bSaveImage);
    }
    else if ( CDBLib::IsStlRatingCodeBS(nDgnCode) )
    {
        CRptRatingPSC_BD21_01 RatingRpt(nDgnCode);
        RatingRpt.Execute_StlRatingReport(nDgnCode, sFileName, iPrintOpt, bSaveImage);
    }
	else if(CDBLib::IsStlRatingCodeNR_GN_CIV(nDgnCode))
	{
		CRptRatingSTL_NR2006 RatingRpt(nDgnCode);
		RatingRpt.Execute_StlRatingReport(nDgnCode, sFileName, iPrintOpt, bSaveImage);
	}
	else ASSERT(0);


}

void CRptKRRating::SetCodeUnit(int nDgnCode)
{
	T_UNIT_INDEX CngUnit;

	switch (nDgnCode)
	{
		case KSCE_USD10_RATING:
		case KSCE_RAIL_USD11_RATING:
		case KSCE_LSD15_RATING:
		case STL_KSCE_ASD05:
		case STL_KSCE_ASD10:
		case STL_KSCE_RAIL_ASD04:
		case STL_KSCE_RAIL_ASD11:
		case STL_KSCE_LSD15:
		case KSCE_USD10_RC_RATING:
		case KSCE_RAIL_USD11_RC_RATING:
		case KSCE_LSD15_RC_RATING:
        case CS_454_20_PSC_RATING:
        case STL_CS454_20:
		case STL_NR_GN_CIV_025_06:
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
			break;
		case STL_AASHTO_LRFD12:
        case STL_AASHTO_LRFD19:
		case AASHTO_LRFD12_PSC_RATING:
        case AASHTO_LRFD19_PSC_RATING:
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_KIP;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
			break;

	}
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit

}


CString CRptKRRating::GetSaveFileDirEx()
{
	CString strFileName=m_pDoc->GetPathName();
	
	CFileCtrl FileCtrl(strFileName);
	
	return FileCtrl.GetFilePath();
}


CString CRptKRRating::GetSaveFileNameEx(BOOL bWithoutExtension /* = FALSE*/)
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
		strSaveFileName = saveFileCtrl->GetFileName();
	
	delete saveFileCtrl;
	
	return strSaveFileName;
}

CString CRptKRRating::GetLoadCombType(int iMaxType)
{
	if (iMaxType > 12) return _T("");

	LPCTSTR aType[] = {_T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN), 
		_LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
		_LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN)};
	
	return aType[iMaxType];
}
