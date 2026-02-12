#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_ULEC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ULEC::CDB_ULEC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ulec.InitHashTable(HASHSIZEULEC);
}

CDB_ULEC::~CDB_ULEC()
{

}

void CDB_ULEC::Add(T_ULEC_K Key,T_ULEC_D& rData,CDB_ELEM* pElem)
{
	T_ULEC_D Data;
	BOOL bExist=m_ulec.Lookup(Key, Data);
	m_ulec.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_ULEC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_ULEC_CMD, Key);
	}
}

BOOL CDB_ULEC::Del(T_ULEC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_ulec.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_ULEC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_ULEC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_ULEC::Get(T_ULEC_K Key,T_ULEC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_ulec->Lookup(Key, uTemp)) return FALSE;
	}
	return m_ulec.Lookup(Key,rData);
}

int CDB_ULEC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ulec->GetCount();
	}
	return m_ulec.GetCount();
}

POSITION CDB_ULEC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ulec->GetStartPosition();
	}
	return m_ulec.GetStartPosition();
}

void CDB_ULEC::GetNext(POSITION& rNextPosition,T_ULEC_K& rKey,T_ULEC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_ulec->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_ulec.Lookup(rKey, rData);
		return;
	}
	m_ulec.GetNextAssoc(rNextPosition,rKey,rData);
}

