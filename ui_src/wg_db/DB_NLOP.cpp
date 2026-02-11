#include "stdafx.h"
#include "DB_NLOP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_NLOP::CDB_NLOP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nlop.InitHashTable(HASHSIZENLOP);
}

CDB_NLOP::~CDB_NLOP()
{

}

void CDB_NLOP::Add(T_NLOP_D& rData)
{
	m_nlop.SetAt(1,rData);
}

BOOL CDB_NLOP::Del()
{
	T_NLOP_D Data;
	BOOL bExist=m_nlop.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_nlop.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_NLOP::Get(T_NLOP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_nlop.Lookup(1,rData);
}

int CDB_NLOP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_nlop.GetCount();
}
