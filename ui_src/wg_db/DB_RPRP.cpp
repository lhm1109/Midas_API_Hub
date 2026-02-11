#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RPRP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RPRP::CDB_RPRP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rprp.InitHashTable(HASHSIZERPRP);
}

CDB_RPRP::~CDB_RPRP()
{

}

void CDB_RPRP::Add(T_RPRP_K Key,T_RPRP_D& rData,CDB_ELEM* pElem)
{
	T_RPRP_D Data;
	BOOL bExist=m_rprp.Lookup(Key, Data);
	m_rprp.SetAt(Key,rData);
	//if(!bExist)pElem->AddListItem(Key, LT_RPRP_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RPRP_CMD, Key);
	}
}

BOOL CDB_RPRP::Del(T_RPRP_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rprp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		//VERIFY(pElem->DelListItem(Key, LT_RPRP_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RPRP_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RPRP::Get(T_RPRP_K Key,T_RPRP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rprs->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rprp.Lookup(Key,rData);
}

int CDB_RPRP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rprp->GetCount();
	}
	return m_rprp.GetCount();
}

POSITION CDB_RPRP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rprp->GetStartPosition();
	}
	return m_rprp.GetStartPosition();
}

void CDB_RPRP::GetNext(POSITION& rNextPosition,T_RPRP_K& rKey,T_RPRP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rprp->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rprp.Lookup(rKey, rData);
		return;
	}
	m_rprp.GetNextAssoc(rNextPosition,rKey,rData);
}

