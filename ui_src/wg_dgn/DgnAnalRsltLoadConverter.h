#pragma once

#include "../../dgnengine/idesign/DGN_link/TAnalRsltLoadConverter.h"

#include "TDgnAnalRsltKey.h"

class CDBDoc;

class CDgnAnalRsltLoadConverter final : public dgn::link::TAnalRsltLoadConverter<
    T_KEY,
    TDgnAnalRsltKey<enCaseKeyType, T_KEY>,
    TDgnAnalRsltKey<enLcomKeyType, T_KEY>,
    T_KEY>
{
public:
    typedef T_KEY MCasKey;
    typedef TDgnAnalRsltKey<enCaseKeyType, T_KEY> LoadKey;
    typedef TDgnAnalRsltKey<enLcomKeyType, T_KEY> SuperKey;
    typedef T_KEY SubKey;

public:
    CDgnAnalRsltLoadConverter(CDBDoc* pDBDoc);
    virtual ~CDgnAnalRsltLoadConverter() override;

protected:
    virtual int GetProductDCasKeyList(std::vector<MCasKey>& vKeyDCas) const override;
    virtual int GetProductCaseKeyList(std::vector<LoadKey>& vKeyCase) const override;

    virtual int GetSuperKeyList(std::vector<SuperKey>& vSuperKey) const override;
    virtual bool HasSub(const SuperKey& SuperKey) const override;
    virtual bool GetSubKeyList(const SuperKey& SuperKey, std::vector<SubKey>& vSubKey) const override;

    virtual bool ConvertProductLcom(const SuperKey& SuperKey, dgn::def::TLcomD& Lcom) const override;
    virtual bool ConvertProductLcom(const SuperKey& SuperKey, const SubKey& SubKey, dgn::def::TLcomD& Lcom) const override;

    virtual dgn::def::enLoadCase GetProductCaseType(const LoadKey& KeyCase) const override;
    virtual int GetProductAnalRsltCaseType(const dgn::def::enLoadCase& enCase) const override;
    virtual int GetEngineAnalRsltCaseKey(const dgn::def::enLoadCase& enCase, const dgn::def::DKey& KeyCase) const override;
    virtual int GetEngineAnalRsltDCasKey(const dgn::def::DKey& KeyDCas) const override;

public:
    MCasKey GetPMCasKey(const dgn::def::DKey& KeyDCas) const;
    LoadKey GetPCaseKey(const dgn::def::DKey& KeyCase) const;

private:
    int GetProductCaseKeyList(const enCaseKeyType KeyType, std::vector<LoadKey>& vKeyCase) const;

private:
    CDBDoc* GetDBDoc() const;
    CDBDoc* m_pDBDoc;
};