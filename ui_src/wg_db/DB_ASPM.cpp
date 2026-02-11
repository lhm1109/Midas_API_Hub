#include "stdafx.h"
#include "DB_ASPM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ASPM::CDB_ASPM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_aspm.InitHashTable(HASHSIZEASPM);
}

CDB_ASPM::~CDB_ASPM()
{

}

void CDB_ASPM::Add(T_ASPM_D& rData)
{
	m_aspm.SetAt(1,rData);
}

BOOL CDB_ASPM::Del()
{
	T_ASPM_D Data;
	BOOL bExist=m_aspm.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_aspm.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_ASPM::Get(T_ASPM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_aspm.Lookup(1,rData);
}

int CDB_ASPM::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_aspm.GetCount();
}
