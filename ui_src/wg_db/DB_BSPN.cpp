#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_BSPN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BSPN::CDB_BSPN()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nStartNum=1;
	m_nLastNum=0;
	m_bspn.InitHashTable(HASHSIZEBSPN);
}

CDB_BSPN::~CDB_BSPN()
{
}

void CDB_BSPN::Add(T_BSPN_K Key,T_BSPN_D& rData)
{
	T_BSPN_D Data;
	BOOL bExist=m_bspn.Lookup(Key, Data);
	m_bspn.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_BSPN_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Final Stage Mode에서도 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_BSPN_CMD, Key);
	}
}

BOOL CDB_BSPN::Del(T_BSPN_K Key)
{
	T_BSPN_D Data;
	BOOL bExist=m_bspn.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_bspn.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_BSPN_K key;
				T_BSPN_D data;
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
		}
		// Final Stage Mode에서도 Add/Del/Mod 될 수 있으므로
		// Current Stage가 Final Base Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && m_pDoc->m_pStagCtrl->IsFinalStag())
		{
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_BSPN_CMD, Key);
		}
		return ret;
	}
	return FALSE;
}

//--------------------------------------------------------------------------
BOOL CDB_BSPN::Get(T_BSPN_K Key,T_BSPN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_bspn->Lookup(Key, uTemp)) return FALSE;
	}
	return m_bspn.Lookup(Key,rData);
}

int CDB_BSPN::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_bspn->GetCount();
	}
	return m_bspn.GetCount();
}

POSITION CDB_BSPN::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_bspn->GetStartPosition();
	}
	return m_bspn.GetStartPosition();
}

void CDB_BSPN::GetNext(POSITION& rNextPosition,T_BSPN_K& rKey,T_BSPN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_bspn->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_bspn.Lookup(rKey, rData);
		return;
	}
	m_bspn.GetNextAssoc(rNextPosition,rKey,rData);
}
