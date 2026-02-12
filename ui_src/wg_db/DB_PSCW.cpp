#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PSCW.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PSCW::CDB_PSCW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pscw.InitHashTable(HASHSIZEPSCW);
}

CDB_PSCW::~CDB_PSCW()
{

}

void CDB_PSCW::Add(T_PSCW_K Key,T_PSCW_D& rData,CDB_ELEM* pElem)
{
	T_PSCW_D Data;
	BOOL bExist=m_pscw.Lookup(Key, Data);
	m_pscw.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PSCW_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PSCW_CMD, Key);
	}
}

BOOL CDB_PSCW::Del(T_PSCW_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_pscw.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PSCW_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PSCW_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PSCW::Get(T_PSCW_K Key,T_PSCW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_pscw->Lookup(Key, uTemp)) return FALSE;
	}
	return m_pscw.Lookup(Key,rData);
}

int CDB_PSCW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pscw->GetCount();
	}
	return m_pscw.GetCount();
}

POSITION CDB_PSCW::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pscw->GetStartPosition();
	}
	return m_pscw.GetStartPosition();
}

void CDB_PSCW::GetNext(POSITION& rNextPosition,T_PSCW_K& rKey,T_PSCW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_pscw->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_pscw.Lookup(rKey, rData);
		return;
	}
	m_pscw.GetNextAssoc(rNextPosition,rKey,rData);
}

