#include "StdAfx.h"

#include "../dgnengine/idesign/DGN_lib/TemporaryObject.h"

#include "TEngineResult.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;
using namespace dgn::converter;

TEngineResult::TEngineResult()
{
    pEResultAll = nullptr;
    pELesultAll = nullptr;
}

const CDgnResultBundle* TEngineResult::GetResultBundle() const
{
    if ( !pEResultAll )
    {
        ASSERT(0); return nullptr;
    }

    return pEResultAll;
}

const CDgnLesultBundle* TEngineResult::GetLesultBundle() const
{
    if ( !pELesultAll )
    {
        ASSERT(0); return nullptr;
    }

    return pELesultAll;
}