#include "stdafx.h"
#include "DB_TDSL.h"

CDB_TDSL::CDB_TDSL()
{
    m_nStartNum = 1;
    m_nLastNum = 0;
    m_tdsl.InitHashTable(HASHSIZETDSL);
}

CDB_TDSL::~CDB_TDSL()
{
}

void CDB_TDSL::Add(T_TDSL_K Key, T_TDSL_D& rData)
{
    T_TDSL_D Data;
    BOOL bExist = m_tdsl.Lookup(Key, Data);
    m_tdsl.SetAt(Key, rData);

    if (Key == m_nStartNum)
    {
        T_TDSL_D data;
        while (TRUE)
        {
            m_nStartNum++;
            if (!Get(m_nStartNum, data)) break;
        }
    }
    if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_TDSL::Del(T_TDSL_K Key)
{
    T_TDSL_D Data;
    BOOL bExist = m_tdsl.Lookup(Key, Data);
    if (bExist)
    {
        BOOL ret = m_tdsl.RemoveKey(Key);
        ASSERT(ret);
        if (Key < m_nStartNum) m_nStartNum = Key;
        if (Key == m_nLastNum)
        {
            T_TDSL_K key;
            T_TDSL_D data;
            if (m_tdsl.Lookup(m_nLastNum - 1, data))
            {
                m_nLastNum--;
            }
            else
            {
                m_nLastNum = 0;
                POSITION pos = GetStart();
                while (pos != NULL)
                {
                    GetNext(pos, key, data);
                    if (key > m_nLastNum) m_nLastNum = key;
                }
            }
        }
        return ret;
    }
    return bExist;
}
