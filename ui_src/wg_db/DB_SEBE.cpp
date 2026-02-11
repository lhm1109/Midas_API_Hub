#include "stdafx.h"
#include "DB_SEBE.h"
#include "DB_ELEM.h"
#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SEBE::CDB_SEBE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_SEBE.InitHashTable(HASHSIZESEBE);
}

CDB_SEBE::~CDB_SEBE()
{
}

void CDB_SEBE::Add(T_SEBE_K Key, T_SEBE_D& rData, CDB_ELEM* pElem)
{
	T_SEBE_D Data;
	BOOL bExist = m_SEBE.Lookup(Key, Data);
	m_SEBE.SetAt(Key, rData);
	if ( !bExist && pElem )
	{
		pElem->AddListItem(Key, LT_SEBE_CMD, Key);
	}
}

BOOL CDB_SEBE::Del(T_SEBE_K Key, CDB_ELEM* pElem)
{
	BOOL bRemove = m_SEBE.RemoveKey(Key);
	ASSERT(bRemove);
	if ( bRemove && pElem )
	{
		VERIFY(pElem->DelListItem(Key, LT_SEBE_CMD, Key));
	}
	return bRemove;
}

BOOL CDB_SEBE::Get(T_SEBE_K Key, T_SEBE_D& rData)
{
	if ( m_pDoc == nullptr ) return FALSE;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return FALSE;
	return m_SEBE.Lookup(Key, rData);
}

int CDB_SEBE::GetCount()
{
	if ( m_pDoc == nullptr ) return 0;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return 0;
	return m_SEBE.GetCount();
}

POSITION CDB_SEBE::GetStart()
{
	if ( m_pDoc == nullptr ) return nullptr;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return nullptr;
	return m_SEBE.GetStartPosition();
}

void CDB_SEBE::GetNext(POSITION& rNextPosition, T_SEBE_K& rKey, T_SEBE_D& rData)
{
	if ( m_pDoc == nullptr ) return;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) { ASSERT(0); return; }
	m_SEBE.GetNextAssoc(rNextPosition, rKey, rData);
}