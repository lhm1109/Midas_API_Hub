#include "stdafx.h"
#include "DB_GRUP.h"
#include "DB_CFTR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CFTR::CDB_CFTR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_cftr.InitHashTable(HASHSIZECFTR);
}

CDB_CFTR::~CDB_CFTR()
{
}

void CDB_CFTR::Add(T_CFTR_K Key,T_CFTR_D& rData,CDB_GRUP* pGrup)
{
	T_CFTR_D Data;
	BOOL bExist=m_cftr.Lookup(Key, Data);
	if(bExist)
	{
		if(pGrup != NULL) pGrup->DelListItem(Data.GroupKey, LT_CFTR_CMD, Key);
	}
	m_cftr.SetAt(Key,rData);
	if(pGrup != NULL) pGrup->AddListItem(Data.GroupKey, LT_CFTR_CMD, Key);

	if(Key == m_nStartNum)
	{
		T_CFTR_D data;
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
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_CFTR_CMD, Key);
	}
}

BOOL CDB_CFTR::Del(T_CFTR_K Key,CDB_GRUP* pGrup)
{
	T_CFTR_D Data;
	m_cftr.Lookup(Key, Data);
	BOOL ret=m_cftr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(pGrup != NULL) pGrup->DelListItem(Data.GroupKey, LT_CFTR_CMD, Key);
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_CFTR_K key;
			T_CFTR_D data;
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
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_CFTR_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CFTR::Get(T_CFTR_K Key,T_CFTR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_cftr->Lookup(Key, uTemp)) return FALSE;
	}
	return m_cftr.Lookup(Key,rData);
}

int CDB_CFTR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag())
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cftr->GetCount();
	}
	return m_cftr.GetCount();
}

POSITION CDB_CFTR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cftr->GetStartPosition();
	}
	return m_cftr.GetStartPosition();
}

void CDB_CFTR::GetNext(POSITION& rNextPosition,T_CFTR_K& rKey,T_CFTR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_cftr->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_cftr.Lookup(rKey, rData);
		return;
	}
	m_cftr.GetNextAssoc(rNextPosition,rKey,rData);
}
