#include "stdafx.h"
#include "DB_SDHY.h"
#include "DB_ELEM.h"

CDB_SDHY::CDB_SDHY()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_sdhy.InitHashTable(HASHSIZESDHY);
}

CDB_SDHY::~CDB_SDHY()
{
}

void CDB_SDHY::Add(T_SDHY_K Key,T_SDHY_D& rData)
{
	m_sdhy.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_SDHY_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SDHY::Del(T_SDHY_K Key)
{
	T_SDHY_D Data;
	BOOL bExist=m_sdhy.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_sdhy.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SDHY_K key;
			T_SDHY_D data;
			if(m_sdhy.Lookup(m_nLastNum-1,data))
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