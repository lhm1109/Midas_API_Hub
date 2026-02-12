#include "stdafx.h"
#include "DB_PPLC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PPLC::CDB_PPLC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pplc.InitHashTable(HASHSIZEPPLC);
}

CDB_PPLC::~CDB_PPLC()
{

}

void CDB_PPLC::Add(T_PPLC_D& rData)
{
	m_pplc.SetAt(1,rData);
}

BOOL CDB_PPLC::Del()
{
	T_PPLC_D Data;
	BOOL bExist=m_pplc.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_pplc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_PPLC::Get(T_PPLC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_pplc.Lookup(1,rData);
}

int CDB_PPLC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pplc.GetCount();
}
