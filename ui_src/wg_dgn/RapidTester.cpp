#include "StdAfx.h"

#include "..\wg_db\DBDoc.h"

#include "RapidTester.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
    struct DoInAdvance final
    {
        DoInAdvance()
        {
            TRapidTetster()();
        }
    };

    //DoInAdvance Instance;
#endif

TRapidTetster::TRapidTetster()
{
}

TRapidTetster::~TRapidTetster()
{
}

bool TRapidTetster::operator()() const
{
    if ( existDoc() )
    {
        //document enable block

        return true;
    }
    else
    {
        //document not enable block

        return true;
    }
}

bool TRapidTetster::existDoc() const
{
    return getDoc() != nullptr;
}

TRapidTetster::document_type* TRapidTetster::getDoc() const
{
    static auto pDoc = CDBDoc::GetDocPoint();
    if ( !pDoc )
    {
        return nullptr;
    }

    return pDoc;
}