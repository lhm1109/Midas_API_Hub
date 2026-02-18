#pragma once

#include "HeaderPre.h"

class IDgnPerformDataBase;

namespace dgn
{
    namespace converter
    {
        class __MY_EXT_CLASS__ IProductResultInitializer
        {
        public:
            IProductResultInitializer();
            virtual ~IProductResultInitializer();

        public:
            bool InitResultMap(const std::vector<T_KEY>& vMembKeyAll,
                               IDgnPerformDataBase* pIDataBase, bool bRecalc) const;

        protected:
            virtual bool DoInitResultMap(const std::vector<T_KEY>& vMembKeyAll,
                                         IDgnPerformDataBase* pIDataBase, bool bRecalc) const = 0;
        };
    }
}

#include "HeaderPost.h"
