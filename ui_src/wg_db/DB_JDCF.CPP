#include "stdafx.h"
#include "DB_JDCF.h"

CDB_JDCF::CDB_JDCF()
{
    m_JDCF.InitHashTable(HASHSIZEJDCF);
}

CDB_JDCF::~CDB_JDCF()
{
    
}

void CDB_JDCF::Add(T_JDCF_D& rData)
{
    m_JDCF.SetAt(1,rData);
}

BOOL CDB_JDCF::Del()
{
    T_JDCF_D Data;
    BOOL bExist = m_JDCF.Lookup(1, Data);
    if(bExist)
    {
        BOOL ret = m_JDCF.RemoveKey(1);
        ASSERT(ret);
        return ret;
    }
    return bExist;
}
