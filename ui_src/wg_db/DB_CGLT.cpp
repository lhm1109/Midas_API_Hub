#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CGLT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGLT::CDB_CGLT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cglt.InitHashTable(HASHSIZECGLT);
}

CDB_CGLT::~CDB_CGLT()
{

}

void CDB_CGLT::Add(T_CGLT_K Key,T_CGLT_D& rData,CDB_ELEM* pElem)
{
	T_CGLT_D Data;
	BOOL bExist=m_cglt.Lookup(Key, Data);
	m_cglt.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CGLT_CMD, Key);
}

BOOL CDB_CGLT::Del(T_CGLT_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cglt.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_CGLT_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGLT::Get(T_CGLT_K Key,T_CGLT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cglt.Lookup(Key,rData);
}

int CDB_CGLT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cglt.GetCount();
}

POSITION CDB_CGLT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cglt.GetStartPosition();
}

void CDB_CGLT::GetNext(POSITION& rNextPosition,T_CGLT_K& rKey,T_CGLT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cglt.GetNextAssoc(rNextPosition,rKey,rData);
}
