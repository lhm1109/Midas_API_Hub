#include "stdafx.h"
#include "DB_PBDD.h"

CDB_PBDD::CDB_PBDD()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_pbdd.InitHashTable(HASHSIZEPBDD);
}

CDB_PBDD::~CDB_PBDD()
{
}

void CDB_PBDD::Add(T_PBDD_K Key,T_PBDD_D& rData)
{
	T_PBDD_D Data;
	BOOL bExist=m_pbdd.Lookup(Key, Data);
	m_pbdd.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_PBDD_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PBDD::Del(T_PBDD_K Key)
{
	T_PBDD_D Data;
	BOOL bExist=m_pbdd.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_pbdd.RemoveKey(Key);
		ASSERT(ret);
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PBDD_K key;
			T_PBDD_D data;
			if(m_pbdd.Lookup(m_nLastNum-1,data))
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