#include "stdafx.h"
#include "DB_GUEL.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GUEL::CDB_GUEL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_guel.InitHashTable(HASHSIZEGUEL);
}

CDB_GUEL::~CDB_GUEL()
{
}

void CDB_GUEL::Add(T_GUEL_K Key,T_GUEL_D& rData, CDB_ELEM* pElem)
{
	T_GUEL_D Data;
	BOOL bExist=m_guel.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pElem->DelListItem(Key, LT_GUEL_CMD, Key));
	}
	m_guel.SetAt(Key,rData);
	if (pElem)
	{
		pElem->AddListItem(Key, LT_GUEL_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_GUEL_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GUEL::Del(T_GUEL_K Key, CDB_ELEM* pElem)
{
	T_GUEL_D Data;
	BOOL bExist=m_guel.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_guel.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pElem)
			{
				VERIFY(pElem->DelListItem(Key, LT_GUEL_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GUEL_K key;
			T_GUEL_D data;
			if(m_guel.Lookup(m_nLastNum-1,data))
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

BOOL CDB_GUEL::Get(T_GUEL_K Key, T_GUEL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_guel->Lookup(Key, uTemp)) return FALSE;
	}
	return m_guel.Lookup(Key, rData);
}
int CDB_GUEL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_guel->GetCount();
	}
	return m_guel.GetCount();
}
POSITION CDB_GUEL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_guel->GetStartPosition();
	}
	return m_guel.GetStartPosition();
}
void CDB_GUEL::GetNext(POSITION& rNextPosition, T_GUEL_K& rKey, T_GUEL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_guel->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_guel.Lookup(rKey, rData);
		return;
	}
	m_guel.GetNextAssoc(rNextPosition, rKey, rData);
}