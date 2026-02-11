#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_AWED.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_AWED::CDB_AWED()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_awed.InitHashTable(HASHSIZEAWED);
}

CDB_AWED::~CDB_AWED()
{

}

void CDB_AWED::Add(T_AWED_K Key,T_AWED_D& rData,CDB_ELEM* pElem)
{
	T_AWED_D Data;
	BOOL bExist=m_awed.Lookup(Key, Data);
	m_awed.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_AWED_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_AWED_CMD, Key);
	}
}

BOOL CDB_AWED::Del(T_AWED_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_awed.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_AWED_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_AWED_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_AWED::Get(T_AWED_K Key,T_AWED_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_awed->Lookup(Key, uTemp)) return FALSE;
	}
	return m_awed.Lookup(Key,rData);
}

int CDB_AWED::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_awed->GetCount();
	}
	return m_awed.GetCount();
}

POSITION CDB_AWED::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_awed->GetStartPosition();
	}
	return m_awed.GetStartPosition();
}

void CDB_AWED::GetNext(POSITION& rNextPosition,T_AWED_K& rKey,T_AWED_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_awed->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_awed.Lookup(rKey, rData);
		return;
	}
	m_awed.GetNextAssoc(rNextPosition,rKey,rData);
}
