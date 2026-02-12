#include "stdafx.h"
#include "DB_JDSH.h"
#include "DB_STOR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_JDSH::CDB_JDSH()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_JDSH.InitHashTable(HASHSIZEJDSH);
}

CDB_JDSH::~CDB_JDSH()
{
	
}

void CDB_JDSH::Add(T_JDSH_K Key,T_JDSH_D& rData,CDB_STOR* pStor)
{
	T_JDSH_D Data;
	BOOL bExist=m_JDSH.Lookup(Key, Data);
	m_JDSH.SetAt(Key,rData);
	if(!bExist && pStor)pStor->AddListItem(Key, LT_JDSH_CMD, Key);
}

BOOL CDB_JDSH::Del(T_JDSH_K Key,CDB_STOR* pStor)
{
	BOOL ret=m_JDSH.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pStor)VERIFY(pStor->DelListItem(Key, LT_JDSH_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_JDSH::Get(T_JDSH_K Key,T_JDSH_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_JDSH.Lookup(Key,rData);
}

int CDB_JDSH::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_JDSH.GetCount();
}

POSITION CDB_JDSH::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_JDSH.GetStartPosition();
}

void CDB_JDSH::GetNext(POSITION& rNextPosition,T_JDSH_K& rKey,T_JDSH_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_JDSH.GetNextAssoc(rNextPosition,rKey,rData);
}
