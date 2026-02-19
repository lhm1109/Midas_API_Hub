#pragma once

#include "HeaderPre.h"
class CDBDoc;
class __MY_EXT_CLASS__ CDgnMemoizationTower
{
private:
    CDgnMemoizationTower();
    ~CDgnMemoizationTower();

public:
    static CDgnMemoizationTower* GetInstance();

public:
    bool InitializeCheck();
    bool InitializeDesign();
    bool End();
    bool Finalize();

private:
    static CDgnMemoizationTower* m_pTower;

private:
    CDBDoc* m_pDoc;
};
#include "HeaderPost.h"
