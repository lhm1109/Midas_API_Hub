#include "StdAfx.h"

#include "IProductResultInitializer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::converter;

IProductResultInitializer::IProductResultInitializer()
{
}

IProductResultInitializer::~IProductResultInitializer()
{
}

bool IProductResultInitializer::InitResultMap(const std::vector<T_KEY>& vMembKeyAll, IDgnPerformDataBase* pIDataBase, bool bRecalc) const
{
    return DoInitResultMap(vMembKeyAll, pIDataBase, bRecalc);
}