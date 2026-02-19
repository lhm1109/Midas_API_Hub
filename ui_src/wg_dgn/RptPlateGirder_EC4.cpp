// RptPlateGirder_EC4.cpp: implementation of the CRptPlateGirder_EC4 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "RptPlateGirder_EC4.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\MSOfficeMgr.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\AnalysisResult.h"
#include "..\wg_db\DBLib.h"

#include "DgnProgressDlg.h"
#include "DgnPlateGirder.h"
#include "DgnCodeCtrl.h"
#include "..\wg_dbLock\LockMgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptPlateGirder_EC4::CRptPlateGirder_EC4()
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

	m_dZero = 1.0E-07;
	m_FontType = _T("Arial");

	HINSTANCE hDllCalc = GetOrLoad_ReportDllHandle(DGNENGINE_US_DLL);
	if (hDllCalc == NULL) ASSERT(0);

	m_pGetStrDgnLang = (GetStrDgnLang*)GetProcAddress(hDllCalc, "GetStrDgnLang");
	if (!m_pGetStrDgnLang) ASSERT(0);

	m_pSetStrLanguage = (SetStrLanguage*)GetProcAddress(hDllCalc, "SetStrLanguage");
	if (!m_pSetStrLanguage) ASSERT(0);

	m_iBendingChapterIx = 0;
	m_iShearChapterIx = 0;
	m_iLTBChapterIx = 0;
	m_iTransverseForceChapterIx = 0;
	m_iLongitudinalChapterShearIx = 0;
	m_iFatigueChapterIx = 0;
	m_iStressLimitChapterIx = 0;
	m_iLongShearSLSChapterIx = 0;
}

CRptPlateGirder_EC4::~CRptPlateGirder_EC4()
{
	if (m_pDataCtrl)
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
		m_pGetStrDgnLang = NULL;
		m_pSetStrLanguage = NULL;
	}
}

void CRptPlateGirder_EC4::SetUnitConvertFactor()
{
	// [Code Unit] : N, mm.
	double dPi = atan(1.0) * 4.0;

	m_dFactorForce = 0.001;
	m_dFactorLength = 0.001;
	m_dFactorLengthEach = (m_bUnitSI) ? 1.0 : 0.1;
	m_dFactorLengthMM = 1.0;
	m_dFactorMoment = 0.001 * 0.001;
	m_dFactorStress = 1.0;
	m_dFactorRebarArea = 1.0;
	m_dFactorInertiaMoment = pow(0.001, 4);
	m_dFactorAngle = 180.0 / (dPi);

	m_strUnitForce.Format(_T("0.00 %s"), ((m_bUnitSI) ? _T("kN") : _T("tonf")));
	m_strUnitLength.Format(_T("0.000 %s"), _T("m"));
	m_strUnitLengthEach.Format(_T("0.00 %s"), ((m_bUnitSI) ? _T("mm") : _T("cm")));
	m_strUnitLengthMM.Format(_T("0.00000 %s"), _T("mm"));
	m_strUnitMoment.Format(_T("0.00 %s"), ((m_bUnitSI) ? _T("kN·m") : _T("tonf·m")));
	m_strUnitStress.Format(_T("0.00 %s"), ((m_bUnitSI) ? _T("MPa") : _T("kgf/cm²")));
	m_strUnitRebarArea.Format(_T("0.00 %s"), ((m_bUnitSI) ? _T("mm²") : _T("cm²")));
	m_strUnitInertiaMoment.Format(_T("0.000000 %s"), _T("m⁴"));
	m_strUnitDegree.Format(_T("0.00 %s"), _T("˚"));

	m_strNone3.Format(_T("0.000"));
	m_strNone4.Format(_T("0.0000"));

	//m_strTens = _T("..... 인장");
}

#define _DLS(ResID) GetDgnStrValue(CString(#ResID))
CString CRptPlateGirder_EC4::GetDgnStrValue(CString strResID)
{
	// Unicode -> Multibyte 변환시 system(OS) ANSI를 따라서 OS 언어에 따라 해당 국가 언어가 아닌 경우 깨지는 문제가 있음.
	// 그래서 일단 막음. 추후 Gen/Civil이 Unicode 변경되면 활용 가능
	return _T("");
#if UNICODE
	CString  strOut;
	(*m_pGetStrDgnLang)(strResID, strOut);
#else
	CStringW strResIDw = CT2W(strResID);
	CStringW strValue;
	(*m_pGetStrDgnLang)(strResIDw, strValue);
	CString  strOut = CW2A(strValue);
#endif
	return strOut;
}

BOOL CRptPlateGirder_EC4::Get_RptElemKey(ArrElemPairKey& aRptElemKey, ArrElemPairKey& aRptGenElemKey)
{
	aRptElemKey.RemoveAll();
	aRptGenElemKey.RemoveAll();

	int i = 0;
	CArray<T_CGRE_K, T_CGRE_K> aPrintElemKey;
	m_pDoc->m_pAttrCtrl2->GetCgreKeyList(aPrintElemKey);

	CArray<T_CGRV_K, T_CGRV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl2->GetCgrvKeyList(aPrintVBeamKey);

	ArrElemPairKey aRptElemPairK;
	CDBLib::ConvertToElemPairKeyList(aPrintElemKey, aPrintVBeamKey, aRptElemPairK);

	int iPrintElemSize = aRptElemPairK.GetSize();
	CString strErrMessage = _T("");
	if (iPrintElemSize > 0)
	{
		CArray<T_CGDP_K, T_CGDP_K> aPgdElemKey;  aPgdElemKey.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetCgdpKeyList(aPgdElemKey);
		
		CArray<T_CGDV_K, T_CGDV_K> aPgdVBeamKey;
		m_pDoc->m_pAttrCtrl->GetCgdvKeyList(aPgdVBeamKey);

		ArrElemPairKey aPgdElemPairK;
		CDBLib::ConvertToElemPairKeyList(aPgdElemKey, aPgdVBeamKey, aPgdElemPairK);

		int iCheckElemSize = aPgdElemPairK.GetSize();
		if (iCheckElemSize <= 0)
		{
			strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("출력할 요소의 설계정보가 없습니다.")
			AfxMessageBox(strErrMessage);
			return FALSE;
		}
		std::map<ElemPairK, ElemPairK> mapCheckElemKey;		
		for (i = 0; i < iCheckElemSize; i++)
		{
			auto ElemK = aPgdElemPairK[i];
			mapCheckElemKey.insert({ ElemK, ElemK });			
		}
		//
		BOOL bIsExistPrintElem = FALSE;
		for (i = 0; i < iPrintElemSize; i++)
		{
			auto ElemK = aRptElemPairK[i];
			const auto& itChk = mapCheckElemKey.find(ElemK);
			if (itChk != mapCheckElemKey.end())
			{
				if (Get_CheckResultAndPrintElemPositionType(ElemK))
				{
					bIsExistPrintElem = TRUE;

					T_SECT_D SectD;   SectD.Initialize();
					m_pDoc->m_pPostCtrl->GetSectPostDgn(ElemK, SectD);
					//legacy yes 인 상태라서 이전처럼 계산서 출력
					if ( SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_G || CDgnCodeCtrl::IsIDesignModule(m_CpgdD.iDgnCode) == false )
					{
						aRptGenElemKey.Add(ElemK);
					}
					else
					{
						aRptElemKey.Add(ElemK);
					}
				}
			}
		}
		if (!bIsExistPrintElem)
		{
			strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("출력할 요소의 설계정보가 없습니다.")
			AfxMessageBox(strErrMessage);
			return FALSE;
		}
	}
	else
	{
		strErrMessage = _LS(IDS_DGN_PSC_RATING_NON_MEMBER); // _T("출력할 요소가 없습니다.")
		AfxMessageBox(strErrMessage);
		return FALSE;
	}

	return TRUE;
}

BOOL CRptPlateGirder_EC4::Get_CheckResultAndPrintElemPositionType(ElemPairK ElemK)
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
		T_CGRV_D CgrvD; CgrvD.Initialize();		
		if (!m_pDoc->m_pAttrCtrl2->GetCgrv(ElemK.first, CgrvD)) return FALSE;
		CgreD = CgrvD;

		T_CGDV_D CgdvD; CgdvD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetCgdv(ElemK.first, CgdvD)) return FALSE;
		CgdpD = CgdvD;
	}
	else ASSERT(0);

	if (CgreD.iDgnPosi == 0) // (0=I단, 1=J단, 2=양쪽모두)
	{
		if (CgdpD.iDgnPosi == 1) return FALSE;
	}
	else if (CgreD.iDgnPosi == 1)
	{
		if (CgdpD.iDgnPosi == 0) return FALSE;
	}

	return TRUE;
}

void CRptPlateGirder_EC4::SetDetailSheetPage(CMSExcel* pXL, CString strNameBefore, CString strNameCurrent)
{
	if (!pXL->IsExistSheetName(strNameCurrent))
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

void CRptPlateGirder_EC4::PrintReport(CString strPath, int iPrintOpt, BOOL bSaveImage, int nLang)
{
	ArrElemPairKey aRptElemKey;
	if (!Get_RptElemKey(aRptElemKey, m_arDetlElem)) return;

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

	// Set DgnLanguage
	switch (nLang)
	{
	case 0:
		m_strBaseBook = _T("plate_girder_EC4_base.dll");
		//(*m_pSetStrLanguage)(CStringW(_T("Eng")));
		break;
	case 1:
		m_strBaseBook = _T("plate_girder_EC4_base_CZ.dll");
		//(*m_pSetStrLanguage)(CStringW(_T("Cze")));
		break;
	case 2:
		m_strBaseBook = _T("plate_girder_EC4_base_PO.dll");
		//(*m_pSetStrLanguage)(CStringW(_T("Pol")));
		break;
	default:
		m_strBaseBook = _T("plate_girder_EC4_base.dll");
		//(*m_pSetStrLanguage)(CStringW(_T("Eng")));
		break;
	}

	// Set Picture Path.
	CString strDataPath = m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strDataPath);
	CString strPathAndName = FileCtrl.GetFilePathAndNameWithoutExtension() + _T(".") + CMSOfficeMgr::GetExtensionNameExcel();

	// Get Exist Excel Process IDs.
	CArray<unsigned int, unsigned int> arProcID;

	GetExistedExcelProcessID(arProcID);

	m_pDataCtrl->MakePlateGirderDesign4Report();
	m_pDataCtrl->Get_PlateGirderPointer()->Set_PrintMode(TRUE);

	int nOutType;
	BOOL bRunMS, bRowMark, bColumnMark;
	CProduct::GetTestOutType(nOutType, bRunMS, bRowMark, bColumnMark);
	CMSExcel* pXL = new CMSExcel(nOutType, bRunMS, bRowMark, bColumnMark);
	pXL->Open(m_strBasePath + m_strBaseBook);

	CString strRptFile = _T("");
	// Apply Print Option.
	if (iPrintOpt == 0 || iPrintOpt == 2)
	{
		pXL->AddBook();
		strRptFile = strPathAndName;
	}
	else
	{
		pXL->Open(strPath);
		pXL->SetActiveBook(pXL->GetBookName());
		strRptFile = strPath;
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

	COleMessageFilter *pFilter = AfxOleGetMessageFilter();
	pFilter->SetMessagePendingDelay();
	pXL->SaveAs(pXL->GetBookName(), strRptFile);
	pFilter->SetMessagePendingDelay();

	int iStRow = 1;
	//T_STED_D StedD;
	//T_STED_BASE StedBase;
	CString strElemName = _T("");

	BOOL bStopCalc = FALSE;
	int iChkSect = 1;  // 검토 부재의 Index.
	int iDetlElem = m_arDetlElem.GetSize();
	CString strSheetBefore = _T("");

	int i = 0, j = 0, k = 0;

	BOOL bCheckStrength[6];
	for (i = 0; i < 6; i++) bCheckStrength[i] = m_CpgdD.bStrength[i];
	BOOL bCheckService[2];
	for (i = 0; i < 2; i++) bCheckService[i] = m_CpgdD.bService[i];

	T_PGCH_D PgchD; PgchD.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCpgCheckItem(PgchD)) PgchD.Initialize();

	for (i = 0; i < iDetlElem; i++)
	{
		//Show Progress Dlg.
		if (ProDlg.Update_ProgressDlg(i + 1, iDetlElem + 1)) { bStopCalc = TRUE;	break; }

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

		T_PRVS_D PrvsD;
		T_PLTB_D PltbD;
		T_PRTF_D PrtfD;
		T_PGLS_D PglsD;
		T_PGRF_D PgrfD;
		T_PGBS_D PgbsD;
		T_PLSS_D PlssD;

		if (bCheckStrength[1]) { if (m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPrvsResultData(ElemK, PrvsD)) {} }
		if (bCheckStrength[2]) { if (m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPltbResultData(ElemK, PltbD)) {} }
		if (bCheckStrength[3]) { if (m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPrtfResultData(ElemK, PrtfD)) {} }
		if (bCheckStrength[4]) { if (m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPglsResultData(ElemK, PglsD)) {} }
		if (bCheckStrength[5]) { if (m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPgrfResultData(ElemK, PgrfD)) {} }
		if (bCheckService[0]) { if (m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPgbsResultData(ElemK, PgbsD)) {} }
		if (bCheckService[1]) { if (m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPlssResultData(ElemK, PlssD)) {} }

		for (j = 0; j < 2; j++) // 0=I, 1=J
		{
			BOOL bPosiI = j == 0 ? TRUE : FALSE;
			if (!m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, j + 1)) continue;
			if (j == 0 && CgreD.iDgnPosi == 1) continue;
			if (j == 1 && CgreD.iDgnPosi == 0) continue;

			m_pDataCtrl->Get_PlateGirderPointer()->SetCheckNode(j);
			strElemName.Format(_T("Elem_%d_%s"), ElemK, (j == 0) ? _T("i") : _T("j"));
			m_strDetlSheet2 = (m_bTestMode) ? _T("Detail") : strElemName;
			if (pXL->IsExistSheetName(m_strDetlSheet2) && iPrintOpt == 1)
			{
				m_strDetlSheet2 = pXL->GetMaximumSheetName(m_strDetlSheet2) + _T("_New");
			}
			m_arSheetName.Add(m_strDetlSheet2);
			strSheetBefore = (m_arSheetName.GetSize() > 1) ? m_arSheetName.GetAt(m_arSheetName.GetSize() - 2) : m_arSheetName.GetAt(m_arSheetName.GetSize() - 1);

			if (!m_bTestMode) iStRow = 0;
			SetDetailSheetPage(pXL, strSheetBefore, m_strDetlSheet2);

			Get_DesignInfo_Section(ElemK, bPosiI);
			PrintDesignInfo(pXL, iStRow, ElemK.first, j + 1);

			int iChpaterIx = 2;
			if (bCheckStrength[0])
			{
				iChkSect = 0;
				for (k = 0; k < 2; k++) // Max/Min, 
				{
					int nPosiNega = k + 1; // Positive           
					T_PGBR_LCOM PgbrRatD; PgbrRatD.Initialize(); // Positive의 I,J를 얻어온다. 
					if (m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPgbrResultData(ElemK, nPosiNega, PgbrRatD))
					{
						T_PGBR_BASE PgbrGd = PgbrRatD.BeforePgbr[j];
						T_PGBR_BASE PgbrBase = PgbrRatD.PgbrBase[j];
						T_PGBR_DETAIL PgbrDetailGd, PgbrDetail;
						if (PgbrBase.bChk && m_pDataCtrl->Get_PlateGirderPointer()->Get_BendingResistanceDetailResult4PlateGirder(ElemK, bPosiI, PgbrGd, PgbrBase, PgbrDetail))
						{
							if (iChkSect == 0)
								m_iBendingChapterIx = iChpaterIx++;
							PrintBendingResistance(pXL, iStRow, ++iChkSect, nPosiNega, PgbrGd, PgbrBase, PgbrDetail);
						}
					}
				}
			}

			if (bCheckStrength[1]) // Shear Resistance
			{
				iChkSect = 0;
				T_PRVS_BASE PrvsGd = PrvsD.BeforePrvs[j];
				T_PRVS_BASE PrvsBase = PrvsD.PrvsBase[j];
				T_PRVS_DETAIL PrvsDetailGd, PrvsDetail;
				if (PrvsBase.bChk && m_pDataCtrl->Get_PlateGirderPointer()->Get_VerticalShearResistanceDetailResult4PlateGirder(ElemK, bPosiI, PrvsGd, PrvsBase, PrvsDetail))
				{
					if (iChkSect == 0)
						m_iShearChapterIx = iChpaterIx++;
					PrintShearResistance(pXL, iStRow, ++iChkSect, k, PrvsGd, PrvsBase, PrvsDetail);
				}
			}
			if (bCheckStrength[2]) // Lateral Torsional Buckling
			{
				iChkSect = 0;
				T_PLTB_BASE PltbGd = PltbD.BeforePltb[j];
				T_PLTB_BASE PltbBase = PltbD.PltbBase[j];
				T_PLTB_DETAIL PltbDetailGd, PltbDetail;
				if (PltbBase.bChk && m_pDataCtrl->Get_PlateGirderPointer()->Get_LateralTorsionalBucklingResistanceDetailResult4PlateGirder(ElemK, bPosiI, PltbGd, PltbBase, PltbDetail))
				{
					if (iChkSect == 0)
						m_iLTBChapterIx = iChpaterIx++;
					PrintLateralTorsionalBucklingResistance(pXL, iStRow, ++iChkSect, k, PltbGd, PltbBase, PltbDetail);
				}
			}
			if (bCheckStrength[3]) // Transverse Force
			{
				iChkSect = 0;
				T_PRTF_BASE PrtfBase = PrtfD.PrtfBase[j];
				T_PRTF_DETAIL PrtfDetail;
				if (PrtfBase.bChk && m_pDataCtrl->Get_PlateGirderPointer()->Get_TransverseForceResistanceDetailResult4PlateGirder(ElemK, bPosiI, PrtfBase, PrtfDetail))
				{
					if (iChkSect == 0)
						m_iTransverseForceChapterIx = iChpaterIx++;
					PrintTransverseForceResistance(pXL, iStRow, ++iChkSect, k, PrtfBase, PrtfDetail);
				}
			}
			if (bCheckStrength[4]) // Longitudinal Shear
			{
				iChkSect = 0;
				T_PGLS_BASE PglsGd = PglsD.BeforePgls[j];
				T_PGLS_BASE PglsBase = PglsD.PglsBase[j];
				T_PGLS_DETAIL PglsDetailGd, PglsDetail;
				if (PglsBase.bChk && m_pDataCtrl->Get_PlateGirderPointer()->Get_LongitudinalShearResistanceDetailResult4PlateGirder(ElemK, bPosiI, PglsGd, PglsBase, PglsDetail))
				{
					if (iChkSect == 0)
						m_iLongitudinalChapterShearIx = iChpaterIx++;
					PrintLongitudinalShearResistance(pXL, iStRow, ++iChkSect, k, PglsGd, PglsBase, PglsDetail);
				}
			}
			if (bCheckStrength[5]) // Fatigue
			{
				iChkSect = 0;
				T_PGRF_BASE PgrfBase = PgrfD.PgrfBase[j];
				T_PGRF_DETAIL PgrfDetail;
				if (PgrfBase.bChk && m_pDataCtrl->Get_PlateGirderPointer()->Get_FatigueResistanceDetailResult4PlateGirder(ElemK, bPosiI, PgrfBase, PgrfDetail))
				{
					if (iChkSect == 0)
						m_iFatigueChapterIx = iChpaterIx++;
					PrintFatigueResistance(pXL, iStRow, ++iChkSect, k, PgrfBase, PgrfDetail);
				}
			}
			if (bCheckService[0])
			{
				iChkSect = 0;
				T_PGBS_BASE PgbsBase = PgbsD.PgbsBase[j];
				T_PGBS_DETAIL PgbsDetail;
				if (PgbsBase.bChk && m_pDataCtrl->Get_PlateGirderPointer()->Get_BeamStressDetailResult4PlateGirder(ElemK, bPosiI, PgbsBase, PgbsDetail))
				{
					if (iChkSect == 0)
						m_iStressLimitChapterIx = iChpaterIx++;
					PrintStressLimit(pXL, iStRow, ++iChkSect, k, PgbsBase, PgbsDetail);
				}
			}
			if (bCheckService[1])
			{
				iChkSect = 0;
				T_PLSS_BASE PlssGd = PlssD.BeforePlss[j];
				T_PLSS_BASE PlssBase = PlssD.PlssBase[j];
				T_PLSS_DETAIL PlssDetailGd, PlssDetail;
				if (PlssBase.bChk && m_pDataCtrl->Get_PlateGirderPointer()->Get_LongitudinalShearResistanceServiceDetailResult4PlateGirder(ElemK, bPosiI, PlssGd, PlssBase, PlssDetail))
				{
					if (iChkSect == 0)
						m_iLongShearSLSChapterIx = iChpaterIx++;
					PrintLongitudinalShear_SLS(pXL, iStRow, ++iChkSect, k, PlssGd, PlssBase, PlssDetail);
				}
			}

			pFilter->SetMessagePendingDelay(5000);
			pXL->Save(pXL->GetBookName());
			pFilter->SetMessagePendingDelay(10000);
		}
	}

	if (ProDlg.Update_ProgressDlg(iDetlElem + 1, iDetlElem + 1)) { bStopCalc = TRUE; }
	/*
	// Set HyperLinks.
	pXL->SetActiveSheet(m_strSummSheet);
	for(i=0; i<iSectCount; i++)
	{
		pXL->SetHyperLink(arAnchorCells.GetAt(i), _T("'") + arDetailSheet.GetAt(i) + _T("'"), arLinkedCells.GetAt(i));
	}
	*/
	// Delete Default Sheet and Save.
	if (m_arSheetName.GetSize() > 0) pXL->DeleteDefaultSheet();
	// Set Active Sheet For Summary Sheet.      
	//pXL->SetSheetSelection(m_strSummSheet);

	pFilter->SetMessagePendingDelay(5000);
	if (CLockMgr::Get_IsRegressionTest())
	{
		pXL->SaveAs(pXL->GetBookName(), strPath);
	}
	else
		pXL->Save(pXL->GetBookName());

	pFilter->SetMessagePendingDelay(10000);

	pXL->Quit();
	delete pXL;

	// Set Default
	//if(nLang==1 || nLang==2)
	//    (*m_pSetStrLanguage)(CStringW(_T("Eng")));

	// Kill Excel Process IDs producted this time.
	KillExcelProcess(arProcID);

	ProDlg.Delete_ProgressDlg();
		
	if (!CLockMgr::Get_IsRegressionTest())
	{
		ShellExecute(NULL, _T("open"), strRptFile, NULL, NULL, SW_SHOWNORMAL);
	}

	// ('06.05.12). For Regression test : After generating excel file, pop up AfxMessageBox.
	if (m_bTestMode)
	{
		AfxMessageBox(_T("Finish generating excel file."), MB_OK);
	}
}

void CRptPlateGirder_EC4::PrintTitle_Sub_Title_Shear_ResistanceChk(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_Resistance_Chk"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Shear_Resistance_Chk));
	iStRow += iRowCnt;
}

void CRptPlateGirder_EC4::PrintTitle_Sub_Title_Shear_Web(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_Web"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Shear_Web));
	iStRow += iRowCnt;
}

void CRptPlateGirder_EC4::PrintTitle_Sub_Title_Shear_LeftWeb(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_Left_Web"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Shear_Left_Web));
	iStRow += iRowCnt;
}

void CRptPlateGirder_EC4::PrintTitle_Sub_Title_Shear_Right_Web(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_Right_Web"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Shear_Right_Web));
	iStRow += iRowCnt;
}

void CRptPlateGirder_EC4::PrintTitle_Sub_Title_Stress_Limitation_Chk(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Title_of_Stress_Limit"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Title_of_Stress_Limit));
	pXL->SetValue(iStRow, 1, m_iStressLimitChapterIx);

	iStRow += iRowCnt;
}


void CRptPlateGirder_EC4::PrintDesignInfo(CMSExcel* pXL, int& iStRow, int iElemNo, int nPosi)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strChapter = _T("");
	CString strBaseSheet = _T("Bending_Resistance");
	int iChapterIx = 1;
	int iSubChapterIx = 1;

	Print_CheckElemPosition(pXL, iStRow, strBaseSheet, iElemNo, nPosi);
	// Design Condition
	strChapter.Format(_T("%d"), iChapterIx);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Title_of_DesignCondition"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, strChapter);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Title_of_DesignCondition));
	iStRow += iRowCnt;
	strChapter.Format(_T("%d.%d"), iChapterIx, iSubChapterIx);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_DgnCdn_parameters"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 1, strChapter);
	SetMultiLang(pXL, iStRow, 3, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_DgnCdn_parameters));
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_param_partial_factors"), iRowCnt, iColCnt); // - Partial factors
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_param_partial_factors));
	SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_param_partial_factors_Gmma_c), TRUE);
	SetMultiLang(pXL, iStRow + 2, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_param_partial_factors_Gmma_s), TRUE);
	SetMultiLang(pXL, iStRow + 3, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_param_partial_factors_Gmma_m0), TRUE);
	SetMultiLang(pXL, iStRow + 4, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_param_partial_factors_Gmma_m1), TRUE);
	SetMultiLang(pXL, iStRow + 1, 13, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_param_partial_factors_Gmma_v), TRUE);
	SetMultiLang(pXL, iStRow + 2, 13, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_param_partial_factors_Gmma_ef), TRUE);
	SetMultiLang(pXL, iStRow + 3, 13, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_param_partial_factors_Gmma_mf), TRUE);
	SetMultiLang(pXL, iStRow + 4, 13, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_param_partial_factors_Gmma_mfs), TRUE);
	pXL->SetValue(iStRow + 1, 9, m_CpgdD.dGamma_C);
	pXL->SetValue(iStRow + 2, 9, m_CpgdD.dGamma_S);
	pXL->SetValue(iStRow + 3, 9, m_CpgdD.dGamma_M0);
	pXL->SetValue(iStRow + 4, 9, m_CpgdD.dGamma_M1);
	pXL->SetValue(iStRow + 1, 27, m_CpgdD.dGamma_V);
	pXL->SetValue(iStRow + 2, 27, m_CpgdD.dGamma_Ff);
	pXL->SetValue(iStRow + 3, 27, m_CpgdD.dGamma_Mf);
	pXL->SetValue(iStRow + 4, 27, m_CpgdD.dGamma_Mfs);

	iStRow += iRowCnt + 1;

	// Material
	iSubChapterIx = 2;
	strChapter.Format(_T("%d.%d"), iChapterIx, iSubChapterIx);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_DgnCdn_MatInfo"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 3, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_DgnCdn_MatInfo));
	pXL->SetValue(iStRow, 1, strChapter);
	iStRow += iRowCnt;
	PrintDesignInfo_Material(pXL, iStRow, strBaseSheet);

	// Section Information  
	iSubChapterIx = 3;
	strChapter.Format(_T("%d.%d"), iChapterIx, iSubChapterIx);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_DgnCdn_SectInfo"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 3, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_DgnCdn_SectInfo));
	pXL->SetValue(iStRow, 1, strChapter);
	iStRow += iRowCnt;
	PrintDesignInfo_Section(pXL, iStRow, strBaseSheet);

	iStRow += 1;
}

void CRptPlateGirder_EC4::PrintDesignInfo_Section(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	// 1. Section Size
	if (m_nSectType != D_SECT_TYPE_COMPO_G)
	{
		PrintDesignInfo_SectionSize(pXL, iStRow, strBaseSheet);
	}
	else if (m_nSectType == D_SECT_TYPE_COMPO_G)
	{
		// Do Nothing
	}
	else ASSERT(0);

	// 2. Section Stiffness
	PrintDesignInfo_SectionStiffness(pXL, iStRow, strBaseSheet);

}

void CRptPlateGirder_EC4::PrintDesignInfo_SectionSize(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	if (m_nSectType == D_SECT_TYPE_COMPO_I)
	{
		PrintDesignInfo_SectionImage_Compo_I(pXL, iStRow, strBaseSheet);
		PrintDesignInfo_SectionSize_Compo_I(pXL, iStRow, strBaseSheet);
	}
	else if (m_nSectType == D_SECT_TYPE_COMPO_B)
	{
		PrintDesignInfo_SectionImage_Compo_Box(pXL, iStRow, strBaseSheet);
		PrintDesignInfo_SectionSize_Compo_Box(pXL, iStRow, strBaseSheet);
	}
	else if (m_nSectType == D_SECT_TYPE_COMPO_TUB)
	{
		PrintDesignInfo_SectionImage_Compo_Tub(pXL, iStRow, strBaseSheet);
		PrintDesignInfo_SectionSize_Compo_Tub(pXL, iStRow, strBaseSheet);
	}
	else
	{
		ASSERT(0);
	}
}

void CRptPlateGirder_EC4::PrintDesignInfo_SectionImage_Compo_I(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_H_Shape_Image"), iRowCnt, iColCnt);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintDesignInfo_SectionImage_Compo_Box(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_Box_Shape_Image"), iRowCnt, iColCnt);

	iStRow += iRowCnt + 1;
}
void CRptPlateGirder_EC4::PrintDesignInfo_SectionImage_Compo_Tub(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_Tub_Shape_Image"), iRowCnt, iColCnt);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintDesignInfo_SectionSize_Compo_I(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	double dh = m_dSize[0]; // C, web의 높이.
	double dtw = m_dSize[1];
	double dB1 = m_dSize[2];
	double dtf1 = m_dSize[3];
	double dB2 = m_dSize[4];
	double dtf2 = m_dSize[5];
	if (dB2 < m_dZero) { dB2 = dB1; }
	if (dtf2 < m_dZero) { dtf2 = dtf1; }

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_H_Shape_Size"), iRowCnt, iColCnt);

	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section));
	SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Slab));
	SetMultiLang(pXL, iStRow + 3, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Girder));

	pXL->SetValue(iStRow + 2, 4, m_dBc);
	pXL->SetValue(iStRow + 2, 13, m_dtc);
	pXL->SetValue(iStRow + 2, 22, m_dHh);
	pXL->SetValue(iStRow + 4, 4, dh);
	pXL->SetValue(iStRow + 4, 13, dB1);
	pXL->SetValue(iStRow + 4, 22, dB2);
	pXL->SetValue(iStRow + 5, 4, dtw);
	pXL->SetValue(iStRow + 5, 13, dtf1);
	pXL->SetValue(iStRow + 5, 22, dtf2);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintDesignInfo_SectionSize_Compo_Box(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	double dh = m_dSize[0]; // C, web의 높이.
	double dtw = m_dSize[1];
	double dB1 = m_dSize[2];
	double dtf1 = m_dSize[4];
	double dB2 = m_dSize[5];
	double dtf2 = m_dSize[7];
	double dBf1 = m_dSize[3];
	double dBf2 = m_dSize[6];
	if (dB2 < m_dZero) { dB2 = dB1; }
	if (dtf2 < m_dZero) { dtf2 = dtf1; }

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_Box_Shape_Size"), iRowCnt, iColCnt);

	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section));
	SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Slab));
	SetMultiLang(pXL, iStRow + 3, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Girder));

	pXL->SetValue(iStRow + 2, 4, m_dBc);
	pXL->SetValue(iStRow + 2, 13, m_dtc);
	pXL->SetValue(iStRow + 2, 22, m_dHh);
	pXL->SetValue(iStRow + 4, 4, dh);
	pXL->SetValue(iStRow + 4, 13, dB1);
	pXL->SetValue(iStRow + 4, 22, dB2);
	pXL->SetValue(iStRow + 5, 4, dtw);
	pXL->SetValue(iStRow + 5, 13, dtf1);
	pXL->SetValue(iStRow + 5, 22, dtf2);
	pXL->SetValue(iStRow + 6, 13, dBf1);
	pXL->SetValue(iStRow + 6, 22, dBf2);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintDesignInfo_SectionSize_Compo_Tub(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	double dh = m_dSize[0]; // C, web의 높이.
	double dtw = m_dSize[1];
	double dB1 = m_dSize[2];
	double dtf1 = m_dSize[4];
	double dB2 = m_dSize[5];
	double dtf2 = m_dSize[7];
	double dBf1 = m_dSize[3];
	double dBf2 = m_dSize[6];
	double dBf3 = m_dSize[8];
	if (dB2 < m_dZero) { dB2 = dB1; }
	if (dtf2 < m_dZero) { dtf2 = dtf1; }

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_Tub_Shape_Size"), iRowCnt, iColCnt);

	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section));
	SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Slab));
	SetMultiLang(pXL, iStRow + 3, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Girder));

	pXL->SetValue(iStRow + 2, 4, m_dBc);
	pXL->SetValue(iStRow + 2, 13, m_dtc);
	pXL->SetValue(iStRow + 2, 22, m_dHh);
	pXL->SetValue(iStRow + 4, 4, dh);
	pXL->SetValue(iStRow + 4, 13, dB1);
	pXL->SetValue(iStRow + 4, 22, dB2);
	pXL->SetValue(iStRow + 5, 4, dtw);
	pXL->SetValue(iStRow + 5, 13, dtf1);
	pXL->SetValue(iStRow + 5, 22, dtf2);
	pXL->SetValue(iStRow + 6, 4, dBf1);
	pXL->SetValue(iStRow + 6, 13, dBf2);
	pXL->SetValue(iStRow + 6, 22, dBf3);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintDesignInfo_SectionStiffness(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Section_Stiffness"), iRowCnt, iColCnt);

	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Stiffness));
	SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Stiffness_Before));
	SetMultiLang(pXL, iStRow + 1, 16, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Stiffness_After));
	SetMultiLang(pXL, iStRow + 8, 16, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Stiffness_Crack));

	pXL->SetValue(iStRow + 2, 4, m_dArea_g);
	pXL->SetValue(iStRow + 3, 4, m_dIyy_g);
	pXL->SetValue(iStRow + 4, 4, m_dIzz_g);
	pXL->SetValue(iStRow + 5, 4, m_dYbar_g);
	pXL->SetValue(iStRow + 6, 4, m_dZbar_g);
	//
	pXL->SetValue(iStRow + 2, 18, m_dArea);
	pXL->SetValue(iStRow + 3, 18, m_dIyy);
	pXL->SetValue(iStRow + 4, 18, m_dIzz);
	pXL->SetValue(iStRow + 5, 18, m_dYbar);
	pXL->SetValue(iStRow + 6, 18, m_dZbar);
	//
	pXL->SetValue(iStRow + 9, 18, m_dArea_cr);
	pXL->SetValue(iStRow + 10, 18, m_dIyy_cr);
	pXL->SetValue(iStRow + 11, 18, m_dIzz_cr);
	pXL->SetValue(iStRow + 12, 18, m_dYbar_cr);
	pXL->SetValue(iStRow + 13, 18, m_dZbar_cr);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintDesignInfo_Material(CMSExcel* pXL, int& iStRow, CString strBaseSheet)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("DgnCdn_Material_Data"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Material_Data_Stl));
	SetMultiLang(pXL, iStRow + 3, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Material_Data_Conc));
	SetMultiLang(pXL, iStRow + 6, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Material_Data_Rbar));
	pXL->SetValue(iStRow + 1, 5, m_dfy);
	pXL->SetValue(iStRow + 4, 5, m_dfck);
	pXL->SetValue(iStRow + 7, 5, m_dfsk);
	pXL->SetValue(iStRow + 1, 15, m_dEs);
	pXL->SetValue(iStRow + 4, 15, m_dEcm);
	pXL->SetValue(iStRow + 7, 15, m_dErs);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintBendingResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int nPosiNega, T_PGBR_BASE& PgbrGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strBaseSheet = _T("Bending_Resistance");
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Title_of_BendingResistance"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Title_of_BendingResistance));
	pXL->SetValue(iStRow, 1, m_iBendingChapterIx);
	iStRow += iRowCnt;

	CString strSubChapter = _T("");
	// Positive Moment
	if (nPosiNega == 1) // Positive
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Bending_Positive"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 3, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Bending_Positive));
	}
	else
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Bending_Negative"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 3, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Bending_Negative));
	}
	strSubChapter.Format(_T("%d.%d"), m_iBendingChapterIx, iChapIndex);
	pXL->SetValue(iStRow, 1, strSubChapter);
	iStRow += iRowCnt;

	CString strLcomName = GetLoadCombinationName(PgbrBase.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Design_Force"), iRowCnt, iColCnt);

	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Design_Force));
	SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_LCOM));

	pXL->SetValue(iStRow + 1, 10, strLcomName);
	pXL->SetValue(iStRow + 2, 5, PgbrGd.dFx*m_dFactorForce);
	pXL->SetValue(iStRow + 3, 5, PgbrBase.dFx*m_dFactorForce);
	pXL->SetValue(iStRow + 4, 5, PgbrBase.dMa_Ed*m_dFactorMoment);
	pXL->SetValue(iStRow + 5, 5, PgbrBase.dMc_Ed*m_dFactorMoment);
	iStRow += iRowCnt + 1;

	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Bending_Stress"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Bending_Stress));
	iStRow += iRowCnt;

	if (m_nSectType == D_SECT_TYPE_COMPO_I || m_nSectType == D_SECT_TYPE_COMPO_B || m_nSectType == D_SECT_TYPE_COMPO_TUB)
	{
		PrintStressInformationDB(pXL, iStRow, strBaseSheet, PgbrDetail.StressInfoHshapeD);
	}
	else if (m_nSectType == D_SECT_TYPE_COMPO_G)
	{
		PrintStressInformation(pXL, iStRow, strBaseSheet, PgbrDetail.GLineDgn, PgbrDetail.StressInfo4Class);
	}
	else ASSERT(0);

	// Classification of sections
	PrintClassificationofSection(pXL, iStRow, PgbrBase.iTopClass, PgbrBase.iWebClass, PgbrBase.iBotClass, PgbrBase.iSectClass);

	int iClassSect = PgbrBase.iSectClass;
	if (iClassSect == 1 || iClassSect == 2) // Plastic
	{
		if (PgbrBase.bEquiClass2)
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Plastic_Moment_Eff_NAxis"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Plastic_resistance_moment), TRUE);
			SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_NAxis_PNA));
			SetMultiLang(pXL, iStRow + 2, 2, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_Eff_NAxis));
			pXL->SetValue(iStRow + 1, 7, PgbrBase.dPlasticNA);
			pXL->SetValue(iStRow + 2, 9, PgbrBase.dPlasticNAEff);
		}
		else
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Plastic_Moment_NAxis"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Plastic_resistance_moment), TRUE);
			SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_NAxis_PNA));
			pXL->SetValue(iStRow + 1, 7, PgbrBase.dPlasticNA);
		}
		iStRow += iRowCnt + 1;

		if (nPosiNega == 1) // Positive
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Plastic_Moment_Positive_Param"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow + 1, 12, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_UPNA));
			SetMultiLang(pXL, iStRow + 2, 12, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_LPNA));
			SetMultiLang(pXL, iStRow + 7, 3, _DLS(IDS_CVL_RPT_EC_here));
			pXL->SetValue(iStRow + 0, 5, PgbrDetail.dN_slab*m_dFactorForce);
			pXL->SetValue(iStRow + 1, 5, PgbrDetail.dNg_top*m_dFactorForce);
			pXL->SetValue(iStRow + 2, 5, PgbrDetail.dNg_bot*m_dFactorForce);
			pXL->SetValue(iStRow + 4, 5, PgbrDetail.dMpl_Rd_cal*m_dFactorMoment);
			pXL->SetValue(iStRow + 5, 5, PgbrBase.dXpl);
			pXL->SetValue(iStRow + 6, 10, PgbrBase.dBeta*PgbrBase.dMpl_Rd*m_dFactorMoment);
			pXL->SetValue(iStRow + 7, 7, PgbrBase.dBeta);
			iStRow += iRowCnt + 1;

			// if xpl/h>0.4이면 Class 1,2 에 대해서도 Elastic에 대해 검토한다
			if (PgbrBase.dXplOverh > 0.4)
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Elastic_resistance_moment"), iRowCnt, iColCnt);
				SetMultiLang(pXL, iStRow + 2, 12, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Elastic_resistance_moment), TRUE);
				iStRow += iRowCnt;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_Xpl_h_check_MoreThan"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 5, PgbrBase.dXplOverh);
				iStRow += iRowCnt;
				// Modify by GAY. PMS:4216. ('12.05.16). Modify output format for k factor.
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_Positive"), iRowCnt, iColCnt);
				SetMultiLang(pXL, iStRow, 9, _DLS(IDS_CVL_RPT_EC_Plate_Elastic_resistance_k));
				SetMultiLang(pXL, iStRow + 1, 3, _DLS(IDS_CVL_RPT_EC_Plate_Elastic_resistance_commt1));
				pXL->SetValue(iStRow, 4, PgbrBase.dk_min);
				iStRow += iRowCnt;
				//if(PgbrBase.dk_min>=0.0)
				{
					pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_by_k_min"), iRowCnt, iColCnt);
					pXL->SetValue(iStRow + 0, 12, PgbrBase.dMel_Rd*m_dFactorMoment);
					iStRow += iRowCnt;
				}
				iStRow++;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_bytElastic_resistance"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 8, PgbrBase.dMel_Rd*m_dFactorMoment);
				iStRow += iRowCnt + 1;

				if (PgbrBase.dNc < 0.0) // 압축력이 있을때 적용한다. 
				{
					if (fabs(PgbrBase.dNc) < PgbrBase.dNc_el)
					{
						pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_Class1_2_Pos_LT_Ncel"), iRowCnt, iColCnt);
						pXL->SetValue(iStRow, 5, PgbrBase.dNc   *m_dFactorForce);
						pXL->SetValue(iStRow + 1, 5, PgbrBase.dNc_el*m_dFactorForce);
						pXL->SetValue(iStRow + 2, 16, PgbrBase.dM_Rd *m_dFactorMoment);
						iStRow += iRowCnt + 1;
					}
					else if (fabs(PgbrBase.dNc) < PgbrBase.dNc_f)
					{
						pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_Class1_2_Pos_GT_Ncel"), iRowCnt, iColCnt);
						pXL->SetValue(iStRow, 5, PgbrBase.dNc   *m_dFactorForce);
						pXL->SetValue(iStRow + 1, 5, PgbrBase.dNc_el*m_dFactorForce);
						pXL->SetValue(iStRow + 2, 5, PgbrBase.dNc_f *m_dFactorForce);
						pXL->SetValue(iStRow + 3, 16, PgbrBase.dM_Rd *m_dFactorMoment);
						iStRow += iRowCnt + 1;
					}
					else ASSERT(0);
				}
			}
		}
		else if (nPosiNega == 2)
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Plastic_Moment_Negative_Param"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow + 1, 12, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_UPNA));
			SetMultiLang(pXL, iStRow + 2, 12, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_LPNA));
			SetMultiLang(pXL, iStRow + 3, 12, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_UPNA));
			SetMultiLang(pXL, iStRow + 4, 12, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_LPNA));
			pXL->SetValue(iStRow + 0, 5, PgbrDetail.dN_slab*m_dFactorForce);
			pXL->SetValue(iStRow + 1, 5, PgbrDetail.dNr_top*m_dFactorForce);
			pXL->SetValue(iStRow + 2, 5, PgbrDetail.dNr_bot*m_dFactorForce);
			pXL->SetValue(iStRow + 3, 5, PgbrDetail.dNg_top*m_dFactorForce);
			pXL->SetValue(iStRow + 4, 5, PgbrDetail.dNg_bot*m_dFactorForce);
			pXL->SetValue(iStRow + 6, 5, PgbrBase.dMpl_Rd*m_dFactorMoment);
			pXL->SetValue(iStRow + 7, 8, PgbrBase.dM_Rd*m_dFactorMoment);

			iStRow += iRowCnt + 1;
		}
		else ASSERT(0);

	}
	else if (iClassSect == 3) // Elastic
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Sub_Title_Elastic_resistance_moment"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow + 2, 12, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Elastic_resistance_moment), TRUE);
		iStRow += iRowCnt;
		// Modify by GAY. PMS:4216. ('12.05.16). Modify output format for k factor.
		CString strBlockName;
		if (nPosiNega == 1) strBlockName = _T("Elastic_resistance_Positive");// Positive
		else             strBlockName = _T("Elastic_resistance_Negative");
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strBlockName, iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 9, _DLS(IDS_CVL_RPT_EC_Plate_Elastic_resistance_k));
		SetMultiLang(pXL, iStRow + 1, 3, (nPosiNega == 1) ? _DLS(IDS_CVL_RPT_EC_Plate_Elastic_resistance_commt1) : _DLS(IDS_CVL_RPT_EC_Plate_Elastic_resistance_commt2));
		pXL->SetValue(iStRow, 4, PgbrBase.dk_min);

		iStRow += iRowCnt;
		//if(PgbrBase.dk_min>=0.0)
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_by_k_min"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow + 0, 12, PgbrBase.dMel_Rd*m_dFactorMoment);
			iStRow += iRowCnt;
		}
		iStRow++;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_bytElastic_resistance"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 8, PgbrBase.dMel_Rd*m_dFactorMoment);
		iStRow += iRowCnt + 1;

	}
	else if (iClassSect == 4) // Effective
	{
		pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Effective_section"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Effective_section));
		iStRow += iRowCnt;

		pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Effective_section_Fx"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Effective_section_Fx), TRUE);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Effective_Section_Stiffness"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Stiffness_Before));
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Stiffness_After));
		pXL->SetValue(iStRow + 1, 4, PgbrDetail.EffGirderFx.Area);
		pXL->SetValue(iStRow + 2, 4, PgbrDetail.EffGirderFx.Ryy);
		pXL->SetValue(iStRow + 3, 4, PgbrDetail.EffGirderFx.Rzz);
		pXL->SetValue(iStRow + 4, 4, PgbrDetail.EffGirderFx.Cym);
		pXL->SetValue(iStRow + 5, 4, PgbrDetail.EffGirderFx.Czm);
		//
		pXL->SetValue(iStRow + 1, 18, PgbrDetail.EffCompositeFx.Area);
		pXL->SetValue(iStRow + 2, 18, PgbrDetail.EffCompositeFx.Ryy);
		pXL->SetValue(iStRow + 3, 18, PgbrDetail.EffCompositeFx.Rzz);
		pXL->SetValue(iStRow + 4, 18, PgbrDetail.EffCompositeFx.Cym);
		pXL->SetValue(iStRow + 5, 18, PgbrDetail.EffCompositeFx.Czm);
		iStRow += iRowCnt + 1;

		pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Effective_section_My"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Effective_section_My), TRUE);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Effective_Section_Stiffness"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Stiffness_Before));
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DgnCdn_Section_Stiffness_After));
		pXL->SetValue(iStRow + 1, 4, PgbrDetail.EffGirder.Area);
		pXL->SetValue(iStRow + 2, 4, PgbrDetail.EffGirder.Ryy);
		pXL->SetValue(iStRow + 3, 4, PgbrDetail.EffGirder.Rzz);
		pXL->SetValue(iStRow + 4, 4, PgbrDetail.EffGirder.Cym);
		pXL->SetValue(iStRow + 5, 4, PgbrDetail.EffGirder.Czm);
		//
		pXL->SetValue(iStRow + 1, 18, PgbrDetail.EffComposite.Area);
		pXL->SetValue(iStRow + 2, 18, PgbrDetail.EffComposite.Ryy);
		pXL->SetValue(iStRow + 3, 18, PgbrDetail.EffComposite.Rzz);
		pXL->SetValue(iStRow + 4, 18, PgbrDetail.EffComposite.Cym);
		pXL->SetValue(iStRow + 5, 18, PgbrDetail.EffComposite.Czm);
		iStRow += iRowCnt + 1;

		pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Added_moment"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Added_moment));
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Added_moment_calc"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 13, PgbrDetail.dDelMa_Ed*m_dFactorMoment);
		pXL->SetValue(iStRow + 1, 13, PgbrDetail.dDelMc_Ed*m_dFactorMoment);
		pXL->SetValue(iStRow + 3, 6, PgbrBase.dSIGa_top);
		pXL->SetValue(iStRow + 5, 6, PgbrBase.dSIGa_bot);
		iStRow += iRowCnt;

		CString strBlockName;
		if (nPosiNega == 1) strBlockName = _T("Elastic_resistance_Positive");// Positive
		else             strBlockName = _T("Elastic_resistance_Negative");
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strBlockName, iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 9, _DLS(IDS_CVL_RPT_EC_Plate_Elastic_resistance_k));
		SetMultiLang(pXL, iStRow + 1, 3, (nPosiNega == 1) ? _DLS(IDS_CVL_RPT_EC_Plate_Elastic_resistance_commt1) : _DLS(IDS_CVL_RPT_EC_Plate_Elastic_resistance_commt2));
		pXL->SetValue(iStRow, 4, PgbrBase.dk_min);
		iStRow += iRowCnt;
		//if(PgbrBase.dk_min>=0.0)
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Elastic_resistance_Class4by_k_min"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow + 0, 17, PgbrBase.dMel_Rd*m_dFactorMoment);
			iStRow += iRowCnt;
		}
		iStRow++;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Moment_resistance_bytElastic_resistance"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 8, PgbrBase.dMel_Rd*m_dFactorMoment);
		iStRow += iRowCnt + 1;
	}
	else ASSERT(0);

	// Verification
	if (PgbrBase.bOK)  pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Verification_OK"), iRowCnt, iColCnt);
	else              pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Verification_NG"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 5, PgbrBase.dM_Rd*m_dFactorMoment);
	double dM_Ed = PgbrGd.dMy + PgbrBase.dMy;
	if (iClassSect == 3 || iClassSect == 4)
	{
		if (PgbrDetail.nKminType == Kmin_Slab || PgbrDetail.nKminType == Kmin_Rbar) dM_Ed = PgbrBase.dMy;
	}

	pXL->SetValue(iStRow, 19, dM_Ed*m_dFactorMoment);
	iStRow += iRowCnt + 1;

}

void CRptPlateGirder_EC4::PrintStressInformationDB(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_STRESS_INFO_I& StrInfoD)
{
	int iRowCnt = 0;
	int iColCnt = 0;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Stress_H_Shape"), iRowCnt, iColCnt);

	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_TF));
	SetMultiLang(pXL, iStRow + 6, 2, _DLS(IDS_CVL_RPT_EC_Plate_BF));
	SetMultiLang(pXL, iStRow + 12, 2, _DLS(IDS_CVL_RPT_EC_Plate_Web));

	T_STRESS_LINE_INFO FlgTL = StrInfoD.FlgTL;
	pXL->SetValue(iStRow + 1, 8, FlgTL.dy1);
	pXL->SetValue(iStRow + 1, 16, FlgTL.dz1);
	pXL->SetValue(iStRow + 1, 24, FlgTL.dStr1);
	pXL->SetValue(iStRow + 2, 8, FlgTL.dy2);
	pXL->SetValue(iStRow + 2, 16, FlgTL.dz2);
	pXL->SetValue(iStRow + 2, 24, FlgTL.dStr2);
	//
	T_STRESS_LINE_INFO FlgTR = StrInfoD.FlgTR;
	pXL->SetValue(iStRow + 3, 8, FlgTR.dy1);
	pXL->SetValue(iStRow + 3, 16, FlgTR.dz1);
	pXL->SetValue(iStRow + 3, 24, FlgTR.dStr1);
	pXL->SetValue(iStRow + 4, 8, FlgTR.dy2);
	pXL->SetValue(iStRow + 4, 16, FlgTR.dz2);
	pXL->SetValue(iStRow + 4, 24, FlgTR.dStr2);
	//
	T_STRESS_LINE_INFO FlgBL = StrInfoD.FlgBL;
	pXL->SetValue(iStRow + 7, 8, FlgBL.dy1);
	pXL->SetValue(iStRow + 7, 16, FlgBL.dz1);
	pXL->SetValue(iStRow + 7, 24, FlgBL.dStr1);
	pXL->SetValue(iStRow + 8, 8, FlgBL.dy2);
	pXL->SetValue(iStRow + 8, 16, FlgBL.dz2);
	pXL->SetValue(iStRow + 8, 24, FlgBL.dStr2);
	//
	T_STRESS_LINE_INFO FlgBR = StrInfoD.FlgBR;
	pXL->SetValue(iStRow + 9, 8, FlgBR.dy1);
	pXL->SetValue(iStRow + 9, 16, FlgBR.dz1);
	pXL->SetValue(iStRow + 9, 24, FlgBR.dStr1);
	pXL->SetValue(iStRow + 10, 8, FlgBR.dy2);
	pXL->SetValue(iStRow + 10, 16, FlgBR.dz2);
	pXL->SetValue(iStRow + 10, 24, FlgBR.dStr2);
	//
	T_STRESS_LINE_INFO Web = StrInfoD.Web;
	pXL->SetValue(iStRow + 13, 8, Web.dy1);
	pXL->SetValue(iStRow + 13, 16, Web.dz1);
	pXL->SetValue(iStRow + 13, 24, Web.dStr1);
	pXL->SetValue(iStRow + 14, 8, Web.dy2);
	pXL->SetValue(iStRow + 14, 16, Web.dz2);
	pXL->SetValue(iStRow + 14, 24, Web.dStr2);
	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintStressInformation(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_GLINE_DGN_D& GLineDgn, T_DLINE_CALC_D& StressInfoD)
{
	//T_GLINE_DGN_D GLineDgn = PgbrDetail.GLineDgn;
	CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&> aTopFlgPartLine;
	CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&> aBotFlgPartLine;
	CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&> aWebPartLine;
	m_pDataCtrl->Get_PlateGirderPointer()->Get_GLineDgnPartData(GLineDgn, aTopFlgPartLine, aBotFlgPartLine, aWebPartLine);

	//T_DLINE_CALC_D StressInfoD = PgbrDetail.StressInfo4Class;
	CArray<T_DLINE_PART_D, T_DLINE_PART_D&> aTopFlangePart;
	CArray<T_DLINE_PART_D, T_DLINE_PART_D&> aBotFlangePart;
	CArray<T_DLINE_PART_D, T_DLINE_PART_D&> aWebPart;
	m_pDataCtrl->Get_PlateGirderPointer()->Get_DLinePartData(StressInfoD, aTopFlangePart, aBotFlangePart, aWebPart);

	// Top
	PrintStressInfo4Class(pXL, iStRow, aTopFlgPartLine, aTopFlangePart);
	// Web
	PrintStressInfo4Class(pXL, iStRow, aWebPartLine, aWebPart);
	// Bottom
	PrintStressInfo4Class(pXL, iStRow, aBotFlgPartLine, aBotFlangePart);

	iStRow++;
}

void CRptPlateGirder_EC4::PrintStressInfo4Class(CMSExcel* pXL, int& iStRow, CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&>& aPartLine, CArray<T_DLINE_PART_D, T_DLINE_PART_D&>& aDgnLinePartD)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	int iPartLineSize = aPartLine.GetSize();
	int iPartSize = aDgnLinePartD.GetSize();
	if (iPartLineSize != iPartSize) { ASSERT(0); return; }

	int i = 0, j = 0;
	CString strBaseSheet = _T("Bending_Resistance"); // Stress 출력은 Bending_Resistance sheet에 있음.

	double dy1 = 0., dz1 = 0., dy2 = 0., dz2 = 0.;
	double dStrEnd1 = 0., dStrEnd2 = 0.;
	for (i = 0; i < iPartSize; i++)
	{
		T_GLINE_PART_DGN GLinePartD = aPartLine[i];
		T_DLINE_PART_D LinePartD = aDgnLinePartD[i];
		int iPartType = LinePartD.nPartType;
		CString strPartType = Get_String4PartShape(iPartType);
		CString strName = GLinePartD.strGroupName;
		CString strPartName;
		strPartName.Format(_T("%s(%s)"), strPartType, strName);
		pXL->SetValue(iStRow, 2, strPartName);
		pXL->SetFont(iStRow, 0, iStRow, 32, 10, 0, m_FontType, FALSE);
		iStRow++;

		int iInternalSize = LinePartD.aInternal.GetSize();
		if (iInternalSize > 0)
		{
			pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Stress_Internal_G_Shape"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_IP));
			iStRow += iRowCnt;
			for (j = 0; j < iInternalSize; j++)
			{
				T_GLINE_DST GLineDst = GLinePartD.aInternal[j];

				T_DGNLINE_LBD DgnLineLBD = LinePartD.aInternal[j];
				T_GLINE_DGN_B aDgnBaseD = DgnLineLBD.aDgnBaseD.GetAt(0);
				dy1 = aDgnBaseD.dy1;
				dz1 = aDgnBaseD.dz1;
				dy2 = aDgnBaseD.dy2;
				dz2 = aDgnBaseD.dz2;

				m_pDataCtrl->Get_PlateGirderPointer()->Get_LineEndStress4LineDgnBase(DgnLineLBD.aDgnBaseD, dStrEnd1, dStrEnd2);
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Stress_G_Shape"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 2, GLineDst.strShape);
				pXL->SetValue(iStRow, 8, dy1);
				pXL->SetValue(iStRow, 16, dz1);
				pXL->SetValue(iStRow, 24, dStrEnd1);
				pXL->SetValue(iStRow + 1, 8, dy2);
				pXL->SetValue(iStRow + 1, 16, dz2);
				pXL->SetValue(iStRow + 1, 24, dStrEnd2);
				iStRow += iRowCnt;
			}
		}

		int iOutstandSize = LinePartD.aOutstand.GetSize();
		if (iOutstandSize > 0)
		{
			pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Stress_Outstand_G_Shape"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_OF));
			iStRow += iRowCnt;
			for (j = 0; j < iOutstandSize; j++)
			{
				T_GLINE_DST GLineDst = GLinePartD.aOutstand[j];

				T_DGNLINE_LBD DgnLineLBD = LinePartD.aOutstand[j];
				T_GLINE_DGN_B aDgnBaseD = DgnLineLBD.aDgnBaseD.GetAt(0);
				dy1 = aDgnBaseD.dy1;
				dz1 = aDgnBaseD.dz1;
				dy2 = aDgnBaseD.dy2;
				dz2 = aDgnBaseD.dz2;

				m_pDataCtrl->Get_PlateGirderPointer()->Get_LineEndStress4LineDgnBase(DgnLineLBD.aDgnBaseD, dStrEnd1, dStrEnd2);
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Bending_Stress_G_Shape"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 2, GLineDst.strShape);
				pXL->SetValue(iStRow, 8, dy1);
				pXL->SetValue(iStRow, 16, dz1);
				pXL->SetValue(iStRow, 24, dStrEnd1);
				pXL->SetValue(iStRow + 1, 8, dy2);
				pXL->SetValue(iStRow + 1, 16, dz2);
				pXL->SetValue(iStRow + 1, 24, dStrEnd2);
				iStRow += iRowCnt;
			}
		}
	}
}

void CRptPlateGirder_EC4::PrintClassificationofSection(CMSExcel* pXL, int& iStRow, int iTopCS, int iWebCS, int iBotCS, int iSectCS)
{
	int iRowCnt = 0;
	int iColCnt = 0;
	// Classification of sections
	CString strBaseSheet = _T("Bending_Resistance");
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Classification_of_sections"), iRowCnt, iColCnt);

	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Classification_of_sections));
	SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_Part));
	SetMultiLang(pXL, iStRow + 1, 7, _DLS(IDS_CVL_RPT_EC_Plate_Class));
	SetMultiLang(pXL, iStRow + 2, 2, _DLS(IDS_CVL_RPT_EC_Plate_TF));
	SetMultiLang(pXL, iStRow + 3, 2, _DLS(IDS_CVL_RPT_EC_Plate_Web));
	SetMultiLang(pXL, iStRow + 4, 2, _DLS(IDS_CVL_RPT_EC_Plate_BF));
	SetMultiLang(pXL, iStRow + 5, 2, _DLS(IDS_CVL_RPT_EC_Plate_Section));

	pXL->SetValue(iStRow + 2, 7, iTopCS);
	pXL->SetValue(iStRow + 3, 7, iWebCS);
	pXL->SetValue(iStRow + 4, 7, iBotCS);
	pXL->SetValue(iStRow + 5, 7, iSectCS);
	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintShearResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("Shear_Resistance");

	// 1. Title
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Title_of_ShearResistance"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Title_of_ShearResistance));
	pXL->SetValue(iStRow, 1, m_iShearChapterIx);
	iStRow += iRowCnt;

	// 2. SubTitle
	CString strSubChapter;

	// 3.Design Force
	PrintShearResistance_ShearDgnForce(pXL, iStRow, PrvsGd, PrvsBase, PrvsDetail);

	// 4. Stress 
	pXL->CopyRange(iStRow, 0, m_strBaseBook, strTitleBaseSheet, _T("Sub_Title_Bending_Stress"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Bending_Stress));
	iStRow += iRowCnt;
	if (m_nSectType == D_SECT_TYPE_COMPO_I || m_nSectType == D_SECT_TYPE_COMPO_B || m_nSectType == D_SECT_TYPE_COMPO_TUB)
	{
		PrintStressInformationDB(pXL, iStRow, strBaseSheet, PrvsDetail.StressInfoHshapeD);
	}
	else if (m_nSectType == D_SECT_TYPE_COMPO_G)
	{
		PrintStressInformation(pXL, iStRow, strBaseSheet, PrvsDetail.GLineDgn, PrvsDetail.StressInfo4Class);
	}
	else ASSERT(0);

	// 5. Classification of sections
	PrintClassificationofSection(pXL, iStRow, PrvsBase.iTopClass, PrvsBase.iWebClass, PrvsBase.iBotClass, PrvsBase.iSectClass);

	// 6. Sub Title
	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_Plastic_resistance"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Shear_Plastic_resistance), TRUE);
	iStRow += iRowCnt;

	// 7. Shear Plastic Moment
	PrintShearResistance_PlasticRestance(pXL, iStRow, PrvsBase, PrvsDetail);

	// 8. ShearBucklingCoeff
	PrintShearResistance_ShearBucklingCoeff(pXL, iStRow, strBaseSheet, PrvsGd, PrvsBase, PrvsDetail);
}

void CRptPlateGirder_EC4::PrintShearResistance_ShearDgnForce(CMSExcel* pXL, int& iStRow, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strBaseSheet = _T("Shear_Resistance");
	CString strLcomName = GetLoadCombinationName(PrvsBase.OrgLcomK);

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_Design_Force"), iRowCnt, iColCnt);

	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_Design_Force));
	SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_LCOM));

	pXL->SetValue(iStRow + 1, 10, strLcomName);
	pXL->SetValue(iStRow + 2, 5, PrvsBase.dFx*m_dFactorForce);
	pXL->SetValue(iStRow + 3, 5, PrvsBase.dMa_Ed*m_dFactorMoment);
	pXL->SetValue(iStRow + 4, 5, PrvsBase.dMy*m_dFactorMoment);
	pXL->SetValue(iStRow + 5, 5, PrvsGd.dFz*m_dFactorForce);
	pXL->SetValue(iStRow + 6, 5, PrvsBase.dFz*m_dFactorForce);
	pXL->SetValue(iStRow + 7, 5, PrvsBase.dV_Ed*m_dFactorForce);

	pXL->SetValue(iStRow + 9, 12, PrvsBase.dM_Ed*m_dFactorMoment);
	pXL->SetValue(iStRow + 10, 5, PrvsDetail.dM_Ed_top*m_dFactorMoment);
	pXL->SetValue(iStRow + 11, 5, PrvsDetail.dM_Ed_bot*m_dFactorMoment);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintShearResistance_Vpl_Rd_Calc(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vpl_Rd_Calc"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow + 2, 5, _DLS(IDS_CVL_RPT_EC_Plate_Shear_Shear_resistance_Vpl_Rd_Calc), TRUE);
	pXL->SetValue(iStRow, 12, PrvsBase.dVpl_Rd*m_dFactorForce);
	pXL->SetValue(iStRow + 1, 5, PrvsBase.dV_Rd*m_dFactorForce);
	pXL->SetValue(iStRow + 2, 12, PrvsBase.dV_Ed*m_dFactorForce);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintShearResistance_Vpl_Rd_ChkRatio(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	if (PrvsBase.dRatio > 1.0)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_V_Ed_Vpl_Rd_morehan_1"), iRowCnt, iColCnt);
	}
	else
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_V_Ed_Vpl_Rd_lessthan_1"), iRowCnt, iColCnt);
	}
	pXL->SetValue(iStRow, 6, PrvsBase.dRatio);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintShearResistance_Vbw_Rd_Calc(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 11, PrvsBase.dVbw_Rd*m_dFactorForce);
	pXL->SetValue(iStRow + 2, 5, PrvsBase.dVbw_Rd*m_dFactorForce);
	pXL->SetValue(iStRow + 3, 12, PrvsBase.dV_Ed*m_dFactorForce);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintShearResistance_Vbw_Rd_Calc_Sum(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc_Sum"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 15, PrvsBase.dVbw_Rd*m_dFactorForce);
	pXL->SetValue(iStRow + 2, 5, PrvsBase.dVbw_Rd*m_dFactorForce);
	pXL->SetValue(iStRow + 3, 12, PrvsBase.dV_Ed*m_dFactorForce);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintShearResistance_Vbw_Rd_Calc_Part(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_Vbw_Rd_Param& Vbw_Rd_Param)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc_Part"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 11, Vbw_Rd_Param.dVbw_Rd*m_dFactorForce);
	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintShearResistance_PlasticRestance(CMSExcel* pXL, int& iStRow, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;
	CString strBaseSheet = _T("Shear_Resistance");

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_Plastic_Moment_Positive"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_Plastic_Moment_Positive_PNA));
	SetMultiLang(pXL, iStRow + 2, 12, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_UPNA));
	SetMultiLang(pXL, iStRow + 3, 12, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_LPNA));
	SetMultiLang(pXL, iStRow + 4, 12, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_UPNA));
	SetMultiLang(pXL, iStRow + 5, 12, _DLS(IDS_CVL_RPT_EC_Plate_Bending_Plastic_Moment_LPNA));

	pXL->SetValue(iStRow, 7, PrvsDetail.dPlasticNA);
	pXL->SetValue(iStRow + 2, 5, PrvsDetail.dN_slab	*	m_dFactorForce);
	pXL->SetValue(iStRow + 3, 5, PrvsDetail.dNr_top   *   m_dFactorForce);
	pXL->SetValue(iStRow + 4, 5, PrvsDetail.dNr_bot   *   m_dFactorForce);
	pXL->SetValue(iStRow + 5, 5, PrvsDetail.dNg_top	*	m_dFactorForce);
	pXL->SetValue(iStRow + 6, 5, PrvsDetail.dNg_bot	*	m_dFactorForce);
	pXL->SetValue(iStRow + 8, 5, PrvsBase.dMpl_Rd		*	m_dFactorMoment);

	iStRow += iRowCnt + 1;
}


void CRptPlateGirder_EC4::PrintShearResistance_ShearBucklingCoeff(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	int iClassSect = PrvsBase.iSectClass;

	// Modify by GAY. PMS:XXXX. ('12.05.17). Shear Buckling이 필요하고, Transverse Stiffener가 없는 경우, 필요 Msg.
	if (m_nSectType == D_SECT_TYPE_COMPO_I || m_nSectType == D_SECT_TYPE_COMPO_B || m_nSectType == D_SECT_TYPE_COMPO_TUB)
	{
		int nSize = PrvsDetail.aGenWebVbwRdParam.GetSize();
		bool bFind = false;
		for (int i = 0; i < nSize; ++i)
		{
			if (bFind) { continue; }

			T_PRVS_Vbw_Rd_Param& VbwRdParam = PrvsDetail.aGenWebVbwRdParam[i];
			if (VbwRdParam.bIsShearBuck && VbwRdParam.iLamda_w_Type == 0)
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_Need_Transverse_Stiffener"), iRowCnt, iColCnt);
				SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_buckling_Need_Trans_Stiffener1));
				SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_buckling_Need_Trans_Stiffener2));
				iStRow += iRowCnt + 1;

				bFind = true;
			}
		}
	}

	if (m_nSectType == D_SECT_TYPE_COMPO_G)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_M_Ed_BOX"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 5, PrvsBase.dM_Ed*m_dFactorMoment);
		iStRow += iRowCnt;
		PrintShearResistance_Mf_Rd(pXL, iStRow, strBaseSheet, PrvsBase, PrvsDetail);
	}

	CString strSubTitleType; strSubTitleType.Empty();
	strSubTitleType = (PrvsDetail.nShearBuckCount > 0) ? _T("Sub_Title_Shear_Calc_Vbw_Rd") : _T("Sub_Title_Shear_Calc_Vpl_Rd");
	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, strSubTitleType, iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, (PrvsDetail.nShearBuckCount > 0) ? _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Shear_Calc_Vbw_Rd) :
		_DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Shear_Calc_Vpl_Rd), TRUE);
	iStRow += iRowCnt;

	if (m_nSectType == D_SECT_TYPE_COMPO_I || m_nSectType == D_SECT_TYPE_COMPO_B || m_nSectType == D_SECT_TYPE_COMPO_TUB)
	{
		// Title
		PrintTitle_Sub_Title_Shear_Web(pXL, iStRow, strBaseSheet);

		int nWebSize = PrvsDetail.aGenWebVbwRdParam.GetSize();

		bool bShearBuckChk = false;
		for (int nWeb = 0; nWeb < nWebSize; ++nWeb)
		{
			if (PrvsDetail.aGenWebVbwRdParam[nWeb].bIsShearBuck) { bShearBuckChk = true; }
		}

		if (!bShearBuckChk) // No need to check shear buckling, check only plastic resistance
		{
			PrintShearResistance_Vpl_Rd_Calc(pXL, strBaseSheet, iStRow, PrvsBase);
			PrintShearResistance_Vpl_Rd_ChkRatio(pXL, strBaseSheet, iStRow, PrvsBase);
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
		if (PrvsBase.dRatio > 1.0) { pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Ved_VRd_Ratio2"), iRowCnt, iColCnt); }
		else { pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Ved_VRd_Ratio"), iRowCnt, iColCnt); }
		pXL->SetValue(iStRow, 10, PrvsBase.dRatio);
		//
		iStRow += iRowCnt + 1;

		PrintShearResistance_Interation_M_V(pXL, iStRow, strBaseSheet, iClassSect, PrvsBase.dEta1, PrvsBase.dEta1_lim, PrvsBase.dEta3, PrvsBase.dCombinedEffe);
	}
	else if (m_nSectType == D_SECT_TYPE_COMPO_G)
	{
		PrintShearResistance_WebShearBucklingCoeff_GenSect(pXL, iStRow, strBaseSheet, PrvsBase, PrvsDetail);
		PrintShearResistance_FlangeShearBucklingCoeff_GenSect(pXL, iStRow, strBaseSheet, PrvsGd, PrvsBase, PrvsDetail);
	}
	else ASSERT(0);
}

void CRptPlateGirder_EC4::PrintShearResistance_SBC_type1_lessthan_1(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, const T_PRVS_Vbw_Rd_Param& rVbwRdParam)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_lessthan_1"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 18, _DLS(IDS_CVL_RPT_EC_when));
	pXL->SetValue(iStRow, 12, rVbwRdParam.dk_tau);
	pXL->SetValue(iStRow, 23, rVbwRdParam.dAlpha);

	iStRow += iRowCnt;
}

void CRptPlateGirder_EC4::PrintShearResistance_SBC_type1_morethan_1(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, const T_PRVS_Vbw_Rd_Param& rVbwRdParam)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_morethan_1"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 18, _DLS(IDS_CVL_RPT_EC_when));
	pXL->SetValue(iStRow, 12, rVbwRdParam.dk_tau);
	pXL->SetValue(iStRow, 23, rVbwRdParam.dAlpha);

	iStRow += iRowCnt;
}

void CRptPlateGirder_EC4::PrintShearResistance_SBC_type1_calc_param(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, const T_PRVS_Vbw_Rd_Param& rVbwRdParam)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_calc_param"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_where));
	pXL->SetValue(iStRow, 7, rVbwRdParam.dhw);
	pXL->SetValue(iStRow + 1, 10, rVbwRdParam.dk_tau_st_limit);
	pXL->SetValue(iStRow + 2, 21, rVbwRdParam.dk_tau_st);
	pXL->SetValue(iStRow + 3, 7, rVbwRdParam.dIsl);
	pXL->SetValue(iStRow + 4, 7, rVbwRdParam.dt);
	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintShearResistance_SBC_type2(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, const T_PRVS_Vbw_Rd_Param& rVbwRdParam)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type2"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 7, rVbwRdParam.dAlpha);
	pXL->SetValue(iStRow + 1, 20, rVbwRdParam.dk_tau);
	pXL->SetValue(iStRow + 2, 7, rVbwRdParam.dIsl);
	pXL->SetValue(iStRow + 3, 7, rVbwRdParam.dt);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintShearResistance_Vbf_Rd_Calc(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow,
	T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail, const T_PRVS_Vbw_Rd_Param& rVbwRdParam)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbf_Rd_Calc"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow + 2, 3, _DLS(IDS_CVL_RPT_EC_where));
	SetMultiLang(pXL, iStRow + 3, 16, _DLS(IDS_CVL_RPT_EC_Plate_Shear_resistance_Vbf_Rd_Calc_commt), TRUE);
	pXL->SetValue(iStRow, 17, rVbwRdParam.dVbf_Rd*m_dFactorForce);
	pXL->SetValue(iStRow + 2, 8, PrvsDetail.dMf_N_Rd*m_dFactorMoment);
	pXL->SetValue(iStRow + 3, 8, PrvsBase.dM_Ed*m_dFactorMoment);
	pXL->SetValue(iStRow + 4, 18, rVbwRdParam.dc);

	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintShearResistance_Vbf_Rd_Calc_Sum(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbf_Rd_Calc_Sum"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 15, PrvsBase.dVbf_Rd*m_dFactorForce);

	iStRow += iRowCnt + 1;
}


void CRptPlateGirder_EC4::PrintShearResistance_WebShearBucklingCoeff_GenSect(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	int iWebPartSize = PrvsDetail.aGenWebVbwRdParam.GetSize();
	if (iWebPartSize <= 0) return;
	CString strPartType;
	CString strPartName = _T("");
	int nPartType = 0;
	int i = 0;
	int iClassSect = PrvsBase.iSectClass;
	int iEndSupport = PrvsDetail.iEndSupportType;
	for (i = 0; i < iWebPartSize; i++)
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
		if (iLongStiffSize == 1 || iLongStiffSize == 2) // one or two longitudinal stiffeners
		{
			if (dAlpha < 3.0)
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type2"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow, 7, VbwParam.dAlpha);
				pXL->SetValue(iStRow + 1, 20, VbwParam.dk_tau);
				pXL->SetValue(iStRow + 2, 7, VbwParam.dIsl);
				pXL->SetValue(iStRow + 3, 7, VbwParam.dt);
				iStRow += iRowCnt + 1;
			}
			else
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_morethan_1"), iRowCnt, iColCnt);
				SetMultiLang(pXL, iStRow, 18, _DLS(IDS_CVL_RPT_EC_when));
				pXL->SetValue(iStRow, 12, VbwParam.dk_tau);
				pXL->SetValue(iStRow, 23, VbwParam.dAlpha);
				iStRow += iRowCnt;
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_calc_param"), iRowCnt, iColCnt);
				SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_where));
				pXL->SetValue(iStRow, 7, VbwParam.dhw);
				pXL->SetValue(iStRow + 1, 10, VbwParam.dk_tau_st_limit);
				pXL->SetValue(iStRow + 2, 21, VbwParam.dk_tau_st);
				pXL->SetValue(iStRow + 3, 7, VbwParam.dIsl);
				pXL->SetValue(iStRow + 4, 7, VbwParam.dt);
				iStRow += iRowCnt + 1;
			}
		}
		else if (iLongStiffSize == 0 || iLongStiffSize > 2) // without longitudinal stiffeners or with more than two longitudinal stiffeners
		{
			if (iLongStiffSize == 0 && VbwParam.iLamda_w_Type != 2)
			{
				//
			}
			else
			{
				if (dAlpha < 1.0)
				{
					pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_lessthan_1"), iRowCnt, iColCnt);
					SetMultiLang(pXL, iStRow, 18, _DLS(IDS_CVL_RPT_EC_when));
					pXL->SetValue(iStRow, 12, VbwParam.dk_tau);
					pXL->SetValue(iStRow, 23, VbwParam.dAlpha);
					iStRow += iRowCnt;
				}
				else
				{
					pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_morethan_1"), iRowCnt, iColCnt);
					SetMultiLang(pXL, iStRow, 18, _DLS(IDS_CVL_RPT_EC_when));
					pXL->SetValue(iStRow, 12, VbwParam.dk_tau);
					pXL->SetValue(iStRow, 23, VbwParam.dAlpha);
					iStRow += iRowCnt;
				}
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_calc_param"), iRowCnt, iColCnt);
				SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_where));
				pXL->SetValue(iStRow, 7, VbwParam.dhw);
				pXL->SetValue(iStRow + 1, 10, VbwParam.dk_tau_st_limit);
				pXL->SetValue(iStRow + 2, 21, VbwParam.dk_tau_st);
				pXL->SetValue(iStRow + 3, 7, VbwParam.dIsl);
				pXL->SetValue(iStRow + 4, 7, VbwParam.dt);
				iStRow += iRowCnt + 1;
			}

		}

		if (VbwParam.bIsShearBuck)
		{
			if (VbwParam.iLamda_w_Type == 0)
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_Need_Transverse_Stiffener"), iRowCnt, iColCnt);
				SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_buckling_Need_Trans_Stiffener1));
				SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_buckling_Need_Trans_Stiffener2));
				iStRow += iRowCnt + 1;
			}

			CString strLamdawType = Get_String4ShearLamdaw(TRUE, VbwParam.iLamda_w_Type);
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strLamdawType, iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 11, VbwParam.dLamda_w);
			iStRow += iRowCnt + 1;

			if (iLongStiffSize > 0 && VbwParam.dLamda_w < VbwParam.Lamda_w_SubD.dLamda_w)
			{
				PrintShearResistance_SubPanel_Lamda_w(pXL, iStRow, strBaseSheet, VbwParam.Lamda_w_SubD);
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_Lamda_w_check"), iRowCnt, iColCnt);
				SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_buckling_Lamda_w_check), TRUE);
				pXL->SetValue(iStRow + 1, 9, VbwParam.dLamda_w_cr);
				iStRow += iRowCnt + 1;
			}

			// Contribution web
			int iLamda_w_scope = VbwParam.iLamda_w_scope;
			CString strXwType = Get_String4ShearXwType(TRUE, iEndSupport, iLamda_w_scope);
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strXwType, iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 10, VbwParam.dXw);
			iStRow += iRowCnt + 1;
			// Vbw_Rd_Calc
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow + 3, 6, _DLS(IDS_CVL_RPT_EC_Plate_Shear_Shear_resistance_Vpl_Rd_Calc), TRUE);
			pXL->SetValue(iStRow, 11, VbwParam.dVbw_Rd*m_dFactorForce);
			pXL->SetValue(iStRow + 2, 5, VbwParam.dV_Rd*m_dFactorForce);
			pXL->SetValue(iStRow + 3, 12, VbwParam.dV_Edi*m_dFactorForce);
			iStRow += iRowCnt;
			if (VbwParam.deta3 > 1.0)
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc_eta3_morethan_1"), iRowCnt, iColCnt);
			else
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vbw_Rd_Calc_eta3_lessthan_1"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 9, VbwParam.deta3);
			iStRow += iRowCnt + 1;
		}
		else
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vpl_Rd_Calc"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow + 2, 5, _DLS(IDS_CVL_RPT_EC_Plate_Shear_Shear_resistance_Vpl_Rd_Calc), TRUE);
			pXL->SetValue(iStRow, 12, VbwParam.dVpl_Rd*m_dFactorForce);
			pXL->SetValue(iStRow + 1, 5, VbwParam.dV_Rd*m_dFactorForce);
			pXL->SetValue(iStRow + 2, 12, VbwParam.dV_Edi*m_dFactorForce);
			iStRow += iRowCnt + 1;
			if (VbwParam.deta3 > 1.0)
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_V_Ed_Vpl_Rd_morehan_1"), iRowCnt, iColCnt);
			else
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_V_Ed_Vpl_Rd_lessthan_1"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 6, VbwParam.deta3);
			iStRow += iRowCnt + 1;
		}
		PrintShearResistance_Interation_M_V(pXL, iStRow, strBaseSheet, iClassSect, PrvsBase.dEta1, PrvsBase.dEta1_lim, VbwParam.deta3, VbwParam.dRatio_InteractionMV);
	}
}

void CRptPlateGirder_EC4::PrintShearResistance_FlangeShearBucklingCoeff_GenSect(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	int iPartSize = PrvsDetail.aGenFlangeVbwRdParam.GetSize();
	if (iPartSize <= 0) return;
	CString strPartType;
	CString strPartName = _T("");
	int nPartType = 0;

	int i = 0;
	for (i = 0; i < iPartSize; i++)
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
		SetMultiLang(pXL, iStRow + 1, 3, _DLS(IDS_CVL_RPT_EC_where));
		pXL->SetValue(iStRow, 12, VbwParam.dtau_Ed);
		pXL->SetValue(iStRow + 1, 7, PrvsGd.dFz*m_dFactorForce);
		pXL->SetValue(iStRow + 2, 7, VbwParam.dQi);
		pXL->SetValue(iStRow + 3, 7, VbwParam.dIyyi);
		pXL->SetValue(iStRow + 4, 7, VbwParam.dt);
		iStRow += iRowCnt + 1;

		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_flange_stress_After_G_shape"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow + 1, 3, _DLS(IDS_CVL_RPT_EC_where));
		pXL->SetValue(iStRow, 12, VbwParam.dtau_Ed_c);
		pXL->SetValue(iStRow + 1, 7, PrvsBase.dFz*m_dFactorForce);
		pXL->SetValue(iStRow + 2, 7, VbwParam.dQi_c);
		pXL->SetValue(iStRow + 3, 7, VbwParam.dIyyi_c);
		pXL->SetValue(iStRow + 4, 7, VbwParam.dt);
		iStRow += iRowCnt + 1;

		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_flange_stress_sum"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 12, VbwParam.dtau_Ed_sum);
		pXL->SetValue(iStRow + 1, 9, VbwParam.dtau_Ed_ave);
		iStRow += iRowCnt + 1;

		// Vbw_Rd_Calc
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vb_Rd_flange_Calc"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 11, VbwParam.dtau_b_Rd);
		iStRow += iRowCnt;

		if (VbwParam.deta3 > 1.0)
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vb_Rd_flange_Calc_eta3_morethan_1"), iRowCnt, iColCnt);
		else
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Vb_Rd_flange_Calc_eta3_lessthan_1"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 9, VbwParam.deta3);
		iStRow += iRowCnt + 1;
	}
}

void CRptPlateGirder_EC4::PrintShearResistance_ContributionWeb(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	// contribution from the web
	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_contribution_web"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Shear_contribution_web));
	iStRow += iRowCnt;

	bool bBoxSect = (m_nSectType == D_SECT_TYPE_COMPO_B || m_nSectType == D_SECT_TYPE_COMPO_TUB);
	int nWebSize = PrvsDetail.aGenWebVbwRdParam.GetSize();

	for (int nWeb = 0; nWeb < nWebSize; ++nWeb)
	{
		if (bBoxSect && nWeb == 0) { PrintTitle_Sub_Title_Shear_LeftWeb(pXL, iStRow, strBaseSheet); }
		if (bBoxSect && nWeb == 1) { PrintTitle_Sub_Title_Shear_Right_Web(pXL, iStRow, strBaseSheet); }

		// Shear Buckling Coeff.
		T_PRVS_Vbw_Rd_Param& VbwRdParam = PrvsDetail.aGenWebVbwRdParam[nWeb];

		double dAlpha = VbwRdParam.dAlpha;
		int iLongStiffSize = VbwRdParam.iStiffSize;

		if (iLongStiffSize == 1 || iLongStiffSize == 2) // one or two longitudinal stiffeners
		{
			if (dAlpha < 3.0)
			{
				PrintShearResistance_SBC_type2(pXL, strBaseSheet, iStRow, VbwRdParam);
			}
			else
			{
				PrintShearResistance_SBC_type1_morethan_1(pXL, strBaseSheet, iStRow, VbwRdParam);
				PrintShearResistance_SBC_type1_calc_param(pXL, strBaseSheet, iStRow, VbwRdParam);
			}
		}
		else if (iLongStiffSize == 0 || iLongStiffSize > 2) // without longitudinal stiffeners or with more than two longitudinal stiffeners
		{
			if (iLongStiffSize == 0)
			{
				// Print Nothing.. ???
			}
			else
			{
				if (dAlpha < 1.0)
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
		CString strXwType = _T("");
		int iEndSupportType = PrvsDetail.iEndSupportType;
		int iLamda_w_scope = VbwRdParam.iLamda_w_scope;

		CString strLamdawType = Get_String4ShearLamdaw(TRUE, VbwRdParam.iLamda_w_Type);
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strLamdawType, iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 11, VbwRdParam.dLamda_w);
		iStRow += iRowCnt + 1;

		if (iLongStiffSize > 0 && VbwRdParam.dLamda_w < VbwRdParam.Lamda_w_SubD.dLamda_w)
		{
			PrintShearResistance_SubPanel_Lamda_w(pXL, iStRow, strBaseSheet, VbwRdParam.Lamda_w_SubD);
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_Lamda_w_check"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_buckling_Lamda_w_check));
			pXL->SetValue(iStRow + 1, 9, VbwRdParam.dLamda_w_cr);
			iStRow += iRowCnt + 1;
		}

		strXwType = Get_String4ShearXwType(TRUE, iEndSupportType, iLamda_w_scope);
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, strXwType, iRowCnt, iColCnt);
		pXL->SetValue(iStRow, 10, VbwRdParam.dXw);
		iStRow += iRowCnt + 1;

		if (bBoxSect)
		{
			PrintShearResistance_Vbw_Rd_Calc_Part(pXL, strBaseSheet, iStRow, VbwRdParam);
		}
	}

	// Vbw_Rd_Calc
	if (bBoxSect)
	{
		PrintShearResistance_Vbw_Rd_Calc_Sum(pXL, strBaseSheet, iStRow, PrvsBase);
	}
	else
	{
		PrintShearResistance_Vbw_Rd_Calc(pXL, strBaseSheet, iStRow, PrvsBase);
	}

	if (PrvsBase.dEta3 > 1.0)
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

void CRptPlateGirder_EC4::PrintShearResistance_SubPanel_Lamda_w(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_Vbw_Rd_Sub_Panel& VbwSubPanel)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_sub_panel_Sub_Title"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_buckling_coeff_sub_panel_Sub), TRUE);
	iStRow += iRowCnt;

	if (VbwSubPanel.nLongStfnSize == 1 || VbwSubPanel.nLongStfnSize == 2) // 0=종방향 보강재 1 또는 2개 포함하는 패널,
	{
		if (VbwSubPanel.dAlpha < 3.0)
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type2_sub_ls"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow, 7, VbwSubPanel.dAlpha);
			pXL->SetValue(iStRow + 1, 20, VbwSubPanel.dk_tau);
			pXL->SetValue(iStRow + 2, 7, VbwSubPanel.dIsl);
			pXL->SetValue(iStRow + 3, 7, VbwSubPanel.dt);
			iStRow += iRowCnt + 1;
		}
		else
		{
			if (VbwSubPanel.dAlpha < 1.0)
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_lessthan_1"), iRowCnt, iColCnt);
				SetMultiLang(pXL, iStRow, 18, _DLS(IDS_CVL_RPT_EC_when));
				pXL->SetValue(iStRow, 12, VbwSubPanel.dk_tau);
				pXL->SetValue(iStRow, 23, VbwSubPanel.dAlpha);
				iStRow += iRowCnt;
			}
			else
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_morethan_1"), iRowCnt, iColCnt);
				SetMultiLang(pXL, iStRow, 18, _DLS(IDS_CVL_RPT_EC_when));
				pXL->SetValue(iStRow, 12, VbwSubPanel.dk_tau);
				pXL->SetValue(iStRow, 23, VbwSubPanel.dAlpha);
				iStRow += iRowCnt;
			}

			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_ls_calc_param"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_where));
			pXL->SetValue(iStRow, 7, VbwSubPanel.dhw);
			pXL->SetValue(iStRow + 1, 10, VbwSubPanel.dk_tau_st_lt);
			pXL->SetValue(iStRow + 2, 21, VbwSubPanel.dk_tau);
			pXL->SetValue(iStRow + 3, 7, VbwSubPanel.dIsl);
			pXL->SetValue(iStRow + 4, 7, VbwSubPanel.dt);
			iStRow += iRowCnt + 1;
		}
	}
	else //1= 없거나 3개이상
	{
		if (VbwSubPanel.dAlpha < 1.0)
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_lessthan_1"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow, 18, _DLS(IDS_CVL_RPT_EC_when));
			pXL->SetValue(iStRow, 12, VbwSubPanel.dk_tau);
			pXL->SetValue(iStRow, 23, VbwSubPanel.dAlpha);
			iStRow += iRowCnt;
		}
		else
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_morethan_1"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow, 18, _DLS(IDS_CVL_RPT_EC_when));
			pXL->SetValue(iStRow, 12, VbwSubPanel.dk_tau);
			pXL->SetValue(iStRow, 23, VbwSubPanel.dAlpha);
			iStRow += iRowCnt;
		}

		if (VbwSubPanel.nLongStfnSize == 0)
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_calc_param"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_where));
			pXL->SetValue(iStRow, 7, VbwSubPanel.dhw);
			iStRow += iRowCnt + 1;
		}
		else
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_coeff_type1_sub_ls_calc_param"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_where));
			pXL->SetValue(iStRow, 7, VbwSubPanel.dhw);
			pXL->SetValue(iStRow + 1, 10, VbwSubPanel.dk_tau_st_lt);
			pXL->SetValue(iStRow + 2, 21, VbwSubPanel.dk_tau_st);
			pXL->SetValue(iStRow + 3, 7, VbwSubPanel.dIsl);
			pXL->SetValue(iStRow + 4, 7, VbwSubPanel.dt);
			iStRow += iRowCnt + 1;
		}
	}

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_buckling_Lamda_w_sub_type3"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 12, VbwSubPanel.dLamda_w);
	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintShearResistance_ContributionFlange(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	// Title - contribution from the Flange
	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_contribution_Flange"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Shear_contribution_Flange));
	iStRow += iRowCnt;

	PrintShearResistance_Mf_Rd(pXL, iStRow, strBaseSheet, PrvsBase, PrvsDetail);

	bool bBoxSect = (m_nSectType == D_SECT_TYPE_COMPO_B || m_nSectType == D_SECT_TYPE_COMPO_TUB);
	int nWebSize = PrvsDetail.aGenWebVbwRdParam.GetSize();

	for (int nWeb = 0; nWeb < nWebSize; ++nWeb)
	{
		if (bBoxSect && nWeb == 0) { PrintTitle_Sub_Title_Shear_LeftWeb(pXL, iStRow, strBaseSheet); }
		if (bBoxSect && nWeb == 1) { PrintTitle_Sub_Title_Shear_Right_Web(pXL, iStRow, strBaseSheet); }

		T_PRVS_Vbw_Rd_Param& VbwRdParam = PrvsDetail.aGenWebVbwRdParam[nWeb];
		PrintShearResistance_Vbf_Rd_Calc(pXL, strBaseSheet, iStRow, PrvsBase, PrvsDetail, VbwRdParam);
	}

	if (bBoxSect) { PrintShearResistance_Vbf_Rd_Calc_Sum(pXL, strBaseSheet, iStRow, PrvsBase); }
}

void CRptPlateGirder_EC4::PrintShearResistance_Interation_M_V(CMSExcel* pXL, int& iStRow, CString strBaseSheet, int iClassSect, double deta1, double dEta1_lim, double deta3, double dRatio)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	// Sub Title
	pXL->CopyRange(iStRow, 0, m_strBaseBook, strBaseSheet, _T("Sub_Title_Shear_interation_M_V"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Sub_Title_Shear_interation_M_V));
	iStRow += iRowCnt;

	if (iClassSect == 1 || iClassSect == 2)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_interation_M_V_Class_1or2"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_resistance_inter_MV_eta1_lt_Lim));
		iStRow += iRowCnt + 1;
	}
	else if (iClassSect == 3 || iClassSect == 4)
	{
		if (deta3 < 0.5)
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_interation_M_V_eta3_lessthan_05"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_resistance_inter_MV_eta3_lt_05));
			pXL->SetValue(iStRow, 4, deta3);
			iStRow += iRowCnt + 1;
		}
		else if (deta1 < dEta1_lim)
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
			if (dRatio > 1.0)
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

void CRptPlateGirder_EC4::PrintShearResistance_Mf_Rd(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	if (PrvsBase.dN_Ed < 0.0)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Mf_Rd0"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_resistance_Mf_Rd0), TRUE);
		SetMultiLang(pXL, iStRow + 2, 2, _DLS(IDS_CVL_RPT_EC_Plate_Reduction_factor_N_Ed), TRUE);
		SetMultiLang(pXL, iStRow + 4, 6, _DLS(IDS_CVL_RPT_EC_Plate_Reduction_factor_N_Ed_dot), TRUE);
		pXL->SetValue(iStRow, 5, PrvsDetail.dMf_Rd_cal*m_dFactorMoment);
		pXL->SetValue(iStRow + 2, 18, PrvsDetail.dReduFactor4N_Ed);
		pXL->SetValue(iStRow + 4, 18, PrvsDetail.dMf_N_Rd*m_dFactorMoment);
		iStRow += iRowCnt + 1;
	}
	else
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Shear_resistance_Mf_Rd"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_Shear_resistance_Mf_Rd), TRUE);
		pXL->SetValue(iStRow, 5, PrvsDetail.dMf_Rd_cal*m_dFactorMoment);
		iStRow += iRowCnt + 1;
	}
}

// Lateral Torsional Buckling Check Result Print
void CRptPlateGirder_EC4::PrintLateralTorsionalBucklingResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PLTB_BASE& PltbGd, T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("LTB_Resistance");
	// Title
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Title_of_LTBResistance"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Title_of_LTBResistance));
	pXL->SetValue(iStRow, 1, m_iLTBChapterIx);
	iStRow += iRowCnt;

	CString strLcomName = GetLoadCombinationName(PltbBase.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("LTB_Design_Force"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DesignLoad));
	SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_LCOM));
	pXL->SetValue(iStRow + 1, 10, strLcomName);
	pXL->SetValue(iStRow + 2, 5, PltbBase.dN_Ed*m_dFactorForce);
	pXL->SetValue(iStRow + 3, 5, PltbBase.dM_Ed*m_dFactorMoment);
	pXL->SetValue(iStRow + 4, 5, PltbBase.dV1  *m_dFactorForce);
	pXL->SetValue(iStRow + 5, 5, PltbBase.dV2  *m_dFactorForce);
	pXL->SetValue(iStRow + 6, 5, PltbBase.dM1  *m_dFactorMoment);
	pXL->SetValue(iStRow + 7, 5, PltbBase.dM2  *m_dFactorMoment);
	pXL->SetValue(iStRow + 8, 5, PltbDetail.dMpl_Rd*m_dFactorMoment);
	pXL->SetValue(iStRow + 9, 5, PltbDetail.dMel_Rd*m_dFactorMoment);
	iStRow += iRowCnt + 1;

	// Mb,Rd Buckling Resistance Moment
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("LTB_Mb_Rd_Buckling_Resistance"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_LTB_Mb_Rd_Buckling_Resistance), TRUE);
	pXL->SetValue(iStRow + 1, 5, PltbDetail.dL * m_dFactorLength);
	pXL->SetValue(iStRow + 2, 8, PltbBase.dcc / m_dFactorLength);
	pXL->SetValue(iStRow + 3, 10, PltbBase.dgamma);
	pXL->SetValue(iStRow + 4, 9, PltbBase.dMu);
	pXL->SetValue(iStRow + 5, 12, PltbBase.dPHI);
	pXL->SetValue(iStRow + 6, 21, PltbBase.dm1);
	pXL->SetValue(iStRow + 7, 21, PltbBase.dm2);
	pXL->SetValue(iStRow + 8, 10, PltbBase.dm);
	pXL->SetValue(iStRow + 9, 5, PltbBase.dAlpha_LT);
	pXL->SetValue(iStRow + 10, 17, PltbBase.dLamda_LT);
	pXL->SetValue(iStRow + 11, 15, PltbBase.dPHI_LT);
	pXL->SetValue(iStRow + 12, 12, PltbBase.dX_LT);
	pXL->SetValue(iStRow + 15, 5, PltbBase.dM_Rd*m_dFactorMoment);
	pXL->SetValue(iStRow + 16, 9, PltbBase.dMb_Rd*m_dFactorMoment);
	iStRow += iRowCnt + 1;

	// Nb,Rd Buckling Resistance Moment
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("LTB_Nb_Rd_Buckling_Resistance"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_LTB_Nb_Rd_Buckling_Resistance), TRUE);
	SetMultiLang(pXL, iStRow, 4, _DLS(IDS_CVL_RPT_EC_Plate_LTB_Nb_Rd_Buckling_Resistance_ComRat));
	pXL->SetValue(iStRow + 1, 5, PltbBase.dX_LT_N);
	pXL->SetValue(iStRow + 2, 11, PltbBase.dNb_Rd*m_dFactorForce);
	pXL->SetValue(iStRow + 4, 14, PltbBase.dInterRat);
	iStRow += iRowCnt + 1;

	if ( CDBLib::IsEC4McrOutput() )
	{
		// U frame model
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("LTB_U_frame_model"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_LTB_U_frame_model));
		pXL->SetValue(iStRow + 1, 4, PltbDetail.iAlpha);
		pXL->SetValue(iStRow + 2, 4, PltbDetail.dhs);
		pXL->SetValue(iStRow + 3, 4, PltbDetail.da);
		pXL->SetValue(iStRow + 4, 4, PltbDetail.dEa);
		pXL->SetValue(iStRow + 5, 4, PltbDetail.dGs);
		pXL->SetValue(iStRow + 6, 4, PltbDetail.dI2);
		pXL->SetValue(iStRow + 7, 9, PltbBase.dk1);
		pXL->SetValue(iStRow + 8, 12, PltbBase.dk2);
		pXL->SetValue(iStRow + 9, 10, PltbBase.dks);
		pXL->SetValue(iStRow + 11, 5, PltbBase.dC4);
		pXL->SetValue(iStRow + 12, 5, PltbBase.dI_at);
		pXL->SetValue(iStRow + 13, 5, PltbBase.dI_afz);
		pXL->SetValue(iStRow + 14, 5, PltbBase.dkc);
		pXL->SetValue(iStRow + 15, 5, PltbBase.dMcr*m_dFactorMoment);
		iStRow += iRowCnt + 1;
	}
}

void CRptPlateGirder_EC4::PrintTransverseForceResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PRTF_BASE& PrtfBase, T_PRTF_DETAIL& PrtfDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("TranForce_Resistance");
	// Title
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Title_of_TransverseForceResistance"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Title_of_TransverseForceResistance));
	pXL->SetValue(iStRow, 1, m_iTransverseForceChapterIx);
	iStRow += iRowCnt;

	// Design Force
	CString strLcomName = GetLoadCombinationName(PrtfBase.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Design_Force"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DesignLoad));
	SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_LCOM));
	pXL->SetValue(iStRow + 1, 5, strLcomName);
	pXL->SetValue(iStRow + 2, 5, PrtfBase.dN_Ed*m_dFactorForce);
	pXL->SetValue(iStRow + 3, 5, PrtfBase.dMy_Ed*m_dFactorMoment);
	pXL->SetValue(iStRow + 4, 5, PrtfBase.dMz_Ed*m_dFactorMoment);
	pXL->SetValue(iStRow + 5, 5, PrtfBase.dF_Ed*m_dFactorForce);
	iStRow += iRowCnt + 1;

	// Eta1, 1993-1-5, (4.15)
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_Title_eta1"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Transverse_Sub_Title_eta1));
	iStRow += iRowCnt;
	if (PrtfBase.dEta1 > 1.0)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_eta1_MoreThan_1"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 15, PrtfBase.dEta1);
	}
	else
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_eta1_LessThan_1"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 15, PrtfBase.dEta1);
	}
	iStRow += iRowCnt;
	//
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_eta1_param"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 3, _DLS(IDS_CVL_RPT_EC_where));
	pXL->SetValue(iStRow, 8, PrtfBase.dey_N);
	pXL->SetValue(iStRow + 1, 8, PrtfDetail.dWy_eff);
	iStRow += iRowCnt + 1;

	// - Eta2, 1993-1-5, (6.14)
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_Title_LoadApp_type"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Transverse_Sub_Title_LoadApp_type));
	iStRow += iRowCnt;

	int iLoadAppType = PrtfDetail.iLoadAppType;
	if (iLoadAppType == 0)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_LoadApp_type_a"), iRowCnt, iColCnt);
		iStRow += iRowCnt;
		if (!PrtfDetail.bIsStiffener)
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_title_kF_without_stiff"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Transverse_Sub_title_kF_without_stiff));
			iStRow += iRowCnt;
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_kF_without_stiff"), iRowCnt, iColCnt);
			pXL->SetValue(iStRow + 0, 9, PrtfBase.dk_F);
			iStRow += iRowCnt + 1;
		}
		else
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_title_kF_with_stiff"), iRowCnt, iColCnt);
			SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Transverse_Sub_title_kF_with_stiff));
			iStRow += iRowCnt;
			if (PrtfDetail.ik_F_calc_type == 1)
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_kF_with_stiff"), iRowCnt, iColCnt);
				SetMultiLang(pXL, iStRow + 1, 3, _DLS(IDS_CVL_RPT_EC_where));
				pXL->SetValue(iStRow + 0, 18, PrtfBase.dk_F);
				pXL->SetValue(iStRow + 1, 18, PrtfBase.dgamma_s);
				pXL->SetValue(iStRow + 2, 18, PrtfDetail.dGamma_s_cal);
				pXL->SetValue(iStRow + 3, 18, PrtfDetail.dGamma_s_limit);
				pXL->SetValue(iStRow + 4, 7, PrtfBase.db1);
				pXL->SetValue(iStRow + 5, 7, PrtfDetail.dhw);
				pXL->SetValue(iStRow + 6, 7, PrtfDetail.da);
			}
			else if (PrtfDetail.ik_F_calc_type == 2)
			{
				pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_kF_without_stiff"), iRowCnt, iColCnt);
				pXL->SetValue(iStRow + 0, 9, PrtfBase.dk_F);
			}
			else ASSERT(0);
			iStRow += iRowCnt + 1;
		}
	}
	else if (iLoadAppType == 1)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_LoadApp_type_b"), iRowCnt, iColCnt);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_kF_Loadtype_b"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow + 1, 3, _DLS(IDS_CVL_RPT_EC_where));
		pXL->SetValue(iStRow + 0, 10, PrtfBase.dk_F);
		pXL->SetValue(iStRow + 1, 7, PrtfDetail.dhw);
		pXL->SetValue(iStRow + 2, 7, PrtfDetail.da);
		iStRow += iRowCnt + 1;
	}
	else if (iLoadAppType == 2)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_LoadApp_type_c"), iRowCnt, iColCnt);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_kF_Loadtype_c"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow + 1, 3, _DLS(IDS_CVL_RPT_EC_where));
		pXL->SetValue(iStRow + 0, 10, PrtfBase.dk_F);
		pXL->SetValue(iStRow + 1, 7, PrtfDetail.dSs);
		pXL->SetValue(iStRow + 2, 7, PrtfDetail.dC);
		pXL->SetValue(iStRow + 3, 7, PrtfDetail.dhw);
		iStRow += iRowCnt + 1;
	}
	else ASSERT(0);

	// Effective loaded length
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_title_Eff_loaded_length"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Transverse_Sub_title_Eff_loaded_leng));
	iStRow += iRowCnt;

	if (iLoadAppType == 0 || iLoadAppType == 1)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Eff_loaded_length_loadtype_a_b"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow + 1, 3, _DLS(IDS_CVL_RPT_EC_where));
		pXL->SetValue(iStRow + 0, 13, PrtfBase.dly);
		pXL->SetValue(iStRow + 1, 7, PrtfBase.dm1);
		pXL->SetValue(iStRow + 2, 7, PrtfBase.dm2);
		iStRow += iRowCnt + 1;
	}
	else if (iLoadAppType == 2)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Eff_loaded_length_loadtype_c"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 14, PrtfBase.dle);
		pXL->SetValue(iStRow + 1, 14, PrtfBase.dly1);
		pXL->SetValue(iStRow + 2, 14, PrtfBase.dly2);
		pXL->SetValue(iStRow + 3, 9, PrtfBase.dly);
		iStRow += iRowCnt + 1;
	}
	else ASSERT(0);

	// Design resistance
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_title_Design_resistance"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Transverse_Sub_title_Design_resist));
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_F_Rd_Calc"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow + 1, 3, _DLS(IDS_CVL_RPT_EC_where));
	pXL->SetValue(iStRow + 0, 11, PrtfBase.dF_Rd*m_dFactorForce);
	pXL->SetValue(iStRow + 1, 15, PrtfBase.dFcr*m_dFactorForce);
	pXL->SetValue(iStRow + 2, 15, PrtfBase.dLamda_F);
	pXL->SetValue(iStRow + 3, 11, PrtfBase.dX_F);
	pXL->SetValue(iStRow + 4, 11, PrtfBase.dL_eff);
	iStRow += iRowCnt + 1;

	// Verification
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Sub_title_Verification"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Verification));
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Transverse_Verification_eta2"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow, 8, PrtfBase.dEta2);
	iStRow += iRowCnt;
	if (PrtfBase.dInterRat > 1.4)
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

void CRptPlateGirder_EC4::PrintLongitudinalShearResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PGLS_BASE& PglsGd, T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("LongShear_Resistance");
	// Title
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Title_of_LongitudinalShearResistance"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Title_of_LongitudinalShearResistance));
	pXL->SetValue(iStRow, 1, m_iLongitudinalChapterShearIx);
	iStRow += iRowCnt;

	// Design Force
	CString strLcomName = GetLoadCombinationName(PglsBase.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Design_Force"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DesignLoad));
	SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_LCOM));
	pXL->SetValue(iStRow + 1, 10, strLcomName);
	pXL->SetValue(iStRow + 2, 5, PglsBase.dNc_el*m_dFactorForce);
	pXL->SetValue(iStRow + 3, 5, PglsBase.dNc_f*m_dFactorForce);
	pXL->SetValue(iStRow + 4, 5, (PglsBase.dMa_Ed + PglsBase.dMy)*m_dFactorMoment);  // CHECK by GAY. PMS:4216. ('12.05.17). 계산은 My_Gd+My_Base 로 : dM_Ed  = dMy_a + dMy;
	pXL->SetValue(iStRow + 5, 5, PglsBase.dFz*m_dFactorForce);
	pXL->SetValue(iStRow + 6, 5, PglsBase.dMpl_Rd*m_dFactorMoment);
	pXL->SetValue(iStRow + 7, 5, PglsBase.dMel_Rd*m_dFactorMoment);
	iStRow += iRowCnt + 1;

	// Shear resistance of a single connector
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_shear_sigle_connector"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_LongShear_Sub_title_shear_sigle));
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow + 0, 13, PglsBase.dP_Rd1*m_dFactorForce);
	pXL->SetValue(iStRow + 1, 15, PglsBase.dP_Rd2*m_dFactorForce);
	pXL->SetValue(iStRow + 2, 11, PglsBase.dP_Rd*m_dFactorForce);
	iStRow += iRowCnt + 1;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param1"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 3, _DLS(IDS_CVL_RPT_EC_where));
	pXL->SetValue(iStRow + 0, 7, PglsDetail.dfu);
	iStRow += iRowCnt;
	if (PglsDetail.iRat_sc_type == 1)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param_alpha_type1"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 19, _DLS(IDS_CVL_RPT_EC_for));
		pXL->SetValue(iStRow + 0, 13, PglsBase.dAlpha);
	}
	else if (PglsDetail.iRat_sc_type == 2)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param_alpha_type2"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 19, _DLS(IDS_CVL_RPT_EC_for));
	}
	else if (PglsDetail.iRat_sc_type == 3)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param_alpha_type3"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 13, PglsBase.dAlpha);
	}
	else ASSERT(0);
	iStRow += iRowCnt;
	//
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param2"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 5, _DLS(IDS_CVL_RPT_EC_Num));
	SetMultiLang(pXL, iStRow + 3, 5, _DLS(IDS_CVL_RPT_EC_space));
	pXL->SetValue(iStRow + 0, 8, PglsDetail.iNum);
	pXL->SetValue(iStRow + 1, 8, PglsDetail.dDsc);
	pXL->SetValue(iStRow + 2, 8, PglsDetail.dhsc);
	pXL->SetValue(iStRow + 3, 8, PglsDetail.dSpace);
	iStRow += iRowCnt + 1;

	//
	// Longitudinal shear force acting on length LA_B
	double dM_Ed = PglsGd.dMy + PglsBase.dMy;
	if ((PglsBase.iSectClass == 1 || PglsBase.iSectClass == 2) && dM_Ed >= 0.0 && dM_Ed - PglsBase.dMel_Rd > 0.0)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_V_L_Ed_calc"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_LongShear_Sub_title_V_L_Ed_calc), TRUE);
		iStRow += iRowCnt;

		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_V_L_Ed_1"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 14, PglsBase.dV_L_Ed*m_dFactorForce);
		iStRow += iRowCnt + 1;
		//
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_Verification_interface"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Verification));
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Ed_interface"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow + 1, 3, _DLS(IDS_CVL_RPT_EC_where));
		pXL->SetValue(iStRow + 0, 9, PglsBase.dv_L_Ed);
		pXL->SetValue(iStRow + 1, 7, PglsDetail.dLv);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Rd"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 12, PglsBase.dv_L_Rd);
		iStRow += iRowCnt;
		if (fabs(PglsBase.dv_L_Ed) > fabs(PglsBase.dv_L_Rd))
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
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Verification));
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Ed_slab"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 12, PglsBase.dv_L_Ed);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Rd"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 12, PglsBase.dv_L_Rd);
		iStRow += iRowCnt;
		if (fabs(PglsBase.dv_L_Ed) > fabs(PglsBase.dv_L_Rd))
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_interface_v_L_Ed_MoreThan_v_L_Rd"), iRowCnt, iColCnt);
		}
		else
		{
			pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_interface_v_L_Ed_LessThan_v_L_Rd"), iRowCnt, iColCnt);
		}
		iStRow += iRowCnt + 1;
	}


	// Modify, Jaoeh. [6/1/2011] 이대근 부장님 불필요하다고 해서 고려안함. 
//  pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_conc"), iRowCnt, iColCnt);
//  SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_LongShear_Sub_title_conc));
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

}

void CRptPlateGirder_EC4::PrintFatigueResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PGRF_BASE& PgrfBase, T_PGRF_DETAIL& PgrfDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("Fatigue_Resistance");
	// Title
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Title_of_FatigueResistance"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Title_of_FatigueResistance));
	pXL->SetValue(iStRow, 1, m_iFatigueChapterIx);
	iStRow += iRowCnt;

	// Design Force
	CString strLcomName = GetLoadCombinationName(PgrfBase.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_Design_Force"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_DesignLoad));
	SetMultiLang(pXL, iStRow + 1, 2, _DLS(IDS_CVL_RPT_EC_Plate_LCOM));
	pXL->SetValue(iStRow + 1, 10, strLcomName);
	pXL->SetValue(iStRow + 2, 5, PgrfBase.dFz*m_dFactorForce);
	iStRow += iRowCnt + 1;
	// Shear stress range for the connector
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_shear_stress_range_stud"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Fatigue_shear_stress_range_stud));
	SetMultiLang(pXL, iStRow + 2, 3, _DLS(IDS_CVL_RPT_EC_where));
	SetMultiLang(pXL, iStRow + 2, 8, _DLS(IDS_CVL_RPT_EC_Plate_Fatigue_shear_stress_range_stud_Fsc), TRUE);
	pXL->SetValue(iStRow + 1, 8, PgrfBase.dDel_Tau);
	pXL->SetValue(iStRow + 2, 20, PgrfBase.dFsc*m_dFactorForce);
	pXL->SetValue(iStRow + 3, 8, PgrfBase.dAsc);
	iStRow += iRowCnt + 1;
	// Shear stress range for the connector
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_Damage_equivalent_factor_lamda_v"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Fatigue_Damage_equivalent_factor));
	SetMultiLang(pXL, iStRow + 2, 3, _DLS(IDS_CVL_RPT_EC_where));
	pXL->SetValue(iStRow + 1, 11, PgrfBase.dLamda_v);
	pXL->SetValue(iStRow + 2, 8, PgrfBase.dLamda_v1);
	pXL->SetValue(iStRow + 3, 8, PgrfBase.dLamda_v2);
	pXL->SetValue(iStRow + 4, 8, PgrfBase.dLamda_v3);
	pXL->SetValue(iStRow + 5, 8, PgrfBase.dLamda_v4);
	iStRow += iRowCnt + 1;
	// Equivalent constant amplitude range of shear stress related to 2 million cycles
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_delta_tau_E2"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Fatigue_delta_tau_E2));
	pXL->SetValue(iStRow + 1, 9, PgrfBase.dDel_Tau_E_2);
	iStRow += iRowCnt + 1;
	// Verification
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_Sub_title_Verification"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Verification));
	iStRow += iRowCnt;
	if (PgrfBase.bOK)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_Verification_LessThan_1"), iRowCnt, iColCnt);
	}
	else
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Fatigue_Verification_MoreThan_1"), iRowCnt, iColCnt);
	}
	pXL->SetValue(iStRow + 0, 10, PgrfBase.dRatio);
	iStRow += iRowCnt + 1;

}

void CRptPlateGirder_EC4::PrintStressLimit(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PGBS_BASE& PgbsBase, T_PGBS_DETAIL& PgbsDetail)
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
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_StressLT_structural_steel));
	SetMultiLang(pXL, iStRow + 1, 3, _DLS(IDS_CVL_RPT_EC_Plate_StressLT_Characteristic));
	SetMultiLang(pXL, iStRow + 2, 13, _DLS(IDS_CVL_RPT_EC_Plate_StressLT_structural_steel_commt1));
	SetMultiLang(pXL, iStRow + 3, 13, _DLS(IDS_CVL_RPT_EC_Plate_StressLT_structural_steel_commt2));
	pXL->SetValue(iStRow + 1, 15, strLcomName);
	pXL->SetValue(iStRow + 2, 6, PgbsBase.ChkGirder.dMaxStress);
	pXL->SetValue(iStRow + 3, 6, fabs(PgbsBase.ChkGirder.dtau_w));
	CString strMaxStressPosi, strPosi;
	strPosi = GetStressPosition(PgbsBase.ChkGirder.iIndex_cr);
	strMaxStressPosi.Format(_T("(%s)"), strPosi);
	pXL->SetValue(iStRow + 2, 13, strMaxStressPosi);
	iStRow += iRowCnt + 1;


	// Normal stress
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Title_Sigma_Ed_ser"), iRowCnt, iColCnt);
	iStRow += iRowCnt;
	if (PgbsBase.ChkGirder.bChk_Stress) { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Check_OK"), iRowCnt, iColCnt); }
	else { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Check_NG"), iRowCnt, iColCnt); }
	pXL->SetValue(iStRow, 3, PgbsBase.ChkGirder.dMaxStress);
	pXL->SetValue(iStRow, 12, PgbsBase.ChkGirder.dAlwStress);
	iStRow += iRowCnt + 1;


	// Shear stress
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Title_Tau_Ed_ser"), iRowCnt, iColCnt);
	iStRow += iRowCnt;
	if (PgbsBase.ChkGirder.bChk_tau) { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Check_OK"), iRowCnt, iColCnt); }
	else { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Check_NG"), iRowCnt, iColCnt); }
	pXL->SetValue(iStRow, 3, fabs(PgbsBase.ChkGirder.dtau_w));
	pXL->SetValue(iStRow, 12, PgbsBase.ChkGirder.dAlw_tau);
	iStRow += iRowCnt + 1;


	// Shear stress
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Title_Sigma_Tau_ser_comb"), iRowCnt, iColCnt);
	iStRow += iRowCnt;
	if (PgbsBase.ChkGirder.bChk_Stress_cb) { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Check_OK"), iRowCnt, iColCnt); }
	else { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Check_NG"), iRowCnt, iColCnt); }
	pXL->SetValue(iStRow, 3, PgbsBase.ChkGirder.dStress_cb);
	pXL->SetValue(iStRow, 12, PgbsBase.ChkGirder.dAlw_stress_cb);
	iStRow += iRowCnt + 1;


	// - In the concrete of the slab
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Title"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_StressLT_Slab_Title));
	iStRow += iRowCnt;
	strLcomName = GetLoadCombinationName(PgbsBase.ChkSlab.OrgLcomK);
	if (PgbsBase.ChkSlab.LcomType == 1)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Title_Quasi"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 3, _DLS(IDS_CVL_RPT_EC_Plate_StressLT_Quasi_permanent));
		pXL->SetValue(iStRow, 15, strLcomName);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Sigma_c_Quasi"), iRowCnt, iColCnt);
		iStRow += iRowCnt;
	}
	else if (PgbsBase.ChkSlab.LcomType == 3)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Title_Characteristic"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 3, _DLS(IDS_CVL_RPT_EC_Plate_StressLT_Characteristic));
		pXL->SetValue(iStRow, 15, strLcomName);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Sigma_c_Characteristic"), iRowCnt, iColCnt);
		iStRow += iRowCnt;
	}
	else ASSERT(0);
	if (PgbsBase.ChkSlab.bChk_Stress_sb) { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Check_OK"), iRowCnt, iColCnt); }
	else { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Check_NG"), iRowCnt, iColCnt); }
	pXL->SetValue(iStRow, 3, PgbsBase.ChkSlab.dStress_sb);
	pXL->SetValue(iStRow, 12, PgbsBase.ChkSlab.dAlwStress_sb);
	iStRow += iRowCnt + 1;



	// - In the reinforcement
	strLcomName = GetLoadCombinationName(PgbsBase.ChkBar.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Rebar"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_StressLT_Rebar));
	SetMultiLang(pXL, iStRow + 1, 3, _DLS(IDS_CVL_RPT_EC_Plate_LCOM));
	iStRow += iRowCnt;
	pXL->SetValue(iStRow - 2, 11, strLcomName);
	if (PgbsBase.ChkBar.dStress_bar < 0.0)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_NoCheck_Rebar"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 3, _DLS(IDS_CVL_RPT_EC_Plate_StressLT_NoCheck_Rebar));
		pXL->SetValue(iStRow, 3, PgbsBase.ChkBar.dStress_bar);
	}
	else // 인장. 
	{
		if (PgbsBase.ChkBar.bChk_Stress_bar) { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Check_OK"), iRowCnt, iColCnt); }
		else { pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("StressLT_Slab_Check_NG"), iRowCnt, iColCnt); }
		pXL->SetValue(iStRow, 3, PgbsBase.ChkBar.dStress_bar);
		pXL->SetValue(iStRow, 12, PgbsBase.ChkBar.dAlwStress_bar);
	}
	iStRow += iRowCnt + 1;
}

void CRptPlateGirder_EC4::PrintLongitudinalShear_SLS(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PLSS_BASE& PlssGd, T_PLSS_BASE& PlssBase, T_PLSS_DETAIL& PlssDetail)
{
	int iRowCnt = 0;
	int iColCnt = 0;

	CString strTitleBaseSheet = _T("Bending_Resistance");
	CString strBaseSheet = _T("LongShear_Resistance");
	// Title
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("Title_of_LongitudinalShearSLS"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Title_of_LongitudinalShearSLS));
	pXL->SetValue(iStRow, 1, m_iLongShearSLSChapterIx);
	iStRow += iRowCnt;

	// Shear resistance of a single connector
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_shear_sigle_connector"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_LongShear_Sub_title_shear_sigle));
	iStRow += iRowCnt;
	CString strLcomName = GetLoadCombinationName(PlssBase.OrgLcomK);
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_Comb_Name_SLS"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_LCOM));
	pXL->SetValue(iStRow, 10, strLcomName);
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow + 0, 13, PlssBase.dP_Rd1*m_dFactorForce);
	pXL->SetValue(iStRow + 1, 15, PlssBase.dP_Rd2*m_dFactorForce);
	pXL->SetValue(iStRow + 2, 11, PlssBase.dP_Rd*m_dFactorForce);
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_SLS"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow + 0, 11, PlssBase.dP_Rd_SLS*m_dFactorForce);
	iStRow += iRowCnt + 1;

	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param1"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 3, _DLS(IDS_CVL_RPT_EC_where));
	pXL->SetValue(iStRow + 0, 7, PlssDetail.dfu);
	iStRow += iRowCnt;
	if (PlssDetail.iRat_sc_type == 1)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param_alpha_type1"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 19, _DLS(IDS_CVL_RPT_EC_for));
		pXL->SetValue(iStRow + 0, 13, PlssBase.dAlpha);
	}
	else if (PlssDetail.iRat_sc_type == 2)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param_alpha_type2"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 19, _DLS(IDS_CVL_RPT_EC_for));
	}
	else if (PlssDetail.iRat_sc_type == 3)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param_alpha_type3"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 13, PlssBase.dAlpha);
	}
	else ASSERT(0);
	iStRow += iRowCnt;
	//
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_calc_param2"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 8, _DLS(IDS_CVL_RPT_EC_Num));
	SetMultiLang(pXL, iStRow + 3, 5, _DLS(IDS_CVL_RPT_EC_space));
	pXL->SetValue(iStRow + 0, 8, PlssDetail.iNum);
	pXL->SetValue(iStRow + 1, 8, PlssDetail.dDsc);
	pXL->SetValue(iStRow + 2, 8, PlssDetail.dhsc);
	pXL->SetValue(iStRow + 3, 8, PlssDetail.dSpace);
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_P_Rd_param_ks_SLS"), iRowCnt, iColCnt);
	pXL->SetValue(iStRow + 0, 8, m_CpgdD.dks);
	iStRow += iRowCnt + 1;

	//
	double dM_Ed = PlssGd.aForce[4] + PlssBase.aForce[4];
	if ((PlssBase.iSectClass == 1 || PlssBase.iSectClass == 2) && dM_Ed >= 0.0 && dM_Ed - PlssBase.dMel_Rd > 0.0)
	{
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_V_L_Ed_calc"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_LongShear_Sub_title_V_L_Ed_calc), TRUE);
		iStRow += iRowCnt;

		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_V_L_Ed_1"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 14, PlssBase.dV_L_Ed*m_dFactorForce);
		iStRow += iRowCnt + 1;
		//
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_Sub_title_Verification_interface"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Verification));
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Ed_interface"), iRowCnt, iColCnt);
		SetMultiLang(pXL, iStRow + 1, 3, _DLS(IDS_CVL_RPT_EC_where));
		pXL->SetValue(iStRow + 0, 9, PlssBase.dv_L_Ed);
		pXL->SetValue(iStRow + 1, 7, PlssDetail.dLv);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Rd_SLS"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 12, PlssBase.dv_L_Rd);
		iStRow += iRowCnt;
		if (fabs(PlssBase.dv_L_Ed) > fabs(PlssBase.dv_L_Rd))
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
		SetMultiLang(pXL, iStRow, 2, _DLS(IDS_CVL_RPT_EC_Plate_Verification));
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Ed_slab"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 12, PlssBase.dv_L_Ed);
		iStRow += iRowCnt;
		pXL->CopyRange(iStRow, 1, m_strBaseBook, strTitleBaseSheet, _T("LongShear_v_L_Rd_SLS"), iRowCnt, iColCnt);
		pXL->SetValue(iStRow + 0, 12, PlssBase.dv_L_Rd);
		iStRow += iRowCnt;
		if (fabs(PlssBase.dv_L_Ed) > fabs(PlssBase.dv_L_Rd))
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

void CRptPlateGirder_EC4::Print_CheckElemPosition(CMSExcel* pXL, int& iStRow, CString strBaseSheet, int iElemNo, int nPosi)
{
	int iRowCnt = 0;
	int iColCnt = 8;
	CString strPosi = nPosi == 1 ? _T("I") : _T("J");
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Check_ElementNo"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 1, _DLS(IDS_CVL_RPT_EC_Plate_Check_ElementNo));
	pXL->SetValue(iStRow, 8, iElemNo);
	iStRow += iRowCnt;
	pXL->CopyRange(iStRow, 1, m_strBaseBook, strBaseSheet, _T("Check_Position"), iRowCnt, iColCnt);
	SetMultiLang(pXL, iStRow, 1, _DLS(IDS_CVL_RPT_EC_Plate_Check_Position));
	pXL->SetValue(iStRow, 8, strPosi);
	iStRow += iRowCnt + 1;
}


BOOL CRptPlateGirder_EC4::Get_DesignInfo_Section(ElemPairK ElemK, BOOL bPosiI)
{
	CPG_SECT_ELEM SectElem;  SectElem.Initialize();
	m_pDataCtrl->Get_PlateGirderPointer()->Get_CpgSectElem(ElemK, SectElem);
	T_SECT_D SectD = SectElem.SectD;

	int nPosiId = bPosiI ? 0 : 1;

	int i = 0;
	int nSectType = 0;
	if (SectD.nStype == D_SECT_TYPE_TAPERED) nSectType = SectD.SectBefore.nStype;
	else nSectType = SectD.nStype;
	m_nSectType = nSectType;

	BOOL bTapered = SectD.nStype == D_SECT_TYPE_TAPERED ? TRUE : FALSE;

	T_SECT_STIFFNESS GirderStiffD; GirderStiffD.Initialize();
	T_SECT_STIFFNESS CompoStiffD;  CompoStiffD.Initialize();

	T_SECT_SECTBASE_D SectBaseD; SectBaseD.Initialize();
	if (nSectType != D_SECT_TYPE_COMPO_G)
	{
		if (bPosiI) SectBaseD = SectD.SectBefore.SectI;
		else       SectBaseD = !bTapered ? SectD.SectBefore.SectI : SectD.CmpTapJ;
		m_dBc = SectD.SectAfter.SectJ.Size[0];
		m_dtc = SectD.SectAfter.SectJ.Size[1];
		m_dHh = SectD.SectAfter.SectJ.Size[2];

		for (i = 0; i < 10; i++) { m_dSize[i] = SectBaseD.Size[i]; }
		// girder
		m_pDataCtrl->Get_PlateGirderPointer()->Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiffD);

		// Composite section  
		if (!bTapered) CompoStiffD = SectD.SectAfter.Stiffness;
		else          CompoStiffD = bPosiI ? SectD.SectAfter.SectI.Stiffness : SectD.SectAfter.SectJ.Stiffness;
	}
	else
	{
		SectBaseD = bTapered && !bPosiI ? SectD.SectBefore.SectJ : SectD.SectBefore.SectI;
		int iGenPartSize = SectBaseD.aGeneralPart.GetSize();
		for (i = 0; i < iGenPartSize; i++)
		{
			T_GSEC_PART PartD = SectBaseD.aGeneralPart[i];
			if (i == 0) GirderStiffD = PartD.Stiffness;
			else     CompoStiffD = PartD.StiffnessSum;
		}
	}

	m_dArea_g = GirderStiffD.Area;
	m_dIyy_g = GirderStiffD.Ryy;
	m_dIzz_g = GirderStiffD.Rzz;
	m_dYbar_g = GirderStiffD.Cym;
	m_dZbar_g = GirderStiffD.Czm;

	m_dArea = CompoStiffD.Area;
	m_dIyy = CompoStiffD.Ryy;
	m_dIzz = CompoStiffD.Rzz;
	m_dYbar = CompoStiffD.Cym;
	m_dZbar = CompoStiffD.Czm;

	T_SECT_STIFFNESS CrackStiff = SectElem.SectPosi[nPosiId].NonConSect;
	m_dArea_cr = CrackStiff.Area;
	m_dIyy_cr = CrackStiff.Ryy;
	m_dIzz_cr = CrackStiff.Rzz;
	m_dYbar_cr = CrackStiff.Cym;
	m_dZbar_cr = CrackStiff.Czm;

	// Material
	m_pDataCtrl->Get_PlateGirderPointer()->Get_MaterialData(ElemK, m_dfy, m_dfy1, m_dEs, m_dfck, m_dEcm, m_dfsk, m_dErs);

	return TRUE;
}

CString CRptPlateGirder_EC4::Get_String4PartShape(int nPartType)
{
	CString strPartType;
	if (nPartType == 0) { strPartType = _DLS(IDS_CVL_RPT_EC_Plate_TF); }//_T("Top Flange");   }
	else if (nPartType == 1) { strPartType = _DLS(IDS_CVL_RPT_EC_Plate_BF); }//_T("Bottom Flange");}
	else if (nPartType == 2) { strPartType = _DLS(IDS_CVL_RPT_EC_Plate_Web); }//_T("Web");		   }
	else ASSERT(0);

	return strPartType;
}

CString CRptPlateGirder_EC4::Get_String4ShearLamdaw(BOOL bWeb, int iLamda_w_Type)
{
	CString strLamdawType;
	if (bWeb)
	{
		if (iLamda_w_Type == 0) strLamdawType = _T("Shear_buckling_Lamda_w_type1");
		else if (iLamda_w_Type == 1) strLamdawType = _T("Shear_buckling_Lamda_w_type2");
		else if (iLamda_w_Type == 2) strLamdawType = _T("Shear_buckling_Lamda_w_type3");
		else ASSERT(0);
	}
	else
	{
		if (iLamda_w_Type == 0) strLamdawType = _T("Shear_buckling_Lamda_flange_type1");
		else if (iLamda_w_Type == 1) strLamdawType = _T("Shear_buckling_Lamda_flange_type2");
		else if (iLamda_w_Type == 2) strLamdawType = _T("Shear_buckling_Lamda_flange_type3");
		else ASSERT(0);
	}

	return strLamdawType;
}

CString CRptPlateGirder_EC4::Get_String4ShearSubPanelLamdaw(BOOL bWeb, int iLamda_w_Type)
{
	CString strLamdawType;
	if (bWeb)
	{
		if (iLamda_w_Type == 0) strLamdawType = _T("Shear_buckling_Lamda_w_sub_type1");
		else if (iLamda_w_Type == 1) strLamdawType = _T("Shear_buckling_Lamda_w_sub_type2");
		else if (iLamda_w_Type == 2) strLamdawType = _T("Shear_buckling_Lamda_w_sub_type3");
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


CString CRptPlateGirder_EC4::Get_String4ShearXwType(BOOL bWeb, int iEndSupportType, int iLamda_w_scope)
{
	CString strXwType;
	if (bWeb)
	{
		if (iEndSupportType == 0) // Rigid end post
		{
			if (iLamda_w_scope == 0) { strXwType = _T("Shear_buckling_Xw_Rigid_end_post_type1"); }
			else if (iLamda_w_scope == 1) { strXwType = _T("Shear_buckling_Xw_Rigid_end_post_type2"); }
			else if (iLamda_w_scope == 2) { strXwType = _T("Shear_buckling_Xw_Rigid_end_post_type3"); }
			else ASSERT(0);
		}
		else if (iEndSupportType == 1 || iEndSupportType == 2)
		{
			if (iLamda_w_scope == 0) { strXwType = _T("Shear_buckling_Xw_Rigid_end_post_type1"); }
			else if (iLamda_w_scope == 1) { strXwType = _T("Shear_buckling_Xw_Rigid_end_post_type2"); }
			else if (iLamda_w_scope == 2) { strXwType = _T("Shear_buckling_Xw_Rigid_end_post_type4"); }
			else ASSERT(0);
		}
		else ASSERT(0);
	}
	else
	{
		if (iEndSupportType == 0) // Rigid end post
		{
			if (iLamda_w_scope == 0) { strXwType = _T("Shear_buckling_X_flange_Rigid_end_post_type1"); }
			else if (iLamda_w_scope == 1) { strXwType = _T("Shear_buckling_X_flange_Rigid_end_post_type2"); }
			else if (iLamda_w_scope == 2) { strXwType = _T("Shear_buckling_X_flange_Rigid_end_post_type3"); }
			else ASSERT(0);
		}
		else if (iEndSupportType == 1 || iEndSupportType == 2)
		{
			if (iLamda_w_scope == 0) { strXwType = _T("Shear_buckling_X_flange_Rigid_end_post_type1"); }
			else if (iLamda_w_scope == 1) { strXwType = _T("Shear_buckling_X_flange_Rigid_end_post_type2"); }
			else if (iLamda_w_scope == 2) { strXwType = _T("Shear_buckling_X_flange_Rigid_end_post_type4"); }
			else ASSERT(0);
		}
		else ASSERT(0);
	}

	return strXwType;
}

CString CRptPlateGirder_EC4::GetLoadCombinationName(int OrgLcomK)
{
	CString strLcomName = _T("");
	T_LCOM_D LcomData;
	if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, OrgLcomK, LcomData))
	{
		strLcomName = LcomData.LoadCombName;
	}
	return strLcomName;
}

CString CRptPlateGirder_EC4::GetStressPosition(int iPosition)
{
	CString strPosi; strPosi.Empty();

	switch (iPosition)
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

void CRptPlateGirder_EC4::SetMultiLang(CMSExcel* pExcel, const int& stRow, const int& stCol, CString strVal, BOOL bSubSupType)
{
	// Unicode -> Multibyte 변환시 system(OS) ANSI를 따라서 OS 언어에 따라 해당 국가 언어가 아닌 경우 깨지는 문제가 있음.
	// 그래서 일단 막음. 추후 Gen/Civil이 Unicode 변경되면 활용 가능
	return;

	if (m_pDoc->GetCivilCodeDgn() != EUROCODE4_2_04_CSG) return;

	if (bSubSupType)
	{
		pExcel->SetValueSubSupscriptParser(stRow, stCol, strVal);
	}
	else
	{
		pExcel->SetValue(stRow, stCol, strVal);
	}
}