#include "StdAfx.h"

#include "IProductObjectFactory.h"

#include "ProductObjectFactoryCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;
using namespace dgn::converter;

std::map<enResultDatabaseType, const IProductObjectFactory*>
    CProductObjectFactoryCtrl::m_mObjectFactory;

CProductObjectFactoryCtrl::CProductObjectFactoryCtrl()
{
    m_mObjectFactory.clear();
}

CProductObjectFactoryCtrl::~CProductObjectFactoryCtrl()
{
}

bool CProductObjectFactoryCtrl::AddObjectFactory(const IProductObjectFactory* pFactory)
{
    if ( !pFactory )
    {
        ASSERT(0); return false;
    }

    const auto& TResultType = pFactory->GetResultDBType();

    const auto ItrResult = m_mObjectFactory.insert(std::make_pair(TResultType, nullptr));
    if ( ItrResult.second )
    {
        ItrResult.first->second = pFactory;
    }

    return true;
}

const IProductDgnLcomSetter* CProductObjectFactoryCtrl::CreateDgnLcomSetter(enResultDatabaseType enDBType)
{
    const auto* pIFactory = GetObjectFactory(enDBType);
    if ( !pIFactory )
    {
        ASSERT(0); return nullptr;
    }

    const auto* pObject = pIFactory->CreateDgnLcomSetter();
    if ( !pObject )
    {
        ASSERT(0); return nullptr;
    }

    return pObject;
}

const IProductMembKeySeparator* CProductObjectFactoryCtrl::CreateMembKeySeparator(enResultDatabaseType enDBType)
{
    const auto* pIFactory = GetObjectFactory(enDBType);
    if ( !pIFactory )
    {
        ASSERT(0); return nullptr;
    }

    const auto* pObject = pIFactory->CreateMembKeySeparator();
    if ( !pObject )
    {
        ASSERT(0); return nullptr;
    }

    return pObject;
}

const IProductSaveObjectBuilder* CProductObjectFactoryCtrl::CreateSaveObjectBuilder(enResultDatabaseType enDBType)
{
    const auto* pIFactory = GetObjectFactory(enDBType);
    if ( !pIFactory )
    {
        ASSERT(0); return nullptr;
    }

    const auto* pObject = pIFactory->CreateSaveObjectBuilder();
    if ( !pObject )
    {
        ASSERT(0); return nullptr;
    }

    return pObject;
}

const IProductObjectFactory* CProductObjectFactoryCtrl::GetObjectFactory(enResultDatabaseType enDBType)
{
    const auto ItrFind = m_mObjectFactory.find(enDBType);
    if ( ItrFind == m_mObjectFactory.end() )
    {
        ASSERT(0); return nullptr;
    }

    return ItrFind->second;
}