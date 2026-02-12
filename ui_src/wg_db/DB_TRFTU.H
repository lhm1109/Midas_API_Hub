#ifndef __DB_TRFTU_DB_H__
#define __DB_TRFTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TRFTU
{
public:
    CDB_TRFTU()
    {
        m_trft.InitHashTable(HASHSIZETRFT);
    }
    virtual ~CDB_TRFTU(){};

public:
    void Add(T_UDRD_KEY Key,T_TRFT_UDRD_D& rData)
        {m_trft.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
        {return m_trft.RemoveKey(Key);}
    void DelAll()
        {m_trft.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_TRFT_UDRD_D& rData)
        {return m_trft.Lookup(Key,rData);}
    int GetCount()
        {return m_trft.GetCount();}
    POSITION GetStart()
        {return m_trft.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TRFT_UDRD_D& rData)
        {m_trft.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
     CMap<T_UDRD_KEY,T_UDRD_KEY,T_TRFT_UDRD_D,T_TRFT_UDRD_D&>m_trft;
};

#endif