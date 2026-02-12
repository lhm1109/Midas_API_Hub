#include "stdafx.h"
#include "DB_PRNT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PRNT::CDB_PRNT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_prnt.InitHashTable(HASHSIZEPRNT);
}

CDB_PRNT::~CDB_PRNT()
{

}

void CDB_PRNT::Add(T_PRNT_D& rData)
{
	m_prnt.SetAt(1,rData);
}

BOOL CDB_PRNT::Del()
{
	T_PRNT_D Data;
	BOOL bExist=m_prnt.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_prnt.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_PRNT::Get(T_PRNT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_prnt.Lookup(1,rData);
}

int CDB_PRNT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_prnt.GetCount();
}
