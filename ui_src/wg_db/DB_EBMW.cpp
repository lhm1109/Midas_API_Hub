#include "stdafx.h"
#include "DB_STOR.h"
#include "DB_ELEM.h"
#include "DB_EBMW.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_EBMW::CDB_EBMW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ebmw.InitHashTable(HASHSIZEEBMW);
}

CDB_EBMW::~CDB_EBMW()
{

}

void CDB_EBMW::Add(T_EBMW_K Key,T_EBMW_D& rData,CDB_STOR* pStor)
{
	T_EBMW_D Data;
	BOOL bExist=m_ebmw.Lookup(Key.keymap, Data);
	m_ebmw.SetAt(Key.keymap,rData);
	if(!bExist)pStor->AddListItem(Key.key.storid, LT_EBMW_CMD, Key.keymap);
}

BOOL CDB_EBMW::Del(T_EBMW_K Key,CDB_STOR* pStor)
{
	T_EBMW_D Data;
	BOOL bExist=m_ebmw.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_ebmw.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)VERIFY(pStor->DelListItem(Key.key.storid, LT_EBMW_CMD, Key.keymap));
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_EBMW::Get(T_EBMW_K Key,T_EBMW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_ebmw.Lookup(Key.keymap,rData);
}

int CDB_EBMW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ebmw.GetCount();
}

POSITION CDB_EBMW::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ebmw.GetStartPosition();
}

void CDB_EBMW::GetNext(POSITION& rNextPosition,T_EBMW_K& rKey,T_EBMW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_ebmw.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
