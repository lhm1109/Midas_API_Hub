#include "StdAfx.h"

#include "TimeChecker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace std::chrono;
using namespace dgn::converter;

CTimeChecker::CTimeChecker(enTimeCheckerOpt Opt) :
    m_Opt(Opt),
    m_StartPoint(std::chrono::system_clock::now())
{
}

CTimeChecker::~CTimeChecker()
{
}

double CTimeChecker::GetCurDuration_seconds() const
{
    const auto& TimePoint = system_clock::now();

    duration<double> Cur = 
        TimePoint - m_StartPoint;

    ResetByOpt();

    return Cur.count();
}

long long CTimeChecker::GetCurDuration_milliseconds() const
{
    const auto& TimePoint = system_clock::now();

    milliseconds Cur = 
        duration_cast<milliseconds>(TimePoint - m_StartPoint);

    ResetByOpt();

    return Cur.count();
}

void CTimeChecker::ResetByOpt() const
{
    switch ( m_Opt )
    {
    case enTimeCheckerOpt::None:
        {
            return;
        }
    case enTimeCheckerOpt::Use_and_Reset:
        {
            const_cast<TimePoint&>(m_StartPoint) = 
                std::chrono::system_clock::now();

            return;
        }
    default: ASSERT(0); return;
    }
}