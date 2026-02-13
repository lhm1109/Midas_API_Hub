#include "StdAfx.h"

#include "../dgnengine/idesign/DGN_lib/TemporaryObject.h"
#include "../dgnengine/idesign/DGN_lib/DGNCompare.h"

#include "TProductResultSaveLcomUpdater.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::converter;

enProductResultSaveLcomUpdateFlag TProductResultSaveLcomUpdater::UpdateKeyRatio(enUpdateType Type,
                                                                                T_KEY EDgnLcomKey,
                                                                                double dCurRatio)
{
    if ( EDgnLcomKey == T_KEY() )
    {
        ASSERT(0); return enUpdateFlag::Failure;
    }

    if ( LT0(dCurRatio) )
    {
        ASSERT(0); return enUpdateFlag::Failure;
    }

    const auto bInit = [&]() 
    {
        const auto itr = m_mbTypeInit.find(Type);
        if (itr == m_mbTypeInit.end())
        {
            ASSERT(0); return true;
        }

        return itr->second;
    }();

    if ( bInit )
    {
        m_mbTypeInit[Type] = false;
        SetKeyRatio(Type, EDgnLcomKey, dCurRatio);
        return enUpdateFlag::Current;
    }
    else
    {
        const auto& PreKeyRatio = GetKeyRatio(Type);
        const auto& dPreRatio = PreKeyRatio.GetData();
        if ( MT(dCurRatio, dPreRatio) )
        {
            SetKeyRatio(Type, EDgnLcomKey, dCurRatio);
            return enUpdateFlag::Current;
        }
        else
        {
            return enUpdateFlag::Previous;
        }
    }
}

T_KEY TProductResultSaveLcomUpdater::GetMaxKey(enUpdateType Type) const
{
    const auto& KeyRatio = GetKeyRatio(Type);
    return KeyRatio.GetKey();
}

double TProductResultSaveLcomUpdater::GetMaxRatio(enUpdateType Type) const
{
    const auto& KeyRatio = GetKeyRatio(Type);
    return KeyRatio.GetData();
}

void TProductResultSaveLcomUpdater::SetKeyRatio(enUpdateType Type,
                                                T_KEY EDgnLcomKey,
                                                double dCurRatio)
{
    m_mTypeKeyRatio[Type].Set(EDgnLcomKey, dCurRatio);

    return;
}

const TKeyData<T_KEY, double>& TProductResultSaveLcomUpdater::GetKeyRatio(enUpdateType Type) const
{
    const auto itr = m_mTypeKeyRatio.find(Type);
    if ( itr == m_mTypeKeyRatio.end() )
    {
        ASSERT(0); return lib::TemporaryObject<TKeyRatio>();
    }

    return itr->second;
}
