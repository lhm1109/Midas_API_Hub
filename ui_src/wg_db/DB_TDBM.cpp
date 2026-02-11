#include "stdafx.h"
#include "DB_TDBM.h"

CDB_TDBM::CDB_TDBM()
{
    m_nStartNum = 1;
    m_nLastNum = 0;
    m_tdbm.InitHashTable(HASHSIZETDBM);
}

CDB_TDBM::~CDB_TDBM()
{
}

void CDB_TDBM::Add(T_TDBM_K Key, T_TDBM_D& rData)
{
    T_TDBM_D Data;
    BOOL bExist = m_tdbm.Lookup(Key, Data);
    m_tdbm.SetAt(Key, rData);

    if (Key == m_nStartNum)
    {
        T_TDBM_D data;
        while (TRUE)
        {
            m_nStartNum++;
            if (!Get(m_nStartNum, data)) break;
        }
    }
    if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_TDBM::Del(T_TDBM_K Key)
{
    T_TDBM_D Data;
    BOOL bExist = m_tdbm.Lookup(Key, Data);
    if (bExist)
    {
        BOOL ret = m_tdbm.RemoveKey(Key);
        ASSERT(ret);
        if (Key < m_nStartNum) m_nStartNum = Key;
        if (Key == m_nLastNum)
        {
            T_TDBM_K key;
            T_TDBM_D data;
            if (m_tdbm.Lookup(m_nLastNum - 1, data))
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
