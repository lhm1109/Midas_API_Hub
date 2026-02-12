#include "stdafx.h"
#include "DB_YSCO.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_YSCO::CDB_YSCO()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ysco.InitHashTable(HASHSIZEYSCO);
}

CDB_YSCO::~CDB_YSCO()
{

}

void CDB_YSCO::Add(T_YSCO_D& rData)
{
	m_ysco.SetAt(1,rData);
}

BOOL CDB_YSCO::Del()
{
	T_YSCO_D Data;
	BOOL bExist=m_ysco.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_ysco.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_YSCO::Get(T_YSCO_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_ysco.Lookup(1,rData);
}

int CDB_YSCO::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ysco.GetCount();
}
