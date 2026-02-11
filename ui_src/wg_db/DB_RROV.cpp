#include "stdafx.h"
#include "DB_RROV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RROV::CDB_RROV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rrov.InitHashTable(HASHSIZERROV);
}

CDB_RROV::~CDB_RROV()
{

}

void CDB_RROV::Add(T_RROV_K Key,T_RROV_D& rData)
{
	T_RROV_D Data;
	BOOL bExist=m_rrov.Lookup(Key, Data);
	m_rrov.SetAt(Key,rData);
}

BOOL CDB_RROV::Del(T_RROV_K Key)
{
	BOOL ret=m_rrov.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RROV::Get(T_RROV_K Key,T_RROV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rrov.Lookup(Key,rData);
}

int CDB_RROV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rrov.GetCount();
}

POSITION CDB_RROV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rrov.GetStartPosition();
}

void CDB_RROV::GetNext(POSITION& rNextPosition,T_RROV_K& rKey,T_RROV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rrov.GetNextAssoc(rNextPosition,rKey,rData);
}

