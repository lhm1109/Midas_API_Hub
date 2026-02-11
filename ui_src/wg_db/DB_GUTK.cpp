#include "stdafx.h"
#include "DB_GUTK.h"
#include "DB_THIK.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GUTK::CDB_GUTK()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_gutk.InitHashTable(HASHSIZEGUTK);
}

CDB_GUTK::~CDB_GUTK()
{
}

void CDB_GUTK::Add(T_GUTK_K Key,T_GUTK_D& rData, CDB_THIK* pThik)
{
	T_GUTK_D Data;
	BOOL bExist=m_gutk.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pThik->DelListItem(Key, LT_GUTK_CMD, Key));
	}
	m_gutk.SetAt(Key,rData);
	if (pThik)
	{
		pThik->AddListItem(Key, LT_GUTK_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_GUTK_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GUTK::Del(T_GUTK_K Key, CDB_THIK* pThik)
{
	T_GUTK_D Data;
	BOOL bExist=m_gutk.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_gutk.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pThik)
			{
				VERIFY(pThik->DelListItem(Key, LT_GUTK_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GUTK_K key;
			T_GUTK_D data;
			if(m_gutk.Lookup(m_nLastNum-1,data))
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

BOOL CDB_GUTK::Get(T_GUTK_K Key, T_GUTK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_gutk->Lookup(Key, uTemp)) return FALSE;
	}
	return m_gutk.Lookup(Key, rData);
}

int CDB_GUTK::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gutk->GetCount();
	}
	return m_gutk.GetCount();
}

POSITION CDB_GUTK::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gutk->GetStartPosition();
	}
	return m_gutk.GetStartPosition();
}
void CDB_GUTK::GetNext(POSITION& rNextPosition, T_GUTK_K& rKey, T_GUTK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_gutk->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_gutk.Lookup(rKey, rData);
		return;
	}
	m_gutk.GetNextAssoc(rNextPosition, rKey, rData);
}