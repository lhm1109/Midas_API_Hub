#include "stdafx.h"
#include "DB_STOR.h"
#include "DB_ELEM.h"
#include "DB_DMAS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DMAS::CDB_DMAS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dmas.InitHashTable(HASHSIZEDMAS);
}

CDB_DMAS::~CDB_DMAS()
{

}

void CDB_DMAS::Add(T_DMAS_K Key,T_DMAS_D& rData,CDB_STOR* pStor)
{
	T_DMAS_D Data;
	BOOL bExist=m_dmas.Lookup(Key.keymap, Data);
	m_dmas.SetAt(Key.keymap,rData);
	if(!bExist)pStor->AddListItem(Key.key.entity, LT_DMAS_CMD, Key.keymap);
}

BOOL CDB_DMAS::Del(T_DMAS_K Key,CDB_STOR* pStor)
{
	T_DMAS_D Data;
	BOOL bExist=m_dmas.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_dmas.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)VERIFY(pStor->DelListItem(Key.key.entity, LT_DMAS_CMD, Key.keymap));
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_DMAS::Get(T_DMAS_K Key,T_DMAS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dmas.Lookup(Key.keymap,rData);
}

int CDB_DMAS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dmas.GetCount();
}

POSITION CDB_DMAS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dmas.GetStartPosition();
}

void CDB_DMAS::GetNext(POSITION& rNextPosition,T_DMAS_K& rKey,T_DMAS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_dmas.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
