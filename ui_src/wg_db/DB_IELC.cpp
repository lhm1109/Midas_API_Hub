#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_STLD.h"
#include "DB_IELC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_IELC::CDB_IELC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ielc.InitHashTable(HASHSIZEIELC);
}

CDB_IELC::~CDB_IELC()
{

}

void CDB_IELC::Add(T_IELC_K Key,T_IELC_D& rData,CDB_ELEM* pElem, CDB_STLD* pStld)
{
	T_IELC_D Data;
	BOOL bExist=m_ielc.Lookup(Key.keymap, Data);
	
	m_ielc.SetAt(Key.keymap,rData);
	if(!bExist)
	{
		if (pElem != NULL) pElem->AddListItem(Key.key.entity, LT_IELC_CMD, Key.keymap);
		if (pStld != NULL) pStld->AddListItem(Key.key.stldid, LT_IELC_CMD, Key.keymap);
	}
}

BOOL CDB_IELC::Del(T_IELC_K Key,CDB_ELEM* pElem, CDB_STLD* pStld)
{
	T_IELC_D Data;
	BOOL bExist=m_ielc.Lookup(Key.keymap, Data);
	if(!bExist) return FALSE;

	BOOL ret=m_ielc.RemoveKey(Key.keymap);
	ASSERT(ret);
	if (ret)
	{
		if(pElem != NULL)VERIFY(pElem->DelListItem(Key.key.entity, LT_IELC_CMD, Key.keymap));
		if(pStld != NULL)VERIFY(pStld->DelListItem(Key.key.stldid, LT_IELC_CMD, Key.keymap));
	}
	return ret;
}


//-------------------------------------------------------------------------
BOOL CDB_IELC::Get(T_IELC_K Key,T_IELC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_ielc.Lookup(Key.keymap,rData);
}

int CDB_IELC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ielc.GetCount();
}

POSITION CDB_IELC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ielc.GetStartPosition();
}

void CDB_IELC::GetNext(POSITION& rNextPosition,T_IELC_K& rKey,T_IELC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_ielc.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
