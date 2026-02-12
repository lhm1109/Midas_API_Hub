#ifndef __DB_DIMMU_DB_H__
#define __DB_DIMMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DIMMU
{
public:
    CDB_DIMMU()
    {
        m_dimm.InitHashTable(HASHSIZEDIMM);
    }
    virtual ~CDB_DIMMU(){};

public:
    void Add(T_UDRD_KEY Key,T_DIMM_UDRD_D& rData)
        {m_dimm.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
        {return m_dimm.RemoveKey(Key);}
    void DelAll()
        {m_dimm.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_DIMM_UDRD_D& rData)
        {return m_dimm.Lookup(Key,rData);}
    int GetCount()
        {return m_dimm.GetCount();}
    POSITION GetStart()
        {return m_dimm.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DIMM_UDRD_D& rData)
        {m_dimm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
     CMap<T_UDRD_KEY,T_UDRD_KEY,T_DIMM_UDRD_D,T_DIMM_UDRD_D&>m_dimm;
};

#endif