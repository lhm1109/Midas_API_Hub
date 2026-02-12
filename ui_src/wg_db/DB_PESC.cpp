#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PESC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PESC::CDB_PESC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pesc.InitHashTable(HASHSIZEPESC);
}

CDB_PESC::~CDB_PESC()
{

}

void CDB_PESC::Add(T_PESC_K Key,T_PESC_D& rData,CDB_ELEM* pElem)
{
	T_PESC_D Data;
	BOOL bExist=m_pesc.Lookup(Key, Data);
	m_pesc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PESC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PESC_CMD, Key);
	}
}

BOOL CDB_PESC::Del(T_PESC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_pesc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PESC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PESC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PESC::Get(T_PESC_K Key,T_PESC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_pesc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_pesc.Lookup(Key,rData);
}

int CDB_PESC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pesc->GetCount();
	}
	return m_pesc.GetCount();
}

POSITION CDB_PESC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pesc->GetStartPosition();
	}
	return m_pesc.GetStartPosition();
}

void CDB_PESC::GetNext(POSITION& rNextPosition,T_PESC_K& rKey,T_PESC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_pesc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_pesc.Lookup(rKey, rData);
		return;
	}
	m_pesc.GetNextAssoc(rNextPosition,rKey,rData);
}

