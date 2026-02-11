#include "stdafx.h"
#include "DB_SCLM.h"

CDB_SCLM::CDB_SCLM()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_sclm.InitHashTable(HASHSIZESCLM);
}

CDB_SCLM::~CDB_SCLM()
{
}

void CDB_SCLM::Add(T_SCLM_K Key,T_SCLM_D& rData)
{
	m_sclm.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_SCLM_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SCLM::Del(T_SCLM_K Key)
{
	BOOL ret=m_sclm.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SCLM_K key;
			T_SCLM_D data;
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

