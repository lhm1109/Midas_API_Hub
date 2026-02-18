#pragma once

#include "HeaderPre.h"

namespace dgn
{
    namespace converter
    {
        class __MY_EXT_CLASS__ IProductMembKeySeparator
        {
        public:
            IProductMembKeySeparator();
            virtual ~IProductMembKeySeparator();

        public:
            int GetSuperSubMembKeyList(T_KEY SuperMembKey, std::vector<T_KEY>& rvMembKeyAll) const;
        
        protected:
            virtual int DoGetSuperSubMembKeyList(T_KEY SuperMembKey, std::vector<T_KEY>& rvMembKeyAll) const = 0;
        };
    }
}

#include "HeaderPost.h"
