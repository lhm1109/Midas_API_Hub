#pragma once

#include "../dgnengine/idesign/DGN_def/DgnResultBundleDef.h"

#include "HeaderPre.h"

namespace dgn
{
    namespace converter
    {
        struct __MY_EXT_CLASS__ TEngineResult final
        {
            TEngineResult();

            const dgn::def::CDgnResultBundle* GetResultBundle() const;
            const dgn::def::CDgnLesultBundle* GetLesultBundle() const;

            dgn::def::CDgnResultBundle* pEResultAll;
            dgn::def::CDgnLesultBundle* pELesultAll;
        };
    }
}

#include "HeaderPost.h"
