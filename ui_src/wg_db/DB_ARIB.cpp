#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_ARIB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ARIB::CDB_ARIB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_arib.InitHashTable(HASHSIZEARIB);
}

CDB_ARIB::~CDB_ARIB()
{

}

void CDB_ARIB::Add(T_ARIB_K Key,T_ARIB_D& rData,CDB_ELEM* pElem)
{
	T_ARIB_D Data;
	BOOL bExist=m_arib.Lookup(Key, Data);
	m_arib.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_ARIB_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_ARIB_CMD, Key);
	}
}

BOOL CDB_ARIB::Del(T_ARIB_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_arib.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_ARIB_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_ARIB_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_ARIB::Get(T_ARIB_K Key,T_ARIB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_arib->Lookup(Key, uTemp)) return FALSE;
	}
	return m_arib.Lookup(Key,rData);
}

int CDB_ARIB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_arib->GetCount();
	}
	return m_arib.GetCount();
}

POSITION CDB_ARIB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_arib->GetStartPosition();
	}
	return m_arib.GetStartPosition();
}

void CDB_ARIB::GetNext(POSITION& rNextPosition,T_ARIB_K& rKey,T_ARIB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_arib->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_arib.Lookup(rKey, rData);
		return;
	}
	m_arib.GetNextAssoc(rNextPosition,rKey,rData);
}
