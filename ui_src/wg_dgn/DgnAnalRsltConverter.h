#pragma once

#include <mutex>

#include "../../dgnengine/idesign/DGN_def/IAnalRsltConverter.h"
#include "../../dgnengine/idesign/DGN_def/TAnalRsltKey.h"

#include "TDgnAnalRsltKey.h"

class CDBDoc;
class CPostCtrl;
class CAttrCtrl;

class CDgnAnalRsltMembConverter;
class CDgnAnalRsltLoadConverter;

#define notIndex -1
typedef std::pair<dgn::def::DKey, dgn::def::DKey> pairDKey;

template <typename TData>
class TPostAnalRslt
{
public:
	TPostAnalRslt();
	virtual ~TPostAnalRslt();

public:
	void InitData(int nCapacity=0);
	void Reserve(int nCapacity);
	bool Exist(const pairDKey& pairKey) const;
	bool Exist(const dgn::def::DKey& iKey1st, const dgn::def::DKey& iKey2nd) const;
	int  GetIndex(const pairDKey& pairKey) const;
	int  GetIndex(const dgn::def::DKey& iKey1st, const dgn::def::DKey& iKey2nd) const;
	void SetData(const pairDKey& pairKey, const TData& Data1, const TData& Data2);
	void SetData(const dgn::def::DKey& iKey1st, const dgn::def::DKey& iKey2nd, const TData& Data1, const TData& Data2);
	bool GetData(const pairDKey& pairKey, TData& Data1, TData& Data2) const;
	bool GetData(const dgn::def::DKey& iKey1st, const dgn::def::DKey& iKey2nd, TData& Data1, TData& Data2) const;
	int  GetDataCount();

private:
	std::unordered_map<pairDKey, UINT, pair_hash>  m_mapKey;
	std::vector<std::array<TData, 2>> m_vecData;
};

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnAnalRsltConverter final :
    public dgn::def::IAnalRsltConverter
{
private:
    typedef TDgnAnalRsltKey<enMembKeyType, T_KEY> PMembKey;
    typedef TDgnAnalRsltKey<enCaseKeyType, T_KEY> PCaseKey;

    typedef dgn::def::TAnalRsltRawDataNode<double, nDeforms>                     CRawDataNode;
    typedef dgn::def::TAnalRsltRawDataNode<double, nReact>                       CRawDataReact;
    typedef dgn::def::TAnalRsltRawData1DElem<double, nDisps, dgn::def::ElemDiv8> CRawData1D3;
    typedef dgn::def::TAnalRsltRawData1DElem<double, n6Dofs, dgn::def::ElemDiv8> CRawData1D6;
    typedef dgn::def::TAnalRsltRawData1DElem<double, n7Dofs, dgn::def::ElemDiv8> CRawData1D9;
    typedef dgn::def::TAnalRsltRawData2DElem<double, n2DForce>                   CRawData2DF;
    typedef dgn::def::TAnalRsltRawData2DElem<double, n2DStress>                  CRawData2DS;

public:
    CDgnAnalRsltConverter(CDBDoc* pDBDoc, T_KEY_LIST& aKeyElem);
    CDgnAnalRsltConverter(CDBDoc* pDBDoc, T_KEY_LIST& aKeyElem, bool bDirectProduct);
    virtual ~CDgnAnalRsltConverter();

public:
    virtual int GetSupportAnalSubType(std::vector<dgn::def::enAnalSubType>& vAnalSubType) const override;

    virtual bool GetAnalData(const dgn::def::DKey& KeyMCas, const dgn::def::enAnalType AnalType, const dgn::def::DKey& KeyCase,
        const std::vector<dgn::def::DKey>& vKeyNode, std::vector<CRawDataNode>& vData) const override;

    virtual bool GetAnalData(const dgn::def::DKey& KeyMCas, const dgn::def::enAnalType AnalType, const dgn::def::DKey& KeyCase,
        const std::vector<dgn::def::DKey>& vKey1D, std::vector<CRawData1D3>& vData) const override;

    virtual bool GetAnalData(const dgn::def::DKey& KeyMCas, const dgn::def::enAnalType AnalType, const dgn::def::DKey& KeyCase,
        const std::vector<dgn::def::DKey>& vKey1D, std::vector<CRawData1D6>& vData) const override;

    virtual bool GetAnalData(const dgn::def::DKey& KeyMCas, const dgn::def::enAnalType AnalType, const dgn::def::DKey& KeyCase,
        const std::vector<dgn::def::DKey>& vKey1D, std::vector<CRawData1D9>& vData) const override;

    virtual bool GetAnalData(const dgn::def::DKey& KeyMCas, const dgn::def::enAnalType AnalType, const dgn::def::DKey& KeyCase,
        const std::vector<dgn::def::DKey>& vKey2D, std::vector<CRawData2DF>& vData) const override;

    virtual bool GetAnalData(const dgn::def::DKey& KeyMCas, const dgn::def::enAnalType AnalType, const dgn::def::DKey& KeyCase,
        const std::vector<dgn::def::DKey>& vKey2D, std::vector<CRawData2DS>& vData) const override;

    virtual bool GetAnalData(const dgn::def::DKey& KeyMCas, const dgn::def::enAnalType AnalType, const dgn::def::DKey& KeyCase,
        const std::vector<dgn::def::DKey>& vKeyNode, std::vector<CRawDataReact>& vData) const override;

    virtual void InitData() override;

public:
    const CDgnAnalRsltMembConverter* GetProductMembConverter() const;
    const CDgnAnalRsltLoadConverter* GetProductLoadConverter() const;

protected:
    virtual dgn::def::IAnalRsltMembConverter* CreateMembConverter() const override;
    virtual dgn::def::IAnalRsltLoadConverter* CreateLoadConverter() const override;

private:
    PCaseKey ConvertCaseKey(const dgn::def::DKey& KeyCase) const;
    bool ConvertKey(const dgn::def::enAnalSubType& AnalSubType,
        const std::vector<dgn::def::DKey>& vKey1D, std::vector<PMembKey>& vKey) const;

    bool SetPostCtrlLoadCase(const PCaseKey KeyCase) const;

    bool GetAnalRawDataNode(const PCaseKey KeyCase, const std::vector<PMembKey>& vKey, std::vector<CRawDataNode>& vData) const;
    bool GetAnalRawDataReact(const PCaseKey KeyCase, const std::vector<PMembKey>& vKey, std::vector<CRawDataReact>& vData) const;
    bool GetAnalRawDataDisp1D(const PCaseKey KeyCase, const std::vector<PMembKey>& vKey, std::vector<CRawData1D3>& vData) const;
    bool GetAnalRawData1D6F(const PCaseKey KeyCase, const std::vector<PMembKey>& vKey, std::vector<CRawData1D6>& vData) const;
    bool GetAnalRawData1D9F(const PCaseKey KeyCase, const std::vector<PMembKey>& vKey, std::vector<CRawData1D9>& vData) const;
    bool GetAnalRawData2DF(const PCaseKey KeyCase, const std::vector<PMembKey>& vKey, std::vector<CRawData2DF>& vData) const;
    bool GetAnalRawData2DS(const PCaseKey KeyCase, const std::vector<PMembKey>& vKey, std::vector<CRawData2DS>& vData) const;

    bool SetNeutralUnit() const;

private:
    CDBDoc* GetDBDoc() const;
    CPostCtrl* GetPostCtrl() const;
    CAttrCtrl* GetAttrCtrl() const;

    void ClearData();

    CDBDoc* m_pDBDoc;
    T_KEY_LIST m_aElemList;

	//TPostAnalRslt<T_STRB_D> m_StrbData;
	//TPostAnalRslt<T_SBCF_D> m_SbcfData;
	//TPostAnalRslt<T_STRT_D> m_StrtData;
	//TPostAnalRslt<T_STRW_D> m_StrwData;
	//TPostAnalRslt<T_DISP_D> m_ElemDispData;
	//TPostAnalRslt<T_DISP_D> m_NodalDispData;
	//TPostAnalRslt<T_REAC_D> m_ReacData;
	//TPostAnalRslt<T_STRP_DL> m_StrpData;
	//TPostAnalRslt<T_SPCF_DL> m_SpcfData;

	static std::mutex m_mutex;
};

#include "HeaderPost.h"

#include "DgnAnalRsltConverter.inl"