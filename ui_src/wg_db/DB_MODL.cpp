#include "stdafx.h"
#include "DB_MODL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MODL::CDB_MODL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_modl.InitHashTable(HASHSIZEMODL);
}

CDB_MODL::~CDB_MODL()
{

}

void CDB_MODL::Add(T_MODL_D& rData)
{
	m_modl.SetAt(1,rData);
}

BOOL CDB_MODL::Del()
{
	T_MODL_D Data;
	BOOL bExist=m_modl.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_modl.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MODL::Get(T_MODL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_modl.Lookup(1,rData);
}

int CDB_MODL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_modl.GetCount();
}
