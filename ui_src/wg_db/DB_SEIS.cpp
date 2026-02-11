#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_SEIS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SEIS::CDB_SEIS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_seis.InitHashTable(HASHSIZESEIS);
}

CDB_SEIS::~CDB_SEIS()
{

}

void CDB_SEIS::Add(T_SEIS_K Key,T_SEIS_D& rData,CDB_STLD* pStld)
{
	T_SEIS_D Data;
	BOOL bExist=m_seis.Lookup(Key, Data);
	m_seis.SetAt(Key,rData);
	if(!bExist)pStld->AddListItem(Key, LT_SEIS_CMD, Key);
}

BOOL CDB_SEIS::Del(T_SEIS_K Key,CDB_STLD* pStld)
{
	T_SEIS_D Data;
	BOOL bExist=m_seis.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_seis.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Key, LT_SEIS_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SEIS::Get(T_SEIS_K Key,T_SEIS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_seis.Lookup(Key,rData);
}

int CDB_SEIS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_seis.GetCount();
}

POSITION CDB_SEIS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_seis.GetStartPosition();
}

void CDB_SEIS::GetNext(POSITION& rNextPosition,T_SEIS_K& rKey,T_SEIS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_seis.GetNextAssoc(rNextPosition,rKey,rData);
}
