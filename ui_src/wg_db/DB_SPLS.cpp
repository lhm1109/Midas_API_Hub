//spls
#include "stdafx.h"
#include "DB_SPLS.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPLS::CDB_SPLS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Spls.InitHashTable(HASHSIZESPLS);
}

CDB_SPLS::~CDB_SPLS()
{

}

void CDB_SPLS::Add(T_SPLS_K Key, T_SPLS_D& rData, CDB_ELEM* pElem)
{
	T_SPLS_D Data;
	BOOL bExist = m_Spls.Lookup(Key, Data);
	m_Spls.SetAt(Key, rData);
	if ( !bExist && pElem )pElem->AddListItem(Key, LT_SPLS_CMD, Key);
	//if(!bExist && pSect)pSect->AddListItem(Key, LT_SPLS_CMD, Key);
}

BOOL CDB_SPLS::Del(T_SPLS_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_Spls.RemoveKey(Key);
	ASSERT(ret);
	if ( ret && pElem )VERIFY(pElem->DelListItem(Key, LT_SPLS_CMD, Key));
	//if(ret && pSect)VERIFY(pSect->DelListItem(Key, LT_SPLS_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SPLS::Get(T_SPLS_K Key, T_SPLS_D& rData)
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return FALSE;
	return m_Spls.Lookup(Key, rData);
}

int CDB_SPLS::GetCount()
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return 0;
	return m_Spls.GetCount();
}

POSITION CDB_SPLS::GetStart()
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return 0;
	return m_Spls.GetStartPosition();
}

void CDB_SPLS::GetNext(POSITION& rNextPosition, T_SPLS_K& rKey, T_SPLS_D& rData)
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
	{
		ASSERT(0);
		return;
	}
	m_Spls.GetNextAssoc(rNextPosition, rKey, rData);
}