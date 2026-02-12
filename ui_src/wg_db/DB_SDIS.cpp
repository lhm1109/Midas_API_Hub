#include "stdafx.h"
#include "DB_SDIS.h"
#include "DB_ELEM.h"

CDB_SDIS::CDB_SDIS()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_sdis.InitHashTable(HASHSIZESDIS);
}

CDB_SDIS::~CDB_SDIS()
{
}

void CDB_SDIS::Add(T_SDIS_K Key,T_SDIS_D& rData)
{
	m_sdis.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_SDIS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SDIS::Del(T_SDIS_K Key)
{
	T_SDIS_D Data;
	BOOL bExist=m_sdis.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_sdis.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SDIS_K key;
			T_SDIS_D data;
			if(m_sdis.Lookup(m_nLastNum-1,data))
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