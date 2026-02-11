#include "stdafx.h"
#include "DB_RIPB.h"

CDB_RIPB::CDB_RIPB()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_ripb.InitHashTable(HASHSIZERIPB);
}

CDB_RIPB::~CDB_RIPB()
{
}

void CDB_RIPB::Add(T_RIPB_K Key,T_RIPB_D& rData)
{
	T_RIPB_D Data;
	BOOL bExist=m_ripb.Lookup(Key, Data);
	m_ripb.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_RIPB_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_RIPB::Del(T_RIPB_K Key)
{
	T_RIPB_D Data;
	BOOL bExist=m_ripb.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_ripb.RemoveKey(Key);
		ASSERT(ret);
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_RIPB_K key;
			T_RIPB_D data;
			if(m_ripb.Lookup(m_nLastNum-1,data))
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