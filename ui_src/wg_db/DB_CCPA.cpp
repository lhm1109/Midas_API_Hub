#include "stdafx.h"
#include "DB_CCPA.h"
#include "DB_ELEM.h"

CDB_CCPA::CDB_CCPA()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_ccpa.InitHashTable(HASHSIZECCPA);
}

CDB_CCPA::~CDB_CCPA()
{
}

void CDB_CCPA::Add(T_CCPA_K Key,T_CCPA_D& rData, CDB_ELEM* pElem)
{
	T_CCPA_D Data;
	BOOL bExist=m_ccpa.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pElem->DelListItem(Key, LT_CCPA_CMD, Key));
	}
	m_ccpa.SetAt(Key,rData);
	if (pElem)
	{
		pElem->AddListItem(Key, LT_CCPA_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_CCPA_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_CCPA::Del(T_CCPA_K Key, CDB_ELEM* pElem)
{
	T_CCPA_D Data;
	BOOL bExist=m_ccpa.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_ccpa.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pElem)
			{
				VERIFY(pElem->DelListItem(Key, LT_CCPA_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_CCPA_K key;
			T_CCPA_D data;
			if(m_ccpa.Lookup(m_nLastNum-1,data))
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