#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_DFBA.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DFBA::CDB_DFBA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dfba.InitHashTable(HASHSIZEDFBA);
}

CDB_DFBA::~CDB_DFBA()
{

}

void CDB_DFBA::Add(T_DFBA_K Key,T_DFBA_D& rData,CDB_ELEM* pElem)
{
	T_DFBA_D Data;
	BOOL bExist=m_dfba.Lookup(Key, Data);
	m_dfba.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_DFBA_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_DFBA_CMD, Key);
	}
}

BOOL CDB_DFBA::Del(T_DFBA_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_dfba.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_DFBA_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_DFBA_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DFBA::Get(T_DFBA_K Key,T_DFBA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_dfba->Lookup(Key, uTemp)) return FALSE;
	}
	return m_dfba.Lookup(Key,rData);
}

int CDB_DFBA::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_dfba->GetCount();
	}
	return m_dfba.GetCount();
}

POSITION CDB_DFBA::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_dfba->GetStartPosition();
	}
	return m_dfba.GetStartPosition();
}

void CDB_DFBA::GetNext(POSITION& rNextPosition,T_DFBA_K& rKey,T_DFBA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_dfba->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_dfba.Lookup(rKey, rData);
		return;
	}
	m_dfba.GetNextAssoc(rNextPosition,rKey,rData);
}

