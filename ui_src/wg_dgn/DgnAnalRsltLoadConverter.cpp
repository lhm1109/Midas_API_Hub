#include "StdAfx.h"

#include <array>
#include <vector>
#include <map>

#include "../../dgnengine/idesign/DGN_lib/Macro.h"

#include "../wg_db/wg_db_DBLib.h"
#include "../wg_db/wg_db_DBDoc.h"
#include "../wg_db/wg_db_AttrCtrl.h"
#include "../wg_db/wg_db_PostCtrl.h"

#include "DgnAnalRsltUtil.h"

#include "DgnAnalRsltLoadConverter.h"

#include "..\dgnengine\idesign\DGN_def\DgnLoadCaseEnumDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;
using namespace dgn::link;

CDgnAnalRsltLoadConverter::CDgnAnalRsltLoadConverter(CDBDoc* pDBDoc) :
    dgn::link::TAnalRsltLoadConverter<MCasKey, LoadKey, SuperKey, SubKey>(),
    m_pDBDoc(pDBDoc)
{
}

CDgnAnalRsltLoadConverter::~CDgnAnalRsltLoadConverter()
{
}

int CDgnAnalRsltLoadConverter::GetProductDCasKeyList(std::vector<MCasKey>& vKeyDCas) const
{
    vKeyDCas.clear();
    vKeyDCas.push_back(1); // Gen/Civil은 Design case가 없으므로 default로 key 1 하나만 채워줌.

    return Macro::GetSize(vKeyDCas);
}

int CDgnAnalRsltLoadConverter::GetProductCaseKeyList(std::vector<LoadKey>& vKeyCase) const
{
    vKeyCase.clear();

    std::vector<enCaseKeyType> vCaseType;
    if ( !CDgnAnalRsltUtil::GetCaseKeyList(vCaseType) )
    {
        ASSERT(0); return 0;
    }

    for ( const auto& CaseType : vCaseType )
    {
        std::vector<LoadKey> vLoad;
        GetProductCaseKeyList(CaseType, vLoad);

        Macro::Append(vKeyCase, vLoad);
    }

    return Macro::GetSize(vKeyCase);
}

int CDgnAnalRsltLoadConverter::GetSuperKeyList(std::vector<SuperKey>& vSuperKey) const
{
    vSuperKey.clear();

    auto* pAttrCtrl = GetDBDoc()->m_pAttrCtrl;

    std::vector<enLcomKeyType> vLcomType;
    if ( !CDgnAnalRsltUtil::GetLcomKeyList(vLcomType) )
    {
        ASSERT(0); return 0;
    }

    for ( const auto& LcomType : vLcomType )
    {
        CArray<T_KEY, T_KEY> aLcom;
        pAttrCtrl->GetLcomKeyList(CDgnAnalRsltUtil::ConvertLcomKindEnum(LcomType), aLcom);

        std::vector<SuperKey> vEach;
        ConvertC2V_F(aLcom, vEach, [&LcomType] (const T_KEY Key)
        {
            return SuperKey(LcomType, Key);
        });

        Macro::Append(vSuperKey, vEach);
    }

    return Macro::GetSize(vSuperKey);
}

bool CDgnAnalRsltLoadConverter::HasSub(const SuperKey& SuperKey) const
{
    //TODO Anal 요건 진짜 들어오면 나중에 하지뭐 =_=
    return false;
}

bool CDgnAnalRsltLoadConverter::GetSubKeyList(const SuperKey& SuperKey, std::vector<SubKey>& vSubKey) const
{
    //TODO Anal 요건 진짜 들어오면 나중에 하지뭐 =_=
    ASSERT(0); return false;
}

bool CDgnAnalRsltLoadConverter::ConvertProductLcom(const SuperKey& SuperKey, TLcomD& Lcom) const
{
    if ( !SuperKey.IsValid() )
    {
        ASSERT(0); return false;
    }

    auto* pAttrCtrl = GetDBDoc()->m_pAttrCtrl;

    const auto LcomKindType = CDgnAnalRsltUtil::ConvertLcomKindType(SuperKey.GetType());

    T_LCOM_D ProductLcom;
    if ( !pAttrCtrl->GetLcom(LcomKindType, SuperKey.GetKey(), ProductLcom) )
    {
        ASSERT(0); return false;
    }

    Lcom.SumType = [] (const T_LCOM_D& ProductLcom)
    {
        switch ( ProductLcom.LoadCombType )
        {
        case EN_DGN_ADD:  return TLcomD::enType::Add;
        case EN_DGN_ENV:  return TLcomD::enType::Env;
        case EN_DGN_ABS:  return TLcomD::enType::Abs;
        case EN_DGN_SRSS: return TLcomD::enType::Srs;
        default: ASSERT(0); return TLcomD::enType::Non;
        }
    } (ProductLcom);

    ConvertC2V_F(ProductLcom.aCombination, Lcom.vLoad, [this] (const T_LCOM_BASE& Base)
    {
        TLoadD Load;

        if ( CDgnAnalRsltUtil::IsLcomCaseType(Base.AnalType) )
        {
            Load.dFactor = Base.Factor;
            Load.KeyA = Base.LoadCaseKey;
            Load.Type = TLoadD::enType::Lcom;
            Load.LcomType = [&Base]()
            {
                switch (Base.AnalType)
                {
                case D_LCOM_CB_STEEL:
                    return enLcomKeySteel;
                case D_LCOM_CB_CONCRETE:
                    return enLcomKeyConcrete;
                case D_LCOM_CB_FOOTING:
                    return enLcomKeyFdn;
                case D_LCOM_CB_SRC:
                    return enLcomKeySrc;
                case D_LCOM_CB_ALUMINUM:
                    return enLcomKeyAluminum;
                case D_LCOM_CB_STLCOMP:
                    return enLcomKeyStlComp;
                case D_LCOM_CB_CFSTEEL:
                    return enLcomKeyCfSteel;
                default:
                    break;
                }
                return enLcomKeyNone;
            }();
        }
        else
        {
            Load.dFactor = Base.Factor;
            Load.KeyA = GetEngineCaseKey(LoadKey(CDgnAnalRsltUtil::ConvertLcomCaseType(Base.AnalType), Base.LoadCaseKey));
            Load.Type = TLoadD::enType::Anal;
        }

        return Load;
    });

    return true;
}

bool CDgnAnalRsltLoadConverter::ConvertProductLcom(const SuperKey& SuperKey, const SubKey& SubKey, TLcomD& Lcom) const
{
    //TODO Anal 요건 진짜 들어오면 나중에 하지뭐 =_=
    return false;
}

CDgnAnalRsltLoadConverter::MCasKey CDgnAnalRsltLoadConverter::GetPMCasKey(const dgn::def::DKey& KeyDCas) const
{
    return __super::GetProductDCasKey(KeyDCas);
}

enLoadCase CDgnAnalRsltLoadConverter::GetProductCaseType(const LoadKey& KeyCase) const
{
    enCaseKeyType enLType = KeyCase.GetType();
    switch (enLType)
    {
    case enCaseKeyNone:
        break;
    case enCaseKeyStatic:
        {
            T_STLD_D StldD;
            if (!m_pDBDoc->m_pAttrCtrl->GetStld(KeyCase.GetKey(), StldD))    return enLoadCase::None;
            
            const CString& strLcasType = StldD.LoadCaseType;

            if (CDBLib::IsStldDL(strLcasType))          return enLoadCase::D;
            else if (CDBLib::IsStldLL(strLcasType))    return enLoadCase::L; // Live
            else if (CDBLib::IsStldSL(strLcasType))    return enLoadCase::L; // Snow
            else if (CDBLib::IsStldRL(strLcasType))    return enLoadCase::L; // Rain
            else if (CDBLib::IsStldEL(strLcasType))    return enLoadCase::E;
            else if (strLcasType == _T("EP")) return enLoadCase::EP;
            else if (strLcasType == _T("EP")) return enLoadCase::EH;
            else if (strLcasType == _T("EP")) return enLoadCase::EV;
            else if (strLcasType == _T("EP")) return enLoadCase::WP;
            else if (strLcasType == _T("B"))  return enLoadCase::B;
            else if (strLcasType == _T("IP")) return enLoadCase::IP;
            else if (strLcasType == _T("T"))  return enLoadCase::T;
            else if (strLcasType == _T("LS")) return enLoadCase::LS;
            else if (strLcasType == _T("W"))  return enLoadCase::W;
            else return enLoadCase::None;
        }
        break;
    case enCaseKeySpectrum:
        return enLoadCase::E;
    case enCaseKeySpectrumND:
        return enLoadCase::E;
    case enCaseKeyHistory:
        return enLoadCase::TH;
        break;
    case enCaseKeyMoving:
        return enLoadCase::L;
    case enCaseKeySettle:
        break;
    case enCasePushOver:
        break;
    case enCaseStage:
        break;
    case enCaseEccenspec:
        return enLoadCase::E;
    case enCaseEccenspecNd:
        return enLoadCase::E;
    case enCaseGridModel:
        break;
    case enCaseCrane:
        break;
    case enCaseKeyNumber:
        break;
    default:
        break;
    }

    return enLoadCase::None;
}

CDgnAnalRsltLoadConverter::LoadKey CDgnAnalRsltLoadConverter::GetPCaseKey(const dgn::def::DKey& KeyCase) const
{
    return __super::GetProductCaseKey(KeyCase);
}

int CDgnAnalRsltLoadConverter::GetProductCaseKeyList(const enCaseKeyType KeyType, std::vector<LoadKey>& vKeyCase) const
{
    CArray<T_KEY, T_KEY> aKey;
    auto* pAttrCtrl = GetDBDoc()->m_pAttrCtrl;
    auto* pPostCtrl = GetDBDoc()->m_pPostCtrl;

    switch ( KeyType )
    {
    case enCaseKeyStatic:
        pAttrCtrl->GetStldKeyList(aKey);
        break;
    case enCaseKeySpectrum:
        pAttrCtrl->GetSplcKeyList(aKey);
        break;
    case enCaseKeyHistory:
        pAttrCtrl->GetThisKeyList(aKey);
        break;
    case enCaseKeyMoving:
		{
			pAttrCtrl->GetMvldKeyList(aKey);    if ( aKey.GetSize() != 0 ) { break; }
			pAttrCtrl->GetMvldbsKeyList(aKey);	if ( aKey.GetSize() != 0 ) { break; }
			pAttrCtrl->GetMvldfrKeyList(aKey);	if ( aKey.GetSize() != 0 ) { break; }
			pAttrCtrl->GetMvldjpKeyList(aKey);	if ( aKey.GetSize() != 0 ) { break; }
			pAttrCtrl->GetMvldchKeyList(aKey);	if ( aKey.GetSize() != 0 ) { break; }
			pAttrCtrl->GetMvldidKeyList(aKey);	if ( aKey.GetSize() != 0 ) { break; }
			pAttrCtrl->GetMvldtrKeyList(aKey);	if ( aKey.GetSize() != 0 ) { break; }
		}
        break;
    case enCaseKeySettle:
        pAttrCtrl->GetSmlcKeyList(aKey);
        break;
    case enCaseEccenspec:
        pPostCtrl->GetRaemKeyList(aKey);
        break;
    case enCaseStage:
        pAttrCtrl->GetSgldKeyList(aKey);
        break;
    default:
        {
            const auto nLoadCaseType = CDgnAnalRsltUtil::ConvertLoadCaseEnum(KeyType);
            const auto strName = CDBLib::GetCaseTypeName_Full(nLoadCaseType);

            CString str;
            str.Format(_T("CDgnAnalRsltLoadConverter: Implement %s\n"), strName);
            TRACE(str);

            break;
        }
    }

    ConvertC2V_F(aKey, vKeyCase, [&KeyType] (const T_KEY& Key)
    {
        return LoadKey(KeyType, Key);
    });

    return Macro::GetSize(vKeyCase);
}

CDBDoc* CDgnAnalRsltLoadConverter::GetDBDoc() const
{
    if ( !m_pDBDoc )
    {
        ASSERT(0); return nullptr;
    }

    return m_pDBDoc;
}

int CDgnAnalRsltLoadConverter::GetProductAnalRsltCaseType(const enLoadCase& enCase) const
{
    switch (enCase)
    {
    case enLoadCase::USER:   // User
    case enLoadCase::D:      // Dead load
    case enLoadCase::DC:     // Component and Attachments
    case enLoadCase::DD:     // Downdrag(말뚝부 마찰력)
    case enLoadCase::DW:     // Wearing Surfaces and Utilities
    case enLoadCase::EP:     // Earth pressure load
    case enLoadCase::EH:     // horizontal earth pressure load
    case enLoadCase::EV:     // vertical earth pressure load
    case enLoadCase::ES:     // Earth SurTCHARge (상재토압)
    case enLoadCase::EL:     // Locked-in Erection stresses (시공과정중 축적된 구속응력)
    case enLoadCase::L:      // Live load
    case enLoadCase::LR:     // Roof live load
    case enLoadCase::IL:     // Impact Load
    case enLoadCase::LC:     // China  평판과차 또는 여대차에 의한 Live Load
    case enLoadCase::LSC:    // China  평판과차 또는 여대차에 의한 토측압력
    case enLoadCase::LS:     // 차량에 의한 토측압력(상재활하중)
    case enLoadCase::LP:     // 과적활하중
    case enLoadCase::ILP:    // 과적활하중에 의한 충격하중
    case enLoadCase::W:      // Wind load
    case enLoadCase::WA:     // 풍하중(풍직각방향)
    case enLoadCase::WT:     // 풍하중(비틀림방향)
    case enLoadCase::WL:     // Wind load added to moving vehicle
    case enLoadCase::BRK:    // Braking load (제동하중)
    case enLoadCase::E:      // Earthquake load
    case enLoadCase::EVT:    // Vertical Earthquake load
    case enLoadCase::CF:     // centrifugal load(원심하중)
    case enLoadCase::CO:     // collision load(충돌하중)
    case enLoadCase::CT:     // vehicle collision load(차량충돌하중)
    case enLoadCase::CV:     // vessel collision load(선박충돌하중)
    case enLoadCase::CR:     // creep load
    case enLoadCase::SH:     // Shrinkage load
    case enLoadCase::T:      // Temperature
    case enLoadCase::TPG:    // Temperature gradient
    case enLoadCase::B:      // 부력 또는 양압력 하중
    case enLoadCase::WP:     // 지하수압(ground water pressure)
    case enLoadCase::FP:     // 유체압(fluid pressure)
    case enLoadCase::SF:     // 유수압(stream flow pressure)
    case enLoadCase::WPR:    // 파압 하중
    case enLoadCase::PS:     // Prestress load
    case enLoadCase::S:      // Snow load
    case enLoadCase::R:      // Rain load
    case enLoadCase::ER:     // Erection load
    case enLoadCase::CRL:    // China 군중하중
    case enLoadCase::IP:     // Ice pressure
    case enLoadCase::FR:     // China 지지점 마찰력
    case enLoadCase::RS:     // AASHTO Rib Shortening
    case enLoadCase::BK:     // AASHTO Longitudinal force from live load
    case enLoadCase::GE:     // India IRC6-2000 Grade effect
    case enLoadCase::SHV:    // Soil Heaving
    case enLoadCase::LIP:    // Weight of leveling, insulation, protection
    case enLoadCase::PL:     // Weight of pavement
    case enLoadCase::BL:     // Weight of ballast
    case enLoadCase::EANN:   // Poland, Earth 관련 하중
    case enLoadCase::EANC:   // Poland, Earth 관련 하중
    case enLoadCase::EAMN:   // Poland, Earth 관련 하중
    case enLoadCase::EAMC:   // Poland, Earth 관련 하중
    case enLoadCase::EPNN:   // Poland, Earth 관련 하중
    case enLoadCase::EPNC:   // Poland, Earth 관련 하중
    case enLoadCase::EPMN:   // Poland, Earth 관련 하중
    case enLoadCase::EPMC:   // Poland, Earth 관련 하중
    case enLoadCase::DRL:    // India IRS Derailment Load Case
    case enLoadCase::EEP:    // 지진토압
    case enLoadCase::CS:     // 시공단계
    case enLoadCase::EX:     // 폭발하중
    case enLoadCase::I:      // Imperfection load
    case enLoadCase::EE:     // Non-Dissipative
    case enLoadCase::GD:     // 지반변동의 영향
    case enLoadCase::SSE:  // Static Seismic Load Eccentricity
        return enCaseKeyStatic;
    case enLoadCase::STL:    // 부등침하 하중(Settlement)
        return enCaseKeySettle;
    case enLoadCase::ESP:    // Response Spectrum Load
    case enLoadCase::ESPz:   // Response Spectrum Load (Z)
        return enCaseKeySpectrum;
    case enLoadCase::ESPE:   // Response Spectrum Load Eccentricity
        return enCaseEccenspec;
    case enLoadCase::M:      // Moving Load, Characteristic
        return enCaseKeyMoving;
    case enLoadCase::TH:     // Time history (시간이력)
        return enCaseKeyHistory;
    case enLoadCase::CRN:    // Crane
        return enCaseCrane;
    case enLoadCase::cDL:    // Construction Dead Load
    case enLoadCase::cLL:    // Construction Live Load
    case enLoadCase::cCR:    // Construction Creep
    case enLoadCase::cSH:    // Construction Shrinkage
    case enLoadCase::TP:     // Tendon Primary
    case enLoadCase::TS:     // Tendon Secondary
        return enCaseStage;
    default:
        break;
    }

    return enCaseKeyStatic;
}

int CDgnAnalRsltLoadConverter::GetEngineAnalRsltCaseKey(const enLoadCase& enCase, const DKey& KeyCase) const
{
    int nType = GetProductAnalRsltCaseType(enCase);
    return GetEngineCaseKey(LoadKey(static_cast<enCaseKeyType>(nType), KeyCase));
}

int CDgnAnalRsltLoadConverter::GetEngineAnalRsltDCasKey(const DKey& KeyDCas) const
{
    return 1;
}
