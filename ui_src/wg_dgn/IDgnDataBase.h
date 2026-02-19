#pragma once

#include "..\dgnengine\idesign\DGN_def\DgnInput.h"
#include "..\dgnengine\idesign\DGN_def\DgnResult.h"
#include <vector>

#include "HeaderPre.h"

namespace dgn
{
    namespace def
    {
        enum class enDgnCode : unsigned int;
        enum class enDgn;
        enum class enMemb;

        class IResult;

        class CMemberInfo;
        class CMemberInfoArray;
        class CSection;
        class CMaterial;
        class CSectionProperty;
        class CEffectiveLength;
        class CSteelGeneralSectParam;
        class CBendingCoefficient;
        class CShearCoefficient;
        class CSafetyFactor;
        class CMaterialSteel;
        class CSeismicProperty;
        class CUnbracedLength;
        class CForceUbln;
        class CLcomInfo;
        class CForce1D;
        class CForce1DArray;
		class CForce1DMember;
        class CPartialFactor;
        class CSectionShape;
        class CMembLength;
        class CStressCheck;
        class CMomentLoadType;
		class CTorsionDesign;
		class CDgnSteelOptions;
        class CForceUbln;
        class CSteelCompositeSection;
        class CMaterialConcrete;
        class CMaterialSteelHybrid;
        class CLoadApplicationType;
        class CLateralTorsionalBuckling;
		class CShearConnector;
		class CSectionRebarCoord;
		class CLongitudinalStiffener;
		class CTransverseStiffener;
        class CShearConnector;
		class CDamageEquivalenceFactorDef;
		class CDamageEquivalenceFactorMembDef;
		class CStressLimitationFactorDef;
		class CStressSteelCompositeDef;
		class CConstrainStressComposite;
		class CMemoizationKey;
		class CImperfectionFactor;
		class CRptSettings;
		class CDesignCode;
		class CCompositeSectionProperty;
		class CPanelInfoArray;
		class CLimitSlendernessRatio;
		class CResidualStressType;
		class CStiffnessScaleFactorArray;

        struct ReportCrit;
    }

    namespace rpt
    {
        class CDgnReportInfo;
    }

    namespace link
    {
        typedef std::vector<std::pair<item::enDgnResultItem, std::pair<def::CDgnInput*, def::ReportCrit>>> ItemInputList;
        typedef std::vector<ItemInputList> MItemInputList;
        typedef std::vector<rpt::CDgnReportInfo*> MembInfoList;
        struct TReportUnitType;
    }
}

struct T_DGN_RPT_DATA
{
    UINT unElem;
    std::wstring sElem;
    std::wstring sPosi;
    std::wstring sLcom;
    std::wstring sLcomVz;
    std::wstring sPosiVz;
    std::wstring sLcomVy;
    std::wstring sPosiVy;

    T_DGN_RPT_DATA()
    {
        unElem = 0;
    }
};

struct T_DGN_RPT_HEAD_INFO
{
    std::wstring sTitle;
    std::wstring sProductVer;
    
    bool         bUserInfor;
    std::wstring sCompanyName;
    std::wstring sWebSite;
    std::wstring sTelNo;
    std::wstring sFaxNo;

    T_DGN_RPT_HEAD_INFO()
    {
        bUserInfor = false;
    }
};

class __MY_EXT_CLASS__ IDgnDataBase
{
public:
    IDgnDataBase();
    virtual ~IDgnDataBase();

public:
    virtual bool SetResult(const dgn::def::IResult* pResult) = 0;

public:
    virtual dgn::def::enDgnCode GetDgnCode() const = 0;
    virtual dgn::def::enDgn GetDgnType() const = 0;
    virtual dgn::def::enMemb GetDgnMembType() const = 0;
    
    virtual UINT GetLcomSize() = 0;

    virtual UINT GetDgnElemNo() const = 0;
    virtual UINT GetDgnPosiNo() const = 0;

    virtual std::wstring GetDgnCodeName() = 0;
    virtual std::wstring GetDgnElemName() = 0;
    virtual std::wstring GetDgnPosiName() = 0;
    virtual std::wstring GetDgnLcomName() = 0;
    virtual std::wstring GetDgnPosiName(int nPos) = 0;
    virtual std::wstring GetDgnLcomName(CString strLcom) = 0;
    virtual void GetDgnCurrentUnit(std::wstring& ForCU, std::wstring& LenCU) = 0;
	virtual void GetRptSettings(const CString& strMembTypeName, dgn::def::CRptSettings& rSettings) = 0;
    virtual std::wstring GetDgnRptNation();
    virtual bool GetCriticalReportInfo(T_DGN_RPT_DATA& Data);
	virtual void GetDgnPreferenceUnit(std::wstring& ForCU, std::wstring& LenCU, std::wstring& ULenCU);
	
	virtual bool InitializeRpt();
	virtual bool FinalizeRpt();

public:
	virtual bool GetDesignCode(dgn::def::CDesignCode* pData) const = 0;
	virtual bool GetMemberInfo(dgn::def::CMemberInfo* pData) const = 0;
    virtual bool GetMemberInfoArray(dgn::def::CMemberInfoArray* pData) const = 0;
    virtual bool GetSection(dgn::def::CSection* pData) const = 0;
    virtual bool GetMaterial(dgn::def::CMaterial* pData) const = 0;
    virtual bool GetSectionProperty(dgn::def::CSectionProperty* pData) const = 0;
    virtual bool GetMaterialSteel(dgn::def::CMaterialSteel* pData) const = 0;
    virtual bool GetEffectiveLength(dgn::def::CEffectiveLength* pData) const = 0;
    virtual bool GetSteelGeneralSectParam(dgn::def::CSteelGeneralSectParam* pData) const = 0;
    virtual bool GetBendingCoefficient(dgn::def::CBendingCoefficient* pData) const = 0;
    virtual bool GetShearCoefficient(dgn::def::CShearCoefficient* pData) const = 0;
    virtual bool GetSafetyFactor(dgn::def::CSafetyFactor* pData) const = 0;
    virtual bool GetSeismicProperty(dgn::def::CSeismicProperty* pData) const = 0;
    virtual bool GetUnbracedLength(dgn::def::CUnbracedLength* pData) const = 0;
    virtual bool GetUnbracedForce(dgn::def::CForceUbln* pData) const = 0;
    virtual bool GetLcomInfo(dgn::def::CLcomInfo* pData) const = 0;
    virtual bool GetForce1D(dgn::def::CForce1D* pData) const = 0;
    virtual bool GetPartialFactor(dgn::def::CPartialFactor* pData) const = 0;
    virtual bool GetSectionShape(dgn::def::CSectionShape* pData) const = 0;
    virtual bool GetMembLength(dgn::def::CMembLength* pData) const = 0;
    virtual bool GetStressCheck(dgn::def::CStressCheck* pData) const = 0;
    virtual bool GetMomentLoadType(dgn::def::CMomentLoadType* pData) const = 0;
	virtual bool GetTorsionDesign(dgn::def::CTorsionDesign* pData) const = 0;
	virtual bool GetDgnSteelOptions(dgn::def::CDgnSteelOptions* pData) const = 0;
    virtual bool UpdateShearData(const int& nDir, std::wstring& strLcb, std::wstring& strPosi, dgn::def::CDgnInput* pInp) = 0;

public:
    virtual bool GetSteelCompositeSection(dgn::def::CSteelCompositeSection* pData) const;
    virtual bool GetMaterialConcrete(dgn::def::CMaterialConcrete* pData) const;
    virtual bool GetMaterialSteelHybrid(dgn::def::CMaterialSteelHybrid* pData) const;
    virtual bool GetForce1DArray(dgn::def::CForce1DArray* pData) const;    
	virtual bool GetForce1DMember(dgn::def::CForce1DMember* pData) const;
    virtual bool GetLoadApplicationType(dgn::def::CLoadApplicationType* pData) const;
    virtual bool GetLateralTorsionalBucklingParam(dgn::def::CLateralTorsionalBuckling* pData) const;
	virtual bool GetShearConnector(dgn::def::CShearConnector* pData) const;
	virtual bool GetSteelCompositeRebar(dgn::def::CSectionRebarCoord* pData) const;
	virtual bool GetLongitudinalStiffener(dgn::def::CLongitudinalStiffener* pData) const;
	virtual bool GetTransverseStiffener(dgn::def::CTransverseStiffener* pData) const;
	virtual bool GetDamageEquivalenceFactor(dgn::def::CDamageEquivalenceFactorDef* pData) const;
	virtual bool GetDamageEquivalenceFactorMember(dgn::def::CDamageEquivalenceFactorMembDef* pData) const;
	virtual bool GetStressLimitationFactor(dgn::def::CStressLimitationFactorDef* pData) const;
	virtual bool GetStressSteelComposite(dgn::def::CStressSteelCompositeDef* pData) const;
	virtual bool GetConstrainStressComposite(dgn::def::CConstrainStressComposite* pData) const;
	virtual bool GetMemoizationKey(dgn::def::CMemoizationKey* pData) const;
	virtual bool GetImperfectionFactor(dgn::def::CImperfectionFactor* pData) const;
	virtual bool GetCompositeSectionProperty(dgn::def::CCompositeSectionProperty* pData) const;
	virtual bool GetPanelInfoArray(dgn::def::CPanelInfoArray* pData) const;
	virtual bool GetLimitSlendernessRatio(dgn::def::CLimitSlendernessRatio* pData) const;
	virtual bool GetResidualStressType(dgn::def::CResidualStressType* pData) const;
	virtual bool GetStiffnessScaleFactor(dgn::def::CStiffnessScaleFactorArray* pData) const;

public:
    virtual bool GetReportInput(dgn::link::MItemInputList& vMembItemInput, dgn::link::MembInfoList& vMembInfo);
    dgn::link::TReportUnitType GetCurrentUnit();
    dgn::link::TReportUnitType GetDgnPreferenceUnit();

};

#include "HeaderPost.h"
