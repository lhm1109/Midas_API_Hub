#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_ELUF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ELUF::CDB_ELUF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_eluf.InitHashTable(HASHSIZEELUF);
}

CDB_ELUF::~CDB_ELUF()
{

}

void CDB_ELUF::Add(T_ELUF_K Key,T_ELUF_D& rData,CDB_ELEM* pElem)
{
	T_ELUF_D Data;
	BOOL bExist=m_eluf.Lookup(Key, Data);
	m_eluf.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_ELUF_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_ELUF_CMD, Key);
	}
}

BOOL CDB_ELUF::Del(T_ELUF_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_eluf.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_ELUF_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_ELUF_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_ELUF::Get(T_ELUF_K Key,T_ELUF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_eluf->Lookup(Key, uTemp)) return FALSE;
	}
	return m_eluf.Lookup(Key,rData);
}

int CDB_ELUF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_eluf->GetCount();
	}
	return m_eluf.GetCount();
}

POSITION CDB_ELUF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_eluf->GetStartPosition();
	}
	return m_eluf.GetStartPosition();
}

void CDB_ELUF::GetNext(POSITION& rNextPosition,T_ELUF_K& rKey,T_ELUF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_eluf->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_eluf.Lookup(rKey, rData);
		return;
	}
	m_eluf.GetNextAssoc(rNextPosition,rKey,rData);
}
