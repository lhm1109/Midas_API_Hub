#include "stdafx.h"

#include "../dgnengine/idesign/DGN_def/DgnTypeEnumDef.h"

#include "IDgnPerformDataBaseSTL.h"

using namespace dgn::def;

IDgnPerformDataBaseSTL::IDgnPerformDataBaseSTL()
{
}

IDgnPerformDataBaseSTL::~IDgnPerformDataBaseSTL()
{
}

dgn::def::enDgn IDgnPerformDataBaseSTL::GetDgnType()
{
    return enDgn::STL;
}

bool IDgnPerformDataBaseSTL::IsDesigning()
{
    return false;
}
