#include "stdafx.h"
#include "DB_TDST.h"

CDB_TDST::CDB_TDST()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_tdst.InitHashTable(HASHSIZETDST);
}

CDB_TDST::~CDB_TDST()
{
}

void CDB_TDST::Add(T_TDST_K Key, T_TDST_D& rData)
{
	T_TDST_D Data;
	BOOL bExist=m_tdst.Lookup(Key, Data);
	m_tdst.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_TDST_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_TDST::Del(T_TDST_K Key)
{
	T_TDST_D Data;
	BOOL bExist=m_tdst.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_tdst.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_TDST_K key;
			T_TDST_D data;
			if(m_tdst.Lookup(m_nLastNum-1,data))
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