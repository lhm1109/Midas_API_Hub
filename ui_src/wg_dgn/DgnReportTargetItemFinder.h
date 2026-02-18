#pragma once

#include "../dgnengine/idesign/DGN_lib/TEnumList.h"

#include "HeaderPre.h"

#pragma region 
/// 전방선언
namespace dgn
{
    namespace link
    {
        class IDgnDoc;
    }

    namespace item
    {
        enum class enDgnResultItem : unsigned int;
    }
}
#pragma endregion

class IDgnPerformDataBase;

class __MY_EXT_CLASS__ CDgnResultExistItemFinder
{
#pragma region 
    /// typedef 정의
    typedef dgn::link::IDgnDoc IDgnDoc;
    typedef dgn::item::enDgnResultItem enDgnResultItem;
    typedef dgn::lib::TEnumList<enDgnResultItem> CItemList;
#pragma endregion

public:
    CDgnResultExistItemFinder();
    virtual ~CDgnResultExistItemFinder();

public:
    bool Initialize(IDgnDoc* const pDgnDoc, IDgnPerformDataBase* const pDataBase);
    int  GetTargetItemList(long MembKey, CItemList& rItemList) const;

protected:
    virtual int DoGetTargetItemList(long MembKey, CItemList& rItemList) const = 0;

    IDgnDoc* const GetDgnDoc() const;
    IDgnPerformDataBase* GetDataBase() const;

private:
    bool IsInitialize() const;

private:
    bool m_bInitialize;
    IDgnDoc* const m_pDgnDoc;
    IDgnPerformDataBase* const m_pDataBase;
};

#include "HeaderPost.h"
