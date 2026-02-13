#include "stdafx.h"

#include "../dgnengine/idesign/DGN_def/DgnTypeEnumDef.h"

#include "IDgnPerformDataBaseRC.h"

using namespace dgn::def;

IDgnPerformDataBaseRC::IDgnPerformDataBaseRC()
{
}

IDgnPerformDataBaseRC::~IDgnPerformDataBaseRC()
{
}

dgn::def::enDgn IDgnPerformDataBaseRC::GetDgnType()
{
    return enDgn::RCS;
}

bool IDgnPerformDataBaseRC::IsDesigning()
{
    switch ( GetDgnMode() )
    {
    case enDgnMode::AsDesign:
    case enDgnMode::RebarDesign:
    case enDgnMode::SectionDesign:
        return true;
    default: return false;
    }
}