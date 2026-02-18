#include "stdafx.h"
#include "DgnInputConverter.h"
#include "IDgnDataBase.h"

#include "../../dgnengine/idesign/DGN_def/DataTypeDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnInput.h"
#include "../../dgnengine/idesign/DGN_def/SectionDef.h"
#include "../../dgnengine/idesign/DGN_def/MaterialDef.h"
#include "../../dgnengine/idesign/DGN_def/SectionPropertyDef.h"
#include "../../dgnengine/idesign/DGN_def/UnbracedLengthDef.h"
#include "../../dgnengine/idesign/DGN_def/EffectiveLengthDef.h"
#include "../../dgnengine/idesign/DGN_def/SteelGeneralSectParamDef.h"
#include "../../dgnengine/idesign/DGN_def/BendingCoefficientDef.h"
#include "../../dgnengine/idesign/DGN_def/ShearCoefficientDef.h"
#include "../../dgnengine/idesign/DGN_def/SafetyFactorDef.h"
#include "../../dgnengine/idesign/DGN_def/SeismicPropertyDef.h"
#include "../../dgnengine/idesign/DGN_def/LimitSlendernessRatioDef.h"
#include "../../dgnengine/idesign/DGN_def/LcomInfoDef.h"
#include "../../dgnengine/idesign/DGN_def/Force1DDef.h"
#include "../../dgnengine/idesign/DGN_def/MembTypeDef.h"
#include "../../dgnengine/idesign/DGN_def/MemberInfoDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnNodeDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnElemDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnMembDef.h"
#include "../../dgnengine/idesign/DGN_def/BindingDataDef.h"
#include "../../dgnengine/idesign/DGN_def/Dgn1DPositionDef.h"
#include "../../dgnengine/idesign/DGN_def/SectionShapeDef.h"
#include "../../dgnengine/idesign/DGN_def/PartialFactorDef.h"
#include "../../dgnengine/idesign/DGN_def/MembLengthDef.h"
#include "../../dgnengine/idesign/DGN_def/ForceUblnDef.h"
#include "../../dgnengine/idesign/DGN_def/Force1DMemberDef.h"
#include "../../dgnengine/idesign/DGN_def/SteelCompositeSectionDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnTypeEnumDef.h"
#include "../../dgnengine/idesign/DGN_def/StatusDef.h"
#include "../../dgnengine/idesign/DGN_def/Force1DArrayDef.h"
#include "../../dgnengine/idesign/DGN_def/MaterialConcreteDef.h"
#include "../../dgnengine/idesign/DGN_def/MaterialSteelHybridDef.h"
#include "../../dgnengine/idesign/DGN_def/MaterialSteelDef.h"
#include "../../dgnengine/idesign/DGN_def/StressCheckDef.h"
#include "../../dgnengine/idesign/DGN_def/MomentLoadType.h"
#include "../../dgnengine/idesign/DGN_def/LateralTorsionalBucklingDef.h"
#include "../../dgnengine/idesign/DGN_def/LoadApplicationTypeDef.h"
#include "../../dgnengine/idesign/DGN_def/ShearConnectorDef.h"
#include "../../dgnengine/idesign/DGN_def/SectionRebarCoordDef.h"
#include "../../dgnengine/idesign/DGN_def/LongitudinalStiffenerDef.h"
#include "../../dgnengine/idesign/DGN_def/TransverseStiffenerDef.h"
#include "../../dgnengine/idesign/DGN_def/DamageEquivalenceFactorDef.h"
#include "../../dgnengine/idesign/DGN_def/DamageEquivalenceFactorMembDef.h"
#include "../../dgnengine/idesign/DGN_def/StressLimitationFactorDef.h"
#include "../../dgnengine/idesign/DGN_def/StressSteelCompositeDef.h"
#include "../../dgnengine/idesign/DGN_def/ConstrainStressCompositeDef.h"
#include "../../dgnengine/idesign/DGN_def/MemoizationKeyDef.h"
#include "../../dgnengine/idesign/DGN_def/TransverseStiffenerDef.h"
#include "../../dgnengine/idesign/DGN_def/TorsionDesignDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnSteelOptionsDef.h"
#include "../../dgnengine/idesign/DGN_def/ImperfectionFactorDef.h"
#include "../../dgnengine/idesign/DGN_def/DesignCodeDef.h"
#include "../../dgnengine/idesign/DGN_def/CompositeSectionPropertyDef.h"
#include "../../dgnengine/idesign/DGN_def/PanelInfoDef.h"
#include "../../dgnengine/idesign/DGN_def/ResidualStressTypeDef.h"
#include "../../dgnengine/idesign/DGN_def/StiffnessScaleFactorDef.h"

#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl.h"
#include "../wg_base/ArrayUtil.h"
#include "../wg_db/MembCtrl.h"

using namespace dgn::converter;
using namespace dgn::def;

CDgnInputConverter::CDgnInputConverter()
{
}

CDgnInputConverter::~CDgnInputConverter()
{
}

bool CDgnInputConverter::AddInputData(const dgn::def::DataType& Type, const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    switch ( Type )
    {
	case dgn::def::DataType::MemberInfoArray:               return AddInputMemberInfoArray(pDgnDataBase, Input);
	case dgn::def::DataType::SectionShape:                  return AddInputSectionShape(pDgnDataBase, Input);
	case dgn::def::DataType::Section:                       return AddInputSection(pDgnDataBase, Input);
	case dgn::def::DataType::Material:                      return AddInputMaterial(pDgnDataBase, Input);
	case dgn::def::DataType::SectionProperty:               return AddInputSectionProperty(pDgnDataBase, Input);
	case dgn::def::DataType::MaterialSteel:                 return AddInputMaterialSteel(pDgnDataBase, Input);
	case dgn::def::DataType::EffectiveLength:               return AddInputEffectiveLength(pDgnDataBase, Input);
	case dgn::def::DataType::SteelGeneralSectParam:         return AddInputSectionStrength(pDgnDataBase, Input);
	case dgn::def::DataType::BendingCoefficient:            return AddInputBendingCoefficient(pDgnDataBase, Input);
	case dgn::def::DataType::ShearCoefficient:              return AddInputShearCoefficient(pDgnDataBase, Input);
	case dgn::def::DataType::SafetyFactor:                  return AddInputSafetyFactor(pDgnDataBase, Input);
	case dgn::def::DataType::SeismicProperty:               return AddInputSeismicProperty(pDgnDataBase, Input);
	case dgn::def::DataType::UnbracedLength:                return AddInputUnbracedLength(pDgnDataBase, Input);
	case dgn::def::DataType::LcomInfo:                      return AddInputLcomInfo(pDgnDataBase, Input);
	case dgn::def::DataType::Force1D:                       return AddInputForce1D(pDgnDataBase, Input);
	case dgn::def::DataType::PartialFactor:                 return AddInputPartialFactor(pDgnDataBase, Input);
	case dgn::def::DataType::MembLength:                    return AddInputMembLength(pDgnDataBase, Input);
	case dgn::def::DataType::ForceUbln:                     return AddInputForceUbln(pDgnDataBase, Input);
	case dgn::def::DataType::Force1DMember:                 return AddInputForce1DMember(pDgnDataBase, Input);
	case dgn::def::DataType::SteelCompositeSection:         return AddInputSteelCompositeSection(pDgnDataBase, Input);
	case dgn::def::DataType::Status:                        return AddInputStatus(pDgnDataBase, Input);
	case dgn::def::DataType::Force1DArray:                  return AddInputForce1DArray(pDgnDataBase, Input);
	case dgn::def::DataType::MaterialConcrete:              return AddInputMaterialConcrete(pDgnDataBase, Input);
	case dgn::def::DataType::MaterialSteelHybrid:           return AddInputMaterialSteelHybrid(pDgnDataBase, Input);
	case dgn::def::DataType::StressCheck:                   return AddInputStressCheck(pDgnDataBase, Input);
	case dgn::def::DataType::MomentLoadType:                return AddInputMomentLoadType(pDgnDataBase, Input);
	case dgn::def::DataType::TorsionDesign:                 return AddInputTorsionDesign(pDgnDataBase, Input);
	case dgn::def::DataType::SteelOptions:                  return AddInputSteelOptions(pDgnDataBase, Input);
	case dgn::def::DataType::LateralTorsionalBucklingParam: return AddInputLateralTorsionalBuckling(pDgnDataBase, Input);
	case dgn::def::DataType::LoadAppType:                   return AddInputLoadApplicationType(pDgnDataBase, Input);
	case dgn::def::DataType::ShearConnector:                return AddInputShearConnector(pDgnDataBase, Input);
	case dgn::def::DataType::SectionRebarCoord:             return AddInputSteelCompositeRebar(pDgnDataBase, Input);
	case dgn::def::DataType::LongitudinalStiffener:         return AddInputLongitudinalStiffener(pDgnDataBase, Input);
	case dgn::def::DataType::TransverseStiffener:           return AddInputTransverseStiffener(pDgnDataBase, Input);
	case dgn::def::DataType::DamageEquivalenceFactor:       return AddInputDamageEquivalenceFactor(pDgnDataBase, Input);
	case dgn::def::DataType::DamageEquivalenceFactorMemb:   return AddInputDamageEquivalenceFactorMember(pDgnDataBase, Input);
	case dgn::def::DataType::StressLimitationFactor:        return AddInputStressLimitationFactor(pDgnDataBase, Input);
	case dgn::def::DataType::SteelCompositeStress:          return AddInputStressSteelComposite(pDgnDataBase, Input);
	case dgn::def::DataType::CompositeConstrainStress:      return AddInputConstrainStressComposite(pDgnDataBase, Input);
	case dgn::def::DataType::MemoizationKey:                return AddInputMemoizationKey(pDgnDataBase, Input);
	case dgn::def::DataType::ImperfectionFactor:            return AddInputImperfectionFactor(pDgnDataBase, Input);
	case dgn::def::DataType::DesignCode:                    return AddInputDesignCode(pDgnDataBase, Input);
	case dgn::def::DataType::CompositeSectionProperty:      return AddInputCompositeSectionProperty(pDgnDataBase, Input);
	case dgn::def::DataType::PanelInfoArray:                return AddInputPanelInfoArray(pDgnDataBase, Input);
	case dgn::def::DataType::MembType:                      return true;
	case dgn::def::DataType::MomentMagnifier:               return true;
	case dgn::def::DataType::EquivalentMoment:              return true;
	case dgn::def::DataType::LimitSlendernessRatio:         return AddInputLimitSlendernessRatio(pDgnDataBase, Input);
	case dgn::def::DataType::ResidualStressType:            return AddInputResidualStressType(pDgnDataBase, Input);
	case dgn::def::DataType::StiffnessScaleFactor:          return AddInputStiffnessScaleFactor(pDgnDataBase, Input);
	default: ASSERT(0); return false;
    }

    return true;
}

bool CDgnInputConverter::AddInputMembType(const dgn::def::enDgn& enDgnType, const dgn::def::enMemb& enMembType, dgn::def::CDgnInput& Input)
{
    auto* pMembType = new CMembType;

    pMembType->DgnType  = enDgnType;
    pMembType->MembType = enMembType;

    return Input.AddData(pMembType);
}

bool CDgnInputConverter::AddInputDesignCode(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pDgnCode = new CDesignCode;

	pDgnDataBase->GetDesignCode(pDgnCode);

	return Input.AddData(pDgnCode);
}

bool CDgnInputConverter::AddInputMemberInfoArray(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pMembInfoArr = new CMemberInfoArray;

    pDgnDataBase->GetMemberInfoArray(pMembInfoArr);

    return Input.AddData(pMembInfoArr);
}

bool CDgnInputConverter::AddInputSectionShape(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pSectionShape = new CSectionShape;

    pDgnDataBase->GetSectionShape(pSectionShape);

    return Input.AddData(pSectionShape);
}

bool CDgnInputConverter::AddInputSection(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pSection = new CSection;

    pDgnDataBase->GetSection(pSection);

    return Input.AddData(pSection);
}

bool CDgnInputConverter::AddInputMaterial(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pMaterial = new CMaterial;

    pDgnDataBase->GetMaterial(pMaterial);

    return Input.AddData(pMaterial);
}

bool CDgnInputConverter::AddInputSectionProperty(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pSectionProperty = new CSectionProperty;

    pDgnDataBase->GetSectionProperty(pSectionProperty);

    return Input.AddData(pSectionProperty);
}

bool CDgnInputConverter::AddInputMaterialSteel(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pMatlProp = new CMaterialSteel;

    pDgnDataBase->GetMaterialSteel(pMatlProp);

    return Input.AddData(pMatlProp);
}

bool CDgnInputConverter::AddInputEffectiveLength(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pEffectiveLength = new CEffectiveLength;

    pDgnDataBase->GetEffectiveLength(pEffectiveLength);

    return Input.AddData(pEffectiveLength);
}

bool CDgnInputConverter::AddInputSectionStrength(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pSectionStrength = new CSteelGeneralSectParam;

    pDgnDataBase->GetSteelGeneralSectParam(pSectionStrength);

    return Input.AddData(pSectionStrength);
}

bool CDgnInputConverter::AddInputBendingCoefficient(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pBendingCoefficient = new CBendingCoefficient;

    pDgnDataBase->GetBendingCoefficient(pBendingCoefficient);

    return Input.AddData(pBendingCoefficient);
}

bool CDgnInputConverter::AddInputShearCoefficient(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pShearCoefficient = new CShearCoefficient;

    pDgnDataBase->GetShearCoefficient(pShearCoefficient);

    return Input.AddData(pShearCoefficient);
}

bool CDgnInputConverter::AddInputSafetyFactor(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pSafetyFactor = new CSafetyFactor;

    pDgnDataBase->GetSafetyFactor(pSafetyFactor);

    return Input.AddData(pSafetyFactor);
}

bool CDgnInputConverter::AddInputSeismicProperty(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pSeismicProperty = new CSeismicProperty;

    pDgnDataBase->GetSeismicProperty(pSeismicProperty);

    return Input.AddData(pSeismicProperty);
}

bool CDgnInputConverter::AddInputUnbracedLength(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pUnbracedLength = new CUnbracedLength;

    pDgnDataBase->GetUnbracedLength(pUnbracedLength);

    return Input.AddData(pUnbracedLength);
}

bool CDgnInputConverter::AddInputLcomInfo(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pLcomInfo = new CLcomInfo;

    pDgnDataBase->GetLcomInfo(pLcomInfo);

    return Input.AddData(pLcomInfo);
}

bool CDgnInputConverter::AddInputForce1D(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pForce1D = new CForce1D;

    pDgnDataBase->GetForce1D(pForce1D);

    return Input.AddData(pForce1D);
}

bool CDgnInputConverter::AddInputForce1DArray(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pForce1DArray = new CForce1DArray;

    pDgnDataBase->GetForce1DArray(pForce1DArray);

    return Input.AddData(pForce1DArray);
}

bool CDgnInputConverter::AddInputPartialFactor(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pPartialFactor = new CPartialFactor;

    pDgnDataBase->GetPartialFactor(pPartialFactor);

    return Input.AddData(pPartialFactor);
}

bool CDgnInputConverter::AddInputMembLength(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pMembLength = new CMembLength;

    pDgnDataBase->GetMembLength(pMembLength);

    return Input.AddData(pMembLength);
}

bool CDgnInputConverter::AddInputForceUbln(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pForceUbln = new CForceUbln;

    pDgnDataBase->GetUnbracedForce(pForceUbln);

    return Input.AddData(pForceUbln);
}

bool CDgnInputConverter::AddInputForce1DMember(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pForce1DMember = new CForce1DMember;

	pDgnDataBase->GetForce1DMember(pForce1DMember);

	return Input.AddData(pForce1DMember);
}

bool CDgnInputConverter::AddInputSteelCompositeSection(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pSCSection = new CSteelCompositeSection;

    pDgnDataBase->GetSteelCompositeSection(pSCSection);

    return Input.AddData(pSCSection);
}

bool CDgnInputConverter::AddInputStatus(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto *pData = new CStatus;
    CStatus::TStatus Status;
    Status.Init(CStatus::EN_STATUS::NG, COLORREF(RGB(255, 0, 0)), 1.0); // red
    pData->vStatus.emplace_back(Status);
    Status.Init(CStatus::EN_STATUS::OK, COLORREF(RGB(0, 0, 255)), 0.0); // blue
    pData->vStatus.emplace_back(Status);
    Status.Init(CStatus::EN_STATUS::Failure, COLORREF(RGB(128, 128, 0)), 1.0); // redgreen
    pData->vStatus.emplace_back(Status);
    return Input.AddData(pData);
}

bool CDgnInputConverter::AddInputMaterialConcrete(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pMaterial = new CMaterialConcrete;

    pDgnDataBase->GetMaterialConcrete(pMaterial);

    return Input.AddData(pMaterial);
}

bool CDgnInputConverter::AddInputMaterialSteelHybrid(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pMaterial = new CMaterialSteelHybrid;

    pDgnDataBase->GetMaterialSteelHybrid(pMaterial);

    return Input.AddData(pMaterial);
}

bool CDgnInputConverter::AddInputStressCheck(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pStressCheck = new CStressCheck;

    pDgnDataBase->GetStressCheck(pStressCheck);

    return Input.AddData(pStressCheck);
}

bool CDgnInputConverter::AddInputMomentLoadType(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pMoment = new CMomentLoadType;
    pDgnDataBase->GetMomentLoadType(pMoment);
    return Input.AddData(pMoment);
}

bool CDgnInputConverter::AddInputTorsionDesign(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pTorsion = new CTorsionDesign;
	pDgnDataBase->GetTorsionDesign(pTorsion);
	return Input.AddData(pTorsion);
}

bool CDgnInputConverter::AddInputSteelOptions(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pOptions = new CDgnSteelOptions;
	pDgnDataBase->GetDgnSteelOptions(pOptions);
	return Input.AddData(pOptions);
}

bool CDgnInputConverter::AddInputLateralTorsionalBuckling(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pLTBParam = new CLateralTorsionalBuckling;
    pDgnDataBase->GetLateralTorsionalBucklingParam(pLTBParam);
    return Input.AddData(pLTBParam);
}

bool CDgnInputConverter::AddInputLoadApplicationType(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
    auto* pLoadApp = new CLoadApplicationType;
    pDgnDataBase->GetLoadApplicationType(pLoadApp);
    return Input.AddData(pLoadApp);
}

bool CDgnInputConverter::AddInputShearConnector(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pShearConnector = new CShearConnector;
	pDgnDataBase->GetShearConnector(pShearConnector);
	return Input.AddData(pShearConnector);
}

bool CDgnInputConverter::AddInputSteelCompositeRebar(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pRebarCoord = new CSectionRebarCoord;
	pDgnDataBase->GetSteelCompositeRebar(pRebarCoord);
	return Input.AddData(pRebarCoord);
}

bool CDgnInputConverter::AddInputLongitudinalStiffener(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pLongStfn = new CLongitudinalStiffener;
	pDgnDataBase->GetLongitudinalStiffener(pLongStfn);
	return Input.AddData(pLongStfn);
}

bool CDgnInputConverter::AddInputTransverseStiffener(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pTranStfn = new CTransverseStiffener;
	pDgnDataBase->GetTransverseStiffener(pTranStfn);
	return Input.AddData(pTranStfn);
}

bool CDgnInputConverter::AddInputDamageEquivalenceFactor(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pFactor = new CDamageEquivalenceFactorDef;
	pDgnDataBase->GetDamageEquivalenceFactor(pFactor);
	return Input.AddData(pFactor);
}

bool CDgnInputConverter::AddInputDamageEquivalenceFactorMember(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pFactor = new CDamageEquivalenceFactorMembDef;
	pDgnDataBase->GetDamageEquivalenceFactorMember(pFactor);
	return Input.AddData(pFactor);
}

bool CDgnInputConverter::AddInputStressLimitationFactor(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pFactor = new CStressLimitationFactorDef;
	pDgnDataBase->GetStressLimitationFactor(pFactor);
	return Input.AddData(pFactor);
}

bool CDgnInputConverter::AddInputStressSteelComposite(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pStress = new CStressSteelCompositeDef;
	pDgnDataBase->GetStressSteelComposite(pStress);
	return Input.AddData(pStress);
}

bool CDgnInputConverter::AddInputConstrainStressComposite(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pStress = new CConstrainStressComposite;
	pDgnDataBase->GetConstrainStressComposite(pStress);
	return Input.AddData(pStress);
}

bool CDgnInputConverter::AddInputMemoizationKey(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pMemo = new CMemoizationKey;
	pDgnDataBase->GetMemoizationKey(pMemo);
	return Input.AddData(pMemo);
}

bool CDgnInputConverter::AddInputImperfectionFactor(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pFactor = new CImperfectionFactor;
	pDgnDataBase->GetImperfectionFactor(pFactor);
	return Input.AddData(pFactor);
}

bool CDgnInputConverter::AddInputCompositeSectionProperty(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pSectionProperty = new CCompositeSectionProperty;

	pDgnDataBase->GetCompositeSectionProperty(pSectionProperty);

	return Input.AddData(pSectionProperty);
}

bool CDgnInputConverter::AddInputPanelInfoArray(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pPanelInfo = new CPanelInfoArray;

	pDgnDataBase->GetPanelInfoArray(pPanelInfo);

	return Input.AddData(pPanelInfo);
}

bool CDgnInputConverter::AddInputResidualStressType(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pRSType = new CResidualStressType;

	pDgnDataBase->GetResidualStressType(pRSType);

	return Input.AddData(pRSType);
}

bool CDgnInputConverter::AddInputLimitSlendernessRatio(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pLimitSlenderness = new CLimitSlendernessRatio;

	pDgnDataBase->GetLimitSlendernessRatio(pLimitSlenderness);

	return Input.AddData(pLimitSlenderness);

}

bool CDgnInputConverter::AddInputStiffnessScaleFactor(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input)
{
	auto* pFactor = new CStiffnessScaleFactorArray;

	pDgnDataBase->GetStiffnessScaleFactor(pFactor);

	return Input.AddData(pFactor);
}
