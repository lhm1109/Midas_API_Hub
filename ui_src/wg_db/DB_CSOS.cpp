#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CSOS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CSOS::CDB_CSOS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_csos.InitHashTable(HASHSIZECSOS);
}

CDB_CSOS::~CDB_CSOS()
{

}

void CDB_CSOS::Add(T_CSOS_K Key,T_CSOS_D& rData,CDB_ELEM* pElem)
{
	T_CSOS_D Data;
	BOOL bExist=m_csos.Lookup(Key, Data);
	m_csos.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CSOS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CSOS_CMD, Key);
	}
}

BOOL CDB_CSOS::Del(T_CSOS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_csos.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CSOS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CSOS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CSOS::Get(T_CSOS_K Key,T_CSOS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_csos->Lookup(Key, uTemp)) return FALSE;
	}
	return m_csos.Lookup(Key,rData);
}

int CDB_CSOS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_csos->GetCount();
	}
	return m_csos.GetCount();
}

POSITION CDB_CSOS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_csos->GetStartPosition();
	}
	return m_csos.GetStartPosition();
}

void CDB_CSOS::GetNext(POSITION& rNextPosition,T_CSOS_K& rKey,T_CSOS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_csos->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_csos.Lookup(rKey, rData);
		return;
	}
	m_csos.GetNextAssoc(rNextPosition,rKey,rData);
}
