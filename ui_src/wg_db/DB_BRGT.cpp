#include "stdafx.h"
#include "DB_BRGT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BRGT::CDB_BRGT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_brgt.InitHashTable(HASHSIZEBRGT);
}

CDB_BRGT::~CDB_BRGT()
{

}

void CDB_BRGT::Add(T_BRGT_D& rData)
{
	m_brgt.SetAt(1,rData);
}

BOOL CDB_BRGT::Del()
{
	T_BRGT_D Data;
	BOOL bExist=m_brgt.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_brgt.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_BRGT::Get(T_BRGT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_brgt.Lookup(1,rData);
}

int CDB_BRGT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_brgt.GetCount();
}
