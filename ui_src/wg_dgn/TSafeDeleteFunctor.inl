#pragma once

namespace dgn
{
    namespace converter
    {
        template <typename TType>
        void TSafeDeleteFunctor::operator()(TType& rp) const
        {
            if ( rp )
            {
                delete rp;
                rp = nullptr;
            }
        }
    }
}