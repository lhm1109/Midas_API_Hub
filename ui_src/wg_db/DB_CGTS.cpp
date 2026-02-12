#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CGTS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGTS::CDB_CGTS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cgts.InitHashTable(HASHSIZECGTS);
}

CDB_CGTS::~CDB_CGTS()
{

}

void CDB_CGTS::Add(T_CGTS_K Key,T_CGTS_D& rData,CDB_ELEM* pElem)
{
	T_CGTS_D Data;
	BOOL bExist=m_cgts.Lookup(Key, Data);
	m_cgts.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CGTS_CMD, Key);
}

BOOL CDB_CGTS::Del(T_CGTS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cgts.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_CGTS_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGTS::Get(T_CGTS_K Key,T_CGTS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cgts.Lookup(Key,rData);
}

int CDB_CGTS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgts.GetCount();
}

POSITION CDB_CGTS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgts.GetStartPosition();
}

void CDB_CGTS::GetNext(POSITION& rNextPosition,T_CGTS_K& rKey,T_CGTS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cgts.GetNextAssoc(rNextPosition,rKey,rData);
}
