#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "..\DgnEngine\Src\DGNe_RUS\DgnCalc_Rus_PSC_Struct.h"
#include "ServiceAgencyDgn.h"
#include "Dgn_ServiceDef.h"
#include "DgnDataCtrl.h"
#include "CRCExportFile.h"
#include "CRCImportFile.h"
#include "CRCExcelOutput.h"
#include "DgnXLProgressDlg.h"
#include "DgnGenSeisFactor.h"
#include "DgnGenSeisFactor_IS18168.h"
#include "CRCDataCtrl.h"
#include "WriteLoadRatingResult.h"
#include "RptSteelGirder.h"
#include "RptKRRating.h"
#include "DgnSaveReportoExcelWithEMFDlg.h"
#include "RptOrthotropicDeck.h"
#include "BrdgSeisEvalKEC2012.h"
#include "BrdgSeisEvalKALIS2023.h"
#include "BrdgSeisEvalKISTEC2019.h"
#include "BrdgSeisEvalKISTEC2015.h"
#include "BrdgSeisEvalKISTEC2015_Report.h"
#include "BrdgSeisEvalKALIS2023_Report.h"
#include "DgnSaveReportoExcelDlg.h"
#include "SeisEvalTextOutMgr.h"
#include "BrdgSeisEval_JP.h"
#include "BridgeSeisEvalReport_JP.h"

#include "RCImportFile_CH.h"
#include "RCrailway.h"
#include "PSCrailway.h"
#include "DgnJTG_D62_04_Seismic.h"
#include "DgnPSCSelectPrintUintDlg.h"
#include "DgnSelectPrintLanguageDlg.h"

#include "..\wg_base\wg_base_MsgDll.h"	// for _ULS.
#include "..\wg_base\MSOfficeMgr.h"
#include "..\wg_db\wg_db_DbToolPmcv.h"
#include "..\wg_base\ModelessDlgCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\IRegressionInterface.h"
#include "..\wg_db\BrdgSeisEvalResult.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

#include "..\dgnengine\idesign\DGN_link\IDgnDocCtrl.h"

#include "DgnSeisReportDlg.h"
#include "Dgn_SeismicManager.h"
#include "DgnMemoizationTower.h"
#include "DgnCodeCtrl.h"
#include "DgnExcelPrintDlg.h"
#include "..\wg_dbLock\LockMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;


CServiceAgencyDgn::CServiceAgencyDgn()
{
}

CServiceAgencyDgn::~CServiceAgencyDgn()
{
}

int CServiceAgencyDgn::DoService(const int nServiceNo, void* pData)
{
	//Finalize 하는 위치를 찾아야하는데 ㅠㅠ
	CDgnMemoizationTower::GetInstance()->InitializeCheck();
	if ( dgn::link::IDgnDocCtrl::Exist() )
	{
		dgn::link::IDgnDocCtrl::Instance()->OnChangeLocale(TLocale(enLanguage::eng));
	}

	switch ( nServiceNo )
	{
	case EN_DGNSA_GEN_KFAC:
		{
			GetGenKfac(pData);
			return 0;
		}
		break;
	case EN_DGNSA_GEN_LENG:
		{
			GetGenLeng(pData);
			return 0;
		}
		break;
	case EN_DGNSA_CRC_EXPORT:
		{
			return ExportCRC();
		}
		break;
	case EN_DGNSA_CRC_IMPORT:
		{
			return ImportCRC(pData);
		}
		break;
	case EN_DGNSA_CRC_XPRINT:
		{
			return PrintExcelCRC(pData);
		}
		break;
	case EN_DGNSA_PSC_RT_PRINT:
		{
			return PrintRT4PSC(pData);
		}
		break;
	case EN_DGNSA_PSC_DESIGN_KSCE:
		{
			return ExecDesignPSC_KSCE(pData);
		}
		break;
	case EN_DGNSA_PSC_DESIGN_JTG:
		{
			return ExecDesignPSC_JTG(pData);
		}
		break;
	case EN_DGNSA_BRIDGE_LOAD_RATING:
		{
			return ExecLoadRating(pData);
		}
		break;
	case EN_DGNSA_RATING_PRINT_XL:
		{
			return PrintExcelLoadRating(pData);
		}
		break;
	case EN_DGNSA_PLATE_GIRDER:
		{
			return MakePlateGiderDesignData(pData);
		}
		break;
	case EN_DGNSA_PLATE_GIRDER_PRINT:
		{
			return PrintPlateGiderDesignData(pData);
		}
		break;
	case EN_DGNSA_PLATE_GIRDER_PRINT_XL:
		{
			return PrintExcelPlateGider(pData);
		}
		break;
	case EN_DGNSA_PLATE_GIRDER_RT_PRINT_XL:
		{
			return PrintExcelPlateGiderRT(pData);
		}
		break;
	case EN_DGNSA_OHTOTROPIC_DECK_DESIGN:
		{
			return MakeOrthotropicDeckDesign(pData);
		}
		break;
	case EN_DGNSA_OHTOTROPIC_DECK_PRINT:
		{
			return PrintOrthotropicDeckDesign(pData);
		}
		break;
	case EN_DGNSA_STL_COMP_GIRDER_DESIGN:
		{
			return ExecuteRUSStlCompDesign(pData);
		}
		break;
	case EN_DGNSA_STL_COMP_GIRDER_PRINT:
		{
			return ExecuteRUSStlCompPrintReport(pData);
		}
		break;
	case EN_DGNSA_CHRC_BEAM_DESIGN_JTG: // [31/10/2006 : maxiao] For CIVIL(CH) 2006                                        
		{
			return RCBeamDesign_JTG(pData);
		}
		break;
	case EN_DGNSA_CHRC_COLUMN_DESIGN_JTG: // [01/11/2006 : maxiao] For CIVIL(CH) 2006
		{
			return RCColmDesign_JTG(pData);
		}
		break;
	case EN_DGNSA_CHRAILRC_BEAM_DESIGN_JTG: // [21/02/2007 : maxiao] For CIVIL(CH Railway Design) 2007
		{
			return RCBeamDesign_JTG_Railway(pData);
		}
		break;
	case EN_DGNSA_CHRAILRC_COLUMN_DESIGN_JTG:
		{
			return RCColmDesign_JTG_Railway(pData);
		}
		break;
	case EN_DGNSA_PSC_BEAM_DESIGN_TB: //railway TB 10002.3-05	
		{
			return PSCBeamDesign_TB_Railway(pData);
		}
		break;
	case EN_DGNSA_PSC_COLUMN_DESIGN_TB:
		{
			return PSCColmDesign_TB_Railway(pData);
		}
		break;
	case EN_DGNSA_GEN_SEIS_FACTOR:
		{
			return GetGenSeisFactor(pData);
		}
		break;
	case EN_DGNSA_PSC_TENDON_STRESS:
		{
			return GetPSCTendonStress(pData);
		}
		break;
	case EN_DGNSA_MATD_DATA:
		{
			return EN_DGNSA_MATD_DATA;
		}
		break;
	case EN_DGNSA_PSC_DESIGN_AASHTO:
		{
			return PSCDesignAASHTO(pData);
		}
		break;
	case EN_DGNSA_PSC_CHECK_ELEMENT_LIST:
		{
			return GetPSCCheckElementList(pData);
		}
		break;
	case EN_DGNSA_CH_SEISMIC_DESIGN://seismic design...
		{
			return SeismicDesign_JTG_D62_04(pData);
		}
		break;
	case EN_DGNSA_GET_DSTL:
		{
			return GetDstl(pData);
		}
		break;
	case EN_DGNSA_GET_DCON:
		{
			return GetDcon(pData);
		}
		break;
	case EN_DGNSA_GET_PSCD:
		{
			return GetPscd(pData);
		}
		break;
	case EN_DGNSA_GET_DSRC:
		{
			return GetDsrc(pData);
		}
		break;
	case EN_DGNSA_BRDG_SEIS_EVAL:
		{
			return ExecBridgeSeismicEval(pData);
		}
		break;
	case EN_DGNSA_BRDG_SEIS_EVAL_PRINT:
		{
			return PrintBridgeSeismicEval(pData);
		}
		break;
	case EN_DGNSA_BRDG_SEIS_EVAL_BEARING:
		{
			return ExecBridgeSeismicEvalBearing(pData);
		}
		break;
	case EN_DGNSA_SEIS_EVAL_REPORT_1STCON:
		{
			return PrintSeisEval1stCon(pData);
		}
		break;
	case EN_DGNSA_SEIS_EVAL_REPORT_1STSTL:
		{
			return PrintSeisEval1stStl(pData);
		}
		break;
	case EN_DGNSA_SEIS_EVAL_REPORT_1STMAS:
		{
			return PrintSeisEval1stMas(pData);
		}
		break;
	case EN_DGNSA_SEIS_EVAL_REPORT_UPHGCON:
		{
			return PrintSeisEvalUphgCon(pData);
		}
		break;
	case EN_DGNSA_SEIS_EVAL_REPORT_UPHGSTL:
		{
			return  PrintSeisEvalUphgStl(pData);
		}
		break;
	case EN_DGNSA_SEIS_EVAL_REPORT_UPHGMAS:
		{
			return PrintSeisEvalUphgMas(pData);
		}
		break;
	case EN_DGNSA_CLEAR_MPHI_RESULT_ALL:
		{
			return ClearMPhiResultAll();
		}
		break;
	case EN_DGNSA_CLEAR_MPHI_RESULT_FRAM:
		{
			return ClearMPhiResultMemb(pData, false);
		}
		break;
	case EN_DGNSA_CLEAR_MPHI_RESULT_WALL:
		{
			return ClearMPhiResultMemb(pData, true);
		}
		break;
	case EN_DGNSA_DELETE_SEISMIC_MANAGER:
		{
			CDgn_SeismicDataPool* pInst = CDgn_SeismicDataPool::Instance();
			delete pInst;
			return 0;
		}
		break;
	case EN_DGNSA_DELETE_MEMOIZATION_TOWER:
		{
			CDgnMemoizationTower* pInst = CDgnMemoizationTower::GetInstance();
			pInst->End();
			return 0;
		}
		break;
	case EN_DGNSA_BRDG_SEIS_EVAL_JP:
		{
			return ExecBridgeSeismicEval_JP(pData);
		}
		break;
	case EN_DGNSA_BRDG_SEIS_EVAL_PRINT_JP:
	{
		return PrintBridgeSeismicEval_JP(pData);
	}
	break;
	}
	ASSERT(0); return D_SP_ERR_IRG_SERVICE_NO;
}

void CServiceAgencyDgn::GetGenKfac(void* pData)
{
	CDgnDataCtrl DataCtrl;

	int iElem = ((_DGN_AGEN_DATA*)pData)->aiIn[0];
	int iPosi = ((_DGN_AGEN_DATA*)pData)->aiIn[1];

	T_KFAC_D KfacD; KfacD.Initialize();
	T_KFAC_D aData; aData.Initialize();
	if(!DataCtrl.Get_DgnGenKfac(iElem, KfacD, aData))	{ ASSERT(FALSE); }
	((_DGN_AGEN_DATA*)pData)->adOut[0] = KfacD.BKy;
	((_DGN_AGEN_DATA*)pData)->adOut[1] = KfacD.BKz;
}

void CServiceAgencyDgn::GetGenLeng(void* pData)
{
	CDgnDataCtrl DataCtrl;

	int iElem = ((_DGN_AGEN_DATA*)pData)->aiIn[0];
	int iPosi = ((_DGN_AGEN_DATA*)pData)->aiIn[1];

	T_LENG_D LengD; LengD.Initialize();
	double dLen=0.0;
	if(!DataCtrl.Get_DgnGenLeng(iElem, iPosi, LengD, dLen))	{ ASSERT(FALSE); }
	((_DGN_AGEN_DATA*)pData)->adOut[0] = LengD.BLy;
	((_DGN_AGEN_DATA*)pData)->adOut[1] = LengD.BLz;
}

int CServiceAgencyDgn::ExportCRC()
{
	CCRCExportFile* pExpFile = new CCRCExportFile(); 
	if(!pExpFile->Export_File()) 
	{
		if(pExpFile != nullptr){delete pExpFile;}
		return -1;
	}
	// Add by Jaeoh.(06.03.07)
	//ExpFile.Get_CrcDataCtrl(CrcDataCtrl);
	//((_CRC_IMP_DATA*)pData)->DataCtrl = &CrcDataCtrl;
	//if(!pCrcDataCtrl) return -1;

	if(pExpFile != nullptr){delete pExpFile;}
	return 1;
}

int CServiceAgencyDgn::ImportCRC(void* pData)
{
	//CrcDataCtrl = *((_CRC_IMP_DATA*)pData)->DataCtrl;
	CCRCImportFile* pImpFile = new CCRCImportFile();
	if(!pImpFile->Export_File()) 
	{
		if(pImpFile != nullptr){delete pImpFile;}
		return -2;
	}
	
	CDgnDataCtrl DataCtrl;
	T_PSCD_D PscdD; PscdD.Initialize();
	DataCtrl.Get_DgnPscPscd(PscdD);

	((_CRC_IMP_DATA*)pData)->iTotLineNum =pImpFile->m_iTotLineNum;
	((_CRC_IMP_DATA*)pData)->FysgKList.Copy(pImpFile->m_FysgKList);
	((_CRC_IMP_DATA*)pData)->FypcKList.Copy(pImpFile->m_FypcKList);
	((_CRC_IMP_DATA*)pData)->FycmKList.Copy(pImpFile->m_FycmKList);
	((_CRC_IMP_DATA*)pData)->BdcwKList.Copy(pImpFile->m_BdcwKList);
	((_CRC_IMP_DATA*)pData)->TbarKList.Copy(pImpFile->m_TbarKList);
	((_CRC_IMP_DATA*)pData)->BdcrKList.Copy(pImpFile->m_BdcrKList);
	((_CRC_IMP_DATA*)pData)->FssgKList.Copy(pImpFile->m_FssgKList); 
	((_CRC_IMP_DATA*)pData)->FsmsKList.Copy(pImpFile->m_FsmsKList); 
	((_CRC_IMP_DATA*)pData)->CwmsKList.Copy(pImpFile->m_CwmsKList); 
	((_CRC_IMP_DATA*)pData)->CumsKList.Copy(pImpFile->m_CumsKList); 
	((_CRC_IMP_DATA*)pData)->CrmsKList.Copy(pImpFile->m_CrmsKList);
	((_CRC_IMP_DATA*)pData)->FsmtKList.Copy(pImpFile->m_FsmtKList); 
	((_CRC_IMP_DATA*)pData)->CwmtKList.Copy(pImpFile->m_CwmtKList); 
	((_CRC_IMP_DATA*)pData)->CumtKList.Copy(pImpFile->m_CumtKList); 
	((_CRC_IMP_DATA*)pData)->CrmtKList.Copy(pImpFile->m_CrmtKList);
	((_CRC_IMP_DATA*)pData)->FassCmpwKList.Copy(pImpFile->m_FassCmpwKList);
	((_CRC_IMP_DATA*)pData)->FmssCmpwKList.Copy(pImpFile->m_FmssCmpwKList);
	((_CRC_IMP_DATA*)pData)->FprtCmpwKList.Copy(pImpFile->m_FprtCmpwKList);
	((_CRC_IMP_DATA*)pData)->FwhlCmpwKList.Copy(pImpFile->m_FwhlCmpwKList);
	((_CRC_IMP_DATA*)pData)->FypcBaseKList.Copy(pImpFile->m_FypcBaseKList);

	UINT DgnLcomK;
	UINT OrgLcomK;
	POSITION Pos = pImpFile->m_DgnLcomKMap.GetStartPosition();
	while(Pos)
	{
		OrgLcomK = 0;
		pImpFile->m_DgnLcomKMap.GetNextAssoc(Pos, DgnLcomK, OrgLcomK);
		((_CRC_IMP_DATA*)pData)->DgnLcomKMap.SetAt(DgnLcomK, OrgLcomK);
	} 

	UINT DgnTdgrK=0;
	Pos = pImpFile->m_DgnTdgrMap.GetStartPosition();
	while(Pos)
	{
		UINT TdgrK = 0;
		pImpFile->m_DgnTdgrMap.GetNextAssoc(Pos, DgnTdgrK, TdgrK);
		((_CRC_IMP_DATA*)pData)->DgnTdgrMap.SetAt(DgnTdgrK, TdgrK);
	} 

	Pos = pImpFile->m_DgnTdnaMap.GetStartPosition();
	while(Pos)
	{
		UINT TdnaK = 0;
		pImpFile->m_DgnTdnaMap.GetNextAssoc(Pos, DgnTdgrK, TdnaK);
		((_CRC_IMP_DATA*)pData)->DgnTdnaMap.SetAt(DgnTdgrK, TdnaK);
	} 

	_CRC_COMF_K ComfK;
	_CRC_COMF_D ComfD;
	Pos = pImpFile->m_ComfDMap.GetStartPosition();
	while(Pos)
	{
		ComfD.Initialize();
		pImpFile->m_ComfDMap.GetNextAssoc(Pos, ComfK, ComfD);
		((_CRC_IMP_DATA*)pData)->ComfDMap.SetAt(ComfK, ComfD);
	} 

	if(pImpFile != nullptr){delete pImpFile;}
	return 2;
}

int CServiceAgencyDgn::PrintExcelCRC(void* pData)
{
	// Add, Jaeoh. [5/12/2010] AASHTO-LRFD07일 경우 출력 단위계 선택(US,SI 둘중 선택)
	int iSelUnit=0;

	CDgnDataCtrl DataCtrl;
	T_PSCD_D PscdD; PscdD.Initialize();
	DataCtrl.Get_DgnPscPscd(PscdD);
	if ( !CLockMgr::Get_IsRegressionTest() )
	{
		if ( !dgn::link::IDgnDocCtrl::Instance()->OnChangeLocale(CDgnCodeCtrl::GetPscLocalePref()) )
		{
			ASSERT(0); return 0;
		}
	}

#if defined(_US)
	if ( PscdD.iDgnCode == EUROCODE2_2_05_PSC )
	{
		if ( CDgnCodeCtrl::GetPscLocalePref().GetLanguage() == enLanguage::pol )
		{
			if ( !CDBDoc::GetDocPoint()->AllowCommand(D_OPTN_ID_RUS_RCPSC_DGN) )
			{
				AfxMessageBox(_LS(IDS_MAIN_NO_LICENSE_RUS_RCPSC_DGN), MB_OK);
				return 0;
			}
		}

		if ( CDgnCodeCtrl::GetPscLocalePref().GetLanguage() == enLanguage::cze )
		{
			if ( !CDBDoc::GetDocPoint()->AllowCommand(D_OPTN_ID_AASHTO_STL_COMP) )
			{
				AfxMessageBox(_LS(IDS_MAIN_NO_LICENSE_AASHTO_STL_COMP), MB_OK);
				return 0;
			}
		}
	}
	if ( CDBLib::IsPscCodeAASHTO(PscdD.iDgnCode) )
	{
		if ( CDgnCodeCtrl::GetPscLocalePref().GetLanguage() == enLanguage::tur )
		{
			if ( !CDBDoc::GetDocPoint()->AllowCommand(D_OPTN_ID_AASHTO_STL_COMP) )
			{
				AfxMessageBox(_LS(IDS_MAIN_NO_LICENSE_AASHTO_STL_COMP), MB_OK);
				return 0;
			}
		}
	}
#endif

	if ( CDBLib::IsPscCodeAASHTO(PscdD.iDgnCode) )  // CSA는 그냥 SI단위 출력?   
	{
		CDBLib::SetReportSelUnit(CDgnCodeCtrl::GetPscReportUnit() == EN_PREF_REPORT_UNIT::EN_REPORT_UNIT_SI ? CDBLib::EN_REPORT_UNIT_SI : CDBLib::EN_REPORT_UNIT_US);
	}
	BOOL bIsValidCode = PscdD.iDgnCode == BS5400_90_PSC || PscdD.iDgnCode == TMH07_3_1989 ? TRUE : FALSE;
	if ( !bIsValidCode)
	{
		CDgnXLProgressDlg* pDlg = (CDgnXLProgressDlg*)(CDBDoc::GetDocPoint()->m_pDlgCtrl->GetDlg(IDD_DGN_PROGRESS_XL_DLG));

		if ( pDlg == 0 ) // Dialog가 없으면 만든다.
		{
			CWnd* pWnd = AfxGetApp()->GetMainWnd();
			pDlg = new CDgnXLProgressDlg(pWnd);

			CRect rtParent, rtDlg;
			pWnd->GetWindowRect(rtParent);
			int iLeft = (rtParent.right)*0.5;
			int iTop =  (rtParent.bottom)*0.5;
			pDlg->SetInitPos(D_INIT_POS_LT, iLeft, iTop);
			pDlg->Create(pWnd);
		}

		if ( pDlg )
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->Execute();
		}
		else ASSERT(0);
	}
	else
	{
		CCRCExcelOutput ExcelOut;
		ExcelOut.Start();
	}

	return EN_DGNSA_CRC_XPRINT;
}

int CServiceAgencyDgn::PrintRT4PSC(void* pData)
{
	CCRCExcelOutput* pOutput = new CCRCExcelOutput();
	pOutput->Start();

	if(pOutput!=nullptr){delete pOutput;}
	return EN_DGNSA_PSC_RT_PRINT;
}

int CServiceAgencyDgn::ExecDesignPSC_KSCE(void* pData)
{
	CCRCImportFile* pImpFile = new CCRCImportFile();

	if(!pImpFile->Export_File()) 
	{
		if(pImpFile != nullptr){delete pImpFile;}
		return -2;
	}
	// Change by ZINU.('04.08.21). For PSC Design (KSCE-USD03).

	((_PSC_KSCE_DATA*)pData)->iTotLineNum =pImpFile->m_iTotLineNum;
	((_PSC_KSCE_DATA*)pData)->FysgKList.Copy(pImpFile->m_FysgKList);
	((_PSC_KSCE_DATA*)pData)->FypcKsceKList.Copy(pImpFile->m_FypcKsceKList);
	((_PSC_KSCE_DATA*)pData)->FycmKList.Copy(pImpFile->m_FycmKList);
	((_PSC_KSCE_DATA*)pData)->BdcwKList.Copy(pImpFile->m_BdcwKList);
	((_PSC_KSCE_DATA*)pData)->TbarKList.Copy(pImpFile->m_TbarKList);
	((_PSC_KSCE_DATA*)pData)->FssgKList.Copy(pImpFile->m_FssgKList); 
	((_PSC_KSCE_DATA*)pData)->FsmsKList.Copy(pImpFile->m_FsmsKList); 
	((_PSC_KSCE_DATA*)pData)->FsmtKList.Copy(pImpFile->m_FsmtKList); 
	((_PSC_KSCE_DATA*)pData)->BdcyKsceKList.Copy(pImpFile->m_BdcyKsceKList);
	((_PSC_KSCE_DATA*)pData)->BdczKsceKList.Copy(pImpFile->m_BdczKsceKList);
	((_PSC_KSCE_DATA*)pData)->CumsKsceKList.Copy(pImpFile->m_CumsKsceKList); 
	((_PSC_KSCE_DATA*)pData)->CrmtKsceKList.Copy(pImpFile->m_CrmtKsceKList);
	((_PSC_KSCE_DATA*)pData)->TndnKsceKList.Copy(pImpFile->m_TndnKsceKList);
	((_PSC_KSCE_DATA*)pData)->FpmsJtgKList.Copy(pImpFile->m_FpmsJtgKList);
	((_PSC_KSCE_DATA*)pData)->FpmtJtgKList.Copy(pImpFile->m_FpmtJtgKList);

	UINT DgnLcomK;
	UINT OrgLcomK;
	POSITION Pos = pImpFile->m_DgnLcomKMap.GetStartPosition();
	while(Pos)
	{
		OrgLcomK = 0;
		pImpFile->m_DgnLcomKMap.GetNextAssoc(Pos, DgnLcomK, OrgLcomK);
		((_PSC_KSCE_DATA*)pData)->DgnLcomKMap.SetAt(DgnLcomK, OrgLcomK);
	} 

	_CRC_COMF_K ComfK;
	_CRC_COMF_D ComfD;
	Pos = pImpFile->m_ComfDMap.GetStartPosition();
	while(Pos)
	{
		ComfD.Initialize();
		pImpFile->m_ComfDMap.GetNextAssoc(Pos, ComfK, ComfD);
		((_PSC_KSCE_DATA*)pData)->ComfDMap.SetAt(ComfK, ComfD);
	} 

	int nSerialK=0;
	Pos = pImpFile->m_mComfKey.GetStartPosition();
	while ( Pos )
	{
		ComfK.Initialize();
		pImpFile->m_mComfKey.GetNextAssoc(Pos, nSerialK, ComfK);
		((_PSC_KSCE_DATA*)pData)->mComfKey.SetAt(nSerialK, ComfK);
	}

	if(pImpFile != nullptr){delete pImpFile;}
	return 3;
}

int CServiceAgencyDgn::ExecDesignPSC_JTG(void* pData)
{
	CCRCImportFile* pImpFile = new CCRCImportFile();
	if(!pImpFile->Export_File())
	{
		if(pImpFile != nullptr){delete pImpFile;}
		return -2;
	}

	((_PSC_JTG_DATA*)pData)->iTotLineNum =pImpFile->m_iTotLineNum;
	((_PSC_JTG_DATA*)pData)->FysgKList.Copy(pImpFile->m_FysgKList);
	((_PSC_JTG_DATA*)pData)->FycmJtgKList.Copy(pImpFile->m_FycmJtgKList);
	((_PSC_JTG_DATA*)pData)->BdcwKList.Copy(pImpFile->m_BdcwKList);
	((_PSC_JTG_DATA*)pData)->FypcJtgKList.Copy(pImpFile->m_FypcJtgKList);
	((_PSC_JTG_DATA*)pData)->FpmsJtgKList.Copy(pImpFile->m_FpmsJtgKList);
	((_PSC_JTG_DATA*)pData)->FpmtJtgKList.Copy(pImpFile->m_FpmtJtgKList);
	((_PSC_JTG_DATA*)pData)->TbarJtgKList.Copy(pImpFile->m_TbarJtgKList);
	((_PSC_JTG_DATA*)pData)->TndnJtgKList.Copy(pImpFile->m_TndnJtgKList);
	((_PSC_JTG_DATA*)pData)->BdcyJtgKList.Copy(pImpFile->m_BdcyJtgKList);
	((_PSC_JTG_DATA*)pData)->CumsJtgKList.Copy(pImpFile->m_CumsJtgKList); 
	((_PSC_JTG_DATA*)pData)->CrmtJtgKList.Copy(pImpFile->m_CrmtJtgKList);
	((_PSC_JTG_DATA*)pData)->CscsJtgKList.Copy(pImpFile->m_CscsJtgKList);

	UINT DgnLcomK;
	UINT OrgLcomK;
	POSITION Pos = pImpFile->m_DgnLcomKMap.GetStartPosition();
	while(Pos)
	{
		OrgLcomK = 0;
		pImpFile->m_DgnLcomKMap.GetNextAssoc(Pos, DgnLcomK, OrgLcomK);
		((_PSC_JTG_DATA*)pData)->DgnLcomKMap.SetAt(DgnLcomK, OrgLcomK);
	} 

	_CRC_COMF_K ComfK;
	_CRC_COMF_D ComfD;
	Pos = pImpFile->m_ComfDMap.GetStartPosition();
	while(Pos)
	{
		ComfD.Initialize();
		pImpFile->m_ComfDMap.GetNextAssoc(Pos, ComfK, ComfD);
		((_PSC_JTG_DATA*)pData)->ComfDMap.SetAt(ComfK, ComfD);
	} 

	if(pImpFile != nullptr){delete pImpFile;}
	return 4;
}

int CServiceAgencyDgn::ExecLoadRating(void* pData)
{
	CWriteLoadRatingResult* pLoadRating = new CWriteLoadRatingResult();
	if(!pLoadRating->Execute_BridgeLoadRaitng())
	{
		if(pLoadRating != nullptr){delete pLoadRating;}
		return -2;
	}

	((LOAD_RATE_DATA*)pData)->arBlrdElemKList.Copy(pLoadRating->m_arBlrdElemKList);
	((LOAD_RATE_DATA*)pData)->arBlrdMembKList.Copy(pLoadRating->m_arBlrdMembKList);
	((LOAD_RATE_DATA*)pData)->arTndnElemKList.Copy(pLoadRating->m_arTndnElemKList);
	((LOAD_RATE_DATA*)pData)->arServRatingCaseKList.Copy(pLoadRating->m_arServRatingCaseKList);
	((LOAD_RATE_DATA*)pData)->arStreRatingCaseKList.Copy(pLoadRating->m_arStreRatingCaseKList);      

	if(pLoadRating != nullptr){delete pLoadRating;}
	return 5;
}

int CServiceAgencyDgn::PrintExcelLoadRating(void* pData)
{
	CString sFileName = _T("");
	int nWriteType  = 0;
	BOOL bSaveImage = FALSE; 

	BOOL bIsRegressionTest = CDBDoc::GetDocPoint()->IsRegTestMode();

	if(bIsRegressionTest)
	{
		sFileName = CDBDoc::GetDocPoint()->GetProgramPath();
		CRptKRRating RptRating; 
		RptRating.Start(sFileName, nWriteType, bSaveImage);
	}
	else
	{
		CDgnSaveReportoExcelWithEMFDlg dlg;
		if(dlg.DoModal() == IDOK)
		{
			sFileName  = dlg.strFileFullName;
			nWriteType = dlg.m_nWriteType;
			bSaveImage = dlg.m_bSaveEMF;    

			CRptKRRating RptRating; 
			RptRating.Start(sFileName, nWriteType, bSaveImage);
		}	  
	}

	return EN_DGNSA_RATING_PRINT_XL;
}

int CServiceAgencyDgn::MakePlateGiderDesignData(void* pData)
{
	CCRCDataCtrl* pCRCDataCtrl = new CCRCDataCtrl();
	if ( !pCRCDataCtrl->MakePlateGiderDesignData() )
	{
		if ( pCRCDataCtrl != nullptr ) { delete pCRCDataCtrl; }
		return -2;
	}

	CArray<T_ELEM_K, T_ELEM_K> aPgdElemK;
	pCRCDataCtrl->GetElemKeyList4ElemPair(EN_EL_BEAM, pCRCDataCtrl->m_arPgdElemKList, aPgdElemK);
	((PLATE_GIRDER_DATA*)pData)->arPgdElemKList.Copy(aPgdElemK);
	/*
	((LOAD_RATE_DATA*)pData)->arTndnElemKList.Copy(LoadRating.m_arTndnElemKList);
	((LOAD_RATE_DATA*)pData)->arServRatingCaseKList.Copy(LoadRating.m_arServRatingCaseKList);
	((LOAD_RATE_DATA*)pData)->arStreRatingCaseKList.Copy(LoadRating.m_arStreRatingCaseKList);
	*/
	if ( pCRCDataCtrl != nullptr ) { delete pCRCDataCtrl; }
	return 6;
}

int CServiceAgencyDgn::PrintPlateGiderDesignData(void* pData)
{
	CCRCDataCtrl* pCRCDataCtrl = new CCRCDataCtrl();
	if ( !pCRCDataCtrl->Print_PlateGiderDesignData() )
	{
		if ( pCRCDataCtrl !=nullptr ) { delete pCRCDataCtrl; }
		return -2;
	}

	CArray<T_ELEM_K, T_ELEM_K> aPgdElemK;
	pCRCDataCtrl->GetElemKeyList4ElemPair(EN_EL_BEAM, pCRCDataCtrl->m_arPgdElemKList, aPgdElemK);
	((PLATE_GIRDER_DATA*)pData)->arPgdElemKList.Copy(aPgdElemK);
	/*
	((LOAD_RATE_DATA*)pData)->arTndnElemKList.Copy(LoadRating.m_arTndnElemKList);
	((LOAD_RATE_DATA*)pData)->arServRatingCaseKList.Copy(LoadRating.m_arServRatingCaseKList);
	((LOAD_RATE_DATA*)pData)->arStreRatingCaseKList.Copy(LoadRating.m_arStreRatingCaseKList);
	*/
	if ( pCRCDataCtrl !=nullptr ) { delete pCRCDataCtrl; }
	return 999;
}

int CServiceAgencyDgn::PrintExcelPlateGider(void* pData)
{
	T_CPGD_D CpgdD; CpgdD.Initialize();
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	CString sFileName = _T("");
	int nWriteType  = 0;
	BOOL bSaveImage = FALSE;

	if ( CpgdD.iDgnCode == EUROCODE4_2_04_CSG || CpgdD.iDgnCode == AS5100_6_17_CSG)
	{
		CString strPathName = CDBDoc::GetDocPoint()->GetPathName();
		CFileCtrl FileCtrl(strPathName);
		sFileName = FileCtrl.GetFilePathAndNameWithoutExtension();

		CRptSteelGirder RptCSG;
		RptCSG.Start(sFileName, nWriteType, bSaveImage);
	}
	else
	{
		CDgnSaveReportoExcelWithEMFDlg dlg;
		if ( dlg.DoModal() == IDOK )
		{
			sFileName  = dlg.strFileFullName;
			nWriteType = dlg.m_nWriteType;
			bSaveImage = dlg.m_bSaveEMF;

			CRptSteelGirder RptCSG;
			RptCSG.Start(sFileName, nWriteType, bSaveImage);
		}
	}

	return EN_DGNSA_PLATE_GIRDER_PRINT_XL;
}

int CServiceAgencyDgn::PrintExcelPlateGiderRT(void* pData)
{
	int nWriteType  = 0; // new~
	BOOL bSaveImage = FALSE;
	_REGRESSION_TEST_D* pInfoD = IRegressionInterface::Instance()->GetPtrRTInfo();
	CString strFileName = [this] (_REGRESSION_TEST_D* pInD)
	{
		if ( pInD != nullptr )
		{
			if ( pInD->arOutFile_Trg.GetSize() != 0 )
			{
				return pInD->arOutFile_Trg[1]; //이거 항상 xls 파일일꺼야...
			}
		}

		return (CString)CDBDoc::GetDocPoint()->GetPathName();
	}(pInfoD);
	CFileCtrl FileCtrl(strFileName);
	CString sFileName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".") + CMSOfficeMgr::GetExtensionNameExcel();

	CRptSteelGirder RptCSG;
	RptCSG.Start(sFileName, nWriteType, bSaveImage);

	return EN_DGNSA_PLATE_GIRDER_RT_PRINT_XL;
}

int CServiceAgencyDgn::MakeOrthotropicDeckDesign(void* pData)
{
	CCRCDataCtrl* pCRCDataCtrl = new CCRCDataCtrl();
	if ( !pCRCDataCtrl->MakeStlOrthDeckDesignData() )
	{
		if ( pCRCDataCtrl !=nullptr ) { delete pCRCDataCtrl; }
		return -2;
	}

	//((STL_ORTHDECK_DATA*)pData)->aSodElemKList.Copy(CRCDataCtrl.m_arPgdElemKList);

	if ( pCRCDataCtrl !=nullptr ) { delete pCRCDataCtrl; }
	return EN_DGNSA_OHTOTROPIC_DECK_DESIGN;
}

int CServiceAgencyDgn::PrintOrthotropicDeckDesign(void* pData)
{
	CString sFileName = _T("");
	int nWriteType  = 0;
	BOOL bSaveImage = FALSE;

	//       CDgnSaveReportoExcelWithEMFDlg dlg;
	//       if(dlg.DoModal() == IDOK)
	//       {
	//         sFileName  = dlg.strFileFullName;
	//         nWriteType = dlg.m_nWriteType;
	//         bSaveImage = dlg.m_bSaveEMF;    			
	//       }

	CRptOrthotropicDeck RptODD;
	RptODD.Start(sFileName, nWriteType, bSaveImage);

	return EN_DGNSA_OHTOTROPIC_DECK_PRINT;
}

int CServiceAgencyDgn::ExecuteRUSStlCompDesign(void* pData)
{
	CCRCDataCtrl* pCRCDataCtrl= new CCRCDataCtrl();
	if ( !pCRCDataCtrl->ExecuteRUSStlCompDesign() )
	{
		if ( pCRCDataCtrl !=nullptr ) { delete pCRCDataCtrl; }
		return -2;
	}

	if ( pCRCDataCtrl !=nullptr ) { delete pCRCDataCtrl; }
	return EN_DGNSA_STL_COMP_GIRDER_DESIGN;
}

int CServiceAgencyDgn::ExecuteRUSStlCompPrintReport(void* pData)
{
	T_CPGD_D CpgdD; CDBDoc::GetDocPoint()->m_pAttrCtrl->GetDgnCpgd(CpgdD);
	CDBDoc::GetDocPoint()->SetCivilCodeDgn(CpgdD.iDgnCode);
	CCRCDataCtrl* pCRCDataCtrl= new CCRCDataCtrl();
	if ( !pCRCDataCtrl->ExecuteRUSStlCompPrintReport() )
	{
		if ( pCRCDataCtrl !=nullptr ) { delete pCRCDataCtrl; }
		return -2;
	}

	if ( pCRCDataCtrl !=nullptr ) { delete pCRCDataCtrl; }
	return EN_DGNSA_STL_COMP_GIRDER_PRINT;
}

int CServiceAgencyDgn::RCBeamDesign_JTG(void* pData)
{
	CRCImportFile_CH* pImpFile = new CRCImportFile_CH();;
	if ( !pImpFile->RcDesign_CH(TRUE) )
	{
		if ( pImpFile !=nullptr ) { delete pImpFile; }
		return -2;
	}

	((_CHRC_JTG_DATA*)pData)->iTotLineNum =pImpFile->m_iTotLineNum;
	((_CHRC_JTG_DATA*)pData)->CbcsBeamKList.Copy(pImpFile->m_CbcsBeamKList);
	((_CHRC_JTG_DATA*)pData)->CsisBeamKList.Copy(pImpFile->m_CsisBeamKList);
	((_CHRC_JTG_DATA*)pData)->CtssBeamKList.Copy(pImpFile->m_CtssBeamKList);
	((_CHRC_JTG_DATA*)pData)->CcwsBeamKList.Copy(pImpFile->m_CcwsBeamKList);
	((_CHRC_JTG_DATA*)pData)->CsccBeamKList.Copy(pImpFile->m_CsccBeamKList);
	((_CHRC_JTG_DATA*)pData)->CtstBeamKList.Copy(pImpFile->m_CtstBeamKList);
	((_CHRC_JTG_DATA*)pData)->CsncBeamKList.Copy(pImpFile->m_CsncBeamKList);
	((_CHRC_JTG_DATA*)pData)->BrlrKList.Copy(pImpFile->m_BrlrKList);
	((_CHRC_JTG_DATA*)pData)->BrcrKList.Copy(pImpFile->m_BrcrKList);

	UINT DgnLcomK;
	UINT OrgLcomK;
	POSITION Pos = pImpFile->m_DgnLcomKMap.GetStartPosition();
	while ( Pos )
	{
		OrgLcomK = 0;
		pImpFile->m_DgnLcomKMap.GetNextAssoc(Pos, DgnLcomK, OrgLcomK);
		((_CHRC_JTG_DATA*)pData)->DgnLcomKMap.SetAt(DgnLcomK, OrgLcomK);
	}
	_CRC_COMF_K ComfK;
	_CRC_COMF_D ComfD;
	Pos = pImpFile->m_ComfDMap.GetStartPosition();
	while ( Pos )
	{
		ComfD.Initialize();
		pImpFile->m_ComfDMap.GetNextAssoc(Pos, ComfK, ComfD);
		((_CHRC_JTG_DATA*)pData)->ComfDMap.SetAt(ComfK, ComfD);
	}

	if ( pImpFile !=nullptr ) { delete pImpFile; }
	return 7;
}

int CServiceAgencyDgn::RCColmDesign_JTG(void* pData)
{
	CRCImportFile_CH* pImpFile = new CRCImportFile_CH();;
	if ( !pImpFile->RcDesign_CH(FALSE) )
	{
		if ( pImpFile !=nullptr ) { delete pImpFile; }
		return -2;
	}

	((_CHRC_JTG_DATA*)pData)->iTotLineNum =pImpFile->m_iTotLineNum;
	((_CHRC_JTG_DATA*)pData)->CcwsColumnKList.Copy(pImpFile->m_CcwsColumnKList);
	((_CHRC_JTG_DATA*)pData)->CtcsColumnKList.Copy(pImpFile->m_CtcsColumnKList);
	((_CHRC_JTG_DATA*)pData)->CscsColumnKList.Copy(pImpFile->m_CscsColumnKList);
	((_CHRC_JTG_DATA*)pData)->CrlrKList.Copy(pImpFile->m_CrlrKList);

	UINT DgnLcomK;
	UINT OrgLcomK;
	POSITION Pos = pImpFile->m_DgnLcomKMap.GetStartPosition();
	while ( Pos )
	{
		OrgLcomK = 0;
		pImpFile->m_DgnLcomKMap.GetNextAssoc(Pos, DgnLcomK, OrgLcomK);
		((_CHRC_JTG_DATA*)pData)->DgnLcomKMap.SetAt(DgnLcomK, OrgLcomK);
	}
	_CRC_COMF_K ComfK;
	_CRC_COMF_D ComfD;
	Pos = pImpFile->m_ComfDMap.GetStartPosition();
	while ( Pos )
	{
		ComfD.Initialize();
		pImpFile->m_ComfDMap.GetNextAssoc(Pos, ComfK, ComfD);
		((_CHRC_JTG_DATA*)pData)->ComfDMap.SetAt(ComfK, ComfD);
	}

	if ( pImpFile !=nullptr ) { delete pImpFile; }
	return 8;
}

int CServiceAgencyDgn::RCBeamDesign_JTG_Railway(void* pData)
{
	CRCrailway* prailway = new CRCrailway();
	if ( !prailway->RCrailway_CH(TRUE) )
	{
		if ( prailway != nullptr ) { delete prailway; }
		return -2;
	}

	((_CHRC_RAIL_DATA*)pData)->iTotLineNum =prailway->m_iTotLineNum;
	((_CHRC_RAIL_DATA*)pData)->BeamConKList.Copy(prailway->m_BeamConKList);
	((_CHRC_RAIL_DATA*)pData)->BeamShearKList.Copy(prailway->m_BeamShearKList);
	((_CHRC_RAIL_DATA*)pData)->BeamRebarKList.Copy(prailway->m_BeamRebarKList);
	((_CHRC_RAIL_DATA*)pData)->BeamSheartKList.Copy(prailway->m_BeamSheartKList);
	((_CHRC_RAIL_DATA*)pData)->BeamShearbKList.Copy(prailway->m_BeamShearbKList);
	((_CHRC_RAIL_DATA*)pData)->BeamCrackKList.Copy(prailway->m_BeamCrackKList);
	UINT DgnLcomK;   UINT OrgLcomK;
	POSITION Pos = prailway->m_DgnLcomKMap.GetStartPosition();
	while ( Pos )
	{
		OrgLcomK = 0;
		prailway->m_DgnLcomKMap.GetNextAssoc(Pos, DgnLcomK, OrgLcomK);
		((_CHRC_RAIL_DATA*)pData)->DgnLcomKMap.SetAt(DgnLcomK, OrgLcomK);
	}
	_CRC_COMF_K ComfK;
	_CRC_COMF_D ComfD;
	Pos = prailway->m_ComfDMap.GetStartPosition();
	while ( Pos )
	{
		ComfD.Initialize();
		prailway->m_ComfDMap.GetNextAssoc(Pos, ComfK, ComfD);
		((_CHRC_RAIL_DATA*)pData)->ComfDMap.SetAt(ComfK, ComfD);
	}

	if ( prailway != nullptr ) { delete prailway; }
	return 9;
}

int CServiceAgencyDgn::RCColmDesign_JTG_Railway(void* pData)
{
	CRCrailway* prailway = new CRCrailway();
	if ( !prailway->RCrailway_CH(FALSE) )
	{
		if ( prailway != nullptr ) { delete prailway; }
		return -2;
	}

	((_CHRC_RAIL_DATA*)pData)->iTotLineNum =prailway->m_iTotLineNum;
	((_CHRC_RAIL_DATA*)pData)->ColumnConKList.Copy(prailway->m_ColumnConKList);
	((_CHRC_RAIL_DATA*)pData)->ColumnShearKList.Copy(prailway->m_ColumnShearKList);
	((_CHRC_RAIL_DATA*)pData)->ColumnRebarcKList.Copy(prailway->m_ColumnRebarcKList);
	((_CHRC_RAIL_DATA*)pData)->ColumnRebarsKList.Copy(prailway->m_ColumnRebarsKList);
	((_CHRC_RAIL_DATA*)pData)->ColumnStabKList.Copy(prailway->m_ColumnStabKList);
	((_CHRC_RAIL_DATA*)pData)->ColumnCrackKList.Copy(prailway->m_ColumnCrackKList);
	((_CHRC_RAIL_DATA*)pData)->SeismicConKList.Copy(prailway->m_SeismicConKList);//seismic
	((_CHRC_RAIL_DATA*)pData)->SeismicRebarcKList.Copy(prailway->m_SeismicRebarcKList);
	((_CHRC_RAIL_DATA*)pData)->SeismicRebarsKList.Copy(prailway->m_SeismicRebarsKList);
	((_CHRC_RAIL_DATA*)pData)->SeismicEKList.Copy(prailway->m_SeismicEKList);
	((_CHRC_RAIL_DATA*)pData)->SeismicStabKList.Copy(prailway->m_SeismicStabKList);
	((_CHRC_RAIL_DATA*)pData)->SeismicDuctKList.Copy(prailway->m_SeismicDuctKList);

	UINT DgnLcomK;     UINT OrgLcomK;
	POSITION Pos = prailway->m_DgnLcomKMap.GetStartPosition();
	while ( Pos )
	{
		OrgLcomK = 0;
		prailway->m_DgnLcomKMap.GetNextAssoc(Pos, DgnLcomK, OrgLcomK);
		((_CHRC_RAIL_DATA*)pData)->DgnLcomKMap.SetAt(DgnLcomK, OrgLcomK);
	}
	_CRC_COMF_K ComfK;
	_CRC_COMF_D ComfD;
	Pos = prailway->m_ComfDMap.GetStartPosition();
	while ( Pos )
	{
		ComfD.Initialize();
		prailway->m_ComfDMap.GetNextAssoc(Pos, ComfK, ComfD);
		((_CHRC_RAIL_DATA*)pData)->ComfDMap.SetAt(ComfK, ComfD);
	}

	if ( prailway != nullptr ) { delete prailway; }
	return 10;
}

int CServiceAgencyDgn::PSCBeamDesign_TB_Railway(void* pData)
{
	CPSCrailway* pPscRailway = new CPSCrailway();
	if ( !pPscRailway->PSCrailway_CH(TRUE) )
	{
		if ( pPscRailway != nullptr ) { delete pPscRailway; }
		return -2;
	}

	((_PSC_TB_DATA*)pData)->iTotLineNum =pPscRailway->m_iTotLineNum;
	((_PSC_TB_DATA*)pData)->Beam_PSC_TBSeqKList.Copy(pPscRailway->m_Beam_PSC_KList);
	((_PSC_TB_DATA*)pData)->Beam_cross_bend_TBSeqKList.Copy(pPscRailway->m_Beam_cross_bend_KList);
	((_PSC_TB_DATA*)pData)->Beam_incline_shear_TBSeqKList.Copy(pPscRailway->m_Beam_incline_shear_KList);
	((_PSC_TB_DATA*)pData)->Beam_cross_fracture_TBSeqKList.Copy(pPscRailway->m_Beam_cross_fracture_KList);
	((_PSC_TB_DATA*)pData)->Beam_incline_fracture_TBSeqKList.Copy(pPscRailway->m_Beam_incline_fracture_KList);
	((_PSC_TB_DATA*)pData)->Beam_incline_tp_TBSeqKList.Copy(pPscRailway->m_Beam_incline_tp_KList);
	((_PSC_TB_DATA*)pData)->Beam_con_c_SS_TBSeqKList.Copy(pPscRailway->m_Beam_con_c_SS_KList);
	((_PSC_TB_DATA*)pData)->Beam_con_t_SS_TBSeqKList.Copy(pPscRailway->m_Beam_con_t_SS_KList);
	((_PSC_TB_DATA*)pData)->Beam_tendon_SS_TBSeqKList.Copy(pPscRailway->m_Beam_tendon_SS_KList);
	((_PSC_TB_DATA*)pData)->Beam_delta_tendon_TBSeqKList.Copy(pPscRailway->m_Beam_delta_tendon_KList);
	((_PSC_TB_DATA*)pData)->Beam_shear_TBSeqKList.Copy(pPscRailway->m_Beam_shear_KList);
	((_PSC_TB_DATA*)pData)->Beam_crack_TBSeqKList.Copy(pPscRailway->m_Beam_crack_KList);
	((_PSC_TB_DATA*)pData)->Beam_tendon_CS_TBSeqKList.Copy(pPscRailway->m_Beam_tendon_CS_KList);
	((_PSC_TB_DATA*)pData)->Beam_con_CS_TBSeqKList.Copy(pPscRailway->m_Beam_con_CS_KList);

	UINT DgnLcomK;
	UINT OrgLcomK;
	POSITION Pos = pPscRailway->m_DgnLcomKMap.GetStartPosition();
	while ( Pos )
	{
		OrgLcomK = 0;
		pPscRailway->m_DgnLcomKMap.GetNextAssoc(Pos, DgnLcomK, OrgLcomK);
		((_PSC_TB_DATA*)pData)->DgnLcomKMap.SetAt(DgnLcomK, OrgLcomK);
	}

	_CRC_COMF_K ComfK;
	_CRC_COMF_D ComfD;
	Pos = pPscRailway->m_ComfDMap.GetStartPosition();
	while ( Pos )
	{
		ComfD.Initialize();
		pPscRailway->m_ComfDMap.GetNextAssoc(Pos, ComfK, ComfD);
		((_PSC_TB_DATA*)pData)->ComfDMap.SetAt(ComfK, ComfD);
	}

	if ( pPscRailway != nullptr ) { delete pPscRailway; }
	return 11;
}

int CServiceAgencyDgn::PSCColmDesign_TB_Railway(void* pData)
{
	CPSCrailway* pPscRailway = new CPSCrailway();
	if ( !pPscRailway->PSCrailway_CH(FALSE) )
	{
		if ( pPscRailway != nullptr ) { delete pPscRailway; }
		return -2;
	}

	((_PSC_TB_DATA*)pData)->iTotLineNum =pPscRailway->m_iTotLineNum;
	((_PSC_TB_DATA*)pData)->Column_PSC_TBSeqKList.Copy(pPscRailway->m_Column_PSC_KList);
	((_PSC_TB_DATA*)pData)->Column_compressive_TBSeqKList.Copy(pPscRailway->m_Column_compressive_KList);
	((_PSC_TB_DATA*)pData)->Column_tension_TBSeqKList.Copy(pPscRailway->m_Column_tension_KList);
	((_PSC_TB_DATA*)pData)->Column_cross_fracture_TBSeqKList.Copy(pPscRailway->m_Column_cross_fracture_KList);
	((_PSC_TB_DATA*)pData)->Column_con_c_SS_TBSeqKList.Copy(pPscRailway->m_Column_con_c_SS_KList);
	((_PSC_TB_DATA*)pData)->Column_con_t_SS_TBSeqKList.Copy(pPscRailway->m_Column_con_t_SS_KList);
	((_PSC_TB_DATA*)pData)->Column_tendon_SS_TBSeqKList.Copy(pPscRailway->m_Column_tendon_SS_KList);
	((_PSC_TB_DATA*)pData)->Column_shear_TBSeqKList.Copy(pPscRailway->m_Column_shear_KList);
	((_PSC_TB_DATA*)pData)->Column_tendon_CS_TBSeqKList.Copy(pPscRailway->m_Column_tendon_CS_KList);
	((_PSC_TB_DATA*)pData)->Column_con_CS_TBSeqKList.Copy(pPscRailway->m_Column_con_CS_KList);

	UINT DgnLcomK;
	UINT OrgLcomK;
	POSITION Pos = pPscRailway->m_DgnLcomKMap.GetStartPosition();
	while ( Pos )
	{
		OrgLcomK = 0;
		pPscRailway->m_DgnLcomKMap.GetNextAssoc(Pos, DgnLcomK, OrgLcomK);
		((_PSC_TB_DATA*)pData)->DgnLcomKMap.SetAt(DgnLcomK, OrgLcomK);
	}

	_CRC_COMF_K ComfK;
	_CRC_COMF_D ComfD;
	Pos = pPscRailway->m_ComfDMap.GetStartPosition();
	while ( Pos )
	{
		ComfD.Initialize();
		pPscRailway->m_ComfDMap.GetNextAssoc(Pos, ComfK, ComfD);
		((_PSC_TB_DATA*)pData)->ComfDMap.SetAt(ComfK, ComfD);
	}

	if ( pPscRailway != nullptr ) { delete pPscRailway; }
	return 12;
}

int CServiceAgencyDgn::GetGenSeisFactor(void* pData)
{
	if (pData == NULL)
	{
		CDgnGenSeisFactor dlg;
		dlg.DoModal();
	}
	else
	{				
		auto pHideSpecialGroup = (_DGN_GEN_SEIS_FACTOR_DATA*)pData;
		if (pHideSpecialGroup->strConCode == _T("IS:800-2007"))
		{
			auto pHideSpecialGroup = (_DGN_GEN_SEIS_FACTOR_DATA*)pData;
			CDgnGenSeisFactor_IS18168 dlg_IS18168;
			dlg_IS18168.SetHideSpecialGroup(pHideSpecialGroup->bHideSpecialGroup);
			dlg_IS18168.SetConcCode(pHideSpecialGroup->strConCode);
			dlg_IS18168.DoModal();
		}
		else
		{
			auto pHideSpecialGroup = (_DGN_GEN_SEIS_FACTOR_DATA*)pData;
			CDgnGenSeisFactor dlg;
			dlg.SetHideSpecialGroup(pHideSpecialGroup->bHideSpecialGroup);
			dlg.SetDgnCode(pHideSpecialGroup->strConCode);
			dlg.DoModal();
		}
	}
	
	return 0;
}

int CServiceAgencyDgn::GetPSCTendonStress(void* pData)
{
	_TNDN_STRESS TndnStrD;
	CCRCDataCtrl* pCRCDataCtrl = new CCRCDataCtrl();
	if ( !pCRCDataCtrl->Get_TndnStress(TndnStrD) ) return D_SP_ERR_IRG_SERVICE_NO;

	T_TDNA_K TdnaK;
	T_FYPC_STRESS rData;
	POSITION Pos = TndnStrD.mapTndnStress.GetStartPosition();
	while ( Pos )
	{
		TdnaK = 0;
		rData.Initialize();
		TndnStrD.mapTndnStress.GetNextAssoc(Pos, TdnaK, rData);
		((_TNDN_STRESS*)pData)->mapTndnStress.SetAt(TdnaK, rData);
	}

	if ( pCRCDataCtrl != nullptr ) { delete pCRCDataCtrl; }
	return EN_DGNSA_PSC_TENDON_STRESS;
}

int CServiceAgencyDgn::PSCDesignAASHTO(void* pData)
{
	CCRCImportFile* pImpFile = new CCRCImportFile();

	if ( !pImpFile->Export_File() )
	{
		if ( pImpFile != nullptr ) { delete pImpFile; }
		return -2;
	}

	// Change by ZINU.('04.08.21). For PSC Design (KSCE-USD03).

	((_PSC_AASHTO_DATA*)pData)->iTotLineNum =pImpFile->m_iTotLineNum;

	UINT DgnLcomK;
	UINT OrgLcomK;
	POSITION Pos = pImpFile->m_DgnLcomKMap.GetStartPosition();
	while ( Pos )
	{
		OrgLcomK = 0;
		pImpFile->m_DgnLcomKMap.GetNextAssoc(Pos, DgnLcomK, OrgLcomK);
		((_PSC_AASHTO_DATA*)pData)->DgnLcomKMap.SetAt(DgnLcomK, OrgLcomK);
	}

	_CRC_COMF_K ComfK;
	_CRC_COMF_D ComfD;
	Pos = pImpFile->m_ComfDMap.GetStartPosition();
	while ( Pos )
	{
		ComfD.Initialize();
		pImpFile->m_ComfDMap.GetNextAssoc(Pos, ComfK, ComfD);
		((_PSC_AASHTO_DATA*)pData)->ComfDMap.SetAt(ComfK, ComfD);
	}

	int nSerialK=0;
	Pos = pImpFile->m_mComfKey.GetStartPosition();
	while ( Pos )
	{
		ComfK.Initialize();
		pImpFile->m_mComfKey.GetNextAssoc(Pos, nSerialK, ComfK);
		((_PSC_AASHTO_DATA*)pData)->mComfKey.SetAt(nSerialK, ComfK);
	}

	if ( pImpFile != nullptr ) { delete pImpFile; }
	return EN_DGNSA_PSC_DESIGN_AASHTO;
}

int CServiceAgencyDgn::GetPSCCheckElementList(void* pData)
{
	CCRCDataCtrl* pCRCDataCtrl = new CCRCDataCtrl();
	ArrElemPairKey arElemK;
	pCRCDataCtrl->Get_CrcElemListForDgn(FALSE, arElemK, TRUE);
	((ArrElemPairKey*)pData)->Copy(arElemK);

	if ( pCRCDataCtrl != nullptr ) { delete pCRCDataCtrl; }
	return EN_DGNSA_PSC_CHECK_ELEMENT_LIST;
}

int CServiceAgencyDgn::SeismicDesign_JTG_D62_04(void* pData)
{
	CDgnJTG_D62_04_Seismic* pSeismicDgn = new CDgnJTG_D62_04_Seismic();
	if ( !pSeismicDgn->Design() )
	{
		if ( pSeismicDgn != nullptr ) { delete pSeismicDgn; }
		return -2;
	}

	((_CHRC_JTGSEIS_DATA*)pData)->iTotLineNum =pSeismicDgn->m_iTotLineNum;
	((_CHRC_JTGSEIS_DATA*)pData)->E1Pier_Comp_KList.Copy(pSeismicDgn->m_E1Pier_Comp_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E1Arch_Cross_KList.Copy(pSeismicDgn->m_E1Arch_Cross_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E1Arch_Comp_KList.Copy(pSeismicDgn->m_E1Arch_Comp_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E1BentCap_Bend_KList.Copy(pSeismicDgn->m_E1BentCap_Bend_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E1BentCap_Shear_KList.Copy(pSeismicDgn->m_E1BentCap_Shear_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E1Foundation_KList.Copy(pSeismicDgn->m_E1Foundation_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E1RubberThick_KList.Copy(pSeismicDgn->m_E1RubberThick_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E1RubberSlip_KList.Copy(pSeismicDgn->m_E1RubberSlip_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E1ActiveHoriDisp_KList.Copy(pSeismicDgn->m_E1ActiveHoriDisp_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E1StatHoriForce_KList.Copy(pSeismicDgn->m_E1StatHoriForce_KList);
	//E2
	((_CHRC_JTGSEIS_DATA*)pData)->E2Pier_Comp_KList.Copy(pSeismicDgn->m_E2Pier_Comp_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E2Arch_Cross_KList.Copy(pSeismicDgn->m_E2Arch_Cross_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E2Arch_Comp_KList.Copy(pSeismicDgn->m_E2Arch_Comp_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E2BentCap_Bend_KList.Copy(pSeismicDgn->m_E2BentCap_Bend_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E2BentCap_Shear_KList.Copy(pSeismicDgn->m_E2BentCap_Shear_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E2Foundation_KList.Copy(pSeismicDgn->m_E2Foundation_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E2Pier_Shear_KList.Copy(pSeismicDgn->m_E2Pier_Shear_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E2RubberThick_KList.Copy(pSeismicDgn->m_E2RubberThick_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E2RubberSlip_KList.Copy(pSeismicDgn->m_E2RubberSlip_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E2ActiveHoriDisp_KList.Copy(pSeismicDgn->m_E2ActiveHoriDisp_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E2StatHoriForce_KList.Copy(pSeismicDgn->m_E2StatHoriForce_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E2FrustaTopDisp_KList.Copy(pSeismicDgn->m_E2FrustaTopDisp_KList);
	((_CHRC_JTGSEIS_DATA*)pData)->E2CornerDisp_KList.Copy(pSeismicDgn->m_E2CornerDisp_KList);

	UINT DgnLcomK;
	UINT OrgLcomK;
	POSITION Pos = pSeismicDgn->m_DgnLcomKMap.GetStartPosition();
	while ( Pos )
	{
		OrgLcomK = 0;
		pSeismicDgn->m_DgnLcomKMap.GetNextAssoc(Pos, DgnLcomK, OrgLcomK);
		((_CHRC_JTGSEIS_DATA*)pData)->DgnLcomKMap.SetAt(DgnLcomK, OrgLcomK);
	}
	_CRC_COMF_K ComfK;
	_CRC_COMF_D ComfD;
	Pos = pSeismicDgn->m_ComfDMap.GetStartPosition();
	while ( Pos )
	{
		ComfD.Initialize();
		pSeismicDgn->m_ComfDMap.GetNextAssoc(Pos, ComfK, ComfD);
		((_CHRC_JTGSEIS_DATA*)pData)->ComfDMap.SetAt(ComfK, ComfD);
	}

	if ( pSeismicDgn != nullptr ) { delete pSeismicDgn; }
	return EN_DGNSA_CH_SEISMIC_DESIGN;
}

int CServiceAgencyDgn::GetDstl(void* pData)
{
	CDgnDataCtrl DataCtrl;
	T_DSTL_D* pDstl = (T_DSTL_D*)pData;
	pDstl->Initialize();
	DataCtrl.Get_DgnStlDstl(*pDstl);

	return D_SP_ERR_IRG_SERVICE_NO;
}

int CServiceAgencyDgn::GetDcon(void* pData)
{
	CDgnDataCtrl DataCtrl;
	T_DCON_D* pDcon = (T_DCON_D*)pData;
	pDcon->Initialize();
	DataCtrl.Get_DgnConDcon(*pDcon);

	return D_SP_ERR_IRG_SERVICE_NO;
}

int CServiceAgencyDgn::GetPscd(void* pData)
{
	CDgnDataCtrl DataCtrl;
	T_PSCD_D* pPscd = (T_PSCD_D*)pData;
	pPscd->Initialize();
	DataCtrl.Get_DgnPscPscd(*pPscd);

	return D_SP_ERR_IRG_SERVICE_NO;
}

int CServiceAgencyDgn::GetDsrc(void* pData)
{
	CDgnDataCtrl DataCtrl;
	T_DSRC_D* pDsrc = (T_DSRC_D*)pData;
	pDsrc->Initialize();
	DataCtrl.Get_DgnSrcDsrc(*pDsrc);

	return D_SP_ERR_IRG_SERVICE_NO;
}

int CServiceAgencyDgn::ExecBridgeSeismicEval(void* pData)
{
	CBrdgSeisEvalResult* pResult = CDBDoc::GetDocPoint()->m_pPostCtrl->GetBrdgSeisEvalResult(); if ( !pResult ) ASSERT(0);

	BOOL bCalcPier = TRUE;
	BOOL bCalcBear = FALSE;
	if ( pResult->ExistResult() )
	{
		int iSure = AfxMessageBox(_LS(IDS_DGN_BN_REPERFORM_BRDG_SEIS_EVAL), MB_ICONQUESTION | MB_YESNO);
		bCalcPier = iSure==IDYES ? TRUE :FALSE;
		bCalcBear = TRUE;
	}

	CDgnDataCtrl DataCtrl;
	int iDgnCode = DataCtrl.GetBrdgSeisEvalCode();
	auto* pSeisEval = [&iDgnCode]() -> CBrdgSeisEval*
	{
		switch(iDgnCode)
		{
			case KEC_2012:	    return new CBrdgSeisEvalKEC2012();
			case KISTEC_2015:	return new CBrdgSeisEvalKISTEC2015();
			case KISTEC_2019:	return new CBrdgSeisEvalKISTEC2019();
			case KALIS_2023:	return new CBrdgSeisEvalKALIS2023();
			default:			ASSERT(0);  return nullptr;
		}
	}();

	if ( bCalcPier )
	{
		if ( !pResult->CreateSeisEvalResultFile() )
		{
			if(pSeisEval != nullptr)
			{
				delete pSeisEval;
				pSeisEval = nullptr;
			}

			return 0;
		}

		if(pSeisEval != nullptr)
		{
			bCalcBear = pSeisEval->DoSeisEval();
		}
	}

	if ( bCalcBear && pResult->ExistBearingResult() )
	{
		int iSure = AfxMessageBox(_LS(IDS_DGN_BN_REPERFORM_BRDG_SEIS_EVAL_BEARING), MB_ICONQUESTION | MB_YESNO);
		bCalcBear = iSure==IDYES ? TRUE :FALSE;
	}

	if ( bCalcBear )
	{
		if ( pResult->CreateSeisEvalBearingResultFile() )
		{
			if(pSeisEval != nullptr)
			{
				pSeisEval->DoSeisEvalBearing();
			}
		}
	}

	if(pSeisEval != nullptr)
	{
		delete pSeisEval;
		pSeisEval = nullptr;
	}

	return EN_DGNSA_BRDG_SEIS_EVAL;
}

int CServiceAgencyDgn::ExecBridgeSeismicEvalBearing(void* pData)
{
	CBrdgSeisEvalResult* pResult = CDBDoc::GetDocPoint()->m_pPostCtrl->GetBrdgSeisEvalResult(); if ( !pResult ) ASSERT(0);
	if ( pResult->ExistBearingResult() )
	{
		int iSure = AfxMessageBox(_LS(IDS_DGN_BN_REPERFORM_BRDG_SEIS_EVAL_BEARING), MB_ICONQUESTION | MB_YESNO);
		if ( iSure!=IDYES )	return EN_DGNSA_BRDG_SEIS_EVAL_BEARING;
	}

	if ( !pResult->CreateSeisEvalBearingResultFile() )
	{
		return 0;
	}

	CDgnDataCtrl DataCtrl;
	int iDgnCode = DataCtrl.GetBrdgSeisEvalCode();
	auto* pSeisEval = [&iDgnCode]() -> CBrdgSeisEval*
	{
		switch(iDgnCode)
		{
			case KEC_2012:	    return new CBrdgSeisEvalKEC2012();
			case KISTEC_2015:	return new CBrdgSeisEvalKISTEC2015();
			case KISTEC_2019:	return new CBrdgSeisEvalKISTEC2019();
			case KALIS_2023:	return new CBrdgSeisEvalKALIS2023();
			default:			ASSERT(0);  return nullptr;
		}
	}();

	if(pSeisEval != nullptr)
	{
		pSeisEval->DoSeisEvalBearing();

		delete pSeisEval;
		pSeisEval = nullptr;
	}

	return EN_DGNSA_BRDG_SEIS_EVAL_BEARING;
}

int CServiceAgencyDgn::PrintBridgeSeismicEval(void* pData)
{
	BOOL bselItems = !CDBDoc::GetDocPoint()->IsRegTestMode();

	BOOL bItems[8] = {FALSE,};
	if(bselItems)
	{
		CDgnSeisReportDlg RptItems;
		if(RptItems.DoModal() != IDOK)    return FALSE;
		memcpy(bItems, RptItems.m_bItems, sizeof(bItems));
	}

	CString sFileName;
	int nWriteType = 0;;

	CString strExt = CMSOfficeMgr::GetExtensionNameExcel();
	CString strDotExt = _T(".") + strExt;
	CString strFileFormat = _T("");

	sFileName = CDBLib::MakeFileNameWithNewExt(strFileFormat);
	strFileFormat.Format(_T("EXCEL File(*.%s)|*.%s|All File(*.*)|*.*||"), strExt, strExt);

	if(sFileName.Find(strDotExt) == -1)
	{
		sFileName = sFileName + strDotExt;
	}

	CFileDialog fd(FALSE, NULL, sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_LONGNAMES, strFileFormat);
	if(fd.DoModal() != IDOK)return FALSE;
	sFileName = fd.GetPathName();

	if(sFileName.Find(strDotExt) == -1)
	{
		sFileName = sFileName + strDotExt;
	}

	CDgnDataCtrl DataCtrl;
	int iDgnCode = DataCtrl.GetBrdgSeisEvalCode();
	if(iDgnCode == KALIS_2023)
	{
		CBrdgSeisEvalKALIS2023_Report xlSeisEval;
		if(bselItems)
		{
			xlSeisEval.SetPrintItems(bItems);
		}
		xlSeisEval.Print_SeisEval(sFileName, nWriteType);
	}
	else
	{
		CBrdgSeisEvalKISTEC2015_Report xlSeisEval;
		if(bselItems)
		{
			xlSeisEval.SetPrintItems(bItems);
		}
		xlSeisEval.Print_SeisEval(sFileName, nWriteType);
	}

	return EN_DGNSA_BRDG_SEIS_EVAL_PRINT;
}

int CServiceAgencyDgn::PrintSeisEval1stCon(void* pData)
{
	_SE_REPORT_DATA* pRprtData = static_cast<_SE_REPORT_DATA*>(pData);
	CSeisEvalTextOutMgr TextMgr;
	if ( !TextMgr.PrintSeisEval1stCon(pRprtData->aElemK, pRprtData->LcomK) ) 
	{
		ASSERT(0);
		return D_SP_ERR_IRG_PROVIDER;
	}    
	return EN_DGNSA_SEIS_EVAL_REPORT_1STCON;
}

int CServiceAgencyDgn::PrintSeisEval1stStl(void* pData)
{
	_SE_REPORT_DATA* pRprtData = static_cast<_SE_REPORT_DATA*>(pData);
	CSeisEvalTextOutMgr TextMgr;
	if ( !TextMgr.PrintSeisEval1stStl(pRprtData->aElemK, pRprtData->LcomK) ) 
	{
		ASSERT(0);
		return D_SP_ERR_IRG_PROVIDER;
	}
	return EN_DGNSA_SEIS_EVAL_REPORT_1STSTL;
}

int CServiceAgencyDgn::PrintSeisEval1stMas(void* pData)
{
	_SE_REPORT_DATA* pRprtData = static_cast<_SE_REPORT_DATA*>(pData);
	CSeisEvalTextOutMgr TextMgr;
	if ( !TextMgr.PrintSeisEval1stMas(pRprtData->aElemK, pRprtData->LcomK) ) 
	{
		ASSERT(0);
		return D_SP_ERR_IRG_PROVIDER;
	}
	return EN_DGNSA_SEIS_EVAL_REPORT_1STMAS;
}

int CServiceAgencyDgn::PrintSeisEvalUphgCon(void* pData)
{
	_SE_REPORT_DATA* pRprtData = static_cast<_SE_REPORT_DATA*>(pData);
	CSeisEvalTextOutMgr TextMgr;
	if ( !TextMgr.PrintSeisEvalUphgCon(pRprtData->aElemK, pRprtData->LcomK) ) 
	{
		ASSERT(0);
		return D_SP_ERR_IRG_PROVIDER;
	}
	return EN_DGNSA_SEIS_EVAL_REPORT_UPHGCON;
}

int CServiceAgencyDgn::PrintSeisEvalUphgStl(void* pData)
{
	_SE_REPORT_DATA* pRprtData = static_cast<_SE_REPORT_DATA*>(pData);
	CSeisEvalTextOutMgr TextMgr;
	if ( !TextMgr.PrintSeisEvalUphgStl(pRprtData->aElemK, pRprtData->LcomK) ) 
	{
		ASSERT(0);
		return D_SP_ERR_IRG_PROVIDER;
	}
	return EN_DGNSA_SEIS_EVAL_REPORT_UPHGSTL;
}

int CServiceAgencyDgn::PrintSeisEvalUphgMas(void* pData)
{
	_SE_REPORT_DATA* pRprtData = (_SE_REPORT_DATA*)(pData);
	CSeisEvalTextOutMgr TextMgr;
	if ( !TextMgr.PrintSeisEvalUphgMas(pRprtData->aElemK, pRprtData->LcomK) ) 
	{
		ASSERT(0);
		return D_SP_ERR_IRG_PROVIDER;
	}
	return EN_DGNSA_SEIS_EVAL_REPORT_UPHGMAS;
}

int CServiceAgencyDgn::ClearMPhiResultAll()
{
	CDgn_SeismicDataPool::Instance()->InitPool();
	return EN_DGNSA_CLEAR_MPHI_RESULT_ALL;
}

int CServiceAgencyDgn::ClearMPhiResultMemb(void* pData, bool bWall)
{
	auto pKey = (UINT*)pData;
	CDgn_SeismicDataPool::Instance()->ClearResult(*pKey, bWall);
	return EN_DGNSA_CLEAR_MPHI_RESULT_ALL;
}

int CServiceAgencyDgn::ExecBridgeSeismicEval_JP(void* pData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) return 0;

	CBrdgSeisEvalResult_JP* pResult = pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	ASSERT(pResult != NULL);
	if (pResult == NULL) return 0;

	if (pResult->ExistResult())
	{
		int iSure = AfxMessageBox(_LS(IDS_DGN_BN_REPERFORM_BRDG_SEIS_EVAL), MB_ICONQUESTION | MB_YESNO);
		if (iSure != IDYES)
			return 0;
	}

	pResult->DeleteAllResult();

	CDgnProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(PROGRESS_TYPE_BRDG_SEIS_EVAL_JP);

	CBrdgSeisEval_JP brdgSeisEval;
	if(brdgSeisEval.DoSeisEval(&ProDlg) == TRUE)
		pResult->WriteBrdgSeisEvalResultAll();

	ProDlg.Delete_ProgressDlg();

	return EN_DGNSA_BRDG_SEIS_EVAL_JP;
}

int CServiceAgencyDgn::PrintBridgeSeismicEval_JP(void* pData)
{
	CDgnExcelPrintDlg dlg;
	if (dlg.DoModal() != IDOK) return 0;
	
	SDgnExcelPrintOpt printOpt;
	dlg.GetPrintOptions(printOpt);
	if(printOpt.aPrintSipaK.GetCount() == 0) 
		return EN_DGNSA_BRDG_SEIS_EVAL_PRINT_JP;

	CString sFileName;
	CString strExt = CMSOfficeMgr::GetExtensionNameExcel();
	CString strDotExt = _T(".") + strExt;
	CString strFileFormat = _T("");

	sFileName = CDBLib::MakeFileNameWithNewExt(strFileFormat);
	strFileFormat.Format(_T("EXCEL File(*.%s)|*.%s|All File(*.*)|*.*||"), strExt, strExt);

	if(sFileName.Find(strDotExt) == -1)
	{
		sFileName = sFileName + strDotExt;
	}

	CFileDialog fd(FALSE, NULL, sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_LONGNAMES, strFileFormat);
	if(fd.DoModal() != IDOK)return FALSE;
	sFileName = fd.GetPathName();

	if(sFileName.Find(strDotExt) == -1)
	{
		sFileName = sFileName + strDotExt;
	}

	CDgnProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(PROGRESS_TYPE_BRDG_SEIS_EVAL_XL_RPT_JP);

	CBridgeSeisEvalReport_JP xlSeisEval(&ProDlg, sFileName);
	xlSeisEval.PrintOut(&printOpt);

	ProDlg.Delete_ProgressDlg();

	return EN_DGNSA_BRDG_SEIS_EVAL_PRINT_JP;
}

//서비스 대리점을 본사에 등록
class CServiceAgencyDgnInit
{
public:
	CServiceAgencyDgnInit()
	{ 
		CServiceHeadOffice* pHeadOffice = CServiceHeadOffice::Instance();
		if(pHeadOffice)
		{
			pHeadOffice->RegAgency(_ULS(dgn), new CServiceAgencyDgn);
		}
	}
};
CServiceAgencyDgnInit init;