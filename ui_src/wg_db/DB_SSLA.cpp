#include "stdafx.h"
#include "DB_SSLA.h"
#include "DB_ELEM.h"

CDB_SSLA::CDB_SSLA()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_ssla.InitHashTable(HASHSIZESSLA);
}

CDB_SSLA::~CDB_SSLA()
{
}

void CDB_SSLA::Add(T_SSLA_K Key,T_SSLA_D& rData, CDB_ELEM* pElem)
{
	T_SSLA_D Data;
	BOOL bExist=m_ssla.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pElem->DelListItem(Key, LT_SSLA_CMD, Key));
	}
	m_ssla.SetAt(Key,rData);
	if (pElem)
	{
		pElem->AddListItem(Key, LT_SSLA_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_SSLA_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SSLA::Del(T_SSLA_K Key, CDB_ELEM* pElem)
{
	T_SSLA_D Data;
	BOOL bExist=m_ssla.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_ssla.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pElem)
			{
				VERIFY(pElem->DelListItem(Key, LT_SSLA_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SSLA_K key;
			T_SSLA_D data;
			if(m_ssla.Lookup(m_nLastNum-1,data))
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