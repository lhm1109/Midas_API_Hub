#pragma once

#include "HeaderPre.h"

namespace dgn
{
    namespace def
    {
        enum class enDgnCode : unsigned int;
    }
}

class __MY_EXT_CLASS__ CDgnCodeTool
{
public:
    CDgnCodeTool();
    virtual ~CDgnCodeTool();

public:
    static bool IsSupportCodeIDesignReport(const dgn::def::enDgnCode enDgnCode);
    static bool IsEurocode2(const dgn::def::enDgnCode enDgnCode);
    static bool IsEurocode4(const dgn::def::enDgnCode enDgnCode);
};

#include "HeaderPost.h"
