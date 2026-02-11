#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_DCBC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DCBC::CDB_DCBC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dcbc.InitHashTable(HASHSIZEDCBC);
}

CDB_DCBC::~CDB_DCBC()
{

}

void CDB_DCBC::Add(T_DCBC_K Key,T_DCBC_D& rData,CDB_ELEM* pElem)
{
	T_DCBC_D Data;
	BOOL bExist=m_dcbc.Lookup(Key, Data);
	m_dcbc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_DCBC_CMD, Key);
}

BOOL CDB_DCBC::Del(T_DCBC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_dcbc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_DCBC_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DCBC::Get(T_DCBC_K Key,T_DCBC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dcbc.Lookup(Key,rData);
}

int CDB_DCBC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dcbc.GetCount();
}

POSITION CDB_DCBC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dcbc.GetStartPosition();
}

void CDB_DCBC::GetNext(POSITION& rNextPosition,T_DCBC_K& rKey,T_DCBC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_dcbc.GetNextAssoc(rNextPosition,rKey,rData);
}
