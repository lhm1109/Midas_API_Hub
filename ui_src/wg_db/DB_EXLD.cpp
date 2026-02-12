#include "stdafx.h"
#include "DB_EXLD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_EXLD::CDB_EXLD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_exld.InitHashTable(HASHSIZEEXLD);
}

CDB_EXLD::~CDB_EXLD()
{

}

void CDB_EXLD::Add(T_EXLD_D& rData)
{
	m_exld.SetAt(1,rData);
}

BOOL CDB_EXLD::Del()
{
	T_EXLD_D Data;
	BOOL bExist=m_exld.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_exld.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_EXLD::Get(T_EXLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_exld.Lookup(1,rData);
}

int CDB_EXLD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_exld.GetCount();
}

