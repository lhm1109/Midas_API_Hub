#include "stdafx.h"
#include "DB_DSRC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DSRC::CDB_DSRC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dsrc.InitHashTable(HASHSIZEDSRC);
}

CDB_DSRC::~CDB_DSRC()
{

}

void CDB_DSRC::Add(T_DSRC_D& rData)
{
	m_dsrc.SetAt(1,rData);
}

BOOL CDB_DSRC::Del()
{
	T_DSRC_D Data;
	BOOL bExist=m_dsrc.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_dsrc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_DSRC::Get(T_DSRC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dsrc.Lookup(1,rData);
}

int CDB_DSRC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dsrc.GetCount();
}
