#include "StdAfx.h"

#include "../dgnengine/idesign/DGN_lib/DgnProgObserver.h"

#include "../wg_db/DBDoc.h"

#include "IProductSaveObjectBuilder.h"
#include "ProductResultSaveController.h"
#include "TSafeDeleteFunctor.h"
#include "ProductDBDocHelper.h"
#include "IProductResultInitializer.h"
#include "ProductObjectFactoryCtrl.h"
#include "IProductMembKeySeparator.h"

#include "IEngineResultConvert.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::lib;
using namespace dgn::def;
using namespace dgn::converter;

IEngineResultConvert::IEngineResultConvert(const CProductDBDocHelper* pDBDocHelper,
	enResultDatabaseType enDBType, bool bRecalc) :
	m_pDBDocHelper(pDBDocHelper),
	m_pSeparator(CProductObjectFactoryCtrl::CreateMembKeySeparator(enDBType)),
	m_pBuilder(CProductObjectFactoryCtrl::CreateSaveObjectBuilder(enDBType)),
	m_pController(new CProductResultSaveController(pDBDocHelper, m_pBuilder)), m_bRecalc(bRecalc)
{
}

IEngineResultConvert::~IEngineResultConvert()
{
    TSafeDeleteFunctor()(m_pSeparator);
    TSafeDeleteFunctor()(m_pBuilder);
    TSafeDeleteFunctor()(m_pController);
}

bool IEngineResultConvert::Initialize(const T_KEY_LIST& SuperMembKeyList, 
                                      IDgnPerformDataBase* pIDataBase)
{
    if ( SuperMembKeyList.IsEmpty() )
    {
        ASSERT(0); return false;
    }

    std::vector<T_KEY> vMembKeyAll;
    vMembKeyAll.clear();

    const auto nSize = SuperMembKeyList.GetSize();
    for ( int nIdx = 0; nIdx < nSize; ++nIdx )
    {
        const auto& SuperMembKey = SuperMembKeyList[nIdx];
        if ( !m_pSeparator->GetSuperSubMembKeyList(SuperMembKey, vMembKeyAll) )
        {
            ASSERT(0); return false;
        }
    }

    std::unique_ptr<const IProductResultInitializer> 
        pInitializer(m_pBuilder->CreateInitializer());

    for ( auto MembKey : vMembKeyAll )
    {
        if ( !m_pController->MakeISaverList(MembKey, pIDataBase) )
        {
            ASSERT(0); return false;
        }
    }

    if ( !pInitializer->InitResultMap(vMembKeyAll, pIDataBase, m_bRecalc) )
    {
        ASSERT(0); return false;
    }

    return true;
}

int IEngineResultConvert::ConvertResults(CDgnProgObserver* pProgObserver, 
                                         const T_KEY_LIST& SuperMembKeyList, 
                                         IDgnPerformDataBase* pIDataBase)
{
    if ( SuperMembKeyList.IsEmpty() )
    {
        ASSERT(0); return false;
    }

    std::vector<T_KEY> vMembKeyAll;
    vMembKeyAll.clear();

    const auto dSize = static_cast<int>(SuperMembKeyList.GetSize());

    const auto& nPercent1 = static_cast<int>(ceil(dSize * 0.05));
    const auto& nPercent2 = 1;
    const auto& nPercent = max(nPercent1, nPercent2);

    const auto nSize = SuperMembKeyList.GetSize();
    for ( int nIdx = 0; nIdx < nSize; ++nIdx )
    {
        const auto& SuperMembKey = SuperMembKeyList[nIdx];
        if ( !m_pSeparator->GetSuperSubMembKeyList(SuperMembKey, vMembKeyAll) )
        {
            ASSERT(0); return false;
        }
    }
    Macro::UniqueSort(vMembKeyAll);

    int nPassCount = 0;
    const auto nSizeAll = static_cast<int>(vMembKeyAll.size());
    for ( int nIdx = 0; nIdx < nSizeAll; ++nIdx )
    {
        const auto& NextIdx    = nIdx + 1;
        const auto& CurMembKey = vMembKeyAll[nIdx];

        if ( !ConvertResult(CurMembKey, pIDataBase) )
        {
            if(pProgObserver != nullptr)
                pProgObserver->SetCurrentStep(NextIdx);
            continue;
        }

        if ( NextIdx  % nPercent == 0 || 
             NextIdx == nSizeAll )
        {
            if (pProgObserver != nullptr)
                pProgObserver->SetCurrentStep(NextIdx);
        }

        ++nPassCount;
    }

    return nPassCount;
}

bool IEngineResultConvert::ConvertResult(T_KEY MembKey,
                                         IDgnPerformDataBase* pIDataBase) const
{
    if ( !IsValidMembKey(MembKey) )
    {
        ASSERT(0); return false;
    }

    if ( !pIDataBase )
    {
        ASSERT(0); return false;
    }

    if ( !m_pController->Play(MembKey, pIDataBase) )
    {
        ASSERT(0); return false;
    }

    return true;
}

bool IEngineResultConvert::IsValidMembKey(T_KEY MembKey) const
{
    return ( MembKey != T_KEY() );
}