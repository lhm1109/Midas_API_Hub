#include "stdafx.h"

#include "IDgnDataBase.h"
#include "DgnMsgDgn.h"
#include "DgnReportMaker.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\dgnengine\idesign\DGN_link\IDgnReport.h"
#include "..\dgnengine\idesign\DGN_link\ReportUnitType.h"
#include "..\dgnengine\idesign\DGN_def\DgnUnitDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnUnitUtil.h"

using namespace dgn::def;
using namespace dgn::link;

IDgnDataBase::IDgnDataBase()
{
}

IDgnDataBase::~IDgnDataBase()
{
}

std::wstring IDgnDataBase::GetDgnRptNation()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    T_PREFERENCE rPreference;
    pDoc->m_pInitCtrl->GetPreference(rPreference);
    int crtLang = rPreference.DgnCode.OutputLanguage;

    return static_cast<std::wstring>(CT2W(CDgnMsgDgn::Get_MsgNation(crtLang)));
}

bool IDgnDataBase::GetCriticalReportInfo(T_DGN_RPT_DATA& Data)
{
    ASSERT(0);
    return false;
}

void IDgnDataBase::GetDgnPreferenceUnit(std::wstring& ForCU, std::wstring& LenCU, std::wstring& ULenCU)
{
	ASSERT(0);
}

bool IDgnDataBase::InitializeRpt()
{
	return true;
}

bool IDgnDataBase::FinalizeRpt()
{
	return true;
}

bool IDgnDataBase::GetSteelCompositeSection(dgn::def::CSteelCompositeSection* pData) const
{
    return true;
}

bool IDgnDataBase::GetMaterialConcrete(dgn::def::CMaterialConcrete* pData) const
{
    return true;
}

bool IDgnDataBase::GetMaterialSteelHybrid(dgn::def::CMaterialSteelHybrid* pData) const
{
    return true;
}

bool IDgnDataBase::GetForce1DArray(dgn::def::CForce1DArray* pData) const
{
    return true;
}

bool IDgnDataBase::GetForce1DMember(dgn::def::CForce1DMember* pData) const
{
	return true;
}

bool IDgnDataBase::GetLoadApplicationType(dgn::def::CLoadApplicationType* pData) const
{
    return true;
}

bool IDgnDataBase::GetLateralTorsionalBucklingParam(dgn::def::CLateralTorsionalBuckling* pData) const
{
    return true;
}

bool IDgnDataBase::GetShearConnector(dgn::def::CShearConnector* pData) const
{
	return true;
}

bool IDgnDataBase::GetSteelCompositeRebar(dgn::def::CSectionRebarCoord* pData) const
{
	return true;
}

bool IDgnDataBase::GetLongitudinalStiffener(dgn::def::CLongitudinalStiffener* pData) const
{
	return true;
}

bool IDgnDataBase::GetTransverseStiffener(dgn::def::CTransverseStiffener* pData) const
{
	return true;
}

bool IDgnDataBase::GetDamageEquivalenceFactor(dgn::def::CDamageEquivalenceFactorDef* pData) const
{
	return true;
}

bool IDgnDataBase::GetDamageEquivalenceFactorMember(dgn::def::CDamageEquivalenceFactorMembDef* pData) const
{
	return true;
}

bool IDgnDataBase::GetStressLimitationFactor(dgn::def::CStressLimitationFactorDef* pData) const
{
	return true;
}

bool IDgnDataBase::GetStressSteelComposite(dgn::def::CStressSteelCompositeDef* pData) const
{
	return true;
}

bool IDgnDataBase::GetConstrainStressComposite(dgn::def::CConstrainStressComposite* pData) const
{
	return true;
}

bool IDgnDataBase::GetMemoizationKey(dgn::def::CMemoizationKey* pData) const
{
	return true;
}

bool IDgnDataBase::GetImperfectionFactor(dgn::def::CImperfectionFactor* pData) const
{
	return true;
}

bool IDgnDataBase::GetCompositeSectionProperty(dgn::def::CCompositeSectionProperty* pData) const
{
	return true;
}

bool IDgnDataBase::GetPanelInfoArray(dgn::def::CPanelInfoArray* pData) const
{
	return true;
}

bool IDgnDataBase::GetReportInput(dgn::link::MItemInputList& vMembItemInput, dgn::link::MembInfoList& vMembInfo)
{
    return true; //통합설계엔진의 perform으로 연결되지 않은 부재 계산서 연결시 사용
}

bool IDgnDataBase::GetLimitSlendernessRatio(dgn::def::CLimitSlendernessRatio* pData) const
{
	return true;
}

bool IDgnDataBase::GetResidualStressType(dgn::def::CResidualStressType* pData) const
{
	return true;
}

bool IDgnDataBase::GetStiffnessScaleFactor(dgn::def::CStiffnessScaleFactorArray* pData) const
{
	return true;
}

dgn::link::TReportUnitType IDgnDataBase::GetCurrentUnit()
{
    wstring F, L;
    GetDgnCurrentUnit(F, L);
    TReportUnitType Unit(dgn::def::CDgnUnitUtil::GetForceUnit(F), CDgnUnitUtil::GetLengthUnit(L), CDgnUnitUtil::GetULengthUnit(L));
    return Unit;
}

dgn::link::TReportUnitType IDgnDataBase::GetDgnPreferenceUnit()
{
    wstring F, L, UL;
    GetDgnPreferenceUnit(F, L, UL);
    TReportUnitType Unit(dgn::def::CDgnUnitUtil::GetForceUnit(F), CDgnUnitUtil::GetLengthUnit(L), CDgnUnitUtil::GetULengthUnit(UL));
    return Unit;
}

