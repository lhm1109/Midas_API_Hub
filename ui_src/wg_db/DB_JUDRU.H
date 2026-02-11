#ifndef __DB_JUDRU_DB_H__
#define __DB_JUDRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JUDRU
{
public:
    CDB_JUDRU()
    {
        m_JUDR.InitHashTable(HASESIZEJUDR);
    }
    virtual ~CDB_JUDRU(){};
    
public:
    void Add(T_UDRD_KEY Key,T_JUDR_UDRD_D& rData)
    {m_JUDR.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
    {return m_JUDR.RemoveKey(Key);}
    void DelAll()
    {m_JUDR.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_JUDR_UDRD_D& rData)
    {return m_JUDR.Lookup(Key,rData);}
    
protected:
    CMap<T_UDRD_KEY,T_UDRD_KEY,T_JUDR_UDRD_D,T_JUDR_UDRD_D&> m_JUDR;
};

#endif