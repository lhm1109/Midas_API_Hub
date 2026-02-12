#include "stdafx.h"
#include "DB_BLRS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BLRS::CDB_BLRS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_blrs.InitHashTable(HASHSIZEBLRS);
}

CDB_BLRS::~CDB_BLRS()
{

}

void CDB_BLRS::Add(T_BLRS_D& rData)
{
	m_blrs.SetAt(1,rData);
}

BOOL CDB_BLRS::Del()
{
	T_BLRS_D Data;
	BOOL bExist=m_blrs.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_blrs.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_BLRS::Get(T_BLRS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_blrs.Lookup(1,rData);
}

int CDB_BLRS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_blrs.GetCount();
}
