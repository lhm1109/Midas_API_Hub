#include "stdafx.h"
#include "DB_SECO.h"
#include "DB_ELEM.h"
#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SECO::CDB_SECO()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_SECO.InitHashTable(HASHSIZESECO);
}

CDB_SECO::~CDB_SECO()
{
}

void CDB_SECO::Add(T_SECO_K Key, T_SECO_D& rData, CDB_ELEM* pElem)
{
	T_SECO_D Data;
	BOOL bExist = m_SECO.Lookup(Key, Data);
	m_SECO.SetAt(Key, rData);
	if ( !bExist && pElem )
	{
		pElem->AddListItem(Key, LT_SECO_CMD, Key);
	}
}

BOOL CDB_SECO::Del(T_SECO_K Key, CDB_ELEM* pElem)
{
	BOOL bRemove = m_SECO.RemoveKey(Key);
	ASSERT(bRemove);
	if ( bRemove && pElem )
	{
		VERIFY(pElem->DelListItem(Key, LT_SECO_CMD, Key));
	}
	return bRemove;
}

BOOL CDB_SECO::Get(T_SECO_K Key, T_SECO_D& rData)
{
	if ( m_pDoc == nullptr ) return FALSE;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return FALSE;
	return m_SECO.Lookup(Key, rData);
}

int CDB_SECO::GetCount()
{
	if ( m_pDoc == nullptr ) return 0;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return 0;
	return m_SECO.GetCount();
}

POSITION CDB_SECO::GetStart()
{
	if ( m_pDoc == nullptr ) return nullptr;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return nullptr;
	return m_SECO.GetStartPosition();
}

void CDB_SECO::GetNext(POSITION& rNextPosition, T_SECO_K& rKey, T_SECO_D& rData)
{
	if ( m_pDoc == nullptr ) return;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) { ASSERT(0); return; }
	m_SECO.GetNextAssoc(rNextPosition, rKey, rData);
}