#pragma once

#ifndef __DB_TDBM_DB_H__
#define __DB_TDBM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDBM
{
public:
    CDB_TDBM();
    virtual ~CDB_TDBM();

public:
    T_TDBM_K m_nStartNum;
    T_TDBM_K m_nLastNum;

public:
    void Add(T_TDBM_K Key, T_TDBM_D& rData);
    BOOL Del(T_TDBM_K Key);

public:
    BOOL Get(T_TDBM_K Key, T_TDBM_D& rData)
        {return m_tdbm.Lookup(Key, rData);}
    int GetCount()
        {return m_tdbm.GetCount();}
    POSITION GetStart()
        {return m_tdbm.GetStartPosition();}
    void GetNext(POSITION& rNextPosition, T_TDBM_K& rKey, T_TDBM_D& rData)
        {m_tdbm.GetNextAssoc(rNextPosition, rKey, rData);}

protected:
    CMap<T_TDBM_K, T_TDBM_K, T_TDBM_D, T_TDBM_D&> m_tdbm;
};

#endif
