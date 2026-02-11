#include "stdafx.h"
#include "DB_SERD.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SERD::CDB_SERD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_SERD.InitHashTable(HASHSIZESERD);
}

CDB_SERD::~CDB_SERD()
{

}

void CDB_SERD::Add(T_SERD_K Key,T_SERD_D& rData,CDB_ELEM* pElem)
{
	T_SERD_D Data;
	BOOL bExist=m_SERD.Lookup(Key, Data);
	m_SERD.SetAt(Key,rData);
	if(!bExist && pElem)pElem->AddListItem(Key, LT_SERD_CMD, Key);
}

BOOL CDB_SERD::Del(T_SERD_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_SERD.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pElem)VERIFY(pElem->DelListItem(Key, LT_SERD_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SERD::Get(T_SERD_K Key,T_SERD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_SERD.Lookup(Key,rData);
}

int CDB_SERD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_SERD.GetCount();
}

POSITION CDB_SERD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_SERD.GetStartPosition();
}

void CDB_SERD::GetNext(POSITION& rNextPosition,T_SERD_K& rKey,T_SERD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_SERD.GetNextAssoc(rNextPosition,rKey,rData);
}
