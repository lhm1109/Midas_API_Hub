#pragma once

#include "HeaderPre.h"

struct T_RPCM_D;

class __MY_EXT_CLASS__ CDgnRebarTool final
{
public:
    CDgnRebarTool();
    virtual ~CDgnRebarTool();

public:
    void CalcPlateAs(T_RPCM_D& RpcmD, double& dAsTop, double& dAsBot);

private:
    CDBDoc* m_pDoc;
};

#include "HeaderPost.h"
