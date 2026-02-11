#ifndef __DB_DMLTU_DB_H__
#define __DB_DMLTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DMLTU
{
public:
    CDB_DMLTU()
    {
        m_dmlt.InitHashTable(HASHSIZEDMLT);
    }
    virtual ~CDB_DMLTU(){};

public:
    void Add(T_UDRD_KEY Key,T_DMLT_UDRD_D& rData)
        {m_dmlt.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
        {return m_dmlt.RemoveKey(Key);}
    void DelAll()
        {m_dmlt.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_DMLT_UDRD_D& rData)
        {return m_dmlt.Lookup(Key,rData);}
    int GetCount()
        {return m_dmlt.GetCount();}
    POSITION GetStart()
        {return m_dmlt.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DMLT_UDRD_D& rData)
        {m_dmlt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
     CMap<T_UDRD_KEY,T_UDRD_KEY,T_DMLT_UDRD_D,T_DMLT_UDRD_D&>m_dmlt;
};

#endif