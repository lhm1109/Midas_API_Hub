#ifndef __DB_PSCNU_DB_H__
#define __DB_PSCNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PSCNU
{
public:
    CDB_PSCNU()
    {
        m_pscn.InitHashTable(HASHSIZEPESC);
    }
    virtual ~CDB_PSCNU(){};
    
public:
    void Add(T_UDRD_KEY Key,T_PSCN_UDRD_D& rData)
    {m_pscn.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
    {return m_pscn.RemoveKey(Key);}
    void DelAll()
    {m_pscn.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_PSCN_UDRD_D& rData)
    {return m_pscn.Lookup(Key,rData);}
    int GetCount()
    {return m_pscn.GetCount();}
    POSITION GetStart()
    {return m_pscn.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PSCN_UDRD_D& rData)
    {m_pscn.GetNextAssoc(rNextPosition,rKey,rData);}
    
protected:
    CMap<T_UDRD_KEY,T_UDRD_KEY,T_PSCN_UDRD_D,T_PSCN_UDRD_D&>m_pscn;
};

#endif