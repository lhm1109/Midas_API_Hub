#pragma once

#include "../../dgnengine/include/IDGN_frx/AllIDgnInclude.h"
#include "../../dgnengine/src/DGN_DB/TDBData.h"

namespace dgn
{
    namespace converter
    {
        template <typename Type>
        struct TDBDataFunctor final
        {
            TDBDataFunctor();

            const Type* operator() (FIterator ItrRefer) const;

        private:
            template <typename T> using TDBData = db::TDBData<T>;
        };
    }
}

#include "TDBDataFunctor.inl"
