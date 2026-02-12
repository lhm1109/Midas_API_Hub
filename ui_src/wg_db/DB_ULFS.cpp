#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_LCOM.h"
#include "DB_ULFS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ULFS::CDB_ULFS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_ulfs.InitHashTable(HASHSIZEULFS);
}

CDB_ULFS::~CDB_ULFS()
{
}

void CDB_ULFS::Add(T_ULFS_K Key,T_ULFS_D& rData)
{
	T_ULFS_D Data;
	BOOL bExist = m_ulfs.Lookup(Key, Data);
	m_ulfs.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_ULFS_D data;
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
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_ULFS_CMD, Key);
	}
}

BOOL CDB_ULFS::Del(T_ULFS_K Key)
{
	BOOL ret=m_ulfs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_ULFS_K key;
			T_ULFS_D data;
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
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_ULFS_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_ULFS::Get(T_ULFS_K Key,T_ULFS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_ulfs->Lookup(Key, uTemp)) return FALSE;
	}
	return m_ulfs.Lookup(Key,rData);
}

int CDB_ULFS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ulfs->GetCount();
	}
	return m_ulfs.GetCount();
}

POSITION CDB_ULFS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ulfs->GetStartPosition();
	}
	return m_ulfs.GetStartPosition();
}

void CDB_ULFS::GetNext(POSITION& rNextPosition,T_ULFS_K& rKey,T_ULFS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_ulfs->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_ulfs.Lookup(rKey, rData);
		return;
	}
	m_ulfs.GetNextAssoc(rNextPosition,rKey,rData);
}
