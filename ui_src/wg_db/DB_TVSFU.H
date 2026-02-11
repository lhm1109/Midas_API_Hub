#ifndef __DB_TVSFU_DB_H__
#define __DB_TVSFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TVSFU
{
public:
    CDB_TVSFU()
    {
        m_tvsf.InitHashTable(HASHSIZETVSF);
    }
    virtual ~CDB_TVSFU(){};

public:
    void Add(T_UDRD_KEY Key,T_TVSF_UDRD_D& rData)
        {m_tvsf.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
        {return m_tvsf.RemoveKey(Key);}
    void DelAll()
        {m_tvsf.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_TVSF_UDRD_D& rData)
        {return m_tvsf.Lookup(Key,rData);}
    int GetCount()
        {return m_tvsf.GetCount();}
    POSITION GetStart()
        {return m_tvsf.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TVSF_UDRD_D& rData)
        {m_tvsf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
     CMap<T_UDRD_KEY,T_UDRD_KEY,T_TVSF_UDRD_D,T_TVSF_UDRD_D&>m_tvsf;
};

#endif