#include "stdafx.h"
#include "DB_KFAV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_KFAV::CDB_KFAV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_kfav.InitHashTable(HASHSIZEKFAV);
}

CDB_KFAV::~CDB_KFAV()
{

}

void CDB_KFAV::Add(T_KFAV_K Key,T_KFAV_D& rData)
{
	T_KFAV_D Data;
	BOOL bExist=m_kfav.Lookup(Key, Data);
	m_kfav.SetAt(Key,rData);
}

BOOL CDB_KFAV::Del(T_KFAV_K Key)
{
	BOOL ret=m_kfav.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_KFAV::Get(T_KFAV_K Key,T_KFAV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_kfav.Lookup(Key,rData);
}

int CDB_KFAV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_kfav.GetCount();
}

POSITION CDB_KFAV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_kfav.GetStartPosition();
}

void CDB_KFAV::GetNext(POSITION& rNextPosition,T_KFAV_K& rKey,T_KFAV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_kfav.GetNextAssoc(rNextPosition,rKey,rData);
}
