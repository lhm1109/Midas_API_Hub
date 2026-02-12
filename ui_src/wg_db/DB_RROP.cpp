#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RROP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RROP::CDB_RROP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rrop.InitHashTable(HASHSIZERROP);
}

CDB_RROP::~CDB_RROP()
{

}

void CDB_RROP::Add(T_RROP_K Key,T_RROP_D& rData,CDB_ELEM* pElem)
{
	T_RROP_D Data;
	BOOL bExist=m_rrop.Lookup(Key, Data);
	m_rrop.SetAt(Key,rData);
	//if(!bExist)pElem->AddListItem(Key, LT_RROP_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RROP_CMD, Key);
	}
}

BOOL CDB_RROP::Del(T_RROP_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rrop.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		//VERIFY(pElem->DelListItem(Key, LT_RROP_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RROP_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RROP::Get(T_RROP_K Key,T_RROP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rrop->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rrop.Lookup(Key,rData);
}

int CDB_RROP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rrop->GetCount();
	}
	return m_rrop.GetCount();
}

POSITION CDB_RROP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rrop->GetStartPosition();
	}
	return m_rrop.GetStartPosition();
}

void CDB_RROP::GetNext(POSITION& rNextPosition,T_RROP_K& rKey,T_RROP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rrop->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rrop.Lookup(rKey, rData);
		return;
	}
	m_rrop.GetNextAssoc(rNextPosition,rKey,rData);
}

