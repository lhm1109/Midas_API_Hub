#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PFNP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PFNP::CDB_PFNP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_PFNP.InitHashTable(HASHSIZEPFNP);
}

CDB_PFNP::~CDB_PFNP()
{

}

void CDB_PFNP::Add(T_PFNP_K Key,T_PFNP_D& rData,CDB_ELEM* pElem)
{
	T_PFNP_D Data;
	BOOL bExist=m_PFNP.Lookup(Key, Data);
	m_PFNP.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PFNP_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PFNP_CMD, Key);
	}
}

BOOL CDB_PFNP::Del(T_PFNP_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_PFNP.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PFNP_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PFNP_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PFNP::Get(T_PFNP_K Key,T_PFNP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_pfnp->Lookup(Key, uTemp)) return FALSE;
	}
	return m_PFNP.Lookup(Key,rData);
}

int CDB_PFNP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pfnp->GetCount();
	}
	return m_PFNP.GetCount();
}

POSITION CDB_PFNP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pfnp->GetStartPosition();
	}
	return m_PFNP.GetStartPosition();
}

void CDB_PFNP::GetNext(POSITION& rNextPosition,T_PFNP_K& rKey,T_PFNP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_pfnp->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_PFNP.Lookup(rKey, rData);
		return;
	}
	m_PFNP.GetNextAssoc(rNextPosition,rKey,rData);
}
