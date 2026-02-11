#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RESC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RESC::CDB_RESC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_resc.InitHashTable(HASHSIZERESC);
}

CDB_RESC::~CDB_RESC()
{

}

void CDB_RESC::Add(T_RESC_K Key,T_RESC_D& rData,CDB_ELEM* pElem)
{
	T_RESC_D Data;
	BOOL bExist=m_resc.Lookup(Key, Data);
	m_resc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RESC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RESC_CMD, Key);
	}
}

BOOL CDB_RESC::Del(T_RESC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_resc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RESC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RESC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RESC::Get(T_RESC_K Key,T_RESC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_resc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_resc.Lookup(Key,rData);
}

int CDB_RESC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_resc->GetCount();
	}
	return m_resc.GetCount();
}

POSITION CDB_RESC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_resc->GetStartPosition();
	}
	return m_resc.GetStartPosition();
}

void CDB_RESC::GetNext(POSITION& rNextPosition,T_RESC_K& rKey,T_RESC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_resc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_resc.Lookup(rKey, rData);
		return;
	}
	m_resc.GetNextAssoc(rNextPosition,rKey,rData);
}

