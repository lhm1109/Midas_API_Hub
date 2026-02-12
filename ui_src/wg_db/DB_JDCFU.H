#ifndef __DB_JDCFU_DB_H__
#define __DB_JDCFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDCFU
{
public:
    CDB_JDCFU()
    {
        m_JDCF.InitHashTable(HASHSIZEJDCF);
    }
    virtual ~CDB_JDCFU(){};
    
public:
    void Add(T_UDRD_KEY Key,T_JDCF_UDRD_D& rData)
    {m_JDCF.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
    {return m_JDCF.RemoveKey(Key);}
    void DelAll()
    {m_JDCF.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_JDCF_UDRD_D& rData)
    {return m_JDCF.Lookup(Key,rData);}
    
protected:
    CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDCF_UDRD_D,T_JDCF_UDRD_D&> m_JDCF;
};

#endif