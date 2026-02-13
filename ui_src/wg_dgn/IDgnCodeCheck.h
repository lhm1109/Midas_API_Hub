#pragma once

#include "..\dgnengine\idesign\DGN_def\DgnModeDef.h"

#include "HeaderPre.h"

namespace dgn
{
    namespace def
    {
        enum class enDgnCode : unsigned int;
        enum class enDgn;
        enum class enMemb;
        class CDgnInput;
    }
}

class IDgnDataBase;

class __MY_EXT_CLASS__ IDgnCodeCheck
{
public:
    IDgnCodeCheck(IDgnDataBase* pDataBase);
    virtual ~IDgnCodeCheck();

public:
    IDgnDataBase* GetDataBase();

    virtual bool GetDgnInput(dgn::def::CDgnInput& rInput, dgn::def::enDgnMode enMode);

protected:
    bool Check_Section(bool bReport);

private:
    IDgnDataBase* m_pDataBase;
};

#include "HeaderPost.h"
