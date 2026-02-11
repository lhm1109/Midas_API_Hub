#include "stdafx.h"
#include "DB_DCRB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DCRB::CDB_DCRB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dcrb.InitHashTable(HASHSIZEDCRB);
}

CDB_DCRB::~CDB_DCRB()
{

}

void CDB_DCRB::Add(T_DCRB_D& rData)
{
	m_dcrb.SetAt(1,rData);
}

BOOL CDB_DCRB::Del()
{
	T_DCRB_D Data;
	BOOL bExist=m_dcrb.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_dcrb.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_DCRB::Get(T_DCRB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dcrb.Lookup(1,rData);
}

int CDB_DCRB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dcrb.GetCount();
}
