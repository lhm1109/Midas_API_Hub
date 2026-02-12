#include "stdafx.h"
#include "DB_MISA.h"
#include "DB_ELEM.h"

CDB_MISA::CDB_MISA()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_misa.InitHashTable(HASHSIZEMISA);
}

CDB_MISA::~CDB_MISA()
{
}

void CDB_MISA::Add(T_MISA_K Key,T_MISA_D& rData, CDB_ELEM* pElem)
{
	T_MISA_D Data;
	BOOL bExist=m_misa.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pElem->DelListItem(Key, LT_MISA_CMD, Key));
	}
	m_misa.SetAt(Key,rData);
	if (pElem)
	{
		pElem->AddListItem(Key, LT_MISA_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_MISA_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MISA::Del(T_MISA_K Key, CDB_ELEM* pElem)
{
	T_MISA_D Data;
	BOOL bExist=m_misa.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_misa.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pElem)
			{
				VERIFY(pElem->DelListItem(Key, LT_MISA_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MISA_K key;
			T_MISA_D data;
			if(m_misa.Lookup(m_nLastNum-1,data))
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