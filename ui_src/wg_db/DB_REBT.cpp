#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_REBT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_REBT::CDB_REBT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rebt.InitHashTable(HASHSIZEREBT);
}

CDB_REBT::~CDB_REBT()
{

}

void CDB_REBT::Add(T_REBT_K Key,T_REBT_D& rData,CDB_SECT* pSect)
{
	T_REBT_D Data;
	BOOL bExist=m_rebt.Lookup(Key, Data);
	m_rebt.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_REBT_CMD, Key);
}

BOOL CDB_REBT::Del(T_REBT_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_rebt.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key, LT_REBT_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_REBT::Get(T_REBT_K Key,T_REBT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rebt.Lookup(Key,rData);
}

int CDB_REBT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rebt.GetCount();
}

POSITION CDB_REBT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rebt.GetStartPosition();
}

void CDB_REBT::GetNext(POSITION& rNextPosition,T_REBT_K& rKey,T_REBT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rebt.GetNextAssoc(rNextPosition,rKey,rData);
}
