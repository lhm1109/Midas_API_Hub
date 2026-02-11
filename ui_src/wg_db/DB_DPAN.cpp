#include "stdafx.h"
#include "DB_DPAN.h"

CDB_DPAN::CDB_DPAN()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_dpan.InitHashTable(HASHSIZEDPAN);
}

CDB_DPAN::~CDB_DPAN()
{
}

void CDB_DPAN::Add(T_DPAN_K Key,T_DPAN_D& rData)
{
	T_DPAN_D Data;
	BOOL bExist=m_dpan.Lookup(Key, Data);
//   if(bExist)
//   {
//     VERIFY(pNode->DelListItem(Data.anNodeK[0], LT_DPAN_CMD, Key));
//     VERIFY(pNode->DelListItem(Data.anNodeK[1], LT_DPAN_CMD, Key));
//   }
	m_dpan.SetAt(Key,rData);
//   if (pNode)
//   {
//     pNode->AddListItem(rData.anNodeK[0], LT_DPAN_CMD, Key);
//     pNode->AddListItem(rData.anNodeK[1], LT_DPAN_CMD, Key);
//   }
	if(Key == m_nStartNum)
	{
		T_DPAN_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_DPAN::Del(T_DPAN_K Key)
{
	T_DPAN_D Data;
	BOOL bExist=m_dpan.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_dpan.RemoveKey(Key);
		ASSERT(ret);
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_DPAN_K key;
			T_DPAN_D data;
			if(m_dpan.Lookup(m_nLastNum-1,data))
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