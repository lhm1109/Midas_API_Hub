#include "stdafx.h"
#include "DB_SEWA.h"
#include "DB_ELEM.h"
#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SEWA::CDB_SEWA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_SEWA.InitHashTable(HASHSIZESEWA);
}

CDB_SEWA::~CDB_SEWA()
{
}

void CDB_SEWA::Add(T_SEWA_K Key, T_SEWA_D& rData, CDB_ELEM* pElem)
{
	T_SEWA_D Data;
	BOOL bExist = m_SEWA.Lookup(Key, Data);
	m_SEWA.SetAt(Key, rData);
	if ( !bExist && pElem )
	{
		pElem->AddListItem(Key, LT_SEWA_CMD, Key);
	}
}

BOOL CDB_SEWA::Del(T_SEWA_K Key, CDB_ELEM* pElem)
{
	BOOL bRemove = m_SEWA.RemoveKey(Key);
	ASSERT(bRemove);
	if ( bRemove && pElem )
	{
		VERIFY(pElem->DelListItem(Key, LT_SEWA_CMD, Key));
	}
	return bRemove;
}

BOOL CDB_SEWA::Get(T_SEWA_K Key, T_SEWA_D& rData)
{
	if ( m_pDoc == nullptr ) return FALSE;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return FALSE;
	return m_SEWA.Lookup(Key, rData);
}

int CDB_SEWA::GetCount()
{
	if ( m_pDoc == nullptr ) return 0;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return 0;
	return m_SEWA.GetCount();
}

POSITION CDB_SEWA::GetStart()
{
	if ( m_pDoc == nullptr ) return nullptr;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return nullptr;
	return m_SEWA.GetStartPosition();
}

void CDB_SEWA::GetNext(POSITION& rNextPosition, T_SEWA_K& rKey, T_SEWA_D& rData)
{
	if ( m_pDoc == nullptr ) return;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) { ASSERT(0); return; }
	m_SEWA.GetNextAssoc(rNextPosition, rKey, rData);
}