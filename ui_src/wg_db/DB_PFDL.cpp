#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PFDL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PFDL::CDB_PFDL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_PFDL.InitHashTable(HASHSIZEPFDL);
}

CDB_PFDL::~CDB_PFDL()
{

}

void CDB_PFDL::Add(T_PFDL_K Key,T_PFDL_D& rData,CDB_ELEM* pElem)
{
	T_PFDL_D Data;
	BOOL bExist=m_PFDL.Lookup(Key, Data);
	m_PFDL.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PFDL_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PFDL_CMD, Key);
	}
}

BOOL CDB_PFDL::Del(T_PFDL_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_PFDL.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PFDL_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PFDL_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PFDL::Get(T_PFDL_K Key,T_PFDL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_pfdl->Lookup(Key, uTemp)) return FALSE;
	}
	return m_PFDL.Lookup(Key,rData);
}

int CDB_PFDL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pfdl->GetCount();
	}
	return m_PFDL.GetCount();
}

POSITION CDB_PFDL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pfdl->GetStartPosition();
	}
	return m_PFDL.GetStartPosition();
}

void CDB_PFDL::GetNext(POSITION& rNextPosition,T_PFDL_K& rKey,T_PFDL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_pfdl->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_PFDL.Lookup(rKey, rData);
		return;
	}
	m_PFDL.GetNextAssoc(rNextPosition,rKey,rData);
}
