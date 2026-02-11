#include "stdafx.h"
#include "DB_SPLC.h"
#include "DB_RSMF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RSMF::CDB_RSMF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rsmf.InitHashTable(HASHSIZERSMF);
}

CDB_RSMF::~CDB_RSMF()
{

}

void CDB_RSMF::Add(T_RSMF_K Key,T_RSMF_D& rData)
{
	T_RSMF_D Data;
	BOOL bExist=m_rsmf.Lookup(Key, Data);
	m_rsmf.SetAt(Key,rData);
}

BOOL CDB_RSMF::Del(T_RSMF_K Key)
{
	T_RSMF_D Data;
	BOOL bExist=m_rsmf.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_rsmf.RemoveKey(Key);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_RSMF::Get(T_RSMF_K Key,T_RSMF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rsmf.Lookup(Key,rData);
}

int CDB_RSMF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rsmf.GetCount();
}

POSITION CDB_RSMF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rsmf.GetStartPosition();
}

void CDB_RSMF::GetNext(POSITION& rNextPosition,T_RSMF_K& rKey,T_RSMF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rsmf.GetNextAssoc(rNextPosition,rKey,rData);
}
