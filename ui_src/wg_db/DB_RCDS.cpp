#include "stdafx.h"
#include "DB_RCDS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RCDS::CDB_RCDS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rcds.InitHashTable(HASHSIZERCDS);
}

CDB_RCDS::~CDB_RCDS()
{

}

void CDB_RCDS::Add(T_RCDS_D& rData)
{
	m_rcds.SetAt(1,rData);
}

BOOL CDB_RCDS::Del()
{
	T_RCDS_D Data;
	BOOL bExist=m_rcds.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_rcds.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RCDS::Get(T_RCDS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rcds.Lookup(1,rData);
}

BOOL CDB_RCDS::Get(T_RCDS_K Key,T_RCDS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rcds.Lookup(Key,rData);
}

int CDB_RCDS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rcds.GetCount();
}
