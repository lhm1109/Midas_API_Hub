#include "stdafx.h"
#include "DB_PEBA.h"
#include "DB_ELEM.h"

CDB_PEBA::CDB_PEBA()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_peba.InitHashTable(HASHSIZEPEBA);
}

CDB_PEBA::~CDB_PEBA()
{
}

void CDB_PEBA::Add(T_PEBA_K Key,T_PEBA_D& rData, CDB_ELEM* pElem)
{
	T_PEBA_D Data;
	BOOL bExist=m_peba.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pElem->DelListItem(Key, LT_PEBA_CMD, Key));
	}
	m_peba.SetAt(Key,rData);
	if (pElem)
	{
		pElem->AddListItem(Key, LT_PEBA_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_PEBA_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PEBA::Del(T_PEBA_K Key, CDB_ELEM* pElem)
{
	T_PEBA_D Data;
	BOOL bExist=m_peba.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_peba.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pElem)
			{
				VERIFY(pElem->DelListItem(Key, LT_PEBA_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PEBA_K key;
			T_PEBA_D data;
			if(m_peba.Lookup(m_nLastNum-1,data))
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