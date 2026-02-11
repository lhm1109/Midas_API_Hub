#include "stdafx.h"
#include "DB_DLCM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DLCM::CDB_DLCM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	
	m_nStartNum=1;
	m_nLastNum=0;
	m_dlcm.InitHashTable(HASHSIZEDLCM);
}

CDB_DLCM::~CDB_DLCM()
{

}

void CDB_DLCM::Add(T_DLCM_K Key, T_DLCM_D& rData)
{
	T_DLCM_D Data;
	BOOL bExist = m_dlcm.Lookup(Key, Data);
	m_dlcm.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_DLCM_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
	
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_DLCM_CMD, Key);
	}
}

BOOL CDB_DLCM::Del(T_DLCM_K Key)
{
	T_DLCM_D Data;
	BOOL bExist=m_dlcm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_dlcm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_DLCM_K key;
				T_DLCM_D data;
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
			// Current Stage가 Final Stage이면 Stage DB를 Update한다.
			if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
				m_pDoc->m_pStagCtrl->DeactiveDesign(LT_DLCM_CMD, Key);
		}
		return ret;
	}
	return FALSE;
}

//-------------------------------------------------------------------------
BOOL CDB_DLCM::Get(T_DLCM_K Key, T_DLCM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_dlcm->Lookup(Key, uTemp)) return FALSE;
	}
	return m_dlcm.Lookup(Key,rData);
}

int CDB_DLCM::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_dlcm->GetCount();
	}
	return m_dlcm.GetCount();
}

POSITION CDB_DLCM::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_dlcm->GetStartPosition();
	}
	return m_dlcm.GetStartPosition();
}

void CDB_DLCM::GetNext(POSITION& rNextPosition,T_DLCM_K& rKey,T_DLCM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_dlcm->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_dlcm.Lookup(rKey, rData);
		return;
	}
	m_dlcm.GetNextAssoc(rNextPosition,rKey,rData);
}