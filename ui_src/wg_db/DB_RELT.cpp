#include "stdafx.h"
#include "DB_RELT.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RELT::CDB_RELT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_RELT.InitHashTable(HASHSIZERELT);
}

CDB_RELT::~CDB_RELT()
{
}

void CDB_RELT::Add(T_RELT_K Key,T_RELT_D& rData, CDB_BNGR* pBngr)
{
	T_RELT_D Data;
	BOOL bExist = m_RELT.Lookup(Key, Data);
	if (bExist)
	{
		if (pBngr) pBngr->DelListItem(Data.GroupKey, LT_RELT_CMD, Key);
	}

	m_RELT.SetAt(Key,rData);

	if (pBngr) pBngr->AddListItem(rData.GroupKey, LT_RELT_CMD, Key);
	if(Key == m_nStartNum)
	{
		T_RELT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_RELT_CMD, Key, rData.GroupKey);
	}

}

BOOL CDB_RELT::Del(T_RELT_K Key, CDB_BNGR* pBngr)
{
	T_RELT_D Data;
	BOOL bExist = m_RELT.Lookup(Key, Data);
	if (bExist)
	{
		BOOL ret = m_RELT.RemoveKey(Key);
		ASSERT(ret);
		if (ret)
		{
			if (pBngr) pBngr->DelListItem(Data.GroupKey, LT_RELT_CMD, Key);

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveBndr(LT_RELT_CMD, Key);
			}
		}

		if (Key < m_nStartNum)m_nStartNum = Key;
		if (Key == m_nLastNum)
		{
			T_RELT_K key;
			T_RELT_D data;
			if (Get(m_nLastNum - 1, data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum = 0;
				POSITION pos = GetStart();
				while (pos != NULL)
				{
					GetNext(pos, key, data);
					if (key > m_nLastNum)m_nLastNum = key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RELT::Get(T_RELT_K Key,T_RELT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_relt->Lookup(Key, StagBngrKey)) return FALSE;
	}
	return m_RELT.Lookup(Key,rData);
}

int CDB_RELT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_relt->GetCount();
	}
	return m_RELT.GetCount();
}

POSITION CDB_RELT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_relt->GetStartPosition();
	}
	return m_RELT.GetStartPosition();
}

void CDB_RELT::GetNext(POSITION& rNextPosition,T_RELT_K& rKey,T_RELT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_relt->GetNextAssoc(rNextPosition, rKey, StagBngrKey);
		m_RELT.Lookup(rKey, rData);
		return;
	}
	m_RELT.GetNextAssoc(rNextPosition,rKey,rData);
}