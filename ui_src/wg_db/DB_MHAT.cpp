#include "stdafx.h"
#include "DB_MATL.h"
#include "DB_MHAT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MHAT::CDB_MHAT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mhat.InitHashTable(HASHSIZEMHAT);
}

CDB_MHAT::~CDB_MHAT()
{

}

void CDB_MHAT::Add(T_MHAT_K Key,T_MHAT_D& rData,CDB_MATL* pMatl)
{
	T_MHAT_D Data;
	BOOL bExist=m_mhat.Lookup(Key, Data);
	m_mhat.SetAt(Key,rData);
	if(!bExist && pMatl != NULL)pMatl->AddListItem(Key, LT_MHAT_CMD, Key);
}

BOOL CDB_MHAT::Del(T_MHAT_K Key,CDB_MATL* pMatl)
{
	T_MHAT_D Data;
	BOOL bExist=m_mhat.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_mhat.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pMatl != NULL)VERIFY(pMatl->DelListItem(Key, LT_MHAT_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MHAT::Get(T_MHAT_K Key,T_MHAT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mhat.Lookup(Key,rData);
}

int CDB_MHAT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mhat.GetCount();
}

POSITION CDB_MHAT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mhat.GetStartPosition();
}

void CDB_MHAT::GetNext(POSITION& rNextPosition,T_MHAT_K& rKey,T_MHAT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_mhat.GetNextAssoc(rNextPosition,rKey,rData);
}
