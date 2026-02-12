#ifndef __DB_DOELU_DB_H__
#define __DB_DOELU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DOELU
{
public:
    CDB_DOELU()
    {
        m_doel.InitHashTable(HASHSIZEDOEL);
    }
    virtual ~CDB_DOELU(){};

public:
    void Add(T_UDRD_KEY Key,T_DOEL_UDRD_D& rData)
        {m_doel.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
        {return m_doel.RemoveKey(Key);}
    void DelAll()
        {m_doel.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_DOEL_UDRD_D& rData)
        {return m_doel.Lookup(Key,rData);}
    int GetCount()
        {return m_doel.GetCount();}
    POSITION GetStart()
        {return m_doel.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DOEL_UDRD_D& rData)
        {m_doel.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
     CMap<T_UDRD_KEY,T_UDRD_KEY,T_DOEL_UDRD_D,T_DOEL_UDRD_D&>m_doel;
};

#endif
