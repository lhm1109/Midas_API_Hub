#include "stdafx.h"
#include "DB_SEEW.h"
#include "DB_STOR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SEEW::CDB_SEEW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_SEEW.InitHashTable(HASHSIZESEEW);
}

CDB_SEEW::~CDB_SEEW()
{

}

void CDB_SEEW::Add(T_SEEW_K Key,T_SEEW_D& rData,CDB_STOR* pStor)
{
	T_SEEW_D Data;
	BOOL bExist=m_SEEW.Lookup(Key, Data);
	m_SEEW.SetAt(Key,rData);
	if(!bExist && pStor)pStor->AddListItem(Key, LT_SEEW_CMD, Key);
}

BOOL CDB_SEEW::Del(T_SEEW_K Key,CDB_STOR* pStor)
{
	BOOL ret=m_SEEW.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pStor)VERIFY(pStor->DelListItem(Key, LT_SEEW_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SEEW::Get(T_SEEW_K Key,T_SEEW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_SEEW.Lookup(Key,rData);
}

int CDB_SEEW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_SEEW.GetCount();
}

POSITION CDB_SEEW::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_SEEW.GetStartPosition();
}

void CDB_SEEW::GetNext(POSITION& rNextPosition,T_SEEW_K& rKey,T_SEEW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_SEEW.GetNextAssoc(rNextPosition,rKey,rData);
}
