#include "stdafx.h"
#include "DB_PILE.h"

CDB_PILE::CDB_PILE()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_pile.InitHashTable(HASHSIZEPILE);
}

CDB_PILE::~CDB_PILE()
{
}

void CDB_PILE::Add(T_PILE_K Key,T_PILE_D& rData)
{
	m_pile.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_PILE_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PILE::Del(T_PILE_K Key)
{
	BOOL ret=m_pile.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PILE_K key;
			T_PILE_D data;
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