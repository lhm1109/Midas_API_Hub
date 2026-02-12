#include "stdafx.h"
#include "DB_STLC.h"

//#include "DBDoc.h"
//#include "StagCtrl.h"

CDB_STLC::CDB_STLC()
{
	//m_pDoc = CDBDoc::GetDocPoint();
	m_stlc.InitHashTable(HASHSIZESTLC);
}

CDB_STLC::~CDB_STLC()
{

}

void CDB_STLC::Add(T_STLC_D& rData)
{
	m_stlc.SetAt(1,rData);
}

BOOL CDB_STLC::Del()
{
	T_STLC_D Data;
	BOOL bExist=m_stlc.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_stlc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_STLC::Get(T_STLC_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return FALSE;
	return m_stlc.Lookup(1,rData);
}

int CDB_STLC::GetCount()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return 0;
	return m_stlc.GetCount();
}
