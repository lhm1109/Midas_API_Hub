#include "stdafx.h"
#include "DB_LDFS.h"

#include "DBDoc.h"

CDB_LDFS::CDB_LDFS()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_ldfs.InitHashTable(HASHSIZELDFS);
}

CDB_LDFS::~CDB_LDFS()
{
}

void CDB_LDFS::Add(T_LDFS_K Key,T_LDFS_D& rData)
{
	T_LDFS_D Data;
	BOOL bExist = m_ldfs.Lookup(Key, Data);
	m_ldfs.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_LDFS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_LDFS::Del(T_LDFS_K Key)
{
	T_LDFS_D Data;
	m_ldfs.Lookup(Key, Data);
	BOOL ret=m_ldfs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_LDFS_K key;
			T_LDFS_D data;
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

//-------------------------------------------------------------------------
