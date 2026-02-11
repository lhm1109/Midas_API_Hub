#include "stdafx.h"
#include "stdafx.h"
#include "DB_RKCV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RKCV::CDB_RKCV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rkcv.InitHashTable(HASHSIZERKCV);
}

CDB_RKCV::~CDB_RKCV()
{

}

void CDB_RKCV::Add(T_RKCV_K Key,T_RKCV_D& rData)
{
	T_RKCV_D Data;
	BOOL bExist=m_rkcv.Lookup(Key, Data);
	m_rkcv.SetAt(Key,rData);
}

BOOL CDB_RKCV::Del(T_RKCV_K Key)
{
	BOOL ret= m_rkcv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RKCV::Get(T_RKCV_K Key,T_RKCV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rkcv.Lookup(Key,rData);
}

int CDB_RKCV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rkcv.GetCount();
}

POSITION CDB_RKCV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rkcv.GetStartPosition();
}

void CDB_RKCV::GetNext(POSITION& rNextPosition,T_RKCV_K& rKey,T_RKCV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rkcv.GetNextAssoc(rNextPosition,rKey,rData);
}

