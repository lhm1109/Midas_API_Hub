#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PSAS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PSAS::CDB_PSAS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_psas.InitHashTable(HASHSIZEPSAS);
}

CDB_PSAS::~CDB_PSAS()
{

}

void CDB_PSAS::Add(T_PSAS_K Key,T_PSAS_D& rData,CDB_ELEM* pElem)
{
	T_PSAS_D Data;
	BOOL bExist=m_psas.Lookup(Key, Data);
	m_psas.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PSAS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PSAS_CMD, Key);
	}
}

BOOL CDB_PSAS::Del(T_PSAS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_psas.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PSAS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PSAS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PSAS::Get(T_PSAS_K Key,T_PSAS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_psas->Lookup(Key, uTemp)) return FALSE;
	}
	return m_psas.Lookup(Key,rData);
}

int CDB_PSAS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_psas->GetCount();
	}
	return m_psas.GetCount();
}

POSITION CDB_PSAS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_psas->GetStartPosition();
	}
	return m_psas.GetStartPosition();
}

void CDB_PSAS::GetNext(POSITION& rNextPosition,T_PSAS_K& rKey,T_PSAS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_psas->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_psas.Lookup(rKey, rData);
		return;
	}
	m_psas.GetNextAssoc(rNextPosition,rKey,rData);
}
