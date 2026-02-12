#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SDGN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SDGN::CDB_SDGN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sdgn.InitHashTable(HASHSIZESDGN);
}

CDB_SDGN::~CDB_SDGN()
{

}

void CDB_SDGN::Add(T_SDGN_K Key,T_SDGN_D& rData,CDB_ELEM* pElem)
{
	T_SDGN_D Data;
	BOOL bExist=m_sdgn.Lookup(Key, Data);
	m_sdgn.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SDGN_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SDGN_CMD, Key);
	}
}

BOOL CDB_SDGN::Del(T_SDGN_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_sdgn.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SDGN_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SDGN_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SDGN::Get(T_SDGN_K Key,T_SDGN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sdgn->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sdgn.Lookup(Key,rData);
}

int CDB_SDGN::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdgn->GetCount();
	}
	return m_sdgn.GetCount();
}

POSITION CDB_SDGN::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdgn->GetStartPosition();
	}
	return m_sdgn.GetStartPosition();
}

void CDB_SDGN::GetNext(POSITION& rNextPosition,T_SDGN_K& rKey,T_SDGN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sdgn->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sdgn.Lookup(rKey, rData);
		return;
	}
	m_sdgn.GetNextAssoc(rNextPosition,rKey,rData);
}