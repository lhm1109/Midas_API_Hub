#include "stdafx.h"
#include "DB_RISS.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RISS::CDB_RISS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_RISS.InitHashTable(HASHSIZERISS);
}

CDB_RISS::~CDB_RISS()
{

}

void CDB_RISS::Add(T_RISS_K Key,T_RISS_D& rData,CDB_ELEM* pElem)
{
	T_RISS_D Data;
	BOOL bExist=m_RISS.Lookup(Key, Data);
	m_RISS.SetAt(Key,rData);
	if(!bExist && pElem)pElem->AddListItem(Key, LT_RISS_CMD, Key);
}

BOOL CDB_RISS::Del(T_RISS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_RISS.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pElem)VERIFY(pElem->DelListItem(Key, LT_RISS_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RISS::Get(T_RISS_K Key,T_RISS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_RISS.Lookup(Key,rData);
}

int CDB_RISS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_RISS.GetCount();
}

POSITION CDB_RISS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_RISS.GetStartPosition();
}

void CDB_RISS::GetNext(POSITION& rNextPosition,T_RISS_K& rKey,T_RISS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_RISS.GetNextAssoc(rNextPosition,rKey,rData);
}
