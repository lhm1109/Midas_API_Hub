#include "stdafx.h"
#include "DB_BUCK.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BUCK::CDB_BUCK()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_buck.InitHashTable(HASHSIZEBUCK);
}

CDB_BUCK::~CDB_BUCK()
{

}

void CDB_BUCK::Add(T_BUCK_D& rData)
{
	m_buck.SetAt(1,rData);
}

BOOL CDB_BUCK::Del()
{
	T_BUCK_D Data;
	BOOL bExist=m_buck.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_buck.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_BUCK::Get(T_BUCK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_buck.Lookup(1,rData);
}

int CDB_BUCK::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_buck.GetCount();
}
