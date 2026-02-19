#pragma once

namespace dgn
{
    namespace converter
    {
        //이거 그냥 다 숫자로 되어 있길래 우선 여기에만 적용.
        enum class enProductDgnMode
        {
            None,
            Design,
            Checking,
            Number
        };

        enum class enProductRebarLayer
        {
            None,
            Single,
            Double,
            Number
        };
    }
}