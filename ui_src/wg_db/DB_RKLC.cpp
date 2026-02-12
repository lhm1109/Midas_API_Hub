#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RKLC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RKLC::CDB_RKLC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rklc.InitHashTable(HASHSIZERKLC);
}

CDB_RKLC::~CDB_RKLC()
{

}

void CDB_RKLC::Add(T_RKLC_K Key,T_RKLC_D& rData)
{
	T_RKLC_D Data;
	BOOL bExist=m_rklc.Lookup(Key, Data);
	m_rklc.SetAt(Key,rData);
}

BOOL CDB_RKLC::Del(T_RKLC_K Key)
{
	BOOL ret=m_rklc.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RKLC::Get(T_RKLC_K Key,T_RKLC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		return FALSE;
	}
	return m_rklc.Lookup(Key,rData);
}

int CDB_RKLC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		return 0;
	}
	return m_rklc.GetCount();
}

POSITION CDB_RKLC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0  && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		return 0;
	}
	return m_rklc.GetStartPosition();
}

void CDB_RKLC::GetNext(POSITION& rNextPosition,T_RKLC_K& rKey,T_RKLC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0); return;
	}
	m_rklc.GetNextAssoc(rNextPosition,rKey,rData);
}
