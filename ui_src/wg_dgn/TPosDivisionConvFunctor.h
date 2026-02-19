#pragma once

#include "HeaderPre.h"

namespace idgn
{
    typedef	DWORD_PTR FIterator;
}

namespace dgn
{
    namespace def
    {
        enum class enSector;
    }
}

namespace dgn
{
    namespace converter
    {
        struct __MY_EXT_CLASS__ TPosDivisionConvFunctor final
        {
            typedef idgn::FIterator FIterator;
            typedef def::enSector enSector;

            TPosDivisionConvFunctor();

            // 2 Division ( I, J )
            static CString Str2Div(UINT Index);
            static CString Str2Div(FIterator ItrRefer);

            static UINT Num2Div(const CString& strPos);
            static UINT Num2Div(FIterator ItrRefer);

            // 3 Divisions ( I M J )
            static CString Str3Div(UINT Index);
            static CString Str3Div(enSector Sector);

            static UINT Num3Div(const CString& strPos);
            static UINT Num3Div(enSector Sector);

            // 5 Divisions ( I 1/4 1/2 4/3 J )
            static CString Str5Div(UINT Index);
            static CString Str5Div(FIterator ItrRefer);
            
            static UINT Num5Div(const CString& strPos);
            static UINT Num5Div(FIterator ItrRefer);
        };
    }
}

#include "HeaderPost.h"
