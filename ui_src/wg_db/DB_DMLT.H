#ifndef __DB_DMLT_DB_H__
#define __DB_DMLT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DMLT
{
public:
    CDB_DMLT();
    virtual ~CDB_DMLT();

public:
    T_DMLT_K m_nStartNum;
    T_DMLT_K m_nLastNum;

public:
    void Add(T_DMLT_K Key,T_DMLT_D& rData);
    BOOL Del(T_DMLT_K Key);

public:
    BOOL Get(T_DMLT_K Key,T_DMLT_D& rData)
        {return m_dmlt.Lookup(Key,rData);}
    int GetCount()
        {return m_dmlt.GetCount();}
    POSITION GetStart()
        {return m_dmlt.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_DMLT_K& rKey,T_DMLT_D& rData)
        {m_dmlt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
    CMap<T_DMLT_K,T_DMLT_K,T_DMLT_D,T_DMLT_D&>m_dmlt;
};

#endif