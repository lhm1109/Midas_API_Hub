#include "stdafx.h"
#include "DB_TDME.h"

CDB_TDME::CDB_TDME()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_tdme.InitHashTable(HASHSIZETDME);
}

CDB_TDME::~CDB_TDME()
{
}

void CDB_TDME::Add(T_TDME_K Key,T_TDME_D& rData)
{
	m_tdme.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_TDME_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_TDME::Del(T_TDME_K Key)
{
	BOOL ret=m_tdme.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_TDME_K key;
			T_TDME_D data;
			if(Get(m_nLastNum-1,data))
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
	}
	return ret;
}

