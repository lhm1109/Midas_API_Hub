#include "StdAfx.h"

#include "IProductObjectFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::converter;

IProductObjectFactory::IProductObjectFactory(enResultDatabaseType Type) :
    m_enResultDBType(Type)
{
}

IProductObjectFactory::~IProductObjectFactory()
{
}

enResultDatabaseType IProductObjectFactory::GetResultDBType() const
{
    return m_enResultDBType;
}

const IProductDgnLcomSetter* IProductObjectFactory::CreateDgnLcomSetter() const
{
    return DoCreateDgnLcomSetter();
}

const IProductMembKeySeparator* IProductObjectFactory::CreateMembKeySeparator() const
{
    return DoCreateMembKeySeparator();
}

const IProductSaveObjectBuilder* IProductObjectFactory::CreateSaveObjectBuilder() const
{
    return DoCreateSaveObjectBuilder();
}