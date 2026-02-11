#pragma once

#ifndef __DB_TDSL_DB_H__
#define __DB_TDSL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDSL
{
public:
    CDB_TDSL();
    virtual ~CDB_TDSL();

public:
    T_TDSL_K m_nStartNum;
    T_TDSL_K m_nLastNum;

public:
    void Add(T_TDSL_K Key, T_TDSL_D& rData);
    BOOL Del(T_TDSL_K Key);

public:
    BOOL Get(T_TDSL_K Key, T_TDSL_D& rData)
        {return m_tdsl.Lookup(Key, rData);}
    int GetCount()
        {return m_tdsl.GetCount();}
    POSITION GetStart()
        {return m_tdsl.GetStartPosition();}
    void GetNext(POSITION& rNextPosition, T_TDSL_K& rKey, T_TDSL_D& rData)
        {m_tdsl.GetNextAssoc(rNextPosition, rKey, rData);}

protected:
    CMap<T_TDSL_K, T_TDSL_K, T_TDSL_D, T_TDSL_D&> m_tdsl;
};

#endif
