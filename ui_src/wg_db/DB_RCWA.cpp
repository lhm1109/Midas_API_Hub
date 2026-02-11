#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RCWA.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RCWA::CDB_RCWA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rcwa.InitHashTable(HASHSIZERCWA);
}

CDB_RCWA::~CDB_RCWA()
{

}

void CDB_RCWA::Add(T_RCWA_K Key,T_RCWA_D& rData,CDB_ELEM* pElem)
{
	T_RCWA_D Data;
	BOOL bExist=m_rcwa.Lookup(Key, Data);
	m_rcwa.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RCWA_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RCWA_CMD, Key);
	}
}

BOOL CDB_RCWA::Del(T_RCWA_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rcwa.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RCWA_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RCWA_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RCWA::Get(T_RCWA_K Key,T_RCWA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rcwa->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rcwa.Lookup(Key,rData);
}

int CDB_RCWA::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rcwa->GetCount();
	}
	return m_rcwa.GetCount();
}

POSITION CDB_RCWA::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rcwa->GetStartPosition();
	}
	return m_rcwa.GetStartPosition();
}

void CDB_RCWA::GetNext(POSITION& rNextPosition,T_RCWA_K& rKey,T_RCWA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rcwa->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rcwa.Lookup(rKey, rData);
		return;
	}
	m_rcwa.GetNextAssoc(rNextPosition,rKey,rData);
}
