#include "stdafx.h"
#include "DB_PESV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PESV::CDB_PESV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pesv.InitHashTable(HASHSIZEPESV);
}

CDB_PESV::~CDB_PESV()
{

}

void CDB_PESV::Add(T_PESV_K Key, T_PESV_D& rData)
{
	T_PESV_D Data;
	BOOL bExist = m_pesv.Lookup(Key, Data);
	m_pesv.SetAt(Key, rData);
}

BOOL CDB_PESV::Del(T_PESV_K Key)
{
	BOOL ret = m_pesv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PESV::Get(T_PESV_K Key, T_PESV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_pesv.Lookup(Key, rData);
}

int CDB_PESV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pesv.GetCount();
}

POSITION CDB_PESV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pesv.GetStartPosition();
}

void CDB_PESV::GetNext(POSITION& rNextPosition, T_PESV_K& rKey, T_PESV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_pesv.GetNextAssoc(rNextPosition, rKey, rData);
}

