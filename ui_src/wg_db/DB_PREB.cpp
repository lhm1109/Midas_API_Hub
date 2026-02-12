#include "stdafx.h"
#include "DB_PREB.h"

CDB_PREB::CDB_PREB()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_preb.InitHashTable(HASHSIZEPREB);
}

CDB_PREB::~CDB_PREB()
{
}

void CDB_PREB::Add(T_PREB_K Key,T_PREB_D& rData)
{
	T_PREB_D Data;
	BOOL bExist=m_preb.Lookup(Key, Data);
	m_preb.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_PREB_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PREB::Del(T_PREB_K Key)
{
	T_PREB_D Data;
	BOOL bExist=m_preb.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_preb.RemoveKey(Key);
		ASSERT(ret);

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PREB_K key;
			T_PREB_D data;
			if(m_preb.Lookup(m_nLastNum-1,data))
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