#include "stdafx.h"
#include "DB_BPLT.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BPLT::CDB_BPLT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_BPLT.InitHashTable(HASHSIZEBPLT);
}

CDB_BPLT::~CDB_BPLT()
{
	
}

void CDB_BPLT::Add(T_BPLT_K Key,T_BPLT_D& rData,CDB_ELEM* pElem)
{
	T_BPLT_D Data;
	BOOL bExist=m_BPLT.Lookup(Key, Data);
	m_BPLT.SetAt(Key,rData);
	if(!bExist && pElem)pElem->AddListItem(Key, LT_BPLT_CMD, Key);
}

BOOL CDB_BPLT::Del(T_BPLT_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_BPLT.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pElem)VERIFY(pElem->DelListItem(Key, LT_BPLT_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_BPLT::Get(T_BPLT_K Key,T_BPLT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_BPLT.Lookup(Key,rData);
}

int CDB_BPLT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_BPLT.GetCount();
}

POSITION CDB_BPLT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_BPLT.GetStartPosition();
}

void CDB_BPLT::GetNext(POSITION& rNextPosition,T_BPLT_K& rKey,T_BPLT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_BPLT.GetNextAssoc(rNextPosition,rKey,rData);
}
