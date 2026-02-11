#include "stdafx.h"
#include "DB_DMLT.h"

CDB_DMLT::CDB_DMLT()
{
    m_nStartNum=1;
    m_nLastNum=0;
    m_dmlt.InitHashTable(HASHSIZEDMLT);
}

CDB_DMLT::~CDB_DMLT()
{
}

void CDB_DMLT::Add(T_DMLT_K Key,T_DMLT_D& rData)
{
    m_dmlt.SetAt(Key,rData);
    if(Key == m_nStartNum)
    {
        T_DMLT_D data;
        while(TRUE)
        {
            m_nStartNum++;
            if(!Get(m_nStartNum,data))break;
        }
    }
    if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_DMLT::Del(T_DMLT_K Key)
{
    BOOL ret=m_dmlt.RemoveKey(Key);
    ASSERT(ret);
    if(ret)
    {
        if(Key < m_nStartNum)m_nStartNum=Key;
        if(Key==m_nLastNum)
        {
            T_DMLT_K key;
            T_DMLT_D data;
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

