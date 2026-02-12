#include "stdafx.h"
#include "DB_MPCC.h"

CDB_MPCC::CDB_MPCC()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_mpcc.InitHashTable(HASHSIZEMPCC);
}

CDB_MPCC::~CDB_MPCC()
{

}

void CDB_MPCC::Add(T_MPCC_K Key,T_MPCC_D& rData)
{
	m_mpcc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MPCC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MPCC::Del(T_MPCC_K Key)
{
	BOOL ret=m_mpcc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MPCC_K key;
			T_MPCC_D data;
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


