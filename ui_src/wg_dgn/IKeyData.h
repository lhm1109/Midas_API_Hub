#pragma once

#include "HeaderPre.h"

namespace dgn
{
    namespace converter
    {
        struct __MY_EXT_CLASS__ IKeyData
        {
            IKeyData();

            void Init();

        protected:
            virtual void DoInit() = 0;
        };
    }
}

#include "HeaderPost.h"
