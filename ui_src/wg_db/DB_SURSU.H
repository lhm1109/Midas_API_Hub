#ifndef __DB_SURSU_DB_H__
#define __DB_SURSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SURSU
{
public:
    CDB_SURSU()
    {
        m_surs.InitHashTable(HASHSIZESURS);
    }
    virtual ~CDB_SURSU(){};
    
public:
    void Add(T_UDRD_KEY Key,T_SURS_UDRD_D& rData)
    {m_surs.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
    {return m_surs.RemoveKey(Key);}
    void DelAll()
    {m_surs.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_SURS_UDRD_D& rData)
    {return m_surs.Lookup(Key,rData);}
    
protected:
    CMap<T_UDRD_KEY,T_UDRD_KEY,T_SURS_UDRD_D,T_SURS_UDRD_D&>m_surs;
};

#endif