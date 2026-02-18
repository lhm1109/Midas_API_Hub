// RptPlateGirder_IRC22.cpp: implementation of the CRptPlateGirder_IRC22 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "RptPlateGirder_IRC22.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\AnalysisResult.h"
// 
#include "..\wg_db\DBLib.h"

#include "DgnProgressDlg.h"
#include "DgnPlateGirder.h"
#include "DgnPlateGirder_IRC.h"
//#include "CRCDataCtrl.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptPlateGirder_IRC22::CRptPlateGirder_IRC22()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;

	m_bTestMode = CDBLib::CheckTheRegistryForDesignTest();

	T_CPGD_D CpgdD; CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

	m_CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(m_CpgdD);

	int nCivilCode = m_CpgdD.iDgnCode;
	m_pDoc->SetCivilCodeDgn(nCivilCode);

	m_pDataCtrl = new CCRCDataCtrl();

	m_dZero=1.0E-07;
	m_FontType = _T("Arial");

	//Initial Data


	m_bCurved  = FALSE;
	m_bSupprotNode= FALSE;
	m_bAppTapered = FALSE;
	m_b1p3RhMy    = FALSE;
	m_bPBTFA4SH   = FALSE;
	m_bDistortion = FALSE;
	m_bIsRatingDgn= FALSE;

	m_dF_l= 0.0;
	m_dP_l= 0.0;
	m_dAlpha= 0.0;
	m_dM_Lat_R  = 0.0;
	m_dM_Lat_df = 0.0;
	m_dM_Lat_cf = 0.0;
	m_dM_Lat   = 0.0;
	m_dTheta   = 0.0;
	m_dCosTap  = 0.0;
	m_bClassfication = FALSE;
	m_dfr = 0.0;
	m_dfdeck =m_dTrstPitch =  0.0;
	m_bCompactPosiServ = m_bTrstProvided = FALSE;

	//ÃÊ±âÈ­.
	m_dfbuc = 0.0; // compression flange
	m_dfbut = 0.0; // tension falange
	m_dft_g = 0.0;
	m_dfb_g = 0.0;
	m_dft_st = 0.0;
	m_dfb_st = 0.0;
	m_dft_lt = 0.0;
	m_dfb_lt = 0.0;
	m_dftop  = 0.0;
	m_dfbot  = 0.0;
	m_nLongStiNum_Left  = 0;
	m_nLongStiNum_Right = 0;

	//ÃÊ±âÈ­Á» ÇÏÀÚ.
	m_nChkType = 0;
	m_dRadius  = 0.0;
	m_dN     = 0.0;
	m_dcrt = 0.0;
	m_dcrb = 0.0;
	m_dfc  = 0.0;
	m_dArt = 0.0;
	m_dArb = 0.0;
	m_dFyr = 0.0;
	m_dRebarDia_Top = 0.0;
	m_dRebarSpace_Top = 0.0;
	m_dRebarDia_Bot = 0.0;
	m_dRebarSpace_Bot = 0.0;
	m_dbfc = 0.0;
	m_dbfc_betweenWeb = 0.0;
	m_dbfc_hf = 0.0;
	m_dtfc = 0.0;
	m_dbft = 0.0;
	m_dbft_betweenWeb = 0.0;
	m_dbft_hf = 0.0;
	m_dtft = 0.0;
	m_dD = 0.0;
	m_dAoh = 0.0;
	m_dAoh_nc = 0.0;
	m_dAft = 0.0;
	m_dAfb = 0.0;
	m_dFyc = 0.0;
	m_dFyt = 0.0;
	m_dFyw = 0.0;
	m_dOrgFyc = 0.0;
	m_dOrgFyt = 0.0;
	m_dOrgFyw = 0.0;
	m_iType =m_nRowNo= 0;
	m_nCategoryType_SC = 0;
	m_dH_SC = 0.0;
	m_dd_SC = 0.0;
	m_dW_SC = 0.0;
	m_dp_use = 0.0;
	m_dst_use = 0.0;
	m_nn = 0;
	m_dFyu = 0.0;
	m_dFrc = 0.0;
	m_nType_ls = 0;
	m_nLStfnNumCFlg = 0;
	m_dbl_ls = 0.0;
	m_dts_ls = 0.0;
	m_dd0_ls = 0.0;
	m_dd1_ls = 0.0;
	m_dI_ls = 0.0;
	m_dI_FromCenter_ls = 0.0;
	m_dA_ls = 0.0;
	m_dy_ls = 0.0;
	m_dFys_ls = 0.0;
	m_dw = 0.0;
	m_dIs = 0.0;
	m_dIs_web = 0.0;
	m_nCurveType = 0;
	m_nType_ts = 0;
	m_dbt_ts = 0.0;
	m_dtp_ts = 0.0;
	m_dd0_ts = 0.0;
	m_dFys_ts = 0.0;
	m_dFys_bs = 0.0;
	m_dDistForce = 0.0;
	m_dConcForce = 0.0;
	m_dEccenLoad = 0.0;
	m_dI = 0.0;
	m_dI_3n = 0.0;
	m_dI_r = 0.0;
	m_dI_n = 0.0;
	m_dI_r3 = 0.0;
	m_dStop = 0.0;
	m_dStop_3n = 0.0;
	m_dStop_r3 = 0.0;
	m_dStop_n = 0.0;
	m_dStop_r = 0.0;
	m_dSbot = 0.0;
	m_dSbot_3n = 0.0;
	m_dSbot_r3 = 0.0;
	m_dSbot_n = 0.0;
	m_dSbot_r = 0.0;
	m_dScom_z = 0.0;
	m_dyc = 0.0;
	m_dyt = 0.0;
	m_dy_n_slab =  m_dBeffFactor =0.0;
	m_dMd1 = m_dAr = 0.0;
	m_dMd2 = 0.0;
	m_dMd3 = 0.0;
	m_dMuy = 0.0;
	m_dVu_gr = 0.0;
	m_dVu_lt = 0.0;
	m_dVu_st = 0.0;
	m_dVu_sm = 0.0;
	m_dMuz = 0.0;
	m_dVu = 0.0;
	m_dVuInclined = 0.0;
	m_dT = 0.0;
	m_dT1 = 0.0;
	m_dT2 = 0.0;
	m_dT3 = 0.0;
	m_dMy_Ulm_max = 0.0;
	m_dLb = 0.0;
	m_dADTT_SL = 0.0;
	m_dADTT_SL75 = 0.0;
	m_nDetailCat = 0;
	m_dn_Fati = 0.0;
	m_nCase_PNA = 0;
	m_dY_Bar_PNA  = 0.0;
	m_dA0 = 0.0;
	m_dMy = 0.0;
	m_dMyt = 0.0;
	m_dMyc = 0.0;
	m_dVn = 0.0;
	m_dVcr = 0.0;
	m_dRh = 0.0;
	m_dMyMax_Pos = 0.0;
	m_dMyMax_Nes = 0.0;
	m_dftop_range = 0.0;
	m_dfbot_range = 0.0;
	m_df_top_com_st = 0.0;
	m_df_top_ten_st = 0.0;
	m_df_bot_com_st = 0.0;
	m_df_bot_ten_st = 0.0;
	m_dLength4SC = 0.0;
	m_nSFCalc = 0;
	m_nWarpType = 0;
	m_dTopFlange = 0.0;
	m_dBotFlange = m_dFu = 0.0;
	m_strShapeType = m_strMatNameSteel = _T("");
}

CRptPlateGirder_IRC22::~CRptPlateGirder_IRC22()
{
	if ( m_pDataCtrl )
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
	}
}

void CRptPlateGirder_IRC22::SetUnitConvertFactor()
{
	// [Code Unit] : N, mm.
	double dPi = atan(1.0) * 4.0;

	m_dFactorForce  = 0.001;
	m_dFactorLength = 0.001;
	m_dFactorLengthEach = (m_bUnitSI)? 1.0 : 0.1;
	m_dFactorLengthMM   = 1.0;
	m_dFactorMoment = 0.001 * 0.001;
	m_dFactorStress = 1.0;
	m_dFactorRebarArea     = 1.0;
	m_dFactorInertiaMoment = pow(0.001, 4);
	m_dFactorAngle  = 180.0 / (dPi);

	m_strUnitForce.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("kN") : _T("tonf")));
	m_strUnitLength.Format(_T("0.000 %s"), _T("m"));
	m_strUnitLengthEach.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("mm") : _T("cm")));
	m_strUnitLengthMM.Format(_T("0.00000 %s"), _T("mm"));
	m_strUnitMoment.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("kN*m") : _T("tonf*m")));
	m_strUnitStress.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("MPa") : _T("kgf/cm^2")));
	m_strUnitRebarArea.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("mm^2") : _T("cm^2")));
	m_strUnitInertiaMoment.Format(_T("0.000000 %s"), _T("m^4"));
	m_strUnitDegree.Format(_T("0.00 %s"), _T("°"));

	m_strNone3.Format(_T("0.000"));
	m_strNone4.Format(_T("0.0000"));

	//m_strTens = _T("..... ÀÎÀå");
}

BOOL CRptPlateGirder_IRC22::Get_RptElemKey(ArrElemPairKey& aRptElemKey)
{
	aRptElemKey.RemoveAll();

	int i=0;
	CArray<T_CGRE_K, T_CGRE_K> aPrintElemKey;
	m_pDoc->m_pAttrCtrl2->GetCgreKeyList(aPrintElemKey);

	CArray<T_CGRV_K, T_CGRV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl2->GetCgrvKeyList(aPrintVBeamKey);

	ArrElemPairKey aRptElemPairK;
	CDBLib::ConvertToElemPairKeyList(aPrintElemKey, aPrintVBeamKey, aRptElemPairK);

	int iPrintElemSize = aRptElemPairK.GetSize();
	CString strErrMessage = _T("");
	if ( iPrintElemSize>0 )
	{
		CArray<T_CGDP_K, T_CGDP_K> aPgdElemKey;  aPgdElemKey.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetCgdpKeyList(aPgdElemKey);
		
		CArray<T_CGDV_K, T_CGDV_K> aPgdVBeamKey;
		m_pDoc->m_pAttrCtrl->GetCgdvKeyList(aPgdVBeamKey);

		ArrElemPairKey aPgdElemPairK;
		CDBLib::ConvertToElemPairKeyList(aPgdElemKey, aPgdVBeamKey, aPgdElemPairK);

		int iCheckElemSize = aPgdElemPairK.GetSize();
		if ( iCheckElemSize<=0 )
		{
			strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("Ãâ·ÂÇÒ ¿ä¼ÒÀÇ ¼³°èÁ¤º¸°¡ ¾ø½À´Ï´Ù.")
			AfxMessageBox(strErrMessage);
			return FALSE;
		}
		std::map<ElemPairK, ElemPairK> mapCheckElemKey;		
		for ( i=0; i<iCheckElemSize; i++ )
		{
			auto ElemK = aPgdElemPairK[i];
			mapCheckElemKey.insert({ ElemK, ElemK });
		}
		//
		BOOL bIsExistPrintElem=FALSE;
		for ( i=0; i<iPrintElemSize; i++ )
		{
			auto ElemK = aRptElemPairK[i];
			T_ELEM_K TempElemK=0;
			const auto& itChk = mapCheckElemKey.find(ElemK);
			if (itChk != mapCheckElemKey.end())
			{
				if ( Get_CheckResultAndPrintElemPositionType(ElemK) )
				{
					bIsExistPrintElem = TRUE;
					break;
				}
			}
		}
		if ( !bIsExistPrintElem )
		{
			strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("Ãâ·ÂÇÒ ¿ä¼ÒÀÇ ¼³°èÁ¤º¸°¡ ¾ø½À´Ï´Ù.")
			AfxMessageBox(strErrMessage);
			return FALSE;
		}
	}
	else
	{
		strErrMessage = _LS(IDS_DGN_PSC_RATING_NON_MEMBER); // _T("Ãâ·ÂÇÒ ¿ä¼Ò°¡ ¾ø½À´Ï´Ù.")
		AfxMessageBox(strErrMessage);
		return FALSE;
	}


	aRptElemKey.Copy(aRptElemPairK);

	return TRUE;
}

BOOL CRptPlateGirder_IRC22::Get_CheckResultAndPrintElemPositionType(ElemPairK ElemK)
{
	T_CGRE_D CgreD; CgreD.Initialize();
	T_CGDP_D CgdpD; CgdpD.Initialize();
	if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetCgre(ElemK.first, CgreD)) return FALSE;
		if (!m_pDoc->m_pAttrCtrl->GetCgdp(ElemK.first, CgdpD)) return FALSE;
	}
	else if (ElemK.second == EN_EL_VBEAM)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetCgrv(ElemK.first, CgreD)) return FALSE;
		if (!m_pDoc->m_pAttrCtrl->GetCgdv(ElemK.first, CgdpD)) return FALSE;
	}
	else ASSERT(0);
	if ( CgreD.iDgnPosi == 0 ) // (0=I´Ü, 1=J´Ü, 2=¾çÂÊ¸ðµÎ)
	{
		if ( CgdpD.iDgnPosi == 1 ) return FALSE;
	}
	else if ( CgreD.iDgnPosi == 1 )
	{
		if ( CgdpD.iDgnPosi == 0 ) return FALSE;
	}

	return TRUE;
}

void CRptPlateGirder_IRC22::SetDetailSheetPage(CMSExcel* pXL, CString strNameBefore, CString strNameCurrent)
{
	if ( !pXL->IsExistSheetName(strNameCurrent) )
	{
		pXL->AddSheet(strNameCurrent);
	}
	pXL->SetActiveSheet(strNameCurrent);
	pXL->MoveSheet(strNameBefore, strNameCurrent);

	pXL->SetStandardFont(_T("Arial"));
	pXL->SetMargin(10.0, 10.0, 10.0, 10.0);  // 1inch = 25.4mm
	pXL->SetColumnWidth(0, 0, 0, 32, 2.0);
	pXL->SetRowHeight(16);

}

void CRptPlateGirder_IRC22::PrintReport(CString strPath, int iPrintOpt, BOOL bSaveImage)
{
	if ( !Get_RptElemKey(m_arDetlElem) ) return;

	//CCurUnitSaver Save(TRUE);
	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
// 	T_UNIT_INDEX CngIndex; 
// 	CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
// 	CngIndex.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
// 	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	//++++++++++++++++++++++++++++++++++++++++++++

	// Create Process Dialog.
	CDgnProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(PROGRESS_TYPE_STEEL_XL_RPT);

	m_strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");
	m_strBaseBook = _T("plate_girder_IRC22_base.dll"); // .dll·Î ¼öÁ¤ÇØ¾ßÇÔ. 

	// Set Picture Path.
	CString strDataPath = m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strDataPath);
	CString strPathAndName = FileCtrl.GetFilePathAndNameWithoutExtension() + _T(".xls");
	CSG_MEMB_POSD_KSCE_LSD    MembLcomD;
	CSG_MEMB_RES_AASHTO_LRFD_D  RptData;
	// Get Exist Excel Process IDs.
	CArray<unsigned int, unsigned int> arProcID;
	if ( !m_bTestMode )
	{
		GetExistedExcelProcessID(arProcID);
	}

	m_pDataCtrl->MakePlateGirderDesign4Report();
	m_pDataCtrl->Get_PlateGirderPointer_IRC()->Set_PrintMode(TRUE);

	int nOutType;
	BOOL bRunMS, bRowMark, bColumnMark;
	CProduct::GetTestOutType(nOutType, bRunMS, bRowMark, bColumnMark);
	CMSExcel* pXL = new CMSExcel(nOutType, bRunMS, bRowMark, bColumnMark);
	pXL->Open(m_strBasePath + m_strBaseBook);

	// Apply Print Option.
	if ( iPrintOpt == 0 || iPrintOpt == 2 )
	{
		pXL->AddBook();
	}
	else
	{
		pXL->Open(strPath);
		pXL->SetActiveBook(pXL->GetBookName());
	}

#if defined(_DEBUG)
	pXL->SetVisible(TRUE);
#else
	if (CTestEnvMgr::IsTestEnvST(_T("ExcelVisible"), _T("yes")))
		pXL->SetVisible(TRUE);
	else
		pXL->SetVisible(FALSE);
#endif

	SetOutputUnitSystem();
	SetUnitConvertFactor();
	m_arSheetName.RemoveAll();

	m_CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(m_CpgdD);
	ConvertCGSGlobalData(m_CpgdD, MembLcomD.Memb.Csgd);

	COleMessageFilter *pFilter = AfxOleGetMessageFilter();
	pFilter->SetMessagePendingDelay();
	if ( iPrintOpt==2 ) pXL->SaveAs(pXL->GetBookName(), strPathAndName);
	else             pXL->SaveAs(pXL->GetBookName(), strPath);
	pFilter->SetMessagePendingDelay();

	int iStRow = 1;
	//T_STED_D StedD;
	//T_STED_BASE StedBase;
	CString strElemName = _T("");

	BOOL bStopCalc = FALSE;
	int iChkSect  = 1;  // °ËÅä ºÎÀçÀÇ Index.
	int iDetlElem = m_arDetlElem.GetSize();
	CString strSheetBefore = _T("");

	int i=0, j=0, k=0;

	BOOL bCheckStrength[6];
	for ( i=0; i<6; i++ ) bCheckStrength[i] = m_CpgdD.bStrength[i];
	BOOL bCheckService[2];
	for ( i=0; i<2; i++ ) bCheckService[i] = m_CpgdD.bService[i];
	RptData.Initialize();
	T_PGCH_D PgchD; PgchD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCpgCheckItem(PgchD) ) PgchD.Initialize();

	for ( i=0; i<iDetlElem; i++ )
	{
		//Show Progress Dlg.
		if ( ProDlg.Update_ProgressDlg(i+1, iDetlElem+1) ) { bStopCalc = TRUE;	break; }

		auto ElemK = m_arDetlElem[i];
		T_CGDP_D CgdpD; CgdpD.Initialize();
		T_CGRE_D CgreD; CgreD.Initialize();
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

		////////


		////////

		T_PRVS_D PrvsD;
		T_PLTB_D PltbD;
		T_PRTF_D PrtfD;
		T_PGLS_D PglsD;
		T_PGRF_D PgrfD;
		T_PGBS_D PgbsD;
		T_PLSS_D PlssD;
		if ( bCheckStrength[1] ) { if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPrvsResultData(ElemK, PrvsD) ) {} }
		if ( bCheckStrength[2] ) { if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPltbResultData(ElemK, PltbD) ) {} }
		if ( bCheckStrength[3] ) { if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPrtfResultData(ElemK, PrtfD) ) {} }
		if ( bCheckStrength[4] ) { if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPglsResultData(ElemK, PglsD) ) {} }
		if ( bCheckStrength[5] ) { if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPgrfResultData(ElemK, PgrfD) ) {} }
		if ( bCheckService[0] ) { if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPgbsResultData(ElemK, PgbsD) ) {} }
		if ( bCheckService[1] ) { if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPlssResultData(ElemK, PlssD) ) {} }

		for ( j=0; j<2; j++ ) // 0=I, 1=J
		{
			BOOL bPosiI = j==0 ? TRUE : FALSE;
			if ( !m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, j+1) ) continue;
			if ( j==0 && CgreD.iDgnPosi==1 ) continue;
			if ( j==1 && CgreD.iDgnPosi==0 ) continue;
			strElemName.Format(_T("Elem_%d_%s"), ElemK, (j==0)? _T("i") : _T("j"));
			m_strDetlSheet2 = (m_bTestMode)? _T("Detail") : strElemName;
			if ( pXL->IsExistSheetName(m_strDetlSheet2) && iPrintOpt == 1 )
			{
				m_strDetlSheet2 = pXL->GetMaximumSheetName(m_strDetlSheet2) + _T("_New");
			}
			m_arSheetName.Add(m_strDetlSheet2);
			strSheetBefore = (m_arSheetName.GetSize() > 1)? m_arSheetName.GetAt(m_arSheetName.GetSize()-2) : m_arSheetName.GetAt(m_arSheetName.GetSize()-1);

			if ( !m_bTestMode ) iStRow = 0;
			SetDetailSheetPage(pXL, strSheetBefore, m_strDetlSheet2);

			Get_DesignInfo_Section(ElemK, bPosiI);
			T_SECT_STIFFNESS SpscSect;
			SpscSect.Initialize();
			if ( !CSectUtil::GetCompoSectCR(TRUE, FALSE, FALSE, 0, TRUE, ElemK, bPosiI, FALSE, SpscSect) )	ASSERT(0);



			int iChpaterIx=2;
			if ( bCheckStrength[0] && PgchD.bPgdCheck[0] ) { m_iBendingChapterIx = iChpaterIx++; }
			if ( bCheckStrength[1] && PgchD.bPgdCheck[1] ) { m_iShearChapterIx = iChpaterIx++; }
			if ( bCheckStrength[2] && PgchD.bPgdCheck[2] )
			{
				if ( m_nSectType==D_SECT_TYPE_COMPO_I ) m_iLTBChapterIx = iChpaterIx++;
			}
			if ( bCheckStrength[3] && PgchD.bPgdCheck[3] )
			{
				if ( m_nSectType==D_SECT_TYPE_COMPO_I ) m_iTransverseForceChapterIx = iChpaterIx++;
			}
			if ( bCheckStrength[4] && PgchD.bPgdCheck[4] ) { m_iLongitudinalChapterShearIx = iChpaterIx++; };
			if ( bCheckStrength[5] && PgchD.bPgdCheck[5] )
			{
				if ( m_nSectType==D_SECT_TYPE_COMPO_I ) m_iFatigueChapterIx = iChpaterIx++;
			}
			if ( bCheckService[0] && PgchD.bPgdCheck[6] ) { m_iStressLimitChapterIx = iChpaterIx++; }
			if ( bCheckService[1] && PgchD.bPgdCheck[7] ) { m_iLongShearSLSChapterIx = iChpaterIx++; }
			BOOL bPrintDgnInfo = TRUE;
			if ( bCheckStrength[0] )
			{
				iChkSect=0;
				for ( k=0; k<2; k++ ) // Max/Min, 
				{
					int nPosiNega = k+1; // Positive           
					T_PGBR_LCOM PgbrRatD; PgbrRatD.Initialize(); // PositiveÀÇ I,J¸¦ ¾ò¾î¿Â´Ù. 
					if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPgbrResultData(ElemK, nPosiNega, PgbrRatD) )
					{
						T_PGBR_BASE PgbrGd   = PgbrRatD.BeforePgbr[j];
						T_PGBR_BASE PgbrBase = PgbrRatD.PgbrBase[j];
						T_PGBR_DETAIL PgbrDetailGd, PgbrDetail;
						if ( PgbrBase.bChk && m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_BendingResistanceDetailResult4PlateGirder(ElemK, bPosiI, PgbrGd, PgbrBase, PgbrDetail) )
						{
							m_dBeffFactor = PgbrBase.dBeffFactor;
							SetRptData(ElemK, j, MembLcomD.Memb, RptData, SpscSect, PgbrBase);
							if ( bPrintDgnInfo )
							{//Print only once
								PrintDesignInfo_Mod(pXL, iStRow, ElemK.first, j+1, RptData, SpscSect);
								bPrintDgnInfo = FALSE;
							}
							PrintBendingResistance(pXL, iStRow, ++iChkSect, nPosiNega, PgbrGd, PgbrBase, PgbrDetail, RptData, SpscSect, PglsD);

						}
					}
				}
			}
			if ( bCheckStrength[2] ) // Lateral Torsional Buckling
			{
				iChkSect=0;
				T_PLTB_BASE PltbGd = PltbD.BeforePltb[j];
				T_PLTB_BASE PltbBase = PltbD.PltbBase[j];
				T_PLTB_DETAIL PltbDetailGd, PltbDetail;
				if ( PltbBase.bChk && m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_LateralTorsionalBucklingResistanceDetailResult4PlateGirder(ElemK, bPosiI, PltbGd, PltbBase, PltbDetail) )
				{
					BOOL bPositive = PltbBase.dMy > -m_dZero ? TRUE : FALSE;
					PrintLateralTorsionalBucklingResistance(pXL, iStRow, ++iChkSect, k, PltbGd, PltbBase, PltbDetail);
					if ( !bPositive )
					{
						if ( m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_LateralTorsionalBucklingResistanceDetailResult4PlateGirder(ElemK, bPosiI, PltbGd, PltbBase, PltbDetail, TRUE) )
						{
							PrintLateralTorsionalBucklingResistance_Neg(pXL, iStRow, ++iChkSect, k, PltbGd, PltbBase, PltbDetail);
						}
					}
				}
			}
			if ( bCheckStrength[1] ) // Shear Resistance
			{
				iChkSect=0;
				T_PRVS_BASE PrvsGd = PrvsD.BeforePrvs[j];
				T_PRVS_BASE PrvsBase = PrvsD.PrvsBase[j];
				T_PRVS_DETAIL PrvsDetailGd, PrvsDetail;
				if ( PrvsBase.bChk && m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_VerticalShearResistanceDetailResult4PlateGirder(ElemK, bPosiI, PrvsGd, PrvsBase, PrvsDetail) )
				{
					PrintShearResistance(pXL, iStRow, ++iChkSect, k, PrvsGd, PrvsBase, PrvsDetail);
				}
			}

			if ( bCheckStrength[3] ) // Transverse Force
			{
				iChkSect=0;
				T_PRTF_BASE PrtfBase = PrtfD.PrtfBase[j];
				T_PRTF_DETAIL PrtfDetail;
				if ( PrtfBase.bChk && m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_TransverseForceResistanceDetailResult4PlateGirder(ElemK, bPosiI, PrtfBase, PrtfDetail) )
				{
					PrintTransverseForceResistance(pXL, iStRow, ++iChkSect, k, PrtfBase, PrtfDetail);
				}
			}
			if ( bCheckStrength[4] ) // Longitudinal Shear
			{
				iChkSect=0;
				T_PGLS_BASE PglsGd = PglsD.BeforePgls[j];
				T_PGLS_BASE PglsBase = PglsD.PglsBase[j];
				T_PGLS_DETAIL PglsDetailGd, PglsDetail;
				if ( PglsBase.bChk && m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_LongitudinalShearResistanceDetailResult4PlateGirder(ElemK, bPosiI, PglsGd, PglsBase, PglsDetail) )
				{
					PrintLongitudinalShearResistance(pXL, iStRow, ++iChkSect, k, PglsGd, PglsBase, PglsDetail);
				}
			}
			if ( bCheckStrength[5] ) // Fatigue
			{
				iChkSect=0;
				T_PGRF_BASE PgrfBase = PgrfD.PgrfBase[j];
				T_PGRF_DETAIL PgrfDetail;
				if ( PgrfBase.bChk && m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_FatigueResistanceDetailResult4PlateGirder(ElemK, bPosiI, PgrfBase, PgrfDetail) )
				{
					PrintFatigueResistance(pXL, iStRow, ++iChkSect, k, PgrfBase, PgrfDetail);
				}
			}
			if ( bCheckService[0] )
			{
				iChkSect=0;
				T_PGBS_BASE PgbsBase = PgbsD.PgbsBase[j];
				T_PGBS_DETAIL PgbsDetail;
				if ( PgbsBase.bChk && m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_BeamStressDetailResult4PlateGirder(ElemK, bPosiI, PgbsBase, PgbsDetail) )
				{
					PrintStressLimit(pXL, iStRow, ++iChkSect, k, PgbsBase, PgbsDetail);
				}
			}
			if ( bCheckService[1] )
			{
				iChkSect=0;
				T_PLSS_BASE PlssGd = PlssD.BeforePlss[j];
				T_PLSS_BASE PlssBase = PlssD.PlssBase[j];
				T_PLSS_DETAIL PlssDetailGd, PlssDetail;
				if ( PlssBase.bChk && m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_LongitudinalShearResistanceServiceDetailResult4PlateGirder(ElemK, bPosiI, PlssGd, PlssBase, PlssDetail) )
				{
					// PrintLongitudinalShear_SLS(pXL, iStRow, ++iChkSect, k, PlssGd, PlssBase, PlssDetail);
				}
			}

			pFilter->SetMessagePendingDelay(5000);
			pXL->Save(pXL->GetBookName());
			pFilter->SetMessagePendingDelay(10000);
		}
	}

	if ( ProDlg.Update_ProgressDlg(iDetlElem+1, iDetlElem+1) ) { bStopCalc = TRUE; }
	/*
	// Set HyperLinks.
	pXL->SetActiveSheet(m_strSummSheet);
	for(i=0; i<iSectCount; i++)
	{
		pXL->SetHyperLink(arAnchorCells.GetAt(i), _T("'") + arDetailSheet.GetAt(i) + _T("'"), arLinkedCells.GetAt(i));
	}
	*/
	// Delete Default Sheet and Save.
	if ( m_arSheetName.GetSize() > 0 ) pXL->DeleteDefaultSheet();
	// Set Active Sheet For Summary Sheet.      
	//pXL->SetSheetSelection(m_strSummSheet);

	pFilter->SetMessagePendingDelay(5000);
	pXL->Save(pXL->GetBookName());
	pFilter->SetMessagePendingDelay(10000);

	pXL->Quit();
	delete pXL;

	// Kill Excel Process IDs producted this time.
	KillExcelProcess(arProcID);


	ProDlg.Delete_ProgressDlg();

	ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL);

	// ('06.05.12). For Regression test : After generating excel file, pop up AfxMessageBox.
	if ( m_bTestMode )
	{
		AfxMessageBox(_T("Finish generating excel file."), MB_OK);
	}
}

void CRptPlateGirder_IRC22::PrintTitle_Sub_Title_Shear_ResistanceChk(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_Resistance_Chk"), iRowCnt, iColCnt);

	iStRow += iRowCnt;
}

void CRptPlateGirder_IRC22::PrintTitle_Sub_Title_Shear_Web(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_Web"), iRowCnt, iColCnt);

	iStRow += iRowCnt;
}

void CRptPlateGirder_IRC22::PrintTitle_Sub_Title_Shear_LeftWeb(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_Left_Web"), iRowCnt, iColCnt);

	iStRow += iRowCnt;
}

void CRptPlateGirder_IRC22::PrintTitle_Sub_Title_Shear_Right_Web(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_Right_Web"), iRowCnt, iColCnt);

	iStRow += iRowCnt;
}

void CRptPlateGirder_IRC22::PrintTitle_Sub_Title_Stress_Limitation_Chk(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Title_of_Stress_Limit"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, m_iStressLimitChapterIx);

	iStRow += iRowCnt;
}


void CRptPlateGirder_IRC22::PrintDesignInfo(CMSExcel* pXL, int& iStRow, int iElemNo, int nPosi)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strChapter   = _T("");
	CString strBaseSheet = _T("Bending_Resistance");
	int iChapterIx = 1;
	int iSubChapterIx = 1;

	Print_CheckElemPosition(pXL, iStRow, strBaseSheet, iElemNo, nPosi);
	// Design Condition
	strChapter.Format(_T("%d"), iChapterIx);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Title_of_DesignCondition"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, strChapter);
	iStRow += iRowCnt;
	strChapter.Format(_T("%d.%d"), iChapterIx, iSubChapterIx);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_DgnCdn_parameters"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, strChapter);
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_param_partial_factors"), iRowCnt, iColCnt); // - Partial factors
	pXL->SetValue(iStRow+1, 9, m_CpgdD.dGamma_C);
	pXL->SetValue(iStRow+2, 9, m_CpgdD.dGamma_S);
	pXL->SetValue(iStRow+3, 9, m_CpgdD.dGamma_M0);
	pXL->SetValue(iStRow+4, 9, m_CpgdD.dGamma_M1);
	pXL->SetValue(iStRow+1, 27, m_CpgdD.dGamma_V);
	pXL->SetValue(iStRow+2, 27, m_CpgdD.dGamma_Ff);
	pXL->SetValue(iStRow+3, 27, m_CpgdD.dGamma_Mf);
	pXL->SetValue(iStRow+4, 27, m_CpgdD.dGamma_Mfs);

	iStRow += iRowCnt + 1;

	// Material
	iSubChapterIx = 2;
	strChapter.Format(_T("%d.%d"), iChapterIx, iSubChapterIx);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_DgnCdn_MatInfo"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, strChapter);
	iStRow += iRowCnt;
	PrintDesignInfo_Material(pXL, iStRow, strBaseSheet);

	// Section Information  
	iSubChapterIx = 3;
	strChapter.Format(_T("%d.%d"), iChapterIx, iSubChapterIx);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_DgnCdn_SectInfo"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, strChapter);
	iStRow += iRowCnt;
	//PrintDesignInfo_Section(pXL, iStRow, strBaseSheet, RptData,SpscSect);

	iStRow += 1;
}

void CRptPlateGirder_IRC22::PrintDesignInfo_Mod(CMSExcel* pXL, int& iStRow, int iElemNo, int nPosi, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, T_SECT_STIFFNESS& SpscSect)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strChapter   = _T("");
	CString strBaseSheet = _T("I_Sect");
	int iChapterIx = 1;
	int iSubChapterIx = 1;

	Print_CheckElemPosition(pXL, iStRow, strBaseSheet, iElemNo, nPosi);
	// Design Condition
	strChapter.Format(_T("%d"), iChapterIx);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Title_of_DesignCondition"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, strChapter);
	iStRow += iRowCnt;
	strChapter.Format(_T("%d.%d"), iChapterIx, iSubChapterIx);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_DgnCdn_parameters"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, strChapter);
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_param_partial_factors"), iRowCnt, iColCnt); // - Partial factors
	pXL->SetValue(iStRow+1, 9, m_CpgdD.dGamma_C);
	pXL->SetValue(iStRow+2, 9, 1.2);
	pXL->SetValue(iStRow+3, 9, m_CpgdD.dGamma_S);
	pXL->SetValue(iStRow+4, 9, m_CpgdD.dGamma_M0);
	pXL->SetValue(iStRow+5, 9, m_CpgdD.dGamma_M1);

	pXL->SetValue(iStRow+1, 27, m_CpgdD.dGamma_V);
	pXL->SetValue(iStRow+2, 27, m_CpgdD.dGamma_Ff);
	pXL->SetValue(iStRow+3, 27, m_CpgdD.dGamma_Mf);
	pXL->SetValue(iStRow+4, 27, m_CpgdD.dGamma_Mfs);

	iStRow += iRowCnt;

	// Material
//   iSubChapterIx = 2;
//   strChapter.Format(_T("%d.%d"),iChapterIx, iSubChapterIx);
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_DgnCdn_MatInfo"), iRowCnt, iColCnt);
//   pXL->SetValue(iStRow, 1, strChapter);
//   iStRow += iRowCnt;
//   PrintDesignInfo_Material(pXL, iStRow, strBaseSheet);

	// Section Information  
	iSubChapterIx = 2;
	strChapter.Format(_T("%d.%d"), iChapterIx, iSubChapterIx);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_DgnCdn_SectInfo"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, strChapter);
	iStRow += iRowCnt;
	PrintDesignInfo_Section(pXL, iStRow, strBaseSheet, RptData, SpscSect);

	iStRow += 1;
}

void CRptPlateGirder_IRC22::PrintDesignInfo_Section(CMSExcel* pXL, int& iStRow, CString strBaseSheet, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, T_SECT_STIFFNESS& SpscSect)
{
	// 1. Section Size
	if ( m_nSectType != D_SECT_TYPE_COMPO_G )
	{
		PrintDesignInfo_SectionSize(pXL, iStRow, strBaseSheet, RptData, SpscSect);
	}
	else if ( m_nSectType == D_SECT_TYPE_COMPO_G )
	{
		// Do Nothing
	}
	else ASSERT(0);

	// 2. Section Stiffness
	//PrintDesignInfo_SectionStiffness(pXL, iStRow, strBaseSheet, RptData,SpscSect);

}

void CRptPlateGirder_IRC22::PrintDesignInfo_SectionSize(CMSExcel* pXL, int& iStRow, CString strBaseSheet, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, T_SECT_STIFFNESS& SpscSect)
{
	if ( m_nSectType == D_SECT_TYPE_COMPO_I )
	{
		PrintDesignInfo_SectionImage_Compo_I(pXL, iStRow, strBaseSheet);
		PrintDesignInfo_SectionSize_Compo_I(pXL, iStRow, strBaseSheet, RptData, SpscSect);
	}
	else if ( m_nSectType == D_SECT_TYPE_COMPO_B )
	{
		PrintDesignInfo_SectionImage_Compo_Box(pXL, iStRow, strBaseSheet);
		PrintDesignInfo_SectionSize_Compo_Box(pXL, iStRow, strBaseSheet, RptData, SpscSect);
	}
	else if ( m_nSectType == D_SECT_TYPE_COMPO_TUB )
	{
		PrintDesignInfo_SectionImage_Compo_Tub(pXL, iStRow, strBaseSheet);
		PrintDesignInfo_SectionSize_Compo_Tub(pXL, iStRow, strBaseSheet, RptData, SpscSect);
	}
	else
	{
		ASSERT(0);
	}
}

void CRptPlateGirder_IRC22::PrintDesignInfo_SectionImage_Compo_I(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_H_Shape_Image"), iRowCnt, iColCnt);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintDesignInfo_SectionImage_Compo_Box(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_Box_Shape_Image"), iRowCnt, iColCnt);

	iStRow += iRowCnt + 1;
}
void CRptPlateGirder_IRC22::PrintDesignInfo_SectionImage_Compo_Tub(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_Tub_Shape_Image"), iRowCnt, iColCnt);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintDesignInfo_SectionSize_Compo_I(CMSExcel* pXL, int& iStRow, CString strBaseSheet, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, T_SECT_STIFFNESS& SpscSect)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	double dh   = m_dSize[0]; // C, webÀÇ ³ôÀÌ.
	double dtw  = m_dSize[1];
	double dB1  = m_dSize[2];
	double dtf1 = m_dSize[3];
	double dB2  = m_dSize[4];
	double dtf2 = m_dSize[5];
	if ( dB2 < m_dZero ) { dB2  = dB1; }
	if ( dtf2< m_dZero ) { dtf2 = dtf1; }

	// 	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_H_Shape_Size"), iRowCnt, iColCnt);
	// 
	// 	pXL->SetValue(iStRow+2, 4, m_dBc);
	// 	pXL->SetValue(iStRow+2,13, m_dtc);
	// 	pXL->SetValue(iStRow+2,22, m_dHh);
	// 	pXL->SetValue(iStRow+4, 4, dh);
	// 	pXL->SetValue(iStRow+4,13, dB1);
	// 	pXL->SetValue(iStRow+4,22, dB2);
	// 	pXL->SetValue(iStRow+5, 4, dtw);
	// 	pXL->SetValue(iStRow+5,13, dtf1);
	// 	pXL->SetValue(iStRow+5,22, dtf2);

	iStRow += iRowCnt + 1;

	CString RptBlock;
	//RptBlock = _T(_T("I_P_00")      );   SetValue_I_P_00  (pExcel, RptBlock, ResData.Memb, stRow);
	RptBlock = _T("I_P_01") ;  // SetValue_I_P_01  (pXL, RptBlock, ResData.Memb, stRow);
	 //int rowCnt = CopyRange(pXL, RptBlock, iStRow);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("I_P_01"), iRowCnt, iColCnt);

	short Row_Ref = 0, Col_Ref = 0;
	// ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
	pXL->GetCellIDByName(m_strBaseBook, strBaseSheet, RptBlock, Row_Ref, Col_Ref);

	m_strBaseSheet = strBaseSheet;
	Row_Ref += 2; Col_Ref-=1;
	SetValue_I_P_01(pXL, RptBlock, Row_Ref, Col_Ref, iRowCnt, RptData, SpscSect);



	RptBlock = _T("I_P_Sect") ;
	//    pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("I_P_Sect"), iRowCnt, iColCnt);
	//    pXL->GetCellIDByName(m_strBaseBook, strBaseSheet, RptBlock, Row_Ref, Col_Ref);
	//    m_strBaseSheet = strBaseSheet;
		// SetValue_I_P_Sect(pXL,RptBlock, Row_Ref, Col_Ref ,iRowCnt, RptData,SpscSect );
	iRowCnt+=Row_Ref - 6;
	SetValue_I_P_Sect(pXL, RptBlock, RptData, iRowCnt);
	RptBlock = _T("I_P_02_Title");   SetValue_I_P_02_Title(pXL, RptBlock, iRowCnt);
	// RptBlock = _T(_T("I_P_02_01")   );   SetValue_I_P_02_01   (pXL, RptBlock,RptData, iRowCnt); // - ¼³°èºÎÀç·Â ¹× ¹ß»ýÀÀ·Â 

	iStRow = iRowCnt +1;
}

void CRptPlateGirder_IRC22::SetValue_I_P_01(CMSExcel* pExcel, CString& RptBlock, short &Row_Ref, short &Col_Ref, int& stRow, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, T_SECT_STIFFNESS& SpscSect)
{

	//   int rowCnt = CopyRange(pExcel, RptBlock, stRow);
	// 
	//  short Row_Ref = 0, Col_Ref = 0; 
	//   // ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
	 //  pExcel->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);
	// 
	// //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Title01p") ,  GetTitle(m_nTitle), Row_Ref, Col_Ref, stRow);
	// //   m_nTitle +=1;
	// 
	// //   CString strSectBlock = _T("Section");
	// //   AddGuideandSectPicture(pExcel, RptBlock, strSectBlock, *m_pMemb, RptData, stRow);
	// 
		//Slab Properties
		//stRow+=3;
		// pExcel->SetValue(stRow, 5, m_dBc);++stRow;//B_s
	//   pExcel->SetValue(stRow, 5, m_dtc);++stRow;//t_s
	//   pExcel->SetValue(stRow, 5, m_dHh);++stRow;//t_h
	//   pExcel->SetValue(stRow, 5, m_dfc);++stRow;//fck
	//   pExcel->SetValue(stRow, 5, m_dEcm);++stRow;//E_c
	//   pExcel->SetValue(stRow, 5, m_dArb);++stRow;//A_r
	//   pExcel->SetValue(stRow, 5, m_dfy);++stRow;//F_yk
	//   pExcel->SetValue(stRow, 5, m_dBc*m_dBeffFactor);++stRow;
	// 
	//   //Girder Properties
	//   stRow+=2;
	//   pExcel->SetValue(stRow, 5, m_dbfc);pExcel->SetValue(stRow, 12, m_dbft);++stRow;
	//   pExcel->SetValue(stRow, 5, m_dtfc);pExcel->SetValue(stRow, 12, m_dtft);++stRow;
	//   pExcel->SetValue(stRow, 5,  m_dHw);pExcel->SetValue(stRow, 12, m_dtw);++stRow;
	// 
	//   //Table 
	//   stRow+=2;CString strCompFlgThk = _T("");
	//   pExcel->SetValue(stRow, 8, m_strMatNameSteel);pExcel->SetValue(stRow, 12, m_dtfc);pExcel->SetValue(stRow, 15, m_dfy);pExcel->SetValue(stRow, 18, m_dFu);
	//   strCompFlgThk = (m_dtfc < 20.0) ? _T("Less Than 20mm") : _T("Greater Than 20mm") ; pExcel->SetValue(stRow, 21, strCompFlgThk);++stRow;
	//   pExcel->SetValue(stRow, 8, m_strMatNameSteel);pExcel->SetValue(stRow, 12, m_dtft);pExcel->SetValue(stRow, 15, m_dfy);pExcel->SetValue(stRow,18, m_dFu);
	//   strCompFlgThk = (m_dtft < 20.0) ? _T("Less Than 20mm") : _T("Greater Than 20mm") ; pExcel->SetValue(stRow,21, strCompFlgThk);++stRow;
	//   pExcel->SetValue(stRow, 8, m_strMatNameSteel);pExcel->SetValue(stRow, 12, m_dtw);pExcel->SetValue(stRow, 15, m_dfy);pExcel->SetValue(stRow, 18, m_dFu);
	//   strCompFlgThk = (m_dtw < 20.0) ? _T("Less Than 20mm") : _T("Greater Than 20mm") ; pExcel->SetValue(stRow, 21, strCompFlgThk);++stRow;
	//   
		//   // ½½·¡ºê Àç¿ø
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Bs"), RptData.Memb.dBs, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("ts"), RptData.Memb.dts, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("th"), RptData.Memb.dth, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("fc"), RptData.Memb.dfck, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Ec"), RptData.Memb.dEc, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Ar"), RptData.Memb.dAr, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Fyr"), RptData.Memb.dFyr, Row_Ref, Col_Ref, stRow);
	//PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Beff") ,  RptData.Memb.dBs* m_dBeffFactor         , Row_Ref, Col_Ref, stRow);
	pExcel->SetValue(stRow+9, 5, m_dBc*m_dBeffFactor);//Beff
// 
//   // °Å´õ-´Ü¸é
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("bfc"), RptData.Memb.GIRDER.dbfc, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("bft"), RptData.Memb.GIRDER.dbft, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("tfc"), RptData.Memb.GIRDER.dtfc, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("tft"), RptData.Memb.GIRDER.dtft, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("D"), RptData.Memb.GIRDER.dD, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("tw"), RptData.Memb.GIRDER.dtw, Row_Ref, Col_Ref, stRow);
	// 
	//   // °Å´õ-»óºÎÇÃ·£Áö
	CString strCompFlgThk = _T("");
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Top_mat"), m_strMatNameSteel, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Top_thk"), RptData.Memb.GIRDER.Top.dthick, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Top_fyo"), RptData.Memb.GIRDER.Top.dfy_or, Row_Ref, Col_Ref, stRow);
	//PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Top_fy")  ,  RptData.GIRDER.Top.dfy       , Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Top_fu"), RptData.Memb.GIRDER.Top.dfu, Row_Ref, Col_Ref, stRow);
	strCompFlgThk = (m_dtfc < 20.0) ? _T("Less Than 20mm") : _T("Greater Than 20mm"); pExcel->SetValue(stRow+17, 21, strCompFlgThk);
	//PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("W56"),  (m_dtfc < 20.0) ? _T("Less Than 20mm") : _T("Greater Than 20mm")    , Row_Ref, Col_Ref, stRow);
// 
//   // °Å´õ-ÇÏºÎÇÃ·£Áö
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Bot_mat"), m_strMatNameSteel, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Bot_thk"), RptData.Memb.GIRDER.Bottom.dthick, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Bot_fyo"), RptData.Memb.GIRDER.Bottom.dfy_or, Row_Ref, Col_Ref, stRow);
	//PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Bot_fy")  ,  RptData.GIRDER.Bottom.dfy       , Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Bot_fu"), RptData.Memb.GIRDER.Bottom.dfu, Row_Ref, Col_Ref, stRow);
	strCompFlgThk = (m_dtft < 20.0) ? _T("Less Than 20mm") : _T("Greater Than 20mm"); pExcel->SetValue(stRow+18, 21, strCompFlgThk);
	//PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Bot_memo"),  RptData.GIRDER.Bottom.strMemo   , Row_Ref, Col_Ref, stRow);
// 
//   // °Å´õ-ÇÏºÎÇÃ·£Áö
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Web_mat"), m_strMatNameSteel, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Web_thk"), RptData.Memb.GIRDER.Web.dthick, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Web_fyo"), RptData.Memb.GIRDER.Web.dfy_or, Row_Ref, Col_Ref, stRow);
	//PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Web_fy")  ,  RptData.GIRDER.Web.dfy       , Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Web_fu"), RptData.Memb.GIRDER.Web.dfu, Row_Ref, Col_Ref, stRow);
	strCompFlgThk = (m_dtw < 20.0) ? _T("Less Than 20mm") : _T("Greater Than 20mm"); pExcel->SetValue(stRow+19, 21, strCompFlgThk);
	//PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Web_memo"),  RptData.GIRDER.Web.strMemo   , Row_Ref, Col_Ref, stRow);
// 
//   // °Å´õ- ¼³°è°­µµ
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Fyc"), RptData.Memb.GIRDER.dFyc, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Fyw"), RptData.Memb.GIRDER.dFyw, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Fyt"), RptData.Memb.GIRDER.dFyt, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Es"), RptData.Memb.GIRDER.dEs, Row_Ref, Col_Ref, stRow);
	pExcel->SetValue(stRow+26, 5, RptData.Memb.GIRDER.dSlen);//Gs


	// ¼öÁ÷º¸°­Àç- !!!¼öÁ¤ÇÊ¿ä
	CString strPos = _ULS('WEB');
	CString strType[2] = { _ULS('1 Side'), _ULS('2 Side') };
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_T_posi"), strPos, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_T_type"), strType[RptData.Memb.STIFFENER_V.nType], Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_T_fy"), RptData.Memb.STIFFENER_V.dfy, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_T_H"), RptData.Memb.STIFFENER_V.dhi, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_T_B"), RptData.Memb.STIFFENER_V.dbi, Row_Ref, Col_Ref, stRow);
	//   if (RptData.STIFFENER_V.nShapeType == 0)
	//   {
	//     PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_T_tw")   ,  _T("-")  , Row_Ref, Col_Ref, stRow);
	//     PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_T_tf")   ,  _T("-")  , Row_Ref, Col_Ref, stRow);
	//   }
	//   else
	//   {
	//     PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_T_tw")   ,  RptData.STIFFENER_V.dtw  , Row_Ref, Col_Ref, stRow);
	//     PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_T_tf")   ,  RptData.STIFFENER_V.dti  , Row_Ref, Col_Ref, stRow); 
	//   }
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_T_do"), RptData.Memb.STIFFENER_V.ddo, Row_Ref, Col_Ref, stRow);
	// 
	// 
	//   // ¼öÆòº¸°­Àç- !!!¼öÁ¤ÇÊ¿ä
	//   //   CString strPos2[3]  = { _LS(IDS_CVL_RPT_US_Web),    _LS(IDS_CVL_RPT_US_Top_Flange), _LS(IDS_CVL_RPT_US_Bottom_Flange)};
	//   //   CString srtShape[3] = { _T("I-type"), _T("T-type"),     _T("U-type")};
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_L_posi") ,  strPos2[RptData.STIFFENER_L.nplace], Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_L_group"),  _T("-")                                       , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_L_type") ,  srtShape[RptData.STIFFENER_L.nType], Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_L_fy")   ,  RptData.STIFFENER_L.dfy            , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_L_bl")   ,  RptData.STIFFENER_L.dbi            , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_L_tl")   ,  RptData.STIFFENER_L.dti            , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_L_hl")   ,  RptData.STIFFENER_L.dhi            , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_L_tw")   ,  RptData.STIFFENER_L.dtw            , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stiff_L_Num")  ,  0/*°³¼ö*/                                     , Row_Ref, Col_Ref, stRow);
	// 
	//  stRow+=(rowCnt+1);
}

void CRptPlateGirder_IRC22::SetValue_I_P_Sect(CMSExcel* pExcel, CString& RptBlock, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, int& stRow)
{
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);

	short Row_Ref = 0, Col_Ref = 0;
	// ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
	pExcel->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);

	// ´Ü¸é°è¼ö- °­Àç´Ü¸é
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("A"), RptData.Memb.Steel.dA, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("I"), RptData.Memb.Steel.dINA, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Ina"), RptData.Memb.Steel.dIzz, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("dtop"), RptData.Memb.Steel.ddTop, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stop"), RptData.Memb.Steel.dSTop, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("dbot"), RptData.Memb.Steel.ddBot, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Sbot"), RptData.Memb.Steel.dSBot, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("dit"), RptData.Memb.Steel.dI, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("ds"), RptData.Memb.Steel.dds, Row_Ref, Col_Ref, stRow);

	// ´Ü¸é°è¼ö- ´Ü±âÇÕ¼º´Ü¸é
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("A_n"), RptData.Memb.ShortComp.dA, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("I_n"), RptData.Memb.ShortComp.dINA, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Ina_n"), RptData.Memb.ShortComp.dIzz, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("dtop_n"), RptData.Memb.ShortComp.ddTop, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stop_n"), RptData.Memb.ShortComp.dSTop, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("dbot_n"), RptData.Memb.ShortComp.ddBot, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Sbot_n"), RptData.Memb.ShortComp.dSBot, Row_Ref, Col_Ref, stRow);
	//PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("ds_n")   ,  RptData.ShortComp.dds      , Row_Ref, Col_Ref, stRow);

	// ´Ü¸é°è¼ö- Àå±âÇÕ¼º´Ü¸é
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("A_3n"), RptData.Memb.LongComp.dA, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("I_3n"), RptData.Memb.LongComp.dINA, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Ina_3n"), RptData.Memb.LongComp.dIzz, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("dtop_3n"), RptData.Memb.LongComp.ddTop, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Stop_3n"), RptData.Memb.LongComp.dSTop, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("dbot_3n"), RptData.Memb.LongComp.ddBot, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Sbot_3n"), RptData.Memb.LongComp.dSBot, Row_Ref, Col_Ref, stRow);
	//PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("ds_3n")   ,  RptData.LongComp.dds      , Row_Ref, Col_Ref, stRow);
	double dl = m_dBc/m_dmodularratiopermt;
	double dy = m_dH + 0.5*m_dtc;
	m_dAYbyILongTerm =  dl*m_dtc*(dy-RptData.Memb.LongComp.ddBot)/RptData.Memb.LongComp.dINA;
	dl = m_dBc/m_dmodularratiotransient;
	m_dAYbyIShortTerm = dl*m_dtc*(dy-RptData.Memb.ShortComp.ddBot + m_dtf2*0.5)/RptData.Memb.ShortComp.dINA;

	stRow+=(rowCnt+1);
}

void CRptPlateGirder_IRC22::SetValue_I_P_02_Title(CMSExcel* pExcel, CString& RptBlock, int& stRow)
{
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);
	short Row_Ref = 0, Col_Ref = 0;
	// ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
	pExcel->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("Title02"), _T(""), Row_Ref, Col_Ref, stRow);
	stRow += rowCnt;
	/*m_nTitle +=1;*/
}

void CRptPlateGirder_IRC22::SetValue_I_P_02_01(CMSExcel* pExcel, CString& RptBlock, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, int& stRow)
{
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);

	short Row_Ref = 0, Col_Ref = 0;
	// ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
	pExcel->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);
	m_nRowNo = Row_Ref;
	// ¼³°èºÎÀç·Â ¹× ÀÀ·Â
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_Lcom")      ,  RptData.strLcomName                   , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_Lcom_Type") ,  RptData.strLcomType                   , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_MD1")       ,  RptData.dMu_gr      * m_dFactorMoment , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_MD2")       ,  RptData.dMu_lt      * m_dFactorMoment , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_MD3")       ,  RptData.dMu_st      * m_dFactorMoment , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_Mu")        ,  (RptData.dMu_gr+ RptData.dMu_lt+RptData.dMu_st ) * m_dFactorMoment , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_Vu")        ,  RptData.dVu         * m_dFactorForce  , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_Tu")        ,  RptData.dTu         * m_dFactorMoment , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_ft_MD1")    ,  RptData.dft_gr      , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_ft_MD2")    ,  RptData.dft_lt      , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_ft_MD3")    ,  RptData.dft_st      , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_ft_Mu")     ,  RptData.dft         , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_fb_MD1")    ,  RptData.dfb_gr      , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_fb_MD2")    ,  RptData.dfb_lt      , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_fb_MD3")    ,  RptData.dfb_st      , Row_Ref, Col_Ref, stRow);
//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_fb_Mu")     ,  RptData.dfb         , Row_Ref, Col_Ref, stRow);

	stRow+=10;
	m_nRowNo = stRow;
}

void CRptPlateGirder_IRC22::PrintDesignInfo_SectionSize_Compo_Box(CMSExcel* pXL, int& iStRow, CString strBaseSheet, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, T_SECT_STIFFNESS& SpscSect)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	double dh		= m_dSize[0]; // C, webÀÇ ³ôÀÌ.
	double dtw		= m_dSize[1];
	double dB1		= m_dSize[2];
	double dtf1		= m_dSize[4];
	double dB2		= m_dSize[5];
	double dtf2		= m_dSize[7];
	double dBf1		= m_dSize[3];
	double dBf2		= m_dSize[6];
	if ( dB2 < m_dZero ) { dB2  = dB1; }
	if ( dtf2< m_dZero ) { dtf2 = dtf1; }

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_Box_Shape_Size"), iRowCnt, iColCnt);

	pXL->SetValue(iStRow+2, 4, m_dBc);
	pXL->SetValue(iStRow+2, 13, m_dtc);
	pXL->SetValue(iStRow+2, 22, m_dHh);
	pXL->SetValue(iStRow+4, 4, dh);
	pXL->SetValue(iStRow+4, 13, dB1);
	pXL->SetValue(iStRow+4, 22, dB2);
	pXL->SetValue(iStRow+5, 4, dtw);
	pXL->SetValue(iStRow+5, 13, dtf1);
	pXL->SetValue(iStRow+5, 22, dtf2);
	pXL->SetValue(iStRow+6, 13, dBf1);
	pXL->SetValue(iStRow+6, 22, dBf2);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintDesignInfo_SectionSize_Compo_Tub(CMSExcel* pXL, int& iStRow, CString strBaseSheet, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, T_SECT_STIFFNESS& SpscSect)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	double dh		= m_dSize[0]; // C, webÀÇ ³ôÀÌ.
	double dtw		= m_dSize[1];
	double dB1		= m_dSize[2];
	double dtf1		= m_dSize[4];
	double dB2		= m_dSize[5];
	double dtf2		= m_dSize[7];
	double dBf1		= m_dSize[3];
	double dBf2		= m_dSize[6];
	double dBf3		= m_dSize[8];
	if ( dB2 < m_dZero ) { dB2  = dB1; }
	if ( dtf2< m_dZero ) { dtf2 = dtf1; }

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_Tub_Shape_Size"), iRowCnt, iColCnt);

	pXL->SetValue(iStRow+2, 4, m_dBc);
	pXL->SetValue(iStRow+2, 13, m_dtc);
	pXL->SetValue(iStRow+2, 22, m_dHh);
	pXL->SetValue(iStRow+4, 4, dh);
	pXL->SetValue(iStRow+4, 13, dB1);
	pXL->SetValue(iStRow+4, 22, dB2);
	pXL->SetValue(iStRow+5, 4, dtw);
	pXL->SetValue(iStRow+5, 13, dtf1);
	pXL->SetValue(iStRow+5, 22, dtf2);
	pXL->SetValue(iStRow+6, 4, dBf1);
	pXL->SetValue(iStRow+6, 13, dBf2);
	pXL->SetValue(iStRow+6, 22, dBf3);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintDesignInfo_SectionStiffness(CMSExcel* pXL, int& iStRow, CString strBaseSheet, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, T_SECT_STIFFNESS& SpscSect)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_Stiffness"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+2, 4, m_dArea_g);
	pXL->SetValue(iStRow+3, 4, m_dIyy_g);
	pXL->SetValue(iStRow+4, 4, m_dIzz_g);
	pXL->SetValue(iStRow+5, 4, m_dYbar_g);
	pXL->SetValue(iStRow+6, 4, m_dZbar_g);
	pXL->SetValue(iStRow+7, 4, m_dIw);
	pXL->SetValue(iStRow+8, 4, m_dIt);
	//
	pXL->SetValue(iStRow+2, 18, m_dArea);
	pXL->SetValue(iStRow+3, 18, m_dIyy);
	pXL->SetValue(iStRow+4, 18, m_dIzz);
	pXL->SetValue(iStRow+5, 18, m_dYbar);
	pXL->SetValue(iStRow+6, 18, m_dZbar);
	//
	pXL->SetValue(iStRow+ 11, 18, m_dArea_cr);
	pXL->SetValue(iStRow+12, 18, m_dIyy_cr);
	pXL->SetValue(iStRow+13, 18, m_dIzz_cr);
	pXL->SetValue(iStRow+14, 18, m_dYbar_cr);
	pXL->SetValue(iStRow+15, 18, m_dZbar_cr);
	pXL->SetValue(iStRow+16, 18, m_dIw);
	pXL->SetValue(iStRow+17, 18, m_dIt);
	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintDesignInfo_Material(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Material_Data"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+1, 5, m_dfy);
	pXL->SetValue(iStRow+4, 5, m_dfck);
	pXL->SetValue(iStRow+7, 5, m_dfsk);
	pXL->SetValue(iStRow+1, 15, m_dEs);
	pXL->SetValue(iStRow+4, 15, m_dEcm);
	pXL->SetValue(iStRow+7, 15, m_dErs);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintBendingResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int nPosiNega, T_PGBR_BASE& PgbrGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail)
{
	//   int iRowCnt = 0;
	//   int iColCnt = 0;
	// // 	dMpl_Rd = PgbrBase.dMpl_Rd*m_dFactorMoment;
	//    CString strBaseSheet = _T("Bending_Resistance");
	// //   pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Title_of_BendingResistance"), iRowCnt, iColCnt);
	// //   pXL->SetValue(iStRow, 1, m_iBendingChapterIx);
	// //   iStRow += iRowCnt;
	// 
	//   short Row_Ref = 0, Col_Ref = 0; 
	//   CString RptBlock = _T(_T("I_P_02_01")   );
	//   // ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
	//   pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);
	//   CString strSubChapter=_T("");
	//   // Positive Moment
	//   if(nPosiNega==1) // Positive
	//   {
	//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Bending_Positive"), iRowCnt, iColCnt);    
	//   }
	//   else
	//   {    
	//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Bending_Negative"), iRowCnt, iColCnt);    
	//   }
	//   strSubChapter.Format(_T("%d.%d"),m_iBendingChapterIx, iChapIndex);
	//   pXL->SetValue(iStRow, 1, strSubChapter);    
	//   iStRow += iRowCnt;
	// //   
	//    CString strLcomName = GetLoadCombinationName(PgbrBase.OrgLcomK);
	// //   pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Design_Force"), iRowCnt, iColCnt);
	// //   pXL->SetValue(iStRow+1,10, strLcomName);
	// //   pXL->SetValue(iStRow+2, 5, PgbrGd.dFx*m_dFactorForce);
	// //   pXL->SetValue(iStRow+3, 5, PgbrBase.dFx*m_dFactorForce);
	// //   pXL->SetValue(iStRow+4, 5, PgbrBase.dMa_Ed*m_dFactorMoment);
	// //   pXL->SetValue(iStRow+5, 5, PgbrBase.dMc_Ed*m_dFactorMoment);
	//   
	//   
	//    Row_Ref = Row_Ref-13;
	// 
	// 
	//      PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("F_Lcom")      ,  strLcomName      , Row_Ref, Col_Ref, iStRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_Lcom_Type") ,  RptData.strLcomType                   , Row_Ref, Col_Ref, stRow);
	//      PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("F_MD1")       ,   PgbrGd.dFx*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_MD2")       ,  RptData.dMu_lt      * m_dFactorMoment , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_MD3")       ,  RptData.dMu_st      * m_dFactorMoment , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_Mu")        ,  (RptData.dMu_gr+ RptData.dMu_lt+RptData.dMu_st ) * m_dFactorMoment , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_Vu")        ,  RptData.dVu         * m_dFactorForce  , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_Tu")        ,  RptData.dTu         * m_dFactorMoment , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_ft_MD1")    ,  RptData.dft_gr      , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_ft_MD2")    ,  RptData.dft_lt      , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_ft_MD3")    ,  RptData.dft_st      , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_ft_Mu")     ,  RptData.dft         , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_fb_MD1")    ,  RptData.dfb_gr      , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_fb_MD2")    ,  RptData.dfb_lt      , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_fb_MD3")    ,  RptData.dfb_st      , Row_Ref, Col_Ref, stRow);
	//   //   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_fb_Mu")     ,  RptData.dfb         , Row_Ref, Col_Ref, stRow);
	// 
	// 
	//   iStRow += iRowCnt + 1;
	// 
	//   pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Bending_Stress"), iRowCnt, iColCnt); 
	//   iStRow += iRowCnt;
	// 
	//   if(m_nSectType==D_SECT_TYPE_COMPO_I || m_nSectType==D_SECT_TYPE_COMPO_B || m_nSectType==D_SECT_TYPE_COMPO_TUB)
	//   {    
	//     PrintStressInformationDB(pXL, iStRow, strBaseSheet, PgbrDetail.StressInfoHshapeD);    
	//   }
	//   else if(m_nSectType==D_SECT_TYPE_COMPO_G)
	//   {
	//     PrintStressInformation(pXL, iStRow, strBaseSheet, PgbrDetail.GLineDgn, PgbrDetail.StressInfo4Class);    
	//   }
	//   else ASSERT(0);  
	//   //Print Section Properties again
	// 
	// 
	//   // Classification of sections
	// 
	//   PrintClassificationofSectionDetail(pXL, iStRow, PgbrBase.iTopClass, PgbrBase.iWebClass, PgbrBase.iBotClass, PgbrBase.iSectClass,PgbrBase.dTopClassRat,PgbrBase.dBotClassRat,PgbrBase.dWebClassRat);  
	// 
	// 	 iStRow += iRowCnt + 1;
	// 	double dEtaIRC = sqrt(250.0/m_dfy);
	// 	//Minimum Web Thickness
	// 	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_Calculation"), iRowCnt, iColCnt);
	// 	
	// 	if (PgbrBase.bTrSt)
	// 	{
	// 		//Transverse stiffeners provided :YES
	// 		//double dEtaFact=270.0;
	// 
	// // 		pXL->SetValue(iStRow+1 , 1, _T("'- Minimum Web and Flange Thickness"));
	// // 		pXL->SetValue(iStRow+2 , 1, _T("'Transverse Stiffeners Provided :"));
	// 		pXL->SetValue(iStRow+1 , 13, _T("YES"));
	// 		pXL->SetValue(iStRow+2 , 13, PgbrBase.dTRSTPitch);
	// // 		pXL->SetValue(iStRow+4 , 1, _T("'Condition :"));pXL->SetValue(iStRow+4 , 9, _T("'d/tw  <="));pXL->SetValue(iStRow+4 , 14, _T("270*Eta"));pXL->SetValue(iStRow+4 , 19, _T("IRC24-2010 Cl.509.6.1"));
	// // 		CString dstrprint = (PgbrBase.dWebClassRat<=dEtaFact*dEtaIRC)?_T("<"):_T(">");
	// 		pXL->SetValue(iStRow+4 , 23, PgbrBase.nCondStage1);//pXL->SetValue(iStRow+5 , 11, dstrprint);pXL->SetValue(iStRow+5 , 14, dEtaFact*dEtaIRC);
	// 		pXL->SetValue(iStRow+4 , 27, (PgbrBase.bCondStage1==TRUE? _T("OK") : _T("NG")));
	// 		pXL->SetValue(iStRow+10 , 23, PgbrBase.nCondStage2);
	// 		pXL->SetValue(iStRow+10 , 27, (PgbrBase.bCondStage2==TRUE? _T("OK") : _T("NG")));
	// 
	// // 		pXL->SetValue(iStRow+6 , 1, _T("'Condition :"));pXL->SetValue(iStRow+6 , 9, _T("'d/tw  <="));pXL->SetValue(iStRow+6 , 14, _T("345*Eta_f"));pXL->SetValue(iStRow+6 , 19, _T("IRC24-2010 Cl.509.6.2"));
	// // 		dstrprint = (PgbrBase.dWebClassRat<=345.0*dEtaIRC)?_T("<"):_T(">");
	// // 		pXL->SetValue(iStRow+7 , 9, PgbrBase.dWebClassRat);pXL->SetValue(iStRow+7 , 11, dstrprint);pXL->SetValue(iStRow+7 , 14, 345.0*dEtaIRC);
	// 
	// 		iStRow += 17;
	// 	}
	// 	else
	// 	{
	// 		pXL->SetValue(iStRow+1 , 13, _T("NO"));
	// 		pXL->SetValue(iStRow+2 , 13, PgbrBase.dTRSTPitch);
	// 		// 		pXL->SetValue(iStRow+4 , 1, _T("'Condition :"));pXL->SetValue(iStRow+4 , 9, _T("'d/tw  <="));pXL->SetValue(iStRow+4 , 14, _T("270*Eta"));pXL->SetValue(iStRow+4 , 19, _T("IRC24-2010 Cl.509.6.1"));
	// 		// 		CString dstrprint = (PgbrBase.dWebClassRat<=dEtaFact*dEtaIRC)?_T("<"):_T(">");
	// 		pXL->SetValue(iStRow+4 , 23, PgbrBase.nCondStage1);//pXL->SetValue(iStRow+5 , 11, dstrprint);pXL->SetValue(iStRow+5 , 14, dEtaFact*dEtaIRC);
	// 		pXL->SetValue(iStRow+4 , 27, (PgbrBase.bCondStage1==TRUE? _T("OK") : _T("NG")));
	// 		pXL->SetValue(iStRow+10 , 23, PgbrBase.nCondStage2);
	// 		pXL->SetValue(iStRow+10 , 27, (PgbrBase.bCondStage2==TRUE? _T("OK") : _T("NG")));
	// 
	// 		iStRow += 17;
	// 	}
	// 
	//   int iClassSect = PgbrBase.iSectClass;
	//   if(iClassSect==1 || iClassSect==2) // Plastic
	//   {
	//     if(PgbrBase.bEquiClass2)
	//     {
	//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Plastic_Moment_Eff_NAxis"), iRowCnt, iColCnt);
	//       pXL->SetValue(iStRow+1 , 7, PgbrBase.dPlasticNA);
	//       pXL->SetValue(iStRow+2 , 9, PgbrBase.dPlasticNAEff);
	//     }
	//     else
	//     {
	//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Plastic_Moment_NAxis"), iRowCnt, iColCnt);
	//       pXL->SetValue(iStRow+1 , 7, PgbrBase.dPlasticNA);
	//     }
	//     iStRow += iRowCnt + 1;
	//     
	//     if(nPosiNega==1) // Positive
	//     {
	//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Plastic_Moment_Positive_Param"), iRowCnt, iColCnt);
	//       pXL->SetValue(iStRow+0 , 5, PgbrDetail.dN_slab*m_dFactorForce);
	//       pXL->SetValue(iStRow+1 , 5, PgbrDetail.dNg_top*m_dFactorForce);
	//       pXL->SetValue(iStRow+2 , 5, PgbrDetail.dNg_bot*m_dFactorForce);
	//       pXL->SetValue(iStRow+4 , 5, PgbrDetail.dMpl_Rd_cal*m_dFactorMoment);
	//       pXL->SetValue(iStRow+5 , 5, PgbrBase.dXpl);
	//       pXL->SetValue(iStRow+6 ,10, PgbrBase.dM_Rd*m_dFactorMoment);
	//       pXL->SetValue(iStRow+7, 7, PgbrBase.dBeta);
	//       iStRow += iRowCnt + 1;
	// 
	//       // if xpl/h>0.4ÀÌ¸é Class 1,2 ¿¡ ´ëÇØ¼­µµ Elastic¿¡ ´ëÇØ °ËÅäÇÑ´Ù
	//       if(PgbrBase.dXplOverh>0.4)
	//       {
	//         pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Elastic_resistance_moment"), iRowCnt, iColCnt);        
	//         iStRow += iRowCnt;
	//         pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_Xpl_h_check_MoreThan"), iRowCnt, iColCnt);        
	//         pXL->SetValue(iStRow, 5, PgbrBase.dXplOverh);
	//         iStRow += iRowCnt;        
	//         // Modify by GAY. PMS:4216. ('12.05.16). Modify output format for k factor.
	//         pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_Positive"), iRowCnt, iColCnt);
	//         pXL->SetValue(iStRow  ,  4, PgbrBase.dk_min);
	//         iStRow += iRowCnt;
	//         //if(PgbrBase.dk_min>=0.0)
	//         {
	//           pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_by_k_min"), iRowCnt, iColCnt);
	//           pXL->SetValue(iStRow+0, 12, PgbrBase.dMel_Rd*m_dFactorMoment);
	//           iStRow += iRowCnt;
	//         }        
	//         iStRow++;
	//         pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_bytElastic_resistance"), iRowCnt, iColCnt);
	//         pXL->SetValue(iStRow, 8, PgbrBase.dMel_Rd*m_dFactorMoment);
	//         iStRow += iRowCnt + 1;
	// 
	//         if(PgbrBase.dNc<0.0) // ¾ÐÃà·ÂÀÌ ÀÖÀ»¶§ Àû¿ëÇÑ´Ù. 
	//         {
	//           if(fabs(PgbrBase.dNc) < PgbrBase.dNc_el)
	//           {
	//             pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_Class1_2_Pos_LT_Ncel"), iRowCnt, iColCnt);
	//             pXL->SetValue(iStRow  , 5, PgbrBase.dNc   *m_dFactorForce);
	//             pXL->SetValue(iStRow+1, 5, PgbrBase.dNc_el*m_dFactorForce);
	//             pXL->SetValue(iStRow+2,16, PgbrBase.dM_Rd *m_dFactorMoment);
	//             iStRow += iRowCnt + 1;
	//           }
	//           else if(fabs(PgbrBase.dNc) < PgbrBase.dNc_f)
	//           {
	//             pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_Class1_2_Pos_GT_Ncel"), iRowCnt, iColCnt);
	//             pXL->SetValue(iStRow  , 5, PgbrBase.dNc   *m_dFactorForce);
	//             pXL->SetValue(iStRow+1, 5, PgbrBase.dNc_el*m_dFactorForce);
	//             pXL->SetValue(iStRow+2, 5, PgbrBase.dNc_f *m_dFactorForce);
	//             pXL->SetValue(iStRow+3,16, PgbrBase.dM_Rd *m_dFactorMoment);
	//             iStRow += iRowCnt + 1;
	//           }
	//           else ASSERT(0);        
	//         }
	//       }      
	//     }
	//     else if(nPosiNega==2)
	//     {
	//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Plastic_Moment_Negative_Param"), iRowCnt, iColCnt);
	//       pXL->SetValue(iStRow+0, 5, PgbrDetail.dN_slab*m_dFactorForce);
	//       pXL->SetValue(iStRow+1, 5, PgbrDetail.dNr_top*m_dFactorForce);
	//       pXL->SetValue(iStRow+2, 5, PgbrDetail.dNg_top*m_dFactorForce);
	//       pXL->SetValue(iStRow+3, 5, PgbrDetail.dNg_bot*m_dFactorForce);
	//       pXL->SetValue(iStRow+5, 5, PgbrBase.dMpl_Rd*m_dFactorMoment);
	//       pXL->SetValue(iStRow+6, 8, PgbrBase.dM_Rd*m_dFactorMoment);
	// 
	//       iStRow += iRowCnt + 1;
	//     }
	//     else ASSERT(0);
	//     
	//   }
	//   else if(iClassSect==3) // Elastic
	//   {
	//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Elastic_resistance_moment"), iRowCnt, iColCnt);
	//     iStRow += iRowCnt;
	//     // Modify by GAY. PMS:4216. ('12.05.16). Modify output format for k factor.
	//     CString strBlockName;
	//     if(nPosiNega==1) strBlockName = _T("Elastic_resistance_Positive");// Positive
	//     else             strBlockName = _T("Elastic_resistance_Negative");
	//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strBlockName, iRowCnt, iColCnt);
	//     pXL->SetValue(iStRow  ,  4, PgbrBase.dk_min);
	// 
	//     iStRow += iRowCnt;
	//     //if(PgbrBase.dk_min>=0.0)
	//     {
	//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_by_k_min"), iRowCnt, iColCnt);
	//       pXL->SetValue(iStRow+0, 12, PgbrBase.dMel_Rd*m_dFactorMoment);
	//       iStRow += iRowCnt;
	//     }    
	//     iStRow++;
	//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_bytElastic_resistance"), iRowCnt, iColCnt);
	//     pXL->SetValue(iStRow, 8, PgbrBase.dMel_Rd*m_dFactorMoment);
	//     iStRow += iRowCnt + 1;
	//     
	//   }
	//   else if(iClassSect==4) // Effective
	//   {
	//     pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Effective_section"), iRowCnt, iColCnt);
	//     iStRow += iRowCnt;
	// 
	//     pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Effective_section_Fx"), iRowCnt, iColCnt);
	//     iStRow += iRowCnt;
	//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Effective_Section_Stiffness"), iRowCnt, iColCnt);
	//     pXL->SetValue(iStRow+1, 4, PgbrDetail.EffGirderFx.Area);
	//     pXL->SetValue(iStRow+2, 4, PgbrDetail.EffGirderFx.Ryy);
	//     pXL->SetValue(iStRow+3, 4, PgbrDetail.EffGirderFx.Rzz);
	//     pXL->SetValue(iStRow+4, 4, PgbrDetail.EffGirderFx.Cym);
	//     pXL->SetValue(iStRow+5, 4, PgbrDetail.EffGirderFx.Czm);
	//      //
	//     pXL->SetValue(iStRow+1,18, PgbrDetail.EffCompositeFx.Area);
	//     pXL->SetValue(iStRow+2,18, PgbrDetail.EffCompositeFx.Ryy);
	//     pXL->SetValue(iStRow+3,18, PgbrDetail.EffCompositeFx.Rzz);
	//     pXL->SetValue(iStRow+4,18, PgbrDetail.EffCompositeFx.Cym);
	//     pXL->SetValue(iStRow+5,18, PgbrDetail.EffCompositeFx.Czm);
	//     iStRow += iRowCnt + 1;
	// 
	//     pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Effective_section_My"), iRowCnt, iColCnt);
	//     iStRow += iRowCnt;
	//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Effective_Section_Stiffness"), iRowCnt, iColCnt);
	//     pXL->SetValue(iStRow+1, 4, PgbrDetail.EffGirder.Area);
	//     pXL->SetValue(iStRow+2, 4, PgbrDetail.EffGirder.Ryy);
	//     pXL->SetValue(iStRow+3, 4, PgbrDetail.EffGirder.Rzz);
	//     pXL->SetValue(iStRow+4, 4, PgbrDetail.EffGirder.Cym);
	//     pXL->SetValue(iStRow+5, 4, PgbrDetail.EffGirder.Czm);
	//      //
	//     pXL->SetValue(iStRow+1,18, PgbrDetail.EffComposite.Area);
	//     pXL->SetValue(iStRow+2,18, PgbrDetail.EffComposite.Ryy);
	//     pXL->SetValue(iStRow+3,18, PgbrDetail.EffComposite.Rzz);
	//     pXL->SetValue(iStRow+4,18, PgbrDetail.EffComposite.Cym);
	//     pXL->SetValue(iStRow+5,18, PgbrDetail.EffComposite.Czm);
	//     iStRow += iRowCnt + 1;
	//     
	//     pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Added_moment"), iRowCnt, iColCnt);
	//     iStRow += iRowCnt;
	//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Added_moment_calc"), iRowCnt, iColCnt);
	//     pXL->SetValue(iStRow  ,13, PgbrDetail.dDelMa_Ed*m_dFactorMoment);
	//     pXL->SetValue(iStRow+1,13, PgbrDetail.dDelMc_Ed*m_dFactorMoment);
	//     pXL->SetValue(iStRow+3, 6, PgbrBase.dSIGa_top);
	//     pXL->SetValue(iStRow+5, 6, PgbrBase.dSIGa_bot);
	//     iStRow += iRowCnt;
	// 
	//     CString strBlockName;
	//     if(nPosiNega==1) strBlockName = _T("Elastic_resistance_Positive");// Positive
	//     else             strBlockName = _T("Elastic_resistance_Negative");
	//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strBlockName, iRowCnt, iColCnt);
	//     pXL->SetValue(iStRow  ,  4, PgbrBase.dk_min);
	//     iStRow += iRowCnt;
	//     //if(PgbrBase.dk_min>=0.0)
	//     {
	//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_Class4by_k_min"), iRowCnt, iColCnt);
	//       pXL->SetValue(iStRow+0, 17, PgbrBase.dMel_Rd*m_dFactorMoment);
	//       iStRow += iRowCnt;
	//     }    
	//     iStRow++;
	//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_bytElastic_resistance"), iRowCnt, iColCnt);
	//     pXL->SetValue(iStRow, 8, PgbrBase.dMel_Rd*m_dFactorMoment);
	//     iStRow += iRowCnt + 1;
	//   }
	//   else ASSERT(0); 
	// 	//For Non Composite Check
	// 
	// 	//Bending_Plastic_Moment_Negative
	// 	CString str;
	// 	pXL->SetValue(iStRow+1 , 1, _T("'- Only Steel Girder (NON-Composite Stage)"));
	// 	//  pXL->SetValue(iStRow+4, 5, PgbrBase.dMa_Ed*m_dFactorMoment);
	// 	str.Format(_T("'%g"),PgbrBase.dMa_Ed*m_dFactorMoment);
	// 	pXL->SetValue(iStRow+2 , 1, _T("'M_aEd ="));pXL->SetValue(iStRow+2 , 4, str);	pXL->SetValue(iStRow+2 , 8, _T("'kN.m"));
	// 	if (iClassSect==1 || iClassSect==2)
	// 	{
	// 		str.Format(_T("'%g"),PgbrBase.dM_Rd*m_dFactorMoment);
	// 		pXL->SetValue(iStRow+3 , 1, _T("'M_Rd ="));pXL->SetValue(iStRow+3 , 4, str);	pXL->SetValue(iStRow+3 , 8, _T("'kN.m"));
	// 		double dRat = abs(PgbrBase.dMa_Ed/PgbrBase.dM_Rd);
	// 		 str.Format(_T("'%g"),dRat);		
	// 		if (dRat<=1)
	// 		{
	// 			pXL->SetValue(iStRow+5 , 1, _T("'Condition :"));pXL->SetValue(iStRow+6 , 9, _T("'M_aEd/M_Rd  = "));pXL->SetValue(iStRow+6 , 14, str);pXL->SetValue(iStRow+6 , 19, _T("'<1   ... OK"));
	// 		}
	// 		else
	// 		{
	// 			pXL->SetValue(iStRow+5 , 1, _T("'Condition :"));pXL->SetValue(iStRow+6 , 9, _T("'M_aEd/M_Rd  = "));pXL->SetValue(iStRow+6 , 14, str);pXL->SetValue(iStRow+6 , 19, _T("'>1   ... NG"));
	// 		}
	// 		
	// 	}
	// 	else 
	// 	{
	// 		str.Format(_T("'%g"),PgbrBase.dMel_Rd*m_dFactorMoment);
	// 		pXL->SetValue(iStRow+3 , 1, _T("'M_elRd ="));pXL->SetValue(iStRow+3 , 4, str);	pXL->SetValue(iStRow+3 , 8, _T("'kN.m"));//dMel_Rd
	// 		double dRat = abs(PgbrBase.dMa_Ed/PgbrBase.dMel_Rd);
	// 		 str.Format(_T("'%g"),dRat);		
	// 		if (dRat<=1)
	// 		{
	// 			pXL->SetValue(iStRow+5 , 1, _T("'Condition :"));pXL->SetValue(iStRow+6 , 9, _T("'M_aEd/M_elRd  = "));pXL->SetValue(iStRow+6 , 14, str);pXL->SetValue(iStRow+6 , 19, _T("'<1   ... OK"));
	// 		}
	// 		else
	// 		{
	// 			pXL->SetValue(iStRow+5 , 1, _T("'Condition :"));pXL->SetValue(iStRow+6 , 9, _T("'M_aEd/M_elRd  = "));pXL->SetValue(iStRow+6 , 14, str);pXL->SetValue(iStRow+6 , 19, _T("'>1   ... NG"));
	// 		}
	// 	}
	// 	iStRow += iRowCnt + 7;
	// 
	// 
	// 
	// 
	//   // Verification
	//   if(PgbrBase.bOK)  pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Verification_OK"), iRowCnt, iColCnt);
	//   else              pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Verification_NG"), iRowCnt, iColCnt);
	//   pXL->SetValue(iStRow  , 5, PgbrBase.dM_Rd*m_dFactorMoment);
	//   double dM_Ed = PgbrGd.dMy + PgbrBase.dMy;  
	// 	if (iClassSect==3 || iClassSect==4)
	// 	{
	// 		if (PgbrDetail.nKminType==Kmin_Slab || PgbrDetail.nKminType==Kmin_Rbar) dM_Ed = PgbrBase.dMy;
	// 	}
	//   pXL->SetValue(iStRow  ,19, dM_Ed*m_dFactorMoment);  
	//   iStRow += iRowCnt + 1;

}

void CRptPlateGirder_IRC22::PrintBendingResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int nPosiNega, T_PGBR_BASE& PgbrGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, T_SECT_STIFFNESS& SpscSect, T_PGLS_D& PglsD)
{
	int iRowCnt = 0;
	int iColCnt = 0;
	// 	dMpl_Rd = PgbrBase.dMpl_Rd*m_dFactorMoment;
	CString strBaseSheet = _T("Bending_Resistance");
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Title_of_BendingResistance"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, m_iBendingChapterIx);
	iStRow += iRowCnt;

	short Row_Ref = 0, Col_Ref = 0;
	CString RptBlock = _T("I_P_02_01") ;
	// ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
	pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);
	CString strSubChapter=_T("");
	// Positive Moment
	if ( nPosiNega==1 ) // Positive
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Bending_Positive"), iRowCnt, iColCnt);
	}
	else
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Bending_Negative"), iRowCnt, iColCnt);
	}
	strSubChapter.Format(_T("%d.%d"), m_iBendingChapterIx, iChapIndex);
	pXL->SetValue(iStRow, 1, strSubChapter);
	iStRow += iRowCnt;
	//   
	CString strLcomName = GetLoadCombinationName(PgbrBase.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Design_Force"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+1, 10, strLcomName);
	pXL->SetValue(iStRow+2, 5, PgbrGd.dFx*m_dFactorForce);
	pXL->SetValue(iStRow+3, 5, PgbrBase.dFx*m_dFactorForce);
	pXL->SetValue(iStRow+4, 5, PgbrBase.dMa_Ed*m_dFactorMoment);
	pXL->SetValue(iStRow+5, 5, PgbrBase.dMc_Ed*m_dFactorMoment);


	iStRow+=6;

	// Positive Moment
//   if(nPosiNega==1) // Positive
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Bending_Positive"), iRowCnt, iColCnt);    
//   }
//   else
//   {    
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Bending_Negative"), iRowCnt, iColCnt);    
//   }
//   strSubChapter.Format(_T("%d.%d"),m_iBendingChapterIx, iChapIndex);
//   pXL->SetValue(iStRow, 1, strSubChapter);    


 // PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("F_Lcom")      ,  strLcomName         , Row_Ref, Col_Ref, iStRow);
	//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_Lcom_Type") ,  RptData.strLcomType                   , Row_Ref, Col_Ref, stRow);
	//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("F_MD1")       ,   PgbrGd.dFx*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
	//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_MD2")       ,  RptData.dMu_lt      * m_dFactorMoment , Row_Ref, Col_Ref, stRow);
	 //PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("F_MD3")       ,  PgbrBase.dFx*m_dFactorForce , Row_Ref, Col_Ref, iStRow);
	//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_Mu")        ,  (RptData.dMu_gr+ RptData.dMu_lt+RptData.dMu_st ) * m_dFactorMoment , Row_Ref, Col_Ref, stRow);
	// PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("F_Vu")        ,  PglsD.PglsBase->dv_Ed        * m_dFactorForce  , Row_Ref, Col_Ref, iStRow);
	 //PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("F_Tu")        ,  PglsD.PglsBase->dNc        * m_dFactorForce , Row_Ref, Col_Ref, iStRow);
	//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_ft_MD1")    ,  RptData.dft_gr      , Row_Ref, Col_Ref, stRow);
	//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_ft_MD2")    ,  RptData.dft_lt      , Row_Ref, Col_Ref, stRow);
	//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_ft_MD3")    ,  RptData.dft_st      , Row_Ref, Col_Ref, stRow);
	//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_ft_Mu")     ,  RptData.dft         , Row_Ref, Col_Ref, stRow);
	//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_fb_MD1")    ,  RptData.dfb_gr      , Row_Ref, Col_Ref, stRow);
	//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_fb_MD2")    ,  RptData.dfb_lt      , Row_Ref, Col_Ref, stRow);
	//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_fb_MD3")    ,  RptData.dfb_st      , Row_Ref, Col_Ref, stRow);
	//   PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_fb_Mu")     ,  RptData.dfb         , Row_Ref, Col_Ref, stRow);






	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Bending_Stress"), iRowCnt, iColCnt);
	iStRow += iRowCnt;

	if ( m_nSectType==D_SECT_TYPE_COMPO_I || m_nSectType==D_SECT_TYPE_COMPO_B || m_nSectType==D_SECT_TYPE_COMPO_TUB )
	{
		PrintStressInformationDB(pXL, iStRow, strBaseSheet, PgbrDetail.StressInfoHshapeD);
	}
	else if ( m_nSectType==D_SECT_TYPE_COMPO_G )
	{
		PrintStressInformation(pXL, iStRow, strBaseSheet, PgbrDetail.GLineDgn, PgbrDetail.StressInfo4Class);
	}
	else ASSERT(0);
	//Print Section Properties again


	// Classification of sections

	PrintClassificationofSectionDetail(pXL, iStRow, PgbrBase.iTopClass, PgbrBase.iWebClass, PgbrBase.iBotClass, PgbrBase.iSectClass, PgbrBase.dTopClassRat, PgbrBase.dBotClassRat, PgbrBase.dWebClassRat);

	iStRow += iRowCnt + 1;
	double dEtaIRC = sqrt(250.0/m_dfy);
	//Minimum Web Thickness
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_Calculation"), iRowCnt, iColCnt);
	double dLambda = 0.0;
	if ( m_dfck < 60.0+cDGN_Zero )  dLambda = 0.80;
	else if ( m_dfck < 110.0+cDGN_Zero ) dLambda = 0.80 - (m_dfck-60.0)/500.0;
	else	  				                 dLambda = 0.70;

	double dEta = 0.0;
	if ( m_dfck <60.0+cDGN_Zero ) dEta = 1.0;//IRC112:2011,A2-35)
	else if ( m_dfck <110.0+cDGN_Zero ) dEta = 1.0 - (m_dfck-60.0)/250.0;//IRC112:2011,A2-36)
	else                           dEta = 0.8;
	//   pXL->SetValue(iStRow+1, 9, m_CpgdD.dGamma_C);
	//   pXL->SetValue(iStRow+2, 9, 1.2);
	//   pXL->SetValue(iStRow+3, 9, m_CpgdD.dGamma_S);
	//   pXL->SetValue(iStRow+4, 9, m_CpgdD.dGamma_M0);
	//   pXL->SetValue(iStRow+5, 9, m_CpgdD.dGamma_M1);
	// 
	//   pXL->SetValue(iStRow+1,27, m_CpgdD.dGamma_V);
	//   pXL->SetValue(iStRow+2,27, m_CpgdD.dGamma_Ff);
	//   pXL->SetValue(iStRow+3,27, m_CpgdD.dGamma_Mf);
	//   pXL->SetValue(iStRow+4,27, m_CpgdD.dGamma_Mfs);

	if ( PgbrBase.bTrSt )
	{
		//Transverse stiffeners provided :YES
		//double dEtaFact=270.0;

		// 		pXL->SetValue(iStRow+1 , 1, _T("'- Minimum Web and Flange Thickness"));
		// 		pXL->SetValue(iStRow+2 , 1, _T("'Transverse Stiffeners Provided :"));
		pXL->SetValue(iStRow+1, 13, _T("YES"));
		pXL->SetValue(iStRow+2, 13, PgbrBase.dTRSTPitch);
		// 		pXL->SetValue(iStRow+4 , 1, _T("'Condition :"));pXL->SetValue(iStRow+4 , 9, _T("'d/tw  <="));pXL->SetValue(iStRow+4 , 14, _T("270*Eta"));pXL->SetValue(iStRow+4 , 19, _T("IRC24-2010 Cl.509.6.1"));
		// 		CString dstrprint = (PgbrBase.dWebClassRat<=dEtaFact*dEtaIRC)?_T("<"):_T(">");
		pXL->SetValue(iStRow+4, 23, PgbrBase.nCondStage1);//pXL->SetValue(iStRow+5 , 11, dstrprint);pXL->SetValue(iStRow+5 , 14, dEtaFact*dEtaIRC);
		pXL->SetValue(iStRow+4, 27, (PgbrBase.bCondStage1==TRUE? _T("OK") : _T("NG")));
		pXL->SetValue(iStRow+10, 23, PgbrBase.nCondStage2);
		pXL->SetValue(iStRow+10, 27, (PgbrBase.bCondStage2==TRUE? _T("OK") : _T("NG")));

		// 		pXL->SetValue(iStRow+6 , 1, _T("'Condition :"));pXL->SetValue(iStRow+6 , 9, _T("'d/tw  <="));pXL->SetValue(iStRow+6 , 14, _T("345*Eta_f"));pXL->SetValue(iStRow+6 , 19, _T("IRC24-2010 Cl.509.6.2"));
		// 		dstrprint = (PgbrBase.dWebClassRat<=345.0*dEtaIRC)?_T("<"):_T(">");
		// 		pXL->SetValue(iStRow+7 , 9, PgbrBase.dWebClassRat);pXL->SetValue(iStRow+7 , 11, dstrprint);pXL->SetValue(iStRow+7 , 14, 345.0*dEtaIRC);

		iStRow += 17;
	}
	else
	{
		pXL->SetValue(iStRow+1, 13, _T("NO"));
		pXL->SetValue(iStRow+2, 13, PgbrBase.dTRSTPitch);
		// 		pXL->SetValue(iStRow+4 , 1, _T("'Condition :"));pXL->SetValue(iStRow+4 , 9, _T("'d/tw  <="));pXL->SetValue(iStRow+4 , 14, _T("270*Eta"));pXL->SetValue(iStRow+4 , 19, _T("IRC24-2010 Cl.509.6.1"));
		// 		CString dstrprint = (PgbrBase.dWebClassRat<=dEtaFact*dEtaIRC)?_T("<"):_T(">");
		pXL->SetValue(iStRow+4, 23, PgbrBase.nCondStage1);//pXL->SetValue(iStRow+5 , 11, dstrprint);pXL->SetValue(iStRow+5 , 14, dEtaFact*dEtaIRC);
		pXL->SetValue(iStRow+4, 27, (PgbrBase.bCondStage1==TRUE? _T("OK") : _T("NG")));
		pXL->SetValue(iStRow+10, 23, PgbrBase.nCondStage2);
		pXL->SetValue(iStRow+10, 27, (PgbrBase.bCondStage2==TRUE? _T("OK") : _T("NG")));

		iStRow += 17;
	}

	int iPNACond = 0;
	if ( PgbrBase.dPlasticNA > m_dH ) iPNACond = 0;// PNA in Slab
	else if ( PgbrBase.dPlasticNA > m_dH - m_dtf1 ) iPNACond = 1;// PNA in Top Flange
	else if ( PgbrBase.dPlasticNA < m_dH-m_dtf1 && PgbrBase.dPlasticNA > m_dtf2 ) iPNACond = 2;// PNA in Web
	else if ( PgbrBase.dPlasticNA < m_dtf2 ) iPNACond = 3;// PNA in Bottom Flange
	else ASSERT(0);

	m_nCase_PNA = iPNACond;
	m_dY_Bar_PNA = PgbrBase.dPlasticNA;
	int iClassSect = PgbrBase.iSectClass;
	if ( iClassSect==1 || iClassSect==2 ) // Plastic
	{
		//Modified Code for aashto like display


		//PNA in TOP Flange
//     if(PgbrBase.bEquiClass2)
//     {
//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Plastic_Moment_Eff_NAxis"), iRowCnt, iColCnt);
//       pXL->SetValue(iStRow+1 , 7, PgbrBase.dPlasticNA);
//       pXL->SetValue(iStRow+2 , 9, PgbrBase.dPlasticNAEff);
//     }
//     else
//     {
//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Plastic_Moment_NAxis"), iRowCnt, iColCnt);
//       pXL->SetValue(iStRow+1 , 7, PgbrBase.dPlasticNA);
//     }
//     iStRow += iRowCnt + 1;

		if ( nPosiNega==1 ) // Positive
		{
			CString strBaseSheet = _T("I_Sect");
			if ( iPNACond==0 )
			{
				//PNA in Slab
				RptBlock = _T("I_P_02_03") ;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
				short Row_Ref = 0, Col_Ref = 0;
				// ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
				pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);
				Col_Ref-=1;

				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("lambda_s")   ,  dLambda , Row_Ref, Col_Ref, iStRow);
			 // PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("eta_s")   ,  dEta , Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fcd_s"), 0.54*m_dfck/m_CpgdD.dGamma_C, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fyd_topflange"), RptData.Memb.GIRDER.Top.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fydweb"), RptData.Memb.GIRDER.Web.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fyd_botflange"), RptData.Memb.GIRDER.Bottom.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fyd_slab"), RptData.Memb.dFyr/m_CpgdD.dGamma_S, Row_Ref, Col_Ref, iStRow);


				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Prt"), PgbrDetail.dNg_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Prb"), PgbrBase.dNa_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Pt"), PgbrBase.dNa_web*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Pw"), PgbrBase.dNa_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("drt"), PgbrBase.dDistWc1, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("drb"), PgbrBase.dDistTc, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM1fc"), PgbrDetail.dNg_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM1FcFin"), PgbrDetail.dNg_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM1xu"), PgbrBase.dXpl, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM1Fcc"), PgbrDetail.dNg_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM1Med"), (PgbrBase.dMa_Ed + PgbrBase.dMc_Ed)*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM1Mres"), (PgbrBase.dMpl_Rd)*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);



			}
			else if ( iPNACond==1 )
			{
				//case 2
				RptBlock = _T("I_P_02_03_PM2") ;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
				pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;

				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2lambda_s")   ,  dLambda , Row_Ref, Col_Ref, iStRow);
			 // PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2eta_s")   ,  dEta , Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2fcd_s"), 0.54*m_dfck/m_CpgdD.dGamma_C, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2fyd_topflange"), RptData.Memb.GIRDER.Top.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2fydweb"), RptData.Memb.GIRDER.Web.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2fyd_botflange"), RptData.Memb.GIRDER.Bottom.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2fyd_slab"), RptData.Memb.dFyr/m_CpgdD.dGamma_S, Row_Ref, Col_Ref, iStRow);


				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Prt_case2"), PgbrDetail.dN_slab*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Natopccase1"), PgbrBase.dNa_topc*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Natoptcase2"), PgbrBase.dNa_topt*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Natopfinalcase2"), PgbrBase.dNa_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Nawebcase2"), PgbrBase.dNa_web*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Nabotcase2"), PgbrBase.dNa_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("xplcase2"), PgbrBase.dXpl, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("accase2"), PgbrBase.dDistWc1, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("atopcase2"), PgbrBase.dDistTc, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("awebccase2"), PgbrBase.dDistTt, Row_Ref, Col_Ref, iStRow);
				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("awebtcase2")    ,  PgbrBase.dDistW  , Row_Ref, Col_Ref, iStRow);
				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("abotcase2")    ,  PgbrBase.dDistB  , Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("P_sumA01case2"), (PgbrDetail.dN_slab+PgbrBase.dNa_top)*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("P_sumB01case2"), (PgbrBase.dNa_bot*m_dFactorForce+PgbrBase.dNa_web*m_dFactorForce), Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Y01_pcase2") , PgbrBase.dPlasticNA-(m_dH - m_dtft), Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Mp01case2") , PgbrBase.dMpl_Rd*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);

			}
			else if ( iPNACond==2 )
			{
				//CASE 3
				RptBlock = _T("I_P_02_03_PM3") ;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
				pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;

				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3lambda_s")   ,  dLambda , Row_Ref, Col_Ref, iStRow);
				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3eta_s")   ,  dEta , Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3fcd_s"), 0.54*m_dfck/m_CpgdD.dGamma_C, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3fyd_topflange"), RptData.Memb.GIRDER.Top.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3fydweb"), RptData.Memb.GIRDER.Web.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3fyd_botflange"), RptData.Memb.GIRDER.Bottom.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3fyd_slab"), RptData.Memb.dFyr/m_CpgdD.dGamma_S, Row_Ref, Col_Ref, iStRow);


				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Prt_case3"), PgbrDetail.dN_slab*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Natopccase3"), PgbrBase.dNa_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Natoptcase3"), PgbrBase.dNa_web*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Nawebcase3"), PgbrBase.dNa_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Nabotcase3")    ,  PgbrDetail.dNg_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("abotcase3"), PgbrBase.dDistWc1, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("atopcase3"), PgbrBase.dDistTc, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("awebccase3"), PgbrBase.dDistTt, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("awebtcase3"), PgbrBase.dDistB, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("xucase3"), PgbrBase.dXpl, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Y01_pcase3") , PgbrBase.dDistW, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fscwebcase3"), PgbrBase.dNa_webc*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fstwebcase3"), PgbrBase.dNa_webt*m_dFactorForce, Row_Ref, Col_Ref, iStRow);


				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("P_sumA01case3"), PgbrDetail.dNg_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("P_sumB01case3"), PgbrBase.dNa_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Mp01case3") , PgbrBase.dMpl_Rd*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);

			}
			iStRow += 50;
			//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Plastic_Moment_Positive_Param"), iRowCnt, iColCnt);
			//       pXL->SetValue(iStRow+0 , 5, PgbrDetail.dN_slab*m_dFactorForce);
			//       pXL->SetValue(iStRow+1 , 5, PgbrDetail.dNg_top*m_dFactorForce);
			//       pXL->SetValue(iStRow+2 , 5, PgbrDetail.dNg_bot*m_dFactorForce);
			//       pXL->SetValue(iStRow+4 , 5, PgbrDetail.dMpl_Rd_cal*m_dFactorMoment);
			//       pXL->SetValue(iStRow+5 , 5, PgbrBase.dXpl);
			//       pXL->SetValue(iStRow+6 ,10, PgbrBase.dM_Rd*m_dFactorMoment);
			//       pXL->SetValue(iStRow+7, 7, PgbrBase.dBeta);
			//       iStRow += iRowCnt + 1;
			// 
			//       // if xpl/h>0.4ÀÌ¸é Class 1,2 ¿¡ ´ëÇØ¼­µµ Elastic¿¡ ´ëÇØ °ËÅäÇÑ´Ù
			//       if(PgbrBase.dXplOverh>0.4)
			//       {
			//         pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Elastic_resistance_moment"), iRowCnt, iColCnt);        
			//         iStRow += iRowCnt;
			//         pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_Xpl_h_check_MoreThan"), iRowCnt, iColCnt);        
			//         pXL->SetValue(iStRow, 5, PgbrBase.dXplOverh);
			//         iStRow += iRowCnt;        
			//         // Modify by GAY. PMS:4216. ('12.05.16). Modify output format for k factor.
			//         pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_Positive"), iRowCnt, iColCnt);
			//         pXL->SetValue(iStRow  ,  4, PgbrBase.dk_min);
			//         iStRow += iRowCnt;
			//         //if(PgbrBase.dk_min>=0.0)
			//         {
			//           pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_by_k_min"), iRowCnt, iColCnt);
			//           pXL->SetValue(iStRow+0, 12, PgbrBase.dMel_Rd*m_dFactorMoment);
			//           iStRow += iRowCnt;
			//         }        
			//         iStRow++;
			//         pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_bytElastic_resistance"), iRowCnt, iColCnt);
			//         pXL->SetValue(iStRow, 8, PgbrBase.dMel_Rd*m_dFactorMoment);
			//         iStRow += iRowCnt + 1;
			// 
			//         if(PgbrBase.dNc<0.0) // ¾ÐÃà·ÂÀÌ ÀÖÀ»¶§ Àû¿ëÇÑ´Ù. 
			//         {
			//           if(fabs(PgbrBase.dNc) < PgbrBase.dNc_el)
			//           {
			//             pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_Class1_2_Pos_LT_Ncel"), iRowCnt, iColCnt);
			//             pXL->SetValue(iStRow  , 5, PgbrBase.dNc   *m_dFactorForce);
			//             pXL->SetValue(iStRow+1, 5, PgbrBase.dNc_el*m_dFactorForce);
			//             pXL->SetValue(iStRow+2,16, PgbrBase.dM_Rd *m_dFactorMoment);
			//             iStRow += iRowCnt + 1;
			//           }
			//           else if(fabs(PgbrBase.dNc) < PgbrBase.dNc_f)
			//           {
			//             pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_Class1_2_Pos_GT_Ncel"), iRowCnt, iColCnt);
			//             pXL->SetValue(iStRow  , 5, PgbrBase.dNc   *m_dFactorForce);
			//             pXL->SetValue(iStRow+1, 5, PgbrBase.dNc_el*m_dFactorForce);
			//             pXL->SetValue(iStRow+2, 5, PgbrBase.dNc_f *m_dFactorForce);
			//             pXL->SetValue(iStRow+3,16, PgbrBase.dM_Rd *m_dFactorMoment);
			//             iStRow += iRowCnt + 1;
			//           }
			//           else ASSERT(0);        
			//         }
			//       }      
		}
		else if ( nPosiNega==2 )
		{
			if ( iPNACond==1 )
			{//PNA in top Flange Negative
				RptBlock = _T("I_P_02_03_PM5") ;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, _T("I_Sect"), RptBlock, iRowCnt, iColCnt);
				pXL->GetCellIDByName(m_strBaseBook, _T("I_Sect"), RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fcd_s"), 0.54*m_dfck/m_CpgdD.dGamma_C, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact2"), RptData.Memb.GIRDER.Top.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact3"), RptData.Memb.GIRDER.Web.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact4"), RptData.Memb.GIRDER.Bottom.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact5"), RptData.Memb.dFyr/m_CpgdD.dGamma_S, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact6"), PgbrDetail.dNr_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact7"), PgbrBase.dNa_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact8"), PgbrBase.dNa_web*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact9"), PgbrBase.dNa_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact10"), PgbrDetail.dNg_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact11"), (PgbrBase.dNa_bot*m_dFactorForce), Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact12"), PgbrBase.dDistB, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact13"), PgbrBase.dDistTt, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact14"), PgbrBase.dNa_botc, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM5fact15") , PgbrBase.dM_Rd*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);
				iStRow += 43;
			}
			else
			{//PNA in Web Negative
				RptBlock = _T("I_P_02_03_PM4") ;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, _T("I_Sect"), RptBlock, iRowCnt, iColCnt);
				pXL->GetCellIDByName(m_strBaseBook, _T("I_Sect"), RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;

				// PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM4lambda_s")   ,  dLambda , Row_Ref, Col_Ref, iStRow);
				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM4eta_s")   ,  dEta , Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM4fcd_s"), 0.54*m_dfck/m_CpgdD.dGamma_C, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM4fyd_topflange"), RptData.Memb.GIRDER.Top.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM4fydweb"), RptData.Memb.GIRDER.Web.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM4fyd_botflange"), RptData.Memb.GIRDER.Bottom.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM4fyd_slab"), RptData.Memb.dFyr/m_CpgdD.dGamma_S, Row_Ref, Col_Ref, iStRow);


				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PrtNeg"), PgbrBase.dDistTc*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("NatopNeg"), PgbrBase.dNa_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("NawebcNeg"), PgbrBase.dNa_web*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("NawebtNeg"), PgbrBase.dNa_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("NabotNeg")    ,  PgbrDetail.dNg_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("asiNeg"), PgbrBase.dDistB, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("atopneg"), PgbrBase.dNa_botc, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("NabotNeg"), PgbrBase.dDistTt, Row_Ref, Col_Ref, iStRow);
				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("awebtneg")    ,  PgbrBase.dDistB  , Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("P_sumA01Neg"), PgbrDetail.dNg_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("P_sumB01Neg"), (PgbrBase.dNa_bot*m_dFactorForce), Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Y01_neg") , PgbrBase.dPlasticNA, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Mp01Neg") , PgbrBase.dM_Rd*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);

				//       pXL->SetValue(iStRow+0, 5, PgbrDetail.dN_slab*m_dFactorForce);
				//       pXL->SetValue(iStRow+1, 5, PgbrDetail.dNr_top*m_dFactorForce);
				//       pXL->SetValue(iStRow+2, 5, PgbrDetail.dNg_top*m_dFactorForce);
				//       pXL->SetValue(iStRow+3, 5, PgbrDetail.dNg_bot*m_dFactorForce);
				//       pXL->SetValue(iStRow+5, 5, PgbrBase.dMpl_Rd*m_dFactorMoment);
				//       pXL->SetValue(iStRow+6, 8, PgbrBase.dM_Rd*m_dFactorMoment);

				iStRow += 49;
			}
			//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Plastic_Moment_Negative_Param"), iRowCnt, iColCnt);
			//       iStRow+=2;

		}
		else ASSERT(0);

	}
	else if ( iClassSect==3 ) // Elastic
	{
		if ( nPosiNega==1 ) // Positive
		{
			CString strBaseSheet = _T("I_Sect");
			if ( iPNACond==0 )
			{
				//PNA in Slab
				RptBlock = _T("I_P_02_03") ;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
				short Row_Ref = 0, Col_Ref = 0;
				// ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
				pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);
				Col_Ref-=1;

				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("lambda_s")   ,  dLambda , Row_Ref, Col_Ref, iStRow);
				// PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("eta_s")   ,  dEta , Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fcd_s"), 0.54*m_dfck/m_CpgdD.dGamma_C, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fyd_topflange"), RptData.Memb.GIRDER.Top.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fydweb"), RptData.Memb.GIRDER.Web.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fyd_botflange"), RptData.Memb.GIRDER.Bottom.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fyd_slab"), RptData.Memb.dFyr/m_CpgdD.dGamma_S, Row_Ref, Col_Ref, iStRow);


				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Prt"), PgbrDetail.dNg_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Prb"), PgbrBase.dNa_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Pt"), PgbrBase.dNa_web*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Pw"), PgbrBase.dNa_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("drt"), PgbrBase.dDistWc1, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("drb"), PgbrBase.dDistTc, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM1fc"), PgbrDetail.dNg_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM1FcFin"), PgbrDetail.dNg_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM1xu"), PgbrBase.dDistT, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM1Fcc"), PgbrDetail.dNg_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM1Med"), (PgbrBase.dMa_Ed + PgbrBase.dMc_Ed)*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM1Mres"), (PgbrBase.dMpl_Rd)*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);



			}
			else if ( iPNACond==1 )
			{
				//case 2
				RptBlock = _T("I_P_02_03_PM2") ;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
				pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;

				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2lambda_s")   ,  dLambda , Row_Ref, Col_Ref, iStRow);
				// PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2eta_s")   ,  dEta , Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2fcd_s"), 0.54*m_dfck/m_CpgdD.dGamma_C, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2fyd_topflange"), RptData.Memb.GIRDER.Top.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2fydweb"), RptData.Memb.GIRDER.Web.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2fyd_botflange"), RptData.Memb.GIRDER.Bottom.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM2fyd_slab"), RptData.Memb.dFyr/m_CpgdD.dGamma_S, Row_Ref, Col_Ref, iStRow);


				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Prt_case2"), PgbrDetail.dN_slab*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Natopccase1"), PgbrBase.dNa_topc*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Natoptcase2"), PgbrBase.dNa_topt*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Natopfinalcase2"), PgbrBase.dNa_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Nawebcase2"), PgbrBase.dNa_web*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Nabotcase2"), PgbrBase.dNa_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("xplcase2"), PgbrBase.dXpl, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("accase2"), PgbrBase.dDistWc1, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("atopcase2"), PgbrBase.dDistTc, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("awebccase2"), PgbrBase.dDistTt, Row_Ref, Col_Ref, iStRow);
				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("awebtcase2")    ,  PgbrBase.dDistW  , Row_Ref, Col_Ref, iStRow);
				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("abotcase2")    ,  PgbrBase.dDistB  , Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("P_sumA01case2"), (PgbrDetail.dN_slab+PgbrBase.dNa_top)*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("P_sumB01case2"), (PgbrBase.dNa_bot*m_dFactorForce+PgbrBase.dNa_web*m_dFactorForce), Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Y01_pcase2") , PgbrBase.dDistW2, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Mp01case2") , PgbrBase.dMpl_Rd*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);

			}
			else if ( iPNACond==2 )
			{
				//CASE 3
				RptBlock = _T("I_P_02_03_PM3") ;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
				pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;

				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3lambda_s")   ,  dLambda , Row_Ref, Col_Ref, iStRow);
				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3eta_s")   ,  dEta , Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3fcd_s"), 0.54*m_dfck/m_CpgdD.dGamma_C, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3fyd_topflange"), RptData.Memb.GIRDER.Top.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3fydweb"), RptData.Memb.GIRDER.Web.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3fyd_botflange"), RptData.Memb.GIRDER.Bottom.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("PM3fyd_slab"), RptData.Memb.dFyr/m_CpgdD.dGamma_S, Row_Ref, Col_Ref, iStRow);


				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Prt_case3"), PgbrDetail.dN_slab*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Natopccase3"), PgbrBase.dNa_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Natoptcase3"), PgbrBase.dNa_web*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Nawebcase3"), PgbrBase.dNa_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				//PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Nabotcase3")    ,  PgbrDetail.dNg_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("abotcase3"), PgbrBase.dDistWc1, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("atopcase3"), PgbrBase.dDistTc, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("awebccase3"), PgbrBase.dDistTt, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("awebtcase3"), PgbrBase.dDistB, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("xucase3"), PgbrBase.dXpl, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Y01_pcase3") , PgbrBase.dDistW, Row_Ref, Col_Ref, iStRow);

				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fscwebcase3"), PgbrBase.dNa_webc*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fstwebcase3"), PgbrBase.dNa_webt*m_dFactorForce, Row_Ref, Col_Ref, iStRow);


				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("P_sumA01case3"), PgbrDetail.dNg_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("P_sumB01case3"), PgbrBase.dNa_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("Mp01case3") , PgbrBase.dMpl_Rd*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);

			}
			iStRow += 50;

		}
		else
		{
			//Semi Compact NEgative
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Elastic_resistance_moment"), iRowCnt, iColCnt);
			iStRow += iRowCnt;
			strBaseSheet = _T("I_Sect"); RptBlock = _T("I_P_02_04_YM") ;
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
			pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;

			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact1"), 0.54*m_dfck/m_CpgdD.dGamma_C, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact2"), RptData.Memb.GIRDER.Top.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact3"), RptData.Memb.GIRDER.Web.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact4"), RptData.Memb.GIRDER.Bottom.dfy_or/m_CpgdD.dGamma_M0_IRC, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact5"), RptData.Memb.dFyr/m_CpgdD.dGamma_S, Row_Ref, Col_Ref, iStRow);

			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact6"), PgbrDetail.dNr_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact7"), PgbrBase.dNa_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact8"), PgbrBase.dNa_web*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact9"), PgbrBase.dNa_bot*m_dFactorForce, Row_Ref, Col_Ref, iStRow);

			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact11"), PgbrDetail.dNg_top*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact12"), (PgbrBase.dNa_bot*m_dFactorForce), Row_Ref, Col_Ref, iStRow);

			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact14"), PgbrBase.dDistB, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact13"), PgbrBase.dDistTt, Row_Ref, Col_Ref, iStRow);

			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact16"), PgbrBase.dNa_botc, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact15") , PgbrBase.dDistT, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMFact17") , PgbrBase.dMpl_Rd*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);

			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMElaFact1") , PgbrDetail.dM_Ed_top*m_dFactorLengthMM, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMElaFact2") , PgbrDetail.dM_Ed_bot, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMElaFact3") , PgbrDetail.dM_Ed, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("YMElaFact4") , PgbrBase.dMel_Rd*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);

			iStRow += 69;
		}



		// Modify by GAY. PMS:4216. ('12.05.16). Modify output format for k factor.

//     pXL->SetValue(iStRow  ,  4, PgbrBase.dk_min);
// 
//     iStRow += iRowCnt;
//     //if(PgbrBase.dk_min>=0.0)
//     {
//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_by_k_min"), iRowCnt, iColCnt);
//       pXL->SetValue(iStRow+0, 12, PgbrBase.dMel_Rd*m_dFactorMoment);
//       iStRow += iRowCnt;
//     }    
//     iStRow++;
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_bytElastic_resistance"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow, 8, PgbrBase.dMel_Rd*m_dFactorMoment);
//     iStRow += iRowCnt + 1;

	}
	else if ( iClassSect==4 ) // Effective
	{
		pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Effective_section"), iRowCnt, iColCnt);
		iStRow += iRowCnt;

		pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Effective_section_Fx"), iRowCnt, iColCnt);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Effective_Section_Stiffness"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow+1, 4, PgbrDetail.EffGirderFx.Area);
		pXL->SetValue(iStRow+2, 4, PgbrDetail.EffGirderFx.Ryy);
		pXL->SetValue(iStRow+3, 4, PgbrDetail.EffGirderFx.Rzz);
		pXL->SetValue(iStRow+4, 4, PgbrDetail.EffGirderFx.Cym);
		pXL->SetValue(iStRow+5, 4, PgbrDetail.EffGirderFx.Czm);
		//
		pXL->SetValue(iStRow+1, 18, PgbrDetail.EffCompositeFx.Area);
		pXL->SetValue(iStRow+2, 18, PgbrDetail.EffCompositeFx.Ryy);
		pXL->SetValue(iStRow+3, 18, PgbrDetail.EffCompositeFx.Rzz);
		pXL->SetValue(iStRow+4, 18, PgbrDetail.EffCompositeFx.Cym);
		pXL->SetValue(iStRow+5, 18, PgbrDetail.EffCompositeFx.Czm);
		iStRow += iRowCnt + 1;

		pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Effective_section_My"), iRowCnt, iColCnt);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Effective_Section_Stiffness"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow+1, 4, PgbrDetail.EffGirder.Area);
		pXL->SetValue(iStRow+2, 4, PgbrDetail.EffGirder.Ryy);
		pXL->SetValue(iStRow+3, 4, PgbrDetail.EffGirder.Rzz);
		pXL->SetValue(iStRow+4, 4, PgbrDetail.EffGirder.Cym);
		pXL->SetValue(iStRow+5, 4, PgbrDetail.EffGirder.Czm);
		//
		pXL->SetValue(iStRow+1, 18, PgbrDetail.EffComposite.Area);
		pXL->SetValue(iStRow+2, 18, PgbrDetail.EffComposite.Ryy);
		pXL->SetValue(iStRow+3, 18, PgbrDetail.EffComposite.Rzz);
		pXL->SetValue(iStRow+4, 18, PgbrDetail.EffComposite.Cym);
		pXL->SetValue(iStRow+5, 18, PgbrDetail.EffComposite.Czm);
		iStRow += iRowCnt + 1;

		pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Added_moment"), iRowCnt, iColCnt);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Added_moment_calc"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 13, PgbrDetail.dDelMa_Ed*m_dFactorMoment);
		pXL->SetValue(iStRow+1, 13, PgbrDetail.dDelMc_Ed*m_dFactorMoment);
		pXL->SetValue(iStRow+3, 6, PgbrBase.dSIGa_top);
		pXL->SetValue(iStRow+5, 6, PgbrBase.dSIGa_bot);
		iStRow += iRowCnt;

		CString strBlockName;
		if ( nPosiNega==1 ) strBlockName = _T("Elastic_resistance_Positive");// Positive
		else             strBlockName = _T("Elastic_resistance_Negative");
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strBlockName, iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 4, PgbrBase.dk_min);
		iStRow += iRowCnt;
		//if(PgbrBase.dk_min>=0.0)
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_Class4by_k_min"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow+0, 17, PgbrBase.dMel_Rd*m_dFactorMoment);
			iStRow += iRowCnt;
		}
		iStRow++;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_bytElastic_resistance"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 8, PgbrBase.dMel_Rd*m_dFactorMoment);
		iStRow += iRowCnt + 1;
	}
	else ASSERT(0);
	//For Non Composite Check

	//Bending_Plastic_Moment_Negative
//   CString str;
//   pXL->SetValue(iStRow+1 , 1, _T("'- Only Steel Girder (NON-Composite Stage)"));
//   //  pXL->SetValue(iStRow+4, 5, PgbrBase.dMa_Ed*m_dFactorMoment);
//   str.Format(_T("'%g"),PgbrBase.dMa_Ed*m_dFactorMoment);
//   pXL->SetValue(iStRow+2 , 1, _T("'M_aEd ="));pXL->SetValue(iStRow+2 , 4, str);	pXL->SetValue(iStRow+2 , 8, _T("'kN.m"));
//   if (iClassSect==1 || iClassSect==2)
//   {
//     str.Format(_T("'%g"),PgbrBase.dM_Rd*m_dFactorMoment);
//     pXL->SetValue(iStRow+3 , 1, _T("'M_Rd ="));pXL->SetValue(iStRow+3 , 4, str);	pXL->SetValue(iStRow+3 , 8, _T("'kN.m"));
//     double dRat = abs(PgbrBase.dMa_Ed/PgbrBase.dM_Rd);
//     str.Format(_T("'%g"),dRat);		
//     if (dRat<=1)
//     {
//       pXL->SetValue(iStRow+5 , 1, _T("'Condition :"));pXL->SetValue(iStRow+6 , 9, _T("'M_aEd/M_Rd  = "));pXL->SetValue(iStRow+6 , 14, str);pXL->SetValue(iStRow+6 , 19, _T("'<1   ... OK"));
//     }
//     else
//     {
//       pXL->SetValue(iStRow+5 , 1, _T("'Condition :"));pXL->SetValue(iStRow+6 , 9, _T("'M_aEd/M_Rd  = "));pXL->SetValue(iStRow+6 , 14, str);pXL->SetValue(iStRow+6 , 19, _T("'>1   ... NG"));
//     }
// 
//   }
//   else 
//   {
//     str.Format(_T("'%g"),PgbrBase.dMel_Rd*m_dFactorMoment);
//     pXL->SetValue(iStRow+3 , 1, _T("'M_elRd ="));pXL->SetValue(iStRow+3 , 4, str);	pXL->SetValue(iStRow+3 , 8, _T("'kN.m"));//dMel_Rd
//     double dRat = abs(PgbrBase.dMa_Ed/PgbrBase.dMel_Rd);
//     str.Format(_T("'%g"),dRat);		
//     if (dRat<=1)
//     {
//       pXL->SetValue(iStRow+5 , 1, _T("'Condition :"));pXL->SetValue(iStRow+6 , 9, _T("'M_aEd/M_elRd  = "));pXL->SetValue(iStRow+6 , 14, str);pXL->SetValue(iStRow+6 , 19, _T("'<1   ... OK"));
//     }
//     else
//     {
//       pXL->SetValue(iStRow+5 , 1, _T("'Condition :"));pXL->SetValue(iStRow+6 , 9, _T("'M_aEd/M_elRd  = "));pXL->SetValue(iStRow+6 , 14, str);pXL->SetValue(iStRow+6 , 19, _T("'>1   ... NG"));
//     }
//   }
//   iStRow += iRowCnt + 7;


	m_dMomentRatio = PgbrBase.dRatio;//PgbrBase.dM_Rd/(PgbrBase.dMa_Ed+PgbrBase.dMc_Ed);
	// Verification
	if ( PgbrBase.bOK )  pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Verification_OK"), iRowCnt, iColCnt);
	else              pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Verification_NG"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 5, PgbrBase.dM_Rd*m_dFactorMoment);
	double dM_Ed = PgbrGd.dMy + PgbrBase.dMy;
	if ( iClassSect==3 || iClassSect==4 )
	{
		if ( PgbrDetail.nKminType==Kmin_Slab || PgbrDetail.nKminType==Kmin_Rbar ) dM_Ed = PgbrBase.dMy;
	}
	pXL->SetValue(iStRow, 19, dM_Ed*m_dFactorMoment);
	iStRow += iRowCnt + 1;

	//For Resistance to lateral bending moment

}

void CRptPlateGirder_IRC22::PrintStressInformationDB(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_STRESS_INFO_I& StrInfoD)
{
	int iRowCnt = 0;
	int iColCnt = 0;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Stress_H_Shape"), iRowCnt, iColCnt);
	T_STRESS_LINE_INFO FlgTL = StrInfoD.FlgTL;
	pXL->SetValue(iStRow+1, 8, FlgTL.dy1);
	pXL->SetValue(iStRow+1, 16, FlgTL.dz1);
	pXL->SetValue(iStRow+1, 24, FlgTL.dStr1);
	pXL->SetValue(iStRow+2, 8, FlgTL.dy2);
	pXL->SetValue(iStRow+2, 16, FlgTL.dz2);
	pXL->SetValue(iStRow+2, 24, FlgTL.dStr2);
	//
	T_STRESS_LINE_INFO FlgTR = StrInfoD.FlgTR;
	pXL->SetValue(iStRow+3, 8, FlgTR.dy1);
	pXL->SetValue(iStRow+3, 16, FlgTR.dz1);
	pXL->SetValue(iStRow+3, 24, FlgTR.dStr1);
	pXL->SetValue(iStRow+4, 8, FlgTR.dy2);
	pXL->SetValue(iStRow+4, 16, FlgTR.dz2);
	pXL->SetValue(iStRow+4, 24, FlgTR.dStr2);
	//
	T_STRESS_LINE_INFO FlgBL = StrInfoD.FlgBL;
	pXL->SetValue(iStRow+7, 8, FlgBL.dy1);
	pXL->SetValue(iStRow+7, 16, FlgBL.dz1);
	pXL->SetValue(iStRow+7, 24, FlgBL.dStr1);
	pXL->SetValue(iStRow+8, 8, FlgBL.dy2);
	pXL->SetValue(iStRow+8, 16, FlgBL.dz2);
	pXL->SetValue(iStRow+8, 24, FlgBL.dStr2);
	//
	T_STRESS_LINE_INFO FlgBR = StrInfoD.FlgBR;
	pXL->SetValue(iStRow+9, 8, FlgBR.dy1);
	pXL->SetValue(iStRow+9, 16, FlgBR.dz1);
	pXL->SetValue(iStRow+9, 24, FlgBR.dStr1);
	pXL->SetValue(iStRow+10, 8, FlgBR.dy2);
	pXL->SetValue(iStRow+10, 16, FlgBR.dz2);
	pXL->SetValue(iStRow+10, 24, FlgBR.dStr2);
	//
	T_STRESS_LINE_INFO Web = StrInfoD.Web;
	pXL->SetValue(iStRow+13, 8, Web.dy1);
	pXL->SetValue(iStRow+13, 16, Web.dz1);
	pXL->SetValue(iStRow+13, 24, Web.dStr1);
	pXL->SetValue(iStRow+14, 8, Web.dy2);
	pXL->SetValue(iStRow+14, 16, Web.dz2);
	pXL->SetValue(iStRow+14, 24, Web.dStr2);
	iStRow += iRowCnt+1;
}

void CRptPlateGirder_IRC22::PrintStressInformation(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_GLINE_DGN_D& GLineDgn, T_DLINE_CALC_D& StressInfoD)
{
	//T_GLINE_DGN_D GLineDgn = PgbrDetail.GLineDgn;
	CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&> aTopFlgPartLine;
	CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&> aBotFlgPartLine;
	CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&> aWebPartLine;
	m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_GLineDgnPartData(GLineDgn, aTopFlgPartLine, aBotFlgPartLine, aWebPartLine);

	//T_DLINE_CALC_D StressInfoD = PgbrDetail.StressInfo4Class;
	CArray<T_DLINE_PART_D, T_DLINE_PART_D&> aTopFlangePart;
	CArray<T_DLINE_PART_D, T_DLINE_PART_D&> aBotFlangePart;
	CArray<T_DLINE_PART_D, T_DLINE_PART_D&> aWebPart;
	m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_DLinePartData(StressInfoD, aTopFlangePart, aBotFlangePart, aWebPart);

	// Top
	PrintStressInfo4Class(pXL, iStRow, aTopFlgPartLine, aTopFlangePart);
	// Web
	PrintStressInfo4Class(pXL, iStRow, aWebPartLine, aWebPart);
	// Bottom
	PrintStressInfo4Class(pXL, iStRow, aBotFlgPartLine, aBotFlangePart);

	iStRow++;
}

void CRptPlateGirder_IRC22::PrintStressInfo4Class(CMSExcel* pXL, int& iStRow, CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&>& aPartLine, CArray<T_DLINE_PART_D, T_DLINE_PART_D&>& aDgnLinePartD)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	int iPartLineSize = aPartLine.GetSize();
	int iPartSize = aDgnLinePartD.GetSize();
	if ( iPartLineSize!=iPartSize ) { ASSERT(0); return; }

	int i=0, j=0;
	CString strBaseSheet = _T("Bending_Resistance"); // Stress Ãâ·ÂÀº Bending_Resistance sheet¿¡ ÀÖÀ½.

	double dy1=0., dz1=0., dy2=0., dz2=0.;
	double dStrEnd1=0., dStrEnd2=0.;
	for ( i=0; i<iPartSize; i++ )
	{
		T_GLINE_PART_DGN GLinePartD = aPartLine[i];
		T_DLINE_PART_D LinePartD = aDgnLinePartD[i];
		int iPartType = LinePartD.nPartType;
		CString strPartType;
		CString strPartName;
		//if     (iPartType==0) strPartType = _T("Sub_Title_Stress_TopFlange_G_Shape");
		//else if(iPartType==1) strPartType = _T("Sub_Title_Stress_BottomFlange_G_Shape");
		//else if(iPartType==2) strPartType = _T("Sub_Title_Stress_Web_G_Shape");
		//else ASSERT(0);

		strPartType = Get_String4PartShape(iPartType);
		CString strName = GLinePartD.strGroupName;
		strPartName.Format(_T("%s(%s)"), strPartType, strName);
		pXL->SetValue(iStRow, 2, strPartName);
		pXL->SetFont(iStRow, 0, iStRow, 32, 10, 0, m_FontType, FALSE);
		iStRow++;
		//pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, strPartType, iRowCnt, iColCnt);    

		int iInternalSize = LinePartD.aInternal.GetSize();
		if ( iInternalSize>0 )
		{
			pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Stress_Internal_G_Shape"), iRowCnt, iColCnt);
			iStRow += iRowCnt;
			for ( j=0; j<iInternalSize; j++ )
			{
				T_GLINE_DST GLineDst = GLinePartD.aInternal[j];

				//         dy1 = GLineDst.dy1;
				//         dz1 = GLineDst.dz1;
				//         dy2 = GLineDst.dy2;
				//         dz2 = GLineDst.dz2;

				T_DGNLINE_LBD DgnLineLBD = LinePartD.aInternal[j];
				T_GLINE_DGN_B aDgnBaseD = DgnLineLBD.aDgnBaseD.GetAt(0);
				dy1 = aDgnBaseD.dy1;
				dz1 = aDgnBaseD.dz1;
				dy2 = aDgnBaseD.dy2;
				dz2 = aDgnBaseD.dz2;

				m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_LineEndStress4LineDgnBase(DgnLineLBD.aDgnBaseD, dStrEnd1, dStrEnd2);
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Stress_G_Shape"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 2, GLineDst.strShape);
				pXL->SetValue(iStRow, 8, dy1);
				pXL->SetValue(iStRow, 16, dz1);
				pXL->SetValue(iStRow, 24, dStrEnd1);
				pXL->SetValue(iStRow+1, 8, dy2);
				pXL->SetValue(iStRow+1, 16, dz2);
				pXL->SetValue(iStRow+1, 24, dStrEnd2);
				iStRow += iRowCnt;
			}
		}

		int iOutstandSize = LinePartD.aOutstand.GetSize();
		if ( iOutstandSize>0 )
		{
			pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Stress_Outstand_G_Shape"), iRowCnt, iColCnt);
			iStRow += iRowCnt;
			for ( j=0; j<iOutstandSize; j++ )
			{
				T_GLINE_DST GLineDst = GLinePartD.aOutstand[j];
				//         dy1 = GLineDst.dy1;
				//         dz1 = GLineDst.dz1;
				//         dy2 = GLineDst.dy2;
				//         dz2 = GLineDst.dz2;

				T_DGNLINE_LBD DgnLineLBD = LinePartD.aOutstand[j];
				T_GLINE_DGN_B aDgnBaseD = DgnLineLBD.aDgnBaseD.GetAt(0);
				dy1 = aDgnBaseD.dy1;
				dz1 = aDgnBaseD.dz1;
				dy2 = aDgnBaseD.dy2;
				dz2 = aDgnBaseD.dz2;

				m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_LineEndStress4LineDgnBase(DgnLineLBD.aDgnBaseD, dStrEnd1, dStrEnd2);
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Stress_G_Shape"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 2, GLineDst.strShape);
				pXL->SetValue(iStRow, 8, dy1);
				pXL->SetValue(iStRow, 16, dz1);
				pXL->SetValue(iStRow, 24, dStrEnd1);
				pXL->SetValue(iStRow+1, 8, dy2);
				pXL->SetValue(iStRow+1, 16, dz2);
				pXL->SetValue(iStRow+1, 24, dStrEnd2);
				iStRow += iRowCnt;
			}
		}
	}

}

void CRptPlateGirder_IRC22::PrintClassificationofSection(CMSExcel* pXL, int& iStRow, int iTopCS, int iWebCS, int iBotCS, int iSectCS)
{
	int iRowCnt = 0;
	int iColCnt = 0;
	// Classification of sections
	CString strBaseSheet = _T("Bending_Resistance");
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Classification_of_sections"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+2, 7, iTopCS);
	pXL->SetValue(iStRow+3, 7, iWebCS);
	pXL->SetValue(iStRow+4, 7, iBotCS);
	pXL->SetValue(iStRow+5, 7, iSectCS);//15
	iStRow += iRowCnt + 1;
}

//	void PrintClassificationofSectionDetail(CMSExcel* pXL, int& iStRow, int iTopCS, int iWebCS, int iBotCS, int iSectCS,double dTopClassRat,double dBotClassRat,double dWebClassRat);
void CRptPlateGirder_IRC22::PrintClassificationofSectionDetail(CMSExcel* pXL, int& iStRow, int iTopCS, int iWebCS, int iBotCS, int iSectCS, double dTopClassRat, double dBotClassRat, double dWebClassRat)
{
	int iRowCnt = 0;
	int iColCnt = 0;
	// Classification of sections
	CString strBaseSheet = _T("Bending_Resistance");
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Classification_of_sections"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+2, 7, iTopCS); pXL->SetValue(iStRow+2, 15, dTopClassRat);
	pXL->SetValue(iStRow+3, 7, iWebCS); pXL->SetValue(iStRow+3, 15, dWebClassRat);
	pXL->SetValue(iStRow+4, 7, iBotCS); pXL->SetValue(iStRow+4, 15, dBotClassRat);
	CString SectClass = _T("");
	if ( iSectCS==1 ) SectClass = _T("Plastic");
	else if ( iSectCS==2 ) SectClass = _T("Compact");
	else if ( iSectCS==3 ) SectClass = _T("Semi Compact");
	else                 SectClass = _T("Revise Section");

	pXL->SetValue(iStRow+5, 7, iSectCS); pXL->SetValue(iStRow+5, 15, SectClass);


	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintShearResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("Shear_Resistance");

	// 1. Title
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Title_of_ShearResistance"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, m_iShearChapterIx);
	iStRow += iRowCnt;

	// 2. SubTitle
	CString strSubChapter;

	// 3.Design Force
	PrintShearResistance_ShearDgnForce(pXL, iStRow, PrvsGd, PrvsBase);

	// 4. Stress 
	//pXL->CopyRange(iStRow, 0, m_strBaseBook, strTitleBaseSheet, _T("Sub_Title_Bending_Stress"), iRowCnt, iColCnt); 
	iStRow += iRowCnt;
	//   if(m_nSectType==D_SECT_TYPE_COMPO_I || m_nSectType==D_SECT_TYPE_COMPO_B || m_nSectType==D_SECT_TYPE_COMPO_TUB)
	//   {
	//     PrintStressInformationDB(pXL, iStRow, strBaseSheet, PrvsDetail.StressInfoHshapeD);    
	//   }
	//   else if(m_nSectType == D_SECT_TYPE_COMPO_G)
	//   {
	//     PrintStressInformation(pXL, iStRow, strBaseSheet, PrvsDetail.GLineDgn, PrvsDetail.StressInfo4Class);    
	//   }
	//   else ASSERT(0);

		// 5. Classification of sections
		//PrintClassificationofSection(pXL, iStRow, PrvsBase.iTopClass, PrvsBase.iWebClass, PrvsBase.iBotClass, PrvsBase.iSectClass);  

		// 6. Sub Title
		//pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_Plastic_resistance"), iRowCnt, iColCnt);
		//iStRow += iRowCnt;

		// 7. Shear Plastic Moment
		//PrintShearResistance_PlasticRestance(pXL, iStRow, PrvsBase, PrvsDetail);

		// 8. ShearBucklingCoeff
	PrintShearResistance_ShearBucklingCoeff(pXL, iStRow, strBaseSheet, PrvsGd, PrvsBase, PrvsDetail);
}

void CRptPlateGirder_IRC22::PrintShearResistance_ShearDgnForce(CMSExcel* pXL, int& iStRow, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strBaseSheet = _T("Shear_Resistance");
	CString strLcomName = GetLoadCombinationName(PrvsBase.OrgLcomK);

	// 	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_Design_Force"), iRowCnt, iColCnt);
	// 
	// 	pXL->SetValue(iStRow+1,10, strLcomName);
	// 	pXL->SetValue(iStRow+2, 5, PrvsBase.dFx*m_dFactorForce);
	// 	pXL->SetValue(iStRow+3, 5, PrvsBase.dMa_Ed*m_dFactorMoment);
	// 	pXL->SetValue(iStRow+4, 5, PrvsBase.dMy*m_dFactorMoment);
	// 	pXL->SetValue(iStRow+5, 5, PrvsGd.dFz*m_dFactorForce);
	// 	pXL->SetValue(iStRow+6, 5, PrvsBase.dFz*m_dFactorForce);
	// 	pXL->SetValue(iStRow+7, 5, PrvsBase.dV_Ed*m_dFactorForce);

	strBaseSheet = _T("I_Sect"); CString RptBlock = _T("I_SH_01_a") ;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
	short Row_Ref = 0, Col_Ref = 0;
	pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;

	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("SH_Lcom"), strLcomName, Row_Ref, Col_Ref, iStRow);

	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("SH_MD1"), PrvsBase.dMa_Ed*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("SH_MD2"), PrvsGd.dFz*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("SH_MD3"), PrvsBase.dMy*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("SH_Mu"), PrvsBase.dFz*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("SH_Vu"), PrvsBase.dFx*m_dFactorForce, Row_Ref, Col_Ref, iStRow);


	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintShearResistance_Vpl_Rd_Calc(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vpl_Rd_Calc"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 12, PrvsBase.dVpl_Rd*m_dFactorForce);
	pXL->SetValue(iStRow+1, 5, PrvsBase.dV_Rd*m_dFactorForce);
	pXL->SetValue(iStRow+2, 12, PrvsBase.dV_Ed*m_dFactorForce);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintShearResistance_Vpl_Rd_ChkRatio(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	if ( PrvsBase.dRatio>1.0 )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_V_Ed_Vpl_Rd_morehan_1"), iRowCnt, iColCnt);
	}
	else
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_V_Ed_Vpl_Rd_lessthan_1"), iRowCnt, iColCnt);
	}
	pXL->SetValue(iStRow, 6, PrvsBase.dRatio);
	iStRow += iRowCnt + 1;


	if ( PrvsBase.dRatio>0.6 )
	{
		//pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_V_Ed_Vpl_Rd_morehan_6"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 2, _T("V_Edi/V_Rd > 0.6")); pXL->SetValue(iStRow, 14, _T("High Shear Condition. Reduction on Plastic Moment"));
		iStRow += iRowCnt + 1;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vb_Rd_flange_Calc_Total"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow+1, 11, PrvsBase.m_dBetaFactHighShear);
		pXL->SetValue(iStRow+1, 17, PrvsBase.m_dBetaFactHighShear);
		pXL->SetValue(iStRow+2, 17, PrvsBase.m_dMfRd*m_dFactorMoment);
		pXL->SetValue(iStRow+3, 17, PrvsBase.m_dHighShearCond1*m_dFactorMoment);
		pXL->SetValue(iStRow+5, 14, PrvsBase.m_dHighDhearCond2*m_dFactorMoment);
		pXL->SetValue(iStRow+6, 14, dMpl_Rd);
	}
	else
	{
		pXL->SetValue(iStRow, 2, _T("V_Edi/V_Rd > 0.6")); pXL->SetValue(iStRow, 14, _T("Normal Shear Condition."));
	}



	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintShearResistance_Vbw_Rd_Calc(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 11, PrvsBase.dVbw_Rd*m_dFactorForce);
	pXL->SetValue(iStRow+2, 5, PrvsBase.dVbw_Rd*m_dFactorForce);
	pXL->SetValue(iStRow+3, 12, PrvsBase.dV_Ed*m_dFactorForce);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintShearResistance_Vbw_Rd_Calc_Sum(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc_Sum"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 15, PrvsBase.dVbw_Rd*m_dFactorForce);
	pXL->SetValue(iStRow+2, 5, PrvsBase.dVbw_Rd*m_dFactorForce);
	pXL->SetValue(iStRow+3, 12, PrvsBase.dV_Ed*m_dFactorForce);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintShearResistance_Vbw_Rd_Calc_Part(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_Vbw_Rd_Param& Vbw_Rd_Param)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc_Part"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 11, Vbw_Rd_Param.dVbw_Rd*m_dFactorForce);
	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintShearResistance_PlasticRestance(CMSExcel* pXL, int& iStRow, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;
	CString strBaseSheet = _T("Shear_Resistance");

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_Plastic_Moment_Positive"), iRowCnt, iColCnt);

	pXL->SetValue(iStRow, 7, PrvsDetail.dPlasticNA);
	pXL->SetValue(iStRow+2, 5, PrvsDetail.dN_slab	*	m_dFactorForce);
	pXL->SetValue(iStRow+3, 5, PrvsDetail.dNg_top	*	m_dFactorForce);
	pXL->SetValue(iStRow+4, 5, PrvsDetail.dNg_bot	*	m_dFactorForce);
	pXL->SetValue(iStRow+6, 5, PrvsBase.dMpl_Rd		*	m_dFactorMoment);

	iStRow += iRowCnt + 1;
}


void CRptPlateGirder_IRC22::PrintShearResistance_ShearBucklingCoeff(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	int iClassSect = PrvsBase.iSectClass;
	double dTaucre, dLambdaw;
	// Modify by GAY. PMS:XXXX. ('12.05.17). Shear BucklingÀÌ ÇÊ¿äÇÏ°í, Transverse Stiffener°¡ ¾ø´Â °æ¿ì, ÇÊ¿ä Msg.
	if ( m_nSectType==D_SECT_TYPE_COMPO_I || m_nSectType==D_SECT_TYPE_COMPO_B || m_nSectType==D_SECT_TYPE_COMPO_TUB )
	{
		int nSize  =  PrvsDetail.aGenWebVbwRdParam.GetSize();
		bool bFind = false;
		for ( int i = 0; i < nSize; ++i )
		{
			if ( bFind ) { continue; }

			T_PRVS_Vbw_Rd_Param& VbwRdParam = PrvsDetail.aGenWebVbwRdParam[i];
			if ( VbwRdParam.bIsShearBuck && VbwRdParam.iLamda_w_Type==0 )
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_Need_Transverse_Stiffener"), iRowCnt, iColCnt);
				iStRow += iRowCnt + 1;
				dTaucre = VbwRdParam.dk_tau_st_limit;
				dLambdaw = VbwRdParam.dLamda_w;
				bFind  = true;
			}
		}
	}

	if ( m_nSectType==D_SECT_TYPE_COMPO_G )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_M_Ed_BOX"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 5, PrvsBase.dM_Ed*m_dFactorMoment);
		iStRow += iRowCnt;
		PrintShearResistance_Mf_Rd(pXL, iStRow, strBaseSheet, PrvsBase, PrvsDetail);
	}

	CString strSubTitleType; strSubTitleType.Empty();
	//   strSubTitleType = (PrvsDetail.nShearBuckCount > 0) ? _T("Sub_Title_Shear_Calc_Vbw_Rd") : _T("Sub_Title_Shear_Calc_Vpl_Rd");
	//   pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, strSubTitleType, iRowCnt, iColCnt);
	iStRow += iRowCnt;

	if ( m_nSectType==D_SECT_TYPE_COMPO_I || m_nSectType==D_SECT_TYPE_COMPO_B || m_nSectType==D_SECT_TYPE_COMPO_TUB )
	{


		int nWebSize = PrvsDetail.aGenWebVbwRdParam.GetSize();

		double d_Alpha=0.0;
		double d_ktau=0.0;
		double iStiffSize=0.0;
		BOOL bShearBuckChk = FALSE;

		for ( int nWeb = 0; nWeb < nWebSize; ++nWeb )
		{
			if ( PrvsDetail.aGenWebVbwRdParam[nWeb].bIsShearBuck ) { bShearBuckChk = TRUE; }

			d_Alpha=max(d_Alpha, PrvsDetail.aGenWebVbwRdParam[nWeb].dAlpha);
			d_ktau=max(d_ktau, PrvsDetail.aGenWebVbwRdParam[nWeb].dk_tau);
			iStiffSize = max(iStiffSize, PrvsDetail.aGenWebVbwRdParam[nWeb].iStiffSize);
			dTaucre = PrvsDetail.aGenWebVbwRdParam[nWeb].dk_tau_st_limit;
			dLambdaw =PrvsDetail.aGenWebVbwRdParam[nWeb].dLamda_w;
			//VbwParam.iStiffSize

		}

		double dh   = m_dSize[0]; // C, webÀÇ ³ôÀÌ.
		double dtw  = m_dSize[1];
		double dB1  = m_dSize[2];
		double dtf1 = m_dSize[3];
		double dB2  = m_dSize[4];
		double dtf2 = m_dSize[5];

		double dtr = dh/dtw;
		double dEta = sqrt(250.0/m_dfy);
		double dvar2 = 67.0 * dEta / sqrt(d_ktau / 5.35);

		if ( bShearBuckChk ) // No need to check shear buckling, check only plastic resistance
		{
			// Title
			//PrintTitle_Sub_Title_Shear_Web(pXL, iStRow, strBaseSheet);
	// 		PrintShearResistance_Vpl_Rd_Calc(pXL, strBaseSheet, iStRow, PrvsBase);
	// 		PrintShearResistance_Vpl_Rd_ChkRatio(pXL, strBaseSheet, iStRow, PrvsBase);  

			//pXL->SetValue(iStRow  ,1, _T("'Check for Resistance to Shear force corresponding to web buckling "));
			iStRow++;

			strBaseSheet = _T("I_Sect"); CString RptBlock = _T("I_SH_02") ;
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt); iStRow++;
			RptBlock = _T("I_SH_02_01") ;
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt); iStRow++;

			if ( m_bTrstProvided )
			{
				if ( d_Alpha<1.0 )
				{
					RptBlock = _T("I_SH_02_Classify") ;
					pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
					short Row_Ref = 0, Col_Ref = 0;
					pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;

					PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("var1part1"), m_dTrstPitch, Row_Ref, Col_Ref, iStRow);
					PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("var2part1"), d_Alpha, Row_Ref, Col_Ref, iStRow);


					PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("var3part1"), dtr, Row_Ref, Col_Ref, iStRow);
					PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("var4part1"), dvar2, Row_Ref, Col_Ref, iStRow);
				}
				else if ( d_Alpha>=1.0 )
				{
					RptBlock = _T("I_SH_02_k_a") ;
					pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
					short Row_Ref = 0, Col_Ref = 0;
					pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;

					PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("var1part2"), m_dTrstPitch, Row_Ref, Col_Ref, iStRow);
					PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("var2part2"), d_Alpha, Row_Ref, Col_Ref, iStRow);


					PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("var3part2"), dtr, Row_Ref, Col_Ref, iStRow);
					PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("var4part2"), dvar2, Row_Ref, Col_Ref, iStRow);

				}
				iStRow += 12;
			}
			else
			{
				RptBlock = _T("I_SH_02_C_a") ;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
				short Row_Ref = 0, Col_Ref = 0;
				pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("var1part4"), dtr, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("var2part4"), dvar2 * sqrt(d_ktau / 5.35), Row_Ref, Col_Ref, iStRow);
				iStRow += 6;

			}
			//I_SH_02_Vn_b
	//     pXL->SetValue(iStRow  ,12, PrvsBase.dVpl_Rd*m_dFactorForce);
	//     pXL->SetValue(iStRow+1, 5, PrvsBase.dV_Rd*m_dFactorForce);
	//     pXL->SetValue(iStRow+2,12, PrvsBase.dV_Ed*m_dFactorForce);
			RptBlock = _T("I_SH_02_Vn_b") ;
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
			short Row_Ref = 0, Col_Ref = 0;
			pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("SH_Vp_02"), PrvsBase.dVb_Rd_org*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("SH_Vp_02_Av"), PrvsBase.dAv*m_dFactorRebarArea, Row_Ref, Col_Ref, iStRow);
			iStRow += 4;
			//SHear Buckling

			RptBlock = _T("I_SH_02_Vn_a") ;
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
			//short Row_Ref = 0, Col_Ref = 0;
			dLambdaw = sqrt(m_dFyw / (sqrt(3.0)*dTaucre));
			pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("taucre"), dTaucre, Row_Ref, Col_Ref, iStRow);
			PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("lambdaw"), dLambdaw, Row_Ref, Col_Ref, iStRow);

			iStRow += 8;
			if ( dLambdaw <= 0.8 )
			{
				//case 1
				double dPart1 = m_dFyw / sqrt(3.0);
				double dPart2 = PrvsBase.dAv * dPart1;
				RptBlock = _T("I_SH_02_Vn_b_01") ;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
				short Row_Ref = 0, Col_Ref = 0;
				pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("case1prt1"), dPart1, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("case1prt2"), dPart2, Row_Ref, Col_Ref, iStRow);
			}
			else if ( dLambdaw >= 1.2 )
			{ //case3
				double dPart1 = (m_dFyw / sqrt(3.0)) / (dLambdaw*dLambdaw);
				double dPart2 = PrvsBase.dAv * dPart1;
				RptBlock = _T("I_SH_02_Vn_b_01_b") ;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
				short Row_Ref = 0, Col_Ref = 0;
				pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("case3prt1"), dPart1, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("case3prt2"), dPart2, Row_Ref, Col_Ref, iStRow);
			}
			else
			{ //case 2
				double dPart1 = (m_dFyw / (sqrt(3.0))) * (1.0 - 0.8*(dLambdaw - 0.8));
				double dPart2 = PrvsBase.dAv * dPart1;
				RptBlock = _T("I_SH_02_Vn_b_01_a") ;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
				short Row_Ref = 0, Col_Ref = 0;
				pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("case2prt1"), dPart1, Row_Ref, Col_Ref, iStRow);
				PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("case2prt2"), dPart2*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
			}
			iStRow += 8;
			// 		if(d_Alpha<1.0)
			// 		{
			// 			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_lessthan_1"), iRowCnt, iColCnt);
			// 			pXL->SetValue(iStRow  ,12, d_ktau);
			// 			pXL->SetValue(iStRow  ,23, d_Alpha);
			// 			iStRow += iRowCnt;
			// 		}
			// 		else
			// 		{
			// 			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_morethan_1"), iRowCnt, iColCnt);
			// 			pXL->SetValue(iStRow  ,12, d_ktau);
			// 			pXL->SetValue(iStRow  ,23, d_Alpha);
			// 			iStRow += iRowCnt;
			// 		}
			// 
			// 
			// 
			// 		if (iStiffSize>0)
			// 		{
			// 			//Stiffeners are provided
			// 			pXL->SetValue(iStRow+1  ,2, _T("'d/tw > 67 Eta / SQRT(K_Tau)/5.35"));
			// 			iStRow++;
			// 			if (dtr > 67.0*dEta/(sqrt(d_ktau)/5.35))
			// 			{
			// 				pXL->SetValue(iStRow+2  ,2, dtr);pXL->SetValue(iStRow+2  ,6, _T("'>"));pXL->SetValue(iStRow+2  ,9,  67.0*dEta/(sqrt(d_ktau)/5.35));
			// 				pXL->SetValue(iStRow+3  ,2, _T("'NOT Governed by Plastic Shear Resistance"));
			// 			}
			// 			else
			// 			{
			// 				pXL->SetValue(iStRow+2  ,2, dtr);pXL->SetValue(iStRow+2  ,6, _T("'<"));pXL->SetValue(iStRow+2  ,9,  67.0*dEta/(sqrt(d_ktau)/5.35));
			// 				pXL->SetValue(iStRow+3  ,2, _T("'Governed by Plastic Shear Resistance"));
			// 			}
			// 		}
			// 		else
			// 		{
			// 			//Stiffeners are not provided NOT Stiffened
			// 			pXL->SetValue(iStRow+1  ,2, _T("'d/tw > 67 Eta"));
			// 			iStRow++;
			// 			if (dtr > 67.0*dEta)
			// 			{
			// 				pXL->SetValue(iStRow+2  ,2, dtr);pXL->SetValue(iStRow+2  ,6, _T("'>"));pXL->SetValue(iStRow+2  ,9,  67.0*dEta);
			// 				pXL->SetValue(iStRow+3  ,2, _T("'NOT Governed by Plastic Shear Resistance"));
			// 			}
			// 			else
			// 			{
			// 				pXL->SetValue(iStRow+2  ,2, dtr);pXL->SetValue(iStRow+2  ,6, _T("'<"));pXL->SetValue(iStRow+2  ,9,  67.0*dEta);
			// 				pXL->SetValue(iStRow+3  ,2, _T("'Governed by Plastic Shear Resistance"));
			// 			}
			// 		}

		}
		else// Need to check shear buckling
		{
			// Contribution of web 
			PrintShearResistance_ContributionWeb(pXL, iStRow, strBaseSheet, PrvsBase, PrvsDetail);

			// Contribution of flange
			PrintShearResistance_ContributionFlange(pXL, iStRow, strBaseSheet, PrvsBase, PrvsDetail);
		}

		// Check Result 
		PrintTitle_Sub_Title_Shear_ResistanceChk(pXL, iStRow, strBaseSheet);
		if ( PrvsBase.dRatio>1.0 ) { pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Ved_VRd_Ratio2"), iRowCnt, iColCnt); }
		else { pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Ved_VRd_Ratio"), iRowCnt, iColCnt); }
		pXL->SetValue(iStRow, 10, PrvsBase.dRatio);
		//


		iStRow += iRowCnt + 1;
		//Design strength under axial force by yielding
		double dPart1 = m_dArea_g*m_dFyt / m_CpgdD.dGamma_M0_IRC;

		CString RptBlock = _T("I_SH_02_Vn_c") ;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, _T("I_Sect"), RptBlock, iRowCnt, iColCnt);
		short Row_Ref = 0, Col_Ref = 0;
		pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;
		PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("SH_Vp_sh02"), dPart1/1000.0, Row_Ref, Col_Ref, iStRow);
		PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("SH_Vn_sh02"), m_dArea_g, Row_Ref, Col_Ref, iStRow);
		iStRow += iRowCnt + 1;
		//PrintShearResistance_Interation_M_V(pXL, iStRow, strBaseSheet, iClassSect, PrvsBase.dEta1, PrvsBase.dEta1_lim, PrvsBase.dEta3, PrvsBase.dCombinedEffe);
		//iStRow += iRowCnt + 1;
		//Interaction M-N
		double dRat = (PrvsBase.dV_Ed / dPart1) + m_dMomentRatio;// + ()
		RptBlock = _T("I_SH_03_02_02") ;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, _T("I_Sect"), RptBlock, iRowCnt, iColCnt);
		pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;
		PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("I_SH_vplrd"), PrvsBase.dVb_Rd_org*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
		PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("I_SH_vrd"), 0.0, Row_Ref, Col_Ref, iStRow);
		PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("I_SH_ved"), 0.0, Row_Ref, Col_Ref, iStRow);
		PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("I_SH_inter_ratio"), 0.0, Row_Ref, Col_Ref, iStRow);
		iStRow += 8;
	}
	else if ( m_nSectType == D_SECT_TYPE_COMPO_G )
	{
		PrintShearResistance_WebShearBucklingCoeff_GenSect(pXL, iStRow, strBaseSheet, PrvsBase, PrvsDetail);
		PrintShearResistance_FlangeShearBucklingCoeff_GenSect(pXL, iStRow, strBaseSheet, PrvsGd, PrvsBase, PrvsDetail);
	}
	else ASSERT(0);
	iStRow++;
}

void CRptPlateGirder_IRC22::PrintShearResistance_SBC_type1_lessthan_1(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, const T_PRVS_Vbw_Rd_Param& rVbwRdParam)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_lessthan_1"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 12, rVbwRdParam.dk_tau);
	pXL->SetValue(iStRow, 23, rVbwRdParam.dAlpha);

	iStRow += iRowCnt;
}

void CRptPlateGirder_IRC22::PrintShearResistance_SBC_type1_morethan_1(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, const T_PRVS_Vbw_Rd_Param& rVbwRdParam)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_morethan_1"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 12, rVbwRdParam.dk_tau);
	pXL->SetValue(iStRow, 23, rVbwRdParam.dAlpha);

	iStRow += iRowCnt;
}

void CRptPlateGirder_IRC22::PrintShearResistance_SBC_type1_calc_param(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, const T_PRVS_Vbw_Rd_Param& rVbwRdParam)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_calc_param"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 7, rVbwRdParam.dhw);
	pXL->SetValue(iStRow+1, 10, rVbwRdParam.dk_tau_st_limit);
	pXL->SetValue(iStRow+2, 21, rVbwRdParam.dk_tau_st);
	pXL->SetValue(iStRow+3, 7, rVbwRdParam.dIsl);
	pXL->SetValue(iStRow+4, 7, rVbwRdParam.dt);
	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintShearResistance_SBC_type2(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, const T_PRVS_Vbw_Rd_Param& rVbwRdParam)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type2"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 7, rVbwRdParam.dAlpha);
	pXL->SetValue(iStRow+1, 20, rVbwRdParam.dk_tau);
	pXL->SetValue(iStRow+2, 7, rVbwRdParam.dIsl);
	pXL->SetValue(iStRow+3, 7, rVbwRdParam.dt);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintShearResistance_Vbf_Rd_Calc(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow,
	T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail, const T_PRVS_Vbw_Rd_Param& rVbwRdParam)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbf_Rd_Calc"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 17, rVbwRdParam.dVbf_Rd*m_dFactorForce);
	pXL->SetValue(iStRow+2, 8, PrvsDetail.dMf_N_Rd*m_dFactorMoment);
	pXL->SetValue(iStRow+3, 8, PrvsBase.dM_Ed*m_dFactorMoment);
	pXL->SetValue(iStRow+4, 18, rVbwRdParam.dc);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintShearResistance_Vbf_Rd_Calc_Sum(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbf_Rd_Calc_Sum"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 15, PrvsBase.dVbf_Rd*m_dFactorForce);

	iStRow += iRowCnt + 1;
}


void CRptPlateGirder_IRC22::PrintShearResistance_WebShearBucklingCoeff_GenSect(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	int iWebPartSize=PrvsDetail.aGenWebVbwRdParam.GetSize();
	if ( iWebPartSize<=0 ) return;
	CString strPartType;
	CString strPartName=_T("");
	int nPartType = 0;
	int i=0;
	int iClassSect = PrvsBase.iSectClass;
	int iEndSupport = PrvsDetail.iEndSupportType;
	for ( i=0; i<iWebPartSize; i++ )
	{
		T_PRVS_Vbw_Rd_Param VbwParam = PrvsDetail.aGenWebVbwRdParam[i];
		nPartType = VbwParam.nPartType;
		strPartType = Get_String4PartShape(nPartType);
		CString strName = VbwParam.strGroupName;
		strPartName.Format(_T("%s(%s)"), strPartType, strName);
		pXL->SetValue(iStRow, 2, strPartName);
		pXL->SetFont(iStRow, 0, iStRow, 32, 10, 0, m_FontType, FALSE);
		iStRow++;
		double dAlpha = VbwParam.dAlpha;
		int iLongStiffSize = VbwParam.iStiffSize;
		if ( iLongStiffSize==1 || iLongStiffSize==2 ) // one or two longitudinal stiffeners
		{
			if ( dAlpha<3.0 )
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type2"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 7, VbwParam.dAlpha);
				pXL->SetValue(iStRow+1, 20, VbwParam.dk_tau);
				pXL->SetValue(iStRow+2, 7, VbwParam.dIsl);
				pXL->SetValue(iStRow+3, 7, VbwParam.dt);
				iStRow += iRowCnt + 1;
			}
			else
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_morethan_1"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 12, VbwParam.dk_tau);
				pXL->SetValue(iStRow, 23, VbwParam.dAlpha);
				iStRow += iRowCnt;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_calc_param"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 7, VbwParam.dhw);
				pXL->SetValue(iStRow+1, 10, VbwParam.dk_tau_st_limit);
				pXL->SetValue(iStRow+2, 21, VbwParam.dk_tau_st);
				pXL->SetValue(iStRow+3, 7, VbwParam.dIsl);
				pXL->SetValue(iStRow+4, 7, VbwParam.dt);
				iStRow += iRowCnt + 1;
			}
		}
		else if ( iLongStiffSize==0 || iLongStiffSize>2 ) // without longitudinal stiffeners or with more than two longitudinal stiffeners
		{
			if ( iLongStiffSize==0 && VbwParam.iLamda_w_Type!=2 )
			{
				//
			}
			else
			{
				if ( dAlpha<1.0 )
				{
					pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_lessthan_1"), iRowCnt, iColCnt);
					pXL->SetValue(iStRow, 12, VbwParam.dk_tau);
					pXL->SetValue(iStRow, 23, VbwParam.dAlpha);
					iStRow += iRowCnt;
				}
				else
				{
					pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_morethan_1"), iRowCnt, iColCnt);
					pXL->SetValue(iStRow, 12, VbwParam.dk_tau);
					pXL->SetValue(iStRow, 23, VbwParam.dAlpha);
					iStRow += iRowCnt;
				}
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_calc_param"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 7, VbwParam.dhw);
				pXL->SetValue(iStRow+1, 10, VbwParam.dk_tau_st_limit);
				pXL->SetValue(iStRow+2, 21, VbwParam.dk_tau_st);
				pXL->SetValue(iStRow+3, 7, VbwParam.dIsl);
				pXL->SetValue(iStRow+4, 7, VbwParam.dt);
				iStRow += iRowCnt + 1;
			}

		}

		if ( VbwParam.bIsShearBuck )
		{
			if ( VbwParam.iLamda_w_Type==0 )
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_Need_Transverse_Stiffener"), iRowCnt, iColCnt);
				iStRow += iRowCnt + 1;
			}

			CString strLamdawType = Get_String4ShearLamdaw(TRUE, VbwParam.iLamda_w_Type);
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strLamdawType, iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 11, VbwParam.dLamda_w);
			iStRow += iRowCnt + 1;

			if ( iLongStiffSize > 0 && VbwParam.dLamda_w < VbwParam.Lamda_w_SubD.dLamda_w )
			{
				PrintShearResistance_SubPanel_Lamda_w(pXL, iStRow, strBaseSheet, VbwParam.Lamda_w_SubD);
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_Lamda_w_check"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow+1, 9, VbwParam.dLamda_w_cr);
				iStRow += iRowCnt + 1;
			}

			// Contribution web
			int iLamda_w_scope = VbwParam.iLamda_w_scope;
			CString strXwType = Get_String4ShearXwType(TRUE, iEndSupport, iLamda_w_scope);
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strXwType, iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 10, VbwParam.dXw);
			iStRow += iRowCnt +1;
			// Vbw_Rd_Calc
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 11, VbwParam.dVbw_Rd*m_dFactorForce);
			pXL->SetValue(iStRow+2, 5, VbwParam.dV_Rd*m_dFactorForce);
			pXL->SetValue(iStRow+3, 12, VbwParam.dV_Edi*m_dFactorForce);
			iStRow += iRowCnt;
			if ( VbwParam.deta3>1.0 )
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc_eta3_morethan_1"), iRowCnt, iColCnt);
			else
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc_eta3_lessthan_1"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 9, VbwParam.deta3);
			iStRow += iRowCnt + 1;
		}
		else
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vpl_Rd_Calc"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 12, VbwParam.dVpl_Rd*m_dFactorForce);
			pXL->SetValue(iStRow+1, 5, VbwParam.dV_Rd*m_dFactorForce);
			pXL->SetValue(iStRow+2, 12, VbwParam.dV_Edi*m_dFactorForce);
			iStRow += iRowCnt + 1;
			if ( VbwParam.deta3>1.0 )
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_V_Ed_Vpl_Rd_morehan_1"), iRowCnt, iColCnt);
			else
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_V_Ed_Vpl_Rd_lessthan_1"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 6, VbwParam.deta3);
			iStRow += iRowCnt + 1;

			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vb_Rd_flange_Calc_Total"), iRowCnt, iColCnt);
			CString str;
			str.Format(_T("'%g"), PrvsBase.dRatio);
			if ( PrvsBase.dRatio>0.6 )
			{

				//pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_V_Ed_Vpl_Rd_morehan_6"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 2, _T("V_Edi/V_Rd =")); pXL->SetValue(iStRow, 6, str); pXL->SetValue(iStRow, 9, _T(" > 0.6")); pXL->SetValue(iStRow, 14, _T("High Shear Condition. Check"));

				//Check for High Shear
				//Shear_resistance_Vb_Rd_flange_Calc_Total
			}
			else
			{
				pXL->SetValue(iStRow, 2, _T("V_Edi/V_Rd =")); pXL->SetValue(iStRow, 6, str); pXL->SetValue(iStRow, 9, _T(" < 0.6")); pXL->SetValue(iStRow, 14, _T("Normal Shear Condition."));
			}


			iStRow += iRowCnt + 1;
		}
		//PrintShearResistance_Interation_M_V(pXL, iStRow, strBaseSheet, iClassSect, PrvsBase.dEta1, PrvsBase.dEta1_lim, VbwParam.deta3, VbwParam.dRatio_InteractionMV); 
	}

}

void CRptPlateGirder_IRC22::PrintShearResistance_FlangeShearBucklingCoeff_GenSect(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	int iPartSize=PrvsDetail.aGenFlangeVbwRdParam.GetSize();
	if ( iPartSize<=0 ) return;
	CString strPartType;
	CString strPartName=_T("");
	int nPartType = 0;

	int i=0;
	for ( i=0; i<iPartSize; i++ )
	{
		T_PRVS_Vbw_Rd_Param VbwParam = PrvsDetail.aGenFlangeVbwRdParam[i];
		nPartType = VbwParam.nPartType;
		strPartType = Get_String4PartShape(nPartType);
		CString strName = VbwParam.strGroupName;
		strPartName.Format(_T("%s(%s)"), strPartType, strName);
		pXL->SetFont(iStRow, 0, iStRow, 32, 10, 0, m_FontType, FALSE);
		pXL->SetValue(iStRow, 2, strPartName);
		iStRow++;

		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_flange_stress_G_shape"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 12, VbwParam.dtau_Ed);
		pXL->SetValue(iStRow+1, 7, PrvsGd.dFz*m_dFactorForce);
		pXL->SetValue(iStRow+2, 7, VbwParam.dQi);
		pXL->SetValue(iStRow+3, 7, VbwParam.dIyyi);
		pXL->SetValue(iStRow+4, 7, VbwParam.dt);
		iStRow += iRowCnt+1;

		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_flange_stress_After_G_shape"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 12, VbwParam.dtau_Ed_c);
		pXL->SetValue(iStRow+1, 7, PrvsBase.dFz*m_dFactorForce);
		pXL->SetValue(iStRow+2, 7, VbwParam.dQi_c);
		pXL->SetValue(iStRow+3, 7, VbwParam.dIyyi_c);
		pXL->SetValue(iStRow+4, 7, VbwParam.dt);
		iStRow += iRowCnt+1;

		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_flange_stress_sum"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 12, VbwParam.dtau_Ed_sum);
		pXL->SetValue(iStRow+1, 9, VbwParam.dtau_Ed_ave);
		iStRow += iRowCnt+1;

		// Vbw_Rd_Calc
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vb_Rd_flange_Calc"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 11, VbwParam.dtau_b_Rd);
		iStRow += iRowCnt;

		if ( VbwParam.deta3>1.0 )
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vb_Rd_flange_Calc_eta3_morethan_1"), iRowCnt, iColCnt);
		else
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vb_Rd_flange_Calc_eta3_lessthan_1"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 9, VbwParam.deta3);
		iStRow += iRowCnt + 1;

	}

}

void CRptPlateGirder_IRC22::PrintShearResistance_ContributionWeb(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	// contribution from the web
	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_contribution_web"), iRowCnt, iColCnt);
	iStRow += iRowCnt;

	bool bBoxSect = (m_nSectType==D_SECT_TYPE_COMPO_B || m_nSectType==D_SECT_TYPE_COMPO_TUB);
	int nWebSize = PrvsDetail.aGenWebVbwRdParam.GetSize();

	for ( int nWeb = 0; nWeb < nWebSize; ++nWeb )
	{
		if ( bBoxSect && nWeb == 0 ) { PrintTitle_Sub_Title_Shear_LeftWeb(pXL, iStRow, strBaseSheet); }
		if ( bBoxSect && nWeb == 1 ) { PrintTitle_Sub_Title_Shear_Right_Web(pXL, iStRow, strBaseSheet); }

		// Shear Buckling Coeff.
		T_PRVS_Vbw_Rd_Param& VbwRdParam = PrvsDetail.aGenWebVbwRdParam[nWeb];

		double dAlpha			= VbwRdParam.dAlpha;
		int iLongStiffSize	= VbwRdParam.iStiffSize;

		if ( iLongStiffSize == 1 || iLongStiffSize == 2 ) // one or two longitudinal stiffeners
		{
			if ( dAlpha < 3.0 )
			{
				PrintShearResistance_SBC_type2(pXL, strBaseSheet, iStRow, VbwRdParam);
			}
			else
			{
				PrintShearResistance_SBC_type1_morethan_1(pXL, strBaseSheet, iStRow, VbwRdParam);
				PrintShearResistance_SBC_type1_calc_param(pXL, strBaseSheet, iStRow, VbwRdParam);
			}
		}
		else if ( iLongStiffSize == 0 || iLongStiffSize > 2 ) // without longitudinal stiffeners or with more than two longitudinal stiffeners
		{
			if ( iLongStiffSize == 0 )
			{
				// Print Nothing.. ???
			}
			else
			{
				if ( dAlpha < 1.0 )
				{
					PrintShearResistance_SBC_type1_lessthan_1(pXL, strBaseSheet, iStRow, VbwRdParam);
				}
				else
				{
					PrintShearResistance_SBC_type1_morethan_1(pXL, strBaseSheet, iStRow, VbwRdParam);
				}

				PrintShearResistance_SBC_type1_calc_param(pXL, strBaseSheet, iStRow, VbwRdParam);
			}
		}
		else
		{
			ASSERT(0);
		}

		//
		CString strXwType		= _T("");
		int iEndSupportType	= PrvsDetail.iEndSupportType;
		int iLamda_w_scope	= VbwRdParam.iLamda_w_scope;

		CString strLamdawType = Get_String4ShearLamdaw(TRUE, VbwRdParam.iLamda_w_Type);
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strLamdawType, iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 11, VbwRdParam.dLamda_w);
		iStRow += iRowCnt + 1;

		if ( iLongStiffSize > 0 && VbwRdParam.dLamda_w < VbwRdParam.Lamda_w_SubD.dLamda_w )
		{
			PrintShearResistance_SubPanel_Lamda_w(pXL, iStRow, strBaseSheet, VbwRdParam.Lamda_w_SubD);
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_Lamda_w_check"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow+1, 9, VbwRdParam.dLamda_w_cr);
			iStRow += iRowCnt + 1;
		}

		strXwType = Get_String4ShearXwType(TRUE, iEndSupportType, iLamda_w_scope);
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strXwType, iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 10, VbwRdParam.dXw);
		iStRow += iRowCnt + 1;

		if ( bBoxSect ) { PrintShearResistance_Vbw_Rd_Calc_Part(pXL, strBaseSheet, iStRow, VbwRdParam); }
	}

	// Vbw_Rd_Calc
	if ( bBoxSect ) { PrintShearResistance_Vbw_Rd_Calc_Sum(pXL, strBaseSheet, iStRow, PrvsBase); }
	else
	{
		PrintShearResistance_Vbw_Rd_Calc(pXL, strBaseSheet, iStRow, PrvsBase);
	}

	if ( PrvsBase.dEta3>1.0 )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc_eta3_morethan_1"), iRowCnt, iColCnt);
	}
	else
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc_eta3_lessthan_1"), iRowCnt, iColCnt);
	}
	pXL->SetValue(iStRow, 9, PrvsBase.dEta3);
	iStRow += iRowCnt + 1;

}

void CRptPlateGirder_IRC22::PrintShearResistance_SubPanel_Lamda_w(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_Vbw_Rd_Sub_Panel& VbwSubPanel)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_sub_panel_Sub_Title"), iRowCnt, iColCnt);
	iStRow += iRowCnt;

	if ( VbwSubPanel.nLongStfnSize==1 || VbwSubPanel.nLongStfnSize==2 ) // 0=Á¾¹æÇâ º¸°­Àç 1 ¶Ç´Â 2°³ Æ÷ÇÔÇÏ´Â ÆÐ³Î,
	{
		if ( VbwSubPanel.dAlpha<3.0 )
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type2_sub_ls"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 7, VbwSubPanel.dAlpha);
			pXL->SetValue(iStRow+1, 20, VbwSubPanel.dk_tau);
			pXL->SetValue(iStRow+2, 7, VbwSubPanel.dIsl);
			pXL->SetValue(iStRow+3, 7, VbwSubPanel.dt);
			iStRow += iRowCnt + 1;
		}
		else
		{
			if ( VbwSubPanel.dAlpha<1.0 )
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_lessthan_1"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 12, VbwSubPanel.dk_tau);
				pXL->SetValue(iStRow, 23, VbwSubPanel.dAlpha);
				iStRow += iRowCnt;
			}
			else
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_morethan_1"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 12, VbwSubPanel.dk_tau);
				pXL->SetValue(iStRow, 23, VbwSubPanel.dAlpha);
				iStRow += iRowCnt;
			}

			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_ls_calc_param"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 7, VbwSubPanel.dhw);
			pXL->SetValue(iStRow+1, 10, VbwSubPanel.dk_tau_st_lt);
			pXL->SetValue(iStRow+2, 21, VbwSubPanel.dk_tau);
			pXL->SetValue(iStRow+3, 7, VbwSubPanel.dIsl);
			pXL->SetValue(iStRow+4, 7, VbwSubPanel.dt);
			iStRow += iRowCnt + 1;
		}
	}
	else //1= ¾ø°Å³ª 3°³ÀÌ»ó
	{
		if ( VbwSubPanel.dAlpha<1.0 )
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_lessthan_1"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 12, VbwSubPanel.dk_tau);
			pXL->SetValue(iStRow, 23, VbwSubPanel.dAlpha);
			iStRow += iRowCnt;
		}
		else
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_morethan_1"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 12, VbwSubPanel.dk_tau);
			pXL->SetValue(iStRow, 23, VbwSubPanel.dAlpha);
			iStRow += iRowCnt;
		}

		if ( VbwSubPanel.nLongStfnSize==0 )
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_calc_param"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 7, VbwSubPanel.dhw);
			iStRow += iRowCnt + 1;
		}
		else
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_ls_calc_param"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 7, VbwSubPanel.dhw);
			pXL->SetValue(iStRow+1, 10, VbwSubPanel.dk_tau_st_lt);
			pXL->SetValue(iStRow+2, 21, VbwSubPanel.dk_tau_st);
			pXL->SetValue(iStRow+3, 7, VbwSubPanel.dIsl);
			pXL->SetValue(iStRow+4, 7, VbwSubPanel.dt);
			iStRow += iRowCnt + 1;
		}
	}

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_Lamda_w_sub_type3"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 12, VbwSubPanel.dLamda_w);
	iStRow += iRowCnt + 1;


}

void CRptPlateGirder_IRC22::PrintShearResistance_ContributionFlange(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	// Title - contribution from the Flange
	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_contribution_Flange"), iRowCnt, iColCnt);
	iStRow += iRowCnt;

	PrintShearResistance_Mf_Rd(pXL, iStRow, strBaseSheet, PrvsBase, PrvsDetail);

	bool bBoxSect = (m_nSectType==D_SECT_TYPE_COMPO_B || m_nSectType==D_SECT_TYPE_COMPO_TUB);
	int nWebSize = PrvsDetail.aGenWebVbwRdParam.GetSize();

	for ( int nWeb = 0; nWeb < nWebSize; ++nWeb )
	{
		if ( bBoxSect && nWeb == 0 ) { PrintTitle_Sub_Title_Shear_LeftWeb(pXL, iStRow, strBaseSheet); }
		if ( bBoxSect && nWeb == 1 ) { PrintTitle_Sub_Title_Shear_Right_Web(pXL, iStRow, strBaseSheet); }

		T_PRVS_Vbw_Rd_Param& VbwRdParam = PrvsDetail.aGenWebVbwRdParam[nWeb];
		PrintShearResistance_Vbf_Rd_Calc(pXL, strBaseSheet, iStRow, PrvsBase, PrvsDetail, VbwRdParam);
	}

	if ( bBoxSect ) { PrintShearResistance_Vbf_Rd_Calc_Sum(pXL, strBaseSheet, iStRow, PrvsBase); }

}

void CRptPlateGirder_IRC22::PrintShearResistance_Interation_M_V(CMSExcel* pXL, int& iStRow, CString strBaseSheet, int iClassSect, double deta1, double dEta1_lim, double deta3, double dRatio)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	// Sub Title
	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_interation_M_V"), iRowCnt, iColCnt);
	iStRow += iRowCnt;

	if ( iClassSect==1 || iClassSect==2 )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_interation_M_V_Class_1or2"), iRowCnt, iColCnt);
		iStRow += iRowCnt + 1;
	}
	else if ( iClassSect==3 || iClassSect==4 )
	{
		if ( deta3 < 0.5 )
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_interation_M_V_eta3_lessthan_05"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 4, deta3);
			iStRow += iRowCnt + 1;
		}
		else if ( deta1 < dEta1_lim )
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_interation_M_V_eta1_lessthan_Limit"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 4, deta1);
			pXL->SetValue(iStRow, 15, dEta1_lim);
			iStRow += iRowCnt + 1;
		}
		else
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_interation_M_V_eta3_morthan_05"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 4, deta3);
			iStRow += iRowCnt;

			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_interation_M_V_eta1"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 4, deta1);
			iStRow += iRowCnt;
			if ( dRatio>1.0 )
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_interation_M_V_morethan_1"), iRowCnt, iColCnt);
			}
			else
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_interation_M_V"), iRowCnt, iColCnt);
			}

			pXL->SetValue(iStRow, 12, dRatio);
			iStRow += iRowCnt + 1;
		}
	}
	else ASSERT(0);
}

void CRptPlateGirder_IRC22::PrintShearResistance_Mf_Rd(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	if ( PrvsBase.dN_Ed<0.0 )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Mf_Rd0"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 5, PrvsDetail.dMf_Rd_cal*m_dFactorMoment);
		pXL->SetValue(iStRow+2, 18, PrvsDetail.dReduFactor4N_Ed);
		pXL->SetValue(iStRow+4, 18, PrvsDetail.dMf_N_Rd*m_dFactorMoment);
		iStRow += iRowCnt + 1;
	}
	else
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Mf_Rd"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 5, PrvsDetail.dMf_Rd_cal*m_dFactorMoment);
		iStRow += iRowCnt + 1;
	}
}

// Lateral Torsional Buckling Check Result Print
void CRptPlateGirder_IRC22::PrintLateralTorsionalBucklingResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PLTB_BASE& PltbGd, T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;
	//if (PltbBase.dM_Ed<m_dZero) return;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("LTB_Resistance");
	// Title
	pXL->CopyRange(iStRow, 1, m_strBaseBook, _T("I_Sect"), _T("Title_of_LTBResistance"), iRowCnt, iColCnt);

	// pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Title_of_LTBResistance"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, m_iLTBChapterIx);
	iStRow += iRowCnt;

	CString strLcomName = GetLoadCombinationName(PltbBase.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("LTB_Design_Force"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+1, 10, strLcomName);
	pXL->SetValue(iStRow+2, 5, PltbBase.dN_Ed*m_dFactorForce);
	pXL->SetValue(iStRow+3, 5, PltbBase.dM_Ed*m_dFactorMoment);
	pXL->SetValue(iStRow+4, 5, PltbBase.dV1  *m_dFactorForce);
	pXL->SetValue(iStRow+5, 5, PltbBase.dV2  *m_dFactorForce);
	pXL->SetValue(iStRow+6, 5, PltbBase.dM1  *m_dFactorMoment);
	pXL->SetValue(iStRow+7, 5, PltbBase.dM2  *m_dFactorMoment);
	pXL->SetValue(iStRow+8, 5, PltbDetail.dMpl_Rd*m_dFactorMoment);
	pXL->SetValue(iStRow+9, 5, PltbDetail.dMel_Rd*m_dFactorMoment);
	iStRow +=11;//= iRowCnt + 1;

//   // Mb,Rd Buckling Resistance Moment
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("LTB_Mb_Rd_Buckling_Resistance"), iRowCnt, iColCnt);
//   pXL->SetValue(iStRow+ 1, 5, PltbDetail.dL * m_dFactorLength);
//   //pXL->SetValue(iStRow+ 2, 8, PltbBase.dcc);
//   //pXL->SetValue(iStRow+ 3,10, PltbBase.dgamma);
//  // pXL->SetValue(iStRow+ 4, 9, PltbBase.dMu);
//   //pXL->SetValue(iStRow+ 5,12, PltbBase.dPHI);
//  // pXL->SetValue(iStRow+ 6,21, PltbBase.dm1);
//  // pXL->SetValue(iStRow+ 7,21, PltbBase.dm2);
//   pXL->SetValue(iStRow+ 2,5, PltbBase.dMcr*m_dFactorMoment);
//   pXL->SetValue(iStRow+ 3, 5, PltbBase.dAlpha_LT);
//   pXL->SetValue(iStRow+4,17, PltbBase.dLamda_LT);
//   pXL->SetValue(iStRow+5,15, PltbBase.dPHI_LT);
//   pXL->SetValue(iStRow+6,12, PltbBase.dX_LT);  
//   pXL->SetValue(iStRow+8, 5, PltbBase.dM_Rd*m_dFactorMoment);
//   pXL->SetValue(iStRow+9, 9, PltbBase.dMb_Rd*m_dFactorMoment);
//   iStRow += iRowCnt + 1;

	CString RptBlock = _T("LTB_Common") ;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
	short Row_Ref = 0, Col_Ref = 0;
	// ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
	pXL->GetCellIDByName(m_strBaseBook, strBaseSheet, RptBlock, Row_Ref, Col_Ref);
	Col_Ref-=1;

	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_1"), PltbBase.dLamda_LT, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_2"), m_dLentot, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_3"), PltbBase.dMcr*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_4"), PltbBase.dM_Rd*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_5"), PltbBase.dX_LT, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_6"), PltbBase.dAlpha_LT, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_mced"), PltbBase.dM_Ed*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);

	// Nb,Rd Buckling Resistance Moment
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("LTB_Nb_Rd_Buckling_Resistance"), iRowCnt, iColCnt);
//   pXL->SetValue(iStRow+ 1, 5, PltbBase.dX_LT_N);
//   pXL->SetValue(iStRow+ 2,11, PltbBase.dNb_Rd*m_dFactorForce);
	//pXL->SetValue(iStRow+ 4,14, PltbBase.dInterRat);
	iStRow += 20;

	// U frame model
//  pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("LTB_U_frame_model"), iRowCnt, iColCnt);
//  pXL->SetValue(iStRow+ 1, 4, PltbDetail.iAlpha);
//  pXL->SetValue(iStRow+ 2, 4, PltbDetail.dhs);
//  pXL->SetValue(iStRow+ 3, 4, PltbDetail.da);
//  pXL->SetValue(iStRow+ 4, 4, PltbDetail.dEa);
//  pXL->SetValue(iStRow+ 5, 4, PltbDetail.dGs);
//  pXL->SetValue(iStRow+ 6, 4, PltbDetail.dI2);
//  pXL->SetValue(iStRow+ 7, 9, PltbBase.dk1);
//  pXL->SetValue(iStRow+ 8,12, PltbBase.dk2);
//  pXL->SetValue(iStRow+ 9,10, PltbBase.dks);
//  pXL->SetValue(iStRow+11, 5, PltbBase.dC4);
//  pXL->SetValue(iStRow+12, 5, PltbBase.dI_at);
//  pXL->SetValue(iStRow+13, 5, PltbBase.dI_afz);
//  pXL->SetValue(iStRow+14, 5, PltbBase.dkc);
//  pXL->SetValue(iStRow+15, 5, PltbBase.dMcr*m_dFactorMoment);
//  iStRow += iRowCnt + 1;

}

void CRptPlateGirder_IRC22::PrintLateralTorsionalBucklingResistance_Neg(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PLTB_BASE& PltbGd, T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;
	//if (PltbBase.dM_Ed<m_dZero) return;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("LTB_Resistance");
	// Title
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Title_of_LTBResistance"), iRowCnt, iColCnt);
//   pXL->SetValue(iStRow, 1, m_iLTBChapterIx);
	iStRow += iRowCnt;

	CString strLcomName = GetLoadCombinationName(PltbBase.OrgLcomK);
	//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("LTB_neg"), iRowCnt, iColCnt);
	//   short Row_Ref = 0, Col_Ref = 0; 
	//   // ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
	//   pXL->GetCellIDByName(m_strBaseBook, strBaseSheet, _T("LTB_neg_1"), Row_Ref, Col_Ref);Col_Ref-=1;
	//   PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_neg_1")   ,strLcomName, Row_Ref, Col_Ref, iStRow);

	CString RptBlock = _T("LTB_neg") ;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
	short Row_Ref = 0, Col_Ref = 0;
	// ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
	pXL->GetCellIDByName(m_strBaseBook, strBaseSheet, RptBlock, Row_Ref, Col_Ref);
	Col_Ref-=1;
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_neg_1"), strLcomName, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_neg_2"), PltbDetail.da*m_dFactorForce/1000.0, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_neg_3"), PltbDetail.dEa*m_dFactorForce/1000.0, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_neg_4"), PltbDetail.da*m_dFactorForce/1000.0, Row_Ref, Col_Ref, iStRow);

	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_neg_5"), PltbDetail.dMel_Rd, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_neg_6"), m_dLentot, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_neg_7"), PltbDetail.dMpl_Rd, Row_Ref, Col_Ref, iStRow);

	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_neg_10"), PltbDetail.dI2, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_neg_11"), PltbDetail.dhs, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_neg_8"), PltbDetail.dhs* PltbDetail.dEa*m_dFactorMoment/1000.0, Row_Ref, Col_Ref, iStRow);



	iStRow += iRowCnt + 1;

	//   // Mb,Rd Buckling Resistance Moment
	//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("LTB_Mb_Rd_Buckling_Resistance"), iRowCnt, iColCnt);
	//   pXL->SetValue(iStRow+ 1, 5, PltbDetail.dL * m_dFactorLength);
	//   //pXL->SetValue(iStRow+ 2, 8, PltbBase.dcc);
	//   //pXL->SetValue(iStRow+ 3,10, PltbBase.dgamma);
	//  // pXL->SetValue(iStRow+ 4, 9, PltbBase.dMu);
	//   //pXL->SetValue(iStRow+ 5,12, PltbBase.dPHI);
	//  // pXL->SetValue(iStRow+ 6,21, PltbBase.dm1);
	//  // pXL->SetValue(iStRow+ 7,21, PltbBase.dm2);
	//   pXL->SetValue(iStRow+ 2,5, PltbBase.dMcr*m_dFactorMoment);
	//   pXL->SetValue(iStRow+ 3, 5, PltbBase.dAlpha_LT);
	//   pXL->SetValue(iStRow+4,17, PltbBase.dLamda_LT);
	//   pXL->SetValue(iStRow+5,15, PltbBase.dPHI_LT);
	//   pXL->SetValue(iStRow+6,12, PltbBase.dX_LT);  
	//   pXL->SetValue(iStRow+8, 5, PltbBase.dM_Rd*m_dFactorMoment);
	//   pXL->SetValue(iStRow+9, 9, PltbBase.dMb_Rd*m_dFactorMoment);
//  iStRow += iRowCnt + 1;

//    RptBlock = _T(_T("LTB_Common")     );   
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
//   //short Row_Ref = 0, Col_Ref = 0; 
//   // ±âÁØÀÌ µÇ´Â Block ½ÃÀÛÁ¡ °¡Á®¿Â´Ù.
//   pXL->GetCellIDByName(m_strBaseBook, strBaseSheet, RptBlock, Row_Ref, Col_Ref);
//   Col_Ref-=1;
// 
//   PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_1")   ,PltbBase.dLamda_LT, Row_Ref, Col_Ref, iStRow);
//   PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_2")   ,PltbDetail.dL * m_dFactorLength, Row_Ref, Col_Ref, iStRow);
//   PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_3")   ,PltbBase.dMcr*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);
//   PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_4")   ,PltbBase.dM_Rd*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);
//   PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_5")   ,PltbBase.dX_LT, Row_Ref, Col_Ref, iStRow);
//   PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_6")   ,PltbBase.dAlpha_LT, Row_Ref, Col_Ref, iStRow);
//   PrintCellData(pXL, m_strBaseBook, strBaseSheet, _T("LTB_mced")   ,PltbBase.dM_Ed*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);

	// Nb,Rd Buckling Resistance Moment
	//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("LTB_Nb_Rd_Buckling_Resistance"), iRowCnt, iColCnt);
	//   pXL->SetValue(iStRow+ 1, 5, PltbBase.dX_LT_N);
	//   pXL->SetValue(iStRow+ 2,11, PltbBase.dNb_Rd*m_dFactorForce);
	//pXL->SetValue(iStRow+ 4,14, PltbBase.dInterRat);
	//iStRow += 16;

	// U frame model
	//  pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("LTB_U_frame_model"), iRowCnt, iColCnt);
	//  pXL->SetValue(iStRow+ 1, 4, PltbDetail.iAlpha);
	//  pXL->SetValue(iStRow+ 2, 4, PltbDetail.dhs);
	//  pXL->SetValue(iStRow+ 3, 4, PltbDetail.da);
	//  pXL->SetValue(iStRow+ 4, 4, PltbDetail.dEa);
	//  pXL->SetValue(iStRow+ 5, 4, PltbDetail.dGs);
	//  pXL->SetValue(iStRow+ 6, 4, PltbDetail.dI2);
	//  pXL->SetValue(iStRow+ 7, 9, PltbBase.dk1);
	//  pXL->SetValue(iStRow+ 8,12, PltbBase.dk2);
	//  pXL->SetValue(iStRow+ 9,10, PltbBase.dks);
	//  pXL->SetValue(iStRow+11, 5, PltbBase.dC4);
	//  pXL->SetValue(iStRow+12, 5, PltbBase.dI_at);
	//  pXL->SetValue(iStRow+13, 5, PltbBase.dI_afz);
	//  pXL->SetValue(iStRow+14, 5, PltbBase.dkc);
	//  pXL->SetValue(iStRow+15, 5, PltbBase.dMcr*m_dFactorMoment);
	//  iStRow += iRowCnt + 1;

}

void CRptPlateGirder_IRC22::PrintTransverseForceResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PRTF_BASE& PrtfBase, T_PRTF_DETAIL& PrtfDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("TranForce_Resistance");
	// Title
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Title_of_TransverseForceResistance"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, m_iTransverseForceChapterIx);
	iStRow += iRowCnt;

	// Design Force
	CString strLcomName = GetLoadCombinationName(PrtfBase.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Design_Force"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+1, 5, strLcomName);
	pXL->SetValue(iStRow+2, 5, PrtfBase.dN_Ed*m_dFactorForce);
	pXL->SetValue(iStRow+3, 5, PrtfBase.dMy_Ed*m_dFactorMoment);
	pXL->SetValue(iStRow+4, 5, PrtfBase.dMz_Ed*m_dFactorMoment);
	pXL->SetValue(iStRow+5, 5, PrtfBase.dF_Ed*m_dFactorForce);
	iStRow += iRowCnt + 1;

	// Eta1, 1993-1-5, (4.15)
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_Title_eta1"), iRowCnt, iColCnt);
//   iStRow += iRowCnt;
//   if(PrtfBase.dEta1>1.0) 
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_eta1_MoreThan_1"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow+0, 15, PrtfBase.dEta1);
//   }
//   else
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_eta1_LessThan_1"), iRowCnt, iColCnt);  
//     pXL->SetValue(iStRow+0, 15, PrtfBase.dEta1);
//   }
//   iStRow += iRowCnt;
//   //
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_eta1_param"), iRowCnt, iColCnt);
//   pXL->SetValue(iStRow  , 8, PrtfBase.dey_N);
//   pXL->SetValue(iStRow+1, 8, PrtfDetail.dWy_eff);
//  pXL->SetValue(iStRow  , 8, PrtfBase.dey_N);
//  pXL->SetValue(iStRow+1, 8, PrtfBase.dez_N);
//  pXL->SetValue(iStRow+2, 8, PrtfDetail.dWy_eff);
//  pXL->SetValue(iStRow+3, 8, PrtfDetail.dWz_eff);
/*  iStRow += iRowCnt + 1;*/

	// - Eta2, 1993-1-5, (6.14)
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_Title_LoadApp_type"), iRowCnt, iColCnt);
//   iStRow += iRowCnt;

/*  int iLoadAppType=PrtfDetail.iLoadAppType; */
//   if(iLoadAppType==0)
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_LoadApp_type_a"), iRowCnt, iColCnt);
//     iStRow += iRowCnt;
//     if(!PrtfDetail.bIsStiffener)
//     {
//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_title_kF_without_stiff"), iRowCnt, iColCnt);
//       iStRow += iRowCnt;
//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_kF_without_stiff"), iRowCnt, iColCnt);
//       pXL->SetValue(iStRow+0,9, PrtfBase.dk_F);  
//       iStRow += iRowCnt+1;
//     }
//     else
//     {
//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_title_kF_with_stiff"), iRowCnt, iColCnt);
//       iStRow += iRowCnt;
//       if(PrtfDetail.ik_F_calc_type==1)
//       {
//         pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_kF_with_stiff"), iRowCnt, iColCnt);
//         pXL->SetValue(iStRow+0,18, PrtfBase.dk_F);
//         pXL->SetValue(iStRow+1,18, PrtfBase.dgamma_s);
//         pXL->SetValue(iStRow+2,18, PrtfDetail.dGamma_s_cal);
//         pXL->SetValue(iStRow+3,18, PrtfDetail.dGamma_s_limit);
//         pXL->SetValue(iStRow+4, 7, PrtfBase.db1);
//         pXL->SetValue(iStRow+5, 7, PrtfDetail.dhw);
//         pXL->SetValue(iStRow+6, 7, PrtfDetail.da);
//       }
//       else if(PrtfDetail.ik_F_calc_type==2)
//       {
//         pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_kF_without_stiff"), iRowCnt, iColCnt);
//         pXL->SetValue(iStRow+0,9, PrtfBase.dk_F);
//       }
//       else ASSERT(0);
//       iStRow += iRowCnt+1;
//     }    
//   }
//   else if(iLoadAppType==1)
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_LoadApp_type_b"), iRowCnt, iColCnt);
//     iStRow += iRowCnt;
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_kF_Loadtype_b"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow+0,10, PrtfBase.dk_F);
//     pXL->SetValue(iStRow+1, 7, PrtfDetail.dhw);
//     pXL->SetValue(iStRow+2, 7, PrtfDetail.da);
//     iStRow += iRowCnt+1;
//   }
//   else if(iLoadAppType==2)
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_LoadApp_type_c"), iRowCnt, iColCnt);
//     iStRow += iRowCnt;
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_kF_Loadtype_c"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow+0,10, PrtfBase.dk_F);
//     pXL->SetValue(iStRow+1, 7, PrtfDetail.dSs);
//     pXL->SetValue(iStRow+2, 7, PrtfDetail.dC);
//     pXL->SetValue(iStRow+3, 7, PrtfDetail.dhw);
//     iStRow += iRowCnt+1;
//   }
//   else ASSERT(0);

	// Effective loaded length
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_title_Eff_loaded_length"), iRowCnt, iColCnt);
//   iStRow += iRowCnt;
//   
//   if(iLoadAppType==0 || iLoadAppType==1)
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Eff_loaded_length_loadtype_a_b"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow+0,13, PrtfBase.dly);
//     pXL->SetValue(iStRow+1, 7, PrtfBase.dm1);
//     pXL->SetValue(iStRow+2, 7, PrtfBase.dm2);    
//     iStRow += iRowCnt + 1;
//   }
//   else if(iLoadAppType==2)
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Eff_loaded_length_loadtype_c"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow+0,14, PrtfBase.dle);
//     pXL->SetValue(iStRow+1,14, PrtfBase.dly1);
//     pXL->SetValue(iStRow+2,14, PrtfBase.dly2);
//     pXL->SetValue(iStRow+3, 9, PrtfBase.dly);
//     iStRow += iRowCnt + 1;
//   }
//   else ASSERT(0);

	// Design resistance
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_title_Design_resistance"), iRowCnt, iColCnt);
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_F_Rd_Calc"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+0, 11, PrtfBase.dF_Rd*m_dFactorForce);
	//   pXL->SetValue(iStRow+1,15, PrtfBase.dFcr*m_dFactorForce);
	//   pXL->SetValue(iStRow+2,15, PrtfBase.dLamda_F);
	//   pXL->SetValue(iStRow+3,11, PrtfBase.dX_F);
	pXL->SetValue(iStRow+1, 11, PrtfBase.dL_eff);
	iStRow += iRowCnt + 1;

	// Verification
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_title_Verification"), iRowCnt, iColCnt);
	iStRow += iRowCnt;
	//    pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Verification_eta2"), iRowCnt, iColCnt);
	//    pXL->SetValue(iStRow ,8, PrtfBase.dEta2);
	//    iStRow += iRowCnt;
	if ( PrtfBase.dInterRat>1.0 )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Verification_eta2_MoreThan"), iRowCnt, iColCnt);
	}
	else
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Verification_eta2_LessThan"), iRowCnt, iColCnt);
	}
	pXL->SetValue(iStRow, 7, PrtfBase.dInterRat);
	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintLongitudinalShearResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PGLS_BASE& PglsGd, T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("LongShear_Resistance");
	strBaseSheet = _T("I_Sect");
	// Title
	iStRow+=2;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, _T("I_Sect"), _T("Title_of_LongitudinalShearResistance"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, m_iLongitudinalChapterShearIx);
	iStRow += iRowCnt;
	/*pXL->CopyRange(iStRow, 1, m_strBaseBook, _T("I_Sect"), _T("resistance_to_longi_shear"), iRowCnt, iColCnt);*/

	CString RptBlock = _T("resistance_to_longi_shear") ;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
	short Row_Ref = 0, Col_Ref = 0;
	pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;

	//Design Force
	CString strLcomName = GetLoadCombinationName(PglsBase.OrgLcomK);

	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_lcname"), strLcomName, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_nc"), PglsBase.dNc_el*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_med"), (PglsBase.dMa_Ed + PglsBase.dMy)*m_dFactorMoment, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_veddl"), max(1.0, 0.5*PglsBase.dFz*m_dFactorForce), Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_vedll"), max(1.0, 0.5*PglsBase.dFz*m_dFactorForce), Row_Ref, Col_Ref, iStRow);

	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longishear_adydbyid"), m_dAYbyILongTerm, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longishear_aiyibyii"), m_dAYbyIShortTerm, Row_Ref, Col_Ref, iStRow);

	double dvalcal = 1000*(max(1.0, 0.5*PglsBase.dFz*m_dFactorForce)*m_dAYbyILongTerm+max(1.0, 0.5*PglsBase.dFz*m_dFactorForce)*m_dAYbyIShortTerm);

	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_vled"), dvalcal, Row_Ref, Col_Ref, iStRow);
	//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param2"), iRowCnt, iColCnt);
	//   pXL->SetValue(iStRow+0, 8, PglsDetail.iNum);
	//   pXL->SetValue(iStRow+1, 8, PglsDetail.dDsc);
	//   pXL->SetValue(iStRow+2, 8, PglsDetail.dhsc);
	//   pXL->SetValue(iStRow+3, 8, PglsDetail.dSpace);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_sprov"), PglsDetail.dSpace*m_dFactorLengthMM, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_nfact"), PglsDetail.iNum, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_qu"), PglsBase.dP_Rd*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
	dvalcal = 1000*(PglsDetail.dSpace*PglsDetail.iNum/dvalcal);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_sl1req"), dvalcal*m_dFactorLengthMM, Row_Ref, Col_Ref, iStRow);
	double dsl1 =  dvalcal*m_dFactorLengthMM;

	//full shear connection
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_asl"), (m_dArt+m_dArb)*m_dFactorRebarArea, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_h1"), (m_dArt+m_dArb)*m_dFyr*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
	double dvalh1 = (m_dArt+m_dArb)*m_dFyr*m_dFactorForce;
	if ( m_nCase_PNA>1 )  PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_aec"), m_dBc*m_dY_Bar_PNA*m_dFactorRebarArea, Row_Ref, Col_Ref, iStRow);
	else PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_aecsteel"), m_dBc*m_dtc*m_dFactorRebarArea, Row_Ref, Col_Ref, iStRow);

	if ( m_nCase_PNA>1 ) dvalcal=0.36*m_dfck* m_dBc*m_dY_Bar_PNA*m_dFactorRebarArea;
	else dvalcal=0.36*m_dfck*m_dBc*m_dtc*m_dFactorRebarArea;
	double dvalh2 = dvalcal*m_dFactorForce;
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_h2"), dvalcal*m_dFactorForce, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_h"), min(dvalh1, dvalh2), Row_Ref, Col_Ref, iStRow);

	dvalcal = PglsBase.dP_Rd*PglsDetail.iNum*m_dLentot/min(dvalh1, dvalh2);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_sl2req"), dvalcal*m_dFactorLengthMM/1000, Row_Ref, Col_Ref, iStRow);
	double dsl2 =  dvalcal*m_dFactorLengthMM/1000;
	//VR
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_vr"), PglsBase.dv_L_Ed, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_vrcal"), PglsBase.dFz*m_dAYbyIShortTerm, Row_Ref, Col_Ref, iStRow);
	double dvr = PglsBase.dFz*m_dAYbyIShortTerm;
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_qr"), 15, Row_Ref, Col_Ref, iStRow);
	double dqr = 15.0;
	dvalcal = dqr* PglsDetail.iNum*1000/dvr;
	double dsl3 =  dvalcal*m_dFactorLengthMM;
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_srreq"), dsl3, Row_Ref, Col_Ref, iStRow);

	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_sreqverification"), min(dsl1, min(dsl2, dsl3)), Row_Ref, Col_Ref, iStRow);

	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("longshear_sprovidedverification"), PglsDetail.dSpace*m_dFactorLengthMM, Row_Ref, Col_Ref, iStRow);


	//PglsBase.iMax
	//longitudinal shear

	// 
	//   
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Design_Force"), iRowCnt, iColCnt);
//   pXL->SetValue(iStRow+1,10, strLcomName);
//   pXL->SetValue(iStRow+2, 5, PglsBase.dNc_el*m_dFactorForce);
//   pXL->SetValue(iStRow+3, 5, PglsBase.dNc_f*m_dFactorForce);
//   pXL->SetValue(iStRow+4, 5, (PglsBase.dMa_Ed + PglsBase.dMy)*m_dFactorMoment);  // CHECK by GAY. PMS:4216. ('12.05.17). °è»êÀº My_Gd+My_Base ·Î : dM_Ed  = dMy_a + dMy;
//   pXL->SetValue(iStRow+5, 5, PglsBase.dFz*m_dFactorForce);  
//   pXL->SetValue(iStRow+6, 5, PglsBase.dMpl_Rd*m_dFactorMoment);  
//   pXL->SetValue(iStRow+7, 5, PglsBase.dMel_Rd*m_dFactorMoment);  
//   iStRow += iRowCnt + 1;
// 	
//   // Shear resistance of a single connector
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_shear_sigle_connector"), iRowCnt, iColCnt);
//   iStRow += iRowCnt;
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc"), iRowCnt, iColCnt);
//   pXL->SetValue(iStRow+0,13, PglsBase.dP_Rd*m_dFactorForce);
// //   pXL->SetValue(iStRow+1,15, PglsBase.dP_Rd2);
// //   pXL->SetValue(iStRow+2,11, PglsBase.dP_Rd);
//   iStRow += iRowCnt + 1;
// 
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param1"), iRowCnt, iColCnt);
//   pXL->SetValue(iStRow+0, 7, PglsDetail.dfu);
//   iStRow += iRowCnt;
//   if(PglsDetail.iRat_sc_type==1)
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param_alpha_type1"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow+0, 13, PglsBase.dAlpha);
//   }
//   else if(PglsDetail.iRat_sc_type==2)
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param_alpha_type2"), iRowCnt, iColCnt);
//   }
//   else if(PglsDetail.iRat_sc_type==3)
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param_alpha_type3"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow+0, 13, PglsBase.dAlpha);
//   }
//   else ASSERT(0);
//   iStRow += iRowCnt;
//   //
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param2"), iRowCnt, iColCnt);
//   pXL->SetValue(iStRow+0, 8, PglsDetail.iNum);
//   pXL->SetValue(iStRow+1, 8, PglsDetail.dDsc);
//   pXL->SetValue(iStRow+2, 8, PglsDetail.dhsc);
//   pXL->SetValue(iStRow+3, 8, PglsDetail.dSpace);
//   iStRow += iRowCnt + 1;
// 
//   //
//   // Longitudinal shear force acting on length LA_B
//   double dM_Ed = PglsGd.dMy + PglsBase.dMy;
//   if((PglsBase.iSectClass==1 || PglsBase.iSectClass==2) && dM_Ed>=0.0 && dM_Ed-PglsBase.dMel_Rd>0.0)
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_V_L_Ed_calc"), iRowCnt, iColCnt);
//     iStRow += iRowCnt;
//   
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_V_L_Ed_1"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow+0,14, PglsBase.dV_L_Ed); 
//     iStRow += iRowCnt+1;
//     //
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_Verification_interface"), iRowCnt, iColCnt);
//     iStRow += iRowCnt;
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Ed_interface"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow+0, 9, PglsBase.dv_L_Ed); 
//     pXL->SetValue(iStRow+1, 7, PglsDetail.dLv); 
//     iStRow += iRowCnt;
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Rd"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow+0,12, PglsBase.dv_L_Rd);  
//     iStRow += iRowCnt;
//     if(fabs(PglsBase.dv_L_Ed)>fabs(PglsBase.dv_L_Rd))
//     {
//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_interface_v_L_Ed_MoreThan_v_L_Rd"), iRowCnt, iColCnt);    
//     }
//     else
//     {
//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_interface_v_L_Ed_LessThan_v_L_Rd"), iRowCnt, iColCnt);    
//     }
//     iStRow += iRowCnt + 1;
//   }
//   else
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_Verification_slab"), iRowCnt, iColCnt);
//     iStRow += iRowCnt;
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Ed_slab"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow+0,12, PglsBase.dv_L_Ed);  
//     iStRow += iRowCnt;
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Rd"), iRowCnt, iColCnt);
//     pXL->SetValue(iStRow+0,12, PglsBase.dv_L_Rd);  
//     iStRow += iRowCnt;
//     if(fabs(PglsBase.dv_L_Ed)>fabs(PglsBase.dv_L_Rd))
//     {
//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_interface_v_L_Ed_MoreThan_v_L_Rd"), iRowCnt, iColCnt);    
//     }
//     else
//     {
//       pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_interface_v_L_Ed_LessThan_v_L_Rd"), iRowCnt, iColCnt);    
//     }
//     iStRow += iRowCnt + 1;
//   }


	// Modify, Jaoeh. [6/1/2011] ÀÌ´ë±Ù ºÎÀå´Ô ºÒÇÊ¿äÇÏ´Ù°í ÇØ¼­ °í·Á¾ÈÇÔ. 
//  pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_conc"), iRowCnt, iColCnt);
//  iStRow += iRowCnt;
//  pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_Ed_calc"), iRowCnt, iColCnt);
//  pXL->SetValue(iStRow+0,11, PglsBase.dv_Ed);
//  iStRow += iRowCnt;
//
//  if(PglsBase.dv_Ed>PglsBase.dv_Ed_lim)
//  {
//    pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_Ed_MoreThan_v_Ed_Limit"), iRowCnt, iColCnt);
//    pXL->SetValue(iStRow+0,13, PglsBase.dv_Ed_lim);
//  }
//  else
//  {
//    pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_Ed_LessTean_v_Ed_Limit"), iRowCnt, iColCnt);
//    pXL->SetValue(iStRow+0,13, PglsBase.dv_Ed_lim);
//  }
//  iStRow += iRowCnt + 1;
	iStRow += 50;
}

void CRptPlateGirder_IRC22::PrintFatigueResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PGRF_BASE& PgrfBase, T_PGRF_DETAIL& PgrfDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("Fatigue_Resistance");
	strBaseSheet = _T("I_Sect");
	// Title
	pXL->CopyRange(iStRow, 2, m_strBaseBook, strBaseSheet, _T("Resistance_2_Fatigue"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, m_iFatigueChapterIx);
	iStRow += iRowCnt;

	CString RptBlock = _T("FATIGUE_RESIST") ;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, RptBlock, iRowCnt, iColCnt);
	short Row_Ref = 0, Col_Ref = 0;
	pXL->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref); Col_Ref-=1;

	//Design Force
	CString strLcomName = GetLoadCombinationName(PgrfBase.OrgLcomK);

	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fatigue_1"), PgrfBase.dDel_Tau_E_2, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fatigue_2"), PgrfBase.dLamda_v1, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fatigue_3"), PgrfBase.dLamda_v2, Row_Ref, Col_Ref, iStRow);

	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fatigue_4"), PgrfBase.dGamma_Ff, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fatigue_5"), PgrfBase.dGamma_Mfs, Row_Ref, Col_Ref, iStRow);

	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fatigue_6"), PgrfBase.dLamda_v3, Row_Ref, Col_Ref, iStRow);
	PrintCellData(pXL, m_strBaseBook, m_strBaseSheet, _T("fatigue_7"), PgrfBase.dLamda_v4, Row_Ref, Col_Ref, iStRow);

	// Design Force

//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_Design_Force"), iRowCnt, iColCnt);
//   pXL->SetValue(iStRow+1, 10, strLcomName);
//   pXL->SetValue(iStRow+2,  5, PgrfBase.dFz*m_dFactorForce);
//   iStRow += iRowCnt + 1;
//   // Shear stress range for the connector
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_shear_stress_range_stud"), iRowCnt, iColCnt);
//   pXL->SetValue(iStRow+1, 8, PgrfBase.dDel_Tau);
//   pXL->SetValue(iStRow+2,20, PgrfBase.dFsc);
//   pXL->SetValue(iStRow+3, 8, PgrfBase.dAsc);
//   iStRow += iRowCnt + 1;
//   // Shear stress range for the connector
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_Damage_equivalent_factor_lamda_v"), iRowCnt, iColCnt);
//  // pXL->SetValue(iStRow+1,11, PgrfBase.dLamda_v);
//   pXL->SetValue(iStRow+2, 8, PgrfBase.dLamda_v1);
//   pXL->SetValue(iStRow+3, 8, PgrfBase.dLamda_v2);
// 	pXL->SetValue(iStRow+4, 8, PgrfBase.dGamma_Ff);
// 	pXL->SetValue(iStRow+5, 8, PgrfBase.dGamma_Mfs);
//   iStRow += iRowCnt + 1;
//   // Equivalent constant amplitude range of shear stress related to 2 million cycles
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_delta_tau_E2"), iRowCnt, iColCnt);
//   pXL->SetValue(iStRow+1, 9, PgrfBase.dDel_Tau_E_2);
// 	pXL->SetValue(iStRow+3, 6, PgrfBase.dLamda_v3);
// 	pXL->SetValue(iStRow+4, 6, PgrfBase.dLamda_v4);
//   iStRow += iRowCnt + 1;
//   // Verification
//   pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_Sub_title_Verification"), iRowCnt, iColCnt);
//   iStRow += iRowCnt;
//   if(PgrfBase.bOK)
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_Verification_LessThan_1"), iRowCnt, iColCnt);    
// 		   
//   }
//   else
//   {
//     pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_Verification_MoreThan_1"), iRowCnt, iColCnt);    
// 		   
//   }
// 	CString str;
// 	str.Format(_T("'%g"),PgrfBase.dLamda_v3);
//   pXL->SetValue(iStRow,10, str);
// 	str.Format(_T("'%g"),PgrfBase.dLamda_v4);
// 	pXL->SetValue(iStRow,27, str);
	iStRow += 15;

}

void CRptPlateGirder_IRC22::PrintStressLimit(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PGBS_BASE& PgbsBase, T_PGBS_DETAIL& PgbsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("Stress_Limit");

	// Title
	PrintTitle_Sub_Title_Stress_Limitation_Chk(pXL, iStRow, strTitleBaseSheet);

	// - In the structural steel
	CString strLcomName = GetLoadCombinationName(PgbsBase.ChkGirder.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_structural_steel"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+1, 15, strLcomName);
	pXL->SetValue(iStRow+2, 6, PgbsBase.ChkGirder.dMaxStress);
	pXL->SetValue(iStRow+3, 6, fabs(PgbsBase.ChkGirder.dtau_w));
	CString strMaxStressPosi, strPosi;
	strPosi = GetStressPosition(PgbsBase.ChkGirder.iIndex_cr);
	strMaxStressPosi.Format(_T("(%s)"), strPosi);
	pXL->SetValue(iStRow+2, 13, strMaxStressPosi);
	iStRow += iRowCnt + 1;


	// Normal stress
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Title_Sigma_Ed_ser"), iRowCnt, iColCnt);
	iStRow += iRowCnt;
	if ( PgbsBase.ChkGirder.bChk_Stress ) { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Check_OK"), iRowCnt, iColCnt); }
	else { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Check_NG"), iRowCnt, iColCnt); }
	pXL->SetValue(iStRow, 3, PgbsBase.ChkGirder.dMaxStress);
	pXL->SetValue(iStRow, 12, PgbsBase.ChkGirder.dAlwStress);
	iStRow += iRowCnt + 1;


	// Shear stress
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Title_Tau_Ed_ser"), iRowCnt, iColCnt);
	iStRow += iRowCnt;
	if ( PgbsBase.ChkGirder.bChk_tau ) { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Check_OK"), iRowCnt, iColCnt); }
	else { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Check_NG"), iRowCnt, iColCnt); }
	pXL->SetValue(iStRow, 3, fabs(PgbsBase.ChkGirder.dtau_w));
	pXL->SetValue(iStRow, 12, PgbsBase.ChkGirder.dAlw_tau);
	iStRow += iRowCnt + 1;


	// Shear stress
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Title_Sigma_Tau_ser_comb"), iRowCnt, iColCnt);
	iStRow += iRowCnt;
	if ( PgbsBase.ChkGirder.bChk_Stress_cb ) { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Check_OK"), iRowCnt, iColCnt); }
	else { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Check_NG"), iRowCnt, iColCnt); }
	pXL->SetValue(iStRow, 3, PgbsBase.ChkGirder.dStress_cb);
	pXL->SetValue(iStRow, 12, PgbsBase.ChkGirder.dAlw_stress_cb);
	iStRow += iRowCnt + 1;


	// - In the concrete of the slab
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Title"), iRowCnt, iColCnt);
	iStRow += iRowCnt;
	strLcomName = GetLoadCombinationName(PgbsBase.ChkSlab.OrgLcomK);
	if ( PgbsBase.ChkSlab.LcomType==1 )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Title_Quasi"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 15, strLcomName);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Sigma_c_Quasi"), iRowCnt, iColCnt);
		iStRow += iRowCnt;
	}
	else if ( PgbsBase.ChkSlab.LcomType==3 )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Title_Characteristic"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 15, strLcomName);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Sigma_c_Characteristic"), iRowCnt, iColCnt);
		iStRow += iRowCnt;
	}
	else ASSERT(0);
	if ( PgbsBase.ChkSlab.bChk_Stress_sb ) { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Check_OK"), iRowCnt, iColCnt); }
	else { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Check_NG"), iRowCnt, iColCnt); }
	pXL->SetValue(iStRow, 3, PgbsBase.ChkSlab.dStress_sb);
	pXL->SetValue(iStRow, 12, PgbsBase.ChkSlab.dAlwStress_sb);
	iStRow += iRowCnt + 1;



	// - In the reinforcement
	strLcomName = GetLoadCombinationName(PgbsBase.ChkBar.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Rebar"), iRowCnt, iColCnt);
	iStRow += iRowCnt;
	pXL->SetValue(iStRow-2, 11, strLcomName);
	if ( PgbsBase.ChkBar.dStress_bar < 0.0 )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_NoCheck_Rebar"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 3, PgbsBase.ChkBar.dStress_bar);
	}
	else // ÀÎÀå. 
	{
		if ( PgbsBase.ChkBar.bChk_Stress_bar ) { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Check_OK"), iRowCnt, iColCnt); }
		else { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Check_NG"), iRowCnt, iColCnt); }
		pXL->SetValue(iStRow, 3, PgbsBase.ChkBar.dStress_bar);
		pXL->SetValue(iStRow, 12, PgbsBase.ChkBar.dAlwStress_bar);
	}
	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_IRC22::PrintLongitudinalShear_SLS(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PLSS_BASE& PlssGd, T_PLSS_BASE& PlssBase, T_PLSS_DETAIL& PlssDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("LongShear_Resistance");
	// Title
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Title_of_LongitudinalShearSLS"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, m_iLongShearSLSChapterIx);
	iStRow += iRowCnt;

	// Shear resistance of a single connector
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_shear_sigle_connector"), iRowCnt, iColCnt);
	iStRow += iRowCnt;
	CString strLcomName = GetLoadCombinationName(PlssBase.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_Comb_Name_SLS"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 10, strLcomName);
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc"), iRowCnt, iColCnt);
	//   pXL->SetValue(iStRow+0,13, PlssBase.dP_Rd1*m_dFactorForce);
	//   pXL->SetValue(iStRow+1,15, PlssBase.dP_Rd2*m_dFactorForce);
	pXL->SetValue(iStRow+0, 5, PlssBase.dP_Rd*m_dFactorForce);
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_SLS"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+0, 11, PlssBase.dP_Rd_SLS*m_dFactorForce);
	iStRow += iRowCnt + 1;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param1"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+0, 7, PlssDetail.dfu);
	iStRow += iRowCnt;
	if ( PlssDetail.iRat_sc_type==1 )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param_alpha_type1"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow+0, 13, PlssBase.dAlpha);
	}
	else if ( PlssDetail.iRat_sc_type==2 )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param_alpha_type2"), iRowCnt, iColCnt);
	}
	else if ( PlssDetail.iRat_sc_type==3 )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param_alpha_type3"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow+0, 13, PlssBase.dAlpha);
	}
	else ASSERT(0);
	iStRow += iRowCnt;
	//
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param2"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+0, 8, PlssDetail.iNum);
	pXL->SetValue(iStRow+1, 8, PlssDetail.dDsc);
	pXL->SetValue(iStRow+2, 8, PlssDetail.dhsc);
	pXL->SetValue(iStRow+3, 8, PlssDetail.dSpace);
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_param_ks_SLS"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow+0, 8, m_CpgdD.dks);
	iStRow += iRowCnt + 1;

	//
	double dM_Ed = PlssGd.aForce[4] + PlssBase.aForce[4];
	if ( (PlssBase.iSectClass==1 || PlssBase.iSectClass==2) && dM_Ed>=0.0 && dM_Ed-PlssBase.dMel_Rd>0.0 )
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_V_L_Ed_calc"), iRowCnt, iColCnt);
		iStRow += iRowCnt;

		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_V_L_Ed_1"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow+0, 14, PlssBase.dV_L_Ed*m_dFactorForce);
		iStRow += iRowCnt+1;
		//
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_Verification_interface"), iRowCnt, iColCnt);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Ed_interface"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow+0, 9, PlssBase.dv_L_Ed);
		pXL->SetValue(iStRow+1, 7, PlssDetail.dLv);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Rd_SLS"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow+0, 12, PlssBase.dv_L_Rd);
		iStRow += iRowCnt;
		if ( fabs(PlssBase.dv_L_Ed)>fabs(PlssBase.dv_L_Rd) )
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_interface_v_L_Ed_MoreThan_v_L_Rd"), iRowCnt, iColCnt);
		}
		else
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_interface_v_L_Ed_LessThan_v_L_Rd"), iRowCnt, iColCnt);
		}
		iStRow += iRowCnt + 1;
	}
	else
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_Verification_slab"), iRowCnt, iColCnt);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Ed_slab"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow+0, 12, PlssBase.dv_L_Ed);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Rd_SLS"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow+0, 12, PlssBase.dv_L_Rd);
		iStRow += iRowCnt;
		if ( fabs(PlssBase.dv_L_Ed)>fabs(PlssBase.dv_L_Rd) )
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_interface_v_L_Ed_MoreThan_v_L_Rd"), iRowCnt, iColCnt);
		}
		else
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_interface_v_L_Ed_LessThan_v_L_Rd"), iRowCnt, iColCnt);
		}
		iStRow += iRowCnt + 1;
	}
}

void CRptPlateGirder_IRC22::Print_CheckElemPosition(CMSExcel* pXL, int& iStRow, CString strBaseSheet, int iElemNo, int nPosi)
{
	int iRowCnt = 0;
	int iColCnt = 8;
	CString strPosi = nPosi==1 ? _T("I") : _T("J");
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Check_ElementNo"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 8, iElemNo);
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Check_Position"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 8, strPosi);
	iStRow += iRowCnt + 1;
}


BOOL CRptPlateGirder_IRC22::Get_DesignInfo_Section(ElemPairK ElemK, BOOL bPosiI)
{
	int nPosiId = bPosiI ? 0 : 1;
	CPG_SECT_ELEM SectElem;  SectElem.Initialize();
	m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_CpgSectElem(ElemK, SectElem);

	CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
	m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_CpgRbarElem(ElemK, RbarElem);
	//
	CPG_SCON_ELEM SconElem; SconElem.Initialize();
	m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_CpgSconElem(ElemK, SconElem);
	//

	CPG_TRST_ELEM TrstElem; TrstElem.Initialize();
	m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_CpgTrstElem(ElemK, TrstElem);

	BOOL bMembTemp=FALSE;
	CPG_MEMB_ELEM MembElemTemp;  MembElemTemp.Initialize();
	if ( !m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_CpgMembElem(ElemK, MembElemTemp) ) { bMembTemp=FALSE; }

	CPG_MEMB_POSI MembPosiTemp;
	MembPosiTemp = bPosiI ? MembElemTemp.MembPosi[0] : MembElemTemp.MembPosi[1];
	// m_dLentot		= MembPosiTemp.dLu; // span length between the rigid supports.

	   //For calculatinf Beff

	int nPosiNega = nPosiId+1; // Positive 
	T_PGBR_LCOM PgbrRatD; PgbrRatD.Initialize(); // PositiveÀÇ I,J¸¦ ¾ò¾î¿Â´Ù. 
	if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPgbrResultData(ElemK, nPosiNega, PgbrRatD) )
	{
		T_PGBR_BASE PgbrGd   = PgbrRatD.BeforePgbr[nPosiId];
		T_PGBR_BASE PgbrBase = PgbrRatD.PgbrBase[nPosiId];
		T_PGBR_DETAIL PgbrDetailGd, PgbrDetail;
		if ( PgbrBase.bChk && m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_BendingResistanceDetailResult4PlateGirder(ElemK, bPosiI, PgbrGd, PgbrBase, PgbrDetail) )
		{
			m_dBeffFactor = PgbrBase.dBeffFactor;
			m_dLentot     = PgbrBase.dBeta;
		}
	}


	//End Beff



	T_SECT_D SectD = SectElem.SectD;



	int i=0;
	int nSectType = 0;
	if ( SectD.nStype == D_SECT_TYPE_TAPERED ) nSectType = SectD.SectBefore.nStype;
	else nSectType = SectD.nStype;
	m_nSectType = nSectType;

	BOOL bTapered = SectD.nStype == D_SECT_TYPE_TAPERED ? TRUE : FALSE;

	T_SECT_STIFFNESS GirderStiffD; GirderStiffD.Initialize();
	T_SECT_STIFFNESS CompoStiffD;  CompoStiffD.Initialize();

	T_SECT_SECTBASE_D SectBaseD; SectBaseD.Initialize();
	if ( nSectType != D_SECT_TYPE_COMPO_G )
	{
		if ( bPosiI ) SectBaseD = SectD.SectBefore.SectI;
		else       SectBaseD = !bTapered ? SectD.SectBefore.SectI : SectD.CmpTapJ;
		m_dBc  = SectD.SectAfter.SectJ.Size[0];
		m_dtc  = SectD.SectAfter.SectJ.Size[1];
		m_dHh  = SectD.SectAfter.SectJ.Size[2];

		for ( i=0; i<10; i++ ) { m_dSize[i] = SectBaseD.Size[i]; }
		// girder
		m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiffD);

		// Composite section  
		if ( !bTapered ) CompoStiffD = SectD.SectAfter.Stiffness;
		else          CompoStiffD = bPosiI ? SectD.SectAfter.SectI.Stiffness : SectD.SectAfter.SectJ.Stiffness;
	}
	else
	{
		SectBaseD = bTapered && !bPosiI ? SectD.SectBefore.SectJ : SectD.SectBefore.SectI;
		int iGenPartSize = SectBaseD.aGeneralPart.GetSize();
		for ( i=0; i<iGenPartSize; i++ )
		{
			T_GSEC_PART PartD = SectBaseD.aGeneralPart[i];
			if ( i==0 ) GirderStiffD = PartD.Stiffness;
			else     CompoStiffD  = PartD.StiffnessSum;
		}
	}

	m_dArea_g = GirderStiffD.Area;
	m_dIyy_g  = GirderStiffD.Ryy;
	m_dIzz_g  = GirderStiffD.Rzz;
	m_dYbar_g = GirderStiffD.Cym;
	m_dZbar_g = GirderStiffD.Czm;
	m_dIw			= GirderStiffD.dIw;
	m_dIt			= GirderStiffD.dIp;

	m_dArea = CompoStiffD.Area;
	m_dIyy  = CompoStiffD.Ryy;
	m_dIzz  = CompoStiffD.Rzz;
	m_dYbar = CompoStiffD.Cym;
	m_dZbar = CompoStiffD.Czm;

	T_SECT_STIFFNESS CrackStiff = SectElem.SectPosi[nPosiId].NonConSect;
	m_dArea_cr = CrackStiff.Area;
	m_dIyy_cr  = CrackStiff.Ryy;
	m_dIzz_cr  = CrackStiff.Rzz;
	m_dYbar_cr = CrackStiff.Cym;
	m_dZbar_cr = CrackStiff.Czm;

	// Material
	m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_MaterialData(ElemK, m_dfy, m_dfy1, m_dEs, m_dfck, m_dEcm, m_dfsk, m_dErs);

	/////////////////////From AASHTO
	// Steel
//   m_pMembInD = pMemb;
//   CSG_SECT_INFO &SectInfo = m_pMembInD->Memb.PosD[m_nChkPos].Sect.SectInfo;
//   m_nSectType  = SectInfo.nStype;
//   int  nPosIndex = pMemb->Memb.ChIf.nChkPos==0 ? 0 : 1;
//   CSG_DATA_POSD* PosSectD = &m_pMembInD->Memb.PosD[nPosIndex];
	// m_dBc  = SectD.SectAfter.SectJ.Size[0];
	double dD    = 0.0;
	double dtw   = 0.0;
	double dB1   = 0.0;
	double dtf1  = 0.0;
	double dB2   = 0.0;
	double dtf2  = 0.0;
	double dBf1  = 0.0;
	double dBf2  = 0.0;
	double dBf3  = 0.0;

	if ( m_nSectType==DGN_SECT_TYPE_COMPO_I )
	{
		m_dHw = SectD.SectBefore.SectI.Size[0];
		m_dtw = dtw  = SectD.SectBefore.SectI.Size[1];
		m_dB1 = dB1  = SectD.SectBefore.SectI.Size[2];
		m_dtf1= dtf1 = SectD.SectBefore.SectI.Size[3];
		m_dB2 = dB2  = SectD.SectBefore.SectI.Size[4]; // 0.0ÀÌ¸é dB1
		m_dtf2= dtf2 = SectD.SectBefore.SectI.Size[5]; // 0.0ÀÌ¸é dtf1
	}
	else
	{ //TUB BOX
		m_dHw =        SectD.SectBefore.SectI.Size[0];
		m_dtw = dtw  = SectD.SectBefore.SectI.Size[1];
		m_dB1 = dB1  = SectD.SectBefore.SectI.Size[2];
		m_dBf1= dBf1 = SectD.SectBefore.SectI.Size[3];
		m_dtf1= dtf1 = SectD.SectBefore.SectI.Size[4];
		m_dB2 = dB2  = SectD.SectBefore.SectI.Size[5];
		m_dBf2= dBf2 = SectD.SectBefore.SectI.Size[6];
		m_dtf2= dtf2 = SectD.SectBefore.SectI.Size[7];
		m_dBf3= dBf3 = SectD.SectBefore.SectI.Size[8];
	}

	if ( m_nSectType==DGN_SECT_TYPE_COMPO_I )   m_iShapeType = DGN_SECT_SHAPE_INDEX_REG_H;
	else if ( m_nSectType==DGN_SECT_TYPE_COMPO_B )   m_iShapeType = DGN_SECT_SHAPE_INDEX_REG_B;
	else if ( m_nSectType==DGN_SECT_TYPE_COMPO_TUB ) m_iShapeType = DGN_SECT_SHAPE_INDEX_REG_TUB;
	else ASSERT(0);

	// dD = Get_D(m_iShapeType);

	if ( dB2  < CSG_Zero ) dB2  = dB1;
	if ( dtf2 < CSG_Zero ) dtf2 = dtf1;
	double dH    = m_dHw + dtf1 + dtf2;


	// section data - slab deck
	m_dBc  = SectD.SectAfter.SectJ.Size[0];
	m_dtc  = SectD.SectAfter.SectJ.Size[1];
	m_dHh  = SectD.SectAfter.SectJ.Size[2];

	m_dBc  =  SectD.SectAfter.SectJ.Size[0];//PosSectD->Sect.SectInfo.Slab.Size[0];//114.0; 
	m_dtc  =  SectD.SectAfter.SectJ.Size[1];//PosSectD->Sect.SectInfo.Slab.Size[1];//9.0;   
	m_dHh  =  SectD.SectAfter.SectJ.Size[2]+dtf1;  //PosSectD->Sect.SectInfo.Slab.Size[2]+dtf1;//3.5;   // concrete deck Áö¾ÐÆÇ(?) + top flange thickness Æ÷ÇÔ.
	double  m_dHt  = dH + SectD.SectAfter.SectJ.Size[2] + m_dtc;
	// Rebar

	CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
	arRbarPosi.Copy(RbarElem.RbarPosi[nPosiId].arRbarPosi);

	int nRebar = arRbarPosi.GetSize();
	double dAreaSum=0.0, dAreaLenSum=0.0;
	for ( int i=0; i<nRebar; ++i )
	{
		_DGN_RBAR_CRC RebarD = arRbarPosi[i];
		dAreaSum += RebarD.dArea;
		dAreaLenSum += (RebarD.dArea * RebarD.dz);
	}
	double dRebarNA = fabs(dAreaSum)>0.0 ? dAreaLenSum / dAreaSum : 0.0;

	double dArt=0.0, dArb=0.0;
	double dArtLen=0.0, dArbLen=0.0;
	for ( i=0; i<nRebar; ++i )
	{
		_DGN_RBAR_CRC RebarD = arRbarPosi[i];
		if ( RebarD.dz > dRebarNA/*m_dtc/2.0*/ ) // top rebar
		{
			dArt += RebarD.dArea;
			dArtLen += (RebarD.dArea * (m_dtc / 2.0 - RebarD.dz));
		}
		else // bottom rebar
		{
			dArb += RebarD.dArea;
			dArbLen += (RebarD.dArea * (m_dtc / 2.0 - RebarD.dz));
		}

	}
	m_dArb = dArb;
	m_dArt = dArt;
	// Rebar Info.
//    for(int i=0; i < arRbarPosi.GetSize(); i++)
//    {
//      _DGN_RBAR_CRC Rbar = arRbarPosi[i];
//      double dAsi = Rbar.dArea;
//      dRebarF += dAsi*dfsd/1.15;
//    }
//   double dtopDepR = fabs(dArt) < CSG_Zero ? dRebarNA : dArtLen/dArt;
//   double dbotDepR = fabs(dArb) < CSG_Zero ? dRebarNA : dArbLen/dArb;
// 
//   if(fabs(dArt) < CSG_Zero || fabs(dArb) < CSG_Zero) // ÇÑ ÁÙÀÎ °æ¿ì, µ¿ÀÏÇÏ°Ô Àû¿ë
//   {
//     double dcrt  = dArt == 0.0 ? 0.0 : dArtLen/dArt;
//     double dcrb  = dArb == 0.0 ? 0.0 : dArbLen/dArb;
//     double dcr   = max(dcrt , dcrb);
//     m_dcrt =  dcr; 
//     m_dcrb =  dcr;
// 
//   }
//   else // µÎ ÁÙ ÀÌ»óÀÎ °æ¿ì
//   {
//     m_dcrt = dArt>0.0 ? dArtLen/dArt : 0.0; // concrete deck top reinforcement.
//     m_dcrb = dArb>0.0 ? dArbLen/dArb : 0.0; // concrete deck bottom reinforcement.
//   }

	//CDgnMatlDB MatlDB;
	// material data - slab deck
	m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_MaterialData(ElemK, m_dfy, m_dfy1, m_dEs, m_dfck, m_dEcm, m_dfsk, m_dErs);
	m_dfc  = m_dfck;//4.0;
//   m_dEc  = m_dEcm;//3644.0;
//   m_dArt = dArt;//100.0;
//   m_dArb = dArb;//100.0;
	m_dFyr = m_dfy1;//60.0;   

	CPG_MATL_ELEM MatlElem;
	if ( !m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_CpgMatlElem(ElemK, MatlElem) ) { return FALSE; }

	// Material
	const T_MATD_D& MatdD = MatlElem.MatdD;
	m_strMatNameSteel = MatdD.Data1.CodeMatlName;
	m_dFu = MatdD.Data1.Design.S_Fu;

	m_dmodularratiopermt = max(15.0, m_dEs/(0.5*m_dEcm));
	m_dmodularratiotransient = max(7.5, m_dEs/m_dEcm);
	// Section Properties

//   m_dI = GirderStiff.dIyy;
//   m_dI    = GirderStiff.dIyy;
//   m_dI_3n = PosSectD->Sect.RbarStiff3n.dIyy;
//   m_dI_r  = PosSectD->Sect.NonSlabStiff.dIyy;
//   m_dI_n  = PosSectD->Sect.RbarStiffn.dIyy;
//   m_dI_r3 = PosSectD->Sect.NonSlabStiff2.dIyy;
// 
//   m_dStop    = m_dI    / GirderStiff.dCzp;
//   m_dStop_3n = m_dI_3n / PosSectD->Sect.RbarStiff3n.dCzp;
//   m_dStop_r  = m_dI_r  / PosSectD->Sect.NonSlabStiff.dCzp;
//   m_dStop_n  = m_dI_n  / PosSectD->Sect.RbarStiffn.dCzp;
//   m_dStop_r3 = m_dI_r3 / PosSectD->Sect.NonSlabStiff2.dCzp;
// 
//   m_dSbot    = m_dI    / GirderStiff.dCzm;
//   m_dSbot_3n = m_dI_3n / PosSectD->Sect.RbarStiff3n.dCzm;
//   m_dSbot_r  = m_dI_r  / PosSectD->Sect.NonSlabStiff.dCzm;
//   m_dSbot_n  = m_dI_n  / PosSectD->Sect.RbarStiffn.dCzm;
//   m_dSbot_r3 = m_dI_r3 / PosSectD->Sect.NonSlabStiff2.dCzm;
// 
		 //m_dCzm_g   = GirderStiff.dCzm;
		 //m_dCzp_g   = GirderStiff.dCzp;
		 //m_dIy_g    = GirderStiff.dIyy;
		 //m_dJ_g     = GirderStiff.dIxx;
// 
//   m_dCzm_cst = PosSectD->Sect.RbarStiffn.dCzm;
//   m_dCzp_cst = PosSectD->Sect.RbarStiffn.dCzp;
//   m_dIy_cst  = PosSectD->Sect.RbarStiffn.dIyy;
//   m_dJ_c     = PosSectD->Sect.RbarStiffn.dIxx;
// 
//   m_dCzm_clt = PosSectD->Sect.RbarStiff3n.dCzm;
//   m_dCzp_clt = PosSectD->Sect.RbarStiff3n.dCzp;
//   m_dIy_clt  = PosSectD->Sect.RbarStiff3n.dIyy;
// 
//   m_dCzm_cn = PosSectD->Sect.NonSlabStiff.dCzm;
//   m_dCzp_cn = PosSectD->Sect.NonSlabStiff.dCzp;
//   m_dIy_cn  = PosSectD->Sect.NonSlabStiff.dIyy;
// 
//   m_dCzm_c3n = PosSectD->Sect.NonSlabStiff2.dCzm;
//   m_dCzp_c3n = PosSectD->Sect.NonSlabStiff2.dCzp;
//   m_dIy_c3n  = PosSectD->Sect.NonSlabStiff2.dIyy;
// 
// 
//   m_nLcomType = pMemb->aChkFor[0].nLcomType;
// 
//   // Get Forces
//   if (pMemb->aChkFor[0].nLcomType == ENUM_CONSTRUCT)
//   {
//     m_dMd1 = pPosForcD->ForNC.dForce[4];
//     m_dMd2 = pPosForcD->ForLT.dForce[4];
//     m_dMd3 = pPosForcD->ForST.dForce[4];
//     m_dMuy = m_dMd1;
// 
//     m_dMuz = pPosForcD->ForNC.dForce[5];
//     m_dVu  = pPosForcD->ForNC.dForce[2];
//     m_dVu_sm  = pPosForcD->ForNC.dForce[2];
//     m_dT   = pPosForcD->ForNC.dForce[3];
//     m_dT1  = pPosForcD->ForNC.dForce[3];
//     m_dT2  = pPosForcD->ForLT.dForce[3];
//     m_dT3  = pPosForcD->ForST.dForce[3];
//   }
//   else if (pMemb->aChkFor[0].nLcomType == 0) // Strength
//   {
// 
//     // StrengthÀÇ ÈÚ°ËÅä½Ã Unbraced length Áß °¡Àå Å« ¸ð¸àÆ®¸¦ Àû¿ëÇÑ´Ù.
//     BOOL bPositiveM = pPosForcD->ForLC.dForce[4] > -cDGN_Zero;
//     double dMy_Ulm_max = 0.0; 
//     int nPosUlm = 0;
//     for (int i = 0; i<3; ++i)
//     {
//       CSG_FORCE_D  ForcD = m_pMembInD->aUlmf[0].ForD[i];
// 
//       double dMy_Ulm = ForcD.dForce[4];
//       if(i==0) 
//       {
//         nPosUlm = 0;
//         dMy_Ulm_max = dMy_Ulm;
//       }
//       else
//       {
//         if(bPositiveM)
//         {
//           if(dMy_Ulm_max < dMy_Ulm)
//           {
//             nPosUlm = i;
//             dMy_Ulm_max = dMy_Ulm;
//           }
//         }
//         else
//         {
//           if(dMy_Ulm_max > dMy_Ulm)
//           {
//             nPosUlm = i;
//             dMy_Ulm_max = dMy_Ulm;
//           }
//         }
// 
//       }
//     }
//     m_dMy_Ulm_max = dMy_Ulm_max;
// 
//     if(!m_pMembInD->bChkUlmf) // Unbraced LengthÀÇ ¾øÀ¸¸é ±âÁ¸°ª Àû¿ë.
//     {
//       m_dMd1 = pPosForcD->ForNC.dForce[4];
//       m_dMd2 = pPosForcD->ForLT.dForce[4];
//       m_dMd3 = pPosForcD->ForST.dForce[4];
//       m_dMuy = pPosForcD->ForLC.dForce[4]; //+pPosForcD->ForNC.dForce[4];  // NC+LT+ST=LC
//     }
//     else
//     {
//       m_bUseUnbraceMoment = TRUE;
//       CSG_FORCE_D  ForcD_NC = m_pMembInD->aUlmf[1].ForD[nPosUlm];
//       CSG_FORCE_D  ForcD_LT = m_pMembInD->aUlmf[2].ForD[nPosUlm];
//       CSG_FORCE_D  ForcD_ST = m_pMembInD->aUlmf[3].ForD[nPosUlm];
// 
//       m_dMd1 = ForcD_NC.dForce[4];
//       m_dMd2 = ForcD_LT.dForce[4];
//       m_dMd3 = ForcD_ST.dForce[4];
//       m_dMuy =  m_dMd1 + m_dMd2 + m_dMd3;
// 
//       m_dMuy = m_dMd1 + m_dMy_Ulm_max; 
//     }
//     m_dMuz = pPosForcD->ForLC.dForce[5]; //+pPosForcD->ForNC.dForce[5];
//     m_dVu  = pPosForcD->ForLC.dForce[2];
//     m_dVu_sm  = pPosForcD->ForLC.dForce[2];
//     m_dT   = pPosForcD->ForLC.dForce[3];
//     m_dT1  = pPosForcD->ForNC.dForce[3];
//     m_dT2  = pPosForcD->ForLT.dForce[3];
//     m_dT3  = pPosForcD->ForST.dForce[3];
//   }
//   else
//   {
//     m_dMd1 = pPosForcD->ForNC.dForce[4];
//     m_dMd2 = pPosForcD->ForLT.dForce[4];
//     m_dMd3 = pPosForcD->ForST.dForce[4];
//     m_dMuy = pPosForcD->ForLC.dForce[4]; //+pPosForcD->ForNC.dForce[4];  // NC+LT+ST=LC
//     m_dMuz = pPosForcD->ForLC.dForce[5]; //+pPosForcD->ForNC.dForce[5];
//     m_dVu  = pPosForcD->ForLC.dForce[2];
//     m_dVu_sm  = pPosForcD->ForLC.dForce[2];
//     m_dT   = pPosForcD->ForLC.dForce[3];
//     m_dT1  = pPosForcD->ForNC.dForce[3];
//     m_dT2  = pPosForcD->ForLT.dForce[3];
//     m_dT3  = pPosForcD->ForST.dForce[3];
//   }
// 
//   m_dVu_gr  = pPosForcD->ForNC.dForce[2];
//   m_dVu_lt  = pPosForcD->ForLT.dForce[2];
//   m_dVu_st  = pPosForcD->ForST.dForce[2];
// 
//   double dSl[2] = {0.0,0.0};
//   if(m_dMuy > 0.0) // Top flange is under compression
//   {
//     if(pMemb->aChkFor[0].nLcomType == ENUM_CONSTRUCT)
//     {
//       dSl[0] = fabs(GirderStiff.dx1) < cDGN_Zero? 0.0 : fabs(GirderStiff.dIzz / GirderStiff.dx1);
//       dSl[1] = fabs(GirderStiff.dx2) < cDGN_Zero? 0.0 : fabs(GirderStiff.dIzz / GirderStiff.dx2);
//       m_dScom_z = max(dSl[0],dSl[1]);
//     }
//     else
//     {
//       dSl[0] = fabs(PosSectD->Sect.RbarStiffn.dx1) < cDGN_Zero? 0.0 : fabs(PosSectD->Sect.RbarStiffn.dIzz / PosSectD->Sect.RbarStiffn.dx1);
//       dSl[1] = fabs(PosSectD->Sect.RbarStiffn.dx2) < cDGN_Zero? 0.0 : fabs(PosSectD->Sect.RbarStiffn.dIzz / PosSectD->Sect.RbarStiffn.dx2);
//       m_dScom_z = max(dSl[0],dSl[1]);
//     }
//   }
//   else // Bottom flange is under compression
//   {
//     if(pMemb->aChkFor[0].nLcomType == ENUM_CONSTRUCT)
//     {
//       dSl[0] = fabs(GirderStiff.dx4) < cDGN_Zero? 0.0 : fabs(GirderStiff.dIzz / GirderStiff.dx4);
//       dSl[1] = fabs(GirderStiff.dx3) < cDGN_Zero? 0.0 : fabs(GirderStiff.dIzz / GirderStiff.dx3);
//       m_dScom_z = max(dSl[0],dSl[1]);
//     }
//     else
//     {
//       dSl[0] = fabs(PosSectD->Sect.NonSlabStiff.dx4) < cDGN_Zero? 0.0 : fabs(PosSectD->Sect.NonSlabStiff.dIzz / PosSectD->Sect.NonSlabStiff.dx4);
//       dSl[1] = fabs(PosSectD->Sect.NonSlabStiff.dx3) < cDGN_Zero? 0.0 : fabs(PosSectD->Sect.NonSlabStiff.dIzz / PosSectD->Sect.NonSlabStiff.dx3);
//       m_dScom_z = max(dSl[0],dSl[1]);
//     }
//   }
// 
//   // Box Tub ÀÏ¶§ WEB ÀÌ 2°³ÀÌ±â ¶§¹®¿¡..
//   if (m_nSectType!=DGN_SECT_TYPE_COMPO_I)
//   {
//     m_dVu /= 2.0;
//     m_dVu_gr /= 2.0;
//     m_dVu_lt /= 2.0;
//     m_dVu_st /= 2.0;
//     m_dVu_sm /= 2.0;
// 
//     //ASSERT(m_nSectType != DGN_SECT_TYPE_COMPO_B);//BOXÀÏ¶§ Dist¸Â´ÂÁö È®ÀÎ
//     double dDist = m_nSectType == DGN_SECT_TYPE_COMPO_TUB ? ( (dB1 + dBf1) - dB2 ) * 0.5 : ( dB1 - dB2 ) * 0.5;
//     if (dDist > 0.0)
//     {
//       m_dVuInclined = m_dVu / cos(dDist/m_dHw);
//       m_dVu_gr /= cos(dDist/m_dHw);
//       m_dVu_lt /= cos(dDist/m_dHw);
//       m_dVu_st /= cos(dDist/m_dHw);
//       m_dVu_sm /= cos(dDist/m_dHw);
//       m_dTheta      = dDist / m_dHw;
//     }
//     else
//     {
//       m_dVuInclined = m_dVu;
//     }
//   }


	// section data - steel girder
	if ( abs(m_dMuy) == m_dMuy )
	{
		if ( m_nSectType==DGN_SECT_TYPE_COMPO_I )
		{// Top flange is in compression.
			m_dbfc = dB1;//16.0;
			m_dbfc_betweenWeb = m_dbfc;
			m_dbfc_hf = dB1;
			m_dtfc = dtf1;//1.0;
			m_dbft = dB2;//18.0;
			m_dbft_betweenWeb = m_dbft;
			m_dbft_hf = m_dbft;
			m_dtft = dtf2;//1.375;
		}
		else if ( m_nSectType==DGN_SECT_TYPE_COMPO_TUB )
		{
			m_dbfc = dBf1;
			m_dbfc_betweenWeb = dBf1;
			m_dbfc_hf = dB1;
			m_dtfc = dtf1;
			m_dbft = dB2 + dBf2 * 2.0;
			m_dbft_betweenWeb = dB2;
			m_dbft_hf = 0.5*dB2;
			m_dtft = dtf2;
		}
		else
		{
			m_dbfc = dBf1 * 2.0 + dB1;
			m_dbfc_betweenWeb = dB1;
			m_dbfc_hf = 0.5*dB1;
			m_dtfc = dtf1;
			m_dbft = dB2 + dBf2 * 2.0;
			m_dbft_betweenWeb = dB2;
			m_dbft_hf = 0.5*dB2;
			m_dtft = dtf2;
		}

		//     int nForceUnit = (m_nSelUnit==0)? D_DGN_UNITSYS_FORCE_INDEX_LBF : D_DGN_UNITSYS_FORCE_INDEX_N;
		//     int nLengthUnit= (m_nSelUnit==0)? D_DGN_UNITSYS_LENGTH_INDEX_IN : D_DGN_UNITSYS_LENGTH_INDEX_MM;
		//     m_pMembInD->Memb.Matl.MatStf.Steel.S_Fy = m_dFyc = Get_FyByThick_Code(nForceUnit, nLengthUnit, PosSectD->Sect.SectInfo.Shape, m_dtfc, m_pMembInD->Memb.Matl.MatStf); // 50.0;
		//     m_pMembInD->Memb.Matl.MatSbf.Steel.S_Fy = m_dFyt = Get_FyByThick_Code(nForceUnit, nLengthUnit, PosSectD->Sect.SectInfo.Shape, m_dtft, m_pMembInD->Memb.Matl.MatSbf); // 50.0;
		//     m_dyc = m_dCzp_cst - m_dtfc;
		//     m_dyt = m_dCzm_cst - m_dtft;
		// 
		//     m_dAft = m_dbfc*m_dtfc;
		//     m_dAfb = m_dbft*m_dtft;
		//     //PMS: BSC-20140420 MQC 4722-14 ½Ã½Ã¸£
		//     m_dy_n_slab = PosSectD->Sect.RbarStiffn.dCzp - m_dtfc + m_dtc + m_dHh;
	}
	else
	{
		if ( m_nSectType==DGN_SECT_TYPE_COMPO_I )
		{// Top flange is in tension.
			m_dbfc = dB2;//18.0;
			m_dbfc_betweenWeb = dB2;
			m_dbfc_hf = dB2;
			m_dtfc = dtf2;//1.375;
			m_dbft = dB1;//16.0;
			m_dbft_betweenWeb = dB1;
			m_dbft_hf = dB1;
			m_dtft = dtf1;//1.0;
		}
		else if ( m_nSectType==DGN_SECT_TYPE_COMPO_TUB )
		{
			m_dbfc = dB2 + dBf2 * 2.0;
			m_dbfc_hf = 0.5*dB2;
			m_dbfc_betweenWeb = dB2;
			m_dtfc = dtf2;
			m_dbft = dBf1;
			m_dbft_betweenWeb = dBf1;
			m_dbft_hf = dBf1;
			m_dtft = dtf1;
		}
		else
		{
			m_dbfc = dB2 + dBf2 * 2.0;
			m_dbfc_betweenWeb = dB2;
			m_dbfc_hf = 0.5*dB2;
			m_dtfc = dtf2;
			m_dbft = dB1 + dBf1 * 2.0;
			m_dbft_betweenWeb = dB1;
			m_dbft_hf = 0.5*dB1;
			m_dtft = dtf1;
		}
		//     int nForceUnit = (m_nSelUnit==0)? D_DGN_UNITSYS_FORCE_INDEX_LBF : D_DGN_UNITSYS_FORCE_INDEX_N;
		//     int nLengthUnit= (m_nSelUnit==0)? D_DGN_UNITSYS_LENGTH_INDEX_IN : D_DGN_UNITSYS_LENGTH_INDEX_MM;
		//     m_pMembInD->Memb.Matl.MatSbf.Steel.S_Fy = m_dFyc = Get_FyByThick_Code(nForceUnit, nLengthUnit, PosSectD->Sect.SectInfo.Shape, m_dtfc, m_pMembInD->Memb.Matl.MatSbf); // 50.0;
		//     m_pMembInD->Memb.Matl.MatStf.Steel.S_Fy = m_dFyt = Get_FyByThick_Code(nForceUnit, nLengthUnit, PosSectD->Sect.SectInfo.Shape, m_dtft, m_pMembInD->Memb.Matl.MatStf); // 50.0;
		//     m_dyc = m_dCzm_cn - m_dtfc;
		//     m_dyt = m_dCzp_cn - m_dtft;  // m_dCzp_cst
		// 
		//     m_dAft = m_dbft*m_dtft;
		//     m_dAfb = m_dbfc*m_dtfc;
		//     //PMS: BSC-20140420 MQC 4722-14 ½Ã½Ã¸£
		//     m_dy_n_slab = PosSectD->Sect.RbarStiffn.dCzp - m_dtft + m_dtc + m_dHh;
	}
	m_dH   = dH;
	m_dD   = dD;//69.0;    // web ¼ø±æÀÌ
	m_dtw  = dtw;//0.5;


//   int nForceUnit = (m_nSelUnit==0)? D_DGN_UNITSYS_FORCE_INDEX_LBF : D_DGN_UNITSYS_FORCE_INDEX_N;
//   int nLengthUnit= (m_nSelUnit==0)? D_DGN_UNITSYS_LENGTH_INDEX_IN : D_DGN_UNITSYS_LENGTH_INDEX_MM;
//   m_pMembInD->Memb.Matl.MatSw.Steel.S_Fy = m_dFyw = Get_FyByThick_Code(nForceUnit, nLengthUnit, PosSectD->Sect.SectInfo.Shape, m_dtw, m_pMembInD->Memb.Matl.MatSw); // 50.0;
//   m_dE  = pMemb->Memb.Matl.MatSw.Steel.Elast;//29000.0;
//   double aSize[8] = {0.0};
// 
//   // Get Stress
//    Calc_FlangeStress(m_iShapeType, m_dfbuc, m_dfbut, m_dft_g, m_dfb_g, m_dft_st, m_dfb_st, m_dft_lt, m_dfb_lt, m_dftop, m_dfbot);
// 
//   m_dAoh    = Calc_Ao();
//   m_dAoh_nc = Calc_Ao(TRUE);

	// shear connector
//   m_iType   = PosSectD->Scon.nType;    // 1:stud-shear, 2:channel // 0:stud shear, 1:channel
//   m_nCategoryType_SC = PosSectD->Scon.nCategory;
//   m_dH_SC   = PosSectD->Scon.dHeight;//PosSectD->Scon.dPitch;      // Overall height //7.0;
//   m_dd_SC   = PosSectD->Scon.dDia;      // Diameter or width of channel //0.875;
//   m_dW_SC   = PosSectD->Scon.dSpace;      // Diameter or width of channel //0.875;
//   m_dp_use  = PosSectD->Scon.dPitch;    // Longitudinal spacing //12.0; // ¼³Ä¡ °£°Ý
//   m_dst_use = PosSectD->Scon.dSpace;    // C to C // 12.0; // ¼³Ä¡ °£°Ý
//   m_nn      = PosSectD->Scon.nNum;      // Number of connectors(transverse) // È¾´Ü¸éÀ¸·Î ¹èÄ¡µÈ Àü´Ü¿¬°áÀçÀÇ °³¼ö // 3
//   m_dFyu    = PosSectD->Scon.dFu;       // Ultimate tensile strength //60.0;
//   m_dFrc    = PosSectD->Scon.dFrc; 
//   m_dLength4SC = pMemb->Memb.PosD[nPosIndex].Scon.dLength;
//   m_nSFCalc    = pMemb->Memb.PosD[nPosIndex].Scon.nSFCalc;  
//   m_nWarpType  = pMemb->Memb.PosD[nPosIndex].Fati.nWarpType;
//   m_dTopFlange = pMemb->Memb.PosD[nPosIndex].Fati.dTopFlange;
//   m_dBotFlange = pMemb->Memb.PosD[nPosIndex].Fati.dBotFlange;

	// Longitudinal Stiffeners
	int nStiffener = 0;
	//   int nStiffenerL = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffLeft.GetSize();
	//   int nStiffenerR = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffRight.GetSize();

	//   m_aStiffTop.RemoveAll();    m_aStiffTop.SetSize(PosSectD->Sect.SectInfo.Before.Stiffener.aStiffTop.GetSize());
	//   m_aStiffBottom.RemoveAll(); m_aStiffBottom.SetSize( PosSectD->Sect.SectInfo.Before.Stiffener.aStiffBottom.GetSize());
	//   for(int k = 0; k < PosSectD->Sect.SectInfo.Before.Stiffener.aStiffTop.GetSize(); ++k)    
	//     m_aStiffTop[k].dSpacing     = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffTop[k].dSpacing;
	//   for(int j = 0; j < PosSectD->Sect.SectInfo.Before.Stiffener.aStiffBottom.GetSize(); ++j)  
	//     m_aStiffBottom[j].dSpacing  = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffBottom[j].dSpacing;


	//   if(m_nSectType == DGN_SECT_TYPE_COMPO_I)
	//   {
	//     if(PosSectD->Sect.SectInfo.Before.Stiffener.nPosition == 0) // both
	//     {
	//       m_nLongStiNum_Left =  PosSectD->Sect.SectInfo.Before.Stiffener.aStiffLeft.GetSize();
	//       m_nLongStiNum_Right = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffLeft.GetSize();
	//     }
	//     else if(PosSectD->Sect.SectInfo.Before.Stiffener.nPosition == 1) // left
	//     {
	//       m_nLongStiNum_Left =  PosSectD->Sect.SectInfo.Before.Stiffener.aStiffLeft.GetSize();
	//       m_nLongStiNum_Right  = 0;
	//     }
	//     else if(PosSectD->Sect.SectInfo.Before.Stiffener.nPosition == 2) // right
	//     {
	//       m_nLongStiNum_Left =  0;
	//       m_nLongStiNum_Right = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffLeft.GetSize();
	//     }
	//   }
	//   else if(m_nSectType == DGN_SECT_TYPE_COMPO_B || m_nSectType == DGN_SECT_TYPE_COMPO_TUB)
	//   {    
	//     m_nLongStiNum_Left  = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffLeft.GetSize();
	//     m_nLongStiNum_Right = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffRight.GetSize();
	//   }
	// 
	// 
	//   int nStiffenerWeb = nStiffenerL + nStiffenerR;
	//   m_nType_ls  = m_nSectType == DGN_SECT_TYPE_COMPO_I ? (PosSectD->Sect.SectInfo.Before.Stiffener.nPosition==0 ? 2 : 1) : 1;
	//   ST_STIFF_PROFILE_D StiffProfD;
	//   StiffProfD.Initialize();
	// 
	//   ST_STIFF_PROFILE_D StiffProf_CF; // ¾ÐÃà ÇÃ·£Áö 
	//   int nCompFlgStfn = 0;
	//   int nPartIdx = 0;
	// 
	//   double dDb=0.0;
	//   // 0:Both 1:Left 2:Right
	//   int nStiffNum = 0;
	//   for (i = 0; i<4; ++i)
	//   {
	//     //PMS:BSC-20140424 ½ºÆ¼ÇÁ³Ê¿¡ µû¶ó °ËÅäÇÏ³Ä ¾ÈÇÏ³Ä (¹ÚÃ¢½Å)
	//     nStiffNum += PosSectD->Sect.SectInfo.Before.Stiffener.nStiffNum[i];
	//   }
	//   m_bExist_ls_flg = FALSE;
	// 
	//   if (m_nSectType == DGN_SECT_TYPE_COMPO_I)
	//   {
	//     if (PosSectD->Sect.SectInfo.Before.Stiffener.nPosition == 2)
	//     {
	//       nStiffener = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffLeft.GetSize();
	//       if(nStiffener>0)
	//         StiffProfD = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffLeft[0];
	//     }
	//     else
	//     {
	//       nStiffener = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffRight.GetSize();
	//       if(nStiffener>0)
	//         StiffProfD = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffRight[0];
	//     }    
	//   }
	//   else if (m_nSectType == DGN_SECT_TYPE_COMPO_B)
	//   {    
	// 
	//     if (IsPositiveMoment(m_dMuy))
	//     {
	//       nPartIdx = 2;
	//       nCompFlgStfn = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffTop.GetSize();  
	//       dDb = dB1-m_dtw;
	//     }
	//     else
	//     {
	//       nPartIdx = 3;
	//       nCompFlgStfn = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffBottom.GetSize();
	//       dDb = dB2-m_dtw;
	//     }    
	// 
	//   }
	//   else if (m_nSectType == DGN_SECT_TYPE_COMPO_TUB)
	//   {
	//     if (IsPositiveMoment(m_dMuy)==FALSE)
	//     {
	//       nPartIdx = 3;
	//       nCompFlgStfn = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffBottom.GetSize();
	//       dDb = dB2-m_dtw;
	//     }
	//   }
	// 
	//   m_nLStfnNumCFlg = nCompFlgStfn;
	//   if (nCompFlgStfn>0) m_bExist_ls_flg = TRUE; 
	// 
	//   int nLongStfnType=0;
	//   int nLongStfnIdx =0;
	// 
	//   m_dIs = m_dIs_web = 0.0;
	//   double dw=0.0, dMaxCTC=0.0, dtp=0.0, dbl=0.0, dIl=0.0, dSpa1=0.0, dSpa2=0.0;
	//   double dA_to=0.0, dr_to=0.0;
	//   double dAst=0.0, dIst=0.0, dr_st=0.0, drz_st=0.0;
	//   double dd0LS=0.0;
	//   if (m_nSectType == DGN_SECT_TYPE_COMPO_B || m_nSectType == DGN_SECT_TYPE_COMPO_TUB)
	//   {
	//     ST_SECT_STIFF_D &StfnD = PosSectD->Sect.SectInfo.Before.Stiffener;
	//     m_pStlDesign->Get_LongStfnData(m_nSectType, nPartIdx, dDb, m_dtfc, StfnD, dtp, dbl, dIl, m_dIs, m_dIs_web, dA_to, dr_to, nLongStfnType, nLongStfnIdx, dd0LS);
	//     m_pStlDesign->Get_LongStfnCTCInfo(m_iShapeType, m_adSize, nPartIdx, StfnD, dMaxCTC, dw, dSpa1, dSpa2);
	// 
	//     if (dw<=0.0) dw = dDb;
	//     m_dw = dw;
	// 
	//     //if      (nStiffenerL>0) StiffProfD = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffLeft[0];
	//     //else if (nStiffenerR>0) StiffProfD = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffRight[0];
	//   }


	//   m_bExist_ls = nStiffenerWeb>0 ? TRUE : FALSE;
	// 
	// 
	//   if (PosSectD->Sect.SectInfo.Before.Stiffener.aStiffShape.GetSize() > 0)
	//   {
	//     ST_STIFF_SHAPE_D   &StiffShapeD = PosSectD->Sect.SectInfo.Before.Stiffener.aStiffShape[StiffProfD.nShape];
	// 
	//     int nType_ls = StiffShapeD.nType; // 0:Solid Rectangular, 1:T-Section, 2:U-Rib  
	// 
	//     if(nType_ls==0) 
	//     {
	//       m_dbl_ls    = StiffShapeD.dSize[0];  // for solid rect. [0] : H , [1] : B
	//       m_dts_ls    = StiffShapeD.dSize[1];
	//     }
	//     else if(nType_ls==1)
	//     {
	//       m_dbl_ls    = StiffShapeD.dSize[1] / 2.0;  // MQC No.70 : T-shape stiffener projecting width´Â flange widthÀÇ ¹Ý°ª Àû¿ë (14.06.10)  by shishir & PCS  
	//       m_dts_ls    = StiffShapeD.dSize[3];  // for Tee. [0] : H , [1] : B,  [2] : tw , [3] : tf
	//     }
	//     else if(nType_ls==2)
	//     {
	//       m_dbl_ls    = StiffShapeD.dSize[1];  // U-rib¿¡ ´ëÇÑ projecting width´Â ³ÐÀº Æø Àû¿ë  ÀÌ°Ç È®ÀÎ ÇÊ¿ä!
	//       m_dts_ls    = StiffShapeD.dSize[3];  // for U-rib. [0] : H , [1] : B1,  [2] : B2 , [3] : t , [4] : R
	//     }   
	//     else ASSERT(0);
	// 
	//     m_dd0_ls    = StiffProfD.dSpacing;//207.0;
	//     m_dI_ls     = StiffShapeD.Stiffness.dIyy;
	//     m_dA_ls     = StiffShapeD.Stiffness.dArea;
	//     m_dy_ls     = StiffShapeD.Stiffness.dCzm;
	//     m_dI_FromCenter_ls = m_dI_ls + (m_dA_ls * m_dy_ls * m_dy_ls);
	//     m_dFys_ls   = m_dFyw;
	//   }
	// 
	//   // Transverse Stiffeners
	//   m_bExist_ts = PosSectD->Trst.bUseStifWeb;
	//   if(SectInfo.nStype == DGN_SECT_TYPE_COMPO_I)
	//   {
	//     BOOL bExsitWebStiff = SectInfo.Before.Stiffener.aStiffLeft.GetSize() > 0 ? TRUE : FALSE;
	//     m_bExist_longStiff_Web = bExsitWebStiff;
	//   }
	//   else if(SectInfo.nStype == DGN_SECT_TYPE_COMPO_B || SectInfo.nStype == DGN_SECT_TYPE_COMPO_TUB)
	//   {
	//     BOOL bExsitWebStiff_left  = SectInfo.Before.Stiffener.aStiffLeft.GetSize() > 0 ? TRUE : FALSE;
	//     BOOL bExsitWebStiff_Right = SectInfo.Before.Stiffener.aStiffRight.GetSize() > 0 ? TRUE : FALSE;
	// 
	//     m_bExist_longStiff_Web = (bExsitWebStiff_left || bExsitWebStiff_Right) ? TRUE : FALSE;
	//   }
	//   else{ASSERT(0);}
	//   m_nType_ts  = PosSectD->Trst.nStifNumWeb==0 ? 1 : 2; // 1:1¸é, 2:2¸é
	//   m_dbt_ts    = PosSectD->Trst.dSizeWeb[0];           // 0: H 1:B //5.0;
	//   m_dtp_ts    = PosSectD->Trst.dSizeWeb[1];           // 0: H 1:B //0.5;
	//   m_dd0_ts    = PosSectD->Trst.dPitchWeb; //207.0;
	//   m_dFys_ts   = PosSectD->Trst.dFyWeb; //50.0;
	// 
	//   // !! Bearing Stiffener
	//   //   m_nType_ts  = PosSectD->Trst.nStifNumWeb==0 ? 1 : 2; // 1:1¸é, 2:2¸é
	//   //   m_dbt_ts    = PosSectD->Trst.dSizeWeb[0];           // 0: H 1:B //5.0;
	//   //   m_dtp_ts    = PosSectD->Trst.dSizeWeb[1];           // 0: H 1:B //0.5;
	//   //   m_dd0_ts    = PosSectD->Trst.dClip4Fillet; //207.0;
	//   m_dFys_bs  = PosSectD->Trst.dFyBearing; //50.0;
	// 
	// 
	// 
	//   // Deck Overhang Loads
	//   m_bUseDohl   = PosSectD->Dohl.bUseDohl; 
	//   m_dDistForce = PosSectD->Dohl.dDistForce;
	//   m_dConcForce = PosSectD->Dohl.dConcForce;
	//   m_dEccenLoad = PosSectD->Dohl.dEccenLoad; 
	// 
	//   // Unbraced Length
	//   m_dLb = pMemb->Memb.Ulen.dLb; //24.0*12.0; // 24ft  
	//   m_bUseUnbraceMoment = FALSE;
	// 
	//   // fatigue
	//   m_dADTT_SL    = pMemb->Memb.PosD[nPosIndex].Fati.dADTT;// 1600.0; // 0.8*2000 PosSectD->Fati.dADTT_SL;  
	//   m_nDetailCat  = pMemb->Memb.PosD[nPosIndex].Fati.nDetailCategory;
	//   m_dn_Fati     = pMemb->Memb.PosD[nPosIndex].Fati.dNum;
	// 
	//   m_bShored     = FALSE; // Suppose unshored design for AASHTO LRFD Steel Composite
	//   m_bCantilever = pMemb->Memb.ChIf.bCantilever;
	// 
	//   m_dRh = Calc_Rh(m_pMembRes->RhD);
	// 
	//   //Box
	//   m_bMultiBox = pMemb->Memb.Csgd.nGirderType == 1 ? TRUE : FALSE;


		////////////////////END From AASHTO



	return TRUE;
}

CString CRptPlateGirder_IRC22::Get_String4PartShape(int nPartType)
{
	CString strPartType;
	if ( nPartType==0 ) { strPartType = _T("Top Flange"); }
	else if ( nPartType==1 ) { strPartType = _T("Bottom Flange"); }
	else if ( nPartType==2 ) { strPartType = _T("Web"); }
	else ASSERT(0);

	return strPartType;
}

CString CRptPlateGirder_IRC22::Get_String4ShearLamdaw(BOOL bWeb, int iLamda_w_Type)
{
	CString strLamdawType;
	if ( bWeb )
	{
		if ( iLamda_w_Type==0 ) strLamdawType = _T("Shear_buckling_Lamda_w_type1");
		else if ( iLamda_w_Type==1 ) strLamdawType = _T("Shear_buckling_Lamda_w_type2");
		else if ( iLamda_w_Type==2 ) strLamdawType = _T("Shear_buckling_Lamda_w_type3");
		else ASSERT(0);
	}
	else
	{
		if ( iLamda_w_Type==0 ) strLamdawType = _T("Shear_buckling_Lamda_flange_type1");
		else if ( iLamda_w_Type==1 ) strLamdawType = _T("Shear_buckling_Lamda_flange_type2");
		else if ( iLamda_w_Type==2 ) strLamdawType = _T("Shear_buckling_Lamda_flange_type3");
		else ASSERT(0);
	}

	return strLamdawType;
}

CString CRptPlateGirder_IRC22::Get_String4ShearSubPanelLamdaw(BOOL bWeb, int iLamda_w_Type)
{
	CString strLamdawType;
	if ( bWeb )
	{
		if ( iLamda_w_Type==0 ) strLamdawType = _T("Shear_buckling_Lamda_w_sub_type1");
		else if ( iLamda_w_Type==1 ) strLamdawType = _T("Shear_buckling_Lamda_w_sub_type2");
		else if ( iLamda_w_Type==2 ) strLamdawType = _T("Shear_buckling_Lamda_w_sub_type3");
		else ASSERT(0);
	}
	else
	{
		//if     (iLamda_w_Type==0) strLamdawType = _T("Shear_buckling_Lamda_flange_type1");
		//else if(iLamda_w_Type==1) strLamdawType = _T("Shear_buckling_Lamda_flange_type2");
		//else if(iLamda_w_Type==2) strLamdawType = _T("Shear_buckling_Lamda_flange_type3");
		//else ASSERT(0);
	}

	return strLamdawType;
}


CString CRptPlateGirder_IRC22::Get_String4ShearXwType(BOOL bWeb, int iEndSupportType, int iLamda_w_scope)
{
	CString strXwType;
	if ( bWeb )
	{
		if ( iEndSupportType==0 ) // Rigid end post
		{
			if ( iLamda_w_scope==0 ) { strXwType = _T("Shear_buckling_Xw_Rigid_end_post_type1"); }
			else if ( iLamda_w_scope==1 ) { strXwType = _T("Shear_buckling_Xw_Rigid_end_post_type2"); }
			else if ( iLamda_w_scope==2 ) { strXwType = _T("Shear_buckling_Xw_Rigid_end_post_type3"); }
			else ASSERT(0);
		}
		else if ( iEndSupportType==1 || iEndSupportType==2 )
		{
			if ( iLamda_w_scope==0 ) { strXwType = _T("Shear_buckling_Xw_Rigid_end_post_type1"); }
			else if ( iLamda_w_scope==1 ) { strXwType = _T("Shear_buckling_Xw_Rigid_end_post_type2"); }
			else if ( iLamda_w_scope==2 ) { strXwType = _T("Shear_buckling_Xw_Rigid_end_post_type4"); }
			else ASSERT(0);
		}
		else ASSERT(0);
	}
	else
	{
		if ( iEndSupportType==0 ) // Rigid end post
		{
			if ( iLamda_w_scope==0 ) { strXwType = _T("Shear_buckling_X_flange_Rigid_end_post_type1"); }
			else if ( iLamda_w_scope==1 ) { strXwType = _T("Shear_buckling_X_flange_Rigid_end_post_type2"); }
			else if ( iLamda_w_scope==2 ) { strXwType = _T("Shear_buckling_X_flange_Rigid_end_post_type3"); }
			else ASSERT(0);
		}
		else if ( iEndSupportType==1 || iEndSupportType==2 )
		{
			if ( iLamda_w_scope==0 ) { strXwType = _T("Shear_buckling_X_flange_Rigid_end_post_type1"); }
			else if ( iLamda_w_scope==1 ) { strXwType = _T("Shear_buckling_X_flange_Rigid_end_post_type2"); }
			else if ( iLamda_w_scope==2 ) { strXwType = _T("Shear_buckling_X_flange_Rigid_end_post_type4"); }
			else ASSERT(0);
		}
		else ASSERT(0);
	}

	return strXwType;
}

CString CRptPlateGirder_IRC22::GetLoadCombinationName(int OrgLcomK)
{
	CString strLcomName = _T("");
	T_LCOM_D LcomData;
	if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, OrgLcomK, LcomData) )
	{
		strLcomName = LcomData.LoadCombName;
	}
	return strLcomName;
}

CString CRptPlateGirder_IRC22::GetStressPosition(int iPosition)
{
	CString strPosi; strPosi.Empty();

	switch ( iPosition )
	{
	case 0: {strPosi = _T("Top-left fiber in the flange");		break; }
	case 1: {strPosi = _T("Top-right fiber in the flange");		break; }
	case 2: {strPosi = _T("Bottom-right fiber in the flange");	break; }
	case 3: {strPosi = _T("Bottom-left fiber in the flange");		break; }
	default:
		ASSERT(0);
	}

	return strPosi;
}


int CRptPlateGirder_IRC22::CopyRange(CMSExcel* pExcel, CString& RptBlock, int stRow)
{
	int rowCnt=0, colCnt=0;
	pExcel->CopyRange(stRow, 0, m_strBaseBook, m_strBaseSheet, RptBlock, rowCnt, colCnt);
	return rowCnt;
}

void CRptPlateGirder_IRC22::SetRptData(ElemPairK ElemK, int nPosi, CSG_MEMB_POSD& InData, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, T_SECT_STIFFNESS& SpscSect, T_PGBR_BASE& PgbrBase)
{
	//RptData Calculation
	BOOL bPosI = (nPosi%2==0 ? TRUE : FALSE);
	T_ELEM_D ElemD;
	ElemD.Initialize();
	BOOL bGetElem=FALSE;
	BOOL bLTerm = FALSE;
	BOOL bCrack = FALSE;
	BOOL bPostMode = TRUE;
	if (ElemK.second == EN_EL_BEAM)
	{
		if (bPostMode)	bGetElem = m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD);
		else					bGetElem = m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemD);
		if (!bGetElem)	ASSERT(0);
	}

	T_SECT_K SectK = ElemD.elpro;
	SectPairK SPairK(0, 0);

	////////////////////////////
	// Get Sect Data.
	T_SECT_K OrgSectK=0;
	T_SECT_D SectD;
	SectD.Initialize();
	BOOL bGetSect = m_pDoc->m_pPostCtrl->GetSectPostDgn(ElemK, SectD);
	if (ElemK.second == EN_EL_BEAM)
	{
		if (bPostMode)
		{
			OrgSectK = m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(SectK);
		}
		else
		{
			OrgSectK = SectK;
		}
		SPairK = make_pair(SectK, ElemK.second);
	}
	else if (ElemK.second == EN_EL_VBEAM)
	{
		SPairK = make_pair(ElemK.first, ElemK.second);
	}
	else ASSERT(0);

	BOOL bSectOK=FALSE;
	if ( SectD.nStype==D_SECT_TYPE_TAPERED )
	{
		if ( CDBLib::IsCompositeSect(SectD.SectBefore.nStype) )    // SectD.SectBefore.nStype==D_SECT_TYPE_COMPO_I || SectD.SectBefore.nStype==D_SECT_TYPE_COMPO_B
			bSectOK = TRUE;
	}
	else if ( CDBLib::IsCompositeSect(SectD.nStype) )	bSectOK = TRUE;  // SectD.nStype==D_SECT_TYPE_COMPO_I || SectD.nStype==D_SECT_TYPE_COMPO_B
	else {}//ASSERT(0);

	BOOL bTapered = SectD.nStype == D_SECT_TYPE_TAPERED ? TRUE : FALSE;

	T_SECT_STIFF_BASE CompoStiffI;
	T_SECT_STIFF_BASE CompoStiffJ;

	double dBs = bTapered && !bPosI ? SectD.CmpTapJ.Size[59] : SectD.SectAfter.SectI.Size[0];  // Slab Width   
	int    dNg = SectD.SectAfter.SectI.BuiltUpFlag;  // Numer of Main Girder
	double dds = SectD.SectAfter.SectI.Size[1];  // C.T.C.
	double dBc = SectD.SectAfter.SectJ.Size[0];
	double dtc = SectD.SectAfter.SectJ.Size[1];
	double dHh = SectD.SectAfter.SectJ.Size[2];
	//
	double dArea, dAsy, dAsz, dIxx, dIyy, dIzz, dCyp, dCym, dCzp, dCzm, dQyb, dQzb, dWArea;
	double dy1, dy2, dy3, dy4;
	double dz1, dz2, dz3, dz4;
	double dYbar, dZbar, dyDim, dzDim;
	double dArea_g, dAsy_g, dAsz_g, dIxx_g, dIyy_g, dIzz_g, dCyp_g, dCym_g, dCzp_g, dCzm_g;
	double dy1_g, dy2_g, dy3_g, dy4_g;
	double dz1_g, dz2_g, dz3_g, dz4_g;
	double dYbar_g, dZbar_g, dyDim_g, dzDim_g;
	double dShrDeform = (SectD.SectBefore.bConsiderShearDeform ? 1.0 : 0.0);

	if ( !bTapered )
	{
		if ( bLTerm==TRUE )
		{
			CompoStiffI.Stiffness.GetStiff(dArea, dAsy, dAsz, dIxx, dIyy, dIzz, dCyp, dCym, dCzp, dCzm, dQyb, dQzb, dWArea);
			dYbar = CompoStiffI.dYbar;
			dZbar = CompoStiffI.dZbar;
			dAsy  *= dShrDeform;
			dAsz  *= dShrDeform;

			dy1	   = CompoStiffI.Stiffness.dx1;
			dy2	   = CompoStiffI.Stiffness.dx2;
			dy3	   = CompoStiffI.Stiffness.dx3;
			dy4	   = CompoStiffI.Stiffness.dx4;
			dz1	   = CompoStiffI.Stiffness.dy1;
			dz2	   = CompoStiffI.Stiffness.dy2;
			dz3	   = CompoStiffI.Stiffness.dy3;
			dz4	   = CompoStiffI.Stiffness.dy4;
			dCyp	 = CompoStiffI.Stiffness.Cyp;
			dCym	 = CompoStiffI.Stiffness.Cym;
			dCzp	 = CompoStiffI.Stiffness.Czp;
			dCzm	 = CompoStiffI.Stiffness.Czm;
		}
		else
		{
			dYbar  = SectD.SectAfter.Design.YBar;
			dZbar  = SectD.SectAfter.Design.ZBar;
			dArea  = SectD.SectAfter.Stiffness.Area;
			dAsy	 = SectD.SectAfter.Stiffness.Asy * dShrDeform;
			dAsz	 = SectD.SectAfter.Stiffness.Asz * dShrDeform;
			dIxx	 = SectD.SectAfter.Stiffness.Rxx;
			dIyy	 = SectD.SectAfter.Stiffness.Ryy;
			dIzz	 = SectD.SectAfter.Stiffness.Rzz;
			dy1	   = SectD.SectAfter.Stiffness.dx1;
			dy2	   = SectD.SectAfter.Stiffness.dx2;
			dy3	   = SectD.SectAfter.Stiffness.dx3;
			dy4	   = SectD.SectAfter.Stiffness.dx4;
			dz1	   = SectD.SectAfter.Stiffness.dy1;
			dz2	   = SectD.SectAfter.Stiffness.dy2;
			dz3	   = SectD.SectAfter.Stiffness.dy3;
			dz4	   = SectD.SectAfter.Stiffness.dy4;
			dCyp	 = SectD.SectAfter.Stiffness.Cyp;
			dCym	 = SectD.SectAfter.Stiffness.Cym;
			dCzp	 = SectD.SectAfter.Stiffness.Czp;
			dCzm	 = SectD.SectAfter.Stiffness.Czm;
		}
		dyDim  = dCyp + dCym;
		dzDim  = dCzp + dCzm;
		// Girder.
		dYbar_g = SectD.SectBefore.Design.YBar;
		dZbar_g = SectD.SectBefore.Design.ZBar;
		dArea_g = SectD.SectBefore.Stiffness.Area;
		dAsy_g	= SectD.SectBefore.Stiffness.Asy * dShrDeform;
		dAsz_g	= SectD.SectBefore.Stiffness.Asz * dShrDeform;
		dIxx_g	= SectD.SectBefore.Stiffness.Rxx;
		dIyy_g	= SectD.SectBefore.Stiffness.Ryy;
		dIzz_g	= SectD.SectBefore.Stiffness.Rzz;
		dy1_g	  = SectD.SectBefore.Stiffness.dx1;
		dy2_g	  = SectD.SectBefore.Stiffness.dx2;
		dy3_g	  = SectD.SectBefore.Stiffness.dx3;
		dy4_g	  = SectD.SectBefore.Stiffness.dx4;
		dz1_g	  = SectD.SectBefore.Stiffness.dy1;
		dz2_g	  = SectD.SectBefore.Stiffness.dy2;
		dz3_g	  = SectD.SectBefore.Stiffness.dy3;
		dz4_g	  = SectD.SectBefore.Stiffness.dy4;
		dCyp_g	= SectD.SectBefore.Stiffness.Cyp;
		dCym_g	= SectD.SectBefore.Stiffness.Cym;
		dCzp_g	= SectD.SectBefore.Stiffness.Czp;
		dCzm_g	= SectD.SectBefore.Stiffness.Czm;
		dyDim_g = dCyp_g + dCym_g;
		dzDim_g = dCzp_g + dCzm_g;
	}
	else
	{
		if ( bLTerm==TRUE )
		{
			T_SECT_STIFF_BASE &CsgStiff = (bPosI) ? CompoStiffI : CompoStiffJ;
			CsgStiff.Stiffness.GetStiff(dArea, dAsy, dAsz, dIxx, dIyy, dIzz, dCyp, dCym, dCzp, dCzm, dQyb, dQzb, dWArea);
			dYbar = CsgStiff.dYbar;
			dZbar = CsgStiff.dZbar;

			dy1	   = CsgStiff.Stiffness.dx1;
			dy2	   = CsgStiff.Stiffness.dx2;
			dy3	   = CsgStiff.Stiffness.dx3;
			dy4	   = CsgStiff.Stiffness.dx4;
			dz1	   = CsgStiff.Stiffness.dy1;
			dz2	   = CsgStiff.Stiffness.dy2;
			dz3	   = CsgStiff.Stiffness.dy3;
			dz4	   = CsgStiff.Stiffness.dy4;
			dCyp	 = CsgStiff.Stiffness.Cyp;
			dCym	 = CsgStiff.Stiffness.Cym;
			dCzp	 = CsgStiff.Stiffness.Czp;
			dCzm	 = CsgStiff.Stiffness.Czm;
		}
		else
		{
			dYbar  = bPosI ? SectD.SectAfter.SectI.Design.YBar : SectD.SectAfter.SectJ.Design.YBar;
			dZbar  = bPosI ? SectD.SectAfter.SectI.Design.ZBar : SectD.SectAfter.SectJ.Design.ZBar;
			dArea  = bPosI ? SectD.SectAfter.SectI.Stiffness.Area : SectD.SectAfter.SectJ.Stiffness.Area;
			dAsy	 = bPosI ? SectD.SectAfter.SectI.Stiffness.Asy  : SectD.SectAfter.SectJ.Stiffness.Asy;
			dAsz	 = bPosI ? SectD.SectAfter.SectI.Stiffness.Asz  : SectD.SectAfter.SectJ.Stiffness.Asz;
			dIxx	 = bPosI ? SectD.SectAfter.SectI.Stiffness.Rxx  : SectD.SectAfter.SectJ.Stiffness.Rxx;
			dIyy	 = bPosI ? SectD.SectAfter.SectI.Stiffness.Ryy  : SectD.SectAfter.SectJ.Stiffness.Ryy;
			dIzz	 = bPosI ? SectD.SectAfter.SectI.Stiffness.Rzz  : SectD.SectAfter.SectJ.Stiffness.Rzz;
			dCyp	 = bPosI ? SectD.SectAfter.SectI.Stiffness.Cyp  : SectD.SectAfter.SectJ.Stiffness.Cyp;
			dCym	 = bPosI ? SectD.SectAfter.SectI.Stiffness.Cym  : SectD.SectAfter.SectJ.Stiffness.Cym;
			dCzp	 = bPosI ? SectD.SectAfter.SectI.Stiffness.Czp  : SectD.SectAfter.SectJ.Stiffness.Czp;
			dCzm	 = bPosI ? SectD.SectAfter.SectI.Stiffness.Czm  : SectD.SectAfter.SectJ.Stiffness.Czm;
			dy1	   = bPosI ? SectD.SectAfter.SectI.Stiffness.dx1  : SectD.SectAfter.SectJ.Stiffness.dx1;
			dy2	   = bPosI ? SectD.SectAfter.SectI.Stiffness.dx2  : SectD.SectAfter.SectJ.Stiffness.dx2;
			dy3	   = bPosI ? SectD.SectAfter.SectI.Stiffness.dx3  : SectD.SectAfter.SectJ.Stiffness.dx3;
			dy4	   = bPosI ? SectD.SectAfter.SectI.Stiffness.dx4  : SectD.SectAfter.SectJ.Stiffness.dx4;
			dz1	   = bPosI ? SectD.SectAfter.SectI.Stiffness.dy1  : SectD.SectAfter.SectJ.Stiffness.dy1;
			dz2	   = bPosI ? SectD.SectAfter.SectI.Stiffness.dy2  : SectD.SectAfter.SectJ.Stiffness.dy2;
			dz3	   = bPosI ? SectD.SectAfter.SectI.Stiffness.dy3  : SectD.SectAfter.SectJ.Stiffness.dy3;
			dz4	   = bPosI ? SectD.SectAfter.SectI.Stiffness.dy4  : SectD.SectAfter.SectJ.Stiffness.dy4;
		}
		dyDim  = dCyp + dCym;
		dzDim  = dCzp + dCzm;
		// Girder.    
		dYbar_g  = bPosI ? SectD.SectBefore.SectI.Design.YBar : SectD.SectBefore.SectJ.Design.YBar;
		dZbar_g  = bPosI ? SectD.SectBefore.SectI.Design.ZBar : SectD.SectBefore.SectJ.Design.ZBar;
		dArea_g  = bPosI ? SectD.SectBefore.SectI.Stiffness.Area : SectD.SectBefore.SectJ.Stiffness.Area;
		dAsy_g	 = bPosI ? SectD.SectBefore.SectI.Stiffness.Asy  : SectD.SectBefore.SectJ.Stiffness.Asy;
		dAsz_g	 = bPosI ? SectD.SectBefore.SectI.Stiffness.Asz  : SectD.SectBefore.SectJ.Stiffness.Asz;
		dIxx_g	 = bPosI ? SectD.SectBefore.SectI.Stiffness.Rxx  : SectD.SectBefore.SectJ.Stiffness.Rxx;
		dIyy_g	 = bPosI ? SectD.SectBefore.SectI.Stiffness.Ryy  : SectD.SectBefore.SectJ.Stiffness.Ryy;
		dIzz_g	 = bPosI ? SectD.SectBefore.SectI.Stiffness.Rzz  : SectD.SectBefore.SectJ.Stiffness.Rzz;
		dCyp_g	 = bPosI ? SectD.SectBefore.SectI.Stiffness.Cyp  : SectD.SectBefore.SectJ.Stiffness.Cyp;
		dCym_g	 = bPosI ? SectD.SectBefore.SectI.Stiffness.Cym  : SectD.SectBefore.SectJ.Stiffness.Cym;
		dCzp_g	 = bPosI ? SectD.SectBefore.SectI.Stiffness.Czp  : SectD.SectBefore.SectJ.Stiffness.Czp;
		dCzm_g	 = bPosI ? SectD.SectBefore.SectI.Stiffness.Czm  : SectD.SectBefore.SectJ.Stiffness.Czm;
		dy1_g	   = bPosI ? SectD.SectBefore.SectI.Stiffness.dx1  : SectD.SectBefore.SectJ.Stiffness.dx1;
		dy2_g	   = bPosI ? SectD.SectBefore.SectI.Stiffness.dx2  : SectD.SectBefore.SectJ.Stiffness.dx2;
		dy3_g	   = bPosI ? SectD.SectBefore.SectI.Stiffness.dx3  : SectD.SectBefore.SectJ.Stiffness.dx3;
		dy4_g	   = bPosI ? SectD.SectBefore.SectI.Stiffness.dx4  : SectD.SectBefore.SectJ.Stiffness.dx4;
		dz1_g	   = bPosI ? SectD.SectBefore.SectI.Stiffness.dy1  : SectD.SectBefore.SectJ.Stiffness.dy1;
		dz2_g	   = bPosI ? SectD.SectBefore.SectI.Stiffness.dy2  : SectD.SectBefore.SectJ.Stiffness.dy2;
		dz3_g	   = bPosI ? SectD.SectBefore.SectI.Stiffness.dy3  : SectD.SectBefore.SectJ.Stiffness.dy3;
		dz4_g	   = bPosI ? SectD.SectBefore.SectI.Stiffness.dy4  : SectD.SectBefore.SectJ.Stiffness.dy4;
		dyDim_g  = dCyp_g + dCym_g;
		dzDim_g  = dCzp_g + dCzm_g;
	}

	int i=0, j=0;
	////////////////////////////
	// Get Rbar Data.
	double dEs=0.0;
	double dAs=0.0;
	double dyz[2]= { 0.,0. };

	CArray<T_SPSC_RBAR, T_SPSC_RBAR> ArSpscRbar;
	ArSpscRbar.RemoveAll();
	BOOL bUseDgn=TRUE;
	BOOL bMbar = FALSE;
	CSectUtil::GetCompoRebarPosi(bPostMode, bCrack, bMbar, SPairK, bPosI, bUseDgn, ArSpscRbar);
	double EsEc = SectD.SectBefore.Matl_Elast;    // Es/Ec
	double DsDc = SectD.SectBefore.Matl_Density;  // Ds/Dc
	if ( bLTerm==TRUE )
	{
		if ( SectD.SectBefore.bMultipleElastic==TRUE )
		{
			if ( SectD.SectBefore.dLongTermESEC>0.0 )
				EsEc = SectD.SectBefore.dLongTermESEC;
		}
	}
	double dSlabGirderE = (EsEc==0.0 ? 0.0 : 1.0/EsEc);
	double dRatE = !bCrack ? (1.0-dSlabGirderE) : 1.0;
	// Density
	double dSlabGirderD = (DsDc==0.0 ? 0.0 : 1.0/DsDc);
	double dRatD = !bCrack ? (1.0-dSlabGirderD) : 1.0;

	// Calculate Combined Data.
	double dACom = !bCrack ? dArea : dArea_g;
	double dyCom = 0.0;
	double dzCom = 0.0;
	double dDivRebar = 1.0;
	// Slab°¡ ¾ø¾îµµ CompositeÀÇ Ybar·Î.
	double dyArea= !bCrack ? dArea*dYbar : dArea_g*dYbar;
	double dzArea= !bCrack ? dArea*dZbar : dArea_g*dZbar_g;
	double dzYcen_slab = dzDim_g + dHh + 0.5*dtc;
	double dHt = dzDim + dHh + dtc;

	for ( i=0; i<ArSpscRbar.GetSize(); i++ )
	{
		if ( ArSpscRbar[i].dAs<=0.0 ) continue;
		dAs = ArSpscRbar[i].dAs / dDivRebar;
		dyz[0] = ArSpscRbar[i].dyz[0];
		dyz[1] = ArSpscRbar[i].dyz[1];
		dACom += dAs*dRatE;
		if ( !bCrack )
		{
			dyArea += dAs*dRatE*(dyz[0]+dYbar);
			dzArea += dAs*dRatE*(dyz[1]+dzYcen_slab);
		}
		else
		{
			dyArea += dAs*dRatE*(dyz[0]+dYbar);
			dzArea += dAs*dRatE*(dyz[1]+dzYcen_slab);
		}
	}
	dyCom = dACom==0.0 ? 0.0 : dyArea/dACom;
	dzCom = dACom==0.0 ? 0.0 : dzArea/dACom;

	if ( !bCrack )
	{
		dIyy += dArea*pow(dZbar-dzCom, 2);
		dIzz += dArea*pow(dYbar-dyCom, 2);
	}
	else
	{
		dIyy_g += dArea_g*pow(dZbar_g-dzCom, 2);
		dIzz_g += dArea_g*pow(dYbar-dyCom, 2);
	}

	////////////////////////////
	// Get Rbar Property.
	double m_dsPi=4.*atan(1.);
	double dRbarIyy=0.0, dRbarIzz=0.0;
	T_RPSC_D RpscD;
	RpscD.Initialize();
	BOOL bGetRpsc=FALSE;
	if ( bPostMode )
	{
		T_SECT_K OrgSectK = m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(SectK);
		if ( !bUseDgn )
			bGetRpsc = m_pDoc->m_pAttrCtrl->GetRpscByConsiderRbar(bMbar, OrgSectK, RpscD);
		else
			bGetRpsc = m_pDoc->m_pAttrCtrl->GetRpsdByConsiderRbar(bMbar, OrgSectK, RpscD);
	}
	else
	{
		if ( !bUseDgn )
			bGetRpsc = m_pDoc->m_pAttrCtrl->GetRpscByConsiderRbar(bMbar, SectK, RpscD);
		else
			bGetRpsc = m_pDoc->m_pAttrCtrl->GetRpsdByConsiderRbar(bMbar, SectK, RpscD);
	}
	if ( bGetRpsc )
	{
		int iPosiM = (RpscD.bMbarJ && !bPosI ? 1 : 0);
		int iMbarSize = RpscD.Mbar[iPosiM].GetSize();
		for ( i=0; i<iMbarSize; i++ )
		{
			T_RPSC_MBAR RpscMbar = RpscD.Mbar[iPosiM].GetAt(i);
			int iStaYRef    = RpscMbar.iStaYRef;	// 0:Centroid, 1:Left->Right
			int iStaZRef		= RpscMbar.iStaZRef;	// 0=Top, 1=Bottom.
			int iBarNum			= RpscMbar.iBarNum;
			CString strBarNa= RpscMbar.strBarNa;
			double dStaY		= RpscMbar.dStaY;
			double dStaZ		= RpscMbar.dStaZ;
			double dPitch		= RpscMbar.dPitch;
			int nPart       = RpscMbar.nPart;
			for ( j=0; j<iBarNum; j++ )
			{
				// Position(0,0) is Section Center.
				double dy1st = 0.0;
				double dz1st = 0.0;
				dy1st = (iStaYRef==0 ? dStaY-dPitch*(iBarNum-1)/2.-(dyCom-dYbar) : dStaY-dyCom);
				if ( nPart==2 ) // slab
					dz1st = iStaZRef==0 ? (dHt-dStaZ)-dzCom : (dzDim_g+dHh+dStaZ)-dzCom;
				else if ( nPart==1 ) // girder
					dz1st = iStaZRef==0 ? (dzDim-dStaZ)-dzCom : dStaZ-dzCom;
				else ASSERT(0);
				double dAs1	 = m_pDoc->m_pMatlDB->Get_RebarArea(strBarNa) / dDivRebar;
				double dDia	 = m_pDoc->m_pMatlDB->Get_RebarDia(strBarNa) / sqrt(dDivRebar);
				double dyPos = dy1st + j*dPitch;
				double dzPos = dz1st;
				double dRatElast = dRatE;
				dRbarIyy += dRatElast * (dAs1*pow(dzPos, 2.0) + m_dsPi*pow(dDia, 4.0)/64.);
				dRbarIzz += dRatElast * (dAs1*pow(dyPos, 2.0) + m_dsPi*pow(dDia, 4.0)/64.);
			}
		}
	}

	// Calculate Combined Data.
	double dIyCom = 0.0;
	double dIzCom = 0.0;
	if ( !bCrack )
	{
		dIyCom	= dIyy + dRbarIyy;
		dIzCom	= dIzz + dRbarIzz;
	}
	else
	{
		dIyCom	= dIyy_g + dRbarIyy;
		dIzCom	= dIzz_g + dRbarIzz;
	}

	// Get Sect Stiffness Factor.	
	T_SECF_D SecfD;
	SecfD.Initialize();
	BOOL bGetSecf = m_pDoc->m_pAttrCtrl->GetSecfSect(OrgSectK, SecfD);
	// Change by ZINU.('07.03.28). NO:2712, Separate T_SECF_D J from I for Tapered Section.
	BOOL bSecf = FALSE;
	double dSecfArea  = (bSecf && bGetSecf ? (bPosI ? SecfD.dScaleFactor[0] : SecfD.dScaleFactorJ[0]) : 1.0);
	double dSecfAsy		= (bSecf && bGetSecf ? (bPosI ? SecfD.dScaleFactor[1] : SecfD.dScaleFactorJ[1]) : 1.0);
	double dSecfAsz		= (bSecf && bGetSecf ? (bPosI ? SecfD.dScaleFactor[2] : SecfD.dScaleFactorJ[2]) : 1.0);
	double dSecfIxx		= (bSecf && bGetSecf ? (bPosI ? SecfD.dScaleFactor[3] : SecfD.dScaleFactorJ[3]) : 1.0);
	double dSecfIyy		= (bSecf && bGetSecf ? (bPosI ? SecfD.dScaleFactor[4] : SecfD.dScaleFactorJ[4]) : 1.0);
	double dSecfIzz		= (bSecf && bGetSecf ? (bPosI ? SecfD.dScaleFactor[5] : SecfD.dScaleFactorJ[5]) : 1.0);
	double dSecfWArea	= (bSecf && bGetSecf ? (bPosI ? SecfD.dScaleFactor[6] : SecfD.dScaleFactorJ[6]) : 1.0);
	// Apply Stiffness Factor.
	dACom	 *= dSecfArea;
	dAsy	 *= dSecfAsy;
	dAsz	 *= dSecfAsz;
	dIxx	 *= dSecfIxx;
	dIyCom *= dSecfIyy;
	dIzCom *= dSecfIzz;
	//
	dAsy_g *= dSecfAsy;
	dAsz_g *= dSecfAsz;
	dIxx_g *= dSecfIxx;

	////////////////////////////
	double dy_Gap = dYbar-dyCom;
	SpscSect.Area = dACom;
	SpscSect.Asy  = !bCrack ? dAsy : dAsy_g;
	SpscSect.Asz  = !bCrack ? dAsz : dAsz_g;
	SpscSect.Rxx  = !bCrack ? dIxx : dIxx_g;
	SpscSect.Ryy  = dIyCom;
	SpscSect.Rzz  = dIzCom;
	SpscSect.Cyp  = dCyp + dy_Gap;
	SpscSect.Cym  = dCym - dy_Gap;
	SpscSect.Czp  = dzDim - dzCom;
	SpscSect.Czm  = dzCom;
	SpscSect.dx1  = dy1+dy_Gap;
	SpscSect.dx2  = dy2+dy_Gap;
	SpscSect.dx3  = dy3+dy_Gap;
	SpscSect.dx4  = dy4+dy_Gap;
	SpscSect.dy1  = !bCrack ? dz1+(dZbar-dzCom) : dz1_g+(dZbar_g-dzCom);
	SpscSect.dy2  = !bCrack ? dz2+(dZbar-dzCom) : dz2_g+(dZbar_g-dzCom);
	SpscSect.dy3  = !bCrack ? dz3+(dZbar-dzCom) : dz3_g+(dZbar_g-dzCom);
	SpscSect.dy4  = !bCrack ? dz4+(dZbar-dzCom) : dz4_g+(dZbar_g-dzCom);
	//RptData SAve
	RptData.Memb.nElem  = ElemK.first;
	RptData.Memb.nPosi  = nPosi;

	double dHw   = 0.0;
	double dD    = 0.0;
	double dtw   = 0.0;
	double dB1   = 0.0;
	double dtf1  = 0.0;
	double dB2   = 0.0;
	double dtf2  = 0.0;
	double dBf1  = 0.0;
	double dBf2  = 0.0;
	double dBf3  = 0.0;
	double dH    = 0.0;

	int nSectType = InData.PosD[nPosi].Sect.SectInfo.nStype;
	nSectType=SectD.SectBefore.nStype;
	if ( nSectType == D_SECT_TYPE_COMPO_I )
	{
		dHw =   bPosI ? SectD.SectBefore.SectI.Size[0]: SectD.SectBefore.SectJ.Size[0];
		//dHw   =  InData.PosD[nPosi].Sect.SectInfo.Before.Size[0];
		dtw  =  bPosI ? SectD.SectBefore.SectI.Size[1]: SectD.SectBefore.SectJ.Size[1];
		dB1  =  bPosI ? SectD.SectBefore.SectI.Size[2]: SectD.SectBefore.SectJ.Size[2];
		dtf1 =  bPosI ? SectD.SectBefore.SectI.Size[3]: SectD.SectBefore.SectJ.Size[3];
		dB2  =  bPosI ? SectD.SectBefore.SectI.Size[4]: SectD.SectBefore.SectJ.Size[4];
		dtf2 =  bPosI ? SectD.SectBefore.SectI.Size[5]: SectD.SectBefore.SectJ.Size[5];
	}
	else
	{
		dHw   = bPosI ? SectD.SectBefore.SectI.Size[0]: SectD.SectBefore.SectJ.Size[0];
		dtw   = bPosI ? SectD.SectBefore.SectI.Size[1]: SectD.SectBefore.SectJ.Size[1];
		dB1   = bPosI ? SectD.SectBefore.SectI.Size[2]: SectD.SectBefore.SectJ.Size[2];
		dBf1  = bPosI ? SectD.SectBefore.SectI.Size[3]: SectD.SectBefore.SectJ.Size[3];
		dtf1  = bPosI ? SectD.SectBefore.SectI.Size[4]: SectD.SectBefore.SectJ.Size[4];
		dB2   = bPosI ? SectD.SectBefore.SectI.Size[5]: SectD.SectBefore.SectJ.Size[5];
		dBf2  = bPosI ? SectD.SectBefore.SectI.Size[6]: SectD.SectBefore.SectJ.Size[6];
		dtf2  = bPosI ? SectD.SectBefore.SectI.Size[7]: SectD.SectBefore.SectJ.Size[7];
		dBf3  = bPosI ? SectD.SectBefore.SectI.Size[8]: SectD.SectBefore.SectJ.Size[8];
	}

	if ( nSectType==D_SECT_TYPE_COMPO_I )
	{
		dD = dHw;
	}
	else if ( nSectType==D_SECT_TYPE_COMPO_B )
	{
		double db1 = 0.5*(dB1-dB2);
		dD = (fabs(db1)<cDGN_Zero) ? dHw : sqrt(dHw * dHw + db1*db1);
	}
	else if ( nSectType==D_SECT_TYPE_COMPO_TUB )
	{
		double db1 = 0.5*(dB1 + 2.0*(dBf1-dBf3) -dB2);
		dD = (fabs(db1)<cDGN_Zero) ? dHw : sqrt(dHw * dHw + db1*db1);
	}
	else ASSERT(0);

	double dIt = ((dB1*dtf1*dtf1*dtf1) + (dB2*dtf2*dtf2*dtf2) + (dHw*dtw*dtw*dtw))/3.0;
	// Slab Material
	// Material
 // m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_MaterialData(ElemK, m_dfy, m_dfy1, m_dEs, m_dfck, m_dEcm, m_dfsk, m_dErs);

	RptData.Memb.dBs  =  SectD.SectAfter.SectJ.Size[0];//InData.PosD[nPosi].Sect.SectInfo.Slab.Size[0];
	RptData.Memb.dts  =  SectD.SectAfter.SectJ.Size[1];//InData.PosD[nPosi].Sect.SectInfo.Slab.Size[1];
	RptData.Memb.dth  =  SectD.SectAfter.SectJ.Size[2];//InData.PosD[nPosi].Sect.SectInfo.Slab.Size[2] + dtf1;
	RptData.Memb.dfck = m_dfck;//InData.Matl.MatSlab.RC.dFck; // _T("fck")   
	RptData.Memb.dEc  = m_dEcm;//InData.Matl.MatSlab.RC.dEc; // _T("Ec")    
	double dA = 0.0;
	for ( int i = 0; i<InData.PosD[nPosi].Rbar.arRbarPosi.GetSize(); ++i )
	{
		dA += InData.PosD[nPosi].Rbar.arRbarPosi[i].dArea;
	}
	RptData.Memb.dAr  = m_dArb;  // _T("Ar")    


	// °Å´õ-´Ü¸é
	RptData.Memb.GIRDER.dbfc  = nSectType == D_SECT_TYPE_COMPO_I ? dB1 : (nSectType == D_SECT_TYPE_COMPO_TUB ? dBf1 : dB1 + dBf1 * 2.0);            // _T("bfc")     
	RptData.Memb.GIRDER.dbft  = nSectType == D_SECT_TYPE_COMPO_I ? dB2 : dB2 + dBf2 * 2.0;            // _T("bft")     
	RptData.Memb.GIRDER.dtfc  = dtf1;           // _T("tfc")     
	RptData.Memb.GIRDER.dtft  = dtf2;           // _T("tft")     
	RptData.Memb.GIRDER.dD    = dD;             // _T("D")       
	RptData.Memb.GIRDER.dtw   = dtw;            // _T("tw")      
	RptData.Memb.GIRDER.dH    = dHw + dtf1 + dtf2; // _T("H")      

	// °Å´õ-»óºÎÇÃ·£Áö
	CPG_MATL_ELEM MatlElem;
	if ( !m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_CpgMatlElem(ElemK, MatlElem) ) ASSERT(0);

	// Material
	const T_MATD_D& MatdD = MatlElem.MatdD;
	m_strMatNameSteel = MatdD.Data1.CodeMatlName;
	m_dFu = MatdD.Data1.Design.S_Fu;
	RptData.Memb.dFyr = MatdD.MainRebarData.B_fy; // _T("Fyr")   //!!!

	m_dFyt = MatdD.Data1.DesignTopFlange.S_Fy1;
	m_dFyw = MatdD.Data1.DesignWeb.S_Fy1;

	RptData.Memb.GIRDER.Top.strMatType = MatdD.Data1.CodeMatlName; // _T("Top_mat") 
	RptData.Memb.GIRDER.Top.dthick     = dtf1;// _T("Top_thk") 
	RptData.Memb.GIRDER.Top.dfy_or     = MatdD.Data1.DesignTopFlange.S_Fy1; // _T("Top_fyo") 
	RptData.Memb.GIRDER.Top.dfy        = MatdD.Data1.Design.S_Fy; // _T("Top_fy")  ÆÇÆøµÎ²²ºñ¿¡ µû¸¥ fy·Î °á°ú±¸Á¶Ã¼¿¡¼­ Ã¤¿ï°ª.
	RptData.Memb.GIRDER.Top.dfu        = MatdD.Data1.Design.S_Fu; // _T("Top_fu")  
	RptData.Memb.GIRDER.Top.strMemo    = _T("-"); // _T("Top_memo")

	// °Å´õ-ÇÏºÎÇÃ·£Áö
	RptData.Memb.GIRDER.Bottom.strMatType = MatdD.Data1.CodeMatlName; // _T("Bot_mat") 
	RptData.Memb.GIRDER.Bottom.dthick     = dtf2;// _T("Bot_thk") 
	RptData.Memb.GIRDER.Bottom.dfy_or     = MatdD.Data1.DesignBotFlange.S_Fy1; // _T("Bot_fyo") 
	RptData.Memb.GIRDER.Bottom.dfy        = MatdD.Data1.Design.S_Fy; // _T("Bot_fy")  ÆÇÆøµÎ²²ºñ¿¡ µû¸¥ fy·Î °á°ú±¸Á¶Ã¼¿¡¼­ Ã¤¿ï
	RptData.Memb.GIRDER.Bottom.dfu        = MatdD.Data1.Design.S_Fu; // _T("Bot_fu")  
	RptData.Memb.GIRDER.Bottom.strMemo    = _T("-"); // _T("Bot_memo")

	// °Å´õ-À¥ 
	RptData.Memb.GIRDER.Web.strMatType  = MatdD.Data1.CodeMatlName; // _T("Web_mat") 
	RptData.Memb.GIRDER.Web.dthick      = dtw;// _T("Web_thk") 
	RptData.Memb.GIRDER.Web.dfy_or      = MatdD.Data1.DesignWeb.S_Fy1; // _T("Web_fyo") 
	RptData.Memb.GIRDER.Web.dfy         = MatdD.Data1.Design.S_Fy; // _T("Web_fy")  ÆÇÆøµÎ²²ºñ¿¡ µû¸¥ fy·Î °á°ú±¸Á¶Ã¼¿¡¼­ Ã¤¿ï
	RptData.Memb.GIRDER.Web.dfu         = MatdD.Data1.Design.S_Fu; // _T("Web_fu")  
	RptData.Memb.GIRDER.Web.strMemo     = _T(""); // _T("Web_memo")

	// °Å´õ- ¼³°è°­µµ
	RptData.Memb.GIRDER.dFyc  = RptData.Memb.GIRDER.Top.dfy_or; //_T("Fyc")    
	RptData.Memb.GIRDER.dFyw  = RptData.Memb.GIRDER.Web.dfy_or; //_T("Fyw")    
	RptData.Memb.GIRDER.dFyt  =  RptData.Memb.GIRDER.Bottom.dfy_or; //_T("Fyt")    
	RptData.Memb.GIRDER.dEs   = MatdD.Data1.Analysis.Elast; //_T("Es")  
	RptData.Memb.GIRDER.dSlen   = MatdD.Data1.Analysis.ShearModulus == 0.0 ? 77000 : MatdD.Data1.Analysis.ShearModulus;//77000;//RptData.Memb.GIRDER.dEs / (1.0 + MatdD.Data1.Analysis.Poisson); //_T("Es")  

	// Transverse Stiffnerds
	CPG_TRST_ELEM TrstElem; TrstElem.Initialize();
	m_pDataCtrl->Get_PlateGirderPointer_IRC()->Get_CpgTrstElem(ElemK, TrstElem);



	RptData.Memb.STIFFENER_V.bUseStiff  = PgbrBase.bTrSt;
	RptData.Memb.STIFFENER_V.nShapeType = InData.PosD[nPosi].Trst.nStiffenerTypeWeb;
	RptData.Memb.STIFFENER_V.nType      = InData.PosD[nPosi].Trst.nStifNumWeb;
	RptData.Memb.STIFFENER_V.ddo        = PgbrBase.dTRSTPitch;
	RptData.Memb.STIFFENER_V.dhi = TrstElem.TrstPosi[nPosi].dht;//InData.PosD[nPosi].Trst.dSizeWeb[0];
	RptData.Memb.STIFFENER_V.dbi =  TrstElem.TrstPosi[nPosi].dt;//InData.PosD[nPosi].Trst.dSizeWeb[1];
	RptData.Memb.STIFFENER_V.dtw =  TrstElem.TrstPosi[nPosi].dPitch;//InData.PosD[nPosi].Trst.dSizeWeb[2];
	RptData.Memb.STIFFENER_V.dti = InData.PosD[nPosi].Trst.dSizeWeb[3];
	RptData.Memb.STIFFENER_V.dfy = m_dfy;

	m_bTrstProvided =PgbrBase.bTrSt;
	m_dTrstPitch =PgbrBase.dTRSTPitch;
	// Longtidudinal Stiffner


	/////
	// ´Ü¸é°è¼ö- °­Àç´Ü¸é
	//SetRptData_SectStiffness( nPosi,  SectD.SectBefore.Stiffness,      RptData.Memb.Steel); 
	//Report Steel Data

	CSG_SectionProperty& RptStlData =  RptData.Memb.Steel;
	double dIny = SectD.SectBefore.Stiffness.Ryy;
	double dZp  = SectD.SectBefore.Stiffness.Czp;
	double dZm  = SectD.SectBefore.Stiffness.Czm;
	RptStlData.dA = SectD.SectBefore.Stiffness.Area;
	RptStlData.dINA    = SectD.SectBefore.Stiffness.Ryy;                    // _T("I")
	RptStlData.dIzz    = SectD.SectBefore.Stiffness.Rzz;                    // _T("Iz")   
	RptStlData.ddTop   = SectD.SectBefore.Stiffness.Czp;                    // _T("dtop") 
	RptStlData.dSTop   = (fabs(dZp-0.0) < CSG_Zero || dZp < 0.0) ? 0.0 : dIny/dZp;     // _T("Stop") 
	RptStlData.ddBot   = SectD.SectBefore.Stiffness.Czm;                    // _T("dbot") 
	RptStlData.dSBot   = (fabs(dZm-0.0) < CSG_Zero || dZm < 0.0)? 0.0 : dIny/dZm;     // _T("Sbot") 
	RptStlData.dI       = dIt;
	RptStlData.dds      = SectD.SectBefore.Stiffness.dIw;
	// ´Ü¸é°è¼ö- ´Ü±âÇÕ¼º´Ü¸é
	//SetRptData_SectStiffness( nPosi,  InData.PosD[nPosi].Sect.RbarStiffn,    RptData.Memb.ShortComp); 
	//Short Term Composite Section
	CSG_SectionProperty& RptStCompData =  RptData.Memb.ShortComp;
	dIny = SectD.SectAfter.Stiffness.Ryy;
	dZp  = SectD.SectAfter.Stiffness.Czp;
	dZm  = SectD.SectAfter.Stiffness.Czm;
	RptStCompData.dA = SectD.SectAfter.Stiffness.Area;
	RptStCompData.dINA    = SectD.SectAfter.Stiffness.Ryy;                    // _T("I")
	RptStCompData.dIzz    = SectD.SectAfter.Stiffness.Rzz;                    // _T("Iz")   
	RptStCompData.ddTop   = SectD.SectAfter.Stiffness.Czp;                    // _T("dtop") 
	RptStCompData.dSTop   = (fabs(dZp-0.0) < CSG_Zero || dZp < 0.0) ? 0.0 : dIny/dZp;     // _T("Stop") 
	RptStCompData.ddBot   = SectD.SectAfter.Stiffness.Czm;                    // _T("dbot") 
	RptStCompData.dSBot   = (fabs(dZm-0.0) < CSG_Zero || dZm < 0.0)? 0.0 : dIny/dZm;     // _T("Sbot") 



	// ´Ü¸é°è¼ö- Àå±âÇÕ¼º´Ü¸é


	CompoStiffI.Stiffness.GetStiff(dArea, dAsy, dAsz, dIxx, dIyy, dIzz, dCyp, dCym, dCzp, dCzm, dQyb, dQzb, dWArea);

	BOOL bStiff3n    = CSectUtil::GetCompoSectCR(TRUE, TRUE, bCrack, 2, TRUE, ElemK, bPosI, bSecf, SpscSect);

	//SetRptData_SectStiffness( nPosi,  SectElem.SectPosi[nPosi].PureSect,   RptData.Memb.LongComp); 
	CSG_SectionProperty& RptStLongCompData =  RptData.Memb.LongComp;
	// dIny = SectD.SectAfter.Stiffness.Ryy;
	dIny = SpscSect.Ryy;
	dZp  = SpscSect.Czp;
	dZm  = SpscSect.Czm;
	RptStLongCompData.dA = SpscSect.Area;
	RptStLongCompData.dINA    = SpscSect.Ryy;                    // _T("I")
	RptStLongCompData.dIzz    = SpscSect.Rzz;                    // _T("Iz")   
	RptStLongCompData.ddTop   = SpscSect.Czp;                    // _T("dtop") 
	RptStLongCompData.dSTop   = (fabs(dZp-0.0) < CSG_Zero || dZp < 0.0) ? 0.0 : dIny/dZp;     // _T("Stop") 
	RptStLongCompData.ddBot   = SpscSect.Czm;                    // _T("dbot") 
	RptStLongCompData.dSBot   = (fabs(dZm-0.0) < CSG_Zero || dZm < 0.0)? 0.0 : dIny/dZm;     // _T("Sbot") 


	// ´Ü¸é°è¼ö- °­Àç´Ü¸é(ºÎ¸Ó¸ð¸àÆ®)
	SetRptData_SectStiffness(nPosi, InData.PosD[nPosi].Sect.NonSlabStiff, RptData.Memb.Steel_M);
	// ´Ü¸é°è¼ö- °­Àç+Ã¶±Ù(ºÎ¸ð¸àÆ®)
	SetRptData_SectStiffness(nPosi, InData.PosD[nPosi].Sect.NonSlabStiff2, RptData.Memb.SteelRebarComp);
}

void CRptPlateGirder_IRC22::SetRptData_SectStiffness(int nPosi, DGN_SECT_STIFFNESS & InData, CSG_SectionProperty& RptData)
{
	double dIny = InData.dIyy;
	double dZp  = InData.dCzp;
	double dZm  = InData.dCzm;
	RptData.dA      = InData.dArea;                   // _T("A")    
	RptData.dINA    = InData.dIyy;                    // _T("I")
	RptData.dIzz    = InData.dIzz;                    // _T("Iz")   
	RptData.ddTop   = InData.dCzp;                    // _T("dtop") 
	RptData.dSTop   = (fabs(dZp-0.0) < CSG_Zero || dZp < 0.0) ? 0.0 : dIny/dZp;     // _T("Stop") 
	RptData.ddBot   = InData.dCzm;                    // _T("dbot") 
	RptData.dSBot   = (fabs(dZm-0.0) < CSG_Zero || dZm < 0.0)? 0.0 : dIny/dZm;     // _T("Sbot") 
}


void CRptPlateGirder_IRC22::ConvertCGSGlobalData(T_CPGD_D &CpgdD, S_CSGD_D &rData)
{

	rData.dPhi_y = CpgdD.dPhi_y;
	rData.dPhi_u = CpgdD.dPhi_u;
	rData.dPhi_c = CpgdD.dPhi_c;
	rData.dPhi_f = CpgdD.dPhi_f;
	rData.dPhi_v = CpgdD.dPhi_v;
	rData.dPhi_se= CpgdD.dPhi_se;
	rData.dPhi_b = CpgdD.dPhi_b;
	memcpy(rData.dCSLcbFactor, CpgdD.dCSLcbFactor, sizeof(rData.dCSLcbFactor));
	rData.nGirderType = CpgdD.nGirderType;
	rData.nForceType  = CpgdD.nForceType;
	memcpy(rData.bDesignParameter, CpgdD.bDesignParameter, sizeof(rData.bDesignParameter));
	rData.nConsiderPv = CpgdD.nConsiderPv;
	rData.bAppendix6  = CpgdD.bAppendix6;

	rData.bDistortion =CpgdD.bDistortion;
	rData.b1p3RhMy    =CpgdD.b1p3RhMy;
	rData.bPBTFA4SH   =CpgdD.bPBTFA4SH;

	rData.bPrtOptSum	= CpgdD.bPrtOptSum;
	rData.bPrtOptDetail	= CpgdD.bPrtOptDetail;

	rData.dPhi_s_Flex  = CpgdD.dPhi_f;
	rData.dPhi_s_Shear = CpgdD.dPhi_v;
	rData.dPhi_s_Comp  = CpgdD.dPhi_c;
	rData.dPhi_S_tens  = CpgdD.dPhi_s_Tens;
	rData.dPhi_tos     = CpgdD.dPhi_s_Tos;
	rData.dPhi_r       = CpgdD.dPhi_y;
	rData.dPhi_SC      = CpgdD.dPhi_se;
	rData.dPhi_Con     = CpgdD.dPhi_b;
	rData.dPhi_ui      = CpgdD.dPhi_u;






}

void CRptPlateGirder_IRC22::PrintCellData(CMSExcel* pExcel, const CString strCellname, CString strData, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
	short Row =0, Col=0;
	//  short Row_Ref=0, Col_Ref=0;
	short Row_Calc=0, Col_Calc=0;
	BOOL bChk = TRUE;

	pExcel->GetCellIDByName(strCellname, Row, Col);
	bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
	if ( bChk ) pExcel->SetValue(Row_Calc, Col_Calc, strData);
}

void CRptPlateGirder_IRC22::PrintCellData(CMSExcel* pExcel, const CString strCellname, double     dData, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
	short Row=0, Col=0;
	//short Row_Ref=0, Col_Ref=0;
	short Row_Calc=0, Col_Calc=0;
	BOOL bChk = TRUE;

	pExcel->GetCellIDByName(strCellname, Row, Col);
	bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
	if ( bChk ) pExcel->SetValue(Row_Calc, Col_Calc, dData);
}


void CRptPlateGirder_IRC22::PrintCellData(CMSExcel* pExcel, const CString strBaseBook, const CString strSheetName, const CString strCellname, CString strData, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
	short Row =0, Col=0;
	//  short Row_Ref=0, Col_Ref=0;
	short Row_Calc=0, Col_Calc=0;
	BOOL bChk = TRUE;

	pExcel->GetCellIDByName(strBaseBook, strSheetName, strCellname, Row, Col);
	bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
	if ( bChk ) pExcel->SetValue(Row_Calc, Col_Calc, strData);
}

void CRptPlateGirder_IRC22::PrintCellData(CMSExcel* pExcel, const CString strBaseBook, const CString strSheetName, const CString strCellname, double     dData, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
	short Row=0, Col=0;
	//short Row_Ref=0, Col_Ref=0;
	short Row_Calc=0, Col_Calc=0;
	BOOL bChk = TRUE;

	pExcel->GetCellIDByName(strBaseBook, strSheetName, strCellname, Row, Col);
	bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
	if ( bChk ) pExcel->SetValue(Row_Calc, Col_Calc, dData);
}

BOOL CRptPlateGirder_IRC22::CalcCellID(int stRow, short Row, short Col, short Row_Ref, short Col_Ref, short &rRow_Calc, short &rCol_Calc)
{
	BOOL bChk = TRUE;

	rRow_Calc = stRow + Row - Row_Ref;
	rCol_Calc = Col - Col_Ref;

	if ( rRow_Calc < 0 || rCol_Calc < 0 ) bChk = FALSE;
	else                               bChk = TRUE;

	return bChk;


}

