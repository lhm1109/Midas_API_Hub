#include "stdafx.h"
#include "DB_GUMB.h"
#include "DB_MEMB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GUMB::CDB_GUMB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_gumb.InitHashTable(HASHSIZEGUMB);
}

CDB_GUMB::~CDB_GUMB()
{
}

void CDB_GUMB::Add(T_GUMB_K Key,T_GUMB_D& rData, CDB_MEMB* pMemb)
{
	T_GUMB_D Data;
	BOOL bExist=m_gumb.Lookup(Key, Data);
	m_gumb.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_GUMB_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GUMB::Del(T_GUMB_K Key, CDB_MEMB* pMemb)
{
	T_GUMB_D Data;
	BOOL bExist=m_gumb.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_gumb.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GUMB_K key;
			T_GUMB_D data;
			if(m_gumb.Lookup(m_nLastNum-1,data))
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

BOOL CDB_GUMB::Get(T_GUMB_K Key, T_GUMB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_gumb->Lookup(Key, uTemp)) return FALSE;
	}
	return m_gumb.Lookup(Key, rData);
}
int CDB_GUMB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gumb->GetCount();
	}
	return m_gumb.GetCount();
}
POSITION CDB_GUMB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gumb->GetStartPosition();
	}
	return m_gumb.GetStartPosition();
}
void CDB_GUMB::GetNext(POSITION& rNextPosition, T_GUMB_K& rKey, T_GUMB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_gumb->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_gumb.Lookup(rKey, rData);
		return;
	}
	m_gumb.GetNextAssoc(rNextPosition, rKey, rData);
}