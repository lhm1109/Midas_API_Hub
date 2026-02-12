#include "stdafx.h"
#include "DB_CPGD.h"

//#include "DBDoc.h"
//#include "StagCtrl.h"

CDB_CPGD::CDB_CPGD()
{
	//m_pDoc = CDBDoc::GetDocPoint();
	m_cpgd.InitHashTable(HASHSIZECPGD);
}

CDB_CPGD::~CDB_CPGD()
{

}

void CDB_CPGD::Add(T_CPGD_D& rData)
{
	m_cpgd.SetAt(1,rData);
}

BOOL CDB_CPGD::Del()
{
	T_CPGD_D Data;
	BOOL bExist=m_cpgd.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_cpgd.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_CPGD::Get(T_CPGD_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return FALSE;
	return m_cpgd.Lookup(1,rData);
}

int CDB_CPGD::GetCount()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return 0;
	return m_cpgd.GetCount();
}
