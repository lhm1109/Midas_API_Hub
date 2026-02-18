// CRCExcelOutput.cpp: implementation of the CCRCExcelOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "..\DgnEngine\Src\DGNe_RUS\DgnCalc_Rus_PSC_Struct.h"
#include "CRCExcelOutput.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\FileCtrl.h"

#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\PCDesign.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\IRegressionInterface.h"
#include "..\wg_dbLock\LockMgr.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\Product.h"
#include "..\wg_base\MSExcel.h"
#include "..\wg_base\MSOfficeMgr.h"
#include "..\wg_base\XLParser.h"

//#include "Dgn_PSCRptManager.h"
#include "CRCDataCtrl.h"
#include "Dgn_PSCRptManager_EC2.h"
#include "Dgn_PSCRptManager_IRC112.h"
#include "Dgn_PSCRptManager_LSD12_Comp.h"
#include "Dgn_PSCRptManager_SNiP20503_84.h"
#include "Dgn_PSCRptManager_AS5100.h"
#include "Dgn_PSCRptManager_IRS.h"
#include "Dgn_PSCRptManager_BS.h"
#include "Dgn_PSCRptManager_TMH7.h"
#include "DgnPscDataCtrl.h"
#include "DgnAASHTO_LRFD08.h"


template<class T>
struct _T_RPT_TABLE
{
    int iIJ;
    BOOL bGirder;
    T   RptData;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCRCExcelOutput::CCRCExcelOutput()
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	m_bStopExecute = FALSE;
	
	m_PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(m_PscdD);
	
	m_strChrType = _T("");
	m_strBasePath= _T("");  
	m_strBaseBook= _T("");  
	m_strBaseSheet= _T(""); 

    switch ( m_PscdD.iDgnCode )
    {
    case KSCE_USD03:
        m_pDgnKSCE = m_DataCtrl.Get_DgnKscePointer();
        m_pPrtKSCE = m_DataCtrl.Get_PrtKscePointer();

        m_pPrtKSCE->Set_PrintFlag(TRUE);
        m_pPrtKSCE->Set_InitData();
        ASSERT(m_pPrtKSCE);
        break;
    case KSCE_USD05:
    case KSCE_RAIL_USD04:
        m_pDgnKSCE05 = m_DataCtrl.Get_DgnKsce05Pointer();
        m_pPrtKSCE   = (CPrtKSCE_USD03 *)m_DataCtrl.Get_PrtKsce05Pointer();

        m_pPrtKSCE->Set_PrintFlag(TRUE);
        m_pPrtKSCE->Set_InitData();
        ASSERT(m_pPrtKSCE);

        m_strChrType = _T("굴림체");
        break;
    case KSCE_USD10:
    case KSCE_RAIL_USD11:
        m_pDgnKSCE05 = m_DataCtrl.Get_DgnKsce05Pointer();
        m_pPrtKSCE   = (CPrtKSCE_USD03 *)m_DataCtrl.Get_PrtKsce05Pointer();
        m_pPrtKSCE->Set_PrintFlag(TRUE);
        m_pPrtKSCE->Set_InitData();
        ASSERT(m_pPrtKSCE);

        m_strChrType = _T("맑은 고딕");
        m_pPrtKSCE10 = (CPrtKSCE_USD10 *)m_DataCtrl.Get_PrtKsce10Pointer();
        break;
    case AASHTO_LRFD08:
    case AASHTO_LRFD12:
    case AASHTO_LRFD14:
    case AASHTO_LRFD16:
    case AASHTO_LRFD17:
    case AASHTO_LRFD20:
	case AASHTO_LRFD24:
	case AREMA_23:
        m_DataCtrl.Cal_LcomDataForDesignAll_for_LRFDPrint();
        m_DataCtrl.Get_JointData4Segment();
        break;
    case BS5400_90_PSC:
        m_DataCtrl.Cal_LcomDataForDesignAll_for_LRFDPrint();
        m_DataCtrl.Get_JointData4Segment();
        break;
    case JTG_D62_04:
    case CSA_S6_10:
    case CSA_S6_14:
	case CSA_S6_19:
    case EUROCODE2_2_05_PSC:
    case KDS_24_14_21_2022_PSC:
    case KSCE_LSD15_PSC:
    case IRC_112_2011_PSC:
    case IRC_112_2020_PSC:
    case AS_5100_5_17_PSC:
    case BD_44_15_PSC:
    case CS_455_PSC:
    case AASHTO_LRFD12_PSC_RATING:
    case AASHTO_LRFD19_PSC_RATING:
    case SNiP_20503_84_PSC:
    case SP_35_13330_11_PSC:
    case SNiP_20503_84_PSC_MKS:
    case SP_35_13330_11_PSC_MKS:
    case IRS_PSC:
	case TMH07_3_1989:
	case ABNT_NBR_6118_2023_PSC:
        break;
    default:
        ASSERT(0);
        break;
    }
	
	m_iCountSheet = 0;
	
	m_iMaxCols = 100;
	
	m_pXL  = NULL;
	m_pPrs = NULL;
}

CCRCExcelOutput::~CCRCExcelOutput()
{  

}

void CCRCExcelOutput::Start()
{  
	CCurUnitSaver Save(TRUE);
	
	BOOL bRegression = CLockMgr::Get_IsRegressionTest();


	CString strSaveFileName = GetSaveFileNameEx();
	if (bRegression==TRUE)
	{
		_REGRESSION_TEST_D* pInfoD = IRegressionInterface::Instance()->GetPtrRTInfo();
		if (pInfoD->arOutFile_Trg.GetSize() > 1)
		{
			strSaveFileName = pInfoD->arOutFile_Trg[1];
		}    
	}

	// 설계요소와 출력요소의 교집합이 존재할 경우 Report과정을 수행함. 그렇지않다면, Message처리함.
	if(!IsPrintPosiData())
	{
		AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CHECK));  // _T("선택된 출력변수가 없습니다.")
		Progress(3, 0);
		return;
	}
	//
	int i=0;
	const int nCodePSC = m_PscdD.iDgnCode;
	if(nCodePSC ==KSCE_USD03 || nCodePSC ==KSCE_USD05 || nCodePSC ==KSCE_RAIL_USD04)
	{
		if(!AfxOleInit())
		{
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_INIT_COMDLL)); // _T("Could not initialize COM dll")
			return;
		}		
		
		UINT nFlg = -1;
		nFlg = XL_PscReport();
		
		if(nFlg<=0)
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	}
	else 	if(nCodePSC ==KSCE_USD10 || nCodePSC ==KSCE_RAIL_USD11)
	{
		if(!AfxOleInit())
		{
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_INIT_COMDLL)); // _T("Could not initialize COM dll")
			return;
		}		
		
		UINT nFlg = -1;
		nFlg = XL_PscReport_KSCE10();
		
		if(nFlg<=0)
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	}

	else if(nCodePSC ==JTG_D62_04)
	{
		ReportStart();
	}
	else if(nCodePSC ==AASHTO_LRFD08)
	{		
		UINT nFlg = -1;

		// Change unit
		T_UNIT_INDEX CngUnit;
		int iRptSelUnit = CDBLib::GetReportSelUnit();
		if(iRptSelUnit==CDBLib::EN_REPORT_UNIT_US) // US unit
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_KIP;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
		}
		else if(iRptSelUnit==CDBLib::EN_REPORT_UNIT_SI) // SI unit
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		}
		// PMS.5699 AASHTO LRFD - PSC 설계 엑셀 리포트 터키어 출력
		else if(iRptSelUnit==CDBLib::EN_REPORT_UNIT_SI_TR) // SI unit with Turkish
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		}
		else ASSERT(0);
		
		// 출력 단위
		m_DataCtrl.Set_PrintUnit(CngUnit.nBase_Force, CngUnit.nBase_Length);

		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
		//---------------------------------------------------------------------------
		nFlg = Execute_PSCReport(nCodePSC, iRptSelUnit, strSaveFileName);  //   Execute_PSCReport_AASHTOLRFD12
		//---------------------------------------------------------------------------
		
		if(nFlg<=0)
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	}
	else if(nCodePSC ==AASHTO_LRFD12 || nCodePSC ==AASHTO_LRFD14 || nCodePSC ==AASHTO_LRFD16 ||
			nCodePSC ==AASHTO_LRFD17 || nCodePSC ==AASHTO_LRFD20 || nCodePSC ==AASHTO_LRFD24)
	{		
		UINT nFlg = -1;
		
		// Change unit
		T_UNIT_INDEX CngUnit;
		int iRptSelUnit = CDBLib::GetReportSelUnit();

		if(iRptSelUnit==CDBLib::EN_REPORT_UNIT_US) // US unit
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_KIP;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
		}
		else if(iRptSelUnit==CDBLib::EN_REPORT_UNIT_SI) // SI unit
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		}
		// PMS.5699 AASHTO LRFD - PSC 설계 엑셀 리포트 터키어 출력
		else if(iRptSelUnit==CDBLib::EN_REPORT_UNIT_SI_TR) // SI unit with Turkish
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		}
		else ASSERT(0);

		// 출력 단위
		m_DataCtrl.Set_PrintUnit(CngUnit.nBase_Force, CngUnit.nBase_Length);

//     CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_KIP;
//     CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
 
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
		//---------------------------------------------------------------------------
		nFlg = Execute_PSCReport(m_PscdD.iDgnCode, iRptSelUnit, strSaveFileName);  //   Execute_PSCReport_AASHTOLRFD12
		//---------------------------------------------------------------------------
		
		if(nFlg<=0)
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	}
	else if(CDBLib::IsPscCodeCSA(m_PscdD.iDgnCode))
	{		
		UINT nFlg = -1;
		
		// Change unit
		T_UNIT_INDEX CngUnit;

		CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
		CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;   

		// 출력 단위
		m_DataCtrl.Set_PrintUnit(CngUnit.nBase_Force, CngUnit.nBase_Length);

		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit =  SI unit

		//---------------------------------------------------------------------------
		nFlg = Execute_PSCReport(m_PscdD.iDgnCode, 1, strSaveFileName);  //   Execute_PSCReport
		//---------------------------------------------------------------------------
		
		if(nFlg<=0)
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	}
	else if(m_PscdD.iDgnCode==EUROCODE2_2_05_PSC )
	{
		BOOL bOK = FALSE;
		
		// Change unit
		T_UNIT_INDEX CngUnit;

		// Default Unit-System. N / MM
		CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
		CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
		//---------------------------------------------------------------------------
		bOK = Execute_PSCReport_EC2_2_05(m_PscdD.iDgnCode, strSaveFileName); // Eurocode2-2:2005, PSC Design!!
		//---------------------------------------------------------------------------
		
		if(!bOK)
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	}  
	else if(m_PscdD.iDgnCode==IRC_112_2011_PSC || m_PscdD.iDgnCode == IRC_112_2020_PSC)
	{
		BOOL bOK = FALSE;

		// Change unit
		T_UNIT_INDEX CngUnit;

		// Default Unit-System. N / MM
		CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
		CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
		//---------------------------------------------------------------------------
		bOK = Execute_PSCReport_IRC112(m_PscdD.iDgnCode, strSaveFileName); // IRC112 , PSC Design!!
		//---------------------------------------------------------------------------

		if(!bOK)
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	} 
	else if( CDBLib::IsPscCodeLSD_KR(m_PscdD.iDgnCode) )
	{
		BOOL bOK = FALSE;
		
		// Change unit
		T_UNIT_INDEX CngUnit;
		
		// Default Unit-System. N / MM
		CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
		CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit

		//---------------------------------------------------------------------------
		bOK = Execute_PSCReport_LSD12(m_PscdD.iDgnCode, strSaveFileName); // PSC Design!!
		//---------------------------------------------------------------------------
		if(!bOK)
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	}  
	else if(m_PscdD.iDgnCode==BS5400_90_PSC)
	{
		BOOL bOK = FALSE;
		
		// Change unit
		T_UNIT_INDEX CngUnit;

		// Default Unit-System. N / MM
		CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
		CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
		//---------------------------------------------------------------------------
		bOK = Execute_PSCReport_BS(m_PscdD.iDgnCode, strSaveFileName); // Eurocode2-2:2005, PSC Design!!
		//---------------------------------------------------------------------------
		
		if(!bOK)
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	} 
	else if (m_PscdD.iDgnCode == TMH07_3_1989)
	{
	BOOL bOK = FALSE;

	// Change unit
	T_UNIT_INDEX CngUnit;

	// Default Unit-System. N / MM
	CngUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
	CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
	//---------------------------------------------------------------------------
	bOK = Execute_PSCReport_TMH(m_PscdD.iDgnCode, strSaveFileName); // Eurocode2-2:2005, PSC Design!!
	//---------------------------------------------------------------------------

	if (!bOK)
		AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	}
	else if(m_PscdD.iDgnCode==SNiP_20503_84_PSC || m_PscdD.iDgnCode==SP_35_13330_11_PSC || m_PscdD.iDgnCode==SNiP_20503_84_PSC_MKS || m_PscdD.iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		BOOL bOK = FALSE;
		
		// Change unit
		T_UNIT_INDEX CngUnit;
		
		// Default Unit-System. N / MM
		CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
		CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit

		//---------------------------------------------------------------------------
		bOK = Execute_PSCReport_SNiP20503_84(m_PscdD.iDgnCode, strSaveFileName); // PSC Design!!
		//---------------------------------------------------------------------------
	}
	else if(m_PscdD.iDgnCode==AS_5100_5_17_PSC )
	{
		BOOL bOK = FALSE;

		// Change unit
		T_UNIT_INDEX CngUnit;

		// Default Unit-System. N / MM
		CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
		CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		// 출력 단위
		m_DataCtrl.Set_PrintUnit(CngUnit.nBase_Force, CngUnit.nBase_Length);
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
		//---------------------------------------------------------------------------
		bOK = Execute_PSCReport_AS_5100_5(m_PscdD.iDgnCode, m_PscdD.nNationalAnnex, strSaveFileName); // Eurocode2-2:2005, PSC Design!!
		//---------------------------------------------------------------------------

		if(!bOK)
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	} 
	else if(m_PscdD.iDgnCode==IRS_PSC )
	{
		BOOL bOK = FALSE;

		// Change unit
		T_UNIT_INDEX CngUnit;

		// Default Unit-System. N / MM
		CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
		CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
		//---------------------------------------------------------------------------
		bOK = Execute_PSCReport_IRS(m_PscdD.iDgnCode, strSaveFileName); // IRC112 , PSC Design!!
		//---------------------------------------------------------------------------

		if(!bOK)
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	} 
	else if (m_PscdD.iDgnCode == ABNT_NBR_6118_2023_PSC)
	{
		BOOL bOK = FALSE;

		// Change unit
		T_UNIT_INDEX CngUnit;

		// Default Unit-System. N / MM
		CngUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
		CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
		//---------------------------------------------------------------------------
		bOK = Execute_PSCReport_EC2_2_05(m_PscdD.iDgnCode, strSaveFileName); // Eurocode2-2:2005, PSC Design!!
		//---------------------------------------------------------------------------

		if (!bOK)
			AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
	}
	else
		ASSERT(0);
	
	Progress(3, 0);
}

void CCRCExcelOutput::ReportStart()
{
/*
BOOL bError=TRUE;
CString strTitle = _T("Report Test");

	CRptJTG_D62_04 RptJTG_D62_04;
	CString strXmlPathName=RptJTG_D62_04.GetFilePathName()+_T(".xml");
	CString strXslPathName=RptJTG_D62_04.GetFilePathName()+_T(".xsl");
	CString strHtmlPathName=RptJTG_D62_04.GetFilePathName()+_T(".html");
	
	CString strFilePathName=RptJTG_D62_04.GetFilePathName()+_T(".html");
	CFileCtrl FileCtrl(strFilePathName);
	if(FileCtrl.FileExists())
	{
		if(IDYES==AfxMessageBox(_T("보고서가 이미 존재합니다. 생성하시겠습니까?"), MB_YESNO|MB_ICONINFORMATION))
		{
	if(!RptJTG_D62_04.GenerateReport()) return;
		}
	}
	else
	{
		if(!RptJTG_D62_04.GenerateReport()) return;
	}
	
		CRptTransformer RptTransformer;
		RptTransformer.ExportTo_HTML(strXmlPathName,strXslPathName,strHtmlPathName);
		
		TRY
		{
		CMDIChildWnd* pFrameWnd = NULL;
		CRuntimeClass* pViewClass = RUNTIME_CLASS(CReportView);
		POSITION pos = GetFirstViewPosition ();
		while(pos != NULL)
		{
		CView* pView = GetDocPoi->GetNextView(pos);
		if(pView->IsKindOf(pViewClass))
		{
		CMDIChildWnd* pTmpFrameWnd = (CMDIChildWnd*)pView->GetParentFrame();
		CString strViewTitle = _T("");
		pTmpFrameWnd->GetWindowText(strViewTitle);
		if(strViewTitle==strTitle)
		{
				pFrameWnd = (CMDIChildWnd*)pView->GetParentFrame();
				break;
		}
		}
		}
		
			if (pFrameWnd) 
			{
			pFrameWnd->ActivateFrame();
			CReportView* pReportView = (CReportView*)pFrameWnd->GetActiveView();
			pReportView->PreView(strFilePathName);
			}
			else
			{
			CAppBase* pApp = (CAppBase*)AfxGetApp();
			CMDIChildWnd* pFrameWnd = pApp->CreateOrActivateFrame(pViewClass, this);
			
			if(pFrameWnd)
			{
			pFrameWnd->SetWindowText(strTitle);
			CReportView* pReportView = (CReportView*)pFrameWnd->GetActiveView();
			pReportView->PreView(strFilePathName);
			}
			}
			}
			CATCH( CException, e )
			{
			bError=TRUE;
			e->ReportError();
			}
			END_CATCH
	*/
}

//EXCEL Work..
BOOL CCRCExcelOutput::XL_PscReport()
{  
	int nOutType;
	BOOL bRunMS, bRowMark, bColumnMark;
	CProduct::GetTestOutType(nOutType, bRunMS, bRowMark, bColumnMark);
	m_pXL = new CMSExcel(nOutType, bRunMS, bRowMark, bColumnMark);
	m_pPrs = new CXLParser(m_pXL);
#if defined(_DEBUG)
	m_pXL->SetVisible(TRUE);
#endif
	if (CTestEnvMgr::IsTestEnvST(_T("ExcelVisible"), _T("yes")))
		m_pXL->SetVisible(TRUE);
	else
		m_pXL->SetVisible(FALSE);
	
	/*
	//////////////////////////////////////////////////////////////
	//수식 저장한 엑셀 파일 열기
	STARTUPINFOA* sInfo = new STARTUPINFOA;
	GetStartupInfo(sInfo);
	CString sStartUp = sInfo->lpTitle;
	delete sInfo;
	
		CStringArray arPath;
		m_pPrs->SplitString(sStartUp, _T("\\"), arPath);
		
		CString sStartUpPath;
		for(int i=0; i<arPath.GetSize()-1; i++)
		{
		sStartUpPath += arPath[i];
		sStartUpPath += _T("\\");
		}
	*/
	int i;
	CString sStartUpPath = m_pDoc->GetDocPoint()->GetProgramPath() + _T("Excel Base File\\");
	CString sEquation = sStartUpPath + _T("Equation.dll");
	
	CFileCtrl fc(sEquation);
	if (!fc.FileExists())
	{
		AfxMessageBox(_T("Can't find Equation.dll!"));
		return FALSE;
	}
	
	if(m_PscdD.bStructPrint[2] || m_PscdD.bStructPrint[3] || m_PscdD.bStructPrint[4])
	{
		m_pXL->Open(sEquation);
	}
	
	CString sCopyBookName = m_pXL->GetBookName();
	
	m_pXL->AddBook();
	
	//bDgnPrint[5]-> 0:응력해석결과, 1:시공단계별 응력도, 2:사용하중시 하중조합별 응력도, 3:응력검토 집계 결과 4:인장철근량계산.
	//bStructPrint[5]-> 0:계수하중 조합 단면력도, 1:하중조합별 부재력 집계, 2:휨강도 검토, 3:전단강도 검토, 4:비틀림강도 검토
	BOOL bExcel = TRUE;
	BOOL bXlPrint = TRUE;
	
	// bDgnPrint[0] : 응력해석결과
	//!/ 나중에 풀어야함.
	//if(m_PscdD.bDgnPrint[0])  { if(!XL_StressAnalysisResult()) { bXlPrint = FALSE; } }      
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	// bDgnPrint[1] : 시공단계별 응력도
	//GSaveHistory(_T("XL_ConsStageStressDiagram"));
	if(m_PscdD.bDgnPrint[1])  { if(!XL_ConsStageStressDiagram()) { bXlPrint = FALSE; } }    
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	// bDgnPrint[2] : 사용하중시 하중조합별 응력도
	if(m_PscdD.bDgnPrint[2])  { if(!XL_ServLcomStressDiagram()) { bXlPrint = FALSE; } }     
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	// bDgnPrint[3] : 응력검토 집계 결과
	if(m_PscdD.bDgnPrint[3])  { if(!XL_StressCollectResult()) { bXlPrint = FALSE; } }       
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	//bStructPrint[0] : 하중조합별 부재력도
	if(m_PscdD.bStructPrint[0])  { if(!XL_UltiLoadDiagram()) { bXlPrint = FALSE; } }        
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	//bStructPrint[1] : 1:하중조합별 부재력 집계
	if(m_PscdD.bStructPrint[1])  { if(!XL_LcomForceCollect()) { bXlPrint = FALSE; } }       
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	//bStructPrint[2] : 휨강도 검토
	if(m_PscdD.bStructPrint[2]) 
	{    
		if(!XL_FlexStrengthCalc(sCopyBookName)) { bXlPrint = FALSE; }  //계산서는 선택된 요소만 프린트.
		if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
		if(!XL_FlexStrengthTable()) { bXlPrint = FALSE; } 
		if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
		if(!XL_PosiNegaMomentDiagram()) { bXlPrint = FALSE; } 		
		if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	}
	
	//bStructPrint[3] : 전단강도 검토
	if(m_PscdD.bStructPrint[3])
	{
		if(!XL_ShearStrengthCalc(sCopyBookName)) { bXlPrint = FALSE; }
		if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
		if(!XL_ShearStrengthTable()) { bXlPrint = FALSE; }	
		if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	}
	
	//bStructPrint[4] : 비틀림강도 검토
	if(m_PscdD.bStructPrint[4])
	{
		if(!XL_TorsStrengthCalc(sCopyBookName)) { bXlPrint = FALSE; }	
		if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	} 
	
	// bDgnPrint[4] : 인장철근량계산.
	if(m_PscdD.bDgnPrint[4])  { if(!XL_ReinforcementCalc(sCopyBookName)) { bXlPrint = FALSE; } }       
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	BOOL bDelSheet = FALSE;
	for(i=0; i<5; i++ )
	{
		if(m_PscdD.bDgnPrint[i]) bDelSheet = TRUE;
		if(m_PscdD.bStructPrint[i]) bDelSheet = TRUE;
	}
	
	if(bDelSheet && m_iCountSheet!=0)
	{
		if (m_pXL->IsExistSheetName(_T("Sheet1"))) { m_pXL->DeleteSheet(_T("Sheet1")); }
		if (m_pXL->IsExistSheetName(_T("Sheet2"))) { m_pXL->DeleteSheet(_T("Sheet2")); }
		if (m_pXL->IsExistSheetName(_T("Sheet3"))) { m_pXL->DeleteSheet(_T("Sheet3")); }
	}
	
	Progress(0, 100);
	ProgressText(1, _T("PSC 계산서 완료..."));
	Progress(2, 100);
	
	CString sBookName = m_pXL->GetBookName();
	CString sFileName = GetSaveFileName();
	m_pXL->SaveAs(sBookName, sFileName);
	m_pXL->Quit();
	delete m_pXL;
	delete m_pPrs;
	
// 	CString sMsg;
// 	sMsg.Format(_T("Save As %s"), sFileName);
// 	AfxMessageBox(sMsg);
	
	ShellExecute(NULL, _T("open"), (sFileName), NULL, NULL, SW_SHOWNORMAL);		
	
	return bExcel;
}


//EXCEL Work..
BOOL CCRCExcelOutput::XL_PscReport_KSCE10()
{  
	int nOutType;
	BOOL bRunMS, bRowMark, bColumnMark;
	CProduct::GetTestOutType(nOutType, bRunMS, bRowMark, bColumnMark);
	m_pXL = new CMSExcel(nOutType, bRunMS, bRowMark, bColumnMark);
	m_pPrs = new CXLParser(m_pXL);
#if defined(_DEBUG)
	m_pXL->SetVisible(TRUE);
#endif
	if (CTestEnvMgr::IsTestEnvST(_T("ExcelVisible"), _T("yes")))
		m_pXL->SetVisible(TRUE);
	else
		m_pXL->SetVisible(FALSE);

		int i;
	CString sStartUpPath = m_pDoc->GetDocPoint()->GetProgramPath() + _T("Excel Base File\\");
	//CString sEquation = sStartUpPath + _T("PSC_KSCE_10_base_ver01.dll");
	m_strBaseBook = _T("PSC_KSCE_10_base_ver01.dll");

	CFileCtrl fc(sStartUpPath +m_strBaseBook);
	if (!fc.FileExists())
	{
		AfxMessageBox(_T("Can't find PSC_KSCE_10_base_ver01.dll!"));
		return FALSE;
	}
	
	if(m_PscdD.bStructPrint[2] || m_PscdD.bStructPrint[3] || m_PscdD.bStructPrint[4])
	{
		m_pXL->Open(sStartUpPath +m_strBaseBook);
	}
	
	CString sCopyBookName = m_pXL->GetBookName();
	
	m_pXL->AddBook();
	
	//bDgnPrint[5]-> 0:응력해석결과, 1:시공단계별 응력도, 2:사용하중시 하중조합별 응력도, 3:응력검토 집계 결과 4:인장철근량계산.
	//bStructPrint[5]-> 0:계수하중 조합 단면력도, 1:하중조합별 부재력 집계, 2:휨강도 검토, 3:전단강도 검토, 4:비틀림강도 검토
	BOOL bExcel = TRUE;
	BOOL bXlPrint = TRUE;
	
	// bDgnPrint[0] : 응력해석결과
	//!/ 나중에 풀어야함.
	//if(m_PscdD.bDgnPrint[0])  { if(!XL_StressAnalysisResult()) { bXlPrint = FALSE; } }      
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	// bDgnPrint[1] : 시공단계별 응력도
	//GSaveHistory(_T("XL_ConsStageStressDiagram"));
	if(m_PscdD.bDgnPrint[1])  { if(!XL_ConsStageStressDiagram()) { bXlPrint = FALSE; } }    
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	// bDgnPrint[2] : 사용하중시 하중조합별 응력도
	if(m_PscdD.bDgnPrint[2])  { if(!XL_ServLcomStressDiagram()) { bXlPrint = FALSE; } }     
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	// bDgnPrint[3] : 응력검토 집계 결과
	if(m_PscdD.bDgnPrint[3])  { if(!XL_StressCollectResult()) { bXlPrint = FALSE; } }       
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	//bStructPrint[0] : 하중조합별 부재력도
	if(m_PscdD.bStructPrint[0])  { if(!XL_UltiLoadDiagram()) { bXlPrint = FALSE; } }        
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	//bStructPrint[1] : 1:하중조합별 부재력 집계
	if(m_PscdD.bStructPrint[1])  { if(!XL_LcomForceCollect()) { bXlPrint = FALSE; } }       
	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	
	//bStructPrint[2] : 휨강도 검토
	if(m_PscdD.bStructPrint[2]) 
	{    
		if(!XL_FlexStrengthCalc_KSCE10(sCopyBookName)) { bXlPrint = FALSE; }  //계산서-이론, 검토.
		if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
		if(!XL_FlexStrengthTable_KSCE10()) { bXlPrint = FALSE; }    // table
		if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
		if(!XL_PosiNegaMomentDiagram()) { bXlPrint = FALSE; } 		
		if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	}
	
	//bStructPrint[3] : 전단강도 검토
	if(m_PscdD.bStructPrint[3])
	{
		if(!XL_ShearStrengthCalc_KSCE10(sCopyBookName)) { bXlPrint = FALSE; }
		if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
		if(!XL_ShearStrengthTable_KSCE10()) { bXlPrint = FALSE; }	
		if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	}
	
	//bStructPrint[4] : 비틀림강도 검토
	if(m_PscdD.bStructPrint[4])
	{
		if(!XL_TorsStrengthCalc_KSCE10(sCopyBookName)) { bXlPrint = FALSE; }	
		if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
	} 
	
	// bDgnPrint[4] : 인장철근량계산.
// 	if(m_PscdD.bDgnPrint[4])  { if(!XL_ReinforcementCalc(sCopyBookName)) { bXlPrint = FALSE; } }       
// 	if(m_bStopExecute) { m_pXL->Quit(); return FALSE; }
// 	
	BOOL bDelSheet = FALSE;
	for(i=0; i<4; i++ )   //5
	{
		if(m_PscdD.bDgnPrint[i]) bDelSheet = TRUE;
		if(m_PscdD.bStructPrint[i]) bDelSheet = TRUE;
	}
	
	if(bDelSheet && m_iCountSheet!=0)
	{
		if (m_pXL->IsExistSheetName(_T("Sheet1"))) { m_pXL->DeleteSheet(_T("Sheet1")); }
		if (m_pXL->IsExistSheetName(_T("Sheet2"))) { m_pXL->DeleteSheet(_T("Sheet2")); }
		if (m_pXL->IsExistSheetName(_T("Sheet3"))) { m_pXL->DeleteSheet(_T("Sheet3")); }
	}
	
	Progress(0, 100);
	ProgressText(1, _T("PSC 계산서 완료..."));
	Progress(2, 100);
	
	CString sBookName = m_pXL->GetBookName();
	CString sFileName = GetSaveFileName();

	m_pXL->SaveAs(sBookName, sFileName);
	m_pXL->Quit();
	delete m_pXL;
	delete m_pPrs;
	
// 	CString sMsg;
// 	sMsg.Format(_T("Save As %s"), sFileName);
// 	AfxMessageBox(sMsg);
	
	ShellExecute(NULL, _T("open"), (sFileName), NULL, NULL, SW_SHOWNORMAL);		
 
	return bExcel;
}



CString CCRCExcelOutput::GetSaveFileName()
{
	CString strFileName=m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strFileName);
	CString strSaveFileName;
	if(m_PscdD.iDgnCode==SNiP_20503_84_PSC || m_PscdD.iDgnCode==SP_35_13330_11_PSC || m_PscdD.iDgnCode==SNiP_20503_84_PSC_MKS || m_PscdD.iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		strSaveFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".")+CMSOfficeMgr::GetExtensionNameExcel();
	}
	else
	{
		//strSaveFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".")+CMSOfficeMgr::GetExtensionNameExcel();
		strSaveFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".xls");
	}
	CFileCtrl* saveFileCtrl = new CFileCtrl(strSaveFileName);
	
	int count  = 0;
	while(saveFileCtrl->FileExists())
	{
		CString sCount;
		sCount.Format(_T("%d"), ++count);

		CString strSaveFileName;
		if(m_PscdD.iDgnCode==SNiP_20503_84_PSC || m_PscdD.iDgnCode==SP_35_13330_11_PSC || m_PscdD.iDgnCode==SNiP_20503_84_PSC_MKS || m_PscdD.iDgnCode==SP_35_13330_11_PSC_MKS)
		{
			strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + sCount + _T(".") + CMSOfficeMgr::GetExtensionNameExcel();
		}
		else
		{
			//strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + sCount + _T(".") + CMSOfficeMgr::GetExtensionNameExcel();
			strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + sCount + _T(".xls");
		}
		
		delete saveFileCtrl;
		saveFileCtrl = new CFileCtrl(strSaveFileName);
	}
	
	delete saveFileCtrl;
	
	return strSaveFileName;
}

CString CCRCExcelOutput::GetSaveFileNameEx(BOOL bWithoutExtension /* = FALSE*/)
{
	CString strFileName=m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strFileName);

	CString strSaveFileName;
	if(m_PscdD.iDgnCode==SNiP_20503_84_PSC || m_PscdD.iDgnCode==SP_35_13330_11_PSC || m_PscdD.iDgnCode==SNiP_20503_84_PSC_MKS || m_PscdD.iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		strSaveFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".")+CMSOfficeMgr::GetExtensionNameExcel();
	}
	else
	{
	    strSaveFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".")+CMSOfficeMgr::GetExtensionNameExcel();
	  //strSaveFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".xls");
	}
	
	CFileCtrl* saveFileCtrl = new CFileCtrl(strSaveFileName);
	
	int count  = 0;
	while(saveFileCtrl->FileExists())
	{
		CString sCount;
		sCount.Format(_T("%d"), ++count);

		CString strSaveFileName;
		if(m_PscdD.iDgnCode==SNiP_20503_84_PSC || m_PscdD.iDgnCode==SP_35_13330_11_PSC || m_PscdD.iDgnCode==SNiP_20503_84_PSC_MKS || m_PscdD.iDgnCode==SP_35_13330_11_PSC_MKS)
		{
			strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + sCount + _T(".") + CMSOfficeMgr::GetExtensionNameExcel();
		}
		else
		{
			strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + sCount + _T(".") + CMSOfficeMgr::GetExtensionNameExcel();
		  //strSaveFileName = FileCtrl.GetFilePathAndNameWithoutExtension() + sCount + _T(".xls");
		}

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

CString CCRCExcelOutput::GetSaveFileDirEx()
{
	CString strFileName=m_pDoc->GetPathName();
	
	CFileCtrl FileCtrl(strFileName);
	
	return FileCtrl.GetFilePath();
}

//응력해석 결과..
BOOL CCRCExcelOutput::XL_StressAnalysisResult()
{ 
	Progress(0, 0);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_STRESS_RESULT));	
	
	CString strTitle = _T("응력해석결과");
	CArray<Prt_ArString,Prt_ArString&> arStrChk; arStrChk.RemoveAll();
	if(!m_pPrtKSCE->Get_ForceDetlXLData(strTitle, arStrChk)) return FALSE;  
	if(m_bStopExecute) return FALSE;
	double dSize = arStrChk.GetSize();
	double dPercent=0.;
	double dIncrement= (dSize==0.0 ? 0.0 : 10./dSize);
	
	int iMaxElement = 100;
		
	int i,j;
	int iMaxRows=0;
	int iTotalElem = arStrChk.GetSize();
	int iSheetCount = iTotalElem/iMaxElement + 1; // Element 100개 기준.
	
	double dmod = fmod((double)iTotalElem, (double)iMaxElement);
	int imod = (int)dmod;    
	for(i=0; i<iSheetCount; i++)
	{
		CArray<Prt_ArString,Prt_ArString&> ArrStrChk;
		ArrStrChk.RemoveAll();
		int iNumber = (iTotalElem-iMaxElement*(i+1)<0.0 ? imod : iMaxElement); 
		ArrStrChk.SetSize(iNumber);
		int k=0;
		for(j=i*iMaxElement; j<(i*iMaxElement+iNumber); j++)
		{
			ArrStrChk.SetAt(k,arStrChk.GetAt(j));
			k++;
		}       
		
		iMaxRows=0;
		for(j=0; j<ArrStrChk.GetSize(); j++)
		{
			CArray<CString,CString> arString;
			arString.Copy(ArrStrChk.GetAt(j).arString);
			iMaxRows += arString.GetSize();    
		}
		m_iMaxRows = iMaxRows + 100;
		CString strNumber; 
		CString strTitle2;
		strNumber.Format(_T("%d"),i+1);
		strTitle2 = strTitle + strNumber;
		if(iSheetCount==1) m_pXL->AddSheet2(strTitle);
		else               m_pXL->AddSheet2(strTitle2);
		m_iCountSheet++;
		m_pXL->SetColumnWidth(0, 0, 0, 100, (double)9.3);
		m_pXL->SetColumnWidth(0, 0, 0, 0, (double)5.0);
		m_pXL->SetColumnWidth(0, 1, 0, 1, (double)16.0);
		m_pXL->SetFont(0,0, m_iMaxRows, m_iMaxCols, 8, 1, m_strChrType, FALSE);
		m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, m_iMaxCols);
		if(m_bStopExecute) return FALSE;
		
		int nRow = 0;
		int nEndRow = 0;
		for(j=0; j<ArrStrChk.GetSize(); j++)
		{
			if(m_bStopExecute) return FALSE;
			CArray<CString,CString> arString;
			arString.Copy(ArrStrChk.GetAt(j).arString);
			dPercent += dIncrement;
			Progress(0, dPercent);
			Progress(2, dPercent);      
			
			m_pPrs->WriteTextTable(nRow, 0, nEndRow, arString, _T(" "));   
			nRow = nEndRow;        
		}  
		
		m_pXL->SetHPageBreak(37,nEndRow-1,6);
		if(m_bStopExecute) return FALSE;

		BOOL m_bRegression = CLockMgr::Get_IsRegressionTest();
		if(m_bRegression) 
		{
			// m_pXL->SetPageNormalView(FALSE);
		}
		else 
		{
			m_pXL->SetPageNormalView(TRUE);
		}
		
		if(m_bStopExecute) return FALSE;
	}
	
	return TRUE;
}

//시공단계별 응력도..
BOOL CCRCExcelOutput::XL_ConsStageStressDiagram()
{ 
	Progress(0, 10);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_CONST_STRESS_DIAGRAM));
	Progress(2, 10);
	
	CArray<CString,CString> arStrTitle; arStrTitle.RemoveAll();
	CArray<CString,CString> arStrCol;   arStrCol.RemoveAll();  
	CArray<double,double> arValue;     arValue.RemoveAll();
	CArray<ChartData,ChartData> StageData; StageData.RemoveAll();
	CArray<Prt_Graph_Data,Prt_Graph_Data&> arGraphData; arGraphData.RemoveAll();
	if(!m_pPrtKSCE->Get_ChartForceStage(arGraphData)) return FALSE;
	if(m_bStopExecute) return FALSE;
		
	double dSize = arGraphData.GetSize();
	double dPercent=10.;
	double dIncrement= (dSize==0.0 ? 0.0 : 10./dSize);
	
	int i,j;
	int iMaxRows=0;
	int iSheetCount=0;
	int iCurr=1;
	CArray<int,int> arRows; arRows.RemoveAll();
	for(i=0; i<arGraphData.GetSize(); i++)
	{
		int iSheet = arGraphData.GetAt(i).iCurrSheet;
		iSheetCount = (iSheetCount<iSheet ? iSheet : iSheetCount);
	}
	for(i=0; i<iSheetCount; i++)
	{
		iMaxRows=0;
		for(j=0;j<arGraphData.GetSize(); j++)
		{
			if(i+1==arGraphData.GetAt(j).iCurrSheet)
				iMaxRows += arGraphData.GetAt(i).arValue.GetSize()/10+4;      
		}
		arRows.Add(iMaxRows);
	}
	
	m_iMaxRows = arRows.GetAt(0);
	CString strTitle = _T("시공단계별 응력도");
	CString strTitle2= _T("시공단계별 응력도1");
	CString strSheetTitle = _T("");
	
	if(iSheetCount==1)
	{
		m_pXL->AddSheet2(strTitle);
		strSheetTitle = strTitle;
	}
	else
	{
		m_pXL->AddSheet2(strTitle2);
		strSheetTitle = strTitle2;
	}
	m_iCountSheet++;
	m_pXL->SetColumnWidth(0, 0, 0, 100, (double)6.0);
	m_pXL->SetFont(0,0, m_iMaxRows, m_iMaxCols, 8, 1, m_strChrType, FALSE);
	m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, m_iMaxCols);
	
	iCurr=1;
	int iChartP=0;
	for(i=0; i<arGraphData.GetSize(); i++)
	{
		arStrTitle.RemoveAll();
		arStrCol.RemoveAll();  
		arValue.RemoveAll();
		StageData.RemoveAll();
		
		int iSheet = arGraphData.GetAt(i).iCurrSheet;
		if(iCurr!=iSheet)
		{      
			iCurr=iSheet;
			iChartP=0;
			m_iMaxRows = arRows.GetAt(iSheet-1);
			strTitle2.Format(_T("%s%d"),strTitle,iSheet);
			strSheetTitle = strTitle2;
			m_pXL->AddSheet2(strTitle2);
			m_iCountSheet++;
			m_pXL->SetColumnWidth(0, 0, 0, 100, (double)6.0);
			m_pXL->SetFont(0,0, m_iMaxRows, m_iMaxCols, 8, 1, m_strChrType, FALSE);
			m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, m_iMaxCols);
		}
		
		dPercent += dIncrement;
		Progress(0, dPercent);
		Progress(2, dPercent);
		
		Prt_Graph_Data GraphData; GraphData.Initialize();
		GraphData = arGraphData.GetAt(i);
		arStrTitle.Copy(GraphData.arStrTitle);
		int iCount = arStrTitle.GetSize()/2;
		for(j=0; j<iCount; j++)
		{
			if(m_bStopExecute) return FALSE;
			m_pXL->SetValue(arStrTitle.GetAt(j*2),arStrTitle.GetAt(j*2+1));
		}
		
		arStrCol.Copy(GraphData.arStrCol);
		arValue.Copy(GraphData.arValue);
		for(j=0; j<arStrCol.GetSize(); j++)
		{
			if(m_bStopExecute) return FALSE;
			m_pXL->SetValue(arStrCol.GetAt(j),arValue.GetAt(j));
		}
		
		double dWidth = 480.0;//500.0;
		double dHeight = 300.0;
		StageData.Copy(GraphData.arChartData);
		if(m_bStopExecute) return FALSE;
		for(j=0; j<StageData.GetSize(); j++)
		{
			int k = StageData.GetSize();
			m_pXL->AddChart(StageData.GetAt(j), (iChartP*k+j)*25, 0, dWidth, dHeight, strSheetTitle);
		}    
		iChartP++;
	}
	
	return TRUE;
}

//사용하중시 하중조합별 응력도..
BOOL CCRCExcelOutput::XL_ServLcomStressDiagram()
{ 
	Progress(0, 20);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_SERVICELC_STRESS_DIAGRAM));
	Progress(2, 20);
	
	CArray<CString,CString> arStrTitle; 
	CArray<CString,CString> arStrCol;   
	CArray<double,double> arValue;     
	CArray<ChartData,ChartData> LcomData; 
	CArray<Prt_Graph_Data,Prt_Graph_Data&> arGraphData; arGraphData.RemoveAll();
	if(!m_pPrtKSCE->Get_ChartForceLcom(arGraphData)) return FALSE;
	if(m_bStopExecute) return FALSE;
	
	int i,j;
	int iMaxRows=0;
	int iSheetCount=0;  
	CArray<int,int> arRows; arRows.RemoveAll();
	for(i=0; i<arGraphData.GetSize(); i++)
	{
		int iSheet = arGraphData.GetAt(i).iCurrSheet;
		iSheetCount = (iSheetCount<iSheet ? iSheet : iSheetCount);
	}
	for(i=0; i<iSheetCount; i++)
	{
		iMaxRows=0;
		for(j=0;j<arGraphData.GetSize(); j++)
		{
			if(i+1==arGraphData.GetAt(j).iCurrSheet)
				iMaxRows += arGraphData.GetAt(i).arValue.GetSize()/10+4;      
		}
		arRows.Add(iMaxRows);
	}
	
	m_iMaxRows = arRows.GetAt(0);
	CString strTitle = _T("사용하중시 하중조합별 응력도");
	CString strTitle2= _T("사용하중시 하중조합별 응력도1");
	CString strSheetTitle = _T("");  
	if(iSheetCount==1)
	{
		m_pXL->AddSheet2(strTitle);
		strSheetTitle = strTitle;
	}
	else
	{
		m_pXL->AddSheet2(strTitle2);
		strSheetTitle = strTitle2;
	}
	m_iCountSheet++;
	m_pXL->SetColumnWidth(0, 0, 0, 100, (double)6.0);
	m_pXL->SetFont(0,0, m_iMaxRows, m_iMaxCols, 8, 1, m_strChrType, FALSE);
	m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, m_iMaxCols);
	if(m_bStopExecute) return FALSE;
		
	double dSize = arGraphData.GetSize();
	double dPercent=20.;
	double dIncrement= (dSize==0.0 ? 0.0 : 10./dSize);
	
	int iCurr=1;
	int iChartP=0;
	for(i=0; i<arGraphData.GetSize(); i++)
	{
		arStrTitle.RemoveAll();
		arStrCol.RemoveAll();  
		arValue.RemoveAll();
		LcomData.RemoveAll();
		
		int iSheet = arGraphData.GetAt(i).iCurrSheet;
		if(iCurr!=iSheet)
		{
			iCurr=iSheet;
			iChartP=0;
			m_iMaxRows = arRows.GetAt(iSheet-1);
			strTitle2.Format(_T("%s%d"),strTitle,iSheet);
			strSheetTitle = strTitle2;
			m_pXL->AddSheet2(strTitle2);
			m_iCountSheet++;
			m_pXL->SetColumnWidth(0, 0, 0, 100, (double)6.0);
			m_pXL->SetFont(0,0, m_iMaxRows, m_iMaxCols, 8, 1, m_strChrType, FALSE);
			m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, m_iMaxCols);
		}
		
		dPercent += dIncrement;
		Progress(0, dPercent);
		Progress(2, dPercent);
		
		Prt_Graph_Data GraphData; GraphData.Initialize();
		GraphData = arGraphData.GetAt(i);
		arStrTitle.Copy(GraphData.arStrTitle);
		int iCount = arStrTitle.GetSize()/2;
		for(j=0; j<iCount; j++)
		{
			if(m_bStopExecute) return FALSE;
			m_pXL->SetValue(arStrTitle.GetAt(j*2),arStrTitle.GetAt(j*2+1));
		}
		
		arStrCol.Copy(GraphData.arStrCol);
		arValue.Copy(GraphData.arValue);
		for(j=0; j<arStrCol.GetSize(); j++)
		{
			if(m_bStopExecute) return FALSE;
			m_pXL->SetValue(arStrCol.GetAt(j),arValue.GetAt(j));
		}
		
		double dWidth = 480;//500.0;
		double dHeight = 300.0;
		LcomData.Copy(GraphData.arChartData);
		for(j=0; j<LcomData.GetSize(); j++)
		{
			int k = LcomData.GetSize();
			if(m_bStopExecute) return FALSE;
			m_pXL->AddChart(LcomData.GetAt(j), (iChartP*k+j)*25, 0, dWidth, dHeight, strSheetTitle);
		}
		iChartP++;    
	}
	
	return TRUE;
}

// 응력검토 집계 결과..
BOOL CCRCExcelOutput::XL_StressCollectResult()
{
	Progress(0, 30);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_STRESS_ASSEMBLE));
	Progress(2, 30);
	
	CArray<CString,CString> arStrStress; arStrStress.RemoveAll();
	if(!m_pPrtKSCE->Get_ForceTotalXLData(arStrStress)) return FALSE; 
	if(m_bStopExecute) return FALSE;  
	
	m_iMaxRows = arStrStress.GetSize()+50;
	m_pXL->AddSheet2(_T("응력 검토 집계 결과"));
	m_iCountSheet++;
	m_pXL->SetColumnWidth(0, 0, 0, 100, (double)6.0);
	m_pXL->SetFont(0,0, m_iMaxRows, 100, 8, 1, m_strChrType, FALSE);
	m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, 100);
	if(m_bStopExecute) return FALSE;
	
	Progress(0, 40);
	Progress(2, 40);
	
	int nRow = 0;
	int nEndRow =0;
	BOOL bSum = FALSE;
	m_pPrs->WriteTable(nRow, 1, nEndRow, arStrStress, _T(" "), bSum);
	
	return TRUE;
}


// ------------------------------------------------
// 계수하중시 검토

// 하중조합별 부재력도.
BOOL CCRCExcelOutput::XL_UltiLoadDiagram()
{
	Progress(0, 40);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_LC_MF_DIAGRAM));
	Progress(2, 40);
	
	CArray<CString,CString> arStrTitle; 
	CArray<CString,CString> arStrCol;   
	CArray<double,double>   arValue;     
	CArray<ChartData,ChartData> arForcDiag;
	CArray<Prt_Graph_Data,Prt_Graph_Data&> arGraphData; arGraphData.RemoveAll();
	if(!m_pPrtKSCE->Get_ChartForceDiagram(arGraphData)) return FALSE;
	if(m_bStopExecute) return FALSE;
	
	int i,j;
	int iMaxRows=0;
	for(i=0; i<arGraphData.GetSize(); i++)
	{
		iMaxRows += arGraphData.GetAt(i).arValue.GetSize()/12;
	}
	m_iMaxRows = iMaxRows+100;
	
	CString strTitle = _T("계수하중 조합 단면력도");
	m_pXL->AddSheet2(strTitle);
	m_iCountSheet++;
	m_pXL->SetColumnWidth(0, 0, 0, 100, (double)6.);
	m_pXL->SetFont(0,0, m_iMaxRows, m_iMaxCols, 8, 1, m_strChrType, FALSE);
	m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, m_iMaxCols);
	if(m_bStopExecute) return FALSE;
	
	double dSize = arGraphData.GetSize();
	double dPercent=40.;
	double dIncrement= (dSize==0.0 ? 0.0 : 10./dSize);
	
	for(i=0; i<arGraphData.GetSize(); i++)
	{
		arStrTitle.RemoveAll();
		arStrCol.RemoveAll();  
		arValue.RemoveAll();
		arForcDiag.RemoveAll();
		
		dPercent += dIncrement;
		Progress(0, dPercent);
		Progress(2, dPercent);
		
		Prt_Graph_Data GraphData; GraphData.Initialize();
		GraphData = arGraphData.GetAt(i);
		arStrTitle.Copy(GraphData.arStrTitle);
		int iCount = arStrTitle.GetSize()/2;
		for(j=0; j<iCount; j++)
		{
			if(m_bStopExecute) return FALSE;
			m_pXL->SetValue(arStrTitle.GetAt(j*2),arStrTitle.GetAt(j*2+1));
		}
		
		arStrCol.Copy(GraphData.arStrCol);
		arValue.Copy(GraphData.arValue);
		for(j=0; j<arStrCol.GetSize(); j++)
		{
			if(m_bStopExecute) return FALSE;
			m_pXL->SetValue(arStrCol.GetAt(j),arValue.GetAt(j));
		}
		
		double dWidth = 480;//500.0;
		double dHeight = 300.0;
		arForcDiag.Copy(GraphData.arChartData);
		for(j=0; j<arForcDiag.GetSize(); j++)
		{
			int k = arForcDiag.GetSize();
			if(m_bStopExecute) return FALSE;
			m_pXL->AddChart(arForcDiag.GetAt(j), (i*k+j)*25, 0, dWidth, dHeight, strTitle);
		}
		
	}
	
	return TRUE;
}

//하중별 단면력 집계.
BOOL CCRCExcelOutput::XL_LcomForceCollect()
{
	Progress(0, 50);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_LC_MF_ASSEMBLE));
	Progress(2, 50);
	
	CArray<Prt_ArString,Prt_ArString&> arStrUlti; arStrUlti.RemoveAll();
	CArray<CString,CString> arUltiData; 
	if(!m_pPrtKSCE->Get_UltiLoadXLData(arStrUlti)) return FALSE;
	if(m_bStopExecute) return FALSE;
	
	int i;
	int iMaxRows=0;
	for(i=0; i<arStrUlti.GetSize(); i++)
	{
		iMaxRows += arStrUlti.GetAt(i).arString.GetSize()+3;
	}
	m_iMaxRows = iMaxRows+100;
	
	double dSize = arStrUlti.GetSize();
	double dPercent=50.;
	double dIncrement= (dSize==0 ? 0 : 10./dSize);
	
	CString strTitle = _T("하중별 단면력 집계");
	m_pXL->AddSheet2(strTitle);
	m_iCountSheet++;
	m_pXL->SetColumnWidth(0, 0, 0, 100, (double)7.7);
	m_pXL->SetColumnWidth(0, 0, 0, 2, (double)5.4);  // 5.4
	m_pXL->SetFont(0,0, m_iMaxRows, m_iMaxCols, 8, 1, m_strChrType, FALSE);
	m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, m_iMaxCols);
	if(m_bStopExecute) return FALSE;
	
	int nRow=0;
	int nEndRow=0;
	for(i=0; i<arStrUlti.GetSize(); i++)
	{
		if(m_bStopExecute) return FALSE;
		arUltiData.RemoveAll();
		arUltiData.Copy(arStrUlti.GetAt(i).arString);
		
		dPercent += dIncrement;
		Progress(0, dPercent);
		Progress(2, dPercent);
		m_pPrs->WriteTextTable(nRow, 0, nEndRow, arUltiData, _T(" "));  
		nRow = nEndRow;    
	}  
		
	m_pXL->SetHPageBreak(51,nEndRow-1,8);
	if(m_bStopExecute) return FALSE;

	BOOL bRegression = CLockMgr::Get_IsRegressionTest();
	if(bRegression) 
	{
		// m_pXL->SetPageNormalView(FALSE);
	}
	else 
	{
		m_pXL->SetPageNormalView(TRUE);
	}
	
	if(m_bStopExecute) return FALSE;
	
	return TRUE;
}

//휨강도 검토.
BOOL CCRCExcelOutput::XL_FlexStrengthCalc(CString sCopyBookName)
{
	Progress(0, 60);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_BENDING_CHECK));
	Progress(2, 60);
	
	CArray<CString,CString> arTheory;    arTheory.RemoveAll();
	CArray<CString,CString> arFlexStre;  arFlexStre.RemoveAll();
	int iCountFormula=0; 
	CArray<Prt_ArString_Detail,Prt_ArString_Detail> arStrDetail;
	if(!m_pPrtKSCE->Get_FlexStrengthCalc(arTheory, arStrDetail)) return FALSE;
	if(m_bStopExecute) return FALSE;
	
	CString strTitle = _T("휨강도 검토(이론)");
	m_pXL->AddSheet2(strTitle);
	m_iCountSheet++;
	m_pXL->SetActiveSheet(strTitle);
	m_pXL->SetColumnWidth(0, 0, 0, 100, (double)0.6);
	m_pXL->SetFont(0,0, 300, 100, 8, 1, m_strChrType, FALSE);
	m_pXL->SetColumnInTextFormat(0,0, 300, 100);
	if(m_bStopExecute) return FALSE;
		
	int nRow=0;
	int nEndRow=0;
	m_pPrs->WriteExpression(nRow, 0, nEndRow, 66, arTheory, _T(" "), sCopyBookName, _T("Sheet1"));  
	if(m_bStopExecute) return FALSE;
	
	int j;
	int iMaxRows=0;
	double dSize = arStrDetail.GetSize();
	double dPercent=60.;
	double dIncrement= (dSize==0.0 ? 0.0 : 10./dSize);
	
	strTitle = _T("휨강도 검토(구조계산)");
	
	int iMaxCount = 100;    
	int iTotalCount = arStrDetail.GetSize();
	int iSheetCount = iTotalCount/iMaxCount + 1; 
	
	double dTotalCount = iTotalCount*1.0;
	double dMaxCount   = iMaxCount*1.0;
	double dmod = fmod((double)iTotalCount, (double)iMaxCount);
	int imod = (int)dmod;    


	for(int i=0; i<iSheetCount; i++)
	{
		CArray<Prt_ArString_Detail,Prt_ArString_Detail> ArrStrDetail;
		ArrStrDetail.RemoveAll();

		int iNumber = (iTotalCount-iMaxCount*(i+1)<0.0 ? imod : iMaxCount); 
		ArrStrDetail.SetSize(iNumber);
		int k=0;
		for(j=i*iMaxCount; j<(i*iMaxCount+iNumber); j++)
		{
			ArrStrDetail.SetAt(k,arStrDetail.GetAt(j));
			k++;
		} 

		iMaxRows = 0;
		for(j=0; j<ArrStrDetail.GetSize(); j++)
		{ 
			iMaxRows += ArrStrDetail.GetAt(j).ArString.arString.GetSize() + ArrStrDetail.GetAt(j).iFormula + 3; // 3=Temp
		}
		m_iMaxRows = iMaxRows+10;
		
		CString strNumber; 
		CString strTitle2;

		strNumber.Format(_T("%d"),i+1);
		strTitle2 = strTitle + strNumber;
		if(iSheetCount==1) m_pXL->AddSheet2(strTitle);
		else               m_pXL->AddSheet2(strTitle2);
				
		m_iCountSheet++;
		m_pXL->SetColumnWidth(0, 0, 0, 100, (double)0.6);
		m_pXL->SetFont(0,0, m_iMaxRows, m_iMaxCols, 8, 1, m_strChrType, FALSE);
		m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, m_iMaxCols);
		if(m_bStopExecute) return FALSE;
		
		nRow=0;
		nEndRow=0;
		for(j=0; j<ArrStrDetail.GetSize(); j++)
		{
			if(m_bStopExecute) return FALSE;
			arFlexStre.RemoveAll();
			arFlexStre.Copy(ArrStrDetail.GetAt(j).ArString.arString);
			dPercent += dIncrement;
			Progress(0, dPercent);
			Progress(2, dPercent);
			m_pPrs->WriteExpression(nRow, 0, nEndRow, 66, arFlexStre, _T(" "), sCopyBookName, _T("Sheet1"));
			nRow = nEndRow;
		}
	}
	
	return TRUE;
}


BOOL CCRCExcelOutput::XL_FlexStrengthCalc_KSCE10(CString sCopyBookName)
{

	Progress(0, 60);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_BENDING_CHECK));
	Progress(2, 60);

	if(m_bStopExecute) return FALSE;

	// 이론
	CString strTitle = _T("휨강도 검토(이론)");
	CString AddSheetName = strTitle; 
	m_pXL->AddSheet2(AddSheetName);
	m_pXL->SetActiveSheet(AddSheetName);
	m_pXL->SetColumnWidth(0, 0, 0, 28, 2.2);
	m_pXL->SetRowHeight(15);
	m_pXL->SetMargin(13, 12, 20, 17); 
	if(m_bStopExecute) return FALSE;
	m_pPrtKSCE10->Set_CMSExcel_KSCE10(m_pXL, m_strBaseBook, m_PscdD.iDgnCode);

	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	m_pPrtKSCE10->Set_CMSExcel_Flexure_theory(m_pXL, PscdD.ifpsMethod);
	m_iCountSheet++;

	
	// 휨 계산
	strTitle = _T("휨강도 검토(구조계산)");
	AddSheetName = strTitle; 
	m_pXL->AddSheet2(AddSheetName);
	m_pXL->SetActiveSheet(AddSheetName);
	m_pXL->SetColumnWidth(0, 0, 0, 28, 2.2);
	m_pXL->SetRowHeight(15);
	m_pXL->SetMargin(13, 12, 20, 17); 
	if(m_bStopExecute) return FALSE;

	CArray<CString,CString> arTheory;   arTheory.RemoveAll();
	CArray<CString,CString> arFlexStre; arFlexStre.RemoveAll();
	int iCountFormula=0; 
	CArray<Prt_ArString_Detail,Prt_ArString_Detail> arStrDetail;
	if(!m_pPrtKSCE->Get_FlexStrengthCalc(arTheory, arStrDetail, m_pXL)) return FALSE; // 05의 Get_FlexStrengthCalc 호출.  계산 및 출력함
	if(m_bStopExecute) return FALSE;
	m_iCountSheet++;
	
	return TRUE;
}



BOOL CCRCExcelOutput::XL_FlexStrengthTable_KSCE10()
{
	CString strTitle = _T("휨강도 검토(Table)");
	CString AddSheetName = strTitle; 
	m_pXL->AddSheet2(AddSheetName);
	m_pXL->SetActiveSheet(AddSheetName);
	m_pXL->SetColumnWidth(0, 0, 0, 17, 6.0);
	m_pXL->SetRowHeight(15);
	m_pXL->SetMargin(5, 5, 20, 17); 
	if(m_bStopExecute) return FALSE;
	m_pPrtKSCE10->Set_CMSExcel_KSCE10(m_pXL, m_strBaseBook, m_PscdD.iDgnCode);

	CArray<CString,CString> arPMomTable;  arPMomTable.RemoveAll();
	CArray<CString,CString> arNMomTable;  arNMomTable.RemoveAll();
	if(!m_pPrtKSCE->Get_FlexStrengthTable_KSCE10(arPMomTable, arNMomTable, m_pXL)) return FALSE;
	if(m_bStopExecute) return FALSE;


	return TRUE;   
}

BOOL CCRCExcelOutput::XL_FlexStrengthTable()
{
	CArray<CString,CString> arPMomTable;  arPMomTable.RemoveAll();
	CArray<CString,CString> arNMomTable;  arNMomTable.RemoveAll();
	if(!m_pPrtKSCE->Get_FlexStrengthTable(arPMomTable, arNMomTable)) return FALSE;
	if(m_bStopExecute) return FALSE;
	
	CString strTitle;
	int nRow = 0;
	int nEndRow =0;
	BOOL bSum = FALSE;
	
	m_iMaxRows = arPMomTable.GetSize() + 100;
	if(arPMomTable.GetSize()>4)
	{
		strTitle = _T("정모멘트에 대한 검토(Table)");
		m_pXL->AddSheet2(strTitle);
		m_iCountSheet++;
		m_pXL->SetColumnWidth(0, 0, 0, 100, (double)6.0);
		m_pXL->SetFont(0,0, m_iMaxRows, 100, 8, 1, m_strChrType, FALSE);
		m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, 100);
		if(m_bStopExecute) return FALSE;
		
		m_pPrs->WriteTable(nRow, 0, nEndRow, arPMomTable, _T(" "), bSum);
		if(m_bStopExecute) return FALSE;
	}
	
	m_iMaxRows = arNMomTable.GetSize() + 100;
	
	if(arNMomTable.GetSize()>4)
	{
		strTitle = _T("부모멘트에 대한 검토(Table)");
		m_pXL->AddSheet2(strTitle);
		m_iCountSheet++;
		m_pXL->SetColumnWidth(0, 0, 0, 100, (double)6.0);
		m_pXL->SetFont(0,0, m_iMaxRows, 100, 8, 1, m_strChrType, FALSE);
		m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, 100);
		if(m_bStopExecute) return FALSE;
		
		nRow = 0;
		nEndRow =0;
		m_pPrs->WriteTable(nRow, 0, nEndRow, arNMomTable, _T(" "), bSum);
		if(m_bStopExecute) return FALSE;
	}
	
	return TRUE;   
}

BOOL CCRCExcelOutput::XL_PosiNegaMomentDiagram()
{
	CArray<CString,CString> strBMD;       strBMD.RemoveAll();
	CArray<CString,CString> strCol;       strCol.RemoveAll();
	CArray<double,double> dValue;         dValue.RemoveAll();
	CArray<ChartData,ChartData> arChaBMD; arChaBMD.RemoveAll();
	if(!m_pPrtKSCE->Get_PosiNegaBMD(strBMD, strCol, dValue, arChaBMD)) return FALSE;
	if(m_bStopExecute) return FALSE;
	
	CString strTitle = _T("ΦMn-Mu그래프");
	m_pXL->AddSheet2(strTitle);
	m_iCountSheet++;
	m_pXL->SetColumnWidth(0, 0, 0, 100, (double)6.0);
	m_pXL->SetFont(0,0, 10000, 100, 8, 1, m_strChrType, FALSE);
	m_pXL->SetColumnInTextFormat(0,0, 10000, 100);
	if(m_bStopExecute) return FALSE;
	
	int iCount = strBMD.GetSize()/2;
	for(int i=0; i<iCount; i++)
	{
		if(m_bStopExecute) return FALSE;
		m_pXL->SetValue(strBMD.GetAt(i*2),strBMD.GetAt(i*2+1));
	}
	
	for(int i=0; i<strCol.GetSize(); i++)
	{
		if(m_bStopExecute) return FALSE;
		m_pXL->SetValue(strCol.GetAt(i),dValue.GetAt(i));
	}
	
	m_pXL->AddChart(_T("휨강도-부모멘트"), arChaBMD.GetAt(1));
	if(m_bStopExecute) return FALSE;
	
	m_pXL->AddChart(_T("휨강도-정모멘트"), arChaBMD.GetAt(0));
	if(m_bStopExecute) return FALSE;
	
	return TRUE;
}
//전단강도 검토.
BOOL CCRCExcelOutput::XL_ShearStrengthCalc(CString sCopyBookName)
{
	Progress(0, 70);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_SHEAR_CHECK));
	Progress(2, 70);
	
	CArray<CString,CString> arTheory;   
	CArray<CString,CString> arShearStr; arShearStr.RemoveAll();
	int iCountFormula=0;
	CArray<Prt_ArString_Detail,Prt_ArString_Detail> arStrDetail;
	if(!m_pPrtKSCE->Get_ShearStrengthCalc(arTheory, arStrDetail)) return FALSE;
	if(m_bStopExecute) return FALSE;
	
	//Theory
	CString strTitle = _T("전단강도 검토(이론)");
	m_pXL->AddSheet2(strTitle);
	m_iCountSheet++;
	m_pXL->SetColumnWidth(0, 0, 0, 100, (double)0.6);
	m_pXL->SetFont(0,0, 500, 100, 8, 1, m_strChrType, FALSE);
	m_pXL->SetColumnInTextFormat(0,0, 500, 100);
	if(m_bStopExecute) return FALSE;
	
	int nRow=0;
	int nEndRow=0;
	m_pPrs->WriteExpression(nRow, 0, nEndRow, 66, arTheory, _T(" "), sCopyBookName, _T("Sheet1"));
	if(m_bStopExecute) return FALSE;
	
	strTitle = _T("전단강도 검토(구조계산)");
	
	int i,j;
	int iMaxRows=0;
	double dSize = arStrDetail.GetSize();
	double dPercent=70.;
	double dIncrement= (dSize==0.0 ? 0.0 : 10./dSize);
	
	int iMaxCount = 100;    
	int iTotalCount = arStrDetail.GetSize();
	int iSheetCount = iTotalCount/iMaxCount + 1; 
	
	double dmod = fmod((double)iTotalCount, (double)iMaxCount);
	int imod = (int)dmod;    
	for(i=0; i<iSheetCount; i++)
	{
		CArray<Prt_ArString_Detail,Prt_ArString_Detail> ArrStrDetail;
		ArrStrDetail.RemoveAll();
		int iNumber = (iTotalCount-iMaxCount*(i+1)<0.0 ? imod : iMaxCount); 
		ArrStrDetail.SetSize(iNumber);
		int k=0;
		for(j=i*iMaxCount; j<(i*iMaxCount+iNumber); j++)
		{
			ArrStrDetail.SetAt(k,arStrDetail.GetAt(j));
			k++;
		} 
		iMaxRows = 0;
		for(j=0; j<ArrStrDetail.GetSize(); j++)
		{
			iMaxRows += ArrStrDetail.GetAt(j).ArString.arString.GetSize() + ArrStrDetail.GetAt(j).iFormula + 3; // 3=Temp
		}
		m_iMaxRows = iMaxRows+10;
		
		CString strNumber; 
		CString strTitle2;
		strNumber.Format(_T("%d"),i+1);
		strTitle2 = strTitle + strNumber;
		if(iSheetCount==1) m_pXL->AddSheet2(strTitle);
		else               m_pXL->AddSheet2(strTitle2);
				
		m_iCountSheet++;
		m_pXL->SetColumnWidth(0, 0, 0, 100, (double)0.6);
		m_pXL->SetFont(0,0, m_iMaxRows, m_iMaxCols, 8, 1, m_strChrType, FALSE);
		m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, m_iMaxCols);
		if(m_bStopExecute) return FALSE;
		
		nRow=0;
		nEndRow=0;
		for(j=0; j<ArrStrDetail.GetSize(); j++)
		{
			if(m_bStopExecute) return FALSE;
			arShearStr.RemoveAll();
			arShearStr.Copy(ArrStrDetail.GetAt(j).ArString.arString);
			dPercent += dIncrement;
			Progress(0, dPercent);
			Progress(2, dPercent);
			m_pPrs->WriteExpression(nRow, 0, nEndRow, 66, arShearStr, _T(" "), sCopyBookName, _T("Sheet1"));  
			nRow = nEndRow;
		}
	}
	
	return TRUE;
}

BOOL CCRCExcelOutput::XL_ShearStrengthCalc_KSCE10(CString sCopyBookName)
{
	Progress(0, 70);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_SHEAR_CHECK));
	Progress(2, 70);

	CString strTitle = _T("전단강도 검토(이론)");
	CString AddSheetName = strTitle; 
	m_pXL->AddSheet2(AddSheetName);
	m_pXL->SetActiveSheet(AddSheetName);
	m_pXL->SetColumnWidth(0, 0, 0, 28, 2.2);
	m_pXL->SetRowHeight(15);
	m_pXL->SetMargin(13, 12, 20, 17); 
	if(m_bStopExecute) return FALSE;
	m_pPrtKSCE10->Set_CMSExcel_KSCE10(m_pXL, m_strBaseBook, m_PscdD.iDgnCode);
	
	m_pPrtKSCE10->Set_CMSExcel_Shear_theory(m_pXL);
	m_iCountSheet++;


	strTitle = _T("전단강도 검토(구조계산)");
	AddSheetName = strTitle; 
	m_pXL->AddSheet2(AddSheetName);
	m_pXL->SetActiveSheet(AddSheetName);
	m_pXL->SetColumnWidth(0, 0, 0, 28, 2.2);
	m_pXL->SetRowHeight(15);
	m_pXL->SetMargin(13, 12, 20, 17); 
	if(m_bStopExecute) return FALSE;
	
	CArray<CString,CString> arTheory;   
	CArray<CString,CString> arShearStr; arShearStr.RemoveAll();
	int iCountFormula=0;
	CArray<Prt_ArString_Detail,Prt_ArString_Detail> arStrDetail;
	if(!m_pPrtKSCE->Get_ShearStrengthCalc(arTheory, arStrDetail)) return FALSE;
	if(m_bStopExecute) return FALSE;
	
	return TRUE;
}
//


BOOL CCRCExcelOutput::XL_ShearStrengthTable_KSCE10()
{

	CString strTitle = _T("전단강도 검토(Table)");
	CString AddSheetName = strTitle; 
	m_pXL->AddSheet2(AddSheetName);
	m_pXL->SetActiveSheet(AddSheetName);
	m_pXL->SetColumnWidth(0, 0, 0, 17, 6.0);
	m_pXL->SetRowHeight(15);
	m_pXL->SetMargin(5, 5, 20, 17); 
	if(m_bStopExecute) return FALSE;
	m_pPrtKSCE10->Set_CMSExcel_KSCE10(m_pXL, m_strBaseBook, m_PscdD.iDgnCode);
	
	CArray<CString,CString> arShearTable; arShearTable.RemoveAll();
	if(!m_pPrtKSCE->Get_ShearStrengthTable_KSCE10(arShearTable, m_pXL)) return FALSE;
	if(m_bStopExecute) return FALSE;


	return TRUE;
}

BOOL CCRCExcelOutput::XL_ShearStrengthTable()
{
	CArray<CString,CString> arShearTable; arShearTable.RemoveAll();
	if(!m_pPrtKSCE->Get_ShearStrengthTable(arShearTable)) return FALSE;
	if(m_bStopExecute) return FALSE;
	
	m_iMaxRows = arShearTable.GetSize() + 100;
	
	CString strTitle = _T("전단강도 검토(Table)");
	m_pXL->AddSheet2(strTitle);
	m_iCountSheet++;
	m_pXL->SetColumnWidth(0, 0, 0, 100, (double)6.0);
	m_pXL->SetFont(0,0, m_iMaxRows, 100, 8, 1, m_strChrType, FALSE);
	m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, 100);
	if(m_bStopExecute) return FALSE;
	
	int nRow = 0;
	int nEndRow =0;
	BOOL bSum = FALSE;
	m_pPrs->WriteTable(nRow, 0, nEndRow, arShearTable, _T(" "), bSum);
	if(m_bStopExecute) return FALSE;
	
	return TRUE;
}

//비틀림 강도 검토.
BOOL CCRCExcelOutput::XL_TorsStrengthCalc(CString sCopyBookName)
{
	Progress(0, 80);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_TORSION_CHECK));
	Progress(2, 80);
	
	CArray<CString,CString> arTheory;
	CArray<CString,CString> arTorsStr;  arTorsStr.RemoveAll();
	int iCountFormula=0;
	CArray<Prt_ArString_Detail,Prt_ArString_Detail> arStrDetail;
	if(!m_pPrtKSCE->Get_TorsStrengthCalc(arTheory, arStrDetail)) return FALSE;
	if(m_bStopExecute) return FALSE;
	
	//Theory
	CString strTitle = _T("비틀림강도 검토(이론)");
	m_pXL->AddSheet2(strTitle);
	m_iCountSheet++;
	m_pXL->SetColumnWidth(0, 0, 0, 100, (double)0.6);
	m_pXL->SetFont(0,0, 500, 100, 8, 1, m_strChrType, FALSE);
	m_pXL->SetColumnInTextFormat(0,0, 500, 100);
	if(m_bStopExecute) return FALSE;
	
	int nRow=0;
	int nEndRow=0;
	m_pPrs->WriteExpression(nRow, 0, nEndRow, 66, arTheory, _T(" "), sCopyBookName, _T("Sheet1"));
	if(m_bStopExecute) return FALSE;
	
	strTitle = _T("비틀림강도 검토(구조계산)");
	
	int i,j;
	int iMaxRows=0;
	double dSize = arStrDetail.GetSize();
	double dPercent=80.;
	double dIncrement= (dSize==0.0 ? 0.0 : 10./dSize);
	
	int iMaxCount = 100;    
	int iTotalCount = arStrDetail.GetSize();
	int iSheetCount = iTotalCount/iMaxCount + 1; 
	
	double dmod = fmod((double)iTotalCount, (double)iMaxCount);
	int imod = (int)dmod;    
	for(i=0; i<iSheetCount; i++)
	{
		CArray<Prt_ArString_Detail,Prt_ArString_Detail> ArrStrDetail;
		ArrStrDetail.RemoveAll();
		int iNumber = (iTotalCount-iMaxCount*(i+1)<0.0 ? imod : iMaxCount); 
		ArrStrDetail.SetSize(iNumber);
		int k=0;
		for(j=i*iMaxCount; j<(i*iMaxCount+iNumber); j++)
		{
			ArrStrDetail.SetAt(k,arStrDetail.GetAt(j));
			k++;
		} 
		iMaxRows = 0;
		for(j=0; j<ArrStrDetail.GetSize(); j++)
		{
			iMaxRows += ArrStrDetail.GetAt(j).ArString.arString.GetSize() + ArrStrDetail.GetAt(j).iFormula + 3; // 3=Temp
		}
		m_iMaxRows = iMaxRows+10;
		
		CString strNumber; 
		CString strTitle2;
		strNumber.Format(_T("%d"),i+1);
		strTitle2 = strTitle + strNumber;
		if(iSheetCount==1) m_pXL->AddSheet2(strTitle);
		else               m_pXL->AddSheet2(strTitle2);
				
		m_iCountSheet++;
		m_pXL->SetColumnWidth(0, 0, 0, 100, (double)0.6);
		m_pXL->SetFont(0,0, m_iMaxRows, m_iMaxCols, 8, 1, m_strChrType, FALSE);
		m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, m_iMaxCols);
		if(m_bStopExecute) return FALSE;
		
		nRow=0;
		nEndRow=0;
		for(j=0; j<ArrStrDetail.GetSize(); j++)
		{
			if(m_bStopExecute) return FALSE;
			arTorsStr.RemoveAll();
			arTorsStr.Copy(ArrStrDetail.GetAt(j).ArString.arString);
			dPercent += dIncrement;
			Progress(0, dPercent);
			Progress(2, dPercent);
			m_pPrs->WriteExpression(nRow, 0, nEndRow, 66, arTorsStr, _T(" "), sCopyBookName, _T("Sheet1"));  
			nRow = nEndRow;
		}  
	}
	
	return TRUE;
}


BOOL CCRCExcelOutput::XL_TorsStrengthCalc_KSCE10(CString sCopyBookName)
{
	Progress(0, 80);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_TORSION_CHECK));
	Progress(2, 80);
	
	// 이론
	CString strTitle = _T("비틀림강도 검토(이론)");
	CString AddSheetName = strTitle; 
	m_pXL->AddSheet2(AddSheetName);
	m_pXL->SetActiveSheet(AddSheetName);
	m_pXL->SetColumnWidth(0, 0, 0, 28, 2.2);
	m_pXL->SetRowHeight(15);
	m_pXL->SetMargin(13, 12, 20, 17); 
	if(m_bStopExecute) return FALSE;
	m_pPrtKSCE10->Set_CMSExcel_KSCE10(m_pXL, m_strBaseBook, m_PscdD.iDgnCode);
	
	m_pPrtKSCE10->Set_CMSExcel_Torsion_theory(m_pXL);
	m_iCountSheet++;

	// 계산
	strTitle = _T("비틀림강도 검토(구조계산)");
	AddSheetName = strTitle; 
	m_pXL->AddSheet2(AddSheetName);
	m_pXL->SetActiveSheet(AddSheetName);
	m_pXL->SetColumnWidth(0, 0, 0, 28, 2.2);
	m_pXL->SetRowHeight(15);
	m_pXL->SetMargin(13, 12, 20, 17); 
	if(m_bStopExecute) return FALSE;

	CArray<CString,CString> arTheory;
	CArray<CString,CString> arTorsStr;  arTorsStr.RemoveAll();
	int iCountFormula=0;
	CArray<Prt_ArString_Detail,Prt_ArString_Detail> arStrDetail;
	if(!m_pPrtKSCE->Get_TorsStrengthCalc(arTheory, arStrDetail)) return FALSE;
	if(m_bStopExecute) return FALSE;
	
	return TRUE;
}

// 인장 철근량 계산.
BOOL CCRCExcelOutput::XL_ReinforcementCalc(CString sCopyBookName)
{
	Progress(0, 90);
	ProgressText(1, _LS(IDS_DGN_PSC_XL_TENSION_BAR_CALC));
	Progress(2, 90);
	
	CArray<CString,CString> arTbarStr;  arTbarStr.RemoveAll();
	int iCountFormula=0;
	CArray<Prt_ArString_Detail,Prt_ArString_Detail> arStrDetail;
	if(!m_pPrtKSCE->Get_ReinforceCalc(arStrDetail)) return FALSE;
	if(m_bStopExecute) return FALSE;
	
	CString strTitle = _T("인장철근량(구조계산)");
	
	int i,j;
	int iMaxRows=0;
	double dSize = arStrDetail.GetSize();
	double dPercent=90.;
	double dIncrement= (dSize==0.0 ? 0.0 : 10./dSize);
	
	int iMaxCount = 100;    
	int iTotalCount = arStrDetail.GetSize();
	int iSheetCount = iTotalCount/iMaxCount + 1; 
	
	double dmod = fmod((double)iTotalCount, (double)iMaxCount);
	int imod = (int)dmod;    
	for(i=0; i<iSheetCount; i++)
	{
		CArray<Prt_ArString_Detail,Prt_ArString_Detail> ArrStrDetail;
		ArrStrDetail.RemoveAll();
		int iNumber = (iTotalCount-iMaxCount*(i+1)<0.0 ? imod : iMaxCount); 
		ArrStrDetail.SetSize(iNumber);
		int k=0;
		for(j=i*iMaxCount; j<(i*iMaxCount+iNumber); j++)
		{
			ArrStrDetail.SetAt(k,arStrDetail.GetAt(j));
			k++;
		} 
		iMaxRows = 0;
		for(j=0; j<ArrStrDetail.GetSize(); j++)
		{
			iMaxRows += ArrStrDetail.GetAt(j).ArString.arString.GetSize() + ArrStrDetail.GetAt(j).iFormula + 3; // 3=Temp
		}
		m_iMaxRows = iMaxRows+10;
		
		CString strNumber; 
		CString strTitle2;
		strNumber.Format(_T("%d"),i+1);
		strTitle2 = strTitle + strNumber;
		if(iSheetCount==1) m_pXL->AddSheet2(strTitle);
		else               m_pXL->AddSheet2(strTitle2);
				
		m_iCountSheet++;
		m_pXL->SetColumnWidth(0, 0, 0, 100, (double)0.6);
		m_pXL->SetFont(0,0, m_iMaxRows, m_iMaxCols, 8, 1, m_strChrType, FALSE);
		m_pXL->SetColumnInTextFormat(0,0, m_iMaxRows, m_iMaxCols);
		if(m_bStopExecute) return FALSE;
		
		int nRow=0;
		int nEndRow=0;
		for(j=0; j<ArrStrDetail.GetSize(); j++)
		{
			if(m_bStopExecute) return FALSE;
			arTbarStr.RemoveAll();
			arTbarStr.Copy(ArrStrDetail.GetAt(j).ArString.arString);
			dPercent += dIncrement;
			Progress(0, dPercent);
			Progress(2, dPercent);
			m_pPrs->WriteExpression(nRow, 0, nEndRow, 66, arTbarStr, _T(" "), sCopyBookName, _T("Sheet1"));  
			nRow = nEndRow;
		}  
	}
	
	return TRUE;
}

// PSC-rating
BOOL CCRCExcelOutput::Execute_PSCReport_Rating(ElemPairK ElemKey, int iRptUnit, T_BDCY_AASHTO& BdcyRes, _BDCY_AASHTO& BdcyResD)
{
//  Start();
		UINT nFlg = -1;
		
		// Change unit
		T_UNIT_INDEX CngUnit;
		int iRptSelUnit = CDBLib::GetReportSelUnit();
		if(iRptSelUnit==0) // US unit
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_KIP;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
		}
		else if(iRptSelUnit==1) // SI unit
		{
			CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_N;
			CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		}
		else ASSERT(0);
		
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
//



	Progress(0, 0);
	int nProgressPercent = 0;
	
	BOOL bSuccess = TRUE;
	try
	{
		if(m_bStopExecute) return FALSE;
		nProgressPercent +=5;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		if(!myAashtoPscReport.Initialize(m_PscdD.iDgnCode, iRptUnit, m_pDoc->GetProgramPath(), GetSaveFileDirEx(), GetSaveFileNameEx()))
			throw FALSE;

		if(m_bStopExecute) return FALSE;
		nProgressPercent +=5;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

			// Get all element keys for which results are to be printed
		CArray<T_POSX_K, T_POSX_K> arrPrintElemKey;
		m_pDoc->m_pAttrCtrl->GetPosxKeyList(arrPrintElemKey);
		
		//T_POSX_D PosxD;
		int nPrintElemKeys = arrPrintElemKey.GetSize();
		double dProgressIncrementPercent = nPrintElemKeys == 0 ? 70.0 : 70.0/((double)nPrintElemKeys);
		int nProgressIncrementPercent = nPrintElemKeys == 0 ? 70 : 70/nPrintElemKeys;


		// 1. Design Condition
		CArray<PSC_DATA_POSI, PSC_DATA_POSI&> arInData;  arInData.RemoveAll(); arInData.SetSize(4);  
		PSC_DATA_POSI InData;    
		_BDCY_PSC_LRFD_BASE EngBdcyD;  

		for(int j=0; j<4; j++) 
		{
			InData.Initialize();
			EngBdcyD.Initialize();
			
			int nNode = (j<2)? 0 : 1;    // I,J
//       // PrintOpt의 I/J 가 전부 제거되었을 때 Detail을 출력하지 않도록
//       if(nNode==0 && !bCheckMomI && !bCheckMomNegI && !bCheckShrI && !bCheckTorI) continue;
//       if(nNode==1 && !bCheckMomJ && !bCheckMomNegJ && !bCheckShrJ && !bCheckTorJ) continue;
			
			int nPosNeg = (j==0 || j==2)? 0 : 1;  // 0:pos  1:neg 
			m_DataCtrl.m_pPscDataCtrl->Get_BdcyReportData( TRUE, ElemKey, nNode, nPosNeg, BdcyRes.BdcyBase[j], InData, EngBdcyD);
			arInData.SetAt(j, InData);
			
			if(j==0 || j==2)
			{
				myAashtoPscReport.Print_Detail_Condition(m_PscdD.iDgnCode, ElemKey, nNode, InData, EngBdcyD);
			}
		}
		
		// 2. Flexure Design
		for(int nIJ=0; nIJ<2; nIJ++)
		{
			int nStart=0;
			int nLast=0;
			if(nIJ==0) { nStart=0; nLast=2; }
			else       { nStart=2; nLast=4; }
			
			BOOL bFirst_flex = TRUE;
			for(int j=nStart; j<nLast; j++) 
			{
				
				if(m_PscdD.bStrePrint4AASHTO[0] && BdcyResD.BdcyBase[j].bCHK)
				{
//           if(j==0 && !bCheckMomI)    continue;
//           if(j==1 && !bCheckMomNegI) continue;
//           if(j==2 && !bCheckMomJ)    continue;
//           if(j==3 && !bCheckMomNegJ) continue;
					
					myAashtoPscReport.Print_Detail_FlexureDesign(m_PscdD.iDgnCode, ElemKey, j, bFirst_flex, arInData.GetAt(j), BdcyResD.BdcyBase[j]);      
					bFirst_flex = FALSE;
				}
			} 
		}
	}
	catch(...)
	{
		bSuccess = FALSE;	
	}
	
	int nType = 1; // sheet 마무리 
	
	myAashtoPscReport.Terminate(m_PscdD.iDgnCode, !bSuccess, nType);
	
	if(m_bStopExecute) return FALSE;
	nProgressPercent = 100;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);
	
	return bSuccess;

}



BOOL CCRCExcelOutput::Execute_PSCReport(int iDgnCode, int iRptUnit, CString sSaveFileName)
{
	Progress(0, 0);
	
	// We actually need an exception library which wraps CException but since there is no library I can find I am going to throw bool. 
	// I could use goto but I cant bring myself to use it.

//	CDgn_PSCRptManager myAashtoPscReport;
	int nProgressPercent = 0;
	 
	BOOL bSuccess = TRUE;
	try
	{
		if(m_bStopExecute) return FALSE;
		nProgressPercent +=5;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		if(!myAashtoPscReport.Initialize(m_PscdD.iDgnCode, iRptUnit, m_pDoc->GetProgramPath(), GetSaveFileDirEx(), sSaveFileName))
			throw FALSE;
		
		if(m_bStopExecute) return FALSE;
		nProgressPercent +=5;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		BOOL bCompatibility = m_PscdD.ifpsMethod==0 ? FALSE : TRUE;
	
		// Get all element keys for which results are to be printed
		CArray<T_POSX_K, T_POSX_K> arrPrintElemKey;
		m_pDoc->m_pAttrCtrl->GetPosxKeyList(arrPrintElemKey);

		CArray<T_POSV_K, T_POSV_K> aPrintVBeamKey;
		m_pDoc->m_pAttrCtrl->GetPosvKeyList(aPrintVBeamKey);

		ArrElemPairKey aPrintEPairKey;
		CDBLib::ConvertToElemPairKeyList(arrPrintElemKey, aPrintVBeamKey, aPrintEPairKey);

		T_POSX_D PosxD;
		int nPrintElemKeys = aPrintEPairKey.GetSize();
		double dProgressIncrementPercent = nPrintElemKeys == 0 ? 70.0 : 70.0/((double)nPrintElemKeys);
		int nProgressIncrementPercent = nPrintElemKeys == 0 ? 70 : 70/nPrintElemKeys;
		
        int nFysgRow = 0;
        int nFycmRow = 0;
        int nFssgRow = 0;
        int nFpmsRow = 0;
        int nFpmtRow = 0;
		CPCDesign* pPCDesign = m_pDoc->m_pPostCtrl->GetPCDesign();

		switch ( m_PscdD.iDgnCode )
		{
		case AASHTO_LRFD12:
		case AASHTO_LRFD14:
		case AASHTO_LRFD16:
		case AASHTO_LRFD17:
		case AASHTO_LRFD20:
		case AASHTO_LRFD24:
		case AREMA_23:
			{
				m_DataCtrl.CalcTendonfpsLpPosition(); // Transmission Lp, Lpt 위치의 fps 계산을 위해..
			}
			break;
		default:
			break;
		}		

		CCRCForceCtrl* pForceCtrl = m_DataCtrl.Get_ForceCtrlPointer();
		if ( pForceCtrl )
		{
			pForceCtrl->Set_LcomDataForDesign();
		}

		for(int i=0; i<nPrintElemKeys; i++)
		{
			if(m_bStopExecute) return FALSE;
			nProgressPercent = (int)(dProgressIncrementPercent*(i+1));
			Progress(0, nProgressPercent);
			Progress(2, nProgressPercent);

			// 출력 단위
			if(m_PscdD.iDgnCode ==AASHTO_LRFD12 || m_PscdD.iDgnCode ==AASHTO_LRFD14 || m_PscdD.iDgnCode ==AASHTO_LRFD16 || m_PscdD.iDgnCode ==AASHTO_LRFD17)
			{
				T_UNIT_INDEX CngUnit;
				m_DataCtrl.Get_PrintUnit(CngUnit.nBase_Force, CngUnit.nBase_Length);
				m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
			}

			auto ElemKey = aPrintEPairKey[i];
			
			BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemKey);

			T_FYSG_AASHTO fysgData;	fysgData.Initialize();
			T_FYCM_AASHTO fycmData;	fycmData.Initialize();
			T_FYSG_AASHTO fysgData_Gr;	fysgData_Gr.Initialize();
			T_FYSG_AASHTO fysgData_Sl;	fysgData_Sl.Initialize();
			T_FYCM_AASHTO fycmData_Gr;	fycmData_Gr.Initialize();
			T_FYCM_AASHTO fycmData_Sl;	fycmData_Sl.Initialize();
			if (!bCompositeElem)
			{
				pPCDesign->ReadFysg_AASHTO(ElemKey, fysgData);
				pPCDesign->ReadFycm_AASHTO(ElemKey, fycmData);
			}
			else if(bCompositeElem)
			{
				pPCDesign->ReadFysg_AASHTO_Composite_Gr(ElemKey, fysgData_Gr);
				pPCDesign->ReadFysg_AASHTO_Composite_Sl(ElemKey, fysgData_Sl);
				pPCDesign->ReadFycm_AASHTO_Composite_Gr(ElemKey, fycmData_Gr);
				pPCDesign->ReadFycm_AASHTO_Composite_Sl(ElemKey, fycmData_Sl);
			}
			else ASSERT(0);

			T_FPMT_JTG fpmtData; fpmtData.Initialize();
			if(!bCompositeElem) pPCDesign->ReadFssg_AASHTO(ElemKey, fpmtData);

			T_FPMS_JTG fpmsData; fpmsData.Initialize();
			if(!bCompositeElem) pPCDesign->ReadFpms_AASHTO(ElemKey, fpmsData);

			T_FPMS_JTG fpms1Data; fpms1Data.Initialize();
			if(!bCompositeElem) pPCDesign->ReadFpmt_AASHTO(ElemKey, fpms1Data);

			//T_TBAR_AASHTO tbarData; tbarData.Initialize();
			//pPCDesign->ReadTbar_AASHTO(ElemKey, tbarData);

			const int nCodePSC = m_PscdD.iDgnCode;
			int nPrtUnit = 0;  // 0 : US  1 : SI
			BOOL bReCalc = FALSE;
			if(nCodePSC ==AASHTO_LRFD08)
			{
				/*
					LRFD08에서는 출력을 위해 계산을 다시 하지 않는다.
				*/
				bReCalc = FALSE;
			}
			else if(nCodePSC ==AASHTO_LRFD12 || nCodePSC ==AASHTO_LRFD14 || nCodePSC ==AASHTO_LRFD16 || 
					nCodePSC ==AASHTO_LRFD17 || nCodePSC ==AASHTO_LRFD20 || nCodePSC ==AASHTO_LRFD24)
			{
				/*
				 LRFD12일 때 BdcyResD, CumsResD, CrmtResD, BdcwResD의 단위를  kip/in(설계단위) 로 바꿔서 가져온다.
				 데이터를 만들기 위해 계산을 다시 하므로..
				 계산을 위해 설계단위로 맞춘뒤 입력값들을 계산쪽으로 넘기고
				 계산후 출력 단위계로 변환한다.
				*/
				T_UNIT_INDEX CngUnit;
				CngUnit.nBase_Force	 = D_UNITSYS_FORCE_INDEX_KIP;
				CngUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;       
				m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit

				bReCalc = TRUE;      
			}
			else if(CDBLib::IsPscCodeCSA(nCodePSC))
			{
				/*
				 CSA_S6_10 일때는 N/mm(설계단위)로 맞춰져서 이 함수로 들어오므로 
				 단위 변환이 필요없음.
				*/

				bReCalc = TRUE;      
			}
			else ASSERT(0);

			nPrtUnit = iRptUnit>0 ? 1 : 0;

			T_BDCY_AASHTO bdcyData;	bdcyData.Initialize(); 
            T_PSCD_DGN_INFO DgnD;
			_BDCY_AASHTO BdcyResD;                         // 계산결과 구조체
			if(pPCDesign->ReadBdcy_AASHTO(ElemKey, bdcyData))
			{
				if(bReCalc) m_DataCtrl.m_pPscDataCtrl->Get_BdcyData(ElemKey, bdcyData, DgnD, &BdcyResD, nPrtUnit);
				//m_DataCtrl.m_pPscDataCtrl->Get_BdcyData(ElemKey, bdcyData, &BdcyResD, nPrtUnit);  
			}

			T_CUMS_AASHTO cumsData;	cumsData.Initialize();
			_CUMS_AASHTO CumsResD;
			if(pPCDesign->ReadCums_AASHTO(ElemKey, cumsData))
			{
				if(bReCalc) m_DataCtrl.m_pPscDataCtrl->Get_CumsData(ElemKey, cumsData, &CumsResD, nPrtUnit);
				//m_DataCtrl.m_pPscDataCtrl->Get_CumsData(ElemKey, cumsData, &CumsResD, nPrtUnit);
			}

			T_CRMT_AASHTO crmtData;	crmtData.Initialize();
			_CRMS_AASHTO CrmtResD;
			if(pPCDesign->ReadCrmt_AASHTO(ElemKey, crmtData))
			{
				if(bReCalc) m_DataCtrl.m_pPscDataCtrl->Get_CrmtData(ElemKey, crmtData, &CrmtResD, nPrtUnit);
				//m_DataCtrl.m_pPscDataCtrl->Get_CrmtData(ElemKey, crmtData, &CrmtResD, nPrtUnit);
			}

			// AASHTO
			T_BDCW_AASHTO bdcwData;	bdcwData.Initialize();
			_BDCW_AASHTO BdcwResD;
			if(pPCDesign->ReadBdcw_AASHTO(ElemKey, bdcwData))
			{
				if(bReCalc) m_DataCtrl.m_pPscDataCtrl->Get_BdcwData(ElemKey, bdcwData, &BdcwResD, nPrtUnit);
				//m_DataCtrl.m_pPscDataCtrl->Get_BdcwData(ElemKey, bdcwData, &BdcwResD, nPrtUnit);
			}

			// CSA COMPOSITE FYSG DETAIL
			fysgData_Gr.Initialize();
			_FYSG_AASHTO FysgResD_Gr;
			if(pPCDesign->ReadFysg_AASHTO_Composite_Gr(ElemKey, fysgData_Gr))
			{
				if(bReCalc) m_DataCtrl.m_pPscDataCtrl->Get_FysgData(ElemKey, fysgData_Gr, &FysgResD_Gr, nPrtUnit);
			}

			fysgData_Sl.Initialize();
			_FYSG_AASHTO FysgResD_Sl;
			if(pPCDesign->ReadFysg_AASHTO_Composite_Sl(ElemKey, fysgData_Sl))
			{
				if(bReCalc) m_DataCtrl.m_pPscDataCtrl->Get_FysgData(ElemKey, fysgData_Sl, &FysgResD_Sl, nPrtUnit);
			}

			fycmData_Gr.Initialize();
			_FYCM_AASHTO FycmResD_Gr;
			if(pPCDesign->ReadFycm_AASHTO_Composite_Gr(ElemKey, fycmData_Gr))
			{
				if(bReCalc) m_DataCtrl.m_pPscDataCtrl->Get_FycmData(ElemKey, fycmData_Gr, &FycmResD_Gr, nPrtUnit);
			}

			fycmData_Sl.Initialize();
			_FYCM_AASHTO FycmResD_Sl;
			if(pPCDesign->ReadFycm_AASHTO_Composite_Sl(ElemKey, fycmData_Sl))
			{
				if(bReCalc) m_DataCtrl.m_pPscDataCtrl->Get_FycmData(ElemKey, fycmData_Sl, &FycmResD_Sl, nPrtUnit);
			}

	
	
			PosxD.Initialize();
			if (ElemKey.second == EN_EL_BEAM)
			{
				m_pDoc->m_pAttrCtrl->GetPosx(ElemKey.first, PosxD);
			}
			else if (ElemKey.second == EN_EL_VBEAM)
			{
				m_pDoc->m_pAttrCtrl->GetPosv(ElemKey.first, PosxD);
			}
			else ASSERT(0);			
			
			BOOL bCheckBarI = FALSE, bCheckBarJ = FALSE;
			ShouldPrintPosxBarIJ(PosxD, bCheckBarI, bCheckBarJ);
			
			BOOL bCheckMomI = FALSE, bCheckMomJ = FALSE;
			ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);
			
			BOOL bCheckShrI = FALSE, bCheckShrJ = FALSE;
			ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);
			
			BOOL bCheckTorI = FALSE, bCheckTorJ = FALSE;
			ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

			// 출력
			for(int j=0; j<4; j++ )
			{
                bool bStartReport = false;
				if(!bCompositeElem)
				{
					if(m_PscdD.bServPrint4AASHTO[0] && fysgData.FysgBase[j].bCHK)
                    {
                        bStartReport = IsFirstRow(++nFysgRow);
                        if ( !myAashtoPscReport.Print_AllowableStresses_Conc_ConstructionStage(m_PscdD.iDgnCode, ElemKey, j, bStartReport, fysgData.FysgBase[j]) )	throw FALSE;
                    }

					if(m_PscdD.bServPrint4AASHTO[1] && fycmData.FycmBase[j].bCHK)
                    {
                        bStartReport = IsFirstRow(++nFycmRow);
                        if ( !myAashtoPscReport.Print_AllowableStresses_Conc_ServiceLoadCombination(m_PscdD.iDgnCode, ElemKey, j, bStartReport, fycmData.FycmBase[j]) )	throw FALSE;
                    }
				}
				else if(bCompositeElem)
				{

					if(m_PscdD.bServPrint4AASHTO[0] && fysgData_Gr.FysgBase[j].bCHK)
                    {
                        bStartReport = IsFirstRow(++nFysgRow);
                        if ( !myAashtoPscReport.Print_AllowableStresses_Conc_ConstructionStage_Comp(m_PscdD.iDgnCode, ElemKey, j, bStartReport, fysgData_Gr.FysgBase[j], 1) )	throw FALSE;
                    }

					if(m_PscdD.bServPrint4AASHTO[0] && fysgData_Sl.FysgBase[j].bCHK)
                    {
                        bStartReport = IsFirstRow(++nFysgRow);
                        if ( !myAashtoPscReport.Print_AllowableStresses_Conc_ConstructionStage_Comp(m_PscdD.iDgnCode, ElemKey, j, bStartReport, fysgData_Sl.FysgBase[j], 0) )	throw FALSE;
                    }

					if(m_PscdD.bServPrint4AASHTO[1] && fycmData_Gr.FycmBase[j].bCHK)
                    {
                        bStartReport = IsFirstRow(++nFycmRow);
                        if ( !myAashtoPscReport.Print_AllowableStresses_Conc_ServiceLoadCombination_Comp(m_PscdD.iDgnCode, ElemKey, j, bStartReport, fycmData_Gr.FycmBase[j], 1) )	throw FALSE;
                    }

					if(m_PscdD.bServPrint4AASHTO[1] && fycmData_Sl.FycmBase[j].bCHK)
                    {
                        bStartReport = IsFirstRow(++nFycmRow);
                        if ( !myAashtoPscReport.Print_AllowableStresses_Conc_ServiceLoadCombination_Comp(m_PscdD.iDgnCode, ElemKey, j, bStartReport, fycmData_Sl.FycmBase[j], 0) )	throw FALSE;
                    }
				}
				else ASSERT(0);

				if(m_PscdD.bServPrint4AASHTO[3] && fpmtData.FpmtBase[j].bCHK)
				{
                    if ( j==1 || j==3 )
                    {
                        bStartReport = IsFirstRow(++nFssgRow);
                        if ( !myAashtoPscReport.Print_AllowablePrincipalStress_Conc_ConstructionStage(m_PscdD.iDgnCode, ElemKey, j, bStartReport, fpmtData.FpmtBase[j]) )	throw FALSE;
                    }
				}				
				if(m_PscdD.bServPrint4AASHTO[4] && fpmsData.FpmsBase[j].bCHK)
				{
                    if ( j==1 || j==3 )
                    {
                        bStartReport = IsFirstRow(++nFpmsRow);
                        if ( !myAashtoPscReport.Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxShear(m_PscdD.iDgnCode, ElemKey, j, bStartReport, fpmsData.FpmsBase[j]) )	throw FALSE;
                    }
				}
				
				if(m_PscdD.bServPrint4AASHTO[5] && fpms1Data.FpmsBase[j].bCHK)
				{
                    if ( j==1 || j==3 )
                    {
                        bStartReport = IsFirstRow(++nFpmtRow);
                        if ( !myAashtoPscReport.Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxTorsion(m_PscdD.iDgnCode, ElemKey, j, bStartReport, fpms1Data.FpmsBase[j]) )	throw FALSE;
                    }
				}
			
			}
			// for(int j=0; j<4; j++ )

			//////////////////////////////////////////////////////////////////////////
			if(m_PscdD.iDgnCode==AASHTO_LRFD08 )
			{
				// 크랙
				for (int j = 0; j < 4; j++)
				{
					if (m_PscdD.bStrePrint4AASHTO[3] && bdcwData.BdcwBase[j].bCHK)
						if (!myAashtoPscReport.Print_CrackCheck(ElemKey, j, bdcwData.BdcwBase[j]))	throw FALSE;
				}

				// 휨, 전단
				for(int j=0; j<4; j++ )
				{
					if (m_PscdD.bStrePrint4AASHTO[0] && bdcyData.BdcyBase[j].bCHK)
						if ((j < 2 && bCheckMomI) || (j > 1 && bCheckMomJ))
							if (!myAashtoPscReport.Print_FlexureDesign(ElemKey, j, bCompatibility, bdcyData.BdcyBase[j]))	throw FALSE;

					if (m_PscdD.bStrePrint4AASHTO[1] && cumsData.CumsBase[j].bCHK)
						if ((j < 2 && bCheckShrI) || (j > 1 && bCheckShrJ))
							if (!myAashtoPscReport.Print_ShearDesign(ElemKey, j, cumsData.CumsBase[j]))	throw FALSE;
				}

				// Torsion
				for(int j=0; j<6; j++)
				{
					if(m_PscdD.bStrePrint4AASHTO[2] && crmtData.CrmtBase[j].bCHK)
						if((j < 3 && bCheckTorI) || (j > 2 && bCheckTorJ))
							if(!myAashtoPscReport.Print_TorsionDesign(ElemKey, j, crmtData.CrmtBase[j]))	throw FALSE;
				}
			}

			if(CDBLib::IsPscCodeAASHTOSince12(m_PscdD.iDgnCode) || 
				 CDBLib::IsPscCodeCSA(m_PscdD.iDgnCode))
			{
				// 0. 결과 없으면 출력 안함.
				if(!m_PscdD.bStrePrint4AASHTO[0] && !m_PscdD.bStrePrint4AASHTO[1] && !m_PscdD.bStrePrint4AASHTO[2] && !m_PscdD.bStrePrint4AASHTO[3]) continue;  
			 
				BOOL bcheck = FALSE;
				for(int j=0; j<4; j++)  // 휨, 전단, 크랙에 출력 여부로 전체 출력 판단
				{
					if(bdcyData.BdcyBase[j].bCHK || cumsData.CumsBase[j].bCHK || bdcwData.BdcwBase[j].bCHK)  {bcheck= TRUE; continue;}
				}

				if(bcheck == TRUE) 
				{
					if(!Print_AASHTO_LRFD12_PSC(ElemKey, bdcyData, BdcyResD, CumsResD, BdcwResD, CrmtResD, FysgResD_Gr, FysgResD_Sl, FycmResD_Gr, FycmResD_Sl)){throw FALSE;}
				}
			}
				
		} // for Keylist
		
		CArray<UINT, UINT> aTndnKey;
		pPCDesign->GetPscCheckTdnaList(aTndnKey);

		// 출력 단위에 따른 tendon 값 출력
		T_UNIT_INDEX CngUnit;
		m_DataCtrl.Get_PrintUnit(CngUnit.nBase_Force, CngUnit.nBase_Length);
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngUnit);  // Code Unit
		//

		for(int i=0; i<aTndnKey.GetSize(); i++)
		{
			T_TDNA_K TdnaK = aTndnKey[i];
			
			T_FYPC_AASHTO fypcData;	fypcData.Initialize();
			pPCDesign->ReadFypc_AASHTO(TdnaK, fypcData);

            bool bStartTendon = false;
            if ( i == 0 ) bStartTendon = true;

			if(m_PscdD.bServPrint4AASHTO[2])
				if(!myAashtoPscReport.Print_AllowableStress_Tendon(m_PscdD.iDgnCode, TdnaK, bStartTendon, fypcData)) throw FALSE;
		}

		if(m_bStopExecute) return FALSE;
		nProgressPercent = 90;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);
		
	}	
	catch(...)
	{
		bSuccess = FALSE;	
	}

	int nType = (CDBLib::IsPscCodeAASHTOSince12(m_PscdD.iDgnCode) || 
							 CDBLib::IsPscCodeCSA(m_PscdD.iDgnCode))? 1 : 0;  // 기준에 따라 출력할 엑셀 tab 지정

	myAashtoPscReport.Terminate( m_PscdD.iDgnCode, !bSuccess, nType);
	
	if(m_bStopExecute) return FALSE;
	nProgressPercent = 100;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);
	
	return bSuccess;
}

// 
// //********************************************************
// // AASHTO-LRFD12
// //********************************************************
BOOL CCRCExcelOutput::Print_AASHTO_LRFD12_PSC(ElemPairK ElemKey, T_BDCY_AASHTO& bdcyData, _BDCY_AASHTO& BdcyResD, _CUMS_AASHTO& CumsResD, _BDCW_AASHTO& BdcwResD, _CRMS_AASHTO& CrmtResD,
																							_FYSG_AASHTO& FysgResD_Gr, _FYSG_AASHTO& FysgResD_Sl, _FYCM_AASHTO& FycmResD_Gr, _FYCM_AASHTO& FycmResD_Sl)
{
//  CDgn_PSCRptManager myAashtoPscReport;
	
	T_POSX_D PosxD; PosxD.Initialize();
	if (ElemKey.second == EN_EL_BEAM)
	{
		m_pDoc->m_pAttrCtrl->GetPosx(ElemKey.first, PosxD);
	}
	else if (ElemKey.second == EN_EL_VBEAM)
	{
		m_pDoc->m_pAttrCtrl->GetPosv(ElemKey.first, PosxD);
	}
	else ASSERT(0);

	BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemKey);
	
	BOOL bCheckMomI = FALSE, bCheckMomJ = FALSE;
	ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);

	BOOL bCheckMomNegI = FALSE, bCheckMomNegJ = FALSE;
	ShouldPrintPosxMomIJ_Negative(PosxD, bCheckMomNegI, bCheckMomNegJ);

	BOOL bCheckShrI = FALSE, bCheckShrJ = FALSE;
	ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);
	
	BOOL bCheckTorI = FALSE, bCheckTorJ = FALSE;
	ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

	BOOL bCheckConI = FALSE, bCheckConJ = FALSE; //Construction Stress Detail
	ShouldPrintPosxTorIJ(PosxD, bCheckConI, bCheckConJ);

	CArray<PSC_DATA_POSI, PSC_DATA_POSI&> arInData;  arInData.RemoveAll(); arInData.SetSize(4);  
	PSC_DATA_POSI InData, InDataPrint;    
	_BDCY_PSC_LRFD_BASE EngBdcyD;  

	// 0. PSC_DATA for Print
	// -출력 단위에 따른  PSC_DATA_POSI 생성

	DGN_JOINT_D JointD;
	m_DataCtrl.Get_JointData4Element(ElemKey, JointD);

	//T_UNIT_INDEX CngUnit, PrtUnit;
	for(int j=0; j<4; j++) 
	{
		InDataPrint.Initialize();
		
		int nNode = (j<2)? 0 : 1;    // I,J

		//joint
		InDataPrint.iJoint = nNode==0 ? JointD.iI_joint : JointD.iJ_joint;

		// PrintOpt의 I/J 가 전부 제거되었을 때 Detail을 출력하지 않도록
		if(nNode==0 && !bCheckMomI && !bCheckMomNegI && !bCheckShrI && !bCheckTorI) continue;
		if(nNode==1 && !bCheckMomJ && !bCheckMomNegJ && !bCheckShrJ && !bCheckTorJ) continue;
		
		int nPosNeg = (j==0 || j==2)? 0 : 1;  // 0:pos  1:neg 
		m_DataCtrl.m_pPscDataCtrl->Get_DesignConditionData(FALSE, ElemKey, nNode, nPosNeg, InDataPrint);
		arInData.SetAt(j, InDataPrint);   
	}

	// 1. Design Condition
	for(int j=0; j<4; j++) 
	{
		InData.Initialize();
		EngBdcyD.Initialize();

		int nNode = (j<2)? 0 : 1;    // I,J
		// PrintOpt의 I/J 가 전부 제거되었을 때 Detail을 출력하지 않도록
		if(nNode==0 && !bCheckMomI && !bCheckMomNegI && !bCheckShrI && !bCheckTorI) continue;
		if(nNode==1 && !bCheckMomJ && !bCheckMomNegJ && !bCheckShrJ && !bCheckTorJ) continue;

		int nPosNeg = (j==0 || j==2)? 0 : 1;  // 0:pos  1:neg 
		m_DataCtrl.m_pPscDataCtrl->Get_BdcyReportData(FALSE, ElemKey, nNode, nPosNeg, bdcyData.BdcyBase[j], InData, EngBdcyD);
		//arInData.SetAt(j, InData);
		InData = arInData.GetAt(j);
		
		if(j==0 || j==2)
		{
			myAashtoPscReport.Print_Detail_Condition(m_PscdD.iDgnCode, ElemKey, nNode, InData, EngBdcyD);
		}
	}
	
	// 2. Flexure Design
	for(int nIJ=0; nIJ<2; nIJ++)
	{
		int nStart=0;
		int nLast=0;
		if(nIJ==0) { nStart=0; nLast=2; }
		else       { nStart=2; nLast=4; }

		BOOL bFirst_flex = TRUE;
		for(int j=nStart; j<nLast; j++) 
		{

			if(m_PscdD.bStrePrint4AASHTO[0] && BdcyResD.BdcyBase[j].bCHK)
			{
				if(j==0 && !bCheckMomI)    continue;
				if(j==1 && !bCheckMomNegI) continue;
				if(j==2 && !bCheckMomJ)    continue;
				if(j==3 && !bCheckMomNegJ) continue;
															
				myAashtoPscReport.Print_Detail_FlexureDesign(m_PscdD.iDgnCode, ElemKey, j, bFirst_flex, arInData.GetAt(j), BdcyResD.BdcyBase[j]);      
				bFirst_flex = FALSE;
			}
		} 
	}


	// 3. Shear
	for(int nIJ=0; nIJ<2; nIJ++)
	{
		int nStart=0;
		int nLast=0;
		if(nIJ==0) { nStart=0; nLast=2; }
		else       { nStart=2; nLast=4; }

		BOOL bFirst_shear = TRUE;
		for(int j=nStart; j<nLast; j++) 
		{
			InData.Initialize();
			if(m_PscdD.bStrePrint4AASHTO[1] && CumsResD.CumsBase[j].bCHK)
			{
				if(j==0 && !bCheckShrI)   continue;
				if(j==1 && !bCheckShrI)   continue;
				if(j==2 && !bCheckShrJ)   continue;
				if(j==3 && !bCheckShrJ)   continue;

				InData = arInData.GetAt(j);
				myAashtoPscReport.Print_Detail_ShearDesign(m_PscdD.iDgnCode, ElemKey, j, bFirst_shear, arInData.GetAt(j), CumsResD.CumsBase[j]);      
				bFirst_shear = FALSE;
			}      
		}
	}

	// 4. Torsion
	for(int nIJ=0; nIJ<2; nIJ++)
	{
		int nStart=0;
		int nLast=0;
		if(nIJ==0) { nStart=0; nLast=3; }
		else       { nStart=3; nLast=6; }
		
		BOOL bFirst_torsion = TRUE; 
		for(int j=nStart; j<nLast; j++) 
		{
			if(j<3  && !bCheckTorI)   continue;
			if(j>=3 && !bCheckTorJ)   continue;
			
			InData.Initialize();
			if(m_PscdD.bStrePrint4AASHTO[2] && CrmtResD.CrmsBase[j].bCHK)
			{
				InData = (nIJ==0)? arInData.GetAt(0) : arInData.GetAt(2);
				myAashtoPscReport.Print_Detail_TorsionDesign(m_PscdD.iDgnCode, ElemKey, j, bFirst_torsion, InData, CrmtResD.CrmsBase[j]);      
				bFirst_torsion = FALSE;
			}      
		}
	}

	 
	 T_BDCW_PSC BdcwD; BdcwD.Initialize();
	 PSC_BDCW_BASE dgnbdcw;  dgnbdcw.Initialize();
	 _BDCW_PSC_LRFD_BASE BdcwBase; BdcwBase.Initialize();

	 // CSA Crack 데이터 만들기
	 if(CDBLib::IsPscCodeCSA(m_PscdD.iDgnCode))
	 {
		 // 데이터 만들기
		 m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_CSA(ElemKey, BdcwD);

		 for(int j=0; j<4; ++j)
		 {
		 // if(!BdcwD.BdcwBase[j].bCHK) continue;
			 
			 InData.Initialize();
			 dgnbdcw.Initialize();
			 
			 int iPosiNo = 0;  iPosiNo = (j<2)? 0 : 1;
			 int iMaxMin = 0;  iMaxMin = j%2;
			 m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData(ElemKey, iPosiNo, iMaxMin, BdcwD.BdcwBase[j], InData, dgnbdcw);     

			 // 변환
			 BdcwBase.Initialize();
			 Convert_PSCBDCWtoAASHTOBDCW(dgnbdcw,  BdcwBase); 

			 BdcwResD.BdcwBase[j] = BdcwBase;
		 }
	 }

	// 5. Crack
	for(int nIJ=0; nIJ<2; nIJ++)
	{
		// PrintOpt의 I/J 가 전부 제거되었을 때 Crack을 출력하지 않도록
		if(nIJ==0 && !bCheckMomI && !bCheckMomNegI && !bCheckShrI && !bCheckTorI) continue;
		if(nIJ==1 && !bCheckMomJ && !bCheckMomNegJ && !bCheckShrJ && !bCheckTorJ) continue;
		int nStart=0;
		int nLast=0;
		if(nIJ==0) { nStart=0; nLast=2; }
		else       { nStart=2; nLast=4; }
		
		BOOL bFirst_crack = TRUE;
		for(int j=nStart; j<nLast; j++) 
		{

			InData.Initialize();
			if(m_PscdD.bStrePrint4AASHTO[3] && BdcwResD.BdcwBase[j].LInf.bCHK)
			{
				InData = arInData.GetAt(j);
				if (bCompositeElem)
				{
					if (j%2==0) continue; // PSC Composite 에서 Top일때는 출력하지 않음 (AASHTO-LRFD 5.14.1.4.5)
					myAashtoPscReport.Print_Detail_CrackCheck(m_PscdD.iDgnCode, ElemKey, j, bFirst_crack, arInData.GetAt(j), BdcwResD.BdcwBase[j]);
				}
				else if (!bCompositeElem)
				{
					myAashtoPscReport.Print_Detail_CrackCheck(m_PscdD.iDgnCode, ElemKey, j, bFirst_crack, arInData.GetAt(j), BdcwResD.BdcwBase[j]);
				}
				else ASSERT(0);    
				bFirst_crack = FALSE;
			}      
		}
	}

	//Construction Stage Stress
	int nCountIJ[2]={0,0};
	BOOL bFirstIJ[2] = {FALSE, FALSE};
	for(int nIJ=0; nIJ<2; nIJ++)
	{
		int nStart=0;
		int nLast=0;
		if(nIJ==0) { nStart=0; nLast=2; }
		else       { nStart=2; nLast=4; }

		for(int j=nStart; j<nLast; j++) 
		{
			InData.Initialize();
			if(m_PscdD.bServPrint4AASHTO[0] && FysgResD_Gr.FysgBase[j].bCHK)
			{
				if(j==0 && !bCheckConI)   continue;
				else if(j==1 && !bCheckConI)   continue;
				else if(j==2 && !bCheckConJ)   continue;
				else if(j==3 && !bCheckConJ)   continue;
				
				bFirstIJ[nIJ] = (nCountIJ[nIJ]==0);

				InData = arInData.GetAt(j);
				if(bCompositeElem)
				{
					myAashtoPscReport.Print_Detail_Allowable_Comp_Fysg(m_PscdD.iDgnCode, ElemKey, j, bFirstIJ[nIJ], arInData.GetAt(j), FysgResD_Gr.FysgBase[j], TRUE);
					//else              myAashtoPscReport.Print_Detail_Allowable_Comp_Fysg(m_PscdD.iDgnCode, ElemKey, j, FALSE, arInData.GetAt(j), FysgResD_Gr.FysgBase[j], TRUE);
				}     
				nCountIJ[nIJ]++;
			}      
		}

		for(int j=nStart; j<nLast; j++) 
		{
			InData.Initialize();
			if(m_PscdD.bServPrint4AASHTO[0] && FysgResD_Sl.FysgBase[j].bCHK)
			{
				if(j==0 && !bCheckConI)   continue;
				else if(j==1 && !bCheckConI)   continue;
				else if(j==2 && !bCheckConJ)   continue;
				else if(j==3 && !bCheckConJ)   continue;

				InData = arInData.GetAt(j);
				if(bCompositeElem)
				{
					myAashtoPscReport.Print_Detail_Allowable_Comp_Fysg(m_PscdD.iDgnCode, ElemKey, j, FALSE, arInData.GetAt(j), FysgResD_Sl.FysgBase[j], FALSE);          
				}        
			}      
		}
}

	//Serviceability Stress
	bFirstIJ[0] = bFirstIJ[1] = FALSE;
	nCountIJ[0] = nCountIJ[1] = 0;
	for(int nIJ=0; nIJ<2; nIJ++)
	{
		int nStart=0;
		int nLast=0;
		if(nIJ==0) { nStart=0; nLast=2; }
		else       { nStart=2; nLast=4; }

		for(int j=nStart; j<nLast; j++) 
		{
			InData.Initialize();
			if(m_PscdD.bServPrint4AASHTO[1] && FycmResD_Gr.FycmBase[j].bCHK)
			{
				if(j==0 && !bCheckConI)   continue;
				else if(j==1 && !bCheckConI)   continue;
				else if(j==2 && !bCheckConJ)   continue;
				else if(j==3 && !bCheckConJ)   continue;

				bFirstIJ[nIJ] = (nCountIJ[nIJ]==0);

				InData = arInData.GetAt(j);
				if(bCompositeElem)
				{
					myAashtoPscReport.Print_Detail_Allowable_Comp_Fycm(m_PscdD.iDgnCode, ElemKey, j,  bFirstIJ[nIJ], arInData.GetAt(j), FycmResD_Gr.FycmBase[j], TRUE);
					//else      myAashtoPscReport.Print_Detail_Allowable_Comp_Fycm(m_PscdD.iDgnCode, ElemKey, j, FALSE, arInData.GetAt(j), FycmResD_Gr.FycmBase[j], TRUE);
				}
				nCountIJ[nIJ]++;
			}
		}

		for(int j=nStart; j<nLast; j++) 
		{
			InData.Initialize();
			if(m_PscdD.bServPrint4AASHTO[1] && FycmResD_Sl.FycmBase[j].bCHK)
			{
				if(j==0 && !bCheckConI)   continue;
				else if(j==1 && !bCheckConI)   continue;
				else if(j==2 && !bCheckConJ)   continue;
				else if(j==3 && !bCheckConJ)   continue;

				InData = arInData.GetAt(j);
				if(bCompositeElem)
				{
					if(j==0 || j==2)  myAashtoPscReport.Print_Detail_Allowable_Comp_Fycm(m_PscdD.iDgnCode, ElemKey, j,  TRUE, arInData.GetAt(j), FycmResD_Sl.FycmBase[j], FALSE);
					else              myAashtoPscReport.Print_Detail_Allowable_Comp_Fycm(m_PscdD.iDgnCode, ElemKey, j, FALSE, arInData.GetAt(j), FycmResD_Sl.FycmBase[j], FALSE);
				}
			}
		}   
		
	}

	return TRUE;
}



void CCRCExcelOutput::ShouldPrintPosxBarIJ(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ)
{
	ShouldPrintPosxIJ(posx.iBarChk[0], bPrintI, bPrintJ);
}

void CCRCExcelOutput::ShouldPrintPosxMomIJ(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ)
{
	ShouldPrintPosxIJ(posx.iMomChk[0], bPrintI, bPrintJ);
}

void CCRCExcelOutput::ShouldPrintPosxMomIJ_Negative(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ)
{
	ShouldPrintPosxIJ(posx.iMomChk[1], bPrintI, bPrintJ);
}


void CCRCExcelOutput::ShouldPrintPosxShrIJ(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ)
{
	ShouldPrintPosxIJ(posx.iShrChk, bPrintI, bPrintJ);
}

void CCRCExcelOutput::ShouldPrintPosxTorIJ(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ)
{
	ShouldPrintPosxIJ(posx.iTorChk, bPrintI, bPrintJ);
}

void CCRCExcelOutput::ShouldPrintPosxFatIJ(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ)
{
	ShouldPrintPosxIJ(posx.nFatiChk, bPrintI, bPrintJ);
}

void CCRCExcelOutput::ShouldPrintPosxCraIJ(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ)
{
	ShouldPrintPosxIJ(posx.nCrack, bPrintI, bPrintJ);
}

void CCRCExcelOutput::ShouldPrintPosxIJ(int nFlag, BOOL &bPrintI, BOOL &bPrintJ)
{
	if      (nFlag==0)  {bPrintI=FALSE; bPrintJ=FALSE;}
	else if (nFlag==1)  {bPrintI=TRUE ; bPrintJ=FALSE;}	// I.
	else if (nFlag==2)  {bPrintI=FALSE; bPrintJ=TRUE;}	// J.
	else if (nFlag==3)  {bPrintI=TRUE ; bPrintJ=TRUE;}	// I&J.
	else	              ASSERT(0);	
}

BOOL CCRCExcelOutput::Execute_PSCReport_EC2_2_05(int iDgnCode, CString sSaveFileName)
{
	// SET PROGRESS-BAR
	if(m_bStopExecute) return FALSE;
	Progress(0, 0);
	int nProgressPercent = 0;

	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	CDgn_PSCRptManager_EC2  myEurocodePscReport;
	if(!myEurocodePscReport.Initialize(iDgnCode, m_pDoc->GetProgramPath(), GetSaveFileDirEx(), sSaveFileName)) return FALSE;

	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	//
	BOOL bSuccess = TRUE;

	if(!m_PscdD.bUltimateEuro[0]       && !m_PscdD.bUltimateEuro[1]       && !m_PscdD.bUltimateEuro[2]       &&
		 !m_PscdD.bServiceabilityEuro[0] && !m_PscdD.bServiceabilityEuro[1] && !m_PscdD.bServiceabilityEuro[2] &&
		 !m_PscdD.bServiceabilityEuro[3] && !m_PscdD.bServiceabilityEuro[4] && !m_PscdD.bServiceabilityEuro[5])
	{
		// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
		VERIFY(myEurocodePscReport.Terminate(iDgnCode, !bSuccess));

		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		return FALSE;
	}

	// Get all element keys for which results are to be printed
	CArray<T_POSX_K, T_POSX_K> arPrintElemKey;
	m_pDoc->m_pAttrCtrl->GetPosxKeyList(arPrintElemKey);

	CArray<T_POSV_K, T_POSV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl->GetPosvKeyList(aPrintVBeamKey);

	ArrElemPairKey aPrintEPairKey;
	CDBLib::ConvertToElemPairKeyList(arPrintElemKey, aPrintVBeamKey, aPrintEPairKey);

	int i=0, j=0, k=0;
	int kk=0;
	int nIJ=0, nMaxMin=0;
	BOOL bPrintI = FALSE, bPrintJ = FALSE;
	int nPrintElemSize = aPrintEPairKey.GetSize();  // Element Key..

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	if(nPrintElemSize==0)
	{
		VERIFY(myEurocodePscReport.Terminate(iDgnCode, !bSuccess));

		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		return FALSE;
	}

	//
	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40/nPrintElemSize;

	// Detail Res-data.
	CArray<PSC_FRCR_D,PSC_FRCR_D> arFrcrD;  arFrcrD.RemoveAll();  arFrcrD.SetSize(4);
	PSC_CUMS_D EngCumsD;  EngCumsD.Initialize();
	PSC_CRMT_D EngCrmtD;  EngCrmtD.Initialize();
	PSC_BDCW_D EngBdcwD;  EngBdcwD.Initialize();
	PSC_FATG_D EngFatgD;  EngFatgD.Initialize();
	//
	PSC_DATA_POSI InData;   InData.Initialize();

	BOOL bCheckMomI = FALSE,          bCheckMomJ = FALSE;
	BOOL bCheckMomI_Negative = FALSE, bCheckMomJ_Negative = FALSE;
	BOOL bCheckShrI = FALSE,          bCheckShrJ = FALSE;
	BOOL bCheckTorI = FALSE,          bCheckTorJ = FALSE;

	try
	{
		//DETATL-REPORT
		if(m_PscdD.bUltimateEuro[0] || m_PscdD.bUltimateEuro[1] || m_PscdD.bUltimateEuro[2] || m_PscdD.bServiceabilityEuro[5])
		{
			for(i=0; i<nPrintElemSize; ++i)
			{
				//
				auto ElemK = aPrintEPairKey[i];
				//
				T_POSC_D PoscD; PoscD.Initialize();
				T_POSX_D PosxD; PosxD.Initialize();
				if (ElemK.second == EN_EL_BEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPosc(ElemK.first, PoscD)) continue;
					if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD)) continue;
				}
				else if (ElemK.second == EN_EL_VBEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPovc(ElemK.first, PoscD)) continue;
					if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD)) continue;
				}
				else ASSERT(0);			

				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

				bCheckMomI = FALSE;
				bCheckMomJ = FALSE;
				ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);

				bCheckMomI_Negative = FALSE;
				bCheckMomJ_Negative = FALSE;
				ShouldPrintPosxMomIJ_Negative(PosxD, bCheckMomI_Negative, bCheckMomJ_Negative);

				bCheckShrI = FALSE;
				bCheckShrJ = FALSE;
				ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);

				bCheckTorI = FALSE;
				bCheckTorJ = FALSE;
				ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

				// SET DATA!
				T_BDCR_PSC       BdcrD;      BdcrD.Initialize();
				T_BDCR_PSC_COMP  BdcrDComp;  BdcrDComp.Initialize();
				PSC_FRCR_D       EngBdcrD;   EngBdcrD.Initialize();  //PSC_FRCR_D rData;

				T_CUMS_PSC       CumsD;	     CumsD.Initialize();
				T_CUMS_PSC_COMP  CumsDComp;	 CumsD.Initialize();
				PSC_CUMS_BASE    EngCumsD;   EngCumsD.Initialize();  //PSC_CUMS_BASE rData;

				T_CRMT_PSC       CrmtD;	     CrmtD.Initialize();
				T_CRMT_PSC_COMP  CrmtDComp;	 CrmtDComp.Initialize();
				PSC_CRMT_BASE    EngCrmtD;   EngCrmtD.Initialize();  //PSC_CRMT_BASE rData;
				
				T_BDCW_PSC       BdcwD;      BdcwD.Initialize();
				T_BDCW_PSC_COMP  BdcwDComp;  BdcwDComp.Initialize();
				PSC_BDCW_BASE    EngBdcwD;   EngBdcwD.Initialize();  //PSC_BDCW_BASE rData;

				T_FATG_PSC       FatgD;      FatgD.Initialize();
				PSC_FATG_BASE    EngFatgD;   EngFatgD.Initialize();  

				//GET DATA!
				for(j=0; j<2; j++) // I,J
				{
					// 다음의 경우를 제거할 수 있는지..확인할 것!!!!!!!
					// - Bending/Shear/Torsion을 출력하지 않고, Crack을 출력하는데, 
					// PrintOpt이 I/J 중 제거된 부분이 있다면..Detail을 출력하지 않도록 Sheet를 만들지 않아야 함.
					if(j==0 && !bCheckMomI && !bCheckMomI_Negative && !bCheckShrI && !bCheckTorI) continue;
					if(j==1 && !bCheckMomJ && !bCheckMomJ_Negative && !bCheckShrJ && !bCheckTorJ) continue;

					// 요소별 Position정보에 따라서 Sheet를 구분함. 즉 1_I / 1_J / 2_I / 2_J 등으로 Sheet 명칭 결정.
					// ***************************
					// Dgn Condition/Parameters
					// ***************************
					InData.Initialize();
					EngBdcrD.Initialize();
					if(bCompositeElem)
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, BdcrDComp))
						{
							m_DataCtrl.m_pPscDataCtrl->Get_BdcrCompReportData(ElemK, j, 0, BdcrDComp.BdcrBase[j*2], InData, EngBdcrD);
							if(!myEurocodePscReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
						else
						{
							if(!myEurocodePscReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
					}
					else
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, BdcrD))
						{
							// if j=0, index is 0 / j=1, index is 2!!
							// if j=0, index is 0 / j=1, index is 0!! k는 Max/Min 변수이므로, 항상 Posi별 Max정보로 가정함. InData를 가져오기 위함이니..
							m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData(ElemK, j, 0, BdcrD.BdcrBase[j*2], InData, EngBdcrD);
							if(!myEurocodePscReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
						else
							if(!myEurocodePscReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
					}
	
					// ***************************
					// CHK Bending Resistance!
					// ***************************
					if(m_PscdD.bUltimateEuro[0])
					{
						if(bCompositeElem)
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, BdcrDComp))
							{
								// Positive나 Negative가 하나라도 있으면..진행함.
								if((j==0 && (bCheckMomI || bCheckMomJ_Negative)) || (j==1 && (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++) // Positive/Negative
									{
										// 출력할 필요가 없는 부분은 빼자!!
										if(j==0 && k==0 && !bCheckMomI)           continue;
										if(j==0 && k==1 && !bCheckMomI_Negative)  continue;
										if(j==1 && k==0 && !bCheckMomJ)           continue;
										if(j==1 && k==1 && !bCheckMomJ_Negative)  continue;
										
										kk = 0;
										kk = j*2+k;
										if(!BdcrDComp.BdcrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
										InData.Initialize();
										EngBdcrD.Initialize();              
										m_DataCtrl.m_pPscDataCtrl->Get_BdcrCompReportData(ElemK, j, k, BdcrDComp.BdcrBase[kk], InData, EngBdcrD);
										
										//
										if(!myEurocodePscReport.Print_DetailReport_FlexureDesign(iDgnCode, ElemK, kk, InData, EngBdcrD))  {}
									}
								}
							}
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, BdcrD))
							{
								// Positive나 Negative가 하나라도 있으면..진행함.
								if((j==0 && (bCheckMomI || bCheckMomJ_Negative)) || (j==1 && (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++) // Positive/Negative
									{
										// 출력할 필요가 없는 부분은 빼자!!
										if(j==0 && k==0 && !bCheckMomI)           continue;
										if(j==0 && k==1 && !bCheckMomI_Negative)  continue;
										if(j==1 && k==0 && !bCheckMomJ)           continue;
										if(j==1 && k==1 && !bCheckMomJ_Negative)  continue;

										kk = 0;
										kk = j*2+k;
										if(!BdcrD.BdcrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngBdcrD.Initialize();              
										m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData(ElemK, j, k, BdcrD.BdcrBase[kk], InData, EngBdcrD);

										//
										if(!myEurocodePscReport.Print_DetailReport_FlexureDesign(iDgnCode, ElemK, kk, InData, EngBdcrD))  {}
									}
								}
							}
						}
					}

					// ***************************
					// CHK Shear Resistance!
					// ***************************
					if(m_PscdD.bUltimateEuro[1])
					{
						if(bCompositeElem)
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC_Comp(ElemK, CumsDComp))
							{
								T_BDCR_PSC_COMP Bdcr4ShrComp;
								m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp);
								if((j==0 && bCheckShrI) || (j==1 && bCheckShrJ))
								{
									for(k=0; k<2; k++) // Max/Min
									{          
										kk = 0;
										kk = j*2+k;
										if(!CumsDComp.CumsBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
										InData.Initialize();
										EngCumsD.Initialize();
										if (j==0) { EngCumsD.dMr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dphiMn : Bdcr4ShrComp.BdcrBase[0].dphiMn; }
										else      { EngCumsD.dMr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dphiMn : Bdcr4ShrComp.BdcrBase[2].dphiMn; }

										if (j==0) { EngCumsD.dz_sh = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
										else      { EngCumsD.dz_sh = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }
										
										if (j==0) { EngCumsD.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_gr : Bdcr4ShrComp.BdcrBase[0].dCc_gr; }
										else      { EngCumsD.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_gr : Bdcr4ShrComp.BdcrBase[2].dCc_gr; }

										if (j==0) { EngCumsD.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_sl : Bdcr4ShrComp.BdcrBase[0].dCc_sl; }
										else      { EngCumsD.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_sl : Bdcr4ShrComp.BdcrBase[2].dCc_sl; }

										if (j==0) { EngCumsD.BdcwBaseComp.dForce_T = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dForce_T : Bdcr4ShrComp.BdcrBase[0].dForce_T; }
										else      { EngCumsD.BdcwBaseComp.dForce_T = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dForce_T : Bdcr4ShrComp.BdcrBase[2].dForce_T; }

										if (j==0) { EngCumsD.BdcwBaseComp.dTst_sl = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dTst_sl : Bdcr4ShrComp.BdcrBase[0].dTst_sl; }
										else      { EngCumsD.BdcwBaseComp.dTst_sl = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dTst_sl : Bdcr4ShrComp.BdcrBase[2].dTst_sl; }

										m_DataCtrl.m_pPscDataCtrl->Get_CumsCompReportData(ElemK, j, k, CumsDComp.CumsBase[kk], InData, EngCumsD);
										
										//
										if(!myEurocodePscReport.Print_DetailReport_ShearDesign(iDgnCode, ElemK, kk, InData, EngCumsD)) {ASSERT(0);}
									}
								}
							}
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC(ElemK, CumsD))
							{
								if((j==0 && bCheckShrI) || (j==1 && bCheckShrJ))
								{
									for(k=0; k<2; k++) // Max/Min
									{          
										kk = 0;
										kk = j*2+k;
										if(!CumsD.CumsBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngCumsD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_CumsReportData(ElemK, j, k, CumsD.CumsBase[kk], InData, EngCumsD);

										//
										if(!myEurocodePscReport.Print_DetailReport_ShearDesign(iDgnCode, ElemK, kk, InData, EngCumsD)) {ASSERT(0);}
									}
								}
							}
						}
					}

					// ***************************
					// CHK Torsion Resistance!
					// ***************************
					if(m_PscdD.bUltimateEuro[2])
					{
						// 지금은 General section이므로 출력 보류!!
						if(bCompositeElem)
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC_Comp(ElemK, CrmtDComp))
							{
								T_BDCR_PSC_COMP Bdcr4ShrComp;
								m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp);

								if((j==0 && bCheckTorI) || (j==1 && bCheckTorJ))
								{
									for(k=0; k<3; k++) // Max/Min
									{          
										kk = 0;
										kk = j*3+k;
										if(!CrmtDComp.CrmtBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
										InData.Initialize();
										EngCrmtD.Initialize();

										if (j==0) { EngCrmtD.dz_sh = (CrmtDComp.CrmtBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
										else      { EngCrmtD.dz_sh = (CrmtDComp.CrmtBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

										m_DataCtrl.m_pPscDataCtrl->Get_CrmtCompReportData(ElemK, j, k, CrmtDComp.CrmtBase[kk], InData, EngCrmtD);
										
										//
										if(!myEurocodePscReport.Print_DetailReport_TorsionDesign(iDgnCode, ElemK, kk, InData, EngCrmtD)) {ASSERT(0);}
									}
								}
							} 
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC(ElemK, CrmtD))
							{
								if((j==0 && bCheckTorI) || (j==1 && bCheckTorJ))
								{
									for(k=0; k<3; k++) // Max/Min
									{          
										kk = 0;
										kk = j*3+k;
										if(!CrmtD.CrmtBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngCrmtD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_CrmtReportData(ElemK, j, k, CrmtD.CrmtBase[kk], InData, EngCrmtD);

										//
										if(!myEurocodePscReport.Print_DetailReport_TorsionDesign(iDgnCode, ElemK, kk, InData, EngCrmtD)) {ASSERT(0);}
									}
								}
							}
						}
					}

					// ***************************
					// CHK Crack
					// ***************************
					if(m_PscdD.bServiceabilityEuro[5])
					{
						if(bCompositeElem) // 합성
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_PSC_Comp(ElemK, BdcwDComp))
							{
								// 영응력 검토
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{

										kk = 0;
										kk = j*2+k;
										if(!BdcwDComp.BdcwBaseZero[kk].bCHK) continue;  // kk : 0=I-Top, 1=I-Bot, 2=J-Top, 3=J-Bot 

										InData.Initialize();
										EngBdcwD.Initialize();
										if (iDgnCode != ABNT_NBR_6118_2023_PSC)
										{
											m_DataCtrl.m_pPscDataCtrl->Get_BdcwCompReportData(ElemK, j, k, BdcwDComp.BdcwBaseZero[kk], InData, EngBdcwD);

											if (!myEurocodePscReport.Print_DetailReport_ZeroStressDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) { ASSERT(0); }
										}
									}
								}

								// 균열
								// Crack은 Moment와 동일한 조건을 고려하도록 한다. (Agree with JOAHN)
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!BdcwDComp.BdcwBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngBdcwD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_BdcwCompReportData(ElemK, j, k, BdcwDComp.BdcwBase[kk], InData, EngBdcwD);
 										if(!myEurocodePscReport.Print_DetailReport_CrackDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) {ASSERT(0);}
										
										// Asmin report 출력을 위한 Data 추가
										if (iDgnCode != ABNT_NBR_6118_2023_PSC)
										{
											m_DataCtrl.m_pPscDataCtrl->Get_BdcwCompAsminReportData(ElemK, kk, BdcwDComp.BdcwBaseAsmin[kk], EngBdcwD);
											if (!myEurocodePscReport.Print_DetailReport_AsminCHKDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) { ASSERT(0); }
										}
									}
								}
							}
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_PSC(ElemK, BdcwD))
							{
								// 영응력 검토
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!BdcwD.BdcwBaseZero[kk].bCHK) continue;  // kk : 0=I-Top, 1=I-Bot, 2=J-Top, 3=J-Bot 

										InData.Initialize();
										EngBdcwD.Initialize();
										if (iDgnCode != ABNT_NBR_6118_2023_PSC)
										{
											m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData(ElemK, j, k, BdcwD.BdcwBaseZero[kk], InData, EngBdcwD);

											if (!myEurocodePscReport.Print_DetailReport_ZeroStressDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) { ASSERT(0); }
										}
									}
								}
								// Crack은 Moment와 동일한 조건을 고려하도록 한다. (Agree with JOAHN)
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!BdcwD.BdcwBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngBdcwD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData(ElemK, j, k, BdcwD.BdcwBase[kk], InData, EngBdcwD);

										if(!myEurocodePscReport.Print_DetailReport_CrackDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) {ASSERT(0);}
										// Asmin report 출력을 위한 Data 추가
										if (iDgnCode != ABNT_NBR_6118_2023_PSC)
										{
											m_DataCtrl.m_pPscDataCtrl->Get_BdcwAsminReportData(ElemK, kk, BdcwD.BdcwBaseAsmin[kk], EngBdcwD);
											if (!myEurocodePscReport.Print_DetailReport_AsminCHKDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) { ASSERT(0); }
										}
									}
								}
							}
						} 
					} // CHK Crack 

					// ***************************
					// CHK Fatigue
					// ***************************
					if(TRUE/*m_PscdD.bServiceabilityEuro[5]*/)
					{ 
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFatg_PSC(ElemK, FatgD))
						{
							
							if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
							{
								for(k=0; k<2; k++)
								{
									kk = 0;
									kk = j*2+k;
									if(!FatgD.FatgBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

									InData.Initialize();
									EngFatgD.Initialize();
									m_DataCtrl.m_pPscDataCtrl->Get_FatgReportData(ElemK, j, k, FatgD.FatgBase[kk], InData, EngFatgD);
								}
							}
						}
					}
					// ***************************
					// CHK Construction Stage Stress
					// ***************************
					if (m_PscdD.bServiceabilityEuro[0])
					{
						if (bCompositeElem)
						{
							T_FYSG_PSC_COMP FysgDComp; FysgDComp.Initialize();
							if (m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_PSC_Comp(ElemK, FysgDComp))
							{
								PSC_FYSG_BASE EngFysgD; 
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!FysgDComp.FysgGrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										InData.Initialize(); EngFysgD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_FysgCompDetailReportData(ElemK, j, k, FysgDComp.FysgGrBase[kk], InData, EngFysgD);
										InData.nGirder = PSC_DATA_POSI::Girder; //girder
										if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fysg(iDgnCode, ElemK, kk, InData, EngFysgD)) {ASSERT(0);}

										if(!FysgDComp.FysgSlBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										InData.Initialize(); EngFysgD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_FysgCompDetailReportData(ElemK, j, k, FysgDComp.FysgSlBase[kk], InData, EngFysgD);
										InData.nGirder = PSC_DATA_POSI::Slab; //slab
										if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fysg(iDgnCode, ElemK, kk, InData, EngFysgD)) {ASSERT(0);}
									}
								}
							}
						}
						else
						{
							T_FYSG_PSC FysgD; FysgD.Initialize();
							if (m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_PSC(ElemK, FysgD))
							{
								PSC_FYSG_BASE EngFysgD; 
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!FysgD.FysgBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										InData.Initialize(); EngFysgD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_FysgDetailReportData(ElemK, j, k, FysgD.FysgBase[kk], InData, EngFysgD);
										InData.nGirder = PSC_DATA_POSI::Girder; //girder
										if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fysg(iDgnCode, ElemK, kk, InData, EngFysgD)) {ASSERT(0);}
									}
								}
							}
						}
					}
					// ***************************
					// CHK Service Load Combination Stress
					// ***************************
					if (m_PscdD.bServiceabilityEuro[1])
					{
						if (bCompositeElem)
						{
							T_FYCM_PSC_COMP FycmDComp; FycmDComp.Initialize();
							if (m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_PSC_Comp(ElemK, FycmDComp))
							{
								PSC_FYCM_BASE EngFycmD; 
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!FycmDComp.FycmGrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										InData.Initialize(); EngFycmD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_FycmCompDetailReportData(ElemK, j, k, FycmDComp.FycmGrBase[kk], InData, EngFycmD);
										InData.nGirder = PSC_DATA_POSI::Girder; //girder
										if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fycm(iDgnCode, ElemK, kk, InData, EngFycmD)) {ASSERT(0);}

										if(!FycmDComp.FycmSlBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										InData.Initialize(); EngFycmD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_FycmCompDetailReportData(ElemK, j, k, FycmDComp.FycmSlBase[kk], InData, EngFycmD);
										InData.nGirder = PSC_DATA_POSI::Slab; //slab
										if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fycm(iDgnCode, ElemK, kk, InData, EngFycmD)) {ASSERT(0);}
									}
								}
							}
						}
						else
						{
							T_FYCM_PSC FycmD; FycmD.Initialize();
							if (m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_PSC(ElemK, FycmD))
							{
								PSC_FYCM_BASE EngFycmD; 
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!FycmD.FycmBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										InData.Initialize(); EngFycmD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_FycmDetailReportData(ElemK, j, k, FycmD.FycmBase[kk], InData, EngFycmD);
										InData.nGirder = PSC_DATA_POSI::Girder; //girder
										if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fycm(iDgnCode, ElemK, kk, InData, EngFycmD)) {ASSERT(0);}
									}
								}
							}
						}
					}


				
				} // I/J
				
				if(m_bStopExecute)	return FALSE;
				nProgressPercent += nProgressIncrementPercent;
				Progress(0, nProgressPercent);
				Progress(2, nProgressPercent);

			} // for nPrintElemSize
		}
	
		//RESULT-TABLE
		T_POSX_D PosxD; PosxD.Initialize();
		BOOL bReport= FALSE;
		BOOL bFinal = FALSE;
		BOOL bChk_I_Crk = FALSE;  // Searching Final Row!!
		BOOL bChk_J_Crk = FALSE;
		BOOL bChk_I_SCS = FALSE;  // Searching Final Row!!
		BOOL bChk_J_SCS = FALSE;
		BOOL bChk_I_SSV = FALSE;  // Searching Final Row!!
		BOOL bChk_J_SSV = FALSE;
		BOOL bChk_I_PSC = FALSE;  // Searching Final Row!!
		BOOL bChk_J_PSC = FALSE;
		BOOL bChk_I_PSS = FALSE;  // Searching Final Row!!
		BOOL bChk_J_PSS = FALSE;

		//
		bCheckMomI = FALSE;
		bCheckMomJ = FALSE;
		//
		bCheckShrI = FALSE;
		bCheckShrJ = FALSE;
		//
		bCheckTorI = FALSE;
		bCheckTorJ = FALSE;

        CArray<_T_RPT_TABLE<PSC_FRCR_D>, _T_RPT_TABLE<PSC_FRCR_D>&> aTRptBR; aTRptBR.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_CUMS_BASE>, _T_RPT_TABLE<PSC_CUMS_BASE>&> aTRptSR; aTRptSR.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_CRMT_BASE>, _T_RPT_TABLE<PSC_CRMT_BASE>&> aTRptTR; aTRptTR.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FYSG_BASE>, _T_RPT_TABLE<PSC_FYSG_BASE>&> aTRptSC; aTRptSC.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FYCM_BASE>, _T_RPT_TABLE<PSC_FYCM_BASE>&> aTRptSS; aTRptSS.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FPSG_BASE>, _T_RPT_TABLE<PSC_FPSG_BASE>&> aTRptPC; aTRptPC.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FPMS_BASE>, _T_RPT_TABLE<PSC_FPMS_BASE>&> aTRptPS; aTRptPS.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_BDCW_BASE>, _T_RPT_TABLE<PSC_BDCW_BASE>&> aTRptCC; aTRptCC.RemoveAll();

        nProgressIncrementPercent = 4;// nPrintElemSize == 0 ? 30 : 30/nPrintElemSize;
		int iPosiNo = 0;
		int iMaxMin = 0;
		for(i=0; i<nPrintElemSize; i++)
		{
			auto ElemK = aPrintEPairKey[i];

			T_POSC_D PoscD; PoscD.Initialize();
			PosxD.Initialize();
			if (ElemK.second == EN_EL_BEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetPosc(ElemK.first, PoscD)) continue;
				if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD)) {}
			}
			else if (ElemK.second == EN_EL_VBEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetPovc(ElemK.first, PoscD)) continue;
				if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD)) {}
			}
			else ASSERT(0);			

			BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

			//Detail Report의 변수는 Calc의 변수를 가져와야 함.
			//Result Table의 변수는 Civil내 저장된 변수를 가져와야 함.			

			bCheckMomI = FALSE;
			bCheckMomJ = FALSE;
			ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);

			bCheckShrI = FALSE;
			bCheckShrJ = FALSE;
			ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);

			bCheckTorI = FALSE;
			bCheckTorJ = FALSE;
			ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

			// ***************************
			// Bending Resistacne
			// ***************************
			bReport = m_PscdD.bUltimateEuro[0];
			if(bReport)
			{
				T_BDCR_PSC_COMP BdcrDComp; BdcrDComp.Initialize();
				T_BDCR_PSC      BdcrD;     BdcrD.Initialize();
				if(bCompositeElem)    
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, BdcrDComp)) continue;
				}
				else                  
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, BdcrD)) continue; 
				}

				PSC_FRCR_D dgnFrcr; dgnFrcr.Initialize(); 

				//
				for(j=0; j<4; ++j)
				{
					// DATA Check!!
					BOOL bCHK = (bCompositeElem)? BdcrDComp.BdcrBase[j].bCHK : BdcrD.BdcrBase[j].bCHK;
					if(!bCHK) continue;
					//
					InData.Initialize();
					dgnFrcr.Initialize(); 

					iPosiNo = 0;  iPosiNo = (j<2)? 0 : 1;
					iMaxMin = 0;  iMaxMin = j%2;

					if(bCompositeElem)  m_DataCtrl.m_pPscDataCtrl->Get_BdcrCompReportData(ElemK, iPosiNo, iMaxMin, BdcrDComp.BdcrBase[j], InData, dgnFrcr);
					else                m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData    (ElemK, iPosiNo, iMaxMin, BdcrD.BdcrBase[j],     InData, dgnFrcr);
				
					if(!dgnFrcr.bChk) continue; // 설계결과가 없을 경우 SKIP함.

					bFinal = TRUE;
					if(!bCheckMomI && !bCheckMomJ)  continue;
					else
					{
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						if((j<2 && bCheckMomI) || (j>1 && bCheckMomJ))
						{
							//if(!myEurocodePscReport.Print_FlexureDesign_EC(iDgnCode, ElemK, j, bFinal, dgnFrcr)) {} 

                            _T_RPT_TABLE<PSC_FRCR_D> rptD;
                            rptD.iIJ     = j;
                            rptD.RptData = dgnFrcr;
                            aTRptBR.Add(rptD);
						}
					}
				}
			}

			// ***************************
			// Shear Resistance
			// ***************************
			bReport = m_PscdD.bUltimateEuro[1];
			if(bReport)
			{
				T_BDCR_PSC_COMP Bdcr4ShrComp; Bdcr4ShrComp.Initialize();

				T_CUMS_PSC_COMP CumsDComp; CumsDComp.Initialize();
				T_CUMS_PSC CumsD; CumsD.Initialize();

				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);
				if(bCompositeElem) 
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC_Comp(ElemK, CumsDComp)) continue;

					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp)) continue;
				}
				else               
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC(ElemK, CumsD)) continue;
				}

				//
				PSC_CUMS_BASE dgncums;  dgncums.Initialize();      
				for(j=0; j<4; ++j)
				{
					//
					BOOL bCHK = (bCompositeElem)? CumsDComp.CumsBase[j].bCHK : CumsD.CumsBase[j].bCHK;
					if(!bCHK) continue;

					//
					InData.Initialize();
					dgncums.Initialize();

					iPosiNo = 0;  iPosiNo = (j<2)? 0 : 1;
					iMaxMin = 0;  iMaxMin = j%2;

					if(bCompositeElem)  
					{
						if (iPosiNo==0) { dgncums.dMr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dphiMn : Bdcr4ShrComp.BdcrBase[0].dphiMn; }
						else            { dgncums.dMr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dphiMn : Bdcr4ShrComp.BdcrBase[2].dphiMn; }
						
						if (iPosiNo==0) { dgncums.dz_sh = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
						else            { dgncums.dz_sh = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

						if (iPosiNo==0) { dgncums.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_gr : Bdcr4ShrComp.BdcrBase[0].dCc_gr; }
						else            { dgncums.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_gr : Bdcr4ShrComp.BdcrBase[2].dCc_gr; }

						if (iPosiNo==0) { dgncums.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_sl : Bdcr4ShrComp.BdcrBase[0].dCc_sl; }
						else            { dgncums.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_sl : Bdcr4ShrComp.BdcrBase[2].dCc_sl; }

						if (iPosiNo==0) { dgncums.BdcwBaseComp.dForce_T = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dForce_T : Bdcr4ShrComp.BdcrBase[0].dForce_T; }
						else            { dgncums.BdcwBaseComp.dForce_T = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dForce_T : Bdcr4ShrComp.BdcrBase[2].dForce_T; }

						if (iPosiNo==0) { dgncums.BdcwBaseComp.dTst_sl = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dTst_sl : Bdcr4ShrComp.BdcrBase[0].dTst_sl; }
						else            { dgncums.BdcwBaseComp.dTst_sl = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dTst_sl : Bdcr4ShrComp.BdcrBase[2].dTst_sl; }

						m_DataCtrl.m_pPscDataCtrl->Get_CumsCompReportData(ElemK, iPosiNo, iMaxMin, CumsDComp.CumsBase[j], InData, dgncums);
					}
					else                
					{
						m_DataCtrl.m_pPscDataCtrl->Get_CumsReportData    (ElemK, iPosiNo, iMaxMin, CumsD.CumsBase[j],     InData, dgncums);
					}

					if(!dgncums.bCHK) continue; // 설계결과가 없을 경우 SKIP함.

					bFinal = TRUE;
					if(!bCheckShrI && !bCheckShrJ)  continue;
					else
					{
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						if((j<2 && bCheckShrI) || (j>1 && bCheckShrJ))
						{
							//if(!myEurocodePscReport.Print_ShearDesign_EC(iDgnCode, ElemK, j, bFinal, dgncums))  throw FALSE;
                            _T_RPT_TABLE<PSC_CUMS_BASE> rptD;
                            rptD.iIJ = j;
                            rptD.RptData = dgncums;
                            aTRptSR.Add(rptD);
						}
					}
				}
			}

			// ***************************
			// Torsional Resistance
			// ***************************
			bReport = m_PscdD.bUltimateEuro[2];
			if(bReport)
			{
				T_BDCR_PSC_COMP Bdcr4ShrComp; Bdcr4ShrComp.Initialize();

				T_CRMT_PSC_COMP CrmtDComp; CrmtDComp.Initialize();
				T_CRMT_PSC      CrmtD;     CrmtD.Initialize();

				if(bCompositeElem)  
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC_Comp(ElemK, CrmtDComp)) continue;
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp)) continue;
				}
				else                
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC(ElemK, CrmtD)) continue;
				}

				//
				PSC_CRMT_BASE dgncrmt;  dgncrmt.Initialize();
				//
				for(j=0; j<6; ++j)
				{
					//
					BOOL bCHK = (bCompositeElem)? CrmtDComp.CrmtBase[j].bCHK : CrmtD.CrmtBase[j].bCHK;
					if(!bCHK) continue;

					//
					InData.Initialize();
					dgncrmt.Initialize();

					iPosiNo = 0;  iPosiNo = (j<3)? 0 : 1;
					iMaxMin = 0;  iMaxMin = j%3;

					if(bCompositeElem)  
					{
						
						if (iPosiNo==0) { dgncrmt.dz_sh = (CrmtDComp.CrmtBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
						else            { dgncrmt.dz_sh = (CrmtDComp.CrmtBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

						m_DataCtrl.m_pPscDataCtrl->Get_CrmtCompReportData(ElemK, iPosiNo, iMaxMin, CrmtDComp.CrmtBase[j], InData, dgncrmt);

					}
					else    
					{
						m_DataCtrl.m_pPscDataCtrl->Get_CrmtReportData    (ElemK, iPosiNo, iMaxMin, CrmtD.CrmtBase[j], InData, dgncrmt);
					}
					//
					if(!dgncrmt.bCHK) continue; // 설계결과가 없을 경우 SKIP함.

					bFinal = TRUE;
					if(!bCheckTorI && !bCheckTorJ)  continue;
					else
					{
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						if((j<3 && bCheckTorI) || (j>2 && bCheckTorJ))
						{
							//if(!myEurocodePscReport.Print_TorsionDesign_EC(iDgnCode, ElemK, j, bFinal, dgncrmt))  {}
                            _T_RPT_TABLE<PSC_CRMT_BASE> rptD;
                            rptD.iIJ = j;
                            rptD.RptData = dgncrmt;
                            aTRptTR.Add(rptD);
						}
					}
				}
			}

			// ************************************************
			// stress for cross section at a construction stage
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[0];
			if(bReport)
			{				
				T_FYSG_PSC      FysgD;     FysgD.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_PSC(ElemK, FysgD))	continue;
				PSC_FYSG_D dgnFysg; dgnFysg.Initialize();

				//
				if(FysgD.FysgBase[0].bCHK || FysgD.FysgBase[1].bCHK || FysgD.FysgBase[2].bCHK || FysgD.FysgBase[3].bCHK)
				{
					dgnFysg.Initialize();
					m_DataCtrl.m_pPscDataCtrl->Get_FysgReportData(ElemK, FysgD, dgnFysg);
					for(j=0; j<4; ++j)
					{
						if(!dgnFysg.FysgBase[j].bCHK) continue;
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						//if(!myEurocodePscReport.Print_AllowableStresses_Conc_ConstructionStage_EC(iDgnCode, ElemK, j, bFinal, dgnFysg.FysgBase[j]))	{}
                        _T_RPT_TABLE<PSC_FYSG_BASE> rptD;
                        rptD.iIJ = j;
                        rptD.RptData = dgnFysg.FysgBase[j];
                        aTRptSC.Add(rptD);
					}
				}
				
			}

			// ************************************************
			// stress for cross section at service loads
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[1];
			if(bReport)
			{
				T_FYCM_PSC FycmD; FycmD.Initialize();
				PSC_FYCM_D dgnFycm; dgnFycm.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_PSC(ElemK, FycmD)) continue;
				//
				if(FycmD.FycmBase[0].bCHK || FycmD.FycmBase[1].bCHK || FycmD.FycmBase[2].bCHK || FycmD.FycmBase[3].bCHK)
				{
					dgnFycm.Initialize();
					m_DataCtrl.m_pPscDataCtrl->Get_FycmReportData(ElemK, FycmD, dgnFycm);
					for(j=0; j<4; ++j)
					{
						if(!dgnFycm.FycmBase[j].bCHK) continue;
						if(j==1 || j==3) continue; // 결과 테이블과 일치 위해...
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						bFinal = TRUE;
						//if(!myEurocodePscReport.Print_AllowableStresses_Conc_ServiceLoad_EC(iDgnCode, ElemK, j, bFinal, dgnFycm.FycmBase[j]))  {}
                        _T_RPT_TABLE<PSC_FYCM_BASE> rptD;
                        rptD.iIJ = j;
                        rptD.RptData = dgnFycm.FycmBase[j];
                        aTRptSS.Add(rptD);
					}
				}
				
			}

			// ************************************************
			// principal stress at a construction stage
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[2];
			if(bReport)
			{
				T_FPSG_PSC FpsgD; FpsgD.Initialize();
				PSC_FPSG_D  dgnFpsg;  dgnFpsg.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpsg_PSC(ElemK, FpsgD)) continue;

				if(FpsgD.FpsgBase[0].bCHK || FpsgD.FpsgBase[1].bCHK || FpsgD.FpsgBase[2].bCHK || FpsgD.FpsgBase[3].bCHK)
				{
					dgnFpsg.Initialize();
					m_DataCtrl.m_pPscDataCtrl->Get_FpsgReportData(ElemK, FpsgD, dgnFpsg);
					for(j=0; j<4; ++j)
					{
						if(!dgnFpsg.FpsgBase[j].bCHK) continue;
						if(j==0 || j==2) continue; // 결과 테이블과 일치 위해...
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						bFinal = TRUE;
						//if(!myEurocodePscReport.Print_AllowablePrincipalStress_Conc_ConstructionStage_EC(iDgnCode, ElemK, j, bFinal, dgnFpsg.FpsgBase[j])) {}
                        _T_RPT_TABLE<PSC_FPSG_BASE> rptD;
                        rptD.iIJ = j;
                        rptD.RptData = dgnFpsg.FpsgBase[j];
                        aTRptPC.Add(rptD);
					}
				}
				
			}

			// ************************************************
			// principal stress at service loads
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[3];
			if(bReport)
			{
				T_FPMS_PSC FpmsD; FpmsD.Initialize();
				PSC_FPMS_D  dgnFpms;  dgnFpms.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpms_PSC(ElemK, FpmsD)) continue;

				if(FpmsD.FpmsBase[0].bCHK || FpmsD.FpmsBase[1].bCHK || FpmsD.FpmsBase[2].bCHK || FpmsD.FpmsBase[3].bCHK)
				{
					dgnFpms.Initialize();          
					m_DataCtrl.m_pPscDataCtrl->Get_FpmsReportData(ElemK, FpmsD, dgnFpms);

					for(j=0; j<4; ++j)
					{
						if(!dgnFpms.FpmsBase[j].bCHK) continue;
						if(j==0 || j==2) continue; // 결과 테이블과 일치 위해...
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						bFinal = TRUE;
						//if(!myEurocodePscReport.Print_AllowablePrincipalStress_Conc_ServiceLoad_EC(iDgnCode, ElemK, j, bFinal, dgnFpms.FpmsBase[j])) {}
                        _T_RPT_TABLE<PSC_FPMS_BASE> rptD;
                        rptD.iIJ = j;
                        rptD.RptData = dgnFpms.FpmsBase[j];
                        aTRptPS.Add(rptD);
					}
				}				
			}

			// ************************************************
			// Crack Control
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[5];
			if(bReport)
			{
				T_BDCW_PSC BdcwD; BdcwD.Initialize();
				PSC_BDCW_BASE dgnbdcw;  dgnbdcw.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_PSC(ElemK, BdcwD)) continue;
			
				for(j=0; j<4; ++j)
				{
					//
					if(!BdcwD.BdcwBase[j].bCHK) continue;
					//
					InData.Initialize();
					dgnbdcw.Initialize();

					iPosiNo = 0;  iPosiNo = (j<2)? 0 : 1;
					iMaxMin = 0;  iMaxMin = j%2;
					m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData(ElemK, iPosiNo, iMaxMin, BdcwD.BdcwBase[j], InData, dgnbdcw);

					// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
					bFinal = TRUE;        
					// Crack은 Moment Check와 검토조건을 동일하게 한다. (Agree with JOAHN)
					if(!bCheckMomI && !bCheckMomJ)  continue;
					else
					{
						if((j<2 && bCheckMomI && dgnbdcw.bCHK) || (j>1 && bCheckMomJ && dgnbdcw.bCHK))
						{
							//if(!myEurocodePscReport.Print_CrackCheck_EC(iDgnCode, ElemK, j, bFinal, dgnbdcw))  {}
                            _T_RPT_TABLE<PSC_BDCW_BASE> rptD;
                            rptD.iIJ = j;
                            rptD.RptData = dgnbdcw;
                            aTRptCC.Add(rptD);
						}
					}
				}
			}

			if(m_bStopExecute)	return FALSE;
			//nProgressPercent += nProgressIncrementPercent;
			//Progress(0, nProgressPercent);
			//Progress(2, nProgressPercent);
		}
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);

        INT_PTR nTBR = aTRptBR.GetSize();
        for (int i=0; i<nTBR; ++i)
        {
            BOOL bFinal = (i==(nTBR-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FRCR_D> TRptD = aTRptBR.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_FlexureDesign_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTSR = aTRptSR.GetSize();
        for (int i = 0; i<nTSR; ++i)
        {
            BOOL bFinal = (i==(nTSR-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_CUMS_BASE> TRptD = aTRptSR.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_ShearDesign_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTTR = aTRptTR.GetSize();
        for (int i = 0; i<nTTR; ++i)
        {
            BOOL bFinal = (i==(nTTR-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_CRMT_BASE> TRptD = aTRptTR.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_TorsionDesign_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTSC = aTRptSC.GetSize();
        for (int i = 0; i<nTSC; ++i)
        {
            BOOL bFinal = (i==(nTSC-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FYSG_BASE> TRptD = aTRptSC.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_AllowableStresses_Conc_ConstructionStage_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTSS = aTRptSS.GetSize();
        for (int i = 0; i<nTSS; ++i)
        {
            BOOL bFinal = (i==(nTSS-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FYCM_BASE> TRptD = aTRptSS.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_AllowableStresses_Conc_ServiceLoad_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTPC = aTRptPC.GetSize();
        for (int i = 0; i<nTPC; ++i)
        {
            BOOL bFinal = (i==(nTPC-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FPSG_BASE> TRptD = aTRptPC.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_AllowablePrincipalStress_Conc_ConstructionStage_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTPS = aTRptPS.GetSize();
        for (int i = 0; i<nTPS; ++i)
        {
            BOOL bFinal = (i==(nTPS-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FPMS_BASE> TRptD = aTRptPS.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_AllowablePrincipalStress_Conc_ServiceLoad_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTCC = aTRptCC.GetSize();
        for (int i = 0; i<nTCC; ++i)
        {
            BOOL bFinal = (i==(nTCC-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_BDCW_BASE> TRptD = aTRptCC.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_CrackCheck_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;


		bReport = m_PscdD.bServiceabilityEuro[4];
		if(bReport)
		{
			CArray<T_TNDN_K,T_TNDN_K> arTndnList;  arTndnList.RemoveAll();
			m_pDoc->m_pPostCtrl->GetPCDesign()->GetPscCheckTdnaList(arTndnList);

			int iTndnKey  = 0;
			int iTndnList = 0;
			iTndnList = arTndnList.GetSize();

			for(i=0; i<iTndnList; ++i)
			{
				iTndnKey  = 0;
				iTndnKey  = arTndnList.GetAt(i);

				T_FYPC_AASHTO FypcD;    FypcD.Initialize();
				PSC_FYPC_D    dgnFypcD; dgnFypcD.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFypc_AASHTO(iTndnKey, FypcD)) continue;
				m_DataCtrl.m_pPscDataCtrl->Get_FypcReportData(iTndnKey, FypcD, dgnFypcD);

				bFinal = FALSE;
				if(i==iTndnList-1)  bFinal = TRUE;
				if(!myEurocodePscReport.Print_AllowableStress_Tendon_EC(iDgnCode, iTndnKey, bFinal, dgnFypcD))	{}
			}
			//
			if(m_bStopExecute)	return FALSE;
            nProgressPercent = 96;//90;
			Progress(0, nProgressPercent);
			Progress(2, nProgressPercent);
		}
	}
	catch(...)
	{
		bSuccess = FALSE;	
	}

	VERIFY(myEurocodePscReport.Terminate(iDgnCode, !bSuccess));

	if(m_bStopExecute)	return FALSE;
	nProgressPercent = 100;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	return bSuccess;
}

BOOL CCRCExcelOutput::Execute_PSCReport_IRC112(int iDgnCode, CString sSaveFileName)
{
	// SET PROGRESS-BAR
	if(m_bStopExecute) return FALSE;
	Progress(0, 0);
	int nProgressPercent = 0;

	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	CDgn_PSCRptManager_IRC112  myEurocodePscReport;
	if(!myEurocodePscReport.Initialize(iDgnCode, m_pDoc->GetProgramPath(), GetSaveFileDirEx(), sSaveFileName)) return FALSE;

	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	//
	BOOL bSuccess = TRUE;

	if(!m_PscdD.bUltimateEuro[0]       && !m_PscdD.bUltimateEuro[1]       && !m_PscdD.bUltimateEuro[2]       &&
	   !m_PscdD.bServiceabilityEuro[0] && !m_PscdD.bServiceabilityEuro[1] && !m_PscdD.bServiceabilityEuro[2] &&
	   !m_PscdD.bServiceabilityEuro[3] && !m_PscdD.bServiceabilityEuro[4] && !m_PscdD.bServiceabilityEuro[5])
	{
		// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
		VERIFY(myEurocodePscReport.Terminate(iDgnCode, !bSuccess));

		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		return FALSE;
	}

	// Get all element keys for which results are to be printed
	CArray<T_POSX_K, T_POSX_K> arPrintElemKey;
	m_pDoc->m_pAttrCtrl->GetPosxKeyList(arPrintElemKey);

	CArray<T_POSV_K, T_POSV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl->GetPosvKeyList(aPrintVBeamKey);

	ArrElemPairKey aPrintEPairKey;
	CDBLib::ConvertToElemPairKeyList(arPrintElemKey, aPrintVBeamKey, aPrintEPairKey);

	int i=0, j=0, k=0;
	int kk=0;
	int nIJ=0, nMaxMin=0;
	BOOL bPrintI = FALSE, bPrintJ = FALSE;
	int nPrintElemSize = aPrintEPairKey.GetSize();  // Element Key..

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	if(nPrintElemSize==0)
	{
		VERIFY(myEurocodePscReport.Terminate(iDgnCode, !bSuccess));

		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		return FALSE;
	}

	//
	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40/nPrintElemSize;

	// Detail Res-data.
	CArray<PSC_FRCR_D,PSC_FRCR_D> arFrcrD;  arFrcrD.RemoveAll();  arFrcrD.SetSize(4);
	PSC_CUMS_D EngCumsD;  EngCumsD.Initialize();
	PSC_CRMT_D EngCrmtD;  EngCrmtD.Initialize();
	PSC_BDCW_D EngBdcwD;  EngBdcwD.Initialize();
	PSC_FATG_D EngFatgD;  EngFatgD.Initialize();
	//
	PSC_DATA_POSI InData;   InData.Initialize();

	BOOL bCheckMomI = FALSE,          bCheckMomJ = FALSE;
	BOOL bCheckMomI_Negative = FALSE, bCheckMomJ_Negative = FALSE;
	BOOL bCheckShrI = FALSE,          bCheckShrJ = FALSE;
	BOOL bCheckTorI = FALSE,          bCheckTorJ = FALSE;

	try
	{
		//DETATL-REPORT
		if(m_PscdD.bUltimateEuro[0] || m_PscdD.bUltimateEuro[1] || m_PscdD.bUltimateEuro[2] || m_PscdD.bServiceabilityEuro[5])
		{
			for(i=0; i<nPrintElemSize; ++i)
			{
				//
				auto ElemK = aPrintEPairKey[i];
				//
				T_POSX_D PosxD; PosxD.Initialize();
				if (ElemK.second == EN_EL_BEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD)) continue;
				}
				else if (ElemK.second == EN_EL_VBEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD)) {}
				}
				else ASSERT(0);

				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

				bCheckMomI = FALSE;
				bCheckMomJ = FALSE;
				ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);

				bCheckMomI_Negative = FALSE;
				bCheckMomJ_Negative = FALSE;
				ShouldPrintPosxMomIJ_Negative(PosxD, bCheckMomI_Negative, bCheckMomJ_Negative);

				bCheckShrI = FALSE;
				bCheckShrJ = FALSE;
				ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);

				bCheckTorI = FALSE;
				bCheckTorJ = FALSE;
				ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

				// SET DATA!
				T_BDCR_PSC_IRC       BdcrD;      BdcrD.Initialize();
		        T_BDCR_PSC_COMP  BdcrDComp;  BdcrDComp.Initialize();
				PSC_FRCR_D       EngBdcrD;   EngBdcrD.Initialize();  //PSC_FRCR_D rData;

				T_CUMS_PSC_IRC       CumsD;	     CumsD.Initialize();
		        T_CUMS_PSC_COMP  CumsDComp;	 CumsD.Initialize();
				PSC_CUMS_BASE    EngCumsD;   EngCumsD.Initialize();  //PSC_CUMS_BASE rData;

				T_CRMT_PSC_IRC       CrmtD;	     CrmtD.Initialize();
		        T_CRMT_PSC_COMP  CrmtDComp;	 CrmtDComp.Initialize();
				PSC_CRMT_BASE    EngCrmtD;   EngCrmtD.Initialize();  //PSC_CRMT_BASE rData;
				
		        T_BDCW_PSC_IRC       BdcwD;      BdcwD.Initialize();
		        T_BDCW_PSC_COMP  BdcwDComp;  BdcwDComp.Initialize();
				PSC_BDCW_BASE    EngBdcwD;   EngBdcwD.Initialize();  //PSC_BDCW_BASE rData;

		        T_FATG_PSC_IRC       FatgD;      FatgD.Initialize();
				PSC_FATG_BASE    EngFatgD;   EngFatgD.Initialize();  

				//GET DATA!
				for(j=0; j<2; j++) // I,J
				{
					// 다음의 경우를 제거할 수 있는지..확인할 것!!!!!!!
					// - Bending/Shear/Torsion을 출력하지 않고, Crack을 출력하는데, 
					// PrintOpt이 I/J 중 제거된 부분이 있다면..Detail을 출력하지 않도록 Sheet를 만들지 않아야 함.
					if(j==0 && !bCheckMomI && !bCheckMomI_Negative && !bCheckShrI && !bCheckTorI) continue;
					if(j==1 && !bCheckMomJ && !bCheckMomJ_Negative && !bCheckShrJ && !bCheckTorJ) continue;

					// 요소별 Position정보에 따라서 Sheet를 구분함. 즉 1_I / 1_J / 2_I / 2_J 등으로 Sheet 명칭 결정.
					// ***************************
					// Dgn Condition/Parameters
					// ***************************
					InData.Initialize();
					EngBdcrD.Initialize();
					if(bCompositeElem)
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, BdcrDComp))
						{
							m_DataCtrl.m_pPscDataCtrl->Get_BdcrCompReportData(ElemK, j, 0, BdcrDComp.BdcrBase[j*2], InData, EngBdcrD);
							if(!myEurocodePscReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
						else
						{
							if(!myEurocodePscReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
					}
					else
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, BdcrD))
						{
							// if j=0, index is 0 / j=1, index is 2!!
							// if j=0, index is 0 / j=1, index is 0!! k는 Max/Min 변수이므로, 항상 Posi별 Max정보로 가정함. InData를 가져오기 위함이니..
							m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData(ElemK, j, 0, BdcrD.BdcrBase[j*2], InData, EngBdcrD);
							if(!myEurocodePscReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
						else
							if(!myEurocodePscReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
					}
	
					// ***************************
					// CHK Bending Resistance!
					// ***************************
					if(m_PscdD.bUltimateEuro[0])
					{
						if(bCompositeElem)
						{
								if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, BdcrDComp))
								{
								// Positive나 Negative가 하나라도 있으면..진행함.
								if((j==0 && (bCheckMomI || bCheckMomJ_Negative)) || (j==1 && (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++) // Positive/Negative
									{
									// 출력할 필요가 없는 부분은 빼자!!
									if(j==0 && k==0 && !bCheckMomI)           continue;
									if(j==0 && k==1 && !bCheckMomI_Negative)  continue;
									if(j==1 && k==0 && !bCheckMomJ)           continue;
									if(j==1 && k==1 && !bCheckMomJ_Negative)  continue;
										
									kk = 0;
									kk = j*2+k;
									if(!BdcrDComp.BdcrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
									InData.Initialize();
									EngBdcrD.Initialize();              
									m_DataCtrl.m_pPscDataCtrl->Get_BdcrCompReportData(ElemK, j, k, BdcrDComp.BdcrBase[kk], InData, EngBdcrD);
										
									//
									if(!myEurocodePscReport.Print_DetailReport_FlexureDesign(iDgnCode, ElemK, kk, InData, EngBdcrD))  {}
									}
								}
							}
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, BdcrD))
							{
								// Positive나 Negative가 하나라도 있으면..진행함.
								if((j==0 && (bCheckMomI || bCheckMomJ_Negative)) || (j==1 && (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++) // Positive/Negative
									{
										// 출력할 필요가 없는 부분은 빼자!!
										if(j==0 && k==0 && !bCheckMomI)           continue;
										if(j==0 && k==1 && !bCheckMomI_Negative)  continue;
										if(j==1 && k==0 && !bCheckMomJ)           continue;
										if(j==1 && k==1 && !bCheckMomJ_Negative)  continue;

										kk = 0;
										kk = j*2+k;
										if(!BdcrD.BdcrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngBdcrD.Initialize();              
										m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData(ElemK, j, k, BdcrD.BdcrBase[kk], InData, EngBdcrD);

										//
										if(!myEurocodePscReport.Print_DetailReport_FlexureDesign(iDgnCode, ElemK, kk, InData, EngBdcrD))  {}
									}
								}
							}
						}
					}

					// ***************************
					// CHK Shear Resistance!
					// ***************************
					if(m_PscdD.bUltimateEuro[1])
					{
						if(bCompositeElem)
						{
								if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC_Comp(ElemK, CumsDComp))
								{
								T_BDCR_PSC_COMP Bdcr4ShrComp;
								m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp);
								if((j==0 && bCheckShrI) || (j==1 && bCheckShrJ))
								{
									for(k=0; k<2; k++) // Max/Min
									{          
									kk = 0;
									kk = j*2+k;
									if(!CumsDComp.CumsBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
									InData.Initialize();
									EngCumsD.Initialize();
									if (j==0) { EngCumsD.dMr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dphiMn : Bdcr4ShrComp.BdcrBase[0].dphiMn; }
									else      { EngCumsD.dMr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dphiMn : Bdcr4ShrComp.BdcrBase[2].dphiMn; }

									if (j==0) { EngCumsD.dz_sh = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
									else      { EngCumsD.dz_sh = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }
										
									if (j==0) { EngCumsD.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_gr : Bdcr4ShrComp.BdcrBase[0].dCc_gr; }
									else      { EngCumsD.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_gr : Bdcr4ShrComp.BdcrBase[2].dCc_gr; }

									if (j==0) { EngCumsD.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_sl : Bdcr4ShrComp.BdcrBase[0].dCc_sl; }
									else      { EngCumsD.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_sl : Bdcr4ShrComp.BdcrBase[2].dCc_sl; }

									m_DataCtrl.m_pPscDataCtrl->Get_CumsCompReportData(ElemK, j, k, CumsDComp.CumsBase[kk], InData, EngCumsD);
										
									//
									if(!myEurocodePscReport.Print_DetailReport_ShearDesign(iDgnCode, ElemK, kk, InData, EngCumsD)) {ASSERT(0);}
									}
								}
								}
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC(ElemK, CumsD))
							{
								if((j==0 && bCheckShrI) || (j==1 && bCheckShrJ))
								{
									for(k=0; k<2; k++) // Max/Min
									{          
										kk = 0;
										kk = j*2+k;
										if(!CumsD.CumsBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngCumsD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_CumsReportData(ElemK, j, k, CumsD.CumsBase[kk], InData, EngCumsD);

										//
										if(!myEurocodePscReport.Print_DetailReport_ShearDesign(iDgnCode, ElemK, kk, InData, EngCumsD)) {ASSERT(0);}
									}
								}
							}
						}
					}

					// ***************************
					// CHK Torsion Resistance!
					// ***************************
					if(m_PscdD.bUltimateEuro[2])
					{
						// 지금은 General section이므로 출력 보류!!
						if(bCompositeElem)
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC_Comp(ElemK, CrmtDComp))
							{
							    T_BDCR_PSC_COMP Bdcr4ShrComp;
							    m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp);

							    if((j==0 && bCheckTorI) || (j==1 && bCheckTorJ))
							    {
								    for(k=0; k<3; k++) // Max/Min
								    {          
								        kk = 0;
								        kk = j*3+k;
								        if(!CrmtDComp.CrmtBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
								        InData.Initialize();
								        EngCrmtD.Initialize();

								        if (j==0) { EngCrmtD.dz_sh = (CrmtDComp.CrmtBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
								        else      { EngCrmtD.dz_sh = (CrmtDComp.CrmtBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

								        m_DataCtrl.m_pPscDataCtrl->Get_CrmtCompReportData(ElemK, j, k, CrmtDComp.CrmtBase[kk], InData, EngCrmtD);
										
								        //
								        if(!myEurocodePscReport.Print_DetailReport_TorsionDesign(iDgnCode, ElemK, kk, InData, EngCrmtD)) {ASSERT(0);}
								    }
							    }
							} 
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC(ElemK, CrmtD))
							{
								if((j==0 && bCheckTorI) || (j==1 && bCheckTorJ))
								{
									for(k=0; k<3; k++) // Max/Min
									{          
										kk = 0;
										kk = j*3+k;
										if(!CrmtD.CrmtBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngCrmtD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_CrmtReportData(ElemK, j, k, CrmtD.CrmtBase[kk], InData, EngCrmtD);

										//
										if(!myEurocodePscReport.Print_DetailReport_TorsionDesign(iDgnCode, ElemK, kk, InData, EngCrmtD)) {ASSERT(0);}
									}
								}
							}
						}
					}

					// ***************************
					// CHK Crack
					// ***************************
					if(m_PscdD.bServiceabilityEuro[5])
					{
						if(bCompositeElem) // 합성
						{
								if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_PSC_Comp(ElemK, BdcwDComp))
							{
							// 영응력 검토
							if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
							{
								for(k=0; k<2; k++)
								{
									_PSC_EXPC_ELEM ExpcElem;
									m_DataCtrl.m_pPscDataCtrl->Get_PscExpcElem(ElemK, ExpcElem);
									if (ExpcElem.ExpcPosi[k].iExposureClass[k] < 2 ) continue;

								    kk = 0;
								    kk = j*2+k;
								    if(!BdcwDComp.BdcwBaseZero[kk].bCHK) continue;  // kk : 0=I-Top, 1=I-Bot, 2=J-Top, 3=J-Bot 

								    InData.Initialize();
								    EngBdcwD.Initialize();
								    m_DataCtrl.m_pPscDataCtrl->Get_BdcwCompReportData(ElemK, j, k, BdcwDComp.BdcwBaseZero[kk], InData, EngBdcwD);
 
								    if(!myEurocodePscReport.Print_DetailReport_ZeroStressDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)){ASSERT(0);}
								}
							}

							// 균열
							// Crack은 Moment와 동일한 조건을 고려하도록 한다. (Agree with JOAHN)
							if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
							{
								for(k=0; k<2; k++)
								{
								    kk = 0;
								    kk = j*2+k;
								    if(!BdcwDComp.BdcwBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

								    InData.Initialize();
								    EngBdcwD.Initialize();
								    m_DataCtrl.m_pPscDataCtrl->Get_BdcwCompReportData(ElemK, j, k, BdcwDComp.BdcwBase[kk], InData, EngBdcwD);
 
									if(!myEurocodePscReport.Print_DetailReport_CrackDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) {ASSERT(0);}
								}
							}
							}
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_PSC(ElemK, BdcwD))
							{
								// Crack은 Moment와 동일한 조건을 고려하도록 한다. (Agree with JOAHN)
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!BdcwD.BdcwBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngBdcwD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData(ElemK, j, k, BdcwD.BdcwBase[kk], InData, EngBdcwD);

										if(!myEurocodePscReport.Print_DetailReport_CrackDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) {ASSERT(0);}
									}
								}
							}
						} 
					} // CHK Crack 

					// ***************************
					// CHK Fatigue
					// ***************************
					if(FALSE/*m_PscdD.bServiceabilityEuro[5]*/) // not print
					{ 
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFatg_PSC(ElemK, FatgD))
						{
							
							if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
							{
								for(k=0; k<2; k++)
								{
									kk = 0;
									kk = j*2+k;
									if(!FatgD.FatgBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

									InData.Initialize();
									EngFatgD.Initialize();
									m_DataCtrl.m_pPscDataCtrl->Get_FatgReportData(ElemK, j, k, FatgD.FatgBase[kk], InData, EngFatgD);

									if(!myEurocodePscReport.Print_DetailReport_FatigueDesign(iDgnCode, ElemK, kk, InData, EngFatgD)) {ASSERT(0);}
								}
							}
						}
					}
				
					// ***************************
					// CHK Construction Load Combination Stress
					// ***************************
					// ***************************
					// CHK Construction Stage Stress
					// ***************************
					if (m_PscdD.bServiceabilityEuro[0])
					{
						if (bCompositeElem)
						{
							T_FYSG_PSC_COMP FysgDComp; FysgDComp.Initialize();
							if (m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_PSC_Comp(ElemK, FysgDComp))
							{
								PSC_FYSG_BASE EngFysgD; 
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<1; k++)
									{
										kk = 0;
										kk = j*2+k;
                                        //Girder Compression
										if(!FysgDComp.FysgGrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										InData.Initialize(); EngFysgD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_FysgCompDetailReportData(ElemK, j, k, FysgDComp.FysgGrBase[kk], InData, EngFysgD);
										InData.nGirder = PSC_DATA_POSI::Girder; //girder
										if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fysg(iDgnCode, ElemK, kk, InData, EngFysgD)) {ASSERT(0);}
                                        //Girder Tension
                                        if(!FysgDComp.FysgGrBase[kk+1].bCHK) continue; // 설계결과가 없으면 Continue
                                        InData.Initialize(); EngFysgD.Initialize();
                                        m_DataCtrl.m_pPscDataCtrl->Get_FysgCompDetailReportData(ElemK, j, k+1, FysgDComp.FysgGrBase[kk+1], InData, EngFysgD);
                                        InData.nGirder = PSC_DATA_POSI::Girder; //girder
                                        if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fysg(iDgnCode, ElemK, kk+1, InData, EngFysgD)) {ASSERT(0);}

                                        //Slab Compression
										if(!FysgDComp.FysgSlBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										InData.Initialize(); EngFysgD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_FysgCompDetailReportData(ElemK, j, k, FysgDComp.FysgSlBase[kk], InData, EngFysgD);
										InData.nGirder = PSC_DATA_POSI::Slab; //slab
										if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fysg(iDgnCode, ElemK, kk, InData, EngFysgD)) {ASSERT(0);}
                                        //Slab Tension
                                        if(!FysgDComp.FysgSlBase[kk+1].bCHK) continue; // 설계결과가 없으면 Continue
                                        InData.Initialize(); EngFysgD.Initialize();
                                        m_DataCtrl.m_pPscDataCtrl->Get_FysgCompDetailReportData(ElemK, j, k+1, FysgDComp.FysgSlBase[kk+1], InData, EngFysgD);
                                        InData.nGirder = PSC_DATA_POSI::Slab; //slab
                                        if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fysg(iDgnCode, ElemK, kk+1, InData, EngFysgD)) {ASSERT(0);}
									}
								}
							}
						}
						else
						{
							T_FYSG_PSC_IRC FysgD; FysgD.Initialize();
							if (m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_PSC(ElemK, FysgD))
							{
								PSC_FYSG_BASE EngFysgD; 
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!FysgD.FysgBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										InData.Initialize(); EngFysgD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_FysgDetailReportData(ElemK, j, k, FysgD.FysgBase[kk], InData, EngFysgD);
										InData.nGirder = PSC_DATA_POSI::Girder; //girder
										if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fysg(iDgnCode, ElemK, kk, InData, EngFysgD)) {ASSERT(0);}
									}
								}
							}
						}
					}

					// ***************************
					// CHK Service Load Combination Stress
					// ***************************
					if (m_PscdD.bServiceabilityEuro[1])
					{
						if (bCompositeElem)
						{
							T_FYCM_PSC_COMP FycmDComp; FycmDComp.Initialize();
							if (m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_PSC_Comp(ElemK, FycmDComp))
							{
								PSC_FYCM_BASE EngFycmD; 
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<1; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!FycmDComp.FycmGrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
                                        //Girder Compression
										InData.Initialize(); EngFycmD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_FycmCompDetailReportData(ElemK, j, k, FycmDComp.FycmGrBase[kk], InData, EngFycmD);
										InData.nGirder = PSC_DATA_POSI::Girder; //girder
										if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fycm(iDgnCode, ElemK, kk, InData, EngFycmD)) {ASSERT(0);}
                                        //Girder Tension
                                        if(!FycmDComp.FycmGrBase[kk+1].bCHK) continue; // 설계결과가 없으면 Continue
                                        InData.Initialize(); EngFycmD.Initialize();
                                        m_DataCtrl.m_pPscDataCtrl->Get_FycmCompDetailReportData(ElemK, j, k+1, FycmDComp.FycmGrBase[kk+1], InData, EngFycmD);
                                        InData.nGirder = PSC_DATA_POSI::Girder; //girder
                                        if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fycm(iDgnCode, ElemK, kk+1, InData, EngFycmD)) {ASSERT(0);}

                                        //Slab Compression
										if(!FycmDComp.FycmSlBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										InData.Initialize(); EngFycmD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_FycmCompDetailReportData(ElemK, j, k, FycmDComp.FycmSlBase[kk], InData, EngFycmD);
										InData.nGirder = PSC_DATA_POSI::Slab; //slab
										if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fycm(iDgnCode, ElemK, kk, InData, EngFycmD)) {ASSERT(0);}
                                        //Slab Tension
                                        if(!FycmDComp.FycmSlBase[kk+1].bCHK) continue; // 설계결과가 없으면 Continue
                                        InData.Initialize(); EngFycmD.Initialize();
                                        m_DataCtrl.m_pPscDataCtrl->Get_FycmCompDetailReportData(ElemK, j, k+1, FycmDComp.FycmSlBase[kk+1], InData, EngFycmD);
                                        InData.nGirder = PSC_DATA_POSI::Slab; //slab
                                        if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fycm(iDgnCode, ElemK, kk+1, InData, EngFycmD)) {ASSERT(0);}
									}
								}
							}
						}
						else
						{
							T_FYCM_PSC_IRC FycmD; FycmD.Initialize();
							if (m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_PSC(ElemK, FycmD))
							{
								PSC_FYCM_BASE EngFycmD; 
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!FycmD.FycmBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										InData.Initialize(); EngFycmD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_FycmDetailReportData(ElemK, j, k, FycmD.FycmBase[kk], InData, EngFycmD);
										InData.nGirder = PSC_DATA_POSI::Girder; //girder
										if(!myEurocodePscReport.Print_DetailReport_AllowableStresses_Fycm(iDgnCode, ElemK, kk, InData, EngFycmD)) {ASSERT(0);}
									}
								}
							}
						}
					}

				} // I/J
				
				if(m_bStopExecute)	return FALSE;
				nProgressPercent += nProgressIncrementPercent;
				Progress(0, nProgressPercent);
				Progress(2, nProgressPercent);

			} // for nPrintElemSize
		}
	
		//RESULT-TABLE
		T_POSX_D PosxD; PosxD.Initialize();
		BOOL bReport= FALSE;
		BOOL bFinal = FALSE;
		BOOL bChk_I_Crk = FALSE;  // Searching Final Row!!
		BOOL bChk_J_Crk = FALSE;
		BOOL bChk_I_SCS = FALSE;  // Searching Final Row!!
		BOOL bChk_J_SCS = FALSE;
		BOOL bChk_I_SSV = FALSE;  // Searching Final Row!!
		BOOL bChk_J_SSV = FALSE;
		BOOL bChk_I_PSC = FALSE;  // Searching Final Row!!
		BOOL bChk_J_PSC = FALSE;
		BOOL bChk_I_PSS = FALSE;  // Searching Final Row!!
		BOOL bChk_J_PSS = FALSE;

		//
		bCheckMomI = FALSE;
		bCheckMomJ = FALSE;
		//
		bCheckShrI = FALSE;
		bCheckShrJ = FALSE;
		//
		bCheckTorI = FALSE;
		bCheckTorJ = FALSE;

        CArray<_T_RPT_TABLE<PSC_FRCR_D>, _T_RPT_TABLE<PSC_FRCR_D>&> aTRptBR; aTRptBR.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_CUMS_BASE>, _T_RPT_TABLE<PSC_CUMS_BASE>&> aTRptSR; aTRptSR.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_CRMT_BASE>, _T_RPT_TABLE<PSC_CRMT_BASE>&> aTRptTR; aTRptTR.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FYCM_BASE>, _T_RPT_TABLE<PSC_FYCM_BASE>&> aTRptSS; aTRptSS.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FPSG_BASE>, _T_RPT_TABLE<PSC_FPSG_BASE>&> aTRptPC; aTRptPC.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FPMS_BASE>, _T_RPT_TABLE<PSC_FPMS_BASE>&> aTRptPS; aTRptPS.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_BDCW_BASE>, _T_RPT_TABLE<PSC_BDCW_BASE>&> aTRptCC; aTRptCC.RemoveAll();

        nProgressIncrementPercent = 4;// nPrintElemSize == 0 ? 30 : 30/nPrintElemSize;
		int iPosiNo = 0;
		int iMaxMin = 0;
		for(i=0; i<nPrintElemSize; i++)
		{
			auto ElemK = aPrintEPairKey[i];

			BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

			//Detail Report의 변수는 Calc의 변수를 가져와야 함.
			//Result Table의 변수는 Civil내 저장된 변수를 가져와야 함.
			PosxD.Initialize();
			if (ElemK.second == EN_EL_BEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD)) {};
			}
			else if (ElemK.second == EN_EL_VBEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD)) {}
			}
			else ASSERT(0);

			bCheckMomI = FALSE;
			bCheckMomJ = FALSE;
			ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);

			bCheckShrI = FALSE;
			bCheckShrJ = FALSE;
			ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);

			bCheckTorI = FALSE;
			bCheckTorJ = FALSE;
			ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

			// ***************************
			// Bending Resistacne
			// ***************************
			bReport = m_PscdD.bUltimateEuro[0];
			if(bReport)
			{
				T_BDCR_PSC_COMP BdcrDComp; BdcrDComp.Initialize();
                T_BDCR_PSC_IRC  BdcrD;     BdcrD.Initialize();
				if(bCompositeElem)    
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, BdcrDComp)) continue;
				}
				else                  
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, BdcrD)) continue; 
				}

				PSC_FRCR_D dgnFrcr; dgnFrcr.Initialize(); 

				//
				for(j=0; j<4; ++j)
				{
					// DATA Check!!
					BOOL bCHK = (bCompositeElem)? BdcrDComp.BdcrBase[j].bCHK : BdcrD.BdcrBase[j].bCHK;
					if(!bCHK) continue;
					//
					InData.Initialize();
					dgnFrcr.Initialize(); 

					iPosiNo = 0;  iPosiNo = (j<2)? 0 : 1;
					iMaxMin = 0;  iMaxMin = j%2;

					if(bCompositeElem)  m_DataCtrl.m_pPscDataCtrl->Get_BdcrCompReportData(ElemK, iPosiNo, iMaxMin, BdcrDComp.BdcrBase[j], InData, dgnFrcr);
					else                m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData    (ElemK, iPosiNo, iMaxMin, BdcrD.BdcrBase[j],     InData, dgnFrcr);
				
					if(!dgnFrcr.bChk) continue; // 설계결과가 없을 경우 SKIP함.

					if(!bCheckMomI && !bCheckMomJ)  continue;
					else
					{
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						if((j<2 && bCheckMomI) || (j>1 && bCheckMomJ))
						{
                            _T_RPT_TABLE<PSC_FRCR_D> rptD;
                            rptD.iIJ = j;
                            rptD.RptData = dgnFrcr;
                            aTRptBR.Add(rptD);
						}
					}
				}
			}

			// ***************************
			// Shear Resistance
			// ***************************
			bReport = m_PscdD.bUltimateEuro[1];
			if(bReport)
			{
				T_BDCR_PSC_COMP Bdcr4ShrComp; Bdcr4ShrComp.Initialize();

				T_CUMS_PSC_COMP CumsDComp; CumsDComp.Initialize();
				T_CUMS_PSC_IRC  CumsD; CumsD.Initialize();

				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);
				if(bCompositeElem) 
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC_Comp(ElemK, CumsDComp)) continue;

					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp)) continue;
				}
				else               
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC(ElemK, CumsD)) continue;
				}

				//
				PSC_CUMS_BASE dgncums;  dgncums.Initialize();      
				for(j=0; j<4; ++j)
				{
					//
					BOOL bCHK = (bCompositeElem)? CumsDComp.CumsBase[j].bCHK : CumsD.CumsBase[j].bCHK;
					if(!bCHK) continue;

					//
					InData.Initialize();
					dgncums.Initialize();

					iPosiNo = 0;  iPosiNo = (j<2)? 0 : 1;
					iMaxMin = 0;  iMaxMin = j%2;

					if(bCompositeElem)  
					{
						if (iPosiNo==0) { dgncums.dMr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dphiMn : Bdcr4ShrComp.BdcrBase[0].dphiMn; }
						else            { dgncums.dMr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dphiMn : Bdcr4ShrComp.BdcrBase[2].dphiMn; }
						
						if (iPosiNo==0) { dgncums.dz_sh = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
						else            { dgncums.dz_sh = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

						if (iPosiNo==0) { dgncums.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_gr : Bdcr4ShrComp.BdcrBase[0].dCc_gr; }
						else            { dgncums.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_gr : Bdcr4ShrComp.BdcrBase[2].dCc_gr; }

						if (iPosiNo==0) { dgncums.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_sl : Bdcr4ShrComp.BdcrBase[0].dCc_sl; }
						else            { dgncums.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_sl : Bdcr4ShrComp.BdcrBase[2].dCc_sl; }

						m_DataCtrl.m_pPscDataCtrl->Get_CumsCompReportData(ElemK, iPosiNo, iMaxMin, CumsDComp.CumsBase[j], InData, dgncums);
					}
					else                
					{
						m_DataCtrl.m_pPscDataCtrl->Get_CumsReportData    (ElemK, iPosiNo, iMaxMin, CumsD.CumsBase[j],     InData, dgncums);
					}

					if(!dgncums.bCHK) continue; // 설계결과가 없을 경우 SKIP함.

					if(!bCheckShrI && !bCheckShrJ)  continue;
					else
					{
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						if((j<2 && bCheckShrI) || (j>1 && bCheckShrJ))
						{
                            _T_RPT_TABLE<PSC_CUMS_BASE> rptD;
                            rptD.iIJ = j;
                            rptD.RptData = dgncums;
                            aTRptSR.Add(rptD);
						}
					}
				}
			}

			// ***************************
			// Torsional Resistance
			// ***************************
			bReport = m_PscdD.bUltimateEuro[2];
			if(bReport)
			{
				T_BDCR_PSC_COMP Bdcr4ShrComp; Bdcr4ShrComp.Initialize();
				m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp);
				T_CRMT_PSC_COMP CrmtDComp; CrmtDComp.Initialize();
				T_CRMT_PSC_IRC  CrmtD;     CrmtD.Initialize();

				if(bCompositeElem)  
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC_Comp(ElemK, CrmtDComp)) continue;
				}
				else                
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC(ElemK, CrmtD)) continue;
				}

				//
				PSC_CRMT_BASE dgncrmt;  dgncrmt.Initialize();
				//
				for(j=0; j<6; ++j)
				{
					//
					BOOL bCHK = (bCompositeElem)? CrmtDComp.CrmtBase[j].bCHK : CrmtD.CrmtBase[j].bCHK;
					if(!bCHK) continue;

					//
					InData.Initialize();
					dgncrmt.Initialize();

					iPosiNo = 0;  iPosiNo = (j<3)? 0 : 1;
					iMaxMin = 0;  iMaxMin = j%3;

					if(bCompositeElem)  
					{
						
						if (iPosiNo==0) { dgncrmt.dz_sh = (CrmtDComp.CrmtBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
						else            { dgncrmt.dz_sh = (CrmtDComp.CrmtBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

						m_DataCtrl.m_pPscDataCtrl->Get_CrmtCompReportData(ElemK, iPosiNo, iMaxMin, CrmtDComp.CrmtBase[j], InData, dgncrmt);

					}
					else    
					{
						m_DataCtrl.m_pPscDataCtrl->Get_CrmtReportData    (ElemK, iPosiNo, iMaxMin, CrmtD.CrmtBase[j], InData, dgncrmt);
					}
					//
					if(!dgncrmt.bCHK) continue; // 설계결과가 없을 경우 SKIP함.

					if(!bCheckTorI && !bCheckTorJ)  continue;
					else
					{
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						if((j<3 && bCheckTorI) || (j>2 && bCheckTorJ))
						{
                            _T_RPT_TABLE<PSC_CRMT_BASE> rptD;
                            rptD.iIJ = j;
                            rptD.RptData = dgncrmt;
                            aTRptTR.Add(rptD);
						}
					}
				}
			}

			// ************************************************
			// stress for cross section at service loads
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[1];
			if(bReport)
			{				
				T_FYCM_PSC_IRC FycmD; FycmD.Initialize();
				PSC_FYCM_D dgnFycm; dgnFycm.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_PSC(ElemK, FycmD)) continue;
				//
				if(FycmD.FycmBase[0].bCHK || FycmD.FycmBase[1].bCHK || FycmD.FycmBase[2].bCHK || FycmD.FycmBase[3].bCHK)
				{
					dgnFycm.Initialize();
					m_DataCtrl.m_pPscDataCtrl->Get_FycmReportData(ElemK, FycmD, dgnFycm);
					for(j=0; j<4; ++j)
					{
						if(!dgnFycm.FycmBase[j].bCHK) continue;
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
                        _T_RPT_TABLE<PSC_FYCM_BASE> rptD;
                        rptD.iIJ = j;
                        rptD.RptData = dgnFycm.FycmBase[j];
                        aTRptSS.Add(rptD);
					}
				}
				
			}

			// ************************************************
			// principal stress at a construction stage
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[2];
			if(bReport)
			{
				T_FPSG_PSC_IRC FpsgD; FpsgD.Initialize();
				PSC_FPSG_D  dgnFpsg;  dgnFpsg.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpsg_PSC(ElemK, FpsgD)) continue;

				if(FpsgD.FpsgBase[0].bCHK || FpsgD.FpsgBase[1].bCHK || FpsgD.FpsgBase[2].bCHK || FpsgD.FpsgBase[3].bCHK)
				{
					dgnFpsg.Initialize();
					m_DataCtrl.m_pPscDataCtrl->Get_FpsgReportData(ElemK, FpsgD, dgnFpsg);
					for(j=0; j<4; ++j)
					{
						if(!dgnFpsg.FpsgBase[j].bCHK) continue;
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.						
                        _T_RPT_TABLE<PSC_FPSG_BASE> rptD;
                        rptD.iIJ = j;
                        rptD.RptData = dgnFpsg.FpsgBase[j];
                        aTRptPC.Add(rptD);
					}
				}
				
			}

			// ************************************************
			// principal stress at service loads
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[3];
			if(bReport)
			{
                T_FPMS_PSC_IRC FpmsD; FpmsD.Initialize();
				PSC_FPMS_D  dgnFpms;  dgnFpms.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpms_PSC(ElemK, FpmsD)) continue;

				if(FpmsD.FpmsBase[0].bCHK || FpmsD.FpmsBase[1].bCHK || FpmsD.FpmsBase[2].bCHK || FpmsD.FpmsBase[3].bCHK)
				{
					dgnFpms.Initialize();          
					m_DataCtrl.m_pPscDataCtrl->Get_FpmsReportData(ElemK, FpmsD, dgnFpms);

					for(j=0; j<4; ++j)
					{
						if(!dgnFpms.FpmsBase[j].bCHK) continue;
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.						
                        _T_RPT_TABLE<PSC_FPMS_BASE> rptD;
                        rptD.iIJ = j;
                        rptD.RptData = dgnFpms.FpmsBase[j];
                        aTRptPS.Add(rptD);
					}
				}
				
			}

			// ************************************************
			// Crack Control
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[5];
			if(bReport)
			{
				T_BDCW_PSC_IRC BdcwD; BdcwD.Initialize();
				PSC_BDCW_BASE dgnbdcw;  dgnbdcw.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_PSC(ElemK, BdcwD)) continue;
			
				for(j=0; j<4; ++j)
				{
					//
					if(!BdcwD.BdcwBase[j].bCHK) continue;
					//
					InData.Initialize();
					dgnbdcw.Initialize();

					iPosiNo = 0;  iPosiNo = (j<2)? 0 : 1;
					iMaxMin = 0;  iMaxMin = j%2;
					m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData(ElemK, iPosiNo, iMaxMin, BdcwD.BdcwBase[j], InData, dgnbdcw);

					// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
					// Crack은 Moment Check와 검토조건을 동일하게 한다. (Agree with JOAHN)
					if(!bCheckMomI && !bCheckMomJ)  continue;
					else
					{
						if((j<2 && bCheckMomI && dgnbdcw.bCHK) || (j>1 && bCheckMomJ && dgnbdcw.bCHK))
						{							
                            _T_RPT_TABLE<PSC_BDCW_BASE> rptD;
                            rptD.iIJ = j;
                            rptD.RptData = dgnbdcw;
                            aTRptCC.Add(rptD);
						}
					}
				}
			}

			if(m_bStopExecute)	return FALSE;
		}
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);

        INT_PTR nTBR = aTRptBR.GetSize();
        for (int i = 0; i<nTBR; ++i)
        {
            BOOL bFinal = (i==(nTBR-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FRCR_D> TRptD = aTRptBR.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_FlexureDesign_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTSR = aTRptSR.GetSize();
        for (int i = 0; i<nTSR; ++i)
        {
            BOOL bFinal = (i==(nTSR-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_CUMS_BASE> TRptD = aTRptSR.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_ShearDesign_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTTR = aTRptTR.GetSize();
        for (int i = 0; i<nTTR; ++i)
        {
            BOOL bFinal = (i==(nTTR-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_CRMT_BASE> TRptD = aTRptTR.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_TorsionDesign_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTSS = aTRptSS.GetSize();
        for (int i = 0; i<nTSS; ++i)
        {
            BOOL bFinal = (i==(nTSS-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FYCM_BASE> TRptD = aTRptSS.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_AllowableStresses_Conc_ServiceLoad_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTPC = aTRptPC.GetSize();
        for (int i = 0; i<nTPC; ++i)
        {
            BOOL bFinal = (i==(nTPC-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FPSG_BASE> TRptD = aTRptPC.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_AllowablePrincipalStress_Conc_ConstructionStage_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTPS = aTRptPS.GetSize();
        for (int i = 0; i<nTPS; ++i)
        {
            BOOL bFinal = (i==(nTPS-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FPMS_BASE> TRptD = aTRptPS.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_AllowablePrincipalStress_Conc_ServiceLoad_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTCC = aTRptCC.GetSize();
        for (int i = 0; i<nTCC; ++i)
        {
            BOOL bFinal = (i==(nTCC-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_BDCW_BASE> TRptD = aTRptCC.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_CrackCheck_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

		bReport = m_PscdD.bServiceabilityEuro[4];
		if(bReport)
		{
			CArray<T_TNDN_K,T_TNDN_K> arTndnList;  arTndnList.RemoveAll();
			m_pDoc->m_pPostCtrl->GetPCDesign()->GetPscCheckTdnaList(arTndnList);

			int iTndnKey  = 0;
			int iTndnList = 0;
			iTndnList = arTndnList.GetSize();

			for(i=0; i<iTndnList; ++i)
			{
				iTndnKey  = 0;
				iTndnKey  = arTndnList.GetAt(i);

				T_FYPC_AASHTO FypcD;    FypcD.Initialize();
				PSC_FYPC_D    dgnFypcD; dgnFypcD.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFypc_AASHTO(iTndnKey, FypcD)) continue;
				m_DataCtrl.m_pPscDataCtrl->Get_FypcReportData(iTndnKey, FypcD, dgnFypcD);

				bFinal = FALSE;
				if(i==iTndnList-1)  bFinal = TRUE;
				if(!myEurocodePscReport.Print_AllowableStress_Tendon_EC(iDgnCode, iTndnKey, bFinal, dgnFypcD))	{}
			}
			//
			if(m_bStopExecute)	return FALSE;
			nProgressPercent = 96;
			Progress(0, nProgressPercent);
			Progress(2, nProgressPercent);
		}
	}
	catch(...)
	{
		bSuccess = FALSE;	
	}

	VERIFY(myEurocodePscReport.Terminate(iDgnCode, !bSuccess));

	if(m_bStopExecute)	return FALSE;
	nProgressPercent = 100;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	return bSuccess;
}

BOOL CCRCExcelOutput::Execute_PSCReport_LSD12(int iDgnCode, CString sSaveFileName)
{
	// SET PROGRESS-BAR
	if(m_bStopExecute) return FALSE;
	Progress(0, 0);
	int nProgressPercent = 0;

	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	CDgn_PSCRptManager_LSD12_Comp myCompositePSCReport;
	if(!myCompositePSCReport.Initialize(iDgnCode, m_pDoc->GetProgramPath(), GetSaveFileDirEx(), sSaveFileName)) return FALSE;

	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	//
	BOOL bSuccess = TRUE;

	if(!m_PscdD.bUltimateEuro[0]       && !m_PscdD.bUltimateEuro[1]       && !m_PscdD.bUltimateEuro[2]       &&
		 !m_PscdD.bServiceabilityEuro[0] && !m_PscdD.bServiceabilityEuro[1] && !m_PscdD.bServiceabilityEuro[2] &&
		 !m_PscdD.bServiceabilityEuro[3] && !m_PscdD.bServiceabilityEuro[4] && !m_PscdD.bServiceabilityEuro[5])
	{
		// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
		VERIFY(myCompositePSCReport.Terminate(iDgnCode, !bSuccess));

		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		return FALSE;
	}

	// Get all element keys for which results are to be printed
	CArray<T_POSX_K, T_POSX_K> arPrintElemKey;
	m_pDoc->m_pAttrCtrl->GetPosxKeyList(arPrintElemKey);

	CArray<T_POSV_K, T_POSV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl->GetPosvKeyList(aPrintVBeamKey);

	ArrElemPairKey aPrintEPairKey;
	CDBLib::ConvertToElemPairKeyList(arPrintElemKey, aPrintVBeamKey, aPrintEPairKey);

	int i=0, j=0, k=0;
	int kk=0;
	int nIJ=0, nMaxMin=0;
	BOOL bPrintI = FALSE, bPrintJ = FALSE;
	int nPrintElemSize = aPrintEPairKey.GetSize();  // Element Key..

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	if(nPrintElemSize==0)
	{
		VERIFY(myCompositePSCReport.Terminate(iDgnCode, !bSuccess));

		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		return FALSE;
	}

	//
	int nProgressIncrementPercent = nPrintElemSize == 0 ? 60 : 60/nPrintElemSize;

	// Detail Res-data.
	CArray<PSC_FRCR_D,PSC_FRCR_D> arFrcrD;  arFrcrD.RemoveAll();  arFrcrD.SetSize(4);
	PSC_CUMS_D EngCumsD;  EngCumsD.Initialize();
	PSC_CRMT_D EngCrmtD;  EngCrmtD.Initialize();
	PSC_BDCW_D EngBdcwD;  EngBdcwD.Initialize();
	//
	PSC_DATA_POSI InData;   InData.Initialize();

	BOOL bCheckMomI = FALSE,          bCheckMomJ = FALSE;
	BOOL bCheckMomI_Negative = FALSE, bCheckMomJ_Negative = FALSE;
	BOOL bCheckShrI = FALSE,          bCheckShrJ = FALSE;
	BOOL bCheckTorI = FALSE,          bCheckTorJ = FALSE;

	try
	{
		//DETATL-REPORT
		if(m_PscdD.bUltimateEuro[0] || m_PscdD.bUltimateEuro[1] || m_PscdD.bUltimateEuro[2] || m_PscdD.bServiceabilityEuro[5])
		{
			T_ELEM_K  ElemK = 0;  
			for(i=0; i<nPrintElemSize; ++i)
			{
				//
				auto ElemK = aPrintEPairKey[i];
				//
				T_POSC_D PoscD; PoscD.Initialize();
				T_POSX_D PosxD; PosxD.Initialize();
				if (ElemK.second == EN_EL_BEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPosc(ElemK.first, PoscD)) continue;
					if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD)) continue;
				}
				else if (ElemK.second == EN_EL_VBEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPovc(ElemK.first, PoscD)) continue;
					if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD)) continue;
				}
				else ASSERT(0);

				
				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

				bCheckMomI = FALSE;
				bCheckMomJ = FALSE;
				ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);

				bCheckMomI_Negative = FALSE;
				bCheckMomJ_Negative = FALSE;
				ShouldPrintPosxMomIJ_Negative(PosxD, bCheckMomI_Negative, bCheckMomJ_Negative);

				bCheckShrI = FALSE;
				bCheckShrJ = FALSE;
				ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);

				bCheckTorI = FALSE;
				bCheckTorJ = FALSE;
				ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

				// SET DATA!
				T_BDCR_PSC       BdcrD;      BdcrD.Initialize();
				T_BDCR_PSC_COMP  BdcrDComp;  BdcrDComp.Initialize();
				PSC_FRCR_D       EngBdcrD;   EngBdcrD.Initialize();  //PSC_FRCR_D rData;
				
				T_CUMS_PSC       CumsD;	     CumsD.Initialize();
				T_CUMS_PSC_COMP  CumsDComp;	 CumsD.Initialize();
				PSC_CUMS_BASE    EngCumsD;   EngCumsD.Initialize();  //PSC_CUMS_BASE rData;
			 
				T_CRMT_PSC       CrmtD;	     CrmtD.Initialize();
				T_CRMT_PSC_COMP  CrmtDComp;	 CrmtDComp.Initialize();
				PSC_CRMT_BASE    EngCrmtD;   EngCrmtD.Initialize();  //PSC_CRMT_BASE rData;

				T_BDCW_PSC       BdcwD;      BdcwD.Initialize();
				T_BDCW_PSC_COMP  BdcwDComp;  BdcwDComp.Initialize();
				PSC_BDCW_BASE    EngBdcwD;   EngBdcwD.Initialize();  //PSC_BDCW_BASE rData;

				//GET DATA!
				for(j=0; j<2; j++) // I,J
				{
					// 다음의 경우를 제거할 수 있는지..확인할 것!!!!!!!
					// - Bending/Shear/Torsion을 출력하지 않고, Crack을 출력하는데, 
					// PrintOpt이 I/J 중 제거된 부분이 있다면..Detail을 출력하지 않도록 Sheet를 만들지 않아야 함.
					if(j==0 && !bCheckMomI && !bCheckMomI_Negative && !bCheckShrI && !bCheckTorI) continue;
					if(j==1 && !bCheckMomJ && !bCheckMomJ_Negative && !bCheckShrJ && !bCheckTorJ) continue;

					// 요소별 Position정보에 따라서 Sheet를 구분함. 즉 1_I / 1_J / 2_I / 2_J 등으로 Sheet 명칭 결정.
					// ***************************
					// Dgn Condition/Parameters
					// ***************************
					InData.Initialize();
					EngBdcrD.Initialize();
					if(bCompositeElem)
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, BdcrDComp))
						{
							m_DataCtrl.m_pPscDataCtrl->Get_BdcrCompReportData(ElemK, j, 0, BdcrDComp.BdcrBase[j*2], InData, EngBdcrD);
							if(!myCompositePSCReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
						else
						{
							if(!myCompositePSCReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
					}
					else
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, BdcrD))
						{
							// if j=0, index is 0 / j=1, index is 2!!
							// if j=0, index is 0 / j=1, index is 0!! k는 Max/Min 변수이므로, 항상 Posi별 Max정보로 가정함. InData를 가져오기 위함이니..
							m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData(ElemK, j, 0, BdcrD.BdcrBase[j*2], InData, EngBdcrD);
							if(!myCompositePSCReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
						else
						{
							if(!myCompositePSCReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
					}

					// ***************************
					// CHK Bending Resistance!
					// ***************************
					if(m_PscdD.bUltimateEuro[0])
					{
						if(bCompositeElem)
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, BdcrDComp))
							{
								// Positive나 Negative가 하나라도 있으면..진행함.
								if((j==0 && (bCheckMomI || bCheckMomJ_Negative)) || (j==1 && (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++) // Positive/Negative
									{
										// 출력할 필요가 없는 부분은 빼자!!
										if(j==0 && k==0 && !bCheckMomI)           continue;
										if(j==0 && k==1 && !bCheckMomI_Negative)  continue;
										if(j==1 && k==0 && !bCheckMomJ)           continue;
										if(j==1 && k==1 && !bCheckMomJ_Negative)  continue;
										
										kk = 0;
										kk = j*2+k;
										if(!BdcrDComp.BdcrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
										InData.Initialize();
										EngBdcrD.Initialize();              
										m_DataCtrl.m_pPscDataCtrl->Get_BdcrCompReportData(ElemK, j, k, BdcrDComp.BdcrBase[kk], InData, EngBdcrD);
										
										//
										if(!myCompositePSCReport.Print_DetailReport_FlexureDesign(iDgnCode, ElemK, kk, InData, EngBdcrD))  {}
									}
								}
							}
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, BdcrD))
							{
								// Positive나 Negative가 하나라도 있으면..진행함.
								if((j==0 && (bCheckMomI || bCheckMomJ_Negative)) || (j==1 && (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++) // Positive/Negative
									{
										// 출력할 필요가 없는 부분은 빼자!!
										if(j==0 && k==0 && !bCheckMomI)           continue;
										if(j==0 && k==1 && !bCheckMomI_Negative)  continue;
										if(j==1 && k==0 && !bCheckMomJ)           continue;
										if(j==1 && k==1 && !bCheckMomJ_Negative)  continue;
										
										kk = 0;
										kk = j*2+k;
										if(!BdcrD.BdcrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
										InData.Initialize();
										EngBdcrD.Initialize();              
										m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData(ElemK, j, k, BdcrD.BdcrBase[kk], InData, EngBdcrD);
										
										//
										if(!myCompositePSCReport.Print_DetailReport_FlexureDesign(iDgnCode, ElemK, kk, InData, EngBdcrD))  {}  
									}
								}
							}
						}
					}
					
					// ***************************
					// CHK Shear Resistance!
					// ***************************
					if(m_PscdD.bUltimateEuro[1])
					{
						if(bCompositeElem)
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC_Comp(ElemK, CumsDComp))
							{
								T_BDCR_PSC_COMP Bdcr4ShrComp;
								m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp);
								if((j==0 && bCheckShrI) || (j==1 && bCheckShrJ))
								{
									for(k=0; k<2; k++) // Max/Min
									{          
										kk = 0;
										kk = j*2+k;
										if(!CumsDComp.CumsBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
										InData.Initialize();
										EngCumsD.Initialize();
										if (j==0) { EngCumsD.dMr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dphiMn : Bdcr4ShrComp.BdcrBase[0].dphiMn; }
										else      { EngCumsD.dMr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dphiMn : Bdcr4ShrComp.BdcrBase[2].dphiMn; }
										
										if (j==0) { EngCumsD.dz_sh = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
										else      { EngCumsD.dz_sh = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

										if (j==0) { EngCumsD.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_gr : Bdcr4ShrComp.BdcrBase[0].dCc_gr; }
										else      { EngCumsD.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_gr : Bdcr4ShrComp.BdcrBase[2].dCc_gr; }

										if (j==0) { EngCumsD.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_sl : Bdcr4ShrComp.BdcrBase[0].dCc_sl; }
										else      { EngCumsD.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_sl : Bdcr4ShrComp.BdcrBase[2].dCc_sl; }

										m_DataCtrl.m_pPscDataCtrl->Get_CumsCompReportData(ElemK, j, k, CumsDComp.CumsBase[kk], InData, EngCumsD);
										
										//
										if(!myCompositePSCReport.Print_DetailReport_ShearDesign(iDgnCode, ElemK, kk, InData, EngCumsD)) {}
									}
								}
							}
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC(ElemK, CumsD))
							{
								T_BDCR_PSC Bdcr4Shr;
								m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, Bdcr4Shr);
								if((j==0 && bCheckShrI) || (j==1 && bCheckShrJ))
								{
									for(k=0; k<2; k++) // Max/Min
									{          
										kk = 0;
										kk = j*2+k;
										if(!CumsD.CumsBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
										InData.Initialize();
										EngCumsD.Initialize();
										if (j==0) { EngCumsD.dMr = (CumsD.CumsBase[kk].dMu<0.0) ? Bdcr4Shr.BdcrBase[1].dphiMn : Bdcr4Shr.BdcrBase[0].dphiMn; }
										else      { EngCumsD.dMr = (CumsD.CumsBase[kk].dMu<0.0) ? Bdcr4Shr.BdcrBase[3].dphiMn : Bdcr4Shr.BdcrBase[2].dphiMn; }
										m_DataCtrl.m_pPscDataCtrl->Get_CumsReportData(ElemK, j, k, CumsD.CumsBase[kk], InData, EngCumsD);
										
										//
										if(!myCompositePSCReport.Print_DetailReport_ShearDesign(iDgnCode, ElemK, kk, InData, EngCumsD)) {}
									}
								}
							}
						}
					}

					// ***************************
					// CHK Torsion Resistance!
					// ***************************
					if(m_PscdD.bUltimateEuro[2])
					{
						if(bCompositeElem)
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC_Comp(ElemK, CrmtDComp))
							{
								T_BDCR_PSC_COMP Bdcr4ShrComp;
								m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp);

								if((j==0 && bCheckTorI) || (j==1 && bCheckTorJ))
								{
									for(k=0; k<3; k++) // Max/Min
									{          
										kk = 0;
										kk = j*3+k;
										if(!CrmtDComp.CrmtBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngCrmtD.Initialize();

										if (j==0) { EngCrmtD.dz_sh = (CrmtDComp.CrmtBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
										else      { EngCrmtD.dz_sh = (CrmtDComp.CrmtBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

										m_DataCtrl.m_pPscDataCtrl->Get_CrmtCompReportData(ElemK, j, k, CrmtDComp.CrmtBase[kk], InData, EngCrmtD);
										
										//
										if(!myCompositePSCReport.Print_DetailReport_TorsionDesign(iDgnCode, ElemK, kk, InData, EngCrmtD)) {}
									}
								}
							} 
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC(ElemK, CrmtD))
							{
								if((j==0 && bCheckTorI) || (j==1 && bCheckTorJ))
								{
									for(k=0; k<3; k++) // Max/Min
									{          
										kk = 0;
										kk = j*3+k;
										if(!CrmtD.CrmtBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
										InData.Initialize();
										EngCrmtD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_CrmtReportData(ElemK, j, k, CrmtD.CrmtBase[kk], InData, EngCrmtD);
										
										//
										if(!myCompositePSCReport.Print_DetailReport_TorsionDesign(iDgnCode, ElemK, kk, InData, EngCrmtD)) {}
									}
								}
							} 
						}
					}
					
					// ***************************
					// CHK Crack
					// ***************************
					if(m_PscdD.bServiceabilityEuro[5])
					{

						if(bCompositeElem) // 합성
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_PSC_Comp(ElemK, BdcwDComp))
							{
								// 영응력 검토
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!BdcwDComp.BdcwBaseZero[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngBdcwD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_BdcwCompReportData(ElemK, j, k, BdcwDComp.BdcwBaseZero[kk], InData, EngBdcwD);
 
										if(!myCompositePSCReport.Print_DetailReport_ZeroStressDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) {}
									}
								}

								// 균열
								// Crack은 Moment와 동일한 조건을 고려하도록 한다. (Agree with JOAHN)
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!BdcwDComp.BdcwBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngBdcwD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_BdcwCompReportData(ElemK, j, k, BdcwDComp.BdcwBase[kk], InData, EngBdcwD);
 
										if(!myCompositePSCReport.Print_DetailReport_CrackDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) {}
									}
								}
							}
						}
						else // 비합성
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_PSC(ElemK, BdcwD))
							{
								// 영응력 검토
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!BdcwD.BdcwBaseZero[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngBdcwD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData(ElemK, j, k, BdcwD.BdcwBaseZero[kk], InData, EngBdcwD);
 
										if(!myCompositePSCReport.Print_DetailReport_ZeroStressDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) {}
									}
								}

								// Crack은 Moment와 동일한 조건을 고려하도록 한다. (Agree with JOAHN)
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										kk = 0;
										kk = j*2+k;
										if(!BdcwD.BdcwBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngBdcwD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData(ElemK, j, k, BdcwD.BdcwBase[kk], InData, EngBdcwD);

										if(!myCompositePSCReport.Print_DetailReport_CrackDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) {}
									}
								}
							}
						
						}
					}
				}
				//
				if(m_bStopExecute)	return FALSE;
				nProgressPercent += nProgressIncrementPercent;
				Progress(0, nProgressPercent);
				Progress(2, nProgressPercent);
			}
		}
	

		//RESULT-TABLE
		T_POSX_D PosxD; PosxD.Initialize();
		BOOL bReport= FALSE;
		BOOL bFinal = FALSE;
		BOOL bChk_I_Crk = FALSE;  // Searching Final Row!!
		BOOL bChk_J_Crk = FALSE;
		BOOL bChk_I_SCS = FALSE;  // Searching Final Row!!
		BOOL bChk_J_SCS = FALSE;
		BOOL bChk_I_SSV = FALSE;  // Searching Final Row!!
		BOOL bChk_J_SSV = FALSE;
		BOOL bChk_I_PSC = FALSE;  // Searching Final Row!!
		BOOL bChk_J_PSC = FALSE;
		BOOL bChk_I_PSS = FALSE;  // Searching Final Row!!
		BOOL bChk_J_PSS = FALSE;

		//
		bCheckMomI = FALSE;
		bCheckMomJ = FALSE;
		//
		bCheckShrI = FALSE;
		bCheckShrJ = FALSE;
		//
		bCheckTorI = FALSE;
		bCheckTorJ = FALSE;

        CArray<_T_RPT_TABLE<PSC_FRCR_D>, _T_RPT_TABLE<PSC_FRCR_D>&> aTRptBR; aTRptBR.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_CUMS_BASE>, _T_RPT_TABLE<PSC_CUMS_BASE>&> aTRptSR; aTRptSR.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_CRMT_BASE>, _T_RPT_TABLE<PSC_CRMT_BASE>&> aTRptTR; aTRptTR.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FYSG_BASE>, _T_RPT_TABLE<PSC_FYSG_BASE>&> aTRptSC; aTRptSC.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FYCM_BASE>, _T_RPT_TABLE<PSC_FYCM_BASE>&> aTRptSS; aTRptSS.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FPSG_BASE>, _T_RPT_TABLE<PSC_FPSG_BASE>&> aTRptPC; aTRptPC.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FPMS_BASE>, _T_RPT_TABLE<PSC_FPMS_BASE>&> aTRptPS; aTRptPS.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_BDCW_BASE>, _T_RPT_TABLE<PSC_BDCW_BASE>&> aTRptCC; aTRptCC.RemoveAll();

        nProgressIncrementPercent = 4;// nPrintElemSize == 0 ? 30 : 30/nPrintElemSize;
		int iPosiNo = 0;
		int iMaxMin = 0;
		for(i=0; i<nPrintElemSize; i++)
		{
			auto ElemK = aPrintEPairKey[i];

			BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

			//Detail Report의 변수는 Calc의 변수를 가져와야 함.
			//Result Table의 변수는 Civil내 저장된 변수를 가져와야 함.
			PosxD.Initialize();
			if (ElemK.second == EN_EL_BEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD)) {};
			}
			else if (ElemK.second == EN_EL_VBEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD)) {}
			}
			else ASSERT(0);

			bCheckMomI = FALSE;
			bCheckMomJ = FALSE;
			ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);

			bCheckShrI = FALSE;
			bCheckShrJ = FALSE;
			ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);

			bCheckTorI = FALSE;
			bCheckTorJ = FALSE;
			ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

			// ***************************
			// Bending Resistacne TABLE
			// ***************************
			bReport = m_PscdD.bUltimateEuro[0];
			if(bReport)
			{
				T_BDCR_PSC_COMP BdcrDComp; BdcrDComp.Initialize();
				T_BDCR_PSC      BdcrD;     BdcrD.Initialize();
				BOOL bExistBdcrResD = FALSE;
				if(bCompositeElem)    
				{
					bExistBdcrResD = m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, BdcrDComp);
				}
				else                  
				{
					bExistBdcrResD = m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, BdcrD);
				}
				//

				if(bExistBdcrResD)
				{
					PSC_FRCR_D dgnFrcr; dgnFrcr.Initialize(); 

					for(j=0; j<4; ++j)
					{
						// DATA Check!!
						BOOL bCHK = (bCompositeElem)? BdcrDComp.BdcrBase[j].bCHK : BdcrD.BdcrBase[j].bCHK;
						if(!bCHK) continue;
						//
						InData.Initialize();
						dgnFrcr.Initialize(); 

						iPosiNo = 0;  iPosiNo = (j<2)? 0 : 1;
						iMaxMin = 0;  iMaxMin = j%2;

						if(bCompositeElem)  m_DataCtrl.m_pPscDataCtrl->Get_BdcrCompReportData(ElemK, iPosiNo, iMaxMin, BdcrDComp.BdcrBase[j], InData, dgnFrcr);
						else                m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData    (ElemK, iPosiNo, iMaxMin, BdcrD.BdcrBase[j],     InData, dgnFrcr);

						if(!dgnFrcr.bChk) continue; // 설계결과가 없을 경우 SKIP함.

						bFinal = TRUE;
						if(!bCheckMomI && !bCheckMomJ)  continue;
						else
						{
							// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
							if((j<2 && bCheckMomI) || (j>1 && bCheckMomJ))
							{             
								//if(!myCompositePSCReport.Print_FlexureDesign_LSD12_Comp(iDgnCode, ElemK, j, bFinal, dgnFrcr)) {}

                                _T_RPT_TABLE<PSC_FRCR_D> rptD;
                                rptD.iIJ = j;
                                rptD.RptData = dgnFrcr;
                                aTRptBR.Add(rptD);
							}
						}
					}
				}				
			}

			// ***************************
			// Shear Resistance TABLE 
			// ***************************
			bReport = m_PscdD.bUltimateEuro[1];
			if(bReport)
			{
				T_CUMS_PSC_COMP CumsDComp; CumsDComp.Initialize();
				T_CUMS_PSC      CumsD;     CumsD.Initialize();
				T_BDCR_PSC_COMP Bdcr4ShrComp; Bdcr4ShrComp.Initialize();

				//
				BOOL bExistCumsResD = FALSE, bExistBdcrResD = FALSE;
				if(bCompositeElem) 
				{
					bExistCumsResD = m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC_Comp(ElemK, CumsDComp);
					bExistBdcrResD = m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp);
				}
				else               
				{
					bExistCumsResD = m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC(ElemK, CumsD);
				}

				BOOL bPrintShearRes = bCompositeElem ? (bExistCumsResD && bExistBdcrResD) : bExistCumsResD;

				if(bPrintShearRes)
				{
					PSC_CUMS_BASE dgncums;  dgncums.Initialize();      

					for(j=0; j<4; ++j)
					{
						//
						BOOL bCHK = (bCompositeElem)? CumsDComp.CumsBase[j].bCHK : CumsD.CumsBase[j].bCHK;
						if(!bCHK) continue;

						//
						InData.Initialize();
						dgncums.Initialize();

						iPosiNo = 0;  iPosiNo = (j<2)? 0 : 1;
						iMaxMin = 0;  iMaxMin = j%2;

						if(bCompositeElem) 
						{
							if (iPosiNo==0) { dgncums.dMr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dphiMn : Bdcr4ShrComp.BdcrBase[0].dphiMn; }
							else            { dgncums.dMr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dphiMn : Bdcr4ShrComp.BdcrBase[2].dphiMn; }

							if (iPosiNo==0) { dgncums.dz_sh = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
							else            { dgncums.dz_sh = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

							if (iPosiNo==0) { dgncums.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_gr : Bdcr4ShrComp.BdcrBase[0].dCc_gr; }
							else            { dgncums.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_gr : Bdcr4ShrComp.BdcrBase[2].dCc_gr; }

							if (iPosiNo==0) { dgncums.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_sl : Bdcr4ShrComp.BdcrBase[0].dCc_sl; }
							else            { dgncums.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_sl : Bdcr4ShrComp.BdcrBase[2].dCc_sl; }

							m_DataCtrl.m_pPscDataCtrl->Get_CumsCompReportData(ElemK, iPosiNo, iMaxMin, CumsDComp.CumsBase[j], InData, dgncums);
						}
						else
						{
							m_DataCtrl.m_pPscDataCtrl->Get_CumsReportData    (ElemK, iPosiNo, iMaxMin, CumsD.CumsBase[j],     InData, dgncums);
						}

						if(!dgncums.bCHK) continue; // 설계결과가 없을 경우 SKIP함.

						bFinal = TRUE;
						if(!bCheckShrI && !bCheckShrJ)  continue;
						else
						{
							// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
							if((j<2 && bCheckShrI) || (j>1 && bCheckShrJ))
							{
								//if(!myCompositePSCReport.Print_ShearDesign_LSD12_Comp(iDgnCode, ElemK, j, bFinal, dgncums))  throw FALSE;
                                _T_RPT_TABLE<PSC_CUMS_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.RptData = dgncums;
                                aTRptSR.Add(rptD);
							}
						}
					}
				}			
			}

			// ***************************
			// Torsional Resistance 
			// ***************************
			bReport = m_PscdD.bUltimateEuro[2];
			if(bReport)
			{
				T_BDCR_PSC_COMP Bdcr4ShrComp; Bdcr4ShrComp.Initialize();
				T_CRMT_PSC_COMP CrmtDComp; CrmtDComp.Initialize();
				T_CRMT_PSC      CrmtD;     CrmtD.Initialize();

				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);
				BOOL bExistCrmtResD = FALSE, bExistBdcrResD = FALSE;
				if(bCompositeElem)  
				{
					bExistCrmtResD = m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC_Comp(ElemK, CrmtDComp);
					bExistBdcrResD = m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp);
				}
				else                
				{
					bExistCrmtResD = m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC(ElemK, CrmtD);
				}

				BOOL bPrintTorsionRes = bCompositeElem ? (bExistCrmtResD && bExistBdcrResD) : bExistCrmtResD;
				
				if(bPrintTorsionRes)
				{				
					PSC_CRMT_BASE dgncrmt;  dgncrmt.Initialize();
					for(j=0; j<6; ++j)
					{
						//
						BOOL bCHK = (bCompositeElem)? CrmtDComp.CrmtBase[j].bCHK : CrmtD.CrmtBase[j].bCHK;
						if(!bCHK) continue;
						//
						InData.Initialize();
						dgncrmt.Initialize();

						iPosiNo = 0;  iPosiNo = (j<3)? 0 : 1;
						iMaxMin = 0;  iMaxMin = j%3;
						if(bCompositeElem)  
						{
							if (iPosiNo==0) { dgncrmt.dz_sh = (CrmtDComp.CrmtBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
							else            { dgncrmt.dz_sh = (CrmtDComp.CrmtBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

							m_DataCtrl.m_pPscDataCtrl->Get_CrmtCompReportData(ElemK, iPosiNo, iMaxMin, CrmtDComp.CrmtBase[j], InData, dgncrmt);
						}
						else
						{
							m_DataCtrl.m_pPscDataCtrl->Get_CrmtReportData    (ElemK, iPosiNo, iMaxMin, CrmtD.CrmtBase[j],     InData, dgncrmt);
						}
						//
						if(!dgncrmt.bCHK) continue; // 설계결과가 없을 경우 SKIP함.

						bFinal = TRUE;
						if(!bCheckTorI && !bCheckTorJ)  continue;
						else
						{
							// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
							if((j<3 && bCheckTorI) || (j>2 && bCheckTorJ))
							{
								//if(!myCompositePSCReport.Print_TorsionDesign_LSD12_Comp(iDgnCode, ElemK, j, bFinal, dgncrmt))  {}
                                _T_RPT_TABLE<PSC_CRMT_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.RptData = dgncrmt;
                                aTRptTR.Add(rptD);
							}
						}
					}
				}
			}

			// ************************************************
			// stress for cross section at a construction stage 
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[0];
			if(bReport)
			{
				if(bCompositeElem) // PSC Composite
				{
					T_FYSG_PSC_COMP FysgDComp; FysgDComp.Initialize();
					if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_PSC_Comp(ElemK, FysgDComp))
					{
						PSC_FYSG_D dgnFysg;
						// 거더 
						if(FysgDComp.FysgGrBase[0].bCHK || FysgDComp.FysgGrBase[1].bCHK || FysgDComp.FysgGrBase[2].bCHK || FysgDComp.FysgGrBase[3].bCHK )
						{
							dgnFysg.Initialize();
							m_DataCtrl.m_pPscDataCtrl->Get_FysgCompReportData(ElemK, FysgDComp, dgnFysg, TRUE);

							for(j=0; j<4; ++j)
							{
								if(!dgnFysg.FysgBase[j].bCHK) continue;
								// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
								//if(!myCompositePSCReport.Print_AllowableStresses_Conc_ConstructionStage_LSD12_Comp(iDgnCode, ElemK, j, bFinal, TRUE, dgnFysg.FysgBase[j]))	{}
                                _T_RPT_TABLE<PSC_FYSG_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.bGirder = TRUE;
                                rptD.RptData = dgnFysg.FysgBase[j];
                                aTRptSC.Add(rptD);
							}
						}

						// 슬래브
						if(FysgDComp.FysgSlBase[0].bCHK || FysgDComp.FysgSlBase[1].bCHK || FysgDComp.FysgSlBase[2].bCHK || FysgDComp.FysgSlBase[3].bCHK)
						{
							dgnFysg.Initialize();
							m_DataCtrl.m_pPscDataCtrl->Get_FysgCompReportData(ElemK, FysgDComp, dgnFysg, FALSE);

							for(j=0; j<4; ++j)
							{
								if(!dgnFysg.FysgBase[j].bCHK) continue;
								// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
								//if(!myCompositePSCReport.Print_AllowableStresses_Conc_ConstructionStage_LSD12_Comp(iDgnCode, ElemK, j, bFinal, FALSE, dgnFysg.FysgBase[j]))	{}
                                _T_RPT_TABLE<PSC_FYSG_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.bGirder = FALSE;
                                rptD.RptData = dgnFysg.FysgBase[j];
                                aTRptSC.Add(rptD);
							}
						}
					}
				}
				else // PSC
				{
					T_FYSG_PSC FysgD;
					// 거더 
					if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_PSC(ElemK, FysgD))
					{
						PSC_FYSG_D dgnFysg;
						m_DataCtrl.m_pPscDataCtrl->Get_FysgReportData(ElemK, FysgD, dgnFysg);

						for(j=0; j<4; ++j)
						{
							if(!dgnFysg.FysgBase[j].bCHK) continue;
							// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
							//if(!myCompositePSCReport.Print_AllowableStresses_Conc_ConstructionStage_LSD12_Comp(iDgnCode, ElemK, j, bFinal, TRUE, dgnFysg.FysgBase[j]))	{}
                            _T_RPT_TABLE<PSC_FYSG_BASE> rptD;
                            rptD.iIJ = j;
                            rptD.bGirder = TRUE;
                            rptD.RptData = dgnFysg.FysgBase[j];
                            aTRptSC.Add(rptD);
						}

					}

				}
			}

			// ************************************************
			// stress for cross section at service loads
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[1];
			if(bReport)
			{
				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

				if(bCompositeElem) // PSC Composite
				{
					T_FYCM_PSC_COMP FycmD;
					PSC_FYCM_D dgnFycm;
					if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_PSC_Comp(ElemK, FycmD))
					{
						// 거더
						if(FycmD.FycmGrBase[0].bCHK || FycmD.FycmGrBase[1].bCHK || FycmD.FycmGrBase[2].bCHK || FycmD.FycmGrBase[3].bCHK)
						{
							dgnFycm.Initialize();
							m_DataCtrl.m_pPscDataCtrl->Get_FycmCompReportData(ElemK, FycmD, dgnFycm, TRUE);
							for(j=0; j<4; ++j)
							{
								if(!dgnFycm.FycmBase[j].bCHK) continue;
								// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
								//bFinal = TRUE;
								//if(!myCompositePSCReport.Print_AllowableStresses_Conc_ServiceLoad_LSD12_Comp(iDgnCode, ElemK, j, bFinal, TRUE, dgnFycm.FycmBase[j]))  {}
                                _T_RPT_TABLE<PSC_FYCM_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.bGirder = TRUE;
                                rptD.RptData = dgnFycm.FycmBase[j];
                                aTRptSS.Add(rptD);
							}
						}

						// 슬래브
						if(FycmD.FycmSlBase[0].bCHK || FycmD.FycmSlBase[1].bCHK || FycmD.FycmSlBase[2].bCHK || FycmD.FycmSlBase[3].bCHK)
						{
							dgnFycm.Initialize();
							m_DataCtrl.m_pPscDataCtrl->Get_FycmCompReportData(ElemK, FycmD, dgnFycm, FALSE);
							for(j=0; j<4; ++j)
							{
								if(!dgnFycm.FycmBase[j].bCHK) continue;
								// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
								//bFinal = TRUE;
								//if(!myCompositePSCReport.Print_AllowableStresses_Conc_ServiceLoad_LSD12_Comp(iDgnCode, ElemK, j, bFinal, FALSE, dgnFycm.FycmBase[j]))  {}
                                _T_RPT_TABLE<PSC_FYCM_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.bGirder = FALSE;
                                rptD.RptData = dgnFycm.FycmBase[j];
                                aTRptSS.Add(rptD);
							}
						}
					}
				}
				else // PSC
				{
					T_FYCM_PSC FycmD;
					PSC_FYCM_D dgnFycm;
					if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_PSC(ElemK, FycmD))
					{
						// 거더
						if(FycmD.FycmBase[0].bCHK || FycmD.FycmBase[1].bCHK || FycmD.FycmBase[2].bCHK || FycmD.FycmBase[3].bCHK)
						{
							dgnFycm.Initialize();
							m_DataCtrl.m_pPscDataCtrl->Get_FycmReportData(ElemK, FycmD, dgnFycm);
							for(j=0; j<4; ++j)
							{
								if(!dgnFycm.FycmBase[j].bCHK) continue;
								// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
								//bFinal = TRUE;
								//if(!myCompositePSCReport.Print_AllowableStresses_Conc_ServiceLoad_LSD12_Comp(iDgnCode, ElemK, j, bFinal, TRUE, dgnFycm.FycmBase[j]))  {}
                                _T_RPT_TABLE<PSC_FYCM_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.bGirder = TRUE;
                                rptD.RptData = dgnFycm.FycmBase[j];
                                aTRptSS.Add(rptD);
							}
						}
					}

				}
			}

			// ************************************************
			// principal stress at a construction stage
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[2];
			if(bReport)
			{
				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

				if(bCompositeElem) // PSC Composite
				{
					T_FPSG_PSC_COMP FpsgD;
					if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpsg_PSC_Comp(ElemK, FpsgD))
					{
						PSC_FPSG_D  dgnFpsg;
						// 거더
						if(FpsgD.FpsgGrBase[0].bCHK || FpsgD.FpsgGrBase[1].bCHK || FpsgD.FpsgGrBase[2].bCHK || FpsgD.FpsgGrBase[3].bCHK)
						{
							dgnFpsg.Initialize();
							m_DataCtrl.m_pPscDataCtrl->Get_FpsgCompReportData(ElemK, FpsgD, dgnFpsg, TRUE);
							for(j=0; j<4; ++j)
							{
								if(!dgnFpsg.FpsgBase[j].bCHK) continue;
								// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
								//bFinal = TRUE;
								//if(!myCompositePSCReport.Print_AllowablePrincipalStress_Conc_ConstructionStage_LSD12_Comp(iDgnCode, ElemK, j, bFinal, TRUE, dgnFpsg.FpsgBase[j])) {}
                                _T_RPT_TABLE<PSC_FPSG_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.bGirder = TRUE;
                                rptD.RptData = dgnFpsg.FpsgBase[j];
                                aTRptPC.Add(rptD);
							}
						}
						// 슬래브
						if(FpsgD.FpsgSlBase[0].bCHK || FpsgD.FpsgSlBase[1].bCHK || FpsgD.FpsgSlBase[2].bCHK || FpsgD.FpsgSlBase[3].bCHK)
						{
							dgnFpsg.Initialize();
							m_DataCtrl.m_pPscDataCtrl->Get_FpsgCompReportData(ElemK, FpsgD, dgnFpsg, FALSE);
							for(j=0; j<4; ++j)
							{
								if(!dgnFpsg.FpsgBase[j].bCHK) continue;
								// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
								//bFinal = TRUE;
								//if(!myCompositePSCReport.Print_AllowablePrincipalStress_Conc_ConstructionStage_LSD12_Comp(iDgnCode, ElemK, j, bFinal, FALSE, dgnFpsg.FpsgBase[j])) {}
                                _T_RPT_TABLE<PSC_FPSG_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.bGirder = FALSE;
                                rptD.RptData = dgnFpsg.FpsgBase[j];
                                aTRptPC.Add(rptD);
							}
						}
					}
				}
				else // PSC
				{
					T_FPSG_PSC FpsgD;
					PSC_FPSG_D  dgnFpsg;
					if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpsg_PSC(ElemK, FpsgD))
					{						// 거더
						if(FpsgD.FpsgBase[0].bCHK || FpsgD.FpsgBase[1].bCHK || FpsgD.FpsgBase[2].bCHK || FpsgD.FpsgBase[3].bCHK)
						{
							dgnFpsg.Initialize();
							m_DataCtrl.m_pPscDataCtrl->Get_FpsgReportData(ElemK, FpsgD, dgnFpsg);
							for(j=0; j<4; ++j)
							{
								if(!dgnFpsg.FpsgBase[j].bCHK) continue;
								// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
								//bFinal = TRUE;
								//if(!myCompositePSCReport.Print_AllowablePrincipalStress_Conc_ConstructionStage_LSD12_Comp(iDgnCode, ElemK, j, bFinal, TRUE, dgnFpsg.FpsgBase[j])) {}
                                _T_RPT_TABLE<PSC_FPSG_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.bGirder = TRUE;
                                rptD.RptData = dgnFpsg.FpsgBase[j];
                                aTRptPC.Add(rptD);
							}
						}
					}
				}	// PSC  
			}

			// ************************************************
			// principal stress at service loads
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[3];
			if(bReport)
			{
				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

				if(bCompositeElem) // PSC Composite
				{
					T_FPMS_PSC_COMP FpmsD;
					if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpms_PSC_Comp(ElemK, FpmsD))
					{
						PSC_FPMS_D  dgnFpms;
						// 거더
						if(FpmsD.FpmsGrBase[0].bCHK || FpmsD.FpmsGrBase[1].bCHK || FpmsD.FpmsGrBase[2].bCHK || FpmsD.FpmsGrBase[3].bCHK)
						{
							dgnFpms.Initialize();          
							m_DataCtrl.m_pPscDataCtrl->Get_FpmsCompReportData(ElemK, FpmsD, dgnFpms, TRUE);

							for(j=0; j<4; ++j)
							{
								if(!dgnFpms.FpmsBase[j].bCHK) continue;
								// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
								/*if(!dgnFpms.FpmsBase[j].bCHK) continue; // 설계결과가 없을 경우 SKIP함.*/
								//bFinal = TRUE;
								//if(!myCompositePSCReport.Print_AllowablePrincipalStress_Conc_ServiceLoad_LSD12_Comp(iDgnCode, ElemK, j, bFinal, TRUE, dgnFpms.FpmsBase[j])) {}
                                _T_RPT_TABLE<PSC_FPMS_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.bGirder = TRUE;
                                rptD.RptData = dgnFpms.FpmsBase[j];
                                aTRptPS.Add(rptD);
							}
						}

						// 슬래브
						if(FpmsD.FpmsSlBase[0].bCHK || FpmsD.FpmsSlBase[1].bCHK || FpmsD.FpmsSlBase[2].bCHK || FpmsD.FpmsSlBase[3].bCHK)
						{
							dgnFpms.Initialize();          
							m_DataCtrl.m_pPscDataCtrl->Get_FpmsCompReportData(ElemK, FpmsD, dgnFpms, FALSE);

							for(j=0; j<4; ++j)
							{
								if(!dgnFpms.FpmsBase[j].bCHK) continue;
								// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
								/*if(!dgnFpms.FpmsBase[j].bCHK) continue; // 설계결과가 없을 경우 SKIP함.*/
								//bFinal = TRUE;
								//if(!myCompositePSCReport.Print_AllowablePrincipalStress_Conc_ServiceLoad_LSD12_Comp(iDgnCode, ElemK, j, bFinal, FALSE, dgnFpms.FpmsBase[j])) {}
                                _T_RPT_TABLE<PSC_FPMS_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.bGirder = FALSE;
                                rptD.RptData = dgnFpms.FpmsBase[j];
                                aTRptPS.Add(rptD);
							}
						} 
					}
				}
				else // PSC
				{
					T_FPMS_PSC FpmsD;
					if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpms_PSC(ElemK, FpmsD))
					{
						PSC_FPMS_D  dgnFpms;
						// 거더
						if(FpmsD.FpmsBase[0].bCHK || FpmsD.FpmsBase[1].bCHK || FpmsD.FpmsBase[2].bCHK || FpmsD.FpmsBase[3].bCHK)
						{
							dgnFpms.Initialize();          
							m_DataCtrl.m_pPscDataCtrl->Get_FpmsReportData(ElemK, FpmsD, dgnFpms);

							for(j=0; j<4; ++j)
							{
								if(!dgnFpms.FpmsBase[j].bCHK) continue;
								// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
								//bFinal = TRUE;
								//if(!myCompositePSCReport.Print_AllowablePrincipalStress_Conc_ServiceLoad_LSD12_Comp(iDgnCode, ElemK, j, bFinal, TRUE, dgnFpms.FpmsBase[j])) {}
                                _T_RPT_TABLE<PSC_FPMS_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.bGirder = TRUE;
                                rptD.RptData = dgnFpms.FpmsBase[j];
                                aTRptPS.Add(rptD);
							}
						}
					}
				}
			}

			// ************************************************
			// Crack Control
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[5];
			if(bReport)
			{
				T_BDCW_PSC_COMP BdcwDComp; BdcwDComp.Initialize();
				T_BDCW_PSC      BdcwD;     BdcwD.Initialize();

				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);
				BOOL bExistBdcwResD = FALSE;
				if(bCompositeElem)  
				{
					bExistBdcwResD = m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_PSC_Comp(ElemK, BdcwDComp);
				}
				else                
				{
					bExistBdcwResD = m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_PSC(ElemK, BdcwD);
				}

				if(bExistBdcwResD)
				{
					PSC_BDCW_BASE dgnbdcw;  			
					for(j=0; j<4; ++j)
					{
						BOOL bcheck = TRUE;
						if(bCompositeElem)  bcheck = BdcwDComp.BdcwBase[j].bCHK;
						else                bcheck = BdcwD.BdcwBase[j].bCHK;
						if(!bcheck) continue;
						//
						InData.Initialize();
						dgnbdcw.Initialize();

						iPosiNo = 0;  iPosiNo = (j<2)? 0 : 1;
						iMaxMin = 0;  iMaxMin = j%2;
						if(bCompositeElem)  
						{
							m_DataCtrl.m_pPscDataCtrl->Get_BdcwCompReportData(ElemK, iPosiNo, iMaxMin, BdcwDComp.BdcwBase[j], InData, dgnbdcw);
						}
						else
						{
							m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData(ElemK, iPosiNo, iMaxMin, BdcwD.BdcwBase[j], InData, dgnbdcw);            
						}

						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						bFinal = TRUE;        
						// Crack은 Moment Check와 검토조건을 동일하게 한다. (Agree with JOAHN)
						if(!bCheckMomI && !bCheckMomJ)  continue;
						else
						{
							if((j<2 && bCheckMomI && dgnbdcw.bCHK) || (j>1 && bCheckMomJ && dgnbdcw.bCHK))
							{
								//if(!myCompositePSCReport.Print_CrackCheck_LSD12_Comp(iDgnCode, ElemK, j, bFinal, dgnbdcw))  {}
                                _T_RPT_TABLE<PSC_BDCW_BASE> rptD;
                                rptD.iIJ = j;
                                rptD.RptData = dgnbdcw;
                                aTRptCC.Add(rptD);
							}
						}
					}
				}

			}

			if(m_bStopExecute)	return FALSE;
			//nProgressPercent += nProgressIncrementPercent;
			//Progress(0, nProgressPercent);
			//Progress(2, nProgressPercent);
		}
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);

        INT_PTR nTBR = aTRptBR.GetSize();
        for (int i = 0; i<nTBR; ++i)
        {
            BOOL bFinal = (i==(nTBR-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FRCR_D> TRptD = aTRptBR.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myCompositePSCReport.Print_FlexureDesign_LSD12_Comp(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTSR = aTRptSR.GetSize();
        for (int i = 0; i<nTSR; ++i)
        {
            BOOL bFinal = (i==(nTSR-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_CUMS_BASE> TRptD = aTRptSR.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myCompositePSCReport.Print_ShearDesign_LSD12_Comp(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTTR = aTRptTR.GetSize();
        for (int i = 0; i<nTTR; ++i)
        {
            BOOL bFinal = (i==(nTTR-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_CRMT_BASE> TRptD = aTRptTR.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myCompositePSCReport.Print_TorsionDesign_LSD12_Comp(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTSC = aTRptSC.GetSize();
        for (int i = 0; i<nTSC; ++i)
        {
            BOOL bFinal = (i==(nTSC-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FYSG_BASE> TRptD = aTRptSC.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myCompositePSCReport.Print_AllowableStresses_Conc_ConstructionStage_LSD12_Comp(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.bGirder, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTSS = aTRptSS.GetSize();
        for (int i = 0; i<nTSS; ++i)
        {
            BOOL bFinal = (i==(nTSS-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FYCM_BASE> TRptD = aTRptSS.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myCompositePSCReport.Print_AllowableStresses_Conc_ServiceLoad_LSD12_Comp(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.bGirder, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTPC = aTRptPC.GetSize();
        for (int i = 0; i<nTPC; ++i)
        {
            BOOL bFinal = (i==(nTPC-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FPSG_BASE> TRptD = aTRptPC.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myCompositePSCReport.Print_AllowablePrincipalStress_Conc_ConstructionStage_LSD12_Comp(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.bGirder, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTPS = aTRptPS.GetSize();
        for (int i = 0; i<nTPS; ++i)
        {
            BOOL bFinal = (i==(nTPS-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FPMS_BASE> TRptD = aTRptPS.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myCompositePSCReport.Print_AllowablePrincipalStress_Conc_ServiceLoad_LSD12_Comp(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.bGirder, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTCC = aTRptCC.GetSize();
        for (int i = 0; i<nTCC; ++i)
        {
            BOOL bFinal = (i==(nTCC-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_BDCW_BASE> TRptD = aTRptCC.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myCompositePSCReport.Print_CrackCheck_LSD12_Comp(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;
	}
	catch(...)
	{
		bSuccess = FALSE;	
	}

	VERIFY(myCompositePSCReport.Terminate(iDgnCode, !bSuccess));

	if(m_bStopExecute)	return FALSE;
	nProgressPercent = 100;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	return bSuccess;
}

BOOL CCRCExcelOutput::Execute_PSCReport_SNiP20503_84(int iDgnCode, CString sSaveFileName)
{
	// SET PROGRESS-BAR
	if(m_bStopExecute) return FALSE;
	Progress(0, 0);
	int nProgressPercent = 0;

	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	//
	BOOL bSuccess = TRUE;

	if(!m_PscdD.bStrePrint4AASHTO[0] && !m_PscdD.bStrePrint4AASHTO[1] && !m_PscdD.bStrePrint4AASHTO[2] &&
		 !m_PscdD.bStrePrint4AASHTO[3] && 
		 !m_PscdD.bServPrint4AASHTO[0] && !m_PscdD.bServPrint4AASHTO[1] && !m_PscdD.bServPrint4AASHTO[2] && 
		 !m_PscdD.bServPrint4AASHTO[3] && !m_PscdD.bServPrint4AASHTO[4] && !m_PscdD.bServPrint4AASHTO[5])
	{
		// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		return FALSE;
	}

	// Get all element keys for which results are to be printed
	CArray<T_POSX_K, T_POSX_K> arPrintElemKey;
	m_pDoc->m_pAttrCtrl->GetPosxKeyList(arPrintElemKey);

	CArray<T_POSV_K, T_POSV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl->GetPosvKeyList(aPrintVBeamKey);

	ArrElemPairKey aPrintEPairKey;
	CDBLib::ConvertToElemPairKeyList(arPrintElemKey, aPrintVBeamKey, aPrintEPairKey);

	int i=0, j=0, k=0;
	int kk=0;
	int nIJ=0, nMaxMin=0;
	BOOL bPrintI = FALSE, bPrintJ = FALSE;
	int nPrintElemSize = aPrintEPairKey.GetSize();  // Element Key..

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	if(nPrintElemSize==0)
	{
		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		return FALSE;
	}

	//
	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40/nPrintElemSize;

	// Detail Res-data.
	T_POSX_D PosxD; PosxD.Initialize();
	CArray<PSC_FRCR_D,PSC_FRCR_D> arFrcrD;  arFrcrD.RemoveAll();  arFrcrD.SetSize(4);
	PSC_CUMS_SNIP_D EngCumsD;
	PSC_BDCW_SNIP_D EngBdcwD;
	PSC_FATI_SNIP_D EngFatiD;
	//
	PSC_DATA_POSI     InData;
	PSC_DRES_RPT_SNIP RptData;
	PSC_TNDN_PTIF     TndnPropD;

	CArray<PSC_DATA_POSI, PSC_DATA_POSI&>         aInData;   aInData.RemoveAll();
	CArray<PSC_DRES_RPT_SNIP, PSC_DRES_RPT_SNIP&> aRptData;  aRptData.RemoveAll();
	CArray<PSC_FYSG_BASE, PSC_FYSG_BASE&>         aFysgSNiP; aFysgSNiP.RemoveAll();
	CArray<PSC_FYCM_BASE, PSC_FYCM_BASE&>         aFycmSNiP; aFycmSNiP.RemoveAll();
	CArray<PSC_FPSG_BASE, PSC_FPSG_BASE&>         aFpsgSNiP; aFpsgSNiP.RemoveAll();
	CArray<PSC_FPMS_BASE, PSC_FPMS_BASE&>         aFpmsSNiP; aFpmsSNiP.RemoveAll();
	CArray<PSC_FYPC_D, PSC_FYPC_D&>               aFypcSNiP; aFypcSNiP.RemoveAll();

	BOOL bCheckMomI_P = FALSE, bCheckMomJ_P = FALSE;
	BOOL bCheckMomI_N = FALSE, bCheckMomJ_N = FALSE;
	BOOL bCheckShrI   = FALSE, bCheckShrJ   = FALSE;
	BOOL bCheckTorI   = FALSE, bCheckTorJ   = FALSE;
	BOOL bCheckFatI   = FALSE, bCheckFatJ   = FALSE;
	BOOL bCheckCraI   = FALSE, bCheckCraJ   = FALSE;

	m_DataCtrl.m_pPscDataCtrl->InitialData(m_PscdD.iDgnCode);
	m_DataCtrl.m_pPscDataCtrl->Get_PscTndnPropData(TndnPropD);
	try
	{
		//DETATL-REPORT
		if(m_PscdD.bStrePrint4AASHTO[0] || m_PscdD.bStrePrint4AASHTO[1] || m_PscdD.bStrePrint4AASHTO[2] || m_PscdD.bStrePrint4AASHTO[3] || m_PscdD.bServPrint4AASHTO[5])
		{      
			for(i=0; i<nPrintElemSize; ++i)
			{
				auto  ElemK = aPrintEPairKey[i];

				PosxD.Initialize();
				if (ElemK.second == EN_EL_BEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD)) {};
				}
				else if (ElemK.second == EN_EL_VBEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD)) {}
				}
				else ASSERT(0);

				bCheckMomI_P = FALSE, bCheckMomJ_P = FALSE;
				bCheckMomI_N = FALSE, bCheckMomJ_N = FALSE;
				bCheckShrI   = FALSE, bCheckShrJ   = FALSE;
				bCheckTorI   = FALSE, bCheckTorJ   = FALSE;
				bCheckFatI   = FALSE, bCheckFatJ   = FALSE;
				bCheckCraI   = FALSE, bCheckCraJ   = FALSE;
				
				ShouldPrintPosxMomIJ(PosxD, bCheckMomI_P, bCheckMomJ_P);
				ShouldPrintPosxMomIJ_Negative(PosxD, bCheckMomI_N, bCheckMomJ_N);
				ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);
				ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);
				ShouldPrintPosxFatIJ(PosxD, bCheckFatI, bCheckFatJ);
				ShouldPrintPosxCraIJ(PosxD, bCheckCraI, bCheckCraJ);

				// SET DATA!
				T_BDCR_SNIP        BdcrD;
				PSC_FRCR_D         EngBdcrD;
				T_CUMS_SNIP        CumsD;
				PSC_CUMS_SNIP_BASE EngCumsD; 
				T_CRMT_SNIP        CrmtD;	   
				T_FATI_SNIP        FatiD;    
				PSC_FATI_SNIP_BASE EngFatiD; 
				T_BDCW_SNIP        BdcwD;    
				PSC_BDCW_SNIP_BASE EngBdcwD; 

				RptData.Initialize();				
				RptData.TndnPropD = TndnPropD;
				
				PSC_DATA_MEMB PscMembD;
				m_DataCtrl.m_pPscDataCtrl->Get_PscDataPosi(ElemK,0,PscMembD.Pos[0]);
				m_DataCtrl.m_pPscDataCtrl->Get_PscDataPosi(ElemK,1,PscMembD.Pos[1]);  

				PSC_RUS_MEMB_RES_D MembResD;
				m_DataCtrl.m_pPscDataCtrl->MakeMemberResDataSNiP(ElemK, PscMembD, MembResD);
				m_DataCtrl.m_pPscDataCtrl->SetMemberResDataSNiP(MembResD);
				RptData.MembD = MembResD.MembD;

				//GET DATA!
				for(j=0; j<2; j++) // I,J
				{
					// 다음의 경우를 제거할 수 있는지..확인할 것!!!!!!!
					// - Bending/Shear/Torsion을 출력하지 않고, Crack을 출력하는데, 
					// PrintOpt이 I/J 중 제거된 부분이 있다면..Detail을 출력하지 않도록 Sheet를 만들지 않아야 함.
					if(j==0 && !bCheckMomI_P && !bCheckMomI_N && !bCheckShrI && !bCheckTorI && !bCheckFatI && !bCheckCraI) continue;
					if(j==1 && !bCheckMomJ_P && !bCheckMomJ_N && !bCheckShrJ && !bCheckTorJ && !bCheckFatJ && !bCheckCraJ) continue;

					// 요소별 Position정보에 따라서 Sheet를 구분함. 즉 1_I / 1_J / 2_I / 2_J 등으로 Sheet 명칭 결정.
					// Dgn Condition/Parameters
					InData.Initialize();
					EngBdcrD.Initialize();
					if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_SNiP(ElemK, BdcrD))
					{
						// if j=0, index is 0 / j=1, index is 2!!
						// if j=0, index is 0 / j=1, index is 0!! k는 Max/Min 변수이므로, 항상 Posi별 Max정보로 가정함. InData를 가져오기 위함이니..
						m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData_SNiP(ElemK, j, 0, BdcrD.BdcrBase[j*2], InData, EngBdcrD);
					}
					RptData.DgnCondi = EngBdcrD;
	
					// CHK Bending Resistance!
					if(m_PscdD.bStrePrint4AASHTO[0])
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_SNiP(ElemK, BdcrD))
						{
							// Positive나 Negative가 하나라도 있으면..진행함.
							if((j==0 && (bCheckMomI_P || bCheckMomI_N)) || (j==1 && (bCheckMomJ_P || bCheckMomJ_N)))
							{
								for(k=0; k<2; k++) // Positive/Negative
								{
									if(j==0 && k==0 && !bCheckMomI_P)  continue;
									if(j==0 && k==1 && !bCheckMomI_N)  continue;
									if(j==1 && k==0 && !bCheckMomJ_P)  continue;
									if(j==1 && k==1 && !bCheckMomJ_N)  continue;

									kk = j*2+k;
									if(!BdcrD.BdcrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

									InData.Initialize();
									EngBdcrD.Initialize();              
									m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData_SNiP(ElemK, j, k, BdcrD.BdcrBase[kk], InData, EngBdcrD);
									RptData.FlexD[k] = EngBdcrD;
								}
							}
						}
					}

					if(m_PscdD.bStrePrint4AASHTO[1])
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_SNiP(ElemK, CumsD))
						{
							if((j==0 && bCheckShrI) || (j==1 && bCheckShrJ))
							{
								if(CumsD.CumsBase[j].bCHK) 
								{
									InData.Initialize();
									EngCumsD.Initialize();
									m_DataCtrl.m_pPscDataCtrl->Get_CumsReportData_SNiP(ElemK, j, 0, CumsD.CumsBase[j], InData, EngCumsD);
									RptData.ShearD = EngCumsD;
								}
							}
						}
					}

					if(m_PscdD.bStrePrint4AASHTO[2])
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_SNiP(ElemK, CrmtD))
						{
							if((j==0 && bCheckTorI) || (j==1 && bCheckTorJ))
							{
								if(CrmtD.CrmtBase[j].bCHK)
								{
									m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_SNiP(ElemK, BdcrD);
									InData.Initialize();
									EngCumsD.Initialize();
									// MQC-15320, 비틀림 검토, 조합 검토 오류 관련, Mr 할당 안되는 오류 수정
									if (CrmtD.CrmtBase[j].dMu>=0.0)
									{
										//RptData.TorsD.dMr = BdcrD.BdcrBase[j*2].dMr;
										EngCumsD.dMr = BdcrD.BdcrBase[j*2].dMr;
									}
									else
									{
										//RptData.TorsD.dMr = BdcrD.BdcrBase[j*2+1].dMr;
										EngCumsD.dMr = BdcrD.BdcrBase[j*2+1].dMr;
									}
									m_DataCtrl.m_pPscDataCtrl->Get_CrmtReportData_SNiP(ElemK, j, 0, CrmtD.CrmtBase[j], InData, EngCumsD); 
									RptData.TorsD = EngCumsD;
								}
							}
						}
					}

					if(m_PscdD.bStrePrint4AASHTO[3])
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFati_SNiP(ElemK, FatiD))
						{
							if((j==0 && bCheckFatI) || (j==1 && bCheckFatJ))
							{
								if(FatiD.FatiBase[j].bCHK)
								{
									InData.Initialize();
									EngFatiD.Initialize();
									m_DataCtrl.m_pPscDataCtrl->Get_FatiReportData_SNiP(ElemK, j, 0, FatiD.FatiBase[j], InData, EngFatiD);
									RptData.FatiD = EngFatiD;
								}
							}
						}
					}

					if(m_PscdD.bServPrint4AASHTO[5])
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_SNiP(ElemK, BdcwD))
						{
							if((j==0 && bCheckCraI) || (j==1 && bCheckCraJ))
							{
								for(k=0; k<2; k++)
								{
									kk = j*2+k;
									if(!BdcwD.BdcwBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

									InData.Initialize();
									EngBdcwD.Initialize();
									m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData_SNiP(ElemK, j, k, BdcwD.BdcwBase[kk], InData, EngBdcwD);
									RptData.CrackD[k] = EngBdcwD;
								}
							}
						}
					}

					BOOL bRptPrint[6] = {bCheckMomI_P, bCheckMomI_N, bCheckShrI, bCheckTorI, bCheckFatI, bCheckCraI};
					if (j==1) 
					{
						bRptPrint[0]=bCheckMomJ_P, bRptPrint[1]=bCheckMomJ_N, bRptPrint[2]=bCheckShrJ, bRptPrint[3]=bCheckTorJ, bRptPrint[4]=bCheckFatJ, bRptPrint[5]=bCheckCraJ;
					}
					m_DataCtrl.m_pPscDataCtrl->Get_ReportBlock4SNiP(bRptPrint, PscMembD.Pos[j],  RptData); 

					PscMembD.Pos[j].iElemK    = ElemK.first;
					PscMembD.Pos[j].iPosition = j;

					aInData.Add(PscMembD.Pos[j]);
					aRptData.Add(RptData);
				}
				//
				if(m_bStopExecute)	return FALSE;
				nProgressPercent += nProgressIncrementPercent;
				Progress(0, nProgressPercent);
				Progress(2, nProgressPercent);
			}
		}
	
		//RESULT-TABLE
		nProgressIncrementPercent = nPrintElemSize == 0 ? 30 : 30/nPrintElemSize;
		int iPosiNo = 0;
		int iMaxMin = 0;
		for(i=0; i<nPrintElemSize; i++)
		{
			auto ElemK = aPrintEPairKey[i];

			//Detail Report의 변수는 Calc의 변수를 가져와야 함.
			//Result Table의 변수는 Civil내 저장된 변수를 가져와야 함.
			PosxD.Initialize();
			if (ElemK.second == EN_EL_BEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD)) {};
			}
			else if (ElemK.second == EN_EL_VBEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD)) {}
			}
			else ASSERT(0);

			// Stress for Cross Section at a Construction Stage
			if(m_PscdD.bServPrint4AASHTO[0])
			{
				T_FYSG_SNIP FysgD; FysgD.Initialize();
				PSC_FYSG_D  dgnFysg; dgnFysg.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_SNiP(ElemK, FysgD))	continue;
				//
				if(FysgD.FysgBase[0].bCHK || FysgD.FysgBase[1].bCHK || FysgD.FysgBase[2].bCHK || FysgD.FysgBase[3].bCHK)
				{
					dgnFysg.Initialize();
					m_DataCtrl.m_pPscDataCtrl->Get_FysgReportData_SNiP(ElemK, FysgD, dgnFysg);

					for(j=0; j<4; ++j)
					{
						if(!dgnFysg.FysgBase[j].bCHK) continue;

						if(j==0)      { dgnFysg.FysgBase[j].nPart = 0; dgnFysg.FysgBase[j].nCompTens = 0; }
						else if(j==1) { dgnFysg.FysgBase[j].nPart = 0; dgnFysg.FysgBase[j].nCompTens = 1; }
						else if(j==2) { dgnFysg.FysgBase[j].nPart = 1; dgnFysg.FysgBase[j].nCompTens = 0; }
						else if(j==3) { dgnFysg.FysgBase[j].nPart = 1; dgnFysg.FysgBase[j].nCompTens = 1; }

						aFysgSNiP.Add(dgnFysg.FysgBase[j]);
					}
				}
			}

			// Stress for Cross Section at Service Loads
			if(m_PscdD.bServPrint4AASHTO[1])
			{
				T_FYCM_SNIP FycmD; FycmD.Initialize();
				PSC_FYCM_D  dgnFycm; dgnFycm.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_SNiP(ElemK, FycmD)) continue;
				//
				if(FycmD.FycmBase[0].bCHK || FycmD.FycmBase[1].bCHK || FycmD.FycmBase[2].bCHK || FycmD.FycmBase[3].bCHK)
				{
					dgnFycm.Initialize();
					m_DataCtrl.m_pPscDataCtrl->Get_FycmReportData_SNiP(ElemK, FycmD, dgnFycm);

					for(j=0; j<4; ++j)
					{
						if(!dgnFycm.FycmBase[j].bCHK) continue;

						if(j==0)      { dgnFycm.FycmBase[j].nPart = 0; dgnFycm.FycmBase[j].nCompTens = 0; }
						else if(j==1) { dgnFycm.FycmBase[j].nPart = 0; dgnFycm.FycmBase[j].nCompTens = 1; }
						else if(j==2) { dgnFycm.FycmBase[j].nPart = 1; dgnFycm.FycmBase[j].nCompTens = 0; }
						else if(j==3) { dgnFycm.FycmBase[j].nPart = 1; dgnFycm.FycmBase[j].nCompTens = 1; }

						aFycmSNiP.Add(dgnFycm.FycmBase[j]);	
					}	
				}
			}

			// Principal Stress at a Construction Stage
			if(m_PscdD.bServPrint4AASHTO[2])
			{
				T_FPSG_SNIP FpsgD; FpsgD.Initialize();
				PSC_FPSG_D  dgnFpsg;  dgnFpsg.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpsg_SNiP(ElemK, FpsgD)) continue;

				if(FpsgD.FpsgBase[0].bCHK || FpsgD.FpsgBase[1].bCHK || FpsgD.FpsgBase[2].bCHK || FpsgD.FpsgBase[3].bCHK)
				{
					dgnFpsg.Initialize();
					m_DataCtrl.m_pPscDataCtrl->Get_FpsgReportData_SNiP(ElemK, FpsgD, dgnFpsg);

					for(j=0; j<4; ++j)
					{
						if(!dgnFpsg.FpsgBase[j].bCHK) continue;

						if(j==0)      { dgnFpsg.FpsgBase[j].nPart = 0; dgnFpsg.FpsgBase[j].nCompTens = 0; }
						else if(j==1) { dgnFpsg.FpsgBase[j].nPart = 0; dgnFpsg.FpsgBase[j].nCompTens = 1; }
						else if(j==2) { dgnFpsg.FpsgBase[j].nPart = 1; dgnFpsg.FpsgBase[j].nCompTens = 0; }
						else if(j==3) { dgnFpsg.FpsgBase[j].nPart = 1; dgnFpsg.FpsgBase[j].nCompTens = 1; }
						
						aFpsgSNiP.Add(dgnFpsg.FpsgBase[j]);	
					}
				}
			}

			// Principal Stress at Service Loads
			if(m_PscdD.bServPrint4AASHTO[3])
			{
				T_FPMS_SNIP FpmsD; FpmsD.Initialize();
				PSC_FPMS_D  dgnFpms;  dgnFpms.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpms_SNiP(ElemK, FpmsD)) continue;

				if(FpmsD.FpmsBase[0].bCHK || FpmsD.FpmsBase[1].bCHK || FpmsD.FpmsBase[2].bCHK || FpmsD.FpmsBase[3].bCHK)
				{
					dgnFpms.Initialize();          
					m_DataCtrl.m_pPscDataCtrl->Get_FpmsReportData_SNiP(ElemK, FpmsD, dgnFpms);

					for(j=0; j<4; ++j)
					{
						if(!dgnFpms.FpmsBase[j].bCHK) continue;

						if(j==0)      { dgnFpms.FpmsBase[j].nPart = 0; dgnFpms.FpmsBase[j].nCompTens = 0; }
						else if(j==1) { dgnFpms.FpmsBase[j].nPart = 0; dgnFpms.FpmsBase[j].nCompTens = 1; }
						else if(j==2) { dgnFpms.FpmsBase[j].nPart = 1; dgnFpms.FpmsBase[j].nCompTens = 0; }
						else if(j==3) { dgnFpms.FpmsBase[j].nPart = 1; dgnFpms.FpmsBase[j].nCompTens = 1; }
						
						aFpmsSNiP.Add(dgnFpms.FpmsBase[j]);	
					}
				}
			}

			if(m_bStopExecute)	return FALSE;
			nProgressPercent += nProgressIncrementPercent;
			Progress(0, nProgressPercent);
			Progress(2, nProgressPercent);
		}

		// Tensile Stress for Prestressing Steel
		if(m_PscdD.bServPrint4AASHTO[4])
		{
			CArray<T_TNDN_K,T_TNDN_K> arTndnList;  arTndnList.RemoveAll();
			m_pDoc->m_pPostCtrl->GetPCDesign()->GetPscCheckTdnaList(arTndnList);

			int iTndnKey  = 0;
			int iTndnList = 0;
			iTndnList = arTndnList.GetSize();

			for(i=0; i<iTndnList; ++i)
			{
				iTndnKey  = 0;
				iTndnKey  = arTndnList.GetAt(i);

				T_FYPC_SNIP FypcD;    FypcD.Initialize();
				PSC_FYPC_D  dgnFypcD; dgnFypcD.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFypc_SNiP(iTndnKey, FypcD)) continue;
				m_DataCtrl.m_pPscDataCtrl->Get_FypcReportData_SNiP(iTndnKey, FypcD, dgnFypcD);
				aFypcSNiP.Add(dgnFypcD);
			}
			//
			if(m_bStopExecute)	return FALSE;
			nProgressPercent = 90;
			Progress(0, nProgressPercent);
			Progress(2, nProgressPercent);
		}

		CDgn_PSCRptManager_SNiP20503_84  mySNiPPscReport;

#if defined(_CIVIL_RUS)
		CString strBasePath = _T("");
		if     (iDgnCode==SNiP_20503_84_PSC     || iDgnCode==SP_35_13330_11_PSC)    strBasePath = m_pDoc->GetProgramPath() + _T("\\Excel Base File\\RUS\\");
		else if(iDgnCode==SNiP_20503_84_PSC_MKS || iDgnCode==SP_35_13330_11_PSC_MKS)strBasePath = m_pDoc->GetProgramPath() + _T("\\Excel Base File\\RUS(MKS)\\");
		else ASSERT(0);
#else 
		CString strBasePath = _T("");
		if     (iDgnCode==SNiP_20503_84_PSC     || iDgnCode==SP_35_13330_11_PSC)    strBasePath = m_pDoc->GetProgramPath() + _T("\\Excel Base File\\US\\");
		else if(iDgnCode==SNiP_20503_84_PSC_MKS || iDgnCode==SP_35_13330_11_PSC_MKS)strBasePath = m_pDoc->GetProgramPath() + _T("\\Excel Base File\\US(MKS)\\");
		else ASSERT(0);
#endif

		mySNiPPscReport.Print_DetailReport(iDgnCode, strBasePath, GetSaveFileDirEx(), sSaveFileName, 
																			 aInData, aRptData, aFysgSNiP, aFycmSNiP, aFpsgSNiP, aFpmsSNiP, aFypcSNiP);
	}
	catch(...)
	{
		bSuccess = FALSE;	
	}

	//VERIFY(mySNiPPscReport.Terminate(iDgnCode, !bSuccess));

	if(m_bStopExecute)	return FALSE;
	nProgressPercent = 100;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	return bSuccess;
}

BOOL CCRCExcelOutput::Execute_PSCReport_AS_5100_5(int nDgnCode, int nAmd, CString sSaveFileName, int iRptUnit)
{
	auto lambda_SetProgress = [this] (const BOOL& bStop, int& nProgressPercent, const int& nIncrementPercent)
	{
		if(bStop) return FALSE;
		nProgressPercent += nIncrementPercent;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);
		return TRUE;
	};
	// SET PROGRESS-BAR
	if(m_bStopExecute) return FALSE;
	Progress(0, 0);
	int nProgressPercent = 0;

	if(!lambda_SetProgress(m_bStopExecute, nProgressPercent, 5)) return FALSE;

	CDgn_PSCRptManager_AS5100  myAS5100PscReport;
	if(!myAS5100PscReport.Initialize(nDgnCode, nAmd, iRptUnit, m_pDoc->GetProgramPath(), GetSaveFileDirEx(), sSaveFileName)) return FALSE;

	if(!lambda_SetProgress(m_bStopExecute, nProgressPercent, 5)) return FALSE;

	BOOL bSuccess = TRUE;

	if(!m_PscdD.bStrePrint4AASHTO[0] && !m_PscdD.bStrePrint4AASHTO[1] && 
		 !m_PscdD.bStrePrint4AASHTO[2] && !m_PscdD.bStrePrint4AASHTO[3] &&
		!m_PscdD.bServPrint4AASHTO[0])
	{
		// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
		VERIFY(myAS5100PscReport.Terminate(nDgnCode, !bSuccess, TRUE));
		if(!lambda_SetProgress(m_bStopExecute, nProgressPercent, 0)) return FALSE;
		return FALSE;
	}
	// Get all element keys for which results are to be printed
	CArray<T_POSX_K, T_POSX_K> arPrintElemKey;
	m_pDoc->m_pAttrCtrl->GetPosxKeyList(arPrintElemKey);

	CArray<T_POSV_K, T_POSV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl->GetPosvKeyList(aPrintVBeamKey);

	ArrElemPairKey aPrintEPairKey;
	CDBLib::ConvertToElemPairKeyList(arPrintElemKey, aPrintVBeamKey, aPrintEPairKey);

	int nPrintElemSize = aPrintEPairKey.GetSize();  // Element Key..
	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	if(nPrintElemSize==0)
	{
		VERIFY(myAS5100PscReport.Terminate(nDgnCode, !bSuccess, TRUE));
		if(!lambda_SetProgress(m_bStopExecute, nProgressPercent, 90)) return FALSE;
		return FALSE;
	}
	//
	int nProgressIncrementPercent = nPrintElemSize == 0 ? 90 : 90/nPrintElemSize;

	BOOL bCheckMomI = FALSE,          bCheckMomJ = FALSE;
	BOOL bCheckMomNegI = FALSE,       bCheckMomNegJ = FALSE;
	BOOL bCheckShrI = FALSE,          bCheckShrJ = FALSE;
	BOOL bCheckTorI = FALSE,          bCheckTorJ = FALSE;

	CPCDesign* pPCDesign = m_pDoc->m_pPostCtrl->GetPCDesign();

    m_DataCtrl.CalcTendonfpsLpPosition(); // Transmission Lp, Lpt 위치의 fps 계산을 위해..

	try
	{
		//DETATL-REPORT
		if(m_PscdD.bStrePrint4AASHTO[0] || m_PscdD.bStrePrint4AASHTO[1] || m_PscdD.bStrePrint4AASHTO[2] || m_PscdD.bStrePrint4AASHTO[3] ||
			m_PscdD.bServPrint4AASHTO[0])
		{
			for(int i = 0; i < nPrintElemSize; ++i)
			{
				//
				auto ElemKey = aPrintEPairKey[i];
				//
				T_POSC_D PoscD; PoscD.Initialize();
				T_POSX_D PosxD; PosxD.Initialize();
				if (ElemKey.second == EN_EL_BEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPosc(ElemKey.first, PoscD)) continue;
					if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemKey.first, PosxD)) continue;
				}
				else if (ElemKey.second == EN_EL_VBEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPovc(ElemKey.first, PoscD)) continue;
					if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemKey.first, PosxD)) continue;
				}
				else ASSERT(0);

				bCheckMomI = FALSE, bCheckMomJ = FALSE;
				ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);

				bCheckMomNegI = FALSE, bCheckMomNegJ = FALSE;
				ShouldPrintPosxMomIJ_Negative(PosxD, bCheckMomNegI, bCheckMomNegJ);

				bCheckShrI = FALSE,	bCheckShrJ = FALSE;
				ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);

				bCheckTorI = FALSE,	bCheckTorJ = FALSE;
				ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

				// SET DATA!
                T_TRAN_PSC_AS TranStrD;
                _TRAN_PSC_BASE EngTranD;
                if ( !pPCDesign->ReadTran_AS(ElemKey, TranStrD) ) continue;

                T_BDCW_PSC_AS TranCrD;
                _BDCW_PSC_LRFD_BASE EngTranCrD;
                if ( !pPCDesign->ReadTrcr_AS(ElemKey, TranCrD) ) continue;

				T_BDCR_PSC_AS       BdcrD;
				_BDCY_PSC_LRFD_BASE EngBdcrD; EngBdcrD.Initialize();
				if(!pPCDesign->ReadBdcr_AS(ElemKey, BdcrD)) continue;
																			
				T_SHRR_PSC_AS       ShrrD;	  
				_CUMS_AASHTO_BASE   EngCumsD; EngCumsD.Initialize();  
				if(!pPCDesign->ReadShrr_AS(ElemKey, ShrrD)) continue;

				T_TORR_PSC_AS       TorrD;	  
				_STCM_AASHTO_BASE   EngCrmtD; EngCrmtD.Initialize();  
				if(!pPCDesign->ReadTorr_AS(ElemKey, TorrD)) continue;

				T_BDCW_PSC_AS       BdcwD;    
				_BDCW_PSC_LRFD_BASE EngBdcwD; EngBdcwD.Initialize();  
				if(!pPCDesign->ReadBdcw_AS(ElemKey, BdcwD)) continue;

				// 0. PSC_DATA for Print
				// -출력 단위에 따른  PSC_DATA_POSI 생성
				CArray<PSC_DATA_POSI, PSC_DATA_POSI&> arInData;  arInData.RemoveAll(); arInData.SetSize(4);
				PSC_DATA_POSI InData;

				DGN_JOINT_D JointD;
				m_DataCtrl.Get_JointData4Element(ElemKey, JointD);
								
				for(int j=0; j<4; j++) 
				{
					InData.Initialize();
					int nNode = (j<2)? 0 : 1;    // I,J
					//joint
					InData.iJoint = nNode==0 ? JointD.iI_joint : JointD.iJ_joint;
					// PrintOpt의 I/J 가 전부 제거되었을 때 Detail을 출력하지 않도록
					if(nNode==0 && !bCheckMomI && !bCheckMomNegI && !bCheckShrI && !bCheckTorI) continue;
					if(nNode==1 && !bCheckMomJ && !bCheckMomNegJ && !bCheckShrJ && !bCheckTorJ) continue;
					int nPosNeg = (j==0 || j==2)? 0 : 1;  // 0:pos  1:neg 
					m_DataCtrl.m_pPscDataCtrl->Get_DesignConditionData(FALSE, ElemKey, nNode, nPosNeg, InData);
					arInData.SetAt(j, InData);   
				}

				// 1. Design Condition
				for(int j=0; j<4; j++) 
				{
					InData.Initialize();
					EngBdcrD.Initialize();

					int nNode = (j<2)? 0 : 1;    // I,J
					// PrintOpt의 I/J 가 전부 제거되었을 때 Detail을 출력하지 않도록
					if(nNode==0 && !bCheckMomI && !bCheckMomNegI && !bCheckShrI && !bCheckTorI) continue;
					if(nNode==1 && !bCheckMomJ && !bCheckMomNegJ && !bCheckShrJ && !bCheckTorJ) continue;

					int nPosNeg = (j==0 || j==2)? 0 : 1;  // 0:pos  1:neg
					m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData_AS(ElemKey, nNode, nPosNeg, BdcrD.BdcrB[j], arInData.GetAt(j), EngBdcrD);
					if(j==0 || j==2)
					{
						myAS5100PscReport.Print_Detail_Condition(m_PscdD.iDgnCode, ElemKey, nNode, arInData.GetAt(j), EngBdcrD);
					}
				}
				if(!lambda_SetProgress(m_bStopExecute, nProgressPercent, nProgressIncrementPercent/4)) return FALSE;

                // 2. Transfer
                for ( int nIJ=0; nIJ<2; nIJ++ )
                {
                    // PrintOpt의 I/J 가 전부 제거되었을 때 Crack을 출력하지 않도록
                    if ( nIJ==0 && !bCheckMomI && !bCheckMomNegI && !bCheckShrI && !bCheckTorI ) continue;
                    if ( nIJ==1 && !bCheckMomJ && !bCheckMomNegJ && !bCheckShrJ && !bCheckTorJ ) continue;
                    int nStart=0;
                    int nLast=0;
                    if ( nIJ==0 ) { nStart=0; nLast=2; }
                    else { nStart=2; nLast=4; }
                    BOOL bFirst_crack = TRUE;
                    for ( int j=nStart; j<nLast; j++ )
                    {
                        int nTopBot = (j==0 || j==2)? 0 : 1;  // 0:top  1:bottom
                        EngTranD.Initialize();
                        if ( m_PscdD.bStrePrint4AASHTO[3] && TranStrD.GrB[j].bCHK )
                        {
                            m_DataCtrl.m_pPscDataCtrl->Get_TranReportData_AS(ElemKey, nIJ, nTopBot, TranStrD.GrB[j], arInData.GetAt(j), EngTranD);
                            myAS5100PscReport.Print_Detail_TransferStress(m_PscdD.iDgnCode, ElemKey, j, bFirst_crack, arInData.GetAt(j), EngTranD);
                            bFirst_crack = FALSE;
                        }
                    }

                    bFirst_crack = TRUE;
                    for ( int j=nStart; j<nLast; j++ )
                    {
                        int nPosNeg = (j==0 || j==2)? 0 : 1;  // 0:pos  1:neg
                        EngTranCrD.Initialize();
                        if ( m_PscdD.bStrePrint4AASHTO[3] && TranCrD.BdcwB[j].bCHK )
                        {
                            EngTranCrD.LInf.bTransfer = true;
                            m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData_AS(ElemKey, nIJ, nPosNeg, TranCrD.BdcwB[j], arInData.GetAt(j), EngTranCrD);
                            myAS5100PscReport.Print_Detail_CrackCheck(m_PscdD.iDgnCode, ElemKey, j, bFirst_crack, arInData.GetAt(j), EngTranCrD);
                            bFirst_crack = FALSE;
                        }
                    }
                }

				// 2. Flexure Design
				for(int nIJ=0; nIJ<2; nIJ++)
				{
					int nStart=0;
					int nLast=0;
					if(nIJ==0) { nStart=0; nLast=2; }
					else       { nStart=2; nLast=4; }

					BOOL bFirst_flex = TRUE;
					for(int j=nStart; j<nLast; j++) 
					{
						if(m_PscdD.bStrePrint4AASHTO[0] && BdcrD.BdcrB[j].bCHK)
						{
							if(j==0 && !bCheckMomI)    continue;
							if(j==1 && !bCheckMomNegI) continue;
							if(j==2 && !bCheckMomJ)    continue;
							if(j==3 && !bCheckMomNegJ) continue;
							int nPosNeg = (j==0 || j==2)? 0 : 1;  // 0:pos  1:neg
							EngBdcrD.Initialize();
							m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData_AS(ElemKey, nIJ, nPosNeg, BdcrD.BdcrB[j], arInData.GetAt(j), EngBdcrD);
							myAS5100PscReport.Print_Detail_FlexureDesign(m_PscdD.iDgnCode, ElemKey, j, bFirst_flex, arInData.GetAt(j), EngBdcrD);      
							bFirst_flex = FALSE;
						}
					} 
				}
				if(!lambda_SetProgress(m_bStopExecute, nProgressPercent, nProgressIncrementPercent/4)) return FALSE;
				// 3. Shear
				for(int nIJ=0; nIJ<2; nIJ++)
				{
					if ( m_PscdD.bStrePrint4AASHTO[1] && ShrrD.ShrrB[nIJ].bCHK )
					{
						if(nIJ==0 && !bCheckShrI)   continue;
						if(nIJ==1 && !bCheckShrJ)   continue;
						int nAr = (nIJ==0)? 0 : 2;
						EngCumsD.Initialize();
						m_DataCtrl.m_pPscDataCtrl->Get_CumsReportData_AS(ElemKey, nIJ, 0, ShrrD.ShrrB[nIJ], arInData.GetAt(nAr), EngCumsD);
						myAS5100PscReport.Print_Detail_ShearDesign(m_PscdD.iDgnCode, ElemKey, nIJ, TRUE, arInData.GetAt(nAr), EngCumsD);  
					}
				}
				if(!lambda_SetProgress(m_bStopExecute, nProgressPercent, nProgressIncrementPercent/4)) return FALSE;
				// 4. Torsion
				for(int nIJ=0; nIJ<2; nIJ++)
				{
					if( m_PscdD.bStrePrint4AASHTO[2] && TorrD.TorrB[nIJ].bCHK )
					{
						if(nIJ==0 && !bCheckTorI)   continue;
						if(nIJ==1 && !bCheckTorJ)   continue;
						int nAr = (nIJ==0)? 0 : 2;
						EngCrmtD.Initialize();
						m_DataCtrl.m_pPscDataCtrl->Get_CrmtReportData_AS(ElemKey, nIJ, 0, TorrD.TorrB[nIJ], arInData.GetAt(nAr), EngCrmtD);
						myAS5100PscReport.Print_Detail_TorsionDesign(m_PscdD.iDgnCode, ElemKey, nIJ, TRUE, arInData.GetAt(nAr), EngCrmtD);
					}      
				}
				if(!lambda_SetProgress(m_bStopExecute, nProgressPercent, nProgressIncrementPercent/4)) return FALSE;
				// 5. Crack 일단 막음 Detail 은 개발하지 않음
				for(int nIJ=0; nIJ<2; nIJ++)
				{
					// PrintOpt의 I/J 가 전부 제거되었을 때 Crack을 출력하지 않도록
					if(nIJ==0 && !bCheckMomI && !bCheckMomNegI && !bCheckShrI && !bCheckTorI) continue;
					if(nIJ==1 && !bCheckMomJ && !bCheckMomNegJ && !bCheckShrJ && !bCheckTorJ) continue;
					int nStart=0;
					int nLast=0;
					if(nIJ==0) { nStart=0; nLast=2; }
					else       { nStart=2; nLast=4; }

					BOOL bFirst_crack = TRUE;
					for(int j=nStart; j<nLast; j++) 
					{
						int nPosNeg = (j==0 || j==2)? 0 : 1;  // 0:pos  1:neg
						EngBdcwD.Initialize();
						if(m_PscdD.bServPrint4AASHTO[0] && BdcwD.BdcwB[j].bCHK)
						{
							m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData_AS(ElemKey, nIJ, nPosNeg, BdcwD.BdcwB[j], arInData.GetAt(j), EngBdcwD);
							myAS5100PscReport.Print_Detail_CrackCheck(m_PscdD.iDgnCode, ElemKey, j, bFirst_crack, arInData.GetAt(j), EngBdcwD);
							bFirst_crack = FALSE;
						}      
					}
				}
			}
		}
	}
	catch(...)
	{
		bSuccess = FALSE;	
	}

	VERIFY(myAS5100PscReport.Terminate(m_PscdD.iDgnCode, !bSuccess, TRUE));

	if(m_bStopExecute)	return FALSE;
	nProgressPercent = 100;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	return TRUE;
	
}

BOOL CCRCExcelOutput::Execute_PSCReport_BS(int iDgnCode, CString sSaveFileName)
{
    // SET PROGRESS-BAR
    if ( m_bStopExecute ) return FALSE;
    Progress(0, 0);
    int nProgressPercent = 0;

    if ( m_bStopExecute ) return FALSE;
    nProgressPercent +=10;
    Progress(0, nProgressPercent);
    Progress(2, nProgressPercent);

    BOOL bSuccess = TRUE;

    if ( !m_PscdD.bUltimateEuro[0]       && !m_PscdD.bUltimateEuro[1]       && !m_PscdD.bUltimateEuro[2]       &&
        !m_PscdD.bServiceabilityEuro[0] && !m_PscdD.bServiceabilityEuro[1] && !m_PscdD.bServiceabilityEuro[2] &&
        !m_PscdD.bServiceabilityEuro[3] && !m_PscdD.bServiceabilityEuro[4] && !m_PscdD.bServiceabilityEuro[5] )
    {
        // Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!

        if ( m_bStopExecute ) return FALSE;
        nProgressPercent = 100;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);

        return FALSE;
    }

    // Get all element keys for which results are to be printed
    CArray<T_POSX_K, T_POSX_K> arPrintElemKey;
    m_pDoc->m_pAttrCtrl->GetPosxKeyList(arPrintElemKey);

	CArray<T_POSV_K, T_POSV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl->GetPosvKeyList(aPrintVBeamKey);

	ArrElemPairKey aPrintEPairKey;
	CDBLib::ConvertToElemPairKeyList(arPrintElemKey, aPrintVBeamKey, aPrintEPairKey);

    int i=0, j=0, k=0;
    int kk=0;
    int nIJ=0, nMaxMin=0;
    BOOL bPrintI = FALSE, bPrintJ = FALSE;
    int nPrintElemSize = aPrintEPairKey.GetSize();  // Element Key..

    // Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
    if ( nPrintElemSize==0 )
    {
        if ( m_bStopExecute ) return FALSE;
        nProgressPercent = 100;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);

        return FALSE;
    }

    int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40/nPrintElemSize;

    // Detail Res-data.
    CArray<PSC_FRCR_D, PSC_FRCR_D> arFrcrD;  arFrcrD.RemoveAll();  arFrcrD.SetSize(4);
    //
    PSC_DATA_POSI InData;   InData.Initialize();
    PSC_RPT_BS_POS RptResD; RptResD.Initialize();
    PSC_RPT_BS_D PsdRptD;
    BOOL bCheckMomI = FALSE, bCheckMomJ = FALSE;
    BOOL bCheckMomNegI = FALSE, bCheckMomNegJ = FALSE;
    BOOL bCheckShrI = FALSE, bCheckShrJ = FALSE;
    BOOL bCheckTorI = FALSE, bCheckTorJ = FALSE;

    PsdRptD.bFlexRpt    = m_PscdD.bUltimateEuro[0];
    PsdRptD.bShearRpt   = m_PscdD.bUltimateEuro[1];
    PsdRptD.bTorRpt     = m_PscdD.bUltimateEuro[2];
    PsdRptD.bFycmRpt    = m_PscdD.bServiceabilityEuro[0];
    PsdRptD.bFysgRpt    = m_PscdD.bServiceabilityEuro[1];
    PsdRptD.bFpmsRpt    = m_PscdD.bServiceabilityEuro[2];
    PsdRptD.bFpsgRpt    = m_PscdD.bServiceabilityEuro[3];
    PsdRptD.bFypcRpt    = m_PscdD.bServiceabilityEuro[4];

    try
    {
        //DETATL-REPORT
        if ( m_PscdD.bUltimateEuro[0] || m_PscdD.bUltimateEuro[1] || m_PscdD.bUltimateEuro[2] || 
			m_PscdD.bServiceabilityEuro[0] || m_PscdD.bServiceabilityEuro[1] || m_PscdD.bServiceabilityEuro[2] || m_PscdD.bServiceabilityEuro[3] )
        {
            for ( i=0; i<nPrintElemSize; ++i )
            {
                //
                auto ElemK = aPrintEPairKey[i];
                //

				T_POSC_D PoscD; PoscD.Initialize();
				T_POSX_D PosxD; PosxD.Initialize();
				if (ElemK.second == EN_EL_BEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPosc(ElemK.first, PoscD)) continue;
					if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD)) continue;
				}
				else if (ElemK.second == EN_EL_VBEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPovc(ElemK.first, PoscD)) continue;
					if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD)) continue;
				}
				else ASSERT(0);

                BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

                bCheckMomI = FALSE;
                bCheckMomJ = FALSE;
                ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);

                bCheckMomNegI = FALSE;
                bCheckMomNegJ = FALSE;
                ShouldPrintPosxMomIJ_Negative(PosxD, bCheckMomNegI, bCheckMomNegJ);

                bCheckShrI = FALSE;
                bCheckShrJ = FALSE;
                ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);

                bCheckTorI = FALSE;
                bCheckTorJ = FALSE;
                ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

                // SET DATA!

                T_BDCR_PSC_BS   BdcrD;
                PSC_FRCR_D      EngBdcrD; EngBdcrD.Initialize();
                if ( !m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_BS(ElemK, BdcrD) ) continue;

                T_SHRR_PSC_BS   ShrrD;
                PSC_CUMS_BASE   EngCumsD; EngCumsD.Initialize();
                if ( !m_pDoc->m_pPostCtrl->GetPCDesign()->ReadShrr_BS(ElemK, ShrrD) ) continue;

                T_TORR_PSC_BS   TorrD;
                PSC_CRMT_BASE   EngCrmtD; EngCrmtD.Initialize();
                if ( !m_pDoc->m_pPostCtrl->GetPCDesign()->ReadTorr_BS(ElemK, TorrD) ) continue;

                T_FYCM_PSC_BS   FycmD;
                PSC_FYCM_BASE   EngFycmD; EngFycmD.Initialize();
                if ( !m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_BS(ElemK, FycmD) ) continue;

                T_FYSG_PSC_BS   FysgD;
                PSC_FYSG_BASE   EngFysgD; EngFysgD.Initialize();
                if ( !m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_BS(ElemK, FysgD) ) continue;

                T_FPMS_PSC      FpmsD;
                PSC_FPMS_BASE   EngFpmsD; EngFysgD.Initialize();
                if ( !m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpms_PSC(ElemK, FpmsD) ) continue;

                T_FPSG_PSC      FpsgD;
                PSC_FPSG_BASE   EngFpsgD; EngFysgD.Initialize();
                if ( !m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpsg_PSC(ElemK, FpsgD) ) continue;

                DGN_JOINT_D JointD;
                m_DataCtrl.Get_JointData4Element(ElemK, JointD);

                BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

                PSC_DATA_MEMB PscMembD;
                for ( int j=0; j<2; ++j ) m_DataCtrl.m_pPscDataCtrl->Get_PscDataPosi(ElemK, j, PscMembD.Pos[j], bComposite);

                for ( int j = 0; j < 2; ++j )
                {
                    int nNode = j;    // 0=I, 1=J

                    PscMembD.Pos[j].iJoint = nNode==0 ? JointD.iI_joint : JointD.iJ_joint; //Joint
                    PscMembD.Pos[j].iJointType = JointD.iJointType;

                    if ( j==0 && !bCheckMomI && !bCheckMomNegI && !bCheckShrI && !bCheckTorI ) continue;
                    if ( j==1 && !bCheckMomJ && !bCheckMomNegJ && !bCheckShrJ && !bCheckTorJ ) continue;
                    
                    RptResD.Pos = PscMembD.Pos[j];
                    RptResD.Pos.CalcD.nClass = PoscD.nClass + 1;
                    RptResD.Pos.CalcD.bTypeC = PoscD.bTypeC;

                    int nPosNegIJ = 0;

                    RptResD.bShear = true;
                    RptResD.bTorsion = true;

                    for ( int k=0; k < 2; ++k)
                    {
                        RptResD.bFlexPos = true;
                        RptResD.bFlexNeg = true;

                        nPosNegIJ = j*2+k; //0,2 Positive 1,3 Negative

                        if ( m_PscdD.bUltimateEuro[0] && BdcrD.BdcrB[nPosNegIJ].bCHK )
                        {
                            m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData_BS(ElemK, j, k, BdcrD.BdcrB[nPosNegIJ], PscMembD.Pos[j], EngBdcrD);
                            RptResD.Fle[k] = EngBdcrD;
                            if ( nPosNegIJ == 0 && !bCheckMomI )    RptResD.bFlexPos = false;
                            if ( nPosNegIJ == 1 && !bCheckMomNegI ) RptResD.bFlexNeg = false;
                            if ( nPosNegIJ == 2 && !bCheckMomJ )    RptResD.bFlexPos = false;
                            if ( nPosNegIJ == 3 && !bCheckMomNegJ ) RptResD.bFlexNeg = false;
                        }

                        if ( m_PscdD.bServiceabilityEuro[0] && FycmD.FycmGrB[nPosNegIJ].bCHK )
                        {
                            InData.nGirder = PSC_DATA_POSI::Girder; //girder
                            m_DataCtrl.m_pPscDataCtrl->Get_FycmDetailReportData_BS(ElemK, j, k, FycmD.FycmGrB[nPosNegIJ], PscMembD.Pos[j], EngFycmD);
                            RptResD.FycmGr[k] = EngFycmD;
                            if ( bComposite )
                            {
                                InData.nGirder = PSC_DATA_POSI::Slab; //slab
                                m_DataCtrl.m_pPscDataCtrl->Get_FycmDetailReportData_BS(ElemK, j, k, FycmD.FycmSlB[nPosNegIJ], PscMembD.Pos[j], EngFycmD);
                                RptResD.FycmSl[k] = EngFycmD;
                            }
                        }

                        if ( m_PscdD.bServiceabilityEuro[1] && FysgD.FysgGrB[nPosNegIJ].bCHK )
                        {
                            InData.nGirder = PSC_DATA_POSI::Girder; //girder
                            m_DataCtrl.m_pPscDataCtrl->Get_FysgDetailReportData_BS(ElemK, j, k, FysgD.FysgGrB[nPosNegIJ], PscMembD.Pos[j], EngFysgD);
                            RptResD.FysgGr[k] = EngFysgD;
                            if ( bComposite )
                            {
                                InData.nGirder = PSC_DATA_POSI::Slab; //slab
                                m_DataCtrl.m_pPscDataCtrl->Get_FysgDetailReportData_BS(ElemK, j, k, FysgD.FysgSlB[nPosNegIJ], PscMembD.Pos[j], EngFysgD);
                                RptResD.FysgSl[k] = EngFysgD;
                            }
                        }
                    }
                    if ( m_PscdD.bUltimateEuro[1] && ShrrD.ShrrB[j].bCHK )
                    {
                        m_DataCtrl.m_pPscDataCtrl->Get_CumsReportData_BS(ElemK, j, 0, ShrrD.ShrrB[j], PscMembD.Pos[j], EngCumsD);
                        RptResD.Shr = EngCumsD;
                        if ( j==0 && !bCheckShrI )  RptResD.bShear = false;
                        if ( j==1 && !bCheckShrJ )  RptResD.bShear = false;
                    }

                    if ( m_PscdD.bUltimateEuro[2] && TorrD.TorrB[j].bCHK )
                    {
                        m_DataCtrl.m_pPscDataCtrl->Get_CrmtReportData_BS(ElemK, j, 0, TorrD.TorrB[j], PscMembD.Pos[j], EngCrmtD);
                        RptResD.Tor = EngCrmtD;
                        if ( j==0 && !bCheckTorI )  RptResD.bTorsion = false;
                        if ( j==1 && !bCheckTorJ )  RptResD.bTorsion = false;
                    }

                    PsdRptD.aRptD.Add(RptResD);
                }
            } // I/J

            for ( i=0; i<nPrintElemSize; i++ )
            {
                auto ElemK = aPrintEPairKey[i];

                // Stress for Cross Section at a Construction Stage
                if ( m_PscdD.bServiceabilityEuro[1] )
                {
                    T_FYSG_PSC_BS FysgD; FysgD.Initialize();
                    PSC_FYSG_D  dgnFysgGr; dgnFysgGr.Initialize();
                    PSC_FYSG_D  dgnFysgSl; dgnFysgSl.Initialize();
                    if ( !m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_BS(ElemK, FysgD) )	continue;
                    //
                    if ( FysgD.FysgGrB[0].bCHK || FysgD.FysgGrB[1].bCHK || FysgD.FysgGrB[2].bCHK || FysgD.FysgGrB[3].bCHK )
                    {
                        m_DataCtrl.m_pPscDataCtrl->Get_FysgReportData_BS(ElemK, FysgD, dgnFysgGr, dgnFysgSl);

                        for ( j=0; j<4; ++j )
                        {
                            if ( !dgnFysgGr.FysgBase[j].bCHK ) continue;
                            dgnFysgGr.FysgBase[j].bComposite = FysgD.bComposite;

                            if ( j==0 ) { dgnFysgGr.FysgBase[j].nPart = 0; dgnFysgGr.FysgBase[j].nCompTens = 0; }
                            else if ( j==1 ) { dgnFysgGr.FysgBase[j].nPart = 0; dgnFysgGr.FysgBase[j].nCompTens = 1; }
                            else if ( j==2 ) { dgnFysgGr.FysgBase[j].nPart = 1; dgnFysgGr.FysgBase[j].nCompTens = 0; }
                            else if ( j==3 ) { dgnFysgGr.FysgBase[j].nPart = 1; dgnFysgGr.FysgBase[j].nCompTens = 1; }
                            PsdRptD.arFysgGr.Add(dgnFysgGr.FysgBase[j]);
                            
                            if (FysgD.bComposite)
                            {
                                if ( !dgnFysgSl.FysgBase[j].bCHK ) continue;

                                if ( j==0 ) { dgnFysgSl.FysgBase[j].nPart = 0; dgnFysgSl.FysgBase[j].nCompTens = 0; }
                                else if ( j==1 ) { dgnFysgSl.FysgBase[j].nPart = 0; dgnFysgSl.FysgBase[j].nCompTens = 1; }
                                else if ( j==2 ) { dgnFysgSl.FysgBase[j].nPart = 1; dgnFysgSl.FysgBase[j].nCompTens = 0; }
                                else if ( j==3 ) { dgnFysgSl.FysgBase[j].nPart = 1; dgnFysgSl.FysgBase[j].nCompTens = 1; }
                                PsdRptD.arFysgSl.Add(dgnFysgSl.FysgBase[j]);
                            }
                        }
                    }
                }

                // Stress for Cross Section at Service Loads
                if ( m_PscdD.bServiceabilityEuro[0] )
                {
                    T_FYCM_PSC_BS FycmD; FycmD.Initialize();
                    PSC_FYCM_D  dgnFycmGr; dgnFycmGr.Initialize();
                    PSC_FYCM_D  dgnFycmSl; dgnFycmSl.Initialize();
                    if ( !m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_BS(ElemK, FycmD) ) continue;
                    //
                    if ( FycmD.FycmGrB[0].bCHK || FycmD.FycmGrB[1].bCHK || FycmD.FycmGrB[2].bCHK || FycmD.FycmGrB[3].bCHK )
                    {
                        m_DataCtrl.m_pPscDataCtrl->Get_FycmReportData_BS(ElemK, FycmD, dgnFycmGr, dgnFycmSl);

                        for ( j=0; j<4; ++j )
                        {
                            if ( !dgnFycmGr.FycmBase[j].bCHK ) continue;
                            dgnFycmGr.FycmBase[j].bComposite = FycmD.bComposite;
                            if ( j==0 ) { dgnFycmGr.FycmBase[j].nPart = 0; dgnFycmGr.FycmBase[j].nCompTens = 0; }
                            else if ( j==1 ) { dgnFycmGr.FycmBase[j].nPart = 0; dgnFycmGr.FycmBase[j].nCompTens = 1; }
                            else if ( j==2 ) { dgnFycmGr.FycmBase[j].nPart = 1; dgnFycmGr.FycmBase[j].nCompTens = 0; }
                            else if ( j==3 ) { dgnFycmGr.FycmBase[j].nPart = 1; dgnFycmGr.FycmBase[j].nCompTens = 1; }
                            PsdRptD.arFycmGr.Add(dgnFycmGr.FycmBase[j]);

                            if ( FycmD.bComposite )
                            {
                                if ( !dgnFycmSl.FycmBase[j].bCHK ) continue;

                                if ( j==0 ) { dgnFycmSl.FycmBase[j].nPart = 0; dgnFycmSl.FycmBase[j].nCompTens = 0; }
                                else if ( j==1 ) { dgnFycmSl.FycmBase[j].nPart = 0; dgnFycmSl.FycmBase[j].nCompTens = 1; }
                                else if ( j==2 ) { dgnFycmSl.FycmBase[j].nPart = 1; dgnFycmSl.FycmBase[j].nCompTens = 0; }
                                else if ( j==3 ) { dgnFycmSl.FycmBase[j].nPart = 1; dgnFycmSl.FycmBase[j].nCompTens = 1; }
                                PsdRptD.arFycmSl.Add(dgnFycmSl.FycmBase[j]);
                            }
                        }
                    }
                }

                // Principal Stress at a Construction Stage
                if ( m_PscdD.bServiceabilityEuro[3] )
                {
                    T_FPSG_PSC FpsgD; FpsgD.Initialize();
                    PSC_FPSG_D dgnFpsg;  dgnFpsg.Initialize();
                    if ( !m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpsg_PSC(ElemK, FpsgD) ) continue;

                    if ( FpsgD.FpsgBase[0].bCHK || FpsgD.FpsgBase[1].bCHK || FpsgD.FpsgBase[2].bCHK || FpsgD.FpsgBase[3].bCHK )
                    {
                        dgnFpsg.Initialize();
                        m_DataCtrl.m_pPscDataCtrl->Get_FpsgReportData_BS(ElemK, FpsgD, dgnFpsg);

                        for ( j=0; j<4; ++j )
                        {
                            if ( !dgnFpsg.FpsgBase[j].bCHK ) continue;

                            if ( j==0 ) { dgnFpsg.FpsgBase[j].nPart = 0; dgnFpsg.FpsgBase[j].nCompTens = 0; }
                            else if ( j==1 ) { dgnFpsg.FpsgBase[j].nPart = 0; dgnFpsg.FpsgBase[j].nCompTens = 1; }
                            else if ( j==2 ) { dgnFpsg.FpsgBase[j].nPart = 1; dgnFpsg.FpsgBase[j].nCompTens = 0; }
                            else if ( j==3 ) { dgnFpsg.FpsgBase[j].nPart = 1; dgnFpsg.FpsgBase[j].nCompTens = 1; }
                            PsdRptD.arFpsg.Add( dgnFpsg.FpsgBase[j] );
                        }
                    }
                }

                // Principal Stress at Service Loads
                if ( m_PscdD.bServiceabilityEuro[2] )
                {
                    T_FPMS_PSC FpmsD; FpmsD.Initialize();
                    PSC_FPMS_D  dgnFpms;  dgnFpms.Initialize();
                    if ( !m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpms_PSC(ElemK, FpmsD) ) continue;

                    if ( FpmsD.FpmsBase[0].bCHK || FpmsD.FpmsBase[1].bCHK || FpmsD.FpmsBase[2].bCHK || FpmsD.FpmsBase[3].bCHK )
                    {
                        dgnFpms.Initialize();
                        m_DataCtrl.m_pPscDataCtrl->Get_FpmsReportData_BS(ElemK, FpmsD, dgnFpms);

                        for ( j=0; j<4; ++j )
                        {
                            if ( !dgnFpms.FpmsBase[j].bCHK ) continue;

                            if ( j==0 ) { dgnFpms.FpmsBase[j].nPart = 0; dgnFpms.FpmsBase[j].nCompTens = 0; }
                            else if ( j==1 ) { dgnFpms.FpmsBase[j].nPart = 0; dgnFpms.FpmsBase[j].nCompTens = 1; }
                            else if ( j==2 ) { dgnFpms.FpmsBase[j].nPart = 1; dgnFpms.FpmsBase[j].nCompTens = 0; }
                            else if ( j==3 ) { dgnFpms.FpmsBase[j].nPart = 1; dgnFpms.FpmsBase[j].nCompTens = 1; }
                            PsdRptD.arFpms.Add(dgnFpms.FpmsBase[j]);
                        }
                    }
                }
            }

            if ( m_PscdD.bServiceabilityEuro[4] )
            {
                CArray<T_TNDN_K, T_TNDN_K> arTndnList;  arTndnList.RemoveAll();
                m_pDoc->m_pPostCtrl->GetPCDesign()->GetPscCheckTdnaList(arTndnList);

                int iTndnKey  = 0;
                int iTndnList = 0;
                iTndnList = arTndnList.GetSize();

                for ( i=0; i<iTndnList; ++i )
                {
                    iTndnKey  = 0;
                    iTndnKey  = arTndnList.GetAt(i);

                    T_FYPC_PSC_BS FypcD;    FypcD.Initialize();
                    PSC_FYPC_D    dgnFypcD; dgnFypcD.Initialize();
                    if ( !m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFypc_BS(iTndnKey, FypcD) ) continue;
                    m_DataCtrl.m_pPscDataCtrl->Get_FypcReportData_BS(iTndnKey, FypcD, dgnFypcD);
                    PsdRptD.arFpycInfo.Add(dgnFypcD);
                }
            }

            if ( m_bStopExecute )	return FALSE;
            nProgressPercent += nProgressIncrementPercent;
            Progress(0, nProgressPercent);
            Progress(2, nProgressPercent);

        } // for nPrintElemSize

        CDgn_PSCRptManager_BS  myBSPscReport;

        myBSPscReport.Print_DetailReport_BS(iDgnCode, m_pDoc->GetProgramPath(), GetSaveFileDirEx(), sSaveFileName, PsdRptD);
    }
    catch ( ... )
    {
        bSuccess = FALSE;
    }

    if ( m_bStopExecute )	return FALSE;
    nProgressPercent = 100;
    Progress(0, nProgressPercent);
    Progress(2, nProgressPercent);

    return bSuccess;

    return TRUE;

}

BOOL CCRCExcelOutput::Execute_PSCReport_TMH(int iDgnCode, CString sSaveFileName)
{
	// SET PROGRESS-BAR
	if (m_bStopExecute) return FALSE;
	Progress(0, 0);
	int nProgressPercent = 0;

	if (m_bStopExecute) return FALSE;
	nProgressPercent += 10;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	BOOL bSuccess = TRUE;

	if (!m_PscdD.bUltimateEuro[0] && !m_PscdD.bUltimateEuro[1] && !m_PscdD.bUltimateEuro[2] &&
		!m_PscdD.bServiceabilityEuro[0] && !m_PscdD.bServiceabilityEuro[1] && !m_PscdD.bServiceabilityEuro[2] &&
		!m_PscdD.bServiceabilityEuro[3] && !m_PscdD.bServiceabilityEuro[4] && !m_PscdD.bServiceabilityEuro[5])
	{
		// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!

		if (m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		return FALSE;
	}

	// Get all element keys for which results are to be printed
	CArray<T_POSX_K, T_POSX_K> arPrintElemKey;
	m_pDoc->m_pAttrCtrl->GetPosxKeyList(arPrintElemKey);

	CArray<T_POSV_K, T_POSV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl->GetPosvKeyList(aPrintVBeamKey);

	ArrElemPairKey aPrintEPairKey;
	CDBLib::ConvertToElemPairKeyList(arPrintElemKey, aPrintVBeamKey, aPrintEPairKey);

	int i = 0, j = 0, k = 0;
	int kk = 0;
	int nIJ = 0, nMaxMin = 0;
	BOOL bPrintI = FALSE, bPrintJ = FALSE;
	int nPrintElemSize = aPrintEPairKey.GetSize();  // Element Key..

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	if (nPrintElemSize == 0)
	{
		if (m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		return FALSE;
	}

	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40 / nPrintElemSize;

	// Detail Res-data.
	CArray<PSC_FRCR_D, PSC_FRCR_D> arFrcrD;  arFrcrD.RemoveAll();  arFrcrD.SetSize(4);
	//
	PSC_DATA_POSI InData;   InData.Initialize();
	PSC_RPT_TMH7_POS RptResD; RptResD.Initialize();
	PSC_RPT_TMH7_D PsdRptD;
	BOOL bCheckMomI = FALSE, bCheckMomJ = FALSE;
	BOOL bCheckMomNegI = FALSE, bCheckMomNegJ = FALSE;
	BOOL bCheckShrI = FALSE, bCheckShrJ = FALSE;
	BOOL bCheckTorI = FALSE, bCheckTorJ = FALSE;

	PsdRptD.bFlexRpt = m_PscdD.bUltimateEuro[0];
	PsdRptD.bShearRpt = m_PscdD.bUltimateEuro[1];
	PsdRptD.bTorRpt = m_PscdD.bUltimateEuro[2];
	PsdRptD.bFycmRpt = m_PscdD.bServiceabilityEuro[0];
	PsdRptD.bFysgRpt = m_PscdD.bServiceabilityEuro[1];
	PsdRptD.bFpmsRpt = m_PscdD.bServiceabilityEuro[2];
	PsdRptD.bFpsgRpt = m_PscdD.bServiceabilityEuro[3];
	PsdRptD.bFypcRpt = m_PscdD.bServiceabilityEuro[4];

	try
	{
		//DETATL-REPORT
		if (m_PscdD.bUltimateEuro[0] || m_PscdD.bUltimateEuro[1] || m_PscdD.bUltimateEuro[2] || m_PscdD.bServiceabilityEuro[0] || m_PscdD.bServiceabilityEuro[1])
		{
			T_ELEM_K  ElemK = 0;
			for (i = 0; i < nPrintElemSize; ++i)
			{
				//
				auto ElemK = aPrintEPairKey[i];
				//

				T_POSC_D PoscD; PoscD.Initialize();
				T_POSX_D PosxD; PosxD.Initialize();
				if (ElemK.second == EN_EL_BEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPosc(ElemK.first, PoscD)) continue;
					if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD)) continue;
				}
				else if (ElemK.second == EN_EL_VBEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPovc(ElemK.first, PoscD)) continue;
					if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD)) continue;
				}
				else ASSERT(0);

				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

				bCheckMomI = FALSE;
				bCheckMomJ = FALSE;
				ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);

				bCheckMomNegI = FALSE;
				bCheckMomNegJ = FALSE;
				ShouldPrintPosxMomIJ_Negative(PosxD, bCheckMomNegI, bCheckMomNegJ);

				bCheckShrI = FALSE;
				bCheckShrJ = FALSE;
				ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);

				bCheckTorI = FALSE;
				bCheckTorJ = FALSE;
				ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

				// SET DATA!

				T_BDCR_PSC_BS   BdcrD;
				PSC_FRCR_D      EngBdcrD; EngBdcrD.Initialize();
				if (!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_BS(ElemK, BdcrD)) continue;

				T_SHRR_PSC_BS   ShrrD;
				PSC_CUMS_BASE   EngCumsD; EngCumsD.Initialize();
				if (!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadShrr_BS(ElemK, ShrrD)) continue;

				T_TORR_PSC_BS   TorrD;
				PSC_CRMT_BASE   EngCrmtD; EngCrmtD.Initialize();
				if (!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadTorr_BS(ElemK, TorrD)) continue;

				T_FYCM_PSC_BS   FycmD;
				PSC_FYCM_BASE   EngFycmD; EngFycmD.Initialize();
				if (!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_BS(ElemK, FycmD)) continue;

				T_FYSG_PSC_BS   FysgD;
				PSC_FYSG_BASE   EngFysgD; EngFysgD.Initialize();
				if (!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_BS(ElemK, FysgD)) continue;

				T_FPMS_PSC      FpmsD;
				PSC_FPMS_BASE   EngFpmsD; EngFysgD.Initialize();
				if (!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpms_PSC(ElemK, FpmsD)) continue;

				T_FPSG_PSC      FpsgD;
				PSC_FPSG_BASE   EngFpsgD; EngFysgD.Initialize();
				if (!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpsg_PSC(ElemK, FpsgD)) continue;

				DGN_JOINT_D JointD;
				m_DataCtrl.Cal_LcomDataForDesignAll();
				m_DataCtrl.Get_JointData4Segment();
				m_DataCtrl.Get_JointData4Element(ElemK, JointD);

				BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

				PSC_DATA_MEMB PscMembD;
				for (int j = 0; j < 2; ++j) m_DataCtrl.m_pPscDataCtrl->Get_PscDataPosi(ElemK, j, PscMembD.Pos[j], bComposite);

				for (int j = 0; j < 2; ++j)
				{
					int nNode = j;    // 0=I, 1=J

					PscMembD.Pos[j].iJoint = nNode == 0 ? JointD.iI_joint : JointD.iJ_joint; //Joint
					PscMembD.Pos[j].iJointType = JointD.iJointType;

					if (j == 0 && !bCheckMomI && !bCheckMomNegI && !bCheckShrI && !bCheckTorI) continue;
					if (j == 1 && !bCheckMomJ && !bCheckMomNegJ && !bCheckShrJ && !bCheckTorJ) continue;

					RptResD.Pos = PscMembD.Pos[j];
					RptResD.Pos.CalcD.nClass = PoscD.nClass + 1;
					RptResD.Pos.CalcD.bTypeC = PoscD.bTypeC;

					int nPosNegIJ = 0;

					RptResD.bShear = true;
					RptResD.bTorsion = true;

					for (int k = 0; k < 2; ++k)
					{
						RptResD.bFlexPos = true;
						RptResD.bFlexNeg = true;

						nPosNegIJ = j * 2 + k; //0,2 Positive 1,3 Negative

						if (m_PscdD.bUltimateEuro[0] && BdcrD.BdcrB[nPosNegIJ].bCHK)
						{
							m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData_BS(ElemK, j, k, BdcrD.BdcrB[nPosNegIJ], PscMembD.Pos[j], EngBdcrD);
							RptResD.Fle[k] = EngBdcrD;
							if (nPosNegIJ == 0 && !bCheckMomI)    RptResD.bFlexPos = false;
							if (nPosNegIJ == 1 && !bCheckMomNegI) RptResD.bFlexNeg = false;
							if (nPosNegIJ == 2 && !bCheckMomJ)    RptResD.bFlexPos = false;
							if (nPosNegIJ == 3 && !bCheckMomNegJ) RptResD.bFlexNeg = false;
						}

						if (m_PscdD.bServiceabilityEuro[0] && FycmD.FycmGrB[nPosNegIJ].bCHK)
						{
							InData.nGirder = PSC_DATA_POSI::Girder; //girder
							m_DataCtrl.m_pPscDataCtrl->Get_FycmDetailReportData_BS(ElemK, j, k, FycmD.FycmGrB[nPosNegIJ], PscMembD.Pos[j], EngFycmD);
							RptResD.FycmGr[k] = EngFycmD;
							if (bComposite)
							{
								InData.nGirder = PSC_DATA_POSI::Slab; //slab
								m_DataCtrl.m_pPscDataCtrl->Get_FycmDetailReportData_BS(ElemK, j, k, FycmD.FycmSlB[nPosNegIJ], PscMembD.Pos[j], EngFycmD);
								RptResD.FycmSl[k] = EngFycmD;
							}
						}

						if (m_PscdD.bServiceabilityEuro[1] && FysgD.FysgGrB[nPosNegIJ].bCHK)
						{
							InData.nGirder = PSC_DATA_POSI::Girder; //girder
							m_DataCtrl.m_pPscDataCtrl->Get_FysgDetailReportData_BS(ElemK, j, k, FysgD.FysgGrB[nPosNegIJ], PscMembD.Pos[j], EngFysgD);
							RptResD.FysgGr[k] = EngFysgD;
							if (bComposite)
							{
								InData.nGirder = PSC_DATA_POSI::Slab; //slab
								m_DataCtrl.m_pPscDataCtrl->Get_FysgDetailReportData_BS(ElemK, j, k, FysgD.FysgSlB[nPosNegIJ], PscMembD.Pos[j], EngFysgD);
								RptResD.FysgSl[k] = EngFysgD;
							}
						}
					}
					if (m_PscdD.bUltimateEuro[1] && ShrrD.ShrrB[j].bCHK)
					{
						m_DataCtrl.m_pPscDataCtrl->Get_CumsReportData_BS(ElemK, j, 0, ShrrD.ShrrB[j], PscMembD.Pos[j], EngCumsD);
						RptResD.Shr = EngCumsD;
						if (j == 0 && !bCheckShrI)  RptResD.bShear = false;
						if (j == 1 && !bCheckShrJ)  RptResD.bShear = false;
					}

					if (m_PscdD.bUltimateEuro[2] && TorrD.TorrB[j].bCHK)
					{
						m_DataCtrl.m_pPscDataCtrl->Get_CrmtReportData_BS(ElemK, j, 0, TorrD.TorrB[j], PscMembD.Pos[j], EngCrmtD);
						RptResD.Tor = EngCrmtD;
						if (j == 0 && !bCheckTorI)  RptResD.bTorsion = false;
						if (j == 1 && !bCheckTorJ)  RptResD.bTorsion = false;
					}

					PsdRptD.aRptD.Add(RptResD);
				}
			} // I/J

			for (i = 0; i < nPrintElemSize; i++)
			{
				auto ElemK = aPrintEPairKey[i];

				// Stress for Cross Section at a Construction Stage
				if (m_PscdD.bServiceabilityEuro[1])
				{
					T_FYSG_PSC_BS FysgD; FysgD.Initialize();
					PSC_FYSG_D  dgnFysgGr; dgnFysgGr.Initialize();
					PSC_FYSG_D  dgnFysgSl; dgnFysgSl.Initialize();
					if (!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_BS(ElemK, FysgD))	continue;
					//
					if (FysgD.FysgGrB[0].bCHK || FysgD.FysgGrB[1].bCHK || FysgD.FysgGrB[2].bCHK || FysgD.FysgGrB[3].bCHK)
					{
						m_DataCtrl.m_pPscDataCtrl->Get_FysgReportData_BS(ElemK, FysgD, dgnFysgGr, dgnFysgSl);

						for (j = 0; j < 4; ++j)
						{
							if (!dgnFysgGr.FysgBase[j].bCHK) continue;
							dgnFysgGr.FysgBase[j].bComposite = FysgD.bComposite;

							if (j == 0) { dgnFysgGr.FysgBase[j].nPart = 0; dgnFysgGr.FysgBase[j].nCompTens = 0; }
							else if (j == 1) { dgnFysgGr.FysgBase[j].nPart = 0; dgnFysgGr.FysgBase[j].nCompTens = 1; }
							else if (j == 2) { dgnFysgGr.FysgBase[j].nPart = 1; dgnFysgGr.FysgBase[j].nCompTens = 0; }
							else if (j == 3) { dgnFysgGr.FysgBase[j].nPart = 1; dgnFysgGr.FysgBase[j].nCompTens = 1; }
							PsdRptD.arFysgGr.Add(dgnFysgGr.FysgBase[j]);

							if (FysgD.bComposite)
							{
								if (!dgnFysgSl.FysgBase[j].bCHK) continue;

								if (j == 0) { dgnFysgSl.FysgBase[j].nPart = 0; dgnFysgSl.FysgBase[j].nCompTens = 0; }
								else if (j == 1) { dgnFysgSl.FysgBase[j].nPart = 0; dgnFysgSl.FysgBase[j].nCompTens = 1; }
								else if (j == 2) { dgnFysgSl.FysgBase[j].nPart = 1; dgnFysgSl.FysgBase[j].nCompTens = 0; }
								else if (j == 3) { dgnFysgSl.FysgBase[j].nPart = 1; dgnFysgSl.FysgBase[j].nCompTens = 1; }
								PsdRptD.arFysgSl.Add(dgnFysgSl.FysgBase[j]);
							}
						}
					}
				}

				// Stress for Cross Section at Service Loads
				if (m_PscdD.bServiceabilityEuro[0])
				{
					T_FYCM_PSC_BS FycmD; FycmD.Initialize();
					PSC_FYCM_D  dgnFycmGr; dgnFycmGr.Initialize();
					PSC_FYCM_D  dgnFycmSl; dgnFycmSl.Initialize();
					if (!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_BS(ElemK, FycmD)) continue;
					//
					if (FycmD.FycmGrB[0].bCHK || FycmD.FycmGrB[1].bCHK || FycmD.FycmGrB[2].bCHK || FycmD.FycmGrB[3].bCHK)
					{
						m_DataCtrl.m_pPscDataCtrl->Get_FycmReportData_BS(ElemK, FycmD, dgnFycmGr, dgnFycmSl);

						for (j = 0; j < 4; ++j)
						{
							if (!dgnFycmGr.FycmBase[j].bCHK) continue;
							dgnFycmGr.FycmBase[j].bComposite = FycmD.bComposite;
							if (j == 0) { dgnFycmGr.FycmBase[j].nPart = 0; dgnFycmGr.FycmBase[j].nCompTens = 0; }
							else if (j == 1) { dgnFycmGr.FycmBase[j].nPart = 0; dgnFycmGr.FycmBase[j].nCompTens = 1; }
							else if (j == 2) { dgnFycmGr.FycmBase[j].nPart = 1; dgnFycmGr.FycmBase[j].nCompTens = 0; }
							else if (j == 3) { dgnFycmGr.FycmBase[j].nPart = 1; dgnFycmGr.FycmBase[j].nCompTens = 1; }
							PsdRptD.arFycmGr.Add(dgnFycmGr.FycmBase[j]);

							if (FycmD.bComposite)
							{
								if (!dgnFycmSl.FycmBase[j].bCHK) continue;

								if (j == 0) { dgnFycmSl.FycmBase[j].nPart = 0; dgnFycmSl.FycmBase[j].nCompTens = 0; }
								else if (j == 1) { dgnFycmSl.FycmBase[j].nPart = 0; dgnFycmSl.FycmBase[j].nCompTens = 1; }
								else if (j == 2) { dgnFycmSl.FycmBase[j].nPart = 1; dgnFycmSl.FycmBase[j].nCompTens = 0; }
								else if (j == 3) { dgnFycmSl.FycmBase[j].nPart = 1; dgnFycmSl.FycmBase[j].nCompTens = 1; }
								PsdRptD.arFycmSl.Add(dgnFycmSl.FycmBase[j]);
							}
						}
					}
				}

				// Principal Stress at a Construction Stage
				if (m_PscdD.bServiceabilityEuro[3])
				{
					T_FPSG_PSC FpsgD; FpsgD.Initialize();
					PSC_FPSG_D dgnFpsg;  dgnFpsg.Initialize();
					if (!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpsg_PSC(ElemK, FpsgD)) continue;

					if (FpsgD.FpsgBase[0].bCHK || FpsgD.FpsgBase[1].bCHK || FpsgD.FpsgBase[2].bCHK || FpsgD.FpsgBase[3].bCHK)
					{
						dgnFpsg.Initialize();
						m_DataCtrl.m_pPscDataCtrl->Get_FpsgReportData_BS(ElemK, FpsgD, dgnFpsg);

						for (j = 0; j < 4; ++j)
						{
							if (!dgnFpsg.FpsgBase[j].bCHK) continue;

							if (j == 0) { dgnFpsg.FpsgBase[j].nPart = 0; dgnFpsg.FpsgBase[j].nCompTens = 0; }
							else if (j == 1) { dgnFpsg.FpsgBase[j].nPart = 0; dgnFpsg.FpsgBase[j].nCompTens = 1; }
							else if (j == 2) { dgnFpsg.FpsgBase[j].nPart = 1; dgnFpsg.FpsgBase[j].nCompTens = 0; }
							else if (j == 3) { dgnFpsg.FpsgBase[j].nPart = 1; dgnFpsg.FpsgBase[j].nCompTens = 1; }
							PsdRptD.arFpsg.Add(dgnFpsg.FpsgBase[j]);
						}
					}
				}

				// Principal Stress at Service Loads
				if (m_PscdD.bServiceabilityEuro[2])
				{
					T_FPMS_PSC FpmsD; FpmsD.Initialize();
					PSC_FPMS_D  dgnFpms;  dgnFpms.Initialize();
					if (!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpms_PSC(ElemK, FpmsD)) continue;

					if (FpmsD.FpmsBase[0].bCHK || FpmsD.FpmsBase[1].bCHK || FpmsD.FpmsBase[2].bCHK || FpmsD.FpmsBase[3].bCHK)
					{
						dgnFpms.Initialize();
						m_DataCtrl.m_pPscDataCtrl->Get_FpmsReportData_BS(ElemK, FpmsD, dgnFpms);

						for (j = 0; j < 4; ++j)
						{
							if (!dgnFpms.FpmsBase[j].bCHK) continue;

							if (j == 0) { dgnFpms.FpmsBase[j].nPart = 0; dgnFpms.FpmsBase[j].nCompTens = 0; }
							else if (j == 1) { dgnFpms.FpmsBase[j].nPart = 0; dgnFpms.FpmsBase[j].nCompTens = 1; }
							else if (j == 2) { dgnFpms.FpmsBase[j].nPart = 1; dgnFpms.FpmsBase[j].nCompTens = 0; }
							else if (j == 3) { dgnFpms.FpmsBase[j].nPart = 1; dgnFpms.FpmsBase[j].nCompTens = 1; }
							PsdRptD.arFpms.Add(dgnFpms.FpmsBase[j]);
						}
					}
				}
			}

			if (m_PscdD.bServiceabilityEuro[4])
			{
				CArray<T_TNDN_K, T_TNDN_K> arTndnList;  arTndnList.RemoveAll();
				m_pDoc->m_pPostCtrl->GetPCDesign()->GetPscCheckTdnaList(arTndnList);

				int iTndnKey = 0;
				int iTndnList = 0;
				iTndnList = arTndnList.GetSize();

				for (i = 0; i < iTndnList; ++i)
				{
					iTndnKey = 0;
					iTndnKey = arTndnList.GetAt(i);

					T_FYPC_PSC_BS FypcD;    FypcD.Initialize();
					PSC_FYPC_D    dgnFypcD; dgnFypcD.Initialize();
					if (!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFypc_BS(iTndnKey, FypcD)) continue;
					m_DataCtrl.m_pPscDataCtrl->Get_FypcReportData_BS(iTndnKey, FypcD, dgnFypcD);
					PsdRptD.arFpycInfo.Add(dgnFypcD);
				}
			}

			if (m_bStopExecute)	return FALSE;
			nProgressPercent += nProgressIncrementPercent;
			Progress(0, nProgressPercent);
			Progress(2, nProgressPercent);

		} // for nPrintElemSize

		CDgn_PSCRptManager_TMH7  myTMH7PscReport;

		myTMH7PscReport.Print_DetailReport_TMH7(iDgnCode, m_pDoc->GetProgramPath(), GetSaveFileDirEx(), sSaveFileName, PsdRptD);
	}
	catch (...)
	{
		bSuccess = FALSE;
	}

	if (m_bStopExecute)	return FALSE;
	nProgressPercent = 100;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	return bSuccess;

	return TRUE;

}

BOOL CCRCExcelOutput::IsPrintPosiData()
{
	if(!IsExistPrintCheckOfGlobalData()) return FALSE;
	// 설계요소/출력요소별 정보를 비교해야 함. 0:None, 1:I, 2:J, 3:I&J

	int i=0;
	int j=0;
	CArray<T_POSC_K, T_POSC_K> arDesignElemKey; arDesignElemKey.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetPoscKeyList(arDesignElemKey);

	CArray<T_POVC_K, T_POVC_K> arDgnVBeamKey;
	m_pDoc->m_pAttrCtrl->GetPovcKeyList(arDgnVBeamKey);

	ArrElemPairKey aDgnEPairKey;
	CDBLib::ConvertToElemPairKeyList(arDesignElemKey, arDgnVBeamKey, aDgnEPairKey);


	CArray<T_POSX_K, T_POSX_K> arPrintElemKey;
	m_pDoc->m_pAttrCtrl->GetPosxKeyList(arPrintElemKey);
	
	CArray<T_POSV_K, T_POSV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl->GetPosvKeyList(aPrintVBeamKey);

	ArrElemPairKey aPrintEPairKey;
	CDBLib::ConvertToElemPairKeyList(arPrintElemKey, aPrintVBeamKey, aPrintEPairKey);

	if (!aDgnEPairKey.GetSize())
	{
		// DesignElemKey에 아무것도 지정되지 않은 경우 전체 설계 가능 Elem으로 설계함.
		aDgnEPairKey.RemoveAll();
		m_DataCtrl.Get_CrcElemListForDgn(FALSE, aDgnEPairKey);
	}

	// Check Array-Size!
	int iDesign = aDgnEPairKey.GetSize();
	int iReport = aPrintEPairKey.GetSize();

	if(iDesign==0) return FALSE; //
	if(iReport==0) return FALSE; // 출력정보가 없을 경우 return함.
		
	if(iDesign>0)
	{
		for(i=0; i<iReport; ++i)
		{
			auto iReportElemKey = aPrintEPairKey.GetAt(i);

			// PrintOpt.을 통해서 None Type을 고려하지 않도록 조치!
			T_POSX_D  PosxD;
			if (iReportElemKey.second == EN_EL_BEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetPosx(iReportElemKey.first, PosxD)) continue;
			}
			else if (iReportElemKey.second == EN_EL_VBEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetPosv(iReportElemKey.first, PosxD)) continue;
			}
			else ASSERT(0);
			
			if(!CheckPosx_NoneTypeElem(PosxD))  continue;

			for(j=0; j<iDesign; ++j)
			{
				auto iDesignElemKey  = aDgnEPairKey.GetAt(j);

				if(iDesignElemKey==iReportElemKey)  return TRUE;  // 교집합이 하나라도 존재하면 Search를 멈추고 출력함.        
			}
		}
	}
	else 
		return TRUE;    

	return FALSE;
}

BOOL CCRCExcelOutput::CheckPosx_NoneTypeElem(T_POSX_D PosxD)
{
	BOOL bOK = FALSE;

	int iDgnCode = m_PscdD.iDgnCode;

	//
	//하나의 아이템이라도 출력항모겡 포함된다면 TRUE를 Return함.
	if(iDgnCode==KSCE_USD03 || iDgnCode==KSCE_USD05 || iDgnCode==KSCE_USD10 || iDgnCode==KSCE_RAIL_USD04 || iDgnCode==KSCE_RAIL_USD11)
	{
		if(PosxD.iMomChk[0] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iMomChk[1] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iShrChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iTorChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iBarChk[0] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iBarChk[1] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
	}
	else if(CDBLib::IsPscCodeAASHTO(iDgnCode) || 
					CDBLib::IsPscCodeCSA(iDgnCode) || iDgnCode==JTG_D62_04|| iDgnCode==CJJ_11_2011)
	{
		if(PosxD.iMomChk[0] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iShrChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
	}
    else if ( CDBLib::IsPscCodeLSD(iDgnCode)|| iDgnCode==IRC_112_2011_PSC || iDgnCode==BS5400_90_PSC ||
					iDgnCode==AS_5100_5_17_PSC || iDgnCode== IRC_112_2020_PSC)
	{    
		if(PosxD.iMomChk[0] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iMomChk[1] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iShrChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iTorChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		/*
		if(PosxD.iMomChk[0]==0)         bOK = FALSE;
		if(!bOK && PosxD.iMomChk[1]==0) bOK = FALSE;
		if(!bOK && PosxD.iShrChk==0)    bOK = FALSE;
		if(!bOK && PosxD.iTorChk==0)    bOK = FALSE;
		*/
	}
	else if (iDgnCode==TMH07_3_1989)
	{
		if (PosxD.iMomChk[0] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if (PosxD.iMomChk[1] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if (PosxD.iShrChk > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if (PosxD.iTorChk > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		/*
		if(PosxD.iMomChk[0]==0)         bOK = FALSE;
		if(!bOK && PosxD.iMomChk[1]==0) bOK = FALSE;
		if(!bOK && PosxD.iShrChk==0)    bOK = FALSE;
		if(!bOK && PosxD.iTorChk==0)    bOK = FALSE;
		*/
	}
	else if (iDgnCode==SNiP_20503_84_PSC || iDgnCode==SP_35_13330_11_PSC || iDgnCode==SNiP_20503_84_PSC_MKS || iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		if(PosxD.iMomChk[0] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iMomChk[1] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iShrChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iTorChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.nFatiChk   > 0)  return TRUE;
		if(PosxD.nCrack     > 0)  return TRUE;
	}
	else if(iDgnCode==IRS_PSC )
	{    
		if(PosxD.iMomChk[0] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iMomChk[1] > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iShrChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		if(PosxD.iTorChk    > 0)  return TRUE;  //  0:None/ 1:I/ 2:J/ 3:I&J
		/*
		if(PosxD.iMomChk[0]==0)         bOK = FALSE;
		if(!bOK && PosxD.iMomChk[1]==0) bOK = FALSE;
		if(!bOK && PosxD.iShrChk==0)    bOK = FALSE;
		if(!bOK && PosxD.iTorChk==0)    bOK = FALSE;
		*/
	}
	else
		ASSERT(0);

	return bOK;
}

BOOL CCRCExcelOutput::IsExistPrintCheckOfGlobalData()
{	
	int i=0;
	int iDgnCode = m_PscdD.iDgnCode;
	BOOL bExistPrintItem=FALSE;

	if(iDgnCode==KSCE_USD03 || iDgnCode==KSCE_USD05 || iDgnCode==KSCE_USD10 || iDgnCode==KSCE_RAIL_USD04 || iDgnCode==KSCE_RAIL_USD11)
	{
		BOOL bPrtSheet = FALSE;
		for(i=0; i<5; i++ )
		{
			if(m_PscdD.bDgnPrint[i])    bExistPrintItem = TRUE;
			if(m_PscdD.bStructPrint[i]) bExistPrintItem = TRUE;
			if(bExistPrintItem) break;
		}		
	}
	else if(CDBLib::IsPscCodeAASHTO(iDgnCode) || 
					CDBLib::IsPscCodeCSA(iDgnCode) ||
					iDgnCode==SNiP_20503_84_PSC || iDgnCode==SP_35_13330_11_PSC || iDgnCode==SNiP_20503_84_PSC_MKS || iDgnCode==SP_35_13330_11_PSC_MKS ||
					iDgnCode==AS_5100_5_17_PSC)
	{
		for(int i=0; i<6; i++ ) { if(m_PscdD.bServPrint4AASHTO[i])    bExistPrintItem = TRUE; }
		if(!bExistPrintItem)
		{
			for(    i=0; i<4; i++ ) { if(m_PscdD.bStrePrint4AASHTO[i])    bExistPrintItem = TRUE; }
		}
		

	}
	else if( CDBLib::IsPscCodeLSD(iDgnCode) || iDgnCode==IRC_112_2011_PSC || iDgnCode==IRS_PSC || iDgnCode==BS5400_90_PSC ||
             iDgnCode == IRC_112_2020_PSC)
	{
		// Ultimate Moment/Shear/Torsional Resistance Detail + Summary!!
		for(i=0; i<3; ++i)
		{
			if(m_PscdD.bUltimateEuro[i])  bExistPrintItem = TRUE;
			if(bExistPrintItem) break;  // Prt항목이 하나라도 존재하면 검색과정을 중단함.
		}
		
		if(!bExistPrintItem)
		{
			// Result Table!!
			for(i=0; i<6; ++i)
			{
				if(m_PscdD.bServiceabilityEuro[i])  bExistPrintItem = TRUE;
				if(bExistPrintItem) break;  // Prt항목이 하나라도 존재하면 검색과정을 중단함.
			} 
		}
	}
	else if (iDgnCode == TMH07_3_1989)
	{
		// Ultimate Moment/Shear/Torsional Resistance Detail + Summary!!
		for (i = 0; i < 3; ++i)
		{
			if (m_PscdD.bUltimateEuro[i])  bExistPrintItem = TRUE;
			if (bExistPrintItem) break;  // Prt항목이 하나라도 존재하면 검색과정을 중단함.
		}

		if (!bExistPrintItem)
		{
			// Result Table!!
			for (i = 0; i < 6; ++i)
			{
				if (m_PscdD.bServiceabilityEuro[i])  bExistPrintItem = TRUE;
				if (bExistPrintItem) break;  // Prt항목이 하나라도 존재하면 검색과정을 중단함.
			}
		}
	}
	else ASSERT(0);  

	return bExistPrintItem;
}


BOOL CCRCExcelOutput::Convert_PSCBDCWtoAASHTOBDCW(const PSC_BDCW_BASE& dgnbdcw,  _BDCW_PSC_LRFD_BASE& BdcwBase)
{

// int    iElemK;
// CString strLcomName;
// double dft ;      //위연단 응력 [Unit=Stress]
// double dfb ;      //아래연단 응력 [Unit=Stress]
// double dEpsilon_cm;
// double dPu;   
// double dMu;   
// double dW_rat;  
	
	// EN1992-2-2:05
//   double dc_neu; // neutral axis, 허용응력법기준, [Unit=Length]
//   double dSigma_s;
//   double dKt;
//   double dfct_eff;
//   double dxi;
//   double dhc_ef;
//   double dphi_s;
//   double dphi_p;
//   double dxi_1;
//   double dAc_eff;
//   double dAp_prim;
//   double dRho_p_eff;
//   double dalpha_e;
//   double dEpsilon_sm_cm;
//   double dc;
//   double dphi;
//   double dk1;
//   double dk2;
//   double dSr_max;
//   BOOL bPureTension;
//   BOOL bUse7_14;  
	
	// CSA
//   double dAst4hc_ef; // [Unit=Area]   유효높이내에서 철근면적
//   double dApt4hc_ef; // [Unit=Area]   유효높이내에서 tendon 면적
//   double ddp;        // [Unit=Length] 균열 계산 위한 최외각 철근/tendon 위치 

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	BdcwBase.LInf.bCHK     = dgnbdcw.bCHK;		//[Unit=None] 
	BdcwBase.LInf.LcomK    = dgnbdcw.LcomK;	
	BdcwBase.LInf.nMax     = dgnbdcw.iMax;	
	BdcwBase.LInf.nKind    = dgnbdcw.iKind;	
	BdcwBase.dft      = dgnbdcw.dft;	
	BdcwBase.dfb      = dgnbdcw.dfb;	
	BdcwBase.dFSS     = dgnbdcw.dFSS;	
	BdcwBase.ds_use   = dgnbdcw.ds_use;	
	BdcwBase.ds_max   = dgnbdcw.ds_max;	
	BdcwBase.dGamma_e = 0.0; 
	BdcwBase.dBeta_s  = 0.0; 
	BdcwBase.ddc      = 0.0; 
	BdcwBase.bOK      = dgnbdcw.bOK;	
									 
	//CSA
	BdcwBase.dkc         = dgnbdcw.dkc;	
	BdcwBase.ddb         = dgnbdcw.ddb;	
	BdcwBase.dpc         = dgnbdcw.dpc;	
	BdcwBase.dAs         = dgnbdcw.dAs ;
	BdcwBase.dAct        = dgnbdcw.dAct;
	BdcwBase.dfs         = dgnbdcw.dfs;
	BdcwBase.dfw         = dgnbdcw.dfw;
	BdcwBase.dw          = dgnbdcw.dWk;
	BdcwBase.dw_max      = dgnbdcw.dWa;
	BdcwBase.dkb         = dgnbdcw.dkb;
	BdcwBase.dbeta_c     = dgnbdcw.dbeta_c;
	BdcwBase.dsm         = dgnbdcw.dsm;
	BdcwBase.dEpsilon_sm = dgnbdcw.dEpsilon_sm;

	BdcwBase.dhc_ef     = dgnbdcw.dhc_ef    ;
	BdcwBase.dhc_ef_1   = dgnbdcw.dhc_ef_1  ;
	BdcwBase.dhc_ef_2   = dgnbdcw.dhc_ef_2  ;
	BdcwBase.dhc_ef_3   = dgnbdcw.dhc_ef_3  ;
	BdcwBase.dAst4hc_ef = dgnbdcw.dAst4hc_ef;
	BdcwBase.dApt4hc_ef = dgnbdcw.dApt4hc_ef;
	BdcwBase.ddp        = dgnbdcw.ddp       ;
	BdcwBase.dc_neu     = dgnbdcw.dc_neu   ;
	BdcwBase.ddst       = dgnbdcw.ddst     ;
	
	BdcwBase.dNu    = dgnbdcw.dPu;
	BdcwBase.dMu    = dgnbdcw.dMu;
	BdcwBase.dSig_s = dgnbdcw.dSig_s;
	BdcwBase.dSig_p = dgnbdcw.dSig_p;
	BdcwBase.dns    = dgnbdcw.dns;
	BdcwBase.dnp    = dgnbdcw.dnp;
	BdcwBase.dAcr   = dgnbdcw.dAcr;
	BdcwBase.dIcr   = dgnbdcw.dIcr;

	return TRUE;
}


BOOL CCRCExcelOutput::Set_pExcel(CMSExcel*	 pXL)
{
	m_pXL = pXL;
	return TRUE;
}

BOOL CCRCExcelOutput::Execute_PSCReport_IRS(int iDgnCode, CString sSaveFileName)
{
	// SET PROGRESS-BAR
	if(m_bStopExecute) return FALSE;
	Progress(0, 0);
	int nProgressPercent = 0;

	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	CDgn_PSCRptManager_IRS  myEurocodePscReport;
	if(!myEurocodePscReport.Initialize(iDgnCode, m_pDoc->GetProgramPath(), GetSaveFileDirEx(), sSaveFileName)) return FALSE;

	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	//
	BOOL bSuccess = TRUE;

	if(!m_PscdD.bUltimateEuro[0]       && !m_PscdD.bUltimateEuro[1]       && !m_PscdD.bUltimateEuro[2]       &&
		 !m_PscdD.bServiceabilityEuro[0] && !m_PscdD.bServiceabilityEuro[1] && !m_PscdD.bServiceabilityEuro[2] &&
		 !m_PscdD.bServiceabilityEuro[3] && !m_PscdD.bServiceabilityEuro[4] && !m_PscdD.bServiceabilityEuro[5])
	{
		// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
		VERIFY(myEurocodePscReport.Terminate(iDgnCode, !bSuccess));

		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		return FALSE;
	}

	// Get all element keys for which results are to be printed
	CArray<T_POSX_K, T_POSX_K> arPrintElemKey;
	m_pDoc->m_pAttrCtrl->GetPosxKeyList(arPrintElemKey);

	CArray<T_POSV_K, T_POSV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl->GetPosvKeyList(aPrintVBeamKey);

	ArrElemPairKey aPrintEPairKey;
	CDBLib::ConvertToElemPairKeyList(arPrintElemKey, aPrintVBeamKey, aPrintEPairKey);

	int i=0, j=0, k=0;
	int kk=0;
	int nIJ=0, nMaxMin=0;
	BOOL bPrintI = FALSE, bPrintJ = FALSE;
	int nPrintElemSize = aPrintEPairKey.GetSize();  // Element Key..

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	if(nPrintElemSize==0)
	{
		VERIFY(myEurocodePscReport.Terminate(iDgnCode, !bSuccess));

		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		return FALSE;
	}

	//
	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40/nPrintElemSize;

	// Detail Res-data.
	CArray<PSC_FRCR_D,PSC_FRCR_D> arFrcrD;  arFrcrD.RemoveAll();  arFrcrD.SetSize(4);
	PSC_CUMS_D EngCumsD;  EngCumsD.Initialize();
	PSC_CRMT_D EngCrmtD;  EngCrmtD.Initialize();
	PSC_BDCW_D EngBdcwD;  EngBdcwD.Initialize();
	PSC_FATG_D EngFatgD;  EngFatgD.Initialize();
	//
	PSC_DATA_POSI InData;   InData.Initialize();

	BOOL bCheckMomI = FALSE,          bCheckMomJ = FALSE;
	BOOL bCheckMomI_Negative = FALSE, bCheckMomJ_Negative = FALSE;
	BOOL bCheckShrI = FALSE,          bCheckShrJ = FALSE;
	BOOL bCheckTorI = FALSE,          bCheckTorJ = FALSE;

	try
	{
		//DETATL-REPORT
		if(m_PscdD.bUltimateEuro[0] || m_PscdD.bUltimateEuro[1] || m_PscdD.bUltimateEuro[2] || m_PscdD.bServiceabilityEuro[5])
		{
			for(i=0; i<nPrintElemSize; ++i)
			{
				//
				auto ElemK = aPrintEPairKey[i];
				//
				T_POSX_D PosxD; PosxD.Initialize();
				if (ElemK.second == EN_EL_BEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD)) continue;
				}
				else if (ElemK.second == EN_EL_VBEAM)
				{
					if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD)) continue;
				}
				else ASSERT(0);

				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

				bCheckMomI = FALSE;
				bCheckMomJ = FALSE;
				ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);

				bCheckMomI_Negative = FALSE;
				bCheckMomJ_Negative = FALSE;
				ShouldPrintPosxMomIJ_Negative(PosxD, bCheckMomI_Negative, bCheckMomJ_Negative);

				bCheckShrI = FALSE;
				bCheckShrJ = FALSE;
				ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);

				bCheckTorI = FALSE;
				bCheckTorJ = FALSE;
				ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

				// SET DATA!
				T_BDCR_PSC_IRS       BdcrD;      BdcrD.Initialize();
				T_BDCR_PSC_COMP  BdcrDComp;  BdcrDComp.Initialize();
				PSC_FRCR_D       EngBdcrD;   EngBdcrD.Initialize();  //PSC_FRCR_D rData;

				T_CUMS_PSC_IRS       CumsD;	     CumsD.Initialize();
				T_CUMS_PSC_COMP  CumsDComp;	 CumsD.Initialize();
				PSC_CUMS_BASE    EngCumsD;   EngCumsD.Initialize();  //PSC_CUMS_BASE rData;

				T_CRMT_PSC_IRS       CrmtD;	     CrmtD.Initialize();
				T_CRMT_PSC_COMP  CrmtDComp;	 CrmtDComp.Initialize();
				PSC_CRMT_BASE    EngCrmtD;   EngCrmtD.Initialize();  //PSC_CRMT_BASE rData;

				T_BDCW_PSC_IRS       BdcwD;      BdcwD.Initialize();
				T_BDCW_PSC_COMP  BdcwDComp;  BdcwDComp.Initialize();
				PSC_BDCW_BASE    EngBdcwD;   EngBdcwD.Initialize();  //PSC_BDCW_BASE rData;

				T_FATG_PSC_IRS       FatgD;      FatgD.Initialize();
				PSC_FATG_BASE    EngFatgD;   EngFatgD.Initialize();  

				//GET DATA!
				for(j=0; j<2; j++) // I,J
				{
					// 다음의 경우를 제거할 수 있는지..확인할 것!!!!!!!
					// - Bending/Shear/Torsion을 출력하지 않고, Crack을 출력하는데, 
					// PrintOpt이 I/J 중 제거된 부분이 있다면..Detail을 출력하지 않도록 Sheet를 만들지 않아야 함.
					if(j==0 && !bCheckMomI && !bCheckMomI_Negative && !bCheckShrI && !bCheckTorI) continue;
					if(j==1 && !bCheckMomJ && !bCheckMomJ_Negative && !bCheckShrJ && !bCheckTorJ) continue;

					// 요소별 Position정보에 따라서 Sheet를 구분함. 즉 1_I / 1_J / 2_I / 2_J 등으로 Sheet 명칭 결정.
					// ***************************
					// Dgn Condition/Parameters
					// ***************************
					InData.Initialize();
					EngBdcrD.Initialize();
					if(bCompositeElem)
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, BdcrDComp))
						{
							m_DataCtrl.m_pPscDataCtrl->Get_BdcrCompReportData(ElemK, j, 0, BdcrDComp.BdcrBase[j*2], InData, EngBdcrD);
							if(!myEurocodePscReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
						else
						{
							if(!myEurocodePscReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
					}
					else
					{
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, BdcrD))
						{
							// if j=0, index is 0 / j=1, index is 2!!
							// if j=0, index is 0 / j=1, index is 0!! k는 Max/Min 변수이므로, 항상 Posi별 Max정보로 가정함. InData를 가져오기 위함이니..
							m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData(ElemK, j, 0, BdcrD.BdcrBase[j*2], InData, EngBdcrD);
							if(!myEurocodePscReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
						}
						else
							if(!myEurocodePscReport.Print_DetailReport_DgnCondition(iDgnCode, ElemK, j, InData, EngBdcrD)) {}
					}
	
					// ***************************
					// CHK Bending Resistance!
					// ***************************
					if(m_PscdD.bUltimateEuro[0])
					{
						if(bCompositeElem)
						{
								if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, BdcrDComp))
								{
								// Positive나 Negative가 하나라도 있으면..진행함.
								if((j==0 && (bCheckMomI || bCheckMomJ_Negative)) || (j==1 && (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++) // Positive/Negative
									{
									// 출력할 필요가 없는 부분은 빼자!!
									if(j==0 && k==0 && !bCheckMomI)           continue;
									if(j==0 && k==1 && !bCheckMomI_Negative)  continue;
									if(j==1 && k==0 && !bCheckMomJ)           continue;
									if(j==1 && k==1 && !bCheckMomJ_Negative)  continue;
										
									kk = 0;
									kk = j*2+k;
									if(!BdcrDComp.BdcrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
									InData.Initialize();
									EngBdcrD.Initialize();              
									m_DataCtrl.m_pPscDataCtrl->Get_BdcrCompReportData(ElemK, j, k, BdcrDComp.BdcrBase[kk], InData, EngBdcrD);
										
									//
									if(!myEurocodePscReport.Print_DetailReport_FlexureDesign(iDgnCode, ElemK, kk, InData, EngBdcrD))  {}
									}
								}
							}
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, BdcrD))
							{
								// Positive나 Negative가 하나라도 있으면..진행함.
								if((j==0 && (bCheckMomI || bCheckMomJ_Negative)) || (j==1 && (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++) // Positive/Negative
									{
										// 출력할 필요가 없는 부분은 빼자!!
										if(j==0 && k==0 && !bCheckMomI)           continue;
										if(j==0 && k==1 && !bCheckMomI_Negative)  continue;
										if(j==1 && k==0 && !bCheckMomJ)           continue;
										if(j==1 && k==1 && !bCheckMomJ_Negative)  continue;

										kk = 0;
										kk = j*2+k;
										if(!BdcrD.BdcrBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngBdcrD.Initialize();              
										m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData(ElemK, j, k, BdcrD.BdcrBase[kk], InData, EngBdcrD);

										//
										if(!myEurocodePscReport.Print_DetailReport_FlexureDesign(iDgnCode, ElemK, kk, InData, EngBdcrD))  {}
									}
								}
							}
						}
					}

					// ***************************
					// CHK Shear Resistance!
					// ***************************
					if(m_PscdD.bUltimateEuro[1])
					{
						if(bCompositeElem)
						{
								if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC_Comp(ElemK, CumsDComp))
								{
								T_BDCR_PSC_COMP Bdcr4ShrComp;
								m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp);
								if((j==0 && bCheckShrI) || (j==1 && bCheckShrJ))
								{
									for(k=0; k<1; k++) // Max/Min
									{          
									kk = 0;
									kk = j*2+k;
									if(!CumsDComp.CumsBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
									InData.Initialize();
									EngCumsD.Initialize();
									if (j==0) { EngCumsD.dMr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dphiMn : Bdcr4ShrComp.BdcrBase[0].dphiMn; }
									else      { EngCumsD.dMr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dphiMn : Bdcr4ShrComp.BdcrBase[2].dphiMn; }

														if (j==0) { EngCumsD.dz_sh = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
									else      { EngCumsD.dz_sh = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }
										
									if (j==0) { EngCumsD.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_gr : Bdcr4ShrComp.BdcrBase[0].dCc_gr; }
									else      { EngCumsD.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_gr : Bdcr4ShrComp.BdcrBase[2].dCc_gr; }

									if (j==0) { EngCumsD.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_sl : Bdcr4ShrComp.BdcrBase[0].dCc_sl; }
									else      { EngCumsD.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_sl : Bdcr4ShrComp.BdcrBase[2].dCc_sl; }

									m_DataCtrl.m_pPscDataCtrl->Get_CumsCompReportData(ElemK, j, k, CumsDComp.CumsBase[kk], InData, EngCumsD);
										
									//
									if(!myEurocodePscReport.Print_DetailReport_ShearDesign_IRS(iDgnCode, ElemK, kk, InData, EngCumsD, EngBdcrD)) {ASSERT(0);}
									}
								}
								}
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC(ElemK, CumsD))
							{
								if((j==0 && bCheckShrI) || (j==1 && bCheckShrJ))
								{
									for(k=0; k<1; k++) // Max/Min
									{          
										kk = 0;
										kk = j*2+k;
										if(!CumsD.CumsBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngCumsD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_CumsReportData(ElemK, j, k, CumsD.CumsBase[kk], InData, EngCumsD);

										//
										if(!myEurocodePscReport.Print_DetailReport_ShearDesign_IRS(iDgnCode, ElemK, kk, InData, EngCumsD, EngBdcrD)) {ASSERT(0);}
									}
								}
							}
						}
					}

					// ***************************
					// CHK Torsion Resistance!
					// ***************************
					if(m_PscdD.bUltimateEuro[2])
					{
						// 지금은 General section이므로 출력 보류!!
						if(bCompositeElem)
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC_Comp(ElemK, CrmtDComp))
							{
											T_BDCR_PSC_COMP Bdcr4ShrComp;
											m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp);

							if((j==0 && bCheckTorI) || (j==1 && bCheckTorJ))
							{
								for(k=0; k<3; k++) // Max/Min
								{          
								kk = 0;
								kk = j*3+k;
								if(!CrmtDComp.CrmtBase[kk].bCHK) continue; // 설계결과가 없으면 Continue
										
								InData.Initialize();
								EngCrmtD.Initialize();

													if (j==0) { EngCrmtD.dz_sh = (CrmtDComp.CrmtBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
													else      { EngCrmtD.dz_sh = (CrmtDComp.CrmtBase[kk].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

								m_DataCtrl.m_pPscDataCtrl->Get_CrmtCompReportData(ElemK, j, k, CrmtDComp.CrmtBase[kk], InData, EngCrmtD);
										
								//
								if(!myEurocodePscReport.Print_DetailReport_TorsionDesign(iDgnCode, ElemK, kk, InData, EngCrmtD)) {ASSERT(0);}
								}
							}
							} 
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC(ElemK, CrmtD))
							{
								if((j==0 && bCheckTorI) || (j==1 && bCheckTorJ))
								{
									for(k=0; k<3; k++) // Max/Min
									{          
										kk = 0;
										kk = j*3+k;
										if(!CrmtD.CrmtBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngCrmtD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_CrmtReportData(ElemK, j, k, CrmtD.CrmtBase[kk], InData, EngCrmtD);

										//
										if(!myEurocodePscReport.Print_DetailReport_TorsionDesign(iDgnCode, ElemK, kk, InData, EngCrmtD)) {ASSERT(0);}
									}
								}
							}
						}
					}

					// ***************************
					// CHK Crack
					// ***************************
					if(m_PscdD.bServiceabilityEuro[5])
					{
						if(bCompositeElem) // 합성
						{
								if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_PSC_Comp(ElemK, BdcwDComp))
							{
							// 영응력 검토
							if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
							{
								for(k=0; k<2; k++)
								{
								// Fatigue Check는 Moment Dir에 관계없이 Top/Bottm 각각을 출력함.
								/*
								if(j==0 && k==0 && !bCheckMomI)           continue;
								if(i==0 && k==1 && !bCheckMomI_Negative)  continue;
								if(j==1 && k==0 && !bCheckMomJ)           continue;
								if(i==1 && k==1 && !bCheckMomJ_Negative)  continue;
								*/

								kk = 0;
								kk = j*2+k;
								if(!BdcwDComp.BdcwBaseZero[kk].bCHK) continue;  // kk : 0=I-Top, 1=I-Bot, 2=J-Top, 3=J-Bot 

								InData.Initialize();
								EngBdcwD.Initialize();
								m_DataCtrl.m_pPscDataCtrl->Get_BdcwCompReportData(ElemK, j, k, BdcwDComp.BdcwBaseZero[kk], InData, EngBdcwD);
 
								if(!myEurocodePscReport.Print_DetailReport_ZeroStressDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)){ASSERT(0);}
								}
							}

							// 균열
							// Crack은 Moment와 동일한 조건을 고려하도록 한다. (Agree with JOAHN)
							if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
							{
								for(k=0; k<2; k++)
								{
								// Crack Check는 Moment Dir에 관계없이 Top/Bottm 각각을 출력함.
								/*
								if(j==0 && k==0 && !bCheckMomI)           continue;
								if(i==0 && k==1 && !bCheckMomI_Negative)  continue;
								if(j==1 && k==0 && !bCheckMomJ)           continue;
								if(i==1 && k==1 && !bCheckMomJ_Negative)  continue;
								*/

								kk = 0;
								kk = j*2+k;
								if(!BdcwDComp.BdcwBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

								InData.Initialize();
								EngBdcwD.Initialize();
								m_DataCtrl.m_pPscDataCtrl->Get_BdcwCompReportData(ElemK, j, k, BdcwDComp.BdcwBase[kk], InData, EngBdcwD);
 
								if(!myEurocodePscReport.Print_DetailReport_CrackDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) {ASSERT(0);}

								}
							}
							}
						}
						else
						{
							if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcw_PSC(ElemK, BdcwD))
							{
								// Crack은 Moment와 동일한 조건을 고려하도록 한다. (Agree with JOAHN)
								if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
								{
									for(k=0; k<2; k++)
									{
										// Crack Check는 Moment Dir에 관계없이 Top/Bottm 각각을 출력함.
										/*
										if(j==0 && k==0 && !bCheckMomI)           continue;
										if(i==0 && k==1 && !bCheckMomI_Negative)  continue;
										if(j==1 && k==0 && !bCheckMomJ)           continue;
										if(i==1 && k==1 && !bCheckMomJ_Negative)  continue;
										*/

										kk = 0;
										kk = j*2+k;
										//if(!BdcwD.BdcwBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

										InData.Initialize();
										EngBdcwD.Initialize();
										m_DataCtrl.m_pPscDataCtrl->Get_BdcwReportData(ElemK, j, k, BdcwD.BdcwBase[kk], InData, EngBdcwD);

										if(!myEurocodePscReport.Print_DetailReport_CrackDesign(iDgnCode, ElemK, kk, InData, EngBdcwD)) {ASSERT(0);}
									}
								}
							}
						} 
					} // CHK Crack 

					// ***************************
					// CHK Fatigue
					// ***************************
					if(TRUE/*m_PscdD.bServiceabilityEuro[5]*/)
					{ 
						if(m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFatg_PSC(ElemK, FatgD))
						{
							
							if((j==0 && (bCheckMomI || bCheckMomI_Negative)) || (j==1 || (bCheckMomJ || bCheckMomJ_Negative)))
							{
								for(k=0; k<2; k++)
								{
									// Crack Check는 Moment Dir에 관계없이 Top/Bottm 각각을 출력함.
									/*
									if(j==0 && k==0 && !bCheckMomI)           continue;
									if(i==0 && k==1 && !bCheckMomI_Negative)  continue;
									if(j==1 && k==0 && !bCheckMomJ)           continue;
									if(i==1 && k==1 && !bCheckMomJ_Negative)  continue;
									*/

									kk = 0;
									kk = j*2+k;
									if(!FatgD.FatgBase[kk].bCHK) continue; // 설계결과가 없으면 Continue

									InData.Initialize();
									EngFatgD.Initialize();
									m_DataCtrl.m_pPscDataCtrl->Get_FatgReportData(ElemK, j, k, FatgD.FatgBase[kk], InData, EngFatgD);

									// 현재 출력 막음.
									//if(!myEurocodePscReport.Print_DetailReport_FatigueDesign(iDgnCode, ElemK, kk, InData, EngFatgD)) {ASSERT(0);}
								}
							}
						}
					}
				
				} // I/J
				
				if(m_bStopExecute)	return FALSE;
				nProgressPercent += nProgressIncrementPercent;
				Progress(0, nProgressPercent);
				Progress(2, nProgressPercent);

			} // for nPrintElemSize
		}
	
		//RESULT-TABLE
		T_POSX_D PosxD; PosxD.Initialize();
		BOOL bReport= FALSE;
		BOOL bFinal = FALSE;
		BOOL bChk_I_Crk = FALSE;  // Searching Final Row!!
		BOOL bChk_J_Crk = FALSE;
		BOOL bChk_I_SCS = FALSE;  // Searching Final Row!!
		BOOL bChk_J_SCS = FALSE;
		BOOL bChk_I_SSV = FALSE;  // Searching Final Row!!
		BOOL bChk_J_SSV = FALSE;
		BOOL bChk_I_PSC = FALSE;  // Searching Final Row!!
		BOOL bChk_J_PSC = FALSE;
		BOOL bChk_I_PSS = FALSE;  // Searching Final Row!!
		BOOL bChk_J_PSS = FALSE;

		//
		bCheckMomI = FALSE;
		bCheckMomJ = FALSE;
		//
		bCheckShrI = FALSE;
		bCheckShrJ = FALSE;
		//
		bCheckTorI = FALSE;
		bCheckTorJ = FALSE;

        CArray<_T_RPT_TABLE<PSC_FRCR_D>, _T_RPT_TABLE<PSC_FRCR_D>&> aTRptBR; aTRptBR.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_CUMS_BASE>, _T_RPT_TABLE<PSC_CUMS_BASE>&> aTRptSR; aTRptSR.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_CRMT_BASE>, _T_RPT_TABLE<PSC_CRMT_BASE>&> aTRptTR; aTRptTR.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FYSG_BASE>, _T_RPT_TABLE<PSC_FYSG_BASE>&> aTRptSC; aTRptSC.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FYCM_BASE>, _T_RPT_TABLE<PSC_FYCM_BASE>&> aTRptSS; aTRptSS.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FPSG_BASE>, _T_RPT_TABLE<PSC_FPSG_BASE>&> aTRptPC; aTRptPC.RemoveAll();
        CArray<_T_RPT_TABLE<PSC_FPMS_BASE>, _T_RPT_TABLE<PSC_FPMS_BASE>&> aTRptPS; aTRptPS.RemoveAll();

        nProgressIncrementPercent = 4;// nPrintElemSize == 0 ? 30 : 30/nPrintElemSize;
		int iPosiNo = 0;
		int iMaxMin = 0;
		for(i=0; i<nPrintElemSize; i++)
		{
			auto ElemK = aPrintEPairKey[i];

			BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);

			//Detail Report의 변수는 Calc의 변수를 가져와야 함.
			//Result Table의 변수는 Civil내 저장된 변수를 가져와야 함.
			PosxD.Initialize();
			if (ElemK.second == EN_EL_BEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD)) {};
			}
			else if (ElemK.second == EN_EL_VBEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD)) {};
			}
			else ASSERT(0);

			bCheckMomI = FALSE;
			bCheckMomJ = FALSE;
			ShouldPrintPosxMomIJ(PosxD, bCheckMomI, bCheckMomJ);

			bCheckShrI = FALSE;
			bCheckShrJ = FALSE;
			ShouldPrintPosxShrIJ(PosxD, bCheckShrI, bCheckShrJ);

			bCheckTorI = FALSE;
			bCheckTorJ = FALSE;
			ShouldPrintPosxTorIJ(PosxD, bCheckTorI, bCheckTorJ);

			// ***************************
			// Bending Resistacne
			// ***************************
			bReport = m_PscdD.bUltimateEuro[0];
			if(bReport)
			{
				T_BDCR_PSC_COMP BdcrDComp; BdcrDComp.Initialize();
				T_BDCR_PSC_IRS      BdcrD;     BdcrD.Initialize();
				if(bCompositeElem)    
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, BdcrDComp)) continue;
				}
				else                  
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC(ElemK, BdcrD)) continue; 
				}

				PSC_FRCR_D dgnFrcr; dgnFrcr.Initialize(); 

				//
				for(j=0; j<4; ++j)
				{
					// DATA Check!!
					BOOL bCHK = (bCompositeElem)? BdcrDComp.BdcrBase[j].bCHK : BdcrD.BdcrBase[j].bCHK;
					if(!bCHK) continue;
					//
					InData.Initialize();
					dgnFrcr.Initialize(); 

					iPosiNo = 0;  iPosiNo = (j<2)? 0 : 1;
					iMaxMin = 0;  iMaxMin = j%2;

					if(bCompositeElem)  m_DataCtrl.m_pPscDataCtrl->Get_BdcrCompReportData(ElemK, iPosiNo, iMaxMin, BdcrDComp.BdcrBase[j], InData, dgnFrcr);
					else                m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData    (ElemK, iPosiNo, iMaxMin, BdcrD.BdcrBase[j],     InData, dgnFrcr);
					//                  m_DataCtrl.m_pPscDataCtrl->Get_BdcrReportData    (ElemK, iPosiNo, iMaxMin, BdcrD.BdcrBase[j], InData, dgnFrcr);
				
					if(!dgnFrcr.bChk) continue; // 설계결과가 없을 경우 SKIP함.

					bFinal = TRUE;
					if(!bCheckMomI && !bCheckMomJ)  continue;
					else
					{
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						if((j<2 && bCheckMomI) || (j>1 && bCheckMomJ))
						{
                            _T_RPT_TABLE<PSC_FRCR_D> rptD;
                            rptD.iIJ = j;
                            rptD.RptData = dgnFrcr;
                            aTRptBR.Add(rptD);
						}
					}
				}
			}

			// ***************************
			// Shear Resistance
			// ***************************
			bReport = m_PscdD.bUltimateEuro[1];
			if(bReport)
			{
				T_BDCR_PSC_COMP Bdcr4ShrComp; Bdcr4ShrComp.Initialize();

				T_CUMS_PSC_COMP CumsDComp; CumsDComp.Initialize();
				T_CUMS_PSC_IRS CumsD; CumsD.Initialize();

				BOOL bCompositeElem = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);
				if(bCompositeElem) 
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC_Comp(ElemK, CumsDComp)) continue;

					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadBdcr_PSC_Comp(ElemK, Bdcr4ShrComp)) continue;
				}
				else               
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCums_PSC(ElemK, CumsD)) continue;
				}

				//
				PSC_CUMS_BASE dgncums;  dgncums.Initialize();      
				for(j=0; j<4; ++j)
				{
					//
					BOOL bCHK = (bCompositeElem)? CumsDComp.CumsBase[j].bCHK : CumsD.CumsBase[j].bCHK;
					if(!bCHK) continue;

					//
					InData.Initialize();
					dgncums.Initialize();

					iPosiNo = 0;  iPosiNo = (j<2)? 0 : 1;
					iMaxMin = 0;  iMaxMin = j%2;

					if(bCompositeElem)  
					{
						if (iPosiNo==0) { dgncums.dMr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dphiMn : Bdcr4ShrComp.BdcrBase[0].dphiMn; }
						else            { dgncums.dMr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dphiMn : Bdcr4ShrComp.BdcrBase[2].dphiMn; }
						
						if (iPosiNo==0) { dgncums.dz_sh = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
						else            { dgncums.dz_sh = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

						if (iPosiNo==0) { dgncums.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_gr : Bdcr4ShrComp.BdcrBase[0].dCc_gr; }
						else            { dgncums.BdcwBaseComp.dCc_gr = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_gr : Bdcr4ShrComp.BdcrBase[2].dCc_gr; }

						if (iPosiNo==0) { dgncums.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dCc_sl : Bdcr4ShrComp.BdcrBase[0].dCc_sl; }
						else            { dgncums.BdcwBaseComp.dCc_sl = (CumsDComp.CumsBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dCc_sl : Bdcr4ShrComp.BdcrBase[2].dCc_sl; }

						m_DataCtrl.m_pPscDataCtrl->Get_CumsCompReportData(ElemK, iPosiNo, iMaxMin, CumsDComp.CumsBase[j], InData, dgncums);
					}
					else                
					{
						m_DataCtrl.m_pPscDataCtrl->Get_CumsReportData    (ElemK, iPosiNo, iMaxMin, CumsD.CumsBase[j],     InData, dgncums);
					}

					if(!dgncums.bCHK) continue; // 설계결과가 없을 경우 SKIP함.

					bFinal = TRUE;
					if(!bCheckShrI && !bCheckShrJ)  continue;
					else
					{
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						if((j<2 && bCheckShrI) || (j>1 && bCheckShrJ))
						{
                            _T_RPT_TABLE<PSC_CUMS_BASE> rptD;
                            rptD.iIJ = j;
                            rptD.RptData = dgncums;
                            aTRptSR.Add(rptD);
						}
					}
				}
			}

			// ***************************
			// Torsional Resistance
			// ***************************
			bReport = m_PscdD.bUltimateEuro[2];
			if(bReport)
			{
				T_BDCR_PSC_COMP Bdcr4ShrComp; Bdcr4ShrComp.Initialize();

				T_CRMT_PSC_COMP CrmtDComp; CrmtDComp.Initialize();
				T_CRMT_PSC_IRS      CrmtD;     CrmtD.Initialize();

				if(bCompositeElem)  
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC_Comp(ElemK, CrmtDComp)) continue;
				}
				else                
				{
					if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadCrmt_PSC(ElemK, CrmtD)) continue;
				}

				//
				PSC_CRMT_BASE dgncrmt;  dgncrmt.Initialize();
				//
				for(j=0; j<6; ++j)
				{
					//
					BOOL bCHK = (bCompositeElem)? CrmtDComp.CrmtBase[j].bCHK : CrmtD.CrmtBase[j].bCHK;
					if(!bCHK) continue;

					//
					InData.Initialize();
					dgncrmt.Initialize();

					iPosiNo = 0;  iPosiNo = (j<3)? 0 : 1;
					iMaxMin = 0;  iMaxMin = j%3;

					if(bCompositeElem)  
					{
						
						if (iPosiNo==0) { dgncrmt.dz_sh = (CrmtDComp.CrmtBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[1].dz_sh : Bdcr4ShrComp.BdcrBase[0].dz_sh; }
						else            { dgncrmt.dz_sh = (CrmtDComp.CrmtBase[j].dMu<0.0) ? Bdcr4ShrComp.BdcrBase[3].dz_sh : Bdcr4ShrComp.BdcrBase[2].dz_sh; }

						m_DataCtrl.m_pPscDataCtrl->Get_CrmtCompReportData(ElemK, iPosiNo, iMaxMin, CrmtDComp.CrmtBase[j], InData, dgncrmt);

					}
					else    
					{
						m_DataCtrl.m_pPscDataCtrl->Get_CrmtReportData    (ElemK, iPosiNo, iMaxMin, CrmtD.CrmtBase[j], InData, dgncrmt);
					}
					//
					if(!dgncrmt.bCHK) continue; // 설계결과가 없을 경우 SKIP함.

					bFinal = TRUE;
					if(!bCheckTorI && !bCheckTorJ)  continue;
					else
					{
						// Data가 없을 경우는 _T("-")로 표현해야 하므로 함수로 그대로 연결함.
						if((j<3 && bCheckTorI) || (j>2 && bCheckTorJ))
						{
                            _T_RPT_TABLE<PSC_CRMT_BASE> rptD;
                            rptD.iIJ = j;
                            rptD.RptData = dgncrmt;
                            aTRptTR.Add(rptD);
						}
					}
				}
			}

			// ************************************************
			// stress for cross section at a construction stage
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[0];
			if(bReport)
			{
				T_FYSG_PSC_IRS      FysgD;     FysgD.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFysg_PSC(ElemK, FysgD))	continue;
				PSC_FYSG_D dgnFysg; dgnFysg.Initialize();

				//
				if(FysgD.FysgBase[0].bCHK || FysgD.FysgBase[1].bCHK || FysgD.FysgBase[2].bCHK || FysgD.FysgBase[3].bCHK)
				{
					dgnFysg.Initialize();
					m_DataCtrl.m_pPscDataCtrl->Get_FysgReportData(ElemK, FysgD, dgnFysg);
					for(j=0; j<4; ++j)
					{
						if(!dgnFysg.FysgBase[j].bCHK) continue;
                        _T_RPT_TABLE<PSC_FYSG_BASE> rptD;
                        rptD.iIJ = j;
                        rptD.RptData = dgnFysg.FysgBase[j];
                        aTRptSC.Add(rptD);
					}
				}
				
			}

			// ************************************************
			// stress for cross section at service loads
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[1];
			if(bReport)
			{
				T_FYCM_PSC_IRS FycmD; FycmD.Initialize();
				PSC_FYCM_D dgnFycm; dgnFycm.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFycm_PSC(ElemK, FycmD)) continue;
				//
				if(FycmD.FycmBase[0].bCHK || FycmD.FycmBase[1].bCHK || FycmD.FycmBase[2].bCHK || FycmD.FycmBase[3].bCHK)
				{
					dgnFycm.Initialize();
					m_DataCtrl.m_pPscDataCtrl->Get_FycmReportData(ElemK, FycmD, dgnFycm);
					for(j=0; j<4; ++j)
					{
						if(!dgnFycm.FycmBase[j].bCHK) continue;
                        _T_RPT_TABLE<PSC_FYCM_BASE> rptD;
                        rptD.iIJ = j;
                        rptD.RptData = dgnFycm.FycmBase[j];
                        aTRptSS.Add(rptD);
					}
				}
				
			}

			// ************************************************
			// principal stress at a construction stage
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[2];
			if(bReport)
			{
				T_FPSG_PSC_IRS FpsgD; FpsgD.Initialize();
				PSC_FPSG_D  dgnFpsg;  dgnFpsg.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpsg_PSC(ElemK, FpsgD)) continue;

				if(FpsgD.FpsgBase[0].bCHK || FpsgD.FpsgBase[1].bCHK || FpsgD.FpsgBase[2].bCHK || FpsgD.FpsgBase[3].bCHK)
				{
					dgnFpsg.Initialize();
					m_DataCtrl.m_pPscDataCtrl->Get_FpsgReportData(ElemK, FpsgD, dgnFpsg);
					for(j=0; j<4; ++j)
					{
						if(!dgnFpsg.FpsgBase[j].bCHK) continue;
                        _T_RPT_TABLE<PSC_FPSG_BASE> rptD;
                        rptD.iIJ = j;
                        rptD.RptData = dgnFpsg.FpsgBase[j];
                        aTRptPC.Add(rptD);
					}
				}
				
			}

			// ************************************************
			// principal stress at service loads
			// ************************************************
			bReport = m_PscdD.bServiceabilityEuro[3];
			if(bReport)
			{
				T_FPMS_PSC_IRS FpmsD; FpmsD.Initialize();
				PSC_FPMS_D  dgnFpms;  dgnFpms.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFpms_PSC(ElemK, FpmsD)) continue;

				if(FpmsD.FpmsBase[0].bCHK || FpmsD.FpmsBase[1].bCHK || FpmsD.FpmsBase[2].bCHK || FpmsD.FpmsBase[3].bCHK)
				{
					dgnFpms.Initialize();          
					m_DataCtrl.m_pPscDataCtrl->Get_FpmsReportData(ElemK, FpmsD, dgnFpms);

					for(j=0; j<4; ++j)
					{
						if(!dgnFpms.FpmsBase[j].bCHK) continue;
                        _T_RPT_TABLE<PSC_FPMS_BASE> rptD;
                        rptD.iIJ = j;
                        rptD.RptData = dgnFpms.FpmsBase[j];
                        aTRptPS.Add(rptD);
					}
				}
				
			}

			if(m_bStopExecute)	return FALSE;			
		}
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);

        INT_PTR nTBR = aTRptBR.GetSize();
        for (int i = 0; i<nTBR; ++i)
        {
            BOOL bFinal = (i==(nTBR-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FRCR_D> TRptD = aTRptBR.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_FlexureDesign_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTSR = aTRptSR.GetSize();
        for (int i = 0; i<nTSR; ++i)
        {
            BOOL bFinal = (i==(nTSR-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_CUMS_BASE> TRptD = aTRptSR.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_ShearDesign_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTTR = aTRptTR.GetSize();
        for (int i = 0; i<nTTR; ++i)
        {
            BOOL bFinal = (i==(nTTR-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_CRMT_BASE> TRptD = aTRptTR.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_TorsionDesign_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTSC = aTRptSC.GetSize();
        for (int i = 0; i<nTSC; ++i)
        {
            BOOL bFinal = (i==(nTSC-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FYSG_BASE> TRptD = aTRptSC.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_AllowableStresses_Conc_ConstructionStage_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTSS = aTRptSS.GetSize();
        for (int i = 0; i<nTSS; ++i)
        {
            BOOL bFinal = (i==(nTSS-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FYCM_BASE> TRptD = aTRptSS.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_AllowableStresses_Conc_ServiceLoad_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTPC = aTRptPC.GetSize();
        for (int i = 0; i<nTPC; ++i)
        {
            BOOL bFinal = (i==(nTPC-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FPSG_BASE> TRptD = aTRptPC.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_AllowablePrincipalStress_Conc_ConstructionStage_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

        INT_PTR nTPS = aTRptPS.GetSize();
        for (int i = 0; i<nTPS; ++i)
        {
            BOOL bFinal = (i==(nTPS-1)) ? TRUE : FALSE;
            _T_RPT_TABLE<PSC_FPMS_BASE> TRptD = aTRptPS.GetAt(i);
			ElemPairK RptEPairK(TRptD.RptData.iElemK, EN_EL_BEAM);
            if (!myEurocodePscReport.Print_AllowablePrincipalStress_Conc_ServiceLoad_EC(iDgnCode, RptEPairK, TRptD.iIJ, bFinal, TRptD.RptData)) { throw FALSE; }
        }
        nProgressPercent += nProgressIncrementPercent;
        Progress(0, nProgressPercent);
        Progress(2, nProgressPercent);
        if (m_bStopExecute)	return FALSE;

		bReport = m_PscdD.bServiceabilityEuro[4];
		if(bReport)
		{
			CArray<T_TNDN_K,T_TNDN_K> arTndnList;  arTndnList.RemoveAll();
			m_pDoc->m_pPostCtrl->GetPCDesign()->GetPscCheckTdnaList(arTndnList);

			int iTndnKey  = 0;
			int iTndnList = 0;
			iTndnList = arTndnList.GetSize();

			for(i=0; i<iTndnList; ++i)
			{
				iTndnKey  = 0;
				iTndnKey  = arTndnList.GetAt(i);

				T_FYPC_IRS FypcD;    FypcD.Initialize();
				PSC_FYPC_D    dgnFypcD; dgnFypcD.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetPCDesign()->ReadFypc_IRS(iTndnKey, FypcD)) continue;
				m_DataCtrl.m_pPscDataCtrl->Get_FypcReportData(iTndnKey, FypcD, dgnFypcD);

				bFinal = (i==iTndnList-1) ? TRUE : FALSE;
				if(!myEurocodePscReport.Print_AllowableStress_Tendon_EC(iDgnCode, iTndnKey, bFinal, dgnFypcD))	{}
			}
			//
			if(m_bStopExecute)	return FALSE;
			nProgressPercent = 96;
			Progress(0, nProgressPercent);
			Progress(2, nProgressPercent);
		}
	}
	catch(...)
	{
		bSuccess = FALSE;	
	}

	VERIFY(myEurocodePscReport.Terminate(iDgnCode, !bSuccess));

	if(m_bStopExecute)	return FALSE;
	nProgressPercent = 100;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	return bSuccess;
}

bool CCRCExcelOutput::IsFirstRow(const int& nRow)
{
    return nRow==1;
}