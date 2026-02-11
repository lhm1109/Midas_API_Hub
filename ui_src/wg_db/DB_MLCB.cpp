#include "stdafx.h"
#include "DB_MLCB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MLCB::CDB_MLCB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mlcb.InitHashTable(HASHSIZEMLCB);
}

CDB_MLCB::~CDB_MLCB()
{

}

void CDB_MLCB::Add(T_MLCB_D& rData)
{
	m_mlcb.SetAt(1,rData);
}

BOOL CDB_MLCB::Del()
{
	T_MLCB_D Data;
	BOOL bExist=m_mlcb.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mlcb.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MLCB::Get(T_MLCB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mlcb.Lookup(1,rData);
}

int CDB_MLCB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mlcb.GetCount();
}
