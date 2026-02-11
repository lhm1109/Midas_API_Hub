#include "stdafx.h"
#include "DB_MSIS.h"

CDB_MSIS::CDB_MSIS()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_msis.InitHashTable(HASHSIZEMSIS);
}

CDB_MSIS::~CDB_MSIS()
{
}

void CDB_MSIS::Add(T_MSIS_K Key,T_MSIS_D& rData)
{
	T_MSIS_D Data;
	BOOL bExist=m_msis.Lookup(Key, Data);
//   if(bExist)
//   {
//     VERIFY(pNode->DelListItem(Data.anNodeK[0], LT_MSIS_CMD, Key));
//     VERIFY(pNode->DelListItem(Data.anNodeK[1], LT_MSIS_CMD, Key));
//   }
	m_msis.SetAt(Key,rData);
//   if (pNode)
//   {
//     pNode->AddListItem(rData.anNodeK[0], LT_MSIS_CMD, Key);
//     pNode->AddListItem(rData.anNodeK[1], LT_MSIS_CMD, Key);
//   }
	if(Key == m_nStartNum)
	{
		T_MSIS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MSIS::Del(T_MSIS_K Key)
{
	T_MSIS_D Data;
	BOOL bExist=m_msis.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_msis.RemoveKey(Key);
		ASSERT(ret);
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MSIS_K key;
			T_MSIS_D data;
			if(m_msis.Lookup(m_nLastNum-1,data))
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