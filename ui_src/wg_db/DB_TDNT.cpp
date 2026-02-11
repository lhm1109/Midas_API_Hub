#include "stdafx.h"
#include "DB_TDNT.h"

CDB_TDNT::CDB_TDNT()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_tdnt.InitHashTable(HASHSIZETDNT);
}

CDB_TDNT::~CDB_TDNT()
{
}

void CDB_TDNT::Add(T_TDNT_K Key,T_TDNT_D& rData)
{
	m_tdnt.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_TDNT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_TDNT::Del(T_TDNT_K Key)
{
	BOOL ret=m_tdnt.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_TDNT_K key;
			T_TDNT_D data;
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

