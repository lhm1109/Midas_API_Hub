#include "stdafx.h"
#include "DB_JUDR.h"

CDB_JUDR::CDB_JUDR()
{
    m_JUDR.InitHashTable(HASESIZEJUDR);
}

CDB_JUDR::~CDB_JUDR()
{
    
}

void CDB_JUDR::Add(T_JUDR_D& rData)
{
    m_JUDR.SetAt(1,rData);
}

BOOL CDB_JUDR::Del()
{
    T_JUDR_D Data;
    BOOL bExist = m_JUDR.Lookup(1, Data);
    if(bExist)
    {
        BOOL ret = m_JUDR.RemoveKey(1);
        ASSERT(ret);
        return ret;
    }
    return bExist;
}
