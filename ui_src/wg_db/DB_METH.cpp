#include "stdafx.h"
#include "DB_METH.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_METH::CDB_METH()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_meth.InitHashTable(HASHSIZEMETH);
}

CDB_METH::~CDB_METH()
{

}

void CDB_METH::Add(T_METH_D& rData)
{
	m_meth.SetAt(1,rData);
}

BOOL CDB_METH::Del()
{
	T_METH_D Data;
	BOOL bExist=m_meth.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_meth.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_METH::Get(T_METH_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_meth.Lookup(1,rData);
}

int CDB_METH::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_meth.GetCount();
}
