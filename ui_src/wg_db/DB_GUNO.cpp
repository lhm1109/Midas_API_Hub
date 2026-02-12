#include "stdafx.h"
#include "DB_GUNO.h"
#include "DB_NODE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GUNO::CDB_GUNO()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_guno.InitHashTable(HASHSIZEGUNO);
}

CDB_GUNO::~CDB_GUNO()
{
}

void CDB_GUNO::Add(T_GUNO_K Key,T_GUNO_D& rData, CDB_NODE* pNode)
{
	T_GUNO_D Data;
	BOOL bExist=m_guno.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pNode->DelListItem(Key, LT_GUNO_CMD, Key));
	}
	m_guno.SetAt(Key,rData);
	if (pNode)
	{
		pNode->AddListItem(Key, LT_GUNO_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_GUNO_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GUNO::Del(T_GUNO_K Key, CDB_NODE* pNode)
{
	T_GUNO_D Data;
	BOOL bExist=m_guno.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_guno.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pNode)
			{
				VERIFY(pNode->DelListItem(Key, LT_GUNO_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GUNO_K key;
			T_GUNO_D data;
			if(m_guno.Lookup(m_nLastNum-1,data))
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

BOOL CDB_GUNO::Get(T_GUNO_K Key, T_GUNO_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_guno->Lookup(Key, uTemp)) return FALSE;
	}
	return m_guno.Lookup(Key, rData);
}
int CDB_GUNO::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_guno->GetCount();
	}
	return m_guno.GetCount();
}
POSITION CDB_GUNO::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_guno->GetStartPosition();
	}
	return m_guno.GetStartPosition();
}
void CDB_GUNO::GetNext(POSITION& rNextPosition, T_GUNO_K& rKey, T_GUNO_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_guno->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_guno.Lookup(rKey, rData);
		return;
	}
	m_guno.GetNextAssoc(rNextPosition, rKey, rData);
}