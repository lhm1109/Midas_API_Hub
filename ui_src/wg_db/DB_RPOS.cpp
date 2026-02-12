#include "stdafx.h"
#include "DB_RPOS.h"

#include "DB_NODE.h"

CDB_RPOS::CDB_RPOS()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_rpos.InitHashTable(HASHSIZERPOS);
}

CDB_RPOS::~CDB_RPOS()
{
}

void CDB_RPOS::Add(T_RPOS_K Key,T_RPOS_D& rData,CDB_NODE* m_pNode)
{
	T_RPOS_D Data;
	BOOL bExist = m_rpos.Lookup(Key, Data);
	CArray<UINT, UINT> aOldNodeK, aNewNodeK;
	if (bExist)
	{
		for (int i = 0; i < Data.aReacPos.GetSize(); i++)
			aOldNodeK.Add(Data.aReacPos[i].Node);
	}
	for (int i = 0; i < rData.aReacPos.GetSize(); i++)
		aNewNodeK.Add(rData.aReacPos[i].Node);
	if (m_pNode) m_pNode->AddListItem(aNewNodeK, aOldNodeK, LT_RPOS_CMD, Key, bExist);

	m_rpos.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_RPOS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_RPOS::Del(T_RPOS_K Key,CDB_NODE* m_pNode)
{
	T_RPOS_D Data;
	m_rpos.Lookup(Key, Data);

	BOOL ret=m_rpos.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if (m_pNode)
		{
			for (int i = 0; i < Data.aReacPos.GetSize(); i++)
				m_pNode->DelListItem(Data.aReacPos[i].Node, LT_RPOS_CMD, Key);     
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_RPOS_K key;
			T_RPOS_D data;
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
