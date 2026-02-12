#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SDLF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SDLF::CDB_SDLF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sdlf.InitHashTable(HASHSIZESDLF);
}

CDB_SDLF::~CDB_SDLF()
{

}

void CDB_SDLF::Add(T_SDLF_K Key,T_SDLF_D& rData,CDB_ELEM* pElem)
{
	T_SDLF_D Data;
	BOOL bExist=m_sdlf.Lookup(Key, Data);
	m_sdlf.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SDLF_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SDLF_CMD, Key);
	}
}

BOOL CDB_SDLF::Del(T_SDLF_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_sdlf.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SDLF_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SDLF_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SDLF::Get(T_SDLF_K Key,T_SDLF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sdlf->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sdlf.Lookup(Key,rData);
}

int CDB_SDLF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdlf->GetCount();
	}
	return m_sdlf.GetCount();
}

POSITION CDB_SDLF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdlf->GetStartPosition();
	}
	return m_sdlf.GetStartPosition();
}

void CDB_SDLF::GetNext(POSITION& rNextPosition,T_SDLF_K& rKey,T_SDLF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sdlf->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sdlf.Lookup(rKey, rData);
		return;
	}
	m_sdlf.GetNextAssoc(rNextPosition,rKey,rData);
}

