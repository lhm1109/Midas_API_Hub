#pragma once

#include "HeaderPre.h"

class IDgnPerformDataBase;

namespace dgn
{
    namespace def
    {
        enum class enMemb;
    }
}

namespace dgn
{
    namespace converter
    {
        enum class enProductSubType;

        class __MY_EXT_CLASS__ IProductDataBaseHelper
        {
            typedef std::pair<int, def::enMemb> MembTypePair;

        public:
            IProductDataBaseHelper();
            virtual ~IProductDataBaseHelper();

        public:
            bool HasDgnLcomList() const;
            int  GetPSubTypeList(MembTypePair TypePair, std::vector<enProductSubType>& rvPSubType) const;
            void SetDgnLcomList(T_KEY MembKey) const;
            virtual IDgnPerformDataBase* GetPerformDataBase() = 0; //안만들고 싶었는데 달리 방도가...

        protected:
            virtual bool DoHasDgnLcomList() const = 0;
            virtual int  DoGetPSubTypeList(MembTypePair TypePair, std::vector<enProductSubType>& rvPSubType) const = 0;
            virtual void DoSetDgnLcomList(T_KEY MembKey) const =0;
        };
    }
}

#include "HeaderPost.h"
