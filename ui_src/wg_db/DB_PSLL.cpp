#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_PSLL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PSLL::CDB_PSLL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_psll.InitHashTable(HASHSIZEPSLL);
}

CDB_PSLL::~CDB_PSLL()
{

}

void CDB_PSLL::Add(T_PSLL_K Key,T_PSLL_D& rData,CDB_STLD* pStld)
{
	T_PSLL_D Data;
	BOOL bExist=m_psll.Lookup(Key, Data);
	m_psll.SetAt(Key,rData);
	if(!bExist)pStld->AddListItem(Key, LT_PSLL_CMD, Key);
}

BOOL CDB_PSLL::Del(T_PSLL_K Key,CDB_STLD* pStld)
{
	T_PSLL_D Data;
	BOOL bExist=m_psll.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_psll.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Key, LT_PSLL_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_PSLL::Get(T_PSLL_K Key,T_PSLL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_psll.Lookup(Key,rData);
}

int CDB_PSLL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_psll.GetCount();
}

POSITION CDB_PSLL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_psll.GetStartPosition();
}

void CDB_PSLL::GetNext(POSITION& rNextPosition,T_PSLL_K& rKey,T_PSLL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_psll.GetNextAssoc(rNextPosition,rKey,rData);
}
