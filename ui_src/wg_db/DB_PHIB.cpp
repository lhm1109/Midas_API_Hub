#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PHIB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PHIB::CDB_PHIB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_PHIB.InitHashTable(HASHSIZEPHIB);
}

CDB_PHIB::~CDB_PHIB()
{

}

void CDB_PHIB::Add(T_PHIB_K Key,T_PHIB_D& rData,CDB_ELEM* pElem)
{
	T_PHIB_D Data;
	BOOL bExist=m_PHIB.Lookup(Key, Data);
	m_PHIB.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PHIB_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PHIB_CMD, Key);
	}
}

BOOL CDB_PHIB::Del(T_PHIB_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_PHIB.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PHIB_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PHIB_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PHIB::Get(T_PHIB_K Key,T_PHIB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_phib->Lookup(Key, uTemp)) return FALSE;
	}
	return m_PHIB.Lookup(Key,rData);
}

int CDB_PHIB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_phib->GetCount();
	}
	return m_PHIB.GetCount();
}

POSITION CDB_PHIB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_phib->GetStartPosition();
	}
	return m_PHIB.GetStartPosition();
}

void CDB_PHIB::GetNext(POSITION& rNextPosition,T_PHIB_K& rKey,T_PHIB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_phib->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_PHIB.Lookup(rKey, rData);
		return;
	}
	m_PHIB.GetNextAssoc(rNextPosition,rKey,rData);
}
