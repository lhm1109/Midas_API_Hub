#pragma once

#include "HeaderPre.h"

namespace dgn
{
    namespace converter
    {
        struct __MY_EXT_CLASS__ TSafeDeleteFunctor final
        {
        public:
            TSafeDeleteFunctor();
            ~TSafeDeleteFunctor();

            template <typename TType>
            void operator() (TType& rp) const;
        };
    }
}

#include "HeaderPost.h"

#include "TSafeDeleteFunctor.inl"
