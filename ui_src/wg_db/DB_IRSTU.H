#ifndef __DB_IRSTU_DB_H__
#define __DB_IRSTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_IRSTU
{
public:
    CDB_IRSTU()
    {
        m_IRST.InitHashTable(HASHSIZEIRST);
    }
    virtual ~CDB_IRSTU(){};
    
public:
    void Add(T_UDRD_KEY Key,T_IRST_UDRD_D& rData)
    {m_IRST.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
    {return m_IRST.RemoveKey(Key);}
    void DelAll()
    {m_IRST.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_IRST_UDRD_D& rData)
    {return m_IRST.Lookup(Key,rData);}
    int GetCount()
    {return m_IRST.GetCount();}
    POSITION GetStart()
    {return m_IRST.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_IRST_UDRD_D& rData)
    {m_IRST.GetNextAssoc(rNextPosition,rKey,rData);}
    
protected:
    CMap<T_UDRD_KEY,T_UDRD_KEY,T_IRST_UDRD_D,T_IRST_UDRD_D&>m_IRST;
};

#endif
