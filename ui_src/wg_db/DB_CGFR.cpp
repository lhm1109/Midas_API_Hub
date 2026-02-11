#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CGFR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGFR::CDB_CGFR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cgfr.InitHashTable(HASHSIZECGFR);
}

CDB_CGFR::~CDB_CGFR()
{

}

void CDB_CGFR::Add(T_CGFR_K Key,T_CGFR_D& rData,CDB_ELEM* pElem)
{
	T_CGFR_D Data;
	BOOL bExist=m_cgfr.Lookup(Key, Data);
	m_cgfr.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CGFR_CMD, Key);
}

BOOL CDB_CGFR::Del(T_CGFR_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cgfr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_CGFR_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGFR::Get(T_CGFR_K Key,T_CGFR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cgfr.Lookup(Key,rData);
}

int CDB_CGFR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgfr.GetCount();
}

POSITION CDB_CGFR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgfr.GetStartPosition();
}

void CDB_CGFR::GetNext(POSITION& rNextPosition,T_CGFR_K& rKey,T_CGFR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cgfr.GetNextAssoc(rNextPosition,rKey,rData);
}
