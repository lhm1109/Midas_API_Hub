#include "stdafx.h"
#include "DB_FSUM.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FSUM::CDB_FSUM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_fsum.InitHashTable(HASHSIZEFSUM);
}

CDB_FSUM::~CDB_FSUM()
{
}

void CDB_FSUM::Add(T_FSUM_K Key,T_FSUM_D& rData, CDB_ELEM* pElem)
{
	T_FSUM_D Data;
	BOOL bExist = m_fsum.Lookup(Key, Data);
	if (pElem) pElem->AddListItem(rData.aElem, Data.aElem, LT_FSUM_CMD, Key, bExist);
	m_fsum.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_FSUM_D data;
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
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_FSUM_CMD, Key);
	}
}

BOOL CDB_FSUM::Del(T_FSUM_K Key, CDB_ELEM* pElem)
{
	T_FSUM_D Data;
	m_fsum.Lookup(Key, Data);
	BOOL ret=m_fsum.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(pElem) pElem->DelListItem(Data.aElem, LT_FSUM_CMD, Key);
		if(Key < m_nStartNum) m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_FSUM_K key;
			T_FSUM_D data;
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
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_FSUM_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_FSUM::Get(T_FSUM_K Key,T_FSUM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_fsum->Lookup(Key, uTemp)) return FALSE;
	}
	return m_fsum.Lookup(Key,rData);
}

int CDB_FSUM::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag())
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_fsum->GetCount();
	}
	return m_fsum.GetCount();
}

POSITION CDB_FSUM::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_fsum->GetStartPosition();
	}
	return m_fsum.GetStartPosition();
}

void CDB_FSUM::GetNext(POSITION& rNextPosition,T_FSUM_K& rKey,T_FSUM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_fsum->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_fsum.Lookup(rKey, rData);
		return;
	}
	m_fsum.GetNextAssoc(rNextPosition,rKey,rData);
}
