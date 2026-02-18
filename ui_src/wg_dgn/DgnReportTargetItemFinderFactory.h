#pragma once

#include <map>

#include "HeaderPre.h"

#pragma region
/// 전방선언
namespace dgn
{
    namespace def
    {
        enum class enDgn;
    }

    namespace converter
    {
        enum enResultDatabaseType;
    }
}

class CDgnResultExistItemFinder;
#pragma endregion

class __MY_EXT_CLASS__ CDgnReportTargetItemFinderFactory final
{
public:
    CDgnReportTargetItemFinderFactory();
    ~CDgnReportTargetItemFinderFactory();

public:
    static bool AddFinder(dgn::converter::enResultDatabaseType enResultDataBase, CDgnResultExistItemFinder* const pFinder);
    static CDgnResultExistItemFinder* const GetFinder(dgn::converter::enResultDatabaseType enResultDataBase);

private:
    static std::map<dgn::converter::enResultDatabaseType, CDgnResultExistItemFinder*> m_mpFinder;
};

#include "HeaderPost.h"
