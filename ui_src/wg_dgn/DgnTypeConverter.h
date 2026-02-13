#pragma once
#include "..\dgnengine\idesign\DGN_link\IDgnCalc.h"

#include "HeaderPre.h"

namespace dgn
{
    namespace def
    {
        enum class enLoadCase;
    }

    namespace converter
    {
        class __MY_EXT_CLASS__ CDgnTypeConverter
        {
        private:
            CDgnTypeConverter();
            virtual ~CDgnTypeConverter();

        public:
            static dgn::def::enDgnCode GetDgnCode(UINT unCode, UINT unNA = 0);
            static dgn::def::enMemb GetMembType(int nMembType);
            static dgn::def::enMemb GetMembTypeByElem(T_ELEM_K ElemK);
            static dgn::def::enLoadCase GetLoadCaseType(CString strLoad);

        public:
            static dgn::def::enDgn GetDgnType(T_ELEM_K ElemK);
            static dgn::def::enDgnCode GetDgnCodeByElem(T_ELEM_K ElemK);
        };
    }
}

#include "HeaderPost.h"
