#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_IMPF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_IMPF::CDB_IMPF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_impf.InitHashTable(HASHSIZEIMPF);
}

CDB_IMPF::~CDB_IMPF()
{

}

void CDB_IMPF::Add(T_IMPF_K Key,T_IMPF_D& rData,CDB_ELEM* pElem)
{
	T_IMPF_D Data;
	BOOL bExist=m_impf.Lookup(Key.keymap, Data);
	m_impf.SetAt(Key.keymap,rData);
	if(!bExist)
	{
		if(pElem) pElem->AddListItem(Key.key.entity, LT_IMPF_CMD, Key.keymap);
	}
	
	// Final Stage에서 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_IMPF_CMD, Key.keymap);
	}
}

BOOL CDB_IMPF::Del(T_IMPF_K Key,CDB_ELEM* pElem)
{
	T_IMPF_D Data;
	m_impf.Lookup(Key.keymap, Data);
	BOOL ret=m_impf.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if(pElem) pElem->DelListItem(Key.key.entity, LT_IMPF_CMD, Key.keymap);
		// Final Stage에서 Add/Del/Mod 될 수 있으므로
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
		{
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_IMPF_CMD, Key.keymap);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_IMPF::Get(T_IMPF_K Key,T_IMPF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_impf->Lookup(Key.keymap, uTemp)) return FALSE;
	}
	return m_impf.Lookup(Key.keymap,rData);
}

int CDB_IMPF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_impf->GetCount();
	}
	return m_impf.GetCount();
}

POSITION CDB_IMPF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_impf->GetStartPosition();
	}
	return m_impf.GetStartPosition();
}

void CDB_IMPF::GetNext(POSITION& rNextPosition,T_IMPF_K& rKey,T_IMPF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_impf->GetNextAssoc(rNextPosition, rKey.keymap, uTemp);
		m_impf.Lookup(rKey.keymap, rData);
		return;
	}
	m_impf.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
