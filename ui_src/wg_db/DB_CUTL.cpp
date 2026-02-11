#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_CUTL.h"

CDB_CUTL::CDB_CUTL()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_cutl.InitHashTable(HASHSIZECUTL);
}

CDB_CUTL::~CDB_CUTL()
{
}

void CDB_CUTL::Add(T_CUTL_K Key,T_CUTL_D& rData)
{
	m_cutl.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_CUTL_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_CUTL::Del(T_CUTL_K Key)
{
	BOOL ret=m_cutl.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_CUTL_K key;
			T_CUTL_D data;
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

