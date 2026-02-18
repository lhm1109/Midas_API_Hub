#include "StdAfx.h"

#include "IProductMembKeySeparator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::converter;

IProductMembKeySeparator::IProductMembKeySeparator()
{
}

IProductMembKeySeparator::~IProductMembKeySeparator()
{
}

int IProductMembKeySeparator::GetSuperSubMembKeyList(T_KEY SuperMembKey, 
                                                     std::vector<T_KEY>& rvMembKeyAll) const
{
    return DoGetSuperSubMembKeyList(SuperMembKey, rvMembKeyAll);
}