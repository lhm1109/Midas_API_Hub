#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_LLLT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LLLT::CDB_LLLT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_lllt.InitHashTable(HASHSIZELLLT);
}

CDB_LLLT::~CDB_LLLT()
{

}

void CDB_LLLT::Add(T_LLLT_K Key,T_LLLT_D& rData,CDB_STLD* pStld)
{
	T_LLLT_D Data;
	BOOL bExist=m_lllt.Lookup(Key, Data);
	m_lllt.SetAt(Key,rData);
	if(!bExist)pStld->AddListItem(Key, LT_LLLT_CMD, Key);
}

BOOL CDB_LLLT::Del(T_LLLT_K Key,CDB_STLD* pStld)
{
	T_LLLT_D Data;
	BOOL bExist=m_lllt.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_lllt.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Key, LT_LLLT_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_LLLT::Get(T_LLLT_K Key,T_LLLT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_lllt.Lookup(Key,rData);
}

int CDB_LLLT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_lllt.GetCount();
}

POSITION CDB_LLLT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_lllt.GetStartPosition();
}

void CDB_LLLT::GetNext(POSITION& rNextPosition,T_LLLT_K& rKey,T_LLLT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_lllt.GetNextAssoc(rNextPosition,rKey,rData);
}
