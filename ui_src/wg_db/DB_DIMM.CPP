#include "stdafx.h"
#include "DB_DIMM.h"

CDB_DIMM::CDB_DIMM()
{
    m_nStartNum=1;
    m_nLastNum=0;
    m_dimm.InitHashTable(HASHSIZEDIMM);
}

CDB_DIMM::~CDB_DIMM()
{
}

void CDB_DIMM::Add(T_DIMM_K Key,T_DIMM_D& rData)
{
    m_dimm.SetAt(Key,rData);
    if(Key == m_nStartNum)
    {
        T_DIMM_D data;
        while(TRUE)
        {
            m_nStartNum++;
            if(!Get(m_nStartNum,data))break;
        }
    }
    if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_DIMM::Del(T_DIMM_K Key)
{
    BOOL ret=m_dimm.RemoveKey(Key);
    ASSERT(ret);
    if(ret)
    {
        if(Key < m_nStartNum)m_nStartNum=Key;
        if(Key==m_nLastNum)
        {
            T_DIMM_K key;
            T_DIMM_D data;
            if(Get(m_nLastNum-1,data))
            {
                m_nLastNum--;
            }
            else
            {
                m_nLastNum=0;
                POSITION pos=GetStart();
                while(pos != NULL)
                {
                    GetNext(pos,key,data);
                    if(key > m_nLastNum)m_nLastNum=key;
                }
            }
        }
    }
    return ret;
}

