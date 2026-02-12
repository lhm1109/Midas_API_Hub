#include "stdafx.h"
#include "DB_STOR.h"
#include "DB_ELEM.h"
#include "DB_DCBW.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DCBW::CDB_DCBW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dcbw.InitHashTable(HASHSIZEDCBW);
}

CDB_DCBW::~CDB_DCBW()
{

}

void CDB_DCBW::Add(T_DCBW_K Key,T_DCBW_D& rData,CDB_STOR* pStor)
{
	T_DCBW_D Data;
	BOOL bExist=m_dcbw.Lookup(Key.keymap, Data);
	m_dcbw.SetAt(Key.keymap,rData);
	if(!bExist)pStor->AddListItem(Key.key.storid, LT_DCBW_CMD, Key.keymap);
}

BOOL CDB_DCBW::Del(T_DCBW_K Key,CDB_STOR* pStor)
{
	T_DCBW_D Data;
	BOOL bExist=m_dcbw.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_dcbw.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)VERIFY(pStor->DelListItem(Key.key.storid, LT_DCBW_CMD, Key.keymap));
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_DCBW::Get(T_DCBW_K Key,T_DCBW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dcbw.Lookup(Key.keymap,rData);
}

int CDB_DCBW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dcbw.GetCount();
}

POSITION CDB_DCBW::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dcbw.GetStartPosition();
}

void CDB_DCBW::GetNext(POSITION& rNextPosition,T_DCBW_K& rKey,T_DCBW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_dcbw.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
