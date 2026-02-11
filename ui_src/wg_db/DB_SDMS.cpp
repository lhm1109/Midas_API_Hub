#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_SDMS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SDMS::CDB_SDMS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sdms.InitHashTable(HASHSIZESDMS);
}

CDB_SDMS::~CDB_SDMS()
{

}

void CDB_SDMS::Add(T_SDMS_K Key,T_SDMS_D& rData,CDB_SECT* pSect)
{
	T_SDMS_D Data;
	BOOL bExist=m_sdms.Lookup(Key, Data);
	m_sdms.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_SDMS_CMD, Key);
}

BOOL CDB_SDMS::Del(T_SDMS_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_sdms.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key, LT_SDMS_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SDMS::Get(T_SDMS_K Key,T_SDMS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_sdms.Lookup(Key,rData);
}

int CDB_SDMS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_sdms.GetCount();
}

POSITION CDB_SDMS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_sdms.GetStartPosition();
}

void CDB_SDMS::GetNext(POSITION& rNextPosition,T_SDMS_K& rKey,T_SDMS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_sdms.GetNextAssoc(rNextPosition,rKey,rData);
}
