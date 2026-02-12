#ifndef __DB_JRNKU_DB_H__
#define __DB_JRNKU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JRNKU
{
public:
    CDB_JRNKU()
    {
        m_JRNK.InitHashTable(HASHSIZEJRNK);
    }
    virtual ~CDB_JRNKU(){};
    
public:
    void Add(T_UDRD_KEY Key,T_JRNK_UDRD_D& rData)
    {m_JRNK.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
    {return m_JRNK.RemoveKey(Key);}
    void DelAll()
    {m_JRNK.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_JRNK_UDRD_D& rData)
    {return m_JRNK.Lookup(Key,rData);}
    int GetCount()
    {return m_JRNK.GetCount();}
    POSITION GetStart()
    {return m_JRNK.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_JRNK_UDRD_D& rData)
    {m_JRNK.GetNextAssoc(rNextPosition,rKey,rData);}
    
protected:
    CMap<T_UDRD_KEY,T_UDRD_KEY,T_JRNK_UDRD_D,T_JRNK_UDRD_D&>m_JRNK;
};

#endif
