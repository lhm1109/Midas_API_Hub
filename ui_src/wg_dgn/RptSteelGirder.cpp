
#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "..\dgnengine\idesign\DGN_link\IDgnDocCtrl.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\AnalysisResult.h"
#include "..\wg_db\DBLib.h"

#include "Dgn_CSGRptManager_LSD12.h"
#include "Dgn_CSGRptManager_KSSC2014.h"

#include "DgnProgressDlg.h"
#include "DgnPlateGirder.h"
#include "DgnCsgDataCtrl.h"

#include "RptPlateGirder_EC4.h"
#include "RptSteelGirder_LSD12.h"
#include "RptSteelGirder_AASHTO12.h"
#include "RptSteelGirder_KSSC2014.h"
#include "DgnPSCSelectPrintUintDlg.h"
#include "RptPlateGirder_IRC22.h"
#include "RptSteelGirder_CSA_S6.h"
#include "RptPlateGirder_IRC22_15.h"

#include "DgnSelectPrintLanguageDlg.h"
#include "DgnCodeCtrl.h"
#include "RptSteelGirder.h"
#include "DgnReportMaker.h"
#include "..\wg_dbLock\LockMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptSteelGirder::CRptSteelGirder()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;

	m_CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(m_CpgdD);  

}

CRptSteelGirder::~CRptSteelGirder()
{
}

void CRptSteelGirder::Start(CString sFileName, int iPrintOpt, BOOL bSaveImage)
{
	CCurUnitSaver Save(TRUE);

	BOOL bRegression = CLockMgr::Get_IsRegressionTest();

	if(m_CpgdD.iDgnCode==EUROCODE4_2_04_CSG)
	{
		ArrElemPairKey aRptElemKey;
		ArrElemPairKey aRptGenElemKey;

		CRptPlateGirder_EC4 pRptEC4;
		pRptEC4.Get_RptElemKey(aRptElemKey, aRptGenElemKey);

		if ( !CDgnCodeCtrl::IsIDesignModule(m_CpgdD.iDgnCode) )
		{
			// PMS.5847 Eurocode Design 엑셀리포트 체코어 출력
			int iSelLang = 0;
			if ( !bRegression )
			{
				if ( CDBLib::AllowCzech() || CDBLib::AllowPolish() )
				{
					CDgnSelectPrintLanguageDlg Dlg;
					if ( Dlg.DoModal() == IDOK )
						iSelLang = Dlg.GetSelLang();
					else return;
				}
			}

			CRptPlateGirder_EC4 pRptEC4;
			pRptEC4.PrintReport(sFileName, iPrintOpt, bSaveImage, iSelLang);
		}
		else
		{
			if ( aRptElemKey.GetSize() > 0 )
			{
				m_pDoc->SetCivilCodeDgn(m_CpgdD.iDgnCode);

				CCRCDataCtrl RcDatCtrl;
				CDgnPlateGirder PlateGirder;
				PlateGirder.SetDataCtrlPointer(&RcDatCtrl);
				CDgnReportMaker ReportMaker(&PlateGirder);

				if ( bRegression )
				{
					PlateGirder.m_strTestDgnFile = sFileName;
				}
				PlateGirder.Set_PrintMode(TRUE);
				ReportMaker.RunDetailReport();
			}

			if ( aRptGenElemKey.GetSize() > 0 )
			{
				// PMS.5847 Eurocode Design 엑셀리포트 체코어 출력
				int iSelLang = 0;
				if ( !bRegression )
				{
					if ( CDBLib::AllowCzech() || CDBLib::AllowPolish() )
					{
						CDgnSelectPrintLanguageDlg Dlg;
						if ( Dlg.DoModal() == IDOK )
							iSelLang = Dlg.GetSelLang();
						else return;
					}
				}

				pRptEC4.PrintReport(sFileName, iPrintOpt, bSaveImage, iSelLang);
			}
		}
	}
	else if (m_CpgdD.iDgnCode==IRC22_08_CSG)
	{
		CRptPlateGirder_IRC22 pRptIRC22;
		pRptIRC22.PrintReport(sFileName, iPrintOpt, bSaveImage);
	}
	else if (m_CpgdD.iDgnCode==IRC22_15_CSG)
	{
		CRptPlateGirder_IRC22_15 pRptIRC22_15;
		pRptIRC22_15.PrintReport(sFileName, iPrintOpt, bSaveImage);
	}
	else if(m_CpgdD.iDgnCode==KSSC_2014_CSG || m_CpgdD.iDgnCode == KDS_24_14_31_2018_CSG) // 만약 , KSSC 2014 강합성에 대하여 SI 아닌 다른 단위계로 결과 출력이 요구될 경우, 여기 수정
	{
		BOOL bOK = FALSE;
		T_UNIT_INDEX CngUnit;
		// Default Unit-System. N / MM
		CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
		CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit

		CRptSteelGirder_KSSC2014 CSG_LSD12_Curve;

		CSG_LSD12_Curve.Execute_CSGReport_KSSC2014(KSSC_2014_CSG, sFileName, iPrintOpt, bSaveImage);
	}
	else if ( CDBLib::IsCSGCodeLSD_KR(m_CpgdD.iDgnCode) )
	{
		T_UNIT_INDEX CngUnit;
		if(!bRegression && m_CpgdD.nConsiderPv==1) // KSSC 2014인 경우로 우선 SI만 지원
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;

			m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit

			CRptSteelGirder_KSSC2014 CSG_LSD12_Curve;

			CSG_LSD12_Curve.Execute_CSGReport_KSSC2014(KSSC_2014_CSG, sFileName, iPrintOpt, bSaveImage);
		}    
		else
		{
			BOOL bOK = FALSE;

			// Default Unit-System. N / MM
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
			m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit

			CRptSteelGirder_LSD12 CSG_LSD12;
			//---------------------------------------------------------------------------
			bOK = CSG_LSD12.Execute_CSGReport_LSD12(m_CpgdD.iDgnCode, sFileName, iPrintOpt, bSaveImage); // 
			//---------------------------------------------------------------------------
		}
	}
	else if (m_CpgdD.iDgnCode==AASHTO_LRFD07_CSG || m_CpgdD.iDgnCode==AASHTO_LRFD12_CSG || m_CpgdD.iDgnCode==AASHTO_LRFD16_CSG || m_CpgdD.iDgnCode==AASHTO_LRFD17_CSG || m_CpgdD.iDgnCode==AASHTO_LRFD20_CSG )
	{
		T_UNIT_INDEX CngUnit;
		int iRptSelUnit = CDBLib::EN_REPORT_UNIT_US;
		if(!bRegression)
		{
			if ( !dgn::link::IDgnDocCtrl::Instance()->OnChangeLocale(CDgnCodeCtrl::GetCpgLocalePref()) )
			{
				ASSERT(0); return;

			}

			iRptSelUnit = CDgnCodeCtrl::GetCpgReportUnit() == EN_PREF_REPORT_UNIT::EN_REPORT_UNIT_SI ? CDBLib::EN_REPORT_UNIT_SI : CDBLib::EN_REPORT_UNIT_US;

			CDBLib::SetReportSelUnit(iRptSelUnit);
// 			CDgnPSCSelectPrintUnitDlg Dlg; Dlg.SetDesignType(CDgnPSCSelectPrintUnitDlg::EN_MEMBER_CSG);			
// 			if(Dlg.DoModal() == IDOK)
// 			{
// 				iRptSelUnit  = Dlg.GetSelectUnit();
// 				CDBLib::SetReportSelUnit(iRptSelUnit);
// 			}
// 			else
// 			{
// 				return;
// 			}
		}

		if(iRptSelUnit==CDBLib::EN_REPORT_UNIT_US ) // US unit // Default Unit-System. Kip / in
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_KIP;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
		}
		else if(iRptSelUnit==CDBLib::EN_REPORT_UNIT_SI ) // SI unit
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		}
		else ASSERT(0);

		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit


		CRptSteelGirder_AASHTO12 CSG_LSD12;
		CSG_LSD12.Execute_CSGReport_AASHTO12(m_CpgdD.iDgnCode, sFileName, iPrintOpt, bSaveImage);
	}
	else if(m_CpgdD.iDgnCode == CSA_S6_10_CSG || m_CpgdD.iDgnCode == CSA_S6_14_CSG || m_CpgdD.iDgnCode == CSA_S6_19_CSG)
	{
		T_UNIT_INDEX CngUnit;
		CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
		CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;

		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit

		CRptSteelGirder_CSA_S6 pRptCSA_S6;
		pRptCSA_S6.Execute_CSGReport_CSA_S6(m_CpgdD.iDgnCode, sFileName, iPrintOpt, bSaveImage);
	}
	else if ( m_CpgdD.iDgnCode == AS5100_6_17_CSG )
	{
	    m_pDoc->SetCivilCodeDgn(m_CpgdD.iDgnCode);
	    
		CCRCDataCtrl DataCtrl;	
	    CDgnCsgDataCtrl CsgCtrl;
		CsgCtrl.SetDataCtrlPointer(&DataCtrl);
	    CDgnReportMaker ReportMaker(&CsgCtrl);

	    if ( bRegression )
	    {
			CsgCtrl.m_strTestDgnFile = sFileName;
	    }
		//CsgCtrl.Set_PrintMode(TRUE);
		if ( !ReportMaker.RunDetailReport() )
		{
			GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_RATING_NON_MEMBER));
		}
	}
	else
		ASSERT(0);
}


CString CRptSteelGirder::GetSaveFileDirEx()
{
	CString strFileName=m_pDoc->GetPathName();

	CFileCtrl FileCtrl(strFileName);

	return FileCtrl.GetFilePath();
}


CString CRptSteelGirder::GetSaveFileNameEx(BOOL bWithoutExtension /* = FALSE*/)
{
	CString strFileName=m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strFileName);

	CString strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".xls");

	CFileCtrl* saveFileCtrl = new CFileCtrl(strSaveFileName);

	int count  = 0;
	while(saveFileCtrl->FileExists())
	{
		CString sCount;
		sCount.Format(_T("%d"), ++count);

		CString strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + sCount + _T(".xls");

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

CStringW CRptSteelGirder::GetLoadCombType(int iMaxType)
{
    CStringW strType(CDBLib::GetConcurrentForceType(iMaxType));

	return strType;
}