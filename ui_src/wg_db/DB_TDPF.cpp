#include "stdafx.h"
#include "DB_TDPF.h"

CDB_TDPF::CDB_TDPF()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_tdpf.InitHashTable(HASHSIZETDPF);
}

CDB_TDPF::~CDB_TDPF()
{
}

void CDB_TDPF::Add(T_TDPF_K Key, T_TDPF_D& rData)
{
	T_TDPF_D Data;
	BOOL bExist=m_tdpf.Lookup(Key, Data);
	m_tdpf.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_TDPF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_TDPF::Del(T_TDPF_K Key)
{
	T_TDPF_D Data;
	BOOL bExist=m_tdpf.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_tdpf.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_TDPF_K key;
			T_TDPF_D data;
			if(m_tdpf.Lookup(m_nLastNum-1,data))
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