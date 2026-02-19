// DgnPlateGirder.cpp: implementation of the CDgnPlateGirder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "..\wg_base\wg_base_I_PolyMaker.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\PlateGirderDesign.h"
#include "..\wg_db\SectDB.h"
#include "..\wg_db\MembCtrl.h"
#include "..\wg_db\PathUtil.h"

// for DgnEngine
#include "..\..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"

#include "..\..\dgnengine\idesign\DGN_lib\DGNCompare.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnCodeDef.h"
#include "..\..\dgnengine\idesign\DGN_lib\NationalAnnexInterface.h"

#include "..\..\dgnengine\idesign\DGN_lib\Macro.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeISymY.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeBoxSymY.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeIUnsymY.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeBoxUnsymY.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeTubUnsymY.h"
#include "..\..\dgnengine\idesign\DGN_lib\Align2D.h"

#include "..\..\dgnengine\idesign\DGN_link\SectionBuilder.h"

#include "..\..\dgnengine\idesign\DGN_def\DgnTypeEnumDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MemberInfoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MaterialDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"
#include "..\..\dgnengine\idesign\DGN_def\EffectiveLengthDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SteelGeneralSectParamDef.h"
#include "..\..\dgnengine\idesign\DGN_def\BendingCoefficientDef.h"
#include "..\..\dgnengine\idesign\DGN_def\ShearCoefficientDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SafetyFactorDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SeismicPropertyDef.h"
#include "..\..\dgnengine\idesign\DGN_def\UnbracedLengthDef.h"
#include "..\..\dgnengine\idesign\DGN_def\LcomInfoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\Force1DDef.h"
#include "..\..\dgnengine\idesign\DGN_def\TResult.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnItemUtil.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnItemCompoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnCodeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MembTypeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnRptCriticalInfoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionShapeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\PartialFactorDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MembLengthDef.h"
#include "..\..\dgnengine\idesign\DGN_def\ForceUblnDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SteelCompositeSectionDef.h"
#include "..\..\dgnengine\idesign\DGN_def\TSectionShape.h"
#include "..\..\dgnengine\idesign\DGN_def\MaterialConcreteDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MaterialSteelHybridDef.h"
#include "..\..\dgnengine\idesign\DGN_link\IDgnDocCtrl.h"
#include "..\..\dgnengine\idesign\DGN_link\IDgnDoc.h"
#include "..\..\dgnengine\idesign\DGN_link\CalcType.h"
#include "..\..\dgnengine\idesign\DGN_link\IDgnQuery.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnItemDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DesignCaseDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnReportInfoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\Force1DArrayDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MaterialSteelDef.h"
#include "..\..\dgnengine\idesign\DGN_def\LoadApplicationTypeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\StressCheckDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MomentLoadType.h"
#include "..\..\dgnengine\idesign\DGN_def\TorsionDesignDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnSteelOptionsDef.h"
#include "..\..\dgnengine\idesign\DGN_def\LateralTorsionalBucklingDef.h"
#include "..\..\dgnengine\idesign\DGN_def\IShearConnectorSingle.h"
#include "..\..\dgnengine\idesign\DGN_def\ShearConnectorSingleStud.h"
#include "..\..\dgnengine\idesign\DGN_def\ShearConnectorDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MainRebarSingle.h"
#include "..\..\dgnengine\idesign\DGN_def\RebarBaseDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionRebarCoordDef.h"
#include "..\..\dgnengine\idesign\DGN_lib\DgnThreadPool.h"
#include "..\..\dgnengine\idesign\DGN_def\TStiffenerSingle.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeRectangle.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeT.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeTubSymY.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeURib.h"
#include "..\..\dgnengine\idesign\DGN_def\StiffenerRectangle.h"
#include "..\..\dgnengine\idesign\DGN_def\StiffenerT.h"
#include "..\..\dgnengine\idesign\DGN_def\StiffenerURib.h"
#include "..\..\dgnengine\idesign\DGN_def\LongitudinalStiffenerDef.h"
#include "..\..\dgnengine\idesign\DGN_def\TransverseStiffenerDef.h"
#include "..\..\dgnengine\idesign\DGN_def\Force1DMemberDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DamageEquivalenceFactorDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DamageEquivalenceFactorMembDef.h"
#include "..\..\dgnengine\idesign\DGN_def\StressLimitationFactorDef.h"
#include "..\..\dgnengine\idesign\DGN_def\StressSteelCompositeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\ConstrainStressCompositeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MemoizationKeyDef.h"
#include "..\..\dgnengine\idesign\DGN_link\IDgnMemoizationLib.h"
#include "..\..\dgnengine\idesign\DGN_def\ImperfectionFactorDef.h"
#include "..\..\dgnengine\idesign\DGN_lib\IDgnBaseInternalLib.h"
#include "..\..\dgnengine\idesign\DGN_def\DesignCodeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\CompositeSectionPropertyDef.h"

#include "DgnCodeTool.h"
#include "DgnForceCtrl.h"
#include "DgnTypeConverter.h"
#include "DgnPlateGirder.h"
#include "CRCForceCtrl.h"
#include "DgnReportMaker.h"
#include "NationalAnnexTool.h"
#include <chrono>
using namespace dgn::lib;
using namespace dgn::def;
using namespace dgn::link;

#if _MSC_VER >= 1700  // vc++11.0 //sjhuh-2014-02-21
using namespace std;
#include "..\MIT_Lib\rptostream.h"	  // for rptwofstream
#else
#include "..\MIT_Lib\rptostream.h"	// for rptwofstream
#endif
#include "DgnCodeCtrl.h"
#include "DgnDataCtrl.h"
#include "..\wg_db\FileCtrl.h"
#include "DgnProgressDlg.h"
#include "..\wg_dbLock\LockMgr.h"

std::mutex g_num_mutex;

// CDgnPlateGirder
//////////////////////////////////////////////////////////////////////

CDgnPlateGirder::CDgnPlateGirder() : CDgnPscCommon(), IDgnDataBase(), IDgnCodeCheck((IDgnDataBase*)this)
{
	//m_pDoc = CDBDoc::GetDocPoint();
	//ASSERT(m_pDoc);
	m_dZero = 1.0E-07;

	CString csDesignOutput = CProduct::GetTestEnvValue(_ULS(Design Output));
	m_bTestMode = csDesignOutput == _ULS(yes) ? TRUE : FALSE;
	m_dPi = 4.0*atan(1.0);

	InitPartialFactor();

	m_bPrintMode = FALSE;

	m_pAnalysisResult = m_pDoc->m_pPostCtrl->GetAnalysisResult();

	m_CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(m_CpgdD);

	m_DgnLcomNo = 0;

	//T_STCT_D StctD; StctD.Initialize();
	m_pDoc->m_pAttrCtrl->GetStct(m_StctD);

	m_pDgnSectUtil = new CDgnSectUtil();

	m_iCivilCode = m_CpgdD.iDgnCode;
	m_nNatinoAnnex = m_CpgdD.nSubCode;
	m_nIJ = 0;

}

CDgnPlateGirder::~CDgnPlateGirder()
{
	if ( m_pDgnSectUtil != nullptr ) { delete m_pDgnSectUtil; }
}

///////////////////////////////////////////
// For Connection to CRCDataCtrl.
void CDgnPlateGirder::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
{
	CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);
	// Set Code Unit.
	T_UNIT_INDEX CodeUnit = GetCodeUnit();
	m_pDataCtrl->Set_UnitIndex(CodeUnit);
	//m_pDataCtrl->Get_CurrUnitIndex(CodeUnit);	
	m_CodeUnit = CodeUnit;

	Make_PlateGirderDgnData();
}

void CDgnPlateGirder::InitPartialFactor()
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

	m_dGamma_C = CpgdD.dGamma_C;
	m_dGamma_S = CpgdD.dGamma_S;
	m_dGamma_M0 = CpgdD.dGamma_M0;
	m_dGamma_M1 = CpgdD.dGamma_M1;
	m_dGamma_V = CpgdD.dGamma_V;
	m_dGamma_Ff = CpgdD.dGamma_Ff;
	m_dGamma_Mf = CpgdD.dGamma_Mf;
	m_dGamma_Mfs = CpgdD.dGamma_Mfs;
	m_dGamma_M_ser = CpgdD.dGamma_M_ser;

	m_dAlpha = 0.85;
}

UINT CDgnPlateGirder::GetIDgnCheckPosition(const std::thread::id& id) const
{
	if ( IsPrintMode() )
	{
		return GetCheckPosition();
	}

	auto itr = m_ElemToLcom.find(id);
	if ( itr == m_ElemToLcom.end() )
	{
		ASSERT(0);//개망인데
		return 0U;
	}
	return std::get<1>(itr->second);
}

UINT CDgnPlateGirder::GetIDgnLcomNo(const std::thread::id& id) const
{
	if ( IsPrintMode() )
	{
		return GetDgnLcomNo();
	}

	auto itr = m_ElemToLcom.find(id);
	if ( itr == m_ElemToLcom.end() )
	{
		ASSERT(0);//개망인데
		return 0U;
	}
	return std::get<2>(itr->second);
}

ElemPairK CDgnPlateGirder::GetIDgnCheckElem(const std::thread::id& id) const
{
	if ( IsPrintMode() )
	{
		return GetCheckElemPairK();
	}

	auto itr = m_ElemToLcom.find(id);
	if ( itr == m_ElemToLcom.end() )
	{
		ASSERT(0);//개망인데
		return {0u, 0U};
	}
	return std::get<0>(itr->second);
}

bool CDgnPlateGirder::InitializeRpt()
{
	//초기에 불러줘야 하는 함수들.
	m_pDataCtrl->MakePlateGirderDesign4Report();

	CArray<T_CGRE_K, T_CGRE_K> aKeyElem;
	m_pDoc->m_pAttrCtrl2->GetCgreKeyList(aKeyElem);

	CArray<T_CGRV_K, T_CGRV_K> aKeyVBeam;
	m_pDoc->m_pAttrCtrl2->GetCgrvKeyList(aKeyVBeam);

	ArrElemPairKey aRptElemPairK;
	CDBLib::ConvertToElemPairKeyList(aKeyElem, aKeyVBeam, aRptElemPairK);

	int nRptElemSize = aRptElemPairK.GetSize();
	if ( nRptElemSize == 0 )
	{
		ASSERT(0);
		return true;
	}

	T_PLATE_GIRDER_PARAM_D ParamD;
	{
		T_CPGD_D CpgdD;
		if ( !m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD) ) { ASSERT(0); }

		for ( int i = 0; i < 6; i++ )
		{
			ParamD.bCheckStrength[i] = CpgdD.bStrength[i];
		}
		for ( int i = 0; i < 2; i++ )
		{
			ParamD.bCheckService[i] = CpgdD.bService[i];
		}

		ParamD.dt_Ld = CpgdD.dt_Ld;
		ParamD.dk1 = CpgdD.dk1;
		ParamD.dk2 = CpgdD.dk2;
		ParamD.dk3 = CpgdD.dk3;
		ParamD.dks = CpgdD.dks;
		ParamD.bIsIDesignModule = CDgnCodeCtrl::IsIDesignModule(m_iCivilCode);
		ParamD.iLcomNum = m_pForcCtrl->Get_LcomDataCount();
	}

	for ( int i = 0; i<nRptElemSize; ++i )
	{		//결과 구조체에 빈 사이즈 미리 잡아주기도 하고, 멀티쓰레딩을 하기 위해 부재력을 map에 올린다.	{

		auto ElemK = aRptElemPairK[i];		
		int nFortype = 0;
		_DGN_FORC_CRC_ELEM ForceLcom;
		if ( !m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, ForceLcom.aForceLcom, ParamD.iLcomNum) ) { ASSERT(0); continue; }
		m_mForce[ElemK] = std::move(ForceLcom);

		_DGN_STRS_CRC_ELEM StressLcom;
		ADGNSTRESS aStressTnpr;
		if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, nFortype, TRUE, FALSE, StressLcom.aStressLcom, aStressTnpr, ParamD.iLcomNum) ) { return false; }
		m_mStress[ElemK] = std::move(StressLcom);

		nFortype = 2;
		_DGN_STRS_CRC_ELEM StressLcomP2;
		ADGNSTRESS aStressTnprP2;
		if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, nFortype, TRUE, FALSE, StressLcomP2.aStressLcom, aStressTnprP2, ParamD.iLcomNum) ) { return false; }
		m_mStressP2[ElemK] = std::move(StressLcomP2);
				
		ElemPairK showElemK = { 0,0 }, IendElemK = { 0,0 }, JendElemK = { 0,0 };
		GetEndElemOfMember(ElemK, showElemK, IendElemK, JendElemK);

		ForceLcom.Initialize();
		if ( m_pForcCtrl->Get_LcomDataForDesign(IendElemK, nFortype, TRUE, FALSE, ForceLcom.aForceLcom, ParamD.iLcomNum) )
		{ 
			m_mForce[IendElemK] = std::move(ForceLcom);
		}
		else ASSERT(0);
			
		ForceLcom.Initialize();
		if ( m_pForcCtrl->Get_LcomDataForDesign(JendElemK, nFortype, TRUE, FALSE, ForceLcom.aForceLcom, ParamD.iLcomNum) )
		{
			m_mForce[JendElemK] = std::move(ForceLcom);
		}
		else ASSERT(0);

		_DGN_SCFP_PART_ELEM ScfpCr, ScfpSh, ScfpTe, ScfpRe;
		if ( !m_pForcCtrl->Get_LcomStressScfp(ElemK, ParamD.iLcomNum, ScfpCr.aScfpStress, ScfpSh.aScfpStress, ScfpTe.aScfpStress, ScfpRe.aScfpStress) ) return FALSE;  // Get Internal Stress
		m_mScfpCr[ElemK] = ScfpCr;
		m_mScfpSh[ElemK] = ScfpSh;
		m_mScfpTe[ElemK] = ScfpTe;

		T_PLATE_GIRDER_RES_D* pRes = new T_PLATE_GIRDER_RES_D;
		int nSize = ForceLcom.aForceLcom.GetSize();
		pRes->svLcomRes.resize(nSize);
		m_svRes[ElemK] = pRes;

		for ( int j = 0; j<nSize; ++j )
		{
			CPG_FORC_STAG ForceStage;
			ForceStage.Initialize();
			T_STCT_D StctD;
			m_pDoc->m_pAttrCtrl->GetStct(StctD);
			//Girder Force
			T_STAG_K nGdLastStagK = 0;
			int nLastStepNo = 0;
			m_pForcCtrl->GetGirderLastStageKey(ElemK, nGdLastStagK, nLastStepNo);
			int iDgnLcomNo = j+1;
			if ( !m_pDataCtrl->Get_GirderCSLoadOfLCOM(StctD, ElemK, nGdLastStagK, nLastStepNo, iDgnLcomNo, ForceStage) ) { ASSERT(0); }
			m_mStageForce[std::make_pair(ElemK, iDgnLcomNo)] = ForceStage;
		}
	}

	return true;
}

bool CDgnPlateGirder::FinalizeRpt()
{
	CArray<T_CGRE_K, T_CGRE_K> aKeyElem;
	m_pDoc->m_pAttrCtrl2->GetCgreKeyList(aKeyElem);

	CArray<T_CGRV_K, T_CGRV_K> aKeyVBeam;
	m_pDoc->m_pAttrCtrl2->GetCgrvKeyList(aKeyVBeam);

	ArrElemPairKey aRptElemPairK;
	CDBLib::ConvertToElemPairKeyList(aKeyElem, aKeyVBeam, aRptElemPairK);

	int nRptElemSize = aRptElemPairK.GetSize();
	if ( nRptElemSize == 0 )
	{
		ASSERT(0);
		return true;
	}

	for ( int i = 0; i< aRptElemPairK.GetSize(); ++i )
	{
		auto itr = m_svRes.find(aRptElemPairK[i]);
		if ( itr == m_svRes.end() )
		{
			ASSERT(0); continue;
		}
		_SAFE_DELETE(itr->second);
	}

	return true;
}

BOOL CDgnPlateGirder::Make_ClassSection(ElemPairK ElemK, T_PGBS_D& PgbsD, T_PGBR_LCOM& PgbrLcomD)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	//+++++++++++++++++++++++++++++++++++++++
	// CurrUnit -> CodeUnit.
	//  m_pDataCtrl->Get_BendingResistanceForPlateGirderCurrToCode(PgbrLcomD);
	//  m_pDataCtrl->Get_BeamStressResistanceForPlateGirderCurrToCode(PgbsD);
	//+++++++++++++++++++++++++++++++++++++++
	BOOL bCalcOK = TRUE;
	BOOL bRebar = FALSE;
	BOOL bShearConnector = FALSE;
	CPG_MATL_ELEM MatlElem;
	if ( !Get_CpgMatlElem(ElemK, MatlElem) )
	{
		// CodeUnit. -> CurrUnit
		//m_pDataCtrl->Get_BendingResistanceForPlateGirderCodeToCurr(PgbrLcomD);
		return FALSE;
	}
	CPG_SECT_ELEM SectElem;
	if ( !Get_CpgSectElem(ElemK, SectElem) )
	{
		// CodeUnit. -> CurrUnit
		//m_pDataCtrl->Get_BendingResistanceForPlateGirderCodeToCurr(PgbrLcomD);
		return FALSE;
	}
	CPG_RBAR_ELEM RbarElem;
	if ( Get_CpgRbarElem(ElemK, RbarElem) ) bRebar = TRUE;

	CPG_SCON_ELEM SconElem;
	if ( Get_CpgSconElem(ElemK, SconElem) ) bShearConnector = TRUE;


	for ( int i = 0; i < 2; i++ )
	{
		BOOL bPosiI = (i == 0 ? TRUE : FALSE);
		T_COMF_KEY ComfKey;
		ComfKey.DgnLcomK = PgbrLcomD.PgbrBase[i].LcomK;
		ComfKey.ElemK = ElemK.first;
		ComfKey.nPos = i;
		//
		T_CLAS_BASE ClassD;
		ClassD.Initialize();

		double aForce[6] = { 0.0,0.0,0.0,0.0,0.0,0.0 };
		double dStress[4];
		aForce[0] = PgbrLcomD.PgbrBase[i].dFx;
		aForce[4] = PgbrLcomD.PgbrBase[i].dMy;
		aForce[5] = PgbrLcomD.PgbrBase[i].dMz;
		double dMa_Ed = PgbrLcomD.PgbrBase[i].dMa_Ed;
		//aForce[3] = PgbrLcomD.PgbrBase[i].dMa_Ed;
		//
		for ( int j = 0; j < 4; j++ )
			dStress[j] = PgbsD.PgbsBase[i].ChkGirder.dStress[j];

		// Longitudinal Stiffener  

		// RBar
		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);

		if ( PgbrLcomD.PgbrBase[i].bChk == FALSE ) continue;

		T_GLINE_DGN_D GLineDgn = SectElem.GLineDgn[i];
		T_SECT_SECTBASE_D SectBaseD;
		T_GENL_DATA GLineData;
		Get_GenLineDataFromGLineDgn(GLineDgn, GLineData);
	}
	//+++++++++++++++++++++++++++++++++++++++
	// CodeUnit -> CurrUnit.
	//  m_pDataCtrl->Get_BendingResistanceForPlateGirderCodeToCurr(PgbrLcomD);
	//  m_pDataCtrl->Get_BeamStressResistanceForPlateGirderCodeToCurr(PgbsD);
	//+++++++++++++++++++++++++++++++++++++++

	return TRUE;
}

BOOL CDgnPlateGirder::Make_ClassInD(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D& ClassInD)
{
	switch ( m_pDgnSectUtil->Get_RealSectType(&rSectD.SectD) )
	{
	case D_SECT_TYPE_COMPO_I: {Make_ClassInD_I(bPosiI, aGirderForce, aCptForce, rSectD, ClassInD); break; }
	case D_SECT_TYPE_COMPO_B: {Make_ClassInD_Box(bPosiI, aGirderForce, aCptForce, rSectD, ClassInD); break; }
	case D_SECT_TYPE_COMPO_TUB: {Make_ClassInD_Tub(bPosiI, aGirderForce, aCptForce, rSectD, ClassInD); break; }
	default: {ASSERT(0);	 return FALSE; }
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Make_ClassInD_I(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D& ClassInD)
{
	// Get Force
	double dFx_a = aGirderForce[0], dMy_a = aGirderForce[4], dMz_a = aGirderForce[5];
	double dFx_c = aCptForce[0], dMy_c = aCptForce[4], dMz_c = aCptForce[5];

	const _DGN_FORC_CRC& ScfpForce = GetScfpForce4Lcom();
	int nPosId = bPosiI ? 0 : 1;
	double dFx_sc = ScfpForce.dFxx[nPosId];
	double dMy_sc = ScfpForce.dMuy[nPosId];
	double dMz_sc = ScfpForce.dMuz[nPosId];
	dFx_c -= dFx_sc; // 구속응력은 밑에서 보간해서 더해주기 때문에 부재력은 빼자
	dMy_c -= dMy_sc;
	dMz_c -= dMz_sc;

	const _DGN_SCFP_PART_LCASE& ScfpStress = GetScfpStress4Lcom();

	// Get Stiffness
	T_SECT_STIFFNESS GirderStiff, PosiStiffD, NegaStiffD;
	Get_GirderStiffnessofCompo(bPosiI, rSectD.SectD, GirderStiff);
	GetNonComSectStiffD(rSectD, bPosiI, NegaStiffD);
	GetRebarSectStiffD(rSectD, bPosiI, PosiStiffD);
	const T_SECT_STIFFNESS& SectStiffD = dMy_c > 0.0 ? PosiStiffD : NegaStiffD;

	double dA_a = GirderStiff.Area, dIyy_a = GirderStiff.Ryy, dIzz_a = GirderStiff.Rzz;
	double dA_c = SectStiffD.Area, dIyy_c = SectStiffD.Ryy, dIzz_c = SectStiffD.Rzz;

	//
	ClassInD.Initialize();

	// 0 : Outer, 1 : Inner
	double dy_a1 = 0.0, dz_a1 = 0.0, dy_a2 = 0.0, dz_a2 = 0.0; // Girder
	double dy_c1 = 0.0, dz_c1 = 0.0, dy_c2 = 0.0, dz_c2 = 0.0; // Composite
	double dc = 0.0, dt = 0.0;
	double dStress_Out = 0.0, dStress_In = 0.0;

	// Make Top Flange
	// Top Left
	ClassInD.aTopFlangeD.SetSize(2);
	double dbft_l = m_pDgnSectUtil->Get_Size_bft_l(&rSectD.SectD, bPosiI);
	double dtw = m_pDgnSectUtil->Get_Size_tw(&rSectD.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dtft = m_pDgnSectUtil->Get_Size_tft(&rSectD.SectD, bPosiI);
	dy_a1 = GirderStiff.dx1; dy_a2 = GirderStiff.dx1 + dbft_l - 0.5*dtw;
	dz_a1 = GirderStiff.dy1; dz_a2 = GirderStiff.dy1;
	dy_c1 = SectStiffD.dx1; dy_c2 = SectStiffD.dx1 + dbft_l - 0.5*dtw;
	dz_c1 = SectStiffD.dy1; dz_c2 = SectStiffD.dy1;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	double df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	double df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;
	dc = fabs(dy_a1 - dy_a2);
	dt = dtft;
	ClassInD.aTopFlangeD[0].nType = 0;
	ClassInD.aTopFlangeD[0].dc = dc;
	ClassInD.aTopFlangeD[0].dt = dt;
	ClassInD.aTopFlangeD[0].dStressOuter = dStress_Out;
	ClassInD.aTopFlangeD[0].dStressInter = dStress_In;

	// Top Right
	double dbft_r = m_pDgnSectUtil->Get_Size_bft_r(&rSectD.SectD, bPosiI);
	dy_a1 = GirderStiff.dx2; dy_a2 = GirderStiff.dx2 - dbft_r + 0.5*dtw;
	dz_a1 = GirderStiff.dy2; dz_a2 = GirderStiff.dy2;
	dy_c1 = SectStiffD.dx2; dy_c2 = SectStiffD.dx2 - dbft_r + 0.5*dtw;
	dz_c1 = SectStiffD.dy2; dz_c2 = SectStiffD.dy2;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = fabs(dy_a1 - dy_a2);
	dt = dtft;
	ClassInD.aTopFlangeD[1].nType = 0;
	ClassInD.aTopFlangeD[1].dc = dc;
	ClassInD.aTopFlangeD[1].dt = dt;
	ClassInD.aTopFlangeD[1].dStressOuter = dStress_Out;
	ClassInD.aTopFlangeD[1].dStressInter = dStress_In;

	// Make Bottom Flange
	// Bottom Right
	ClassInD.aBotFlangeD.SetSize(2);
	double dbfb_r = m_pDgnSectUtil->Get_Size_bfb_r(&rSectD.SectD, bPosiI);
	double dtfb = m_pDgnSectUtil->Get_Size_tfb(&rSectD.SectD, bPosiI);
	dy_a1 = GirderStiff.dx3; dy_a2 = GirderStiff.dx3 - dbfb_r + 0.5*dtw;
	dz_a1 = GirderStiff.dy3; dz_a2 = GirderStiff.dy3;
	dy_c1 = SectStiffD.dx3; dy_c2 = SectStiffD.dx3 - dbfb_r + 0.5*dtw;
	dz_c1 = SectStiffD.dy3; dz_c2 = SectStiffD.dy3;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;
	
	dc = fabs(dy_a1 - dy_a2);
	dt = dtfb;
	ClassInD.aBotFlangeD[0].nType = 0;
	ClassInD.aBotFlangeD[0].dc = dc;
	ClassInD.aBotFlangeD[0].dt = dt;
	ClassInD.aBotFlangeD[0].dStressOuter = dStress_Out;
	ClassInD.aBotFlangeD[0].dStressInter = dStress_In;

	// Bottom Left
	double dbfb_l = m_pDgnSectUtil->Get_Size_bfb_l(&rSectD.SectD, bPosiI);
	dy_a1 = GirderStiff.dx4; dy_a2 = GirderStiff.dx4 + dbfb_l - 0.5*dtw;
	dz_a1 = GirderStiff.dy4; dz_a2 = GirderStiff.dy4;
	dy_c1 = SectStiffD.dx4; dy_c2 = SectStiffD.dx4 + dbfb_l - 0.5*dtw;
	dz_c1 = SectStiffD.dy4; dz_c2 = SectStiffD.dy4;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = fabs(dy_a1 - dy_a2);
	dt = dtfb;
	ClassInD.aBotFlangeD[1].nType = 0;
	ClassInD.aBotFlangeD[1].dc = dc;
	ClassInD.aBotFlangeD[1].dt = dt;
	ClassInD.aBotFlangeD[1].dStressOuter = dStress_Out;
	ClassInD.aBotFlangeD[1].dStressInter = dStress_In;

	// Make Web
	ClassInD.aWebD.SetSize(1);
	dy_a1 = GirderStiff.dx1 + dbft_l;	dy_a2 = GirderStiff.dx4 + dbfb_l;
	dz_a1 = GirderStiff.dy1 - dtft;		dz_a2 = GirderStiff.dy4 + dtfb;
	dy_c1 = SectStiffD.dx1 + dbft_l;	dy_c2 = SectStiffD.dx4 + dbfb_l;
	dz_c1 = SectStiffD.dy1 - dtft;		dz_c2 = SectStiffD.dy4 + dtfb;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = fabs(dz_a1 - dz_a2);
	dt = dtw;
	ClassInD.aWebD[0].nType = 1;
	ClassInD.aWebD[0].dc = dc;
	ClassInD.aWebD[0].dt = dt;
	ClassInD.aWebD[0].dStressOuter = dStress_Out;
	ClassInD.aWebD[0].dStressInter = dStress_In;

	return TRUE;
}

BOOL CDgnPlateGirder::Make_ClassInD_Box(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D& ClassInD)
{
	ClassInD.Initialize();

	// Get Force
	double dFx_a = aGirderForce[0], dMy_a = aGirderForce[4], dMz_a = aGirderForce[5];
	double dFx_c = aCptForce[0], dMy_c = aCptForce[4], dMz_c = aCptForce[5];

	const _DGN_FORC_CRC& ScfpForce = GetScfpForce4Lcom();
	int nPosId = bPosiI ? 0 : 1;
	double dFx_sc = ScfpForce.dFxx[nPosId];
	double dMy_sc = ScfpForce.dMuy[nPosId];
	double dMz_sc = ScfpForce.dMuz[nPosId];
	dFx_c -= dFx_sc;
	dMy_c -= dMy_sc;
	dMz_c -= dMz_sc;

	// Get Stiffness
	T_SECT_STIFFNESS GirderStiff, PosiStiffD, NegaStiffD;
	Get_GirderStiffnessofCompo(bPosiI, rSectD.SectD, GirderStiff);
	GetNonComSectStiffD(rSectD, bPosiI, NegaStiffD);
	GetRebarSectStiffD(rSectD, bPosiI, PosiStiffD);
	const T_SECT_STIFFNESS& SectStiffD = dMy_c > 0.0 ? PosiStiffD : NegaStiffD;

	double dA_a = GirderStiff.Area, dIyy_a = GirderStiff.Ryy, dIzz_a = GirderStiff.Rzz;
	double dA_c = SectStiffD.Area, dIyy_c = SectStiffD.Ryy, dIzz_c = SectStiffD.Rzz;

	// 0 : Outer, 1 : Inner
	double dy_a1 = 0.0, dz_a1 = 0.0, dy_a2 = 0.0, dz_a2 = 0.0; // Girder
	double dy_c1 = 0.0, dz_c1 = 0.0, dy_c2 = 0.0, dz_c2 = 0.0; // Composite
	double dc = 0.0, dt = 0.0;
	double dStress_Out = 0.0, dStress_In = 0.0;
	double df1Const = 0.0, df2Const = 0.0;

	// Make Top Flange
	ClassInD.aTopFlangeD.SetSize(3);

	// Top Left
	double dbft_l = m_pDgnSectUtil->Get_Size_bft_l(&rSectD.SectD, bPosiI);
	double dtwl = m_pDgnSectUtil->Get_Size_tw(&rSectD.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dtft = m_pDgnSectUtil->Get_Size_tft(&rSectD.SectD, bPosiI);
	if ( dbft_l > 0.0 )
	{
		dy_a1 = GirderStiff.dx1; dy_a2 = GirderStiff.dx1 + dbft_l - dtwl;
		dz_a1 = GirderStiff.dy1; dz_a2 = GirderStiff.dy1;
		dy_c1 = SectStiffD.dx1; dy_c2 = SectStiffD.dx1 + dbft_l - dtwl;
		dz_c1 = SectStiffD.dy1; dz_c2 = SectStiffD.dy1;
		dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
		dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
		df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
		dStress_Out += df1Const;

		dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
		dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
		df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
		dStress_In += df2Const;

		dc = fabs(dy_a1 - dy_a2);
		dt = dtft;
		ClassInD.aTopFlangeD[0].nType = 0;
		ClassInD.aTopFlangeD[0].dc = dc;
		ClassInD.aTopFlangeD[0].dt = dt;
		ClassInD.aTopFlangeD[0].dStressOuter = dStress_Out;
		ClassInD.aTopFlangeD[0].dStressInter = dStress_In;
	}

	// Top Middle
	double dbft_m = m_pDgnSectUtil->Get_Size_bft_m(&rSectD.SectD, bPosiI);
	dy_a1 = GirderStiff.dx1 + dbft_l;	dy_a2 = dy_a1 + dbft_m;
	dz_a1 = GirderStiff.dy1;			dz_a2 = GirderStiff.dy1;
	dy_c1 = SectStiffD.dx1 + dbft_l;	dy_c2 = dy_c1 + dbft_m;
	dz_c1 = SectStiffD.dy1;				dz_c2 = SectStiffD.dy1;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = fabs(dy_a1 - dy_a2);
	dt = dtft;
	ClassInD.aTopFlangeD[1].nType = 1;
	ClassInD.aTopFlangeD[1].dc = dc;
	ClassInD.aTopFlangeD[1].dt = dt;
	ClassInD.aTopFlangeD[1].dStressOuter = dStress_Out;
	ClassInD.aTopFlangeD[1].dStressInter = dStress_In;

	// Top Right
	double dbft_r = m_pDgnSectUtil->Get_Size_bft_r(&rSectD.SectD, bPosiI);
	double dtwr = m_pDgnSectUtil->Get_Size_tw(&rSectD.SectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
	if ( dbft_r > 0.0 )
	{
		dy_a1 = GirderStiff.dx2; dy_a2 = GirderStiff.dx2 - dbft_r + dtwr;
		dz_a1 = GirderStiff.dy2; dz_a2 = GirderStiff.dy2;
		dy_c1 = SectStiffD.dx2; dy_c2 = SectStiffD.dx2 - dbft_r + dtwr;
		dz_c1 = SectStiffD.dy2; dz_c2 = SectStiffD.dy2;
		dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
		dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
		df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
		dStress_Out += df1Const;

		dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
		dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
		df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
		dStress_In += df2Const;

		dc = fabs(dy_a1 - dy_a2);
		dt = dtft;
		ClassInD.aTopFlangeD[2].nType = 0;
		ClassInD.aTopFlangeD[2].dc = dc;
		ClassInD.aTopFlangeD[2].dt = dt;
		ClassInD.aTopFlangeD[2].dStressOuter = dStress_Out;
		ClassInD.aTopFlangeD[2].dStressInter = dStress_In;
	}

	// Make Bottom Flange
	ClassInD.aBotFlangeD.SetSize(3);

	// Bottom Right
	double dbfb_r = m_pDgnSectUtil->Get_Size_bfb_r(&rSectD.SectD, bPosiI);
	double dtfb = m_pDgnSectUtil->Get_Size_tfb(&rSectD.SectD, bPosiI);
	if ( dbfb_r > 0.0 )
	{
		dy_a1 = GirderStiff.dx3; dy_a2 = GirderStiff.dx3 - dbfb_r + dtwr;
		dz_a1 = GirderStiff.dy3; dz_a2 = GirderStiff.dy3;
		dy_c1 = SectStiffD.dx3; dy_c2 = SectStiffD.dx3 - dbfb_r + dtwr;
		dz_c1 = SectStiffD.dy3; dz_c2 = SectStiffD.dy3;
		dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
		dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
		df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
		dStress_Out += df1Const;

		dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
		dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
		df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
		dStress_In += df2Const;

		dc = fabs(dy_a1 - dy_a2);
		dt = dtfb;
		ClassInD.aBotFlangeD[0].nType = 0;
		ClassInD.aBotFlangeD[0].dc = dc;
		ClassInD.aBotFlangeD[0].dt = dt;
		ClassInD.aBotFlangeD[0].dStressOuter = dStress_Out;
		ClassInD.aBotFlangeD[0].dStressInter = dStress_In;
	}

	// Bottom Middle
	double dbfb_l = m_pDgnSectUtil->Get_Size_bfb_l(&rSectD.SectD, bPosiI);
	double dbfb_m = m_pDgnSectUtil->Get_Size_bfb_m(&rSectD.SectD, bPosiI);
	dy_a1 = GirderStiff.dx4 + dbfb_l;		dy_a2 = dy_a1 + dbfb_m;
	dz_a1 = GirderStiff.dy4;				dz_a2 = GirderStiff.dy4;
	dy_c1 = SectStiffD.dx4 + dbfb_l;		dy_c2 = dy_c1 + dbfb_m;
	dz_c1 = SectStiffD.dy4;					dz_c2 = SectStiffD.dy4;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = fabs(dy_a1 - dy_a2);
	dt = dtfb;
	ClassInD.aBotFlangeD[1].nType = 1;
	ClassInD.aBotFlangeD[1].dc = dc;
	ClassInD.aBotFlangeD[1].dt = dt;
	ClassInD.aBotFlangeD[1].dStressOuter = dStress_Out;
	ClassInD.aBotFlangeD[1].dStressInter = dStress_In;

	// Bottom Left
	if ( dbfb_l > 0.0 )
	{
		dy_a1 = GirderStiff.dx4;	dy_a2 = GirderStiff.dx4 + dbfb_l - dtwl;
		dz_a1 = GirderStiff.dy4;	dz_a2 = GirderStiff.dy4;
		dy_c1 = SectStiffD.dx4;		dy_c2 = SectStiffD.dx4 + dbfb_l - dtwl;
		dz_c1 = SectStiffD.dy4;		dz_c2 = SectStiffD.dy4;
		dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
		dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
		df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
		dStress_Out += df1Const;

		dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
		dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
		df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
		dStress_In += df2Const;

		dc = fabs(dy_a1 - dy_a2);
		dt = dtfb;
		ClassInD.aBotFlangeD[2].nType = 0;
		ClassInD.aBotFlangeD[2].dc = dc;
		ClassInD.aBotFlangeD[2].dt = dt;
		ClassInD.aBotFlangeD[2].dStressOuter = dStress_Out;
		ClassInD.aBotFlangeD[2].dStressInter = dStress_In;
	}

	// Make Web
	ClassInD.aWebD.SetSize(2);

	// Left Web
	dy_a1 = GirderStiff.dx1 + dbft_l - 0.5*dtwl;	dy_a2 = GirderStiff.dx4 + dbfb_l - 0.5*dtwl;
	dz_a1 = GirderStiff.dy1 - dtft;					dz_a2 = GirderStiff.dy4 + dtfb;
	dy_c1 = SectStiffD.dx1 + dbft_l - 0.5*dtwl;		dy_c2 = SectStiffD.dx4 + dbfb_l - 0.5*dtwl;
	dz_c1 = SectStiffD.dy1 - dtft;					dz_c2 = SectStiffD.dy4 + dtfb;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = m_pDgnSectUtil->Get_Size_Inclind_hw(&rSectD.SectD, 0, bPosiI);
	dt = dtwl;
	ClassInD.aWebD[0].nType = 1;
	ClassInD.aWebD[0].dc = dc;
	ClassInD.aWebD[0].dt = dt;
	ClassInD.aWebD[0].dStressOuter = dStress_Out;
	ClassInD.aWebD[0].dStressInter = dStress_In;

	// Right Web
	dy_a1 = GirderStiff.dx2 - dbft_r + 0.5*dtwr;	dy_a2 = GirderStiff.dx2 - dbfb_r + 0.5*dtwr;
	dz_a1 = GirderStiff.dy1 - dtft;					dz_a2 = GirderStiff.dy4 + dtfb;
	dy_c1 = SectStiffD.dx2 - dbft_r + 0.5*dtwr;		dy_c2 = SectStiffD.dx2 - dbfb_r + 0.5*dtwr;
	dz_c1 = SectStiffD.dy1 - dtft;					dz_c2 = SectStiffD.dy4 + dtfb;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = m_pDgnSectUtil->Get_Size_Inclind_hw(&rSectD.SectD, 1, bPosiI);
	dt = dtwr;
	ClassInD.aWebD[1].nType = 1;
	ClassInD.aWebD[1].dc = dc;
	ClassInD.aWebD[1].dt = dt;
	ClassInD.aWebD[1].dStressOuter = dStress_Out;
	ClassInD.aWebD[1].dStressInter = dStress_In;

	return TRUE;
}

BOOL CDgnPlateGirder::Make_ClassInD_Tub(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D& ClassInD)
{
	ClassInD.Initialize();

	// Get Force
	double dFx_a = aGirderForce[0], dMy_a = aGirderForce[4], dMz_a = aGirderForce[5];
	double dFx_c = aCptForce[0], dMy_c = aCptForce[4], dMz_c = aCptForce[5];

	const _DGN_FORC_CRC& ScfpForce = GetScfpForce4Lcom();
	int nPosId = bPosiI ? 0 : 1;
	double dFx_sc = ScfpForce.dFxx[nPosId];
	double dMy_sc = ScfpForce.dMuy[nPosId];
	double dMz_sc = ScfpForce.dMuz[nPosId];
	dFx_c -= dFx_sc;
	dMy_c -= dMy_sc;
	dMz_c -= dMz_sc;

	// Get Stiffness
	T_SECT_STIFFNESS GirderStiff, PosiStiffD, NegaStiffD;
	Get_GirderStiffnessofCompo(bPosiI, rSectD.SectD, GirderStiff);
	GetNonComSectStiffD(rSectD, bPosiI, NegaStiffD);
	GetRebarSectStiffD(rSectD, bPosiI, PosiStiffD);
	const T_SECT_STIFFNESS& SectStiffD = dMy_c > 0.0 ? PosiStiffD : NegaStiffD;

	double dA_a = GirderStiff.Area, dIyy_a = GirderStiff.Ryy, dIzz_a = GirderStiff.Rzz;
	double dA_c = SectStiffD.Area, dIyy_c = SectStiffD.Ryy, dIzz_c = SectStiffD.Rzz;

	// 0 : Outer, 1 : Inner
	double dy_a1 = 0.0, dz_a1 = 0.0, dy_a2 = 0.0, dz_a2 = 0.0; // Girder
	double dy_c1 = 0.0, dz_c1 = 0.0, dy_c2 = 0.0, dz_c2 = 0.0; // Composite
	double dc = 0.0, dt = 0.0;
	double dStress_Out = 0.0, dStress_In = 0.0;
	double df1Const = 0.0, df2Const = 0.0;

	// Make Top Flange
	ClassInD.aTopFlangeD.SetSize(4);

	// Top Left - Left Part
	double dbft_ll = m_pDgnSectUtil->Get_Size_bft_ll(&rSectD.SectD, bPosiI);
	double dtwl = m_pDgnSectUtil->Get_Size_tw(&rSectD.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dtft = m_pDgnSectUtil->Get_Size_tft(&rSectD.SectD, bPosiI);
	dy_a1 = GirderStiff.dx1; dy_a2 = GirderStiff.dx1 + dbft_ll - 0.5*dtwl;
	dz_a1 = GirderStiff.dy1; dz_a2 = GirderStiff.dy1;
	dy_c1 = SectStiffD.dx1; dy_c2 = SectStiffD.dx1 + dbft_ll - 0.5*dtwl;
	dz_c1 = SectStiffD.dy1; dz_c2 = SectStiffD.dy1;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = fabs(dy_a1 - dy_a2);
	dt = dtft;
	ClassInD.aTopFlangeD[0].nType = 0;
	ClassInD.aTopFlangeD[0].dc = dc;
	ClassInD.aTopFlangeD[0].dt = dt;
	ClassInD.aTopFlangeD[0].dStressOuter = dStress_Out;
	ClassInD.aTopFlangeD[0].dStressInter = dStress_In;

	// Top Left - Right Part
	double dbft_l = m_pDgnSectUtil->Get_Size_bft_l(&rSectD.SectD, bPosiI);
	dy_a1 = GirderStiff.dx1 + dbft_ll + 0.5*dtwl; dy_a2 = GirderStiff.dx1 + dbft_l;
	dz_a1 = GirderStiff.dy1; dz_a2 = GirderStiff.dy1;
	dy_c1 = SectStiffD.dx1 + dbft_ll + 0.5*dtwl; dy_c2 = SectStiffD.dx1 + dbft_l;
	dz_c1 = SectStiffD.dy1; dz_c2 = SectStiffD.dy1;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = fabs(dy_a1 - dy_a2);
	dt = dtft;
	ClassInD.aTopFlangeD[1].nType = 0;
	ClassInD.aTopFlangeD[1].dc = dc;
	ClassInD.aTopFlangeD[1].dt = dt;
	ClassInD.aTopFlangeD[1].dStressOuter = dStress_Out;
	ClassInD.aTopFlangeD[1].dStressInter = dStress_In;


	// Top Right - Right Part
	double dbft_rr = m_pDgnSectUtil->Get_Size_bft_rr(&rSectD.SectD, bPosiI);
	double dtwr = m_pDgnSectUtil->Get_Size_tw(&rSectD.SectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
	dy_a1 = GirderStiff.dx2; dy_a2 = GirderStiff.dx2 - dbft_rr + 0.5*dtwr;
	dz_a1 = GirderStiff.dy2; dz_a2 = GirderStiff.dy2;
	dy_c1 = SectStiffD.dx2; dy_c2 = SectStiffD.dx2 - dbft_rr + 0.5*dtwr;
	dz_c1 = SectStiffD.dy2; dz_c2 = SectStiffD.dy2;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = fabs(dy_a1 - dy_a2);
	dt = dtft;
	ClassInD.aTopFlangeD[2].nType = 0;
	ClassInD.aTopFlangeD[2].dc = dc;
	ClassInD.aTopFlangeD[2].dt = dt;
	ClassInD.aTopFlangeD[2].dStressOuter = dStress_Out;
	ClassInD.aTopFlangeD[2].dStressInter = dStress_In;

	// Top Right - Left Part
	double dbft_r = m_pDgnSectUtil->Get_Size_bft_r(&rSectD.SectD, bPosiI);
	dy_a1 = GirderStiff.dx2 - dbft_rr - 0.5*dtwr; dy_a2 = GirderStiff.dx2 - dbft_r;
	dz_a1 = GirderStiff.dy2; dz_a2 = GirderStiff.dy2;
	dy_c1 = SectStiffD.dx2 - dbft_rr - 0.5*dtwr; dy_c2 = SectStiffD.dx2 - dbft_r;
	dz_c1 = SectStiffD.dy2; dz_c2 = SectStiffD.dy2;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = fabs(dy_a1 - dy_a2);
	dt = dtft;
	ClassInD.aTopFlangeD[3].nType = 0;
	ClassInD.aTopFlangeD[3].dc = dc;
	ClassInD.aTopFlangeD[3].dt = dt;
	ClassInD.aTopFlangeD[3].dStressOuter = dStress_Out;
	ClassInD.aTopFlangeD[3].dStressInter = dStress_In;


	// Make Bottom Flange
	ClassInD.aBotFlangeD.SetSize(3);

	// Bottom Right
	double dbfb_r = m_pDgnSectUtil->Get_Size_bfb_r(&rSectD.SectD, bPosiI);
	double dtfb = m_pDgnSectUtil->Get_Size_tfb(&rSectD.SectD, bPosiI);
	if ( dbfb_r > 0.0 )
	{
		dy_a1 = GirderStiff.dx3; dy_a2 = GirderStiff.dx3 - dbfb_r + 0.5*dtwr;
		dz_a1 = GirderStiff.dy3; dz_a2 = GirderStiff.dy3;
		dy_c1 = SectStiffD.dx3; dy_c2 = SectStiffD.dx3 - dbfb_r + 0.5*dtwr;
		dz_c1 = SectStiffD.dy3; dz_c2 = SectStiffD.dy3;
		dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
		dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
		df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
		dStress_Out += df1Const;

		dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
		dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
		df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
		dStress_In += df2Const;

		dc = fabs(dy_a1 - dy_a2);
		dt = dtfb;
		ClassInD.aBotFlangeD[0].nType = 0;
		ClassInD.aBotFlangeD[0].dc = dc;
		ClassInD.aBotFlangeD[0].dt = dt;
		ClassInD.aBotFlangeD[0].dStressOuter = dStress_Out;
		ClassInD.aBotFlangeD[0].dStressInter = dStress_In;
	}

	// Bottom Middle
	double dbfb_l = m_pDgnSectUtil->Get_Size_bfb_l(&rSectD.SectD, bPosiI);
	double dbfb_m = m_pDgnSectUtil->Get_Size_bfb_m(&rSectD.SectD, bPosiI);
	dy_a1 = GirderStiff.dx4 + dbfb_l - 0.5*dtwl;		dy_a2 = dy_a1 + dbfb_m - 0.5*dtwr;
	dz_a1 = GirderStiff.dy4;				            dz_a2 = GirderStiff.dy4;
	dy_c1 = SectStiffD.dx4 + dbfb_l - 0.5*dtwl;		dy_c2 = dy_c1 + dbfb_m - 0.5*dtwr;
	dz_c1 = SectStiffD.dy4;					        dz_c2 = SectStiffD.dy4;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = fabs(dy_a1 - dy_a2);
	dt = dtfb;
	ClassInD.aBotFlangeD[1].nType = 1;
	ClassInD.aBotFlangeD[1].dc = dc;
	ClassInD.aBotFlangeD[1].dt = dt;
	ClassInD.aBotFlangeD[1].dStressOuter = dStress_Out;
	ClassInD.aBotFlangeD[1].dStressInter = dStress_In;

	// Bottom Left
	if ( dbfb_l > 0.0 )
	{
		dy_a1 = GirderStiff.dx4;	dy_a2 = GirderStiff.dx4 + dbfb_l - 0.5*dtwl;
		dz_a1 = GirderStiff.dy4;	dz_a2 = GirderStiff.dy4;
		dy_c1 = SectStiffD.dx4;		dy_c2 = SectStiffD.dx4 + dbfb_l - 0.5*dtwl;
		dz_c1 = SectStiffD.dy4;		dz_c2 = SectStiffD.dy4;
		dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
		dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
		df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
		dStress_Out += df1Const;

		dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
		dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
		df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
		dStress_In += df2Const;

		dc = fabs(dy_a1 - dy_a2);
		dt = dtfb;
		ClassInD.aBotFlangeD[2].nType = 0;
		ClassInD.aBotFlangeD[2].dc = dc;
		ClassInD.aBotFlangeD[2].dt = dt;
		ClassInD.aBotFlangeD[2].dStressOuter = dStress_Out;
		ClassInD.aBotFlangeD[2].dStressInter = dStress_In;
	}

	// Make Web
	ClassInD.aWebD.SetSize(2);

	// Left Web
	dy_a1 = GirderStiff.dx1 + dbft_ll - 0.5*dtwl;	dy_a2 = GirderStiff.dx4 + dbfb_l - 0.5*dtwl;
	dz_a1 = GirderStiff.dy1 - dtft;					dz_a2 = GirderStiff.dy4 + dtfb;
	dy_c1 = SectStiffD.dx1 + dbft_ll - 0.5*dtwl;	dy_c2 = SectStiffD.dx4 + dbfb_l - 0.5*dtwl;
	dz_c1 = SectStiffD.dy1 - dtft;					dz_c2 = SectStiffD.dy4 + dtfb;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = m_pDgnSectUtil->Get_Size_Inclind_hw(&rSectD.SectD, 0, bPosiI);
	dt = dtwl;
	ClassInD.aWebD[0].nType = 1;
	ClassInD.aWebD[0].dc = dc;
	ClassInD.aWebD[0].dt = dt;
	ClassInD.aWebD[0].dStressOuter = dStress_Out;
	ClassInD.aWebD[0].dStressInter = dStress_In;

	// Right Web
	dy_a1 = GirderStiff.dx2 - dbft_rr + 0.5*dtwr;	dy_a2 = GirderStiff.dx2 - dbfb_r + 0.5*dtwr;
	dz_a1 = GirderStiff.dy1 - dtft;					dz_a2 = GirderStiff.dy4 + dtfb;
	dy_c1 = SectStiffD.dx2 - dbft_rr + 0.5*dtwr;	dy_c2 = SectStiffD.dx2 - dbfb_r + 0.5*dtwr;
	dz_c1 = SectStiffD.dy1 - dtft;					dz_c2 = SectStiffD.dy4 + dtfb;
	dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a1, dz_a1);
	dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c1, dz_c1);
	df1Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a1, dz_a1);
	dStress_Out += df1Const;

	dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIzz_a, dy_a2, dz_a2);
	dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIzz_c, dy_c2, dz_c2);
	df2Const = CalcStressInterpolate(nPosId, GirderStiff, dy_a2, dz_a2);
	dStress_In += df2Const;

	dc = m_pDgnSectUtil->Get_Size_Inclind_hw(&rSectD.SectD, 1, bPosiI);
	dt = dtwr;
	ClassInD.aWebD[1].nType = 1;
	ClassInD.aWebD[1].dc = dc;
	ClassInD.aWebD[1].dt = dt;
	ClassInD.aWebD[1].dStressOuter = dStress_Out;
	ClassInD.aWebD[1].dStressInter = dStress_In;

	return TRUE;
}

BOOL CDgnPlateGirder::Get_BendingResistanceForPlateGirder(ElemPairK ElemK, T_PGBR_LCOM& PgbrLcomD)
{
	//+++++++++++++++++++++++++++++++++++++++
	// CurrUnit -> CodeUnit.
	//  m_pDataCtrl->Get_BendingResistanceForPlateGirderCurrToCode(PgbrLcomD);
	//+++++++++++++++++++++++++++++++++++++++

	///////////////////////////////////////////////
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;

	for ( int i = 0; i < 2; i++ )
	{
		if ( !Set_CalcDataByPos(CalcD, i) ) { ASSERT(0); return FALSE; }

		T_COMF_KEY ComfKey;
		ComfKey.DgnLcomK = PgbrLcomD.PgbrBase[i].LcomK;
		ComfKey.ElemK = ElemK.first;
		ComfKey.nPos = CalcD.iPos;

		if ( PgbrLcomD.PgbrBase[i].bChk == FALSE ) continue;
	}
	//+++++++++++++++++++++++++++++++++++++++
	// CodeUnit -> CurrUnit.
	//m_pDataCtrl->Get_BendingResistanceForPlateGirderCodeToCurr(PgbrLcomD);
	//+++++++++++++++++++++++++++++++++++++++  

	return TRUE;
}

BOOL CDgnPlateGirder::Get_VerticalShearResistanceForPlateGirder(ElemPairK ElemK, T_PRVS_D& PrvsD)
{
	//+++++++++++++++++++++++++++++++++++++++
	// CurrUnit -> CodeUnit.
	//m_pDataCtrl->Get_VerticalShearResistanceForPlateGirderCurrToCode(PrvsD);
	//+++++++++++++++++++++++++++++++++++++++

	///////////////////////////////////////////////
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;

	for ( int i = 0; i < 2; i++ )
	{
		if ( !Set_CalcDataByPos(CalcD, i) ) { ASSERT(0); return FALSE; }

		if ( PrvsD.PrvsBase[i].bChk == FALSE ) continue;
		T_PRVS_DETAIL PrvsDetail;
		//if(!Calc_VerticalShearResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, PrvsD.PrvsBase[i], PrvsDetail)) continue;    
	}
	//+++++++++++++++++++++++++++++++++++++++
	// CodeUnit -> CurrUnit.
	//m_pDataCtrl->Get_VerticalShearResistanceForPlateGirderCodeToCurr(PrvsD);
	//+++++++++++++++++++++++++++++++++++++++  

	return TRUE;
}

BOOL CDgnPlateGirder::Get_LateralTorsionalBucklingResistanceForPlateGirder(ElemPairK ElemK, T_PLTB_D& PltbD)
{
	//+++++++++++++++++++++++++++++++++++++++
	// CurrUnit -> CodeUnit.
	//m_pDataCtrl->Get_LateralTorsionalBucklingResistanceForPlateGirderCurrToCode(PltbD);
	//+++++++++++++++++++++++++++++++++++++++
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;

	for ( int i = 0; i < 2; i++ )
	{
		if ( !Set_CalcDataByPos(CalcD, i) ) { ASSERT(0); return FALSE; }

		T_COMF_KEY ComfKey;
		ComfKey.DgnLcomK = PltbD.PltbBase[i].LcomK;
		ComfKey.ElemK = ElemK.first;
		ComfKey.nPos = CalcD.iPos;

		if ( PltbD.PltbBase[i].bChk == FALSE ) continue;
		T_PLTB_DETAIL PltbDetail;
		if ( !Calc_LateralTorsionalBucklingResistance(CalcD, PltbD.PltbBase[i], PltbDetail) ) continue;
	}
	//+++++++++++++++++++++++++++++++++++++++
	// CodeUnit -> CurrUnit
	//m_pDataCtrl->Get_LateralTorsionalBucklingResistanceForPlateGirderCodeToCurr(PltbD);
	//+++++++++++++++++++++++++++++++++++++++  

	return TRUE;
}

BOOL CDgnPlateGirder::Get_TransverseForceResistanceForPlateGirder(ElemPairK ElemK, T_PRTF_D& PrtfD)
{
	//+++++++++++++++++++++++++++++++++++++++
	// CurrUnit -> CodeUnit.
	//m_pDataCtrl->Get_TransverseForceResistanceForPlateGirderCurrToCode(PrtfD);
	//+++++++++++++++++++++++++++++++++++++++
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;

	for ( int i = 0; i < 2; i++ )
	{
		if ( !Set_CalcDataByPos(CalcD, i) ) { ASSERT(0); return FALSE; }

		if ( PrtfD.PrtfBase[i].bChk == FALSE ) continue;
		if ( !CalcD.bChkData[PGCHK_TYPE::BUCK_COEFF] || (CalcD.bChkData[PGCHK_TYPE::BUCK_COEFF] && !CalcD.BcdtPosi.bUseTfType) )
		{
			PrtfD.PrtfBase[i].bChk = FALSE;
			continue;
		}
		T_PRTF_DETAIL PrtfDetail;
		if ( !Calc_TransverseForceResistance(CalcD, PrtfD.PrtfBase[i], PrtfDetail) ) continue;
	}
	//+++++++++++++++++++++++++++++++++++++++
	// CodeUnit -> CurrUnit.
	//m_pDataCtrl->Get_TransverseForceResistanceForPlateGirderCodeToCurr(PrtfD);
	//+++++++++++++++++++++++++++++++++++++++  

	return TRUE;
}

BOOL CDgnPlateGirder::Get_LongitudinalShearResistanceForPlateGirder(ElemPairK ElemK, T_PGLS_D& PglsD)
{
	//+++++++++++++++++++++++++++++++++++++++
	// CurrUnit -> CodeUnit.
	//m_pDataCtrl->Get_LongitudinalShearResistanceForPlateGirderCurrToCode(PglsD);
	//+++++++++++++++++++++++++++++++++++++++
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;

	for ( int i = 0; i < 2; i++ )
	{
		if ( !Set_CalcDataByPos(CalcD, i) ) { ASSERT(0); return FALSE; }

		T_COMF_KEY ComfKey;
		ComfKey.DgnLcomK = PglsD.PglsBase[i].LcomK;
		ComfKey.ElemK = ElemK.first;
		ComfKey.nPos = CalcD.iPos;

		if ( PglsD.PglsBase[i].bChk == FALSE ) continue;
		if ( !CalcD.bChkData[PGCHK_TYPE::SHEAR_CONNECTOR] || (CalcD.bChkData[PGCHK_TYPE::SHEAR_CONNECTOR] && !CalcD.SconPosi.bUseShear) )
		{
			PglsD.PglsBase[i].bChk = FALSE;
			continue;
		}
	}
	//+++++++++++++++++++++++++++++++++++++++
	// CodeUnit -> CurrUnit.
	//m_pDataCtrl->Get_LongitudinalShearResistanceForPlateGirderCodeToCurr(PglsD);
	//+++++++++++++++++++++++++++++++++++++++  

	return TRUE;
}

BOOL CDgnPlateGirder::Get_FatigueResistanceForPlateGirder(ElemPairK ElemK, T_PGRF_D& PgrfD, double dt_Ld)
{
	//+++++++++++++++++++++++++++++++++++++++
	// CurrUnit -> CodeUnit.
	//m_pDataCtrl->Get_FatigueResistanceForPlateGirderCurrToCode(PgrfD);
	//+++++++++++++++++++++++++++++++++++++++
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;

	for ( int i = 0; i < 2; i++ )
	{
		if ( !Set_CalcDataByPos(CalcD, i) ) { ASSERT(0); return FALSE; }

		if ( PgrfD.PgrfBase[i].bChk == FALSE ) continue;
		if ( !CalcD.bChkData[PGCHK_TYPE::SHEAR_CONNECTOR] || (CalcD.bChkData[PGCHK_TYPE::SHEAR_CONNECTOR] && !CalcD.SconPosi.bUseShear) )
		{
			PgrfD.PgrfBase[i].bChk = FALSE;
			continue;
		}
		if ( !Calc_FatigueResistance(CalcD, dt_Ld, PgrfD.PgrfBase[i]) ) continue;
	}
	//+++++++++++++++++++++++++++++++++++++++
	// CodeUnit -> CurrUnit.
	//m_pDataCtrl->Get_FatigueResistanceForPlateGirderCodeToCurr(PgrfD);
	//+++++++++++++++++++++++++++++++++++++++  

	return TRUE;
}

BOOL CDgnPlateGirder::Get_BeamStressResistanceForPlateGirder(ElemPairK ElemK, T_PGBS_D& PgbsD)
{
	return TRUE;
}

BOOL CDgnPlateGirder::Get_DesignResultExist(BOOL* abChkItem)
{
	return TRUE;
}

BOOL CDgnPlateGirder::Set_CalcDataCpgElem(ElemPairK ElemK, T_PLATE_GIRDER_CALC_D& param)
{
	param.ElemK = ElemK;

	for ( auto i = 0; i < PGCHK_TYPE::NUM; ++i )
		param.bChkData[i] = FALSE;

	///////////////////////////////////////////////
	param.MatlElem.Initialize();
	if ( !Get_CpgMatlElem(ElemK, param.MatlElem) ) { return FALSE; }
	param.SectElem.Initialize();
	if ( Get_CpgSectElem(ElemK, param.SectElem) ) {}
	param.RbarElem.Initialize();
	if ( Get_CpgRbarElem(ElemK, param.RbarElem) ) { param.bChkData[PGCHK_TYPE::REBAR] = TRUE; }
	param.SconElem.Initialize();
	if ( Get_CpgSconElem(ElemK, param.SconElem) ) { param.bChkData[PGCHK_TYPE::SHEAR_CONNECTOR] = TRUE; }
	param.LstiElem.Initialize();
	if ( Get_CpgLstiElem(ElemK, param.LstiElem) ) { param.bChkData[PGCHK_TYPE::LONG_STIFF] = TRUE; }
	param.TrstElem.Initialize();
	if ( Get_CpgTrstElem(ElemK, param.TrstElem) ) { param.bChkData[PGCHK_TYPE::TRANS_STIFF] = TRUE; }
	param.TsesElem.Initialize();
	if ( Get_CpgTsesElem(ElemK, param.TsesElem) ) { param.bChkData[PGCHK_TYPE::END_SUPPORT] = TRUE; }
	param.LtbdElem.Initialize();
	if ( Get_CpgLtbdElem(ElemK, param.LtbdElem) ) { param.bChkData[PGCHK_TYPE::LTBD] = TRUE; }
	param.MembElem.Initialize();
	if ( Get_CpgMembElem(ElemK, param.MembElem) ) { param.bChkData[PGCHK_TYPE::MEMB] = TRUE; }
	param.BcdtElem.Initialize();
	if ( Get_CpgBcdtElem(ElemK, param.BcdtElem) ) { param.bChkData[PGCHK_TYPE::BUCK_COEFF] = TRUE; }
	param.FrdtElem.Initialize();
	if ( Get_CpgFrdtElem(ElemK, param.FrdtElem) ) { param.bChkData[PGCHK_TYPE::LAMDA] = TRUE; }

	param.bIsGenSect = param.SectElem.bGenSect;
	param.bTapered = m_pDgnSectUtil->Is_TaperedSect(&param.SectElem.SectD);
	Get_CompositeGeneralSectionInfo(param, TRUE);

	return TRUE;
}
BOOL CDgnPlateGirder::Set_CalcDataByPos(T_PLATE_GIRDER_CALC_D& param, int iPos)
{
	if ( iPos > 1 ) { ASSERT(0); return FALSE; }

	param.ClassBase.Initialize();
	param.GLineData.Initialize();
	param.GLineDgn.Initialize();
	param.DLineCalcD.Initialize();

	param.bPosiI = (iPos == 0);
	param.iPos = iPos;

	auto SectD = param.SectElem.SectD;
	param.SectBaseD = param.bTapered && !param.bPosiI ? SectD.SectBefore.SectJ : SectD.SectBefore.SectI;
	if ( param.SectElem.bGenSect )
	{
		param.GLineDgn = param.SectElem.GLineDgn[iPos];
		Get_GenLineDataFromGLineDgn(param.GLineDgn, param.GLineData);
	}

	// RBar
	param.arRbarPosi.RemoveAll();
	param.arRbarPosi.Copy(param.RbarElem.RbarPosi[iPos].arRbarPosi);
	// Longitudinal Stiffener  
	param.arLstiPosi.RemoveAll();
	param.arLstiPosi.Copy(param.LstiElem.LstiPosi[iPos].arLstiPosi);
	// Shear Connector.
	param.SconPosi = param.SconElem.SconPosi[iPos];
	// Transverse Stiffener
	param.TrstPosi = param.TrstElem.TrstPosi[iPos];
	// End Support
	param.TsesPosi = param.TsesElem.TsesPosi[iPos];
	// Later-torsional buckling data
	param.LtbdPosi = param.LtbdElem.LtbdPosi[iPos];
	// Member data, Lu
	param.MembPosi = param.MembElem.MembPosi[iPos];
	// Buckling coefficients for different types of load application.
	param.BcdtPosi = param.BcdtElem.BcdtPosi[iPos];
	// Lamda_v2, Lamda_v4
	param.FrdtPosi = param.FrdtElem.FrdtPosi[iPos];

	return TRUE;
}

BOOL CDgnPlateGirder::Calc_PlateGirderElem(ElemPairK ElemK, const T_PLATE_GIRDER_PARAM_D& ParamD, T_PLATE_GIRDER_RES_D& ResD)
{
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;	

	BOOL bCalcEngine = (ParamD.bIsIDesignModule && !CalcD.bIsGenSect);

    static mutex mtxGeneralSect;
	//general은 멀티쓰레드 안돌게 한다.
	if (!bCalcEngine)
	{
		mtxGeneralSect.lock();
	}

	g_num_mutex.lock();
	// 4. Get Member Data
		
	ElemPairK showElemK = { 0,0 }, IendElemK = { 0,0 }, JendElemK = { 0,0 };
	GetEndElemOfMember(ElemK, showElemK, IendElemK, JendElemK);

	CArray<T_ELEM_K, T_ELEM_K> arMembElemList;
	if (ElemK.second == EN_EL_BEAM)
	{
		BOOL bReverse = FALSE;
		m_pMembCtrl->GetElemListByIncludeElem(ElemK.first, bReverse, showElemK.first, arMembElemList);
	}

	// 5. Get Design Force
	int nFortype = 0;
	auto itr = m_mForce.find(ElemK);
	if ( itr == m_mForce.end() )
	{
		ASSERT(0); return FALSE;
	}
	auto& aForceLcom = itr->second.aForceLcom;

	ADGNSTRESS aStressLcom;
	ADGNSTRESS aStressTnpr;
	if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, nFortype, TRUE, FALSE, aStressLcom, aStressTnpr, ParamD.iLcomNum) ) { return false; }

	nFortype = 2;
	ADGNSTRESS aStressLcomP2;
	ADGNSTRESS aStressTnprP2;
	if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, nFortype, TRUE, FALSE, aStressLcomP2, aStressTnprP2, ParamD.iLcomNum) ) { return false; }

	ADGNFORCE aForceConst;
	m_pForcCtrl->Get_LcomForce4ScfpSum(ElemK, ParamD.iLcomNum, aForceConst);

	ASCFPSTRESS aScfpCr, aScfpSh, aScfpTe, aScfpRe;
	bool bExistCrp = false, bExistShr = false, bExistBST = false;
	if ( !m_pForcCtrl->Get_LcomStressScfp(ElemK, ParamD.iLcomNum, aScfpCr, aScfpSh, aScfpTe, aScfpRe) ) return FALSE;  // Get Internal Stress

	T_STAG_K nGdLastStagK = 0;
	int nLastStepNo = 0;
	m_pForcCtrl->GetGirderLastStageKey(ElemK, nGdLastStagK, nLastStepNo);
	g_num_mutex.unlock();

	for ( int j = 0; j < 2; j++ )
	{
		ResD.PgbrPosiRat.PgbrBase[j].ElemK = ElemK.first;
		ResD.PgbrNegaRat.PgbrBase[j].ElemK = ElemK.first;
		ResD.PrvsRat.PrvsBase[j].ElemK = ElemK.first;
		ResD.PltbRat.PltbBase[j].ElemK = ElemK.first;
		ResD.PrtfRat.PrtfBase[j].ElemK = ElemK.first;
		ResD.PglsRat.PglsBase[j].ElemK = ElemK.first;
		ResD.PgrfRat.PgrfBase[j].ElemK = ElemK.first;
		ResD.PgbsRat.PgbsBase[j].ElemK = ElemK.first;
		ResD.PlssRat.PlssBase[j].ElemK = ElemK.first;
	}

	int iMaxMin = 0, iDgnLcomNo = 0, iOrgLcomNo = 0, iSerLcomCount = 0, iMaxMinType = 0, iServLcomType = 0;
	int iForceLcomSize = aForceLcom.GetSize();
	CString strOrgLcomNa = _T("");
	double dC4 = 0.0;

	int iPosiCount[2] = { 0,0 }, iNegaCount[2] = { 0,0 };
	double dPosiMaxRatio[2] = { 0.0, 0.0 }, dNegaMaxRatio[2] = { 0.0, 0.0 };
	int iVCount[2] = { 0, 0 };
	double dVMaxRatio[2] = { 0.0, 0.0 };
	int iLTBCount[2] = { 0, 0 };
	double dLTBMaxRatio[2] = { 0.0, 0.0 };
	int iRTFCount[2] = { 0, 0 };
	double dRTFMaxRatio[2] = { 0.0, 0.0 };
	int iLSCount[2] = { 0, 0 };
	double dLSMaxRatio[2] = { 0.0, 0.0 };
	int iRFCount[2] = { 0, 0 };
	double dRFMaxRatio[2] = { 0.0 };
	int iStressGrSLS_Count[2] = { 0, 0 }; // girder stress
	double dStressGrMaxRatio[2] = { 0.0, 0.0 };
	int iStressSbSLS_Count[2] = { 0, 0 }; // slab stress
	double dStressSbMaxRatio[2] = { 0.0, 0.0 };
	int iStressBarSLS_Count[2] = { 0, 0 }; // Rebar stress
	double dStressBarMaxRatio[2] = { 0.0, 0.0 };
	int iLS_SLS_Count[2] = { 0, 0 };
	double dLS_SLSMaxRatio[2] = { 0.0, 0.0 };

	ResD.dtau_Ed_max[0] = ResD.dtau_Ed_max[1] = 0.0;

	for ( int j = 0; j < iForceLcomSize; j++ )
	{
		iDgnLcomNo = j + 1;
		SetDgnLcomNo(iDgnLcomNo);
		if ( !m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType) )
		{
			ASSERT(0);
		}

		CString strExecuteMsg;
		strExecuteMsg.Format(_LS(IDS_DGN_CPG_LCOM_INFO), ElemK, iOrgLcomNo, iDgnLcomNo);
		//GSaveHistoryNF(strExecuteMsg);

		BOOL bULSLcom = m_pForcCtrl->Is_LcomForStrn(j + 1);
		BOOL bSLSLcom = m_pForcCtrl->Is_LcomForStrs(j + 1);
		BOOL bFatLcom = m_pForcCtrl->Is_LcomForFati(j + 1);

		//////////////////////////////////////////////////////////////////////////
		//g_num_mutex.lock();
		//////////////////////////////////////////////////////////////////////////

		if ( bSLSLcom ) { iServLcomType = m_pForcCtrl->Get_ServLcomType(iDgnLcomNo); }
		const _DGN_FORC_CRC& ForceCrc = aForceLcom.GetAt(j);
		const _DGN_STRS_CRC& StressCrc = aStressLcom.GetAt(j);
		const _DGN_STRS_CRC& StressSlab = aStressLcomP2.GetAt(j);

		const _DGN_FORC_CRC& ForceConst = aForceConst[j];
		_DGN_SCFP_PART_LCASE& ScfpCr = aScfpCr[j];
		_DGN_SCFP_PART_LCASE& ScfpSh = aScfpSh[j];
		_DGN_SCFP_PART_LCASE& ScfpTe = aScfpTe[j];

		m_pForcCtrl->Get_SfcpExist(D_LCOMTYPE_STEEL, iOrgLcomNo, bExistCrp, bExistShr, bExistBST);
		if ( !bExistCrp ) ScfpCr.Initialize();
		if ( !bExistShr ) ScfpSh.Initialize();
		if ( !bExistBST ) ScfpTe.Initialize();

		_DGN_SCFP_PART_LCASE ScfpSum;
		ScfpSum += ScfpCr;
		ScfpSum += ScfpSh;
		ScfpSum += ScfpTe;
		SetScfpForce4Lcom(ForceConst, ScfpSum);
		//////////////////////////////////////////////////////////////////////////      
		auto& LcomResD = ResD.svLcomRes[j];
		LcomResD.iDgnLcomNo = iDgnLcomNo;
		LcomResD.bULSLcom = bULSLcom;
		LcomResD.bSLSLcom = bSLSLcom;

		// I-End
		_DGN_FORC_CRC MembEndForce;
		double dM1 = 0.0, dV1 = 0.0, dM2 = 0.0, dV2 = 0.0;
		if (ElemK.second == EN_EL_BEAM)
		{
			if (bULSLcom && ParamD.bCheckStrength[2] && !CalcD.bIsGenSect)
			{
				m_pForcCtrl->Get_LcomDataForDesign_LcomK(IendElemK, iDgnLcomNo, TRUE, FALSE, MembEndForce);
				dM1 = MembEndForce.dMuy[0];
				dV1 = MembEndForce.dFzz[0];
				//
				MembEndForce.Initialize();
				m_pForcCtrl->Get_LcomDataForDesign_LcomK(JendElemK, iDgnLcomNo, TRUE, FALSE, MembEndForce);
				dM2 = MembEndForce.dMuy[1];
				dV2 = MembEndForce.dFzz[1];
				// Get, C4 
				ArrElemPairKey aMembElemPairK;
				CDBLib::ConvertToElemPairKey(EN_EL_BEAM, arMembElemList, aMembElemPairK);
				if (m_pDataCtrl->Get_C4valueForMomentType(iDgnLcomNo, showElemK, aMembElemPairK, dC4)) {}
				else { dC4 = 0.0; }
			}
		}
		// Jaeoh. Modification. 이대근 부장님 요청 
		
		auto itr = m_mStageForce.find(std::make_pair(ElemK, iDgnLcomNo));
		if ( itr == m_mStageForce.end() )
		{
			ASSERT(0); return false;
		}
		CPG_FORC_STAG& ForceStage = (CPG_FORC_STAG&)itr->second;
		//////////////////////////////////////////////////////////////////////////
		//g_num_mutex.unlock();
		//////////////////////////////////////////////////////////////////////////

		for ( int k = 0; k < 2; k++ )	// Repeat I-End & J-End.
		{
			if ( !ParamD.bChkIJ[k] ) { continue; }

			if ( !Set_CalcDataByPos(CalcD, k) ) { ASSERT(0); continue; }

			double dFxx = ForceCrc.dFxx[k];
			double dFyy = ForceCrc.dFyy[k];
			double dFzz = ForceCrc.dFzz[k];
			double dMux = ForceCrc.dMux[k];
			double dMuy = ForceCrc.dMuy[k];
			double dMuz = ForceCrc.dMuz[k];
			double dMa_Ed = ForceStage.MaxForce.dMuy[k];
			double aGirderForce[6] = { ForceStage.MaxForce.dFxx[k], ForceStage.MaxForce.dFyy[k], ForceStage.MaxForce.dFzz[k],
				ForceStage.MaxForce.dMux[k], ForceStage.MaxForce.dMuy[k], ForceStage.MaxForce.dMuz[k] };

			double aForce[6] = { dFxx, dFyy, dFzz, dMux, dMuy, dMuz };
			double aCptForce[6] = { 0.0 };
			for ( int l = 0; l < 6; ++l ) { aCptForce[l] = aForce[l] - aGirderForce[l]; }

			double aStress[4] = { StressCrc.dStl[k], StressCrc.dStr[k], StressCrc.dSbr[k], StressCrc.dSbl[k] };

			if ( dMuy > -m_dZero ) // 정모멘트 인데 Stage 모멘트가 부모멘트면 0으로..
			{
				if ( dMa_Ed < 0.0 ) { dMa_Ed = 0.0; }
			}
			else
			{
				if ( dMa_Ed > 0.0 ) { dMa_Ed = 0.0; }
			}
			UINT nPosi = (k == 0 ? 1 : 2);
			T_PGBR_BASE& PgbrGd = LcomResD.PgbrLcomD.BeforePgbr[k];
			T_PGBR_BASE& PgbrBase = LcomResD.PgbrLcomD.PgbrBase[k];
			T_PRVS_BASE& PrvsGd = LcomResD.PrvsD.BeforePrvs[k];
			T_PRVS_BASE& PrvsBase = LcomResD.PrvsD.PrvsBase[k];
			T_PLTB_BASE& PltbGd = LcomResD.PltbD.BeforePltb[k];
			T_PLTB_BASE& PltbBase = LcomResD.PltbD.PltbBase[k];
			T_PRTF_BASE& PrtfBase = LcomResD.PrtfD.PrtfBase[k];
			T_PGLS_BASE& PglsGd = LcomResD.PglsD.BeforePgls[k];
			T_PGLS_BASE& PglsBase = LcomResD.PglsD.PglsBase[k];
			T_PGRF_BASE& PgrfBase = LcomResD.PgrfD.PgrfBase[k];
			T_PGBS_BASE& PgbsBase = LcomResD.PgbsD.PgbsBase[k];
			T_PLSS_BASE& PlssGd = LcomResD.PlssD.BeforePlss[k];
			T_PLSS_BASE& PlssBase = LcomResD.PlssD.PlssBase[k];
			T_RHOC_BASE& RhoCBase = LcomResD.RhoC.Rho_C[k];


			//여기서 부터 이렇게 하면 될려나?
			if ( bCalcEngine )
			{
				T_PGBS_DETAIL PgbsDetail;
				SetPgbsBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, iServLcomType, aGirderForce, aCptForce, PgbsBase);
				//SetPartStress(k, StressCrc, StressSlab, PgbsBase, PgbsDetail);
			}

			SetCheckPosition(k);
			if ( bCalcEngine )
			{
				//thread 별로 현재 Lcom에 대한 정보를 들고 있어야 나중에 input converter에서 해당 lcom에 대한 data를 가져올수 있다.
				m_ElemToLcom[std::this_thread::get_id()] = std::make_tuple(ElemK, k, iDgnLcomNo);
				IDgnCodeCheck::Check_Section(false);
			}
			SetPgbrBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, PgbrGd, PgbrBase);

			// 6. Get Section Class
			if ( !bCalcEngine )
			{
				if ( !Get_ClassifyClassSection(CalcD, aGirderForce, aCptForce) ) ASSERT(0);
			}

			// 7. Flexure Resistance (Bending)
			T_PGBR_DETAIL PgbrDetail;
			if ( !bCalcEngine )
			{
				if ( !CalcD.bIsGenSect ) { Calc_BendingResistance(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
				else { Calc_BendingResistance4GenSect(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
			}
			CalcD.PgbrBase = PgbrBase;
			CalcD.PgbrDetail = PgbrDetail;

			if ( !bCalcEngine )
			{
				SetRhocBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, PgbrDetail, RhoCBase);
			}			

			if ( bULSLcom ) // Get Critical Flexure Design Result
			{
				if ( PgbrBase.dMy > -m_dZero )
				{
					if ( iPosiCount[k] == 0 )
					{
						dPosiMaxRatio[k] = PgbrBase.dRatio;
						ResD.PgbrPosiRat.BeforePgbr[k] = PgbrGd;
						ResD.PgbrPosiRat.PgbrBase[k] = PgbrBase;
						ResD.RhocPosiRat.Rho_C[k] = RhoCBase;
					}
					else
					{
						if ( dPosiMaxRatio[k] < PgbrBase.dRatio )
						{
							dPosiMaxRatio[k] = PgbrBase.dRatio;
							ResD.PgbrPosiRat.BeforePgbr[k] = PgbrGd;
							ResD.PgbrPosiRat.PgbrBase[k] = PgbrBase;
							ResD.RhocPosiRat.Rho_C[k] = RhoCBase;
						}
					}
					iPosiCount[k]++;
				}
				else
				{
					if ( iNegaCount[k] == 0 )
					{
						dNegaMaxRatio[k] = PgbrBase.dRatio;
						ResD.PgbrNegaRat.BeforePgbr[k] = PgbrGd;
						ResD.PgbrNegaRat.PgbrBase[k] = PgbrBase;
						ResD.RhocNegaRat.Rho_C[k] = RhoCBase;
					}
					else
					{
						if ( dNegaMaxRatio[k] < PgbrBase.dRatio )
						{
							dNegaMaxRatio[k] = PgbrBase.dRatio;
							ResD.PgbrNegaRat.BeforePgbr[k] = PgbrGd;
							ResD.PgbrNegaRat.PgbrBase[k] = PgbrBase;
							ResD.RhocNegaRat.Rho_C[k] = RhoCBase;
						}
					}
					iNegaCount[k]++;
				}
			}

			// 8. Vertical Shear Resistance
			if ( bULSLcom && ParamD.bCheckStrength[1] )
			{
				T_PRVS_DETAIL PrvsDetail;
				SetPrvsBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, PrvsGd, PrvsBase);
				if ( !bCalcEngine )
				{
					if ( !CalcD.bIsGenSect ) { Calc_VerticalShearResistance(CalcD, PrvsGd, PrvsBase, PrvsDetail); }
					else { Calc_VerticalShearResistance4GenSect(CalcD, PrvsGd, PrvsBase, PrvsDetail); }
				}

				if ( iVCount[k] == 0 )
				{
					dVMaxRatio[k] = PrvsBase.dRatio;
					ResD.PrvsRat.BeforePrvs[k] = PrvsGd;
					ResD.PrvsRat.PrvsBase[k] = PrvsBase;
				}
				else
				{
					if ( dVMaxRatio[k] < PrvsBase.dRatio )
					{
						dVMaxRatio[k] = PrvsBase.dRatio;
						ResD.PrvsRat.BeforePrvs[k] = PrvsGd;
						ResD.PrvsRat.PrvsBase[k] = PrvsBase;
					}
				}
				iVCount[k]++;
			}

			// 9. Lateral Torsional Buckling
			if ( bULSLcom && !CalcD.bIsGenSect && ParamD.bCheckStrength[2] )
			{
				T_PLTB_DETAIL PltbDetail;
				SetPltbBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, dV1, dV2, dM1, dM2, dC4, PltbGd, PltbBase);
				if ( !bCalcEngine )
				{
					Calc_LateralTorsionalBucklingResistance(CalcD, PltbBase, PltbDetail);
				}

				if ( iLTBCount[k] == 0 )
				{
					dLTBMaxRatio[k] = PltbBase.dRatio;
					ResD.PltbRat.BeforePltb[k] = PltbGd;
					ResD.PltbRat.PltbBase[k] = PltbBase;
				}
				else
				{
					if ( dLTBMaxRatio[k] < PltbBase.dRatio )
					{
						dLTBMaxRatio[k] = PltbBase.dRatio;
						ResD.PltbRat.BeforePltb[k] = PltbGd;
						ResD.PltbRat.PltbBase[k] = PltbBase;
					}
				}
				iLTBCount[k]++;
			}

			// 10. Transverse Force Resistance
			if ( bULSLcom && !CalcD.bIsGenSect && ParamD.bCheckStrength[3] )
			{
				SetPrtfBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aForce, PrtfBase);
				// Modify, Jaeoh. MQC. 3797, [1/21/2011]
				BOOL bCalcPrtf = FALSE;
				if ( PrtfBase.bChk )
				{
					if ( !CalcD.bChkData[PGCHK_TYPE::BUCK_COEFF] || (CalcD.bChkData[PGCHK_TYPE::BUCK_COEFF] && !CalcD.BcdtPosi.bUseTfType) )
					{
						PrtfBase.bChk = FALSE;
						bCalcPrtf = FALSE;
					}
					else bCalcPrtf = TRUE;
				}
				else bCalcPrtf = FALSE;

				if ( bCalcPrtf )
				{
					T_PRTF_DETAIL PrtfDetail;
					if ( !bCalcEngine )
					{
						Calc_TransverseForceResistance(CalcD, PrtfBase, PrtfDetail);
					}
					LcomResD.PrtfD.PrtfBase[k] = PrtfBase;
					if ( iRTFCount[k] == 0 ) { dRTFMaxRatio[k] = PrtfBase.dRatio; ResD.PrtfRat.PrtfBase[k] = PrtfBase; }
					else
					{
						if ( dRTFMaxRatio[k] < PrtfBase.dRatio ) { dRTFMaxRatio[k] = PrtfBase.dRatio;  ResD.PrtfRat.PrtfBase[k] = PrtfBase; }
					}
					iRTFCount[k]++;
				}
			}

			// 11. Longitudinal Shear Resistance
			T_PGLS_DETAIL PglsDetail;
			if ( bULSLcom && ParamD.bCheckStrength[4] )
			{
				SetPglsBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, PglsGd, PglsBase);
				if ( !bCalcEngine )
				{
					if ( !CalcD.bIsGenSect ) { Calc_LongitudinalShearResistance(bSLSLcom, CalcD, ParamD.dks, PglsGd, PglsBase, PglsDetail); }
					else { Calc_LongitudinalShearResistance4GenSect(bSLSLcom, CalcD, ParamD.dks, PglsGd, PglsBase, PglsDetail); }
				}
				CalcD.PglsBase = PglsBase;

				// Modify by GAY. PMS:4216. ('12.05.17). 합성전 거더의 설계결과 할당 누락 수정.
				if ( iLSCount[k] == 0 ) { dLSMaxRatio[k] = PglsBase.dRatio; ResD.PglsRat.BeforePgls[k] = PglsGd; ResD.PglsRat.PglsBase[k] = PglsBase; }
				else
				{
					if ( dLSMaxRatio[k] < PglsBase.dRatio ) { dLSMaxRatio[k] = PglsBase.dRatio; ResD.PglsRat.BeforePgls[k] = PglsGd;  ResD.PglsRat.PglsBase[k] = PglsBase; }
				}
				iLSCount[k]++;
			}

			// 12. Fatigue Resistance
			if ( bFatLcom && !CalcD.bIsGenSect && ParamD.bCheckStrength[5] )
			{
				// Fatigue Resistance를 단독으로 사용할 경우 Longitudinal Shear Resistance가 먼저 선행되어야 하기 때문에..
				if ( !CalcD.bIsGenSect )
				{
					SetPglsBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, PglsGd, PglsBase);
					if ( !bCalcEngine )
					{
						Calc_LongitudinalShearResistance(bSLSLcom, CalcD, ParamD.dks, PglsGd, PglsBase, PglsDetail);
					}
					CalcD.PglsBase = PglsBase;
				}

				T_PGRF_DETAIL PgrfDetail;
				SetPgfrBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aForce, PgrfBase);
				if ( !bCalcEngine )
				{
					Calc_FatigueResistance(CalcD, ParamD.dt_Ld, PgrfBase);
				}

				if ( iRFCount[k] == 0 ) { dRFMaxRatio[k] = PgrfBase.dRatio; ResD.PgrfRat.PgrfBase[k] = PgrfBase; }
				else
				{
					if ( dRFMaxRatio[k] < PgrfBase.dRatio ) { dRFMaxRatio[k] = PgrfBase.dRatio;  ResD.PgrfRat.PgrfBase[k] = PgrfBase; }
				}
				iRFCount[k]++;
			}

			// 13. Check Stress
			if ( bSLSLcom && ParamD.bCheckService[0] )
			{
				T_PGBS_DETAIL PgbsDetail;
				SetPgbsBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, iServLcomType, aGirderForce, aCptForce, PgbsBase);
				int nPosIx = nPosi - 1;
				//Get_ConstraintStress(nPosIx, ScfpCr, ScfpSh, ScfpTe, PgbsDetail.StrD);
				SetPartStress(k, StressCrc, StressSlab, PgbsBase, PgbsDetail);
				if ( !bCalcEngine )
				{
					Calc_StressCheck4SLS(CalcD, ParamD.dk1, ParamD.dk2, ParamD.dk3, ForceConst, PgbsBase, PgbsDetail);
				}

				ResD.PgbsRat.PgbsBase[k].bChk = ParamD.bChkIJ[k];
				ResD.PgbsRat.PgbsBase[k].ElemK = ElemK.first;
				ResD.PgbsRat.PgbsBase[k].nPosi = nPosi;
				// Girder
				if ( iServLcomType == 3 ) // Characteristic
				{
					if ( iStressGrSLS_Count[k] == 0 )
					{
						dStressGrMaxRatio[k] = PgbsBase.ChkGirder.dMaxRatio;
						ResD.PgbsRat.PgbsBase[k].ChkGirder = PgbsBase.ChkGirder; //PgbsGrCrD = PgbsBase.ChkGirder;
						ResD.dtau_Ed_max[k] = PgbsBase.ChkGirder.dtau_w;
					}
					else
					{
						if ( dStressGrMaxRatio[k] < PgbsBase.ChkGirder.dMaxRatio )
						{
							dStressGrMaxRatio[k] = PgbsBase.ChkGirder.dMaxRatio;
							ResD.PgbsRat.PgbsBase[k].ChkGirder = PgbsBase.ChkGirder; //PgbsGrCrD = PgbsBase.ChkGirder;
						}
						if ( ResD.dtau_Ed_max[k] < PgbsBase.ChkGirder.dtau_w )
						{
							ResD.dtau_Ed_max[k] = PgbsBase.ChkGirder.dtau_w;
						}
					}
					iStressGrSLS_Count[k]++;
				}
				// Slab
				if ( iServLcomType == 1 || iServLcomType == 3 ) // 1=Quasi, 3=Characteristic
				{
					if ( iStressSbSLS_Count[k] == 0 )
					{
						dStressSbMaxRatio[k] = PgbsBase.ChkSlab.dMaxRatio_sb;
						ResD.PgbsRat.PgbsBase[k].ChkSlab = PgbsBase.ChkSlab; //PgbsSbCrD = PgbsBase.ChkSlab;
					}
					else
					{
						if ( dStressSbMaxRatio[k] < PgbsBase.ChkSlab.dMaxRatio_sb )
						{
							dStressSbMaxRatio[k] = PgbsBase.ChkSlab.dMaxRatio_sb;
							ResD.PgbsRat.PgbsBase[k].ChkSlab = PgbsBase.ChkSlab; //PgbsSbCrD = PgbsBase.ChkSlab;
						}
					}
					iStressSbSLS_Count[k]++;
				}
				// Rebar
				if ( iStressBarSLS_Count[k] == 0 )
				{
					dStressBarMaxRatio[k] = PgbsBase.ChkBar.dMaxRatio_bar;
					ResD.PgbsRat.PgbsBase[k].ChkBar = PgbsBase.ChkBar; //PgbsBarCrD = PgbsBase.ChkBar;
				}
				else
				{
					if ( dStressBarMaxRatio[k] < PgbsBase.ChkBar.dMaxRatio_bar )
					{
						dStressBarMaxRatio[k] = PgbsBase.ChkBar.dMaxRatio_bar;
						ResD.PgbsRat.PgbsBase[k].ChkBar = PgbsBase.ChkBar; //PgbsBarCrD = PgbsBase.ChkBar;
					}
				}
				iStressBarSLS_Count[k]++;
				//
				BOOL bChkGirder = TRUE, bChkSlab = TRUE, bChkRebar = TRUE;
				if ( ResD.PgbsRat.PgbsBase[k].ChkGirder.LcomK != 0 && !ResD.PgbsRat.PgbsBase[k].ChkGirder.bOK ) { bChkGirder = FALSE; }
				if ( ResD.PgbsRat.PgbsBase[k].ChkSlab.LcomK != 0 && !ResD.PgbsRat.PgbsBase[k].ChkSlab.bChk_Stress_sb ) { bChkSlab = FALSE; }
				if ( ResD.PgbsRat.PgbsBase[k].ChkBar.LcomK != 0 && !ResD.PgbsRat.PgbsBase[k].ChkBar.bChk_Stress_bar ) { bChkRebar = FALSE; }

				if ( bChkGirder && bChkSlab && bChkRebar ) { ResD.PgbsRat.PgbsBase[k].bOK = TRUE; }
				else { ResD.PgbsRat.PgbsBase[k].bOK = FALSE; }
			}

			// 14. Check Longitudianl Shear for SLS
			if ( bSLSLcom && ParamD.bCheckService[1] )
			{
				T_PLSS_DETAIL PlssDetail;
				SetPlssBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, iServLcomType, aGirderForce, aCptForce, PlssGd, PlssBase);
				if ( !bCalcEngine )
				{
					Calc_LongitudinalShearResistance4SLS(CalcD, ParamD.dks, PlssGd, PlssBase, PlssDetail);
				}

				// Modify by GAY. PMS:4216. ('12.05.17). 합성전 거더의 설계결과 할당 누락 수정.
				if ( iLS_SLS_Count[k] == 0 ) { dLS_SLSMaxRatio[k] = PlssBase.dRatio; ResD.PlssRat.BeforePlss[k] = PlssGd;  ResD.PlssRat.PlssBase[k] = PlssBase; }
				else
				{
					if ( dLS_SLSMaxRatio[k] < PlssBase.dRatio ) { dLS_SLSMaxRatio[k] = PlssBase.dRatio; ResD.PlssRat.BeforePlss[k] = PlssGd;  ResD.PlssRat.PlssBase[k] = PlssBase; }
				}
				iLS_SLS_Count[k]++;
			}
		}

		//g_num_mutex.unlock();
	}

    if ( !bCalcEngine )
    {
        mtxGeneralSect.unlock();
    }

	return true;
}

BOOL CDgnPlateGirder::Calc_PlateGirderElem_IDesign(ElemPairK ElemK, const T_PLATE_GIRDER_PARAM_D& ParamD, T_PLATE_GIRDER_RES_D& ResD)
{
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;

	BOOL bCalcEngine = (ParamD.bIsIDesignModule && !CalcD.bIsGenSect);

	g_num_mutex.lock();
	// 4. Get Member Data
	ElemPairK showElemK = { 0,0 }, IendElemK = { 0,0 }, JendElemK = { 0,0 };
	GetEndElemOfMember(ElemK, showElemK, IendElemK, JendElemK);

	CArray<T_ELEM_K, T_ELEM_K> arMembElemList;
	if (ElemK.second == EN_EL_BEAM)
	{
		BOOL bReverse = FALSE;
		m_pMembCtrl->GetElemListByIncludeElem(ElemK.first, bReverse, showElemK.first, arMembElemList);
	}

	// 5. Get Design Force
	int nFortype = 0;
	ADGNFORCE aForceLcom;
	if ( !m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, ParamD.iLcomNum) ) { return false; }

	ADGNSTRESS aStressLcom;
	ADGNSTRESS aStressTnpr;
	if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, nFortype, TRUE, FALSE, aStressLcom, aStressTnpr, ParamD.iLcomNum) ) { return false; }

	nFortype = 2;
	ADGNSTRESS aStressLcomP2;
	ADGNSTRESS aStressTnprP2;
	if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, nFortype, TRUE, FALSE, aStressLcomP2, aStressTnprP2, ParamD.iLcomNum) ) { return false; }

	ADGNFORCE aForceConst;
	m_pForcCtrl->Get_LcomForce4ScfpSum(ElemK, ParamD.iLcomNum, aForceConst);

	ASCFPSTRESS aScfpCr, aScfpSh, aScfpTe, aScfpRe;
	bool bExistCrp = false, bExistShr = false, bExistBST = false;
	if ( !m_pForcCtrl->Get_LcomStressScfp(ElemK, ParamD.iLcomNum, aScfpCr, aScfpSh, aScfpTe, aScfpRe) ) return FALSE;  // Get Internal Stress

	T_STAG_K nGdLastStagK = 0;
	int nLastStepNo = 0;
	m_pForcCtrl->GetGirderLastStageKey(ElemK, nGdLastStagK, nLastStepNo);
	g_num_mutex.unlock();

	for ( int j = 0; j < 2; j++ )
	{
		ResD.PgbrPosiRat.PgbrBase[j].ElemK = ElemK.first;
		ResD.PgbrNegaRat.PgbrBase[j].ElemK = ElemK.first;
		ResD.PrvsRat.PrvsBase[j].ElemK = ElemK.first;
		ResD.PltbRat.PltbBase[j].ElemK = ElemK.first;
		ResD.PrtfRat.PrtfBase[j].ElemK = ElemK.first;
		ResD.PglsRat.PglsBase[j].ElemK = ElemK.first;
		ResD.PgrfRat.PgrfBase[j].ElemK = ElemK.first;
		ResD.PgbsRat.PgbsBase[j].ElemK = ElemK.first;
		ResD.PlssRat.PlssBase[j].ElemK = ElemK.first;
	}

	int iMaxMin = 0, iDgnLcomNo = 0, iOrgLcomNo = 0, iSerLcomCount = 0, iMaxMinType = 0, iServLcomType = 0;
	int iForceLcomSize = aForceLcom.GetSize();
	CString strOrgLcomNa = _T("");
	double dC4 = 0.0;

	int iPosiCount[2] = { 0,0 }, iNegaCount[2] = { 0,0 };
	double dPosiMaxRatio[2] = { 0.0, 0.0 }, dNegaMaxRatio[2] = { 0.0, 0.0 };
	int iVCount[2] = { 0, 0 };
	double dVMaxRatio[2] = { 0.0, 0.0 };
	int iLTBCount[2] = { 0, 0 };
	double dLTBMaxRatio[2] = { 0.0, 0.0 };
	int iRTFCount[2] = { 0, 0 };
	double dRTFMaxRatio[2] = { 0.0, 0.0 };
	int iLSCount[2] = { 0, 0 };
	double dLSMaxRatio[2] = { 0.0, 0.0 };
	int iRFCount[2] = { 0, 0 };
	double dRFMaxRatio[2] = { 0.0 };
	int iStressGrSLS_Count[2] = { 0, 0 }; // girder stress
	double dStressGrMaxRatio[2] = { 0.0, 0.0 };
	int iStressSbSLS_Count[2] = { 0, 0 }; // slab stress
	double dStressSbMaxRatio[2] = { 0.0, 0.0 };
	int iStressBarSLS_Count[2] = { 0, 0 }; // Rebar stress
	double dStressBarMaxRatio[2] = { 0.0, 0.0 };
	int iLS_SLS_Count[2] = { 0, 0 };
	double dLS_SLSMaxRatio[2] = { 0.0, 0.0 };

	ResD.dtau_Ed_max[0] = ResD.dtau_Ed_max[1] = 0.0;

	for ( int j = 0; j < iForceLcomSize; j++ )
	{
		iDgnLcomNo = j + 1;
		SetDgnLcomNo(iDgnLcomNo);
		if ( !m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType) )
		{
			ASSERT(0);
		}

		CString strExecuteMsg;
		strExecuteMsg.Format(_LS(IDS_DGN_CPG_LCOM_INFO), ElemK, iOrgLcomNo, iDgnLcomNo);
		//GSaveHistoryNF(strExecuteMsg);

		BOOL bULSLcom = m_pForcCtrl->Is_LcomForStrn(j + 1);
		BOOL bSLSLcom = m_pForcCtrl->Is_LcomForStrs(j + 1);
		BOOL bFatLcom = m_pForcCtrl->Is_LcomForFati(j + 1);

		//////////////////////////////////////////////////////////////////////////
		g_num_mutex.lock();
		//////////////////////////////////////////////////////////////////////////

		if ( bSLSLcom ) { iServLcomType = m_pForcCtrl->Get_ServLcomType(iDgnLcomNo); }
		const _DGN_FORC_CRC& ForceCrc = aForceLcom.GetAt(j);
		const _DGN_STRS_CRC& StressCrc = aStressLcom.GetAt(j);
		const _DGN_STRS_CRC& StressSlab = aStressLcomP2.GetAt(j);

		const _DGN_FORC_CRC& ForceConst = aForceConst[j];
		_DGN_SCFP_PART_LCASE& ScfpCr = aScfpCr[j];
		_DGN_SCFP_PART_LCASE& ScfpSh = aScfpSh[j];
		_DGN_SCFP_PART_LCASE& ScfpTe = aScfpTe[j];

		m_pForcCtrl->Get_SfcpExist(D_LCOMTYPE_STEEL, iOrgLcomNo, bExistCrp, bExistShr, bExistBST);
		if ( !bExistCrp ) ScfpCr.Initialize();
		if ( !bExistShr ) ScfpSh.Initialize();
		if ( !bExistBST ) ScfpTe.Initialize();

		_DGN_SCFP_PART_LCASE ScfpSum;
		ScfpSum += ScfpCr;
		ScfpSum += ScfpSh;
		ScfpSum += ScfpTe;
		SetScfpForce4Lcom(ForceConst, ScfpSum);
		//////////////////////////////////////////////////////////////////////////      
		auto& LcomResD = ResD.svLcomRes[j];
		LcomResD.iDgnLcomNo = iDgnLcomNo;
		LcomResD.bULSLcom = bULSLcom;
		LcomResD.bSLSLcom = bSLSLcom;

		// I-End
		_DGN_FORC_CRC MembEndForce;
		double dM1 = 0.0, dV1 = 0.0, dM2 = 0.0, dV2 = 0.0;
		if ( bULSLcom && ParamD.bCheckStrength[2] && !CalcD.bIsGenSect )
		{
			m_pForcCtrl->Get_LcomDataForDesign_LcomK(IendElemK, iDgnLcomNo, TRUE, FALSE, MembEndForce);
			dM1 = MembEndForce.dMuy[0];
			dV1 = MembEndForce.dFzz[0];
			//
			MembEndForce.Initialize();
			m_pForcCtrl->Get_LcomDataForDesign_LcomK(JendElemK, iDgnLcomNo, TRUE, FALSE, MembEndForce);
			dM2 = MembEndForce.dMuy[1];
			dV2 = MembEndForce.dFzz[1];
			// Get, C4 
			ArrElemPairKey aMembElemPairK;
			CDBLib::ConvertToElemPairKey(EN_EL_BEAM, arMembElemList, aMembElemPairK);
			if ( m_pDataCtrl->Get_C4valueForMomentType(iDgnLcomNo, showElemK, aMembElemPairK, dC4) ) {}
			else { dC4 = 0.0; }
		}
		// Jaeoh. Modification. 이대근 부장님 요청 
		CPG_FORC_STAG ForceStage;
		ForceStage.Initialize();
		T_STCT_D StctD;
		m_pDoc->m_pAttrCtrl->GetStct(StctD);
		if ( !m_pDataCtrl->Get_GirderCSLoadOfLCOM(StctD, ElemK, nGdLastStagK, nLastStepNo, iDgnLcomNo, ForceStage) ) { ASSERT(0); }

		//////////////////////////////////////////////////////////////////////////
		g_num_mutex.unlock();
		//////////////////////////////////////////////////////////////////////////

		for ( int k = 0; k < 2; k++ )	// Repeat I-End & J-End.
		{
			if ( !ParamD.bChkIJ[k] ) { continue; }

			if ( !Set_CalcDataByPos(CalcD, k) ) { ASSERT(0); continue; }

			double dFxx = ForceCrc.dFxx[k];
			double dFyy = ForceCrc.dFyy[k];
			double dFzz = ForceCrc.dFzz[k];
			double dMux = ForceCrc.dMux[k];
			double dMuy = ForceCrc.dMuy[k];
			double dMuz = ForceCrc.dMuz[k];
			double dMa_Ed = ForceStage.MaxForce.dMuy[k];
			double aGirderForce[6] = { ForceStage.MaxForce.dFxx[k], ForceStage.MaxForce.dFyy[k], ForceStage.MaxForce.dFzz[k],
				ForceStage.MaxForce.dMux[k], ForceStage.MaxForce.dMuy[k], ForceStage.MaxForce.dMuz[k] };

			double aForce[6] = { dFxx, dFyy, dFzz, dMux, dMuy, dMuz };
			double aCptForce[6] = { 0.0 };
			for ( int l = 0; l < 6; ++l ) { aCptForce[l] = aForce[l] - aGirderForce[l]; }

			double aStress[4] = { StressCrc.dStl[k], StressCrc.dStr[k], StressCrc.dSbr[k], StressCrc.dSbl[k] };

			if ( dMuy > -m_dZero ) // 정모멘트 인데 Stage 모멘트가 부모멘트면 0으로..
			{
				if ( dMa_Ed < 0.0 ) { dMa_Ed = 0.0; }
			}
			else
			{
				if ( dMa_Ed > 0.0 ) { dMa_Ed = 0.0; }
			}
			UINT nPosi = (k == 0 ? 1 : 2);
			T_PGBR_BASE& PgbrGd = LcomResD.PgbrLcomD.BeforePgbr[k];
			T_PGBR_BASE& PgbrBase = LcomResD.PgbrLcomD.PgbrBase[k];
			T_PRVS_BASE& PrvsGd = LcomResD.PrvsD.BeforePrvs[k];
			T_PRVS_BASE& PrvsBase = LcomResD.PrvsD.PrvsBase[k];
			T_PLTB_BASE& PltbGd = LcomResD.PltbD.BeforePltb[k];
			T_PLTB_BASE& PltbBase = LcomResD.PltbD.PltbBase[k];
			T_PRTF_BASE& PrtfBase = LcomResD.PrtfD.PrtfBase[k];
			T_PGLS_BASE& PglsGd = LcomResD.PglsD.BeforePgls[k];
			T_PGLS_BASE& PglsBase = LcomResD.PglsD.PglsBase[k];
			T_PGRF_BASE& PgrfBase = LcomResD.PgrfD.PgrfBase[k];
			T_PGBS_BASE& PgbsBase = LcomResD.PgbsD.PgbsBase[k];
			T_PLSS_BASE& PlssGd = LcomResD.PlssD.BeforePlss[k];
			T_PLSS_BASE& PlssBase = LcomResD.PlssD.PlssBase[k];
			T_RHOC_BASE& RhoCBase = LcomResD.RhoC.Rho_C[k];


			//여기서 부터 이렇게 하면 될려나?
			if ( bCalcEngine )
			{
				T_PGBS_DETAIL PgbsDetail;
				SetPgbsBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, iServLcomType, aGirderForce, aCptForce, PgbsBase);
				//SetPartStress(k, StressCrc, StressSlab, PgbsBase, PgbsDetail);
			}

			SetCheckPosition(k);
			if ( bCalcEngine ) IDgnCodeCheck::Check_Section(false);
			SetPgbrBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, PgbrGd, PgbrBase);

			// 6. Get Section Class
			if ( !bCalcEngine )
			{
				if ( !Get_ClassifyClassSection(CalcD, aGirderForce, aCptForce) ) ASSERT(0);
			}

			// 7. Flexure Resistance (Bending)
			T_PGBR_DETAIL PgbrDetail;
			if ( !bCalcEngine )
			{
				if ( !CalcD.bIsGenSect ) { Calc_BendingResistance(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
				else { Calc_BendingResistance4GenSect(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
			}
			CalcD.PgbrBase = PgbrBase;
			CalcD.PgbrDetail = PgbrDetail;

			if ( !bCalcEngine )
			{
				SetRhocBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, PgbrDetail, RhoCBase);
			}

			if ( bULSLcom ) // Get Critical Flexure Design Result
			{
				if ( PgbrBase.dMy > -m_dZero )
				{
					if ( iPosiCount[k] == 0 )
					{
						dPosiMaxRatio[k] = PgbrBase.dRatio;
						ResD.PgbrPosiRat.BeforePgbr[k] = PgbrGd;
						ResD.PgbrPosiRat.PgbrBase[k] = PgbrBase;
						ResD.RhocPosiRat.Rho_C[k] = RhoCBase;
					}
					else
					{
						if ( dPosiMaxRatio[k] < PgbrBase.dRatio )
						{
							dPosiMaxRatio[k] = PgbrBase.dRatio;
							ResD.PgbrPosiRat.BeforePgbr[k] = PgbrGd;
							ResD.PgbrPosiRat.PgbrBase[k] = PgbrBase;
							ResD.RhocPosiRat.Rho_C[k] = RhoCBase;
						}
					}
					iPosiCount[k]++;
				}
				else
				{
					if ( iNegaCount[k] == 0 )
					{
						dNegaMaxRatio[k] = PgbrBase.dRatio;
						ResD.PgbrNegaRat.BeforePgbr[k] = PgbrGd;
						ResD.PgbrNegaRat.PgbrBase[k] = PgbrBase;
						ResD.RhocNegaRat.Rho_C[k] = RhoCBase;
					}
					else
					{
						if ( dNegaMaxRatio[k] < PgbrBase.dRatio )
						{
							dNegaMaxRatio[k] = PgbrBase.dRatio;
							ResD.PgbrNegaRat.BeforePgbr[k] = PgbrGd;
							ResD.PgbrNegaRat.PgbrBase[k] = PgbrBase;
							ResD.RhocNegaRat.Rho_C[k] = RhoCBase;
						}
					}
					iNegaCount[k]++;
				}
			}

			// 8. Vertical Shear Resistance
			if ( bULSLcom && ParamD.bCheckStrength[1] )
			{
				T_PRVS_DETAIL PrvsDetail;
				SetPrvsBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, PrvsGd, PrvsBase);
				if ( !bCalcEngine )
				{
					if ( !CalcD.bIsGenSect ) { Calc_VerticalShearResistance(CalcD, PrvsGd, PrvsBase, PrvsDetail); }
					else { Calc_VerticalShearResistance4GenSect(CalcD, PrvsGd, PrvsBase, PrvsDetail); }
				}

				if ( iVCount[k] == 0 )
				{
					dVMaxRatio[k] = PrvsBase.dRatio;
					ResD.PrvsRat.BeforePrvs[k] = PrvsGd;
					ResD.PrvsRat.PrvsBase[k] = PrvsBase;
				}
				else
				{
					if ( dVMaxRatio[k] < PrvsBase.dRatio )
					{
						dVMaxRatio[k] = PrvsBase.dRatio;
						ResD.PrvsRat.BeforePrvs[k] = PrvsGd;
						ResD.PrvsRat.PrvsBase[k] = PrvsBase;
					}
				}
				iVCount[k]++;
			}

			// 9. Lateral Torsional Buckling
			if ( bULSLcom && !CalcD.bIsGenSect && ParamD.bCheckStrength[2] )
			{
				T_PLTB_DETAIL PltbDetail;
				SetPltbBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, dV1, dV2, dM1, dM2, dC4, PltbGd, PltbBase);
				if ( !bCalcEngine )
				{
					Calc_LateralTorsionalBucklingResistance(CalcD, PltbBase, PltbDetail);
				}

				if ( iLTBCount[k] == 0 )
				{
					dLTBMaxRatio[k] = PltbBase.dRatio;
					ResD.PltbRat.BeforePltb[k] = PltbGd;
					ResD.PltbRat.PltbBase[k] = PltbBase;
				}
				else
				{
					if ( dLTBMaxRatio[k] < PltbBase.dRatio )
					{
						dLTBMaxRatio[k] = PltbBase.dRatio;
						ResD.PltbRat.BeforePltb[k] = PltbGd;
						ResD.PltbRat.PltbBase[k] = PltbBase;
					}
				}
				iLTBCount[k]++;
			}

			// 10. Transverse Force Resistance
			if ( bULSLcom && !CalcD.bIsGenSect && ParamD.bCheckStrength[3] )
			{
				SetPrtfBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aForce, PrtfBase);
				// Modify, Jaeoh. MQC. 3797, [1/21/2011]
				BOOL bCalcPrtf = FALSE;
				if ( PrtfBase.bChk )
				{
					if ( !CalcD.bChkData[PGCHK_TYPE::BUCK_COEFF] || (CalcD.bChkData[PGCHK_TYPE::BUCK_COEFF] && !CalcD.BcdtPosi.bUseTfType) )
					{
						PrtfBase.bChk = FALSE;
						bCalcPrtf = FALSE;
					}
					else bCalcPrtf = TRUE;
				}
				else bCalcPrtf = FALSE;

				if ( bCalcPrtf )
				{
					T_PRTF_DETAIL PrtfDetail;
					if ( !bCalcEngine )
					{
						Calc_TransverseForceResistance(CalcD, PrtfBase, PrtfDetail);
					}
					LcomResD.PrtfD.PrtfBase[k] = PrtfBase;
					if ( iRTFCount[k] == 0 ) { dRTFMaxRatio[k] = PrtfBase.dRatio; ResD.PrtfRat.PrtfBase[k] = PrtfBase; }
					else
					{
						if ( dRTFMaxRatio[k] < PrtfBase.dRatio ) { dRTFMaxRatio[k] = PrtfBase.dRatio;  ResD.PrtfRat.PrtfBase[k] = PrtfBase; }
					}
					iRTFCount[k]++;
				}
			}

			// 11. Longitudinal Shear Resistance
			T_PGLS_DETAIL PglsDetail;
			if ( bULSLcom && ParamD.bCheckStrength[4] )
			{
				SetPglsBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, PglsGd, PglsBase);
				if ( !bCalcEngine )
				{
					if ( !CalcD.bIsGenSect ) { Calc_LongitudinalShearResistance(bSLSLcom, CalcD, ParamD.dks, PglsGd, PglsBase, PglsDetail); }
					else { Calc_LongitudinalShearResistance4GenSect(bSLSLcom, CalcD, ParamD.dks, PglsGd, PglsBase, PglsDetail); }
				}
				CalcD.PglsBase = PglsBase;

				// Modify by GAY. PMS:4216. ('12.05.17). 합성전 거더의 설계결과 할당 누락 수정.
				if ( iLSCount[k] == 0 ) { dLSMaxRatio[k] = PglsBase.dRatio; ResD.PglsRat.BeforePgls[k] = PglsGd; ResD.PglsRat.PglsBase[k] = PglsBase; }
				else
				{
					if ( dLSMaxRatio[k] < PglsBase.dRatio ) { dLSMaxRatio[k] = PglsBase.dRatio; ResD.PglsRat.BeforePgls[k] = PglsGd;  ResD.PglsRat.PglsBase[k] = PglsBase; }
				}
				iLSCount[k]++;
			}

			// 12. Fatigue Resistance
			if ( bFatLcom && !CalcD.bIsGenSect && ParamD.bCheckStrength[5] )
			{
				// Fatigue Resistance를 단독으로 사용할 경우 Longitudinal Shear Resistance가 먼저 선행되어야 하기 때문에..
				if ( !CalcD.bIsGenSect )
				{
					SetPglsBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, PglsGd, PglsBase);
					if ( !bCalcEngine )
					{
						Calc_LongitudinalShearResistance(bSLSLcom, CalcD, ParamD.dks, PglsGd, PglsBase, PglsDetail);
					}
					CalcD.PglsBase = PglsBase;
				}

				T_PGRF_DETAIL PgrfDetail;
				SetPgfrBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aForce, PgrfBase);
				if ( !bCalcEngine )
				{
					Calc_FatigueResistance(CalcD, ParamD.dt_Ld, PgrfBase);
				}

				if ( iRFCount[k] == 0 ) { dRFMaxRatio[k] = PgrfBase.dRatio; ResD.PgrfRat.PgrfBase[k] = PgrfBase; }
				else
				{
					if ( dRFMaxRatio[k] < PgrfBase.dRatio ) { dRFMaxRatio[k] = PgrfBase.dRatio;  ResD.PgrfRat.PgrfBase[k] = PgrfBase; }
				}
				iRFCount[k]++;
			}

			// 13. Check Stress
			if ( bSLSLcom && ParamD.bCheckService[0] )
			{
				T_PGBS_DETAIL PgbsDetail;
				SetPgbsBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, iServLcomType, aGirderForce, aCptForce, PgbsBase);
				int nPosIx = nPosi - 1;
				//Get_ConstraintStress(nPosIx, ScfpCr, ScfpSh, ScfpTe, PgbsDetail.StrD);
				SetPartStress(k, StressCrc, StressSlab, PgbsBase, PgbsDetail);
				if ( !bCalcEngine )
				{
					Calc_StressCheck4SLS(CalcD, ParamD.dk1, ParamD.dk2, ParamD.dk3, ForceConst, PgbsBase, PgbsDetail);
				}

				ResD.PgbsRat.PgbsBase[k].bChk = ParamD.bChkIJ[k];
				ResD.PgbsRat.PgbsBase[k].ElemK = ElemK.first;
				ResD.PgbsRat.PgbsBase[k].nPosi = nPosi;
				// Girder
				if ( iServLcomType == 3 ) // Characteristic
				{
					if ( iStressGrSLS_Count[k] == 0 )
					{
						dStressGrMaxRatio[k] = PgbsBase.ChkGirder.dMaxRatio;
						ResD.PgbsRat.PgbsBase[k].ChkGirder = PgbsBase.ChkGirder; //PgbsGrCrD = PgbsBase.ChkGirder;
						ResD.dtau_Ed_max[k] = PgbsBase.ChkGirder.dtau_w;
					}
					else
					{
						if ( dStressGrMaxRatio[k] < PgbsBase.ChkGirder.dMaxRatio )
						{
							dStressGrMaxRatio[k] = PgbsBase.ChkGirder.dMaxRatio;
							ResD.PgbsRat.PgbsBase[k].ChkGirder = PgbsBase.ChkGirder; //PgbsGrCrD = PgbsBase.ChkGirder;
						}
						if ( ResD.dtau_Ed_max[k] < PgbsBase.ChkGirder.dtau_w )
						{
							ResD.dtau_Ed_max[k] = PgbsBase.ChkGirder.dtau_w;
						}
					}
					iStressGrSLS_Count[k]++;
				}
				// Slab
				if ( iServLcomType == 1 || iServLcomType == 3 ) // 1=Quasi, 3=Characteristic
				{
					if ( iStressSbSLS_Count[k] == 0 )
					{
						dStressSbMaxRatio[k] = PgbsBase.ChkSlab.dMaxRatio_sb;
						ResD.PgbsRat.PgbsBase[k].ChkSlab = PgbsBase.ChkSlab; //PgbsSbCrD = PgbsBase.ChkSlab;
					}
					else
					{
						if ( dStressSbMaxRatio[k] < PgbsBase.ChkSlab.dMaxRatio_sb )
						{
							dStressSbMaxRatio[k] = PgbsBase.ChkSlab.dMaxRatio_sb;
							ResD.PgbsRat.PgbsBase[k].ChkSlab = PgbsBase.ChkSlab; //PgbsSbCrD = PgbsBase.ChkSlab;
						}
					}
					iStressSbSLS_Count[k]++;
				}
				// Rebar
				if ( iStressBarSLS_Count[k] == 0 )
				{
					dStressBarMaxRatio[k] = PgbsBase.ChkBar.dMaxRatio_bar;
					ResD.PgbsRat.PgbsBase[k].ChkBar = PgbsBase.ChkBar; //PgbsBarCrD = PgbsBase.ChkBar;
				}
				else
				{
					if ( dStressBarMaxRatio[k] < PgbsBase.ChkBar.dMaxRatio_bar )
					{
						dStressBarMaxRatio[k] = PgbsBase.ChkBar.dMaxRatio_bar;
						ResD.PgbsRat.PgbsBase[k].ChkBar = PgbsBase.ChkBar; //PgbsBarCrD = PgbsBase.ChkBar;
					}
				}
				iStressBarSLS_Count[k]++;
				//
				BOOL bChkGirder = TRUE, bChkSlab = TRUE, bChkRebar = TRUE;
				if ( ResD.PgbsRat.PgbsBase[k].ChkGirder.LcomK != 0 && !ResD.PgbsRat.PgbsBase[k].ChkGirder.bOK ) { bChkGirder = FALSE; }
				if ( ResD.PgbsRat.PgbsBase[k].ChkSlab.LcomK != 0 && !ResD.PgbsRat.PgbsBase[k].ChkSlab.bChk_Stress_sb ) { bChkSlab = FALSE; }
				if ( ResD.PgbsRat.PgbsBase[k].ChkBar.LcomK != 0 && !ResD.PgbsRat.PgbsBase[k].ChkBar.bChk_Stress_bar ) { bChkRebar = FALSE; }

				if ( bChkGirder && bChkSlab && bChkRebar ) { ResD.PgbsRat.PgbsBase[k].bOK = TRUE; }
				else { ResD.PgbsRat.PgbsBase[k].bOK = FALSE; }
			}

			// 14. Check Longitudianl Shear for SLS
			if ( bSLSLcom && ParamD.bCheckService[1] )
			{
				T_PLSS_DETAIL PlssDetail;
				SetPlssBaseData(ParamD.bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, iServLcomType, aGirderForce, aCptForce, PlssGd, PlssBase);
				if ( !bCalcEngine )
				{
					Calc_LongitudinalShearResistance4SLS(CalcD, ParamD.dks, PlssGd, PlssBase, PlssDetail);
				}

				// Modify by GAY. PMS:4216. ('12.05.17). 합성전 거더의 설계결과 할당 누락 수정.
				if ( iLS_SLS_Count[k] == 0 ) { dLS_SLSMaxRatio[k] = PlssBase.dRatio; ResD.PlssRat.BeforePlss[k] = PlssGd;  ResD.PlssRat.PlssBase[k] = PlssBase; }
				else
				{
					if ( dLS_SLSMaxRatio[k] < PlssBase.dRatio ) { dLS_SLSMaxRatio[k] = PlssBase.dRatio; ResD.PlssRat.BeforePlss[k] = PlssGd;  ResD.PlssRat.PlssBase[k] = PlssBase; }
				}
				iLS_SLS_Count[k]++;
			}
		}

		g_num_mutex.unlock();
	}

	return true;
}

BOOL CDgnPlateGirder::Get_PlateGirderDgnResult(BOOL* abChkItem)
{
	IDgnMemoizationLib::Instance()->CreateMemo(IDgnMemoizationLib::EN_MEMO_PLASTIC_MOMENT_NA);

	//////////////////////////////////////////////////////////////////////////
	// 1. Get General Input Data
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	CPlateGirderDesign* pPGD = pDoc->m_pPostCtrl->GetPlateGirder();
	ASSERT(pPGD);

	pDoc->SetDesignStatus(6);
	for ( int i = 0; i < D_CPG_CHECK_ITEM; i++ ) { abChkItem[i] = FALSE; }

	system_clock::time_point start_time = system_clock::now();

	T_PLATE_GIRDER_PARAM_D ParamD;
	{
		T_CPGD_D CpgdD;
		if ( !pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD) ) { ASSERT(0); }

		for ( int i = 0; i < 6; i++ )
		{
			ParamD.bCheckStrength[i] = CpgdD.bStrength[i];
		}
		for ( int i = 0; i < 2; i++ )
		{
			ParamD.bCheckService[i] = CpgdD.bService[i];
		}

		ParamD.dt_Ld = CpgdD.dt_Ld;
		ParamD.dk1 = CpgdD.dk1;
		ParamD.dk2 = CpgdD.dk2;
		ParamD.dk3 = CpgdD.dk3;
		ParamD.dks = CpgdD.dks;
		ParamD.bIsIDesignModule = CDgnCodeCtrl::IsIDesignModule(m_iCivilCode);
		ParamD.iLcomNum = m_pForcCtrl->Get_LcomDataCount();
	}

	//////////////////////////////////////////////////////////////////////////
	// 2. Repeat to check element
	ArrElemPairKey arCheckElemK;
	m_pDataCtrl->GetPgdElemKeyList(arCheckElemK);
	int iElemSize = arCheckElemK.GetSize();

	m_nThread = /*ParamD.bIsIDesignModule ? 1 :*/ CDgnThreadPool::GetNumberOfThreads();
	dgn::lib::CDgnThreadPool threadpool(m_nThread);
	std::vector<std::future<BOOL>> futures(iElemSize);

	int iStrnLcomSize = m_pForcCtrl->Get_LcomNumForStrn();
	int iSLSTypeSize = m_pForcCtrl->Get_LcomNumForSLS(); // Sub Lcom 대화상자에서 수정한 하중조합
	int iServLcomTypeSize = m_pForcCtrl->Get_LcomNumForStrs();

	m_svRes.clear();
	m_ElemToLcom.clear();
	CDgnProgressDlg ProDlg;
	ProDlg.Create_ProgressDlgCVL(PROGRESS_TYPE_STEEL_COMPOSITE);
	
	//결과 구조체에 빈 사이즈 미리 잡아주기도 하고, 멀티쓰레딩을 하기 위해 부재력을 map에 올린다.
	for ( int i = 0; i < iElemSize; ++i )
	{
		auto ElemK = arCheckElemK[i];

		int nFortype = 0;
		_DGN_FORC_CRC_ELEM ForceLcom;
		if ( !m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, ForceLcom.aForceLcom, ParamD.iLcomNum) ) { ASSERT(0); continue; }
		m_mForce[ElemK] = std::move(ForceLcom);

		_DGN_STRS_CRC_ELEM StressLcom;
		ADGNSTRESS aStressTnpr;
		if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, nFortype, TRUE, FALSE, StressLcom.aStressLcom, aStressTnpr, ParamD.iLcomNum) ) { return false; }
		m_mStress[ElemK] = std::move(StressLcom);

		nFortype = 2;
		_DGN_STRS_CRC_ELEM StressLcomP2;
		ADGNSTRESS aStressTnprP2;
		if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, nFortype, TRUE, FALSE, StressLcomP2.aStressLcom, aStressTnprP2, ParamD.iLcomNum) ) { return false; }
		m_mStressP2[ElemK] = std::move(StressLcomP2);

		ElemPairK showElemK = { 0,0 }, IendElemK = { 0,0 }, JendElemK = { 0,0 };
		GetEndElemOfMember(ElemK, showElemK, IendElemK, JendElemK);

		nFortype = 0;
		ForceLcom.Initialize();
		if ( m_pForcCtrl->Get_LcomDataForDesign(IendElemK, nFortype, TRUE, FALSE, ForceLcom.aForceLcom, ParamD.iLcomNum) )
		{ 
			m_mForce[IendElemK] = std::move(ForceLcom);
		}
		else ASSERT(0);
			
		ForceLcom.Initialize();
		if ( m_pForcCtrl->Get_LcomDataForDesign(JendElemK, nFortype, TRUE, FALSE, ForceLcom.aForceLcom, ParamD.iLcomNum) )
		{
			m_mForce[JendElemK] = std::move(ForceLcom);
		}
		else ASSERT(0);

		_DGN_SCFP_PART_ELEM ScfpCr, ScfpSh, ScfpTe, ScfpRe;
		if ( !m_pForcCtrl->Get_LcomStressScfp(ElemK, ParamD.iLcomNum, ScfpCr.aScfpStress, ScfpSh.aScfpStress, ScfpTe.aScfpStress, ScfpRe.aScfpStress) ) return FALSE;  // Get Internal Stress
		m_mScfpCr[ElemK] = ScfpCr;
		m_mScfpSh[ElemK] = ScfpSh;
		m_mScfpTe[ElemK] = ScfpTe;

		T_PLATE_GIRDER_RES_D* pRes = new T_PLATE_GIRDER_RES_D;
		int nSize = ForceLcom.aForceLcom.GetSize();
		pRes->svLcomRes.resize(nSize);
		m_svRes[ElemK] = pRes;
		
		for ( int j = 0; j<nSize; ++j )
		{
			CPG_FORC_STAG ForceStage;
			ForceStage.Initialize();
			T_STCT_D StctD;
			m_pDoc->m_pAttrCtrl->GetStct(StctD);
			//Girder Force
			T_STAG_K nGdLastStagK = 0;
			int nLastStepNo = 0;
			m_pForcCtrl->GetGirderLastStageKey(ElemK, nGdLastStagK, nLastStepNo);
			int iDgnLcomNo = j+1;
			if ( !m_pDataCtrl->Get_GirderCSLoadOfLCOM(StctD, ElemK, nGdLastStagK, nLastStepNo, iDgnLcomNo, ForceStage) ) { ASSERT(0); }
			m_mStageForce[std::make_pair(ElemK, iDgnLcomNo)] = ForceStage;
		}
	}

	int nProgress = 0;
	BOOL bChkIJ[2] = { FALSE, FALSE };
	for ( int i = 0; i < iElemSize; ++i )
	{
		auto ElemK = arCheckElemK[i];

		ParamD.bChkIJ[0] = m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, 1);
		ParamD.bChkIJ[1] = m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, 2);
		SetCheckElem(ElemK.first);

		if ( ParamD.bChkIJ[0] || ParamD.bChkIJ[1] )
		{
			if ( iStrnLcomSize > 0 )
			{
				for ( int j = 0; j < 6; ++j )
				{
					if ( ParamD.bCheckStrength[j] ) { abChkItem[j] = TRUE; }
				}
			}

			if ( iSLSTypeSize > 0 )
			{
				if ( ParamD.bCheckService[0] ) { abChkItem[6] = TRUE; }
			}

			if ( iServLcomTypeSize > 0 )
			{
				if ( ParamD.bCheckService[1] ) { abChkItem[7] = TRUE; }
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 3. Get Design Data
		auto lambdaCalc = [this] (auto ElemK, auto& ParamD, T_PLATE_GIRDER_RES_D* pResD)
		{
			return Calc_PlateGirderElem(ElemK, ParamD, *pResD);
		};

		futures[i] = threadpool.EnqueueTask(lambdaCalc, ElemK, ParamD, m_svRes[ElemK]);
	}

	for ( int i = 0; i < iElemSize; ++i )
	{
		futures[i].wait();

		if ( !futures[i].get() )
			continue;

		int iMaxMin = 0;

		auto ElemK = arCheckElemK[i];
		T_PLATE_GIRDER_RES_D* ResD = m_svRes[ElemK];

		for ( auto& LcomResD : ResD->svLcomRes )
		{
			int iDgnLcomNo = LcomResD.iDgnLcomNo;
			// 15. Save Design Result for Each Design Lcom
			{
				if ( LcomResD.bULSLcom )
				{
					// Bending Resistance
					// if(bCheckStrength[0]) Bending은 다른 검토에서도 필요하기 때문에 결과를 쓴다. 
					if ( !pPGD->WriteBendingResistanceResult(ElemK, LcomResD.iDgnLcomNo, iMaxMin, LcomResD.PgbrLcomD) ) { ASSERT(0); }

					// Vertical Shear Resistance
					if ( ParamD.bCheckStrength[1] ) { if ( !pPGD->WriteVerticalShearResistanceResult(ElemK, iDgnLcomNo, iMaxMin, LcomResD.PrvsD) ) { ASSERT(0); } }
					// Lateral Torsional Buckling
					if ( ParamD.bCheckStrength[2] ) { if ( !pPGD->WriteLateralTorsionalBucklingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, LcomResD.PltbD) ) { ASSERT(0); } }
					// Transverse Force Resistance
					if ( ParamD.bCheckStrength[3] ) { if ( !pPGD->WriteTransverseForceResistanceResult(ElemK, iDgnLcomNo, iMaxMin, LcomResD.PrtfD) ) { ASSERT(0); } }
					// Longitudinal Shear Resistance
					if ( ParamD.bCheckStrength[4] ) { if ( !pPGD->WriteLongitudinalShearResistanceResult(ElemK, iDgnLcomNo, iMaxMin, LcomResD.PglsD) ) { ASSERT(0); } }
					// Fatigue Resistance
					if ( ParamD.bCheckStrength[5] ) { if ( !pPGD->WriteFatigueResistanceResult(ElemK, iDgnLcomNo, iMaxMin, LcomResD.PgrfD) ) { ASSERT(0); } }
				}

				if ( LcomResD.bSLSLcom )
				{
					if ( ParamD.bCheckService[0] ) { if ( !pPGD->WriteBeamStressResistanceResult(ElemK, iDgnLcomNo, iMaxMin, LcomResD.PgbsD) ) { ASSERT(0); } }
					//
					if ( ParamD.bCheckService[1] ) { if ( !pPGD->WriteLongitudinalShearResistanceServiceResult(ElemK, iDgnLcomNo, iMaxMin, LcomResD.PlssD) ) { ASSERT(0); } }
				}
			}
		}


		// Positive, Min Ratio
		int iDgnLcomNo = 0;

		// Critical for each Element
		for ( int j = 0; j < 2; ++j )
		{
			if ( ResD->PgbsRat.PgbsBase[j].bChk == FALSE ) continue;
			const T_PGBS_GR_BASE &PgbsB = ResD->PgbsRat.PgbsBase[j].ChkGirder;
			double dStressComb2 = pow(PgbsB.dMaxStress, 2.0) + 3.0*pow(ResD->dtau_Ed_max[j], 2.0);
			double dStressComb = sqrt(dStressComb2);
			BOOL bChkStressComb = dStressComb > PgbsB.dAlw_stress_cb ? FALSE : TRUE;
			ResD->PgbsRat.PgbsBase[j].ChkGirder.dtau_w = ResD->dtau_Ed_max[j];
			ResD->PgbsRat.PgbsBase[j].ChkGirder.bChk_tau = (ResD->dtau_Ed_max[j] < ResD->PgbsRat.PgbsBase[j].ChkGirder.dAlw_tau);
			ResD->PgbsRat.PgbsBase[j].ChkGirder.dStress_cb = dStressComb;
			ResD->PgbsRat.PgbsBase[j].ChkGirder.bChk_Stress_cb = bChkStressComb;
		}
		//////////////////////////////////

		iMaxMin = 1;
		if ( !pPGD->WriteBendingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, ResD->PgbrPosiRat) ) { ASSERT(0); }
		if ( !pPGD->WriteBendingRhocResult(ElemK, iDgnLcomNo, iMaxMin, ResD->RhocPosiRat) ) { ASSERT(0); }

		// Negative, Min Ratio
		iMaxMin = 2;
		if ( !pPGD->WriteBendingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, ResD->PgbrNegaRat) ) { ASSERT(0); }
		if ( !pPGD->WriteBendingRhocResult(ElemK, iDgnLcomNo, iMaxMin, ResD->RhocNegaRat) ) { ASSERT(0); }

		iMaxMin = 1;
		if ( ParamD.bCheckStrength[1] ) { if ( !pPGD->WriteVerticalShearResistanceResult(ElemK, iDgnLcomNo, iMaxMin, ResD->PrvsRat) ) { ASSERT(0); } }
		if ( ParamD.bCheckStrength[2] ) { if ( !pPGD->WriteLateralTorsionalBucklingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, ResD->PltbRat) ) { ASSERT(0); } }
		if ( ParamD.bCheckStrength[3] ) { if ( !pPGD->WriteTransverseForceResistanceResult(ElemK, iDgnLcomNo, iMaxMin, ResD->PrtfRat) ) { ASSERT(0); } }
		if ( ParamD.bCheckStrength[4] ) { if ( !pPGD->WriteLongitudinalShearResistanceResult(ElemK, iDgnLcomNo, iMaxMin, ResD->PglsRat) ) { ASSERT(0); } }
		if ( ParamD.bCheckStrength[5] ) { if ( !pPGD->WriteFatigueResistanceResult(ElemK, iDgnLcomNo, iMaxMin, ResD->PgrfRat) ) { ASSERT(0); } }
		if ( ParamD.bCheckService[0] ) { if ( !pPGD->WriteBeamStressResistanceResult(ElemK, iDgnLcomNo, iMaxMin, ResD->PgbsRat) ) { ASSERT(0); } }
		if ( ParamD.bCheckService[1] ) { if ( !pPGD->WriteLongitudinalShearResistanceServiceResult(ElemK, iDgnLcomNo, iMaxMin, ResD->PlssRat) ) { ASSERT(0); } }

		ProDlg.Update_ProgressDlg(nProgress++, iElemSize);
	}

	for ( int i = 0; i<arCheckElemK.GetSize(); ++i )
	{
		auto itr = m_svRes.find(arCheckElemK[i]);
		if ( itr == m_svRes.end() )
		{
			ASSERT(0); continue;
		}
		_SAFE_DELETE(itr->second);
	}

	ProDlg.Delete_ProgressDlg();
	system_clock::time_point end_time = system_clock::now();
	milliseconds mill = duration_cast<milliseconds>(end_time - start_time);

	IDgnMemoizationLib::Instance()->DeleteMemo(IDgnMemoizationLib::EN_MEMO_PLASTIC_MOMENT_NA);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_BendingResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGBR_BASE& PgbrGd,
	const T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail)
{
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;
	if ( !Set_CalcDataByPos(CalcD, bPosiI ? 0 : 1) ) { ASSERT(0); return FALSE; }

	CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
	if ( !m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE) ) ASSERT(0);

	int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
	int nFortype = 0;
	ADGNFORCE aForceLcom;
	if ( !m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum) ) { return FALSE; }
	if ( m_pForcCtrl->Get_LcomNumForStrn() == 0 ) { return FALSE; }

	ADGNSTRESS aStressLcom;
	ADGNSTRSADD aStressAdd;
	if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum) ) { return FALSE; }
	Get_CompositeGeneralSectionInfo(CalcD);

	int iDgnLcomNo = 0;
	int iOrgLcomNo = 0;
	int iSerLcomCount = 0;
	int iMaxMinType = 0;
	CString strOrgLcomNa = _T("");

	iDgnLcomNo = PgbrBase.LcomK;
	if ( !m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType) )
	{
		ASSERT(0);
		return FALSE;
	}

	double aForceGd[6] = { 0.0 };
	double aForce[6] = { 0.0 };
	aForceGd[0] = PgbrGd.dFx;
	aForceGd[4] = PgbrGd.dMy;
	aForceGd[5] = PgbrGd.dMz;
	aForce[0] = PgbrBase.dFx;
	aForce[4] = PgbrBase.dMy;
	aForce[5] = PgbrBase.dMz;

	int k = bPosiI ? 0 : 1;
	_DGN_FORC_CRC ForceConst;
	m_pForcCtrl->ConvertDgnPSCForceToDgnForce(k, PgbrBase.ForceConst, ForceConst);	

	_DGN_SCFP_PART_LCASE ScfpSum;
	m_pForcCtrl->Get_ScfpToDgnScfp(PgbrBase.ScfpForce, ScfpSum.ForceP1);

	SetScfpForce4Lcom(ForceConst, ScfpSum);

	Get_ClassifyClassSection(CalcD, aForceGd, aForce);

	T_PGBR_BASE TempPgbrGd = PgbrGd;
	T_PGBR_BASE TempPgbrBase = PgbrBase;
	if ( !CalcD.bIsGenSect ) { Calc_BendingResistance(CalcD, TempPgbrGd, TempPgbrBase, PgbrDetail); }
	else { Calc_BendingResistance4GenSect(CalcD, TempPgbrGd, TempPgbrBase, PgbrDetail); }

	return TRUE;
}

BOOL CDgnPlateGirder::Get_VerticalShearResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PRVS_BASE& PrvsGd, const T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;
	if ( !Set_CalcDataByPos(CalcD, bPosiI ? 0 : 1) ) { ASSERT(0); return FALSE; }

	CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
	if ( !m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE) ) { ASSERT(0); }

	int nFortype = 0;
	int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
	ADGNFORCE aForceLcom;
	if ( !m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum) ) { return FALSE; }
	if ( m_pForcCtrl->Get_LcomNumForStrn() == 0 )	return FALSE;

	ADGNSTRESS aStressLcom;
	ADGNSTRSADD aStressAdd;
	if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum) ) { return FALSE; }
	Get_CompositeGeneralSectionInfo(CalcD);

	int iMaxMin = 0;
	int iDgnLcomNo = 0;
	int iOrgLcomNo = 0;
	int iSerLcomCount = 0;
	int iMaxMinType = 0;
	CString strOrgLcomNa = _T("");

	iDgnLcomNo = PrvsBase.LcomK;
	if ( !m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType) )
	{
		ASSERT(0);
		return FALSE;
	}

	// Bending 검토가 선행되어야 한다. 
	T_PGBR_LCOM PgbrLcomD;
	if ( !m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgbrLcomD) ) { return FALSE; }

	int i = CalcD.iPos;
	T_PGBR_BASE PgbrGd = PgbrLcomD.BeforePgbr[i];
	T_PGBR_BASE PgbrBase = PgbrLcomD.PgbrBase[i];
	T_PGBR_DETAIL PgbrDetail;

	double aForceGd[6] = { 0.0 };
	double aForce[6] = { 0.0 };
	aForceGd[0] = PgbrGd.dFx;
	aForceGd[4] = PgbrGd.dMy;
	aForceGd[5] = PgbrGd.dMz;
	aForce[0] = PgbrBase.dFx;
	aForce[4] = PgbrBase.dMy;
	aForce[5] = PgbrBase.dMz;

	Get_ClassifyClassSection(CalcD, aForceGd, aForce);

	if ( !CalcD.SectElem.bGenSect ) { Calc_BendingResistance(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
	else { Calc_BendingResistance4GenSect(CalcD, PgbrGd, PgbrBase, PgbrDetail); }

	T_PLATE_GIRDER_LCOM_RES_D LcomResD;
	LcomResD.PgbrLcomD.BeforePgbr[i] = PgbrGd;
	LcomResD.PgbrLcomD.PgbrBase[i] = PgbrBase;
	CalcD.PgbrBase = PgbrBase;
	CalcD.PgbrDetail = PgbrDetail;

	T_PRVS_BASE TempPrvsGd = PrvsGd;
	T_PRVS_BASE TempPrvsBase = PrvsBase;
	if ( !CalcD.SectElem.bGenSect ) { Calc_VerticalShearResistance(CalcD, TempPrvsGd, TempPrvsBase, PrvsDetail); }
	else { Calc_VerticalShearResistance4GenSect(CalcD, TempPrvsGd, TempPrvsBase, PrvsDetail); }

	return TRUE;
}

BOOL CDgnPlateGirder::Get_LateralTorsionalBucklingResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PLTB_BASE& PltbGd, const T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail)
{
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;
	if ( !Set_CalcDataByPos(CalcD, bPosiI ? 0 : 1) ) { ASSERT(0); return FALSE; }

	CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
	if ( !m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE) ) { ASSERT(0); }

	int nFortype = 0;
	int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
	ADGNFORCE aForceLcom;
	if ( !m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum) ) { return FALSE; }
	if ( m_pForcCtrl->Get_LcomNumForStrn() == 0 ) { return FALSE; }

	ADGNSTRESS aStressLcom;
	ADGNSTRSADD aStressAdd;
	if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum) ) { return FALSE; }

	Get_CompositeGeneralSectionInfo(CalcD);

	int iMaxMin = 0;
	int iDgnLcomNo = 0;
	int iOrgLcomNo = 0;
	int iSerLcomCount = 0;
	int iMaxMinType = 0;
	CString strOrgLcomNa = _T("");

	iDgnLcomNo = PltbBase.LcomK;
	if ( !m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType) )
	{
		ASSERT(0);
		return FALSE;
	}

	int k = bPosiI ? 0 : 1;
	// Bending 검토가 선행되어야 한다. 
	T_PGBR_LCOM PgbrLcomD;
	if ( !m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgbrLcomD) ) { return FALSE; }

	T_PGBR_BASE PgbrGd = PgbrLcomD.BeforePgbr[k];
	T_PGBR_BASE PgbrBase = PgbrLcomD.PgbrBase[k];

	double aForceGd[6] = { 0.0 };
	double aForce[6] = { 0.0 };
	aForceGd[0] = PgbrGd.dFx;
	aForceGd[4] = PgbrGd.dMy;
	aForceGd[5] = PgbrGd.dMz;
	aForce[0] = PgbrBase.dFx;
	aForce[4] = PgbrBase.dMy;
	aForce[5] = PgbrBase.dMz;

	Get_ClassifyClassSection(CalcD, aForceGd, aForce);

	T_PGBR_DETAIL PgbrDetail;
	if ( !CalcD.bIsGenSect ) { Calc_BendingResistance(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
	else { Calc_BendingResistance4GenSect(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
	CalcD.PgbrBase = PgbrBase;
	CalcD.PgbrDetail = PgbrDetail;

	T_PLTB_BASE TempPltbBase = PltbBase;
	Calc_LateralTorsionalBucklingResistance(CalcD, TempPltbBase, PltbDetail);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_TransverseForceResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PRTF_BASE& PrtfBase, T_PRTF_DETAIL& PrtfDetail)
{
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;
	if ( !Set_CalcDataByPos(CalcD, bPosiI ? 0 : 1) ) { ASSERT(0); return FALSE; }

	CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
	if ( !m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE) ) { ASSERT(0); }
	Get_CompositeGeneralSectionInfo(CalcD);

	int iMaxMin = 0;
	int iDgnLcomNo = 0;
	int iOrgLcomNo = 0;
	int iSerLcomCount = 0;
	int iMaxMinType = 0;
	CString strOrgLcomNa = _T("");

	iDgnLcomNo = PrtfBase.LcomK;
	if ( !m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType) )
	{
		ASSERT(0);
		return FALSE;
	}

	int k = CalcD.iPos;
	// Bending 검토가 선행되어야 한다. 
	T_PGBR_LCOM PgbrLcomD;
	if ( !m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgbrLcomD) ) { return FALSE; }

	T_PGBR_BASE PgbrGd = PgbrLcomD.BeforePgbr[k];
	T_PGBR_BASE PgbrBase = PgbrLcomD.PgbrBase[k];

	double aForceGd[6] = { 0.0 };
	double aForce[6] = { 0.0 };
	aForceGd[0] = PgbrGd.dFx;
	aForceGd[4] = PgbrGd.dMy;
	aForceGd[5] = PgbrGd.dMz;
	aForce[0] = PgbrBase.dFx;
	aForce[4] = PgbrBase.dMy;
	aForce[5] = PgbrBase.dMz;

	Get_ClassifyClassSection(CalcD, aForceGd, aForce);

	T_PGBR_DETAIL PgbrDetail;
	if ( !CalcD.bIsGenSect ) { Calc_BendingResistance(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
	else { Calc_BendingResistance4GenSect(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
	CalcD.PgbrBase = PgbrBase;
	CalcD.PgbrDetail = PgbrDetail;

	T_PRTF_BASE TempPrtfBase = PrtfBase;
	Calc_TransverseForceResistance(CalcD, TempPrtfBase, PrtfDetail);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_LongitudinalShearResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGLS_BASE& PglsGd, const T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail)
{
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;
	if ( !Set_CalcDataByPos(CalcD, bPosiI ? 0 : 1) ) { ASSERT(0); return FALSE; }

	CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
	if ( !m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE) ) { ASSERT(0); }

	int nFortype = 0;
	int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
	ADGNFORCE aForceLcom;
	if ( !m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum) ) { return FALSE; }

	ADGNSTRESS aStressLcom;
	ADGNSTRSADD aStressAdd;
	if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum) ) { return FALSE; }
	Get_CompositeGeneralSectionInfo(CalcD);

	int iMaxMin = 0;
	int iDgnLcomNo = 0;
	int iOrgLcomNo = 0;
	int iSerLcomCount = 0;
	int iMaxMinType = 0;
	CString strOrgLcomNa = _T("");

	CalcD.GLineDgn.Initialize();
	CalcD.GLineData.Initialize();
	CalcD.ClassBase.Initialize();

	iDgnLcomNo = PglsBase.LcomK;
	if ( !m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType) )
	{
		ASSERT(0);
		return FALSE;
	}

	// Bending 검토가 선행되어야 한다. 
	T_PGBR_LCOM PgbrLcomD;
	if ( !m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgbrLcomD) ) { return FALSE; }

	T_PGBR_BASE PgbrGd = PgbrLcomD.BeforePgbr[CalcD.iPos];
	T_PGBR_BASE PgbrBase = PgbrLcomD.PgbrBase[CalcD.iPos];

	double aForceGd[6] = { 0.0 };
	double aForce[6] = { 0.0 };
	aForceGd[0] = PgbrGd.dFx;
	aForceGd[4] = PgbrGd.dMy;
	aForceGd[5] = PgbrGd.dMz;
	aForce[0] = PgbrBase.dFx;
	aForce[4] = PgbrBase.dMy;
	aForce[5] = PgbrBase.dMz;

	Get_ClassifyClassSection(CalcD, aForceGd, aForce);

	T_PGBR_DETAIL PgbrDetail;
	if ( !CalcD.bIsGenSect ) { Calc_BendingResistance(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
	else { Calc_BendingResistance4GenSect(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
	CalcD.PgbrBase = PgbrBase;
	CalcD.PgbrDetail = PgbrDetail;

	T_PGLS_BASE TempPglsGd = PglsGd;
	T_PGLS_BASE TempPglsBase = PglsBase;
	if ( !CalcD.bIsGenSect ) { Calc_LongitudinalShearResistance(TRUE, CalcD, m_CpgdD.dks, TempPglsGd, TempPglsBase, PglsDetail); }
	else { Calc_LongitudinalShearResistance4GenSect(TRUE, CalcD, m_CpgdD.dks, TempPglsGd, TempPglsBase, PglsDetail); }

	return TRUE;
}

BOOL CDgnPlateGirder::Get_FatigueResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGRF_BASE& PgrfBase, T_PGRF_DETAIL& PgrfDetail)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	T_CPGD_D CpgdD; CpgdD.Initialize();
	if ( !pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD) ) { ASSERT(0); }
	double dt_Ld = CpgdD.dt_Ld;

	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;
	if ( !Set_CalcDataByPos(CalcD, bPosiI ? 0 : 1) ) { ASSERT(0); return FALSE; }

	CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
	if ( !m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE) ) { ASSERT(0); }

	int nFortype = 0;
	int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
	ADGNFORCE aForceLcom;
	if ( !m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum) ) { return FALSE; }
	if ( m_pForcCtrl->Get_LcomNumForStrn() == 0 ) { return FALSE; }

	ADGNSTRESS aStressLcom;
	ADGNSTRSADD aStressAdd;
	if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum) ) { return FALSE; }
	Get_CompositeGeneralSectionInfo(CalcD);

	int iMaxMin = 0;
	int iDgnLcomNo = 0;
	int iOrgLcomNo = 0;
	int iSerLcomCount = 0;
	int iMaxMinType = 0;
	CString strOrgLcomNa = _T("");

	iDgnLcomNo = PgrfBase.LcomK;
	if ( !m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType) )
	{
		ASSERT(0);
		return FALSE;
	}

	// Bending 검토가 선행되어야 한다. 
	T_PGBR_LCOM PgbrLcomD;
	if ( !m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgbrLcomD) ) { return FALSE; }

	T_PGBR_BASE PgbrGd = PgbrLcomD.BeforePgbr[CalcD.iPos];
	T_PGBR_BASE PgbrBase = PgbrLcomD.PgbrBase[CalcD.iPos];

	double aForceGd[6] = { 0.0 };
	double aForce[6] = { 0.0 };
	aForceGd[0] = PgbrGd.dFx;
	aForceGd[4] = PgbrGd.dMy;
	aForceGd[5] = PgbrGd.dMz;
	aForce[0] = PgbrBase.dFx;
	aForce[4] = PgbrBase.dMy;
	aForce[5] = PgbrBase.dMz;

	Get_ClassifyClassSection(CalcD, aForceGd, aForce);

	T_PGBR_DETAIL PgbrDetail;
	if ( !CalcD.bIsGenSect ) { Calc_BendingResistance(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
	else { Calc_BendingResistance4GenSect(CalcD, PgbrGd, PgbrBase, PgbrDetail); }
	CalcD.PgbrBase = PgbrBase;
	CalcD.PgbrDetail = PgbrDetail;

	T_PGLS_D PglsD;
	T_PGLS_BASE PglsGd;
	T_PGLS_BASE PglsBase;
	T_PGLS_DETAIL PglsDetail;
	if ( m_pAnalysisResult->GetCpgLongitudinalShearResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PglsD) )
	{
		CalcD.PglsBase = PglsD.PglsBase[CalcD.iPos];
	}
	else
	{
		if ( !CalcD.bIsGenSect ) Calc_LongitudinalShearResistance(TRUE, CalcD, m_CpgdD.dks, PglsGd, PglsBase, PglsDetail);
		CalcD.PglsBase = PglsBase;
	}

	T_PGRF_BASE TempPgrfBase = PgrfBase;
	Calc_FatigueResistance(CalcD, dt_Ld, TempPgrfBase);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_BeamStressDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGBS_BASE& PgbsBase, T_PGBS_DETAIL& PgbsDetail)
{
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;
	if ( !Set_CalcDataByPos(CalcD, bPosiI ? 0 : 1) ) { ASSERT(0); return FALSE; }

	CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
	if ( !m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE) ) { ASSERT(0); }

	int nFortype = 0;
	int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
	ADGNFORCE aForceLcom;
	if ( !m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum) ) { return FALSE; }
	if ( m_pForcCtrl->Get_LcomNumForStrs() == 0 ) { return FALSE; }

	ADGNSTRESS aStressLcom;
	ADGNSTRESS aStressTnpr;
	if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, nFortype, TRUE, FALSE, aStressLcom, aStressTnpr, iLcomNum) ) { return FALSE; }

	nFortype = 2;
	ADGNSTRESS aStressLcomP2;
	ADGNSTRESS aStressTnprP2;
	if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, nFortype, TRUE, FALSE, aStressLcomP2, aStressTnprP2, iLcomNum) ) { return FALSE; }

	ADGNFORCE aForceConst;
	m_pForcCtrl->Get_LcomForce4ScfpSum(ElemK, iLcomNum, aForceConst);

	ASCFPSTRESS aScfpCr, aScfpSh, aScfpTe, aScfpRe;
	bool bExistCrp = false, bExistShr = false, bExistBST = false;
	if ( !m_pForcCtrl->Get_LcomStressScfp(ElemK, iLcomNum, aScfpCr, aScfpSh, aScfpTe, aScfpRe) ) return FALSE;  // Get Internal Stress

	Get_CompositeGeneralSectionInfo(CalcD, TRUE);

	int iMaxMin = 0;
	int iDgnLcomNo = 0;
	int iOrgLcomNo = 0;
	int iSerLcomCount = 0;
	int iMaxMinType = 0;
	CString strOrgLcomNa = _T("");

	iDgnLcomNo = PgbsBase.ChkGirder.LcomK;
	if ( !m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType) )
	{
		ASSERT(0);
		return FALSE;
	}
	int nDgnLcomIdx = iDgnLcomNo - 1;
	const _DGN_FORC_CRC& ForceCrc = aForceLcom[nDgnLcomIdx];
	const _DGN_STRS_CRC& StressCrc = aStressLcom[nDgnLcomIdx];
	const _DGN_STRS_CRC& StressP2 = aStressLcomP2[nDgnLcomIdx];

	const _DGN_FORC_CRC& ForceConst = aForceConst[nDgnLcomIdx];
	_DGN_SCFP_PART_LCASE& ScfpCr = aScfpCr[nDgnLcomIdx];
	_DGN_SCFP_PART_LCASE& ScfpSh = aScfpSh[nDgnLcomIdx];
	_DGN_SCFP_PART_LCASE& ScfpTe = aScfpTe[nDgnLcomIdx];

	m_pForcCtrl->Get_SfcpExist(D_LCOMTYPE_STEEL, iOrgLcomNo, bExistCrp, bExistShr, bExistBST);
	if ( !bExistCrp ) ScfpCr.Initialize();
	if ( !bExistShr ) ScfpSh.Initialize();
	if ( !bExistBST ) ScfpTe.Initialize();

	int k = bPosiI ? 0 : 1;
	double dMuy = ForceCrc.dMuy[k];
	double dMa_Ed = ForceStage.MaxForce.dMuy[k];
	double aForce_Gr[6] = { ForceStage.MaxForce.dFxx[k], ForceStage.MaxForce.dFyy[k], ForceStage.MaxForce.dFzz[k],
		ForceStage.MaxForce.dMux[k], ForceStage.MaxForce.dMuy[k], ForceStage.MaxForce.dMuz[k] };
	double aForce[6] = { ForceCrc.dFxx[k], ForceCrc.dFyy[k], ForceCrc.dFzz[k],
		ForceCrc.dMux[k], ForceCrc.dMuy[k], ForceCrc.dMuz[k] };
	double aStress[4] = { StressCrc.dStl[k],  StressCrc.dStr[k],  StressCrc.dSbr[k],  StressCrc.dSbl[k] };

	if ( dMuy > -m_dZero ) // 정모멘트 인데 Stage 모멘트가 부모멘트면 0으로..
	{
		if ( dMa_Ed < 0.0 ) { dMa_Ed = 0.0; }
	}
	else
	{
		if ( dMa_Ed > 0.0 ) { dMa_Ed = 0.0; }
	}
	Get_ClassifyClassSection(CalcD, aForce_Gr, aForce);

	T_PGBS_BASE TempPgbsBase = PgbsBase;
	SetPartStress(k, StressCrc, StressP2, TempPgbsBase, PgbsDetail);
	Calc_StressCheck4SLS(CalcD, m_CpgdD.dk1, m_CpgdD.dk2, m_CpgdD.dk3, ForceConst, TempPgbsBase, PgbsDetail);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_LongitudinalShearResistanceServiceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PLSS_BASE& PlssGd, const T_PLSS_BASE& PlssBase, T_PLSS_DETAIL& PlssDetail)
{
	T_PLATE_GIRDER_CALC_D CalcD;
	if ( !Set_CalcDataCpgElem(ElemK, CalcD) ) return FALSE;
	if ( !Set_CalcDataByPos(CalcD, bPosiI ? 0 : 1) ) { ASSERT(0); return FALSE; }

	CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
	if ( !m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE) ) { ASSERT(0); }

	int nFortype = 0;
	int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
	ADGNFORCE aForceLcom;
	if ( !m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum) ) { return FALSE; }
	if ( m_pForcCtrl->Get_LcomNumForStrs() == 0 ) { return FALSE; }

	ADGNSTRESS aStressLcom;
	ADGNSTRSADD aStressAdd;
	if ( !m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum) ) { return FALSE; }

	Get_CompositeGeneralSectionInfo(CalcD);

	int iMaxMin = 0;
	int iDgnLcomNo = 0;
	int iOrgLcomNo = 0;
	int iSerLcomCount = 0;
	int iMaxMinType = 0;
	CString strOrgLcomNa = _T("");

	iDgnLcomNo = PlssBase.LcomK;
	if ( !m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType) )
	{
		ASSERT(0);
		return FALSE;
	}
	const _DGN_FORC_CRC& ForceCrc = aForceLcom[iDgnLcomNo - 1];
	const _DGN_STRS_CRC& StressCrc = aStressLcom[iDgnLcomNo - 1];

	int k = CalcD.iPos;
	double dMuy = ForceCrc.dMuy[k];
	double dMa_Ed = ForceStage.MaxForce.dMuy[k];
	double aForce_Gr[6] = { ForceStage.MaxForce.dFxx[k], ForceStage.MaxForce.dFyy[k], ForceStage.MaxForce.dFzz[k],
		ForceStage.MaxForce.dMux[k], ForceStage.MaxForce.dMuy[k], ForceStage.MaxForce.dMuz[k] };
	double aForce[6] = { ForceCrc.dFxx[k], ForceCrc.dFyy[k], ForceCrc.dFzz[k], ForceCrc.dMux[k], ForceCrc.dMuy[k], ForceCrc.dMuz[k] };
	double aStress[4] = { StressCrc.dStl[k],  StressCrc.dStr[k],	  StressCrc.dSbr[k],	  StressCrc.dSbl[k] };

	if ( dMuy > -m_dZero ) // 정모멘트 인데 Stage 모멘트가 부모멘트면 0으로..
	{
		if ( dMa_Ed < 0.0 ) { dMa_Ed = 0.0; }
	}
	else
	{
		if ( dMa_Ed > 0.0 ) { dMa_Ed = 0.0; }
	}

	Get_ClassifyClassSection(CalcD, aForce_Gr, aForce);

	T_PLSS_BASE TempPlssGd = PlssGd;
	T_PLSS_BASE TempPlssBase = PlssBase;
	Calc_LongitudinalShearResistance4SLS(CalcD, m_CpgdD.dks, TempPlssGd, TempPlssBase, PlssDetail);

	return TRUE;
}

void CDgnPlateGirder::Set_PrintMode(BOOL bPrint)
{
	m_bPrintMode = bPrint;
}

BOOL CDgnPlateGirder::IsPrintMode() const
{
	return m_bPrintMode;
}

BOOL CDgnPlateGirder::Get_BetaReductionFactor(double dXpl, double dh, double& dBeta)
{
	if ( dh < m_dZero || dXpl < 0.0 ) { return FALSE; }

	double dRat = dXpl / dh;
	// y=ax + b
	double da = (0.85 - 1.0) / (0.4 - 0.15);
	double db = 1.0 - 0.15*da; // b = y-ax;

	if ( dRat < 0.15 - m_dZero ) { dBeta = 1.0; }
	else if ( dRat > 0.15 && dRat < 0.4 ) { dBeta = da * dRat + db; }
	else if ( dRat > 0.4 - m_dZero ) { dBeta = 0.85; }
	else ASSERT(0);

	return TRUE;
}

BOOL CDgnPlateGirder::Calc_PlasticResistance(T_PLATE_GIRDER_CALC_D& param, T_PGBR_BASE& PgbrBase, double dPlasticNA, double& dMpl_Rd, BOOL bNeglectWeb)
{
	// 1. Material.
	T_MATD_D& MatdD = param.MatlElem.MatdD;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfck = param.MatlElem.dfck;
	double dfsk = param.MatlElem.MatdD.MainRebarData.B_fy;
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;
	double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk / dgam_sr;

	// 2. Section.  
	T_SECT_D& SectD = param.SectElem.SectD;
	BOOL bPosiI = param.bPosiI;
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dTw_t = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTF1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dTF2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dhw = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);
	double dfyt = Get_fy4Thick(dfy, dfy1, dTF1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTF2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;

	// 3. Force
	double dMy = PgbrBase.dMy;
	double dNc = 0.0;
	double dNc_f = 0.0;
	BOOL bPositive = dMy > -m_dZero ? TRUE : FALSE;
	double dAlpha = m_dAlpha;
	double dtfc = bPositive ? dTF1 : dTF2;	// Thick. of Compression Flange
	double dbfc = bPositive ? dB1 : dB2;		// Width. of Compression Flange

	// 4. Calc. Plastic Resistance for Bending
	double dPlasticNAEff = 0.0, dEffeLen = 0.0, dHoleLen = 0.0;
	double dNa_top = dB1 * dTF1*dfyd_t;
	double dNa_bot = dB2 * dTF2*dfyd_b;
	double dNa_web = bNeglectWeb ? 0.0 : dhw * dTw_t*dfyd_w;
	double dXpl = dHt - dPlasticNA;

	// dXpl중심에서 거리.
	double dDistT = 0.0, dDistB = 0.0, dDistW = 0.0;

	if ( bPositive ) // Positive일 경우는 강도 계산할 때 압축콘크리트 중심(Xpl*0.5)에서 계산한다.
	{
		if ( dPlasticNA > dH ) // PNA in Slab
		{
			if ( dXpl > dtc )
			{
				dDistT = dHt - 0.5*dtc - (dH - 0.5*dTF1);
				dDistB = dHt - 0.5*dtc - 0.5*dTF2;
				dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
				dNc_f = dBc * dtc*dAlpha*dfcd;
			}
			else if ( dXpl < dtc )
			{
				dDistT = dHt - 0.5*dXpl - (dH - 0.5*dTF1);
				dDistB = dHt - 0.5*dXpl - 0.5*dTF2;
				dDistW = dHt - 0.5*dXpl - dTF2 - 0.5*dhw;
				dNc_f = dBc * dXpl*dAlpha*dfcd;
			}

			dMpl_Rd = dNa_top * dDistT + dNa_bot * dDistB + dNa_web * dDistW;
		}
		else
		{
			if ( dPlasticNA > dH - dTF1 )  // PNA in Top Flange
			{
				double dTfc = dH - dPlasticNA;
				double dTft = dPlasticNA - (dH - dTF1);
				double dNa_topc = dB1 * dTfc*dfyd_t;
				double dNa_topt = dB1 * dTft*dfyd_t;
				double dDistTc = 0.5*dtc + dHh + 0.5*dTfc;
				double dDistTt = 0.5*dtc + dHh + dTfc + 0.5*dTft;
				dDistB = dHt - 0.5*dtc - 0.5*dTF2;
				dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
				//
				dMpl_Rd = -dNa_topc * dDistTc + dNa_topt * dDistTt + dNa_bot * dDistB + dNa_web * dDistW;
			}
			else if ( dPlasticNA < dH - dTF1 && dPlasticNA > dTF2 ) // PNA in Web
			{
				double dWc = (dH - dTF1) - dPlasticNA;
				double dWt = dPlasticNA - dTF2;
				double dNa_webc = bNeglectWeb ? 0.0 : dWc * dTw_t*dfyd_w;
				double dNa_webt = bNeglectWeb ? 0.0 : dWt * dTw_t*dfyd_w;
				double dDistWc = 0.5*dtc + dHh + dTF1 + 0.5*dWc;
				double dDistWt = 0.5*dtc + dHh + dTF1 + dWc + 0.5*dWt;
				dDistT = dHt - 0.5*dtc - (dH - 0.5*dTF1);
				dDistB = dHt - 0.5*dtc - 0.5*dTF2;
				//
				dMpl_Rd = -dNa_top * dDistT - dNa_webc * dDistWc - dNa_webt * dDistWt + dNa_bot * dDistB;
			}
			else if ( dPlasticNA < dTF2 ) // PNA in Bottom Flange
			{
				double dBfc = dTF2 - dPlasticNA;
				double dBft = dPlasticNA;
				double dNa_botc = dB2 * dBfc*dfyd_b;
				double dNa_bott = dB2 * dBft*dfyd_b;
				double dDistBc = 0.5*dtc + dHh + dTF1 + dhw + 0.5*dBfc;
				double dDistBt = 0.5*dtc + dHh + dTF1 + dhw + dBfc + 0.5*dBft;
				dDistT = dHt - 0.5*dtc - (dH - 0.5*dTF1);
				dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
				//
				dMpl_Rd = -dNa_top * dDistT - dNa_web * dDistW - dNa_botc * dDistBc + dNa_bott * dDistBt;
			}
		}
	}
	else // Negative일 경우는 강도 계산할 때 PlasticNA가 기준. 
	{
		double dCompfsdAsidsi = 0.0;
		double dTensfsdAsidsi = 0.0;
		double ddsi = 0.0;
		double dGap = (dH + dHh + 0.5*dtc) - dPlasticNA;

		for ( int i = 0; i < param.arRbarPosi.GetSize(); i++ )
		{
			const _DGN_RBAR_CRC& Rbar = param.arRbarPosi[i];
			double dNAdsi = dGap + Rbar.dz;
			double dAsi = Rbar.dArea;
			if ( dNAdsi < 0.0 )
			{
				dCompfsdAsidsi += dAsi * dfsd*fabs(dNAdsi);
			}
			else
			{
				dTensfsdAsidsi += dAsi * dfsd*fabs(dNAdsi);
			}
		}

		if ( dPlasticNA > dH ) // PNA in Slab
		{
			dDistT = dPlasticNA - (dH - 0.5*dTF1);
			dDistW = dPlasticNA - (0.5*dhw + dTF2);
			dDistB = dPlasticNA - 0.5*dTF2;
			//
			dMpl_Rd = dNa_top * dDistT + dNa_bot * dDistB + dNa_web * dDistW + dCompfsdAsidsi + dTensfsdAsidsi;
		}
		else
		{
			if ( dPlasticNA > dH - dTF1 ) // PNA in Top Flange
			{
				double dTfc = dPlasticNA - (dH - dTF1);
				double dTft = dH - dPlasticNA;
				double dNa_topc = dB1 * dTfc*dfyd_t;
				double dNa_topt = dB1 * dTft*dfyd_t;
				double dDistTc = 0.5*dTfc;
				double dDistTt = 0.5*dTft;
				dDistB = dPlasticNA - 0.5*dTF2;
				dDistW = dPlasticNA - (dTF2 + 0.5*dhw);
				//
				dMpl_Rd = dNa_topc * dDistTc + dNa_topt * dDistTt + dNa_bot * dDistB + dNa_web * dDistW + dCompfsdAsidsi + dTensfsdAsidsi;
			}
			else if ( dPlasticNA < dH - dTF1 && dPlasticNA > dTF2 ) // PNA in Web
			{
				double dWc = dPlasticNA - dTF2;
				double dWt = (dH - dTF1) - dPlasticNA;
				double dNa_webc = bNeglectWeb ? 0.0 : dWc * dTw_t*dfyd_w;
				double dNa_webt = bNeglectWeb ? 0.0 : dWt * dTw_t*dfyd_w;
				double dDistWc = 0.5*dWc;
				double dDistWt = 0.5*dWt;
				dDistT = (dH - 0.5*dTF1) - dPlasticNA;
				dDistB = dPlasticNA - 0.5*dTF2;
				//
				dMpl_Rd = dNa_top * dDistT + dNa_webc * dDistWc + dNa_webt * dDistWt + dNa_bot * dDistB + dCompfsdAsidsi + dTensfsdAsidsi;
			}
			else if ( dPlasticNA < dTF2 ) // PNA in Bottom Flange
			{
				double dBfc = dPlasticNA;
				double dBft = dTF2 - dPlasticNA;
				double dNa_botc = dB2 * dBfc*dfyd_b;
				double dNa_bott = dB2 * dBft*dfyd_b;
				double dDistBc = 0.5*dBfc;
				double dDistBt = 0.5*dBft;
				dDistT = (dH - 0.5*dTF1) - dPlasticNA;
				dDistW = (dTF2 + 0.5*dhw) - dPlasticNA;
				//
				dMpl_Rd = dNa_top * dDistT + dNa_web * dDistW + dNa_botc * dDistBc + dNa_bott * dDistBt + dCompfsdAsidsi + dTensfsdAsidsi;
			}
		}
	}

	return TRUE;
}

double CDgnPlateGirder::Calc_kmin(BOOL bPositive, double dk_rbar, double dk_slab, double dk_top, double dk_bot, int &nKminType)
{
	double dk_rbar_ab = fabs(dk_rbar);
	double dk_slab_ab = fabs(dk_slab);
	double dk_top_ab = fabs(dk_top);
	double dk_bot_ab = fabs(dk_bot);

	double dk_min = 0.0;
	double dk_min_ab = 0.0;

	if ( bPositive )
	{
		double dk_ab[3] = { dk_slab_ab, dk_top_ab, dk_bot_ab };
		double dk[3] = { dk_slab   , dk_top   , dk_bot };

		for ( int i = 0; i < 3; i++ )
		{
			if ( i == 0 )
			{
				dk_min_ab = dk_ab[i];
				dk_min = dk[i];
				nKminType = Kmin_Slab;
			}
			else
			{
				if ( dk_min_ab > dk_ab[i] )
				{
					dk_min_ab = dk_ab[i];
					dk_min = dk[i];
					if ( i == 1 ) nKminType = Kmin_girder_top;
					else      nKminType = Kmin_girder_bot;
				}
			}
		}
	}
	else
	{
		double dk_ab[3] = { dk_rbar_ab, dk_top_ab, dk_bot_ab };
		double dk[3] = { dk_rbar   , dk_top   , dk_bot };

		for ( int i = 0; i < 3; i++ )
		{
			if ( i == 0 )
			{
				dk_min_ab = dk_ab[i];
				dk_min = dk[i];
				nKminType = Kmin_Rbar;
			}
			else
			{
				if ( dk_min_ab > dk_ab[i] )
				{
					dk_min_ab = dk_ab[i];
					dk_min = dk[i];
					if ( i == 1 ) nKminType = Kmin_girder_top;
					else      nKminType = Kmin_girder_bot;
				}
			}
		}
	}

	return dk_min;
}

// 6.2.1.4 Non-linear resistance to bending.
BOOL CDgnPlateGirder::Calc_NonLinearResistance(ElemPairK ElemK, BOOL bPosiI, T_PGBR_BASE& PgbrBase)
{
	double dNc = PgbrBase.dNc;
	double dNc_el = PgbrBase.dNc_el;
	double dNc_f = PgbrBase.dNc_f;
	double dMa_Ed = PgbrBase.dMa_Ed;
	double dMc_Ed = PgbrBase.dMy;
	double dMpl_Rd = PgbrBase.dMpl_Rd;
	double dMel_Rd = PgbrBase.dMel_Rd;
	double dM_Rd = 0.0;

	if ( dNc < 0.0 )  // Concrete slab under compression
	{
		if ( fabs(dNc) <= dNc_el )
		{
			dM_Rd = fabs(dNc_el) < m_dZero ? dMa_Ed : dMa_Ed + (dMel_Rd - dMa_Ed)*fabs(dNc) / dNc_el;
		}
		else if ( dNc_el <= fabs(dNc) && fabs(dNc) <= dNc_f )
		{
			dM_Rd = fabs(dNc_f - dNc_el) < m_dZero ? dMel_Rd : dMel_Rd + (dMpl_Rd - dMel_Rd)*(fabs(dNc) - dNc_el) / (dNc_f - dNc_el);
		}
		else ASSERT(0);
	}
	else
	{
		dM_Rd = dMel_Rd;
	}

	PgbrBase.dM_Rd = dM_Rd;

	return TRUE;
}

BOOL CDgnPlateGirder::Get_Eta4SteelMaterial(int iMatClass, double dgam0, double dgam1, double& dEta)
{
	dEta = 0.0;
	if ( dgam0 == 0.0 ) { return FALSE; }
	if ( iMatClass == 1 ) // S235, S275, S355 계열.
	{
		dEta = 1.20*dgam1 / dgam0;
	}
	else if ( iMatClass == 2 ) // S420, S460 계열.
	{
		dEta = 1.05*dgam1 / dgam0;
	}
	else if ( iMatClass == 3 ) // etc.
	{
		dEta = 1.00*dgam1 / dgam0;
	}
	else
		ASSERT(0);

	return TRUE;
}

// Modify by GAY. PMS:4216. ('12.05.11). 
double CDgnPlateGirder::Get_Eta4SteelGrade(int nMatClass, double dFy1)
{
	// See EN1993-1-5, 5.1(2).
	// η should be defined in the National Annex of EN1993-1-5.
	// η = 1.2 for structural steel up to and including S460.
	// η = 1.0 for higher steel grades.
	double dEta = 0.0;
	if ( nMatClass == 1 || nMatClass == 2 )
	{
		dEta = 1.2;  // EN05(S) List의 Steel Grade. (S235~S460)
	}
	else if ( nMatClass == 3 )
	{
		if ( dFy1 > 460.0 ) { dEta = 1.0; }
		else { dEta = 1.2; }
	}
	else ASSERT(0);

	return dEta;
}

double CDgnPlateGirder::Get_Zj_Second_Value(double dH, double dHw, double dtw, double dB1, double dtf1, double dB2, double dtf2,
	double dCzm, double dAa, double dIay)
{
	double dVal = 0.0;
	if ( dIay < m_dZero ) { return dVal; }
	// H형 일 경우만.

	// integral( z*y^2 dA )
	double dA_tf = dB1 * dtf1;
	double dA_bf = dB2 * dtf2;
	double dIy_tf = dB1 * pow(dtf1, 3) / 12.0;
	double dIy_bf = dB2 * pow(dtf2, 3) / 12.0;
	double dZ_tf_cm = (dH - 0.5*dtf1) - dCzm;
	double dZ_bf_cm = dCzm - 0.5*dtf2;
	double dIntegral_zy2_tf = (dIy_tf + dA_tf * pow(dZ_tf_cm, 2))*dZ_tf_cm;
	double dIntegral_zy2_bf = (dIy_bf + dA_bf * pow(dZ_bf_cm, 2))*dZ_bf_cm;
	double dIntegral_dzy2 = dIntegral_zy2_tf + dIntegral_zy2_bf;

	// integral( z*z^2 dA )  
	double dIntegral_z3_tf = 2.0*dtf1*pow(0.5*dB1, 4); // 2*[tf1*z^4/4]^(dB1/2)
	double dIntegral_z3_bf = 2.0*dtf2*pow(0.5*dB2, 4);
	double dIntegral_z3_w = 2.0*dHw*pow(0.5*dtw, 4);
	double dIntegral_z3 = dIntegral_z3_tf + dIntegral_z3_bf + dIntegral_z3_w;

	dVal = dAa / (2.0*dIay)*(dIntegral_dzy2 + dIntegral_z3);

	return dVal;
}

double CDgnPlateGirder::Get_ShearCenter_H(double dh, double dt1, double db1, double dt2, double db2)
{
	// h = top flange 중심과 bottom flange 중심 사이의 거리 
	// h2 = bottom flange 중심에서 전단중심 사이의 거리
	double dh2_val1 = dt1 * pow(db1, 3)*dh;
	double dh2_val2 = dt1 * pow(db1, 3) + dt2 * pow(db2, 3);
	double dh2 = dh2_val2 < m_dZero ? 0.0 : dh2_val1 / dh2_val2;

	double dZsc = dh2 + 0.5*dt2;
	return dh2;
}

double CDgnPlateGirder::Get_fy4Thick(double dfy, double dfy1, double dt)
{
	double dfy4thick = dt < 40.0 || fabs(dt - 40.0) < m_dZero ? dfy : dfy1;
	return dfy4thick;
}

BOOL CDgnPlateGirder::Get_WidthThickRatio4Web(BOOL bStiffened, double deta, double dhw, double dtw, double dEpsi, double dk_tau, double& dCheck_lim, BOOL& bCheck)
{
	if ( dtw < m_dZero || fabs(deta) < m_dZero || dk_tau < 0.0 ) { return FALSE; }
	double dVal_lim = 0.0;

	if ( !bStiffened )
	{
		dVal_lim = 72.0 / deta * dEpsi;
	}
	else
	{
		dVal_lim = 31.0 / deta * dEpsi*sqrt(dk_tau);
	}
	dCheck_lim = dVal_lim;
	double dWTRatio = dtw < m_dZero ? 0.0 : dhw / dtw;

	if ( dWTRatio > dVal_lim ) { bCheck = TRUE; }
	else { bCheck = FALSE; }

	return TRUE;
}

double CDgnPlateGirder::Get_Vbw_Rd(double dXw, double dfyw, double dAvw, double dGamma_M1)
{
	if ( dGamma_M1 <= m_dZero ) { return 0.0; }
	double dVbw_Rd = dXw * dfyw*dAvw / (sqrt(3.0)*dGamma_M1);
	return dVbw_Rd;
}

double CDgnPlateGirder::Get_Vbw_Rd_Limit(double dEta, double dfyw, double dAvw, double dGamma_M1)
{
	if ( dGamma_M1 <= m_dZero ) { return 0.0; }
	double dVb_Rd_Limit = dEta * dfyw*dAvw / (sqrt(3.0)*dGamma_M1);
	return dVb_Rd_Limit;
}

double CDgnPlateGirder::Get_tau_b_Rd(double dX, double dfyf, double dGamma_M1)
{
	if ( dGamma_M1 <= m_dZero ) { return 0.0; }
	double dVbw_Rd = dX * dfyf / (sqrt(3.0)*dGamma_M1);
	return dVbw_Rd;
}

double CDgnPlateGirder::Get_ReductonFactorOfFlange4N_Ed(double dGamma_M2, double dfy_f1, double dfy_f2, double dN_Ed, double dAf1, double dAf2)
{
	// Modify by GAY. PMS:4216. ('12.05.11). 압축인 경우만 계산하며, 계산할 때는 절대값으로 고려.
	if ( dN_Ed > 0.0 ) { ASSERT(0); return 1.0; }

	double dFactor_param = (dAf1*dfy_f1 + dAf2 * dfy_f2) / dGamma_M2;
	double dfactor = fabs(dFactor_param) < m_dZero ? 0.0 : 1.0 - fabs(dN_Ed) / dFactor_param;
	if ( dfactor < 0.0 ) { return 0.0; }
	if ( dfactor > 1.0 ) { return 1.0; }
	return dfactor;
}

int CDgnPlateGirder::Get_Lamda_w_Type4Shear(BOOL bLongStiff, CPG_TRST_POSI& TrstPosi)
{
	int iType = 0;
	if ( TrstPosi.bUseStif )
	{
		BOOL bInterTransverse = TRUE; // 사용했으면 보강된 것으로 
		//if(TrstPosi.bInterNonRigidStif || TrstPosi.bInterRigidStif) bInterTransverse=TRUE;  // intermediate transverse stiffener가 있다고 봄. 
		if ( bInterTransverse/* || bLongStiff*/ ) iType = 2; // EN 1993-1-5, 5.3(3), b) // MQC 11106 Transverse Stiff있는 경우는 모두 이 케이스로 고려
		//else iType = 1;    
	}
	else
	{
		/*if(bLongStiff) */iType = 1;  // 그 외에는 모두 EN 1993-1-5, 5.3(3), a)식으로 고려
	}

	return iType;
}

double CDgnPlateGirder::Get_Alpha4Lamda_w(double da, double dhw)
{
	double dAlpha = dhw < m_dZero ? 0.0 : da / dhw;
	return dAlpha;
}

BOOL CDgnPlateGirder::IsMaterialS420orS460(double dfy)
{
	BOOL bMatOK = FALSE;
	BOOL bS420 = fabs(dfy - 420.0) < m_dZero ? TRUE : FALSE;
	BOOL bS460 = fabs(dfy - 460.0) < m_dZero ? TRUE : FALSE;

	if ( bS420 || bS460 ) bMatOK = TRUE;
	return bMatOK;
}

double CDgnPlateGirder::Get_RatioOfInteractionMV(double deta1, double dBendingRat, double deta3)
{
	double dRatio = deta1 + (1.0 - dBendingRat)*pow((2.0*deta3 - 1.0), 2);
	return dRatio;
}

BOOL CDgnPlateGirder::GetStressInfo(bool bPosiI, const T_SECT_D& crSectD, double aGirderForce[6], double aCptForce[6], T_SECT_STIFFNESS& GdStiffD, T_SECT_STIFFNESS& StiffD, T_STRESS_INFO_I& rData)
{
	double dArea_a = GdStiffD.Area;
	double dIy_a = GdStiffD.Ryy;
	double dIz_a = GdStiffD.Rzz;
	double dArea = StiffD.Area;
	double dIy = StiffD.Ryy;
	double dIz = StiffD.Rzz;

	double dFx_a = aGirderForce[0];
	double dMy_a = aGirderForce[4];
	double dMz_a = aGirderForce[5];
	double dFx_c = aCptForce[0];
	double dMy_c = aCptForce[4];
	double dMz_c = aCptForce[5];

	const _DGN_FORC_CRC& ScfpForce = GetScfpForce4Lcom();
	int nPosId = bPosiI ? 0 : 1;
	double dFx_sc = ScfpForce.dFxx[nPosId];
	double dMy_sc = ScfpForce.dMuy[nPosId];
	double dMz_sc = ScfpForce.dMuz[nPosId];
	dFx_c -= dFx_sc;
	dMy_c -= dMy_sc;
	dMz_c -= dMz_sc;

	double aStress_Pos[4] = { 0.0 }; // 0=TL, 1=TR, 2 = BR, 3 = BL
	Get_StressOfPosition(aGirderForce, aCptForce, GdStiffD, StiffD, aStress_Pos);

	double dy1 = 0.0, dy2 = 0.0, dz1 = 0.0, dz2 = 0.0;
	double dy1_a = 0.0, dz1_a = 0.0, dy1_c = 0.0, dz1_c = 0.0;
	double dy2_a = 0.0, dz2_a = 0.0, dy2_c = 0.0, dz2_c = 0.0;
	double dStr_a1 = 0.0, dStr_c1 = 0.0;
	double dStr_a2 = 0.0, dStr_c2 = 0.0;
	double dStr1 = 0.0, dStr2 = 0.0;

	T_STRESS_LINE_INFO FlgLine[4]; // 0=TL, 1=TR, 2 = BR, 3 = BL
	BOOL bTop = TRUE, bLeft = TRUE;
	for ( int i = 0; i < 4; ++i )
	{
		FlgLine[i].Initialize();
		bTop = (i == 0 || i == 1) ? TRUE : FALSE;
		bLeft = (i == 0 || i == 3) ? TRUE : FALSE;
		GetCentFlgLinePosD(bPosiI, bTop, bLeft, crSectD, GdStiffD, dy1_a, dz1_a, dy2_a, dz2_a);
		GetCentFlgLinePosD(bPosiI, bTop, bLeft, crSectD, StiffD, dy1_c, dz1_c, dy2_c, dz2_c);
		dStr_a1 = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, dy1_a, dz1_a);
		dStr_c1 = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea, dIy, dIz, dy1_c, dz1_c);
		dStr_a2 = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, dy2_a, dz2_a);
		dStr_c2 = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea, dIy, dIz, dy2_c, dz2_c);
		double df1Const = CalcStressInterpolate(nPosId, GdStiffD, dy1_a, dz1_a);
		double df2Const = CalcStressInterpolate(nPosId, GdStiffD, dy2_a, dz2_a);

		dStr1 = dStr_a1 + dStr_c1 + df1Const;
		dStr2 = dStr_a2 + dStr_c2 + df2Const;
		FlgLine[i].dy1 = dy1_c;
		FlgLine[i].dz1 = dz1_c;
		FlgLine[i].dy2 = dy2_c,
		FlgLine[i].dz2 = dz2_c;
		FlgLine[i].dStr1 = dStr1;
		FlgLine[i].dStr2 = dStr2;
	}

	T_STRESS_LINE_INFO WebLine[2]; // 0 = Left, 1 = Right
	int nWebSize = m_pDgnSectUtil->Get_WebPartNum(&crSectD);

	for ( int i = 0; i < nWebSize; ++i )
	{
		WebLine[i].Initialize();
		bLeft = (i == 0) ? TRUE : FALSE;
		GetCentWebLinePosD(bPosiI, bLeft, crSectD, GdStiffD, dy1_a, dz1_a, dy2_a, dz2_a);
		GetCentWebLinePosD(bPosiI, bLeft, crSectD, StiffD, dy1_c, dz1_c, dy2_c, dz2_c);
		dStr_a1 = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, dy1_a, dz1_a);
		dStr_c1 = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea, dIy, dIz, dy1_c, dz1_c);
		dStr_a2 = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, dy2_a, dz2_a);
		dStr_c2 = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea, dIy, dIz, dy2_c, dz2_c);
		double df1Const = CalcStressInterpolate(nPosId, GdStiffD, dy1_a, dz1_a);
		double df2Const = CalcStressInterpolate(nPosId, GdStiffD, dy2_a, dz2_a);

		dStr1 = dStr_a1 + dStr_c1 + df1Const;
		dStr2 = dStr_a2 + dStr_c2 + df2Const;
		WebLine[i].dy1 = dy1_c;
		WebLine[i].dz1 = dz1_c;
		WebLine[i].dy2 = dy2_c,
		WebLine[i].dz2 = dz2_c;
		WebLine[i].dStr1 = dStr1;
		WebLine[i].dStr2 = dStr2;
	}

	rData.FlgTL = FlgLine[0];
	rData.FlgTR = FlgLine[1];
	rData.Web   = WebLine[0];
	rData.FlgBL = FlgLine[3];
	rData.FlgBR = FlgLine[2];
	return TRUE;
}

double CDgnPlateGirder::Get_Mel_RdOfElastResistance(double dfy, double dFx, double dMy, double dA, double dIy, double dz)
{
	if ( dA < m_dZero || dIy < m_dZero || dz < m_dZero ) { return 0.0; }
	double dMel_Rd_min = 0.0;

	double dIz = 1.0;
	double dStress_a = Calc_StressOfPosition(dFx, dMy, 0.0, dA, dIy, dIz, 0.0, dz);
	if ( dStress_a > 0.0 )
	{
		dMel_Rd_min = dIy / fabs(dz)*(dfy - dFx / dA);
	}
	else
	{
		dMel_Rd_min = dIy / fabs(dz)*(-dfy - dFx / dA);
	}
	dMel_Rd_min = fabs(dMel_Rd_min);

	return dMel_Rd_min;
}

BOOL CDgnPlateGirder::Get_PlasticMomentOfDividedPart(double dfy1, double dfy2, double dCzm_NAz, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLineData, double& dMpl, double& dNgir)
{
	dMpl = 0.0;
	int i = 0;
	int iLineSize = aLineData.GetSize();
	double dOutMaxPt[2] = { 0. };
	double dOutMinPt[2] = { 0. };
	//  T_GSEC_POLYGON TempPolyD = PolyD;
	//  CArray<T_GSEC_LINE, T_GSEC_LINE&> aTempLineData;
	//  aTempLineData.Copy(aLineData);
	//  CSectUtil::Get_MaxMinPoint4LineType(TempPolyD, aTempLineData, dOutMaxPt, dOutMinPt);


	for ( i = 0; i < iLineSize; i++ )
	{
		T_GSEC_LINE LineD = aLineData[i];
		int v1 = LineD.v1;
		int v2 = LineD.v2;

		T_GSEC_VERTEX vtx1 = PolyD.aVertex[v1];
		T_GSEC_VERTEX vtx2 = PolyD.aVertex[v2];
		double dx1 = vtx1.dX;
		double dy1 = vtx1.dY;
		double dx2 = vtx2.dX;
		double dy2 = vtx2.dY;
		double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
		double dt = LineD.dThik;
		double dAl = dbi * dt;
		double dfy = Get_fy4Thick(dfy1, dfy2, dt);
		double dfyd = dfy / m_dGamma_M0;
		double dzc = 0.5*(dy1 + dy2) - dCzm_NAz;
		double dMpli = fabs(dzc)*dAl*dfyd;
		dMpl += dMpli;
		dNgir += dAl * dfyd;
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_PlasticMomentOfHoriDividedLine(double dfy1, double dfy2, double dCzm_NAz, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLineData, const CArray<double, double&>& aCutTopThick,
	double& dMpl_top, double& dMpl_bot, double& dNgir_top, double& dNgir_bot)
{
	dMpl_top = dMpl_bot = 0.0;
	dNgir_top = dNgir_bot = 0.0;
	int i = 0;
	int iLineSize = aLineData.GetSize();
	double dOutMaxPt[2] = { 0. };
	double dOutMinPt[2] = { 0. };

	for ( i = 0; i < iLineSize; i++ )
	{
		T_GSEC_LINE LineD = aLineData[i];
		int v1 = LineD.v1;
		int v2 = LineD.v2;
		int nAlign = LineD.nAlign;

		T_GSEC_VERTEX vtx1 = PolyD.aVertex[v1];
		T_GSEC_VERTEX vtx2 = PolyD.aVertex[v2];
		double dx1 = vtx1.dX;
		double dy1 = vtx1.dY;
		double dx2 = vtx2.dX;
		double dy2 = vtx2.dY;
		double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
		double dt = LineD.dThik;
		double dt_t = aCutTopThick[i];
		double dt_b = dt - dt_t;
		double dAl_t = dbi * dt_t;
		double dAl_b = dbi * dt_b;
		double dfy = Get_fy4Thick(dfy1, dfy2, dt);
		double dfyd = dfy / m_dGamma_M0;

		T_GSEC_POLYGON LinePolyD;
		CSectUtil::GetCoorPointOfLine(nAlign, dx1, dy1, dx2, dy2, dt, LinePolyD);
		double dp1y = LinePolyD.aVertex[0].dY;
		double dp2y = LinePolyD.aVertex[1].dY;
		double dzc_t = 0.0, dzc_b = 0.0;

		if ( dp2y < dp1y )
		{
			dzc_t = (dp1y - 0.5*dt_t) - dCzm_NAz;
			dzc_b = (dp2y + 0.5*dt_b) - dCzm_NAz;
		}
		else if ( dp1y < dp2y )
		{
			dzc_t = (dp2y - 0.5*dt_t) - dCzm_NAz;
			dzc_b = (dp1y + 0.5*dt_b) - dCzm_NAz;
		}

		double dMpli_t = fabs(dzc_t)*dAl_t*dfyd;
		double dMpli_b = fabs(dzc_b)*dAl_b*dfyd;
		dMpl_top += dMpli_t;
		dMpl_bot += dMpli_b;
		dNgir_top += dAl_t * dfyd;
		dNgir_bot += dAl_b * dfyd;
	}

	return TRUE;
}

double CDgnPlateGirder::Get_Lamda3_DesignLifeInYears(int iYears)
{
	double dLamda = 0.0;

	switch ( iYears )
	{
	case 50: {dLamda = 0.871;      break; }
	case 60: {dLamda = 0.903;      break; }
	case 70: {dLamda = 0.931;      break; }
	case 80: {dLamda = 0.956;      break; }
	case 90: {dLamda = 0.979;      break; }
	case 100: {dLamda = 1.000;      break; }
	case 120: {dLamda = 1.037;      break; }
	default: {dLamda = 0.871;      		}
	}

	return dLamda;
}

BOOL CDgnPlateGirder::Get_NonDimensionalSlenderness(double dL, double db, double dfy, double dEs, double dm, double dAwc, double dAf, double& dLamda)
{
	if ( dEs*dm < m_dZero || db < m_dZero )
	{
		dLamda = 0.0;
	}
	else
	{
		if ( dAf < m_dZero )
			dLamda = 1.103*dL / db * sqrt(dfy / (dEs*dm));
		else
			dLamda = 1.103*dL / db * sqrt(dfy / (dEs*dm))*sqrt(1.0 + dAwc / (3.0*dAf));
	}

	return TRUE;
}

double CDgnPlateGirder::Get_PHIform(double &dM1, double &dM2, double dNu)
{
	double dMrat = 0.0;
	if ( dM2*dM1 < m_dZero ) // 부호가 다르면.
	{
		dMrat = 0.0;
	}
	else
	{
		if ( fabs(dM1) >= fabs(dM2) ) { dMrat = fabs(dM1) < m_dZero ? 0.0 : dM2 / dM1; }
		else
		{
			dMrat = fabs(dM2) < m_dZero ? 0.0 : dM1 / dM2;
			double dM1_t = dM1;
			dM1 = dM2;
			dM2 = dM1_t;
		}
	}
	double dPHI = fabs(1.0 + dNu) < m_dZero ? 0.0 : 2.0*(1.0 - dMrat) / (1.0 + dNu);

	return dPHI;
}

BOOL CDgnPlateGirder::Get_m_Value(double dNu, double dPHI, double dgamma, double& dm1, double& dm2, double& dm_min, double& dm)
{
	dm1 = 1.0 + 0.44*(1.0 + dNu)*pow(dPHI, 1.5) + (3.0 + 2.0*dPHI)*dgamma / (350.0 - 50.0*dNu);
	dm2 = 1.0 + 0.44*(1.0 + dNu)*pow(dPHI, 1.5) + (0.195 + (0.05 + dNu / 100.0)*dPHI)*pow(dgamma, 0.5);
	dm = dm_min = min(dm1, dm2);
	if ( dm < 1.0 ) { dm = 1.0; }

	return TRUE;
}

double CDgnPlateGirder::Get_PHI_LT(double dAlpha, double dLamda_LT)
{
	double dPHI_LT = 0.5*(1.0 + dAlpha * (dLamda_LT - 0.2) + pow(dLamda_LT, 2));
	return dPHI_LT;
}

double CDgnPlateGirder::Get_X_LT(double dPHI_LT, double dLamda_LT, double& dX_LT_org)
{
	double dX_LT = 0.0;
	double dX_LT_Parm1 = dPHI_LT * dPHI_LT - dLamda_LT * dLamda_LT;
	if ( dX_LT_Parm1 < 0.0 ) { dX_LT_Parm1 = 0.0; }

	dX_LT_org = fabs(dPHI_LT + sqrt(dX_LT_Parm1)) < m_dZero ? 0.0 : 1.0 / (dPHI_LT + sqrt(dX_LT_Parm1));
	dX_LT = dX_LT_org;

	if ( dX_LT > 1.0 ) { dX_LT = 1.0; }

	return dX_LT;
}


int CDgnPlateGirder::Get_BucklingCurveForCrossSection(double dh, double db, BOOL bIsSectI/* = TRUE*/)
{
	int iType;
	double dRatio = db < m_dZero ? 0.0 : dh / db;
	// Buckling curve
	// a0=0, a=1, b=2, c=3, d=4    
	if ( dRatio < 2.0 + m_dZero && bIsSectI )
	{
		iType = 3; // c    
	}
	else
	{
		iType = 4; // d
	}

	return iType;
}

double CDgnPlateGirder::Get_ImperfectionFactor(int iCase)
{
	// Table 6.3: Imperfection factors for lateral torsional buckling curves. EN 1993-1-1, 6.3.2.2, 61p
	double dAlpha_LT = 0.0;
	switch ( iCase )
	{
	case 1: {dAlpha_LT = 0.21;      break; }
	case 2: {dAlpha_LT = 0.34;      break; }
	case 3: {dAlpha_LT = 0.49;      break; }
	case 4: {dAlpha_LT = 0.76;      break; }
	default: {						break; }
	}

	return dAlpha_LT;
}

BOOL CDgnPlateGirder::Get_IslOfLongiStiff(int iType, double dh, double dt, double dtw, double dEpsi, double& dIsl)
{
	if ( dh < m_dZero || dt < m_dZero || dtw < m_dZero || dEpsi < m_dZero ) { return FALSE; }

	double dIzz_sl = dt * pow(dh, 3) / 12.0;
	double dhw = dt + 2.0*(15.0*dEpsi*dtw);
	double dIzz_mo = dhw * pow(dtw, 3) / 12.0;

	if ( iType == 1 )
	{
		double dAw = dhw * dtw;
		double dAls = dh * dt;
		double dArea_t = dAw + dAls;
		double dZcen = (dAw*0.5*dtw + dAls * (dtw + 0.5*dh)) / dArea_t;
		double dd = (dtw + 0.5*dh) - dZcen;
		double dd2 = dZcen - 0.5*dtw;
		dIsl = dIzz_sl + dAls * pow(dd, 2) + dIzz_mo + dAw * pow(dd2, 2);
	}
	else if ( iType == 2 )
	{
		double dAls = dh * dt;
		double dd = (0.5*dt + 0.5*dh);
		dIsl = 2.0*(dIzz_sl + dAls * pow(dd, 2)) + dIzz_mo;
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_IslOfLongiStiff(const CPG_LSTI_DATA& InD, double dtw, double dEpsi, double& dIsl)
{
	double dIzz_sl = InD.dIyy;
	double dhw = InD.dt + 2.0*(15.0*dEpsi*dtw);
	double dIzz_mo = dhw * pow(dtw, 3) / 12.0;
	double dAw = dhw * dtw;
	double dAls = InD.dArea;
	double dArea_t = dAw + dAls;
	double dZcen = (dAw*0.5*dtw + dAls * (dtw + InD.dCzm)) / dArea_t;
	double dd = (dtw + InD.dCzm) - dZcen;
	double dd2 = dZcen - 0.5*dtw;
	dIsl = dIzz_sl + dAls * pow(dd, 2) + dIzz_mo + dAw * pow(dd2, 2);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_IslOfLongStfn4GS(double dEpsi, T_GLINE_STIFFENER& StfnD, double dtw, double& dIsl)
{
	int nType = StfnD.nType;
	int nPosType = StfnD.nPosLine;
	double dZst = StfnD.dZbar;
	double dAst = StfnD.dA;
	double dIst = StfnD.dIy;
	double dMoLen = 2.0*(15.0*dEpsi*dtw);
	if ( nType == 0 )
	{
		dMoLen += StfnD.dSize[1];
	}
	else if ( nType == 1 )
	{
		dMoLen += StfnD.dSize[2];
	}
	else if ( nType == 2 )
	{
		dMoLen += 2.0*(dMoLen + StfnD.dSize[3]);
	}
	else ASSERT(0);

	double dAw = dMoLen * dtw;
	double dIw = dMoLen * pow(dtw, 3) / 12.0;
	if ( nPosType == 0 || nPosType == 1 ) // Stiffener 하나. 
	{
		double dArea_t = dAw + dAst;
		double dZcen = dArea_t == 0.0 ? 0.0 : (dAw*0.5*dtw + dAst * (dtw + dZst)) / dArea_t;
		double dIsl_mo = dIw + dAw * pow(dZcen - 0.5*dtw, 2);
		double dIsl_st = dIst + dAst * pow(dtw + dZst - dZcen, 2);
		dIsl = dIsl_mo + dIsl_st;
	}
	else if ( nPosType == 2 )
	{
		double dIsl_st = dIst + dAst * pow(0.5*dtw + dZst, 2);
		dIsl = dIw + 2.0*dIsl_st;
	}
	else
		ASSERT(0);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_SlendernessParamLamda(int iType, int iLongStiffSize, double dhw, double da, double dt, double dIsl, double dEpsi,
	double& dk_tau, double& dk_tau_st, double& dk_tau_st_Limit, double& dLamda)
{
	if ( iType == 1 )
	{
		if ( dt < m_dZero || dEpsi < m_dZero ) { return FALSE; }
		dLamda = dhw / (86.4*dt*dEpsi);
	}
	else if ( iType == 2 )
	{
		if ( da < m_dZero || dhw < m_dZero || dt < m_dZero || dEpsi < m_dZero ) { return FALSE; }
		Get_ShearBucklingCoeff(iLongStiffSize, dhw, da, dt, dIsl, dk_tau, dk_tau_st, dk_tau_st_Limit);
		if ( dk_tau < m_dZero ) { return FALSE; }
		dLamda = dhw / (37.4*dt*dEpsi*sqrt(dk_tau));
	}
	//else ASSERT(0);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_ShearBucklingCoeff(int iLongStiffSize, double dhw, double da, double dt, double dIsl, double& dk_tau, double& dk_tau_st, double& dk_tau_st_Limit)
{
	if ( da < m_dZero || dhw < m_dZero || dt < m_dZero ) { return FALSE; }

	double dk_rat = dIsl / (pow(dt, 3)*dhw);
	double dk_tau_st1 = 9.0*pow(dhw / da, 2);
	double dk_tau_st2 = pow(dk_rat, 3);
	dk_tau_st = dk_tau_st1 * pow(dk_tau_st2, 1. / 4.);
	dk_tau_st_Limit = 2.1 / dt * pow(dIsl / dhw, 1. / 3.);

	if ( dk_tau_st < dk_tau_st_Limit ) { dk_tau_st = dk_tau_st_Limit; }

	double dk_tau1 = 6.3 + 0.18*dk_rat;
	double dk_tau2 = 2.2*pow(dk_rat, 1. / 3.);
	double dAlpha = da / dhw;

	// EN 1993-1-5, A.3 Shear buckling coefficients
	if ( iLongStiffSize == 1 || iLongStiffSize == 2 )
	{
		if ( dAlpha < 3.0 )
		{
			dk_tau = 4.1 + dk_tau1 / pow(dAlpha, 2) + dk_tau2;
		}
		else
		{
			dk_tau = 5.34 + 4.0*pow(dhw / da, 2) + dk_tau_st;
		}
	}
	else // 없거나 3개이상 
	{
		if ( da / dhw < 1.0 ) { dk_tau = 4.0 + 5.34*pow(dhw / da, 2) + dk_tau_st; }
		else { dk_tau = 5.34 + 4.0*pow(dhw / da, 2) + dk_tau_st; }
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_ContributionFromWebFactor(int iEndpostType, double dEta, double dLamda, double& dXw, int& iLamda_w_scope)
{
	if ( dEta < m_dZero || dLamda < m_dZero ) { return FALSE; }

	if ( iEndpostType == 0 ) // Rigid end post
	{
		if ( dLamda < 0.83 / dEta )
		{
			dXw = dEta;
			iLamda_w_scope = 0;
		}
		else if ( 0.83 / dEta < dLamda + m_dZero && dLamda < 1.08 )
		{
			dXw = 0.83 / dLamda;
			iLamda_w_scope = 1;
		}
		else if ( dLamda > 1.08 )
		{
			dXw = fabs(0.7 + dLamda) < m_dZero ? 0.0 : 1.37 / (0.7 + dLamda);
			iLamda_w_scope = 2;
		}
	}
	else if ( iEndpostType == 1 ) // Non-rigid end post
	{
		if ( dLamda < 0.83 / dEta )
		{
			dXw = dEta;
			iLamda_w_scope = 0;
		}
		else if ( 0.83 / dEta < dLamda + m_dZero && dLamda < 1.08 )
		{
			dXw = 0.83 / dLamda;
			iLamda_w_scope = 1;
		}
		else if ( dLamda > 1.08 )
		{
			dXw = 0.83 / dLamda;
			iLamda_w_scope = 2;
		}
	}
	else ASSERT(0);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_bf4c_DB(bool bPosiI, T_SECT_D& SectD, bool bTopF, int nWebPart, double dEpsilon, double& dbf)
{
	dbf = 0.0;

	int nRealSectType = m_pDgnSectUtil->Get_RealSectType(&SectD);

	switch ( nRealSectType )
	{
	case D_SECT_TYPE_COMPO_I:
		{
			if ( bTopF )
			{
				double dtf = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
				double dbft = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
				double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
				double dbf_lim = 15.0*dEpsilon*dtf;
				double dbf_h = (dbft - dTw)*0.5;

				if ( dbf_h > dbf_lim ) { dbf = dTw + dbf_lim * 2.0; }
				else { dbf = dbft; }
			}
			else
			{
				double dtf = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
				double dbft = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
				double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
				double dbf_lim = 15.0*dEpsilon*dtf;
				double dbf_h = (dbft - dTw)*0.5;

				if ( dbf_h > dbf_lim ) { dbf = dTw + dbf_lim * 2.0; }
				else { dbf = dbft; }
			}
			break;
		}
	case D_SECT_TYPE_COMPO_B:
		{
			if ( bTopF )
			{
				double dtf = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
				double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
				double dbft_out = m_pDgnSectUtil->Get_Size_bft_l(&SectD, bPosiI) - 0.5*dTw; // left and right side is same size of box section
				double dbft_in = 0.5*m_pDgnSectUtil->Get_Size_bft_m(&SectD, bPosiI);
				double dbf_lim = 15.0*dEpsilon*dtf;

				double dbft_eff_out = dbft_out > dbf_lim ? dbf_lim : dbft_out;
				double dbft_eff_in = dbft_in > dbf_lim ? dbf_lim : dbft_in;

				dbf = dbft_eff_out + dbft_eff_in + dTw;
			}
			else
			{
				double dtf = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
				double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
				double dbft_out = m_pDgnSectUtil->Get_Size_bfb_l(&SectD, bPosiI) - 0.5*dTw;
				double dbft_in = m_pDgnSectUtil->Get_Size_bfb_m(&SectD, bPosiI);
				double dbf_lim = 15.0*dEpsilon*dtf;

				double dbft_eff_out = dbft_out > dbf_lim ? dbf_lim : dbft_out;
				double dbft_eff_in = dbft_in > dbf_lim ? dbf_lim : dbft_in;

				dbf = dbft_eff_out + dbft_eff_in + dTw;
			}
			break;
		}
	case D_SECT_TYPE_COMPO_TUB:
		{
			if ( bTopF )
			{
				double dtf = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
				double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
				double dbft_l = m_pDgnSectUtil->Get_Size_bft_ll(&SectD, bPosiI) - dTw; // left and right side is same size of box section
				double dbft_r = m_pDgnSectUtil->Get_Size_bft_l(&SectD, bPosiI) - m_pDgnSectUtil->Get_Size_bft_ll(&SectD, bPosiI);
				double dbf_lim = 15.0*dEpsilon*dtf;

				double dbft_eff_l = dbft_l > dbf_lim ? dbf_lim : dbft_l;
				double dbft_eff_r = dbft_r > dbf_lim ? dbf_lim : dbft_r;

				dbf = dbft_eff_l + dbft_eff_r + dTw;
			}
			else
			{
				double dtf = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
				double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
				double dbft_out = m_pDgnSectUtil->Get_Size_bfb_l(&SectD, bPosiI) - dTw;
				double dbft_in = m_pDgnSectUtil->Get_Size_bfb_m(&SectD, bPosiI);
				double dbf_lim = 15.0*dEpsilon*dtf;

				double dbft_eff_out = dbft_out > dbf_lim ? dbf_lim : dbft_out;
				double dbft_eff_in = dbft_in > dbf_lim ? dbf_lim : dbft_in;

				dbf = dbft_eff_out + dbft_eff_in + dTw;
			}
			break;
		}
	default:
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDgnPlateGirder::Get_PlasticNAEff(BOOL bPositive, T_PLATE_GIRDER_CALC_D& param, double dPlasticNAxis, double& dEffPlasticNA, double& dEffeLen, double& dHoleLen)
{
	// 1. Initialize
	dEffPlasticNA = dPlasticNAxis;
	dHoleLen = 0.0;   dEffeLen = 0.0;

	// 2. Section
	T_SECT_D crSectD = param.SectElem.SectD;
	BOOL bPosiI = param.bPosiI;
	double dH = m_pDgnSectUtil->Get_Size_H(&crSectD, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&crSectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dTw_t = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
	double dTf1 = m_pDgnSectUtil->Get_Size_tft(&crSectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&crSectD, bPosiI);
	double dTf2 = m_pDgnSectUtil->Get_Size_tfb(&crSectD, bPosiI);
	double dhw = m_pDgnSectUtil->Get_Size_hw(&crSectD, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&crSectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&crSectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&crSectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&crSectD, bPosiI);

	// 3. Material
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dfy = param.MatlElem.MatdD.Data1.Design.S_Fy1;
	double dfy1 = param.MatlElem.MatdD.Data1.Design.S_Fy2;
	double dfck = param.MatlElem.dfck;
	double dfsk = param.MatlElem.MatdD.MainRebarData.B_fy;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;
	double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk / dgam_sr;
	double dfyt = Get_fy4Thick(dfy, dfy1, dTf1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTf2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;
	double dRebarF = 0.0;

	BOOL bPlasticClass = TRUE;
	double dfy_r = dfyw;
	double dEpsi = Get_Epsilon(dfy_r);

	// 4. Calc. Each Part's Force
	// EN 1993-1-1, 6.2.2.4 Effective properties of cross section with class 3 webs and class 1 or 2 flanges 
	double dConcF = dBc * dtc*m_dAlpha*dfcd;
	double dStopfF = dB1 * dTf1*dfyd_t;
	double dSbotfF = dB2 * dTf2*dfyd_b;
	double dSwebF = dhw * dTw_t*dfyd_w;
	dEffeLen = 20.0*dEpsi*dTw; // Figure 6.3:Effective class 2 web for compression flange
	double dEffeLen2 = 2.0*dEffeLen;

	if ( dEffeLen2 > dhw ) { return FALSE; }

	double dBwc = 0.0; // Web's height under compression for N.A. 

	if ( bPositive )
	{
		if ( dH - dTf1 > dPlasticNAxis ) // dPlasticNAxis>dTf2
		{
			dBwc = dH - dTf1 - dPlasticNAxis;
		}
		else { return FALSE; }
	}
	else
	{
		if ( dTf2 < dPlasticNAxis ) // dPlasticNAxis<dH-dTf1
		{
			dBwc = dPlasticNAxis - dTf2;
		}
		else { return FALSE; }
	}

	double dWebEffeArea = dEffeLen2 * dTw_t;
	double dWebEffFc = dWebEffeArea * dfyd_w;
	double dGirderF = dStopfF + dSbotfF + dSwebF;
	double dGirderFeff = dStopfF + dSbotfF + dWebEffeArea;
	//
	double dN_top = 0.0, dN_bot = 0.0;
	double dN_bf_ew = 0.0; // bottom flange + web effective force
	double dN_tf_ew = 0.0; // top flange + web effective force
	double dEffeLendHoleLen = 0.0;

	// 5. Calc. Eff.PNA
	if ( bPositive )
	{
		// 중립축 위부터 결정한다. 
		dN_top = dConcF + dStopfF + dWebEffFc;
		dN_bf_ew = dSbotfF;
		if ( dN_top < dN_bf_ew )
		{
			double dVal = dN_bf_ew - dN_top;
			double dtbfx = dfyd_b * dB2 < m_dZero ? 0.0 : dVal / (2.0*dfyd_b*dB2);
			dEffPlasticNA = dTf2 - dtbfx;
			dHoleLen = dhw - 2.0*dEffeLen;
		}
		else
		{
			double dN_top_minus_bf_ew = dN_top - dN_bf_ew;
			double dWebLenP = dfyd_w * dTw_t < m_dZero ? 0.0 : dN_top_minus_bf_ew / (dfyd_w*dTw_t);
			double dWebEffBotLen = dWebLenP + dEffeLen;
			//if ( dWebEffBotLen > dhw-dEffeLen ) { return FALSE; } // 계산한 길이가 Top의 web와 겹치면 안된다.

			dEffPlasticNA = dTf2 + dWebLenP;
			dHoleLen = dhw - dWebEffBotLen - dEffeLen;
		}
	}
	else
	{
		for ( int i = 0; i < param.arRbarPosi.GetSize(); i++ )
		{
			const _DGN_RBAR_CRC& Rbar = param.arRbarPosi[i];
			double dAsi = Rbar.dArea;
			dRebarF += dAsi * dfsd;
		}

		dN_bot = dSbotfF + dWebEffFc;
		dN_tf_ew = dRebarF + dStopfF;
		if ( dN_bot > dN_tf_ew )
		{
			double dVal = dN_tf_ew - dN_bot;
			if ( dVal > dStopfF )
			{
				return FALSE;
			}
			else
			{
				double dtfx = dfyd_t * dB1 < m_dZero ? 0.0 : dVal / (2.0*dfyd_t*dB1);
				dEffPlasticNA = dTf2 + dhw + dtfx;
				dHoleLen = dhw - 2.0*dEffeLen;
			}
		}
		else
		{
			double dN_bot_minus_tf_ew = dN_bot - dN_tf_ew;
			double dWebLenP = dfyd_w * dTw_t < m_dZero ? 0.0 : dN_bot_minus_tf_ew / (dfyd_w*dTw_t);
			double dWebEffTopLen = dWebLenP + dEffeLen;
			//if(dWebEffTopLen > dhw-dEffeLen) {return FALSE;}

			dEffPlasticNA = dH - dTf1 - dWebLenP;
			dHoleLen = dhw - dWebEffTopLen - dEffeLen;
		}
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_EffectiveSectionProperty(BOOL bPosiI, double dMy, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi, T_CLAS_BASE& ClassD,
	T_SECT_STIFFNESS& EffeStlSect, T_SECT_STIFFNESS& EffeComSect)
{
	EffeStlSect.Initialize();
	EffeComSect.Initialize();

	int i = 0;
	double dMc_Ed = dMy;

	// Material
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;

	double dfy = MatlElem.MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatlElem.MatdD.Data1.Design.S_Fy2;
	double dfck = MatlElem.dfck;
	double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
	double dfcd = dgam_c == 0.0 ? 0.0 : dfck / dgam_c;
	double dfsd = dgam_sr == 0.0 ? 0.0 : dfsk / dgam_sr;

	// Section.  
	T_SECT_D SectD = SectElem.SectD;
	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
	T_SECT_SECTBASE_D SectBase = bTapered && !bPosiI ? SectD.CmpTapJ : SectD.SectBefore.SectI;

	//T_SECT_STIFFNESS StlSect; 
	//StlSect.Initialize();
	T_SECT_STIFFNESS NonConSect;    NonConSect.Initialize();
	T_SECT_STIFFNESS RebarSect;     RebarSect.Initialize();
	GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

	//
	T_SECT_STIFFNESS GirderStiff;
	if ( !bTapered )
	{
		GirderStiff = SectD.SectBefore.Stiffness;
	}
	else
	{
		GirderStiff = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;  // girder
	}

	//
	T_SECT_STIFFNESS SectTemp;
	//SectTemp = dMc_Ed>=0.0 ? RebarSect : NonConSect;

	// Dimension	
	double dH = SectBase.Size[0] + SectBase.Size[3] + SectBase.Size[5];
	double dTw = SectBase.Size[1];
	double dB1 = SectBase.Size[2];
	double dTF1 = SectBase.Size[3];
	double dB2 = SectBase.Size[4];
	double dTF2 = SectBase.Size[5];
	double dr1 = SectBase.Size[6];
	double dr2 = SectBase.Size[7];
	if ( dB2 == 0. )	dB2 = dB1;
	if ( dTF2 == 0. )	dTF2 = dTF1;
	double dh = SectBase.Size[0]; // C, web의 높이.

	double dScc = SectD.SectAfter.SectI.Size[1];  // C.T.C.
	double dEr = SectD.SectBefore.Matl_Elast;    // Es/Ec
	double dDr = SectD.SectBefore.Matl_Density;  // Ds/Dc
	double dBc = SectD.SectAfter.SectJ.Size[0];
	double dtc = SectD.SectAfter.SectJ.Size[1];
	double dHh = SectD.SectAfter.SectJ.Size[2];
	//
	double dHt = dH + dHh + dtc; // 전체높이.  

	double dbc = 0.0;
	//double dc=0.0;
	//double dAlpha=0.0;
	//double dPsi=0.0;
	//double dk_sig=0.0;

	// Parameter  
	int iClassSect = ClassD.iClassSect;
	int iWebClass = ClassD.iWebClass;
	int iWebDeter = ClassD.iWebDeter;
	double dPsi = ClassD.dPsi[4];
	double dbeff = ClassD.dbeff[4];
	double dbe1 = ClassD.dbe1[4];
	double dbe2 = ClassD.dbe2[4];
	//double dbc   = ClassD.d

	double dSubWidth = 0.0;
	double dSubZbar = 0.0;
	double dSubArea = 0.0;
	double dSubIyy = 0.0;
	//
	double dEffArea = 0.0;
	double dEffCzp = 0.0;
	double dEffCzm = 0.0;
	double dEffIyy = 0.0;

	//!/ moment와 iWebDeter가 다를 경우 처리 해야함. 
	// Modify, 작용한 Moment만으로는 ClassD.iWebDeter을 if문으로 판단하는 것은 부적절 하여 막음.
	/*
	if(dMc_Ed>=0.0)
	{
	if(ClassD.iWebDeter==2)
	ASSERT(0);
	}
	else
	{
	if(ClassD.iWebDeter==1)
	ASSERT(0);
	}
	*/

	// Gross Section
	if ( iClassSect == 1 || iClassSect == 2 || iClassSect == 3 )
	{
		// 
		EffeStlSect = GirderStiff;
		EffeComSect = dMc_Ed >= 0.0 ? RebarSect : NonConSect;
	}
	else if ( iClassSect == 4 )
	{
		for ( i = 0; i < 2; i++ )
		{
			if ( i == 0 )
			{
				SectTemp = GirderStiff;
			}
			else
			{
				SectTemp = dMc_Ed >= 0.0 ? RebarSect : NonConSect;
			}
			//
			if ( 1.0 - m_dZero < dPsi && dPsi < 1.0 + m_dZero )
			{
				dSubWidth = dh - dbe1 - dbe2;
				dSubZbar = dTF2 + dbe2 + 0.5*dSubWidth;
			}
			else if ( 0.0 <= dPsi && dPsi < 1.0 )
			{
				if ( iWebDeter == 1 ) // Sigma1 = Top.
				{
					dSubWidth = dh - dbe1 - dbe2;
					dSubZbar = dTF2 + dbe2 + 0.5*dSubWidth;
				}
				else if ( iWebDeter == 2 ) // Sigma2 = Bot.
				{
					dSubWidth = dh - dbe1 - dbe2;
					dSubZbar = dTF2 + dbe1 + 0.5*dSubWidth;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if ( dPsi < 0.0 )
			{
				if ( iWebDeter == 1 ) // 정모멘트와 같은 의미.
				{
					dbc = dH - SectTemp.Czm - dTF1;
					if ( dbc < 0.0 )
						ASSERT(0);
					dSubWidth = dbc - dbe1 - dbe2;
					dSubZbar = SectTemp.Czm + dbe2 + 0.5*dSubWidth;
				}
				else if ( iWebDeter == 2 ) // 부모멘트와 같은 의미.
				{
					dbc = NonConSect.Czm - dTF2;
					if ( dbc < 0.0 )
						ASSERT(0);
					dSubWidth = dbc - dbe1 - dbe2;
					dSubZbar = dTF2 + dbe1 + 0.5*dSubWidth;
				}
				else
				{
					ASSERT(0);
				}
			}
			//
			dSubArea = dSubWidth * dTw;
			dSubIyy = dTw * pow(dSubWidth, 3) / 12.0;

			if ( dSubWidth < 0.0 )
				ASSERT(0);

			// 정모멘트 부모멘트에 따라 Effective Area가 다르다. 둘다 고려해야 한다. 
			dEffArea = SectTemp.Area - dSubArea;
			dEffCzm = (SectTemp.Area*SectTemp.Czm - dSubArea * dSubZbar) / dEffArea;
			dEffCzp = SectTemp.Czp;
			dEffIyy = SectTemp.Ryy + SectTemp.Area*pow((SectTemp.Czm - dEffCzm), 2)
				- (dSubIyy + dSubArea * pow((dSubZbar - dEffCzm), 2));
			//
			if ( i == 0 )
			{
				EffeStlSect = SectTemp;
				EffeStlSect.Czm = dEffCzm;
				EffeStlSect.Czp = dEffCzp;
				EffeStlSect.Area = dEffArea;
				EffeStlSect.Ryy = dEffIyy;
				EffeStlSect.dy1 = SectTemp.dy1 + (SectTemp.Czm - dEffCzm);
				EffeStlSect.dy2 = SectTemp.dy2 + (SectTemp.Czm - dEffCzm);
				EffeStlSect.dy3 = SectTemp.dy3 + (SectTemp.Czm - dEffCzm);
				EffeStlSect.dy4 = SectTemp.dy4 + (SectTemp.Czm - dEffCzm);
			}
			else
			{
				EffeComSect = SectTemp;
				EffeComSect.Czm = dEffCzm;
				EffeComSect.Czp = dEffCzp;
				EffeComSect.Area = dEffArea;
				EffeComSect.Ryy = dEffIyy;
				EffeComSect.dy1 = SectTemp.dy1 + (SectTemp.Czm - dEffCzm);
				EffeComSect.dy2 = SectTemp.dy2 + (SectTemp.Czm - dEffCzm);
				EffeComSect.dy3 = SectTemp.dy3 + (SectTemp.Czm - dEffCzm);
				EffeComSect.dy4 = SectTemp.dy4 + (SectTemp.Czm - dEffCzm);
			}
		}
	}
	else ASSERT(0);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_StiffnessOfEffeSectCompo(BOOL bPositive, T_SECT_INFO_D& SectInfoD, T_SECT_STIFFNESS& EffeStiff)
{
	int nPartSize = SectInfoD.aSectPart.GetSize();
	if ( nPartSize <= 0 ) { return FALSE; }

	double dAreaSum = 0.0;
	double dArea_z_Sum = 0.0;
	double dRyy_Sum = 0.0;
	for ( int i = 0; i < nPartSize; ++i )
	{
		T_SECT_PART_D &PART = SectInfoD.aSectPart[i];
		double dn = 1.0;
		if ( i > 0 && SectInfoD.aSectPart[0].dElast > 0.0 )
		{
			if ( bPositive )
			{
				dn = PART.dElast / SectInfoD.aSectPart[0].dElast;
			}
			else
			{
				if ( SectInfoD.aSectPart[1].aRbarData.GetSize() > 0 )
				{
					dn = SectInfoD.aSectPart[1].aRbarData[0].dEs / SectInfoD.aSectPart[0].dElast;
				}
			}
		}
		T_SECT_STIFFNESS &Stiffness = PART.Stiff.Stiffness;

		double dAtr = dn * Stiffness.Area;
		dAreaSum += dAtr;
		dArea_z_Sum += dAtr * PART.Stiff.dZbar;
	}

	double dZcom = dAreaSum < m_dZero ? 0.0 : dArea_z_Sum / dAreaSum;

	for ( int i = 0; i < nPartSize; ++i )
	{
		T_SECT_PART_D &PART = SectInfoD.aSectPart[i];
		double dn = 1.0;
		if ( i > 0 && SectInfoD.aSectPart[0].dElast > 0.0 )
		{
			if ( bPositive )
			{
				dn = PART.dElast / SectInfoD.aSectPart[0].dElast;
			}
			else
			{
				if ( SectInfoD.aSectPart[1].aRbarData.GetSize() > 0 )
				{
					dn = SectInfoD.aSectPart[1].aRbarData[0].dEs / SectInfoD.aSectPart[0].dElast;
				}
			}
		}
		T_SECT_STIFFNESS &Stiffness = PART.Stiff.Stiffness;

		double dGap_z = dZcom - PART.Stiff.dZbar;
		dRyy_Sum += dn * (Stiffness.Ryy + Stiffness.Area*pow(dGap_z, 2));
	}

	T_SECT_PART_D &PART = SectInfoD.aSectPart[0];

	EffeStiff = PART.Stiff.Stiffness; // 거더의 강성을 저장한다.
	double dH_gr_temp = EffeStiff.Czp + EffeStiff.Czm;

	double dez = dZcom - PART.Stiff.Stiffness.Czm;
	EffeStiff.Area = dAreaSum; // 설계에 필요한 기본 테이터를 저장한다.
	EffeStiff.Ryy = dRyy_Sum;
	EffeStiff.Czm = dZcom;
	EffeStiff.Czp = dH_gr_temp - dZcom;
	EffeStiff.dy1 -= dez;
	EffeStiff.dy2 -= dez;
	EffeStiff.dy3 -= dez;
	EffeStiff.dy4 -= dez;

	return TRUE;
}

BOOL CDgnPlateGirder::Get_EffectiveCzmData4Sect(bool bPosiI, T_CLAS_BASE& ClassD, BOOL bPositive, T_SECT_D& SectD, CArray<CPG_LSTI_DATA, CPG_LSTI_DATA&>& arLstiPosi,
	T_DGNLINE_LBD& LineLBD, double& dA_eff, double& dCzm_eff, double& dIy_eff)
{
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dtw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dtf1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dtf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);

	CArray<T_STIFF_CALC_D, T_STIFF_CALC_D&> aStiffCalcD;
	T_STIFF_CALC_D StiffCalcD;

	// Top Flange Part
	double dA_tf = dB1 * dtf1;
	double dz_tf = dH - 0.5*dtf1;
	double dAz_tf = dA_tf * dz_tf;
	StiffCalcD.dArea = dA_tf;
	StiffCalcD.dIy = Get_InertiaOfSB(dtf1, dB1);
	StiffCalcD.dz = dz_tf;
	aStiffCalcD.Add(StiffCalcD);

	// Bottom Flange Part
	double dA_bf = dB2 * dtf2;
	double dz_bf = 0.5*dtf2;
	double dAz_bf = dA_bf * dz_bf;
	StiffCalcD.dArea = dA_bf;
	StiffCalcD.dIy = Get_InertiaOfSB(dtf2, dB2);
	StiffCalcD.dz = dz_bf;
	aStiffCalcD.Add(StiffCalcD);

	double dArea_eff = dA_tf + dA_bf;
	double dAz_eff = dAz_tf + dAz_bf;

	int iSubLineSize = LineLBD.aDgnBaseD.GetSize();
	int iLineStressType = LineLBD.iLineStressType;
	int iCompPanelSize = LineLBD.iCompPanelSize;
	BOOL bCompStr = FALSE, bDbCurve = FALSE;

	// -1=양연인장 
	// 0=양연압축이고 aDgnBaseD의 첫번째 dStr1이 Sig1(Sig1은 압축응력이며 둘 중 큰값)
	// 1=양연압축이고 aDgnBaseD의 마지막 dStr2가 Sig1
	// 2=양연압축/인장 aDgnBaseD의 첫번째 dStr1이 Sig1
	// 3=양연압축/인장 aDgnBaseD의 마지막 dStr2가 Sig1
	if ( iLineStressType == 0 || iLineStressType == 2 )
	{
		double dCurrZ = dH - dtf1;
		for ( int i = 0; i < iSubLineSize; i++ )
		{
			T_GLINE_DGN_B LineDgnB = LineLBD.aDgnBaseD[i];
			T_SUB_PANEL_BASE SubPanD = LineDgnB.SubPanelD;
			CPG_LSTI_DATA LstiD; LstiD.Initialize();
			double dbs = LineDgnB.db;
			double dts = LineDgnB.dt;
			double db_blank = SubPanD.db_blank_eff;
			double dAsp = dbs * dts;
			bCompStr = LineDgnB.bCompStr;
			bDbCurve = LineDgnB.bDbCurve;

			if ( bCompStr )
			{
				if ( !bDbCurve )
				{
					double db_sup_eff = SubPanD.db_sup_eff;
					double dA_sup_eff = SubPanD.dA_sup_eff;
					double dz_sup_eff = dCurrZ - 0.5*SubPanD.db_sup_eff;
					double dAz_sup_eff = dA_sup_eff * dz_sup_eff;
					StiffCalcD.dArea = dA_sup_eff;
					StiffCalcD.dIy = Get_InertiaOfSB(db_sup_eff, dts);
					StiffCalcD.dz = dz_sup_eff;
					aStiffCalcD.Add(StiffCalcD);

					//////////////////////////////////////////////////////////////////////////    
					double db_inf_eff = SubPanD.db_inf_eff;
					double dA_inf_eff = SubPanD.dA_inf_eff;
					double dz_inf_eff = dCurrZ - dbs + 0.5*SubPanD.db_inf_eff;
					double dAz_inf_eff = dA_sup_eff * dz_inf_eff;
					StiffCalcD.dArea = dA_inf_eff;
					StiffCalcD.dIy = Get_InertiaOfSB(db_inf_eff, dts);
					StiffCalcD.dz = dz_inf_eff;
					aStiffCalcD.Add(StiffCalcD);

					dArea_eff += dA_sup_eff + dA_inf_eff;
					dAz_eff += dAz_sup_eff + dAz_inf_eff;
				}
				else
				{
					// Test ******************************************
					// 압축 큰 곳 유효길이
					double db_sup_eff = SubPanD.db_sup_eff;
					double dA_sup_eff = SubPanD.dA_sup_eff;
					double dz_sup_eff = 0.0;
					if ( bPositive ) { dz_sup_eff = dCurrZ - 0.5*SubPanD.db_sup_eff; }
					else { dz_sup_eff = dCurrZ - dbs + 0.5*SubPanD.db_sup_eff; }

					double dAz_sup_eff = dA_sup_eff * dz_sup_eff;
					StiffCalcD.dArea = dA_sup_eff;
					StiffCalcD.dIy = Get_InertiaOfSB(db_sup_eff, dts);
					StiffCalcD.dz = dz_sup_eff;
					aStiffCalcD.Add(StiffCalcD);

					// 압축 작은 곳 유효길이
					double db_inf_eff = SubPanD.db_inf_eff;
					double dA_inf_eff = SubPanD.dA_inf_eff;
					double dz_inf_eff = 0.0;
					if ( bPositive ) { dz_inf_eff = dCurrZ - dbs + 0.5*SubPanD.db_inf_eff; }
					else { dz_inf_eff = dCurrZ - SubPanD.dbt - 0.5*SubPanD.db_inf_eff; }

					double dAz_inf_eff = dA_inf_eff * dz_inf_eff;
					StiffCalcD.dArea = dA_inf_eff;
					StiffCalcD.dIy = Get_InertiaOfSB(db_inf_eff, dts);
					StiffCalcD.dz = dz_inf_eff;
					aStiffCalcD.Add(StiffCalcD);

					dArea_eff += dA_sup_eff + dA_inf_eff;
					dAz_eff += dAz_sup_eff + dAz_inf_eff;

					//////////////////////////////////////////////////////////////////////////     
					// 인장축
					double dLen_ten = fabs(LineDgnB.dz2 - LineDgnB.dz0);
					double dA_ten = dLen_ten * LineDgnB.dt;

					double dz_ten = 0.0;
					if ( bPositive ) { dz_ten = dCurrZ - dbs + 0.5*dLen_ten; }
					else { dz_ten = dCurrZ - 0.5*dLen_ten; }

					double dAz_ten = dA_ten * dz_ten;
					StiffCalcD.dArea = dA_ten;
					StiffCalcD.dIy = Get_InertiaOfSB(dLen_ten, dts);
					StiffCalcD.dz = dz_ten;
					aStiffCalcD.Add(StiffCalcD);

					dArea_eff += dA_ten;
					dAz_eff += dAz_ten;
				}
			}
			else // 인장
			{
				double dA_ten = dbs * dts;
				double dz_ten = dCurrZ - 0.5*dbs;
				double dAz_ten = dA_ten * dz_ten;
				StiffCalcD.dArea = dA_ten;
				StiffCalcD.dIy = Get_InertiaOfSB(dbs, dts);
				StiffCalcD.dz = dz_ten;
				aStiffCalcD.Add(StiffCalcD);

				dArea_eff += dA_ten;
				dAz_eff += dA_ten * dz_ten;
			}

			if ( iSubLineSize > 1 )
			{
				if ( i < iSubLineSize - 1 )
				{
					LstiD = arLstiPosi[i];
					double db_sf = LstiD.dWidth + dts;
					double dt_sf = LstiD.dt;
					double dA_sw = dts * LstiD.dt; // web 일부분
					double dA_sf = LstiD.dArea + dts * LstiD.dt; // web 일부분 + stiffener
					double dz_sf = dH - dtf1 - LstiD.dz;
					double dAz_sf = dA_sf * dz_sf;
					double dAz_sw = dA_sw * dz_sf;
					StiffCalcD.dArea = dA_sw;
					StiffCalcD.dIy = Get_InertiaOfSB(dt_sf, dts); //Get_InertiaOfSB(dt_sf, db_sf);
					StiffCalcD.dz = dz_sf;
					aStiffCalcD.Add(StiffCalcD);

					dArea_eff += dA_sw;
					dAz_eff += dAz_sf;
				}
			}
			dCurrZ -= dbs + LstiD.dt;
		}
	}
	else if ( iLineStressType == 1 || iLineStressType == 3 )
	{
		double dCurrZ = dtf2;
		for ( int i = 0; i < iSubLineSize; i++ )
		{
			int idx = iSubLineSize - 1 - i;
			T_GLINE_DGN_B LineDgnB = LineLBD.aDgnBaseD[idx];
			T_SUB_PANEL_BASE SubPanD = LineDgnB.SubPanelD;
			CPG_LSTI_DATA LstiD; LstiD.Initialize();
			double dbs = LineDgnB.db;
			double dts = LineDgnB.dt;
			double dAsp = dbs * dts;
			bCompStr = LineDgnB.bCompStr;
			bDbCurve = LineDgnB.bDbCurve;

			if ( bCompStr )
			{
				if ( !bDbCurve )
				{
					double db_sup_eff = SubPanD.db_sup_eff;
					double dA_sup_eff = SubPanD.dA_sup_eff;
					double dz_sup_eff = dCurrZ + 0.5*SubPanD.db_sup_eff;
					double dAz_sup_eff = dA_sup_eff * dz_sup_eff;
					StiffCalcD.dArea = dA_sup_eff;
					StiffCalcD.dIy = Get_InertiaOfSB(db_sup_eff, dts);
					StiffCalcD.dz = dz_sup_eff;
					aStiffCalcD.Add(StiffCalcD);

					//////////////////////////////////////////////////////////////////////////    
					double db_inf_eff = SubPanD.db_inf_eff;
					double dA_inf_eff = SubPanD.dA_inf_eff;
					double dz_inf_eff = dCurrZ + dbs - 0.5*SubPanD.db_inf_eff;
					double dAz_inf_eff = dA_sup_eff * dz_inf_eff;
					StiffCalcD.dArea = dA_inf_eff;
					StiffCalcD.dIy = Get_InertiaOfSB(db_inf_eff, dts);
					StiffCalcD.dz = dz_inf_eff;
					aStiffCalcD.Add(StiffCalcD);

					dArea_eff += dA_sup_eff + dA_inf_eff;
					dAz_eff += dAz_sup_eff + dAz_inf_eff;
				}
				else
				{
					double db_sup_eff = SubPanD.db_sup_eff;
					double dA_sup_eff = SubPanD.dA_sup_eff;
					double dz_sup_eff = dCurrZ + 0.5*SubPanD.db_sup_eff;
					double dAz_sup_eff = dA_sup_eff * dz_sup_eff;
					StiffCalcD.dArea = dA_sup_eff;
					StiffCalcD.dIy = Get_InertiaOfSB(db_sup_eff, dts);
					StiffCalcD.dz = dz_sup_eff;
					aStiffCalcD.Add(StiffCalcD);

					// 압축 작은 곳 유효길이
					double db_inf_eff = SubPanD.db_inf_eff;
					double dA_inf_eff = SubPanD.dA_inf_eff;
					double dz_inf_eff = 0.0;
					if ( bPositive ) { dz_inf_eff = dCurrZ - SubPanD.dbc + 0.5*SubPanD.db_inf_eff; }
					else { dz_inf_eff = dCurrZ + SubPanD.dbc - 0.5*SubPanD.db_inf_eff; }

					double dAz_inf_eff = dA_inf_eff * dz_inf_eff;
					StiffCalcD.dArea = dA_inf_eff;
					StiffCalcD.dIy = Get_InertiaOfSB(db_inf_eff, dts);
					StiffCalcD.dz = dz_inf_eff;
					aStiffCalcD.Add(StiffCalcD);


					//////////////////////////////////////////////////////////////////////////          
					double dLen_ten = fabs(LineDgnB.dz2 - LineDgnB.dz0);
					double dA_ten = dLen_ten * LineDgnB.dt;
					double dz_ten = dCurrZ + dbs - 0.5*dLen_ten;
					double dAz_ten = dA_ten * dz_ten;
					StiffCalcD.dArea = dA_ten;
					StiffCalcD.dIy = Get_InertiaOfSB(dLen_ten, dts);
					StiffCalcD.dz = dz_ten;
					aStiffCalcD.Add(StiffCalcD);

					dArea_eff += dA_sup_eff + dA_inf_eff + dA_ten;
					dAz_eff += dAz_sup_eff + dAz_inf_eff + dAz_ten;
				}
			}
			else // 인장
			{
				double dA_ten = dbs * dts;
				double dz_ten = dCurrZ + 0.5*dbs;
				double dAz_ten = dA_ten * dz_ten;
				StiffCalcD.dArea = dA_ten;
				StiffCalcD.dIy = Get_InertiaOfSB(dbs, dts);
				StiffCalcD.dz = dz_ten;
				aStiffCalcD.Add(StiffCalcD);

				dArea_eff += dA_ten;
				dAz_eff += dA_ten * dz_ten;
			}
			if ( iSubLineSize > 1 )
			{
				int nLSIndex = idx - 1;
				if ( 0 < idx && idx < iSubLineSize )
				{
					LstiD = arLstiPosi[idx - 1];
					double db_sf = LstiD.dWidth + dts;
					double dt_sf = LstiD.dt;
					double dA_sw = dts * LstiD.dt; // web 일부분
					double dA_sf = LstiD.dArea + dA_sw; // web 일부분 + stiffener
					double dz_sf = dH - dtf1 - LstiD.dz;
					double dAz_sf = dA_sf * dz_sf;
					double dAz_sw = dA_sw * dz_sf;
					StiffCalcD.dArea = dA_sw;
					StiffCalcD.dIy = Get_InertiaOfSB(dt_sf, dts); //Get_InertiaOfSB(dt_sf, db_sf);
					StiffCalcD.dz = dz_sf;
					aStiffCalcD.Add(StiffCalcD);

					dArea_eff += dA_sw;
					dAz_eff += dAz_sf;
				}
			}
			dCurrZ += dbs + LstiD.dt;
		}
	}
	else if ( iLineStressType == -1 )
	{
		StiffCalcD.dArea = dh * dtw;
		StiffCalcD.dIy = Get_InertiaOfSB(dh, dtw);
		StiffCalcD.dz = dtf2 + 0.5*dh;
		aStiffCalcD.Add(StiffCalcD);

		dArea_eff += dh * dtw;
		dAz_eff += dh * dtw*(dtf2 + 0.5*dh);
	}
	else
		ASSERT(0);

	dCzm_eff = dArea_eff < m_dZero ? 0.0 : dAz_eff / dArea_eff;

	double dCzm_eff2 = 0.0, dIy_eff2 = 0.0;
	Get_StiffnessOfEffeSect(aStiffCalcD, dCzm_eff2, dIy_eff2);

	dA_eff = dArea_eff;
	dCzm_eff = dCzm_eff2;
	dIy_eff = dIy_eff2;

	return TRUE;
}

BOOL CDgnPlateGirder::Get_LineDgnInfo4SectDB(BOOL bIsBefore, BOOL bPositiveM, bool bPosiI, T_CLAS_BASE& ClassD, double dfy1, double dfy2, double dEs, double dPoisson, double* aForceGd, double* aForce, T_SECT_D& SectD,
	double da, const CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&>& aStiffness, CArray<CPG_LSTI_DATA, CPG_LSTI_DATA&>& arLstiPosi,
	CArray<T_GLINE_DGN_B, T_GLINE_DGN_B&>& aLineDgnBase, T_DGNLINE_LBD& LineLBD)
{
	if ( aStiffness.GetSize() < 3 )
	{
		ASSERT(0); return FALSE;
	}
	double dFx_g = aForceGd[0], dMy_g = aForceGd[4], dMz_g = aForceGd[5];
	double dFx = aForce[0], dMy = aForce[4], dMz = aForce[5];
	//
	const _DGN_FORC_CRC& ScfpForce = GetScfpForce4Lcom();
	int nPosId = bPosiI ? 0 : 1;
	double dFx_sc = ScfpForce.dFxx[nPosId];
	double dMy_sc = ScfpForce.dMuy[nPosId];
	double dMz_sc = ScfpForce.dMuz[nPosId];
	//dFx_c -= dFx_sc;
	//dMy_c -= dMy_sc;
	//dMz_c -= dMz_sc;

	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dtw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dtf1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2  = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dtf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dh   = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);

	T_SECT_STIFFNESS AppStiffGd = aStiffness[0];
	double dArea_g = AppStiffGd.Area;
	double dIy_g   = AppStiffGd.Ryy;
	double dIz_g   = AppStiffGd.Rzz;

	T_SECT_STIFFNESS AppStiff = bPositiveM ? aStiffness[1] : aStiffness[2];
	double dArea   = AppStiff.Area;
	double dIy     = AppStiff.Ryy;
	double dIz     = AppStiff.Rzz;

	double dy1 = 0., dz1 = 0., dy2 = 0., dz2 = 0., dy3 = 0., dz3 = 0., dy4 = 0., dz4 = 0.;
	AppStiff.GetPosition(dy1, dz1, dy2, dz2, dy3, dz3, dy4, dz4);
	double dbi = 0.0, dti = 0.0;
	double dyi1 = 0.0, dyi2 = 0.0, dzi1 = 0.0, dzi2 = 0.0;

	int iStressType = 0, iNtype = 0, iCompPanelSize = 0;
	double dStressI = 0.0, dStressJ = 0.0, dStr1 = 0.0, dStr2 = 0.0, df1 = 0.0, df2 = 0.0;
	double dbc_sum = 0.0, dbt_sum = 0.0, dk_sig = 0.0, dLamda_p = 0.0, dPsi = 0.0, dbs = 0.0, dts = 0.0;
	BOOL bInternal = TRUE;
	BOOL bIs1_sig1 = FALSE;
	BOOL bLineDbCurve = FALSE;

	int iLineDgnSize = aLineDgnBase.GetSize();

	for ( int i = 0; i < iLineDgnSize; i++ )
	{
		T_GLINE_DGN_B LineDgn = aLineDgnBase[i];
		dbs = LineDgn.db;
		dts = LineDgn.dt;
		double dfyi = Get_fy4Thick(dfy1, dfy2, dts);
		double dEpsilon = Get_Epsilon(dfyi);

		double dStr1G = Calc_StressOfPosition(dFx_g, dMy_g, dMz_g, dArea_g, dIy_g, dIz_g, LineDgn.dy1, LineDgn.dz1);
		double dStr2G = Calc_StressOfPosition(dFx_g, dMy_g, dMz_g, dArea_g, dIy_g, dIz_g, LineDgn.dy2, LineDgn.dz2);

		double dGapz = AppStiffGd.Czm - AppStiff.Czm;
		double dStr1C = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, LineDgn.dy1, LineDgn.dz1 + dGapz);
		double dStr2C = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, LineDgn.dy2, LineDgn.dz2 + dGapz);

		double dStr1_temp = dStr1G + dStr1C;
		double dStr2_temp = dStr2G + dStr2C;

		double df1Const = CalcStressInterpolate(nPosId, AppStiff, LineDgn.dy1, LineDgn.dz1);
		double df2Const = CalcStressInterpolate(nPosId, AppStiff, LineDgn.dy2, LineDgn.dz2);
		dStr1_temp += df1Const;
		dStr2_temp += df2Const;

		// test	*************************
		if ( bPositiveM ) // Positive
		{
			dStr1 = dStr1_temp;
			dStr2 = dStr2_temp;
		}
		else
		{
			dStr1 = dStr2_temp;
			dStr2 = dStr1_temp;
		}
		// ******************************

		Get_f1_f2(dStr1, dStr2, df1, df2, bIs1_sig1);
		LineDgn.dStr1 = dStr1;
		LineDgn.dStr2 = dStr2;
		LineDgn.df1 = df1;   // psi 계산 위한 응력값. (f1: 압축응력(+) 큰 것   
		LineDgn.df2 = df2;
		LineDgn.bIs1_sig1 = bIs1_sig1;
		BOOL bCompStr = Is_CompStress(dStr1, dStr2);
		BOOL bDbCurve = Is_DoubleCurvature(dStr1, dStr2);
		LineDgn.bCompStr = bCompStr;
		LineDgn.bDbCurve = bDbCurve;

		if ( i == 0 ) { dStressI = dStr1, dStressJ = dStr2; }
		else { dStressJ = dStr2; }

		iStressType = Get_StressDistributionType4Internal(dStr1, dStr2);
		T_SUB_PANEL_BASE SubPanelD;
		if ( bCompStr )
		{
			iCompPanelSize++;
			dPsi = Get_Psi(df1, df2);
			dk_sig = Get_InternalBucklingfactor(dPsi);
			dLamda_p = Get_Lamda_p(dbs, dts, dEpsilon, dk_sig);
			double dRho = 1.0;
			if ( ClassD.iWebClass == 4 )
			{
				Get_ReductionFactorRho(bInternal, dbs, dts, dLamda_p, dPsi, dRho);
			}

			SubPanelD.nType = iStressType;
			SubPanelD.dPsi = dPsi;
			SubPanelD.dk_sigma = dk_sig;
			SubPanelD.dLamda_p = dLamda_p;
			SubPanelD.dRho = dRho;
			int iNtype = 0, iNtype2 = 0;
			BOOL bGrossWidth = TRUE;
			double dbb = 0.0, db_eff = 0.0;

			if ( bInternal )
			{
				if ( !bDbCurve )
				{
					dbc_sum += dbs;
					iNtype = Get_StreeTypeOfStiffenedPlates(dStr1, dStr2, iNtype2);
					// Gross width
					bGrossWidth = TRUE;
					dbb = Get_Width4StiffenedPlates(iNtype, dbs, dPsi);
					Set_CalcSubPanel(bGrossWidth, iNtype, dbb, dts, SubPanelD);
					//
					dbb = Get_Width4StiffenedPlates(iNtype2, dbs, dPsi);
					Set_CalcSubPanel(bGrossWidth, iNtype2, dbb, dts, SubPanelD);
					// Effective width
					bGrossWidth = FALSE;
					db_eff = dRho * dbs;
					dbb = Get_Width4StiffenedPlates(iNtype, db_eff, dPsi);
					Set_CalcSubPanel(bGrossWidth, iNtype, dbb, dts, SubPanelD);
					//
					dbb = Get_Width4StiffenedPlates(iNtype2, db_eff, dPsi);
					Set_CalcSubPanel(bGrossWidth, iNtype2, dbb, dts, SubPanelD);

					SubPanelD.db_blank_eff = dbs - db_eff;
				}
				else
				{
					// test  *******************
					if ( TRUE )
					{
						bLineDbCurve = TRUE; // 하나라도 있으면. 

						double dbeff = 0.0;
						double dbe1 = 0.0;
						double dbe2 = 0.0;
						Get_EffectiveWidth4Internal(dPsi, dbs, dRho, dbeff, dbe1, dbe2);

						double dbc = 0.0; // 압축영역길이.
						Get_CompWidthOfDoubleCurvatrue(LineDgn, dbc);

						dbc_sum += dbeff;       // 유효압축영역
						dbt_sum += dbs - dbc;   // 인장영역
						SubPanelD.dbc = dbc;
						SubPanelD.dbt = dbs - dbc;

						SubPanelD.db_sup_eff = dbe1;        // 압축 큰곳
						SubPanelD.dA_sup_eff = dbe1 * dts;
						SubPanelD.db_inf_eff = dbe2;
						SubPanelD.dA_inf_eff = dbe2 * dts;
						SubPanelD.db_blank_eff = dbc - dbeff;

					}// *****************************
					else
					{
						ASSERT(0);
						bLineDbCurve = TRUE; // 하나라도 있으면. 
						double dbc = 0.0;
						Get_CompWidthOfDoubleCurvatrue(LineDgn, dbc);
						dbc_sum += dbc;         // 압축영역
						dbt_sum += dbs - dbc;   // 인장영역
						SubPanelD.db_sup = dbc;
						iNtype = Get_StreeTypeOfStiffenedPlates(dStr1, dStr2, iNtype2);
						dbb = Get_Width4StiffenedPlates(iNtype, dbc, dPsi, TRUE);
						SubPanelD.db_sup_eff = dbb;
						SubPanelD.dA_sup_eff = dbb * dts;
					}
				}
			}
		}
		else // Tension
		{

		}

		LineDgn.SubPanelD = SubPanelD;
		aLineDgnBase.SetAt(i, LineDgn);
	}
	LineLBD.aDgnBaseD.Copy(aLineDgnBase);

	int iLineStressType = -1;
	BOOL bIs1_f1 = FALSE;
	double df1_temp = 0.0, df2_temp = 0.0;
	Get_f1_f2(dStressI, dStressJ, df1_temp, df2_temp, bIs1_f1);
	if ( !bLineDbCurve )
	{
		if ( bIs1_f1 ) iLineStressType = 0;
		else        iLineStressType = 1;
	}
	else
	{
		if ( bIs1_f1 ) iLineStressType = 2;
		else        iLineStressType = 3;
	}

	// Stiffener
	double dAsl_sum = 0.0;
	double dIsl_sum = 0.0;
	double dIp = 0.0;
	int iSubPanelSize = iLineDgnSize;
	if ( bInternal && iSubPanelSize > 1 )
	{
		LineLBD.aDgnStfnD.SetSize(iSubPanelSize - 1);
		dIp = Get_Ip_4BendingOfPlate(dh, dtw, dPoisson);
		for ( int i = 0; i < iSubPanelSize - 1; i++ )
		{
			//T_GLINE_STIFFENER StfnD = LineDst.aStiffener[i];
			CPG_LSTI_DATA LstiD = arLstiPosi[i];
			double db_ls = LstiD.dWidth;
			double dt_ls = LstiD.dt;
			double dA_ls = LstiD.dArea;
			T_GLINE_DGN_B PreLineDgn = aLineDgnBase[i];
			T_GLINE_DGN_B NextLineDgn = aLineDgnBase[i + 1];
			int iPreStrType = PreLineDgn.SubPanelD.nType;
			int iNextStrType = NextLineDgn.SubPanelD.nType;
			double db_inf = 0.0, db_sup = 0.0, db_inf_eff = 0.0, db_sup_eff = 0.0, dt_inf = 0.0, dt_sup = 0.0;

			if ( iPreStrType == Internal_Stress_Type1 || iPreStrType == Internal_Stress_Type2 )
			{
				db_inf = PreLineDgn.SubPanelD.db_inf;
				db_sup = NextLineDgn.SubPanelD.db_sup;
				db_inf_eff = PreLineDgn.SubPanelD.db_inf_eff;
				db_sup_eff = NextLineDgn.SubPanelD.db_sup_eff;
				dt_inf = PreLineDgn.dt;
				dt_sup = NextLineDgn.dt;
			}
			else if ( iPreStrType == Internal_Stress_Type3 )
			{
				db_inf = NextLineDgn.SubPanelD.db_inf;
				db_sup = PreLineDgn.SubPanelD.db_sup;
				db_inf_eff = NextLineDgn.SubPanelD.db_inf_eff;
				db_sup_eff = PreLineDgn.SubPanelD.db_sup_eff;
				dt_inf = NextLineDgn.dt;
				dt_sup = PreLineDgn.dt;
			}
			else
			{
				// 인장. 
			}

			double dt_m = dt_inf;
			double dA_sf = LstiD.dArea;//StfnD.dA;
			double dI_sf = Get_InertiaOfStiffener(db_ls, dt_ls);
			double dZbar_sf = 0.5*db_ls;

			double dA_inf = db_inf * dt_inf;
			double dA_sup = db_sup * dt_sup;
			double dA_mc = dt_ls * dt_m;
			double dAsl = dA_inf + dA_sup + dA_mc + dA_sf;
			double dSf_bar = 0.5*dt_m + dZbar_sf;
			double dXsl = fabs(dAsl) < m_dZero ? 0.0 : dSf_bar * dA_sf / dAsl; // 모재 두께의 1/2이 기준. 
			double dI_inf = db_inf * pow(dt_inf, 3) / 12.0;
			double dI_sup = db_sup * pow(dt_sup, 3) / 12.0;
			double dI_mc = dt_ls * pow(dt_m, 3) / 12.0;
			double dIsl = (dI_inf + dI_sup + dI_mc) + (dA_inf + dA_sup + dA_mc)*pow(dXsl, 2) + dI_sf + dA_sf * pow((dSf_bar - dXsl), 2);
			double de1 = dXsl;
			double de2 = dSf_bar - dXsl;
			double dSigma_sl = Get_Sigma_cr_sl4ColumnTypeBehaviour(dEs, dIsl, dAsl, da);
			//
			double dA_inf_eff = db_inf_eff * dt_inf;
			double dA_sup_eff = db_sup_eff * dt_sup;
			double dAsl_eff = dA_inf_eff + dA_sup_eff + dA_sf;

			dAsl_sum += dAsl;
			dIsl_sum += dIsl;

			T_GLINE_DGN_STFN DgnStfn;
			DgnStfn.dA = dA_sf;
			DgnStfn.dIy = dI_sf;
			DgnStfn.dZbar = dZbar_sf;
			DgnStfn.db_inf = db_inf;
			DgnStfn.db_inf = db_inf_eff;
			DgnStfn.db_sup = db_sup;
			DgnStfn.db_sup_eff = db_sup_eff;
			DgnStfn.dAsl = dAsl;
			DgnStfn.dIsl = dIsl;
			DgnStfn.dXsl = dXsl;
			DgnStfn.de1 = de1;
			DgnStfn.de2 = de2;
			DgnStfn.de = max(de1, de2);
			DgnStfn.dAsl_eff = dAsl_eff;
			DgnStfn.dSigma_sl = dSigma_sl;

			LineLBD.aDgnStfnD.SetAt(i, DgnStfn);
		}
	}

	//  
	LineLBD.dbc = dbc_sum;
	LineLBD.dbt = dbt_sum;
	LineLBD.dt = dtw;
	LineLBD.iLineStressType = iLineStressType;
	LineLBD.iCompPanelSize = iCompPanelSize;
	LineLBD.dAsl_sum = dAsl_sum;
	LineLBD.dIsl_sum = dIsl_sum;
	LineLBD.dIp = dIp;

	//    
	double dfyb = Get_fy4Thick(dfy1, dfy2, dtw);
	Get_ElasticCriticalPlateBucklingData(dfyb, da, dEs, dPoisson, aForce, LineLBD);
	//
	BOOL bIsOpenStfn = TRUE; // I형은 직사각형 보강재만 입력가능 
	Get_ColumnBucklingData(dfyb, da, dEs, dPoisson, bIsOpenStfn, aForce, LineLBD);

	T_DGNLINE_PTB PlateBuckD = LineLBD.PlateBuckD;
	double dSigma_cr_p = PlateBuckD.dSigma_cr_p;
	double dRho_p = PlateBuckD.dRho;

	T_DGNLINE_CTB ColumnBuckD = LineLBD.ColumnBuckD;
	double dSigma_cr_c = ColumnBuckD.dSigma_cr_c;
	double dXc = ColumnBuckD.dXc;

	double dxi = Get_xi(dSigma_cr_p, dSigma_cr_c);
	double dRho_c = Get_Rho_c_FinalReductionFactor(dRho_p, dXc, dxi);

	LineLBD.dxi = dxi;
	LineLBD.dRho_c = dRho_c;

	return TRUE;
}

BOOL CDgnPlateGirder::Get_LineInfo4SectDB(bool bPosiI, T_SECT_D& SectD, const T_SECT_STIFFNESS& AppStiffD, CArray<CPG_LSTI_DATA, CPG_LSTI_DATA&>& arLstiPosi,
	CArray<T_GLINE_DGN_B, T_GLINE_DGN_B&>& aLineDgnBase)
{
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dtw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dtf1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dtf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dCym = AppStiffD.Cym;
	double dCzm = AppStiffD.Czm;

	int iStiffSize = arLstiPosi.GetSize();
	int iLineDgnSize = iStiffSize + 1;
	aLineDgnBase.SetSize(iLineDgnSize);

	for ( int i = 0; i < iLineDgnSize; i++ )
	{
		CPG_LSTI_DATA LstiD; LstiD.Initialize();
		if ( iStiffSize != i ) { LstiD = arLstiPosi[i]; }
		T_GLINE_DGN_B LineDgn;
		LineDgn.iLineType = 0; // 0=Internal, 1=Outstand
		// 먼저 좌하단 원점좌표 기준으로 만들자. 
		double dyi = dCym, dzi1 = 0.0, dzi2 = 0.0, dbi = 0.0;
		double dStiffSta = dH - dtf1;

		if ( i == 0 ) // first
		{
			if ( iLineDgnSize == 1 )
			{
				dzi1 = dH - dtf1;
				dzi2 = dtf2;
			}
			else
			{
				dzi1 = dH - dtf1;
				dzi2 = dzi1 - (LstiD.dz - 0.5*LstiD.dt);
			}
		}
		else if ( iLineDgnSize - 1 == i ) // last
		{
			CPG_LSTI_DATA PreLstiD = arLstiPosi[i - 1];
			dzi1 = dStiffSta - (PreLstiD.dz + 0.5*PreLstiD.dt);
			dzi2 = dtf2;
		}
		else // mid
		{
			CPG_LSTI_DATA PreLstiD = arLstiPosi[i - 1];
			dzi1 = dStiffSta - (PreLstiD.dz + 0.5*PreLstiD.dt);
			dzi2 = dStiffSta - (LstiD.dz - 0.5*LstiD.dt);
		}
		dbi = dzi1 - dzi2;
		LineDgn.dy1 = dyi;
		LineDgn.dz1 = dzi1;
		LineDgn.dy2 = dyi;
		LineDgn.dz2 = dzi2;
		LineDgn.db = dbi;
		LineDgn.dt = dtw;
		aLineDgnBase.SetAt(i, LineDgn);
	}

	// 도심기준으로 좌표를 변환 
	Get_ConvertPoint4Centroid(dCym, dCzm, aLineDgnBase);

	return TRUE;
}

double CDgnPlateGirder::Get_InertiaOfStiffener(double db, double dt)
{
	double dIy = dt * pow(db, 3) / 12.0;
	return dIy;
}

BOOL CDgnPlateGirder::Get_StressOfPosition(double aGirderForce[6], double aCptForce[6], T_SECT_STIFFNESS& GdStiffD, T_SECT_STIFFNESS& StiffD, double aStress[4])
{
	double dArea_a = GdStiffD.Area;
	double dIy_a = GdStiffD.Ryy;
	double dIz_a = GdStiffD.Rzz;
	double dArea = StiffD.Area;
	double dIy = StiffD.Ryy;
	double dIz = StiffD.Rzz;

	double dFx_a = aGirderForce[0];
	double dMy_a = aGirderForce[4];
	double dMz_a = aGirderForce[5];
	double dFx_c = aCptForce[0];
	double dMy_c = aCptForce[4];
	double dMz_c = aCptForce[5];

	double aStress_a[4] = { 0.0 }, aStress_c[4] = { 0.0 };
	// Girder
	aStress_a[0] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, GdStiffD.dx1, GdStiffD.dy1);
	aStress_a[1] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, GdStiffD.dx2, GdStiffD.dy2);
	aStress_a[2] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, GdStiffD.dx3, GdStiffD.dy3);
	aStress_a[3] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, GdStiffD.dx4, GdStiffD.dy4);
	// Composite
	aStress_c[0] = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea, dIy, dIz, StiffD.dx1, StiffD.dy1);
	aStress_c[1] = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea, dIy, dIz, StiffD.dx2, StiffD.dy2);
	aStress_c[2] = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea, dIy, dIz, StiffD.dx3, StiffD.dy3);
	aStress_c[3] = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea, dIy, dIz, StiffD.dx4, StiffD.dy4);

	for ( int i = 0; i < 4; i++ )
	{
		aStress[i] = aStress_a[i] + aStress_c[i];
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_ConstraintStress(int nPos, const _DGN_SCFP_PART_LCASE& Cr, const _DGN_SCFP_PART_LCASE& Sh, const _DGN_SCFP_PART_LCASE& Te, CPG_STRESS_CONS &rData)
{
	if ( nPos < 0 || nPos>1 )
	{
		return FALSE;
	}

	_DGN_SCFP_PART_LCASE ConsStr;
	ConsStr += Cr;
	ConsStr += Sh;
	ConsStr += Te;

	Get_ConstraintStressLCase(nPos, ConsStr, rData);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_ConstraintStressLCase(int nPos, const _DGN_SCFP_PART_LCASE& Cons, CPG_STRESS_CONS &rData)
{
	if ( nPos < 0 || nPos>1 )
	{
		return FALSE;
	}
	int nPart = 1; // Girder
	Calc_ConstraintStress4Pos(nPos, nPart, Cons, rData.GirA.dStr);
	nPart = 2; // Slab
	Calc_ConstraintStress4Pos(nPos, nPart, Cons, rData.Slab.dStr);

	return TRUE;
}

BOOL CDgnPlateGirder::Calc_ConstraintStress4Pos(int nPos, int nPart, const _DGN_SCFP_PART_LCASE& Cons, double adStr[4])
{
	if ( nPos < 0 || nPos>1 )
	{
		return FALSE;
	}

	if ( nPart < 1 || nPart>2 )
	{
		return FALSE;
	}

	const _DGN_SCFP_D& ConStr = (nPart == 1) ? Cons.ForceP1 : Cons.ForceP2;
	double dStrA = ConStr.dbLStress[nPos][0];
	int nIDy = 9;
	adStr[0] = dStrA + ConStr.dbLStress[nPos][1] + ConStr.dbLStress[nPos][nIDy + 1];
	adStr[1] = dStrA + ConStr.dbLStress[nPos][1] + ConStr.dbLStress[nPos][nIDy + 9];
	adStr[2] = dStrA + ConStr.dbLStress[nPos][9] + ConStr.dbLStress[nPos][nIDy + 9];
	adStr[3] = dStrA + ConStr.dbLStress[nPos][9] + ConStr.dbLStress[nPos][nIDy + 1];

	return TRUE;
}

BOOL CDgnPlateGirder::SetPartStress(int nPos, const _DGN_STRS_CRC& P1, const _DGN_STRS_CRC& P2, T_PGBS_BASE& PgbsB, T_PGBS_DETAIL& Det)
{
	if ( nPos < 0 || nPos>1 )
	{
		return FALSE;
	}

	PgbsB.ChkGirder.dStress[0] = P1.dStl[nPos];
	PgbsB.ChkGirder.dStress[1] = P1.dStr[nPos];
	PgbsB.ChkGirder.dStress[2] = P1.dSbr[nPos];
	PgbsB.ChkGirder.dStress[3] = P1.dSbl[nPos];
	//
	Det.StrD.Slab.dStr[0] = P2.dStl[nPos];
	Det.StrD.Slab.dStr[1] = P2.dStr[nPos];
	Det.StrD.Slab.dStr[2] = P2.dSbr[nPos];
	Det.StrD.Slab.dStr[3] = P2.dSbl[nPos];

	return TRUE;
}

BOOL CDgnPlateGirder::Get_ConvertPoint4Centroid(double dCym, double dCzm, CArray<T_GLINE_DGN_B, T_GLINE_DGN_B&>& aLineDgnBase)
{
	// CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&> aLineDgnBase의 좌표가 원점 기준일때만 적용
	int iLineDgnSize = aLineDgnBase.GetSize();
	// 도심기준으로 좌표를 변환 
	for ( int i = 0; i < iLineDgnSize; i++ )
	{
		T_GLINE_DGN_B LineDgn = aLineDgnBase[i];
		LineDgn.dy1 -= dCym;
		LineDgn.dy2 -= dCym;
		LineDgn.dz1 -= dCzm;
		LineDgn.dz2 -= dCzm;
		aLineDgnBase.SetAt(i, LineDgn);
	}
	return TRUE;
}

BOOL CDgnPlateGirder::Get_InternalWebClassClassification(bool bPosiI, int iBotClass, double aForce[6], double dStress[4], CPG_MATL_ELEM& MatlElem,
	const T_SECT_D& crSectD, CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&>& arRbarPosi,
	int& iClass, T_CLAS_BASE& ClasBase)
{
	iClass = 0;
	ClasBase.iWebDeter = 0;
	ClasBase.dPsi[4] = 0.0;
	ClasBase.dk_sig[4] = 0.0;
	ClasBase.dRho[4] = 0.0;
	ClasBase.dbeff[4] = 0.0;
	ClasBase.dbe1[4] = 0.0;
	ClasBase.dbe2[4] = 0.0;

	double dStressTL = dStress[0];
	double dStressTR = dStress[1];
	double dStressBR = dStress[2];
	double dStressBL = dStress[3];

	if ( dStressTL > 0.0 && dStressTR > 0.0 && dStressBR > 0.0 && dStressBL > 0.0 )
	{
		iClass = 1;
		return TRUE;
	}

	double dFx = aForce[0];
	double dMy = aForce[4];
	double dMz = aForce[5];
	BOOL bPositiveMy = dMy > -m_dZero;

	// Material
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;

	double dfy = MatlElem.MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatlElem.MatdD.Data1.Design.S_Fy2;
	double dfck = MatlElem.dfck;
	double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
	double dfcd = dfck / dgam_c;
	double dfsd = dfsk / dgam_sr;

	// Section
	double dH = m_pDgnSectUtil->Get_Size_H(&crSectD, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&crSectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dTf1 = m_pDgnSectUtil->Get_Size_tft(&crSectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&crSectD, bPosiI);
	double dTf2 = m_pDgnSectUtil->Get_Size_tfb(&crSectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&crSectD, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&crSectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&crSectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&crSectD);

	double dfyt = Get_fy4Thick(dfy, dfy1, dTf1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTf2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;

	double dPlasticNA = 0.0;
	double dConcF = 0.0, dRebarF = 0.0;
	double dStopfF = 0.0, dSbotfF = 0.0, dSwebF = 0.0, dGirderF = 0.0;
	BOOL bPlasticClass = FALSE;
	BOOL bDeterClass = TRUE;

	double dEpsi = Get_Epsilon(dfyw);
	double dct = dh / dTw;
	double dbt = 0.0, dbc = 0.0, dc = 0.0;
	double dAlpha = 0.0;
	double dPsi = 0.0;
	double dk_sig = 0.0;
	double dLamda_p = 0.0;
	double dRho = 0.0;
	double dbeff = 0.0;
	double dbe1 = 0.0, dbe2 = 0.0;

	int iWebDeter = 0;
	BOOL bInternal = TRUE;
	BOOL bTensionWeb = FALSE;

	if ( bPositiveMy )
	{
		if ( dTf2 < m_dPlasticNA && m_dPlasticNA < dTf2 + dh + m_dZero ) // PNA in Web
		{
			dbc = dTf2 + dh - m_dPlasticNA;
		}
		else if ( dTf2 > m_dPlasticNA ) // PNA in Bottom Flange
		{
			dbc = dh;
		}
		else if ( dTf2 + dh < m_dPlasticNA ) // PNA in Top Flange
		{
			bTensionWeb = TRUE;
		}

		dAlpha = dbc / dh;
	}
	else
	{
		if ( dTf2 < m_dPlasticNA && m_dPlasticNA < dTf2 + dh + m_dZero ) // PNA in Web
		{
			dbc = m_dPlasticNA - dTf2;
		}
		else if ( dTf2 > m_dPlasticNA )// PNA in Bottom Flange
		{
			bTensionWeb = TRUE;
		}
		else if ( dTf2 + dh < m_dPlasticNA ) // PNA in Top Flange
		{
			dbc = dh;
		}

		dAlpha = dbc / dh;
	}

	if ( !bTensionWeb )
	{
		iClass = Get_WebPlaticClass(dct, dEpsi, dAlpha);
	}
	else
	{
		iClass = 1;
	}

	// Plastic으로 결정이 되면 리턴.  
	double dStressT = (dStressTL + dStressTR) / 2.0;
	double dStressB = (dStressBL + dStressBR) / 2.0;

	if ( iClass > 2 )
	{
		if ( dStressT == 0.0 && dStressB == 0.0 )
		{
			iClass = 1;
			bDeterClass = TRUE;
		}
		else if ( dStressT < 0.0 && dStressB < 0.0 )
		{
			dAlpha = 1.0;
			dbc = dh;
			if ( fabs(dStressT - dStressB) < m_dZero )
			{
				dPsi = 1.0;
				dk_sig = Get_InternalBucklingfactor(dPsi);
				dLamda_p = Get_Lamda_p(dh, dTw, dEpsi, dk_sig);
				Get_ReductionFactorRho(bInternal, dh, dTw, dLamda_p, dPsi, dRho);
				dbeff = dRho * dbc;
				dbe1 = 0.5*dbeff;
				dbe2 = 0.5*dbeff;
			}
			else
			{
				if ( bPositiveMy )
				{
					dPsi = dStressB / dStressT;
					iWebDeter = 1; // Top = Sigma1
				}
				else
				{
					dPsi = dStressT / dStressB;
					iWebDeter = 2; // Bot = Sigma1
				}
				dk_sig = Get_InternalBucklingfactor(dPsi);
				dLamda_p = Get_Lamda_p(dh, dTw, dEpsi, dk_sig);
				Get_ReductionFactorRho(bInternal, dh, dTw, dLamda_p, dPsi, dRho);

				dbeff = dRho * dbc;
				dbe1 = 2.0 / (5.0 - dPsi)*dbeff;
				dbe2 = dbeff - dbe1;
			}
			//
			iClass = Get_WebElasticClass(dAlpha, dct, dEpsi, dPsi);
			bDeterClass = TRUE;
		}
		else if ( dStressT*dStressB <= 0.0 )
		{
			double dx0 = 0.0;
			if ( dStressT < 0.0 && dStressB >= 0.0 )
			{
				dbc = fabs(dStressT) / (fabs(dStressT) + fabs(dStressB))*dh;
				dAlpha = dbc / dh;
				dPsi = dStressB / dStressT;
				iWebDeter = 1; // Top = Sigma1
			}
			else if ( dStressT >= 0.0 && dStressB < 0.0 )
			{
				dbc = fabs(dStressB) / (fabs(dStressT) + fabs(dStressB))*dh;
				dAlpha = dbc / dh;
				dPsi = dStressT / dStressB;
				iWebDeter = 2; // Bot = Sigma1
			}
			dk_sig = Get_InternalBucklingfactor(dPsi);
			dLamda_p = Get_Lamda_p(dh, dTw, dEpsi, dk_sig);
			Get_ReductionFactorRho(bInternal, dh, dTw, dLamda_p, dPsi, dRho);
			dbeff = dRho * dh / (1.0 - dPsi);
			dbe1 = 0.4*dbeff;
			dbe2 = 0.6*dbeff;
			//
			iClass = Get_WebElasticClass(dAlpha, dct, dEpsi, dPsi);
			bDeterClass = TRUE;
		}
	}

	ClasBase.iWebDeter = iWebDeter;
	ClasBase.iClass[4] = iClass;
	ClasBase.dPsi[4] = dPsi;
	ClasBase.dk_sig[4] = dk_sig;
	ClasBase.dRho[4] = dRho;
	ClasBase.dbeff[4] = dbeff;
	ClasBase.dbe1[4] = dbe1;
	ClasBase.dbe2[4] = dbe2;

	return TRUE;
}

BOOL CDgnPlateGirder::Calc_OutstandClassUnit(const T_CLASS_IN_UNIT_D& ClassInD, double dfy, double dfy1,
	int& iClass, CLASS_BASE_UNIT& ClassBase)
{
	ClassBase.Initialize();
	iClass = 0;

	double dStressOuter = ClassInD.dStressOuter;
	double dStressInner = ClassInD.dStressInter;
	double dc = ClassInD.dc;
	double dt = ClassInD.dt;

	// 1. Part subject to compression
	int iClass_N = 0, iClass_NM = 0;
	double dfy_r = Get_fy4Thick(dfy, dfy1, dt);;
	double dEpsi = Get_Epsilon(dfy_r);
	double dBTR = dt < m_dZero ? 0.0 : dc / dt;
	BOOL bIsCompression = FALSE;

	if ( dStressOuter < 0.0 && dStressInner < 0.0 && fabs(dStressOuter - dStressInner) < m_dZero )
	{
		bIsCompression = TRUE;

		if ( dBTR < 9.0*dEpsi + m_dZero ) { iClass_N = 1; }
		else if ( dBTR < 10.0*dEpsi + m_dZero ) { iClass_N = 2; }
		else if ( dBTR < 14.0*dEpsi + m_dZero ) { iClass_N = 3; }
		else { iClass_N = 4; }
	}

	// 2. Part subject to bending and compression
	int iStressType = 0;
	double dbc = 0.0, dbt = 0.0, dCalc_k_sig = 0.0;
	double dAlpha = 0.0, dPsi = 0.0, dk_sig = 0.0, dLamda_p = 0.0, dRho = 0.0, dbeff = 0.0, dx0 = 0.0;
	BOOL bTens = FALSE, bInternal = FALSE;

	double da = dc < m_dZero ? 0.0 : (dStressOuter - dStressInner) / dc;

	if ( dStressOuter < m_dZero && dStressInner < m_dZero )
	{
		dAlpha = 1;
		dbt = 0.0;
		dbc = dc;

		if ( fabs(dStressOuter) > fabs(dStressInner) )
		{
			iStressType = 1;
			dPsi = dStressInner / dStressOuter;
			dk_sig = Get_OutstandBucklingfactor(iStressType, dPsi);
		}
		else if ( fabs(dStressOuter) < fabs(dStressInner) )
		{
			iStressType = 2;
			dPsi = dStressOuter / dStressInner;
			dk_sig = Get_OutstandBucklingfactor(iStressType, dPsi);
		}
		else
		{
			iStressType = 1;
			dPsi = 1.0;
			dk_sig = 0.43;
		}

		dLamda_p = Get_Lamda_p(dc, dt, dEpsi, dk_sig);
		Get_ReductionFactorRho(bInternal, dc, dt, dLamda_p, dPsi, dRho);
		dbeff = dRho * dc;
	}
	else if ( dStressOuter * dStressInner < 0.0 )
	{
		dx0 = fabs(da) < m_dZero ? 0.0 : -dStressInner / da;
		ASSERT(dx0 > m_dZero);

		if ( dStressOuter < 0.0 )
		{
			iStressType = 1;
			dPsi = dStressInner / dStressOuter;
			if ( dPsi < -3.0 ) { dPsi = -3.0; }
		}
		else
		{
			iStressType = 2;
			dPsi = dStressOuter / dStressInner;
			if ( dPsi < -1.0 ) { dPsi = -1.0; }
		}

		dk_sig = Get_OutstandBucklingfactor(iStressType, dPsi);
		dbc = dStressInner < -cDGN_Zero ? dx0 : dc - dx0;
		dbt = dc - dbc;
		dAlpha = fabs(dbt) < m_dZero ? 0.0 : dbc / dbt;

		dLamda_p = Get_Lamda_p(dc, dt, dEpsi, dk_sig);
		Get_ReductionFactorRho(bInternal, dc, dt, dLamda_p, dPsi, dRho);
		dbeff = dRho * dc / (1.0 - dPsi);
	}
	else if ( dStressOuter > 0.0 && dStressInner > 0.0 )
	{
		bTens = TRUE;
		dPsi = 0.0;
		dk_sig = 0.0;
		dbt = dc;
		dbc = 0.0;
		dAlpha = fabs(dbt) < m_dZero ? 0.0 : dbc / dbt;
		dLamda_p = Get_Lamda_p(dc, dt, dEpsi, dk_sig);
		Get_ReductionFactorRho(bInternal, dc, dt, dLamda_p, dPsi, dRho);
		dbeff = dRho * dc / (1.0 - dPsi);
	}
	else
	{
		ASSERT(0);
	}

	if ( !bTens ) { iClass_NM = Get_PartClass(iStressType, dBTR, dEpsi, dAlpha, dk_sig); }
	else { iClass_NM = 1; }

	ClassBase.iClass = iClass_NM;
	ClassBase.dPsi = dPsi;
	ClassBase.dk_sig = dk_sig;
	ClassBase.dRho = dRho;
	ClassBase.dbeff = dbeff;

	iClass = max(iClass_N, iClass_NM);

	return TRUE;
}

BOOL CDgnPlateGirder::Calc_PartLineClassSection4GenSect(double dfy1, double dfy2, int iBotClass, double* aGirderForce, double* dForce, double dPlasticNA, T_SECT_STIFFNESS& GirStiffD, T_SECT_STIFFNESS& CptStiffD, T_SECT_STIFFNESS& OrgStiffD, const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD,
	T_DLINE_PART_D& DLinePartD)
{
	int i = 0;

	double dFx = dForce[0];
	double dMy = dForce[4];
	double dMz = dForce[5];

	int nPartType = PartDgnD.nPartType;
	int iClass = 0;
	// Outstand  
	int iOutstandSize = PartDgnD.aOutstand.GetSize();
	int iOutstandClass = 0;
	if ( iOutstandSize > 0 ) DLinePartD.aOutstand.SetSize(iOutstandSize);
	for ( i = 0; i < iOutstandSize; i++ )
	{
		T_DGNLINE_LBD DLineLbd;
		T_GLINE_DST LineDstD = PartDgnD.aOutstand[i];
		int iLineClass = 0;
		Calc_SubLineClassSection4GenSect(nPartType, dfy1, dfy2, iBotClass, aGirderForce, dForce, dPlasticNA, GirStiffD, CptStiffD, OrgStiffD, GLineData, LineDstD, DLineLbd, iLineClass);

		if ( i == 0 ) iOutstandClass = iLineClass;
		else     iOutstandClass = max(iLineClass, iOutstandClass);
		DLineLbd.iClass = iLineClass;
		DLinePartD.aOutstand.SetAt(i, DLineLbd);
	}

	// Internal
	int iInternalSize = PartDgnD.aInternal.GetSize();
	int iInternalClass = 0;
	if ( iInternalSize > 0 ) DLinePartD.aInternal.SetSize(iInternalSize);
	for ( i = 0; i < iInternalSize; i++ )
	{
		T_DGNLINE_LBD DLineLbd;
		T_GLINE_DST LineDstD = PartDgnD.aInternal[i];
		int iLineClass = 0;
		Calc_SubLineClassSection4GenSect(nPartType, dfy1, dfy2, iBotClass, aGirderForce, dForce, dPlasticNA, GirStiffD, CptStiffD, OrgStiffD, GLineData, LineDstD, DLineLbd, iLineClass);

		if ( i == 0 ) iInternalClass = iLineClass;
		else     iInternalClass = max(iLineClass, iInternalClass);
		DLineLbd.iClass = iLineClass;
		DLinePartD.aInternal.SetAt(i, DLineLbd);
	}

	int iClassOfSect = max(iOutstandClass, iInternalClass);

	DLinePartD.iClassOut = iOutstandClass;
	DLinePartD.iClassIn = iInternalClass;
	DLinePartD.iClass = iClassOfSect;

	return TRUE;
}

BOOL CDgnPlateGirder::Calc_SubLineClassSection4GenSect(int nPartType, double dfy1, double dfy2, int iBotClass, double* aGirderForce, double* dForce, double dPlasticNA, T_SECT_STIFFNESS& GirStiffD, T_SECT_STIFFNESS& CptStiffD, T_SECT_STIFFNESS& OrgStiffD, const T_GENL_DATA& GLineData, const T_GLINE_DST& LineDst, T_DGNLINE_LBD& DLineLbd, int& iClass)
{
	iClass = 0;
	int i = 0;
	double dFx_a = aGirderForce[0];
	double dMy_a = aGirderForce[4];
	double dMz_a = aGirderForce[5];
	//
	double dFx = dForce[0];
	double dMy = dForce[4];
	double dMz = dForce[5];
	BOOL bPositive = dMy >= 0.0;

	const _DGN_FORC_CRC& ScfpForce = GetScfpForce4Lcom();
	int nPosId = GetCheckNode();
	double dFx_sc = ScfpForce.dFxx[nPosId];
	double dMy_sc = ScfpForce.dMuy[nPosId];
	double dMz_sc = ScfpForce.dMuz[nPosId];
	dFx -= dFx_sc;
	dMy -= dMy_sc;
	dMz -= dMz_sc;

	//
	double dArea_a = GirStiffD.Area;
	double dIy_a = GirStiffD.Ryy;
	double dIz_a = GirStiffD.Rzz;
	double dArea = CptStiffD.Area;
	double dIy = CptStiffD.Ryy;
	double dIz = CptStiffD.Rzz;

	BOOL bInternal = LineDst.nType == 0 ? TRUE : FALSE;
	BOOL bIs1_Sig1 = TRUE;
	BOOL bSig1IsFree = TRUE;

	int iLineDgnSize = LineDst.aLineDgn.GetSize();
	//LineLBD.aDgnBaseD.SetSize(iLineDgnSize);

	double db_t = LineDst.db;
	double dt_t = LineDst.dt;
	double dfy = Get_fy4Thick(dfy1, dfy2, dt_t);
	double dEpsilon = Get_Epsilon(dfy);

	double dCzm_gap_ao = OrgStiffD.Czm - GirStiffD.Czm; // org와 거더만 있을 때의 차이 
	double dCzm_gap_co = OrgStiffD.Czm - CptStiffD.Czm; // org와 합성 과의 차이 
	// Plastic N.A와 차이. 
	double dCzm_gap_pl = OrgStiffD.Czm - dPlasticNA;

	double dStrEnd1 = 0.0, dStrEnd2 = 0.0;
	double dLen_c = 0.0, dLen_t = 0.0;

	DLineLbd.aDgnBaseD.SetSize(iLineDgnSize);
	for ( i = 0; i < iLineDgnSize; i++ )
	{
		T_GLINE_DGN_B LineDgnB;
		T_GLINE_BASE LineBaseD = LineDst.aLineDgn[i];
		double dy1_a = LineBaseD.dy1; // Panel의 양끝.
		double dz1_a = LineBaseD.dz1 + dCzm_gap_ao;
		double dy2_a = LineBaseD.dy2;
		double dz2_a = LineBaseD.dz2 + dCzm_gap_ao;
		//
		double dy1 = LineBaseD.dy1; // Panel의 양끝.
		double dz1 = LineBaseD.dz1 + dCzm_gap_co;
		double dy2 = LineBaseD.dy2;
		double dz2 = LineBaseD.dz2 + dCzm_gap_co;
		//
		double dy1_p = LineBaseD.dy1; // Panel의 양끝.
		double dz1_p = LineBaseD.dz1 + dCzm_gap_pl;
		double dy2_p = LineBaseD.dy2;
		double dz2_p = LineBaseD.dz2 + dCzm_gap_pl;
		//
		double dbs = LineBaseD.db;
		double dts = LineBaseD.dt;
		//
		double dStr1_a = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, dy1_a, dz1_a);
		double dStr2_a = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, dy2_a, dz2_a);
		double dStr1_c = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dy1, dz1);
		double dStr2_c = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dy2, dz2);
		double df1Const = CalcStressInterpolate(nPosId, GirStiffD, dy1_a, dz1_a);
		double df2Const = CalcStressInterpolate(nPosId, GirStiffD, dy2_a, dz2_a);
		double dStr1 = dStr1_a + dStr1_c + df1Const;
		double dStr2 = dStr2_a + dStr2_c + df2Const;

		LineDgnB.db = LineBaseD.db;
		LineDgnB.dt = LineBaseD.dt;          // thickness,
		LineDgnB.dy1 = dy1;
		LineDgnB.dz1 = dz1;     // 양끝단의 도심에서 떨어진 거리.
		LineDgnB.dy2 = dy2;
		LineDgnB.dz2 = dz2;
		LineDgnB.dStr1 = dStr1;
		LineDgnB.dStr2 = dStr2;

		if ( i == 0 )
		{
			dt_t = LineBaseD.dt;
			dStrEnd1 = dStr1;
			dStrEnd2 = dStr2;
		}
		else if ( i == iLineDgnSize - 1 )
		{
			dt_t = min(dt_t, LineBaseD.dt);
			dStrEnd2 = dStr2;
		}
		else
		{
			dt_t = min(dt_t, LineBaseD.dt);
		}

		double dLc = 0.0, dLt = 0.0;
		Get_CompLengthOfPlastic(bPositive, dy1_p, dz1_p, dy2_p, dz2_p, dLc, dLt);
		dLen_c += dLc;
		dLen_t += dLt;

		DLineLbd.aDgnBaseD.SetAt(i, LineDgnB);
	}

	BOOL bPlasticCompStr = dLen_c > 0.0;
	BOOL bCompStr = Is_CompStress(dStrEnd1, dStrEnd2);
	BOOL bDbCurve = Is_DoubleCurvature(dStrEnd1, dStrEnd2);
	double dct = dt_t == 0.0 ? 0.0 : db_t / dt_t;

	double df1 = 0.0, df2 = 0.0;
	double dbc = 0.0, dbt = 0.0;
	double dAlpha = 0.0;
	double dPsi = 0.0;
	double dk_sigma = 0.0;
	double dAlpha_p = db_t == 0.0 ? 0.0 : dLen_c / db_t;

	int iClass_p = 0.0;
	BOOL bChkPlastic = TRUE;
	// 무조건 Bending and Compression 으로.. 합성보에서는 대부분이 Plastic, Elastic Axis에서 상하대칭이 아니기 때문. 
	int iInternalStrType = 2;
	int iOutstandStrType = 0;
	// class 1, class 2 검토 
	if ( dLen_c > 0.0 )
	{
		if ( bPositive )
		{
			if ( bInternal )
			{
				bChkPlastic = TRUE;
				iClass_p = Get_InternalPartClass(bChkPlastic, iInternalStrType, dct, dEpsilon, dAlpha_p, dPsi);
			}
			else
			{
				double dk_sigma = 0.0;
				iOutstandStrType = 1;

				dk_sigma = Get_OutstandBucklingfactor(Outstand_Stress_Type1, dPsi);
				iClass_p = Get_PartClass(iOutstandStrType, dct, dEpsilon, dAlpha_p, dk_sigma);
			}
		}
		else
		{
			if ( iBotClass < 3 )
			{
				if ( bInternal )
				{
					bChkPlastic = TRUE;
					iClass_p = Get_InternalPartClass(bChkPlastic, iInternalStrType, dct, dEpsilon, dAlpha_p, dPsi);
				}
				else
				{
					double dk_sigma = 0.0;
					iOutstandStrType = 1;

					dk_sigma = Get_OutstandBucklingfactor(Outstand_Stress_Type1, dPsi);
					iClass_p = Get_PartClass(iOutstandStrType, dct, dEpsilon, dAlpha_p, dk_sigma);
				}
			}
		}
	}
	else
	{
		iClass_p = 1; // 인장. 
	}

	if ( bCompStr )
	{
		//
		Get_f1_f2(dStrEnd1, dStrEnd2, df1, df2, bIs1_Sig1);
		dPsi = Get_Psi(df1, df2);

		//
		if ( !bDbCurve )
		{
			dAlpha = 1.0; // 
		}
		else
		{
			if ( bIs1_Sig1 ) Get_b1b2DbCurvature(db_t, df1, df2, dbc, dbt);
			else          Get_b1b2DbCurvature(db_t, df2, df1, dbc, dbt);
			dAlpha = db_t == 0.0 ? 0.0 : dbc / db_t;
		}

		if ( bInternal )
		{
			if ( !bPositive && nPartType == 2 && iBotClass > 2 )
			{
				// 부모멘트 이면서 Bottom class가 3이상이면 class 3부터 검토한다. 
				bChkPlastic = FALSE;
				iClass = Get_InternalPartClass(bChkPlastic, iInternalStrType, dct, dEpsilon, dAlpha, dPsi);
			}
			else
			{
				if ( iClass_p > 2 )
				{
					bChkPlastic = FALSE;
					iClass = Get_InternalPartClass(bChkPlastic, iInternalStrType, dct, dEpsilon, dAlpha, dPsi);
				}
				else
					iClass = iClass_p;
			}
		}
		else
		{
			double dk_sigma = 0.0;
			if ( !bDbCurve ) iOutstandStrType = 3;
			else
			{
				if ( LineDst.b1isFixed )
				{
					if ( bIs1_Sig1 ) iOutstandStrType = 1;
					else          iOutstandStrType = 2;
				}
				else
				{
					if ( bIs1_Sig1 ) iOutstandStrType = 2;
					else          iOutstandStrType = 1;
				}
			}

			bSig1IsFree = Is_Sig1IsFree4Outstand(bIs1_Sig1);
			int iStressType = Get_StressDistributionType4Outstand(bSig1IsFree, df1, df2);
			if ( iStressType == Outstand_Stress_Type1 || iStressType == Outstand_Stress_Type3 )
				iOutstandStrType = 3;
			else if ( iStressType == Outstand_Stress_Type2 || iStressType == Outstand_Stress_Type4 )
				iOutstandStrType = 2;

			dk_sigma = Get_OutstandBucklingfactor(iStressType, dPsi);
			iClass = Get_PartClass(iOutstandStrType, dct, dEpsilon, dAlpha, dk_sigma);
		}
	}
	else
	{
		iClass = 1;
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Is_Sig1IsFree4Outstand(BOOL bIs1_Sig1)
{
	return bIs1_Sig1 ? FALSE : TRUE; // Outstand일 경우 0번째에 자유단이 아닌 다른 Line과 연결된 vertex가 있다. 
}

double CDgnPlateGirder::Get_InternalBucklingfactor(double dPsi)
{
	BOOL bPsi_Limit = FALSE;
	if ( dPsi < -3.0 )
	{
		bPsi_Limit = TRUE;
		dPsi = -3.0;
	}

	double dk_sig = 0.0;

	if ( 1.0 - m_dZero < dPsi && dPsi < 1.0 + m_dZero ) { dk_sig = 4.0; }
	else if ( 0.0 < dPsi && dPsi < 1.0 ) { dk_sig = 8.2 / (1.05 + dPsi); }
	else if ( fabs(dPsi) < m_dZero ) { dk_sig = 7.81; }
	else if ( -1.0 < dPsi && dPsi < 0.0 ) { dk_sig = 7.81 - 6.29*dPsi + 9.78*dPsi*dPsi; }
	else if ( -1.0 - m_dZero < dPsi && dPsi < -1.0 + m_dZero ) { dk_sig = 23.9; }
	else if ( bPsi_Limit || (-3.0 <= dPsi && dPsi < -1.0) ) { dk_sig = 5.98*(1.0 - dPsi)*(1.0 - dPsi); }

	return dk_sig;
}

double CDgnPlateGirder::Get_OutstandBucklingfactor(int iType, double dPsi)
{
	double dk_sig = 0.0;
	if ( iType == 1 )
	{
		if ( 1.0 - m_dZero < dPsi && dPsi < 1.0 + m_dZero ) { dk_sig = 0.43; }
		else if ( -1.0 - m_dZero < dPsi && dPsi < -1.0 + m_dZero ) { dk_sig = 0.85; }
		else { dk_sig = 0.57 - 0.21*dPsi + 0.07*dPsi*dPsi; }
	}
	else if ( iType == 2 )
	{
		if ( 1.0 - m_dZero < dPsi && dPsi < 1.0 + m_dZero ) { dk_sig = 0.43; }
		else if ( 0.0 < dPsi && dPsi < 1.0 ) { dk_sig = 0.578 / (dPsi + 0.34); }
		else if ( dPsi == 0.0 ) { dk_sig = 1.7; }
		else if ( -1.0 < dPsi && dPsi < 0.0 ) { dk_sig = 1.7 - 5.0*dPsi + 17.1*dPsi*dPsi; } // Modify, Jaeoh. [7/12/2010] 0.5 -> 5.0
		else if ( -1.0 - m_dZero < dPsi && dPsi < -1.0 + m_dZero ) { dk_sig = 23.8; }
		else
			ASSERT(0);
	}
	//else 
	//  ASSERT(0);

	return dk_sig;
}

int CDgnPlateGirder::Get_WebPlaticClass(double dct, double dEpsi, double dAlpha)
{
	int iClass = 0;
	double dLimit1 = 0.0, dLimit2 = 0.0;
	double dMaxValue = pow(1.0 / m_dZero, 5);

	if ( dAlpha > 0.5 )
	{
		dLimit1 = 396.0*dEpsi / (13.0*dAlpha - 1.0);
		dLimit2 = 456.0*dEpsi / (13.0*dAlpha - 1.0);

		if ( dct < dLimit1 + m_dZero ) { iClass = 1; }
		else if ( dct < dLimit2 + m_dZero ) { iClass = 2; }
		else { iClass = 3; }
	}
	else
	{
		dLimit1 = fabs(dAlpha) < m_dZero ? dMaxValue : 36.0*dEpsi / dAlpha;
		dLimit2 = fabs(dAlpha) < m_dZero ? dMaxValue : 41.5*dEpsi / dAlpha;

		if ( dct < dLimit1 + m_dZero ) { iClass = 1; }
		else if ( dct < dLimit2 + m_dZero ) { iClass = 2; }
		else { iClass = 3; }
	}

	return iClass;
}

int CDgnPlateGirder::Get_WebElasticClass(double dAlpha, double dct, double dEpsi, double dPsi)
{
	int iClass = 0;

	double dLim_bendcomp31 = 42.0*dEpsi / (0.67 + 0.33*dPsi);
	double dLim_bendcomp32 = 62.3*dEpsi*(1.0 - dPsi)*sqrt(fabs(dPsi));

	// Part subject to bending and compression
	BOOL bDeter = FALSE;
	if ( !bDeter )
	{
		if ( dPsi > -1.0 )
		{
			if ( dct < dLim_bendcomp31 + m_dZero ) { iClass = 3; }
			else { iClass = 4; }
		}
		else
		{
			if ( dct < dLim_bendcomp32 + m_dZero ) { iClass = 3; }
			else { iClass = 4; }
		}
	}

	return iClass;
}

int CDgnPlateGirder::Get_PartClass(int iStreType, double dct, double dEpsi, double dAlpha, double dk)
{
	int iClass = 1;
	double dLimit1 = 0.0;
	double dLimit2 = 0.0;
	double dLimit3 = 0.0;
	BOOL bCompression = FALSE;

	if ( dAlpha < m_dZero ) { ASSERT(0); }
	if ( dk < 0.0 ) { ASSERT(0); }

	if ( 1.0 - m_dZero < dAlpha && dAlpha < 1.0 + m_dZero ) { bCompression = TRUE; }

	if ( iStreType == 1 )
	{
		dLimit1 = 9.0*dEpsi / dAlpha;
		dLimit2 = 10.0*dEpsi / dAlpha;
		dLimit3 = bCompression ? 14.0*dEpsi : 21.0*dEpsi*sqrt(dk);

		if ( dct < dLimit1 ) { iClass = 1; }
		else if ( dct < dLimit2 ) { iClass = 2; }
		else if ( dct < dLimit3 ) { iClass = 3; }
		else { iClass = 4; }
	}
	else if ( iStreType == 2 )
	{
		dLimit1 = 9.0*dEpsi / (dAlpha*sqrt(dAlpha));
		dLimit2 = 10.0*dEpsi / (dAlpha*sqrt(dAlpha));
		dLimit3 = bCompression ? 14.0*dEpsi : 21.0*dEpsi*sqrt(dk);

		if ( dct < dLimit1 ) { iClass = 1; }
		else if ( dct < dLimit2 ) { iClass = 2; }
		else if ( dct < dLimit3 ) { iClass = 3; }
		else { iClass = 4; }
	}
	else if ( iStreType == 3 ) // 주의: Compression, Table 5.2의 첫번째. 
	{
		dLimit1 = 9.0*dEpsi;
		dLimit2 = 10.0*dEpsi;
		dLimit3 = 14.0*dEpsi;

		if ( dct < dLimit1 ) { iClass = 1; }
		else if ( dct < dLimit2 ) { iClass = 2; }
		else if ( dct < dLimit3 ) { iClass = 3; }
		else { iClass = 4; }
	}
	else
		ASSERT(0);

	return iClass;
}

int CDgnPlateGirder::Get_InternalPartClass(BOOL bChkPlastic, int iStreType, double dct, double dEpsi, double dAlpha, double dPsi)
{
	int iClass = 0;

	double dLim_bend1 = 72.0*dEpsi;
	double dLim_bend2 = 83.0*dEpsi;
	double dLim_bend3 = 124.0*dEpsi;
	double dLim_comp1 = 33.0*dEpsi;
	double dLim_comp2 = 38.0*dEpsi;
	double dLim_comp3 = 42.0*dEpsi;
	double dLim_bendcomp11 = 396.0*dEpsi / (13.0*dAlpha - 1.0);
	double dLim_bendcomp12 = dAlpha == 0.0 ? pow(1.0 / m_dZero, 5) : 36.0*dEpsi / dAlpha;
	double dLim_bendcomp21 = 456.0*dEpsi / (13.0*dAlpha - 1.0);
	double dLim_bendcomp22 = dAlpha == 0.0 ? pow(1.0 / m_dZero, 5) : 41.5*dEpsi / dAlpha;
	double dLim_bendcomp31 = 42.0*dEpsi / (0.67 + 0.33*dPsi);
	double dLim_bendcomp32 = 62.3*dEpsi*(1.0 - dPsi)*sqrt(fabs(dPsi));

	// Part subject to bending
	if ( iStreType == 0 )
	{
		if ( dct < dLim_bend1 || fabs(dct - dLim_bend1) < m_dZero ) { iClass = 1; }
		else if ( dct < dLim_bend2 || fabs(dct - dLim_bend2) < m_dZero ) { iClass = 2; }
		else if ( dct < dLim_bend3 || fabs(dct - dLim_bend3) < m_dZero ) { iClass = 3; }
		else { iClass = 4; }
	}
	// Part subject to compression
	else if ( iStreType == 1 )
	{
		if ( dct < dLim_comp1 || fabs(dct - dLim_comp1) < m_dZero ) { iClass = 1; }
		else if ( dct < dLim_comp2 || fabs(dct - dLim_comp2) < m_dZero ) { iClass = 2; }
		else if ( dct < dLim_comp3 || fabs(dct - dLim_comp3) < m_dZero ) { iClass = 3; }
		else { iClass = 4; }
	}
	// Part subject to bending and compression
	else
	{
		if ( bChkPlastic )
		{
			if ( dAlpha > 0.5 )
			{
				if ( dct < dLim_bendcomp11 || fabs(dct - dLim_bendcomp11) < m_dZero )
				{
					iClass = 1;
				}
				else if ( dct < dLim_bendcomp12 || fabs(dct - dLim_bendcomp12) < m_dZero )
				{
					iClass = 2;
				}
				else { iClass = 3; }
			}
			else
			{
				if ( dct < dLim_bendcomp21 || fabs(dct - dLim_bendcomp21) < m_dZero )
				{
					iClass = 1;
				}
				else if ( dct < dLim_bendcomp22 || fabs(dct - dLim_bendcomp22) < m_dZero )
				{
					iClass = 2;
				}
				else { iClass = 3; }
			}
		}
		else
		{
			if ( dPsi > -1.0 )
			{
				if ( dct < dLim_bendcomp31 || fabs(dct - dLim_bendcomp31) < m_dZero )
				{
					iClass = 3;
				}
				else
				{
					iClass = 4;
				}
			}
			else
			{
				if ( dct < dLim_bendcomp32 || fabs(dct - dLim_bendcomp32) < m_dZero )
				{
					iClass = 3;
				}
				else
				{
					iClass = 4;
				}
			}
		}

	}

	return iClass;
}

BOOL CDgnPlateGirder::Get_ReductionFactorRho(BOOL bInternal, double dBar_b, double dt, double dLamda_p, double dPsi, double& dRho)
{
	dRho = 1.0;

	if ( bInternal )
	{
		if ( dLamda_p < 0.673 + m_dZero )
		{
			dRho = 1.0;
		}
		else
		{
			double dVal = 3.0 + dPsi;

			if ( dVal < 0.0 ) { dVal = 0.0; }

			dRho = min(1.0, (dLamda_p - 0.055*dVal) / pow(dLamda_p, 2));
		}
	}
	else
	{
		if ( dLamda_p < 0.748 + m_dZero )
		{
			dRho = 1.0;
		}
		else
		{
			dRho = min(1.0, (dLamda_p - 0.188) / pow(dLamda_p, 2));
		}
	}

	return TRUE;
}

double CDgnPlateGirder::Get_Lamda_p(double db, double dt, double dEpsilon, double dk_sig)
{
	if ( dt < m_dZero ) { return 0.0; }
	double dLamda_param = 28.4*dEpsilon*sqrt(dk_sig);
	double dbtr = db / dt;
	double dLamda = fabs(dLamda_param) < m_dZero ? 0.0 : dbtr / dLamda_param;

	return dLamda;
}

double CDgnPlateGirder::Get_Epsilon(double dfy)
{
	if ( dfy <= 0.0 )
	{
		ASSERT(0);
		return 0;
	}
	else
	{
		return sqrt(235. / dfy);
	}
}

BOOL CDgnPlateGirder::Get_ConvertVertexId(int vs, int v1, int v2, int& vc1, int& vc2)
{
	if ( vs == v1 ) { vc1 = v1, vc2 = v2; }
	else if ( vs == v2 ) { vc1 = v2, vc2 = v1; }
	else ASSERT(0);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_ConvertSectStiffenerD(const T_DGN_STL_STIFFENER_PART& InD, CArray<CPG_LSTI_DATA, CPG_LSTI_DATA&>& arLstiPosi)
{
	int nStiffNum = arLstiPosi.GetSize();

	if ( nStiffNum > 0 )
	{
		for ( int nSize = 0; nSize < nStiffNum; ++nSize )
		{
			T_STIFF_SHAPE_D InD;
			InD.nType = 0;
			InD.dSize[0] = arLstiPosi[nSize].dWidth;
			InD.dSize[1] = arLstiPosi[nSize].dt;
			m_pDgnSectUtil->Calc_Stiffener_Area(0, InD, arLstiPosi[nSize].dArea);
			m_pDgnSectUtil->Calc_Stiffener_Iyy(0, InD, arLstiPosi[nSize].dIyy, arLstiPosi[nSize].dCzp, arLstiPosi[nSize].dCzm);
			m_pDgnSectUtil->Calc_Stiffener_Izz(0, InD, arLstiPosi[nSize].dIzz, arLstiPosi[nSize].dCyp, arLstiPosi[nSize].dCym);
		}
		return TRUE;
	}

	nStiffNum = InD.arStiffD.GetSize();
	arLstiPosi.SetSize(nStiffNum);

	for ( int i = 0; i < nStiffNum; ++i )
	{
		arLstiPosi[i].dArea = InD.arStiffD[i].dArea;
		arLstiPosi[i].dIyy = InD.arStiffD[i].dIyy;
		arLstiPosi[i].dCzp = InD.arStiffD[i].dCzp;
		arLstiPosi[i].dCzm = InD.arStiffD[i].dCzm;
		arLstiPosi[i].dIzz = InD.arStiffD[i].dIzz;
		arLstiPosi[i].dCyp = InD.arStiffD[i].dCyp;
		arLstiPosi[i].dCym = InD.arStiffD[i].dCym;
		arLstiPosi[i].dz = InD.arStiffD[i].dDist;
		arLstiPosi[i].dWidth = InD.arStiffD[i].dSize[0];
		arLstiPosi[i].dt = InD.arStiffD[i].dSize[1];
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_EffectiveSect4PartLine(double dfy1, double dfy2, double* dForce, double dArea, double dIy, double dIz, const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD,
	T_GENL_DATA& EffGLineD, T_GLINE_PART_DGN& EffPartDgnD)
{
	int i = 0, k = 0;

	EffPartDgnD = PartDgnD;
	// Outstand  
	int iOutstandSize = PartDgnD.aOutstand.GetSize();
	if ( iOutstandSize > 0 ) EffPartDgnD.aOutstand.SetSize(iOutstandSize);
	for ( i = 0; i < iOutstandSize; i++ )
	{
		T_GLINE_DST EffLineDst;
		T_GLINE_DST LineDstD = PartDgnD.aOutstand[i];
		Get_EffectiveSection4StiffenedPlateElements(dfy1, dfy2, dForce, dArea, dIy, dIz, GLineData, LineDstD, EffGLineD, EffLineDst);
		EffPartDgnD.aOutstand.SetAt(i, EffLineDst);
	}

	// Internal
	int iInternalSize = PartDgnD.aInternal.GetSize();
	if ( iInternalSize > 0 ) EffPartDgnD.aInternal.SetSize(iInternalSize);
	for ( i = 0; i < iInternalSize; i++ )
	{
		T_GLINE_DST EffLineDst;
		T_GLINE_DST LineDstD = PartDgnD.aInternal[i];
		Get_EffectiveSection4StiffenedPlateElements(dfy1, dfy2, dForce, dArea, dIy, dIz, GLineData, LineDstD, EffGLineD, EffLineDst);
		EffPartDgnD.aInternal.SetAt(i, EffLineDst);
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_EffectiveWidth4Internal(double dPsi, double db, double dRho, double& dbeff, double& dbe1, double& dbe2)
{
	if ( fabs(dPsi - 1.0) < m_dZero )
	{
		dbeff = dRho * db;
		dbe1 = dbe2 = 0.5*dbeff;
	}
	else if ( -m_dZero < dPsi && dPsi < 1.0 )
	{
		dbeff = dRho * db;
		dbe1 = 2. / (5. - dPsi)*dbeff;
		dbe2 = dbeff - dbe1;
	}
	else if ( dPsi < 0.0 )
	{
		dbeff = dRho * db / (1. - dPsi);
		dbe1 = 0.4*dbeff;
		dbe2 = 0.6*dbeff;
	}
	else
	{
		dbeff = dbe1 = dbe2 = 0.0;
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_EffectiveWidth4Outstand(int nType, double dPsi, double dc, double dRho, double& dbeff)
{
	// nType : 1993-1-5, Table 4.2, 0=(첫버째,두번째) 1=(세번째,네버째)
	// 계산은 같기 때문에 굳이 나눌 필요는 없음. 
	if ( nType == Outstand_Stress_Type1 || nType == Outstand_Stress_Type2 )
	{
		if ( -m_dZero < dPsi && (dPsi < 1.0 || fabs(dPsi - 1.0) < m_dZero) )
		{
			dbeff = dRho * dc;
		}
		else if ( dPsi < 0.0 )
		{
			dbeff = dRho * dc / (1. - dPsi);
		}
	}
	else if ( nType == Outstand_Stress_Type3 || nType == Outstand_Stress_Type4 )
	{
		if ( -m_dZero < dPsi && (dPsi < 1.0 || fabs(dPsi - 1.0) < m_dZero) )
		{
			dbeff = dRho * dc;
		}
		else if ( dPsi < 0.0 )
		{
			dbeff = dRho * dc / (1. - dPsi);
		}
	}
	else
	{
		ASSERT(0);
		dbeff = 0.0;
		return FALSE;
	}
	return TRUE;
}

BOOL CDgnPlateGirder::Get_CalcSectInfo4EffeSect(T_PLATE_GIRDER_CALC_D& param, double dfy1, double dfy2, double da, double* dForce, double dCzm_gap, double dStiff[3], T_DLINE_CALC_D& DlineInfo)
{
	T_GSEC_POLYGON GsecPolyD = param.GLineData.PolyD;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
	aLine.Copy(param.GLineData.aLine);
	int iGLineSize = aLine.GetSize();
	if ( iGLineSize <= 0 ) return FALSE;

	CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&> aPartLine;
	T_GLINE_DGN_D GLineDgn = param.bPosiI ? param.SectElem.GLineDgn[0] : param.SectElem.GLineDgn[1];
	aPartLine.Copy(GLineDgn.aPartLine);
	int iPartSize = aPartLine.GetSize();
	DlineInfo.aPartLine.SetSize(iPartSize);

	double dEs = param.MatlElem.MatdD.Data1.Analysis.Elast;
	double dPoisson = param.MatlElem.MatdD.Data1.Analysis.Poisson;
	for ( int i = 0; i < iPartSize; i++ )
	{
		T_DLINE_PART_D DLinePart;
		T_GLINE_PART_DGN EffPartDgnD;
		T_GLINE_PART_DGN PartDgnD = aPartLine[i];
		Get_CalcSectPartLine4EffeSect(dfy1, dfy2, dEs, dPoisson, da, dForce, dCzm_gap, dStiff, param.GLineData, PartDgnD, DLinePart);
		DlineInfo.aPartLine.SetAt(i, DLinePart);
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_CalcSectPartLine4EffeSect(double dfy1, double dfy2, double dE, double dPoisson, double da, double* aForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD, T_DLINE_PART_D& DLinePart)
{
	int i = 0, j = 0, k = 0;

	double db_in = 0.0, dt_in = 0.0;;
	double dt_out = 0.0, dt_int = 0.0;
	// Outstand  
	int iOutstandSize = PartDgnD.aOutstand.GetSize();
	if ( iOutstandSize > 0 ) DLinePart.aOutstand.SetSize(iOutstandSize);
	DLinePart.nPartType = PartDgnD.nPartType;
	for ( i = 0; i < iOutstandSize; i++ )
	{
		T_DGNLINE_LBD DLineLbd;
		T_GLINE_DST LineDstD = PartDgnD.aOutstand[i];
		db_in += LineDstD.db;
		double dti = LineDstD.dt;
		if ( i == 0 ) dt_out = dti;
		else     dt_out = min(dt_out, dti);
		Get_LocalBucklingDataOfSubPanel(dfy1, dfy2, dE, dPoisson, da, aForce, dCzm_gap, dStiff, GLineData, LineDstD, DLineLbd);
		DLinePart.aOutstand.SetAt(i, DLineLbd);
	}

	// Internal
	int iInternalSize = PartDgnD.aInternal.GetSize();
	if ( iInternalSize > 0 ) DLinePart.aInternal.SetSize(iInternalSize);
	for ( i = 0; i < iInternalSize; i++ )
	{
		T_DGNLINE_LBD DLineLbd;
		T_GLINE_DST LineDstD = PartDgnD.aInternal[i];
		db_in += LineDstD.db;
		double dti = LineDstD.dt;
		if ( i == 0 ) dt_int = dti;
		else     dt_int = min(dt_int, dti);
		Get_LocalBucklingDataOfSubPanel(dfy1, dfy2, dE, dPoisson, da, aForce, dCzm_gap, dStiff, GLineData, LineDstD, DLineLbd);
		double dfyi = Get_fy4Thick(dfy1, dfy2, LineDstD.dt);

		int nOpenStfnSize = 0;
		int nClosedStfnSize = 0;
		int nStfnSizeOfInt = LineDstD.aStiffener.GetSize();
		for ( j = 0; j < nStfnSizeOfInt; j++ )
		{
			// Stiffener를 계산하기 위해.
			int nStfnLineSize = LineDstD.aStiffener[j].aLineIx.GetSize();
			T_GLINE_DGN_STFN StfnGLineD = DLineLbd.aDgnStfnD[j];
			for ( k = 0; k < nStfnLineSize; k++ )
			{
				int nStfnType = LineDstD.aStiffener[j].nType;
				int nLid = LineDstD.aStiffener[j].aLineIx[k];
				T_GSEC_LINE StLine = GLineData.aLine[nLid];
				T_GLINE_BASE StGLine;
				if ( nStfnType == 0 )
				{
					StGLine.iLineType = 1;
					nOpenStfnSize++;
				}
				else if ( nStfnType == 1 )
				{
					if ( k == 0 ) StGLine.iLineType = 0;
					else     StGLine.iLineType = 1;
					nOpenStfnSize++;
				}
				else if ( nStfnType == 2 )
				{
					StGLine.iLineType = 0;
					nClosedStfnSize++;
				}
				else ASSERT(0);

				StGLine.aLineIx.Add(nLid);
				T_GSEC_VERTEX vtx1 = GLineData.PolyD.aVertex[StLine.v1];
				T_GSEC_VERTEX vtx2 = GLineData.PolyD.aVertex[StLine.v2];
				double dy1 = vtx1.dX;
				double dz1 = vtx1.dY;
				double dy2 = vtx2.dX;
				double dz2 = vtx2.dY;
				double dLen = CMathFunc::mathLength(dy1, dz1, dy2, dz2);
				StGLine.db = dLen;
				StGLine.dt = StLine.dThik;
				StGLine.dy1 = dy1;
				StGLine.dz1 = dz1;
				StGLine.dy2 = dy2;
				StGLine.dz2 = dz2;
				StGLine.ve1 = StLine.v1;
				StGLine.ve2 = StLine.v2;                //
				T_GLINE_DST StfnLineDst;
				StfnLineDst.aLineDgn.Add(StGLine);
				T_DGNLINE_LBD StfnDLineLbd;
				Get_LocalBucklingDataOfSubPanel(dfy1, dfy2, dE, dPoisson, da, aForce, dCzm_gap, dStiff, GLineData, StfnLineDst, StfnDLineLbd);
				StfnGLineD.aStfnDgnBaseD.Add(StfnDLineLbd.aDgnBaseD[0]);
			}
			DLineLbd.aDgnStfnD.SetAt(j, StfnGLineD);
		}

		Get_ElasticCriticalPlateBucklingData(dfyi, da, dE, dPoisson, aForce, DLineLbd);
		//    
		BOOL bIsOpenStfn = nOpenStfnSize >= nClosedStfnSize; // 같거나 크면 open stiffener
		Get_ColumnBucklingData(dfyi, da, dE, dPoisson, bIsOpenStfn, aForce, DLineLbd);

		T_DGNLINE_PTB PlateBuckD = DLineLbd.PlateBuckD;
		double dSigma_cr_p = PlateBuckD.dSigma_cr_p;
		double dRho_p = PlateBuckD.dRho;

		T_DGNLINE_CTB ColumnBuckD = DLineLbd.ColumnBuckD;
		double dSigma_cr_c = ColumnBuckD.dSigma_cr_c;
		double dXc = ColumnBuckD.dXc;

		double dxi = Get_xi(dSigma_cr_p, dSigma_cr_c);
		double dRho_c = Get_Rho_c_FinalReductionFactor(dRho_p, dXc, dxi);

		DLineLbd.dxi = dxi;
		DLineLbd.dRho_c = dRho_c;

		DLinePart.aInternal.SetAt(i, DLineLbd);
	}

	if ( dt_out > 0 && dt_int > 0 ) dt_in = min(dt_out, dt_int);
	else if ( dt_out > 0 && dt_int == 0.0 ) dt_in = dt_out;
	else if ( dt_int > 0 && dt_out == 0.0 ) dt_in = dt_int;
	//else ASSERT(0); //!/

	DLinePart.db_in = db_in;
	DLinePart.dt_in = dt_in;

	return TRUE;
}


BOOL CDgnPlateGirder::Get_ElasticCriticalPlateBucklingData(double dfy, double da, double dE, double dPoisson, double* aForce, T_DGNLINE_LBD& DLineLbd)
{
	int iPanelSize = DLineLbd.aDgnBaseD.GetSize();
	int iLineStressType = DLineLbd.iLineStressType;
	int iCompPanelSize = DLineLbd.iCompPanelSize;

	double db = DLineLbd.dbc + DLineLbd.dbt;
	double dt = DLineLbd.dt;
	double dPsi = DLineLbd.dPsi;

	T_DGNLINE_PTB PlateBuckD;
	T_DGNLINE_LBD CompLineLbd;
	Get_ConvertCompPanelDgnLineLBD(DLineLbd, CompLineLbd);

	double dLamda_p = 0.0;
	if ( iCompPanelSize == 1 )
	{
		Get_Sigma_cr_p_4Stiffener(da, dE, dPoisson, aForce, CompLineLbd, DLineLbd);
		double dEpsilon = Get_Epsilon(dfy);
		double dk_sig = CompLineLbd.aDgnBaseD[0].SubPanelD.dk_sigma;
		dLamda_p = Get_Lamda_p(db, dt, dEpsilon, dk_sig);
		//
	}
	else if ( iCompPanelSize > 1 )
	{
		Get_Sigma_cr_p_4Stiffener(da, dE, dPoisson, aForce, CompLineLbd, DLineLbd);
		PlateBuckD = DLineLbd.PlateBuckD;
		double dSigma_cr_p = PlateBuckD.dSigma_cr_p;
		double dAc = PlateBuckD.dAc;
		double dAc_eff_loc = PlateBuckD.dAc_eff_loc;
		double dBeta_A_c = dAc == 0.0 ? 0.0 : dAc_eff_loc / dAc;
		dLamda_p = Get_Lamda_p4PlateTypeBehaviour(dBeta_A_c, dfy, dSigma_cr_p);
	}

	T_GLINE_DGN_B LineDgnB = DLineLbd.aDgnBaseD[0];
	BOOL bInternal = LineDgnB.iLineType == 0 ? TRUE : FALSE;
	double dRho = 0.0;
	Get_ReductionFactorRho(bInternal, db, dt, dLamda_p, dPsi, dRho);

	DLineLbd.PlateBuckD.dLamda_p = dLamda_p;
	DLineLbd.PlateBuckD.dRho = dRho;

	return TRUE;
}

BOOL CDgnPlateGirder::Get_ConvertCompPanelDgnLineLBD(const T_DGNLINE_LBD& DLineLbd, T_DGNLINE_LBD& CompDLineLbd)
{
	int iLineStressType = DLineLbd.iLineStressType;
	int iPanelSize = DLineLbd.aDgnBaseD.GetSize();
	CompDLineLbd = DLineLbd;
	if ( iLineStressType == 0 || iLineStressType == 2 ) { return TRUE; }

	if ( iLineStressType == 1 || iLineStressType == 3 )  // 1=양연압축이고 aDgnBaseD의 마지막 dStr2가 Sig1, 3=양연압축/인장 aDgnBaseD의 마지막 dStr2가 Sig1
	{
		CompDLineLbd.aDgnBaseD.RemoveAll();
		CompDLineLbd.aDgnBaseD.SetSize(iPanelSize);

		for ( int i = 0; i < iPanelSize; i++ )
		{
			int iPanIdx = iPanelSize - 1 - i;
			T_GLINE_DGN_B LineDgnB = DLineLbd.aDgnBaseD[iPanIdx];
			int iSubStressType = LineDgnB.SubPanelD.nType;
			CompDLineLbd.aDgnBaseD.SetAt(i, LineDgnB);
		}

		int iStfnSize = DLineLbd.aDgnStfnD.GetSize();
		CompDLineLbd.aDgnStfnD.RemoveAll();
		CompDLineLbd.aDgnStfnD.SetSize(iStfnSize);

		for ( int i = 0; i < iStfnSize; i++ )
		{
			int iStfnIdx = iStfnSize - 1 - i;
			T_GLINE_DGN_STFN StfnD = DLineLbd.aDgnStfnD[iStfnIdx];
			CompDLineLbd.aDgnStfnD.SetAt(i, StfnD);
		}
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_ColumnBucklingData(double dfy, double da, double dE, double dPoisson, BOOL bIsOpenStfn, double* aForce,
	T_DGNLINE_LBD& DLineLbd)
{
	int iPanelSize = DLineLbd.aDgnBaseD.GetSize();

	int iCompPanelSize = 0;
	for ( int i = 0; i < iPanelSize; i++ )
	{
		T_GLINE_DGN_B LineDgnB = DLineLbd.aDgnBaseD[i];
		if ( LineDgnB.SubPanelD.nType != -1 ) iCompPanelSize++;
	}

	double dt_t = DLineLbd.dt;
	double dSigma_cr_c = 0.0;
	if ( iCompPanelSize == 1 )
	{
		dSigma_cr_c = Get_Sigma_cr_c4ColumnTypeBehaviour(dE, dt_t, dPoisson, da);
		DLineLbd.ColumnBuckD.dSigma_cr_c = dSigma_cr_c;
	}
	else if ( iCompPanelSize > 1 )
	{

		T_DGNLINE_PTB PlateBuckD = DLineLbd.PlateBuckD;

		T_GLINE_DGN_STFN DgnStfnD = DLineLbd.aDgnStfnD[0];
		double dAsl1 = DgnStfnD.dAsl;
		double dIsl1 = DgnStfnD.dIsl;
		double dAsl1_eff = DgnStfnD.dAsl_eff;
		double dIsl1_eff = DgnStfnD.dIsl_eff;
		double dBeta_A_c = dAsl1 == 0.0 ? 0.0 : dAsl1_eff / dAsl1;
		double dSigma_sl = DgnStfnD.dSigma_sl;

		T_GLINE_DGN_B LineDgnB = DLineLbd.aDgnBaseD[0];
		int nCase_IpfF = bIsOpenStfn ? 3 : 2;
		double dalpha = Get_ImperfectionFactor(nCase_IpfF); // EN1993-1-5, 4.5.3(5) alpha for stiffened plate
		double dbc_t = DLineLbd.dbc;
		double db1 = LineDgnB.db;
		dSigma_cr_c = dbc_t / (dbc_t - db1)*dSigma_sl;

		double dLamda_c = Get_Lamda_p4PlateTypeBehaviour(dBeta_A_c, dfy, dSigma_cr_c);
		double di = dAsl1 == 0.0 ? 0.0 : sqrt(dIsl1 / dAsl1);
		double de = DgnStfnD.de;
		double di_div_e = de == 0.0 ? 0.0 : di / de;
		double dalpha_E = di_div_e == 0.0 ? 0.0 : dalpha + 0.09 / (di_div_e);
		double dPhi = Get_PHI_LT(dalpha_E, dLamda_c);
		double dX_org = 0.0;
		double dXc = Get_X_LT(dPhi, dLamda_c, dX_org);

		T_DGNLINE_CTB ColumnBuckD;
		ColumnBuckD.dBeta_A_c = dBeta_A_c;
		ColumnBuckD.dSigma_cr_c = dSigma_cr_c;
		ColumnBuckD.dSigma_sl = dSigma_sl;
		ColumnBuckD.dLamda_c = dLamda_c;
		ColumnBuckD.di = di;
		ColumnBuckD.dalpha_E = dalpha_E;
		ColumnBuckD.dPhi = dPhi;
		ColumnBuckD.dXc = dXc;
		DLineLbd.ColumnBuckD = ColumnBuckD;
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_Sigma_cr_p_4Stiffener(double da, double dE, double dPoisson, double* aForce, const T_DGNLINE_LBD& CompDLineLbd,
	T_DGNLINE_LBD& DLineLbd)
{
	int iCompPanelSize = DLineLbd.iCompPanelSize;
	if ( iCompPanelSize < 1 ) { return TRUE; }

	double dFx = aForce[0];
	double dMy = aForce[4];
	double dMz = aForce[5];
	int iLineStressType = DLineLbd.iLineStressType;

	int i = 0;
	double dbc_t = DLineLbd.dbc;
	double dbt_t = DLineLbd.dbt;
	double dt_t = DLineLbd.dt;
	double db_t = dbc_t + dbt_t;
	double dSigma_cr_p = 0.0;

	int iPanelSize = DLineLbd.aDgnBaseD.GetSize();
	int idx = 0;
	// 1=양연압축이고 aDgnBaseD의 마지막 dStr2가 Sig1, 3=양연압축/인장 aDgnBaseD의 마지막 dStr2가 Sig1

	double db1 = 0.0, db2 = 0.0, db3 = 0.0;
	double dt1 = 0.0, dt2 = 0.0, dt3 = 0.0;
	double db = 0.0, dt = 0.0;
	double dB1 = 0.0, dB2 = 0.0;
	double dAc = 0.0, dAc_eff_loc = 0.0;

	T_GLINE_DGN_STFN DgnStfnD;
	T_DGNLINE_PTB CalcPlateBuckD;
	Get_AreaAndEffArea4Line(CompDLineLbd, dAc, dAc_eff_loc);
	CalcPlateBuckD.dAc = dAc;
	CalcPlateBuckD.dAc_eff_loc = dAc_eff_loc;

	if ( iCompPanelSize == 1 )
	{
		db1 = CompDLineLbd.aDgnBaseD[0].db;
		dt1 = CompDLineLbd.aDgnBaseD[0].dt;

		double dk_sig = CompDLineLbd.aDgnBaseD[0].SubPanelD.dk_sigma;
		double dSigma_E = Get_Sigma_E(dE, db_t, dt_t, dPoisson);
		dSigma_cr_p = dk_sig * dSigma_E;

		CalcPlateBuckD.dSigma_cr_p = dSigma_cr_p;
	}
	else if ( iCompPanelSize == 2 )
	{
		db1 = CompDLineLbd.aDgnBaseD[0].db;
		db2 = CompDLineLbd.aDgnBaseD[1].db;
		dt1 = CompDLineLbd.aDgnBaseD[0].dt;
		dt2 = CompDLineLbd.aDgnBaseD[1].dt;
		dB1 = db1 + db2;
		dt = min(dt1, dt2);
		DgnStfnD = DLineLbd.aDgnStfnD[0];
		double da_c1 = 0.0;
		double dAsl1 = DgnStfnD.dAsl;
		double dIsl1 = DgnStfnD.dIsl;
		double dSigma_sl1 = DgnStfnD.dSigma_sl;
		// Asl를 T_GLINE_DGN_B 에 추가. 
		double dSigma_cr_sl1 = Get_Sigma_cr_sl4SimplifiedModel(dE, dPoisson, dt, dB1, db1, db2, dAsl1, dIsl1, da, da_c1);
		dSigma_cr_p = fabs(dbc_t - db1) < m_dZero ? 0.0 : dbc_t / (dbc_t - db1)*dSigma_cr_sl1;
		CalcPlateBuckD.dSigma_cr_p = dSigma_cr_p;

	}
	else if ( iCompPanelSize == 3 )
	{
		// Stiffener I    
		db1 = CompDLineLbd.aDgnBaseD[0].db;
		db2 = CompDLineLbd.aDgnBaseD[1].db;
		db3 = CompDLineLbd.aDgnBaseD[2].db;
		dt1 = CompDLineLbd.aDgnBaseD[0].dt;
		dt2 = CompDLineLbd.aDgnBaseD[1].dt;
		dt3 = CompDLineLbd.aDgnBaseD[2].dt;
		dB1 = db1 + db2;
		dt = min(dt1, dt2);

		DgnStfnD = DLineLbd.aDgnStfnD[0];
		double da_c1 = 0.0;
		double dAsl1 = DgnStfnD.dAsl;
		double dIsl1 = DgnStfnD.dIsl;
		double dSigma_sl1 = DgnStfnD.dSigma_sl;
		// Asl를 T_GLINE_DGN_B 에 추가. 
		double dSigma_cr_sl1 = Get_Sigma_cr_sl4SimplifiedModel(dE, dPoisson, dt, dB1, db1, db2, dAsl1, dIsl1, da, da_c1);
		double dSigma_cr_p1 = fabs(dbc_t - db1) < m_dZero ? 0.0 : dbc_t / (dbc_t - db1)*dSigma_cr_sl1; // 사다리꼴이면?

		// Stiffener II
		db1 = CompDLineLbd.aDgnBaseD[1].db;
		db2 = CompDLineLbd.aDgnBaseD[2].db;
		dt1 = CompDLineLbd.aDgnBaseD[1].dt;
		dt2 = CompDLineLbd.aDgnBaseD[2].dt;
		dB2 = db1 + db2;
		dt = min(dt1, dt2);

		DgnStfnD = DLineLbd.aDgnStfnD[1];
		double da_c2 = 0.0;
		double dAsl2 = DgnStfnD.dAsl;
		double dIsl2 = DgnStfnD.dIsl;
		double dSigma_sl2 = DgnStfnD.dSigma_sl;
		// Asl를 T_GLINE_DGN_B 에 추가. 
		double dSigma_cr_sl2 = Get_Sigma_cr_sl4SimplifiedModel(dE, dPoisson, dt, dB2, db1, db2, dAsl2, dIsl2, da, da_c2);
		double dSigma_cr_p2 = fabs(dbc_t - dB1) < m_dZero ? 0.0 : dbc_t / (dbc_t - dB1)*dSigma_cr_sl2;

		// Lumped stiffener
		double da_c_lumped = 0.0;
		double dAsl_lumped = dAsl1 + dAsl2;
		double dIsl_lumped = dIsl1 + dIsl2;
		double dFsl1 = dAsl1 * dSigma_sl1;
		double dFsl2 = dAsl2 * dSigma_sl2;
		double db_lumped1 = fabs(dFsl1 + dFsl2) < m_dZero ? 0.0 : (dFsl1*db1 + dFsl2 * db2) / (dFsl1 + dFsl2);
		double db_lumped2 = db_t - db_lumped1;
		double dSigma_cr_sl_lumped = Get_Sigma_cr_sl4SimplifiedModel(dE, dPoisson, dt, db_t, db_lumped1, db_lumped2, dAsl_lumped, dIsl_lumped, da, da_c_lumped);
		double dSigma_cr_p_lumped = fabs(dbc_t - db_lumped1) < m_dZero ? 0.0 : dbc_t / (dbc_t - db_lumped1)*dSigma_cr_sl_lumped;

		//
		dSigma_cr_p = min(min(dSigma_cr_p1, dSigma_cr_p2), dSigma_cr_p_lumped); // min
		CalcPlateBuckD.dSigma_cr_p = dSigma_cr_p;
	}
	else if ( iCompPanelSize > 3 )
	{
		double dAp = DLineLbd.dAp;
		double dIp = DLineLbd.dIp;
		double dAsl_sum = DLineLbd.dAsl_sum;
		double dIsl_sum = DLineLbd.dIsl_sum;
		double dalpha = DLineLbd.dalpha;
		double dgamma = dIp == 0.0 ? 0.0 : dIsl_sum / dIp;
		double dPsi = DLineLbd.dPsi;
		double ddelta = dAp == 0.0 ? 0.0 : dAsl_sum / dAp;
		double dalpha_lim = 0.0;
		double dk_sigma_p = Get_k_sigma_p(dalpha, dgamma, dPsi, ddelta, dalpha_lim);
		double dSigma_E = Get_Sigma_E(dE, db_t, dt_t, dPoisson);

		dSigma_cr_p = dk_sigma_p * dSigma_E;

		CalcPlateBuckD.dgamma = dgamma;
		CalcPlateBuckD.dalpha_lt = dalpha;
		CalcPlateBuckD.ddelta = ddelta;
		CalcPlateBuckD.dk_sigma_p = dk_sigma_p;
		CalcPlateBuckD.dSigma_cr_p = dSigma_cr_p;

	}
	else
		ASSERT(0);

	DLineLbd.PlateBuckD = CalcPlateBuckD;
	return TRUE;
}

BOOL CDgnPlateGirder::Get_Sigma_cr_p_4MultifleStiffener(double da, double dE, double dPoisson, double* aForce, T_DGNLINE_LBD& DLineLbd)
{
	int i = 0;
	double dFx = aForce[0];
	double dMy = aForce[4];
	double dMz = aForce[5];
	int iLineStressType = DLineLbd.iLineStressType;

	double dbc_t = DLineLbd.dbc;
	double dbt_t = DLineLbd.dbt;
	double db_t = dbc_t + dbt_t;
	double dt_t = DLineLbd.dt;
	double dSigma_cr_p = 0.0;

	int iPanelSize = DLineLbd.aDgnBaseD.GetSize();
	int idx = 0;
	T_DGNLINE_LBD CompDLineLbd;

	T_GLINE_DGN_STFN DgnStfnD;

	double dAp = DLineLbd.dAp;
	double dIp = DLineLbd.dIp;
	double dAsl_sum = DLineLbd.dAsl_sum;
	double dIsl_sum = DLineLbd.dIsl_sum;
	double dalpha = DLineLbd.dalpha;
	double dgamma = dIp == 0.0 ? 0.0 : dIsl_sum / dIp;
	double dPsi = DLineLbd.dPsi;
	double ddelta = dAsl_sum / dAp;
	double dalpha_lim = 0.0;
	double dk_sigma_p = Get_k_sigma_p(dalpha, dgamma, dPsi, ddelta, dalpha_lim);
	double dSigma_E = Get_Sigma_E(dE, db_t, dt_t, dPoisson);

	dSigma_cr_p = dk_sigma_p * dSigma_E;

	DLineLbd.PlateBuckD.dSigma_cr_p = dSigma_cr_p;
	return TRUE;
}

double CDgnPlateGirder::Get_Sigma_E(double dEs, double db, double dt, double dPoisson)
{
	double dPi = m_dPI;
	double dSigma = (dPi*dPi)*dEs*(dt*dt) / (12.0*(1.0 - (dPoisson*dPoisson))*(db*db));

	return dSigma;
}

double CDgnPlateGirder::Get_k_sigma_p(double dalpha, double dgamma, double dPsi, double ddelta, double& dalpha_lim)
{
	double dk_sigma_p = 0.0;

	if ( dPsi < 0.5 ) dPsi = 0.5; // Annex A, A.1
	if ( dalpha < 0.5 ) dalpha = 0.5; // Annex A, A.1

	double dk_param0 = (dPsi + 1.0)*(1.0 + ddelta);
	if ( dk_param0 == 0.0 ) return 0.0;

	double dVal = pow(dgamma, 1. / 4.);

	if ( dalpha < dVal || fabs(dalpha - dVal) < m_dZero )
	{
		double dk_param1 = 1.0 + pow(dalpha, 2);
		dk_sigma_p = 2.0*(dk_param1*dk_param1 + dgamma - 1.0) / (dalpha*dalpha*dk_param0);
	}
	else if ( dalpha > dVal )
	{
		dk_sigma_p = 4.0*(1.0 + sqrt(dgamma)) / dk_param0;
	}
	else
		ASSERT(0);

	return dk_sigma_p;
}

double CDgnPlateGirder::Get_Ip_4BendingOfPlate(double db, double dt, double dPoisson)
{
	if ( fabs(dPoisson - 1.0) < m_dZero ) { return 0.0; }
	double dIp = db * pow(dt, 3) / (12.0*(1.0 - dPoisson * dPoisson));

	return dIp;
}

double CDgnPlateGirder::Get_Width4StiffenedPlates(int iNtype, double db, double dPsi, BOOL bSup)
{
	double dwidth = 0.0;
	if ( iNtype == Stiffened_Plate_inf )
	{
		dwidth = (3.0 - dPsi) / (5.0 - dPsi)*db;
	}
	else if ( iNtype == Stiffened_Plate_sup )
	{
		dwidth = 2.0 / (5.0 - dPsi)*db;
	}
	else if ( iNtype == Stiffened_Plate_sup_dbc )
	{
		if ( bSup ) { dwidth = 0.4*db; }
		else { dwidth = 0.6*db; }
	}
	else
	{
		ASSERT(0);
		dwidth = -1.0;
	}

	return dwidth;
}

int CDgnPlateGirder::Get_StreeTypeOfStiffenedPlates(double df1, double df2, int& iStrType2)
{
	int iType = -1;
	BOOL bCompStr = Is_CompStress(df1, df2);
	BOOL bDbCurve = Is_DoubleCurvature(df1, df2);

	if ( bCompStr )
	{
		if ( !bDbCurve )
		{
			if ( fabs(df1) < fabs(df2) )
			{
				iType = Stiffened_Plate_inf;
				iStrType2 = Stiffened_Plate_sup;
			}
			else if ( fabs(df1) > fabs(df2) || fabs(df1 - df2) < m_dZero )
			{
				iType = Stiffened_Plate_sup;
				iStrType2 = Stiffened_Plate_inf;
			}
		}
		else
		{
			iType = Stiffened_Plate_sup_dbc;
		}
	}
	else
	{
		iType = -1; // 인장
	}

	return iType;
}

BOOL CDgnPlateGirder::Get_AreaAndEffArea4Line(const T_DGNLINE_LBD& DgnLineLBD, double& dAc, double& dAc_eff_loc)
{
	int iLineStressType = DgnLineLBD.iLineStressType;
	int iCompPanelSize = DgnLineLBD.iCompPanelSize;
	int iPanelSize = DgnLineLBD.aDgnBaseD.GetSize();
	int iStfnSize = DgnLineLBD.aDgnStfnD.GetSize();

	for ( int i = 0; i < iPanelSize; i++ )
	{
		T_GLINE_DGN_B LineDgnB = DgnLineLBD.aDgnBaseD[i];
		T_SUB_PANEL_BASE SubPanelD = LineDgnB.SubPanelD;
		if ( !LineDgnB.bCompStr ) continue;  // aDgnBaseD정보는 0번째부터 압축이 들어있어야함. 

		if ( iCompPanelSize == 1 )
		{
		}
		else
		{
			if ( i == 0 )
			{
				dAc += SubPanelD.dA_inf; // dA_sup는 edge로..
				dAc_eff_loc += SubPanelD.dA_inf_eff;
			}
			else if ( i == iCompPanelSize - 1 )
			{
				dAc += SubPanelD.dA_sup; // dA_inf는 edge로..
				dAc_eff_loc += SubPanelD.dA_sup_eff;
			}
			else
			{
				dAc += SubPanelD.dA_sup + SubPanelD.dA_inf;
				dAc_eff_loc += SubPanelD.dA_inf_eff + SubPanelD.dA_sup_eff;
			}
		}
	}
	return TRUE;
}

BOOL CDgnPlateGirder::Get_MomentofAreaOfFlange(int iChkPartIx, double dCzm_gap, T_SECT_STIFFNESS& StiffD, const T_GLINE_DGN_D& GLineDgn, double& dQflg)
{
	if ( iChkPartIx > GLineDgn.aPartLine.GetSize() - 1 ) return FALSE; // Data가 없으면 error
	dQflg = 0.0;

	int i = 0, j = 0;
	double dCym = StiffD.Cym;
	double dCzm = StiffD.Czm;
	T_GLINE_PART_DGN GLinePartDgn = GLineDgn.aPartLine[iChkPartIx];

	CArray<T_GSEC_POLYGON, T_GSEC_POLYGON &> aGsecPolygon;
	T_GSEC_POLYGON PolygonD = GLineDgn.ModifiedGsecPolyD;
	aGsecPolygon.SetSize(1);
	aGsecPolygon.SetAt(0, PolygonD);
	double dMaxPt[2], dMinPt[2];
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aTempLineData;
	aTempLineData.Copy(GLineDgn.aModifiedLine);
	CSectUtil::Get_MaxMinPoint4LineType(PolygonD, aTempLineData, dMaxPt, dMinPt);

	double dYcent = dMinPt[0] + dCym; // 도심의 좌표.
	double dZcent = dMinPt[1] + dCzm;

	CArray<int, int&> aPartLine;
	int iOutSize = GLinePartDgn.aOutstand.GetSize();
	for ( i = 0; i < iOutSize; i++ )
	{
		T_GLINE_DST GLineDstD = GLinePartDgn.aOutstand[i];
		int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
		for ( j = 0; j < iLineDgnSize; j++ )
		{
			T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[j];
			aPartLine.Append(GLineBaseD.aLineIx);
		}
	}
	int iIntSize = GLinePartDgn.aInternal.GetSize();
	for ( i = 0; i < iIntSize; i++ )
	{
		T_GLINE_DST GLineDstD = GLinePartDgn.aInternal[i];
		int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
		for ( j = 0; j < iLineDgnSize; j++ )
		{
			T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[j];
			aPartLine.Append(GLineBaseD.aLineIx);
		}
	}

	int iPartLineSize = aPartLine.GetSize();

	for ( i = 0; i < iPartLineSize; i++ )
	{
		int iLineIx = aPartLine[i];
		T_GSEC_LINE GLineD = GLineDgn.aModifiedLine[iLineIx];
		double dt = GLineD.dThik;
		double dy1 = GLineDgn.ModifiedGsecPolyD.aVertex[GLineD.v1].dX;
		double dz1 = GLineDgn.ModifiedGsecPolyD.aVertex[GLineD.v1].dY + dCzm_gap;
		double dy2 = GLineDgn.ModifiedGsecPolyD.aVertex[GLineD.v2].dX;
		double dz2 = GLineDgn.ModifiedGsecPolyD.aVertex[GLineD.v2].dY + dCzm_gap;

		double dyi_max = max(dy1, dy2);
		BOOL bCalcLine = FALSE;
		double dQi = 0.0;
		double dLen = CMathFunc::mathLength(dy1, dz1, dy2, dz2);
		double dAi = dt * dLen;
		double dzc = 0.5*(dz1 + dz2);
		//    if(dyi_max<dYcent || fabs(dyi_max-dYcent)<m_dZero)
		//    {      
		//      double dLen = CMathFunc::mathLength(dy1, dz1, dy2, dz2);
		//      dAi = dt*dLen;
		//      dzc = 0.5*(dz1+dz2);      
		//    }
		//    else if(dy1<dYcent && dYcent<dy2)
		//    {
		//      double dcutLength = CMathFunc::mathLength(dy1, dz1, dYcent, dz2);
		//      dAi = dt*dcutLength;
		//      dzc = 0.5*(dz1+dz2);
		//    }
		//    else if(dy2<dYcent && dYcent<dy1)
		//    {
		//      double dcutLength = CMathFunc::mathLength(dy2, dz2, dYcent, dz2);
		//      dAi = dt*dcutLength;
		//      dzc = 0.5*(dz1+dz2);
		//    }
		//    else 
		//      continue;

		double dZdis = fabs(dzc);
		dQi = dAi * dZdis;
		dQflg += dQi;
	}

	return TRUE;
}

int CDgnPlateGirder::Get_StiffenerSize(const T_GLINE_DST& GLineDST)
{
	int iStfnSize = 0;
	int iInputStfnSize = GLineDST.aStiffener.GetSize();
	for ( int i = 0; i < iInputStfnSize; i++ )
	{
		int iStfnType = GLineDST.aStiffener[i].nType;
		if ( iStfnType == 0 || iStfnType == 1 ) iStfnSize++;
		else if ( iStfnType == 2 ) iStfnSize += 2;
		else ASSERT(0);
	}

	return iStfnSize;
}

double CDgnPlateGirder::Get_Lamda_p4PlateTypeBehaviour(double dBeta_A_c, double dfy, double dSigma_cr_p)
{
	if ( dSigma_cr_p <= 0.0 ) return 0.0;
	double dLamda_p = sqrt(dBeta_A_c*dfy / dSigma_cr_p);

	return dLamda_p;
}

double CDgnPlateGirder::Get_Sigma_cr_c4ColumnTypeBehaviour(double dEs, double dt, double dnu, double da)
{
	if ( da == 0.0 ) return 0.0;
	double dSigma_cr_c = (m_dPI*m_dPI)*dEs*(dt*dt) / (12.*(1. - dnu * dnu)*(da*da));
	return dSigma_cr_c;
}

double CDgnPlateGirder::Get_Sigma_cr_sl4ColumnTypeBehaviour(double dEs, double dI_sl, double dA_sl, double da)
{
	if ( dA_sl == 0.0 || da == 0.0 ) { return 0.0; }
	double dSigma_sl = (m_dPI*m_dPI)*dEs*dI_sl / (dA_sl*(da*da));
	return dSigma_sl;
}

double CDgnPlateGirder::Get_Sigma_cr_sl4SimplifiedModel(double dE, double dPoisson, double dt, double db, double db1, double db2,
	double dAsl, double dIsl, double da, double& da_c)
{
	if ( dAsl == 0.0 || db1 == 0.0 || db2 == 0.0 || fabs(dPoisson - 1.0) < m_dZero ) { return 0.0; }

	double dSigma = 0.0;
	double dPi = CMathFunc::m_pi;

	double da_c_param = dIsl * pow(db1, 2)*pow(db2, 2) / (pow(dt, 3)*db);
	da_c = 4.33*pow(da_c_param, 1.0 / 4.0);

	if ( da > da_c || fabs(da - da_c) < m_dZero )
	{
		dSigma = 1.05*dE / dAsl * (sqrt(dIsl*pow(dt, 3)*db)) / (db1*db2);
	}
	else
	{
		double dSigma_param1 = 4.0*pow(dPi, 2)*(1.0 - pow(dPoisson, 2))*dAsl*pow(db1, 2)*pow(db2, 2);
		dSigma = fabs(da) < m_dZero ? 0.0 : pow(dPi, 2)*dE*dIsl / (dAsl*pow(da, 2)) + dE * pow(dt, 3)*db*pow(da, 2) / dSigma_param1;
	}

	return dSigma;
}

double CDgnPlateGirder::Get_xi(double dSigma_cr_p, double dSigma_cr_c)
{
	double dxi = dSigma_cr_c == 0.0 ? dSigma_cr_p / m_dZero : dSigma_cr_p / dSigma_cr_c;
	if ( dxi < 0.0 ) dxi = 0.0;
	else if ( dxi > 1.0 ) dxi = 1.0;

	return dxi;
}

double CDgnPlateGirder::Get_Rho_c_FinalReductionFactor(double dRho, double dXc, double dxi)
{
	double dRho_c = (dRho - dXc)*dxi*(2.0 - dxi) + dXc;

	return dRho_c;
}


BOOL CDgnPlateGirder::Get_EffectiveSection4StiffenedPlateElements(double dfy1, double dfy2, double* dForce, double dArea, double dIy, double dIz,
	const T_GENL_DATA& GLineData, const T_GLINE_DST& LineDst, T_GENL_DATA& EffGLineD, T_GLINE_DST& EffLineDst)
{
	int i = 0;

	T_GSEC_POLYGON GsecPolyD = GLineData.PolyD;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
	aLine.Copy(GLineData.aLine);
	int iGLineSize = aLine.GetSize();
	if ( iGLineSize <= 0 ) return FALSE;

	EffGLineD = GLineData;
	EffLineDst = LineDst;

	double dFx = dForce[0];
	double dMy = dForce[4];
	double dMz = dForce[5];
	double dStiff[3] = { dArea, dIy, dIz };

	int iLineDgnSize = LineDst.aLineDgn.GetSize();
	double dSig1 = 0., dSig2 = 0., dPsi = 0.;
	double dk_sig = 0.0, dLamda_p = 0.0;
	double dRho = 0.0;
	double dbeff = 0., dbe1 = 0., dbe2 = 0.;
	BOOL bInternal = TRUE;
	int iOutstandType = 0;
	BOOL bIs1_Sig1 = TRUE;
	int iStressType = -1;
	double dxe = 0.0, dye = 0.0, dze = 0.0;
	BOOL b1stIdx = TRUE;
	int iStaVtx = 0;
	for ( i = 0; i < iLineDgnSize; i++ )
	{
		T_GLINE_BASE EffLineBaseD;
		T_GLINE_BASE LineBaseD = LineDst.aLineDgn[i]; // Panel
		int iLineSize = LineBaseD.aLineIx.GetSize();
		bInternal = LineBaseD.iLineType == 0 ? TRUE : FALSE;
		double dy1 = LineBaseD.dy1; // Panel의 양끝.
		double dz1 = LineBaseD.dz1;
		double dy2 = LineBaseD.dy2;
		double dz2 = LineBaseD.dz2;
		double dStr1 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dy1, dz1);
		double dStr2 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dy2, dz2);
		BOOL bCompStr = Is_CompStress(dStr1, dStr2);
		BOOL bDbCurve = Is_DoubleCurvature(dStr1, dStr2);
		int iDivLineIx = 0;


		double dbs = LineBaseD.db;
		double dts = LineBaseD.dt;
		double dfyi = Get_fy4Thick(dfy1, dfy2, dts);
		double dEpsilon = Get_Epsilon(dfy1);
		Get_f1_f2(dStr1, dStr2, dSig1, dSig2, bIs1_Sig1);
		if ( bCompStr )
		{
			dPsi = Get_Psi(dSig1, dSig2);
			dk_sig = Get_InternalBucklingfactor(dPsi);
			dLamda_p = Get_Lamda_p(dbs, dts, dEpsilon, dk_sig);
			Get_ReductionFactorRho(bInternal, dbs, dts, dLamda_p, dPsi, dRho);
			if ( bInternal )
			{
				EffGLineD.Initialize();
				b1stIdx = bIs1_Sig1 ? TRUE : FALSE;
				iStressType = Get_StressDistributionType4Internal(dStr1, dStr2);
				Get_EffectiveWidth4Internal(dPsi, dbs, dRho, dbeff, dbe1, dbe2);
				// Sigma1        
				if ( Get_EffectivePanelInfo(bInternal, iStressType, TRUE, iStaVtx, dForce, dStiff, dbe1, dbe2, GLineData, LineBaseD, EffGLineD, EffLineBaseD) )
				{
				}
				// Sigma2                 
				if ( Get_EffectivePanelInfo(bInternal, iStressType, FALSE, iStaVtx, dForce, dStiff, dbe1, dbe2, GLineData, LineBaseD, EffGLineD, EffLineBaseD) )
				{
				}
				ASSERT(0); // Set하기전에 EffLineBaseD의 내부정보를 만들 함수가 필요. 
				EffLineDst.aLineDgn.SetAt(i, EffLineBaseD);
			}
			else
			{
				BOOL bSig1IsFree = TRUE;
				iStressType = Get_StressDistributionType4Outstand(bSig1IsFree, dStr1, dStr2);
				Get_EffectiveWidth4Outstand(iOutstandType, dPsi, dbs, dRho, dbe1);
				if ( iStressType == Outstand_Stress_Type1 || iStressType == Outstand_Stress_Type2 || iStressType == Outstand_Stress_Type3 )
				{
					if ( Get_EffectivePanelInfo(bInternal, iStressType, b1stIdx, iStaVtx, dForce, dStiff, dbe1, dbe2, GLineData, LineBaseD, EffGLineD, EffLineBaseD) )
					{
					}
					EffLineDst.aLineDgn.SetAt(i, EffLineBaseD);
				}
				else if ( iStressType == Outstand_Stress_Type4 )
				{
					// Sigma1        
					if ( Get_EffectivePanelInfo(bInternal, iStressType, TRUE, iStaVtx, dForce, dStiff, dbe1, dbe2, GLineData, LineBaseD, EffGLineD, EffLineBaseD) )
					{
					}
					// Sigma2                 
					if ( Get_EffectivePanelInfo(bInternal, iStressType, FALSE, iStaVtx, dForce, dStiff, dbe1, dbe2, GLineData, LineBaseD, EffGLineD, EffLineBaseD) )
					{
					}
					ASSERT(0); // Set하기전에 EffLineBaseD의 내부정보를 만들 함수가 필요. 
					EffLineDst.aLineDgn.SetAt(i, EffLineBaseD);
				}
				else ASSERT(0);
			}
		}
		else // 인장일 경우.
		{
			EffLineDst.aLineDgn.SetAt(i, LineBaseD);
		}
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_EffectivePanelInfo(BOOL bInternal, int iStressType, BOOL b1stIdx, int ve, double* dForce, double dStiff[3], double dbe1, double dbe2,
	const T_GENL_DATA& GLineData, const T_GLINE_BASE& LineBaseD, T_GENL_DATA& EffLineD, T_GLINE_BASE& EffLineBaseD)
{
	// b1stIdx : 첫번째 Index부터냐...
	int i = 0;
	int iLineSize = LineBaseD.aLineIx.GetSize();

	double dFx = dForce[0];
	double dMy = dForce[4];
	double dMz = dForce[5];
	//
	double dArea = dStiff[0];
	double dIy = dStiff[1];
	double dIz = dStiff[2];

	int iEffPolyVtxSize = EffLineD.PolyD.aVertex.GetSize();
	int iEffLineSize = EffLineD.aLine.GetSize();

	CArray<int, int&> aEffLineIx;

	T_GSEC_POLYGON GsecPolyD = GLineData.PolyD;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
	aLine.Copy(GLineData.aLine);

	int iIx = 0, iIx2 = 0;
	double dbei = dbe1;

	double dxe = 0., dye = 0.;
	int v1 = 0, v2 = 0, v3 = 0, v4 = 0;
	int vcn = 0; // Line 2개중 연결된 vertex
	int vI = 0, vJ = 0;

	BOOL bCheckLine = FALSE;
	int iConnVtx = 0;
	T_GSEC_VERTEX VtxDiv;
	for ( i = 0; i < iLineSize; i++ )
	{
		if ( b1stIdx )
		{
			iIx = i;
			iIx2 = i + 1;
		}
		else
		{
			iIx = iLineSize - 1;
			iIx2 = iLineSize - 2;
		}
		int iLix = LineBaseD.aLineIx[iIx];
		T_GSEC_LINE LineD = aLine[iLix]; // 순차적으로 연결된 정보이어야함 
		T_GSEC_LINE Line2D;

		v1 = LineD.v1;
		v2 = LineD.v2;
		if ( iLineSize == 1 )
		{
			if ( ve == v1 ) { vI = v1, vJ = v2; }
			else if ( ve == v2 ) { vI = v2, vJ = v1; }
			else ASSERT(0);
		}
		else
		{
			if ( iLineSize - 1 == i )
			{
				if ( vcn == v1 ) { vI = v1, vJ = v2; }
				else if ( vcn == v2 ) { vI = v2, vJ = v1; }
				else ASSERT(0);
			}
			else
			{
				Line2D = aLine[iIx2];
				v3 = Line2D.v1;
				v4 = Line2D.v2;
				vcn = Get_ConnectedVertexId(v1, v2, v3, v4);
				if ( v1 == vcn ) { vI = v2, vJ = v1; }
				else if ( v2 == vcn ) { vI = v1, vJ = v2; }
				else ASSERT(0);
			}
		}

		if ( b1stIdx ) iConnVtx = v2;
		else        iConnVtx = v1;
		T_GSEC_VERTEX vtx1 = GsecPolyD.aVertex[vI];
		T_GSEC_VERTEX vtx2 = GsecPolyD.aVertex[vJ];
		double dx1 = vtx1.dX;
		double dy1 = vtx1.dY;
		double dx2 = vtx2.dX;
		double dy2 = vtx2.dY;
		double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
		double dt = LineD.dThik;
		double df1 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dx1, dy1);
		double df2 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dx2, dy2);
		BOOL bCompStr = Is_CompStress(df1, df2);
		BOOL bDbCurve = Is_DoubleCurvature(df1, df2);

		BOOL b1stIsComp = TRUE;
		if ( i == 0 )
		{
			b1stIsComp = df1 < 0.0 ? TRUE : FALSE;
		}

		if ( bInternal )
		{
			if ( iStressType == Internal_Stress_Type1 || iStressType == Internal_Stress_Type2 )
			{
				if ( dbi < dbei || fabs(dbi - dbei) < m_dZero ) // 나누어지는 Line을 찾기 위해.
				{
					dbei -= dbi;
					aEffLineIx.Add(iLix);
					continue;
				}
				Get_PointCoor4beff(dbei, dx1, dy1, dx2, dy2, dxe, dye);
			}
			else if ( iStressType == Internal_Stress_Type3 )
			{
				if ( !bCompStr ) // 인장이면 
				{
					aEffLineIx.Add(iLix);
					continue;
				}
				else // 압축이면
				{
					if ( bDbCurve )
					{
						double db1 = 0.0, db2 = 0.0;
						if ( b1stIsComp )
						{
							Get_PointCoor4beff(dbi, dx1, dy1, dx2, dy2, dxe, dye);
						}
						else
						{
							double dbti = 0.0;
							Get_b1b2DbCurvature(dbi, df1, df2, db1, db2);
							if ( df1 > 0.0 ) // 1쪽이 인장이면 
							{
								if ( dbe2 > db2 ) // be2가 더 큰경우 
								{
									dbei -= db2;
									aEffLineIx.Add(iLix);
									continue;
								}
								else // be2가 작을경우, 같을경우 포함. 
								{
									dbti = db1 + dbe2;
									Get_PointCoor4beff(dbti, dx1, dy1, dx2, dy2, dxe, dye);
								}
							}
							else // 2쪽이 인장이면 
							{
								if ( dbe2 > db1 ) // be2가 더 큰경우 
								{
									dbei -= db1;
									aEffLineIx.Add(iLix);
									continue;
								}
								else // be2가 작을경우, 같을경우 포함.  
								{
									dbti = db2 + dbe2;
									Get_PointCoor4beff(dbti, dx2, dy2, dx1, dy1, dxe, dye);
								}
							}
						}
					}
					else // single curvature
					{
						if ( dbi < dbei || fabs(dbi - dbei) < m_dZero ) // 나누어지는 Line을 찾기 위해.
						{
							dbei -= dbi;
							aEffLineIx.Add(iLix);
							continue;
						}
						Get_PointCoor4beff(dbei, dx1, dy1, dx2, dy2, dxe, dye);
					}
				}
			}

			VtxDiv.dX = dxe;
			VtxDiv.dY = dye;
			EffLineD.PolyD.aVertex.Add(VtxDiv);
			T_GSEC_LINE ELine1 = LineD;
			ELine1.v1 = vI;
			ELine1.v2 = iEffPolyVtxSize;
			EffLineD.aLine.Add(ELine1);
			aEffLineIx.Add(iEffLineSize);
			break;

		} // End. Internal 
		else
		{
			if ( iStressType == Outstand_Stress_Type1 || iStressType == Outstand_Stress_Type3 )
			{
				if ( dbi < dbei || fabs(dbi - dbei) < m_dZero ) // 나누어지는 Line을 찾기 위해.
				{
					dbei -= dbi;
					aEffLineIx.Add(iLix);
					continue;
				}
				Get_PointCoor4beff(dbei, dx1, dy1, dx2, dy2, dxe, dye);
			}
			else if ( iStressType == Outstand_Stress_Type2 )
			{
				if ( !bCompStr ) // 인장이면 
				{
					aEffLineIx.Add(iLix);
					continue;
				}
				else // 압축이면
				{
					if ( bDbCurve )
					{
						double db1 = 0.0, db2 = 0.0;
						double dbti = 0.0;
						Get_b1b2DbCurvature(dbi, df1, df2, db1, db2);
						if ( df1 > 0.0 ) // 1쪽이 인장이면 
						{
							// Outstand에서는 be1이 b_eff를 
							if ( dbe1 > db2 ) // be2가 더 큰경우 
							{
								dbei -= db2;
								aEffLineIx.Add(iLix);
								continue;
							}
							else // be2가 작을경우, 같을경우 포함. 
							{
								dbti = db1 + dbe1;
								Get_PointCoor4beff(dbti, dx1, dy1, dx2, dy2, dxe, dye);
							}
						}
						else // 2쪽이 인장이면 
						{
							if ( dbe1 > db1 ) // be2가 더 큰경우 
							{
								dbei -= db1;
								aEffLineIx.Add(iLix);
								continue;
							}
							else // be2가 작을경우, 같을경우 포함.  
							{
								dbti = db2 + dbe1;
								Get_PointCoor4beff(dbti, dx2, dy2, dx1, dy1, dxe, dye);
							}
						}
					}
					else // single curvature
					{
						if ( dbi < dbei || fabs(dbi - dbei) < m_dZero ) // 나누어지는 Line을 찾기 위해.
						{
							dbei -= dbi;
							aEffLineIx.Add(iLix);
							continue;
						}
						Get_PointCoor4beff(dbei, dx1, dy1, dx2, dy2, dxe, dye);
					}
				}
			}
			else if ( iStressType == Outstand_Stress_Type4 )
			{
				if ( !bCompStr )
				{
					aEffLineIx.Add(iLix);
					continue;
				}
				else
				{
					if ( bDbCurve )
					{
						double db1 = 0.0, db2 = 0.0;
						double dbti = 0.0;
						Get_b1b2DbCurvature(dbi, df1, df2, db1, db2);
						if ( df1 > 0.0 ) // 1쪽이 인장이면 
						{
							Get_PointCoor4beff(db1, dx1, dy1, dx2, dy2, dxe, dye);
						}
						else // 2쪽이 인장이면 
						{
							Get_PointCoor4beff(db2, dx1, dy1, dx2, dy2, dxe, dye);
						}
					}
					else
					{
						if ( dbi < dbei || fabs(dbi - dbei) < m_dZero ) // 나누어지는 Line을 찾기 위해.
						{
							dbei -= dbi;
							aEffLineIx.Add(iLix);
							continue;
						}
						Get_PointCoor4beff(dbei, dx1, dy1, dx2, dy2, dxe, dye);
					}
				}

			}
			//
			VtxDiv.dX = dxe;
			VtxDiv.dY = dye;
			EffLineD.PolyD.aVertex.Add(VtxDiv);
			T_GSEC_LINE ELine1 = LineD;
			ELine1.v1 = vI;
			ELine1.v2 = iEffPolyVtxSize;
			EffLineD.aLine.Add(ELine1);
			aEffLineIx.Add(iEffLineSize);
			break;

		}
	}

	EffLineBaseD.aLineIx.Copy(aEffLineIx);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_LocalBucklingDataOfSubPanel(double dfy1, double dfy2, double dE, double dPoisson, double da, double* dForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData,
	const T_GLINE_DST& LineDst, T_DGNLINE_LBD& LineLBD)
{
	int i = 0;
	double dFx = dForce[0];
	double dMy = dForce[4];
	double dMz = dForce[5];
	//
	double dArea = dStiff[0];
	double dIy = dStiff[1];
	double dIz = dStiff[2];

	double dPsi = 0.0;
	double dk_sig = 0.0;
	double dLamda_p = 0.0;
	BOOL bInternal = TRUE;
	BOOL bIs1_Sig1 = TRUE;
	BOOL bSig1IsFree = TRUE;

	int iLineDgnSize = LineDst.aLineDgn.GetSize();
	LineLBD.aDgnBaseD.SetSize(iLineDgnSize);

	double db_t = 0.0, dt_t = 0.0;
	double dbc_sum = 0.0, dbt_sum = 0.0;
	double dAp_sum = 0.0;
	CArray<T_GLINE_DGN_B, T_GLINE_DGN_B&> aPanelDgnBaseD; // Stiffener정보를 위해.  
	double dStressI = 0.0, dStressJ = 0.0;
	BOOL bLineCompStr = FALSE;
	BOOL bLineDbCurve = FALSE;
	int iLineStressType = -1;
	int iCompPanelSize = 0;
	for ( i = 0; i < iLineDgnSize; i++ )
	{
		T_GLINE_DGN_B LineDgnBase;
		T_GLINE_BASE LineBaseD = LineDst.aLineDgn[i]; // 의 !!!dy1,dz1, dy2,dz2는 순차적인 값이 맞음
		bInternal = LineBaseD.iLineType == 0 ? TRUE : FALSE;
		double dbi = LineBaseD.db;
		double dti = LineBaseD.dt;
		double dApi = dbi * dti;
		dAp_sum += dApi;
		double dfyi = Get_fy4Thick(dfy1, dfy2, dti);
		double dEpsilon = Get_Epsilon(dfyi);
		db_t += dbi;
		if ( i == 0 ) dt_t = dti;
		else     dt_t = min(dt_t, dti);
		double dy1 = LineBaseD.dy1; // Panel의 양끝.
		double dz1 = LineBaseD.dz1 + dCzm_gap;
		double dy2 = LineBaseD.dy2;
		double dz2 = LineBaseD.dz2 + dCzm_gap;
		double dbs = LineBaseD.db;
		double dts = LineBaseD.dt;
		double dStr1 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dy1, dz1);
		double dStr2 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dy2, dz2);
		if ( IsValueZero(dStr1) ) dStr1 = 0.0;
		if ( IsValueZero(dStr2) ) dStr2 = 0.0;
		BOOL bCompStr = Is_CompStress(dStr1, dStr2);
		BOOL bDbCurve = Is_DoubleCurvature(dStr1, dStr2);
		int iStressType = -1;
		double dy0 = 0.0, dz0 = 0.0;
		double dSig1 = 0.0, dSig2 = 0.0;

		Get_f1_f2(dStr1, dStr2, dSig1, dSig2, bIs1_Sig1);

		// vertex정보가 순차적으로 있다고 가정(계산 들어오기 전에 Line정보를 array에 순차적으로 해주어야함) 
		if ( i == 0 )
		{
			dStressI = dStr1;
			dStressJ = dStr2;
		}
		else
		{
			dStressJ = dStr2;
		}

		if ( bCompStr )
		{
			iCompPanelSize++;
			bLineCompStr = TRUE; // 하나라도 압축이 있으면 압축.
		}
		if ( bDbCurve ) bLineDbCurve = TRUE; // 하나라도 복곡률이 있으면 복곡률

		dPsi = Get_Psi(dSig1, dSig2);
		if ( bCompStr )
		{
			if ( bInternal )
			{
				iStressType = Get_StressDistributionType4Internal(dStr1, dStr2);
				dk_sig = Get_InternalBucklingfactor(dPsi);
			}
			else
			{
				int iOutType = -1;
				bSig1IsFree = Is_Sig1IsFree4Outstand(bIs1_Sig1);
				iStressType = Get_StressDistributionType4Outstand(bSig1IsFree, dStr1, dStr2);
				if ( iStressType == Outstand_Stress_Type1 || iStressType == Outstand_Stress_Type2 )
					iOutType = 1;
				else if ( iStressType == Outstand_Stress_Type3 || iStressType == Outstand_Stress_Type4 )
					iOutType = 2;
				dk_sig = Get_OutstandBucklingfactor(iOutType, dPsi);
			}
		}

		Set_LineDgnBase(LineBaseD.iLineType, LineBaseD.db, LineBaseD.dt, dy1, dz1, dy2, dz2, dSig1, dSig2, bCompStr, bDbCurve, bIs1_Sig1, dy0, dz0, LineDgnBase);
		LineDgnBase.dStr1 = dStr1;
		LineDgnBase.dStr2 = dStr2;
		int iStressType4EffSect = Get_StressType4EffSect(bCompStr, bDbCurve, bIs1_Sig1);
		LineDgnBase.iStressType = iStressType4EffSect;

		double dbb = 0.0, db_eff = 0.0;
		double dRho = 0.0;
		dLamda_p = Get_Lamda_p(dbs, dts, dEpsilon, dk_sig);
		Get_ReductionFactorRho(bInternal, dbs, dts, dLamda_p, dPsi, dRho);

		T_SUB_PANEL_BASE SubPanelD;
		SubPanelD.nType = iStressType;
		SubPanelD.dPsi = dPsi;
		SubPanelD.dk_sigma = dk_sig;
		SubPanelD.dLamda_p = dLamda_p;
		SubPanelD.dRho = dRho;
		int iNtype = 0, iNtype2 = 0;
		BOOL bGrossWidth = TRUE;
		if ( bInternal )
		{
			// 
			if ( bCompStr )
			{
				if ( !bDbCurve )
				{
					dbc_sum += dbi;
					iNtype = Get_StreeTypeOfStiffenedPlates(dStr1, dStr2, iNtype2);
					// Gross width
					bGrossWidth = TRUE;
					dbb = Get_Width4StiffenedPlates(iNtype, dbs, dPsi);
					Set_CalcSubPanel(bGrossWidth, iNtype, dbb, dti, SubPanelD);
					//
					dbb = Get_Width4StiffenedPlates(iNtype2, dbs, dPsi);
					Set_CalcSubPanel(bGrossWidth, iNtype2, dbb, dti, SubPanelD);
					// Effective width
					bGrossWidth = FALSE;
					db_eff = dRho * dbs;
					dbb = Get_Width4StiffenedPlates(iNtype, db_eff, dPsi);
					Set_CalcSubPanel(bGrossWidth, iNtype, dbb, dti, SubPanelD);
					//
					dbb = Get_Width4StiffenedPlates(iNtype2, db_eff, dPsi);
					Set_CalcSubPanel(bGrossWidth, iNtype2, dbb, dti, SubPanelD);
				}
				else
				{
					double dbc = 0.0;
					Get_CompWidthOfDoubleCurvatrue(dForce, dCzm_gap, dArea, dIy, dIz, GLineData, LineBaseD, dbc);
					// Effective width
					bGrossWidth = FALSE;
					db_eff = dRho * dbc;
					SubPanelD.db_sup = dbc;
					iNtype = Get_StreeTypeOfStiffenedPlates(dStr1, dStr2, iNtype2);
					dbb = Get_Width4StiffenedPlates(iNtype, db_eff, dPsi, TRUE);
					SubPanelD.db_sup_eff = dbb;
					SubPanelD.dA_sup_eff = dbb * dti;
					//
					dbb = Get_Width4StiffenedPlates(iNtype2, db_eff, dPsi, FALSE);
					SubPanelD.db_inf_eff = dbb;
					SubPanelD.dA_inf_eff = dbb * dti;
					//          
					dbc_sum += dbc;
					dbt_sum += dbi - dbc;
				}
			}
			else
			{
				dbt_sum += dbi;
				// 인장.
			}
		}
		else // outstand
		{
			if ( bCompStr )
			{
				Get_EffectiveWidth4Outstand(iStressType, dPsi, dbi, dRho, db_eff);
				if ( !bDbCurve )
				{
					dbc_sum += dbi;
					SubPanelD.db_sup = dbi; // db_sup에 넣자. 
					SubPanelD.dA_sup = dbi * dti;
					SubPanelD.db_sup_eff = db_eff;
				}
				else
				{
					double dbc = 0.0;
					Get_CompWidthOfDoubleCurvatrue(dForce, dCzm_gap, dArea, dIy, dIz, GLineData, LineBaseD, dbc);
					dbc_sum += dbc;
					dbt_sum += dbi - dbc;
					SubPanelD.db_sup = dbc; // db_sup에 넣자. 
					SubPanelD.dA_sup = dbc * dti;
					SubPanelD.db_sup_eff = db_eff;
				}
			}
			else
			{
				dbt_sum += dbi;
			}
		}
		LineDgnBase.SubPanelD = SubPanelD;
		aPanelDgnBaseD.Add(LineDgnBase); // 압축이든 인장이든 Data는 넣어주자.    

		LineLBD.aDgnBaseD.SetAt(i, LineDgnBase);
	}

	if ( bLineCompStr )
	{
		BOOL bIs1_f1 = FALSE;
		double df1_temp = 0.0, df2_temp = 0.0;
		Get_f1_f2(dStressI, dStressJ, df1_temp, df2_temp, bIs1_f1);
		if ( !bLineDbCurve )
		{
			if ( bIs1_f1 ) iLineStressType = 0;
			else        iLineStressType = 1;
		}
		else
		{
			if ( bIs1_f1 ) iLineStressType = 2;
			else        iLineStressType = 3;
		}
	}
	else
	{
		iLineStressType = -1;
	}

	// Stiffener
	int iDgnLineSerialIx = 0;
	CArray<int, int&> aLineDgnIndex4Stfn;
	for ( i = 0; i < LineDst.aStiffener.GetSize(); i++ )
	{
		int iStfnType = LineDst.aStiffener[i].nType;
		if ( iStfnType == 0 || iStfnType == 1 )
		{
			aLineDgnIndex4Stfn.Add(i);
		}
		else if ( iStfnType == 2 )
		{
			aLineDgnIndex4Stfn.Add(i);
			aLineDgnIndex4Stfn.Add(i);
		}
		else ASSERT(0);
	}
	double dAsl_sum = 0.0;
	double dIsl_sum = 0.0;
	double dIsl_sh_sum = 0.0;
	double dIp = 0.0;
	int iSubPanelSize = aPanelDgnBaseD.GetSize();  // 압축만..
	if ( bInternal && iSubPanelSize > 1 )
	{
		LineLBD.aDgnStfnD.SetSize(iSubPanelSize - 1);
		dIp = Get_Ip_4BendingOfPlate(db_t, dt_t, dPoisson);
		for ( i = 0; i < iSubPanelSize - 1; i++ )
		{
			int iStfnIx = aLineDgnIndex4Stfn[i];
			T_GLINE_STIFFENER StfnD = LineDst.aStiffener[iStfnIx];
			T_GLINE_DGN_B PreLineDgn = aPanelDgnBaseD[i];
			T_GLINE_DGN_B NextLineDgn = aPanelDgnBaseD[i + 1];
			int iPreStrType = PreLineDgn.SubPanelD.nType;
			int iNextStrType = NextLineDgn.SubPanelD.nType;
			double db_inf = 0.0, db_sup = 0.0;
			double db_inf_eff = 0.0, db_sup_eff = 0.0;
			double dt_inf = 0.0, dt_sup = 0.0;
			if ( iPreStrType == Internal_Stress_Type1 || iPreStrType == Internal_Stress_Type2 )
			{
				db_inf = PreLineDgn.SubPanelD.db_inf;
				db_sup = NextLineDgn.SubPanelD.db_sup;
				db_inf_eff = PreLineDgn.SubPanelD.db_inf_eff;
				db_sup_eff = NextLineDgn.SubPanelD.db_sup_eff;
				dt_inf = PreLineDgn.dt;
				dt_sup = NextLineDgn.dt;
			}
			else if ( iPreStrType == Internal_Stress_Type3 )
			{
				db_inf = NextLineDgn.SubPanelD.db_inf;
				db_sup = PreLineDgn.SubPanelD.db_sup;
				db_inf_eff = NextLineDgn.SubPanelD.db_inf_eff;
				db_sup_eff = PreLineDgn.SubPanelD.db_sup_eff;
				dt_inf = NextLineDgn.dt;
				dt_sup = PreLineDgn.dt;
			}
			else
			{
				// 인장. 
			}

			double dfyi = Get_fy4Thick(dfy1, dfy2, dt_t);
			double dEpsilon = Get_Epsilon(dfyi);
			int nPosLine = StfnD.nPosLine; // Stiffener 위치 0:좌측 1:우측 2:양측
			double dt_m = dt_inf;
			double dA_sf = StfnD.dA;
			double dI_sf = StfnD.dIy;
			if ( StfnD.nType == 2 ) // U-rib 경우. 
			{
				dA_sf *= 0.5;
				dI_sf *= 0.5;
			}
			double dZbar_sf = StfnD.dZbar;
			double dA_inf = db_inf * dt_inf;
			double dA_sup = db_sup * dt_sup;
			double dAsl = dA_inf + dA_sup + dA_sf;
			double dSf_bar = 0.5*dt_m + dZbar_sf;
			double dXsl = dAsl == 0.0 ? 0.0 : dSf_bar * dA_sf / dAsl; // 모재 두께의 1/2이 기준. 
			double dI_inf = db_inf * pow(dt_inf, 3) / 12.0;
			double dI_sup = db_sup * pow(dt_sup, 3) / 12.0;
			double dIsl = dI_inf + dI_sup + (dA_inf + dA_sup)*pow(dXsl, 2) + dI_sf + dA_sf * pow((dSf_bar - dXsl), 2);
			double de1 = dXsl;
			double de2 = dSf_bar - dXsl;
			double dIsl_sh = 0.0;

			if ( StfnD.nType == 0 || StfnD.nType == 1 )
			{
				Get_IslOfLongStfn4GS(dEpsilon, StfnD, dt_m, dIsl_sh);
			}
			else if ( StfnD.nType == 2 )
			{
				if ( i % 2 == 0 )
					Get_IslOfLongStfn4GS(dEpsilon, StfnD, dt_m, dIsl_sh);
			}

			dAsl_sum += dAsl;
			dIsl_sum += dIsl;
			dIsl_sh_sum += dIsl_sh;

			T_GLINE_DGN_STFN DgnStfn;
			DgnStfn.dA = dA_sf;
			DgnStfn.dIy = dI_sf;
			DgnStfn.dZbar = dZbar_sf;
			DgnStfn.db_inf = db_inf;
			DgnStfn.db_inf = db_inf_eff;
			DgnStfn.db_sup = db_sup;
			DgnStfn.db_sup_eff = db_sup_eff;
			DgnStfn.dAsl = dAsl;
			DgnStfn.dIsl = dIsl;
			DgnStfn.dIsl_sh = dIsl_sh;
			DgnStfn.dXsl = dXsl;
			DgnStfn.de1 = dXsl;
			DgnStfn.de2 = dSf_bar - dXsl;
			DgnStfn.de = max(de1, de2);
			DgnStfn.dSigma_sl = LineLBD.aDgnBaseD[i].dStr2;

			LineLBD.aDgnStfnD.SetAt(i, DgnStfn);
		}
	}

	//  
	LineLBD.dalpha = LineDst.db == 0.0 ? 0.0 : da / LineDst.db;
	LineLBD.dbc = dbc_sum;
	LineLBD.dbt = dbt_sum;
	LineLBD.dt = dt_t;
	LineLBD.iLineStressType = iLineStressType;
	LineLBD.iCompPanelSize = iCompPanelSize;
	LineLBD.dAsl_sum = dAsl_sum;
	LineLBD.dIsl_sum = dIsl_sum;
	LineLBD.dIsl_sh_sum = dIsl_sh_sum;
	LineLBD.dAp = dAp_sum;
	LineLBD.dIp = dIp;

	return TRUE;
}

void CDgnPlateGirder::Set_CalcSubPanel(BOOL bGrossWidth, int iNtype, double dbb, double dti, T_SUB_PANEL_BASE& rData)
{
	if ( bGrossWidth )
	{
		if ( iNtype == Stiffened_Plate_inf )
		{
			rData.db_inf = dbb;
			rData.dA_inf = dbb * dti;
		}
		else if ( iNtype == Stiffened_Plate_sup )
		{
			rData.db_sup = dbb;
			rData.dA_sup = dbb * dti;
		}
		else ASSERT(0);
	}
	else
	{
		if ( iNtype == Stiffened_Plate_inf )
		{
			rData.db_inf_eff = dbb;
			rData.dA_inf_eff = dbb * dti;
		}
		else if ( iNtype == Stiffened_Plate_sup )
		{
			rData.db_sup_eff = dbb;
			rData.dA_sup_eff = dbb * dti;
		}
		else ASSERT(0);
	}
}

int CDgnPlateGirder::Get_StressType4EffSect(BOOL bCompStr, BOOL bDbCurve, BOOL bIs1_Sig1)
{
	int iStrType = -1;
	if ( bCompStr )
	{
		if ( !bDbCurve )
		{
			if ( bIs1_Sig1 ) iStrType = 0; // 0=양연압축 이고 dStr1이 f1
			else          iStrType = 1; // 1=양연압축 이고 dStr2이 f1
		}
		else
		{
			if ( bIs1_Sig1 ) iStrType = 2; // 2=양연압축/인장 이고 dStr1이 f1
			else          iStrType = 3; // 3=양연압축/인장 이고 dStr2이 f1
		}
	}
	return iStrType;
}

BOOL CDgnPlateGirder::Get_CompWidthOfDoubleCurvatrue(T_GLINE_DGN_B& LineDgnB, double& dbcc)
{
	double dStr1 = LineDgnB.dStr1;
	double dStr2 = LineDgnB.dStr2;
	double db = LineDgnB.db;
	double dy1 = LineDgnB.dy1;
	double dz1 = LineDgnB.dz1;
	double dy2 = LineDgnB.dy2;
	double dz2 = LineDgnB.dz2;
	double dye = 0.0, dze = 0.0;
	if ( !Is_DoubleCurvature(dStr1, dStr2) ) { return FALSE; }
	double df1 = LineDgnB.df1;
	double df2 = LineDgnB.df2;
	double dbc = 0.0, dbt = 0.0;
	BOOL bIs1_Sig1 = LineDgnB.bIs1_sig1;

	if ( bIs1_Sig1 )
	{
		Get_b1b2DbCurvature(db, df1, df2, dbc, dbt);
		Get_PointCoor4beff(dbc, dy1, dz1, dy2, dz2, dye, dze);
	}
	else
	{
		Get_b1b2DbCurvature(db, df2, df1, dbc, dbt);
		Get_PointCoor4beff(dbc, dy2, dz2, dy1, dz1, dye, dze);
	}
	LineDgnB.dy0 = dye;
	LineDgnB.dz0 = dze;

	dbcc = dbc;

	return TRUE;
}

BOOL CDgnPlateGirder::Get_CompWidthOfDoubleCurvatrue(double* dForce, double dCzm_gap, double dArea, double dIy, double dIz, const T_GENL_DATA& GLineData, const T_GLINE_BASE& LineBaseD, double& dbc)
{
	// b1stIdx : 첫번째 Index부터냐...
	int i = 0;
	int iLineSize = LineBaseD.aLineIx.GetSize();

	double dFx = dForce[0];
	double dMy = dForce[4];
	double dMz = dForce[5];
	//

	T_GSEC_POLYGON GsecPolyD = GLineData.PolyD;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
	aLine.Copy(GLineData.aLine);

	int iIx = 0, iIx2 = 0;
	double dbei = 0.0;

	double dxe = 0., dye = 0.;
	int v1 = 0, v2 = 0, v3 = 0, v4 = 0;
	int vcn = 0; // Line 2개중 연결된 vertex
	int vI = 0, vJ = 0;

	BOOL bCheckLine = FALSE;
	int iConnVtx = 0;
	T_GSEC_VERTEX VtxDiv;
	double dbt = 0.0;
	double db1 = 0.0, db2 = 0.0;
	BOOL b1stIsComp = TRUE;
	for ( i = 0; i < iLineSize; i++ )
	{
		int iLix = LineBaseD.aLineIx[i];
		T_GSEC_LINE LineD = aLine[iLix]; // 순차적으로 연결된 정보이어야함     
		v1 = LineD.v1;
		v2 = LineD.v2;
		if ( i == 0 )
		{
			vI = LineBaseD.ve1;
			vJ = CSectUtil::Get_NextVertexIndex(vI, v1, v2); // 다른쪽 끝단을 알기위해.
		}
		else
		{
			vI = vJ;
			vJ = CSectUtil::Get_NextVertexIndex(vJ, v1, v2);
		}

		T_GSEC_VERTEX vtx1 = GsecPolyD.aVertex[vI];
		T_GSEC_VERTEX vtx2 = GsecPolyD.aVertex[vJ];
		double dx1 = vtx1.dX;
		double dy1 = vtx1.dY + dCzm_gap;
		double dx2 = vtx2.dX;
		double dy2 = vtx2.dY + dCzm_gap;
		double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
		double dt = LineD.dThik;
		double df1 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dx1, dy1);
		double df2 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dx2, dy2);
		BOOL bCompStr = Is_CompStress(df1, df2);
		BOOL bDbCurve = Is_DoubleCurvature(df1, df2);


		if ( i == 0 )
		{
			b1stIsComp = df1 < 0.0 ? TRUE : FALSE;
		}

		if ( bDbCurve )
		{
			Get_b1b2DbCurvature(dbi, df1, df2, db1, db2);
			if ( df1 < df2 )
			{
				dbc += db1;
				dbt += db2;
			}
			else
			{
				dbc += db2;
				dbt += db1;
			}
		}
		else
		{
			if ( bCompStr ) dbc += dbi;
			else         dbt += dbi;
		}
	}

	return TRUE;
}

int CDgnPlateGirder::Get_ConnectedVertexId(int v1, int v2, int v3, int v4)
{
	int vcn = -1;

	if ( v2 == v3 ) vcn = v2;
	else if ( v2 == v4 ) vcn = v2;
	else if ( v1 == v3 ) vcn = v1;
	else if ( v1 == v4 ) vcn = v1;
	else
		ASSERT(0);

	return vcn;
}


BOOL CDgnPlateGirder::Get_PointCoor4beff(double dbe, double dx1, double dy1, double dx2, double dy2, double& dx, double& dy)
{
	// x1, y1이 좌표는 계산하고자 하는 시작점의 좌표로..
	double dX = dx2 - dx1;
	double dY = dy2 - dy1;
	double dX_len = fabs(dX);
	double dY_len = fabs(dY);
	double da = 0.0; // 기울기
	double db = 0.0; // y절편
	double dLength = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
	if ( fabs(dX) < m_dZero ) // 수직.
	{
		if ( dy1 > dy2 )
		{
			dx = dx1;
			dy = dy1 - dbe;
		}
		else
		{
			dx = dx1;
			dy = dy1 + dbe;
		}
	}
	else
	{
		da = dY / dX;
		db = dy1 - da * dx1; // y=ax+b -> b=y-ax
		if ( da > 0 ) // 기울기 +
		{
			double dtheta = atan(fabs(da));
			double dx_len = dbe * cos(dtheta);
			if ( dx1 < dx2 ) // 시작점이 왼쪽.
			{
				//double dLen1 = dLength - dbe;
				//double dx_len = Get_traiangleRatioVal(dLen1, dLength, dX_len);
				dx = dx1 + dx_len;
			}
			else // 시작점이 오른쪽
			{
				//double dx_len = Get_traiangleRatioVal(dbe, dLength, dX_len);
				dx = dx1 - dx_len;
			}
			dy = Get_yOfStrightLine(da, db, dx);
		}
		else if ( da < 0 ) // 기울기 -
		{
			double dtheta = atan(fabs(da));
			double dx_len = dbe * cos(dtheta);
			if ( dx1 < dx2 ) // 시작점이 왼쪽.
			{
				//double dx_len = Get_traiangleRatioVal(dbe, dLength, dX_len);
				//dx = dx1 + dx_len;
				dx = dx1 + dx_len;
			}
			else // 시작점이 오른쪽
			{
				double dLen1 = dLength - dbe;
				//double dx_len = Get_traiangleRatioVal(dLen1, dLength, dX_len);
				dx = dx1 - dx_len;
			}
			dy = Get_yOfStrightLine(da, db, dx);
		}
		else // 수평
		{
			if ( dx1 < dx2 )
			{
				dy = dy1;
				dx = dx1 + dbe;
			}
			else
			{
				dy = dy1;
				dx = dx1 - dbe;
			}
		}
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_CompLengthOfPlastic(BOOL bPositive, double dy1, double dz1, double dy2, double dz2, double& dLen_c, double& dLen_t)
{
	double dc = 0.0, dt = 0.0;
	double dLen = CMathFunc::mathLength(dy1, dz1, dy2, dz2);
	if ( dz1*dz2 < 0.0 ) // 
	{
		if ( dz1 > 0.0 )
		{
			dc = dLen * fabs(dz1) / (fabs(dz1) + fabs(dz2));
			dt = dLen - dc;
		}
		else
		{
			dt = dLen * fabs(dz1) / (fabs(dz1) + fabs(dz2));
			dc = dLen - dt;
		}
	}
	else if ( dz1*dz2 > 0.0 )
	{
		if ( dz1 > 0.0 )
		{
			dc = dLen;
			dt = 0.0;
		}
		else
		{
			dc = 0.0;
			dt = dLen;
		}
	}
	else
	{
		if ( dz1*dz2 == 0.0 )
		{
		}
		else if ( dz1 == 0.0 )
		{
			if ( dz2 > 0.0 ) { dc = dLen; dt = 0.0; }
			else { dc = 0.0;  dt = dLen; }
		}
		else if ( dz2 == 0.0 )
		{
			if ( dz1 > 0.0 ) { dc = dLen; dt = 0.0; }
			else { dc = 0.0;  dt = dLen; }
		}
	}

	if ( bPositive )
	{
		dLen_c = dc;
		dLen_t = dt;
	}
	else
	{
		dLen_c = dt;
		dLen_t = dc;
	}

	return TRUE;
}


double CDgnPlateGirder::Get_Psi(double dSig1, double dSig2)
{
	double dPsi = dSig1 == 0.0 ? dSig2 / m_dZero : dSig2 / dSig1;

	return dPsi;
}

BOOL CDgnPlateGirder::Get_Psi(double dStr1, double dStr2, double& dSig1, double& dSig2, double& dPsi)
{
	BOOL bIs1_Sig1 = FALSE;
	Get_f1_f2(dStr1, dStr2, dSig1, dSig2, bIs1_Sig1);
	dPsi = Get_Psi(dSig1, dSig2);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_GLineDgnPartData(const T_GLINE_DGN_D& GLineDgnD, CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&>& aTopFlangePart, CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&>& aBotFlangePart, CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&>& aWebPart)
{
	aTopFlangePart.RemoveAll();
	aBotFlangePart.RemoveAll();
	aWebPart.RemoveAll();
	int i = 0;
	int iPartLineSize = GLineDgnD.aPartLine.GetSize();
	for ( i = 0; i < iPartLineSize; i++ )
	{
		T_GLINE_PART_DGN GLinePartD = GLineDgnD.aPartLine[i];
		int nPartType = GLinePartD.nPartType;
		if ( nPartType == 0 ) aTopFlangePart.Add(GLinePartD);
		else if ( nPartType == 1 ) aBotFlangePart.Add(GLinePartD);
		else if ( nPartType == 2 ) aWebPart.Add(GLinePartD);
		else ASSERT(0);
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_DLinePartData(const T_DLINE_CALC_D& DLineInfo, CArray<T_DLINE_PART_D, T_DLINE_PART_D&>& aTopFlangePart, CArray<T_DLINE_PART_D, T_DLINE_PART_D&>& aBotFlangePart, CArray<T_DLINE_PART_D, T_DLINE_PART_D&>& aWebPart)
{
	aTopFlangePart.RemoveAll();
	aBotFlangePart.RemoveAll();
	aWebPart.RemoveAll();
	int i = 0;
	int iPartLineSize = DLineInfo.aPartLine.GetSize();
	for ( i = 0; i < iPartLineSize; i++ )
	{
		T_DLINE_PART_D DLinePartD = DLineInfo.aPartLine[i];
		int nPartType = DLinePartD.nPartType;
		if ( nPartType == 0 ) aTopFlangePart.Add(DLinePartD);
		else if ( nPartType == 1 ) aBotFlangePart.Add(DLinePartD);
		else if ( nPartType == 2 ) aWebPart.Add(DLinePartD);
		else ASSERT(0);
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_CompositeGeneralSectionInfo(T_PLATE_GIRDER_CALC_D& param, BOOL bChkXY)
{
	T_SECT_D SectD = param.SectElem.SectD;
	memset(param.dZbar_sb, 0, sizeof(param.dZbar_sb));
	memset(param.dCzm_sb, 0, sizeof(param.dCzm_sb));
	memset(param.dCzp_sb, 0, sizeof(param.dCzp_sb));
	memset(param.dArea_sb, 0, sizeof(param.dArea_sb));
	memset(param.dBc_sb, 0, sizeof(param.dBc_sb));
	memset(param.dHt, 0, sizeof(param.dHt));
	memset(param.aY_sb, 0, sizeof(param.aY_sb));
	memset(param.aZ_sb, 0, sizeof(param.aZ_sb));

	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
	for ( int iPos = 0; iPos < 2; iPos++ )
	{
		BOOL bPosI = (iPos == 0);
		auto SectBaseD = param.bTapered && !bPosI ? SectD.SectBefore.SectJ : SectD.SectBefore.SectI;

		double aMaxPt[2] = { 0.0 }, aMinPt[2] = { 0.0 };
		CSectUtil::Get_MaxMinPoint4GenPart(SectBaseD, aMaxPt, aMinPt);

		double dArea_slab = 0.0;
		double dZbar_slab = 0.0;
		double dCzp_slab = 0.0, dCzm_slab = 0.0;
		double dCyp_slab = 0.0, dCym_slab = 0.0;
		int nBeforePart = SectBaseD.nBeforePart; // (General Composite) Before 의 Part 수
		int iPartSize = SectBaseD.aGeneralPart.GetSize();
		for ( int i = 0; i < iPartSize; i++ )
		{
			T_GSEC_PART PartD = SectBaseD.aGeneralPart[i];
			if ( PartD.bPlane )
			{
				dArea_slab = PartD.Stiffness.Area;
				dZbar_slab = PartD.ZBar;
				dCyp_slab = PartD.Stiffness.Cyp;
				dCym_slab = PartD.Stiffness.Cym;
				dCzp_slab = PartD.Stiffness.Czp;
				dCzm_slab = PartD.Stiffness.Czm;
				if ( bChkXY )
				{
					param.aY_sb[iPos][0] = PartD.Stiffness.dx1, param.aZ_sb[iPos][0] = PartD.Stiffness.dy1;
					param.aY_sb[iPos][1] = PartD.Stiffness.dx2, param.aZ_sb[iPos][1] = PartD.Stiffness.dy2;
					param.aY_sb[iPos][2] = PartD.Stiffness.dx3, param.aZ_sb[iPos][2] = PartD.Stiffness.dy3;
					param.aY_sb[iPos][3] = PartD.Stiffness.dx4, param.aZ_sb[iPos][3] = PartD.Stiffness.dy4;
				}
				break;
			}
		}

		param.dZbar_sb[iPos] = dZbar_slab;
		param.dCzm_sb[iPos] = dCzm_slab;
		param.dCzp_sb[iPos] = dCzp_slab;
		param.dArea_sb[iPos] = dArea_slab;
		param.dHt[iPos] = fabs(aMaxPt[1] - aMinPt[1]);
		param.dBc_sb[iPos] = dCym_slab + dCyp_slab;
	}
	return TRUE;
}

void CDgnPlateGirder::Set_LineDgnBase(int iLineType, double db, double dt, double dy1, double dz1, double dy2, double dz2, double df1, double df2,
	BOOL bCompStr, BOOL bDbCurve, BOOL bIs1_sig1, double dy0, double dz0, T_GLINE_DGN_B& LineDgnBase)
{
	LineDgnBase.iLineType = iLineType;
	LineDgnBase.db = db;
	LineDgnBase.dt = dt;
	LineDgnBase.dy1 = dy1;
	LineDgnBase.dz1 = dz1;
	LineDgnBase.dy2 = dy2;
	LineDgnBase.dz2 = dz2;
	LineDgnBase.df1 = df1;
	LineDgnBase.df2 = df2;
	LineDgnBase.bCompStr = bCompStr;
	LineDgnBase.bDbCurve = bDbCurve;
	LineDgnBase.bIs1_sig1 = bIs1_sig1;
	LineDgnBase.dy0 = dy0;
	LineDgnBase.dz0 = dz0;
}

BOOL CDgnPlateGirder::Calc_Vbw_Rd(T_PLATE_GIRDER_CALC_D& param, double& dVbw_Rd, T_VERTICAL_SHAER_RES_D& ResD)
{
	// 1. Initailize & Get Basic Input Data
	T_SECT_D SectD = param.SectElem.SectD;
	dVbw_Rd = 0.0;
	bool bBoxSect = m_pDgnSectUtil->Is_StlBoxSect(&SectD);
	int iMatClass = param.MatlElem.iMatClass;
	int iEndpost = param.TsesPosi.iEndpost; // 0=Rigid end post, 1=Non-rigid end post, 2=No end post
	double dhw = m_pDgnSectUtil->Get_Size_hw(&SectD, param.bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, param.bPosiI);
	double dTw_t = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, param.bPosiI);
	double dfy = param.MatlElem.MatdD.Data1.Design.S_Fy1;
	double dfy1 = param.MatlElem.MatdD.Data1.Design.S_Fy2;
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dEta = Get_Eta4SteelGrade(iMatClass, dfy);

	T_DGN_STL_STIFFENER_PART WebStiffD;
	m_pDgnSectUtil->Get_Web_StiffenerD(true, &SectD, WebStiffD);
	Get_ConvertSectStiffenerD(WebStiffD, param.arLstiPosi);

	// 2. Calc. Lambda_w
	double dLambda_w = 0.0;
	if ( !Calc_Lambda_w(param, dLambda_w, ResD.ShearResD[0]) ) { ASSERT(0); } // Left Web

	// 3 . Calc Xi Factor
	int iLamda_w_scope[2] = { 0,0 };
	double dXw[2] = { 0.0, 0.0 };

	if ( !Get_ContributionFromWebFactor(iEndpost, dEta, dLambda_w, dXw[0], iLamda_w_scope[0]) ) { ASSERT(0); }

	// 4. Calc. Vbw_Rd
	double dVbw_Rd_LR[2] = { 0.0, 0.0 }; // Left Web, Right Web
	double dgam_M1 = m_dGamma_M1;
	dVbw_Rd_LR[0] = fabs(dgam_M1) < m_dZero ? 0.0 : dXw[0] * dfyw*dhw*dTw / (sqrt(3.0)*dgam_M1);

	// Repeat for right web
	if ( bBoxSect ) // Right Web
	{
		WebStiffD.Initialize(); param.arLstiPosi.RemoveAll();
		m_pDgnSectUtil->Get_Web_StiffenerD(false, &SectD, WebStiffD);
		Get_ConvertSectStiffenerD(WebStiffD, param.arLstiPosi);

		if ( !Calc_Lambda_w(param, dLambda_w, ResD.ShearResD[1]) ) { ASSERT(0); }
		if ( !Get_ContributionFromWebFactor(iEndpost, dEta, dLambda_w, dXw[1], iLamda_w_scope[1]) ) { ASSERT(0); }
		dVbw_Rd_LR[1] = fabs(dgam_M1) < m_dZero ? 0.0 : dXw[1] * dfyw*dhw*dTw / (sqrt(3.0)*dgam_M1);
	}

	dVbw_Rd = dVbw_Rd_LR[0] + dVbw_Rd_LR[1];

	// 5. Save Result
	ResD.dVbw_Rd = dVbw_Rd;

	// Save for Left Web Part
	ResD.ShearResD[0].dVbw_Rd = dVbw_Rd_LR[0];
	ResD.ShearResD[0].dXw = dXw[0];
	ResD.ShearResD[0].iEndpost = iEndpost;
	ResD.ShearResD[0].iLamda_w_scope = iLamda_w_scope[0];

	if ( bBoxSect ) 	// Save for Right Web Part
	{
		ResD.ShearResD[1].dVbw_Rd = dVbw_Rd_LR[1];
		ResD.ShearResD[1].dXw = dXw[1];
		ResD.ShearResD[1].iEndpost = iEndpost;
		ResD.ShearResD[1].iLamda_w_scope = iLamda_w_scope[1];
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Calc_Lambda_w(T_PLATE_GIRDER_CALC_D& param, double& dLambda_w, T_VERTICAL_SHAER_RES_UNIT_D& ResD)
{
	// 1. Initailize
	dLambda_w = 0.0;

	// 2. Get Input Data
	int iEndpost = param.TsesPosi.iEndpost; // 0=Rigid end post, 1=Non-rigid end post, 2=No end post
	int iLongStiffSize = param.arLstiPosi.GetSize();
	int iMatClass = param.MatlElem.iMatClass;
	BOOL bPartLongStiff = iLongStiffSize > 0 ? TRUE : FALSE;
	BOOL bLongStiff = iLongStiffSize == 0 ? FALSE : TRUE;
	BOOL bEndSupport = param.TsesPosi.bUseStif;
	BOOL bTranStiff = param.TrstPosi.bUseStif;
	BOOL bInterRigidStif = param.TrstPosi.bInterRigidStif;
	BOOL bInterNonRigidStif = param.TrstPosi.bInterNonRigidStif;

	T_SECT_D SectD = param.SectElem.SectD;
	BOOL bPosiI = param.bPosiI;
	double dCosA = fabs(cos(m_pDgnSectUtil->Get_Size_WebSlope(&SectD, 0, bPosiI)));
	double dhw = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dhw_in = m_pDgnSectUtil->Get_Size_Inclind_hw(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dfy = param.MatlElem.MatdD.Data1.Design.S_Fy1;
	double dfy1 = param.MatlElem.MatdD.Data1.Design.S_Fy2;
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dEpsi = Get_Epsilon(dfyw);
	double dInterRigPitch = param.TrstPosi.dDistRstif;
	double dNonRigPitch = param.TrstPosi.dDistNRstif;
	double dRigidPitch = param.TrstPosi.dPitch;
	double dEta = Get_Eta4SteelGrade(iMatClass, dfy);

	// 2. a // 5.3(3) The slenderness parameter (Lamda_w) , EN 1993-1-5 : 2004, 23page
	double dMinValue = -1.0 / pow(m_dZero, 10);
	double dMax_Lamda_wi = dMinValue;
	double dMax_Bar_Lamda_w = dMinValue;

	// Calc. Isl
	double dIsl = 0.0, dIsl_org = 0.0;

	for ( int i = 0; i < iLongStiffSize; i++ )
	{
		CPG_LSTI_DATA& LstiD = param.arLstiPosi[i];
		double dh_ls = LstiD.dWidth;
		double dt_ls = LstiD.dt;
		double dI_ls = 0.0;
		if ( !Get_IslOfLongiStiff(LstiD, dTw, dEpsi, dI_ls) ) { dI_ls = 0.0; }
		dIsl += dI_ls;
	}
	dIsl_org = dIsl;
	dIsl = 1.0 / 3.0*dIsl;// 5.3(4) The second moment of area of a longitudinal stiffener should be reduced to 1/3 of their actual value when calculating k_tau

	// Check the Whole Web
	BOOL bIsShearBuck = FALSE;
	double dk_tau_p = 0.0, dk_tau_st_p = 0.0, dk_tau_st_Limit_p = 0.0, dLamda_w_part = 0.0, dCheck_lim = 0.0;

	int iLamda_w_Type = Get_Lamda_w_Type4Shear(bPartLongStiff, param.TrstPosi);
	BOOL bStiffened = (iLamda_w_Type == 1 || iLamda_w_Type == 2);

	if ( Get_SlendernessParamLamda(iLamda_w_Type, iLongStiffSize, dhw_in, dRigidPitch, dTw, dIsl, dEpsi, dk_tau_p, dk_tau_st_p,
		dk_tau_st_Limit_p, dLamda_w_part) ) {
		dMax_Bar_Lamda_w = dLamda_w_part;
	}
	else { ASSERT(0); }

	int nShearBuckCount = 0;
	if ( !Get_WidthThickRatio4Web(bStiffened, dEta, dhw_in, dTw, dEpsi, dk_tau_p, dCheck_lim, bIsShearBuck) ) { ASSERT(0); }
	if ( bIsShearBuck ) { nShearBuckCount++; }

	// Check
	int nLSsize_cr = 0;
	double dLamda_wi = 0.0, dhwi = 0.0, dai = 0.0, dk_taui = 0.0, dk_tau_st_i = 0.0, dk_tau_st_Limit_i = 0.0, dDist4Shr = 0.0;
	double dhi_cr = 0.0, dai_cr = 0.0, dk_tau_si_cr = 0.0, dk_tau_sti_cr = 0.0, dk_tau_st_lti_cr = 0.0, dIsli_cr = 0.0;

	if ( !bInterRigidStif && !bInterNonRigidStif ) // Case 1.
	{
		Get_SlendernessParamLamda(iLamda_w_Type, iLongStiffSize, dhw_in, dRigidPitch, dTw, dIsl, dEpsi, dk_taui, dk_tau_st_i,
			dk_tau_st_Limit_i, dLamda_wi);
		if ( dMax_Lamda_wi < dLamda_wi )
		{
			dMax_Lamda_wi = dLamda_wi;
			dhi_cr = dhwi;
			dai_cr = dRigidPitch;
			dk_tau_si_cr = dk_taui;
			dk_tau_sti_cr = dk_tau_st_i;
			nLSsize_cr = iLongStiffSize;
			dk_tau_st_lti_cr = dk_tau_st_Limit_i;
			dIsli_cr = dIsl;
		}
		dDist4Shr = dRigidPitch;
	}
	else if ( !bInterRigidStif && bInterNonRigidStif ) // Case 2.
	{
		double dDist1 = dNonRigPitch;
		double dDist2 = dRigidPitch - dNonRigPitch;

		for ( int j = 0; j < 2; j++ ) // Column
		{
			dai = j == 0 ? dDist1 : dDist2;
			dk_taui = 0.0;
			dLamda_wi = 0.0;
			Get_SlendernessParamLamda(2, iLongStiffSize, dhw_in, dai, dTw, dIsl, dEpsi, dk_taui, dk_tau_st_i, dk_tau_st_Limit_i, dLamda_wi);
			if ( dMax_Lamda_wi < dLamda_wi )
			{
				dMax_Lamda_wi = dLamda_wi;
				dhi_cr = dhwi;
				dai_cr = dai;
				dk_tau_si_cr = dk_taui;
				dk_tau_sti_cr = dk_tau_st_i;
				nLSsize_cr = iLongStiffSize;
				dk_tau_st_lti_cr = dk_tau_st_Limit_i;
				dIsli_cr = dIsl;
			}
		}
		dDist4Shr = max(dDist1, dDist2);
	}
	else if ( bInterRigidStif && !bInterNonRigidStif ) // Case 3.
	{
		double dDist1 = dInterRigPitch;
		double dDist2 = dRigidPitch - dInterRigPitch;

		for ( int j = 0; j < 2; j++ ) // Column
		{
			dai = j == 0 ? dDist1 : dDist2;
			dk_taui = 0.0;
			dLamda_wi = 0.0;
			Get_SlendernessParamLamda(2, iLongStiffSize, dhw_in, dai, dTw, dIsl, dEpsi, dk_taui, dk_tau_st_i, dk_tau_st_Limit_i, dLamda_wi);

			if ( dMax_Lamda_wi < dLamda_wi )
			{
				dMax_Lamda_wi = dLamda_wi;
				dhi_cr = dhwi;
				dai_cr = dai;
				dk_tau_si_cr = dk_taui;
				dk_tau_sti_cr = dk_tau_st_i;
				nLSsize_cr = iLongStiffSize;
				dk_tau_st_lti_cr = dk_tau_st_Limit_i;
				dIsli_cr = dIsl;
			}
		}
		dDist4Shr = max(dDist1, dDist2);
	}
	else if ( bInterRigidStif && bInterNonRigidStif ) // Case 4.
	{
		for ( int j = 0; j < 4; j++ ) // Column
		{
			if ( dInterRigPitch < dNonRigPitch + m_dZero ) // Inter-Non-rigid가 Inter-rigid보다 멀리 있다.
			{
				if ( j == 0 ) { dai = dInterRigPitch; } // a1
				else if ( j == 1 ) { dai = dNonRigPitch - dInterRigPitch; } // a2
				else if ( j == 2 ) { dai = dRigidPitch - dNonRigPitch; } // a3
				else { dai = dRigidPitch - dInterRigPitch; } // a4
			}
			else
			{
				if ( j == 0 ) { dai = dNonRigPitch; } // a1
				else if ( j == 1 ) { dai = dInterRigPitch - dNonRigPitch; } // a2
				else if ( j == 2 ) { dai = dRigidPitch - dInterRigPitch; } // a3
				else { dai = dInterRigPitch; } // a4
			}

			dk_taui = 0.0;
			dLamda_wi = 0.0;
			Get_SlendernessParamLamda(2, iLongStiffSize, dhw_in, dai, dTw, dIsl, dEpsi, dk_taui, dk_tau_st_i, dk_tau_st_Limit_i, dLamda_wi);

			if ( dMax_Lamda_wi < dLamda_wi )
			{
				dMax_Lamda_wi = dLamda_wi;
				dhi_cr = dhwi;
				dai_cr = dai;
				dk_tau_si_cr = dk_taui;
				dk_tau_sti_cr = dk_tau_st_i;
				nLSsize_cr = iLongStiffSize;
				dk_tau_st_lti_cr = dk_tau_st_Limit_i;
				dIsli_cr = dIsl;
			}
		}

		double dDist[3] = { 0.0, 0.0, 0.0 };

		for ( int j = 0; j < 3; j++ )
		{
			if ( dInterRigPitch < dNonRigPitch + m_dZero ) // Inter-Non-rigid가 Inter-rigid보다 멀리 있다.
			{
				if ( j == 0 ) { dDist[0] = dInterRigPitch; } // a1
				else if ( j == 1 ) { dDist[1] = dNonRigPitch - dInterRigPitch; } // a2
				else if ( j == 2 ) { dDist[2] = dRigidPitch - dNonRigPitch; } // a3
			}
			else if ( dInterRigPitch > dNonRigPitch )
			{
				if ( j == 0 ) { dDist[0] = dNonRigPitch; } // a1
				else if ( j == 1 ) { dDist[1] = dInterRigPitch - dNonRigPitch; } // a2
				else if ( j == 2 ) { dDist[2] = dRigidPitch - dInterRigPitch; } // a3
			}
		}
		//
		double dMaxDist = 0;
		for ( int j = 0; j < 3; j++ )
		{
			if ( j == 0 )
			{
				dMaxDist = dDist[0];
			}
			else
			{
				if ( dMaxDist < dDist[j] ) { dMaxDist = dDist[j]; }
			}
		}
		dDist4Shr = dMaxDist;
	}

	// 5.3(5) For webs with longitudinal stiffeners the slenderness parameter (lamda_w) in (3)
	// should not be taken as less than , EN 1993-1-5 : 2004, 24page
	if ( bLongStiff && bTranStiff )
	{
		int iLongStiffSize_sub = 0;
		double dIsl_i = 0.0;

		for ( int i = 0; i < iLongStiffSize + 1; i++ )
		{
			if ( i == 0 )
			{
				dhwi = dCosA < m_dZero ? 0.0 : param.arLstiPosi[i].dz / dCosA;
			}
			else if ( i > 0 && i < iLongStiffSize )
			{
				dhwi = dCosA < m_dZero ? 0.0 : (param.arLstiPosi[i].dz - param.arLstiPosi[i - 1].dz) / dCosA;
			}
			else
			{
				dhwi = dCosA < m_dZero ? 0.0 : (dhw - param.arLstiPosi[i - 1].dz) / dCosA;
			}

			if ( !bInterRigidStif && !bInterNonRigidStif ) // Case 1.
			{
				dai = dRigidPitch;
				dk_taui = 0.0, dLamda_wi = 0.0;
				Get_SlendernessParamLamda(2, iLongStiffSize_sub, dhwi, dai, dTw, dIsl_i, dEpsi, dk_taui, dk_tau_st_i,
					dk_tau_st_Limit_i, dLamda_wi);

				if ( dMax_Lamda_wi < dLamda_wi )
				{
					dMax_Lamda_wi = dLamda_wi;
					dhi_cr = dhwi;
					dai_cr = dai;
					dk_tau_si_cr = dk_taui;
					dk_tau_sti_cr = dk_tau_st_i;
					nLSsize_cr = iLongStiffSize_sub;
					dk_tau_st_lti_cr = dk_tau_st_Limit_i;
					dIsli_cr = dIsl_i;
				}
			}
			else if ( !bInterRigidStif && bInterNonRigidStif ) // Case 2.
			{
				for ( int j = 0; j < 2; j++ ) // Column
				{
					dk_taui = 0.0; dLamda_wi = 0.0;
					dai = j == 0 ? dNonRigPitch : dRigidPitch - dNonRigPitch;

					Get_SlendernessParamLamda(2, iLongStiffSize_sub, dhwi, dai, dTw, dIsl_i, dEpsi, dk_taui, dk_tau_st_i,
						dk_tau_st_Limit_i, dLamda_wi);

					if ( dMax_Lamda_wi < dLamda_wi )
					{
						dMax_Lamda_wi = dLamda_wi;
						dhi_cr = dhwi;
						dai_cr = dai;
						nLSsize_cr = iLongStiffSize_sub;
						dk_tau_si_cr = dk_taui;
						dk_tau_sti_cr = dk_tau_st_i;
						dk_tau_st_lti_cr = dk_tau_st_Limit_i;
						dIsli_cr = dIsl_i;
						//nSubPannelCalcType = Get_SubPannelCalcType(iLongStiffSize);
					}
				}
			}
			else if ( bInterRigidStif && !bInterNonRigidStif ) // Case 3.
			{
				for ( int j = 0; j < 2; j++ ) // Column
				{
					dk_taui = 0.0;	dLamda_wi = 0.0;
					dai = j == 0 ? dInterRigPitch : dRigidPitch - dInterRigPitch;

					Get_SlendernessParamLamda(2, iLongStiffSize_sub, dhwi, dai, dTw, dIsl_i, dEpsi, dk_taui, dk_tau_st_i,
						dk_tau_st_Limit_i, dLamda_wi);
					if ( dMax_Lamda_wi < dLamda_wi )
					{
						dMax_Lamda_wi = dLamda_wi;
						dhi_cr = dhwi;
						dai_cr = dai;
						nLSsize_cr = iLongStiffSize_sub;
						dk_tau_si_cr = dk_taui;
						dk_tau_sti_cr = dk_tau_st_i;
						dk_tau_st_lti_cr = dk_tau_st_Limit_i;
						dIsli_cr = dIsl_i;
					}
				}
			}
			else if ( bInterRigidStif && bInterNonRigidStif ) // Case 4.
			{
				for ( int j = 0; j < 4; j++ ) // Column
				{
					double dai = 0.0;
					dk_taui = 0.0; dLamda_wi = 0.0;
					if ( dInterRigPitch < dNonRigPitch ) // Inter-Non-rigid가 Inter-rigid보다 멀리 있다.
					{
						if ( j == 0 ) { dai = dInterRigPitch; } // a1
						else if ( j == 1 ) { dai = dNonRigPitch - dInterRigPitch; } // a2
						else if ( j == 2 ) { dai = dRigidPitch - dNonRigPitch; } // a3
						else { dai = dRigidPitch - dInterRigPitch; } // a4
					}
					else if ( dInterRigPitch > dNonRigPitch )
					{
						if ( j == 0 ) { dai = dNonRigPitch; } // a1
						else if ( j == 1 ) { dai = dInterRigPitch - dNonRigPitch; } // a2
						else if ( j == 2 ) { dai = dRigidPitch - dInterRigPitch; } // a3
						else { dai = dInterRigPitch; } // a4
					}
					//            

					Get_SlendernessParamLamda(2, iLongStiffSize_sub, dhwi, dai, dTw, dIsl_i, dEpsi, dk_taui, dk_tau_st_i,
						dk_tau_st_Limit_i, dLamda_wi);

					if ( dMax_Lamda_wi < dLamda_wi )
					{
						dMax_Lamda_wi = dLamda_wi;
						dhi_cr = dhwi;
						dai_cr = dai;
						nLSsize_cr = iLongStiffSize_sub;
						dk_tau_si_cr = dk_taui;
						dk_tau_sti_cr = dk_tau_st_i;
						dk_tau_st_lti_cr = dk_tau_st_Limit_i;
						dIsli_cr = dIsl_i;
					}
				}
			}
		}
	}

	// contribution from the web factor
	// 5.3(5) For webs with longitudinal stiffeners the slenderness parameter (lamda_w) in (3) should not be taken as less than ,EN 1993-1-5 : 2004, 24page   
	//if(dMax_Bar_Lamda_w<dMax_Lamda_wi)
	//  dMax_Bar_Lamda_w = dMax_Lamda_wi;
	if ( fabs(dMax_Bar_Lamda_w - dMinValue) < m_dZero ) { dMax_Bar_Lamda_w = 0.0; } // If do not find value, set 0.0
	if ( fabs(dMax_Lamda_wi - dMinValue) < m_dZero ) { dMax_Lamda_wi = 0.0; }	// If do not find value, set 0.0

	double dLamda_w_cr = max(dMax_Bar_Lamda_w, dMax_Lamda_wi);

	// Save Result
	dLambda_w = dMax_Bar_Lamda_w;
	ResD.bIsShearBuck = bIsShearBuck;
	ResD.nShearBuckCount = nShearBuckCount;
	ResD.bStiffened = bStiffened;
	ResD.dIsl = dIsl;
	ResD.dIsl_org = dIsl_org;
	ResD.dMax_Bar_Lamda_w = dMax_Bar_Lamda_w;
	ResD.dRigidPitch = dRigidPitch;
	ResD.iLongStiffSize = iLongStiffSize;
	ResD.dDist4Shr = dDist4Shr;

	ResD.dhi_cr = dhi_cr;
	ResD.dai_cr = dai_cr;
	ResD.dk_tau_si_cr = dk_tau_si_cr;
	ResD.dk_tau_sti_cr = dk_tau_sti_cr;
	ResD.nLSsize_cr = nLSsize_cr;
	ResD.dk_tau_st_lti_cr = dk_tau_st_lti_cr;
	ResD.dIsli_cr = dIsli_cr;
	ResD.dMax_Lamda_wi = dMax_Lamda_wi;

	ResD.dk_tau_p = dk_tau_p;
	ResD.dk_tau_st_p = dk_tau_st_p;
	ResD.dk_tau_st_Limit_p = dk_tau_st_Limit_p;
	ResD.dLamda_w_part = dLamda_w_part;
	ResD.dLamda_w_cr = dLamda_w_cr;
	ResD.iLamda_w_Type = iLamda_w_Type;

	return TRUE;
}

void CDgnPlateGirder::Set_Calc_Vbw_Param(double dAlpha, double dk_tau, double dk_tau_st, double dk_tau_st_limit, double dIsl, double dhw, double dt,
	double dLamda_w, double dLamda_w_cr, int iLamda_w_Type, T_PRVS_Vbw_Rd_Param& rData)
{
	rData.dAlpha = dAlpha;
	rData.dk_tau = dk_tau;
	rData.dk_tau_st = dk_tau_st;
	rData.dk_tau_st_limit = dk_tau_st_limit;
	rData.dIsl = dIsl;
	rData.dhw = dhw;
	rData.dt = dt;
	rData.dLamda_w = dLamda_w;
	rData.dLamda_w_cr = dLamda_w_cr;
	rData.iLamda_w_Type = iLamda_w_Type;
}

void CDgnPlateGirder::Set_Calc_Vbw_Param2(int iLamda_w_scope, double dXw, double dVbw_Rd, double dV_Rd, double dVb_Rd_lim, double dV_Edi, double deta3, T_PRVS_Vbw_Rd_Param& rData)
{
	rData.iLamda_w_scope = iLamda_w_scope;
	rData.dXw = dXw;
	rData.dVbw_Rd = dVbw_Rd;
	rData.dV_Rd = dV_Rd;
	rData.dV_Rd_Limit = dVb_Rd_lim;
	rData.dV_Edi = dV_Edi;
	rData.deta3 = deta3;
}

void CDgnPlateGirder::Set_Calc_Vbw_SubPanel_Param(int nLongStfnSize, double dhw, double dt, double dAlpha, double dk_tau, double dk_tau_st, double dk_tau_st_lt, double dIsl, double dLamda_w, T_PRVS_Vbw_Rd_Sub_Panel& rData)
{
	rData.nLongStfnSize = nLongStfnSize;
	rData.dhw = dhw;
	rData.dt = dt;
	rData.dAlpha = dAlpha;
	rData.dk_tau = dk_tau;
	rData.dk_tau_st = dk_tau_st;
	rData.dk_tau_st_lt = dk_tau_st_lt;
	rData.dIsl = dIsl;
	rData.dLamda_w = dLamda_w;
}

void CDgnPlateGirder::Set_Calc_FlangeShearstress_Param(double dtau_Ed, double dtau_b_Rd, double dV_Edi, double dQi, double dIyyi, double dtf, T_PRVS_Vbw_Rd_Param& rData)
{
	rData.dtau_Ed = dtau_Ed;
	rData.dtau_b_Rd = dtau_b_Rd;
	rData.dV_Edi = dV_Edi;
	rData.dQi = dQi;
	rData.dIyyi = dIyyi;
	rData.dt = dtf;
}

void CDgnPlateGirder::SetPgbrBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PGBR_BASE& rPgbrGd, T_PGBR_BASE& rData)
{
	rPgbrGd.bChk = rData.bChk = bChk;
	rPgbrGd.ElemK = rData.ElemK = ElemK.first;
	rPgbrGd.nPosi = rData.nPosi = nPosi;
	rPgbrGd.LcomK = rData.LcomK = iDgnLcomNo;
	rPgbrGd.OrgLcomK = rData.OrgLcomK = iOrgLcomNo;
	rPgbrGd.iMax = rData.iMax = iMaxMinType;

	rPgbrGd.dFx = aForceGd[0];
	rPgbrGd.dMy = aForceGd[4];
	rPgbrGd.dMz = aForceGd[5];

	rData.dFx = aForce[0]; //- aForceGd[0];
	rData.dMy = aForce[4]; //- aForceGd[4]; 
	rData.dMz = aForce[5]; //- aForceGd[5];
	rData.dMa_Ed = aForceGd[4];
	rData.dMc_Ed = rData.dMy;

	const _DGN_FORC_CRC& ForceConst = GetScfpForce4Lcom();
	m_pForcCtrl->ConvertDgnForceToPSCForce(nPosi, ForceConst, rData.ForceConst);

	const _DGN_SCFP_PART_LCASE& ScfpStress = GetScfpStress4Lcom();
	m_pForcCtrl->ConvertDgnScfpToScfp(ScfpStress.ForceP1, rData.ScfpForce);
}

BOOL CDgnPlateGirder::SetRhocBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, const T_PGBR_DETAIL& PgbrDetail, T_RHOC_BASE& Rho_C)
{
	Rho_C.bChk = bChk; // 계산을 통과하면 TRUE
	Rho_C.ElemK = ElemK.first;
	Rho_C.nPosi = nPosi;
	Rho_C.LcomK = iDgnLcomNo;
	Rho_C.OrgLcomK = iOrgLcomNo;
	Rho_C.iMax = iMaxMinType;

	Rho_C.dRho_c = PgbrDetail.dRho_c;
	Rho_C.dAa_eff_Fx = PgbrDetail.EffGirderFx.Area;
	Rho_C.dIya_eff_Fx = PgbrDetail.EffGirderFx.Ryy;
	Rho_C.dIza_eff_Fx = PgbrDetail.EffGirderFx.Rzz;
	Rho_C.dAc_eff_Fx = PgbrDetail.EffCompositeFx.Area;
	Rho_C.dIyc_eff_Fx = PgbrDetail.EffCompositeFx.Ryy;
	Rho_C.dIzc_eff_Fx = PgbrDetail.EffCompositeFx.Rzz;

	Rho_C.dAa_eff_My = PgbrDetail.EffGirder.Area;
	Rho_C.dIya_eff_My = PgbrDetail.EffGirder.Ryy;
	Rho_C.dIza_eff_My = PgbrDetail.EffGirder.Rzz;
	Rho_C.dAc_eff_My = PgbrDetail.EffComposite.Area;
	Rho_C.dIyc_eff_My = PgbrDetail.EffComposite.Ryy;
	Rho_C.dIzc_eff_My = PgbrDetail.EffComposite.Rzz;

	Rho_C.RhocBeforeFx = PgbrDetail.RhocBeforeFx;
	Rho_C.RhocAfterFx = PgbrDetail.RhocAfterFx;
	Rho_C.RhocBeforeMy = PgbrDetail.RhocBeforeMy;
	Rho_C.RhocAfterMy = PgbrDetail.RhocAfterMy;

	return TRUE;
}

void CDgnPlateGirder::SetPrvsBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PRVS_BASE& rPrvsGd, T_PRVS_BASE& rData)
{
	rPrvsGd.bChk = rData.bChk = bChk;
	rPrvsGd.ElemK = rData.ElemK = ElemK.first;
	rPrvsGd.nPosi = rData.nPosi = nPosi;
	rPrvsGd.LcomK = rData.LcomK = iDgnLcomNo;
	rPrvsGd.OrgLcomK = rData.OrgLcomK = iOrgLcomNo;
	rPrvsGd.iMax = rData.iMax = iMaxMinType;

	rPrvsGd.dFx = aForceGd[0];
	rPrvsGd.dFz = aForceGd[2];
	rPrvsGd.dMy = aForceGd[4];
	rPrvsGd.dMz = aForceGd[5];

	rData.dFx = aForce[0];
	rData.dFz = aForce[2];
	rData.dMy = aForce[4];
	rData.dMz = aForce[5];
	rData.dN_Ed = aForceGd[0] + aForce[0];
	rData.dV_Ed = aForceGd[2] + aForce[2];
	rData.dMa_Ed = aForceGd[4];
}

void CDgnPlateGirder::SetPltbBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce,
	double dV1, double dV2, double dM1, double dM2, double dC4, T_PLTB_BASE& rPltbGd, T_PLTB_BASE& rData)
{
	rPltbGd.bChk = rData.bChk = bChk;
	rPltbGd.ElemK = rData.ElemK = ElemK.first;
	rPltbGd.nPosi = rData.nPosi = nPosi;
	rPltbGd.LcomK = rData.LcomK = iDgnLcomNo;
	rPltbGd.OrgLcomK = rData.OrgLcomK = iOrgLcomNo;
	rPltbGd.iMax = rData.iMax = iMaxMinType;

	rPltbGd.dFx = aForceGd[0];
	rPltbGd.dMy = aForceGd[4];
	rPltbGd.dMz = aForceGd[5];

	rData.dFx = aForce[0];
	rData.dMy = aForce[4];
	rData.dMz = aForce[5];

	rData.dN_Ed = aForceGd[0] + aForce[0];
	rData.dM_Ed = aForceGd[4] + aForce[4];
	rData.dV1 = dV1;
	rData.dV2 = dV2;
	rData.dM1 = dM1;
	rData.dM2 = dM2;
	rData.dC4 = dC4;

}

void CDgnPlateGirder::SetPrtfBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForce, T_PRTF_BASE& rData)
{
	rData.bChk = bChk;
	rData.ElemK = ElemK.first;
	rData.nPosi = nPosi;
	rData.LcomK = iDgnLcomNo;
	rData.OrgLcomK = iOrgLcomNo;
	rData.iMax = iMaxMinType;
	rData.dFx = aForce[0];
	rData.dMy = aForce[4];
	rData.dMz = aForce[5];
	rData.dN_Ed = aForce[0];
	rData.dMy_Ed = aForce[4];
	rData.dMz_Ed = aForce[5];
	rData.dF_Ed = aForce[2];
}

void CDgnPlateGirder::SetPglsBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PGLS_BASE& rPglsGd, T_PGLS_BASE& rData)
{
	rData.bChk = bChk;
	rData.ElemK = ElemK.first;
	rData.nPosi = nPosi;
	rData.LcomK = iDgnLcomNo;
	rData.OrgLcomK = iOrgLcomNo;
	rData.iMax = iMaxMinType;

	rPglsGd.dFx = aForceGd[0];
	rPglsGd.dFz = aForceGd[2];
	rPglsGd.dMy = aForceGd[4];
	rPglsGd.dMz = aForceGd[5];

	rData.dFx = aForce[0];
	rData.dFz = aForce[2];
	rData.dMy = aForce[4];
	rData.dMz = aForce[5];
	rData.dMa_Ed = aForceGd[4];
}

void CDgnPlateGirder::SetPgfrBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForce, T_PGRF_BASE& rData)
{
	rData.bChk = bChk;
	rData.ElemK = ElemK.first;
	rData.nPosi = nPosi;
	rData.LcomK = iDgnLcomNo;
	rData.OrgLcomK = iOrgLcomNo;
	rData.iMax = iMaxMinType;
	rData.dFx = aForce[0];
	rData.dFz = aForce[2];
	rData.dMy = aForce[4];
	rData.dMz = aForce[5];
}

void CDgnPlateGirder::SetPgbsBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, int iLcomType,
	double* aForceGd, double* aForce, T_PGBS_BASE& rData)
{
	rData.bChk = bChk;
	rData.ElemK = ElemK.first;
	rData.nPosi = nPosi;
	rData.ChkGirder.LcomK = iDgnLcomNo;
	rData.ChkGirder.OrgLcomK = iOrgLcomNo;
	rData.ChkGirder.LcomType = iLcomType;
	rData.ChkGirder.iMax = iMaxMinType;
	memcpy(rData.ChkGirder.aBeforeForce, aForceGd, sizeof(rData.ChkGirder.aBeforeForce));
	memcpy(rData.ChkGirder.aForce, aForce, sizeof(rData.ChkGirder.aForce));
	//
	rData.ChkSlab.LcomK = iDgnLcomNo;
	rData.ChkSlab.OrgLcomK = iOrgLcomNo;
	rData.ChkSlab.LcomType = iLcomType;
	rData.ChkSlab.iMax = iMaxMinType;
	memcpy(rData.ChkSlab.aForce, aForce, sizeof(rData.ChkSlab.aForce));
	//
	rData.ChkBar.LcomK = iDgnLcomNo;
	rData.ChkBar.OrgLcomK = iOrgLcomNo;
	rData.ChkBar.LcomType = iLcomType;
	rData.ChkBar.iMax = iMaxMinType;
	memcpy(rData.ChkBar.aForce, aForce, sizeof(rData.ChkBar.aForce));

}

void CDgnPlateGirder::SetPlssBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, int iLcomType, double* aForceGd, double* aForce, T_PLSS_BASE& rPlssGd, T_PLSS_BASE& rData)
{
	rData.bChk = bChk;
	rData.ElemK = ElemK.first;
	rData.nPosi = nPosi;
	rData.LcomK = iDgnLcomNo;
	rData.OrgLcomK = iOrgLcomNo;
	rData.LcomType = iLcomType;
	rData.iMax = iMaxMinType;

	memcpy(rPlssGd.aForce, aForceGd, sizeof(rPlssGd.aForce));
	memcpy(rData.aForce, aForce, sizeof(rData.aForce));

	rData.dMa_Ed = aForceGd[4];
}

BOOL CDgnPlateGirder::Get_MaterialData(ElemPairK ElemK, double& dfy, double& dfy1, double& dEs, double& dfck, double& dEcm, double& dfsk, double& dErs)
{
	CPG_MATL_ELEM MatlElem;
	if ( !Get_CpgMatlElem(ElemK, MatlElem) ) { return FALSE; }

	// Material
	const T_MATD_D& MatdD = MatlElem.MatdD;
	// Steel.
	dfy = MatdD.Data1.Design.S_Fy1;
	dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfu = MatdD.Data1.Design.S_Fu;
	dEs = MatdD.Data1.Analysis.Elast;
	// Concrete
	dfck = MatlElem.dfck;
	double dfcm = MatlElem.dfcm;
	double dfctm = MatlElem.dfctm;
	dEcm = MatlElem.dEcm;
	// Rebar
	dfsk = MatlElem.MatdD.MainRebarData.B_fy;
	dErs = MatdD.MainRebarData.B_Elast; //dEs; // Steel로..

	return TRUE;
}

BOOL CDgnPlateGirder::Get_LineEndStress4LineDgnBase(const CArray<T_GLINE_DGN_B, T_GLINE_DGN_B&>& aDgnBaseD, double& dStr1, double& dStr2)
{
	int iDgnLineSize = aDgnBaseD.GetSize();
	if ( iDgnLineSize <= 0 ) { return FALSE; }

	dStr1 = aDgnBaseD[0].dStr1; // array 시작.
	dStr2 = aDgnBaseD[iDgnLineSize - 1].dStr2; // array 끝.

	return TRUE;
}

BOOL CDgnPlateGirder::Get_ForceLineGS(double dfy1, double dfy2, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLine, double& dTotalForce)
{
	dTotalForce = 0.0;
	int nLineSize = aLine.GetSize();
	for ( int i = 0; i < nLineSize; i++ )
	{
		T_GSEC_LINE LineD = aLine[i];
		int v1 = LineD.v1;
		int v2 = LineD.v2;

		T_GSEC_VERTEX vtx1 = PolyD.aVertex[v1];
		T_GSEC_VERTEX vtx2 = PolyD.aVertex[v2];
		double dx1 = vtx1.dX;
		double dy1 = vtx1.dY;
		double dx2 = vtx2.dX;
		double dy2 = vtx2.dY;
		double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
		double dt = LineD.dThik;
		double dAl = dbi * dt;
		double dfy = Get_fy4Thick(dfy1, dfy2, dt);
		double dfyd = dfy / m_dGamma_M0;
		double dForcei = dAl * dfyd;
		dTotalForce += dForcei;
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_TopBotForceLineGS(double dfy1, double dfy2, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLine, const CArray<double, double&>& aTopThick,
	double& dTopForce, double& dBotForce)
{
	dTopForce = dBotForce = 0.0;
	int nLineSize = aLine.GetSize();
	for ( int i = 0; i < nLineSize; i++ )
	{
		T_GSEC_LINE LineD = aLine[i];
		int v1 = LineD.v1;
		int v2 = LineD.v2;

		T_GSEC_VERTEX vtx1 = PolyD.aVertex[v1];
		T_GSEC_VERTEX vtx2 = PolyD.aVertex[v2];
		double dx1 = vtx1.dX;
		double dy1 = vtx1.dY;
		double dx2 = vtx2.dX;
		double dy2 = vtx2.dY;
		double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
		double dt = LineD.dThik;
		double dt_top = aTopThick[i];
		double dt_bot = dt - dt_top;
		double dAl_top = dbi * dt_top;
		double dAl_bot = dbi * dt_bot;
		double dfy = Get_fy4Thick(dfy1, dfy2, dt);
		double dfyd = dfy / m_dGamma_M0;
		double dForceTopi = dAl_top * dfyd;
		double dForceBoti = dAl_bot * dfyd;
		dTopForce += dForceTopi;
		dBotForce += dForceBoti;
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_PartLineId(int nPartId, const T_GLINE_DGN_D& GLineDgnD, CArray<int, int&>& aWebLineId)
{
	int i = 0, j = 0, k = 0;
	int iPartLineSize = GLineDgnD.aPartLine.GetSize();
	for ( i = 0; i < iPartLineSize; i++ )
	{
		T_GLINE_PART_DGN GLinePartD = GLineDgnD.aPartLine[i];
		int nPartType = GLinePartD.nPartType;
		if ( nPartType == nPartId )
		{
			int iOutSize = GLinePartD.aOutstand.GetSize();
			for ( j = 0; j < iOutSize; j++ )
			{
				T_GLINE_DST GLineDstD = GLinePartD.aOutstand[j];
				int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
				for ( k = 0; k < iLineDgnSize; k++ )
				{
					T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[k];
					aWebLineId.Append(GLineBaseD.aLineIx);
				}
				int iStfnLineSize = GLineDstD.aStiffener.GetSize();
				for ( k = 0; k < iStfnLineSize; k++ )
				{
					T_GLINE_STIFFENER StfnLineD = GLineDstD.aStiffener[j];
					aWebLineId.Append(StfnLineD.aLineIx);
				}
			}
			int iIntSize = GLinePartD.aInternal.GetSize();
			for ( j = 0; j < iIntSize; j++ )
			{
				T_GLINE_DST GLineDstD = GLinePartD.aInternal[j];
				int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
				for ( k = 0; k < iLineDgnSize; k++ )
				{
					T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[k];
					aWebLineId.Append(GLineBaseD.aLineIx);
				}
				int iStfnLineSize = GLineDstD.aStiffener.GetSize();
				for ( k = 0; k < iStfnLineSize; k++ )
				{
					T_GLINE_STIFFENER StfnLineD = GLineDstD.aStiffener[k];
					aWebLineId.Append(StfnLineD.aLineIx);
				}
			}
		}
	}

	return TRUE;
}

double CDgnPlateGirder::Get_ShearStress(double dV, double dQ, double dIyy, double dbw)
{
	double dtau = dIyy * dbw == 0.0 ? 0.0 : fabs(dV)*dQ / (dIyy*dbw);
	return dtau;
}

double CDgnPlateGirder::Get_Q_FirstMomentOfGirderCentroid(double* aSize, double dZcen_g, double dZcen_c)
{
	double dQ = 0.0;
	if ( dZcen_c < dZcen_g ) return dQ;
	double dH = 0., dTw = 0., dB1 = 0., dTF1 = 0., dB2 = 0., dTF2 = 0., dHw = 0.;
	Get_SectionSizeOfSect_I(aSize, dH, dTw, dB1, dTF1, dB2, dTF2, dHw);
	if ( dZcen_g > dTF2 )
	{
		double dQ_flg_b = (dTF2*dB2)*(dZcen_c - 0.5*dTF2);
		double dQ_web = (dZcen_g - dTF2)*dTw*(dZcen_c - (dTF2 + 0.5*(dZcen_g - dTF2)));
		dQ = dQ_flg_b + dQ_web;
	}
	else
	{
		double dQ_flg_b = (dZcen_g*dB2)*(dZcen_c - 0.5*dZcen_g);
		dQ = dQ_flg_b;
	}

	return dQ;
}

BOOL CDgnPlateGirder::Get_ThicknessOfWeb4GenSect(const T_GLINE_DGN_D& GLineDgn, double& dbw, double& dAweb)
{
	dbw = 0.0, dAweb = 0.0;
	int iPartSize = GLineDgn.aPartLine.GetSize();
	int i = 0, j = 0, k = 0;
	for ( i = 0; i < iPartSize; i++ )
	{
		T_GLINE_PART_DGN PartLineD = GLineDgn.aPartLine[i];
		if ( PartLineD.nPartType == 2 ) // 2=Web
		{
			dbw += PartLineD.dt_in;
			//////////////////////////////////////////////////////////////////////////      

			int iOutSize = PartLineD.aOutstand.GetSize();
			for ( j = 0; j < iOutSize; j++ )
			{
				T_GLINE_DST GLineDstD = PartLineD.aOutstand[j];
				int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
				for ( k = 0; k < iLineDgnSize; k++ )
				{
					T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[k];
					double dAwi = GLineBaseD.db * GLineBaseD.dt;
					dAweb += dAwi;
				}
			}
			int iIntSize = PartLineD.aInternal.GetSize();
			for ( j = 0; j < iIntSize; j++ )
			{
				T_GLINE_DST GLineDstD = PartLineD.aInternal[j];
				int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
				for ( k = 0; k < iLineDgnSize; k++ )
				{
					T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[k];
					double dAwi = GLineBaseD.db * GLineBaseD.dt;
					dAweb += dAwi;
				}
			}
		}
	}

	return TRUE;
}


BOOL CDgnPlateGirder::Get_ThicknessOfFlange4GenSect(const T_GLINE_DGN_D& GLineDgn, double& dt_top, double& dt_bot)
{
	dt_top = dt_bot = 0.0;

	auto _l_Thickness = [&](const int& nPart, const T_GLINE_DGN_D& GLineD) -> double
	{
		if ( nPart < 0 || nPart>1 )
		{
			ASSERT(0);
			return 0.0;
		}

		double dt_min = DBL_MAX;
		for ( int i = 0; i < GLineDgn.aPartLine.GetSize(); i++ )
		{
			T_GLINE_PART_DGN PartLineD = GLineDgn.aPartLine[i];
			if ( PartLineD.nPartType == nPart ) // 0=Top, 1=Bottom
			{
				for ( int j = 0; j < PartLineD.aOutstand.GetSize(); j++ )
				{
					const T_GLINE_DST& GLineDstD = PartLineD.aOutstand[j];
					for ( int k = 0; k < GLineDstD.aLineDgn.GetSize(); k++ )
					{
						const T_GLINE_BASE& GLineBaseD = GLineDstD.aLineDgn[k];
						if ( LT(GLineBaseD.dt, dt_min) )
						{
							dt_min = GLineBaseD.dt;
						}
					}
				}
				for ( int j = 0; j < PartLineD.aInternal.GetSize(); j++ )
				{
					const T_GLINE_DST& GLineDstD = PartLineD.aInternal[j];
					for ( int k = 0; k < GLineDstD.aLineDgn.GetSize(); k++ )
					{
						const T_GLINE_BASE& GLineBaseD = GLineDstD.aLineDgn[k];
						if ( LT(GLineBaseD.dt, dt_min) )
						{
							dt_min = GLineBaseD.dt;
						}
					}
				}
			}
		}
		return dt_min;
	};

	dt_top = _l_Thickness(0, GLineDgn);
	dt_bot = _l_Thickness(1, GLineDgn);

	return TRUE;
}

BOOL CDgnPlateGirder::IsValueZero(double dValue)
{
	BOOL bIsZero = FALSE;
	if ( fabs(dValue) < m_dZero ) bIsZero = TRUE;
	return bIsZero;
}

BOOL CDgnPlateGirder::Get_SectPropOfDLinePart(CArray<T_DLINE_PART_D, T_DLINE_PART_D&>& aDLinePart, double& dArea, double& dt)
{
	int i = 0, j = 0;
	int nPartSize = aDLinePart.GetSize();
	double dt_out = 0.0;
	double dt_int = 0.0;
	for ( i = 0; i < nPartSize; ++i )
	{
		double dAout = 0.0;
		double dt_out_i = 0.0;
		Get_SectPropOfLineLBD(aDLinePart[i].aOutstand, dAout, dt_out_i);
		//
		double dAint = 0.0;
		double dt_int_i = 0.0;
		Get_SectPropOfLineLBD(aDLinePart[i].aInternal, dAint, dt_int_i);
		dArea += dAout + dAint;
		double dt_pi = 0.0;
		if ( dt_out_i > 0.0 && dt_int_i > 0.0 ) dt_pi = min(dt_out_i, dt_int_i);
		else if ( dt_out_i > 0.0 )            dt_pi = dt_out_i;
		else if ( dt_int_i > 0.0 )            dt_pi = dt_int_i;
		else ASSERT(0);
		if ( i == 0 ) dt = dt_pi;
		else     dt = min(dt, dt_pi);
	}
	return TRUE;
}

BOOL CDgnPlateGirder::Get_SectPropOfLineLBD(CArray<T_DGNLINE_LBD, T_DGNLINE_LBD&>& aDLineLBD, double& dArea, double& dt)
{
	int i = 0;
	int nLineLBDSize = aDLineLBD.GetSize();
	for ( i = 0; i < nLineLBDSize; ++i )
	{
		double dArea_i = aDLineLBD[i].dAp;
		double dti = aDLineLBD[i].dt;
		dArea += dArea_i;
		if ( i == 0 ) dt = dti;
		else     dt = min(dt, dti);
	}
	return TRUE;
}

void CDgnPlateGirder::GetPureSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData)
{
	if ( bPosI ) { rData = SectElemD.SectPosi[0].PureSect; }
	else { rData = SectElemD.SectPosi[1].PureSect; }
}

void CDgnPlateGirder::GetNonComSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData)
{
	if ( bPosI ) { rData = SectElemD.SectPosi[0].NonConSect; }
	else { rData = SectElemD.SectPosi[1].NonConSect; }
}

void CDgnPlateGirder::GetRebarSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData) const
{
	if ( bPosI ) { rData = SectElemD.SectPosi[0].RebarSect; }
	else { rData = SectElemD.SectPosi[1].RebarSect; }
}

double CDgnPlateGirder::GetPlasticNA()
{
	return m_dPlasticNA;
}

void CDgnPlateGirder::SetPlasticNA(const double& dPlasticNA)
{
	m_dPlasticNA = dPlasticNA;
}

void CDgnPlateGirder::Convert_VerbShearResD(const T_VERTICAL_SHAER_RES_UNIT_D& InD, T_PRVS_BASE_UNIT& rData)
{
	rData.dVpl_Rd = InD.dVpl_Rd;
	rData.dVbw_Rd = InD.dVbw_Rd;
	rData.dVbf_Rd = InD.dVbf_Rd;
	rData.dIsl_org = InD.dIsl_org;
	rData.dIsl = InD.dIsl;
	rData.dXw = InD.dXw;
	rData.dMax_Bar_Lamda_w = InD.dMax_Bar_Lamda_w;
	rData.dc = InD.dc;
	rData.dbf = InD.dbf;
	rData.dtf = InD.dtf;
	rData.dVb_Rd_org = InD.dVb_Rd_org;
	rData.dVb_Rd_Lim = InD.dVb_Rd_Lim;
}

BOOL CDgnPlateGirder::IsPositiveMy(double dTopStress, double dBotStress)
{
	BOOL bPositiveMy = TRUE;
	double dStressVal = dTopStress * dBotStress;
	if ( dStressVal < 0.0 )
	{
		if ( dTopStress < 0.0 ) bPositiveMy = TRUE;
		else if ( dBotStress < 0.0 ) bPositiveMy = FALSE;
	}
	else
	{
		if ( dTopStress > dBotStress ) bPositiveMy = FALSE;
		else                       bPositiveMy = TRUE;
	}

	return bPositiveMy;
}

double CDgnPlateGirder::GetOutStandFlgLineLengD(BOOL bTop, BOOL bLeft, bool bPosiI, const T_SECT_D& crSectD)
{
	double dOutStandLength = 0.0;
	int nSectType = m_pDgnSectUtil->Get_RealSectType(&crSectD);
	switch ( nSectType )
	{
	case D_SECT_TYPE_COMPO_I:
		{
			const double dtw = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			if ( bTop )
			{
				dOutStandLength = bLeft ? m_pDgnSectUtil->Get_Size_bft_l(&crSectD, bPosiI) - dtw * 0.5 : m_pDgnSectUtil->Get_Size_bft_r(&crSectD, bPosiI) - dtw * 0.5;
			}
			else
			{
				dOutStandLength = bLeft ? m_pDgnSectUtil->Get_Size_bfb_l(&crSectD, bPosiI) - dtw * 0.5 : m_pDgnSectUtil->Get_Size_bfb_r(&crSectD, bPosiI) - dtw * 0.5;
			}
			break;
		}
	case D_SECT_TYPE_COMPO_B:
		{
			const double dtwL = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			const double dtwR = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
			if ( bTop )
			{
				dOutStandLength = bLeft ? m_pDgnSectUtil->Get_Size_bft_l(&crSectD, bPosiI) - dtwL * 0.5 : m_pDgnSectUtil->Get_Size_bft_r(&crSectD, bPosiI) - dtwR; ///MUSTCHECK : right part
			}
			else
			{
				dOutStandLength = bLeft ? m_pDgnSectUtil->Get_Size_bfb_l(&crSectD, bPosiI) - dtwL * 0.5 : m_pDgnSectUtil->Get_Size_bfb_r(&crSectD, bPosiI) - dtwR;
			}
			break;
		}
	case D_SECT_TYPE_COMPO_TUB:
		{
			const double dtwL = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			const double dtwR = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
			if ( bTop )
			{
				dOutStandLength = bLeft ? m_pDgnSectUtil->Get_Size_bft_ll(&crSectD, bPosiI) - dtwL : m_pDgnSectUtil->Get_Size_bft_rr(&crSectD, bPosiI) - dtwR;
			}
			else
			{
				dOutStandLength = bLeft ? m_pDgnSectUtil->Get_Size_bfb_l(&crSectD, bPosiI) - dtwL : m_pDgnSectUtil->Get_Size_bfb_r(&crSectD, bPosiI) - dtwR;
			}
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}

	return dOutStandLength;
}

BOOL CDgnPlateGirder::GetCentFlgLinePosD(bool bPosiI, BOOL bTop, BOOL bLeft, const T_SECT_D& crSectD, const T_SECT_STIFFNESS& crStiffD,
	double& dy1, double& dz1, double& dy2, double& dz2)
{
	dy1 = dy2 = dz1 = dz2 = 0.0;
	double dOutFlgLength = GetOutStandFlgLineLengD(bTop, bLeft, bPosiI, crSectD);

	if ( bTop )
	{
		dy1 = bLeft ? crStiffD.dx1 : crStiffD.dx2;
		dz1 = bLeft ? crStiffD.dy1 : crStiffD.dy2;
		dy2 = bLeft ? crStiffD.dx1 + dOutFlgLength : crStiffD.dx2 - dOutFlgLength;
		dz2 = dz1;
	}
	else
	{
		dy1 = bLeft ? crStiffD.dx4 : crStiffD.dx3;
		dz1 = bLeft ? crStiffD.dy4 : crStiffD.dy3;
		dy2 = bLeft ? crStiffD.dx4 + dOutFlgLength : crStiffD.dx3 - dOutFlgLength;
		dz2 = dz1;
	}

	return TRUE;
}

BOOL CDgnPlateGirder::GetCentWebLinePosD(bool bPosiI, BOOL bLeft, const T_SECT_D& crSectD, const T_SECT_STIFFNESS& crStiffD,
	double& dy1, double& dz1, double& dy2, double& dz2)
{
	dy1 = dy2 = dz1 = dz2 = 0.0;
	const CDgnSectUtil::EN_WEB_THK enWebThk = bLeft ? CDgnSectUtil::EN_WEB_THK_LEFT : CDgnSectUtil::EN_WEB_THK_RIGHT;
	double dtw = m_pDgnSectUtil->Get_Size_tw(&crSectD, enWebThk, bPosiI);
	double dtft = m_pDgnSectUtil->Get_Size_tft(&crSectD, bPosiI);
	double dtfb = m_pDgnSectUtil->Get_Size_tfb(&crSectD, bPosiI);
	double dOutFlgLength_T = GetOutStandFlgLineLengD(TRUE, bLeft, bPosiI, crSectD);
	double dOutFlgLength_B = GetOutStandFlgLineLengD(FALSE, bLeft, bPosiI, crSectD);

	if ( bLeft )
	{
		dy1 = crStiffD.dx1 + dOutFlgLength_T + 0.5 *dtw;
		dz1 = crStiffD.dy1 - dtft;

		dy2 = crStiffD.dx4 + dOutFlgLength_B + 0.5 *dtw;
		dz2 = crStiffD.dy4 + dtfb;
	}
	else
	{
		dy1 = crStiffD.dx2 - dOutFlgLength_T - 0.5 *dtw;
		dz1 = crStiffD.dy1 - dtft;

		dy2 = crStiffD.dx3 - dOutFlgLength_B - 0.5 *dtw;
		dz2 = crStiffD.dy4 + dtfb;
	}

	return TRUE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Detail Print
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CDgnPlateGirder::Execute_Detail_SteelPlateGirder(ArrElemPairKey& arElemK, BOOL* bPgdChk)
{
	int iElementNum = arElemK.GetSize();
	if ( iElementNum == 0 ) { return FALSE; }

	m_strUnitMM = _T(" mm");
	m_strUnitMM2 = _T(" mm^2");
	m_strUnitMM3 = _T(" mm^3");
	m_strUnitMM4 = _T(" mm^4");
	m_strUnitStress = _T(" N/mm^2");
	m_strUnitForce = _T(" N");
	m_strUnitForce2 = _T(" N/mm");
	m_strUnitMoment = _T(" N.mm");

	//
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
	//

	//m_pAnalysisResult = m_pDoc->m_pPostCtrl->GetAnalysisResult();

	m_strTestDgnFile.Format(_T("%s\\DataForPlateGirder.txt"), CPathUtil::GetOrMakeMIDASTempFolder());
	rptwofstream fout(m_strTestDgnFile, ios::trunc);

	for ( int i = 0; i < iElementNum; i++ )
	{
		auto ElemK = arElemK[i];
		Detail_Header(ElemK);

		for ( int j = 0; j < 2; j++ )
		{
			UINT nPosi = (j == 0 ? 1 : 2);
			if ( m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, nPosi) )
			{
				BOOL bPosiI = j == 0 ? TRUE : FALSE;
				Detail_SectionProperty_Material(ElemK, bPosiI);
				if ( bPgdChk[0] ) { Detail_BendingResistance(ElemK, bPosiI); }
				if ( bPgdChk[1] ) { Detail_VerticalShearResistance(ElemK, bPosiI); }
				if ( bPgdChk[2] ) { Detail_LateralTorsionalBucklingResistance(ElemK, bPosiI); }
				if ( bPgdChk[3] ) { Detail_TransverseForceResistance(ElemK, bPosiI); }
				if ( bPgdChk[4] ) { Detail_LongitudinalShearResistance(ElemK, bPosiI); }
				if ( bPgdChk[5] ) { Detail_FatigueResistance(ElemK, bPosiI); }
			}
		}
	}

	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

	return TRUE;
}

BOOL CDgnPlateGirder::Detail_Header(ElemPairK ElemK)
{
	rptwofstream fout(m_strTestDgnFile, ios::app);

	T_ELEM_D ElemData;
	ElemData.Initialize();
	if (ElemK.second == EN_EL_BEAM)
	{
		m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemData);
	}

	//int iElemType = ElemData.eltyp;
	int iSectNo = ElemData.elpro;
	int iMatlNo = CDBLib::GetMaterialKey4ElemPairK(ElemK);
	if (ElemK.second == EN_EL_VBEAM)
	{
		iSectNo = ElemK.first;
	}

	//CString strLcomNo=_T("");	strLcomNo.Format(_T("%6s"), LcomDesign.DesignLcomNa);
	CString strElemNo = _T("");	strElemNo.Format(_T("%6d"), ElemK.first);
	CString strSectNo = _T("");	strSectNo.Format(_T("%6d"), iSectNo);
	CString strMatlNo = _T("");	strMatlNo.Format(_T("%6d"), iMatlNo);

	fout << endl
		<< _T("======================================================================================")
		<< endl;

	fout << endl
		<< _T("  *. MEMBER NO    = ") << strElemNo << endl
		<< _T("  *. MATERIAL NO  = ") << strMatlNo << endl
		<< _T("  *. SECTION NO   = ") << strSectNo << endl
		<< endl;

	//<<_T("      *. LOADCOMB NO = ")<<strLcomNo<<

	return TRUE;
}

BOOL CDgnPlateGirder::Detail_SectionProperty_Material(ElemPairK ElemK, BOOL bPosiI)
{
	rptwofstream fout(m_strTestDgnFile, ios::app);

	CPG_SECT_ELEM SectElem;
	if ( !Get_CpgSectElem(ElemK, SectElem) ) ASSERT(0);
	//
	CPG_MATL_ELEM MatlElem;
	if ( !Get_CpgMatlElem(ElemK, MatlElem) ) ASSERT(0);

	// 1. Section.  
	T_SECT_D& SectD = SectElem.SectD;
	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTf1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dTf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

	// 2. Material
	const T_MATD_D& MatdD = MatlElem.MatdD;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfck = MatlElem.dfck;
	double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
	double dEcm = MatlElem.dEcm;
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;
	double dfyt = Get_fy4Thick(dfy, dfy1, dTf1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTf2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk / dgam_sr;
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;
	// girder
	T_SECT_STIFFNESS GirderStiffD;
	GirderStiffD.Initialize();
	if ( !bTapered )
		GirderStiffD = SectD.SectBefore.Stiffness;
	else
		GirderStiffD = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;

	double dArea_g = GirderStiffD.Area;
	double dIyy_g = GirderStiffD.Ryy;
	double dIzz_g = GirderStiffD.Rzz;
	double dYbar_g = GirderStiffD.Cym;
	double dZbar_g = GirderStiffD.Czm;

	// Composite section
	T_SECT_STIFFNESS CompoStiffD;
	CompoStiffD.Initialize();
	if ( !bTapered )
		CompoStiffD = SectD.SectAfter.Stiffness;
	else
		CompoStiffD = bPosiI ? SectD.SectAfter.SectI.Stiffness : SectD.SectAfter.SectJ.Stiffness;

	double dArea = CompoStiffD.Area;
	double dIyy = CompoStiffD.Ryy;
	double dIzz = CompoStiffD.Rzz;
	double dYbar = CompoStiffD.Cym;
	double dZbar = CompoStiffD.Czm;
	//
	T_SECT_STIFFNESS RebarSect; RebarSect.Initialize();
	GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

	double dArea_r = RebarSect.Area;
	double dIyy_r = RebarSect.Ryy;
	double dIzz_r = RebarSect.Rzz;
	double dYbar_r = RebarSect.Cym;
	double dZbar_r = RebarSect.Czm;

	CString strPosi = bPosiI ? _T("I") : _T("J");
	CString strPosition;      strPosition.Format(_T("%6s"), strPosi);
	//
	CString strArea_g;	 strArea_g.Format(_T("%16g"), dArea_g);
	CString strIyy_g;	   strIyy_g.Format(_T("%16g"), dIyy_g);
	CString strIzz_g;	   strIzz_g.Format(_T("%16g"), dIzz_g);
	CString strYbar_g;   strYbar_g.Format(_T("%16g"), dYbar_g);
	CString strZbar_g;   strZbar_g.Format(_T("%16g"), dZbar_g);
	//
	CString strArea;	 strArea.Format(_T("%16g"), dArea);
	CString strIyy;	   strIyy.Format(_T("%16g"), dIyy);
	CString strIzz;	   strIzz.Format(_T("%16g"), dIzz);
	CString strYbar;   strYbar.Format(_T("%16g"), dYbar);
	CString strZbar;   strZbar.Format(_T("%16g"), dZbar);
	//
	CString strArea_r;	 strArea_r.Format(_T("%16g"), dArea_r);
	CString strIyy_r;	   strIyy_r.Format(_T("%16g"), dIyy_r);
	CString strIzz_r;	   strIzz_r.Format(_T("%16g"), dIzz_r);
	CString strYbar_r;   strYbar_r.Format(_T("%16g"), dYbar_r);
	CString strZbar_r;   strZbar_r.Format(_T("%16g"), dZbar_r);

	fout << endl
		<< _T("--------------------------------------------------------------------------------------") << endl
		<< _T("  *. POSITION     = ") << strPosition << endl
		<< endl;
	// Section demension
	fout << _T("  *. Section Size") << endl
		<< _T("   -Girder") << endl
		<< _T("     H   = ") << Get_String_Format(dH) << m_strUnitMM << endl
		<< _T("     tw  = ") << Get_String_Format(dTw) << m_strUnitMM << endl
		<< _T("     Hw  = ") << Get_String_Format(dh) << m_strUnitMM << endl
		<< _T("     B1  = ") << Get_String_Format(dB1) << m_strUnitMM << endl
		<< _T("     tf1 = ") << Get_String_Format(dTf1) << m_strUnitMM << endl
		<< _T("     B2  = ") << Get_String_Format(dB2) << m_strUnitMM << endl
		<< _T("     tf2 = ") << Get_String_Format(dTf2) << m_strUnitMM << endl
		<< endl
		<< _T("   -Slab") << endl
		<< _T("     Bc   = ") << Get_String_Format(dBc) << m_strUnitMM << endl
		<< _T("     tc   = ") << Get_String_Format(dtc) << m_strUnitMM << endl
		<< _T("     Hh   = ") << Get_String_Format(dHh) << m_strUnitMM << endl
		<< endl;

	// Gross section
	fout << _T("  *. Section Property(Girder)") << endl
		<< _T("     Area = ") << strArea_g << m_strUnitMM2 << endl
		<< _T("     Iyy  = ") << strIyy_g << m_strUnitMM4 << endl
		<< _T("     Izz  = ") << strIzz_g << m_strUnitMM4 << endl
		<< _T("     Ybar = ") << strYbar_g << m_strUnitMM << endl
		<< _T("     Zbar = ") << strZbar_g << m_strUnitMM << endl
		<< endl;

	// Gross section
	fout << _T("  *. Section Property(Composite)") << endl
		<< _T("     Area = ") << strArea << m_strUnitMM2 << endl
		<< _T("     Iyy  = ") << strIyy << m_strUnitMM4 << endl
		<< _T("     Izz  = ") << strIzz << m_strUnitMM4 << endl
		<< _T("     Ybar = ") << strYbar << m_strUnitMM << endl
		<< _T("     Zbar = ") << strZbar << m_strUnitMM << endl
		<< endl;

	// Rebar
	fout << _T("  *. Section Property(Composite-R)") << endl
		<< _T("     Area = ") << strArea_r << m_strUnitMM2 << endl
		<< _T("     Iyy  = ") << strIyy_r << m_strUnitMM4 << endl
		<< _T("     Izz  = ") << strIzz_r << m_strUnitMM4 << endl
		<< _T("     Ybar = ") << strYbar_r << m_strUnitMM << endl
		<< _T("     Zbar = ") << strZbar_r << m_strUnitMM << endl
		<< endl;

	// Material
	fout << _T("  *. Material") << endl
		<< _T("     fy     = ") << Get_String_Format(dfy) << m_strUnitStress << endl
		<< _T("     fy1    = ") << Get_String_Format(dfy1) << m_strUnitStress << endl
		<< _T("     fy,tf  = ") << Get_String_Format(dfyd_t) << m_strUnitStress << endl
		<< _T("     fy,bf  = ") << Get_String_Format(dfyd_b) << m_strUnitStress << endl
		<< _T("     fy,w   = ") << Get_String_Format(dfyd_w) << m_strUnitStress << endl
		<< _T("     fck    = ") << Get_String_Format(dfck) << m_strUnitStress << endl
		<< _T("     fcd    = ") << Get_String_Format(dfcd) << m_strUnitStress << endl
		<< _T("     fsk    = ") << Get_String_Format(dfsk) << m_strUnitStress << endl
		<< _T("     fsd    = ") << Get_String_Format(dfsd) << m_strUnitStress << endl
		<< _T("     Ecm    = ") << Get_String_Format(dEcm) << m_strUnitStress << endl
		<< endl;


	return TRUE;
}

BOOL CDgnPlateGirder::Detail_BendingResistance(ElemPairK ElemK, BOOL bPosiI)
{
	rptwofstream fout(m_strTestDgnFile, ios::app);

	BOOL bSect = TRUE, bMatl = TRUE;
	CPG_SECT_ELEM SectElem;
	if ( !Get_CpgSectElem(ElemK, SectElem) ) { bSect = FALSE; }

	CPG_MATL_ELEM MatlElem;
	if ( !Get_CpgMatlElem(ElemK, MatlElem) ) { bMatl = FALSE; }

	// 1. Section.  
	T_SECT_D& SectD = SectElem.SectD;
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTf1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dTf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

	// 2. Material
	const T_MATD_D& MatdD = MatlElem.MatdD;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfck = MatlElem.dfck;
	double dgam_M0 = m_dGamma_M0;
	double dgam_c = m_dGamma_C;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;
	double dfyt = Get_fy4Thick(dfy, dfy1, dTf1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTf2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;

	// Load combination
	LPCTSTR aType[] = { _T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN),
		_LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
		_LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN) };

	////////////////////////////////////////////////////////////////////
	BOOL bPositive;
	double dXpl;
	double dXplOverh = 0.;
	double dPlasticNA = 0., dPlasticNAEff = 0.;
	/*
	double dNa_topc=0., dNa_topt=0.;
	double dNa_webc=0., dNa_webt=0.;
	double dNa_botc=0., dNa_bott=0.;
	double dNa_webc1=0., dNa_webc2=0.;
	double dNa_web_Effe=0.;
	double dDistT=0., dDistB=0., dDistW=0.;
	double dDistTc=0., dDistTt=0.;
	double dDistWc=0., dDistWt=0.;
	double dDistBc=0., dDistBt=0.;
	double dDistWc1=0., dDistWc2=0.;
	double dDistW1=0., dDistW2=0.;
	double dCompfsdAsidsi=0., dTensfsdAsidsi=0.;
	double dBeta=0.;
	// elastic
	double dStrStlTop=0., dStrComTop=0., dStrStlBot=0., dStrComBot=0.;
	double dSIGa_top=0., dSIGa_bot=0.;
	double dk_min=0.;
	*/
	/////////////////////////////////////////////////////////////////////  

	for ( int i = 0; i < 2; i++ ) // 0=positive, 1=negative
	{
		int iMaxMin = i == 0 ? 1 : 2;
		T_PGBR_LCOM PgbrLcomD;
		PgbrLcomD.Initialize();
		if ( !m_pAnalysisResult->GetPgbrResultData(ElemK, iMaxMin, PgbrLcomD) ) { continue; }
		//
		T_PGBR_BASE PgbrBase;
		if ( bPosiI ) { PgbrBase = PgbrLcomD.PgbrBase[0]; }
		else { PgbrBase = PgbrLcomD.PgbrBase[1]; }

		if ( !PgbrBase.bChk ) continue;
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		dXpl = PgbrBase.dXpl;
		dXplOverh = PgbrBase.dXplOverh;

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		bPositive = PgbrBase.dMy > -m_dZero ? TRUE : FALSE;
		BOOL bEquiClass2 = PgbrBase.bEquiClass2;
		// Lcom
		CString strLoadComb;
		T_LCOM_D LcomData;
		if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, PgbrBase.OrgLcomK, LcomData) )
		{
			strLoadComb.Format(_T("%s(%s)"), LcomData.LoadCombName, aType[PgbrBase.iMax]);
		}
		else
		{
			strLoadComb = _T("-");
		}
		// Class
		CString strTopFlange; strTopFlange.Format(_T("Class %d"), PgbrBase.iTopClass);
		CString strBotFlange; strBotFlange.Format(_T("Class %d"), PgbrBase.iBotClass);
		CString strWeb; strWeb.Format(_T("Class %d"), PgbrBase.iWebClass);
		CString strSectClass;
		if ( bEquiClass2 )
			strSectClass = _T("Effective Class 2");
		else
			strSectClass.Format(_T("Class %d"), PgbrBase.iSectClass);

		fout << endl
			<< _T("--------------------------------------------------------------------------------------") << endl
			<< endl
			<< _T("  *. Bending Resistance") << endl
			<< endl
			<< _T("  -  Load Combination     = ") << strLoadComb << endl
			<< endl;

		fout << _T("  -  Force") << endl
			<< _T("     Ma,Ed = ") << Get_String_Format(PgbrBase.dMa_Ed) << m_strUnitMoment << endl
			<< _T("     Mc,Ed = ") << Get_String_Format(PgbrBase.dMc_Ed) << m_strUnitMoment << endl
			<< endl;

		fout << _T("  -  Classification of cross sections") << endl
			<< _T("     Top flange    = ") << strTopFlange << endl
			<< _T("     Bottom flange = ") << strBotFlange << endl
			<< _T("     Web           = ") << strWeb << endl
			<< _T("     Section       = ") << strSectClass << endl
			<< endl;

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
		fout << _T("  -  Plastic resistance moment Mpl,Rd") << endl;

		if ( bEquiClass2 )
		{
			fout << _T("     Effective Plastic NA    = ") << Get_String_Format(PgbrBase.dPlasticNAEff) << m_strUnitMM << endl;

		}
		else
		{
			fout << _T("     Plastic NA = ") << Get_String_Format(PgbrBase.dPlasticNA) << m_strUnitMM << endl;
		}


		fout << _T("     Xpl        = ") << Get_String_Format(PgbrBase.dXpl) << m_strUnitMM << endl
			<< _T("     Xpl/h      = ") << Get_String_Format(PgbrBase.dXplOverh) << endl
			<< _T("     Alpha      = ") << Get_String_Format(PgbrBase.dAlpha) << endl
			<< endl;

		if ( !bEquiClass2 )
		{
			// Positive일 경우는 강도 계산할 때 압축콘크리트 중심(Xpl*0.5)에서 계산한다.
			if ( bPositive )
			{
				if ( PgbrBase.dPlasticNA > dH )
				{
					if ( dXpl > dtc )
					{
						//dNc_f = dBc*dtc*dAlpha*dfcd;
						//fout<<_T("     Nc,f       = ")<<_T("Bc*tc*Alpha*fcd")<<Get_String_Format(PgbrBase.dNc_f)<<m_strUnitForce<<endl;            
					}
					else if ( dXpl < dtc )
					{
						//dNc_f = dBc*dXpl*dAlpha*dfcd;
						//fout<<_T("     Nc,f       = ")<<_T("Bc*Xpl*Alpha*fcd")<<Get_String_Format(PgbrBase.dNc_f)<<m_strUnitForce<<endl;
					}
					// dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW;
					//
					fout << _T("   - Mpl,Rd") << endl
						<< _T("     Dist.TF   = ") << Get_String_Format(PgbrBase.dDistT) << m_strUnitMM << _T("  (from Concrete Center)") << endl
						<< _T("     Dist.W    = ") << Get_String_Format(PgbrBase.dDistW) << m_strUnitMM << endl
						<< _T("     Dist.BF   = ") << Get_String_Format(PgbrBase.dDistB) << m_strUnitMM << endl
						<< _T("     Na,top    = ") << Get_String_Format(PgbrBase.dNa_top) << m_strUnitForce << endl
						<< _T("     Na,web    = ") << Get_String_Format(PgbrBase.dNa_web) << m_strUnitForce << endl
						<< _T("     Na,bot    = ") << Get_String_Format(PgbrBase.dNa_bot) << m_strUnitForce << endl
						<< _T("     Mpl,Rd    = ") << _T("Na,top*Dist.TF + Na,web*Dist.W + Na,bot*Dist.BF = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
						<< endl;
				}
				else
				{
					if ( PgbrBase.dPlasticNA > dH - dTf1 ) // Top에 위치할 때 
					{
						//dTfc = dH - dPlasticNA;
						//dTft = dPlasticNA - (dH-dTF1);
						//dNa_topc = dB1*dTfc*dfyd_t;
						//dNa_topt = dB1*dTft*dfyd_t;
						//dDistTc = 0.5*dtc + dHh + 0.5*dTfc;
						//dDistTt = 0.5*dtc + dHh + dTfc + 0.5*dTft;
						//dDistB = dHt - 0.5*dtc - 0.5*dTF2;
						//dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dh;
						//
						//dMpl_Rd = -dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web*dDistW;
						fout << _T("   - Mpl,Rd") << endl
							<< _T("     Dist.TFc  = ") << Get_String_Format(PgbrBase.dDistTc) << m_strUnitMM << _T("  (from Concrete Center)") << endl
							<< _T("     Dist.TFt  = ") << Get_String_Format(PgbrBase.dDistTt) << m_strUnitMM << endl
							<< _T("     Dist.W    = ") << Get_String_Format(PgbrBase.dDistW) << m_strUnitMM << endl
							<< _T("     Dist.BF   = ") << Get_String_Format(PgbrBase.dDistB) << m_strUnitMM << endl
							<< _T("     Na,top,c  = ") << Get_String_Format(PgbrBase.dNa_topc) << m_strUnitForce << endl
							<< _T("     Na,top,t  = ") << Get_String_Format(PgbrBase.dNa_topt) << m_strUnitForce << endl
							<< _T("     Na,web    = ") << Get_String_Format(PgbrBase.dNa_web) << m_strUnitForce << endl
							<< _T("     Na,bot    = ") << Get_String_Format(PgbrBase.dNa_bot) << m_strUnitForce << endl
							<< _T("     Mpl,Rd    = ") << _T("-Na,top,c*Dist.TFc + Na,top,t*Dist.TFt + Na,web*Dist.W + Na,bot*Dist.BF = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
							<< endl;
					}
					else if ( PgbrBase.dPlasticNA<dH - dTf1 && PgbrBase.dPlasticNA>dTf2 ) // Web에 위치할 때 
					{
						//double dWc = (dH-dTF1) - dPlasticNA;
						//double dWt = dPlasticNA - dTF2;
						//dNa_webc = dWc*dTw*dfyd_w;
						//dNa_webt = dWt*dTw*dfyd_w;
						//dDistWc = 0.5*dtc + dHh + dTF1 + 0.5*dWc;
						//dDistWt = 0.5*dtc + dHh + dTF1 + dWc + 0.5*dWt;
						//dDistT = dHt - 0.5*dtc - (dH-0.5*dTF1);
						//dDistB = dHt - 0.5*dtc - 0.5*dTF2;        
						//
						//dMpl_Rd = -dNa_top*dDistT-dNa_webc*dDistWc+dNa_webt*dDistWt+dNa_bot*dDistB;
						fout << _T("   - Mpl,Rd") << endl
							<< _T("     Dist.TF   = ") << Get_String_Format(PgbrBase.dDistT) << m_strUnitMM << _T("  (from Concrete Center)") << endl
							<< _T("     Dist.Wc   = ") << Get_String_Format(PgbrBase.dDistWc) << m_strUnitMM << endl
							<< _T("     Dist.Wt   = ") << Get_String_Format(PgbrBase.dDistWt) << m_strUnitMM << endl
							<< _T("     Dist.BF   = ") << Get_String_Format(PgbrBase.dDistB) << m_strUnitMM << endl
							<< _T("     Na,top    = ") << Get_String_Format(PgbrBase.dNa_top) << m_strUnitForce << endl
							<< _T("     Na,web,c  = ") << Get_String_Format(PgbrBase.dNa_webc) << m_strUnitForce << endl
							<< _T("     Na,web,t  = ") << Get_String_Format(PgbrBase.dNa_webt) << m_strUnitForce << endl
							<< _T("     Na,bot    = ") << Get_String_Format(PgbrBase.dNa_bot) << m_strUnitForce << endl
							<< _T("     Mpl,Rd    = ") << _T("-Na,top*Dist.TF - Na,web,c*Dist.Wc + Na,web,t*Dist.Wt + Na,bot*Dist.BF = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
							<< endl;
					}
					else if ( PgbrBase.dPlasticNA < dTf2 ) // Bot에 위치할 때.
					{
						//double dBfc = dTF2 - dPlasticNA;
						//double dBft = dPlasticNA;
						//dNa_botc = dB2*dBfc*dfyd_b;
						//dNa_bott = dB2*dBft*dfyd_b;
						//dDistBc = 0.5*dtc + dHh + dTF1 + dh + 0.5*dBfc;
						//dDistBt = 0.5*dtc + dHh + dTF1 + dh + dBfc + 0.5*dBft;
						//dDistT= dHt - 0.5*dtc - (dH-0.5*dTF1);
						//dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dh;
						//
						//dMpl_Rd = -dNa_top*dDistT  - dNa_web*dDistW - dNa_botc*dDistBc + dNa_bott*dDistBt;
						fout << _T("   - Mpl,Rd") << endl
							<< _T("     Dist.TF   = ") << Get_String_Format(PgbrBase.dDistT) << m_strUnitMM << _T("  (from Concrete Center)") << endl
							<< _T("     Dist.W    = ") << Get_String_Format(PgbrBase.dDistW) << m_strUnitMM << endl
							<< _T("     Dist.BFc  = ") << Get_String_Format(PgbrBase.dDistBc) << m_strUnitMM << endl
							<< _T("     Dist.BFt  = ") << Get_String_Format(PgbrBase.dDistBt) << m_strUnitMM << endl
							<< _T("     Na,top    = ") << Get_String_Format(PgbrBase.dNa_top) << m_strUnitForce << endl
							<< _T("     Na,web    = ") << Get_String_Format(PgbrBase.dNa_web) << m_strUnitForce << endl
							<< _T("     Na,bot,c  = ") << Get_String_Format(PgbrBase.dNa_botc) << m_strUnitForce << endl
							<< _T("     Na,bot,t  = ") << Get_String_Format(PgbrBase.dNa_bott) << m_strUnitForce << endl
							<< _T("     Mpl,Rd    = ") << _T("-Na,top*Dist.TF - Na,web*Dist.W - Na,bot,c*Dist.BFc + Na,bot,t*Dist.BFt = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
							<< endl;
					}
				}
			}
			// Negative일 경우는 강도 계산할 때 PlasticNA가 기준. 
			else // Negative moment.
			{
				// Rebar 계산.
				/*
				dCompfsdAsidsi=0.0;
				dTensfsdAsidsi=0.0;
				double ddsi=0.0;
				double dAsi=0.0;
				double dNAdsi=0.0;
				double dGap = (dH+dHh+0.5*dtc) - dPlasticNA;
				for(i=0; i<arRbarPosi.GetSize(); i++)
				{
				_DGN_RBAR_CRC Rbar;
				Rbar = arRbarPosi[i];
				// PlasticNA에서 떨어진 거리.
				dNAdsi = dGap + Rbar.dz;
				dAsi = Rbar.dArea;
				if(dNAdsi<0.0) // 압축.
				{
				dCompfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
				}
				else
				{
				dTensfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
				}
				}
				*/

				fout << _T("   - Force in the reinforcement") << endl
					<< _T("     SUM(fsd*As*ds),C  = ") << Get_String_Format(PgbrBase.dCompfsdAsidsi) << m_strUnitMoment << endl
					<< _T("     SUM(fsd*As*ds),T  = ") << Get_String_Format(PgbrBase.dTensfsdAsidsi) << m_strUnitMoment << endl
					<< endl;

				if ( PgbrBase.dPlasticNA > dH )
				{
					//dDistT = dPlasticNA - (dH-0.5*dTF1);
					//dDistW = dPlasticNA - (0.5*dh+dTF2);
					//dDistB = dPlasticNA - 0.5*dTF2;      
					//
					//dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW + dCompfsdAsidsi + dTensfsdAsidsi;
					fout << _T("   - Mpl,Rd") << endl
						<< _T("     Dist.TF   = ") << Get_String_Format(PgbrBase.dDistT) << m_strUnitMM << _T("  (from Plastic NA)") << endl
						<< _T("     Dist.W    = ") << Get_String_Format(PgbrBase.dDistW) << m_strUnitMM << endl
						<< _T("     Dist.BF   = ") << Get_String_Format(PgbrBase.dDistB) << m_strUnitMM << endl
						<< _T("     Na,top    = ") << Get_String_Format(PgbrBase.dNa_top) << m_strUnitForce << endl
						<< _T("     Na,web    = ") << Get_String_Format(PgbrBase.dNa_web) << m_strUnitForce << endl
						<< _T("     Na,bot    = ") << Get_String_Format(PgbrBase.dNa_bot) << m_strUnitForce << endl
						<< _T("     Mpl,Rd    = ") << _T("Na,top*Dist.TF + Na,web*Dist.W + Na,bot*Dist.BF + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
						<< endl;
				}
				else
				{
					if ( PgbrBase.dPlasticNA > dH - dTf1 ) // Top에 위치할 때 
					{
						//double dTfc = dPlasticNA - (dH-dTF1);
						//double dTft = dH - dPlasticNA;
						//dNa_topc = dB1*dTfc*dfyd_t;
						//dNa_topt = dB1*dTft*dfyd_t;
						//dDistTc  = 0.5*dTfc;
						//dDistTt  = 0.5*dTft;
						//dDistB   = dPlasticNA - 0.5*dTF2;
						//dDistW   = dPlasticNA - (dTF2+0.5*dh);        
						//
						//dMpl_Rd = dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web*dDistW + dCompfsdAsidsi + dTensfsdAsidsi;
						fout << _T("   - Mpl,Rd") << endl
							<< _T("     Dist.TFc   = ") << Get_String_Format(PgbrBase.dDistTc) << m_strUnitMM << _T("  (from Plastic NA)") << endl
							<< _T("     Dist.TFt   = ") << Get_String_Format(PgbrBase.dDistTt) << m_strUnitMM << endl
							<< _T("     Dist.W    = ") << Get_String_Format(PgbrBase.dDistW) << m_strUnitMM << endl
							<< _T("     Dist.BF   = ") << Get_String_Format(PgbrBase.dDistB) << m_strUnitMM << endl
							<< _T("     Na,top,c  = ") << Get_String_Format(PgbrBase.dNa_topc) << m_strUnitForce << endl
							<< _T("     Na,top,t  = ") << Get_String_Format(PgbrBase.dNa_topt) << m_strUnitForce << endl
							<< _T("     Na,web    = ") << Get_String_Format(PgbrBase.dNa_web) << m_strUnitForce << endl
							<< _T("     Na,bot    = ") << Get_String_Format(PgbrBase.dNa_bot) << m_strUnitForce << endl
							<< _T("     Mpl,Rd    = ") << _T("Na,top,t*Dist.TFt + Na,top,c*Dist.TFc + Na,web*Dist.W + Na,bot*Dist.BF + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
							<< endl;
					}
					else if ( PgbrBase.dPlasticNA<dH - dTf1 && PgbrBase.dPlasticNA>dTf2 ) // Web에 위치할 때 
					{
						//double dWc = dPlasticNA - dTF2; 
						//double dWt = (dH-dTF1) - dPlasticNA;
						//dNa_webc = dWc*dTw*dfyd_w;
						//dNa_webt = dWt*dTw*dfyd_w;
						//dDistWc = 0.5*dWc;
						//dDistWt = 0.5*dWt;
						//dDistT = (dH-0.5*dTF1) - dPlasticNA;
						//dDistB = dPlasticNA - 0.5*dTF2;        
						//
						//dMpl_Rd = dNa_top*dDistT+dNa_webc*dDistWc+dNa_webt*dDistWt+dNa_bot*dDistB + dCompfsdAsidsi + dTensfsdAsidsi;
						fout << _T("   - Mpl,Rd") << endl
							<< _T("     Dist.TF   = ") << Get_String_Format(PgbrBase.dDistT) << m_strUnitMM << _T("  (from Plastic NA)") << endl
							<< _T("     Dist.Wc   = ") << Get_String_Format(PgbrBase.dDistWc) << m_strUnitMM << endl
							<< _T("     Dist.Wt   = ") << Get_String_Format(PgbrBase.dDistWt) << m_strUnitMM << endl
							<< _T("     Dist.BF   = ") << Get_String_Format(PgbrBase.dDistB) << m_strUnitMM << endl
							<< _T("     Na,top    = ") << Get_String_Format(PgbrBase.dNa_top) << m_strUnitForce << endl
							<< _T("     Na,web,c  = ") << Get_String_Format(PgbrBase.dNa_webc) << m_strUnitForce << endl
							<< _T("     Na,web,t  = ") << Get_String_Format(PgbrBase.dNa_webt) << m_strUnitForce << endl
							<< _T("     Na,bot    = ") << Get_String_Format(PgbrBase.dNa_bot) << m_strUnitForce << endl
							<< _T("     Mpl,Rd    = ") << _T("Na,top*Dist.TF + Na,web,t*Dist.Wt + Na,web,c*Dist.Wc + Na,bot*Dist.BF + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
							<< endl;
					}
					else if ( PgbrBase.dPlasticNA < dTf2 ) // Bot에 위치할 때.
					{
						//double dBfc = dPlasticNA;
						//double dBft = dTF2 - dPlasticNA;
						//dNa_botc = dB2*dBfc*dfyd_b;
						//dNa_bott = dB2*dBft*dfyd_b;
						//dDistBc = 0.5*dBfc;
						//dDistBt = 0.5*dBft;
						//dDistT = (dH-0.5*dTF1)-dPlasticNA;
						//dDistW = (dTF2+0.5*dh)-dPlasticNA;
						//
						//dMpl_Rd = dNa_top*dDistT + dNa_web*dDistW + dNa_botc*dDistBc + dNa_bott*dDistBt + dCompfsdAsidsi + dTensfsdAsidsi;
						fout << _T("   - Mpl,Rd") << endl
							<< _T("     Dist.TF   = ") << Get_String_Format(PgbrBase.dDistT) << m_strUnitMM << _T("  (from Plastic NA)") << endl
							<< _T("     Dist.W    = ") << Get_String_Format(PgbrBase.dDistW) << m_strUnitMM << endl
							<< _T("     Dist.BFc  = ") << Get_String_Format(PgbrBase.dDistBc) << m_strUnitMM << endl
							<< _T("     Dist.BFt  = ") << Get_String_Format(PgbrBase.dDistBt) << m_strUnitMM << endl
							<< _T("     Na,top    = ") << Get_String_Format(PgbrBase.dNa_top) << m_strUnitForce << endl
							<< _T("     Na,web    = ") << Get_String_Format(PgbrBase.dNa_web) << m_strUnitForce << endl
							<< _T("     Na,bot,c  = ") << Get_String_Format(PgbrBase.dNa_botc) << m_strUnitForce << endl
							<< _T("     Na,bot,t  = ") << Get_String_Format(PgbrBase.dNa_bott) << m_strUnitForce << endl
							<< _T("     Mpl,Rd    = ") << _T("Na,top*Dist.TF + Na,web*Dist.W + Na,bot,t*Dist.BFt + Na,bot,c*Dist.BFc +SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
							<< endl;
					}
				}
			}
		}
		else
		{
			// Positive일 경우는 강도 계산할 때 압축콘크리트 중심(Xpl*0.5)에서 계산한다.
			if ( bPositive )
			{
				if ( PgbrBase.dPlasticNAEff > dH )
				{
					if ( dXpl > dtc )
					{
						//dNc_f = dBc*dtc*dAlpha*dfcd;
					}
					else if ( dXpl < dtc )
					{
						//dNc_f = dBc*dXpl*dAlpha*dfcd;
					}
					//
					//dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW;
					fout << _T("   - Mpl,Rd") << endl
						<< _T("     Dist.TF   = ") << Get_String_Format(PgbrBase.dDistT) << m_strUnitMM << _T("  (from Concrete Center)") << endl
						<< _T("     Dist.W    = ") << Get_String_Format(PgbrBase.dDistW) << m_strUnitMM << endl
						<< _T("     Dist.BF   = ") << Get_String_Format(PgbrBase.dDistB) << m_strUnitMM << endl
						<< _T("     Na,top    = ") << Get_String_Format(PgbrBase.dNa_top) << m_strUnitForce << endl
						<< _T("     Na,web    = ") << Get_String_Format(PgbrBase.dNa_web) << m_strUnitForce << endl
						<< _T("     Na,bot    = ") << Get_String_Format(PgbrBase.dNa_bot) << m_strUnitForce << endl
						<< _T("     Mpl,Rd    = ") << _T("Na,top*Dist.TF + Na,web*Dist.W + Na,bot*Dist.BF = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
						<< endl;
				}
				else
				{
					if ( PgbrBase.dPlasticNAEff > dH - dTf1 ) // Top에 위치할 때 
					{
						//double dTfc = dH - dPlasticNA;
						//double dTft = dPlasticNA - (dH-dTF1);
						//dNa_topc = dB1*dTfc*dfyd_t;
						//dNa_topt = dB1*dTft*dfyd_t;
						//dDistTc = 0.5*dtc + dHh + 0.5*dTfc;
						//dDistTt = 0.5*dtc + dHh + dTfc + 0.5*dTft;
						//dDistB = dHt - 0.5*dtc - 0.5*dTF2;
						//dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dh;
						//
						//dMpl_Rd = -dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web*dDistW;
						fout << _T("   - Mpl,Rd") << endl
							<< _T("     Dist.TFc  = ") << Get_String_Format(PgbrBase.dDistTc) << m_strUnitMM << _T("  (from Concrete Center)") << endl
							<< _T("     Dist.TFt  = ") << Get_String_Format(PgbrBase.dDistTt) << m_strUnitMM << endl
							<< _T("     Dist.W    = ") << Get_String_Format(PgbrBase.dDistW) << m_strUnitMM << endl
							<< _T("     Dist.BF   = ") << Get_String_Format(PgbrBase.dDistB) << m_strUnitMM << endl
							<< _T("     Na,top,c  = ") << Get_String_Format(PgbrBase.dNa_topc) << m_strUnitForce << endl
							<< _T("     Na,top,t  = ") << Get_String_Format(PgbrBase.dNa_topt) << m_strUnitForce << endl
							<< _T("     Na,web    = ") << Get_String_Format(PgbrBase.dNa_web) << m_strUnitForce << endl
							<< _T("     Na,bot    = ") << Get_String_Format(PgbrBase.dNa_bot) << m_strUnitForce << endl
							<< _T("     Mpl,Rd    = ") << _T("-Na,top,c*Dist.TFc + Na,top,t*Dist.TFt + Na,web*Dist.W + Na,bot*Dist.BF = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
							<< endl;

					}
					else if ( PgbrBase.dPlasticNAEff<dH - dTf1 && PgbrBase.dPlasticNAEff>dTf2 ) // Web에 위치할 때 
					{
						//double dWc1 = dEffeLen;
						//double dWc2 = dEffeLen;
						//double dWt = dh-dEffeLen*2.0-dHoleLen;
						//dNa_webc1 = dWc1*dTw*dfyd_w;
						//dNa_webc2 = dWc1*dTw*dfyd_w;
						//dNa_webt = dWt*dTw*dfyd_w;
						//dDistWc1 = 0.5*dtc + dHh + dTF1 + 0.5*dWc1;
						//dDistWc2 = 0.5*dtc + dHh + dTF1 + dWc1 + dHoleLen + 0.5*dWc2;
						//dDistWt  = 0.5*dtc + dHh + dTF1 + dWc1 + dWc2+dHoleLen + 0.5*dWt;
						//dDistT = dHt - 0.5*dtc - (dH-0.5*dTF1);
						//dDistB = dHt - 0.5*dtc - 0.5*dTF2;        
						//
						//dMpl_Rd = -dNa_top*dDistT-dNa_webc1*dDistWc1-dNa_webc2*dDistWc2+dNa_webt*dDistWt+dNa_bot*dDistB;
						fout << _T("   - Mpl,Rd") << endl
							<< _T("     Dist.TF   = ") << Get_String_Format(PgbrBase.dDistT) << m_strUnitMM << _T("  (from Concrete Center)") << endl
							<< _T("     Dist.Wc1  = ") << Get_String_Format(PgbrBase.dDistWc1) << m_strUnitMM << endl
							<< _T("     Dist.Wc2  = ") << Get_String_Format(PgbrBase.dDistWc2) << m_strUnitMM << endl
							<< _T("     Dist.Wt   = ") << Get_String_Format(PgbrBase.dDistWt) << m_strUnitMM << endl
							<< _T("     Dist.BF   = ") << Get_String_Format(PgbrBase.dDistB) << m_strUnitMM << endl
							<< _T("     Na,top    = ") << Get_String_Format(PgbrBase.dNa_top) << m_strUnitForce << endl
							<< _T("     Na,web,c1 = ") << Get_String_Format(PgbrBase.dNa_webc1) << m_strUnitForce << endl
							<< _T("     Na,web,c2 = ") << Get_String_Format(PgbrBase.dNa_webc2) << m_strUnitForce << endl
							<< _T("     Na,web,t  = ") << Get_String_Format(PgbrBase.dNa_webt) << m_strUnitForce << endl
							<< _T("     Na,bot    = ") << Get_String_Format(PgbrBase.dNa_bot) << m_strUnitForce << endl
							<< _T("     Mpl,Rd    = ") << _T("-Na,top*Dist.TF - Na,web,c1*Dist.Wc1 - Na,web,c2*Dist.Wc2 + Na,web,t*Dist.Wt + Na,bot*Dist.BF = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
							<< endl;

					}
					else if ( PgbrBase.dPlasticNAEff < dTf2 ) // Bot에 위치할 때.
					{
						//double dBfc = dTF2 - dPlasticNA;
						//double dBft = dPlasticNA;
						//dNa_botc = dB2*dBfc*dfyd_b;
						//dNa_bott = dB2*dBft*dfyd_b;
						//dDistBc = 0.5*dtc + dHh + dTF1 + dh + 0.5*dBfc;
						//dDistBt = 0.5*dtc + dHh + dTF1 + dh + dBfc + 0.5*dBft;
						//dDistT= dHt - 0.5*dtc - (dH-0.5*dTF1);
						//dNa_web_Effe = dTw*dEffeLen*dfyd_w;
						//dDistW1 = 0.5*dtc + dHh + 0.5*dEffeLen;
						//dDistW2 = 0.5*dtc + dHh + dEffeLen + dHoleLen + 0.5*dEffeLen;
						//
						//dMpl_Rd = -dNa_top*dDistT  - dNa_web_Effe*dDistW1 - dNa_web_Effe*dDistW2 - dNa_botc*dDistBc + dNa_bott*dDistBt;
						fout << _T("   - Mpl,Rd") << endl
							<< _T("     Dist.TF   = ") << Get_String_Format(PgbrBase.dDistT) << m_strUnitMM << _T("  (from Concrete Center)") << endl
							<< _T("     Dist.W1   = ") << Get_String_Format(PgbrBase.dDistW1) << m_strUnitMM << endl
							<< _T("     Dist.W2   = ") << Get_String_Format(PgbrBase.dDistW2) << m_strUnitMM << endl
							<< _T("     Dist.BFc  = ") << Get_String_Format(PgbrBase.dDistBc) << m_strUnitMM << endl
							<< _T("     Dist.BFt  = ") << Get_String_Format(PgbrBase.dDistBt) << m_strUnitMM << endl
							<< _T("     Na,top    = ") << Get_String_Format(PgbrBase.dNa_top) << m_strUnitForce << endl
							<< _T("     Na,web    = ") << Get_String_Format(PgbrBase.dNa_web_Effe) << m_strUnitForce << endl
							<< _T("     Na,bot,c  = ") << Get_String_Format(PgbrBase.dNa_botc) << m_strUnitForce << endl
							<< _T("     Na,bot,t  = ") << Get_String_Format(PgbrBase.dNa_bott) << m_strUnitForce << endl
							<< _T("     Mpl,Rd    = ") << _T("-Na,top*Dist.TF - Na,web*Dist.W1 - Na,web*Dist.W2 - Na,bot,c*Dist.BFc + Na,bot,t*Dist.BFt = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
							<< endl;
					}
				}
			}
			// Negative일 경우는 강도 계산할 때 PlasticNA가 기준. 
			else // Negative moment.
			{
				/*
				// Rebar 계산.
				dCompfsdAsidsi=0.0;
				dTensfsdAsidsi=0.0;
				double ddsi=0.0;
				double dAsi=0.0;
				double dNAdsi=0.0;
				double dGap = (dH+dHh+0.5*dtc) - dPlasticNA;
				for(i=0; i<arRbarPosi.GetSize(); i++)
				{
				_DGN_RBAR_CRC Rbar;
				Rbar = arRbarPosi[i];
				// PlasticNA에서 떨어진 거리.
				dNAdsi = dGap + Rbar.dz;
				dAsi = Rbar.dArea;
				if(dNAdsi<0.0) // 압축.
				{
				dCompfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
				}
				else
				{
				dTensfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
				}
				}
				*/
				//
				fout << _T("   - Force in the reinforcement") << endl
					<< _T("     SUM(fsd*As*ds),C  = ") << Get_String_Format(PgbrBase.dCompfsdAsidsi) << m_strUnitMoment << endl
					<< _T("     SUM(fsd*As*ds),T  = ") << Get_String_Format(PgbrBase.dTensfsdAsidsi) << m_strUnitMoment << endl
					<< endl;


				if ( PgbrBase.dPlasticNAEff > dH )
				{
					//dDistT = dPlasticNAEff - (dH-0.5*dTF1);
					//dDistW1 = dPlasticNAEff - (0.5*dEffeLen+dTF2);
					//dDistW2 = dPlasticNAEff - (+0.5*dEffeLen+dHoleLen+dEffeLen+dTF2);
					//dDistB = dPlasticNAEff - 0.5*dTF2;      
					//dNa_web_Effe = dEffeLen*dTw*dfyd_w;
					//
					//dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web_Effe*dDistW1 + dNa_web_Effe*dDistW2 + dCompfsdAsidsi + dTensfsdAsidsi;
					fout << _T("   - Mpl,Rd") << endl
						<< _T("     Dist.TF   = ") << Get_String_Format(PgbrBase.dDistT) << m_strUnitMM << _T("  (from Effective Plastic NA)") << endl
						<< _T("     Dist.W1   = ") << Get_String_Format(PgbrBase.dDistW1) << m_strUnitMM << endl
						<< _T("     Dist.W2   = ") << Get_String_Format(PgbrBase.dDistW2) << m_strUnitMM << endl
						<< _T("     Dist.BF   = ") << Get_String_Format(PgbrBase.dDistB) << m_strUnitMM << endl
						<< _T("     Na,top    = ") << Get_String_Format(PgbrBase.dNa_top) << m_strUnitForce << endl
						<< _T("     Na,web    = ") << Get_String_Format(PgbrBase.dNa_web_Effe) << m_strUnitForce << endl
						<< _T("     Na,bot    = ") << Get_String_Format(PgbrBase.dNa_bot) << m_strUnitForce << endl
						<< _T("     Mpl,Rd    = ") << _T("Na,top*Dist.TF + Na,web*Dist.W1 + Na,web*Dist.W2 + Na,bot*Dist.BF + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
						<< endl;
				}
				else
				{
					if ( PgbrBase.dPlasticNAEff > dH - dTf1 ) // Top에 위치할 때 
					{
						//double dTfc = dPlasticNA - (dH-dTF1);
						//double dTft = dH - dPlasticNA;
						//dNa_topc = dB1*dTfc*dfyd_t;
						//dNa_topt = dB1*dTft*dfyd_t;
						//dDistTc = 0.5*dTfc;
						//dDistTt = 0.5*dTft;
						//dDistB = dPlasticNAEff - 0.5*dTF2;
						//dDistW1 = dPlasticNAEff - (0.5*dEffeLen+dTF2);
						//dDistW2 = dPlasticNAEff - (+0.5*dEffeLen+dHoleLen+dEffeLen+dTF2);
						//dNa_web_Effe = dEffeLen*dTw*dfyd_w;
						//
						//dMpl_Rd = dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web_Effe*dDistW1 + dNa_web_Effe*dDistW2 + dCompfsdAsidsi + dTensfsdAsidsi;
						fout << _T("   - Mpl,Rd") << endl
							<< _T("     Dist.TFc  = ") << Get_String_Format(PgbrBase.dDistTc) << m_strUnitMM << _T("  (from Effective Plastic NA)") << endl
							<< _T("     Dist.TFt  = ") << Get_String_Format(PgbrBase.dDistTt) << m_strUnitMM << endl
							<< _T("     Dist.W1   = ") << Get_String_Format(PgbrBase.dDistW1) << m_strUnitMM << endl
							<< _T("     Dist.W2   = ") << Get_String_Format(PgbrBase.dDistW2) << m_strUnitMM << endl
							<< _T("     Dist.BF   = ") << Get_String_Format(PgbrBase.dDistB) << m_strUnitMM << endl
							<< _T("     Na,top,c  = ") << Get_String_Format(PgbrBase.dNa_topc) << m_strUnitForce << endl
							<< _T("     Na,top,t  = ") << Get_String_Format(PgbrBase.dNa_topt) << m_strUnitForce << endl
							<< _T("     Na,web    = ") << Get_String_Format(PgbrBase.dNa_web_Effe) << m_strUnitForce << endl
							<< _T("     Na,bot    = ") << Get_String_Format(PgbrBase.dNa_bot) << m_strUnitForce << endl
							<< _T("     Mpl,Rd    = ") << _T("Na,top,t*Dist.TFt + Na,top,c*Dist.TFc + Na,web*Dist.W1 + Na,web*Dist.W2 + Na,bot*Dist.BF + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
							<< endl;
					}
					else if ( PgbrBase.dPlasticNAEff<dH - dTf1 && PgbrBase.dPlasticNAEff>dTf2 ) // Web에 위치할 때 
					{
						//double dWc1 = dEffeLen; 
						//double dWc2 = dEffeLen; 
						//double dWt = (dH-dTF1) - dPlasticNAEff;
						//dNa_webc1 = dWc1*dTw*dfyd_w;
						//dNa_webc2 = dWc2*dTw*dfyd_w;
						//dNa_webt = dWt*dTw*dfyd_w;
						//dDistWc1 = 0.5*dWc1;
						//dDistWc2 = dWc1+dHoleLen+dWc2*0.5;
						//dDistWt = 0.5*dWt;
						//dDistT = (dH-0.5*dTF1) - dPlasticNAEff;
						//dDistB = dPlasticNAEff - 0.5*dTF2;        
						//
						//dMpl_Rd = dNa_top*dDistT+dNa_webc1*dDistWc1+dNa_webc2*dDistWc2+dNa_webt*dDistWt+dNa_bot*dDistB + dCompfsdAsidsi + dTensfsdAsidsi;
						fout << _T("   - Mpl,Rd") << endl
							<< _T("     Dist.TF   = ") << Get_String_Format(PgbrBase.dDistT) << m_strUnitMM << _T("  (from Effective Plastic NA)") << endl
							<< _T("     Dist.Wc1  = ") << Get_String_Format(PgbrBase.dDistWc1) << m_strUnitMM << endl
							<< _T("     Dist.Wc2  = ") << Get_String_Format(PgbrBase.dDistWc2) << m_strUnitMM << endl
							<< _T("     Dist.Wt   = ") << Get_String_Format(PgbrBase.dDistWt) << m_strUnitMM << endl
							<< _T("     Dist.BF   = ") << Get_String_Format(PgbrBase.dDistB) << m_strUnitMM << endl
							<< _T("     Na,top    = ") << Get_String_Format(PgbrBase.dNa_top) << m_strUnitForce << endl
							<< _T("     Na,web,c1 = ") << Get_String_Format(PgbrBase.dNa_webc1) << m_strUnitForce << endl
							<< _T("     Na,web,c2 = ") << Get_String_Format(PgbrBase.dNa_webc2) << m_strUnitForce << endl
							<< _T("     Na,web,t  = ") << Get_String_Format(PgbrBase.dNa_webt) << m_strUnitForce << endl
							<< _T("     Na,bot    = ") << Get_String_Format(PgbrBase.dNa_bot) << m_strUnitForce << endl
							<< _T("     Mpl,Rd    = ") << _T("Na,top*Dist.TF + Na,web,t*Dist.Wt + Na,web,c1*Dist.Wc1 + Na,web,c2*Dist.Wc2 + Na,bot*Dist.BF + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
							<< endl;
					}
					else if ( PgbrBase.dPlasticNAEff < dTf2 ) // Bot에 위치할 때.
					{
						//double dBfc = dPlasticNAEff;
						//double dBft = dTF2 - dPlasticNAEff;
						//dNa_botc = dB2*dBfc*dfyd_b;
						//dNa_bott = dB2*dBft*dfyd_b;
						//dDistBc = 0.5*dBfc;
						//dDistBt = 0.5*dBft;
						//dDistT = (dH-0.5*dTF1)-dPlasticNAEff;
						//dDistW = (dTF2+0.5*dh)-dPlasticNAEff;
						//
						//dMpl_Rd = dNa_top*dDistT + dNa_web*dDistW + dNa_botc*dDistBc + dNa_bott*dDistBt + dCompfsdAsidsi + dTensfsdAsidsi;
						fout << _T("   - Mpl,Rd") << endl
							<< _T("     Dist.TF   = ") << Get_String_Format(PgbrBase.dDistT) << m_strUnitMM << _T("  (from Effective Plastic NA)") << endl
							<< _T("     Dist.W    = ") << Get_String_Format(PgbrBase.dDistW) << m_strUnitMM << endl
							<< _T("     Dist.BFc  = ") << Get_String_Format(PgbrBase.dDistBc) << m_strUnitMM << endl
							<< _T("     Dist.BFt  = ") << Get_String_Format(PgbrBase.dDistBt) << m_strUnitMM << endl
							<< _T("     Na,top    = ") << Get_String_Format(PgbrBase.dNa_top) << m_strUnitForce << endl
							<< _T("     Na,web    = ") << Get_String_Format(PgbrBase.dNa_web_Effe) << m_strUnitForce << endl
							<< _T("     Na,bot,c  = ") << Get_String_Format(PgbrBase.dNa_botc) << m_strUnitForce << endl
							<< _T("     Na,bot,t  = ") << Get_String_Format(PgbrBase.dNa_bott) << m_strUnitForce << endl
							<< _T("     Mpl,Rd    = ") << _T("Na,top*Dist.TF + Na,web*Dist.Wt + Na,bot,t*Dist.BFt + Na,bot,c*Dist.BFc + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
							<< endl;
					}
				}
			}
		}

		BOOL bS420, bS460;
		if ( bPositive )
		{
			// 6.2.1.2(2)
			bS420 = dfy - m_dZero >= 420.0 && dfy + m_dZero <= 420.0 ? TRUE : FALSE;
			bS460 = dfy - m_dZero >= 460.0 && dfy + m_dZero <= 460.0 ? TRUE : FALSE;
			fout << _T("   - Reduction factor beta") << endl
				<< _T("     Xpl/h    = ") << Get_String_Format(PgbrBase.dXplOverh) << endl
				<< _T("     Beta     = ") << Get_String_Format(PgbrBase.dBeta) << endl
				<< endl;
		}

		// Beta 적용은 정모멘트만.
		//PgbrBase.dMpl_Rd = dBeta*dMpl_Rd;
		//PgbrBase.dNc_f = dNc_f;
		//PgbrBase.dNc = dFx;          // Nc       
		//PgbrBase.dNc_el = 0.0;       // Nc,el    

		// Elastic Resistance
		//Calc_ElasticResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, SconElem, PgbrBase);
		T_COMF_KEY ComfKey;
		ComfKey.DgnLcomK = PgbrBase.LcomK;
		ComfKey.ElemK = ElemK.first;
		ComfKey.nPos = bPosiI ? 0 : 1;

		T_SECT_STIFFNESS EffeStlSect;    EffeStlSect.Initialize();
		T_SECT_STIFFNESS EffeComSect;    EffeComSect.Initialize();
		//
		CPG_SECT_EFFE EffeSect;
		EffeSect.Initialize();
		//    
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCpgSectEffeResult(ElemK, bPosiI, PgbrBase.LcomK, EffeSect);
		EffeStlSect = EffeSect.EffeStlSect;
		EffeComSect = EffeSect.EffeComSect;

		// Before
		double dStl_Iyy = EffeStlSect.Ryy;
		double dStl_Czm = EffeStlSect.Czm;
		double dStl_Czp = EffeStlSect.Czp;
		double dStl_zDim = dStl_Czp + dStl_Czm;
		double dStl_Stop = dStl_Czp;
		// Composite
		double dIyy = EffeComSect.Ryy;
		double dCzm = EffeComSect.Czm;
		double dCzp = EffeComSect.Czp;
		double dStop = dH - dCzm;

		//double dSIGa_top=0.0;
		//double dSIGa_bot=0.0;  

		//double dStrStlTop = dMa_Ed/dStl_Iyy*(-dStl_Stop); // dStl_Iyy 이것도 Effective 이어야함. 
		//double dStrComTop = dMc_Ed/dIyy*(-dStop);
		//double dStrStlBot = dMa_Ed/dStl_Iyy*dStl_Czm;
		//double dStrComBot = dMc_Ed/dIyy*dCzm;
		//dSIGa_top = dStrStlTop + dStrComTop; // (-) 압축, (+) 인장.
		//dSIGa_bot = dStrStlBot + dStrComBot;

		fout << _T("   - Elastic resistance moment Mel,Rd") << endl
			<< _T("   ()Effective stiffness for girder") << endl
			<< _T("     Iyy_stl  = ") << Get_String_Format(dStl_Iyy) << m_strUnitMM4 << endl
			<< _T("     Zt_stl   = ") << Get_String_Format(dStl_Czp) << m_strUnitMM << endl
			<< _T("     Zb_stl   = ") << Get_String_Format(dStl_Czm) << m_strUnitMM << endl
			<< endl;

		fout << _T("   ()Effective stiffness for Composite") << endl
			<< _T("     Iyy_com  = ") << Get_String_Format(dIyy) << m_strUnitMM4 << endl
			<< _T("     Zt_com   = ") << Get_String_Format(dStop) << m_strUnitMM << endl
			<< _T("     Zb_com   = ") << Get_String_Format(dCzm) << m_strUnitMM << endl
			<< endl;

		fout << _T("     Sigma,top_stl  = Ma,Ed/Iyy_stl*Zt_stl = ") << Get_String_Format(PgbrBase.dStrStlTop) << m_strUnitStress << endl
			<< _T("     Sigma,bot_stl  = Ma,Ed/Iyy_stl*Zb_stl = ") << Get_String_Format(PgbrBase.dStrStlBot) << m_strUnitStress << endl
			<< _T("     Sigma,top_com  = Mc,Ed/Iyy_com*Zt_com = ") << Get_String_Format(PgbrBase.dStrComTop) << m_strUnitStress << endl
			<< _T("     Sigma,bot_com  = Mc,Ed/Iyy_com*Zb_com = ") << Get_String_Format(PgbrBase.dStrComTop) << m_strUnitStress << endl
			<< _T("     Sigma,top      = Sigma,top_stl + Sigma,top_com = ") << Get_String_Format(PgbrBase.dSIGa_top) << m_strUnitStress << endl
			<< _T("     Sigma,bot      = Sigma,bot_stl + Sigma,bot_com = ") << Get_String_Format(PgbrBase.dSIGa_top) << m_strUnitStress << endl
			<< endl;

		if ( fabs(PgbrBase.dSIGa_top) >= fabs(PgbrBase.dSIGa_bot) )
		{
			//dk_min = dStrComTop==0.0 ? 0.0 : (dfy-fabs(dStrStlTop))/fabs(dStrComTop);
			fout << _T("     Sigma,top>=Sigma,bot") << endl
				<< _T("     k    = (fy-ABS(Sigma,top_stl))/ABS(Sigma,top_com) = ") << Get_String_Format(PgbrBase.dk_min) << endl
				<< endl;
		}
		else
		{
			//dk_min = dStrComTop==0.0 ? 0.0 : (dfy-fabs(dStrStlBot))/fabs(dStrComBot);
			fout << _T("     Sigma,top<Sigma,bot") << endl
				<< _T("     k    = (fy-ABS(Sigma,bot_stl))/ABS(Sigma,bot_com) = ") << Get_String_Format(PgbrBase.dk_min) << endl
				<< endl;
		}

		//dMel_Rd = fabs(dMa_Ed) + dk_min*fabs(dMc_Ed);
		fout << _T("     Mel,Rd = Ma,Ed + k*Mc,Ed = ") << Get_String_Format(PgbrBase.dMel_Rd) << m_strUnitMoment << endl
			<< endl;

		// Positive 일 때만 계산.
		//if(dMy>=0.0)
		//{
		//  dNc_el = dk_min*dMc_Ed/dIyy*(dStop-0.5*dtc);
		//}

		BOOL bNonLinear = FALSE;
		if ( dXplOverh > 0.4 ) { bNonLinear = TRUE; }

		// Non-linear Resistance
		if ( bPositive && (bS420 || bS460) && bNonLinear )
		{
			//dNc_el = dk_min*dMc_Ed/dIyy*(dStop-0.5*dtc);
			//Calc_NonLinearResistance(ElemK, bPosiI, PgbrBase);

			fout << _T("  -  Non-linear resistance moment M,Rd") << endl
				<< _T("     Nc,el = k*Mc,Ed/Iyy_com*(Zt_com-0.5*tc) = ") << Get_String_Format(PgbrBase.dNc_el) << m_strUnitForce << endl;
			if ( fabs(PgbrBase.dNc) <= PgbrBase.dNc_el )
			{
				//dM_Rd = dNc_el==0.0 ? dMa_Ed : dMa_Ed+(dMel_Rd-dMa_Ed)*fabs(dNc)/dNc_el;
				fout << _T("     M_Rd = Ma,Ed+(Mel,Rd-Ma,Ed)*Nc/Nc,el") << Get_String_Format(PgbrBase.dM_Rd) << m_strUnitMoment << endl;
			}
			else if ( PgbrBase.dNc_el <= fabs(PgbrBase.dNc) && fabs(PgbrBase.dNc) <= PgbrBase.dNc_f )
			{
				if ( dXpl > dtc )
				{
					//dNc_f = dBc*dtc*dAlpha*dfcd;
					fout << _T("     Nc,f = Bc*tc*Alpha*fcd = ") << Get_String_Format(PgbrBase.dNc_f) << m_strUnitForce << endl;
				}
				else if ( dXpl < dtc )
				{
					//dNc_f = dBc*dXpl*dAlpha*dfcd;
					fout << _T("     Nc,f = Bc*Xpl*Alpha*fcd = ") << Get_String_Format(PgbrBase.dNc_f) << m_strUnitForce << endl;
				}

				//dM_Rd = (dNc_f-dNc_el)==0.0 ? dMel_Rd : dMel_Rd+(dMpl_Rd-dMel_Rd)*(fabs(dNc)-dNc_el)/(dNc_f-dNc_el);
				fout << _T("     M_Rd = Mel,Rd+(Mpl,Rd-Mel,Ed)*(Nc-Nc,el)/(Nc,f-Nc,el) = ") << Get_String_Format(PgbrBase.dM_Rd) << m_strUnitMoment << endl;
			}
			else
			{
			}
		}
		else
		{
			//PgbrBase.dM_Rd = iClassSect==1 || iClassSect==2 ? PgbrBase.dMpl_Rd : PgbrBase.dMel_Rd;
			if ( PgbrBase.iSectClass == 1 || PgbrBase.iSectClass == 2 )
				fout << _T("     M,Rd = Mpl,Rd = ") << Get_String_Format(PgbrBase.dM_Rd) << m_strUnitMoment << endl;
			else
				fout << _T("     M,Rd = Mel,Rd = ") << Get_String_Format(PgbrBase.dM_Rd) << m_strUnitMoment << endl;

		}
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Detail_VerticalShearResistance(ElemPairK ElemK, BOOL bPosiI)
{
	rptwofstream fout(m_strTestDgnFile, ios::app);

	T_PRVS_D PrvsD;
	if ( !m_pAnalysisResult->GetPrvsResultData(ElemK, PrvsD) ) { return FALSE; }

	T_PRVS_BASE PrvsBase;
	PrvsBase = bPosiI ? PrvsD.PrvsBase[0] : PrvsD.PrvsBase[1];

	BOOL bSect = TRUE, bMatl = TRUE, bTses = TRUE;
	CPG_SECT_ELEM SectElem;
	if ( !Get_CpgSectElem(ElemK, SectElem) ) { bSect = FALSE; }

	CPG_MATL_ELEM MatlElem;
	if ( !Get_CpgMatlElem(ElemK, MatlElem) ) { bMatl = FALSE; }

	CPG_TSES_ELEM TsesElem;
	if ( Get_CpgTsesElem(ElemK, TsesElem) ) { bTses = FALSE; }

	CPG_TSES_POSI TsesPosi;
	TsesPosi = bPosiI ? TsesElem.TsesPosi[0] : TsesElem.TsesPosi[1];

	// 1. Section.  
	T_SECT_D& SectD = SectElem.SectD;
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTf1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dTf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

	// 2. Material
	T_MATD_D& MatdD = MatlElem.MatdD;
	int iMatClass = MatlElem.iMatClass;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfu = MatdD.Data1.Design.S_Fu;
	double dEs = MatdD.Data1.Analysis.Elast;
	double dfck = MatlElem.dfck;
	double dfcm = MatlElem.dfcm;
	double dfctm = MatlElem.dfctm;
	double dEcm = MatlElem.dEcm;
	double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
	double dErs = dEs;
	double dgam_M0 = m_dGamma_M0;
	double dgam_M1 = m_dGamma_M1;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;
	double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk / dgam_sr;
	double dfyt = Get_fy4Thick(dfy, dfy1, dTf1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTf2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;

	// Load combination
	LPCTSTR aType[] = { _T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN),
		_LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
		_LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN) };

	// Lcom
	CString strLoadComb;
	T_LCOM_D LcomData;
	if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, PrvsBase.OrgLcomK, LcomData) )
	{
		strLoadComb.Format(_T("%s(%s)"), LcomData.LoadCombName, aType[PrvsBase.iMax]);
	}
	else
	{
		strLoadComb = _T("-");
	}

	//
	T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
	T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
	GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

	T_SECT_STIFFNESS StiffData;  StiffData.Initialize();
	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
	if ( !bTapered )
	{
		StiffData = SectD.SectBefore.Stiffness;
	}
	else
	{
		StiffData = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;
	}

	// Girder Stiffness	
	double dIyy_f = StiffData.Ryy;
	double dIzz_f = StiffData.Rzz;
	double dCzp_f = StiffData.Czp;
	double dCzm_f = StiffData.Czm;

	//T_SECT_STIFFNESS CompoSect;
	//CompoSect = dMy>=0.0 ? RebarSect : NonConSect;
	//double dIyy_c = CompoSect.Ryy;
	//double dCzp_c = CompoSect.Czp;
	//double dCzm_c = CompoSect.Czm;  

	fout << endl
		<< _T("--------------------------------------------------------------------------------------") << endl
		<< endl
		<< _T("  *. Resistance to vertical shear") << endl
		<< endl
		<< _T("    - Load Combination     = ") << strLoadComb << endl
		<< _T("      V,Ed = ") << Get_String_Format(PrvsBase.dV_Ed) << m_strUnitForce << endl
		<< endl;

	if ( iMatClass == 1 ) // S235, S275, S355 계열.
	{
		//dEta = 1.20*dgam1/dgam0;
		fout << _T("     eta    = 1.20*gamma1/gamma0 = ") << Get_String_Format(PrvsBase.deta) << endl;
	}
	else if ( iMatClass == 2 ) // S420, S460 계열.
	{
		//dEta = 1.05*dgam1/dgam0;
		fout << _T("     eta    = 1.05*gamma1/gamma0 = ") << Get_String_Format(PrvsBase.deta) << endl;
	}
	else if ( iMatClass == 3 ) // etc.
	{
		//dEta = 1.00*dgam1/dgam0;
		fout << _T("     eta    = 1.00*gamma1/gamma0 = ") << Get_String_Format(PrvsBase.deta) << endl;
	}
	else
	{
	}


	// Plastic resistance to vertical shear, EN 1993-1-1: 2005, 6.2.6 (50page)

	//dAv = dEta*dHw*dTw;
	//dVpl_Rd = dgam_M0==0.0 ? 0.0 : dAv*(dfyd_w/sqrt(3.0))/dgam_M0;
	fout << _T("     Av     = eta*Hw*tw = ") << Get_String_Format(PrvsBase.dAv) << m_strUnitMM2 << endl
		<< _T("     Vpl,Rd = Av*(fyd/Root(3))/gamma,M0 = ") << Get_String_Format(PrvsBase.dVpl_Rd) << m_strUnitForce << endl
		<< endl;

	// Shear buckling resistance, EN 1993-1-5,5 (22page)
	BOOL bEndSupport = TsesPosi.bUseStif ? TRUE : FALSE;
	int iEndpost = TsesPosi.iEndpost; // 0=Rigid end post, 1=Non-rigid end post, 2=No end post,
	//BOOL bLongStiff = arLstiPosi.GetSize()==0 ? FALSE : TRUE;
	//BOOL bTranStiff = TrstPosi.bUseStif;


	//

	// I_sl is the sum of the stiffness of the individual stiffeners. EN 1993-1-5 : 2004, 42page
	//
	fout << _T("   - The second moment of area of a longitudinal stiffener") << endl
		<< _T("     I,sl   = ") << Get_String_Format(PrvsBase.dIsl_org) << m_strUnitMM4 << endl
		<< _T("     I,sl   = 1/3*I,sl = ") << Get_String_Format(PrvsBase.dIsl) << m_strUnitMM4 << endl
		<< endl;

	//
	fout << _T("   - Contribution from thw web Xw to shear buckling resistance") << endl;
	if ( !bEndSupport || (bEndSupport && iEndpost == 2) ) //2=No end post
	{
		// 이 경우 기준이 없음. 그래서 dXw=1
		//dXw = 1.0;  
		//
		//dDist4Shr = 1.0; //stiffener가 없으면 1로 한다. 
		fout << _T("     Xw  = ") << Get_String_Format(PrvsBase.dXw) << endl;
	}
	else
	{
		//
		//if(!bLongStiff && bTranStiff)

		// 5.3(3) The slenderness parameter (Lamda_w) , EN 1993-1-5 : 2004, 23page

		//
		fout << _T("     Lamda,w  = ") << Get_String_Format(PrvsBase.dMax_Bar_Lamda_w) << endl
			<< _T("     Xw       = ") << Get_String_Format(PrvsBase.dXw) << endl;

		//
	}

	// contribution from the web factor
	// 5.3(5) For webs with longitudinal stiffeners the slenderness parameter (lamda_w) in (3) should not be taken as less than ,EN 1993-1-5 : 2004, 24page   
	//if(dMax_Bar_Lamda_w<dMax_Lamda_wi)
	//  dMax_Bar_Lamda_w = dMax_Lamda_wi;

	//Get_ContributionFromWebFactor(iEndpost,dEta,dMax_Bar_Lamda_w,dXw);

	//dVbw_Rd = dgam_M1==0.0 ? 0.0 : dXw*dfyd_w*dHw*dTw/(sqrt(3.0)*dgam_M1);

	fout << endl
		<< _T("   - Contribution from the web") << endl
		<< _T("     Vbw,Rd  = Xw*fyw*hw*tw/(Root(3)*gamma1) = ") << Get_String_Format(PrvsBase.dVbw_Rd) << endl
		<< endl;


	// EN 1993-1-5 7.1 (28page)
	// 7.1 Interaction between shear force, bending moment and axial force
	//double dMf_Rd = 0.0;
	//double dMpl_Rd = 0.0;
	//double dWtop_f = dCzp_f==0.0 ? 0.0 : dIyy_f/dCzp_f;
	//double dWbot_f = dCzm_f==0.0 ? 0.0 : dIyy_f/dCzm_f;
	//double dWtop_c = dCzp_c==0.0 ? 0.0 : dIyy_c/dCzp_c;
	//double dWbot_c = dCzm_c==0.0 ? 0.0 : dIyy_c/dCzm_c;
	//

	//dMf_Rd = PrvsBase.dMf_Rd;
	//dMpl_Rd = PrvsBase.dMpl_Rd; // Bending의 Mpl_Rd와 다를 수 있다. 

	//CPG_SECT_EFFE EffeSect;
	//EffeSect.Initialize();
	//if(!m_EffeSectMap.Lookup(ComfKey, EffeSect))
	//  ASSERT(0);

	//T_SECT_STIFFNESS EffeStlSect;
	//EffeStlSect.Initialize();
	//T_SECT_STIFFNESS EffeComSect;
	//EffeComSect.Initialize();

	//EffeStlSect = EffeSect.EffeStlSect;
	//EffeComSect = EffeSect.EffeComSect;

	// Before
	/*
	double dStl_Iyy  = EffeStlSect.Ryy;
	double dStl_Czm  = EffeStlSect.Czm;
	double dStl_Czp  = EffeStlSect.Czp;
	double dStl_zDim = dStl_Czp+dStl_Czm;
	double dStl_Stop = dStl_Czp;
	// Composite
	double dIyy  = EffeComSect.Ryy;
	double dCzm  = EffeComSect.Czm;
	double dCzp  = EffeComSect.Czp;
	double dzDim = dCzm+dCzp;
	double dStop = dH - dCzm;

	double dStrStlTop = dMa_Ed/dStl_Iyy*(-dStl_Stop); // dStl_Iyy 이것도 Effective 이어야함.
	double dStrComTop = dMc_Ed/dIyy*(-dStop);
	double dStrStlBot = dMa_Ed/dStl_Iyy*dStl_Czm;
	double dStrComBot = dMc_Ed/dIyy*dCzm;
	double dSIGa_top = dStrStlTop + dStrComTop;
	double dSIGa_bot = dStrStlBot + dStrComBot;

	double dM_Ed_top = dSIGa_top*(dIyy/dStop);
	double dM_Ed_bot = dSIGa_bot*(dIyy/dCzm);;
	double dM_Ed = max(fabs(dM_Ed_top), fabs(dM_Ed_bot));
	double dV_Ed = PrvsBase.dV_Ed;

	double dBendingRat = dMpl_Rd==0.0 ? 0.0 : dMf_Rd/dMpl_Rd;
	double dBar_Eta1 = dMpl_Rd==0.0 ? 0.0 : dM_Ed/dMpl_Rd;
	double dBar_Eta3 = dVbw_Rd==0.0 ? 0.0 : fabs(dV_Ed)/dVbw_Rd;

	double dCombinedEffe=0.0;
	dCombinedEffe = dBar_Eta1+(1.0-dBendingRat)*pow((2.0*dBar_Eta3-1.0),2);

	BOOL bEta3Exceed05=FALSE;
	BOOL bCombinedEffe=FALSE;
	if(dBar_Eta3>0.5)
	bEta3Exceed05 = TRUE;

	*/

	// Contribution from flange, EN 1993-1-5 , 25page
	//double dAf_top = dB1*dTF1;
	//double dAf_bot = dB2*dTF2;  
	//double dbf=0.0;
	//double dtf=0.0;
	//double dfyf=0.0;
	//double da = dDist4Shr;  // _T("a") is the distance bethween transverse stiffeners 

	fout << _T("   - Contribution from the flanges") << endl
		<< _T("     M,Ed  = ") << Get_String_Format(PrvsBase.dM_Ed) << m_strUnitMoment << endl
		<< _T("     Mf,Rd = ") << Get_String_Format(PrvsBase.dMf_Rd) << m_strUnitMoment << endl
		<< _T("     fyw   = ") << Get_String_Format(dfyd_w) << m_strUnitStress << endl
		<< _T("     fyf   = ") << Get_String_Format(dfyd_t) << m_strUnitStress << endl
		<< _T("     bf    = ") << Get_String_Format(PrvsBase.dbf) << m_strUnitMM << endl
		<< _T("     tf    = ") << Get_String_Format(PrvsBase.dtf) << m_strUnitMM << endl;


	/*
	if(dAf_top<=dAf_bot)
	{
	dfyf = dfyd_t;
	dtf = dTF1;
	double dbf_lim = 15.0*dEpsi*dtf;
	double dbf_h = (dB1-dTw)*0.5;
	if(dbf_h>dbf_lim)
	dbf = dTw+dbf_lim*2.0;
	else
	dbf = dB1;
	}
	else
	{
	dfyf = dfyd_b;
	dtf = dTF2;
	double dbf_lim = 15.0*dEpsi*dtf;
	double dbf_h = (dB2-dTw)*0.5;
	if(dbf_h>dbf_lim)
	dbf = dTw+dbf_lim*2.0;
	else
	dbf = dB2;
	}
	*/

	//double dc = dTw*pow(dHw,2)*dfyd_w==0.0 ? 0.0 : da*(0.25+1.6*dbf*pow(dtf,2)*dfyf/(dTw*pow(dHw,2)*dfyd_w));
	fout << _T("     c = a*(0.25+1.6*bf*tf^2*fyf/(tw*hw^2*fyw) = ") << Get_String_Format(PrvsBase.dc) << m_strUnitMM << endl;
	if ( PrvsBase.dMf_Rd != 0.0 && PrvsBase.dM_Ed < PrvsBase.dMf_Rd )
	{
		//dVbf_Rd = dc*dgam_M1==0.0 ? 0.0 : dbf*pow(dtf,2)*dfyf/(dc*dgam_M1)*(1.0-pow(dM_Ed/dMf_Rd,2));
		fout << _T("     Vbf,Rd  = bf*tf^2*fyf/(c*gamma,M1)*(1-(M,Ed/Mf,Rd)^2) = ") << Get_String_Format(PrvsBase.dVbf_Rd) << m_strUnitForce << endl
			<< endl;
	}
	else
	{
		//dVbf_Rd = 0.0;
		fout << _T("     Mf,Rd = 0 or M,Ed > Mf,Rd") << endl
			<< _T("     Vbf,Rd  = ") << Get_String_Format(PrvsBase.dVbf_Rd) << m_strUnitForce << endl
			<< endl;
	}

	//
	//dVb_Rd = dVbw_Rd + dVbf_Rd;
	fout << _T("     Vb,Rd = Vbw,Rd + Vbf,Rd  = ") << Get_String_Format(PrvsBase.dVb_Rd_org) << m_strUnitForce << endl;
	//double dVb_Rd_Lim = dgam_M1==0.0 ? 0.0 : dEta*dfyd_w*dHw*dTw/(sqrt(3.0)*dgam_M1);
	if ( PrvsBase.dVb_Rd_org > PrvsBase.dVb_Rd_Lim )
	{
		//dVb_Rd = dVb_Rd_Lim;
		fout << _T("     Vb,Rd,Limit = eta*fyw*Hw*Tw/(Root(3)*gamma,M1) = ") << Get_String_Format(PrvsBase.dVb_Rd_Lim) << m_strUnitForce << endl
			<< _T("     Vb,Rd > Vb,Rd,Limit") << endl
			<< _T("     Vb,Rd = Vb,Rd,Limit = ") << Get_String_Format(PrvsBase.dVb_Rd) << m_strUnitForce << endl
			<< endl;
	}

	//double dBendingRat = dMpl_Rd==0.0 ? 0.0 : dMf_Rd/dMpl_Rd;
	//double dBar_Eta1 = dMpl_Rd==0.0 ? 0.0 : dM_Ed/dMpl_Rd;
	//double dBar_Eta3 = dVbw_Rd==0.0 ? 0.0 : fabs(dV_Ed)/dVbw_Rd;

	//double dCombinedEffe=0.0;
	//dCombinedEffe = dBar_Eta1+(1.0-dBendingRat)*pow((2.0*dBar_Eta3-1.0),2);
	if ( PrvsBase.bEta3Exceed05 && PrvsBase.bEta1Ratio )
	{
		fout << _T("    - Eta3>0.5 and Eta1>Mf,Rd/Mpl,Rd ") << endl
			<< _T("     Eta1  = M,Ed/Mpl,Rd = ") << Get_String_Format(PrvsBase.dEta1) << endl
			<< _T("     Eta3  = V,Ed/Vbw,Rd = ") << Get_String_Format(PrvsBase.dEta3) << endl;

		CString strSign; strSign = PrvsBase.bCombinedEffe ? _T("  <  1.0") : _T("  >  1.0");
		fout << _T("     Eta1+(1-Mf,Rd/Mpl,Rd)*(2*Eta3-1)^2 = ") << Get_String_Format(PrvsBase.dCombinedEffe) << strSign << endl
			<< endl;
	}


	return TRUE;
}

BOOL CDgnPlateGirder::Detail_LateralTorsionalBucklingResistance(ElemPairK ElemK, BOOL bPosiI)
{
	rptwofstream fout(m_strTestDgnFile, ios::app);

	T_PLTB_D PltbD;
	if ( !m_pAnalysisResult->GetPltbResultData(ElemK, PltbD) ) { return FALSE; }

	T_PLTB_BASE PltbBase;
	PltbBase = bPosiI ? PltbD.PltbBase[0] : PltbD.PltbBase[1];

	BOOL bSect = TRUE, bMatl = TRUE, bTses = TRUE, bLtbd = TRUE, bMemb = TRUE;
	CPG_SECT_ELEM SectElem;
	if ( !Get_CpgSectElem(ElemK, SectElem) ) { bSect = FALSE; }

	CPG_MATL_ELEM MatlElem;
	if ( !Get_CpgMatlElem(ElemK, MatlElem) ) { bMatl = FALSE; }

	CPG_TSES_ELEM TsesElem;
	if ( !Get_CpgTsesElem(ElemK, TsesElem) ) { bTses = FALSE; }

	CPG_TSES_POSI TsesPosi;
	TsesPosi = bPosiI ? TsesElem.TsesPosi[0] : TsesElem.TsesPosi[1];

	CPG_LTBD_ELEM LtbdElem;  LtbdElem.Initialize();
	if ( !Get_CpgLtbdElem(ElemK, LtbdElem) ) { bLtbd = FALSE; }
	// Later-torsional buckling data
	CPG_LTBD_POSI LtbdPosi;
	LtbdPosi = bPosiI ? LtbdElem.LtbdPosi[0] : LtbdElem.LtbdPosi[1];

	CPG_MEMB_ELEM MembElem;  MembElem.Initialize();
	if ( !Get_CpgMembElem(ElemK, MembElem) ) { bMemb = FALSE; }
	// Member data, Lu
	CPG_MEMB_POSI MembPosi;
	MembPosi = bPosiI ? MembElem.MembPosi[0] : MembElem.MembPosi[1];

	// 1. Section.  
	T_SECT_D& SectD = SectElem.SectD;
	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTf1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dTf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

	// 2. Material
	T_MATD_D& MatdD = MatlElem.MatdD;
	int iMatClass = MatlElem.iMatClass;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfu = MatdD.Data1.Design.S_Fu;
	double dEs = MatdD.Data1.Analysis.Elast;
	double dPoissonS = MatdD.Data1.Analysis.Poisson;
	double dGs = dEs / (1.0 + dPoissonS); //MatdD.Data1.Analysis.ShearModulus; ?
	double dfck = MatlElem.dfck;
	double dfcm = MatlElem.dfcm;
	double dfctm = MatlElem.dfctm;
	double dEcm = MatlElem.dEcm;
	double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
	double dErs = dEs;
	double dgam_M0 = m_dGamma_M0;
	double dgam_M1 = m_dGamma_M1;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;
	double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk / dgam_sr;
	double dfyt = Get_fy4Thick(dfy, dfy1, dTf1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTf2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;

	// Load combination
	LPCTSTR aType[] = { _T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN),
		_LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
		_LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN) };

	// Lcom
	CString strLoadComb;
	T_LCOM_D LcomData;
	if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, PltbBase.OrgLcomK, LcomData) )
		strLoadComb.Format(_T("%s(%s)"), LcomData.LoadCombName, aType[PltbBase.iMax]);
	else
		strLoadComb = _T("-");

	//
	T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
	T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
	GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

	T_SECT_STIFFNESS StiffData;
	StiffData.Initialize();
	if ( !bTapered )
		StiffData = SectD.SectBefore.Stiffness;
	else
		StiffData = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;

	// Girder Stiffness	
	double dArea_g = StiffData.Area;
	double dIxx_g = StiffData.Rxx;
	double dIyy_g = StiffData.Ryy;
	double dIzz_g = StiffData.Rzz;
	double dCzp_g = StiffData.Czp;
	double dCzm_g = StiffData.Czm;

	T_SECT_STIFFNESS CompoSect;
	CompoSect = PltbBase.dMy >= 0.0 ? RebarSect : NonConSect;
	double dArea_c = CompoSect.Area;
	double dIyy_c = CompoSect.Ryy;
	double dCzp_c = CompoSect.Czp;
	double dCzm_c = CompoSect.Czm;

	//
	T_PGBR_D PgbrD;
	PgbrD.Initialize();
	T_PGBR_LCOM PgbrLcomD;
	PgbrLcomD.Initialize();
	int iDgnLcomNo = PltbBase.LcomK;
	int iMaxMin = 0;
	if ( !m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgbrLcomD) )
	{
	}

	T_PGBR_BASE PgbrBase;
	PgbrBase = bPosiI ? PgbrLcomD.PgbrBase[0] : PgbrLcomD.PgbrBase[1];
	//dMpl_Rd = PgbrBase.dMpl_Rd;
	//dMel_Rd = PgbrBase.dMel_Rd;


	fout << endl
		<< _T("--------------------------------------------------------------------------------------") << endl
		<< endl
		<< _T("  *. Resistance to lateral-torsional buckling") << endl
		<< endl
		<< _T("    - Load Combination     = ") << strLoadComb << endl
		<< _T("      N,Ed    = ") << Get_String_Format(PltbBase.dN_Ed) << m_strUnitForce << endl
		<< _T("      M,Ed    = ") << Get_String_Format(PltbBase.dM_Ed) << m_strUnitMoment << endl
		<< _T("      V1      = ") << Get_String_Format(PltbBase.dV1) << m_strUnitForce << endl
		<< _T("      V2      = ") << Get_String_Format(PltbBase.dV2) << m_strUnitForce << endl
		<< _T("      M1      = ") << Get_String_Format(PltbBase.dM1) << m_strUnitMoment << endl
		<< _T("      M2      = ") << Get_String_Format(PltbBase.dM2) << m_strUnitMoment << endl
		<< _T("      Mpl,Rd  = ") << Get_String_Format(PgbrBase.dMpl_Rd) << m_strUnitMoment << endl
		<< _T("      Mel,Rd  = ") << Get_String_Format(PgbrBase.dMel_Rd) << m_strUnitMoment << endl
		<< endl;
	/*
	int iClassSect = ClassD.iClassSect;

	// 6.4 Lateral-torsional buckling of composite beams

	double dX_LT = 0.0; // reduction factor for lateral-torsional buckling to the relative slenderness Lamda_LT
	if(iClassSect == 1 || iClassSect==2)
	dM_Rd = dMpl_Rd;
	else if(iClassSect == 3)
	dM_Rd = dMel_Rd;
	else if(iClassSect == 4)
	dM_Rd = dMel_Rd;
	else ASSERT(0);
	*/
	// Lateral torsional bucking curves-General case, EN 1993-1-1, 6.3.2.2, 61p, 

	//double dLamda_LT = 0.0;
	double dL = MembPosi.dLu; // span length between the rigid supports. L은 어디서 입력 받는가?.
	double db = PltbBase.dMy >= 0.0 ? dB1 : dB2;
	double dtf = PltbBase.dMy >= 0.0 ? dTf1 : dTf2;
	//double dAwc = 0.0;
	//double dAf = 0.0;
	// Awc는 Class 4 일 경우는 effective Area를 고려. 
	//if(dCzm_c<dH-dTF1)
	//{ 
	//  dAwc = dMy>=0.0 ? (dH-dTF1-dCzm_c)*dTw : (dCzm_c-dTF2)*dTw;
	//  dAf = dMy>=0.0 ? dB1*dTF1 : dB2*dTF2;
	//}
	//else
	//{
	//  dAwc = dMy>=0.0 ? 0.0 : dHw*dTw;
	//  dAf = dMy>=0.0 ? 0.0 : dB2*dTF2;
	//}

	//double dV1 = PltbBase.dV1; // member개념으로 요소의 I단 전단력
	//double dV2 = PltbBase.dV2; // J단 전단력
	//double dM1 = PltbBase.dM1; // I 단 모멘트
	//double dM2 = PltbBase.dM2; // J 단 모멘트
	//double dCd = LtbdPosi.dCd; // spring stiffness
	//double dl = LtbdPosi.dl;  //distance between the springs.  
	//double dcc = dl==0.0 ? 0.0 : dCd/dl;
	//double dgamma = dcc*pow(dL,4)/(dEs*dIyy_c);  
	//double dNu = 0.0; 
	//if(fabs(dV1)>=fabs(dV2))
	//  dNu = dV1==0.0 ? 0.0 : fabs(dV2/dV1);
	//else
	//  dNu = dV2==0.0 ? 0.0 : fabs(dV1/dV2);
	//
	//double dPHI = Get_PHIform(dM1,dM2,dNu);
	//
	//double dm = 0.0;
	//Get_m_Value(dNu,dPHI,dgamma,dm);    
	// Designer's guide to EN 1994-2
	// Eurocode 4: Design of steel and composite structures, (D6.14), 102p    
	//Get_NonDimensionalSlenderness(dL,db,dfy,dEs,dm,dAwc,dAf,dLamda_LT);  

	// 
	fout << _T("    - Mb,Rd") << endl
		<< _T("      L     = ") << Get_String_Format(dL) << m_strUnitMM << endl
		<< _T("      c     = Cd/l = ") << Get_String_Format(PltbBase.dcc) << m_strUnitStress << endl
		<< _T("      gamma = c*L^4/(E*I) = ") << Get_String_Format(PltbBase.dgamma) << endl
		<< endl;

	if ( fabs(PltbBase.dV1) >= fabs(PltbBase.dV2) )
		fout << _T("      mu  = V2/V1 = ") << Get_String_Format(PltbBase.dMu) << endl;
	else
		fout << _T("      mu  = V1/V2 = ") << Get_String_Format(PltbBase.dMu) << endl;

	fout << _T("      PHI = 2(1-M2/M1)/(1+mu) = ") << Get_String_Format(PltbBase.dPHI) << endl
		<< endl;

	fout << _T("      m1  = 1+0.44*(1+mu)*PHI^1.5+(3+2*PHI)*gamma/(350-50*mu) = ") << Get_String_Format(PltbBase.dm1) << endl
		<< _T("      m2  = 1+0.44*(1+mu)*PHI^1.5+(0.195+(0.05+mu/100)*PHI)*gamma^0.5 = ") << Get_String_Format(PltbBase.dm2) << endl
		<< _T("      m   = Min(m1,m2) = ") << Get_String_Format(PltbBase.dm_min) << endl;
	if ( PltbBase.dm_min > 1.0 )
	{
		fout << _T("      m > 1.0 --> m = ") << Get_String_Format(PltbBase.dm) << endl;
	}

	//int iCase = Get_BucklingCurveForCrossSection(dH,db);
	//double dAlpha_LT = Get_ImperfectionFactor(iCase);  
	//double dPHI_LT = Get_PHI_LT(dAlpha_LT, dLamda_LT);
	//dX_LT = Get_X_LT(dPHI_LT,dLamda_LT);
	//
	//double dPHI_LT = 0.5*(1.0+dAlpha*(dLamda_LT-0.2)+pow(dAlpha,2));
	//
	//dX_LT = dPHI_LT+sqrt(dX_LT_Parm1)==0.0 ? 0.0 : 1.0/(dPHI_LT+sqrt(dX_LT_Parm1));
	//if(dX_LT>1.0) 
	//  dX_LT = 1.0;

	// dLamda = 1.103*dL/db*sqrt(dfy/(dEs*dm))*sqrt(1.0+dAwc/(3.0*dAf));

	fout << _T("      Alpha,LT = ") << Get_String_Format(PltbBase.dAlpha_LT) << endl
		<< _T("      Lamda,LT = 1.103*L/b*Root(fy/(Es*m))*Root(1+Awc/(3*Af)) = ") << Get_String_Format(PltbBase.dLamda_LT) << endl
		<< _T("      PHI,LT   = 0.5*(1.0+Alpha,LT*(Lamda,LT-0.2)+Lamda,LT^2) = ") << Get_String_Format(PltbBase.dPHI_LT) << endl
		<< _T("      X,LT     = 1/(PHI,LT+Root(PHI,LT^2-Lamda,LT^2)) = ") << Get_String_Format(PltbBase.dX_LT_org) << endl;
	if ( PltbBase.dX_LT > 1.0 )
	{
		fout << _T("      X,LT > 1.0")
			<< _T("      X,LT     = ") << Get_String_Format(PltbBase.dX_LT) << endl;
	}
	fout << endl;
	//
	fout << _T("      M,Rd     = ") << Get_String_Format(PltbBase.dM_Rd) << m_strUnitMoment << endl
		<< _T("      Mb,Rd    = X,LT*M,Rd = ") << Get_String_Format(PltbBase.dMb_Rd) << m_strUnitMoment << endl
		<< endl;


	// design buckling resistance moment
	//dMb_Rd = dX_LT*dM_Rd;

	// N_Ed  
	//dm = 1.0;
	//dAwc = 0.0; // class 4면 effe Area, 아니면 Plastic NA에 대한 Web의 Area.
	//dAf=dAf;
	//double dLamda_LT_N=dLamda_LT;    
	//double dPHI_LT_N = Get_PHI_LT(dAlpha_LT, dLamda_LT_N);
	//double dX_LT_N = Get_X_LT(dPHI_LT_N, dLamda_LT_N);
	//double dNb_Rd = dX_LT_N*dArea_c*dfyd;
	//
	//double dN_Rat = dNb_Rd==0.0 ? 0.0 : fabs(dN_Ed)/dNb_Rd;
	//double dM_Rat = dMb_Rd==0.0 ? 0.0 : fabs(dM_Ed)/dMb_Rd;
	//double dCombRat = dN_Rat + dM_Rat;

	//
	fout << _T("    - Nb,Rd") << endl
		<< _T("      X,LT,N  = ") << Get_String_Format(PltbBase.dX_LT_N) << endl
		<< _T("      Nb,Rd   = X,LT,N*Area*fyd = ") << Get_String_Format(PltbBase.dNb_Rd) << m_strUnitForce << endl
		<< endl;
	fout << _T("      Combined Ratio = N,Ed/Nb,Rd + M,Ed/Mb,Rd = ") << Get_String_Format(PltbBase.dInterRat) << endl
		<< endl;


	// U frame model 나중에 고려.
	int iAlpha = LtbdPosi.iAlpha;
	//double dMcr=0.0, dMcr_val=0.0;
	//double dk1=0.0, dk2=0.0;
	//double dk2_val1=0.0;
	// double dks=0.0;
	double dEa = dEs; // girder
	double dI2 = CompoSect.Ryy; // girder+steel
	double dv_a = dPoissonS; // Poisson's ratio for Steel
	double dhs = dH - 0.5*(dTf1 + dTf2);
	double da = LtbdPosi.da;

	//dk1 = da==0.0 ? 0.0 : iAlpha*dEa*dI2/da;
	//dk2_val1=4.0*(1.0-dv_a*dv_a)*dhs;
	//dk2 = dk2_val1==0.0 ? 0.0 : dEa*pow(dTw,3)/dk2_val1;
	//dks = dk1+dk2==0.0 ? 0.0 : dk1*dk2/(dk1+dk2);

	//
	fout << _T("    - U frame model") << endl
		<< _T("      Alpha = ") << Get_String_Format(iAlpha) << endl
		<< _T("      hs    = ") << Get_String_Format(dhs) << m_strUnitMM << endl
		<< _T("      a     = ") << Get_String_Format(da) << m_strUnitMM << endl
		<< _T("      Ea    = ") << Get_String_Format(dEa) << m_strUnitStress << endl
		<< _T("      G     = ") << Get_String_Format(dGs) << m_strUnitStress << endl
		<< _T("      I2    = ") << Get_String_Format(dI2) << m_strUnitMM4 << endl
		<< _T("      k1    = Alpha*Ea*I2/a =") << Get_String_Format(PltbBase.dk1) << endl
		<< _T("      k2    = Ea*tw^3/(4*(1-nu,a^2)*hs) =") << Get_String_Format(PltbBase.dk2) << endl
		<< _T("      ks    = k1*k2/(k1+k2) = ") << Get_String_Format(PltbBase.dks) << endl
		<< endl;

	//
	//double dkc=0.0, dkc_val1=0.0, dkc_val2=0.0;  
	//double dC4=PltbBase.dC4;
	//double dI_at=dIxx_g; // St.Venant torsional stiffness of the steel section  
	//double dI_afz=dTF2*pow(dB2,3)/12.0; // ???,  is the flexural stiffness of bottom flange respect the z axis of the steel section
	/*
	double dAa  = dArea_g;
	double dA   = RebarSect.Area; // 전체환산단면.
	double dIay = dIyy_g;
	double dIaz = dIzz_g;
	double dix2 = dAa==0.0 ? 0.0 : (dIay+dIaz)/dAa;
	double dhc  = dH-0.5*(dTf1+dTf2);
	double dSCzm_g = Get_ShearCenter_H(dh,dTF1,dB1,dTF2,dB2); // shear center
	double dZs  = dCzm_g - dSCzm_g; // is the distance between section centre of gravity _T("C") and shear barycentre(positive if shear barycentre is lower than _T("C"))
	double dZc  = dH+dHh+0.5*dtc-dCzm_g;
	double de   = dAa*dZc*(dA-dAa)==0.0 ? 0.0 : dA*dIay/(dAa*dZc*(dA-dAa));
	double dZf  = dIaz==0.0 ? 0.0 : dhs*dI_afz/dIaz;
	double dZj_val2 = Get_Zj_Second_Value(dH,dHw,dTw,dB1,dTF1,dB2,dTF2,dCzm_g,dAa,dIay);
	double dZj = dZs - dZj_val2;
	*/

	fout << _T("      C4     = ") << Get_String_Format(PltbBase.dC4) << endl
		<< _T("      I,at   = ") << Get_String_Format(PltbBase.dI_at) << m_strUnitMM4 << endl
		<< _T("      I,afz  = ") << Get_String_Format(PltbBase.dI_afz) << m_strUnitMM4 << endl
		<< _T("      kc     = ") << Get_String_Format(PltbBase.dkc) << endl
		<< _T("      Mcr    = kc*C4/L*((G*I,at+ks*L^2/pi^2)*Ea*I,afz)^(1/2) =") << Get_String_Format(PltbBase.dMcr) << m_strUnitMoment << endl
		<< endl;
	//


	return TRUE;
}

BOOL CDgnPlateGirder::Detail_TransverseForceResistance(ElemPairK ElemK, BOOL bPosiI)
{
	rptwofstream fout(m_strTestDgnFile, ios::app);

	T_PRTF_D PrtfD;
	if ( !m_pAnalysisResult->GetPrtfResultData(ElemK, PrtfD) ) { return FALSE; }

	T_PRTF_BASE PrtfBase;
	PrtfBase = bPosiI ? PrtfD.PrtfBase[0] : PrtfD.PrtfBase[1];

	BOOL bSect = TRUE, bMatl = TRUE, bBcdt = TRUE;
	CPG_SECT_ELEM SectElem;
	if ( !Get_CpgSectElem(ElemK, SectElem) ) { bSect = FALSE; }

	CPG_MATL_ELEM MatlElem;
	if ( !Get_CpgMatlElem(ElemK, MatlElem) ) { bMatl = FALSE; }

	CPG_BCDT_ELEM BcdtElem;
	BcdtElem.Initialize();
	if ( !Get_CpgBcdtElem(ElemK, BcdtElem) ) { bBcdt = FALSE; }
	//
	CPG_BCDT_POSI& BcdtPosi = bPosiI ? BcdtElem.BcdtPosi[0] : BcdtElem.BcdtPosi[1];

	// 1. Section.  
	T_SECT_D& SectD = SectElem.SectD;
	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTf1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dTf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

	// 2. Material
	T_MATD_D& MatdD = MatlElem.MatdD;
	int iMatClass = MatlElem.iMatClass;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfu = MatdD.Data1.Design.S_Fu;
	double dEs = MatdD.Data1.Analysis.Elast;
	double dPoissonS = MatdD.Data1.Analysis.Poisson;
	double dGs = dEs / (1.0 + dPoissonS);
	double dfck = MatlElem.dfck;
	double dfcm = MatlElem.dfcm;
	double dfctm = MatlElem.dfctm;
	double dEcm = MatlElem.dEcm;
	double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
	double dErs = dEs;
	double dgam_M0 = m_dGamma_M0;
	double dgam_M1 = m_dGamma_M1;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;
	double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk / dgam_sr;
	double dfyt = Get_fy4Thick(dfy, dfy1, dTf1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTf2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;

	// Load combination
	LPCTSTR aType[] = { _T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN),
		_LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
		_LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN) };

	// Lcom
	CString strLoadComb;
	T_LCOM_D LcomData;
	if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, PrtfBase.OrgLcomK, LcomData) )
		strLoadComb.Format(_T("%s(%s)"), LcomData.LoadCombName, aType[PrtfBase.iMax]);
	else
		strLoadComb = _T("-");

	//
	T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
	T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
	GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

	T_SECT_STIFFNESS StiffData;
	StiffData.Initialize();
	if ( !bTapered )
		StiffData = SectD.SectBefore.Stiffness;
	else
		StiffData = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;

	fout << endl
		<< _T("--------------------------------------------------------------------------------------") << endl
		<< endl
		<< _T("  *. Resistance to Transverse Force") << endl
		<< endl
		<< _T("    - Load Combination     = ") << strLoadComb << endl
		<< _T("      N,Ed    = ") << Get_String_Format(PrtfBase.dN_Ed) << m_strUnitForce << endl
		<< _T("      My,Ed    = ") << Get_String_Format(PrtfBase.dMy_Ed) << m_strUnitMoment << endl
		<< _T("      Mz,Ed    = ") << Get_String_Format(PrtfBase.dMz_Ed) << m_strUnitMoment << endl
		<< _T("      F,Ed     = ") << Get_String_Format(PrtfBase.dF_Ed) << m_strUnitForce << endl
		<< endl;


	T_SECT_STIFFNESS CompSect;
	CompSect = PrtfBase.dMy_Ed >= 0.0 ? RebarSect : NonConSect;

	///////////////////////////////////////////////////
	T_COMF_KEY ComfKey;
	ComfKey.DgnLcomK = PrtfBase.LcomK;
	ComfKey.ElemK = ElemK.first;
	ComfKey.nPos = bPosiI ? 0 : 1;

	CPG_SECT_EFFE EffeSect;
	EffeSect.Initialize();
	//    
	m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCpgSectEffeResult(ElemK, bPosiI, PrtfBase.LcomK, EffeSect);

	T_SECT_STIFFNESS EffeStlSect;
	EffeStlSect.Initialize();
	T_SECT_STIFFNESS EffeComSect;
	EffeComSect.Initialize();

	EffeStlSect = EffeSect.EffeStlSect;
	EffeComSect = EffeSect.EffeComSect;

	// Before
	double dStl_Iyy = EffeStlSect.Ryy;
	double dStl_Czm = EffeStlSect.Czm;
	double dStl_Czp = EffeStlSect.Czp;
	double dStl_zDim = dStl_Czp + dStl_Czm;
	double dStl_Stop = dStl_Czp;
	// Composite
	double dIyy = EffeComSect.Ryy;
	double dIzz = EffeComSect.Rzz;
	double dCzm = EffeComSect.Czm;
	double dCzp = EffeComSect.Czp;
	double dzDim = dCzm + dCzp;
	double dStop = dH - dCzm;
	double dy1 = EffeComSect.dx1;
	double dy2 = EffeComSect.dx1;
	double dy3 = EffeComSect.dx3;
	double dy4 = EffeComSect.dx4;
	double dz1 = EffeComSect.dy1;
	double dz2 = EffeComSect.dy1;
	double dz3 = EffeComSect.dy3;
	double dz4 = EffeComSect.dy4;

	double dAeff = 0.0;
	//double dey_N = CompSect.Cym-EffeComSect.Cym;
	//double dez_N = CompSect.Czm-EffeComSect.Czm;
	double dEta1[4] = { 0., 0., 0., 0., };
	double dEta1_max = 0.0;
	double dWy_eff[4]; // 0=TL, 1=TR, 2=BR, 3=BL
	double dWz_eff[4];
	//double dEta1_fx = dgam_M0==0.0 || dfy*dAeff==0.0 ? 0.0 : dN_Ed/(dfy*dAeff/dgam_M0);
	dWy_eff[0] = dy1 == 0.0 ? 0.0 : dIyy / dy1;
	dWy_eff[1] = dy2 == 0.0 ? 0.0 : dIyy / dy2;
	dWy_eff[2] = dy3 == 0.0 ? 0.0 : dIyy / dy3;
	dWy_eff[3] = dy4 == 0.0 ? 0.0 : dIyy / dy4;
	//
	dWz_eff[0] = dz1 == 0.0 ? 0.0 : dIzz / dz1;
	dWz_eff[1] = dz2 == 0.0 ? 0.0 : dIzz / dz2;
	dWz_eff[2] = dz3 == 0.0 ? 0.0 : dIzz / dz3;
	dWz_eff[3] = dz4 == 0.0 ? 0.0 : dIzz / dz4;

	fout << _T("    - Eta1, 1993-1-5, (4.15) ") << endl
		<< _T("      ey,N      = ") << Get_String_Format(PrtfBase.dey_N) << m_strUnitMM << endl
		<< _T("      ez,N      = ") << Get_String_Format(PrtfBase.dez_N) << m_strUnitMM << endl
		<< _T("      Wy,eff,TL = ") << Get_String_Format(dWy_eff[0]) << m_strUnitMM3 << endl
		<< _T("      Wy,eff,TR = ") << Get_String_Format(dWy_eff[1]) << m_strUnitMM3 << endl
		<< _T("      Wy,eff,BR = ") << Get_String_Format(dWy_eff[2]) << m_strUnitMM3 << endl
		<< _T("      Wy,eff,BL = ") << Get_String_Format(dWy_eff[3]) << m_strUnitMM3 << endl
		<< _T("      Wz,eff,TL = ") << Get_String_Format(dWz_eff[0]) << m_strUnitMM3 << endl
		<< _T("      Wz,eff,TR = ") << Get_String_Format(dWz_eff[1]) << m_strUnitMM3 << endl
		<< _T("      Wz,eff,BR = ") << Get_String_Format(dWz_eff[2]) << m_strUnitMM3 << endl
		<< _T("      Wz,eff,BL = ") << Get_String_Format(dWz_eff[3]) << m_strUnitMM3 << endl
		<< endl;

	/*
	if(dgam_M0>0)
	{
	// Compression(-), Tension(+)
	for(int i=0; i<4; i++)
	{
	double dRat_y = dfy*dWy_eff[i]==0.0 ? 0.0 : (-1.)*(dMy_Ed+dN_Ed*dey_N)/(dfy*dWy_eff[i]/dgam_M0);
	double dRat_z = dfy*dWz_eff[i]==0.0 ? 0.0 : (-1.)*(dMz_Ed+dN_Ed*dez_N)/(dfy*dWz_eff[i]/dgam_M0);
	dEta1[i] = dEta1_fx + dRat_y + dRat_z;
	if(i==0)
	{
	dEta1_max = fabs(dEta1[0]);
	}
	else
	{
	if(dEta1_max<fabs(dEta1[i]))
	{
	dEta1_max = fabs(dEta1[i]);
	}
	}
	}
	}
	*/

	fout << _T("      Eta1,TL   = ") << Get_String_Format(PrtfBase.dEta1_p[0]) << endl
		<< _T("      Eta1,TR   = ") << Get_String_Format(PrtfBase.dEta1_p[1]) << endl
		<< _T("      Eta1,BR   = ") << Get_String_Format(PrtfBase.dEta1_p[2]) << endl
		<< _T("      Eta1,BL   = ") << Get_String_Format(PrtfBase.dEta1_p[3]) << endl
		<< _T("      Eta1      = ") << Get_String_Format(PrtfBase.dEta1) << endl
		<< endl;

	// EN 1993-1-5, 26page.
	int iLoadAppType = BcdtPosi.iLoadAppType;
	double da = BcdtPosi.da;
	double dSs = BcdtPosi.dSs;
	double dC = BcdtPosi.dC;

	//
	/*
	double dIsl1 = 0.0;
	double db1 = 0.0;
	BOOL bExistLsti = arLstiPosi.GetSize()>0 ? TRUE : FALSE;
	if(bExistLsti)
	{
	CPG_LSTI_DATA LstiD = arLstiPosi[0];
	double dh_ls = LstiD.dWidth;
	double dt_ls = LstiD.dt;
	double dI_ls = 0.0;
	db1 = LstiD.dz-LstiD.dt*0.5;
	if(!Get_IslOfLongiStiff(1,dh_ls, dt_ls, dTw, dEpsi, dIsl1))
	dIsl1=0.0;
	}
	*/

	fout << _T("    - Eta2, 1993-1-5, (6.14) ") << endl
		<< _T("      Calc. k,F") << endl
		<< _T("      a  = ") << Get_String_Format(da) << m_strUnitMM << endl
		<< _T("      Ss = ") << Get_String_Format(dSs) << m_strUnitMM << endl;


	double db1 = PrtfBase.db1;
	double dRat = da == 0.0 ? 0.0 : dh / da;
	double db1hw = dh == 0.0 ? 0.0 : db1 / dh;
	double db1a = da == 0.0 ? 0.0 : db1 / da;

	// 0=Type(a), 1=Type(b), 2=Type(c)
	if ( iLoadAppType == 0 )
	{
		fout << _T("      Type(a)") << endl;

		if ( (0.05 <= db1hw && db1hw <= 0.3) && (db1a <= 0.3) )
		{
			/*
			double dGamma_s = dh*dTw==0.0 ? 0.0 : 10.9*dIsl1/(dh*pow(dTw,3));
			double dGamma_s_lim = dh*da==0.0 ? 0.0 : 13.0*pow(da/dh,3)+210*(0.3-db1/da);
			if(dGamma_s>dGamma_s_lim)
			dGamma_s = dGamma_s_lim;
			dk_F = 6.0+2.0*pow(dh/da,2)+(5.44*db1/da-0.21)*sqrt(dGamma_s);
			*/
			//
			fout << _T("      0.05<b1/hw<0.3 , b1/a<0.3") << endl
				<< _T("      Isl,1   = ") << Get_String_Format(PrtfBase.dIsl1) << m_strUnitMM4 << endl
				<< _T("      gamma,s = ") << Get_String_Format(PrtfBase.dgamma_s) << m_strUnitMM4 << endl
				<< _T("      k,F     = 6+2*[hw/a]^2+[5.44*b1/a-0.21]*Root(gamma,s) = ") << Get_String_Format(PrtfBase.dk_F) << endl
				<< endl;
		}
		else
		{
			//dk_F = 6.0+2.0*pow(dRat,2);
			fout << _T("      k,F   = 6+2*(hw/a)^2 = ") << Get_String_Format(PrtfBase.dk_F) << endl;
		}
	}
	else if ( iLoadAppType == 1 )
	{
		//double dRat = da==0.0 ? 0.0 : dh/da;
		//dk_F = 3.5+2.0*pow(dRat,2);
		fout << _T("      Type(b)") << endl
			<< _T("      k,F   = 3.5+2*(hw/a)^2 = ") << Get_String_Format(PrtfBase.dk_F) << endl;
	}
	else if ( iLoadAppType == 2 )
	{
		//double dRat = dh==0.0 ? 0.0 : (dSs+dC)/dh;
		//dk_F = 2.0+6.0*dRat;
		fout << _T("      Type(c)") << endl
			<< _T("      k,F   = 2+6*(Ss+c)/hw<6 = ") << Get_String_Format(PrtfBase.dk_F) << endl;
	}
	else
	{
	}
	fout << endl;

	//!/ (6.3) 과 (6.9) 식에 모순이 있다.
	double dX_F = 0.0; // reduction factor
	double dl_y = 0.0; // effective loaded length

	// effective loaded length
	double dbf = dB1;
	double dtf = dTf1;
	//double dm1 = dfyd_w*dTw==0.0 ? 0.0 : dfyd_t*dbf/(dfyd_w*dTw);
	//double dm2 = 0.0;

	//double dRat = dtf==0.0 ? 0.0 : dh/dtf;
	//dm2 = 0.02*pow(dRat,2);
	/*
	if(dLamda_F>0.5)
	{
	double dRat = dtf==0.0 ? 0.0 : dh/dtf;
	dm2 = 0.02*pow(dRat,2);
	}
	else
	{
	dm2 = 0.0;
	}
	*/

	fout << _T("    - Calc. ly") << endl
		<< _T("      m1  = ") << Get_String_Format(PrtfBase.dm1) << endl
		<< _T("      m2  = ") << Get_String_Format(PrtfBase.dm2) << endl;

	// 
	if ( iLoadAppType == 0 || iLoadAppType == 1 )
	{
		//dl_y = dSs + 2.0*dtf*(1.0+sqrt(dm1+dm2));
		if ( iLoadAppType == 0 )
			fout << _T("      Type(a)") << endl;
		else
			fout << _T("      Type(b)") << endl;
		fout << _T("      ly  = Ss+2*tf*(1+Root(m1+m2)) = ") << Get_String_Format(PrtfBase.dly) << m_strUnitMM << endl;
	}
	else if ( iLoadAppType == 2 )
	{
		/*
		double dl_e = 2.0*dfyd_w*dh==0.0 ? 0.0 : dk_F*dEs*pow(dTw,2)/(2.0*dfyd_w*dh);
		if(dl_e>dSs+dC)
		dl_e = dSs+dC;
		double dl_y1_val = dtf==0.0 ? dm1/2.0+dm2 : dm1/2.0+pow(dl_e/dtf,2)+dm2;
		double dl_y1 = dl_e+dtf*sqrt(dl_y1_val);
		double dl_y2 = dl_e+dtf*sqrt(dm1+dm2);
		dl_y = min(dl_y1,dl_y2);
		*/

		fout << _T("      Type(c)") << endl
			<< _T("      le   = k,F*E*tw^2/(2*fyw*hw) = ") << Get_String_Format(PrtfBase.dle) << m_strUnitMM << endl
			<< _T("      ly1  = le+tf*Root(m1/2+(le/tf)^2+m2) = ") << Get_String_Format(PrtfBase.dly1) << m_strUnitMM << endl
			<< _T("      ly2  = le+tf*Root(m1+m2) = ") << Get_String_Format(PrtfBase.dly2) << m_strUnitMM << endl
			<< _T("      ly   = min(ly1,ly2) = ") << Get_String_Format(PrtfBase.dly) << m_strUnitMM << endl
			<< endl;
	}
	else
	{
	}

	// reduction factor
	//double dFcr = dh==0.0 ? 0.0 : 0.9*dk_F*dEs*pow(dTw,3)/dh;
	//double dLamda1 = dFcr==0.0 ? 0.0 : dl_y*dTw*dfyd_w/dFcr;
	//double dLamda_F = dLamda1<0.0 ? 0.0 : sqrt(dLamda1);
	//dX_F = dLamda_F==0.0 ? 0.0 : 0.5/dLamda_F;
	//if(dX_F>1.0)
	//    dX_F = 1.0;
	//
	//double dLeff = dX_F*dl_y;
	//double dF_Rd = dgam_M1==0.0 ? 0.0 : dfyd_w*dLeff*dTw/dgam_M1;
	//  
	//double dEta2 = dF_Rd==0.0 ? 0.0 : fabs(dF_Ed)/dF_Rd;

	fout << endl
		<< _T("    - Calc. F,Rd") << endl
		<< _T("      Fcr     = 0.9*k,F*E*tw^3/hw = ") << Get_String_Format(PrtfBase.dFcr) << m_strUnitForce << endl
		<< _T("      Lamda,F = Root(ly*tw*fyw/Fcr) = ") << Get_String_Format(PrtfBase.dLamda_F) << endl
		<< _T("      X,F     = 0.5/Lamda,F<1.0 = ") << Get_String_Format(PrtfBase.dX_F) << endl
		<< _T("      L,eff   = X,F*ly = ") << Get_String_Format(PrtfBase.dL_eff) << m_strUnitMM << endl
		<< _T("      F,Rd    = fyw*L,eff*tw/gamma,M1 = ") << Get_String_Format(PrtfBase.dF_Rd) << m_strUnitForce << endl
		<< _T("      Eta,2    = F,Ed/F,Rd = ") << Get_String_Format(PrtfBase.dEta2) << endl
		<< endl;

	//double dInterRat = dEta2 + 0.8*dEta1_max; // 
	fout << _T("      Eta2 + 0.8*Eta1 < 1.4 = ") << Get_String_Format(PrtfBase.dInterRat) << endl
		<< endl;

	return TRUE;
}

BOOL CDgnPlateGirder::Detail_LongitudinalShearResistance(ElemPairK ElemK, BOOL bPosiI)
{
	rptwofstream fout(m_strTestDgnFile, ios::app);

	T_PGLS_D PglsD;
	if ( !m_pAnalysisResult->GetPglsResultData(ElemK, PglsD) ) { return FALSE; }

	T_PGLS_BASE PglsBase;
	PglsBase = bPosiI ? PglsD.PglsBase[0] : PglsD.PglsBase[1];

	BOOL bSect = TRUE, bMatl = TRUE, bScon = TRUE;
	CPG_SECT_ELEM SectElem;
	if ( !Get_CpgSectElem(ElemK, SectElem) ) { bSect = FALSE; }

	CPG_MATL_ELEM MatlElem;
	if ( !Get_CpgMatlElem(ElemK, MatlElem) ) { bMatl = FALSE; }

	CPG_SCON_ELEM SconElem;
	SconElem.Initialize();
	if ( !Get_CpgSconElem(ElemK, SconElem) ) { bScon = FALSE; }

	// Shear Connector.
	CPG_SCON_POSI SconPosi = bPosiI ? SconElem.SconPosi[0] : SconElem.SconPosi[1];

	// 1. Section.  
	T_SECT_D SectD = SectElem.SectD;
	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTf1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dTf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

	// 2. Material
	T_MATD_D& MatdD = MatlElem.MatdD;
	int iMatClass = MatlElem.iMatClass;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfu = MatdD.Data1.Design.S_Fu;
	double dEs = MatdD.Data1.Analysis.Elast;
	double dPoissonS = MatdD.Data1.Analysis.Poisson;
	double dGs = dEs / (1.0 + dPoissonS);
	double dfck = MatlElem.dfck;
	double dfcm = MatlElem.dfcm;
	double dfctm = MatlElem.dfctm;
	double dEcm = MatlElem.dEcm;
	double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
	double dErs = dEs;
	double dgam_M0 = m_dGamma_M0;
	double dgam_M1 = m_dGamma_M1;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;
	double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk / dgam_sr;
	double dfyt = Get_fy4Thick(dfy, dfy1, dTf1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTf2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;

	// Load combination
	LPCTSTR aType[] = { _T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN),
		_LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
		_LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN) };

	// Lcom
	CString strLoadComb;
	T_LCOM_D LcomData;
	if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, PglsBase.OrgLcomK, LcomData) )
		strLoadComb.Format(_T("%s(%s)"), LcomData.LoadCombName, aType[PglsBase.iMax]);
	else
		strLoadComb = _T("-");

	//
	T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
	T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
	GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

	//
	T_PGBR_D PgbrD;
	PgbrD.Initialize();
	T_PGBR_LCOM PgbrLcomD;
	PgbrLcomD.Initialize();
	int iDgnLcomNo = PglsBase.LcomK;
	int iMaxMin = 0;
	if ( !m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgbrLcomD) )
	{
	}

	T_PGBR_BASE PgbrBase;
	PgbrBase = bPosiI ? PgbrLcomD.PgbrBase[0] : PgbrLcomD.PgbrBase[1];
	//dMpl_Rd = PgbrBase.dMpl_Rd;

	double dMpl_Rd = PgbrBase.dMpl_Rd;
	double dMel_Rd = PgbrBase.dMel_Rd;
	double dNc_el = PgbrBase.dNc_el;
	double dNc_f = PgbrBase.dNc_f;
	//double dM_Ed  = dMy;

	fout << endl
		<< _T("--------------------------------------------------------------------------------------") << endl
		<< endl
		<< _T("  *. Resistance to Longitudinal Shear") << endl
		<< endl
		<< _T("    - Load Combination     = ") << strLoadComb << endl
		<< _T("      Mpl,Rd  = ") << Get_String_Format(dMpl_Rd) << m_strUnitMoment << endl
		<< _T("      Mel,Rd  = ") << Get_String_Format(dMel_Rd) << m_strUnitMoment << endl
		<< _T("      Nc,el   = ") << Get_String_Format(dNc_el) << m_strUnitForce << endl
		<< _T("      Nc,d    = ") << Get_String_Format(dNc_f) << m_strUnitForce << endl
		<< _T("      M,Ed    = ") << Get_String_Format(PglsBase.dMy) << m_strUnitMoment << endl
		<< _T("      V,Ed    = ") << Get_String_Format(PglsBase.dv_Ed) << m_strUnitMoment << endl
		<< endl;

	// EN 1994-2, 6.6 Shear connection, 46page
	int iNum = SconPosi.iNum;
	double dhsc = SconPosi.dhsc;
	double dfu_sc = SconPosi.dfu;
	double dDia_sc = SconPosi.dDia;
	//double dSt_sc  = SconPosi.dSt;
	double dSpace = SconPosi.dSpace;
	double dAlpha = 0.0;
	double dRat_sc = dDia_sc == 0.0 ? 0.0 : dhsc / dDia_sc;
	//
	CString strNumSC; strNumSC.Format(_T("%d"), iNum);
	fout << _T("    - Calc. P,Rd") << endl
		<< _T("      Num.  = ") << strNumSC << endl
		<< _T("      h,sc  = ") << Get_String_Format(dhsc) << m_strUnitMM << endl
		<< _T("      fu    = ") << Get_String_Format(dfu_sc) << m_strUnitStress << endl
		<< _T("      d     = ") << Get_String_Format(dDia_sc) << m_strUnitMM << endl
		<< _T("      Space = ") << Get_String_Format(dSpace) << m_strUnitMM << endl
		<< endl;

	fout << _T("      P,Rd1  = (0.8*fu*PI*d^2/4)/gamma,V            = ") << Get_String_Format(PglsBase.dP_Rd1) << m_strUnitForce << endl;

	if ( 3.0 <= dRat_sc && dRat_sc <= 4.0 )
	{
		//dAlpha = 0.2*(dRat_sc+1.0);
		fout << _T("      3 < h,sc/d < 4") << endl
			<< _T("      Alpha = 0.2*(h,sc/d+1) = ") << Get_String_Format(PglsBase.dAlpha) << endl;
	}
	else if ( dRat_sc > 4.0 )
	{
		//dAlpha = 1.0;
		fout << _T("      h,sc/d > 4") << endl
			<< _T("      Alpha = ") << Get_String_Format(PglsBase.dAlpha) << endl;
	}
	else
	{
		fout << _T("      3 > h,sc/d") << endl
			<< _T("      Alpha = 0.2*(h,sc/d+1) = ") << Get_String_Format(PglsBase.dAlpha) << endl;
		/*
		dAlpha = 0.2*(dRat_sc+1.0);
		if(dAlpha>1.0)
		dAlpha = 1.0;
		*/
	}
	//double dP_Rd = dgam_V==0.0 ? 0.0 : 0.29*dAlpha*pow(dDia_sc,2)*sqrt(dfck*dEcm)/dgam_V;

	fout << _T("      P,Rd2  = 0.29*Alpha*d^2*Root(fck*Ecm)/gamma,V = ") << Get_String_Format(PglsBase.dP_Rd2) << m_strUnitForce << endl;
	fout << _T("      P,Rd   = min(P,Rd1 , P,Rd2) = ") << Get_String_Format(PglsBase.dP_Rd) << m_strUnitForce << endl;

	/*
	double dAz_Slab  = 0.0;
	double dAz_Hunch = 0.0;
	double dErat = dEcm == 0.0 ? 0.0 : dEs/dEcm;   // Es/Ec, Section에 있는 값.
	double dZbar = 0.0; // 철근 고려 안한. Original Section Stiffeness.
	double dIyy_Org = 0.0;

	if(!bTapered)
	{
	dZbar = SectD.SectAfter.Stiffness.Czm;
	dIyy_Org = SectD.SectAfter.Stiffness.Ryy;
	}
	else
	{
	dZbar = bPosiI ? SectD.SectAfter.SectI.Stiffness.Czm : SectD.SectAfter.SectJ.Stiffness.Czm;
	dIyy_Org = bPosiI ? SectD.SectAfter.SectI.Stiffness.Ryy : SectD.SectAfter.SectJ.Stiffness.Ryy;
	}

	dAz_Slab  = dErat==0.0 ? 0.0 : (dBc*dtc/dErat)*dtc*((dHt-0.5*dtc)-dZbar);
	dAz_Hunch = dErat==0.0 ? 0.0 : (dB1*dHh/dErat)*((dHt-dtc-0.5*dHh)-dZbar);
	double dAzI = dIyy_Org==0.0 ? 0.0 : (dAz_Slab+dAz_Hunch)/dIyy_Org;
	*/

	//
	//double dv_L_Ed = fabs(dV_Ed)*dAzI; // Shear flow
	//double dv_L_Rd = dSpace==0.0 ? 0.0 : dP_Rd*((double)iNum)/dSpace;
	//

	fout << endl
		<< _T("    - Calc. v,L,Ed , v,L,Rd , V,L,Ed") << endl
		<< _T("      A*z/I  = ") << Get_String_Format(PglsBase.dAzI) << _T(" mm^-1") << endl
		<< _T("      v,L,Ed = V,Ed*(A*z/I) = ") << Get_String_Format(PglsBase.dv_L_Ed) << m_strUnitForce2 << endl
		<< _T("      v,L,Rd = P,Rd*Num/Space = ") << Get_String_Format(PglsBase.dv_L_Rd) << m_strUnitForce2 << endl
		<< endl;

	double dV_L_Ed = 0.0;
	if ( PglsBase.dMy >= 0.0 )
	{
		//dV_L_Ed = dMpl_Rd-dMel_Rd==0.0 ? 0.0 : (dNc_f-dNc_el)*(fabs(dM_Ed)-dMel_Rd)/(dMpl_Rd-dMel_Rd);
		fout << _T("      M,Ed > 0 ") << endl
			<< _T("      V,L,Ed = (Nc,f-Nc,el)*(M,Ed-Mel,Rd)/(Mpl,Rd-Mel,Rd) = ") << Get_String_Format(PglsBase.dV_L_Ed) << m_strUnitForce << endl;
	}
	else
	{
		//dV_L_Ed = 0.0;
		fout << _T("      M,Ed < 0 ") << endl
			<< _T("      V,L,Ed = ") << Get_String_Format(PglsBase.dV_L_Ed) << m_strUnitForce << endl;
	}

	/*
	double de_d = 0.0; // e_d = 0, b_eff = Bc defined in Section Data, 07.06.** 답변메일 내용으로 반영
	double dLength_v = de_d + dBc; // EN 1994-2, 6.6.2.3, Figure 6.12, 49p~51p 참고.
	double dv_L_Ed2 = dLength_v==0.0 ? 0.0 : dV_L_Ed / dLength_v;
	//
	double dv_Ed=dv_L_Ed/(2*dtc);
	double dnu = 0.6*(1.0-dfck/250.0);
	double dTheta_f = m_dPi/180.0*45.0; // 45로 가정
	double dv_Ed_lim =  dnu*dfcd*sin(dTheta_f)*cos(dTheta_f);// Designers' guide to EN 1994-2, 128p
	*/
	fout << _T("      v,Ed = v,L,Ed/(2*tc) = ") << Get_String_Format(PglsBase.dv_Ed) << m_strUnitForce2 << endl
		<< endl;

	return TRUE;
}

BOOL CDgnPlateGirder::Detail_FatigueResistance(ElemPairK ElemK, BOOL bPosiI)
{
	rptwofstream fout(m_strTestDgnFile, ios::app);

	T_PGRF_D PgrfD;
	if ( !m_pAnalysisResult->GetPgrfResultData(ElemK, PgrfD) ) return FALSE;

	T_PGRF_BASE PgrfBase;
	PgrfBase = bPosiI ? PgrfD.PgrfBase[0] : PgrfD.PgrfBase[1];

	BOOL bSect = TRUE, bMatl = TRUE, bScon = TRUE, bFrdt = TRUE;
	CPG_SECT_ELEM SectElem;
	if ( !Get_CpgSectElem(ElemK, SectElem) ) bSect = FALSE;

	CPG_MATL_ELEM MatlElem;
	if ( !Get_CpgMatlElem(ElemK, MatlElem) ) bMatl = FALSE;

	CPG_SCON_ELEM SconElem;
	SconElem.Initialize();
	if ( !Get_CpgSconElem(ElemK, SconElem) ) bScon = FALSE;
	// Shear Connector.
	CPG_SCON_POSI SconPosi = bPosiI ? SconElem.SconPosi[0] : SconElem.SconPosi[1];

	CPG_FRDT_ELEM FrdtElem;
	FrdtElem.Initialize();
	if ( !Get_CpgFrdtElem(ElemK, FrdtElem) ) bFrdt = FALSE;
	// Lamda_v2, Lamda_v4
	CPG_FRDT_POSI FrdtPosi;
	FrdtPosi = bPosiI ? FrdtElem.FrdtPosi[0] : FrdtElem.FrdtPosi[1];

	// 1. Section.  
	T_SECT_D& SectD = SectElem.SectD;
	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTf1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dTf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

	// 2. Material
	T_MATD_D& MatdD = MatlElem.MatdD;
	int iMatClass = MatlElem.iMatClass;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfu = MatdD.Data1.Design.S_Fu;
	double dEs = MatdD.Data1.Analysis.Elast;
	double dPoissonS = MatdD.Data1.Analysis.Poisson;
	double dGs = dEs / (1.0 + dPoissonS);
	double dfck = MatlElem.dfck;
	double dfcm = MatlElem.dfcm;
	double dfctm = MatlElem.dfctm;
	double dEcm = MatlElem.dEcm;
	double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
	double dErs = dEs;
	double dgam_M0 = m_dGamma_M0;
	double dgam_M1 = m_dGamma_M1;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;
	double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk / dgam_sr;
	double dfyt = Get_fy4Thick(dfy, dfy1, dTf1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTf2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;

	// Load combination
	LPCTSTR aType[] = { _T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN),
		_LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
		_LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN) };

	// Lcom
	CString strLoadComb;
	T_LCOM_D LcomData;
	if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, PgrfBase.OrgLcomK, LcomData) )
		strLoadComb.Format(_T("%s(%s)"), LcomData.LoadCombName, aType[PgrfBase.iMax]);
	else
		strLoadComb = _T("-");

	///////////////////////////////////////////////////

	//
	T_PGLS_D PglsD;
	PglsD.Initialize();
	int iDgnLcomNo = PgrfBase.LcomK;
	int iMaxMin = 0;
	if ( !m_pAnalysisResult->GetCpgLongitudinalShearResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PglsD) )
	{
	}

	T_PGLS_BASE PglsBase;
	PglsBase = bPosiI ? PglsD.PglsBase[0] : PglsD.PglsBase[1];

	//
	fout << endl
		<< _T("--------------------------------------------------------------------------------------") << endl
		<< endl
		<< _T("  *. Resistance to Fatigue") << endl
		<< endl
		<< _T("    - Load Combination     = ") << strLoadComb << endl
		<< _T("      My   = ") << Get_String_Format(PgrfBase.dMy) << m_strUnitMoment << endl
		<< endl;

	/*
	// EN 1994-2, 6.6 Shear connection, 46page
	int iNum = SconPosi.iNum;
	double dhsc    = SconPosi.dhsc;
	double dfu_sc  = SconPosi.dfu;
	double dDia_sc = SconPosi.dDia;
	double dSt_sc  = SconPosi.dSt;
	double dSpace  = SconPosi.dSpace;
	double dAlpha = 0.0;

	double dv_L_Ed = PglsBase.dV_L_Ed;
	double dDel_Tau=0.0;
	double dLamda_v=0.0;
	double dLamda_v1=0.0;
	double dLamda_v2=FrdtPosi.dLamda_v2; // 입력 받는것으로.
	double dLamda_v3=0.0;
	double dLamda_v4=FrdtPosi.dLamda_v4;
	double dDel_Tau_E_2=0.0;
	double dDel_Tau_c=0.0;
	double dRatio=0.0;

	// The range of shear force per connector
	double dFsc = iNum==0.0 ? 0.0 : dv_L_Ed*dSpace/((double)iNum);
	double dA_sc = m_dPi*pow(dDia_sc,2)/4.0;
	dDel_Tau = dA_sc==0.0 ? 0.0 : dFsc/dA_sc;
	// EN 1994-2, 6.8.6.2 Shear connection, 76page
	// EN 1993-2, 9.5.2 Damage equivalence factors λ for road bridges, 42page
	dLamda_v1 = 1.55; // // 6.8.6.2(4), 76page
	double dQ0 = 480.0; // 480 kN
	double dN0 = 0.5*10E6;
	double dN_Obs = 0.5*10E6; // The UK's National Annex to EN 1991-2 may modify this value
	double dSigma_niQi5 = 8.051*10E18;
	double dSigma_ni = 1.0*10E6;
	double dQm1 = pow(dSigma_niQi5/dSigma_ni, 1./5.); //kN
	//dLamda_v2 = dQ0==0.0 || dN0==0.0 ? 0.0 : dQm1/dQ0*pow((dN_Obs/dN0),1./5.);
	//
	dLamda_v3 = pow(dt_Ld/100.0, 1./5.);
	//
	//dLamda_v4 = 1.0; // ?? EN 1993-2, 9.5.2(6), 44page
	//
	dLamda_v = dLamda_v1*dLamda_v2*dLamda_v3*dLamda_v4;
	//
	dDel_Tau_E_2 = dLamda_v*dDel_Tau;
	// EN 1994-2, 6.8.3(3), 72page.
	dDel_Tau_c = 90.0; // is the reference value at Nc=2*10^6 cycles with Δτ_c equal to 90 N/mm^2
	//
	dRatio = dgam_Mfs==0.0 || dDel_Tau_c==0.0 ? dgam_Ff*dDel_Tau_E_2/m_dZero : dgam_Ff*dDel_Tau_E_2/(dDel_Tau_c/dgam_Mfs);
	dRatio = fabs(dRatio);
	if(dRatio<=1.0)
	PgrfBase.bOK = TRUE;
	else
	PgrfBase.bOK = FALSE;
	*/
	//
	fout << _T("    - Calc. Delta,tau,E,2") << endl
		<< _T("      F,sc         = ") << Get_String_Format(PgrfBase.dFsc) << m_strUnitForce << endl
		<< _T("      A,sc         = ") << Get_String_Format(PgrfBase.dAsc) << m_strUnitMM2 << endl
		<< _T("      Delta,tau    = F,sc/A,sc = ") << Get_String_Format(PgrfBase.dDel_Tau) << m_strUnitStress << endl
		<< _T("      Lamda,v1     = ") << Get_String_Format(PgrfBase.dLamda_v1) << endl
		<< _T("      Lamda,v2     = ") << Get_String_Format(PgrfBase.dLamda_v2) << endl
		<< _T("      Lamda,v3     = ") << Get_String_Format(PgrfBase.dLamda_v3) << endl
		<< _T("      Lamda,v4     = ") << Get_String_Format(PgrfBase.dLamda_v4) << endl
		<< _T("      Lamda,v      = Lamda,v1*Lamda,v2*Lamda,v3*Lamda,v4 = ") << Get_String_Format(PgrfBase.dLamda_v) << endl
		<< _T("      Delta,tau,E2 = Lamda,v*Delta,tau = ") << Get_String_Format(PgrfBase.dDel_Tau_E_2) << m_strUnitStress << endl
		<< _T("      Delta,tau,c  = ") << Get_String_Format(PgrfBase.dDel_Tau_c) << m_strUnitStress << endl
		<< endl;

	fout << _T("      (gamma,Ff*Delta,tau,E2)/(Delta,tau,c/gamma,Mf,s) = ") << Get_String_Format(PgrfBase.dRatio) << endl;

	return TRUE;
}

CString CDgnPlateGirder::Get_String_Format(double dValue, int iNum)
{
	CString strFormat;

	switch ( iNum )
	{
	case 16: {strFormat.Format(_T("%16g"), dValue);      break; }
	case 17: {strFormat.Format(_T("%17g"), dValue);      break; }
	case 18: {strFormat.Format(_T("%18g"), dValue);      break; }
	default: {strFormat.Format(_T("%16g"), dValue);      break; }
	}

	return strFormat;
}

void CDgnPlateGirder::Print_EffectivePolyAndLineData(T_GSEC_POLYGON& PolyD, CArray<T_GSEC_LINE, T_GSEC_LINE&>& aEffLine)
{
	int nLineSize = aEffLine.GetSize();
	for ( int i = 0; i < nLineSize; i++ )
	{
		T_GSEC_LINE EffLine = aEffLine[i];
		int v1 = EffLine.v1;
		int v2 = EffLine.v2;
		double dx1 = PolyD.aVertex[v1].dX;
		double dy1 = PolyD.aVertex[v1].dY;
		double dx2 = PolyD.aVertex[v2].dX;
		double dy2 = PolyD.aVertex[v2].dY;
		TRACE(_T("%lf  %lf\n"), dx1, dy1);
		TRACE(_T("%lf  %lf\n"), dx2, dy2);
	}
}

int CDgnPlateGirder::Get_SubPannelCalcType(int nLongStfnSize)
{
	// 0=종방향 보강재 1 또는 2개 포함하는 패널, 1 = 없거나 3개이상 
	int nSubCalcType = 0;
	if ( nLongStfnSize == 1 || nLongStfnSize == 2 ) { nSubCalcType = 0; }
	else { nSubCalcType = 1; }

	return nSubCalcType;
}

void CDgnPlateGirder::SetScfpForce4Lcom(const _DGN_FORC_CRC& ForD, const _DGN_SCFP_PART_LCASE& StrD)
{
	m_ScfpForce = ForD;
	m_ScfpStress = StrD;
}

void CDgnPlateGirder::SetCheckNode(int nIJ)
{
	m_nIJ = nIJ;
}

int CDgnPlateGirder::GetCheckNode()
{
	return m_nIJ;
}

_DGN_FORC_CRC& CDgnPlateGirder::GetScfpForce4Lcom()
{
	return m_ScfpForce;
}

_DGN_SCFP_PART_LCASE& CDgnPlateGirder::GetScfpStress4Lcom()
{
	return m_ScfpStress;
}

double CDgnPlateGirder::CalcStressInterpolate(int nIJ, const T_SECT_STIFFNESS& StiffD, double dy, double dz)
{
	if ( nIJ < 0 || nIJ >1 ) return 0.0;

	double dWidth = StiffD.Cym + StiffD.Cyp;
	double dHeight = StiffD.Czm + StiffD.Czp;
	double dLeft = min(StiffD.dx1, StiffD.dx4);
	double dTop = max(StiffD.dy1, StiffD.dy2);

	double dYFromLeft = fabs(dy - dLeft);
	double dZFromTop = fabs(dTop - dz);

	double dYrat = SafeDiv(dYFromLeft, dWidth);
	double dZrat = SafeDiv(dZFromTop, dHeight);

	double dw = 0.125;
	double dCol = SafeDiv(dYrat, dw);
	double dRow = SafeDiv(dZrat, dw);

	auto _PosIdx = [&] (int nIndex) -> int
	{
		if ( nIndex < 0 ) return 0;
		if ( nIndex > 8 ) return 8;
		return nIndex;
	};

	int nCol = static_cast<int>(floor(dCol));  // 응력 보간을 위한 위치
	int nRow = static_cast<int>(floor(dRow));
	int nColP = _PosIdx(nCol - 1) + 1; // 모멘트 값은 index 1 부터 있음
	int nColN = _PosIdx(nCol + 1) + 1;
	int nRowP = _PosIdx(nRow - 1) + 1;
	int nRowN = _PosIdx(nRow + 1) + 1;

	const _DGN_SCFP_PART_LCASE& ScfpStress = GetScfpStress4Lcom();

	const _DGN_SCFP_D& ConStr = ScfpStress.ForceP1;
	double dStrA = ConStr.dbLStress[nIJ][0];
	int nIDy = 9;
	double adStr[4] = { 0.0, 0.0, 0.0, 0.0 };
	adStr[0] = dStrA + ConStr.dbLStress[nIJ][nRowP] + ConStr.dbLStress[nIJ][nIDy + nColP];
	adStr[1] = dStrA + ConStr.dbLStress[nIJ][nRowP] + ConStr.dbLStress[nIJ][nIDy + nColN];
	adStr[2] = dStrA + ConStr.dbLStress[nIJ][nRowN] + ConStr.dbLStress[nIJ][nIDy + nColN];
	adStr[3] = dStrA + ConStr.dbLStress[nIJ][nRowN] + ConStr.dbLStress[nIJ][nIDy + nColP];

	double dRatioFromBottom = 1.0 - dZrat;
	double dStress = CMathFunc::mathInterpolate(adStr[3], adStr[2], adStr[0], adStr[1], dYrat, dRatioFromBottom);

	return dStress;
}

void CDgnPlateGirder::SetDgnLcomNo(int nDgnLcomNo)
{
	m_DgnLcomNo = nDgnLcomNo;
}

int  CDgnPlateGirder::GetDgnLcomNo() const
{
	if ( !IsPrintMode() )
	{
		ASSERT(m_nThread == 1);
	}
	return m_DgnLcomNo;
}

int CDgnPlateGirder::GetDgnCurrentLcomIndex(int nLcom)
{
	ASSERT(nLcom!=0);
	return nLcom - 1;
}

T_UNIT_INDEX CDgnPlateGirder::GetCodeUnit() const
{
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	// Set Code Unit.
	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
	CodeUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_N;

	return CodeUnit;
}

bool CDgnPlateGirder::SetResult(const dgn::def::IResult* pResult)
{
	//BS TODO
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE )
	{
		return false; // 일단 double만
	}

	BOOL bULSLcom = m_pForcCtrl->Is_LcomForStrn(GetIDgnLcomNo(std::this_thread::get_id()));
	BOOL bSLSLcom = m_pForcCtrl->Is_LcomForStrs(GetIDgnLcomNo(std::this_thread::get_id()));
	BOOL bFLSLcom = m_pForcCtrl->Is_LcomForFati(GetIDgnLcomNo(std::this_thread::get_id()));

	auto nElemK = GetIDgnCheckElem(std::this_thread::get_id());
	int  nDgnLcomIndex = GetDgnCurrentLcomIndex(GetIDgnLcomNo(std::this_thread::get_id()));

	int iMaxMin = 0;

	CPlateGirderDesign* pPGD = m_pDoc->m_pPostCtrl->GetPlateGirder();
	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	dgn::item::enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetIDgnCheckPosition(std::this_thread::get_id());
	switch ( enResult )
	{
	case dgn::item::enDgnResultItem::SECTION_CLASS_SHEAR____P:
	case dgn::item::enDgnResultItem::SHEAR_ZF_P:
	case dgn::item::enDgnResultItem::SHEAR_VALUE3__N_P:
	case dgn::item::enDgnResultItem::SHEAR_VALUE_ZF_P:
	case dgn::item::enDgnResultItem::BEND__M_P:
		{
			if ( !bULSLcom ) return true;
			T_PLATE_GIRDER_RES_D* pData = m_svRes[nElemK];
			T_PRVS_D* pPrvsD = &(pData->svLcomRes[nDgnLcomIndex].PrvsD);
			SetShearResistanceLcomResult(pResult, *pPrvsD);
		}
		break;
	case dgn::item::enDgnResultItem::SECTION_CLASS_BEND_P____P:
	case dgn::item::enDgnResultItem::SECTION_CLASS_BEND_N____P:
	case dgn::item::enDgnResultItem::BEND__MNP:
	case dgn::item::enDgnResultItem::BEND__MPP:
	case dgn::item::enDgnResultItem::BEND_YMNP:
	case dgn::item::enDgnResultItem::BEND_YMPP:
	case dgn::item::enDgnResultItem::BEND_PR__MNT:
	case dgn::item::enDgnResultItem::BEND_PR__MPT:
		{
			if ( !bULSLcom ) return true;						
			T_PLATE_GIRDER_RES_D* pData = m_svRes[nElemK];
			T_PGBR_LCOM* pBending = &(pData->svLcomRes[nDgnLcomIndex].PgbrLcomD);
			SetBendingResistanceLcomResult(pResult, *pBending);
		}
		break;
	case dgn::item::enDgnResultItem::EFFECTIVE_STIFF__A_P:
	case dgn::item::enDgnResultItem::EFFECTIVE_STIFF__I_P:
	case dgn::item::enDgnResultItem::EFFECTIVE_STIFF_BEND__I_P:
		{
			if ( !bULSLcom ) return true;
			T_PLATE_GIRDER_RES_D* pData = m_svRes[nElemK];
			T_RHOC_LCOM* pRhoc = &(pData->svLcomRes[nDgnLcomIndex].RhoC);
			SetRhocAndEffectiveStiffnessParam(pResult, *pRhoc);
		}
		break;
	case dgn::item::enDgnResultItem::SECTION_CLASS_LTB____P:
	case dgn::item::enDgnResultItem::LTB_AXIAL__F_P:
	case dgn::item::enDgnResultItem::LTB_BENDING__M_P:
	case dgn::item::enDgnResultItem::LTB_BENDING_MCR__M_P:
	case dgn::item::enDgnResultItem::LTB_INTERACTION____P:
		{
			if ( !bULSLcom ) return true;
			T_PLATE_GIRDER_RES_D* pData = m_svRes[nElemK];
			T_PLTB_D* pPltbD = &(pData->svLcomRes[nDgnLcomIndex].PltbD);
			SetLateralTorsionalBucklingLcomResult(pResult, *pPltbD);
		}
		break;
	case dgn::item::enDgnResultItem::FATIGUE_SC_STR__S_P:
	case dgn::item::enDgnResultItem::FATIGUE_SC_VR___P:
		{
			if ( !bFLSLcom ) return true;
			T_PLATE_GIRDER_RES_D* pData = m_svRes[nElemK];
			T_PGRF_D* pPgrfD = &(pData->svLcomRes[nDgnLcomIndex].PgrfD);
			SetFatigueResistanceLcomResult(pResult, *pPgrfD);
		}
		break;
	case dgn::item::enDgnResultItem::LONG_SHEAR_FOR__F_P:
	case dgn::item::enDgnResultItem::LONG_SHEAR_DC__F_P:
		{
			if ( !bULSLcom ) return true;
			T_PLATE_GIRDER_RES_D* pData = m_svRes[nElemK];
			T_PGLS_D* pPglsD = &(pData->svLcomRes[nDgnLcomIndex].PglsD);
			SetLongitudinalShearResistanceLcomResult(pResult, *pPglsD);
		}
		break;
	case dgn::item::enDgnResultItem::TRANSVERSE_FORCE_FR__F_P:
	case dgn::item::enDgnResultItem::TRANSVERSE_FORCE_INTER___P:
		{
			if ( !bULSLcom ) return true;
			T_PLATE_GIRDER_RES_D* pData = m_svRes[nElemK];
			T_PRTF_D* pPrtfD = &(pData->svLcomRes[nDgnLcomIndex].PrtfD);
			SetTransverseForceLcomResult(pResult, *pPrtfD);
		}
		break;
	case dgn::item::enDgnResultItem::STRS_ST__SNP:
	case dgn::item::enDgnResultItem::STRS_ST__SPP:
	case dgn::item::enDgnResultItem::STRS_STN_S_P:
	case dgn::item::enDgnResultItem::STRS_STS_S_P:
	case dgn::item::enDgnResultItem::STRS_STC_S_P:
		//case dgn::item::enDgnResultItem::STRS_CC_C__SNP:
		//case dgn::item::enDgnResultItem::STRS_CC_C__SPP:
		{
			if ( !bSLSLcom ) return true;
			T_PLATE_GIRDER_RES_D* pData = m_svRes[nElemK];
			T_PGBS_D* pPgbsD = &(pData->svLcomRes[nDgnLcomIndex].PgbsD);
			SetStressLcomResult(pResult, *pPgbsD);
		}
		break;
	case dgn::item::enDgnResultItem::SLS_LONG_SHEAR_FOR__F_P:
	case dgn::item::enDgnResultItem::SLS_LONG_SHEAR_DC__F_P:
		{
			if ( !bSLSLcom ) return true;
			T_PLATE_GIRDER_RES_D* pData = m_svRes[nElemK];
			T_PLSS_D* pPlssD = &(pData->svLcomRes[nDgnLcomIndex].PlssD);
			SetLongitudinalShearResistanceSLSLcomResult(pResult, *pPlssD);
		}
		break;
	case dgn::item::enDgnResultItem::FATIGUE_LONG_SHEAR_DC__F_P:
	case dgn::item::enDgnResultItem::FATIGUE_LONG_SHEAR_FOR__F_P:
		break; // 이게 들어오면 안되는거 아닌가? 확인 하자.
	case dgn::item::enDgnResultItem::KFAC____M:
	case dgn::item::enDgnResultItem::BGGL__L_P:
	case dgn::item::enDgnResultItem::AXIAL__F_P:
	case dgn::item::enDgnResultItem::TORSION__M_P:
	case dgn::item::enDgnResultItem::SHEAR_YF_P:
	case dgn::item::enDgnResultItem::BEND_YM_P:
	case dgn::item::enDgnResultItem::BEND_BUCK_YM_P:
	case dgn::item::enDgnResultItem::BEND_ZM_P:
	case dgn::item::enDgnResultItem::SECTION_CLASS_BEND_Y__P:
	case dgn::item::enDgnResultItem::SECTION_CLASS_BEND_Z__P:
	case dgn::item::enDgnResultItem::COMBINE____P:
	case dgn::item::enDgnResultItem::COMB_AXIAL__F_P:
	case dgn::item::enDgnResultItem::COMB_BEND_YM_P:
	case dgn::item::enDgnResultItem::COMB_BEND_BUCK_YM_P:
	case dgn::item::enDgnResultItem::COMB_BEND_ZM_P:
	case dgn::item::enDgnResultItem::COMB_SHEAR_ZF_P:
	case dgn::item::enDgnResultItem::COMB_SHEAR_YF_P:
	case dgn::item::enDgnResultItem::COMB_TORSION__M_P:
	case dgn::item::enDgnResultItem::SECTION_CLASS_INTER_P:
	case dgn::item::enDgnResultItem::STFN__I_P:
	case dgn::item::enDgnResultItem::STFN__A_P:
	case dgn::item::enDgnResultItem::STFN_END__F_P:
	case dgn::item::enDgnResultItem::CMFT____P:
	case dgn::item::enDgnResultItem::LTB_CBFT____P:
	case dgn::item::enDgnResultItem::SLENDER____P:
	case dgn::item::enDgnResultItem::DEFLECTION__L_P:
	case dgn::item::enDgnResultItem::FATIGUE__S_P:
		break; // 이게 들어오면 안되는거 아닌가? 확인 하자.
	default:
		ASSERT(0);
		break;
	}

	return true;
}


void CDgnPlateGirder::SetBendingResistanceLcomResult(const dgn::def::IResult* pResult, T_PGBR_LCOM& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	dgn::item::enDgnResultItem enResult = pResult->GetItem();

	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	int nCheckPosition = GetIDgnCheckPosition(std::this_thread::get_id());
	int nDgnLcomNo = GetIDgnLcomNo(std::this_thread::get_id());

	rData.PgbrBase[nCheckPosition].bChk = true;
	rData.PgbrBase[nCheckPosition].ElemK = ElemK.first;
	rData.PgbrBase[nCheckPosition].nPosi = nCheckPosition;
	rData.PgbrBase[nCheckPosition].LcomK = nDgnLcomNo;
	rData.PgbrBase[nCheckPosition].OrgLcomK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);

	//Girder Force
	T_STAG_K nGdLastStagK = 0;
	int nLastStepNo = 0;
	m_pForcCtrl->GetGirderLastStageKey(ElemK, nGdLastStagK, nLastStepNo);

	auto itrCS = m_mStageForce.find(std::make_pair(ElemK, nDgnLcomNo));
	if ( itrCS == m_mStageForce.end() )
	{
		ASSERT(0); return;
	}
	CPG_FORC_STAG& ForceStage = (CPG_FORC_STAG&)itrCS->second;	
	const double dMy_a = ForceStage.MaxForce.dMuy[nCheckPosition];

	auto itr = m_mForce.find(ElemK);
	if ( itr == m_mForce.end() )
	{
		ASSERT(0); return;
	}
	auto& aForceLcom = itr->second.aForceLcom;
	_DGN_FORC_CRC* pForD = nullptr;
	int nLcomSize = aForceLcom.GetSize();
	if ( nLcomSize > 0 )
	{
		int nDgnLcomIndex = nDgnLcomNo - 1;
		if ( nDgnLcomNo > 0 && nDgnLcomIndex < nLcomSize )
		{
			pForD = (_DGN_FORC_CRC*)&(aForceLcom[nDgnLcomIndex]);
		}
	}
	const double dMuy = pForD->dMuy[nCheckPosition];

	const double dMy_c = dMuy - dMy_a;

	bool  bPositiveM = ME0(dMy_c);

	switch ( enResult )
	{
	case dgn::item::enDgnResultItem::SECTION_CLASS_BEND_P____P:
	case dgn::item::enDgnResultItem::SECTION_CLASS_BEND_N____P:
		{
			rData.PgbrBase[nCheckPosition].iSectClass = static_cast<int>(pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_0));
			rData.PgbrBase[nCheckPosition].iTopClass = static_cast<int>(pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_1));
			rData.PgbrBase[nCheckPosition].iWebClass = static_cast<int>(pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_2));
			rData.PgbrBase[nCheckPosition].iBotClass = static_cast<int>(pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_3));
			if ( rData.PgbrBase[nCheckPosition].iSectClass == 5 )
			{
				rData.PgbrBase[nCheckPosition].iSectClass = 2;
			}
		}
		break;
	case dgn::item::enDgnResultItem::BEND__MPP:
		{
			if ( bPositiveM )
			{
				rData.PgbrBase[nCheckPosition].dMpl_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN);
			}
		}
		break;
	case dgn::item::enDgnResultItem::BEND__MNP:
		{
			if ( !bPositiveM )
			{
				rData.PgbrBase[nCheckPosition].dMpl_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN);
			}
		}
		break;
	case dgn::item::enDgnResultItem::BEND_PR__MNT:
	case dgn::item::enDgnResultItem::BEND_PR__MPT:
		{
			rData.PgbrBase[nCheckPosition].dMel_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_VALUE);
		}
		break;
	case dgn::item::enDgnResultItem::BEND_YMNP:
	case dgn::item::enDgnResultItem::BEND_YMPP:
		{
			//rData.PgbrBase[nCheckPosition].dMel_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_REQ);
			double dMRd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN);
			rData.PgbrBase[nCheckPosition].dM_Rd = dMRd;
			double dM_Edc = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_REQ);
			if ( EQ0(dMRd) )
			{
				rData.PgbrBase[nCheckPosition].dRatio = SafeDivAbs(dM_Edc, m_dZero);
			}
			else
			{
				rData.PgbrBase[nCheckPosition].dRatio = SafeDivAbs(dM_Edc, dMRd);
			}			
		}
		break;
	default:
		break;
	}
	rData.PgbrBase[nCheckPosition].bOK = LE(rData.PgbrBase[nCheckPosition].dRatio, 1.0);
}

void CDgnPlateGirder::SetShearResistanceLcomResult(const dgn::def::IResult* pResult, T_PRVS_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	dgn::item::enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetIDgnCheckPosition(std::this_thread::get_id());
	int nDgnLcomNo = GetIDgnLcomNo(std::this_thread::get_id());

	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	rData.PrvsBase[nCheckPosition].bChk = true;
	rData.PrvsBase[nCheckPosition].ElemK = ElemK.first;
	rData.PrvsBase[nCheckPosition].nPosi = GetIDgnCheckPosition(std::this_thread::get_id());
	rData.PrvsBase[nCheckPosition].LcomK = nDgnLcomNo;
	rData.PrvsBase[nCheckPosition].OrgLcomK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);

	if ( enResult == dgn::item::enDgnResultItem::SECTION_CLASS_SHEAR____P )
	{
		rData.PrvsBase[nCheckPosition].iSectClass = static_cast<int>(pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_0));
		rData.PrvsBase[nCheckPosition].iTopClass = static_cast<int>(pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_1));
		rData.PrvsBase[nCheckPosition].iWebClass = static_cast<int>(pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_2));
		rData.PrvsBase[nCheckPosition].iBotClass = static_cast<int>(pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_3));
		if (rData.PrvsBase[nCheckPosition].iSectClass == 5)
		{
			rData.PrvsBase[nCheckPosition].iSectClass = 2;
		}
	}
	else if ( enResult == dgn::item::enDgnResultItem::SHEAR_ZF_P )
	{
		rData.PrvsBase[nCheckPosition].dFz = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_REQ);
		rData.PrvsBase[nCheckPosition].dV_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN);
	}
	else if ( enResult == dgn::item::enDgnResultItem::SHEAR_VALUE3__N_P )
	{
		rData.PrvsBase[nCheckPosition].dEta3     = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_0);
		rData.PrvsBase[nCheckPosition].dEta1     = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_1);
		rData.PrvsBase[nCheckPosition].dEta1_lim = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_2);
	}
	else if ( enResult == dgn::item::enDgnResultItem::SHEAR_VALUE_ZF_P )
	{
		rData.PrvsBase[nCheckPosition].dV_Ed   = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_0);
		rData.PrvsBase[nCheckPosition].dVpl_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_1);
		rData.PrvsBase[nCheckPosition].dVb_Rd  = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_2);
		rData.PrvsBase[nCheckPosition].dCombinedEffe = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_3);

	}
	else if ( enResult == dgn::item::enDgnResultItem::BEND__M_P )
	{
		rData.PrvsBase[nCheckPosition].dM_Ed = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN);
	}
	else ASSERT(0);

	// ???
	rData.PrvsBase[nCheckPosition].dRatio = SafeDivAbs(rData.PrvsBase[nCheckPosition].dFz, rData.PrvsBase[nCheckPosition].dV_Rd);
	BOOL bOK = FALSE;
	if ( LE(rData.PrvsBase[nCheckPosition].dRatio, 1.0) )
	{
		if ( rData.PrvsBase[nCheckPosition].iSectClass == 3 || rData.PrvsBase[nCheckPosition].iSectClass == 4 )
		{
			if ( MT(rData.PrvsBase[nCheckPosition].dEta3, 0.5) )
			{
				if (ME(rData.PrvsBase[nCheckPosition].dEta1, rData.PrvsBase[nCheckPosition].dEta1_lim))
				{
					if (LE(rData.PrvsBase[nCheckPosition].dCombinedEffe, 1.0))
					{
						bOK = TRUE;
					}
				}
				else
					bOK = TRUE;
			}
			else
				bOK = TRUE;
		}
		else
			bOK = TRUE;
	}

	rData.PrvsBase[nCheckPosition].bOK = bOK;
}

void CDgnPlateGirder::SetLateralTorsionalBucklingLcomResult(const dgn::def::IResult* pResult, T_PLTB_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	dgn::item::enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetIDgnCheckPosition(std::this_thread::get_id());
	int nDgnLcomNo = GetIDgnLcomNo(std::this_thread::get_id());

	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	rData.PltbBase[nCheckPosition].bChk = true;
	rData.PltbBase[nCheckPosition].ElemK = ElemK.first;
	rData.PltbBase[nCheckPosition].nPosi = GetIDgnCheckPosition(std::this_thread::get_id());
	rData.PltbBase[nCheckPosition].LcomK = nDgnLcomNo;
	rData.PltbBase[nCheckPosition].OrgLcomK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);


	if ( enResult == dgn::item::enDgnResultItem::SECTION_CLASS_LTB____P )
	{
		rData.PltbBase[nCheckPosition].iSectClass = static_cast<int>(pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_0));
	}
	else if ( enResult == dgn::item::enDgnResultItem::LTB_AXIAL__F_P )
	{
		rData.PltbBase[nCheckPosition].dNb_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V2_1);
	}
	else if ( enResult == dgn::item::enDgnResultItem::LTB_BENDING__M_P )
	{
		rData.PltbBase[nCheckPosition].dMb_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V2_1);
	}
	else if ( enResult == dgn::item::enDgnResultItem::LTB_BENDING_MCR__M_P )
	{
		rData.PltbBase[nCheckPosition].dMcr = pCasted->GetResult(dgn::item::EN_DGN_COMPO_VALUE);
	}
	else if ( enResult == dgn::item::enDgnResultItem::LTB_INTERACTION____P )
	{
		rData.PltbBase[nCheckPosition].dInterRat = pCasted->GetResult(dgn::item::EN_DGN_COMPO_VALUE);
		rData.PltbBase[nCheckPosition].dRatio = rData.PltbBase[nCheckPosition].dInterRat;

	}
	else ASSERT(0);

	rData.PltbBase[nCheckPosition].bOK = LE(rData.PltbBase[nCheckPosition].dRatio, 1.0);
}

void CDgnPlateGirder::SetTransverseForceLcomResult(const dgn::def::IResult* pResult, T_PRTF_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	dgn::item::enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetIDgnCheckPosition(std::this_thread::get_id());
	int nDgnLcomNo = GetIDgnLcomNo(std::this_thread::get_id());

	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	rData.PrtfBase[nCheckPosition].bChk = true;
	rData.PrtfBase[nCheckPosition].ElemK = ElemK.first;
	rData.PrtfBase[nCheckPosition].nPosi = GetIDgnCheckPosition(std::this_thread::get_id());
	rData.PrtfBase[nCheckPosition].LcomK = nDgnLcomNo;
	rData.PrtfBase[nCheckPosition].OrgLcomK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);


	if ( enResult == dgn::item::enDgnResultItem::TRANSVERSE_FORCE_FR__F_P )
	{
		rData.PrtfBase[nCheckPosition].dF_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN);
	}
	else if ( enResult == dgn::item::enDgnResultItem::TRANSVERSE_FORCE_INTER___P )
	{
		rData.PrtfBase[nCheckPosition].dEta2 = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_0);
		rData.PrtfBase[nCheckPosition].dEta1 = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_1);
		rData.PrtfBase[nCheckPosition].dInterRat = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_2);
		rData.PrtfBase[nCheckPosition].dRatio = rData.PrtfBase[nCheckPosition].dInterRat;
	}
	else ASSERT(0);

	rData.PrtfBase[nCheckPosition].bOK = LE(rData.PrtfBase[nCheckPosition].dRatio, 1.4);
}

void CDgnPlateGirder::SetFatigueResistanceLcomResult(const dgn::def::IResult* pResult, T_PGRF_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	dgn::item::enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetIDgnCheckPosition(std::this_thread::get_id());
	int nDgnLcomNo = GetIDgnLcomNo(std::this_thread::get_id());

	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	rData.PgrfBase[nCheckPosition].bChk = true;
	rData.PgrfBase[nCheckPosition].ElemK = ElemK.first;
	rData.PgrfBase[nCheckPosition].nPosi = GetIDgnCheckPosition(std::this_thread::get_id());
	rData.PgrfBase[nCheckPosition].LcomK = nDgnLcomNo;
	rData.PgrfBase[nCheckPosition].OrgLcomK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);

	if ( enResult == dgn::item::enDgnResultItem::FATIGUE_SC_STR__S_P )
	{
		rData.PgrfBase[nCheckPosition].dDel_Tau = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_0);
		rData.PgrfBase[nCheckPosition].dDel_Tau_E_2 = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_1);
		rData.PgrfBase[nCheckPosition].dDel_Tau_c = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_2);
	}
	else if ( enResult == dgn::item::enDgnResultItem::FATIGUE_SC_VR___P )
	{
		rData.PgrfBase[nCheckPosition].dLamda_v = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V2_0);
		rData.PgrfBase[nCheckPosition].dRatio = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V2_1);
	}
	else ASSERT(0);

	rData.PgrfBase[nCheckPosition].bOK = LE(rData.PgrfBase[nCheckPosition].dRatio, 1.0);
}


void CDgnPlateGirder::SetLongitudinalShearResistanceLcomResult(const dgn::def::IResult* pResult, T_PGLS_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	dgn::item::enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetIDgnCheckPosition(std::this_thread::get_id());
	int nDgnLcomNo = GetIDgnLcomNo(std::this_thread::get_id());

	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	rData.PglsBase[nCheckPosition].bChk = true;
	rData.PglsBase[nCheckPosition].ElemK = ElemK.first;
	rData.PglsBase[nCheckPosition].nPosi = GetIDgnCheckPosition(std::this_thread::get_id());
	rData.PglsBase[nCheckPosition].LcomK = nDgnLcomNo;
	rData.PglsBase[nCheckPosition].OrgLcomK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);


	if ( enResult == dgn::item::enDgnResultItem::LONG_SHEAR_FOR__F_P )
	{
		rData.PglsBase[nCheckPosition].dV_L_Ed = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V2_0);
		rData.PglsBase[nCheckPosition].dP_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V2_1);
	}
	else if ( enResult == dgn::item::enDgnResultItem::LONG_SHEAR_DC__F_P )
	{
		rData.PglsBase[nCheckPosition].dv_L_Ed = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_0);
		rData.PglsBase[nCheckPosition].dv_L_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_1);
		rData.PglsBase[nCheckPosition].dRatio = SafeDgnRatioC0(rData.PglsBase[nCheckPosition].dv_L_Ed, rData.PglsBase[nCheckPosition].dv_L_Rd);
	}
	else ASSERT(0);

	rData.PglsBase[nCheckPosition].bOK = LE(rData.PglsBase[nCheckPosition].dRatio, 1.0);
}

void CDgnPlateGirder::SetStressLcomResult(const dgn::def::IResult* pResult, T_PGBS_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE ) // double만 된다고?
	{
		return; // 일단 double만
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	dgn::item::enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetIDgnCheckPosition(std::this_thread::get_id());
	int nDgnLcomNo = GetIDgnLcomNo(std::this_thread::get_id());
	int nOrgLcomKey = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);

	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	rData.PgbsBase[nCheckPosition].bChk = true;
	rData.PgbsBase[nCheckPosition].ElemK = ElemK.first;
	rData.PgbsBase[nCheckPosition].nPosi = GetIDgnCheckPosition(std::this_thread::get_id());

	rData.PgbsBase[nCheckPosition].ChkGirder.LcomK = nDgnLcomNo;
	rData.PgbsBase[nCheckPosition].ChkGirder.OrgLcomK = nOrgLcomKey;

	rData.PgbsBase[nCheckPosition].ChkSlab.LcomK = nDgnLcomNo;
	rData.PgbsBase[nCheckPosition].ChkSlab.OrgLcomK = nOrgLcomKey;

	rData.PgbsBase[nCheckPosition].ChkBar.LcomK = nDgnLcomNo;
	rData.PgbsBase[nCheckPosition].ChkBar.OrgLcomK = nOrgLcomKey;

	switch ( enResult )
	{
	case dgn::item::enDgnResultItem::STRS_ST__SNP:
		{
			rData.PgbsBase[nCheckPosition].ChkBar.dStress_bar = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_REQ);
			rData.PgbsBase[nCheckPosition].ChkBar.dAlwStress_bar = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN);
			rData.PgbsBase[nCheckPosition].ChkBar.dMaxRatio_bar = SafeDivAbs(pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_REQ), pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN));
			rData.PgbsBase[nCheckPosition].ChkBar.bChk_Stress_bar = LE(rData.PgbsBase[nCheckPosition].ChkBar.dMaxRatio_bar, 1.0);
		}
		break;
	case dgn::item::enDgnResultItem::STRS_ST__SPP:
		{
			rData.PgbsBase[nCheckPosition].ChkSlab.dStress_sb = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_REQ);
			rData.PgbsBase[nCheckPosition].ChkSlab.dAlwStress_sb = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN);
			rData.PgbsBase[nCheckPosition].ChkSlab.dMaxRatio_sb = SafeDivAbs(pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_REQ), pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN));
			rData.PgbsBase[nCheckPosition].ChkSlab.bChk_Stress_sb = LE(rData.PgbsBase[nCheckPosition].ChkSlab.dMaxRatio_sb, 1.0);
		}
		break;
	case dgn::item::enDgnResultItem::STRS_STN_S_P:
		{
			rData.PgbsBase[nCheckPosition].ChkGirder.dMaxStress = -1.0*pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_REQ);
			rData.PgbsBase[nCheckPosition].ChkGirder.dAlwStress = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN);
			rData.PgbsBase[nCheckPosition].ChkGirder.bChk_Stress = AbsLE(rData.PgbsBase[nCheckPosition].ChkGirder.dMaxStress, rData.PgbsBase[nCheckPosition].ChkGirder.dAlwStress);
			rData.PgbsBase[nCheckPosition].ChkGirder.dMaxRatio = SafeDivAbs(rData.PgbsBase[nCheckPosition].ChkGirder.dMaxStress, rData.PgbsBase[nCheckPosition].ChkGirder.dAlwStress);
		}
		break;
	case dgn::item::enDgnResultItem::STRS_STS_S_P:
		{
			rData.PgbsBase[nCheckPosition].ChkGirder.dtau_w = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_REQ);
			rData.PgbsBase[nCheckPosition].ChkGirder.dAlw_tau = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN);
			rData.PgbsBase[nCheckPosition].ChkGirder.bChk_tau = AbsLE(rData.PgbsBase[nCheckPosition].ChkGirder.dtau_w, rData.PgbsBase[nCheckPosition].ChkGirder.dAlw_tau);
		}
		break;
	case dgn::item::enDgnResultItem::STRS_STC_S_P:
		{
			rData.PgbsBase[nCheckPosition].ChkGirder.dStress_cb = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_REQ);
			rData.PgbsBase[nCheckPosition].ChkGirder.dAlw_stress_cb = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN);
			rData.PgbsBase[nCheckPosition].ChkGirder.bChk_Stress_cb = AbsLE(rData.PgbsBase[nCheckPosition].ChkGirder.dStress_cb, rData.PgbsBase[nCheckPosition].ChkGirder.dAlw_stress_cb);
		}
		break;
		// 	case dgn::item::enDgnResultItem::STRS_CC_C__SNP:
		// 	case dgn::item::enDgnResultItem::STRS_CC_C__SPP:
		// 		{
		// 			rData.PgbsBase[nCheckPosition].ChkSlab.dStress_sb = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_REQ);
		// 			rData.PgbsBase[nCheckPosition].ChkSlab.dAlwStress_sb = pCasted->GetResult(dgn::item::EN_DGN_COMPO_DC_OWN);
		// 			rData.PgbsBase[nCheckPosition].ChkSlab.bChk_Stress_sb = AbsLE(rData.PgbsBase[nCheckPosition].ChkGirder.dStress_cb, rData.PgbsBase[nCheckPosition].ChkGirder.dAlw_stress_cb);
		// 		}
		// 		break;
	default:
		break;
	}
	BOOL bCheckGirder = (rData.PgbsBase[nCheckPosition].ChkGirder.bChk_Stress &&
		rData.PgbsBase[nCheckPosition].ChkGirder.bChk_tau &&
		rData.PgbsBase[nCheckPosition].ChkGirder.bChk_Stress_cb);
	rData.PgbsBase[nCheckPosition].ChkGirder.bOK = bCheckGirder;

	rData.PgbsBase[nCheckPosition].bOK = (bCheckGirder &&
		rData.PgbsBase[nCheckPosition].ChkBar.bChk_Stress_bar &&
		rData.PgbsBase[nCheckPosition].ChkSlab.bChk_Stress_sb);
}

void CDgnPlateGirder::SetLongitudinalShearResistanceSLSLcomResult(const dgn::def::IResult* pResult, T_PLSS_D& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE )
	{
		return;
	}

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	dgn::item::enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetIDgnCheckPosition(std::this_thread::get_id());
	int nDgnLcomNo = GetIDgnLcomNo(std::this_thread::get_id());

	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	rData.PlssBase[nCheckPosition].bChk = true;
	rData.PlssBase[nCheckPosition].ElemK = ElemK.first;
	rData.PlssBase[nCheckPosition].nPosi = GetIDgnCheckPosition(std::this_thread::get_id());
	rData.PlssBase[nCheckPosition].LcomK = nDgnLcomNo;
	rData.PlssBase[nCheckPosition].OrgLcomK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);

	if ( enResult == dgn::item::enDgnResultItem::SLS_LONG_SHEAR_FOR__F_P )
	{
		rData.PlssBase[nCheckPosition].dV_L_Ed = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V2_0);
		rData.PlssBase[nCheckPosition].dP_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V2_1);
		rData.PlssBase[nCheckPosition].dP_Rd_SLS = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V2_1);
	}
	else if ( enResult == dgn::item::enDgnResultItem::SLS_LONG_SHEAR_DC__F_P )
	{
		rData.PlssBase[nCheckPosition].dv_L_Ed = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_0);
		rData.PlssBase[nCheckPosition].dv_L_Rd = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V3_1);
		rData.PlssBase[nCheckPosition].dRatio = SafeDivAbs(rData.PlssBase[nCheckPosition].dv_L_Ed, rData.PlssBase[nCheckPosition].dv_L_Rd);
	}
	else ASSERT(0);

	rData.PlssBase[nCheckPosition].bOK = LE(rData.PlssBase[nCheckPosition].dRatio, 1.0);
}

void CDgnPlateGirder::SetRhocAndEffectiveStiffnessParam(const dgn::def::IResult* pResult, T_RHOC_LCOM& rData)
{
	if ( pResult->GetType() != dgn::def::ResultType::EN_DOUBLE )
	{
		return;
	}

	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	CPG_SECT_ELEM SectElem; SectElem.Initialize();
	Get_CpgSectElem(ElemK, SectElem);

	const auto* pCasted = static_cast<const dgn::def::TResult<double>*>(pResult);
	dgn::item::enDgnResultItem enResult = pResult->GetItem();

	int nCheckPosition = GetIDgnCheckPosition(std::this_thread::get_id());
	int nDgnLcomNo = GetIDgnLcomNo(std::this_thread::get_id());
	BOOL bPosiI = (nCheckPosition == 0);

	T_SECT_STIFFNESS GirderStiff;
	Get_GirderStiffnessofCompo(bPosiI, SectElem.SectD, GirderStiff);

	const T_SECT_STIFFNESS& RbarStiff = SectElem.SectPosi[nCheckPosition].RebarSect;

	rData.Rho_C[nCheckPosition].bChk = TRUE;
	rData.Rho_C[nCheckPosition].ElemK = ElemK.first;
	rData.Rho_C[nCheckPosition].nPosi = nCheckPosition;
	rData.Rho_C[nCheckPosition].LcomK = nDgnLcomNo;
	rData.Rho_C[nCheckPosition].OrgLcomK = m_pForcCtrl->GetOrgLcomKey(nDgnLcomNo);
	rData.Rho_C[nCheckPosition].dRho_c = 1.0; // 이전에도 값을 1로 정하고 있고 할당하지 않고 있어 우선 1로..

	if ( enResult == dgn::item::enDgnResultItem::EFFECTIVE_STIFF__A_P )
	{
		rData.Rho_C[nCheckPosition].dAa_eff_Fx = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_0);
		if ( EQ0(rData.Rho_C[nCheckPosition].dAa_eff_Fx) )
		{
			rData.Rho_C[nCheckPosition].dAa_eff_Fx = GirderStiff.Area;
		}
		rData.Rho_C[nCheckPosition].dAc_eff_Fx = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_1);
		if ( EQ0(rData.Rho_C[nCheckPosition].dAc_eff_Fx) )
		{
			rData.Rho_C[nCheckPosition].dAc_eff_Fx = RbarStiff.Area;
		}
		rData.Rho_C[nCheckPosition].dAa_eff_My = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_2);
		if ( EQ0(rData.Rho_C[nCheckPosition].dAa_eff_My) )
		{
			rData.Rho_C[nCheckPosition].dAa_eff_My = GirderStiff.Area;
		}
		rData.Rho_C[nCheckPosition].dAc_eff_My = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_3);
		if ( EQ0(rData.Rho_C[nCheckPosition].dAc_eff_My) )
		{
			rData.Rho_C[nCheckPosition].dAc_eff_My = RbarStiff.Area;
		}
	}
	else if ( enResult == dgn::item::enDgnResultItem::EFFECTIVE_STIFF__I_P )
	{
		rData.Rho_C[nCheckPosition].dIya_eff_Fx = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_0);
		if ( EQ0(rData.Rho_C[nCheckPosition].dIya_eff_Fx) )
		{
			rData.Rho_C[nCheckPosition].dIya_eff_Fx = GirderStiff.Ryy;
		}
		rData.Rho_C[nCheckPosition].dIza_eff_Fx = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_1);
		if ( EQ0(rData.Rho_C[nCheckPosition].dIza_eff_Fx) )
		{
			rData.Rho_C[nCheckPosition].dIza_eff_Fx = GirderStiff.Rzz;
		}
		rData.Rho_C[nCheckPosition].dIyc_eff_Fx = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_2);
		if ( EQ0(rData.Rho_C[nCheckPosition].dIyc_eff_Fx) )
		{
			rData.Rho_C[nCheckPosition].dIyc_eff_Fx = RbarStiff.Ryy;
		}
		rData.Rho_C[nCheckPosition].dIzc_eff_Fx = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_3);
		if ( EQ0(rData.Rho_C[nCheckPosition].dIzc_eff_Fx) )
		{
			rData.Rho_C[nCheckPosition].dIzc_eff_Fx = RbarStiff.Rzz;
		}
	}
	else if ( enResult == dgn::item::enDgnResultItem::EFFECTIVE_STIFF_BEND__I_P )
	{
		rData.Rho_C[nCheckPosition].dIya_eff_My = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_0);
		if ( EQ0(rData.Rho_C[nCheckPosition].dIya_eff_My) )
		{
			rData.Rho_C[nCheckPosition].dIya_eff_My = GirderStiff.Ryy;
		}
		rData.Rho_C[nCheckPosition].dIza_eff_My = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_1);
		if ( EQ0(rData.Rho_C[nCheckPosition].dIza_eff_My) )
		{
			rData.Rho_C[nCheckPosition].dIza_eff_My = GirderStiff.Rzz;
		}
		rData.Rho_C[nCheckPosition].dIyc_eff_My = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_2);
		if ( EQ0(rData.Rho_C[nCheckPosition].dIyc_eff_My) )
		{
			rData.Rho_C[nCheckPosition].dIyc_eff_My = RbarStiff.Ryy;
		}
		rData.Rho_C[nCheckPosition].dIzc_eff_My = pCasted->GetResult(dgn::item::EN_DGN_COMPO_V4_3);
		if ( EQ0(rData.Rho_C[nCheckPosition].dIzc_eff_My) )
		{
			rData.Rho_C[nCheckPosition].dIzc_eff_My = RbarStiff.Rzz;
		}
	}
	else ASSERT(0);

}

// T_PGBR_LCOM* CDgnPlateGirder::GetBendingResistanceLcomResult()
// {
// 	return m_pCurPgbrLcom;
// }
// 
// T_PRVS_D* CDgnPlateGirder::GetShearResistanceLcomResult()
// {
// 	return m_pCurPrvs;
// }
// 
// T_PLTB_D* CDgnPlateGirder::GetLateralTorsionalBucklingLcomResult()
// {
// 	return m_pCurPltb;
// }
// 
// T_PRTF_D* CDgnPlateGirder::GetTransverseForceLcomResult()
// {
// 	return m_pCurPrtf;
// }
// 
// T_PGRF_D* CDgnPlateGirder::GetFatigueResistanceLcomResult()
// {
// 	return m_pCurPgrf;
// }
// 
// T_PGLS_D* CDgnPlateGirder::GetLongitudinalShearResistanceLcomResult()
// {
// 	return m_pCurPgls;
// }
// 
// T_PGBS_D* CDgnPlateGirder::GetStressLcomResult()
// {
// 	return m_pCurPgbs;
// }
// 
// T_PLSS_D* CDgnPlateGirder::GetLongitudinalShearSLSLcomResult()
// {
// 	return m_pCurPlss;
// }
// 
// T_RHOC_LCOM* CDgnPlateGirder::GetRhocAndEffectiveStiffnessParam()
// {
// 	return m_pCurRhocLcom;
// }

void CDgnPlateGirder::ConvertBendingRes(const T_PGBR_BASE &rIn, T_PGBR_BASE &rOut)
{
	rOut.iSectClass = rIn.iSectClass;
	rOut.iTopClass = rIn.iTopClass;
	rOut.iWebClass = rIn.iWebClass;
	rOut.iBotClass = rIn.iBotClass;
	rOut.dMpl_Rd = rIn.dMpl_Rd;
	rOut.dMel_Rd = rIn.dMel_Rd;
	rOut.dM_Rd = rIn.dM_Rd;
	rOut.dRatio = rIn.dRatio;
	rOut.bOK = rIn.bOK;
}

void CDgnPlateGirder::ConvertVerticalShearRes(const T_PRVS_BASE &rIn, T_PRVS_BASE &rOut)
{
	rOut.iSectClass = rIn.iSectClass;
	rOut.iTopClass = rIn.iTopClass;
	rOut.iWebClass = rIn.iWebClass;
	rOut.iBotClass = rIn.iBotClass;
	rOut.dVpl_Rd = rIn.dVpl_Rd;
	rOut.dVb_Rd = rIn.dVb_Rd;
	rOut.dM_Ed = rIn.dM_Ed;
	rOut.dRatio = rIn.dRatio;
	rOut.bOK = rIn.bOK;
}

void CDgnPlateGirder::ConvertLTBRes(const T_PLTB_BASE &rIn, T_PLTB_BASE &rOut)
{
	rOut.iSectClass = rIn.iSectClass;
	rOut.dNb_Rd = rIn.dNb_Rd;
	rOut.dMb_Rd = rIn.dMb_Rd;
	rOut.dMcr = rIn.dMcr;
	rOut.dInterRat = rIn.dInterRat;
	rOut.dRatio = rIn.dRatio;
	rOut.bOK = rIn.bOK;
}

void CDgnPlateGirder::ConvertTransverseForceRes(const T_PRTF_BASE &rIn, T_PRTF_BASE &rOut)
{
	rOut.iSectClass = rIn.iSectClass;
	rOut.dF_Rd = rIn.dF_Rd;
	rOut.dEta2 = rIn.dEta2;
	rOut.dEta1 = rIn.dEta1;
	rOut.dInterRat = rIn.dInterRat;
	rOut.dRatio = rIn.dRatio;
	rOut.bOK = rIn.bOK;
}

void CDgnPlateGirder::ConvertFatigueRes(const T_PGRF_BASE &rIn, T_PGRF_BASE &rOut)
{
	rOut.dLamda_v = rIn.dLamda_v;
	rOut.dDel_Tau = rIn.dDel_Tau;
	rOut.dDel_Tau_E_2 = rIn.dDel_Tau_E_2;
	rOut.dDel_Tau_c = rIn.dDel_Tau_c;
	rOut.dRatio = rIn.dRatio;
	rOut.bOK = rIn.bOK;
}

void CDgnPlateGirder::ConvertLongitudinalShearRes(const T_PGLS_BASE &rIn, T_PGLS_BASE &rOut)
{
	rOut.iSectClass = rIn.iSectClass;
	rOut.dV_L_Ed = rIn.dV_L_Ed;
	rOut.dv_L_Ed = rIn.dv_L_Ed;
	rOut.dv_L_Ed2 = rIn.dv_L_Ed2;
	rOut.dP_Rd = rIn.dP_Rd;
	rOut.dv_L_Rd = rIn.dv_L_Rd;
	rOut.dRatio = rIn.dRatio;
	rOut.bOK = rIn.bOK;
}

void CDgnPlateGirder::ConvertStressLcomRes(const T_PGBS_BASE &rIn, T_PGBS_BASE &rOut)
{
	rOut.ChkGirder.dMaxStress = rIn.ChkGirder.dMaxStress;
	rOut.ChkGirder.dAlwStress = rIn.ChkGirder.dAlwStress;
	rOut.ChkGirder.bChk_Stress = rIn.ChkGirder.bChk_Stress;
	rOut.ChkGirder.dMaxRatio = rIn.ChkGirder.dMaxRatio;
	rOut.ChkGirder.dtau_w = rIn.ChkGirder.dtau_w;
	rOut.ChkGirder.dAlw_tau = rIn.ChkGirder.dAlw_tau;
	rOut.ChkGirder.dStress_cb = rIn.ChkGirder.dStress_cb;
	rOut.ChkGirder.dAlw_stress_cb = rIn.ChkGirder.dAlw_stress_cb;
	rOut.ChkGirder.bOK = rIn.ChkGirder.bOK;
	//
	rOut.ChkSlab.dStress_sb = rIn.ChkSlab.dStress_sb;
	rOut.ChkSlab.dAlwStress_sb = rIn.ChkSlab.dAlwStress_sb;
	rOut.ChkSlab.dMaxRatio_sb = rIn.ChkSlab.dMaxRatio_sb;
	rOut.ChkSlab.bChk_Stress_sb = rIn.ChkSlab.bChk_Stress_sb;
	//
	rOut.ChkBar.dStress_bar = rIn.ChkBar.dStress_bar;
	rOut.ChkBar.dAlwStress_bar = rIn.ChkBar.dAlwStress_bar;
	rOut.ChkBar.dMaxRatio_bar = rIn.ChkBar.dMaxRatio_bar;
	rOut.ChkBar.bChk_Stress_bar = rIn.ChkBar.bChk_Stress_bar;

	rOut.bOK = rIn.bOK;
}

void CDgnPlateGirder::ConvertLongitudinalShearSLSRes(const T_PLSS_BASE &rIn, T_PLSS_BASE &rOut)
{
	rOut.dv_L_Ed = rIn.dv_L_Ed;
	rOut.dv_L_Ed2 = rIn.dv_L_Ed2;
	rOut.dP_Rd = rIn.dP_Rd;
	rOut.dP_Rd_SLS = rIn.dP_Rd_SLS;
	rOut.dv_L_Rd = rIn.dv_L_Rd;
	rOut.dRatio = rIn.dRatio;
	rOut.bOK = rIn.bOK;
}

void CDgnPlateGirder::ConvertRhocAndEffectiveStiffness(const T_RHOC_BASE &rIn, T_RHOC_BASE &rOut)
{
	rOut.dRho_c = rIn.dRho_c;
	rOut.dAa_eff_Fx = rIn.dAa_eff_Fx;
	rOut.dIya_eff_Fx = rIn.dIya_eff_Fx;
	rOut.dIza_eff_Fx = rIn.dIza_eff_Fx;
	rOut.dAc_eff_Fx = rIn.dAc_eff_Fx;
	rOut.dIyc_eff_Fx = rIn.dIyc_eff_Fx;
	rOut.dIzc_eff_Fx = rIn.dIzc_eff_Fx;

	rOut.dAa_eff_My = rIn.dAa_eff_My;
	rOut.dIya_eff_My = rIn.dIya_eff_My;
	rOut.dIza_eff_My = rIn.dIza_eff_My;
	rOut.dAc_eff_My = rIn.dAc_eff_My;
	rOut.dIyc_eff_My = rIn.dIyc_eff_My;
	rOut.dIzc_eff_My = rIn.dIzc_eff_My;
	rOut.dRho_c = rIn.dRho_c;
}

bool CDgnPlateGirder::GetReportInput(dgn::link::MItemInputList& vMembItemInput, dgn::link::MembInfoList& vMembInfo)
{
	T_CPGD_D CpgdD;
	if ( !m_pDoc->m_pAttrCtrl->GetCpgd(CpgdD) )
	{
		CpgdD.Initialize();
	}

	CArray<T_CGRE_K, T_CGRE_K> aKeyBeam;
	m_pDoc->m_pAttrCtrl2->GetCgreKeyList(aKeyBeam);
	
	CArray<T_CGRV_K, T_CGRV_K> aKeyVBeam;
	m_pDoc->m_pAttrCtrl2->GetCgrvKeyList(aKeyVBeam);

	ArrElemPairKey aRptElemPairK;
	CDBLib::ConvertToElemPairKeyList(aKeyBeam, aKeyVBeam, aRptElemPairK);

	int nRptElemSize = aRptElemPairK.GetSize();
	if ( nRptElemSize == 0 )
		return true;

	std::vector<std::pair<ElemPairK, int>> vRptElem;
	T_CGRE_D CgreD;

	for ( int i = 0; i < nRptElemSize; ++i )
	{
		auto ElemK = aRptElemPairK[i];
		CPG_SECT_ELEM SectElem; SectElem.Initialize();
		const auto& itSE = m_mapCpgSectElem.find(ElemK);
		if (itSE == m_mapCpgSectElem.end()) continue;
		SectElem = itSE->second;

		if (SectElem.bGenSect) continue; // General 단면은 통합엔진으로 진행하지 않음

		BOOL bGetCgre = FALSE;
		if (ElemK.second == EN_EL_BEAM)
		{
			bGetCgre = m_pDoc->m_pAttrCtrl2->GetCgre(ElemK.first, CgreD);
		}
		else if (ElemK.second == EN_EL_VBEAM)
		{
			bGetCgre = m_pDoc->m_pAttrCtrl2->GetCgrv(ElemK.first, CgreD);
		}
		else ASSERT(0);
		

		if (bGetCgre)
		{
			// 설계 대상이 아닌 단면을 물고 있는 요소를 입력할 수 가 있는 상태여서 추가..
			BOOL bpErr[5] = { FALSE,FALSE,FALSE,FALSE,FALSE };
			CString strMsg = _T("");
			if (!CDBLib::IsPlateGirderDgnFor2nd(ElemK, bpErr, strMsg)) continue;

			BOOL bGetCgdp = FALSE;
			T_CGDP_D CgdpD; CgdpD.Initialize();
			if (ElemK.second == EN_EL_BEAM)
			{
				bGetCgdp = m_pDoc->m_pAttrCtrl->GetCgdp(ElemK.first, CgdpD);
			}
			else if (ElemK.second == EN_EL_VBEAM)
			{
				bGetCgdp = m_pDoc->m_pAttrCtrl->GetCgdv(ElemK.first, CgdpD);
			}
			else ASSERT(0);
			
			if (bGetCgdp)
			{
				if ( CgreD.iDgnPosi == 0 && CgdpD.iDgnPosi == 1 ) continue;
				if ( CgreD.iDgnPosi == 1 && CgdpD.iDgnPosi == 0 ) continue;

				switch ( CgreD.iDgnPosi )
				{
				case 0:
				case 1:
					vRptElem.push_back(std::make_pair(ElemK, CgreD.iDgnPosi));
					break;
				case 2:
					vRptElem.push_back(std::make_pair(ElemK, 0));
					vRptElem.push_back(std::make_pair(ElemK, 1));
					break;
				default:
					ASSERT(0);
					break;
				}
			}
		}
	}

	auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
	if ( pDocCtrl == nullptr ) return false;
	dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(m_pDoc);

	CCalcType CType(GetDgnCode(), GetDgnType(), GetDgnMembType());

	CItemList ItemList;
	if ( pDgnDoc->GetDgnCalc()->GetPerformSectionItem(CType, ItemList) == 0 )
	{
		ASSERT(0); return false;
	}
	std::vector<item::enDgnResultItem> ResultItem = ItemList.GetList();
	int nSize = ResultItem.size();

	for ( auto itr : vRptElem )
	{
		auto nElemK = itr.first;

		std::vector<std::pair<item::enDgnResultItem, std::pair<def::CDgnInput*, def::ReportCrit>>> ItemInputList;
		auto* pRptInfo = new dgn::rpt::CDgnReportInfo;
		pRptInfo->DgnType = enDgn::CSG;
		pRptInfo->MembType = enMemb::Beam;
		auto* pRptName = new dgn::rpt::CRptMemberNameInfo;
		pRptName->MembK = nElemK.first;
		CString strPos = itr.second == 0 ? _T("i") : _T("j");
		CString strElemPos; strElemPos.Format(_T("%d - %s"), itr.first, strPos);
		CString strMemberName;
		strMemberName.Format(_T("Steel Composite  :  %s"), strElemPos);
		pRptName->strMemberName = CT2W(strMemberName);
		pRptInfo->PushReportInfo(pRptName);
		auto* pRptDgnCode = new dgn::rpt::CRptDesignCodeInfo;
		CString strCodeName = CDBLib::GetCsgCodeName(CpgdD.iDgnCode);
		CString strNA = CNationalAnnexTool::ConvertNationalAnnex(CpgdD.nSubCode);
		CString strCsgCodeName;
		strCsgCodeName.Format(_T("%s (NA : %s)"), strCodeName, strNA);
		pRptDgnCode->strDesignCode = CT2W(strCsgCodeName);
		pRptInfo->PushReportInfo(pRptDgnCode);

		auto* pRptSection = new dgn::rpt::CRptSectionInfo;
		T_SECT_D SectD;   SectD.Initialize();
		m_pDoc->m_pPostCtrl->GetSectPostDgn(nElemK, SectD);
		pRptSection->strSectionName = CT2W(SectD.SName);
		T_SECT_SECTBASE_D& SectB = (itr.second == 1 && SectD.nStype == D_SECT_TYPE_TAPERED) ? SectD.CmpTapJ : SectD.SectBefore.SectI;
		for ( int j = 0; j < 14; ++j ) // 14 -> CRptSectionInfo의 dSize
		{
			pRptSection->dSize[j] = SectB.Size[j];
		}
		pRptInfo->PushReportInfo(pRptSection);

		auto* pRptMembLength = new dgn::rpt::CRptMemberLengthInfo;
		pRptMembLength->dL = CDBLib::GetLengthOfBeam(nElemK);
		pRptInfo->PushReportInfo(pRptMembLength);

		CPG_MATL_ELEM MatlElem; MatlElem.Initialize();
		Get_CpgMatlElem(nElemK, MatlElem);
		const T_MATD_D& MatdD = MatlElem.MatdD;
		auto* pRptMaterialSTL = new dgn::rpt::CRptMaterialNameSTLInfo;
		pRptMaterialSTL->dFy = MatdD.Data1.Design.S_Fy1;
		pRptMaterialSTL->dEs = MatdD.Data1.Analysis.Elast;
		pRptInfo->PushReportInfo(pRptMaterialSTL);
		auto* pRptMaterialRC = new dgn::rpt::CRptMaterialNameRCInfo;
		pRptMaterialRC->dfck = MatlElem.dfck;
		pRptMaterialRC->dEc  = MatlElem.dEcm;
		pRptMaterialRC->dfy  = MatdD.MainRebarData.B_fy;
		if ( MatdD.Data2.CodeName.CompareNoCase(_T("None")) == 0)
		{
			pRptMaterialRC->dEs = MatdD.Data1.Analysis.Elast;
		}
		else
		{
			pRptMaterialRC->dEs = MatdD.MainRebarData.B_Elast;
		}
		
		pRptInfo->PushReportInfo(pRptMaterialRC);

		//이걸 어쩔쓰까나
		SetCheckElemPairK(nElemK);
		SetCheckPosition(itr.second);
		for ( int i = 0; i < nSize; ++i )
		{
			std::pair<item::enDgnResultItem, std::pair<def::CDgnInput*, def::ReportCrit>> pairCritic;
			auto Item = ResultItem[i];

			TTargetMembItem MembItem;
			MembItem.Sector = enSector::None;
			MembItem.Item = Item;

			TCriticalInfo Info;
			Info.iDcasK = INTEGRATIONDESIGNCASE;

			ReportCrit Critic;

			//불리한 하중조합과 그때 position으로 input data를 만들어줘야 한다.
			int iDgnLcomNo = 0, iMaxMin = 0;
			UINT OrgLcomK = 0;
			bool bCS = false;
			switch ( Item )
			{
			case dgn::item::enDgnResultItem::BEND_YMNP:
				{
					if ( !CpgdD.bStrength[0] )
						continue;

					T_PGBR_LCOM PgbrLcomD;
					if ( !m_pAnalysisResult->GetCpgBendingResistanceResult(nElemK, 0, 2, PgbrLcomD) )
					{
						ASSERT(0); continue;
					}

					OrgLcomK = PgbrLcomD.PgbrBase[itr.second].OrgLcomK;
					iDgnLcomNo = PgbrLcomD.PgbrBase[itr.second].LcomK;
					iMaxMin = PgbrLcomD.PgbrBase[itr.second].iMax;
				}
				break;
			case dgn::item::enDgnResultItem::BEND_YMPP:
				{
					if ( !CpgdD.bStrength[0] )
						continue;

					T_PGBR_LCOM PgbrLcomD;
					if ( !m_pAnalysisResult->GetCpgBendingResistanceResult(nElemK, 0, 1, PgbrLcomD) )
					{
						ASSERT(0); continue;
					}

					OrgLcomK = PgbrLcomD.PgbrBase[itr.second].OrgLcomK;
					iDgnLcomNo = PgbrLcomD.PgbrBase[itr.second].LcomK;
					iMaxMin = PgbrLcomD.PgbrBase[itr.second].iMax;
				}
				break;
			case dgn::item::enDgnResultItem::SHEAR_ZF_P:
			case dgn::item::enDgnResultItem::SHEAR_VALUE_ZF_P:
				{
					if ( !CpgdD.bStrength[1] )
						continue;

					T_PRVS_D PrvsD;
					if ( !m_pAnalysisResult->GetCpgVerticalShearResistanceResult(nElemK, 0, 1, PrvsD) )
					{
						ASSERT(0); continue;
					}

					OrgLcomK = PrvsD.PrvsBase[itr.second].OrgLcomK;
					iDgnLcomNo = PrvsD.PrvsBase[itr.second].LcomK;
					iMaxMin = PrvsD.PrvsBase[itr.second].iMax;
				}
				break;
			case dgn::item::enDgnResultItem::CS_0:
			case dgn::item::enDgnResultItem::CS_1:
			case dgn::item::enDgnResultItem::CS_2:
				{
					if ( !CpgdD.bStrength[2] )
						continue;

					T_PLTB_D PltbD;
					if ( !m_pAnalysisResult->GetPltbResultData(nElemK, PltbD) )
					{
						ASSERT(0); continue;
					}

					OrgLcomK = PltbD.PltbBase[itr.second].OrgLcomK;
					iDgnLcomNo = PltbD.PltbBase[itr.second].LcomK;
					iMaxMin = PltbD.PltbBase[itr.second].iMax;
					bCS = true;
				}
				break;
			case dgn::item::enDgnResultItem::LTB_INTERACTION____P:
				//case dgn::item::enDgnResultItem::LTB_BENDING__M_P:
				{
					if ( !CpgdD.bStrength[2] )
						continue;

					T_PLTB_D PltbD;
					if ( !m_pAnalysisResult->GetPltbResultData(nElemK, PltbD) )
					{
						ASSERT(0); continue;
					}

					OrgLcomK = PltbD.PltbBase[itr.second].OrgLcomK;
					iDgnLcomNo = PltbD.PltbBase[itr.second].LcomK;
					iMaxMin = PltbD.PltbBase[itr.second].iMax;
				}
				break;
			case dgn::item::enDgnResultItem::TRANSVERSE_FORCE_INTER___P:
				{
					if ( !CpgdD.bStrength[3] )
						continue;

					T_PRTF_D PrtfD;
					if ( !m_pAnalysisResult->GetPrtfResultData(nElemK, PrtfD) )
					{
						ASSERT(0); continue;
					}
					OrgLcomK = PrtfD.PrtfBase[itr.second].OrgLcomK;
					iDgnLcomNo = PrtfD.PrtfBase[itr.second].LcomK;
					iMaxMin = PrtfD.PrtfBase[itr.second].iMax;
				}
				break;
			case dgn::item::enDgnResultItem::LONG_SHEAR_FOR__F_P:
			case dgn::item::enDgnResultItem::LONG_SHEAR_DC__F_P:
				{
					if ( !CpgdD.bStrength[4] )
						continue;

					T_PGLS_D PglsD;
					if ( !m_pAnalysisResult->GetPglsResultData(nElemK, PglsD) )
					{
						ASSERT(0); continue;
					}
					OrgLcomK = PglsD.PglsBase[itr.second].OrgLcomK;
					iDgnLcomNo = PglsD.PglsBase[itr.second].LcomK;
					iMaxMin = PglsD.PglsBase[itr.second].iMax;
				}
				break;
			case dgn::item::enDgnResultItem::FATIGUE_SC_STR__S_P:
			case dgn::item::enDgnResultItem::FATIGUE_SC_VR___P:
				{
					if ( !CpgdD.bStrength[5] )
						continue;

					T_PGRF_D PgrfD;
					if ( !m_pAnalysisResult->GetPgrfResultData(nElemK, PgrfD) )
					{
						ASSERT(0); continue;
					}

					OrgLcomK = PgrfD.PgrfBase[itr.second].OrgLcomK;
					iDgnLcomNo = PgrfD.PgrfBase[itr.second].LcomK;
					iMaxMin = PgrfD.PgrfBase[itr.second].iMax;
				}
				break;
			case dgn::item::enDgnResultItem::STRS_STC_S_P:
			case dgn::item::enDgnResultItem::STRS_ST__SNP:
			case dgn::item::enDgnResultItem::STRS_ST__SPP:
				{
					if ( !CpgdD.bService[0] )
						continue;

					T_PGBS_D PgbsD;
					if ( !m_pAnalysisResult->GetCpgBeamStressResistanceResult(nElemK, 0, 1, PgbsD) )
					{
						ASSERT(0); continue;
					}

					if ( Item == dgn::item::enDgnResultItem::STRS_STC_S_P )
					{
						OrgLcomK = PgbsD.PgbsBase[itr.second].ChkGirder.OrgLcomK;
						iDgnLcomNo = PgbsD.PgbsBase[itr.second].ChkGirder.LcomK;
						iMaxMin = PgbsD.PgbsBase[itr.second].ChkGirder.iMax;
					}
					else if ( Item == dgn::item::enDgnResultItem::STRS_ST__SNP )
					{
						OrgLcomK = PgbsD.PgbsBase[itr.second].ChkBar.OrgLcomK;
						iDgnLcomNo = PgbsD.PgbsBase[itr.second].ChkBar.LcomK;
						iMaxMin = PgbsD.PgbsBase[itr.second].ChkBar.iMax;
					}
					else if ( Item == dgn::item::enDgnResultItem::STRS_ST__SPP )
					{
						OrgLcomK = PgbsD.PgbsBase[itr.second].ChkSlab.OrgLcomK;
						iDgnLcomNo = PgbsD.PgbsBase[itr.second].ChkSlab.LcomK;
						iMaxMin = PgbsD.PgbsBase[itr.second].ChkSlab.iMax;
					}
					else ASSERT(0);

					int nFortype = 0;
					int nLcomNum = m_pForcCtrl->Get_LcomDataCount();
					ADGNSTRESS aStressLcom;
					ADGNSTRESS aStressTnpr;
					if ( !m_pForcCtrl->Get_LcomDataForStress(nElemK, nFortype, TRUE, FALSE, aStressLcom, aStressTnpr, nLcomNum) ) { continue; }

					nFortype = 2;
					ADGNSTRESS aStressLcomP2;
					ADGNSTRESS aStressTnprP2;
					if ( !m_pForcCtrl->Get_LcomDataForStress(nElemK, nFortype, TRUE, FALSE, aStressLcomP2, aStressTnprP2, nLcomNum) ) { continue; }
				}
				break;
			case dgn::item::enDgnResultItem::SLS_LONG_SHEAR_DC__F_P:
				{
					if ( !CpgdD.bService[1] )
						continue;

					T_PLSS_D PlssD;
					if ( !m_pAnalysisResult->GetPlssResultData(nElemK, PlssD) )
					{
						ASSERT(0); continue;
					}
					OrgLcomK = PlssD.PlssBase[itr.second].OrgLcomK;
					iDgnLcomNo = PlssD.PlssBase[itr.second].LcomK;
					iMaxMin = PlssD.PlssBase[itr.second].iMax;
				}
				break;
			default:
				//ASSERT(0);//추가해줘야지.
				break;
			}

			if ( iDgnLcomNo == 0 )
				continue;

			T_LCOM_D LcomD; LcomD.Initialize();
			CString strLcomName = _T("");
			if ( bCS )
			{
				Critic.aInfo[ReportCrit::LcomName] = _T("Construction Stage");
			}
			else if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, OrgLcomK, LcomD) )
			{
				Critic.aInfo[ReportCrit::LcomName] = CT2W(LcomD.LoadCombName);
			}
			else
			{
				Critic.aInfo[ReportCrit::LcomName] = CT2W(_T("-"));
			}

			SetDgnLcomNo(iDgnLcomNo);
			auto* pDgnInput = new CDgnInput;
			IDgnCodeCheck::GetDgnInput(*pDgnInput, enDgnMode::Checking);
			pairCritic.first = Item;
			pairCritic.second = std::make_pair(pDgnInput, Critic);

			ItemInputList.push_back(pairCritic);
		}

		if ( ItemInputList.size() == 0 )
		{
			_SAFE_DELETE(pRptName);
			_SAFE_DELETE(pRptInfo);
			continue;
		}

		vMembInfo.push_back(pRptInfo);
		vMembItemInput.push_back(ItemInputList);
	}

	return true;
}

dgn::def::enDgnCode CDgnPlateGirder::GetDgnCode() const
{
	return dgn::converter::CDgnTypeConverter::GetDgnCode(m_iCivilCode, m_nNatinoAnnex);
}

dgn::def::enDgn CDgnPlateGirder::GetDgnType() const
{
	return dgn::def::enDgn::CSG;
}

dgn::def::enMemb CDgnPlateGirder::GetDgnMembType() const
{
	return dgn::def::enMemb::Beam;
}

UINT CDgnPlateGirder::GetLcomSize()
{
	UINT unLcomNum = 1; // 하나씩 돌린다?
	return unLcomNum;
}

UINT CDgnPlateGirder::GetDgnElemNo() const
{
	ASSERT(m_nThread == 1);
	return __super::GetCheckElem();
}

UINT CDgnPlateGirder::GetDgnPosiNo() const
{
	ASSERT(m_nThread == 1);
	return __super::GetCheckPosition();
}

std::wstring CDgnPlateGirder::GetDgnCodeName()
{
	CString strCodeName = CDBLib::GetCsgCodeName(m_iCivilCode);
	return static_cast<std::wstring>(CT2W(strCodeName));
}

std::wstring CDgnPlateGirder::GetDgnElemName()
{
	CString sElem; sElem.Format(_T("%d"), GetCheckElem());
	return static_cast<std::wstring>(CT2W(sElem));
}

std::wstring CDgnPlateGirder::GetDgnPosiName()
{
	CString strPosiNo = m_pForcCtrl->m_pDgnForceCtrl->Get_PosiNa(GetCheckPosition());
	std::wstring sPosi = static_cast<std::wstring>(CT2W(strPosiNo));

	return sPosi;
}

std::wstring CDgnPlateGirder::GetDgnLcomName()
{
	CString strLcomName = _T(""); //???
	return static_cast<std::wstring>(CT2W(strLcomName));
}

std::wstring CDgnPlateGirder::GetDgnPosiName(int nPos)
{
	CString strPosName = m_pForcCtrl->m_pDgnForceCtrl->Get_PosiNa(nPos);
	std::wstring sPosi = static_cast<std::wstring>(CT2W(strPosName));

	return sPosi;
}

std::wstring CDgnPlateGirder::GetDgnLcomName(CString strLcom)
{
	return static_cast<std::wstring>(CT2W(strLcom));
}

void CDgnPlateGirder::GetDgnCurrentUnit(std::wstring& ForCU, std::wstring& LenCU)
{
	T_UNIT_INDEX CodeUnit = GetCodeUnit();

	CString strForU = m_pDoc->m_pUnitCtrl->Get_ForUnit(CodeUnit.nBase_Force);
	CString strLenU = m_pDoc->m_pUnitCtrl->Get_LenUnit(CodeUnit.nBase_Length);

	ForCU = static_cast<std::wstring>(CT2W(strForU));
	LenCU = static_cast<std::wstring>(CT2W(strLenU));
}

void CDgnPlateGirder::GetRptSettings(const CString& strMembTypeName, dgn::def::CRptSettings& rSettings)
{
	rSettings.Init();

	CString strName = m_pDoc->GetPathName();

	enReportType enRptType = enReportType::None;
	enReportLevel enRptLevel = enReportLevel::Detail;
	if (CLockMgr::Get_IsRegressionTest() )
	{
		enRptType = enReportType::PlainText;
		enRptLevel = enReportLevel::Regression;

		CFileCtrl FileCtrl(m_strTestDgnFile);
		strName = FileCtrl.GetFilePathAndNameWithoutExtension();
	}
	else
	{
		enRptType = enReportType::WinWordRTF;

		CString strExt = _T("");
		strExt.Format(_T("%s.doc"), strMembTypeName);

		CFileCtrl FileCtrl(strName);
		strName = FileCtrl.GetFilePathAndNameWithoutExtension() + strExt;
		if ( FileCtrl.FileExists(strName) )
		{
			strName = IDgnBaseInternalLib::Instance()->GetNewStringName(strName);
		}
	}

	const wstring strFileName = CT2W(strName);

	rSettings.SetUserPref(true);
	rSettings.SetDgnCode(GetDgnCode());
	rSettings.SetDgnType(enDgn::CSG);
	rSettings.SetFileOutType(enRptType);
	rSettings.SetFileOutPath(strFileName);
	rSettings.SetReportLevel(enRptLevel);
	rSettings.SetRptUnit(enPredefinedCode);

	rSettings.SetDesignCase(INTEGRATIONDESIGNCASE);
	rSettings.SetMinSigFig(3);
	rSettings.SetReportLanguage(CDgnCodeCtrl::GetCpgLocalePref());
}


bool CDgnPlateGirder::GetDesignCode(dgn::def::CDesignCode* pData) const
{
	T_UNIT_INDEX CodeUnit = GetCodeUnit();
	pData->STL = GetDgnCode();
	pData->STL_unitL = static_cast<enLengthUnit>(CodeUnit.nBase_Length);
	pData->STL_unitF = static_cast<enForceUnit>(CodeUnit.nBase_Force);

	return true;
}

bool CDgnPlateGirder::GetMemberInfo(dgn::def::CMemberInfo* pData) const
{
	UINT nElemNo = GetDgnElemNo();
	pData->Key = static_cast<long>(nElemNo);
	pData->DgnType = GetDgnType();
	pData->MembType = GetDgnMembType();
	return true;
}

bool CDgnPlateGirder::GetMemberInfoArray(dgn::def::CMemberInfoArray* pData) const
{
	dgn::def::CMemberInfo MembInfo;
	GetMemberInfo(&MembInfo);
	pData->vMembInfo.push_back(MembInfo);

	return true;
}
bool CDgnPlateGirder::GetSection(dgn::def::CSection* pData) const
{
	return true;
	//     CPG_SECT_ELEM SectElem; SectElem.Initialize();
	//     Get_CpgSectElem(GetIDgnCheckElem(std::this_thread::get_id()), SectElem);
	// 
	//     UINT nPos = GetIDgnCheckPosition(std::this_thread::get_id());
	//     bool bPosiI = (nPos==0);
	// 
	//     const T_SECT_D& TSectD = SectElem.SectD;
	// 
	//     pData->strShape    = CT2W(SectElem.SectD.SectBefore.Shape);
	//     pData->strSection  = CT2W(SectElem.SectD.SName);
	//     pData->enSectType  = dgn::def::CSection::EN_BUILT_UP;
	//     // 어떤 순서로?
	//     pData->adSize[0]  = m_pDgnSectUtil->Get_Size_H(&TSectD, bPosiI);
	//     pData->adSize[1]  = m_pDgnSectUtil->Get_Size_bft(&TSectD, bPosiI);
	//     pData->adSize[2]  = m_pDgnSectUtil->Get_Size_tw(&TSectD, 0, bPosiI);
	//     pData->adSize[3]  = m_pDgnSectUtil->Get_Size_tft(&TSectD, bPosiI);
	//     pData->adSize[4]  = m_pDgnSectUtil->Get_Size_bfb(&TSectD, bPosiI);;
	//     pData->adSize[5]  = m_pDgnSectUtil->Get_Size_tfb(&TSectD, bPosiI);
	//     //pData->adSize[6]  = m_D7[m_iPosiNo];
	//     //pData->adSize[7]  = m_D8[m_iPosiNo];
	//     //pData->adSize[8]  = m_D9[m_iPosiNo];
	//     //pData->adSize[9]  = m_D10[m_iPosiNo];
	//     switch ( SectElem.SectD.SectBefore.nStype )
	//     {
	//     case D_SECT_TYPE_COMPO_I:    pData->enSectShape = dgn::def::CSection::EN_SECT_SHAPE_CMP_I;       break;
	//     case D_SECT_TYPE_COMPO_B:    pData->enSectShape = dgn::def::CSection::EN_SECT_SHAPE_CMP_B;       break;
	//     case D_SECT_TYPE_COMPO_TUB:  pData->enSectShape = dgn::def::CSection::EN_SECT_SHAPE_CMP_TUB;     break;
	//     default: ASSERT(0); break;
	//     }
	// 
	//     return true;
}

bool CDgnPlateGirder::GetMaterial(dgn::def::CMaterial* pData) const
{
	CPG_MATL_ELEM MatlElem; MatlElem.Initialize();
	Get_CpgMatlElem(GetIDgnCheckElem(std::this_thread::get_id()), MatlElem);
	pData->strCode = CT2W(MatlElem.MatdD.Data1.CodeName);
	pData->strName = CT2W(MatlElem.MatdD.Data1.CodeMatlName);
	return true;
}

bool CDgnPlateGirder::GetSectionProperty(dgn::def::CSectionProperty* pData) const
{
	CPG_SECT_ELEM SectElem; SectElem.Initialize();
	Get_CpgSectElem(GetIDgnCheckElem(std::this_thread::get_id()), SectElem);

	UINT nPos = GetIDgnCheckPosition(std::this_thread::get_id());
	BOOL bPosiI = (nPos == 0);

	T_SECT_STIFFNESS GirderStiff;
	Get_GirderStiffnessofCompo(bPosiI, SectElem.SectD, GirderStiff);

	T_SECT_DESIGN GirderDgnStiff;
	Get_GirderStiffnessofCompo(bPosiI, SectElem.SectD, GirderDgnStiff);

	const T_SECT_STIFFNESS& RbarStiff = SectElem.SectPosi[nPos].RebarSect;
	// property가 여러개 필요한데...

	pData->adProp[dgn::def::CSectionProperty::Area] = GirderStiff.Area;
	pData->adProp[dgn::def::CSectionProperty::J] = GirderStiff.Rxx;
	pData->adProp[dgn::def::CSectionProperty::Ix] = GirderStiff.Ryy;
	pData->adProp[dgn::def::CSectionProperty::Iy] = GirderStiff.Rzz;
	//pData->adProp[dgn::def::CSectionProperty::Ixy]  = GirderDgnStiff.
	pData->adProp[dgn::def::CSectionProperty::Sx] = GirderDgnStiff.Syy;
	pData->adProp[dgn::def::CSectionProperty::Sy] = GirderDgnStiff.Szz;
	pData->adProp[dgn::def::CSectionProperty::Zx] = GirderDgnStiff.Zyy;
	pData->adProp[dgn::def::CSectionProperty::Zy] = GirderDgnStiff.Zzz;
	pData->adProp[dgn::def::CSectionProperty::Cx] = GirderStiff.Cym;
	pData->adProp[dgn::def::CSectionProperty::Cxm] = GirderStiff.Cym;
	pData->adProp[dgn::def::CSectionProperty::Cxp] = GirderStiff.Cyp;
	pData->adProp[dgn::def::CSectionProperty::Cy] = GirderStiff.Czm;
	pData->adProp[dgn::def::CSectionProperty::Cym] = GirderStiff.Czm;
	pData->adProp[dgn::def::CSectionProperty::Cyp] = GirderStiff.Czp;
	pData->adProp[dgn::def::CSectionProperty::x1] = GirderStiff.dx1;
	pData->adProp[dgn::def::CSectionProperty::y1] = GirderStiff.dy1;
	pData->adProp[dgn::def::CSectionProperty::x2] = GirderStiff.dx2;
	pData->adProp[dgn::def::CSectionProperty::y2] = GirderStiff.dy2;
	pData->adProp[dgn::def::CSectionProperty::x3] = GirderStiff.dx3;
	pData->adProp[dgn::def::CSectionProperty::y3] = GirderStiff.dy3;
	pData->adProp[dgn::def::CSectionProperty::x4] = GirderStiff.dx4;
	pData->adProp[dgn::def::CSectionProperty::y4] = GirderStiff.dy4;
	pData->adProp[dgn::def::CSectionProperty::Xbar] = GirderDgnStiff.YBar;
	pData->adProp[dgn::def::CSectionProperty::Ybar] = GirderDgnStiff.ZBar;
	pData->adProp[dgn::def::CSectionProperty::Rox] = EQ0(GirderDgnStiff.Roy) ? sqrt(SafeDiv(GirderStiff.Ryy, GirderStiff.Area)) : GirderDgnStiff.Roy;
	pData->adProp[dgn::def::CSectionProperty::Roy] = EQ0(GirderDgnStiff.Roz) ? sqrt(SafeDiv(GirderStiff.Rzz, GirderStiff.Area)) : GirderDgnStiff.Roz;
	pData->adProp[dgn::def::CSectionProperty::Rop] = GirderDgnStiff.Rop;
	pData->adProp[dgn::def::CSectionProperty::Asx] = GirderStiff.Asy;
	pData->adProp[dgn::def::CSectionProperty::Asy] = GirderStiff.Asz;
	pData->adProp[dgn::def::CSectionProperty::Cw] = GirderDgnStiff.Cwp;
	//     pData->adProp[dgn::def::CSectionProperty::Qxb] = m_Qyb[m_iPosiNo];
	//     pData->adProp[dgn::def::CSectionProperty::Qyb] = m_Qzb[m_iPosiNo];
	return true;
}

bool CDgnPlateGirder::GetCompositeSectionProperty(dgn::def::CCompositeSectionProperty* pData) const
{
	CPG_SECT_ELEM SectElem; SectElem.Initialize();
	Get_CpgSectElem(GetIDgnCheckElem(std::this_thread::get_id()), SectElem);

	UINT nPos = GetIDgnCheckPosition(std::this_thread::get_id());
	BOOL bPosiI = (nPos == 0);

	T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
	GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

	const T_SECT_STIFFNESS& RebarSectLT = SectElem.SectPosi[nPos].RbarStiff3n;
	const T_SECT_STIFFNESS& NonSlab     = SectElem.SectPosi[nPos].NonConSect;
	const T_SECT_STIFFNESS& NonSlabR3   = SectElem.SectPosi[nPos].NonConSect;  // NonSlabStiff2 이걸 사용 하면 바꾸자.

	pData->adProp[dgn::def::CSectionProperty::Area] = RebarSect.Area;
	pData->adProp[dgn::def::CSectionProperty::J] = RebarSect.Rxx;
	pData->adProp[dgn::def::CSectionProperty::Ix] = RebarSect.Ryy;
	pData->adProp[dgn::def::CSectionProperty::Iy] = RebarSect.Rzz;
	//pData->adProp[dgn::def::CSectionProperty::Ixy]  = GirderDgnStiff.
	//pData->adProp[dgn::def::CSectionProperty::Sx] = GirderDgnStiff.Syy;
	//pData->adProp[dgn::def::CSectionProperty::Sy] = GirderDgnStiff.Szz;
	//pData->adProp[dgn::def::CSectionProperty::Zx] = GirderDgnStiff.Zyy;
	//pData->adProp[dgn::def::CSectionProperty::Zy] = GirderDgnStiff.Zzz;
	pData->adProp[dgn::def::CSectionProperty::Cx] = RebarSect.Cym;
	pData->adProp[dgn::def::CSectionProperty::Cxm] = RebarSect.Cym;
	pData->adProp[dgn::def::CSectionProperty::Cxp] = RebarSect.Cyp;
	pData->adProp[dgn::def::CSectionProperty::Cy] = RebarSect.Czm;
	pData->adProp[dgn::def::CSectionProperty::Cym] = RebarSect.Czm;
	pData->adProp[dgn::def::CSectionProperty::Cyp] = RebarSect.Czp;
	pData->adProp[dgn::def::CSectionProperty::x1] = RebarSect.dx1;
	pData->adProp[dgn::def::CSectionProperty::y1] = RebarSect.dy1;
	pData->adProp[dgn::def::CSectionProperty::x2] = RebarSect.dx2;
	pData->adProp[dgn::def::CSectionProperty::y2] = RebarSect.dy2;
	pData->adProp[dgn::def::CSectionProperty::x3] = RebarSect.dx3;
	pData->adProp[dgn::def::CSectionProperty::y3] = RebarSect.dy3;
	pData->adProp[dgn::def::CSectionProperty::x4] = RebarSect.dx4;
	pData->adProp[dgn::def::CSectionProperty::y4] = RebarSect.dy4;
	//pData->adProp[dgn::def::CSectionProperty::Xbar] = GirderDgnStiff.YBar;
	pData->adProp[dgn::def::CSectionProperty::Ybar] = RebarSect.Czm;
	//pData->adProp[dgn::def::CSectionProperty::Rox] = EQ0(GirderDgnStiff.Roy) ? sqrt(SafeDiv(RebarSect.Ryy, RebarSect.Area)) : GirderDgnStiff.Roy;
	//pData->adProp[dgn::def::CSectionProperty::Roy] = EQ0(GirderDgnStiff.Roz) ? sqrt(SafeDiv(RebarSect.Rzz, RebarSect.Area)) : GirderDgnStiff.Roz;
	//pData->adProp[dgn::def::CSectionProperty::Rop] = GirderDgnStiff.Rop;
	pData->adProp[dgn::def::CSectionProperty::Asx] = RebarSect.Asy;
	pData->adProp[dgn::def::CSectionProperty::Asy] = RebarSect.Asz;
	//pData->adProp[dgn::def::CSectionProperty::Cw] = GirderDgnStiff.Cwp;
	//     pData->adProp[dgn::def::CSectionProperty::Qxb] = m_Qyb[m_iPosiNo];
	//     pData->adProp[dgn::def::CSectionProperty::Qyb] = m_Qzb[m_iPosiNo];

	pData->adPropLT[dgn::def::CSectionProperty::Area] = RebarSectLT.Area;
	pData->adPropLT[dgn::def::CSectionProperty::J]    = RebarSectLT.Rxx;
	pData->adPropLT[dgn::def::CSectionProperty::Ix]   = RebarSectLT.Ryy;
	pData->adPropLT[dgn::def::CSectionProperty::Iy]   = RebarSectLT.Rzz;
	pData->adPropLT[dgn::def::CSectionProperty::Cx]   = RebarSectLT.Cym;
	pData->adPropLT[dgn::def::CSectionProperty::Cxm]  = RebarSectLT.Cym;
	pData->adPropLT[dgn::def::CSectionProperty::Cxp]  = RebarSectLT.Cyp;
	pData->adPropLT[dgn::def::CSectionProperty::Cy]   = RebarSectLT.Czm;
	pData->adPropLT[dgn::def::CSectionProperty::Cym]  = RebarSectLT.Czm;
	pData->adPropLT[dgn::def::CSectionProperty::Cyp]  = RebarSectLT.Czp;
	pData->adPropLT[dgn::def::CSectionProperty::x1]   = RebarSectLT.dx1;
	pData->adPropLT[dgn::def::CSectionProperty::y1]   = RebarSectLT.dy1;
	pData->adPropLT[dgn::def::CSectionProperty::x2]   = RebarSectLT.dx2;
	pData->adPropLT[dgn::def::CSectionProperty::y2]   = RebarSectLT.dy2;
	pData->adPropLT[dgn::def::CSectionProperty::x3]   = RebarSectLT.dx3;
	pData->adPropLT[dgn::def::CSectionProperty::y3]   = RebarSectLT.dy3;
	pData->adPropLT[dgn::def::CSectionProperty::x4]   = RebarSectLT.dx4;
	pData->adPropLT[dgn::def::CSectionProperty::y4]   = RebarSectLT.dy4;
	pData->adPropLT[dgn::def::CSectionProperty::Ybar] = RebarSectLT.Czm;
	pData->adPropLT[dgn::def::CSectionProperty::Asx]  = RebarSectLT.Asy;
	pData->adPropLT[dgn::def::CSectionProperty::Asy]  = RebarSectLT.Asz;

	pData->adPropNonSlab[dgn::def::CSectionProperty::Area] = NonSlab.Area;
	pData->adPropNonSlab[dgn::def::CSectionProperty::J]    = NonSlab.Rxx;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Ix]   = NonSlab.Ryy;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Iy]   = NonSlab.Rzz;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Cx]   = NonSlab.Cym;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Cxm]  = NonSlab.Cym;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Cxp]  = NonSlab.Cyp;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Cy]   = NonSlab.Czm;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Cym]  = NonSlab.Czm;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Cyp]  = NonSlab.Czp;
	pData->adPropNonSlab[dgn::def::CSectionProperty::x1]   = NonSlab.dx1;
	pData->adPropNonSlab[dgn::def::CSectionProperty::y1]   = NonSlab.dy1;
	pData->adPropNonSlab[dgn::def::CSectionProperty::x2]   = NonSlab.dx2;
	pData->adPropNonSlab[dgn::def::CSectionProperty::y2]   = NonSlab.dy2;
	pData->adPropNonSlab[dgn::def::CSectionProperty::x3]   = NonSlab.dx3;
	pData->adPropNonSlab[dgn::def::CSectionProperty::y3]   = NonSlab.dy3;
	pData->adPropNonSlab[dgn::def::CSectionProperty::x4]   = NonSlab.dx4;
	pData->adPropNonSlab[dgn::def::CSectionProperty::y4]   = NonSlab.dy4;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Ybar] = NonSlab.Czm;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Asx]  = NonSlab.Asy;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Asy]  = NonSlab.Asz;

	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Area] = NonSlabR3.Area;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::J]    = NonSlabR3.Rxx;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Ix]   = NonSlabR3.Ryy;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Iy]   = NonSlabR3.Rzz;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Cx]   = NonSlabR3.Cym;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Cxm]  = NonSlabR3.Cym;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Cxp]  = NonSlabR3.Cyp;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Cy]   = NonSlabR3.Czm;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Cym]  = NonSlabR3.Czm;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Cyp]  = NonSlabR3.Czp;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::x1]   = NonSlabR3.dx1;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::y1]   = NonSlabR3.dy1;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::x2]   = NonSlabR3.dx2;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::y2]   = NonSlabR3.dy2;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::x3]   = NonSlabR3.dx3;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::y3]   = NonSlabR3.dy3;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::x4]   = NonSlabR3.dx4;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::y4]   = NonSlabR3.dy4;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Ybar] = NonSlabR3.Czm;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Asx]  = NonSlabR3.Asy;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Asy]  = NonSlabR3.Asz;

	return true;
}

bool CDgnPlateGirder::GetMaterialSteel(dgn::def::CMaterialSteel* pData) const
{
	CPG_SECT_ELEM SectElem; SectElem.Initialize();
	Get_CpgSectElem(GetIDgnCheckElem(std::this_thread::get_id()), SectElem);

	UINT nPos = GetIDgnCheckPosition(std::this_thread::get_id());
	bool bPosiI = (nPos == 0);

	CPG_MATL_ELEM MatlElem; MatlElem.Initialize();
	Get_CpgMatlElem(GetIDgnCheckElem(std::this_thread::get_id()), MatlElem);

	const T_MATD_D& MatdD = MatlElem.MatdD;

	const double dFu = MatdD.Data1.Design.S_Fu;
	TMaterialSteelSingle Single1(MatdD.Data1.Design.S_Fy1, dFu);
	TMaterialSteelSingle Single2(MatdD.Data1.Design.S_Fy2, dFu);
	TMaterialSteelSingle Single3(MatdD.Data1.Design.S_Fy3, dFu);
	TMaterialSteelSingle Single4(MatdD.Data1.Design.S_Fy4, dFu);
	TMaterialSteelSingle Single5(MatdD.Data1.Design.S_Fy5, dFu);
	TMaterialSteelSingle Single6(MatdD.Data1.Design.S_Fy6, dFu);

	double dSteelThkWeb = m_pDgnSectUtil->Get_Size_tw(&SectElem.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dSteelThkFlangeTop = m_pDgnSectUtil->Get_Size_tft(&SectElem.SectD, bPosiI);
	double dSteelThkFlangeBot = m_pDgnSectUtil->Get_Size_tfb(&SectElem.SectD, bPosiI);

	double dMinThick = min(min(dSteelThkFlangeTop, dSteelThkFlangeBot), dSteelThkWeb);

	pData->Data.Common.dElastic = MatdD.Data1.Analysis.Elast;
	pData->Data.Common.dShear = MatdD.Data1.Analysis.ShearModulus;
	pData->Data.Common.dPoisson = MatdD.Data1.Analysis.Poisson;
	pData->Data.Common.dWeightDensity = MatdD.Data1.Analysis.Density;
	pData->Data.Steel.nList = 1;
	pData->Data.Steel.bUser = (MatdD.Data1.CodeName.CompareNoCase(_T("None")) == 0);

	pData->Data.Steel.aProp[0] = SteelThickProp(dMinThick, Single1);
	pData->Data.Steel.aProp[1] = SteelThickProp(dMinThick, Single2);
	pData->Data.Steel.aProp[2] = SteelThickProp(dMinThick, Single3);
	pData->Data.Steel.aProp[3] = SteelThickProp(dMinThick, Single4);
	pData->Data.Steel.aProp[4] = SteelThickProp(dMinThick, Single5);
	pData->Data.Steel.aProp[5] = SteelThickProp(dMinThick, Single6);

	//pData->ConcD.dfck = MatlElem.dfck;
	//pData->ConcD.dfy  = MatdD.MainRebarData2.B_fy;
	//pData->ConcD.dEc  = MatdD.Data2.Analysis.Elast;

	return true;
}

bool CDgnPlateGirder::GetEffectiveLength(dgn::def::CEffectiveLength* pData) const
{
	T_KFAC_D KFacD; KFacD.Initialize();

	bool bKfac = false;

	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	if (ElemK.second == EN_EL_BEAM)
	{
		if (m_pDoc->m_pAttrCtrl->GetKfac(ElemK.first, KFacD)) bKfac = TRUE;
	}
	else if (ElemK.second == EN_EL_VBEAM)
	{
		T_KFAV_D KFavD; KFavD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetKfav(ElemK.first, KFavD))
		{
			bKfac = TRUE;
			KFacD.BKy = KFavD.BKy;
			KFacD.BKz = KFavD.BKz;
			KFacD.BKt = KFavD.BKt;
		}
	}
	else ASSERT(0);

	if ( bKfac )
	{
		pData->dKy = KFacD.BKy;
		pData->dKz = KFacD.BKz;
	}
	else
	{
		pData->dKy = pData->dKz = 1.0;
	}

	return true;
}

bool CDgnPlateGirder::GetSteelGeneralSectParam(dgn::def::CSteelGeneralSectParam* pData) const
{
	pData->bGeneral = false; // only for nGen
//     pData->adProp[dgn::def::CSteelGeneralSectParam::Compression] = m_UFa;
//     pData->adProp[dgn::def::CSteelGeneralSectParam::Tension] = m_UFt;
//     pData->adProp[dgn::def::CSteelGeneralSectParam::Bending] = m_UFby;
//     pData->adProp[dgn::def::CSteelGeneralSectParam::ShearAwx] = m_UFbz;
//     pData->adProp[dgn::def::CSteelGeneralSectParam::ShearAwy] = m_UFv;

	return true;
}

bool CDgnPlateGirder::GetBendingCoefficient(dgn::def::CBendingCoefficient* pData) const
{
	//T_CBFT_D CbftD;
	//CbftD.Initialize();
	//m_pDgnDataCtrl->Get_DgnStlCbft(m_iElemNo, CbftD);

	//pData->Factor.v1 = CbftD.bAuto;
	//pData->Factor.v2 = (CbftD.bAuto ? -1.0 : CbftD.Cb);
	return true;
}

bool CDgnPlateGirder::GetShearCoefficient(dgn::def::CShearCoefficient* pData) const
{
	//     T_CVFT_D CvftD;
	//     CvftD.Initialize();
	//     m_pDgnDataCtrl->Get_DgnStlCvft(m_iElemNo, CvftD);
	//     pData->Factor.v1 = CvftD.bAuto;
	//     pData->Factor.v2 = (CvftD.bAuto ? 0.0 : CvftD.Cv
	return true;
}

bool CDgnPlateGirder::GetSafetyFactor(dgn::def::CSafetyFactor* pData) const
{
	//     pData->aFactor[dgn::def::CSafetyFactor::Tension] = dgn::lib::TFactor(true, m_dPhi[0]);
	//     pData->aFactor[dgn::def::CSafetyFactor::Compression] = dgn::lib::TFactor(true, m_dPhi[2]);
	//     pData->aFactor[dgn::def::CSafetyFactor::Bending] = dgn::lib::TFactor(true, m_dPhi[3]);
	//     pData->aFactor[dgn::def::CSafetyFactor::Shear] = dgn::lib::TFactor(true, m_dPhi[4]);
		//pData->aFactor[dgn::def::CSafetyFactor::Torsion    ] = dgn::def::CSafetyFactor::TFactor(true, m_dPhi[1]);

	return true;
}

bool CDgnPlateGirder::GetSeismicProperty(dgn::def::CSeismicProperty* pData) const
{
	pData->bSeismic = false;// m_bEqSpecial;
	pData->FrameType = dgn::def::enSeismicFrameType::None;

	return true;
}

bool CDgnPlateGirder::GetUnbracedLength(dgn::def::CUnbracedLength* pData) const
{
	CPG_MEMB_ELEM MembElem;  MembElem.Initialize();
	Get_CpgMembElem(GetIDgnCheckElem(std::this_thread::get_id()), MembElem);
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());

	pData->dLy = dgn::lib::TFactor(false, MembElem.MembPosi[unPos].dLy);
	pData->dLz = dgn::lib::TFactor(false, MembElem.MembPosi[unPos].dLz);
	pData->dLb = dgn::lib::TFactor(false, MembElem.MembPosi[unPos].dLu);

	return true;
}

bool CDgnPlateGirder::GetUnbracedForce(dgn::def::CForceUbln* pData) const
{
	return true;
}

bool CDgnPlateGirder::GetForce1DMember(dgn::def::CForce1DMember* pData) const
{
	const auto nSizeType = CForce1DMember::TypeNumber;
	const auto nSizeCatch = CForce1DMember::CatchNumber;

	typedef std::array<double, nSizeType> TypeList;
	typedef std::array<TypeList, nSizeCatch> CatchList;

	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());
	int nDgnLcom = GetIDgnLcomNo(std::this_thread::get_id());

	BOOL bReverse = FALSE;
	ElemPairK showElemK = { 0,0 }, IendElemK = { 0,0 }, JendElemK = { 0,0 };
	GetEndElemOfMember(ElemK, showElemK, IendElemK, JendElemK);

	_DGN_FORC_CRC ForI, ForJ;
	auto itrI = m_mForce.find(IendElemK);
	if ( itrI == m_mForce.end() )
	{
		ASSERT(0); return true;
	}
	auto& aForceLcomI = itrI->second.aForceLcom;
	int nLcomSizeI = aForceLcomI.GetSize();
	if ( nLcomSizeI > 0 )
	{
		int nDgnLcomIndex = nDgnLcom - 1;
		if ( nDgnLcom > 0 && nDgnLcomIndex < nLcomSizeI )
		{
			ForI = (aForceLcomI[nDgnLcomIndex]);
		}
	}

	auto itrJ = m_mForce.find(JendElemK);
	if ( itrJ == m_mForce.end() )
	{
		ASSERT(0); return true;
	}
	auto& aForceLcom = itrJ->second.aForceLcom;
	int nLcomSizeJ = aForceLcom.GetSize();
	if ( nLcomSizeJ > 0 )
	{
		int nDgnLcomIndex = nDgnLcom - 1;
		if ( nDgnLcom > 0 && nDgnLcomIndex < nLcomSizeJ )
		{
			ForJ = (aForceLcom[nDgnLcomIndex]);
		}
	}

	for ( int i = 0; i < 2; ++i )
	{
		_DGN_FORC_CRC& MembEndForce = i == 0 ? ForI : ForJ;
		TypeList aForce;
		aForce[CForce1DMember::My] = MembEndForce.dMuy[i];
		aForce[CForce1DMember::Fz] = MembEndForce.dFzz[i];

		pData->vdForceByPos.push_back(aForce);
	}

	return true;
}

bool CDgnPlateGirder::GetLcomInfo(dgn::def::CLcomInfo* pData) const
{
	CString strOrgLcomNa = _T("");
	int nOrgLcomNo = 0;
	int nSerLcomCount = 0;
	int nMaxMinType = 0;
	int nDgnLcom = GetIDgnLcomNo(std::this_thread::get_id());
	m_pForcCtrl->Get_LcomType(nDgnLcom, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType);

	T_LCOM_D LcomD;
	if ( !m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, nOrgLcomNo, LcomD) ) { ASSERT(0); LcomD.Initialize(); }

	BOOL bFatLcom = m_pForcCtrl->Is_LcomForFati(nDgnLcom);
	int nServLcomType = m_pForcCtrl->Get_ServLcomType(nDgnLcom);

	pData->strName = CT2W(LcomD.LoadCombName);
	pData->strDesc = CT2W(LcomD.Description);
	if ( LcomD.nActive == EN_DGN_NONE )
	{
		pData->unDgnType = dgn::def::CLcomInfo::EN_LCOM_NONE;
		pData->unSubType = dgn::def::CLcomInfo::EN_LCOM_SUB_NONE;
	}
	else
	{
		if ( bFatLcom )
		{
			pData->unDgnType = dgn::def::CLcomInfo::EN_LCOM_FATI;
		}
		else
		{
			pData->unDgnType = LcomD.nActive == EN_DGN_SERV ? dgn::def::CLcomInfo::EN_LCOM_SERV : dgn::def::CLcomInfo::EN_LCOM_STRN;
			pData->unSubType = dgn::def::CLcomInfo::EN_LCOM_SUB_NONE;
			if ( LcomD.nActive == EN_DGN_SERV )
			{
				switch ( nServLcomType )
				{
				case 1: pData->unSubType = dgn::def::CLcomInfo::EN_LCOM_SUB_QUAS; break;
				case 2: pData->unSubType = dgn::def::CLcomInfo::EN_LCOM_SUB_FREQ; break;
				case 3: pData->unSubType = dgn::def::CLcomInfo::EN_LCOM_SUB_CHAR; break;
				default: break;
				}
			}
		}
	}

	return true;
}

bool CDgnPlateGirder::GetForce1D(dgn::def::CForce1D* pData) const
{
	ASSERT(0); //Force1DArray 쓰면되
	return true;
}

bool CDgnPlateGirder::GetPartialFactor(dgn::def::CPartialFactor* pData) const
{
	pData->aFactor[dgn::def::CPartialFactor::GammaC_Fundamental] = dgn::lib::TFactor(false, m_CpgdD.dGamma_C);
	pData->aFactor[dgn::def::CPartialFactor::GammaS_Fundamental] = dgn::lib::TFactor(false, m_CpgdD.dGamma_S);
	pData->aFactor[dgn::def::CPartialFactor::GammaC_SLS] = dgn::lib::TFactor(false, m_CpgdD.dGamma_C);
	pData->aFactor[dgn::def::CPartialFactor::GammaS_SLS] = dgn::lib::TFactor(false, m_CpgdD.dGamma_M_ser);
	pData->aFactor[dgn::def::CPartialFactor::GammaM0] = dgn::lib::TFactor(false, m_CpgdD.dGamma_M0);
	pData->aFactor[dgn::def::CPartialFactor::GammaM1] = dgn::lib::TFactor(false, m_CpgdD.dGamma_M1);
	//pData->aFactor[dgn::def::CPartialFactor::GammaM2] = dgn::lib::TFactor(false, m_CpgdD.dGamma_);
	pData->aFactor[dgn::def::CPartialFactor::GammaV] = dgn::lib::TFactor(false, m_CpgdD.dGamma_V);
	pData->aFactor[dgn::def::CPartialFactor::GammaFf] = dgn::lib::TFactor(false, m_CpgdD.dGamma_Ff);
	pData->aFactor[dgn::def::CPartialFactor::GammaMf] = dgn::lib::TFactor(false, m_CpgdD.dGamma_Mf);
	pData->aFactor[dgn::def::CPartialFactor::GammaMfs] = dgn::lib::TFactor(false, m_CpgdD.dGamma_Mfs);

	return true;
}

bool CDgnPlateGirder::GetSectionShape(dgn::def::CSectionShape* pData) const
{
	CPG_SECT_ELEM SectElem; SectElem.Initialize();
	Get_CpgSectElem(GetIDgnCheckElem(std::this_thread::get_id()), SectElem);

	UINT nPos = GetIDgnCheckPosition(std::this_thread::get_id());
	bool bPosiI = (nPos == 0);
	CSectionBuilder Builder;

	//Tapered를 지원해야되나 보군.
	const T_SECT_D& TSectD = SectElem.SectD;
	switch ( SectElem.SectD.SectBefore.nStype )
	{	
	case D_SECT_TYPE_COMPO_I:
		{
			double dH = m_pDgnSectUtil->Get_Size_H(&TSectD, bPosiI);
			double dtw = m_pDgnSectUtil->Get_Size_tw(&TSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			double dBftop = m_pDgnSectUtil->Get_Size_bft(&TSectD, bPosiI);
			double dTfTop = m_pDgnSectUtil->Get_Size_tft(&TSectD, bPosiI);
			double dBfbot = m_pDgnSectUtil->Get_Size_bfb(&TSectD, bPosiI);
			double dTfBot = m_pDgnSectUtil->Get_Size_tfb(&TSectD, bPosiI);

			pData->pSectionShape = Builder.CreateH(dBftop, dH, dtw, dTfTop, dBfbot, dTfBot, 0.0, 0.0, enMakingType::Builtup);
		}
		break;
	case D_SECT_TYPE_COMPO_B:
		{
			double dH = m_pDgnSectUtil->Get_Size_H(&TSectD, bPosiI);
			double dtw = m_pDgnSectUtil->Get_Size_tw(&TSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			double dBftop = m_pDgnSectUtil->Get_Size_bft(&TSectD, bPosiI);
			double dTfTop = m_pDgnSectUtil->Get_Size_tft(&TSectD, bPosiI);
			double dBfbot = m_pDgnSectUtil->Get_Size_bfb(&TSectD, bPosiI);
			double dTfBot = m_pDgnSectUtil->Get_Size_tfb(&TSectD, bPosiI);
			double dCt = m_pDgnSectUtil->Get_Size_bft_m(&TSectD, bPosiI);
			double dCb = m_pDgnSectUtil->Get_Size_bfb_m(&TSectD, bPosiI);

			pData->pSectionShape = Builder.CreateBox(dBftop, dH, dBfbot, dtw, dTfTop, dTfBot, dCt, dCb, 0.0, enMakingType::Builtup);
		}
		break;
	case D_SECT_TYPE_COMPO_TUB:
		{
			double dH = m_pDgnSectUtil->Get_Size_H(&TSectD, bPosiI);
			double dtw = m_pDgnSectUtil->Get_Size_tw(&TSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			double dBftop = 0.5*m_pDgnSectUtil->Get_Size_bft(&TSectD, bPosiI);
			double dTfTop = m_pDgnSectUtil->Get_Size_tft(&TSectD, bPosiI);
			double dBfTopOut = m_pDgnSectUtil->Get_Size_bft_ll(&TSectD, bPosiI);
			double dBfbot = m_pDgnSectUtil->Get_Size_bfb(&TSectD, bPosiI);
			double dTfBot = m_pDgnSectUtil->Get_Size_tfb(&TSectD, bPosiI);
			double dBfBotOut = m_pDgnSectUtil->Get_Size_bfb_l(&TSectD, bPosiI);
			double dBfTopVoid = m_pDgnSectUtil->Get_Size_bft_m(&TSectD, bPosiI);
			double dBfBotCtC = m_pDgnSectUtil->Get_Size_bfb_m(&TSectD, bPosiI);
			double dBTop = 2.0*dBftop + dBfTopVoid;

			pData->pSectionShape = Builder.CreateTub(dH, dBTop, dBftop, dBfbot, dBfTopOut, dBfBotOut, dtw, dTfTop, dTfBot, enMakingType::Builtup);
		}
		break;
	case D_SECT_TYPE_COMPO_STLG_I:
		{
			double dH = m_pDgnSectUtil->Get_Size_H(&TSectD, bPosiI);
			double dtw = m_pDgnSectUtil->Get_Size_tw(&TSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			double dBfTL = m_pDgnSectUtil->Get_Size_bft_l(&TSectD, bPosiI);
			double dBfTR = m_pDgnSectUtil->Get_Size_bft_r(&TSectD, bPosiI);
			double dTfTop = m_pDgnSectUtil->Get_Size_tft(&TSectD, bPosiI);
			double dBfBL = m_pDgnSectUtil->Get_Size_bfb_l(&TSectD, bPosiI);
			double dBfBR = m_pDgnSectUtil->Get_Size_bfb_r(&TSectD, bPosiI);
			double dTfBot = m_pDgnSectUtil->Get_Size_tfb(&TSectD, bPosiI);
			double dDistT = m_pDgnSectUtil->Get_Size_DistTopFlange(&TSectD, bPosiI);
			double dDistB = m_pDgnSectUtil->Get_Size_DistBotFlange(&TSectD, bPosiI);
						
			pData->pSectionShape = Builder.CreateIUnsymY(dH, dBfTL, dBfTR, dBfBL, dBfBR, dtw, dTfTop, dTfBot, dDistT, dDistB, enMakingType::Builtup);
		}
		break;
	case D_SECT_TYPE_COMPO_STLG_B:
		{
			double dH = m_pDgnSectUtil->Get_Size_H(&TSectD, bPosiI);
			double dtwL = m_pDgnSectUtil->Get_Size_tw(&TSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			double dtwR = m_pDgnSectUtil->Get_Size_tw(&TSectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
			double dBfT = m_pDgnSectUtil->Get_Size_bft(&TSectD, bPosiI);
			double dBfB = m_pDgnSectUtil->Get_Size_bfb(&TSectD, bPosiI);
			double dBfTL = m_pDgnSectUtil->Get_Size_bft_l(&TSectD, bPosiI);
			double dBfTR = m_pDgnSectUtil->Get_Size_bft_r(&TSectD, bPosiI);
			double dTfTop = m_pDgnSectUtil->Get_Size_tft(&TSectD, bPosiI);
			double dBfBL = m_pDgnSectUtil->Get_Size_bfb_l(&TSectD, bPosiI);
			double dBfBR = m_pDgnSectUtil->Get_Size_bfb_r(&TSectD, bPosiI);
			double dTfBot = m_pDgnSectUtil->Get_Size_tfb(&TSectD, bPosiI);
			double dDistT = m_pDgnSectUtil->Get_Size_DistTopFlange(&TSectD, bPosiI);
			double dDistB = m_pDgnSectUtil->Get_Size_DistBotFlange(&TSectD, bPosiI);

			pData->pSectionShape = Builder.CreateBoxUnsymY(dH, dBfT, dBfB, dBfTL, dBfTR, dBfBL, dBfBR, dtwL, dtwR, dTfTop, dTfBot, dDistT, dDistB, enMakingType::Builtup);
		}
		break;		
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			double dH = m_pDgnSectUtil->Get_Size_H(&TSectD, bPosiI);
			double dBfTL = m_pDgnSectUtil->Get_Size_bft_l(&TSectD, bPosiI);
			double dBfTR = m_pDgnSectUtil->Get_Size_bft_r(&TSectD, bPosiI);
			double dBfTV = m_pDgnSectUtil->Get_Size_bft_m(&TSectD, bPosiI);
			double dBfTLOut = m_pDgnSectUtil->Get_Size_bft_ll(&TSectD, bPosiI);
			double dBFTROut = m_pDgnSectUtil->Get_Size_bft_rr(&TSectD, bPosiI);
			double dBfB = m_pDgnSectUtil->Get_Size_bfb(&TSectD, bPosiI);
			double dBfBL = m_pDgnSectUtil->Get_Size_bfb_l(&TSectD, bPosiI);
			double dBfBR = m_pDgnSectUtil->Get_Size_bfb_r(&TSectD, bPosiI);			
			double dtwL = m_pDgnSectUtil->Get_Size_tw(&TSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			double dtwR = m_pDgnSectUtil->Get_Size_tw(&TSectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
			double dtfT = m_pDgnSectUtil->Get_Size_tft(&TSectD, bPosiI);
			double dtfB = m_pDgnSectUtil->Get_Size_tfb(&TSectD, bPosiI);
			double dDistT = m_pDgnSectUtil->Get_Size_DistTopFlange(&TSectD, bPosiI);
			double dDistB = m_pDgnSectUtil->Get_Size_DistBotFlange(&TSectD, bPosiI);

			pData->pSectionShape = Builder.CreateTubUnsymY(dH, dBfTL, dBfTR, dBfTV, dBfTLOut, dBFTROut, dBfB, dBfBL, dBfBR, dtwL, dtwR, dtfT, dtfB, dDistT, dDistB, enMakingType::Builtup);
		}
		break;
	default: ASSERT(0); break;
	}

	return true;
}

bool CDgnPlateGirder::GetMembLength(dgn::def::CMembLength* pData) const
{
	return true;
}

bool CDgnPlateGirder::GetStressCheck(dgn::def::CStressCheck* pData) const
{
	pData->adFactor[CStressCheck::k1] = dgn::lib::TFactor(false, m_CpgdD.dk1);
	pData->adFactor[CStressCheck::k2] = dgn::lib::TFactor(false, m_CpgdD.dk2);
	pData->adFactor[CStressCheck::k3] = dgn::lib::TFactor(false, m_CpgdD.dk3);
	return true;
}

bool CDgnPlateGirder::GetMomentLoadType(dgn::def::CMomentLoadType* pData) const
{
	//뭐야 이거 어디서 줍줍하는거냐
	bool bLongTermType = false;
	for ( int unCalcLoadType = 0; unCalcLoadType < CMomentLoadType::enType::Number; ++unCalcLoadType )
	{
		pData->aMoment_y[unCalcLoadType].reserve(1);
		pData->aMoment_y[unCalcLoadType].push_back(0.0);
		pData->aTerm[unCalcLoadType] = [&unCalcLoadType] (bool bLongTermType)
		{
			switch ( unCalcLoadType )
			{
			case dgn::def::CMomentLoadType::Permanent:
				return CMomentLoadType::enTerm::Long;
			case dgn::def::CMomentLoadType::Variable:
				return bLongTermType ? CMomentLoadType::enTerm::Long : CMomentLoadType::enTerm::Short;
			case dgn::def::CMomentLoadType::EtcLoad:
				return CMomentLoadType::enTerm::Short;
			default: ASSERT(0); return CMomentLoadType::enTerm::Long;
			}
		} (bLongTermType);
	}

	return true;
}

bool CDgnPlateGirder::GetTorsionDesign(dgn::def::CTorsionDesign* pData) const
{
	pData->bTorsionDesign = false;

	return true;
}

bool CDgnPlateGirder::GetDgnSteelOptions(dgn::def::CDgnSteelOptions* pData) const
{
	pData->Init();
	return true;
}

bool CDgnPlateGirder::GetImperfectionFactor(dgn::def::CImperfectionFactor* pData) const
{
	pData->Init();
	return true;
}

bool CDgnPlateGirder::GetSteelCompositeSection(dgn::def::CSteelCompositeSection* pData) const
{
	CPG_SECT_ELEM SectElem; SectElem.Initialize();
	Get_CpgSectElem(GetIDgnCheckElem(std::this_thread::get_id()), SectElem);

	UINT nPos = GetIDgnCheckPosition(std::this_thread::get_id());
	bool bPosiI = (nPos == 0);

	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectElem.SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectElem.SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectElem.SectD);

	const int nIdxRCS = Macro::Ordinal(enType::Concrete);

	CSectionBuilder Builder;
	pData->apSectionShape[nIdxRCS] = Builder.CreateRectangle(dBc, dtc);

	const int nIdxSTL = Macro::Ordinal(enType::Steel);

	double dBh = 0.0;
	switch ( SectElem.SectD.SectBefore.nStype )
	{
	case D_SECT_TYPE_COMPO_I:
		{
			double dSteelHeight = m_pDgnSectUtil->Get_Size_H(&SectElem.SectD, bPosiI);
			double dSteelWidthFlangeTop = m_pDgnSectUtil->Get_Size_bft(&SectElem.SectD, bPosiI);
			double dSteelWidthFlangeBot = m_pDgnSectUtil->Get_Size_bfb(&SectElem.SectD, bPosiI);
			double dSteelThkWeb = m_pDgnSectUtil->Get_Size_tw(&SectElem.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			double dSteelThkFlangeTop = m_pDgnSectUtil->Get_Size_tft(&SectElem.SectD, bPosiI);
			double dSteelThkFlangeBot = m_pDgnSectUtil->Get_Size_tfb(&SectElem.SectD, bPosiI);

			pData->apSectionShape[nIdxSTL] =
				new CSectionShapeISymY(CAlignedShapeISymY(dSteelHeight,
					dSteelWidthFlangeTop,
					dSteelWidthFlangeBot,
					dSteelThkWeb,
					dSteelThkFlangeTop,
					dSteelThkFlangeBot,
					CAlign2D(), enMakingType::Builtup));

			dBh = dSteelWidthFlangeTop;
		}
		break;
	case D_SECT_TYPE_COMPO_B:
		{
			double dSteelHeight = m_pDgnSectUtil->Get_Size_H(&SectElem.SectD, bPosiI);
			double dSteelWidthFlangeTop = m_pDgnSectUtil->Get_Size_bft(&SectElem.SectD, bPosiI);
			double dSteelWidthFlangeBot = m_pDgnSectUtil->Get_Size_bfb(&SectElem.SectD, bPosiI);
			double dSteelThkWeb = m_pDgnSectUtil->Get_Size_tw(&SectElem.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			double dSteelThkFlangeTop = m_pDgnSectUtil->Get_Size_tft(&SectElem.SectD, bPosiI);
			double dSteelThkFlangeBot = m_pDgnSectUtil->Get_Size_tfb(&SectElem.SectD, bPosiI);
			double dCt = m_pDgnSectUtil->Get_Size_bft_m(&SectElem.SectD, bPosiI);
			double dCb = m_pDgnSectUtil->Get_Size_bfb_m(&SectElem.SectD, bPosiI);
			double dCosA = fabs(cos(m_pDgnSectUtil->Get_Size_WebSlope(&SectElem.SectD, 0, bPosiI)));
			double dtwh = SafeDiv(dSteelThkWeb, dCosA);
			double dtwhHalf = 0.5*dtwh;

			double dBfTopOut = m_pDgnSectUtil->Get_Size_bft_l(&SectElem.SectD, bPosiI) - dtwhHalf;
			double dBfBotOut = m_pDgnSectUtil->Get_Size_bfb_l(&SectElem.SectD, bPosiI) - dtwhHalf;

			pData->apSectionShape[nIdxSTL] =
				new CSectionShapeBoxSymY(CAlignedShapeBoxSymY(dSteelHeight,
					dSteelWidthFlangeTop,
					dSteelWidthFlangeBot,
					dBfTopOut,
					dBfBotOut,
					dSteelThkWeb,
					dSteelThkFlangeTop,
					dSteelThkFlangeBot,
					CAlign2D(), enMakingType::Builtup));

			dBh = dSteelWidthFlangeTop;
		}
		break;
	case D_SECT_TYPE_COMPO_TUB:
		{
			double dH = m_pDgnSectUtil->Get_Size_H(&SectElem.SectD, bPosiI);
			double dtw = m_pDgnSectUtil->Get_Size_tw(&SectElem.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			double dBftop = 0.5*m_pDgnSectUtil->Get_Size_bft(&SectElem.SectD, bPosiI);
			double dTfTop = m_pDgnSectUtil->Get_Size_tft(&SectElem.SectD, bPosiI);
			double dBfTopOut = m_pDgnSectUtil->Get_Size_bft_ll(&SectElem.SectD, bPosiI);
			double dBfbot = m_pDgnSectUtil->Get_Size_bfb(&SectElem.SectD, bPosiI);
			double dTfBot = m_pDgnSectUtil->Get_Size_tfb(&SectElem.SectD, bPosiI);
			double dBfBotOut = m_pDgnSectUtil->Get_Size_bfb_l(&SectElem.SectD, bPosiI);
			double dBfTopCtC = m_pDgnSectUtil->Get_Size_bft_m(&SectElem.SectD, bPosiI);
			double dBfBotCtC = m_pDgnSectUtil->Get_Size_bfb_m(&SectElem.SectD, bPosiI);
			double dBTop = 2.0*dBftop + dBfTopCtC;

			pData->apSectionShape[nIdxSTL] =
				new CSectionShapeTubSymY(CAlignedShapeTubSymY(dH,
					dBTop,
					dBftop,
					dBfbot,
					dBfTopOut,
					dBfBotOut,
					dtw,
					dTfTop,
					dTfBot,
					CAlign2D(), enMakingType::Builtup));

			dBh = dBftop;
		}
		break;
	case D_SECT_TYPE_COMPO_STLG_I:
		{
			/// compose steel section.
			double dH = m_pDgnSectUtil->Get_Size_H(&SectElem.SectD, bPosiI);
			double dBfTL = m_pDgnSectUtil->Get_Size_bft_l(&SectElem.SectD, bPosiI);
			double dBfTR = m_pDgnSectUtil->Get_Size_bft_r(&SectElem.SectD, bPosiI);
			double dBfBL = m_pDgnSectUtil->Get_Size_bfb_l(&SectElem.SectD, bPosiI);
			double dBfBR = m_pDgnSectUtil->Get_Size_bfb_r(&SectElem.SectD, bPosiI);
			double dtw   = m_pDgnSectUtil->Get_Size_tw(&SectElem.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			double dtfT  = m_pDgnSectUtil->Get_Size_tft(&SectElem.SectD, bPosiI);
			double dtfB  = m_pDgnSectUtil->Get_Size_tfb(&SectElem.SectD, bPosiI);
			double dDistT = m_pDgnSectUtil->Get_Size_DistTopFlange(&SectElem.SectD, bPosiI);
			double dDistB = m_pDgnSectUtil->Get_Size_DistBotFlange(&SectElem.SectD, bPosiI);
						
			pData->apSectionShape[nIdxSTL] =
				new CSectionShapeIUnsymY(CAlignedShapeIUnsymY(dH,
					dBfTL,
					dBfTR,
					dBfBL,
					dBfBR,
					dtw,
					dtfT,
					dtfB,
					dDistT, 
					dDistB,
					CAlign2D(), enMakingType::Builtup));

			dBh = dBfTL + dBfTR;
		}
		break;
	case D_SECT_TYPE_COMPO_STLG_B:
		{
			double dH = m_pDgnSectUtil->Get_Size_H(&SectElem.SectD, bPosiI);
			double dBfT = m_pDgnSectUtil->Get_Size_bft(&SectElem.SectD, bPosiI);
			double dBfB = m_pDgnSectUtil->Get_Size_bfb(&SectElem.SectD, bPosiI);
			double dtwL = m_pDgnSectUtil->Get_Size_tw(&SectElem.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			double dtwR = m_pDgnSectUtil->Get_Size_tw(&SectElem.SectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
			double dtfT = m_pDgnSectUtil->Get_Size_tft(&SectElem.SectD, bPosiI);
			double dtfB = m_pDgnSectUtil->Get_Size_tfb(&SectElem.SectD, bPosiI);
			double dBfTL = m_pDgnSectUtil->Get_Size_bft_l(&SectElem.SectD, bPosiI);
			double dBfTR = m_pDgnSectUtil->Get_Size_bft_r(&SectElem.SectD, bPosiI);
			double dBfBL = m_pDgnSectUtil->Get_Size_bfb_l(&SectElem.SectD, bPosiI);
			double dBfBR = m_pDgnSectUtil->Get_Size_bfb_r(&SectElem.SectD, bPosiI);
			double dDistT = m_pDgnSectUtil->Get_Size_DistTopFlange(&SectElem.SectD, bPosiI);
			double dDistB = m_pDgnSectUtil->Get_Size_DistBotFlange(&SectElem.SectD, bPosiI);

			pData->apSectionShape[nIdxSTL] =
				new CSectionShapeBoxUnsymY(CAlignedShapeBoxUnsymY(dH, dBfT, dBfB, dBfTL, dBfTR, dBfBL, dBfBR,
					dtwL, dtwR, dtfT, dtfB, dDistT, dDistB,
					CAlign2D(), enMakingType::Builtup));

			dBh = dBfT;			
		}
		break;
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			double dH = m_pDgnSectUtil->Get_Size_H(&SectElem.SectD, bPosiI);
			double dBfTL = m_pDgnSectUtil->Get_Size_bft_l(&SectElem.SectD, bPosiI);
			double dBfTR = m_pDgnSectUtil->Get_Size_bft_r(&SectElem.SectD, bPosiI);
			double dBfTV = m_pDgnSectUtil->Get_Size_bft_m(&SectElem.SectD, bPosiI);
			double dBfTLOut = m_pDgnSectUtil->Get_Size_bft_ll(&SectElem.SectD, bPosiI);
			double dBFTROut = m_pDgnSectUtil->Get_Size_bft_rr(&SectElem.SectD, bPosiI);
			double dBfB = m_pDgnSectUtil->Get_Size_bfb(&SectElem.SectD, bPosiI);
			double dBfBL = m_pDgnSectUtil->Get_Size_bfb_l(&SectElem.SectD, bPosiI);
			double dBfBR = m_pDgnSectUtil->Get_Size_bfb_r(&SectElem.SectD, bPosiI);
			double dtwL = m_pDgnSectUtil->Get_Size_tw(&SectElem.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			double dtwR = m_pDgnSectUtil->Get_Size_tw(&SectElem.SectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
			double dtfT = m_pDgnSectUtil->Get_Size_tft(&SectElem.SectD, bPosiI);
			double dtfB = m_pDgnSectUtil->Get_Size_tfb(&SectElem.SectD, bPosiI);
			double dDistT = m_pDgnSectUtil->Get_Size_DistTopFlange(&SectElem.SectD, bPosiI);
			double dDistB = m_pDgnSectUtil->Get_Size_DistBotFlange(&SectElem.SectD, bPosiI);

			pData->apSectionShape[nIdxSTL] =
				new CSectionShapeTubUnsymY(CAlignedShapeTubUnsymY(dH, dBfTL, dBfTR, dBfTV, dBfTLOut, dBFTROut, dBfB, dBfBL, dBfBR,
					dtwL, dtwR, dtfT, dtfB, dDistT, dDistB,
					CAlign2D(), enMakingType::Builtup));

			dBh = dBfTL + dBfTR + dBfTV;
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	const int nIdxHunch = Macro::Ordinal(enType::Hunch);
	pData->apSectionShape[nIdxHunch] = Builder.CreateRectangle(dBh, dHh);

	switch (SectElem.SectD.SectBefore.nStype)
	{
	case D_SECT_TYPE_COMPO_I:
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_TUB:
		GetLongitudinalStiffenerType1(&pData->Stiffener);
		break;
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		GetLongitudinalStiffenerType2(&pData->Stiffener);
		break;
	default:
		ASSERT(0);
		break;
	}

	return true;
}

bool CDgnPlateGirder::GetMaterialConcrete(dgn::def::CMaterialConcrete* pData) const
{
	CPG_MATL_ELEM MatlElem; MatlElem.Initialize();
	Get_CpgMatlElem(GetIDgnCheckElem(std::this_thread::get_id()), MatlElem);

	const T_MATD_D& MatdD = MatlElem.MatdD;

	pData->Data.strName = (MatdD.Data2.CodeMatlName);
	if ( MatdD.Data2.CodeName.CompareNoCase(_T("None")) == 0)
	{
		pData->Data.Common.dElastic = MatlElem.dEcm;
	}
	else
	{
		pData->Data.Common.dElastic = MatdD.Data2.Analysis.Elast;
	}
	
	pData->Data.Common.dPoisson = MatdD.Data2.Analysis.Poisson;
	pData->Data.Common.dWeightDensity = MatdD.Data2.Analysis.Density;
	//
	pData->Data.Concrete.dStrength = MatlElem.dfck;
	//
	pData->Data.Rebar.dYield = MatdD.MainRebarData.B_fy;

	return true;
}

bool CDgnPlateGirder::GetMaterialSteelHybrid(dgn::def::CMaterialSteelHybrid* pData) const
{
	CPG_SECT_ELEM SectElem; SectElem.Initialize();
	Get_CpgSectElem(GetIDgnCheckElem(std::this_thread::get_id()), SectElem);

	UINT nPos = GetIDgnCheckPosition(std::this_thread::get_id());
	bool bPosiI = (nPos == 0);

	CPG_MATL_ELEM MatlElem; MatlElem.Initialize();
	Get_CpgMatlElem(GetIDgnCheckElem(std::this_thread::get_id()), MatlElem);

	const T_MATD_D& MatdD = MatlElem.MatdD;

	const double dFu = MatdD.Data1.Design.S_Fu;
	TMaterialSteelSingle Single1(MatdD.Data1.Design.S_Fy1, dFu);
	TMaterialSteelSingle Single2(MatdD.Data1.Design.S_Fy2, dFu);
	TMaterialSteelSingle Single3(MatdD.Data1.Design.S_Fy3, dFu);
	TMaterialSteelSingle Single4(MatdD.Data1.Design.S_Fy4, dFu);
	TMaterialSteelSingle Single5(MatdD.Data1.Design.S_Fy5, dFu);
	TMaterialSteelSingle Single6(MatdD.Data1.Design.S_Fy6, dFu);


	double dSteelThkWeb = m_pDgnSectUtil->Get_Size_tw(&SectElem.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dSteelThkFlangeTop = m_pDgnSectUtil->Get_Size_tft(&SectElem.SectD, bPosiI);
	double dSteelThkFlangeBot = m_pDgnSectUtil->Get_Size_tfb(&SectElem.SectD, bPosiI);


	TMaterialSteel MaterialSteel;
	MaterialSteel.Common.dElastic = MatdD.Data1.Analysis.Elast;
	MaterialSteel.Common.dPoisson = MatdD.Data1.Analysis.Poisson;
	MaterialSteel.Common.dWeightDensity = MatdD.Data1.Analysis.Density;
	MaterialSteel.Steel.nList = 1;
	MaterialSteel.Steel.bUser = (MatdD.Data1.CodeName.CompareNoCase(_T("None")) == 0);
	MaterialSteel.Steel.aProp[0] = SteelThickProp(dSteelThkFlangeTop, Single1);
	pData->aData[0] = MaterialSteel;

	MaterialSteel.Steel.aProp[0] = SteelThickProp(dSteelThkFlangeBot, Single1);
	pData->aData[1] = MaterialSteel;

	MaterialSteel.Steel.aProp[0] = SteelThickProp(dSteelThkWeb, Single1);
	pData->aData[2] = MaterialSteel;



	return true;
}

bool CDgnPlateGirder::GetForce1DArray(dgn::def::CForce1DArray* pData) const
{
	CForce1D Force1D;
	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());
	int nDgnLcom = GetIDgnLcomNo(std::this_thread::get_id());

	//Girder Force
	T_STAG_K nGdLastStagK = 0;
	int nLastStepNo = 0;
	m_pForcCtrl->GetGirderLastStageKey(ElemK, nGdLastStagK, nLastStepNo);

	auto itrCS = m_mStageForce.find(std::make_pair(ElemK, nDgnLcom));
	if ( itrCS == m_mStageForce.end() )
	{
		ASSERT(0); return false;
	}
	CPG_FORC_STAG& ForceStage = (CPG_FORC_STAG&)itrCS->second;
	Force1D.adForce[dgn::def::CForce1D::Fx] = ForceStage.MaxForce.dFxx[unPos];
	Force1D.adForce[dgn::def::CForce1D::Fy] = ForceStage.MaxForce.dFyy[unPos];
	Force1D.adForce[dgn::def::CForce1D::Fz] = ForceStage.MaxForce.dFzz[unPos];
	Force1D.adForce[dgn::def::CForce1D::Mx] = ForceStage.MaxForce.dMux[unPos];
	Force1D.adForce[dgn::def::CForce1D::My] = ForceStage.MaxForce.dMuy[unPos];
	Force1D.adForce[dgn::def::CForce1D::Mz] = ForceStage.MaxForce.dMuz[unPos];

	pData->adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Girder, Force1D));

	//Composite Longterm Force
	int nFortype = 0;
	int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
	// 	if (m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum))
	// 	{
	// 		_DGN_FORC_CRC ForD;
	// 		int nLcomSize = aForceLcom.GetSize();
	// 		if (nLcomSize > 0)
	// 		{
	// 			int nDgnLcomIndex = nDgnLcom - 1;
	// 			if (nDgnLcom > 0 && nDgnLcomIndex < nLcomSize)
	// 			{
	// 				ForD = aForceLcom[nDgnLcomIndex];
	// 			}
	// 		}
	// 
	// 		Force1D.adForce[dgn::def::CForce1D::Fx] = ForD.dFxx[unPos];
	// 		Force1D.adForce[dgn::def::CForce1D::Fy] = ForD.dFyy[unPos];
	// 		Force1D.adForce[dgn::def::CForce1D::Fz] = ForD.dFzz[unPos];
	// 		Force1D.adForce[dgn::def::CForce1D::Mx] = ForD.dMux[unPos];
	// 		Force1D.adForce[dgn::def::CForce1D::My] = ForD.dMuy[unPos];
	// 		Force1D.adForce[dgn::def::CForce1D::Mz] = ForD.dMuz[unPos];
	// 
	// 		pData->adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Composited_LongTerm, Force1D));
	// 	}

	//Composite Shorterm Force
	nFortype = 0;
	auto itr = m_mForce.find(ElemK);
	if ( itr == m_mForce.end() )
	{
		ASSERT(0); return true;
	}
	auto& aForceLcom = itr->second.aForceLcom;
	_DGN_FORC_CRC* pForD = nullptr;
	int nLcomSize = aForceLcom.GetSize();
	if ( nLcomSize > 0 )
	{
		int nDgnLcomIndex = nDgnLcom - 1;
		if ( nDgnLcom > 0 && nDgnLcomIndex < nLcomSize )
		{
			pForD = (_DGN_FORC_CRC*)&(aForceLcom[nDgnLcomIndex]);
		}
	}

	Force1D.adForce[dgn::def::CForce1D::Fx] = pForD->dFxx[unPos] - ForceStage.MaxForce.dFxx[unPos];
	Force1D.adForce[dgn::def::CForce1D::Fy] = pForD->dFyy[unPos] - ForceStage.MaxForce.dFyy[unPos];
	Force1D.adForce[dgn::def::CForce1D::Fz] = pForD->dFzz[unPos] - ForceStage.MaxForce.dFzz[unPos];
	Force1D.adForce[dgn::def::CForce1D::Mx] = pForD->dMux[unPos] - ForceStage.MaxForce.dMux[unPos];
	Force1D.adForce[dgn::def::CForce1D::My] = pForD->dMuy[unPos] - ForceStage.MaxForce.dMuy[unPos];
	Force1D.adForce[dgn::def::CForce1D::Mz] = pForD->dMuz[unPos] - ForceStage.MaxForce.dMuz[unPos];

	pData->adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Composited_ShortTerm, Force1D));
	return true;
}

bool CDgnPlateGirder::GetLoadApplicationType(dgn::def::CLoadApplicationType* pData) const
{
	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());

	CPG_BCDT_ELEM BcdtElem; BcdtElem.Initialize();
	Get_CpgBcdtElem(GetIDgnCheckElem(std::this_thread::get_id()), BcdtElem);

	const CPG_BCDT_POSI& BcdtB = BcdtElem.BcdtPosi[unPos];

	pData->adValue[dgn::def::CLoadApplicationType::PanelPitch] = BcdtB.da;
	pData->adValue[dgn::def::CLoadApplicationType::Ss] = BcdtB.dSs;
	pData->adValue[dgn::def::CLoadApplicationType::C] = BcdtB.dC;

	if ( BcdtB.iLoadAppType == 0 )
	{
		pData->enLoadApp = dgn::def::CLoadApplicationType::Flange_Web;
	}
	else if ( BcdtB.iLoadAppType == 1 )
	{
		pData->enLoadApp = dgn::def::CLoadApplicationType::Flange_Flange;
	}
	else if ( BcdtB.iLoadAppType == 2 )
	{
		pData->enLoadApp = dgn::def::CLoadApplicationType::Flange_unstiffened_end;
	}
	else
		ASSERT(0);

	return true;
}

bool CDgnPlateGirder::GetLateralTorsionalBucklingParam(dgn::def::CLateralTorsionalBuckling* pData) const
{
	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());
	int nDgnLcom = GetIDgnLcomNo(std::this_thread::get_id());

	CPG_LTBD_ELEM LtbdElem; LtbdElem.Initialize();
	Get_CpgLtbdElem(GetIDgnCheckElem(std::this_thread::get_id()), LtbdElem);

	BOOL bReverse = FALSE;
	CArray<T_ELEM_K, T_ELEM_K> arMembElemList;
	int iCountElem = 0;

	ElemPairK showElemK = { 0,0 }, IendElemK = { 0,0 }, JendElemK = { 0,0 };
	GetEndElemOfMember(ElemK, showElemK, IendElemK, JendElemK);

	double dC4 = 0.0;
	if (ElemK.second == EN_EL_BEAM)
	{
		m_pMembCtrl->GetElemListByIncludeElem(ElemK.first, bReverse, showElemK.first, arMembElemList);

		ArrElemPairKey aMembElemPairK;
		CDBLib::ConvertToElemPairKey(EN_EL_BEAM, arMembElemList, aMembElemPairK);
		m_pDataCtrl->Get_C4valueForMomentType(nDgnLcom, showElemK, aMembElemPairK, dC4);
	}
	else if (ElemK.second == EN_EL_VBEAM)
	{
		arMembElemList.Add(IendElemK.first);
		ArrElemPairKey aMembElemPairK;
		CDBLib::ConvertToElemPairKey(EN_EL_VBEAM, arMembElemList, aMembElemPairK);
		m_pDataCtrl->Get_C4valueForMomentType(nDgnLcom, showElemK, aMembElemPairK, dC4);
	}
	else ASSERT(0);

	const CPG_LTBD_POSI& LtbdB = LtbdElem.LtbdPosi[unPos];

	pData->adValue[dgn::def::CLateralTorsionalBuckling::Dspring] = LtbdB.dl;
	pData->adValue[dgn::def::CLateralTorsionalBuckling::Cd] = LtbdB.dCd;
	pData->adValue[dgn::def::CLateralTorsionalBuckling::a] = LtbdB.da;
	pData->adValue[dgn::def::CLateralTorsionalBuckling::Alpha] = static_cast<double>(LtbdB.iAlpha);
	pData->adValue[dgn::def::CLateralTorsionalBuckling::C4] = dC4;

	return true;
}

bool CDgnPlateGirder::GetShearConnector(dgn::def::CShearConnector* pData) const
{
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());

	CPG_SCON_ELEM SconElem; SconElem.Initialize();
	if ( Get_CpgSconElem(GetIDgnCheckElem(std::this_thread::get_id()), SconElem) )
	{
		const CPG_SCON_POSI& SconB = SconElem.SconPosi[unPos];

		TStudSingle StudSingle;
		StudSingle.dDia = SconB.dDia;
		StudSingle.dHeight = SconB.dhsc;
		StudSingle.dLength = SconB.dSt;

		enConnectorType SconType = enConnectorType::Stud;

		TMaterialSteelSingle MSingle(SconB.dfu, SconB.dfu);


		CShearConnectorSingleStud Stud(StudSingle, SconType, MSingle);

		double dNumTran = static_cast<double>(SconB.iNum);

		pData->ShearConnector.SetSingle(&Stud);
		pData->ShearConnector.SetLongitudinalInfo(dNumTran, SconB.dSpace);
	}

	return true;
}

bool CDgnPlateGirder::GetSteelCompositeRebar(dgn::def::CSectionRebarCoord* pData) const
{
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());

	CPG_MATL_ELEM MatlElem; MatlElem.Initialize();
	Get_CpgMatlElem(GetIDgnCheckElem(std::this_thread::get_id()), MatlElem);

	const T_MATD_D& MatdD = MatlElem.MatdD;

	CPG_SECT_ELEM SectElem; SectElem.Initialize();
	Get_CpgSectElem(GetIDgnCheckElem(std::this_thread::get_id()), SectElem);

	UINT nPos = GetIDgnCheckPosition(std::this_thread::get_id());
	bool bPosiI = (nPos == 0);

	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectElem.SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectElem.SectD);
	double dHg = m_pDgnSectUtil->Get_Size_H(&SectElem.SectD, bPosiI);

	CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
	Get_CpgRbarElem(GetIDgnCheckElem(std::this_thread::get_id()), RbarElem);

	const _PSC_RBAR_POSI& RbarPosB = RbarElem.RbarPosi[unPos];


	const auto dfy = MatdD.MainRebarData.B_fy;
	const auto dEs = MatdD.MainRebarData.B_Elast;

	for ( int i = 0; i < RbarPosB.arRbarPosi.GetSize(); ++i )
	{
		const _DGN_RBAR_CRC& RbarB = RbarPosB.arRbarPosi[i];

		const auto dArea = RbarB.dArea;
		const auto dPerimeter = RbarB.dDia;
		const auto dDia = RbarB.dDia;
		std::wstring strDia = static_cast<std::wstring>(CT2W(RbarB.strRbarNa));

		const auto dx = RbarB.dy;
		const auto dy = RbarB.dz + (0.5*dHg + dHh + 0.5*dtc);

		TDiaGeom Geom(dArea, dPerimeter, dDia);
		TDiaMatl Matl(dfy, dEs);
		TDia Dia(Geom, Matl, strDia.c_str());

		CMainRebarSingle Single(DVector2d(dx, dy), Dia);

		pData->vSingle.push_back(Single);
	}

	return true;
}

bool CDgnPlateGirder::GetLongitudinalStiffener(dgn::def::CLongitudinalStiffener* pData) const
{
	CPG_SECT_ELEM SectElem; SectElem.Initialize();
	Get_CpgSectElem(GetIDgnCheckElem(std::this_thread::get_id()), SectElem);

	switch (SectElem.SectD.SectBefore.nStype)
	{
	case D_SECT_TYPE_COMPO_I:
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_TUB:
		return GetLongitudinalStiffenerType1(pData);
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		return GetLongitudinalStiffenerType2(pData);
	default: ASSERT(0); break;
	}

	return true;
}

bool CDgnPlateGirder::GetLongitudinalStiffenerType1(CLongitudinalStiffener* pData) const
{
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());

	CPG_MATL_ELEM MatlElem; MatlElem.Initialize();
	Get_CpgMatlElem(GetIDgnCheckElem(std::this_thread::get_id()), MatlElem);

	const T_MATD_ALL& StlMatdD = MatlElem.MatdD.Data1;
	TMaterialCommonProp MComm(StlMatdD.Analysis.Elast, StlMatdD.Analysis.Poisson, StlMatdD.Analysis.Density);
	TMaterialSteelSingle MSingle(StlMatdD.Design.S_Fy1, StlMatdD.Design.S_Fu);

	TStiffenerMaterial MtStfn(MComm, MSingle);

	CPG_SECT_ELEM SectElem; SectElem.Initialize();
	Get_CpgSectElem(GetIDgnCheckElem(std::this_thread::get_id()), SectElem);

	BOOL bTapered = SectElem.SectD.nStype == D_SECT_TYPE_TAPERED;

	T_SECT_SECTBASE_D& SectBaseD = (unPos == 1 && bTapered) ? SectElem.SectD.CmpTapJ : SectElem.SectD.SectBefore.SectI;
	if ( unPos == 1 && bTapered ) SectBaseD.Stiffener = SectElem.SectD.SectBefore.SectI.Stiffener;

	int nSectPart = SectElem.SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_I ? 2 : 4;

	double dHw = SectBaseD.Size[0];

	auto _L_RevStiffener = [&](CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&>& aStiff) -> void
		{
			CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aModStiff;
			int nStiffSize = aStiff.GetSize();
			double dBottomToStiff = 0.0;
			for (int i = 0; i < nStiffSize; ++i)
			{
				int nIdx = nStiffSize - 1 - i;
				const T_STIFF_PROFILE_D& ProfileB = aStiff[nIdx];
				dBottomToStiff += ProfileB.dSpacing;
			}

			for (int i = 0; i < nStiffSize; ++i)
			{
				int nIdx = nStiffSize - 1 - i;
				T_STIFF_PROFILE_D ProfileB = aStiff[nIdx];
				if (nIdx == nStiffSize - 1)
				{
					ProfileB.dSpacing = dHw - dBottomToStiff;
				}
				else
				{
					ProfileB.dSpacing = aStiff[nIdx + 1].dSpacing;
				}
				aModStiff.Add(ProfileB);
			}
			aStiff.Copy(aModStiff);
		};
	
	const bool bShapeI = (SectElem.SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_I) ? true : false;

	std::vector<IStiffenerSingle*> vpSingle;
	for ( int nPart = 0; nPart < nSectPart; ++nPart ) // 0:Left, 1:Right, 2:Bottom, 3:Top 
	{
		CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiff;

		TStiffenerLocation StfnLoc;
		dgn::def::enPart enSPart;
		switch ( nPart )
		{
		case 0:
			{
				aStiff.Copy(SectBaseD.Stiffener.aStiffLeft);
				if (SectBaseD.Stiffener.nRefPos2 == 1)
				{
					_L_RevStiffener(aStiff);
				}
				enSPart = bShapeI ? enPart::Web : enPart::WebL;

				StfnLoc.Location = bShapeI ? enStiffenerLocation::Left : enStiffenerLocation::Right;
				StfnLoc.RefPoint = SectBaseD.Stiffener.nRefPos2 == 0 ? enStiffenerRefPoint::Top : enStiffenerRefPoint::Bottom;
				StfnLoc.RefPart = enStiffenerRefPart::None;
			}
			break;
		case 1:
			{
			    aStiff.Copy(SectBaseD.Stiffener.aStiffRight);
				if (SectBaseD.Stiffener.nRefPos2 == 1)
				{
					_L_RevStiffener(aStiff);
				}
				enSPart = bShapeI ? enPart::Web : enPart::WebR;
				
				StfnLoc.Location = bShapeI ? enStiffenerLocation::Right : enStiffenerLocation::Left;
				StfnLoc.RefPoint = SectBaseD.Stiffener.nRefPos2 == 0 ? enStiffenerRefPoint::Top : enStiffenerRefPoint::Bottom;
				StfnLoc.RefPart = enStiffenerRefPart::None;
			}
			break;
		case 2:
			{
			    aStiff.Copy(SectBaseD.Stiffener.aStiffBottom);
				enSPart = enPart::Bot;
				StfnLoc.Location = enStiffenerLocation::Bot;
				StfnLoc.RefPoint = enStiffenerRefPoint::Left;
				StfnLoc.RefPart = enStiffenerRefPart::Deck2;
			}
			break;
		case 3:
			{
			    aStiff.Copy(SectBaseD.Stiffener.aStiffTop);
				enSPart = enPart::Top;
				StfnLoc.Location = enStiffenerLocation::Top;
				StfnLoc.RefPoint = enStiffenerRefPoint::Left;
				StfnLoc.RefPart = enStiffenerRefPart::Deck2;
			}
			break;
		default: ASSERT(0); break;
		}

		double dPosZ = 0.0;
		for ( int i = 0; i < aStiff.GetSize(); ++i )
		{
			const T_STIFF_PROFILE_D& ProfileB = aStiff.GetAt(i);
			
			const T_STIFF_SHAPE_D& Shape = SectBaseD.Stiffener.aStiffShape[ProfileB.nShape];

			dPosZ = ProfileB.dSpacing;
			dgn::lib::enTypicalType enType;
			switch ( Shape.nType ) // 0:I, 1:T, 2:U-Rib, 3:L
			{
			case 0:
				{
					enType = enTypicalType::Rectangle;
					CAlignedShapeRectangle ShapeRect(Shape.dSize[1], Shape.dSize[0], CAlign2D());
					auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(0.0, dPosZ));
					ShapeRect.SetAlign(AlignToStfn);

					CStiffenerRectangle LSShape(ShapeRect, MtStfn, enSPart, StfnLoc);
					vpSingle.push_back(LSShape.Clone());
				}
				break;
			case 1:
				{
					enType = enTypicalType::T;
					double dB = Shape.dSize[1];
					double dH = Shape.dSize[0];
					double dtw = Shape.dSize[2];
					double dtf = Shape.dSize[3];
					CAlignedShapeT ShapeT(dB, dH, dtw, dtf, CAlign2D());
					auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(0.0, dPosZ));
					ShapeT.SetAlign(AlignToStfn);

					CStiffenerT LSShape(ShapeT, MtStfn, enSPart, StfnLoc);
					vpSingle.push_back(LSShape.Clone());
				}
				break;
			case 2:
				{
					enType = enTypicalType::URib;
					double dB = Shape.dSize[1];
					double dH = Shape.dSize[0];
					double dBb = Shape.dSize[2];
					double dt = Shape.dSize[3];
					double dR = Shape.dSize[4];
					CAlignedShapeURib ShapeU(dB, dH, dBb, dt, dR, CAlign2D());
					auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(0.0, dPosZ));
					ShapeU.SetAlign(AlignToStfn);

					CStiffenerURib LSShape(ShapeU, MtStfn, enSPart, StfnLoc);
					vpSingle.push_back(LSShape.Clone());
				}
				break;
			default: ASSERT(0); break;
			}

		}
	}

	pData->Set(vpSingle);

	return true;
}

bool CDgnPlateGirder::GetLongitudinalStiffenerType2(dgn::def::CLongitudinalStiffener* pData) const
{
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());

	CPG_MATL_ELEM MatlElem; MatlElem.Initialize();
	Get_CpgMatlElem(GetIDgnCheckElem(std::this_thread::get_id()), MatlElem);

	const T_MATD_ALL& StlMatdD = MatlElem.MatdD.Data1;
	TMaterialCommonProp MComm(StlMatdD.Analysis.Elast, StlMatdD.Analysis.Poisson, StlMatdD.Analysis.Density);
	TMaterialSteelSingle MSingle(StlMatdD.Design.S_Fy1, StlMatdD.Design.S_Fu);

	TStiffenerMaterial MtStfn(MComm, MSingle);

	CPG_SECT_ELEM SectElem; SectElem.Initialize();
	Get_CpgSectElem(GetIDgnCheckElem(std::this_thread::get_id()), SectElem);

	T_SECT_D SectD = SectElem.SectD;
	BOOL bTapered = SectD.nStype == D_SECT_TYPE_TAPERED;

	T_SECT_SECTBASE_D& SectBaseD = (unPos == 1 && bTapered) ? SectD.CmpTapJ : SectD.SectBefore.SectI;
	if (unPos == 1 && bTapered) SectBaseD.Stiffener = SectD.SectBefore.SectI.Stiffener;
	
	double dHw = SectBaseD.Size[0];
		
	std::vector<IStiffenerSingle*> vpSingle;
	int nStiffSize = SectD.SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize();
	if (nStiffSize > 0)
	{
		auto* pLongitudinalStiff = new CLongitudinalStiffener;
		
		for (int j = 0; j < nStiffSize; ++j)
		{
			double dPosZ = 0.0;
			int nStiffDataSize = SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[j].aStiffData.GetSize();
			for (int k = 0; k < nStiffDataSize; ++k)
			{
				for (int l = 0; l < SectD.SectBefore.SectI.SODStiffener.aStiffShape.GetSize(); ++l)
				{
					if (SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[j].aStiffData[k].strShapeName == SectD.SectBefore.SectI.SODStiffener.aStiffShape[l].strName)
					{
						dgn::def::enPart enSPart = [&SectD, j]()
							{
								if (SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I)
								{
									switch (SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[j].nDeckPos)
									{
									case 0:// Top
										return enPart::Top;
									case 1:// WEB
										return enPart::Web;
									case 2:// BOTTOM
										return enPart::Bot;
									default:
										ASSERT(0);
										break;
									}
									return enPart::Web;
								}
								else if (SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B)
								{
									switch (SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[j].nDeckPos)
									{
									case 0:// Top
										return enPart::Top;
									case 1:// WEB
										return enPart::WebL;
									case 2:// WEB
										return enPart::WebR;
									case 3:// BOTTOM
										return enPart::Bot;
									default:
										ASSERT(0);
										break;
									}
									return enPart::Web;
								}
								else if (SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB)
								{
									switch (SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[j].nDeckPos)
									{
									case 0:// Top
										return enPart::Top;
									case 1:// WEB
										return enPart::WebL;
									case 2:// WEB
										return enPart::WebR;
									case 3:// BOTTOM
										return enPart::Bot;
									default:
										ASSERT(0);
										break;
									}
									return enPart::Web;
								}
								else
								{
									ASSERT(0);
									return enPart::Web;
								}

							}();


						dPosZ = SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[j].aStiffData[k].dSpacing;
						dgn::lib::enTypicalType enType;
						TStiffenerLocation Location;
						Location.Location = [&SectD, j, k, enSPart]()
							{
								switch (enSPart)
								{
								case enPart::Top:
									return enStiffenerLocation::Top;
								case enPart::Bot:
									return enStiffenerLocation::Bot;
								case enPart::Web:
								case enPart::WebL:
								case enPart::WebR:
									{
										switch (SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[j].aStiffData[k].nStiffPos)
										{
										case 0:
											return enStiffenerLocation::Left;
										case 1:
											return enStiffenerLocation::Right;
										case 2:
											return enStiffenerLocation::Both;
										default:
											break;
										}
										ASSERT(0);
										return enStiffenerLocation::Left;
									}
									break;
								default:
									break;
								}
								ASSERT(0);
								return enStiffenerLocation::Left;
							}();
						Location.RefPoint = [enSPart, &SectD, j]()
							{
								switch (enSPart)
								{
								case enPart::Top:
									return SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[j].nRefPos == 0 ? enStiffenerRefPoint::Left : enStiffenerRefPoint::Right;
								case enPart::Bot:
									return SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[j].nRefPos == 0 ? enStiffenerRefPoint::Left : enStiffenerRefPoint::Right;
								case enPart::Web:
								case enPart::WebL:
								case enPart::WebR:
									return SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[j].nRefPos == 0 ? enStiffenerRefPoint::Top : enStiffenerRefPoint::Bottom;
								default:
									break;
								}
								ASSERT(0);
								return enStiffenerRefPoint::None;
							}();

						Location.RefPart = [enSPart, &SectD, j]()
							{
								switch (enSPart)
								{
								case enPart::Top:
									return (enStiffenerRefPart)(SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[j].nDeckPart + 1);
								case enPart::Bot:
									return (enStiffenerRefPart)(SectD.SectBefore.SectI.SODStiffener.aSODStiffSub[j].nDeckPart + 1);
								case enPart::Web:
								case enPart::WebL:
								case enPart::WebR:
									return enStiffenerRefPart::None;
								default:
									break;
								}
								return enStiffenerRefPart::None;
							}();

						auto Shape = SectD.SectBefore.SectI.SODStiffener.aStiffShape[l];
						switch (SectD.SectBefore.SectI.SODStiffener.aStiffShape[l].nType)
						{
						case 0: //I
							{
								enType = enTypicalType::Rectangle;
								CAlignedShapeRectangle ShapeRect(Shape.dSize[1], Shape.dSize[0], CAlign2D());
								auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(0.0, dPosZ));
								ShapeRect.SetAlign(AlignToStfn);

								CStiffenerRectangle LSShape(ShapeRect, MtStfn, enSPart, Location);
								vpSingle.push_back(LSShape.Clone());
							}
							break;
						case 1: //T
							{
								enType = enTypicalType::T;
								double dB = Shape.dSize[1];
								double dH = Shape.dSize[0];
								double dtw = Shape.dSize[2];
								double dtf = Shape.dSize[3];
								CAlignedShapeT ShapeT(dB, dH, dtw, dtf, CAlign2D());
								auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(0.0, dPosZ));
								ShapeT.SetAlign(AlignToStfn);

								CStiffenerT LSShape(ShapeT, MtStfn, enSPart, Location);
								vpSingle.push_back(LSShape.Clone());
							}
							break;
						case 2: //U
							{
								enType = enTypicalType::URib;
								double dB = Shape.dSize[1];
								double dH = Shape.dSize[0];
								double dBb = Shape.dSize[2];
								double dt = Shape.dSize[3];
								double dR = Shape.dSize[4];
								CAlignedShapeURib ShapeU(dB, dH, dBb, dt, dR, CAlign2D());
								auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(0.0, dPosZ));
								ShapeU.SetAlign(AlignToStfn);

								CStiffenerURib LSShape(ShapeU, MtStfn, enSPart, Location);
								vpSingle.push_back(LSShape.Clone());
							}
							break;
						default:
							ASSERT(0);
							break;
						}
					}
				}
			}
		}		
	}	

	pData->Set(vpSingle);

	return true;
}

bool CDgnPlateGirder::GetTransverseStiffener(dgn::def::CTransverseStiffener* pData) const
{
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());

	CPG_MATL_ELEM MatlElem; MatlElem.Initialize();
	Get_CpgMatlElem(GetIDgnCheckElem(std::this_thread::get_id()), MatlElem);

	CPG_TRST_ELEM TStfnElem; TStfnElem.Initialize();
	Get_CpgTrstElem(GetIDgnCheckElem(std::this_thread::get_id()), TStfnElem);

	CPG_TSES_ELEM TsesElem; TsesElem.Initialize();
	Get_CpgTsesElem(GetIDgnCheckElem(std::this_thread::get_id()), TsesElem);

	const CPG_TRST_POSI& TStfnPosB = TStfnElem.TrstPosi[unPos];
	const CPG_TSES_POSI& TsesPosB = TsesElem.TsesPosi[unPos];


	if ( TStfnPosB.bUseStif || TsesPosB.bUseStif)
	{
		const T_MATD_D& MatdD = MatlElem.MatdD;

		const double dFu = MatdD.Data1.Design.S_Fu;

		const double& dElastic = MatdD.Data1.Analysis.Elast;
		const double& dPoisson = MatdD.Data1.Analysis.Poisson;
		const double& dWeightDensity = MatdD.Data1.Analysis.Density;

		TMaterialCommonProp MComm(dElastic, dPoisson, dWeightDensity);
		TMaterialSteelSingle MSingle(MatdD.Data1.Design.S_Fy1, dFu);

		TStiffenerMaterial MtStfn(MComm, MSingle);

		CAlignedShapeRectangle ShapeRect(TStfnPosB.dht, TStfnPosB.dt, CAlign2D());
		auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(TStfnPosB.dPitch, 0.0));

		dgn::def::enPart enSPart = enPart::Web;

		CStiffenerRectangle TSShape(ShapeRect, MtStfn, enSPart, TStiffenerLocation());

		TTransverseData TransverseD;
		TransverseD.dPitch = TStfnPosB.dPitch;
		TransverseD.dInternalRigidPitch = TStfnPosB.dDistRstif;
		TransverseD.dNonRigidPitch = TStfnPosB.dDistNRstif;
		TransverseD.bInternalRigid = TStfnPosB.bInterRigidStif;
		TransverseD.bNonRigid = TStfnPosB.bInterNonRigidStif;
		TransverseD.enEndType = enEndSupport::NonRigidEndPost;
		if ( TsesPosB.bUseStif )
		{
			TransverseD.bTStiffenerEndSupport = true;
			switch ( TsesPosB.iEndpost )
			{
			case 0: TransverseD.enEndType = enEndSupport::RigidEndPost; break;
			case 1: TransverseD.enEndType = enEndSupport::NonRigidEndPost; break;
			case 2: TransverseD.enEndType = enEndSupport::NoEndPost; break;
			default: ASSERT(0); break;
			}
		}

		pData->Set(TSShape.Clone(), TransverseD);
	}

	return true;
}

bool CDgnPlateGirder::GetDamageEquivalenceFactor(dgn::def::CDamageEquivalenceFactorDef* pData) const
{
	pData->adValue[CDamageEquivalenceFactorDef::t_LD] = m_CpgdD.dt_Ld;

	return true;
}

bool CDgnPlateGirder::GetDamageEquivalenceFactorMember(dgn::def::CDamageEquivalenceFactorMembDef* pData) const
{
	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());

	CPG_FRDT_ELEM FrdtElem; FrdtElem.Initialize();
	Get_CpgFrdtElem(GetIDgnCheckElem(std::this_thread::get_id()), FrdtElem);

	const CPG_FRDT_POSI& FrdtPosB = FrdtElem.FrdtPosi[unPos];

	pData->adValue[CDamageEquivalenceFactorMembDef::LambdaV2] = FrdtPosB.dLamda_v2;
	pData->adValue[CDamageEquivalenceFactorMembDef::LambdaV4] = FrdtPosB.dLamda_v4;

	return true;
}

bool CDgnPlateGirder::GetStressLimitationFactor(dgn::def::CStressLimitationFactorDef* pData) const
{
	pData->adValue[CStressLimitationFactorDef::ks] = m_CpgdD.dks;

	return true;
}

bool CDgnPlateGirder::GetStressSteelComposite(dgn::def::CStressSteelCompositeDef* pData) const
{
	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());
	UINT nLcomIdx = GetIDgnLcomNo(std::this_thread::get_id()) - 1;

	auto itrP1 = m_mStress.find(ElemK);
	if ( itrP1 == m_mStress.end() )
	{
		ASSERT(0); return false;
	}

	auto itrP2 = m_mStressP2.find(ElemK);
	if ( itrP2 == m_mStressP2.end() )
	{
		ASSERT(0); return false;
	}

	T_RPSC_D RpscD; RpscD.Initialize();
	if (m_pDoc->m_pAttrCtrl2->GetRpscByConsiderRbarEPairK(ElemK, TRUE, RpscD))
	{
		pData->bCrackedSlab = RpscD.bCracked;
	}	

	pData->adGirder[CStressSteelCompositeDef::LT] = itrP1->second.aStressLcom[nLcomIdx].dStl[unPos];
	pData->adGirder[CStressSteelCompositeDef::RT] = itrP1->second.aStressLcom[nLcomIdx].dStr[unPos];
	pData->adGirder[CStressSteelCompositeDef::RB] = itrP1->second.aStressLcom[nLcomIdx].dSbr[unPos];
	pData->adGirder[CStressSteelCompositeDef::LB] = itrP1->second.aStressLcom[nLcomIdx].dSbl[unPos];
	//
	pData->adSlab[CStressSteelCompositeDef::LT] = itrP2->second.aStressLcom[nLcomIdx].dStl[unPos];
	pData->adSlab[CStressSteelCompositeDef::RT] = itrP2->second.aStressLcom[nLcomIdx].dStr[unPos];
	pData->adSlab[CStressSteelCompositeDef::RB] = itrP2->second.aStressLcom[nLcomIdx].dSbr[unPos];
	pData->adSlab[CStressSteelCompositeDef::LB] = itrP2->second.aStressLcom[nLcomIdx].dSbl[unPos];

	return true;
}

bool CDgnPlateGirder::GetConstrainStressComposite(dgn::def::CConstrainStressComposite* pData) const
{
	auto ElemK = GetIDgnCheckElem(std::this_thread::get_id());
	UINT unPos = GetIDgnCheckPosition(std::this_thread::get_id());
	UINT nLcomIdx = GetIDgnLcomNo(std::this_thread::get_id()) - 1;

	if ( CConstrainStressComposite::Number != 19 )
	{
		ASSERT(0); return false;
	}

	// Load case 분리할 필요 없으니 더해서 쓰자. 필요 하면 분리
	if ( m_mScfpCr.size() > 0 )
	{
		auto itrP1 = m_mScfpCr.find(ElemK);
		if ( itrP1 != m_mScfpCr.end() )
		{
			const _DGN_SCFP_D& ForceP1 = itrP1->second.aScfpStress[nLcomIdx].ForceP1;
			const _DGN_SCFP_D& ForceP2 = itrP1->second.aScfpStress[nLcomIdx].ForceP2;
			for ( int i = 0; i < CConstrainStressComposite::Number; ++i )
			{
				pData->adGirder[i] += ForceP1.dbLStress[unPos][i];
				pData->adSlab[i]   += ForceP2.dbLStress[unPos][i];
			}			
		}
	}

	if ( m_mScfpSh.size() > 0 )
	{
		auto itrP1 = m_mScfpSh.find(ElemK);
		if ( itrP1 != m_mScfpSh.end() )
		{
			const _DGN_SCFP_D& ForceP1 = itrP1->second.aScfpStress[nLcomIdx].ForceP1;
			const _DGN_SCFP_D& ForceP2 = itrP1->second.aScfpStress[nLcomIdx].ForceP2;
			for ( int i = 0; i < CConstrainStressComposite::Number; ++i )
			{
				pData->adGirder[i] += ForceP1.dbLStress[unPos][i];
				pData->adSlab[i]   += ForceP2.dbLStress[unPos][i];
			}
		}
	}

	if ( m_mScfpTe.size() > 0 )
	{
		auto itrP1 = m_mScfpTe.find(ElemK);
		if ( itrP1 != m_mScfpTe.end() )
		{
			const _DGN_SCFP_D& ForceP1 = itrP1->second.aScfpStress[nLcomIdx].ForceP1;
			const _DGN_SCFP_D& ForceP2 = itrP1->second.aScfpStress[nLcomIdx].ForceP2;
			for ( int i = 0; i < CConstrainStressComposite::Number; ++i )
			{
				pData->adGirder[i] += ForceP1.dbLStress[unPos][i];
				pData->adSlab[i]   += ForceP2.dbLStress[unPos][i];
			}
		}
	}

	return true;
}

bool CDgnPlateGirder::GetMemoizationKey(dgn::def::CMemoizationKey* pData) const
{
	auto EPairK = GetIDgnCheckElem(std::this_thread::get_id());
	pData->vMemoKey.push_back(std::make_pair(DataType::Elem, (UINT)EPairK.first));
	pData->vMemoKey.push_back(std::make_pair(DataType::Node, (UINT)GetIDgnCheckPosition(std::this_thread::get_id())));
	return true;
}

bool CDgnPlateGirder::UpdateShearData(const int& nDir, std::wstring& strLcb, std::wstring& strPosi, dgn::def::CDgnInput* pInp)
{
	return true;
}

BOOL CDgnPlateGirder::Calc_Vbf_Rd(T_PLATE_GIRDER_CALC_D& param, T_SECT_D& SectD, T_PRVS_BASE& PrvsBase, double& dVbf_Rd, T_VERTICAL_SHAER_RES_D& ResD)
{
	// 1. Initailize
	dVbf_Rd = 0.0;
	double dVbf_Rd_LR[2] = { 0.0, 0.0 };

	BOOL bPosiI = param.bPosiI;
	// 2. Get Input Data
	T_PGBR_BASE PgbrBase = param.PgbrBase;
	bool bBoxSect = m_pDgnSectUtil->Is_StlBoxSect(&SectD);
	double dhw_in = m_pDgnSectUtil->Get_Size_Inclind_hw(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTF1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dTF2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dfy = param.MatlElem.MatdD.Data1.Design.S_Fy1;
	double dfy1 = param.MatlElem.MatdD.Data1.Design.S_Fy2;
	double dfyt = Get_fy4Thick(dfy, dfy1, dTF1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTF2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dgam_M1 = m_dGamma_M1;
	double dgam_M2 = m_dGamma_M0;
	double dEpsi = Get_Epsilon(dfyw);

	// EN 1993-1-5 7.1 (28page)
	// 7.1 Interaction between shear force, bending moment and axial force
	double dMf_Rd = PgbrBase.dMf_Rd;
	double dMpl_Rd = PgbrBase.dMpl_Rd_s; // Bending의 Mpl_Rd와 다를 수 있다.
	double dM_Ed = param.PgbrDetail.dM_Ed;
	double dN_Ed = PrvsBase.dFx;
	double dV_Ed = PrvsBase.dV_Ed;
	double dFactor_fr = 0.0;

	// In case of compression  
	double dAf_top = dB1 * dTF1;
	double dAf_bot = dB2 * dTF2;
	if ( dN_Ed < 0.0 ) { dFactor_fr = Get_ReductonFactorOfFlange4N_Ed(dgam_M2, dfyt, dfyb, dN_Ed, dAf_top, dAf_bot); }
	double dMf_Rd_factor = dFactor_fr * dMf_Rd;

	//
	int iClassSect = param.ClassBase.iClassSect;
	double dVbw_Rd = ResD.dVbw_Rd;
	double dBendingRat = fabs(dMpl_Rd) < m_dZero ? 0.0 : dMf_Rd_factor / dMpl_Rd;
	double dBar_Eta1 = fabs(dMpl_Rd) < m_dZero ? 0.0 : fabs(dM_Ed) / dMpl_Rd;
	double dBar_Eta1_Lim = dBendingRat;
	double dBar_Eta3 = fabs(dVbw_Rd) < m_dZero ? 0.0 : fabs(dV_Ed) / dVbw_Rd;

	//
	double dCombinedEffe = dBar_Eta1 + (1.0 - dBendingRat)*pow((2.0*dBar_Eta3 - 1.0), 2);

	BOOL bEta3Exceed05 = FALSE, bCombinedEffe = FALSE, bEta1Ratio = FALSE;
	if ( dBar_Eta3 > 0.5 ) { bEta3Exceed05 = TRUE; }

	if ( dBar_Eta1 > dBendingRat - m_dZero )
	{
		bEta1Ratio = TRUE;
		if ( dCombinedEffe < 1.0 + m_dZero ) { bCombinedEffe = TRUE; }
	}
	else
	{
		bCombinedEffe = TRUE;
	}

	// Contribution from flange, EN 1993-1-5 , 25page
	double dbf[2] = { 0.0, 0.0 };
	double dtf = 0.0;
	double dfyf = 0.0;
	bool bTopFlange = true;
	if ( dAf_top < dAf_bot + m_dZero )
	{
		bTopFlange = true;
		dtf = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
		dfyf = dfyt; // Modify by GAY. PMS:4216. ('12.05.17). Design strength -> Nominal strength.
	}
	else
	{
		bTopFlange = false;
		dtf = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
		dfyf = dfyb; // Modify by GAY. PMS:4216. ('12.05.17). Design strength -> Nominal strength.
	}

	Get_bf4c_DB(bPosiI, SectD, bTopFlange, 0, dEpsi, dbf[0]);

	double da = ResD.ShearResD[0].dDist4Shr;  // _T("a") is the distance between transverse stiffeners 
	double dc_LR[2] = { 0.0, 0.0 };

	dc_LR[0] = (dTw* dhw_in *dfyw < m_dZero) ? 0.0 : da * (0.25 + 1.6*dbf[0] * pow(dtf, 2)*dfyf / (dTw*pow(dhw_in, 2)*dfyw));

	if ( fabs(dMf_Rd_factor) > m_dZero && dM_Ed < dMf_Rd_factor )
	{
		dVbf_Rd_LR[0] = fabs(dc_LR[0] * dgam_M1) < m_dZero ? 0.0 : dbf[0] * pow(dtf, 2)*dfyf / (dc_LR[0] * dgam_M1)*(1.0 - pow(dM_Ed / dMf_Rd_factor, 2));
	}
	else // Do not consider shear buckling resistance of flange's contribution
	{
		dVbf_Rd_LR[0] = 0.0;
	}

	if ( bBoxSect )
	{
		Get_bf4c_DB(bPosiI, SectD, bTopFlange, 1, dEpsi, dbf[1]);

		dc_LR[1] = (dTw* dhw_in *dfyw < m_dZero) ? 0.0 : da * (0.25 + 1.6*dbf[1] * pow(dtf, 2)*dfyf / (dTw*pow(dhw_in, 2)*dfyw));

		if ( fabs(dMf_Rd_factor) > m_dZero && dM_Ed < dMf_Rd_factor )
		{
			dVbf_Rd_LR[1] = fabs(dc_LR[1] * dgam_M1) < m_dZero ? 0.0 : dbf[1] * pow(dtf, 2)*dfyf / (dc_LR[1] * dgam_M1)*(1.0 - pow(dM_Ed / dMf_Rd_factor, 2));
		}
		else // Do not consider shear buckling resistance of flange's contribution
		{
			dVbf_Rd_LR[1] = 0.0;
		}
	}

	dVbf_Rd = dVbf_Rd_LR[0] + dVbf_Rd_LR[1];

	// Save Result
	ResD.dVbf_Rd = dVbf_Rd;

	ResD.ShearResD[0].bEta3Exceed05 = bEta3Exceed05;
	ResD.ShearResD[0].bCombinedEffe = bCombinedEffe;
	ResD.ShearResD[0].dVbf_Rd = dVbf_Rd_LR[0];
	ResD.ShearResD[0].dM_Ed = dM_Ed;
	ResD.ShearResD[0].dMf_Rd = dMf_Rd;
	ResD.ShearResD[0].dFactor_fr = dFactor_fr;
	ResD.ShearResD[0].dMf_Rd_factor = dMf_Rd_factor;
	ResD.ShearResD[0].dMpl_Rd = dMpl_Rd;
	ResD.ShearResD[0].dBar_Eta1 = dBar_Eta1;
	ResD.ShearResD[0].dBar_Eta1_Lim = dBar_Eta1_Lim;
	ResD.ShearResD[0].dBar_Eta3 = dBar_Eta3;
	ResD.ShearResD[0].dCombinedEffe = dCombinedEffe;
	ResD.ShearResD[0].dbf = dbf[0];
	ResD.ShearResD[0].dtf = dtf;
	ResD.ShearResD[0].dc = dc_LR[0];

	if ( bBoxSect )
	{
		ResD.ShearResD[1].bEta3Exceed05 = bEta3Exceed05;
		ResD.ShearResD[1].bCombinedEffe = bCombinedEffe;
		ResD.ShearResD[1].dVbf_Rd = dVbf_Rd_LR[1];
		ResD.ShearResD[1].dM_Ed = dM_Ed;
		ResD.ShearResD[1].dMf_Rd = dMf_Rd;
		ResD.ShearResD[1].dFactor_fr = dFactor_fr;
		ResD.ShearResD[1].dMf_Rd_factor = dMf_Rd_factor;
		ResD.ShearResD[1].dMpl_Rd = dMpl_Rd;
		ResD.ShearResD[1].dBar_Eta1 = dBar_Eta1;
		ResD.ShearResD[1].dBar_Eta1_Lim = dBar_Eta1_Lim;
		ResD.ShearResD[1].dBar_Eta3 = dBar_Eta3;
		ResD.ShearResD[1].dCombinedEffe = dCombinedEffe;
		ResD.ShearResD[1].dbf = dbf[1];
		ResD.ShearResD[1].dtf = dtf;
		ResD.ShearResD[1].dc = dc_LR[1];
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Calc_Vb_Rd(T_PLATE_GIRDER_CALC_D& param, T_SECT_D& SectD, T_PRVS_BASE& PrvsBase, double& dVb_Rd, T_VERTICAL_SHAER_RES_D& ResD)
{
	// 1. Initailize
	dVb_Rd = 0.0;

	// 2. Get Basic Input Data
	int iMatClass = param.MatlElem.iMatClass;
	double dgam_M1 = m_dGamma_M1;
	double dhw = m_pDgnSectUtil->Get_Size_hw(&SectD, param.bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, param.bPosiI);
	double dTw_t = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, param.bPosiI);
	double dfy = param.MatlElem.MatdD.Data1.Design.S_Fy1;
	double dfy1 = param.MatlElem.MatdD.Data1.Design.S_Fy2;
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dEta = Get_Eta4SteelGrade(iMatClass, dfy);

	// 2. Calc. Vbw_Rd (Contribution of web)
	double dVbw_Rd = 0.0;
	if ( !Calc_Vbw_Rd(param, dVbw_Rd, ResD) ) { ASSERT(0); }

	// 3. Calc. Vbf_Rd (Contribution of flange)
	double dVbf_Rd = 0.0;
	if ( !Calc_Vbf_Rd(param, SectD, PrvsBase, dVbf_Rd, ResD) ) { ASSERT(0); }

	// 4. Calc. Vb_Rd
	double dVb_Rd_lim = 0.0, dVb_Rd_org = 0.0;
	dVb_Rd = dVb_Rd_org = dVbw_Rd + dVbf_Rd;

	// 5. Check Limit Shear Buckling Resistance Value
	double dVb_Rd_Lim = fabs(dgam_M1) < m_dZero ? 0.0 : dEta * dfyw*dhw*dTw_t / (sqrt(3.0)*dgam_M1);

	if ( dVb_Rd > dVb_Rd_Lim ) { dVb_Rd = dVb_Rd_Lim; }

	// 6. Save Result Data
	ResD.ShearResD[0].dVb_Rd_org = dVb_Rd_org;
	ResD.ShearResD[0].dVb_Rd_Lim = dVb_Rd_Lim;
	ResD.ShearResD[1].dVb_Rd_org = dVb_Rd_org;
	ResD.ShearResD[1].dVb_Rd_Lim = dVb_Rd_Lim;

	return TRUE;
}
double CDgnPlateGirder::Get_Height4Gen(const T_PLATE_GIRDER_CALC_D& param)
{
	double dHt = param.bPosiI ? param.dHt[0] : param.dHt[1];
	return dHt;
}
double CDgnPlateGirder::Get_ZcentroidofSlab(const T_PLATE_GIRDER_CALC_D& param)
{
	double dZbar_slab = param.bPosiI ? param.dZbar_sb[0] : param.dZbar_sb[1];
	return dZbar_slab;
}

double CDgnPlateGirder::Get_CzmofSlab(const T_PLATE_GIRDER_CALC_D& param)
{
	double dCzm_slab = param.bPosiI ? param.dCzm_sb[0] : param.dCzm_sb[1];
	return dCzm_slab;
}
double CDgnPlateGirder::Get_CzpofSlab(const T_PLATE_GIRDER_CALC_D& param)
{
	double dCzp_slab = param.bPosiI ? param.dCzp_sb[0] : param.dCzp_sb[1];
	return dCzp_slab;
}
double CDgnPlateGirder::Get_AreaofSlab(const T_PLATE_GIRDER_CALC_D& param)
{
	double dArea_slab = param.bPosiI ? param.dArea_sb[0] : param.dArea_sb[1];
	return dArea_slab;
}
double CDgnPlateGirder::Get_BcofSlab(const T_PLATE_GIRDER_CALC_D& param)
{
	double dBc_slab = param.bPosiI ? param.dBc_sb[0] : param.dBc_sb[1];
	return dBc_slab;
}
BOOL CDgnPlateGirder::Get_Nc_fofSlab(T_PLATE_GIRDER_CALC_D& param, double dfcd, double dXpl, double dHt, double& dNc_f)
{
	BOOL bPoisI = param.bPosiI;
	double dCzm_s = Get_CzmofSlab(param);
	double dCzp_s = Get_CzpofSlab(param);
	double dHs = dCzm_s + dCzp_s;

	double dArea_s = 0.0;
	int iPartSize = param.SectBaseD.aGeneralPart.GetSize();
	double dzpos = dHt - dXpl;
	for ( int i = 0; i < iPartSize; i++ )
	{
		T_GSEC_PART PartD = param.SectBaseD.aGeneralPart[i];
		if ( PartD.bPlane )
		{
			if ( dXpl < dHs )
			{
				double aArea[4];
				memset(aArea, 0, sizeof(aArea));
				CArray<T_GSEC_LINE, T_GSEC_LINE&> aTopLine, aBotLine, aHCutLine;
				CArray<double, double&> aHCutTopThick;
				T_GSEC_POLYGON PolyD;
				double aZcen[4]; // 자르는 직선기준으로 잘려진 단면의 도심 
				memset(aZcen, 0, sizeof(aZcen));
				T_SECT_SECTBASE_D CalcSectBaseD; CalcSectBaseD.Initialize();
				Get_ConvertSectBase4GenLineData(param.SectBaseD, param.GLineDgn, CalcSectBaseD);
				CSectUtil::Get_SectPropertyOfCuttingArea(dzpos, CalcSectBaseD, aArea, aZcen, PolyD, aTopLine, aBotLine, aHCutLine, aHCutTopThick);
				dArea_s = aArea[0];
			}
			else
			{
				dArea_s = PartD.Stiffness.Area;
			}
			break;
		}
	}
	dNc_f = dfcd * dArea_s;

	return TRUE;
}

// LineBaseD 정보는 0번째 Index가 Sigma1이 데이터 이어야 한다. 
// 이 함수 들어오기전에 0번째 Index가 Sigma1이 아니면 LineBaseD를 Convert한 후 계산.
BOOL CDgnPlateGirder::Get_EffectivePanelInformation(T_PLATE_GIRDER_CALC_D& param, int iClass, BOOL bInternal, int iStressType, double* aForce, double dCzm_gap, double dStiff[3], const T_GLINE_BASE& LineBaseD,
	const T_GLINE_DGN_B& LineDgnB, T_GENL_DATA& EffLineD)
{
	int i = 0;
	int iLineSize = LineBaseD.aLineIx.GetSize();

	double dFx = aForce[0];
	double dMy = aForce[4];
	double dMz = aForce[5];
	//
	double dArea = dStiff[0];
	double dIy = dStiff[1];
	double dIz = dStiff[2];

	int iEffPolyVtxSize = EffLineD.PolyD.aVertex.GetSize();
	int iEffLineSize = EffLineD.aLine.GetSize();
	int iNextLineIdx = iEffLineSize;
	int iNextVtxIdx = iEffPolyVtxSize;

	CArray<int, int&> aEffLineIx;

	T_GSEC_POLYGON GsecPolyD = param.GLineData.PolyD;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
	aLine.Copy(param.GLineData.aLine);

	double dbe1 = LineDgnB.SubPanelD.db_sup_eff;
	double dbe2 = LineDgnB.SubPanelD.db_inf_eff;

	double db = LineDgnB.db;
	double dbc = LineDgnB.SubPanelD.dbc;
	double dbt = LineDgnB.SubPanelD.dbt;
	int SubStrType = LineDgnB.SubPanelD.nType;
	double dbe2StartLen = 0.0;
	if ( SubStrType == 2 ) // Double curvature.
	{
		dbe2StartLen = LineDgnB.SubPanelD.db_sup - dbe2;
	}
	else
	{
		dbe2StartLen = db - dbe2;
	}

	double dPreLen = 0.0, dCurLen = 0.0;

	double dxe = 0., dye = 0.;
	double dxs = 0., dys = 0.; // Start점 
	int v1 = 0, v2 = 0;

	BOOL bCheckLine = FALSE;
	T_GSEC_VERTEX VtxDiv;

	BOOL bIs1_Sig1 = LineDgnB.bIs1_sig1;
	int vs = bIs1_Sig1 ? LineBaseD.ve1 : LineBaseD.ve2;
	for ( i = 0; i < iLineSize; i++ )
	{
		int iLIx = bInternal && !bIs1_Sig1 ? (iLineSize - 1) - i : i;
		int iLineIx = LineBaseD.aLineIx[iLIx];
		T_GSEC_LINE LineD = aLine[iLineIx];
		int v1 = LineD.v1;
		int v2 = LineD.v2;
		//////////////////////////////////////////////////////////////////////////
		int vc1 = 0, vc2 = 0;
		Get_ConvertVertexId(vs, v1, v2, vc1, vc2);
		vs = vc2;
		LineD.v1 = vc1, LineD.v2 = vc2;
		//////////////////////////////////////////////////////////////////////////    
		T_GSEC_VERTEX vtx1 = GsecPolyD.aVertex[vc1];
		T_GSEC_VERTEX vtx2 = GsecPolyD.aVertex[vc2];
		double dx1 = vtx1.dX;
		double dy1 = vtx1.dY;
		double dx2 = vtx2.dX;
		double dy2 = vtx2.dY;
		double dy1_s = vtx1.dY + dCzm_gap;
		double dy2_s = vtx2.dY + dCzm_gap;
		double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
		double dt = LineD.dThik;
		double dStr1 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dx1, dy1_s);
		double dStr2 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dx2, dy2_s);
		BOOL bCompStr = Is_CompStress(dStr1, dStr2);
		BOOL bDbCurve = Is_DoubleCurvature(dStr1, dStr2);
		if ( i == 0 ) { dxs = dx1, dys = dy1; }

		T_GSEC_VERTEX VtxDiv;
		dCurLen += dbi;
		if ( bInternal )
		{
			if ( LineDgnB.bCompStr && iClass == 4 )
			{
				// be1에 대해 
				if ( dCurLen < dbe1 || fabs(dCurLen - dbe1) < m_dZero ) // 나누어지는 Line을 찾기 위해.
				{
					aEffLineIx.Add(iLineIx); // 기존 데이터로 가능.           
				}
				else if ( dCurLen > dbe1 )
				{
					if ( dPreLen < dbe1 )
					{
						double dlen = dbe1 - dPreLen;
						Get_PointCoor4beff(dlen, dx1, dy1, dx2, dy2, dxe, dye);
						aEffLineIx.Add(iNextLineIdx); iNextLineIdx++;
						//
						VtxDiv.Set(dxe, dye);
						EffLineD.PolyD.aVertex.Add(VtxDiv);
						T_GSEC_LINE ELine = LineD;
						//ELine.v1 = v1;
						ELine.v2 = iNextVtxIdx++;
						EffLineD.aLine.Add(ELine);
					}
				}
				// be2에 대해 
				if ( dbe2StartLen < dCurLen )
				{
					if ( dPreLen < dbe2StartLen )
					{
						double dlen = dbe2StartLen - dPreLen;
						Get_PointCoor4beff(dlen, dx1, dy1, dx2, dy2, dxe, dye);
						aEffLineIx.Add(iNextLineIdx); iNextLineIdx++;
						VtxDiv.Set(dxe, dye);
						EffLineD.PolyD.aVertex.Add(VtxDiv);
						T_GSEC_LINE ELine = LineD;
						ELine.v1 = iNextVtxIdx++;
						//ELine.v2 = v2;
						EffLineD.aLine.Add(ELine);
					}
					else
					{
						aEffLineIx.Add(iLineIx);
					}
				}
			}
			else // 인장.
			{
				aEffLineIx.Add(iLineIx);  // 기존 데이터로 가능.      
			}
		}
		else // Outstand
		{
			if ( iClass == 4 )
			{
				// !!!한쪽이 지지된 곳이 LineBaseD.aLineIx의 0번째 Index      
				if ( iStressType == 0 || iStressType == 1 ) // Outstand_Stress_Type3 와 같음.
				{
					// be1에 대해 
					if ( dCurLen < dbe1 || fabs(dCurLen - dbe1) < m_dZero ) // 나누어지는 Line을 찾기 위해.
					{
						aEffLineIx.Add(iLineIx); // 기존 데이터로 가능.           
					}
					else if ( dCurLen > dbe1 )
					{
						if ( dPreLen < dbe1 )
						{
							double dlen = dbe1 - dPreLen;
							Get_PointCoor4beff(dlen, dx1, dy1, dx2, dy2, dxe, dye);
							aEffLineIx.Add(iNextLineIdx); iNextLineIdx++;
							//
							VtxDiv.Set(dxe, dye);
							EffLineD.PolyD.aVertex.Add(VtxDiv);
							T_GSEC_LINE ELine = LineD;
							//ELine.v1 = v1;
							ELine.v2 = iNextVtxIdx++;
							EffLineD.aLine.Add(ELine);
						}
					}
				}
				else if ( iStressType == 2 ) //Outstand_Stress_Type4 와 같음. 
				{
					// beff에 대해 
					if ( dCurLen < dbe1 || fabs(dCurLen - dbe1) < m_dZero ) // 나누어지는 Line을 찾기 위해.
					{
						aEffLineIx.Add(iLineIx); // 기존 데이터로 가능.           
					}
					else if ( dCurLen > dbe1 )
					{
						if ( dPreLen < dbe1 )
						{
							double dlen = dbe1 - dPreLen;
							Get_PointCoor4beff(dlen, dx1, dy1, dx2, dy2, dxe, dye);
							aEffLineIx.Add(iNextLineIdx); iNextLineIdx++;
							//
							VtxDiv.Set(dxe, dye);
							EffLineD.PolyD.aVertex.Add(VtxDiv);
							T_GSEC_LINE ELine = LineD;
							//ELine.v1 = v1;
							ELine.v2 = iNextVtxIdx++;
							EffLineD.aLine.Add(ELine);
						}
					}
					//
					if ( dCurLen > dbc )
					{
						if ( dPreLen < dbc )
						{
							double dlen = dbc - dPreLen;
							Get_PointCoor4beff(dlen, dx1, dy1, dx2, dy2, dxe, dye);
							aEffLineIx.Add(iNextLineIdx); iNextLineIdx++;
							VtxDiv.Set(dxe, dye);
							EffLineD.PolyD.aVertex.Add(VtxDiv);
							T_GSEC_LINE ELine = LineD;
							ELine.v1 = iNextVtxIdx++;
							//ELine.v2 = v2;
							EffLineD.aLine.Add(ELine);
						}
						else
						{
							aEffLineIx.Add(iLineIx);
						}
					}
				}
				else if ( iStressType == 3 ) //Outstand_Stress_Type2 와 같음. 
				{
					double dEffLen = dbt + dbe1;
					if ( dCurLen < dEffLen || fabs(dCurLen - dEffLen) < m_dZero )
					{
						aEffLineIx.Add(iLineIx);  // 기존 데이터로 가능.
					}
					else if ( dCurLen > dEffLen )
					{
						if ( dPreLen < dEffLen )
						{
							double dlen = dEffLen - dPreLen;
							Get_PointCoor4beff(dlen, dx1, dy1, dx2, dy2, dxe, dye);
							aEffLineIx.Add(iNextLineIdx); iNextLineIdx++;
							VtxDiv.Set(dxe, dye);
							EffLineD.PolyD.aVertex.Add(VtxDiv);
							T_GSEC_LINE ELine = LineD;
							//ELine.v1 = v1;
							ELine.v2 = iNextVtxIdx++;
							EffLineD.aLine.Add(ELine);
						}
					}
				}
				else // 인장.
				{
					aEffLineIx.Add(iLineIx);  // 기존 데이터로 가능.  
				}
			}
			else
				aEffLineIx.Add(iLineIx);  // 기존 데이터로 가능.  
		}
		dPreLen = dCurLen;
	}

	if ( aEffLineIx.GetSize() > 0 ) param.aEffLineIx.Append(aEffLineIx);
	return TRUE;
}
BOOL CDgnPlateGirder::Get_EffectiveSectInfo4PlateElements(T_PLATE_GIRDER_CALC_D& param, int iClass, double* aForce, double dCzm_gap, double dStiff[3], T_GLINE_DST& GLineDST, T_DGNLINE_LBD& LineLBD, T_GENL_DATA& EffGLineD)
{
	int iLineShape = GLineDST.nType; // 0=Internal, 1=Outstand
	BOOL bInternal = iLineShape == 0 ? TRUE : FALSE;

	int i = 0, j = 0;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
	int iPanelSize = LineLBD.aDgnBaseD.GetSize();
	for ( i = 0; i < iPanelSize; i++ )
	{
		T_GLINE_BASE LineBaseD = GLineDST.aLineDgn[i];
		T_GLINE_DGN_B LineDgnB = LineLBD.aDgnBaseD[i];
		int iStressType = LineDgnB.iStressType;
		Get_EffectivePanelInformation(param, iClass, bInternal, iStressType, aForce, dCzm_gap, dStiff, LineBaseD, LineDgnB, EffGLineD);
	}
	// Stiffener
	int nStfnSize = GLineDST.aStiffener.GetSize();
	int nLBDStfnSize = LineLBD.aDgnStfnD.GetSize();
	if ( nStfnSize == nLBDStfnSize )
	{
		for ( i = 0; i < nStfnSize; i++ )
		{
			T_GLINE_DGN_STFN LineStfnD = LineLBD.aDgnStfnD[i];
			int nStfnLineSize = LineStfnD.aStfnDgnBaseD.GetSize();
			for ( j = 0; j < nStfnLineSize; j++ )
			{
				int nLid = GLineDST.aStiffener[i].aLineIx[j];
				T_GSEC_LINE StLine = param.GLineData.aLine[nLid];
				T_GLINE_DGN_B LineDgnB = LineLBD.aDgnStfnD[i].aStfnDgnBaseD[j];
				int iStressType = LineDgnB.iStressType;
				T_GLINE_BASE LineBaseD;
				LineBaseD.aLineIx.Add(GLineDST.aStiffener[i].aLineIx[j]);
				LineBaseD.db = LineDgnB.db;
				LineBaseD.dt = LineDgnB.dt;
				LineBaseD.dy1 = LineDgnB.dy1;
				LineBaseD.dz1 = LineDgnB.dz1;
				LineBaseD.dy2 = LineDgnB.dy2;
				LineBaseD.dz2 = LineDgnB.dz2;
				LineBaseD.iLineType = LineDgnB.iLineType;
				LineBaseD.ve1 = StLine.v1;
				LineBaseD.ve2 = StLine.v2;
				Get_EffectivePanelInformation(param, iClass, bInternal, iStressType, aForce, dCzm_gap, dStiff, LineBaseD, LineDgnB, EffGLineD);
			}
		}
	}

	return TRUE;
}
BOOL CDgnPlateGirder::Get_EffectiveSectInfo4PartLine(T_PLATE_GIRDER_CALC_D& param, double* aForce, double dCzm_gap, double dStiff[3], const T_GLINE_PART_DGN& PartDgnD, const T_DLINE_PART_D& DLinePart, T_GENL_DATA& EffGLineD)
{
	// Outstand  
	int iOutstandSize = PartDgnD.aOutstand.GetSize();
	for ( int i = 0; i < iOutstandSize; i++ )
	{
		T_GLINE_DST GLineDST = PartDgnD.aOutstand[i];
		T_DGNLINE_LBD LineLBD = DLinePart.aOutstand[i];
		Get_EffectiveSectInfo4PlateElements(param, DLinePart.iClass, aForce, dCzm_gap, dStiff, GLineDST, LineLBD, EffGLineD);
	}

	// Internal
	int iInternalSize = PartDgnD.aInternal.GetSize();
	for ( int i = 0; i < iInternalSize; i++ )
	{
		T_GLINE_DST GLineDST = PartDgnD.aInternal[i];
		T_DGNLINE_LBD LineLBD = DLinePart.aInternal[i];
		Get_EffectiveSectInfo4PlateElements(param, DLinePart.iClass, aForce, dCzm_gap, dStiff, GLineDST, LineLBD, EffGLineD);
	}

	return TRUE;
}
BOOL CDgnPlateGirder::Get_EffectiveSectData(BOOL bAfter, double* dForce, T_PLATE_GIRDER_CALC_D& param, T_DLINE_CALC_D& DLineInfo, T_GENL_DATA& EffGLineD, T_GLINE_DGN_D& EffGLineDgn)
{
	T_MATD_D MatdD = param.MatlElem.MatdD;
	double dfy1 = MatdD.Data1.Design.S_Fy1;
	double dfy2 = MatdD.Data1.Design.S_Fy2;
	double da = param.TrstPosi.dPitch;

	double dFx = dForce[0];
	double dMy = dForce[4];
	double dMz = dForce[5];

	BOOL bPosiI = param.bPosiI;
	T_SECT_D SectD = param.SectElem.SectD;
	int nSectType = m_pDgnSectUtil->Get_RealSectType(&SectD);
	int nPosId = bPosiI ? 0 : 1;
	T_SECT_STIFFNESS Stiffness;
	if ( !bAfter )
	{
		Get_GirderStiffnessofCompo(bPosiI, param.SectElem.SectD, Stiffness);
	}
	else
	{
		if ( dMy >= 0.0 ) Stiffness = param.SectElem.SectPosi[nPosId].RebarSect;
		else         Stiffness = param.SectElem.SectPosi[nPosId].NonConSect;
	}
	double dCzm_org = param.SectElem.SectPosi[nPosId].PureSect.Czm;
	double dCzm_gap = dCzm_org - Stiffness.Czm;

	double dArea = Stiffness.Area;
	double dIy = Stiffness.Ryy;
	double dIz = Stiffness.Rzz;
	double aStiff[3] = { dArea, dIy, dIz };

	T_GLINE_DGN_D GLineDgn = bPosiI ? param.SectElem.GLineDgn[0] : param.SectElem.GLineDgn[1];
	Get_CalcSectInfo4EffeSect(param, dfy1, dfy2, da, dForce, dCzm_gap, aStiff, DLineInfo);

	param.aEffLineIx.RemoveAll();
	CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&> aPartLine;
	aPartLine.Copy(GLineDgn.aPartLine);
	int iPartSize = aPartLine.GetSize();
	EffGLineDgn.aPartLine.SetSize(iPartSize);
	int i = 0, j = 0, k = 0;
	for ( i = 0; i < iPartSize; i++ )
	{
		T_GLINE_PART_DGN PartDgnD = aPartLine[i];
		T_DLINE_PART_D DlinePartD = DLineInfo.aPartLine[i];
		if ( PartDgnD.nPartType == 0 ) DlinePartD.iClass = DLineInfo.iClassFlgTop;
		else if ( PartDgnD.nPartType == 1 ) DlinePartD.iClass = DLineInfo.iClassFlgBot;
		else if ( PartDgnD.nPartType == 2 ) DlinePartD.iClass = DLineInfo.iClassWeb;

		Get_EffectiveSectInfo4PartLine(param, dForce, dCzm_gap, aStiff, PartDgnD, DlinePartD, EffGLineD);
	}

	return TRUE;
}
BOOL CDgnPlateGirder::Get_ElasticResistance4GenSect(BOOL bPositive, double aForceGd[6], double aForce[6], T_PLATE_GIRDER_CALC_D& param, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail)
{
	// Material.
	T_MATD_D MatdD = param.MatlElem.MatdD;
	// Steel.
	double dfy1 = MatdD.Data1.Design.S_Fy1;
	double dfy2 = MatdD.Data1.Design.S_Fy2;
	double dfu = MatdD.Data1.Design.S_Fu;
	double dEs = MatdD.Data1.Analysis.Elast;
	// Concrete
	double dfck = param.MatlElem.dfck;
	double dfcm = param.MatlElem.dfcm;
	double dfctm = param.MatlElem.dfctm;
	double dEcm = param.MatlElem.dEcm;
	// Rebar
	double dfsk = param.MatlElem.MatdD.MainRebarData.B_fy;
	double dErs = dEs; // Steel로..

	// Material
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;

	double dfcd = dgam_c == 0.0 ? dfck : dfck / dgam_c;
	double dfsd = dgam_sr == 0.0 ? dfsk : dfsk / dgam_sr;

	// Section.
	T_SECT_D SectD = param.SectElem.SectD;
	T_SECT_SECTBASE_D SectBase = param.SectBaseD;
	T_SECT_STIFFNESS NonConSect; NonConSect.Initialize();
	T_SECT_STIFFNESS RebarSect;  RebarSect.Initialize();

	BOOL bPosiI = param.bPosiI;
	GetNonComSectStiffD(param.SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(param.SectElem, bPosiI, RebarSect);

	T_SECT_STIFFNESS GirderStiff; GirderStiff.Initialize();
	Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);

	double dHt = Get_Height4Gen(param);

	// Force
	double dFx = 0., dMy = 0., dMz = 0.;
	double dMa_Ed = 0., dMc_Ed = 0.0;
	double dMel_Rd = 0.0;
	double dNc_el = 0.0;
	dFx = aForce[0];
	dMy = aForce[4];
	dMz = aForce[5];
	dMc_Ed = dMy;
	dMa_Ed = aForceGd[4];

	int iClassSect = PgbrBase.iSectClass;
	//
	double dForce2[6];
	T_DLINE_CALC_D DLineInfo;
	DLineInfo.iClassFlgTop = param.ClassBase.iTopFlgClass;
	DLineInfo.iClassWeb = param.ClassBase.iWebClass;
	DLineInfo.iClassFlgBot = param.ClassBase.iBotFlgClass;

	// 축력에 대하여.
	memset(dForce2, 0, sizeof(dForce2));
	dForce2[0] = aForceGd[0];
	//T_GENL_DATA EffGLineGdD = GLineData;
	T_GENL_DATA EffGLineD = param.GLineData;
	T_GLINE_DGN_D EffGLineDgn;
	double da = param.TrstPosi.dPitch;
	//
	double dDelta_z_a = 0.0, dDelta_z_c = 0.0;
	double dDelta_Ma_y = 0.0, dDelta_Mc_y = 0.0;

	// Girder만 있을 경우       
	T_SECT_STIFFNESS TempEffStiff4Fx; TempEffStiff4Fx.Initialize();
	T_SECT_STIFFNESS TempCompEffStiff4Fx; TempCompEffStiff4Fx.Initialize();

	T_SECT_STIFFNESS GirderEffStiff4Fx; GirderEffStiff4Fx.Initialize();
	BOOL bAfter = FALSE;
	if ( iClassSect == 4 && dForce2[0] < -m_dZero ) // 압축일 경우만 계산한다. 
	{
		Get_EffectiveSectData(bAfter, dForce2, param, DLineInfo, EffGLineD, EffGLineDgn);
		Get_EffectivePropertyForClass4(param, bPositive, EffGLineD, GirderEffStiff4Fx, TempCompEffStiff4Fx);
		dDelta_z_a = GirderStiff.Czm - GirderEffStiff4Fx.Czm;
		dDelta_Ma_y = dDelta_z_a * fabs(dForce2[0]);
	}
	else
	{
		GirderEffStiff4Fx = GirderStiff;
	}

	// Composite일 때 작용한 Force
	EffGLineD.Initialize();
	EffGLineD = param.GLineData;
	memset(dForce2, 0, sizeof(dForce2));
	dForce2[0] = aForce[0];
	bAfter = TRUE;
	T_SECT_STIFFNESS CompEffStiffness4Fx; CompEffStiffness4Fx.Initialize();
	if ( iClassSect == 4 && dFx < -m_dZero ) // 압축일 경우만 계산한다. 
	{
		Get_EffectiveSectData(bAfter, dForce2, param, DLineInfo, EffGLineD, EffGLineDgn);

		Get_EffectivePropertyForClass4(param, bPositive, EffGLineD, TempEffStiff4Fx, CompEffStiffness4Fx);

		if ( bPositive )
		{
			dDelta_z_c = RebarSect.Czm - CompEffStiffness4Fx.Czm;
			dDelta_Mc_y = dDelta_z_c * fabs(dFx); // if dDelta_z<0, negative moment.
		}
		else
		{
			dDelta_z_c = NonConSect.Czm - CompEffStiffness4Fx.Czm;
			dDelta_Mc_y = dDelta_z_c * fabs(dFx); // if dDelta_z<0, negative moment.
		}
	}
	else
	{
		CompEffStiffness4Fx = RebarSect; // 축력에 관한 것이므로.. 
	}


	// 모멘트에 대하여.
	// Girder
	EffGLineD.Initialize();
	EffGLineD = param.GLineData;
	memset(dForce2, 0, sizeof(dForce2));
	dForce2[4] = aForceGd[4];
	T_SECT_STIFFNESS GirderEffStiff4My; GirderEffStiff4My.Initialize();
	T_SECT_STIFFNESS TempCompEffStiffness4My; TempCompEffStiffness4My.Initialize();
	bAfter = FALSE;
	if ( iClassSect == 4 )
	{
		Get_EffectiveSectData(bAfter, dForce2, param, DLineInfo, EffGLineD, EffGLineDgn);
		Get_EffectivePropertyForClass4(param, bPositive, EffGLineD, GirderEffStiff4My, TempCompEffStiffness4My);
	}
	else
	{
		GirderEffStiff4My = GirderStiff;
	}

	// Composite
	EffGLineD.Initialize();
	EffGLineD = param.GLineData;
	memset(dForce2, 0, sizeof(dForce2));
	dForce2[4] = dMy;
	bAfter = TRUE;
	T_SECT_STIFFNESS TempEffStiffness4My; TempEffStiffness4My.Initialize();
	T_SECT_STIFFNESS CompEffStiffness4My; CompEffStiffness4My.Initialize();
	if ( iClassSect == 4 )
	{
		Get_EffectiveSectData(bAfter, dForce2, param, DLineInfo, EffGLineD, EffGLineDgn);
		Get_EffectivePropertyForClass4(param, bPositive, EffGLineD, TempEffStiffness4My, CompEffStiffness4My);
	}
	else
	{
		CompEffStiffness4My = bPositive ? RebarSect : NonConSect;
	}

	double dMy_gir = aForceGd[4] + dDelta_Ma_y;
	double dMy_com = dMy + dDelta_Mc_y;

	CArray<T_DLINE_PART_D, T_DLINE_PART_D&> aTopFlangePart;
	CArray<T_DLINE_PART_D, T_DLINE_PART_D&> aBotFlangePart;
	CArray<T_DLINE_PART_D, T_DLINE_PART_D&> aWebPart;

	Get_DLinePartData(DLineInfo, aTopFlangePart, aBotFlangePart, aWebPart);
	double dAf_t = 0.0, dAf_b = 0.0;
	double dtf_top = 0.0;
	double dtf_bot = 0.0;
	Get_SectPropOfDLinePart(aTopFlangePart, dAf_t, dtf_top);
	Get_SectPropOfDLinePart(aBotFlangePart, dAf_b, dtf_bot);

	//
	double dfyt = Get_fy4Thick(dfy1, dfy2, dtf_top);
	double dfyb = Get_fy4Thick(dfy1, dfy2, dtf_bot);
	double dfyd_t = dgam_M0 == 0.0 ? dfyt : dfyt / dgam_M0;
	double dfyd_b = dgam_M0 == 0.0 ? dfyb : dfyb / dgam_M0;

	T_SECT_STIFFNESS EffeStlSect; EffeStlSect.Initialize();
	T_SECT_STIFFNESS EffeComSect; EffeComSect.Initialize();

	param.tEffeStlSect = EffeStlSect = GirderEffStiff4My; // Fx에 대한 Effective stiffness
	param.tEffeComSect = EffeComSect = CompEffStiffness4My; // My에 대한 Effective stiffness

	// Before
	double dStl_Area = GirderEffStiff4Fx.Area;
	double dStl_Iyy = GirderEffStiff4My.Ryy;
	double dStl_Izz = GirderEffStiff4My.Ryy;
	double dStl_Czm = GirderEffStiff4My.Czm;
	double dStl_Czp = GirderEffStiff4My.Czp;
	double dStl_zDim = dStl_Czp + dStl_Czm;
	double dStl_Stop = dStl_Czp;
	// Composite
	double dArea_c = CompEffStiffness4Fx.Area;
	double dIyy_c = CompEffStiffness4My.Ryy;
	double dIzz_c = CompEffStiffness4My.Rzz;
	double dCzm_c = CompEffStiffness4My.Czm;
	double dCzp_c = CompEffStiffness4My.Czp;
	double dStop = 0.0;

	// Rebar  
	int i = 0;
	double dRbarZ_max = 0.0;
	int iRbarSize = param.arRbarPosi.GetSize();
	for ( i = 0; i < iRbarSize; i++ )
	{
		_DGN_RBAR_CRC RbarD = param.arRbarPosi[i];
		if ( i == 0 ) { dRbarZ_max = RbarD.dz; }
		else
		{
			if ( dRbarZ_max < RbarD.dz )
			{
				dRbarZ_max = RbarD.dz;
			}
		}
	}

	double dSIGa_top = 0.0;
	double dSIGa_bot = 0.0;

	// 이대근 부장님과 확인필요.   
	// Modify, Jaeoh. [6/2/2011] Mel,Rd 계산방법 수정.
	// 정모멘트 : Slab Top, girder top, bottom 검토해서 불리한..
	// 부모멘트 : Slab Rebar, girder top, bottom 검토해서 불리한..
	double dFx_a = aForceGd[0];
	double dMz_a = 0.0;
	double dStrStlTop = Calc_StressOfPosition(dFx_a, dMy_gir, dMz_a, dStl_Area, dStl_Iyy, dStl_Izz, 0.0, dStl_Czp);
	double dStrComTop = Calc_StressOfPosition(dFx, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, dCzp_c);
	double dStrStlBot = Calc_StressOfPosition(dFx_a, dMy_gir, dMz_a, dStl_Area, dStl_Iyy, dStl_Izz, 0.0, -dStl_Czm);
	double dStrComBot = Calc_StressOfPosition(dFx, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, -dCzm_c);
	dSIGa_top = dStrStlTop + dStrComTop; // (-) 압축, (+) 인장.
	dSIGa_bot = dStrStlBot + dStrComBot;

	double dn = dEcm == 0.0 ? 0.0 : dEs / dEcm;
	double dCentToSlabTop = dHt - dCzm_c;
	double dStrSlab_top = Calc_StressOfPosition(dFx, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, dCentToSlabTop);
	dStrSlab_top = dn == 0.0 ? 0.0 : dStrSlab_top / dn;
	double dCentToRbar = dRbarZ_max - dCzm_c;
	double dStrRbar_top = Calc_StressOfPosition(dFx, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, dCentToRbar);

	double dM_Ed_top = dSIGa_top * (dIyy_c / dCzp_c); // For shear resistance
	double dM_Ed_bot = dSIGa_bot * (dIyy_c / dCzm_c);
	dM_Ed_top = fabs(dM_Ed_top);
	dM_Ed_bot = fabs(dM_Ed_bot);
	double dM_Ed = max(dM_Ed_top, dM_Ed_bot);

	double dCzp_sb = Get_CzpofSlab(param);

	////// 
	// k : 수정 2013.10.28  // Compression(-), Tension(+)
	//     수정 2014.04.17  :  개념 : Sig_a + k*Sig_c = Sig_y
	//                                Sig_c 와 Sig_y의 부호는 같도록 한다.
	//     수정 2014.04.29  :   Sig_a + Sig_Dela + k*Sig_c + Sig_Delc  = Sig_y

	//double dk_slab_temp = dStrSlab_top==0.0 ? dfcd/m_dZero : fabs(dfcd/dStrSlab_top);
	double dSigma_sl_gir = 0.0; // 합성전 슬래브 단면 없다.
	double dSigma_sl_My = (dIyy_c == 0.0) ? 1.0 / m_dZero : (-1.)*dMy           *dCentToSlabTop / dIyy_c / dn;
	double dSigma_sl_DelMy = (dIyy_c == 0.0) ? 1.0 / m_dZero : (-1.)*dDelta_Mc_y   *dCentToSlabTop / dIyy_c / dn;
	double dfcd_temp = (dSigma_sl_My + dSigma_sl_DelMy < 0.0) ? (-1.0)*dfcd : dfcd;
	double dk_slab = dSigma_sl_My == 0.0 ? dfcd / m_dZero : (dfcd_temp - dSigma_sl_gir - dSigma_sl_DelMy) / (dSigma_sl_My);

	//double dk_rbar_temp = dStrRbar_top==0.0 ? dfsd/m_dZero : fabs(dfsd/dStrRbar_top);
	double dSigma_rb_gir = 0.0; // 합성전 철근 없다.
	double dSigma_rb_My = (dIyy_c == 0.0) ? 1.0 / m_dZero : (-1.)*dMy           *dCentToRbar / dIyy_c;
	double dSigma_rb_DelMy = (dIyy_c == 0.0) ? 1.0 / m_dZero : (-1.)*dDelta_Mc_y   *dCentToRbar / dIyy_c;
	double dfsd_temp = (dSigma_rb_My + dSigma_rb_DelMy < 0.0) ? (-1.0)*dfsd : dfsd;
	double dk_rbar = dSigma_rb_My == 0.0 ? dfsd / m_dZero : (dfsd_temp - dSigma_rb_gir - dSigma_rb_DelMy) / (dSigma_rb_My);

	//double dk_top_temp = dStrComTop==0.0 ? (dfyd_t-fabs(dStrStlTop))/m_dZero : (dfyd_t-fabs(dStrStlTop))/fabs(dStrComTop);
	double dSigma_top_gir = (dIyy_c == 0.0) ? 1.0 / m_dZero : (-1.)*dMy_gir       *dStl_Czp / dStl_Iyy;
	double dSigma_top_My = (dIyy_c == 0.0) ? 1.0 / m_dZero : (-1.)*dMy           *dCzp_c / dIyy_c;
	double dSigma_top_DelMy = (dIyy_c == 0.0) ? 1.0 / m_dZero : (-1.)*dDelta_Mc_y   *dCzp_c / dIyy_c;
	double dfyd_t_temp = (dSigma_top_My + dSigma_top_DelMy < 0.0) ? (-1.0)*dfyd_t : dfyd_t;
	double dk_top = dSigma_top_My == 0.0 ? 1.0 / m_dZero : (dfyd_t_temp - dSigma_top_gir - dSigma_top_DelMy) / (dSigma_top_My);

	//double dk_bot_temp = dStrComBot==0.0 ? (dfyd_b-fabs(dStrStlBot))/m_dZero : (dfyd_b-fabs(dStrStlBot))/fabs(dStrComBot);
	double dSigma_bot_gir = (dIyy_c == 0.0) ? 1.0 / m_dZero : (-1.)*dMy_gir       *(-dStl_Czm) / dStl_Iyy;
	double dSigma_bot_My = (dIyy_c == 0.0) ? 1.0 / m_dZero : (-1.)*dMy           *(-dCzm_c) / dIyy_c;
	double dSigma_bot_DelMy = (dIyy_c == 0.0) ? 1.0 / m_dZero : (-1.)*dDelta_Mc_y   *(-dCzm_c) / dIyy_c;
	double dfyd_b_temp = (dSigma_bot_My + dSigma_bot_DelMy < 0.0) ? (-1.0)*dfyd_t : dfyd_t;
	double dk_bot = dSigma_bot_My == 0.0 ? 1.0 / m_dZero : (dfyd_b_temp - dSigma_bot_gir - dSigma_bot_DelMy) / (dSigma_bot_My);
	//////

	BOOL bPositive2 = (dMy_com >= 0.0) ? TRUE : FALSE;
	int    nkminType = 0;
	double dk_min = Calc_kmin(bPositive2, dk_rbar, dk_slab, dk_top, dk_bot, nkminType);
	//   if(bPositive)
	//   {
	//     dk_min = min(dk_slab, min(dk_top, dk_bot));
	//     // Modify, Jaeoh. 이대근 부장님 요청. [6/10/2011], k<0 일경우 dMel_Rd를 fy관련 하여 계산해주기.
	//   }
	//   else
	//   {
	//     dk_min = min(dk_rbar, min(dk_top, dk_bot));    
	//   }

	//   if(dk_min<0.0)
	//   {
	//     double dMel_Rd_k_top = Get_Mel_RdOfElastResistance(dfyd_t, dFx_a, dMy_gir, dStl_Area, dStl_Iyy, dStl_Czp);
	//     double dMel_Rd_k_bot = Get_Mel_RdOfElastResistance(dfyd_b, dFx_a, dMy_gir, dStl_Area, dStl_Iyy, -dStl_Czm);
	//     dMel_Rd = min(dMel_Rd_k_top, dMel_Rd_k_bot);
	//   }
	//   else
	{
		//dMel_Rd = fabs(dMy_gir) + dk_min*fabs(dMy_com);   
		//double dMel_Rd_temp = dMy_gir + dk_min*dMy_com;
		double dMel_Rd_temp = dMa_Ed + dk_min * dMy + dDelta_Mc_y;
		if ( nkminType == Kmin_Slab || nkminType == Kmin_Rbar )
		{
			dMel_Rd_temp = dk_min * dMy + dDelta_Mc_y;
		}
		dMel_Rd = fabs(dMel_Rd_temp);
	}

	// Positive 일 때만 계산.
	if ( bPositive )
	{
		//double dza_sc = dHt - dStl_Czm - dCzp_sb; // Ma
		// [4/11/2014 joahn], dSig_c_mean 값은 거더를 포함하지 않는 것으로 수정, 최성기 CJ님 요청
		double dzc_sc = dHt - dCzm_c - dCzp_sb;
		double dSig_c_mean = dk_min * dMc_Ed / dIyy_c * dzc_sc / dn; // Slab중심에서의 응력 , dMa_Ed/dStl_Iyy*dza_sc
		double dNc_el_calc = dSig_c_mean * Get_AreaofSlab(param);
		dNc_el = fabs(dNc_el_calc);
	}

	PgbrBase.dMel_Rd = dMel_Rd;
	PgbrBase.dNc_el = dNc_el;
	PgbrBase.dElasticNA = dMy >= 0.0 ? RebarSect.Czm : NonConSect.Czm;
	PgbrBase.dElasticNAEff = EffeComSect.Czm;// 
	//
	PgbrBase.dStrStlTop = dStrStlTop;
	PgbrBase.dStrComTop = dStrComTop;
	PgbrBase.dStrStlBot = dStrStlBot;
	PgbrBase.dStrComBot = dStrComBot;
	PgbrBase.dSIGa_top = dSIGa_top;
	PgbrBase.dSIGa_bot = dSIGa_bot;
	PgbrBase.dk_min = dk_min;
	//////////////////////////////////////////////////////////////////////////
	PgbrDetail.dM_Ed_top = dM_Ed_top;
	PgbrDetail.dM_Ed_bot = dM_Ed_bot;
	PgbrDetail.dM_Ed = dM_Ed;
	PgbrDetail.nKminType = nkminType;

	// BF의 dRho_c
	double dRho_c_BF = 1.0;
	if ( DLineInfo.aPartLine.GetSize() < 3 || DLineInfo.aPartLine[2].aInternal.GetSize() == 0 )
		dRho_c_BF = 1.0;
	else
		dRho_c_BF = DLineInfo.aPartLine[2].aInternal[0].dRho_c;

	PgbrDetail.dRho_c = dRho_c_BF;
	PgbrDetail.EffGirderFx = GirderEffStiff4Fx;
	PgbrDetail.EffCompositeFx = CompEffStiffness4Fx;
	PgbrDetail.EffGirder = EffeStlSect;
	PgbrDetail.EffComposite = EffeComSect;


	if ( IsPrintMode() )
	{
		PgbrDetail.dDelMa_Ed = dDelta_Ma_y;
		PgbrDetail.dDelMc_Ed = dDelta_Mc_y;
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_Mf_Rd_PlasticMoment4GenSect(BOOL bPositive, T_PLATE_GIRDER_CALC_D& param, T_SECT_SECTBASE_D& SectBaseD, double& dMf_Rd)
{
	int i = 0;
	double dPlasNA_sh = 0.0;
	T_GLINE_DGN_D GLineDgnMoD = param.GLineDgn;
	GLineDgnMoD.aModifiedLine.RemoveAll();
	int nWebLineSize = param.aWebLineId.GetSize();
	CMap<int, int&, int, int&> mapWebLine;
	for ( i = 0; i < nWebLineSize; i++ )
	{
		int nWebId = param.aWebLineId[i];
		mapWebLine.SetAt(nWebId, nWebId);
	}

	int nMoLineSize = param.GLineDgn.aModifiedLine.GetSize();
	for ( i = 0; i < nMoLineSize; i++ )
	{
		int nLineId = 0;
		if ( !mapWebLine.Lookup(i, nLineId) )
		{
			GLineDgnMoD.aModifiedLine.Add(param.GLineDgn.aModifiedLine[i]);
		}
	}

	double dPlasticNAOfMf_Rd = Get_PlasticNA4GenSect(bPositive, param, SectBaseD, GLineDgnMoD);

	T_PGBR_DETAIL TempPgbrDetail;
	Get_PlasticMoment4GenSect(bPositive, param, GLineDgnMoD, dPlasticNAOfMf_Rd, dMf_Rd, TempPgbrDetail);

	return TRUE;
}

BOOL CDgnPlateGirder::Get_PlasticMoment4GenSect(BOOL bPositive, T_PLATE_GIRDER_CALC_D& param, const T_GLINE_DGN_D& GLineDgn, double dPlasticNA, double& dMpl_Rd, T_PGBR_DETAIL& PgbrDetail)
{
	// Material.
	T_MATD_D MatdD = param.MatlElem.MatdD;
	// Steel.
	double dfy1 = MatdD.Data1.Design.S_Fy1;
	double dfy2 = MatdD.Data1.Design.S_Fy2;
	double dfu = MatdD.Data1.Design.S_Fu;
	double dEs = MatdD.Data1.Analysis.Elast;
	// Concrete
	double dfck = param.MatlElem.dfck;
	double dfcm = param.MatlElem.dfcm;
	double dfctm = param.MatlElem.dfctm;
	double dEcm = param.MatlElem.dEcm;
	// Rebar
	double dfsk = param.MatlElem.MatdD.MainRebarData.B_fy;
	double dErs = dEs; // Steel로..

	// Material
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;

	double dfcd = dgam_c == 0.0 ? dfck : dfck / dgam_c;
	double dfsd = dgam_sr == 0.0 ? dfsk : dfsk / dgam_sr;

	double dCzp = param.SectBaseD.Stiffness.Czp;
	double dCzm = param.SectBaseD.Stiffness.Czm;

	double dCpz = dCzp; // 철근정보를 어떻게 만들수 있나.??
	double dHt = Get_Height4Gen(param);
	double dHg = 0.0; // girder height, ??

	double dNeuC = bPositive ? dHt - dPlasticNA : dPlasticNA;
	// dPlasticNA는 정,부모멘트의 방향을 고려.

	double dCzp_slab = Get_CzpofSlab(param);
	double dCzm_slab = Get_CzmofSlab(param);
	double dHsb = dCzp_slab + dCzm_slab;
	double dZcen_BotPos = dHt - dHsb; // girder bottom에서 Slab bottom까지 거리   

	double aArea[4];
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aTopLine, aBotLine, aHCutLine;
	CArray<double, double&> aHCutTopThick;
	T_GSEC_POLYGON PolyD;
	double aZcen[4] = { 0.0 }; // 자르는 직선기준으로 잘려진 단면의 도심 
	// Convert
	T_SECT_SECTBASE_D CalcSectBaseD; CalcSectBaseD.Initialize();
	Get_ConvertSectBase4GenLineData(param.SectBaseD, GLineDgn, CalcSectBaseD);
	CSectUtil::Get_SectPropertyOfCuttingArea(dPlasticNA, CalcSectBaseD, aArea, aZcen, PolyD, aTopLine, aBotLine, aHCutLine, aHCutTopThick);

	double dDist_PNAtoSlabTop = 0.0, dDist_PNAtoSlabBot = 0.0;
	if ( dPlasticNA > dZcen_BotPos )
	{
		dDist_PNAtoSlabTop = aZcen[0];
		dDist_PNAtoSlabBot = aZcen[1];
	}
	else
	{
		dDist_PNAtoSlabTop = dHt - dPlasticNA - dCzp_slab;
		dDist_PNAtoSlabBot = 0.0;
	}

	double dMpl_gt = 0.0, dMpl_gb = 0.0; // girder top, bottom
	double dMpl_lgt = 0.0, dMpl_lgb = 0.0;
	double dMpl_st = 0.0, dMpl_sb = 0.0; // slab 압축
	double dMpl_rt = 0.0, dMpl_rb = 0.0; // Rebar 인장.   

	int i = 0;
	dMpl_Rd = 0.0;
	BOOL bYaxis = TRUE;
	// Rebar
	double dNr_top = 0.0, dNr_bot = 0.0;
	int iRbarSize = param.arRbarPosi.GetSize();
	if ( !bPositive )
	{
		for ( i = 0; i < iRbarSize; i++ )
		{
			_DGN_RBAR_CRC RbarD = param.arRbarPosi[i];
			double dAs = RbarD.dArea;
			double dzr = RbarD.dz - dPlasticNA; // 확인해야함. 
			double dN_bari = dAs * dfsd;
			double dMpl_ri = dAs * dfsd*fabs(dzr);
			if ( dzr >= 0.0 )
			{
				dNr_top += dN_bari;
				dMpl_rt += dMpl_ri;
			}
			else
			{
				dNr_bot += dN_bari;
				dMpl_rb += dMpl_ri;
			}
		}
	}

	// girder
	double aMaxPt[2] = { 0.0 }, aMinPt[2] = { 0.0 };
	CSectUtil::Get_MaxMinPoint4GenPart(CalcSectBaseD, aMaxPt, aMinPt);
	double dCzm_NAz = aMinPt[1] + dPlasticNA; // 소성 중립축 좌표.

	double dNg_top = 0.0, dNg_bot = 0.0;
	double dNg_ltop = 0.0, dNg_lbot = 0.0;
	Get_PlasticMomentOfDividedPart(dfy1, dfy2, dCzm_NAz, PolyD, aTopLine, dMpl_gt, dNg_top);
	Get_PlasticMomentOfDividedPart(dfy1, dfy2, dCzm_NAz, PolyD, aBotLine, dMpl_gb, dNg_bot);
	if ( aHCutLine.GetSize() > 0 )
		Get_PlasticMomentOfHoriDividedLine(dfy1, dfy2, dCzm_NAz, PolyD, aHCutLine, aHCutTopThick, dMpl_lgt, dMpl_lgb, dNg_ltop, dNg_lbot);

	// Slab
	double A_slab_top = aArea[0];
	double A_slab_bot = aArea[1];
	double dN_slab_top = m_dAlpha * A_slab_top*dfcd;
	double dN_slab_bot = m_dAlpha * A_slab_bot*dfcd;
	dMpl_st = dN_slab_top * fabs(dDist_PNAtoSlabTop);
	dMpl_sb = dN_slab_bot * fabs(dDist_PNAtoSlabBot);

	double dN_slab = 0.0;
	if ( bPositive )
	{
		dN_slab = dN_slab_top;
		dMpl_Rd = (dMpl_gt + dMpl_lgt) + dMpl_gb + dMpl_st;
	}
	else
	{
		dN_slab = dN_slab_bot;
		dMpl_Rd = (dMpl_gt + dMpl_lgb) + dMpl_gb + dMpl_rt + dMpl_rb;
	}

	if ( IsPrintMode() )
	{
		PgbrDetail.dN_slab = dN_slab;
		PgbrDetail.dNr_top = dNr_top;
		PgbrDetail.dNr_bot = dNr_bot;
		PgbrDetail.dNg_top = dNg_top + dNg_ltop;
		PgbrDetail.dNg_bot = dNg_bot + dNg_lbot;
	}

	return TRUE;
}
BOOL CDgnPlateGirder::Calc_LongitudinalShearResistance4GenSect(BOOL bSLS, T_PLATE_GIRDER_CALC_D& param, double dks, T_PGLS_BASE& PglsGd, T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail)
{
	T_CLAS_BASE& ClassD = param.ClassBase;
	T_ELEM_K ElemK = param.ElemK.first;
	// Material.
	T_MATD_D MatdD = param.MatlElem.MatdD;
	// Steel.
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfu = MatdD.Data1.Design.S_Fu;
	double dEs = MatdD.Data1.Analysis.Elast;
	// Concrete
	double dfck = param.MatlElem.dfck;
	double dfcm = param.MatlElem.dfcm;
	double dfctm = param.MatlElem.dfctm;
	double dEcm = param.MatlElem.dEcm;
	// Rebar
	double dfsk = param.MatlElem.MatdD.MainRebarData.B_fy;
	double dErs = dEs; // Steel로..

	// Material
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	//
	double dgam_M1 = m_dGamma_M1;
	//double dgam_M2=1.0;
	double dgam_V = m_dGamma_V;

	double dfcd = dgam_c == 0.0 ? dfck : dfck / dgam_c;
	double dfsd = dgam_sr == 0.0 ? dfsk : dfsk / dgam_sr;

	BOOL bPosiI = param.bPosiI;
	// Section.
	T_SECT_D SectD = param.SectElem.SectD;
	T_SECT_SECTBASE_D SectBase = param.SectBaseD;
	T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
	T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
	T_SECT_STIFFNESS OrgSect;		OrgSect.Initialize();

	GetPureSectStiffD(param.SectElem, bPosiI, OrgSect);
	GetNonComSectStiffD(param.SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(param.SectElem, bPosiI, RebarSect);

	T_SECT_STIFFNESS GirderStiff; GirderStiff.Initialize();
	Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);

	double dBc = Get_BcofSlab(param);
	double dArea_sb = Get_AreaofSlab(param);
	double dCzp_sb = Get_CzpofSlab(param);
	double dCzm_sb = Get_CzmofSlab(param);
	double dtc = dCzp_sb + dCzm_sb; // 직사각형
	double dZbar_sb = Get_ZcentroidofSlab(param);
	// 전체높이
	double dHt = Get_Height4Gen(param);

	// Force
	double aForce[6] = { 0.0,0.0,0.0,0.0,0.0,0.0 };
	double dFx = 0., dMy = 0., dMz = 0.;
	double dNc = 0.0;
	double dMc_Ed = 0.0;
	double dM_Rd = 0.0;
	double dMb_Rd = 0.0;
	double dN_Ed = 0.0;
	double dMy_a = PglsGd.dMy;
	dFx = aForce[0] = PglsBase.dFx;
	dMy = aForce[4] = PglsBase.dMy;
	dMz = aForce[5] = PglsBase.dMz;
	double dV_Ed = PglsBase.dFz;
	double dMa_Ed = PglsBase.dMa_Ed;

	T_PGBR_BASE PgbrBase = param.PgbrBase;
	double dMpl_Rd = PgbrBase.dMpl_Rd;
	double dMel_Rd = PgbrBase.dMel_Rd;
	double dNc_el = PgbrBase.dNc_el;
	double dNc_f = PgbrBase.dNc_f;
	double dM_Ed = dMy_a + dMy;

	///////////////////////////////////////////////////////////////////////
	int iClassSect = ClassD.iClassSect;

	// EN 1994-2, 6.6 Shear connection, 46page
	auto SconPosi = param.SconElem.SconPosi[param.iPos];
	int iNum = SconPosi.iNum;
	double dhsc = SconPosi.dhsc;
	double dfu_sc = SconPosi.dfu;
	double dDia_sc = SconPosi.dDia;
	double dSt_sc = SconPosi.dSt;
	double dSpace = SconPosi.dSpace;
	double dAlpha = 0.0;
	double dRat_sc = dDia_sc == 0.0 ? 0.0 : dhsc / dDia_sc;
	int iRat_sc_type = 0;
	// 6.6.3.1(1), 52p, EN 1994-2
	if ( 3.0 <= dRat_sc && dRat_sc <= 4.0 )
	{
		dAlpha = 0.2*(dRat_sc + 1.0);
		iRat_sc_type = 1;
	}
	else if ( dRat_sc > 4.0 )
	{
		dAlpha = 1.0;
		iRat_sc_type = 2;
	}
	else
	{
		dAlpha = 0.2*(dRat_sc + 1.0);
		if ( dAlpha > 1.0 )
			dAlpha = 1.0;
		iRat_sc_type = 3;
	}

	// Modify, Jaeoh. [4/28/2009] P_Rd 계산방법 수정 , 이혜연 사원님 요청 
	double dP_Rd1 = 0.8*dfu_sc*m_dPi*pow(dDia_sc, 2) / 4.0 / dgam_V;
	double dP_Rd2 = dgam_V == 0.0 ? 0.0 : 0.29*dAlpha*pow(dDia_sc, 2)*sqrt(dfck*dEcm) / dgam_V;
	double dP_Rd = 0.0;
	double dP_Rd_SLS = 0.0;
	int iCheck_P_Rd = 0;
	if ( dP_Rd1 < dP_Rd2 )  // 6.6.3.1(1), (6.18), (6.19)
	{
		dP_Rd = dP_Rd1;
		iCheck_P_Rd = 0;
	}
	else
	{
		dP_Rd = dP_Rd2;
		iCheck_P_Rd = 1;
	}
	if ( bSLS ) dP_Rd_SLS = dks * dP_Rd;

	double dAz_Slab = 0.0;
	double dAz_Hunch = 0.0;
	double dErat = dEcm == 0.0 ? 0.0 : dEs / dEcm;   // Es/Ec, Section에 있는 값.
	double dZbar = 0.0; // 철근 고려 안한. Original Section Stiffeness.  
	double dIyy_Org = 0.0;

	dZbar = OrgSect.Czm;
	dIyy_Org = OrgSect.Ryy;

	double dHg = GirderStiff.Czm + GirderStiff.Czp;
	if ( dHg - dZbar >= 0.0 )
	{
		dAz_Slab = dErat == 0.0 ? 0.0 : (dArea_sb / dErat)*(dZbar_sb - dZbar);
	}
	else
	{
		dAz_Slab = dErat == 0.0 ? 0.0 : (GirderStiff.Area)*(dZbar - GirderStiff.Czm);
	}

	double de_d = 0.0; // e_d = 0, b_eff = Bc defined in Section Data, 07.06.** 답변메일 내용으로 반영 
	double dLength_v = de_d + dBc; // EN 1994-2, 6.6.2.3, Figure 6.12, 49p~51p 참고.
	double dAzI = dIyy_Org == 0.0 ? 0.0 : (dAz_Slab) / dIyy_Org;
	//
	double dv_L_Ed = 0.0; // Shear flow
	double dv_L_Rd = 0.0;
	if ( bSLS ) dv_L_Rd = dSpace == 0.0 ? 0.0 : dP_Rd_SLS * ((double)iNum) / dSpace;
	else     dv_L_Rd = dSpace == 0.0 ? 0.0 : dP_Rd * ((double)iNum) / dSpace;
	//
	// Modification, [12/5/2011] 이대근 bj님 요청 
	// 정모멘트이면서 Class가 1,2이고 dM_Ed-dMel_Rd>0 인 단면은 dV_L_Ed = (dNc_f-dNc_el)*(fabs(dM_Ed)-dMel_Rd)/(dMpl_Rd-dMel_Rd) 사용
	// 정모멘트 이면서 위 경우가 아니거나, 부모멘트인 경우는 V_L_Ed = V_Ed
	double dV_L_Ed = 0.0;
	if ( (iClassSect == 1 || iClassSect == 2) && dM_Ed >= 0.0 && dM_Ed - dMel_Rd > 0.0 )
	{
		dV_L_Ed = dMpl_Rd - dMel_Rd == 0.0 ? 0.0 : (dNc_f - dNc_el)*(fabs(dM_Ed) - dMel_Rd) / (dMpl_Rd - dMel_Rd);
		dv_L_Ed = dLength_v == 0.0 ? 0.0 : dV_L_Ed / dLength_v;
	}
	else
	{
		dV_L_Ed = dV_Ed;
		dv_L_Ed = fabs(dV_Ed)*dAzI;
	}

	//double dv_L_Ed2 = dLength_v==0.0 ? 0.0 : dV_L_Ed / dLength_v;
	//  
	double dv_Ed = dv_L_Ed / (2.0*dtc);
	double dnu = 0.6*(1.0 - dfck / 250.0);
	double dTheta_f = m_dPi / 180.0*45.0; // 45로 가정
	double dv_Ed_lim = dnu * dfcd*sin(dTheta_f)*cos(dTheta_f);// Designers' guide to EN 1994-2, 128p


	PglsBase.iSectClass = ClassD.iClassSect;
	PglsBase.dNc = dNc;
	PglsBase.dNc_el = dNc_el;
	PglsBase.dNc_f = dNc_f;
	//PglsBase.dMa_Ed   = 0.0;        
	//PglsBase.dMc_Ed   = 0.0;        
	PglsBase.dMpl_Rd = dMpl_Rd;
	PglsBase.dMel_Rd = dMel_Rd;
	PglsBase.dV_L_Ed = dV_L_Ed;
	PglsBase.dv_L_Ed = dv_L_Ed;     // Slab로 계산된.    
	//PglsBase.dv_L_Ed2  = dv_L_Ed2;    // V_L,Ed = Nc-Nc,el로 계산된.
	PglsBase.dP_Rd1 = dP_Rd1;
	PglsBase.dP_Rd2 = dP_Rd2;
	PglsBase.dP_Rd = dP_Rd;
	PglsBase.iCheck_P_Rd = iCheck_P_Rd;
	PglsBase.dv_L_Rd = dv_L_Rd;     // P_Rd*(Stud개수)/간격
	PglsBase.dv_Ed = dv_Ed;       // v_Ed(Slab의..)      
	PglsBase.dv_Ed_lim = dv_Ed_lim;   // v_Ed(Slab의..)      
	//
	PglsBase.dRatio = dv_L_Rd == 0.0 ? fabs(dv_L_Ed) / m_dZero : fabs(dv_L_Ed) / fabs(dv_L_Rd);
	if ( PglsBase.dRatio < 1.0 + m_dZero )
	{
		PglsBase.bOK = TRUE;
		//if(dv_Ed>dv_Ed_lim) PglsBase.bOK = FALSE;    
	}
	else
	{
		PglsBase.bOK = FALSE;
	}

	//
	PglsBase.dAlpha = dAlpha;
	PglsBase.dAzI = dAzI;

	PglsDetail.dmuy_c = dAzI * dIyy_Org;
	PglsDetail.dP_Rd_SLS = dP_Rd_SLS;
	if ( IsPrintMode() )
	{
		PglsDetail.dfu = dfu_sc;
		PglsDetail.iRat_sc_type = iRat_sc_type;
		PglsDetail.iNum = iNum;
		PglsDetail.dDsc = dDia_sc;
		PglsDetail.dhsc = dhsc;
		PglsDetail.dSpace = dSpace;
		PglsDetail.dLv = dLength_v;
	}

	return TRUE;
}
BOOL CDgnPlateGirder::Calc_VerticalShearResistance4GenSect(T_PLATE_GIRDER_CALC_D& param, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	T_CLAS_BASE& ClassD = param.ClassBase;
	T_ELEM_K ElemK = param.ElemK.first;
	// Material.
	T_MATD_D MatdD = param.MatlElem.MatdD;
	int iMatClass = param.MatlElem.iMatClass;
	// Steel.
	double dfy1 = MatdD.Data1.Design.S_Fy1;
	double dfy2 = MatdD.Data1.Design.S_Fy2;
	double dfu = MatdD.Data1.Design.S_Fu;
	double dEs = MatdD.Data1.Analysis.Elast;
	// Concrete
	double dfck = param.MatlElem.dfck;
	double dfcm = param.MatlElem.dfcm;
	double dfctm = param.MatlElem.dfctm;
	double dEcm = param.MatlElem.dEcm;
	// Rebar
	double dfsk = param.MatlElem.MatdD.MainRebarData.B_fy;
	double dErs = dEs; // Steel로..

	// Material
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dgam_M1 = m_dGamma_M1;
	double dgam_M2 = m_dGamma_M0;
	// Modify by GAY. PMS:4216. ('12.05.11).
	double dEta = Get_Eta4SteelGrade(iMatClass, dfy1);
	//   double dEta=0.0; // η may be conservativel taken equal 1.0 
	//   Get_Eta4SteelMaterial(iMatClass, dgam_M0, dgam_M1, dEta);

	double dfcd = dgam_c == 0.0 ? dfck : dfck / dgam_c;
	double dfsd = dgam_sr == 0.0 ? dfsk : dfsk / dgam_sr;

	// Section.  
	T_SECT_D SectD = param.SectElem.SectD;
	T_SECT_SECTBASE_D SectBaseD = param.SectBaseD;
	T_SECT_STIFFNESS NonConSect; NonConSect.Initialize();
	T_SECT_STIFFNESS RebarSect;  RebarSect.Initialize();
	T_SECT_STIFFNESS OrgSect; OrgSect.Initialize();

	BOOL bPosiI = param.bPosiI;
	GetPureSectStiffD(param.SectElem, bPosiI, OrgSect);
	GetNonComSectStiffD(param.SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(param.SectElem, bPosiI, RebarSect);

	// Girder Stiffness	
	T_SECT_STIFFNESS GirderStiffD;  GirderStiffD.Initialize();
	Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiffD);

	// Force
	double aForce[6] = { 0.0,0.0,0.0,0.0,0.0,0.0 };
	double dFx = 0., dMy = 0., dMz = 0.;
	double dNc = 0.0;
	double dNc_el = 0.0;
	double dNc_f = 0.0;
	double dMa_Ed = 0.0;
	double dMc_Ed = 0.0;
	double dMel_Rd = 0.0;
	double dM_Rd = 0.0;
	dFx = aForce[0] = PrvsBase.dFx;
	dMy = aForce[4] = PrvsBase.dMy;
	dMz = aForce[5] = PrvsBase.dMz;
	dMa_Ed = PrvsBase.dMa_Ed;
	dMc_Ed = dMy;
	double dV_Ed_a = PrvsGd.dFz;
	double dV_Ed_c = PrvsBase.dFz;
	double dV_Ed = PrvsBase.dV_Ed;

	T_SECT_STIFFNESS CompoSect;
	CompoSect = dMy >= 0.0 ? RebarSect : NonConSect;
	double dIyy_c = CompoSect.Ryy;
	double dCzp_c = CompoSect.Czp;
	double dCzm_c = CompoSect.Czm;
	double dIyy_a = GirderStiffD.Ryy;

	// Shear buckling resistance, EN 1993-1-5,5 (22page)
	BOOL bEndSupport = param.TsesPosi.bUseStif ? TRUE : FALSE;
	int iEndpost = param.TsesPosi.iEndpost; // 0=Rigid end post, 1=Non-rigid end post, 2=No end post,
	//BOOL bLongStiff = arLstiPosi.GetSize()==0 ? FALSE : TRUE;
	BOOL bTranStiff = param.TrstPosi.bUseStif;

	T_GENL_DATA EffGLineD = param.GLineData;
	T_GLINE_DGN_D EffGLineDgn;

	int iClassSect = ClassD.iClassSect;

	T_DLINE_CALC_D DLineInfo; // 데이터를 받아와야함.
	DLineInfo.iClassFlgTop = ClassD.iTopFlgClass;
	DLineInfo.iClassWeb = ClassD.iWebClass;
	DLineInfo.iClassFlgBot = ClassD.iBotFlgClass;

	BOOL bAfter = TRUE;
	Get_EffectiveSectData(bAfter, aForce, param, DLineInfo, EffGLineD, EffGLineDgn);
	CArray<T_DLINE_PART_D, T_DLINE_PART_D&> aTopFlangePart;
	CArray<T_DLINE_PART_D, T_DLINE_PART_D&> aBotFlangePart;
	CArray<T_DLINE_PART_D, T_DLINE_PART_D&> aWebPart;

	int i = 0, j = 0, k = 0;
	Get_DLinePartData(DLineInfo, aTopFlangePart, aBotFlangePart, aWebPart);

	// EN 1993-1-5 7.1 (28page)
	// 7.1 Interaction between shear force, bending moment and axial force
	double dMf_Rd = param.PgbrBase.dMf_Rd;
	double dMpl_Rd = param.PgbrBase.dMpl_Rd_s; // Bending의 Mpl_Rd와 다를 수 있다. 

	double dM_Ed = param.PgbrDetail.dM_Ed; // for M,Ed/Mpl,Rd
	double dN_Ed = dFx;
	double dFactor_fr = 1.0;
	// 압축일때만 
	double dfy_ft = dfy1, dfy_fb = dfy1;
	double dAf_t = 0.0, dAf_b = 0.0;
	double dtf_t = 0.0, dtf_b = 0.0;
	Get_SectPropOfDLinePart(aTopFlangePart, dAf_t, dtf_t);
	Get_SectPropOfDLinePart(aBotFlangePart, dAf_b, dtf_b);
	if ( dN_Ed < 0.0 ) dFactor_fr = Get_ReductonFactorOfFlange4N_Ed(dgam_M2, dfy_ft, dfy_fb, dN_Ed, dAf_t, dAf_b);
	double dMf_Rd_factor = dFactor_fr * dMf_Rd;

	double dBendingRat = dMpl_Rd == 0.0 ? 0.0 : dMf_Rd_factor / dMpl_Rd;
	double dBar_Eta1 = dMpl_Rd == 0.0 ? 0.0 : max(dM_Ed / dMpl_Rd, dBendingRat); // Modify by GAY. See EN1993-1-5:2004(E), EQ(7.1)
	double dBar_Eta3 = 0.0;

	int iPartSize = DLineInfo.aPartLine.GetSize();
	int iGLinePartSize = param.GLineDgn.aPartLine.GetSize();
	ASSERT(iPartSize == iGLinePartSize);
	int iWebSize = aWebPart.GetSize();
	double dV_Edi = iWebSize == 0 ? 0.0 : dV_Ed / (double)iWebSize; // Web의 수로 나눈다.   

	int iLamda_w_Type = 0;
	int iLamda_w_Type_sub = 2;
	double dLamda_w_sub_max = 0.0;
	double dLamda_w_line_max = 0.0;
	double dLamda_w_part_max = 0.0;
	double dLamda_w_part = 0.0;
	double da_si_cr = 0.0, db_si_cr = 0.0, dt_si_cr = 0.0, dIsl_si_cr = 0.0, dEpsilon_si_cr = 0.0;
	double dk_tau_si_cr = 0.0, dk_tau_st_si_cr = 0.0, dk_tau_st_Limit_si_cr = 0.0;
	double dIsli_cr = 0.0;
	double dLamda_w_si_cr = 0.0;
	double deta3_max = 0.0;
	double dRatio_Interac_MV_max = 0.0;
	int iShearCheckPart = 0;
	int iPartCount = 0;

	double dVw_Rd_sum = 0.0;
	double dVpl_Rd_sum = 0.0;
	CArray<T_PRVS_Vbw_Rd_Param, T_PRVS_Vbw_Rd_Param&> aGenWebVbwRdParam;
	CArray<T_PRVS_Vbw_Rd_Param, T_PRVS_Vbw_Rd_Param&> aGenFlangeVbwRdParam;
	int nShearBuckCount = 0;
	int nSubPannelCalcType = 1;
	double da = param.TrstPosi.dPitch;
	for ( i = 0; i < iPartSize; i++ )
	{
		T_PRVS_Vbw_Rd_Param CalcVbwParam;
		T_DLINE_PART_D DLinePart = DLineInfo.aPartLine[i];
		T_GLINE_PART_DGN GLinePart = param.GLineDgn.aPartLine[i];
		int nPartType = GLinePart.nPartType;
		CalcVbwParam.strGroupName = GLinePart.strGroupName;
		double db_p = DLinePart.db_in;
		double dt_p = DLinePart.dt_in;
		double dAv_p = db_p * dt_p;

		double dfy_p = Get_fy4Thick(dfy1, dfy2, dt_p);
		double dEpsilon_p = Get_Epsilon(dfy_p);
		double dCheck_lim = 0.0;
		BOOL bStiffened = FALSE;
		BOOL bIsShearBuck = FALSE;
		double dk_tau_p = 0.0, dk_tau_st_p = 0.0, dk_tau_st_Limit_p = 0.0;
		int iLongStiffSize_p = 0;
		double dIsl_part_sum = 0.0;
		int iInternalSize = DLinePart.aInternal.GetSize();
		double dk_tau_i = 0.0;
		int iInternalCount = 0;
		int iSubPanelCount = 0;
		for ( j = 0; j < iInternalSize; j++ )
		{
			iInternalCount++;
			T_DGNLINE_LBD DLineLBD = DLinePart.aInternal[j];
			T_GLINE_DST GLineDST = GLinePart.aInternal[j];
			int iSubPanelSize = DLineLBD.aDgnBaseD.GetSize();
			for ( k = 0; k < iSubPanelSize; k++ )
			{
				iSubPanelCount++;
				T_GLINE_BASE GLineBaseD = GLineDST.aLineDgn[k];
				double db_si = GLineBaseD.db;
				double dt_si = GLineBaseD.dt;
				double dfy_si = Get_fy4Thick(dfy1, dfy2, dt_si);
				double dEpsilon_si = Get_Epsilon(dfy_si);
				double dIsl_si = 0.0;
				double dk_tau_si = 0.0;
				double dk_tau_st_si = 0.0;
				double dk_tau_st_Limit_si = 0.0;
				double dLamda_w_si = 0.0;
				//Get_ShearBucklingCoeff(iLongStiffSize, db_i, da, dt_i, dIsl_part_sum, dk_tau_i);
				Get_SlendernessParamLamda(iLamda_w_Type_sub, 0, db_si, da, dt_si, dIsl_si, dEpsilon_si, dk_tau_si, dk_tau_st_si, dk_tau_st_Limit_si, dLamda_w_si);
				if ( iSubPanelCount == 1 )
				{
					dLamda_w_sub_max = dLamda_w_si;
					db_si_cr = db_si, dt_si_cr = dt_si, dIsl_si_cr = dIsl_si, dEpsilon_si_cr = dEpsilon_si;
					dk_tau_si_cr = dk_tau_si, dk_tau_st_si_cr = dk_tau_st_si, dk_tau_st_Limit_si_cr = dk_tau_st_Limit_si;
					dIsli_cr = dIsl_si;
					dLamda_w_si_cr = dLamda_w_si;
					//nSubPannelCalcType = Get_SubPannelCalcType(0);
				}
				else
				{
					if ( dLamda_w_sub_max < dLamda_w_si )
					{
						dLamda_w_sub_max = dLamda_w_si;
						db_si_cr = db_si, dt_si_cr = dt_si, dIsl_si_cr = dIsl_si, dEpsilon_si_cr = dEpsilon_si;
						dk_tau_si_cr = dk_tau_si, dk_tau_st_si_cr = dk_tau_st_si, dk_tau_st_Limit_si_cr = dk_tau_st_Limit_si;
						dIsli_cr = dIsl_si;
						dLamda_w_si_cr = dLamda_w_si;
						//nSubPannelCalcType = Get_SubPannelCalcType(0);
					}
				}
			}
			int iLongStiff = Get_StiffenerSize(GLineDST); //GLineDST.aStiffener.GetSize();
			BOOL bInternalLongStiff = iLongStiff > 0 ? TRUE : FALSE;
			iLamda_w_Type = Get_Lamda_w_Type4Shear(bInternalLongStiff, param.TrstPosi); // Internal의 Lamda,w type 결정       
			double db_i = GLineDST.db;
			double dt_i = GLineDST.dt;
			double dfy_i = Get_fy4Thick(dfy1, dfy2, dt_i);
			double dEpsilon_i = Get_Epsilon(dfy_i);
			double dIsl_i = DLineLBD.dIsl_sh_sum*(1.0 / 3.0);
			double dk_tau_i = 0.0;
			double dk_tau_st_i = 0.0;
			double dk_tau_st_Limit_i = 0.0;
			double dLamda_w_i = 0.0;
			iLongStiffSize_p += iLongStiff;
			dIsl_part_sum += dIsl_i;
			//Get_ShearBucklingCoeff(iLongStiffSize, db_i, da, dt_i, dIsl_part_sum, dk_tau_i);
			Get_SlendernessParamLamda(iLamda_w_Type, iLongStiff, db_i, da, dt_i, dIsl_i, dEpsilon_i, dk_tau_i, dk_tau_st_i, dk_tau_st_Limit_i, dLamda_w_i);
			if ( iInternalCount == 1 )
			{
				dLamda_w_line_max = dLamda_w_i;
			}
			else
			{
				if ( dLamda_w_line_max < dLamda_w_i ) dLamda_w_line_max = dLamda_w_i;
			}
		}
		if ( iInternalCount <= 0 ) continue; // internal이 있는 part만 계산하자. 
		iShearCheckPart++;

		double dtau_Ed_a = 0.0, dtau_Ed_c = 0.0;
		double dtau_Ed = 0.0, dtau_Ed_ave = 0.0, dtau_b_Rd = 0.0, dQflg_a = 0.0, dQflg_c = 0.0, dbf = 0.0;
		double dCzm_gap = 0.0;
		if ( nPartType == 0 || nPartType == 1 ) // Flange
		{
			// Before, tau_Ed,a
			dCzm_gap = OrgSect.Czm - GirderStiffD.Czm;
			Get_MomentofAreaOfFlange(i, dCzm_gap, OrgSect, param.GLineDgn, dQflg_a);
			dtau_Ed_a = dIyy_a * dt_p == 0.0 ? 0.0 : dV_Ed_a * dQflg_a / (dIyy_a*dt_p);

			// After, tau_Ed,c
			dCzm_gap = 0.0;
			Get_MomentofAreaOfFlange(i, dCzm_gap, OrgSect, param.GLineDgn, dQflg_c);
			dtau_Ed_c = dIyy_c * dt_p == 0.0 ? 0.0 : dV_Ed_c * dQflg_c / (dIyy_c*dt_p);
			dtau_Ed = fabs(dtau_Ed_a + dtau_Ed_c);
			dtau_Ed_ave = 0.5*dtau_Ed;
		}

		BOOL bPartLongStiff = iLongStiffSize_p > 0 ? TRUE : FALSE;
		iLamda_w_Type = Get_Lamda_w_Type4Shear(bPartLongStiff, param.TrstPosi); // Internal의 Lamda,w type 결정 
		bStiffened = iLamda_w_Type == 1 || iLamda_w_Type == 2;

		Get_SlendernessParamLamda(iLamda_w_Type, iLongStiffSize_p, db_p, da, dt_p, dIsl_part_sum, dEpsilon_p, dk_tau_p, dk_tau_st_p, dk_tau_st_Limit_p, dLamda_w_part);
		Get_WidthThickRatio4Web(bStiffened, dEta, db_p, dt_p, dEpsilon_p, dk_tau_p, dCheck_lim, bIsShearBuck);
		if ( bIsShearBuck )
		{
			if ( nPartType == 2 ) nShearBuckCount++; // Web일 경우만. 
		}
		// sub 보다는 커야한다.
		double dLamda_w_cr = max(dLamda_w_sub_max, dLamda_w_part);

		double dXwi = 0.0;
		int iLamda_w_scope = 0;
		Get_ContributionFromWebFactor(iEndpost, dEta, dLamda_w_cr, dXwi, iLamda_w_scope);
		double dVbw_Rd_p = Get_Vbw_Rd(dXwi, dfy_p, dAv_p, dgam_M1);
		double dVb_Rd_Limit = Get_Vbw_Rd_Limit(dEta, dfy_p, dAv_p, dgam_M1);
		// Plastic resistance to vertical shear, EN 1993-1-1: 2005, 6.2.6 (50page)
		double dVpl_Rd_p = dgam_M0 == 0.0 ? 0.0 : dEta * dAv_p*(dfy_p / sqrt(3.0)) / dgam_M0;
		if ( nPartType == 2 ) dVpl_Rd_sum += dVpl_Rd_p; // Web일 경우만 

		double dV_Rd_p = 0.0;
		double dEta3_p = 0.0;
		double dRatio_Interac_MV_part = 0.0;
		double dRho_sh = 0.0;
		if ( nPartType == 2 )
		{
			if ( bIsShearBuck )
			{
				dV_Rd_p = min(dVbw_Rd_p, dVb_Rd_Limit);
				dVw_Rd_sum += dV_Rd_p; // 각 Part가 제한값을 넘을 수 없으므로 dVbw_Rd_p를 더하지 말고 dV_Rd_p로 하자. 
			}
			else
			{
				dV_Rd_p = dVpl_Rd_p;
			}
			dEta3_p = dV_Rd_p == 0.0 ? fabs(dV_Edi) / m_dZero : fabs(dV_Edi) / dV_Rd_p;
			//
			if ( iClassSect == 1 || iClassSect == 2 )
			{
				// 6.2.2.4 Bending and vertical shear. EN 1994-2,2005
				double dRho_sh_sqrt = dV_Rd_p == 0.0 ? 0.0 : (2.0*fabs(dV_Ed_c / dV_Rd_p) - 1.0);
				dRho_sh = pow(dRho_sh_sqrt, 2);
				dRho_sh = min(dRho_sh, 1.0); // 1보다는 크면 안된다.
			}
			else if ( iClassSect == 3 || iClassSect == 4 )
			{
				dRatio_Interac_MV_part = Get_RatioOfInteractionMV(dBar_Eta1, dBendingRat, dEta3_p);
			}
		}
		else if ( nPartType == 0 || nPartType == 1 )
		{
			dtau_b_Rd = Get_tau_b_Rd(dEta, dfy_p, dgam_M1);
			dEta3_p = dtau_b_Rd == 0.0 ? fabs(dtau_Ed_ave) / m_dZero : fabs(dtau_Ed_ave) / dtau_b_Rd;
		}
		else ASSERT(0);

		if ( i == 0 )
		{
			deta3_max = dEta3_p;
			dRatio_Interac_MV_max = dRatio_Interac_MV_part;
		}
		else
		{
			if ( dEta3_p < deta3_max ) deta3_max = dEta3_p;
			if ( dRatio_Interac_MV_max < dRatio_Interac_MV_part ) dRatio_Interac_MV_max = dRatio_Interac_MV_part;
		}


		if ( IsPrintMode() )
		{
			double dAlpha_p = Get_Alpha4Lamda_w(da, db_p);
			CalcVbwParam.iStiffSize = iLongStiffSize_p;
			if ( iLongStiffSize_p > 0 )
			{
				int nLongStiffSize_sub_temp = 0; // Sub 이므로 보강재가 없다. 
				double dAlpha_si = Get_Alpha4Lamda_w(da, db_si_cr);
				Set_Calc_Vbw_SubPanel_Param(nLongStiffSize_sub_temp, db_si_cr, dt_si_cr, dAlpha_si, dk_tau_si_cr, dk_tau_st_si_cr, dk_tau_st_Limit_si_cr, dIsli_cr, dLamda_w_si_cr, CalcVbwParam.Lamda_w_SubD);
			}

			CalcVbwParam.nPartType = nPartType;
			CalcVbwParam.bStiffened = bStiffened;
			CalcVbwParam.bIsShearBuck = bIsShearBuck;
			CalcVbwParam.dVpl_Rd = dVpl_Rd_p;
			if ( nPartType == 2 ) // Web
			{
				Set_Calc_Vbw_Param(dAlpha_p, dk_tau_p, dk_tau_st_p, dk_tau_st_Limit_p, dIsl_part_sum, db_p, dt_p, dLamda_w_part, dLamda_w_cr, iLamda_w_Type, CalcVbwParam);
				Set_Calc_Vbw_Param2(iLamda_w_scope, dXwi, dVbw_Rd_p, dV_Rd_p, dVb_Rd_Limit, dV_Edi, dEta3_p, CalcVbwParam);
				CalcVbwParam.dRatio_InteractionMV = dRatio_Interac_MV_part;

				aGenWebVbwRdParam.Add(CalcVbwParam);
			}
			else if ( nPartType == 0 || nPartType == 1 ) // Flange
			{
				Set_Calc_Vbw_Param(dAlpha_p, dk_tau_p, dk_tau_st_p, dk_tau_st_Limit_p, dIsl_part_sum, db_p, dt_p, dLamda_w_part, dLamda_w_cr, iLamda_w_Type, CalcVbwParam);
				Set_Calc_Vbw_Param2(iLamda_w_scope, dXwi, dVbw_Rd_p, dV_Rd_p, dVb_Rd_Limit, dV_Edi, dEta3_p, CalcVbwParam);
				Set_Calc_FlangeShearstress_Param(dtau_Ed_a, dtau_b_Rd, dV_Ed_a, dQflg_a, dIyy_a, dt_p, CalcVbwParam);
				CalcVbwParam.iLamda_w_scope = iLamda_w_scope;
				//
				CalcVbwParam.dQi_c = dQflg_c;
				CalcVbwParam.dIyyi_c = dIyy_c;
				CalcVbwParam.dtau_Ed_c = dtau_Ed_c;
				CalcVbwParam.dtau_Ed_sum = dtau_Ed;
				CalcVbwParam.dtau_Ed_ave = dtau_Ed_ave;

				aGenFlangeVbwRdParam.Add(CalcVbwParam);
			}
			else ASSERT(0);
		}
	}

	// 각 Part별 계산에 적용할 Lamda_w_max를 찾는다. 
	// Plastic resistance to vertical shear, EN 1993-1-1: 2005, 6.2.6 (50page)
	double dAv = 0.0;
	double dV_Rd = 0.0;
	double dVpl_Rd = dVpl_Rd_sum;
	//
	double dVb_Rd = dVw_Rd_sum;
	double dVb_Rd_org = dVw_Rd_sum;
	double dVbw_Rd = dVw_Rd_sum;
	double dVbf_Rd = 0.0;
	double dVb_Rd_Limit = 0.0;
	//
	double dXw = 0.0;
	double dLamda_w = 0.0;
	double dk_tau = 0.0;

	BOOL bEta3Exceed05 = FALSE;
	BOOL bCombinedEffe = FALSE;
	BOOL bEta1Ratio = FALSE;
	if ( deta3_max > 0.5 ) bEta3Exceed05 = TRUE;

	// Contribution from flange, EN 1993-1-5 , 25page
	double dRatio = 0.0;
	if ( nShearBuckCount > 0 )
	{
		dV_Rd = dVb_Rd;
		dRatio = dVb_Rd == 0.0 ? fabs(dV_Ed) / m_dZero : fabs(dV_Ed) / dVb_Rd;
	}
	else
	{
		dV_Rd = dVpl_Rd;
		dRatio = dVpl_Rd == 0.0 ? fabs(dV_Ed) / m_dZero : fabs(dV_Ed) / dVpl_Rd;
	}

	if ( dRatio > 1.0 )
		PrvsBase.bOK = FALSE;
	else
		PrvsBase.bOK = TRUE;


	if ( bEta3Exceed05 )
	{
		if ( dRatio_Interac_MV_max <= 1.0 ) bCombinedEffe = TRUE;
		else                           bCombinedEffe = FALSE;
	}
	else
		bCombinedEffe = TRUE;

	if ( PrvsBase.bOK && bEta3Exceed05 )
	{
		if ( ClassD.iClassSect == 3 || ClassD.iClassSect == 4 )
		{
			if ( !bCombinedEffe )
				PrvsBase.bOK = FALSE;
		}
	}

	PrvsBase.iTopClass = ClassD.iTopFlgClass;
	PrvsBase.iBotClass = ClassD.iBotFlgClass;
	PrvsBase.iWebClass = ClassD.iWebClass;
	PrvsBase.iSectClass = ClassD.iClassSect;
	PrvsBase.dM_Ed = dM_Ed;
	PrvsBase.dMf_Rd = dMf_Rd;
	PrvsBase.dMpl_Rd = dMpl_Rd;
	PrvsBase.dVpl_Rd = dVpl_Rd;
	PrvsBase.dVbw_Rd = dVbw_Rd;
	PrvsBase.dVbf_Rd = dVbf_Rd;
	PrvsBase.dVb_Rd = dVb_Rd;
	PrvsBase.dV_Rd = dV_Rd;
	PrvsBase.dEta1 = dBar_Eta1;
	PrvsBase.dEta3 = deta3_max;
	PrvsBase.dCombinedEffe = dRatio_Interac_MV_max; // Unit = None
	PrvsBase.bCombinedEffe = bCombinedEffe;
	PrvsBase.bEta3Exceed05 = bEta3Exceed05;
	PrvsBase.dRatio = dRatio;
	//
	PrvsBase.dAv = dAv;
	PrvsBase.deta = dEta;
	//PrvsBase.dIsl_org = dIsl_org;   
	//PrvsBase.dIsl = dIsl;         
	PrvsBase.dXw = dXw;
	//PrvsBase.dMax_Bar_Lamda_w = dMax_Bar_Lamda_w;
	//PrvsBase.dc = dc;
	//PrvsBase.dbf = dbf;         
	//PrvsBase.dtf = dtf;         
	PrvsBase.dVb_Rd_org = dVb_Rd_org;
	//PrvsBase.dVb_Rd_Lim = dVb_Rd_Lim;  
	PrvsBase.bEta1Ratio = FALSE;

	// Detail
	PrvsDetail.GLineDgn = param.GLineDgn;
	PrvsDetail.dM_Ed_top = param.PgbrDetail.dM_Ed_top;
	PrvsDetail.dM_Ed_bot = param.PgbrDetail.dM_Ed_bot;
	if ( IsPrintMode() )
	{
		PrvsDetail.GLineDgn = param.GLineDgn;
		PrvsDetail.StressInfo4Class = param.DLineCalcD;
		PrvsDetail.aGenWebVbwRdParam.Copy(aGenWebVbwRdParam);
		PrvsDetail.aGenFlangeVbwRdParam.Copy(aGenFlangeVbwRdParam);
		PrvsDetail.iEndSupportType = iEndpost;
		PrvsDetail.dPlasticNA = param.PgbrBase.dPlasticNA;
		PrvsDetail.dN_slab = param.PgbrDetail.dN_slab;
		PrvsDetail.dNr_top = param.PgbrDetail.dNr_top;
		PrvsDetail.dNr_bot = param.PgbrDetail.dNr_bot;
		PrvsDetail.dNg_top = param.PgbrDetail.dNg_top;
		PrvsDetail.dNg_bot = param.PgbrDetail.dNg_bot;
		PrvsDetail.dMf_Rd_cal = dMf_Rd;
		PrvsDetail.dMf_N_Rd = dMf_Rd_factor;
		PrvsDetail.dReduFactor4N_Ed = dFactor_fr;
		PrvsDetail.nShearBuckCount = nShearBuckCount;
	}
	return TRUE;
}

BOOL CDgnPlateGirder::Calc_BendingResistance4GenSect(T_PLATE_GIRDER_CALC_D& param, T_PGBR_BASE& PgbrGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail)
{
	T_CLAS_BASE& ClassD = param.ClassBase;
	auto ElemK = param.ElemK;
	// Material.
	T_MATD_D MatdD = param.MatlElem.MatdD;
	// Steel.
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfu = MatdD.Data1.Design.S_Fu;
	double dEs = MatdD.Data1.Analysis.Elast;
	// Concrete
	double dfck = param.MatlElem.dfck;
	double dfcm = param.MatlElem.dfcm;
	double dfctm = param.MatlElem.dfctm;
	double dEcm = param.MatlElem.dEcm;
	// Rebar
	double dfsk = param.MatlElem.MatdD.MainRebarData.B_fy;
	double dErs = dEs; // Steel로..

	// Material
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;

	double dfcd = dgam_c == 0.0 ? dfck : dfck / dgam_c;
	double dfsd = dgam_sr == 0.0 ? dfsk : dfsk / dgam_sr;

	double dAlpha = m_dAlpha;
	// Section.  
	T_SECT_D SectD = param.SectElem.SectD;
	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
	T_SECT_SECTBASE_D SectBaseD = param.SectBaseD;
	T_SECT_STIFFNESS NonConSect, RebarSect;
	NonConSect.Initialize();   RebarSect.Initialize();
	BOOL bPosiI = param.bPosiI;
	GetNonComSectStiffD(param.SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(param.SectElem, bPosiI, RebarSect);

	double dCzp = SectBaseD.Stiffness.Czp;
	double dCzm = SectBaseD.Stiffness.Czm;
	double dHg = dCzp + dCzm; // Height of girder

	BOOL bEquivalentClass2 = FALSE;
	int iClassSect = ClassD.iClassSect;

	PgbrBase.iTopClass = ClassD.iTopFlgClass;
	PgbrBase.iBotClass = ClassD.iBotFlgClass;
	PgbrBase.iWebClass = ClassD.iWebClass;
	PgbrBase.iSectClass = ClassD.iClassSect;
	PgbrBase.bEquiClass2 = bEquivalentClass2;

	//double dFx=0., dFy=0., dFz=0., dMx=0., dMy=0., dMz=0.;
	double dNc = 0.0;
	double dNc_el = 0.0;
	double dNc_f = 0.0;
	double dMpl_Rd = 0.0;
	double dMel_Rd = 0.0;
	double dM_Rd = 0.0;
	double aForceGd[6] = { 0.0 }; // Girder
	double aForce[6] = { 0.0 };   // Composite에만 
	aForceGd[0] = PgbrGd.dFx;
	aForceGd[4] = PgbrGd.dMy;
	aForceGd[5] = PgbrGd.dMz;
	aForce[0] = PgbrBase.dFx;
	aForce[4] = PgbrBase.dMy;
	aForce[5] = PgbrBase.dMz;
	double dMa_Ed = PgbrBase.dMa_Ed;
	double dMc_Ed = PgbrBase.dMy;
	double dM_Ed_c = dMa_Ed + dMc_Ed;

	double dPlasticNA = 0.0;
	double dPlasticNAEff = 0.0;
	BOOL bPositive = dMc_Ed >= 0.0 ? TRUE : FALSE;
	dPlasticNA = param.dPlasticNA; //GetPlasticNA();
	//Get_PlasticNA4GenSect(bPosiI, bPositive, MatlElem, SectBaseD, arRbarPosi, GLineData, GLineDgn, dPlasticNA);
	// General Section인 경우는 Effective class 2 web을 적용하기 힘들다. 
	// EN 1993-1-1, 6.2.2.4 Effective properties of cross sections with class 3 webs and class1 or 2 flanges, 48p
	//if(bEquivalentClass2)
	//    Get_PlasticNAEff(bPosiI, bPositive, MatlElem, SectData, arRbarPosi, dPlasticNAEff, dEffeLen, dHoleLen);

	double dHt = Get_Height4Gen(param);
	double dXpl = !bEquivalentClass2 ? dHt - dPlasticNA : dHt - dPlasticNAEff;
	double dXplOverh = dHt == 0.0 ? 0.0 : dXpl / dHt; // Ht : 전체높이.
	BOOL bNonLinear = FALSE;
	if ( dXplOverh > 0.4 ) bNonLinear = TRUE;
	//
	// 0=Plastic resistance moment 
	// 1=Non-linear resistance to bending
	// 2=Elastic resistance to bending.  

	// 
	Get_PlasticMoment4GenSect(bPositive, param, param.GLineDgn, dPlasticNA, dMpl_Rd, PgbrDetail);
	// dNc_f.
	if ( bPositive ) Get_Nc_fofSlab(param, dfcd, dXpl, dHt, dNc_f);

	Get_ElasticResistance4GenSect(bPositive, aForceGd, aForce, param, PgbrBase, PgbrDetail);

	// 6.2.1.2(2)  
	double dBeta = 1.0;
	BOOL bS420orS460 = IsMaterialS420orS460(dfy);
	dBeta = 1.0;
	if ( bPositive )
	{
		if ( bS420orS460 )
		{
			if ( !Get_BetaReductionFactor(dXpl, dHt, dBeta) )
				dBeta = 1.0;
		}
	}

	PgbrBase.dMpl_Rd = dMpl_Rd;

	// For Shear Resistance
	double dMf_Rd = 0.0;
	Get_Mf_Rd_PlasticMoment4GenSect(bPositive, param, SectBaseD, dMf_Rd);
	PgbrBase.dMpl_Rd_s = dMpl_Rd;
	PgbrBase.dMf_Rd = dMf_Rd;

	// Non-linear Resistance
	if ( aForce[4] >= 0.0 && bS420orS460 && bNonLinear )
		Calc_NonLinearResistance(ElemK, bPosiI, PgbrBase);
	else
		PgbrBase.dM_Rd = iClassSect == 1 || iClassSect == 2 ? dBeta * PgbrBase.dMpl_Rd : PgbrBase.dMel_Rd;

	PgbrBase.dPlasticNA = dPlasticNA;
	PgbrBase.dPlasticNAEff = dPlasticNAEff;
	//PgbrBase.dEffeLen = dEffeLen;                    
	//PgbrBase.dHoleLen = dHoleLen;                    

	if ( iClassSect == 3 || iClassSect == 4 )
	{
		if ( PgbrDetail.nKminType == Kmin_Slab || PgbrDetail.nKminType == Kmin_Rbar ) dM_Ed_c = dMc_Ed;
	}
	PgbrBase.dRatio = PgbrBase.dM_Rd == 0.0 ? fabs(dM_Ed_c) / m_dZero : fabs(dM_Ed_c) / fabs(PgbrBase.dM_Rd);

	if ( PgbrBase.dRatio < 1.0 )
		PgbrBase.bOK = TRUE;
	else
		PgbrBase.bOK = FALSE;

	// For Detail
	PgbrBase.dh = dHt;
	PgbrBase.dXpl = dXpl;
	PgbrBase.dXplOverh = dXplOverh;
	PgbrBase.dAlpha = dAlpha;
	PgbrBase.dCompfsdAsidsi = 0.0; //dCompfsdAsidsi;
	PgbrBase.dTensfsdAsidsi = 0.0; //dTensfsdAsidsi;
	PgbrBase.dBeta = dBeta;
	//PgbrBase.dStrStlTop = dStrStlTop;
	//PgbrBase.dStrComTop = dStrComTop;
	//PgbrBase.dStrStlBot = dStrStlBot;
	//PgbrBase.dStrComBot = dStrComBot;  
	//PgbrBase.dSIGa_top = dSIGa_top;
	//PgbrBase.dSIGa_bot = dSIGa_bot;  
	//PgbrBase.dk_min = dk_min;     
	PgbrBase.dNc_f = dNc_f;
	//PgbrBase.dNc_el = dNc_el;

	if ( IsPrintMode() )
	{
		PgbrDetail.GLineDgn = param.GLineDgn;
		PgbrDetail.StressInfo4Class = param.DLineCalcD;
		PgbrDetail.dMpl_Rd_cal = dMpl_Rd;
	}

	return TRUE;
}
BOOL CDgnPlateGirder::Calc_LongitudinalShearResistance4SLS(T_PLATE_GIRDER_CALC_D& param, double dks, T_PLSS_BASE& PlssGd, T_PLSS_BASE& PlssBase, T_PLSS_DETAIL& PlssDetail)
{
	T_ELEM_K ElemK = param.ElemK.first;
	//For construction reasons, it is not normally planned to change the number of
	//studs per unit length continuously. The bridge total length is therefore divided
	//into n segments of length l,i i∈[1,n]. A number N,i∈[1,n] of studs is then
	//arranged in each one (constant density per segment).
	T_PGLS_BASE PglsBase;
	PglsBase.bChk = PlssBase.bChk;
	PglsBase.ElemK = PlssBase.ElemK;
	PglsBase.nPosi = PlssBase.nPosi;
	PglsBase.LcomK = PlssBase.LcomK;
	PglsBase.OrgLcomK = PlssBase.OrgLcomK;
	PglsBase.iMax = PlssBase.iMax;
	PglsBase.dFx = PlssBase.aForce[0];
	PglsBase.dFz = PlssBase.aForce[2];
	PglsBase.dMy = PlssBase.aForce[4];
	PglsBase.dMz = PlssBase.aForce[5];

	T_PGLS_BASE PglsGd;
	PglsGd.dFx = PlssGd.aForce[0];
	PglsGd.dFz = PlssGd.aForce[2];
	PglsGd.dMy = PlssGd.aForce[4];
	PglsGd.dMz = PlssGd.aForce[5];

	T_PGLS_DETAIL PglsDetail;
	BOOL bIsGenSect = param.bIsGenSect;
	if ( !bIsGenSect ) { Calc_LongitudinalShearResistance(TRUE, param, dks, PglsGd, PglsBase, PglsDetail); }
	else { Calc_LongitudinalShearResistance4GenSect(TRUE, param, dks, PglsGd, PglsBase, PglsDetail); }

	// Modify by GAY. PMS:4216. ('12.05.18). 데이터 할당 누락부분 수정.
	PlssBase.iSectClass = PglsBase.iSectClass;
	PlssBase.dAlpha = PglsBase.dAlpha;
	PlssBase.dNc = PglsBase.dNc;
	PlssBase.dNc_el = PglsBase.dNc_el;
	PlssBase.dNc_f = PglsBase.dNc_f;
	PlssBase.dMa_Ed = PglsBase.dMa_Ed;
	PlssBase.dMc_Ed = PglsBase.dMc_Ed;
	PlssBase.dMpl_Rd = PglsBase.dMpl_Rd;
	PlssBase.dMel_Rd = PglsBase.dMel_Rd;
	PlssBase.dV_L_Ed = PglsBase.dV_L_Ed;
	PlssBase.dv_L_Ed = PglsBase.dv_L_Ed;
	PlssBase.dv_L_Ed2 = PglsBase.dv_L_Ed2;
	PlssBase.dP_Rd1 = PglsBase.dP_Rd1;
	PlssBase.dP_Rd2 = PglsBase.dP_Rd2;
	PlssBase.dP_Rd = PglsBase.dP_Rd;
	PlssBase.dP_Rd_SLS = PglsDetail.dP_Rd_SLS;
	PlssBase.iCheck_P_Rd = PglsBase.iCheck_P_Rd;
	PlssBase.dv_L_Rd = PglsBase.dv_L_Rd;
	PlssBase.dv_Ed = PglsBase.dv_Ed;
	PlssBase.dv_Ed_lim = PglsBase.dv_Ed_lim;
	PlssBase.dRatio = fabs(PglsBase.dv_L_Rd) < m_dZero ? fabs(PglsBase.dv_L_Ed / m_dZero) : fabs(PglsBase.dv_L_Ed / PglsBase.dv_L_Rd);
	PlssBase.dAzI = PglsBase.dAzI; // Q/Iy  
	PlssBase.bOK = fabs(PglsBase.dv_L_Ed) > fabs(PglsBase.dv_L_Rd) ? FALSE : TRUE;

	PlssDetail.dfu = PglsDetail.dfu;
	PlssDetail.iRat_sc_type = PglsDetail.iRat_sc_type;
	PlssDetail.iNum = PglsDetail.iNum;
	PlssDetail.dDsc = PglsDetail.dDsc;
	PlssDetail.dhsc = PglsDetail.dhsc;
	PlssDetail.dSpace = PglsDetail.dSpace;
	PlssDetail.dLv = PglsDetail.dLv;
	PlssDetail.dmuy_c = PglsDetail.dmuy_c;

	return TRUE;
}
BOOL CDgnPlateGirder::Calc_StressCheck4SLS(T_PLATE_GIRDER_CALC_D& param, double dk1, double dk2, double dk3, const _DGN_FORC_CRC& ForceConst, T_PGBS_BASE& PgbsBase, T_PGBS_DETAIL& PgbsDetail)
{
	BOOL bPosiI = param.bPosiI;
	// 1. Material.
	T_MATD_D&  MatdD = param.MatlElem.MatdD;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dEs = MatdD.Data1.Analysis.Elast;
	double dEcm = param.MatlElem.dEcm;
	double dfck = param.MatlElem.dfck;
	double dfsk = param.MatlElem.MatdD.MainRebarData.B_fy;
	double dgam_M_ser = m_dGamma_M_ser;

	// 2. Section.
	T_SECT_D& SectD = param.SectElem.SectD;
	int iSectShapeType = Get_SectShapeType(SectD);
	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
	const T_SECT_SECTBASE_D& SectBase2D = param.SectBaseD;
	T_SECT_STIFFNESS NonConSect, RebarSect, OrgSect;
	NonConSect.Initialize(); RebarSect.Initialize(); OrgSect.Initialize();
	GetPureSectStiffD(param.SectElem, bPosiI, OrgSect);
	GetNonComSectStiffD(param.SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(param.SectElem, bPosiI, RebarSect);

	// Dimension
	double dHt = iSectShapeType == D_SECT_TYPE_COMPO_G ? Get_Height4Gen(param) : m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);
 	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
 	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
 	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
 	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
 	double dTw_t = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
	double dTF1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dTF2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
 	double dHw_in = m_pDgnSectUtil->Get_Size_Inclind_hw(&SectD, bPosiI);

	if ( iSectShapeType == D_SECT_TYPE_COMPO_G )
	{
		Get_ThicknessOfFlange4GenSect(param.GLineDgn, dTF1, dTF2);
	}

	double dfyt = Get_fy4Thick(dfy, dfy1, dTF1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTF2);
	double adfy[4] = { dfyt, dfyt, dfyb, dfyb };

	int nPosI = bPosiI ? 0 : 1;

	// 3. Force
	// Before
	double dFx_a = PgbsBase.ChkGirder.aBeforeForce[0];
	double dFz_a = PgbsBase.ChkGirder.aBeforeForce[2];
	double dMy_a = PgbsBase.ChkGirder.aBeforeForce[4];
	double dMz_a = PgbsBase.ChkGirder.aBeforeForce[5];
	// After
	double dFx = PgbsBase.ChkGirder.aForce[0] - ForceConst.dFxx[nPosI];
	double dFz = PgbsBase.ChkGirder.aForce[2];
	double dMy = PgbsBase.ChkGirder.aForce[4] - ForceConst.dMuy[nPosI];
	double dMz = PgbsBase.ChkGirder.aForce[5] - ForceConst.dMuz[nPosI];
	//
	double dV_Ed = dFz_a + dFz;
	BOOL bPositiveMy = dMy > -m_dZero;

	// 4. Girder Stress Check
	T_SECT_STIFFNESS GirderStiffD;  GirderStiffD.Initialize();
	Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiffD);
	T_SECT_STIFFNESS CalcStressStiff = bPositiveMy ? RebarSect : NonConSect;

	double dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4;
	double dx1_a, dy1_a, dx2_a, dy2_a, dx3_a, dy3_a, dx4_a, dy4_a;
	double dArea, dAsy, dAsz, dRxx, dRyy, dRzz, dCyp, dCym, dCzp, dCzm, dQyb, dQzb, dWArea;
	double dArea_a, dAsy_a, dAsz_a, dRxx_a, dRyy_a, dRzz_a, dCyp_a, dCym_a, dCzp_a, dCzm_a, dQyb_a, dQzb_a, dWArea_a;

	GirderStiffD.GetStiff(dArea_a, dAsy_a, dAsz_a, dRxx_a, dRyy_a, dRzz_a, dCyp_a, dCym_a, dCzp_a, dCzm_a, dQyb_a, dQzb_a, dWArea_a);
	GirderStiffD.GetPosition(dx1_a, dy1_a, dx2_a, dy2_a, dx3_a, dy3_a, dx4_a, dy4_a);
	CalcStressStiff.GetStiff(dArea, dAsy, dAsz, dRxx, dRyy, dRzz, dCyp, dCym, dCzp, dCzm, dQyb, dQzb, dWArea);
	CalcStressStiff.GetPosition(dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4);

	double aStressG[4] = { 0.0 }, aStressC[4] = { 0.0 };
	double aStress4[4] = { 0.0 }, aStressRat[4] = { 0.0 };

	aStressG[0] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dRyy_a, dRzz_a, dx1_a, dy1_a);
	aStressG[1] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dRyy_a, dRzz_a, dx2_a, dy2_a);
	aStressG[2] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dRyy_a, dRzz_a, dx3_a, dy3_a);
	aStressG[3] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dRyy_a, dRzz_a, dx4_a, dy4_a);

	for ( int i = 0; i < 4; i++ ) { aStress4[i] = PgbsBase.ChkGirder.dStress[i]; }

	double aSigma_ser_lt[4] = { 0.0, };
	for ( int i = 0; i < 4; i++ )
	{
		aSigma_ser_lt[i] = fabs(dgam_M_ser) < m_dZero ? 0.0 : adfy[i] / dgam_M_ser;
	}

	double dMaxStressRat = 0.0;
	double dMaxStress = 0.0;
	int iMaxStressRatIdx = 0;

	for ( int i = 0; i < 4; i++ )
	{
		aStressRat[i] = fabs(aSigma_ser_lt[i]) < m_dZero ? 0.0 : fabs(aStress4[i]) / aSigma_ser_lt[i];
		if ( i == 0 )
		{
			iMaxStressRatIdx = 0;
			dMaxStress = aStress4[i];
			dMaxStressRat = aStressRat[i];
		}
		else
		{
			if ( dMaxStressRat < aStressRat[i] )
			{
				iMaxStressRatIdx = i;
				dMaxStress = aStress4[i];
				dMaxStressRat = aStressRat[i];
			}
		}
	}

	BOOL bChkStress_gr = dMaxStressRat > 1.0 ? FALSE : TRUE;

	// Check Shear stress, 합성되기전 거더의 도심에서 계산하자. 
	double dbw = 0.0, dQ = 0.0;
	double dAv_web = 0.0;
	double dtau_Ed_ser = 0.0;

	BOOL bAveShearStress = TRUE;
	if ( bAveShearStress ) // 평균 전단 응력. 
	{
		if ( iSectShapeType == D_SECT_TYPE_COMPO_I || iSectShapeType == D_SECT_TYPE_COMPO_B || iSectShapeType == D_SECT_TYPE_COMPO_TUB )
		{
			dAv_web = dHw_in * dTw_t;
			dtau_Ed_ser = dAv_web < m_dZero ? 0.0 : dV_Ed / dAv_web; //Get_ShearStress(dFz, dQ, RebarSect.Ryy, dbw);    
		}
		else if ( iSectShapeType == D_SECT_TYPE_COMPO_G )
		{
			Get_ThicknessOfWeb4GenSect(param.GLineDgn, dbw, dAv_web);
			dtau_Ed_ser = dAv_web < m_dZero ? 0.0 : dV_Ed / dAv_web; //Get_ShearStress(dFz, dQ, RebarSect.Ryy, dbw);
		}
		else ASSERT(0);
	}
	else // 도심의 전단응력으로 적용할 때 사용. 
	{
		ASSERT(0);
	}

	double dTau_ser_lt = fabs(dgam_M_ser) < m_dZero ? 0.0 : dfy / (sqrt(3.0)*dgam_M_ser);
	BOOL bChkShearStress = fabs(dtau_Ed_ser) > dTau_ser_lt ? FALSE : TRUE;

	double dStress_comb2 = pow(dMaxStress, 2) + 3.0*pow(dtau_Ed_ser, 2);
	double dStress_comb = sqrt(dStress_comb2);
	BOOL bChkStress_comb = dStress_comb > aSigma_ser_lt[iMaxStressRatIdx] ? FALSE : TRUE;
	BOOL bOK_gr = (bChkStress_gr && bChkShearStress && bChkStress_comb);

	memcpy(PgbsBase.ChkGirder.dRatio, aStressRat, sizeof(PgbsBase.ChkGirder.dRatio));

	PgbsBase.ChkGirder.dMaxStress = -1.0*dMaxStress;
	PgbsBase.ChkGirder.dMaxRatio = dMaxStressRat;
	PgbsBase.ChkGirder.iIndex_cr = iMaxStressRatIdx;
	PgbsBase.ChkGirder.dAlwStress = aSigma_ser_lt[iMaxStressRatIdx];
	PgbsBase.ChkGirder.bChk_Stress = bChkStress_gr;
	PgbsBase.ChkGirder.dtau_w = fabs(dtau_Ed_ser);
	PgbsBase.ChkGirder.dAlw_tau = dTau_ser_lt;
	PgbsBase.ChkGirder.bChk_tau = bChkShearStress;
	PgbsBase.ChkGirder.dStress_cb = dStress_comb;
	PgbsBase.ChkGirder.dAlw_stress_cb = aSigma_ser_lt[iMaxStressRatIdx];
	PgbsBase.ChkGirder.bChk_Stress_cb = bChkStress_comb;
	PgbsBase.ChkGirder.bOK = bOK_gr;

	//////////////////////////////////////////////////////////////////////////
	// 5. Slab stress Check
	double dyGap = CalcStressStiff.Cym - OrgSect.Cym; // 도심의 차이 
	double dn = dEcm < m_dZero ? 0.0 : dEs / dEcm;
	double aSlabStress4[4] = { 0.0 }, aSlabStressRat[4] = { 0.0 };
	double dx1_s, dy1_s, dx2_s, dy2_s, dx3_s, dy3_s, dx4_s, dy4_s;

	if ( iSectShapeType == D_SECT_TYPE_COMPO_I || iSectShapeType == D_SECT_TYPE_COMPO_B || iSectShapeType == D_SECT_TYPE_COMPO_TUB )
	{
		dx1_s = -0.5*dBc - dyGap, dy1_s = dHt - CalcStressStiff.Czm;
		dx2_s = 0.5*dBc - dyGap, dy2_s = dy1_s;
		dx3_s = dx2_s, dy3_s = dH + dHh - CalcStressStiff.Czm;
		dx4_s = dx1_s, dy4_s = dy3_s;
	}
	else if ( iSectShapeType == D_SECT_TYPE_COMPO_G )
	{
		double aYcr_sb[4] = { 0.0 }, aZcr_sb[4] = { 0.0 };
		double dZbar_sb = Get_ZcentroidofSlab(param);

		for ( int i = 0; i < 4; i++ )
		{
			aYcr_sb[i] = param.aY_sb[nPosI][i];
			aZcr_sb[i] = param.aZ_sb[nPosI][i];
		}

		dx1_s = aYcr_sb[0] - dyGap;
		dx2_s = aYcr_sb[1] - dyGap;
		dx3_s = aYcr_sb[2] - dyGap;
		dx4_s = aYcr_sb[3] - dyGap;
		//
		double dZgap_cent_sb = dZbar_sb - CalcStressStiff.Czm;
		dy1_s = aZcr_sb[0] + dZgap_cent_sb;
		dy2_s = aZcr_sb[1] + dZgap_cent_sb;
		dy3_s = aZcr_sb[2] + dZgap_cent_sb;
		dy4_s = aZcr_sb[3] + dZgap_cent_sb;
	}
	else ASSERT(0);

	//if(bPositiveMy) // 균열일경우는 계산하지 않는다. 
	for ( int i = 0; i < 4; ++i )
	{
		aSlabStress4[i] = PgbsDetail.StrD.Slab.dStr[i];
		if ( aSlabStress4[i] > 0.0 ) aSlabStress4[i] = 0.0;
	}


	double dSigma_c_lt = 0.0;
	int iLcomSLSType = PgbsBase.ChkSlab.LcomType;

	if ( iLcomSLSType == 1 ) { dSigma_c_lt = dk2 * dfck; } // 1=Quasi-permanent
	else if ( iLcomSLSType == 3 ) { dSigma_c_lt = dk1 * dfck; } // 3=Characteristic
	//else ASSERT(0); 계산은 다하자.

	double dMaxSlabStressRat = 0.0;
	double dMaxSlabStress = 0.0;
	int iMaxSlabStressRatIdx = 0;

	for ( int i = 0; i < 4; i++ )
	{
		aSlabStressRat[i] = fabs(dSigma_c_lt) < m_dZero ? 0.0 : fabs(aSlabStress4[i]) / dSigma_c_lt;
		if ( i == 0 )
		{
			iMaxSlabStressRatIdx = 0;
			dMaxSlabStress = aSlabStress4[i];
			dMaxSlabStressRat = aSlabStressRat[i];
		}
		else
		{
			if ( dMaxSlabStressRat < aSlabStressRat[i] )
			{
				iMaxSlabStressRatIdx = i;
				dMaxSlabStress = aSlabStress4[i];
				dMaxSlabStressRat = aSlabStressRat[i];
			}
		}
	}
	BOOL bChkSlabStress = dMaxSlabStressRat > 1.0 ? FALSE : TRUE;

	PgbsBase.ChkSlab.dStress_sb = -1.0*dMaxSlabStress;
	PgbsBase.ChkSlab.dAlwStress_sb = dSigma_c_lt;
	PgbsBase.ChkSlab.dMaxRatio_sb = dMaxSlabStressRat;
	PgbsBase.ChkSlab.bChk_Stress_sb = bChkSlabStress;

	//////////////////////////////////////////////////////////////////////////
	// 6. Rebar Stress
	double dMaxStress_bar = 0.0;
	double dSigma_s_lt = dk3 * dfsk;

	double dy_ro = 0.0, dz_ro = 0.0;
	// CRCDataCtrl.cpp 참조. Get_CpgRbar(T_ELEM_K ElemK, BOOL bPosI, BOOL bCrack, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi)

	int iRebarSize = param.arRbarPosi.GetSize();
	for ( int i = 0; i < iRebarSize; i++ )
	{
		const _DGN_RBAR_CRC& Rbar = param.arRbarPosi[i];
		if ( iSectShapeType == D_SECT_TYPE_COMPO_I || iSectShapeType == D_SECT_TYPE_COMPO_B || iSectShapeType == D_SECT_TYPE_COMPO_TUB )
		{
			dy_ro = Rbar.dy - dyGap; // Slab의 도심기준으로 되어 있음. 
			dz_ro = (dHt - 0.5*dtc + Rbar.dz) - CalcStressStiff.Czm; //Rbar.dz - CalcStressStiff.Czm;  // Rbar.dz=Slab 도심(dtc/2) 에서 떨어진 거리로
		}
		else if ( iSectShapeType == D_SECT_TYPE_COMPO_G )
		{
			int iGenPart = SectBase2D.aGeneralPart.GetSize();
			dy_ro = Rbar.dy - dyGap;
			dz_ro = Rbar.dz - CalcStressStiff.Czm;
			//dz_ro = bPosiI ? m_dZbar_sb[0]-m_dCzm_sb[0] : m_dZbar_sb[1]-m_dCzm_sb[1];
			//dz_ro += Rbar.dz;
		}

		double dStress_bar = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dRyy, dRzz, dy_ro, dz_ro);
		dStress_bar = dStress_bar < 0.0 ? max(dStress_bar, -dfsk) : min(dStress_bar, dfsk);
		if ( i == 0 )
		{
			dMaxStress_bar = dStress_bar;
		}
		else if ( fabs(dMaxStress_bar) < fabs(dStress_bar) )
		{
			dMaxStress_bar = dStress_bar;
		}
	}

	double dMaxRebarStressRat = fabs(dSigma_s_lt) < m_dZero ? 0.0 : fabs(dMaxStress_bar) / dSigma_s_lt;
	BOOL bChkRebarStress = dMaxRebarStressRat > 1.0 ? FALSE : TRUE;

	// Save Result Data
	PgbsBase.ChkBar.dStress_bar = dMaxStress_bar;
	PgbsBase.ChkBar.dAlwStress_bar = dSigma_s_lt;
	PgbsBase.ChkBar.dMaxRatio_bar = dMaxRebarStressRat;
	PgbsBase.ChkBar.bChk_Stress_bar = bChkRebarStress;

	return TRUE;
}


BOOL CDgnPlateGirder::Calc_FatigueResistance(T_PLATE_GIRDER_CALC_D& param, double dt_Ld, T_PGRF_BASE& PgrfBase)
{
	T_ELEM_K ElemK = param.ElemK.first;
	// 1. Material.
	T_MATD_D& MatdD = param.MatlElem.MatdD;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfck = param.MatlElem.dfck;
	double dfcm = param.MatlElem.dfcm;
	double dfctm = param.MatlElem.dfctm;
	double dEcm = param.MatlElem.dEcm;
	double dgam_M0 = m_dGamma_M0;
	double dgam_Ff = m_dGamma_Ff;
	double dgam_Mfs = m_dGamma_Mfs;

	CPG_FRDT_POSI& FrdtPosi = param.FrdtPosi;

	// 2. Section.
	int iClassSect = param.ClassBase.iClassSect;

	// 3. Force
	double dMy = PgrfBase.dMy;
	double dV_Ed = PgrfBase.dFz;

	// Get Shear Connector Input Data
	// EN 1994-2, 6.6 Shear connection, 46page
	// Shear Connector.
	CPG_SCON_POSI& SconPosi = param.SconElem.SconPosi[param.iPos];
	int iNum = SconPosi.iNum;
	double dhsc = SconPosi.dhsc;
	double dfu_sc = SconPosi.dfu;
	double dDia_sc = SconPosi.dDia;
	double dSt_sc = SconPosi.dSt;
	double dSpace = SconPosi.dSpace;
	double dAlpha = 0.0;

	double dv_L_Ed = param.PglsBase.dv_L_Ed;
	double dLamda_v1 = 1.55; // // 6.8.6.2(4), 76page
	double dLamda_v2 = FrdtPosi.dLamda_v2;
	double dLamda_v3 = 0.0;
	double dLamda_v4 = FrdtPosi.dLamda_v4;
	double dDel_Tau_E_2 = 0.0;
	double dDel_Tau_c = 0.0;

	// The range of shear force per connector
	double dFsc = (iNum == 0) ? 0.0 : dv_L_Ed * dSpace / ((double)iNum);
	double dA_sc = m_dPi * pow(dDia_sc, 2) / 4.0;
	double dDel_Tau = dA_sc < m_dZero ? 0.0 : dFsc / dA_sc;

	// EN 1994-2, 6.8.6.2 Shear connection, 76page
	// EN 1993-2, 9.5.2 Damage equivalence factors λ for road bridges, 42page
	double dQ0 = 480.0; // 480 kN
	double dN0 = 0.5*10E6;
	double dN_Obs = 0.5*10E6; // The UK's National Annex to EN 1991-2 may modify this value
	double dSigma_niQi5 = 8.051*10E18;
	double dSigma_ni = 1.0*10E6;
	double dQm1 = pow(dSigma_niQi5 / dSigma_ni, 1. / 5.); //kN  

	//dLamda_v2 = dQ0==0.0 || dN0==0.0 ? 0.0 : dQm1/dQ0*pow((dN_Obs/dN0),1./5.);

	dLamda_v3 = pow(dt_Ld / 100.0, 1. / 8.); // EN 1994-2 §6.8.6.2(4) we should consider 1/8 instead 1/5

	//dLamda_v4 = 1.0; // ?? EN 1993-2, 9.5.2(6), 44page

	double dLamda_v = dLamda_v1 * dLamda_v2*dLamda_v3*dLamda_v4;
	// 
	dDel_Tau_E_2 = dLamda_v * dDel_Tau;
	// EN 1994-2, 6.8.3(3), 72page.
	dDel_Tau_c = 90.0; // is the reference value at Nc=2*10^6 cycles with Δτ_c equal to 90 N/mm^2
	//
	double dRatio = fabs(dgam_Mfs*dDel_Tau_c) < m_dZero ? dgam_Ff * dDel_Tau_E_2 / m_dZero : dgam_Ff * dDel_Tau_E_2 / (dDel_Tau_c / dgam_Mfs);
	dRatio = fabs(dRatio);

	// Save Result
	PgrfBase.bOK = dRatio > 1.0 ? FALSE : TRUE;
	PgrfBase.dDel_Tau = dDel_Tau;      // Δτ                [Unit=Stress]
	PgrfBase.dLamda_v = dLamda_v;      // λv                 [Unit=None]
	PgrfBase.dLamda_v1 = dLamda_v1;     // λv1                [Unit=None]
	PgrfBase.dLamda_v2 = dLamda_v2;     // λv2                [Unit=None]
	PgrfBase.dLamda_v3 = dLamda_v3;     // λv3                [Unit=None]
	PgrfBase.dLamda_v4 = dLamda_v4;     // λv4                [Unit=None]
	PgrfBase.dDel_Tau_E_2 = dDel_Tau_E_2;  // Δτ_E,2            [Unit=Stress]
	PgrfBase.dDel_Tau_c = dDel_Tau_c;    // Δτ_c              [Unit=Stress]
	PgrfBase.dGamma_Ff = dgam_Ff;       //                     [Unit=None]
	PgrfBase.dGamma_Mfs = dgam_Mfs;      //                     [Unit=None]
	PgrfBase.dRatio = dRatio;        // rFf*Δτ_E,2/(Δτ_c/rMf,s)  [Unit=None]  
	PgrfBase.dv_L_Ed = dv_L_Ed;
	PgrfBase.dFsc = dFsc;
	PgrfBase.dAsc = dA_sc;

	return TRUE;
}
BOOL CDgnPlateGirder::Calc_LongitudinalShearResistance(BOOL bSLS, T_PLATE_GIRDER_CALC_D& param, double dks, T_PGLS_BASE& PglsGd, T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail)
{
	T_CLAS_BASE& ClassD = param.ClassBase;
	T_ELEM_K ElemK = param.ElemK.first;
	// 1. Material.
	T_MATD_D& MatdD = param.MatlElem.MatdD;
	double dEs = MatdD.Data1.Analysis.Elast;
	double dfck = param.MatlElem.dfck;
	double dEcm = param.MatlElem.dEcm;
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dgam_M1 = m_dGamma_M1;
	double dgam_V = m_dGamma_V;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;

	// 2. Section.
	int iClassSect = ClassD.iClassSect;
	T_SECT_D& SectD = param.SectElem.SectD;
	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);

	T_SECT_STIFFNESS GirderStiff; GirderStiff.Initialize();
	Get_GirderStiffnessofCompo(param.bPosiI, SectD, GirderStiff);

	// Dimension
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, param.bPosiI);
	double dTF1 = m_pDgnSectUtil->Get_Size_tft(&SectD, param.bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&SectD, param.bPosiI);

	// 3. Force
	const T_PGBR_BASE& PgbrBase = param.PgbrBase;
	double dNc = 0.0, dMc_Ed = 0.0, dM_Rd = 0.0, dMb_Rd = 0.0, dN_Ed = 0.0;
	double dMy = PglsBase.dMy;
	double dMy_a = PglsGd.dMy;
	double dV_Ed = PglsBase.dFz;
	double dMa_Ed = PglsBase.dMa_Ed;
	double dMpl_Rd = PgbrBase.dMpl_Rd; // PGBR에서 결정된 값.
	double dMel_Rd = PgbrBase.dMel_Rd; // PGBR에서 결정된 값.
	double dNc_el = PgbrBase.dNc_el;
	double dNc_f = PgbrBase.dNc_f;
	double dM_Ed = dMy_a + dMy;

	// 4. Get Shear Connector Input Data
	// EN 1994-2, 6.6 Shear connection, 46page
	int iRat_sc_type = 0;

	auto SconPosi = param.SconElem.SconPosi[param.iPos];
	int iNum = SconPosi.iNum;
	double dhsc = SconPosi.dhsc;
	double dfu_sc = SconPosi.dfu;
	double dDia_sc = SconPosi.dDia;
	double dSt_sc = SconPosi.dSt;
	double dSpace = SconPosi.dSpace;
	double dAlpha = 0.0;
	double dRat_sc = dDia_sc < m_dZero ? 0.0 : dhsc / dDia_sc;

	// 5. Calc. Shear Strength of Shear connector
	// 6.6.3.1(1), 52p, EN 1994-2  
	if ( 3.0 - m_dZero < dRat_sc && dRat_sc < 4.0 + m_dZero )
	{
		iRat_sc_type = 1;
		dAlpha = 0.2*(dRat_sc + 1.0);
	}
	else if ( dRat_sc > 4.0 )
	{
		iRat_sc_type = 2;
		dAlpha = 1.0;
	}
	else
	{
		iRat_sc_type = 3;
		if ( dAlpha > 1.0 ) { dAlpha = 1.0; }
		else { dAlpha = 0.2*(dRat_sc + 1.0); }
	}

	// Modify, Jaeoh. [4/28/2009] P_Rd 계산방법 수정 , 이혜연 사원님 요청 
	int iCheck_P_Rd = 0;
	double dP_Rd1 = 0.8*dfu_sc*m_dPi*pow(dDia_sc, 2) / 4.0 / dgam_V;
	double dP_Rd2 = fabs(dgam_V) < m_dZero ? 0.0 : 0.29*dAlpha*pow(dDia_sc, 2)*sqrt(dfck*dEcm) / dgam_V;
	double dP_Rd = 0.0;
	double dP_Rd_SLS = 0.0;

	if ( dP_Rd1 < dP_Rd2 )  // EN 1994. 6.6.3.1(1), (6.18), (6.19)
	{
		dP_Rd = dP_Rd1;
		iCheck_P_Rd = 0;
	}
	else
	{
		dP_Rd = dP_Rd2;
		iCheck_P_Rd = 1;
	}
	if ( bSLS ) dP_Rd_SLS = dks * dP_Rd;

	// 6. Calc. Shear Force & Shear Flow
	double dAz_Slab = 0.0, dAz_Hunch = 0.0, dZbar = 0.0, dIyy_Org = 0.0; // 철근 고려 안한. Original Section Stiffness.  
	double dErat = dEcm < m_dZero ? 0.0 : dEs / dEcm;   // Es/Ec, Section에 있는 값.

	if ( !bTapered )
	{
		dZbar = SectD.SectAfter.Stiffness.Czm;
		dIyy_Org = SectD.SectAfter.Stiffness.Ryy;
	}
	else
	{
		dZbar = param.bPosiI ? SectD.SectAfter.SectI.Stiffness.Czm : SectD.SectAfter.SectJ.Stiffness.Czm;
		dIyy_Org = param.bPosiI ? SectD.SectAfter.SectI.Stiffness.Ryy : SectD.SectAfter.SectJ.Stiffness.Ryy;
	}

	double dHg = GirderStiff.Czm + GirderStiff.Czp;
	if ( dHg - m_dZero > dZbar )
	{
		dAz_Slab = dErat < m_dZero ? 0.0 : (dBc*dtc / dErat)*((dHt - 0.5*dtc) - dZbar);
		dAz_Hunch = dErat < m_dZero ? 0.0 : (dB1*dHh / dErat)*((dHt - dtc - 0.5*dHh) - dZbar);
	}
	else
	{
		dAz_Slab = dErat < m_dZero ? 0.0 : (GirderStiff.Area)*(dZbar - GirderStiff.Czm);
		dAz_Hunch = 0.0;
	}

	double dv_L_Rd = 0.0;
	if ( bSLS ) { dv_L_Rd = dSpace < m_dZero ? 0.0 : dP_Rd_SLS * ((double)iNum) / dSpace; }
	else { dv_L_Rd = dSpace < m_dZero ? 0.0 : dP_Rd * ((double)iNum) / dSpace; }

	// Modification, [12/5/2011] 이대근 bj님 요청 
	// 정모멘트이면서 Class가 1,2이고 dM_Ed-dMel_Rd>0 인 단면은 dV_L_Ed = (dNc_f-dNc_el)*(fabs(dM_Ed)-dMel_Rd)/(dMpl_Rd-dMel_Rd) 사용
	// 정모멘트 이면서 위 경우가 아니거나, 부모멘트인 경우는 V_L_Ed = V_Ed
	double dV_L_Ed = 0.0; // Shear Force
	double dv_L_Ed = 0.0; // Shear flow
	double de_d = 0.0; // e_d = 0, b_eff = Bc defined in Section Data, 07.06.** 답변메일 내용으로 반영 
	double dLength_v = de_d + dBc; // EN 1994-2, 6.6.2.3, Figure 6.12, 49p~51p 참고.
	double dAzI = dIyy_Org < m_dZero ? 0.0 : (dAz_Slab + dAz_Hunch) / dIyy_Org;
	if ( (iClassSect == 1 || iClassSect == 2) && dM_Ed > -m_dZero && dM_Ed - dMel_Rd > 0.0 )
	{
		dV_L_Ed = fabs(dMpl_Rd - dMel_Rd) < m_dZero ? 0.0 : (dNc_f - dNc_el)*(fabs(dM_Ed) - dMel_Rd) / (dMpl_Rd - dMel_Rd);
		dv_L_Ed = fabs(dLength_v) < m_dZero ? 0.0 : dV_L_Ed / dLength_v;
	}
	else
	{
		dV_L_Ed = dV_Ed;
		dv_L_Ed = fabs(dV_Ed)*dAzI;
	}

	double dv_Ed = dv_L_Ed / (2.0*dtc);
	double dnu = 0.6*(1.0 - dfck / 250.0);
	double dTheta_f = m_dPi / 180.0*45.0; // 45로 가정
	double dv_Ed_lim = dnu * dfcd*sin(dTheta_f)*cos(dTheta_f);// Designers' guide to EN 1994-2, 128p

	// 7. Save Longitudinal Force Check Result Data
	PglsBase.iSectClass = ClassD.iClassSect;
	PglsBase.dNc = dNc;
	PglsBase.dNc_el = dNc_el;
	PglsBase.dNc_f = dNc_f;
	PglsBase.dMpl_Rd = dMpl_Rd;
	PglsBase.dMel_Rd = dMel_Rd;
	PglsBase.dV_L_Ed = dV_L_Ed;
	PglsBase.dv_L_Ed = dv_L_Ed;     // Slab로 계산된.    
	PglsBase.dP_Rd1 = dP_Rd1;
	PglsBase.dP_Rd2 = dP_Rd2;
	PglsBase.dP_Rd = dP_Rd;
	PglsBase.iCheck_P_Rd = iCheck_P_Rd;
	PglsBase.dv_L_Rd = dv_L_Rd;     // P_Rd*(Stud개수)/간격
	PglsBase.dv_Ed = dv_Ed;       // v_Ed(Slab의..)      
	PglsBase.dv_Ed_lim = dv_Ed_lim;   // v_Ed(Slab의..)      
	PglsBase.dRatio = fabs(dv_L_Rd) < m_dZero ? fabs(dv_L_Ed) / m_dZero : fabs(dv_L_Ed) / fabs(dv_L_Rd);
	PglsBase.bOK = PglsBase.dRatio < 1.0 + m_dZero ? TRUE : FALSE;
	PglsBase.dAlpha = dAlpha;
	PglsBase.dAzI = dAzI;
	PglsDetail.dmuy_c = dAzI * dIyy_Org;
	PglsDetail.dP_Rd_SLS = dP_Rd_SLS;

	if ( IsPrintMode() )
	{
		PglsDetail.dfu = dfu_sc;
		PglsDetail.iRat_sc_type = iRat_sc_type;
		PglsDetail.iNum = iNum;
		PglsDetail.dDsc = dDia_sc;
		PglsDetail.dhsc = dhsc;
		PglsDetail.dSpace = dSpace;
		PglsDetail.dLv = dLength_v;
	}

	return TRUE;
}
BOOL CDgnPlateGirder::Calc_TransverseForceResistance(T_PLATE_GIRDER_CALC_D& param, T_PRTF_BASE& PrtfBase, T_PRTF_DETAIL& PrtfDetail)
{
	T_CLAS_BASE& ClassD = param.ClassBase;
	T_ELEM_K ElemK = param.ElemK.first;
	// 1. Material.
	T_MATD_D& MatdD = param.MatlElem.MatdD;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dEs = MatdD.Data1.Analysis.Elast;
	double dgam_M0 = m_dGamma_M0;
	double dgam_M1 = m_dGamma_M1;
	CPG_BCDT_POSI& BcdtPos = param.BcdtPosi;

	// 2. Section.
	T_SECT_D& SectD = param.SectElem.SectD;
	T_SECT_STIFFNESS RebarSect; RebarSect.Initialize();
	BOOL bPosiI = param.bPosiI;
	GetRebarSectStiffD(param.SectElem, bPosiI, RebarSect);

	// Dimension
	bool bBoxSect = m_pDgnSectUtil->Is_StlBoxSect(&SectD);
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dTw_t = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTF1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dfyt = Get_fy4Thick(dfy, dfy1, dTF1);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfyd = fabs(dgam_M0) < m_dZero ? dfy : dfy / dgam_M0;
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;

	const T_SECT_STIFFNESS& EffeStlSect = param.tEffeStlSect;
	const T_SECT_STIFFNESS& EffeComSect = param.tEffeComSect;

	// 3. Force
	double dN_Ed = PrtfBase.dFx;
	double dMy_Ed = PrtfBase.dMy;
	double dMz_Ed = PrtfBase.dMz;
	double dF_Ed = PrtfBase.dF_Ed;

	// Before
	double dStl_Iyy = EffeStlSect.Ryy;
	double dStl_Czm = EffeStlSect.Czm;
	double dStl_Czp = EffeStlSect.Czp;
	double dStl_zDim = dStl_Czp + dStl_Czm;
	double dStl_Stop = dStl_Czp;
	// Composite
	double dIyy = EffeComSect.Ryy;
	double dIzz = EffeComSect.Rzz;
	double dCzm = EffeComSect.Czm;
	double dCzp = EffeComSect.Czp;
	double dzDim = dCzm + dCzp;
	double dStop = dH - dCzm;
	double dy1 = EffeComSect.dx1;   double dz1 = EffeComSect.dy1;
	double dy2 = EffeComSect.dx1;	  double dz2 = EffeComSect.dy1;
	double dy3 = EffeComSect.dx3;	  double dz3 = EffeComSect.dy3;
	double dy4 = EffeComSect.dx4;	  double dz4 = EffeComSect.dy4;

	double dWy_eff[4] = { 0.0, 0.0, 0.0, 0.0 }, dWz_eff[4] = { 0.0, 0.0, 0.0, 0.0 }; // 0=TL, 1=TR, 2=BR, 3=BL
	dWy_eff[0] = fabs(dy1) < m_dZero ? 0.0 : dIyy / dz1;  dWz_eff[0] = fabs(dz1) < m_dZero ? 0.0 : dIzz / dy1;
	dWy_eff[1] = fabs(dy2) < m_dZero ? 0.0 : dIyy / dz2;  dWz_eff[1] = fabs(dz2) < m_dZero ? 0.0 : dIzz / dy2;
	dWy_eff[2] = fabs(dy3) < m_dZero ? 0.0 : dIyy / dz3;  dWz_eff[2] = fabs(dz3) < m_dZero ? 0.0 : dIzz / dy3;
	dWy_eff[3] = fabs(dy4) < m_dZero ? 0.0 : dIyy / dz4;  dWz_eff[3] = fabs(dz4) < m_dZero ? 0.0 : dIzz / dy4;

	// 1993-1-5, (4.15), 21p
	int iEta1_max_posi = 0;
	double dAeff = param.PgbrDetail.EffCompositeFx.Area;
	double dey_N = RebarSect.Czm - param.PgbrDetail.EffCompositeFx.Czm;
	double dez_N = RebarSect.Cym - param.PgbrDetail.EffCompositeFx.Cym;
	double dEta1[4] = { 0.0, 0.0, 0.0, 0.0 };
	double dEta1_max = 0.0;
	double dEta1_fx = fabs(dgam_M0*dfy*dAeff) < m_dZero ? 0.0 : dN_Ed / (dfy*dAeff / dgam_M0);

	if ( dgam_M0 > 0.0 )
	{
		for ( int i = 0; i < 4; i++ )// 0=TL, 1=TR, 2=BR, 3=BL
		{
			double dRat_y = fabs(dfy*dWy_eff[i]) < m_dZero ? 0.0 : (-1.)*(dMy_Ed + dN_Ed * dey_N) / (dfy*dWy_eff[i] / dgam_M0);
			double dRat_z = fabs(dfy*dWz_eff[i]) < m_dZero ? 0.0 : (-1.)*(dMz_Ed + dN_Ed * dez_N) / (dfy*dWz_eff[i] / dgam_M0);

			// Compression(-), Tension(+)
			dEta1[i] = dEta1_fx + dRat_y; // + dRat_z;   // 수정. [4/25/2012] z방향 텀은 빼기로 함. 이대근 BJ님 동의  
			if ( i == 0 )
			{
				dEta1_max = fabs(dEta1[0]);
				iEta1_max_posi = i;
			}
			else
			{
				if ( dEta1_max < fabs(dEta1[i]) )
				{
					dEta1_max = fabs(dEta1[i]);
					iEta1_max_posi = i;
				}
			}
		}
	}

	// EN 1993-1-5, 26page.
	T_DGN_STL_STIFFENER_PART WebStiffD;
	m_pDgnSectUtil->Get_Web_StiffenerD(true, &SectD, WebStiffD);
	Get_ConvertSectStiffenerD(WebStiffD, param.arLstiPosi);
	BOOL bIsStiffener = param.arLstiPosi.GetSize() > 0;

	CPG_BCDT_POSI& BcdtPosi = param.BcdtPosi;
	int iLoadAppType = BcdtPosi.iLoadAppType;
	double da = BcdtPosi.da;
	double dSs = BcdtPosi.dSs;
	double dC = BcdtPosi.dC;
	double dEpsi = Get_Epsilon(dfyw);
	double dIsl1 = 0.0;
	double db1 = 0.0;
	double dk_F = 0.0;

	if ( bIsStiffener )
	{
		const CPG_LSTI_DATA& LstiD = param.arLstiPosi[0];
		double dI_ls = 0.0;
		db1 = LstiD.dz - LstiD.dt*0.5;

		if ( !Get_IslOfLongiStiff(LstiD, dTw, dEpsi, dIsl1) ) { dIsl1 = 0.0; }
	}

	// Modify by GAY. PMS:4216. ('12.05.15). 계산서 출력을 위한 Gamma_s_limit 적용전 계산값 저장.
	int ik_F_calc_type = 0;  // 0=Type(a), 1=Type(b), 2=Type(c)
	double dGamma_s = 0.0, dGamma_s_lim = 0.0, dGamma_s_cal = 0.0;

	if ( iLoadAppType == 0 )
	{
		double dRat = da < m_dZero ? 0.0 : dh / da;
		double db1hw = dh < m_dZero ? 0.0 : db1 / dh;
		double db1a = da < m_dZero ? 0.0 : db1 / da;

		if ( (0.05 - m_dZero < db1hw && db1hw < 0.3 + m_dZero) && (db1a < 0.3 + m_dZero) )
		{
			dGamma_s_cal = (dh*dTw < m_dZero) ? 0.0 : 10.9*dIsl1 / (dh*pow(dTw, 3));
			dGamma_s_lim = (dh*da < m_dZero) ? 0.0 : 13.0*pow(da / dh, 3) + 210 * (0.3 - db1 / da);
			dGamma_s = dGamma_s_cal < dGamma_s_lim ? dGamma_s_cal : dGamma_s_lim;
			dk_F = 6.0 + 2.0*pow(dh / da, 2) + (5.44*db1 / da - 0.21)*sqrt(dGamma_s);
			ik_F_calc_type = 1;
		}
		else
		{
			dk_F = 6.0 + 2.0*pow(dRat, 2);
			ik_F_calc_type = 2;
		}
	}
	else if ( iLoadAppType == 1 )
	{
		double dRat = da < m_dZero ? 0.0 : dh / da;
		dk_F = 3.5 + 2.0*pow(dRat, 2);
	}
	else if ( iLoadAppType == 2 )
	{
		double dRat = dh < m_dZero ? 0.0 : (dSs + dC) / dh;
		dk_F = 2.0 + 6.0*dRat;
		if ( dk_F > 6.0 ) { dk_F = 6.0; }
	}
	else ASSERT(0);

	//!/ (6.3) 과 (6.9) 식에 모순이 있다.
	// effective loaded length
	double dbf = dB1;
	if ( bBoxSect )
	{
		double dbf_LR[2] = { 0.0, 0.0 };
		Get_bf4c_DB(bPosiI, SectD, true, 0, dEpsi, dbf_LR[0]);
		Get_bf4c_DB(bPosiI, SectD, true, 1, dEpsi, dbf_LR[1]);
		dbf = dbf_LR[0] + dbf_LR[1];
	}

	double dm1 = dfyw * dTw < m_dZero ? 0.0 : dfyt * dbf / (dfyw*dTw);
	double dRat = dTF1 < m_dZero ? 0.0 : dh / dTF1;
	double dm2 = 0.02*pow(dRat, 2);
	double dl_y = 0.0, dl_e = 0.0, dl_y1 = 0.0, dl_y2 = 0.0; // effective loaded length

	if ( iLoadAppType == 0 || iLoadAppType == 1 )
	{
		dl_y = dSs + 2.0*dTF1*(1.0 + sqrt(dm1 + dm2));
	}
	else if ( iLoadAppType == 2 )
	{
		dl_e = dfyw * dh < m_dZero ? 0.0 : dk_F * dEs*pow(dTw, 2) / (2.0*dfyw*dh);
		if ( dl_e > dSs + dC ) { dl_e = dSs + dC; }

		double dl_y1_val = dTF1 < m_dZero ? dm1 / 2.0 + dm2 : dm1 / 2.0 + pow(dl_e / dTF1, 2) + dm2;
		dl_y1 = dl_e + dTF1 * sqrt(dl_y1_val);
		dl_y2 = dl_e + dTF1 * sqrt(dm1 + dm2);
		dl_y = min(dl_y1, dl_y2);
	}
	else ASSERT(0);

	// reduction factor
	double dFcr = fabs(dh) < m_dZero ? 0.0 : 0.9*dk_F*dEs*pow(dTw, 3) / dh;
	double dLamda1 = fabs(dFcr) < m_dZero ? 0.0 : dl_y * dTw*dfyw / dFcr;
	double dLamda_F = dLamda1 < 0.0 ? 0.0 : sqrt(dLamda1);
	double dX_F = fabs(dLamda_F) < m_dZero ? 0.0 : 0.5 / dLamda_F; // reduction factor
	if ( dX_F > 1.0 ) { dX_F = 1.0; }

	double dLeff = dX_F * dl_y;
	double dF_Rd = fabs(dgam_M1) < m_dZero ? 0.0 : dfyw * dLeff*dTw_t / dgam_M1;
	double dEta2 = fabs(dF_Rd) < m_dZero ? 0.0 : fabs(dF_Ed) / dF_Rd;
	double dInterRat = dEta2 + 0.8*dEta1_max; // (7.2), 29page 적용하면 수정 

	// Save Transverse Force Check Result
	PrtfBase.bOK = dInterRat < 1.4 + m_dZero ? TRUE : FALSE;
	PrtfBase.iSectClass = ClassD.iClassSect;
	PrtfBase.dF_Rd = dF_Rd;
	PrtfBase.dEta1 = dEta1_max;         // EN 1993-1-5, (4.14), 21page
	PrtfBase.dEta2 = dEta2;
	PrtfBase.dInterRat = dInterRat;     // Eta2+0.8*Eta1<=1.4  [Unit=None]  
	PrtfBase.dRatio = dInterRat;       //
	PrtfBase.dey_N = dey_N;         // ey,N                [Unit=Length]
	PrtfBase.dez_N = dez_N;         // ez,N                [Unit=Length]
	PrtfBase.dIsl1 = dIsl1;         // Isl,1               [Unit=Length^4]
	PrtfBase.dgamma_s = dGamma_s;      // gamma,s             [Unit=None]
	PrtfBase.dk_F = dk_F;          // k,F                 [Unit=None]
	PrtfBase.db1 = db1;
	PrtfBase.dm1 = dm1;
	PrtfBase.dm2 = dm2;      // m1,m2               [Unit=None]
	PrtfBase.dly1 = dl_y1;
	PrtfBase.dly2 = dl_y2;    // ly1, ly2            [Unit=Length]
	PrtfBase.dle = dl_e;
	PrtfBase.dly = dl_y;      // le, ly              [Unit=Length]
	PrtfBase.dFcr = dFcr;          // Fcr                 [Unit=Force]
	PrtfBase.dLamda_F = dLamda_F;      // Lamda,F             [Unit=None]
	PrtfBase.dX_F = dX_F;          // X,F                 [Unit=None]
	PrtfBase.dL_eff = dLeff;        // L,eff               [Unit=Length]
	for ( int k = 0; k < 4; k++ ) { PrtfBase.dEta1_p[k] = dEta1[k]; }    // eta1, 4지점 [Unit=None]

	if ( IsPrintMode() )
	{
		PrtfDetail.iEta1_max_posi = iEta1_max_posi;
		PrtfDetail.dWy_eff = dWy_eff[iEta1_max_posi];
		PrtfDetail.dWz_eff = dWz_eff[iEta1_max_posi];
		PrtfDetail.iLoadAppType = iLoadAppType;
		PrtfDetail.bIsStiffener = bIsStiffener;
		PrtfDetail.ik_F_calc_type = ik_F_calc_type;
		PrtfDetail.dGamma_s_limit = dGamma_s_lim;
		PrtfDetail.dGamma_s_cal = dGamma_s_cal;
		PrtfDetail.dhw = dh;
		PrtfDetail.da = da;
		PrtfDetail.dSs = dSs;
		PrtfDetail.dC = dC;
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Calc_LateralTorsionalBucklingResistance(T_PLATE_GIRDER_CALC_D& param, T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail)
{
	T_CLAS_BASE& ClassD = param.ClassBase;
	T_ELEM_K ElemK = param.ElemK.first;
	// 1. Material.
	T_MATD_D& MatdD = param.MatlElem.MatdD;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dEs = MatdD.Data1.Analysis.Elast;
	double dPoissonS = MatdD.Data1.Analysis.Poisson;
	double dGs = dEs / (1.0 + dPoissonS);
	double dgam_M0 = m_dGamma_M0;
	double dfyd = fabs(dgam_M0) < m_dZero ? dfy : dfy / dgam_M0;

	BOOL bPosiI = param.bPosiI;
	CPG_MEMB_POSI& MembPosi = param.MembPosi;
	CPG_LTBD_POSI& LtbdPosi = param.LtbdPosi;

	// 2. Section.
	T_SECT_D& SectD = param.SectElem.SectD;
	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
	T_SECT_STIFFNESS NonConSect, RebarSect;	  NonConSect.Initialize();	RebarSect.Initialize();
	GetNonComSectStiffD(param.SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(param.SectElem, bPosiI, RebarSect);

	// Get Section Dimension
	BOOL bSectI_Type = (m_pDgnSectUtil->Get_RealSectType(&SectD) == D_SECT_TYPE_COMPO_I);
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dTw_t = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTF1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dTF2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dHw = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);

	// 3. Force
	double dMy = PltbBase.dMy;
	double dMa_Ed = PltbBase.dMa_Ed;
	double dN_Ed = PltbBase.dN_Ed;
	double dM_Ed = PltbBase.dM_Ed;
	BOOL	bPositiveM = dMy > -m_dZero ? TRUE : FALSE;

	T_PGBR_BASE PgbrBase = param.PgbrBase;
	double dMpl_Rd = PgbrBase.dMpl_Rd; // PGBR에서 결정된 값
	double dMel_Rd = PgbrBase.dMel_Rd; // PGBR에서 결정된 값

	///////////////////////////////////////////////////////////////////////
	T_SECT_STIFFNESS StiffData;  StiffData.Initialize();
	if ( !bTapered ) { StiffData = SectD.SectBefore.Stiffness; }
	else { StiffData = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness; }

	// Girder Stiffness	
	double dArea_g = StiffData.Area;
	double dIxx_g = StiffData.Rxx;
	double dIyy_g = StiffData.Ryy;
	double dIzz_g = StiffData.Rzz;
	double dCzp_g = StiffData.Czp;
	double dCzm_g = StiffData.Czm;

	T_SECT_STIFFNESS CompoSect = bPositiveM ? RebarSect : NonConSect;
	double dArea_c = CompoSect.Area;
	double dIyy_c = CompoSect.Ryy;
	double dCzp_c = CompoSect.Czp;
	double dCzm_c = CompoSect.Czm;

	// 6.4 Lateral-torsional buckling of composite beams
	int iClassSect = ClassD.iClassSect;
	double dM_Rd = 0.0;

	if ( iClassSect == 1 || iClassSect == 2 ) { dM_Rd = dMpl_Rd; }
	else if ( iClassSect == 3 || iClassSect == 4 ) { dM_Rd = dMel_Rd; }
	else ASSERT(0);

	// Lateral torsional bucking curves-General case, EN 1993-1-1, 6.3.2.2, 61p, 
	double dLamda_LT = 0.0;
	double dL = MembPosi.dLu; // span length between the rigid supports.
	double db = bPositiveM ? dB1 : dB2;
	double dtf = bPositiveM ? dTF1 : dTF2;
	double dAwc = 0.0;
	double dAf = 0.0;

	// Awc는 Class 4 일 경우는 effective Area를 고려. 
	if ( dCzm_c < dH - dTF1 )
	{
		dAwc = bPositiveM ? (dH - dTF1 - dCzm_c)*dTw_t : (dCzm_c - dTF2)*dTw_t;
		dAf = bPositiveM ? dB1 * dTF1 : dB2 * dTF2;
	}
	else
	{
		dAwc = bPositiveM ? 0.0 : dHw * dTw_t;
		dAf = bPositiveM ? 0.0 : dB2 * dTF2;
	}

	double dV1 = PltbBase.dV1; // member개념으로 요소의 I단 전단력
	double dV2 = PltbBase.dV2; // J단 전단력
	double dM1 = PltbBase.dM1; // I 단 모멘트
	double dM2 = PltbBase.dM2; // J 단 모멘트
	double dCd = LtbdPosi.dCd; // spring stiffness
	double dl = LtbdPosi.dl;  //distance between the springs.  
	double dcc = fabs(dl) < m_dZero ? 0.0 : dCd / dl;
	double dIzz_bf = dTF2 * pow(dB2, 3) / 12.0; // Bottom flange, z축 단면이차모멘트, Design guide, 1993-2 참조
	double dgamma = (dEs*dIzz_bf == 0.0) ? 0.0 : dcc * pow(dL, 4) / (dEs*dIzz_bf);
	double dNu = 0.0;

	if ( fabs(dV1) < fabs(dV2) ) { dNu = fabs(dV2) < m_dZero ? 0.0 : fabs(dV1 / dV2); }
	else { dNu = fabs(dV1) < m_dZero ? 0.0 : fabs(dV2 / dV1); }
	double dPHI = Get_PHIform(PltbBase.dM1, PltbBase.dM2, dNu);

	double dm1 = 0.0, dm2 = 0.0, dm_min = 0.0, dm = 0.0;
	Get_m_Value(dNu, dPHI, dgamma, dm1, dm2, dm_min, dm);
	// Designer's guide to EN 1994-2
	// Eurocode 4: Design of steel and composite structures, (D6.14), 102p    
	Get_NonDimensionalSlenderness(dL, db, dfy, dEs, dm, dAwc, dAf, dLamda_LT);

	int iCase = Get_BucklingCurveForCrossSection(dH, db, bSectI_Type);
	double dAlpha_LT = Get_ImperfectionFactor(iCase);
	double dPHI_LT = Get_PHI_LT(dAlpha_LT, dLamda_LT);
	double dX_LT = 0.0; // reduction factor for lateral-torsional buckling to the relative slenderness Lamda_LT
	double dX_LT_org = 0.0;
	dX_LT = Get_X_LT(dPHI_LT, dLamda_LT, dX_LT_org);

	// design buckling resistance moment
	double dMb_Rd = dX_LT * dM_Rd;

	// N_Ed 만 작용하는 것으로 게산한다. Designer's guide to EN 1994-2, Example 6.6 참조   
	double dm_N = 1.0; //
	double dAwc_N = dHw * dTw_t;
	double dAf_N = dAf;
	double dLamda_LT_N = 0.0;
	Get_NonDimensionalSlenderness(dL, db, dfy, dEs, dm_N, dAwc_N, dAf_N, dLamda_LT_N);
	double dPHI_LT_N = Get_PHI_LT(dAlpha_LT, dLamda_LT_N);
	double dX_LT_N_org = 0.0;
	double dX_LT_N = Get_X_LT(dPHI_LT_N, dLamda_LT_N, dX_LT_N_org);
	double dNb_Rd = dX_LT_N * dArea_c*dfyd;
	//
	double dN_Rat = fabs(dNb_Rd) < m_dZero ? 0.0 : fabs(dN_Ed) / dNb_Rd;
	double dM_Rat = fabs(dMb_Rd) < m_dZero ? 0.0 : fabs(dM_Ed) / dMb_Rd;
	double dCombRat = dN_Rat + dM_Rat;

	int iAlpha = LtbdPosi.iAlpha;
	double dEa = dEs; // girder
	double dI2 = CompoSect.Ryy; // girder+steel
	double dv_a = dPoissonS; // Poisson's ratio for Steel
	double dhs = dH - 0.5*(dTF1 + dTF2);
	double da = LtbdPosi.da;
	double dk1 = fabs(da) < m_dZero ? 0.0 : double(iAlpha)*dEa*dI2 / da;
	double dk2_val1 = 4.0*(1.0 - dv_a * dv_a)*dhs;
	double dk2 = fabs(dk2_val1) < m_dZero ? 0.0 : dEa * pow(dTw, 3) / dk2_val1;
	double dks = fabs(dk1 + dk2) < m_dZero ? 0.0 : dk1 * dk2 / (dk1 + dk2);

	//
	double dC4 = PltbBase.dC4;
	double dI_at = dIxx_g; // St.Venant torsional stiffness of the steel section  
	double dI_afz = dTF2 * pow(dB2, 3) / 12.0; // ???,  is the flexural stiffness of bottom flange respect the z axis of the steel section
	double dAa = dArea_g;
	double dA = RebarSect.Area; // 전체환산단면.
	double dIay = dIyy_g;
	double dIaz = dIzz_g;
	double dix2 = dAa < m_dZero ? 0.0 : (dIay + dIaz) / dAa;
	double dhc = dH - 0.5*(dTF1 + dTF2);
	double dSCzm_g = Get_ShearCenter_H(dHw, dTF1, dB1, dTF2, dB2); // shear center
	double dZs = dCzm_g - dSCzm_g; // is the distance between section centre of gravity _T("C") and shear barycentre(positive if shear barycentre is lower than _T("C"))
	double dZc = dH + dHh + 0.5*dtc - dCzm_g;
	double de = fabs(dAa*dZc*(dA - dAa)) < m_dZero ? 0.0 : dA * dIay / (dAa*dZc*(dA - dAa));
	double dZf = dIaz < m_dZero ? 0.0 : dhs * dI_afz / dIaz;
	double dZj_val2 = Get_Zj_Second_Value(dH, dHw, dTw, dB1, dTF1, dB2, dTF2, dCzm_g, dAa, dIay);
	double dZj = dZs - dZj_val2;

	//
	if ( dI_afz > 0.5*dIaz )
	{
		dZj = fabs(dIaz) < m_dZero ? 0.0 : 0.4*dhs*(2.0*dI_afz / dIaz - 1.0);
	}
	else
	{
		dZj = dZs - dZj_val2;
	}

	double dkc_val1 = dIay < m_dZero ? 0.0 : dhs * dIyy_c / dIay;
	double dkc_val2 = fabs(de) < m_dZero ? 0.0 : (pow((dZf - dZs), 2) + dix2) / de + 2.0*(dZf - dZj);
	double dkc = fabs(dkc_val2) < m_dZero ? 0.0 : dkc_val1 / dkc_val2;

	double dMcr_val = (dGs*dI_at + dks * dL*dL / (m_dPi*m_dPi))*dEa*dI_afz;
	double dMcr = fabs(dL) < m_dZero ? 0.0 : dkc * dC4 / dL * sqrt(dMcr_val);

	// Save Lateral Torsional Buckling Check Result
	PltbBase.bOK = dCombRat > 1.0 ? FALSE : TRUE;
	PltbBase.iSectClass = ClassD.iClassSect;
	PltbBase.dMpl_Rd = dMpl_Rd;
	PltbBase.dMel_Rd = dMel_Rd;
	PltbBase.dNb_Rd = dNb_Rd;
	PltbBase.dMb_Rd = dMb_Rd;
	PltbBase.dInterRat = dCombRat;
	PltbBase.dMcr = dMcr;
	PltbBase.dRatio = dCombRat;
	PltbBase.dM_Rd = dM_Rd;         // M_Rd               [Unit=Moment]
	PltbBase.dcc = dcc;           // Cd/l               [Unit=Stress]
	PltbBase.dgamma = dgamma;        //                    [Unit=None]
	PltbBase.dMu = dNu;           // V1/V2 or V1/V2     [Unit=None] 위에는 dNu라고 되어 있음.
	PltbBase.dPHI = dPHI;          //                    [Unit=None]
	PltbBase.dm1 = dm1;
	PltbBase.dm2 = dm2;           //                    [Unit=None]
	PltbBase.dm_min = dm_min;
	PltbBase.dm = dm;            //                    [Unit=None]
	PltbBase.dAwc = dAwc;          //                    [Unit=Area]
	PltbBase.dAf = dAf;           //                    [Unit=Area]
	PltbBase.dLamda_LT = dLamda_LT;     //                    [Unit=None]
	PltbBase.dAlpha_LT = dAlpha_LT;     //                    [Unit=None]
	PltbBase.dPHI_LT = dPHI_LT;       //                    [Unit=None]
	PltbBase.dX_LT_org = dX_LT_org;     //                    [Unit=None]
	PltbBase.dX_LT = dX_LT;         //                    [Unit=None]
	PltbBase.dX_LT_N = dX_LT_N;
	PltbBase.dk1 = dk1;
	PltbBase.dk2 = dk2;
	PltbBase.dks = dks;           //                    [Unit=None]
	PltbBase.dI_at = dI_at;         //                    [Unit=L^4]
	PltbBase.dI_afz = dI_afz;        //                    [Unit=L^4]
	PltbBase.dkc = dkc;

	if ( IsPrintMode() )
	{
		PltbDetail.dMpl_Rd = dMpl_Rd;
		PltbDetail.dMel_Rd = dMel_Rd;
		PltbDetail.dL = dL;
		PltbDetail.dhs = dhs;
		PltbDetail.da = da;
		PltbDetail.dEa = dEa;
		PltbDetail.dGs = dGs;
		PltbDetail.dI2 = dI2;
		PltbDetail.iAlpha = iAlpha;
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Calc_VerticalShearResistance(T_PLATE_GIRDER_CALC_D& param, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
	T_ELEM_K ElemK = param.ElemK.first;
	T_CLAS_BASE& ClassD = param.ClassBase;
	T_VERTICAL_SHAER_RES_D ResD;

	// 1. Material.
	T_MATD_D& MatdD = param.MatlElem.MatdD;
	int iMatClass = param.MatlElem.iMatClass;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dgam_M0 = m_dGamma_M0;
	double dEta = Get_Eta4SteelGrade(iMatClass, dfy);

	BOOL bPosiI = param.bPosiI;
	// 2. Section.
	T_SECT_D& SectD = param.SectElem.SectD;
	bool bBoxSectT = m_pDgnSectUtil->Is_StlBoxSect(&SectD);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dTw_t = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
	double dHw = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dhw_in = m_pDgnSectUtil->Get_Size_Inclind_hw(&SectD, bPosiI);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dEpsi = Get_Epsilon(dfyw);

	// 3. Force
	double aForce[6] = { PrvsBase.dFx, 0.0, 0.0, 0.0, PrvsBase.dMy, PrvsBase.dMz };
	double dFx = PrvsBase.dFx;
	double dMy = PrvsBase.dMy;
	double dMz = PrvsBase.dMz;
	double dMa_Ed = PrvsBase.dMa_Ed;
	double dMc_Ed = dMy;
	double dV_Ed_a = PrvsGd.dFz;
	double dV_Ed_c = PrvsBase.dFz;
	double dV_Ed = PrvsBase.dV_Ed;

	// 4. Calc. Plastic resistance to vertical shear, EN 1993-1-1: 2005, 6.2.6 (50page)
	double dAv = dEta * dhw_in*dTw_t;
	double dVpl_Rd = dgam_M0 < m_dZero ? 0.0 : dAv * (dfyw / sqrt(3.0)) / dgam_M0;

	// 5. Calc. Shear buckling resistance, EN 1993-1-5,5 (22page)
	double dVb_Rd = 0.0;
	Calc_Vb_Rd(param, SectD, PrvsBase, dVb_Rd, ResD);

	// 6. Determine V_Rd (Vertical Shear Resistance)
	double dV_Rd = 0.0;
	double dRatio = 0.0;

	if ( ResD.ShearResD[0].bIsShearBuck || ResD.ShearResD[1].bIsShearBuck ) // Case 1. Shear Buckling Check is needed 
	{
		dV_Rd = dVb_Rd;
		dRatio = fabs(dVb_Rd) < m_dZero ? fabs(dV_Ed) / m_dZero : fabs(dV_Ed) / dVb_Rd;
	}
	else // Case 2. Shear Buckling Check is not needed 
	{
		dV_Rd = dVpl_Rd;
		dRatio = fabs(dVpl_Rd) < m_dZero ? fabs(dV_Ed) / m_dZero : fabs(dV_Ed) / dVpl_Rd;
	}

	if ( dRatio > 1.0 ) { PrvsBase.bOK = FALSE; }
	else { PrvsBase.bOK = TRUE; }

	if ( PrvsBase.bOK && ResD.ShearResD[0].bEta3Exceed05 )
	{
		if ( ClassD.iClassSect == 3 || ClassD.iClassSect == 4 )
		{
			if ( !ResD.ShearResD[0].bCombinedEffe ) { PrvsBase.bOK = FALSE; }
		}
	}

	// 7. Save Vertical Shear Check Result
	PrvsBase.iTopClass = ClassD.iTopFlgClass;
	PrvsBase.iBotClass = ClassD.iBotFlgClass;
	PrvsBase.iWebClass = ClassD.iWebClass;
	PrvsBase.iSectClass = ClassD.iClassSect;
	PrvsBase.dM_Ed = ResD.ShearResD[0].dM_Ed;
	PrvsBase.dMf_Rd = ResD.ShearResD[0].dMf_Rd;
	PrvsBase.dMpl_Rd = ResD.ShearResD[0].dMpl_Rd;
	PrvsBase.dVpl_Rd = dVpl_Rd;
	PrvsBase.dVbw_Rd = ResD.dVbw_Rd;
	PrvsBase.dVbf_Rd = ResD.dVbf_Rd;
	PrvsBase.dVb_Rd = dVb_Rd;
	PrvsBase.dV_Rd = dV_Rd;
	PrvsBase.dEta1 = ResD.ShearResD[0].dBar_Eta1;
	PrvsBase.dEta1_lim = ResD.ShearResD[0].dBar_Eta1_Lim;
	PrvsBase.dEta3 = ResD.ShearResD[0].dBar_Eta3;
	PrvsBase.dCombinedEffe = ResD.ShearResD[0].dCombinedEffe;
	PrvsBase.bCombinedEffe = ResD.ShearResD[0].bCombinedEffe;
	PrvsBase.bEta3Exceed05 = ResD.ShearResD[0].bEta3Exceed05;
	PrvsBase.dRatio = dRatio;
	PrvsBase.dAv = dAv;
	PrvsBase.deta = dEta;
	PrvsBase.dIsl_org = ResD.ShearResD[0].dIsl_org;
	PrvsBase.dIsl = ResD.ShearResD[0].dIsl;
	PrvsBase.dXw = ResD.ShearResD[0].dXw;
	PrvsBase.dMax_Bar_Lamda_w = ResD.ShearResD[0].dMax_Bar_Lamda_w;
	PrvsBase.dc = ResD.ShearResD[0].dc;
	PrvsBase.dbf = ResD.ShearResD[0].dbf;
	PrvsBase.dtf = ResD.ShearResD[0].dtf;
	PrvsBase.dVb_Rd_org = ResD.ShearResD[0].dVb_Rd_org;
	PrvsBase.dVb_Rd_Lim = ResD.ShearResD[0].dVb_Rd_Lim;
	PrvsBase.bEta1Ratio = FALSE;

	Convert_VerbShearResD(ResD.ShearResD[0], PrvsBase.DetailResD[0]);
	if ( bBoxSectT ) { Convert_VerbShearResD(ResD.ShearResD[1], PrvsBase.DetailResD[1]); }

	// 8. Save Vertical Shear Check Result for Print
	CArray<T_PRVS_Vbw_Rd_Param, T_PRVS_Vbw_Rd_Param&> aGenWebVbwRdParam;

	if ( IsPrintMode() )
	{
		int nWebSize = bBoxSectT ? 2 : 1;
		for ( int nWeb = 0; nWeb < nWebSize; ++nWeb )
		{
			T_PRVS_Vbw_Rd_Param CalcVbwParam;
			const T_VERTICAL_SHAER_RES_UNIT_D& Res = ResD.ShearResD[nWeb];
			double dAlpha_p = Get_Alpha4Lamda_w(Res.dRigidPitch, dhw_in);
			CalcVbwParam.iStiffSize = Res.iLongStiffSize;
			CalcVbwParam.dc = Res.dc;
			CalcVbwParam.dVbf_Rd = Res.dVbf_Rd;

			if ( Res.iLongStiffSize > 0 )
			{
				double dAlpha_si = Get_Alpha4Lamda_w(Res.dai_cr, Res.dhi_cr);
				Set_Calc_Vbw_SubPanel_Param(Res.nLSsize_cr, Res.dhi_cr, dTw, dAlpha_si, Res.dk_tau_si_cr, Res.dk_tau_sti_cr,
					Res.dk_tau_st_lti_cr, Res.dIsli_cr, Res.dMax_Lamda_wi, CalcVbwParam.Lamda_w_SubD);
			}

			int nPartType = 2; //Web
			CalcVbwParam.nPartType = nPartType;
			CalcVbwParam.bStiffened = Res.bStiffened;
			CalcVbwParam.bIsShearBuck = Res.bIsShearBuck;

			if ( nPartType == 2 ) // Web
			{
				Set_Calc_Vbw_Param(dAlpha_p, Res.dk_tau_p, Res.dk_tau_st_p, Res.dk_tau_st_Limit_p, Res.dIsl, dhw_in, dTw,
					Res.dLamda_w_part, Res.dLamda_w_cr, Res.iLamda_w_Type, CalcVbwParam);
				Set_Calc_Vbw_Param2(Res.iLamda_w_scope, Res.dXw, Res.dVbw_Rd, dVb_Rd, Res.dVb_Rd_Lim, dV_Ed, Res.dBar_Eta3, CalcVbwParam);
				CalcVbwParam.dVpl_Rd = dVpl_Rd;

				aGenWebVbwRdParam.Add(CalcVbwParam);
			}
		}

		PrvsDetail.StressInfoHshapeD = param.PgbrDetail.StressInfoHshapeD;
		PrvsDetail.iEndSupportType = ResD.ShearResD[0].iEndpost;
		PrvsDetail.dPlasticNA = param.PgbrBase.dPlasticNA;
		PrvsDetail.dN_slab = param.PgbrDetail.dN_slab;
		PrvsDetail.dNr_top = param.PgbrDetail.dNr_top;
		PrvsDetail.dNr_bot = param.PgbrDetail.dNr_bot;
		PrvsDetail.dNg_top = param.PgbrDetail.dNg_top;
		PrvsDetail.dNg_bot = param.PgbrDetail.dNg_bot;
		PrvsDetail.dM_Ed_top = param.PgbrDetail.dM_Ed_top;
		PrvsDetail.dM_Ed_bot = param.PgbrDetail.dM_Ed_bot;
		PrvsDetail.dMf_Rd_cal = ResD.ShearResD[0].dMf_Rd;
		PrvsDetail.dMf_N_Rd = ResD.ShearResD[0].dMf_Rd_factor;
		PrvsDetail.dReduFactor4N_Ed = ResD.ShearResD[0].dFactor_fr;
		PrvsDetail.nShearBuckCount = ResD.ShearResD[0].nShearBuckCount;
		PrvsDetail.aGenWebVbwRdParam.Copy(aGenWebVbwRdParam);
	}

	return TRUE;
}
BOOL CDgnPlateGirder::Calc_ElasticResistance(BOOL bPositive, double aForceGd[6], double aForce[6], T_PLATE_GIRDER_CALC_D& param, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail)
{
	T_ELEM_K ElemK = param.ElemK.first;
	// 1. Material.
	T_MATD_D& MatdD = param.MatlElem.MatdD;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dEs = MatdD.Data1.Analysis.Elast;
	double dfck = param.MatlElem.dfck;
	double dEcm = param.MatlElem.dEcm;
	double dfsk = param.MatlElem.MatdD.MainRebarData.B_fy;
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;
	double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk / dgam_sr;

	BOOL bPosiI = param.bPosiI;
	// 2. Section.
	T_SECT_D& SectD = param.SectElem.SectD;
	T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
	T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();

	GetNonComSectStiffD(param.SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(param.SectElem, bPosiI, RebarSect);

	// Dimension
	double dTF1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dTF2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);
	double dfyt = Get_fy4Thick(dfy, dfy1, dTF1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTF2);
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;

	// 3. Force
	double dMel_Rd = 0.0, dNc_el = 0.0;
	double dFx = aForce[0];
	double dMy = aForce[4];
	double dMa_Ed = aForceGd[4];
	double dMc_Ed = dMy;

	T_CLAS_BASE ClassD = param.ClassBase;
	int iClassSect = ClassD.iClassSect;

	T_SECT_STIFFNESS EffeStlSect;  EffeStlSect.Initialize();
	T_SECT_STIFFNESS EffeComSect;  EffeComSect.Initialize();
	//
	double da = param.TrstPosi.dPitch;

	T_SECT_STIFFNESS GirderStiff;
	Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);

	double aForceGd2[6];  memset(aForceGd2, 0, sizeof(aForceGd2));
	double aForce2[6];  memset(aForce2, 0, sizeof(aForce2));
	aForceGd2[0] = aForceGd[0];

	double dDelta_z_a = 0.0, dDelta_z_c = 0.0;
	double dDelta_Ma_y = 0.0, dDelta_Mc_y = 0.0;

	T_DGNLINE_LBD_BR RhocBeforeFx, RhocAfterFx, RhocBeforeMy, RhocAfterMy;
	T_SECT_STIFFNESS TempStiffness, EffStiffness4Fx, CompEffStiffness4Fx;
	TempStiffness.Initialize(); EffStiffness4Fx.Initialize(); CompEffStiffness4Fx.Initialize();

	BOOL bBeforePositiveM = (dMa_Ed >= 0.0);
	if ( iClassSect == 4 && aForceGd[0] < -m_dZero )
	{
		T_DGNLINE_LBD_BR RhocAfterFxTemp;
		Get_EffectiveSectionProperty4DB(TRUE, bBeforePositiveM, aForceGd2, aForce2, da, param, EffStiffness4Fx, TempStiffness, RhocBeforeFx, RhocAfterFxTemp);
		dDelta_z_a = GirderStiff.Czm - EffStiffness4Fx.Czm;
		dDelta_Ma_y = dDelta_z_a * fabs(aForceGd[0]);
	}
	else
	{
		EffStiffness4Fx = GirderStiff;
	}

	// Composite일 때 작용한 Force
	BOOL bAfterPositiveM = (dMy >= 0.0);
	aForce2[0] = aForce[0];
	if ( iClassSect == 4 && dFx < -m_dZero )
	{
		T_DGNLINE_LBD_BR RhocBeforeFxTemp;
		Get_EffectiveSectionProperty4DB(FALSE, bAfterPositiveM, aForceGd2, aForce2, da, param, TempStiffness, CompEffStiffness4Fx, RhocBeforeFxTemp, RhocAfterFx, TRUE);

		if ( bPositive )
		{
			dDelta_z_c = RebarSect.Czm - CompEffStiffness4Fx.Czm;
			dDelta_Mc_y = dDelta_z_c * fabs(dFx); // if dDelta_z<0, negative moment.
		}
		else
		{
			dDelta_z_c = RebarSect.Czm - CompEffStiffness4Fx.Czm; // 편심은 정모멘트 gross 기준으로
			dDelta_Mc_y = dDelta_z_c * fabs(dFx); // if dDelta_z<0, negative moment.
		}
	}
	else
	{
		CompEffStiffness4Fx = RebarSect;
	}

	double dMy_gir = aForceGd[4] + dDelta_Ma_y;
	double dMy_com = dMy + dDelta_Mc_y;

	// 모멘트에 대하여
	memset(aForceGd2, 0, sizeof(aForceGd2));
	memset(aForce2, 0, sizeof(aForce2));
	aForceGd2[4] = aForceGd[4];
	T_SECT_STIFFNESS EffStiffness4My; EffStiffness4My.Initialize();
	T_SECT_STIFFNESS CompEffStiffness4My; CompEffStiffness4My.Initialize();

	if ( iClassSect == 4 )
	{
		T_DGNLINE_LBD_BR RhocAfterMyTemp;
		Get_EffectiveSectionProperty4DB(TRUE, bBeforePositiveM, aForceGd2, aForce2, da, param, EffStiffness4My, TempStiffness, RhocBeforeMy, RhocAfterMyTemp);
	}
	else
	{
		EffStiffness4My = GirderStiff;
	}

	memset(aForce2, 0, sizeof(aForce2));
	aForce2[4] = dMy;
	if ( iClassSect == 4 )
	{
		T_DGNLINE_LBD_BR RhocBeforeMyTemp;
		Get_EffectiveSectionProperty4DB(FALSE, bAfterPositiveM, aForceGd2, aForce2, da, param, TempStiffness, CompEffStiffness4My, RhocBeforeMyTemp, RhocAfterMy, TRUE);
	}
	else
	{
		CompEffStiffness4My = bPositive ? RebarSect : NonConSect;
	}

	param.tEffeStlSect = EffeStlSect = EffStiffness4My; // My에 대한 Effective stiffness
	param.tEffeComSect = EffeComSect = CompEffStiffness4My; // My에 대한 Composite Effective stiffness

	// Before
	double dStl_Area = EffStiffness4Fx.Area;
	double dStl_Iyy = EffStiffness4My.Ryy;
	double dStl_Izz = EffStiffness4My.Rzz;
	double dStl_Czm = EffStiffness4My.Czm;
	double dStl_Czp = EffStiffness4My.Czp;
	// Composite
	double dArea_c = CompEffStiffness4Fx.Area;
	double dIyy_c = CompEffStiffness4My.Ryy;
	double dIzz_c = CompEffStiffness4My.Rzz;
	double dCzm_c = CompEffStiffness4My.Czm;
	double dCzp_c = CompEffStiffness4My.Czp;

	double dSIGa_top = 0.0;
	double dSIGa_bot = 0.0;

	// Rebar  
	double dRbarZ_max = 0.0;
	int iRbarSize = param.arRbarPosi.GetSize();
	for ( int i = 0; i < iRbarSize; i++ )
	{
		const _DGN_RBAR_CRC& RbarD = param.arRbarPosi[i];
		if ( i == 0 ) { dRbarZ_max = RbarD.dz; }
		else { if ( dRbarZ_max < RbarD.dz ) { dRbarZ_max = RbarD.dz; } }
	}

	// MQC-20121, M_Ed_t, M_Ed_b를 계산할 때 응력은 축력을 고려하지 않는다.
	double dFx_a = aForceGd[0];
	double dMz_a = 0.0;
	double dStrStlTop = Calc_StressOfPosition(0.0, dMy_gir, dMz_a, dStl_Area, dStl_Iyy, dStl_Izz, 0.0, dStl_Czp);
	double dStrComTop = Calc_StressOfPosition(0.0, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, dCzp_c);
	double dStrStlBot = Calc_StressOfPosition(0.0, dMy_gir, dMz_a, dStl_Area, dStl_Iyy, dStl_Izz, 0.0, -dStl_Czm);
	double dStrComBot = Calc_StressOfPosition(0.0, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, -dCzm_c);
	dSIGa_top = dStrStlTop + dStrComTop; // (-) 압축, (+) 인장.
	dSIGa_bot = dStrStlBot + dStrComBot;

	double dn = dEcm < m_dZero ? 0.0 : dEs / dEcm;
	double dCentToSlabTop = dHt - dCzm_c;
	double dStrSlab_top = dn < m_dZero ? 0.0 : Calc_StressOfPosition(dFx, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, dCentToSlabTop) / dn;
	double dCentToRbar = dHt - dtc / 2.0 + dRbarZ_max - dCzm_c;  //dRbarZ_max : 슬래브 도심에서 철근까지 거리.
	double dStrRbar_top = Calc_StressOfPosition(dFx, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, dCentToRbar);

	double dM_Ed_top = fabs(dCzp_c) < m_dZero ? 0.0 : fabs(dSIGa_top*(dIyy_c / dCzp_c)); // For shear resistance
	double dM_Ed_bot = fabs(dCzm_c) < m_dZero ? 0.0 : fabs(dSIGa_bot*(dIyy_c / dCzm_c));
	double dM_Ed = max(dM_Ed_top, dM_Ed_bot);

	// k : 수정 2013.10.28  // Compression(-), Tension(+)
	//     수정 2014.04.17  :  개념 : Sig_a + k*Sig_c = Sig_y
	//                                Sig_c 와 Sig_y의 부호는 같도록 한다.
	//     수정 2014.04.29  :   Sig_a + Sig_Dela + k*Sig_c + Sig_Delc  = Sig_y

	//double dk_slab_temp = dStrSlab_top==0.0 ? dfcd/m_dZero : fabs(dfcd/dStrSlab_top);
	double dSigma_sl_gir = 0.0; // 합성전 슬래브 단면 없다.
	double dSigma_sl_My = (dIyy_c * dn < m_dZero) ? 1.0 / m_dZero : (-1.)*dMy           *dCentToSlabTop / dIyy_c / dn;
	double dSigma_sl_DelMy = (dIyy_c * dn < m_dZero) ? 1.0 / m_dZero : (-1.)*dDelta_Mc_y   *dCentToSlabTop / dIyy_c / dn;
	double dfcd_temp = (dSigma_sl_My + dSigma_sl_DelMy < 0.0) ? (-1.0)*dfcd : dfcd;
	double dk_slab = fabs(dSigma_sl_My) < m_dZero ? dfcd / m_dZero : (dfcd_temp - dSigma_sl_gir - dSigma_sl_DelMy) / (dSigma_sl_My);

	//double dk_rbar_temp = dStrRbar_top==0.0 ? dfsd/m_dZero : fabs(dfsd/dStrRbar_top);
	double dSigma_rb_gir = 0.0; // 합성전 철근 없다.
	double dSigma_rb_My = (dIyy_c < m_dZero) ? 1.0 / m_dZero : (-1.)*dMy           *dCentToRbar / dIyy_c;
	double dSigma_rb_DelMy = (dIyy_c < m_dZero) ? 1.0 / m_dZero : (-1.)*dDelta_Mc_y   *dCentToRbar / dIyy_c;
	double dfsd_temp = (dSigma_rb_My + dSigma_rb_DelMy < 0.0) ? (-1.0)*dfsd : dfsd;
	double dk_rbar = fabs(dSigma_rb_My) < m_dZero ? dfsd / m_dZero : (dfsd_temp - dSigma_rb_gir - dSigma_rb_DelMy) / (dSigma_rb_My);

	//double dk_top_temp = dStrComTop==0.0 ? (dfyd_t-fabs(dStrStlTop))/m_dZero : (dfyd_t-fabs(dStrStlTop))/fabs(dStrComTop);
	double dSigma_top_gir = (dStl_Iyy < m_dZero) ? 1.0 / m_dZero : (-1.)*dMy_gir       *dStl_Czp / dStl_Iyy;
	double dSigma_top_My = (dIyy_c < m_dZero) ? 1.0 / m_dZero : (-1.)*dMy           *dCzp_c / dIyy_c;
	double dSigma_top_DelMy = (dIyy_c < m_dZero) ? 1.0 / m_dZero : (-1.)*dDelta_Mc_y   *dCzp_c / dIyy_c;
	double dfyd_t_temp = (dSigma_top_My + dSigma_top_DelMy < 0.0) ? (-1.0)*dfyd_t : dfyd_t;
	double dk_top = fabs(dSigma_top_My) < m_dZero ? 1.0 / m_dZero : (dfyd_t_temp - dSigma_top_gir - dSigma_top_DelMy) / (dSigma_top_My);

	//double dk_bot_temp = dStrComBot==0.0 ? (dfyd_b-fabs(dStrStlBot))/m_dZero : (dfyd_b-fabs(dStrStlBot))/fabs(dStrComBot);
	double dSigma_bot_gir = (dStl_Iyy < m_dZero) ? 1.0 / m_dZero : (-1.)*dMy_gir       *(-dStl_Czm) / dStl_Iyy;
	double dSigma_bot_My = (dIyy_c < m_dZero) ? 1.0 / m_dZero : (-1.)*dMy           *(-dCzm_c) / dIyy_c;
	double dSigma_bot_DelMy = (dIyy_c < m_dZero) ? 1.0 / m_dZero : (-1.)*dDelta_Mc_y   *(-dCzm_c) / dIyy_c;
	double dfyd_b_temp = (dSigma_bot_My + dSigma_bot_DelMy < 0.0) ? (-1.0)*dfyd_t : dfyd_t;
	double dk_bot = fabs(dSigma_bot_My) < m_dZero ? 1.0 / m_dZero : (dfyd_b_temp - dSigma_bot_gir - dSigma_bot_DelMy) / (dSigma_bot_My);

	BOOL bPositive2 = (dMy_com > -m_dZero) ? TRUE : FALSE;
	int    nkminType = 0;
	double dk_min = Calc_kmin(bPositive2, dk_rbar, dk_slab, dk_top, dk_bot, nkminType);
	double dMel_Rd_temp = dMa_Ed + dk_min * dMy + dDelta_Mc_y; // (20140313 수정) : k는 축력의 영향을 고려안한 모멘트에만 추가.
	if ( nkminType == Kmin_Slab || nkminType == Kmin_Rbar )
	{
		dMel_Rd_temp = dk_min * dMy + dDelta_Mc_y;
	}
	dMel_Rd = dMel_Rd_temp;

	// Positive 일 때만 계산.
	double dSig_c_mean_temp = 0.0;
	double dNc_el_temp = 0.0;

	double dStop = dHt - dCzm_c;
	double dSig_c_mean = 0.0;
	if ( bPositive )
	{
		//double dza_sc = dHt - dStl_Czm - 0.5*dtc; // Ma
		// [4/11/2014 joahn], dSig_c_mean 값은 거더를 포함하지 않는 것으로 수정, 최성기 CJ님 요청
		dSig_c_mean = dk_min * dMc_Ed / dIyy_c * (dStop - 0.5*dtc) / dn;
		dNc_el = dSig_c_mean * (dBc*dtc);
	}

	// Save Result
	PgbrBase.dMel_Rd = dMel_Rd;
	PgbrBase.dNc_el = dNc_el;
	PgbrBase.dElasticNA = dMy > -m_dZero ? RebarSect.Czm : NonConSect.Czm;
	PgbrBase.dElasticNAEff = EffeComSect.Czm;// 
	//
	PgbrBase.dStrStlTop = dStrStlTop;
	PgbrBase.dStrComTop = dStrComTop;
	PgbrBase.dStrStlBot = dStrStlBot;
	PgbrBase.dStrComBot = dStrComBot;
	PgbrBase.dSIGa_top = dSIGa_top;
	PgbrBase.dSIGa_bot = dSIGa_bot;
	PgbrBase.dk_min = dk_min;

	PgbrDetail.dM_Ed_top = dM_Ed_top;
	PgbrDetail.dM_Ed_bot = dM_Ed_bot;
	PgbrDetail.dM_Ed = dM_Ed;
	PgbrDetail.EffGirderFx = EffStiffness4Fx;
	PgbrDetail.EffCompositeFx = CompEffStiffness4Fx;
	PgbrDetail.EffGirder = EffeStlSect;
	PgbrDetail.EffComposite = EffeComSect;
	PgbrDetail.nKminType = nkminType;

	if ( IsPrintMode() )
	{
		PgbrDetail.dDelMa_Ed = dDelta_Ma_y;
		PgbrDetail.dDelMc_Ed = dDelta_Mc_y;
	}

	PgbrDetail.RhocBeforeFx = RhocBeforeFx;
	PgbrDetail.RhocAfterFx = RhocAfterFx;
	PgbrDetail.RhocBeforeMy = RhocBeforeMy;
	PgbrDetail.RhocAfterMy = RhocAfterMy;

	return TRUE;
}

BOOL CDgnPlateGirder::Get_EffeSectPropAfterDB(BOOL bIsBefore, BOOL bPositiveM, double dfy1, double dfy2, double* aForceGd, double* aForce, double da,
	T_PLATE_GIRDER_CALC_D& param, T_SECT_STIFFNESS& EffeStiff, T_DGNLINE_LBD_BR& RhoC, BOOL bCompoStiffData/*=FALSE*/)
{
	// 1. Material
	T_MATD_D& MatdD = param.MatlElem.MatdD;
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dEgir = MatdD.Data1.Analysis.Elast;
	double dEslab = 0.0;
	if ( MatdD.Type == _T("SRC") )
	{
		dEslab = MatdD.Data2.Analysis.Elast;
	}
	else
	{
		dEslab = param.MatlElem.dEcm;
	}
	double dPg = MatdD.Data1.Analysis.Poisson;
	double dPS = MatdD.Data2.Analysis.Poisson;

	// 2. Section.  
	T_SECT_D& SectD = param.SectElem.SectD;
	T_SECT_STIFFNESS NonConSect;   NonConSect.Initialize();
	T_SECT_STIFFNESS RebarSect;  RebarSect.Initialize();

	BOOL bPosiI = param.bPosiI;
	GetNonComSectStiffD(param.SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(param.SectElem, bPosiI, RebarSect);

	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dhw = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dtw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);
	double dBs = bPosiI ? SectD.SectAfter.SectI.Size[0] : SectD.CmpTapJ.Size[59];  // Slab Width
	int    iNg = SectD.SectAfter.SectI.BuiltUpFlag;  // Number of Main Girder

	// 3. Force
	//double dFx = aForce[0], dMy = aForce[4], dMz = aForce[5];
	//double dMy = bIsBefore ? aForceGd[4] : aForce[4];
	//BOOL bPositive = dMy > -m_dZero ? TRUE : FALSE;

	T_SECT_STIFFNESS GirderStiff;
	Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);

	T_SECT_STIFFNESS CompoStiff;  CompoStiff.Initialize();
	if ( bPositiveM ) { CompoStiff = RebarSect; }
	else { CompoStiff = NonConSect; }
	CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&> aStiffness;
	aStiffness.SetSize(3);
	aStiffness.SetAt(0, GirderStiff);
	aStiffness.SetAt(1, RebarSect);
	aStiffness.SetAt(2, NonConSect);

	T_SECT_STIFFNESS CompoGiderEffeStiffD;
	Get_EffeSectPropBeforeDB(bIsBefore, bPositiveM, param, dfy1, dfy2, aForceGd, aForce, da, aStiffness, CompoGiderEffeStiffD, RhoC);

	// Slab, Rebar 적용..
	BOOL bScaleFactor = FALSE;
	T_SECT_INFO_D SectInfoD;

	// Section.
	double dScaleFactor[7] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

	// Girder
	T_SECT_PART_D GirderPartD;
	GirderPartD.Stiff.Stiffness = CompoGiderEffeStiffD;
	GirderPartD.Stiff.dYbar = CompoGiderEffeStiffD.Cym;
	GirderPartD.Stiff.dZbar = CompoGiderEffeStiffD.Czm;
	GirderPartD.dElast = dEgir;
	GirderPartD.dPoisson = dPg;
	SectInfoD.aSectPart.Add(GirderPartD);

	CString strCodeNa = _T(""), strRbarNa = _T("");
	CDBLib::GetDefaultConRbar(strCodeNa, strRbarNa);
	T_MATL_REBAR MatlRbarD; MatlRbarD.Initialize();
	m_pDoc->m_pMatlDB->GetRebarData(strCodeNa, strRbarNa, MatlRbarD);
	double dEr = MatlRbarD.B_Elast;
	double dDr = MatlRbarD.B_Density;

	// Slab  
	T_SECT_INFO_D SlabSectInfoD;
	T_SECT_PROP_D SlabSectPropD;
	T_SECT_PART_D SlabPartD;
	T_SECT_STIFFNESS SlabOrgStiffD; SlabOrgStiffD.Initialize();
	T_SECT_STIFFNESS SlabRbarStiffD; SlabRbarStiffD.Initialize();
	Get_StiffnessOfSB(dBs, dBc, dtc, iNg, SlabOrgStiffD);

	double dZslab_sta = dH + dHh;
	double dZcent_slb = dHt - 0.5*dtc;
	SlabPartD.dElast = dEslab;
	SlabPartD.dPoisson = 0.3;
	SlabPartD.Stiff.Stiffness = SlabOrgStiffD;
	SlabPartD.Stiff.dYbar = SlabOrgStiffD.Cym;
	SlabPartD.Stiff.dZbar = dZcent_slb;

	int iRbarSize = param.arRbarPosi.GetSize();

	if ( !bPositiveM && iRbarSize > 0 )
	{
		CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arTempRbarPosi;
		SlabPartD.aRbarData.SetSize(iRbarSize);
		arTempRbarPosi.Copy(param.arRbarPosi);
		//Get_ConvertRbarPoint4Org(RebarSect.Cym, dZslab_sta, arTempRbarPosi); // 원점 좌표로 수정.  
		Get_ConvertRbarPoint4Org(0.0, 0.5*dtc, arTempRbarPosi);                // 슬래브 바닥 중심으로 수정.  
		//Get_ConvertRbarPoint4Org(RebarSect.Cym, dZcent_slb, arTempRbarPosi); // 슬래브 도심 기준에서 원점 좌표로 수정. 

		for ( int i = 0; i < iRbarSize; i++ )
		{
			_DGN_RBAR_CRC& RbarCrc = arTempRbarPosi[i];
			T_GSEC_RBAR GsecRbar;
			GsecRbar.dEs = dEr;
			GsecRbar.dDs = dDr;
			Get_ConvertRbarCrcToGsecRbar(RbarCrc, GsecRbar);
			SlabPartD.aRbarData.SetAt(i, GsecRbar);
		}
	}

	if ( bPositiveM ) // Slab + Rebar
	{
		SlabSectInfoD.aSectPart.Add(SlabPartD);
		SlabSectInfoD.StiffMainD.Stiffness = SlabOrgStiffD;
		SlabSectInfoD.StiffMainD.dYbar = SlabOrgStiffD.Cym;
		SlabSectInfoD.StiffMainD.dZbar = dZcent_slb;
		CSectUtil::Get_GeneralSectProperty(bScaleFactor, SlabSectInfoD, dScaleFactor, SlabSectPropD);
		SlabRbarStiffD = SlabSectPropD.aSectProp[0].StiffRbar.Stiffness;
		SlabPartD.Stiff.Stiffness = SlabRbarStiffD;
		SlabPartD.Stiff.dYbar = SlabSectPropD.aSectProp[0].StiffRbar.dYbar;
		SlabPartD.Stiff.dZbar = SlabSectPropD.aSectProp[0].StiffRbar.dZbar + dH + dHh;
	}
	else  // Rebar
	{
		T_SECT_STIFFNESS RbarStiffD;
		CSectUtil::Get_Stiffness4Rebar(SlabPartD.aRbarData, RbarStiffD);
		RbarStiffD.Ryy = CSectUtil::Get_RebarInertia(SlabPartD.aRbarData);
		RbarStiffD.Rzz = RbarStiffD.Ryy;
		SlabPartD.Stiff.Stiffness = RbarStiffD;
		SlabPartD.Stiff.dYbar = RbarStiffD.Cym;
		SlabPartD.Stiff.dZbar = RbarStiffD.Czm + dH + dHh;
		SlabPartD.Stiff.Stiffness.Czm = RbarStiffD.Czm + dH + dHh;
	}
	SectInfoD.aSectPart.Add(SlabPartD);

	T_SECT_PROP_D SectPropD;
	CSectUtil::Get_GeneralSectProperty(bScaleFactor, SectInfoD, dScaleFactor, SectPropD);

	if ( !bCompoStiffData )
	{
		EffeStiff = SectPropD.aSectProp[0].StiffPure.Stiffness;
	}
	else
	{
		Get_StiffnessOfEffeSectCompo(bPositiveM, SectInfoD, EffeStiff); // 강성 재계산
	}

	return TRUE;
}

BOOL CDgnPlateGirder::Get_EffeSectPropBeforeDB(BOOL bIsBefore, BOOL bPositiveM, T_PLATE_GIRDER_CALC_D& param, double dfy1, double dfy2, double* aForceGd, double* aForce, double da,
	const CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&>& aStiffness, T_SECT_STIFFNESS& EffeStiff, T_DGNLINE_LBD_BR& RhoC)
{
	//double dFx = aForce[0], dMy = aForce[4], dMz = aForce[5];
	double dMy = bIsBefore ? aForceGd[4] : aForce[4];
	BOOL bPosiI = param.bPosiI;
	T_SECT_D& SectD = param.SectElem.SectD;
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dtw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dtf1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dtf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dfyt = Get_fy4Thick(dfy1, dfy2, dtf1);
	double dfyb = Get_fy4Thick(dfy1, dfy2, dtf2);
	double dfyw = Get_fy4Thick(dfy1, dfy2, dtw);

	const T_SECT_STIFFNESS& GirderStiff = aStiffness[0];
	double dAg = GirderStiff.Area;
	double dIy = GirderStiff.Ryy;
	double dIz = GirderStiff.Rzz;
	double dCym = GirderStiff.Cym;
	double dCzm = GirderStiff.Czm;

	int iStiffSize = param.arLstiPosi.GetSize();
	CArray<T_GLINE_DGN_B, T_GLINE_DGN_B&> aLineDgnBase;
	Get_LineInfo4SectDB(bPosiI, SectD, GirderStiff, param.arLstiPosi, aLineDgnBase);

	T_MATD_D& MatdD = param.MatlElem.MatdD;
	double dPoisson = MatdD.Data1.Analysis.Poisson;
	double dEs = MatdD.Data1.Analysis.Elast;

	T_DGNLINE_LBD LineLBD;
	Get_LineDgnInfo4SectDB(bIsBefore, bPositiveM, bPosiI, param.ClassBase, dfy1, dfy2, dEs, dPoisson, aForceGd, aForce, SectD, da, aStiffness, param.arLstiPosi, aLineDgnBase, LineLBD);
	RhoC.iClass = LineLBD.iClass;
	RhoC.dxi = LineLBD.dxi;
	RhoC.dRho_c = LineLBD.dRho_c;

	double dArea_eff = 0.0, dCzm_eff = 0.0, dIy_eff = 0.0;
	Get_EffectiveCzmData4Sect(bPosiI, param.ClassBase, bPositiveM, SectD, param.arLstiPosi, LineLBD, dArea_eff, dCzm_eff, dIy_eff);

	double dheight = GirderStiff.Czm + GirderStiff.Czp;
	double decc_z = GirderStiff.Czm - dCzm_eff;
	EffeStiff = GirderStiff;
	EffeStiff.Area = dArea_eff;
	EffeStiff.Czm = dCzm_eff;
	EffeStiff.Czp = dheight - dCzm_eff;
	EffeStiff.Ryy = dIy_eff;
	EffeStiff.dy1 += decc_z;
	EffeStiff.dy2 += decc_z;
	EffeStiff.dy3 += decc_z;
	EffeStiff.dy4 += decc_z;

	return TRUE;
}

double CDgnPlateGirder::Get_PlasticNA4GenSect(BOOL bPositive, T_PLATE_GIRDER_CALC_D& param, T_SECT_SECTBASE_D& SectBaseD, const T_GLINE_DGN_D& GLineDgn)
{
	int i = 0;
	// Material
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;

	double dfy = param.MatlElem.MatdD.Data1.Design.S_Fy1;
	double dfy1 = param.MatlElem.MatdD.Data1.Design.S_Fy2;
	double dfck = param.MatlElem.dfck;
	double dfsk = param.MatlElem.MatdD.MainRebarData.B_fy;
	double dfcd = dgam_c == 0.0 ? dfck : dfck / dgam_c;
	double dfsd = dgam_sr == 0.0 ? dfsk : dfsk / dgam_sr;
	double dfyd = dgam_M0 == 0.0 ? dfy : dfy / dgam_M0;

	double dCpz = 0.0; // 철근정보는 어떻게 해야하나??
	double dHt = Get_Height4Gen(param);

	double dCcDel = 0.0;
	double dTOL2 = 0.1;
	double dToler = 0.0;

	double dNeuC = 0.5*dHt;
	// 중립축 결정을 위해서. 
	BOOL bPosiForce = TRUE;
	double dNeuC1 = dHt;
	double dNeuC2 = 0.0;

	double dCompForce = 0., dTensForce = 0.0, dForce_ratio = 0.0;
	double dPe_Sum = 0., dMe_Sum = 0., dep_Sum = 0., dAps_Sum = 0.;

	double dAs_top = 0.0, dAs_bot = 0.0; // Slab
	double dAr_top = 0.0, dAr_bot = 0.0; // Rebar
	double dAg_top = 0.0, dAg_bot = 0.0; // Girder
	double aArea[4];
	double dFcc = 0.0, dFtc = 0.0; // Fcc : Force compression concrete, Ftc : Force tension concrete
	double dFcg = 0.0, dFtg = 0.0;
	double dFcg_cl = 0.0, dFtg_cl = 0.0; // cut line
	double dFcr = 0.0, dFtr = 0.0;

	// Convert
	T_SECT_SECTBASE_D CalcSectBaseD; CalcSectBaseD.Initialize();
	Get_ConvertSectBase4GenLineData(SectBaseD, GLineDgn, CalcSectBaseD);

	int iCount = 0;
	BOOL bYaxis = TRUE;
	T_GSEC_POLYGON PolyD;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aTopLine, aBotLine, aHCutLine;
	CArray<double, double&> aHCutTopThick;
	double aZcen[4];
	do
	{
		dAr_top = dAr_bot = 0.0;
		dFcg_cl = dFtg_cl = 0.0;
		PolyD.Initialize();
		aTopLine.RemoveAll();
		aBotLine.RemoveAll();
		aHCutLine.RemoveAll();
		aHCutTopThick.RemoveAll();

		double dcutZpos = bPositive ? dHt - dNeuC : dNeuC;
		CSectUtil::Get_SectPropertyOfCuttingArea(dcutZpos, CalcSectBaseD, aArea, aZcen, PolyD, aTopLine, aBotLine, aHCutLine, aHCutTopThick);
		dAs_top = aArea[0];
		dAs_bot = aArea[1];
		dAg_top = aArea[2];
		dAg_bot = aArea[3];

		// arTranRbarPosi의 정보는 도심중심인지 알아야함. 
		int iRbarSize = param.arRbarPosi.GetSize();
		for ( i = 0; i < iRbarSize; i++ )
		{
			_DGN_RBAR_CRC RbarD = param.arRbarPosi[i];
			if ( RbarD.dz - dcutZpos >= 0.0 ) dAr_top += RbarD.dArea;
			else                       dAr_bot += RbarD.dArea;
		}

		if ( bPositive )
		{
			// 정모멘트일 경우 철근은 무시. 
			dFcc = m_dAlpha * dfcd * dAs_top;
			dFtc = 0.0;
			Get_ForceLineGS(dfy, dfy1, PolyD, aTopLine, dFcg); // dFcg = dfyd * dAg_top;
			Get_ForceLineGS(dfy, dfy1, PolyD, aBotLine, dFtg); // dFtg = dfyd * dAg_bot;
			Get_TopBotForceLineGS(dfy, dfy1, PolyD, aHCutLine, aHCutTopThick, dFcg_cl, dFtg_cl);
			dFtr = dfsd * dAr_bot;
		}
		else
		{
			dFcc = m_dAlpha * dfcd * dAs_bot;
			dFtc = 0.0;
			Get_ForceLineGS(dfy, dfy1, PolyD, aBotLine, dFcg); // dFcg = dfyd * dAg_bot;
			Get_ForceLineGS(dfy, dfy1, PolyD, aTopLine, dFtg); // dFtg = dfyd * dAg_top; 
			Get_TopBotForceLineGS(dfy, dfy1, PolyD, aHCutLine, aHCutTopThick, dFtg_cl, dFcg_cl);
			dFtr = dAr_top * dfsd;
			dFcr = 0.0;//dAr_bot * dfsd;
		}
		dCompForce = dFcc + dFcg + dFcg_cl;
		dTensForce = dFtg + dFtr + dFtg_cl;
		dForce_ratio = dTensForce == 0.0 ? fabs(dCompForce / m_dZero) : fabs(dCompForce / dTensForce);
		dCcDel = dTensForce - dCompForce;
		dToler = dForce_ratio - 1.0; //(dCompForce==0.0 ? 0.0 : dCcDel/dCompForce);

		//---------------------------------------------------------------
		//PSC_FRCR_NEUT_AXIS_D NeutAxisD;
		//NeutAxisD.dx = dc_neu;
		//NeutAxisD.dCompF_c = dCc;
		//NeutAxisD.dCompF_s = dCsc;
		//NeutAxisD.dTensF_p = dTp;
		//NeutAxisD.dTensF_s = dTst;
		//NeutAxisD.dRatio   = dForce_ratio;
		//arNeutAxisInfo.Add(NeutAxisD);
		//---------------------------------------------------------------

		if ( fabs(dCcDel) < dTOL2 ) break;
		if ( iCount > m_Iteration - 1 ) break;

		if ( dCcDel >= 0.0 ) dNeuC2 = dNeuC; // 인장력이 클경우
		else            dNeuC1 = dNeuC; // 압축력이 클경우

		dNeuC = 0.5*(dNeuC1 + dNeuC2);  //Get neutral axis position for next iteration    

		++iCount;

	}
	while ( fabs(dCcDel) > dTOL2 );

	return bPositive ? dHt - dNeuC : dNeuC; // dNeuC는 압축영역이다.;
}

BOOL CDgnPlateGirder::Calc_BendingResistance(T_PLATE_GIRDER_CALC_D& param, T_PGBR_BASE& PgbrGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail)
{
	auto ElemK = param.ElemK;
	T_CLAS_BASE& ClassD = param.ClassBase;

	// 1. Material.
	T_MATD_D& MatdD = param.MatlElem.MatdD;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;
	double dfck = param.MatlElem.dfck;
	double dfsk = param.MatlElem.MatdD.MainRebarData.B_fy;
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;
	double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk / dgam_sr;

	// 2. Section.  
	T_SECT_D& SectD = param.SectElem.SectD;
	T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
	T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
	T_SECT_STIFFNESS GirderStiff;	GirderStiff.Initialize();

	BOOL bPosiI = param.bPosiI;
	GetNonComSectStiffD(param.SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(param.SectElem, bPosiI, RebarSect);
	Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);

	// Section's Dimension
	double dH = m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dTw_t = m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
	double dTF1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dTF2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dhw = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&SectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&SectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);
	double dCosA = fabs(cos(m_pDgnSectUtil->Get_Size_WebSlope(&SectD, 0, bPosiI)));

	double dfyt = Get_fy4Thick(dfy, dfy1, dTF1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTF2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;

	// 3. Force   
	double aForceGd[6] = { PgbrGd.dFx, 0.0, 0.0, 0.0, PgbrGd.dMy, PgbrGd.dMz }; // Girder
	double aForce[6] = { PgbrBase.dFx,	0.0, 0.0, 0.0, PgbrBase.dMy, PgbrBase.dMz };   // Composite 
	double dFx = PgbrBase.dFx, dMy = PgbrBase.dMy, dMz = PgbrBase.dMz;
	double dNc = 0.0, dNc_el = 0.0, dNc_f = 0.0, dMpl_Rd = 0.0, dMel_Rd = 0.0, dM_Rd = 0.0;
	double dMa_Ed = PgbrBase.dMa_Ed;
	double dMc_Ed = PgbrBase.dMy;
	double dM_Ed_c = dMa_Ed + dMc_Ed;
	BOOL bPositive = dMy > -m_dZero ? TRUE : FALSE;

	double dAlpha = m_dAlpha;
	double dtfc = bPositive ? dTF1 : dTF2;// Thick. of compression flange 
	double dbfc = bPositive ? dB1 : dB2; // Width. of compression flange 

	// 4. Decide Section Class
	BOOL bEquivalentClass2 = FALSE;
	if ( (ClassD.iFlangeClass == 1 || ClassD.iFlangeClass == 2) && ClassD.iWebClass == 3 ) { bEquivalentClass2 = TRUE; }
	int iClassSect = ClassD.iClassSect;

	// 5. Calc. PNA considering effective width of web
	double dPlasticNAEff = 0.0, dEffeLen = 0.0, dHoleLen = 0.0;

	double dPlasticNA = Get_PlasticNA(bPositive, param);
	BOOL bCheckEffPlasticNAxis = TRUE;
	if ( bEquivalentClass2 )   // EN 1993-1-1, 6.2.2.4 Effective properties of cross sections with class 3 webs and class1 or 2 flanges, 48p
	{
		bCheckEffPlasticNAxis = Get_PlasticNAEff(bPositive, param, dPlasticNA, dPlasticNAEff, dEffeLen, dHoleLen);
	}

	// 6. Check Non-linear
	double dXpl = !bEquivalentClass2 ? dHt - dPlasticNA : dHt - dPlasticNAEff;
	double dXplOverh = dHt < m_dZero ? 0.0 : dXpl / dHt;
	BOOL bNonLinear = dXplOverh > 0.4 ? TRUE : FALSE;

	// 0 = Plastic resistance moment 
	// 1 = Non-linear resistance to bending
	// 2 = Elastic resistance to bending.  

	double dN_slab = 0.0, dNg_top = 0.0, dNg_bot = 0.0, dNr_top = 0.0, dNr_bot = 0.0;
	double dNa_topc = 0.0, dNa_topt = 0.0, dNa_webc = 0.0, dNa_webt = 0.0, dNa_botc = 0.0, dNa_bott = 0.;
	double dNa_webc1 = 0.0, dNa_webc2 = 0.0, dNa_web_Effe = 0.0, dDistT = 0.0, dDistB = 0.0, dDistW = 0.0;
	double dDistTc = 0.0, dDistTt = 0.0, dDistWc = 0.0, dDistWt = 0.0, dDistBc = 0.0, dDistBt = 0.0;
	double dDistWc1 = 0.0, dDistWc2 = 0.0, dDistW1 = 0.0, dDistW2 = 0.0, dCompfsdAsidsi = 0.0, dTensfsdAsidsi = 0.0;
	double dCompfsdAsi_sum = 0.0, dTensfsdAsi_sum = 0.0;

	// 7. Calc. Plastic Bending Resistance
	double dNa_top = dB1 * dTF1*dfyd_t;
	double dNa_bot = dB2 * dTF2*dfyd_b;
	double dNa_web = SafeDiv(dhw, dCosA) * dTw_t*dfyd_w;

	if ( !bEquivalentClass2 || !bCheckEffPlasticNAxis )
	{
		if ( bPositive ) // Positive일 경우는 강도 계산할 때 압축콘크리트 중심(Xpl*0.5)에서 계산한다.
		{
			if ( dPlasticNA > dH ) // PNA in Slab
			{
				if ( dXpl > dtc )
				{
					dDistT = dHt - 0.5*dtc - (dH - 0.5*dTF1);
					dDistB = dHt - 0.5*dtc - 0.5*dTF2;
					dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
					dNc_f = dBc * dtc*dAlpha*dfcd;
				}
				else if ( dXpl < dtc )
				{
					dDistT = dHt - 0.5*dXpl - (dH - 0.5*dTF1);
					dDistB = dHt - 0.5*dXpl - 0.5*dTF2;
					dDistW = dHt - 0.5*dXpl - dTF2 - 0.5*dhw;
					dNc_f = dBc * dXpl*dAlpha*dfcd;
				}
				//
				dMpl_Rd = dNa_top * dDistT + dNa_bot * dDistB + dNa_web * dDistW;
				dN_slab = dNc_f;
				dNg_bot = dNa_top + dNa_bot + dNa_web;
			}
			else
			{
				dNc_f = dBc * dtc*dAlpha*dfcd;
				if ( dPlasticNA > dH - dTF1 ) // PNA in Top Flange
				{
					double dTfc = dH - dPlasticNA;
					double dTft = dPlasticNA - (dH - dTF1);
					dNa_topc = dB1 * dTfc*dfyd_t;
					dNa_topt = dB1 * dTft*dfyd_t;
					dDistTc = 0.5*dtc + dHh + 0.5*dTfc;
					dDistTt = 0.5*dtc + dHh + dTfc + 0.5*dTft;
					dDistB = dHt - 0.5*dtc - 0.5*dTF2;
					dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
					//
					dMpl_Rd = -dNa_topc * dDistTc + dNa_topt * dDistTt + dNa_bot * dDistB + dNa_web * dDistW;
					dN_slab = dNc_f;
					dNg_top = dNa_topc;
					dNg_bot = dNa_topt + dNa_bot + dNa_web;
				}
				else if ( dPlasticNA < dH - dTF1 && dPlasticNA > dTF2 ) // PNA in Web
				{
					double dWc = (dH - dTF1) - dPlasticNA;
					double dWt = dPlasticNA - dTF2;
					double dWc_in = SafeDiv(dWc, dCosA);
					double dWt_in = SafeDiv(dWt, dCosA);
					dNa_webc = dWc_in * dTw_t*dfyd_w;
					dNa_webt = dWt_in * dTw_t*dfyd_w;
					dDistWc = 0.5*dtc + dHh + dTF1 + 0.5*dWc;
					dDistWt = 0.5*dtc + dHh + dTF1 + dWc + 0.5*dWt;
					dDistT = dHt - 0.5*dtc - (dH - 0.5*dTF1);
					dDistB = dHt - 0.5*dtc - 0.5*dTF2;
					//
					dMpl_Rd = -dNa_top * dDistT - dNa_webc * dDistWc + dNa_webt * dDistWt + dNa_bot * dDistB;
					dN_slab = dNc_f;
					dNg_top = dNa_top + dNa_webc;
					dNg_bot = dNa_webt + dNa_bot;
				}
				else if ( dPlasticNA < dTF2 ) // PNA in Bottom Flange
				{
					double dBfc = dTF2 - dPlasticNA;
					double dBft = dPlasticNA;
					dNa_botc = dB2 * dBfc*dfyd_b;
					dNa_bott = dB2 * dBft*dfyd_b;
					dDistBc = 0.5*dtc + dHh + dTF1 + dhw + 0.5*dBfc;
					dDistBt = 0.5*dtc + dHh + dTF1 + dhw + dBfc + 0.5*dBft;
					dDistT = dHt - 0.5*dtc - (dH - 0.5*dTF1);
					dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
					//
					dMpl_Rd = -dNa_top * dDistT - dNa_web * dDistW - dNa_botc * dDistBc + dNa_bott * dDistBt;
					dN_slab = dNc_f;
					dNg_top = dNa_top + dNa_web + dNa_botc;
					dNg_bot = dNa_bott;
				}
			}
		}
		else // Negative moment, Negative일 경우는 강도 계산할 때 PlasticNA가 기준
		{
			dCompfsdAsidsi = 0.0;
			dTensfsdAsidsi = 0.0;
			dCompfsdAsi_sum = dTensfsdAsi_sum = 0.0;
			double ddsi = 0.0, dAsi = 0.0, dNAdsi = 0.0;
			double dGap = (dH + dHh + 0.5*dtc) - dPlasticNA;

			for ( int i = 0; i < param.arRbarPosi.GetSize(); i++ )
			{
				_DGN_RBAR_CRC& Rbar = param.arRbarPosi[i];
				dNAdsi = dGap + Rbar.dz;
				dAsi = Rbar.dArea;
				double dFbari = dAsi * dfsd;
				if ( dNAdsi < 0.0 )
				{
					dCompfsdAsidsi += dFbari * fabs(dNAdsi);
					dCompfsdAsi_sum += dFbari;
				}
				else
				{
					dTensfsdAsidsi += dAsi * dfsd*fabs(dNAdsi);
					dTensfsdAsi_sum += dFbari;
				}
			}
			dNr_top = dTensfsdAsi_sum;
			dNr_bot = dCompfsdAsi_sum;

			if ( dPlasticNA > dH ) // PNA in Slab
			{
				dDistT = dPlasticNA - (dH - 0.5*dTF1);
				dDistW = dPlasticNA - (0.5*dhw + dTF2);
				dDistB = dPlasticNA - 0.5*dTF2;
				//
				dMpl_Rd = dNa_top * dDistT + dNa_bot * dDistB + dNa_web * dDistW + dCompfsdAsidsi + dTensfsdAsidsi;
				dN_slab = dTensfsdAsi_sum;
				dNg_top = 0.0;
				dNg_bot = dNa_top + dNa_web + dNa_bot;
			}
			else
			{
				if ( dPlasticNA > dH - dTF1 ) // PNA in Top Flange
				{
					double dTfc = dPlasticNA - (dH - dTF1);
					double dTft = dH - dPlasticNA;
					dNa_topc = dB1 * dTfc*dfyd_t;
					dNa_topt = dB1 * dTft*dfyd_t;
					dDistTc = 0.5*dTfc;
					dDistTt = 0.5*dTft;
					dDistB = dPlasticNA - 0.5*dTF2;
					dDistW = dPlasticNA - (dTF2 + 0.5*dhw);
					//
					dMpl_Rd = dNa_topc * dDistTc + dNa_topt * dDistTt + dNa_bot * dDistB + dNa_web * dDistW + dCompfsdAsidsi + dTensfsdAsidsi;
					dN_slab = dTensfsdAsi_sum;
					dNg_top = dNa_topt;
					dNg_bot = dNa_topc + dNa_web + dNa_bot;
				}
				else if ( dPlasticNA < dH - dTF1 && dPlasticNA > dTF2 ) // PNA in Web
				{
					double dWc = dPlasticNA - dTF2;
					double dWt = (dH - dTF1) - dPlasticNA;
					double dWc_in = SafeDiv(dWc, dCosA);
					double dWt_in = SafeDiv(dWt, dCosA);
					dNa_webc = dWc_in * dTw_t*dfyd_w;
					dNa_webt = dWt_in * dTw_t*dfyd_w;
					dDistWc = 0.5*dWc;
					dDistWt = 0.5*dWt;
					dDistT = (dH - 0.5*dTF1) - dPlasticNA;
					dDistB = dPlasticNA - 0.5*dTF2;
					//
					dMpl_Rd = dNa_top * dDistT + dNa_webc * dDistWc + dNa_webt * dDistWt + dNa_bot * dDistB + dCompfsdAsidsi + dTensfsdAsidsi;
					dN_slab = dTensfsdAsi_sum;
					dNg_top = dNa_top + dNa_webt;
					dNg_bot = dNa_webc + dNa_bot;
				}
				else if ( dPlasticNA < dTF2 ) // PNA in Bottom Flange
				{
					double dBfc = dPlasticNA;
					double dBft = dTF2 - dPlasticNA;
					dNa_botc = dB2 * dBfc*dfyd_b;
					dNa_bott = dB2 * dBft*dfyd_b;
					dDistBc = 0.5*dBfc;
					dDistBt = 0.5*dBft;
					dDistT = (dH - 0.5*dTF1) - dPlasticNA;
					dDistW = (dTF2 + 0.5*dhw) - dPlasticNA;
					//
					dMpl_Rd = dNa_top * dDistT + dNa_web * dDistW + dNa_botc * dDistBc + dNa_bott * dDistBt + dCompfsdAsidsi + dTensfsdAsidsi;
					dN_slab = dTensfsdAsi_sum;
					dNg_top = dNa_top + dNa_web + dNa_bott;
					dNg_bot = dNa_botc;
				}
			}
		}
	}
	else // needed to considering effective depth of web
	{
		if ( bPositive ) // Positive일 경우는 강도 계산할 때 압축콘크리트 중심(Xpl*0.5)에서 계산한다.
		{
			if ( dPlasticNAEff > dH ) // PNA in Slab
			{
				if ( dXpl > dtc )
				{
					dDistT = dHt - 0.5*dtc - (dH - 0.5*dTF1);
					dDistB = dHt - 0.5*dtc - 0.5*dTF2;
					dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
					dNc_f = dBc * dtc*dAlpha*dfcd;
				}
				else if ( dXpl < dtc )
				{
					dDistT = dHt - 0.5*dXpl - (dH - 0.5*dTF1);
					dDistB = dHt - 0.5*dXpl - 0.5*dTF2;
					dDistW = dHt - 0.5*dXpl - dTF2 - 0.5*dhw;
					dNc_f = dBc * dXpl*dAlpha*dfcd;
				}
				//
				dMpl_Rd = dNa_top * dDistT + dNa_bot * dDistB + dNa_web * dDistW;
				dN_slab = dNc_f;
				dNg_bot = dNa_top + dNa_bot + dNa_web;
			}
			else
			{
				dNc_f = dBc * dtc*dAlpha*dfcd;

				if ( dPlasticNAEff > dH - dTF1 ) // PNA in Top Flange
				{
					double dTfc = dH - dPlasticNAEff;
					double dTft = dPlasticNAEff - (dH - dTF1);
					dNa_topc = dB1 * dTfc*dfyd_t;
					dNa_topt = dB1 * dTft*dfyd_t;
					dDistTc = 0.5*dtc + dHh + 0.5*dTfc;
					dDistTt = 0.5*dtc + dHh + dTfc + 0.5*dTft;
					dDistB = dHt - 0.5*dtc - 0.5*dTF2;
					dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
					//
					dMpl_Rd = -dNa_topc * dDistTc + dNa_topt * dDistTt + dNa_bot * dDistB + dNa_web * dDistW;
					dN_slab = dNc_f;
					dNg_top = dNa_topc;
					dNg_bot = dNa_topt + dNa_bot + dNa_web;
				}
				else if ( dPlasticNAEff < dH - dTF1 && dPlasticNAEff > dTF2 ) // PNA in Web
				{
					double dWc1 = dEffeLen;
					double dWc2 = dEffeLen;
					double dWt = dhw - dEffeLen * 2.0 - dHoleLen;
					dNa_webc1 = dWc1 * dTw_t*dfyd_w;
					dNa_webc2 = dWc1 * dTw_t*dfyd_w;
					dNa_webt = dWt * dTw_t*dfyd_w;
					dDistWc1 = 0.5*dtc + dHh + dTF1 + 0.5*dWc1;
					dDistWc2 = 0.5*dtc + dHh + dTF1 + dWc1 + dHoleLen + 0.5*dWc2;
					dDistWt = 0.5*dtc + dHh + dTF1 + dWc1 + dWc2 + dHoleLen + 0.5*dWt;
					dDistT = dHt - 0.5*dtc - (dH - 0.5*dTF1);
					dDistB = dHt - 0.5*dtc - 0.5*dTF2;
					//
					dMpl_Rd = -dNa_top * dDistT - dNa_webc1 * dDistWc1 - dNa_webc2 * dDistWc2 + dNa_webt * dDistWt + dNa_bot * dDistB;
					dN_slab = dNc_f;
					dNg_top = dNa_top + dNa_webc1 + dNa_webc2;
					dNg_bot = dNa_webt + dNa_bot;
				}
				else if ( dPlasticNAEff < dTF2 ) // PNA in Bottom Flange
				{
					double dBfc = dTF2 - dPlasticNAEff;
					double dBft = dPlasticNAEff;
					dNa_botc = dB2 * dBfc*dfyd_b;
					dNa_bott = dB2 * dBft*dfyd_b;
					dDistBc = 0.5*dtc + dHh + dTF1 + dhw + 0.5*dBfc;
					dDistBt = 0.5*dtc + dHh + dTF1 + dhw + dBfc + 0.5*dBft;
					dDistT = dHt - 0.5*dtc - (dH - 0.5*dTF1);
					dNa_web_Effe = dTw_t * dEffeLen*dfyd_w;
					dDistW1 = 0.5*dtc + dHh + 0.5*dEffeLen;
					dDistW2 = 0.5*dtc + dHh + dEffeLen + dHoleLen + 0.5*dEffeLen;
					//
					dMpl_Rd = -dNa_top * dDistT - dNa_web_Effe * dDistW1 - dNa_web_Effe * dDistW2 - dNa_botc * dDistBc + dNa_bott * dDistBt;
					dN_slab = dNc_f;
					dNg_top = dNa_top + dNa_web_Effe + dNa_web_Effe + dNa_botc;
					dNg_bot = dNa_bott;
				}
			}
		}
		else // Negative moment, Negative일 경우는 강도 계산할 때 PlasticNA가 기준
		{
			dCompfsdAsidsi = dTensfsdAsidsi = 0.0;
			dCompfsdAsi_sum = dTensfsdAsi_sum = 0.0;
			double ddsi = 0.0;
			double dAsi = 0.0;
			double dNAdsi = 0.0;
			double dGap = (dH + dHh + 0.5*dtc) - dPlasticNAEff;
			double dFbari = 0.0;

			for ( int i = 0; i < param.arRbarPosi.GetSize(); i++ )
			{
				const _DGN_RBAR_CRC& crRbar = param.arRbarPosi[i];
				dNAdsi = dGap + crRbar.dz;
				dAsi = crRbar.dArea;
				dFbari = dAsi * dfsd;

				if ( dNAdsi < 0.0 )
				{
					dCompfsdAsidsi += dFbari * fabs(dNAdsi);
					dCompfsdAsi_sum += dFbari;
				}
				else
				{
					dTensfsdAsidsi += dAsi * dfsd*fabs(dNAdsi);
					dTensfsdAsi_sum += dFbari;
				}
			}
			dNr_top = dTensfsdAsi_sum;
			dNr_bot = dCompfsdAsi_sum;

			if ( dPlasticNAEff > dH ) // PNA in Slab
			{
				dDistT = dPlasticNAEff - (dH - 0.5*dTF1);
				dDistW1 = dPlasticNAEff - (0.5*dEffeLen + dTF2);
				dDistW2 = dPlasticNAEff - (+0.5*dEffeLen + dHoleLen + dEffeLen + dTF2);
				dDistB = dPlasticNAEff - 0.5*dTF2;
				dNa_web_Effe = dEffeLen * dTw_t*dfyd_w;
				//
				dMpl_Rd = dNa_top * dDistT + dNa_bot * dDistB + dNa_web_Effe * dDistW1 + dNa_web_Effe * dDistW2 + dCompfsdAsidsi + dTensfsdAsidsi;
				dN_slab = dTensfsdAsi_sum;
				dNg_top = 0.0;
				dNg_bot = dNa_top + dNa_bot + dNa_web_Effe + dNa_web_Effe;
			}
			else
			{
				if ( dPlasticNAEff > dH - dTF1 ) // PNA in Top Flange
				{
					double dTfc = dPlasticNAEff - (dH - dTF1);
					double dTft = dH - dPlasticNAEff;
					dNa_topc = dB1 * dTfc*dfyd_t;
					dNa_topt = dB1 * dTft*dfyd_t;
					dDistTc = 0.5*dTfc;
					dDistTt = 0.5*dTft;
					dDistB = dPlasticNAEff - 0.5*dTF2;
					dDistW1 = dPlasticNAEff - (0.5*dEffeLen + dTF2);
					dDistW2 = dPlasticNAEff - (+0.5*dEffeLen + dHoleLen + dEffeLen + dTF2);
					dNa_web_Effe = dEffeLen * dTw_t*dfyd_w;
					//
					dMpl_Rd = dNa_topc * dDistTc + dNa_topt * dDistTt + dNa_bot * dDistB + dNa_web_Effe * dDistW1 + dNa_web_Effe * dDistW2 + dCompfsdAsidsi + dTensfsdAsidsi;
					dN_slab = dTensfsdAsi_sum;
					dNg_top = dNa_topt;
					dNg_bot = dNa_topc + dNa_bot + dNa_web_Effe + dNa_web_Effe;
				}
				else if ( dPlasticNAEff < dH - dTF1 && dPlasticNAEff > dTF2 ) // PNA in Web
				{
					double dWc1 = dEffeLen;
					double dWc2 = dEffeLen;
					double dWt = (dH - dTF1) - dPlasticNAEff;
					dNa_webc1 = dWc1 * dTw_t*dfyd_w;
					dNa_webc2 = dWc2 * dTw_t*dfyd_w;
					dNa_webt = dWt * dTw_t*dfyd_w;
					dDistWc1 = 0.5*dWc1;
					dDistWc2 = dWc1 + dHoleLen + dWc2 * 0.5;
					dDistWt = 0.5*dWt;
					dDistT = (dH - 0.5*dTF1) - dPlasticNAEff;
					dDistB = dPlasticNAEff - 0.5*dTF2;
					//
					dMpl_Rd = dNa_top * dDistT + dNa_webc1 * dDistWc1 + dNa_webc2 * dDistWc2 + dNa_webt * dDistWt + dNa_bot * dDistB + dCompfsdAsidsi + dTensfsdAsidsi;
					dN_slab = dTensfsdAsi_sum;
					dNg_top = dNa_top + dNa_webt;
					dNg_bot = dNa_webc1 + dNa_webc2 + dNa_bot;
				}
				else if ( dPlasticNAEff < dTF2 ) // PNA in Bottom Flange
				{
					double dBfc = dPlasticNAEff;
					double dBft = dTF2 - dPlasticNAEff;
					dNa_botc = dB2 * dBfc*dfyd_b;
					dNa_bott = dB2 * dBft*dfyd_b;
					dDistBc = 0.5*dBfc;
					dDistBt = 0.5*dBft;
					dDistT = (dH - 0.5*dTF1) - dPlasticNAEff;
					dDistW = (dTF2 + 0.5*dhw) - dPlasticNAEff;
					//
					dMpl_Rd = dNa_top * dDistT + dNa_web * dDistW + dNa_botc * dDistBc + dNa_bott * dDistBt + dCompfsdAsidsi + dTensfsdAsidsi;
					dN_slab = dTensfsdAsi_sum;
					dNg_top = dNa_top + dNa_web + dNa_bott;
					dNg_bot = dNa_botc;
				}
			}
		}
	}

	// 8. For shear resistance
	double dMpl_Rd_s = 0.0, dMf_Rd = 0.0;
	BOOL bNeglectWeb = TRUE;
	if ( bEquivalentClass2 )
	{
		Calc_PlasticResistance(param, PgbrBase, dPlasticNA, dMpl_Rd_s);
	}
	else
	{
		dMpl_Rd_s = dMpl_Rd;
	}
	//
	double dPlasticNA_NeglWeb = Get_PlasticNA(bPositive, param, bNeglectWeb);
	Calc_PlasticResistance(param, PgbrBase, dPlasticNA_NeglWeb, dMf_Rd, bNeglectWeb);
	PgbrBase.dMf_Rd = dMf_Rd;
	PgbrBase.dMpl_Rd_s = dMpl_Rd_s;

	// Consider EC3, 6.2.1.2(2)
	BOOL bS420orS460 = IsMaterialS420orS460(dfy);
	double dBeta = 1.0;
	if ( bPositive && bS420orS460 )
	{
		if ( !Get_BetaReductionFactor(dXpl, dHt, dBeta) ) { dBeta = 1.0; }
	}

	// Beta 적용은 정모멘트만.
	PgbrBase.dMpl_Rd = dMpl_Rd;	 // Modify by GAY. PMS:4216. ('12.05.15). M_Rd = Beta*M_pl_Rd.
	PgbrBase.dNc_f = dNc_f;
	PgbrBase.dNc = dFx;       // Nc       
	PgbrBase.dNc_el = 0.0;       // Nc,el    

	// Elastic Resistance
	Calc_ElasticResistance(bPositive, aForceGd, aForce, param, PgbrBase, PgbrDetail);

	// Non-linear Resistance
	if ( bPositive && bS420orS460 && bNonLinear )
	{
		Calc_NonLinearResistance(ElemK, bPosiI, PgbrBase);
	}
	else
	{
		if ( iClassSect == 1 || iClassSect == 2 )
		{
			PgbrBase.dM_Rd = dBeta * PgbrBase.dMpl_Rd;
		}
		else
		{
			PgbrBase.dM_Rd = fabs(PgbrBase.dMel_Rd);

			if ( PgbrDetail.nKminType == Kmin_Slab || PgbrDetail.nKminType == Kmin_Rbar ) dM_Ed_c = dMc_Ed;
			if ( dM_Ed_c * PgbrBase.dMel_Rd < 0.0 ) { dM_Ed_c = 0.0; } // MQC 13393.
		}
	}

	// 9. Save Result
	PgbrBase.iTopClass = ClassD.iTopFlgClass;
	PgbrBase.iBotClass = ClassD.iBotFlgClass;
	PgbrBase.iWebClass = ClassD.iWebClass;
	PgbrBase.iSectClass = ClassD.iClassSect;
	PgbrBase.bEquiClass2 = bEquivalentClass2;
	PgbrBase.dPlasticNA = dPlasticNA;
	PgbrBase.dPlasticNAEff = dPlasticNAEff;
	PgbrBase.dEffeLen = dEffeLen;
	PgbrBase.dHoleLen = dHoleLen;
	PgbrBase.dMel_Rd = fabs(PgbrBase.dMel_Rd);
	PgbrBase.dRatio = fabs(PgbrBase.dM_Rd) < m_dZero ? fabs(dM_Ed_c) / m_dZero : fabs(dM_Ed_c) / fabs(PgbrBase.dM_Rd);
	PgbrBase.bOK = PgbrBase.dRatio < 1.0 ? TRUE : FALSE;
	PgbrBase.dh = dHt;
	PgbrBase.dXpl = dXpl;
	PgbrBase.dXplOverh = dXplOverh;
	PgbrBase.dAlpha = dAlpha;
	PgbrBase.dNa_top = dNa_top;
	PgbrBase.dNa_web = dNa_web;
	PgbrBase.dNa_bot = dNa_bot;
	PgbrBase.dNa_topc = dNa_topc;
	PgbrBase.dNa_topt = dNa_topt;
	PgbrBase.dNa_webc = dNa_webc;
	PgbrBase.dNa_webt = dNa_webt;
	PgbrBase.dNa_botc = dNa_botc;
	PgbrBase.dNa_bott = dNa_bott;
	PgbrBase.dNa_webc1 = dNa_webc1;
	PgbrBase.dNa_webc2 = dNa_webc2;
	PgbrBase.dNa_web_Effe = dNa_web_Effe;
	PgbrBase.dDistT = dDistT;
	PgbrBase.dDistB = dDistB;
	PgbrBase.dDistW = dDistW;
	PgbrBase.dDistTc = dDistTc;
	PgbrBase.dDistTt = dDistTt;
	PgbrBase.dDistWc = dDistWc;
	PgbrBase.dDistWt = dDistWt;
	PgbrBase.dDistBc = dDistBc;
	PgbrBase.dDistBt = dDistBt;
	PgbrBase.dDistWc1 = dDistWc1;
	PgbrBase.dDistWc2 = dDistWc2;
	PgbrBase.dDistW1 = dDistW1;
	PgbrBase.dDistW2 = dDistW2;
	PgbrBase.dCompfsdAsidsi = dCompfsdAsidsi;
	PgbrBase.dTensfsdAsidsi = dTensfsdAsidsi;
	PgbrBase.dBeta = dBeta;

	// 10. For Design Result Print
	if ( IsPrintMode() )
	{
		T_STRESS_INFO_I StressInfoD;
		T_SECT_STIFFNESS SectStiffD = bPositive ? RebarSect : NonConSect;
		GetStressInfo(bPosiI, SectD, aForceGd, aForce, GirderStiff, SectStiffD, StressInfoD);

		PgbrDetail.StressInfoHshapeD = StressInfoD;
		PgbrDetail.dMpl_Rd_cal = dMpl_Rd;
		if ( bPositive )
		{
			PgbrDetail.dN_slab = dN_slab;
		}
		else
		{
			PgbrDetail.dN_slab = 0.0;
		}
		PgbrDetail.dNr_top = dNr_top;
		PgbrDetail.dNr_bot = dNr_bot;
		PgbrDetail.dNg_top = dNg_top;
		PgbrDetail.dNg_bot = dNg_bot;
		PgbrDetail.dPlasticNA_NeglWeb = dPlasticNA_NeglWeb;
	}
	return TRUE;
}
BOOL CDgnPlateGirder::Get_EffectiveSectionProperty4DB(BOOL bIsBefore, BOOL bPositiveM, double* aForceGd, double* aForce, double da, T_PLATE_GIRDER_CALC_D& param, T_SECT_STIFFNESS& EffeStlSect, T_SECT_STIFFNESS& EffeComSect,
	T_DGNLINE_LBD_BR& RhocBefore, T_DGNLINE_LBD_BR& RhocAfter,
	BOOL bCompoStiffData/*=FALSE*/)
{
	EffeStlSect.Initialize();
	EffeComSect.Initialize();

	T_CLAS_BASE& ClassD = param.ClassBase;

	// 1. Material
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;

	T_MATD_D& MatdD = param.MatlElem.MatdD;
	double dfy1 = MatdD.Data1.Design.S_Fy1;
	double dfy2 = MatdD.Data1.Design.S_Fy2;
	double dfck = param.MatlElem.dfck;
	double dfsk = MatdD.MainRebarData.B_fy;
	double dfcd = fabs(dgam_c) < m_dZero ? 0.0 : dfck / dgam_c;
	double dfsd = fabs(dgam_sr) < m_dZero ? 0.0 : dfsk / dgam_sr;
	double dFx = aForce[0];
	double dMy = aForce[4];
	double dMz = aForce[5];
	double dMc_Ed = dMy;

	// 2. Section.  
	T_SECT_D& SectD = param.SectElem.SectD;
	T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
	T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
	T_SECT_STIFFNESS GirderStiff; GirderStiff.Initialize();
	BOOL bPosiI = param.bPosiI;
	GetNonComSectStiffD(param.SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(param.SectElem, bPosiI, RebarSect);
	Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);

	T_DGNLINE_LBD_BR RhoC_Before;
	T_SECT_STIFFNESS GirderEffeStiffD;
	double dPoisson = MatdD.Data1.Analysis.Poisson;
	double dEs = MatdD.Data1.Analysis.Elast;
	CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&> aStiffness;
	aStiffness.SetSize(3);
	aStiffness.SetAt(0, GirderStiff);
	aStiffness.SetAt(1, RebarSect);
	aStiffness.SetAt(2, NonConSect);
	Get_EffeSectPropBeforeDB(bIsBefore, bPositiveM, param, dfy1, dfy2, aForceGd, aForce, da, aStiffness, GirderEffeStiffD, RhoC_Before);

	T_SECT_STIFFNESS CompoStiff;  CompoStiff.Initialize();
	if ( bPositiveM ) { CompoStiff = RebarSect; }
	else { CompoStiff = NonConSect; }

	T_DGNLINE_LBD_BR RhoC_After;
	T_SECT_STIFFNESS CompoEffeStiffD;
	Get_EffeSectPropAfterDB(bIsBefore, bPositiveM, dfy1, dfy2, aForceGd, aForce, da, param, CompoEffeStiffD, RhoC_After, bCompoStiffData);

	if ( ClassD.iClassSect == 4 )
	{
		EffeStlSect = GirderEffeStiffD;
		EffeComSect = CompoEffeStiffD;
	}
	else
	{
		EffeStlSect = GirderStiff;
		EffeComSect = CompoStiff;
	}

	RhocBefore = RhoC_Before;
	RhocAfter = RhoC_After;

	return TRUE;
}

BOOL CDgnPlateGirder::Calc_ClassifyClassSection4GenSect(BOOL bChkRestrained, double dfy1, double dfy2, double* aGirderForce, double* dForce, T_SECT_STIFFNESS& GirStiffD, T_SECT_STIFFNESS& CptStiffD, T_SECT_STIFFNESS& OrgStiffD, T_PLATE_GIRDER_CALC_D& param)
{
	T_GSEC_POLYGON GsecPolyD = param.GLineData.PolyD;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
	aLine.Copy(param.GLineData.aLine);
	int iGLineSize = aLine.GetSize();
	if ( iGLineSize <= 0 ) return FALSE;

	CArray<T_GLINE_PART_DGN, T_GLINE_PART_DGN&> aPartLine;
	aPartLine.Copy(param.GLineDgn.aPartLine);
	int iPartSize = aPartLine.GetSize();
	param.DLineCalcD.aPartLine.SetSize(iPartSize);

	int i = 0;
	int iSectClass = 0;
	int iWebClass = 0;
	int iFlangeClass = 0;
	int iFlangeTopClass = 0;
	int iFlangeBotClass = 0;
	int iWebCount = 0, iFlangeCount = 0;
	int iFlgTopCount = 0, iFlgBotCount = 0;

	// 
	if ( bChkRestrained )
	{
		for ( i = 0; i < iPartSize; i++ )
		{
			T_DLINE_PART_D DLinePartD;
			T_GLINE_PART_DGN PartDgnD = aPartLine[i];
			DLinePartD.nPartType = PartDgnD.nPartType;
			int iTempBotClass = 1;
			Calc_PartLineClassSection4GenSect(dfy1, dfy2, iTempBotClass, aGirderForce, dForce, param.dPlasticNA, GirStiffD, CptStiffD, OrgStiffD, param.GLineData, PartDgnD, DLinePartD);
			param.DLineCalcD.aPartLine.SetAt(i, DLinePartD);
			int nPartType = PartDgnD.nPartType;
			int iPartClass = DLinePartD.iClass;
			if ( nPartType == 2 )// web
			{
				iWebCount++;
				if ( iWebCount == 1 ) iWebClass = iPartClass;
				else
				{
					if ( iWebClass < iPartClass ) iWebClass = iPartClass;
				}
			}
			else if ( nPartType == 0 )
			{
				iFlgTopCount++;
				if ( iFlgTopCount == 1 ) iFlangeTopClass = iPartClass;
				else
				{
					if ( iFlangeTopClass < iPartClass ) iFlangeTopClass = iPartClass;
				}
			}
			else if ( nPartType == 1 )
			{
				iFlgBotCount++;
				if ( iFlgBotCount == 1 ) iFlangeBotClass = iPartClass;
				else
				{
					if ( iFlangeBotClass < iPartClass ) iFlangeBotClass = iPartClass;
				}
			}
		}
		if ( bChkRestrained ) iFlangeTopClass = 1;
	}
	else
	{
		// Bottom
		for ( i = 0; i < iPartSize; i++ )
		{
			T_DLINE_PART_D DLinePartD;
			T_GLINE_PART_DGN PartDgnD = aPartLine[i];
			int nPartType = PartDgnD.nPartType;
			DLinePartD.nPartType = nPartType;
			if ( nPartType != 1 ) continue;
			int iTempBotClass = 1;
			Calc_PartLineClassSection4GenSect(dfy1, dfy2, iTempBotClass, aGirderForce, dForce, param.dPlasticNA, GirStiffD, CptStiffD, OrgStiffD, param.GLineData, PartDgnD, DLinePartD);
			param.DLineCalcD.aPartLine.SetAt(i, DLinePartD);
			int iPartClass = DLinePartD.iClass;
			iFlgBotCount++;
			if ( iFlgBotCount == 1 ) iFlangeBotClass = iPartClass;
			else
			{
				if ( iFlangeBotClass < iPartClass ) iFlangeBotClass = iPartClass;
			}
		}
		// Web
		for ( i = 0; i < iPartSize; i++ )
		{
			T_DLINE_PART_D DLinePartD;
			T_GLINE_PART_DGN PartDgnD = aPartLine[i];
			int nPartType = PartDgnD.nPartType;
			DLinePartD.nPartType = nPartType;
			if ( nPartType != 2 ) continue;
			Calc_PartLineClassSection4GenSect(dfy1, dfy2, iFlangeBotClass, aGirderForce, dForce, param.dPlasticNA, GirStiffD, CptStiffD, OrgStiffD, param.GLineData, PartDgnD, DLinePartD);
			param.DLineCalcD.aPartLine.SetAt(i, DLinePartD);
			int iPartClass = DLinePartD.iClass;
			iWebCount++;
			if ( iWebCount == 1 ) iWebClass = iPartClass;
			else
			{
				if ( iWebClass < iPartClass ) iWebClass = iPartClass;
			}
		}
		// Top
		for ( i = 0; i < iPartSize; i++ )
		{
			T_DLINE_PART_D DLinePartD;
			T_GLINE_PART_DGN PartDgnD = aPartLine[i];
			int nPartType = PartDgnD.nPartType;
			DLinePartD.nPartType = nPartType;
			if ( nPartType != 0 ) continue;
			Calc_PartLineClassSection4GenSect(dfy1, dfy2, iFlangeBotClass, aGirderForce, dForce, param.dPlasticNA, GirStiffD, CptStiffD, OrgStiffD, param.GLineData, PartDgnD, DLinePartD);
			param.DLineCalcD.aPartLine.SetAt(i, DLinePartD);
			int iPartClass = DLinePartD.iClass;
			iFlgTopCount++;
			if ( iFlgTopCount == 1 ) iFlangeTopClass = iPartClass;
			else
			{
				if ( iFlangeTopClass < iPartClass ) iFlangeTopClass = iPartClass;
			}
		}
		// MQC-14064, 정모멘트,부모멘트 상관없이 Top flange class는 1로 고정한다.
		iFlangeTopClass = 1;
	}

	//
	iFlangeClass = max(iFlangeTopClass, iFlangeBotClass);
	iSectClass = max(iWebClass, iFlangeClass);

	//
	param.DLineCalcD.iClass = iSectClass;
	param.DLineCalcD.iClassFlg = iFlangeClass;
	param.DLineCalcD.iClassWeb = iWebClass;
	param.DLineCalcD.iClassFlgTop = iFlangeTopClass;
	param.DLineCalcD.iClassFlgBot = iFlangeBotClass;

	return TRUE;
}
BOOL CDgnPlateGirder::Get_ClassifyClassSection(T_PLATE_GIRDER_CALC_D& param, double aGirderForce[6], double aCptForce[6])
{
	// 1. Material.
	T_MATD_D& MatdD = param.MatlElem.MatdD;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;

	BOOL bPosiI = param.bPosiI;
	// 2. Section.  
	T_SECT_D& SectD = param.SectElem.SectD;
	BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
	int nSectType = m_pDgnSectUtil->Get_RealSectType(&SectD);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
	double dTF1 = m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
	double dTF2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&SectD);
	double dgamma_s = m_dGamma_M0;

	T_SECT_SECTBASE_D& SectBase = bTapered && !bPosiI ? SectD.CmpTapJ : SectD.SectBefore.SectI;
	T_SECT_STIFFNESS GirderStiff, PureSect, NonConSect, RebarSect;
	GirderStiff.Initialize(); PureSect.Initialize(); NonConSect.Initialize(); RebarSect.Initialize();
	Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);
	GetPureSectStiffD(param.SectElem, bPosiI, PureSect);
	GetNonComSectStiffD(param.SectElem, bPosiI, NonConSect);
	GetRebarSectStiffD(param.SectElem, bPosiI, RebarSect);

	// 3. Force
	double dFx = aCptForce[0];
	double dMy = aCptForce[4];
	double dMz = aCptForce[5];

	// 정모멘트만 1994-2:2005, 5.5.2(1) 검토
	BOOL bPositiveMy = dMy > -m_dZero ? TRUE : FALSE;
	double dtfc = bPositiveMy ? dTF1 : dTF2; // Thick of Compression Flange
	double dbfc = bPositiveMy ? dB1 : dB2; //  Width of Compression Flange
	BOOL bChkRestrained = bPositiveMy;

	// Shear connector.
	const CPG_SCON_POSI& SconPosi = bPosiI ? param.SconElem.SconPosi[0] : param.SconElem.SconPosi[1];
	BOOL bShearConn = SconPosi.bUseShear; // Use Shear Connector (default=TRUE)
	int iSconNum = SconPosi.iNum;      // Number of connectors(transverse)  
	double dSconSt = SconPosi.dSt;       // C to C  
	double dSconSpace = SconPosi.dSpace;    // Longitudinal spacing 
	double dSconDia = SconPosi.dDia;

	//EN 1994-2:2005 5.5.2(1) , EN 1994-2:2005 6.6.5.5 
	BOOL bSconCtoCLimit = FALSE;
	BOOL bSconClearDistLimit = FALSE;
	BOOL bSconLongCtoCLimit = FALSE;
	double dSconCtoCLt = (dfy < m_dZero) ? 0.0 : 22.0*dtfc*sqrt(235. / dfy);
	double dSconClearDistLt = (dfy < m_dZero) ? 0.0 : 9.0*dtfc*sqrt(235. / dfy);
	double dSconLongCtoCLt = min(4.0*dtc, 800.0);

	// Is steel compression flange restrained by shear connectors to clause 5.5.2(1)?
	if ( bChkRestrained )
	{
		if ( dSconSt < dSconCtoCLt ) { bSconCtoCLimit = TRUE; }
		if ( dSconSpace < dSconLongCtoCLt ) { bSconLongCtoCLimit = TRUE; }

		double dSconClearDist = (dbfc - ((iSconNum - 1)*dSconSt + dSconDia))*0.5;
		if ( dSconClearDist < dSconClearDistLt ) { bSconClearDistLimit = TRUE; }
	}

	T_SECT_STIFFNESS SectStiffD = bPositiveMy ? RebarSect : NonConSect;
	BOOL bEffeSection = FALSE;
	BOOL bFlangeDeter = TRUE; // Modify, Jaeoh.(08.03.14) 이대근 CJ님 요청으로 합성단면에서 콘크리트 슬래브에 연결되는 top flange는 자동으로 class 1으로 분류

	param.ClassBase.Initialize();
	param.DLineCalcD.Initialize();
	if ( nSectType == D_SECT_TYPE_COMPO_G )
	{
		param.dPlasticNA = Get_PlasticNA4GenSect(bPositiveMy, param, SectBase, param.GLineDgn);
		Calc_ClassifyClassSection4GenSect(bChkRestrained, dfy, dfy1, aGirderForce, aCptForce, GirderStiff, SectStiffD, PureSect, param);
		param.ClassBase.iFlangeClass = param.DLineCalcD.iClassFlg;
		param.ClassBase.iTopFlgClass = param.DLineCalcD.iClassFlgTop;
		param.ClassBase.iBotFlgClass = param.DLineCalcD.iClassFlgBot;
		param.ClassBase.iWebClass = param.DLineCalcD.iClassWeb;
		param.ClassBase.iClassSect = param.DLineCalcD.iClass;

		param.aWebLineId.RemoveAll();
		param.aTopFlgLineId.RemoveAll();
		param.aBotFlgLineId.RemoveAll();
		Get_PartLineId(2, param.GLineDgn, param.aWebLineId); // Web
		Get_PartLineId(0, param.GLineDgn, param.aTopFlgLineId); // Top
		Get_PartLineId(1, param.GLineDgn, param.aBotFlgLineId); // Bottom
	}
	else
	{
		param.dPlasticNA = Get_PlasticNA(bPositiveMy, param);

		T_CLASS_IN_D ClassInD;
		Make_ClassInD(bPosiI, aGirderForce, aCptForce, param.SectElem, ClassInD);
		// MQC-14064, 정모멘트,부모멘트 상관없이 Top flange class는 1로 고정한다.
		BOOL bTopRestrained = TRUE;
		if ( !Get_SectionClass(param, bTopRestrained, ClassInD) )
		{
			ASSERT(0);
			return FALSE;
		}
	}

	return TRUE;
}
double CDgnPlateGirder::Get_PlasticNA(BOOL bPositive, T_PLATE_GIRDER_CALC_D& param, BOOL bNeglectWeb)
{
	T_SECT_D& crSectD = param.SectElem.SectD;
	// 1. Initialize
	double dPlasticNA = 0.0;

	BOOL bPosiI = param.bPosiI;
	// 2. Section
	double dH = m_pDgnSectUtil->Get_Size_H(&crSectD, bPosiI);
	double dB1 = m_pDgnSectUtil->Get_Size_bft(&crSectD, bPosiI);
	double dTw = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
	double dTw_t = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
	double dTf1 = m_pDgnSectUtil->Get_Size_tft(&crSectD, bPosiI);
	double dB2 = m_pDgnSectUtil->Get_Size_bfb(&crSectD, bPosiI);
	double dTf2 = m_pDgnSectUtil->Get_Size_tfb(&crSectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&crSectD, bPosiI);
	double dhw_in = m_pDgnSectUtil->Get_Size_Inclind_hw(&crSectD, 0, bPosiI);
	double dBc = m_pDgnSectUtil->Get_Size_Bc(&crSectD);
	double dtc = m_pDgnSectUtil->Get_Size_tc(&crSectD);
	double dHh = m_pDgnSectUtil->Get_Size_Hh(&crSectD);
	double dHt = m_pDgnSectUtil->Get_Size_Ht(&crSectD, bPosiI);
	double dCosA = fabs(cos(m_pDgnSectUtil->Get_Size_WebSlope(&crSectD, 0, bPosiI)));

	// 3. Material
	double dgam_M0 = m_dGamma_M0;
	double dgam_sr = m_dGamma_S;
	double dgam_c = m_dGamma_C;
	double dfy = param.MatlElem.MatdD.Data1.Design.S_Fy1;
	double dfy1 = param.MatlElem.MatdD.Data1.Design.S_Fy2;
	double dfck = param.MatlElem.dfck;
	double dfsk = param.MatlElem.MatdD.MainRebarData.B_fy;
	double dfcd = fabs(dgam_c) < m_dZero ? dfck : dfck / dgam_c;
	double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk / dgam_sr;
	double dfyt = Get_fy4Thick(dfy, dfy1, dTf1);
	double dfyb = Get_fy4Thick(dfy, dfy1, dTf2);
	double dfyw = Get_fy4Thick(dfy, dfy1, dTw);
	double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt / dgam_M0;
	double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb / dgam_M0;
	double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw / dgam_M0;
	BOOL bPlasticClass = TRUE;

	double dbt = 0.0;
	double dbc = 0.0;
	double dc = 0.0;
	double dAlpha = 0.0;
	double dPsi = 0.0;
	double dk_sig = 0.0;

	// 4. Calc. Each Part's Force
	double dConcF = dBc * dtc*m_dAlpha*dfcd;
	double dStopfF = dB1 * dTf1*dfyd_t;
	double dSbotfF = dB2 * dTf2*dfyd_b;
	double dSwebF = bNeglectWeb ? 0.0 : dhw_in * dTw_t*dfyd_w;
	double dGirderF = dStopfF + dSbotfF + dSwebF;

	// 5. Calc. PNA
	if ( bPositive )	// Positive Moment.
	{
		if ( dConcF > dGirderF ) // PNA in Concrete Slab
		{
			double dDeltaF = dConcF - dGirderF;
			double dDeltaNA = dBc * m_dAlpha*dfcd < m_dZero ? 0.0 : dDeltaF / (dBc*m_dAlpha*dfcd);
			dPlasticNA = dH + dHh + dDeltaNA;
			dbc = 0.0;
			dAlpha = dh < m_dZero ? 0.0 : dbc / dh;
		}
		else if ( dConcF < dGirderF )
		{
			if ( !bNeglectWeb )
			{
				if ( (dConcF + dStopfF) >(dGirderF - dStopfF) ) // PNA in  Top flange
				{
					double dComp = dConcF + dStopfF;
					double dTens = dGirderF - dStopfF;
					double dDeltaNA = dB1 * dfyd_t < m_dZero ? 0.0 : (dComp - dTens) / (2.0*dB1*dfyd_t); // Comp-x*dB1*dfyd_t = Tens+x*dB1*dfyd_t
					dPlasticNA = dH - dTf1 + dDeltaNA;
				}
				else if ( (dConcF + dStopfF + dSwebF) > dSbotfF ) // PNA in  Web
				{
					double dComp = dConcF + dStopfF + dSwebF;
					double dTens = dSbotfF;
					double dDeltaNA = dTw_t * dfyd_w < m_dZero ? 0.0 : (dComp - dTens) / (2.0*dTw_t*dfyd_w); // Comp-x*dTw_t*dfyd_w = Tens+x*dTw_t*dfyd_w
					double dDeltaHw = dCosA * dDeltaNA;
					dPlasticNA = dTf2 + dDeltaHw;
				}
				else if ( (dConcF + dStopfF + dSwebF) < dSbotfF ) // PNA in  Bottom flange
				{
					double dComp = dConcF + dStopfF + dSwebF;
					double dTens = dSbotfF;
					double dDeltaNA = dB2 * dfyd_b < m_dZero ? 0.0 : (dTens - dComp) / (2.0*dB2*dfyd_b); // Comp+x*dB2*dfyd_b = Tens-x*dB2*dfyd_b
					dPlasticNA = dTf2 - dDeltaNA;
				}
				else ASSERT(0);
			}
			else
			{
				if ( (dConcF + dStopfF) >(dGirderF - dStopfF) ) // PNA in Top flange
				{
					double dComp = dConcF + dStopfF;
					double dTens = dGirderF - dStopfF;
					double dDeltaNA = dB1 * dfyd_t < m_dZero ? 0.0 : (dComp - dTens) / (2.0*dB1*dfyd_t); // Comp-x*dB1*dfyd_t = Tens+x*dB1*dfyd_t
					dPlasticNA = dH - dTf1 + dDeltaNA;
				}
				else if ( (dConcF + dStopfF + dSwebF) < dSbotfF )// PNA in Bottom flange
				{
					double dComp = dConcF + dStopfF + dSwebF;
					double dTens = dSbotfF;
					double dDeltaNA = dB2 * dfyd_b < m_dZero ? 0.0 : (dTens - dComp) / (2.0*dB2*dfyd_b); // Comp+x*dB2*dfyd_b = Tens-x*dB2*dfyd_b
					dPlasticNA = dTf2 - dDeltaNA;
				}
			}
		}
	}
	else // Negative Moment
	{
		double dRebarF = 0.0;
		// Rebar Info.
		for ( int i = 0; i < param.arRbarPosi.GetSize(); i++ )
		{
			_DGN_RBAR_CRC Rbar = param.arRbarPosi[i];
			double dAsi = Rbar.dArea;
			dRebarF += dAsi * dfsd;
		}

		if ( dGirderF > dRebarF )
		{
			if ( !bNeglectWeb )
			{
				if ( (dRebarF + dStopfF) > (dGirderF - dStopfF) ) // PNA in  Top flange
				{
					double dTens = dRebarF + dStopfF;
					double dComp = dGirderF - dStopfF;
					double dDeltaNA = dB1 * dfyd_t < m_dZero ? 0.0 : (dTens - dComp) / (2.0*dB1*dfyd_t); // Comp-x*dB1 = Tens+x*dB1
					dPlasticNA = dH - dTf1 + dDeltaNA;
				}
				else if ( (dRebarF + dStopfF + dSwebF) > dSbotfF ) // PNA in  Web
				{
					double dTens = dRebarF + dStopfF + dSwebF;
					double dComp = dSbotfF;
					double dDeltaNA = dTw_t * dfyd_w < m_dZero ? 0.0 : (dTens - dComp) / (2.0*dTw_t*dfyd_w); // Comp-x*dTw_t = Tens+x*dTw_t
					double dDeltaHw = dCosA * dDeltaNA;
					dPlasticNA = dTf2 + dDeltaHw;
				}
				else if ( (dRebarF + dStopfF + dSwebF) < dSbotfF ) // PNA in  Bottom flange
				{
					double dTens = dRebarF + dStopfF + dSwebF;
					double dComp = dSbotfF;
					double dDeltaNA = dB2 * dfyd_b < m_dZero ? 0.0 : (dComp - dTens) / (2.0*dB2*dfyd_b); // Comp+x*dB2 = Tens-x*dB2
					dPlasticNA = dTf2 - dDeltaNA;
				}
				else ASSERT(0);
			}
			else
			{
				if ( (dRebarF + dStopfF) >(dGirderF - dStopfF) ) // PNA in Top flange
				{
					double dTens = dRebarF + dStopfF;
					double dComp = dGirderF - dStopfF;
					double dDeltaNA = dB1 * dfyd_t < m_dZero ? 0.0 : (dTens - dComp) / (2.0*dB1*dfyd_t); // Comp-x*dB1 = Tens+x*dB1
					dPlasticNA = dH - dTf1 + dDeltaNA;
				}
				else if ( (dRebarF + dStopfF + dSwebF) < dSbotfF ) // PNA in Bottom flange
				{
					double dTens = dRebarF + dStopfF + dSwebF;
					double dComp = dSbotfF;
					double dDeltaNA = dB2 * dfyd_b < m_dZero ? 0.0 : (dComp - dTens) / (2.0*dB2*dfyd_b); // Comp+x*dB2 = Tens-x*dB2
					dPlasticNA = dTf2 - dDeltaNA;
				}
			}
		}
		else
		{
			// Iteration으로 찾아야함. 하지만 수렴 안하는 경우도 있을것임.
			// 그럴때는 rebar끝과 Top flange 중간으로 함.      
			int iCount = 0;
			double dCompF = dGirderF;
			double dTensF = dRebarF;
			double dDeltaF = 0.0;
			double dNeuC1 = dH;
			double dNeuC2 = dHt;
			double dTol = 1.0; // 1%
			double dToler = 0.0;
			double dNeuC = (dHt - 0.5*dtc);
			int iIter = 20;
			do
			{
				double dGap = (dHt - 0.5*dtc) - dNeuC;
				double dNAdsi = 0.0;
				double dCompfsdAs = 0.0;
				double dTensfsdAs = 0.0;

				for ( int i = 0; i < param.arRbarPosi.GetSize(); i++ )
				{
					_DGN_RBAR_CRC Rbar;
					Rbar = param.arRbarPosi[i];
					// PlasticNA에서 떨어진 거리.        
					double dNAdsi = dGap + Rbar.dz;
					double dAsi = Rbar.dArea;
					if ( dNAdsi < 0.0 ) // 압축.
					{
						dCompfsdAs += dAsi * dfsd;
					}
					else
					{
						dTensfsdAs += dAsi * dfsd;
					}
				}

				dCompF = dGirderF + dCompfsdAs;
				dTensF = dTensfsdAs;
				dDeltaF = dCompF - dTensF;
				dToler = (fabs(dCompF) < m_dZero ? 0.0 : dTensF / dCompF * 100.0);

				if ( fabs(dToler) < dTol ) break;
				if ( iCount > iIter )break;

				if ( dDeltaF <= 0.0 ) { dNeuC2 = dNeuC; }
				else { dNeuC1 = dNeuC; }
				dPlasticNA = 0.5*(dNeuC1 + dNeuC2);
				dNeuC = dPlasticNA;

				++iCount;
			}
			while ( fabs(dToler) > dTol );

		}
	}
	return dPlasticNA;
}
BOOL CDgnPlateGirder::Calc_InternalPlateClassUnit(T_PLATE_GIRDER_CALC_D& param, const T_CLASS_IN_UNIT_D& ClassInD, double dfy, double dfy1,
	int& iClass, CLASS_BASE_UNIT& ClassBase, bool bWeb, bool bTop)
{
	// Initialize 
	iClass = 0;
	ClassBase.Initialize();

	T_SECT_D& SectD = param.SectElem.SectD;
	int nSectType = m_pDgnSectUtil->Get_RealSectType(&SectD);
	BOOL bPosiI = param.bPosiI;
	double dStress1 = ClassInD.dStressOuter;
	double dStress2 = ClassInD.dStressInter;
	double dRangeStress = fabs(dStress1 - dStress2);
	double dc = ClassInD.dc;
	double dt = ClassInD.dt;
	double dWTR = dt < m_dZero ? 0.0 : dc / dt;
	double dfy_r = Get_fy4Thick(dfy, dfy1, dt);
	double dEpsi = Get_Epsilon(dfy_r);

	if ( dStress1 > 0.0 && dStress2 > 0.0 )
	{
		iClass = 1;
		ClassBase.iClass = iClass;
		return TRUE;
	}

	double dAlpha = 0.0, dbc = 0.0, dbt = 0.0;
	double dPsi = 0.0, dk_sig = 0.0, dLamda_p = 0.0, dRho = 0.0;
	double dbeff = 0.0, dbe1 = 0.0, dbe2 = 0.0;
	int iWebDeter = 0;
	BOOL bInternal = TRUE, bTensionWeb = FALSE, bDeterClass = FALSE;
	BOOL bPositiveMy = IsPositiveMy(dStress1, dStress2);
	double dTf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
	double dh = m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);

	if ( bWeb )
	{
		if ( bPositiveMy )
		{
			if ( dTf2 < param.dPlasticNA && param.dPlasticNA < dTf2 + dh + m_dZero ) // PNA in Web
			{
				dbc = dTf2 + dh - param.dPlasticNA;
			}
			else if ( dTf2 > param.dPlasticNA ) // PNA in Bottom Flange
			{
				dbc = dh;
			}
			else if ( dTf2 + dh < param.dPlasticNA ) // PNA in Top Flange
			{
				bTensionWeb = TRUE;
			}

			dAlpha = dh < m_dZero ? 0.0 : dbc / dh;
		}
		else
		{
			if ( dTf2 < param.dPlasticNA && param.dPlasticNA < dTf2 + dh + m_dZero ) // PNA in Web
			{
				dbc = param.dPlasticNA - dTf2;
			}
			else if ( dTf2 > param.dPlasticNA )// PNA in Bottom Flange
			{
				bTensionWeb = TRUE;
			}
			else if ( dTf2 + dh < param.dPlasticNA ) // PNA in Top Flange
			{
				dbc = dh;
			}

			dAlpha = dh < m_dZero ? 0.0 : dbc / dh;
		}
	}
	else
	{
		double dbf = 0.0;
		if ( nSectType == D_SECT_TYPE_COMPO_B )
		{
			dbf = bTop ? m_pDgnSectUtil->Get_Size_bft_m(&SectD, bPosiI) : m_pDgnSectUtil->Get_Size_bfb_m(&SectD, bPosiI);
		}
		else if ( nSectType == D_SECT_TYPE_COMPO_TUB )
		{
			dbf = m_pDgnSectUtil->Get_Size_bfb_m(&SectD, bPosiI);
		}
		else ASSERT(0);
		dbc = 0.5 * dbf; // 대칭이라고 가정
		dAlpha = dbf < m_dZero ? 0.0 : dbc / dbf;
	}

	if ( !bTensionWeb )
	{
		iClass = Get_WebPlaticClass(dWTR, dEpsi, dAlpha);
	}
	else
	{
		iClass = 1;
	}

	if ( iClass > 2 ) // EN 1993-1-5:2006, 4.4, Elastic Case, not Class 1 or 2
	{
		if ( fabs(dStress1 * dStress2) < m_dZero )
		{
			iClass = 1;
			bDeterClass = TRUE;
		}
		else if ( dStress1 < 0.0 && dStress2 < 0.0 )
		{
			dAlpha = 1.0;
			dbc = dc;
			if ( fabs(dStress1 - dStress2) < m_dZero )
			{
				dPsi = 1.0;
				dk_sig = Get_InternalBucklingfactor(dPsi);
				dLamda_p = Get_Lamda_p(dc, dt, dEpsi, dk_sig);
				Get_ReductionFactorRho(bInternal, dc, dt, dLamda_p, dPsi, dRho);
				dbeff = dRho * dbc;
				dbe1 = 0.5*dbeff;
				dbe2 = 0.5*dbeff;
			}
			else
			{
				if ( fabs(dStress1) > fabs(dStress2) )
				{
					dPsi = dStress2 / dStress1;
					iWebDeter = 1; // Top = Sigma1
				}
				else
				{
					dPsi = dStress1 / dStress2;
					iWebDeter = 2; // Bot = Sigma1
				}

				dk_sig = Get_InternalBucklingfactor(dPsi);
				dLamda_p = Get_Lamda_p(dc, dt, dEpsi, dk_sig);
				Get_ReductionFactorRho(bInternal, dc, dt, dLamda_p, dPsi, dRho);

				dbeff = dRho * dbc;
				dbe1 = 2.0 / (5.0 - dPsi)*dbeff;
				dbe2 = dbeff - dbe1;
			}

			iClass = Get_WebElasticClass(dAlpha, dWTR, dEpsi, dPsi);
			bDeterClass = TRUE;
		}
		else if ( dStress1 * dStress2 < m_dZero )
		{
			double dx0 = 0.0;
			if ( dStress1 < 0.0 && dStress2 > -m_dZero )
			{
				dbc = fabs(dStress1) / (fabs(dStress1) + fabs(dStress2))*dc;
				dAlpha = dbc / dc;
				dPsi = dStress2 / dStress1;
				iWebDeter = 1; // Top = Sigma1
			}
			else if ( dStress1 > -m_dZero && dStress2 < 0.0 )
			{
				dbc = fabs(dStress2) / (fabs(dStress1) + fabs(dStress2))*dc;
				dAlpha = dbc / dc;
				dPsi = dStress1 / dStress2;
				iWebDeter = 2; // Bot = Sigma1
			}
			dk_sig = Get_InternalBucklingfactor(dPsi);
			dLamda_p = Get_Lamda_p(dc, dt, dEpsi, dk_sig);
			Get_ReductionFactorRho(bInternal, dc, dt, dLamda_p, dPsi, dRho);
			dbeff = dRho * dc / (1.0 - dPsi);
			dbe1 = 0.4*dbeff;
			dbe2 = 0.6*dbeff;
			//
			iClass = Get_WebElasticClass(dAlpha, dWTR, dEpsi, dPsi);
			bDeterClass = TRUE;
		}
	}

	// save result
	ClassBase.iClass = iClass;
	ClassBase.iWebDeter = iWebDeter;
	ClassBase.dPsi = dPsi;
	ClassBase.dk_sig = dk_sig;
	ClassBase.dRho = dRho;
	ClassBase.dbeff = dbeff;
	ClassBase.dbe1 = dbe1;
	ClassBase.dbe2 = dbe2;

	return TRUE;
}
BOOL CDgnPlateGirder::Get_SectionClass(T_PLATE_GIRDER_CALC_D& param, BOOL bChkRestrained, const T_CLASS_IN_D& crClassInD)
{
	T_CLAS_BASE& ClassD = param.ClassBase;
	T_MATD_D& MatdD = param.MatlElem.MatdD;
	double dfy = MatdD.Data1.Design.S_Fy1;
	double dfy1 = MatdD.Data1.Design.S_Fy2;

	// 2. Section.  
	T_SECT_D& SectD = param.SectElem.SectD;
	T_MATL_DESIGN& DgnMatD = MatdD.Data1.Design;
	int iClass = 0, iClassSect = 0;
	int iClassFlg = 0, iClassWeb = 0;
	int iClassFlg_T = 0, iClassFlg_B = 0;

	// 1. Top Flange Class
	int nTopPartSize = crClassInD.aTopFlangeD.GetSize();
	for ( int nTop = 0; nTop < nTopPartSize; ++nTop )
	{
		iClass = 0;
		const T_CLASS_IN_UNIT_D& ClassInD = crClassInD.aTopFlangeD[nTop];
		CLASS_BASE_UNIT ClassResD;
		if ( ClassInD.nType == 0 ) { Calc_OutstandClassUnit(ClassInD, dfy, dfy1, iClass, ClassResD); }
		else if ( ClassInD.nType == 1 ) { Calc_InternalPlateClassUnit(param, ClassInD, dfy, dfy1, iClass, ClassResD, false, true); }
		else { ASSERT(0); return FALSE; }

		iClassFlg_T = bChkRestrained ? 1 : max(iClassFlg_T, ClassResD.iClass);

		if ( nTop == 0 || iClassFlg_T <= ClassResD.iClass )
		{
			int nIndexT = nTop == 0 ? 0 : 1;
			ClassD.iClass[nIndexT] = ClassResD.iClass;
			ClassD.dPsi[nIndexT] = ClassResD.dPsi;
			ClassD.dk_sig[nIndexT] = ClassResD.dk_sig;
			ClassD.dRho[nIndexT] = ClassResD.dRho;
			ClassD.dbeff[nIndexT] = ClassResD.dbeff;
			ClassD.dbe1[nIndexT] = ClassResD.dbe1;
			ClassD.dbe2[nIndexT] = ClassResD.dbe2;
		}
	}

	// 2. Bottom Flange Class
	int nBotPartSize = crClassInD.aBotFlangeD.GetSize();
	for ( int nBot = 0; nBot < nBotPartSize; ++nBot )
	{
		iClass = 0;
		const T_CLASS_IN_UNIT_D& ClassInD = crClassInD.aBotFlangeD[nBot];
		CLASS_BASE_UNIT ClassResD;
		if ( ClassInD.nType == 0 ) { Calc_OutstandClassUnit(ClassInD, dfy, dfy1, iClass, ClassResD); }
		else if ( ClassInD.nType == 1 ) { Calc_InternalPlateClassUnit(param, ClassInD, dfy, dfy1, iClass, ClassResD, false, false); }
		else { ASSERT(0); return FALSE; }

		iClassFlg_B = max(iClassFlg_B, ClassResD.iClass);

		if ( nBot == 0 || iClassFlg_B <= ClassResD.iClass )
		{
			int nIndexB = nBot == 0 ? 2 : 3;
			ClassD.iClass[nIndexB] = ClassResD.iClass;
			ClassD.dPsi[nIndexB] = ClassResD.dPsi;
			ClassD.dk_sig[nIndexB] = ClassResD.dk_sig;
			ClassD.dRho[nIndexB] = ClassResD.dRho;
			ClassD.dbeff[nIndexB] = ClassResD.dbeff;
			ClassD.dbe1[nIndexB] = ClassResD.dbe1;
			ClassD.dbe2[nIndexB] = ClassResD.dbe2;
		}
	}

	iClassFlg = max(iClassFlg_T, iClassFlg_B);

	// 3. Web Class
	int nWebPartSize = crClassInD.aWebD.GetSize();
	for ( int nWeb = 0; nWeb < nWebPartSize; ++nWeb )
	{
		iClass = 0;
		const T_CLASS_IN_UNIT_D& ClassInD = crClassInD.aWebD[nWeb];
		CLASS_BASE_UNIT ClassResD;
		if ( ClassInD.nType == 1 ) { Calc_InternalPlateClassUnit(param, ClassInD, dfy, dfy1, iClass, ClassResD); }
		else { ASSERT(0); return FALSE; }

		if ( iClassWeb < ClassResD.iClass )
		{
			iClassWeb = ClassResD.iClass;
			ClassD.iWebDeter = ClassResD.iWebDeter;
			ClassD.iClass[4] = ClassResD.iClass;
			ClassD.dPsi[4] = ClassResD.dPsi;
			ClassD.dk_sig[4] = ClassResD.dk_sig;
			ClassD.dRho[4] = ClassResD.dRho;
			ClassD.dbeff[4] = ClassResD.dbeff;
			ClassD.dbe1[4] = ClassResD.dbe1;
			ClassD.dbe2[4] = ClassResD.dbe2;
		}
	}

	iClassSect = max(iClassFlg, iClassWeb);

	if ( iClassWeb == 3 && (iClassFlg == 1 || iClassFlg == 2) ) // EN 1993-1-1 : 2005, 5.5.2(11)
	{
		iClassSect = 2;
	}

	ClassD.iTopFlgClass = iClassFlg_T;
	ClassD.iBotFlgClass = iClassFlg_B;
	ClassD.iFlangeClass = iClassFlg;
	ClassD.iWebClass = iClassWeb;
	ClassD.iClassSect = iClassSect;

	return TRUE;
}
BOOL CDgnPlateGirder::Get_EffectivePropertyForClass4(T_PLATE_GIRDER_CALC_D& param, BOOL bPositive, T_GENL_DATA& EffGLineD, T_SECT_STIFFNESS& GirderEffStiff, T_SECT_STIFFNESS& CompoEffStiff)
{
	T_SECT_D CalcSectD; CalcSectD.Initialize();
	CalcSectD.SectBefore.Shape = D_SECT_SHAPE_REG_GEN;
	CalcSectD.SectBefore.SectI.aOuterPolygon.Add(EffGLineD.PolyD);

	int iEffLineSize = param.aEffLineIx.GetSize();
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aEffLine;
	aEffLine.SetSize(iEffLineSize);
	for ( int i = 0; i < iEffLineSize; i++ )
	{
		int iEffLineIx = param.aEffLineIx[i];
		aEffLine.SetAt(i, EffGLineD.aLine[iEffLineIx]);
	}
	CalcSectD.SectBefore.SectI.aLine.Copy(aEffLine);

	// vertex정보를 보고싶을 때 
	//Print_EffectivePolyAndLineData(EffGLineD.PolyD, aEffLine);

	// girder만 있을경우 
	CSectUtil::Get_LinetypeSectProperty(CalcSectD);
	GirderEffStiff = CalcSectD.SectBefore.SectI.Stiffness;

	// Composite 경우. 
	// Girder + Slab
	// Tapered가 아니여도 J단 값을 Set 해야함.
	CalcSectD.Initialize();
	CalcSectD.nStype = D_SECT_TYPE_TAPERED;
	CalcSectD.SectBefore.nStype = D_SECT_TYPE_COMPO_G;
	CalcSectD.SectBefore.SectI = param.SectBaseD;
	CalcSectD.SectBefore.SectI.aLine.Copy(aEffLine); // I이지만. aEffLine은 bPosiI에 데이터를 따른다. 

	int iStaIx = 0, iEndIx = 0;
	int iGenPartSize = CalcSectD.SectBefore.SectI.aGeneralPart.GetSize();
	for ( int i = 0; i < iGenPartSize; i++ )
	{
		T_GSEC_PART GsecPartD = param.SectBaseD.aGeneralPart[i];
		if ( GsecPartD.bPlane )
		{
			//
		}
		else // Line
		{
			iStaIx = GsecPartD.IdxStart[0]; // 0=Polygon
			iEndIx = GsecPartD.IdxEnd[0];
			int iSize = iEndIx - iStaIx;
			if ( iSize != 1 ) { ASSERT(0); return FALSE; }
			// Effective polygon으로 교체      
			CalcSectD.SectBefore.SectI.aOuterPolygon.SetAt(iStaIx, EffGLineD.PolyD);
			GsecPartD.IdxStart[2] = 0;
			GsecPartD.IdxEnd[2] = aEffLine.GetSize();
			CalcSectD.SectBefore.SectI.aGeneralPart.SetAt(i, GsecPartD);
		}

	}
	CalcSectD.SectBefore.SectJ = CalcSectD.SectBefore.SectI;

	m_pDoc->m_pSectDB->CalcSectCompo_G(CalcSectD, TRUE);

	BOOL bScaleFactor = FALSE;
	T_SECT_INFO_D SectInfoD;
	double dScaleFactor[7] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
	double dPosition = param.bPosiI ? 0.0 : 1.0;
	BOOL bDgn = TRUE;
	CSectUtil::Get_SectInfoData(param.ElemK, dPosition, SectInfoD, bDgn);
	if ( bPositive ) SectInfoD.bCracked = FALSE;
	else          SectInfoD.bCracked = TRUE;
	SectInfoD.StiffMainD.Stiffness = CalcSectD.SectBefore.SectI.Stiffness;
	SectInfoD.StiffMainD.dYbar = CalcSectD.SectBefore.SectI.Design.YBar;
	SectInfoD.StiffMainD.dZbar = CalcSectD.SectBefore.SectI.Design.ZBar;
	SectInfoD.aSectPart[0].Stiff.Stiffness = GirderEffStiff;
	//SectInfoD.aSectPart[0].Stiff.dYbar = 
	SectInfoD.aSectPart[0].Stiff.dZbar = GirderEffStiff.Czm;

	T_SECT_PROP_D SectPropD;
	CSectUtil::Get_GeneralSectProperty(bScaleFactor, SectInfoD, dScaleFactor, SectPropD);
	if ( SectPropD.aSectProp.GetSize() > 0 )
		CompoEffStiff = SectPropD.aSectProp[0].StiffRbar.Stiffness;

	return TRUE;
}