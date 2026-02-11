#ifndef __DB_MADOU_DB_H__
#define __DB_MADOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MADOU
{
public:
    CDB_MADOU()
    {
        m_mado.InitHashTable(HASHSIZEMADO);
    }
    virtual ~CDB_MADOU(){};

public:
    void Add(T_UDRD_KEY Key,T_MADO_UDRD_D& rData)
        {m_mado.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
        {return m_mado.RemoveKey(Key);}
    void DelAll()
        {m_mado.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key,T_MADO_UDRD_D& rData)
        {return m_mado.Lookup(Key,rData);}
    int GetCount()
        {return m_mado.GetCount();}
    POSITION GetStart()
        {return m_mado.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MADO_UDRD_D& rData)
        {m_mado.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
     CMap<T_UDRD_KEY,T_UDRD_KEY,T_MADO_UDRD_D,T_MADO_UDRD_D&>m_mado;
};

#endif