#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_DALW.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DALW::CDB_DALW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dalw.InitHashTable(HASHSIZEDALW);
}

CDB_DALW::~CDB_DALW()
{

}

void CDB_DALW::Add(T_DALW_K Key,T_DALW_D& rData,CDB_ELEM* pElem)
{
	T_DALW_D Data;
	BOOL bExist=m_dalw.Lookup(Key, Data);
	m_dalw.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_DALW_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_DALW_CMD, Key);
	}
}

BOOL CDB_DALW::Del(T_DALW_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_dalw.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_DALW_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_DALW_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DALW::Get(T_DALW_K Key,T_DALW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_dalw->Lookup(Key, uTemp)) return FALSE;
	}
	return m_dalw.Lookup(Key,rData);
}

int CDB_DALW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_dalw->GetCount();
	}
	return m_dalw.GetCount();
}

POSITION CDB_DALW::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_dalw->GetStartPosition();
	}
	return m_dalw.GetStartPosition();
}

void CDB_DALW::GetNext(POSITION& rNextPosition,T_DALW_K& rKey,T_DALW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_dalw->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_dalw.Lookup(rKey, rData);
		return;
	}
	m_dalw.GetNextAssoc(rNextPosition,rKey,rData);
}
