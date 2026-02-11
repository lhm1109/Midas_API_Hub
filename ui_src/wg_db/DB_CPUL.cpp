#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CPUL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPUL::CDB_CPUL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cpul.InitHashTable(HASHSIZECPUL);
}

CDB_CPUL::~CDB_CPUL()
{

}

void CDB_CPUL::Add(T_CPUL_K Key,T_CPUL_D& rData,CDB_ELEM* pElem)
{
	T_CPUL_D Data;
	BOOL bExist=m_cpul.Lookup(Key, Data);
	m_cpul.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CPUL_CMD, Key);
}

BOOL CDB_CPUL::Del(T_CPUL_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cpul.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_CPUL_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPUL::Get(T_CPUL_K Key,T_CPUL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cpul.Lookup(Key,rData);
}

int CDB_CPUL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpul.GetCount();
}

POSITION CDB_CPUL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpul.GetStartPosition();
}

void CDB_CPUL::GetNext(POSITION& rNextPosition,T_CPUL_K& rKey,T_CPUL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cpul.GetNextAssoc(rNextPosition,rKey,rData);
}
