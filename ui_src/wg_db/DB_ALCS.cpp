#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_ALCS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ALCS::CDB_ALCS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_alcs.InitHashTable(HASHSIZEALCS);
}

CDB_ALCS::~CDB_ALCS()
{

}

void CDB_ALCS::Add(T_ALCS_K Key,T_ALCS_D& rData)
{
	T_ALCS_D Data;
	BOOL bExist=m_alcs.Lookup(Key, Data);
	m_alcs.SetAt(Key,rData);
}

BOOL CDB_ALCS::Del(T_ALCS_K Key)
{
	BOOL ret=m_alcs.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_ALCS::Get(T_ALCS_K Key,T_ALCS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		return FALSE;
	}
	return m_alcs.Lookup(Key,rData);
}

int CDB_ALCS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		return 0;
	}
	return m_alcs.GetCount();
}

POSITION CDB_ALCS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0  && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		return 0;
	}
	return m_alcs.GetStartPosition();
}

void CDB_ALCS::GetNext(POSITION& rNextPosition,T_ALCS_K& rKey,T_ALCS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0); return;
	}
	m_alcs.GetNextAssoc(rNextPosition,rKey,rData);
}
