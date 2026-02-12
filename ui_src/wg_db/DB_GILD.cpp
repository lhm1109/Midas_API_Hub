#include "stdafx.h"
#include "DB_GILD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GILD::CDB_GILD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_gild.InitHashTable(HASHSIZEGILD);
}

CDB_GILD::~CDB_GILD()
{

}

void CDB_GILD::Add(T_GILD_K Key,T_GILD_D& rData)
{
	T_GILD_D Data;
	BOOL bExist=m_gild.Lookup(Key, Data);
	m_gild.SetAt(Key,rData);

	// Final Stage Mode에서도 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_GILD_CMD, Key);
	}
}

BOOL CDB_GILD::Del(T_GILD_K Key)
{
	T_GILD_D Data;
	BOOL bExist=m_gild.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_gild.RemoveKey(Key);
		ASSERT(ret);

		// Final Stage Mode에서도 Add/Del/Mod 될 수 있으므로
		// Current Stage가 Final Base Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && m_pDoc->m_pStagCtrl->IsFinalStag())
		{
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_GILD_CMD, Key);
		}
		return ret;
	}
	return bExist;
}


//--------------------------------------------------------------------------
BOOL CDB_GILD::Get(T_GILD_K Key,T_GILD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_gild->Lookup(Key, uTemp)) return FALSE;
	}
	return m_gild.Lookup(Key,rData);
}

int CDB_GILD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gild->GetCount();
	}
	return m_gild.GetCount();
}

POSITION CDB_GILD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gild->GetStartPosition();
	}
	return m_gild.GetStartPosition();
}

void CDB_GILD::GetNext(POSITION& rNextPosition,T_GILD_K& rKey,T_GILD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_gild->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_gild.Lookup(rKey, rData);
		return;
	}
	m_gild.GetNextAssoc(rNextPosition,rKey,rData);
}
