#include "stdafx.h" 
#include "DB_ELEM.h"
#include "DB_MLTT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MLTT::CDB_MLTT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mltt.InitHashTable(HASHSIZEMLTT);
}

CDB_MLTT::~CDB_MLTT()
{

}

void CDB_MLTT::Add(T_MLTT_K Key,T_MLTT_D& rData,CDB_ELEM* pElem)
{
	T_MLTT_D Data;
	BOOL bExist=m_mltt.Lookup(Key.keymap, Data);
	m_mltt.SetAt(Key.keymap,rData);
	if(!bExist && (Key.key.serial == 0 || Key.key.serial == 1 || Key.key.serial == 2))
		pElem->AddListItem(Key.key.entity, LT_MLTT_CMD, Key.keymap);
}

BOOL CDB_MLTT::Del(T_MLTT_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_mltt.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret && (Key.key.serial == 0 || Key.key.serial == 1 || Key.key.serial == 2))
		VERIFY(pElem->DelListItem(Key.key.entity, LT_MLTT_CMD, Key.keymap));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_MLTT::Get(T_MLTT_K Key,T_MLTT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mltt->Lookup(Key.keymap, uTemp)) return FALSE;
	}
	return m_mltt.Lookup(Key.keymap,rData);
}

int CDB_MLTT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mltt->GetCount();
	}
	return m_mltt.GetCount();
}

POSITION CDB_MLTT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mltt->GetStartPosition();
	}
	return m_mltt.GetStartPosition();
}

void CDB_MLTT::GetNext(POSITION& rNextPosition,T_MLTT_K& rKey,T_MLTT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mltt->GetNextAssoc(rNextPosition, rKey.keymap, uTemp);
		m_mltt.Lookup(rKey.keymap, rData);
		return;
	}
	m_mltt.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
