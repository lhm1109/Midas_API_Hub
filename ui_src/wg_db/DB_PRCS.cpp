#include "stdafx.h"
#include "DB_PRCS.h"

CDB_PRCS::CDB_PRCS()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_prcs.InitHashTable(HASHSIZEPRCS);
}

CDB_PRCS::~CDB_PRCS()
{
}

void CDB_PRCS::Add(T_PRCS_K Key,T_PRCS_D& rData)
{
	T_PRCS_D Data;
	BOOL bExist=m_prcs.Lookup(Key, Data);
	m_prcs.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_PRCS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PRCS::Del(T_PRCS_K Key)
{
	T_PRCS_D Data;
	BOOL bExist=m_prcs.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_prcs.RemoveKey(Key);
		ASSERT(ret);

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PRCS_K key;
			T_PRCS_D data;
			if(m_prcs.Lookup(m_nLastNum-1,data))
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