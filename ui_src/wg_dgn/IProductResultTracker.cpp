#include "StdAfx.h"

#include "IProductResultSaver.h"

#include "IProductResultTracker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;
using namespace dgn::converter;

IProductResultTracker::IProductResultTracker(const CProductDBDocHelper* pDBDocHelper) :
    m_pDBDocHelper(pDBDocHelper)
{
    if ( !m_pDBDocHelper )
    {
        ASSERT(0);
    }

    m_mKeyFlag.clear();
}

IProductResultTracker::~IProductResultTracker()
{
}

bool IProductResultTracker::IsSkipCondition(T_KEY MembKey,
                                            const IProductResultSaver* pISaver) const
{
    const auto& CurKeySet = DoCreateKeySet(MembKey, pISaver);

    const auto itr = m_mKeyFlag.find(CurKeySet);
    return (itr != m_mKeyFlag.end());
}

bool IProductResultTracker::SetDone(T_KEY MembKey,
                                    const IProductResultSaver* pISaver)
{
    const auto& CurKeySet = DoCreateKeySet(MembKey, pISaver);

    const auto ItrInsert = m_mKeyFlag.insert(std::make_pair(CurKeySet, Flag::Done));
    if ( !ItrInsert.second )
    {
        //? KeySet이 중복입니다.
        ASSERT(0); return false;
    }

    return true;
}

const CProductDBDocHelper* IProductResultTracker::GetDBDocHelper() const
{
    return m_pDBDocHelper;
}