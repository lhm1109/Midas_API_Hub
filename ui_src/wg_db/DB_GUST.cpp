#include "stdafx.h"
#include "DB_GUST.h"
#include "DB_STOR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GUST::CDB_GUST()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_gust.InitHashTable(HASHSIZEGUST);
}

CDB_GUST::~CDB_GUST()
{
}

void CDB_GUST::Add(T_GUST_K Key,T_GUST_D& rData, CDB_STOR* pStor)
{
	T_GUST_D Data;
	BOOL bExist=m_gust.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pStor->DelListItem(Key, LT_GUST_CMD, Key));
	}
	m_gust.SetAt(Key,rData);
	if (pStor)
	{
		pStor->AddListItem(Key, LT_GUST_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_GUST_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GUST::Del(T_GUST_K Key, CDB_STOR* pStor)
{
	T_GUST_D Data;
	BOOL bExist=m_gust.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_gust.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pStor)
			{
				VERIFY(pStor->DelListItem(Key, LT_GUST_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GUST_K key;
			T_GUST_D data;
			if(m_gust.Lookup(m_nLastNum-1,data))
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

BOOL CDB_GUST::Get(T_GUST_K Key, T_GUST_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_gust->Lookup(Key, uTemp)) return FALSE;
	}
	return m_gust.Lookup(Key, rData);
}
int CDB_GUST::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gust->GetCount();
	}
	return m_gust.GetCount();
}
POSITION CDB_GUST::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gust->GetStartPosition();
	}
	return m_gust.GetStartPosition();
}
void CDB_GUST::GetNext(POSITION& rNextPosition, T_GUST_K& rKey, T_GUST_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_gust->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_gust.Lookup(rKey, rData);
		return;
	}
	m_gust.GetNextAssoc(rNextPosition, rKey, rData);
}