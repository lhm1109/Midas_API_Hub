#include "stdafx.h"
#include "DB_SEHK.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SEHK::CDB_SEHK()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_SEHK.InitHashTable(HASHSIZESEHK);
}

CDB_SEHK::~CDB_SEHK()
{

}

void CDB_SEHK::Add(T_SEHK_K Key,T_SEHK_D& rData,CDB_ELEM* pElem)
{
	T_SEHK_D Data;
	BOOL bExist=m_SEHK.Lookup(Key, Data);
	m_SEHK.SetAt(Key,rData);
	if(!bExist && pElem)pElem->AddListItem(Key, LT_SEHK_CMD, Key);
}

BOOL CDB_SEHK::Del(T_SEHK_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_SEHK.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pElem)VERIFY(pElem->DelListItem(Key, LT_SEHK_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SEHK::Get(T_SEHK_K Key,T_SEHK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_SEHK.Lookup(Key,rData);
}

int CDB_SEHK::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_SEHK.GetCount();
}

POSITION CDB_SEHK::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_SEHK.GetStartPosition();
}

void CDB_SEHK::GetNext(POSITION& rNextPosition,T_SEHK_K& rKey,T_SEHK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_SEHK.GetNextAssoc(rNextPosition,rKey,rData);
}
