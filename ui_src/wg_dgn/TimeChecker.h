#pragma once

#include <chrono>

#include "HeaderPre.h"

namespace dgn
{
    namespace converter
    {
        enum class enTimeCheckerOpt
        {
            None,
            Use_and_Reset,
        };

        class __MY_EXT_CLASS__ CTimeChecker final
        {
            typedef std::chrono::system_clock::time_point TimePoint;

        public:
            CTimeChecker(enTimeCheckerOpt Opt = enTimeCheckerOpt::None);
            ~CTimeChecker();

        public:
            double GetCurDuration_seconds() const;
            long long GetCurDuration_milliseconds() const;

        private:
            void ResetByOpt() const;

        private:
            enTimeCheckerOpt m_Opt;
            TimePoint m_StartPoint;
        };
    }
}

#include "HeaderPost.h"
