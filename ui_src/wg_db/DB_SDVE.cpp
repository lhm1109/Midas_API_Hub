#include "stdafx.h"
#include "DB_SDVE.h"
#include "DB_ELEM.h"

CDB_SDVE::CDB_SDVE()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_sdve.InitHashTable(HASHSIZESDVE);
}

CDB_SDVE::~CDB_SDVE()
{
}

void CDB_SDVE::Add(T_SDVE_K Key,T_SDVE_D& rData)
{
	m_sdve.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_SDVE_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SDVE::Del(T_SDVE_K Key)
{
	T_SDVE_D Data;
	BOOL bExist=m_sdve.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_sdve.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SDVE_K key;
			T_SDVE_D data;
			if(m_sdve.Lookup(m_nLastNum-1,data))
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