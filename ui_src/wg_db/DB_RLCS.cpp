#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RLCS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RLCS::CDB_RLCS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rlcs.InitHashTable(HASHSIZERLCS);
}

CDB_RLCS::~CDB_RLCS()
{

}

void CDB_RLCS::Add(T_RLCS_K Key,T_RLCS_D& rData)
{
	T_RLCS_D Data;
	BOOL bExist=m_rlcs.Lookup(Key, Data);
	m_rlcs.SetAt(Key,rData);
}

BOOL CDB_RLCS::Del(T_RLCS_K Key)
{
	BOOL ret=m_rlcs.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RLCS::Get(T_RLCS_K Key,T_RLCS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		return FALSE;
	}
	return m_rlcs.Lookup(Key,rData);
}

int CDB_RLCS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		return 0;
	}
	return m_rlcs.GetCount();
}

POSITION CDB_RLCS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0  && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		return 0;
	}
	return m_rlcs.GetStartPosition();
}

void CDB_RLCS::GetNext(POSITION& rNextPosition,T_RLCS_K& rKey,T_RLCS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0); return;
	}
	m_rlcs.GetNextAssoc(rNextPosition,rKey,rData);
}
