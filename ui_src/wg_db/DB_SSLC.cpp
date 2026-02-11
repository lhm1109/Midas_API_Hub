#include "stdafx.h"
#include "DB_SSLC.h"

CDB_SSLC::CDB_SSLC()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_sslc.InitHashTable(HASHSIZESSLC);
}

CDB_SSLC::~CDB_SSLC()
{
}

void CDB_SSLC::Add(T_SSLC_K Key,T_SSLC_D& rData)
{
	T_SSLC_D Data;
	BOOL bExist=m_sslc.Lookup(Key, Data);
	m_sslc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_SSLC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SSLC::Del(T_SSLC_K Key)
{
	T_SSLC_D Data;
	BOOL bExist=m_sslc.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_sslc.RemoveKey(Key);
		ASSERT(ret);
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SSLC_K key;
			T_SSLC_D data;
			if(m_sslc.Lookup(m_nLastNum-1,data))
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