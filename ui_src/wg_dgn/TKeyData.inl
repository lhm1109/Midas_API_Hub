#pragma once

namespace dgn
{
    namespace converter
    {
        template <typename TKey, typename TData>
        TKeyData<TKey, TData>::TKeyData(std::function<void (TKey&, TData&)> pInitFunc) :
            pInitFunc(pInitFunc)
        {
        }

		template <typename TKey, typename TData>
        TKeyData<TKey, TData>::TKeyData(const TKey& Key, const TData& Data)
        {
            this->Key = Key;
            this->Data = Data;
        }

        template <typename TKey, typename TData>
        void TKeyData<TKey, TData>::Set(TKey Key, const TData& crData)
        {
            this->Key = Key;
            this->Data = crData;

            return;
        }

        template <typename TKey, typename TData>
        const TKey& TKeyData<TKey, TData>::GetKey() const
        {
            return Key;
        }

        template <typename TKey, typename TData>
        const TData& TKeyData<TKey, TData>::GetData() const
        {
            return Data;
        }

        template <typename TKey, typename TData>
        void TKeyData<TKey, TData>::DoInit()
        {
            if ( pInitFunc )
            {
                pInitFunc(Key, Data);
            }
            else
            {
                Key = TKey();
                Data = TData();
            }
        }

        template <typename TData>
        void TSpecialInit(T_KEY& rKey, TData& rData)
        {
            rKey = T_KEY();
            rData.Initialize();

            return;
        }

        template <typename TData, std::size_t _size>
        void TSpecialInit(T_KEY& rKey, std::array<TData, _size>& raData)
        {
            rKey = T_KEY();

            for ( auto& Data : raData )
            {
                Data.Initialize();
            }

            return;
        }
    }
}