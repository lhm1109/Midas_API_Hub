#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PHGE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PHGE::CDB_PHGE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_phge.InitHashTable(HASHSIZEPHGE);
}

CDB_PHGE::~CDB_PHGE()
{

}

void CDB_PHGE::Add(T_PHGE_K Key,T_PHGE_D& rData,CDB_ELEM* pElem)
{
	T_PHGE_D Data;
	BOOL bExist=m_phge.Lookup(Key.keymap, Data);
	m_phge.SetAt(Key.keymap,rData);
	if(!bExist)
	{
		if(Key.key.type!=D_PHGE_GLINK && Key.key.type!=D_PHGE_PSPR)
			if(pElem) pElem->AddListItem(Key.key.objK, LT_PHGE_CMD, Key.keymap);
	}
	
	// Final Stage에서 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_PHGE_CMD, Key.keymap);
	}
}

BOOL CDB_PHGE::Del(T_PHGE_K Key,CDB_ELEM* pElem)
{
	T_PHGE_D Data;
	m_phge.Lookup(Key.keymap, Data);
	BOOL ret=m_phge.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if(Key.key.type!=D_PHGE_GLINK && Key.key.type!=D_PHGE_PSPR)
			if(pElem) pElem->DelListItem(Key.key.objK, LT_PHGE_CMD, Key.keymap);
		// Final Stage에서 Add/Del/Mod 될 수 있으므로
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
		{
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_PHGE_CMD, Key.keymap);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PHGE::Get(T_PHGE_K Key,T_PHGE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_phge->Lookup(Key.keymap, uTemp)) return FALSE;
	}
	return m_phge.Lookup(Key.keymap,rData);
}

int CDB_PHGE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_phge->GetCount();
	}
	return m_phge.GetCount();
}

POSITION CDB_PHGE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_phge->GetStartPosition();
	}
	return m_phge.GetStartPosition();
}

void CDB_PHGE::GetNext(POSITION& rNextPosition,T_PHGE_K& rKey,T_PHGE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_phge->GetNextAssoc(rNextPosition, rKey.keymap, uTemp);
		m_phge.Lookup(rKey.keymap, rData);
		return;
	}
	m_phge.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
