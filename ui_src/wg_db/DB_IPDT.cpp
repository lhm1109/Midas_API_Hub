#include "stdafx.h"
#include "DB_IPDT.h"
#include "DB_STOR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_IPDT::CDB_IPDT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_IPDT.InitHashTable(HASHSIZEIPDT);
}

CDB_IPDT::~CDB_IPDT()
{

}

void CDB_IPDT::Add(T_IPDT_K Key,T_IPDT_D& rData,CDB_STOR* pStor)
{
	T_IPDT_D Data;
	BOOL bExist=m_IPDT.Lookup(Key, Data);
	m_IPDT.SetAt(Key,rData);
	if(!bExist && pStor)pStor->AddListItem(Key, LT_IPDT_CMD, Key);
}

BOOL CDB_IPDT::Del(T_IPDT_K Key,CDB_STOR* pStor)
{
	BOOL ret=m_IPDT.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pStor)VERIFY(pStor->DelListItem(Key, LT_IPDT_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_IPDT::Get(T_IPDT_K Key,T_IPDT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_IPDT.Lookup(Key,rData);
}

int CDB_IPDT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_IPDT.GetCount();
}

POSITION CDB_IPDT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_IPDT.GetStartPosition();
}

void CDB_IPDT::GetNext(POSITION& rNextPosition,T_IPDT_K& rKey,T_IPDT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_IPDT.GetNextAssoc(rNextPosition,rKey,rData);
}
