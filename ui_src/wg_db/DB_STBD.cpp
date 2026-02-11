#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_STBD.h" 

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STBD::CDB_STBD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_stbd.InitHashTable(HASHSIZESTBD);
}

CDB_STBD::~CDB_STBD()
{

}

void CDB_STBD::Add(T_STBD_K Key,T_STBD_D& rData,CDB_ELEM* pElem)
{
	T_STBD_D Data;
	BOOL bExist=m_stbd.Lookup(Key, Data);
	m_stbd.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_STBD_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_STBD_CMD, Key);
	}
}

BOOL CDB_STBD::Del(T_STBD_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_stbd.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_STBD_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_STBD_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_STBD::Get(T_STBD_K Key,T_STBD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_stbd->Lookup(Key, uTemp)) return FALSE;
	}
	return m_stbd.Lookup(Key,rData);
}

int CDB_STBD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_stbd->GetCount();
	}
	return m_stbd.GetCount();
}

POSITION CDB_STBD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_stbd->GetStartPosition();
	}
	return m_stbd.GetStartPosition();
}

void CDB_STBD::GetNext(POSITION& rNextPosition,T_STBD_K& rKey,T_STBD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_stbd->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_stbd.Lookup(rKey, rData);
		return;
	}
	m_stbd.GetNextAssoc(rNextPosition,rKey,rData);
}
