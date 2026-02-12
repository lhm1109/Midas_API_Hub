#include "stdafx.h"
#include "DB_RPAC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RPAC::CDB_RPAC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rpac.InitHashTable(HASHSIZERPAC);
}

CDB_RPAC::~CDB_RPAC()
{

}

void CDB_RPAC::Add(T_RPAC_D& rData)
{
	m_rpac.SetAt(1,rData);
}

BOOL CDB_RPAC::Del()
{
	T_RPAC_D Data;
	BOOL bExist=m_rpac.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_rpac.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RPAC::Get(T_RPAC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rpac.Lookup(1,rData);
}

int CDB_RPAC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rpac.GetCount();
}
