//spcs
#include "stdafx.h"
#include "DB_SPCS.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPCS::CDB_SPCS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Spcs.InitHashTable(HASHSIZESPCS);
}

CDB_SPCS::~CDB_SPCS()
{

}

void CDB_SPCS::Add(T_SPCS_K Key, T_SPCS_D& rData, CDB_ELEM* pElem)
{
	T_SPCS_D Data;
	BOOL bExist = m_Spcs.Lookup(Key, Data);
	m_Spcs.SetAt(Key, rData);
	if ( !bExist && pElem )pElem->AddListItem(Key, LT_SPCS_CMD, Key);
	//if(!bExist && pSect)pSect->AddListItem(Key, LT_SPCS_CMD, Key);
}

BOOL CDB_SPCS::Del(T_SPCS_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_Spcs.RemoveKey(Key);
	ASSERT(ret);
	if ( ret && pElem )VERIFY(pElem->DelListItem(Key, LT_SPCS_CMD, Key));
	//if(ret && pSect)VERIFY(pSect->DelListItem(Key, LT_SPCS_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SPCS::Get(T_SPCS_K Key, T_SPCS_D& rData)
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return FALSE;
	return m_Spcs.Lookup(Key, rData);
}

int CDB_SPCS::GetCount()
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return 0;
	return m_Spcs.GetCount();
}

POSITION CDB_SPCS::GetStart()
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return 0;
	return m_Spcs.GetStartPosition();
}

void CDB_SPCS::GetNext(POSITION& rNextPosition, T_SPCS_K& rKey, T_SPCS_D& rData)
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
	{
		ASSERT(0);
		return;
	}
	m_Spcs.GetNextAssoc(rNextPosition, rKey, rData);
}