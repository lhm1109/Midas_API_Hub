#include "stdafx.h"
#include "DB_LENV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LENV::CDB_LENV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_lenv.InitHashTable(HASHSIZELENV);
}

CDB_LENV::~CDB_LENV()
{

}

void CDB_LENV::Add(T_LENV_K Key,T_LENV_D& rData)
{
	T_LENV_D Data;
	BOOL bExist=m_lenv.Lookup(Key, Data);
	m_lenv.SetAt(Key,rData);
}

BOOL CDB_LENV::Del(T_LENV_K Key)
{
	BOOL ret=m_lenv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_LENV::Get(T_LENV_K Key,T_LENV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_lenv.Lookup(Key,rData);
}

int CDB_LENV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_lenv.GetCount();
}

POSITION CDB_LENV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_lenv.GetStartPosition();
}

void CDB_LENV::GetNext(POSITION& rNextPosition,T_LENV_K& rKey,T_LENV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_lenv.GetNextAssoc(rNextPosition,rKey,rData);
}
