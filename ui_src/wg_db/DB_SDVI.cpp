#include "stdafx.h"
#include "DB_SDVI.h"
#include "DB_ELEM.h"

CDB_SDVI::CDB_SDVI()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_sdvi.InitHashTable(HASHSIZESDVI);
}

CDB_SDVI::~CDB_SDVI()
{
}

void CDB_SDVI::Add(T_SDVI_K Key,T_SDVI_D& rData)
{
	m_sdvi.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_SDVI_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SDVI::Del(T_SDVI_K Key)
{
	T_SDVI_D Data;
	BOOL bExist=m_sdvi.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_sdvi.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SDVI_K key;
			T_SDVI_D data;
			if(m_sdvi.Lookup(m_nLastNum-1,data))
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