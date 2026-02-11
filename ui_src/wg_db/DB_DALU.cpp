#include "stdafx.h"
#include "DB_DALU.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DALU::CDB_DALU()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dalu.InitHashTable(HASHSIZEDALU);
}

CDB_DALU::~CDB_DALU()
{

}

void CDB_DALU::Add(T_DALU_D& rData)
{
	m_dalu.SetAt(1,rData);
}

BOOL CDB_DALU::Del()
{
	T_DALU_D Data;
	BOOL bExist=m_dalu.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_dalu.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_DALU::Get(T_DALU_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dalu.Lookup(1,rData);
}

int CDB_DALU::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dalu.GetCount();
}
