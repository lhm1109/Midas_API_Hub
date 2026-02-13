#pragma once

#include <map>

namespace dgn
{
    namespace converter
    {
        struct IKeyData;
        template <typename TKey, typename TData> struct TKeyData;

        template <typename TEnum>
        class TKeyDataWrapper
        {
        public:
            TKeyDataWrapper();
            virtual ~TKeyDataWrapper();

        public:
            std::map<TEnum, IKeyData*>* GetEnumKeyDataMap();
            bool HasIKeyData(TEnum Enum) const;
            IKeyData* CreateIKeyData(TEnum Enum) const;

        protected:
            virtual IKeyData* DoCreateIKeyData(TEnum Enum) const = 0;

        private:
            std::map<TEnum, IKeyData*> m_mEnumKeyData;
        };

        template <typename TEnum, typename TKey, typename TData>
        TKeyData<TKey, TData>* CastTKeyDataByWrapper(TKeyDataWrapper<TEnum>* pWrapper, TEnum Enum);
    }
}

#include "TKeyDataWrapper.inl"
