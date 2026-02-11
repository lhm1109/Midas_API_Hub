#include "stdafx.h"
#include "DB_SPLV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPLV::CDB_SPLV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_splv.InitHashTable(HASHSIZESPLV);
}

CDB_SPLV::~CDB_SPLV()
{

}

void CDB_SPLV::Add(T_SPLV_K Key,T_SPLV_D& rData)
{
	T_SPLV_D Data;
	BOOL bExist=m_splv.Lookup(Key, Data);
	m_splv.SetAt(Key,rData);
}

BOOL CDB_SPLV::Del(T_SPLV_K Key)
{
	BOOL ret=m_splv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SPLV::Get(T_SPLV_K Key,T_SPLV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_splv.Lookup(Key,rData);
}

int CDB_SPLV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_splv.GetCount();
}

POSITION CDB_SPLV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_splv.GetStartPosition();
}

void CDB_SPLV::GetNext(POSITION& rNextPosition,T_SPLV_K& rKey,T_SPLV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_splv.GetNextAssoc(rNextPosition,rKey,rData);
}
