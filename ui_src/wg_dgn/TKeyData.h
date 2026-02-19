#pragma once

#include <functional>

#include "IKeyData.h"

namespace dgn
{
    namespace converter
    {
        template <typename TKey, typename TData>
        struct TKeyData final : 
            public IKeyData
        {
            TKey  Key;
            TData Data;

            TKeyData(std::function<void (TKey&, TData&)> pInitFunc = nullptr);
            TKeyData(const TKey& Key, const TData& Data);

            void Set(TKey Key, const TData& crData);

            const TKey&  GetKey() const;
            const TData& GetData() const;

        protected:
            virtual void DoInit() override;

        private:
            std::function<void (TKey&, TData&)> pInitFunc;
        };

        template <typename TData>
        void TSpecialInit(T_KEY& rKey, TData& rData);

        template <typename TData, std::size_t _size>
        void TSpecialInit(T_KEY& rKey, std::array<TData, _size>& raData);
    }
}

#include "TKeyData.inl"
