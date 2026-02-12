#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_LCOM.h"
#include "DB_ULFD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ULFD::CDB_ULFD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_ulfd.InitHashTable(HASHSIZEULFD);
}

CDB_ULFD::~CDB_ULFD()
{
}

void CDB_ULFD::Add(T_ULFD_K Key,T_ULFD_D& rData)
{
	T_ULFD_D Data;
	BOOL bExist = m_ulfd.Lookup(Key, Data);
	m_ulfd.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_ULFD_D data;
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
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_ULFD_CMD, Key);
	}
}

BOOL CDB_ULFD::Del(T_ULFD_K Key)
{
	BOOL ret=m_ulfd.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_ULFD_K key;
			T_ULFD_D data;
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
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_ULFC_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_ULFD::Get(T_ULFD_K Key,T_ULFD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_ulfd->Lookup(Key, uTemp)) return FALSE;
	}
	return m_ulfd.Lookup(Key,rData);
}

int CDB_ULFD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ulfd->GetCount();
	}
	return m_ulfd.GetCount();
}

POSITION CDB_ULFD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ulfd->GetStartPosition();
	}
	return m_ulfd.GetStartPosition();
}

void CDB_ULFD::GetNext(POSITION& rNextPosition,T_ULFD_K& rKey,T_ULFD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_ulfd->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_ulfd.Lookup(rKey, rData);
		return;
	}
	m_ulfd.GetNextAssoc(rNextPosition,rKey,rData);
}
