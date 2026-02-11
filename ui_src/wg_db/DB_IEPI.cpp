#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_IEPI.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_IEPI::CDB_IEPI()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_iepi.InitHashTable(HASHSIZEIEPI);
}

CDB_IEPI::~CDB_IEPI()
{

}

void CDB_IEPI::Add(T_IEPI_K Key,T_IEPI_D& rData,CDB_ELEM* pElem)
{
	T_IEPI_D Data;
	BOOL bExist=m_iepi.Lookup(Key, Data);
	
	m_iepi.SetAt(Key,rData);
	if(!bExist)
	{
		if (pElem != NULL) pElem->AddListItem(Key, LT_IEPI_CMD, Key);
	}
}

BOOL CDB_IEPI::Del(T_IEPI_K Key,CDB_ELEM* pElem)
{
	T_IEPI_D Data;
	BOOL bExist=m_iepi.Lookup(Key, Data);
	if(!bExist) return FALSE;

	BOOL ret=m_iepi.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		if(pElem != NULL)VERIFY(pElem->DelListItem(Key, LT_IEPI_CMD, Key));
	}
	return ret;
}


//-------------------------------------------------------------------------
BOOL CDB_IEPI::Get(T_IEPI_K Key,T_IEPI_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_iepi.Lookup(Key,rData);
}

int CDB_IEPI::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_iepi.GetCount();
}

POSITION CDB_IEPI::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_iepi.GetStartPosition();
}

void CDB_IEPI::GetNext(POSITION& rNextPosition,T_IEPI_K& rKey,T_IEPI_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_iepi.GetNextAssoc(rNextPosition,rKey,rData);
}
