#include "stdafx.h"
#include "DB_PBDC.h"

CDB_PBDC::CDB_PBDC()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_pbdc.InitHashTable(HASHSIZEPBDC);
}

CDB_PBDC::~CDB_PBDC()
{
}

void CDB_PBDC::Add(T_PBDC_K Key,T_PBDC_D& rData)
{
	T_PBDC_D Data;
	BOOL bExist=m_pbdc.Lookup(Key, Data);
	m_pbdc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_PBDC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PBDC::Del(T_PBDC_K Key)
{
	T_PBDC_D Data;
	BOOL bExist=m_pbdc.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_pbdc.RemoveKey(Key);
		ASSERT(ret);
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PBDC_K key;
			T_PBDC_D data;
			if(m_pbdc.Lookup(m_nLastNum-1,data))
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