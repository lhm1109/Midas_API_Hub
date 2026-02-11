#include "stdafx.h"
#include "DB_CCPT.h"

CDB_CCPT::CDB_CCPT()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_ccpt.InitHashTable(HASHSIZECCPT);
}

CDB_CCPT::~CDB_CCPT()
{
}

void CDB_CCPT::Add(T_CCPT_K Key,T_CCPT_D& rData)
{
	T_CCPT_D Data;
	BOOL bExist=m_ccpt.Lookup(Key, Data);
//   if(bExist)
//   {
//     VERIFY(pNode->DelListItem(Data.anNodeK[0], LT_CCPT_CMD, Key));
//     VERIFY(pNode->DelListItem(Data.anNodeK[1], LT_CCPT_CMD, Key));
//   }
	m_ccpt.SetAt(Key,rData);
//   if (pNode)
//   {
//     pNode->AddListItem(rData.anNodeK[0], LT_CCPT_CMD, Key);
//     pNode->AddListItem(rData.anNodeK[1], LT_CCPT_CMD, Key);
//   }
	if(Key == m_nStartNum)
	{
		T_CCPT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_CCPT::Del(T_CCPT_K Key)
{
	T_CCPT_D Data;
	BOOL bExist=m_ccpt.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_ccpt.RemoveKey(Key);
		ASSERT(ret);
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_CCPT_K key;
			T_CCPT_D data;
			if(m_ccpt.Lookup(m_nLastNum-1,data))
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