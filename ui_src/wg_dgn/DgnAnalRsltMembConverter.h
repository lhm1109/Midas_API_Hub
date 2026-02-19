#pragma once

#include "../../dgnengine/idesign/DGN_link/TAnalRsltMembConverter.h"
#include "../../dgnengine/idesign/DGN_def/TAnalRsltKey.h"

#include "TDgnAnalRsltKey.h"

typedef UINT T_KEY;
typedef CArray<T_KEY, T_KEY> T_KEY_LIST;

class CDBDoc;

class CDgnAnalRsltMembConverter final :
    public dgn::link::TAnalRsltMembConverter<TDgnAnalRsltKey<enMembKeyType, T_KEY>>
{
public:
    typedef TDgnAnalRsltKey<enMembKeyType, T_KEY> MembKey;

public:
    CDgnAnalRsltMembConverter(CDBDoc* pDBDoc, T_KEY_LIST* paKeyElem);
    virtual ~CDgnAnalRsltMembConverter() override;

protected:
    virtual int DoGetKeyList(const dgn::def::enAnalSubType& AnalSubType, std::vector<MembKey>& vKey) const override;
    virtual int DoGetElemPosition1D(const MembKey& KeyMember, const dgn::def::enAnalSubType& AnalSubType, std::vector<dgn::def::TElemPos1DMemb<MembKey>>& vElemPos) const override;
    virtual bool DoGetPosition1D(const MembKey& KeyMember, dgn::def::CPosition1D& Position1D) const override;

protected:
    virtual dgn::def::enElemType DoGetElemType(const MembKey& KeyMember) const override;
    virtual bool DoIsValidAnalSubType(const MembKey& KeyMember, const dgn::def::enAnalSubType AnalSubType) const override;

protected:
    virtual int GetProductMemberKeyList(std::vector<MembKey>& vKeyMember) const override;
    virtual int GetProductNodeKeyList(std::vector<MembKey>& vKeyNode) const override;

public:
    int GetMemberKeyList(const enMembKeyType& KeyType, std::vector<MembKey>& vKey) const;
    int GetNodeKeyList(const enMembKeyType& KeyType, std::vector<MembKey>& vKey) const;
    bool GetPKeyList(const dgn::def::enAnalSubType& AnalSubType,
        const std::vector<dgn::def::DKey>& vKey, std::vector<MembKey>& vPKey) const;

private:
    T_KEY GetElemKeyFront(const MembKey& KeyMember) const;
    int GetKeyList1D(const dgn::def::enAnalSubType& AnalSubType, std::vector<MembKey>& vKey) const;
    int GetKeyList2D(const dgn::def::enAnalSubType& AnalSubType, std::vector<MembKey>& vKey) const;
    int GetKeyListNode(const dgn::def::enAnalSubType& AnalSubType, std::vector<MembKey>& vKey) const;
    T_KEY_LIST* GetElemKeyList() const;

private:
    CDBDoc* GetDBDoc() const;
    CDBDoc* m_pDBDoc;
    T_KEY_LIST* m_paKeyElem;
};