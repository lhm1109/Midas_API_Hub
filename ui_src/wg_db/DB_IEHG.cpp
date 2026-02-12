#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_IEHG.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_IEHG::CDB_IEHG()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_iehg.InitHashTable(HASHSIZEIEHG);
}

CDB_IEHG::~CDB_IEHG()
{

}

void CDB_IEHG::Add(T_IEHG_K Key,T_IEHG_D& rData,CDB_ELEM* pElem)
{
	T_IEHG_D Data;
	BOOL bExist=m_iehg.Lookup(Key, Data);
	m_iehg.SetAt(Key,rData);
	if(!bExist)
	{
		if(pElem) pElem->AddListItem(Key.key.objK, LT_IEHG_CMD, Key);
	}
}

BOOL CDB_IEHG::Del(T_IEHG_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_iehg.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(pElem) pElem->DelListItem(Key.key.objK, LT_IEHG_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_IEHG::Get(T_IEHG_K Key,T_IEHG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_iehg->Lookup(Key, uTemp)) return FALSE;
	}
	return m_iehg.Lookup(Key,rData);
}

int CDB_IEHG::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_iehg->GetCount();
	}
	return m_iehg.GetCount();
}

POSITION CDB_IEHG::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_iehg->GetStartPosition();
	}
	return m_iehg.GetStartPosition();
}

void CDB_IEHG::GetNext(POSITION& rNextPosition,T_IEHG_K& rKey,T_IEHG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_iehg->GetNextAssoc(rNextPosition, rKey.keymap, uTemp);
		m_iehg.Lookup(rKey.keymap, rData);
		return;
	}
	m_iehg.GetNextAssoc(rNextPosition, rKey.keymap, rData);
}
