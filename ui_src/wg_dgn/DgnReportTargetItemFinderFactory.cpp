#include "StdAfx.h"

#include "../dgnengine/idesign/DGN_def/DgnTypeEnumDef.h"

#include "DgnReportTargetItemFinderFactory.h"
#include "TProductTypeStruct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;

std::map<dgn::converter::enResultDatabaseType, CDgnResultExistItemFinder*> CDgnReportTargetItemFinderFactory::m_mpFinder;

CDgnReportTargetItemFinderFactory::CDgnReportTargetItemFinderFactory()
{
}

CDgnReportTargetItemFinderFactory::~CDgnReportTargetItemFinderFactory()
{
}

bool CDgnReportTargetItemFinderFactory::AddFinder(dgn::converter::enResultDatabaseType enResultDataBase,
                                                 CDgnResultExistItemFinder* const pFinder)
{
    if ( !pFinder )
    {
        ASSERT(0); return false;
    }

    m_mpFinder[enResultDataBase] = pFinder;

    return true;
}

CDgnResultExistItemFinder* const CDgnReportTargetItemFinderFactory::GetFinder(dgn::converter::enResultDatabaseType enResultDataBase)
{
    auto itrFind = m_mpFinder.find(enResultDataBase);
    if ( itrFind == m_mpFinder.end() )
    {
        ASSERT(0); return nullptr;
    }

    auto* const pFinder = itrFind->second;
    if ( !pFinder )
    {
        ASSERT(0); return nullptr;
    }

    return pFinder;
}