#include "stdafx.h"
#include "DB_RPAS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RPAS::CDB_RPAS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rpas.InitHashTable(HASHSIZERPAS);
}

CDB_RPAS::~CDB_RPAS()
{

}

void CDB_RPAS::Add(T_RPAS_D& rData)
{
	m_rpas.SetAt(1,rData);
}

BOOL CDB_RPAS::Del()
{
	T_RPAS_D Data;
	BOOL bExist=m_rpas.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_rpas.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RPAS::Get(T_RPAS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rpas.Lookup(1,rData);
}

int CDB_RPAS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rpas.GetCount();
}
