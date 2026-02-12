#include "stdafx.h"
#include "DB_MHTP.h"

CDB_MHTP::CDB_MHTP()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_mhtp.InitHashTable(HASHSIZEMHTP);
}

CDB_MHTP::~CDB_MHTP()
{
}

void CDB_MHTP::Add(T_MHTP_K Key,T_MHTP_D& rData)
{
	m_mhtp.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MHTP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MHTP::Del(T_MHTP_K Key)
{
	BOOL ret=m_mhtp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MHTP_K key;
			T_MHTP_D data;
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

