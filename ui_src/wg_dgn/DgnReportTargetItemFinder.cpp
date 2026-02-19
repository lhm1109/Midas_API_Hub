#include "StdAfx.h"

#include "DgnReportTargetItemFinder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDgnResultExistItemFinder::CDgnResultExistItemFinder() :
    m_bInitialize(false),
    m_pDgnDoc(nullptr),
    m_pDataBase(nullptr)
{
}

CDgnResultExistItemFinder::~CDgnResultExistItemFinder()
{
}

bool CDgnResultExistItemFinder::Initialize(IDgnDoc* const pDgnDoc, 
                                           IDgnPerformDataBase* const pDataBase)
{
    if ( !pDgnDoc )
    {
        ASSERT(0); return false;
    }

    if ( !pDataBase )
    {
        ASSERT(0); return false;
    }

    //여기서만ㅠㅠ
    const_cast<IDgnDoc*>(m_pDgnDoc) = pDgnDoc;
    const_cast<IDgnPerformDataBase*>(m_pDataBase) = pDataBase;

    m_bInitialize = true;

    return true;
}

int CDgnResultExistItemFinder::GetTargetItemList(long MembKey, 
                                                CItemList& rItemList) const
{
    if ( !IsInitialize() )
    {
        //Initialize하고 들어오심요.
        ASSERT(0); return 0;
    }

    return DoGetTargetItemList(MembKey, rItemList);
}

CDgnResultExistItemFinder::IDgnDoc* const CDgnResultExistItemFinder::GetDgnDoc() const
{
    return m_pDgnDoc;
}

IDgnPerformDataBase* CDgnResultExistItemFinder::GetDataBase() const
{
    return m_pDataBase;
}

bool CDgnResultExistItemFinder::IsInitialize() const
{
    return m_bInitialize;
}