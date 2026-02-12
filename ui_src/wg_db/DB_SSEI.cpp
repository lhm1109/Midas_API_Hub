#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_SSEI.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SSEI::CDB_SSEI()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ssei.InitHashTable(HASHSIZESSEI);
}

CDB_SSEI::~CDB_SSEI()
{

}

void CDB_SSEI::Add(T_SSEI_K Key,T_SSEI_D& rData,CDB_STLD* pStld)
{
	T_SSEI_D Data;
	BOOL bExist=m_ssei.Lookup(Key, Data);
	m_ssei.SetAt(Key,rData);
	if(!bExist)pStld->AddListItem(Key, LT_SSEI_CMD, Key);
}

BOOL CDB_SSEI::Del(T_SSEI_K Key,CDB_STLD* pStld)
{
	T_SSEI_D Data;
	BOOL bExist=m_ssei.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_ssei.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Key, LT_SSEI_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SSEI::Get(T_SSEI_K Key,T_SSEI_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_ssei.Lookup(Key,rData);
}

int CDB_SSEI::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ssei.GetCount();
}

POSITION CDB_SSEI::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ssei.GetStartPosition();
}

void CDB_SSEI::GetNext(POSITION& rNextPosition,T_SSEI_K& rKey,T_SSEI_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_ssei.GetNextAssoc(rNextPosition,rKey,rData);
}
