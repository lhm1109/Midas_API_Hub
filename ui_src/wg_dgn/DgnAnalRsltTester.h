#pragma once

#include "HeaderPre.h"

class CDBDoc;

class __MY_EXT_CLASS__ CDgnAnalRsltTester final
{
public:
    CDgnAnalRsltTester();
    virtual ~CDgnAnalRsltTester();

public:
    bool Test();

private:
    CDBDoc* GetDBDoc() const;
    CDBDoc* m_pDBDoc;
};

#include "HeaderPost.h"
