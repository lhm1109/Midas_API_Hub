#include "stdafx.h"
#include "DB_MVCTid.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVCTid::CDB_MVCTid()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mvctid.InitHashTable(HASHSIZEMVCTid);
}

CDB_MVCTid::~CDB_MVCTid()
{

}

void CDB_MVCTid::Add(T_MVCTid_D& rData)
{
	m_mvctid.SetAt(1,rData);
}

BOOL CDB_MVCTid::Del()
{
	T_MVCTid_D Data;
	BOOL bExist=m_mvctid.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mvctid.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MVCTid::Get(T_MVCTid_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mvctid.Lookup(1,rData);
}

int CDB_MVCTid::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvctid.GetCount();
}
