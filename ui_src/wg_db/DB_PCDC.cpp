#include "stdafx.h"
#include "DB_PCDC.h"

CDB_PCDC::CDB_PCDC()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_pcdc.InitHashTable(HASHSIZEPCDC);
}

CDB_PCDC::~CDB_PCDC()
{
}

void CDB_PCDC::Add(T_PCDC_K Key,T_PCDC_D& rData)
{
	T_PCDC_D Data;
	BOOL bExist=m_pcdc.Lookup(Key, Data);
	m_pcdc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_PCDC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PCDC::Del(T_PCDC_K Key)
{
	T_PCDC_D Data;
	BOOL bExist=m_pcdc.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_pcdc.RemoveKey(Key);
		ASSERT(ret);
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PCDC_K key;
			T_PCDC_D data;
			if(m_pcdc.Lookup(m_nLastNum-1,data))
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