#pragma once

#include "../wg_dgn/TKeyDataWrapper.h"

namespace dgn
{
    namespace converter
    {
        enum class enProductDBDocHelperWrapper
        {
            None,
            Element,
            RebarBeam,
            RebarColumn,
            RebarBrace,
            RebarWall,
            RebarMeshedSlab,
            WallKeyData,
            WallKeyUnion,
            Story,
            WallMark,
            RepMembKeyOnSbdoMember,
            Material_AttrOnly,
            CodeUnitType,

            Number
        };

        class CProductDBDocHelperWrapper final :
            public TKeyDataWrapper<enProductDBDocHelperWrapper>
        {
            typedef enProductDBDocHelperWrapper TEnum;

        public:
            CProductDBDocHelperWrapper();
            virtual ~CProductDBDocHelperWrapper() override;

        protected:
            virtual IKeyData* DoCreateIKeyData(TEnum Enum) const override;
        };
    }
}