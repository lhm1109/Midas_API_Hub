#pragma once

//! RAPIDTESTER kh1012 21.11.24
/*  so simple tester
 */

#include "RapidTesterPreDecl.h"

#include "HeaderPre.h"

struct __MY_EXT_CLASS__ TRapidTetster
{
private:
    using document_type = CDBDoc;

public:
    TRapidTetster();
    ~TRapidTetster();

    bool operator() () const;

private:
    bool existDoc() const;
    document_type* getDoc() const;
};

#include "HeaderPost.h"