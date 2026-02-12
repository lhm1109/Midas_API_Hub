#ifndef __DB_UCCFU_DB_H__
#define __DB_UCCFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_UCCFU
{
public:
    CDB_UCCFU()
    {
        m_uccf.InitHashTable(HASHSIZEUCCF);
    }
    virtual ~CDB_UCCFU(){};
    
public:
    void Add(T_UDRD_KEY Key,T_UCCF_UDRD_D& rData)
        {m_uccf.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
        {return m_uccf.RemoveKey(Key);}
    void DelAll()
        {m_uccf.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_UCCF_UDRD_D& rData)
        {return m_uccf.Lookup(Key,rData);}
    int GetCount()
        {return m_uccf.GetCount();}
    POSITION GetStart()
        {return m_uccf.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_UCCF_UDRD_D& rData)
        {m_uccf.GetNextAssoc(rNextPosition,rKey,rData);}
    
protected:
    CMap<T_UDRD_KEY,T_UDRD_KEY,T_UCCF_UDRD_D,T_UCCF_UDRD_D&>m_uccf;
};

#endif