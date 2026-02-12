#ifndef __DB_DIMM_DB_H__
#define __DB_DIMM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DIMM
{
public:
    CDB_DIMM();
    virtual ~CDB_DIMM();

public:
    T_DIMM_K m_nStartNum;
    T_DIMM_K m_nLastNum;

public:
    void Add(T_DIMM_K Key,T_DIMM_D& rData);
    BOOL Del(T_DIMM_K Key);

public:
    BOOL Get(T_DIMM_K Key,T_DIMM_D& rData)
        {return m_dimm.Lookup(Key,rData);}
    int GetCount()
        {return m_dimm.GetCount();}
    POSITION GetStart()
        {return m_dimm.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_DIMM_K& rKey,T_DIMM_D& rData)
        {m_dimm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
    CMap<T_DIMM_K,T_DIMM_K,T_DIMM_D,T_DIMM_D&>m_dimm;
};

#endif