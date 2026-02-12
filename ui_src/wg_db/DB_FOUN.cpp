#include "stdafx.h"
#include "DB_FOUN.h"

CDB_FOUN::CDB_FOUN()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_foun.InitHashTable(HASHSIZEFOUN);
}

CDB_FOUN::~CDB_FOUN()
{
}

void CDB_FOUN::Add(T_FOUN_K Key,T_FOUN_D& rData)
{
	m_foun.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_FOUN_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_FOUN::Del(T_FOUN_K Key)
{
	BOOL ret=m_foun.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_FOUN_K key;
			T_FOUN_D data;
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