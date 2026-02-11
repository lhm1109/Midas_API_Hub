#pragma once

#ifndef __DB_TDSLU_DB_H__
#define __DB_TDSLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDSLU
{
public:
    CDB_TDSLU()
    {
        m_tdsl.InitHashTable(HASHSIZETDSL);
    }
    virtual ~CDB_TDSLU(){};

public:
    void Add(T_UDRD_KEY Key, T_TDSL_UDRD_D& rData)
        {m_tdsl.SetAt(Key, rData);}
    BOOL Del(T_UDRD_KEY Key)
        {return m_tdsl.RemoveKey(Key);}
    void DelAll()
        {m_tdsl.RemoveAll();}
    BOOL Get(T_UDRD_KEY Key, T_TDSL_UDRD_D& rData)
        {return m_tdsl.Lookup(Key, rData);}
    int GetCount()
        {return m_tdsl.GetCount();}
    POSITION GetStart()
        {return m_tdsl.GetStartPosition();}
    void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_TDSL_UDRD_D& rData)
        {m_tdsl.GetNextAssoc(rNextPosition, rKey, rData);}

protected:
    CMap<T_UDRD_KEY, T_UDRD_KEY, T_TDSL_UDRD_D, T_TDSL_UDRD_D&> m_tdsl;
};

#endif
