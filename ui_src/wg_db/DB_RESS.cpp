#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RESS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RESS::CDB_RESS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ress.InitHashTable(HASHSIZERESS);
}

CDB_RESS::~CDB_RESS()
{

}

void CDB_RESS::Add(T_RESS_K Key,T_RESS_D& rData,CDB_ELEM* pElem)
{
	T_RESS_D Data;
	BOOL bExist=m_ress.Lookup(Key, Data);
	m_ress.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RESS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RESS_CMD, Key);
	}
}

BOOL CDB_RESS::Del(T_RESS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_ress.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RESS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RESS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RESS::Get(T_RESS_K Key,T_RESS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_ress->Lookup(Key, uTemp)) return FALSE;
	}
	return m_ress.Lookup(Key,rData);
}

int CDB_RESS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ress->GetCount();
	}
	return m_ress.GetCount();
}

POSITION CDB_RESS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ress->GetStartPosition();
	}
	return m_ress.GetStartPosition();
}

void CDB_RESS::GetNext(POSITION& rNextPosition,T_RESS_K& rKey,T_RESS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_ress->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_ress.Lookup(rKey, rData);
		return;
	}
	m_ress.GetNextAssoc(rNextPosition,rKey,rData);
}

