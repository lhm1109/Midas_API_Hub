#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PFMP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PFMP::CDB_PFMP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_PFMP.InitHashTable(HASHSIZEPFMP);
}

CDB_PFMP::~CDB_PFMP()
{

}

void CDB_PFMP::Add(T_PFMP_K Key,T_PFMP_D& rData,CDB_ELEM* pElem)
{
	T_PFMP_D Data;
	BOOL bExist=m_PFMP.Lookup(Key, Data);
	m_PFMP.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PFMP_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PFMP_CMD, Key);
	}
}

BOOL CDB_PFMP::Del(T_PFMP_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_PFMP.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PFMP_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PFMP_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PFMP::Get(T_PFMP_K Key,T_PFMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_pfmp->Lookup(Key, uTemp)) return FALSE;
	}
	return m_PFMP.Lookup(Key,rData);
}

int CDB_PFMP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pfmp->GetCount();
	}
	return m_PFMP.GetCount();
}

POSITION CDB_PFMP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pfmp->GetStartPosition();
	}
	return m_PFMP.GetStartPosition();
}

void CDB_PFMP::GetNext(POSITION& rNextPosition,T_PFMP_K& rKey,T_PFMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_pfmp->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_PFMP.Lookup(rKey, rData);
		return;
	}
	m_PFMP.GetNextAssoc(rNextPosition,rKey,rData);
}
