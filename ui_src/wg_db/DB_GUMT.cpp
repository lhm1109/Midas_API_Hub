#include "stdafx.h"
#include "DB_GUMT.h"
#include "DB_MATL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GUMT::CDB_GUMT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_gumt.InitHashTable(HASHSIZEGUMT);
}

CDB_GUMT::~CDB_GUMT()
{
}

void CDB_GUMT::Add(T_GUMT_K Key,T_GUMT_D& rData, CDB_MATL* pMatl)
{
	T_GUMT_D Data;
	BOOL bExist=m_gumt.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pMatl->DelListItem(Key, LT_GUMT_CMD, Key));
	}
	m_gumt.SetAt(Key,rData);
	if (pMatl)
	{
		pMatl->AddListItem(Key, LT_GUMT_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_GUMT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GUMT::Del(T_GUMT_K Key, CDB_MATL* pMatl)
{
	T_GUMT_D Data;
	BOOL bExist=m_gumt.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_gumt.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pMatl)
			{
				VERIFY(pMatl->DelListItem(Key, LT_GUMT_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GUMT_K key;
			T_GUMT_D data;
			if(m_gumt.Lookup(m_nLastNum-1,data))
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

BOOL CDB_GUMT::Get(T_GUMT_K Key, T_GUMT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_gumt->Lookup(Key, uTemp)) return FALSE;
	}
	return m_gumt.Lookup(Key, rData);
}
int CDB_GUMT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gumt->GetCount();
	}
	return m_gumt.GetCount();
}
POSITION CDB_GUMT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gumt->GetStartPosition();
	}
	return m_gumt.GetStartPosition();
}
void CDB_GUMT::GetNext(POSITION& rNextPosition, T_GUMT_K& rKey, T_GUMT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_gumt->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_gumt.Lookup(rKey, rData);
		return;
	}
	m_gumt.GetNextAssoc(rNextPosition, rKey, rData);
}