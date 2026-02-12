#include "stdafx.h"
#include "DB_GUSC.h"
#include "DB_SECT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GUSC::CDB_GUSC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_gusc.InitHashTable(HASHSIZEGUSC);
}

CDB_GUSC::~CDB_GUSC()
{
}

void CDB_GUSC::Add(T_GUSC_K Key,T_GUSC_D& rData, CDB_SECT* pSect)
{
	T_GUSC_D Data;
	BOOL bExist=m_gusc.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pSect->DelListItem(Key, LT_GUSC_CMD, Key));
	}
	m_gusc.SetAt(Key,rData);
	if (pSect)
	{
		pSect->AddListItem(Key, LT_GUSC_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_GUSC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GUSC::Del(T_GUSC_K Key, CDB_SECT* pSect)
{
	T_GUSC_D Data;
	BOOL bExist=m_gusc.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_gusc.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pSect)
			{
				VERIFY(pSect->DelListItem(Key, LT_GUSC_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GUSC_K key;
			T_GUSC_D data;
			if(m_gusc.Lookup(m_nLastNum-1,data))
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

BOOL CDB_GUSC::Get(T_GUSC_K Key, T_GUSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_gusc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_gusc.Lookup(Key, rData);
}
int CDB_GUSC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gusc->GetCount();
	}
	return m_gusc.GetCount();
}
POSITION CDB_GUSC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gusc->GetStartPosition();
	}
	return m_gusc.GetStartPosition();
}
void CDB_GUSC::GetNext(POSITION& rNextPosition, T_GUSC_K& rKey, T_GUSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_gusc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_gusc.Lookup(rKey, rData);
		return;
	}
	m_gusc.GetNextAssoc(rNextPosition, rKey, rData);
}