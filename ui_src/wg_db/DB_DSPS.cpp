#include "stdafx.h"
#include "DB_DSPS.h"

CDB_DSPS::CDB_DSPS()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_dsps.InitHashTable(HASHSIZEDSPS);
}

CDB_DSPS::~CDB_DSPS()
{
}

void CDB_DSPS::Add(T_DSPS_K Key, T_DSPS_D& rData)
{
	T_DSPS_D Data;
	BOOL bExist=m_dsps.Lookup(Key, Data);
	m_dsps.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_DSPS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_DSPS::Del(T_DSPS_K Key)
{
	T_DSPS_D Data;
	BOOL bExist=m_dsps.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_dsps.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_DSPS_K key;
			T_DSPS_D data;
			if(m_dsps.Lookup(m_nLastNum-1,data))
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
