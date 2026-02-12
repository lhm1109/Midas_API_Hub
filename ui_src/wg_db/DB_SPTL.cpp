#include "stdafx.h"
#include "DB_SPTL.h"

CDB_SPTL::CDB_SPTL()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_sptl.InitHashTable(HASHSIZESPTL);
}

CDB_SPTL::~CDB_SPTL()
{
}

void CDB_SPTL::Add(T_SPTL_K Key, T_SPTL_D& rData)
{
	T_SPTL_D Data;
	BOOL bExist=m_sptl.Lookup(Key, Data);
	m_sptl.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_SPTL_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SPTL::Del(T_SPTL_K Key)
{
	T_SPTL_D Data;
	BOOL bExist=m_sptl.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_sptl.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SPTL_K key;
			T_SPTL_D data;
			if(m_sptl.Lookup(m_nLastNum-1,data))
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
