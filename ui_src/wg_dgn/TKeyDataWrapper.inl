#pragma once

#include "TKeyData.h"
#include "TSafeDeleteFunctor.h"

namespace dgn
{
    namespace converter
    {
        template <typename TEnum>
        TKeyDataWrapper<TEnum>::TKeyDataWrapper()
        {
            m_mEnumKeyData.clear();
        }

        template <typename TEnum>
        TKeyDataWrapper<TEnum>::~TKeyDataWrapper()
        {
            for ( auto& Pair : m_mEnumKeyData )
            {
                auto& pIKeyData = Pair.second;

                TSafeDeleteFunctor()(pIKeyData);
            }
        }

        template <typename TEnum>
        std::map<TEnum, IKeyData*>* TKeyDataWrapper<TEnum>::GetEnumKeyDataMap()
        {
            return &m_mEnumKeyData;
        }

        template <typename TEnum>
        bool TKeyDataWrapper<TEnum>::HasIKeyData(TEnum Enum) const
        {
            const auto& itr = m_mEnumKeyData.find(Enum);
            return (itr != m_mEnumKeyData.end());
        }

        template <typename TEnum>
        IKeyData* TKeyDataWrapper<TEnum>::CreateIKeyData(TEnum Enum) const
        {
            return DoCreateIKeyData(Enum);
        }

        template <typename TEnum, typename TKey, typename TData>
        TKeyData<TKey, TData>* CastTKeyDataByWrapper(TKeyDataWrapper<TEnum>* pWrapper,
                                                     TEnum Enum)
        {
            typedef std::map<TEnum, IKeyData*> mapEnumIKeyData;

            if ( Enum == TEnum::None )
            {
                ASSERT(0); return nullptr;
            }

            auto* pEnumKeyDataMap = pWrapper->GetEnumKeyDataMap();
            if ( !pWrapper->HasIKeyData(Enum) )
            {
                auto& ItrResult = pEnumKeyDataMap->insert(std::make_pair(Enum, nullptr));
                if ( !ItrResult.second )
                {
                    ASSERT(0); return nullptr;
                }

                ItrResult.first->second = pWrapper->CreateIKeyData(Enum);
                ItrResult.first->second->Init();
            }

            auto* pIKeyData = pEnumKeyDataMap->at(Enum);
            auto* pTKeyData = dynamic_cast<TKeyData<TKey, TData>*>(pIKeyData);
            if ( !pTKeyData )
            {
                ASSERT(0); return nullptr;
            }

            return pTKeyData;
        }
    }
}