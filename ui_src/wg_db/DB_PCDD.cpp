#include "stdafx.h"
#include "DB_PCDD.h"

CDB_PCDD::CDB_PCDD()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_pcdd.InitHashTable(HASHSIZEPCDD);
}

CDB_PCDD::~CDB_PCDD()
{
}

void CDB_PCDD::Add(T_PCDD_K Key,T_PCDD_D& rData)
{
	T_PCDD_D Data;
	BOOL bExist=m_pcdd.Lookup(Key, Data);
	m_pcdd.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_PCDD_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PCDD::Del(T_PCDD_K Key)
{
	T_PCDD_D Data;
	BOOL bExist=m_pcdd.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_pcdd.RemoveKey(Key);
		ASSERT(ret);
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PCDD_K key;
			T_PCDD_D data;
			if(m_pcdd.Lookup(m_nLastNum-1,data))
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
		return ret;
	}
	return bExist;
}