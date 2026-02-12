#include "stdafx.h"
#include "DB_SYEF.h"

CDB_SYEF::CDB_SYEF()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_syef.InitHashTable(HASHSIZESYEF);
}

CDB_SYEF::~CDB_SYEF()
{

}

void CDB_SYEF::Add(T_SYEF_K Key,T_SYEF_D& rData)
{
	m_syef.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_SYEF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SYEF::Del(T_SYEF_K Key)
{
	BOOL ret=m_syef.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SYEF_K key;
			T_SYEF_D data;
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


