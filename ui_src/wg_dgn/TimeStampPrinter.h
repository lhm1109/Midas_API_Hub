#pragma once

//Include 하기 귀찮을테니 여기에서 한방에 합시다.
#include "TimeChecker.h"

#include "HeaderPre.h"

namespace dgn
{
    namespace converter
    {
        class __MY_EXT_CLASS__ CTimeStampPrinter final
        {
            typedef const CString& crStr;

        public:
            struct TColIdx final {
                enum enPriority
                {
                    Key,
                    Time1,
                    Time2,
                    Time3,
                    Time4
                };

                enPriority Priority;
                CString str;

                bool operator < (const TColIdx& src) const {
                    const auto _Left = std::make_pair(Priority, str);
                    const auto _Right = std::make_pair(src.Priority, src.str);

                    return (_Left.first < _Right.first ||
                        (!(_Right.first < _Left.first) && _Left.second < _Right.second));
                }
            };

            struct TColData final {
                std::vector<CString> vStr;
            };

        public:
            static void Init(crStr strFileName);
            
            static void AddKey(crStr strTitle, T_KEY nKey);
            static void AddTime1(crStr strTitle, const CTimeChecker& Time);
            static void AddTime2(crStr strTitle, const CTimeChecker& Time);
            static void AddTime3(crStr strTitle, const CTimeChecker& Time);
            static void AddTime4(crStr strTitle, const CTimeChecker& Time);

            static void PrintAll();

        private:
            static void AddTime(crStr strTitle, const CTimeChecker& Time, TColIdx::enPriority P);

            static CString ConvSameSpaceStr(const CString& str);
            static CString ConvSameSpaceStr(T_KEY Key);
            static CString ConvSameSpaceStr(const CTimeChecker& Time);

        private:
            static CString m_strFileName;
            static std::map<TColIdx, TColData> m_mColIdxData;

        private:
            CTimeStampPrinter();
            ~CTimeStampPrinter();
        };
    }
}

#include "HeaderPost.h"
