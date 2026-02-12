#include "stdafx.h"
#include "DB_SECT.h" 
#include "DB_STDC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STDC::CDB_STDC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_stdc.InitHashTable(HASHSIZESTDC);
}

CDB_STDC::~CDB_STDC()
{

}

void CDB_STDC::Add(T_STDC_K Key,T_STDC_D& rData,CDB_SECT* pSect)
{
	T_STDC_D Data;
	BOOL bExist=m_stdc.Lookup(Key, Data);
	m_stdc.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_STDC_CMD, Key);
}

BOOL CDB_STDC::Del(T_STDC_K Key,CDB_SECT* pSect)
{
	T_STDC_D Data;
	BOOL bExist=m_stdc.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_stdc.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_STDC_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_STDC::Get(T_STDC_K Key,T_STDC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_stdc.Lookup(Key,rData);
}

int CDB_STDC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_stdc.GetCount();
}

POSITION CDB_STDC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_stdc.GetStartPosition();
}

void CDB_STDC::GetNext(POSITION& rNextPosition,T_STDC_K& rKey,T_STDC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_stdc.GetNextAssoc(rNextPosition,rKey,rData);
}
