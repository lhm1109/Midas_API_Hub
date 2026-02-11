#include "stdafx.h"
#include "DB_DCON.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DCON::CDB_DCON()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dcon.InitHashTable(HASHSIZEDCON);
}

CDB_DCON::~CDB_DCON()
{

}

void CDB_DCON::Add(T_DCON_D& rData)
{
	m_dcon.SetAt(1,rData);
}

BOOL CDB_DCON::Del()
{
	T_DCON_D Data;
	BOOL bExist=m_dcon.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_dcon.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_DCON::Get(T_DCON_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dcon.Lookup(1,rData);
}

int CDB_DCON::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dcon.GetCount();
}
