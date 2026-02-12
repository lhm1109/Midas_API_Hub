#include "stdafx.h"
#include "DB_ENVL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ENVL::CDB_ENVL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_envl.InitHashTable(HASHSIZEENVL);
}

CDB_ENVL::~CDB_ENVL()
{
}

void CDB_ENVL::Add(T_ENVL_K Key,T_ENVL_D& rData)
{
	return; // No support from ver 5.2.0
	T_ENVL_D Data;
	BOOL bExist = m_envl.Lookup(Key, Data);
	m_envl.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_ENVL_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
	// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_ENVL_CMD, Key);
	}
}

BOOL CDB_ENVL::Del(T_ENVL_K Key)
{
	BOOL ret=m_envl.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_ENVL_K key;
			T_ENVL_D data;
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
		// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_ENVL_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_ENVL::Get(T_ENVL_K Key,T_ENVL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_envl->Lookup(Key, uTemp)) return FALSE;
	}
	return m_envl.Lookup(Key,rData);
}

int CDB_ENVL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_envl->GetCount();
	}
	return m_envl.GetCount();
}

POSITION CDB_ENVL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		return m_pDoc->m_pStagCtrl->m_envl->GetStartPosition();
	}
	return m_envl.GetStartPosition();
}

void CDB_ENVL::GetNext(POSITION& rNextPosition,T_ENVL_K& rKey,T_ENVL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_envl->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_envl.Lookup(rKey, rData);
		return;
	}
	m_envl.GetNextAssoc(rNextPosition,rKey,rData);
}
