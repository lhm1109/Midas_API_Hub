#include "stdafx.h"
#include "DB_MSTR.h"

CDB_MSTR::CDB_MSTR()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_mstr.InitHashTable(HASHSIZEMSTR);
}

CDB_MSTR::~CDB_MSTR()
{
}

void CDB_MSTR::Add(T_MSTR_K Key,T_MSTR_D& rData)
{
	T_MSTR_D Data;
	BOOL bExist=m_mstr.Lookup(Key, Data);
//   if(bExist)
//   {
//     VERIFY(pElem->DelListItem(MembK, LT_MSTR_CMD, Key));
//   }
	m_mstr.SetAt(Key,rData);
//   if (pElem)
//   {
//     pElem->AddListItem(MembK, LT_MSTR_CMD, Key);
//   }

	if(Key == m_nStartNum)
	{
		T_MSTR_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MSTR::Del(T_MSTR_K Key)
{
	T_MSTR_D Data;
	BOOL bExist=m_mstr.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_mstr.RemoveKey(Key);
		ASSERT(ret);
//     if(ret)
//     {
//       if (pElem)
//       {
//         VERIFY(pElem->DelListItem(MembK, LT_MSTR_CMD, Key));
//       }
//     }
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MSTR_K key;
			T_MSTR_D data;
			if(m_mstr.Lookup(m_nLastNum-1,data))
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