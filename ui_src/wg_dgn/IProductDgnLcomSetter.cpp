#include "StdAfx.h"

#include "../wg_db/DBDoc.h"

#include "ProductDBDocHelper.h"
#include "TSafeDeleteFunctor.h"

#include "IProductDgnLcomSetter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::converter;

IProductDgnLcomSetter::IProductDgnLcomSetter() :
    m_pHelper(new CProductDBDocHelper(CDBDoc::GetDocPoint()))
{
}

IProductDgnLcomSetter::~IProductDgnLcomSetter()
{
    TSafeDeleteFunctor()(m_pHelper);
}

bool IProductDgnLcomSetter::SetDgnLcomList(IDgnPerformDataBase* pIDataBase) const
{
    return DoSetDgnLcomList(pIDataBase);
}

const CProductDBDocHelper* IProductDgnLcomSetter::GetHelper() const
{
    return m_pHelper;
}