#include "StdAfx.h"

#include "../dgnengine/idesign/DGN_lib/Macro.h"
#include "../dgnengine/idesign/DGN_def/Rebar2D.h"
#include "../dgnengine/idesign/DGN_def/DgnTypeEnumDef.h"
#include "../dgnengine/idesign/DGN_def/DgnUnitDef.h"

#include "../wg_dgn/DgnStruct.h"

#include "ProductDBDocHelperWrapper.h"
#include "ProductDBDocHelperPreDecl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;
using namespace dgn::converter;

CProductDBDocHelperWrapper::CProductDBDocHelperWrapper()
{
}

CProductDBDocHelperWrapper::~CProductDBDocHelperWrapper()
{
}

IKeyData* CProductDBDocHelperWrapper::DoCreateIKeyData(TEnum Enum) const
{
    typedef Macro::TNumber<def::enRebar2D>            Rebar2DNumb;
    typedef std::array<tRebar2D, 4>                   RbmsDList;
    typedef TKeyData<T_KEY, T_WALL_D>                 TWallKeyData;
    typedef std::map<T_KEY, T_KEY>                    mapKeyPair;

    switch ( Enum )
    {
    case TEnum::Element:
        return new TKeyData<T_KEY, T_ELEM_D>(TSpecialInit<T_ELEM_D>);
    case TEnum::RebarBeam:
        return new TKeyData<T_KEY, T_REBB_D>(TSpecialInit<T_REBB_D>);
    case TEnum::RebarColumn:
        return new TKeyData<T_KEY, T_REBC_D>(TSpecialInit<T_REBC_D>);
    case TEnum::RebarBrace:
        return new TKeyData<T_KEY, T_REBR_D>(TSpecialInit<T_REBR_D>);
    case TEnum::RebarWall:
        return new TKeyData<T_KEY, T_REBW_D>(TSpecialInit<T_REBW_D>);
    case TEnum::RebarMeshedSlab:
        return new TKeyData<T_KEY, RbmsDList>(TSpecialInit<tRebar2D, 4>);
    case TEnum::WallKeyData:
        return new TKeyData<T_KEY, TWallKeyData>(
            [] (T_KEY& rKey, TWallKeyData& rData)
        {
            rKey = T_KEY();
            rData.Key = T_KEY();
            rData.Data.Initialize();
        });
    case TEnum::WallKeyUnion:
        return new TKeyData<T_KEY, T_WALL_K>();
    case TEnum::Story:
        return new TKeyData<T_KEY, T_STOR_D>(TSpecialInit<T_STOR_D>);
    case TEnum::WallMark:
        return new TKeyData<T_KEY, T_WMAK_D>(TSpecialInit<T_WMAK_D>);
    case TEnum::RepMembKeyOnSbdoMember:
        return new TKeyData<T_KEY, mapKeyPair>(
            [] (T_KEY& rKey, mapKeyPair& rData)
        {
            rKey = T_KEY();
            rData.clear();
        });
    case TEnum::Material_AttrOnly:
        return new TKeyData<T_KEY, T_MATD_D>(TSpecialInit<T_MATD_D>);
    case TEnum::CodeUnitType:
        return new TKeyData<enDgn, enUnitType>();
    default: ASSERT(0); return nullptr;
    }
}