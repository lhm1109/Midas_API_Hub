#include "StdAfx.h"

#include "IProductDataBaseHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;
using namespace dgn::converter;

IProductDataBaseHelper::IProductDataBaseHelper()
{
}

IProductDataBaseHelper::~IProductDataBaseHelper()
{
}

bool IProductDataBaseHelper::HasDgnLcomList() const
{
    return DoHasDgnLcomList();
}

int IProductDataBaseHelper::GetPSubTypeList(MembTypePair TypePair,
                                            std::vector<enProductSubType>& rvPSubType) const
{
    return DoGetPSubTypeList(TypePair, rvPSubType);
}

void IProductDataBaseHelper::SetDgnLcomList(T_KEY MembKey) const
{
    DoSetDgnLcomList(MembKey);
}