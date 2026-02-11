#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_RBSB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RBSB::CDB_RBSB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rbsb.InitHashTable(HASHSIZERBSB);
}

CDB_RBSB::~CDB_RBSB()
{

}

void CDB_RBSB::Add(T_RBSB_K Key,T_RBSB_D& rData,CDB_SECT* pSect)
{
	T_RBSB_D Data;
	BOOL bExist=m_rbsb.Lookup(Key, Data);
	m_rbsb.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_RBSB_CMD, Key);
}

BOOL CDB_RBSB::Del(T_RBSB_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_rbsb.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key, LT_RBSB_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RBSB::Get(T_RBSB_K Key,T_RBSB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rbsb.Lookup(Key,rData);
}

int CDB_RBSB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rbsb.GetCount();
}

POSITION CDB_RBSB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rbsb.GetStartPosition();
}

void CDB_RBSB::GetNext(POSITION& rNextPosition,T_RBSB_K& rKey,T_RBSB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rbsb.GetNextAssoc(rNextPosition,rKey,rData);
}
