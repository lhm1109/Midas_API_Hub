#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_STAN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STAN::CDB_STAN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_stan.InitHashTable(HASHSIZESTAN);
}

CDB_STAN::~CDB_STAN()
{

}

void CDB_STAN::Add(T_STAN_K Key,T_STAN_D& rData,CDB_ELEM* pElem)
{
	T_STAN_D Data;
	BOOL bExist=m_stan.Lookup(Key, Data);
	m_stan.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_STAN_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_STAN_CMD, Key);
	}
}

BOOL CDB_STAN::Del(T_STAN_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_stan.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_STAN_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_STAN_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_STAN::Get(T_STAN_K Key,T_STAN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_stan->Lookup(Key, uTemp)) return FALSE;
	}
	return m_stan.Lookup(Key,rData);
}

int CDB_STAN::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_stan->GetCount();
	}
	return m_stan.GetCount();
}

POSITION CDB_STAN::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_stan->GetStartPosition();
	}
	return m_stan.GetStartPosition();
}

void CDB_STAN::GetNext(POSITION& rNextPosition,T_STAN_K& rKey,T_STAN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_stan->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_stan.Lookup(rKey, rData);
		return;
	}
	m_stan.GetNextAssoc(rNextPosition,rKey,rData);
}

