#pragma once


#include "..\wg_dgn\IDgnDataBase.h"
#include "..\wg_dgn\IDgnCodeCheck.h"

#include "DgnPscCommon.h"
#include "DgnForceStressST.h"
#include <concurrent_unordered_map.h>
#include <thread>
#include <tuple>

#include "HeaderPre.h"


namespace dgn
{
	namespace def
	{
		class IResult;
		class CDgnRptCriticalInfo;
		class CRptSettings;
		class CSectionProperty;
	}
}

struct S_CSGD_D;
struct CSG_MEMB_ULEN;
struct CSG_ULMS_D;
struct CSG_CHECK_RESULT_CODES;
struct STL_KR_RES_D;
struct CSG_FORCE_COMP;
struct CSG_FORCE_D;
struct CSG_ULMF_D;
struct CSG_MEMB_FOR_D;
struct CSG_CHECK_RESULT_CODES;
struct CSG_CHK_RES_DTR_PARAM;
struct CSG_CHECK_RESULT_CODES;
struct CSG_MATL_ELEM;
struct T_DGN_MATL_STEEL;
struct CSG_SECT_POSD;
struct CSG_TRST_SECT;
struct CSG_TRST_POSD;
struct CSG_TSES_POSD;
struct CSG_LCOM_MV_IN;
struct CSG_CHECK_RESULT_CODES;
struct CSG_SECT_ELEM;
struct CSG_CPSC_ELEM;
struct CSG_CPFP_ELEM;
struct CSG_CPCB_ELEM;
struct CSG_DOHL_ELEM;
struct CSG_EBSL_ELEM;
struct CSG_LTBD_ELEM;
struct CSG_DWSP_ELEM;
struct CSG_UFRA_ELEM;
struct CSG_MDFS_ELEM;
struct CSG_LSHR_ELEM;

class CMembCtrl;
class CDgnSectUtil;

class __MY_EXT_CLASS__ CDgnCsgDataBase : public CDgnPscCommon, public IDgnDataBase, public IDgnCodeCheck
{
	// Member Functions.
public:
	CDgnCsgDataBase();
	virtual ~CDgnCsgDataBase();


protected:
	virtual T_UNIT_INDEX GetCodeUnit() const = 0;
	virtual int  GetDgnLcomNo() const = 0;

protected:
	virtual bool SetResult(const dgn::def::IResult* pResult) = 0;

public:
	virtual dgn::def::enDgnCode GetDgnCode() const override;
	virtual dgn::def::enDgn GetDgnType() const override;
	virtual dgn::def::enMemb GetDgnMembType() const override;

	virtual UINT GetLcomSize() override;

	//virtual UINT GetDgnElemNo() const override final;
	//virtual UINT GetDgnPosiNo() const override final;

	virtual std::wstring GetDgnCodeName() override final;
	virtual std::wstring GetDgnElemName() override final;
	virtual std::wstring GetDgnPosiName() override final;
	virtual std::wstring GetDgnLcomName() override final;
	virtual std::wstring GetDgnPosiName(int nPos) override final;
	virtual std::wstring GetDgnLcomName(CString strLcom) override final;
	virtual void GetDgnCurrentUnit(std::wstring& ForCU, std::wstring& LenCU)  override final;
	virtual void GetRptSettings(const CString& strMembTypeName, dgn::def::CRptSettings& rSettings) override final;

	virtual bool GetDesignCode(dgn::def::CDesignCode* pData) const override final;
	virtual bool GetMemberInfo(dgn::def::CMemberInfo* pData) const override final;
	virtual bool GetMemberInfoArray(dgn::def::CMemberInfoArray* pData) const override final;
	virtual bool GetSection(dgn::def::CSection* pData) const override final;
	virtual bool GetMaterial(dgn::def::CMaterial* pData) const override final;
	virtual bool GetSectionProperty(dgn::def::CSectionProperty* pData) const override final;
	virtual bool GetMaterialSteel(dgn::def::CMaterialSteel* pData) const override final;
	virtual bool GetEffectiveLength(dgn::def::CEffectiveLength* pData) const override final;
	virtual bool GetSteelGeneralSectParam(dgn::def::CSteelGeneralSectParam* pData) const override final;
	virtual bool GetBendingCoefficient(dgn::def::CBendingCoefficient* pData) const override final;
	virtual bool GetShearCoefficient(dgn::def::CShearCoefficient* pData) const override final;
	virtual bool GetSafetyFactor(dgn::def::CSafetyFactor* pData) const override final;
	virtual bool GetSeismicProperty(dgn::def::CSeismicProperty* pData) const override final;
	virtual bool GetUnbracedLength(dgn::def::CUnbracedLength* pData) const override;
	virtual bool GetUnbracedForce(dgn::def::CForceUbln* pData) const override final;
	virtual bool GetLcomInfo(dgn::def::CLcomInfo* pData) const override final;
	virtual bool GetForce1D(dgn::def::CForce1D* pData) const override final;
	virtual bool GetPartialFactor(dgn::def::CPartialFactor* pData) const override final;
	virtual bool GetSectionShape(dgn::def::CSectionShape* pData) const override final;
	virtual bool GetMembLength(dgn::def::CMembLength* pData) const override final;
	virtual bool GetStressCheck(dgn::def::CStressCheck* pData) const override final;
	virtual bool GetMomentLoadType(dgn::def::CMomentLoadType* pData) const override final;
	virtual bool GetTorsionDesign(dgn::def::CTorsionDesign* pData) const override final;
	virtual bool GetDgnSteelOptions(dgn::def::CDgnSteelOptions* pData) const override final;
	virtual bool GetImperfectionFactor(dgn::def::CImperfectionFactor* pData) const override final;

	virtual bool UpdateShearData(const int& nDir, std::wstring& strLcb, std::wstring& strPosi, dgn::def::CDgnInput* pInp) override final;

public:
	virtual bool GetSteelCompositeSection(dgn::def::CSteelCompositeSection* pData) const override final;
	virtual bool GetMaterialConcrete(dgn::def::CMaterialConcrete* pData) const override final;
	virtual bool GetMaterialSteelHybrid(dgn::def::CMaterialSteelHybrid* pData) const override final;
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
	virtual bool GetMemoizationKey(dgn::def::CMemoizationKey* pData) const;
	virtual bool GetCompositeSectionProperty(dgn::def::CCompositeSectionProperty* pData) const override final;
	virtual bool GetPanelInfoArray(dgn::def::CPanelInfoArray* pData) const override final;
	virtual bool GetResidualStressType(dgn::def::CResidualStressType* pData) const override final;
	virtual bool GetStiffnessScaleFactor(dgn::def::CStiffnessScaleFactorArray* pData) const override final;

public:
	bool GetCsgSectElem(ElemPairK EPairK, CSG_SECT_ELEM& SectElem) const;


protected:
	double GetWebSlope(const CSG_SECT_POSD& rData) const;
	void   SetDgnForce(const T_CSGF_MEMB& rData);
	void   SetPanelForce(const T_CSGD_COMF Panelij[2]);
	void   SetDgnGirderStress(const _DGN_STRS_CRC& rData);
	void   SetDgnSlabStress(const _DGN_STRS_CRC& rData);
	void   SetReportMode(const bool& bReport);
	bool   IsReportMode() const;

	bool GetForce1DArrayDgn(dgn::def::CForce1DArray* pData) const;
	bool GetForce1DArrayRpt(dgn::def::CForce1DArray* pData) const;
	bool GetForce1DMemberDgn(dgn::def::CForce1DMember* pData) const;
	bool GetForce1DMemberRpt(dgn::def::CForce1DMember* pData) const;
	bool GetPanelInfoForceDgn(dgn::def::CPanelInfoArray* pData) const;
	bool GetPanelInfoForceRpt(dgn::def::CPanelInfoArray* pData) const;
	bool GetPanelInfoSectionProperty(dgn::def::CPanelInfoArray* pData) const;
	bool GetStressSteelCompositeDgn(dgn::def::CStressSteelCompositeDef* pData) const;
	bool GetStressSteelCompositeRpt(dgn::def::CStressSteelCompositeDef* pData) const;

	bool GetPanelInfoForce(_DGN_FORC_CRC& rForNC, _DGN_FORC_CRC& rForLT, _DGN_FORC_CRC& rForST) const;
	double GetForceMuy() const;

	bool AreThere3orMoreLStiffenerInWeb();
	bool IsFlangeWithLS();
	bool IsFlangeChecking();
	bool IsWebWithLS();	
	bool IsLongitudinalStiffener();
	//
	bool IsBendingChecking(bool bPositiveM);
	bool IsCombinedMVChecking();

private:
	bool GetLongitudinalStiffenerData(dgn::def::CLongitudinalStiffener* pData) const;

	double GetH(const CSG_SECT_POSD& rData) const;
	double GetHw(const CSG_SECT_POSD& rData) const;
	double GetTw(const CSG_SECT_POSD& rData) const;
	double GetBfTop(const CSG_SECT_POSD& rData) const;
	double GetTfTop(const CSG_SECT_POSD& rData) const;
	double GetBfIntTop(const CSG_SECT_POSD& rData) const;
	double GetBfTopVoid(const CSG_SECT_POSD& rData) const;
	double GetBfOutTop(const CSG_SECT_POSD& rData) const;
	double GetBfOut2Top(const CSG_SECT_POSD& rData) const;
	double GetBfBot(const CSG_SECT_POSD& rData) const;
	double GetTfBot(const CSG_SECT_POSD& rData) const;
	double GetBfIntBot(const CSG_SECT_POSD& rData) const;
	double GetBfOutBot(const CSG_SECT_POSD& rData) const;

	double GetBc(const CSG_SECT_POSD& rData) const;
	double GetTc(const CSG_SECT_POSD& rData) const;
	double GetHh(const CSG_SECT_POSD& rData) const;

	void   ConvertDgnSectStiffToSectionProperty(const DGN_SECT_STIFFNESS& rDgnStiff, dgn::def::CSectionProperty* pData) const;
	void   ConvertDgnSectStiffToSectionPropertyArr(const DGN_SECT_STIFFNESS& rDgnStiff, std::array<double, dgn::def::CSectionProperty::Number>& adProp) const;
	void   ConvertDgnSectStiffToCompositeSectionProperty(const CSG_SECT_POSD& rSectPos, dgn::def::CCompositeSectionProperty* pData) const;


public:
	std::map<ElemPairK, SectPairK> m_mCsgElemSect;

	CMap<T_MATL_K, T_MATL_K, CSG_MATL_ELEM, CSG_MATL_ELEM&> m_mCsgMatl;
	std::map<SectPairK, CSG_SECT_ELEM> m_mCsgSect;
	std::map<SectPairK, CSG_TRST_SECT> m_mCsgTrst;
	std::map<ElemPairK, CSG_CPSC_ELEM> m_mCsgCpsc;
	std::map<ElemPairK, CSG_CPFP_ELEM> m_mCsgCpfp;
	std::map<ElemPairK, CSG_CPCB_ELEM> m_mCsgCpcb;
	std::map<ElemPairK, CSG_DOHL_ELEM> m_mCsgDohl;
	std::map<ElemPairK, CSG_EBSL_ELEM> m_mCsgEbsl;
	std::map<ElemPairK, CSG_LTBD_ELEM> m_mCsgLtbd;
	std::map<ElemPairK, CSG_DWSP_ELEM> m_mCsgDwsp;
	std::map<ElemPairK, CSG_UFRA_ELEM> m_mCsgUfra;
	std::map<ElemPairK, CSG_MDFS_ELEM> m_mCsgMdfs;
	std::map<ElemPairK, CSG_LSHR_ELEM> m_mCsgLshr;
	

protected:
	Concurrency::concurrent_unordered_map<UINT, _DGN_FORC_CRC_ELEM> m_mForce;
	Concurrency::concurrent_unordered_map<UINT, _DGN_STRS_CRC_ELEM> m_mStress;
	Concurrency::concurrent_unordered_map<UINT, _DGN_STRS_CRC_ELEM> m_mStressP2;
	Concurrency::concurrent_unordered_map<UINT, _DGN_FORC_UL_MEMB> m_mMembForce;
	Concurrency::concurrent_unordered_map<UINT, _DGN_FORC_UL_MEMB> m_mMembForce_LT;

	T_CPGD_D m_CpgdD;

private:
	CMembCtrl* m_pMembCtrl;
	CDgnSectUtil *m_pDgnSectUtil;

	T_STCT_D m_StctD;

	T_CSGF_MEMB m_DgnForD;
	T_CSGD_COMF  m_Panelij[2];
	_DGN_STRS_CRC m_GirderStress;
	_DGN_STRS_CRC m_SlabStress;
	bool m_bReport;

};

#include "HeaderPost.h"