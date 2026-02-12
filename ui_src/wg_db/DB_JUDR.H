#ifndef __DB_JUDR_DB_H__
#define __DB_JUDR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JUDR
{
public:
    CDB_JUDR();
    virtual ~CDB_JUDR();
    
public:
    void Add(T_JUDR_D& rData);
    BOOL Del();
    
public:
    BOOL Get(T_JUDR_D& rData)
    {return m_JUDR.Lookup(1,rData);}
    int GetCount()
    {return m_JUDR.GetCount();}
    
protected:
    CMap<T_JUDR_K,T_JUDR_K,T_JUDR_D,T_JUDR_D&> m_JUDR;
};

#endif