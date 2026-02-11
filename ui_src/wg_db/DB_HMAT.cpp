#include "stdafx.h"
#include "DB_MATL.h"
#include "DB_HMAT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HMAT::CDB_HMAT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_hmat.InitHashTable(HASHSIZEHMAT);
}

CDB_HMAT::~CDB_HMAT()
{

}

void CDB_HMAT::Add(T_HMAT_K Key,T_HMAT_D& rData,CDB_MATL* pMatl)
{
	T_HMAT_D Data;
	BOOL bExist=m_hmat.Lookup(Key, Data);
	m_hmat.SetAt(Key,rData);
	if(!bExist && pMatl != NULL)pMatl->AddListItem(Key, LT_HMAT_CMD, Key);
}

BOOL CDB_HMAT::Del(T_HMAT_K Key,CDB_MATL* pMatl)
{
	T_HMAT_D Data;
	BOOL bExist=m_hmat.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_hmat.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pMatl != NULL)VERIFY(pMatl->DelListItem(Key, LT_HMAT_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_HMAT::Get(T_HMAT_K Key,T_HMAT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_hmat.Lookup(Key,rData);
}

int CDB_HMAT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_hmat.GetCount();
}

POSITION CDB_HMAT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_hmat.GetStartPosition();
}

void CDB_HMAT::GetNext(POSITION& rNextPosition,T_HMAT_K& rKey,T_HMAT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_hmat.GetNextAssoc(rNextPosition,rKey,rData);
}
