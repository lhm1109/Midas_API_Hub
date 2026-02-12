#ifndef __DB_MSRB_DB_H__
#define __DB_MSRB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MSRB
{
public:
    CDB_MSRB();
    virtual ~CDB_MSRB();
    CDBDoc* m_pDoc;

public:
    void Add(T_MSRB_D& rData);
    BOOL Del();

public:
    BOOL Get(T_MSRB_D& rData);
        //{return m_msrb.Lookup(1,rData);}
    int GetCount();
        //{return m_msrb.GetCount();}

protected:
    CMap<T_MSRB_K,T_MSRB_K,T_MSRB_D,T_MSRB_D&>m_msrb;
};

#endif