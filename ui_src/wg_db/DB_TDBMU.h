#pragma once

#ifndef __DB_TDBMU_DB_H__
#define __DB_TDBMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDBMU
{
public:
    CDB_TDBMU()
    {
        m_tdbm.InitHashTable(HASHSIZETDBM);
    }
    virtual ~CDB_TDBMU(){};

public:
    void Add(T_UDRD_KEY Key, T_TDBM_UDRD_D& rData)
        {m_tdbm.SetAt(Key, rData);}
    BOOL Del(T_UDRD_KEY Key)
        {return m_tdbm.RemoveKey(Key);}
    void DelAll()
        {m_tdbm.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key, T_TDBM_UDRD_D& rData)
        {return m_tdbm.Lookup(Key, rData);}
    int GetCount()
        {return m_tdbm.GetCount();}
    POSITION GetStart()
        {return m_tdbm.GetStartPosition();}
    void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_TDBM_UDRD_D& rData)
        {m_tdbm.GetNextAssoc(rNextPosition, rKey, rData);}

protected:
    CMap<T_UDRD_KEY, T_UDRD_KEY, T_TDBM_UDRD_D, T_TDBM_UDRD_D&> m_tdbm;
};

#endif
