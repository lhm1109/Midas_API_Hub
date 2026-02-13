// DgnJTG_D62_04.cpp: implementation of the CDgnJTG_D62_04 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnJTG_D62_04.h"
//#include "RCDataCtrl_CH.h"

#include "CRCForceCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\MIT_Lib\rptostream.h"
#include "DesignForceResult.h"
#include "..\..\dgnengine\src/CVL_DgnCalc_CH/DgnCalc_CH_PSC_Struct.h"
#include "CH_DGN_INTERFACE_UNICODE.h"

using namespace std;
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnJTG_D62_04::CDgnJTG_D62_04()
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	m_dZero = 1.0E-07;
	m_bReport = FALSE;

	m_RptBdcy.Initialize();
	m_RptCums.Initialize();
	m_RptCrmt.Initialize();

	// Coded by Seungjun for Test. ('20060719)
	CString csDesignOutput = CProduct::GetTestEnvValue(_ULS(Design Output));
	m_bTestMode = csDesignOutput == _ULS(yes) ? TRUE : FALSE;

	//coded by maxiao for dgnCH.dll
	CString strCalcDll = Get_DllFileName(JTG_D62_04);
	if ((m_hDllCalc = LoadLibrary(strCalcDll)) == NULL)
	{
		AfxMessageBox(_T("Can't Find CVL_DgnCalc_CH.dll"));
	}
}

CDgnJTG_D62_04::~CDgnJTG_D62_04()
{
}

void CDgnJTG_D62_04::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
{
	CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	// Set Code Unit.
	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
	CodeUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
	// m_pDataCtrl->Set_UnitIndex(CodeUnit);	
	m_pDataCtrl->Set_UnitIndex(CodeUnit);	 //kyungSik

	//m_Print.SetDataCtrlPointer(m_pDataCtrl,m_pForcCtrl);

	Make_PscDgnData();
}

//CRptJTG_D62_04* CDgnJTG_D62_04::Get_RptJtgPointer()	{return &m_Print;}

void CDgnJTG_D62_04::Set_ReportFlag(BOOL bReport) { m_bReport = bReport; }


BOOL CDgnJTG_D62_04::Get_RptBdcy(_RPT_BDCY_JTG& RptBdcy)
{
	RptBdcy = m_RptBdcy;
	return TRUE;
}

BOOL CDgnJTG_D62_04::Get_RptCums(_RPT_CUMS_JTG& RptCums)
{
	RptCums = m_RptCums;
	return TRUE;
}

BOOL CDgnJTG_D62_04::Get_RptCrmt(_RPT_CRMT_JTG& RptCrmt)
{
	RptCrmt = m_RptCrmt;
	return TRUE;
}

BOOL CDgnJTG_D62_04::Get_MatlTndn(ElemPairK ElemK, double& dfcpd, double& dftpd)
{
	// Always Code Units (N,mm).
	dfcpd = 0.0;
	dftpd = 0.0;
	////////////////////////////
	int nMatlK = CDBLib::GetMaterialKey4ElemPairK(ElemK);

	// Get Matl Data.
	T_MATD_D MatdD;
	MatdD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(nMatlK, MatdD)) { ASSERT(0); return FALSE; }
	if (MatdD.Data1.CodeName != MATLCODE_STL_JTG04) { ASSERT(0); return FALSE; }
	// See JTG D62-04 Table 3.2.4-2 fpd,f'pd(프리스트레스철근 인장,압축설계값).
	if (MatdD.Data1.CodeMatlName == _T("Strand1470")) { dfcpd = 1000.;  dftpd = 390.; }
	else if (MatdD.Data1.CodeMatlName == _T("Strand1570")) { dfcpd = 1070.;  dftpd = 390.; }
	else if (MatdD.Data1.CodeMatlName == _T("Strand1720")) { dfcpd = 1170.;  dftpd = 390.; }
	else if (MatdD.Data1.CodeMatlName == _T("Strand1860")) { dfcpd = 1260.;  dftpd = 390.; }
	else if (MatdD.Data1.CodeMatlName == _T("Wire1470")) { dfcpd = 1000.;  dftpd = 410.; }
	else if (MatdD.Data1.CodeMatlName == _T("Wire1570")) { dfcpd = 1070.;  dftpd = 410.; }
	else if (MatdD.Data1.CodeMatlName == _T("Wire1670")) { dfcpd = 1140.;  dftpd = 410.; }
	else if (MatdD.Data1.CodeMatlName == _T("Wire1770")) { dfcpd = 1200.;  dftpd = 410.; }
	else if (MatdD.Data1.CodeMatlName == _T("Steelbar540")) { dfcpd = 450.;  dftpd = 400.; }
	else if (MatdD.Data1.CodeMatlName == _T("Steelbar785")) { dfcpd = 650.;  dftpd = 400.; }
	else if (MatdD.Data1.CodeMatlName == _T("Steelbar930")) { dfcpd = 770.;  dftpd = 400.; }
	else { ASSERT(0);	return FALSE; }

	return TRUE;
}

BOOL CDgnJTG_D62_04::Get_MatlConc(ElemPairK ElemK, double& dfck, double& dftk, double& dfcd, double& dftd, double& dGc)
{
	// Always Code Units (N,mm).
	dfck = 0.0;
	dftk = 0.0;
	dfcd = 0.0;
	dftd = 0.0;
	dGc = 0.0;
	////////////////////////////
	int nMatlK = CDBLib::GetMaterialKey4ElemPairK(ElemK);

	// Get Matl Data.
	T_MATD_D MatdD;
	MatdD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(nMatlK, MatdD)) { ASSERT(0); return FALSE; }
	if (MatdD.Data1.CodeName != MATLCODE_CON_JTG04) { ASSERT(0); return FALSE; }
	// See JTG D62-04 Table 3.1.5 fck,ftk(콘크리트압축,인장표준값), 3.1.6 fcd,ftd(콘크리트압축,인장설계값).
	if (MatdD.Data1.CodeMatlName == _T("C15")) { dfck = 10.0; dftk = 1.27; dfcd = 6.9; dftd = 0.88; }
	else if (MatdD.Data1.CodeMatlName == _T("C20")) { dfck = 13.4; dftk = 1.54; dfcd = 9.2; dftd = 1.06; }
	else if (MatdD.Data1.CodeMatlName == _T("C25")) { dfck = 16.7; dftk = 1.78; dfcd = 11.5; dftd = 1.23; }
	else if (MatdD.Data1.CodeMatlName == _T("C30")) { dfck = 20.1; dftk = 2.01; dfcd = 13.8; dftd = 1.39; }
	else if (MatdD.Data1.CodeMatlName == _T("C35")) { dfck = 23.4; dftk = 2.20; dfcd = 16.1; dftd = 1.52; }
	else if (MatdD.Data1.CodeMatlName == _T("C40")) { dfck = 26.8; dftk = 2.40; dfcd = 18.4; dftd = 1.65; }
	else if (MatdD.Data1.CodeMatlName == _T("C45")) { dfck = 29.6; dftk = 2.51; dfcd = 20.5; dftd = 1.74; }
	else if (MatdD.Data1.CodeMatlName == _T("C50")) { dfck = 32.4; dftk = 2.65; dfcd = 22.4; dftd = 1.83; }
	else if (MatdD.Data1.CodeMatlName == _T("C55")) { dfck = 35.5; dftk = 2.74; dfcd = 24.4; dftd = 1.89; }
	else if (MatdD.Data1.CodeMatlName == _T("C60")) { dfck = 38.5; dftk = 2.85; dfcd = 26.5; dftd = 1.96; }
	else if (MatdD.Data1.CodeMatlName == _T("C65")) { dfck = 41.5; dftk = 2.93; dfcd = 28.5; dftd = 2.02; }
	else if (MatdD.Data1.CodeMatlName == _T("C70")) { dfck = 44.5; dftk = 3.00; dfcd = 30.5; dftd = 2.07; }
	else if (MatdD.Data1.CodeMatlName == _T("C75")) { dfck = 47.4; dftk = 3.05; dfcd = 32.4; dftd = 2.10; }
	else if (MatdD.Data1.CodeMatlName == _T("C80")) { dfck = 50.2; dftk = 3.10; dfcd = 34.6; dftd = 2.14; }
	else { ASSERT(0);	return FALSE; }
	// See JTG D62-04 3.1.7 Gc(콘크리트전단변형계수).
	dGc = 0.4 * MatdD.Data1.Analysis.Elast;

	////////////////////////////
	// Get Sect Data.
	T_SECT_D SectD;
	SectD.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetSectPostDgn(ElemK, SectD)) { ASSERT(0); return FALSE; }
	double dCyp[2] = { SectD.SectBefore.SectI.Stiffness.Cyp, SectD.SectBefore.SectJ.Stiffness.Cyp };
	double dCym[2] = { SectD.SectBefore.SectI.Stiffness.Cym, SectD.SectBefore.SectJ.Stiffness.Cym };
	double dCzp[2] = { SectD.SectBefore.SectI.Stiffness.Czp, SectD.SectBefore.SectJ.Stiffness.Czp };
	double dCzm[2] = { SectD.SectBefore.SectI.Stiffness.Czm, SectD.SectBefore.SectJ.Stiffness.Czm };
	double dyDim = max(dCyp[0] + dCym[0], dCyp[1] + dCym[1]);
	double dzDim = max(dCzp[0] + dCzm[0], dCzp[1] + dCzm[1]);
	// See JTG D62-04 Table 3.1.6 주.
	if (max(dyDim, dzDim) < 300. + m_dZero) { dfcd *= 0.8; dftd *= 0.8; }

	return TRUE;
}

CString CDgnJTG_D62_04::Get_DllFileName(int iCode)
{
	BOOL bDebug = FALSE;
	CString strDllName = _T("");

#if defined(_DEBUG)
	bDebug = TRUE;
#endif

	if (iCode == JTG_D62_04)
	{
		strDllName = bDebug ? _T("CVL_DgnCalc_CHD.dgne") : _T("CVL_DgnCalc_CH.dgne");
	}
	else
		ASSERT(0);
	return strDllName;
}

//  1. 시공단계 합성응력검토
BOOL CDgnJTG_D62_04::Get_JtgFysg(ElemPairK ElemK, T_FYSG_D& FysgD)
{
	BOOL bCalcOK = TRUE;

	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode == CJJ_11_2011 && PscdD.bExistSpecialLoad)
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_BeforeAllowbleStressofConc_CJJ"));
	else
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_BeforeAllowbleStressofConc"));
	//pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_BeforeAllowbleStressofConc")); 
	size_t sizeIn, sizeResu;
	sizeIn = sizeof(_DGN_FYSG_D_W);
	sizeResu = sizeof(T_FYSG_BASE);
	//==================================================================================================

	int i = 0;
	// Material.
	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if (!Get_PscSectElem(FALSE, ElemK, SectElem))
	{
		return FALSE;
	}

	_PSC_SECG_ELEM SecgElem;
	if (!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK = FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if (!Get_PscRbarElem(FALSE, ElemK, RbarElem)) bCalcOK = FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if (!Get_PscTndnElem(FALSE, ElemK, TndnElem)) bCalcOK = FALSE;

	double dMatFactor = Get_MatlFactor();
	double dfck_p = MatlElem.dfckp;
	double dftk_p = MatlElem.dftkp;

	// Tendon
	CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;
	arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
	for (i = 0; i < 2; i++)  // 0=I, 1=J.
	{
		BOOL bPre = TRUE;  // TRUE=Pre-tension, FALSE=Post-tension  
		BOOL bPosiI = (i == 0 ? TRUE : FALSE);
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		// Section_g
		T_SPSC_SECT SpsgSect = SecgElem.SectPosi[i];
		// RBar
		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		// Tendon
		CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		int iPreCount = 0;
		for (int k = 0; k < arTndnPosi.GetSize(); k++)
		{
			_DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(k);
			int iLoadType = rTndn.iLoadType; // 0:Pre-tension, 1:Post-tension
			if (iLoadType == 0) iPreCount++;
		}
		if (iPreCount >= arTndnPosi.GetSize() / 2) bPre = TRUE;
		else                                 bPre = FALSE;

		double dAg = SpsgSect.dArea;
		for (int j = 0; j < 2; j++)  // 0=MAX, 1=MIN.
		{
			if (FysgD.FysgBase[i * 2 + j].bCHK == FALSE)  continue;

			//==================================================================================================
			_DGN_FYSG_D_W DgnFysgD;
			DgnFysgD.Initialize();
			DgnFysgD.bPre = bPre;
			DgnFysgD.dfck_p = dfck_p;
			DgnFysgD.dftk_p = dftk_p;
			DgnFysgD.dAg = dAg;
			DgnFysgD.j = j;
			DgnFysgD.SpscSect = WA_Convert::Convert_A2W(SpscSect);

			if (arRbarPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arRbarPosi, DgnFysgD.arRbarPosi);
			if (arTndnPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arTndnPosi, DgnFysgD.arTndnPosi);

			T_DETAIL_STR DetailStr; DetailStr.Initialize();
			BOOL bVersionOK = FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnFysgD, &FysgD.FysgBase[i * 2 + j], &DetailStr, sizeIn, sizeResu, bVersionOK);
			if (!bVersionOK)
			{
				AfxMessageBox(_T("Conversion of Design data failed"));
				continue;
			}
			if (!bCheck) continue;
			//==================================================================================================
		}
	}
	return bCalcOK;
}

//  2. 정상사용극한상태 PC강재 최대응력도 (텐던별)
BOOL CDgnJTG_D62_04::Get_JtgFypc(T_TDNA_K TdnaK, T_FYPC_JTG& FypcJtg)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode == CJJ_11_2011 && PscdD.bExistSpecialLoad)
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_AllowbleStressofPC_CJJ"));
	else
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_AllowbleStressofPC"));

	//pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_AllowbleStressofPC")); 
	size_t sizeIn, sizeResu;
	sizeIn = sizeof(_DGN_FYPC_D_W);
	sizeResu = sizeof(T_FYPC_JTG);
	//==================================================================================================

	//==================================================================================================
	_DGN_FYPC_D_W DgnFypcD;
	DgnFypcD.Initialize();
	if (!Get_PscTndnTdna(TdnaK, DgnFypcD.tnsn))
	{
		return FALSE;
	}
	T_DETAIL_STR DetailStr; DetailStr.Initialize();
	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*pDgnCalc)(&DgnFypcD, &FypcJtg, &DetailStr, sizeIn, sizeResu, bVersionOK);
	if (!bVersionOK)
	{
		AfxMessageBox(_T("Conversion of Design data failed"));
		return FALSE;
	}
	if (!bCheck)
	{
		// CodeUnit -> CurrUnit.
		return FALSE;
	}
	//==================================================================================================

	return TRUE;
}

BOOL CDgnJTG_D62_04::Get_JtgFypc(CMap<T_TDNA_K, T_TDNA_K, T_FYPC_JTG, T_FYPC_JTG>& mapFypcJtg)
{
	//+++++++++++++++++++++++++++++++++++++++
	T_TDNA_K TdnaK;
	T_FYPC_JTG FypcJtgD;
	POSITION Pos = mapFypcJtg.GetStartPosition();
	while (Pos)
	{
		FypcJtgD.Initialize();
		mapFypcJtg.GetNextAssoc(Pos, TdnaK, FypcJtgD);
		// 최대값을 찾기 위해.
		FypcJtgD.dFLL = -1. / pow(m_dZero, 7);
		mapFypcJtg.SetAt(TdnaK, FypcJtgD);
	}
	//+++++++++++++++++++++++++++++++++++++++

	// B type Tendon Stress를 계산하기 위해.////////////////
	CDgnPscCommon PscCommon;
	PscCommon.SetDataCtrlPointer(m_pDataCtrl);
	PscCommon.Make_PscDgnDataForJTG_TndnStress();
	////////////////////////////////////////////////////////

	int i, j, k;
	BOOL bCalcOK = TRUE;
	// 설계 가능한 전체 요소
	ArrElemPairKey arTotalElemK;
	arTotalElemK.RemoveAll();
	m_pDataCtrl->Get_ElemListForTendon(arTotalElemK);
	int iTotalElemNum = arTotalElemK.GetSize();

	double dMuI_Pmax = -1. / pow(m_dZero, 7);
	double dMuI_Nmin = +1. / pow(m_dZero, 7);
	double dMuJ_Pmax = -1. / pow(m_dZero, 7);
	double dMuJ_Nmin = +1. / pow(m_dZero, 7);

	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	for (i = 0; i < iTotalElemNum; i++)
	{
		auto ElemK = arTotalElemK[i];

		// Get Force
		CArray<_DGN_FORC_CRC, _DGN_FORC_CRC&> aForceLcom;
		// Change by Jaeoh. ('06.09.27).
		//if(!m_pForcCtrl->Get_LcomDataForJtgTndn(ElemK,FALSE,aForceLcom))	continue;	// TRUE=Include Tendon Primary.
		if (!m_pForcCtrl->Get_LcomDataForDesign(ElemK, 0/*nFortype*/, TRUE, FALSE, aForceLcom, iLcomNum)) continue;

		for (j = 0; j < aForceLcom.GetSize(); j++)
		{
			// Check 탄성단계.
			if (!m_pForcCtrl->Is_LcomForElst(j + 1)) continue;
			for (k = 0; k < 2; k++) // I,J
			{
				double dMu = aForceLcom[j].dMuy[k];
				if (k == 0) // I
				{
					if (dMuI_Pmax < dMu) dMuI_Pmax = dMu;
					if (dMuI_Nmin > dMu) dMuI_Nmin = dMu;
				}
				else if (k == 1) // J
				{
					if (dMuJ_Pmax < dMu) dMuJ_Pmax = dMu;
					if (dMuJ_Nmin > dMu) dMuJ_Nmin = dMu;
				}
			}
		}
		// Get Data
		_PSC_MATL_ELEM MatlElem;
		if (!PscCommon.Get_PscMatlElem(ElemK, MatlElem)) continue;
		_PSC_SECT_ELEM SectElem;
		if (!PscCommon.Get_PscSectElem(TRUE, ElemK, SectElem)) continue;
		_PSC_SECG_ELEM SecgElem;
		if (!PscCommon.Get_PscSecgElem(ElemK, SecgElem)) bCalcOK = FALSE;
		_PSC_RBAR_ELEM RbarElem;
		if (!PscCommon.Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK = FALSE;
		//_PSC_TNDN_ELEM TndnElem;    
		//if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;

		double dfcd = MatlElem.dfcd;
		double dftd = MatlElem.dftd;
		for (j = 0; j < 2; j++) // I,J
		{
			BOOL bPosiI = (j == 0 ? TRUE : FALSE);
			BOOL bRebarExist = TRUE;
			BOOL bTndnExist = TRUE;
			// Section
			T_SPSC_SECT SpscSect = SectElem.SectPosi[j].SpscSect;
			CArray<T_SPSC_COOR, T_SPSC_COOR> arOuter;
			arOuter.Copy(SectElem.SectPosi[j].arOuter);
			CArray<T_SPSC_COOR_AR*, T_SPSC_COOR_AR*> arpInner;
			int iInnerCellNum = SectElem.SectPosi[j].iInnerCellNum;
			for (k = 0; k < iInnerCellNum; k++)
			{
				CArray<T_SPSC_COOR, T_SPSC_COOR> arInner;
				arInner.Copy(SectElem.SectPosi[j].arInner[k]);
				arpInner.Add(&arInner);
			}

			// RBar
			CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
			arRbarPosi.Copy(RbarElem.RbarPosi[j].arRbarPosi);
			// Tendon
			BOOL bIsGrup = FALSE;	// Grup (NOT Sngl).
			double dMuyMax = 0.0, dMuyMin = 0.0;
			CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;
			if (!m_pDataCtrl->Get_ConTndn(ElemK, bPosiI, bIsGrup, dMuyMax, dMuyMin, arTndnPosi)) bTndnExist = FALSE;

			double dMuP = 0.0, dMuN = 0.0;
			dMuP = j == 0 ? dMuI_Pmax : dMuJ_Pmax;
			dMuN = j == 0 ? dMuI_Nmin : dMuJ_Nmin;
			// No tendon
			if (arTndnPosi.GetSize() == 0) bTndnExist = FALSE;
			// bw
			double dShrThk[3];
			dShrThk[0] = SectElem.SectPosi[j].dpShrThk[0];
			dShrThk[1] = SectElem.SectPosi[j].dpShrThk[1];
			dShrThk[2] = SectElem.SectPosi[j].dpShrThk[2];
			double dbw = min(min(dShrThk[0], dShrThk[1]), dShrThk[2]);
			double dFlgThk[2] = { SectElem.SectPosi[j].dpFlgThk[0], SectElem.SectPosi[j].dpFlgThk[1] };
			double dpMomPos[2][2];
			dpMomPos[0][0] = SectElem.SectPosi[j].dpMomPos[2][0];
			dpMomPos[0][1] = SectElem.SectPosi[j].dpMomPos[2][1];
			dpMomPos[1][0] = SectElem.SectPosi[j].dpMomPos[3][0];
			dpMomPos[1][1] = SectElem.SectPosi[j].dpMomPos[3][1];
			// if no tendon, Skip
			if (!bTndnExist) continue;

			for (k = 0; k < 2; k++) // Positive, Negative
			{
				BOOL bPosiMom = k == 0 ? TRUE : FALSE;
				double dMuy = 0.0;
				if (k == 0)
				{
					bPosiMom = TRUE;
					dMuy = dMuP > 0.0 ? dMuP : 0.0; // 정모멘트 계산 때 부모멘트면 계산할 필요가 없다.          
				}
				else
				{
					bPosiMom = FALSE;
					dMuy = dMuN < 0.0 ? dMuN : 0.0; // 부모멘트 계산 때 정모멘트면 계산할 필요가 없다.
				}
				CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arCalcTndnPosi;
				arCalcTndnPosi.RemoveAll();

				if (Get_TendonStress(ElemK, bPosiMom, dMuy, dfcd, dftd, dbw, dFlgThk, dpMomPos, MatlElem, arOuter, arpInner, SpscSect, arRbarPosi, arTndnPosi, arCalcTndnPosi))
				{
					for (int m = 0; m < arCalcTndnPosi.GetSize(); m++)
					{
						T_TDNA_K TdnaKey = arCalcTndnPosi[m].KeyTdna;
						double dStressLL = arCalcTndnPosi[m].dStressLL;
						T_FYPC_JTG FypcData; FypcData.Initialize();
						if (mapFypcJtg.Lookup(TdnaKey, FypcData))
						{
							if (FypcData.dFLL < dStressLL)
							{
								FypcData.dFLL = dStressLL;
								mapFypcJtg.SetAt(TdnaKey, FypcData);
							}
						}
					} // m
				}

			} // k

			// Delete because of New Operator.
			for (int k = 0; k < arpInner.GetSize(); k++)
			{
				if (arpInner[k])
				{
					arpInner[k]->RemoveAll();
					delete arpInner[k];
					arpInner[k] = NULL;
				}
			}
			arpInner.RemoveAll();

		} // j
	} // i

	Pos = mapFypcJtg.GetStartPosition();
	while (Pos)
	{
		FypcJtgD.Initialize();
		mapFypcJtg.GetNextAssoc(Pos, TdnaK, FypcJtgD);
		// 허용응력과 비교.
		Get_AllowbleStressofPC(TdnaK, FypcJtgD);
		// CodeUnit -> CurrUnit.
		mapFypcJtg.SetAt(TdnaK, FypcJtgD);
	}

	//!//////////////////////
	// FypcData.dFLL는 테이블에서 -10^-30 보다 작으면 설계 안된 것으로 간주하고 _T("-") 표시하는 걸로 작업해야함.
	////////////////////////

	/*
	if(!Get_AllowbleStressofPC(TdnaK, FypcJtg))
	{
	// CodeUnit -> CurrUnit.
	m_pDataCtrl->Get_JtgFypcCodeToCurr(FypcJtg);
	return FALSE;
	}

	//+++++++++++++++++++++++++++++++++++++++
	// CodeUnit -> CurrUnit.
	m_pDataCtrl->Get_JtgFypcCodeToCurr(FypcJtg);
	//+++++++++++++++++++++++++++++++++++++++
	*/

	return TRUE;
}

//  3. 정상사용극한상태 합성응력도
BOOL CDgnJTG_D62_04::Get_JtgFycm(ElemPairK ElemK, T_FYCM_JTG& FycmJtg, T_DETAIL_STR& DetailStr)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode == CJJ_11_2011 && PscdD.bExistSpecialLoad)
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_AfterAllowbleStressofConc_CJJ"));
	else
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_AfterAllowbleStressofConc"));

	size_t sizeIn, sizeResu;
	sizeIn = sizeof(_DGN_FYCM_D_W);
	sizeResu = sizeof(T_FYCM_JTG_BASE);
	//================================================================================================== 


	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	double dfck = MatlElem.dfck;
	double dftk = MatlElem.dftk;

	int i;
	for (i = 0; i < 2; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i == 0 ? TRUE : FALSE);

		for (int j = 0; j < 2; j++)  // 0=MAX, 1=MIN.
		{
			if (FycmJtg.FycmBase[i * 2 + j].bCHK == FALSE)  continue;
			//==================================================================================================
			_DGN_FYCM_D_W DgnFycmD;
			DgnFycmD.Initialize();
			DgnFycmD.dfck = dfck;
			DgnFycmD.dftk = dftk;
			DgnFycmD.j = j;
			DgnFycmD.ElemK = ElemK.first;
			DgnFycmD.iIJ = i * 2 + j;
			T_PSCD_D PscdD; PscdD.Initialize();
			m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
			DgnFycmD.iBrgType = PscdD.iBrgType;
			DgnFycmD.isServ = m_pForcCtrl->Is_LcomForServ(FycmJtg.FycmBase[i * 2 + j].LcomK);
			DgnFycmD.isLong = DgnFycmD.isServ ? m_pForcCtrl->Is_LongTerm(FycmJtg.FycmBase[i * 2 + j].LcomK) : FALSE;

			//      T_DETAIL_STR DetailStr; DetailStr.Initialize();
			BOOL bVersionOK = FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnFycmD, &FycmJtg.FycmBase[i * 2 + j], &DetailStr, sizeIn, sizeResu, bVersionOK);
			if (!bVersionOK)
			{
				AfxMessageBox(_T("Conversion of Design data failed"));
				continue;
			}
			if (!bCheck) continue;
			//==================================================================================================     
		}
	}

	if (PscdD.iBrgType == 1)
	{
		for (i = 0; i < 2; i++)  // 0=MAX, 1=MIN.
		{
			int k = i + 4;
			if (FycmJtg.FycmBase[k].bCHK == FALSE)  continue;
			//==================================================================================================
			_DGN_FYCM_D DgnFycmD;
			DgnFycmD.Initialize();
			DgnFycmD.dfck = dfck;
			DgnFycmD.dftk = dftk;
			DgnFycmD.j = 1;
			DgnFycmD.ElemK = ElemK.first;
			DgnFycmD.iIJ = i;
			T_PSCD_D PscdD; PscdD.Initialize();
			m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
			DgnFycmD.iBrgType = PscdD.iBrgType;
			DgnFycmD.isServ = m_pForcCtrl->Is_LcomForServ(FycmJtg.FycmBase[k].LcomK);
			DgnFycmD.isLong = DgnFycmD.isServ ? m_pForcCtrl->Is_LongTerm(FycmJtg.FycmBase[k].LcomK) : FALSE;

			//      T_DETAIL_STR DetailStr; DetailStr.Initialize();
			BOOL bVersionOK = FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnFycmD, &FycmJtg.FycmBase[k], &DetailStr, sizeIn, sizeResu, bVersionOK);
			if (!bVersionOK)
			{
				AfxMessageBox(_T("Conversion of Design data failed"));
				continue;
			}
			if (!bCheck) continue;
			//==================================================================================================     
		}
	}
	return bCalcOK;
}

//  4. 정상사용극한상태 주응력도 (전단력 최대시)
BOOL CDgnJTG_D62_04::Get_JtgFpms(ElemPairK ElemK, T_FPMS_JTG& FpmsJtg, T_DETAIL_STR& DetailStr)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode == CJJ_11_2011 && PscdD.bExistSpecialLoad)
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_AllowPrincipalStress_CJJ"));
	else
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_AllowPrincipalStress"));

	size_t sizeIn, sizeResu;
	sizeIn = sizeof(_DGN_FPMS_D_W);
	sizeResu = sizeof(T_FPMS_JTG_BASE);
	//==================================================================================================
	BOOL bCalcOK = TRUE;
	// Material.
	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if (!Get_PscSectElem(FALSE, ElemK, SectElem))
	{
		return FALSE;
	}
	_PSC_RBAR_ELEM RbarElem;
	if (!Get_PscRbarElem(FALSE, ElemK, RbarElem)) bCalcOK = FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if (!Get_PscRshrElem(FALSE, ElemK, RshrElem)) bCalcOK = FALSE;

	double dfck = MatlElem.dfck;
	double dftk = MatlElem.dftk;
	double dfskS = MatlElem.dfskS;

	int i;
	for (i = 0; i < 2; i++)  // 0=I, 1=J.
	{
		BOOL bPre = TRUE;
		BOOL bPosiI = (i == 0 ? TRUE : FALSE);
		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0], dShrThk[1]), dShrThk[2]);
		// Asv
		double dAsv = RshrElem.RshrPosi[i].RpscSbar.dSbarAw;
		// Sv
		double dSv = RshrElem.RshrPosi[i].RpscSbar.dSbarPitch;
		for (int j = 0; j < 2; j++)  // 0=MAX, 1=MIN.
		{
			if (FpmsJtg.FpmsBase[i * 2 + j].bCHK == FALSE)  continue;
			//==================================================================================================
			_DGN_FPMS_D_W DgnFpmsD;
			DgnFpmsD.Initialize();

			//			T_PSCD_D PscdD; PscdD.Initialize();
			//			m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
			DgnFpmsD.iBrgType = PscdD.iBrgType;
			DgnFpmsD.iCastType = PscdD.iCastType;
			DgnFpmsD.j = j;
			DgnFpmsD.ElemK = ElemK.first;
			DgnFpmsD.iIJ = i * 2 + j;
			DgnFpmsD.dfck = dfck;
			DgnFpmsD.dftk = dftk;
			DgnFpmsD.dfskS = dfskS;
			DgnFpmsD.dSig_tp = FpmsJtg.FpmsBase[i * 2 + j].dFMAX;
			DgnFpmsD.dbw = dbw;
			DgnFpmsD.dAsv = dAsv;
			DgnFpmsD.dSv = dSv;

			//      T_DETAIL_STR DetailStr; DetailStr.Initialize();
			BOOL bVersionOK = FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnFpmsD, &FpmsJtg.FpmsBase[i * 2 + j], &DetailStr, sizeIn, sizeResu, bVersionOK);
			if (!bVersionOK)
			{
				AfxMessageBox(_T("Conversion of Design data failed"));
				continue;
			}
			if (!bCheck) continue;
			//==================================================================================================
		}
	}
	return bCalcOK;
}

//  5. 정상사용극한상태 주응력도 (비틀림 최대시)
BOOL CDgnJTG_D62_04::Get_JtgFpmt(ElemPairK ElemK, T_FPMT_JTG& FpmtJtg)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode == CJJ_11_2011 && PscdD.bExistSpecialLoad)
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_AllowPrincipalStress2_CJJ"));
	else
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_AllowPrincipalStress2"));

	//pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_AllowPrincipalStress2")); 
	size_t sizeIn, sizeResu;
	sizeIn = sizeof(_DGN_FPMT_D_W);
	sizeResu = sizeof(T_FPMT_JTG_BASE);
	//==================================================================================================
	BOOL bCalcOK = TRUE;
	// Material.
	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if (!Get_PscSectElem(FALSE, ElemK, SectElem))
	{
		return FALSE;
	}
	_PSC_RBAR_ELEM RbarElem;
	if (!Get_PscRbarElem(FALSE, ElemK, RbarElem)) bCalcOK = FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if (!Get_PscRshrElem(FALSE, ElemK, RshrElem)) bCalcOK = FALSE;

	double dfck = MatlElem.dfck;
	double dftk = MatlElem.dftk;
	double dfskS = MatlElem.dfskS;

	int i;
	for (i = 0; i < 2; i++)  // 0=I, 1=J.
	{
		BOOL bPre = TRUE;
		BOOL bPosiI = (i == 0 ? TRUE : FALSE);
		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0], dShrThk[1]), dShrThk[2]);
		// Asv
		double dAsv = RshrElem.RshrPosi[i].RpscSbar.dSbarAw;
		// Sv
		double dSv = RshrElem.RshrPosi[i].RpscSbar.dSbarPitch;
		for (int j = 0; j < 2; j++)  // 0=MAX, 1=MIN.
		{
			if (FpmtJtg.FpmtBase[i * 2 + j].bCHK == FALSE)  continue;
			//==================================================================================================
			_DGN_FPMT_D_W DgnFpmtD;
			DgnFpmtD.Initialize();

			//			T_PSCD_D PscdD; PscdD.Initialize();
			//			m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
			DgnFpmtD.iBrgType = PscdD.iBrgType;
			DgnFpmtD.iCastType = PscdD.iCastType;
			DgnFpmtD.j = j;
			DgnFpmtD.ElemK = ElemK.first;
			DgnFpmtD.iIJ = i * 2 + j;
			DgnFpmtD.dfck = dfck;
			DgnFpmtD.dftk = dftk;
			DgnFpmtD.dfskS = dfskS;
			DgnFpmtD.dSig_tp = FpmtJtg.FpmtBase[i * 2 + j].dFMAX;
			DgnFpmtD.dbw = dbw;
			DgnFpmtD.dAsv = dAsv;
			DgnFpmtD.dSv = dSv;

			T_DETAIL_STR DetailStr; DetailStr.Initialize();
			BOOL bVersionOK = FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnFpmtD, &FpmtJtg.FpmtBase[i * 2 + j], &DetailStr, sizeIn, sizeResu, bVersionOK);
			if (!bVersionOK)
			{
				AfxMessageBox(_T("Conversion of Design data failed"));
				continue;
			}
			if (!bCheck) continue;
			//==================================================================================================   
		}
	}
	return bCalcOK;
}


//  6. 정상사용극한상태 휨 균열폭
BOOL CDgnJTG_D62_04::Get_JtgBdcw(ElemPairK ElemK, T_BDCW_D& BdcwD, T_DETAIL_STR& DetailStr)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode == CJJ_11_2011 && PscdD.bExistSpecialLoad)
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_CrackCheck_CJJ"));
	else
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_CrackCheck"));

	//pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_CrackCheck")); 
	size_t sizeIn, sizeResu;
	sizeIn = sizeof(_DGN_BDCW_D_W);
	sizeResu = sizeof(T_BDCW_BASE);
	//==================================================================================================
	BOOL bCalcOK = TRUE;
	// Get Crack Width.
	//T_PSCD_D PscdD; PscdD.Initialize();
	//m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if (!Get_PscSectElem(TRUE, ElemK, SectElem))
	{
		return FALSE;
	}
	_PSC_SECG_ELEM SecgElem;
	if (!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK = FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if (!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK = FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if (!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK = FALSE;

	//Material
	T_MATD_D MatdD = MatlElem.MatdD;

	// PSCW
	double dC1 = 1.0, dC2 = 1.0, dC3 = 1.0;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	T_PSCW_D PscwD;
	BOOL bUserInputPscw = pDoc->m_pAttrCtrl->GetPscw(ElemK.first, PscwD);
	if (bUserInputPscw)
	{
		dC1 = PscwD.dC1;
		dC2 = PscwD.dC2;
		dC3 = PscwD.dC3;
	}

	// Set Data.
	/*
	CArray<_DGN_FORC_CRC,_DGN_FORC_CRC> aForceLcom;
	CArray<_DGN_FORC_CRC,_DGN_FORC_CRC> aForceTnpr;
	CArray<_DGN_FORC_CRC,_DGN_FORC_CRC> aForceTnsc;
	if(!m_pForcCtrl->Get_LcomDataForDesign(ElemK, FALSE, FALSE, aForceLcom, aForceTnpr, aForceTnsc)) return FALSE;
	if(m_pForcCtrl->Get_LcomNumForStrs()==0)	return FALSE;
	if(aForceLcom.GetSize()!=aForceTnpr.GetSize())	{ASSERT(0); return FALSE;}
	// For Md.
	CArray<_DGN_FORC_CRC,_DGN_FORC_CRC> aForceLcomMd;
	if(!m_pForcCtrl->Get_LcomDataForDesign(ElemK, TRUE, FALSE, aForceLcomMd)) return FALSE;
	if(aForceLcom.GetSize()!=aForceLcomMd.GetSize())	{ASSERT(0); return FALSE;}

	// 응력계산을 위해.
	CArray<_DGN_STRS_CRC,_DGN_STRS_CRC> aStressLcom;
	CArray<_DGN_STRS_CRC,_DGN_STRS_CRC> aStressTnpr;
	if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressTnpr)) return FALSE;
	if(aStressLcom.GetSize()!=aStressTnpr.GetSize())  {ASSERT(0); return FALSE;}
	*/

	int iLcomNum = m_pForcCtrl->Get_LcomDataCount();

	int i = 0, j = 0, k = 0;
	double dMmaxI = -1. / pow(m_dZero, 40);
	double dMminI = 1. / pow(m_dZero, 40);
	double dMmaxJ = -1. / pow(m_dZero, 40);
	double dMminJ = 1. / pow(m_dZero, 40);
	double dM_IL[2] = { 0.0, 0.0 }; // Max,Min
	double dM_JL[2] = { 0.0, 0.0 };
	BOOL bIsLongTerm = FALSE;
	for (i = 0; i < iLcomNum; i++)
	{
		int iDgnLcomNo = i + 1;
		if (!m_pForcCtrl->Is_LcomForStrs(iDgnLcomNo))	continue;	// Only 정상사용(응력검토시.)
		if (m_pForcCtrl->Is_LcomForElst(iDgnLcomNo)) continue;
		if (!m_pForcCtrl->Is_LongTerm(iDgnLcomNo)) continue;  // Only Long Term.

		/*
		_DGN_FORC_CRC ForcLcom; ForcLcom.Initialize();
		_DGN_FORC_CRC ForceTnpr; ForceTnpr.Initialize();
		_DGN_FORC_CRC ForceTnsc; ForceTnsc.Initialize();
		ForcLcom = aForceLcom.GetAt(i);
		ForceTnpr = aForceTnpr.GetAt(i);
		ForceTnsc = aForceTnsc.GetAt(i);
		*/
		//
		_DGN_FORC_CRC ForcLcom;
		m_pDataCtrl->GetDesignForceResult()->GetForceLcomExTpResult(ElemK, iDgnLcomNo, ForcLcom);
		//
		_DGN_FORC_CRC ForceTnpr;
		m_pDataCtrl->GetDesignForceResult()->GetForceTnprResult(ElemK, iDgnLcomNo, ForceTnpr);
		//
		_DGN_FORC_CRC ForceTnsc;
		m_pDataCtrl->GetDesignForceResult()->GetForceTnscResult(ElemK, iDgnLcomNo, ForceTnsc);

		_DGN_FORC_CRC Force; Force.Initialize();
		for (j = 0; j < 2; j++)
		{
			Force.dMuy[j] = ForcLcom.dMuy[j] - ForceTnsc.dMuy[j]; // Ms = Md - M2nd (Md는 Primary 제외)    
			double dMom = Force.dMuy[j];  //Except Tendon Primary,  Ms+Mp2
			// End-I
			if (j == 0)
			{
				if (dMom > dMmaxI) { dMmaxI = dMom; dM_IL[0] = dMom; }
				if (dMom < dMminI) { dMminI = dMom; dM_IL[1] = dMom; }
			}
			// End-J
			else if (j == 1)
			{
				if (dMom > dMmaxJ) { dMmaxJ = dMom; dM_JL[0] = dMom; }
				if (dMom < dMminJ) { dMminJ = dMom; dM_JL[1] = dMom; }
			}
		}

		bIsLongTerm = TRUE;
	}


	BOOL bTitle = TRUE;
	for (i = 0; i < 2; i++)  // 0=I, 1=J
	{
		BOOL bPosiI = (i == 0 ? TRUE : FALSE);
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		// RBar
		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		BOOL bRebarExist = TRUE;
		if (arRbarPosi.GetSize() == 0)
		{
			bRebarExist = FALSE;
			CString sEndIJ = (bPosiI ? _T("I") : _T("J"));
			CString sErrMsg = _T("");
			sErrMsg.Format(_LS(IDS_DGN_PSC_NO_REBAR_Err), ElemK.first, sEndIJ);
			//GSaveHistoryNF(sErrMsg);    
		}
		BOOL bTndn = FALSE;
		int iMaxLoadType = TndnElem.TndnPosi[i].iMaxLoadType;
		if (iMaxLoadType == 0 || iMaxLoadType == 1) bTndn = TRUE;   //Strand or Wire.
		else if (iMaxLoadType == 2)               bTndn = FALSE;  //Steel Bar.


		int iCrackType;
		if (bTndn) iCrackType = PscdD.iCrackWidth[1];
		else      iCrackType = PscdD.iCrackWidth[0] + 2;

		// Tendon
		CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0], dShrThk[1]), dShrThk[2]);
		double dFlgThk[2] = { SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1] };
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];

		for (j = 0; j < 2; j++) // 0=Top, 1=Bot
		{
			int m = i * 2 + j;
			if (!bRebarExist) BdcwD.BdcwBase[m].bCHK = FALSE;
			if (BdcwD.BdcwBase[m].bCHK == FALSE) continue;

			double dPrtMd = 0., dPrtMs = 0., dPrtMl = 0., dPrtM_exTp = 0.;
			double dPrtFT = 0., dPrtFB = 0.;

			double dWcMax = -1. / pow(m_dZero, 40);
			// For zzzDataForDgn_JTG_D62_04.txt
			if (m_bTestMode)
			{
				const auto it = m_aSelElemK.find(ElemK);
				if (it != m_aSelElemK.end())
				{
					rptwofstream fout(m_strTestDgnFile, ios::app);

					CString strTitle; strTitle.Format(_T("%s"), (bTitle ? _T("▷ Crack Breadth(사용단계 균열폭검토)") : _T("")));
					bTitle = FALSE;
					CString strElem;	strElem.Format(_T("%6d  (%s)  (%s)"), ElemK, (i == 0 ? _T("I") : _T("J")), (j == 0 ? _T("Top") : _T("Bottom")));
					fout << endl
						<< strTitle << endl
						<< _T(" Elem  Posi  Top/Bottom : ") << strElem << endl
						<< endl;
				}
			}

			for (k = 0; k < iLcomNum; k++)
			{
				int iDgnLcomNo = k + 1;
				if (!m_pForcCtrl->Is_LcomForStrs(iDgnLcomNo))	continue;	// Only 정상사용(응력검토시.)
				if (m_pForcCtrl->Is_LcomForElst(iDgnLcomNo)) continue;
				if (m_pForcCtrl->Is_LongTerm(iDgnLcomNo)) continue;

				// Get LcomKind.
				int iDgnLcomKind = m_pForcCtrl->Get_LcomKind(iDgnLcomNo);
				// Get LcomType.
				CString strOrgLcomNa = _T("");
				int iMaxMinType = 0;
				if (!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iMaxMinType))	ASSERT(0);

				_DGN_FORC_CRC ForcLcom; ForcLcom.Initialize();
				_DGN_FORC_CRC ForceTnpr; ForceTnpr.Initialize();
				_DGN_FORC_CRC ForceTnsc; ForceTnsc.Initialize();
				_DGN_FORC_CRC ForcLcomMd; ForcLcomMd.Initialize();
				_DGN_FORC_CRC Force; Force.Initialize();
				_DGN_STRS_CRC StressLcom; StressLcom.Initialize();
				//
				m_pDataCtrl->GetDesignForceResult()->GetForceLcomExTpResult(ElemK, iDgnLcomNo, ForcLcom);
				//        
				m_pDataCtrl->GetDesignForceResult()->GetForceTnprResult(ElemK, iDgnLcomNo, ForceTnpr);
				//        
				m_pDataCtrl->GetDesignForceResult()->GetForceTnscResult(ElemK, iDgnLcomNo, ForceTnsc);
				//
				m_pDataCtrl->GetDesignForceResult()->GetForceLcomResult(ElemK, iDgnLcomNo, ForcLcomMd);
				//
				m_pDataCtrl->GetDesignForceResult()->GetStressLcomResult(ElemK, iDgnLcomNo, StressLcom);

				Force.dMuy[i] = ForcLcom.dMuy[i] - ForceTnsc.dMuy[i];
				double dM_exTp = ForcLcom.dMuy[i];  //Except Tendon Primary,  Ms+Mp2
				double dMs = Force.dMuy[i];
				double dMl = 0.0;
				double dMd = ForcLcomMd.dMuy[i]; //; // Include TP
				if (bIsLongTerm)
				{
					if (i == 0) dMl = dM_IL[1 - j]; // Nega, Posi
					else if (i == 1) dMl = dM_JL[1 - j];
				}

				// Change, Jaeoh('07.03.28) 
				// aStressLcom.GetAt(i) -> StressLcom , i가 아니라 k이어야 했음.        
				_DGN_STRS_CRC StrsMax;
				_DGN_STRS_CRC StrsMin;
				StrsMax.Initialize();
				StrsMin.Initialize();
				if (!m_pForcCtrl->Get_StrsByStrs(ElemK, iDgnLcomNo, StressLcom, StrsMax, StrsMin))	continue;
				double dFT = StrsMin.dStop[i];
				double dFB = StrsMin.dSbot[i];
				////////////////////////////////////////////////////////////////////////////
				if (!bUserInputPscw)
				{
					Get_CrackCheckParam(dMs, dMl, dC1, dC2, dC3);
				}
				BOOL bPrint = FALSE;
				//==================================================================================================
				_DGN_BDCW_D_W DgnBdcwD;
				DgnBdcwD.Initialize();
				DgnBdcwD.bPrint = bPrint;
				DgnBdcwD.m_bTestMode = m_bTestMode;
				DgnBdcwD.m_strTestDgnFile = m_strTestDgnFile;
				const auto& itSelElem = m_aSelElemK.find(ElemK);
				DgnBdcwD.bSelElemK = (itSelElem != m_aSelElemK.end());
				DgnBdcwD.ElemK = ElemK.first;
				DgnBdcwD.iTopBot = j;
				DgnBdcwD.iCrackType = iCrackType;
				DgnBdcwD.dMd = dMd;
				DgnBdcwD.dMs = dMs;
				DgnBdcwD.dMl = dMl;
				DgnBdcwD.dMd_extp = dM_exTp;
				DgnBdcwD.dShrThk = dbw;
				DgnBdcwD.dFlgThk[0] = dFlgThk[0];
				DgnBdcwD.dFlgThk[1] = dFlgThk[1];
				DgnBdcwD.dpMomPos[0][0] = dpMomPos[0][0];
				DgnBdcwD.dpMomPos[0][1] = dpMomPos[0][1];
				DgnBdcwD.dpMomPos[1][0] = dpMomPos[1][0];
				DgnBdcwD.dpMomPos[1][1] = dpMomPos[1][1];
				DgnBdcwD.dC1 = dC1;
				DgnBdcwD.dC2 = dC2;
				DgnBdcwD.dC3 = dC3;
				DgnBdcwD.MatlElem = WA_Convert::Convert_A2W(MatlElem);
				DgnBdcwD.SpscSect = WA_Convert::Convert_A2W(SpscSect);
				if (arRbarPosi.GetSize() > 0)
					WA_Convert::Convert_A2W(arRbarPosi, DgnBdcwD.arRbarPosi);
				if (arTndnPosi.GetSize() > 0)
					WA_Convert::Convert_A2W(arTndnPosi, DgnBdcwD.arTndnPosi);

				DgnBdcwD.dFT = dFT;
				DgnBdcwD.dFB = dFB;

				//				T_DETAIL_STR DetailStr; DetailStr.Initialize();
				T_BDCW_BASE base;
				BOOL bVersionOK = FALSE;
				BOOL bCheck = (*pDgnCalc)(&DgnBdcwD, &base, &DetailStr, sizeIn, sizeResu, bVersionOK);
				if (!bVersionOK)
				{
					AfxMessageBox(_T("Conversion of Design data failed"));
					continue;
				}
				if (!bCheck) continue;
				//==================================================================================================

				if (base.dWC > dWcMax)
				{
					dWcMax = base.dWC;

					//BdcwD.BdcwBase[m].bCHK	= m_pDataCtrl->Get_ChkFlag(ElemK, i, TRUE);
					BdcwD.BdcwBase[m].LcomK = iDgnLcomNo;
					BdcwD.BdcwBase[m].iKind = m_pForcCtrl->Get_LcomKind(iDgnLcomNo);
					BdcwD.BdcwBase[m].iMax = iMaxMinType;
					BdcwD.BdcwBase[m].dFT = dFT;
					BdcwD.BdcwBase[m].dFB = dFB;
					BdcwD.BdcwBase[m].dFRT = base.dFRT;
					BdcwD.BdcwBase[m].dWC = base.dWC;
					BdcwD.BdcwBase[m].dAWC = base.dAWC;

					// For Print 
					dPrtMd = dMd; dPrtMs = dMs; dPrtMl = dMl; dPrtM_exTp = dM_exTp;
					dPrtFT = dFT; dPrtFB = dFB;
				}
			}

			double dSigss = 0.0, dwf = 0.0, dwa = 0.0;
			BOOL bPrint = TRUE;
			//==================================================================================================
			_DGN_BDCW_D_W DgnBdcwD;
			DgnBdcwD.Initialize();
			DgnBdcwD.bPrint = bPrint;
			DgnBdcwD.m_bTestMode = m_bTestMode;
			DgnBdcwD.m_strTestDgnFile = CT2W(m_strTestDgnFile);
			const auto& itSelElem = m_aSelElemK.find(ElemK);
			DgnBdcwD.bSelElemK = (itSelElem != m_aSelElemK.end());
			DgnBdcwD.ElemK = ElemK.first;
			DgnBdcwD.iTopBot = j;
			DgnBdcwD.iCrackType = iCrackType;
			DgnBdcwD.dMd = dPrtMd;
			DgnBdcwD.dMs = dPrtMs;
			DgnBdcwD.dMl = dPrtMl;
			DgnBdcwD.dMd_extp = dPrtM_exTp;
			DgnBdcwD.dShrThk = dbw;
			DgnBdcwD.dFlgThk[0] = dFlgThk[0];
			DgnBdcwD.dFlgThk[1] = dFlgThk[1];
			DgnBdcwD.dpMomPos[0][0] = dpMomPos[0][0];
			DgnBdcwD.dpMomPos[0][1] = dpMomPos[0][1];
			DgnBdcwD.dpMomPos[1][0] = dpMomPos[1][0];
			DgnBdcwD.dpMomPos[1][1] = dpMomPos[1][1];
			DgnBdcwD.dC1 = dC1;
			DgnBdcwD.dC2 = dC2;
			DgnBdcwD.dC3 = dC3;
			DgnBdcwD.MatlElem = WA_Convert::Convert_A2W(MatlElem);
			DgnBdcwD.SpscSect = WA_Convert::Convert_A2W(SpscSect);

			if (arRbarPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arRbarPosi, DgnBdcwD.arRbarPosi);
			if (arTndnPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arTndnPosi, DgnBdcwD.arTndnPosi);
			DgnBdcwD.dFT = dPrtFT;
			DgnBdcwD.dFB = dPrtFB;

			//				T_DETAIL_STR DetailStr; DetailStr.Initialize();
			T_BDCW_BASE base;
			BOOL bVersionOK = FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnBdcwD, &base, &DetailStr, sizeIn, sizeResu, bVersionOK);
			if (!bVersionOK)
			{
				AfxMessageBox(_T("Conversion of Design data failed"));
				continue;
			}
			if (!bCheck) continue;
			//==================================================================================================
			dSigss = base.dFRT;
			dwf = base.dWC;
			dwa = base.dAWC;


		}
	}

	return bCalcOK;
}

//  7. 정상사용극한상태 인장철근

//  8. 정상사용/재하능력극한상태 인장텐던량

//  9. 재하능력극한상태 휨강도 (강축)
BOOL CDgnJTG_D62_04::Get_JtgBdcy(ElemPairK ElemK, T_BDCY_JTG& BdcyJtg, T_DETAIL_STR& DetailStr, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode == CJJ_11_2011 && PscdD.bExistSpecialLoad)
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_FlexureStrength_CJJ"));
	else
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_FlexureStrength"));

	//pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_FlexureStrength")); 
	size_t sizeIn, sizeResu;
	sizeIn = sizeof(_DGN_BDCY_D_W);
	sizeResu = sizeof(T_BDCY_JTG_BASE);
	//==================================================================================================
	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if (!Get_PscSectElem(TRUE, ElemK, SectElem))
	{
		return FALSE;
	}
	_PSC_SECG_ELEM SecgElem;
	if (!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK = FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if (!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK = FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if (!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK = FALSE;
	double dgam0;
	if (!m_pDataCtrl->Get_JtgGamma(dgam0)) bCalcOK = FALSE;
	double dfcd = MatlElem.dfcd;
	double dftd = MatlElem.dftd;

	BOOL bTitle = TRUE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for (int i = iSta; i < iEnd; i++)
	{
		BOOL bPosiI = (i == 0 ? TRUE : FALSE);
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		// RBar
		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		// Tendon
		CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		BOOL bRebarExist = TRUE;
		BOOL bTndnExist = TRUE;
		if (arRbarPosi.GetSize() == 0 && arTndnPosi.GetSize() == 0)
		{
			bRebarExist = FALSE;
			bTndnExist = FALSE;
			CString sEndIJ = (bPosiI ? _T("I") : _T("J"));
			CString sErrMsg = _T("");
			sErrMsg.Format(_LS(IDS_DGN_PSC_NO_REBAR_TENDON_Err), ElemK, sEndIJ);
			//GSaveHistoryNF(sErrMsg);    
		}

		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0], dShrThk[1]), dShrThk[2]);
		double dFlgThk[2] = { SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1] };
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];

		double dXib;
		if (!Get_Xib_JTG(ElemK, bPosiI, dXib)) bCalcOK = FALSE;
		for (int j = 0; j < 2; j++) //Max, Min
		{
			int k = i * 2 + j;
			if (!bRebarExist && !bTndnExist) BdcyJtg.BdcyBase[k].bCHK = FALSE;
			if (BdcyJtg.BdcyBase[k].bCHK == FALSE) { m_RptBdcy.BdcyBase[k].bChk = FALSE; continue; }
			// For zzzDataForDgn_JTG_D62_04.txt
			if (m_bTestMode)
			{
				const auto it = m_aSelElemK.find(ElemK);
				if (it != m_aSelElemK.end())
				{
					rptwofstream fout(m_strTestDgnFile, ios::app);

					CString strTitle; strTitle.Format(_T("%s"), (bTitle ? _T("▷ Flexural strength(사용단계 정단면 휨검토)") : _T("")));
					bTitle = FALSE;
					CString strElem;	strElem.Format(_T("%6d  (%s)  (%s)"), ElemK, (i == 0 ? _T("I") : _T("J")), (j == 0 ? _T("Max") : _T("Min")));
					fout << endl
						<< strTitle << endl
						<< _T(" Elem  Posi  Max/Min : ") << strElem << endl
						<< endl;
					Get_PrintPropertyData(ElemK, MatlElem, SpscSect, arRbarPosi, arTndnPosi);
				}
			}

			//==================================================================================================
			_DGN_BDCY_D_W DgnBdcyD;
			DgnBdcyD.Initialize();
			DgnBdcyD.m_bReport = m_bReport;
			DgnBdcyD.m_bTestMode = m_bTestMode;
			DgnBdcyD.m_strTestDgnFile = m_strTestDgnFile;
			const auto& itSelElem = m_aSelElemK.find(ElemK);
			DgnBdcyD.bSelElemK = (itSelElem != m_aSelElemK.end());
			DgnBdcyD.ElemK = ElemK.first;
			DgnBdcyD.iIJ = i * 2 + j;
			DgnBdcyD.bYaxis = TRUE;
			DgnBdcyD.dgam0 = dgam0;
			DgnBdcyD.dxib = dXib;
			DgnBdcyD.dfcd = dfcd;
			DgnBdcyD.dftd = dftd;
			DgnBdcyD.dbw = dbw;
			DgnBdcyD.dFlgThk[0] = dFlgThk[0];
			DgnBdcyD.dFlgThk[1] = dFlgThk[1];
			DgnBdcyD.dpMomPos[0][0] = dpMomPos[0][0];
			DgnBdcyD.dpMomPos[0][1] = dpMomPos[0][1];
			DgnBdcyD.dpMomPos[1][0] = dpMomPos[1][0];
			DgnBdcyD.dpMomPos[1][1] = dpMomPos[1][1];
			DgnBdcyD.MatlElem = WA_Convert::Convert_A2W(MatlElem);
			DgnBdcyD.SpscSect = WA_Convert::Convert_A2W(SpscSect);
			if (arRbarPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arRbarPosi, DgnBdcyD.arRbarPosi);
			if (arTndnPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arTndnPosi, DgnBdcyD.arTndnPosi);

			WA_Convert::Convert_A2W(m_RptBdcy.BdcyBase[k], DgnBdcyD.RptBdcy);

			//      T_DETAIL_STR DetailStr; DetailStr.Initialize();
			BOOL bVersionOK = FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnBdcyD, &BdcyJtg.BdcyBase[k], &DetailStr, sizeIn, sizeResu, bVersionOK);
			if (!bVersionOK)
			{
				AfxMessageBox(_T("Conversion of Design data failed"));
				continue;
			}
			if (!bCheck) continue;
			//==================================================================================================
		}
	}
	return bCalcOK;
}

// 10. 재하능력극한상태 전단강도
BOOL CDgnJTG_D62_04::Get_JtgCums(ElemPairK ElemK, T_CUMS_JTG& CumsJtg, T_DETAIL_STR& DetailStr, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode == CJJ_11_2011 && PscdD.bExistSpecialLoad)
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_ShearDesign_CJJ"));
	else
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_ShearDesign"));

	//pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_ShearDesign")); 
	size_t sizeIn, sizeResu;
	sizeIn = sizeof(_DGN_CUMS_D_W);
	sizeResu = sizeof(T_CUMS_JTG_BASE);
	//==================================================================================================
	BOOL bCalcOK = TRUE;

	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if (!Get_PscSectElem(TRUE, ElemK, SectElem))
	{
		return FALSE;
	}
	_PSC_SECG_ELEM SecgElem;
	if (!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK = FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if (!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK = FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if (!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK = FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if (!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK = FALSE;
	T_MATD_D MatdD = MatlElem.MatdD;
	double dgam0;
	if (!m_pDataCtrl->Get_JtgGamma(dgam0)) bCalcOK = FALSE;

	BOOL bTitle = TRUE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for (int i = iSta; i < iEnd; i++)
	{
		BOOL bPosiI = (i == 0 ? TRUE : FALSE);
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		// RBar
		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		// Tendon
		CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		T_RPSC_SBAR RpscSbar = RshrElem.RshrPosi[i].RpscSbar;
		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dShrThick = min(min(dShrThk[0], dShrThk[1]), dShrThk[2]);
		double dFlgThk[2] = { SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1] };

		for (int j = 0; j < 2; j++) //Max, Min
		{
			int k = i * 2 + j;
			if (CumsJtg.CumsBase[k].bCHK == FALSE) { m_RptCums.CumsBase[k].bChk = FALSE; continue; }
			// For zzzDataForDgn_JTG_D62_04.txt
			if (m_bTestMode)
			{
				const auto it = m_aSelElemK.find(ElemK);
				if (it != m_aSelElemK.end())
				{
					rptwofstream fout(m_strTestDgnFile, ios::app);

					CString strTitle; strTitle.Format(_T("%s"), (bTitle ? _T("▷ Shear strength(사용단계 경사단면 전단검토)") : _T("")));
					bTitle = FALSE;
					CString strElem;	strElem.Format(_T("%6d  (%s)  (%s)"), ElemK, (i == 0 ? _T("I") : _T("J")), (j == 0 ? _T("Max") : _T("Min")));
					fout << endl
						<< strTitle << endl
						<< _T(" Elem  Posi  Max/Min : ") << strElem << endl
						<< endl;
				}
			}

			//==================================================================================================
			_DGN_CUMS_D_W DgnCumsD;
			DgnCumsD.Initialize();
			//			T_PSCD_D PscdD; PscdD.Initialize();
			//			m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
			DgnCumsD.iBrgType = PscdD.iBrgType;
			DgnCumsD.m_bReport = m_bReport;
			DgnCumsD.m_bTestMode = m_bTestMode;
			DgnCumsD.m_strTestDgnFile = m_strTestDgnFile;
			const auto& itSelElem = m_aSelElemK.find(ElemK);
			DgnCumsD.bSelElemK = (itSelElem != m_aSelElemK.end());
			DgnCumsD.ElemK = ElemK.first;
			DgnCumsD.bYaxis = TRUE;
			DgnCumsD.iIJ = i * 2 + j;
			DgnCumsD.dgam0 = dgam0;

			DgnCumsD.dShrThk = dShrThick;
			DgnCumsD.RpscSbar = WA_Convert::Convert_A2W(RpscSbar);
			WA_Convert::Convert_A2W(m_RptCums.CumsBase[k], DgnCumsD.RptCums);

			DgnCumsD.MatlElem = WA_Convert::Convert_A2W(MatlElem);
			DgnCumsD.SpscSect = WA_Convert::Convert_A2W(SpscSect);

			if (arRbarPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arRbarPosi, DgnCumsD.arRbarPosi);
			if (arTndnPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arTndnPosi, DgnCumsD.arTndnPosi);

			//      T_DETAIL_STR DetailStr; DetailStr.Initialize();
			BOOL bVersionOK = FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnCumsD, &CumsJtg.CumsBase[k], &DetailStr, sizeIn, sizeResu, bVersionOK);
			if (!bVersionOK)
			{
				AfxMessageBox(_T("Conversion of Design data failed"));
				continue;
			}
			if (!bCheck) continue;
			//==================================================================================================
		}
	}
	return bCalcOK;
}

// 11. 재하능력극한상태 비틀림강도 
BOOL CDgnJTG_D62_04::Get_JtgCrmt(ElemPairK ElemK, T_CRMT_JTG& CrmtJtg, T_DETAIL_STR& DetailStr, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode == CJJ_11_2011 && PscdD.bExistSpecialLoad)
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_TorsionDesign_CJJ"));
	else
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_TorsionDesign"));

	//pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_TorsionDesign")); 
	size_t sizeIn, sizeResu;
	sizeIn = sizeof(_DGN_CRMT_D_W);
	sizeResu = sizeof(T_CRMT_JTG_BASE);
	//==================================================================================================
	BOOL bCalcOK = TRUE;
	int iIJ = 0;
	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if (!Get_PscSectElem(TRUE, ElemK, SectElem))
	{
		return FALSE;
	}
	_PSC_SECG_ELEM SecgElem;
	if (!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK = FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if (!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK = FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if (!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK = FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if (!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK = FALSE;
	T_MATD_D MatdD = MatlElem.MatdD;
	double dgam0;
	if (!m_pDataCtrl->Get_JtgGamma(dgam0)) bCalcOK = FALSE;
	double dfcd = MatlElem.dfcd;
	double dftd = MatlElem.dfck;
	double dfsd = MatlElem.dfsdtM;
	double dfskS = MatlElem.dfskS;

	BOOL bTitle = TRUE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for (int i = iSta; i < iEnd; i++)
	{
		BOOL bPosiI = (i == 0 ? TRUE : FALSE);
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		// RBar
		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		// Tendon
		CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		T_RPSC_SBAR RpscSbar = RshrElem.RshrPosi[i].RpscSbar;

		BOOL bSolidSect;
		if (!SectElem.SectPosi[i].bClosedSect) bSolidSect = TRUE;    // bClosedSect -> Box형
		else                                  bSolidSect = FALSE;
		double dAoh = SectElem.SectPosi[i].dAoh;
		double dPh = SectElem.SectPosi[i].dPh;
		double dKt = SectElem.SectPosi[i].dKt;
		double dWt = SectElem.SectPosi[i].dWt;
		double dTorThk = SectElem.SectPosi[i].dTorThk;
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dShrThick = min(min(dShrThk[0], dShrThk[1]), dShrThk[2]);
		double dFlgThk[2] = { SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1] };
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];

		for (int j = 0; j < 3; j++) //Max, Min
		{
			int k = i * 3 + j;
			if (CrmtJtg.CrmtBase[k].bCHK == FALSE) { m_RptCrmt.CrmtBase[k].bChk = FALSE; continue; }
			// For zzzDataForDgn_JTG_D62_04.txt
			if (m_bTestMode)
			{
				const auto it = m_aSelElemK.find(ElemK);
				if (it != m_aSelElemK.end())
				{
					rptwofstream fout(m_strTestDgnFile, ios::app);

					CString strTitle; strTitle.Format(_T("%s"), (bTitle ? _T("▷ Torsional moment strength(사용단계 비틀림검토)") : _T("")));
					bTitle = FALSE;
					CString strElem;	strElem.Format(_T("%6d  (%s)  (%s)"), ElemK, (i == 0 ? _T("I") : _T("J")), (j == 0 ? _T("Max") : _T("Min")));
					fout << endl
						<< strTitle << endl
						<< _T(" Elem  Posi  Max/Min : ") << strElem << endl
						<< endl;
				}
			}

			//==================================================================================================
			_DGN_CRMT_D_W DgnCrmtD;
			DgnCrmtD.Initialize();
			//			T_PSCD_D PscdD; PscdD.Initialize();
			//			m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
			DgnCrmtD.iBrgType = PscdD.iBrgType;
			DgnCrmtD.m_bReport = m_bReport;
			DgnCrmtD.m_bTestMode = m_bTestMode;
			DgnCrmtD.m_strTestDgnFile = m_strTestDgnFile;
			const auto& itSelElem = m_aSelElemK.find(ElemK);
			DgnCrmtD.bSelElemK = (itSelElem != m_aSelElemK.end());			
			DgnCrmtD.ElemK = ElemK.first;
			DgnCrmtD.bYaxis = TRUE;
			DgnCrmtD.iIJ = iIJ++;
			DgnCrmtD.dgam0 = dgam0;
			DgnCrmtD.MatlElem = WA_Convert::Convert_A2W(MatlElem);
			DgnCrmtD.SpscSect = WA_Convert::Convert_A2W(SpscSect);
			DgnCrmtD.bSolidSect = bSolidSect;
			DgnCrmtD.dAoh = dAoh;
			DgnCrmtD.dPh = dPh;
			DgnCrmtD.dWt = dWt;
			DgnCrmtD.dShrThk = dShrThick;
			DgnCrmtD.dFlgThk[0] = dFlgThk[0];
			DgnCrmtD.dFlgThk[1] = dFlgThk[1];
			DgnCrmtD.dTorThk = dTorThk;
			DgnCrmtD.dpMomPos[0][0] = dpMomPos[0][0];
			DgnCrmtD.dpMomPos[0][1] = dpMomPos[0][1];
			DgnCrmtD.dpMomPos[1][0] = dpMomPos[1][0];
			DgnCrmtD.dpMomPos[1][1] = dpMomPos[1][1];
			DgnCrmtD.RpscSbar = WA_Convert::Convert_A2W(RpscSbar);
			WA_Convert::Convert_A2W(m_RptCrmt.CrmtBase[k], DgnCrmtD.RptCrmt);

			if (arRbarPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arRbarPosi, DgnCrmtD.arRbarPosi);
			if (arTndnPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arTndnPosi, DgnCrmtD.arTndnPosi);

			//      T_DETAIL_STR DetailStr; DetailStr.Initialize();
			BOOL bVersionOK = FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnCrmtD, &CrmtJtg.CrmtBase[k], &DetailStr, sizeIn, sizeResu, bVersionOK);
			if (!bVersionOK)
			{
				AfxMessageBox(_T("Conversion of Design data failed"));
				continue;
			}
			if (!bCheck) continue;
			//==================================================================================================
		}
	}
	return bCalcOK;
}

/************************************************************************/
/* 12.compressive stress check at service stage.예箕駱炬                */
/************************************************************************/
BOOL CDgnJTG_D62_04::Get_JtgCscs(ElemPairK ElemK, T_CSTCS_CH& CscsJtg, T_DETAIL_STR& DetailStr, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode == CJJ_11_2011 && PscdD.bExistSpecialLoad)
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_CompressiveCheck_CJJ"));
	else
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_CompressiveCheck"));

	//pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_CompressiveCheck")); 
	size_t sizeIn, sizeResu;
	sizeIn = sizeof(_DGN_JTCS_D_W);
	sizeResu = sizeof(T_CSTCS_CH_BASE);
	//==================================================================================================
	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if (!Get_PscSectElem(TRUE, ElemK, SectElem))
	{
		return FALSE;
	}

	//딧혤챘쌔충鑒앴
	T_SECT_D SectD;
	SectD.Initialize();	
	if (!m_pDoc->m_pPostCtrl->GetSectPostDgn(ElemK, SectD))	return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if (!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK = FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if (!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK = FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if (!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK = FALSE;
	T_MATD_D MatdD = MatlElem.MatdD;
	double dgam0;
	if (!m_pDataCtrl->Get_JtgGamma(dgam0)) bCalcOK = FALSE;
	double dfcd = MatlElem.dfcd;
	double dftd = MatlElem.dfck;

	//get l0
	T_LENG_D Leng; Leng.Initialize();
	T_KFAC_D Kfac; Kfac.Initialize();
	m_pDoc->m_pAttrCtrl->GetLeng(ElemK.first, Leng);
	m_pDoc->m_pAttrCtrl->GetKfac(ElemK.first, Kfac);
	//	double dl0 = Leng.BLy*Kfac.BKy;
	double dl0 = Leng.BLy*Kfac.BKy;
	if (dl0<1.0E-07 && dl0>-1.0E-07) {
		GSaveHistoryNF(_LS(IDS_DGN_PSC_NO_L0_COMPRESS_Err));
		return FALSE;
	}

	BOOL bTitle = TRUE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for (int i = iSta; i < iEnd; i++)
	{
		BOOL bPosiI = (i == 0 ? TRUE : FALSE);
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		//modify by MITC(sect)
		BOOL bGetJData = FALSE;
		if ((SectD.nStype == D_SECT_TYPE_PSC) ||
			(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.nStype == D_SECT_TYPE_PSC))
		{
			if (fabs(SectD.SectBefore.SectJ.Stiffness.Area) < 1.0e-7 || fabs(SectD.SectBefore.SectJ.Stiffness.Rxx) < 1.0e-7
				|| fabs(SectD.SectBefore.SectJ.Stiffness.Ryy) < 1.0e-7 || fabs(SectD.SectBefore.SectJ.Stiffness.Rzz) < 1.0e-7)
			{
			}
			else
				bGetJData = TRUE;
		}
		SpscSect.dArea = i == 1 && bGetJData ? SectD.SectBefore.SectJ.Stiffness.Area : SectD.SectBefore.SectI.Stiffness.Area;
		SpscSect.dIxx = i == 1 && bGetJData ? SectD.SectBefore.SectJ.Stiffness.Rxx : SectD.SectBefore.SectI.Stiffness.Rxx;
		SpscSect.dIyy = i == 1 && bGetJData ? SectD.SectBefore.SectJ.Stiffness.Ryy : SectD.SectBefore.SectI.Stiffness.Ryy;
		SpscSect.dIzz = i == 1 && bGetJData ? SectD.SectBefore.SectJ.Stiffness.Rzz : SectD.SectBefore.SectI.Stiffness.Rzz;

		// RBar
		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		// Tendon
		CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);

		BOOL bRebarExist = TRUE;
		BOOL bTndnExist = TRUE;
		if (arRbarPosi.GetSize() == 0 && arTndnPosi.GetSize() == 0)
		{
			bRebarExist = FALSE;
			bTndnExist = FALSE;
			CString sEndIJ = (bPosiI ? _T("I") : _T("J"));
			CString sErrMsg = _T("");
			sErrMsg.Format(_LS(IDS_DGN_PSC_NO_REBAR_TENDON_Err), ElemK, sEndIJ);
			//GSaveHistoryNF(sErrMsg);    
		}

		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0], dShrThk[1]), dShrThk[2]);
		double dFlgThk[2] = { SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1] };
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];

		for (int j = 0; j < 5; j++) //Max, Min
		{
			int k = i * 5 + j;
			//      if(CscsJtg.cstcsBase.bCHK==FALSE) { m_RptCscs.CrmtBase[k].bChk=FALSE; continue; } //reportdata
			//==================================================================================================
			_DGN_JTCS_D_W DgnJtcsD;
			DgnJtcsD.Initialize();
			DgnJtcsD.ElemK = ElemK.first;
			DgnJtcsD.bYaxis = TRUE;
			DgnJtcsD.iIJ = k;
			DgnJtcsD.dgam0 = dgam0;
			DgnJtcsD.dl0 = dl0;
			DgnJtcsD.dfcd = dfcd;
			DgnJtcsD.dftd = dftd;
			DgnJtcsD.dbw = dbw;
			DgnJtcsD.dFlgThk[0] = dFlgThk[0];
			DgnJtcsD.dFlgThk[1] = dFlgThk[1];
			DgnJtcsD.dpMomPos[0][0] = dpMomPos[0][0];
			DgnJtcsD.dpMomPos[0][1] = dpMomPos[0][1];
			DgnJtcsD.dpMomPos[1][0] = dpMomPos[1][0];
			DgnJtcsD.dpMomPos[1][1] = dpMomPos[1][1];
			DgnJtcsD.MatlElem = WA_Convert::Convert_A2W(MatlElem);
			DgnJtcsD.SpscSect = WA_Convert::Convert_A2W(SpscSect);
			if (arRbarPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arRbarPosi, DgnJtcsD.arRbarPosi);
			if (arTndnPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arTndnPosi, DgnJtcsD.arTndnPosi);

			//      T_DETAIL_STR DetailStr; DetailStr.Initialize();
			BOOL bVersionOK = FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnJtcsD, &CscsJtg.cstcsBase[k], &DetailStr, sizeIn, sizeResu, bVersionOK);
			if (!bVersionOK)
			{
				AfxMessageBox(_T("Conversion of Design data failed"));
				continue;
			}
			if (!bCheck) continue;
			//==================================================================================================
		}
	}
	return bCalcOK;
}

BOOL CDgnJTG_D62_04::Get_JtgTbar(ElemPairK ElemK, T_TBAR_JTG& TbarJtg, T_DETAIL_STR& DetailStr, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/, int iUseKind/*0*/)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode == CJJ_11_2011 && PscdD.bExistSpecialLoad)
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_TensionRebar_CJJ"));
	else
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_TensionRebar"));

	//pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_TensionRebar")); 
	size_t sizeIn, sizeResu;
	sizeIn = sizeof(_DGN_TBAR_D_W);
	sizeResu = sizeof(T_TBAR_JTG_BASE);
	//==================================================================================================
	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if (!Get_PscSectElem(TRUE, ElemK, SectElem))
	{
		return FALSE;
	}
	_PSC_SECG_ELEM SecgElem;
	if (!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK = FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if (!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK = FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if (!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK = FALSE;
	double dgam0;
	if (!m_pDataCtrl->Get_JtgGamma(dgam0)) bCalcOK = FALSE;
	double dfcd = MatlElem.dfcd;
	double dftd = MatlElem.dftd;

	BOOL  bTitle = TRUE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for (int i = iSta; i < iEnd; i++)
	{
		BOOL bPosiI = (i == 0 ? TRUE : FALSE);
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		// RBar
		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		// Tendon
		CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		BOOL bRebarExist = TRUE;
		BOOL bTndnExist = TRUE;
		if (arRbarPosi.GetSize() == 0 && arTndnPosi.GetSize() == 0)
		{
			bRebarExist = FALSE;
			bTndnExist = FALSE;
			CString sEndIJ = (bPosiI ? _T("I") : _T("J"));
			CString sErrMsg = _T("");
			sErrMsg.Format(_LS(IDS_DGN_PSC_NO_REBAR_TENDON_Err), ElemK, sEndIJ);
			//GSaveHistoryNF(sErrMsg);    
		}

		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0], dShrThk[1]), dShrThk[2]);
		double dFlgThk[2] = { SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1] };
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];

		double dXib;
		if (!Get_Xib_JTG(ElemK, bPosiI, dXib)) bCalcOK = FALSE;
		for (int j = 0; j < 2; j++) //Top, Bot
		{
			int k = i * 2 + j;
			if (!bRebarExist && !bTndnExist) TbarJtg.TbarBase[k].bCHK = FALSE;
			if (TbarJtg.TbarBase[k].bCHK == FALSE) continue;
			// For zzzDataForDgn_JTG_D62_04.txt
			if (m_bTestMode)
			{
				const auto it = m_aSelElemK.find(ElemK);
				if (it != m_aSelElemK.end())
				{
					rptwofstream fout(m_strTestDgnFile, ios::app);

					CString strTitle; strTitle.Format(_T("%s"), (bTitle ? _T("▷ Required tension rebars(필요인장철근량)") : _T("")));
					bTitle = FALSE;
					CString strElem;	strElem.Format(_T("%6d  (%s)  (%s)"), ElemK, (i == 0 ? _T("I") : _T("J")), (j == 0 ? _T("Top") : _T("Bottom")));
					fout << endl
						<< strTitle << endl
						<< _T(" Elem  Posi  Top/Bot : ") << strElem << endl
						<< endl;
				}
			}
			//==================================================================================================
			_DGN_TBAR_D_W DgnTbarD;
			DgnTbarD.Initialize();
			DgnTbarD.m_bTestMode = m_bTestMode;
			DgnTbarD.m_strTestDgnFile = m_strTestDgnFile;
			const auto& itSelElem = m_aSelElemK.find(ElemK);
			DgnTbarD.bSelElemK = (itSelElem != m_aSelElemK.end());			
			DgnTbarD.ElemK = ElemK.first;
			DgnTbarD.iIJ = i;
			DgnTbarD.iTopBot = j;
			DgnTbarD.MatlElem = WA_Convert::Convert_A2W(MatlElem);
			DgnTbarD.dgam0 = dgam0;
			DgnTbarD.dfcd = dfcd;
			DgnTbarD.dftd = dftd;
			DgnTbarD.dxib = dXib;
			DgnTbarD.dbw = dbw;
			DgnTbarD.dFlgThk[0] = dFlgThk[0];
			DgnTbarD.dFlgThk[1] = dFlgThk[1];
			DgnTbarD.dpMomPos[0][0] = dpMomPos[0][0];
			DgnTbarD.dpMomPos[0][1] = dpMomPos[0][1];
			DgnTbarD.dpMomPos[1][0] = dpMomPos[1][0];
			DgnTbarD.dpMomPos[1][1] = dpMomPos[1][1];
			DgnTbarD.SpscSect = WA_Convert::Convert_A2W(SpscSect);
			if (arRbarPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arRbarPosi, DgnTbarD.arRbarPosi);
			if (arTndnPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arTndnPosi, DgnTbarD.arTndnPosi);

			//      T_DETAIL_STR DetailStr; DetailStr.Initialize();
			BOOL bVersionOK = FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnTbarD, &TbarJtg.TbarBase[k], &DetailStr, sizeIn, sizeResu, bVersionOK);
			if (!bVersionOK)
			{
				AfxMessageBox(_T("Conversion of Design data failed"));
				continue;
			}
			if (!bCheck) continue;
			//==================================================================================================
		}
	}
	return bCalcOK;

}

BOOL CDgnJTG_D62_04::Get_JtgTndn(ElemPairK ElemK, T_TNDN_JTG& TndnJtg, T_DETAIL_STR& DetailStr)
{
	//==================================================================================================
	// Explicit Linkage for DLL : 
	typedef BOOL(PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);
	PDGNCALC *pDgnCalc;
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iDgnCode == CJJ_11_2011 && PscdD.bExistSpecialLoad)
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_ReqTendon_CJJ"));
	else
		pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_ReqTendon"));

	//pDgnCalc = (PDGNCALC*)GetProcAddress(m_hDllCalc, ("Get_ReqTendon")); 
	size_t sizeIn, sizeResu;
	sizeIn = sizeof(_DGN_TNDN_D_W);
	sizeResu = sizeof(T_TNDN_JTG_BASE);
	//==================================================================================================
	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if (!Get_PscSectElem(TRUE, ElemK, SectElem))
	{
		return FALSE;
	}
	_PSC_SECG_ELEM SecgElem;
	if (!Get_PscSecgElem(ElemK, SecgElem))
	{
		return FALSE;
	}

	_PSC_RBAR_ELEM RbarElem;
	if (!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK = FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if (!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK = FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if (!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK = FALSE;
	T_MATD_D MatdD = MatlElem.MatdD;
	double dgam0;
	if (!m_pDataCtrl->Get_JtgGamma(dgam0)) bCalcOK = FALSE;
	double dfcd = MatlElem.dfcd;
	double dftd = MatlElem.dftd;
	double dfsd = MatlElem.dfsdtM;
	double dfskS = MatlElem.dfskS;

	double dfck_p = MatlElem.dfckp;
	double dftk_p = MatlElem.dftkp;
	double dfck = MatlElem.dfck;
	double dftk = MatlElem.dftk;

	_DGN_STRS_CRC StrsMax; StrsMax.Initialize();
	_DGN_STRS_CRC StrsMin; StrsMin.Initialize();
	Get_Sig_pc(ElemK, StrsMax, StrsMin);

	for (int i = 0; i < 2; i++)
	{
		// Section
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		// RBar
		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		// Tendon
		CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);

		double dFlgThk[2] = { SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1] };

		for (int j = 0; j < 2; j++) // Top, Bot.
		{
			int k = i * 2 + j;
			if (TndnJtg.TndnBase[k].bCHK == FALSE) continue;
			//==================================================================================================
			_DGN_TNDN_D_W DgnTndnD;
			DgnTndnD.Initialize();
			DgnTndnD.ElemK = ElemK.first;
			DgnTndnD.iIJ = i;
			DgnTndnD.iTopBot = j;
			DgnTndnD.dfck_p = dfck_p;
			DgnTndnD.dftk_p = dftk_p;
			DgnTndnD.dfck = dfck;
			DgnTndnD.dftk = dftk;
			DgnTndnD.dFlgThk[0] = dFlgThk[0];
			DgnTndnD.dFlgThk[1] = dFlgThk[1];
			DgnTndnD.SpscSect = WA_Convert::Convert_A2W(SpscSect);
			DgnTndnD.SpscgSect = WA_Convert::Convert_A2W(SpscgSect);
			TndnJtg.TndnBase[k].dMcr = (k % 2 == 0 ? StrsMin.dScom[k / 2] : StrsMax.dScom[k / 2]);

			int iDgnLcomNoStrn = (k % 2 == 0 ? StrsMin.iRefNo[k / 2] : StrsMax.iRefNo[k / 2]);
			CString strOrgLcomNaStrn = _T("");
			int iMaxMinTypeStrn = 0;
			if (!m_pForcCtrl->Get_LcomType(iDgnLcomNoStrn, strOrgLcomNaStrn, iMaxMinTypeStrn))	ASSERT(0);

			TndnJtg.TndnBase[k].iMaxCrack = iMaxMinTypeStrn;
			TndnJtg.TndnBase[k].LCombKCrack = iDgnLcomNoStrn;

			//TndnJtg.TndnBase[k].dMcr = m_pDataCtrl->Cng_StressFromCurrToCode(TndnJtg.TndnBase[k].dMcr);
			GetMcrByJTG(ElemK, TndnJtg.TndnBase[k].LCombKCrack, i == 0 ? TRUE : FALSE, SectElem.SectPosi[i], TndnJtg.TndnBase[k].dMcr, dftk);

			if (arRbarPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arRbarPosi, DgnTndnD.arRbarPosi);
			if (arTndnPosi.GetSize() > 0)
				WA_Convert::Convert_A2W(arTndnPosi, DgnTndnD.arTndnPosi);

			//      T_DETAIL_STR DetailStr; DetailStr.Initialize();
			BOOL bVersionOK = FALSE;
			BOOL bCheck = (*pDgnCalc)(&DgnTndnD, &TndnJtg.TndnBase[k], &DetailStr, sizeIn, sizeResu, bVersionOK);
			if (!bVersionOK)
			{
				AfxMessageBox(_T("Conversion of Design data failed"));
				continue;
			}
			if (!bCheck) continue;
			//==================================================================================================
		}
	}
	return bCalcOK;
}

BOOL CDgnJTG_D62_04::Get_BeforeAllowbleStressofConc(BOOL bPre, double dfck_p, double dftk_p, double dAg, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi,
	CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&>& arTndnPosi, T_FYSG_BASE FysgBase, double& dalComf, double& dalTenf, BOOL& bRho)
{
	//Section property.
	double dCtemp = SpscSect.dzCen;
	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();
	BOOL bCalcOK = Get_TndnReBarInfor(TRUE, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb);
	double dAp_p = rTdRb.dApctol + rTdRb.dApttol;
	double dAs_p = rTdRb.dAsctol + rTdRb.dAsttol;
	double dRho = 0.0;
	if (bPre) dRho = (dAg == 0.0 ? 0.0 : (dAs_p + dAp_p) / dAg);
	else     dRho = (dAg == 0.0 ? 0.0 : (dAs_p) / dAg);
	//Compression Stress
	dalComf = -0.7*dfck_p;  // JTG D62-04, 식(7.2.8)
	// Tension Stress

	// Change by Jaeoh('05.03.09)
	double dMaxTenf = 1.15*dftk_p;
	double dMinTenf = 0.7*dftk_p;
	dalTenf = dMaxTenf;
	double dItpTenf = 0.0;
	double dTRho = 0.0;

	if (FysgBase.dFMAX > 0.0) dalTenf = dMinTenf;
	else
	{
		if (dRho <= 0.002) dalTenf = dMinTenf;
		else if (0.002 < dRho && dRho < 0.004)
		{
			double dInterpTenf = dMinTenf + (dRho - 0.002) / (0.002)*(dMaxTenf - dMinTenf);
			dalTenf = dInterpTenf;
		}
		else if (dRho >= 0.004)
		{
			dalTenf = dMaxTenf;
		}
	}

	return  bCalcOK;
}

BOOL CDgnJTG_D62_04::Get_AfterAllowbleStressofConc(T_LCOM_K LcomK, double dfck, double dftk, double& dalComf, double& dalTenf)
{
	//Compression Stress.
	dalComf = -0.5*dfck;
	//Tension Stress.
	// Change by ZINU.('04.12.13). 6.3.1.1(2) Agree with Kye BJ.
	// Get Crack Width.
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iBrgType == 1) // Partially (A-Type).
	{
		int iDgnLcomKind = m_pForcCtrl->Get_LcomKind(LcomK);
		// JTG D62-04 (                    2=FR이없는경우(장기), 3=SF가있는경우(장기), 4=IP가있는경우(장기),
		//             1=영구하중시(단기), 5=FR이없는경우(단기), 6=SF가있는경우(단기), 7=IP가있는경우(단기), 8=탄성단계,
		//            11=계수하중시).
		// Change by ZINU.('05.04.01). Check Long/Short Term by Is_LongTerm() Only if Is_LcomForServ()=TRUE.
		dalTenf = 0.0;
		if (m_pForcCtrl->Is_LcomForServ(LcomK))
		{
			dalTenf = (m_pForcCtrl->Is_LongTerm(LcomK) ? 0.0 : 0.7*dftk);
		}
	}
	else  dalTenf = 0.0;

	return TRUE;
}

BOOL CDgnJTG_D62_04::Get_AllowPrincipalStress(double dfck, double dftk, double dfsk, double dSig_tp, double dbw, double dAsv, double dSv, double& dalSig_cp, double& dalSig_tp)
{
	// Compression Stress.
	dalSig_cp = -0.6*dfck;
	// Tension Stress.
	/*
	if(dSig_tp<=0.5*dftk)
	{
	dalSig_tp = 0.5*dftk;
	}
	else if(dSig_tp>0.5*dftk)
	{
	dalSig_tp = (dSv*dbw==0.0 ? 0.5*dftk : (dfsk*dAsv)/(dSv*dbw));
	}
	*/

	// Tension Allow Stress 수정.
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	if (PscdD.iBrgType == 0)  // 0=Fully Psc.
	{
		if (PscdD.iCastType == 0)      dalSig_tp = 0.6*dftk;  // Precast.
		else if (PscdD.iCastType == 1) dalSig_tp = 0.4*dftk;  // Cast-in-place
	}
	else if (PscdD.iBrgType == 1 || PscdD.iBrgType == 2)  // 1=Partially Psc(A-type), 2=Partially Psc(B-type)
	{
		if (PscdD.iCastType == 0)      dalSig_tp = 0.7*dftk;  // Precast.
		else if (PscdD.iCastType == 1) dalSig_tp = 0.5*dftk;  // Cast-in-place
	}

	return TRUE;
}

BOOL CDgnJTG_D62_04::Get_AllowbleStressofPC(T_TDNA_K TdnaK, T_FYPC_JTG& FypcJtg)
{
	_PSC_TNDN_TDNA_W TndnTdna;
	if (!Get_PscTndnTdna(TdnaK, TndnTdna)) return FALSE;
	double dfpk = TndnTdna.dfpk;
	int iTndn = 0;  // TndnTdna. 여기에서 받음.
	double dAFDL1 = 0.0, dAFDL2 = 0.0, dAFLL = 0.0;
	if (iTndn == 0)  // 0:Tendon,Wire
	{
		dAFDL1 = 0.75*dfpk;
		dAFLL = 0.65*dfpk;
	}
	else if (iTndn == 1) // 1:Steel Bar(정압나사문철근)
	{
		dAFDL1 = 0.90*dfpk;
		dAFLL = 0.80*dfpk;
	}
	else  ASSERT(0);

	FypcJtg.dAFDL1 = dAFDL1;
	FypcJtg.dAFLL = dAFLL;
	return (dAFDL1*dAFLL > 0.0);
}

BOOL CDgnJTG_D62_04::Get_CrackCheck(BOOL bPrint, ElemPairK ElemK, UINT iTopBot, int iCrackType, double dMd, double dMs, double dMl, double dMd_extp, double dShrThk, double dFlgThk[2],
	double dpMomPos[2][2], double dC1, double dC2, double dC3, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi,
	CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&>& arTndnPosi, double dFT, double dFB, double& dSigss, double& dWfk, double& dwa)
{

	switch (iCrackType)
	{
	case 0: dwa = 0.10; break; //Wire, Strand I,II
	case 1: dwa = 0.0; break; //Wire, Strand III, IV
	case 2: dwa = 0.20; break; //정압문나사철근 I,II
	case 3: dwa = 0.15; break; //정압문나사철근 III, IV
	}

	// Change by Jaeoh ('05.06.01)
	// Change by Jaeoh ('05.09.07)
	if (iTopBot == 0 && dMd > 0.0) return FALSE;        // Top계산할 때  정모멘트
	else if (iTopBot == 1 && dMd < 0.0) return FALSE;   // Bottom계산할 때 부보멘트

	// Material
	T_MATD_D MatdD = MatlElem.MatdD;
	double dEs = MatdD.MainRebarData.B_Elast;
	double dfsdtM = MatlElem.dfsdtM;
	double dfsdcM = MatlElem.dfsdcM;
	// Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy = dyDim - dyCen;
	double dCpz = dzDim - dzCen;
	double dA0 = SpscSect.dArea;
	double dIyy = SpscSect.dIyy;
	double dIzz = SpscSect.dIzz;
	double dh = dzDim;
	//Force  
	int i = 0, j = 0;
	//RBar, Tendon.
	double dgap = 0.0;
	double dApt = 0.0, dNtodpt = 0.0, dapt = 0.0;
	double dApc = 0.0, dNtodpc = 0.0, dapc = 0.0;
	double dAst = 0.0, dNtodst = 0.0, dast = 0.0;
	double dAsc = 0.0, dNtodsc = 0.0, dasc = 0.0;
	double dat = 0.0, dac = 0.0;

	BOOL bCalcOK = FALSE;

	// dMd는 원래 모멘트에서 M_1st만 빠져서 넘어오는 값.
	// Ms, Mp2는 지금 사용안함.  

	double dCtemp = 0.0;
	dCtemp = (dMd_extp >= 0.0 ? dCpz : dzCen);
	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();
	bCalcOK = Get_TndnReBarInfor(TRUE, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dfsdtM);
	if (dMd_extp >= 0.0 && iTopBot == 1)  // 정모멘트 
	{
		dApt = rTdRb.dApttol, dapt = (dApt == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dApc = rTdRb.dApctol, dapc = rTdRb.ddpc;
		dAst = rTdRb.dAsttol, dast = (dAst == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dAsc = rTdRb.dAsctol, dasc = rTdRb.ddsc;
		dat = rTdRb.daBott;
		dac = rTdRb.daTopc;
	}
	else if (dMd_extp < 0.0 && iTopBot == 0)
	{
		dApt = rTdRb.dApctol, dapt = rTdRb.ddpc;
		dApc = rTdRb.dApttol, dapc = (dApc == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dAst = rTdRb.dAsctol, dast = rTdRb.ddsc;
		dAsc = rTdRb.dAsttol, dasc = (dAsc == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dat = rTdRb.daTopt;
		dac = rTdRb.daBotc;
	}
	double dh0 = dh - dat; //dast
	if (dat == 0.0) dh0 = 0.8*dh;

	double dAs = dAst;
	double dAp = dApt;
	double dd = 0.0;

	// Change by Jaeoh ('05.04.14)
	// 종방향인장철근 직경 계산.  
	double dSumBarD = 0.0, dSumTndnD = 0.0;
	double dSumBarD2 = 0.0, dSumTndnD2 = 0.0;
	// Rebar
	for (i = 0; i < arRbarPosi.GetSize(); i++)
	{
		_DGN_RBAR_CRC Rbar = arRbarPosi.GetAt(i);
		if (dMd_extp >= 0.0 && iTopBot == 1)
		{
			if (Rbar.dz < 0.0)
			{
				dSumBarD += Rbar.dDia;
				dSumBarD2 += pow(Rbar.dDia, 2);
			}
		}
		else if (dMd_extp < 0.0 && iTopBot == 0)
		{
			if (Rbar.dz > 0.0)
			{
				dSumBarD += Rbar.dDia;
				dSumBarD2 += pow(Rbar.dDia, 2);
			}
		}
	}

	// Tendon
	for (i = 0; i < arTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(i);

		if (dMd_extp > 0.0 && iTopBot == 1)
		{
			if (Tndn.dz < 0.0)
			{
				dSumTndnD += Tndn.dEffDia;
				dSumTndnD2 += pow(Tndn.dEffDia, 2);
			}
		}
		else if (dMd_extp <= 0.0 && iTopBot == 0)
		{
			if (Tndn.dz > 0.0)
			{
				dSumTndnD += Tndn.dEffDia;
				dSumTndnD2 += pow(Tndn.dEffDia, 2);
			}
		}
	}
	double dnidi2 = dSumBarD2 + dSumTndnD2;
	double dnidi = dSumBarD + dSumTndnD;
	dd = (dnidi == 0.0 ? 0.0 : dnidi2 / dnidi);

	double dbf_top = dyDim;
	double dbf_bot = fabs(dpMomPos[0][0] - dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.
	double dbft = (dMd_extp > 0.0 ? dbf_bot : dbf_top);
	double dbfc = (dMd_extp > 0.0 ? dbf_top : dbf_bot);
	double db = dShrThk; //dbw
	double dhft = (dMd_extp > 0.0 ? dFlgThk[1] : dFlgThk[0]);
	double dhfc = (dMd_extp > 0.0 ? dFlgThk[0] : dFlgThk[1]);
	dhfc = (dhfc > 0.2*dh0 ? 0.2*dh0 : dhfc);
	double dRho = (db*dh0 + (dbft - db)*dhft == 0.0 ? 0.0 : (dAs + dAp) / (db*dh0 + (dbft - db)*dhft));
	if (dRho > 0.02)       dRho = 0.02;
	else if (dRho < 0.006) dRho = 0.006;

	double dgam_f = (db*dh0 == 0.0 ? 0.0 : (dbfc - db)*dhfc / (db*dh0));
	double dl = 0.0; // 부재의 길이.
	double dl0 = 0.0; //Kl.
	double deta_s = 0.0;
	double dys = (dMd_extp > 0.0 ? dzCen - dast : dCpz - dast);
	double dNp0, dep0, dhps;
	// dep0 : (+)인장영역 편심길이, (-)압축영역 편심길이
	bCalcOK = Get_SumForceEccent(TRUE, dMd_extp, dCtemp, SpscSect, arRbarPosi, arTndnPosi, dNp0, dep0, dhps);
	//if(dNp0<0.0) ASSERT(0);
	//double de0 = (dNs==0.0 ? 0.0 : dMs/dNs);
	//deta_s=(dh0==0.0 || 4000.0*de0/dh0==0.0 || dh==0.0 ? 0.0 : 1.0+dl/(4000.0*de0/dh0)*pow(dl0/dh,2));
	//deta_s=(dl0/dh<14.0 ? 1.0 : deta_s);
	deta_s = 1.0;
	double dep = 0.0;
	double dMoment = dMd_extp; // Tendon Primary 제외. 
	if (dMd_extp >= 0.0 && iTopBot == 1)
	{
		dep = (dzCen - dat) - dep0;
	}
	else if (dMd_extp < 0.0 && iTopBot == 0)
	{
		dep = (dCpz - dat) - dep0;
	}
	double de = dep + (dNp0 == 0.0 ? 0.0 : dMoment / dNp0);
	// Change by ZINU.('04.12.12). Check Divided by Zero.
	//double des = deta_s*de0 + dys;
	double des = de;
	double dz = (des == 0.0 ? 0.0 : ((0.87 - 0.12*(1.0 - dgam_f)*pow(dh0 / des, 2))*dh0));
	double dSigss_cal = 0.0;
	if (arTndnPosi.GetSize() == 0.0 && dAst > 0.0)
	{
		dSigss_cal = dMs / (0.87*dAs*dh0);
		if (dSigss_cal >= 0.0) dSigss = (dSigss_cal > dfsdtM ? dfsdtM : dSigss_cal);
		else if (dSigss_cal < 0.0) dSigss = (fabs(dSigss_cal) > dfsdcM ? -dfsdcM : dSigss_cal);
	}
	else if (dApt > 0.0 || dApc > 0.0)
	{
		// 인장철근 응력은 압축(-),인장(+)표현 -> 시공단계 응력표현 고려해서 나중에 수정.
		dSigss_cal = ((dAp + dAs)*dz == 0.0 ? 0.0 : (dMoment - dNp0 * (dz - dep)) / ((dAp + dAs)*dz));
		if (dSigss_cal >= 0.0) dSigss = (dSigss_cal > dfsdtM ? dfsdtM : dSigss_cal);
		else if (dSigss_cal < 0.0) dSigss = (fabs(dSigss_cal) > dfsdcM ? -dfsdcM : dSigss_cal);
	}

	dWfk = dC1 * dC2*dC3*dSigss / dEs * (30.0 + dd) / (0.28 + 10.0*dRho);
	if (dSigss < 0.0) dWfk = 0.0; // 인장철근이 압축이면 Wfk=0.0, 계BJ님 요청

	// Add by Jaeoh. ('06.03.16). 계BJ님 요청.
	if (iTopBot == 0) // Top
	{
		if (dFT > 0.0) { dSigss = 0.0, dWfk = 0.0; }
	}
	else if (iTopBot == 1) // Bot
	{
		if (dFB > 0.0) { dSigss = 0.0, dWfk = 0.0; }
	}
	else ASSERT(0);

	//++++++++++++++++++++++++++
	// For zzzDataForDgn_JTG_D62_04.txt
	if (m_bTestMode && bPrint)
	{
		const auto it = m_aSelElemK.find(ElemK);
		if (it != m_aSelElemK.end())
		{
			rptwofstream fout(m_strTestDgnFile, ios::app);

			CString strMs;  strMs.Format(_T("%16g"), dMs);
			CString strMl;  strMl.Format(_T("%16g"), dMl);
			CString strMd_extp;  strMd_extp.Format(_T("%16g"), dMd_extp);
			fout << _T("Ms     : ") << strMs << endl;
			fout << _T("Ml     : ") << strMl << endl;
			fout << _T("Ms+Mp2 : ") << strMd_extp << endl;
			// dRho      
			CString strast; strast.Format(_T("%16g"), dat);
			CString strAs;  strAs.Format(_T("%16g"), dAs);
			CString strAp;  strAp.Format(_T("%16g"), dAp);
			CString strb;   strb.Format(_T("%16g"), db);
			CString strh0;  strh0.Format(_T("%16g"), dh0);
			CString strbft; strbft.Format(_T("%16g"), dbft);
			CString strhft; strhft.Format(_T("%16g"), dhft);
			CString strRho; strRho.Format(_T("%16g"), dRho);
			fout << _T("  Rho") << endl
				<< _T(" a   : ") << strast << endl
				<< _T(" As  : ") << strAs << endl
				<< _T(" Ap  : ") << strAp << endl
				<< _T(" b   : ") << strb << endl
				<< _T(" h0  : ") << strh0 << endl
				<< _T(" bft : ") << strbft << endl
				<< _T(" hft : ") << strhft << endl
				<< _T(" Rho : ") << strRho << endl;

			// gam_f
			CString strbfc;  strbfc.Format(_T("%16g"), dbfc);
			CString strhfc;  strhfc.Format(_T("%16g"), dhfc);
			CString strgamf; strgamf.Format(_T("%16g"), dgam_f);
			fout << endl << _T("  gam_f") << endl
				<< _T(" bfc  : ") << strbfc << endl
				<< _T(" hfc  : ") << strhfc << endl
				<< _T(" gam_f: ") << strgamf << endl;

			// Sigss
			CString strMoment; strMoment.Format(_T("%16g"), dMoment);
			CString strNp0;    strNp0.Format(_T("%16g"), dNp0);
			CString strep0;    strep0.Format(_T("%16g"), dep0);
			CString strz;      strz.Format(_T("%16g"), dz);
			CString strep;     strep.Format(_T("%16g"), dep);
			CString strSigss_cal; strSigss_cal.Format(_T("%16g"), dSigss_cal);
			CString strSigss; strSigss.Format(_T("%16g"), dSigss);
			fout << endl << _T("  Sig_ss") << endl
				<< _T(" Np0    : ") << strNp0 << endl
				<< _T(" ep0    : ") << strep0 << endl
				<< _T(" z      : ") << strz << endl
				<< _T(" ep     : ") << strep << endl
				<< _T(" Sig_ss(cal) : ") << strSigss_cal << endl
				<< _T(" Sig_ss      : ") << dSigss << endl;

			// Wtk
			CString strC1;  strC1.Format(_T("%16g"), dC1);
			CString strC2;  strC2.Format(_T("%16g"), dC2);
			CString strC3;  strC3.Format(_T("%16g"), dC3);
			CString strd;   strd.Format(_T("%16g"), dd);
			CString strWfk; strWfk.Format(_T("%16g"), dWfk);
			fout << endl << _T("  Wtk") << endl
				<< _T(" C1  : ") << strC1 << endl
				<< _T(" C2  : ") << strC2 << endl
				<< _T(" C3  : ") << strC3 << endl
				<< _T(" d   : ") << strd << endl
				<< _T(" Wfk : ") << strWfk << endl;
		}
	}

	return TRUE;
}

BOOL CDgnJTG_D62_04::Get_PrintPropertyData(ElemPairK ElemK, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi,
	CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&>& arTndnPosi)
{
	if (m_bTestMode)
	{
		const auto it = m_aSelElemK.find(ElemK);
		if (it != m_aSelElemK.end())
		{
			rptwofstream fout(m_strTestDgnFile, ios::app);

			// Material
			// 콘크리트.
			CString strfck;    strfck.Format(_T("%16g"), MatlElem.dfck);
			CString strftk;    strftk.Format(_T("%16g"), MatlElem.dftk);
			CString strfcd;    strfcd.Format(_T("%16g"), MatlElem.dfcd);
			CString strftd;    strftd.Format(_T("%16g"), MatlElem.dftd);
			// 종방향철근.
			CString strEs;     strEs.Format(_T("%16g"), MatlElem.MatdD.MainRebarData.B_Elast);
			CString strfskM;   strfskM.Format(_T("%16g"), MatlElem.dfskM);
			CString strfsdtM;  strfsdtM.Format(_T("%16g"), MatlElem.dfsdtM);
			CString strfsdcM;  strfsdcM.Format(_T("%16g"), MatlElem.dfsdcM);
			// 전단철근.
			CString strfskS;   strfskS.Format(_T("%16g"), MatlElem.dfskS);
			CString strfsdtS;  strfsdtS.Format(_T("%16g"), MatlElem.dfsdtS);
			CString strfsdcS;  strfsdcS.Format(_T("%16g"), MatlElem.dfsdcS);
			// 경사철근.
			CString strfskD;   strfskD.Format(_T("%16g"), MatlElem.dfskD);
			CString strfsdtD;  strfsdtD.Format(_T("%16g"), MatlElem.dfsdtD);
			CString strfsdcD;  strfsdcD.Format(_T("%16g"), MatlElem.dfsdcD);
			// 전단강봉 (0=I,1=J).
			CString strfpkVi;  strfpkVi.Format(_T("%16g"), MatlElem.dfpkV[0]);
			CString strfpkVj;  strfpkVj.Format(_T("%16g"), MatlElem.dfpkV[1]);
			CString strfpdtVi; strfpdtVi.Format(_T("%16g"), MatlElem.dfpdtV[0]);
			CString strfpdtVj; strfpdtVj.Format(_T("%16g"), MatlElem.dfpdtV[1]);
			CString strfpdcVi; strfpdcVi.Format(_T("%16g"), MatlElem.dfpdcV[0]);
			CString strfpdcVj; strfpdcVj.Format(_T("%16g"), MatlElem.dfpdcV[1]);

			// Section
			CString stryDim;   stryDim.Format(_T("%16g"), SpscSect.dyDim);
			CString strzDim;   strzDim.Format(_T("%16g"), SpscSect.dzDim);
			CString stryCen;   stryCen.Format(_T("%16g"), SpscSect.dyCen);
			CString strzCen;   strzCen.Format(_T("%16g"), SpscSect.dzCen);
			CString strArea;	 strArea.Format(_T("%16g"), SpscSect.dArea);
			CString strIxx;	   strIxx.Format(_T("%16g"), SpscSect.dIxx);
			CString strIyy;	   strIyy.Format(_T("%16g"), SpscSect.dIyy);
			CString strIzz;	   strIzz.Format(_T("%16g"), SpscSect.dIzz);

			fout << endl
				<< _T(" Material") << endl
				<< _T("  Concrete") << endl
				<< _T("   fck : ") << strfck << endl
				<< _T("   ftk : ") << strftk << endl
				<< _T("   fcd : ") << strfcd << endl
				<< _T("   ftd : ") << strftd << endl
				<< _T("  Longitudinal reinforcement") << endl
				<< _T("   Es   : ") << strEs << endl
				<< _T("   fsk  : ") << strfskM << endl
				<< _T("   fsdt : ") << strfsdtM << endl
				<< _T("   ftdc : ") << strfsdcM << endl
				<< _T("  Stirrup") << endl
				<< _T("   fsk  : ") << strfskS << endl
				<< _T("   fsdt : ") << strfsdtS << endl
				<< _T("   fsdc : ") << strfsdtS << endl
				<< _T("  Diagonal Reinforcement") << endl
				<< _T("   fsk  : ") << strfskD << endl
				<< _T("   fsdt : ") << strfsdtD << endl
				<< _T("   fsdc : ") << strfsdtD << endl
				<< _T("  Steel Bar for Web (0=I,1=J)") << endl
				<< _T("   fpkVi  : ") << strfpkVi << endl
				<< _T("   fpkVj  : ") << strfpkVj << endl
				<< _T("   fpdtVi : ") << strfpdtVi << endl
				<< _T("   fpdtVj : ") << strfpdtVj << endl
				<< _T("   fpdcVi : ") << strfpdcVi << endl
				<< _T("   fpdcVj : ") << strfpdcVj << endl
				<< endl
				<< _T(" Section") << endl
				<< _T("  yDim : ") << stryDim << endl
				<< _T("  zDim : ") << strzDim << endl
				<< _T("  yCen : ") << stryCen << endl
				<< _T("  zCen : ") << strzCen << endl
				<< _T("  Area : ") << strArea << endl
				<< _T("  Ixx  : ") << strIxx << endl
				<< _T("  Iyy  : ") << strIyy << endl
				<< _T("  Izz  : ") << strIzz << endl
				<< endl;

			// 종방향철근 Property.
			CString strRbarColTitle;
			strRbarColTitle.Format(_T(" %8s %16s %16s %16s %16s"), _T("Name"), _T("Area"), _T("Dia"), _T("Posi_y"), _T("Posi_z"));
			fout << _T(" Rbar") << endl
				<< strRbarColTitle << endl;
			for (int i = 0; i < arRbarPosi.GetSize(); i++)
			{
				_DGN_RBAR_CRC Rbar = arRbarPosi.GetAt(i);
				CString strRbarPosi;
				strRbarPosi.Format(_T(" %8s %16g %16g %16g %16g"), Rbar.strRbarNa, Rbar.dArea, Rbar.dDia, Rbar.dy, Rbar.dz);
				fout << strRbarPosi << endl;
			}

			// Tendon Property.
			CString strTndnColTitle;
			strTndnColTitle.Format(_T(" %16s %8s %8s %8s %16s %16s %16s %16s %16s %16s %16s %16s %16s %16s %16s"),
				_T("GroupName"), _T("Int/Ext"), _T("BondType"), _T("Pre/Post"), _T("Eps"), _T("Area"), _T("Posi_y"), _T("Posi_z"),
				_T("EffDia"), _T("fpk"), _T("fpdt"), _T("fpdc"), _T("Loss-Stress"), _T("Elastic"), _T("Creep/Shrink"));
			fout << _T(" Tendon") << endl
				<< strTndnColTitle << endl;
			for (int i = 0; i < arTndnPosi.GetSize(); i++)
			{
				_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(i);
				CString strTndnPosi;

				CString strTndnGrup; strTndnGrup.Format(_T("%s"), Tndn.strTndnGrup);
				CString strTndnType; strTndnType.Format(_T("%s"), (Tndn.iTndnType == 0 ? _T("Internal") : _T("External")));
				CString strBondType; strBondType.Format(_T("%s"), (Tndn.iBondType == 0 ? _T("Bonded") : _T("Unbonded")));
				CString strLoadType; strLoadType.Format(_T("%s"), (Tndn.iLoadType == 0 ? _T("Pre-ten") : _T("Post-ten")));
				strTndnPosi.Format(_T(" %16s %8s %8s %8s %16g %16g %16g %16g %16g %16g %16g %16g %16g %16g %16g"),
					Tndn.strTndnGrup, strTndnType, strBondType, strLoadType,
					Tndn.dEps, Tndn.dArea, Tndn.dy, Tndn.dz, Tndn.dfpk, Tndn.dfpdt, Tndn.dfpdc, Tndn.dEffDia,
					Tndn.dStress[0], Tndn.dStrLoss[0][0], Tndn.dStrLoss[0][1]);
				fout << strTndnPosi << endl;
			}

			fout << endl;
		}
	}

	return TRUE;
}

///////////////////////////////////////////
// For Calculation.
//휨강도 검토(강축)
BOOL CDgnJTG_D62_04::Get_FlexureStrength(ElemPairK ElemK, BOOL bYaxis, double dgam0, double dxib, double dfcd, double dftd, double dbw, double dFlgThk[2], double dpMomPos[2][2], _PSC_MATL_ELEM& MatlElem,
	T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&>& arTndnPosi, T_BDCY_JTG_BASE& BdcyJtg, _RPT_BDCY_BASE& RptBdcy)
{
	if (arRbarPosi.GetSize() == 0.0 && arTndnPosi.GetSize() == 0.0) return FALSE;
	T_MATD_D MatdD = MatlElem.MatdD;

	// Material
	double dEc = MatdD.Data1.Analysis.Elast;
	double dfy = MatdD.MainRebarData.B_fy;
	double dEs = MatdD.MainRebarData.B_Elast;
	double dEp = 0.0;
	double dfsd = MatlElem.dfsdtM;
	double dfsdc = MatlElem.dfsdcM;

	// Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy = dyDim - dyCen;
	double dCpz = dzDim - dzCen;
	double dA0 = SpscSect.dArea;  // 전체환산단면적.
	double dIyy = SpscSect.dIyy;
	double dIzz = SpscSect.dIzz;

	//Force
	double dMd = BdcyJtg.dMuy;

	int i = 0, j = 0;
	double dgap = 0.0;
	double dAs = 0.0, dAsY = 0.0;
	double dApt = 0.0, dNtodpt = 0.0, dapt = 0.0;
	double dApc = 0.0, dNtodpc = 0.0, dapc = 0.0;
	double dAst = 0.0, dNtodst = 0.0, dast = 0.0;
	double dAsc = 0.0, dNtodsc = 0.0, dasc = 0.0;
	double dat = 0.0, dac = 0.0;
	//Tendon
	CArray<double, double> arApti, arApci;
	CArray<double, double> arapti, arapci;
	//Rbar
	CArray<double, double> arAsti, arAsci;
	CArray<double, double> arasti, arasci;

	double dNueC = 0.0;
	double dfst = 0.0, dfsc = 0.0;

	double dMn = 0.0, dr0Mn = 0.0;
	double dr0 = dgam0;
	double dr0Md = dr0 * dMd;
	double dTOL = 0.001;

	double dh = (bYaxis == TRUE ? dzDim : dyDim);
	double dbf_top = dyDim;
	double dbf_bot = fabs(dpMomPos[0][0] - dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.
	double db = (dMd >= 0.0 ? dbf_top : dbf_bot);
	double dLength = (bYaxis == TRUE ? dyDim : dzDim);

	// 압축영영의 높이 x<a's 일 때.
	double dApt_cent = 0.0, dapt_cent = 0.0;
	double dApc_cent = 0.0, dapc_cent = 0.0;
	double dAst_cent = 0.0, dast_cent = 0.0;
	double dAsc_cent = 0.0, dasc_cent = 0.0;
	double dat_cent = 0.0, dac_cent = 0.0;
	double dCcent = (dMd >= 0.0 ? dh - dzCen : dzCen);
	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();
	BOOL bCalcOK = Get_TndnReBarInfor(bYaxis, dCcent, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dfsd);
	if (dMd >= 0.0)  // 정모멘트 
	{
		dApt_cent = rTdRb.dApttol, dapt_cent = (dApt_cent == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dApc_cent = rTdRb.dApctol, dapc_cent = rTdRb.ddpc;
		dAst_cent = rTdRb.dAsttol, dast_cent = (dAst_cent == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dAsc_cent = rTdRb.dAsctol, dasc_cent = rTdRb.ddsc;
		dat_cent = rTdRb.daBott;
		dac_cent = rTdRb.daTopc;
	}
	else
	{
		dApt_cent = rTdRb.dApctol, dapt_cent = rTdRb.ddpc;
		dApc_cent = rTdRb.dApttol, dapc_cent = (dApc == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dAst_cent = rTdRb.dAsctol, dast_cent = rTdRb.ddsc;
		dAsc_cent = rTdRb.dAsttol, dasc_cent = (dAsc == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dat_cent = rTdRb.daTopt;
		dac_cent = rTdRb.daBotc;
	}

	double dx = 0.5*dh;  // 초기가정 
	double dCp = 0.0;
	if (dMd >= 0.0) dCp = (bYaxis == TRUE ? dCpz : dCpy);
	else         dCp = (bYaxis == TRUE ? dzCen : dyCen);
	double ddist = (bYaxis == TRUE ? dzDim : dyDim);
	double dCtemp = 0.0;
	UINT iCount = 1;

	double dToler = 0.0;
	double dh0 = 0.0, dxb = 0.0;
	double dTst = 0.0, dTps = 0.0, dCsc = 0.0, dCps = 0.0;

	double dCdist = (dMd >= 0.0 ? dh - dzCen : dzCen);
	double db0 = db - dbw;
	double dhf = (dMd >= 0.0 ? dFlgThk[0] : dFlgThk[1]);
	double dhfc = (dMd >= 0.0 ? dFlgThk[1] : dFlgThk[0]);

	CArray<_RPT_FLEX_DATA, _RPT_FLEX_DATA&> arFlex; // Report

	do
	{
		dTst = 0.0, dTps = 0.0, dCsc = 0.0, dCps = 0.0;
		//중립축을 기준으로 Tendon dy,dz를 수정.
		dCtemp = (dMd >= 0.0 ? dx : dh - dx);
		rTdRb.Initialize();
		bCalcOK = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dfsd);
		if (dMd >= 0.0)  // 정모멘트 
		{
			dApt = rTdRb.dApttol, dapt = (dApt == 0.0 ? 0.0 : dh - rTdRb.ddpt);
			dApc = rTdRb.dApctol, dapc = rTdRb.ddpc;
			dAst = rTdRb.dAsttol, dast = (dAst == 0.0 ? 0.0 : dh - rTdRb.ddst);
			dAsc = rTdRb.dAsctol, dasc = rTdRb.ddsc;
			dat = rTdRb.daBott;
			dac = rTdRb.daTopc;
		}
		else
		{
			dApt = rTdRb.dApctol, dapt = rTdRb.ddpc;
			dApc = rTdRb.dApttol, dapc = (dApc == 0.0 ? 0.0 : dh - rTdRb.ddpt);
			dAst = rTdRb.dAsctol, dast = rTdRb.ddsc;
			dAsc = rTdRb.dAsttol, dasc = (dAsc == 0.0 ? 0.0 : dh - rTdRb.ddst);
			dat = rTdRb.daTopt;
			dac = rTdRb.daBotc;
		}
		dh0 = dh - dat;
		//if(dat==0.0) dh0 = 0.8*dh;
		dxb = dxib * dh0;

		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> rarRbarPosi;
		CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> rarTndnPosi;
		bCalcOK = Get_TndnReBarTransform(bYaxis, dMd, dx, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);

		// Rbar.
		for (i = 0; i < rarRbarPosi.GetSize(); i++)
		{
			_DGN_RBAR_CRC Rbar = rarRbarPosi.GetAt(i);
			if (dMd > 0.0)
			{
				if (Rbar.dz > 0.0)      dCsc += dfsdc * Rbar.dArea;
				else if (Rbar.dz < 0.0) dTst += dfsd * Rbar.dArea;
			}
			else if (dMd <= 0.0)
			{
				if (Rbar.dz > 0.0)      dTst += dfsd * Rbar.dArea;
				else if (Rbar.dz < 0.0) dCsc += dfsdc * Rbar.dArea;
			}
		}

		// Tendon.
		for (i = 0; i < rarTndnPosi.GetSize(); i++)
		{
			_DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
			if (dMd > 0.0)
			{
				// Ap
				if (Tndn.dz > 0.0)
				{
					double dfpdci = Tndn.dfpdc;
					double dSigpoc = Tndn.dStress[0] - Tndn.dStrLoss[0][0];
					dCps += (dfpdci - dSigpoc)*Tndn.dArea;
				} // Ap'        
				else if (Tndn.dz < 0.0)
				{
					double dfpdi = Tndn.dfpdt;
					dTps += dfpdi * Tndn.dArea;
				}
			}
			else if (dMd <= 0.0)
			{
				// Ap
				if (Tndn.dz > 0.0)
				{
					double dfpdi = Tndn.dfpdt;
					dTps += dfpdi * Tndn.dArea;
				} // Ap'        
				else if (Tndn.dz < 0.0)
				{
					double dfpdci = Tndn.dfpdc;
					double dSigpoc = Tndn.dStress[0] - Tndn.dStrLoss[0][0];
					dCps += (dfpdci - dSigpoc)*Tndn.dArea;
				}
			}
		}

		double dCc = 0.0;
		if (dx <= dhf)
			dCc = dfcd * db*dx;
		else if (dx > dhf)
		{
			if (dx <= dh)     dCc = dfcd * (dbw*dx + (db - dbw)*dhf);
			else if (dx > dh) dCc = dfcd * (dbw*dh + (db - dbw)*dhf);
		}

		double dTens = dTst + dTps;
		double dComp = dCc + dCsc + dCps;
		double dCcDel = dTens - dComp;
		dToler = (dComp == 0.0 ? 0.0 : dCcDel / dComp);

		//Report
		//////////////////////////////////////////////////////////////////
		if (m_bReport)
		{
			_RPT_FLEX_DATA FlexD;
			FlexD.dx = dx;
			FlexD.dTst = dTst;
			FlexD.dTps = dTps;
			FlexD.dCc = dCc;
			FlexD.dCsc = dCsc;
			FlexD.dCps = dCps;
			FlexD.dTens = dTens;
			FlexD.dComp = dComp;
			FlexD.dTolerance = dToler;

			arFlex.Add(FlexD);
		}
		//////////////////////////////////////////////////////////////////

		if (fabs(dToler) < dTOL) break;
		if (iCount > 30) break;
		double dDeltaX = (dfcd*db == 0.0 ? 0.0 : fabs(dCcDel) / (dfcd*db));
		if (dCcDel < 0.0) dx -= dDeltaX;
		else           dx += dDeltaX;

		++iCount;

	} while (fabs(dToler) > dTOL);

	double dx_Cal = dx;
	dx = (dx > dxb ? dxb : dx);
	dCtemp = (dMd >= 0.0 ? dx : dh - dx);
	double dx_Posi = 0.0;
	if (dx <= dac_cent)
	{
		dCtemp = (dMd >= 0.0 ? dzCen : dh - dzCen);
		dx_Posi = dCtemp;
	}
	else
	{
		dx_Posi = dx;
	}

	rTdRb.Initialize();
	bCalcOK = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dfsd);
	if (dMd >= 0.0)  // 정모멘트 
	{
		dApt = rTdRb.dApttol, dapt = (dApt == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dApc = rTdRb.dApctol, dapc = rTdRb.ddpc;
		dAst = rTdRb.dAsttol, dast = (dAst == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dAsc = rTdRb.dAsctol, dasc = rTdRb.ddsc;
		dat = rTdRb.daBott;
		dac = rTdRb.daTopc;
	}
	else
	{
		dApt = rTdRb.dApctol, dapt = rTdRb.ddpc;
		dApc = rTdRb.dApttol, dapc = (dApc == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dAst = rTdRb.dAsctol, dast = rTdRb.ddsc;
		dAsc = rTdRb.dAsttol, dasc = (dAsc == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dat = rTdRb.daTopt;
		dac = rTdRb.daBotc;
	}

	CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> rarRbarPosi;
	CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> rarTndnPosi;
	Get_TndnReBarTransform(bYaxis, dMd, dx_Posi, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);

	double dMnCon = 0.0, dMnStc = 0.0, dMnPsc = 0.0;
	double dMnStc_t = 0.0, dMnPsc_t = 0.0; //JTG D62-04, 5.2.5절의 조건일 경우 사용
	double dMnCon_s = 0.0, dMnStc_s = 0.0, dMnPsc_s = 0.0;
	double dTndnComp = 0.0;
	for (i = 0; i < rarTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
		double dApci = 0.0, dapci = 0.0;
		dApci = Tndn.dArea;
		double dfpdci = Tndn.dfpdc;
		// Sig_po = Sig_con - Sig_l + Sig_l4.
		double dSigpoc = Tndn.dStress[0] - Tndn.dStrLoss[0][0]; //[0][0] : x-Elastic 손실.
		if (dMd >= 0.0)
		{
			// Ap
			if (Tndn.dz > 0.0)
			{
				double dCpsi = 0.0;
				dTndnComp += (dfpdci - dSigpoc);
			}
		}
		else if (dMd < 0.0)
		{
			if (Tndn.dz < 0.0)
			{
				double dCpsi = 0.0;
				dTndnComp += (dfpdci - dSigpoc);
			}
		}
	}

	for (i = 0; i < rarRbarPosi.GetSize(); i++)
	{
		_DGN_RBAR_CRC Rbar = rarRbarPosi.GetAt(i);
		double dAsci = 0.0, dasci = 0.0;
		dAsci = Rbar.dArea;
		if (dMd >= 0.0)
		{
			if (Rbar.dz > 0.0)
			{
				dasci = dx_Posi - Rbar.dz;
				dMnStc += dfsdc * dAsci*(dh0 - dasci);
			}
			else
			{
				double dasi = (dh - dx_Posi) - fabs(Rbar.dz);
				dMnStc_t += dfsd * Rbar.dArea*(dh - dasi - dac);//식(5.2.5-1) 두번째항.  //(dCp-dac+fabs(Rbar.dz)
				dMnStc_s += dfsd * Rbar.dArea*(dh - dasi - dasc);
			}
		}
		else if (dMd < 0.0)
		{
			if (Rbar.dz < 0.0)
			{
				dasci = dx_Posi - fabs(Rbar.dz);
				dMnStc += dfsdc * dAsci*(dh0 - dasci);
			}
			else
			{
				double dasi = (dh - dx_Posi) - fabs(Rbar.dz);
				dMnStc_t += dfsd * Rbar.dArea*(dh - dasi - dac);
				dMnStc_s += dfsd * Rbar.dArea*(dh - dasi - dasc);
			}
		}
	}

	for (i = 0; i < rarTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
		double dApci = 0.0, dapci = 0.0;
		dApci = Tndn.dArea;
		double dfpdci = Tndn.dfpdc;
		double dfpdti = Tndn.dfpdt;
		// Sig_po = Sig_con - Sig_l + Sig_l4.
		double dSigpoc = Tndn.dStress[0] - Tndn.dStrLoss[0][0]; //[0][0] : x-Elastic 손실.
		if (dMd >= 0.0)
		{
			// Ap
			if (Tndn.dz > 0.0)
			{
				double dCpsi = (dfpdci - dSigpoc)*Tndn.dArea;
				dapci = dx_Posi - Tndn.dz;
				dMnPsc += dCpsi * (dh0 - dapci);      //(dh0-Tndn.dz)
				dMnCon_s += dCpsi * (dapci - dasc);
				//dTndnComp += (dfpdci-dSigpoc);
			}
			else
			{
				double dapi = (dh - dx_Posi) - fabs(Tndn.dz);
				dMnPsc_t += dfpdti * Tndn.dArea*(dh - dapi - dac); //식(5.2.5-1) 첫번째항.
				dMnPsc_s += dfpdti * Tndn.dArea*(dh - dapi - dasc);
			}
		}
		else if (dMd < 0.0)
		{
			if (Tndn.dz < 0.0)
			{
				double dCpsi = dCpsi = (dfpdci - dSigpoc)*Tndn.dArea;
				dapci = dx_Posi - fabs(Tndn.dz);
				dMnPsc += dCpsi * (dh0 - dapci);    //(dh0-fabs(Tndn.dz))
				dMnCon_s += dCpsi * (dapci - dasc);
				//dTndnComp += (dfpdci-dSigpoc);
			}
			else
			{
				double dapi = (dh - dx_Posi) - fabs(Tndn.dz);
				dMnPsc_t += dfpdti * Tndn.dArea*(dh - dapi - dac);
				dMnPsc_s += dfpdti * Tndn.dArea*(dh - dapi - dasc);
			}
		}
	}

	if (dTndnComp > 0.0)
	{
		if (dx >= 2.0*dac)
		{
			if (dx <= dhf)
				dMnCon = dfcd * db*dx*(dh0 - dx / 2.0);
			else if (dx > dhf)
				dMnCon = dfcd * (dbw*dx*(dh0 - dx / 2.0) + (db - dbw)*dhf*(dh0 - dhf / 2.0));

			dMn = dMnCon + dMnStc + dMnPsc;
			if (dMn < 0.0) dMn = dMnPsc_t + dMnStc_t;
		}
		else
		{
			dMn = dMnPsc_t + dMnStc_t;
		}
	}
	else if (dTndnComp <= 0.0)
	{
		if (dx >= 2.0*dasc)
		{
			if (dx <= dhf)
				dMnCon = dfcd * db*dx*(dh0 - dx / 2.0);
			else if (dx > dhf)
				dMnCon = dfcd * (dbw*dx*(dh0 - dx / 2.0) + (db - dbw)*dhf*(dh0 - dhf / 2.0));

			dMn = dMnCon + dMnStc + dMnPsc;
			if (dMn < 0.0) dMn = dMnPsc_s + dMnStc_s - dMnCon_s;
		}
		else
		{
			dMn = dMnPsc_s + dMnStc_s - dMnCon_s;
		}
	}
	else ASSERT(0);

	double dRatM = (dMn == 0.0 ? 0.0 : fabs(dr0Md / dMn));

	BdcyJtg.dMuy = dMd;
	BdcyJtg.drMuy = dr0Md;
	BdcyJtg.dMny = dMn;
	BdcyJtg.dRatY = dRatM;

	if (m_bReport)
	{
		RptBdcy.dxib = dxib;
		RptBdcy.dfcd = dfcd;
		RptBdcy.dfsdt = dfsd;
		RptBdcy.dfsdc = dfsdc;
		RptBdcy.dxCal = dx_Cal;  RptBdcy.dh = dh;
		RptBdcy.dx = dx;      RptBdcy.dh0 = dh0;
		RptBdcy.dxb = dxb;     RptBdcy.dhf = dhf;
		RptBdcy.db = db;      RptBdcy.dat = dat;
		RptBdcy.dbw = dbw;     RptBdcy.dac = dac;
		RptBdcy.dast = dast;    RptBdcy.dasc = dasc;
		RptBdcy.dMd = dMd;     RptBdcy.dr0Md = dr0Md;
		RptBdcy.dMn = dMn;
		RptBdcy.dTndnComp = dTndnComp;
		RptBdcy.dMnCon = dMnCon;
		RptBdcy.dMnBarC = dMnStc;
		RptBdcy.dMnPsC = dMnPsc;
		RptBdcy.dMnPsT = dMnPsc_t;
		RptBdcy.dMnBarT = dMnStc_t;
		RptBdcy.dMnPsTs = dMnPsc_s;
		RptBdcy.dMnBarTs = dMnStc_s;
		RptBdcy.dMnPsCs = dMnCon_s;

		RptBdcy.arFlex.Copy(arFlex);

		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbar[2]; // 0=압축, 1=인장
		CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> arTndn[2]; // 0=압축, 1=인장

		for (i = 0; i < 2; i++)
		{
			arRbar[i].RemoveAll();
			arTndn[i].RemoveAll();
		}

		// Tendon.
		for (i = 0; i < rarTndnPosi.GetSize(); i++)
		{
			_DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
			if (dMd >= 0.0)
			{
				if (Tndn.dz > 0.0) { arTndn[0].Add(Tndn); }
				else if (Tndn.dz < 0.0) { arTndn[1].Add(Tndn); }
			}
			else if (dMd < 0.0)
			{
				if (Tndn.dz < 0.0) { arTndn[0].Add(Tndn); }
				else if (Tndn.dz > 0.0) { arTndn[1].Add(Tndn); }
			}
		}

		// Rebar.
		for (i = 0; i < rarRbarPosi.GetSize(); i++)
		{
			_DGN_RBAR_CRC Rbar = rarRbarPosi.GetAt(i);
			if (dMd >= 0.0)
			{
				if (Rbar.dz > 0.0) { arRbar[0].Add(Rbar); }
				else if (Rbar.dz < 0.0) { arRbar[1].Add(Rbar); }
			}
			else if (dMd < 0.0)
			{
				if (Rbar.dz < 0.0) { arRbar[0].Add(Rbar); }
				else if (Rbar.dz > 0.0) { arRbar[1].Add(Rbar); }
			}
		}

		for (i = 0; i < 2; i++)
		{
			RptBdcy.arRbar[i].Copy(arRbar[i]);
			RptBdcy.arTndn[i].Copy(arTndn[i]);
		}

	}

	//++++++++++++++++++++++++++
	// For zzzDataForDgn_JTG_D62_04.txt
	if (m_bTestMode)
	{
		const auto it = m_aSelElemK.find(ElemK);
		if (it != m_aSelElemK.end())
		{
			rptwofstream fout(m_strTestDgnFile, ios::app);

			// 
			CString strXcal;    strXcal.Format(_T("%16g"), dx_Cal);
			CString strX;       strX.Format(_T("%16g"), dx);
			CString strxib;     strxib.Format(_T("%16g"), dxb);
			CString strb;       strb.Format(_T("%16g"), db);
			CString strbw;      strbw.Format(_T("%16g"), dbw);
			CString strdh;      strdh.Format(_T("%16g"), dh);
			CString strdh0;     strdh0.Format(_T("%16g"), dh0);
			CString strdhf;     strdhf.Format(_T("%16g"), dhf);
			CString strat;      strat.Format(_T("%16g"), dat);
			CString strac;      strac.Format(_T("%16g"), dac);
			fout << _T(" x_cal : ") << strXcal << endl
				<< _T(" x     : ") << strX << endl
				<< _T(" xi_b  : ") << strxib << endl
				<< _T(" b     : ") << strb << endl
				<< _T(" bw    : ") << strbw << endl
				<< _T(" h     : ") << strdh << endl
				<< _T(" h0    : ") << strdh0 << endl
				<< _T(" hf    : ") << strdhf << endl
				<< _T(" a     : ") << strat << endl
				<< _T(" a'    : ") << strac << endl
				<< endl;

			double dTndnStress = 0.0; //dTndnComp와 같음
			for (i = 0; i < rarTndnPosi.GetSize(); i++)
			{
				_DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
				double dApci = 0.0, dapci = 0.0;
				dApci = Tndn.dArea;
				double dfpdci = Tndn.dfpdc;
				// Sig_po = Sig_con - Sig_l + Sig_l4.
				double dSigpoc = Tndn.dStress[0] - Tndn.dStrLoss[0][0]; //[0][0] : x-Elastic 손실.

				// (f'pd-Sig'po)
				CString strTndnGrup = Tndn.strTndnGrup;
				CString strTndnComp = _T("압축영역 프리스트레스 철근의 응력");
				if (dMd >= 0.0)
				{
					// Ap
					if (Tndn.dz > 0.0)
					{
						double dCpsi = dfpdci - dSigpoc;
						dTndnStress += (dfpdci - dSigpoc);
						CString strTndnStress;    strTndnStress.Format(_T(" %s : (%8g-%8g) = %8g"), strTndnGrup, dfpdci, dSigpoc, dCpsi);
						fout << strTndnStress << endl;
					}
				}
				else if (dMd < 0.0)
				{
					if (Tndn.dz < 0.0)
					{
						double dCpsi = dfpdci - dSigpoc;
						dTndnStress += (dfpdci - dSigpoc);
						CString strTndnStress;    strTndnStress.Format(_T(" %s : (%8g-%8g) = %8g"), dfpdci, dSigpoc, dCpsi);
						fout << strTndnStress << endl;
					}
				}
			}
			CString strTotalTndnComp; strTotalTndnComp.Format(_T(" (f'pd-Sig'po) = %8g"), dTndnStress);
			fout << strTotalTndnComp << endl << endl;

			CStringArray arMnStc; arMnStc.RemoveAll();
			CStringArray arMnStc_t; arMnStc_t.RemoveAll();
			CStringArray arMnStc_s; arMnStc_s.RemoveAll();
			double dDum_MnStc = 0.0, dDum_MnStc_t = 0.0, dDum_MnStc_s = 0.0;
			for (i = 0; i < rarRbarPosi.GetSize(); i++)
			{
				_DGN_RBAR_CRC Rbar = rarRbarPosi.GetAt(i);
				double dAsci = 0.0, dasci = 0.0;
				dAsci = Rbar.dArea;
				CString strRbarNa = Rbar.strRbarNa;
				if (dMd >= 0.0)
				{
					if (Rbar.dz > 0.0)
					{
						dasci = dx_Posi - Rbar.dz;
						double dMnStci = dfsdc * dAsci*(dh0 - dasci);
						dDum_MnStc += dfsdc * dAsci*(dh0 - dasci);
						CString strMnStc;
						strMnStc.Format(_T(" %s : %5g*%8g*(%8g-%8g) = %16g"), strRbarNa, dfsdc, dAsci, dh0, dasci, dMnStci);
						arMnStc.Add(strMnStc);
					}
					else
					{
						double dasi = (dh - dx_Posi) - fabs(Rbar.dz);
						double dMnStc_ti = dfsd * Rbar.dArea*(dh - dasi - dac);
						dDum_MnStc_t += dfsd * Rbar.dArea*(dh - dasi - dac);//식(5.2.5-1) 두번째항.
						CString strMnStc_t;
						strMnStc_t.Format(_T(" %s : %5g*%8g*(%8g-%8g-%8g) = %16g"), strRbarNa, dfsd, dAsci, dh, dasi, dac, dMnStc_ti);
						arMnStc_t.Add(strMnStc_t);

						double MnStc_si = dfsd * Rbar.dArea*(dh - dasi - dasc);
						dDum_MnStc_s += dfsd * Rbar.dArea*(dh - dasi - dasc);
						CString strMnStc_s;
						//double dasi = dh - (dx+fabs(Rbar.dz));
						strMnStc_s.Format(_T(" %s : %5g*%8g*(%8g-%8g-%8g) = %16g"), strRbarNa, dfsd, dAsci, dh, dasi, dasc, MnStc_si);
						arMnStc_s.Add(strMnStc_s);

					}
				}
				else if (dMd < 0.0)
				{
					if (Rbar.dz < 0.0)
					{
						dasci = dx_Posi - fabs(Rbar.dz);
						double dMnStci = dfsdc * dAsci*(dh0 - dasci);
						dDum_MnStc += dfsdc * dAsci*(dh0 - dasci);
						CString strMnStc; strMnStc.Format(_T(" %s : %5g*%8g*(%8g-%8g) = %16g"),
							strRbarNa, dfsdc, dAsci, dh0, dasci, dMnStci);
						arMnStc.Add(strMnStc);
					}
					else
					{
						double dasi = (dh - dx) - fabs(Rbar.dz);
						double dMnStc_ti = dfsd * Rbar.dArea*(dh - dasi - dac);
						dDum_MnStc_t += dfsd * Rbar.dArea*(dh - dasi - dac);
						CString strMnStc_t;
						strMnStc_t.Format(_T(" %s : %5g*%8g*(%8g-%8g-%8g) = %16g"), strRbarNa, dfsd, dAsci, dh, dasi, dac, dMnStc_ti);
						arMnStc_t.Add(strMnStc_t);

						double MnStc_si = dfsd * Rbar.dArea*(dh - dasi - dasc);
						dDum_MnStc_s += dfsd * Rbar.dArea*(dh - dasi - dasc);
						CString strMnStc_s;
						//double dasi = dh - (dx+fabs(Rbar.dz));
						strMnStc_s.Format(_T(" %s : %5g*%8g*(%8g-%8g-%8g) = %16g"), strRbarNa, dfsd, dAsci, dh, dasi, dasc, MnStc_si);
						arMnStc_s.Add(strMnStc_s);
					}
				}
			}

			CStringArray arMnPsc; arMnPsc.RemoveAll();
			CStringArray arMnPsc_t; arMnPsc_t.RemoveAll();
			CStringArray arMnPsc_s; arMnPsc_s.RemoveAll();
			CStringArray arMnCon_s; arMnCon_s.RemoveAll();
			double dDum_MnPsc = 0.0, dDum_MnPsc_t = 0.0, dDum_MnPsc_s = 0.0, dDum_MnCon_s = 0.0;
			for (i = 0; i < rarTndnPosi.GetSize(); i++)
			{
				_DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
				double dApci = 0.0, dapci = 0.0;
				dApci = Tndn.dArea;
				double dfpdci = Tndn.dfpdc;
				double dfpdti = Tndn.dfpdt;
				CString strTndnGroup = Tndn.strTndnGrup;
				// Sig_po = Sig_con - Sig_l + Sig_l4.
				double dSigpoc = Tndn.dStress[0] - Tndn.dStrLoss[0][0]; //[0][0] : x-Elastic 손실.
				if (dMd >= 0.0)
				{
					// Ap
					if (Tndn.dz > 0.0)
					{
						double dCpsi = (dfpdci - dSigpoc)*Tndn.dArea;
						dapci = dx_Posi - Tndn.dz;
						double dMnPsci = dCpsi * (dh0 - dapci);
						dDum_MnPsc += dCpsi * (dh0 - dapci);
						double dMnCon_si = dCpsi * (dapci - dasc);
						dDum_MnCon_s += dCpsi * (dapci - dasc);
						//dTndnComp += (dfpdci-dSigpoc);

						CString strMnPsc;
						strMnPsc.Format(_T(" %s : (%5g-%8g)*%8g*(%8g-%8g) = %16g"),
							strTndnGroup, dfpdci, dSigpoc, dApci, dh0, dapci, dMnPsci);
						arMnPsc.Add(strMnPsc);

						CString strMnCon_s;
						strMnCon_s.Format(_T(" %s : (%5g-%8g)*%8g*(%8g-%8g) = %16g"),
							strTndnGroup, dfpdci, dSigpoc, dApci, dapci, dasc, dMnCon_si);
						arMnCon_s.Add(strMnCon_s);
					}
					else
					{
						double dapi = (dh - dx_Posi) - fabs(Tndn.dz);
						double dMnPsc_ti = dfpdti * Tndn.dArea*(dh - dapi - dac);
						dMnPsc_t += dfpdti * Tndn.dArea*(dh - dapi - dac); //식(5.2.5-1) 첫번째항.
						double dMnPsc_si = dfpdti * Tndn.dArea*(dh - dapi - dasc);
						dMnPsc_s += dfpdti * Tndn.dArea*(dh - dapi - dasc);

						CString strMnPsc_t;
						strMnPsc_t.Format(_T(" %s : %5g*%8g*(%8g-%8g-%8g) = %16g"),
							strTndnGroup, dfpdti, Tndn.dArea, dh, dapi, dac, dMnPsc_ti);
						arMnPsc_t.Add(strMnPsc_t);

						CString strMnPsc_s;
						strMnPsc_s.Format(_T(" %s : %5g*%8g*(%8g-%8g-%8g) = %16g"),
							strTndnGroup, dfpdti, Tndn.dArea, dh, dapi, dasc, dMnPsc_si);
						arMnPsc_t.Add(strMnPsc_s);
					}
				}
				else if (dMd < 0.0)
				{
					if (Tndn.dz < 0.0)
					{
						double dCpsi = (dfpdci - dSigpoc)*Tndn.dArea;
						dapci = dx_Posi - fabs(Tndn.dz);
						double dMnPsci = dCpsi * (dh0 - dapci);
						dDum_MnPsc += dCpsi * (dh0 - dapci);
						double dMnCon_si = dCpsi * (dapci - dasc);
						dDum_MnCon_s += dCpsi * (dapci - dasc);
						//dTndnComp += (dfpdci-dSigpoc);

						CString strMnPsc;
						strMnPsc.Format(_T(" %s : (%5g-%8g)*%8g*(%8g-%8g) = %16g"),
							strTndnGroup, dfpdci, dSigpoc, dApci, dh0, dapci, dMnPsci);
						arMnPsc.Add(strMnPsc);

						CString strMnCon_s;
						strMnCon_s.Format(_T(" %s : (%5g-%8g)*%8g*(%8g-%8g) = %16g"),
							strTndnGroup, dfpdci, dSigpoc, dApci, dapci, dasc, dMnCon_si);
						arMnCon_s.Add(strMnCon_s);

					}
					else
					{
						double dapi = (dh - dx_Posi) - fabs(Tndn.dz);
						double dMnPsc_ti = dfpdti * Tndn.dArea*(dh - dapi - dac);
						dMnPsc_t += dfpdti * Tndn.dArea*(dh - dapi - dac); //식(5.2.5-1) 첫번째항.
						double dMnPsc_si = dfpdti * Tndn.dArea*(dh - dapi - dasc);
						dMnPsc_s += dfpdti * Tndn.dArea*(dh - dapi - dasc);

						CString strMnPsc_t;
						strMnPsc_t.Format(_T(" %s : %5g*%8g*(%8g-%8g-%8g) = %16g"),
							strTndnGroup, dfpdti, Tndn.dArea, dh, dapi, dac, dMnPsc_ti);
						arMnPsc_t.Add(strMnPsc_t);

						CString strMnPsc_s;
						strMnPsc_s.Format(_T(" %s : %5g*%8g*(%8g-%8g-%8g) = %16g"),
							strTndnGroup, dfpdti, Tndn.dArea, dh, dapi, dasc, dMnPsc_si);
						arMnPsc_t.Add(strMnPsc_s);
					}
				}
			}

			//CString strTndnComp;      strTndnComp.Format(_T("%16g"),dTndncomp);
			if (dTndnComp > 0.0)
			{
				if (dx >= 2.0*dac)
				{
					CString strxLG2ac;    strxLG2ac.Format(_T("%s"), _T("x > 2*a'"));
					fout << _T("  ") << strxLG2ac << endl;
					if (dx <= dhf)
					{
						CString strxSMhf = _T(" x < hf");
						fout << _T("  ") << strxSMhf << endl;

						dMnCon = dfcd * db*dx*(dh0 - dx / 2.0);
						CString strMnConCalc1 = _T("  Mn_con = fcd*b*x*(h0 - x/2)");
						CString strMnConCalc2;  strMnConCalc2.Format(_T("= %5g*%8g*%8g*(%8g-%8g/2) = %16g"), dfcd, db, dx, dh0, dx, dMnCon);
						fout << strMnConCalc1 << endl
							<< _T("     ") << strMnConCalc2 << endl;
					}
					else if (dx > dhf)
					{
						CString strxLGhf = _T(" x > hf");
						fout << _T("  ") << strxLGhf << endl;

						dMnCon = dfcd * (dbw*dx*(dh0 - dx / 2.0) + (db - dbw)*dhf*(dh0 - dhf / 2.0));
						CString strMnConCalc1 = _T("  Mn_con = fcd*(bw*x*(h0-x/2) + (b-bw)*hf*(h0 -hf/2))");
						CString strMnConCalc2;  strMnConCalc2.Format(_T("= %5g*(%8g*%8g*(%8g-%8g/2)+(%8g-%8g)*%8g*(%8g-%8g/2)) = %16g")
							, dfcd, dbw, dx, dh0, dx, db, dbw, dhf, dh0, dhf, dMnCon);
						fout << strMnConCalc1 << endl
							<< _T("     ") << strMnConCalc2 << endl;
					}


					fout << _T("* Mn_bar : f'sd*A's(h0-a's)") << endl;
					for (i = 0; i < arMnStc.GetSize(); i++)
					{
						CString str = arMnStc.GetAt(i);
						fout << str << endl;
					}
					fout << endl;

					fout << _T("* Mn_ps : (f'pd-Sig'po)A'p(h0-a'p)") << endl;
					for (i = 0; i < arMnPsc.GetSize(); i++)
					{
						CString str = arMnPsc.GetAt(i);
						fout << str << endl;
					}
					fout << endl;


					CString strMnText = _T(" Mn = Mn_con + Mn_bar + Mn_ps");
					dMn = dMnCon + dMnStc + dMnPsc;
					CString strMn;    strMn.Format(_T("    = %16g+%16g+%16g = %16g"), dMnCon, dMnStc, dMnPsc, dMn);
					fout << strMnText << endl
						<< strMn << endl;
					if (dMn < 0.0) dMn = dMnPsc_t + dMnStc_t;
				}
				else
				{
					CString strxSM2ac;    strxSM2ac.Format(_T("%s"), _T("x < 2*a'"));
					fout << _T("  ") << strxSM2ac << endl;

					fout << _T("* Mn_ps : fpd*Ap(h-ap-a')") << endl;
					for (i = 0; i < arMnPsc_t.GetSize(); i++)
					{
						CString str = arMnPsc_t.GetAt(i);
						fout << str << endl;
					}
					fout << endl;

					fout << _T("* Mn_bar : fsd*As(h-as-a')") << endl;
					for (i = 0; i < arMnStc_t.GetSize(); i++)
					{
						CString str = arMnStc_t.GetAt(i);
						fout << str << endl;
					}
					fout << endl;

					CString strMnText = _T(" Mn = Mn_ps + Mn_bar");
					dMn = dMnPsc_t + dMnStc_t;
					CString strMn;    strMn.Format(_T("    = %16g+%16g = %16g"), dMnPsc_t, dMnStc_t, dMn);
					fout << strMnText << endl
						<< strMn << endl;
				}
			}
			else if (dTndnComp <= 0.0)
			{
				if (dx >= 2.0*dasc)
				{
					CString strxLG2asc;    strxLG2asc.Format(_T("%s"), _T("x > 2*a's"));
					fout << _T("  ") << strxLG2asc << endl;

					if (dx <= dhf)
					{
						CString strxSMhf = _T(" x < hf");
						fout << _T("  ") << strxSMhf << endl;

						dMnCon = dfcd * db*dx*(dh0 - dx / 2.0);
						CString strMnConCalc1 = _T("  Mn_con = fcd*b*x*(h0 - x/2)");
						CString strMnConCalc2;  strMnConCalc2.Format(_T("= %5g*%8g*%8g*(%8g-%8g/2) = %16g"), dfcd, db, dx, dh0, dx, dMnCon);
						fout << strMnConCalc1 << endl
							<< _T("     ") << strMnConCalc2 << endl;
					}
					else if (dx > dhf)
					{
						CString strxLGhf = _T(" x > hf");
						fout << _T("  ") << strxLGhf << endl;

						dMnCon = dfcd * (dbw*dx*(dh0 - dx / 2.0) + (db - dbw)*dhf*(dh0 - dhf / 2.0));
						CString strMnConCalc1 = _T("  Mn_con = fcd*(bw*x*(h0-x/2) + (b-bw)*hf*(h0-hf/2))");
						CString strMnConCalc2;  strMnConCalc2.Format(_T("= %5g*(%8g*%8g*(%8g-%8g/2)+(%8g-%8g)*%8g*(%8g-%8g/2)) = %16g")
							, dfcd, dbw, dx, dh0, dx, db, dbw, dhf, dh0, dhf, dMnCon);
						fout << strMnConCalc1 << endl
							<< _T("     ") << strMnConCalc2 << endl;
					}

					fout << _T("* Mn_bar : f'sd*A's(h0-a's)") << endl;
					for (i = 0; i < arMnStc.GetSize(); i++)
					{
						CString str = arMnStc.GetAt(i);
						fout << str << endl;
					}
					fout << endl;

					fout << _T("* Mn_ps : (f'pd-Sig'po)A'p(h0-a'p)") << endl;
					for (i = 0; i < arMnPsc.GetSize(); i++)
					{
						CString str = arMnPsc.GetAt(i);
						fout << str << endl;
					}
					fout << endl;


					CString strMnText = _T(" Mn = Mn_con + Mn_bar + Mn_ps");
					dMn = dMnCon + dMnStc + dMnPsc;
					CString strMn;    strMn.Format(_T("    = %16g+%16g+%16g = %16g"), dMnCon, dMnStc, dMnPsc, dMn);
					fout << strMnText << endl
						<< strMn << endl;

					if (dMn < 0.0) dMn = dMnPsc_s + dMnStc_s - dMnCon_s;
				}
				else
				{
					CString strxSM2ac;    strxSM2ac.Format(_T("%s"), _T("x < 2*a'"));
					fout << _T("  ") << strxSM2ac << endl;

					fout << _T("* Mn_ps : fpd*Ap(h-ap-a's)") << endl;
					for (i = 0; i < arMnPsc_s.GetSize(); i++)
					{
						CString str = arMnPsc_s.GetAt(i);
						fout << str << endl;
					}
					fout << endl;

					fout << _T("* Mn_bar : fsd*As(h-as-a's)") << endl;
					for (i = 0; i < arMnStc_s.GetSize(); i++)
					{
						CString str = arMnStc_s.GetAt(i);
						fout << str << endl;
					}

					fout << _T("* Mn_comp_tndn : (f'pd-Sig'po)A'p(a'p-a's)") << endl;
					for (i = 0; i < arMnCon_s.GetSize(); i++)
					{
						CString str = arMnCon_s.GetAt(i);
						fout << str << endl;
					}

					fout << endl;

					CString strMnText = _T(" Mn = Mn_ps + Mn_bar - Mn_comp_tndn");
					dMn = dMnPsc_s + dMnStc_s - dMnCon_s;
					CString strMn;    strMn.Format(_T("    = %16g+%16g+%16g = %16g"), dMnPsc_s, dMnStc_s, dMnCon_s, dMn);
					fout << strMnText << endl
						<< strMn << endl;
				}
			}

		}

	}
	return bCalcOK;

}

BOOL CDgnJTG_D62_04::Get_ShearDesign(ElemPairK ElemK, BOOL bYaxis, UINT iIJ, double dgam0, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, double dShrThk,
	CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, T_CUMS_JTG_BASE& CumsJtg, _RPT_CUMS_BASE& RptCums)
{
	T_MATD_D MatdD = MatlElem.MatdD;

	double dfcuk = MatdD.Data1.Design.C_fc;
	double dftd = MatlElem.dftd;       // Concrete
	// Change by Jaeoh. ('05.06.20)
	double dfsd = MatlElem.dfsdtM;     // 주철근. dfsdtD -> dfsdtM.
	double dfsv = MatlElem.dfsdtS;     // 전단 stirrup: 
	double dfpd = MatlElem.dfpdtV[iIJ];  // 전단강봉.
	double dfpdc = MatlElem.dfpdcV[iIJ];

	// Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy = dyDim - dyCen;
	double dCpz = dzDim - dzCen;
	double dA0 = SpscSect.dArea;  // 전체환산단면적.
	double dIyy = SpscSect.dIyy;
	double dIzz = SpscSect.dIzz;
	double dh = dzDim;

	double dbw = dShrThk;
	double dAsv = RpscSbar.dStirAw;    //Stirrup 
	double dSv_s = RpscSbar.dStirPitch;
	double dAsb = RpscSbar.dSbarAw;     //경사 철근.
	double dSv_b = RpscSbar.dSbarPitch;  //경사철근 간격.
	double dThetas = RpscSbar.dSbarTheta;
	double dAsv1 = RpscSbar.dLbarAwt;
	double dSv_t = RpscSbar.dLbarPitch;
	double dApbar = RpscSbar.dTndnAp;
	double dSv_p = RpscSbar.dTndnDist;
	double dThetap = RpscSbar.dTndnTheta;

	//Force
	double dMd = CumsJtg.dMu;
	double dMd_Tnpr = CumsJtg.dMu_P;
	double dVd = CumsJtg.dVu;

	double dAp = 0.0, dApb = 0.0;
	double dApt = 0.0, dNtodpt = 0.0, dapt = 0.0;
	double dApc = 0.0, dNtodpc = 0.0, dapc = 0.0;
	double dAst = 0.0, dNtodst = 0.0, dast = 0.0;
	double dAsc = 0.0, dNtodsc = 0.0, dasc = 0.0;
	double dat = 0.0, dac = 0.0;
	double dypc = 0.0, dypt = 0.0, dyst = 0.0, dysc = 0.0;

	double dr0 = dgam0;
	double dr0Vd = dr0 * dVd;
	double dalpha1, dalpha2, dalpha3;
	BOOL bCalcOK = Get_Alpha123(dMd, dMd_Tnpr, dalpha1, dalpha2, dalpha3);

	double dCp = 0.0, dCtemp = 0.0;
	if (dMd >= 0.0) dCp = (bYaxis == TRUE ? dCpz : dCpy);
	else         dCp = (bYaxis == TRUE ? dzCen : dyCen);

	dCtemp = (dMd >= 0.0 ? dCp : dh - dCp);
	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();
	bCalcOK = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dfsd);
	if (dMd >= 0.0)  // 정모멘트 
	{
		dApt = rTdRb.dApttol, dapt = (dApt == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dApc = rTdRb.dApctol, dapc = rTdRb.ddpc;
		dAst = rTdRb.dAsttol, dast = (dAst == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dAsc = rTdRb.dAsctol, dasc = rTdRb.ddsc;
		dat = rTdRb.daBott;
		dac = rTdRb.daTopc;
		dypt = dNtodpt; dypc = dNtodpc;
		dyst = dNtodst; dysc = dNtodsc;
	}
	else
	{
		dApt = rTdRb.dApctol, dapt = rTdRb.ddpc;
		dApc = rTdRb.dApttol, dapc = (dApc == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dAst = rTdRb.dAsctol, dast = rTdRb.ddsc;
		dAsc = rTdRb.dAsttol, dasc = (dAsc == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dat = rTdRb.daTopt;
		dac = rTdRb.daBotc;
		dypt = rTdRb.dNtodpc; dypc = rTdRb.dNtodpt;
		dyst = rTdRb.dNtodsc; dysc = rTdRb.dNtodst;
	}

	double dVn = 0.0;
	double dVcs = 0.0, dVsb = 0.0, dVpb = 0.0;
	CArray<_RPT_TNDN_CUMS, _RPT_TNDN_CUMS&> arTndnCums; arTndnCums.RemoveAll();
	// Change by Jaeoh. ('05.04.12)
	// Vpb 계산. 
	int i;
	double dPi = 4 * atan(1.0);
	for (i = 0; i < arTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
		double dfpdi = rTndn.dfpdt;
		double dApbi = rTndn.dArea;
		double dStreX = rTndn.dStress[0]; // x방향 분력.
		double dStreY = rTndn.dStress[2]; // z방향 분력.
		double dStreXY = sqrt(dStreX*dStreX + dStreY * dStreY);
		double dSINt = (dStreXY == 0.0 ? 0.0 : dStreY / dStreXY);

		_RPT_TNDN_CUMS rTndnCums;
		rTndnCums.Initialize();
		if (fabs(dSINt) > m_dZero)
		{
			dApb += dApbi;
			dVpb += 0.75*dfpdi*dApbi*fabs(dSINt);
			if (m_bReport)
			{
				rTndnCums.strTndnGroup = rTndn.strTndnGrup;
				rTndnCums.bSlope = TRUE;
				rTndnCums.dAp = dApbi;
				rTndnCums.dfpd = dfpdi;
				double dThetap = asin(dSINt)*(180.0 / dPi);
				rTndnCums.dThetap = dThetap;
				// Add
				arTndnCums.Add(rTndnCums);
			}
		}
		else
		{
			BOOL bTen = FALSE;
			if (dMd >= 0.0)
			{
				if (rTndn.dz < 0.0)
				{
					dAp += dApbi;
					bTen = TRUE;
				}

			}
			else
			{
				if (rTndn.dz > 0.0)
				{
					dAp += dApbi;
					bTen = TRUE;
				}
			}

			if (m_bReport && bTen)
			{
				rTndnCums.strTndnGroup = rTndn.strTndnGrup;
				rTndnCums.bSlope = FALSE;
				rTndnCums.dAp = dApbi;
				rTndnCums.dfpd = dfpdi;
				rTndnCums.dThetap = 0.0;
				// Add
				arTndnCums.Add(rTndnCums);
			}

		}
	}

	//공식은 (KN)으로 되어 있기 때문에. (N)으로 표현하기 위해 공식에서 10E-3,10E-6은 고려하지 않음. 
	// Change by Jaeoh ('05.04.12) dast -> dat로 수정
	double dh0 = dh - dat;
	if (dat == 0.0) dh0 = 0.8*dh;
	double dRho = (dbw*dh0 == 0.0 ? 0.0 : (dAp + dApb + dAst) / (dbw*dh0));
	double dPcal = 100.0*dRho;
	double dP = dPcal;
	double dRhosv = (dSv_s*dbw == 0.0 ? 0.0 : dAsv / (dSv_s*dbw));
	double dm_cal = (dVd*dh0 == 0.0 ? 0.0 : dMd / (dVd*dh0));
	double dm = dm_cal;
	dm = fabs(dm);
	if (dm > 3.0) dm = 3.0;
	double dC = 0.6*dm*dh0;
	int iNumAsb = (int)(dSv_b == 0.0 ? 0.0 : dC / dSv_b);
	int iNumApb = (int)(dSv_p == 0.0 ? 0.0 : dC / dSv_p);

	double dShrSect = 0.51*sqrt(dfcuk)*dbw*dh0;
	BOOL bShrSect = (fabs(dr0Vd) <= dShrSect ? TRUE : FALSE);
	double dShrLoadSkip = 0.50*dalpha2*dftd*dbw*dh0;
	BOOL bShrLoadSkip = (fabs(dr0Vd) <= dShrLoadSkip ? TRUE : FALSE);

	if (dP > 2.5) dP = 2.5;
	dVcs = dalpha1 * dalpha2*dalpha3*0.45*dbw*dh0*sqrt((2.0 + 0.6*dP)*sqrt(dfcuk)*dRhosv*dfsv);
	for (i = 0; i < iNumAsb; i++) dVsb += 0.75*dfsd*dAsb*sin(dThetas);
	//for(i=0; i<iNumApb; i++) dVpb += 0.75*dfpd*dApb*sin(dThetap); 

	// Stirrup이 배근되지 않은 경우, 식(5.2.10)에의해 콘크리트가 지지할 수 있는 전단력으로 한다. 
	// Change by Jaeoh.('05.07.22) Strirrup이 배근되지 않으면 0 으로 수정 ( CH요청 )
	dVn = dVcs + dVsb + dVpb;
	double dRatV = (dVn == 0.0 ? 0.0 : fabs(dr0Vd / dVn));

	double dxi = 0.6;
	double dSv_sreq = (dxi*dr0Vd == 0.0 ? 0.0 : pow(dalpha1, 2)*pow(dalpha2, 2)*(2.0 + 0.6*dP)*sqrt(dfcuk)*dAsv*dfsv*dbw*pow(dh0, 2) / pow(dxi*dr0Vd, 2));
	double dasv_bot = pow(dalpha1, 2)*pow(dalpha2, 2)*(2.0 + 0.6*dP)*sqrt(dfcuk)*dfsv*dbw*pow(dh0, 2);
	double dAsv_req = (dasv_bot == 0.0 ? 0.0 : dSv_s * pow(dxi*dr0Vd, 2) / dasv_bot);

	//휨부재의 경사면 휨재하력 검토 고려
	CumsJtg.dMu = dMd;
	CumsJtg.dVu = dVd;
	CumsJtg.drVu = dr0Vd;
	CumsJtg.dVn = dVn;
	CumsJtg.dRatV = dRatV;
	CumsJtg.dShrSect = dShrSect;
	CumsJtg.dShrLoad = dShrLoadSkip;

	if (m_bReport)
	{
		RptCums.dalp1 = dalpha1;
		RptCums.dalp2 = dalpha2;
		RptCums.dalp3 = dalpha3;
		RptCums.dfcuk = dfcuk;
		RptCums.dftd = dftd;
		RptCums.dfsdt = dfsd;
		RptCums.dfsv = dfsv;
		RptCums.dbw = dbw;
		RptCums.dh = dh;
		RptCums.dh0 = dh0;
		RptCums.dAsv = dAsv;
		RptCums.dSv_s = dSv_s;
		RptCums.dThetas = dThetas;
		RptCums.dAsb = dAsb;
		RptCums.dSv_b = dSv_b;
		RptCums.dMd = dMd;
		RptCums.dVd = dVd;
		RptCums.dr0Vd = dr0Vd;
		RptCums.dVn = dVn;
		RptCums.dVcs = dVcs;
		RptCums.dVsb = dVsb;
		RptCums.dVpb = dVpb;
		RptCums.dAp = dAp;
		RptCums.dApb = dApb;
		RptCums.dAst = dAst;
		RptCums.dP = dP;
		RptCums.dPcal = dPcal;
		RptCums.dRho = dRho;
		RptCums.dRhosv = dRhosv;
		RptCums.dm = dm;
		RptCums.dm_cal = dm_cal;
		RptCums.dC = dC;
		RptCums.iAsbN = iNumAsb;
		RptCums.dShrSect = dShrSect;
		RptCums.dShrLoadSkip = dShrLoadSkip;

		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarT; // 인장철근    

		// Rebar.
		for (i = 0; i < arRbarPosi.GetSize(); i++)
		{
			_DGN_RBAR_CRC Rbar = arRbarPosi.GetAt(i);
			if (dMd >= 0.0)
			{
				if (Rbar.dz < 0.0) { arRbarT.Add(Rbar); }
			}
			else if (dMd < 0.0)
			{
				if (Rbar.dz > 0.0) { arRbarT.Add(Rbar); }
			}
		}

		RptCums.arRbarT.Copy(arRbarT);
		RptCums.arTndnCums.Copy(arTndnCums);

	}

	//++++++++++++++++++++++++++
	// For zzzDataForDgn_JTG_D62_04.txt
	if (m_bTestMode)
	{
		const auto it = m_aSelElemK.find(ElemK);
		if (it != m_aSelElemK.end())
		{
			rptwofstream fout(m_strTestDgnFile, ios::app);

			// Reinforcement  
			CString strAsv;    strAsv.Format(_T("%16g"), dAsv);    //Stirrup 
			CString strSv_s;   strSv_s.Format(_T("%16g"), dSv_s);
			CString strAsb;    strAsb.Format(_T("%16g"), dAsb);     //경사 철근.
			CString strSv_b;   strSv_b.Format(_T("%16g"), dSv_b);    //경사철근 간격.
			CString strThetas; strThetas.Format(_T("%16g"), dThetas);
			CString strAsv1;   strAsv1.Format(_T("%16g"), dAsv1);
			CString strSv_t;   strSv_t.Format(_T("%16g"), dSv_t);
			CString strApb;    strApb.Format(_T("%16g"), dApb);
			CString strSv_p;   strSv_p.Format(_T("%16g"), dSv_p);
			CString strThetap; strThetap.Format(_T("%16g"), dThetap);
			fout << _T("  Reinforce") << endl
				<< _T(" Asv    : ") << strAsv << endl
				<< _T(" Sv_s   : ") << strSv_s << endl
				<< _T(" Asb    : ") << strAsb << endl
				<< _T(" Sv_b   : ") << strSv_b << endl
				<< _T(" Thetas : ") << strThetas << endl
				<< _T(" Asv1   : ") << strAsv1 << endl
				<< _T(" Sv_t   : ") << strSv_t << endl
				<< _T(" Apb    : ") << strApb << endl
				<< _T(" Sv_p   : ") << strSv_p << endl
				<< _T(" Thetap : ") << strThetap << endl
				<< endl;

			CString strMd;  strMd.Format(_T("%16g"), dMd);
			CString strMdtnpr;  strMdtnpr.Format(_T("%16g"), dMd_Tnpr);
			CString strVd;  strVd.Format(_T("%16g"), dVd);
			fout << _T(" Md : ") << strMd << endl
				<< _T(" Vd : ") << strVd << endl
				<< _T(" Md_Tnpr : ") << strMdtnpr << endl
				<< endl;

			// Vcs
			CString stral1;   stral1.Format(_T("%16g"), dalpha1);
			CString stral2;   stral2.Format(_T("%16g"), dalpha2);
			CString stral3;   stral3.Format(_T("%16g"), dalpha3);
			CString strb;     strb.Format(_T("%16g"), dbw);
			CString stra;     stra.Format(_T("%16g"), dat);
			CString strh0;    strh0.Format(_T("%16g"), dh0);
			CString strApt;   strApt.Format(_T("%16g"), dApt);
			CString strAst;   strAst.Format(_T("%16g"), dAst);
			CString strRho;   strRho.Format(_T("%16g"), dRho);
			CString strP;     strP.Format(_T("%16g"), dP);
			CString strfcuk;  strfcuk.Format(_T("%16g"), dfcuk);
			CString strRhosv; strRhosv.Format(_T("%16g"), dRhosv);
			CString strfsv;   strfsv.Format(_T("%16g"), dfsv);
			CString strVcs;   strVcs.Format(_T("%16g"), dVcs);
			fout << _T("  Vcs") << endl
				<< _T(" alpha1 : ") << stral1 << endl
				<< _T(" alpha2 : ") << stral2 << endl
				<< _T(" alpha3 : ") << stral3 << endl
				<< _T(" bw     : ") << strb << endl
				<< _T(" a      : ") << stra << endl
				<< _T(" h0     : ") << strh0 << endl
				<< _T(" Apt    : ") << strApt << endl
				<< _T(" Apb    : ") << strApb << endl
				<< _T(" Ast    : ") << strAst << endl
				<< _T(" Rho    : ") << strRho << endl
				<< _T(" P      : ") << strP << endl
				<< _T(" fcuk   : ") << strfcuk << endl
				<< _T(" Rhosv  : ") << strRhosv << endl
				<< _T(" fsv    : ") << strfsv << endl
				<< _T(" Vcs    : ") << strVcs << endl
				<< endl;

			// Vsb, Vpb
			CString strm;      strm.Format(_T("%16g"), dm);
			CString strC;      strC.Format(_T("%16g"), dC);
			CString strNumAsb; strNumAsb.Format(_T("%16d"), iNumAsb);
			CString strNumApb; strNumApb.Format(_T("%16d"), iNumApb);
			CString strfsd;    strfsd.Format(_T("%16g"), dfsd);
			CString strfpd;    strfpd.Format(_T("%16g"), dfpd);
			CString strVsb;    strVsb.Format(_T("%16g"), dVsb);
			CString strVpb;    strVpb.Format(_T("%16g"), dVpb);
			fout << _T("  Vsb, Vpb") << endl
				<< _T(" m      : ") << strm << endl
				<< _T(" C      : ") << strC << endl
				<< _T(" NumAsb : ") << strNumAsb << endl
				<< _T(" NumApb : ") << strNumApb << endl
				<< _T(" fsd    : ") << strfsd << endl
				<< _T(" fpd    : ") << strfpd << endl
				<< _T(" Vsb    : ") << strVsb << endl
				<< _T(" Vpb    : ") << strVpb << endl
				<< endl;

			// Vn
			CString strVn; strVn.Format(_T("%16g"), dVn);
			fout << _T(" Vn : ") << strVn << endl
				<< endl;

			// ShrSect, ShrLoad
			CString strSect;     strSect.Format(_T("%16g"), dShrSect);
			CString strLoadSkip; strLoadSkip.Format(_T("%16g"), dShrLoadSkip);
			fout << _T(" ShrSect : ") << strSect << endl
				<< _T(" ShrLoad : ") << strLoadSkip << endl
				<< endl;
		}
	}

	return bCalcOK;
}

BOOL CDgnJTG_D62_04::Get_TorsionDesign(ElemPairK ElemK, BOOL bYaxis, UINT iIJ, double dgam0, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, BOOL bSolidSect, double dAoh, double dPh, double dWt, double dShrThk, double dFlgThk[2], double dTorThk,
	double dpMomPos[2][2], CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, T_CRMT_JTG_BASE& CrmtJtg, _RPT_CRMT_BASE& RptCrmt)
{
	T_MATD_D MatdD = MatlElem.MatdD;

	double dfcuk = MatdD.Data1.Design.C_fc;
	double dftd = MatlElem.dftd;       // Concrete
	double dfcd = MatlElem.dfcd;
	// Change by Jaeoh. ('05.06.20)
	double dfsd = MatlElem.dfsdtM;     // 주철근. dfsdtD -> dfsdtM
	double dfst = MatlElem.dfsdtS;     // 전단 stirrup
	double dfpd = MatlElem.dfpdtV[iIJ];  //전단강봉.
	double dfpdc = MatlElem.dfpdcV[iIJ];
	double dfsv = MatlElem.dfsdtS; //비틀림 stirrup, 

	// Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy = dyDim - dyCen;
	double dCpz = dzDim - dzCen;
	double dA0 = SpscSect.dArea;  // 전체환산단면적.
	double dIyy = SpscSect.dIyy;
	double dIzz = SpscSect.dIzz;
	//  double dWt   = dWt;   // Change by Jaeoh('05.03.18) Ixx -> dKt, Change by ZINU.('06.08.28). dKt -> dWt. 
	double dh = dzDim;
	double dyt = dzCen;
	double dI = dIyy;

	double dbw = dShrThk;
	double dAsv = RpscSbar.dStirAw;    //Stirrup 
	double dSv_s = RpscSbar.dStirPitch;
	double dAsv1 = RpscSbar.dLbarAwt;
	double dAslt = RpscSbar.dLbarAlt;
	double dSv_t = RpscSbar.dLbarPitch;
	double dAp = 0.0;
	double dApb = 0.0; // RpscSbar.dTndnAp;
	double dSv_p = 0.0; // RpscSbar.dTndnDist;

	//Force
	double dMd = CrmtJtg.dMu;
	double dMd_Tnpr = CrmtJtg.dMu_P;
	double dTd = fabs(CrmtJtg.dTu);
	double dTdr = CrmtJtg.dTu;
	double dVd = CrmtJtg.dVu;

	double bPureTorsion = (dVd == 0.0 && dMd == 0.0 ? TRUE : FALSE);

	double dApt = 0.0, dNtodpt = 0.0, dapt = 0.0;
	double dApc = 0.0, dNtodpc = 0.0, dapc = 0.0;
	double dAst = 0.0, dNtodst = 0.0, dast = 0.0;
	double dAsc = 0.0, dNtodsc = 0.0, dasc = 0.0;
	double dat = 0.0, dac = 0.0;
	double dypc = 0.0, dypt = 0.0, dyst = 0.0, dysc = 0.0;

	double dr0 = dgam0;
	double dr0Td = dr0 * dTd;
	double dr0Tdr = dr0 * dTdr;
	double dr0Vd = dr0 * dVd;
	double dalpha1, dalpha2, dalpha3;
	BOOL bCalcOK = Get_Alpha123(dMd, dMd_Tnpr, dalpha1, dalpha2, dalpha3);

	BOOL bTorsionChk = FALSE;
	double dCp = 0.0, dCtemp = 0.0;
	if (dMd >= 0.0) dCp = (bYaxis == TRUE ? dCpz : dCpy);
	else         dCp = (bYaxis == TRUE ? dzCen : dyCen);

	dCtemp = (dMd >= 0.0 ? dCp : dh - dCp);
	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();
	bCalcOK = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dfsd);
	if (dMd >= 0.0)  // 정모멘트 
	{
		dApt = rTdRb.dApttol, dapt = (dApt == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dApc = rTdRb.dApctol, dapc = rTdRb.ddpc;
		dAst = rTdRb.dAsttol, dast = (dAst == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dAsc = rTdRb.dAsctol, dasc = rTdRb.ddsc;
		dat = rTdRb.daBott;
		dac = rTdRb.daTopc;
		dypt = dNtodpt; dypc = dNtodpc;
		dyst = dNtodst; dysc = dNtodsc;
	}
	else
	{
		dApt = rTdRb.dApctol, dapt = rTdRb.ddpc;
		dApc = rTdRb.dApttol, dapc = (dApc == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dAst = rTdRb.dAsctol, dast = rTdRb.ddsc;
		dAsc = rTdRb.dAsttol, dasc = (dAsc == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dat = rTdRb.daTopt;
		dac = rTdRb.daBotc;
		dypt = dNtodpc; dypc = dNtodpt;
		dyst = dNtodsc; dysc = dNtodst;
	}

	// Pure Torsion Member.
	double dh0 = dh - dat;
	if (dat == 0.0) dh0 = 0.8*dh;
	double dAcor = dAoh;
	double dUcor = dPh;
	double dTn = 0.0;
	double dZeta = 0.0, dZetaCal;
	double dPsEffectParam = 0.0;
	double dBetaa = 0.0;
	double dt1 = 0.0, dt2 = 0.0;
	double dbf_top = dyDim;
	double dbf_bot = fabs(dpMomPos[0][0] - dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.

	if (!bSolidSect)
	{
		double db = min(dbf_top, dbf_bot); //dyDim;
		dt1 = dTorThk;
		dt2 = min(dFlgThk[0], dFlgThk[1]);

		if ((0.1*db <= dt2 && dt2 <= 0.25*db) || (0.1*dh <= dt1 && dt1 <= 0.25*dh))
		{
			double dba1 = (db == 0.0 ? 0.0 : 4.0*dt2 / db);
			double dba2 = (dh == 0.0 ? 0.0 : 4.0*dt1 / dh);
			dBetaa = min(dba1, dba2);
		}
		else if (dt2 > 0.25*db && dt1 > 0.25*dh) { dBetaa = 1.0; }
		else if (0.1*db > dt2 && 0.1*dh > dt1)
		{
			dBetaa = 1.0;
			//ASSERT(0);
		}
		else { dBetaa = 1.0; }
	}
	else dBetaa = 1.0;

	dZeta = (dfsv*dAsv1*dUcor == 0.0 ? 0.0 : dfsd * dAslt*dSv_t / (dfsv*dAsv1*dUcor));  //여기서 dfsv=비틀림 스트럽
	dZetaCal = dZeta;
	//if(dZeta<0.6)      dZeta = 0.6;
	double dNp0, dep0, dhps;
	bCalcOK = Get_SumForceEccent(TRUE, dMd, dCp, SpscSect, arRbarPosi, arTndnPosi, dNp0, dep0, dhps);
	if (dNp0 > 0.3*dfcd*dA0) dNp0 = 0.3*dfcd*dA0;
	dPsEffectParam = (dA0 == 0.0 ? 0.0 : 0.05*dNp0*dWt / dA0);
	if (fabs(dep0) <= dh / 6.0 && dZeta >= 1.7)
	{
		dZeta = 1.7;
	}
	else if (fabs(dep0) > dh / 6.0 || dZeta < 1.7)
	{
		// Change by Jaeoh ('05.06.08)
		if (dZeta < 0.6)      dZeta = 0.6;
		else if (dZeta > 1.7) dZeta = 1.7;
		dPsEffectParam = 0.0;
		dalpha2 = 1.0;
	}
	dTn = (dSv_t == 0.0 ? 0.35*dBetaa*dftd*dWt + dPsEffectParam : 0.35*dBetaa*dftd*dWt + 1.2*sqrt(dZeta)*dfsv*dAsv1*dAcor / dSv_t + dPsEffectParam);

	// Bending-Shear-Torsion
	CArray<_RPT_TNDN_CUMS, _RPT_TNDN_CUMS&> arTndnCums; arTndnCums.RemoveAll();
	double dPi = 4 * atan(1.0);
	for (int i = 0; i < arTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
		double dfpdi = rTndn.dfpdt;
		double dApbi = rTndn.dArea;
		double dStreX = rTndn.dStress[0]; // x방향 분력.
		double dStreY = rTndn.dStress[2]; // z방향 분력.
		double dStreXY = sqrt(dStreX*dStreX + dStreY * dStreY);
		double dSINt = (dStreXY == 0.0 ? 0.0 : dStreY / dStreXY);

		_RPT_TNDN_CUMS rTndnCums;
		rTndnCums.Initialize();
		if (fabs(dSINt) > m_dZero)
		{
			dApb += dApbi;
			if (m_bReport)
			{
				rTndnCums.strTndnGroup = rTndn.strTndnGrup;
				rTndnCums.bSlope = TRUE;
				rTndnCums.dAp = dApbi;
				rTndnCums.dfpd = dfpdi;
				double dThetap = asin(dSINt)*(180.0 / dPi);
				rTndnCums.dThetap = dThetap;
				// Add
				arTndnCums.Add(rTndnCums);
			}
		}
		else
		{
			BOOL bTen = FALSE;
			if (dMd >= 0.0)
			{
				if (rTndn.dz < 0.0)
				{
					dAp += dApbi;
					bTen = TRUE;
				}

			}
			else
			{
				if (rTndn.dz > 0.0)
				{
					dAp += dApbi;
					bTen = TRUE;
				}
			}

			if (m_bReport && bTen)
			{
				rTndnCums.strTndnGroup = rTndn.strTndnGrup;
				rTndnCums.bSlope = FALSE;
				rTndnCums.dAp = dApbi;
				rTndnCums.dfpd = dfpdi;
				rTndnCums.dThetap = 0.0;
				// Add
				arTndnCums.Add(rTndnCums);
			}

		}
	}

	double dRho = (dbw*dh0 == 0.0 ? 0.0 : (dAp + dApb + dAst) / (dbw*dh0));
	double dPcal = 100.0*dRho;
	double dP = dPcal;
	if (dP > 2.5) dP = 2.5;
	double dSTValue = 0.0;
	if (dbw*dh0 == 0.0 && dWt == 0.0) dSTValue = 0.0;
	else if (dbw*dh0 == 0.0) dSTValue = dr0 * dTd / dWt;
	else if (dWt == 0.0) dSTValue = dr0 * dVd / (dbw*dh0);
	else dSTValue = dr0 * fabs(dVd) / (dbw*dh0) + dr0 * fabs(dTd) / dWt;
	double dSTCheck = 0.51*sqrt(dfcuk);
	double dSTSkip = 0.50*dalpha2*dftd;

	BOOL bBSTCheck = FALSE;
	if (dSTValue <= dSTCheck) bBSTCheck = TRUE;
	BOOL bSTSkip = FALSE;
	if (dSTValue <= dSTSkip) bSTSkip = TRUE;

	double dBetat = 0.0;
	dBetat = (dTd*dbw*dh0 == 0.0 ? 0.0 : 1.5 / (1.0 + 0.5*fabs(dVd)*dWt / (dTd*dbw*dh0)));
	if (dBetat < 0.5) dBetat = 0.5;
	else if (dBetat > 1.0) dBetat = 1.0;
	if (!bSolidSect) dWt = dBetaa * dWt;
	// Shear.
	double dRhosv = (dSv_s*dbw == 0.0 ? 0.0 : dAsv / (dSv_s*dbw));
	double dShr1 = dalpha1 * dalpha2*dalpha3*(10.0 - 2.0*dBetat) / 20.0*dbw*dh0;
	double dShr2 = sqrt((2.0 + 0.6*dP)*sqrt(dfcuk)*dRhosv*dfsv);
	double dShrLoad = dShr1 * dShr2; //(N)
	// Stirrup이 배근되지 않은 경우, 식(5.2.10)에의해 콘크리트가 지지할 수 있는 전단력으로 한다. 
	// 전단설계와 같은 조건. 
	// Change by Jaeoh.('05.07.22) Strirrup이 배근되지 않으면 0 으로 수정 ( CH요청 )
	double dShrLoadSkip = 0.50*dalpha2*dftd*dbw*dh0;

	// Torsion.
	double dTor1 = dBetat * (0.35*dBetaa*dftd*dWt + dPsEffectParam);
	double dTor2 = (dSv_t == 0.0 ? 0.0 : 1.2*sqrt(dZeta)*dfsv*dAsv1*dAcor / dSv_t);
	double dTorLoad = dTor1 + dTor2; //(N.mm)
	// 종방향 필요철근면적.
	double dAslt1 = (dSv_t == 0.0 ? 0.0 : 1.2*dfsv*dAsv1*dAcor / dSv_t);
	double dAslt2 = (dAslt1 == 0.0 ? 0.0 : (dr0Td - dTor1) / dAslt1);
	if (dAslt2 < 0.0) dAslt2 = 0.0;
	double dAslt3 = (dfsv*dAsv1*dUcor == 0.0 ? 0.0 : dfsd * dSv_t / (dfsv*dAsv1*dUcor));
	double dAslt_req = (dAslt3 == 0.0 ? 0.0 : pow(dAslt2, 2) / dAslt3);
	// 비틀림 필요Stirrup.
	double dasv = dr0 * dTd - dBetat * (0.35*dBetaa*dftd*dWt + dPsEffectParam);
	double dAsv1_req = (1.2*sqrt(dZeta)*dfsv*dAcor == 0.0 ? 0.0 : dasv / (1.2*sqrt(dZeta)*dfsv*dAcor / dSv_t));
	if (dAsv1_req < 0.0)  dAsv1_req = 0.0;

	double dRatT = 0.0;
	if (dVd == 0.0 && dMd == 0.0) dRatT = (dTn == 0.0 ? 0.0 : fabs(dr0Td / dTn));
	else                     dRatT = (dTorLoad == 0.0 ? 0.0 : fabs(dr0Td / dTorLoad));

	CrmtJtg.dTu = dTdr; //dTd;		          //계수 비틀림모멘트					[Unit=Moment]
	CrmtJtg.drTu = dr0Tdr; //dr0Td;           //
	CrmtJtg.dMu = dMd;	 	          //계수 모멘트
	CrmtJtg.dVu = dVd;	            //계수 전단력
	CrmtJtg.drVu = dr0Vd;	            //계수 전단력
	CrmtJtg.dVn = (bPureTorsion ? 0.0 : dShrLoad);        //전단재하력.
	CrmtJtg.dTn = (bPureTorsion ? dTn : dTorLoad);        //비틀림강도 
	CrmtJtg.dRatT = dRatT;           //비틀림강도 안전률					[Unit=None]  
	CrmtJtg.dShrTorVal = dSTValue;
	CrmtJtg.dTorSect = dSTCheck;
	CrmtJtg.dTorLoad = dSTSkip;

	if (m_bReport)
	{
		RptCrmt.dalp1 = dalpha1;
		RptCrmt.dalp2 = dalpha2;
		RptCrmt.dalp3 = dalpha3;
		RptCrmt.dfcuk = dfcuk;
		RptCrmt.dftd = dftd;
		RptCrmt.dfsdt = dfsd;
		RptCrmt.dfsv = dfsv;
		RptCrmt.dbw = dbw;
		RptCrmt.dh = dh;
		RptCrmt.dh0 = dh0;
		RptCrmt.dAsv = dAsv;
		RptCrmt.dSv_s = dSv_s;
		RptCrmt.dMd = dMd;
		RptCrmt.dVd = dVd;
		RptCrmt.dr0Vd = dr0Vd;
		RptCrmt.dr0Td = dr0Tdr;
		RptCrmt.dTn = dTn;
		RptCrmt.dVn = dShrLoad;
		RptCrmt.dAp = dAp;
		RptCrmt.dApb = dApb;
		RptCrmt.dAst = dAst;
		RptCrmt.dP = dP;
		RptCrmt.dPcal = dPcal;
		RptCrmt.dRho = dRho;
		RptCrmt.dRhosv = dRhosv;
		RptCrmt.dSTstress = dSTValue;
		RptCrmt.dTorSect = dSTCheck;
		RptCrmt.dTorLoadSkip = dSTSkip;
		RptCrmt.dAsv1 = dAsv1;
		RptCrmt.dAslt = dAslt;
		RptCrmt.dSv_t = dSv_t;
		RptCrmt.dBetaa = dBetaa;
		RptCrmt.dBetat = dBetat;
		RptCrmt.dWt = dWt;
		RptCrmt.dAcor = dAcor;
		RptCrmt.dUcor = dUcor;
		RptCrmt.dNp0 = dNp0;
		RptCrmt.dep0 = dep0;
		RptCrmt.dA0 = dA0;
		RptCrmt.bPureTorsion = bPureTorsion;
		RptCrmt.dZeta = dZeta;
		RptCrmt.dZetaCal = dZetaCal;
		RptCrmt.bSolid = bSolidSect;


		//CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarT; // 인장철근    

		// Rebar.
		/*
		for(i=0; i<arRbarPosi.GetSize(); i++)
		{
		_DGN_RBAR_CRC Rbar = arRbarPosi.GetAt(i);
		if(dMd>=0.0)
		{
		if(Rbar.dz<0.0)  { arRbarT.Add(Rbar); }
		}
		else if(dMd<0.0)
		{
		if(Rbar.dz>0.0)  { arRbarT.Add(Rbar); }
		}
		}
		*/

		RptCrmt.arTndnCums.Copy(arTndnCums);

	}

	//++++++++++++++++++++++++++
	// For zzzDataForDgn_JTG_D62_04.txt
	if (m_bTestMode)
	{
		const auto it = m_aSelElemK.find(ElemK);
		if (it != m_aSelElemK.end())
		{
			rptwofstream fout(m_strTestDgnFile, ios::app);

			CString strMd;  strMd.Format(_T("%16g"), dMd);
			CString strMdtnpr;  strMdtnpr.Format(_T("%16g"), dMd_Tnpr);
			CString strVd;  strVd.Format(_T("%16g"), dVd);
			CString strTd;  strTd.Format(_T("%16g"), dTdr);
			fout << _T(" Md : ") << strMd << endl
				<< _T(" Vd : ") << strVd << endl
				<< _T(" Td : ") << strTd << endl
				<< _T(" Md_Tnpr : ") << strMdtnpr << endl
				<< endl;

			// Pure Torsion Member.
			CString strBeta_a;  strBeta_a.Format(_T("%16g"), dBetaa);
			CString strftd;     strftd.Format(_T("%16g"), dftd);
			CString strWt;      strWt.Format(_T("%16g"), dWt);
			CString strZeta;    strZeta.Format(_T("%16g"), dZeta);
			CString strfsv;     strfsv.Format(_T("%16g"), dfsv);
			CString strAsv1;    strAsv1.Format(_T("%16g"), dAsv1);
			CString strAcor;    strAcor.Format(_T("%16g"), dAcor);
			CString strSv_t;    strSv_t.Format(_T("%16g"), dSv_t);
			CString strPsParam; strPsParam.Format(_T("%16g"), dPsEffectParam);
			fout << _T("  Pure torsion load(순수 비틀림 재하력)") << endl
				<< _T(" Beta_a  : ") << strBeta_a << endl
				<< _T(" ftd     : ") << strftd << endl
				<< _T(" Wt      : ") << strWt << endl
				<< _T(" Zeta    : ") << strZeta << endl
				<< _T(" fsv     : ") << strfsv << endl
				<< _T(" Asv1    : ") << strAsv1 << endl
				<< _T(" Acor    : ") << strAcor << endl
				<< _T(" Sv_t    : ") << strSv_t << endl
				<< _T(" PsParam : ") << strPsParam << endl
				<< endl;

			// 전단재하력
			CString stral1;   stral1.Format(_T("%16g"), dalpha1);
			CString stral2;   stral2.Format(_T("%16g"), dalpha2);
			CString stral3;   stral3.Format(_T("%16g"), dalpha3);
			CString strb;     strb.Format(_T("%16g"), dbw);
			CString stra;     stra.Format(_T("%16g"), dat);
			CString strh0;    strh0.Format(_T("%16g"), dh0);
			CString strApt;   strApt.Format(_T("%16g"), dApt);
			CString strApb;   strApb.Format(_T("%16g"), dApb);
			CString strAst;   strAst.Format(_T("%16g"), dAst);
			CString strRho;   strRho.Format(_T("%16g"), dRho);
			CString strP;     strP.Format(_T("%16g"), dP);
			CString strfcuk;  strfcuk.Format(_T("%16g"), dfcuk);
			CString strRhosv; strRhosv.Format(_T("%16g"), dRhosv);
			CString strShrLoad;   strShrLoad.Format(_T("%16g"), dShrLoad);
			fout << _T("  Shear load(전단재하력)") << endl
				<< _T(" alpha1 : ") << stral1 << endl
				<< _T(" alpha2 : ") << stral2 << endl
				<< _T(" alpha3 : ") << stral3 << endl
				<< _T(" bw     : ") << strb << endl
				<< _T(" a      : ") << stra << endl
				<< _T(" h0     : ") << strh0 << endl
				<< _T(" Apt    : ") << strApt << endl
				<< _T(" Apb    : ") << strApb << endl
				<< _T(" Ast    : ") << strAst << endl
				<< _T(" Rho    : ") << strRho << endl
				<< _T(" P      : ") << strP << endl
				<< _T(" fcuk   : ") << strfcuk << endl
				<< _T(" Rhosv  : ") << strRhosv << endl
				<< _T(" fsv    : ") << strfsv << endl
				<< _T(" Vn     : ") << strShrLoad << endl
				<< endl;

			// 비틀림재하력
			CString strBeta_t;  strBeta_t.Format(_T("%16g"), dBetat);
			CString strNp0;     strNp0.Format(_T("%16g"), dNp0);
			CString strA0;      strA0.Format(_T("%16g"), dA0);
			CString strTorLoad; strTorLoad.Format(_T("%16g"), dTorLoad);

			fout << _T("  Torsion load(비틀림 재하력)") << endl
				<< _T(" Beta_a  : ") << strBeta_a << endl
				<< _T(" Beta_t  : ") << strBeta_t << endl
				<< _T(" ftd     : ") << strftd << endl
				<< _T(" Wt      : ") << strWt << endl
				<< _T(" Np0     : ") << strNp0 << endl
				<< _T(" A0      : ") << strA0 << endl
				<< _T(" Zeta    : ") << strZeta << endl
				<< _T(" fsv     : ") << strfsv << endl
				<< _T(" Asv1    : ") << strAsv1 << endl
				<< _T(" Acor    : ") << strAcor << endl
				<< _T(" Sv_t    : ") << strSv_t << endl
				<< _T(" PsParam : ") << strPsParam << endl
				<< _T(" Tn      : ") << strTorLoad << endl
				<< endl;

			// ShrTorValue, TorSect, TorLad
			CString strSTValue; strSTValue.Format(_T("%16g"), dSTValue);
			CString strSect;    strSect.Format(_T("%16g"), dSTCheck);
			CString strLoad;    strLoad.Format(_T("%16g"), dSTSkip);
			fout << _T(" STValue : ") << strSTValue << endl
				<< _T(" TorSect : ") << strSect << endl
				<< _T(" TorLoad : ") << strLoad << endl
				<< endl;
		}
	}


	return bCalcOK;
}
/*
BOOL CDgnJTG_D62_04::Get_CompressiveCheck(T_ELEM_K ElemK, BOOL bYaxis, double dgam0, double dl0, double dfcd, double dftd, double dbw, double dFlgThk[2], double dpMomPos[2][2], _PSC_MATL_ELEM& MatlElem,
T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_CSTCS_CH_BASE& CscsJtg)
{
if(arRbarPosi.GetSize()==0.0 && arTndnPosi.GetSize()==0.0) return FALSE;
T_MATD_D MatdD = MatlElem.MatdD;

// Material
double dEc = MatdD.Data1.Analysis.Elast;
double dfy = MatdD.MainRebarData.B_fy;
double dEs = MatdD.MainRebarData.B_Elast;
double dEp = 0.0;
double dfsd  = MatlElem.dfsdtM;
double dfsdc = MatlElem.dfsdcM;
double dEpsilon = 0.0;//┖cu
double dbeta = 0.0;//5.3.3
double dPhi = 1.0;//5.3.1
CString  strConcName = MatdD.Data1.CodeMatlName;   //살퀸皐돨깃뵀
if(strConcName==_T("C15") || strConcName==_T("C20") || strConcName==_T("C25") || strConcName==_T("C30") ||  strConcName==_T("C35") || strConcName==_T("C45") || strConcName==_T("C50"))
{ dEpsilon=0.0033;  dbeta=0.80;}
else if(strConcName==_T("C55")) { dEpsilon=0.00325; dbeta=0.79;}
else if(strConcName==_T("C60")) { dEpsilon=0.0032;  dbeta=0.78;}
else if(strConcName==_T("C65")) { dEpsilon=0.00315; dbeta=0.77;}
else if(strConcName==_T("C70")) { dEpsilon=0.0031;  dbeta=0.76;}
else if(strConcName==_T("C75")) { dEpsilon=0.00305; dbeta=0.75;}
else if(strConcName==_T("C80")) { dEpsilon=0.003;   dbeta=0.74;}
else return FALSE;

// Section
double dyDim = SpscSect.dyDim;
double dzDim = SpscSect.dzDim;
double dyCen = SpscSect.dyCen;
double dzCen = SpscSect.dzCen;
double dCpy  = dyDim - dyCen;
double dCpz  = dzDim - dzCen;
double dA0   = SpscSect.dArea;  // 전체환산단면적.
double dIyy  = SpscSect.dIyy;
double dIzz  = SpscSect.dIzz;
double dIxx  = SpscSect.dIxx;

//Force
double dMd = CscsJtg.dMd;
double dNd = CscsJtg.dNd;
if (dNd <1.0E-07 && dNd >-1.0E-07)  return TRUE;

int i=0,j=0;
double dgap=0.0;
double dAs=0.0, dAsY=0.0;
double dApt=0.0, dNtodpt=0.0, dapt=0.0;
double dApc=0.0, dNtodpc=0.0, dapc=0.0;
double dAst=0.0, dNtodst=0.0, dast=0.0;
double dAsc=0.0, dNtodsc=0.0, dasc=0.0;
double dat=0.0, dac=0.0;
//Tendon
CArray<double,double> arApti, arApci;
CArray<double,double> arapti, arapci;
//Rbar
CArray<double,double> arAsti, arAsci;
CArray<double,double> arasti, arasci;

double dNueC=0.0;
double dfst=0.0, dfsc=0.0;

double dMn=0.0, dr0Mn=0.0;
double dr0 = dgam0;
double dr0Md = dr0*dMd;
double dTOL = 0.001;

double dh  = (bYaxis==TRUE ? dzDim : dyDim);
double dbf_top = dyDim;
double dbf_bot = fabs(dpMomPos[0][0] - dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.
double db  = (dMd>=0.0 ? dbf_top : dbf_bot);
double dbt  = (dMd<0.0 ? dbf_top : dbf_bot);
double dLength = (bYaxis==TRUE ? dyDim : dzDim);

// 압축영영의 높이 x<a's 일 때.
double dApt_cent=0.0, dapt_cent=0.0;
double dApc_cent=0.0, dapc_cent=0.0;
double dAst_cent=0.0, dast_cent=0.0;
double dAsc_cent=0.0, dasc_cent=0.0;
double dat_cent =0.0, dac_cent =0.0;
double dCcent = (dMd>=0.0 ? dh-dzCen : dzCen);
_DGN_TNDN_RBAR rTdRb;
rTdRb.Initialize();
BOOL bCalcOK = Get_TndnReBarInfor(bYaxis, dCcent, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dfsd);
if(dMd>=0.0)  // 정모멘트
{
dApt_cent=rTdRb.dApttol, dapt_cent=(dApt_cent==0.0 ? 0.0 : dh - rTdRb.ddpt);
dApc_cent=rTdRb.dApctol, dapc_cent=rTdRb.ddpc;
dAst_cent=rTdRb.dAsttol, dast_cent=(dAst_cent==0.0 ? 0.0 : dh - rTdRb.ddst);
dAsc_cent=rTdRb.dAsctol, dasc_cent=rTdRb.ddsc;
dat_cent = rTdRb.daBott;
dac_cent = rTdRb.daTopc;
}
else
{
dApt_cent=rTdRb.dApctol, dapt_cent=rTdRb.ddpc;
dApc_cent=rTdRb.dApttol, dapc_cent=(dApc==0.0 ? 0.0 : dh - rTdRb.ddpt);
dAst_cent=rTdRb.dAsctol, dast_cent=rTdRb.ddsc;
dAsc_cent=rTdRb.dAsttol, dasc_cent=(dAsc==0.0 ? 0.0 : dh - rTdRb.ddst);
dat_cent = rTdRb.daTopt;
dac_cent = rTdRb.daBotc;
}

double dx=0.5*dh;  // 초기가정
double dCp = 0.0;
if(dMd>=0.0) dCp = (bYaxis==TRUE ? dCpz : dCpy);
else         dCp = (bYaxis==TRUE ? dzCen : dyCen);
double ddist = (bYaxis==TRUE ? dzDim : dyDim);
double dCtemp=0.0;
UINT iCount = 1;

double dToler=0.0;
double dh0=0.0, dxb=0.0;
double dTst=0.0, dTps=0.0, dCsc=0.0, dCps=0.0;

double dCdist = (dMd>=0.0 ? dh-dzCen : dzCen);
double db0 = db - dbw;
double dhf =(dMd>=0.0 ? dFlgThk[0] : dFlgThk[1]);
double dhfc=(dMd>=0.0 ? dFlgThk[1] : dFlgThk[0]);

//get Phi
CRCDataCtrl_CH dataCtrl;
if (dhf >1.0E-07 || dhfc >1.0E-07) {
dPhi = dataCtrl.Get_Phi(1, dl0, sqrt(min(dIyy,dIzz)/dA0));// i=(I/A)^0.5，I=min(Iyy，Izz);
}else{
dPhi = dataCtrl.Get_Phi(0, dl0, min(dbw,dh));
}


CArray<_RPT_FLEX_DATA, _RPT_FLEX_DATA&> arFlex; // Report

//1.get dx...
double dtempXi = 0.0;
do
{
dTst=0.0, dTps=0.0, dCsc=0.0, dCps=0.0;
//중립축을 기준으로 Tendon dy,dz를 수정.
dCtemp = (dMd>=0.0 ? dx : dh-dx);
rTdRb.Initialize();
bCalcOK = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dfsd);
if(dMd>=0.0)  // 정모멘트
{
dApt=rTdRb.dApttol, dapt=(dApt==0.0 ? 0.0 : dh - rTdRb.ddpt);
dApc=rTdRb.dApctol, dapc=rTdRb.ddpc;
dAst=rTdRb.dAsttol, dast=(dAst==0.0 ? 0.0 : dh - rTdRb.ddst);
dAsc=rTdRb.dAsctol, dasc=rTdRb.ddsc;
dat = rTdRb.daBott;
dac = rTdRb.daTopc;
}
else
{
dApt=rTdRb.dApctol, dapt=rTdRb.ddpc;
dApc=rTdRb.dApttol, dapc=(dApc==0.0 ? 0.0 : dh - rTdRb.ddpt);
dAst=rTdRb.dAsctol, dast=rTdRb.ddsc;
dAsc=rTdRb.dAsttol, dasc=(dAsc==0.0 ? 0.0 : dh - rTdRb.ddst);
dat = rTdRb.daTopt;
dac = rTdRb.daBotc;
}
dh0 = dh - dat;
//if(dat==0.0) dh0 = 0.8*dh;

CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> rarRbarPosi;
CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> rarTndnPosi;
bCalcOK = Get_TndnReBarTransform(bYaxis, dMd, dx, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);

// Rbar.
for(i=0; i<rarRbarPosi.GetSize(); i++)
{
_DGN_RBAR_CRC Rbar = rarRbarPosi.GetAt(i);
if(dMd>0.0)
{
if(Rbar.dz>0.0)      dCsc += dfsdc*Rbar.dArea;
else if(Rbar.dz<0.0) dTst += dfsd*Rbar.dArea;
}
else if(dMd<=0.0)
{
if(Rbar.dz>0.0)      dTst += dfsd*Rbar.dArea;
else if(Rbar.dz<0.0) dCsc += dfsdc*Rbar.dArea;
}
}

// Tendon.
dtempXi = 0.0;
for(i=0; i<rarTndnPosi.GetSize(); i++)
{
_DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
if(dMd>0.0)
{
// Ap
if(Tndn.dz>0.0)
{
double dfpdci = Tndn.dfpdc;
double dSigpoc = Tndn.dStress[0] - Tndn.dStrLoss[0][0];
dCps += (dfpdci-dSigpoc)*Tndn.dArea;
} // Ap'
else if(Tndn.dz<0.0)
{
double dfpdi = Tndn.dfpdt;
dTps += dfpdi*Tndn.dArea;
double dSigpo = Tndn.dStress[0] - Tndn.dStrLoss[0][0];
dtempXi += (dfpdi-dSigpo)/Tndn.dEps/dEpsilon;
}
}
else if(dMd<=0.0)
{
// Ap
if(Tndn.dz>0.0)
{
double dfpdi = Tndn.dfpdt;
dTps += dfpdi*Tndn.dArea;
double dSigpo = Tndn.dStress[0] - Tndn.dStrLoss[0][0];
dtempXi += (dfpdi-dSigpo)/Tndn.dEps/dEpsilon;
} // Ap'
else if(Tndn.dz<0.0)
{
double dfpdci=Tndn.dfpdc;
double dSigpoc = Tndn.dStress[0] - Tndn.dStrLoss[0][0];
dCps += (dfpdci-dSigpoc)*Tndn.dArea;
}
}
}

double dCc=0.0;
if(dx<=dhf)
dCc = dfcd*db*dx;
else if(dx>dhf)
{
if(dx<=dh)     dCc = dfcd*(dbw*dx+(db-dbw)*dhf);
else if(dx>dh) dCc = dfcd*(dbw*dh+(db-dbw)*dhf);
}

double dTens = dTst + dTps;
double dComp = dCc + dCsc + dCps;
double dCcDel = dTens - dComp;
dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);

if(fabs(dToler)<dTOL) break;
if(iCount>30) break;
double dDeltaX = (dfcd*db==0.0 ? 0.0 : fabs(dCcDel)/(dfcd*db));
if(dCcDel<0.0) dx -= dDeltaX;
else           dx += dDeltaX;

++iCount;

}while(fabs(dToler)>dTOL);

//2.get dXib...
double dxib = dbeta/(1+dtempXi);
dxb = dxib*dh0;

//3.털뙤댕鬼튤懃
if (dx <= dhfc) { db0 = db; dhfc = dhf = 0.0;	}// T  to rect.
BOOL bLarge = (dx <= dxb ? TRUE : FALSE) ;//*^^*
double dx_Cal = dx;
dCtemp = (dMd>=0.0 ? dx : dh-dx);
double dx_Posi = 0.0;
if(dx<=dac_cent)
{
dCtemp = (dMd>=0.0 ? dzCen : dh-dzCen);
dx_Posi = dCtemp;
}
else
{
dx_Posi = dx;
}

rTdRb.Initialize();
bCalcOK = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dfsd);
if(dMd>=0.0)  // 정모멘트
{
dApt=rTdRb.dApttol, dapt=(dApt==0.0 ? 0.0 : dh - rTdRb.ddpt);
dApc=rTdRb.dApctol, dapc=rTdRb.ddpc;
dAst=rTdRb.dAsttol, dast=(dAst==0.0 ? 0.0 : dh - rTdRb.ddst);
dAsc=rTdRb.dAsctol, dasc=rTdRb.ddsc;
dat = rTdRb.daBott;
dac = rTdRb.daTopc;
}
else
{
dApt=rTdRb.dApctol, dapt=rTdRb.ddpc;
dApc=rTdRb.dApttol, dapc=(dApc==0.0 ? 0.0 : dh - rTdRb.ddpt);
dAst=rTdRb.dAsctol, dast=rTdRb.ddsc;
dAsc=rTdRb.dAsttol, dasc=(dAsc==0.0 ? 0.0 : dh - rTdRb.ddst);
dat = rTdRb.daTopt;
dac = rTdRb.daBotc;
}

CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> rarRbarPosi;
CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> rarTndnPosi;
Get_TndnReBarTransform(bYaxis, dMd, dx_Posi, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);

double dNnRbarc=0.0, dNnRbart=0.0, dNnPscc=0.0, dNnPsct=0.0;//5.3.6-1
double dMnRbarc=0.0, dMnPscc=0.0;//5.3.6-2
double dMn_Rbar=0.0, dMn_Psc=0.0;//5.3.6-3  or  -4
double dNnRbarAll =0.0,dNnPscAll=0.0;
//////////////////////////////////////////////////////////////////////////
//get value..
for(i=0; i<rarRbarPosi.GetSize(); i++)
{
_DGN_RBAR_CRC Rbar = rarRbarPosi.GetAt(i);
double dAsci=0.0, dasci=0.0;
dAsci = Rbar.dArea;
dNnRbarAll = dNnRbarAll + dfsdc*dAsci;
if(dMd>=0.0)
{
if(Rbar.dz>0.0)
{
dNnRbarc += dfsdc*dAsci;//As'
dMnRbarc += dfsdc*dAsci*(dh0 - dasc);//h0-as'
}
else
{
double dhoi = dh - fabs(Rbar.dz);//h0i
if (bLarge)  dNnRbart += dfsd * dAsci;
else         dNnRbart += ( dEpsilon * dEs * (dbeta*dhoi/dx_Posi -1) ) * dAsci;
dMn_Rbar += dfsdc * dAsci * (dh-dat-dast);//h0' - das
}
}
else if(dMd<0.0)
{
if(Rbar.dz<0.0)
{
dNnRbarc += dfsdc*dAsci;//As'
dMnRbarc += dfsdc*dAsci*(dh0 - dasc);
}
else
{
double dhoi = dh - fabs(Rbar.dz);//h0i
if (bLarge)  dNnRbart += dfsd * dAsci;
else         dNnRbart += ( dEpsilon * dEs * (dbeta*dhoi/dx_Posi -1) ) * dAsci;
dMn_Rbar += dfsdc * dAsci * (dh-dat-dast);//h0' - das
}
}
}

for(i=0; i<rarTndnPosi.GetSize(); i++)
{
_DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
double dApci=0.0, dapci=0.0;
dApci = Tndn.dArea;
double dfpdci = Tndn.dfpdc;
double dfpdti = Tndn.dfpdt;
// Sig_po = Sig_con - Sig_l + Sig_l4.
double dSigpoc = Tndn.dStress[0] - Tndn.dStrLoss[0][0]; //[0][0] : x-Elastic 손실.
dNnPscAll = dNnPscAll+ (dfpdci - dSigpoc)*Tndn.dArea;
if(dMd>=0.0)
{
if(Tndn.dz>0.0)
{
dNnPscc += (dfpdci-dSigpoc)*Tndn.dArea;//// Ap'
dapci = fabs(Tndn.dz);
dMnPscc += (dfpdci-dSigpoc)*Tndn.dArea * (dh0-dapci);
}
else
{
double dhoi = dh - fabs(Tndn.dz);//h0i
if (bLarge) dNnPsct += Tndn.dfpdt * Tndn.dArea;
else 				dNnPsct += ( dEpsilon * Tndn.dEps * (dbeta*dhoi/dx_Posi -1) ) * Tndn.dArea + dSigpoc;
dMn_Psc += (dfpdci - dSigpoc) * Tndn.dArea * (dh-dat-dapt);
}
}
else if(dMd<0.0)
{
if(Tndn.dz<0.0)
{
dNnPscc += (dfpdci-dSigpoc)*Tndn.dArea;//// Ap'
dapci = fabs(Tndn.dz);
dMnPscc += (dfpdci-dSigpoc)*Tndn.dArea * (dh0-dapci);
}
else
{
double dhoi = dh - fabs(Tndn.dz);//h0i
if (bLarge) dNnPsct += Tndn.dfpdt * Tndn.dArea;
else 				dNnPsct += ( dEpsilon * Tndn.dEps * (dbeta*dhoi/dx_Posi -1) ) * Tndn.dArea + dSigpoc;
dMn_Psc += (dfpdci - dSigpoc) * Tndn.dArea * (dh-dat-dapt);
}
}
}
double dConNn = dfcd * (db0*dx - (db - db0)*dhfc) ;//5.3.6-1
double dConMn = dfcd * (db0*dx*(dh0 - dx/2) + (db - db0)*dhfc*(dh0 - dhfc/2))  ;//5.3.6-2
double dConMnc1 = dfcd * (db0*dh*(dh/2 - dat) + (db - db0)*dhfc*(dhfc/2 -dat));//5.3.6-3  or -4
double dConMnc2 = dfcd * (db0*dh*(dh/2 - dat) + (dbt - db0)*dhf*(dh -dat - dhf/2));//5.3.6-3  or -4

CscsJtg.dx = dx;
CscsJtg.dMd = dMd;
CscsJtg.dNd = dNd;
if (dMd<1.0E-07 && dMd >-1.0E-07) {//菉懃
CscsJtg.drNd = dr0 * dNd;
CscsJtg.dNn = 0.9*dPhi*(dfcd*dA0 + dNnRbarAll + dNnPscAll);
return TRUE;
}
CscsJtg.dNn = dConNn + dNnRbarc + dNnPscc - dNnRbart - dNnPsct;
CscsJtg.dNne = dConMn + dMnRbarc + dMnPscc;
CscsJtg.dNne1 = max(dConMnc1,dConMnc2) + dMn_Rbar + dMn_Psc;
CscsJtg.drNd = dr0 * dNd;

double de0 = dMd / dNd; double dEta = 1.0;
double dZeta1= 0.2+2.7*de0/dh0; // get ┎.see [JTG D62--2004] 5.3.10 P39
double dZeta2= 1.15-0.01*dl0/dh;
if (dZeta1 > 1.0 ) dZeta1 = 1.0; if (dZeta2 > 1.0 ) dZeta2 = 1.0;
if (dl0/dh <=5  ) {//몽앴낀玖궐털뙤角뤠옘쪄┎.Lo/i>17.5 <=> Lo/h>5
dEta = 1.0;
}else{
dEta = 1+1.0/(1400.0*de0/dh0)*(dl0/dh)*(dl0/dh)*dZeta1*dZeta2;
}
double de = dEta*de0+dh/2-dat;//e=┎e0+h/2-as;
double de1 = dh/2-dEta*de0-dac;//e'=h/2-┎e0-as'
CscsJtg.drNde = CscsJtg.drNd * de;
CscsJtg.drNde1 = CscsJtg.drNd * de1;
return TRUE;
}
*/

BOOL CDgnJTG_D62_04::Get_CrackCheckParam(double dMs, double dMl, double& dC1, double& dC2, double& dC3)
{
	BOOL bRib = TRUE;
	dC1 = (!bRib ? 1.4 : 1.0);                  //철근표면형상계수 
	dC2 = (dMs == 0.0 ? 1.0 : 1.0 + 0.5*dMl / dMs);   //작용(또는 하중)장기효응영향계수
	if (dC2 < 0.5) dC2 = 0.5;
	if (dC2 > 1.5) dC2 = 1.5;
	dC3 = 1.0;                                  //부재의 힘특성과 관련된 계수.

	return TRUE;
}

BOOL CDgnJTG_D62_04::Get_Alpha123(double dMu, double dMu_Tnpr, double& dalpha1, double& dalpha2, double& dalpha3)
{
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	int iBrgType = PscdD.iBrgType;

	dalpha1 = 1.0;

	if (iBrgType == 0 || iBrgType == 1)
	{
		if ((dMu > 0.0 && dMu_Tnpr > 0.0) || (dMu < 0.0 && dMu_Tnpr < 0.0))
		{
			dalpha2 = 1.0;
		}
		else
		{
			dalpha2 = 1.25;
		}
	}
	else if (iBrgType == 2)
	{
		dalpha2 = 1.0;
	}

	dalpha3 = 1.1;
	return TRUE;
}

/*
BOOL CDgnJTG_D62_04::Get_TndnReBarTransform(BOOL bYaxis, double dMu, double dNueC, double dCp, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi,
CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& rarRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& rarTndnPosi)
{
int i=0;
// Reinforcement.
for(i=0 ; i<arRbarPosi.GetSize(); i++)
{
_DGN_RBAR_CRC rRbar = arRbarPosi.GetAt(i);
_DGN_RBAR_CRC rTnfrC; rTnfrC.Initialize();
rTnfrC = rRbar;
double dgap =  (dMu>=0.0 ? dNueC-dCp : dCp-dNueC);
if(bYaxis) rTnfrC.dz = rRbar.dz+dgap;
else       rTnfrC.dy = rRbar.dy+dgap;
rarRbarPosi.Add(rTnfrC);
}

// Tendon.
for(i=0 ; i<arTndnPosi.GetSize(); i++)
{
_DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
_DGN_TNDN_CRC rTnfrC; rTnfrC.Initialize();
rTnfrC = rTndn;
double dgap =  (dMu>=0.0 ? dNueC-dCp : dCp-dNueC);
if(bYaxis) rTnfrC.dz = rTndn.dz+dgap;
else       rTnfrC.dy = rTndn.dy+dgap;
rarTndnPosi.Add(rTnfrC);
}

return TRUE;
}
*/

BOOL CDgnJTG_D62_04::Get_SumForceEccent(BOOL bYaxis, double dMd, double dNueC, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&>& arTndnPosi, double& dNp0, double& dep0, double& dhps)
{
	// Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy = dyDim - dyCen;
	double dCpz = dzDim - dzCen;
	double dA0 = SpscSect.dArea;  // 전체환산단면적.
	double dIyy = SpscSect.dIyy;
	double dIzz = SpscSect.dIzz;
	double dh = dzDim;

	double dApt = 0.0, dNtodpt = 0.0, dapt = 0.0;
	double dApc = 0.0, dNtodpc = 0.0, dapc = 0.0;
	double dAst = 0.0, dNtodst = 0.0, dast = 0.0;
	double dAsc = 0.0, dNtodsc = 0.0, dasc = 0.0;
	double dat = 0.0, dac = 0.0;
	double dyst = 0.0, dysc = 0.0;
	double dhs = 0.0, das = 0.0;
	double dSigpot = 0.0, dSigpoc = 0.0, dSigl6t = 0.0, dSigl6c = 0.0;

	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();
	double dfsd = 0.0; // Temp
	BOOL bGetOK = Get_TndnReBarInfor(bYaxis, dNueC, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dfsd);
	if (dMd >= 0.0)  // 정모멘트 
	{
		dApt = rTdRb.dApttol, dapt = (dApt == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dApc = rTdRb.dApctol, dapc = rTdRb.ddpc;
		dAst = rTdRb.dAsttol, dast = (dAst == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dAsc = rTdRb.dAsctol, dasc = rTdRb.ddsc;
		dyst = rTdRb.dNtodst;
		dysc = rTdRb.dNtodsc;
		dhs = rTdRb.ddst;
		das = rTdRb.ddsc;
	}
	else
	{
		dApt = rTdRb.dApctol, dapt = rTdRb.ddpc;
		dApc = rTdRb.dApttol, dapc = (dApc == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dAst = rTdRb.dAsctol, dast = rTdRb.ddsc;
		dAsc = rTdRb.dAsttol, dasc = (dAsc == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dyst = rTdRb.dNtodsc;
		dysc = rTdRb.dNtodst;
		dhs = dh - rTdRb.ddsc;
		das = dh - rTdRb.ddst;
	}

	int iNumTnC = 0, iNumTnT = 0;
	double dSigpotAp_sum = 0.0, dSigpocAp_sum = 0.0, dSigl6t_sum = 0.0, dSigl6c_sum = 0.0;
	double dSigpotAp_yp_sum = 0.0, dSigpocAp_yp_sum = 0.0;
	double dSigpocAp_ap_sum = 0.0, dSigpotAp_hp_sum = 0.0;
	for (int i = 0; i < arTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(i);
		double dApci = 0.0, dapci = 0.0;
		dApci = Tndn.dArea;
		double dfpdci = Tndn.dfpdc;
		double dfpdti = Tndn.dfpdt;
		double dSigpo = Tndn.dStress[0] - Tndn.dStrLoss[0][0]; //[0][0] : x-Elastic 손실 
		double dSl6 = -1.0*Tndn.dStrLoss[0][1]; //(-)값이 손실이기 때문에 공식과 표현을 같게 하기 위해 -1을 곱함.  
		double dSigpocAp = 0.0, dSigpotAp = 0.0;
		double dhpi = 0.0, dapi = 0.0;
		if (dMd >= 0.0)
		{
			// Ap
			if (Tndn.dz > 0.0)
			{
				dSigpocAp = dSigpo * dApci;
				dSigpocAp_sum += dSigpocAp;
				dSigl6c_sum += dSl6;                     //[0][2] : 2=Creep/Shrinkage 손실.
				dSigpocAp_yp_sum += dSigpocAp * Tndn.dz;
				iNumTnC++;

				dapi = dNueC - Tndn.dz;
				dSigpocAp_ap_sum += dSigpocAp * dapi;
			}
			else
			{
				dSigpotAp = dSigpo * dApci;
				dSigpotAp_sum += dSigpotAp;
				dSigl6t_sum += dSl6;
				dSigpotAp_yp_sum += dSigpotAp * fabs(Tndn.dz);
				iNumTnT++;

				dhpi = dNueC + fabs(Tndn.dz);
				dSigpotAp_hp_sum += dSigpotAp * dhpi;
			}
		}
		else if (dMd < 0.0)
		{
			if (Tndn.dz < 0.0)
			{
				dSigpocAp = dSigpo * dApci;
				dSigpocAp_sum += dSigpocAp;
				dSigl6c_sum += dSl6;
				dSigpocAp_yp_sum += dSigpocAp * fabs(Tndn.dz);
				iNumTnC++;

				dapi = dNueC - fabs(Tndn.dz);
				dSigpocAp_ap_sum += dSigpocAp * dapi;
			}
			else
			{
				dSigpotAp = dSigpo * dApci;
				dSigpotAp_sum += dSigpotAp;
				dSigl6t_sum += dSl6;
				dSigpotAp_yp_sum += dSigpotAp * Tndn.dz;
				iNumTnT++;

				dhpi = dNueC + Tndn.dz;
				dSigpotAp_hp_sum += dSigpotAp * dhpi;
			}
		}
	}
	double dSigl6c_ave = 0.0, dSigl6t_ave = 0.0;
	dSigl6c_ave = (iNumTnC == 0.0 ? 0.0 : dSigl6c_sum / iNumTnC);
	dSigl6t_ave = (iNumTnT == 0.0 ? 0.0 : dSigl6t_sum / iNumTnT);
	dNp0 = dSigpotAp_sum + dSigpocAp_sum - dSigl6t_ave * dAst - dSigl6c_ave * dAsc;
	dep0 = (dNp0 == 0.0 ? 0.0 : (dSigpotAp_yp_sum - dSigpocAp_yp_sum - dSigl6t_ave * dAst*dyst + dSigl6c_ave * dAsc*dysc) / dNp0);
	dhps = (dNp0 == 0.0 ? 0.0 : (dSigpotAp_hp_sum - dSigpocAp_ap_sum - dSigl6t_ave * dAst*dhs + dSigl6c_ave * dAsc*das) / dNp0);

	return bGetOK;
}

BOOL CDgnJTG_D62_04::Get_Np0_eN_hps(BOOL bYaxis, double dMd, double dNueC, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&>& arTndnPosi, double& dNp0, double& deN, double& dhps)
{
	// Ref. JTG D62-04
	// Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy = dyDim - dyCen;
	double dCpz = dzDim - dzCen;
	double dA0 = SpscSect.dArea;  // 전체환산단면적.
	double dIyy = SpscSect.dIyy;
	double dIzz = SpscSect.dIzz;
	double dh = dzDim;

	double dApt = 0.0, dNtodpt = 0.0, dapt = 0.0;
	double dApc = 0.0, dNtodpc = 0.0, dapc = 0.0;
	double dAst = 0.0, dNtodst = 0.0, dast = 0.0;
	double dAsc = 0.0, dNtodsc = 0.0, dasc = 0.0;
	double dat = 0.0, dac = 0.0;
	double dyst = 0.0, dysc = 0.0;
	double dhs = 0.0, das = 0.0;
	double dSigpot = 0.0, dSigpoc = 0.0, dSigl6t = 0.0, dSigl6c = 0.0;

	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();
	double dfsd = 0.0; // Temp
	BOOL bGetOK = Get_TndnReBarInfor(bYaxis, dNueC, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dfsd);
	if (dMd >= 0.0)  // 정모멘트 
	{
		dApt = rTdRb.dApttol, dapt = (dApt == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dApc = rTdRb.dApctol, dapc = rTdRb.ddpc;
		dAst = rTdRb.dAsttol, dast = (dAst == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dAsc = rTdRb.dAsctol, dasc = rTdRb.ddsc;
		dyst = rTdRb.dNtodst;
		dysc = rTdRb.dNtodsc;
		dhs = dh - dast; //rTdRb.ddst    
	}
	else
	{
		dApt = rTdRb.dApctol, dapt = rTdRb.ddpc;
		dApc = rTdRb.dApttol, dapc = (dApc == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dAst = rTdRb.dAsctol, dast = rTdRb.ddsc;
		dAsc = rTdRb.dAsttol, dasc = (dAsc == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dyst = rTdRb.dNtodsc;
		dysc = rTdRb.dNtodst;
		dhs = dh - dast; //rTdRb.ddst    
	}

	int iNumTnC = 0, iNumTnT = 0;
	double dSigpotAp_sum = 0.0, dSigpocAp_sum = 0.0, dSigl6t_sum = 0.0, dSigl6c_sum = 0.0;
	double dSigpotAp_yp_sum = 0.0, dSigpocAp_yp_sum = 0.0;
	double dSigpocAp_ap_sum = 0.0, dSigpotAp_hp_sum = 0.0;
	//
	for (int i = 0; i < arTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(i);
		double dApci = 0.0, dapci = 0.0;
		dApci = Tndn.dArea;
		double dfpdci = Tndn.dfpdc;
		double dfpdti = Tndn.dfpdt;
		double dSigpo = Tndn.dStress[0] - Tndn.dStrLoss[0][0]; //[0][0] : x-Elastic 손실 
		double dSl6 = Tndn.dStrLoss[0][1]; // 6.1.6-1 , -1을 곱하지 않는것으로 수정.
		double dSigpocAp = 0.0, dSigpotAp = 0.0;
		double dhpi = 0.0, dapi = 0.0;
		if (dMd >= 0.0)
		{
			// Ap
			if (Tndn.dz > 0.0)
			{
				dSigpocAp = dSigpo * dApci;
				dSigpocAp_sum += dSigpocAp;
				dSigl6c_sum += dSl6;                     //[0][1] : 2=Creep/Shrinkage 손실.
				dSigpocAp_yp_sum += dSigpocAp * Tndn.dz;
				iNumTnC++;

				dapi = dNueC - Tndn.dz;
				dSigpocAp_ap_sum += dSigpocAp * dapi;
			}
			else
			{
				dSigpotAp = dSigpo * dApci;
				dSigpotAp_sum += dSigpotAp;
				dSigl6t_sum += dSl6;
				dSigpotAp_yp_sum += dSigpotAp * fabs(Tndn.dz);
				iNumTnT++;

				dhpi = dNueC + fabs(Tndn.dz);
				dSigpotAp_hp_sum += dSigpotAp * dhpi;
			}
		}
		else if (dMd < 0.0)
		{
			if (Tndn.dz < 0.0)
			{
				dSigpocAp = dSigpo * dApci;
				dSigpocAp_sum += dSigpocAp;
				dSigl6c_sum += dSl6;
				dSigpocAp_yp_sum += dSigpocAp * fabs(Tndn.dz);
				iNumTnC++;

				dapi = dNueC - fabs(Tndn.dz);
				dSigpocAp_ap_sum += dSigpocAp * dapi;
			}
			else
			{
				dSigpotAp = dSigpo * dApci;
				dSigpotAp_sum += dSigpotAp;
				dSigl6t_sum += dSl6;
				dSigpotAp_yp_sum += dSigpotAp * Tndn.dz;
				iNumTnT++;

				dhpi = dNueC + Tndn.dz;
				dSigpotAp_hp_sum += dSigpotAp * dhpi;
			}
		}
	}
	double dSigl6c_ave = 0.0, dSigl6t_ave = 0.0;
	dSigl6c_ave = (iNumTnC == 0.0 ? 0.0 : dSigl6c_sum / iNumTnC);
	dSigl6t_ave = (iNumTnT == 0.0 ? 0.0 : dSigl6t_sum / iNumTnT);

	//Change by Jaeoh. ('06.09.25). dSigl6t_ave이 (손실)(-) 이므로 더하는 것으로 수정
	//dNp0 = dSigpotAp_sum+dSigpocAp_sum-dSigl6t_ave*dAst-dSigl6c_ave*dAsc;  
	dNp0 = dSigpotAp_sum + dSigpocAp_sum + dSigl6t_ave * dAst + dSigl6c_ave * dAsc;
	dhps = (dNp0 == 0.0 ? 0.0 : (dSigpotAp_hp_sum + dSigpocAp_ap_sum + dSigl6t_ave * dAst*dhs + dSigl6c_ave * dAsc*dasc) / dNp0);
	deN = (dNp0 == 0.0 ? 0.0 : fabs(dMd) / dNp0 - dhps);

	return TRUE;
}

BOOL CDgnJTG_D62_04::Get_SafetyCoeff(double& drc, double& drs)
{
	drc = 1.25; // 콘트리트의 안전계수.
	drs = 1.25; // 철근의 안전계수.
	return TRUE;
}

BOOL CDgnJTG_D62_04::Get_Alphas(double& dAlpha_s)
{
	dAlpha_s = 0.8; // 처음 프리스트레스철근면적을 예측할 때 근사하게 alpha_s=0.8로 할 수 있다. 1.텐던면적의 예측계산
	return TRUE;
}

BOOL CDgnJTG_D62_04::Get_Alpha1(double& dAlpha1)
{
	dAlpha1 = 0.75; // 설계경험계수, Alpha1=0.75~0.77. //!/
	return TRUE;
}

double CDgnJTG_D62_04::Get_MatlFactor()
{
	// Add by Jaeoh('05.03.09) Req. 계BJ님.
	double dFactor = 0.0;
	return dFactor;
}
// Calculation Tension Rebar.
BOOL CDgnJTG_D62_04::Get_TensionRebar(ElemPairK ElemK, UINT iIJ, UINT iTopBot, _PSC_MATL_ELEM& MatlElem,
	double dgam0, double dfcd, double dftd, double dxib, double dbw, double dFlgThk[2], double dpMomPos[2][2],
	T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&>& arTndnPosi,
	T_TBAR_JTG_BASE& TbarJtg)
{
	// Material
	double dRa = dfcd;
	double dRg = MatlElem.dfsdtM;
	// Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy = dyDim - dyCen;
	double dCpz = dzDim - dzCen;
	double dA0 = SpscSect.dArea;  // 전체환산단면적.
	double dIyy = SpscSect.dIyy;
	double dIzz = SpscSect.dIzz;
	double dh = dzDim;
	// 철근안전계수.
	double drc, drs;
	Get_SafetyCoeff(drc, drs);

	double dApt = 0.0, dNtodpt = 0.0, dapt = 0.0;
	double dApc = 0.0, dNtodpc = 0.0, dapc = 0.0;
	double dAst = 0.0, dNtodst = 0.0, dast = 0.0;
	double dAsc = 0.0, dNtodsc = 0.0, dasc = 0.0;
	double dat = 0.0, dac = 0.0;

	double dMj = TbarJtg.dMj;

	double dCtemp = (dMj >= 0.0 ? dCpz : dzCen);
	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();
	BOOL bCalcOK = Get_TndnReBarInfor(TRUE, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dRg);
	if (dMj >= 0.0)  // 정모멘트 
	{
		dApt = rTdRb.dApttol, dapt = (dApt == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dApc = rTdRb.dApctol, dapc = rTdRb.ddpc;
		dAst = rTdRb.dAsttol, dast = (dAst == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dAsc = rTdRb.dAsctol, dasc = rTdRb.ddsc;
		dat = rTdRb.daBott;
		dac = rTdRb.daTopc;
	}
	else
	{
		dApt = rTdRb.dApctol, dapt = rTdRb.ddpc;
		dApc = rTdRb.dApttol, dapc = (dApc == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dAst = rTdRb.dAsctol, dast = rTdRb.ddsc;
		dAsc = rTdRb.dAsttol, dasc = (dAsc == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dat = rTdRb.daTopt;
		dac = rTdRb.daBotc;
	}

	double dh0 = 0.0;
	double da_yg = dat;   // a_yg : A점에 인장영역 사이드까지의 거리  
	if (da_yg > 0.0)
	{
		dh0 = dh - da_yg;
	}
	else if (da_yg <= 0.0)
	{
		dh0 = 0.8*dh; // 텐던,철근이 모두 없을 경우
	}

	// 압축영역 높이 x
	double dbf_top = dyDim;
	double dbf_bot = fabs(dpMomPos[0][0] - dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.
	double db = (dMj >= 0.0 ? dbf_top : dbf_bot); // b: 단면의 너비.

	BOOL bYaxis = TRUE;
	double dx = 0.95*dh;
	double dCp = 0.0;
	if (dMj >= 0.0) dCp = dCpz;
	else         dCp = dzCen;
	double ddist = dzDim;
	UINT iCount = 1;

	double dTOL = 1.0E-3;
	double dToler = 0.0;

	double dCdist = (dMj >= 0.0 ? dh - dzCen : dzCen);
	double dhf = (dMj >= 0.0 ? dFlgThk[0] : dFlgThk[1]);
	//double dhfc=(dMd>=0.0 ? dFlgThk[1] : dFlgThk[0]);

	BOOL bCalcRebar = TRUE;
	if (iTopBot == 0)
	{
		if (dMj > 0.0) bCalcRebar = FALSE;
	}
	else if (iTopBot == 1)
	{
		if (dMj < 0.0) bCalcRebar = FALSE;
	}

	double dxb = 0.0, dx_Cal = 0.0;
	double dRyAy = 0.0; // 계산된 dx를 기준으로 계산한다.
	if (bCalcRebar)
	{
		do
		{
			//중립축을 기준으로 Tendon dy,dz를 수정.
			dCtemp = (dMj >= 0.0 ? dx : dh - dx);

			double dMoment = 0.0;
			if (dx <= dhf)
				dMoment = (1.0 / dgam0)*dRa*db*dx*(dh0 - dx / 2.);
			else if (dx > dhf)
				dMoment = (1.0 / dgam0)*dRa*(dbw*dx*(dh0 - dx / 2.) + (db - dbw)*dhf*(dh0 - dhf / 2.));

			double dMdelt = dMj - dMoment;
			dToler = (dMj == 0.0 ? 0.0 : dMdelt / dMj);

			if (fabs(dToler) < dTOL) break;
			if (iCount > 30) break;
			if (dMdelt < 0.0) dx -= ddist * pow(0.5, iCount);
			else           dx += ddist * pow(0.5, iCount);
			++iCount;

		} while (fabs(dToler) > dTOL);

		dx_Cal = dx;
		dxb = dxib * dh0;
		if (dx > dxb)   dx = dxb;
		else if (dx < dac) dx = dac;

		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> rarRbarPosi;
		CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&> rarTndnPosi;
		bCalcOK = Get_TndnReBarTransform(bYaxis, dMj, dx, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);
		// Tendon.
		for (int i = 0; i < rarTndnPosi.GetSize(); i++)
		{
			_DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
			if (iTopBot == 1)  // 0=Top, 1=Bot
			{
				if (Tndn.dz < 0.0)
				{
					double dfpdti = Tndn.dfpdt;
					dRyAy += dfpdti * Tndn.dArea;
				}
			}
			else if (iTopBot == 0)
			{
				// Ap
				if (Tndn.dz > 0.0)
				{
					double dfpdti = Tndn.dfpdt;
					dRyAy += dfpdti * Tndn.dArea;
				}
			}
		}
	}// bCalcRebar

	double dAreq = 0.0;
	double dAuse = 0.0;
	double dAmin = 0.0;

	if (dx <= dhf)
		dAreq = (dRg == 0.0 ? 0.0 : (dRa*db*dx - dRyAy) / dRg);
	else if (dx > dhf)
		dAreq = (dRg == 0.0 ? 0.0 : (dRa*(dbw*dx + (db - dbw)*dhf) - dRyAy) / dRg);

	if (dAreq < 0.0) dAreq = 0.0; // if A_req < 0 , dAreq = 0.0 

	dAuse = (iTopBot == 0 ? rTdRb.dAsctol : rTdRb.dAsttol);
	dAmin = 0.0;

	// if(!bCalcRebar) -> Table에 Ag_REQ= _T("-")로 ?
	TbarJtg.dREQ = (bCalcRebar ? dAreq : 0.0);
	TbarJtg.dUSE = dAuse;
	TbarJtg.dMIN = dAmin;


	//++++++++++++++++++++++++++
	// For zzzDataForDgn_JTG_D62_04.txt
	if (m_bTestMode)
	{
		const auto it = m_aSelElemK.find(ElemK);
		if (it != m_aSelElemK.end())
		{
			rptwofstream fout(m_strTestDgnFile, ios::app);

			if (bCalcRebar)
			{
				CString strXcal;   strXcal.Format(_T("%16g"), dx_Cal);
				CString strX;      strX.Format(_T("%16g"), dx);
				CString strxib;    strxib.Format(_T("%16g"), dxb);
				CString strb;      strb.Format(_T("%16g"), db);
				CString strbw;     strbw.Format(_T("%16g"), dbw);
				CString strh;      strh.Format(_T("%16g"), dh);
				CString strh0;     strh0.Format(_T("%16g"), dh0);
				CString strhf;     strhf.Format(_T("%16g"), dhf);
				CString strRa;     strRa.Format(_T("%16g"), dRa);
				CString strRg;     strRg.Format(_T("%16g"), dRg);
				fout << _T(" x_cal : ") << strXcal << endl
					<< _T(" x     : ") << strX << endl
					<< _T(" xi_b  : ") << strxib << endl
					<< _T(" b     : ") << strb << endl
					<< _T(" bw    : ") << strbw << endl
					<< _T(" h     : ") << strh << endl
					<< _T(" h0    : ") << strh0 << endl
					<< _T(" hf    : ") << strhf << endl
					<< _T(" Ra    : ") << strRa << endl
					<< _T(" Rg    : ") << strRg << endl
					<< endl;

				if (dx <= dhf)
				{
					CString strdxSMhf = _T(" x < hf");
					CString strAreq = _T(" As,req = (Ra*b*x-RyAy)/Rg");
					dAreq = (dRg == 0.0 ? 0.0 : (dRa*db*dx - dRyAy) / dRg);
					CString strCalcAreq;
					strCalcAreq.Format(_T("        =(%8g*%8g*%8g-%8g)/%8g=%8g"),
						dRa, db, dx, dRyAy, dRg, dAreq);
					fout << strdxSMhf << endl
						<< strAreq << endl
						<< strCalcAreq << endl
						<< endl;
				}
				else if (dx > dhf)
				{
					CString strdxSMhf = _T(" x > hf");
					CString strAreq = _T(" As,req = (Ra*(bw*dx+(b-bw)*hf)-RyAy)/Rg");
					dAreq = (dRg == 0.0 ? 0.0 : (dRa*(dbw*dx + (db - dbw)*dhf) - dRyAy) / dRg);
					CString strCalcAreq;
					strCalcAreq.Format(_T("        =(%8g*(%8g*%8g+(%8g-%8g)*%8g)-%8g)/%8g=%8g"),
						dRa, dbw, dx, db, dbw, dhf, dRyAy, dRg, dAreq);
					fout << strdxSMhf << endl
						<< strAreq << endl
						<< strCalcAreq << endl
						<< endl;
				}
			}
		}
	}

	return bCalcOK;
}

BOOL CDgnJTG_D62_04::Get_TendonStress(ElemPairK ElemK, BOOL bPosiMom, double dMuy, double dfcd, double dftd, double dbw, double dFlgThk[2], double dpMomPos[2][2], _PSC_MATL_ELEM& MatlElem, CArray<T_SPSC_COOR, T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*, T_SPSC_COOR_AR*>& arInner,
	T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&>& arTndnPosi, CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&>& arCalcTndnPosi)
{
	// Mu가 0이면 계산할 필요가 없다. 
	if (dMuy == 0.0) return FALSE;

	if (arRbarPosi.GetSize() == 0.0 && arTndnPosi.GetSize() == 0.0) return FALSE;
	T_MATD_D MatdD = MatlElem.MatdD;

	int i;
	// Material
	double dEc = MatdD.Data1.Analysis.Elast;
	double dfy = MatdD.MainRebarData.B_fy;
	double dEs = MatdD.MainRebarData.B_Elast;
	double dEp = 0.0;
	double dfsd = MatlElem.dfsdtM;
	double dfsdc = MatlElem.dfsdcM;
	double dalpha_ES = dEc == 0.0 ? 0.0 : dEs / dEc;

	// Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy = dyDim - dyCen;
	double dCpz = dzDim - dzCen;
	double dA0 = SpscSect.dArea;  // 전체환산단면적.
	double dIyy = SpscSect.dIyy;
	double dIzz = SpscSect.dIzz;

	//Axis
	BOOL bYaxis = TRUE;
	//Force
	double dMd = dMuy;

	double dgap = 0.0;
	double dAs = 0.0, dAsY = 0.0;
	double dApt = 0.0, dNtodpt = 0.0, dapt = 0.0;
	double dApc = 0.0, dNtodpc = 0.0, dapc = 0.0;
	double dAst = 0.0, dNtodst = 0.0, dast = 0.0;
	double dAsc = 0.0, dNtodsc = 0.0, dasc = 0.0;
	double dat = 0.0, dac = 0.0;
	//Tendon
	CArray<double, double> arApti, arApci;
	CArray<double, double> arapti, arapci;
	//Rbar
	CArray<double, double> arAsti, arAsci;
	CArray<double, double> arasti, arasci;

	double dNueC = 0.0;
	double dfst = 0.0, dfsc = 0.0;

	double dMn = 0.0, dr0Mn = 0.0;
	double dTOL = 0.001;

	double dh = (bYaxis == TRUE ? dzDim : dyDim);
	double dbf_top = dyDim;
	double dbf_bot = fabs(dpMomPos[0][0] - dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.
	double db = (dMd >= 0.0 ? dbf_top : dbf_bot);
	double dLength = (bYaxis == TRUE ? dyDim : dzDim);

	// 압축영영의 높이 x<a's 일 때.
	double dApt_cent = 0.0, dapt_cent = 0.0;
	double dApc_cent = 0.0, dapc_cent = 0.0;
	double dAst_cent = 0.0, dast_cent = 0.0;
	double dAsc_cent = 0.0, dasc_cent = 0.0;
	double dat_cent = 0.0, dac_cent = 0.0;
	double dCcent = (dMd >= 0.0 ? dh - dzCen : dzCen);
	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();
	BOOL bCalcOK = Get_TndnReBarInfor(bYaxis, dCcent, SpscSect, arRbarPosi, arTndnPosi, rTdRb, dfsd);
	if (dMd >= 0.0)  // 정모멘트 
	{
		dApt_cent = rTdRb.dApttol, dapt_cent = (dApt_cent == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dApc_cent = rTdRb.dApctol, dapc_cent = rTdRb.ddpc;
		dAst_cent = rTdRb.dAsttol, dast_cent = (dAst_cent == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dAsc_cent = rTdRb.dAsctol, dasc_cent = rTdRb.ddsc;
		dat_cent = rTdRb.daBott;
		dac_cent = rTdRb.daTopc;
	}
	else
	{
		dApt_cent = rTdRb.dApctol, dapt_cent = rTdRb.ddpc;
		dApc_cent = rTdRb.dApttol, dapc_cent = (dApc == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dAst_cent = rTdRb.dAsctol, dast_cent = rTdRb.ddsc;
		dAsc_cent = rTdRb.dAsttol, dasc_cent = (dAsc == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dat_cent = rTdRb.daTopt;
		dac_cent = rTdRb.daBotc;
	}

	double dCp = 0.0;
	if (dMd >= 0.0) dCp = (bYaxis == TRUE ? dCpz : dCpy);
	else         dCp = (bYaxis == TRUE ? dzCen : dyCen);
	double ddist = (bYaxis == TRUE ? dzDim : dyDim);
	double dCtemp = 0.0;
	UINT iCount = 1;

	double dToler = 0.0;
	double dh0 = 0.0, dxb = 0.0;
	double dTst = 0.0, dTps = 0.0, dCsc = 0.0, dCps = 0.0;

	double dCdist = (dMd >= 0.0 ? dh - dzCen : dzCen);
	double db0 = db - dbw;
	double dhf = (dMd >= 0.0 ? dFlgThk[0] : dFlgThk[1]);

	// 직사각형 단면이면.
	if (fabs(dh - dhf) < m_dZero) dhf = 0.0;

	//부록G 균열을 허용하는 B종류의 프리스트레스 콘크리트 휨부재의 압축영역 높이 계산.
	// e_N, N_p0, h_ps 계산.
	double dCompH = 0.0;
	double deN = 0.0;
	double dNp0 = 0.0;
	double dhps = 0.0;
	Get_Np0_eN_hps(TRUE, dMd, dCp, SpscSect, arRbarPosi, arTndnPosi, dNp0, deN, dhps);

	double dAA = dbw;
	double dBB = 3.0*dbw*deN;
	double dCC = 0.0;
	double dDD = 0.0;
	double dCC_2nd = 0.0, dCC_3rd = 0.0;
	double dDD_2nd = 0.0, dDD_3rd = 0.0;
	// Tendon.
	double dApti_gpti = 0.0;
	double dApci_gpci = 0.0;
	double dApti_hpti_gpti = 0.0;
	double dApci_hpci_gpci = 0.0;
	double dAsti_hsti_gpti = 0.0;
	double dAsci_hsci_gpci = 0.0;
	for (i = 0; i < arTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(i);
		double dApi = Tndn.dArea;
		double dalpha_EPi = dEc == 0.0 ? 0.0 : Tndn.dEps / dEc;
		double dhpi = 0.0, dgpi = 0.0;
		if (dMd > 0.0)
		{
			// Ap'
			if (Tndn.dz > 0.0)
			{
				dhpi = dCp - Tndn.dz;
				dgpi = deN + dhpi;
				dApci_gpci += dalpha_EPi * dApi*dgpi;
				dApci_hpci_gpci += dalpha_EPi * dApi*dhpi*dgpi;
			} // Ap        
			else if (Tndn.dz < 0.0)
			{
				dhpi = dCp + fabs(Tndn.dz);
				dgpi = deN + dhpi;
				dApti_gpti += dalpha_EPi * dApi*dgpi;
				dApti_hpti_gpti += dalpha_EPi * dApi*dhpi*dgpi;
			}
		}
		else if (dMd <= 0.0)
		{
			// Ap
			if (Tndn.dz > 0.0)
			{
				dhpi = dCp + Tndn.dz;
				dgpi = deN + dhpi;
				dApti_gpti += dalpha_EPi * dApi*dgpi;
				dApti_hpti_gpti += dalpha_EPi * dApi*dhpi*dgpi;
			} // Ap'        
			else if (Tndn.dz < 0.0)
			{
				dhpi = dCp + Tndn.dz;
				dgpi = deN + dhpi;
				dApci_gpci += dalpha_EPi * dApi*dgpi;
				dApci_hpci_gpci += dalpha_EPi * dApi*dhpi*dgpi;
			}
		}
	}
	// Rebar.
	double dAsti_gsti = 0.0;
	double dAsci_gsci = 0.0;
	double dAsti_hsti_gsti = 0.0;
	double dAsci_hsci_gsci = 0.0;
	for (i = 0; i < arRbarPosi.GetSize(); i++)
	{
		_DGN_RBAR_CRC Rbar = arRbarPosi.GetAt(i);
		double dAsi = Rbar.dArea;
		double dhsi = 0.0, dgsi = 0.0;
		if (dMd > 0.0)
		{
			// As'
			if (Rbar.dz > 0.0)
			{
				dhsi = dCp - Rbar.dz;
				dgsi = deN + dhsi;
				dAsci_gsci += dalpha_ES * dAsi*dgsi;
				dAsci_hsci_gpci += dalpha_ES * dAsi*dhsi*dgsi;
			}// As
			else if (Rbar.dz < 0.0)
			{
				dhsi = dCp + fabs(Rbar.dz);
				dgsi = deN + dhsi;
				dAsti_gsti += dalpha_ES * dAsi*dgsi;
				dAsti_hsti_gpti += dalpha_ES * dAsi*dhsi*dgsi;
			}
		}
		else if (dMd <= 0.0)
		{
			// As
			if (Rbar.dz > 0.0)
			{
				dhsi = dCp + Rbar.dz;
				dgsi = deN + dhsi;
				dAsti_gsti += dalpha_ES * dAsi*dgsi;
				dAsti_hsti_gpti += dalpha_ES * dAsi*dhsi*dgsi;
			}// As'
			else if (Rbar.dz < 0.0)
			{
				dhsi = dCp + Rbar.dz;
				dgsi = deN + dhsi;
				dAsci_gsci += dalpha_ES * dAsi*dgsi;
				dAsci_hsci_gpci += dalpha_ES * dAsi*dhsi*dgsi;
			}
		}
	}

	dCC_2nd = 6.0*(dApti_gpti + dApci_gpci);
	dCC_3rd = 6.0*(dAsti_gsti + dAsci_gsci);
	dDD_2nd = 6.0*(dApti_hpti_gpti + dApci_hpci_gpci);
	dDD_3rd = 6.0*(dAsti_hsti_gsti + dAsci_hsci_gsci);
	//
	dCC = 3.0*db0*dhf*(2.0*deN + dhf) + dCC_2nd + dCC_3rd;
	dDD = -1.0*db0*dhf*dhf*(3.0*deN + 2.0*dhf) - dDD_2nd - dDD_3rd;

	// 압축영역 높이 계산.
	double dx1 = dh;
	double dx2 = 0.0;
	double dxn = 0.0;
	double dfx = 0.0, dDfx = 0.0;
	double dDelta = 0.0;
	double dTol = 1.0; // 1mm
	BOOL bFindCompH = FALSE;
	BOOL bErrZero = FALSE;
	//
	double dZcr = 0.0;
	double dAcr = 0.0;
	double dAcrYcr = 0.0;
	double dIcr = 0.0;
	double dc = 0.0;
	double de0N = 0.0;
	double dAtn_cr = 0.0, dAtnYtn_cr = 0.0, dAtnYtn2_cr = 0.0;
	double dArb_cr = 0.0, dArbYrb_cr = 0.0, dArbYrb2_cr = 0.0;
	//
	double dcutYcent = 0.0, dcutZcent = 0.0, dcutArea = 0.0, dcutIyy = 0.0, dcutIzz = 0.0, dcutIxx = 0.0;
	double dConPosi_bot = 0.0;
	double dGapCr = 0.0;
	CArray<T_SPSC_COOR, T_SPSC_COOR> arGetOuter;
	CArray<T_SPSC_COOR_AR*, T_SPSC_COOR_AR*> arGetInner;
	T_SPSC_COOR CutPointD;
	arGetOuter.RemoveAll();
	arGetInner.RemoveAll();
	double dCutPos = 0.0;
	BOOL bCuttingLine = FALSE;
	BOOL bSkipCutSect = FALSE;
	BOOL bDirection = bPosiMom ? TRUE : FALSE;

	for (i = 0; i < 3; i++)
	{
		iCount = 0;
		do
		{
			dfx = Get_FunctionX(dAA, dBB, dCC, dDD, dx1);
			dDfx = Get_FunctionXprim(dAA, dBB, dCC, dDD, dx1);
			if (dDfx == 0.0)
			{
				bErrZero = TRUE;
				break;
			}
			dx2 = dx1 - dfx / dDfx;
			dDelta = fabs(dx2 - dx1);
			dx1 = dx2;
			if (iCount > 30) break;
			iCount++;
		} while (dDelta > dTol);
		//
		dCompH = dx1;
		if (iCount < 31) bFindCompH = TRUE;
		if (!bErrZero || bFindCompH) break;
		dx1 = dh * (2 + i);
	}

	// 압축영역 높이를 찾지 못하면 계산 못하는 것으로 함.
	if (!bFindCompH) goto END;
	// dCompH<=0.0
	if (dCompH <= 0.0) goto END;
	// dCompH>=dh  
	if (dCompH >= dh)
	{
		dCompH = dh;
		bSkipCutSect = TRUE;
	}

	// 압축영역 높이에서 단면특성 계산.
	// 정모멘트는 아래연단기준, 부모멘트는 위연단기준.
	dCutPos = bPosiMom ? dh - dCompH : dCompH;
	bCuttingLine = CSectUtil::Get_PolygonCuttingLine(arOuter, arInner, bDirection, bYaxis, dCutPos, dLength, arGetOuter, arGetInner, CutPointD);

	if (bCuttingLine)
	{
		if (!CSectUtil::GetPscCutSectProperty(arGetOuter, arGetInner, dcutYcent, dcutZcent, dcutArea, dcutIyy, dcutIzz, dcutIxx)) return FALSE;
	}

	// 균열단면의 Section Property 계산.
	/////////////////////////////////////////////////////////////////////////////////////////  
	// Tendon.
	for (i = 0; i < arTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(i);
		double dApi = Tndn.dArea;
		double dalpha_EPi = dEc == 0.0 ? 0.0 : Tndn.dEps / dEc;
		double dCompZi = bPosiMom ? dCpz - Tndn.dz : dzCen + Tndn.dz; //압축연단에서 부터의 거리로 변환.
		double dzi_bot = dh - dCompZi;
		if (dCompZi < dCompH)
		{
			dAtn_cr += (dalpha_EPi - 1.0)*dApi;
			dAtnYtn_cr += (dalpha_EPi - 1.0)*dApi*dzi_bot;
		}
		else
		{
			dAtn_cr += dalpha_EPi * dApi;
			dAtnYtn_cr += dalpha_EPi * dApi*dzi_bot;
		}
	}
	// Rebar
	for (i = 0; i < arRbarPosi.GetSize(); i++)
	{
		_DGN_RBAR_CRC Rbar = arRbarPosi.GetAt(i);
		double dAsi = Rbar.dArea;
		double dCompZi = bPosiMom ? dCpz - Rbar.dz : dzCen + Rbar.dz; //압축연단에서 부터의 거리로 변환.
		double dzi_bot = dh - dCompZi;
		if (dCompZi < dCompH)
		{
			dArb_cr += (dalpha_ES - 1.0)*dAsi;
			dArbYrb_cr += (dalpha_ES - 1.0)*dAsi*dzi_bot;
		}
		else
		{
			dArb_cr += dalpha_ES * dAsi;
			dArbYrb_cr += dalpha_ES * dAsi*dzi_bot;
		}
	}

	// Acr
	dAcr = dcutArea + dAtn_cr + dArb_cr;
	// Z centroid
	dConPosi_bot = bPosiMom ? dh - dCompH + dcutZcent : dh - dcutZcent;
	dAcrYcr = dcutArea * dConPosi_bot + dAtnYtn_cr + dArbYrb_cr;
	dZcr = dAcr == 0.0 ? 0.0 : dAcrYcr / dAcr;
	dc = dh - dZcr;  // 단면압축여역사이드에서 균열환산단면 중심축까지의 거리
	de0N = dc + deN; // Np0 작용점에서 균열단면 중심축까지의 거리.

	// Icr 계산.
	// Tendon.
	for (i = 0; i < arTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(i);
		double dApi = Tndn.dArea;
		double dalpha_EPi = dEc == 0.0 ? 0.0 : Tndn.dEps / dEc;
		double dCompZi = bPosiMom ? dCpz - Tndn.dz : dzCen + Tndn.dz; //압축연단에서 부터의 거리로 변환.
		double dzi_bot = dh - dCompZi;
		double ddpi = (dZcr - dzi_bot);
		double ddp2 = ddpi * ddpi;
		if (dCompZi < dCompH)
		{
			dAtnYtn2_cr += (dalpha_EPi - 1.0)*dApi*ddp2;
		}
		else
		{
			dAtnYtn2_cr += dalpha_EPi * dApi*ddp2;
		}
	}
	// Rebar
	for (i = 0; i < arRbarPosi.GetSize(); i++)
	{
		_DGN_RBAR_CRC Rbar = arRbarPosi.GetAt(i);
		double dAsi = Rbar.dArea;
		double dCompZi = bPosiMom ? dCpz - Rbar.dz : dzCen + Rbar.dz; //압축연단에서 부터의 거리로 변환.
		double dzi_bot = dh - dCompZi;
		double ddsi = (dZcr - dzi_bot);
		double dds2 = ddsi * ddsi;
		if (dCompZi < dCompH)
		{
			dArbYrb2_cr += (dalpha_ES - 1.0)*dAsi*dds2;
		}
		else
		{
			dArbYrb2_cr += dalpha_ES * dAsi*dds2;
		}
	}
	// Icr
	dGapCr = dConPosi_bot - dZcr;
	dIcr = dcutIyy + dcutArea * dGapCr*dGapCr + dAtnYtn2_cr + dArbYrb2_cr;
	/////////////////////////////////////////////////////////////////////////////////////////

	// 단면특성이 0일 수는 없다.
	if (dAcr == 0.0 || dIcr == 0.0) goto END;

	// 균열단면 프리스트레스철근의 응력증분(7.1.4-5)
	arCalcTndnPosi.SetSize(arTndnPosi.GetSize());
	for (i = 0; i < arTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(i);
		double dApi = Tndn.dArea;
		double dalpha_EPi = dEc == 0.0 ? 0.0 : Tndn.dEps / dEc;
		double dhpi = bPosiMom ? dCp - Tndn.dz : dzCen + Tndn.dz;
		//double dSigPi = dalpha_EPi*(dNp0/dA0 - dNp0*de0N*(dhpi-dc)/dIyy);        
		double dSigPi = dalpha_EPi * (dNp0 / dAcr - dNp0 * de0N*(dhpi - dc) / dIcr);
		Tndn.dStressLL = (Tndn.dStress[0] - Tndn.dStrLoss[0][0]) + dSigPi;
		arCalcTndnPosi.SetAt(i, Tndn);
	}

	// Delete because of New Operator.
	for (i = 0; i < arGetInner.GetSize(); i++)
	{
		if (arGetInner[i])
		{
			arGetInner[i]->RemoveAll();
			delete arGetInner[i];
			arGetInner[i] = NULL;
		}
	}
	arGetInner.RemoveAll();

	return TRUE;

END:;
	// Delete because of New Operator.
	for (i = 0; i < arGetInner.GetSize(); i++)
	{
		if (arGetInner[i])
		{
			arGetInner[i]->RemoveAll();
			delete arGetInner[i];
			arGetInner[i] = NULL;
		}
	}
	arGetInner.RemoveAll();

	return FALSE;
}

// f(x)
double CDgnJTG_D62_04::Get_FunctionX(double dA, double dB, double dC, double dD, double dx)
{
	double dValue = 0.0;
	dValue = dA * dx*dx*dx + dB * dx*dx + dC * dx + dD;
	return dValue;
}
// f'(x)
double CDgnJTG_D62_04::Get_FunctionXprim(double dA, double dB, double dC, double dD, double dx)
{
	double dValue = 0.0;
	dValue = 3.0*dA*dx*dx + 2.0*dB*dx + dC;
	return dValue;
}

BOOL CDgnJTG_D62_04::Get_ReqTendon(ElemPairK ElemK, UINT iIJ, UINT iTopBot,
	double dfck_p, double dftk_p, double dfck, double dftk, double dFlgThk[2],
	T_SPSC_SECT& SpscgSect, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi,
	CArray<_DGN_TNDN_CRC, _DGN_TNDN_CRC&>& arTndnPosi, T_TNDN_JTG_BASE& TndnJtg)
{
	// Material.
	//!/ 계BJ님에게 문의 
	double dSig_hL1 = 0.7*dftk_p;   // 시공단계 콘크리트의 한계인장응력 
	double dSig_ha1 = 0.7*dfck_p;   // 시공단계 콘크리트의 한계압축응력 
	double dSig_hL2 = 0.0;          // 사용단계 콘크리트의 한계인장응력 
	double dSig_ha2 = 0.5*dfck;     // 사용단계 콘크리트의 한계압축응력 

	double dfti = dSig_hL1;
	double dfci = dSig_ha1;
	double dfte = dSig_hL2;
	double dfce = dSig_ha2;

	// Section.
	double dh = SpscgSect.dzDim;   // 높이
	double dyb = SpscgSect.dzCen;   // 중립축에서 아래연단 까지 거리.
	double dyt = dh - dyb;          // 중립축에서 위연단 까지 거리.
	double dAg = SpscgSect.dArea;   // 전단면적
	double dIyy = SpscgSect.dIyy;    // Iyy
	// T_SPSC_SECT, Slt, Srt, Slb, Srb는 약축에 대한 단면상수..
	double dSlt = SpscgSect.dSlt;
	double dSrt = SpscgSect.dSrt;
	double dSlb = SpscgSect.dSlb;
	double dSrb = SpscgSect.dSrb;
	double dSt = max(dSlt, dSrt);
	double dSb = max(dSlb, dSrb);

	if (dIyy == 0.0) return FALSE;
	/////////////////////////
	dSt = dIyy / dyt;
	dSb = dIyy / dyb;
	/////////////////////////

	double dFlgThk_Top = dFlgThk[0];
	double dFlgThk_Bot = dFlgThk[1];
	// 직사각형일 경우 flange 두께가 전체높이임. 
	if (fabs(dFlgThk_Top - dh) < m_dZero)
	{
		dFlgThk_Top = dyt / 2.0;
		dFlgThk_Bot = dyb / 2.0;
	}

	// Moment. //!/
	double dM_DL = TndnJtg.dMg1;
	double dMa = TndnJtg.dMsum;
	double dMj = TndnJtg.dMj;

	double dAlphas;
	Get_Alphas(dAlphas);
	double dR = dAlphas;
	double dr2 = sqrt(dIyy / dAg);
	double dkb = dr2 / dyt; // 하핵점 : 위연단에 인장응력이 발생하지 않는 압축단면력의 한계위치.
	double dkt = dr2 / dyb; // 상핵점 : 아래연단에 인장응력이 발생하지 않는 압축단면력의 한계위치.  

	// e : 중립축 아래(+), 중립축 위(-) 
	BOOL bPi_Magnel = TRUE;
	BOOL bPi_Check = TRUE;
	double dPi_use = 0.0;
	double dPi_mid = 0.0;

	// 중립축을 기준으로 위 아래 텐던의 도심.
	double de = 0.0;

	//중립축을 기준으로 Tendon dy,dz를 수정.
	double dApt = 0.0, dNtodpt = 0.0, dapt = 0.0;
	double dApc = 0.0, dNtodpc = 0.0, dapc = 0.0;
	double dAst = 0.0, dNtodst = 0.0, dast = 0.0;
	double dAsc = 0.0, dNtodsc = 0.0, dasc = 0.0;
	double dat = 0.0, dac = 0.0;

	double dCtemp = dyt;
	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();
	double dfsd = 0.0; // Temp
	BOOL bCalcOK = Get_TndnReBarInfor(TRUE, dCtemp, SpscgSect, arRbarPosi, arTndnPosi, rTdRb, dfsd);
	if (iTopBot == 0) // 0=Top, 1=Bot
	{
		double dAtndn_Top = rTdRb.dApctol;
		if (dAtndn_Top > 0.0)
		{
			de = -rTdRb.dNtodpc;
		}
		else if (dAtndn_Top <= 0.0)
		{
			de = -(dyt - dFlgThk_Top / 2.0);
		}
	}
	else if (iTopBot == 1)
	{
		double dAtndn_Bot = rTdRb.dApttol;
		if (dAtndn_Bot > 0.0)
		{
			de = rTdRb.dNtodpt;
		}
		else if (dAtndn_Bot <= 0.0)
		{
			de = dyb - dFlgThk_Bot / 2.0;
		}
	}

	// 재하능력극한상태로 계산할 때 필요.
	if (dMj >= 0.0)
	{
		dApt = rTdRb.dApttol, dNtodpt = (dApt == 0.0 ? 0.0 : rTdRb.dNtodpt), dapt = (dApt == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dApc = rTdRb.dApctol, dNtodpc = (dApc == 0.0 ? 0.0 : rTdRb.dNtodpc), dapc = rTdRb.ddpc;
		dAst = rTdRb.dAsttol, dast = (dAst == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dAsc = rTdRb.dAsctol, dasc = rTdRb.ddsc;
		dat = rTdRb.daBott;
		dac = rTdRb.daTopc;
	}
	else if (dMj < 0.0)
	{
		dApt = rTdRb.dApctol, dNtodpt = (dApt == 0.0 ? 0.0 : rTdRb.dNtodpc), dapt = rTdRb.ddpc;
		dApc = rTdRb.dApttol, dNtodpc = (dApc == 0.0 ? 0.0 : rTdRb.dNtodpt), dapc = (dApc == 0.0 ? 0.0 : dh - rTdRb.ddpt);
		dAst = rTdRb.dAsctol, dast = rTdRb.ddsc;
		dAsc = rTdRb.dAsttol, dasc = (dAsc == 0.0 ? 0.0 : dh - rTdRb.ddst);
		dat = rTdRb.daTopt;
		dac = rTdRb.daBotc;
	}

	// Magnel.    
	double dPti = 0.0;
	double dPci = 0.0;
	double dPte = 0.0;
	double dPce = 0.0;
	double dPi_min = 0.0;
	double dPi_max = 0.0;
	double dPi_req = 0.0;

	double dParam_te = 0.0;
	double dParam_ce = 0.0;
	double dParam_ci = 0.0;
	double dParam_ti = 0.0;
	double dPti_Top = 0.0;
	double dPti_Bot = 0.0;
	double dPci_Top = 0.0;
	double dPci_Bot = 0.0;
	double dParam_ci_Top = 0.0;
	double dParam_ti_Bot = 0.0;
	double dParam_ti_Top = 0.0;
	double dParam_ci_Bot = 0.0;

	if (dM_DL >= 0.0 && dMa >= 0.0)
	{
		// 정모멘트.    
		// 식(A) : 위연단 시공단계 허용인장응력을 만족할 때.
		double dPti = 0.0;
		double dParam_ti = 1.0 / dAg - de / dSt;
		if (dParam_ti != 0.0)
		{
			dPti = (-dfti - dM_DL / dSt) / dParam_ti;
			// if(dParam_ti>0.0) {}   // dPi_scope > dPti
			// if(dParam_ti<0.0) {}   // dPi_scope < dPti
		}

		// 식(B) : 아래연단 시공단계 허용압축응력을 만족할 때. 
		double dParam_ci = 1.0 / dAg + de / dSb;
		double dPci = (dfci + dM_DL / dSb) / dParam_ci;
		// dPi_scope < dPci

		// 식(C) : 위연단 사용단계 허용압축응력을 만족할 때.
		double dPce = 0.0;
		double dParam_ce = dR * (1.0 / dAg - de / dSt);
		if (dParam_ce != 0.0)
		{
			dPce = (dfce - dMa / dSt) / dParam_ce;
			// if(dParam_ce>0.0) {}   // dPi_scope < dPce
			// if(dParam_ce<0.0) {}   // dPi_scope > dPce
		}

		// 식(D) : 아래연단 사용단계 허용인장응력을 만족할 때.
		double dParam_te = dR * (1.0 / dAg + de / dSb);
		double dPte = (-dfte + dMa / dSb) / dParam_te;
		// dPi_scope > dPte

		if (dParam_ti > 0.0)
		{
			// dParam_ce>0.0 이다.

			// dPi_scope > dPti
			// dPi_scope < dPci
			// dPi_scope < dPce
			// dPi_scope > dPte
			dPi_min = max(dPti, dPte);
			dPi_max = min(dPci, dPce);
			if (dPi_min < dPi_max) { dPi_req = dPi_min; }
			else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
			else { dPi_req = dPi_min; }
		}
		else if (dParam_ti < 0.0)
		{
			// dParam_ce<0.0 이다.

			// dPi_scope < dPti
			// dPi_scope < dPci
			// dPi_scope > dPce
			// dPi_scope > dPte
			dPi_min = max(dPce, dPte);
			dPi_max = min(dPti, dPci);
			if (dPi_min < dPi_max) { dPi_req = dPi_min; }
			else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
			else { dPi_req = dPi_min; }
		}
		else //dParam_ti==0.0
		{
			// dPi_scope < dPci
			// dPi_scope > dPte
			dPi_min = dPte;
			dPi_max = dPci;
			if (dPi_min < dPi_max) { dPi_req = dPi_min; }
			else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
			else { dPi_req = dPi_min; }
		}
	}

	else if (dM_DL <= 0.0 && dMa <= 0.0)
	{
		if (dM_DL != 0.0 && dMa != 0.0)
		{
			// 부모멘트.    
			// 식(A) : 위연단 사용단계 허용인장응력을 만족할 때.
			dPte = 0.0;
			dParam_te = dR * (1.0 / dAg - de / dSt);
			if (dParam_te != 0.0)
			{
				dPte = (-dfte - dMa / dSt) / dParam_te;
				// if(dParam_te>0.0) {}   // dPi_scope > dPte
				// if(dParam_te<0.0) {}   // dPi_scope < dPte
			}

			// 식(B) : 아래연단 사용단계 허용압축응력을 만족할 때. 
			dParam_ce = dR * (1.0 / dAg + de / dSb);
			dPce = (dfce + dMa / dSb) / dParam_ce;
			// dPi_scope < dPce

			// 식(C) : 위연단 시공단계 허용압축응력을 만족할 때.
			dPce = 0.0;
			dParam_ci = (1.0 / dAg - de / dSt);
			if (dParam_ci != 0.0)
			{
				dPci = (dfci - dM_DL / dSt) / dParam_ci;
				// if(dParam_ci>0.0) {}   // dPi_scope < dPci
				// if(dParam_ci<0.0) {}   // dPi_scope > dPci
			}

			// 식(D) : 아래연단 시공단계 허용인장응력을 만족할 때.
			dParam_ti = (1.0 / dAg + de / dSb);
			dPti = (-dfti + dM_DL / dSb) / dParam_ti;
			// dPi_scope > dPti

			if (dParam_te > 0.0)
			{
				// dParam_ce>0.0 이다.

				// dPi_scope > dPte
				// dPi_scope < dPce
				// dPi_scope < dPci
				// dPi_scope > dPti
				dPi_min = max(dPte, dPti);
				dPi_max = min(dPce, dPci);
				if (dPi_min < dPi_max) { dPi_req = dPi_min; }
				else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
				else { dPi_req = dPi_min; }
			}
			else if (dParam_te < 0.0)
			{
				// dParam_ce<0.0 이다.

				// dPi_scope < dPte
				// dPi_scope < dPce
				// dPi_scope > dPci
				// dPi_scope > dPti
				dPi_min = max(dPci, dPti);
				dPi_max = min(dPte, dPce);
				if (dPi_min < dPi_max) { dPi_req = dPi_min; }
				else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
				else { dPi_req = dPi_min; }
			}
			else //dParam_te==0.0
			{
				// dPi_scope < dPce
				// dPi_scope > dPti
				dPi_min = dPti;
				dPi_max = dPce;
				if (dPi_min < dPi_max) { dPi_req = dPi_min; }
				else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
				else { dPi_req = dPi_min; }
			}
		}

	}

	// 시공단계 정모멘트 , 사용단계 부모멘트.
	else if (dM_DL >= 0.0 && dMa <= 0.0)
	{
		if (dM_DL != 0.0 && dMa != 0.0)
		{
			// 정모멘트.    
			// 식(A) : 위연단 시공단계 허용인장응력을 만족할 때.
			double dParam_ti_Top = 1.0 / dAg - de / dSt;
			if (dParam_ti_Top != 0.0)
			{
				double dPti_Top = (-dfti - dM_DL / dSt) / dParam_ti_Top;
				// if(dParam_ti_Top>0.0)  // dPi_scope > dPti_Top
				// if(dParam_ti_Top<0.0)  // dPi_scope < dPti_Top
			}

			// 식(B) : 아래연단 시공단계 허용압축응력을 만족할 때. 
			double dParam_ci_Bot = 1.0 / dAg + de / dSb;
			if (dParam_ci_Bot != 0.0)
			{
				double dPci_Bot = (dfci + dM_DL / dSb) / dParam_ci_Bot;
				// if(dParam_ci_Bot>0.0)  // dPi_scope < dPci_Bot
				// if(dParam_ci_Bot<0.0)  // dPi_scope > dPci_Bot
			}

			// 식(C) : 위연단 시공단계 허용압축응력을 만족할 때.
			double dParam_ci_Top = 1.0 / dAg - de / dSt;
			if (dParam_ci_Top != 0.0)
			{
				double dPci_Top = (dfci - dM_DL / dSt) / dParam_ci_Top;
				// if(dParam_ci_Top>0.0)  // dPi_scope < dPci_Bot
				// if(dParam_ci_Top<0.0)  // dPi_scope > dPci_Bot
			}

			// 식(D) : 아래연단 시공단계 허용인장응력을 만족할 때.
			double dParam_ti_Bot = 1.0 / dAg + de / dSb;
			if (dParam_ti_Bot != 0.0)
			{
				double dPti_Bot = (-dfti + dM_DL / dSb) / dParam_ti_Bot;
				// if(dParam_ti_Bot>0.0)  // dPi_scope > dPti_Bot
				// if(dParam_ti_Bot<0.0)  // dPi_scope < dPti_Bot
			}


			// 식(E) : 위연단 사용단계 허용인장응력을 만족할 때.
			double dParam_te = dR * (1.0 / dAg - de / dSt);
			if (dParam_te != 0.0)
			{
				double dPte = (-dfte - dMa / dSt) / dParam_te;
				// if(dParam_te>0.0) {}   // dPi_scope > dPte
				// if(dParam_te<0.0) {}   // dPi_scope < dPte
			}

			// 식(F) : 아래연단 사용단계 허용인장응력을 만족할 때.
			double dParam_ce = dR * (1.0 / dAg + de / dSb);
			if (dParam_ce != 0.0)
			{
				double dPce = (dfce + dMa / dSb) / dParam_ce;
				// if(dParam_ce>0.0) {}   // dPi_scope < dPce
				// if(dParam_ce<0.0) {}   // dPi_scope > dPce
			}


			if (dParam_ti_Top > 0.0 && dParam_ci_Top > 0.0)
			{
				// if dParam_ti_Top>0, ( dParam_ci_Bot>0, dParam_te>0 )
				// if dParam_ci_Top>0, ( dParam_ti_Bot>0, dParam_ce>0 ) 

				// (A)
				// if(dParam_ti_Top>0) {}   // dPi_scope > dPti_Top
				// if(dParam_ti_Top<0) {}   // dPi_scope < dPti_Top
				// (B)
				// if(dParam_ci_Top>0) {}   // dPi_scope < dPci_Top
				// if(dParam_ci_Top<0) {}   // dPi_scope > dPci_Top
				// (C)
				// if(dParam_ci_Bot>0) {}   // dPi_scope < dPci_Bot
				// if(dParam_ci_Bot<0) {}   // dPi_scope > dPci_Bot
				// (D)
				// if(dParam_ti_Bot>0) {}   // dPi_scope > dPti_Bot
				// if(dParam_ti_Bot<0) {}   // dPi_scope < dPti_Bot
				// (E)
				// if(dParam_te>0.0) {}   // dPi_scope > dPte
				// if(dParam_te<0.0) {}   // dPi_scope < dPte
				// (F)
				// if(dParam_ce>0.0) {}   // dPi_scope < dPce
				// if(dParam_ce<0.0) {}   // dPi_scope > dPce

				// if(dParam_ti_Top>0) {}   // dPi_scope > dPti_Top
				// if(dParam_ci_Top>0) {}   // dPi_scope < dPci_Top
				// if(dParam_ci_Bot>0) {}   // dPi_scope < dPci_Bot
				// if(dParam_ti_Bot>0) {}   // dPi_scope > dPti_Bot
				// if(dParam_te>0.0) {}     // dPi_scope > dPte
				// if(dParam_ce>0.0) {}     // dPi_scope < dPce


				dPi_min = max(max(dPti_Top, dPti_Bot), dPte);
				dPi_max = min(min(dPci_Top, dPci_Bot), dPce);
				if (dPi_min < dPi_max) { dPi_req = dPi_min; }
				else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
				else { dPi_req = dPi_min; }
			}
			else if (dParam_ti_Top < 0.0 && dParam_ci_Top < 0.0)
			{
				// if(dParam_ti_Top<0) {}   // dPi_scope < dPti_Top
				// if(dParam_ci_Top<0) {}   // dPi_scope > dPci_Top
				// if(dParam_ci_Bot<0) {}   // dPi_scope > dPci_Bot
				// if(dParam_ti_Bot<0) {}   // dPi_scope < dPti_Bot
				// if(dParam_te<0.0) {}     // dPi_scope < dPte
				// if(dParam_ce<0.0) {}     // dPi_scope > dPce

				dPi_min = max(max(dPci_Top, dPci_Bot), dPce);
				dPi_max = min(min(dPti_Top, dPti_Bot), dPte);
				if (dPi_min < dPi_max) { dPi_req = dPi_min; }
				else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
				else { dPi_req = dPi_min; }
			}
			else if (dParam_ti_Top > 0.0 && dParam_ci_Top < 0.0)
			{
				// if dParam_ti_Top>0, ( dParam_ci_Bot>0, dParam_te>0 )
				// if dParam_ci_Top>0, ( dParam_ti_Bot>0, dParam_ce>0 ) 

				// if(dParam_ti_Top>0) {}   // dPi_scope > dPti_Top
				// if(dParam_ci_Top<0) {}   // dPi_scope > dPci_Top
				// if(dParam_ci_Bot>0) {}   // dPi_scope < dPci_Bot
				// if(dParam_ti_Bot<0) {}   // dPi_scope < dPti_Bot
				// if(dParam_te>0.0) {}     // dPi_scope > dPte
				// if(dParam_ce<0.0) {}     // dPi_scope > dPce

				dPi_min = max(max(dPti_Top, dPci_Top), max(dPte, dPce));
				dPi_max = min(dPte, dPce);
				if (dPi_min < dPi_max) { dPi_req = dPi_min; }
				else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
				else { dPi_req = dPi_min; }
			}
			else if (dParam_ti_Top<0.0 && dParam_ci_Top>0.0)
			{
				// if(dParam_ti_Top<0) {}   // dPi_scope < dPti_Top
				// if(dParam_ci_Top>0) {}   // dPi_scope < dPci_Top
				// if(dParam_ci_Bot<0) {}   // dPi_scope > dPci_Bot
				// if(dParam_ti_Bot>0) {}   // dPi_scope > dPti_Bot
				// if(dParam_te<0.0) {}     // dPi_scope < dPte
				// if(dParam_ce>0.0) {}     // dPi_scope < dPce

				dPi_min = max(dPci_Bot, dPti_Bot);
				dPi_max = min((dPti_Top, dPci_Top), min(dPte, dPce));
				if (dPi_min < dPi_max) { dPi_req = dPi_min; }
				else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
				else { dPi_req = dPi_min; }
			}
		}
	}

	// 시공단계 부모멘트 , 사용단계 정모멘트.
	else if (dM_DL <= 0.0 && dMa >= 0.0)
	{
		if (dM_DL != 0.0 && dMa != 0.0)
		{
			// 부모멘트.    
			// 식(A) : 위연단 시공단계 허용압축응력을 만족할 때.
			dParam_ci_Top = 1.0 / dAg - de / dSt;
			if (dParam_ci_Top != 0.0)
			{
				dPci_Top = (dfci - dM_DL / dSt) / dParam_ci_Top;
				// if(dParam_ci_Top>0.0) {}   // dPi_scope < dPci_Top
				// if(dParam_ci_Top<0.0) {}   // dPi_scope > dPci_Top
			}

			// 식(B) : 아래연단 시공단계 허용인장응력을 만족할 때. 
			dParam_ti_Bot = 1.0 / dAg + de / dSb;
			if (dParam_ti_Bot != 0.0)
			{
				dPti_Bot = (-dfti + dM_DL / dSb) / dParam_ti_Top;
				// if(dParam_ti_Bot>0.0) {}   // dPi_scope > dPti_Bot
				// if(dParam_ti_Bot<0.0) {}   // dPi_scope < dPti_Bot
			}

			// 식(C) : 위연단 시공단계 허용인장응력을 만족할 때.
			dParam_ti_Top = 1.0 / dAg - de / dSt;
			if (dParam_ti_Top != 0.0)
			{
				dPti_Top = (-dfti - dM_DL / dSt) / dParam_ti_Top;
				// if(dParam_ti_Top>0.0) {}   // dPi_scope > dPti_Top
				// if(dParam_ti_Top<0.0) {}   // dPi_scope < dPti_Top
			}

			// 식(D) : 아래연단 시공단계 허용압축응력을 만족할 때.
			dParam_ci_Bot = 1.0 / dAg + de / dSb;
			if (dParam_ci_Bot != 0.0)
			{
				dPci_Bot = (dfci + dM_DL / dSb) / dParam_ci_Bot;
				// if(dParam_ci_Bot>0.0) {}   // dPi_scope < dPci_Bot
				// if(dParam_ci_Bot<0.0) {}   // dPi_scope > dPci_Bot
			}

			// 식(E) : 위연단 사용단계 허용압축응력을 만족할 때.
			dParam_ce = dR * (1.0 / dAg - de / dSt);
			if (dParam_ce != 0.0)
			{
				dPce = (dfce - dMa / dSt) / dParam_ce;
				// if(dParam_ce>0.0) {}   // dPi_scope < dPce
				// if(dParam_ce<0.0) {}   // dPi_scope > dPce
			}

			// 식(F) : 아래연단 사용단계 허용인장응력을 만족할 때.
			dParam_te = dR * (1.0 / dAg + de / dSb);
			if (dParam_te != 0.0)
			{
				dPte = (-dfte + dMa / dSb) / dParam_te;
				// if(dParam_te>0.0) {}   // dPi_scope > dPte
				// if(dParam_te<0.0) {}   // dPi_scope < dPte
			}

			if (dParam_ci_Top > 0.0 && dParam_ti_Bot > 0.0)
			{
				// if dParam_ci_Top>0, ( dParam_ti_Top>0, dParam_ce>0 )
				// if dParam_ti_Bot>0, ( dParam_ci_Bot>0, dParam_te>0 ) 

				// (A)
				// if(dParam_ci_Top>0.0)  // dPi_scope < dPci_Top
				// if(dParam_ci_Top<0.0)  // dPi_scope > dPci_Top
				// (B)
				// if(dParam_ti_Bot>0.0)  // dPi_scope > dPti_Bot
				// if(dParam_ti_Bot<0.0)  // dPi_scope < dPti_Bot
				// (C)
				// if(dParam_ti_Top>0.0)  // dPi_scope > dPti_Top
				// if(dParam_ti_Top<0.0)  // dPi_scope < dPti_Top
				// (D)
				// if(dParam_ci_Bot>0.0)  // dPi_scope < dPci_Bot
				// if(dParam_ci_Bot<0.0)  // dPi_scope > dPci_Bot
				// (E)
				// if(dParam_ce>0.0)      // dPi_scope < dPce
				// if(dParam_ce<0.0)      // dPi_scope > dPce
				// (F)
				// if(dParam_te>0.0)      // dPi_scope > dPte
				// if(dParam_te<0.0)      // dPi_scope < dPte

				// if(dParam_ci_Top>0.0)  // dPi_scope < dPci_Top
				// if(dParam_ti_Bot>0.0)  // dPi_scope > dPti_Bot
				// if(dParam_ti_Top>0.0)  // dPi_scope > dPti_Top
				// if(dParam_ci_Bot>0.0)  // dPi_scope < dPci_Bot
				// if(dParam_ce>0.0)      // dPi_scope < dPce
				// if(dParam_te>0.0)      // dPi_scope > dPte

				dPi_min = max(max(dPti_Bot, dPti_Top), dPte);
				dPi_max = min(min(dPci_Top, dPci_Bot), dPce);
				if (dPi_min < dPi_max) { dPi_req = dPi_min; }
				else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
				else { dPi_req = dPi_min; }
			}
			else if (dParam_ci_Top < 0.0 && dParam_ti_Bot < 0.0)
			{
				// if(dParam_ci_Top<0.0)  // dPi_scope > dPci_Top
				// if(dParam_ti_Bot<0.0)  // dPi_scope < dPti_Bot
				// if(dParam_ti_Top<0.0)  // dPi_scope < dPti_Top
				// if(dParam_ci_Bot<0.0)  // dPi_scope > dPci_Bot
				// if(dParam_ce<0.0)      // dPi_scope > dPce
				// if(dParam_te<0.0)      // dPi_scope < dPte

				dPi_min = max(max(dPci_Top, dPci_Bot), dPce);
				dPi_max = min(min(dPti_Top, dPti_Bot), dPte);
				if (dPi_min < dPi_max) { dPi_req = dPi_min; }
				else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
				else { dPi_req = dPi_min; }
			}
			else if (dParam_ci_Top > 0.0 && dParam_ti_Bot < 0.0)
			{
				// if dParam_ci_Top>0, ( dParam_ti_Top>0, dParam_ce>0 )
				// if dParam_ti_Bot>0, ( dParam_ci_Bot>0, dParam_te>0 ) 

				// if(dParam_ci_Top>0.0)  // dPi_scope < dPci_Top
				// if(dParam_ti_Bot<0.0)  // dPi_scope < dPti_Bot
				// if(dParam_ti_Top>0.0)  // dPi_scope > dPti_Top
				// if(dParam_ci_Bot<0.0)  // dPi_scope > dPci_Bot
				// if(dParam_ce>0.0)      // dPi_scope < dPce
				// if(dParam_te<0.0)      // dPi_scope < dPte

				dPi_min = max(dPti_Top, dPci_Bot);
				dPi_max = min(min(dPci_Top, dPti_Bot), min(dPce, dPte));
				if (dPi_min < dPi_max) { dPi_req = dPi_min; }
				else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
				else { dPi_req = dPi_min; }
			}
			else if (dParam_ci_Top<0.0 && dParam_ti_Bot>0.0)
			{
				// if(dParam_ci_Top<0.0)  // dPi_scope > dPci_Top
				// if(dParam_ti_Bot>0.0)  // dPi_scope > dPti_Bot
				// if(dParam_ti_Top<0.0)  // dPi_scope < dPti_Top
				// if(dParam_ci_Bot>0.0)  // dPi_scope < dPci_Bot
				// if(dParam_ce<0.0)      // dPi_scope > dPce
				// if(dParam_te>0.0)      // dPi_scope > dPte

				dPi_min = max(max(dPci_Top, dPti_Bot), max(dPce, dPte));
				dPi_max = min(dPti_Top, dPci_Bot);
				if (dPi_min < dPi_max) { dPi_req = dPi_min; }
				else if (dPi_min > dPi_max) { bPi_Check = FALSE; }
				else { dPi_req = dPi_min; }
			}
			else ASSERT(0);
		}
	}

	// 검토
	//double dftopi = dPi/dAg - (dPi*de)*dyt/dIyy + dM_DL*dyt/dIyy;
	//double dfboti = dPi/dAg + (dPi*de)*dyb/dIyy - dM_DL*dyb/dIyy;
	//double dftope = dPe/dAg - (dPe*de)*dyt/dIyy + dMa  *dyt/dIyy;
	//double dfbote = dPe/dAg + (dPe*de)*dyb/dIyy - dMa  *dyb/dIyy;


	// 재하능력극한생태로 예측계산.
	double dRa = dfck;              // 콘크리트의 축방향 압축설계강도
	double db = SpscgSect.dyDim;   // 직사각형 너비
	double dh0 = 0.0;               // 단면의 유효높이
	double dRy = 0.0;               // 인장영역 프리스트레스 인장설계강도  
	double dalpha1 = 0.0;
	Get_Alpha1(dalpha1);

	if (dat > 0.0) dh0 = dh - dat;
	else        dh0 = 0.8*dh;

	// Tendon.
	double dfpdt = 1.0E+7, dfpdti = 0.0;
	double dfpdb = 1.0E+7, dfpdbi = 0.0;
	BOOL bTopExist = FALSE, bBotExist = FALSE;
	for (int i = 0; i < arTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(i);

		if (Tndn.dz < 0.0)
		{
			bBotExist = TRUE;
			dfpdbi = Tndn.dfpdt;
			dfpdb = min(dfpdb, dfpdbi);
		}
		else if (Tndn.dz > 0.0)
		{
			bTopExist = TRUE;
			dfpdti = Tndn.dfpdt;
			dfpdt = min(dfpdt, dfpdti);
		}
	}

	// Chagne by Jaeoh ('05.04.23)
	if (iTopBot == 0)
	{
		//!/ Top,Bot에 텐던이 다 없다면?.
		if (bTopExist) dRy = dfpdt;
		else          dRy = dfpdb; // 압축텐던이 없으면 인장텐던의 값으로. 
	}
	else if (iTopBot == 1)
	{
		if (bBotExist) dRy = dfpdb;
		else          dRy = dfpdt;
	}

	if (!bTopExist && !bBotExist) { dRy = 0.0; }

	// 콘트리트 안전계수.
	double drc, drs;
	Get_SafetyCoeff(drc, drs);

	// 직사각형일 경우. 
	double dAtnStrn = 0.0;
	double dNyStrn = 0.0;
	if (fabs(dFlgThk_Top - dh) < m_dZero)
	{
		if (dRa*db*dh0*dh0 == 0.0 || 1.0 - 2.0*drc*fabs(dMj) / (dRa*db*dh0*dh0) < 0.0)
		{
			dNyStrn = 0.0;
			dAtnStrn = 0.0;
		}
		else
		{
			dNyStrn = dRa * db*dh0*(1.0 - sqrt(1.0 - 2.0*drc*fabs(dMj) / (dRa*db*dh0*dh0)));
			dAtnStrn = (dRy == 0.0 ? 0.0 : dNyStrn / dRy);
		}
	}
	else
	{
		dNyStrn = (dalpha1*dh == 0.0 ? 0.0 : drc * fabs(dMj) / (dalpha1*dh));
		dAtnStrn = (dRy == 0.0 ? 0.0 : dNyStrn / dRy);
	}

	if (iTopBot == 0)
	{
		if (dMj > 0.0) { dNyStrn = 0.0; dAtnStrn = 0.0; }
	}
	else if (iTopBot == 1)
	{
		if (dMj < 0.0) { dNyStrn = 0.0; dAtnStrn = 0.0; }
	}

	// dAtnStrn : 재하능력극한상태로 예측계산.
	// dAymin   : Magnel공식을 이용한 계산. 
	double dAymin = 0.0;
	dAymin = (dRy == 0.0 ? 0.0 : dPi_req / dRy);

	TndnJtg.dey = -de; // Table에 표현할 때는 Top(+), Bottom(-)
	TndnJtg.dNymin = max(dPi_req, dNyStrn);
	TndnJtg.dAymin = max(dAymin, dAtnStrn); // 두 조건단 만족하기 위해. 
	TndnJtg.dAyuse = 0.0; //dAyuse;

	return bCalcOK;
}

void CDgnJTG_D62_04::GetMcrByJTG(ElemPairK ElemK, int iSerialLcom, BOOL bIsEndI, _PSC_SECT_POSI& sectposi, double& dMcr, double dftk)
{

	// Section
	double dyDim = sectposi.SpscSect.dyDim;
	double dzDim = sectposi.SpscSect.dzDim;
	double dyCen = sectposi.SpscSect.dyCen;
	double dzCen = sectposi.SpscSect.dzCen;
	double dCpy = dyDim - dyCen;
	double dCpz = dzDim - dzCen;
	double dA0 = sectposi.SpscSect.dArea;  // 전체환산단면적.
	double dIyy = sectposi.SpscSect.dIyy;
	double dIzz = sectposi.SpscSect.dIzz;

	_DGN_FORC_CRC ForceCrc;
	ForceCrc.Initialize();
	m_pDataCtrl->GetDesignForceResult()->GetForceLcomResult(ElemK, iSerialLcom, ForceCrc);
	double dMd = bIsEndI ? ForceCrc.dMuy[0] : ForceCrc.dMuy[1];
	double dh = dzDim;
	double dbf_top = dyDim;
	double dbf_bot = fabs(sectposi.dpMomPos[0][0] - sectposi.dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.
	double db = (dMd >= 0.0 ? dbf_top : dbf_bot);

	double dCp = 0.0;
	if (dMd >= 0.0) dCp = dCpz;
	else         dCp = dzCen;

	double dhf = (dMd >= 0.0 ? sectposi.dpFlgThk[0] : sectposi.dpFlgThk[1]);

	double dShrThk[3];
	dShrThk[0] = sectposi.dpShrThk[0];
	dShrThk[1] = sectposi.dpShrThk[1];
	dShrThk[2] = sectposi.dpShrThk[2];
	double dbw = min(min(dShrThk[0], dShrThk[1]), dShrThk[2]);

	double dS0 = 0.0;
	if (dCpz <= dhf)
	{
		dS0 = db * dCp * dCp / 2.0;
	}
	else
	{
		dS0 = db * dhf * (dCp - 0.5*dhf) + dbw * (dCp - dhf) * (dCp - dhf - 0.5*(dCp - dhf));
	}

	double dW0 = fabs(dzDim - dCp) < 1.0e-6 ? 0.0 : dIyy / (dzDim - dCp);

	double gama = fabs(dW0) < 1.0e-6 ? 0.0 : 2 * dS0 / dW0;
	double dsigpc = dMcr;
	dMcr = (dMcr + gama * dftk)*dW0;

	//////////////////////////////////////////////////////////////////////////
	CString strout = _T("");
	strout.Format(_T("\n Elem:%u,EndIJ:%d,S0:%0.6f,W0:%0.6f,sigpc:%0.6f,r:%0.6f,Iyy:%0.6f,y0:%0.6f=Mcr:%0.6f"),
		ElemK, bIsEndI ? 0 : 1, dS0, dW0, dsigpc, gama, dIyy, dzDim - dCp, dMcr);
	OutputDebugString(strout);
}

void CDgnJTG_D62_04::Get_Sig_pc(ElemPairK ElemK, _DGN_STRS_CRC& StrsMax, _DGN_STRS_CRC& StrsMin)
{
	_DGN_STRS_CRC  StressTnpr;
	_DGN_STRS_CRC  StressTnsc;

	int iLcomNum = m_pForcCtrl->Get_LcomDataCount();

	for (int j = 0; j < iLcomNum; j++)
	{
		int iSerialLcom = j + 1;
		if (!m_pForcCtrl->Is_LcomForStrs(iSerialLcom))	continue; // Only 정상사용극한상태.

		BOOL bElasticStage = m_pForcCtrl->Is_LcomForElst(iSerialLcom);
		BOOL bServiceStage = m_pForcCtrl->Is_LcomForServ(iSerialLcom);

		StressTnpr.Initialize();
		StressTnsc.Initialize();

		m_pDataCtrl->GetDesignForceResult()->GetStressTnprResult(ElemK, iSerialLcom, StressTnpr);
		m_pDataCtrl->GetDesignForceResult()->GetStressTnscResult(ElemK, iSerialLcom, StressTnsc);

		StressTnsc += StressTnpr;
		for (int i = 0; i < cPOSI_CRC; i++)
		{
			// Add by ZINU.('05.01.04). Add Stress by Beam Section Temperature.
			// Chang by Jaeoh. (05.12.15).

			// Combined Stresses.
			double dStop = StressTnsc.dStop[i];	// 상단.
			double dSbot = StressTnsc.dSbot[i];	// 하단.
			double dStl = StressTnsc.dStl[i];	// 상단좌단.
			double dStr = StressTnsc.dStr[i];	// 상단우단.
			double dSbr = StressTnsc.dSbr[i];	// 하단우단.
			double dSbl = StressTnsc.dSbl[i];	// 하단좌단.
			double dSxx = StressTnsc.dSxx[i];
			// Change by ZINU.('04.04.18). Tension(-), Compression(+) by Kouno.
			dStop *= -1;
			dSbot *= -1;
			dStl *= -1;
			dStr *= -1;
			dSbr *= -1;
			dSbl *= -1;
			dSxx *= -1;
			//		  double dSmax = max(max(dStl,dStr),max(dSbr,dSbl));
			//		  double dSmin = min(min(dStl,dStr),min(dSbr,dSbl));
			double dSmax = max(max(max(dStl, dStr), max(dSbr, dSbl)), max(dStop, dSbot));//by maxiao
			double dSmin = min(min(min(dStl, dStr), min(dSbr, dSbl)), min(dStop, dSbot));
			// Chnage by ZINU.('04.08.27). Divide Stresses by dExtraRat.
			if (bElasticStage && ((dSmax) > (StrsMax.dScom[i] / StrsMax.dExtraRat[i]) || StrsMax.dScom[i] == 0.0))
			{
				StrsMax.iRefNo[i] = iSerialLcom;
				StrsMax.dStop[i] = dStop;
				StrsMax.dSbot[i] = dSbot;
				StrsMax.dStl[i] = dStl;
				StrsMax.dStr[i] = dStr;
				StrsMax.dSbl[i] = dSbl;
				StrsMax.dSbr[i] = dSbr;
				StrsMax.dScom[i] = dSmax;
				StrsMax.dExtraRat[i] = 1.0;
				StrsMax.dSxx[i] = dSxx;
			}
			if (bServiceStage && ((dSmin) < ((StrsMin.dScom[i]) / StrsMin.dExtraRat[i]) || StrsMin.dScom[i] == 0.0))
			{

				StrsMin.iRefNo[i] = iSerialLcom;
				StrsMin.dStop[i] = dStop;
				StrsMin.dSbot[i] = dSbot;
				StrsMin.dStl[i] = dStl;
				StrsMin.dStr[i] = dStr;
				StrsMin.dSbl[i] = dSbl;
				StrsMin.dSbr[i] = dSbr;
				StrsMin.dScom[i] = dSmin;
				StrsMin.dExtraRat[i] = 1.0;
				StrsMin.dSxx[i] = dSxx;
			}
		}
	}
}

BOOL CDgnJTG_D62_04::Get_PscTndnTdna(T_TDNA_K TdnaK, _PSC_TNDN_TDNA_W& TndnTdna)
{
	_PSC_TNDN_TDNA TdnaD;
	CDgnPscCommon::Get_PscTndnTdna(TdnaK, TdnaD);
	ConvertTndnTdna(TdnaD, TndnTdna);

	return TRUE;
}

void CDgnJTG_D62_04::ConvertTndnTdna(const _PSC_TNDN_TDNA& InD, _PSC_TNDN_TDNA_W& rData)
{
	rData.iLoadType = InD.iLoadType;
	rData.iRelxType = InD.iRelxType;
	rData.dfpu = InD.dfpu;
	rData.dfpy = InD.dfpy;
	rData.dfpk = InD.dfpk;
	rData.dfpdt = InD.dfpdt;
	rData.dfpdc = InD.dfpdc;
	rData.dFp01 = InD.dFp01;
	rData.dfp01k = InD.dfp01k;
	rData.dfpd = InD.dfpd;
	rData.dTendonLength = InD.dTendonLength;
	rData.iBoundaryNum = InD.iBoundaryNum;
}

/*
BOOL CDgnJTG_D62_04::Get_TndnReBarTransform(BOOL bYaxis, double dMu, double dNeuC, double dCp, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi,
CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& rarRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& rarTndnPosi,
CArray<double,double&>& rarAsti, CArray<double,double&>& rarAsci, CArray<double,double&>& rarasti, CArray<double,double&>& rarasci,
CArray<double,double&>& rarApti, CArray<double,double&>& rarApci, CArray<double,double&>& rarapti, CArray<double,double&>& rarapci)
{
int i=0;
// Reinforcement.
for(i=0 ; i<arRbarPosi.GetSize(); i++)
{
_DGN_RBAR_CRC rRbar = arRbarPosi.GetAt(i);
_DGN_RBAR_CRC rTnfrC; rTnfrC.Initialize();
rTnfrC = rRbar;
dgap =  (dMu>=0.0 ? dNueC-dCp : dCp-dNueC);
if(bYaxis) rTnfrC.dz = rRbar.dz+dgap;
else       rTnfrC.dy = rRbar.dy+dgap;
rarRbarPosi.Add(rTnfrC);

double dposi = (bYaxis==TRUE ? rTnfrC.dz : rTnfrC.dy);
if(dMu>=0.0)
{
if(dposi>0.0)
{
rarAsci.Add(rTnfrC.dArea);
double asci = dNueC-dposi;
rarasci.Add(asci);
}
else if(dposi<0.0)
{
rarAsti.Add(rTnfrC.dArea);
double asti = (dh-dNueC-fabs(dposi));
rarasti.Add(asti);
}
}
else
{
if(dposi>0.0)
{
rarAsti.Add(rTnfrC.dArea);
double asti = (dh-dNueC-fabs(dposi));
rarasti.Add(asti);
}
else if(dposi<0.0)
{
rarAsci.Add(rTnfrC.dArea);
double asci = dNueC-dposi;
rarasci.Add(asci);
}
}

}

// Tendon.
for(i=0 ; i<arTndnPosi.GetSize(); i++)
{
_DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
_DGN_TNDN_CRC rTnfrC; rTnfrC.Initialize();
rTnfrC = rTndn;
dgap =  (dMu>=0.0 ? dNueC-dCp : dCp-dNueC);
if(bYaxis) rTnfrC.dz = rTndn.dz+dgap;
else       rTnfrC.dy = rTndn.dy+dgap;
rarTndnPosi.Add(rTnfrC);

double dposi = (bYaxis==TRUE ? rTnfrC.dz : rTnfrC.dy);
if(dMu>=0.0)
{
if(dposi>0.0)
{
rarApci.Add(rTnfrC.dArea);
double apci = dNueC-dposi;
rarapci.Add(apci);
}
else if(dposi<0.0)
{
rarApti.Add(rTnfrC.dArea);
double apti = (dh-dNueC-fabs(dposi));
rarapti.Add(apti);
}
}
else
{
if(dposi>0.0)
{
rarApti.Add(rTnfrC.dArea);
double apti = (dh-dNueC-fabs(dposi));
rarapti.Add(apti);
}
else if(dposi<0.0)
{
rarApci.Add(rTnfrC.dArea);
double apci = dNueC-dposi;
rarapci.Add(apci);
}
}
}

return TRUE;
}
*/
