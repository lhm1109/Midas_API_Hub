#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RROS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RROS::CDB_RROS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rros.InitHashTable(HASHSIZERROS);
}

CDB_RROS::~CDB_RROS()
{

}

void CDB_RROS::Add(T_RROS_K Key,T_RROS_D& rData,CDB_ELEM* pElem)
{
	T_RROS_D Data;
	BOOL bExist=m_rros.Lookup(Key, Data);
	m_rros.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RROS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RROS_CMD, Key);
	}
}

BOOL CDB_RROS::Del(T_RROS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rros.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RROS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RROS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RROS::Get(T_RROS_K Key,T_RROS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rros->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rros.Lookup(Key,rData);
}

int CDB_RROS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rros->GetCount();
	}
	return m_rros.GetCount();
}

POSITION CDB_RROS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rros->GetStartPosition();
	}
	return m_rros.GetStartPosition();
}

void CDB_RROS::GetNext(POSITION& rNextPosition,T_RROS_K& rKey,T_RROS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rros->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rros.Lookup(rKey, rData);
		return;
	}
	m_rros.GetNextAssoc(rNextPosition,rKey,rData);
}

